#include "pppKeShpTailLc.h"

void pppKeShpTailLc(pppPObject *pobj, void *dat, pppCtrlTable *ctbl) {}

void pppKeShpTailLcDraw(pppPObject *pobj, void *dat, pppCtrlTable *ctbl) {}

void pppKeShpTailLcCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeShpTailLc *pThis = (VKeShpTailLc *)((char *)pobj + 0xa0 + ctbl->useVal[0]);

    pThis->StkNum = 31;
    pThis->Cnt = 0;
    pThis->Seq = 0;
    pThis->Cur = 0;
    pThis->Use = 0;
}
