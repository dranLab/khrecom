// PARTIAL: added for Game/Bullet/CreateBullet
#ifndef GAME_ENTITYFACTORY_H
#define GAME_ENTITYFACTORY_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Vector.h" // PARTIAL: added for Game/Player/FriendAladdin

class CEntity;
struct BULLET_PARAM;
struct OBJ_INFO; // PARTIAL: added for Game/Enemy/MapEnemyManager

// PARTIAL: added for Game/Player/FriendAladdin (original: Game/Prize.h)
struct PRIZE_PARAM {
    FVECTOR vPos;
    FVECTOR vRot;
    INT PrizeKind; /* CARD_KIND */
    INT iNumber;
    FLOAT fSpeed;
    USHORT uiFlag;
    USHORT uiPrizeFlag;
};

class CEntityFactory : public CTask {
public:
    CEntityFactory();
    virtual ~CEntityFactory();
    CEntity* createBullet(const BULLET_PARAM& inParam, CTask* inParent);
    static CEntityFactory* getInstance();                                // PARTIAL: added for Game/Player/FriendAladdin
    CEntity* createPrize(const PRIZE_PARAM& inParam, CTask* inParent); // PARTIAL: added for Game/Player/FriendAladdin
    CEntity* createMapEnemy(const OBJ_INFO& inInfo, CTask* inParent, void* inExtraInfo); // PARTIAL: added for Game/Enemy/MapEnemyManager
    CEntity* createEntity(INT inKind, INT inID, CTask* inParent, void* inExtraInfo); // PARTIAL: added for Game/Gimmick/GimmickDeceive
};

#endif /* GAME_ENTITYFACTORY_H */
