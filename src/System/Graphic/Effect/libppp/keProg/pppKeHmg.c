#include "pppKeHmg.h"

void pppKeHmgEff(pppPObject *pobj, PKeHmgEff *dat, pppCtrlTable *ctbl) {
    sceVu0FVECTOR fv0;
    sceVu0FVECTOR fv1;
    sceVu0FVECTOR vec;
    VKeHmgEff *_t = (VKeHmgEff *)&pobj->val[ctbl->useVal[IKeHmgEff_This]];
    VPoint *pTgt;
    VMove *pMv;
    float e;

    if (pobj->time == dat->cdt.time) {
        _t->Corner = dat->Corner;
        _t->Tgt = (dat->Tgt.pdtn != 0xFFFFFFFF)
                      ? (VPoint *)&ppvMng->m_pdtval[dat->Tgt.pdtn].pobj->val[dat->Tgt.valoff]
                      : (VPoint *)ppvDbgTemp;
        if (pobj->time == 0 && pobj->loop_in == 0) {
            return;
        }
    }
    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    pTgt = _t->Tgt;
    if (pTgt == (VPoint *)ppvDbgTemp) {
        return;
    }
    {
        float *pPos = (float *)&pobj->val[ctbl->useVal[IKeHmgEff_Point]];
        pMv = (VMove *)&pobj->val[ctbl->useVal[IKeHmgEff_Move]];
        sceVu0SubVector(fv0, (float *)&pTgt->pos, pPos);
    }
    sceVu0Normalize(fv0, fv0);
    sceVu0CopyVector(vec, (float *)&pMv->vec);
    e = KeVu0VecLen(vec);
    sceVu0Normalize(vec, vec);
    sceVu0AddVector(fv1, vec, fv0);
    if (((long *)fv1)[0] == 0 && ((long *)fv1)[1] == 0) {
        fv0[KeVu0RndI() % 3] += 0.05f;
    }
    sceVu0InterVector(fv0, fv0, vec, _t->Corner);
    sceVu0Normalize(fv0, fv0);
    sceVu0ScaleVector(fv0, fv0, e);
    KeVu0CopyVecXYZ((float *)&pMv->vec, fv0);
}

void pppKeHmgEffCon(pppPObject *pobj, pppCtrlTable *ctbl) {
}

void pppKeGrvTgt(pppPObject *pobj, PKeGrvTgt *dat, pppCtrlTable *ctbl) {
    VKeGrvTgt *_t;

    if (pobj->time == dat->cdt.time) {
        _t = (VKeGrvTgt *)&pobj->val[ctbl->useVal[IKeGrvTgt_This]];
        _t->ValidR = dat->ValidR;
        _t->Pow = dat->Pow;
        _t->Pt = (VPoint *)&pobj->val[ctbl->useVal[IKeGrvTgt_Point]];
    }
}

void pppKeGrvTgtCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeGrvTgt *_t = (VKeGrvTgt *)&pobj->val[ctbl->useVal[IKeGrvTgt_This]];

    _t->Pt = NULL;
}

void pppKeGrvEff(pppPObject *pobj, PKeGrvEff *dat, pppCtrlTable *ctbl) {
    sceVu0FVECTOR fv0;
    VKeGrvEff *_t = (VKeGrvEff *)&pobj->val[ctbl->useVal[IKeGrvEff_This]];
    VKeGrvTgt *pTgt;
    VMove *pMv;
    float f;

    if (pobj->time == dat->cdt.time) {
        _t->Tgt = (dat->Tgt.pdtn != 0xFFFFFFFF)
                      ? (VKeGrvTgt *)&ppvMng->m_pdtval[dat->Tgt.pdtn].pobj->val[dat->Tgt.valoff]
                      : (VKeGrvTgt *)ppvDbgTemp;
        if (pobj->time == 0 && pobj->loop_in == 0) {
            return;
        }
    }
    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    pTgt = _t->Tgt;
    if (pTgt == (VKeGrvTgt *)ppvDbgTemp) {
        return;
    }
    {
        float *pPos = (float *)&pobj->val[ctbl->useVal[IKeGrvEff_Point]];
        pMv = (VMove *)&pobj->val[ctbl->useVal[IKeGrvEff_Move]];
        sceVu0SubVector(fv0, (float *)&pTgt->Pt->pos, pPos);
    }
    f = KeVu0VecLen(fv0);
    sceVu0Normalize(fv0, fv0);
    if (f < pTgt->ValidR) {
        sceVu0ScaleVector(fv0, fv0, (1.0f - f / pTgt->ValidR) * pTgt->Pow);
        KeVu0AddVecXYZ((float *)&pMv->vec, (float *)&pMv->vec, fv0);
    }
}

void pppKeGrvEffCon(pppPObject *pobj, pppCtrlTable *ctbl) {
}

void pppKeHitBall(pppPObject *pobj, PKeHitBall *dat, pppCtrlTable *ctbl) {
    VKeHitBall *_t;

    if (pobj->time == dat->cdt.time) {
        _t = (VKeHitBall *)&pobj->val[ctbl->useVal[IKeHitBall_This]];
        _t->Pt = (VPoint *)&pobj->val[ctbl->useVal[IKeHitBall_Point]];
        _t->HitR = dat->HitR;
    }
}

void pppKeHitBallCon(pppPObject *pobj, pppCtrlTable *ctbl) {
}

void pppKeHitBorn(pppPObject *pobj, PKeHitBorn *dat, pppCtrlTable *ctbl) {
    VKeHitBorn *_t = (VKeHitBorn *)&pobj->val[ctbl->useVal[IKeHitBorn_This]];
    VKeHitChk *pHitChk;
    pppPObject *pObj;
    VPoint *pPt;
    VPoint *pBornPt;
    pppPDataVal *pdv;

    if (_t->Stt & FKeHitBorn_SttBorned) {
        return;
    }
    if (pobj->time == dat->cdt.time) {
        if (dat->CmdContinue) {
            _t->Stt |= FKeHitBorn_SttContinue;
        } else {
            _t->Stt &= ~FKeHitBorn_SttContinue;
        }
    }
    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    pHitChk = (VKeHitChk *)&pobj->val[ctbl->useVal[IKeHitBorn_KeHitChk]];
    if (!(pHitChk->Stt & FKeHitChk_SttHit)) {
        return;
    }
    if (dat->BornPt.pdtn == 0xFFFFFFFF) {
        return;
    }
    pdv = &ppvMng->m_pdtval[dat->BornPt.pdtn];
    if (pdv == NULL) {
        pObj = NULL;
    } else {
        pObj = pppCreatePObject(ppvMng, pdv);
        if (pObj == NULL) {
            return;
        }
        pObj->ppobj = pobj;
    }
    if (pObj == NULL) {
        return;
    }
    pBornPt = (VPoint *)&pObj->val[dat->BornPt.valoff];
    pPt = (VPoint *)&pobj->val[ctbl->useVal[IKeHitBorn_Point]];
    if (!(_t->Stt & FKeHitBorn_SttContinue)) {
        _t->Stt |= FKeHitBorn_SttBorned;
        KeVu0CopyVecXYZ((float *)&pBornPt->pos, pHitChk->HitPos);
    } else {
        KeVu0CopyVecXYZ((float *)&pBornPt->pos, (float *)&pPt->pos);
    }
}

void pppKeHitBornCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeHitBorn *_t = (VKeHitBorn *)&pobj->val[ctbl->useVal[IKeHitBorn_This]];

    _t->Stt = FKeHitBorn_SttClr_;
}

void pppKeHitChkPxB(pppPObject *pobj, PKeHitChkPxB *dat, pppCtrlTable *ctbl) {
    KeaVu0ItrsBallLine aItrs;
    VKeHitChkPxB *_t = (VKeHitChkPxB *)&pobj->val[ctbl->useVal[IKeHitChkPxB_This]];
    VPoint *pPt;
    VMove *pMv;
    sceVu0FVECTOR fv0;

    if (_t->Stt & FKeHitChkPxB_SttHit) {
        return;
    }
    if (pobj->time == dat->cdt.time) {
        _t->Tgt = (dat->Tgt.pdtn != 0xFFFFFFFF)
                      ? (VKeHitBall *)&ppvMng->m_pdtval[dat->Tgt.pdtn].pobj->val[dat->Tgt.valoff]
                      : (VKeHitBall *)ppvDbgTemp;
        if (dat->CmdHitErase) {
            _t->Stt |= FKeHitChkPxB_SttHitErase;
        }
        if (pobj->time == 0 && pobj->loop_in == 0) {
            return;
        }
    }
    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    if (_t->Tgt == (VKeHitBall *)ppvDbgTemp) {
        return;
    }
    pPt = (VPoint *)&pobj->val[ctbl->useVal[IKeHitChkPxB_Point]];
    pMv = (VMove *)&pobj->val[ctbl->useVal[IKeHitChkPxB_Move]];
    if (_t->Tgt->HitR == 0.0f) {
        return;
    }
    aItrs.BallR = _t->Tgt->HitR;
    sceVu0SubVector(aItrs.LinePos, (float *)&pPt->pos, (float *)&_t->Tgt->Pt->pos);
    sceVu0CopyVector(aItrs.LineDist, (float *)&pMv->vec);
    if (!KeVu0ItrsBallLine(&aItrs)) {
        return;
    }
    if (aItrs.T < -0.0f || aItrs.T > 1.0f) {
        return;
    }
    _t->Stt |= FKeHitChkPxB_SttHit;
    sceVu0ScaleVector(fv0, aItrs.LineDist, aItrs.T);
    sceVu0AddVector(fv0, fv0, aItrs.LinePos);
    sceVu0AddVector(_t->HitPos, (float *)&_t->Tgt->Pt->pos, fv0);
    if (_t->Stt & FKeHitChkPxB_SttHitErase) {
        KeVu0SubVecXYZ((float *)&pMv->vec, _t->HitPos, (float *)&pPt->pos);
        pobj->time = 0xFFFF0000;
    }
}

void pppKeHitChkPxBCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeHitChkPxB *_t = (VKeHitChkPxB *)&pobj->val[ctbl->useVal[IKeHitChkPxB_This]];

    _t->HitPos[3] = 1.0f;
    _t->Stt = FKeHitChkPxB_SttClr_;
}
// SOURCE DATA: .rodata
