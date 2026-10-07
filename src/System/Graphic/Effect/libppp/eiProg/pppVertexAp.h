#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPVERTEXAP_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPVERTEXAP_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"

typedef struct { /* 0x14 */
    pppCDT cdt;
    short vsfno;
    u_char fapn;
    u_char faps;
    u_char aptype;
    char dummy0[3];
    pppParamPObj pos;
} PVertexAp;

typedef struct { /* 0x4 */
    u_short ict;
    u_short aptct;
} VVertexAp;

void pppVertexApCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppVertexAp(pppPObject *pobj, PVertexAp *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPVERTEXAP_H */
