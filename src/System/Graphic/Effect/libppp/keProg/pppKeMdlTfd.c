#include "pppKeMdlTfd.h"
#include "KeMdl.h"

void pppKeMdlTfdDraw(pppPObject *pobj, PKeMdlTfd *dat, pppCtrlTable *ctbl) {
    sceGsLoadImage *gli;
    VKeMdlTfd *pThis;
    pppModelSt *pMSt;
    u_int h;
    int cOfs;

    pThis = (VKeMdlTfd *)&pobj->val[ctbl->useVal[IKeMdlTfd_This]];
    if (!ppvUserStopPartF && ppvMng->bFlagStop == 0 && ppvMng->bBoolSlow == 0) {
        pThis->LvSpd += pThis->LvAcc;
        pThis->Lv += pThis->LvSpd;
        if (pobj->time == dat->cdt.time) {
            pThis->Lv += dat->Lv;
            pThis->LvSpd += dat->LvSpd;
            pThis->LvAcc += dat->LvAcc;
        }
    }
    if (dat->Mdl != 0xFFFF) {
        u_long128 *pPkt;

        pMSt = &ppvEnv->lpModel[dat->Mdl];
        h = pMSt->tex0h;
        cOfs = dat->CltIdx * 4;
        gli = (sceGsLoadImage *)(ppvCurPrimp + 1);
        sceGsSetDefLoadImage(gli, ((h >> 5) & 0x3FFF) + cOfs, 4, (h >> 19) & 0xF, 0, 0, 16, 16);
        /* not a struct-member store: keeps the ppvCurPrimp reload below it */
        *(u_long *)&gli->giftag1 = 0x0800000000008100;
        /* separate single-set pointer: gives the in-place add after op_ot_add_before (as gli reused, it is hoisted) */
        pPkt = ppvCurPrimp;
        KeMdl_MkTkClut(*(u_int *)&dat->ColBs, *(u_int *)&dat->ColSs, pThis->Lv, (u_int *)(pPkt + 7));
        op_ot_add_before(ppvCurPrimp, 0x107, 0, 0);
        /* stored as int: keeps the useVal load below the store */
        *(int *)&ppvCurPrimp = (int)(pPkt + 0x107);
        KeMdl_Draw(dat->Mdl, (u_short *)&pobj->val[ctbl->useVal[IKeMdlTfd_Color]], cOfs, 0, 0, 1, 0);
    }
}

void pppKeMdlTfdCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeMdlTfd *pThis;

    pThis = (VKeMdlTfd *)&pobj->val[ctbl->useVal[IKeMdlTfd_This]];
    pThis->Lv = 0.0f;
    pThis->LvSpd = 0.0f;
    pThis->LvAcc = 0.0f;
}
