#include "pppKeLns.h"

#define VAL(type, i) ((type *)&pobj->val[ctbl->useVal[i]])

void pppKeLnsLp(pppPObject *pobj, PKeLnsLp *dat, pppCtrlTable *ctbl) {
    KeLnsLp *_p = VAL(KeLnsLp, 0);

    if (dat->cdt.time == pobj->time) {
        _p->Pow += dat->Pow;
        _p->R = dat->R;
        _p->LVec[3] += dat->LWid;
        _p->Md = dat->Md;
    }
}

void pppKeLnsLpDraw(pppPObject *pobj, PKeLnsLp *dat, pppCtrlTable *ctbl) {
    sceVu0FVECTOR fv0;
    KeWkLnsLp_Update w;
    KeLnsLp *_p = VAL(KeLnsLp, 0);

    if (_p->Md) {
        *(u_long128 *)fv0 = 0;
        fv0[3] = fv0[2] = 1.0f;
        sceVu0ApplyMatrix(fv0, ppvPObj->cmat, fv0);
        KeVu0CopyVecXYZ(_p->LVec, fv0);
    }
    *(_p->LcPos + 0) = ppvPObj->cmat[3][0];
    *(_p->LcPos + 1) = ppvPObj->cmat[3][1];
    *(_p->LcPos + 2) = ppvPObj->cmat[3][2];
    w.RPos = &ppvPObj->cmat[3];
    w.WvMat = &ppvWorldMatrix;
    w.VsMat = &ppvScreenMatrix;
    w.This = _p;
    KeLnsLp_Update(&w);
}

void pppKeLnsLpCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    KeLnsLp *pThis = VAL(KeLnsLp, 0);

    KeLnsLp_Init(pThis);
    pThis->LVec[3] = 0.0f;
    pThis->Pow = 0.0f;
}

void pppKeLnsLpCon2(pppPObject *pobj, pppCtrlTable *ctbl) {
    KeLnsLp *pThis = VAL(KeLnsLp, 0);

    pThis->LVec[3] = 0.0f;
    pThis->Pow = 0.0f;
}

void pppKeLnsArndUpdate(pppPObject *pobj, PKeLnsArnd *dat, pppCtrlTable *ctbl) {
    KeLnsArnd *_p = VAL(KeLnsArnd, 0);
    void *wp;

    if (dat->cdt.time == pobj->time) {
        _p->Lp = VAL(KeLnsLp, 1);
        if (dat->Shp == 0xFFFF) {
            _p->Shp = NULL;
            return;
        }
        wp = &ppvEnv->lpShape[dat->Shp];
        if (wp != _p->Shp) {
            _p->Shp = wp;
            _p->Flr.Cnt = ((OANM_HEADER *)_p->Shp->shape)->seq[0].wait;
            _p->Flr.SeqIdx = 0;
        }
    }
}

void pppKeLnsArnd(pppPObject *pobj, PKeLnsArnd *dat, pppCtrlTable *ctbl) {
    pppKeLnsArndUpdate(pobj, dat, ctbl);
}

void pppKeLnsArndDraw(pppPObject *pobj, PKeLnsArnd *dat, pppCtrlTable *ctbl) {
    KeWkLnsArnd_Draw w;

    w.Col[0] = dat->Col.r * ppvMng->parCol[0];
    w.Col[1] = dat->Col.g * ppvMng->parCol[1];
    w.Col[2] = dat->Col.b * ppvMng->parCol[2];
    w.Col[3] = dat->Col.a * ppvMng->parCol[3];
    w.Scl = dat->Scl;
    w.VsMat = &ppvScreenMatrix;
    w.SclVec = &ppvParScl;
    w.This = VAL(KeLnsArnd, 0);
    KeLnsArnd_Draw(&w, 0);
}

void pppKeLnsArndCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    KeLnsArnd_Init(VAL(KeLnsArnd, 0));
}

void pppKeLnsClmUpdate(pppPObject *pobj, PKeLnsClm *dat, pppCtrlTable *ctbl) {
    KeLnsClm *_p = VAL(KeLnsClm, 0);
    void *wp;

    if (dat->cdt.time == pobj->time) {
        _p->Lp = VAL(KeLnsLp, 1);
        if (dat->Shp == 0xFFFF) {
            _p->Shp = NULL;
        } else {
            wp = &ppvEnv->lpShape[dat->Shp];
            if (wp != _p->Shp) {
                KeLnsClm_SetShp(_p, wp, dat->Seed);
            }
        }
        _p->Betw = dat->Betw;
        _p->StOfs = dat->StOfs;
        _p->FlgType = dat->FlgType;
    }
}

void pppKeLnsClm(pppPObject *pobj, PKeLnsClm *dat, pppCtrlTable *ctbl) {
    pppKeLnsClmUpdate(pobj, dat, ctbl);
}

void pppKeLnsClmDraw(pppPObject *pobj, PKeLnsClm *dat, pppCtrlTable *ctbl) {
    KeWkLnsClm_Draw w;

    w.R = dat->R;
    w.Col[0] = dat->Col.r * ppvMng->parCol[0];
    w.Col[1] = dat->Col.g * ppvMng->parCol[1];
    w.Col[2] = dat->Col.b * ppvMng->parCol[2];
    w.Col[3] = dat->Col.a * ppvMng->parCol[3];
    w.Scl = dat->Scl;
    w.VsMat = &ppvScreenMatrix;
    w.This = VAL(KeLnsClm, 0);
    w.SclVec = &ppvParScl;
    KeLnsClm_Draw(&w, 0);
}

void pppKeLnsClmCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    KeLnsClm *_p = VAL(KeLnsClm, 0);

    KeLnsClm_Init(_p);
    _p->Num = 4;
}

void pppKeLnsCrnUpdate(pppPObject *pobj, PKeLnsCrn *dat, pppCtrlTable *ctbl) {
    KeLnsCrn *_p = VAL(KeLnsCrn, 0);
    void *wp;

    if (dat->cdt.time == pobj->time) {
        _p->Lp = VAL(KeLnsLp, 1);
        if (dat->Shp == 0xFFFF) {
            _p->Shp = NULL;
        } else {
            wp = &ppvEnv->lpShape[dat->Shp];
            if (wp != _p->Shp) {
                KeLnsCrn_SetShp(_p, wp);
            }
        }
    }
}

void pppKeLnsCrn(pppPObject *pobj, PKeLnsCrn *dat, pppCtrlTable *ctbl) {
    pppKeLnsCrnUpdate(pobj, dat, ctbl);
}

void pppKeLnsCrnDraw(pppPObject *pobj, PKeLnsCrn *dat, pppCtrlTable *ctbl) {
    KeWkLnsCrn_Draw w;

    w.Col[0] = dat->Col.r * ppvMng->parCol[0];
    w.Col[1] = dat->Col.g * ppvMng->parCol[1];
    w.Col[2] = dat->Col.b * ppvMng->parCol[2];
    w.Col[3] = dat->Col.a * ppvMng->parCol[3];
    w.PLen = dat->PLen;
    w.DLen = dat->DLen;
    w.Scl = dat->Scl;
    w.VsMat = &ppvScreenMatrix;
    w.This = VAL(KeLnsCrn, 0);
    w.SclVec = &ppvParScl;
    KeLnsCrn_Draw(&w, 0);
}

void pppKeLnsCrnCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    KeLnsCrn_Init(VAL(KeLnsCrn, 0));
}

void pppKeLnsFlsUpdate(pppPObject *pobj, PKeLnsFls *dat, pppCtrlTable *ctbl) {
    KeLnsFls *p = VAL(KeLnsFls, 0);
    void *wp;

    if (dat->cdt.time == pobj->time) {
        p->Lp = VAL(KeLnsLp, 1);
        if (dat->Shp == 0xFFFF) {
            p->Shp = NULL;
            return;
        }
        wp = &ppvEnv->lpShape[dat->Shp];
        if (wp != p->Shp) {
            p->Shp = wp;
            p->Flr.Cnt = ((OANM_HEADER *)p->Shp->shape)->seq[0].wait;
            p->Flr.SeqIdx = 0;
        }
    }
}

void pppKeLnsFls(pppPObject *pobj, PKeLnsFls *dat, pppCtrlTable *ctbl) {
    pppKeLnsFlsUpdate(pobj, dat, ctbl);
}

void pppKeLnsFlsDraw(pppPObject *pobj, PKeLnsFls *dat, pppCtrlTable *ctbl) {
    KeWkLnsFls_Draw w;

    w.Col[0] = dat->Col.r * ppvMng->parCol[0];
    w.Col[1] = dat->Col.g * ppvMng->parCol[1];
    w.Col[2] = dat->Col.b * ppvMng->parCol[2];
    w.Col[3] = dat->Col.a * ppvMng->parCol[3];
    w.Scl = dat->Scl;
    w.PLen = dat->PLen;
    *(u_short *)w.Typ = *(u_short *)&dat->Typ0;
    w.VsMat = &ppvScreenMatrix;
    w.This = VAL(KeLnsFls, 0);
    w.SclVec = &ppvParScl;
    KeLnsFls_Draw(&w, 0);
}

void pppKeLnsFlsCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    KeLnsFls_Init(VAL(KeLnsFls, 0));
}

struct tGifPk {
    u_long sceGifTag[2];
    u_long BITBLTBUF[2];
    u_long TRXPOS[2];
    u_long TRXREG[2];
    u_long TRXDIR[2];
    u_long GifTag2[2];
};

typedef struct tWk {
    struct tGifPk Pk;
    u_long128 Buf[768];
} tWk;

/* Original line numbers (from .mdebug): the line notes affect scheduling here. */
#line 431
/* Original line numbers (from .mdebug): the line notes affect scheduling here. */
#line 431
/* Original line numbers (from .mdebug): the line notes affect scheduling here. */
#line 431
/* Original line numbers (from .mdebug): the line notes affect scheduling here. */
#line 431
/* Original line numbers (from .mdebug): the line notes affect scheduling here. */
#line 431
/* Original line numbers (from .mdebug): the line notes affect scheduling here. */
#line 431
/* Original line numbers (from .mdebug): the line notes affect scheduling here. */
#line 431
/* Original line numbers (from .mdebug): the line notes affect scheduling here. */
#line 431
/* Original line numbers (from .mdebug): the line notes affect scheduling here. */
#line 431
/* Original line numbers (from .mdebug): the line notes affect scheduling here. */
#line 431
static void pppKeLnsLpTOtFunc(VKeLnsLpT *p, int Idx, int Time) {
    tWk *pw;
    sceVu0IVECTOR iv0;
    sceVu0IVECTOR smpl;
    sceVu0IVECTOR scrn;
    sceGsStoreImage si;
    int ox;
    int oy;
    int smplWd;
    int smplHt;
    int storeWd;
    int storeHt;
    int i;
    int j;
    int k;
    int sW;
    int sH;
    u_char *cp;
    float f;
    static sceGsDrawEnv1 env;
    sceGsDrawEnv1 *e = &env; /* not in the debug info, but keeps &env in a register as the original */

















    *(u_long128 *)&e->frame1 = ppvGsFrame;
    e->frame1addr = 0x4C;
    *(u_long128 *)&e->zbuf1 = ppvGsZbuf;
    e->zbuf1addr = 0x4E;
    *(u_long128 *)&e->xyoffset1 = ppvGsXyoffset;
    e->xyoffset1addr = 0x18;
    *(u_long128 *)&e->scissor1 = ppvGsScissor;
    e->scissor1addr = 0x40;
    *(u_long128 *)&e->prmodecont = 0;
    e->prmodecontaddr = 0x1A;
    *(u_long128 *)&e->colclamp = ppvGsColorClamp;
    e->colclampaddr = 0x46;
    *(u_long128 *)&e->dthe = ppvGsDthe;
    e->dtheaddr = 0x45;
    *(u_long128 *)&e->test1 = ppvGsTest;
    e->test1addr = 0x47;











    if (p->Lp.VPow == 0.0f) return;


    i = p->Lp.Mbk + 1;
    if (p->Lp.Idx % i != Time % i) {

        p->Lp.VPow *= p->Lp.PreHidePow;
        return;
    }


















    ox = e->xyoffset1.OFX;
    oy = e->xyoffset1.OFY;
    scrn[0] = e->scissor1.SCAX0;
    scrn[1] = e->scissor1.SCAY0;
    scrn[2] = e->scissor1.SCAX1;
    scrn[3] = e->scissor1.SCAY1;
    scrn[0] = ox + (scrn[0] << 4);
    scrn[1] = oy + (scrn[1] << 4);
    scrn[2] = ox + ((scrn[2] + 1) << 4);
    scrn[3] = oy + ((scrn[3] + 1) << 4);





    sceVu0FTOI4Vector(iv0, p->ScrnPos);




    sW = p->SmplW;
    sH = p->SmplH;

    smpl[0] = iv0[0] - sW * 8;
    smpl[1] = iv0[1] - sH * 8;
    smpl[2] = iv0[0] + sW * 8;
    smpl[3] = iv0[1] + sH * 8;


    if (smpl[0] < scrn[0]) smpl[0] = scrn[0];
    else if (smpl[0] > scrn[2]) smpl[0] = scrn[2];
    if (smpl[1] < scrn[1]) smpl[1] = scrn[1];
    else if (smpl[1] > scrn[3]) smpl[1] = scrn[3];
    if (smpl[2] < scrn[0]) smpl[2] = scrn[0];
    else if (smpl[2] > scrn[2]) smpl[2] = scrn[2];
    if (smpl[3] < scrn[1]) smpl[3] = scrn[1];
    else if (smpl[3] > scrn[3]) smpl[3] = scrn[3];


    smplWd = (smpl[2] - smpl[0] + 15) >> 4;
    smplHt = (smpl[3] - smpl[1] + 15) >> 4;

    if (smplWd <= 0 || smplHt <= 0) {


        p->Lp.VPow = 0.0f;
        p->Lp.PreHidePow = 0.0f;
        return;
    }


    ox = (smpl[0] - ox) >> 4;
    oy = (smpl[1] - oy) >> 4;
    storeWd = (smplWd + 7) & ~7;
    storeHt = (smplHt + 3) & ~3;




    pw = op_spr_alloc(sizeof(tWk));





































    sceGsSetDefStoreImage(&si, e->zbuf1.ZBP << 5, e->frame1.FBW, 0x31, ox, oy, storeWd, storeHt);

    FlushCache(0);
    sceGsExecStoreImage(&si, pw->Buf);
    sceGsSyncPath(0, 0);



    f = 0.0f;
    k = iv0[2] >> 4;
    cp = (u_char *)pw->Buf;
    for (i = 0; i < smplHt; i++, cp += (storeWd - smplWd) * 3) {

        for (j = 0; j < smplWd; j++, cp += 3) {

            if ((u_int)(cp[0] | (cp[1] << 8) | (cp[2] << 16)) < (u_int)k) f += 1.0f;

        }
    }





    op_spr_free(sizeof(tWk));
    f /= sW * sH;


    p->Lp.VPow *= f;
    p->Lp.PreHidePow = f;


}

void pppKeLnsLpT(pppPObject *pobj, PKeLnsLpT *dat, pppCtrlTable *ctbl) {
    VKeLnsLpT *p;

    pppKeLnsLp(pobj, (PKeLnsLp *)dat, ctbl);
    p = VAL(VKeLnsLpT, 0);
    if (dat->cdt.time == pobj->time) {
        p->SmplW = dat->SmplW * 8;
        p->SmplH = dat->SmplH * 4;
        p->Lp.Mbk = dat->Mbk;
    }
}

void pppKeLnsLpTDraw(pppPObject *pobj, PKeLnsLpT *dat, pppCtrlTable *ctbl) {
    VKeLnsLpT *p;
    sceVu0FVECTOR fv0;

    pppKeLnsLpDraw(pobj, (PKeLnsLp *)dat, ctbl);
    p = VAL(VKeLnsLpT, 0);
    fv0[0] = 0.0f;
    fv0[1] = 0.0f;
    fv0[2] = 0.0f;
    fv0[3] = 1.0f;
    sceVu0ApplyMatrix(fv0, ppvPObj->dmat, fv0);
    sceVu0ScaleVector(p->ScrnPos, fv0, 1.0f / fv0[3]);
    pppKeLnsLpTOtFunc(p, 0, 0);
}

void pppKeLnsLpTCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    static u_char KeLns_LpTCnt = 0;
    VKeLnsLpT *p = VAL(VKeLnsLpT, 0);

    KeLnsLp_Init(&p->Lp);
    p->Lp.LVec[3] = 0.0f;
    p->Lp.Pow = 0.0f;
    p->Lp.Idx = KeLns_LpTCnt;
    p->Lp.Mbk = 2;
    p->SmplW = 0;
    p->SmplH = 0;
    KeLns_LpTCnt++;
}

void pppKeLnsLpTCon2(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeLnsLpT *p = VAL(VKeLnsLpT, 0);

    p->Lp.LVec[3] = 0.0f;
    p->Lp.Pow = 0.0f;
}

void pppKeLnsLpTDes(pppPObject *pobj, pppCtrlTable *ctbl) {
}

static void pppKeLnsArndTOtFunc(VKeLnsArndT *p, int Idx, int Time) {
    KeWkLnsArnd_Draw w;

    w.This = &p->Arnd;
    *(u_int *)w.Col = *(u_int *)p->Col;
    w.Scl = p->Scl;
    w.VsMat = &ppvScreenMatrix;
    w.SclVec = &p->SclVec;
    KeLnsArnd_Draw(&w, 0);
}

void pppKeLnsArndT(pppPObject *pobj, PKeLnsArndT *dat, pppCtrlTable *ctbl) {
    VKeLnsArndT *p = VAL(VKeLnsArndT, 0);

    pppKeLnsArndUpdate(pobj, (PKeLnsArnd *)dat, ctbl);
    if (dat->cdt.time == pobj->time) {
        p->Scl = dat->Scl;
    }
}

void pppKeLnsArndTDraw(pppPObject *pobj, PKeLnsArndT *dat, pppCtrlTable *ctbl) {
    VKeLnsArndT *p = VAL(VKeLnsArndT, 0);

    p->Col[0] = dat->Col.r * ppvMng->parCol[0];
    p->Col[1] = dat->Col.g * ppvMng->parCol[1];
    p->Col[2] = dat->Col.b * ppvMng->parCol[2];
    p->Col[3] = dat->Col.a * ppvMng->parCol[3];
    sceVu0CopyVector(p->SclVec, ppvParScl);
    pppKeLnsArndTOtFunc(p, 0, 0);
}

void pppKeLnsArndTCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    KeLnsArnd_Init(VAL(KeLnsArnd, 0));
}

void pppKeLnsArndTDes(pppPObject *pobj, pppCtrlTable *ctbl) {
}

static void pppKeLnsClmTOtFunc(VKeLnsClmT *p, int Idx, int Time) {
    KeWkLnsClm_Draw w;

    w.This = (KeLnsClm *)&p->Hd;
    w.R = p->R;
    *(u_int *)w.Col = *(u_int *)p->Col;
    w.Scl = p->Scl;
    w.VsMat = &ppvScreenMatrix;
    w.SclVec = &p->SclVec;
    KeLnsClm_Draw(&w, 0);
}

void pppKeLnsClmT(pppPObject *pobj, PKeLnsClmT *dat, pppCtrlTable *ctbl) {
    VKeLnsClmT *p = VAL(VKeLnsClmT, 0);

    pppKeLnsClmUpdate(pobj, (PKeLnsClm *)dat, ctbl);
    if (dat->cdt.time == pobj->time) {
        p->Scl = dat->Scl;
        p->R = dat->R;
    }
}

void pppKeLnsClmTDraw(pppPObject *pobj, PKeLnsClmT *dat, pppCtrlTable *ctbl) {
    VKeLnsClmT *p = VAL(VKeLnsClmT, 0);

    p->Col[0] = dat->Col.r * ppvMng->parCol[0];
    p->Col[1] = dat->Col.g * ppvMng->parCol[1];
    p->Col[2] = dat->Col.b * ppvMng->parCol[2];
    p->Col[3] = dat->Col.a * ppvMng->parCol[3];
    sceVu0CopyVector(p->SclVec, ppvParScl);
    pppKeLnsClmTOtFunc(p, 0, 0);
}

void pppKeLnsClmTCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeLnsClmT *p = VAL(VKeLnsClmT, 0);

    KeLnsClm_Init((KeLnsClm *)&p->Hd);
    p->Hd.Num = 4;
}

void pppKeLnsClmTDes(pppPObject *pobj, pppCtrlTable *ctbl) {
}

static void pppKeLnsCrnTOtFunc(VKeLnsCrnT *p, int Idx, int Time) {
    KeWkLnsCrn_Draw w;

    w.This = &p->Crn;
    w.PLen = p->PLen;
    w.DLen = p->DLen;
    *(u_int *)w.Col = *(u_int *)p->Col;
    w.Scl = p->Scl;
    w.VsMat = &ppvScreenMatrix;
    w.SclVec = &p->SclVec;
    KeLnsCrn_Draw(&w, 0);
}

void pppKeLnsCrnT(pppPObject *pobj, PKeLnsCrnT *dat, pppCtrlTable *ctbl) {
    VKeLnsCrnT *p = VAL(VKeLnsCrnT, 0);

    pppKeLnsCrnUpdate(pobj, (PKeLnsCrn *)dat, ctbl);
    if (dat->cdt.time == pobj->time) {
        p->Scl = dat->Scl;
        p->PLen = dat->PLen;
        p->DLen = dat->DLen;
    }
}

void pppKeLnsCrnTDraw(pppPObject *pobj, PKeLnsCrnT *dat, pppCtrlTable *ctbl) {
    VKeLnsCrnT *p = VAL(VKeLnsCrnT, 0);

    p->Col[0] = dat->Col.r * ppvMng->parCol[0];
    p->Col[1] = dat->Col.g * ppvMng->parCol[1];
    p->Col[2] = dat->Col.b * ppvMng->parCol[2];
    p->Col[3] = dat->Col.a * ppvMng->parCol[3];
    sceVu0CopyVector(p->SclVec, ppvParScl);
    pppKeLnsCrnTOtFunc(p, 0, 0);
}

void pppKeLnsCrnTCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    KeLnsCrn_Init(VAL(KeLnsCrn, 0));
}

void pppKeLnsCrnTDes(pppPObject *pobj, pppCtrlTable *ctbl) {
}

static void pppKeLnsFlsTOtFunc(VKeLnsFlsT *p, int Idx, int Time) {
    KeWkLnsFls_Draw w;

    w.This = &p->Fls;
    *(u_int *)w.Col = *(u_int *)p->Col;
    w.Scl = p->Scl;
    w.PLen = p->PLen;
    *(u_short *)w.Typ = p->Typ;
    w.VsMat = &ppvScreenMatrix;
    w.SclVec = &p->SclVec;
    KeLnsFls_Draw(&w, 0);
}

void pppKeLnsFlsT(pppPObject *pobj, PKeLnsFlsT *dat, pppCtrlTable *ctbl) {
    VKeLnsFlsT *p = VAL(VKeLnsFlsT, 0);

    pppKeLnsFlsUpdate(pobj, (PKeLnsFls *)dat, ctbl);
    if (dat->cdt.time == pobj->time) {
        p->Scl = dat->Scl;
        p->PLen = dat->PLen;
        p->Typ = *(u_short *)&dat->Typ0;
    }
}

void pppKeLnsFlsTDraw(pppPObject *pobj, PKeLnsFlsT *dat, pppCtrlTable *ctbl) {
    VKeLnsFlsT *p = VAL(VKeLnsFlsT, 0);

    p->Col[0] = dat->Col.r * ppvMng->parCol[0];
    p->Col[1] = dat->Col.g * ppvMng->parCol[1];
    p->Col[2] = dat->Col.b * ppvMng->parCol[2];
    p->Col[3] = dat->Col.a * ppvMng->parCol[3];
    sceVu0CopyVector(p->SclVec, ppvParScl);
    pppKeLnsFlsTOtFunc(p, 0, 0);
}

void pppKeLnsFlsTCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    KeLnsFls_Init(VAL(KeLnsFls, 0));
}

void pppKeLnsFlsTDes(pppPObject *pobj, pppCtrlTable *ctbl) {
}
// SOURCE DATA: .data .bss
