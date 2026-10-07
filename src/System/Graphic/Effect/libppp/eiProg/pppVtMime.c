#include "pppVtMime.h"
#include "System/Graphic/Effect/libppp/pppAlloc.h"

void pppVtMime(pppPObject *pobj, PVtMime *p, pppCtrlTable *ctbl) {
    VVtMime *vvm = (VVtMime *)&pobj->val[ctbl->useVal[0]];

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    vvm->dinp += vvm->ddinp;
    vvm->inp += vvm->dinp;
    if (p->cdt.time != pobj->time) {
        return;
    }
    vvm->inp += p->inp;
    vvm->dinp += p->dinp;
    vvm->ddinp += p->ddinp;
}

void pppDrawVtMime(pppPObject *pobj, PVtMime *p, pppCtrlTable *ctbl) {
    VVtMime *vvm;
    pppSVECTOR *v0;
    pppSVECTOR *v1;
    int nv;
    pppSVECTOR *v;

    if (p->sta_mdlno == 0xFFFF && p->end_mdlno == 0xFFFF) {
        return;
    }
    vvm = (VVtMime *)&pobj->val[ctbl->useVal[0]];
    v0 = ppvEnv->lpModel[p->sta_mdlno].vert;
    nv = ppvEnv->lpModel[p->sta_mdlno].nvert;
    v1 = ppvEnv->lpModel[p->end_mdlno].vert;
    if (vvm->lpvt == NULL) {
        vvm->lpvt = pppAlloc(&ppvEnv->heapMng, nv * sizeof(pppSVECTOR));
        if (vvm->lpvt == NULL) {
            return;
        }
    }
    v = vvm->lpvt;
    while (nv-- != 0) {
        v->x = (v1->x - v0->x) * vvm->inp + v0->x;
        v->y = (v1->y - v0->y) * vvm->inp + v0->y;
        v->z = (v1->z - v0->z) * vvm->inp + v0->z;
        v0++;
        v1++;
        v++;
    }
    ppvPObj->lpvert = vvm->lpvt;
}

void pppVtMimeCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VVtMime *vvm = (VVtMime *)&pobj->val[ctbl->useVal[0]];

    vvm->lpvt = NULL;
    vvm->inp = vvm->dinp = vvm->ddinp = 0.0f;
}

void pppVtMimeCon2(pppPObject *pobj, pppCtrlTable *ctbl) {
    VVtMime *vvm = (VVtMime *)&pobj->val[ctbl->useVal[0]];

    vvm->inp = vvm->dinp = vvm->ddinp = 0.0f;
}

void pppVtMimeDes(pppPObject *pobj, pppCtrlTable *ctbl) {
    VVtMime *vvm = (VVtMime *)&pobj->val[ctbl->useVal[0]];

    if (vvm->lpvt != NULL) {
        pppFree(&ppvEnv->heapMng, vvm->lpvt);
        vvm->lpvt = NULL;
    }
}
