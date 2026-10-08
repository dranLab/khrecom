#include "KeDttSpr.h"
#include "System/Graphic/Effect/libppp/draw/draw_w.h"
#include "include_asm.h"

void KeDttSpr_MkGifPk_Pre(KeaDttSpr_MkGifPk *a) {
    struct {
        u_long p[16];
        int ofs[2];
        int pad0[2];
    } *w;

    w = op_spr_alloc(sizeof(*w));
    w->p[0] = 0x4000000000000001;
    w->p[1] = 0xEEEE;
    w->p[2] = (a->TEX0 & 0xFFFFFFFC03FFFFFF) | 0x2A8000000;
    w->p[3] = 0x06;
    if (a->_Pad0[0] & 4) {
        w->p[4] = 0x31001;
    } else {
        w->p[4] = 0x71001;
    }
    w->p[5] = 0x47;
    w->p[6] = 0x44;
    w->p[7] = 0x42;
    w->p[8] = 2 | (2 << 2) | ((u_long)((sceGsScissor *)&a->SCISSOR)->SCAX0 << 4) |
              (((u_long)((sceGsScissor *)&a->SCISSOR)->SCAX1 - 1) << 14) |
              ((u_long)((sceGsScissor *)&a->SCISSOR)->SCAY0 << 24) |
              (((u_long)((sceGsScissor *)&a->SCISSOR)->SCAY1 - 1) << 34);
    w->p[9] = 0x08;
    w->p[10] = 0x302EC0000000000A;
    w->p[11] = 0x412;
    w->p[12] = 0x1000000000008001;
    w->p[13] = 0x0E;
    ((int *)&w->p[14])[0] = ((int *)&os_gs2_register.test)[0];
    ((int *)&w->p[14])[1] = ((int *)&os_gs2_register.test)[1];
    w->p[15] = 0x47;
    {
        u_long t0 = (u_long)((sceGsXyoffset *)&a->XYOFFSET)->OFX - 8;
        w->ofs[0] = t0;
    }
    {
        u_long t1 = (u_long)((sceGsXyoffset *)&a->XYOFFSET)->OFY - 8;
        w->ofs[1] = t1;
    }
    __asm__ __volatile__(
        "lqc2 $vf25, 0x0(%0)\n"
        "lqc2 $vf27, 0x10(%0)\n"
        "lqc2 $vf29, 0x20(%0)\n"
        "lqc2 $vf21, 0x30(%0)\n"
        "lqc2 $vf14, 0x40(%0)\n"
        "lqc2 $vf20, 0x50(%0)\n"
        "lqc2 $vf9, 0x60(%0)\n"
        "lqc2 $vf3, 0x70(%0)\n"
        "lqc2 $vf2, 0x0(%2)\n"
        "lqc2 $vf31, 0x10(%2)\n"
        "vadd.y $vf2, $vf0, $vf31\n"
        "lui $v0, 0x3a80\n"
        "qmtc2.ni $v0, $vf24\n"
        "lqc2 $vf4, 0x0(%1)\n"
        "lqc2 $vf5, 0x10(%1)\n"
        "lqc2 $vf6, 0x20(%1)\n"
        "lqc2 $vf7, 0x30(%1)\n"
        "lqc2 $vf11, 0x80(%0)\n"
        "vitof4.xyzw $vf11, $vf11\n"
        "lqc2 $vf31, 0x50(%3)\n"
        "vmove.x $vf15, $vf0\n"
        "vaddw.y $vf15, $vf0, $vf31w\n"
        "vaddz.x $vf17, $vf0, $vf31z\n"
        "vmove.y $vf17, $vf0\n"
        "dli $v0, 0x3f3504f33f3504f3\n"
        "qmtc2.ni $v0, $vf16\n"
        "vmul.xy $vf16, $vf16, $vf2\n"
        "lqc2 $vf1, 0x30(%3)\n"
        "vmul.xy $vf1, $vf2, $vf1\n"
        "vmulz.xy $vf18, $vf2, $vf1z\n"
        "vmulz.xy $vf19, $vf16, $vf1z\n"
        "vmul.x $vf16, $vf16, $vf17\n"
        "vmul.y $vf16, $vf16, $vf15\n"
        "vmul.y $vf15, $vf15, $vf2\n"
        "vmul.x $vf17, $vf17, $vf2\n"
        "lui $v0, 0x4580\n"
        "qmtc2.ni $v0, $vf28\n"
        "vaddx.y $vf28, $vf0, $vf28x\n"
        "vaddw.z $vf26, $vf0, $vf0w\n"
        "vsubw.w $vf30, $vf0, $vf0w\n"
        :
        : "r"(w), "r"(a->LSMat), "r"(a->VSMat), "r"(a)
        : "$2", "memory");
    op_spr_free(sizeof(*w));
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Effect/libppp/keProg/KeDttSpr", KeDttSpr_MkGifPk);

/* All but the tail matches: the original puts the 0x80 constant in a0, computes the new Pk into v0 and
 * stores Pk before Z. Tried return/branch shapes, store orders, register variables and clobber sets. */
#if 0
int KeDttSpr_MkGifPk(KeaDttSpr_MkGifPk *a) {
    sceVu0FVECTOR zz;
    register KeaDttSpr_MkGifPk *ra asm("$6") = a;
    u_long *p = ra->Pk;
    register int i asm("$5");
    int z;

    __asm__ __volatile__(
        "lqc2 $vf8, 0x0(%2)\n"
        "vmulax.xyzw $ACC, $vf4, $vf8x\n"
        "vmadday.xyzw $ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "vdiv $Q, $vf0w, $vf12w\n"
        "vwaitq\n"
        "vmulq.xyz $vf12, $vf12, $Q\n"
        "vmulq.y $vf15, $vf15, $Q\n"
        "vmulq.xy $vf16, $vf16, $Q\n"
        "vmulq.x $vf17, $vf17, $Q\n"
        "vmove.zw $vf10, $vf12\n"
        "vsub.x $vf10, $vf12, $vf17\n"
        "vsub.y $vf10, $vf12, $vf15\n"
        "vadd.x $vf13, $vf17, $vf17\n"
        "vadd.y $vf13, $vf15, $vf15\n"
        "vsub.xy $vf13, $vf28, $vf13\n"
        "ctc2.ni $0, $vi16\n"
        "vsub.xyw $vf0, $vf10, $vf0\n"
        "vsub.xy $vf0, $vf13, $vf10\n"
        "vnop\n"
        "vnop\n"
        "vnop\n"
        "vnop\n"
        "vnop\n"
        "cfc2.ni %0, $vi16\n"
        "andi %0, %0, 0x80\n"
        "bnez %0, 1f\n"
        "vnop\n"
        "vmulq.xy $vf22, $vf18, $Q\n"
        "vmulq.xy $vf23, $vf19, $Q\n"
        "vmulq.xy $vf31, $vf1, $Q\n"
        "vadd.xy $vf11, $vf11, $vf31\n"
        "lqc2 $vf31, 0x10(%2)\n"
        "vftoi0.xyzw $vf31, $vf31\n"
        "sqc2 $vf31, 0x70(%3)\n"
        "lb $v0, 0x3c(%2)\n"
        "andi $v0, $v0, 0x1\n"
        "bnez $v0, 2f\n"
        "nop\n"
        "vsub.w $vf31, $vf0, $vf0\n"
        "2:\n"
        "sqc2 $vf25, 0x0(%3)\n"
        "sqc2 $vf27, 0x10(%3)\n"
        "sqc2 $vf29, 0x20(%3)\n"
        "sqc2 $vf21, 0x30(%3)\n"
        "sqc2 $vf14, 0x40(%3)\n"
        "sqc2 $vf20, 0x50(%3)\n"
        "sqc2 $vf31, 0xa0(%3)\n"
        "sqc2 $vf31, 0xd0(%3)\n"
        "sqc2 $vf31, 0x100(%3)\n"
        "sqc2 $vf31, 0x130(%3)\n"
        "sqc2 $vf31, 0x160(%3)\n"
        "sqc2 $vf31, 0x190(%3)\n"
        "sqc2 $vf31, 0x1c0(%3)\n"
        "sqc2 $vf31, 0x1f0(%3)\n"
        "sqc2 $vf31, 0x220(%3)\n"
        "sqc2 $vf9, 0x240(%3)\n"
        "sqc2 $vf3, 0x250(%3)\n"
        "vftoi4.xyz $vf30, $vf12\n"
        "sqc2 $vf30, 0x80(%3)\n"
        "vsub.xy $vf30, $vf12, $vf11\n"
        "vmulx.xy $vf26, $vf30, $vf24x\n"
        "sqc2 $vf26, 0x60(%3)\n"
        "vadd.xy $vf10, $vf12, $vf15\n"
        "vftoi4.xyz $vf30, $vf10\n"
        "sqc2 $vf30, 0xb0(%3)\n"
        "sqc2 $vf30, 0x230(%3)\n"
        "vsub.xy $vf30, $vf10, $vf11\n"
        "vadd.y $vf30, $vf30, $vf22\n"
        "vmulx.xy $vf26, $vf30, $vf24x\n"
        "sqc2 $vf26, 0x90(%3)\n"
        "sqc2 $vf26, 0x210(%3)\n"
        "vadd.xy $vf10, $vf12, $vf16\n"
        "vftoi4.xyz $vf30, $vf10\n"
        "sqc2 $vf30, 0xe0(%3)\n"
        "vsub.xy $vf30, $vf10, $vf11\n"
        "vadd.xy $vf30, $vf30, $vf23\n"
        "vmulx.xy $vf26, $vf30, $vf24x\n"
        "sqc2 $vf26, 0xc0(%3)\n"
        "vadd.xy $vf10, $vf12, $vf17\n"
        "vftoi4.xyz $vf30, $vf10\n"
        "sqc2 $vf30, 0x110(%3)\n"
        "vsub.xy $vf30, $vf10, $vf11\n"
        "vadd.x $vf30, $vf30, $vf22\n"
        "vmulx.xy $vf26, $vf30, $vf24x\n"
        "sqc2 $vf26, 0xf0(%3)\n"
        "vadd.x $vf10, $vf12, $vf16\n"
        "vsub.y $vf10, $vf12, $vf16\n"
        "vftoi4.xyz $vf30, $vf10\n"
        "sqc2 $vf30, 0x140(%3)\n"
        "vsub.xy $vf30, $vf10, $vf11\n"
        "vadd.x $vf30, $vf30, $vf23\n"
        "vsub.y $vf30, $vf30, $vf23\n"
        "vmulx.xy $vf26, $vf30, $vf24x\n"
        "sqc2 $vf26, 0x120(%3)\n"
        "vsub.xy $vf10, $vf12, $vf15\n"
        "vftoi4.xyz $vf30, $vf10\n"
        "sqc2 $vf30, 0x170(%3)\n"
        "vsub.xy $vf30, $vf10, $vf11\n"
        "vsub.y $vf30, $vf30, $vf22\n"
        "vmulx.xy $vf26, $vf30, $vf24x\n"
        "sqc2 $vf26, 0x150(%3)\n"
        "vsub.xy $vf10, $vf12, $vf16\n"
        "vftoi4.xyz $vf30, $vf10\n"
        "sqc2 $vf30, 0x1a0(%3)\n"
        "vsub.xy $vf30, $vf10, $vf11\n"
        "vsub.xy $vf30, $vf30, $vf23\n"
        "vmulx.xy $vf26, $vf30, $vf24x\n"
        "sqc2 $vf26, 0x180(%3)\n"
        "vsub.xy $vf10, $vf12, $vf17\n"
        "vftoi4.xyz $vf30, $vf10\n"
        "sqc2 $vf30, 0x1d0(%3)\n"
        "vsub.xy $vf30, $vf10, $vf11\n"
        "vsub.x $vf30, $vf30, $vf22\n"
        "vmulx.xy $vf26, $vf30, $vf24x\n"
        "sqc2 $vf26, 0x1b0(%3)\n"
        "vsub.x $vf10, $vf12, $vf16\n"
        "vadd.y $vf10, $vf12, $vf16\n"
        "vftoi4.xyz $vf30, $vf10\n"
        "sqc2 $vf30, 0x200(%3)\n"
        "vsub.xy $vf30, $vf10, $vf11\n"
        "vsub.x $vf30, $vf30, $vf23\n"
        "vadd.y $vf30, $vf30, $vf23\n"
        "vmulx.xy $vf26, $vf30, $vf24x\n"
        "sqc2 $vf26, 0x1e0(%3)\n"
        "sqc2 $vf12, %1\n"
        "add %0, $0, $0\n"
        "1:\n"
        : "=r"(i), "=m"(zz), "+r"(ra)
        : "r"(p)
        : "$2", "$3", "$4", "memory");
    z = ((int *)zz)[2];
    if (i != 0x80) {
        ra->Pk = p + 0x4C;
        ra->Z = z;
        return 1;
    }
    return 0;
}
#endif
