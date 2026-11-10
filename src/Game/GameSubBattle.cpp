#define COLLUTIL_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollUtil.h" /* MATCH: this unit names CollUtil.h so (header default "Game/Collision/CollUtil.h") */
#define COLLDISP_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollDisp.h" /* MATCH: this unit names CollDisp.h so (header default "Game/Collision/CollDisp.h") */
#define STATUS_H_PATH "c:/KHCOM/MainApp/Game/Status.h" /* MATCH: this unit names Status.h so (header default "Game/Status.h") */
/* .rodata starts with the strings of the included headers' inline functions, in this order. */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "System/Task.h"
#include "Game/Collision/CollUtilA.h"
#include "System/TaskMan.h"
#include "Game/Collision/CollUtil.h"
#include "Game/Collision/CollDisp.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"

/* MATCH: Placeholder: an empty string follows Str.h's strings in .rodata; which header it comes from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}

/* MATCH: Placeholders for an unknown header's inline functions (prize messages) that come next in .rodata
 * (as in Game/Player/PlayerManager). */
inline void _premiumPrizeMessage() {
    TRACE("\x83v\x83\x8c\x83~\x83" "A\x83\x80\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}
inline void _roulettePrizeMessage() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("\x83\x8b\x81[\x83\x8c\x83" "b\x83g\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}

#include "System/Graphic/Effect/effect.h"
#include "Game/Status.h"
#include "System/Fsm.h"
#include "Game/GameSubBattle.h"
#include "System/Graphic/Camera.h"
#include "Game/SceneEnv.h"
#include "Game/Bullet/BulletManager.h"
#include "Game/MinigameManager.h"
#include "Form/Menu/LevelUp.h"
#include "System/MainApp.h"
#include "System/Debug.h"
#include "System/Graphic/WorldEnv.h"
#include "System/Graphic/DeFocus.h"
#include "System/Graphic/ShadowModel.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "System/Graphic/2d/2DSpr.h"
#include "System/Sound/SoundMan.h"
#include "Game/Game.h"
#include "Game/GameWork.h"
#include "Game/SceneInfo.h"
#include "Game/NowLoading.h"
#include "Game/Map.h"
#include "Game/MessageManager.h"
#include "Game/LockOnManager.h"
#include "Game/EntityFactory.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/Enemy/MapEnemyManager.h"
#include "Game/Player/PlayerManager.h"
#include "include_asm.h"
extern "C" int sprintf(char* s, const char* fmt, ...);



#include "System/RsrcDB.h"






#include "System/Rcf.h"







#include "Game/Enemy/BattleEnemyManager.h"






#include "Game/Atel/Atel.h"





#include "System/Graphic/PackMan.h"
#include "System/HeapMan.h"





/* Game/Collision/CollUtil.h's dispose accessor (line 202) */
static inline void _collUtilDispose() {
    CCollUtil*& instance = CCollUtil::m_pInstance;
    if (!instance) AssertApp(COLLUTIL_H_PATH, 202, "m_pInstance", 0);
    instance->dispose();
}

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

enum _EETYPE {
    _EETYPE_NORMAL = 0,
    _EETYPE_BLACK = 1,
    _EETYPE_WHITE = 2,
    _EETYPE_NO_FADE = 3
};

enum _EESEQ {
    _EESEQ_NOP = 0,
    _EESEQ_START = 1,
    _EESEQ_END = 2
};

class CEncountEffectTask : public CTask {
private:
    SHORT m_nStep;      /* 0x40 */
    SHORT _nDummy;      /* 0x42 */
    HANDLE m_hEffect;   /* 0x44 */
    CCamera* m_pCamera; /* 0x48 */
    BOOL m_bNowLoading; /* 0x4c */

public:
    CEncountEffectTask();
    virtual ~CEncountEffectTask();
    virtual void main();

private:
    void setEffect();
    BOOL checkEffect();
};

template <class T> static inline T& _ref(T& v) {
    return v;
}

namespace {
_EETYPE s_nEnType;
_EESEQ s_nEnEffSeq;
}

CGameSubBattle::CGameSubBattle() : CGameSub((CStateTemplate<CGameSub>*)m_State) {
    m_hLevelUp = 0;
    m_State[0].set(this, &CGameSubBattle::preLoadField, &CGameSubBattle::loadField, NULL);
    m_State[1].set(this, &CGameSubBattle::preWaitEncountEffect, &CGameSubBattle::waitEncountEffect, NULL);
    m_State[2].set(this, &CGameSubBattle::preSetupAtel, &CGameSubBattle::setupAtel, NULL);
    m_State[3].set(this, &CGameSubBattle::preSetupTasks, &CGameSubBattle::setupTasks, NULL);
    m_State[4].set(this, &CGameSubBattle::preWaitStartAtel, &CGameSubBattle::waitStartAtel, NULL);
    m_State[5].set(this, &CGameSubBattle::prePlay, &CGameSubBattle::playing, NULL);
    m_State[6].set(this, &CGameSubBattle::preToField, &CGameSubBattle::toField, NULL);
    m_State[7].set(this, &CGameSubBattle::preToFieldDispose, &CGameSubBattle::toFieldDispose, NULL);
    m_State[8].set(this, &CGameSubBattle::preLevelUp, &CGameSubBattle::levelUp, NULL);
    m_State[9].set(this, &CGameSubBattle::preSleep, &CGameSubBattle::sleeping, NULL);
    changeState(0);
}

CGameSubBattle::~CGameSubBattle() {
    CSceneInfo::removeCurrentBattleSceneRsrc();
}

INT CGameSubBattle::getGameSubState() const {
    switch (getCurrentStateID()) {
    case 2:
    case 3:
    case 4:
        return 1;
    case 5:
        return 2;
    case 9:
        return 6;
    }
    return 0;
}

BOOL CGameSubBattle::reqChangeGameSubState(INT nState) {
    INT nNext = -1;
    switch (getCurrentStateID()) {
    case 9:
        if (nState == 3) nNext = 1;
        break;
    case 4:
        if (nState == 2) nNext = 5;
        break;
    case 5:
        if (nState == 4) nNext = 6;
        break;
    }
    if (nNext < 0) {
        return FALSE;
    }
    changeState(nNext);
    return TRUE;
}

void CGameSubBattle::terminate() {
    TRACE_COL((TtyColor)31, "[CGameSubBattle]: --- Terminate ---\n");
    disposeTasks();
}

void CGameSubBattle::preLoadField() {
    INT nBattleScene;
    INT nCacheID;
    INT nMapID;
    BOOL bOnCache;
    CMainApp::setFpsFull();
    if (CSceneInfo::getCurrentWorldID(1) != 10) {
        PLM::mallocBattleRsrcBuffer(0);
    }
    INT nArea = CSceneInfo::getCurrentAreaNum();
    nBattleScene = CSceneInfo::getCurrentBattleSceneNum();
    if (nBattleScene > 0) {
        PLM::setReqLoadPlayer(0);
        AREA_SCENE as(nArea, nBattleScene);
        nCacheID = CSceneInfo::getSceneCacheID(as);
        if (nCacheID < 0) {
            nCacheID = CSceneInfo::addToSceneCache(as);
        }
        if (CSceneInfo::getCurrentSceneCacheID() != nCacheID) {
            CRsrcDB::setSceneAttachMode(nCacheID);
        }
        nMapID = CRcf::getMapID(as._nArea, as._nScene);
        setMapID(nMapID);
        bOnCache = CSceneInfo::checkMapRsrcCache(nMapID, 0) >= 0;
        CSceneInfo::updateMapRsrcCache(nMapID, 2);
        if (!bOnCache && nMapID) {
            TRACE_USER_COL((USER)4, (TtyColor)34, ">>> LOAD MAP-RSRC [[[%d]]]\n", nMapID);
            CRcf::reqLoadRCF((UINT)nMapID / 10000, (UINT)nMapID % 10000, 0x100, NULL, 0);
        }
        CRcf::reqLoadScene(as._nArea, as._nScene);
        if ((UINT)((SHORT)nBattleScene - 1050) >= 149) {
            CNowLoading::start();
        }
    }
}

void CGameSubBattle::loadField() {
    if (CRcf::isCompleteLoadRCF(0, 0)) {
        INT nBattle = CSceneInfo::getCurrentBattleSceneNum();
        if (CSceneInfo::getCurrentSceneNum() != nBattle) {
            CRsrcDB::clearSceneAttachMode();
        }
        changeState(9);
        CNowLoading::end(0);
    }
}

void CGameSubBattle::preWaitEncountEffect() {
    if (!CSceneInfo::isMinigameScene()) {
        PLM::setReqLoadBattlePlayer(0);
    }
}

static CHAR* s_pszBgmTbl[14] = {
    NULL,          "01F_Town_B",     "26_Aladdin_B", "03F_Hercules_B", "21_Alice_Battle", "27_Pinocchio_B", "18_Helloween_B",
    "39_LittleMermaid_B", "23_PeterPan_Battle", "43_Hollow_B", NULL, "31_Twilight_B", "33_Destiny_B", "29_Forget_B",
};

static void _setBattleBGM() {
    WORLD_ID w;
    CHAR* pszBgm;
    if (CSceneInfo::isBossBattleScene(-1) || CGameWork::getFlag(1, 0x32)) {
        return;
    }
    w = (WORLD_ID)CSceneInfo::getCurrentWorldID(0);
    pszBgm = (w < 14) ? s_pszBgmTbl[w] : NULL;
    if (pszBgm) {
        CSound::crossFadeinBGM(pszBgm, 40, -1, 0);
    }
}

void CGameSubBattle::waitEncountEffect() {
    if (CEncountEffect::isStart()) {
        if (!CEncountEffect::isEnd()) {
            return;
        }
    } else {
        _setBattleBGM();
    }
    CEncountEffect::reset();
    changeState(2);
}

void CGameSubBattle::preSetupAtel() {
    CMainApp::setFpsHalf();
    void** pMCD = (void**)CRsrcDB::getRsrcMCD(getMapID());
    if (pMCD) {
        CCollUtil::initializeData(*pMCD, NULL);
    }
    CGameSub::preSetupAtel();
    if (s_nEnType == _EETYPE_NORMAL || s_nEnType == _EETYPE_WHITE) {
        CGame::setWhite();
    }
}

void CGameSubBattle::setupAtel() {
    if (CGameSub::setupAtel()) {
        changeState(3);
    }
}

void CGameSubBattle::preSetupTasks() {
    CGameWork::setFlag(1, 0x2D, 0);
}

#line 350
void CGameSubBattle::setupTasks() {
    CTask* pGame = CTaskMan::getSysTask(1);
    CMap* pMap = new (__FILE__ "(352)") CMap(pGame, CRcf::getMapID(CSceneInfo::getCurrentAreaNum(), CSceneInfo::getCurrentSceneNum()));
    ASSERT(pMap);
    Message::CMessageManager::getInstance();
    LockOnManager::CLockOnManager::getInstance();
    CEntityFactory::getInstance();
    new (__FILE__ "(365)") CGimmickManager(pGame);
    if (!CSceneInfo::isMinigameScene()) {
        new (__FILE__ "(370)") CBattleEnemyManager(pGame);
        new (__FILE__ "(373)") CBulletManager(pGame);
        new (__FILE__ "(376)") PLM::CPlayerManager(pGame);
        PLM::createPlayer(0, NULL, 0);
    } else {
        new (__FILE__ "(384)") CMinigameManager(pGame);
    }
    pMap->init(1, 1);
    CShadowModelMan::setAutomatic();
    CShadowModelMan::setUpPer(0.5f);
    CShadowModelMan::setDownLen(2.8f);
    CDeFocus::create(pGame);
    CDeFocus::offNear();
    CCollDisp::initialize(pGame, dbgButtonDown(USR_SYSTEM, 0x100, 1) | CCollDisp::m_fgDisp, CCollDisp::m_fgMapZoneDisp);
    changeState(4);
}

void CGameSubBattle::preWaitStartAtel() {
    m_nStep = 2;
    CWorldEnv::readyMike(1, 1);
}

void CGameSubBattle::waitStartAtel() {
    if (m_nStep > 0) {
        if (--m_nStep == 0) {
            if (!CGameWork::getFlag(1, 4) && !CSceneInfo::isMinigameScene()) {
                CGameWork::setInt(1, 0xE2, 0);
            }
            startAtel();
            if (s_nEnType == _EETYPE_NORMAL || s_nEnType == _EETYPE_WHITE) {
                CGame::reqWhiteFadeIn(20);
            }
        }
    } else if (CGame::isFadeDone()) {
        changeState(5);
    }
}

void CGameSubBattle::prePlay() {
    if (!CWorldEnv::isReadyMike()) {
        CWorldEnv::readyMike(1, 0);
    }
    CSceneInfo::doneChangeScene();
}

void CGameSubBattle::playing() {}

void CGameSubBattle::preToField() {
    CGame::reqBlackFadeOut(20);
    if (CGameWork::getFlag(1, 0x2D) || CGameWork::getFlag(1, 0x32)) {
        CSound::fadeoutAllSE(20, 0);
    } else {
        CSound::fadeoutAllBGM(20);
        CSound::fadeoutAllSE(20, 1);
    }
}

void CGameSubBattle::toField() {
    if (CGame::isFadeDone()) {
        changeState(7);
    }
}

void CGameSubBattle::preToFieldDispose() {
    m_nStep = 0;
}

void CGameSubBattle::toFieldDispose() {
    if (m_nStep == 0) {
        disposeTasks();
        m_nStep++;
        return;
    }
    if (CSceneInfo::isMinigameScene() && PLM::getBattleRsrcFirst()) {
        PLM::freeBattleRsrcBuffer();
    }
    if (CGameWork::getFlag(1, 0x2D)) {
        CWorldEnv::setFogColor(0, 0, 0);
        changeState(8);
    } else {
        CWorldEnv::readyMike(0, 0);
        changeState(9);
    }
}

void CGameSubBattle::preLevelUp() {
    m_nStep = 0;
}

#line 558
void CGameSubBattle::levelUp() {
    switch (m_nStep) {
    case 0:
        CGame::dispOffFadeMask();
        m_nStep++;
        break;
    case 1: {
        CLevelUp* p = new (__FILE__ "(565)") CLevelUp(CTaskMan::getSysTask(1), 0);
        ASSERT(p);
        m_hLevelUp = p->getHandle();
        m_nStep++;
        break;
    }
    default:
        if (!CTaskMan::getTask(m_hLevelUp)) {
            m_hLevelUp = 0;
            CWorldEnv::readyMike(0, 0);
            changeState(9);
        }
        break;
    }
}

void CGameSubBattle::preSleep() {}

void CGameSubBattle::sleeping() {}

void CGameSubBattle::disposeTasks() {
    CTask* pTask;
    exitAtel();
    PLM::destroyPlayer();
    if ((pTask = CTaskMan::getSysTask(2))) pTask->out();
    if ((pTask = CTaskMan::getSysTask(31))) pTask->out();
    if ((pTask = CTaskMan::getSysTask(4))) pTask->out();
    if ((pTask = CTaskMan::getSysTask(17))) pTask->out();
    if ((pTask = CTaskMan::getSysTask(25))) pTask->out();
    if ((pTask = CTaskMan::getSysTask(27))) pTask->out();
    if ((pTask = CTaskMan::getSysTask(33))) pTask->out();
    if ((pTask = CTaskMan::getSysTask(32))) pTask->out();
    if ((pTask = CTaskMan::getSysTask(23))) pTask->out();
    CWorldEnv::removeAllCamera();
    CDeFocus::remove();
    CEffectMan::killAll((ObjType::eObjBit)0xFFFFFFFF, 0.0f);
    D2::C2DSpr::destroyAllForSceneChange();
    CCollDisp::finalize();
    _collUtilDispose();
}

#line 738
CEncountEffectTask::CEncountEffectTask() : CTask(CTask::getSysTask(0), 0x4000) {
    m_nStep = 0;
    m_hEffect = 0;
    m_pCamera = NULL;
    if (!(_EESEQ_NOP == _ref(s_nEnEffSeq))) AssertApp(__FILE__, __LINE__, "_EESEQ_NOP == s_nEnEffSeq", 0);
    s_nEnEffSeq = _EESEQ_START;
    m_bNowLoading = CNowLoading::isEnable();
    if (m_bNowLoading) {
        CNowLoading::enable(FALSE);
    }
    if (CSceneInfo::hasReqEnterBattle() == 1) {
        if (CGameWork::getFlag(1, 0x25)) {
            s_nEnType = CAtel::isFadeActive() ? _EETYPE_NO_FADE : _EETYPE_WHITE;
            CGameWork::setFlag(1, 0x25, 0);
        } else {
            s_nEnType = _EETYPE_NORMAL;
        }
    } else {
        s_nEnType = _EETYPE_BLACK;
    }
}

#line 763
CEncountEffectTask::~CEncountEffectTask() {
    if (!(_EESEQ_START == _ref(s_nEnEffSeq))) AssertApp(__FILE__, __LINE__, "_EESEQ_START == s_nEnEffSeq", 0);
    _ref(s_nEnEffSeq) = _EESEQ_END;
    if (m_bNowLoading) {
        CNowLoading::enable(TRUE);
    }
    if (!CGame::isBlackBeforeEncount()) {
        FVECTOR vColor;
        vColor.set(1.0f, 1.0f, 1.0f, 1.0f);



        CSceneEnv::setColor((ObjType::eObjBit)0xFFFEFFFF, 0.0f, vColor, NULL, 0);
    }
    _setBattleBGM();
}

#line 783
void CEncountEffectTask::main() {
    switch (m_nStep) {
    case 0:
        if (CGame::isBlackBeforeEncount()) {
            CWorldEnv::setFogColor(0, 0, 0);
        } else {
            CGameWork::setInt(1, 0xE2, -1);
            CPackMan::changeDrawMode(1);
        }
        m_nStep = 1;
        break;
    case 1:
        CGame::dispOffFadeMask();
        m_pCamera = CWorldEnv::getActiveCamera();
        if (!m_pCamera) {
            m_pCamera = new (__FILE__ "(807)") CCamera(this, -1);
        }
        if (CGame::isBlackBeforeEncount()) {
            m_pCamera->setProjectionDist(0.0f);
        } else {
            FVECTOR vColor;
            CEffectMan::killAll((ObjType::eObjBit)0xFFFFFFFF, 0.0f);
            vColor.x = 1.0f;
            vColor.y = 1.0f;
            vColor.z = 1.0f;
            vColor.w = 1.0f;
            CSceneEnv::setColor((ObjType::eObjBit)0xFFFEFFFF, 0.0f, vColor, NULL, 0);
        }
        m_nStep = 2;
        break;
    case 2:
        if (!CGame::isBlackBeforeEncount()) {
            FVECTOR vColor;
            vColor._q = 0;
            vColor.w = 1.0f;
            FLOAT fTime = CSceneInfo::isEventScene() ? 30.0f : 60.0f;
            CSceneEnv::setColor((ObjType::eObjBit)(INT)0x80000000, fTime, vColor, NULL, 0);
            CPackMan::enableDraw(4);
        }
        switch (s_nEnType) {
        case _EETYPE_BLACK:
            CGame::reqBlackFadeOut(30);
            break;
        case _EETYPE_WHITE:
            CGame::reqWhiteFadeOut(30);
            break;
        case _EETYPE_NO_FADE:
            break;
        default:
            setEffect();
            break;
        }
        m_nStep = 3;
        break;
    case 3: {
        BOOL bEnd;
        switch (s_nEnType) {
        case _EETYPE_BLACK:
        case _EETYPE_WHITE:
            bEnd = CGame::isFadeDone();
            break;
        case _EETYPE_NO_FADE:
            bEnd = TRUE;
            break;
        default:
            bEnd = checkEffect();
            if (!bEnd) {
                return;
            }
            CGame::setWhite();
            break;
        }
        if (bEnd) {
            m_nStep = 5;
        }
        break;
    }
    case 4:
        break;
    case 5:
        if (m_hEffect) {
            CEffectMan::killEffect(m_hEffect, 0.0f);
        }
        out();
        CPackMan::changeDrawMode(0);
        break;
    }
}

#line 866
void CEncountEffectTask::setEffect() {
    FVECTOR vPos;
    FVECTOR vRot;
    BOOL bReadyMike;
    if (m_hEffect) {
        CEffectMan::killEffect(m_hEffect, 0.0f);
    }
    ASSERT(m_pCamera);
    vPos.x = 0.0f;
    vPos.y = 0.0f;
    vPos.z = 2.15f;
    vPos.w = 1.0f;
    vRot.set(0.0f, 3.1415927f, 0.0f);
    m_hEffect = CEffectMan::attachEffect("encount", 0, (ObjType::eObjType)30, &m_pCamera->Crd(0), &vPos, &vRot, 0.0f);
    bReadyMike = CWorldEnv::isReadyMike();
    if (!bReadyMike) {
        CWorldEnv::readyMike(1, 0);
    }
    CSound::playSE(40, -1, 64);
    if (!bReadyMike) {
        CWorldEnv::readyMike(0, 0);
    }
}

BOOL CEncountEffectTask::checkEffect() {
    CEffect* pEff;
    if (m_hEffect && (pEff = CEffectMan::getEffect(m_hEffect))) {
        return pEff->isEndFrame();
    }
    return TRUE;
}

void CEncountEffect::initialize() {
    s_nEnEffSeq = _EESEQ_NOP;
    s_nEnType = _EETYPE_NORMAL;
    CGame::setBlackBeforeEncount(0);
}

#line 924
void CEncountEffect::start() {
    if (s_nEnEffSeq == _EESEQ_NOP) {
        new (__FILE__ "(925)") CEncountEffectTask;
    }
}

void CEncountEffect::reset() {
    s_nEnEffSeq = _EESEQ_NOP;
}

BOOL CEncountEffect::isStart() {
    return s_nEnEffSeq != _EESEQ_NOP;
}

BOOL CEncountEffect::isEnd() {
    return s_nEnEffSeq == _EESEQ_END;
}
// SOURCE DATA: .data .rodata .bss
