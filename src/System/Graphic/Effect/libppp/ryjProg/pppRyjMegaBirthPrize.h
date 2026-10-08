#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHPRIZE_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHPRIZE_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    RYJMEGABIRTHPRIZE_THIS = 0
};

typedef struct { /* 0x88 */
    pppCDT cdt;
    int nShape0;
    pppCVECTOR Col0;
    float fScale0;
    int nShape1;
    pppCVECTOR Col1;
    float fScale1;
    int nShape2;
    pppCVECTOR Col2;
    float fScale2;
    int nShape3;
    pppCVECTOR Col3;
    float fScale3;
    int nShape4;
    pppCVECTOR Col4;
    float fScale4;
    int nShape5;
    pppCVECTOR Col5;
    float fScale5;
    int nShape6;
    pppCVECTOR Col6;
    float fScale6;
    int nShape7;
    pppCVECTOR Col7;
    float fScale7;
    int nShape8;
    pppCVECTOR Col8;
    float fScale8;
    int nShape9;
    pppCVECTOR Col9;
    float fScale9;
    u_int unDelta;
    u_char bFog;
    u_char bNear;
    u_char bSort;
} PRyjMegaBirthPrize;

void pppRyjDrawMegaBirthPrize(pppPObject *pobj, PRyjMegaBirthPrize *param, pppCtrlTable *ctbl);
void pppRyjMegaBirthPrizeCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHPRIZE_H */
