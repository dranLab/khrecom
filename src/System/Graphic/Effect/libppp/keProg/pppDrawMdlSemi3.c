#include "pppDrawMdlSemi3.h"
#include "System/Graphic/Effect/libppp/eiProg/pppColor.h"
#include "System/Graphic/Effect/libppp/draw/md.h"

void pppDrawMdlSemi3(pppPObject *pobj, PDrawMdlSemi3 *p, pppCtrlTable *ctbl) {
    VColor *pv;
    OMD_IN mdi;
    sceVu0FMATRIX lvwMat;
    pppModelSt *mdl;
    OMD_H *omd;

    if (p->model == 0xFFFF) {
        return;
    }
    pv = (VColor *)&pobj->val[ctbl->useVal[IDrawMdlSemi3_Color]];
    mdi.a = pv->release_col.a;
    if (mdi.a == 0) {
        return;
    }
    mdl = &ppvEnv->lpModel[p->model];
    omd = mdl->omd;
    mdi.flag = 0;
    if (p->draw_mode & 2) {
        mdi.flag = 4;
        if (!(omd->flag & 1)) {
            mdi.flag |= 8;
        }
    } else {
        if (p->draw_mode & 1) {
            mdi.flag = 0x80;
        }
        if (!(omd->flag & 1)) {
            mdi.flag |= 8;
        }
    }
    if (!(omd->flag & 2)) {
        mdi.flag |= 0x10;
    }
    if (p->fog) {
        mdi.flag |= 0x200;
    }
    if (p->near) {
        mdi.flag |= 0x100;
    }
    if (p->scissor) {
        mdi.flag |= 0x2000;
        mdi.lviewmat = &lvwMat;
        sceVu0MulMatrix(lvwMat, ppvWorldMatrix, ppvPObj->cmat);
    }
    if (p->zCrct) {
        mdi.pzmat = (sceVu0FMATRIX *)&pobj->val[ctbl->useVal[IDrawMdlSemi3_KeZCrct]];
    } else {
        mdi.pzmat = 0;
    }
    *(u_short *)&mdi.alpha = *(u_short *)&p->acalc;
    mdi.pri = 0;
    *(int *)&mdi.ar = 0;
    mdi.va = ppvPObj->lpvert;
    mdi.na = ppvPObj->lpnorm;
    *(int *)&mdi.toff = 0;
    *(u_long *)&mdi.u = 0;
    *(int *)&mdi.ot = 0;
    mdi.tmp_a = (char *)ppvVNWork;
    mdi.r = pv->release_col.r;
    mdi.g = pv->release_col.g;
    mdi.b = pv->release_col.b;
    mdi.pmat = &ppvPObj->dmat;
    mdi.lcmat = &ppvColorMatrix;
    mdi.lvmat = &ppvLightMatrix;
    mdi.lwmat = &ppvPObj->cmat;
    mdi.packet = ppvCurPrimp;
    ppvCurPrimp = op1_md_draw(omd, &mdi, mdl->nPacketSize);
}
