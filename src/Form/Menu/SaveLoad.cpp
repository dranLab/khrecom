#include "khtypes.h"
#include "Form/Menu/SaveLoad.h"
#include "Form/Menu/SaveLoadCtrl.h"
#include "Form/Menu/SaveLoadUtil.h"
#include "System/Fsm.h"
#include "System/TaskMan.h"
#include "System/Debug.h"
#include "System/PadHandler.h"
#include "System/Sound/SoundMan.h"
#include "Game/GameWork.h"
#include <string.h>
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
public:
    enum LIGHTMARK_TYPE {
        LIGHTMARK_TYPE_RESULT = 0,
        LIGHTMARK_TYPE_CURSOR = 1
    };

private:
    CStateTemplate<CSaveLoadUI> m_State[19];     /* 0x06c */
    INT m_nNextState;                            /* 0x2cc */
    HANDLE m_hParent;                            /* 0x2d0 */
    SAVELOAD m_mode;                             /* 0x2d4 */
    CMemocaAgent* m_pMemoca;                     /* 0x2d8 */
    CMemocaDataEntries* m_pGameDataEntries;      /* 0x2dc */
    HANDLE m_hLay;                               /* 0x2e0 */
    HANDLE m_hCategory;                          /* 0x2e4 */
    CHAR* m_pszLad;                              /* 0x2e8 */
    INT m_nLastResult;                           /* 0x2ec */
    SHORT m_nMainSeqUnit;                        /* 0x2f0 */
    SHORT m_nIconSeqUnit;                        /* 0x2f2 */
    SHORT m_nFlag;                               /* 0x2f4 */
    SHORT m_nSlot;                               /* 0x2f6 */
    SHORT m_nDlgType;                            /* 0x2f8 */
    SHORT m_nDlgResult;                          /* 0x2fa */
    SHORT m_nDlgSel;                             /* 0x2fc */
    C2DSeqAgent* m_pMainFrameAgent;              /* 0x300 */
    C2DSeqAgent* m_pRoomNameAgent;               /* 0x304 */
    D2::C2DSeqCtrl* m_pRoomNameCtrl;             /* 0x308 */
    C2DSeqAgent* m_pWorldIconAgent;              /* 0x30c */
    D2::C2DSeqCtrl* m_pWorldIconCtrl;            /* 0x310 */
    C2DSeqAgent* m_pMainFrameHelpAgent;          /* 0x314 */
    D2::C2DSeqCtrl* m_pMainFrameHelpCtrl;        /* 0x318 */
    D2::C2DFnt* m_pMainFrameHelpFnt;             /* 0x31c */
    char m_szMainFrameHelpStr[128];              /* 0x320 */
    C2DSeqCursorCtrl* m_pMainCursor;             /* 0x3a0 */
    C2DSeqLightMarkAgent* m_pMainLightMark;      /* 0x3a4 */
    C2DSeqAgent* m_pTimeLabelAgent;              /* 0x3a8 */
    D2::C2DSeqCtrl* m_pTimeLabelCtrl;            /* 0x3ac */
    C2DSeqTimeCtrl* m_pTimeCtrl;                 /* 0x3b0 */
    C2DSeqAgent* m_pMPLabelAgent;                /* 0x3b4 */
    D2::C2DSeqCtrl* m_pMPLabelCtrl;              /* 0x3b8 */
    C2DSeqNumericCtrl* m_pMPCtrl;                /* 0x3bc */
    D2::C2DSeqCtrl* m_pResultPlateCtrl[2];       /* 0x3c0 */
    C2DSeqAgent* m_pResultPlateAgent[2];         /* 0x3c8 */
    D2::C2DFnt* m_pResultPlateFnt[2];            /* 0x3d0 */
    C2DSeqLightMarkAgent* m_pResultLightAgent[2]; /* 0x3d8 */
    C2DSeqSelectable* m_pSelSlotPlate[2];        /* 0x3e0 */
    C2DSeqAgent* m_pMemocaInfoAgent;             /* 0x3e8 */
    D2::C2DSeqCtrl* m_pMemocaInfoCtrl;           /* 0x3ec */
    D2::C2DSeqCtrl* m_pMemocaInfoBar[3];         /* 0x3f0 */
    char m_szMemocaInfoStr[3][32];               /* 0x3fc */
    C2DSeqAgent* m_pDataListContainerAgent;      /* 0x45c */
    D2::C2DSeqCtrl* m_pDataListContainerCtrl;    /* 0x460 */
    C2DSeqSelectable* m_pDataItemPlate[4];       /* 0x464 */
    D2::C2DSeqCtrl* m_pDataItemNumCircle[4];     /* 0x474 */
    D2::C2DFnt* m_pDataItemNumFnt[4];            /* 0x484 */
    D2::C2DSeqCtrl* m_pDataItemIcon[4];          /* 0x494 */
    D2::C2DSeqCtrl* m_pDataItemClearIcon[4];     /* 0x4a4 */
    char m_szDataItemContent[4][128];            /* 0x4b4 */
    char m_szDataItemID[4][4];                   /* 0x6b4 */
    char m_szDataItemTime[4][8];                 /* 0x6c4 */
    C2DSeqScrollbarCtrl* m_pDataListScrollbar;   /* 0x6e4 */
    SHORT m_nDataListTop;                        /* 0x6e8 */
    SHORT m_nDataListSel;                        /* 0x6ea */
    SHORT m_nStep;                               /* 0x6ec */
    SHORT m_nTimer;                              /* 0x6ee */
    SHORT m_nExecError;                          /* 0x6f0 */
    SHORT m_nTargetIndex;                        /* 0x6f2 */
    char m_szTargetFile[128];                    /* 0x6f4 */
    C2DSeqAgent* m_pMessageDlgAgent;             /* 0x774 */
    D2::C2DSeqCtrl* m_pMessageDlgCtrl;           /* 0x778 */
    D2::C2DFnt* m_pMessageDlgFntInf;             /* 0x77c */
    char m_szMessageDlgStr[256];                 /* 0x780 */
    C2DSeqSelectable* m_pMessageDlgBtnAgent[2];  /* 0x880 */
    C2DSeqCursorCtrl* m_pMessageDlgCursor;       /* 0x888 */
    static HANDLE m_hThis;

public:
    CSaveLoadUI(CSaveLoad& parent, SAVELOAD mode, GAMECLEAR clear);
    virtual ~CSaveLoadUI();
    virtual void main();
    void changeState(INT nNextState) { gotoState(m_State[nNextState]); }
    void changeNextState() {
        if (m_nNextState >= 0) changeState(m_nNextState);
    }

private:
    INT getRsrcType() const;
    void continueProc() { m_nFlag |= 1; }
    void preInit();
    void exeInit();
    void endInit();
    void preCheckBoot();
    void exeCheckBoot();
    void preLayIn();
    void exeLayIn();
    void preLayOut();
    void exeLayOut();
    void preExit();
    void exeExit();
    void preNop();
    void exeNop();
    void preOpenSlot();
    void exeOpenSlot();
    void preSelSlot();
    void exeSelSlot();
    void endSelSlot();
    void preCheckSlot();
    void exeCheckSlot();
    void endCheckSlot();
    void preOpenDataList();
    void exeOpenDataList();
    void preSelDataList();
    void exeSelDataList();
    void endSelDataList();
    void backtoSelSlot();
    void preSaveData();
    void exeSaveData();
    void preLoadData();
    void exeLoadData();
    void preCheckOverwrite();
    void exeCheckOverwrite();
    void preCheckNoMemory();
    void exeCheckNoMemory();
    void preCheckNoCard();
    void exeCheckNoCard();
    void preCheckUnformat();
    void exeCheckUnformat();
    void preConfirmExecErr();
    void exeConfirmExecErr();
    void preMessageOnly();
    void exeMessageOnly();
    C2DSeqAgent* createMainFrame();
    C2DSeqAgent* createWorldIcon(D2::C2DSeqCtrl*& pOutCtrl);
    C2DSeqAgent* createRoomName(D2::C2DSeqCtrl*& pOutCtrl);
    C2DSeqCursorCtrl* createCursor();
    C2DSeqLightMarkAgent* createLightMark(LIGHTMARK_TYPE nType, INT nLayer, BOOL bFollow);
    C2DSeqAgent* createTimeLabel(D2::C2DSeqCtrl*& pOutCtrl);
    C2DSeqTimeCtrl* createTimeCtrl();
    C2DSeqAgent* createMPLabel(D2::C2DSeqCtrl*& pOutCtrl);
    C2DSeqNumericCtrl* createMPCtrl();
    C2DSeqAgent* createMainHelp(D2::C2DSeqCtrl*& pOutCtrl, D2::C2DFnt*& pOutFnt);
    C2DSeqAgent* createResultPlate(D2::C2DSeqCtrl*& pOutCtrl, D2::C2DFnt*& pOutFnt, INT nIdx);
    C2DSeqSelectable* createSelSlotPlate(INT nIdx);
    C2DSeqAgent* createMemocaInfo(D2::C2DSeqCtrl*& pOutCtrl, D2::C2DSeqCtrl** ppOutInfoBar);
    C2DSeqAgent* createDataListContainer(D2::C2DSeqCtrl*& pOutCtrl, C2DSeqScrollbarCtrl*& pOutScrollbar);
    C2DSeqSelectable* createDataItemPlate(D2::C2DSeqCtrl& parent, D2::C2DSeqCtrl*& pOutNumCircle, D2::C2DFnt*& pOutNumFnt,
                                          D2::C2DSeqCtrl*& pOutIcon, D2::C2DSeqCtrl*& pOutClearIcon, INT nIdx);
    C2DSeqAgent* createMessageDlg(D2::C2DSeqCtrl*& pOutCtrl, D2::C2DFnt*& pOutFntInf);
    C2DSeqSelectable* createMessageDlgBtn(D2::C2DSeqCtrl* pParent, INT nIdx);
    void setActiveSlotPlate(INT nSlot);
    void setupDataPlate(SHORT nDataIdx);
    void updateMemocaInfoBar();
    void updateMemocaInfoStr();
    void setHelpMessage(const CHAR* pszMes);
    void outHelpMessage();
    void openMessageDlg(const CHAR* pszTxt, INT nIcon, INT nType);
    void closeMessageDlg();
    BOOL exeMessageDlg();
    INT getDlgResult() const { return m_nDlgResult; }
    void outAllAgent();
    void outDataList(INT nNextState);
    static void callbackMainFrame(C2DSeqAgent& agent);
    static void callbackSlotPlateIn(C2DSeqAgent& agent);
    static void callbackResultPlate(C2DSeqAgent& agent);
    static void callbackDataList(C2DSeqAgent& agent);
    static void callbackMemocaInfo(C2DSeqAgent& agent);
    static void callbackMessageDlg(C2DSeqAgent& agent);
    void lockPadControl(BOOL bLock) {
        if (bLock) m_nFlag |= 2;
        else m_nFlag &= ~2;
    }
    BOOL isPadEnable() const { return !(m_nFlag & 2); }
    INT isButtonRpt(UINT b) const {
        if (!isPadEnable()) return 0;
        return CPadHandler::isButtonRpt(b);
    }
    INT isButtonTrg(UINT b) const {
        if (!isPadEnable()) return 0;
        return CPadHandler::isButtonTrg(b);
    }
    BOOL isTrgOK() const { return isPadEnable() ? CPadHandler::isTrgOK() : FALSE; }
    BOOL isTrgCANCEL() const { return isPadEnable() ? CPadHandler::isTrgCANCEL() : FALSE; }
    BOOL checkSelector(SHORT& nOutIdx, SHORT nMax, INT nValDownKey, INT nValUpKey, SHORT nUpDownValue, BOOL bSetSE,
                       BOOL bLoopChk);
    BOOL checkSelectorV(SHORT& nOutIdx, SHORT nMax, BOOL bSetSE, BOOL bLoopChk) {
        return checkSelector(nOutIdx, nMax, 0x101000, 0x204000, 1, bSetSE, bLoopChk);
    }
    BOOL checkSelectorH(SHORT& nOutIdx, SHORT nMax, BOOL bSetSE, BOOL bLoopChk) {
        return checkSelector(nOutIdx, nMax, 0x408000, 0x802000, 1, bSetSE, bLoopChk);
    }
    void createBootInfoMsg(INT nRes, CHAR* pszOutMsg);
    void createCheckingMsg(INT nSlot, CHAR* pszOutMsg);
};
typedef char _check_saveloadui_size[sizeof(CSaveLoadUI) == 0x890 ? 1 : -1];

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

static inline BOOL _isFadeDone() { return s_pFadeMask ? s_pFadeMask->isFadeDone() : TRUE; }
static inline BOOL _isAgentState(C2DSeqAgent* p, INT nState) { return p->getState() == nState; }

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
void CSaveLoadUI::endInit() {}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preCheckBoot__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeCheckBoot__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preLayIn__11CSaveLoadUI);
void CSaveLoadUI::exeLayIn() {}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preLayOut__11CSaveLoadUI);
void CSaveLoadUI::exeLayOut() {
    if (m_mode != SLMODE_LOAD) return;
    if (m_nStep != 0) return;
    if (_isFadeDone()) {
        outAllAgent();
        m_nStep++;
    }
}
void CSaveLoadUI::preExit() {
    CGameWork::setByte(1, 0x140, 0);
    out();
}
void CSaveLoadUI::exeExit() {}
void CSaveLoadUI::preNop() {}
void CSaveLoadUI::exeNop() {}
void CSaveLoadUI::preOpenSlot() {
    for (INT i = 0; i < 2; i++) {
        m_pSelSlotPlate[i]->startOrRestart();
    }
}
void CSaveLoadUI::exeOpenSlot() {}
void CSaveLoadUI::preSelSlot() {
    setActiveSlotPlate(m_nSlot);
    m_pMainLightMark->m_nFlag &= ~2;
}
void CSaveLoadUI::exeSelSlot() {
    if (isTrgCANCEL()) {
        changeState(3);
        return;
    }
    if (isTrgOK()) {
        changeState(8);
        for (INT i = 0; i < 2; i++) {
            m_pSelSlotPlate[i]->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
        }
        CSound::playSE(2, -1, 64);
        return;
    }
    if (checkSelectorV(m_nSlot, 2, TRUE, TRUE)) setActiveSlotPlate(m_nSlot);
}
void CSaveLoadUI::endSelSlot() {
    if (m_pMainCursor) m_pMainCursor->enableDisp(FALSE);
    if (m_pMainLightMark && m_pMainLightMark->isSeqCtrlVisible()) {
        m_pMainLightMark->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
        m_pMainLightMark->m_nFlag |= 2;
    }
    outHelpMessage();
}
void CSaveLoadUI::preCheckSlot() {
    CHAR szMsg[256];
    m_pResultPlateAgent[1]->startOrRestart();
    m_pMemoca->setCurrentSlot(m_nSlot);
    createCheckingMsg(m_nSlot + 1, szMsg);
    openMessageDlg(szMsg, 0, 0);
    m_nDataListTop = 0;
    m_nDataListSel = 0;
    m_pGameDataEntries->clearAll();
    m_nStep = 0;
    m_nTargetIndex = 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeCheckSlot__11CSaveLoadUI);
void CSaveLoadUI::endCheckSlot() {}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preOpenDataList__11CSaveLoadUI);
void CSaveLoadUI::exeOpenDataList() {}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preSelDataList__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeSelDataList__11CSaveLoadUI);
void CSaveLoadUI::endSelDataList() {
    outHelpMessage();
}
void CSaveLoadUI::backtoSelSlot() {
    outDataList(6);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preSaveData__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeSaveData__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preLoadData__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeLoadData__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preCheckOverwrite__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeCheckOverwrite__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preCheckNoMemory__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeCheckNoMemory__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preCheckNoCard__11CSaveLoadUI);
void CSaveLoadUI::exeCheckNoCard() {
    INT nResult = -9999;
    switch (m_nStep) {
    case 0:
        if (m_pMemoca->reqIdleCheckCard()) m_nStep++;
        break;
    case 1:
        if (m_pMemoca->isCompleteReq(NULL)) {
            nResult = m_pMemoca->getLastResult();
            m_nStep = 0;
        }
        break;
    default:
        return;
    }
    if (nResult == 0) {
        closeMessageDlg();
        outDataList(8);
        m_nStep = 99;
        return;
    }
    if (m_nDlgResult) {
        m_nStep = 99;
        closeMessageDlg();
        backtoSelSlot();
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preCheckUnformat__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeCheckUnformat__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preConfirmExecErr__11CSaveLoadUI);
void CSaveLoadUI::exeConfirmExecErr() {
    switch (m_nStep) {
    case 0:
        if (m_pMemoca->reqIdleCheckCard()) m_nStep++;
        break;
    case 1:
        if (m_pMemoca->isCompleteReq(NULL)) m_nStep = 0;
        break;
    }
    switch (m_nStep) {
    case 0:
    case 1:
        if (m_nDlgResult) {
            closeMessageDlg();
            m_nStep = 2;
        }
        break;
    case 2:
        if (isPadEnable()) backtoSelSlot();
        break;
    }
}
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
void CSaveLoadUI::setHelpMessage(const CHAR* pszMes) {
    strcpy(m_szMainFrameHelpStr, pszMes);
    if (_isAgentState(m_pMainFrameHelpAgent, C2DSeqAgent::_STATE_NOT_START)) m_pMainFrameHelpAgent->start();
    else if (m_pMainFrameHelpAgent->getState() != C2DSeqAgent::STATE_LOOP) m_pMainFrameHelpAgent->changeState(C2DSeqAgent::STATE_IN, TRUE, -1);
}
void CSaveLoadUI::outHelpMessage() {
    m_pMainFrameHelpAgent->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", openMessageDlg__11CSaveLoadUIPCcii);
void CSaveLoadUI::closeMessageDlg() {
    m_pMessageDlgAgent->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    for (INT i = 0; i < 2; i++) {
        m_pMessageDlgBtnAgent[i]->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    }
    m_nFlag &= ~4;
    m_pMessageDlgCursor->enableDisp(FALSE);
    if (m_nDlgType && !(m_nFlag & 0x20) && m_pMainLightMark->getState() < C2DSeqAgent::STATE_OUT) {
        m_pMainLightMark->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeMessageDlg__11CSaveLoadUI);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", outAllAgent__11CSaveLoadUI);
void CSaveLoadUI::outDataList(INT nNextState) {
    if (m_pResultPlateAgent[1]->getState() < C2DSeqAgent::STATE_OUT) {
        m_pResultPlateAgent[1]->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
        m_pResultLightAgent[1]->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    }
    if (m_pDataListContainerAgent && m_pDataListContainerAgent->getState() < C2DSeqAgent::STATE_OUT) {
        if (m_pMemocaInfoAgent) {
            m_pMemocaInfoAgent->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
            for (INT i = 2; i >= 0; i--) {
                m_szMemocaInfoStr[i][0] = 0;
            }
        }
        m_pDataListContainerAgent->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
        C2DSeqScrollbarCtrl* pScrollbar = m_pDataListScrollbar;
        for (INT i = 0; i < 3; i++) {
            pScrollbar->m_pParts[i]->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
        }
        for (INT i = 0; i < 4; i++) {
            m_pDataItemPlate[i]->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
        }
        m_nNextState = nNextState;
        if (m_pMainCursor) m_pMainCursor->enableDisp(FALSE);
        if (m_pMainLightMark && m_pMainLightMark->isSeqCtrlVisible()) {
            m_pMainLightMark->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
            m_pMainLightMark->m_nFlag |= 2;
        }
    } else {
        if (nNextState >= 0) changeState(nNextState);
    }
}
void CSaveLoadUI::callbackMainFrame(C2DSeqAgent& agent) {
    CSaveLoadUI* p = (CSaveLoadUI*)CTaskMan::getTask(m_hThis);
    if (!p) return;
    switch (agent.getState()) {
    case C2DSeqAgent::STATE_IN:
        CSound::setBGMMasterVolume(64, -1);
        p->changeState(6);
        if (p->m_pRoomNameCtrl) p->m_pRoomNameCtrl->enableDisp(TRUE);
        if (p->m_pWorldIconCtrl) p->m_pWorldIconCtrl->enableDisp(TRUE);
        if (p->m_pTimeCtrl) p->m_pTimeCtrl->show();
        if (p->m_pMPCtrl) p->m_pMPCtrl->show();
        break;
    case C2DSeqAgent::STATE_OUT:
        p->changeState(4);
        break;
    }
}
void CSaveLoadUI::callbackSlotPlateIn(C2DSeqAgent& agent) {
    CSaveLoadUI* p = (CSaveLoadUI*)CTaskMan::getTask(m_hThis);
    if (p) p->changeState(7);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", callbackResultPlate__11CSaveLoadUIR11C2DSeqAgent);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", callbackDataList__11CSaveLoadUIR11C2DSeqAgent);
void CSaveLoadUI::callbackMemocaInfo(C2DSeqAgent& agent) {
    CSaveLoadUI* p = (CSaveLoadUI*)CTaskMan::getTask(m_hThis);
    if (!p) return;
    switch (agent.getState()) {
    case C2DSeqAgent::STATE_IN:
        p->updateMemocaInfoStr();
        p->updateMemocaInfoBar();
        break;
    case C2DSeqAgent::STATE_OUT:
        for (INT i = 0; i < 3; i++) {
            p->m_pMemocaInfoBar[i]->enableDisp(FALSE);
        }
        break;
    }
}
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
