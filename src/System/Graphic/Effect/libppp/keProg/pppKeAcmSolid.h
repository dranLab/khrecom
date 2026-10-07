#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEACMSOLID_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEACMSOLID_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    IKeAcmSolid_This = 0
};

typedef struct {
    pppCDT cdt;
} PKeAcmSolid;

typedef struct {
} VKeAcmSolid;

void pppKeAcmSolidEnd(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEACMSOLID_H */
