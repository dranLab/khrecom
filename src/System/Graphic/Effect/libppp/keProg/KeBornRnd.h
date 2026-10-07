#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KEBORNRND_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KEBORNRND_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct _pppVsfSt { /* 0x8 */
    short modelno;
    short indexn;
    u_short *index;
} pppVsfSt;

typedef void (*KetBornRnd_Born)(pppPObject *pObj, void *pDat, pppSVECTOR *pVert);

int KeVu0RndI(void);

void KeBornRnd2(pppPObject *pObj, void *pDat, void *fnc, u_short *pIdxNxt, short idxVsf, u_char md, u_char num);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KEBORNRND_H */
