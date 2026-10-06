#include "pppRyjMegaBirthUserCtrl.h"
#include "RyjMegaBirthUserCtrl.h"

void pppRyjDrawMegaBirthUserCtrl(pppPObject *pobj, PRyjMegaBirthUserCtrl *param, pppCtrlTable *ctbl) {
    if (ppvReferencePosList != NULL && ppvReferencePosNum > 0) {
        RyjMegaBirthUserCtrDrawShapeRefPos(param->shape, &param->shape_color, param->fog, param->near);
    }
}

void pppRyjMegaBirthUserCtrlCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    ppvMng->bBoolNoViewClip = 1;
    ppvMng->bDrawPriority = 3;
}
