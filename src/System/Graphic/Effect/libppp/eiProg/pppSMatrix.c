#include "pppSMatrix.h"

void pppSMatrix(pppPObject *pobj, PSMatrix *p, pppCtrlTable *ctbl) {
    VSMatrix *v;

    v = (VSMatrix *)&pobj->val[ctbl->useVal[0]];
    ppvlpCMatrix = &v->mat;
}
