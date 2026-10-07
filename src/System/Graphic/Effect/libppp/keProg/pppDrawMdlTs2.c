#include "pppDrawMdlTs2.h"
#include "System/Graphic/Effect/libppp/eiProg/pppColor.h"
#include "System/Graphic/Effect/libppp/draw/md.h"
#include "include_asm.h"

void pppDrawMdlTs2Con(pppPObject *pobj, pppCtrlTable *ctbl) {
    VDrawMdlTs2 *v = (VDrawMdlTs2 *)&pobj->val[ctbl->useVal[2]];

    v->tu = v->dtu = v->ddtu = v->tv = v->dtv = v->ddtv = 0.0f;
}

void pppDrawMdlTs2(pppPObject *pobj, PDrawMdlTs2 *p, pppCtrlTable *ctbl) {
    VDrawMdlTs2 *v = (VDrawMdlTs2 *)&pobj->val[ctbl->useVal[2]];

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    v->dtu += v->ddtu;
    v->dtv += v->ddtv;
    v->tu += v->dtu;
    v->tv += v->dtv;
    if (p->cdt.time != pobj->time) {
        return;
    }
    v->tu += p->tu;
    v->dtu += p->dtu;
    v->ddtu += p->ddtu;
    v->tv += p->tv;
    v->dtv += p->dtv;
    v->ddtv += p->ddtv;
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Effect/libppp/keProg/pppDrawMdlTs2", pppDrawDrawMdlTs2);

/* Differs only in the position of the mdi.pzmat store (the original stores it after mdi.ot, ours right
 * after mdi.ztest_off). Statement positions of pmat/pzmat/ot/invisible/ztest_off were searched (~700
 * orders, with and without the pmat = pzmat chain) without a match. */
#if 0
void pppDrawDrawMdlTs2(pppPObject *pobj, PDrawMdlTs2 *p, pppCtrlTable *ctbl) {
    VDrawMdlTs2 *v;
    VColor *pv;
    OMD_IN mdi;
    pppModelSt *mdl;
    OMD_H *omd;

    v = (VDrawMdlTs2 *)&pobj->val[ctbl->useVal[2]];
    if (p->model == 0xFFFF) {
        return;
    }
    pv = (VColor *)&pobj->val[ctbl->useVal[0]];
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
    *(u_short *)&mdi.alpha = *(u_short *)&omd->alpha;
    mdi.pri = 0;
    *(int *)&mdi.ar = 0;
    mdi.va = ppvPObj->lpvert;
    mdi.na = ppvPObj->lpnorm;
    mdi.tmp_a = (char *)ppvVNWork;
    mdi.r = pv->release_col.r;
    mdi.g = pv->release_col.g;
    mdi.b = pv->release_col.b;
    mdi.pmat = mdi.pzmat = &ppvPObj->dmat;
    mdi.lcmat = &ppvColorMatrix;
    mdi.lvmat = &ppvLightMatrix;
    mdi.lwmat = &ppvPObj->cmat;
    mdi.packet = ppvCurPrimp;
    *(int *)&mdi.toff = 0;
    mdi.u = v->tu * 65536.0f;
    mdi.v = v->tv * 65536.0f;
    *(int *)&mdi.ot = 0;
    mdi.invisible = 0;
    mdi.ztest_off = p->ztest;
    ppvCurPrimp = op1_md_draw(omd, &mdi, mdl->nPacketSize);
}
#endif
