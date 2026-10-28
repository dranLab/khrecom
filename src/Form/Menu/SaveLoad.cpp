#include "khtypes.h"
#define SAVELOADCTRL_H_PATH "c:/KHCOM/MainApp/Form/Menu/SaveLoadCtrl.h"
/* .rodata starts with the strings of the included headers' inline functions, in this order. */
#include "Form/Menu/SaveLoad.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Form/Menu/SaveLoadCtrl.h"
#include "System/Ctd/CtdMan.h"
#include "System/Graphic/Anim.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"
#include "Form/Menu/SaveLoadUtil.h"
#include "System/Fsm.h"
#include "System/TaskMan.h"
#include "System/Debug.h"
#include "System/PadHandler.h"
#include "System/Sound/SoundMan.h"
#include "Game/GameWork.h"
#include <string.h>
extern "C" int sprintf(char* buf, const char* fmt, ...);
#include "System/Graphic/ScreenMask.h"
#include "System/Graphic/2d/2DFnt.h"
#include "include_asm.h"

inline void* operator new(unsigned int, void* p) throw() { return p; }
#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

namespace {
BOOL s_bSaveLoadDone = 0;
CFadeIOMask* s_pFadeMask = NULL;
SLMSG_RESULT s_nMsgResult = SLMSG_RESULT_CANCEL;
UINT s_nWorldIconRsrc = 0;
SLMSG s_nMessage;
}


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
    INT getRsrcType() const { return m_mode != SLMODE_SAVE; }
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
        if (isPadEnable()) return CPadHandler::isButtonRpt(b);
        return 0;
    }
    INT isButtonTrg(UINT b) const {
        if (isPadEnable()) return CPadHandler::isButtonTrg(b);
        return 0;
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


/* constant tables of the original (initializers in .rodata) */
struct _LayoutInfo {
    SHORT _nLayIdx;
    SHORT _nSeqIdx;
};
struct SEQTBL2 {
    C2DSeqAgent::SeqTbl t[2];
};

/* Access through a reference: keeps the original's order of global stores and later loads. */
template <class T> static inline T& _ref(T& x) { return x; }
static inline BOOL _isFadeDone() { return s_pFadeMask ? s_pFadeMask->isFadeDone() : TRUE; }
static inline BOOL _isAgentState(C2DSeqAgent* p, INT nState) { return p->getState() == nState; }

class CPlayer { /* PARTIAL: Game/Player/Player.h needs the game headers */
public:
    static void exportStatus();
};
class CRsrcDB { /* PARTIAL: System/RsrcDB.h needs RsrcData.h internals */
public:
    static void removeGroup(INT nGroupID, INT nMemberID);
    static class CRsrcData* getFirstRsrc(INT nRsrcType, INT nGroupID, BOOL bFollowLink);
};
class CRcf { /* PARTIAL: System/Rcf.h pulls in System/SJoint.h */
public:
    static void reqLoadRCFFront(INT nGroup, INT nMember, INT nType, BYTE* pBuff, INT nLinkOwner);
    static BOOL isCompleteLoadRCF(INT nGroup, INT nMember);
};
class CSceneInfo { /* PARTIAL: Game/SceneInfo.h needs the game headers */
public:
    static void initNewGame(BOOL bSora);
};
class CGame { /* PARTIAL: Game/Game.h needs the game headers */
public:
    static BOOL isRikuGame();
};
class CPackMan { /* PARTIAL: System/Graphic/PackMan.h is not written yet */
public:
    static void changeDrawMode(INT nMode);
};

HANDLE CSaveLoadUI::m_hThis = 0;

/* The file's inline helpers, whose strings follow the headers' in .rodata: the CTD text with its "" fallback, the
 * memory card slot name (createCheckingMsg, exeCheckSlot) and a placeholder for main's update() loop guard. */
static inline const CHAR* _ctdText(UINT nID) {
    const CHAR* p = CCtdMan::getText(nID);
    return p ? p : "";
}
static inline void _getSlotName(INT nSlot, CHAR* pszOutStr) {
    char szNum[16];
    if (nSlot == 12) {
        strcpy(szNum, "s 1 and 2");
    } else {
        strcpy(szNum, nSlot == 2 ? " 2" : " 1");
    }
    const CHAR* pszText = _ctdText(0x600001);
    sprintf(pszOutStr, "%s%s", pszText, szNum);
}
static inline void _updateOverrun() { TRACE("CSaveLoadUI \x82\xcc update() \x82\xaa\x89\xf1\x82\xe8\x82\xb7\x82\xac\x81" "c\n"); }

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", __11CSaveLoadUIR9CSaveLoad8SAVELOAD9GAMECLEAR);
__asm__(".rdata\n.align 3\n.word 0x74737953\n.word 0x462F6D65\n.word 0x682E6D73\n.word 0x00000000\n.word 0x736E4970\n.word 0x636E6174\n.word 0x00000065\n.word 0x00000000\n.word 0x426E6670\n.word 0x6E696765\n.word 0x74617453\n.word 0x00000065\n.word 0xFFFF0000\n.word 0x00348F38\n.word 0xFFFF0000\n.word 0x00349018\n.word 0xFFFF0000\n.word 0x00349148\n.word 0xFFFF0000\n.word 0x00349150\n.word 0xFFFF0000\n.word 0x00349248\n.word 0xFFFF0000\n.word 0x003493F8\n.word 0xFFFF0000\n.word 0x00349788\n.word 0xFFFF0000\n.word 0x00349790\n.word 0xFFFF0000\n.word 0x00349860\n.word 0xFFFF0000\n.word 0x003498D8\n.word 0xFFFF0000\n.word 0x00349938\n.word 0xFFFF0000\n.word 0x00349940\n.word 0xFFFF0000\n.word 0x00349948\n.word 0xFFFF0000\n.word 0x00349950\n.word 0xFFFF0000\n.word 0x003499D0\n.word 0xFFFF0000\n.word 0x003499D8\n.word 0xFFFF0000\n.word 0x00349A18\n.word 0xFFFF0000\n.word 0x00349B18\n.word 0xFFFF0000\n.word 0x00349B98\n.word 0xFFFF0000\n.word 0x00349C48\n.word 0xFFFF0000\n.word 0x00349FE8\n.word 0xFFFF0000\n.word 0x00349FF0\n.word 0xFFFF0000\n.word 0x0034A1D0\n.word 0xFFFF0000\n.word 0x0034A1D8\n.word 0xFFFF0000\n.word 0x0034A2D8\n.word 0xFFFF0000\n.word 0x0034A6A8\n.word 0xFFFF0000\n.word 0x0034A6E8\n.word 0xFFFF0000\n.word 0x0034A890\n.word 0xFFFF0000\n.word 0x0034AAC0\n.word 0xFFFF0000\n.word 0x0034ACA8\n.word 0xFFFF0000\n.word 0x0034ADE0\n.word 0xFFFF0000\n.word 0x0034AE68\n.word 0xFFFF0000\n.word 0x0034AF80\n.word 0xFFFF0000\n.word 0x0034B228\n.word 0xFFFF0000\n.word 0x0034B338\n.word 0xFFFF0000\n.word 0x0034B4E0\n.word 0xFFFF0000\n.word 0x0034B5A8\n.word 0xFFFF0000\n.word 0x0034B7B8\n.word 0xFFFF0000\n.word 0x0034BB00\n.word 0xFFFF0000\n.word 0x0034BCF0\n.word 0xFFFF0000\n.word 0x0034BDC0\n.word 0xFFFF0000\n.word 0x0034BEA0\n.word 0x65766173\n.word 0x64616C2E\n.word 0x00000000\n.word 0x00000000\n.word 0x64616F6C\n.word 0x64616C2E\n.word 0x00000000\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", _$_11CSaveLoadUI);
void CSaveLoadUI::preInit() {
    if (!(m_mode == SLMODE_BOOT || m_mode == SLMODE_MESSAGE)) CPackMan::changeDrawMode(1);
    INT nID = !getRsrcType() ? 0x6A : 0x6B;
    if (!CRsrcDB::getFirstRsrc(0x11, nID + 20000, FALSE)) CRcf::reqLoadRCFFront(2, nID, 0, NULL, 0);
    if (s_nWorldIconRsrc) {
        INT nMember = s_nWorldIconRsrc;
        if (!CRsrcDB::getFirstRsrc(0x11, nMember + 20000, FALSE)) CRcf::reqLoadRCFFront(2, s_nWorldIconRsrc, 0, NULL, 0);
    }
    if (!CSystemDataChecker::isExited()) CSystemDataChecker::abort();
    m_nTimer = 2;
}
void CSaveLoadUI::exeInit() {
    if (m_nTimer > 0) m_nTimer--;
    INT nID = !getRsrcType() ? 0x6A : 0x6B;
    if (!CRcf::isCompleteLoadRCF(2, nID)) return;
    if (!CSystemDataChecker::isExited()) return;
    if (m_nTimer > 0) return;
    D2::C2DMan::changeLevel(D2::C2DMan::D2MAN_LEVEL1);
    switch (m_mode) {
    case SLMODE_SAVE:
        CGameWork::setByte(1, 0x140, 7);
    case SLMODE_LOAD:
        changeState(2);
        break;
    case SLMODE_BOOT:
        changeState(1);
        break;
    case SLMODE_MESSAGE:
        changeState(18);
        break;
    }
    m_pMemoca = new (com_malloc_d(sizeof(CMemocaAgent), __FILE__ "(708)", RET_ADDR())) CMemocaAgent(this);
    m_pGameDataEntries = new (com_malloc_d(sizeof(CMemocaDataEntries), __FILE__ "(709)", RET_ADDR())) CMemocaDataEntries(99);
}
void CSaveLoadUI::endInit() {}
void CSaveLoadUI::preCheckBoot() {
    TRACE_COL((TtyColor)0x1F, "\x83\x81\x83\x82\x83J\x97" "e\x97\xca\x83`\x83" "F\x83" "b\x83N\x8aJ\x8en\x81" "E\x81" "E\x81" "E\n");
    m_pMessageDlgAgent = createMessageDlg(m_pMessageDlgCtrl, m_pMessageDlgFntInf);
    for (INT i = 0; i < 2; i++) {
        m_pMessageDlgBtnAgent[i] = createMessageDlgBtn(m_pMessageDlgCtrl, i);
    }
    m_pMessageDlgCursor = createCursor();
    m_pMessageDlgCursor->changeLayer(14);
    m_pMainLightMark = createLightMark(LIGHTMARK_TYPE_CURSOR, 16, TRUE);
    for (INT j = 0; j < 2; j++) {
        m_pMessageDlgBtnAgent[j]->m_pCursor = m_pMessageDlgCursor;
        m_pMessageDlgBtnAgent[j]->m_pLightMark = m_pMainLightMark;
    }
    m_nStep = 0;
    m_nTimer = 0;
}
void CSaveLoadUI::exeCheckBoot() {
    char szMsg[256];
    switch (m_nStep) {
    case 0:
        m_nTimer--;
        if (m_pMemoca->reqCheckFreeMem()) m_nStep++;
        break;
    case 1:
        m_nTimer--;
        if (!m_pMemoca->isCompleteReq(NULL)) break;
        m_nLastResult = m_pMemoca->getLastResult();
        if (m_nLastResult >= 0) TRACE_COL((TtyColor)0x1F, "\x83\x81\x83\x82\x83J\x97" "e\x97\xca\x83`\x83" "F\x83" "b\x83N\x82n\x82j\n");
        m_nStep++;
    case 2:
        if (m_nTimer > 0) {
            m_nTimer--;
            break;
        }
        if (m_nLastResult >= 0) {
            closeMessageDlg();
            m_pMainLightMark->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
            m_nStep = 20;
        } else {
            createBootInfoMsg(m_nLastResult, szMsg);
            openMessageDlg(szMsg, 0, 2);
            m_nStep = 10;
        }
        break;
    case 10:
        if (!m_nDlgResult) break;
        if (m_nDlgResult == 1) {
            m_nLastResult = 0;
            m_nStep = 2;
        } else if (m_nDlgResult == 2) {
            createCheckingMsg(12, szMsg);
            openMessageDlg(szMsg, 0, 0);
            m_nStep = 0;
            m_nTimer = 60;
        }
        break;
    case 20:
        if (isPadEnable()) changeState(4);
        break;
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", preLayIn__11CSaveLoadUI);
void CSaveLoadUI::exeLayIn() {}
void CSaveLoadUI::preLayOut() {
    if (m_mode == SLMODE_LOAD && s_bSaveLoadDone) {
        _ref(s_pFadeMask) = new (com_malloc_d(sizeof(CFadeIOMask), __FILE__ "(915)", RET_ADDR()))
            CFadeIOMask(*CTaskMan::getTask(m_hParent)->getParent(), 0x18);
        s_pFadeMask->reqBlackOut(30, 1.0f);
        CSound::fadeoutAllBGM(30);
        m_nStep = 0;
    } else {
        outAllAgent();
        CSound::playSE(4, -1, 64);
    }
}
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
__asm__(".rdata\n.align 4\n.word .L00349C88\n.word .L00349CAC\n.word .L00349CD0\n.word .L00349EA8\n.word .L00349EA8\n.word .L00349EA8\n.word .L00349EA8\n.word .L00349EA8\n.word .L00349EA8\n.word .L00349EA8\n.word .L00349CF0\n.word .L00349E5C\n.word .L00349E80\n.word 0\n.text\n");
void CSaveLoadUI::endCheckSlot() {}
void CSaveLoadUI::preOpenDataList() {
    if (m_pMemocaInfoAgent) m_pMemocaInfoAgent->startOrRestart();
    m_pDataListContainerAgent->startOrRestart();
    m_pDataListScrollbar->setScrollPos(m_nDataListTop, TRUE);
    m_pDataListScrollbar->startOrRestart();
    for (INT i = 0; i < 4; i++) {
        m_pDataItemPlate[i]->startOrRestart();
        m_pDataItemPlate[i]->enableDispCaption(FALSE);
        m_pDataItemNumCircle[i]->enableDisp(FALSE);
        m_pDataItemIcon[i]->enableDisp(FALSE);
        m_pDataItemClearIcon[i]->enableDisp(FALSE);
    }
}
void CSaveLoadUI::exeOpenDataList() {}
void CSaveLoadUI::preSelDataList() {
    SHORT n = m_nDataListSel - m_nDataListTop;
    CHAR* pszMes;
    if (n < 0) {
        m_nDataListTop = m_nDataListSel;
        if (!(0 <= m_nDataListTop)) AssertApp(__FILE__, 1185, "0 <= m_nDataListTop", 0);
    } else if (n >= 4) {
        m_nDataListTop = m_nDataListSel - 3;
        if (!(0 <= m_nDataListTop)) AssertApp(__FILE__, 1190, "0 <= m_nDataListTop", 0);
    }
    m_pMainLightMark->setSleep(FALSE);
    m_pDataItemPlate[(SHORT)(m_nDataListSel - m_nDataListTop)]->select(TRUE);
    if (getRsrcType()) {
        pszMes = "Select file to load";
    } else {
        pszMes = "Select file to save";
    }
    setHelpMessage(pszMes);
    m_nStep = 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeSelDataList__11CSaveLoadUI);
void CSaveLoadUI::endSelDataList() {
    outHelpMessage();
}
void CSaveLoadUI::backtoSelSlot() {
    outDataList(6);
}
void CSaveLoadUI::preSaveData() {
    char szSlot[32];
    _getSlotName(m_nSlot + 1, szSlot);
    char szMsg[256];
    const CHAR* pszFmt = _ctdText(0x600203);
    const CHAR* pszText = _ctdText(0x600000);
    sprintf(szMsg, pszFmt, pszText, szSlot, pszText);
    openMessageDlg(szMsg, 0, 0);
    m_nStep = 0;
}
void CSaveLoadUI::exeSaveData() {
    switch (m_nStep) {
    case 0:
        if (m_pMemoca->reqLoadSysData()) m_nStep++;
        break;
    case 1:
        if (!m_pMemoca->isCompleteReq(NULL)) break;
        m_nStep++;
    case 2:
        if (m_pMemoca->reqSaveGameData(m_nDataListSel, *m_pGameDataEntries, (m_nFlag & 0x40) != 0)) m_nStep++;
        break;
    case 3:
        if (!m_pMemoca->isCompleteReq(NULL)) break;
        m_nLastResult = m_pMemoca->getLastResult();
        if (m_nLastResult < 0) {
            m_nExecError = 2;
            m_nStep = 10;
            break;
        }
        m_nStep++;
    case 4:
        if (m_pMemoca->reqSaveSysData()) m_nStep++;
        break;
    case 5:
        if (!m_pMemoca->isCompleteReq(NULL)) break;
        m_nLastResult = m_pMemoca->getLastResult();
        if (m_nLastResult < 0) m_nExecError = 3;
        m_nStep = 10;
        break;
    case 10:
        closeMessageDlg();
        m_nStep++;
        break;
    case 11:
        if (!isPadEnable()) break;
        if (m_nLastResult >= 0) {
            _ref(s_bSaveLoadDone) = TRUE;
            if (!(m_nFlag & 0x100)) m_pGameDataEntries->useOneSavableUnit(m_nDataListSel);
            for (INT i = 0; i < 4; i++) {
                setupDataPlate(m_nDataListTop + i);
            }
            updateMemocaInfoStr();
            updateMemocaInfoBar();
            m_pGameDataEntries->setLatestEntryIndex(m_nSlot, m_nDataListSel);
            CSound::playSE(11, -1, 64);
            changeState(10);
        } else {
            changeState(17);
        }
        break;
    }
}
void CSaveLoadUI::preLoadData() {
    char szSlot[32];
    _getSlotName(m_nSlot + 1, szSlot);
    char szMsg[256];
    const CHAR* pszFmt = _ctdText(0x600300);
    const CHAR* pszText = _ctdText(0x600000);
    sprintf(szMsg, pszFmt, pszText, szSlot, pszText);
    openMessageDlg(szMsg, 0, 0);
    GameDataHeader* p = m_pGameDataEntries->getEntry(m_nDataListSel);
    if (!p) AssertApp(__FILE__, 1474, "p", 0);
    CGameWork::resetAll();
    CSceneInfo::initNewGame(p->_cFlag & 1);
    m_nStep = 0;
}
void CSaveLoadUI::exeLoadData() {
    switch (m_nStep) {
    case 0:
        if (m_pMemoca->reqLoadSysData()) m_nStep++;
        break;
    case 1:
        if (!m_pMemoca->isCompleteReq(NULL)) break;
        m_nStep++;
    case 2:
        if (m_pMemoca->reqLoadGameData(m_nDataListSel)) m_nStep++;
        break;
    case 3:
        if (!m_pMemoca->isCompleteReq(NULL)) break;
        closeMessageDlg();
        m_nStep++;
    case 4:
        if (!isPadEnable()) break;
        if (m_pMemoca->getLastResult() == 0) {
            s_bSaveLoadDone = TRUE;
            changeState(3);
            CSound::playSE(11, -1, 64);
            m_pGameDataEntries->setLatestEntryIndex(m_nSlot, m_nDataListSel);
        } else {
            m_nExecError = 1;
            changeState(17);
        }
        break;
    }
}
void CSaveLoadUI::preCheckOverwrite() {
    openMessageDlg(_ctdText(0x600200), 1, 3);
    m_nStep = 0;
}
void CSaveLoadUI::exeCheckOverwrite() {
    INT r = -9999;
    switch (m_nStep) {
    case 0:
        if (m_pMemoca->reqIdleCheckCard()) m_nStep++;
        break;
    case 1:
        if (m_pMemoca->isCompleteReq(NULL)) {
            r = m_pMemoca->getLastResult();
            m_nStep = 0;
        }
        break;
    }
    if (r == -2) {
        changeState(15);
        return;
    }
    switch (m_nStep) {
    case 0:
    case 1:
        if (m_nDlgResult) {
            if (m_nDlgResult == 1) {
                m_nFlag |= 0x100;
                changeState(11);
                continueProc();
            } else {
                closeMessageDlg();
                m_nStep = 2;
            }
        }
        break;
    case 2:
        if (isPadEnable()) changeState(10);
        break;
    }
}
void CSaveLoadUI::preCheckNoMemory() {
    char szSlot[32];
    _getSlotName(m_nSlot + 1, szSlot);
    char szLine1[128];
    CCtdMan::getText(0x600201);
    CHAR* pszLine1 = szLine1;
    const CHAR* pszFmt = _ctdText(0x600201);
    sprintf(pszLine1, pszFmt, _ctdText(0x600000), szSlot);
    char szLine2[128];
    CHAR* pszLine2 = szLine2;
    const CHAR* pszFmt2 = _ctdText(0x60020A);
    sprintf(pszLine2, pszFmt2, 100, _ctdText(0x100000));
    char szMsg[256];
    sprintf(szMsg, "%s\n%s", pszLine1, pszLine2);
    openMessageDlg(szMsg, 0, 1);
    m_nStep = 0;
}
void CSaveLoadUI::exeCheckNoMemory() {
    INT r = -9999;
    switch (m_nStep) {
    case 0:
        if (m_pMemoca->reqIdleCheckCard()) m_nStep++;
        break;
    case 1:
        if (m_pMemoca->isCompleteReq(NULL)) {
            r = m_pMemoca->getLastResult();
            m_nStep = 0;
        }
        break;
    }
    if (r == -2) {
        changeState(15);
        return;
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
        if (isPadEnable()) {
            if (m_nFlag & 0x80) {
                changeState(10);
            } else {
                backtoSelSlot();
            }
            m_nStep = 99;
        }
        break;
    }
}
void CSaveLoadUI::preCheckNoCard() {
    char szSlot[32];
    _getSlotName(m_nSlot + 1, szSlot);
    char szMsg[256];
    const CHAR* pszFmt = _ctdText(0x600202);
    sprintf(szMsg, pszFmt, _ctdText(0x600000), szSlot);
    openMessageDlg(szMsg, 0, 1);
    m_nStep = 0;
}
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
void CSaveLoadUI::preCheckUnformat() {
    char szSlot[32];
    _getSlotName(m_nSlot + 1, szSlot);
    char szLine1[128];
    const CHAR* pszFmt = _ctdText(0x600205);
    sprintf(szLine1, pszFmt, _ctdText(0x600000), szSlot);
    char szMsg[256];
    sprintf(szMsg, "%s\n%s", szLine1, _ctdText(0x600206));
    openMessageDlg(szMsg, 1, 3);
    m_nStep = 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeCheckUnformat__11CSaveLoadUI);
__asm__(".rdata\n.align 4\n.word .L0034B86C\n.word .L0034B86C\n.word .L0034B88C\n.word .L0034BAE4\n.word .L0034BAE4\n.word .L0034BAE4\n.word .L0034BAE4\n.word .L0034BAE4\n.word .L0034BAE4\n.word .L0034BAE4\n.word .L0034B8C4\n.word .L0034BA3C\n.word .L0034BA54\n.word .L0034BAE4\n.word .L0034BAE4\n.word .L0034BAE4\n.word .L0034BAE4\n.word .L0034BAE4\n.word .L0034BAE4\n.word .L0034BAE4\n.word .L0034BAA4\n.word .L0034BABC\n.text\n");
void CSaveLoadUI::preConfirmExecErr() {
    INT nCtd;
    switch (m_nExecError) {
    case 2:
        nCtd = 0x600204;
        break;
    case 3:
        nCtd = 0x60020B;
        break;
    case 4:
        nCtd = 0x600208;
        break;
    case 1:
    default:
        nCtd = 0x600301;
        break;
    }
    char szMsg[256];
    CHAR* pszMsg = szMsg;
    char szSlot[32];
    _getSlotName(m_nSlot + 1, szSlot);
    const CHAR* pszFmt = _ctdText(nCtd);
    sprintf(szMsg, pszFmt, _ctdText(0x600000), szSlot);
    openMessageDlg(pszMsg, 0, 1);
    m_nStep = 0;
}
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
void CSaveLoadUI::preMessageOnly() {
    m_pMessageDlgAgent = createMessageDlg(m_pMessageDlgCtrl, m_pMessageDlgFntInf);
    for (INT i = 0; i < 2; i++) {
        m_pMessageDlgBtnAgent[i] = createMessageDlgBtn(m_pMessageDlgCtrl, i);
    }
    m_pMessageDlgCursor = createCursor();
    m_pMessageDlgCursor->changeLayer(14);
    m_pMainLightMark = createLightMark(LIGHTMARK_TYPE_CURSOR, 16, TRUE);
    for (INT i = 0; i < 2; i++) {
        m_pMessageDlgBtnAgent[i]->attachCursor(m_pMessageDlgCursor);
        m_pMessageDlgBtnAgent[i]->attachLightMark(m_pMainLightMark);
    }
    m_nStep = 0;
    m_nTimer = 0;
}
void CSaveLoadUI::exeMessageOnly() {
    switch (m_nStep) {
    case 0: {
        BOOL bSoraClear = CGameWork::getFlag(0, 9);
        INT nCtd = 0;
        INT nIcon;
        INT nType;
        switch (s_nMessage) {
        case 0:
            nCtd = bSoraClear ? 0x600501 : 0x600500;
            nIcon = 0;
            nType = 1;
            break;
        case 1:
            nCtd = bSoraClear ? 0x600503 : 0x600502;
            nIcon = 1;
            nType = 3;
            break;
        }
        if (!nCtd) {
            changeState(4);
            break;
        }
        openMessageDlg(_ctdText(nCtd), nIcon, nType);
        m_nStep++;
        break;
    }
    case 1:
        if (!m_nDlgResult) break;
        {
            SLMSG_RESULT r;
            switch (m_nDlgResult) {
            case 1:
                r = SLMSG_RESULT_OK;
                break;
            case 2:
                r = SLMSG_RESULT_NO;
                break;
            default:
                r = SLMSG_RESULT_CANCEL;
                break;
            }
            s_nMsgResult = r;
        }
        closeMessageDlg();
        m_nStep++;
        break;
    case 2:
        if (isPadEnable()) changeState(4);
        break;
    }
}
C2DSeqAgent* CSaveLoadUI::createMainFrame() {
    C2DSeqAgent::SeqTbl tbl[2] = {{{2, 4, 5}}, {{3, 5, 6}}};
    C2DSeqAgent* pAgent = new (com_malloc_d(sizeof(C2DSeqAgent), __FILE__ "(1990)", RET_ADDR())) C2DSeqAgent(this);
    pAgent->setupSeq(m_pszLad, 11);
    pAgent->setupState(tbl[getRsrcType()], callbackMainFrame, callbackMainFrame);
    return pAgent;
}
C2DSeqAgent* CSaveLoadUI::createWorldIcon(D2::C2DSeqCtrl*& pOutCtrl) {
    static CHAR* s_pszWorldIcon = "world_title.lad";
    if (m_nFlag & 0x40) s_pszWorldIcon = "world_dummy.lad";
    C2DSeqAgent::SeqTbl tbl = {{0, 1, 2}};
    FVECTOR v;
    C2DSeqAgent* pAgent;
    pOutCtrl = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(2006)", RET_ADDR())) D2::C2DSeqCtrl;
    pOutCtrl->addSeq(s_pszWorldIcon, 0, tbl._tbl[0]);
    pOutCtrl->changeLayer(12);
    v.init();
    pOutCtrl->setPos(v);
    pOutCtrl->enableColorCtrl(TRUE);
    pAgent = new (com_malloc_d(sizeof(C2DSeqAgent), __FILE__ "(2014)", RET_ADDR())) C2DSeqAgent(this);
    pAgent->setupSeq(*pOutCtrl, s_pszWorldIcon, 0, 0);
    pAgent->setupState(tbl);
    return pAgent;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createRoomName__11CSaveLoadUIRPQ22D210C2DSeqCtrl);
__asm__(".rdata\n.align 3\n.word 0x002B002A\n.word 0x0000002C\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x38323032\n.word 0x00000029\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x37343032\n.word 0x00000029\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x31353032\n.word 0x00000029\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createCursor__11CSaveLoadUI);
__asm__(".rdata\n.align 3\n.word 0x00070008\n.word 0x00070008\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x38363032\n.word 0x00000029\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createLightMark__11CSaveLoadUIQ211CSaveLoadUI14LIGHTMARK_TYPEii);
__asm__(".rdata\n.align 3\n.word 0x00220021\n.word 0x00210023\n.word 0x00230022\n.word 0x000A0009\n.word 0x0009000B\n.word 0x000B000A\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x36383032\n.word 0x00000029\n.text\n");
C2DSeqAgent* CSaveLoadUI::createTimeLabel(D2::C2DSeqCtrl*& pOutCtrl) {
    C2DSeqAgent::SeqTbl tbl = {{45, 46, 47}};
    FVECTOR v;
    C2DSeqAgent* pAgent;
    pOutCtrl = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(2101)", RET_ADDR())) D2::C2DSeqCtrl;
    pOutCtrl->addSeq(m_pszLad, m_nMainSeqUnit, tbl._tbl[0]);
    pOutCtrl->changeLayer(12);
    v.init();
    if (CGame::isRikuGame()) v.y = -35.0f;
    pOutCtrl->setPos(v);
    pOutCtrl->enableColorCtrl(TRUE);
    pAgent = new (com_malloc_d(sizeof(C2DSeqAgent), __FILE__ "(2110)", RET_ADDR())) C2DSeqAgent(this);
    pAgent->setupSeq(*pOutCtrl, m_pszLad, m_nMainSeqUnit, 0);
    pAgent->setupState(tbl);
    return pAgent;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createTimeCtrl__11CSaveLoadUI);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x32323132\n.word 0x00000029\n.text\n");
C2DSeqAgent* CSaveLoadUI::createMPLabel(D2::C2DSeqCtrl*& pOutCtrl) {
    C2DSeqAgent::SeqTbl tbl = {{39, 40, 41}};
    FVECTOR v;
    C2DSeqAgent* pAgent;
    pOutCtrl = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(2142)", RET_ADDR())) D2::C2DSeqCtrl;
    pOutCtrl->addSeq(m_pszLad, m_nMainSeqUnit, tbl._tbl[0]);
    pOutCtrl->changeLayer(12);
    v.init();
    pOutCtrl->setPos(v);
    pOutCtrl->enableColorCtrl(TRUE);
    pAgent = new (com_malloc_d(sizeof(C2DSeqAgent), __FILE__ "(2150)", RET_ADDR())) C2DSeqAgent(this);
    pAgent->setupSeq(*pOutCtrl, m_pszLad, m_nMainSeqUnit, 0);
    pAgent->setupState(tbl);
    return pAgent;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createMPCtrl__11CSaveLoadUI);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x32363132\n.word 0x00000029\n.text\n");
C2DSeqAgent* CSaveLoadUI::createMainHelp(D2::C2DSeqCtrl*& pOutCtrl, D2::C2DFnt*& pOutFnt) {
    C2DSeqAgent::SeqTbl tbl[2] = {{{89, 90, 91}}, {{90, 91, 92}}};
    _LayoutInfo ltbl[2] = {{3, 9}, {4, 9}};
    pOutCtrl = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(2189)", RET_ADDR())) D2::C2DSeqCtrl;
    C2DSeqAgent::SeqTbl& t = tbl[getRsrcType()];
    pOutCtrl->addSeq(m_pszLad, m_nMainSeqUnit, t._tbl[0]);
    pOutCtrl->changeLayer(12);
    G_RGBA rgba;
    rgba.col = 0x80707070;
    _LayoutInfo& info = ltbl[getRsrcType()];
    m_szMainFrameHelpStr[0] = '\0';
    pOutFnt = new (com_malloc_d(sizeof(D2::C2DFnt), __FILE__ "(2198)", RET_ADDR()))
        D2::C2DFnt(0, 0, &rgba, 16, m_szMainFrameHelpStr, 64);
    pOutCtrl->addFnt(m_pszLad, m_nMainSeqUnit, pOutFnt);
    pOutCtrl->enableColorCtrl(TRUE);
    FVECTOR vPos;
    D2::C2DMan::getLayPosition(m_pszLad, info._nLayIdx, info._nSeqIdx, vPos, FALSE);
    pOutCtrl->setPos(vPos);
    C2DSeqAgent* pAgent = new (com_malloc_d(sizeof(C2DSeqAgent), __FILE__ "(2206)", RET_ADDR())) C2DSeqAgent(this);
    pAgent->setupSeq(*pOutCtrl, m_pszLad, m_nMainSeqUnit, 0);
    pAgent->setupState(t);
    return pAgent;
}
static CHAR* s_pStrSaveLoad[2] = {"Save", "Load"};
static CHAR* s_pStrSlot[2] = {"Slot 1", "Slot 2"};
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createResultPlate__11CSaveLoadUIRPQ22D210C2DSeqCtrlRPQ22D26C2DFnti);
__asm__(".rdata\n.align 3\n.word 0x00250024\n.word 0x00240026\n.word 0x00260025\n.word 0x00050004\n.word 0x00040006\n.word 0x00060005\n.word 0x00040003\n.word 0x00040004\n.word 0x00050003\n.word 0x00050004\n.word 0x40800000\n.word 0x41800000\n.word 0xC2300000\n.word 0x40E00000\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x34333232\n.word 0x00000029\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x34343232\n.word 0x00000029\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x37343232\n.word 0x00000029\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x38353232\n.word 0x00000029\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createSelSlotPlate__11CSaveLoadUIi);
__asm__(".rdata\n.align 3\n.word 0x00020001\n.word 0x00010003\n.word 0x00030002\n.word 0x00000000\n.word 0x00060003\n.word 0x00060004\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x39373232\n.word 0x00000029\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createMemocaInfo__11CSaveLoadUIRPQ22D210C2DSeqCtrlPPQ22D210C2DSeqCtrl);
__asm__(".rdata\n.align 3\n.word 0x006C006B\n.word 0x0000006D\n.word 0x00080003\n.word 0x00000000\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x31313332\n.word 0x00000029\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x39313332\n.word 0x00000029\n.word 0x00390037\n.word 0x00000038\n.word 0x00010006\n.word 0x00000000\n.word 0x80208000\n.word 0x80204080\n.word 0x80804020\n.word 0x00000000\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x31333332\n.word 0x00000029\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x34343332\n.word 0x00000029\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createDataListContainer__11CSaveLoadUIRPQ22D210C2DSeqCtrlRP19C2DSeqScrollbarCtrl);
__asm__(".rdata\n.align 3\n.word 0x00610060\n.word 0x005D0062\n.word 0x005F005E\n.word 0x00000000\n.word 0x00030003\n.word 0x00030004\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x34363332\n.word 0x00000029\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x33373332\n.word 0x00000029\n.word 0x00540053\n.word 0x00500055\n.word 0x00520051\n.word 0x00570056\n.word 0x00540058\n.word 0x00560055\n.word 0x00520051\n.word 0x00570053\n.word 0x00590058\n.word 0x00000000\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x36383332\n.word 0x00000029\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createDataItemPlate__11CSaveLoadUIRQ22D210C2DSeqCtrlRPQ22D210C2DSeqCtrlRPQ22D26C2DFntT2T2i);
__asm__(".rdata\n.align 3\n.word 0x00010007\n.word 0x00010008\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x38323432\n.word 0x00000029\n.word 0x004F004B\n.word 0x00000000\n.word 0x00010001\n.word 0x00010001\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x31353432\n.word 0x00000029\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x32363432\n.word 0x00000029\n.word 0x00020001\n.word 0x00020001\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x39363432\n.word 0x00000029\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x31383432\n.word 0x00000029\n.word 0x00730076\n.word 0x00000000\n.word 0x00030001\n.word 0x00030001\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x32393432\n.word 0x00000029\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createMessageDlg__11CSaveLoadUIRPQ22D210C2DSeqCtrlRPQ22D26C2DFnt);
__asm__(".rdata\n.align 3\n.word 0x00350034\n.word 0x00340036\n.word 0x00360035\n.word 0x00380037\n.word 0x00370039\n.word 0x00390038\n.word 0x00000000\n.word 0x00000000\n.word 0x00000002\n.word 0x00000002\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x32323532\n.word 0x00000029\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x38323532\n.word 0x00000029\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x33333532\n.word 0x00000029\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x32343532\n.word 0x00000029\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createMessageDlgBtn__11CSaveLoadUIPQ22D210C2DSeqCtrli);
__asm__(".rdata\n.align 3\n.word 0x00320031\n.word 0x00310033\n.word 0x00330032\n.word 0x00000000\n.word 0x00300030\n.word 0x00000000\n.word 0x00010000\n.word 0x00020000\n.word 0x00010000\n.word 0x00020000\n.word 0x00020002\n.word 0x00010002\n.word 0x00020002\n.word 0x00010002\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x726F462F\n.word 0x654D2F6D\n.word 0x532F756E\n.word 0x4C657661\n.word 0x2E64616F\n.word 0x28707063\n.word 0x37363532\n.word 0x00000029\n.text\n");
void CSaveLoadUI::setActiveSlotPlate(INT nSlot) {
    unsigned short seqTbl[2][2] = {{0, 2}, {0, 2}};
    char szMes[64];
    CHAR* pszFrm;
    for (INT i = 0; i < 2; i++) {
        m_pSelSlotPlate[i]->select(i == nSlot);
    }
    if (m_mode == SLMODE_SAVE) {
        pszFrm = "Save data to %s";
    } else {
        pszFrm = "Load data from %s";
    }
    sprintf(szMes, pszFrm, s_pStrSlot[nSlot]);
    setHelpMessage(szMes);
    m_pResultPlateFnt[1]->setText(s_pStrSlot[nSlot]);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", setupDataPlate__11CSaveLoadUIs);
__asm__(".rdata\n.align 3\n.word 0x76615343\n.word 0x616F4C65\n.word 0x3A495564\n.word 0x7465733A\n.word 0x61447075\n.word 0x6C506174\n.word 0x28657461\n.word 0x20296425\n.word 0x202E2E2E\n.word 0xCD88CD94\n.word 0xC5824F8A\n.word 0x000AB782\n.word 0x004C004B\n.word 0x0050004F\n.word 0x64323025\n.word 0x00000000\n.word 0x20207325\n.word 0x45F9564C\n.word 0x00643225\n.word 0x00000000\n.word 0x20207325\n.word 0x45F9564C\n.word 0x20643225\n.word 0x20202020\n.word 0x64352520\n.word 0x00504D20\n.word 0x72746E45\n.word 0x65636E61\n.word 0x6C614820\n.word 0x0000006C\n.word 0x00466425\n.word 0x00000000\n.word 0x00000020\n.word 0x00000000\n.word 0x25207325\n.word 0x00000073\n.word 0x3A643325\n.word 0x64323025\n.word 0x00000000\n.word 0x00000000\n.word 0x250A7325\n.word 0x73250A73\n.word 0x00000000\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", updateMemocaInfoBar__11CSaveLoadUI);
__asm__(".rdata\n.align 3\n.word 0x6C6C416E\n.word 0x00000000\n.text\n");
void CSaveLoadUI::updateMemocaInfoStr() {
    CHAR* pszStrTbl[3] = {"Available\n\x81@\x81@\x81@\x81@%3d", "Saved\n\x81@\x81@\x81@\x81@%3d", "Other data"};
    USHORT nSavable = m_pGameDataEntries->getDataUnitCountByType(0);
    USHORT nSaved = m_pGameDataEntries->getDataUnitCountByType(1);
    if (nSavable + nSaved >= 100) nSavable = 99 - nSaved;
    for (INT i = 0; i < 3; i++) {
        m_pMemocaInfoBar[i]->enableDisp(TRUE);
        switch (i) {
        case 0:
        case 1:
            sprintf(m_szMemocaInfoStr[i], pszStrTbl[i], !i ? nSavable : nSaved);
            break;
        case 2:
            strcpy(m_szMemocaInfoStr[2], pszStrTbl[2]);
            break;
        }
    }
}
void CSaveLoadUI::setHelpMessage(const CHAR* pszMes) {
    strcpy(m_szMainFrameHelpStr, pszMes);
    if (_isAgentState(m_pMainFrameHelpAgent, C2DSeqAgent::_STATE_NOT_START)) m_pMainFrameHelpAgent->start();
    else if (m_pMainFrameHelpAgent->getState() != C2DSeqAgent::STATE_LOOP) m_pMainFrameHelpAgent->changeState(C2DSeqAgent::STATE_IN, TRUE, -1);
}
void CSaveLoadUI::outHelpMessage() {
    m_pMainFrameHelpAgent->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", openMessageDlg__11CSaveLoadUIPCcii);
__asm__(".rdata\n.align 3\n.word 0x00000042\n.word 0x00000043\n.word 0x00000046\n.word 0x00000047\n.word 0x4F464E49\n.word 0x54414D52\n.word 0x004E4F49\n.word 0x00000000\n.word 0x53455551\n.word 0x4E4F4954\n.word 0x00000000\n.word 0x00000000\n.word D_004F3E18\n.word D_004F3E28\n.word 0xC3160000\n.word 0xC2600000\n.word 0xC1700000\n.word 0xC1F00000\n.word 0xC1F00000\n.word 0xC1F00000\n.word 0x00004B4F\n.word 0x00000000\n.word 0x00736559\n.word 0x00000000\n.word 0x00006F4E\n.word 0x00000000\n.text\n");
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
/* Parked: the case-1 OK tail (m_nDlgResult = 1; CSound::playSE(2, -1, 64)) is cross-jumped into the identical tail of
 * case 2/3 in ours, the original keeps both copies; also the lockPadControl(TRUE) tail loads into v1 (original v0) and
 * one nop in the select loop; tried: switch vs if/else chain, combined early return, store after the call,
 * SLMSG_RESULT_OK / m_nDlgType as the value, permuter 4 min (6886 candidates, no gain) */
#if 0
BOOL CSaveLoadUI::exeMessageDlg() {
    if (!m_pMessageDlgAgent || m_pMessageDlgAgent->getState() != C2DSeqAgent::STATE_LOOP || m_nDlgResult) return FALSE;
    switch (m_nDlgType) {
    case 1:
        lockPadControl(FALSE);
        if (isTrgOK()) {
            m_nDlgResult = 1;
            CSound::playSE(2, -1, 64);
        } else if (isTrgCANCEL()) {
            m_nDlgResult = 3;
            CSound::playSE(4, -1, 64);
        }
        break;
    case 2:
    case 3:
        lockPadControl(FALSE);
        if (checkSelectorH(m_nDlgSel, 2, TRUE, TRUE)) {
            for (INT i = 0; i < 2; i++) {
                m_pMessageDlgBtnAgent[i]->select(i == m_nDlgSel);
            }
        } else if (isTrgOK()) {
            m_nDlgResult = m_nDlgSel + 1;
            CSound::playSE(2, -1, 64);
        } else if (m_nDlgType == 3 && isTrgCANCEL()) {
            m_nDlgResult = 3;
            CSound::playSE(4, -1, 64);
        }
        break;
    default:
        return FALSE;
    }
    lockPadControl(TRUE);
    return FALSE;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeMessageDlg__11CSaveLoadUI);
#endif
void CSaveLoadUI::outAllAgent() {
    m_pMainFrameAgent->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    if (m_pRoomNameAgent) m_pRoomNameAgent->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    if (m_pWorldIconAgent) m_pWorldIconAgent->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    m_pMainFrameHelpCtrl->enableDisp(FALSE);
    if (m_pMainCursor) m_pMainCursor->enableDisp(FALSE);
    if (m_pMainLightMark && m_pMainLightMark->isSeqCtrlVisible()) {
        m_pMainLightMark->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    }
    if (m_pRoomNameCtrl) m_pRoomNameCtrl->enableDisp(FALSE);
    if (m_pWorldIconCtrl) m_pWorldIconCtrl->enableDisp(FALSE);
    if (m_pTimeLabelAgent) m_pTimeLabelAgent->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    if (m_pMPLabelAgent) m_pMPLabelAgent->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    if (m_pTimeCtrl) m_pTimeCtrl->hide();
    if (m_pMPCtrl) m_pMPCtrl->hide();
    for (INT i = 0; i < 2; i++) {
        if (m_pResultPlateAgent[i]->getState() < C2DSeqAgent::STATE_OUT) {
            m_pResultPlateAgent[i]->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
            m_pResultLightAgent[i]->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
        }
    }
    for (INT i = 0; i < 2; i++) {
        m_pSelSlotPlate[i]->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    }
    if (m_nFlag & 4) closeMessageDlg();
    outDataList(-1);
    CSound::setBGMMasterVolume(-1, -1);
}
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
void CSaveLoadUI::callbackResultPlate(C2DSeqAgent& agent) {
    CSaveLoadUI* pThis = (CSaveLoadUI*)CTaskMan::getTask(m_hThis);
    if (!pThis) return;
    INT idx = -1;
    for (INT i = 0; i < 2; i++) {
        if (&agent == pThis->m_pResultPlateAgent[i]) {
            idx = i;
            break;
        }
    }
    if (idx < 0) return;
    if (!agent.getSeqCtrl()) return;
    float ofsTbl[2][2] = {{136.0f, 48.0f}, {56.0f, -4.0f}};
    FLOAT* pTbl = ofsTbl[idx];
    FVECTOR vPos = agent.getSeqCtrl()->m_seqControl.vPos;
    vPos.x += pTbl[0];
    vPos.y += pTbl[1];
    C2DSeqAgent* pLight = pThis->m_pResultLightAgent[idx];
    if (!pLight) return;
    if (_isAgentState(pLight, C2DSeqAgent::_STATE_NOT_START)) {
        pLight->getSeqCtrl()->setLocalPos(vPos);
        pLight->start();
    } else {
        pLight->changeState(C2DSeqAgent::STATE_IN, TRUE, -1);
    }
}
extern "C" char D_004F3D90[]; /* "%3d:%02d": first used by setupDataPlate (still assembly) */
void CSaveLoadUI::callbackDataList(C2DSeqAgent& agent) {
    CSaveLoadUI* pThis = (CSaveLoadUI*)CTaskMan::getTask(m_hThis);
    if (!pThis) return;
    switch (agent.getState()) {
    case C2DSeqAgent::STATE_IN:
        pThis->changeState(10);
        for (INT i = 0; i < 4; i++) {
            pThis->m_pDataItemPlate[i]->enableDispCaption(TRUE);
            pThis->m_pDataItemNumCircle[i]->enableDisp(TRUE);
            GameDataHeader* pInfo = pThis->m_pGameDataEntries->getEntry(pThis->m_nDataListTop + i);
            BOOL bDispIcon;
            if (pInfo && pInfo->hasData()) {
                sprintf(pThis->m_szDataItemTime[i], D_004F3D90, pInfo->_nTime / 3600, pInfo->_nTime % 3600 / 60);
                bDispIcon = (pInfo->_cWorld & 0xF) < 14;
            } else {
                *pThis->m_szDataItemTime[i] = '\0';
                bDispIcon = FALSE;
            }
            pThis->m_pDataItemIcon[i]->enableDisp(bDispIcon);
            pThis->m_pDataItemClearIcon[i]->enableDisp(pInfo && pInfo->isGameCleared());
        }
        break;
    case C2DSeqAgent::STATE_OUT:
        for (INT i = 0; i < 4; i++) {
            pThis->m_pDataItemPlate[i]->enableDisp(FALSE);
            pThis->m_pDataItemNumCircle[i]->enableDisp(FALSE);
            pThis->m_pDataItemIcon[i]->enableDisp(FALSE);
            pThis->m_pDataItemClearIcon[i]->enableDisp(FALSE);
        }
        pThis->changeNextState();
        break;
    }
}
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
void CSaveLoadUI::callbackMessageDlg(C2DSeqAgent& agent) {
    CSaveLoadUI* pThis = (CSaveLoadUI*)CTaskMan::getTask(m_hThis);
    if (!pThis) return;
    switch (agent.getState()) {
    case C2DSeqAgent::STATE_IN:
        if (pThis->m_nDlgType) {
            C2DSeqSelectable* pBtn = pThis->m_pMessageDlgBtnAgent[pThis->m_nDlgSel];
            if (pBtn) pBtn->select(TRUE);
        }
        break;
    case C2DSeqAgent::STATE_OUT:
        pThis->lockPadControl(FALSE);
        if (pThis->m_pMainCursor) pThis->m_pMainCursor->setNormalSeq();
        break;
    }
}
/* Parked: registers only (22 lines): the original keeps this in s1, nOutIdx in s2 and nValDownKey in s3, ours
 * nValDownKey/this/nOutIdx in s1/s2/s3 (global-alloc priority); code and layout match once isButtonRpt/Trg are the
 * if-return-call inlines; tried: ternary/if forms of the pad inlines, permuter 4 min (5617 candidates, no gain) */
#if 0
BOOL CSaveLoadUI::checkSelector(SHORT& nOutIdx, SHORT nMax, INT nValDownKey, INT nValUpKey, SHORT nUpDownValue,
                                BOOL bSetSE, BOOL bLoopChk) {
    SHORT nNewIdx = nOutIdx;
    if (isButtonRpt(nValDownKey)) {
        nNewIdx -= nUpDownValue;
        if (nNewIdx < 0) {
            if (nOutIdx != 0) {
                nNewIdx = 0;
            } else if (!bLoopChk || isButtonTrg(nValDownKey)) {
                nNewIdx = nMax - 1;
            } else {
                nNewIdx = 0;
            }
        }
    } else if (isButtonRpt(nValUpKey)) {
        nNewIdx += nUpDownValue;
        if (nNewIdx > nMax - 1) {
            if (nOutIdx != nMax - 1) {
                nNewIdx = nMax - 1;
            } else if (!bLoopChk || isButtonTrg(nValUpKey)) {
                nNewIdx = 0;
            } else {
                nNewIdx = nMax - 1;
            }
        }
    }
    BOOL bUpdate = nNewIdx != nOutIdx;
    if (bUpdate) {
        nOutIdx = nNewIdx;
        if (bSetSE) CSound::playSE(1, -1, 64);
    }
    return bUpdate;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", checkSelector__11CSaveLoadUIRssiisii);
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createBootInfoMsg__11CSaveLoadUIiPc);
void CSaveLoadUI::createCheckingMsg(INT nSlot, CHAR* pszOutMsg) {
    char szSlot[32];
    pszOutMsg[0] = '\0';
    _getSlotName(nSlot, szSlot);
    const CHAR* pszText = _ctdText(0x600000);
    sprintf(pszOutMsg, _ctdText(0x600105), pszText, szSlot, pszText);
}
HANDLE CSaveLoad::m_hThis = 0;

CSaveLoad::CSaveLoad(CTask* pParent, SAVELOAD mode, GAMECLEAR clear) : CTask(pParent, 0x8000) {
    m_nFlag = 0;
    construct(mode);
    new (com_malloc_d(sizeof(CSaveLoadUI), __FILE__ "(3485)", RET_ADDR())) CSaveLoadUI(*this, mode, clear);
}
CSaveLoad::CSaveLoad(CTask* pParent, SLMSG msg) : CTask(pParent, 0x8000) {
    m_nFlag = 0;
    construct(SLMODE_MESSAGE);
    s_nMessage = msg;
    new (com_malloc_d(sizeof(CSaveLoadUI), __FILE__ "(3493)", RET_ADDR())) CSaveLoadUI(*this, SLMODE_MESSAGE, GAMECLEAR_NONE);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", _$_9CSaveLoad);
void CSaveLoad::construct(SAVELOAD mode) {
    if (m_hThis) {
        TRACE("\x82\xb7\x82\xc5\x82\xc9 SAVE-LOAD \x83\x81\x83j\x83\x85\x81[\x82\xcd\x8d\xec\x90\xac\x8d\xcf\x82\xdd\x82\xc5\x82\xb7\n");
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
static HANDLE s_hSystemDataChecker = 0;

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
        m_pMemoca = new (com_malloc_d(sizeof(CMemocaAgent), __FILE__ "(3599)", RET_ADDR())) CMemocaAgent(this);
    }
    virtual ~CSystemDataCheckerLocal() {}
    virtual void main();
    void abort() { m_nFlag |= 1; }
};

void CSystemDataChecker::start() {
    if (s_hSystemDataChecker) {
        TRACE("\x82\xb7\x82\xc5\x82\xc9 CSystemDataChecker \x82\xcd\x8bN\x93\xae\x8d\xcf\x82\xdd\x82\xc5\x82\xb7\n");
        return;
    }
    new (com_malloc_d(sizeof(CSystemDataCheckerLocal), __FILE__ "(3687)", RET_ADDR())) CSystemDataCheckerLocal(CTaskMan::getSysTask(0));
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
/* The vtables (their key functions are still assembly), the CStateTemplate assert string and the constant tables only assembly uses, with their strings. */
__asm__(".rdata\n.align 3\n.globl _vt$23CSystemDataCheckerLocal\n_vt$23CSystemDataCheckerLocal:\n.word 0\n.word 0\n.word 0\n.word _$_23CSystemDataCheckerLocal\n.word 0\n.word main__23CSystemDataCheckerLocal\n.word 0\n.word disp__5CTask\n.word 0\n.word after__5CTask\n.word 0\n.word onOut__5CTask\n.word 0\n.word getTypeID__C5CTask\n.word 0\n.word 0\n.globl _vt$11CSaveLoadUI$4CFSM\n_vt$11CSaveLoadUI$4CFSM:\n.word 0x0000FFC0\n.word 0\n.word 0x0000FFC0\n.word _$_11CSaveLoadUI\n.word 0\n.word update__4CFSM\n.word 0\n.word beginStateInitial__4CFSM\n.word 0\n.word stateInitial__4CFSM\n.word 0\n.word endStateInitial__4CFSM\n.word 0\n.word endStateProc__4CFSM\n.word 0\n.word 0\n.globl _vt$11CSaveLoadUI\n_vt$11CSaveLoadUI:\n.word 0\n.word 0\n.word 0\n.word _$_11CSaveLoadUI\n.word 0\n.word main__11CSaveLoadUI\n.word 0\n.word disp__5CTask\n.word 0\n.word after__5CTask\n.word 0\n.word onOut__5CTask\n.word 0\n.word getTypeID__C5CTask\n.word 0\n.word 0\n.word 0x49705F6D\n.word 0x6174736E\n.word 0x0065636E\n.word 0x00000000\n.globl _vt$t14CStateTemplate1Z11CSaveLoadUI\n_vt$t14CStateTemplate1Z11CSaveLoadUI:\n.word 0\n.word 0\n.word 0\n.word _$_t14CStateTemplate1Z11CSaveLoadUI\n.word 0\n.word executeBeginState__t14CStateTemplate1Z11CSaveLoadUI\n.word 0\n.word executeState__t14CStateTemplate1Z11CSaveLoadUI\n.word 0\n.word executeEndState__t14CStateTemplate1Z11CSaveLoadUI\n.word 0\n.word 0\n.globl _vt$17C2DSeqNumericCtrl\n_vt$17C2DSeqNumericCtrl:\n.word 0\n.word 0\n.word 0\n.word _$_17C2DSeqNumericCtrl\n.word 0\n.word addSeq__Q22D210C2DSeqCtrlPCcUiUi\n.word 0\n.word addSeq__Q22D210C2DSeqCtrlPCcUi\n.word 0\n.word addFnt__Q22D210C2DSeqCtrlPCcUiPQ22D26C2DFnt\n.word 0\n.word addFnt__Q22D210C2DSeqCtrlPCcPQ22D26C2DFnt\n.word 0\n.word replaceSeq__Q22D210C2DSeqCtrlUiPCcUiUi\n.word 0\n.word replaceSeq__Q22D210C2DSeqCtrlUiPCcUi\n.word 0\n.word replaceFnt__Q22D210C2DSeqCtrlUiPCcUiPQ22D26C2DFnt\n.word 0\n.word replaceFnt__Q22D210C2DSeqCtrlUiPCcPQ22D26C2DFnt\n.word 0\n.word addSeqShift__Q22D210C2DSeqCtrlPCcUiUiff\n.word 0\n.word addSeqShift__Q22D210C2DSeqCtrlPCcUiff\n.word 0\n.word addFntShift__Q22D210C2DSeqCtrlPCcUiPQ22D26C2DFntff\n.word 0\n.word addFntShift__Q22D210C2DSeqCtrlPCcPQ22D26C2DFntff\n.word 0\n.word replaceSeqShift__Q22D210C2DSeqCtrlUiPCcUiUiff\n.word 0\n.word replaceSeqShift__Q22D210C2DSeqCtrlUiPCcUiff\n.word 0\n.word replaceFntShift__Q22D210C2DSeqCtrlUiPCcUiPQ22D26C2DFntff\n.word 0\n.word replaceFntShift__Q22D210C2DSeqCtrlUiPCcPQ22D26C2DFntff\n.word 0\n.word shiftSeqPos__Q22D210C2DSeqCtrlUiff\n.word 0\n.word setFntPos__Q22D210C2DSeqCtrlUiff\n.word 0\n.word 0\n.globl _vt$14C2DSeqTimeCtrl\n_vt$14C2DSeqTimeCtrl:\n.word 0\n.word 0\n.word 0\n.word _$_14C2DSeqTimeCtrl\n.word 0\n.word addSeq__Q22D210C2DSeqCtrlPCcUiUi\n.word 0\n.word addSeq__Q22D210C2DSeqCtrlPCcUi\n.word 0\n.word addFnt__Q22D210C2DSeqCtrlPCcUiPQ22D26C2DFnt\n.word 0\n.word addFnt__Q22D210C2DSeqCtrlPCcPQ22D26C2DFnt\n.word 0\n.word replaceSeq__Q22D210C2DSeqCtrlUiPCcUiUi\n.word 0\n.word replaceSeq__Q22D210C2DSeqCtrlUiPCcUi\n.word 0\n.word replaceFnt__Q22D210C2DSeqCtrlUiPCcUiPQ22D26C2DFnt\n.word 0\n.word replaceFnt__Q22D210C2DSeqCtrlUiPCcPQ22D26C2DFnt\n.word 0\n.word addSeqShift__Q22D210C2DSeqCtrlPCcUiUiff\n.word 0\n.word addSeqShift__Q22D210C2DSeqCtrlPCcUiff\n.word 0\n.word addFntShift__Q22D210C2DSeqCtrlPCcUiPQ22D26C2DFntff\n.word 0\n.word addFntShift__Q22D210C2DSeqCtrlPCcPQ22D26C2DFntff\n.word 0\n.word replaceSeqShift__Q22D210C2DSeqCtrlUiPCcUiUiff\n.word 0\n.word replaceSeqShift__Q22D210C2DSeqCtrlUiPCcUiff\n.word 0\n.word replaceFntShift__Q22D210C2DSeqCtrlUiPCcUiPQ22D26C2DFntff\n.word 0\n.word replaceFntShift__Q22D210C2DSeqCtrlUiPCcPQ22D26C2DFntff\n.word 0\n.word shiftSeqPos__Q22D210C2DSeqCtrlUiff\n.word 0\n.word setFntPos__Q22D210C2DSeqCtrlUiff\n.word 0\n.word 0\n.globl _vt$16C2DSeqSelectable\n_vt$16C2DSeqSelectable:\n.word 0\n.word 0\n.word 0\n.word _$_16C2DSeqSelectable\n.word 0\n.word main__11C2DSeqAgent\n.word 0\n.word disp__5CTask\n.word 0\n.word after__5CTask\n.word 0\n.word onOut__5CTask\n.word 0\n.word getTypeID__C5CTask\n.word 0\n.word dispose__16C2DSeqSelectable\n.word 0\n.word procLoop__11C2DSeqAgent\n.word 0\n.word onOutSeq__11C2DSeqAgent\n.word 0\n.word 0\n.globl _vt$20C2DSeqLightMarkAgent\n_vt$20C2DSeqLightMarkAgent:\n.word 0\n.word 0\n.word 0\n.word _$_20C2DSeqLightMarkAgent\n.word 0\n.word main__11C2DSeqAgent\n.word 0\n.word disp__5CTask\n.word 0\n.word after__5CTask\n.word 0\n.word onOut__5CTask\n.word 0\n.word getTypeID__C5CTask\n.word 0\n.word dispose__11C2DSeqAgent\n.word 0\n.word procLoop__20C2DSeqLightMarkAgent\n.word 0\n.word onOutSeq__20C2DSeqLightMarkAgent\n.word 0\n.word 0\n.globl _vt$16C2DSeqCursorCtrl\n_vt$16C2DSeqCursorCtrl:\n.word 0\n.word 0\n.word 0\n.word _$_16C2DSeqCursorCtrl\n.word 0\n.word addSeq__Q22D210C2DSeqCtrlPCcUiUi\n.word 0\n.word addSeq__Q22D210C2DSeqCtrlPCcUi\n.word 0\n.word addFnt__Q22D210C2DSeqCtrlPCcUiPQ22D26C2DFnt\n.word 0\n.word addFnt__Q22D210C2DSeqCtrlPCcPQ22D26C2DFnt\n.word 0\n.word replaceSeq__Q22D210C2DSeqCtrlUiPCcUiUi\n.word 0\n.word replaceSeq__Q22D210C2DSeqCtrlUiPCcUi\n.word 0\n.word replaceFnt__Q22D210C2DSeqCtrlUiPCcUiPQ22D26C2DFnt\n.word 0\n.word replaceFnt__Q22D210C2DSeqCtrlUiPCcPQ22D26C2DFnt\n.word 0\n.word addSeqShift__Q22D210C2DSeqCtrlPCcUiUiff\n.word 0\n.word addSeqShift__Q22D210C2DSeqCtrlPCcUiff\n.word 0\n.word addFntShift__Q22D210C2DSeqCtrlPCcUiPQ22D26C2DFntff\n.word 0\n.word addFntShift__Q22D210C2DSeqCtrlPCcPQ22D26C2DFntff\n.word 0\n.word replaceSeqShift__Q22D210C2DSeqCtrlUiPCcUiUiff\n.word 0\n.word replaceSeqShift__Q22D210C2DSeqCtrlUiPCcUiff\n.word 0\n.word replaceFntShift__Q22D210C2DSeqCtrlUiPCcUiPQ22D26C2DFntff\n.word 0\n.word replaceFntShift__Q22D210C2DSeqCtrlUiPCcPQ22D26C2DFntff\n.word 0\n.word shiftSeqPos__Q22D210C2DSeqCtrlUiff\n.word 0\n.word setFntPos__Q22D210C2DSeqCtrlUiff\n.word 0\n.word 0\n.word 0\n.word 0\n.word 0\n.word _$_6CState\n.word 0\n.word __pure_virtual\n.word 0\n.word __pure_virtual\n.word 0\n.word __pure_virtual\n.word 0\n.word 0\n.globl _vt$9CSaveLoad\n_vt$9CSaveLoad:\n.word 0\n.word 0\n.word 0\n.word _$_9CSaveLoad\n.word 0\n.word main__5CTask\n.word 0\n.word disp__5CTask\n.word 0\n.word after__5CTask\n.word 0\n.word onOut__5CTask\n.word 0\n.word getTypeID__C5CTask\n.word 0\n.word 0\n.word 0x676E694B\n.word 0x206D6F64\n.word 0x0079654B\n.word 0x00000000\n.word 0x65726854\n.word 0x69572065\n.word 0x73656873\n.word 0x00000000\n.word 0x62617243\n.word 0x77616C63\n.word 0x00000000\n.word 0x00000000\n.word 0x706D7550\n.word 0x686E696B\n.word 0x00646165\n.word 0x00000000\n.word 0x72696146\n.word 0x61482079\n.word 0x00007072\n.word 0x00000000\n.word 0x68736957\n.word 0x20676E69\n.word 0x72617453\n.word 0x00000000\n.word 0x6C657053\n.word 0x6E69626C\n.word 0x00726564\n.word 0x00000000\n.word 0x6174654D\n.word 0x6843206C\n.word 0x626F636F\n.word 0x0000006F\n.word 0x6D796C4F\n.word 0x00616970\n.word 0x6E6F694C\n.word 0x72616568\n.word 0x00000074\n.word 0x00000000\n.word 0x7964614C\n.word 0x63754C20\n.word 0x0000006B\n.word 0x00000000\n.word 0x69766944\n.word 0x5220656E\n.word 0x0065736F\n.word 0x00000000\n.word 0x6874614F\n.word 0x7065656B\n.word 0x00007265\n.word 0x00000000\n.word 0x696C624F\n.word 0x6E6F6976\n.word 0x00000000\n.word 0x00000000\n.word 0x69746C55\n.word 0x5720616D\n.word 0x6F706165\n.word 0x0000006E\n.word 0x6D616944\n.word 0x20646E6F\n.word 0x74737544\n.word 0x00000000\n.word 0x2D656E4F\n.word 0x676E6957\n.word 0x41206465\n.word 0x6C65676E\n.word 0x00000000\n.word 0x00000000\n.word 0x6C756F53\n.word 0x74614520\n.word 0x00007265\n.word 0x00000000\n.word 0x72617453\n.word 0x65655320\n.word 0x0072656B\n.word 0x00000000\n.word 0x6F6E6F4D\n.word 0x6F726863\n.word 0x0000656D\n.word 0x00000000\n.word 0x6C6C6F46\n.word 0x7420776F\n.word 0x57206568\n.word 0x00646E69\n.word 0x64646948\n.word 0x44206E65\n.word 0x6F676172\n.word 0x0000006E\n.word 0x746F6850\n.word 0x44206E6F\n.word 0x67756265\n.word 0x00726567\n.word 0x646E6F42\n.word 0x20666F20\n.word 0x6D616C46\n.word 0x00000065\n.word 0x0000002D\n.word 0x00000000\n.word 0x65726946\n.word 0x00000000\n.word 0x7A696C42\n.word 0x6472617A\n.word 0x00000000\n.word 0x00000000\n.word 0x6E756854\n.word 0x00726564\n.word 0x65727543\n.word 0x00000000\n.word 0x76617247\n.word 0x00797469\n.word 0x706F7453\n.word 0x00000000\n.word 0x6F726541\n.word 0x00000000\n.word 0x626D6953\n.word 0x00000061\n.word 0x696E6547\n.word 0x00000065\n.word 0x626D6142\n.word 0x00000069\n.word 0x626D7544\n.word 0x0000006F\n.word 0x6B6E6954\n.word 0x42207265\n.word 0x006C6C65\n.word 0x00000000\n.word 0x6873754D\n.word 0x00000075\n.word 0x756F6C43\n.word 0x00000064\n.word 0x69746F50\n.word 0x00006E6F\n.word 0x502D6948\n.word 0x6F69746F\n.word 0x0000006E\n.word 0x00000000\n.word 0x6167654D\n.word 0x746F502D\n.word 0x006E6F69\n.word 0x00000000\n.word 0x65687445\n.word 0x00000072\n.word 0x6167654D\n.word 0x6874452D\n.word 0x00007265\n.word 0x00000000\n.word 0x78696C45\n.word 0x00007269\n.word 0x6167654D\n.word 0x6978696C\n.word 0x00000072\n.word 0x00000000\n.word 0x616E6F44\n.word 0x4420646C\n.word 0x006B6375\n.word 0x00000000\n.word 0x666F6F47\n.word 0x00000079\n.word 0x64616C41\n.word 0x006E6964\n.word 0x65697241\n.word 0x0000006C\n.word 0x6B63614A\n.word 0x00000000\n.word 0x65746550\n.word 0x61502072\n.word 0x0000006E\n.word 0x00000000\n.word 0x20656854\n.word 0x73616542\n.word 0x00000074\n.word 0x00000000\n.word 0x20656854\n.word 0x676E694B\n.word 0x00000000\n.word 0x00000000\n.word 0x74756C50\n.word 0x0000006F\n.word 0x72617547\n.word 0x72412064\n.word 0x00726F6D\n.word 0x00000000\n.word 0x63697254\n.word 0x73616D6B\n.word 0x00726574\n.word 0x00000000\n.word 0x6166614A\n.word 0x00000072\n.word 0x69676F4F\n.word 0x6F422065\n.word 0x6569676F\n.word 0x00000000\n.word 0x61726150\n.word 0x65746973\n.word 0x67614320\n.word 0x00000065\n.word 0x75737255\n.word 0x0000616C\n.word 0x67617244\n.word 0x4D206E6F\n.word 0x66656C61\n.word 0x6E656369\n.word 0x00000074\n.word 0x00000000\n.word 0x6B726144\n.word 0x65646973\n.word 0x00000000\n.word 0x00000000\n.word 0x64616853\n.word 0x0000776F\n.word 0x646C6F53\n.word 0x00726569\n.word 0x6772614C\n.word 0x6F422065\n.word 0x00007964\n.word 0x00000000\n.word 0x20646552\n.word 0x74636F4E\n.word 0x656E7275\n.word 0x00000000\n.word 0x65756C42\n.word 0x61685220\n.word 0x646F7370\n.word 0x00000079\n.word 0x6C6C6559\n.word 0x4F20776F\n.word 0x61726570\n.word 0x00000000\n.word 0x65657247\n.word 0x6552206E\n.word 0x65697571\n.word 0x0000006D\n.word 0x65776F50\n.word 0x6C697772\n.word 0x00000064\n.word 0x00000000\n.word 0x6E756F42\n.word 0x69777963\n.word 0x0000646C\n.word 0x00000000\n.word 0x20726941\n.word 0x646C6F53\n.word 0x00726569\n.word 0x00000000\n.word 0x646E6142\n.word 0x00007469\n.word 0x20746146\n.word 0x646E6142\n.word 0x00007469\n.word 0x00000000\n.word 0x72726142\n.word 0x53206C65\n.word 0x65646970\n.word 0x00000072\n.word 0x72616553\n.word 0x47206863\n.word 0x74736F68\n.word 0x00000000\n.word 0x20616553\n.word 0x6E6F654E\n.word 0x00000000\n.word 0x00000000\n.word 0x65726353\n.word 0x76696477\n.word 0x00007265\n.word 0x00000000\n.word 0x61757141\n.word 0x6B6E6174\n.word 0x00000000\n.word 0x00000000\n.word 0x68676957\n.word 0x6E4B2074\n.word 0x74686769\n.word 0x00000000\n.word 0x67726147\n.word 0x656C796F\n.word 0x00000000\n.word 0x00000000\n.word 0x61726950\n.word 0x00006574\n.word 0x20726941\n.word 0x61726950\n.word 0x00006574\n.word 0x00000000\n.word 0x6B726144\n.word 0x6C6C6162\n.word 0x00000000\n.word 0x00000000\n.word 0x65666544\n.word 0x7265646E\n.word 0x00000000\n.word 0x00000000\n.word 0x65767957\n.word 0x00006E72\n.word 0x617A6957\n.word 0x00006472\n.word 0x736F654E\n.word 0x6F646168\n.word 0x00000077\n.word 0x00000000\n.word 0x74696857\n.word 0x754D2065\n.word 0x6F726873\n.word 0x00006D6F\n.word 0x63616C42\n.word 0x7546206B\n.word 0x7375676E\n.word 0x00000000\n.word 0x65657243\n.word 0x20726570\n.word 0x6E616C50\n.word 0x00000074\n.word 0x6E726F54\n.word 0x206F6461\n.word 0x70657453\n.word 0x00000000\n.word 0x73657243\n.word 0x646E6563\n.word 0x0000006F\n.word 0x00000000\n.word 0x64726143\n.word 0x6C6F5320\n.word 0x72656964\n.word 0x00000000\n.word 0x65646148\n.word 0x00000073\n.word 0x6B6F6F48\n.word 0x00000000\n.word 0x756B6952\n.word 0x00000000\n.word 0x6C657841\n.word 0x00000000\n.word 0x7872614C\n.word 0x00656E65\n.word 0x65786556\n.word 0x0000006E\n.word 0x6C72614D\n.word 0x61697875\n.word 0x00000000\n.word 0x00000000\n.word 0x6178654C\n.word 0x00737565\n.word 0x65736E41\n.word 0x0000006D\n.word 0x6978655A\n.word 0x00006E6F\n.word 0x6E6D6558\n.word 0x00007361\n.word 0x62676958\n.word 0x00007261\n.word 0x646C6158\n.word 0x00006E69\n.word 0xC7836153\n.word 0x00000078\n.word 0x796D6544\n.word 0x00000078\n.word 0x6F78754C\n.word 0x00006472\n.word 0x61786F52\n.word 0x00000073\n.word 0x646C6F47\n.word 0x72614320\n.word 0x00000064\n.word 0x00000000\n.word 0x74616C50\n.word 0x6D756E69\n.word 0x72614320\n.word 0x00000064\n.word 0x64726143\n.word 0x6C6F5320\n.word 0x72656964\n.word 0x61705320\n.word 0x00006564\n.word 0x00000000\n.word 0x64726143\n.word 0x6C6F5320\n.word 0x72656964\n.word 0x61654820\n.word 0x00007472\n.word 0x00000000\n.word 0x89834E83\n.word 0x68834583\n.word 0x00000041\n.word 0x00000000\n.word 0x89834E83\n.word 0x68834583\n.word 0x00000042\n.word 0x00000000\n.word 0x66836E83\n.word 0x00415883\n.word 0x66836E83\n.word 0x00425883\n.word 0x62837483\n.word 0x00414E83\n.word 0x62837483\n.word 0x00424E83\n.word 0x62837483\n.word 0x00434E83\n.word 0x62837483\n.word 0x00444E83\n.word 0x4E834183\n.word 0x8B835A83\n.word 0x00000041\n.word 0x00000000\n.word 0x4E834183\n.word 0x8B835A83\n.word 0x00000042\n.word 0x00000000\n.word 0x4E838983\n.word 0x5B815683\n.word 0x00416B83\n.word 0x00000000\n.word 0x4E838983\n.word 0x5B815683\n.word 0x00426B83\n.word 0x00000000\n.word 0x4E838983\n.word 0x5B815683\n.word 0x00436B83\n.word 0x00000000\n.word 0x4E838A83\n.word 0x00000041\n.word 0x4E838A83\n.word 0x00000042\n.word 0x4E838A83\n.word 0x00000043\n.word 0x4E838A83\n.word 0x00000044\n.word 0x42839483\n.word 0x5A834E83\n.word 0x00419383\n.word 0x00000000\n.word 0x42839483\n.word 0x5A834E83\n.word 0x00429383\n.word 0x00000000\n.word 0x5B817D83\n.word 0x5B818B83\n.word 0x83835683\n.word 0x00000041\n.word 0x5B817D83\n.word 0x5B818B83\n.word 0x83835683\n.word 0x00000042\n.word 0x5B817D83\n.word 0x5B818B83\n.word 0x83835683\n.word 0x00000043\n.word 0x5B817D83\n.word 0x5B818B83\n.word 0x83835683\n.word 0x00000032\n.word 0x5B817D83\n.word 0x5B818B83\n.word 0x83835683\n.word 0x00000033\n.word 0x4E838C83\n.word 0x45835A83\n.word 0x00415883\n.word 0x00000000\n.word 0x4E838C83\n.word 0x45835A83\n.word 0x00425883\n.word 0x00000000\n.word 0x4E838C83\n.word 0x45835A83\n.word 0x00435883\n.word 0x00000000\n.word 0x93834183\n.word 0x80835A83\n.word 0x00000041\n.word 0x00000000\n.word 0x93834183\n.word 0x80835A83\n.word 0x00000042\n.word 0x00000000\n.word 0x4E835B83\n.word 0x49835683\n.word 0x00419383\n.word 0x00000000\n.word 0x8D838A83\n.word 0x68835B81\n.word 0x00000000\n.word 0x00000000\n.word 0x6E617254\n.word 0x6C697571\n.word 0x72614420\n.word 0x73656E6B\n.word 0x00000073\n.word 0x00000000\n.word 0x6D656554\n.word 0x20676E69\n.word 0x6B726144\n.word 0x7373656E\n.word 0x00000000\n.word 0x00000000\n.word 0x62656546\n.word 0x4420656C\n.word 0x6E6B7261\n.word 0x00737365\n.word 0x696D6C41\n.word 0x79746867\n.word 0x72614420\n.word 0x73656E6B\n.word 0x00000073\n.word 0x00000000\n.word 0x65656C53\n.word 0x676E6970\n.word 0x72614420\n.word 0x73656E6B\n.word 0x00000073\n.word 0x00000000\n.word 0x6D6F6F4C\n.word 0x20676E69\n.word 0x6B726144\n.word 0x7373656E\n.word 0x00000000\n.word 0x00000000\n.word 0x6D657250\n.word 0x206D7569\n.word 0x6D6F6F52\n.word 0x00000000\n.word 0x74696857\n.word 0x6F522065\n.word 0x00006D6F\n.word 0x00000000\n.word 0x63616C42\n.word 0x6F52206B\n.word 0x00006D6F\n.word 0x00000000\n.word 0x74746F42\n.word 0x656C6D6F\n.word 0x44207373\n.word 0x6E6B7261\n.word 0x00737365\n.word 0x00000000\n.word 0x6C756F52\n.word 0x65747465\n.word 0x6F6F5220\n.word 0x0000006D\n.word 0x7472614D\n.word 0x206C6169\n.word 0x696B6157\n.word 0x0000676E\n.word 0x63726F53\n.word 0x756F7265\n.word 0x61572073\n.word 0x676E696B\n.word 0x00000000\n.word 0x00000000\n.word 0x68636C41\n.word 0x63696D65\n.word 0x6B615720\n.word 0x00676E69\n.word 0x7465654D\n.word 0x20676E69\n.word 0x756F7247\n.word 0x0000646E\n.word 0x67617453\n.word 0x746E616E\n.word 0x61705320\n.word 0x00006563\n.word 0x6F727453\n.word 0x4920676E\n.word 0x6974696E\n.word 0x76697461\n.word 0x00000065\n.word 0x00000000\n.word 0x7473614C\n.word 0x20676E69\n.word 0x657A6144\n.word 0x00000000\n.word 0x6D6C6143\n.word 0x756F4220\n.word 0x0079746E\n.word 0x00000000\n.word 0x72617547\n.word 0x20646564\n.word 0x766F7254\n.word 0x00000065\n.word 0x736C6146\n.word 0x6F422065\n.word 0x79746E75\n.word 0x00000000\n.word 0x656D6F4D\n.word 0x7327746E\n.word 0x70655220\n.word 0x76656972\n.word 0x00000065\n.word 0x00000000\n.word 0x676E694D\n.word 0x676E696C\n.word 0x726F5720\n.word 0x0073646C\n.word 0x676F6F4D\n.word 0x5220656C\n.word 0x006D6F6F\n.word 0x00000000\n.word 0x646E6152\n.word 0x4A206D6F\n.word 0x72656B6F\n.word 0x00000000\n.word 0x2079654B\n.word 0x4220666F\n.word 0x6E696765\n.word 0x676E696E\n.word 0x00000073\n.word 0x00000000\n.word 0x2079654B\n.word 0x4720666F\n.word 0x61646975\n.word 0x0065636E\n.word 0x2079654B\n.word 0x54206F74\n.word 0x68747572\n.word 0x00000000\n.word 0x2079654B\n.word 0x52206F74\n.word 0x72617765\n.word 0x00007364\n.word 0x76617254\n.word 0x65737265\n.word 0x776F5420\n.word 0x0000006E\n.word 0x61726741\n.word 0x00686162\n.word 0x6D796C4F\n.word 0x20737570\n.word 0x696C6F43\n.word 0x6D756573\n.word 0x00000000\n.word 0x00000000\n.word 0x646E6F57\n.word 0x616C7265\n.word 0x0000646E\n.word 0x00000000\n.word 0x736E6F4D\n.word 0x006F7274\n.word 0x6C6C6148\n.word 0x6565776F\n.word 0x6F54206E\n.word 0x00006E77\n.word 0x616C7441\n.word 0x6369746E\n.word 0x00000061\n.word 0x00000000\n.word 0x6576654E\n.word 0x6E616C72\n.word 0x00000064\n.word 0x00000000\n.word 0x6C6C6F48\n.word 0x4220776F\n.word 0x69747361\n.word 0x00006E6F\n.word 0x20303031\n.word 0x65726341\n.word 0x6F6F5720\n.word 0x00007364\n.word 0x6C697754\n.word 0x74686769\n.word 0x776F5420\n.word 0x0000006E\n.word 0x74736544\n.word 0x20796E69\n.word 0x616C7349\n.word 0x0073646E\n.word 0x74736143\n.word 0x4F20656C\n.word 0x76696C62\n.word 0x006E6F69\n.word 0x90505845\n.word 0x00AC8FC2\n.word 0x90505845\n.word 0x008692C2\n.word 0x90505845\n.word 0x00E591C2\n.word 0x90505845\n.word 0x00AC8FD4\n.word 0x90505845\n.word 0x008692D4\n.word 0x90505845\n.word 0x00E591D4\n.word 0x8B505845\n.word 0x000000E2\n.word 0xAC8F5048\n.word 0x00000000\n.word 0xE5915048\n.word 0x00000000\n.word 0xAC8F504D\n.word 0x00000000\n.word 0xE591504D\n.word 0x00000000\n.word 0x00000050\n.word 0x00000000\n.word 0x00000052\n.word 0x00000000\n.word 0x6E6B6E55\n.word 0x206E776F\n.word 0x6D6F6F52\n.word 0x00000000\n.word 0x716E6F43\n.word 0x6F726575\n.word 0x20732772\n.word 0x70736552\n.word 0x00657469\n.word 0x00000000\n.word 0x6D6F6F52\n.word 0x20666F20\n.word 0x61776552\n.word 0x00736472\n.globl cardDataTbl_004F4ED0\ncardDataTbl_004F4ED0:\n.word 0x00000000\n.word D_004F2EC8\n.word 0x03000211\n.word D_004F43B8\n.word 0x03010211\n.word D_004F43C8\n.word 0x03020211\n.word D_004F43D8\n.word 0x03030211\n.word D_004F43E8\n.word 0x03040211\n.word D_004F43F8\n.word 0x03050211\n.word D_004F4408\n.word 0x03060211\n.word D_004F4418\n.word 0x03070211\n.word D_004F4428\n.word 0x03000311\n.word D_004F4438\n.word 0x03010311\n.word D_004F4440\n.word 0x03020311\n.word D_004F4450\n.word 0x03030311\n.word D_004F4460\n.word 0x03040311\n.word D_004F4470\n.word 0x03050311\n.word D_004F4480\n.word 0x03060311\n.word D_004F4490\n.word 0x03070311\n.word D_004F44A0\n.word 0x03000411\n.word D_004F44B0\n.word 0x04010411\n.word D_004F44C8\n.word 0x03020411\n.word D_004F44D8\n.word 0x03030411\n.word D_004F44E8\n.word 0x03040411\n.word D_004F44F8\n.word 0x03050411\n.word D_004F4508\n.word 0x03060411\n.word D_004F4518\n.word 0x03070411\n.word D_004F4528\n.word 0x00001A11\n.word D_004F4538\n.word 0x03000522\n.word D_004F4540\n.word 0x03010522\n.word D_004F4548\n.word 0x03020522\n.word D_004F4558\n.word 0x03030522\n.word D_004F4560\n.word 0x03040522\n.word D_004F4568\n.word 0x03050522\n.word D_004F4570\n.word 0x03060522\n.word D_004F4578\n.word 0x00021A22\n.word D_004F4538\n.word 0x00011A22\n.word D_004F4538\n.word 0x03070533\n.word D_004F4580\n.word 0x03000633\n.word D_004F4588\n.word 0x03010633\n.word D_004F4590\n.word 0x03020633\n.word D_004F4598\n.word 0x03030633\n.word D_004F45A0\n.word 0x03040633\n.word D_004F45B0\n.word 0x03050633\n.word D_004F45B8\n.word 0x06060644\n.word D_004F45C0\n.word 0x06070644\n.word D_004F45C8\n.word 0x02000744\n.word D_004F45D8\n.word 0x02010744\n.word D_004F45E8\n.word 0x02020744\n.word D_004F45F0\n.word 0x02030744\n.word D_004F4600\n.word 0x02040744\n.word D_004F4608\n.word 0x02050755\n.word D_004F4618\n.word 0x02060755\n.word D_004F4628\n.word 0x02070755\n.word D_004F4630\n.word 0x02000855\n.word D_004F4638\n.word 0x02010855\n.word D_004F4640\n.word 0x02020855\n.word D_004F4648\n.word 0x02030855\n.word D_004F4658\n.word 0x04040855\n.word D_004F4668\n.word 0x02050855\n.word D_004F4678\n.word 0x06060866\n.word D_004F4680\n.word 0x06060866\n.word D_004F4690\n.word 0x06060866\n.word D_004F46A0\n.word 0x06060866\n.word D_004F46A8\n.word 0x06060866\n.word D_004F46B8\n.word 0x06060866\n.word D_004F46C8\n.word 0x06060866\n.word D_004F46D0\n.word 0x06060866\n.word D_004F46E8\n.word 0x040708E6\n.word D_004F4538\n.word 0x000009E6\n.word D_004F4538\n.word 0x06010977\n.word D_004F46F8\n.word 0x02020977\n.word D_004F4700\n.word 0x06030977\n.word D_004F4708\n.word 0x02040977\n.word D_004F4718\n.word 0x02050977\n.word D_004F4728\n.word 0x02060977\n.word D_004F4738\n.word 0x02070977\n.word D_004F4748\n.word 0x06000A77\n.word D_004F4758\n.word 0x02010A77\n.word D_004F4768\n.word 0x02020A77\n.word D_004F4778\n.word 0x02030A77\n.word D_004F4788\n.word 0x06040A77\n.word D_004F4790\n.word 0x02050A77\n.word D_004F47A0\n.word 0x06060A77\n.word D_004F47B0\n.word 0x06070A77\n.word D_004F47C0\n.word 0x02000B77\n.word D_004F47D0\n.word 0x02010B77\n.word D_004F47E0\n.word 0x06020B77\n.word D_004F47F0\n.word 0x02030B77\n.word D_004F4800\n.word 0x06040B77\n.word D_004F4810\n.word 0x02050B77\n.word D_004F4818\n.word 0x04060B77\n.word D_004F4828\n.word 0x06070B77\n.word D_004F4838\n.word 0x02000C77\n.word D_004F4848\n.word 0x02010C77\n.word D_004F4850\n.word 0x02020C77\n.word D_004F4858\n.word 0x02030C77\n.word D_004F4868\n.word 0x02040C77\n.word D_004F4878\n.word 0x02050C77\n.word D_004F4888\n.word 0x02060C77\n.word D_004F4898\n.word 0x02070C77\n.word D_004F48A8\n.word 0x06000D77\n.word D_004F4680\n.word 0x06010D77\n.word D_004F46B8\n.word 0x06020D77\n.word D_004F4690\n.word 0x06030D77\n.word D_004F46E8\n.word 0x02040D77\n.word D_004F48B8\n.word 0x06050D77\n.word D_004F48C8\n.word 0x06060D77\n.word D_004F46A0\n.word 0x06070D77\n.word D_004F46A8\n.word 0x06000E77\n.word D_004F46C8\n.word 0x06010E77\n.word D_004F48D0\n.word 0x06020E77\n.word D_004F46D0\n.word 0x02030E77\n.word D_004F48D8\n.word 0x02040E77\n.word D_004F48E0\n.word 0x02050E77\n.word D_004F48E8\n.word 0x02060E77\n.word D_004F48F0\n.word 0x02070E77\n.word D_004F48F8\n.word 0x06000F77\n.word D_004F4908\n.word 0x02010F77\n.word D_004F4910\n.word 0x06030F77\n.word D_004F4918\n.word 0x02040F77\n.word D_004F4920\n.word 0x02050F77\n.word D_004F4928\n.word 0x02060F77\n.word D_004F4930\n.word 0x02070F77\n.word D_004F4938\n.word 0x02001077\n.word D_004F4940\n.word 0x02011077\n.word D_004F4948\n.word 0x02021077\n.word D_004F4950\n.word 0x02061577\n.word D_004F4958\n.word 0x02071577\n.word D_004F4968\n.word 0x00020F77\n.word D_004F4978\n.word 0x00040D77\n.word D_004F4990\n.word 0x00001617\n.word D_004F49A8\n.word 0x00011617\n.word D_004F49B8\n.word 0x00021617\n.word D_004F49C8\n.word 0x00031627\n.word D_004F49D0\n.word 0x00041617\n.word D_004F49D8\n.word 0x00051617\n.word D_004F49E0\n.word 0x00061617\n.word D_004F49E8\n.word 0x00071627\n.word D_004F49F0\n.word 0x00001717\n.word D_004F49F8\n.word 0x00011717\n.word D_004F4A08\n.word 0x00021717\n.word D_004F4A18\n.word 0x00031717\n.word D_004F4A28\n.word 0x00041727\n.word D_004F4A38\n.word 0x00051717\n.word D_004F4A48\n.word 0x00061717\n.word D_004F4A50\n.word 0x00071717\n.word D_004F4A58\n.word 0x00001817\n.word D_004F4A60\n.word 0x00011817\n.word D_004F4A68\n.word 0x00021827\n.word D_004F4A78\n.word 0x00031817\n.word D_004F4A88\n.word 0x00041817\n.word D_004F4A98\n.word 0x00051827\n.word D_004F4AA8\n.word 0x00061877\n.word D_004F4AB8\n.word 0x00071877\n.word D_004F4AC8\n.word 0x00001917\n.word D_004F4AD8\n.word 0x00011917\n.word D_004F4AE8\n.word 0x00021927\n.word D_004F4AF8\n.word 0x00031917\n.word D_004F4B08\n.word 0x00041927\n.word D_004F4B18\n.word 0x00051927\n.word D_004F4B28\n.word 0x00031A77\n.word D_004F4538\n.word 0x06000088\n.word D_004F4B38\n.word 0x06031099\n.word D_004F4B48\n.word 0x06041099\n.word D_004F4B60\n.word 0x06051099\n.word D_004F4B78\n.word 0x06061099\n.word D_004F4B88\n.word 0x06071099\n.word D_004F4BA0\n.word 0x06001199\n.word D_004F4BB8\n.word 0x02011199\n.word D_004F4BD0\n.word 0x02021199\n.word D_004F4BE0\n.word 0x06031199\n.word D_004F4BF0\n.word 0x06041199\n.word D_004F4C00\n.word 0x06051199\n.word D_004F4C18\n.word 0x060611A9\n.word D_004F4C28\n.word 0x020711A9\n.word D_004F4C38\n.word 0x020012A9\n.word D_004F4C50\n.word 0x060112A9\n.word D_004F4C60\n.word 0x060212A9\n.word D_004F4C70\n.word 0x060312A9\n.word D_004F4C80\n.word 0x060412A9\n.word D_004F4C98\n.word 0x020512B9\n.word D_004F4CA8\n.word 0x020612B9\n.word D_004F4CB8\n.word 0x020712B9\n.word D_004F4CC8\n.word 0x060013B9\n.word D_004F4CD8\n.word 0x060113B9\n.word D_004F4CF0\n.word 0x020213B9\n.word D_004F4D00\n.word 0x060313B9\n.word D_004F4D10\n.word 0x060413CA\n.word D_004F4D20\n.word 0x060513CA\n.word D_004F4D38\n.word 0x060613CA\n.word D_004F4D48\n.word 0x020713CA\n.word D_004F4D58\n.word 0x060014DB\n.word D_004F4D68\n.word 0x060314DB\n.word D_004F4D78\n.word 0x060214DB\n.word D_004F4D80\n.word 0x060114DB\n.word D_004F4D98\n.word 0x060514DB\n.word D_004F4DA8\n.word 0x060414DB\n.word D_004F4DB0\n.word 0x060614DB\n.word D_004F4DC0\n.word 0x060714DB\n.word D_004F4DD0\n.word 0x060015DB\n.word D_004F4DE0\n.word 0x020115DB\n.word D_004F4DF0\n.word 0x060215DB\n.word D_004F4E00\n.word 0x060315DB\n.word D_004F4E10\n.word 0x040415DB\n.word D_004F4E20\n.word 0x0000000D\n.word D_004F4E30\n.word 0x0001000D\n.word D_004F4E38\n.word 0x0002000D\n.word D_004F4E40\n.word 0x0003000D\n.word D_004F4E48\n.word 0x0004000D\n.word D_004F4E50\n.word 0x0005000D\n.word D_004F4E58\n.word 0x0006000D\n.word D_004F4E60\n.word 0x0000010D\n.word D_004F4E68\n.word 0x0001010D\n.word D_004F4E70\n.word 0x0000020D\n.word D_004F4E78\n.word 0x0001020D\n.word D_004F4E80\n.word 0x0000030D\n.word D_004F4E88\n.word 0x0000040D\n.word D_004F4E90\n.word 0x0000000C\n.word D_004F4E98\n.word 0x0000000C\n.word D_004F4EA8\n.word 0x0000000C\n.word D_004F4EC0\n.word 0x0000000C\n.word D_004F2EC8\n.word 0x0000000C\n.word D_004F2EC8\n.word 0x00000000\n.word D_004F2EC8\n.word 0x00000000\n.word D_004F2EC8\n.word 0x00000000\n.word D_004F2EC8\n.globl s_seqDataPlateTbl\ns_seqDataPlateTbl:\n.word 0x00650064\n.word 0x00610066\n.word 0x00630062\n.word 0x0070006F\n.word 0x006C0071\n.word 0x006E006D\n.word 0x00740073\n.word 0x00700075\n.word 0x00720071\n.word 0x00000000\n.globl s_seqDataActPlateTbl\ns_seqDataActPlateTbl:\n.word 0x00600063\n.word 0x006B006E\n.word 0x006F0072\n.word 0x00000000\n.word 0x00000000\n.word 0x00000000\n.text\n");
// SOURCE DATA: .data .rodata .bss
