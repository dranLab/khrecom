#include "pppRyjMegaPlaceShape.h"

void pppRyjMegaPlaceShape(pppPObject *pPobj, PRyjMegaPlaceShape *pParam, pppCtrlTable *pCtbl) {
    VRyjMegaPlaceShape *pVal = (VRyjMegaPlaceShape *)&pPobj->val[pCtbl->useVal[1]];

    if (!pVal->bEntry) {
        if (entry_megaplace_shape(&pPobj->val[pCtbl->useVal[0]], pParam)) {
            pVal->bEntry = 1;
        }
    }
}

void pppRyjMegaPlaceShapeCon(pppPObject *pPobj, pppCtrlTable *pCtbl) {
    VRyjMegaPlaceShape *pVal = (VRyjMegaPlaceShape *)&pPobj->val[pCtbl->useVal[1]];
    VRyjMegaPlace *pMegaPlace = (VRyjMegaPlace *)&pPobj->val[pCtbl->useVal[0]];

    pVal->bEntry = 0;
    pMegaPlace->unShapeNum++;
}
