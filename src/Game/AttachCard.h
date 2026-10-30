// PARTIAL: added for Form/Menu/RoomCreation
#ifndef GAME_ATTACHCARD_H
#define GAME_ATTACHCARD_H

#include "khtypes.h"
#include "Game/Entity.h"

#include "Game/CardKindEnum.h"



class CAttachCard : public CEntity {
private:
    CARD_KIND m_CardKind;     /* 0x1d0 */
    MODELDATA* m_pCardModel;  /* 0x1d4 */

public:
    CAttachCard(CTask* pParent);
    virtual ~CAttachCard();
    BOOL setup(CARD_KIND kind, CCoord* pParent, const FVECTOR* pPos, const FVECTOR* pRot, FLOAT fScale);

private:
    BOOL initCard_();
};

#endif /* GAME_ATTACHCARD_H */
