#include "pppDrawHook.h"

void pppDrawHook(pppPObject *pobj, PDrawHook *p, pppCtrlTable *ctbl) {
    VDrawHook *v;
    PPPUSRFUNC func;

    v = (VDrawHook *)&pobj->val[ctbl->useVal[0]];
    /* pointer-typed store: keeps the ppvHookFuncTbl load after it (aliasing) */
    *(PPPUSRFUNC *)&v->param = *(PPPUSRFUNC *)&p->param;

    if (ppvHookFuncTbl != NULL) {
        func = ppvHookFuncTbl[pobj->pdtval->pdtno];
        if (func != NULL) {
            func(pobj, p->param);
        }
    }
}
