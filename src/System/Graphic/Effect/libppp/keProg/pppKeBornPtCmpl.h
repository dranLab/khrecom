#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNPTCMPL_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNPTCMPL_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"
#include "System/Graphic/Effect/libppp/eiProg/pppColor.h"
#include "System/Graphic/Effect/libppp/keProg/KeBornRnd.h"

enum {
    IKeBornPtCmpl_This = 0,
    IKeBornPtCmpl_POINT = 1
};

typedef struct { /* 0x18 */
    pppCDT cdt;
    u_char Num;
    char dummy0[3];
    pppParamPObj Pos;
    pppParamPObj Col;
} PKeBornPtCmpl;

typedef struct { /* 0xc */
    VPoint *PrePt;
    VColor *PreCol;
    float Pstg;
} VKeBornPtCmpl;

void KeBornPtCmlp(pppPObject *pObj, PKeBornPtCmpl *pDat, pppCtrlTable *pTbl);
void pppKeBornPtCmpl(pppPObject *pobj, PKeBornPtCmpl *dat, pppCtrlTable *ctbl);
void pppKeBornPtCmplCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNPTCMPL_H */
