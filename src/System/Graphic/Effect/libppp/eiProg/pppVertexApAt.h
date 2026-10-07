#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPVERTEXAPAT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPVERTEXAPAT_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppVertexAttend.h"

typedef struct { /* 0x14 */
    pppCDT cdt;
    short vsfno;
    u_char fapn;
    u_char faps;
    u_char aptype;
    char dummy0[3];
    pppParamPObj att;
} PVertexApAt;

typedef struct { /* 0x4 */
    u_short ict;
    u_short aptct;
} VVertexApAt;

void pppVertexApAtCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppVertexApAt(pppPObject *pobj, PVertexApAt *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPVERTEXAPAT_H */
