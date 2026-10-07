#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLTFD2_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLTFD2_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    IKeMdlTfd2_Color = 0,
    IKeMdlTfd2_DMat = 1,
    IKeMdlTfd2_This = 2
};

typedef struct { /* 0x20 */
    pppCDT cdt;
    int Mdl;
    float Lv;
    float LvSpd;
    float LvAcc;
    pppCVECTOR ColBs;
    pppCVECTOR ColSs;
    unsigned char CltIdx;
    unsigned char draw_mode;
    unsigned char ztest;
} PKeMdlTfd2;

typedef struct { /* 0x10 */
    float Lv;
    float LvSpd;
    float LvAcc;
    unsigned int VAdr;
} VKeMdlTfd2;

void pppKeMdlTfd2Draw(pppPObject *pobj, PKeMdlTfd2 *dat, pppCtrlTable *ctbl);
void pppKeMdlTfd2Con(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLTFD2_H */
