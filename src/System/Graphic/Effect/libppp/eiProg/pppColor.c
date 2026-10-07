#include "pppColor.h"

void pppColor(pppPObject *pobj, PColor *p, pppCtrlTable *ctbl) {
    VColor *v = (VColor *)&pobj->val[ctbl->useVal[0]];
    pppMngSt *pMng;
    float max;
    float y;

    if (ppvUserStopPartF == 0 && !ppvMng->bFlagStop && !ppvMng->bBoolSlow && p->cdt.time == pobj->time) {
        v->col.r += p->col.r;
        v->col.g += p->col.g;
        v->col.b += p->col.b;
        v->col.a += p->col.a;
    }

    pMng = ppvMng;
    max = 32767.0f;
    __asm__ __volatile__("lw $2, 4(%0)\n"
                         "\tpextlw $2, $2, $0\n"
                         "\tlw $3, 0(%0)\n"
                         "\tor $2, $2, $3\n"
                         "\tpextlh $2, $0, $2\n"
                         "\tqmtc2.ni $2, $vf1\n"
                         "\tlw $2, %2\n"
                         "\tvitof0.xyzw $vf1, $vf1\n"
                         "\tlqc2 $vf2, 0(%1)\n"
                         "\tvmul.xyzw $vf1, $vf1, $vf2\n"
                         "\tqmtc2.ni $2, $vf2\n"
                         "\tvminix.xyzw $vf1, $vf1, $vf2x\n"
                         "\tvftoi0.xyzw $vf1, $vf1\n"
                         "\tqmfc2.ni $2, $vf1\n"
                         "\tpsrlw $2, $2, 7\n"
                         "\tppach $2, $0, $2\n"
                         "\tppacb $2, $0, $2\n"
                         "\tsw $2, 8(%0)"
                         : : "r"(v), "r"(pMng->parCol), "m"(max) : "$2", "$3", "memory");

    if (ppvMonochrome) {
        y = v->release_col.r * 0.3f + v->release_col.g * 0.59f + v->release_col.b * 0.11f;
        v->release_col.r = v->release_col.g = v->release_col.b = (int)y;
    }
}

void pppColorCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VColor *v = (VColor *)&pobj->val[ctbl->useVal[0]];

    v->col.r = v->col.g = v->col.b = v->col.a = 0;
}
