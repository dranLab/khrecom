#include "pppRyjMegaBirthUserCtrl2.h"
#include "RyjMegaBirthUserCtrl.h"

void pppRyjDrawMegaBirthUserCtrl2(pppPObject *pobj, PRyjMegaBirthUserCtrl2 *param, pppCtrlTable *ctbl) {
    if (ppvReferenceMatrixList != NULL && ppvReferenceMatrixNum > 0) {
        RyjMegaBirthUserCtrlDrawModel(param->model, &param->model_color, param->fog, param->near);
        RyjMegaBirthUserCtrDrawShape(param->shape, &param->shape_color, param->fog, param->near);
    }
}

void pppRyjMegaBirthUserCtrl2Con(pppPObject *pobj, pppCtrlTable *ctbl) {
    ppvMng->bBoolNoViewClip = 1;
    ppvMng->bDrawPriority = 3;
}
