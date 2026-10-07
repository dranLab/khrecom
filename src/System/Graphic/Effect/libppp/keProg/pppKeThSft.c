#include "pppKeThSft.h"

void pppKeThSft(pppPObject *pobj, PKeThSft *dat, pppCtrlTable *ctbl) {
    VKeThSft *pThis;
    VKeTh *pCp;
    KetThBp_Pr *pPr;

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    pThis = (VKeThSft *)&pobj->val[ctbl->useVal[IKeThSft_This]];
    pCp = (VKeTh *)&pobj->val[ctbl->useVal[IKeThSft_KeTh]];
    pPr = &pCp->Pr;
    if (pobj->time == dat->cdt.time) {
        pCp->Pr.LowMax = dat->LowNum;
        pThis->Spd += dat->Spd;
        pThis->Wid += dat->RootWid;
        pThis->Wild += dat->Wild;
        pThis->Corner += dat->Corner;
        pThis->ColCe.r += dat->ColCe.r;
        pThis->ColCe.g += dat->ColCe.g;
        pThis->ColCe.b += dat->ColCe.b;
        pThis->ColCe.a += dat->ColCe.a;
        pThis->ColEg.r += dat->ColEg.r;
        pThis->ColEg.g += dat->ColEg.g;
        pThis->ColEg.b += dat->ColEg.b;
        pThis->ColEg.a += dat->ColEg.a;
        pThis->AddColCe.r += dat->AddColCe.r;
        pThis->AddColCe.g += dat->AddColCe.g;
        pThis->AddColCe.b += dat->AddColCe.b;
        pThis->AddColCe.a += dat->AddColCe.a;
        pThis->AddColEg.r += dat->AddColEg.r;
        pThis->AddColEg.g += dat->AddColEg.g;
        pThis->AddColEg.b += dat->AddColEg.b;
        pThis->AddColEg.a += dat->AddColEg.a;
        if (pobj->time == 0 && pobj->loop_in == 0) {
            return;
        }
    }
    pCp->Spd += pThis->Spd;
    pPr->Wid += pThis->Wid;
    pPr->Wild += pThis->Wild;
    pPr->Corner += pThis->Corner;
    pCp->ColCe.r += pThis->ColCe.r;
    pCp->ColCe.g += pThis->ColCe.g;
    pCp->ColCe.b += pThis->ColCe.b;
    pCp->ColCe.a += pThis->ColCe.a;
    pCp->ColEg.r += pThis->ColEg.r;
    pCp->ColEg.g += pThis->ColEg.g;
    pCp->ColEg.b += pThis->ColEg.b;
    pCp->ColEg.a += pThis->ColEg.a;
    pCp->AddColCe.r += pThis->AddColCe.r;
    pCp->AddColCe.g += pThis->AddColCe.g;
    pCp->AddColCe.b += pThis->AddColCe.b;
    pCp->AddColCe.a += pThis->AddColCe.a;
    pCp->AddColEg.r += pThis->AddColEg.r;
    pCp->AddColEg.g += pThis->AddColEg.g;
    pCp->AddColEg.b += pThis->AddColEg.b;
    pCp->AddColEg.a += pThis->AddColEg.a;
}

void pppKeThSftCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeThSft *pThis = (VKeThSft *)&pobj->val[ctbl->useVal[IKeThSft_This]];

    *(int *)&pThis->Spd = 0;
    *(int *)&pThis->Wid = 0;
    *(int *)&pThis->Wild = 0;
    *(int *)&pThis->Corner = 0;
    *(u_long *)&pThis->ColCe = 0;
    *(u_long *)&pThis->ColEg = 0;
    *(u_long *)&pThis->AddColCe = 0;
    *(u_long *)&pThis->AddColEg = 0;
}
