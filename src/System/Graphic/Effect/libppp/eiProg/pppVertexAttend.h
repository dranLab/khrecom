#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPVERTEXATTEND_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPVERTEXATTEND_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    pppParamPObj mat;
    short vsfno;
} PVertexAttend;

typedef struct { /* 0x2 */
    u_short indexno;
} VVertexAttend;

void pppVertexAttend(pppPObject *pobj, PVertexAttend *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPVERTEXATTEND_H */
