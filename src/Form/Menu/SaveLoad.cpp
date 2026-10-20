#include "khtypes.h"
#include "Form/Menu/SaveLoad.h"
#include "Form/Menu/SaveLoadCtrl.h"
#include "Form/Menu/SaveLoadUtil.h"
#include "System/Fsm.h"
#include "System/TaskMan.h"
#include "System/Debug.h"
#include "System/Graphic/ScreenMask.h"
#include "System/Graphic/2d/2DFnt.h"
#include "include_asm.h"

inline void* operator new(unsigned int, void* p) throw() { return p; }
#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

namespace {
extern BOOL s_bSaveLoadDone;
extern CFadeIOMask* s_pFadeMask;
extern SLMSG_RESULT s_nMsgResult;
extern UINT s_nWorldIconRsrc;
extern SLMSG s_nMessage;
}
extern "C" HANDLE s_hSystemDataChecker;

extern "C" char D_004F3E80[];
extern "C" char D_004F3EB0[];
extern "C" char D_004F3EE0[];
extern "C" char D_004F3F10[];
extern "C" char D_004F3F40[];
extern "C" char D_004F3F70[];

class CSaveLoadUI : public CTask, public CFSM {
    UBYTE _m_0x6c[0x890 - 0x6c]; // PARTIAL
public:
    CSaveLoadUI(CSaveLoad& parent, SAVELOAD mode, GAMECLEAR clear);
    virtual ~CSaveLoadUI();
};

class CSystemDataCheckerLocal : public CTask {
private:
    CMemocaAgent* m_pMemoca; /* 0x40 */
    SHORT m_nStep;           /* 0x44 */
    USHORT m_nFlag;          /* 0x46 */

public:
    CSystemDataCheckerLocal(CTask* pParent) : CTask(pParent, 0x4000) {
        m_nStep = 0;
        m_nFlag = 0;
        s_hSystemDataChecker = getHandle();
        m_pMemoca = new (com_malloc_d(sizeof(CMemocaAgent), D_004F3F10, RET_ADDR())) CMemocaAgent(this);
    }
    virtual ~CSystemDataCheckerLocal() {}
    virtual void main();
    void abort() { m_nFlag |= 1; }
};

class CPlayer { /* PARTIAL: Game/Player/Player.h needs the game headers */
public:
    static void exportStatus();
};
class CRsrcDB { /* PARTIAL: System/RsrcDB.h needs RsrcData.h internals */
public:
    static void removeGroup(INT nGroupID, INT nMemberID);
};

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", __11CSaveLoadUIR9CSaveLoad8SAVELOAD9GAMECLEAR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", _$_11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preInit__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeInit__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", endInit__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preCheckBoot__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeCheckBoot__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preLayIn__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeLayIn__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preLayOut__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeLayOut__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preExit__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeExit__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preNop__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeNop__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preOpenSlot__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeOpenSlot__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preSelSlot__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeSelSlot__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", endSelSlot__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preCheckSlot__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeCheckSlot__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", endCheckSlot__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preOpenDataList__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeOpenDataList__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preSelDataList__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeSelDataList__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", endSelDataList__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", backtoSelSlot__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preSaveData__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeSaveData__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preLoadData__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeLoadData__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preCheckOverwrite__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeCheckOverwrite__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preCheckNoMemory__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeCheckNoMemory__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preCheckNoCard__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeCheckNoCard__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preCheckUnformat__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeCheckUnformat__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preConfirmExecErr__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeConfirmExecErr__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preMessageOnly__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeMessageOnly__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createMainFrame__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createWorldIcon__11CSaveLoadUIRPQ22D210C2DSeqCtrl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createRoomName__11CSaveLoadUIRPQ22D210C2DSeqCtrl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createCursor__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createLightMark__11CSaveLoadUIQ211CSaveLoadUI14LIGHTMARK_TYPEii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createTimeLabel__11CSaveLoadUIRPQ22D210C2DSeqCtrl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createTimeCtrl__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createMPLabel__11CSaveLoadUIRPQ22D210C2DSeqCtrl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createMPCtrl__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createMainHelp__11CSaveLoadUIRPQ22D210C2DSeqCtrlRPQ22D26C2DFnt);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createResultPlate__11CSaveLoadUIRPQ22D210C2DSeqCtrlRPQ22D26C2DFnti);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createSelSlotPlate__11CSaveLoadUIi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createMemocaInfo__11CSaveLoadUIRPQ22D210C2DSeqCtrlPPQ22D210C2DSeqCtrl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createDataListContainer__11CSaveLoadUIRPQ22D210C2DSeqCtrlRP19C2DSeqScrollbarCtrl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createDataItemPlate__11CSaveLoadUIRQ22D210C2DSeqCtrlRPQ22D210C2DSeqCtrlRPQ22D26C2DFntT2T2i);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createMessageDlg__11CSaveLoadUIRPQ22D210C2DSeqCtrlRPQ22D26C2DFnt);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createMessageDlgBtn__11CSaveLoadUIPQ22D210C2DSeqCtrli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", setActiveSlotPlate__11CSaveLoadUIi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", setupDataPlate__11CSaveLoadUIs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", updateMemocaInfoBar__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", updateMemocaInfoStr__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", setHelpMessage__11CSaveLoadUIPCc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", outHelpMessage__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", openMessageDlg__11CSaveLoadUIPCcii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", closeMessageDlg__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeMessageDlg__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", outAllAgent__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", outDataList__11CSaveLoadUIi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", callbackMainFrame__11CSaveLoadUIR11C2DSeqAgent);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", callbackSlotPlateIn__11CSaveLoadUIR11C2DSeqAgent);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", callbackResultPlate__11CSaveLoadUIR11C2DSeqAgent);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", callbackDataList__11CSaveLoadUIR11C2DSeqAgent);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", callbackMemocaInfo__11CSaveLoadUIR11C2DSeqAgent);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", callbackMessageDlg__11CSaveLoadUIR11C2DSeqAgent);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", checkSelector__11CSaveLoadUIRssiisii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createBootInfoMsg__11CSaveLoadUIiPc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createCheckingMsg__11CSaveLoadUIiPc);
CSaveLoad::CSaveLoad(CTask* pParent, SAVELOAD mode, GAMECLEAR clear) : CTask(pParent, 0x8000) {
    m_nFlag = 0;
    construct(mode);
    new (com_malloc_d(sizeof(CSaveLoadUI), D_004F3E80, RET_ADDR())) CSaveLoadUI(*this, mode, clear);
}
CSaveLoad::CSaveLoad(CTask* pParent, SLMSG msg) : CTask(pParent, 0x8000) {
    m_nFlag = 0;
    construct(SLMODE_MESSAGE);
    s_nMessage = msg;
    new (com_malloc_d(sizeof(CSaveLoadUI), D_004F3EB0, RET_ADDR())) CSaveLoadUI(*this, SLMODE_MESSAGE, GAMECLEAR_NONE);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", _$_9CSaveLoad);
void CSaveLoad::construct(SAVELOAD mode) {
    if (m_hThis) {
        TRACE(D_004F3EE0);
        out();
        return;
    }
    if (!CMainApp::isEnableSoftReset()) m_nFlag |= 4;
    else m_nFlag &= ~4;
    CMainApp::enableSoftReset(FALSE);
    if (CMainApp::isFpsHalf()) m_nFlag |= 2;
    else m_nFlag &= ~2;
    CMainApp::setFpsFull();
    ignorePause(TRUE);
    ignorePlaySpeed(TRUE);
    m_hThis = getHandle();
    s_bSaveLoadDone = FALSE;
    if (mode == SLMODE_SAVE) CPlayer::exportStatus();
}
BOOL CSaveLoad::isExited() {
    return m_hThis == 0;
}
void CSaveLoad::disposeRsrc() {
    CRsrcDB::removeGroup(2, 0x6B);
    CRsrcDB::removeGroup(2, 0x6A);
    delete s_pFadeMask;
    s_pFadeMask = NULL;
}
BOOL CSaveLoad::isSuccessLoad() {
    return s_bSaveLoadDone;
}
BOOL CSaveLoad::isSuccessSave() {
    return s_bSaveLoadDone;
}
SLMSG_RESULT CSaveLoad::getMsgResult() {
    return s_nMsgResult;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", main__23CSystemDataCheckerLocal);
void CSystemDataChecker::start() {
    if (s_hSystemDataChecker) {
        TRACE(D_004F3F40);
        return;
    }
    new (com_malloc_d(sizeof(CSystemDataCheckerLocal), D_004F3F70, RET_ADDR())) CSystemDataCheckerLocal(CTaskMan::getSysTask(0));
}
void CSystemDataChecker::abort() {
    if (!s_hSystemDataChecker) return;
    CSystemDataCheckerLocal* p = (CSystemDataCheckerLocal*)CTaskMan::getTask(s_hSystemDataChecker);
    if (p) p->abort();
    else s_hSystemDataChecker = 0;
}
BOOL CSystemDataChecker::isExited() {
    return s_hSystemDataChecker == 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", func_00350FC8);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", _$_16C2DSeqCursorCtrl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", _$_20C2DSeqLightMarkAgent);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", procLoop__20C2DSeqLightMarkAgent);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", onOutSeq__20C2DSeqLightMarkAgent);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", _$_16C2DSeqSelectable);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", dispose__16C2DSeqSelectable);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", _$_14C2DSeqTimeCtrl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", _$_17C2DSeqNumericCtrl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", _$_t14CStateTemplate1Z11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", main__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", changeState__11CSaveLoadUIi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", changeNextState__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", getRsrcType__C11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", continueProc__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", getDlgResult__C11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", lockPadControl__11CSaveLoadUIi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", isPadEnable__C11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", isButtonRpt__C11CSaveLoadUIUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", isButtonTrg__C11CSaveLoadUIUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", isTrgOK__C11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", isTrgCANCEL__C11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", checkSelectorV__11CSaveLoadUIRssii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", checkSelectorH__11CSaveLoadUIRssii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", __23CSystemDataCheckerLocalP5CTask);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", _$_23CSystemDataCheckerLocal);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", abort__23CSystemDataCheckerLocal);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", executeBeginState__t14CStateTemplate1Z11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", executeState__t14CStateTemplate1Z11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", executeEndState__t14CStateTemplate1Z11CSaveLoadUI);
