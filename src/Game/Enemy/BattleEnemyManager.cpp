#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
/* .rodata starts with the strings of the included headers' inline functions, in this order. */
#include "System/Task.h"
#include "System/TaskMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/BattleEnemy.h"

#include "Game/Enemy/BattleEnemyManager.h"
#include "Game/OsdDef.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "Game/Enemy/EnemyParam.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"
/* MATCH: Placeholder: an empty string follows CollUtil.h's strings in .rodata; which header it comes from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
#include "Game/Card.h"
#include "Game/Enemy/MonstroEventBattle.h"
#include "Game/EntityFactory.h"
#include "Game/SceneInfo.h"
#include "Game/Game.h"
#include "Game/GameWork.h"
#include "Game/Debug/DebugWork.h"
#include "System/Math3D.h"
#include "System/Rnd.h"
#include <math.h>
#include "include_asm.h"

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new(size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}

#define CLASS_TYPE_CBATTLE_ENEMY 0x1004
#define CLASS_TYPE_CPRIZE 0x1008
#define OBJ_GROUP_MAX 16U
#define DEG2RAD 0.017453292f
template <class T> static inline T& _ref(T& v) { return v; }

#include "Game/Player/Player.h"

extern CPlayer* g_pPlayer;

#include "Game/Prize.h"












#include "Form/Menu/Roulette.h"






#include "Game/Player/PlayerGauge.h"

#include "System/Buf.h"












#include "System/RsrcDB.h"





/* CBaseGimmick::setParam (Game/Gimmick/BaseGimmick.h): m_iParam at 0x1d4 */
static inline void _setGimmickParam(CEntity* inEntity, INT inParam) {
    *(INT*)((UBYTE*)inEntity + 0x1d4) = inParam;
}

INT CBattleEnemyManager::m_iBeforeFormation = -1;

CBattleEnemyManager::CBattleEnemyManager(CTask* inParent) : CTask(inParent, 0x4000) {
    m_bDead = FALSE;
    m_bPremiumPrizeGet = FALSE;
    m_bRoulettePrizeGet = FALSE;
    m_bCreateCard = FALSE;
    m_bFaintContinue = FALSE;
    m_bCheckBeforeCardBattle = TRUE;
    m_iNextGroup = 0;
    m_pHeader = NULL;
    m_pFormation = NULL;
    m_iTotalEnemyNum = 0;
    m_iAppearGroupCount = 0;
    m_hDidCardBattleEnemy = 0;
    m_hMonstroEventBattle = 0;
    declareSysTask(0x19);
    CGameWork::setFlag(1, 2, FALSE);
    if (CMonstroEventBattle::isMonstroEventBattleScene()) {
#line 76
        CMonstroEventBattle* event = new (__FILE__ "(76)") CMonstroEventBattle(this);
        ASSERT(event);
        event->setup();
        m_hMonstroEventBattle = event->getHandle();
        setAppearPremiumPrize(TRUE);
        m_iStep = 8;
    } else {
        m_iNextGroupNum = 1;
        m_iGroupIndex = 0;
        OSD_HEADER* header = getOSD_();
        if (header) {
            m_pHeader = header;
            m_pFormation = getFormation_();
            for (INT i = 0; i < OBJ_GROUP_MAX; i++) {
                OBJ_GROUP* group = m_pFormation->getEnemyGroup(i);
                if (group) {
                    m_iGroupIndex = i;
                    m_iAppearGroupCount = 0;
                    if (group->iNextGroupNum > 0) m_iNextGroupNum = group->iNextGroupNum;
                    break;
                }
            }
            for (INT i = 0; i < OBJ_GROUP_MAX; i++) {
                OBJ_GROUP* group = m_pFormation->getEnemyGroup(i);
                if (group) m_iTotalEnemyNum += group->iObjCount;
            }
            for (INT i = 0; i < OBJ_GROUP_MAX; i++) {
                OBJ_GROUP* group = m_pFormation->getGimmickGroup(i);
                if (group) {
                    OBJ_INFO* info = group->pObjInfo;
                    for (INT j = 0; j < group->iObjCount; j++, info++) {
                        CEntity* gimmick = CEntityFactory::getInstance()->createEntity(*info, NULL, &info->iUniqueID);
                        if (gimmick) {
                            _setGimmickParam(gimmick, info->iParam);
                            gimmick->AfterInit();
                        }
                        info->setFlag(1, TRUE);
                    }
                }
            }
        }
        setAppearPremiumPrize(FALSE);
        m_iStep = 1;
    }
    setAppearRoulettePrize(FALSE);
    TRACE("---- Constructor CBattleEnemyManager\n");
}

CBattleEnemyManager::~CBattleEnemyManager() {
    TRACE("---- Destructor CBattleEnemyManager\n");
    if (!m_bDead) endBattle_(2);
    CGameWork::setFlag(1, 0x22, FALSE);
}

void CBattleEnemyManager::clearBeforeFormation() {
    m_iBeforeFormation = -1;
}

void CBattleEnemyManager::main() {
    switch (m_iStep) {
    case 1:
        startNearestCardBattleEnemy_();
        createBattleEnemies_();
        if (isEndBattle_()) {
            if (isRoulettePrizeGet()) {
                m_iStep = 4;
            } else {
                if (m_bCreateCard) {
                    m_tqTimer = 15000;
                    m_iStep = 2;
                } else {
                    m_tqTimer = 6000;
                    m_iStep = 2;
                }
            }
        }
        if (dbgButtonTrg((USER)0, 1, 1) && CSceneInfo::getCurrentAreaNum() == 22
            && (UINT)(CSceneInfo::getCurrentSceneNum() - 1050) < 31) {
            for (INT i = 0; i < OBJ_GROUP_MAX; i++) {
                OBJ_GROUP* group = m_pFormation->getEnemyGroup(i);
                if (group) {
                    OBJ_INFO* info = group->pObjInfo;
                    if (info) {
                        CEntity* entity = CEntityFactory::getInstance()->createEntity(*info, this, &info->iParam);
                        if (entity) {
                            entity->setEntityFlag(0x40000, 0);
                            m_iTotalEnemyNum++;
                        }
                        break;
                    }
                }
            }
        }
        break;
    case 2:
        if (Gauge::CPrizeGauge::isNowDispCard()) {
            m_iStep = 3;
        } else {
            m_tqTimer -= getFrameRate();
            if (m_tqTimer <= 0) m_iStep = 4;
        }
        break;
    case 3:
        if (!Gauge::CPrizeGauge::isNowDispCard()) m_iStep = 4;
        break;
    case 4:
        if (CPlayer::isEnableBattleEnd()) {
            m_bDead = TRUE;
            endBattle_(1);
            if (isPremiumPrizeGet()) {
#line 312
                new (__FILE__ "(312)") CRoulette(this, 0);
                m_iStep = 5;
            } else if (isRoulettePrizeGet()) {
#line 318
                new (__FILE__ "(318)") CRoulette(this, 1);
                m_iStep = 6;
            } else {
                exitBattle_();
                m_iStep = 7;
            }
        }
        break;
    case 5:
        if (!CTaskMan::getSysTask(0x22)) {
            if (isRoulettePrizeGet()) {
#line 336
                new (__FILE__ "(336)") CRoulette(this, 1);
                m_iStep = 6;
            } else {
                exitBattle_();
                m_iStep = 7;
            }
        }
        break;
    case 6:
        if (!CTaskMan::getSysTask(0x22)) {
            exitBattle_();
            m_iStep = 7;
        }
        break;
    case 8:
        startNearestCardBattleEnemy_();
        break;
    case 7:
        CBattleEnemy::setBossKilledFlag();
        break;
    case 9:
    case 10:
        break;
    }
}

void CBattleEnemyManager::disp() {}

void CBattleEnemyManager::setHpEnemies(INT inHp) {
    for (SLContainerT<CTask>* pContainer = getChildren().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task && task->getTypeID() == CLASS_TYPE_CBATTLE_ENEMY) {
            ((CBattleEnemy*)task)->setHp(inHp);
        }
    }
}

SLContainerT<CTask>* CBattleEnemyManager::getFirstBattleEnemy() {
    return getChildren().getHead();
}

HANDLE CBattleEnemyManager::getNearestBattleEnemy(CEntity& inEntity) {
    FLOAT nearest = 100000000.0f;
    HANDLE handle = -1;
    for (SLContainerT<CTask>* pContainer = getChildren().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task && task->getTypeID() == CLASS_TYPE_CBATTLE_ENEMY) {
            FLOAT dist = ((CEntity*)task)->calcDistanceSq(inEntity);
            FLOAT childDist;
            HANDLE childHandle = ((CBattleEnemy*)task)->getNearestChild(childDist, inEntity);
            if (childHandle != -1 && childDist < dist) {
                dist = childDist;
                task = CTaskMan::getTask(childHandle);
            }
            if (dist < nearest) {
                handle = task->getHandle();
                nearest = dist;
            }
        }
    }
    return handle;
}

HANDLE CBattleEnemyManager::getLockOnBattleEnemy(CEntity& inEntity) {
    FLOAT nearest = 100000000.0f;
    HANDLE handle = -1;
    for (SLContainerT<CTask>* pContainer = getChildren().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task && task->getTypeID() == CLASS_TYPE_CBATTLE_ENEMY && ((CEntity*)task)->isPermissionLockOn()) {
            FLOAT dist = ((CEntity*)task)->calcDistanceSq(inEntity);
            FLOAT childDist;
            HANDLE childHandle = ((CBattleEnemy*)task)->getNearestChild(childDist, inEntity);
            if (childHandle != -1 && childDist < dist) {
                dist = childDist;
                task = CTaskMan::getTask(childHandle);
            }
            if (dist < nearest) {
                handle = task->getHandle();
                nearest = dist;
            }
        }
    }
    return handle;
}

INT CBattleEnemyManager::getEnemyCount() const {
    INT count = 0;
    for (SLContainerT<CTask>* pContainer = getChildren().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task && task->getTypeID() == CLASS_TYPE_CBATTLE_ENEMY) count++;
    }
    return count;
}

INT CBattleEnemyManager::getPrizeCount() const {
    INT count = 0;
    for (SLContainerT<CTask>* pContainer = getChildren().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task && task->getTypeID() == CLASS_TYPE_CPRIZE) count++;
    }
    return count;
}

BOOL CBattleEnemyManager::canCardBattle(HANDLE inHandle) {
    if (getEnemyCount() < 2) return TRUE;
    if (!m_bCheckBeforeCardBattle) return TRUE;
    return m_hDidCardBattleEnemy != inHandle;
}

BOOL CBattleEnemyManager::canCreateEnemyCard() const {
    CMonstroEventBattle* event = (CMonstroEventBattle*)CTaskMan::getTask(m_hMonstroEventBattle);
    if (event) {
        if (event->isFullGauge()) return TRUE;
    } else if (getTotalEnemyNum() <= 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL CBattleEnemyManager::isAppearEnd() const {
    for (INT i = m_iGroupIndex; i < OBJ_GROUP_MAX; i++) {
        if (m_pFormation->getEnemyGroup(i)) return FALSE;
    }
    return TRUE;
}

BOOL CBattleEnemyManager::checkBattleEnemyFlag(ULONGLONG inFlag) {
    for (SLContainerT<CTask>* pContainer = getChildren().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task && task->getTypeID() == CLASS_TYPE_CBATTLE_ENEMY) {
            if (((CBattleEnemy*)task)->getBattleEnemyFlag() & inFlag) return TRUE;
        }
    }
    return FALSE;
}

void CBattleEnemyManager::notifyDead(HANDLE inHandle) {
    CMonstroEventBattle* event = (CMonstroEventBattle*)CTaskMan::getTask(m_hMonstroEventBattle);
    for (SLContainerT<CTask>* pContainer = getChildren().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task && task->getHandle() == inHandle) {
#line 622
            ASSERT(task->getTypeID() == CLASS_TYPE_CBATTLE_ENEMY);
            if (!event && (((CEntity*)task)->getFlag() & 0x40000)) {
                if (--m_iTotalEnemyNum <= 0) CPlayer::notifyExterminate();
            }
            return;
        }
    }
}

INT CBattleEnemyManager::removePrize() {
    INT count = 0;
    for (SLContainerT<CTask>* pContainer = getChildren().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task && task->getTypeID() == CLASS_TYPE_CPRIZE) {
            task->out();
            count++;
        }
    }
    return count;
}

CEntity* CBattleEnemyManager::createRandomFormationEnemy(const FVECTOR& inPos, INT inCheckID) {
    OBJ_INFO* select[16];
    INT num = 0;
    SHORT checkGroupID = (UINT)inCheckID / 10000;
    SHORT checkRsrcID = (UINT)inCheckID % 10000;
    for (INT i = 0; i < OBJ_GROUP_MAX; i++) {
        OBJ_GROUP* group = m_pFormation->getEnemyGroup(i);
        if (group) {
            OBJ_INFO* info = group->pObjInfo;
            for (INT j = 0; j < group->iObjCount; j++, info++) {
                if (info->sID == checkRsrcID && info->sGroupID == checkGroupID) continue;
                BOOL find = FALSE;
                for (INT k = 0; k < num; k++) {
                    if (select[k]->iGroupRsrcID == info->iGroupRsrcID) {
                        find = TRUE;
                        break;
                    }
                }
                if (!find) select[num++] = info;
            }
        }
    }
    if (num == 0) return NULL;
    CEntity* enemy = CEntityFactory::getInstance()->createEntity(*select[RND::genrand_int31() % num], this, NULL);
    if (enemy) {
        enemy->setEntityFlag(0x40000, 0);
        enemy->AfterInit();
        enemy->warp(inPos);
        enemy->updateCoord();
        enemy->coordFix();
        m_iTotalEnemyNum++;
    }
    return enemy;
}

CEntity* CBattleEnemyManager::createEnemy(INT inGroupID, INT inID, const FVECTOR& inPos, FLOAT inDir, INT inParam) {
    CEntity* enemy = CEntityFactory::getInstance()->createEntity(inGroupID, inID, inPos, inDir, this, &inParam);
    if (enemy) {
        enemy->setEntityFlag(0x40000, 0);
        enemy->AfterInit();
        m_iTotalEnemyNum++;
    }
    return enemy;
}

CEntity* CBattleEnemyManager::createPremiumPrize(const FVECTOR& inPos, INT inEnemyID) {
    PRIZE_PARAM param;
    if (CGame::isRikuGame()) return NULL;
    if ((USHORT)(CSceneInfo::getCurrentSceneNum() - 1050) < 149) return NULL;
    if (isAppearPremiumPrize()) return NULL;
    if (CCard::isCurrentDeckPremiumFull()) return NULL;
    if (!(DebugWork::g_dw.enemy.uiFlag & 0x80)) {
        RoomInfo* room = CSceneInfo::getRoomInfo(-1);
        if (!room) return NULL;
        switch (room->getCardID()) {
        case 0xa7:
            if (RND::genrand_int31() % 10) return NULL;
            break;
        case 0xa8:
            if (inEnemyID != 0x10 && inEnemyID != 0x22) return NULL;
            break;
        default:
            if (RND::genrand_int31() % 1000) return NULL;
            break;
        }
    }
    setAppearPremiumPrize(TRUE);
    param.vPos = inPos;
    param.fSpeed = 0.25f;
    param.uiFlag = 1;
    param.PrizeKind = 0xd6;
    param.uiPrizeFlag = 0;
    param.iNumber = 0;
    param.vRot.set((-70.0f - RND::genrand_real1() * 10.0f) * DEG2RAD, RND::genrand_real1() * 360.0f * DEG2RAD, 0.0f);
    Math3D::LimitPIVector(param.vRot);
    return CEntityFactory::getInstance()->createPrize(param, NULL);
}

CEntity* CBattleEnemyManager::createRoulettePrize(const FVECTOR& inPos) {
    PRIZE_PARAM param;
    if ((USHORT)(CSceneInfo::getCurrentSceneNum() - 1050) < 149) return NULL;
    if (isAppearRoulettePrize()) return NULL;
    if (CCard::isMapCardFull()) return NULL;
    if (!CCard::m_pCardInfo->_nMapTotal) return NULL;
    if (!(DebugWork::g_dw.enemy.uiFlag & 0x100)) {
        RoomInfo* room = CSceneInfo::getRoomInfo(-1);
        if (!room) return NULL;
        if (room->getCardID() != 0xab) return NULL;
        if (RND::genrand_int31() % 100 >= 16) return NULL;
    }
    setAppearRoulettePrize(TRUE);
    param.vPos = inPos;
    param.fSpeed = 0.25f;
    param.uiFlag = 1;
    param.PrizeKind = 0xd7;
    param.uiPrizeFlag = 0;
    param.iNumber = 0;
    param.vRot.set((-70.0f - RND::genrand_real1() * 10.0f) * DEG2RAD, RND::genrand_real1() * 360.0f * DEG2RAD, 0.0f);
    Math3D::LimitPIVector(param.vRot);
    return CEntityFactory::getInstance()->createPrize(param, NULL);
}

void CBattleEnemyManager::monstroEventBattleGaugeUp() {
    CMonstroEventBattle* event = (CMonstroEventBattle*)CTaskMan::getTask(m_hMonstroEventBattle);
    if (event) event->gaugeUp();
}

void CBattleEnemyManager::warpPrize(const FVECTOR& inCenter) {
    for (SLContainerT<CTask>* pContainer = getChildren().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task && task->getTypeID() == CLASS_TYPE_CPRIZE) {
            FVECTOR pos = inCenter;
            FLOAT theta = Math3D::LimitPI(RND::genrand_real1() * 360.0f);
            pos.x += (RND::genrand_real1() * 1.5f + 2.5f) * cosf(theta);
            pos.z += (RND::genrand_real1() * 1.5f + 2.5f) * sinf(theta);
            pos.y += 1.0f;
            ((CPrize*)task)->move(pos);
        }
    }
}

void CBattleEnemyManager::allGetPrize(UINT inFlag) {
    for (SLContainerT<CTask>* pContainer = getChildren().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task && task->getTypeID() == CLASS_TYPE_CPRIZE) {
            if (((CPrize*)task)->getPrizeKind() >= 203) {
                if (inFlag & 1) ((CPrize*)task)->forceGetPrize();
            } else {
                if (inFlag & 2) ((CPrize*)task)->forceGetPrize();
            }
        }
    }
}

void CBattleEnemyManager::prizeFlyHigh(BOOL bAll) {
    for (SLContainerT<CTask>* pContainer = getChildren().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task && task->getTypeID() == CLASS_TYPE_CPRIZE) {
            ((CPrize*)task)->flyHigh(bAll);
        }
    }
}

void CBattleEnemyManager::deleteAllEnemies() {
    for (SLContainerT<CTask>* pContainer = getChildren().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task) task->out();
    }
    m_iStep = 10;
}

OSD_HEADER* CBattleEnemyManager::getOSD_() {
    OSD_HEADER* header = NULL;
    CRsrcDB::initSeekRsrc(0x20, -1);
    CRsrcData* rsrc = CRsrcDB::seekNextRsrc_Scene();
    if (rsrc) header = (OSD_HEADER*)rsrc->getRsrcData();
    if (CGame::isRikuGame()) {
        rsrc = CRsrcDB::seekNextRsrc_Scene();
        if (rsrc) {
            OSD_HEADER* riku = (OSD_HEADER*)rsrc->getRsrcData();
            if (riku) header = riku;
        }
    }
    if (header && header->addrMapping()) return header;
    return NULL;
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemyManager", createBattleEnemies___19CBattleEnemyManager);
/* Near match: only `this` and `group` swap registers (s2/s3). */
#if 0
INT CBattleEnemyManager::createBattleEnemies_() {
    INT count;
    INT index;
    INT n;
    _MapEnemyRoomInfo* info;
    if (!m_pFormation) return 0;
    for (INT i = 0; i < m_iAppearGroupCount; i++) {
        if (getAppearGroupCount_(m_iAppearGroup[i]) <= 0) {
            m_iAppearGroupCount--;
            for (INT j = i; j < m_iAppearGroupCount; j++) m_iAppearGroup[j] = m_iAppearGroup[j + 1];
            break;
        }
    }
    if (m_iAppearGroupCount - m_iNextGroupNum >= 0) return 0;
    n = m_iNextGroupNum - m_iAppearGroupCount;
    count = 0;
    index = m_iGroupIndex;
    for (INT i = 0; i < n; i++) {
        for (INT j = index; j < OBJ_GROUP_MAX; j++) {
            OBJ_GROUP* group = m_pFormation->getEnemyGroup(j);
            if (group) {
                OBJ_INFO* info = group->pObjInfo;
                for (INT k = 0; k < group->iObjCount; k++, info++) {
                    CEntity* enemy = CEntityFactory::getInstance()->createEntity(*info, this, &info->iParam);
                    if (enemy) {
                        info->setFlag(1, TRUE);
                        count++;
                        ((CBattleEnemy*)enemy)->setGroupNo(j);
                        enemy->setEntityFlag(0x40000, 0);
                        enemy->AfterInit();
                    }
                }
                m_iAppearGroup[m_iAppearGroupCount++] = j;
                if (group->iNextGroupNum > 0) m_iNextGroupNum = group->iNextGroupNum;
                index = j + 1;
                break;
            }
        }
    }
    m_iGroupIndex = index;
    info = CSceneInfo::getCurrentRoomMapEnemyInfo();
    if (info && info->_byHitPlayerAttack) {
        if (!isBadStatusContinued_()) info->_byHitPlayerAttack = 0;
    }
    return count;
}
#endif

INT CBattleEnemyManager::getAppearGroupCount_(INT inGroupNo) {
    INT count = 0;
    for (SLContainerT<CTask>* pContainer = getChildren().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task && task->getTypeID() == CLASS_TYPE_CBATTLE_ENEMY) {
            if (((CBattleEnemy*)task)->getGroupNo() == inGroupNo) count++;
        }
    }
    return count;
}

OBJ_FORMATION* CBattleEnemyManager::getFormation_() {
    INT start = 0;
    INT end = 0;
    SHORT encount = 0;
    int select[16];
    INT index;
    INT n;
    if (DebugWork::g_dw.enemy.uiFlag & 4) {
        start = end = DebugWork::g_dw.enemy.sFormation;
    } else {
        _MapEnemyRoomInfo* info = CSceneInfo::getCurrentRoomMapEnemyInfo();
        if (CSceneInfo::getCurrentWorldID(TRUE) == 9) {
            RoomInfo* room = CSceneInfo::getRoomInfo(-1);
            if (room) {
                switch (room->getCardID()) {
                case 0xb0:
                    end = 6;
                    break;
                case 0xa7:
                    start = 7;
                    end = 11;
                    break;
                default:
                    if (info) {
                        start = info->_byFormationNo1;
                        end = info->_byFormationNo2;
                    }
                    break;
                }
            }
            if (info) encount = info->_sEncountEnemyID;
        } else if (info) {
            start = info->_byFormationNo1;
            end = info->_byFormationNo2;
            encount = info->_sEncountEnemyID;
        }
    }
    n = 0;
    for (INT i = start; i <= end; i++) {
        if (m_iBeforeFormation == i) continue;
        OBJ_FORMATION* formation = m_pHeader->pFormation[i];
        if (!formation) continue;
        INT num = 1;
        for (INT j = 0; j < OBJ_GROUP_MAX; j++) {
            OBJ_GROUP* group = formation->getEnemyGroup(j);
            if (group) {
                num = group->iNextGroupNum;
                break;
            }
        }
        for (INT j = 0; j < OBJ_GROUP_MAX; j++) {
            OBJ_GROUP* group = formation->getEnemyGroup(j);
            if (group) {
                BOOL find = FALSE;
                OBJ_INFO* info = group->pObjInfo;
                for (INT k = 0; k < group->iObjCount; k++, info++) {
                    if (info->sID == encount) {
                        select[n++] = i;
                        find = TRUE;
                        break;
                    }
                }
                if (find) break;
                if (--num <= 0) break;
            }
        }
    }
    if (n > 0) {
        _ref(m_iBeforeFormation) = select[RND::genrand_int31() % n];
        return m_pHeader->pFormation[m_iBeforeFormation];
    } else {
        index = end - start;
        if (index <= 0) {
            _ref(m_iBeforeFormation) = start;
        } else {
            _ref(m_iBeforeFormation) = start + RND::genrand_int31() % (index + 1);
        }
        if (!m_pHeader->pFormation[m_iBeforeFormation]) _ref(m_iBeforeFormation) = 0;
    }
    return m_pHeader->pFormation[m_iBeforeFormation];
}

void CBattleEnemyManager::startNearestCardBattleEnemy_() {
    CBattleEnemy* nearestEnemy = NULL;
    FLOAT nearestDist = 100000000.0f;
    for (SLContainerT<CTask>* pContainer = getChildren().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task && task->getTypeID() == CLASS_TYPE_CBATTLE_ENEMY) {
            CBattleEnemy* enemy = (CBattleEnemy*)task;
            if (enemy->isPermissionCardBattle()) {
                enemy->clearBtlEnemyFlag(0x200);
                FLOAT dist = enemy->calcDistanceSq(_ref(g_pPlayer));
                if (dist < nearestDist) {
                    nearestEnemy = enemy;
                    nearestDist = dist;
                }
            }
        }
    }
    if (nearestEnemy) {
        nearestEnemy->setBtlEnemyFlag(0x800);
        setDidCardBattleEnemy(nearestEnemy->getHandle());
    }
}

void CBattleEnemyManager::endBattle_(BYTE inStatus) {
    _MapEnemyRoomInfo* info = CSceneInfo::getCurrentRoomMapEnemyInfo();
    if (!info) return;
    SHORT id = info->_sEncountUniqueID;
    INT i = 0;
    do {
        _MapEnemyInfo& e = info->_MapEnemies[i];
        if (e._byID == id) {
            if (inStatus == 2 && info->_byAppearType == 1) {
                e._byDead = 0;
            } else {
                e._byDead = inStatus;
            }
            break;
        }
    } while (++i < 32);
    info->_byHitPlayerAttack = 0;
    info->_sEncountEnemyID = 0;
    info->_sEncountUniqueID = 0;
}

BOOL CBattleEnemyManager::isBadStatusContinued_() {
    RoomInfo* room = CSceneInfo::getRoomInfo(-1);
    if (room) {
        if (room->getCardID() == 0xb2) return TRUE;
    }
    return FALSE;
}

BOOL CBattleEnemyManager::isEndBattle_() {
    if (getChildrenCount() > 0) return FALSE;
    if (getTotalEnemyNum() > 0) return FALSE;
    return !((USHORT)(CSceneInfo::getCurrentSceneNum() - 1050) < 149);
}

void CBattleEnemyManager::exitBattle_() {
    if (!CGameWork::getFlag(1, 4)) CSceneInfo::reqExitBattle();
}

// SOURCE DATA: .data .rodata
