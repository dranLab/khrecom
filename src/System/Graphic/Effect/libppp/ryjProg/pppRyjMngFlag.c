#include "pppRyjMngFlag.h"

void pppRyjMngFlag(pppPObject *pPobj, PRyjMngFlag *pParam, pppCtrlTable *pCtbl) {
    if (pParam->cdt.time == pPobj->time) {
        ppvMng->bBoolFollowCam = pParam->bFollowCam;
        ppvMng->bFlagStop = pParam->bStop;
        ppvMng->bDrawPriority = pParam->bDrawPriority;
    }
}
