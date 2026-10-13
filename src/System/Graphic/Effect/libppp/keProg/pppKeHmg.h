#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEHMG_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEHMG_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppMove.h"
#include "System/Graphic/Effect/libppp/keProg/Ke.h"

enum {
    IKeHitChk_Point = 0,
    IKeHitChk_Move = 1,
    IKeHitChk_This = 2
};

enum {
    FKeHitChk_SttClr_ = 0,
    FKeHitChk_SttHit = 1
};

typedef u_char TKeHitChk_Stt;

typedef struct {
    pppCDT cdt;
} PKeHitChk;

typedef struct {
    sceVu0FVECTOR HitPos;
    TKeHitChk_Stt Stt;
} VKeHitChk;

enum {
    IKeHitBall_Point = 0,
    IKeHitBall_This = 1
};

typedef struct {
    pppCDT cdt;
    float HitR;
} PKeHitBall;

typedef struct {
    float HitR;
    VPoint *Pt;
} VKeHitBall;

enum {
    IKeHitChkPxB_Point = 0,
    IKeHitChkPxB_Move = 1,
    IKeHitChkPxB_This = 2
};

enum {
    FKeHitChkPxB_SttClr_ = 0,
    FKeHitChkPxB_SttHit = 1,
    FKeHitChkPxB_SttHitErase = 2
};

typedef struct {
    pppCDT cdt;
    pppParamPObj Tgt;
    u_char CmdHitErase;
} PKeHitChkPxB;

typedef struct {
    sceVu0FVECTOR HitPos;
    TKeHitChk_Stt Stt;
    char dummy0[3];
    VKeHitBall *Tgt;
} VKeHitChkPxB;

enum {
    IKeHitBorn_Point = 0,
    IKeHitBorn_KeHitChk = 1,
    IKeHitBorn_This = 2
};

enum {
    FKeHitBorn_SttClr_ = 0,
    FKeHitBorn_SttContinue = 1,
    FKeHitBorn_SttBorned = 2
};

typedef u_char TKeHitBorn_Stt;

typedef struct {
    pppCDT cdt;
    pppParamPObj BornPt;
    u_char CmdContinue;
} PKeHitBorn;

typedef struct {
    TKeHitBorn_Stt Stt;
} VKeHitBorn;

enum {
    IKeGrvTgt_Point = 0,
    IKeGrvTgt_This = 1
};

typedef struct {
    pppCDT cdt;
    float ValidR;
    float Pow;
} PKeGrvTgt;

typedef struct {
    float ValidR;
    float Pow;
    VPoint *Pt;
} VKeGrvTgt;

enum {
    IKeGrvEff_Point = 0,
    IKeGrvEff_Move = 1,
    IKeGrvEff_This = 2
};

typedef struct {
    pppCDT cdt;
    pppParamPObj Tgt;
} PKeGrvEff;

typedef struct {
    VKeGrvTgt *Tgt;
} VKeGrvEff;

enum {
    IKeHmgEff_Point = 0,
    IKeHmgEff_Move = 1,
    IKeHmgEff_This = 2
};

typedef struct {
    pppCDT cdt;
    float Corner;
    pppParamPObj Tgt;
} PKeHmgEff;

typedef struct {
    float Corner;
    VPoint *Tgt;
} VKeHmgEff;

void sceVu0SubVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2);
void sceVu0InterVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2, float t);

void pppKeHmgEff(pppPObject *pobj, PKeHmgEff *dat, pppCtrlTable *ctbl);
void pppKeHmgEffCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeGrvTgt(pppPObject *pobj, PKeGrvTgt *dat, pppCtrlTable *ctbl);
void pppKeGrvTgtCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeGrvEff(pppPObject *pobj, PKeGrvEff *dat, pppCtrlTable *ctbl);
void pppKeGrvEffCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeHitBall(pppPObject *pobj, PKeHitBall *dat, pppCtrlTable *ctbl);
void pppKeHitBallCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeHitBorn(pppPObject *pobj, PKeHitBorn *dat, pppCtrlTable *ctbl);
void pppKeHitBornCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeHitChkPxB(pppPObject *pobj, PKeHitChkPxB *dat, pppCtrlTable *ctbl);
void pppKeHitChkPxBCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEHMG_H */
