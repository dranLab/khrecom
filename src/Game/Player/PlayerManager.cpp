#define GAME_PLAYER_PLAYERMANAGER_CPP /* MATCH: unit gate (its header gives this unit its own parts); without it the image differs (tested 2026-11-02) */
/* .rodata starts with the strings of the included headers' inline functions, in this order. */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/TaskMan.h"
#include "System/Task.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in Friend and Summon). */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}
inline void _emptyStringPlaceholder2() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}

/* MATCH: Placeholders for an unknown header's inline functions (prize messages) that come next in .rodata. */
inline void _premiumPrizeMessage() {
    TRACE("\x83v\x83\x8c\x83~\x83" "A\x83\x80\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}
inline void _roulettePrizeMessage() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("\x83\x8b\x81[\x83\x8c\x83" "b\x83g\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}

#include "System/Graphic/Effect/effect.h"
#include "Game/Player/PlayerManager.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/Player/Friend.h"
#include "Game/Player/FriendAladdin.h"
#include "Game/Player/FriendAriel.h"
#include "Game/Player/FriendBeast.h"
#include "Game/Player/FriendDonald.h"
#include "Game/Player/FriendGoofy.h"
#include "Game/Player/FriendJack.h"
#include "Game/Player/FriendKing.h"
#include "Game/Player/FriendPan.h"
#include "Game/Player/FriendPluto.h"
#include "Game/Player/Summon.h"
#include "Game/Player/SummonBambi.h"
#include "Game/Player/SummonCloud.h"
#include "Game/Player/SummonDumbo.h"
#include "Game/Player/SummonGenie.h"
#include "Game/Player/SummonMushu.h"
#include "Game/Player/SummonSinba.h"
#include "Game/Player/SummonTink.h"
#include "Game/SceneInfo.h"
#include "Game/Game.h"
#include "Game/GameWork.h"
#include "Game/Debug/DebugWork.h"
#include "Game/Collision/CollMapCamera.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/Enemy/MapEnemyManager.h"
#include "Game/SceneEnv.h"
#include "System/PlaySpeedCtrl.h"
#include "System/Graphic/PathCamera.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "System/HeapMan.h"
#include "System/VramMan.h"
#include "System/PadIO.h"
#include "System/Rnd.h"
#include "System/Math3D.h"
#include "System/Debug.h"
#include "Form/DebugWindow.h"
#include "Game/Player/DWTPlayer.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" float atan2f(float y, float x);

inline void* operator new(size_t, void* p) throw() { return p; }
static inline UINT RET_ADDR() {
    UINT nRA;
    __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA));
    return nRA;
}

#include "System/Rcf.h"






#include "System/RsrcDB.h"








class CHeapMan {
public:
    static float getUsedHeapRatio();
};

#include "Game/Atel/VSME.h"




#include "System/Sound/SoundMan.h"
#line 120

#include "System/Graphic/PackMan.h"



/* PARTIAL: stand-in for System/Graphic/ShadowModel CShadowModelMan: the ordering asm in the dead-player block reads the private m_iStatus */
class CShadowModelMan {
public:
    static UINT m_iStatus;
};

#include "Game/Enemy/BattleEnemyManager.h"




static inline void _setSceneColor(ObjType::eObjBit inBit, FLOAT inFrame, const FVECTOR* inColor) {
    CSceneEnv::setColor(inBit, inFrame, *inColor, inColor, 0);
}
static inline void _setSceneColor3(ObjType::eObjBit inBit, FLOAT inFrame, const FVECTOR* inColor, INT ch) {
    CSceneEnv::setColor(inBit, inFrame, *inColor, inColor, ch);
}

/* CCardBattle's first enchant card kind as a skill (sys task 0x16). */
static inline INT _getEnchantKind(CTask* pCB) {
    BYTE* pKind = (BYTE*)pCB + 0x17C;
    return *pKind < 0x44 ? 0 : *pKind - 0x43;
}

#include "Form/Menu/Camp.h"




#include "Form/Menu/JMemoFlag.h"




#include "Game/Player/PlayerSora.h"
#include "Game/Player/PlayerRiku.h"
#include "Game/Player/PlayerDRiku.h"












void ScaleVectorXYZ(FVECTOR& outV, const FVECTOR& inV, FLOAT inScale);
void AddVectorXYZ(FVECTOR& outV, const FVECTOR& inV0, const FVECTOR& inV1);
void DivVector(FVECTOR& outV, const FVECTOR& inV, FLOAT inDiv);

template <class T> static inline T& _ref(T& v) {
    return v;
}

static inline void offFlag(UINT& ioFlag, UINT flag) {
    ioFlag &= ~flag;
}

namespace PL {
extern FVECTOR g_vFieldPlayerPos;
}

static inline FLOAT _roundPI(FLOAT inR) {
    if (inR < -3.1415927f) {
        inR += 6.2831855f;
    } else if (3.1415927f < inR) {
        inR -= 6.2831855f;
    }
    return inR;
}

extern "C" {
void sceVu0UnitMatrix(FMATRIX& m);
void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
}
void ApplyMatrixXYZ(FVECTOR& outV, const FMATRIX& inM, const FVECTOR& inV);

namespace PLM {

static LPCSTR g_szCamPDead000 = "PDead000";
static LPCSTR g_szCamPDead001 = "PDead001";
static LPCSTR g_szCamPDead002 = "PDead002";
CPlayerManager* CPlayerManager::m_pInstance = NULL;
UINT g_uiPlayerID = 1;
UINT g_uiPlayerExID = 0;
UINT g_uiBattleCommonRCID = 1001;
BOOL g_bIsLastSceneBattle = FALSE;
UINT g_uiRsrcSizeTotal = 0x280000;
void* g_pRsrcFirst = NULL;

UINT g_nRCDataBlockSize[57] = {
    0, 0x29900, 0x15200, 0x80400, 0x79700, 0xCCC00, 0x1F800, 0x14200, 0x8F00, 0x1EA00, 0xD200, 0x13500, 0x5FB00, 0x63500, 0x6DE00, 0x2E300, 0x5A500, 0x54E00, 0x5C100, 0x27300, 0x6D600, 0x26100, 0x13F00, 0x16000, 0x1B800, 0xC000, 0xAD00, 0x5A400, 0xB1600, 0xE6700, 0x98E00, 0x86D00, 0x7E600, 0xBC900, 0xBE100, 0x12FE00, 0x9DB00, 0xF9400, 0x108A00, 0xDAB00, 0xDC600, 0x90300, 0x9CE00, 0x16D700, 0xA8400, 0x10CF00, 0x13E500, 0xBA00, 0xDB00, 0x11000, 0x11800, 0x13900, 0x18F00, 0x57000, 0x70C00, 0x43600, 0x9200
};
const UINT g_uiPlayerRcid[7] = {0, 1, 11, 101, 111, 201, 211};
const UINT g_uiFriendRcid[14] = {0, 401, 402, 403, 404, 405, 406, 407, 408, 409, 0, 451, 452, 458};
const UINT g_uiSummonRcid[8] = {0, 301, 305, 303, 302, 306, 304, 307};
static char* g_szRCDATA_DpxName[57] = {
    NULL,
    "finalbreak",
    "limit1",
    "Limit2",
    "limit4",
    "Limit5",
    "wild_crusher",
    NULL,
    "dark_faiga",
    NULL,
    "holy_burst",
    "holy_burst",
    "figa_burst",
    "raising_storm",
    "mega_flare",
    "freez",
    "aquasp",
    "magnespiral",
    "lethalframe",
    "shockimpact",
    "tornade",
    "quake",
    "bind",
    "confu",
    "terror",
    "synchro",
    "rec_m_gift",
    "holy",
    NULL,
    "guffy",
    "_al_5000",
    "lm_5030",
    NULL,
    "_pp_5000",
    "_pc_5000",
    "mm_miracle",
    "pluto",
    "_sum_simba_b",
    "genie_st_ed",
    "sum_banbi_b_add",
    "sum_dumbo_b",
    "_sum_tink_b",
    "_sum_mushu_b",
    "_sum_cloud",
    NULL,
    "guffy",
    "mm_miracle",
    "impals",
    "maelstrom",
    NULL,
    "impals",
    NULL,
    "dark_sword",
    "dark_sword",
    "impals",
    "maelstrom",
    "recom",
};

void RB_DATA::init() {
    nState = 0;
    nRCID = 0;
    nData = 0;
    pAddr = NULL;
    nSize = 0;
}

CPlayerManager::CPlayerManager(CTask* inParent) : CTask(inParent, 0x4000) {
    declareSysTask(0x1F);
    m_pInstance = this;
    init();
}

CPlayerManager::~CPlayerManager() {
    INT i;
    if (m_pCollMapCamera) delete m_pCollMapCamera;
    m_pCollMapCamera = NULL;
    if (m_pCB) delete (CTask*)m_pCB;
    m_pCB = NULL;
    if (_ref(g_pRsrcFirst)) {
        for (i = 0; i < 16; i++) {
            removeRsrcBlock(m_blockData[i]);
        }
    }
    m_pInstance = NULL;
}

INT CPlayerManager::removeRsrcBlock(RB_DATA& inData) {
    if (inData.nState == 2) {
        TRACE("PLM:: removeGroup = %d,", inData.nRCID);
        if (g_szRCDATA_DpxName[inData.nData]) {
            CEffectMan::killAll(g_szRCDATA_DpxName[inData.nData], 0.0f);
            TRACE("  kill Dpx = [ %s ],", g_szRCDATA_DpxName[inData.nData]);
        }
        TRACE("\n");
        CRsrcDB::removeGroup(1, inData.nRCID);
        inData.init();
        inData.pAddr = m_pRsrcEnd;
        return 1;
    }
    return 0;
}

void CPlayerManager::init() {
    INT worldID;
    INT roomID;
    INT nScene;
    INT i;

    m_vWind.init();
    m_uiFlag = 0;
    m_iStep = 0;
    m_tqTimer = 0;
    m_pPathCamera = NULL;
    m_pCollMapCamera = NULL;
    m_pCB = NULL;
    m_pPlayer = NULL;
    m_pPlayerEx = NULL;
    m_pFriend = NULL;
    m_pFriendEx = NULL;
    m_pSummon = NULL;
    m_hDeadSE = 0;
    m_pRsrcCurrent = NULL;
    m_pRsrcEnd = NULL;
    m_uiRemain = 0;
    m_uiCurrentBlock = 0;
    m_uiReserve = 0;
    worldID = CSceneInfo::getCurrentWorldID(0);
    roomID = CSceneInfo::getCurrentRoomID();
    nScene = CSceneInfo::getCurrentSceneNum();
    switch (worldID) {
    case 1:
        if (roomID != 0 && roomID != 99 && nScene != 11 && nScene != 14) m_uiFlag |= 0x80;
        break;
    case 2:
        if (nScene == 1050 || nScene == 1150) m_uiFlag |= 0x2000;
        break;
    case 4:
        if (nScene == 1050 || nScene == 1150) m_uiFlag |= 0x10000;
        break;
    case 6:
        if (roomID != 0 && roomID != 99) m_uiFlag |= 0x100;
        break;
    case 8:
        if (nScene == 1050 || nScene == 1150) m_uiFlag |= 0x4000;
        break;
    case 10:
        m_uiFlag |= 0x200;
        break;
    case 12:
        if (CGame::isRikuGame() && nScene == 1151) m_uiFlag |= 0x800;
        break;
    case 13:
        if (CGame::isSoraGame() && nScene == 1053) m_uiFlag |= 0x8000;
        break;
    case 22:
        if (nScene == 1025) m_uiFlag |= 0x8000;
        if (nScene == 1016) m_uiFlag |= 0x2000;
        if (CGame::isRikuGame() && nScene == 1023) m_uiFlag |= 0x800;
        break;
    }
    if (CSceneInfo::isBattleScene() || CSceneInfo::isMinigameScene()) {
        m_uiFlag |= 1;
        mallocBattleRsrcBuffer(TRUE);
        m_pRsrcCurrent = (BYTE*)_ref(g_pRsrcFirst);
        m_pRsrcEnd = (BYTE*)_ref(g_pRsrcFirst) + _ref(g_uiRsrcSizeTotal);
        m_uiRemain = _ref(g_uiRsrcSizeTotal);
        if (roomID != 0 && roomID != 99) g_bIsLastSceneBattle = TRUE;
        m_pCB = CardBattle::CCardBattle::getInstance();
    }
    initRsrcBlock();
    for (i = 0; i < 17; i++) {
        sprintf(m_strBlock[i], " ");
        DebugWork::g_dw.player.m_szBlock[i] = m_strBlock[i];
    }
    m_pWindow = new (com_malloc_d(0x1C0, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(701)", RET_ADDR()))
        CDebugWindow(this, DWTPlayer::debugPlayerBlockInfo);
    m_pWindow->dispOff(0);
}

void CPlayerManager::main() {
    INT i;
    if (m_uiFlag & 1) stateBattle();
    if (m_uiFlag & 4) procPlayerDead();
    exeWind();
    m_tqTimer -= getFrameRate();
    if (DebugWork::g_dw.player.iBlockInfo) {
        sprintf(m_strBlock[0], "Current:%d/16 Remain:%06X Reserve:%d/6", m_uiCurrentBlock, m_uiRemain, m_uiReserve);
        char* col[4] = {"\xf9S", "\xf9U", "\xf9X", "\xf9T"};
        for (i = 0; i < 16; i++) {
            RB_DATA& data = m_blockData[i];
            UINT size = getPlayerRsrcGrpSize(data.nRCID);
            sprintf(m_strBlock[i + 1], "%sS:%d D:%02d-%04d AD:%08X M:%06X/%06X", col[data.nState], data.nState,
                    data.nData, data.nRCID, data.pAddr, size, data.nSize);
        }
        m_pWindow->dispOn(0);
    } else {
        m_pWindow->dispOff(0);
    }
}

void CPlayerManager::stateBattle() {
    if (m_uiCurrentBlock < 16) {
        RB_DATA& data = m_blockData[m_uiCurrentBlock];
        if (data.nState == 1 && CRcf::isCompleteLoadRCF(0, 0)) {
            UINT size = getPlayerRsrcGrpSize(data.nRCID);
            UINT nSize = g_nRCDataBlockSize[data.nData];
            data.nState = 2;
            data.nSize = nSize;
            TRACE("PLM:: CompleteLoad : id=%d, size=0x%08x / 0x%08x\n", data.nRCID, size, data.nSize);
            if (data.nSize < size) {
                char buf[256];
                sprintf(buf, "\x8bZ\x97" "e\x97\xcaover!! id:%d  size:0x%08x / 0x%08x", data.nRCID, size, data.nSize);
                if (!(size <= data.nSize)) {
                    AssertApp("c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp", 780, "size<=data.nSize", buf);
                }
            }
            m_uiCurrentBlock++;
            m_pRsrcCurrent += data.nSize;
            m_uiRemain = m_pRsrcEnd - m_pRsrcCurrent;
            TRACE("PLM:: Current : block=%d, addr=0x%08x, remain=0x%08x\n", m_uiCurrentBlock, m_pRsrcCurrent, m_uiRemain);
            if (m_uiCurrentBlock >= 16) {
                TRACE("PLM:: \x81\xa6" "error!! Block Over : %d,\n", m_uiCurrentBlock);
                m_uiCurrentBlock = 0;
            }
            dbgDispTTY_RsrcBlock();
            setReserveLoad();
        }
        if (dbgCheckUser(KKIKUCHI) && dbgButtonDown(USR_SYSTEM, 4, 1) && dbgButtonTrg(USR_SYSTEM, 1, 1)) {
            dbgDispTTY_RsrcBlock();
        }
    }
}

void CPlayerManager::dbgDispTTY_RsrcBlock() {
    INT i;
    TRACE("PLM:: RB_DATA--------------------------------\n");
    TRACE("PLM:: CurrentBlock=%d, Remain=0x%06x\n", m_uiCurrentBlock, m_uiRemain);
    for (i = 0; i < 16; i++) {
        RB_DATA& data = m_blockData[i];
        UINT size = getPlayerRsrcGrpSize(data.nRCID);
        TRACE("%2d: state=%d, data=%d, rcid=%d, addr=0x%08x, size=0x%06x(0x%06x),\n", i, data.nState, data.nData,
              data.nRCID, data.pAddr, data.nSize, size);
    }
    TRACE("PLM:: --------------------------------RB_DATA\n");
}

} // namespace PLM

namespace PLM {

void CPlayerManager::procPlayerDead() {
    CPlayer* pPlayer;
    if (m_uiFlag & 0x400) {
        pPlayer = m_pPlayerEx;
    } else {
        pPlayer = m_pPlayer;
    }
    switch (m_iStep) {
    case 7:
        if (m_tqTimer > 0) break;
        CGame::reqWhiteFadeOut(1);
        CSceneEnv::setStop((ObjType::eObjBit)0x20006);
        CGameWork::setFlag(1, 6, 1);
        CGameWork::setFlag(1, 0x21, 1);
        CSound::invalidateSE(1);
        CSound::fadeoutAllSE(300, 1);
        if (m_hDeadSE > 0) {
            if (CGame::isSoraGame()) {
                CSound::setSEVolume(m_hDeadSE, 0x4B, 0x7F, 1);
            } else {
                CSound::setSEVolume(m_hDeadSE, 0x4C, 0x7F, 1);
            }
        }
        m_tqTimer = 250;
        m_iStep = 8;
        {
            CTask* pCB = CTaskMan::getSysTask(0x16);
            if ((pCB && _getEnchantKind(pCB) == 0x2E) || (dbgCheckUser(KKIKUCHI) && dbgButtonDown(USR_SYSTEM, 0x20, 1))) {
                m_uiFlag |= 0x10;
                CSound::setBGMVolume(CSound::getCurrentBGMHandle(), 0, 300);
            } else {
                offFlag(m_uiFlag, 0x10);
                CSound::fadeoutBGM(CSound::getCurrentBGMHandle(), 300);
            }
        }
    case 8:
        if (m_tqTimer > 0) break;
        if (!CGame::isFadeDone()) break;
        CGame::reqWhiteFadeIn(10);
        CPackMan::setFrameEffect(2, 0, 0, 0);
        CPlaySpeedCtrl::setPlaySpeed(0.1f);
        pPlayer->getPlayerCamera()->setFlag(0x20, 0);
        m_pPathCamera = new (com_malloc_d(0x110, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(892)", RET_ADDR()))
            CPathCamera(pPlayer);
        if (!m_pPathCamera) AssertApp("c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp", 893, "m_pPathCamera", 0);
        m_pPathCamera->setLocal(TRUE);
        m_pPathCamera->init(_ref(g_szCamPDead000), TRUE, FALSE, 0xC0);
        m_pPathCamera->initBGCheck(FALSE);
        m_pPathCamera->ignorePlaySpeed(FALSE);
        m_pPathCamera->run();
        if (!m_pCollMapCamera) {
            m_pCollMapCamera = new (com_malloc_d(0x30, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(901)", RET_ADDR()))
                CCollMapCamera();
        }
        if (m_pCollMapCamera) m_pCollMapCamera->init((COLL_KIND)0x8000000C, (COLL_TYPE)2, 0, getHandle());
        m_tqTimer = 500;
        m_iStep = 9;
        break;
    case 9:
        if (m_tqTimer > 0) break;
        CGame::reqWhiteFadeOut(100);
        m_iStep = 11;
        break;
    case 11:
        if (!CGame::isFadeDone()) break;
        CGame::reqWhiteFadeOut(100);
        m_iStep = 10;
        if (m_uiFlag & 0x10) {
            m_pCollMapCamera->off();
            m_pPathCamera->out();
            m_pPathCamera = NULL;
            pPlayer->getPlayerCamera()->setFlag(0, 0x20);
        } else {
            FVECTOR pos;
            pos.init();
            pPlayer->warp(pos);
            FVECTOR rot = pPlayer->getRot();
            rot.y = -1.5707964f;
            pPlayer->setRot(rot);
            pPlayer->setDir(rot.y);
            m_pPathCamera->setLocal(FALSE);
            m_pPathCamera->setPos(((const CPlayer*)pPlayer)->getPos());
            if (m_uiFlag & 8) {
                m_pPathCamera->init(g_szCamPDead002, TRUE, FALSE, 0xC0);
            } else {
                m_pPathCamera->init(g_szCamPDead001, TRUE, FALSE, 0xC0);
            }
            m_pPathCamera->stopBGCheck();
            m_pPathCamera->ignorePlaySpeed(FALSE);
            m_pPathCamera->run();
        }
        break;
    case 10:
        if (!CGame::isFadeDone()) break;
        CGame::reqWhiteFadeIn(60);
        CPackMan::setFrameEffect(0, 0, 0, 0);
        CPlaySpeedCtrl::setPlaySpeed(1.0f);
        CSound::invalidateSE(0);
        CSceneEnv::setStop((ObjType::eObjBit)0);
        if (m_uiFlag & 0x10) {
            CGameWork::setFlag(1, 0x21, 0);
            CGameWork::setFlag(1, 6, 0);
            pPlayer->notifyDead(1);
            CGame::recoverBGM(1);
            CGameWork::setInt(1, 0xE2, 0);
            offFlag(m_uiFlag, 4);
            m_iStep = 13;
        } else {
            FVECTOR vColor;
            vColor._q = 0;
            CShadowModelMan::m_iStatus |= 4;
            {
                /* stand-in: the original sets a3 after the m_iStatus store (gcc's sched1 puts `move a3,zero`
                   first); the asm's dummy memory input orders it, the $4 clobber keeps `li a0` for the delay slot */
                INT ch;
                __asm__("move %0, $0" : "=r"(ch) : "m"(CShadowModelMan::m_iStatus) : "$4");
                _setSceneColor3((ObjType::eObjBit)0x1FFC, 0.0f, &vColor, ch);
            }
            CSceneEnv::setStop((ObjType::eObjBit)0x1FFC);
            CEffectMan::killAll((ObjType::eObjBit)0x1FFC, 0.0f);
            CTask* pTask = CTaskMan::getSysTask(0x18);
            if (pTask) ((CMapEnemyManager*)pTask)->deleteEnemies();
            pTask = CTaskMan::getSysTask(0x19);
            if (pTask) ((CBattleEnemyManager*)pTask)->deleteAllEnemies();
            pTask = CTaskMan::getSysTask(0x11);
            if (pTask) ((CGimmickManager*)pTask)->deleteAllGimmicks();
            pPlayer->notifyDead(0);
            m_iStep = 12;
        }
        break;
    case 12:
        if (!CGame::isFadeDone()) break;
        CGameWork::setFlag(1, 0x21, 0);
        CGameWork::setFlag(1, 6, 0);
        CGameWork::setFlag(1, 0xA, 0);
        CGameWork::setFlag(1, 0xB, 0);
        if (m_uiFlag & 8) {
            CGame::reqGameOver(0);
        } else {
            CGame::reqGameOver(1);
        }
        m_iStep = 13;
        break;
    case 13:
        break;
    }
    if (m_pCollMapCamera && m_pPathCamera) {
        m_pCollMapCamera->setStartPos(m_pPathCamera->getGazePosition());
        m_pCollMapCamera->setEndPos(m_pPathCamera->getPosition());
        m_pCollMapCamera->collideMapTrueCamera(0);
    }
}

} // namespace PLM

namespace PLM {

void CPlayerManager::calcFieldStartPos() {
    FVECTOR& pos = PL::g_vFieldPlayerPos;
    if (CGameWork::getInt(0, 10)) {
        F_UL w;
        pos._ui[0] = CGameWork::getInt(0, 12);
        pos._ui[1] = CGameWork::getInt(0, 13);
        pos._ui[2] = CGameWork::getInt(0, 14);
        w.ul = CGameWork::getInt(0, 15);
        pos.w = w.f * 57.295776f;
        CGameWork::setInt(0, 10, 0);
        return;
    }
    if (g_bIsLastSceneBattle) {
        g_bIsLastSceneBattle = FALSE;
        return;
    }
    pos.set(0.0f, 0.0035f, 0.0f, 180.0f);
    CTask* pTask = CTaskMan::getSysTask(0x11);
    if (!pTask) return;
    DOOR_ID doorID = CSceneInfo::getEnteredDoorID();
    CEntity* pDoor = ((CGimmickManager*)pTask)->getDoor(doorID, NULL, NULL);
    if (!pDoor) return;
    pos.set(0.0f, 0.0f, 2.0f, 1.0f);
    DoorInfo* pDoorInfo = CSceneInfo::getDoorInfo(doorID, -1);
    if (pDoorInfo) {
        if (pDoorInfo->isWorldExit()) {
            pos.z = 6.5f;
        } else if (pDoorInfo->isWorldEntrance()) {
            pos.z = 3.0f;
        }
    }
    FMATRIX mat;
    FLOAT r;
    FLOAT rotY = ((const CEntity*)pDoor)->getRot().y;
    if (rotY < -3.1415927f) {
        r = rotY + 6.2831855f;
    } else {
        r = rotY;
        if (3.1415927f < r) r -= 6.2831855f;
    }
    sceVu0UnitMatrix(mat);
    sceVu0RotMatrixY(mat, mat, r);
    ApplyMatrixXYZ(pos, mat, pos);
    AddVectorXYZ(pos, pos, pDoor->getPos());
    pos.y += 0.0035f;
    pos.w = r / 3.1415927f * 180.0f;
}

} // namespace PLM

namespace PLM {

UINT CPlayerManager::getPlayerRsrcGrpSize(INT nMemberID) {
    return CRsrcDB::getRsrcGrpSizeOnMemory(1, nMemberID);
}

void CPlayerManager::setReqLoad(UINT inID, INT inReady) {
    UINT rc_data = getRCData_toSkill(inID);
    UINT rc_id;
    INT i;
    if (rc_data == 0) return;
    rc_id = getRCID_toRCData(rc_data);
    if (rc_id == 0) return;
    if (getRsrcBlock(rc_id) != -1) return;
    if (CRcf::isCompleteLoadRCF(0, 0)) {
        if (m_uiRemain < g_nRCDataBlockSize[rc_data]) {
            if (m_blockData[0].nState == 3) {
                TRACE("PLM:: Buff Over & Current Reset ... but Block[0] is Using ... goto stack\n");
                goto stack;
            }
            m_uiCurrentBlock = 0;
            m_pRsrcCurrent = (BYTE*)_ref(g_pRsrcFirst);
            m_uiRemain = _ref(g_uiRsrcSizeTotal);
            TRACE("PLM:: Buff Over & Current Reset : %d,\n", 0);
        }
        {
            RB_DATA& data = m_blockData[m_uiCurrentBlock];
            removeRsrcBlock(data);
            UINT nextBlock = m_uiCurrentBlock + 1;
            if (nextBlock < 16) {
                BYTE* addr = m_pRsrcCurrent + g_nRCDataBlockSize[rc_data];
                TRACE("PLM:: addr : 0x%x, 0x%x + 0x%x,\n", addr, m_pRsrcCurrent, g_nRCDataBlockSize[rc_data]);
                for (i = nextBlock; i < 16; i++) {
                    RB_DATA& next = m_blockData[i];
                    if (next.pAddr < addr) {
                        TRACE("PLM:: next->pAddr %d: 0x%x,", i, next.pAddr);
                        if (next.nState == 1 || next.nState == 3) {
                            TRACE("...stack (next->nState=%d)\n", next.nState);
                            break;
                        }
                        if (removeRsrcBlock(next)) {
                            TRACE("...init: 0x%x,\n", next.pAddr);
                        } else {
                            TRACE("\n");
                        }
                    }
                }
                if (i < 16) goto stack;
            }
            if (data.nState == 0) {
                data.nRCID = rc_id;
                data.nData = rc_data;
                data.nState = 1;
                data.pAddr = m_pRsrcCurrent;
                TRACE("PLM:: reqLoad : id=%d, addr=0x%08x,\n", data.nRCID, data.pAddr);
                CRcf::reqLoadRCF(1, data.nRCID, 0x40, data.pAddr, 0);
                return;
            }
        }
    }
stack:
    if (m_uiReserve < 5) {
        m_uiReserveBuf[m_uiReserve] = rc_data;
        m_uiReserve++;
        TRACE("PLM:: Reserve : data=%d %d, stack=%d\n", rc_data, rc_id, m_uiReserve);
    } else {
        TRACE("PLM:: \x81\xa6" "error!! Stack Over : data=%d %d,\n", rc_data, rc_id);
    }
}

void CPlayerManager::setReserveLoad() {
    UINT rc_data;
    UINT rc_id;
    INT i;
    if (m_uiReserve == 0) return;
    rc_data = m_uiReserveBuf[0];
    rc_id = getRCID_toRCData(rc_data);
    if (rc_id != 0 && getRsrcBlock(rc_id) == -1) {
        if (!CRcf::isCompleteLoadRCF(0, 0)) return;
        if (m_uiRemain < g_nRCDataBlockSize[rc_data]) {
            if (m_blockData[0].nState == 3) {
                TRACE("PLM:: setReserveLoad  Buff Over & Current Reset ... but Block[0] is Using ... return\n");
                return;
            }
            m_uiCurrentBlock = 0;
            m_pRsrcCurrent = (BYTE*)_ref(g_pRsrcFirst);
            m_uiRemain = _ref(g_uiRsrcSizeTotal);
            TRACE("PLM:: Buff Over & Current Reset : %d,\n", 0);
        }
        RB_DATA& data = m_blockData[m_uiCurrentBlock];
        removeRsrcBlock(data);
        if (m_uiCurrentBlock + 1 < 16) {
            BYTE* addr = m_pRsrcCurrent + g_nRCDataBlockSize[rc_data];
            for (i = m_uiCurrentBlock + 1; i < 16; i++) {
                RB_DATA& next = m_blockData[i];
                if (next.pAddr < addr) {
                    if (next.nState == 1) return;
                    if (next.nState == 3) return;
                    removeRsrcBlock(next);
                }
            }
        }
        if (data.nState != 0) return;
        data.nRCID = rc_id;
        data.nData = rc_data;
        data.nState = 1;
        data.pAddr = m_pRsrcCurrent;
        TRACE("PLM:: reqLoad : id=%d, addr=0x%08x,\n", data.nRCID, data.pAddr);
        CRcf::reqLoadRCF(1, data.nRCID, 0x40, data.pAddr, 0);
    }
    for (INT j = 0; j < 5; j++) {
        m_uiReserveBuf[j] = m_uiReserveBuf[j + 1];
    }
    m_uiReserveBuf[5] = 0;
    m_uiReserve--;
    TRACE("PLM:: Reserve : stack=%d\n", m_uiReserve);
}

UINT CPlayerManager::getRCData_toCard(UINT inID) {
    INT result = 0;
    switch (inID) {
    case 49: result = isHalloweenTown() ? 44 : 28; break;
    case 50: result = isHalloweenTown() ? 45 : 29; break;
    case 51: result = 30; break;
    case 53: result = 32; break;
    case 52: result = 31; break;
    case 54: result = 33; break;
    case 55: result = 34; break;
    case 56: result = isHalloweenTown() ? 46 : 35; break;
    case 57: result = 36; break;
    case 35: result = 37; break;
    case 38: result = 40; break;
    case 37: result = 39; break;
    case 40: result = 42; break;
    case 36: result = 38; break;
    case 39: result = 41; break;
    case 41: result = 43; break;
    }
    return result;
}

UINT CPlayerManager::getRCData_toSkill(UINT inID) {
    INT result = 0;
    switch (inID) {
    case 5: result = 1; break;
    case 9: result = 2; break;
    case 10: result = 3; break;
    case 11: result = 4; break;
    case 12: result = 5; break;
    case 32: result = 12; break;
    case 33: result = 13; break;
    case 34: result = 14; break;
    case 35: result = 15; break;
    case 37: result = 16; break;
    case 38: result = 17; break;
    case 39: result = 18; break;
    case 40: result = 19; break;
    case 41: result = 20; break;
    case 42: result = 21; break;
    case 45: result = 22; break;
    case 46: result = 23; break;
    case 47: result = 24; break;
    case 48: result = 25; break;
    case 49: result = 26; break;
    case 51: result = 27; break;
    case 74: result = 6; break;
    case 95: result = 7; break;
    case 96: result = 8; break;
    case 97: result = 9; break;
    case 100: result = 10; break;
    case 101: result = 11; break;
    case 68: case 69: case 70: result = isHalloweenTown() ? 44 : 28; break;
    case 71: case 72: case 73: result = isHalloweenTown() ? 45 : 29; break;
    case 75: case 76: result = 30; break;
    case 77: case 78: result = 32; break;
    case 79: case 80: result = 31; break;
    case 81: case 82: result = 33; break;
    case 83: case 84: result = 34; break;
    case 98: case 99: result = isHalloweenTown() ? 46 : 35; break;
    case 85: case 86: result = 36; break;
    case 52: case 53: result = 37; break;
    case 54: case 55: result = 40; break;
    case 56: case 57: case 58: result = 39; break;
    case 59: case 60: result = 42; break;
    case 61: case 62: result = 38; break;
    case 63: case 64: result = 41; break;
    case 65: case 66: case 67: result = 43; break;
    case 102: result = 47; break;
    case 103: result = 48; break;
    case 104: result = 49; break;
    case 105: result = 50; break;
    case 106: result = 51; break;
    case 107: result = 52; break;
    case 146: result = 53; break;
    case 147: result = 54; break;
    case 148: result = 55; break;
    case 149: result = 56; break;
    }
    return result;
}

UINT CPlayerManager::getRCID_toRCData(UINT inID) {
    INT result = 0;
    switch (inID) {
    case 1: result = 1005; break;
    case 2: result = 1009; break;
    case 3: result = 1010; break;
    case 4: result = 1011; break;
    case 5: result = 1012; break;
    case 12: result = 1032; break;
    case 13: result = 1033; break;
    case 14: result = 1034; break;
    case 15: result = 1035; break;
    case 16: result = 1037; break;
    case 17: result = 1038; break;
    case 18: result = 1039; break;
    case 19: result = 1040; break;
    case 20: result = 1041; break;
    case 21: result = 1042; break;
    case 22: result = 1045; break;
    case 23: result = 1046; break;
    case 24: result = 1047; break;
    case 25: result = 1048; break;
    case 26: result = 1049; break;
    case 27: result = 1051; break;
    case 6: result = 1074; break;
    case 7: result = 1087; break;
    case 8: result = 1088; break;
    case 9: result = 1089; break;
    case 10: result = 1092; break;
    case 11: result = 1093; break;
    case 28: result = isHalloweenTown() ? 451 : 401; break;
    case 29: result = isHalloweenTown() ? 452 : 402; break;
    case 30: result = 403; break;
    case 31: result = 404; break;
    case 32: result = 405; break;
    case 33: result = 406; break;
    case 34: result = 407; break;
    case 36: result = 409; break;
    case 44: result = isHalloweenTown() ? 451 : 401; break;
    case 45: result = isHalloweenTown() ? 452 : 402; break;
    case 35: case 46: result = isHalloweenTown() ? 458 : 408; break;
    case 37: result = 301; break;
    case 38: result = 305; break;
    case 39: result = 303; break;
    case 40: result = 302; break;
    case 41: result = 306; break;
    case 42: result = 304; break;
    case 43: result = 307; break;
    case 47: result = 1301; break;
    case 48: result = 1302; break;
    case 49: result = 1303; break;
    case 50: result = 1311; break;
    case 51: result = 1312; break;
    case 52: result = 1313; break;
    case 53: result = 1350; break;
    case 54: result = 1351; break;
    case 55: result = 1352; break;
    case 56: result = 1353; break;
    }
    return result;
}

UINT CPlayerManager::getRCID_toSkill(UINT inID) {
    return getRCID_toRCData(getRCData_toSkill(inID));
}

void CPlayerManager::initRsrcBlock() {
    INT i;
    for (i = 0; i < 16; i++) {
        m_blockData[i].init();
        m_blockData[i].pAddr = m_pRsrcEnd;
    }
    m_uiCurrentBlock = 0;
    m_uiReserve = 0;
    for (i = 0; i < 6; i++) {
        m_uiReserveBuf[6] = 0;
    }
}

INT CPlayerManager::getRsrcBlock(UINT inID) {
    INT result = -1;
    INT i;
    for (i = 0; i < 16; i++) {
        if (inID == m_blockData[i].nRCID) {
            result = i;
            break;
        }
    }
    return result;
}

void CPlayerManager::setReqLoadFirst() {
    return;
    TRACE("PLM:: ---- \x83o\x83g\x83\x8b\x8aJ\x8en\x8e\x9e\x82\xc9\x90\xe6\x8ds\x82\xb5\x82\xc4\x83\x8a\x83\\\x81[\x83X\x93\xc7\x8d\x9e ----\n");
}

INT CPlayerManager::isCompleteLoad(UINT inID) {
    UINT rc_data = getRCData_toSkill(inID);
    UINT rc_id;
    if (rc_data == 0) return -1;
    rc_id = getRCID_toRCData(rc_data);
    if (rc_id == 0) return -1;
    return isCompleteLoadRCID(rc_id);
}

INT CPlayerManager::isCompleteLoadRCID(UINT inRCID) {
    INT block = getRsrcBlock(inRCID);
    if (block != -1 && m_blockData[block].nState >= 2) return 1;
    return 0;
}

INT CPlayerManager::isCompleteLoadCard(UINT inID) {
    UINT rc_data = getRCData_toCard(inID);
    UINT rc_id;
    if (rc_data == 0) return -1;
    rc_id = getRCID_toRCData(rc_data);
    if (rc_id == 0) return -1;
    return isCompleteLoadRCID(rc_id);
}

void CPlayerManager::setUseRsrcBlockRCID(UINT inRCID, INT inFlag) {
    INT block = getRsrcBlock(inRCID);
    if (block == -1) return;
    if (!inFlag) {
        if (m_blockData[block].nState == 3) m_blockData[block].nState = 2;
    } else {
        if (m_blockData[block].nState == 2) m_blockData[block].nState = 3;
    }
}

void CPlayerManager::setUseRsrcBlockSkill(UINT inSkillID, INT inFlag) {
    UINT rc_data = getRCData_toSkill(inSkillID);
    UINT rc_id;
    if (rc_data == 0) return;
    rc_id = getRCID_toRCData(rc_data);
    if (rc_id == 0) return;
    setUseRsrcBlockRCID(rc_id, inFlag);
}

void* getBattleRsrcFirst() {
    return g_pRsrcFirst;
}

UINT getBattleRsrcSize() {
    if (!g_pRsrcFirst) return 0;
    return g_uiRsrcSizeTotal;
}

void mallocBattleRsrcBuffer(BOOL inAssert) {
    UINT uiRsrcSizeMin;
    INT worldID;
    INT roomID;
    INT nScene;
    if (g_pRsrcFirst) {
        TRACE("PLM:: \x82\xb7\x82\xc5\x82\xc9\x83\x8a\x83\\\x81[\x83X\x97p\x83o\x83" "b\x83t\x83@\x8am\x95\xdb\x8d\xcf\x82\xdd\n");
        return;
    }
    worldID = CSceneInfo::getCurrentWorldID(0);
    roomID = CSceneInfo::getCurrentRoomID();
    nScene = CSceneInfo::getNextBattleSceneNum();
    if (CGame::isSoraGame()) {
        g_uiRsrcSizeTotal = 0x280000;
        uiRsrcSizeMin = 0x1C0000;
        if (worldID == 6 && roomID != 0 && roomID != 99) uiRsrcSizeMin = 0x1E0000;
        if (worldID == 13 && nScene == 1053) g_uiRsrcSizeTotal = uiRsrcSizeMin;
    } else {
        g_uiRsrcSizeTotal = 0x200000;
        uiRsrcSizeMin = 0x150000;
        if (worldID == 6 && roomID != 0 && roomID != 99) uiRsrcSizeMin = 0x160000;
    }
    while (!g_pRsrcFirst) {
        TRACE("PLM:: Malloc( %d KB ) \x83\x8a\x83\\\x81[\x83X\x97p\x82\xc9\x8am\x95\xdb\x82\xb5\x82\xdc\x82\xb7...",
              g_uiRsrcSizeTotal >> 10);
        g_pRsrcFirst = com_malloc_d(g_uiRsrcSizeTotal, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(1985)", (UINT)-1);
        if (g_pRsrcFirst) {
            TRACE("\x90\xac\x8c\xf7\x81I addr = 0x%08x\n", g_pRsrcFirst);
        } else if (g_uiRsrcSizeTotal == uiRsrcSizeMin) {
            INT nEvent;
            INT nRoom;
            TRACE("\x81\xa6\x8c\xc0\x8a" "E\x88\xc8\x89\xba\x82\xc5\x82\xb7\x81I\x89\xbd\x82\xa9\x8d\xed\x82\xe7\x82\xc8\x82\xa2\x82\xc6...(;\x81L\x84t\x81\xbc) \n");
            nEvent = CGameWork::getInt(0, 3);
            TRACE("E:%03d W:%02d S:%04d R:%02d M:%04d\n", nEvent, CSceneInfo::getCurrentAreaNum(),
                  CSceneInfo::getCurrentSceneNum(), CSceneInfo::getCurrentRoomID(), CGame::getMapID());
            TRACE("Memory:%d%% VRAM:%d%%\n", (INT)CHeapMan::getUsedHeapRatio(), CVramMan::getUsedRatio());
            if (inAssert) {
                if (!g_pRsrcFirst) {
                    AssertApp("c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp", 2010, "g_pRsrcFirst",
                              "\x97" "e\x97\xca\x95s\x91\xab:\x8bZ\x83\x81\x83\x82\x83\x8a\x8am\x95\xdb\x82\xc5\x82\xab\x82\xdc\x82\xb9\x82\xf1!!");
                }
            } else {
                TRACE("\x81\xa6\x82\xb1\x82\xb1\x82\xc5\x82\xcd\x8am\x95\xdb\x82\xf0\x92\xfa\x82\xdf\x82\xdc\x82\xb7\x81" "E\x81" "E\x81" "E\n");
            }
            return;
        } else {
            TRACE("\x8e\xb8\x94s\x81I\x97" "e\x97\xca\x82\xf0\x8c\xb8\x82\xe7\x82\xb5\x82\xc4\x8d\xc4\x8e\x8e\x8ds...\n");
            g_uiRsrcSizeTotal -= 0x80000;
            if (g_uiRsrcSizeTotal < uiRsrcSizeMin) g_uiRsrcSizeTotal = uiRsrcSizeMin;
        }
    }
}

void freeBattleRsrcBuffer() {
    if (!g_pRsrcFirst) {
        TRACE("freeBattleRsrcBuffer:: PLM \x82\xb7\x82\xc5\x82\xc9\x83\x8a\x83\\\x81[\x83X\x97p\x83o\x83" "b\x83t\x83@\x8aJ\x95\xfa\x8d\xcf\x82\xdd\n");
        return;
    }
    if (CPlayerManager::getPLM()) {
        TRACE("freeBattleRsrcBuffer:: PLM \x8fI\x97\xb9\x82\xb5\x82\xc4\x82\xa2\x82\xc8\x82\xa2\x82\xcc\x82\xc5NG\n");
        return;
    }
    TRACE("PLM:: Free( 0x%08x )\n", g_pRsrcFirst);
    com_free(g_pRsrcFirst);
    g_pRsrcFirst = NULL;
}

static inline UINT _getPlayerRcid(INT nMember) {
    return g_uiPlayerRcid[nMember];
}

static inline void calcPlayerID(UINT inID) {
    INT worldID;
    INT roomID;
    if (inID == 0) {
        worldID = CSceneInfo::getCurrentWorldID(0);
        roomID = CSceneInfo::getCurrentRoomID();
        if (CGame::isSoraGame()) {
            if (worldID == 6 && roomID != 0 && roomID != 99) {
                g_uiPlayerID = 2;
            } else {
                g_uiPlayerID = 1;
            }
        } else if (CGame::isRikuGame()) {
            INT nScene = CSceneInfo::getNextBattleSceneNum();
            if (worldID == 12 && nScene == 1151) {
                g_uiPlayerID = 5;
            } else if (worldID == 6 && roomID != 0 && roomID != 99) {
                g_uiPlayerID = 4;
            } else {
                g_uiPlayerID = 3;
            }
        }
    } else {
        g_uiPlayerID = inID;
    }
}

void setReqLoadPlayer(UINT inID) {
    UINT rcID;
    calcPlayerID(inID);
    rcID = g_uiPlayerRcid[g_uiPlayerID];
    if (!CRsrcDB::getFirstRsrc(12, rcID + 10000, 1)) {
        TRACE("-- setReqLoadPlayer:: %d:%d,\n", g_uiPlayerID, rcID);
        CRcf::reqLoadPlayer(rcID);
        g_bIsLastSceneBattle = FALSE;
    }
}

static inline void calcBattleCommonRCID() {
    switch (g_uiPlayerID) {
    case 5:
        g_uiBattleCommonRCID = 1201;
        g_uiPlayerExID = 0;
        break;
    case 3:
        g_uiBattleCommonRCID = 1101;
        g_uiPlayerExID = 5;
        break;
    case 4:
        g_uiBattleCommonRCID = 1101;
        g_uiPlayerExID = 6;
        break;
    case 1:
    case 2:
    default:
        g_uiBattleCommonRCID = 1001;
        g_uiPlayerExID = 0;
        break;
    }
}

void setReqLoadBattlePlayer(UINT inID) {
    UINT rcID;
    calcPlayerID(inID);
    calcBattleCommonRCID();
    if (!CRsrcDB::hasRsrcGroup(1, g_uiBattleCommonRCID)) {
        TRACE("-- setReqLoadBattlePlayer:: %d:%d,\n", g_uiPlayerID, g_uiBattleCommonRCID);
        CRcf::reqLoadPlayer(g_uiBattleCommonRCID);
        g_bIsLastSceneBattle = FALSE;
    }
    if (g_uiPlayerExID) {
        rcID = g_uiPlayerRcid[g_uiPlayerExID];
        if (!CRsrcDB::hasRsrcGroup(1, rcID)) {
            TRACE("-- setReqLoadBattlePlayer:: %d:%d,\n", g_uiPlayerExID, rcID);
            CRcf::reqLoadPlayer(rcID);
        }
    }
}

void removePlayerRsrc(UINT inID) {
    UINT rcID;
    if (inID) g_uiPlayerID = inID;
    rcID = g_uiPlayerRcid[g_uiPlayerID];
    CRsrcDB::removeRsrc(rcID + 10000);
    g_bIsLastSceneBattle = FALSE;
    TRACE("-- removePlayerRsrc:: %d:%d,\n", g_uiPlayerID, rcID);
    if (g_pRsrcFirst) {
        CCamp::removeRsrc();
        CJiminyMemoFlag::outTask();
        if (CPlayerManager::getPLM()) {
            TRACE("PLM:: **** Warning!! ****  PLM!=NULL\n");
        }
        TRACE("PLM:: Free( 0x%08x )\n", g_pRsrcFirst);
        com_free(g_pRsrcFirst);
        g_pRsrcFirst = NULL;
    }
}

void removeBattlePlayerRsrc(UINT inID) {
    UINT rcID;
    if (inID) g_uiPlayerID = inID;
    calcBattleCommonRCID();
    CRsrcDB::removeRsrc(g_uiBattleCommonRCID + 10000);
    g_bIsLastSceneBattle = FALSE;
    TRACE("-- removeBattlePlayerRsrc:: %d:%d,\n", g_uiPlayerID, g_uiBattleCommonRCID);
    if (g_uiPlayerExID) {
        rcID = g_uiPlayerRcid[g_uiPlayerExID];
        CRsrcDB::removeRsrc(rcID + 10000);
        TRACE("-- removeBattlePlayerRsrc:: %d:%d,\n", g_uiPlayerExID, rcID);
    }
}

CPlayer* createPlayer(UINT inID, CTask* inParent, INT inEx) {
    CTask* parent;
    CPlayerManager* pPLM;
    CPlayer* pPlayer;
    UINT uiPlayerID;
    if (inID && !inEx) g_uiPlayerID = inID;
    if (inParent) {
        parent = inParent;
    } else {
        parent = CTaskMan::getSysTask(1);
    }
    pPLM = CPlayerManager::getPLM();
    if (pPLM == NULL) return NULL;
    pPlayer = pPLM->getPlayer();
    uiPlayerID = g_uiPlayerID;
    if (inEx) {
        pPlayer = pPLM->getPlayerEx();
        uiPlayerID = inID;
    }
    TRACE("-- createPlayer:: ID=%d,\n", uiPlayerID);
    if (pPlayer) return pPlayer;
    if ((USHORT)(CSceneInfo::getCurrentSceneNum() - 2000) < 1000) {
        TRACE("\x81\xa6\x83" "C\x83x\x83\x93\x83g\x90\xea\x97p\x83V\x81[\x83\x93\x82\xc8\x82\xcc\x82\xc5\x83v\x83\x8c\x83" "C\x83\x84\x81[\x8d\xec\x90\xac\x83L\x83\x83\x83\x93\x83Z\x83\x8b\n");
        return NULL;
    }
    if (CRsrcDB::getFirstRsrc(12, _getPlayerRcid(uiPlayerID) + 10000, 1)) {
        switch (uiPlayerID) {
        case 1:
        case 2:
            pPlayer = new (com_malloc_d(0x7C0, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2297)", RET_ADDR()))
                PL::CPlayerSora(parent);
            break;
        case 3:
        case 4:
            pPlayer = new (com_malloc_d(0x780, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2301)", RET_ADDR()))
                PL::CPlayerRiku(parent);
            break;
        case 5:
            pPlayer = new (com_malloc_d(0x7C0, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2305)", RET_ADDR()))
                PL::CPlayerDRiku(parent, 5, inEx);
            break;
        case 6:
            pPlayer = new (com_malloc_d(0x7C0, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2308)", RET_ADDR()))
                PL::CPlayerDRiku(parent, 6, inEx);
            break;
        default:
            pPlayer = new (com_malloc_d(0x770, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2312)", RET_ADDR()))
                CPlayer(parent, g_uiPlayerID, 0);
            break;
        }
        if (!pPlayer) AssertApp("c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp", 2315, "pPlayer", 0);
        pPLM->notifyPlayer(pPlayer, inEx);
    } else {
        TRACE("\x81\xa6\x83\x8a\x83\\\x81[\x83X\x82\xaa\x96\xb3\x82\xa2\x82\xcc\x82\xc5\x90\xb6\x90\xac\x8e\xb8\x94s\x81I %d,%d\n",
              uiPlayerID, _getPlayerRcid(uiPlayerID));
    }
    return pPlayer;
}

void CPlayerManager::notifyPlayer(CPlayer* inPlayer, INT inEx) {
    if (!inEx) {
        m_pPlayer = inPlayer;
    } else {
        m_pPlayerEx = inPlayer;
    }
}

void CPlayerManager::notifyPlayerDead(INT inFlag) {
    if (inFlag) {
        m_iStep = 7;
        m_tqTimer = 50;
        m_uiFlag |= 4;
        CGameWork::setInt(1, 0xE2, -1);
        offFlag(m_uiFlag, 8);
        if (!g_pPlayer->isConditionFlag(0x20000)) m_uiFlag |= 8;
        offFlag(m_uiFlag, 0x400);
        if (g_pPlayer == m_pPlayerEx) m_uiFlag |= 0x400;
    } else {
        offFlag(m_uiFlag, 4);
    }
}

void destroyPlayer() {
    CPlayerManager* pPLM = CPlayerManager::getPLM();
    CPlayer* pPlayer;
    if (pPLM) {
        TRACE("-- destroyPlayer:: ");
        pPlayer = pPLM->getPlayerEx();
        if (pPlayer) {
            pPlayer->out();
            TRACE("PlayerEx out,");
        }
        pPlayer = pPLM->getPlayer();
        if (pPlayer) {
            pPlayer->out();
            TRACE("Player out,");
        }
        TRACE(" complete!\n");
    }
}

CEntity* CPlayerManager::createFriend(UINT inID, CTask* inParent) {
    CTask* parent = this;
    m_pFriend = NULL;
    m_pFriendEx = NULL;
    if (inParent) parent = inParent;
    offFlag(m_uiFlag, 0x42);
    switch (inID) {
    case 1:
        m_pFriend = new (com_malloc_d(0x340, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2418)", RET_ADDR()))
            Friend::CFriendDonald(parent);
        break;
    case 2:
        m_pFriend = new (com_malloc_d(0x340, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2421)", RET_ADDR()))
            Friend::CFriendGoofy(parent, 0);
        break;
    case 3:
        m_pFriend = new (com_malloc_d(0x340, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2424)", RET_ADDR()))
            Friend::CFriendAladdin(parent);
        break;
    case 4:
        m_pFriend = new (com_malloc_d(0x340, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2427)", RET_ADDR()))
            Friend::CFriendAriel(parent);
        break;
    case 5:
        m_pFriend = new (com_malloc_d(0x340, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2430)", RET_ADDR()))
            Friend::CFriendJack(parent);
        break;
    case 6:
        m_pFriend = new (com_malloc_d(0x350, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2433)", RET_ADDR()))
            Friend::CFriendPan(parent);
        break;
    case 7:
        m_pFriend = new (com_malloc_d(0x340, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2436)", RET_ADDR()))
            Friend::CFriendBeast(parent);
        break;
    case 8:
        m_pFriend = new (com_malloc_d(0x340, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2439)", RET_ADDR()))
            Friend::CFriendKing(parent);
        break;
    case 9:
        m_pFriend = new (com_malloc_d(0x350, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2442)", RET_ADDR()))
            Friend::CFriendPluto(parent);
        break;
    case 10:
        m_pFriendEx = new (com_malloc_d(0x340, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2445)", RET_ADDR()))
            Friend::CFriendGoofy(parent, 1);
        m_pFriend = new (com_malloc_d(0x340, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2446)", RET_ADDR()))
            Friend::CFriendDonald(parent);
        break;
    }
    if (m_pFriendEx && m_pFriendEx->getRCID()) setUseRsrcBlockRCID(m_pFriendEx->getRCID(), 1);
    if (m_pFriend && m_pFriend->getRCID()) setUseRsrcBlockRCID(m_pFriend->getRCID(), 1);
    return (CEntity*)m_pFriend;
}

void CPlayerManager::notifyFriend(Friend::CFriend* inFriend, INT inFlag, INT inEx) {
    if (inFlag) {
        if (!inEx) {
            m_pFriend = inFriend;
            m_uiFlag |= 2;
        } else {
            m_pFriendEx = inFriend;
            m_uiFlag |= 0x40;
        }
    } else if (!inEx) {
        if (m_pFriend == inFriend) {
            if (inFriend->getRCID()) setUseRsrcBlockRCID(inFriend->getRCID(), 0);
            m_pFriend = NULL;
            offFlag(m_uiFlag, 2);
        }
    } else {
        if (m_pFriendEx == inFriend) {
            if (m_pFriendEx->getRCID()) setUseRsrcBlockRCID(m_pFriendEx->getRCID(), 0);
            m_pFriendEx = NULL;
            offFlag(m_uiFlag, 0x40);
        }
    }
}

INT CPlayerManager::getAppearFriendCard(INT inNum, INT inFlag) {
    INT worldID;
    INT roomID;
    INT kind;
    if ((UINT)inNum >= 3) return 0;
    worldID = CSceneInfo::getCurrentWorldID(0);
    if (worldID >= 14) worldID = 13;
    roomID = CSceneInfo::getCurrentRoomID();
    if (roomID == 0 || roomID == 99) worldID = 13;
    if (CGame::isRikuGame()) worldID += 13;
    kind = CardBattle::g_DropFriendCardList[worldID][inNum + 4];
    if (inFlag) {
        switch (kind) {
        case 49:
            if (!(CGameWork::getInt(0, 0x22) & 1)) kind = 0;
            break;
        case 50:
            if (!(CGameWork::getInt(0, 0x22) & 2)) kind = 0;
            break;
        case 51:
            if (!(CGameWork::getInt(0, 0x22) & 4)) kind = 0;
            break;
        case 53:
            if (!(CGameWork::getInt(0, 0x22) & 0x10)) kind = 0;
            break;
        case 52:
            if (!(CGameWork::getInt(0, 0x22) & 8)) kind = 0;
            break;
        case 54:
            if (!(CGameWork::getInt(0, 0x22) & 0x20)) kind = 0;
            break;
        case 55:
            if (!(CGameWork::getInt(0, 0x22) & 0x40)) kind = 0;
            break;
        case 56:
            if (!(CGameWork::getInt(0, 0x22) & 0x80)) kind = 0;
            break;
        case 57:
            if (!(CGameWork::getInt(0, 0x22) & 0x100)) kind = 0;
            break;
        }
    }
    return kind;
}

UINT CPlayerManager::getFriendID_toCard(UINT inID) {
    INT result = 0;
    switch (inID) {
    case 49: result = 1; break;
    case 50: result = 2; break;
    case 51: result = 3; break;
    case 53: result = 5; break;
    case 52: result = 4; break;
    case 54: result = 6; break;
    case 55: result = 7; break;
    case 56: result = 8; break;
    case 57: result = 9; break;
    }
    return result;
}

UINT CPlayerManager::getFriendID_toSkill(UINT inID) {
    INT result = 0;
    switch (inID) {
    case 68: case 69: case 70: result = 1; break;
    case 71: case 72: case 73: result = 2; break;
    case 75: case 76: result = 3; break;
    case 77: case 78: result = 5; break;
    case 79: case 80: result = 4; break;
    case 81: case 82: result = 6; break;
    case 83: case 84: result = 7; break;
    case 98: case 99: case 100: case 101: result = 8; break;
    case 85: case 86: result = 9; break;
    case 74: result = 10; break;
    }
    return result;
}

CEntity* CPlayerManager::createSummon(UINT inID, CTask* inParent) {
    CTask* parent = this;
    m_pSummon = NULL;
    if (inParent) parent = inParent;
    offFlag(m_uiFlag, 0x20);
    switch (inID) {
    case 1:
        m_pSummon = new (com_malloc_d(0x340, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2687)", RET_ADDR()))
            Summon::CSummonSinba(parent);
        break;
    case 2:
        m_pSummon = new (com_malloc_d(0x350, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2690)", RET_ADDR()))
            Summon::CSummonGenie(parent);
        break;
    case 3:
        m_pSummon = new (com_malloc_d(0x340, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2693)", RET_ADDR()))
            Summon::CSummonBambi(parent);
        break;
    case 4:
        m_pSummon = new (com_malloc_d(0x340, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2696)", RET_ADDR()))
            Summon::CSummonDumbo(parent);
        break;
    case 5:
        m_pSummon = new (com_malloc_d(0x360, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2699)", RET_ADDR()))
            Summon::CSummonTink(parent);
        break;
    case 6:
        m_pSummon = new (com_malloc_d(0x340, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2702)", RET_ADDR()))
            Summon::CSummonMushu(parent);
        break;
    case 7:
        m_pSummon = new (com_malloc_d(0x350, "c:/KHCOM/MainApp/Game/Player/PlayerManager.cpp(2705)", RET_ADDR()))
            Summon::CSummonCloud(parent);
        break;
    }
    if (m_pSummon && m_pSummon->getRCID()) setUseRsrcBlockRCID(m_pSummon->getRCID(), 1);
    return (CEntity*)m_pSummon;
}

void CPlayerManager::notifySummon(Summon::CSummon* inSummon, INT inFlag) {
    if (inFlag) {
        m_pSummon = inSummon;
        m_uiFlag |= 0x20;
    } else if (m_pSummon == inSummon) {
        if (m_pSummon->getRCID()) setUseRsrcBlockRCID(m_pSummon->getRCID(), 0);
        m_pSummon = NULL;
        offFlag(m_uiFlag, 0x20);
    }
}

UINT CPlayerManager::getSummonID_toCard(UINT inID) {
    INT result = 0;
    switch (inID) {
    case 35: result = 1; break;
    case 36: result = 2; break;
    case 37: result = 3; break;
    case 38: result = 4; break;
    case 39: result = 5; break;
    case 40: result = 6; break;
    case 41: result = 7; break;
    }
    return result;
}

UINT CPlayerManager::getSummonID_toSkill(UINT inID) {
    INT result = 0;
    switch (inID) {
    case 52: case 53: result = 1; break;
    case 61: case 62: result = 2; break;
    case 56: case 57: case 58: result = 3; break;
    case 54: case 55: result = 4; break;
    case 63: case 64: result = 5; break;
    case 59: case 60: result = 6; break;
    case 65: case 66: case 67: result = 7; break;
    }
    return result;
}

void CPlayerManager::enableWind(INT inSet) {
    if (!inSet) {
        offFlag(m_uiFlag, 0x1000);
    } else {
        m_uiFlag |= 0x1000;
    }
    m_vWind.init();
}

void CPlayerManager::exeWind() {
    if (isEnableWind()) {
        if (Math3D::VectorLength(getWind()) > 0.003f) {
            ScaleVectorXYZ(getWind(), getWind(), 0.8f);
        } else {
            enableWind(0);
        }
    }
}

void CPlayerManager::setWind(FVECTOR* inVel) {
    m_uiFlag |= 0x1000;
    m_vWind = *inVel;
}

void CPlayerManager::addWindXYZ(FVECTOR* inVel) {
    if (!isEnableWind()) enableWind(0x1000);
    AddVectorXYZ(getWind(), getWind(), *inVel);
}

void addIntBR(UINT nIdx) {
    INT num = CGameWork::getInt(0, nIdx);
    if (num < 9999) num++;
    CGameWork::setInt(0, nIdx, num);
}

void addStockBR(UINT nSkillID) {
    UINT nIdx = nSkillID + 0x44;
    INT num = CGameWork::getInt(0, nIdx);
    if (num < 9999) num++;
    CGameWork::setInt(0, nIdx, num);
}

void calcFavoriteStockCount() {
    INT skill = 0x44;
    INT count = 0;
    INT num;
    INT i;
    for (i = 0x48; i < 0xB2; i++) {
        num = CGameWork::getInt(0, i);
        if (count < num) {
            count = num;
            skill = i;
        }
    }
    CGameWork::setInt(0, 0x3D, skill - 0x44);
    CGameWork::setInt(0, 0x3E, count);
}

void calcDuelWin() {
    F_UL count;
    F_UL win;
    F_UL success;
    count.ul = CGameWork::getInt(0, 0x43);
    if (count.ul) {
        win.ul = CGameWork::getInt(0, 0x42);
        success.f = (FLOAT)win.ul / (FLOAT)count.ul;
    } else {
        success.ul = 0;
    }
    CGameWork::setInt(0, 0x40, success.ul);
}

void setVibration(INT inID) {
    INT motorL;
    INT motorS;
    TIMEQ tq;
    switch (inID) {
    case 1:
        motorL = 0;
        motorS = 2;
        tq = 300;
        break;
    case 2:
        motorL = 0x40;
        motorS = 2;
        tq = 400;
        break;
    case 3:
        motorL = 0xFF;
        motorS = 2;
        tq = 500;
        break;
    case 4:
        if (RND::genrand_int31() % 1000 >= 500) return;
        motorS = 1;
        tq = 1;
        motorL = RND::genrand_int31() % 256;
        break;
    default:
        return;
    }
    if (dbgCheckUser(KKIKUCHI) && DebugWork::g_dw.player.iTemp == 8) {
        FLOAT fl = DebugWork::g_dw.player.vTemp.x * 100.0f;
        FLOAT fs = DebugWork::g_dw.player.vTemp.y * 100.0f;
        FLOAT ft = DebugWork::g_dw.player.vTemp.z * 10000.0f;
        motorL = (INT)(fl + 0.5f);
        motorS = (INT)(fs + 0.5f);
        tq = (INT)(ft + 0.5f);
    }
    CPadIO::startVibration(motorL, motorS, tq, 0);
}

INT isTaskEntity(HANDLE inHandle) {
    CTask* pTask = CTaskMan::getTask(inHandle);
    if (!pTask) return 0;
    return isEntityTypeID(pTask->getTypeID());
}

INT isEntityTypeID(INT inID) {
    switch (inID) {
    case 0x200:
    case 0x1000:
    case 0x1002:
    case 0x1004:
    case 0x1005:
    case 0x1006:
    case 0x1007:
    case 0x1008:
    case 0x1009:
    case 0x100A:
    case 0x2000:
    case 0x2001:
    case 0x2002:
    case 0x2003:
    case 0x2004:
    case 0x2005:
    case 0x4000:
        break;
    default:
        return 0;
    }
    return 1;
}

static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}

INT getWorldRotY(FLOAT& outRotY, const FMATRIX& inMat) {
    FMATRIX mMat = inMat;
    float fDist[3];
    INT i;
    for (i = 0; i < 3; i++) {
        register INT iTmp asm("$9");
        FLOAT ret;
        asm volatile("lqc2 $vf4, 0x0(%2)\n"
                     "vaddy.x $vf6, $vf0, $vf4y\n"
                     "vaddz.x $vf7, $vf0, $vf4z\n"
                     "vmulax.x $ACC, $vf4, $vf4x\n"
                     "vmadday.x $ACC, $vf6, $vf4y\n"
                     "vmaddz.x $vf5, $vf7, $vf4z\n"
                     "qmfc2 %1, $vf5\n"
                     "mtc1 %1, %0\n"
                     : "=f"(ret), "=r"(iTmp)
                     : "r"(&mMat.v[i]));
        fDist[i] = _sqrtf(ret);
        if (fDist[i] < 0.0001f) return 0;
    }
    DivVector(mMat.v[0], mMat.v[0], fDist[0]);
    if (mMat.v[0].z < 0.0001f && mMat.v[0].x < 0.0001f) return 0;
    outRotY = -atan2f(mMat.v[0].z, mMat.v[0].x);
    return 1;
}

} // namespace PLM

// SOURCE DATA: .data .rodata .bss
