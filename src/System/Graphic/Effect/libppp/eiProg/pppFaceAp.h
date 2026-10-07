#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPFACEAP_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPFACEAP_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct {
    pppCDT cdt;
    short vsfno;
    u_char fapn;
    u_char faps;
    u_char aptype;
    char dummy0[3];
    pppParamPObj pos;
} PFaceAp;

typedef struct {
    u_short ict;
    u_short aptct;
} VFaceAp;

void pppFaceAp(pppPObject *pobj, PFaceAp *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPFACEAP_H */
