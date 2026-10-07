#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLTFDUV_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLTFDUV_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    IKeMdlTfdUv_Color = 0,
    IKeMdlTfdUv_DMat = 1,
    IKeMdlTfdUv_This = 2
};

typedef struct { /* 0x38 */
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
} PKeMdlTfdUv;

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
} VKeMdlTfdUv;

void pppKeMdlTfdUvDraw(pppPObject *pobj, PKeMdlTfdUv *dat, pppCtrlTable *ctbl);
void pppKeMdlTfdUvCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLTFDUV_H */
