#include "pppKeTkFade.h"
#include "Ke.h"

typedef union _tWrkCol {
    u_int i;
    u_char c[4];
} _tWrkCol;

typedef struct _tWrkBuf {
    _tWrkCol Col[256];
    signed char Sft[32];
} _tWrkBuf;

void pppKeTkFadeDraw(pppPObject *pobj, PKeTkFade *dat, pppCtrlTable *ctbl) {
    sceVu0FVECTOR fv0;
    sceVu0FVECTOR fv1;
    sceGsLoadImage gli;
    VKeTkFade *pThis;
    pppModelSt *pMSt;
    u_int *pClt;
    _tWrkCol w;
    _tWrkCol w2;
    _tWrkBuf *wBuf;
    float f;
    int i;
    int j;
    u_long tex0;

    pThis = (VKeTkFade *)&pobj->val[ctbl->useVal[IKeTkFade_This]];
    if (!ppvUserStopPartF && !ppvMng->bFlagStop && !ppvMng->bBoolSlow) {
        pThis->LvSpd += pThis->LvAcc;
        pThis->Lv += pThis->LvSpd;
        if (pobj->time == dat->cdt.time) {
            pThis->Lv += dat->Lv;
            pThis->LvSpd += dat->LvSpd;
            pThis->LvAcc += dat->LvAcc;
        }
    }
    if (dat->Mdl == 0xFFFF) {
        return;
    }
    wBuf = (_tWrkBuf *)op_spr_alloc(sizeof(_tWrkBuf));
    ((u_long128 *)wBuf->Sft)[0] = ((u_long128 *)KeCltSft)[0];
    ((u_long128 *)wBuf->Sft)[1] = ((u_long128 *)KeCltSft)[1];
    pMSt = &ppvEnv->lpModel[dat->Mdl];
    i = pThis->Lv;
    w2.i = *(u_int *)&dat->ColSs;
    w.i = *(u_int *)&dat->ColBs;
    tex0 = ((u_long)pMSt->tex0h << 32) | pMSt->tex0l;
    if (i < 0) {
        if (i < -256) {
            i = -256;
        }
        i += 256;
        pClt = &wBuf->Col[(i & 0xE0) + KeCltOfs[i & 0x1F]].i;
        for (j = i; j < 256; j++) {
            *pClt = w.i;
            pClt += wBuf->Sft[j & 0x1F];
        }
        j = 0;
        f = i;
        pClt = &wBuf->Col[0].i;
    } else {
        if (i >= 256) {
            i = 255;
        }
        pClt = &wBuf->Col[0].i;
        for (j = 0; j < i; j++) {
            *pClt = w2.i;
            pClt += wBuf->Sft[j & 0x1F];
        }
        f = 256.0f - i;
        i = 256;
    }
    fv1[0] = w2.c[0];
    fv1[1] = w2.c[1];
    fv1[2] = w2.c[2];
    fv1[3] = w2.c[3];
    fv0[0] = w.c[0];
    fv0[1] = w.c[1];
    fv0[2] = w.c[2];
    fv0[3] = w.c[3];
    sceVu0SubVector(fv0, fv0, fv1);
    sceVu0DivVector(fv0, fv0, f);
    for (; j < i; j++) {
        *pClt = ((u_int)fv1[3] << 24) | ((u_int)fv1[2] << 16) | ((u_int)fv1[1] << 8) | (u_int)fv1[0];
        sceVu0AddVector(fv1, fv1, fv0);
        pClt += wBuf->Sft[j & 0x1F];
    }
    sceGsSetDefLoadImage(&gli, (tex0 & 0x7FFE000000000) >> 37, 4, (tex0 & 0x78000000000000) >> 51, 0, 0, 16, 16);
    FlushCache(0);
    sceGsExecLoadImage(&gli, (u_long128 *)wBuf);
    sceGsSyncPath(0, 0);
    op_spr_free(sizeof(_tWrkBuf));
}

void pppKeTkFadeCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeTkFade *pThis = (VKeTkFade *)&pobj->val[ctbl->useVal[IKeTkFade_This]];

    pThis->Lv = 0.0f;
    pThis->LvSpd = 0.0f;
    pThis->LvAcc = 0.0f;
}
