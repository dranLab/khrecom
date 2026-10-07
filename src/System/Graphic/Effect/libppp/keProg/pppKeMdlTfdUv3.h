#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLTFDUV3_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLTFDUV3_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    IKeMdlTfdUv3_Color = 0,
    IKeMdlTfdUv3_DMat = 1,
    IKeMdlTfdUv3_This = 2,
    IKeMdlTfdUv3_KeZCrct = 3
};

typedef struct { /* 0x40 */
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
    unsigned char fog;
    unsigned char scissor;
    unsigned char near;
    unsigned char zCrct;
    unsigned char ztest;
} PKeMdlTfdUv3;

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
} VKeMdlTfdUv3;

void pppKeMdlTfdUv3Draw(pppPObject *pobj, PKeMdlTfdUv3 *dat, pppCtrlTable *ctbl);
void pppKeMdlTfdUv3Con(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLTFDUV3_H */
