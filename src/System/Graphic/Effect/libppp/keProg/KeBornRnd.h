#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KEBORNRND_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KEBORNRND_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef void (*KetBornRnd_Born)(pppPObject *pObj, void *pDat, pppSVECTOR *pVert);

int KeVu0RndI(void);

void KeBornRnd2(pppPObject *pObj, void *pDat, void *fnc, u_short *pIdxNxt, short idxVsf, u_char md, u_char num);


void KeVu0CopyVecXYZ(float *d, float *s);
void KeVu0AddVecXYZ(float *d, float *s0, float *s1);
float KeVu0Sqrt(float x);
void KeVu0SubVecXYZ(float *d, float *s0, float *s1);



#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KEBORNRND_H */
