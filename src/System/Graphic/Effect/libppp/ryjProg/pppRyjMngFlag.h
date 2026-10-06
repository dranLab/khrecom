#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMNGFLAG_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMNGFLAG_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x8 */
    pppCDT cdt;
    unsigned char bFollowCam;
    unsigned char bStop;
    unsigned char bDrawPriority;
} PRyjMngFlag;

void pppRyjMngFlag(pppPObject *pPobj, PRyjMngFlag *pParam, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMNGFLAG_H */
