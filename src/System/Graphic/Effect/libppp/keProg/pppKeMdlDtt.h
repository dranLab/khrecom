#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLDTT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLDTT_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    IKeMdlDtt_This = 0,
    IKeMdlDtt_Color = 1,
    IKeMdlDtt_KeDMat = 2
};

typedef struct { /* 0x18 */
    pppCDT cdt;
    int Mdl;
    float DttX;
    float SpdX;
    float AccX;
    unsigned char Page;
    unsigned char Mode;
} PKeMdlDtt;

typedef struct { /* 0xc */
    float DttX;
    float SpdX;
    float AccX;
} VKeMdlDtt;

void pppKeMdlDttDraw(pppPObject *pobj, PKeMdlDtt *dat, pppCtrlTable *ctbl);
void pppKeMdlDttCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeMdlDttCon2(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLDTT_H */
