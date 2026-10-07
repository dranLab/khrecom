#include "pppEiWfacc.h"

void pppEiWfacc(pppPObject *pobj, PEiWfacc *p, pppCtrlTable *ctbl) {
    VPoint *vp;
    VEiWindFun *ewf;
    VPoint *ep;
    sceVu0FVECTOR v0;
    float wip;
    float wlen;
    VMove *vm;
    float wp;

    vp = (VPoint *)&pobj->val[ctbl->useVal[0]];
    if (p->fun.pdtn != 0xFFFFFFFF) {
        ewf = (VEiWindFun *)&ppvMng->m_pdtval[p->fun.pdtn].pobj->val[p->fun.valoff];
    } else {
        ewf = (VEiWindFun *)ppvDbgTemp;
    }
    if (p->pos.pdtn != 0xFFFFFFFF) {
        ep = (VPoint *)&ppvMng->m_pdtval[p->pos.pdtn].pobj->val[p->pos.valoff];
    } else {
        ep = (VPoint *)ppvDbgTemp;
    }

    v0[0] = vp->pos.x - ep->pos.x;
    v0[1] = vp->pos.y - ep->pos.y;
    v0[2] = vp->pos.z - ep->pos.z;
    __asm__ __volatile__("lqc2 $vf10, 0(%1)\n"
                         "\tvadd.xyz $vf8, $vf0, $vf10\n"
                         "\tvmul.xyz $vf10, $vf10, $vf10\n"
                         "\tvmove.xyz $vf9, $vf10\n"
                         "\tvaddx.y $vf9, $vf9, $vf9x\n"
                         "\tvaddy.z $vf9, $vf9, $vf9y\n"
                         "\tvsqrt $Q, $vf9z\n"
                         "\tvwaitq\n"
                         "\tvaddq.x $vf10, $vf0, $Q\n"
                         "\tqmfc2.ni %0, $vf10\n"
                         "\tvdiv $Q, $vf0w, $vf10x\n"
                         "\tvwaitq\n"
                         "\tvmulq.xyz $vf9, $vf8, $Q\n"
                         "\tsqc2 $vf9, 0(%1)"
                         : "=r"(wlen) : "r"(v0) : "memory");

    if (wlen < ewf->len) {
        wip = sceVu0InnerProduct((float *)&ewf->vec, v0);
        if (ewf->cosa < wip) {
            vm = (VMove *)&pobj->val[ctbl->useVal[1]];
            wp = ewf->wv * ((wip - ewf->cosa) / (1.0f - ewf->cosa)) * ((ewf->len - wlen) / ewf->len);
            wp /= p->mass;
            vm->vec.x += v0[0] * wp;
            vm->vec.y += v0[1] * wp;
            vm->vec.z += v0[2] * wp;
        }
    }
}
