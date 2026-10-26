#include "khtypes.h"
#include "Game/Atel/VSME.h"
#include "Game/Atel/Atel.h"
#define GAME_ATEL_AVATAR_CPP /* the full CAvatar class (the other includers get a stub) */
#include "Game/Atel/Avatar.h"
#include "Form/Menu/WorldCard.h"
#include "Form/Menu/LevelUp.h"
inline void* operator new(size_t, void* p) throw() { return p; }
/* PARTIAL: the original System/HeapMan.h(134) has it inline */
inline void* operator new(size_t size) throw() { return com_malloc(size); }
#include "Game/Atel/AtelCrossFade.h"
#include "System/Rcf.h"
#include "Game/GameWork.h"
#include "System/Debug.h"
#include "System/TaskMan.h"
#include "Form/DebugWindow.h"
#include "Game/Debug/DebugWork.h"
#include <string.h>
#include "include_asm.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);

using namespace ATEL;

extern "C" char D_0048CD50[];
extern "C" char D_00491328[];
extern "C" char D_00491338[];
extern "C" char D_00491348[];
extern "C" char D_00491358[];
extern "C" char D_00491370[];
extern "C" char D_00491398[];
extern "C" char D_004913C0[];
extern "C" char D_004913E8[];
extern "C" char D_00491410[];
extern "C" char D_00491438[];
extern "C" char D_00491460[];
extern "C" char D_00491488[];
extern "C" char D_004914B0[];
extern "C" char D_004914D8[];
extern "C" char D_00491500[];
extern "C" char D_00491538[];
extern "C" char D_00491568[];
extern "C" char D_00491590[];
extern "C" char D_004915B8[];
extern "C" char D_004915E0[];
extern "C" char D_00491610[];
extern "C" char D_00491638[];
extern "C" char D_00491668[];
extern "C" char D_004916A0[];
extern "C" char D_004916D0[];
extern "C" char D_004916E8[];
extern "C" char D_00491700[];
extern "C" char D_00491718[];
extern "C" char D_004917D8[];
extern "C" char D_004917F0[];
extern "C" char D_00491810[];
extern "C" char D_00491830[];
extern "C" char D_00491850[];
extern "C" char D_00491868[];
extern "C" char D_004918D0[];
extern "C" char D_004918F0[];
extern "C" char D_00491918[];
extern "C" char D_00491A70[];
extern "C" char D_00491AE0[];
extern "C" char D_00491AF0[];
extern "C" char D_00491B08[];
extern "C" char D_00491B20[];
extern "C" char D_00491B40[];
static inline void _setRTP(RTP& r, USHORT lv, USHORT actor, SHORT tag, SHORT mactor) {
    r.rq_lv = lv;
    r.rq_actor = actor;
    r.rq_tag = tag;
    r.rq_m_actor = mactor;
}
namespace Gauge {
class CPrizeGauge { /* PARTIAL: Game/Player/PlayerGauge.h declares the class only for its own unit */
public:
    static BOOL isNowDispCard();
};
}

namespace DWTEvent {
static BOOL DebugSetNpcIDWindow(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x20) {
        DebugWork::g_dw.event.iNpcID = inY;
        CActorDebug* p;
        if (DebugWork::g_dw.event.pClass) {
            p = (CActorDebug*)DebugWork::g_dw.event.pClass;
            delete p;
        }
        p = new CActorDebug(CTaskMan::getSysTask(1));
        DebugWork::g_dw.event.pClass = (INT*)p;
        DebugWork::g_dw.event.iNpcCoordDisp = 1;
    }
    return 1;
}
}
/* the asm .data (debug window tables) refers to it by name until the data comes from source */
__asm__(".globl DebugSetNpcIDWindow__8DWTEventP12CDebugWindowii");
namespace DWTEvent {
static INT debugWinNPCListAllCall(CDebugWindow* pObj, INT inParam) {
    CTask* pA;
    INT ret;
    if (inParam & 1) {
        pA = CTaskMan::getSysTask(0x1A);
        ret = 0;
        if (pA && ((CAtel*)pA)->getVSM()) ret = ((CAtel*)pA)->getVSM()->getActorNum();
        return ret;
    }
    return 1;
}
}
/* the asm .data (debugWinNPCList table) refers to it by name until the data comes from source */
__asm__(".globl debugWinNPCListAllCall__8DWTEventP12CDebugWindowi");
namespace DWTEvent {
static INT debugWinNPCListDispCall(LPSTR pStr, INT iLineY, const void* pValue) {
    CTask* pA = CTaskMan::getSysTask(0x1A);
    if (pA && ((CAtel*)pA)->getVSM()) sprintf(pStr, D_0048CD50, ((CAtel*)pA)->getVSM()->getAvatarName(iLineY));
    return 1;
}
}
/* the asm .data (debugWinNPCList table) refers to it by name until the data comes from source */
__asm__(".globl debugWinNPCListDispCall__8DWTEventPciPCv");
namespace DWTEvent {
static BOOL DebugCreateNpcWindow(CDebugWindow* pObj, INT inY, INT inParam) {
    CTask* pA = CTaskMan::getSysTask(0x1A);
    if (pA && ((CAtel*)pA)->getVSM()) {
        if (inParam & 0x180) {
            if (DebugWork::g_dw.event.iNpcCoordDisp == 1) {
                CActorDebug* p = new CActorDebug(CTaskMan::getSysTask(1));
                DebugWork::g_dw.event.pClass = (INT*)p;
            }
        }
    }
    return 1;
}
}
/* the asm .data (debug window tables) refers to it by name until the data comes from source */
__asm__(".globl DebugCreateNpcWindow__8DWTEventP12CDebugWindowii");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", DebugCreateEventLineDisp__8DWTEventP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", DebugAtelWorkDef__8DWTEventP12CDebugWindowi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", debugBtlReportCallBackParam__8DWTEventP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", debugBtlReportCallBack__8DWTEventP12CDebugWindowi);
namespace DWTEvent {
static INT debugKeyTresurCallBack(CDebugWindow* pObj, INT inParam) {
    DebugWork::g_dw.atelwork.TreTr = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[1];
    DebugWork::g_dw.atelwork.TreAg = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[2];
    DebugWork::g_dw.atelwork.TreOl = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[3];
    DebugWork::g_dw.atelwork.TreWo = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[4];
    DebugWork::g_dw.atelwork.TreMo = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[5];
    DebugWork::g_dw.atelwork.TreHa = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[6];
    DebugWork::g_dw.atelwork.TreAt = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[7];
    DebugWork::g_dw.atelwork.TreNe = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[8];
    DebugWork::g_dw.atelwork.TreHo = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[9];
    DebugWork::g_dw.atelwork.TrePo = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[10];
    DebugWork::g_dw.atelwork.TreTw = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[11];
    DebugWork::g_dw.atelwork.TreDe = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[12];
    DebugWork::g_dw.atelwork.TreCa = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[13];
    return 1;
}
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", DebugSetWorldTitleFlag__8DWTEventP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", DebugSetTutorialFlag__8DWTEventP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", DebugSetCardPossibleFlag__8DWTEventP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", DebugSetFriendPossibleFlag__8DWTEventP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", DebugSetPoohFlag__8DWTEventP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", DebugSetPcSkillFlag__8DWTEventP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", DebugSetSoraMovieFlag__8DWTEventP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", DebugSetRikuMovieFlag__8DWTEventP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", DebugSetKH2CardGetFlag__8DWTEventP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", DebugSetJiminyMemoFlag__8DWTEventP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", debugGameInfomationParam__8DWTEventP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", DebugSetGameWorkParam__8DWTEventP12CDebugWindowii);
namespace DWTEvent {
static INT debugWinNPCMainlCall(CDebugWindow* pObj, INT inParam) {
    if (inParam & 1) {
        DebugWork::g_dw.event.pClass = NULL;
        DebugWork::g_dw.event.pEventlineClass = NULL;
    }
    return 1;
}
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", _TRACE__4ATELiPCce);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", __Q24ATEL4CVSMR5CAteli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", _$_Q24ATEL4CVSM);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", initActor__Q24ATEL4CVSM);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", loadFromMem__Q24ATEL4CVSMPCUc);
void CVSM::deleteActorAll() {}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", checkEvents__Q24ATEL4CVSM);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", searchLockOnNpc__Q24ATEL4CVSM);
void CVSM::clearReq() {
    RTP* p = m_ReqBuf;
    for (INT i = 0; i < 64; i++, p++) {
        p->rq_lv = 0;
        p->rq_actor = 0;
        p->rq_tag = 0;
        p->rq_m_actor = 0xFF;
        p->status = 0;
        p->wait_key = 0;
    }
    m_nReqBufPtr = 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", setReq__Q24ATEL4CVSMRCQ24ATEL3RTP);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", tuneReq__Q24ATEL4CVSM);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", exec__Q24ATEL4CVSMi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", execInit__Q24ATEL4CVSM);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", execActor__Q24ATEL4CVSM);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", execCode__Q24ATEL4CVSMi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", execStdCode__Q24ATEL4CVSMiRiRQ24ATEL13eATEL_REQ_RET);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", execExt1Code__Q24ATEL4CVSMiRQ24ATEL13eATEL_REQ_RET);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", execExt2Code__Q24ATEL4CVSMiRQ24ATEL13eATEL_REQ_RET);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", execExt3Code__Q24ATEL4CVSMiRQ24ATEL13eATEL_REQ_RET);
INT CVSM::getActorIndex(const CHAR* pszName) const {
    for (INT i = 0; i < m_ActorCount; i++) {
        if (strcmp(pszName, m_ActorTbl[i]._ATP.name) == 0) return i;
    }
    return -1;
}
INT CVSM::getActorIndex(INT nID) const {
    for (INT i = 0; i < m_ActorCount; i++) {
        if (nID == m_ActorTbl[i]._ATP.id) return i;
    }
    return -1;
}
BOOL CVSM::reqN(Actor& act, INT nTag, INT nLv, BOOL bTop) {
    ATP& atp = act._ATP;
    atp.rq_lv = nLv;
    atp.status = 1;
    atp.rq_tag = nTag;
    atp.address = atp.loc[nTag];
    atp.rq_m_actor = 0xFF;
    if (!bTop) atp.Freg = 0;
    return TRUE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", reqNS__Q24ATEL4CVSMRQ24ATEL5ActorPCci);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", reqSS__Q24ATEL4CVSMPCcT1i);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", reqSSI__Q24ATEL4CVSMPCcii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", reqSSW__Q24ATEL4CVSMPCcT1ii);
BOOL CVSM::reqSSD(INT nTagIdx, INT nActIdx, INT nLv) {
    RTP rq;
    _setRTP(rq, nLv, nActIdx, nTagIdx, 0xFF);
    setReq(rq);
    return TRUE;
}
INT CVSM::searchActor(const CHAR* pszActor) const {
    TRACE(D_00491328, pszActor);
    INT i = getActorIndex(pszActor);
    if (i < 0) {
        _TRACE(TRUE, D_00491338);
        return -1;
    }
    return i;
}
INT CVSM::searchActorID(const CHAR* pszActor) const {
    TRACE(D_00491348);
    INT i = getActorIndex(pszActor);
    if (i < 0) {
        _TRACE(TRUE, D_00491358);
        return -1;
    }
    return m_ActorTbl[i]._ATP.id;
}
/* Parked: same instructions and order except the wait_timer product: the original multiplies into LO ("mult $zero,s1,v1"
   + mflo before the sh), ours with the 3-operand mult into v1 (gcc's mulsi3_mult3 "l" vs "d" alternative, chosen by
   regclass: LO only when the product feeds a plain register copy, as for a call argument in PathData::delPath); ours
   is 4 bytes shorter. Line tags: status (5114) before next_address (5115), the whole product on the isNTSC line. tried:
   ternary inside/outside the product and with commuted arms, if/else, INT/USHORT/TIMEQ product locals, an INT
   multiplier local, (UINT)/(SHORT)/float operands, an inline USHORT setter, statement orders, tools/permuter.py
   10 min (50056 candidates, no better than 4). */
#if 0
void CVSM::acWaitReq(Actor& act, INT nTime, INT nAddr) {
    ATP& atp = act._ATP;
    atp.status = 3;
    atp.next_address = nAddr;
    atp.wait_timer = nTime * (CMainApp::isNTSC() ? 25 : 25);
    _TRACE(m_bTrace, D_00491370, atp.id, nTime, nAddr);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", acWaitReq__Q24ATEL4CVSMRQ24ATEL5Actorii);
#endif
void CVSM::acWaitLoadRsrc(Actor& act, INT nAddr) {
    ATP& atp = act._ATP;
    atp.next_address = nAddr;
    atp.status = 26;
    atp.wait_key = 0;
    _TRACE(m_bTrace, D_00491398, atp.id, nAddr);
}
void CVSM::acWaitCardWind(Actor& act, INT nAddr) {
    ATP& atp = act._ATP;
    atp.next_address = nAddr;
    atp.status = 27;
    atp.wait_key = 0;
    _TRACE(m_bTrace, D_004913C0, atp.id, nAddr);
}
void CVSM::acWaitTechWind(Actor& act, INT nAddr) {
    ATP& atp = act._ATP;
    atp.next_address = nAddr;
    atp.status = 28;
    atp.wait_key = 0;
    _TRACE(m_bTrace, D_004913E8, atp.id, nAddr);
}
void CVSM::acWaitWorldCardWind(Actor& act, INT nAddr) {
    ATP& atp = act._ATP;
    atp.next_address = nAddr;
    atp.status = 29;
    atp.wait_key = 0;
    _TRACE(m_bTrace, D_00491410, atp.id, nAddr);
}
void CVSM::acWaitCrossFade(Actor& act, INT nAddr) {
    ATP& atp = act._ATP;
    atp.next_address = nAddr;
    atp.status = 30;
    atp.wait_key = 0;
    _TRACE(m_bTrace, D_00491438, atp.id, nAddr);
}
void CVSM::acWaitPad(Actor& act, INT nKey, INT nAddr) {
    ATP& atp = act._ATP;
    atp.status = 7;
    atp.wait_key = nKey;
    atp.next_address = nAddr;
    m_PadWaitTime = 1;
    _TRACE(m_bTrace, D_00491460, atp.id, nKey, nAddr);
}
void CVSM::acWaitMes(Actor& act, INT nKey, INT nAddr) {
    ATP& atp = act._ATP;
    atp.wait_key = nKey;
    atp.status = 9;
    atp.next_address = nAddr;
    _TRACE(m_bTrace, D_00491488, atp.id, nKey, nAddr);
}
void CVSM::acWaitCam(Actor& act, INT nKey, INT nAddr) {
    ATP& atp = act._ATP;
    atp.wait_key = nKey;
    atp.status = 10;
    atp.next_address = nAddr;
    _TRACE(m_bTrace, D_004914B0, atp.id, nKey, nAddr);
}
void CVSM::acWaitBusy(Actor& act, INT nKey, INT nAddr) {
    ATP& atp = act._ATP;
    atp.wait_key = nKey;
    atp.status = 20;
    atp.next_address = nAddr;
    _TRACE(m_bTrace, D_004914D8, atp.id, nKey, nAddr);
}
void CVSM::acWaitBusyActor(Actor& act, INT nKey, INT nWin, INT nAddr) {
    ATP& atp = act._ATP;
    atp.next_address = nAddr;
    atp.wait_key = nKey;
    atp.status = 21;
    atp.wait_timer = nWin;
    _TRACE(m_bTrace, D_00491500, atp.id, nKey, nWin, nAddr);
}
void CVSM::acWaitActor(Actor& act, INT nKey, INT nWin, INT nAddr) {
    ATP& atp = act._ATP;
    atp.next_address = nAddr;
    atp.wait_key = nKey;
    atp.status = 24;
    atp.wait_timer = nWin;
    _TRACE(m_bTrace, D_00491538, atp.id, nKey, nWin, nAddr);
}
void CVSM::acWaitRsrc(Actor& act, INT nKey, INT nAddr) {
    ATP& atp = act._ATP;
    atp.wait_key = nKey;
    atp.status = 16;
    atp.next_address = nAddr;
    _TRACE(m_bTrace, D_00491568, atp.id, nKey, nAddr);
}
void CVSM::acWaitSnatch(Actor& act, INT nKey, INT nAddr) {
    ATP& atp = act._ATP;
    atp.wait_key = nKey;
    atp.status = 11;
    atp.wait_timer = 1;
    atp.next_address = nAddr;
    _TRACE(m_bTrace, D_00491590, atp.id, nKey, nAddr);
}
void CVSM::acWaitEquip(Actor& act, INT nKey, INT nAddr) {
    ATP& atp = act._ATP;
    atp.wait_key = nKey;
    atp.status = 17;
    atp.next_address = nAddr;
    _TRACE(m_bTrace, D_004915B8, atp.id, nKey, nAddr);
}
void CVSM::acWaitDelLeftSword(Actor& act, INT nKey, INT nAddr) {
    ATP& atp = act._ATP;
    atp.wait_key = nKey;
    atp.status = 22;
    atp.next_address = nAddr;
    _TRACE(m_bTrace, D_004915E0, atp.id, nKey, nAddr);
}
void CVSM::acWaitLoadEnemy(Actor& act, INT nAddr) {
    ATP& atp = act._ATP;
    atp.next_address = nAddr;
    atp.status = 15;
    atp.wait_key = 0;
    _TRACE(m_bTrace, D_00491610, atp.id, nAddr);
}
void CVSM::acWaitSelectPad(Actor& act, INT nKey, INT nWin, INT nAddr) {
    ATP& atp = act._ATP;
    atp.wait_key = nWin;
    atp.wait_timer = nKey;
    atp.status = 8;
    atp.next_address = nAddr;
    _TRACE(m_bTrace, D_00491638, atp.id, nKey, nAddr);
}
void CVSM::acWaitSelectPadEx(Actor& act, INT nKey, INT nWin, INT nAddr) {
    ATP& atp = act._ATP;
    atp.wait_key = nWin;
    atp.wait_timer = nKey;
    atp.status = 19;
    atp.next_address = nAddr;
    _TRACE(m_bTrace, D_00491668, atp.id, nKey, nAddr);
}
void CVSM::acWaitChangeWork(Actor& act, INT nKey, INT nVal, INT nAddr) {
    ATP& atp = act._ATP;
    atp.wait_timer = nVal;
    atp.next_address = nAddr;
    atp.wait_key = nKey;
    atp.status = 18;
    _TRACE(m_bTrace, D_004916A0, nKey, nVal, nAddr);
}
BOOL CVSM::acStatNop() {
    return FALSE;
}
BOOL CVSM::acStatExec() {
    m_Pctr = m_pCurrentActor->_ATP.address;
    return TRUE;
}
BOOL CVSM::acStatWait() {
    ATP& atp = m_pCurrentActor->_ATP;
    TIMEQ timer = atp.wait_timer;
    TIMEQ nTQ = m_nFrameRate;
    if (CMainApp::isNTSC()) {
        nTQ >>= 1;
    } else {
        nTQ >>= 1;
    }
    if (timer < nTQ) {
        _TRACE(m_bTrace, D_004916D0, atp.id);
        setPC(atp.next_address);
        return TRUE;
    }
    atp.wait_timer = timer - nTQ;
    return FALSE;
}
BOOL CVSM::acStatRWait() {
    ATP& atp = m_pCurrentActor->_ATP;
    if (atp.wait_timer) return FALSE;
    _TRACE(m_bTrace, D_004916E8, atp.id);
    m_Pctr = atp.next_address;
    return TRUE;
}
BOOL CVSM::acStatCWait() {
    ATP& atp = m_pCurrentActor->_ATP;
    if (!m_pCurrentActor->_pAvatar->isAvatarBusy()) {
        _TRACE(m_bTrace, D_00491700, atp.id);
        m_Pctr = atp.next_address;
        return TRUE;
    }
    return FALSE;
}
BOOL CVSM::acStatIWait() {
    ATP& atp = m_pCurrentActor->_ATP;
    if (!(m_nFlag & 2)) {
        _TRACE(m_bTrace, D_00491718, atp.id, atp.address);
        m_Pctr = atp.address;
        return TRUE;
    }
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", acStatPWait__Q24ATEL4CVSM);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", acStatSWait__Q24ATEL4CVSM);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", acStatMWait__Q24ATEL4CVSM);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", acStatCPWait__Q24ATEL4CVSM);
BOOL CVSM::acStatSNWait() {
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", acStatMSWait__Q24ATEL4CVSM);
BOOL CVSM::acStatShopWait() {
    return FALSE;
}
BOOL CVSM::acStatDropWait() {
    return FALSE;
}
BOOL CVSM::acStatRsrcLoadWait() {
    if (CRcf::isCompleteLoadRCF(m_pCurrentActor->_GroupID, m_pCurrentActor->_nMembarID)) {
        _TRACE(m_bTrace, D_004917D8);
        m_Pctr = m_pCurrentActor->_ATP.next_address;
        return TRUE;
    }
    return FALSE;
}
BOOL CVSM::acStatCardWndWait() {
    if (Gauge::CPrizeGauge::isNowDispCard()) return FALSE;
    TRACE(D_004917F0);
    m_Pctr = m_pCurrentActor->_ATP.next_address;
    return TRUE;
}
BOOL CVSM::acStatTechWndWait() {
    if (CLevelUp::isEnd()) {
        TRACE(D_00491810);
        m_Pctr = m_pCurrentActor->_ATP.next_address;
        return TRUE;
    }
    return FALSE;
}
BOOL CVSM::acStatWorldCardWndWait() {
    if (WorldCard::CWorldCardSeq::isNowDispCard()) return FALSE;
    TRACE(D_00491830);
    m_Pctr = m_pCurrentActor->_ATP.next_address;
    return TRUE;
}
BOOL CVSM::acStatCrossFadeWait() {
    CAtelCrossFade* p = CAtelCrossFade::m_pInstance;
    if (p && p->isCrossFadeDrawImage()) return FALSE;
    TRACE(D_00491850);
    m_Pctr = m_pCurrentActor->_ATP.next_address;
    return TRUE;
}
BOOL CVSM::acStatEnemyLoadWait() {
    return FALSE;
}
BOOL CVSM::acStatRCWait() {
    m_Pctr = m_pCurrentActor->_ATP.next_address;
    return TRUE;
}
BOOL CVSM::acStatEQWait() {
    m_Pctr = m_pCurrentActor->_ATP.next_address;
    return TRUE;
}
BOOL CVSM::acStatWKWait() {
    m_Pctr = m_pCurrentActor->_ATP.next_address;
    return TRUE;
}
BOOL CVSM::acStatXWait() {
    if (CPadHandler::isTrgCANCEL()) {
        CGameWork::setByte(1, 0, 0xFF);
        _TRACE(m_bTrace, D_00491868, m_pCurrentActor->_ATP.id);
        m_Pctr = m_pCurrentActor->_ATP.next_address;
        return TRUE;
    }
    return acStatSWait();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", isAvatarBusy__Q24ATEL4CVSMiPQ24ATEL7CAvatar);
BOOL CVSM::acStatBSWait() {
    ATP& atp = m_pCurrentActor->_ATP;
    if (isAvatarBusy(atp.wait_key, m_pCurrentActor->_pAvatar)) return FALSE;
    _TRACE(m_bTrace, D_004918D0, atp.id, atp.wait_key);
    m_Pctr = atp.next_address;
    return TRUE;
}
BOOL CVSM::acStatBAWait() {
    ATP& atp = m_pCurrentActor->_ATP;
    if (isAvatarBusy(atp.wait_key, getAvatar(atp.wait_timer))) return FALSE;
    _TRACE(m_bTrace, D_004918F0, atp.id, atp.wait_key);
    m_Pctr = atp.next_address;
    return TRUE;
}
BOOL CVSM::acStatActorWait() {
    ATP& atp = m_pCurrentActor->_ATP;
    /* the original passes wait_timer (lhu 0x1b2) as the avatar argument */
    if (isAvatarBusy(atp.wait_key, (CAvatar*)atp.wait_timer) != TRUE) return FALSE;
    _TRACE(m_bTrace, D_00491918, atp.id, atp.wait_key);
    m_Pctr = atp.next_address;
    return TRUE;
}
BOOL CVSM::acStatEternityWait() {
    return FALSE;
}
BOOL CVSM::acStatDLWait() {
    m_Pctr = m_pCurrentActor->_ATP.next_address;
    return FALSE;
}
BOOL CVSM::acStatBGWait() {
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", player_ctrl__Q24ATEL4CVSMi);
INT CVSM::player_ctrl(INT nCode, FLOAT fArg) {
    if (m_pCurrentActor->_pAvatar) _TRACE(m_bTrace, D_00491A70);
    return -2;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", cam_ctrl__Q24ATEL4CVSMiiRi);
void CVSM::info_disp(INT nCmnd) {
    _TRACE(m_bTrace, D_00491AE0);
}
BOOL CVSM::actor_ref(INT nCmnd, INT nArg) {
    _TRACE(m_bTrace, D_00491AF0);
    return FALSE;
}
void CVSM::cam_ajt(INT nCmnd, F_UL& arg, INT nID) {
    _TRACE(m_bTrace, D_00491B08);
}
BOOL CVSM::player_rsrc(INT nCmnd, INT nArg) {
    _TRACE(m_bTrace, D_00491B20, nCmnd, nArg);
    _TRACE(m_bTrace, D_00491B40, nArg);
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", ctd_cal__Q24ATEL4CVSMPCQ24ATEL7CAvatariiPQ24ATEL21ChatWndCreateCTDParam);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", AtelEventSkip__Q24ATEL4CVSM);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", close_dest_se__Fi);
void WinOpenSE(INT ctd) {}
void WinCloseSE() {}
static void dispActorTag(INT nMax, const Actor* pTbl) {}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", __11CActorDebugP5CTask);
CActorDebug::~CActorDebug() {
    if (m_pWindow) m_pWindow->out();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", main__11CActorDebug);
CEventLineDisp::CEventLineDisp(CTask* pParent) : CTask(pParent, 0x8000) {
    CTaskMan::getSysTask(0x1A);
}
CEventLineDisp::~CEventLineDisp() {}
void CEventLineDisp::main() {}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", disp__14CEventLineDisp);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", __static_initialization_and_destruction_0_002505D8);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", _TRACE___4ATELiPCce);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", isInitComplete__CQ24ATEL4CVSM);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", startMain__Q24ATEL4CVSM);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", isMainStarted__CQ24ATEL4CVSM);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", getAvatar__Q24ATEL4CVSMi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", getEventTrap__Q24ATEL4CVSMi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", getAvatarName__Q24ATEL4CVSMi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", getActorNum__Q24ATEL4CVSM);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", setPC__Q24ATEL4CVSMi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", getPC__CQ24ATEL4CVSM);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", clearFlag__Q24ATEL4CVSMi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", setFlag__Q24ATEL4CVSMi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", isFlagOn__CQ24ATEL4CVSMi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", _GLOBAL_$I$c__KHCOM_MainApp_Game_Atel_VSME.cppEPaaaa);
