#include "pppEiDrawShipoly.h"
#include "System/Graphic/Effect/libppp/pppAlloc.h"
#include "System/Graphic/Effect/libppp/eiProg/pppColor.h"
#include "System/Graphic/Effect/libppp/eiProg/pppScale.h"
#include "System/Graphic/Effect/libppp/draw/md.h"
#include "System/Debug.h"
#include "include_asm.h"

enum {
    DRAWSHIPOLY_MATRIX = 0,
    DRAWSHIPOLY_SCALE = 1,
    DRAWSHIPOLY_COLOR = 2,
    DRAWSHIPOLY_THIS = 3
};

typedef struct { /* 0x10 */
    char flag;
    char ptype;
    short pc;
    u_char tno;
    u_char timposx;
    u_char res[2];
    u_word tex0l;
    u_word tex0h;
} OMD_PRIM_H;

extern sceVu0FMATRIX ppvCameraMatrixInv;
extern char D_00469F88[];

/* a macro, not an inline function: each expansion's asm carries its own line, so jump2 does not cross-jump them */
#define ApplyMatrix(v0, m, v1)                       \
    __asm__ volatile(                                \
        "lqc2 $vf27, 0x0(%2)\n"                      \
        "lqc2 $vf28, 0x0(%1)\n"                      \
        "lqc2 $vf29, 0x10(%1)\n"                     \
        "lqc2 $vf30, 0x20(%1)\n"                     \
        "lqc2 $vf31, 0x30(%1)\n"                     \
        "vmulax.xyzw $ACC, $vf28, $vf27x\n"          \
        "vmadday.xyzw $ACC, $vf29, $vf27y\n"         \
        "vmaddaz.xyzw $ACC, $vf30, $vf27z\n"         \
        "vmaddw.xyzw $vf26, $vf31, $vf0w\n"          \
        "sqc2 $vf26, 0x0(%0)\n"                      \
        :                                            \
        : "r"(v0), "r"(m), "r"(v1)                   \
        : "memory")

/* v0 = v1 + v2 */
#define AddVector(v0, v1, v2)                        \
    __asm__ volatile(                                \
        "lqc2 $vf10, 0x0(%1)\n"                      \
        "lqc2 $vf11, 0x0(%2)\n"                      \
        "vadd.xyzw $vf10, $vf10, $vf11\n"            \
        "sqc2 $vf10, 0x0(%0)\n"                      \
        :                                            \
        : "r"(v0), "r"(v1), "r"(v2)                  \
        : "memory")

/* v0 = v1 - v2 */
#define SubVector(v0, v1, v2)                        \
    __asm__ volatile(                                \
        "lqc2 $vf10, 0x0(%1)\n"                      \
        "lqc2 $vf11, 0x0(%2)\n"                      \
        "vsub.xyzw $vf10, $vf10, $vf11\n"            \
        "sqc2 $vf10, 0x0(%0)\n"                      \
        :                                            \
        : "r"(v0), "r"(v1), "r"(v2)                  \
        : "memory")

/* v.xyz = normalize(v.xyz) */
#define Normalize(v)                                 \
    __asm__ volatile(                                \
        "lqc2 $vf10, 0x0(%0)\n"                      \
        "vadd.xyz $vf8, $vf0, $vf10\n"               \
        "vmul.xyz $vf10, $vf10, $vf10\n"             \
        "vmove.xyz $vf9, $vf10\n"                    \
        "vaddx.y $vf9, $vf9, $vf9x\n"                \
        "vaddy.z $vf9, $vf9, $vf9y\n"                \
        "vsqrt $Q, $vf9z\n"                          \
        "vwaitq\n"                                   \
        "vaddq.x $vf10, $vf0, $Q\n"                  \
        "vdiv $Q, $vf0w, $vf10x\n"                   \
        "vwaitq\n"                                   \
        "vmulq.xyz $vf9, $vf8, $Q\n"                 \
        "sqc2 $vf9, 0x0(%0)\n"                       \
        :                                            \
        : "r"(v)                                     \
        : "memory")

/* len = |v.xyz|; v.xyz = normalize(v.xyz) */
#define NormalizeLen(len, v)                         \
    __asm__ volatile(                                \
        "lqc2 $vf10, 0x0(%1)\n"                      \
        "vadd.xyz $vf8, $vf0, $vf10\n"               \
        "vmul.xyz $vf10, $vf10, $vf10\n"             \
        "vmove.xyz $vf9, $vf10\n"                    \
        "vaddx.y $vf9, $vf9, $vf9x\n"                \
        "vaddy.z $vf9, $vf9, $vf9y\n"                \
        "vsqrt $Q, $vf9z\n"                          \
        "vwaitq\n"                                   \
        "vaddq.x $vf10, $vf0, $Q\n"                  \
        "qmfc2.ni %0, $vf10\n"                       \
        "vdiv $Q, $vf0w, $vf10x\n"                   \
        "vwaitq\n"                                   \
        "vmulq.xyz $vf9, $vf8, $Q\n"                 \
        "sqc2 $vf9, 0x0(%1)\n"                       \
        : "=r"(len)                                  \
        : "r"(v)                                     \
        : "memory")

#define CopyMatrix(m0, m1)                           \
    __asm__ volatile(                                \
        "lq $8, 0x0(%1)\n"                           \
        "lq $9, 0x10(%1)\n"                          \
        "lq $10, 0x20(%1)\n"                         \
        "lq $11, 0x30(%1)\n"                         \
        "sq $8, 0x0(%0)\n"                           \
        "sq $9, 0x10(%0)\n"                          \
        "sq $10, 0x20(%0)\n"                         \
        "sq $11, 0x30(%0)\n"                         \
        :                                            \
        : "r"(m0), "r"(m1)                           \
        : "$8", "$9", "$10", "$11", "memory")

#define CopyVector(v0, v1)                           \
    __asm__ volatile(                                \
        "lq $8, 0x0(%1)\n"                           \
        "sq $8, 0x0(%0)\n"                           \
        :                                            \
        : "r"(v0), "r"(v1)                           \
        : "$8", "memory")

/* m0 = m1 * m2 */
#define MulMatrix(m0, m1, m2)                        \
    __asm__ volatile(                                \
        "lqc2 $vf28, 0x0(%1)\n"                      \
        "lqc2 $vf29, 0x10(%1)\n"                     \
        "lqc2 $vf30, 0x20(%1)\n"                     \
        "lqc2 $vf31, 0x30(%1)\n"                     \
        "lqc2 $vf24, 0x0(%2)\n"                      \
        "lqc2 $vf25, 0x10(%2)\n"                     \
        "lqc2 $vf26, 0x20(%2)\n"                     \
        "lqc2 $vf27, 0x30(%2)\n"                     \
        "vmulax.xyzw $ACC, $vf28, $vf24x\n"          \
        "vmadday.xyzw $ACC, $vf29, $vf24y\n"         \
        "vmaddaz.xyzw $ACC, $vf30, $vf24z\n"         \
        "vmaddw.xyzw $vf24, $vf31, $vf24w\n"         \
        "vmulax.xyzw $ACC, $vf28, $vf25x\n"          \
        "vmadday.xyzw $ACC, $vf29, $vf25y\n"         \
        "vmaddaz.xyzw $ACC, $vf30, $vf25z\n"         \
        "vmaddw.xyzw $vf25, $vf31, $vf25w\n"         \
        "vmulax.xyzw $ACC, $vf28, $vf26x\n"          \
        "vmadday.xyzw $ACC, $vf29, $vf26y\n"         \
        "vmaddaz.xyzw $ACC, $vf30, $vf26z\n"         \
        "vmaddw.xyzw $vf26, $vf31, $vf26w\n"         \
        "vmulax.xyzw $ACC, $vf28, $vf27x\n"          \
        "vmadday.xyzw $ACC, $vf29, $vf27y\n"         \
        "vmaddaz.xyzw $ACC, $vf30, $vf27z\n"         \
        "vmaddw.xyzw $vf27, $vf31, $vf27w\n"         \
        "sqc2 $vf24, 0x0(%0)\n"                      \
        "sqc2 $vf25, 0x10(%0)\n"                     \
        "sqc2 $vf26, 0x20(%0)\n"                     \
        "sqc2 $vf27, 0x30(%0)\n"                     \
        :                                            \
        : "r"(m0), "r"(m1), "r"(m2)                  \
        : "memory")

/* d = |v1.xyz - v0.xyz| */
#define Distance(d, v0, v1)                          \
    __asm__ volatile(                                \
        "lqc2 $vf9, 0x0(%1)\n"                       \
        "lqc2 $vf10, 0x0(%2)\n"                      \
        "vsub.xyz $vf10, $vf10, $vf9\n"              \
        "vmul.xyz $vf10, $vf10, $vf10\n"             \
        "vaddy.x $vf10, $vf10, $vf10y\n"             \
        "vaddz.x $vf10, $vf10, $vf10z\n"             \
        "vsqrt $Q, $vf10x\n"                         \
        "vwaitq\n"                                   \
        "vaddq.x $vf10, $vf0, $Q\n"                  \
        "qmfc2.ni %0, $vf10\n"                       \
        : "=r"(d)                                    \
        : "r"(v0), "r"(v1)                           \
        : "memory")

/* v0 = v1 * t + v2 * (1 - t) */
#define InterVector(v0, v1, v2, t)                   \
    __asm__ volatile(                                \
        "lqc2 $vf4, 0x0(%1)\n"                       \
        "lqc2 $vf5, 0x0(%2)\n"                       \
        "qmtc2.ni %3, $vf6\n"                        \
        "vaddw.x $vf7, $vf0, $vf0w\n"                \
        "vsub.x $vf8, $vf7, $vf6\n"                  \
        "vmulax.xyzw $ACC, $vf4, $vf6x\n"            \
        "vmaddx.xyzw $vf9, $vf5, $vf8x\n"            \
        "sqc2 $vf9, 0x0(%0)\n"                       \
        :                                            \
        : "r"(v0), "r"(v1), "r"(v2), "r"(t)          \
        : "memory")

void drawShipoly(pppModelSt *mdl, PEiDrawShipoly *p, VEiDrawShipoly *val, VColor *col, sceVu0FVECTOR (*linev)[256]);
void drawDistShipoly(PEiDrawShipoly *p, VEiDrawShipoly *val, VColor *col, sceVu0FVECTOR (*linev)[256]);

extern int ppvZFrameBase;
void pppSendGsPacket(u_long128 *pkt_top);
void pppWaitDmaGs(void);

typedef union {
    u_long128 u128;
    u_long u64[2];
    u_int u32[4];
    float f32[4];
    u_short u16[8];
} QWdata;

/* sv = screen xyz (12.4 fixed) of v; FALSE if outside the GS drawing range */
#define RotTransPersClip(ret, sv, v)                 \
    ret = 1;                                         \
    __asm__ volatile(                                \
        "lqc2 $vf12, 0x0(%1)\n"                      \
        "lqc2 $vf4, 0x0(%2)\n"                       \
        "lqc2 $vf5, 0x10(%2)\n"                      \
        "lqc2 $vf6, 0x20(%2)\n"                      \
        "lqc2 $vf7, 0x30(%2)\n"                      \
        "vmulax.xyzw $ACC, $vf4, $vf12x\n"           \
        "vmadday.xyzw $ACC, $vf5, $vf12y\n"          \
        "vmaddaz.xyzw $ACC, $vf6, $vf12z\n"          \
        "vmaddw.xyzw $vf14, $vf7, $vf12w\n"          \
        "vdiv $Q, $vf0w, $vf14w\n"                   \
        "vmove.xyzw $vf15, $vf14\n"                  \
        "vsub.w $vf15, $vf15, $vf15\n"               \
        "vwaitq\n"                                   \
        "vmulq.xyz $vf15, $vf15, $Q\n"               \
        "vftoi4.xyz $vf15, $vf15\n"                  \
        "sqc2 $vf15, 0x0(%0)\n"                      \
        :                                            \
        : "r"(sv), "r"(v), "r"(ppvScreenMatrix)      \
        : "memory");                                 \
    if (sv[0] < 0) {                                 \
        ret = 0;                                     \
    } else if (sv[0] > 0xFFF0) {                     \
        ret = 0;                                     \
    }                                                \
    if (sv[1] < 0) {                                 \
        ret = 0;                                     \
    } else if (sv[1] > 0xFFF0) {                     \
        ret = 0;                                     \
    }                                                \
    if (sv[2] <= 0) {                                \
        ret = 0;                                     \
    }

/* stv.xy = (screen xy of v + st_sub.xy) * st_scl.xy */
static inline void RotTransPersST(float *stv, float *v, float *st_sub, float *st_scl) {
    __asm__ volatile(
        "lqc2 $vf12, 0x0(%1)\n"
        "lqc2 $vf4, 0x0(%2)\n"
        "lqc2 $vf5, 0x10(%2)\n"
        "lqc2 $vf6, 0x20(%2)\n"
        "lqc2 $vf7, 0x30(%2)\n"
        "vmulax.xyzw $ACC, $vf4, $vf12x\n"
        "vmadday.xyzw $ACC, $vf5, $vf12y\n"
        "vmaddaz.xyzw $ACC, $vf6, $vf12z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf12w\n"
        "vdiv $Q, $vf0w, $vf12w\n"
        "lqc2 $vf4, 0x0(%3)\n"
        "lqc2 $vf5, 0x0(%4)\n"
        "vwaitq\n"
        "vmulq.xyz $vf12, $vf12, $Q\n"
        "vadd.xy $vf12, $vf12, $vf4\n"
        "vmul.xy $vf12, $vf12, $vf5\n"
        "sqc2 $vf12, 0x0(%0)\n"
        :
        : "r"(stv), "r"(v), "r"(ppvScreenMatrix), "r"(st_sub), "r"(st_scl)
        : "memory");
    stv[2] = 1.0f;
    stv[3] = 1.0f;
}

static void init_spline(sceVu0FVECTOR *x, int n, sceVu0FVECTOR *data) {
    sceVu0FVECTOR *d;
    static sceVu0FVECTOR c = { 4.0f, -1.0f, 0.0f, 0.0f };

    d = (sceVu0FVECTOR *)0x70002000;
    __asm__ volatile(
        ".set noreorder\n"
        "addi $t1, $a1, -1\n"
        "addi $t2, $a1, -2\n"
        "lqc2 $vf1, 0x0(%0)\n"
        "sqc2 $vf0, 0x0($a2)\n"
        "sll $t3, $t1, 4\n"
        "add $t3, $t3, $a2\n"
        "sqc2 $vf0, 0x0($t3)\n"
        "daddu $t0, $t1, $zero\n"
        "daddu $t3, $zero, $zero\n"
        "1:\n"
        "blez $t0, 2f\n"
        "addi $t0, $t0, -1\n"
        "sll $t4, $t3, 4\n"
        "addi $t5, $t4, 0x10\n"
        "add $t6, $t4, $a0\n"
        "add $t7, $t5, $a0\n"
        "add $s0, $t5, %1\n"
        "lqc2 $vf10, 0x0($t6)\n"
        "lqc2 $vf11, 0x0($t7)\n"
        "vsub.xyzw $vf10, $vf11, $vf10\n"
        "sqc2 $vf10, 0x0($s0)\n"
        "b 1b\n"
        "addi $t3, $t3, 1\n"
        "2:\n"
        "lqc2 $vf10, 0x20(%1)\n"
        "lqc2 $vf11, 0x10(%1)\n"
        "lqc2 $vf12, 0x0($a2)\n"
        "vsub.xyzw $vf10, $vf10, $vf11\n"
        "vsub.xyzw $vf10, $vf10, $vf12\n"
        "sqc2 $vf10, 0x10($a2)\n"
        "vaddx.xyzw $vf11, $vf0, $vf1x\n"
        "sqc2 $vf11, 0x10(%1)\n"
        "daddu $t0, $t2, $zero\n"
        "addi $t0, $t0, -1\n"
        "addi $t3, $zero, 1\n"
        "3:\n"
        "blez $t0, 4f\n"
        "addi $t0, $t0, -1\n"
        "sll $t4, $t3, 4\n"
        "addi $t5, $t4, 0x10\n"
        "addi $t6, $t5, 0x10\n"
        "add $t7, $t4, %1\n"
        "add $s0, $t5, %1\n"
        "add $s1, $t6, %1\n"
        "add $s2, $t5, $a2\n"
        "add $s3, $t4, $a2\n"
        "lqc2 $vf10, 0x0($t7)\n"
        "lqc2 $vf11, 0x0($s0)\n"
        "lqc2 $vf12, 0x0($s1)\n"
        "lqc2 $vf13, 0x0($s3)\n"
        "vdiv $Q, $vf0w, $vf10x\n"
        "vwaitq\n"
        "vaddq.x $vf15, $vf0, $Q\n"
        "vdiv $Q, $vf0w, $vf10y\n"
        "vwaitq\n"
        "vaddq.y $vf15, $vf0, $Q\n"
        "vdiv $Q, $vf0w, $vf10z\n"
        "vwaitq\n"
        "vaddq.z $vf15, $vf0, $Q\n"
        "vsub.xyzw $vf16, $vf12, $vf11\n"
        "vmul.xyzw $vf17, $vf13, $vf15\n"
        "vsub.xyzw $vf17, $vf16, $vf17\n"
        "sqc2 $vf17, 0x0($s2)\n"
        "vmuly.xyzw $vf18, $vf15, $vf1y\n"
        "vaddx.xyzw $vf18, $vf18, $vf1x\n"
        "sqc2 $vf18, 0x0($s0)\n"
        "b 3b\n"
        "addi $t3, $t3, 1\n"
        "4:\n"
        "sll $t3, $t1, 4\n"
        "add $t3, $t3, $a2\n"
        "sll $t4, $t2, 4\n"
        "add $t4, $t4, $a2\n"
        "lqc2 $vf10, 0x0($t3)\n"
        "lqc2 $vf11, 0x0($t4)\n"
        "vsub.xyzw $vf11, $vf11, $vf10\n"
        "sqc2 $vf11, 0x0($t4)\n"
        "daddu $t3, $t2, $zero\n"
        "5:\n"
        "blezl $t3, 6f\n"
        "nop\n"
        "sll $t4, $t3, 4\n"
        "addi $t5, $t4, 0x10\n"
        "add $t7, $t4, %1\n"
        "add $s0, $t4, $a2\n"
        "add $s1, $t5, $a2\n"
        "lqc2 $vf10, 0x0($t7)\n"
        "lqc2 $vf11, 0x0($s0)\n"
        "lqc2 $vf12, 0x0($s1)\n"
        "vsub.xyzw $vf11, $vf11, $vf12\n"
        "vdiv $Q, $vf11x, $vf10x\n"
        "vwaitq\n"
        "vaddq.x $vf15, $vf0, $Q\n"
        "vdiv $Q, $vf11y, $vf10y\n"
        "vwaitq\n"
        "vaddq.y $vf15, $vf0, $Q\n"
        "vdiv $Q, $vf11z, $vf10z\n"
        "vwaitq\n"
        "vaddq.z $vf15, $vf0, $Q\n"
        "sqc2 $vf15, 0x0($s0)\n"
        "b 5b\n"
        "addi $t3, $t3, -1\n"
        "6:\n"
        ".set reorder\n"
        :
        : "r"(c), "r"(d)
        : "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$16", "$17", "$18", "$19", "memory");
}

static void spline(float *out, sceVu0FVECTOR *x, float t, sceVu0FVECTOR *data) {
    int i;
    float d;
    static sceVu0FVECTOR c = { 3.0f, 2.0f, 0.0f, 0.0f };

    i = t;
    d = t - i;
    x += i;
    data += i;
    c[2] = d;
    __asm__ volatile(
        "lqc2 $vf1, 0x0(%1)\n"
        "lqc2 $vf2, 0x10(%1)\n"
        "lqc2 $vf3, 0x0(%2)\n"
        "lqc2 $vf4, 0x10(%2)\n"
        "lqc2 $vf5, 0x0(%3)\n"
        "vsub.xyzw $vf10, $vf4, $vf3\n"
        "vmuly.xyzw $vf11, $vf3, $vf5y\n"
        "vmulx.xyzw $vf12, $vf3, $vf5x\n"
        "vsub.xyzw $vf13, $vf2, $vf1\n"
        "vmulz.xyzw $vf10, $vf10, $vf5z\n"
        "vadd.xyzw $vf11, $vf11, $vf4\n"
        "vadd.xyzw $vf10, $vf10, $vf12\n"
        "vsub.xyzw $vf13, $vf13, $vf11\n"
        "vmulz.xyzw $vf10, $vf10, $vf5z\n"
        "vadd.xyzw $vf10, $vf10, $vf13\n"
        "vmulz.xyzw $vf10, $vf10, $vf5z\n"
        "vadd.xyzw $vf10, $vf10, $vf1\n"
        "sqc2 $vf10, 0x0(%0)\n"
        :
        : "r"(out), "r"(x), "r"(data), "r"(c)
        : "memory");
}

static void get_pos_3D(PEiDrawShipoly *param, VEiDrawShipoly *val, float w, float *va, float *vb) {
    sceVu0FVECTOR vec;

    vec[0] = 0.0f;
    vec[1] = w * 0.5f;
    vec[2] = 0.0f;
    vec[3] = 1.0f;
    ApplyMatrix(va, val->CMat, vec);
    vec[1] = -w * 0.5f;
    ApplyMatrix(vb, val->CMat, vec);
    switch (param->Coordinate) {
    case 0:
        break;
    case 1:
        ApplyMatrix(va, ppvParMatrix, va);
        ApplyMatrix(vb, ppvParMatrix, vb);
        break;
    case 2:
        if (ppvReferenceMatrixList) {
            ApplyMatrix(va, ppvParMatrix, va);
            ApplyMatrix(va, ppvReferenceMatrixList[1], va);
            ApplyMatrix(vb, ppvParMatrix, vb);
            ApplyMatrix(vb, ppvReferenceMatrixList[1], vb);
        }
        break;
    case 3:
        ApplyMatrix(va, ppvParMatrix, va);
        ApplyMatrix(va, ppvCameraMatrixInv, va);
        ApplyMatrix(vb, ppvParMatrix, vb);
        ApplyMatrix(vb, ppvCameraMatrixInv, vb);
        break;
    }
}

static void get_pos_2D(PEiDrawShipoly *param, VEiDrawShipoly *val, float *va) {
    *(u_long128 *)va = *(u_long128 *)val->CMat[3];
    switch (param->Coordinate) {
    case 0:
        break;
    case 1:
        ApplyMatrix(va, ppvParMatrix, va);
        break;
    case 2:
        if (ppvReferenceMatrixList) {
            ApplyMatrix(va, ppvParMatrix, va);
            ApplyMatrix(va, ppvReferenceMatrixList[1], va);
        }
        break;
    case 3:
        ApplyMatrix(va, ppvParMatrix, va);
        ApplyMatrix(va, ppvCameraMatrixInv, va);
        break;
    }
}

static void get_st(PEiDrawShipoly *param, float *s_min, float *s_max, float *t_min, float *t_max) {
    float smin;
    float smax;
    float tmin;
    float tmax;

    smin = 0.0f;
    smax = 1.0f;
    tmin = 0.0f;
    tmax = 1.0f;
    switch (param->TexS) {
    case 1:
        smax = 0.5f;
        break;
    case 2:
        smax = 0.25f;
        break;
    }
    switch (param->TexT) {
    case 1:
        tmax *= 0.5f;
        break;
    case 2:
        tmax *= 0.25f;
        break;
    }
    switch (param->TexX) {
    case 1:
        smin += 0.5f;
        smax += 0.25f;
        break;
    case 2:
        smin += 0.5f;
        smax += 0.5f;
        break;
    case 3:
        smin += 0.75f;
        smax += 0.75f;
        break;
    }
    *s_min = smin + 0.01f;
    *s_max = smax - 0.01f;
    *t_min = tmin + 0.01f;
    *t_max = tmax - 0.01f;
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly", drawShipoly);
#if 0
/* register allocation differs; also short float loops (bc1t hazard nop lost by fix_short_loops) */
void drawShipoly(pppModelSt *mdl, PEiDrawShipoly *p, VEiDrawShipoly *val, VColor *col, sceVu0FVECTOR (*linev)[256]) {
    int poly_num;
    float s_add;
    float s;
    float s_min;
    float s_max;
    float t_min;
    float t_max;
    float gradation;
    float gradation_del;
    float gradation_add;
    int gradation_fadeout;
    float fade;
    float fade_add;
    float alpha0;
    float alpha1;
    int vtn;
    u_long tex0l;
    u_long tex0h;
    OMD_H *omd;
    OMD_PRIM_H *mph;
    float r1;
    float r2;
    int n;
    sceVu0IVECTOR c1;
    sceVu0IVECTOR c0;
    QWdata *packet;
    QWdata *packet0;
    QWdata *packet1;
    int i;
    int j;
    int idx;
    int spad_id;
    long128 *packet_db[2];

    s_add = 0.0f;
    poly_num = p->PolyNumH;
    gradation = 1.0f;
    gradation_del = 0.0f;
    gradation_add = 0.0f;
    gradation_fadeout = 0;
    fade = 0.0f;
    fade_add = 0.0f;
    if (val->GetPosCt < 3) {
        return;
    }
    if (mdl) {
        omd = mdl->omd;
        mph = (OMD_PRIM_H *)((char *)omd + omd->pa);
        tex0l = mph->tex0l;
        tex0h = mph->tex0h;
    } else {
        tex0l = 0;
        tex0h = 0;
    }
    s = 0.0f;
    get_st(p, &s_min, &s_max, &t_min, &t_max);
    if (p->TexMapping == 0) {
        switch (p->TexDir) {
        case 0:
        case 2:
            s = s_min;
            s_add = (s_max - s) / poly_num;
            break;
        case 1:
        case 3:
            s = s_max;
            s_add = -(s - s_min) / poly_num;
            break;
        }
    } else {
        switch (p->TexDir) {
        case 0:
        case 2:
            s = s_min;
            s_add = (s_max - s) * ((float)val->GetPosCt / val->SmpNum) / poly_num;
            break;
        case 1:
        case 3:
            s = s_max;
            s_add = -((s - s_min) * ((float)val->GetPosCt / val->SmpNum)) / poly_num;
            break;
        }
    }
    if (p->Gradation) {
        n = poly_num * p->GradationCenter;
        r1 = n;
        r2 = poly_num - r1;
        if (r1 <= 0.0f) {
            gradation = 1.0f;
        } else {
            gradation = 0.0f;
        }
        if (r1 > 0.0f) {
            gradation_add = 1.0f / r1;
        }
        if (r2 > 0.0f) {
            gradation_del = 1.0f / r2;
        }
    }
    packet_db[0] = (long128 *)0x70000000;
    packet_db[1] = (long128 *)0x70002000;
    packet = (QWdata *)0x70000000;
    n = 0;
    packet[n].u128 = 0;
    packet[n].u32[0] = 0x70000007;
    n++;
    packet[n].u64[0] = 0x6000000000008001;
    packet[n].u64[1] = 0xEEEEEE;
    n++;
    if (!p->ZTest) {
        packet[n].u64[0] = 0x7000D;
        packet[n].u64[1] = 0x47;
        n++;
    } else {
        packet[n].u64[0] = 0x3000D;
        packet[n].u64[1] = 0x47;
        n++;
    }
    packet[n].u64[0] = ppvZFrameBase | 0x31000000 | 0x100000000;
    packet[n].u64[1] = 0x4E;
    n++;
    packet[n].u32[0] = tex0l;
    packet[n].u32[1] = tex0h;
    packet[n].u64[1] = 6;
    n++;
    switch (p->AlphaMode) {
    case 0:
        packet[n].u64[0] = 0x8000000044;
        break;
    case 1:
        packet[n].u64[0] = 0x8000000048;
        break;
    case 2:
        packet[n].u64[0] = 0x8000000042;
        break;
    case 3:
        packet[n].u64[0] = 0x8000000041;
        break;
    case 4:
        packet[n].u64[0] = 0x8000000046;
        break;
    }
    packet[n].u64[1] = 0x42;
    n++;
    packet[n].u64[0] = 0x60;
    packet[n].u64[1] = 0x14;
    n++;
    packet[n].u64[0] = 0;
    packet[n].u64[1] = 8;
    n++;
    pppSendGsPacket((u_long128 *)0x70000000);
    pppWaitDmaGs();
    vtn = p->PolyNumV * 2 + 2;
    packet0 = (QWdata *)packet_db[0];
    packet1 = (QWdata *)packet_db[1];
    packet1->u128 = 0;
    packet0->u128 = 0;
    packet0->u32[0] = packet1->u32[0] = 0x70000000 | (vtn * 3 + 1);
    n = 1;
    if (mdl) {
        packet0[n].u64[0] = packet1[n].u64[0] =
            vtn | 0x8000 | ((u_long)0x5C << 47) | ((u_long)1 << 46) | ((u_long)3 << 60);
    } else {
        packet0[n].u64[0] = packet1[n].u64[0] =
            vtn | 0x8000 | ((u_long)0x5C << 47) | ((u_long)1 << 46) | ((u_long)3 << 60);
    }
    packet0[n].u64[1] = packet1[n].u64[1] = 0x412;
    vtn /= 2;
    spad_id = 0;
    c0[0] = col->release_col.r;
    c0[1] = col->release_col.g;
    c0[2] = col->release_col.b;
    c0[3] = col->release_col.a;
    *(u_long128 *)c1 = *(u_long128 *)c0;
    for (i = 0; i < poly_num; i++) {
        float s0;
        float s1;
        int draw;

        draw = 1;
        if (p->Gradation) {
            alpha0 = col->release_col.a * gradation;
            if (gradation >= 1.0f) {
                if (!gradation_fadeout) {
                    gradation_fadeout = 1;
                }
            }
            if (!gradation_fadeout) {
                gradation += gradation_add;
            } else {
                gradation -= gradation_del;
            }
            if (gradation < 0.0f) {
                gradation = 0.0f;
            } else if (gradation >= 1.0f) {
                gradation = 1.0f;
            }
            alpha1 = col->release_col.a * gradation;
        } else {
            alpha1 = alpha0 = col->release_col.a;
        }
        s0 = s;
        if (p->TexRepeat <= 0.0f) {
            s1 = s0 + s_add;
            if (s1 > s_max) {
                s = s_max;
            } else {
                s = s1;
            }
        } else {
            float linevw;
            float rate;

            linevw = linev[0][i][3] / (p->TexRepeat * 0.01f);
            rate = (s_max - s_min) * linevw;
            s1 = s0 + rate;
            if (s1 > s_max) {
                s = s1 - s_max + s_min;
            } else {
                s = s1;
            }
        }
        s1 += val->TexScrollS;
        s0 += val->TexScrollS;
        if (p->Fade) {
            fade_add = 2.0f / (vtn - 1);
            fade = 0.0f;
        }
        packet = (QWdata *)packet_db[spad_id];
        spad_id ^= 1;
        idx = 2;
        for (j = 0; j < vtn; j++) {
            sceVu0FVECTOR lv;
            sceVu0IVECTOR sv0;
            sceVu0IVECTOR sv1;
            float r;
            float a0;
            float a1;
            float t;
            float s0_;
            float s1_;
            float t0_;
            float t1_;

            r = j / (float)(vtn - 1);
            a0 = alpha0;
            a1 = alpha1;
            t = (t_max - t_min) * r + t_min + val->TexScrollT;
            if (p->Fade) {
                a0 *= fade;
                a1 *= fade;
                fade += fade_add;
                if (fade >= 1.0f) {
                    fade = 1.0f;
                    fade_add = -fade_add;
                } else if (fade <= 0.0f) {
                    fade = 0.0f;
                }
            }
            InterVector(lv, linev[1][i], linev[0][i], r);
            lv[3] = 1.0f;
            {
                int ret;

                RotTransPersClip(ret, sv0, lv);
                if (!ret) {
                    draw = 0;
                    break;
                }
            }
            InterVector(lv, linev[1][i + 1], linev[0][i + 1], r);
            lv[3] = 1.0f;
            {
                int ret;

                RotTransPersClip(ret, sv1, lv);
                if (!ret) {
                    draw = 0;
                    break;
                }
            }
            c0[3] = a0;
            c1[3] = a1;
            if (sv0[2] > 24000000) {
                draw = 0;
                break;
            }
            if (sv0[2] > 6000000) {
                c0[3] = c0[3] * (1.0f - (sv0[2] - 6000000.0f) / 18000000.0f);
            }
            if (sv1[2] > 24000000) {
                draw = 0;
                break;
            }
            if (sv1[2] > 6000000) {
                c1[3] = c1[3] * (1.0f - (sv1[2] - 6000000.0f) / 18000000.0f);
            }
            if (p->TexDir < 2) {
                s0_ = s0;
                t0_ = t;
                s1_ = s1;
                t1_ = t;
            } else {
                s0_ = t;
                t0_ = s0;
                s1_ = t;
                t1_ = s1;
            }
            packet[idx].f32[0] = s0_;
            packet[idx].f32[1] = t0_;
            packet[idx].f32[2] = 1.0f;
            packet[idx].u32[3] = 0;
            idx++;
            packet[idx++].u128 = *(u_long128 *)c0;
            packet[idx++].u128 = *(u_long128 *)sv0;
            packet[idx].f32[0] = s1_;
            packet[idx].f32[1] = t1_;
            packet[idx].f32[2] = 1.0f;
            packet[idx].u32[3] = 0;
            idx++;
            packet[idx++].u128 = *(u_long128 *)c1;
            packet[idx++].u128 = *(u_long128 *)sv1;
        }
        pppWaitDmaGs();
        if (draw) {
            pppSendGsPacket((u_long128 *)packet);
        }
    }
    pppWaitDmaGs();
    if (!ppvUserStopPartF && !(ppvMng->bFlagStop || ppvMng->bBoolSlow)) {
        val->TexScrollT += p->TexScrollT;
        val->TexScrollS += p->TexScrollS;
        while (val->TexScrollT > 1.0f) {
            val->TexScrollT -= 1.0f;
        }
        while (val->TexScrollT < 0.0f) {
            val->TexScrollT += 1.0f;
        }
        while (val->TexScrollS > 1.0f) {
            val->TexScrollS -= 1.0f;
        }
        while (val->TexScrollS < 0.0f) {
            val->TexScrollS += 1.0f;
        }
    }
}

#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly", drawDistShipoly);
#if 0
/* register allocation of loop temporaries (v0/v1/a0) and a few float regs differs */
void drawDistShipoly(PEiDrawShipoly *p, VEiDrawShipoly *val, VColor *col, sceVu0FVECTOR (*linev)[256]) {
    int poly_num;
    float gradation;
    float gradation_del;
    float gradation_add;
    int gradation_fadeout;
    float fade;
    float fade_add;
    int vtn;
    sceVu0FVECTOR st_scl;
    sceVu0FVECTOR st_sub;
    float r1;
    float r2;
    int n;
    sceVu0IVECTOR c1;
    sceVu0IVECTOR c0;
    QWdata *packet;
    QWdata *packet0;
    QWdata *packet1;
    int i;
    int j;
    int idx;
    int spad_id;
    long128 *packet_db[2];
    float table_r[16];
    float table_dr[16];

    poly_num = p->PolyNumH;
    gradation = 1.0f;
    gradation_del = 0.0f;
    gradation_add = 0.0f;
    gradation_fadeout = 0;
    fade = 0.0f;
    fade_add = 0.0f;
    if (val->GetPosCt < 3) {
        return;
    }
    if (p->DistMode == 1) {
        st_sub[0] = ppvDrawWidth / 2 - 2048;
        st_sub[1] = ppvDrawHeight / 2 - 2048;
    } else {
        st_sub[0] = ppvDispWidth / 2 - 2048;
        st_sub[1] = ppvDispHeight / 2 - 2048;
    }
    st_scl[1] = st_scl[0] = 1.0f / 1024.0f;
    if (p->Gradation) {
        n = poly_num * p->GradationCenter;
        r1 = n;
        r2 = poly_num - r1;
        if (r1 <= 0.0f) {
            gradation = 1.0f;
        } else {
            gradation = 0.0f;
        }
        if (r1 > 0.0f) {
            gradation_add = 1.0f / r1;
        }
        if (r2 > 0.0f) {
            gradation_del = 1.0f / r2;
        }
    }
    packet_db[0] = (long128 *)0x70000000;
    packet_db[1] = (long128 *)0x70002000;
    packet = (QWdata *)0x70000000;
    n = 0;
    packet[n].u128 = 0;
    packet[n].u32[0] = 0x70000008;
    n++;
    packet[n].u64[0] = 0x7000000000008001;
    packet[n].u64[1] = 0xEEEEEEE;
    n++;
    if (!p->ZTest) {
        packet[n].u64[0] = 0x7000D;
        packet[n].u64[1] = 0x47;
        n++;
    } else {
        packet[n].u64[0] = 0x3000D;
        packet[n].u64[1] = 0x47;
        n++;
    }
    packet[n].u64[0] = ppvZFrameBase | 0x31000000 | 0x100000000;
    packet[n].u64[1] = 0x4E;
    n++;
    if (p->DistMode == 1) {
        packet[n].u64[0] = (u_long)(ppvDrawFrameBase << 5) | ((u_long)(ppvDrawWidth / 64) << 14) | ((u_long)10 << 26) |
                           ((u_long)10 << 30);
    } else {
        packet[n].u64[0] = (u_long)(ppvDispFrameBase << 5) | ((u_long)(ppvDispWidth / 64) << 14) | ((u_long)10 << 26) |
                           ((u_long)10 << 30);
    }
    packet[n].u64[1] = 6;
    n++;
    switch (p->AlphaMode) {
    case 0:
        packet[n].u64[0] = 0x8000000044;
        break;
    case 1:
        packet[n].u64[0] = 0x8000000048;
        break;
    case 2:
        packet[n].u64[0] = 0x8000000042;
        break;
    }
    packet[n].u64[1] = 0x42;
    n++;
    packet[n].u64[0] = 0x60;
    packet[n].u64[1] = 0x14;
    n++;
    packet[n].u64[0] = 0;
    packet[n].u64[1] = 8;
    n++;
    packet[n].u64[0] = 0;
    packet[n].u64[1] = 0x3F;
    n++;
    pppSendGsPacket((u_long128 *)0x70000000);
    pppWaitDmaGs();
    vtn = p->PolyNumV * 2 + 2;
    packet0 = (QWdata *)packet_db[0];
    packet1 = (QWdata *)packet_db[1];
    packet1->u128 = 0;
    packet0->u128 = 0;
    packet0->u32[0] = packet1->u32[0] = 0x70000000 | (vtn * 3 + 1);
    packet0[1].u64[0] = packet1[1].u64[0] =
        vtn | 0x8000 | ((u_long)0x5C << 47) | ((u_long)1 << 46) | ((u_long)3 << 60);
    packet0[1].u64[1] = packet1[1].u64[1] = 0x412;
    vtn = (u_int)vtn >> 1;
    c0[0] = col->release_col.r;
    c0[1] = col->release_col.g;
    c0[2] = col->release_col.b;
    c0[3] = col->release_col.a;
    *(u_long128 *)c1 = *(u_long128 *)c0;
    for (j = 0; j < vtn; j++) {
        float r;

        r = j / (float)(vtn - 1);
        table_r[j] = r;
        if (!p->Gradation) {
            table_dr[j] = (r - 0.5f) * (val->Dist + 1.0f) + 0.5f;
        }
    }
    spad_id = 0;
    for (i = 0; i < poly_num; i++) {
        int draw;

        draw = 1;
        if (p->Gradation) {
            float r;

            if (gradation >= 1.0f) {
                if (!gradation_fadeout) {
                    gradation_fadeout = 1;
                }
            }
            if (!gradation_fadeout) {
                gradation += gradation_add;
            } else {
                gradation -= gradation_del;
            }
            if (gradation < 0.0f) {
                gradation = 0.0f;
            } else if (gradation >= 1.0f) {
                gradation = 1.0f;
            }
            for (j = 0; j < vtn; j++) {
                table_dr[j] = (table_r[j] - 0.5f) * (val->Dist * gradation + 1.0f) + 0.5f;
            }
        }
        if (p->Fade) {
            fade_add = 2.0f / (vtn - 1);
            fade = 0.0f;
        }
        packet = (QWdata *)packet_db[spad_id];
        spad_id ^= 1;
        {
            idx = 2;

            for (j = 0; j < vtn; j++) {
                sceVu0FVECTOR lv;
                sceVu0IVECTOR sv0;
                sceVu0IVECTOR sv1;
                float a;
                float r;

                a = col->release_col.a;
                r = table_r[j];
                if (p->Fade) {
                    a *= fade;
                    fade += fade_add;
                    if (fade >= 1.0f) {
                        fade = 1.0f;
                        fade_add = -fade_add;
                    } else if (fade <= 0.0f) {
                        fade = 0.0f;
                    }
                }
                c0[3] = c1[3] = a;
                InterVector(lv, linev[1][i], linev[0][i], r);
                lv[3] = 1.0f;
                {
                    int ret;

                    RotTransPersClip(ret, sv0, lv);
                    if (!ret) {
                        draw = 0;
                        break;
                    }
                }
                InterVector(lv, linev[1][i + 1], linev[0][i + 1], r);
                lv[3] = 1.0f;
                {
                    int ret;

                    RotTransPersClip(ret, sv1, lv);
                    if (!ret) {
                        draw = 0;
                        break;
                    }
                }
                r = table_dr[j];
                InterVector(lv, linev[1][i], linev[0][i], r);
                lv[3] = 1.0f;
                RotTransPersST(packet[idx++].f32, lv, st_sub, st_scl);
                packet[idx++].u128 = *(u_long128 *)c0;
                sv0[0] -= 8;
                sv0[1] -= 8;
                packet[idx++].u128 = *(u_long128 *)sv0;
                InterVector(lv, linev[1][i + 1], linev[0][i + 1], r);
                lv[3] = 1.0f;
                RotTransPersST(packet[idx++].f32, lv, st_sub, st_scl);
                packet[idx++].u128 = *(u_long128 *)c1;
                sv1[0] -= 8;
                sv1[1] -= 8;
                packet[idx++].u128 = *(u_long128 *)sv1;
            }
        }
        pppWaitDmaGs();
        if (draw) {
            pppSendGsPacket((u_long128 *)packet);
        }
    }
    pppWaitDmaGs();
}

#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly", pppEiDrawDrawShipoly);
#if 0
/* register allocation of loop temporaries differs (a0/a1/a2, thin loop) */
void pppEiDrawDrawShipoly(pppPObject *pobj, PEiDrawShipoly *param, pppCtrlTable *ctbl) {
    VEiDrawShipoly *val;
    VScale *scl;
    VColor *col;
    pppModelSt *mdl;
    sceVu0FVECTOR *centv;
    extern sceVu0FVECTOR D_00645050[2][256];
#define linev D_00645050
    sceVu0FMATRIX cnv_mat;
    u_int frame_ct;
    u_int smp_num;
    sceVu0FVECTOR *smp;
    int poly_num;
    int i;
    int j;
    int k;
    int n;

    val = (VEiDrawShipoly *)&pobj->val[ctbl->useVal[DRAWSHIPOLY_THIS]];
    scl = (VScale *)&pobj->val[ctbl->useVal[DRAWSHIPOLY_SCALE]];
    col = (VColor *)&pobj->val[ctbl->useVal[DRAWSHIPOLY_COLOR]];
    smp_num = val->SmpNum;
    frame_ct = val->FrameCt - 1;
    smp = val->Smp;
    poly_num = param->PolyNumH;
    if (!smp) {
        return;
    }
    if (param->Model == 0xFFFF) {
        mdl = 0;
    } else {
        mdl = &ppvEnv->lpModel[param->Model];
    }
    centv = (sceVu0FVECTOR *)0x70001800;
    switch (param->Coordinate) {
    case 0:
        CopyMatrix(cnv_mat, ppvWorldMatrix);
        break;
    case 1:
        CopyMatrix(cnv_mat, ppvCameraMatrix);
        break;
    case 2:
        if (ppvReferenceMatrixList) {
            CopyMatrix(cnv_mat, ppvReferenceMatrixList[0]);
            MulMatrix(cnv_mat, ppvCameraMatrix, cnv_mat);
        }
        break;
    case 3:
        CopyMatrix(cnv_mat, ppvCameraMatrix);
        MulMatrix(cnv_mat, ppvCameraMatrix, cnv_mat);
        break;
    }
    if (param->Type == 0) {
        if (val->GetPosCt >= 3) {
            for (j = 0; j < 2; j++) {
                for (k = 0; k < val->GetPosCt; k++) {
                    n = frame_ct - k;
                    if (n < 0) {
                        n += smp_num;
                    }
                    CopyVector(((sceVu0FVECTOR *)0x70000000)[k], smp[n * 2 + j]);
                    ((sceVu0FVECTOR *)0x70000000)[k][3] = 1.0f;
                    ApplyMatrix(((sceVu0FVECTOR *)0x70000000)[k], cnv_mat, ((sceVu0FVECTOR *)0x70000000)[k]);
                }
                init_spline((sceVu0FVECTOR *)0x70000000, val->GetPosCt, (sceVu0FVECTOR *)0x70000800);
                for (k = 0; k < poly_num + 1; k++) {
                    spline(linev[j][k], (sceVu0FVECTOR *)0x70000000, k / (poly_num / (float)(val->GetPosCt - 1)),
                           (sceVu0FVECTOR *)0x70000800);
                    linev[j][k][3] = 1.0f;
                }
            }
        }
    } else if (val->GetPosCt >= 3) {
        sceVu0FVECTOR *spl;

        for (k = 0, spl = (sceVu0FVECTOR *)0x70000000; k < val->GetPosCt; k++) {
            n = frame_ct - k;
            if (n < 0) {
                n += smp_num;
            }
            CopyVector(*spl, smp[n]);
            (*spl)[3] = 1.0f;
            spl++;
        }
        init_spline((sceVu0FVECTOR *)0x70000000, val->GetPosCt, (sceVu0FVECTOR *)0x70000800);
        for (k = 0; k < poly_num + 1; k++) {
            spline(centv[k], (sceVu0FVECTOR *)0x70000000, (float)k / poly_num * (val->GetPosCt - 1),
                   (sceVu0FVECTOR *)0x70000800);
            centv[k][3] = 1.0f;
            ApplyMatrix(centv[k], cnv_mat, centv[k]);
        }
        if (param->TexRepeat > 0.0f) {
            for (k = 0; k < poly_num; k++) {
                Distance(centv[k][3], centv[k], centv[k + 1]);
            }
            centv[k][3] = 0.0f;
        }
        if (param->Type == 1) {
            sceVu0FVECTOR dv0;
            sceVu0FVECTOR dv1;
            sceVu0FVECTOR dv;
            sceVu0FVECTOR pos0;
            sceVu0FVECTOR pos1;
            float width;
            float dvr;

            *(u_long128 *)dv1 = 0;
            *(u_long128 *)dv0 = 0;
            width = param->Width * scl->scl.x * ppvParScl[0];
            for (k = 0; k < poly_num + 1; k++) {
                if (k == poly_num) {
                    SubVector(dv, centv[poly_num - 1], centv[poly_num]);
                } else {
                    SubVector(dv, centv[k], centv[k + 1]);
                }
                dv1[3] = 0.0f;
                Normalize(dv);
                SCALE_VECTOR_XYZ(dv, width);
                dvr = dv[0];
                dv[0] = -dv[1];
                dv[1] = dvr;
                AddVector(pos0, centv[k], dv);
                SubVector(pos1, centv[k], dv);
                linev[0][k][0] = pos0[0];
                linev[0][k][1] = pos0[1];
                linev[0][k][2] = linev[1][k][2] = centv[k][2];
                linev[0][k][3] = linev[1][k][3] = centv[k][3];
                linev[1][k][0] = pos1[0];
                linev[1][k][1] = pos1[1];
            }
        } else {
            sceVu0FVECTOR dv0;
            sceVu0FVECTOR dv1;
            sceVu0FVECTOR dv;
            sceVu0FVECTOR pos0;
            sceVu0FVECTOR pos1;
            float width;

            *(u_long128 *)dv1 = 0;
            *(u_long128 *)dv0 = 0;
            width = param->Width * scl->scl.x * ppvParScl[0];
            for (k = 0; k < poly_num + 1; k++) {
                if (k == poly_num) {
                    dv1[0] = dv1[2] = 0.0f;
                } else {
                    SubVector(dv1, centv[k], centv[k + 1]);
                }
                dv1[2] = dv1[3] = 0.0f;
                Normalize(dv1);
                SCALE_VECTOR_XYZ(dv1, width);
                AddVector(dv, dv0, dv1);
                {
                    float t = dv[1];
                    dv[1] = dv[0];
                    dv[0] = -t;
                }
                *(u_long128 *)dv0 = *(u_long128 *)dv1;
                AddVector(pos0, centv[k], dv);
                SubVector(pos1, centv[k], dv);
                pos0[2] = pos1[2] = 0.0f;
                linev[0][k][0] = pos0[0];
                linev[0][k][1] = pos0[1];
                linev[0][k][2] = linev[1][k][2] = centv[k][2];
                linev[0][k][3] = linev[1][k][3] = centv[k][3];
                linev[1][k][0] = pos1[0];
                linev[1][k][1] = pos1[1];
            }
        }
    }
    if (param->Thin) {
        sceVu0FVECTOR point[2];
        sceVu0FVECTOR v;
        float r;
        float thin_r;

        r = 1.0f;
        thin_r = r / poly_num;
        for (i = 0; i < poly_num + 1; i++) {
            for (j = 0; j < 2; j++) {
                point[j][0] = linev[j][i][0];
                point[j][1] = linev[j][i][1];
                point[j][2] = linev[j][i][2];
            }
            AddVector(v, point[0], point[1]);
            SCALE_VECTOR_XYZ(v, 0.5f);
            InterVector(point[0], point[0], v, r);
            InterVector(point[1], point[1], v, r);
            linev[0][i][0] = point[0][0];
            linev[0][i][1] = point[0][1];
            linev[0][i][2] = point[0][2];
            linev[1][i][0] = point[1][0];
            linev[1][i][1] = point[1][1];
            linev[1][i][2] = point[1][2];
            r -= thin_r;
            if (r <= 0.0f) {
                r = 0.0f;
            }
        }
    }
    if (!param->DistMode) {
        drawShipoly(mdl, param, val, col, linev);
    } else {
        if (!ppvUserStopPartF && !(ppvMng->bFlagStop || ppvMng->bBoolSlow)) {
            val->DistV += val->DistA;
            val->Dist += val->DistV;
            if (param->cdt.time == pobj->time) {
                val->Dist += param->Dist;
                val->DistV += param->DistV;
                val->DistA += param->DistA;
            }
        }
        drawDistShipoly(param, val, col, linev);
    }
}

#endif
void pppEiDrawShipoly(pppPObject *pobj, PEiDrawShipoly *param, pppCtrlTable *ctbl) {
    VEiDrawShipoly *val;
    int i;
    float f;

    val = (VEiDrawShipoly *)&pobj->val[ctbl->useVal[DRAWSHIPOLY_THIS]];
    if (ppvUserStopPartF) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    if (!param->Stop) {
        CopyMatrix(val->CMat, ppvPObj->cmat);
    }
    if (!val->StartParam) {
        val->StartParam = param;
    }
    if (!val->Smp) {
        val->SmpNum = param->FrameNum;
        if (val->SmpNum < 4) {
            val->SmpNum = 4;
        } else if (val->SmpNum >= 128) {
            val->SmpNum = 128;
        }
        if (param->Type == 0) {
            val->Smp = pppAlloc(&ppvEnv->heapMng, val->SmpNum * 32);
        } else {
            val->Smp = pppAlloc(&ppvEnv->heapMng, val->SmpNum * 16);
        }
        if (!val->Smp) {
            TRACE(D_00469F88);
            return;
        }
        switch (param->UvRandom) {
        case 0:
            break;
        case 1:
            PPP_RAND_F(f);
            val->TexScrollS = f;
            break;
        case 2:
            PPP_RAND_F(f);
            val->TexScrollT = f;
            break;
        case 3:
            PPP_RAND_F(f);
            val->TexScrollS = f;
            PPP_RAND_F(f);
            val->TexScrollT = f;
            break;
        }
    }
    if (param->Type == 0) {
        if (!param->Stop) {
            get_pos_3D(param, val, param->Width, val->Smp[val->FrameCt * 2], val->Smp[val->FrameCt * 2 + 1]);
            val->GetPosCt++;
            if (val->GetPosCt > val->SmpNum) {
                val->GetPosCt = val->SmpNum;
            }
        }
    } else {
        if (!param->Stop) {
            int n = val->FrameCt;

            get_pos_2D(param, val, val->Smp[n]);
            if (param->Centrifugal) {
                val->Smp[n][3] = val->CentrifugalForce;
            }
            val->GetPosCt++;
            if (val->GetPosCt > val->SmpNum) {
                val->GetPosCt = val->SmpNum;
            }
        }
    }
    if (param->Centrifugal && param->Type) {
        int n;
        sceVu0FVECTOR g;

        *(u_long128 *)g = 0;
        for (i = 0; i < val->GetPosCt; i++) {
            n = val->FrameCt - i;
            if (n < 0) {
                n += val->SmpNum;
            }
            AddVector(g, g, val->Smp[n]);
        }
        SCALE_VECTOR_XYZ(g, 1.0f / val->GetPosCt);
        for (i = 1; i < val->GetPosCt - 2; i++) {
            int n;
            sceVu0FVECTOR v;
            float centrifugal_force;

            n = val->FrameCt - i;
            if (n < 0) {
                n += val->SmpNum;
            }
            centrifugal_force = val->Smp[n][3];
            SubVector(v, val->Smp[n], g);
            Normalize(v);
            SCALE_VECTOR_XYZ(v, centrifugal_force);
            AddVector(val->Smp[n], val->Smp[n], v);
            val->Smp[n][3] = centrifugal_force;
        }
        if (!ppvUserStopPartF && !(ppvMng->bFlagStop || ppvMng->bBoolSlow)) {
            val->CentrifugalForceV += val->CentrifugalForceA;
            val->CentrifugalForce += val->CentrifugalForceV;
            if (param->cdt.time == pobj->time) {
                val->CentrifugalForce += param->CentrifugalForce * 0.01f;
                val->CentrifugalForceV += param->CentrifugalForceV * 0.01f;
                val->CentrifugalForceA += param->CentrifugalForceA * 0.01f;
            }
        }
    }
    if (param->WindMode && param->Type) {
        int n;

        for (i = 1; i < val->GetPosCt; i++) {
            n = val->FrameCt - i;
            if (n < 0) {
                n += val->SmpNum;
            }
            AddVector(val->Smp[n], val->Smp[n], val->Wind);
        }
        if (!ppvUserStopPartF && !(ppvMng->bFlagStop || ppvMng->bBoolSlow)) {
            AddVector(val->WindV, val->WindV, val->WindA);
            AddVector(val->Wind, val->Wind, val->WindV);
            if (param->cdt.time == pobj->time) {
                val->Wind[0] += param->Wind.x * 0.01f;
                val->Wind[1] += param->Wind.y * -0.01f;
                val->Wind[2] += param->Wind.z * -0.01f;
                val->WindV[0] += param->WindV.x * 0.01f;
                val->WindV[1] += param->WindV.y * -0.01f;
                val->WindV[2] += param->WindV.z * -0.01f;
                val->WindA[0] += param->WindA.x * 0.01f;
                val->WindA[1] += param->WindA.y * -0.01f;
                val->WindA[2] += param->WindA.z * -0.01f;
            }
        }
    }
    if (param->GravityMode && param->Type) {
        for (i = 1; i < val->GetPosCt; i++) {
            int n;
            sceVu0FVECTOR v;
            float len;
            float g;
            sceVu0FVECTOR gp;

            n = val->FrameCt - i;
            if (n < 0) {
                n += val->SmpNum;
            }
            if (param->GravityMode == 2) {
                SubVector(v, val->GPoint, val->Smp[n]);
            } else {
                ApplyMatrix(gp, val->CMat, val->GPoint);
                SubVector(v, gp, val->Smp[n]);
            }
            NormalizeLen(len, v);
            g = len / param->GRange;
            if (g < 0.0f) {
                g = 0.0f;
            } else if (g > 1.0f) {
                g = 1.0f;
            }
            g = val->Gravity * (1.0f - g);
            SCALE_VECTOR_XYZ(v, g);
            v[3] = 0.0f;
            AddVector(val->Smp[n], val->Smp[n], v);
        }
        if (!ppvUserStopPartF && !(ppvMng->bFlagStop || ppvMng->bBoolSlow)) {
            AddVector(val->GPointV, val->GPointV, val->GPointA);
            AddVector(val->GPoint, val->GPoint, val->GPointV);
            val->GravityV += val->GravityA;
            val->Gravity += val->GravityV;
            if (param->cdt.time == pobj->time) {
                val->GPoint[0] += param->GPoint.x * 0.01f;
                val->GPoint[1] += param->GPoint.y * -0.01f;
                val->GPoint[2] += param->GPoint.z * -0.01f;
                val->GPointV[0] += param->GPointV.x * 0.01f;
                val->GPointV[1] += param->GPointV.y * -0.01f;
                val->GPointV[2] += param->GPointV.z * -0.01f;
                val->GPointA[0] += param->GPointA.x * 0.01f;
                val->GPointA[1] += param->GPointA.y * -0.01f;
                val->GPointA[2] += param->GPointA.z * -0.01f;
                val->Gravity += param->Gravity * 0.01f;
                val->GravityV += param->GravityV * 0.01f;
                val->GravityA += param->GravityA * 0.01f;
            }
        }
    }
    if (!param->Stop) {
        val->FrameCt++;
        if (val->FrameCt >= val->SmpNum) {
            val->FrameCt = 0;
        }
    }
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly", pppEiDrawShipolyCon);
#if 0
/* store scheduling differs: the original loads ppvMng after all the stores, ours hoists it */
void pppEiDrawShipolyCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VEiDrawShipoly *val = (VEiDrawShipoly *)&pobj->val[ctbl->useVal[DRAWSHIPOLY_THIS]];

    val->TexScrollS = val->Gravity = val->GravityV = val->GravityA = val->Dist = val->DistV = val->DistA =
        val->CentrifugalForce = val->CentrifugalForceV = val->CentrifugalForceA = val->TexScrollT = 0.0f;
    *(u_long128 *)val->Wind = 0;
    val->StartParam = 0;
    *(u_long128 *)val->WindV = 0;
    val->FrameCt = 0;
    *(u_long128 *)val->WindA = 0;
    val->SmpNum = 0;
    *(u_long128 *)val->GPoint = 0;
    val->Standby = 1;
    *(u_long128 *)val->GPointV = 0;
    val->Smp = 0;
    *(u_long128 *)val->GPointA = 0;
    val->GetPosCt = 0;
    ppvMng->bBoolNoViewClip = 1;
}
#endif

void pppEiDrawShipolyCon2(pppPObject *pobj, pppCtrlTable *ctbl) {
    VEiDrawShipoly *val = (VEiDrawShipoly *)&pobj->val[ctbl->useVal[DRAWSHIPOLY_THIS]];
    PEiDrawShipoly *param = (PEiDrawShipoly *)val->StartParam;

    val->GPoint[0] -= param->GPoint.x * 0.01f;
    val->GPoint[1] -= param->GPoint.y * -0.01f;
    val->GPoint[2] -= param->GPoint.z * -0.01f;
    *(u_long128 *)val->GPointV = 0;
    *(u_long128 *)val->GPointA = 0;
    val->Gravity -= param->Gravity * 0.01f;
    val->GravityV = val->GravityA = 0.0f;
    val->Wind[0] -= param->Wind.x * 0.01f;
    val->Wind[1] -= param->Wind.y * -0.01f;
    val->Wind[2] -= param->Wind.z * -0.01f;
    *(u_long128 *)val->WindV = 0;
    *(u_long128 *)val->WindA = 0;
    val->CentrifugalForce -= param->CentrifugalForce * 0.01f;
    val->CentrifugalForceV = val->CentrifugalForceA = 0.0f;
    val->Dist -= param->Dist;
    val->DistV = val->DistA = 0.0f;
}

void pppEiDrawShipolyDes(pppPObject *pobj, pppCtrlTable *ctbl) {
    VEiDrawShipoly *val = (VEiDrawShipoly *)&pobj->val[ctbl->useVal[DRAWSHIPOLY_THIS]];

    if (val->Smp) {
        pppFree(&ppvEnv->heapMng, val->Smp);
        val->Smp = 0;
    }
}
