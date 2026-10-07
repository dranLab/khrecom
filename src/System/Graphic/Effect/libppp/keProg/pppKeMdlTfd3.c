#include "pppKeMdlTfd3.h"
#include "KeMdl.h"

void pppKeMdlTfd3Draw(pppPObject *pobj, PKeMdlTfd3 *dat, pppCtrlTable *ctbl) {
    sceGsLoadImage *gli;
    VKeMdlTfd3 *pThis;
    pppModelSt *pMSt;
    u_int h;
    int cOfs;

    pThis = (VKeMdlTfd3 *)&pobj->val[ctbl->useVal[IKeMdlTfd3_This]];
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
        /* separate single-set pointer: gives the in-place add after op_ot_add_before */
        pPkt = ppvCurPrimp;
        KeMdl_MkTkClut(*(u_int *)&dat->ColBs, *(u_int *)&dat->ColSs, pThis->Lv, (u_int *)(pPkt + 7));
        op_ot_add_before(ppvCurPrimp, 0x107, 0, 0);
        /* stored as int: keeps the useVal load below the store */
        *(int *)&ppvCurPrimp = (int)(pPkt + 0x107);
        KeMdl_Draw(dat->Mdl, (u_short *)&pobj->val[ctbl->useVal[IKeMdlTfd3_Color]], cOfs, 0, 0,
                   dat->draw_mode | dat->fog | dat->near | dat->scissor | dat->zCrct,
                   (sceVu0FMATRIX *)&pobj->val[ctbl->useVal[IKeMdlTfd3_KeZCrct]]);
    }
}

void pppKeMdlTfd3Con(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeMdlTfd3 *pThis;

    pThis = (VKeMdlTfd3 *)&pobj->val[ctbl->useVal[IKeMdlTfd3_This]];
    pThis->Lv = 0.0f;
    pThis->LvSpd = 0.0f;
    pThis->LvAcc = 0.0f;
}
