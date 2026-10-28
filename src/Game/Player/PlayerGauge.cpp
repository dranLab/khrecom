#include "khtypes.h"
#define GAME_PLAYER_PLAYERGAUGE_CPP
#include "Game/Player/PlayerGauge.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/TaskMan.h"
#include "Game/GameWork.h"
#include "Game/SceneEnv.h"
#include "Game/Game.h"
#include "Game/Entity.h"
#include "Game/Player/PlayerCamera.h"
#include "include_asm.h"

using namespace Gauge;

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
inline void* operator new(unsigned int, void* p) throw() { return p; }
namespace Gauge {
extern LPCSTR g_szSeqCockpit;
extern LPCSTR g_szSeqGauge;
extern LPCSTR g_szLadCockpit;
}
FVECTOR getPlayerPos(BOOL flag);
namespace D2 {
class C2DMan { /* PARTIAL: stand-in for System/Graphic/2d/2DMan C2DMan: gated by SYSTEM_GRAPHIC_2D_2DMAN_CPP */
public:
    static BOOL getLayPosition(const CHAR* pLadName, UINT inNo, UINT inIdx, FVECTOR& outPos, BOOL bLocal);
};
}
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
FLOAT InnerProduct(const FVECTOR& inV0, const FVECTOR& inV1);
static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}
extern "C" char D_004A3238[];
template <class T> static inline T& _ref(T& v) { return v; }

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", DebugPlayerCamera__9DWTPlayerP12CDebugWindowii_00284530);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", DebugPlayerAttack__9DWTPlayerP12CDebugWindowii_00284640);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", DebugWinPlayer__9DWTPlayerP12CDebugWindowii_002848F0);
CPlayerGauge::CPlayerGauge(CTask* p) : CTask(p, 0x4000), m_pNaviMap(NULL), m_pFriend(NULL) { _initialize(); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _$_Q25Gauge12CPlayerGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _initialize__Q25Gauge12CPlayerGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", main__Q25Gauge12CPlayerGauge);
void CPlayerGauge::changeTimeHP(D2::C2DSeqCtrl* p, UINT nIdx, UINT nHP) {
    p->changeTime(nIdx, (UINT)((FLOAT)nHP / 560.0f * 470.0f));
}
void CPlayerGauge::setMaxHP(INT n) {
    m_nMaxHP = n > -1 ? n : 0;
    if (m_nMaxHP > 560) m_nMaxHP = 560;
    changeTimeHP(m_pSeqCtrl, 1, n);
}
void CPlayerGauge::setHP(INT inHP, INT inFlag) {
    if (m_nHP == inHP) return;
    m_nHP = inHP > -1 ? inHP : 0;
    if (m_nHP > 560) m_nHP = 560;
    changeTimeHP(m_pSeqCtrl, 3, inHP);
    m_tqRed = 1500;
    if (inFlag || m_nRedHP < m_nHP) {
        m_nRedHP = m_nHP;
        changeTimeHP(m_pSeqCtrl, 2, m_nRedHP);
        m_tqRed = 0;
    }
    if (inFlag) {
        INT max = m_nMaxHP; // MATCH: signed division (USHORT / 4 is shortened to unsigned)
        if (m_nHP <= max / 4) {
            m_pSeqCtrl->replaceSeq(0, g_szSeqGauge, 0xA);
            m_pSeqCtrl->replaceSeq(4, m_szSeqFace, 0);
        }
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setDamage__Q25Gauge12CPlayerGaugeii);
void CPlayerGauge::setBattleEnd() { CNaviMap::m_bBattleEnd = TRUE; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setDPGauge__Q25Gauge12CPlayerGaugei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setDP__Q25Gauge12CPlayerGaugeii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setDMode__Q25Gauge12CPlayerGaugeiii);
void CPlayerGauge::exeDPGauge() {
    if (!m_pSeqCtrlDP) return;
    if (!(m_uiFlag & 4)) {
        if (CGame::isRikuGame() && (CGameWork::getInt(0, 0x24) & 8)) m_uiFlag |= 4;
    } else {
        INT n = m_nStateDP;
        if (n >= 10) return;
        if (n >= 8 && m_pSeqCtrl->isEnd(5)) {
            m_pSeqCtrl->destroyIndexToLast(5);
            m_pSeqCtrlDP->destroyIndexToLast(2);
            setDPGauge(2);
        }
    }
}
CLockonMarker::CLockonMarker(CTask* inParent, BOOL inFlag) : CTask(inParent, 0x4000), m_pGauge(NULL), m_bFlag(inFlag) { _initialize(); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _$_Q25Gauge13CLockonMarker);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _initialize__Q25Gauge13CLockonMarker);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", main__Q25Gauge13CLockonMarker);
void CLockonMarker::calcMarkerScale() {
    FVECTOR pos = getPlayerPos(FALSE);
    FLOAT dist;
    FLOAT old_sc;
    FVECTOR v;
    dist = m_fDist;
    if (PlayerCamera::CPlayerCamera::getPlayerCameraPos(pos)) {
        SubVectorXYZ(v, pos, m_vPos);
        dist = _sqrtf(InnerProduct(v, v));
    }
    old_sc = m_fScale;
    if (dist < 3.5f) m_fScale = 1.0f;
    else if (dist < 14.0f) m_fScale = (14.0f - dist) / 10.5f * 0.5f + 0.5f;
    else m_fScale = 0.5f;
    if (m_fScale != old_sc) {
        m_pSeqCtrl->setScl(m_fScale);
        if (m_uiFlag & 2) m_pSeqCtrlPush->setScl(m_fScale);
    }
}
void CLockonMarker::setTarget(HANDLE inTarget) {
    m_hTarget = inTarget;
    m_bLockon = FALSE;
    m_bTarget = FALSE;
    m_iDist = 0;
    if (!inTarget && (m_uiFlag & 1)) {
        m_uiFlag = (INT)m_uiFlag & ~1;
        m_pSeqCtrl->enableDisp(0, TRUE);
        m_pSeqCtrl->restart(0, 0.0f);
    }
    m_uiFlag = (INT)m_uiFlag & ~0x10;
    if (m_hTarget > 0) {
        CEntity* pTarget = (CEntity*)CTaskMan::getTask(m_hTarget);
        if (pTarget) {
            switch (pTarget->getTypeID()) {
            case 0x1006:
            case 0x2000:
            case 0x2003:
            case 0x2004:
            case 0x2005:
            case 0x4000:
                if (!(m_uiFlag & 0x80)) m_uiFlag |= 0x10;
                break;
            }
            m_bTarget = TRUE;
        }
    }
}
void CLockonMarker::setLockon(HANDLE inTarget, INT inDist) {
    m_iDist = inDist;
    m_hTarget = inTarget;
    m_uiFlag = (INT)m_uiFlag & ~0x10;
    m_bTarget = FALSE;
    m_bLockon = FALSE;
    if (inTarget > 0 && CTaskMan::getTask(inTarget)) m_bLockon = TRUE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", chkLockon__Q25Gauge13CLockonMarkerRi);
void CLockonMarker::cancelLockon(INT& ioHandle, INT inFlag) {
    if (!inFlag) {
        m_hTarget = 0;
        m_bLockon = FALSE;
        m_bTarget = FALSE;
        m_uiFlag = (INT)m_uiFlag & ~0x10;
        m_uiFlag = (INT)m_uiFlag & ~1;
        m_iDist = 0;
        ioHandle = 0;
        m_hLockon = 0;
        m_iMode = 0;
        m_bTrg = FALSE;
        if (m_pGauge) CEnemyGauge::resetHP();
        holdTarget(0);
        offPush();
    } else if (m_uiKind != 6) {
        m_hTarget = 0;
        m_bLockon = FALSE;
        m_bTarget = FALSE;
        m_uiFlag = (INT)m_uiFlag & ~0x10;
        m_uiFlag = (INT)m_uiFlag & ~1;
        m_iDist = 0;
        ioHandle = 0;
        m_hLockon = 0;
        m_iMode = 0;
        m_bTrg = FALSE;
        if (m_pGauge) CEnemyGauge::resetHP();
        holdTarget(0);
        offPush();
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", onPush__Q25Gauge13CLockonMarkeri);
void CLockonMarker::offPush() {
    if (m_uiFlag & 2) {
        UINT f = m_uiFlag;
        m_uiFlag = (INT)f & ~2;
        if (!(f & 0x20)) {
            m_pSeqCtrlPush->destroy();
            if (m_uiKind == 6) m_uiFlag |= 0x120;
        }
        m_uiKind = 0;
    }
}
void CLockonMarker::holdTarget(INT inFlag) {
    if (inFlag) {
        m_uiFlag |= 8;
        m_hHoldTarget = m_hTarget;
    } else {
        m_hHoldTarget = 0;
        m_uiFlag = (INT)m_uiFlag & ~8;
    }
}
CEnemyGauge::CEnemyGauge(CTask* p) : CTask(p, 0x4000) {
    m_pInstance = this;
    _initialize();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _$_Q25Gauge11CEnemyGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _initialize__Q25Gauge11CEnemyGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", main__Q25Gauge11CEnemyGauge);
void CEnemyGauge::resetHP() {
    if (!m_pInstance) return;
    if ((m_pInstance->m_uiFlag & 1) == 0) {
        *(UINT*)&m_pInstance->m_uiFlag = (INT)(m_pInstance->m_uiFlag | 1) & ~0x1E; // MATCH: not a struct-field store, so m_pInstance is reloaded after it
        m_pInstance->m_pSeqCtrl->enableDispEach(0);
    }
    m_pInstance->m_hTarget = 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setHP__Q25Gauge11CEnemyGaugeiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", notifyHP__Q25Gauge11CEnemyGaugeiii);
HANDLE CEnemyGauge::getNowTarget() {
    if (!m_pInstance) return 0;
    if (CGameWork::getInt(1, 0xe2) & 0x10) return 0;
    return m_pInstance->m_hTarget;
}
CCommonGauge::CCommonGauge(CTask* p) : CTask(p, 0x4000) {
    m_pInstance = this;
    _initialize();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _$_Q25Gauge12CCommonGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _initialize__Q25Gauge12CCommonGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", main__Q25Gauge12CCommonGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", dispSeqCtrl__Q25Gauge12CCommonGaugeiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setDispSeq__Q25Gauge12CCommonGaugeii);
CPrizeGauge::CPrizeGauge(CTask* p) : CTask(p ? p : getSysTask(1), 0x4000) {
    m_pInstance = this;
    if (p) m_uiMode = 1;
    else m_uiMode = 0;
    m_tqTimer = 0;
    _initialize();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _$_Q25Gauge11CPrizeGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _initialize__Q25Gauge11CPrizeGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", main__Q25Gauge11CPrizeGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setDispSeq__Q25Gauge11CPrizeGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setDispMogSeq__Q25Gauge11CPrizeGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setPrizeExp__Q25Gauge11CPrizeGaugei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setPrizeMp__Q25Gauge11CPrizeGaugei);
/* Parked: 18 lines off: the %hi(m_pInstance) base and its loop copy swap s2/s3, and the inMp <= 0 return
   branch doesn't get the epilogue's lq s0 in its delay slot, so the loop top gets an alignment nop. Worked:
   *(T*)& stores for m_uiMogState and m_iDropMp (m_pInstance reloaded after them, as resetHP). Tried: loop shapes
   (index base 0/1, do-while, pointer local), early-return shapes; permuter 3 min (8172, no gain). */
#if 0
void CPrizeGauge::setDropMp(INT inMp) {
    INT i;
    if (!m_pInstance) return;
    if (inMp <= 0) return;
    if (CGameWork::getInt(1, 0xE2)) return;
    if (m_pInstance->m_uiMogState < 7) {
        *(UINT*)&m_pInstance->m_uiMogState = 7; // MATCH: as resetHP (m_pInstance reloaded after it)
        m_pInstance->m_pMogSeqCtrls->replaceSeq(0, g_szSeqCockpit, 0x8C);
        m_pInstance->m_pMogSeqCtrls->enableDisp(0, TRUE);
        for (i = 1; i < 6; i++) {
            m_pInstance->m_pMogSeqCtrls[i].replaceSeq(0, g_szSeqCockpit, 0x82);
            m_pInstance->m_pMogSeqCtrls[i].enableDisp(0, FALSE);
        }
    }
    *(INT*)&m_pInstance->m_iDropMp = inMp; // MATCH: as resetHP
    m_pInstance->m_iOldMp = 0;
    m_pInstance->m_iMp = 0;
    m_pInstance->setDispMogSeq();
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setDropMp__Q25Gauge11CPrizeGaugei);
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setPrizeCard__Q25Gauge11CPrizeGaugeR4CARDii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", exePrizeCard__Q25Gauge11CPrizeGauge);
BOOL CPrizeGauge::isNowDispCard() {
    if (!m_pInstance) return FALSE;
    return m_pInstance->m_uiCardState != 0;
}
CEscapeGauge::CEscapeGauge(CTask* p) : CTask(p, 0x4000) { _initialize(); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _$_Q25Gauge12CEscapeGauge);
void CEscapeGauge::_initialize() {
    m_uiState = 0;
    m_uiFlag = 0;
    m_pSeqCtrl = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), D_004A3238, RET_ADDR())) D2::C2DSeqCtrl;
    m_pSeqCtrl->setLayer(8);
}
void CEscapeGauge::main() {
    if (CSceneEnv::isStopGauge()) return;
    switch (m_uiState) {
    case 1:
        if (m_pSeqCtrl->isEnd(0)) {
            m_uiState = 2;
            m_pSeqCtrl->replaceSeq(0, _ref(g_szSeqCockpit), 0x3A);
        }
        break;
    case 0:
    case 2:
        break;
    case 3:
        if (m_pSeqCtrl->isEnd(0)) {
            m_uiState = 0;
            m_pSeqCtrl->destroy();
        }
        break;
    }
}
/* Parked: 3 lines off, the getLayPosition argument loads: the original sets a2 first and a1 (7) last (in the
   jal delay slot), ours a1 first. Worked: case 2 with its own copy of case 1's body (separate tree nodes 1/0/2/3,
   cross-jumped later), case 0 falling into it. Tried: _ref(g_szLadCockpit), FALSE vs 0, casts; permuter 3 min
   (12013 candidates, no gain). */
#if 0
void CEscapeGauge::setCharge(FLOAT inRate) {
    FVECTOR pos;
    switch (m_uiState) {
    case 0:
        m_uiState = 1;
        m_pSeqCtrl->addSeq(g_szSeqCockpit, 0x39);
        m_pSeqCtrl->addSeq(g_szSeqCockpit, 0x3C);
        D2::C2DMan::getLayPosition(g_szLadCockpit, 7, 0, pos, FALSE);
        m_pSeqCtrl->setPos(pos);
    case 1:
        m_pSeqCtrl->changeTime(1, (UINT)inRate);
        break;
    case 2:
        m_pSeqCtrl->changeTime(1, (UINT)inRate);
        break;
    case 3:
        m_uiState = 2;
        m_pSeqCtrl->replaceSeq(0, g_szSeqCockpit, 0x3A);
        m_pSeqCtrl->changeTime(1, (UINT)inRate);
        break;
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setCharge__Q25Gauge12CEscapeGaugef);
#endif
void CEscapeGauge::destroy() {
    switch (m_uiState) {
    case 1:
    case 2:
        m_uiState = 3;
        m_pSeqCtrl->replaceSeq(0, _ref(g_szSeqCockpit), 0x3B);
        m_pSeqCtrl->changeTime(1, 0);
        break;
    }
}
CFriendGauge::CFriendGauge(CTask* p) : CTask(p, 0x4000) { _initialize(); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _$_Q25Gauge12CFriendGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _initialize__Q25Gauge12CFriendGauge);
void CFriendGauge::main() {
    if (CSceneEnv::isStopGauge()) return;
    if (m_uiState < 2) return;
    if (m_uiState != 2) return;
    if (m_pSeqCtrls->isEnd(0)) setGauge(0);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setGauge__Q25Gauge12CFriendGaugei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", __static_initialization_and_destruction_0_0028B0E0);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setDistance__Q25Gauge13CLockonMarkeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", on__Q25Gauge13CLockonMarker);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", off__Q25Gauge13CLockonMarker);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", isFlag__Q25Gauge13CLockonMarker);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setFlag__Q25Gauge13CLockonMarkerUiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", changeMode__Q25Gauge13CLockonMarker);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", getTarget__Q25Gauge13CLockonMarker);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", getLockon__Q25Gauge13CLockonMarker);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", getPos__Q25Gauge13CLockonMarker);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", getUseCardPos__Q25Gauge13CLockonMarker);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", getCameraTargetPos__Q25Gauge13CLockonMarker);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", changeMode__Q25Gauge13CLockonMarkeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", getMode__Q25Gauge13CLockonMarker);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _GLOBAL_$I$c__KHCOM_MainApp_Game_Player_PlayerGauge.cppWQaaaa);
