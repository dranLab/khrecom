#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKETH_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKETH_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/keProg/KeTh.h"
#include "System/Graphic/Effect/libppp/keProg/pppKeDrct.h"

enum {
    IKeTh_This = 0,
    IKeTh_Res = 1,
    IKeTh_KeDrct = 2
};

typedef struct { /* 0x5c */
    pppCDT cdt;
    pppParamPObj Tp;
    pppHCVECTOR ColCe;
    pppHCVECTOR ColEg;
    pppHCVECTOR AddColCe;
    pppHCVECTOR AddColEg;
    float MaxLen;
    float RootWid;
    float TopWid;
    float CntrWid;
    float MvWidSz;
    float MvWidMv;
    float Wild;
    float Spd;
    float Corner;
    u_short Curv;
    u_char LowNum;
    u_char LowPstg;
    u_char Alpha;
    u_char SttLazer;
    u_char SttQuick;
    u_char SttChange;
    u_char SttCpSync;
} PKeTh;

enum {
    IKeThTp_This = 0,
    IKeThTp_KeMat = 1
};

typedef struct { /* 0x18 */
    pppCDT cdt;
    float HitR;
    float ValidR;
    float Power;
    pppParamPObj Next;
} PKeThTp;

typedef struct { /* 0x20 */
    KeThTP Tp;
} VKeThTp;

typedef struct { /* 0x18 */
    pppCDT cdt;
    float HitR;
    float ValidR;
    float Power;
    pppParamPObj Next;
} PKeThTp2;

typedef struct { /* 0x20 */
    KeThTP Tp;
} VKeThTp2;

typedef struct {
    pppCDT cdt;
} PKeThRes;

typedef struct {
    KeThResHd Hd;
} VKeThRes;

void pppKeTh(pppPObject *pobj, PKeTh *dat, pppCtrlTable *ctbl);
void pppKeThDraw(pppPObject *pobj, PKeTh *dat, pppCtrlTable *ctbl);
void pppKeThCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThCon2(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThDes(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThTp(pppPObject *pobj, PKeThTp *dat, pppCtrlTable *ctbl);
void pppKeThTpCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThTp2Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThResCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes8Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes8x4Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes8x128Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes16Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes16x4Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes16x64Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes24Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes24x4Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes32Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes32x4Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes32x32Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes40Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes40x4Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes48Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes48x4Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes64Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes64x4Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes64x16Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes128Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes128x4Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes128x8Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes255Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeThRes255x4Con(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKETH_H */
