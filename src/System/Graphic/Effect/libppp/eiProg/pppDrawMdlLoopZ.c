#include "pppDrawMdlLoopZ.h"
#include "pppColor.h"
#include "System/Graphic/Effect/libppp/draw/md.h"
#include <math.h>
#include "System/Graphic/Effect/libppp/keProg/pppKeZCrct.h"

static void pppDrawMdlLoopZInit(PDrawMdlLoopZ *p, VDrawMdlLoopZ *pv) {
    pv->m_tu = p->m_tu;
    pv->m_tv = p->m_tv;
    pv->m_dtu = pv->m_ddtu = pv->m_dtv = pv->m_ddtv = 0.0f;
    pv->m_pp = p;
}

void pppDrawMdlLoopZConstruct(pppPObject *pobj, pppCtrlTable *ctbl) {
    VDrawMdlLoopZ *pv = (VDrawMdlLoopZ *)&pobj->val[ctbl->useVal[2]];

    pv->m_pp = NULL;
}

void pppDrawMdlLoopZConstruct2(pppPObject *pobj, pppCtrlTable *ctbl) {
    VDrawMdlLoopZ *pv = (VDrawMdlLoopZ *)&pobj->val[ctbl->useVal[2]];

    pv->m_dtu = pv->m_ddtu = pv->m_dtv = pv->m_ddtv = 0.0f;
}

void pppDrawMdlLoopZDestruct(pppPObject *pobj, pppCtrlTable *ctbl) {}

void pppDrawMdlLoopZCalc(pppPObject *pobj, PDrawMdlLoopZ *p, pppCtrlTable *ctbl) {
    VDrawMdlLoopZ *pv = (VDrawMdlLoopZ *)&pobj->val[ctbl->useVal[2]];

    if (pv->m_pp == NULL) {
        pppDrawMdlLoopZInit(p, pv);
    }
    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    if (p->cdt.time == pobj->time) {
        pv->m_dtu += p->m_dtu;
        pv->m_ddtu += p->m_ddtu;
        pv->m_dtv += p->m_dtv;
        pv->m_ddtv += p->m_ddtv;
    }
    if (ppvEmptyLoop != 0) {
        return;
    }
    pv->m_dtu += pv->m_ddtu;
    pv->m_dtv += pv->m_ddtv;
    pv->m_tu += pv->m_dtu;
    pv->m_tv += pv->m_dtv;
    pv->m_tu = fmodf(pv->m_tu, 32768.0f);
    pv->m_tv = fmodf(pv->m_tv, 32768.0f);
}

void pppDrawMdlLoopZDraw(pppPObject *pobj, PDrawMdlLoopZ *p, pppCtrlTable *ctbl) {
    VDrawMdlLoopZ *pv;
    VColor *pcol;
    OMD_H *omd;
    OMD_IN mdi;
    sceVu0FMATRIX wvmat;
    VKeZCrct *pZ;

    pv = (VDrawMdlLoopZ *)&pobj->val[ctbl->useVal[2]];
    if (p->m_model == 0xFFFF) {
        return;
    }
    omd = ppvEnv->lpModel[p->m_model].omd;
    pcol = (VColor *)&pobj->val[ctbl->useVal[0]];
    mdi.r = pcol->release_col.r;
    mdi.g = pcol->release_col.g;
    mdi.a = pcol->release_col.a;
    mdi.b = pcol->release_col.b;
    if (mdi.a == 0) {
        return;
    }
    mdi.flag = 0;
    if (p->m_draw & 2) {
        mdi.flag = 4;
    } else if (p->m_draw & 1) {
        mdi.flag = 0x80;
    }
    if (!(omd->flag & 1)) {
        mdi.flag |= 8;
    }
    if (!(omd->flag & 2)) {
        mdi.flag |= 0x10;
    }
    if (p->m_fog) {
        mdi.flag |= 0x200;
    }
    if (p->m_near) {
        mdi.flag |= 0x100;
    }
    if (p->m_scis) {
        mdi.flag = (mdi.flag & ~0x10) | 0x2000;
        mdi.lviewmat = &wvmat;
        sceVu0MulMatrix(wvmat, ppvWorldMatrix, ppvPObj->cmat);
    } else {
        mdi.lviewmat = 0;
    }
    mdi.tmp_a = (char *)ppvVNWork;
    if (p->m_acalc == 0xFF) {
        *(u_short *)&mdi.alpha = *(u_short *)&omd->alpha;
    } else {
        *(u_short *)&mdi.alpha = *(u_short *)&p->m_acalc;
    }
    mdi.u = pv->m_tu * 65536.0f;
    mdi.v = pv->m_tv * 65536.0f;
    mdi.pri = 0;
    *(int *)&mdi.toff = 0;
    if (p->m_zCrct) {
        pZ = (VKeZCrct *)&pobj->val[ctbl->useVal[3]];
        mdi.pzmat = &pZ->Mat;
    } else {
        mdi.pzmat = 0;
    }
    *(int *)&mdi.ar = 0;
    mdi.pmat = &ppvPObj->dmat;
    mdi.lcmat = &ppvColorMatrix;
    mdi.lvmat = &ppvLightMatrix;
    mdi.lwmat = &ppvPObj->cmat;
    mdi.packet = ppvCurPrimp;
    mdi.va = ppvPObj->lpvert;
    mdi.na = ppvPObj->lpnorm;
    *(int *)&mdi.ot = 0;
    mdi.invisible = 0;
    mdi.ztest_off = 0;
    ppvCurPrimp = op1_md_draw(omd, &mdi, ppvEnv->lpModel[p->m_model].nPacketSize);
}
