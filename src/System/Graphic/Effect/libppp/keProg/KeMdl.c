#include "KeMdl.h"
#include "Ke.h"
#include "System/Graphic/Effect/libppp/draw/md.h"

void KeMdl_MkTkClut(u_int colBs, u_int colSs, float lv, u_int *pCltBuf) {
    sceVu0FVECTOR fv0;
    sceVu0FVECTOR fv1;
    u_int *pClt;
    signed char *sft;
    float f;
    int i;
    int j;

    sft = op_spr_alloc(0x20);
    i = lv;
    ((u_long128 *)sft)[0] = ((u_long128 *)KeCltSft)[0];
    ((u_long128 *)sft)[1] = ((u_long128 *)KeCltSft)[1];
    if (i < 0) {
        if (i < -256) {
            i = -256;
        }
        i += 256;
        pClt = pCltBuf + (i & 0xE0) + KeCltOfs[i & 0x1F];
        for (j = i; j < 256; j++) {
            *pClt = colBs;
            pClt += sft[j & 0x1F];
        }
        j = 0;
        f = i;
        pClt = pCltBuf;
    } else {
        if (i >= 256) {
            i = 255;
        }
        pClt = pCltBuf;
        for (j = 0; j < i; j++) {
            *pClt = colSs;
            pClt += sft[j & 0x1F];
        }
        f = 256.0f - i;
        i = 256;
    }
    fv1[0] = ((u_char *)&colSs)[0];
    fv1[1] = ((u_char *)&colSs)[1];
    fv1[2] = ((u_char *)&colSs)[2];
    fv1[3] = ((u_char *)&colSs)[3];
    fv0[0] = ((u_char *)&colBs)[0];
    fv0[1] = ((u_char *)&colBs)[1];
    fv0[2] = ((u_char *)&colBs)[2];
    fv0[3] = ((u_char *)&colBs)[3];
    sceVu0SubVector(fv0, fv0, fv1);
    sceVu0DivVector(fv0, fv0, f);
    for (; j < i; j++) {
        *pClt = ((u_int)fv1[3] << 24) | ((u_int)fv1[2] << 16) | ((u_int)fv1[1] << 8) | (u_int)fv1[0];
        sceVu0AddVector(fv1, fv1, fv0);
        pClt += sft[j & 0x1F];
    }
    op_spr_free(0x20);
}


/* Logic matches; the original threads the flg & 2 path (mdi.flag = 4, then 4 | 8 folded to 0xC) and keeps
 * omd in s1 / pCol in s2. Tried else/ternary/negated forms of the flag selection. */
void KeMdl_Draw(int mdlIdx, u_short *pCol, int cOfs, int u, int v, u_char flg, sceVu0FMATRIX *zMat) {
    sceVu0FMATRIX lvwMat;
    pppModelSt *pMdl;
    OMD_H *omd;
    OMD_IN mdi;

    if (mdlIdx == 0xFFFF) {
        return;
    }
    pMdl = &ppvEnv->lpModel[mdlIdx];
    omd = pMdl->omd;
    mdi.flag = 0;
    if (flg & 2) {
        mdi.flag = 4;
        if (!(omd->flag & 1)) {
            mdi.flag |= 8;
        }
    } else {
        if (flg & 1) {
            mdi.flag = 0x80;
        }
        if (!(omd->flag & 1)) {
            mdi.flag |= 8;
        }
    }
    if (!(omd->flag & 2)) {
        mdi.flag |= 0x10;
    }
    if (flg & 4) {
        mdi.flag |= 0x200;
    }
    if (flg & 8) {
        mdi.flag |= 0x100;
    }
    if (flg & 0x10) {
        mdi.flag |= 0x2000;
        mdi.lviewmat = &lvwMat;
        sceVu0MulMatrix(lvwMat, ppvWorldMatrix, ppvPObj->cmat);
    }
    if (flg & 0x20) {
        mdi.pzmat = zMat;
    } else {
        mdi.pzmat = 0;
    }
    *(u_short *)&mdi.alpha = *(u_short *)&omd->alpha;
    *(int *)&mdi.ar = 0;
    mdi.va = ppvPObj->lpvert;
    mdi.na = ppvPObj->lpnorm;
    mdi.tmp_a = (char *)ppvVNWork;
    mdi.coff = cOfs;
    mdi.u = u << 16;
    mdi.v = v << 16;
    mdi.r = pCol[0] >> 7;
    mdi.g = pCol[1] >> 7;
    mdi.b = pCol[2] >> 7;
    mdi.a = pCol[3] >> 7;
    mdi.pmat = &ppvPObj->dmat;
    mdi.lcmat = &ppvColorMatrix;
    mdi.lvmat = &ppvLightMatrix;
    mdi.lwmat = &ppvPObj->cmat;
    mdi.packet = ppvCurPrimp;
    mdi.toff = 0;
    *(int *)&mdi.ot = 0;
    mdi.pri = 0;
    ppvCurPrimp = op1_md_draw(omd, &mdi, pMdl->nPacketSize);
}
