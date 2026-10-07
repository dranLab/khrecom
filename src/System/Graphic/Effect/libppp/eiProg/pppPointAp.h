#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPPOINTAP_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPPOINTAP_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    pppParamPObj pos;
    unsigned char faps;
    unsigned char apsu;
} PPointAp;

typedef struct { /* 0x2 */
    unsigned char faps;
    unsigned char aptct;
} VPointAp;

void pppPointApCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppPointAp(pppPObject *pobj, PPointAp *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPPOINTAP_H */
