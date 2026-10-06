#include "pppKeDrct.h"

void pppKeDrct(pppPObject *pobj, PKeDrct *dat, pppCtrlTable *ctbl) {
    VKeDrct *_t;

    if (pobj->time == dat->cdt.time) {
        _t = (VKeDrct *)&pobj->val[ctbl->useVal[0]];
        _t->Drct[0] = dat->Drct.x;
        _t->Drct[1] = dat->Drct.y;
        _t->Drct[2] = dat->Drct.z;
    }
}

void pppKeDrctCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeDrct *_t = (VKeDrct *)&pobj->val[ctbl->useVal[0]];

    _t->Drct[3] = 1.0f;
}
