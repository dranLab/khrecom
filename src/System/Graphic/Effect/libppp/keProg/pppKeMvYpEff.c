#include "pppKeMvYpEff.h"
#include <math.h>

int __KeGetDeg2(float *xy, float *deg) {
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

void pppKeMvYpEff(pppPObject *pobj, PKeMvYpEff *dat, pppCtrlTable *ctbl) {
    VMove *pMv;
    VAngle *pAng;
    float val[2];
    float f;
    float e;
    float d;

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    pMv = (VMove *)&pobj->val[ctbl->useVal[IKeMvYpEff_Move]];
    f = pMv->vec.y;
    e = pMv->vec.x;
    if (f == 0.0f && e == 0.0f && pMv->vec.z == 0.0f) {
        return;
    }
    pAng = (VAngle *)&pobj->val[ctbl->useVal[IKeMvYpEff_Angle]];
    val[0] = -f;
    val[1] = e;
    if (__KeGetDeg2(val, &d)) {
        pAng->ang.z = d * 32768.0f / 3.141592654;
    }
    val[0] = KeVu0Sqrt(f * f + e * e);
    val[1] = -pMv->vec.z;
    if (__KeGetDeg2(val, &d)) {
        pAng->ang.x = d * 32768.0f / 3.141592654;
    }
}
