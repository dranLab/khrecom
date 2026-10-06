#include "pppKeMatSN.h"

void pppKeMatSN(pppPObject *pobj, PKeMatSN *dat, pppCtrlTable *ctbl) {
    VKeMatSN *pThis;
    VAngle *pAng;
    pppIVECTOR iv0;

    pThis = (VKeMatSN *)&pobj->val[ctbl->useVal[0]];
    pAng = (VAngle *)&pobj->val[ctbl->useVal[1]];
    /* stored as int: keeps the pAng loads behind this store (scheduling) */
    *(int *)&ppvlpCMatrix = (int)&pThis->mat;
    iv0 = pThis->PreRot;
    pThis->PreRot.x = pAng->ang.x;
    pThis->PreRot.y = pAng->ang.y;
    pThis->PreRot.z = pAng->ang.z;
    iv0.x -= pThis->PreRot.x;
    iv0.y -= pThis->PreRot.y;
    iv0.z -= pThis->PreRot.z;
    pppGetRotMatrixXYZ(pThis->Nega, &iv0);
    *(u_long128 *)pThis->Nega[3] = *(u_long128 *)KeVec0001;
}

void pppKeMatSNCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeMatSN *pThis;

    pThis = (VKeMatSN *)&pobj->val[ctbl->useVal[0]];
    pThis->PreRot.x = 0;
    pThis->PreRot.y = 0;
    pThis->PreRot.z = 0;
}
