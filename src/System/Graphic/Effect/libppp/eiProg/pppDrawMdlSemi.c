#include "pppDrawMdlSemi.h"
#include "pppColor.h"
#include "System/Graphic/Effect/libppp/draw/md.h"

void pppDrawMdlSemi(pppPObject *pobj, PDrawMdlSemi *p, pppCtrlTable *ctbl) {
    VColor *pv;
    OMD_IN mdi;
    pppModelSt *mdl;
    OMD_H *omd;

    if (p->model == 0xFFFF) {
        return;
    }
    pv = (VColor *)&pobj->val[ctbl->useVal[0]];
    mdi.a = pv->col.a >> 7;
    if (mdi.a == 0) {
        return;
    }
    mdl = &ppvEnv->lpModel[p->model];
    omd = mdl->omd;
    mdi.flag = 0x80;
    if (!(omd->flag & 2)) {
        mdi.flag = 0x90;
    }
    *(u_short *)&mdi.alpha = *(u_short *)&p->acalc;
    mdi.pri = 0;
    *(int *)&mdi.ar = 0;
    mdi.va = ppvPObj->lpvert;
    mdi.na = ppvPObj->lpnorm;
    mdi.tmp_a = (char *)ppvVNWork;
    mdi.r = pv->col.r >> 7;
    mdi.g = pv->col.g >> 7;
    mdi.b = pv->col.b >> 7;
    mdi.pmat = &ppvPObj->dmat;
    mdi.lcmat = &ppvColorMatrix;
    mdi.lvmat = &ppvLightMatrix;
    mdi.lwmat = &ppvPObj->cmat;
    mdi.packet = ppvCurPrimp;
    *(int *)&mdi.toff = 0;
    *(u_long *)&mdi.u = 0;
    *(int *)&mdi.ot = 0;
    mdi.pzmat = 0;
    ppvCurPrimp = op1_md_draw(omd, &mdi, mdl->nPacketSize);
}
