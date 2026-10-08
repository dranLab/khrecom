#include "pppKeBornRnd6.h"
#include "System/Graphic/Effect/libppp/eiProg/pppAngle.h"
#include <math.h>

int _KeGetDeg2(float *xy, float *deg) {
    float f = xy[0] * xy[1];

    if (f > 0.0f) {
        f = (xy[0] > 0.0f) ? 0.0f : -3.1415927f;
        f = f + atan(xy[1] / xy[0]);
    } else if (f < 0.0f) {
        f = (xy[1] > 0.0f) ? 1.5707964f : -1.5707964f;
        f = f - atan(xy[0] / xy[1]);
    } else if (xy[0] != 0.0f) {
        f = (xy[0] > 0.0f) ? 0.0f : 3.1415927f;
    } else if (xy[1] != 0.0f) {
        f = (xy[1] > 0.0f) ? 1.5707964f : -1.5707964f;
    } else {
        return 0;
    }
    *deg = f;
    return 1;
}

void pppKeBornRnd6_Born(pppPObject *pObj, PKeBornRnd6 *pDat, pppSVECTOR *pVert) {
    sceVu0FVECTOR fv0;
    pppPObject *pNewObj;
    VAngle *pAng;
    float f;
    float e;
    float d;
    pppPDataVal *pdv;

    pdv = &ppvMng->m_pdtval[pDat->Pos.pdtn];
    if (pdv == NULL) {
        pNewObj = NULL;
    } else {
        pNewObj = pppCreatePObject(ppvMng, pdv);
        if (pNewObj == NULL) {
            return;
        }
        pNewObj->ppobj = pObj;
    }
    if (pNewObj != NULL) {
        fv0[0] = pVert->x;
        fv0[1] = pVert->y;
        fv0[2] = pVert->z;
        fv0[3] = 1.0f;
        KeVu0CopyVecXYZ((float *)&pNewObj->val[pDat->Pos.valoff], fv0);
        f = fv0[1];
        e = fv0[0];
        pAng = (VAngle *)&pNewObj->val[pDat->Ang.valoff];
        fv0[0] = -f;
        fv0[1] = e;
        if (_KeGetDeg2(fv0, &d)) {
            pAng->ang.z = d * 32768.0f / 3.141592654;
        }
        fv0[0] = KeVu0Sqrt(f * f + e * e);
        fv0[1] = -fv0[2];
        if (_KeGetDeg2(fv0, &d)) {
            pAng->ang.x = d * 32768.0f / 3.141592654;
        }
        pAng->ang.y = 0;
    }
}

void pppKeBornRnd6(pppPObject *pobj, PKeBornRnd6 *dat, pppCtrlTable *ctbl) {
    VKeBornRnd6 *_t;

    if (ppvEmptyLoop != 0) {
        return;
    }
    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    if (dat->Wait == 0) {
        return;
    }
    _t = (VKeBornRnd6 *)&pobj->val[ctbl->useVal[IKeBornRnd6_This]];
    if (_t->Cnt == 0) {
        _t->Cnt = dat->Wait;
        if (dat->Pos.pdtn == 0xFFFFFFFF) {
            return;
        }
        if (dat->Ang.pdtn == 0xFFFFFFFF) {
            return;
        }
        KeBornRnd2(pobj, dat, pppKeBornRnd6_Born, &_t->NextIdx, dat->IdxVsf, dat->Mode, dat->Num);
    }
    _t->Cnt--;
}

void pppKeBornRnd6Con(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeBornRnd6 *_t = (VKeBornRnd6 *)&pobj->val[ctbl->useVal[IKeBornRnd6_This]];

    _t->NextIdx = 0;
    _t->Cnt = 0;
}
