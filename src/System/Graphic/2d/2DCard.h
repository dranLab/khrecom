// PARTIAL: added for Game/Player/NaviMap
#ifndef SYSTEM_GRAPHIC_2D_2DCARD_H
#define SYSTEM_GRAPHIC_2D_2DCARD_H

#include "khtypes.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "Game/CardKind.h"
#include "Game/SceneInfo.h" /* PARTIAL: RCNORMA::NormaSrc, for Form/Menu/RoomCreation */

class C2DCard : public D2::C2DSeqCtrl {
public:
    CARD m_card;                     /* 0x160 */
private:
    D2::C2DSeqCtrl* m_pParentCtrl;   /* 0x164 */
    HANDLE m_hSeqHandleBack;         /* 0x168 */
    SHORT m_idxSelect;               /* 0x16c */
    SHORT m_fgBack;                  /* 0x16e */

public:
    C2DCard();
    virtual ~C2DCard();
    void create(CARD card);
    void checkBack();
    void setParent(D2::C2DSeqCtrl* pParent);
    void enableDisp(BOOL bEnable);
    void changeLayer(INT nLayer); /* PARTIAL: added for Form/Menu/Roulette */
    void replace(CARD card);      /* PARTIAL: added for Form/Menu/Roulette */
    void createNorma(RCNORMA::NormaSrc src, INT n);  /* PARTIAL: added for Form/Menu/RoomCreation */
    void replaceNorma(RCNORMA::NormaSrc src, INT n); /* PARTIAL: added for Form/Menu/RoomCreation */
};

#endif /* SYSTEM_GRAPHIC_2D_2DCARD_H */
