#include "Ke.h"
#include <math.h>

int KeRnd_Value = 0;

signed char KeCltSft[32] __attribute__((aligned(16))) = {
    1, 1, 1, 1, 1, 1, 1, 9, 1, 1, 1, 1, 1, 1, 1, -15, 1, 1, 1, 1, 1, 1, 1, 9, 1, 1, 1, 1, 1, 1, 1, 1,
};

signed char KeCltOfs[32] = {
    0, 1, 2, 3, 4, 5, 6, 7, 16, 17, 18, 19, 20, 21, 22, 23, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25, 26, 27, 28, 29, 30, 31,
};

sceVu0FVECTOR KeVec0001 = {0.f, 0.f, 0.f, 1.f};

sceVu0FVECTOR KeVec1001 = {1.f, 0.f, 0.f, 1.f};

sceVu0FVECTOR KeVec0101 = {0.f, 1.f, 0.f, 1.f};

sceVu0FVECTOR KeVec0011 = {0.f, 0.f, 1.f, 1.f};

sceVu0FMATRIX KeTestMat __attribute__((section(".bss")));

int KeGetDeg2(float *xy, float *deg) {
    float f = xy[0] * xy[1];

    if (f > 0.0f) {
        f = (xy[0] > 0.0f) ? 0.0f : -3.1415927f;
        f = f + atan(xy[1] / xy[0]);
    } else if (f < 0.0f) {
        f = (xy[1] > 0.0f) ? 1.5707964f : -1.5707964f;
        f = f - atan(xy[0] / xy[1]);
    } else if (xy[0] != 0.0f) {
        f = (xy[0] > 0.0f) ? 0.0f : 3.1415927f;
    } else if (xy[1] != 0.0f) {
        f = (xy[1] > 0.0f) ? 1.5707964f : -1.5707964f;
    } else {
        return 0;
    }
    *deg = f;
    return 1;
}

int KeRnd() {
    KeRnd_Value = (KeRnd_Value * 0x80D + 7) & 0xFFFF;
    return KeRnd_Value;
}

int KeVu0RndI() {
    float f;

    __asm__ __volatile__(
        "addi $8, $0, -1\n"
        "qmtc2.ni $8, $vf2\n"
        "vrxor $R, $vf2x\n"
        "vitof0.x $vf2, $vf2\n"
        "vrnext.x $vf1, $R\n"
        "vadd.x $vf1, $vf1, $vf2\n"
        "qmfc2.ni %0, $vf1\n"
        : "=r"(f)
        :
        : "$8");
    return f * 32768.0f;
}


/* The tail (-b and a + a computed before the d < 0 test) matches; the dot products differ in load order
 * and register allocation. Tried all orders of the a/b/c statements. */
int KeVu0ItrsBallLine(KeaVu0ItrsBallLine *_a) {
    float a;
    float b;
    float c;
    float d;

    a = _a->LineDist[0] * _a->LineDist[0];
    a += _a->LineDist[1] * _a->LineDist[1];
    a += _a->LineDist[2] * _a->LineDist[2];
    b = _a->LineDist[0] * _a->LinePos[0];
    b += _a->LineDist[1] * _a->LinePos[1];
    b += _a->LineDist[2] * _a->LinePos[2];
    c = _a->LinePos[0] * _a->LinePos[0];
    c += _a->LinePos[1] * _a->LinePos[1];
    c += _a->LinePos[2] * _a->LinePos[2];
    c -= _a->BallR * _a->BallR;
    b = b + b;
    d = b * b - 4.0f * a * c;
    c = -b;
    b = a + a;
    if (d < 0.0f) {
        return 0;
    }
    _a->T = (c - KeVu0Sqrt(d)) / b;
    return 1;
}

float KeVu0LowClamp(float s, float low) {
    sceVu0FVECTOR v;

    v[0] = s;
    v[1] = low;
    __asm__ __volatile__(
        "lqc2 $vf1, %0\n"
        "vmaxy.x $vf1, $vf1, $vf1y\n"
        "sqc2 $vf1, %0\n"
        : "+m"(v));
    return v[0];
}

void KeVu0CopyVecXY(float *d, float *s) {
    __asm__ __volatile__(
        "lqc2 $vf1, 0x0(%0)\n"
        "lqc2 $vf2, 0x0(%1)\n"
        "vmove.xy $vf1, $vf2\n"
        "sqc2 $vf31, 0x0(%0)\n"
        :
        : "r"(d), "r"(s)
        : "memory");
}

void KeVu0CopyVecXYZ(float *d, float *s) {
    __asm__ __volatile__(
        "lqc2 $vf1, 0x0(%0)\n"
        "lqc2 $vf2, 0x0(%1)\n"
        "vmove.xyz $vf1, $vf2\n"
        "sqc2 $vf1, 0x0(%0)\n"
        :
        : "r"(d), "r"(s)
        : "memory");
}

void KeVu0ScaleVecXYZ(float *d, float *s, float v) {
    __asm__ __volatile__(
        "swc1 %2, 0x0(%0)\n"
        "lqc2 $vf1, 0x0(%0)\n"
        "lqc2 $vf2, 0x0(%1)\n"
        "vmulx.xyz $vf2, $vf2, $vf1x\n"
        "sqc2 $vf2, 0x0(%0)\n"
        :
        : "r"(d), "r"(s), "f"(v)
        : "memory");
}

void KeVu0ScaleVecXY(float *d, float *s, float v) {
    __asm__ __volatile__(
        "swc1 %2, 0x0(%0)\n"
        "lqc2 $vf1, 0x0(%0)\n"
        "lqc2 $vf2, 0x0(%1)\n"
        "vmulx.xy $vf2, $vf2, $vf1x\n"
        "sqc2 $vf2, 0x0(%0)\n"
        :
        : "r"(d), "r"(s), "f"(v)
        : "memory");
}

void KeVu0AddVecXY(float *d, float *t, float *s) {
    __asm__ __volatile__(
        "lqc2 $vf1, 0x0(%1)\n"
        "lqc2 $vf2, 0x0(%2)\n"
        "lqc2 $vf3, 0x0(%0)\n"
        "vadd.xy $vf3, $vf1, $vf2\n"
        "sqc2 $vf3, 0x0(%0)\n"
        :
        : "r"(d), "r"(t), "r"(s)
        : "memory");
}

void KeVu0SubVecXYZ(float *d, float *t, float *s) {
    __asm__ __volatile__(
        "lqc2 $vf1, 0x0(%1)\n"
        "lqc2 $vf2, 0x0(%2)\n"
        "lqc2 $vf3, 0x0(%0)\n"
        "vsub.xyz $vf3, $vf1, $vf2\n"
        "sqc2 $vf3, 0x0(%0)\n"
        :
        : "r"(d), "r"(t), "r"(s)
        : "memory");
}

void KeVu0AddVecXYZ(float *d, float *t, float *s) {
    __asm__ __volatile__(
        "lqc2 $vf1, 0x0(%1)\n"
        "lqc2 $vf2, 0x0(%2)\n"
        "lqc2 $vf3, 0x0(%0)\n"
        "vadd.xyz $vf3, $vf1, $vf2\n"
        "sqc2 $vf3, 0x0(%0)\n"
        :
        : "r"(d), "r"(t), "r"(s)
        : "memory");
}

void KeVu0SubVecXY(float *d, float *t, float *s) {
    __asm__ __volatile__(
        "lqc2 $vf1, 0x0(%1)\n"
        "lqc2 $vf2, 0x0(%2)\n"
        "lqc2 $vf3, 0x0(%0)\n"
        "vsub.xy $vf3, $vf1, $vf2\n"
        "sqc2 $vf3, 0x0(%0)\n"
        :
        : "r"(d), "r"(t), "r"(s)
        : "memory");
}

/* MATCH: one asm per VU0 instruction; the constant, the flag test and the branch are C. `li %0, 0` stays asm: the original's dead load (its delay-slot instruction written outside .set noreorder, so the assembler put a nop in the slot and the li runs on the fall-through, overwritten at once); C drops a dead store (`f = 0;`: 2 lines, reorg then fills the slot with the li 0x8000) */
void KeVu0PacAddVecXY(int *d, float *t, float *s) {
    int f;

    __asm__ __volatile__("lqc2 $vf1, 0x0(%0)" : : "r"(t) : "memory");
    __asm__ __volatile__("lqc2 $vf2, 0x0(%0)" : : "r"(s) : "memory");
    __asm__ __volatile__("vadd.xy $vf1, $vf1, $vf2");
    __asm__ __volatile__("vftoi4.xy $vf31, $vf1");
    __asm__ __volatile__("vftoi0.zw $vf31, $vf1");
    __asm__ __volatile__("qmtc2.ni %0, $vf29" : : "r"((u_long)0x4580000045800000));
    __asm__ __volatile__("ctc2.ni $0, $vi16");
    __asm__ __volatile__("vsub.xyw $vf0, $vf1, $vf0");
    __asm__ __volatile__("vsub.xy $vf0, $vf29, $vf1");
    __asm__ __volatile__("vnop");
    __asm__ __volatile__("vnop");
    __asm__ __volatile__("vnop");
    __asm__ __volatile__("vnop");
    __asm__ __volatile__("cfc2.ni %0, $vi16" : "=r"(f));
    if (f & 0xc0) {
        __asm__ __volatile__("li %0, 0" : "=r"(f));
        __asm__ __volatile__("ctc2.ni %0, $vi1" : : "r"(0x8000));
        __asm__ __volatile__("vmfir.w $vf31, $vi1");
    }
    __asm__ __volatile__("sqc2 $vf31, 0x0(%0)" : : "r"(d) : "memory");
}

/* MATCH: one asm per VU0 instruction; the constant, the flag test and the branch are C. `li %0, 0` stays asm: the original's dead load (its delay-slot instruction written outside .set noreorder, so the assembler put a nop in the slot and the li runs on the fall-through, overwritten at once); C drops a dead store (`f = 0;`: 2 lines, reorg then fills the slot with the li 0x8000) */
void KeVu0PacVec(int *d, float *s) {
    int f;

    __asm__ __volatile__("lqc2 $vf1, 0x0(%0)" : : "r"(s) : "memory");
    __asm__ __volatile__("vftoi4.xy $vf2, $vf1");
    __asm__ __volatile__("vftoi0.zw $vf2, $vf1");
    __asm__ __volatile__("qmtc2.ni %0, $vf29" : : "r"((u_long)0x4580000045800000));
    __asm__ __volatile__("ctc2.ni $0, $vi16");
    __asm__ __volatile__("vsub.xyw $vf0, $vf1, $vf0");
    __asm__ __volatile__("vsub.xy $vf0, $vf29, $vf1");
    __asm__ __volatile__("vnop");
    __asm__ __volatile__("vnop");
    __asm__ __volatile__("vnop");
    __asm__ __volatile__("vnop");
    __asm__ __volatile__("cfc2.ni %0, $vi16" : "=r"(f));
    if (f & 0xc0) {
        __asm__ __volatile__("li %0, 0" : "=r"(f));
        __asm__ __volatile__("ctc2.ni %0, $vi1" : : "r"(0x8000));
        __asm__ __volatile__("vmfir.w $vf2, $vi1");
    }
    __asm__ __volatile__("sqc2 $vf2, 0x0(%0)" : : "r"(d) : "memory");
}

void KeVu0NormXY(float *d, float *s) {
    __asm__ __volatile__(
        "lqc2 $vf1, 0x0(%1)\n"
        "vmul.xy $vf31, $vf1, $vf1\n"
        "vaddy.x $vf31, $vf31, $vf31y\n"
        "vsqrt $Q, $vf31x\n"
        "vwaitq\n"
        "vaddq.x $vf31, $vf0, $Q\n"
        "vdiv $Q, $vf0w, $vf31x\n"
        "vwaitq\n"
        "vmulq.xy $vf31, $vf1, $Q\n"
        "sqc2 $vf31, 0x0(%0)\n"
        :
        : "r"(d), "r"(s)
        : "memory");
}

/* MATCH: one asm per VU0 instruction, the constant in C. The andi stays asm: its result is dead in the original (void function) and C drops it (`f &= 0xc0;`: 1 line). vmaddaz, vmaddw and vdiv share one line: Ps2EeAs pads a vdiv within two instructions of a line label with nops (on separate lines: 2 nops) */
void KeVu0RTPersF(float *d, float *m[4], float *s) {
    int f;

    __asm__ __volatile__("lqc2 $vf1, 0x0(%0)" : : "r"(m) : "memory");
    __asm__ __volatile__("lqc2 $vf2, 0x10(%0)" : : "r"(m) : "memory");
    __asm__ __volatile__("lqc2 $vf3, 0x20(%0)" : : "r"(m) : "memory");
    __asm__ __volatile__("lqc2 $vf4, 0x30(%0)" : : "r"(m) : "memory");
    __asm__ __volatile__("lqc2 $vf5, 0x0(%0)" : : "r"(s) : "memory");
    __asm__ __volatile__("vmulax.xyzw $ACC, $vf1, $vf5x");
    __asm__ __volatile__("vmadday.xyzw $ACC, $vf2, $vf5y");
    __asm__ __volatile__("vmaddaz.xyzw $ACC, $vf3, $vf5z"); __asm__ __volatile__("vmaddw.xyzw $vf5, $vf4, $vf5w"); __asm__ __volatile__("vdiv $Q, $vf0w, $vf5w");
    __asm__ __volatile__("qmtc2.ni %0, $vf29" : : "r"((u_long)0x4580000045800000));
    __asm__ __volatile__("vwaitq");
    __asm__ __volatile__("vmulq.xyz $vf5, $vf5, $Q");
    __asm__ __volatile__("vnop");
    __asm__ __volatile__("vnop");
    __asm__ __volatile__("vnop");
    __asm__ __volatile__("ctc2.ni $0, $vi16");
    __asm__ __volatile__("vsub.xyw $vf0, $vf5, $vf0");
    __asm__ __volatile__("vsub.xy $vf0, $vf29, $vf5");
    __asm__ __volatile__("vnop");
    __asm__ __volatile__("vnop");
    __asm__ __volatile__("vnop");
    __asm__ __volatile__("vnop");
    __asm__ __volatile__("cfc2.ni %0, $vi16" : "=r"(f));
    __asm__ __volatile__("andi %0, %0, 0xc0" : "+r"(f));
    __asm__ __volatile__("sqc2 $vf5, 0x0(%0)" : : "r"(d) : "memory");
}

void KeVu0PacPers(int *dst, float *src) {
    __asm__ __volatile__(
        "lqc2 $vf10, 0x0(%1)\n"
        "nop\n"
        "vdiv $Q, $vf0w, $vf10w\n"
        "vwaitq\n"
        "vmulq.xyz $vf9, $vf10, $Q\n"
        "vftoi4.xy $vf9, $vf9\n"
        "vftoi0.z $vf9, $vf9\n"
        "vftoi0.w $vf9, $vf10\n"
        "sqc2 $vf9, 0x0(%0)\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

float KePow(float v, int n) {
    float r;
    int i;

    r = 1.0f;
    for (i = 0; i < n; i++) {
        r *= v;
    }
    return r;
}

float KeVu0Sqrt(float v) {
    sceVu0FVECTOR w;
    float r;

    w[1] = v;
    __asm__ __volatile__(
        "lqc2 $vf10, %1\n"
        "nop\n"
        "vsqrt $Q, $vf10y\n"
        "vwaitq\n"
        "cfc2.ni %0, $vi22\n"
        : "=r"(r)
        : "m"(*(u_long128 *)w));
    return r;
}

float KeVu0VecLen(float *v) {
    sceVu0FVECTOR fv0;

    __asm__ __volatile__(
        "lqc2 $vf10, 0x0(%1)\n"
        "vmul.xyz $vf10, $vf10, $vf10\n"
        "vmove.xyz $vf9, $vf10\n"
        "vaddx.y $vf9, $vf9, $vf9x\n"
        "vaddy.z $vf9, $vf9, $vf9y\n"
        "vsqrt $Q, $vf9z\n"
        "vwaitq\n"
        "vaddq.x $vf10, $vf0, $Q\n"
        "sqc2 $vf10, %0\n"
        : "=m"(fv0)
        : "r"(v));
    return fv0[0];
}

float KeVu0VecLenXY(float *v) {
    float r;

    __asm__ __volatile__(
        "lqc2 $vf10, 0x0(%1)\n"
        "vmul.xy $vf10, $vf10, $vf10\n"
        "vaddx.y $vf10, $vf10, $vf10x\n"
        "vsqrt $Q, $vf10y\n"
        "vwaitq\n"
        "cfc2.ni %0, $vi22\n"
        : "=r"(r)
        : "r"(v));
    return r;
}

float KeVu0GetDist(float *b, float *t) {
    sceVu0FVECTOR v0;

    sceVu0SubVector(v0, b, t);
    return KeVu0VecLen(v0);
}

u_short KeIdx_Alloc(KeIdx *_p) {
    u_short r;

    if (_p->Free == 0xFFFF) {
        return 0xFFFF;
    }
    r = _p->Free;
    _p->Free = _p->Wk[r];
    return r;
}

void KeIdx_Init(KeIdx *_p, u_short *wk, u_short num) {
    u_short i;

    _p->Wk = wk;
    _p->Free = 0;
    _p->Num = num;
    for (i = 1; i < num; i++) {
        *wk++ = i;
    }
    *wk = 0xFFFF;
}
// SOURCE DATA: .data .bss
