#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLTFD_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLTFD_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    IKeMdlTfd_Color = 0,
    IKeMdlTfd_DMat = 1,
    IKeMdlTfd_This = 2
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
} PKeMdlTfd;

typedef struct { /* 0x10 */
    float Lv;
    float LvSpd;
    float LvAcc;
    unsigned int VAdr;
} VKeMdlTfd;

void pppKeMdlTfdDraw(pppPObject *pobj, PKeMdlTfd *dat, pppCtrlTable *ctbl);
void pppKeMdlTfdCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLTFD_H */
