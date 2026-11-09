#include "pppKeShpTail3X.h"
#include "Ke.h"
#include "System/Graphic/Effect/libppp/eiProg/pppColor.h"
#include "System/Graphic/Effect/libppp/keProg/pppKeZCrctShp.h"
#include "System/Graphic/Effect/libppp/draw/shape.h"
#include "System/Graphic/Effect/libppp/draw/sin_t.h"

int rand(void);

void pppKeShpTail3X(pppPObject *pobj, PKeShpTail3X *dat, pppCtrlTable *ctbl) {
    VKeShpTail3X *pThis;
    sceVu0FVECTOR fv0;
    sceVu0FVECTOR *pFv;
    int i;

    if (ppvUserStopPartF) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    pThis = (VKeShpTail3X *)&pobj->val[ctbl->useVal[IKeShpTail3X_This]];
    if (pobj->time == 0 && pobj->loop_in == 0) {
        if (dat->World) {
            sceVu0ApplyMatrix(fv0, ppvParMatrix, ppvPObj->cmat[3]);
        } else {
            sceVu0CopyVector(fv0, ppvPObj->cmat[3]);
        }
        fv0[3] = 1.0f;
        __asm__ __volatile__(""); /* scheduling barrier, as in pppKeShpTail */
        pFv = pThis->Stk;
        for (i = 28; i > 0; i--) {
            sceVu0CopyVector(*pFv, fv0);
            pFv++;
        }
    }
    if (pThis->Cur == 0) {
        pThis->Cur = 28;
    }
    pThis->Cur--;
    if (dat->World) {
        sceVu0ApplyMatrix(pThis->Stk[pThis->Cur], ppvParMatrix, ppvPObj->cmat[3]);
        pThis->Stk[pThis->Cur][3] = 1.0f;
    } else {
        sceVu0CopyVector(pThis->Stk[pThis->Cur], ppvPObj->cmat[3]);
    }
    pThis->Col0Mov.r += pThis->Col0Acc.r;
    pThis->Col0.r += pThis->Col0Mov.r;
    pThis->Col0Mov.g += pThis->Col0Acc.g;
    pThis->Col0.g += pThis->Col0Mov.g;
    pThis->Col0Mov.b += pThis->Col0Acc.b;
    pThis->Col0.b += pThis->Col0Mov.b;
    pThis->Col0Mov.a += pThis->Col0Acc.a;
    pThis->Col0.a += pThis->Col0Mov.a;
    pThis->Col1Mov.r += pThis->Col1Acc.r;
    pThis->Col1.r += pThis->Col1Mov.r;
    pThis->Col1Mov.g += pThis->Col1Acc.g;
    pThis->Col1.g += pThis->Col1Mov.g;
    pThis->Col1Mov.b += pThis->Col1Acc.b;
    pThis->Col1.b += pThis->Col1Mov.b;
    pThis->Col1Mov.a += pThis->Col1Acc.a;
    pThis->Col1.a += pThis->Col1Mov.a;
    if (pobj->time == dat->cdt.time) {
        pThis->Col0.r += dat->Col0.r;
        pThis->Col0.g += dat->Col0.g;
        pThis->Col0.b += dat->Col0.b;
        pThis->Col0.a += dat->Col0.a;
        pThis->Col0Mov.r += dat->Col0Mov.r;
        pThis->Col0Mov.g += dat->Col0Mov.g;
        pThis->Col0Mov.b += dat->Col0Mov.b;
        pThis->Col0Mov.a += dat->Col0Mov.a;
        pThis->Col0Acc.r += dat->Col0Acc.r;
        pThis->Col0Acc.g += dat->Col0Acc.g;
        pThis->Col0Acc.b += dat->Col0Acc.b;
        pThis->Col0Acc.a += dat->Col0Acc.a;
        pThis->Col1.r += dat->Col1.r;
        pThis->Col1.g += dat->Col1.g;
        pThis->Col1.b += dat->Col1.b;
        pThis->Col1.a += dat->Col1.a;
        pThis->Col1Mov.r += dat->Col1Mov.r;
        pThis->Col1Mov.g += dat->Col1Mov.g;
        pThis->Col1Mov.b += dat->Col1Mov.b;
        pThis->Col1Mov.a += dat->Col1Mov.a;
        pThis->Col1Acc.r += dat->Col1Acc.r;
        pThis->Col1Acc.g += dat->Col1Acc.g;
        pThis->Col1Acc.b += dat->Col1Acc.b;
        pThis->Col1Acc.a += dat->Col1Acc.a;
    }
}

/* VU0 macro-mode helpers (vf26..vf31 as scratch, as in the original) */
#define TAIL_SUB(d, t, s)                                                               \
    __asm__ __volatile__("lqc2 $vf31, 0x0(%1)\n"                                        \
                         "\tlqc2 $vf30, 0x0(%2)\n"                                      \
                         "\tvsub.xyzw $vf31, $vf31, $vf30\n"                            \
                         "\tsqc2 $vf31, 0x0(%0)"                                        \
                         : : "r"(d), "r"(t), "r"(s))

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

/* the work matrices and vectors kept in vf registers across the loop */
#define TAIL3X_LOAD(pw)                                                                  \
    __asm__ __volatile__("lqc2 $vf10, 0x0(%0)\n"                                        \
                         "\tlqc2 $vf11, 0x10(%0)\n"                                     \
                         "\tlqc2 $vf12, 0x20(%0)\n"                                     \
                         "\tlqc2 $vf13, 0x30(%0)\n"                                     \
                         "\tlqc2 $vf14, 0x40(%0)\n"                                     \
                         "\tlqc2 $vf15, 0x50(%0)\n"                                     \
                         "\tlqc2 $vf16, 0x60(%0)\n"                                     \
                         "\tlqc2 $vf17, 0x70(%0)\n"                                     \
                         "\tlqc2 $vf6, 0x80(%0)\n"                                      \
                         "\tlqc2 $vf7, 0x90(%0)\n"                                      \
                         "\tlqc2 $vf26, 0x100(%0)\n"                                    \
                         "\tlqc2 $vf27, 0x110(%0)\n"                                    \
                         "\tlqc2 $vf28, 0x120(%0)"                                      \
                         : : "r"(pw))

#define TAIL3X_SAVE(pw)                                                                  \
    __asm__ __volatile__("sqc2 $vf2, 0xc0(%0)\n"                                        \
                         "\tsqc2 $vf3, 0xd0(%0)\n"                                      \
                         "\tsqc2 $vf4, 0xe0(%0)\n"                                      \
                         "\tsqc2 $vf5, 0xf0(%0)\n"                                      \
                         "\tsqc2 $vf26, 0x100(%0)\n"                                    \
                         "\tsqc2 $vf27, 0x110(%0)\n"                                    \
                         "\tsqc2 $vf28, 0x120(%0)"                                      \
                         : : "r"(pw))

#define TAIL3X_MULROW(r)                                                                 \
    "\tvmulax.xyzw $ACC, $vf14, " r "x\n"                                               \
    "\tvmadday.xyzw $ACC, $vf15, " r "y\n"                                              \
    "\tvmaddaz.xyzw $ACC, $vf16, " r "z\n"                                              \
    "\tvmaddw.xyzw " r ", $vf17, " r "w\n"

#define TAIL3X_COPYMAT(d, s)                                                             \
    {                                                                                   \
        ((u_long128 *)(d))[0] = ((u_long128 *)(s))[0];                                  \
        ((u_long128 *)(d))[1] = ((u_long128 *)(s))[1];                                  \
        ((u_long128 *)(d))[2] = ((u_long128 *)(s))[2];                                  \
        ((u_long128 *)(d))[3] = ((u_long128 *)(s))[3];                                  \
    }

static __inline__ void TailHcToF(float *fv, short *Src, float a) {
    fv[0] = Src[0] >> 7;
    fv[1] = Src[1] >> 7;
    fv[2] = Src[2] >> 7;
    fv[3] = (Src[3] >> 7) * a;
}

void pppKeShpTail3XDraw(pppPObject *pobj, PKeShpTail3X *dat, pppCtrlTable *ctbl) {
    sceVu0FMATRIX mat0;
    sceVu0FMATRIX mat1;
    sceVu0FVECTOR fv0;
    sceVu0FVECTOR fv1;
    sceVu0FVECTOR fv2;
    sceVu0FVECTOR fv3;
    sceVu0FVECTOR fvw;
    sceVu0FVECTOR col;
    sceVu0FVECTOR colSft;
    sceVu0FVECTOR *pFv0;
    VKeShpTail3X *pThis;
    VColor *pCol;
    OSHAPE_IN shi;
    OSHAPE_VU1 *sh;
    VKeZCrctShp *pZ;
    OANM_HEADER *ah;
    pppShapeSt *shp;
    float f;
    float e;
    float d;
    float c;
    float b;
    float scl;
    float sclSft;
    float fRnd;
    int i;
    int j;
    int k;
    u_int cnt;
    u_int cntRnd;
    u_int rnd;
    u_int cntBas;
    u_int seqBas;
    struct {
        sceVu0FMATRIX WMat;
        sceVu0FMATRIX SMat;
        sceVu0FMATRIX CMat;
        sceVu0FMATRIX AMat;
        sceVu0FVECTOR FV3;
        sceVu0FVECTOR FV2;
        sceVu0FVECTOR FV1;
        sceVu0FVECTOR _P0;
        sceVu0FMATRIX ZMat;
    } *pw;
    /* MATCH: a nested function: pw lives in memory (a nested function uses it) and the inlined copy addresses it
     * through the static chain, a copy of the frame base ("move a3,sp"; "lw 0x120(a3)" after the if/else join) */
    auto __inline__ void SetMat(int World) {
        if (World) {
            TAIL3X_COPYMAT(pw->WMat, ppvCameraMatrix);
        } else {
            TAIL3X_COPYMAT(pw->WMat, ppvWorldMatrix);
        }
        TAIL3X_COPYMAT(pw->SMat, ppvScreenMatrix);
        /* MATCH: not a struct-field store, so the ppvPObj load below stays after it (alias.c) */
        ((float *)pw->AMat)[3] = 0.0f;
        ((float *)pw->AMat)[7] = 0.0f;
        __asm__ __volatile__("lqc2 $vf31, 0x0(%1)\n"
                             "\tlqc2 $vf30, 0x0(%2)\n"
                             "\tlqc2 $vf29, 0x10(%2)\n"
                             "\tvmulx.xyz $vf28, $vf30, $vf31x\n"
                             "\tvmuly.xyz $vf27, $vf29, $vf31y\n"
                             "\tsqc2 $vf28, 0x80(%0)\n"
                             "\tsqc2 $vf27, 0x90(%0)"
                             : : "r"(pw), "r"(ppvParScl), "r"(ppvPObj->cmat));
    }

    if (dat->Shp == 0xFFFF) {
        return;
    }
    pThis = (VKeShpTail3X *)&pobj->val[ctbl->useVal[IKeShpTail3X_This]];
    pw = op_spr_alloc(0x180);
    pCol = (VColor *)&pobj->val[ctbl->useVal[IKeShpTail3X_Color]];
    d = pCol->release_col.a * (1.0f / 128.0f);
    TailHcToF(col, &pThis->Col0.r, d);
    TailHcToF(fv0, &pThis->Col1.r, d);
    i = dat->Num;
    f = i - 1;
    sceVu0SubVector(fv1, col, fv0);
    sceVu0DivVector(colSft, fv1, f);
    shp = &ppvEnv->lpShape[dat->Shp];
    shi.toff = shi.coff = shi.ot = shi.z = 0;
    shi.pri = 0;
    shi.pmat = &pw->AMat;
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
        pZ = (VKeZCrctShp *)&pobj->val[ctbl->useVal[IKeShpTail3X_KeZCrctShp]];
        shi.pzmat = &pZ->Mat;
    } else {
        shi.pzmat = 0;
    }
    ah = (OANM_HEADER *)shp->shape;
    sceVu0CopyMatrix(mat0, ppvPObj->cmat);
    sceVu0UnitMatrix(mat1);
    scl = dat->Scl0;
    sclSft = (scl - dat->Scl1) / f;
    d = dat->Sep;
    if (dat->World) {
        d *= 0.01f;
    }
    e = d * (sclSft / scl);
    if (d == 0.0f) {
        i = 0;
    }
    pFv0 = pThis->Stk;
    j = pThis->Cur;
    k = j;
    sceVu0CopyVector(fv0, pFv0[j]);
    j++;
    b = d;
    sceVu0CopyVector(fv1, fv0);
    if (k == 27) {
        j = 0;
    }
    sceVu0CopyVector(fv3, pFv0[j]);
    sceVu0SubVector(fv2, fv3, fv1);
    c = KeVu0VecLen(fv2);
    f = c;
    cnt = pThis->Cnt;
    cntBas = ah->seq[0].wait;
    seqBas = ah->seqc;
    rnd = pThis->Seed;
    SetMat(dat->World);
    TAIL3X_LOAD(pw);
    __asm__ __volatile__("lqc2 $vf29, 0x0(%0)\n"
                         "\tlqc2 $vf28, 0x0(%1)\n"
                         "\tlqc2 $vf27, 0x0(%2)\n"
                         "\tlqc2 $vf26, 0x0(%3)\n"
                         "\tvsub.xyzw $vf2, $vf2, $vf2\n"
                         "\tvsub.xyzw $vf3, $vf3, $vf3\n"
                         "\tvsub.xyzw $vf4, $vf4, $vf4\n"
                         "\tvsub.xyzw $vf5, $vf5, $vf5"
                         : : "r"(fv0), "r"(fv1), "r"(fv2), "r"(fv3));
    if (dat->RtDraw == 0) {
        goto next;
    }
    while (1) {
        rnd = (rnd * 0x80D + 7) & 0xFFFF;
        fRnd = rnd / 65535.0f;
        cntRnd = (cnt + rnd) / cntBas % seqBas;
        sh = (OSHAPE_VU1 *)((char *)ah + ah->seq[cntRnd].sha);
        TAIL_FTOI_COL(&shi.r, col);
        fRnd = 1.0f - fRnd * dat->RndScl;
        fvw[0] = scl * fRnd;
        __asm__ __volatile__("lqc2 $vf31, 0x0(%0)\n"
                             "\tvmulx.xyz $vf2, $vf6, $vf31x\n"
                             "\tvmulx.xyz $vf3, $vf7, $vf31x\n"
                             "\tvmulax.xyzw $ACC, $vf10, $vf29x\n"
                             "\tvmadday.xyzw $ACC, $vf11, $vf29y\n"
                             "\tvmaddaz.xyzw $ACC, $vf12, $vf29z\n"
                             "\tvmaddw.xyzw $vf5, $vf13, $vf0w"
                             : : "r"(fvw));
        if (dat->RndRot) {
            fvw[0] = eff_sin_t[rnd >> 4];
            fvw[1] = eff_sin_t[(u_short)(rnd + 0x4000) >> 4];
            __asm__ __volatile__("lqc2 $vf22, 0x0(%0)\n"
                                 "\tvmove.xyzw $vf23, $vf2\n"
                                 "\tvmulay.xyzw $ACC, $vf2, $vf22y\n"
                                 "\tvmaddx.xyzw $vf2, $vf3, $vf22x\n"
                                 "\tvmulay.xyzw $ACC, $vf3, $vf22y\n"
                                 "\tvmsubx.xyzw $vf3, $vf23, $vf22x"
                                 : : "r"(fvw));
        }
        __asm__ __volatile__(TAIL3X_MULROW("$vf2") TAIL3X_MULROW("$vf3") TAIL3X_MULROW("$vf4")
                             TAIL3X_MULROW("$vf5"));
        shi.packet = ppvCurPrimp;
        TAIL3X_SAVE(pw);
        ppvCurPrimp = op_shape_draw(sh, &shi, shp->nPacketSize);
        TAIL3X_LOAD(pw);
    next:
        i--;
        if (i == 0) {
            goto end;
        }
        TAIL_SUB(col, col, colSft);
        d -= e;
        scl -= sclSft;
        if (d <= 0.0f) {
            goto end;
        }
        for (;;) {
            if (d <= c) {
                fvw[0] = b / f;
                __asm__ __volatile__("lqc2 $vf31, 0x0(%0)\n"
                                     "\tvmulx.xyzw $vf29, $vf27, $vf31x\n"
                                     "\tvadd.xyzw $vf29, $vf28, $vf29"
                                     : : "r"(fvw));
                b += d;
                c -= d;
                break;
            }
            /* MATCH: the unsigned sizeof compare keeps fold-const.c from turning j++ == 27 into ++j == 28 */
            if (j++ == sizeof(pThis->Stk) / sizeof(pThis->Stk[0]) - 1) {
                j = 0;
            }
            if (j == k) {
                goto end;
            }
            __asm__ __volatile__("vmove.xyzw $vf28, $vf26\n"
                                 "\tlqc2 $vf26, 0x0(%0)\n"
                                 "\tvsub.xyzw $vf27, $vf26, $vf28"
                                 : : "r"(pFv0[j]));
            b -= f;
            __asm__ __volatile__("vmul.xyz $vf31, $vf27, $vf27\n"
                                 "\tvaddy.x $vf30, $vf31, $vf31y\n"
                                 "\tvaddz.x $vf31, $vf30, $vf31z\n"
                                 "\tvsqrt $Q, $vf31x\n"
                                 "\tvwaitq\n"
                                 "\tvaddq.x $vf31, $vf0, $Q\n"
                                 "\tsqc2 $vf31, 0x0(%0)"
                                 : : "r"(fvw));
            f = fvw[0];
            c += f;
        }
    }
end:
    if (!ppvUserStopPartF && !(ppvMng->bFlagStop || ppvMng->bBoolSlow)) {
        pThis->Cnt += dat->AnmSpd;
    }
    op_spr_free(0x180);
}

void pppKeShpTail3XCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeShpTail3X *pThis = (VKeShpTail3X *)&pobj->val[ctbl->useVal[IKeShpTail3X_This]];

    pThis->Cur = 0;
    pThis->Seq = 0;
    pThis->Cnt = 0;
    pThis->Seed = rand();
    *(u_long *)&pThis->Col0 = 0;
    *(u_long *)&pThis->Col1 = 0;
    *(u_long *)&pThis->Col0Mov = 0;
    *(u_long *)&pThis->Col0Acc = 0;
    *(u_long *)&pThis->Col1Mov = 0;
    *(u_long *)&pThis->Col1Acc = 0;
}

void pppKeShpTail3XCon2(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeShpTail3X *pThis = (VKeShpTail3X *)&pobj->val[ctbl->useVal[IKeShpTail3X_This]];

    *(u_long *)&pThis->Col0 = 0;
    *(u_long *)&pThis->Col1 = 0;
    *(u_long *)&pThis->Col0Mov = 0;
    *(u_long *)&pThis->Col0Acc = 0;
    *(u_long *)&pThis->Col1Mov = 0;
    *(u_long *)&pThis->Col1Acc = 0;
}
