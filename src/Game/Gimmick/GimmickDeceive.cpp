#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "Game/Gimmick/GimmickDeceive.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in GimmickRideDest); which
 * headers they come from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() {
    TRACE("");
}
inline void _emptyStringPlaceholder2() {
    TRACE("");
}

#include "System/Graphic/Model.h"
#include "Game/Movement.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/Gimmick/GimmickDeceiveEnemy.h"
#include "Game/LockOnManager.h"
#include "Game/EntityFactory.h"
#include "Game/Player/Player.h"

struct DropPrize {
    INT StockKind;
    INT Kind; /* CARD_KIND */
    SHORT Num;
    SHORT No;
};

CGimmickDeceive::CGimmickDeceive(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_Stat = 0;
}

CGimmickDeceive::~CGimmickDeceive() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickDeceive::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    bodyCollOffBody(-1);
    rideModelEnableAllCollision(FALSE);
    return TRUE;
}

void CGimmickDeceive::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    if (!m_Stat) {
        bodyCollOnBody(-1);
        rideModelEnableAllCollision(TRUE);
    }
    if (getParam() == 5) rideModelChangeListR2();
}

void CGimmickDeceive::beginStateMain() {
    if (m_Stat == 1) {
        saveStat(1);
        out();
    }
}

void CGimmickDeceive::stateMain() {}

void CGimmickDeceive::endStateMain() {}

void CGimmickDeceive::beginStateHitAttack() {
    DropPrize prize;
    prize.StockKind = 0;
    prize.Kind = 0;
    prize.Num = 0;
    prize.No = 0;
    if (m_Stat) return;
    if (createPrize(0, 0) == 4) {
        if (calcDistance(g_pPlayer) <= 8.0f) {
            CEntity* entity;
            switch (getID() % 10000U) {
            case 3:
                entity = CEntityFactory::getInstance()->createEntity(11, 4, getSysTask(17), NULL);
                break;
            case 0x14a:
                entity = CEntityFactory::getInstance()->createEntity(11, 0x14b, getSysTask(17), NULL);
                break;
            case 0xe6:
                entity = CEntityFactory::getInstance()->createEntity(11, 0xe7, getSysTask(17), NULL);
                break;
            case 0x82:
                entity = CEntityFactory::getInstance()->createEntity(11, 0x83, getSysTask(17), NULL);
                break;
            case 0x33e:
                entity = CEntityFactory::getInstance()->createEntity(11, 0x33f, getSysTask(17), NULL);
                break;
            case 0x35e:
                entity = CEntityFactory::getInstance()->createEntity(11, 0x362, getSysTask(17), NULL);
                break;
            case 0x212:
                entity = CEntityFactory::getInstance()->createEntity(11, 0x213, getSysTask(17), NULL);
                break;
            case 0x46a:
                entity = CEntityFactory::getInstance()->createEntity(11, 0x46b, getSysTask(17), NULL);
                break;
            case 0x48a:
                entity = CEntityFactory::getInstance()->createEntity(11, 0x48b, getSysTask(17), NULL);
                break;
            case 0x4ce:
                entity = CEntityFactory::getInstance()->createEntity(11, 0x4cf, getSysTask(17), NULL);
                break;
            }
            if (entity) {
                INT DmgKind = 0;
                entity->setPos(getPos());
                switch (m_pBodyColl->getHitCollBody()->getDamageKind()) {
                case ATK_KIND_GM_DAMAGE:
                    DmgKind = 2;
                    break;
                case ATK_KIND_GM_FAINT:
                    DmgKind = 2;
                    break;
                case ATK_KIND_GM_BIG_DAMAGE:
                    DmgKind = 3;
                    break;
                case ATK_KIND_GM_CONFUSE:
                    DmgKind = 6;
                    break;
                }
                ((CGimmickDeceiveEnemy*)entity)->setDmgKind(DmgKind);
                setEntityFlag(1, 0);
            }
        } else {
            TRACE("\x95\xcf\x8c\x60\x82\xb3\x82\xb9\x82\xc8\x82\xa2\x81\x49\x81\x49\x8b\x97\x97\xa3\x97\xa3\x82\xea\x82\xc4\x82\xe9");
        }
    }
    changeMotion(2, 0, FALSE, -1, 0, 0);
    setEntityFlag(1, 0);
    rideModelEnableAllCollision(FALSE);
    m_Stat = 1;
    changeGimmickState(0);
}

void CGimmickDeceive::stateHitAttack() {}

void CGimmickDeceive::endStateHitAttack() {}
// SOURCE DATA: .rodata
