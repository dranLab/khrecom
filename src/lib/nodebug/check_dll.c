/* libsn (SN Systems ProDG): relocatable DLL loader */
#include "include_asm.h"

typedef unsigned int u_int;
typedef unsigned short u_short;
typedef unsigned char u_char;
typedef unsigned int size_t;

typedef struct {
    u_int offset; /* address to patch */
    u_int info; /* symbol index << 8 | relocation type */
    u_int addend;
} SN_RELOC;

typedef struct {
    char *name;
    u_int value;
    u_short hash;
    u_char type; /* 0 local, 1 undefined, 2 global, 3 weak, 4 absolute global */
    u_char defined;
} SN_SYMBOL;

typedef struct sn_dll_header {
    u_char magic[4]; /* "SNR1" / "SNR2" */
    SN_RELOC *relocs;
    int nrelocs;
    SN_SYMBOL *syms;
    int nsyms;
    char *name;
    void (*ctor)(void);
    void (*dtor)(void);
    u_int *table;
    int ntable;
    u_int align;
    u_int size;
    struct sn_dll_header *next;
    u_char *packed_relocs;
    char *free_memory;
} SN_DLL_HEADER;

typedef struct {
    u_int name;
    u_int header;
    u_int check;
} SN_RELMOD;

typedef struct {
    u_int count;
    u_int check;
    SN_RELMOD mods[0x400];
} SN_RELMODS;

extern SN_RELMODS SN_LOADED_RELMODS;
extern SN_DLL_HEADER sn_dll_header_root;
extern SN_DLL_HEADER *header_root;
extern int notify;

extern int strcmp(const char *, const char *);
extern void *memcpy(void *, const void *, size_t);
extern int printf(const char *, ...);
extern void snDllCacheFlush(void);
extern void snNotifyDebugger(void);
extern void snAllBreaksOut(void);
extern void snAllBreaksIn(void);
static int relocate_dll(SN_DLL_HEADER *h, u_int delta);
int snDllUnload(SN_DLL_HEADER *h);

#define RELMOD_CHECK 0x31415926

static int check_dll(SN_DLL_HEADER *h)
{
    if ((*(u_int *)h->magic & 0xffffff) != 0x524e53) /* "SNR" */
        return 1;
    if (h->magic[3] != '1' && h->magic[3] != '2')
        return 2;
    if (((h->align - 1) & (u_int)h) != 0)
        return 4;
    return 0;
}

static u_int lookup_symbol(char *name, u_short hash)
{
    SN_DLL_HEADER *h;
    SN_SYMBOL *s;
    int n;

    for (h = header_root; h != 0; h = h->next) {
        for (n = h->nsyms, s = h->syms; n != 0; n--, s++) {
            if (s->defined && s->hash == hash && !strcmp(s->name, name))
                return s->value;
        }
    }
    return 0;
}

static int patch(u_int *addr, u_int type, u_int value)
{
    switch (type) {
    case 0: /* R_MIPS_NONE */
        break;
    case 1: /* R_MIPS_16 */
        *(u_short *)addr = value;
        break;
    case 2: /* R_MIPS_32 */
        *addr = value;
        break;
    case 4: /* R_MIPS_26 */
        if ((*addr & 0xfc00003f) == 0xd) /* break: not to be patched */
            return 0;
        *addr = (*addr & 0xfc000000) | ((value >> 2) & 0x3ffffff);
        break;
    case 5: /* R_MIPS_HI16 */
        if ((*addr & 0xfc00003f) == 0xd)
            return 0;
        *(u_short *)addr = (value + 0x8000) >> 16;
        break;
    case 6: /* R_MIPS_LO16 */
        if ((*addr & 0xfc00003f) == 0xd)
            return 0;
        *(u_short *)addr = value;
        break;
    case 0x77: /* R_MIPS_DVP_U15_S3 */
        *addr = (*addr & 0xffe0003f) | ((value << 3) & 0x1fffc0);
        break;
    case 0x78: /* R_MIPS_DVP_11_PCREL */
        *addr = (*addr & 0xfffff800) | (((value - (u_int)addr) >> 3) & 0x7ff);
        break;
    case 0x79: /* R_MIPS_DVP_27_S4 */
        *addr = (*addr & 0x8000000f) | (value & 0x7ffffff0);
        break;
    case 0x7a: /* R_MIPS_DVP_11_S4 */
        value >>= 4;
        *addr = (*addr & 0xfffff800) | (value & 0x7ff);
        break;
    case 0x7b: /* R_MIPS_DVP_U15_S3 (VU lower) */
        value >>= 3;
        *addr = (*addr & 0xfe1ff800) | (value & 0x7ff) | ((value & 0x7800) << 10);
        break;
    default:
        return 0;
    }
    return 1;
}

static int patch_sym_uses(SN_RELOC *r, u_int n, u_int sym, u_int value)
{
    while (n--) {
        if ((r->info >> 8) == sym) {
            if (!patch((u_int *)r->offset, r->info & 0xff, value + r->addend))
                return 0;
        }
        r++;
    }
    return 1;
}

static int unpatch(u_int *addr, u_int type)
{
    return patch(addr, type, 0);
}

static int unpatch_sym_uses(SN_RELOC *r, u_int n, u_int sym)
{
    while (n--) {
        if ((r->info >> 8) == sym) {
            if (!unpatch((u_int *)r->offset, r->info & 0xff))
                return 0;
        }
        r++;
    }
    return 1;
}

static int define_symbol(SN_SYMBOL *sym)
{
    SN_DLL_HEADER *h;
    SN_SYMBOL *s;
    int n;
    char *name;
    u_short hash;
    u_int value;

    sym->defined = 1;
    hash = sym->hash;
    value = sym->value;
    name = sym->name;
    for (h = header_root; h != 0; h = h->next) {
        for (n = h->nsyms, s = h->syms; n != 0; n--, s++) {
            if (s->hash == hash && !strcmp(s->name, name) && (s->type == 1 || s->type == 3)) {
                if (!patch_sym_uses(h->relocs, h->nrelocs, s - h->syms, value))
                    return 0;
                break;
            }
        }
    }
    return 1;
}

static int define_symbol_global(SN_SYMBOL *sym)
{
    SN_DLL_HEADER *h;
    SN_SYMBOL *s;
    int n;

    for (h = header_root; h != 0; h = h->next) {
        for (n = h->nsyms, s = h->syms; n != 0; n--, s++) {
            if (s->defined && s->hash == sym->hash && !strcmp(s->name, sym->name)) {
                if (s->type == 2 || s->type == 4)
                    return 1;
                s->defined = 0;
                goto define;
            }
        }
    }
define:
    return define_symbol(sym);
}

static int define_symbol_weak(SN_SYMBOL *sym)
{
    SN_DLL_HEADER *h;
    SN_SYMBOL *s;
    int n;

    for (h = header_root; h != 0; h = h->next) {
        for (n = h->nsyms, s = h->syms; n != 0; n--, s++) {
            if (s->defined && s->hash == sym->hash && !strcmp(s->name, sym->name))
                return 1;
        }
    }
    return define_symbol(sym);
}

static int undefine_symbol(char *name, u_short hash)
{
    SN_DLL_HEADER *h;
    SN_SYMBOL *s;
    SN_SYMBOL *weak = 0;
    int n;

    for (h = header_root; h != 0; h = h->next) {
        for (n = h->nsyms, s = h->syms; n != 0; s++, n--) {
            if (s->hash == hash && !strcmp(s->name, name)) {
                switch (s->type) {
                case 2:
                case 4:
                    return define_symbol(s);
                case 3:
                    if (!weak)
                        weak = s;
                    break;
                }
            }
        }
    }
    if (weak)
        return define_symbol(weak);

    for (h = header_root; h != 0; h = h->next) {
        for (n = h->nsyms, s = h->syms; n != 0; n--, s++) {
            if (s->hash == hash && !strcmp(s->name, name) && (s->type == 1 || s->type == 3)) {
                if (!unpatch_sym_uses(h->relocs, h->nrelocs, s - h->syms))
                    return 0;
                break;
            }
        }
    }
    return 1;
}

#define RELOCATE(p) \
    if (p)          \
    (p) = (void *)((char *)(p) + delta)

#if 0
/* Parked: only the scheduling of the HI16 patch() call's arguments differs */
static int relocate_dll(SN_DLL_HEADER *h, u_int delta)
{
    SN_SYMBOL *s;
    SN_RELOC *r;
    int n;
    u_int *t;
    u_char *p;
    u_char *addr;

    RELOCATE(h->relocs);
    RELOCATE(h->syms);
    RELOCATE(h->name);
    RELOCATE(h->ctor);
    RELOCATE(h->dtor);
    RELOCATE(h->table);
    RELOCATE(h->packed_relocs);
    RELOCATE(h->free_memory);
    h->next = 0;

    for (n = h->nsyms, s = h->syms; n != 0; s++, n--) {
        s->name += delta;
        s->defined = 0;
        switch (s->type) {
        default:
            return 7;
        case 0:
            s->value += delta;
            break;
        case 2:
            s->value += delta;
            if (!define_symbol_global(s))
                return 8;
            break;
        case 1:
            s->value = lookup_symbol(s->name, s->hash);
            break;
        case 3:
            s->value += delta;
            define_symbol_weak(s);
            break;
        case 4:
            if (!define_symbol_global(s))
                return 9;
            break;
        }
    }

    /* the packed local relocations: a byte stream of (offset step | type) */
    if (h->packed_relocs != 0) {
        p = h->packed_relocs;
        addr = 0;
        for (;;) {
            int type = *p & 3;
            int c = *p++;
            c &= 0xfc;
            if (c == 0) {
                u_int a = p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
                p += 4;
                if (a == 0)
                    break;
                addr = (u_char *)(a + delta);
            } else {
                addr += c;
            }
            switch (type) {
            case 0:
                patch((u_int *)addr, 6, delta + *(u_short *)addr);
                break;
            case 1: {
                /* HI16: the low half is in the next LO16 entry */
                u_int hi = *(u_short *)addr << 16;
                u_char *q = p;
                u_char *lo = addr;
                int t2;
                do {
                    int c2;
                    t2 = *q & 3;
                    c2 = *q++;
                    if ((c2 & ~3) == 0) {
                        u_int a = q[0] | (q[1] << 8) | (q[2] << 16) | (q[3] << 24);
                        q += 4;
                        if (a == 0)
                            return 10;
                        lo = (u_char *)(a + delta);
                    } else {
                        lo += c2 & ~3;
                    }
                } while (t2 != 0);
                hi += *(short *)lo;
                patch((u_int *)addr, 5, delta + hi);
            } break;
            case 2:
                patch((u_int *)addr, 4, delta + ((*(u_int *)addr & 0x3ffffff) << 2));
                break;
            case 3:
                patch((u_int *)addr, 2, delta + *(u_int *)addr);
                break;
            }
        }
    }

    for (n = h->nrelocs, r = h->relocs; n != 0; n--, r++) {
        r->offset = delta + r->offset;
        if (!patch((u_int *)r->offset, r->info & 0xff, h->syms[r->info >> 8].value + r->addend))
            return 10;
    }

    for (n = h->ntable, t = h->table; n != 0; n--, t++) {
        if (*t)
            *t += delta;
    }

    {
        SN_DLL_HEADER *last = header_root;
        while (last->next)
            last = last->next;
        last->next = h;
    }
    snDllCacheFlush();
    return 0;
}
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/check_dll", relocate_dll);
asm(".rdata\n.align 3\njtbl_004FB300:\n"
    ".word .L00361054\n.word .L00361088\n.word .L00361064\n.word .L0036109C\n.word .L003610B8\n"
    ".text\n");

static int add_relmod(u_int name, u_int header)
{
    u_int i;

    for (i = 0; i < SN_LOADED_RELMODS.count; i++) {
        if (SN_LOADED_RELMODS.mods[i].header == 0) {
            SN_LOADED_RELMODS.mods[i].name = name;
            SN_LOADED_RELMODS.mods[i].header = header;
            SN_LOADED_RELMODS.mods[i].check = name ^ header ^ RELMOD_CHECK;
            return i;
        }
    }
    if (SN_LOADED_RELMODS.count == 0x400)
        return -1;
    i = SN_LOADED_RELMODS.count++;
    SN_LOADED_RELMODS.mods[i].name = name;
    SN_LOADED_RELMODS.mods[i].header = header;
    SN_LOADED_RELMODS.mods[i].check = name ^ header ^ RELMOD_CHECK;
    SN_LOADED_RELMODS.check = SN_LOADED_RELMODS.count ^ RELMOD_CHECK;
    return i;
}

static int remove_relmod(u_int header)
{
    u_int i;

    for (i = 0; i < SN_LOADED_RELMODS.count; i++) {
        if (SN_LOADED_RELMODS.mods[i].header == header) {
            SN_LOADED_RELMODS.mods[i].name = 0;
            SN_LOADED_RELMODS.mods[i].header = 0;
            SN_LOADED_RELMODS.mods[i].check = 0;
            return i;
        }
    }
    return -1;
}

int snInitDllSystem(u_int *ptable)
{
    SN_SYMBOL *s;
    int n;

    SN_LOADED_RELMODS.check = RELMOD_CHECK;
    SN_LOADED_RELMODS.count = 0;
    if (ptable)
        *ptable = (u_int)sn_dll_header_root.table;

    for (n = header_root->nsyms, s = header_root->syms; n != 0; n--, s++) {
        switch (s->type) {
        default:
            return 7;
        case 0:
        case 1:
            s->defined = 0;
            break;
        case 2:
        case 3:
        case 4:
            s->defined = 1;
            break;
        }
    }
    return 0;
}

int snDllLoaded(SN_DLL_HEADER *h, u_int *ptable)
{
    int err;
    int idx;

    if (ptable)
        *ptable = 0;
    if ((err = check_dll(h)) != 0)
        return err;
    snAllBreaksOut();
    if ((err = relocate_dll(h, (u_int)h)) != 0) {
        snAllBreaksIn();
        return err;
    }
    if (ptable && h->ntable)
        *ptable = (u_int)h->table;
    idx = add_relmod((u_int)h->name, (u_int)h);
    err = idx < 0 ? 6 : 0;
    snAllBreaksIn();
    if (err)
        return err;
    if (notify == 0) {
        snNotifyDebugger();
    } else if (notify == 1) {
        printf("###DLL:Loaded %08x %08x %08x %08x %s\n", idx, SN_LOADED_RELMODS.mods[idx].name,
               SN_LOADED_RELMODS.mods[idx].header, SN_LOADED_RELMODS.mods[idx].check, SN_LOADED_RELMODS.mods[idx].name);
    }
    if (h->ctor)
        h->ctor();
    return 0;
}

#if 0
/* Parked: only the register of notify's address differs */
int snDllUnload(SN_DLL_HEADER *h)
{
    SN_DLL_HEADER **pp;
    SN_SYMBOL *s;
    int n;
    int idx;

    for (pp = &header_root; *pp != 0; pp = &(*pp)->next) {
        if (*pp == h)
            break;
    }
    if (*pp == 0)
        return 5;
    *pp = (*pp)->next;

    if (h->dtor)
        h->dtor();
    if ((idx = remove_relmod((u_int)h)) < 0)
        return 11;
    snAllBreaksOut();
    for (n = h->nsyms, s = h->syms; n != 0; n--, s++) {
        if (s->defined)
            undefine_symbol(s->name, s->hash);
    }
    snDllCacheFlush();
    snAllBreaksIn();
    if (notify == 0) {
        snNotifyDebugger();
    } else if (notify == 1) {
        printf("###DLL:Unloaded %08x\n", idx);
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/check_dll", snDllUnload);

/* keeps snDllUnload's string in this object's .rodata while the function is asm */
static inline const char *snDllUnload_string(void)
{
    return "###DLL:Unloaded %08x\n";
}

int snDllMove(SN_DLL_HEADER *dst, SN_DLL_HEADER *src, u_int *ptable)
{
    int err;
    int idx;

    if (ptable)
        *ptable = 0;
    if (((char *)src)[3] != '1')
        return 2;
    if ((err = snDllUnload(src)) != 0)
        return err;
    memcpy(dst, src, src->size);
    if ((err = check_dll(dst)) != 0)
        return err;
    snAllBreaksOut();
    if ((err = relocate_dll(dst, (u_int)dst - (u_int)src)) != 0) {
        snAllBreaksIn();
        return err;
    }
    if (dst->ctor)
        dst->ctor();
    if (ptable)
        *ptable = (u_int)dst->table;
    idx = add_relmod((u_int)dst->name, (u_int)dst);
    err = idx < 0 ? 6 : 0;
    snAllBreaksIn();
    if (err)
        return err;
    if (notify == 0) {
        snNotifyDebugger();
    } else if (notify == 1) {
        printf("###DLL:Loaded %08x %08x %08x %08x %s\n", idx, SN_LOADED_RELMODS.mods[idx].name,
               SN_LOADED_RELMODS.mods[idx].header, SN_LOADED_RELMODS.mods[idx].check, SN_LOADED_RELMODS.mods[idx].name);
    }
    return 0;
}

u_int snDllGetFunctionAddress(char *name)
{
    u_short hash = 0;
    char *p;

    for (p = name; *p; p++)
        hash = hash * 31 + *p;
    return lookup_symbol(name, hash);
}

void snDllNotifyDebuggerAndPause(void)
{
    notify = 0;
}

void snDllNotifyDebuggerAndContinue(void)
{
    notify = 1;
}

void snDllNoNotifyDebugger(void)
{
    notify = 2;
}

char *snDllGetFreeMemory(SN_DLL_HEADER *h)
{
    if (h->free_memory)
        return h->free_memory;
    return (char *)h + h->size;
}
// SOURCE DATA: .rodata
