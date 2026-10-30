#include "include_asm.h"
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "System/Task.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtilA.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in GimmickCarry); which header it
 * comes from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

#include "System/Debug.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/TaskMan.h"
#include "System/PadHandler.h"
#include "Game/GameWork.h"
#include "Game/SceneInfo.h"
#include "Game/SceneEnv.h"
#include "Form/Menu/Pause.h"
#include "Game/Player/PlayerManager.h"
#include "Game/MinigameManager.h"

/* The 100 Acre Wood minigames live in a DLL (../Dlls/Minigame100Acre/). Their headers leave statics in .data/.bss
 * and the strings of their inline functions' asserts in .rodata; the classes themselves are only constructed
 * here (the original calls address 0). */
template <class T>
void LIMIT_RANGE(T& a, T min, T max) {
    if (a > max) {
        a = max;
    } else if (a < min) {
        a = min;
    }
}

namespace Mini100AcreWildDive {
static INT MOT_SORA_IDLE = 0;
static INT MOT_SORA_RIGHT = 1;
static INT MOT_SORA_LEFT = 2;
static INT MOT_SORA_DAMAGE = 3;
static INT MOT_SORA_ROLL_L = 4;
static INT MOT_SORA_ROLL_R = 5;
static INT MOT_POO_IDLE = 62;
static INT MOT_POO_RIGHT = 63;
static INT MOT_POO_LEFT = 64;
static INT MOT_POO_DAMAGE = 65;
static INT MOT_POO_ONE_IDLE = 71;
static FLOAT PLAYER_POS_X_MIN = -3.8f;
static FLOAT PLAYER_POS_X_MAX = 3.8f;
static FLOAT PLAYER_POS_Z_MIN = -3.f;
static FLOAT PLAYER_POS_Z_MAX = 3.f;
static FLOAT PLAYER_RADIUS = 0.5f;
static FLOAT PLAYER_OFFSET_X = 0.f;
static FLOAT PLAYER_OFFSET_Y = 0.f;
static FLOAT PLAYER_OFFSET_Z = 0.f;
static FLOAT POO_RADIUS = 0.7f;
static LPCSTR s_CockpitSeqName = "mgame_po_cmd00:00";
static LPCSTR s_EffectName = "rec_po_minigame";
static FLOAT POO_POS_X_MIN = PLAYER_POS_X_MIN + 0.35f;
static FLOAT POO_POS_X_MAX = PLAYER_POS_X_MAX - 0.35f;
static FLOAT POO_POS_Z_MIN = PLAYER_POS_Z_MIN + 0.5f;
static FLOAT POO_POS_Z_MAX = PLAYER_POS_Z_MAX - 0.5f;
static FLOAT PRIZE_POS_X_MIN = PLAYER_POS_X_MIN + 1.2f;
static FLOAT PRIZE_POS_X_MAX = PLAYER_POS_X_MAX - 1.2f;
static FLOAT PRIZE_POS_Z_MIN = PLAYER_POS_Z_MIN + 1.2f;
static FLOAT PRIZE_POS_Z_MAX = PLAYER_POS_Z_MAX - 1.2f;
static const FVECTOR s_vStartPos = {{0.f, 150.f, 1.f, 1.f}};
static const FVECTOR s_vEndPos = {{0.f, 141.25f, 0.f, 1.f}};
static FLOAT s_fFogEndDisY = s_vStartPos.y - s_vEndPos.y;
} // namespace Mini100AcreWildDive

#line 1 "../Dlls/Minigame100Acre/Mini100AcreWildDive.h"
inline void _wdPlaceholder(INT state) {
    if (!((0 <= state) && (state < 1))) AssertApp(__FILE__, __LINE__, "(0 <= state) && (state < WD_MAIN_STATE_MAX)", 0);
    if (!((0 <= state) && (state < 2))) AssertApp(__FILE__, __LINE__, "(0 <= state) && (state < WD_PLAYER_STATE_MAX)", 0);
    if (!((0 <= state) && (state < 3))) AssertApp(__FILE__, __LINE__, "(0 <= state) && (state < WD_POO_STATE_MAX)", 0);
}
#line 1 "../Dlls/Minigame100Acre/Mini100AcreBalloonGlider.h"
inline void _blPlaceholder(INT state) {
    if (!((0 <= state) && (state < 1))) AssertApp(__FILE__, __LINE__, "(0 <= state) && (state < BL_MAIN_STATE_MAX)", 0);
    if (!((0 <= state) && (state < 2))) AssertApp(__FILE__, __LINE__, "(0 <= state) && (state < BL_PLAYER_STATE_MAX)", 0);
    if (!((0 <= state) && (state < 3))) AssertApp(__FILE__, __LINE__, "(0 <= state) && (state < BL_POO_STATE_MAX)", 0);
    if (!((0 <= state) && (state < 4))) AssertApp(__FILE__, __LINE__, "(0 <= state) && (state < BL_OWL_STATE_MAX)", 0);
    if (!((0 <= state) && (state < 5))) AssertApp(__FILE__, __LINE__, "(0 <= state) && (state < BL_BEEOBJ_STATE_MAX)", 0);
    if (!((0 <= state) && (state < 6))) AssertApp(__FILE__, __LINE__, "(0 <= state) && (state < BL_BALLOONOBJ_STATE_MAX)", 0);
}
#line 1 "../Dlls/Minigame100Acre/Mini100AcreTiggerJump.h"
inline void _tjPlaceholder(INT state, FLOAT& f) {
    if (!((0 <= state) && (state < 1))) AssertApp(__FILE__, __LINE__, "(0 <= state) && (state < TJ_MAIN_STATE_MAX)", 0);
    if (!((0 <= state) && (state < 2))) AssertApp(__FILE__, __LINE__, "(0 <= state) && (state < TJ_PLAYER_STATE_MAX)", 0);
    if (!((0 <= state) && (state < 3))) AssertApp(__FILE__, __LINE__, "(0 <= state) && (state < TJ_TIGGER_STATE_MAX)", 0);
    LIMIT_RANGE<float>(f, 0.f, 1.f);
}

#line 40 "src/Game/MinigameManager.cpp"
#define DLL_MINIGAME(name, size)                         \
    class name : public CMiniGameBase {                  \
        BYTE _pad[size - sizeof(CMiniGameBase)];         \
    public:                                              \
        name(CTask* p);                                  \
    }

DLL_MINIGAME(CMini100AcreVegPanicMain, 0x170);
DLL_MINIGAME(CMini100AcreBalloonGlider, 0x190);
DLL_MINIGAME(CMini100AcreTiggerJump, 0x380);
DLL_MINIGAME(CMini100AcreWildDive, 0x1C0);
DLL_MINIGAME(CMini100AcreBeeBattleMain, 0x150);

/* Ps2EeAs makes undefined weak symbols global (the link would fail), so define them as weak absolute 0: the
 * same "jal 0" as the original. */
__asm__(
    ".weak __24CMini100AcreVegPanicMainP5CTask\n__24CMini100AcreVegPanicMainP5CTask = 0\n"
    ".weak __25CMini100AcreBalloonGliderP5CTask\n__25CMini100AcreBalloonGliderP5CTask = 0\n"
    ".weak __22CMini100AcreTiggerJumpP5CTask\n__22CMini100AcreTiggerJumpP5CTask = 0\n"
    ".weak __20CMini100AcreWildDiveP5CTask\n__20CMini100AcreWildDiveP5CTask = 0\n"
    ".weak __25CMini100AcreBeeBattleMainP5CTask\n__25CMini100AcreBeeBattleMainP5CTask = 0\n"
);

#include "System/Sound/SoundMan.h"
#line 130

#include "Form/Menu/JMemoFlag.h"




class CRsrcDB {
public:
    static BOOL hasRsrcGroup(INT nKind, INT nGroup);
    static void removeRsrc(INT nID);
};

namespace PLM {
CPlayer* createPlayer(UINT nKind, CTask* pParent, INT nParam);
}

inline void* operator new(size_t, void* p) throw() { return p; }
#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

CMiniGameBase* CMinigameManager::m_pGame = NULL;
HANDLE CMinigameManager::m_hHelp = 0;

CMinigameManager::CMinigameManager(CTask* inpParent) : CTask(inpParent, 0x4000) {
    m_nStep = 0;
    m_nNextScene = 0;
    m_nClearEventFlagIdx = -1;
    m_nClearNextScene = 0;
    m_hEndMenu = 0;
    m_uiPauseDisableFlag = 0;
    declareSysTask(0x21);
    TRACE("---- CMinigameManager ctor.\n");
    m_State[STATE_MAIN].set(this, &CMinigameManager::beginStateMain, &CMinigameManager::stateMain,
                            &CMinigameManager::endStateMain);
    m_State[STATE_HELP].set(this, &CMinigameManager::beginStateHelp, &CMinigameManager::stateHelp,
                            &CMinigameManager::endStateHelp);
    m_State[STATE_END_MENU].set(this, &CMinigameManager::beginStateEndMenu, &CMinigameManager::stateEndMenu,
                                &CMinigameManager::endStateEndMenu);
    initialize();
    changeState(STATE_MAIN);
}

CMinigameManager::~CMinigameManager() {
    m_pGame = NULL;
    m_hHelp = 0;
    m_hEndMenu = 0;
    m_uiPauseDisableFlag = 0;
    _enablePause(0, TRUE);
    TRACE("---- CMinigameManager dtor.\n");
}

void CMinigameManager::changeState(INT inState) {
#line 111
    if (!(inState >= 0 && inState < STATE_MAX)) AssertApp(__FILE__, __LINE__, "inState >= 0 && inState < STATE_MAX", 0);
    gotoState(m_State[inState]);
}

BOOL CMinigameManager::isMainState() {
    return isState(m_State[STATE_MAIN]);
}

void CMinigameManager::main() {
    update();
}

void CMinigameManager::initialize() {
#line 133
    if (!(__null == m_pGame)) AssertApp(__FILE__, __LINE__, "__null == m_pGame", 0);
    switch (getMinigameType()) {
    case 0:
#line 138
        m_pGame = new (com_malloc_d(sizeof(CMini100AcreVegPanicMain), __FILE__ "(138)", RET_ADDR())) CMini100AcreVegPanicMain(this);
        m_nClearEventFlagIdx = 0xF;
        m_nClearNextScene = 0xA29;
        break;
    case 1:
#line 144
        m_pGame = new (com_malloc_d(sizeof(CMini100AcreBalloonGlider), __FILE__ "(144)", RET_ADDR())) CMini100AcreBalloonGlider(this);
        m_nClearEventFlagIdx = 0xB;
        m_nClearNextScene = 0x899;
        break;
    case 2:
#line 150
        m_pGame = new (com_malloc_d(sizeof(CMini100AcreTiggerJump), __FILE__ "(150)", RET_ADDR())) CMini100AcreTiggerJump(this);
        m_nClearEventFlagIdx = 0xE;
        m_nClearNextScene = 0x9C5;
        break;
    case 3:
#line 156
        m_pGame = new (com_malloc_d(sizeof(CMini100AcreWildDive), __FILE__ "(156)", RET_ADDR())) CMini100AcreWildDive(this);
        m_nClearEventFlagIdx = 0xC;
        m_nClearNextScene = 0x8FD;
        break;
    case 4:
#line 162
        m_pGame = new (com_malloc_d(sizeof(CMini100AcreBeeBattleMain), __FILE__ "(162)", RET_ADDR())) CMini100AcreBeeBattleMain(this);
        m_nClearEventFlagIdx = 0xD;
        m_nClearNextScene = 0x961;
        break;
    }
    if (m_pGame) {
        _enablePause(0, FALSE);
        CMainApp::setFpsFull();
        m_pGame->initialize();
        CGameWork::setByte(1, 0x141, 2);
    } else {
        CTask* pGame = CTaskMan::getSysTask(1);
#line 181
        new (com_malloc_d(0x600, __FILE__ "(181)", RET_ADDR())) PLM::CPlayerManager(pGame);
        PLM::createPlayer(0, NULL, 0);
        m_nClearEventFlagIdx = -1;
        m_nClearNextScene = 0;
    }
}

void CMinigameManager::beginStateMain() {
}

void CMinigameManager::stateMain() {
}

void CMinigameManager::endStateMain() {
}

void CMinigameManager::beginStateHelp() {
    m_nStep = 0;
    CSceneEnv::setStop(ObjType::OBJ_BIT_ALL);
    if (m_pGame) m_pGame->setActive(FALSE);
    _enablePause(1, FALSE);
    reqDispHelp(this, 0, TRUE);
}

void CMinigameManager::stateHelp() {
    if (isEndDispHelp()) changeState(STATE_MAIN);
}

void CMinigameManager::endStateHelp() {
    CSceneEnv::setStop(ObjType::OBJ_BIT_CLEAR);
    if (m_pGame) m_pGame->setActive(TRUE);
    _enablePause(1, TRUE);
}

void CMinigameManager::beginStateEndMenu() {
    m_nStep = 0;
    CSceneEnv::setStop(ObjType::OBJ_BIT_ALL);
    if (m_pGame) m_pGame->setActive(FALSE);
    _enablePause(2, FALSE);
    CSound::fadeoutAllSE(0x2D, TRUE);
}

void CMinigameManager::stateEndMenu() {
    switch (m_nStep) {
    case 0:
        m_nStep++;
        break;
    case 1: {
#line 257
        CTask* pTask = new (com_malloc_d(sizeof(CPause), __FILE__ "(257)", RET_ADDR())) CPause(this, 3);
        if (pTask) {
            m_hEndMenu = pTask->getHandle();
            m_nStep++;
        }
        break;
    }
    default:
        if (isEndMenu()) {
            switch (CPause::getResult()) {
            case 2:
                reqRetry();
                m_nStep++;
                break;
            case 3:
                reqFinish(m_nNextScene);
                m_nStep++;
                break;
            default:
                changeState(STATE_MAIN);
                break;
            }
        }
        break;
    }
}

void CMinigameManager::endStateEndMenu() {
}

INT CMinigameManager::getMinigameType() {
    INT nRet = -1;
    switch (CGameWork::getByte(1, 0x142)) {
    case 0:
        nRet = 0;
        break;
    case 1:
        nRet = 1;
        break;
    case 2:
        nRet = 2;
        break;
    case 3:
        nRet = 3;
        break;
    case 4:
        nRet = 4;
        break;
    }
    if (nRet != -1) {
        INT nOld = nRet;
        if (CSceneInfo::getCurrentAreaNum() == 10) {
            switch (CSceneInfo::getCurrentBattleSceneNum()) {
            case 0x4B1:
                if (nRet != 0) nRet = 0;
                break;
            case 0x4B2:
                nRet = 1;
                break;
            case 0x4B3:
                nRet = 2;
                break;
            case 0x4B4:
                if (nRet != 3 && nRet != 4) nRet = 3;
                if (CPadHandler::isButtonDown(0x800, 1)) {
                    CGameWork::setByte(1, 0x142, 4);
                    nRet = 4;
                    nOld = 4;
                }
                break;
            }
        } else {
            nRet = -1;
            TRACE("\x82P\x82O\x82O\x83G\x81[\x83J\x81[\x82\xcc\x90X\x88\xc8\x8aO\x82\xc5\x83~\x83j\x83Q\x81[\x83\x80\x82\xf0\x8bN\x93\xae\x82\xb5\x82\xe6\x82\xa4\x82\xc6\x82\xb5\x82\xc4\x82\xa2\x82\xdc\x82\xb7\x81" "B\n");
        }
        if (nRet != nOld) {
            TRACE("\x83~\x83j\x83Q\x81[\x83\x80\x8e\xed\x95\xca\x83\x8f\x81[\x83N (AGB_POOH_MINIGAME_KIND) \x82\xc9\x8c\xeb\x82\xc1\x82\xbd\x92l(%d)\x82\xaa\x83Z\x83" "b\x83g\x82\xb3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7\x81" "B\n", nOld);
        }
    }
    return nRet;
}

UINT CMinigameManager::sendEvent(UINT nEvent, void* pParam) {
    switch (nEvent) {
    case 1:
        TRACE("\x83~\x83j\x83Q\x81[\x83\x80\x81" "F\x81g\x82\xe2\x82\xdf\x82\xe9\x81h\x82\xaa\x92\xca\x92m\x82\xb3\x82\xea\x82\xdc\x82\xb5\x82\xbd\x81" "B\n");
        break;
    case 2:
        TRACE("\x83~\x83j\x83Q\x81[\x83\x80\x81" "F\x81g\x83\x8a\x83g\x83\x89\x83" "C\x81h\x82\xaa\x92\xca\x92m\x82\xb3\x82\xea\x82\xdc\x82\xb5\x82\xbd\x81" "B\n");
        break;
    }
    if (nEvent == 1) _reqFinish();
    if (m_pGame) {
        switch (nEvent) {
        case 1:
        case 2:
            CSound::fadeoutAllVoice(15);
            break;
        }
        return m_pGame->sendEvent(nEvent, pParam);
    }
    return 0;
}

CMinigameManager* CMinigameManager::getManager() {
    return (CMinigameManager*)CTaskMan::getSysTask(0x21);
}

void CMinigameManager::reqFinish(SHORT inNextScene) {
    if (m_pGame) {
        _reqFinish();
        CSceneInfo::reqExitMinigame(inNextScene);
    }
}

void CMinigameManager::_reqFinish() {
    CGameWork::setByte(1, 0x141, 3);
    INT nType = CGameWork::getByte(1, 0x142);
    if (nType < 5) {
        if (nType >= 0) {
            CJiminyMemoFlag::checkEvent(8, nType, 0);
            static int __tbl[10] = {220, 221, 230, 231, 240, 241, 250, 251, 260, 261};
            for (INT i = 0; i < sizeof(__tbl) / sizeof(__tbl[0]); i++) {
                INT nID = __tbl[i];
                if (CRsrcDB::hasRsrcGroup(2, nID)) CRsrcDB::removeRsrc(nID + 20000);
            }
        }
    }
}

void CMinigameManager::reqRetry() {
    if (m_pGame) CSceneInfo::reqRetryMinigame();
}

void CMinigameManager::reqEndMenu(SHORT inNextScene) {
    CMinigameManager* pGM = getManager();
    if (pGM && pGM->isMainState()) {
        pGM->m_nNextScene = inNextScene;
        pGM->changeState(STATE_END_MENU);
    }
}

BOOL CMinigameManager::isEndMenu() {
    CTask* pTask = CTaskMan::getTask(m_hEndMenu);
    if (pTask && !pTask->isOut()) return FALSE;
    m_hEndMenu = 0;
    return TRUE;
}

BOOL CMinigameManager::reqDispHelp(CTask* inParent, INT nLayer, BOOL inAppearSE) {
    if (!isEndDispHelp()) return TRUE;
    CMinigameManager* pGM = getManager();
    if (pGM && m_pGame) {
        if (!inParent && pGM->isMainState()) {
            pGM->changeState(STATE_HELP);
            return TRUE;
        }
        m_hHelp = m_pGame->doDispHelpScreen(inParent, nLayer, inAppearSE);
        if (m_hHelp) return TRUE;
    }
    return FALSE;
}

BOOL CMinigameManager::isEndDispHelp() {
    CTask* pTask = CTaskMan::getTask(m_hHelp);
    if (pTask && !pTask->isOut()) return FALSE;
    m_hHelp = 0;
    return TRUE;
}

BOOL CMinigameManager::isAlreadyClear() {
    CMinigameManager* pGM = getManager();
    if (pGM && m_pGame) {
        if (pGM->m_nClearEventFlagIdx >= 0) return CGameWork::getFlag(0, pGM->m_nClearEventFlagIdx);
        return FALSE;
    }
    return FALSE;
}

SHORT CMinigameManager::getClearNextSceneNo() {
    CMinigameManager* pGM = getManager();
    if (pGM && m_pGame) return pGM->m_nClearNextScene;
    return 0;
}

void CMinigameManager::enablePauseIdx(INT nIdx, BOOL inSw) {
    CMinigameManager* pGM = getManager();
    if (pGM && m_pGame) pGM->_enablePause(nIdx, inSw);
}

void CMinigameManager::_enablePause(INT nIdx, BOOL inSw) {
#line 543
    if (!(nIdx >= 0 && nIdx < PD_FLAG_IDX_MAX)) AssertApp(__FILE__, __LINE__, "nIdx >= 0 && nIdx < PD_FLAG_IDX_MAX", 0);
    if (inSw) {
        m_uiPauseDisableFlag &= ~(1 << nIdx);
    } else {
        m_uiPauseDisableFlag |= 1 << nIdx;
    }
    CGameWork::setFlag(1, 6, m_uiPauseDisableFlag);
}

CMiniGameBase::CMiniGameBase(CTask* p) : CTask(p, 0x4000) {
}

CMiniGameBase::~CMiniGameBase() {
    enablePause(TRUE);
}

void CMiniGameBase::main() {
    update();
}

void CMiniGameBase::initialize() {
}

HANDLE CMiniGameBase::doDispHelpScreen(CTask* inParent, INT inLayer, BOOL inAppearSE) {
    return 0;
}

UINT CMiniGameBase::sendEvent(UINT nEvent, void* pParam) {
    return 0;
}

namespace MG_SKILL {
HANDLE createMGSkill(INT inType, INT inLevel) {
    if (inType < 0 || inType >= 3) return 0;
    EVENT_PARAM wk;
    wk.nType = inType;
    wk.nLevel = inLevel;
    return CMinigameManager::sendEvent(3, &wk);
}

BOOL isMGSkillAlive() {
    return CMinigameManager::sendEvent(4, NULL) != 0;
}

void killMGSkill() {
    CMinigameManager::sendEvent(5, NULL);
}
} // namespace MG_SKILL

// SOURCE DATA: .data .rodata .bss
