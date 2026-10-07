#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKETHHITBORN_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKETHHITBORN_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/keProg/KeTh.h"
#include "System/Graphic/Effect/libppp/keProg/KeBornRnd.h"

enum {
    IKeThHitBorn_KeTh = 0,
    IKeThHitBorn_This = 1
};

enum {
    FKeThHitBorn_SttClr_ = 0,
    FKeThHitBorn_SttContinue = 2
};

typedef struct {
    pppCDT cdt;
    pppParamPObj BornPt;
    u_char CmdContinue;
} PKeThHitBorn;

typedef struct {
    u_char Stt;
} VKeThHitBorn;

void pppKeThHitBorn(pppPObject *pobj, PKeThHitBorn *dat, pppCtrlTable *ctbl);
void pppKeThHitBornCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKETHHITBORN_H */
