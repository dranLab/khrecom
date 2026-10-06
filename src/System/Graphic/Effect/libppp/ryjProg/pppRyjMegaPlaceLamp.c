#include "pppRyjMegaPlaceLamp.h"

void pppRyjMegaPlaceLamp(pppPObject *pPobj, PRyjMegaPlaceLamp *pParam, pppCtrlTable *pCtbl) {
    VRyjMegaPlaceLamp *pVal = (VRyjMegaPlaceLamp *)&pPobj->val[pCtbl->useVal[1]];

    if (!pVal->bEntry) {
        if (entry_megaplace_lamp(&pPobj->val[pCtbl->useVal[0]], pParam)) {
            pVal->bEntry = 1;
        }
    }
}

void pppRyjMegaPlaceLampCon(pppPObject *pPobj, pppCtrlTable *pCtbl) {
    VRyjMegaPlaceLamp *pVal = (VRyjMegaPlaceLamp *)&pPobj->val[pCtbl->useVal[1]];
    VRyjMegaPlace *pMegaPlace = (VRyjMegaPlace *)&pPobj->val[pCtbl->useVal[0]];

    pVal->bEntry = 0;
    pMegaPlace->unLampNum++;
}
