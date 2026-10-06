#include "pppRyjMegaBirthUserCtrl3.h"
#include "RyjMegaBirthUserCtrl.h"

void pppRyjDrawMegaBirthUserCtrl3(pppPObject *pobj, PRyjMegaBirthUserCtrl3 *param, pppCtrlTable *ctbl) {
    if (ppvReferenceMatrixList != NULL && ppvReferenceMatrixNum > 0) {
        RyjMegaBirthUserCtrDrawShapeLine(param->shape, &param->shape_color, param->fog, param->near, param->scale, param->length, param->num);
    }
}

void pppRyjMegaBirthUserCtrl3Con(pppPObject *pobj, pppCtrlTable *ctbl) {
    ppvMng->bBoolNoViewClip = 1;
    ppvMng->bDrawPriority = 3;
}
