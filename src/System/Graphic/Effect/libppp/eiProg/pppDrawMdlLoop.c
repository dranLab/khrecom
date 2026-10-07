#include "pppDrawMdlLoop.h"
#include "pppColor.h"
#include "System/Graphic/Effect/libppp/draw/md.h"
#include <math.h>
#include "include_asm.h"

static void pppDrawMdlLoopInit(PDrawMdlLoop *p, VDrawMdlLoop *pv) {
    pv->m_tu = p->m_tu;
    pv->m_tv = p->m_tv;
    pv->m_dtu = pv->m_ddtu = pv->m_dtv = pv->m_ddtv = 0.0f;
    pv->m_pp = p;
}

void pppDrawMdlLoopConstruct(pppPObject *pobj, pppCtrlTable *ctbl) {
    VDrawMdlLoop *pv = (VDrawMdlLoop *)&pobj->val[ctbl->useVal[2]];

    pv->m_pp = NULL;
}

void pppDrawMdlLoopConstruct2(pppPObject *pobj, pppCtrlTable *ctbl) {
    VDrawMdlLoop *pv = (VDrawMdlLoop *)&pobj->val[ctbl->useVal[2]];

    pv->m_dtu = pv->m_ddtu = pv->m_dtv = pv->m_ddtv = 0.0f;
}

void pppDrawMdlLoopDestruct(pppPObject *pobj, pppCtrlTable *ctbl) {}

void pppDrawMdlLoopCalc(pppPObject *pobj, PDrawMdlLoop *p, pppCtrlTable *ctbl) {
    VDrawMdlLoop *pv = (VDrawMdlLoop *)&pobj->val[ctbl->useVal[2]];

    if (pv->m_pp == NULL) {
        pppDrawMdlLoopInit(p, pv);
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

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Effect/libppp/eiProg/pppDrawMdlLoop", pppDrawMdlLoopDraw);

/* Logic matches; the tail after the scissor block differs in register allocation (the original keeps
 * %hi(ppvPObj) in a2 across both branches) and store scheduling. Tried scissor-block orders, color and
 * uv statement orders. */
#if 0
void pppDrawMdlLoopDraw(pppPObject *pobj, PDrawMdlLoop *p, pppCtrlTable *ctbl) {
    VDrawMdlLoop *pv;
    VColor *pcol;
    OMD_H *omd;
    OMD_IN mdi;
    sceVu0FMATRIX wvmat;

    pv = (VDrawMdlLoop *)&pobj->val[ctbl->useVal[2]];
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
    *(int *)&mdi.ar = 0;
    mdi.lcmat = &ppvColorMatrix;
    mdi.pmat = &ppvPObj->dmat;
    mdi.lvmat = &ppvLightMatrix;
    mdi.lwmat = &ppvPObj->cmat;
    mdi.packet = ppvCurPrimp;
    mdi.pzmat = 0;
    mdi.va = ppvPObj->lpvert;
    *(int *)&mdi.ot = 0;
    mdi.na = ppvPObj->lpnorm;
    mdi.invisible = 0;
    mdi.ztest_off = 0;
    ppvCurPrimp = op1_md_draw(omd, &mdi, ppvEnv->lpModel[p->m_model].nPacketSize);
}
#endif
