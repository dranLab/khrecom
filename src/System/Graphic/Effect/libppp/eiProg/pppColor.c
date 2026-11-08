#include "pppColor.h"

/* MATCH: one asm per VU0/MMI instruction; the loads, the or and the store are C. max is read back as an int
   from its stack slot (`*(int *)&max`: the swc1/lw pair), and the empty asm with its "m" operand keeps the
   swc1 before the colour loads (without it: 7 lines). pCol is read before the sequence (pMng->parCol in the
   lqc2 operand: 7 lines). The empty asm after the store clobbers v0, v1 and a0, the registers the sequence
   used: local-alloc then keeps them out of the ppvMonochrome %hi that follows (lui a1; without it lui v0 /
   lw v1, 3 lines; "$2", "$3" only: 2 lines). No register locals needed; the qmfc2..ppach temps are u_long128
   (the packs read all four words), c is u_long (a | on u_long128 is unsupported in gcc 2.95) */
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
    {
        void *pCol = pMng->parCol;
        u_long c, l;
        u_long128 q;
        int r;

        __asm__ __volatile__("" : : "m"(max));
        c = *(int *)&v->col.b;
        __asm__ __volatile__("pextlw %0, %1, $0" : "=r"(c) : "r"(c));
        l = *(int *)&v->col.r;
        c |= l;
        __asm__ __volatile__("pextlh %0, $0, %1" : "=r"(q) : "r"(c));
        __asm__ __volatile__("qmtc2.ni %0, $vf1" : : "r"(q));
        r = *(int *)&max;
        __asm__ __volatile__("vitof0.xyzw $vf1, $vf1");
        __asm__ __volatile__("lqc2 $vf2, 0(%0)" : : "r"(pCol) : "memory");
        __asm__ __volatile__("vmul.xyzw $vf1, $vf1, $vf2");
        __asm__ __volatile__("qmtc2.ni %0, $vf2" : : "r"(r));
        __asm__ __volatile__("vminix.xyzw $vf1, $vf1, $vf2x");
        __asm__ __volatile__("vftoi0.xyzw $vf1, $vf1");
        __asm__ __volatile__("qmfc2.ni %0, $vf1" : "=r"(q));
        __asm__ __volatile__("psrlw %0, %1, 7" : "=r"(q) : "r"(q));
        __asm__ __volatile__("ppach %0, $0, %1" : "=r"(q) : "r"(q));
        __asm__ __volatile__("ppacb %0, $0, %1" : "=r"(r) : "r"(q));
        *(int *)&v->release_col = r;
        __asm__ __volatile__("" : : : "$2", "$3", "$4");
    }

    if (ppvMonochrome) {
        y = v->release_col.r * 0.3f + v->release_col.g * 0.59f + v->release_col.b * 0.11f;
        v->release_col.r = v->release_col.g = v->release_col.b = (int)y;
    }
}

void pppColorCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VColor *v = (VColor *)&pobj->val[ctbl->useVal[0]];

    v->col.r = v->col.g = v->col.b = v->col.a = 0;
}
