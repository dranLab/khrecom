#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_SNOPROG_PPPSNOMDLDTTNORM_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_SNOPROG_PPPSNOMDLDTTNORM_H

#include "System/Graphic/Effect/libppp/partMng.h"

typedef struct { /* 0x20 */
    pppCDT cdt;
    int Mdl;
    float DttX;
    float SpdX;
    float AccX;
    unsigned char Page;
    unsigned char Mode;
    unsigned char Normal;
    unsigned char ztest;
    float zoffset;
    unsigned char m_acalc;
} PSnoMdlDttNorm;

typedef struct { /* 0xc */
    float DttX;
    float SpdX;
    float AccX;
} VSnoMdlDttNorm;

void pppSnoMdlDttNormCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppSnoMdlDttNormCon2(pppPObject *pobj, pppCtrlTable *ctbl);
void pppSnoMdlDttNormDraw(pppPObject *pobj, PSnoMdlDttNorm *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_SNOPROG_PPPSNOMDLDTTNORM_H */
