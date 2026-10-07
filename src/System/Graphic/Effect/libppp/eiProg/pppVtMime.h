#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPVTMIME_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPVTMIME_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x18 */
    pppCDT cdt;
    int sta_mdlno;
    int end_mdlno;
    float inp;
    float dinp;
    float ddinp;
} PVtMime;

typedef struct { /* 0x10 */
    float inp;
    float dinp;
    float ddinp;
    pppSVECTOR *lpvt;
} VVtMime;

void pppVtMime(pppPObject *pobj, PVtMime *p, pppCtrlTable *ctbl);
void pppDrawVtMime(pppPObject *pobj, PVtMime *p, pppCtrlTable *ctbl);
void pppVtMimeCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppVtMimeCon2(pppPObject *pobj, pppCtrlTable *ctbl);
void pppVtMimeDes(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPVTMIME_H */
