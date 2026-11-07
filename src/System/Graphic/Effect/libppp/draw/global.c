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
        /* one variable reassigned step by step: a single expression gives the temporaries other registers */
        w = (word)table + code * 4;
        w = *(u_word *)w;
        w = w + (word)table;
        return ((void **)w)[number];
    }
}

/* MATCH: one asm per VU0 instruction; the float-to-GPR move, the constants and the result move are C. Register locals: r/a/b pin v0/v1/a0 (unpinned: other registers), f pins $f0 so the mtc1 stays before the vrinit (a plain local is combined into the return copy after it). The empty asm keeps the li 0x3ff19120 after the other loads (without it the scheduler issues its lui first, the longer chain) */
float op_random(float ran) {
    register float r asm("$2") = ran;
    register int a asm("$3") = 0x40000000;
    register int b asm("$4") = 0x40400000;
    int c;
    register float f asm("$f0");

    __asm__ __volatile__("");
    c = 0x3ff19120;
    __asm__ __volatile__("vrnext.xyzw $vf31, $R");
    __asm__ __volatile__("qmtc2.ni %0, $vf30" : : "r"(r));
    __asm__ __volatile__("qmtc2.ni %0, $vf29" : : "r"(a));
    __asm__ __volatile__("qmtc2.ni %0, $vf28" : : "r"(b));
    __asm__ __volatile__("qmtc2.ni %0, $vf27" : : "r"(c));
    __asm__ __volatile__("vmul.x $vf31, $vf31, $vf29");
    __asm__ __volatile__("vsubx.w $vf25, $vf31, $vf27x");
    __asm__ __volatile__("vsub.x $vf31, $vf31, $vf28");
    __asm__ __volatile__("vmul.x $vf31, $vf31, $vf30");
    __asm__ __volatile__("qmfc2.ni %0, $vf31" : "=r"(r));
    f = r;
    __asm__ __volatile__("vrinit $R, $vf25w");
    return f;
}

/* MATCH: one asm per VU0 instruction, the moves and constants in C, as op_random: r/a/c pin v0/v1/a1, f pins $f0 (mtc1 before the vrinit), the empty asm keeps the li 0x3ff19120 after the other loads */
float op_random_p(float ran) {
    register float r asm("$2") = ran;
    register int a asm("$3") = 0x3f800000;
    register int c asm("$5");
    register float f asm("$f0");

    __asm__ __volatile__("");
    c = 0x3ff19120;
    __asm__ __volatile__("vrnext.xyzw $vf31, $R");
    __asm__ __volatile__("qmtc2.ni %0, $vf30" : : "r"(r));
    __asm__ __volatile__("qmtc2.ni %0, $vf29" : : "r"(a));
    __asm__ __volatile__("qmtc2.ni %0, $vf27" : : "r"(c));
    __asm__ __volatile__("vsub.xyzw $vf31, $vf31, $vf29");
    __asm__ __volatile__("vsubx.w $vf25, $vf31, $vf27x");
    __asm__ __volatile__("vmul.x $vf31, $vf31, $vf30");
    __asm__ __volatile__("qmfc2.ni %0, $vf31" : "=r"(r));
    f = r;
    __asm__ __volatile__("vrinit $R, $vf25w");
    return f;
}
