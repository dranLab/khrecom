#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KE_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KE_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct _KeaVu0ItrsBallLine { /* 0x30 */
    sceVu0FVECTOR LinePos;
    sceVu0FVECTOR LineDist;
    float BallR;
    float T;
} KeaVu0ItrsBallLine;

typedef struct _KeIdx { /* 0x8 */
    u_short *Wk;
    u_short Num;
    u_short Free;
} KeIdx;

extern int KeRnd_Value;
extern signed char KeCltSft[32];
extern signed char KeCltOfs[32];
extern sceVu0FVECTOR KeVec0001;
extern sceVu0FVECTOR KeVec1001;
extern sceVu0FVECTOR KeVec0101;
extern sceVu0FVECTOR KeVec0011;
extern sceVu0FMATRIX KeTestMat;

int KeGetDeg2(float *xy, float *deg);
int KeRnd();
int KeVu0RndI();
int KeVu0ItrsBallLine(KeaVu0ItrsBallLine *_a);
float KeVu0LowClamp(float s, float low);
void KeVu0CopyVecXY(float *d, float *s);
void KeVu0CopyVecXYZ(float *d, float *s);
void KeVu0ScaleVecXYZ(float *d, float *s, float v);
void KeVu0ScaleVecXY(float *d, float *s, float v);
void KeVu0AddVecXY(float *d, float *t, float *s);
void KeVu0SubVecXYZ(float *d, float *t, float *s);
void KeVu0AddVecXYZ(float *d, float *t, float *s);
void KeVu0SubVecXY(float *d, float *t, float *s);
void KeVu0PacAddVecXY(int *d, float *t, float *s);
void KeVu0PacVec(int *d, float *s);
void KeVu0NormXY(float *d, float *s);
void KeVu0RTPersF(float *d, float *m[4], float *s);
void KeVu0PacPers(int *dst, float *src);
float KePow(float v, int n);
float KeVu0Sqrt(float v);
float KeVu0VecLen(float *v);
float KeVu0VecLenXY(float *v);
float KeVu0GetDist(float *b, float *t);
u_short KeIdx_Alloc(KeIdx *_p);
void KeIdx_Init(KeIdx *_p, u_short *wk, u_short num);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KE_H */
