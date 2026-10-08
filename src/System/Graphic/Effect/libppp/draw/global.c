#include "global.h"
#include "global_w.h"
#include "include_asm.h"

int sceOpen(const char *filename, int flag, ...);
int sceClose(int fd);
int sceRead(int fd, void *buf, int nbyte);
int sceLseek(int fd, int offset, int where);

void opc() {}

void opc2() {}

void opc3() {}

void op_init_global() {}

void ops_init_spr() {
    spr_a[0] = 0x70000000;
    spr_rev_a[0] = 0x70004000;
    spr_index = 0;
    spr_rev_index = 0;
}

void op_num_info(void *gs) {}

void *op_spr_alloc(word mem) {
    word w;
    word spr;
    word spr2;

    spr = spr_a[spr_index++];
    spr_a[spr_index] = spr + mem;
    return (void *)spr;
}

void *op_spr_rev_alloc(word mem) {
    word w;
    word spr;

    spr = spr_rev_a[spr_rev_index++];
    spr_rev_a[spr_rev_index] = spr;
    return (void *)(spr - mem);
}

void op_spr_free(word mem) {
    word w;

    spr_index--;
}

void op_spr_rev_free(word mem) {
    word w;

    spr_rev_index--;
}

int op_spr_rev_check(word mem) {
    word spr;

    if ((word)spr_a + mem <= spr_rev_a[spr_rev_index + 1]) {
        return 0;
    }
    return -1;
}

int op_spr_check(word mem) {
    word spr;

    if (spr_a[spr_index + 1] <= (word)spr_rev_a - mem) {
        return 0;
    }
    return -1;
}

int op_file_read(char *fn, char *adr) {
    int rfd;
    int size;

    rfd = sceOpen(fn, 1);
    if (rfd < 0) {
        *(int *)0 = 0;
        return 0;
    }
    size = sceLseek(rfd, 0, 2);
    sceLseek(rfd, 0, 0);
    if (size != sceRead(rfd, adr, size)) {
        size = 0;
    }
    sceClose(rfd);
    FlushCache(0);
    return size;
}

int op_file_size(char *fn) {
    int rfd;
    int size;

    rfd = sceOpen(fn, 1);
    if (rfd < 0) {
        *(int *)0 = 0;
        return 0;
    }
    size = sceLseek(rfd, 0, 2);
    sceClose(rfd);
    return size;
}

void op_oef2_set(OEF2_HEADER *table, void *data) {
    OEF2_TEXINFO *ti;
    OEF2_CLUTINFO *ci;
    int c;
    char *ss;
    char *dd;
#define D ((OEF2_HEADER *)data)

    if ((u_int)data & 0xF) {
        *(int *)0 = 0;
    }
    if (D->obj_c) {
        ss = (char *)(D->obj + (word)data);
        dd = (char *)(table->obj + (word)table);
        for (c = 0; c < D->obj_c; c++) {
            if (*(u_word *)ss) {
                *(u_word *)dd = *(u_word *)ss + (u_word)data;
            }
            ss += 4;
            dd += 4;
        }
    }
    if (D->texture_c) {
        ss = (char *)(D->texture + (word)data);
        ti = (OEF2_TEXINFO *)(table->texture + (word)table);
        for (c = 0; c < D->texture_c; c++) {
            if (*(u_word *)ss) {
                ti->data = *(u_word *)ss + (u_word)data;
            }
            ss += 4;
            ti++;
        }
    }
    if (D->clut_c) {
        ss = (char *)(D->clut + (word)data);
        ci = (OEF2_CLUTINFO *)(table->clut + (word)table);
        for (c = 0; c < D->clut_c; c++) {
            if (*(u_word *)ss) {
                ci->data = *(u_word *)ss + (u_word)data;
            }
            ss += 4;
            ci++;
        }
    }
    if (D->model_c) {
        ss = (char *)(D->model + (word)data);
        dd = (char *)(table->model + (word)table);
        for (c = 0; c < D->model_c; c++) {
            if (*(u_word *)ss) {
                *(u_word *)dd = *(u_word *)ss + (u_word)data;
            }
            ss += 4;
            dd += 4;
        }
    }
    if (D->particle_c) {
        ss = (char *)(D->particle + (word)data);
        dd = (char *)(table->particle + (word)table);
        for (c = 0; c < D->particle_c; c++) {
            if (*(u_word *)ss) {
                *(u_word *)dd = *(u_word *)ss + (u_word)data;
            }
            ss += 4;
            dd += 4;
        }
    }
    if (D->etc_c) {
        ss = (char *)(D->etc + (word)data);
        dd = (char *)(table->etc + (word)table);
        for (c = 0; c < D->etc_c; c++) {
            if (*(u_word *)ss) {
                *(u_word *)dd = *(u_word *)ss + (u_word)data;
            }
            ss += 4;
            dd += 4;
        }
    }
#undef D
}

void op_oef2_set2(OEF2_HEADER *table) {
    op_oef2_set(table, (char *)table + table->table);
}

void op_oef2_set2_16(char *oef2_16) {
    OEF2_HEADER *table;

    table = (OEF2_HEADER *)(oef2_16 + 0x10);
    op_oef2_set(table, (char *)table + table->table);
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Effect/libppp/draw/global", op_oef2_a_get);

/* Same code except register allocation: the original keeps table in a0 and computes number * 4 last;
 * ours copies table to a3. Tried if/switch forms, pointer/integer arithmetic and temporaries. */
#if 0
void *op_oef2_a_get(OEF2_HEADER *table, int code, int number) {
    OEF2_TEXINFO *ti;
    OEF2_CLUTINFO *ci;
    char *dd;
    word w;

    switch (code) {
    case 2:
        ti = (OEF2_TEXINFO *)(table->texture + (word)table);
        return &ti[number];
    case 3:
        ci = (OEF2_CLUTINFO *)(table->clut + (word)table);
        return &ci[number];
    default:
        w = ((u_word *)table)[code] + (word)table;
        return ((void **)w)[number];
    }
}
#endif

float op_random(float ran) {
    __asm__ __volatile__(
        "mfc1 $v0, $f12\n"
        "lui $v1, 0x4000\n"
        "lui $a0, 0x4040\n"
        "li $a1, 0x3ff19120\n"
        "vrnext.xyzw $vf31, $R\n"
        "qmtc2.ni $v0, $vf30\n"
        "qmtc2.ni $v1, $vf29\n"
        "qmtc2.ni $a0, $vf28\n"
        "qmtc2.ni $a1, $vf27\n"
        "vmul.x $vf31, $vf31, $vf29\n"
        "vsubx.w $vf25, $vf31, $vf27x\n"
        "vsub.x $vf31, $vf31, $vf28\n"
        "vmul.x $vf31, $vf31, $vf30\n"
        "qmfc2.ni $v0, $vf31\n"
        "mtc1 $v0, $f0\n"
        "vrinit $R, $vf25w\n");
}

float op_random_p(float ran) {
    __asm__ __volatile__(
        "mfc1 $v0, $f12\n"
        "lui $v1, 0x3f80\n"
        "li $a1, 0x3ff19120\n"
        "vrnext.xyzw $vf31, $R\n"
        "qmtc2.ni $v0, $vf30\n"
        "qmtc2.ni $v1, $vf29\n"
        "qmtc2.ni $a1, $vf27\n"
        "vsub.xyzw $vf31, $vf31, $vf29\n"
        "vsubx.w $vf25, $vf31, $vf27x\n"
        "vmul.x $vf31, $vf31, $vf30\n"
        "qmfc2.ni $v0, $vf31\n"
        "mtc1 $v0, $f0\n"
        "vrinit $R, $vf25w\n");
}
