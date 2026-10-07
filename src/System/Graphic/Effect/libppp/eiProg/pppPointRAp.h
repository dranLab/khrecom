#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPPOINTRAP_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPPOINTRAP_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"
#include "System/Graphic/Effect/libppp/eiProg/pppMove.h"

typedef struct { /* 0x20 */
    pppCDT cdt;
    float apr;
    float speedr;
    pppParamPObj pos;
    pppParamPObj mov;
    unsigned char faps;
} PPointRAp;

typedef struct { /* 0x2 */
    unsigned char faps;
    unsigned char aptct;
} VPointRAp;

void pppPointRApCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppPointRAp(pppPObject *pobj, PPointRAp *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPPOINTRAP_H */
