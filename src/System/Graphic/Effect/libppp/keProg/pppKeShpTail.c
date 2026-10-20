#include "pppKeShpTail.h"
#include "Ke.h"
#include "System/Graphic/Effect/libppp/eiProg/pppColor.h"
#include "System/Graphic/Effect/libppp/draw/shape.h"

void pppKeShpTail(pppPObject *pobj, PKeShpTail *dat, pppCtrlTable *ctbl) {
    VKeShpTail *pThis;
    sceVu0FVECTOR fv0;
    sceVu0FVECTOR *pFv;
    int i;
    OANM_HEADER *ah;
    OANM_SEQ *seq;
    pppShapeSt *shp;

    if (ppvUserStopPartF) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    pThis = (VKeShpTail *)&pobj->val[ctbl->useVal[IKeShpTail_This]];
    if (pobj->time == 0 && pobj->loop_in == 0) {
        if (dat->World) {
            sceVu0ApplyMatrix(fv0, ppvParMatrix, ppvPObj->cmat[3]);
        } else {
            sceVu0CopyVector(fv0, ppvPObj->cmat[3]);
        }
        fv0[3] = 1.0f;
        __asm__ __volatile__(""); /* scheduling barrier: keeps the store before the loop setup as in the original */
        pFv = pThis->Stk;
        for (i = pThis->StkNum; i > 0; i--) {
            sceVu0CopyVector(*pFv, fv0);
            pFv++;
        }
    }
    if (pThis->Cur == 0) {
        pThis->Cur = pThis->StkNum;
    }
    pThis->Cur--;
    if (dat->World) {
        sceVu0ApplyMatrix(pThis->Stk[pThis->Cur], ppvParMatrix, ppvPObj->cmat[3]);
        pThis->Stk[pThis->Cur][3] = 1.0f;
    } else {
        KeVu0CopyVecXYZ(pThis->Stk[pThis->Cur], ppvPObj->cmat[3]);
    }
    shp = &ppvEnv->lpShape[dat->Shp];
    ah = (OANM_HEADER *)shp->shape;
    seq = &ah->seq[pThis->Seq];
    pThis->Cnt += dat->AnmSpd;
    if (pThis->Cnt >= seq->wait) {
        unsigned short s = pThis->Seq;

        pThis->Cnt -= seq->wait;
        pThis->Seq = s + 1;
        if (pThis->Seq >= ah->seqc) {
            if (seq->flag & 0x80) {
                pThis->Cnt = pThis->Seq = 0;
            } else {
                pThis->Seq = s;
                pThis->Cnt = 0;
            }
        }
    }
}

/* VU0 macro-mode helpers (vf26..vf31 as scratch, as in the original) */
#define TAIL_SCALE_XYZ(d, s, v)                                                         \
    {                                                                                   \
        float _s = (v);                                                                 \
        __asm__ __volatile__("lqc2 $vf31, 0x0(%1)\n"                                  \
                             "\tlqc2 $vf29, 0x0(%0)\n"                                \
                             "\tmfc1 $2, %2\n"                                        \
                             "\tqmtc2.ni $2, $vf30\n"                                 \
                             "\tvmulx.xyz $vf29, $vf31, $vf30x\n"                     \
                             "\tsqc2 $vf29, 0x0(%0)"                                   \
                             : : "r"(d), "r"(s), "f"(_s) : "$2", "memory");             \
    }

#define TAIL_SCALE_XYZW(d, s, v)                                                        \
    {                                                                                   \
        float _s = (v);                                                                 \
        __asm__ __volatile__("lqc2 $vf31, 0x0(%1)\n"                                  \
                             "\tmfc1 $2, %2\n"                                        \
                             "\tqmtc2.ni $2, $vf30\n"                                 \
                             "\tvmulx.xyzw $vf31, $vf31, $vf30x\n"                    \
                             "\tsqc2 $vf31, 0x0(%0)"                                   \
                             : : "r"(d), "r"(s), "f"(_s) : "$2");             \
    }

#define TAIL_COPY(d, s) __asm__ __volatile__("lqc2 $vf31, 0x0(%1)\n\tsqc2 $vf31, 0x0(%0)" : : "r"(d), "r"(s))

#define TAIL_SUB(d, t, s)                                                               \
    __asm__ __volatile__("lqc2 $vf31, 0x0(%1)\n"                                        \
                         "\tlqc2 $vf30, 0x0(%2)\n"                                      \
                         "\tvsub.xyzw $vf31, $vf31, $vf30\n"                            \
                         "\tsqc2 $vf31, 0x0(%0)"                                        \
                         : : "r"(d), "r"(t), "r"(s))

#define TAIL_ADD(d, t, s)                                                               \
    __asm__ __volatile__("lqc2 $vf31, 0x0(%1)\n"                                        \
                         "\tlqc2 $vf30, 0x0(%2)\n"                                      \
                         "\tvadd.xyzw $vf31, $vf31, $vf30\n"                            \
                         "\tsqc2 $vf31, 0x0(%0)"                                        \
                         : : "r"(d), "r"(t), "r"(s))

#define TAIL_LEN(r, v)                                                                  \
    __asm__ __volatile__("lqc2 $vf31, 0x0(%1)\n"                                        \
                         "\tvmul.xyz $vf31, $vf31, $vf31\n"                             \
                         "\tvaddy.x $vf31, $vf31, $vf31y\n"                             \
                         "\tvaddz.x $vf31, $vf31, $vf31z\n"                             \
                         "\tvsqrt $Q, $vf31x\n"                                         \
                         "\tvwaitq\n"                                                   \
                         "\tvaddq.x $vf31, $vf0, $Q\n"                                  \
                         "\tqmfc2.ni $2, $vf31\n"                                       \
                         "\tmtc1 $2, %0"                                                \
                         : "=f"(r) : "r"(v) : "$2")

#define TAIL_APPLY(d, m, v)                                                             \
    __asm__ __volatile__("lqc2 $vf27, 0x0(%1)\n"                                        \
                         "\tlqc2 $vf28, 0x10(%1)\n"                                     \
                         "\tlqc2 $vf29, 0x20(%1)\n"                                     \
                         "\tlqc2 $vf30, 0x30(%1)\n"                                     \
                         "\tlqc2 $vf31, 0x0(%2)\n"                                      \
                         "\tvmulax.xyzw $ACC, $vf27, $vf31x\n"                          \
                         "\tvmadday.xyzw $ACC, $vf28, $vf31y\n"                         \
                         "\tvmaddaz.xyzw $ACC, $vf29, $vf31z\n"                         \
                         "\tvmaddw.xyzw $vf27, $vf30, $vf31w\n"                         \
                         "\tsqc2 $vf27, 0x0(%0)"                                        \
                         : : "r"(d), "r"(m), "r"(v) : "memory")

#define TAIL_MULMAT_ROW(o)                                                              \
    "\tlqc2 $vf31, " o "(%0)\n"                                                         \
    "\tvmulax.xyzw $ACC, $vf27, $vf31x\n"                                               \
    "\tvmadday.xyzw $ACC, $vf28, $vf31y\n"                                              \
    "\tvmaddaz.xyzw $ACC, $vf29, $vf31z\n"                                              \
    "\tvmaddw.xyzw $vf26, $vf30, $vf31w\n"                                              \
    "\tsqc2 $vf26, " o "(%0)\n"

#define TAIL_MULMAT(d, m)                                                               \
    __asm__ __volatile__("lqc2 $vf27, 0x0(%1)\n"                                        \
                         "\tlqc2 $vf28, 0x10(%1)\n"                                     \
                         "\tlqc2 $vf29, 0x20(%1)\n"                                     \
                         "\tlqc2 $vf30, 0x30(%1)\n"                                     \
                         TAIL_MULMAT_ROW("0x0") TAIL_MULMAT_ROW("0x10")                 \
                         TAIL_MULMAT_ROW("0x20") TAIL_MULMAT_ROW("0x30")                \
                         : : "r"(d), "r"(m) : "memory")

#define TAIL_FTOI_COL(d, v)                                                             \
    __asm__ __volatile__("lqc2 $vf31, 0x0(%0)\n"                                        \
                         "\tvftoi0.xyzw $vf31, $vf31\n"                                 \
                         "\tqmfc2.ni $2, $vf31\n"                                       \
                         "\tsb $2, 0x0(%1)\n"                                           \
                         "\tmtsab $0, 0x4\n"                                            \
                         "\tqfsrv $2, $2, $2\n"                                         \
                         "\tsb $2, 0x1(%1)\n"                                           \
                         "\tqfsrv $2, $2, $2\n"                                         \
                         "\tsb $2, 0x2(%1)\n"                                           \
                         "\tqfsrv $2, $2, $2\n"                                         \
                         "\tsb $2, 0x3(%1)"                                             \
                         : : "r"(v), "r"(d) : "$2", "memory")

static __inline__ void TailHcToF(float *fv, short *Src) {
    fv[0] = Src[0] >> 7;
    fv[1] = Src[1] >> 7;
    fv[2] = Src[2] >> 7;
    fv[3] = Src[3] >> 7;
}

void pppKeShpTailDraw(pppPObject *pobj, PKeShpTail *dat, pppCtrlTable *ctbl) {
    sceVu0FMATRIX mat0;
    sceVu0FMATRIX mat1;
    sceVu0FVECTOR fv0;
    sceVu0FVECTOR fv1;
    sceVu0FVECTOR fv2;
    sceVu0FVECTOR fv3;
    sceVu0FVECTOR col;
    sceVu0FVECTOR colSft;
    sceVu0FVECTOR *pFv0;
    VKeShpTail *pThis;
    VColor *pCol;
    OSHAPE_IN shi;
    OSHAPE_VU1 *sh;
    OANM_HEADER *ah;
    OANM_SEQ *seq;
    pppShapeSt *shp;
    float f;
    float e;
    float d;
    float c;
    float b;
    float scl;
    float sclSft;
    int i;
    int j;
    int k;
    int m;

    if (dat->Shp == 0xFFFF) {
        return;
    }
    pCol = (VColor *)&pobj->val[ctbl->useVal[IKeShpTail_Color]];
    col[0] = pCol->release_col.r;
    col[1] = pCol->release_col.g;
    col[2] = pCol->release_col.b;
    col[3] = pCol->release_col.a;
    i = dat->Num;
    f = i - 1;
    TailHcToF(fv0, &dat->LastCol.r);
    sceVu0SubVector(fv1, col, fv0);
    sceVu0DivVector(colSft, fv1, f);
    pThis = (VKeShpTail *)&pobj->val[ctbl->useVal[IKeShpTail_This]];
    shp = &ppvEnv->lpShape[dat->Shp];
    *(int *)&shi.toff = 0;
    shi.pri = 0;
    shi.pmat = &mat1;
    shi.pzmat = 0;
    if (dat->draw_mode == 0) {
        shi.flag = 0x80;
    } else {
        shi.flag = 0;
    }
    if (dat->ztest == 1) {
        shi.flag |= 0x10;
    }
    shi.ot = 0;
    shi.z = 0;
    ah = (OANM_HEADER *)shp->shape;
    seq = &ah->seq[pThis->SeqNow];
    sh = (OSHAPE_VU1 *)((char *)ah + seq->sha);
    sceVu0CopyMatrix(mat0, ppvPObj->cmat);
    {
        float *_p = ppvParScl;

        TAIL_SCALE_XYZ(mat0[0], mat0[0], _p[0]);
        TAIL_SCALE_XYZ(mat0[1], mat0[1], _p[1]);
    }
    scl = 1.0f;
    sceVu0UnitMatrix(mat1);
    sclSft = (scl - dat->LastScl) / f;
    d = dat->Sep;
    if (dat->World) {
        d *= 0.01f;
    }
    e = d * sclSft;
    if (d == 0.0f) {
        i = 0;
    }
    pFv0 = pThis->Stk;
    j = pThis->Cur;
    m = pThis->StkNum - 1;
    k = j;
    sceVu0CopyVector(fv0, pFv0[j]);
    j++;
    sceVu0CopyVector(fv1, fv0);
    if (k == m) {
        j = 0;
    }
    sceVu0CopyVector(fv3, pFv0[j]);
    b = d;
    /* Hack: an empty asm takes an issue slot in this block, which keeps sched1 from moving the hoisted
     * %hi(ppvCurPrimp) up; with it, global-alloc gives s8 to that instead of to &fv0. */
    __asm__("" : "+f"(sclSft));
    sceVu0SubVector(fv2, fv3, fv1);
    c = KeVu0VecLen(fv2);
    f = c;
    while (1) {
        TAIL_FTOI_COL(&shi.r, col);
        TAIL_SCALE_XYZ(mat1[0], mat0[0], scl);
        TAIL_SCALE_XYZ(mat1[1], mat0[1], scl);
        if (dat->World) {
            TAIL_APPLY(mat1[3], ppvCameraMatrix, fv0);
        } else {
            TAIL_APPLY(mat1[3], ppvWorldMatrix, fv0);
        }
        TAIL_MULMAT(mat1, ppvScreenMatrix);
        shi.packet = ppvCurPrimp;
        i--;
        ppvCurPrimp = op_shape_draw(sh, &shi, shp->nPacketSize);
        if (i == 0) {
            return;
        }
        TAIL_SUB(col, col, colSft);
        d -= e;
        scl -= sclSft;
        if (d <= 0.0f) {
            return;
        }
        for (;;) {
            if (d <= c) {
                TAIL_SCALE_XYZW(fv0, fv2, b / f);
                TAIL_ADD(fv0, fv1, fv0);
                b += d;
                c -= d;
                break;
            }
            if (j++ == m) {
                j = 0;
            }
            if (j == k) {
                return;
            }
            TAIL_COPY(fv1, fv3);
            TAIL_COPY(fv3, (float *)((j << 4) + (u_int)pFv0));
            TAIL_SUB(fv2, fv3, fv1);
            b -= f;
            TAIL_LEN(f, fv2);
            c += f;
        }
    }
}

void pppKeShpTailCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeShpTail *pThis = (VKeShpTail *)&pobj->val[ctbl->useVal[IKeShpTail_This]];

    pThis->StkNum = 31;
    pThis->Seq = 0;
    pThis->SeqNow = 0;
    pThis->Cur = 0;
    pThis->Cnt = 0;
}
