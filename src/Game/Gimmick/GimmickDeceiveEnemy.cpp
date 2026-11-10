
#include "Game/Gimmick/GimmickDeceiveEnemy.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in GimmickStatue); which
 * headers they come from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}
inline void _emptyStringPlaceholder2() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}

#include "Game/Movement.h"
#include "Game/Player/Player.h"
#include "Game/GameWork.h"
#include "Game/SceneInfo.h"
#include "Game/SceneEnv.h"
#include "Game/Enemy/MapEnemyManager.h"

CGimmickDeceiveEnemy::CGimmickDeceiveEnemy(CTask* inParent, INT inGimmickID) : CBaseGimmick(inParent) {
    m_GimID = inGimmickID;
    m_Step = 0;
    m_WaitCounter = 0.0f;
}

CGimmickDeceiveEnemy::~CGimmickDeceiveEnemy() {
    CGameWork::setFlag(1, 5, FALSE);
    CGameWork::setFlag(1, 3, FALSE);
    CGameWork::setFlag(1, 4, FALSE);
}

BOOL CGimmickDeceiveEnemy::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    CSceneEnv::setStop(ObjType::OBJ_BIT_ENEMY);
    CGameWork::setFlag(1, 5, TRUE);
    CGameWork::setFlag(1, 3, TRUE);
    CGameWork::setFlag(1, 4, TRUE);
    switch (m_GimID) {
    case 4:
    case 0x83:
    case 0xE7:
    case 0x14B:
    case 0x213:
    case 0x33F:
    case 0x362:
    case 0x46B:
    case 0x48B:
    case 0x4CF:
        changeMotion(1, 0, FALSE, 0, 0, 0);
        break;
    case 0x2332:
        changeMotion(1, 0, FALSE, 0, 0, 0);
        break;
    }
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    return TRUE;
}

void CGimmickDeceiveEnemy::beginStateMain() {}

void CGimmickDeceiveEnemy::stateMain() {
    switch (m_GimID) {
    case 4:
    case 0x83:
    case 0xE7:
    case 0x14B:
    case 0x213:
    case 0x33F:
    case 0x362:
    case 0x46B:
    case 0x48B:
    case 0x4CF:
        BarrelSpider();
        break;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 0x2332:
        TresureEnemy();
        break;
    }
}

void CGimmickDeceiveEnemy::endStateMain() {}

void CGimmickDeceiveEnemy::BarrelSpider() {
    if (g_pPlayer->getPlayerState() == 0x1D) return;
    switch (m_Step) {
    case 0:
        changeMotion(2, 0, FALSE, -1, 0, 0);
        m_Step = 10;
        break;
    case 10:
        m_WaitCounter += (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_WaitCounter > 100.0f) {
            CMapEnemyManager::setEncountEnemy(0x1C, 0, m_DmgKind, 0);
            CGameWork::setFlag(1, 3, FALSE);
            CGameWork::setFlag(1, 4, FALSE);
            CSceneEnv::setStop(ObjType::OBJ_BIT_CLEAR);
            CSceneInfo::reqEnterBattle(0);
            m_Step = 50;
        }
        if (m_pAnim->getFlag(1) & 2) m_Step = 20;
        break;
    case 20:
        changeMotion(3, 0, FALSE, -1, 0, 0);
        CMapEnemyManager::setEncountEnemy(0x1C, 0, m_DmgKind, 0);
        CGameWork::setFlag(1, 3, FALSE);
        CGameWork::setFlag(1, 4, FALSE);
        CSceneEnv::setStop(ObjType::OBJ_BIT_CLEAR);
        CSceneInfo::reqEnterBattle(0);
        m_Step = 50;
        break;
    }
}

void CGimmickDeceiveEnemy::TresureEnemy() {
    switch (m_Step) {
    case 0:
        m_pAnim->change(1, 0, FALSE);
        for (INT i = 0; i < getModelCount(); i++) {
            getModel(i)->setAlphaScale(0.0f);
        }
        m_Step = 5;
        break;
    case 5:
        for (INT i = 0; i < getModelCount(); i++) {
            FLOAT alpha = getModel()[i].getAlphaScale();
            alpha += (FLOAT)getFrameRate() * 0.0200000014f * 0.05f;
            if (alpha >= 1.0f) {
                alpha = 1.0f;
                m_Step = 10;
            }
            getModel(i)->setAlphaScale(alpha);
        }
        break;
    case 10:
        m_WaitCounter += (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_WaitCounter > 20.0f) {
            switch (m_GimID) {
            case 6:
                CMapEnemyManager::setEncountEnemy(3, 0, 0, 0);
                break;
            case 7:
                CMapEnemyManager::setEncountEnemy(4, 0, 0, 0);
                break;
            case 8:
                CMapEnemyManager::setEncountEnemy(5, 0, 0, 0);
                break;
            case 9:
                CMapEnemyManager::setEncountEnemy(13, 0, 0, 0);
                break;
            case 10:
                CMapEnemyManager::setEncountEnemy(6, 0, 0, 0);
                break;
            case 11:
                CMapEnemyManager::setEncountEnemy(18, 0, 0, 0);
                break;
            case 12:
                CMapEnemyManager::setEncountEnemy(25, 0, 0, 0);
                break;
            case 13:
                CMapEnemyManager::setEncountEnemy(15, 0, 0, 0);
                break;
            case 14:
                CMapEnemyManager::setEncountEnemy(29, 0, 0, 0);
                break;
            }
            CGameWork::setFlag(1, 3, FALSE);
            CGameWork::setFlag(1, 4, FALSE);
            CSceneInfo::reqEnterBattle(0);
            CSceneEnv::setStop(ObjType::OBJ_BIT_CLEAR);
            m_Step = 50;
        }
        break;
    }
}

// SOURCE DATA: .rodata
// SOURCE DATA SLUS_217.99_retail: .rodata
