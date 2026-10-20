#define FORM_MENU_JIMINYMEMO_CPP
#include "khtypes.h"
#include "Form/Menu/JiminyMemo.h"
#include "Form/Menu/Menu.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DFnt.h"
#include "System/Sound/SoundMan.h"
#include "Game/GameWork.h"
#include "Form/Menu/JMemoFlag.h"
#include "System/HeapMan.h"
#include "System/Debug.h"
#include "include_asm.h"

extern "C" char D_004E7A30[];

namespace D2 {
class C2DMan { /* PARTIAL: System/Graphic/2d/2DMan.h declares the class only for its own unit */
public:
    static void destroy(HANDLE hHandle);
    static HANDLE create(const CHAR* pBaseName, UINT inNo);
    static void setLayer(INT inLayer);
};
}

namespace JiminyMemo {
INT getHostGroupId();
INT getHostRsrcId();
INT getCT0014Code(USHORT inLow);
void playSE(INT inSENo);
BOOL is100AcreGoodEnding();
}

class CMemo2DLayout : public CTask {
private:
    LPCTSTR m_pLayoutName;          /* 0x40 */
    LPCTSTR m_pLayoutFileName;      /* 0x44 */
    INT* m_pLayoutNoTbl;            /* 0x48 */
    INT m_iState;                   /* 0x4c */
    UINT m_uiStateFlags;            /* 0x50 */
    HANDLE m_hLayer;                /* 0x54 */
    INT m_iSeqCtrlNum;              /* 0x58 */
    D2::C2DSeqCtrl* m_pSeqCtrlArray[32]; /* 0x5c */
    D2::C2DFnt* m_pSeqFntArray[32]; /* 0xdc */
    INT m_iSeqNum;                  /* 0x15c */
    struct LayoutDataParam* m_pSeqNoTbl; /* 0x160 */
    INT* m_pSeqCodeTbl;             /* 0x164 */
    INT m_iInitState;               /* 0x168 */
    INT m_iLayerLevel;              /* 0x16c */

public:
    CMemo2DLayout(CTask* p);
    virtual ~CMemo2DLayout();

protected:
    virtual void main();

private:
    void initialize();
    void finalize();
    void createLayer(INT inKind);
    void destroyLayer();

public:
    void init();
    void setState(INT inState);
    INT getState();
    UINT isStateFlag(UINT inFlags) { return m_uiStateFlags & inFlags; }
    D2::C2DSeqCtrl* getSeqCtrl(INT inSeqNo);
    D2::C2DFnt* getFnt(INT inSeqNo);
    D2::C2DFnt* setSeqText(UINT inSeqNo, const CHAR* inText);
};

struct CursorPosition {
    FLOAT x;
    FLOAT y;
};

void AddVectorXYZ(FVECTOR& out, const FVECTOR& a, const FVECTOR& b);
void ScaleVectorXYZ(FVECTOR& out, const FVECTOR& a, FLOAT s);

class CMemo2DCursor;
class CMemo2DSequence : public D2::C2DSeqCtrl {
private:
    FVECTOR m_vBasePosition;  /* 0x160 */
    FVECTOR m_vShiftPosition; /* 0x170 */
    FVECTOR m_vAimPosition;   /* 0x180 */
    FVECTOR m_vPosition;      /* 0x190 */
    SHORT m_iState;           /* 0x1a0 */
    USHORT m_iStateFlag;      /* 0x1a2 */
    int m_SeqTbl[5];          /* 0x1a4 */
    char m_LayoutName[32];    /* 0x1b8 */
    FLOAT m_fScale;           /* 0x1d8 */
    FLOAT m_fMoveTimer;       /* 0x1dc */
    BOOL m_bMoveSlideEnable;  /* 0x1e0 */

public:
    virtual ~CMemo2DSequence();
    void setPosition();
    void setLocalPosition();
    void setScale(FLOAT inScale);
    void replaceSequence(INT inSeqNo);
    void replaceSequenceId(INT inSeqId);
    void setInState(INT inSeqId);
    void setOutState(INT inSeqId);
};

class CMemo2DCursor : public D2::C2DSeqCtrl {
private:
    INT m_iPosiNum;           /* 0x160 */
    CursorPosition* m_pPosiTbl; /* 0x164 */
    float m_fSpacing[4];      /* 0x168 */
    INT m_iStartNo;           /* 0x178 */
    int m_iLadTbl[4];         /* 0x17c */
    INT m_iLadSeq;            /* 0x18c */
    LPCTSTR m_lpctLayoutName; /* 0x190 */
    FVECTOR m_vPosi;          /* 0x1a0 */
    FVECTOR m_vAimPosi;       /* 0x1b0 */
    FVECTOR m_vShiftPosi;     /* 0x1c0 */
    INT m_iPosiId;            /* 0x1d0 */
    BOOL m_bMoveSlideEnable;  /* 0x1d4 */
    FLOAT m_fMoveTimer;       /* 0x1d8 */
    HANDLE m_hParent;         /* 0x1dc */
    INT m_iState;             /* 0x1e0 */
    CursorPosition m_ScreenLimit[2]; /* 0x1e4 */
    FLOAT m_fScale;           /* 0x1f4 */
    UINT m_uiSize;            /* 0x1f8 */

public:
    virtual ~CMemo2DCursor();

private:
    void finalise();

public:
    INT setPosiId(INT inPosiId);
    void setScale(FLOAT inScale);
    void getSubVector(CursorPosition& outSub, INT inId0, INT inId1);
    void setLimitPosi(CursorPosition& inPosi, INT inId);
    CursorPosition* getLimitPosi(INT inId);
};

class CMemo2DCard : public D2::C2DSeqCtrl {
private:
    FVECTOR m_vBasePosition;  /* 0x160 */
    FVECTOR m_vShiftPosition; /* 0x170 */
    INT m_iState;             /* 0x180 */
    FLOAT m_fScale;           /* 0x184 */
    UINT m_iTexNo;            /* 0x188 */
    UINT m_iTexPos;           /* 0x18c */
    UINT m_iFrameSeqNo;       /* 0x190 */
    CHAR* m_pEmptySeqName;    /* 0x194 */

public:
    virtual ~CMemo2DCard();
    void setPosition();
    void setLocalPosition();
};

class CJiminyMemo : public CMenu {
private:
    UBYTE _m_0x80[0x10];  /* 0x080 */
    INT m_iStep;          /* 0x090 */
    INT m_iPlayerMode;    /* 0x094 */
    INT m_iContentMode;   /* 0x098 */
    HANDLE m_hLayout;     /* 0x09c */
    UINT m_uiStateFlags;  /* 0x0a0 */
    UBYTE pContentFunc[8]; /* 0x0a4 */
    D2::C2DSeqCtrl* m_pMessSeq; /* 0x0ac */
    D2::C2DFnt* m_pMessString;  /* 0x0b0 */
    CHAR* m_pLadName;     /* 0x0b4 */
    CHAR* m_pLadFileName; /* 0x0b8 */
    UBYTE m_Content[0x570 - 0xbc]; /* 0x0bc m_ContentCommon .. m_ContentMinigame */

    INT& work(INT inOfs) { return *(INT*)&m_Content[inOfs - 0xbc]; }

public:
    CJiminyMemo(CTask* inParent, INT inMode);
    virtual ~CJiminyMemo();

private:
    template <class T> T getTaskPointer(HANDLE inHandle);
    BOOL isTaskOut(HANDLE inHandle);
    UINT checkLayoutState(HANDLE inHandle, INT inFlags);
    void setLayoutState(HANDLE inHandle, INT inState);
    INT changeStateSequence(D2::C2DSeqCtrl** inSeq, INT inNum, INT inMode, INT inSeqId);
    INT changeStateLoopSequence(D2::C2DSeqCtrl** inSeq, INT inNum);
    INT changeStateOutSequence(D2::C2DSeqCtrl** inSeq, INT inNum);
    void setTelopMessage(const CHAR* inMess);
    INT getPrevContentId(INT inId, INT inNum, INT inMax);
    INT getNextContentId(INT inId, INT inNum, INT inMax);
    BOOL nextPage(INT* ioTop, INT inMax, INT inDisp);
    BOOL prevPage(INT* ioTop, INT inMin, INT inDisp);
    BOOL haveSpecialCard();
    INT getNextCardId(INT inId);
    INT getPrevCardId(INT inId);
    INT getNextCharaId(INT inId);
    INT getPrevCharaId(INT inId);
    INT getNextStoryId(INT inId);
    INT getPrevStoryId(INT inId);
};
typedef char _check_jm_size[sizeof(CJiminyMemo) == 0x570 ? 1 : -1];

INT JiminyMemo::getHostGroupId() {
    return 10;
}
INT JiminyMemo::getHostRsrcId() {
    return 0x7D4;
}
INT JiminyMemo::getCT0014Code(USHORT inLow) {
    return inLow | 0x700000;
}
void JiminyMemo::playSE(INT inSENo) {
    CSound::playSE(inSENo, -1, 64);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getOpenCloseStringData__10JiminyMemoii);
BOOL JiminyMemo::is100AcreGoodEnding() {
    return CGameWork::getInt(0, 0x23) & 0x40;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", __10CMemo3DObjP5CTaskiiUii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", _$_10CMemo3DObj);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", main__10CMemo3DObj);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", disp__10CMemo3DObj);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", calcLocalMatrix__10CMemo3DObj);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setRotateY__10CMemo3DObjf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setPosition__10CMemo3DObjRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", __13CMemo2DLayoutP5CTask);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", _$_13CMemo2DLayout);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", main__13CMemo2DLayout);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setup__13CMemo2DLayoutPCcT1PCiiPC15LayoutDataParami);
void CMemo2DLayout::init() {
    if (m_uiStateFlags & 0x20) initialize();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", initialize__13CMemo2DLayout);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", finalize__13CMemo2DLayout);
void CMemo2DLayout::setState(INT inState) {
    m_iState = inState;
}
INT CMemo2DLayout::getState() {
    return m_iState;
}
void CMemo2DLayout::createLayer(INT inKind) {
    D2::C2DMan::setLayer(m_iLayerLevel);
    m_hLayer = D2::C2DMan::create(m_pLayoutFileName, m_pLayoutNoTbl[inKind]);
    if (!m_hLayer) TRACE_USER((USER)10, D_004E7A30, inKind);
}
void CMemo2DLayout::destroyLayer() {
    D2::C2DMan::destroy(m_hLayer);
    m_hLayer = 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", createSequence__13CMemo2DLayoutii);
D2::C2DSeqCtrl* CMemo2DLayout::getSeqCtrl(INT inSeqNo) {
    if (inSeqNo < 0 || inSeqNo >= m_iSeqNum) return NULL;
    return m_pSeqCtrlArray[inSeqNo];
}
D2::C2DFnt* CMemo2DLayout::getFnt(INT inSeqNo) {
    if (inSeqNo < 0 || inSeqNo >= m_iSeqNum) return NULL;
    return m_pSeqFntArray[inSeqNo];
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setSeqState__13CMemo2DLayoutUii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getSeqState__13CMemo2DLayoutUi);
D2::C2DFnt* CMemo2DLayout::setSeqText(UINT inSeqNo, const CHAR* inText) {
    if (inSeqNo >= m_iSeqNum) return NULL;
    D2::C2DFnt* pFnt = m_pSeqFntArray[inSeqNo];
    if (pFnt) pFnt->setText(inText);
    return pFnt;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", __15CMemo2DSequence);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", _$_15CMemo2DSequence);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setup__15CMemo2DSequencePCcPCii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setup__15CMemo2DSequencePCcii);
void CMemo2DSequence::setPosition() {
    FVECTOR v;
    AddVectorXYZ(v, m_vBasePosition, m_vShiftPosition);
    setPos(v);
}
void CMemo2DSequence::setLocalPosition() {
    FVECTOR v;
    AddVectorXYZ(v, m_vBasePosition, m_vShiftPosition);
    setLocalPos(v);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", main__15CMemo2DSequencef);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setSlideMoveParam__15CMemo2DSequenceP13CMemo2DCursor);
void CMemo2DSequence::setScale(FLOAT inScale) {
    m_fScale = inScale;
    if (inScale < 0.01f) m_fScale = 0.01f;
    if (m_iStateFlag & 2) setScl(m_fScale);
}
void CMemo2DSequence::replaceSequence(INT inSeqNo) {
    replaceSeq(0, m_LayoutName, inSeqNo);
}
void CMemo2DSequence::replaceSequenceId(INT inSeqId) {
    replaceSeq(0, m_LayoutName, m_SeqTbl[inSeqId]);
}
void CMemo2DSequence::setInState(INT inSeqId) {
    if (m_nSeq == 0) {
        addSeq(m_LayoutName, m_SeqTbl[inSeqId]);
        enableDisp(TRUE);
    } else {
        replaceSequenceId(inSeqId);
    }
    m_iState = 1;
}
void CMemo2DSequence::setOutState(INT inSeqId) {
    replaceSequenceId(inSeqId);
    m_iState = 3;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", __13CMemo2DCursori);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", _$_13CMemo2DCursor);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setup__13CMemo2DCursorR15CursorDataParam);
void CMemo2DCursor::finalise() {
    if (m_pPosiTbl) com_free(m_pPosiTbl);
    m_pPosiTbl = NULL;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setReplaceSeq__13CMemo2DCursori);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setState__13CMemo2DCursori);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setPosiId__13CMemo2DCursori);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", main__13CMemo2DCursor);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getFrameRateF__13CMemo2DCursor);
void CMemo2DCursor::setLimitPosi(CursorPosition& inPosi, INT inId) {}
CursorPosition* CMemo2DCursor::getLimitPosi(INT inId) {
    return NULL;
}
void CMemo2DCursor::setScale(FLOAT inScale) {
    m_fScale = inScale;
    if (m_iState & 0x4000000) setScl(inScale);
    setPosiId(m_iPosiId);
}
void CMemo2DCursor::getSubVector(CursorPosition& outSub, INT inId0, INT inId1) {
    outSub.x = m_pPosiTbl[inId0].x - m_pPosiTbl[inId1].x;
    outSub.y = m_pPosiTbl[inId0].y - m_pPosiTbl[inId1].y;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", __11CMemo2DCardPCc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", _$_11CMemo2DCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", create__11CMemo2DCardG4CARDi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", createPicture__11CMemo2DCardUiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", changePicture__11CMemo2DCardPQ22D29C2DSeqObj);
void CMemo2DCard::setPosition() {
    FVECTOR v;
    AddVectorXYZ(v, m_vBasePosition, m_vShiftPosition);
    ScaleVectorXYZ(v, v, m_fScale);
    setPos(v);
}
void CMemo2DCard::setLocalPosition() {
    FVECTOR v;
    AddVectorXYZ(v, m_vBasePosition, m_vShiftPosition);
    ScaleVectorXYZ(v, v, m_fScale);
    setLocalPos(v);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setState__11CMemo2DCardi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", main__11CMemo2DCardf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", __11CJiminyMemoP5CTaski);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", _$_11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", initialize__11CJiminyMemoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", finalize__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", main__11CJiminyMemo);
UINT CJiminyMemo::checkLayoutState(HANDLE inHandle, INT inFlags) {
    CMemo2DLayout* p = getTaskPointer<CMemo2DLayout*>(inHandle);
    if (p) return p->isStateFlag(inFlags);
    return 1;
}
void CJiminyMemo::setLayoutState(HANDLE inHandle, INT inState) {
    CMemo2DLayout* p = getTaskPointer<CMemo2DLayout*>(inHandle);
    if (p) p->setState(inState);
}
BOOL CJiminyMemo::isTaskOut(HANDLE inHandle) {
    return getTaskPointer<CTask*>(inHandle) == NULL;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setContentMode__11CJiminyMemoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", createSequence__11CJiminyMemoPCiiiiP15CMemo2DSequence);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", createSequence__11CJiminyMemoiiiiP15CMemo2DSequence);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", createListSequence__11CJiminyMemoP15CMemo2DSequenceiiiPPQ22D210C2DSeqCtrlPPQ22D26C2DFntUiiiPCc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", createListSequenceTypewriterFont__11CJiminyMemoP15CMemo2DSequenceiiiPPQ22D210C2DSeqCtrlPPQ22D26C2DFntUiiiPCcf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", replaceNoteList__11CJiminyMemoPPQ22D26C2DFntPPCci);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", replaceNoteListWithOpenCheck__11CJiminyMemoPPQ22D26C2DFntUsiiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", replaceNoteListWithOpenCheck__11CJiminyMemoPPQ22D26C2DFntUsPUciii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", replaceNoteListWithOpenCheck__11CJiminyMemoPPQ22D26C2DFntPUsiiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", replaceNoteListMark__11CJiminyMemoPPQ22D210C2DSeqCtrlPUciiiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", replaceNoteListMark__11CJiminyMemoPPQ22D210C2DSeqCtrliiiiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", createCollectionCursorSequence__11CJiminyMemoPCiRC14CursorPositionT2iiiiiiiifP15CMemo2DSequence);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", createCursorPosiTbl__11CJiminyMemoP14CursorPositionRC14CursorPositionT2ii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", createCursorSequence__11CJiminyMemoPCiiiRC14CursorPositioniiiiiiiiP15CMemo2DSequence);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", createMarkSequence__11CJiminyMemoiiiiiPQ22D210C2DSeqCtrlPPQ22D210C2DSeqCtrl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", changeStateSequence__11CJiminyMemoPPQ22D210C2DSeqCtrliii);
INT CJiminyMemo::changeStateLoopSequence(D2::C2DSeqCtrl** inSeq, INT inNum) {
    return changeStateSequence(inSeq, inNum, 0, 0x25);
}
INT CJiminyMemo::changeStateOutSequence(D2::C2DSeqCtrl** inSeq, INT inNum) {
    return changeStateSequence(inSeq, inNum, 1, 0x49);
}
void CJiminyMemo::setTelopMessage(const CHAR* inMess) {
    if (m_pMessString) m_pMessString->setText(inMess);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", splitMessage__11CJiminyMemoPiPPPcPCci);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setPlateState__11CJiminyMemoii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getPlateState__11CJiminyMemoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setPlateText__11CJiminyMemoii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setPlateNumSeq__11CJiminyMemoii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getMenuContentFlag__11CJiminyMemoiiPiT3);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getPrevContentId__11CJiminyMemoiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getNextContentId__11CJiminyMemoiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", ScrollBarInit__11CJiminyMemoPPQ22D210C2DSeqCtrliR7FVECTORfP15CMemo2DSequence);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", ScrollBarChange__11CJiminyMemoPPQ22D210C2DSeqCtrlffiii);
BOOL CJiminyMemo::nextPage(INT* ioTop, INT inMax, INT inDisp) {
    BOOL bRet;
    INT nLimit = inMax - inDisp;
    if (*ioTop < nLimit) {
        bRet = TRUE;
        *ioTop += inDisp;
        if (*ioTop > nLimit) *ioTop = nLimit;
    } else {
        bRet = FALSE;
    }
    return bRet;
}
BOOL CJiminyMemo::prevPage(INT* ioTop, INT inMin, INT inDisp) {
    BOOL bRet;
    if (*ioTop > inMin) {
        bRet = TRUE;
        *ioTop -= inDisp;
        if (*ioTop < inMin) *ioTop = inMin;
    } else {
        bRet = FALSE;
    }
    return bRet;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", nextPage__11CJiminyMemoP13CMemo2DCursorPiiif);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", prevPage__11CJiminyMemoP13CMemo2DCursorPiiif);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", isButtonRptLimitStop__11CJiminyMemoUiUii);
BOOL CJiminyMemo::haveSpecialCard() {
    BOOL bRet = FALSE;
    if (!m_iPlayerMode) {
        if (CJiminyMemoFlag::isContentOpen(0x14D) || CJiminyMemoFlag::isContentOpen(0x14E)) bRet = TRUE;
    }
    return bRet;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", contentMenu__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", contentStory__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", contentStoryView__11CJiminyMemo);
INT CJiminyMemo::getPrevStoryId(INT inId) {
    return getPrevContentId(inId, work(0x178), work(0x170));
}
INT CJiminyMemo::getNextStoryId(INT inId) {
    return getNextContentId(inId, work(0x178), work(0x170));
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", contentCollection__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", CardScreenSideClip__11CJiminyMemoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", CardScreenOutClip__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setParamFromCollectToCardView__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", contentCard__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", contentCardSelect__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", contentCardView__11CJiminyMemo);
INT CJiminyMemo::getPrevCardId(INT inId) {
    return getPrevContentId(inId, work(0x2D4), work(0x2D0));
}
INT CJiminyMemo::getNextCardId(INT inId) {
    return getNextContentId(inId, work(0x2D4), work(0x2D0));
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setParamFromCardViewToCollect__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", contentChara__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", contentCharaSelect__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", contentCharaView__11CJiminyMemo);
INT CJiminyMemo::getPrevCharaId(INT inId) {
    return getPrevContentId(inId, work(0x3A0), work(0x39C));
}
INT CJiminyMemo::getNextCharaId(INT inId) {
    return getNextContentId(inId, work(0x3A0), work(0x39C));
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", contentCharaView_ModelView__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", contentMinigame__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getTaskPointer__H1ZP13CMemo2DLayout_11CJiminyMemoi_X01);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getTaskPointer__H1ZP5CTask_11CJiminyMemoi_X01);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getTaskPointer__H1ZP10CMemo3DObj_11CJiminyMemoi_X01);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", __static_initialization_and_destruction_0_00335600);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", isErase__10CMemo3DObj);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", isLoadEnable__10CMemo3DObj);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", isLightOff__10CMemo3DObj);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setErase__10CMemo3DObj);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getRotateY__10CMemo3DObj);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getPosition__10CMemo3DObj);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setScale__10CMemo3DObjf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getScale__10CMemo3DObj);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setLightOff__10CMemo3DObji);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setFadeCountMax__10CMemo3DObjf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setNotSemiFlag__10CMemo3DObji);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setFade2PathFlag__10CMemo3DObji);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", isEndLayer__13CMemo2DLayout);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setStateFlag__13CMemo2DLayoutii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", isStateFlag__13CMemo2DLayouti);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", isFontSeqTbl__13CMemo2DLayouti);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getSeqCtrlNum__13CMemo2DLayout);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getSeqNum__13CMemo2DLayout);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setBasePosition__15CMemo2DSequenceRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getBasePosition__15CMemo2DSequence);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setShiftPosition__15CMemo2DSequenceRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setAimPosition__15CMemo2DSequenceRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setPosition__15CMemo2DSequenceRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setLocalPosition__15CMemo2DSequenceRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setState__15CMemo2DSequencei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setStateFlag__15CMemo2DSequenceUsUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", isStateFlag__15CMemo2DSequenceUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setMoveTimer__15CMemo2DSequencef);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getMoveTimer__15CMemo2DSequence);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", addMoveTimer__15CMemo2DSequencef);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getSideInId__15CMemo2DSequencei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getSideOutId__15CMemo2DSequencei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setShiftVector__13CMemo2DCursorRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getShiftVector__13CMemo2DCursor);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getAimVector__13CMemo2DCursor);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getPosition__13CMemo2DCursor);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setSeqNo__13CMemo2DCursori);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setBasePosition__11CMemo2DCardRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getBasePosition__11CMemo2DCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setShiftPosition__11CMemo2DCardRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getShiftPosition__11CMemo2DCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setScale__11CMemo2DCardf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setStateFlag__11CJiminyMemoii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", isStateFlag__11CJiminyMemoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", isModeJiminyMemo__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", isModeDReport__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getLoadResourceIndex__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setContentStep__11CJiminyMemoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getContentStep__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setContentTimer__11CJiminyMemof);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getContentTimer__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setNextContentMode__11CJiminyMemoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getNextContentMode__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setPrevContentMode__11CJiminyMemoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getPrevContentMode__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setContentParam__11CJiminyMemoii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getContentParam__11CJiminyMemoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", setContentScroll__11CJiminyMemoiRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", getContentScroll__11CJiminyMemoiR7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", mainInit__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", mainLoadWait__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", mainIn__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", mainInWait__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", mainLoop__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", mainOut__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", mainOutWait__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", replaceNoteListString__11CJiminyMemoPQ22D26C2DFntPCc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", _GLOBAL_$I$c__KHCOM_MainApp_Form_Menu_JiminyMemo.cppbgbaaa);
