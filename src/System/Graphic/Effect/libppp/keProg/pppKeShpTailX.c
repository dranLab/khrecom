#include "pppKeShpTailX.h"
#include "Ke.h"
#include "System/Graphic/Effect/libppp/eiProg/pppColor.h"
#include "System/Graphic/Effect/libppp/keProg/pppKeZCrctShp.h"
#include "System/Graphic/Effect/libppp/draw/shape.h"

void pppKeShpTailX(pppPObject *pobj, PKeShpTailX *dat, pppCtrlTable *ctbl) {
    VKeShpTailX *pThis;
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
    pThis = (VKeShpTailX *)&pobj->val[ctbl->useVal[IKeShpTailX_This]];
    if (pobj->time == 0 && pobj->loop_in == 0) {
        if (dat->World) {
            sceVu0ApplyMatrix(fv0, ppvParMatrix, ppvPObj->cmat[3]);
        } else {
            sceVu0CopyVector(fv0, ppvPObj->cmat[3]);
        }
        fv0[3] = 1.0f;
        __asm__ __volatile__(""); /* scheduling barrier, as in pppKeShpTail */
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
    pThis->SeqNow = pThis->Seq;
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

/* VU0 macro-mode helpers (vf26..vf31 as scratch, as in the original). The original used inline functions:
 * their parameters show up as locals in the debug info (Src, _s), and their argument copies change the
 * insn count gcse sees, which decides the stack slots of the loop's spilled invariants. */
static __inline__ void TailFToICol(u_char *d, float *v) {
    __asm__ __volatile__("lqc2 $vf31, 0x0(%0)\n"
                         "\tvftoi0.xyzw $vf31, $vf31\n"
                         "\tqmfc2.ni $2, $vf31\n"
                         "\tsb $2, 0x0(%1)\n"
                         "\tmtsab $0, 0x4\n"
                         "\tqfsrv $2, $2, $2\n"
                         "\tsb $2, 0x1(%1)\n"
                         "\tqfsrv $2, $2, $2\n"
                         "\tsb $2, 0x2(%1)\n"
                         "\tqfsrv $2, $2, $2\n"
                         "\tsb $2, 0x3(%1)"
                         : : "r"(v), "r"(d) : "$2", "memory");
}

static __inline__ void TailScaleXYZ(float *d, float *s, float _s) {
    __asm__ __volatile__("lqc2 $vf31, 0x0(%1)\n"
                         "\tlqc2 $vf29, 0x0(%0)\n"
                         "\tmfc1 $2, %2\n"
                         "\tqmtc2.ni $2, $vf30\n"
                         "\tvmulx.xyz $vf29, $vf31, $vf30x\n"
                         "\tsqc2 $vf29, 0x0(%0)"
                         : : "r"(d), "r"(s), "f"(_s) : "$2");
}

static __inline__ void TailScaleXYZW(float *d, float *s, float _s) {
    __asm__ __volatile__("lqc2 $vf31, 0x0(%1)\n"
                         "\tmfc1 $2, %2\n"
                         "\tqmtc2.ni $2, $vf30\n"
                         "\tvmulx.xyzw $vf31, $vf31, $vf30x\n"
                         "\tsqc2 $vf31, 0x0(%0)"
                         : : "r"(d), "r"(s), "f"(_s) : "$2");
}

static __inline__ void TailCopy(float *d, float *_s) {
    __asm__ __volatile__("lqc2 $vf31, 0x0(%1)\n\tsqc2 $vf31, 0x0(%0)" : : "r"(d), "r"(_s));
}

static __inline__ void TailSub(float *d, float *t, float *s) {
    __asm__ __volatile__("lqc2 $vf31, 0x0(%1)\n"
                         "\tlqc2 $vf30, 0x0(%2)\n"
                         "\tvsub.xyzw $vf31, $vf31, $vf30\n"
                         "\tsqc2 $vf31, 0x0(%0)"
                         : : "r"(d), "r"(t), "r"(s));
}

static __inline__ void TailAdd(float *d, float *t, float *s) {
    __asm__ __volatile__("lqc2 $vf31, 0x0(%1)\n"
                         "\tlqc2 $vf30, 0x0(%2)\n"
                         "\tvadd.xyzw $vf31, $vf31, $vf30\n"
                         "\tsqc2 $vf31, 0x0(%0)"
                         : : "r"(d), "r"(t), "r"(s));
}

static __inline__ float TailLen(float *v) {
    float r;

    __asm__ __volatile__("lqc2 $vf31, 0x0(%1)\n"
                         "\tvmul.xyz $vf31, $vf31, $vf31\n"
                         "\tvaddy.x $vf31, $vf31, $vf31y\n"
                         "\tvaddz.x $vf31, $vf31, $vf31z\n"
                         "\tvsqrt $Q, $vf31x\n"
                         "\tvwaitq\n"
                         "\tvaddq.x $vf31, $vf0, $Q\n"
                         "\tqmfc2.ni $2, $vf31\n"
                         "\tmtc1 $2, %0"
                         : "=f"(r) : "r"(v) : "$2");
    return r;
}

static __inline__ void TailApply(float *d, sceVu0FVECTOR *m, float *v) {
    __asm__ __volatile__("lqc2 $vf27, 0x0(%1)\n"
                         "\tlqc2 $vf28, 0x10(%1)\n"
                         "\tlqc2 $vf29, 0x20(%1)\n"
                         "\tlqc2 $vf30, 0x30(%1)\n"
                         "\tlqc2 $vf31, 0x0(%2)\n"
                         "\tvmulax.xyzw $ACC, $vf27, $vf31x\n"
                         "\tvmadday.xyzw $ACC, $vf28, $vf31y\n"
                         "\tvmaddaz.xyzw $ACC, $vf29, $vf31z\n"
                         "\tvmaddw.xyzw $vf27, $vf30, $vf31w\n"
                         "\tsqc2 $vf27, 0x0(%0)"
                         : : "r"(d), "r"(m), "r"(v) : "memory");
}

#define TAIL_MULMAT_ROW(o)                                                              \
    "\tlqc2 $vf31, " o "(%0)\n"                                                         \
    "\tvmulax.xyzw $ACC, $vf27, $vf31x\n"                                               \
    "\tvmadday.xyzw $ACC, $vf28, $vf31y\n"                                              \
    "\tvmaddaz.xyzw $ACC, $vf29, $vf31z\n"                                              \
    "\tvmaddw.xyzw $vf26, $vf30, $vf31w\n"                                              \
    "\tsqc2 $vf26, " o "(%0)\n"

static __inline__ void TailMulMatrix(sceVu0FVECTOR *d, sceVu0FVECTOR *m) {
    __asm__ __volatile__("lqc2 $vf27, 0x0(%1)\n"
                         "\tlqc2 $vf28, 0x10(%1)\n"
                         "\tlqc2 $vf29, 0x20(%1)\n"
                         "\tlqc2 $vf30, 0x30(%1)\n"
                         TAIL_MULMAT_ROW("0x0") TAIL_MULMAT_ROW("0x10")
                         TAIL_MULMAT_ROW("0x20") TAIL_MULMAT_ROW("0x30")
                         : : "r"(d), "r"(m) : "memory");
}

static __inline__ void TailHcToF(float *fv, short *Src) {
    fv[0] = Src[0] >> 7;
    fv[1] = Src[1] >> 7;
    fv[2] = Src[2] >> 7;
    fv[3] = Src[3] >> 7;
}

void pppKeShpTailXDraw(pppPObject *pobj, PKeShpTailX *dat, pppCtrlTable *ctbl) {
    sceVu0FMATRIX mat0;
    sceVu0FMATRIX mat1;
    sceVu0FVECTOR fv0;
    sceVu0FVECTOR fv1;
    sceVu0FVECTOR fv2;
    sceVu0FVECTOR fv3;
    sceVu0FVECTOR col;
    sceVu0FVECTOR colSft;
    sceVu0FVECTOR *pFv0;
    VKeShpTailX *pThis;
    VColor *pCol;
    OSHAPE_IN shi;
    OSHAPE_VU1 *sh;
    VKeZCrctShp *pZ;
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
    pCol = (VColor *)&pobj->val[ctbl->useVal[IKeShpTailX_Color]];
    col[0] = pCol->release_col.r;
    col[1] = pCol->release_col.g;
    col[2] = pCol->release_col.b;
    col[3] = pCol->release_col.a;
    i = dat->Num;
    f = i - 1;
    TailHcToF(fv0, &dat->LastCol.r);
    sceVu0SubVector(fv1, col, fv0);
    sceVu0DivVector(colSft, fv1, f);
    pThis = (VKeShpTailX *)&pobj->val[ctbl->useVal[IKeShpTailX_This]];
    shp = &ppvEnv->lpShape[dat->Shp];
    *(int *)&shi.toff = 0;
    shi.pri = 0;
    shi.pmat = &mat1;
    if (dat->draw_mode == 0) {
        shi.flag = 0x80;
    } else {
        shi.flag = 0;
    }
    if (dat->ztest == 1) {
        shi.flag |= 0x10;
    }
    if (dat->fog) {
        shi.flag |= 0x800;
    }
    if (dat->near) {
        shi.flag |= 0x400;
    }
    if (dat->zCrct) {
        pZ = (VKeZCrctShp *)&pobj->val[ctbl->useVal[IKeShpTailX_KeZCrctShp]];
        shi.pzmat = &pZ->Mat;
    } else {
        shi.pzmat = 0;
    }
    shi.ot = 0;
    shi.z = 0;
    ah = (OANM_HEADER *)shp->shape;
    seq = &ah->seq[pThis->SeqNow];
    sh = (OSHAPE_VU1 *)((char *)ah + seq->sha);
    scl = 1.0f;
    sceVu0CopyMatrix(mat0, ppvPObj->cmat);
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
    /* Hack (as in pppKeShpTail): an empty asm takes an issue slot here, so sched1 keeps the loop-invariant
     * col address after the KeVu0VecLen call. */
    __asm__("" : "+f"(sclSft));
    sceVu0SubVector(fv2, fv3, fv1);
    f = KeVu0VecLen(fv2);
    c = f;
    while (1) {
        TailFToICol(&shi.r, col);
        {
            float *_p = ppvParScl;

            TailScaleXYZ(mat1[0], mat0[0], scl * _p[0]);
            TailScaleXYZ(mat1[1], mat0[1], scl * _p[1]);
        }
        if (dat->World) {
            TailApply(mat1[3], ppvCameraMatrix, fv0);
        } else {
            TailApply(mat1[3], ppvWorldMatrix, fv0);
        }
        TailMulMatrix(mat1, ppvScreenMatrix);
        shi.packet = ppvCurPrimp;
        i--;
        ppvCurPrimp = op_shape_draw(sh, &shi, shp->nPacketSize);
        if (i == 0) {
            return;
        }
        TailSub(col, col, colSft);
        d -= e;
        scl -= sclSft;
        if (d <= 0.0f) {
            return;
        }
        for (;;) {
            if (d <= c) {
                TailScaleXYZW(fv0, fv2, b / f);
                TailAdd(fv0, fv1, fv0);
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
            TailCopy(fv1, fv3);
            TailCopy(fv3, pFv0[j]);
            TailSub(fv2, fv3, fv1);
            b -= f;
            f = TailLen(fv2);
            c += f;
        }
    }
}


void pppKeShpTailXCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeShpTailX *pThis = (VKeShpTailX *)&pobj->val[ctbl->useVal[IKeShpTailX_This]];

    pThis->StkNum = 31;
    pThis->Seq = 0;
    pThis->SeqNow = 0;
    pThis->Cur = 0;
    pThis->Cnt = 0;
}
