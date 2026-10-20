#include "khtypes.h"
#define GAME_PLAYER_PLAYERGAUGE_CPP
#include "Game/Player/PlayerGauge.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/TaskMan.h"
#include "Game/GameWork.h"
#include "Game/SceneEnv.h"
#include "include_asm.h"

using namespace Gauge;

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
inline void* operator new(unsigned int, void* p) throw() { return p; }
namespace Gauge {
extern LPCSTR g_szSeqCockpit;
}
extern "C" char D_004A3238[];

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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setHP__Q25Gauge12CPlayerGaugeii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setDamage__Q25Gauge12CPlayerGaugeii);
void CPlayerGauge::setBattleEnd() { CNaviMap::m_bBattleEnd = TRUE; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setDPGauge__Q25Gauge12CPlayerGaugei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setDP__Q25Gauge12CPlayerGaugeii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setDMode__Q25Gauge12CPlayerGaugeiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", exeDPGauge__Q25Gauge12CPlayerGauge);
CLockonMarker::CLockonMarker(CTask* inParent, BOOL inFlag) : CTask(inParent, 0x4000), m_pGauge(NULL), m_bFlag(inFlag) { _initialize(); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _$_Q25Gauge13CLockonMarker);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _initialize__Q25Gauge13CLockonMarker);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", main__Q25Gauge13CLockonMarker);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", calcMarkerScale__Q25Gauge13CLockonMarker);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setTarget__Q25Gauge13CLockonMarkeri);
void CLockonMarker::setLockon(HANDLE inTarget, INT inDist) {
    m_iDist = inDist;
    m_hTarget = inTarget;
    m_uiFlag = (INT)m_uiFlag & ~0x10;
    m_bTarget = FALSE;
    m_bLockon = FALSE;
    if (inTarget > 0 && CTaskMan::getTask(inTarget)) m_bLockon = TRUE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", chkLockon__Q25Gauge13CLockonMarkerRi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", cancelLockon__Q25Gauge13CLockonMarkerRii);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", __Q25Gauge11CEnemyGaugeP5CTask);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _$_Q25Gauge11CEnemyGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _initialize__Q25Gauge11CEnemyGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", main__Q25Gauge11CEnemyGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", resetHP__Q25Gauge11CEnemyGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setHP__Q25Gauge11CEnemyGaugeiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", notifyHP__Q25Gauge11CEnemyGaugeiii);
HANDLE CEnemyGauge::getNowTarget() {
    if (!m_pInstance) return 0;
    if (CGameWork::getInt(1, 0xe2) & 0x10) return 0;
    return m_pInstance->m_hTarget;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", __Q25Gauge12CCommonGaugeP5CTask);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _$_Q25Gauge12CCommonGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _initialize__Q25Gauge12CCommonGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", main__Q25Gauge12CCommonGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", dispSeqCtrl__Q25Gauge12CCommonGaugeiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setDispSeq__Q25Gauge12CCommonGaugeii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", __Q25Gauge11CPrizeGaugeP5CTask);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _$_Q25Gauge11CPrizeGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", _initialize__Q25Gauge11CPrizeGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", main__Q25Gauge11CPrizeGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setDispSeq__Q25Gauge11CPrizeGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setDispMogSeq__Q25Gauge11CPrizeGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setPrizeExp__Q25Gauge11CPrizeGaugei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setPrizeMp__Q25Gauge11CPrizeGaugei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setDropMp__Q25Gauge11CPrizeGaugei);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", main__Q25Gauge12CEscapeGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", setCharge__Q25Gauge12CEscapeGaugef);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", destroy__Q25Gauge12CEscapeGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerGauge", __Q25Gauge12CFriendGaugeP5CTask);
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
