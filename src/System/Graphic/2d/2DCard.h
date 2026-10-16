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

#endif /* SYSTEM_GRAPHIC_2D_2DCARD_H */
