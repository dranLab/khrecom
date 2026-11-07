#ifndef SYSTEM_GRAPHIC_2D_2DCARD_H
#define SYSTEM_GRAPHIC_2D_2DCARD_H

#include "khtypes.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "Game/CardKind.h"
#include "Game/SceneInfo.h" /* RCNORMA::NormaSrc */

class C2DCard : public D2::C2DSeqCtrl {
public:
    CARD m_card;                     /* 0x160 */
private:
    D2::C2DSeqCtrl* m_pParentCtrl;   /* 0x164 */
    HANDLE m_hSeqHandleBack;         /* 0x168 */
    SHORT m_idxSelect;               /* 0x16c */
    SHORT m_fgBack;                  /* 0x16e */
    static LPCTSTR m_pSeqName;

public:
    C2DCard(INT nType);
    C2DCard();
    virtual ~C2DCard();
    void create(CARD inCard);
    void createNorma(RCNORMA::NormaSrc inNorma, INT inStatus);
    void replace(CARD inCard);
    void replaceNorma(RCNORMA::NormaSrc inNorma, INT inStatus);
    void createEmpty();
    void select(BOOL bUnusable);
    void unselect();
    void replaceNumberNormal(BYTE inNumber);
    void replaceNumberBlue(BYTE inNumber);
    void replaceNumberRoulette();
    void checkBack();
    void setParent(D2::C2DSeqCtrl* p2DSeqCtrl);
    void setRot(FVECTOR& vRot);
    void enableDisp(BOOL bEnable);
    void changeLayer(INT nLayer);
    void changeTime(UINT inTime);

private:
    void createBack();
};


/* MATCH: a by-value NormaSrc reaches a1 as four lhu of a stack copy combined with dsll/or (gcc's unaligned-argument path,
   extract_bit_field in 16-bit pieces). No struct shape or installed compiler gives that from a plain call, so the pieces are
   written out and createNorma/replaceNorma are called with the 64-bit value: the bytes in a1 are the struct's. */
#define _NORMA_PIECES(t) ({ \
    register ULONGLONG lo asm("$2") = (USHORT)(t)._nOp; \
    register ULONGLONG num asm("$7") = (USHORT)(t)._nNumber; \
    ULONGLONG hi = (USHORT)(t)._nColor; \
    lo <<= 16; \
    register ULONGLONG top asm("$3") = (USHORT)(t).__nDummy; \
    lo |= num; \
    hi <<= 32; \
    hi |= lo; \
    top <<= 48; \
    top | hi; })
extern void _createNormaRaw(C2DCard* pThis, ULONGLONG inNorma, INT inStatus) __asm__("createNorma__7C2DCardGQ27RCNORMA8NormaSrci");
extern void _replaceNormaRaw(C2DCard* pThis, ULONGLONG inNorma, INT inStatus) __asm__("replaceNorma__7C2DCardGQ27RCNORMA8NormaSrci");

#endif /* SYSTEM_GRAPHIC_2D_2DCARD_H */
