#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLTFDUV2_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLTFDUV2_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    IKeMdlTfdUv2_Color = 0,
    IKeMdlTfdUv2_DMat = 1,
    IKeMdlTfdUv2_This = 2
};

typedef struct { /* 0x3C */
    pppCDT cdt;
    int Mdl;
    float Lv;
    float LvSpd;
    float LvAcc;
    pppCVECTOR ColBs;
    pppCVECTOR ColSs;
    unsigned char CltIdx;
    char dummy0[3];
    float tu;
    float dtu;
    float ddtu;
    float tv;
    float dtv;
    float ddtv;
    unsigned char draw_mode;
    unsigned char ztest;
} PKeMdlTfdUv2;

typedef struct { /* 0x28 */
    float Lv;
    float LvSpd;
    float LvAcc;
    unsigned int VAdr;
    float tu;
    float dtu;
    float ddtu;
    float tv;
    float dtv;
    float ddtv;
} VKeMdlTfdUv2;

void pppKeMdlTfdUv2Draw(pppPObject *pobj, PKeMdlTfdUv2 *dat, pppCtrlTable *ctbl);
void pppKeMdlTfdUv2Con(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLTFDUV2_H */
