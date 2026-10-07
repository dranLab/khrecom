#include "pppKeMdlTfdUv.h"
#include "KeMdl.h"

void pppKeMdlTfdUvDraw(pppPObject *pobj, PKeMdlTfdUv *dat, pppCtrlTable *ctbl) {
    sceGsLoadImage *gli;
    VKeMdlTfdUv *pThis;
    pppModelSt *pMSt;
    u_int h;
    int cOfs;

    pThis = (VKeMdlTfdUv *)&pobj->val[ctbl->useVal[IKeMdlTfdUv_This]];
    if (!ppvUserStopPartF && ppvMng->bFlagStop == 0 && ppvMng->bBoolSlow == 0) {
        pThis->LvSpd += pThis->LvAcc;
        pThis->Lv += pThis->LvSpd;
        pThis->dtu += pThis->ddtu;
        pThis->tu += pThis->dtu;
        pThis->dtv += pThis->ddtv;
        pThis->tv += pThis->dtv;
        if (pobj->time == dat->cdt.time) {
            pThis->Lv += dat->Lv;
            pThis->LvSpd += dat->LvSpd;
            pThis->LvAcc += dat->LvAcc;
            pThis->tu += dat->tu;
            pThis->dtu += dat->dtu;
            pThis->ddtu += dat->ddtu;
            pThis->tv += dat->tv;
            pThis->dtv += dat->dtv;
            pThis->ddtv += dat->ddtv;
        }
    }
    if (dat->Mdl != 0xFFFF) {
        u_long128 *pPkt;

        pMSt = &ppvEnv->lpModel[dat->Mdl];
        h = pMSt->tex0h;
        cOfs = dat->CltIdx * 4;
        gli = (sceGsLoadImage *)(ppvCurPrimp + 1);
        sceGsSetDefLoadImage(gli, ((h >> 5) & 0x3FFF) + cOfs, 4, (h >> 19) & 0xF, 0, 0, 16, 16);
        *(u_long *)&gli->giftag1 = 0x0800000000008100;
        pPkt = ppvCurPrimp;
        KeMdl_MkTkClut(*(u_int *)&dat->ColBs, *(u_int *)&dat->ColSs, pThis->Lv, (u_int *)(pPkt + 7));
        op_ot_add_before(ppvCurPrimp, 0x107, 0, 0);
        *(int *)&ppvCurPrimp = (int)(pPkt + 0x107);
        KeMdl_Draw(dat->Mdl, (u_short *)&pobj->val[ctbl->useVal[IKeMdlTfdUv_Color]], cOfs, (int)pThis->tu, (int)pThis->tv, 1, 0);
    }
}

void pppKeMdlTfdUvCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeMdlTfdUv *pThis;

    pThis = (VKeMdlTfdUv *)&pobj->val[ctbl->useVal[IKeMdlTfdUv_This]];
    pThis->Lv = pThis->LvSpd = pThis->LvAcc = pThis->tu = pThis->dtu = pThis->ddtu = pThis->tv = pThis->dtv = pThis->ddtv = 0.0f;
}
