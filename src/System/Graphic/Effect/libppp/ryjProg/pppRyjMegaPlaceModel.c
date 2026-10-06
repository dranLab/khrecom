#include "pppRyjMegaPlaceModel.h"

void pppRyjMegaPlaceModel(pppPObject *pPobj, PRyjMegaPlaceModel *pParam, pppCtrlTable *pCtbl) {
    VRyjMegaPlaceModel *pVal = (VRyjMegaPlaceModel *)&pPobj->val[pCtbl->useVal[1]];

    if (!pVal->bEntry) {
        if (entry_megaplace_model(&pPobj->val[pCtbl->useVal[0]], pParam)) {
            pVal->bEntry = 1;
        }
    }
}

void pppRyjMegaPlaceModelCon(pppPObject *pPobj, pppCtrlTable *pCtbl) {
    VRyjMegaPlaceModel *pVal = (VRyjMegaPlaceModel *)&pPobj->val[pCtbl->useVal[1]];
    VRyjMegaPlace *pMegaPlace = (VRyjMegaPlace *)&pPobj->val[pCtbl->useVal[0]];

    pVal->bEntry = 0;
    pMegaPlace->unModelNum++;
}
