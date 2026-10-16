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
private:
    static CEntityFactory* m_pInstance; // PARTIAL: added for Game/EntityFactory
public:
    CEntityFactory();
    virtual ~CEntityFactory();
    virtual void main(); // PARTIAL: added for Game/EntityFactory
    CEntity* createBullet(const BULLET_PARAM& inParam, CTask* inParent);
    static CEntityFactory* getInstance();                                // PARTIAL: added for Game/Player/FriendAladdin
    CEntity* createPrize(const PRIZE_PARAM& inParam, CTask* inParent); // PARTIAL: added for Game/Player/FriendAladdin
    CEntity* createMapEnemy(const OBJ_INFO& inInfo, CTask* inParent, void* inExtraInfo); // PARTIAL: added for Game/Enemy/MapEnemyManager
    CEntity* createEntity(INT inKind, INT inID, CTask* inParent, void* inExtraInfo); // PARTIAL: added for Game/Gimmick/GimmickDeceive
    CEntity* createEntity(const OBJ_INFO& inInfo, CTask* inParent, void* inExtraInfo); // PARTIAL: added for Game/Gimmick/GimmickManager
    // PARTIAL: added for Game/EntityFactory
    CEntity* createEntity(INT inGroupID, INT inRsrcID, const FVECTOR& inPos, FLOAT inDir, CTask* inParent, void* inExtraInfo);
    CEntity* createEntity(INT inGroupID, INT inRsrcID, const FVECTOR& inPos, const FVECTOR& inRot, CTask* inParent, void* inExtraInfo);
    CEntity* createEnemy(INT inRsrcID, CTask* inParent, void* inExtraInfo);
    CEntity* createGimmick(INT inRsrcID, CTask* inParent, void* inExtraInfo); // PARTIAL: added for Game/Gimmick/CreateGimmick
};

void LoadMapEnemy(); // PARTIAL: added for Game/EntityFactory
void LoadBattleEnemy();

#endif /* GAME_ENTITYFACTORY_H */
