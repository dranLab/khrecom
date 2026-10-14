#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWKEKOTO_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWKEKOTO_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    RYJDRAWKEKOTO_COLOR = 0,
    RYJDRAWKEKOTO_DRAWMATRIX = 1,
    RYJDRAWKEKOTO_THIS = 2
};

typedef struct { /* 0x34 */
    pppCDT cdt;
    int nModel;
    float fLength;
    float fWidth;
    float fColSclA_R;
    float fColSclA_G;
    float fColSclA_B;
    float fColSclA_A;
    float fColSclB_R;
    float fColSclB_G;
    float fColSclB_B;
    float fColSclB_A;
    unsigned char bPers;
    unsigned char bTexS;
    unsigned char bTexT;
    unsigned char bTexX;
} PRyjDrawKekoto;

typedef struct { /* 0x80 */
    sceVu0FVECTOR vecStq[8];
} VRyjDrawKekoto;

void pppRyjDrawKekoto(pppPObject *pPobj, PRyjDrawKekoto *pParam, pppCtrlTable *pCtbl);
void pppRyjDrawDrawKekoto(pppPObject *pPobj, PRyjDrawKekoto *pParam, pppCtrlTable *pCtbl);
void pppRyjDrawKekotoCon(pppPObject *pPobj, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWKEKOTO_H */
