#include "pppKeTh.h"
#include "System/Graphic/Effect/libppp/keProg/KeBornRnd.h"
#include "System/Debug.h"

void pppKeTh(pppPObject *pobj, PKeTh *dat, pppCtrlTable *ctbl) {
    sceVu0FMATRIX fm0;
    sceVu0FVECTOR fv0;
    sceVu0FVECTOR fv1;
    KeaThCp_Update aCp;
    VKeTh *_t = (VKeTh *)&pobj->val[ctbl->useVal[IKeTh_This]];
    VKeDrct *pDr = (VKeDrct *)&pobj->val[ctbl->useVal[IKeTh_KeDrct]];
    KetThBp_Pr *pPr = &_t->Pr;
    VKeThRes *pRes;
    VKeThTp *pTp;
    float y;

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    sceVu0CopyMatrix(fm0, ppvPObj->cmat);
    *(u_long128 *)fm0[3] = 0;
    fm0[3][3] = 1.0f;
    sceVu0ApplyMatrix(fv0, fm0, pDr->Drct);
    sceVu0Normalize(fv0, fv0);
    sceVu0CopyVector(fv1, ppvPObj->cmat[3]);
    if (pobj->time == dat->cdt.time) {
        if (pobj->time == 0) {
            pRes = (VKeThRes *)&pobj->val[ctbl->useVal[IKeTh_Res]];
            pTp = (dat->Tp.pdtn != 0xFFFFFFFF)
                      ? (VKeThTp *)&ppvMng->m_pdtval[dat->Tp.pdtn].pobj->val[dat->Tp.valoff]
                      : (VKeThTp *)ppvDbgTemp;
            if (_t->Tp == (KeThTP *)ppvDbgTemp) {
                TRACE("KemPCalc() :: Cp: No Tp.\n");
                return;
            }
            _t->Tp = &pTp->Tp;
            _t->Res = &pRes->Hd;
            if (!dat->SttQuick) {
                _t->Cp.Stt |= KeThCp_SttSpdFix;
            }
            if (dat->SttLazer) {
                _t->Cp.Stt |= KeThCp_SttNonStop;
            }
            if (dat->SttCpSync) {
                _t->Cp.Stt |= KeThCp_SttCpSync;
            }
            if (dat->SttChange) {
                _t->Cp.Stt |= KeThCp_SttRvrs;
            }
            KeThCp_AddBp(&_t->Cp, _t->Res, &fv1, &fv0);
        }
        _t->ColCe.r += dat->ColCe.r;
        _t->ColCe.g += dat->ColCe.g;
        _t->ColCe.b += dat->ColCe.b;
        _t->ColCe.a += dat->ColCe.a;
        _t->ColEg.r += dat->ColEg.r;
        _t->ColEg.g += dat->ColEg.g;
        _t->ColEg.b += dat->ColEg.b;
        _t->ColEg.a += dat->ColEg.a;
        _t->AddColCe.r += dat->AddColCe.r;
        _t->AddColCe.g += dat->AddColCe.g;
        _t->AddColCe.b += dat->AddColCe.b;
        _t->AddColCe.a += dat->AddColCe.a;
        _t->AddColEg.r += dat->AddColEg.r;
        _t->AddColEg.g += dat->AddColEg.g;
        _t->AddColEg.b += dat->AddColEg.b;
        _t->AddColEg.a += dat->AddColEg.a;
        _t->MaxLen = dat->MaxLen;
        _t->Spd += dat->Spd;
        pPr->MaxStep = _t->Res->RPNum - 1;
        pPr->Wid += dat->RootWid;
        pPr->CePstg = dat->CntrWid;
        pPr->MvWidSz = dat->MvWidSz;
        pPr->MvWidMv = dat->MvWidMv;
        pPr->Wild += dat->Wild;
        pPr->Corner += dat->Corner;
        pPr->Curv = dat->Curv * 3.141592654 * (3.141592654 / 180.0);
        pPr->LowMax = dat->LowNum;
        pPr->Pstg = dat->LowPstg;
        pPr->AddWid = -(pPr->Wid - pPr->Wid * dat->TopWid) / (float)pPr->MaxStep;
    }
    pPr->Spd1st = _t->MaxLen / (float)pPr->MaxStep;
    pPr->StepSpd = _t->Spd / pPr->Spd1st;
    pPr->ColCe[0] = _t->ColCe.r >> 7;
    pPr->ColCe[1] = _t->ColCe.g >> 7;
    pPr->ColCe[2] = _t->ColCe.b >> 7;
    pPr->ColCe[3] = _t->ColCe.a >> 7;
    pPr->ColLt[0] = _t->ColEg.r >> 7;
    pPr->ColLt[1] = _t->ColEg.g >> 7;
    pPr->ColLt[2] = _t->ColEg.b >> 7;
    pPr->ColLt[3] = _t->ColEg.a >> 7;
    pPr->AddColCe[0] = _t->AddColCe.r >> 7;
    pPr->AddColCe[1] = _t->AddColCe.g >> 7;
    pPr->AddColCe[2] = _t->AddColCe.b >> 7;
    pPr->AddColCe[3] = _t->AddColCe.a >> 7;
    pPr->AddColLt[0] = _t->AddColEg.r >> 7;
    pPr->AddColLt[1] = _t->AddColEg.g >> 7;
    pPr->AddColLt[2] = _t->AddColEg.b >> 7;
    pPr->AddColLt[3] = _t->AddColEg.a >> 7;
    if (ppvMonochrome) {
        y = pPr->ColCe[0] * 0.3f + pPr->ColCe[1] * 0.59f + pPr->ColCe[2] * 0.11f;
        pPr->ColCe[0] = pPr->ColCe[1] = pPr->ColCe[2] = (int)y;
        y = pPr->ColLt[0] * 0.3f + pPr->ColLt[1] * 0.59f + pPr->ColLt[2] * 0.11f;
        pPr->ColLt[0] = pPr->ColLt[1] = pPr->ColLt[2] = (int)y;
        y = pPr->AddColCe[0] * 0.3f + pPr->AddColCe[1] * 0.59f + pPr->AddColCe[2] * 0.11f;
        pPr->AddColCe[0] = pPr->AddColCe[1] = pPr->AddColCe[2] = (int)y;
        y = pPr->AddColLt[0] * 0.3f + pPr->AddColLt[1] * 0.59f + pPr->AddColLt[2] * 0.11f;
        pPr->AddColLt[0] = pPr->AddColLt[1] = pPr->AddColLt[2] = (int)y;
    }
    aCp.This = &_t->Cp;
    aCp.TP = _t->Tp;
    aCp.Res = _t->Res;
    aCp.Pr = &_t->Pr;
    aCp.RPos = &fv1;
    aCp.Vec = &fv0;
    KeThCp_Update(&aCp);
}

void pppKeThDraw(pppPObject *pobj, PKeTh *dat, pppCtrlTable *ctbl) {
    u_long Alpha;
    u_long Test;
    float scl[2];
    VKeTh *_t = (VKeTh *)&pobj->val[ctbl->useVal[IKeTh_This]];

    scl[0] = ppvParScl[0];
    scl[1] = ppvParScl[1];
    switch (dat->Alpha) {
    case 0:
        Test = 0x51801;
        Alpha = 0x48;
        break;
    case 1:
        Test = 0x51801;
        Alpha = 0x42;
        break;
    default:
        Test = 0x70000;
        Alpha = 0x80000000A8;
        break;
    }
    KeThCp_Draw(&_t->Cp, &ppvSWMatrix, scl, &_t->Pr, &ppvCurPrimp, Alpha, Test);
}

void pppKeThCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeTh *_t = (VKeTh *)&pobj->val[ctbl->useVal[IKeTh_This]];
    KetThBp_Pr *pPr = &_t->Pr;

    KeThCp_Init(&_t->Cp);
    *(u_long *)&_t->ColCe = 0;
    *(u_long *)&_t->ColEg = 0;
    *(u_long *)&_t->AddColCe = 0;
    *(u_long *)&_t->AddColEg = 0;
    _t->Spd = 0.0f;
    pPr->Wid = 0.0f;
    pPr->Wild = 0.0f;
    pPr->Corner = 0.0f;
}

void pppKeThCon2(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeTh *_t = (VKeTh *)&pobj->val[ctbl->useVal[IKeTh_This]];
    KetThBp_Pr *pPr = &_t->Pr;

    *(u_long *)&_t->ColCe = 0;
    *(u_long *)&_t->ColEg = 0;
    *(u_long *)&_t->AddColCe = 0;
    *(u_long *)&_t->AddColEg = 0;
    _t->Spd = 0.0f;
    pPr->Wid = 0.0f;
    pPr->Wild = 0.0f;
    pPr->Corner = 0.0f;
}

void pppKeThDes(pppPObject *pobj, pppCtrlTable *ctbl) {
}

void pppKeThTp(pppPObject *pobj, PKeThTp *dat, pppCtrlTable *ctbl) {
    VKeThTp *_t = (VKeThTp *)&pobj->val[ctbl->useVal[IKeThTp_This]];

    if (pobj->time == dat->cdt.time) {
        _t->Tp.ValidR = dat->ValidR;
        _t->Tp.HitR = dat->HitR;
        _t->Tp.Pow = dat->Power;
        _t->Tp.Nxt = (dat->Next.pdtn != 0xFFFFFFFF)
                         ? (KeThTP *)&ppvMng->m_pdtval[dat->Next.pdtn].pobj->val[dat->Next.valoff]
                         : (KeThTP *)ppvDbgTemp;
        if (_t->Tp.Nxt == (KeThTP *)ppvDbgTemp) {
            _t->Tp.Nxt = NULL;
        }
    }
    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    KeVu0CopyVecXYZ(_t->Tp.RPos, ppvPObj->cmat[3]);
}

void pppKeThTpCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeThTp *_t = (VKeThTp *)&pobj->val[ctbl->useVal[IKeThTp_This]];

    _t->Tp.Nxt = NULL;
    _t->Tp.RPos[3] = 0.0f;
}

void pppKeThTp2Con(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeThTp2 *_t = (VKeThTp2 *)&pobj->val[ctbl->useVal[IKeThTp_This]];

    _t->Tp.RPos[3] = 1.0f;
    _t->Tp.Nxt = NULL;
}

#define KETH_RES_CON(name, bp, rp)                                              \
    void name(pppPObject *pobj, pppCtrlTable *ctbl) {                           \
        KeThResHd_Init((KeThResHd *)&pobj->val[ctbl->useVal[0]], bp, rp);       \
    }

KETH_RES_CON(pppKeThResCon, 1, 3)
KETH_RES_CON(pppKeThRes8Con, 1, 8)
KETH_RES_CON(pppKeThRes8x4Con, 4, 8)
KETH_RES_CON(pppKeThRes8x128Con, 128, 8)
KETH_RES_CON(pppKeThRes16Con, 1, 16)
KETH_RES_CON(pppKeThRes16x4Con, 4, 16)
KETH_RES_CON(pppKeThRes16x64Con, 64, 16)
KETH_RES_CON(pppKeThRes24Con, 1, 24)
KETH_RES_CON(pppKeThRes24x4Con, 4, 24)
KETH_RES_CON(pppKeThRes32Con, 1, 32)
KETH_RES_CON(pppKeThRes32x4Con, 4, 32)
KETH_RES_CON(pppKeThRes32x32Con, 32, 32)
KETH_RES_CON(pppKeThRes40Con, 1, 40)
KETH_RES_CON(pppKeThRes40x4Con, 4, 40)
KETH_RES_CON(pppKeThRes48Con, 1, 48)
KETH_RES_CON(pppKeThRes48x4Con, 4, 48)
KETH_RES_CON(pppKeThRes64Con, 1, 64)
KETH_RES_CON(pppKeThRes64x4Con, 4, 64)
KETH_RES_CON(pppKeThRes64x16Con, 16, 64)
KETH_RES_CON(pppKeThRes128Con, 1, 128)
KETH_RES_CON(pppKeThRes128x4Con, 4, 128)
KETH_RES_CON(pppKeThRes128x8Con, 8, 128)
KETH_RES_CON(pppKeThRes255Con, 1, 255)
KETH_RES_CON(pppKeThRes255x4Con, 4, 255)
// SOURCE DATA: .rodata
