#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLTFD3_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLTFD3_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    IKeMdlTfd3_Color = 0,
    IKeMdlTfd3_DMat = 1,
    IKeMdlTfd3_This = 2,
    IKeMdlTfd3_KeZCrct = 3
};

typedef struct { /* 0x24 */
    pppCDT cdt;
    int Mdl;
    float Lv;
    float LvSpd;
    float LvAcc;
    pppCVECTOR ColBs;
    pppCVECTOR ColSs;
    unsigned char CltIdx;
    unsigned char draw_mode;
    unsigned char fog;
    unsigned char scissor;
    unsigned char near;
    unsigned char zCrct;
    unsigned char ztest;
} PKeMdlTfd3;

typedef struct { /* 0x10 */
    float Lv;
    float LvSpd;
    float LvAcc;
    unsigned int VAdr;
} VKeMdlTfd3;

void pppKeMdlTfd3Draw(pppPObject *pobj, PKeMdlTfd3 *dat, pppCtrlTable *ctbl);
void pppKeMdlTfd3Con(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLTFD3_H */
