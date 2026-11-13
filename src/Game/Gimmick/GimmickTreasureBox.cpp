#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Gimmick/GimmickTreasureBox.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"

#include "Game/CardBattle.h"
#include "Game/Card.h"
#include "Game/Technic.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in GimmickTutorial); which header
 * it comes from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}


#include "Game/Atel/ChatMan.h"

#include "System/Graphic/PathCamera.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "System/PadHandler.h"
#include "Game/Movement.h"
#include "Game/EntityFactory.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "Game/Player/Player.h"
#include "Game/SceneInfo.h"
#include "Game/SceneEnv.h"
#include "Game/GameWork.h"

#include "include_asm.h"


#include "Game/Gimmick/GimmickManager.h"






/* Local stand-ins (original: Game/Atel/VSME.h ChatWndCreateCTDParam with CPointI, Form/Menu/LevelUp.h,
 * Game/Player/PlayerGauge.h, the sound header). */
struct ChatWndSetWordsParam;
struct ChatPointI {
    INT m_x;
    INT m_y;
    void setPoint(INT x, INT y) {
        m_x = x;
        m_y = y;
    }
};

struct ChatWndCreateCTDParam {
    ChatPointI pos;
    ChatPointI cyobiPos;
    UINT ctdID;
    UINT waitFrame;
    ChatWndSetWordsParam* pCwswp;
    UINT nLine;
    ChatWndCreateCTDParam() { nLine = 0; } /* MATCH: the beta's markers put the nLine store in WndCmnd.h's inline (VSME.cpp) */
};
#include "System/Sound/SoundMan.h"
#line 74

#include "Form/Menu/LevelUp.h"








#include "Game/Player/PlayerGauge.h"
#include "System/HeapMan.h"

CGimmickTreasureBox::CGimmickTreasureBox(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_Stat = 0;
    m_LocalStep = 0;
    m_pPathCamera = NULL;
    m_WaitCounter = 0.0f;
}

CGimmickTreasureBox::~CGimmickTreasureBox() {
    if (m_pPathCamera) {
        delete m_pPathCamera;
        m_pPathCamera = NULL;
    }
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickTreasureBox::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    m_pAnim->change(1, 0, FALSE);
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    return TRUE;
}

INT CGimmickTreasureBox::getTypeID() const {
    return 0x2000;
}

void CGimmickTreasureBox::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    KH_DEBUG(if (getParam() != 0 && getParam() != 10) m_Stat |= 4;)
    if (CSceneInfo::getCurrentSceneNum() == 24) {
        if (CGameWork::progMCW()._KeyRoomsCreateState) {
            if (getParam() == 0) out();
        } else {
            if (getParam() == 10) out();
        }
    }
    if (m_Stat & 1) {
        changeMotion(3, 0, FALSE, 0, 0, 0);
        rideModelEnableChipCollision(0, FALSE);
        rideModelEnableChipCollision(1, TRUE);
        bodyCollOffBody(0);
        m_Step = 9;
    } else {
        changeMotion(1, 0, FALSE, 0, 0, 0);
        bodyCollOffBody(1);
        bodyCollOffBody(2);
        rideModelEnableChipCollision(0, TRUE);
        rideModelEnableChipCollision(1, FALSE);
        getSysTask(32);
        LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    }
}

void CGimmickTreasureBox::OpenTreasureBox() {
    m_Step = 2;
    CGameWork::setFlag(1, 3, TRUE);
    CGameWork::setFlag(1, 4, TRUE);
    CGameWork::setFlag(1, 6, TRUE);
}

void CGimmickTreasureBox::setState(INT in_State) {
    CGimmickManager* task = (CGimmickManager*)getSysTask(17);
    m_Stat = task->getStorageGimmickState(getUniqueID()) | in_State;
    task->setStorageGimmickState(getUniqueID(), m_Stat);
}

void CGimmickTreasureBox::setLock(BOOL in_Disp) {
    if (in_Disp == TRUE && ((m_Stat ^ 1) & 1)) {
        setEntityFlag(0x10000, 0);
    } else {
        setEntityFlag(0, 0x10000);
    }
}

BOOL CGimmickTreasureBox::checkOpenTresureBoxPermission() {
    return TRUE;
}

BOOL CGimmickTreasureBox::CardMaxWarningMain() {
    switch (m_LocalStep) {
    case 0: {
        ChatWndCreateCTDParam CWCP2;
        CWCP2.pos.setPoint(-1, -1);

        CWCP2.cyobiPos.setPoint(0x80, 0x100);
        CWCP2.pCwswp = NULL; /* MATCH: the beta's marker order (287 pCwswp, 289 ctdID) */
        CWCP2.waitFrame = 0;
        CWCP2.ctdID = 0x400000;
        ATEL::CChatMan::instance().reqCommand(0x405, &CWCP2, 0);
        ATEL::CChatMan::instance().reqCommand(0x3EA, NULL, 0);
        CSound::playSE(3, -1, 0x40);
        m_LocalStep = 10;
        break;
    }
    case 10: {
        INT ret;
        INT iFg[1]; // MATCH: [1]: retail ties this+0x40 as the original only with the local in memory from the start (beta matches both), user-accepted 2026-11-13
        ATEL::CChatMan::instance().reqCommand(0x3ED, &ret, 0);
        if (ret != 5) return FALSE;
        if (CPadHandler::isTrgOK() || CPadHandler::isTrgCANCEL()) {
            iFg[0] = 1;
            ATEL::CChatMan::instance().reqCommand(0x3EB, iFg, 0);
            CSound::playSE(4, -1, 0x40);
            m_LocalStep = 20;
        }
        break;
    }
    case 20: {
        INT ret;
        INT iFg[1];
        ATEL::CChatMan::instance().reqCommand(0x3ED, &ret, 0);
        if (ret != 9) return FALSE;
        iFg[0] = 0;
        ATEL::CChatMan::instance().reqCommand(0x3EC, iFg, 0);
        CGameWork::setFlag(1, 5, FALSE);
        CSceneEnv::setStop(ObjType::OBJ_BIT_CLEAR);
        m_Step = 9;
        m_WaitCounter = 0.0f;
        m_Stat |= 1;
        CGameWork::setFlag(1, 5, FALSE);
        bodyCollOnBody(1);
        bodyCollOnBody(2);
        bodyCollOffBody(0);
        CSceneEnv::setStop(ObjType::OBJ_BIT_CLEAR);
        if (m_pPathCamera) {
            delete m_pPathCamera;
        }
        m_pPathCamera = NULL;
        CGameWork::setInt(1, 0xE2, 0);
        rideModelEnableChipCollision(0, FALSE);
        rideModelEnableChipCollision(1, TRUE);
        CGameWork::setFlag(1, 3, FALSE);
        CGameWork::setFlag(1, 4, FALSE);
        saveStat(m_Stat);
        ((CGimmickManager*)getSysTask(17))->changeTreasureBoxLock(TRUE);
        return TRUE;
    }
    }
    return FALSE;
}

void CGimmickTreasureBox::beginStateMain() {}

void CGimmickTreasureBox::stateMain() {
    if ((m_Stat ^ 1) & 1) {
        if (m_Stat & 4) {
            EnemyMain();
        } else {
            NormalMain();
        }
    }
}

void CGimmickTreasureBox::endStateMain() {}

void CGimmickTreasureBox::NormalMain() {
    switch (m_Step) {
    case 1:
        if (CardMaxWarningMain()) m_Step = 0;
        break;
    case 2:
        ((CGimmickManager*)getSysTask(17))->changeTreasureBoxLock(FALSE);
        CGameWork::setFlag(1, 3, TRUE);
        CGameWork::setFlag(1, 4, TRUE);
        CGameWork::setFlag(1, 6, FALSE);
        setEntityFlag(0, 0x10000);
        g_pPlayer->cancelLockon();
        CSceneEnv::setStop((ObjType::eObjBit)(ObjType::OBJ_BIT_BULLET | ObjType::OBJ_BIT_ENEMY));
        m_pPathCamera = new KH_AT("(486)") CPathCamera(this);
        CGameWork::setInt(1, 0xE2, -1);
        if (m_pPathCamera->init("test", TRUE, FALSE, 0x80)) {
            m_pPathCamera->initBGCheck(FALSE);
            m_pPathCamera->setLocal(TRUE);
            m_pPathCamera->run();
            changeMotion(2, 0, FALSE, -1, 0, 0);
        } else {
            if (m_pPathCamera) {
                delete m_pPathCamera;
            }
            m_pPathCamera = NULL;
        }
        m_Step = 4;
        break;
    case 4: {
        if (m_pPathCamera->isCamEnd()) {
            CGimmickManager* mgr = (CGimmickManager*)getSysTask(17);
            DropPrize prize;
            prize.StockKind = 0;
            prize.Kind = CARD_KIND_None;
            prize.Num = 0;
            prize.No = 0;
            prize = mgr->DecideTreasurePrize();
            KH_DEBUG(mgr->GimDebugTrace(&prize);)
            CSceneEnv::setStop((ObjType::eObjBit)(ObjType::OBJ_BIT_TRUE_ALL & ~(ObjType::OBJ_BIT_2D | ObjType::OBJ_BIT_FUKIDASI | ObjType::OBJ_BIT_GIMMICK)));
            if (prize.StockKind) {
                CTechnic::addTechnic(prize.StockKind);
                new KH_AT("(525)") CLevelUp(getSysTask(1), prize.StockKind);
                m_Step = 7;
                CGameWork::setFlag(1, 5, TRUE);
            } else if (prize.Kind == 0) {
                TRACE("\x83\x4a\x81\x5b\x83\x68\x82\xa2\x82\xc1\x82\xcf\x82\xa2\x81\x49\x81\x49\n");
                m_LocalStep = 0;
                CGameWork::setFlag(1, 5, TRUE);
                m_Step = 1;
            } else {
                CGameWork::setFlag(1, 6, TRUE);
                CPlayer::notifyGetPrize(prize.Kind, prize.No, 2);
                CGameWork::setFlag(1, 5, TRUE);
                m_Step = 8;
            }
        }
        break;
    }
    case 7:
        if (CLevelUp::isEnd()) {
            m_Step = 9;
            m_Stat |= 1;
            CGameWork::setFlag(1, 5, FALSE);
            bodyCollOnBody(1);
            bodyCollOnBody(2);
            bodyCollOffBody(0);
            CSceneEnv::setStop(ObjType::OBJ_BIT_CLEAR);
            if (m_pPathCamera) {
                delete m_pPathCamera;
            }
            m_pPathCamera = NULL;
            CGameWork::setInt(1, 0xE2, 0);
            rideModelEnableChipCollision(0, FALSE);
            rideModelEnableChipCollision(1, TRUE);
            CGameWork::setFlag(1, 3, FALSE);
            CGameWork::setFlag(1, 4, FALSE);
            ((CGimmickManager*)getSysTask(17))->changeTreasureBoxLock(TRUE);
            saveStat(m_Stat);
        }
        break;
    case 8:
        if (!Gauge::CPrizeGauge::isNowDispCard()) {
            m_WaitCounter = 0.0f;
            m_Step = 9;
            m_Stat |= 1;
            CGameWork::setFlag(1, 5, FALSE);
            bodyCollOnBody(1);
            bodyCollOnBody(2);
            bodyCollOffBody(0);
            CSceneEnv::setStop(ObjType::OBJ_BIT_CLEAR);
            if (m_pPathCamera) {
                delete m_pPathCamera;
            }
            m_pPathCamera = NULL;
            CGameWork::setInt(1, 0xE2, 0);
            rideModelEnableChipCollision(0, FALSE);
            rideModelEnableChipCollision(1, TRUE);
            CGameWork::setFlag(1, 3, FALSE);
            CGameWork::setFlag(1, 4, FALSE);
            CGameWork::setFlag(1, 6, FALSE);
            ((CGimmickManager*)getSysTask(17))->changeTreasureBoxLock(TRUE);
            saveStat(m_Stat);
        }
        break;
    case 9:
        break;
    }
}

void CGimmickTreasureBox::EnemyMain() {
    switch (m_Step) {
    case 1:
        if (CardMaxWarningMain()) m_Step = 0;
        break;
    case 2:
        CGameWork::setFlag(1, 3, TRUE);
        CGameWork::setFlag(1, 4, TRUE);
        CGameWork::setFlag(1, 6, FALSE);
        setEntityFlag(0, 0x10000);
        g_pPlayer->cancelLockon();
        CSceneEnv::setStop((ObjType::eObjBit)(ObjType::OBJ_BIT_BULLET | ObjType::OBJ_BIT_ENEMY));
        m_pPathCamera = new KH_AT("(675)") CPathCamera(this);
        CGameWork::setInt(1, 0xE2, -1);
        if (m_pPathCamera->init("test", TRUE, FALSE, 0x80)) {
            m_pPathCamera->initBGCheck(FALSE);
            m_pPathCamera->setLocal(TRUE);
            m_pPathCamera->run();
            changeMotion(2, 0, FALSE, 0, 1, 0);
        } else {
            if (m_pPathCamera) {
                delete m_pPathCamera;
            }
            m_pPathCamera = NULL;
        }
        m_Step = 3;
        break;
    case 3:
        if (m_pAnim->getCurrentTqCrd() > 0x1964) {
            EnemyPop();
            m_Step = 5;
            m_Stat |= 1;
            saveStat(m_Stat);
        }
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        break;
    }
}

void CGimmickTreasureBox::EnemyPop() {
    CEntity* entity;
    FVECTOR v;
    INT world;

    v = getPos();
    entity = NULL;
    v.y += 1.0f;
    world = CSceneInfo::getCurrentWorldID();
    KH_DEBUG(if (world == 23) world = CSceneInfo::getCurrentSceneNum();)
    switch (world) {
    case 0:
    case 10:
        break;
    case 1:
    case 4:
    case 13:
        CEffectMan::setEffect("_ex_2110", 2, ObjType::OBJ_TYPE_GIMMICK, &v, NULL, 0.0f);
        entity = CEntityFactory::getInstance()->createEntity(11, 6, getSysTask(17), NULL);
        break;
    case 2:
    case 5:
    case 8:
        CEffectMan::setEffect("_ex_2130", 2, ObjType::OBJ_TYPE_GIMMICK, &v, NULL, 0.0f);
        entity = CEntityFactory::getInstance()->createEntity(11, 8, getSysTask(17), NULL);
        break;
    case 3:
        CEffectMan::setEffect("_ex_2120", 2, ObjType::OBJ_TYPE_GIMMICK, &v, NULL, 0.0f);
        entity = CEntityFactory::getInstance()->createEntity(11, 7, getSysTask(17), NULL);
        break;
    case 6:
        CEffectMan::setEffect("_ex_2230", 2, ObjType::OBJ_TYPE_GIMMICK, &v, NULL, 0.0f);
        entity = CEntityFactory::getInstance()->createEntity(11, 9, getSysTask(17), NULL);
        break;
    case 7:
        CEffectMan::setEffect("_ex_2070", 2, ObjType::OBJ_TYPE_GIMMICK, &v, NULL, 0.0f);
        entity = CEntityFactory::getInstance()->createEntity(11, 11, getSysTask(17), NULL);
        break;
    case 11:
        CEffectMan::setEffect("_ex_2160", 2, ObjType::OBJ_TYPE_GIMMICK, &v, NULL, 0.0f);
        entity = CEntityFactory::getInstance()->createEntity(11, 13, getSysTask(17), NULL);
        break;
    case 9:
    case 12:
        CEffectMan::setEffect("m_ex730", 2, ObjType::OBJ_TYPE_GIMMICK, &v, NULL, 0.0f);
        entity = CEntityFactory::getInstance()->createEntity(11, 14, getSysTask(17), NULL);
        break;
    }
    if (entity) {
        v.y -= 0.5f;
        entity->setPos(v);
        entity->warp(v);
        v = getRot();
        entity->setRot(v);
    }
}

CGimmickTreasureBoxForEffectID::CGimmickTreasureBoxForEffectID(CTask* inParent, const ZONEPARTS& inParts)
    : CBaseGimmick(inParent, inParts) {}

CGimmickTreasureBoxForEffectID::~CGimmickTreasureBoxForEffectID() {}

void CGimmickTreasureBoxForEffectID::beginStateMain() {}

void CGimmickTreasureBoxForEffectID::stateMain() {}

void CGimmickTreasureBoxForEffectID::endStateMain() {}

void CGimmickTreasureBoxForEffectID::beginStateHitAttack() {}

void CGimmickTreasureBoxForEffectID::stateHitAttack() {}

void CGimmickTreasureBoxForEffectID::endStateHitAttack() {}
// SOURCE DATA: .rodata
// SOURCE DATA SLUS_217.99_retail: .rodata
