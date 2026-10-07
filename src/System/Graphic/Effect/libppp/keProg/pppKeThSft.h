#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKETHSFT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKETHSFT_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/keProg/KeTh.h"

enum {
    IKeThSft_This = 0,
    IKeThSft_KeTh = 1
};

typedef struct { /* 0x3c */
    pppCDT cdt;
    pppHCVECTOR ColCe;
    pppHCVECTOR ColEg;
    pppHCVECTOR AddColCe;
    pppHCVECTOR AddColEg;
    float RootWid;
    float Wild;
    float Spd;
    float Corner;
    u_char LowNum;
    u_char LowPstg;
    u_char SttLazer;
    u_char SttQuick;
    u_char SttCpSync;
} PKeThSft;

typedef struct { /* 0x38 */
    pppHCVECTOR ColCe;
    pppHCVECTOR ColEg;
    pppHCVECTOR AddColCe;
    pppHCVECTOR AddColEg;
    float Wid;
    float Wild;
    float Spd;
    float Corner;
    u_char LowNum;
    u_char LowPstg;
    u_char SttLazer;
    u_char SttQuick;
    u_char SttCpSync;
} VKeThSft;

void pppKeThSft(pppPObject *pobj, PKeThSft *dat, pppCtrlTable *ctbl);
void pppKeThSftCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKETHSFT_H */
