#define FORM_MENU_JIMINYMEMO_CPP /* MATCH: unit gate (its header gives this unit its own parts); without it the unit compiles only against its full header (tested 2026-11-02) */
#include "khtypes.h"
#include "Form/Menu/Menu.h"
#include "System/TaskMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/2d/2DMan.h"
#define CTDMAN_H_PATH "System/Ctd/CtdMan.h" /* MATCH: this unit names CtdMan.h so (header default "System/CTD/CtdMan.h") */
#include "System/Ctd/CtdMan.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "System/Graphic/TexAnm.h"
#include "Game/CardBattle.h"
#include "Form/Menu/JiminyMemo.h"
#include "System/Math3D.h"
#include "System/Com.h"
#include "Game/CardKind.h"
#include "System/Graphic/Camera.h"
#include "System/Graphic/WorldEnv.h"
#include "System/Rcf.h"
#include "System/Graphic/2d/2DSeqObj.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DFnt.h"
#include "System/Sound/SoundMan.h"
#include "Game/GameWork.h"
#include "Form/Menu/JMemoFlag.h"
#include "System/HeapMan.h"
#include "System/Debug.h"
#include "include_asm.h"

inline void* operator new(size_t, void* p) throw() { return p; }












#include "System/RsrcDB.h"









namespace JiminyMemo {
INT getHostGroupId();
INT getHostRsrcId();
INT getCT0014Code(USHORT inLow);
void playSE(INT inSENo);
BOOL is100AcreGoodEnding();
CHAR* getOpenCloseStringData(INT inCTNo, BOOL inIsFlag);
extern INT RsrcId[2];
}

class CLightUnit;
#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
inline void* operator new(size_t size, char* msg) throw() { return com_malloc_d(size, msg, RET_ADDR()); }
inline void* operator new[](size_t size, char* msg) throw() { return com_malloc_d(size, msg, RET_ADDR()); }
inline void operator delete[](void* mem) { com_free(mem); }
class CMemo3DObj : public CTask {
public:
    CMemo3DObj(CTask* p, INT inGroupId, INT inRsrcId, UINT inDispLayer, BOOL inLoadEnable);
    virtual ~CMemo3DObj();
    virtual void main();
    virtual void disp();
    void setRotateY(FLOAT inRotY);
    BOOL isLoadEnable() { return m_bLoadEnable; }
    void setPosition(const FVECTOR& inPosition);
    void calcLocalMatrix();
    BOOL isLightOff() { return m_bLightOff; }
    void setErase() { m_bEraseModel = TRUE; }
    FLOAT getRotateY() { return m_fRotY; }
    void setScale(FLOAT s) { m_fScale = s; }
    void setFadeCountMax(FLOAT inMax) { m_fFadeCountMax = inMax; }
    void setNotSemiFlag(BOOL inFlag) { m_bNotSemiFlag = inFlag; }
    void setFade2PathFlag(BOOL inFlag) { m_bFade2PathFlag = inFlag; }

private:
    FLOAT m_fRotY;           /* 0x40 */
    CModel* m_pModels;       /* 0x44 */
    INT m_iModelCount;       /* 0x48 */
    CAnim* m_pAnime;         /* 0x4c */
    CTexAnm* m_pTexAnim;     /* 0x50 */
    INT m_iTexAnimNum;       /* 0x54 */
    INT m_iTexAnimNo;        /* 0x58 */
    INT m_iTexAnimSeqNo;     /* 0x5c */
    CLightUnit* m_pLight;    /* 0x60 */
    INT m_iStep;             /* 0x64 */
    INT m_iGroupId;          /* 0x68 */
    INT m_iResourceId;       /* 0x6c */
    FLOAT m_fFadeCount;      /* 0x70 */
    FLOAT m_fFadeCountMax;   /* 0x74 */
    BOOL m_bEraseModel;      /* 0x78 */
    BOOL m_bTouch;           /* 0x7c */
    FVECTOR m_vPosition;     /* 0x80 */
    FVECTOR m_vBasePosition; /* 0x90 */
    FLOAT m_fBaseScale;      /* 0xa0 */
    FLOAT m_fScale;          /* 0xa4 */
    UINT m_uiDispLayer;      /* 0xa8 */
    BOOL m_bLoadEnable;      /* 0xac */
    HANDLE m_hEffect;        /* 0xb0 */
    BOOL m_bLightOff;        /* 0xb4 */
    BOOL m_bNotSemiFlag;     /* 0xb8 */
    BOOL m_bFade2PathFlag;   /* 0xbc */
};

struct LayoutDataParam {
    INT In;
    INT Loop;
    INT Out;
    FLOAT XPosi;
    FLOAT YPosi;
    INT Layer;
    UINT CTCode;
};

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
    INT setup(const CHAR* inSeqName, const CHAR* inFileName, const INT* inLayNoTbl, INT inSeqNum,
              const LayoutDataParam* inSeqNoTbl, INT inLayerLevel);
    void setState(INT inState);
    INT getState();
    BOOL isEndLayer() { return D2::C2DMan::isEnd(m_hLayer); }
    void setStateFlag(INT inOn, INT inOff) { m_uiStateFlags = (m_uiStateFlags & ~inOff) | inOn; }
    UINT isStateFlag(UINT inFlags) { return m_uiStateFlags & inFlags; }
    D2::C2DSeqCtrl* getSeqCtrl(INT inSeqNo);
    D2::C2DFnt* getFnt(INT inSeqNo);
    D2::C2DFnt* setSeqText(UINT inSeqNo, const CHAR* inText);
    D2::C2DSeqCtrl* setSeqState(UINT inSeqNo, INT inState);
    INT getSeqState(UINT inSeqNo);
    D2::C2DSeqCtrl* createSequence(INT inSeqNo, INT inLayer);
};

struct CursorPosition {
    FLOAT x;
    FLOAT y;

    void copy(const CursorPosition& cp) {
        x = cp.x;
        y = cp.y;
    }
};
struct CursorDataParam {
    INT posiNum;
    CursorPosition* posiTbl;
    int spacing[4];
    INT startNo;
    int ladTbl[4];
    CursorPosition* limitTbl;
    LPCTSTR LayoutName;
    FLOAT scale;

    void init() {
        posiNum = 0;
        posiTbl = NULL;
        for (INT i = 0; i < 4; i++) spacing[i] = 0;
        startNo = 0;
        for (INT i = 0; i < 4; i++) ladTbl[i] = -1;
        limitTbl = NULL;
        LayoutName = NULL;
        scale = 1.0f;
    }
    void set(LPCTSTR _layoutName, INT _posiNum, INT _startNo, const INT* _ladTbl, CursorPosition* _limitTbl,
             FLOAT _scale = 1.0f) {
        posiNum = _posiNum;
        startNo = _startNo;
        for (INT i = 0; i < 4; i++) ladTbl[i] = _ladTbl[i];
        limitTbl = _limitTbl;
        LayoutName = _layoutName;
        scale = _scale;
    }
    void setSpacing(INT _left, INT _top, INT _right, INT _bottom) {
        spacing[0] = _left;
        spacing[1] = _top;
        spacing[2] = _right;
        spacing[3] = _bottom;
    }
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
    CMemo2DSequence();
    virtual ~CMemo2DSequence();
    void setPosition();
    void setLocalPosition();
    void setScale(FLOAT inScale);
    void replaceSequence(INT inSeqNo);
    void replaceSequenceId(INT inSeqId);
    void setInState(INT inSeqId);
    void setOutState(INT inSeqId);
    void setSlideMoveParam(class CMemo2DCursor* inCursor);
    void main(FLOAT inFrameRate);
    void setState(INT inState) { m_iState = inState; }
    void setBasePosition(const FVECTOR& inPosition) { m_vBasePosition = inPosition; }
    void setLocalPosition(FVECTOR& inPosi) {
        m_vPosition = inPosi;
        setLocalPos(m_vPosition);
    }
    void setStateFlag(USHORT inOrFlag, USHORT inAndFlag) { m_iStateFlag = (m_iStateFlag & ~inAndFlag) | inOrFlag; }
    void addMoveTimer(FLOAT inTimeQ) { m_fMoveTimer += inTimeQ; }
    INT getSideInId(INT inId) { return inId == 1 ? 0 : 3; }
    INT getSideOutId(INT inId) { return inId == 1 ? 2 : 4; }
    void setup(const CHAR* inSeqName, const INT* inSeqTbl, INT inLayer);
    void setup(const CHAR* inSeqName, INT inSeqNo, INT inLayer);
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
    CMemo2DCursor(HANDLE inHandle);
    virtual ~CMemo2DCursor();

private:
    void finalise();

public:
    void setup(CursorDataParam& inParam);
    void main();
    INT setPosiId(INT inPosiId);
    void setScale(FLOAT inScale);
    void getSubVector(CursorPosition& outSub, INT inId0, INT inId1);
    void setLimitPosi(CursorPosition& inPosi, INT inId);
    FVECTOR& getShiftVector() { return m_vShiftPosi; }
    void setShiftVector(const FVECTOR& inVector) { m_vShiftPosi = inVector; }
    FVECTOR& getAimVector() { return m_vAimPosi; }
    FVECTOR& getPosition() { return m_vPosi; }
    void setSeqNo(INT inSeqNo) { m_iLadTbl[3] = inSeqNo; }
    CursorPosition* getLimitPosi(INT inId);
    FLOAT getFrameRateF();
    void setReplaceSeq(INT inSeq);
    void setState(INT inState);
};

namespace JiminyMemo {
extern UINT CursorSeqTbl[];
}
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
    CMemo2DCard(const CHAR* inEmptySeqName);
    virtual ~CMemo2DCard();
    void setPosition();
    void setLocalPosition();
    void create(CARD inCard, INT inMode);
    void createPicture(UINT inTexNo, UINT inTexPos);
    void changePicture(D2::C2DSeqObj* p2DSeqObj);
    void setState(INT inState);
    void main(FLOAT inFrameRate);
    void setBasePosition(const FVECTOR& inPosition) { m_vBasePosition = inPosition; }
    FVECTOR& getBasePosition() { return m_vBasePosition; }
    void setShiftPosition(const FVECTOR& inShift) { m_vShiftPosition = inShift; }
    void setScale(FLOAT inScale) {
        m_fScale = inScale;
        setScl(inScale);
    }
};

struct ContentCommonWork {
    INT iStep;
    FLOAT fTimer;
    INT iNextContent;
    INT iPrevContent;
    int param[2];
    CursorPosition scroll[2];

    void init() {
        iStep = 0;
        fTimer = 0.0f;
        iNextContent = 0;
        iPrevContent = -1;
    }
};

typedef USHORT CT_LOWER;
struct ContentMenuWork {
    HANDLE hJiminyObj; /* 0x00 */
    CMemo2DSequence* pList; /* 0x04 */
    CMemo2DSequence* pTalk; /* 0x08 */
    CMemo2DCursor* pCursor; /* 0x0c */
    CMemo2DSequence* pCursorLight; /* 0x10 */
    INT iStringNum; /* 0x14 */
    D2::C2DFnt** pString; /* 0x18 */
    INT iSeqCtrlNum; /* 0x1c */
    D2::C2DSeqCtrl** pSeqCtrl; /* 0x20 */
    INT iCursorCurrent; /* 0x24 */
    INT iCursorPosiNum; /* 0x28 */
    INT iTelopNo; /* 0x2c */

    void init() {
        hJiminyObj = 0;
        pList = NULL;
        pTalk = NULL;
        pCursor = NULL;
        pCursorLight = NULL;
        pString = NULL;
        pSeqCtrl = NULL;
        iCursorCurrent = 0;
        iCursorPosiNum = 0;
        iTelopNo = 0;
    }
};
struct ContentStoryWork {
    CMemo2DSequence* pList; /* 0x00 */
    CMemo2DCursor* pCursor; /* 0x04 */
    D2::C2DFnt** pString; /* 0x08 */
    INT iStringNum; /* 0x0c */
    D2::C2DSeqCtrl** pSeqCtrl; /* 0x10 */
    INT iSeqCtrlNum; /* 0x14 */
    D2::C2DSeqCtrl** pNewMarkSeq; /* 0x18 */
    D2::C2DSeqCtrl** pScrollBar; /* 0x1c */
    INT iCursorCurrent; /* 0x20 */
    FLOAT fCursorIntervalY; /* 0x24 */
    SHORT iNoteListNum; /* 0x28 */
    SHORT iCursorPosiNum; /* 0x2a */
    INT iCTCodeOffset; /* 0x2c */

    void init() {
        pList = NULL;
        pCursor = NULL;
        pString = NULL;
        pSeqCtrl = NULL;
        pNewMarkSeq = NULL;
        pScrollBar = NULL;
        iCursorCurrent = 0;
        fCursorIntervalY = 0.0f;
        iStringNum = 0;
        iSeqCtrlNum = 0;
        iNoteListNum = 0;
        iCursorPosiNum = 0;
        iCTCodeOffset = 0;
    }
};
struct ContentStoryViewWork {
    CMemo2DSequence* pList; /* 0x00 */
    CHAR** pTextLineBuf; /* 0x04 */
    INT iStringNum; /* 0x08 */
    D2::C2DFnt** pString; /* 0x0c */
    INT iSeqCtrlNum; /* 0x10 */
    D2::C2DSeqCtrl** pSeqCtrl; /* 0x14 */
    D2::C2DSeqCtrl* pTopArrowSeq; /* 0x18 */
    D2::C2DSeqCtrl* pBottomArrowSeq; /* 0x1c */
    D2::C2DSeqCtrl* pPictSeq; /* 0x20 */
    INT iCurrentTextLine; /* 0x24 */
    INT iTextLineNum; /* 0x28 */
    INT iWorldNo; /* 0x2c */
    INT iCTCodeOffset; /* 0x30 */
    INT iAllStoryNum; /* 0x34 */
    INT iPictureNo; /* 0x38 */
    INT iPrevNo; /* 0x3c */
    INT iNextNo; /* 0x40 */
    USHORT iGroupId; /* 0x44 */
    USHORT iRsrcId; /* 0x46 */
    char pPictFileName[16]; /* 0x48 */
    char pPictSeqName[16]; /* 0x58 */

    void init() {
        pList = NULL;
        pTextLineBuf = NULL;
        pString = NULL;
        pSeqCtrl = NULL;
        pTopArrowSeq = NULL;
        pBottomArrowSeq = NULL;
        pPictSeq = NULL;
        iCurrentTextLine = 0;
        iTextLineNum = 0;
        iStringNum = 0;
        iSeqCtrlNum = 0;
        iWorldNo = 0;
        iCTCodeOffset = 0;
        iAllStoryNum = 0;
        iPictureNo = 0;
        iPrevNo = -1;
        iNextNo = -1;
        iGroupId = 0;
        iRsrcId = 0;
        for (INT i = 0; i < 16; i++) pPictFileName[i] = 0;
        for (INT i = 0; i < 16; i++) pPictSeqName[i] = 0;
    }
};
struct ContentCollectWork {
    CMemo2DSequence* pList; /* 0x00 */
    CMemo2DSequence* pBase; /* 0x04 */
    CMemo2DCursor* pCursor; /* 0x08 */
    CMemo2DSequence* pLight; /* 0x0c */
    CMemo2DSequence* pFrame; /* 0x10 */
    CMemo2DSequence* pFlicker; /* 0x14 */
    D2::C2DFnt** pString; /* 0x18 */
    INT iStringNum; /* 0x1c */
    D2::C2DSeqCtrl** pSeqCtrl; /* 0x20 */
    INT iSeqCtrlNum; /* 0x24 */
    CMemo2DCard** pCard; /* 0x28 */
    INT iCardNum; /* 0x2c */
    SHORT sCursorPosiX; /* 0x30 */
    SHORT sCursorPosiY; /* 0x32 */
    char Percentage[16]; /* 0x34 */
    FLOAT fScale; /* 0x44 */
    FLOAT fZoomTimer; /* 0x48 */
    FLOAT fDefaultScale; /* 0x4c */
    INT iCursorLimitX; /* 0x50 */
    INT iCursorLimitY; /* 0x54 */
    CursorPosition vShiftVec; /* 0x58 */
    CursorPosition vShiftPosi; /* 0x60 */
    D2::C2DSeqCtrl* pMaskFG; /* 0x68 */

    void init() {
        pList = NULL;
        vShiftPosi.y = 0.0f;
        pBase = NULL;
        pCursor = NULL;
        pLight = NULL;
        pFrame = NULL;
        pFlicker = NULL;
        pString = NULL;
        pSeqCtrl = NULL;
        pCard = NULL;
        pMaskFG = NULL;
        iStringNum = 0;
        iSeqCtrlNum = 0;
        sCursorPosiX = 0;
        sCursorPosiY = 0;
        iCardNum = 0;
        iCursorLimitX = 0;
        iCursorLimitY = 0;
        Percentage[0] = 0;
        fScale = 0.0f;
        fZoomTimer = 0.0f;
        vShiftVec.x = 0.0f;
        vShiftVec.y = 0.0f;
        vShiftPosi.x = 0.0f;
        fDefaultScale = 1.0f;
    }
};
struct ContentCardWork {
    CMemo2DSequence* pList; /* 0x00 */
    CMemo2DCursor* pCursor; /* 0x04 */
    D2::C2DFnt** pString; /* 0x08 */
    INT iStringNum; /* 0x0c */
    D2::C2DSeqCtrl** pSeqCtrl; /* 0x10 */
    INT iSeqCtrlNum; /* 0x14 */
    D2::C2DSeqCtrl** pNewMarkSeq; /* 0x18 */
    D2::C2DSeqCtrl** pKingMarkSeq; /* 0x1c */
    D2::C2DSeqCtrl** pScrollBar; /* 0x20 */
    INT iCursorCurrent; /* 0x24 */
    INT iListNum; /* 0x28 */
    INT iCursorPosiNum; /* 0x2c */
    FLOAT fCursorIntervalY; /* 0x30 */
    CT_LOWER starCTNo; /* 0x34 */
    BYTE* pCardKindFlagTbl; /* 0x38 */
    BYTE* pCompFlagTbl; /* 0x3c */

    void init() {
        pList = NULL;
        pCursor = NULL;
        pString = NULL;
        pSeqCtrl = NULL;
        pCardKindFlagTbl = NULL;
        pNewMarkSeq = NULL;
        pKingMarkSeq = NULL;
        pCompFlagTbl = NULL;
        pScrollBar = NULL;
        iStringNum = 0;
        iSeqCtrlNum = 0;
        iCursorCurrent = 0;
        iListNum = 0;
        iCursorPosiNum = 0;
        fCursorIntervalY = 0.0f;
        starCTNo = 0;
    }
};
struct ContentCardSelectWork {
    CMemo2DSequence* pList; /* 0x00 */
    CMemo2DCursor* pCursor; /* 0x04 */
    D2::C2DFnt** pString; /* 0x08 */
    INT iStringNum; /* 0x0c */
    D2::C2DSeqCtrl** pSeqCtrl; /* 0x10 */
    INT iSeqCtrlNum; /* 0x14 */
    D2::C2DSeqCtrl** pNewMarkSeq; /* 0x18 */
    CT_LOWER* pCTCodeTbl; /* 0x1c */
    D2::C2DSeqCtrl** pScrollBar; /* 0x20 */
    INT iCursorCurrent; /* 0x24 */
    FLOAT fCursorIntervalY; /* 0x28 */
    INT iKindNo; /* 0x2c */
    INT iListNum; /* 0x30 */
    INT iCursorPosiNum; /* 0x34 */
    INT iEventFlagId; /* 0x38 */
    FLOAT fScrollBarLength; /* 0x3c */
    FLOAT fScrollBarParListNum; /* 0x40 */

    void init() {
        pList = NULL;
        pCursor = NULL;
        pString = NULL;
        pSeqCtrl = NULL;
        pNewMarkSeq = NULL;
        pCTCodeTbl = NULL;
        pScrollBar = NULL;
        iCursorCurrent = 0;
        fCursorIntervalY = 0.0f;
        iStringNum = 0;
        iSeqCtrlNum = 0;
        iKindNo = 0;
        iListNum = 0;
        iCursorPosiNum = 0;
        iEventFlagId = 0;
        fScrollBarLength = 0.0f;
        fScrollBarParListNum = 0.0f;
    }
};
struct ContentCardViewWork {
    CMemo2DSequence* pList; /* 0x00 */
    CMemo2DCard* pCard; /* 0x04 */
    CHAR** pTextLineBuf; /* 0x08 */
    INT iStringNum; /* 0x0c */
    D2::C2DFnt** pString; /* 0x10 */
    INT iSeqCtrlNum; /* 0x14 */
    D2::C2DSeqCtrl** pSeqCtrl; /* 0x18 */
    INT* pListIndex; /* 0x1c */
    D2::C2DSeqCtrl* pTopArrowSeq; /* 0x20 */
    D2::C2DSeqCtrl* pBottomArrowSeq; /* 0x24 */
    INT iCurrentTextLine; /* 0x28 */
    INT iTextLineNum; /* 0x2c */
    INT iKindNo; /* 0x30 */
    INT iCardNo; /* 0x34 */
    INT iKindCardMax; /* 0x38 */
    INT iListNum; /* 0x3c */
    INT iPrevNo; /* 0x40 */
    INT iNextNo; /* 0x44 */

    void init() {
        pList = NULL;
        pCard = NULL;
        pTextLineBuf = NULL;
        pString = NULL;
        pSeqCtrl = NULL;
        pListIndex = NULL;
        pTopArrowSeq = NULL;
        pBottomArrowSeq = NULL;
        iCurrentTextLine = 0;
        iTextLineNum = 0;
        iStringNum = 0;
        iSeqCtrlNum = 0;
        iKindNo = 0;
        iCardNo = 0;
        iKindCardMax = 0;
        iListNum = 0;
        iPrevNo = -1;
        iNextNo = -1;
    }
};
struct ContentCharaWork {
    CMemo2DSequence* pList; /* 0x00 */
    CMemo2DCursor* pCursor; /* 0x04 */
    D2::C2DFnt** pString; /* 0x08 */
    INT iStringNum; /* 0x0c */
    D2::C2DSeqCtrl** pSeqCtrl; /* 0x10 */
    INT iSeqCtrlNum; /* 0x14 */
    D2::C2DSeqCtrl** pScrollBar; /* 0x18 */
    D2::C2DSeqCtrl** pNewMarkSeq; /* 0x1c */
    D2::C2DSeqCtrl** pKingMarkSeq; /* 0x20 */
    INT iCursorCurrent; /* 0x24 */
    FLOAT fCursorIntervalY; /* 0x28 */
    INT iNoteListNum; /* 0x2c */
    INT iCursorPosiNum; /* 0x30 */
    INT iCTCodeOffset; /* 0x34 */
    BYTE* pCardKindFlagTbl; /* 0x38 */
    BYTE* pCompFlagTbl; /* 0x3c */

    void init() {
        pList = NULL;
        pCursor = NULL;
        pString = NULL;
        pSeqCtrl = NULL;
        pCardKindFlagTbl = NULL;
        pNewMarkSeq = NULL;
        pKingMarkSeq = NULL;
        pScrollBar = NULL;
        iCursorCurrent = 0;
        fCursorIntervalY = 0.0f;
        iStringNum = 0;
        iSeqCtrlNum = 0;
        iNoteListNum = 0;
        iCursorPosiNum = 0;
        iCTCodeOffset = 0;
    }
};
struct ContentCharaSelectWork {
    CMemo2DSequence* pList; /* 0x00 */
    CMemo2DCursor* pCursor; /* 0x04 */
    D2::C2DFnt** pString; /* 0x08 */
    INT iStringNum; /* 0x0c */
    D2::C2DSeqCtrl** pSeqCtrl; /* 0x10 */
    INT iSeqCtrlNum; /* 0x14 */
    D2::C2DSeqCtrl** pScrollBar; /* 0x18 */
    D2::C2DSeqCtrl** pNewMarkSeq; /* 0x1c */
    CT_LOWER* pCTCodeTbl; /* 0x20 */
    INT iCursorCurrent; /* 0x24 */
    FLOAT fCursorIntervalY; /* 0x28 */
    INT iKindNo; /* 0x2c */
    INT iListNum; /* 0x30 */
    INT iCursorPosiNum; /* 0x34 */
    INT iEventFlagId; /* 0x38 */
    FLOAT fScrollBarLength; /* 0x3c */
    FLOAT fScrollBarParListNum; /* 0x40 */

    void init() {
        pList = NULL;
        pCursor = NULL;
        pString = NULL;
        pSeqCtrl = NULL;
        pNewMarkSeq = NULL;
        pCTCodeTbl = NULL;
        pScrollBar = NULL;
        iCursorCurrent = 0;
        fCursorIntervalY = 0.0f;
        iStringNum = 0;
        iSeqCtrlNum = 0;
        iKindNo = 0;
        iListNum = 0;
        iCursorPosiNum = 0;
        iEventFlagId = 0;
        fScrollBarLength = 0.0f;
        fScrollBarParListNum = 0.0f;
    }
};
struct ContentCharaViewWork {
    CMemo2DSequence* pList; /* 0x00 */
    CMemo2DCard* pChara; /* 0x04 */
    CHAR** pTextLineBuf; /* 0x08 */
    INT iStringNum; /* 0x0c */
    D2::C2DFnt** pString; /* 0x10 */
    INT iSeqCtrlNum; /* 0x14 */
    D2::C2DSeqCtrl** pSeqCtrl; /* 0x18 */
    D2::C2DSeqCtrl* pTopArrowSeq; /* 0x1c */
    D2::C2DSeqCtrl* pBottomArrowSeq; /* 0x20 */
    CT_LOWER* pCTCodeTbl; /* 0x24 */
    INT iCurrentTextLine; /* 0x28 */
    INT iTextLineNum; /* 0x2c */
    INT iKindNo; /* 0x30 */
    INT iCharaNo; /* 0x34 */
    INT iListNum; /* 0x38 */
    INT iPrevNo; /* 0x3c */
    INT iNextNo; /* 0x40 */
    USHORT iGroupId; /* 0x44 */
    USHORT iRsrcId; /* 0x46 */
    char DefeatEnemyStr[16]; /* 0x48 */

    void init() {
        pList = NULL;
        pChara = NULL;
        pTextLineBuf = NULL;
        pString = NULL;
        pSeqCtrl = NULL;
        pTopArrowSeq = NULL;
        pBottomArrowSeq = NULL;
        pCTCodeTbl = NULL;
        iCurrentTextLine = 0;
        iTextLineNum = 0;
        iStringNum = 0;
        iSeqCtrlNum = 0;
        iKindNo = 0;
        iCharaNo = 0;
        iListNum = 0;
        iPrevNo = -1;
        iGroupId = 0;
        iNextNo = -1;
        iRsrcId = 0;
        for (INT i = 0; i < 16; i++) DefeatEnemyStr[i] = 0;
    }
};
struct ContentCharaModelViewWork {
    HANDLE hObj; /* 0x00 */
    CMemo2DSequence* pDustyList; /* 0x04 */
    INT iStep; /* 0x08 */
    INT iState; /* 0x0c */
    char ScaleStr[64]; /* 0x10 */

    void init() {
        hObj = 0;
        pDustyList = NULL;
        iStep = 70;
        iState = 0;
    }
};
struct ContentMinigameWork {
    CMemo2DSequence* pList; /* 0x00 */
    D2::C2DFnt** pString; /* 0x04 */
    INT iStringNum; /* 0x08 */
    D2::C2DSeqCtrl** pSeqCtrl; /* 0x0c */
    INT iSeqCtrlNum; /* 0x10 */
    char strBGlider[64]; /* 0x14 */
    char strWDive[64]; /* 0x54 */
    char strEBee[64]; /* 0x94 */
    char strTJump[64]; /* 0xd4 */
    char strBPanic[64]; /* 0x114 */

    void init() {
        pList = NULL;
        pString = NULL;
        pSeqCtrl = NULL;
        iStringNum = 0;
        iSeqCtrlNum = 0;
        strBGlider[0] = 0;
        strWDive[0] = 0;
        strEBee[0] = 0;
        strTJump[0] = 0;
        strBPanic[0] = 0;
    }
};
class CJiminyMemo : public CMenu {
private:
    UBYTE _m_0x80[0x10];  /* 0x080 */
    INT m_iStep;          /* 0x090 */
    INT m_iPlayerMode;    /* 0x094 */
    INT m_iContentMode;   /* 0x098 */
    HANDLE m_hLayout;     /* 0x09c */
    UINT m_uiStateFlags;  /* 0x0a0 */
    void (CJiminyMemo::*pContentFunc)(); /* 0x0a4 */
    D2::C2DSeqCtrl* m_pMessSeq; /* 0x0ac */
    D2::C2DFnt* m_pMessString;  /* 0x0b0 */
    CHAR* m_pLadName;     /* 0x0b4 */
    CHAR* m_pLadFileName; /* 0x0b8 */
    ContentCommonWork m_ContentCommon;              /* 0x0bc */
    ContentMenuWork m_ContentMenu;                 /* 0x0e4 */
    ContentStoryWork m_ContentStory;               /* 0x114 */
    ContentStoryViewWork m_ContentStoryView;       /* 0x144 */
    ContentCollectWork m_ContentCollect;           /* 0x1ac */
    ContentCardWork m_ContentCard;                 /* 0x218 */
    ContentCardSelectWork m_ContentCardSelect;     /* 0x258 */
    ContentCardViewWork m_ContentCardView;         /* 0x29c */
    ContentCharaWork m_ContentChara;               /* 0x2e4 */
    ContentCharaSelectWork m_ContentCharaSelect;   /* 0x324 */
    ContentCharaViewWork m_ContentCharaView;       /* 0x368 */
    ContentCharaModelViewWork m_ContentCharaModelView; /* 0x3c0 */
    ContentMinigameWork m_ContentMinigame;         /* 0x410 */
    UBYTE _m_0x564[0xc];                           /* 0x564 */

    INT& work(INT inOfs) { return *(INT*)((UBYTE*)&m_ContentCommon + (inOfs - 0xbc)); }
    BOOL isModeJiminyMemo() { return m_iPlayerMode == 0; }
    BOOL isModeDReport() { return m_iPlayerMode != 0; }
    INT getLoadResourceIndex() { return m_iPlayerMode != 0; }
    void setContentStep(INT inStep) { m_ContentCommon.iStep = inStep; }
    INT getContentStep() { return m_ContentCommon.iStep; }
    INT getNextContentMode() { return m_ContentCommon.iNextContent; }
    void setContentParam(INT inIdx, INT inParam) { m_ContentCommon.param[inIdx] = inParam; }
    void setContentTimer(FLOAT inTimer) { m_ContentCommon.fTimer = inTimer; }
    FLOAT getContentTimer() { return m_ContentCommon.fTimer; }
    INT getContentParam(INT inIdx) { return m_ContentCommon.param[inIdx]; }
    INT getPrevContentMode() { return m_ContentCommon.iPrevContent; }
    void setNextContentMode(INT inMode) { m_ContentCommon.iNextContent = inMode; }
    void setContentScroll(INT inIdx, const FVECTOR& inPosi) {
        m_ContentCommon.scroll[inIdx].x = inPosi.x;
        m_ContentCommon.scroll[inIdx].y = inPosi.y;
    }
    void getContentScroll(INT inIdx, FVECTOR& outPosi) {
        outPosi.x = m_ContentCommon.scroll[inIdx].x;
        outPosi.y = m_ContentCommon.scroll[inIdx].y;
        outPosi.z = 0.0f;
        outPosi.w = 1.0f;
    }

public:
    CJiminyMemo(CTask* inParent, INT inMode);
    virtual ~CJiminyMemo();

private:
    void initialize(INT inPlayerMode);
    void finalize();
    virtual void main();
    void mainInit();
    void mainLoadWait();
    void mainIn();
    void mainInWait();
    void mainLoop();
    void mainOut();
    void mainOutWait();
    void setContentMode(INT inContentMode);
    void replaceNoteList(D2::C2DFnt** inFnt, const CHAR** inString, INT inNum);
    void replaceNoteListMark(D2::C2DSeqCtrl** inMark, BYTE* inFlagTbl, INT inListNum, INT inListAllNum, INT inStartNo,
                             INT inLayoutNo, INT inLayoutSeqNo);
    void replaceNoteListMark(D2::C2DSeqCtrl** inMark, INT inFlagId, INT inListNum, INT inListAllNum, INT inStartNo,
                             INT inLayoutNo, INT inLayoutSeqNo);
    void getMenuContentFlag(INT inFlagId, INT inNum, INT* inFlagWork, BOOL* inCompWork);
    void setPlateState(INT inKind, INT inState);
    void setPlateNumSeq(INT inKind, INT inParam);
    CMemo2DCursor* createCollectionCursorSequence(const INT* inSeqTbl, const CursorPosition& inStartPosi,
                                                  const CursorPosition& inStepPosi, INT inStepNum, INT inListNum,
                                                  INT inOffset, INT inStart, INT inPosiWidth, INT inLayer,
                                                  INT inState, INT inPosiId, FLOAT inScale, CMemo2DSequence* inParent);
    CMemo2DCursor* createCursorSequence(const INT* inSeqTbl, INT inLayoutNo, INT inStartSeq,
                                        const CursorPosition& inPosiOffset, INT inStepNum, INT inListNum, INT inOffset,
                                        INT inStart, INT inWidth, INT inLayer, INT inState, INT inPosiId,
                                        CMemo2DSequence* inParent);
    INT splitMessage(INT* outNum, CHAR*** outBuf, const CHAR* inStr, INT inWidth);
    void CardScreenOutClip();
    void CardScreenSideClip(BOOL inAllDisp);
    void ScrollBarInit(D2::C2DSeqCtrl** inSeqCtrl, INT inLayer, FVECTOR& inPos, FLOAT inBaseHeight,
                       CMemo2DSequence* inParent);
    void ScrollBarChange(D2::C2DSeqCtrl** inSeqCtrl, FLOAT inBasePosY, FLOAT inBaseHeight, INT inCursorPosiNum,
                         INT inNoteListNum, INT inStartNum);
    INT getPlateState(INT inKind);
    void setPlateText(INT inKind, INT inCTNo);
    CMemo2DSequence* createSequence(const INT* inSeqData, INT inLayer, INT inLayoutNo, INT inLayoutSeqNo,
                                    CMemo2DSequence* inParent);
    CMemo2DSequence* createSequence(INT inSeqNo, INT inLayer, INT inLayoutNo, INT inLayoutSeqNo,
                                    CMemo2DSequence* inParent);
    void createMarkSequence(INT inMarkSeq, INT inNum, INT inLayoutNo, INT inLayoutSeqOffset, INT inLayer,
                            D2::C2DSeqCtrl* inParent, D2::C2DSeqCtrl** inArray);
    INT createListSequence(CMemo2DSequence* inList, INT inLayer, INT inLayoutNo, INT inLayoutSeqNo,
                           D2::C2DSeqCtrl** inFontSeq, D2::C2DFnt** inFont, UINT inFontCol, INT inFontSize,
                           INT inFontStyle, const CHAR* inStr);
    INT createListSequenceTypewriterFont(CMemo2DSequence* inList, INT inLayer, INT inLayoutNo, INT inLayoutSeqNo,
                                         D2::C2DSeqCtrl** inFontSeq, D2::C2DFnt** inFont, UINT inFontCol,
                                         INT inFontSize, INT inFontStyle, const CHAR* inStr, FLOAT inWidth);
    void replaceNoteListString(D2::C2DFnt* inList, const CHAR* inString) {
        if (inList) inList->setText(inString);
    }
    void replaceNoteListWithOpenCheck(D2::C2DFnt** inList, CT_LOWER inStartCtNo, INT inFlagId, INT inListNum,
                                      INT inListAllNum, INT inStartNo);
    void replaceNoteListWithOpenCheck(D2::C2DFnt** inList, CT_LOWER inStartCtNo, BYTE* inFlagTbl, INT inListNum,
                                      INT inListAllNum, INT inStartNo);
    void replaceNoteListWithOpenCheck(D2::C2DFnt** inList, CT_LOWER* inCtTbl, INT inFlagId, INT inListNum,
                                      INT inListAllNum, INT inStartNo);
    BOOL isButtonRptLimitStop(UINT inBit, UINT inPad, BOOL inStopCompare);
    void createCursorPosiTbl(CursorPosition* inPosiTbl, const CursorPosition& inStartPosi, const CursorPosition& inStepPosi,
                             INT inListNum, INT inWidth);
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
    BOOL nextPage(CMemo2DCursor* pCursor, INT* pCursorCurrent, INT iNoteListNum, INT iCursorPosiNum,
                  FLOAT fCursorIntervalY);
    BOOL prevPage(CMemo2DCursor* pCursor, INT* pCursorCurrent, INT iNoteListNum, INT iCursorPosiNum,
                  FLOAT fCursorIntervalY);
    BOOL haveSpecialCard();
    INT setParamFromCollectToCardView();
    INT setParamFromCardViewToCollect();
    INT getNextCardId(INT inId);
    INT getPrevCardId(INT inId);
    INT getNextCharaId(INT inId);
    INT getPrevCharaId(INT inId);
    INT getNextStoryId(INT inId);
    INT getPrevStoryId(INT inId);
    void contentMenu();
    void contentStory();
    void contentStoryView();
    void contentCollection();
    void contentCard();
    void contentCardSelect();
    void contentCardView();
    void contentChara();
    void contentCharaSelect();
    void contentCharaView();
    INT contentCharaView_ModelView();
    void contentMinigame();
    static void (CJiminyMemo::*m_pContentTbl[11])();
};
typedef char _check_jm_size[sizeof(CJiminyMemo) == 0x570 ? 1 : -1];

struct ILLUST_TEX_DATA {
    BYTE No;
    BYTE Posi;
};
struct MODEL_VIEW_DATA {
    USHORT ResourceNo;
    USHORT ScaleFix8;
    SHORT PosiYFix8;
    SHORT RotYDeg;
};
/* .data (in this order): DefaultMessage and the tables of pointers to the .rodata tables at the end */
namespace JiminyMemo {
namespace Base {
static CHAR* DefaultMessage = "";
}
extern SHORT CardAttackTbl[];
extern SHORT CardMagicTbl[];
extern SHORT CardSummonsTbl[];
extern SHORT CardItemTbl[];
extern SHORT CardFriendTbl[];
extern SHORT CardEnemyTbl[];
extern SHORT CardMapTbl[];
extern SHORT CardWorldTbl[];
extern SHORT CardGimmickTbl[];
extern SHORT CardSpecialTbl[];
extern SHORT RikuCardBattleTbl[];
extern SHORT RikuCardEnemyTbl[];
extern SHORT RikuCardMapTbl[];
extern SHORT RikuCardWorldTbl[];
extern CT_LOWER CardNameAttackTbl[];
extern CT_LOWER CardNameMagicTbl[];
extern CT_LOWER CardNameSummonsTbl[];
extern CT_LOWER CardNameItemTbl[];
extern CT_LOWER CardNameFriendTbl[];
extern CT_LOWER CardNameEnemyTbl[];
extern CT_LOWER CardNameMapTbl[];
extern CT_LOWER CardNameWorldTbl[];
extern CT_LOWER CardNameGimmickTbl[];
extern CT_LOWER CardNameSpecialTbl[];
extern CT_LOWER RikuCardNameBattleTbl[];
extern CT_LOWER RikuCardNameEnemyTbl[];
extern CT_LOWER RikuCardNameMapTbl[];
extern CT_LOWER RikuCardNameWorldTbl[];
extern CT_LOWER CardManualAttackTbl[];
extern CT_LOWER CardManualMagicTbl[];
extern CT_LOWER CardManualSummonsTbl[];
extern CT_LOWER CardManualItemTbl[];
extern CT_LOWER CardManualFriendTbl[];
extern CT_LOWER CardManualEnemyTbl[];
extern CT_LOWER CardManualMapTbl[];
extern CT_LOWER CardManualWorldTbl[];
extern CT_LOWER CardManualGimmickTbl[];
extern CT_LOWER CardManualSpecialTbl[];
extern CT_LOWER RikuCardManualBattleTbl[];
extern CT_LOWER RikuCardManualEnemyTbl[];
extern CT_LOWER RikuCardManualMapTbl[];
extern CT_LOWER RikuCardManualWorldTbl[];
extern ILLUST_TEX_DATA CharaIllustMain1Tbl[];
extern ILLUST_TEX_DATA CharaIllustMain2Tbl[];
extern ILLUST_TEX_DATA CharaIllustEtcTbl[];
extern ILLUST_TEX_DATA CharaIllustTRTbl[];
extern ILLUST_TEX_DATA CharaIllustWOTbl[];
extern ILLUST_TEX_DATA CharaIllustOLTbl[];
extern ILLUST_TEX_DATA CharaIllustAGTbl[];
extern ILLUST_TEX_DATA CharaIllustHATbl[];
extern ILLUST_TEX_DATA CharaIllustMOTbl[];
extern ILLUST_TEX_DATA CharaIllustATTbl[];
extern ILLUST_TEX_DATA CharaIllustNETbl[];
extern ILLUST_TEX_DATA CharaIllustHOTbl[];
extern ILLUST_TEX_DATA CharaIllustPOTbl[];
extern ILLUST_TEX_DATA CharaIllustDETbl[];
extern ILLUST_TEX_DATA CharaIllustHeartLessTbl[];
extern ILLUST_TEX_DATA CharaIllustRiku1Tbl[];
extern ILLUST_TEX_DATA CharaIllustRiku2Tbl[];
extern ILLUST_TEX_DATA CharaIllustRikuHeartLessTbl[];
extern CT_LOWER CharaNameMain1Tbl[];
extern CT_LOWER CharaNameMain2Tbl[];
extern CT_LOWER CharaNameEtcTbl[];
extern CT_LOWER CharaNameTRTbl[];
extern CT_LOWER CharaNameWOTbl[];
extern CT_LOWER CharaNameOLTbl[];
extern CT_LOWER CharaNameAGTbl[];
extern CT_LOWER CharaNameHATbl[];
extern CT_LOWER CharaNameMOTbl[];
extern CT_LOWER CharaNameATTbl[];
extern CT_LOWER CharaNameNETbl[];
extern CT_LOWER CharaNameHOTbl[];
extern CT_LOWER CharaNamePOTbl[];
extern CT_LOWER CharaNameDETbl[];
extern CT_LOWER CharaNameHeartLessTbl[];
extern CT_LOWER CharaNameRiku1Tbl[];
extern CT_LOWER CharaNameRiku2Tbl[];
extern CT_LOWER CharaNameRikuHeartLessTbl[];
extern CT_LOWER CharaManualMain1Tbl[];
extern CT_LOWER CharaManualMain2Tbl[];
extern CT_LOWER CharaManualEtcTbl[];
extern CT_LOWER CharaManualTRTbl[];
extern CT_LOWER CharaManualWOTbl[];
extern CT_LOWER CharaManualOLTbl[];
extern CT_LOWER CharaManualAGTbl[];
extern CT_LOWER CharaManualHATbl[];
extern CT_LOWER CharaManualMOTbl[];
extern CT_LOWER CharaManualATTbl[];
extern CT_LOWER CharaManualNETbl[];
extern CT_LOWER CharaManualHOTbl[];
extern CT_LOWER CharaManualPOTbl[];
extern CT_LOWER CharaManualDETbl[];
extern CT_LOWER CharaManualHeartLessTbl[];
extern CT_LOWER CharaManualRiku1Tbl[];
extern CT_LOWER CharaManualRiku2Tbl[];
extern CT_LOWER CharaManualRikuHeartLessTbl[];
extern MODEL_VIEW_DATA CharaModelMain1Tbl[];
extern MODEL_VIEW_DATA CharaModelMain2Tbl[];
extern MODEL_VIEW_DATA CharaModelEtcTbl[];
extern MODEL_VIEW_DATA CharaModelTRTbl[];
extern MODEL_VIEW_DATA CharaModelWOTbl[];
extern MODEL_VIEW_DATA CharaModelOLTbl[];
extern MODEL_VIEW_DATA CharaModelAGTbl[];
extern MODEL_VIEW_DATA CharaModelHATbl[];
extern MODEL_VIEW_DATA CharaModelMOTbl[];
extern MODEL_VIEW_DATA CharaModelATTbl[];
extern MODEL_VIEW_DATA CharaModelNETbl[];
extern MODEL_VIEW_DATA CharaModelHOTbl[];
extern MODEL_VIEW_DATA CharaModelPOTbl[];
extern MODEL_VIEW_DATA CharaModelDETbl[];
extern MODEL_VIEW_DATA CharaModelHeartLessTbl[];
extern MODEL_VIEW_DATA CharaModelRiku1Tbl[];
extern MODEL_VIEW_DATA CharaModelRiku2Tbl[];
extern MODEL_VIEW_DATA CharaModelRikuHeartLessTbl[];
SHORT* CardEncyclTbl[10] = { CardAttackTbl, CardMagicTbl, CardSummonsTbl, CardItemTbl, CardFriendTbl, CardEnemyTbl, CardMapTbl, CardWorldTbl, CardGimmickTbl, CardSpecialTbl };
SHORT* RikuCardEncyclTbl[4] = { RikuCardBattleTbl, RikuCardEnemyTbl, RikuCardMapTbl, RikuCardWorldTbl };
CT_LOWER* CardNameTbl[10] = { CardNameAttackTbl, CardNameMagicTbl, CardNameSummonsTbl, CardNameItemTbl, CardNameFriendTbl, CardNameEnemyTbl, CardNameMapTbl, CardNameWorldTbl, CardNameGimmickTbl, CardNameSpecialTbl };
CT_LOWER* RikuCardNameTbl[4] = { RikuCardNameBattleTbl, RikuCardNameEnemyTbl, RikuCardNameMapTbl, RikuCardNameWorldTbl };
CT_LOWER* CardManualTbl[10] = { CardManualAttackTbl, CardManualMagicTbl, CardManualSummonsTbl, CardManualItemTbl, CardManualFriendTbl, CardManualEnemyTbl, CardManualMapTbl, CardManualWorldTbl, CardManualGimmickTbl, CardManualSpecialTbl };
CT_LOWER* RikuCardManualTbl[4] = { RikuCardManualBattleTbl, RikuCardManualEnemyTbl, RikuCardManualMapTbl, RikuCardManualWorldTbl };
ILLUST_TEX_DATA* CharaIllustTextureTbl[15] = { CharaIllustMain1Tbl, CharaIllustMain2Tbl, CharaIllustEtcTbl, CharaIllustTRTbl, CharaIllustWOTbl, CharaIllustOLTbl, CharaIllustAGTbl, CharaIllustHATbl, CharaIllustMOTbl, CharaIllustATTbl, CharaIllustNETbl, CharaIllustHOTbl, CharaIllustPOTbl, CharaIllustDETbl, CharaIllustHeartLessTbl };
ILLUST_TEX_DATA* RikuCharaIllustTextureTbl[3] = { CharaIllustRiku1Tbl, CharaIllustRiku2Tbl, CharaIllustRikuHeartLessTbl };
CT_LOWER* CharacterNameTbl[15] = { CharaNameMain1Tbl, CharaNameMain2Tbl, CharaNameEtcTbl, CharaNameTRTbl, CharaNameWOTbl, CharaNameOLTbl, CharaNameAGTbl, CharaNameHATbl, CharaNameMOTbl, CharaNameATTbl, CharaNameNETbl, CharaNameHOTbl, CharaNamePOTbl, CharaNameDETbl, CharaNameHeartLessTbl };
CT_LOWER* RikuCharacterNameTbl[3] = { CharaNameRiku1Tbl, CharaNameRiku2Tbl, CharaNameRikuHeartLessTbl };
CT_LOWER* CharacterManualTbl[15] = { CharaManualMain1Tbl, CharaManualMain2Tbl, CharaManualEtcTbl, CharaManualTRTbl, CharaManualWOTbl, CharaManualOLTbl, CharaManualAGTbl, CharaManualHATbl, CharaManualMOTbl, CharaManualATTbl, CharaManualNETbl, CharaManualHOTbl, CharaManualPOTbl, CharaManualDETbl, CharaManualHeartLessTbl };
CT_LOWER* RikuCharacterManualTbl[3] = { CharaManualRiku1Tbl, CharaManualRiku2Tbl, CharaManualRikuHeartLessTbl };
MODEL_VIEW_DATA* CharaModelResourceTbl[15] = { CharaModelMain1Tbl, CharaModelMain2Tbl, CharaModelEtcTbl, CharaModelTRTbl, CharaModelWOTbl, CharaModelOLTbl, CharaModelAGTbl, CharaModelHATbl, CharaModelMOTbl, CharaModelATTbl, CharaModelNETbl, CharaModelHOTbl, CharaModelPOTbl, CharaModelDETbl, CharaModelHeartLessTbl };
MODEL_VIEW_DATA* RikuCharaModelResourceTbl[3] = { CharaModelRiku1Tbl, CharaModelRiku2Tbl, CharaModelRikuHeartLessTbl };
}
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
CHAR* JiminyMemo::getOpenCloseStringData(INT inCTNo, BOOL inIsFlag) {
    CHAR* rval;
    if (inIsFlag) {
        rval = CCtdMan::getText(inCTNo);
    } else {
        static CHAR* pDefaultStr = "\x81\x96\x81\x96\x81\x96\x81\x96\x81\x96\x81\x96\x81\x96\x81\x96\x81\x96\x81\x96";
        CHAR* pCloseStr = CCtdMan::getText(0x700003);
        if (!pCloseStr) pCloseStr = pDefaultStr;
        INT closeLen = strlen(pCloseStr);
        if (!closeLen) {
            pCloseStr = pDefaultStr;
            closeLen = strlen(pCloseStr);
        }
        CHAR* pMessStr = CCtdMan::getText(inCTNo);
        if (!pMessStr) {
            rval = pCloseStr + (closeLen - 2);
        } else {
            INT messLen = strlen(pMessStr);
            if (!messLen) {
                rval = pCloseStr + (closeLen - 4);
            } else if (closeLen < messLen) {
                rval = pCloseStr;
            } else {
                rval = pCloseStr + (closeLen - messLen);
            }
        }
    }
    return rval;
}
void (CJiminyMemo::*CJiminyMemo::m_pContentTbl[11])() = {
    &CJiminyMemo::contentMenu,
    &CJiminyMemo::contentStory,
    &CJiminyMemo::contentCollection,
    &CJiminyMemo::contentCard,
    &CJiminyMemo::contentChara,
    &CJiminyMemo::contentMinigame,
    &CJiminyMemo::contentStoryView,
    &CJiminyMemo::contentCardSelect,
    &CJiminyMemo::contentCardView,
    &CJiminyMemo::contentCharaSelect,
    &CJiminyMemo::contentCharaView
};
BOOL JiminyMemo::is100AcreGoodEnding() {
    return CGameWork::getInt(0, 0x23) & 0x40;
}
CMemo3DObj::CMemo3DObj(CTask* p, INT inGroupId, INT inRsrcId, UINT inDispLayer, BOOL inLoadEnable)
    : CTask(p, 0x4000), m_pModels(NULL), m_iModelCount(0), m_pAnime(NULL), m_pTexAnim(NULL), m_iTexAnimNum(0),
      m_iTexAnimNo(0), m_iTexAnimSeqNo(0), m_pLight(NULL), m_iStep(1), m_iGroupId(inGroupId), m_iResourceId(inRsrcId),
      m_fFadeCount(0.0f), m_fFadeCountMax(0.0f), m_bEraseModel(FALSE), m_bTouch(FALSE), m_fBaseScale(1.0f),
      m_fScale(1.0f), m_uiDispLayer(inDispLayer), m_bLoadEnable(inLoadEnable), m_hEffect(0), m_bLightOff(FALSE),
      m_bNotSemiFlag(FALSE), m_bFade2PathFlag(FALSE) {
    m_fRotY = 3.1415927f;
    m_vPosition.init();
    m_vBasePosition.init();
    if (isLoadEnable()) CRcf::reqLoadRCFFront(m_iGroupId, m_iResourceId, 0, NULL, 0);
    m_iStep = 0;
    m_fFadeCountMax = 6.0f;
}
/* Parked: final group, see the note above getTaskPointer<T> at the end of the file (matches when the group is C) */
#if 0
CMemo3DObj::~CMemo3DObj() {
    if (m_pModels) delete[] m_pModels;
    m_pModels = NULL;
    if (m_pAnime) delete m_pAnime;
    m_pAnime = NULL;
    if (m_pLight) delete m_pLight;
    m_pLight = NULL;
    if (m_pTexAnim) delete[] m_pTexAnim;
    m_pTexAnim = NULL;
    if (isLoadEnable()) {
        if (m_iStep == 0 && !CRcf::isCompleteLoadRCF(m_iGroupId, m_iResourceId)) {
            CRcf::cancelLoadRCF(m_iGroupId, m_iResourceId);
        } else {
            CRsrcDB::removeGroup(m_iGroupId, m_iResourceId);
        }
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", _$_10CMemo3DObj);
#endif
__asm__(".rdata\n"); /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
void CMemo3DObj::main() {
    INT nID;
    MODELDATA** pModelTbl;
    TEXANM_DATA* pTexAnimData;
    switch (m_iStep) {
    case 0:
        if (!CRcf::isCompleteLoadRCF(m_iGroupId, m_iResourceId)) break;
    case 1:
        nID = m_iGroupId * 10000 + m_iResourceId;
        m_iModelCount = CRsrcDB::getModelCount(nID);
        if (m_iModelCount > 0) {
            pModelTbl = CRsrcDB::getRsrcMDL(nID);
            if (pModelTbl) {
#line 2485
                m_pModels = new (__FILE__ "(2485)") CModel[m_iModelCount];
#line 2487
                if (!m_pModels) AssertApp(__FILE__, __LINE__, "m_pModels", 0);
                pTexAnimData = CRsrcDB::getRsrcTXA(nID);
                if (pTexAnimData) {
                    m_iTexAnimNum = pTexAnimData->getDataNum();
                    if (m_iTexAnimNum > 0) {
                        m_pTexAnim = new (__FILE__ "(2501)") CTexAnm[m_iTexAnimNum];
                        for (INT i = 0; i < m_iTexAnimNum; i++) m_pTexAnim[i].setup(*pTexAnimData, i);
                    }
                }
                for (INT i = 0; i < m_iModelCount; i++) {
                    m_pModels[i].link(this, pModelTbl[i]);
                    if (m_bFade2PathFlag) m_pModels[i].m_nFlag |= 0x20;
                    if (m_bNotSemiFlag) m_pModels[i].m_nFlag |= 0x800;
                    if (isLightOff()) m_pModels[i].setColorScale(0.0f, 0.0f, 0.0f, 0.0f);
                    if (m_pTexAnim) m_pModels[i].linkTexAnm(m_pTexAnim, m_iTexAnimNum);
                }
                m_pAnime = new (__FILE__ "(2525)") CAnim;
#line 2527
                if (!m_pAnime) AssertApp(__FILE__, __LINE__, "m_pAnime", 0);
                m_pAnime->link(this, nID);
                m_pAnime->init(1, 0);
                for (INT i = 0; i < m_iModelCount; i++) m_pModels[i].changeLayerExt(1, m_uiDispLayer);
                for (INT i = 0; i < m_iModelCount; i++) {
                    m_pModels[i].setSemiTrans(ALPHA_BLEND);
                    m_pModels[i].setAlphaScale(m_fFadeCount / m_fFadeCountMax);
                }
                if (m_iGroupId == 10 && m_iResourceId == 2030) {
                    CEffectMan::setLastDraw(TRUE);
                    for (INT i = 0; i < m_iModelCount; i++) {
                        INT headId = pModelTbl[i]->getBoneIndex("head");
                        if (headId >= 0) {
                            FVECTOR pos;
                            FVECTOR rot;
                            pos.set(0.25f, 0.0f, 0.0f);
                            rot.set(0.0f, DEG2RAD(270.0f), 0.0f);
                            Math3D::LimitPIVector(rot);
                            m_hEffect = CEffectMan::attachEffect("n2030", 0, ObjType::OBJ_TYPE_2D, &Crd(headId), &pos,
                                                                 &rot, 0.0f);
                            break;
                        }
                    }
                }
                if (m_pTexAnim && m_iTexAnimNum > 0) m_pTexAnim->setSeq("EYE_BLINK");
                m_iStep = 2;
                break;
            }
        }
        m_iStep = 0xFFFF;
        break;
    case 2:
        m_vBasePosition.x = 0.0f;
        m_vBasePosition.y = -1.0f;
        m_vBasePosition.z = 4.0f;
        m_fBaseScale = 1.0f;
        calcLocalMatrix();
        m_iStep = 3;
    case 3:
        m_fFadeCount += getFrameRateF();
        if (m_fFadeCount >= m_fFadeCountMax) {
            m_fFadeCount = m_fFadeCountMax;
            for (INT i = 0; i < m_iModelCount; i++) {
                m_pModels[i].setAlphaScale(1.0f);
                m_pModels[i].setSemiTrans(ALPHA_OFF);
            }
            m_iStep = 4;
        } else {
            for (INT i = 0; i < m_iModelCount; i++) m_pModels[i].setAlphaScale(m_fFadeCount / m_fFadeCountMax);
        }
        break;
    case 4:
        if (m_bEraseModel) {
            m_iStep = 5;
            for (INT i = 0; i < m_iModelCount; i++) m_pModels[i].setSemiTrans(ALPHA_BLEND);
            if (m_hEffect) CEffectMan::getEffect(m_hEffect)->fadeout(8.0f);
            m_fFadeCount = m_fFadeCountMax;
        } else {
            if (m_bTouch) {
                m_bTouch = FALSE;
                calcLocalMatrix();
            }
            break;
        }
    case 5:
        m_fFadeCount -= getFrameRateF();
        if (m_fFadeCount <= 0.0f) {
            m_fFadeCount = 0.0f;
            m_iStep = 0xFFFF;
        } else {
            for (INT i = 0; i < m_iModelCount; i++) m_pModels[i].setAlphaScale(m_fFadeCount / m_fFadeCountMax);
        }
        break;
    case 0xFFFF:
    default:
        out();
        CEffectMan::setLastDraw(FALSE);
        break;
    }
    if (m_iStep < 0xFFFF) {
        if (m_pAnime) m_pAnime->play(getFrameRate(), 0);
        if (m_pTexAnim) {
            for (INT i = 0; i < m_iTexAnimNum; i++) m_pTexAnim[i].update(getFrameRateF());
        }
    }
}
void CMemo3DObj::disp() {
    if (!m_pModels) return;
    if (m_iStep >= 0xFFFF) return;
    for (INT i = 0; i < m_iModelCount; i++) {
        m_pModels[i].disp();
    }
}
void MulMatrix(FMATRIX& m0, const FMATRIX& m1, const FMATRIX& m2);
void scaleMatrixSelf(FMATRIX& m, FLOAT fScale);
extern "C" void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
static inline void _unitMatrixVu0(FMATRIX& m) {
    asm volatile("vmr32.xyzw $vf1, $vf0\n"
                 "sqc2 $vf0, 0x30(%0)\n"
                 "vmr32.xyzw $vf2, $vf1\n"
                 "sqc2 $vf1, 0x20(%0)\n"
                 "vmr32.xyzw $vf3, $vf2\n"
                 "sqc2 $vf2, 0x10(%0)\n"
                 "sqc2 $vf3, 0x0(%0)\n"
                 :
                 : "r"(&m)
                 : "memory");
}
void CMemo3DObj::calcLocalMatrix() {
    CCoord& crd = Crd(0);
    FVECTOR posi = m_vBasePosition;
    FLOAT rate = m_vBasePosition.z / 3.0f;
    posi.x += m_vPosition.x * -rate;
    posi.y += m_vPosition.y * rate;
    _unitMatrixVu0(crd.getLocalMatrix());
    sceVu0RotMatrixY(crd.getLocalMatrix(), crd.getLocalMatrix(), m_fRotY);
    crd.getLocalMatrix().setTrans(posi);
    CCamera* pCamera = CWorldEnv::getActiveCamera();
    if (pCamera && pCamera->getCoordNum() > 0) {
        MulMatrix(crd.getLocalMatrix(), pCamera->Crd(0).getLocalMatrix(), crd.getLocalMatrix());
    }
    scaleMatrixSelf(crd.getLocalMatrix(), m_fBaseScale * m_fScale);
    crd.touch();
}
void CMemo3DObj::setRotateY(FLOAT inRotY) {
    if (__builtin_fabsf(Math3D::LimitPI(m_fRotY - inRotY)) != 0.0f) {
        m_fRotY = inRotY;
        m_bTouch = TRUE;
    }
}
void CMemo3DObj::setPosition(const FVECTOR& inPosition) {
    if (!Math3D::EqualVector(m_vPosition, inPosition, 1e-05f)) {
        m_vPosition.set(inPosition.x, inPosition.y, inPosition.z);
        m_bTouch = TRUE;
    }
}
CMemo2DLayout::CMemo2DLayout(CTask* p) : CTask(p, 0x4000) {
    setState(0);
    m_uiStateFlags = 0;
    m_hLayer = 0;
    m_iSeqCtrlNum = 0;
    for (INT i = 0; i < 32; i++) {
        m_pSeqCtrlArray[i] = NULL;
        m_pSeqFntArray[i] = NULL;
    }
    m_iLayerLevel = 11;
    m_iSeqNum = 0;
    m_pLayoutNoTbl = NULL;
    m_pSeqNoTbl = NULL;
    m_pSeqCodeTbl = NULL;
    m_pLayoutName = NULL;
    m_pLayoutFileName = NULL;
}
/* Parked: final group, see the note above getTaskPointer<T> at the end of the file (matches when the group is C) */
#if 0
CMemo2DLayout::~CMemo2DLayout() {
    finalize();
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", _$_13CMemo2DLayout);
#endif
void CMemo2DLayout::main() {
    D2::C2DSeqCtrl* pSeq;
    switch (getState()) {
    case 0:
        break;
    case 1:
        createLayer(0);
        initialize();
        setState(2);
        break;
    case 2:
        if (isEndLayer()) {
            destroyLayer();
            createLayer(1);
            setStateFlag(4, 0);
            setState(3);
        }
        break;
    case 3:
        break;
    case 4:
        destroyLayer();
        createLayer(2);
        setState(5);
        break;
    case 5:
        if (isEndLayer()) {
            destroyLayer();
            setStateFlag(0x10, 0);
            setState(6);
        }
        break;
    case 6:
        break;
    case 7:
        out();
        break;
    default:
        out();
        break;
    }
    for (INT i = 0; i < m_iSeqNum; i++) {
        pSeq = getSeqCtrl(i);
        if (pSeq && pSeq->isEnableDisp() && m_pSeqCodeTbl) {
            if (m_pSeqCodeTbl[i] == m_pSeqNoTbl[i].In && pSeq->isEnd(0)) {
                setSeqState(i, 1);
            } else if (m_pSeqCodeTbl[i] == m_pSeqNoTbl[i].Out && pSeq->isEnd(0)) {
                setSeqState(i, -1);
            }
        }
    }
}
INT CMemo2DLayout::setup(const CHAR* inSeqName, const CHAR* inFileName, const INT* inLayNoTbl, INT inSeqNum,
                         const LayoutDataParam* inSeqNoTbl, INT inLayerLevel) {
    INT rval = FALSE;
    m_pLayoutFileName = (LPCTSTR)inFileName;
    m_pLayoutName = (LPCTSTR)inSeqName;
    m_pLayoutNoTbl = (INT*)com_malloc_d(sizeof(INT) * 3, __FILE__ "(2877)", -1);
    for (INT i = 0; i < 3; i++) m_pLayoutNoTbl[i] = inLayNoTbl[i];
    m_iSeqNum = inSeqNum;
    size_t size = sizeof(LayoutDataParam) * inSeqNum;
    m_pSeqNoTbl = (LayoutDataParam*)com_malloc_d(size, __FILE__ "(2881)",
                                                 ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; }));
    if (m_pSeqNoTbl) {
        for (INT i = 0; i < m_iSeqNum; i++) {
            m_pSeqNoTbl[i].In = inSeqNoTbl[i].In;
            m_pSeqNoTbl[i].Loop = inSeqNoTbl[i].Loop;
            m_pSeqNoTbl[i].Out = inSeqNoTbl[i].Out;
            m_pSeqNoTbl[i].XPosi = inSeqNoTbl[i].XPosi;
            m_pSeqNoTbl[i].YPosi = inSeqNoTbl[i].YPosi;
            m_pSeqNoTbl[i].Layer = inSeqNoTbl[i].Layer;
            m_pSeqNoTbl[i].CTCode = inSeqNoTbl[i].CTCode;
        }
        rval = TRUE;
    }
    m_pSeqCodeTbl = (INT*)com_malloc_d(sizeof(INT) * m_iSeqNum, __FILE__ "(2894)", -1);
    setStateFlag(0x20, 0);
    m_iLayerLevel = inLayerLevel;
    return rval;
}
void CMemo2DLayout::init() {
    if (m_uiStateFlags & 0x20) initialize();
}
void CMemo2DLayout::initialize() {
    FVECTOR pos;
    D2::C2DSeqCtrl* pSeq;
    D2::C2DSeqCtrl* pSeqParent;
    if (isStateFlag(1)) return;
    setStateFlag(1, 0);
    pos.init();
    pSeqParent = NULL;
    for (INT i = 0; i < m_iSeqNum; i++) {
        if (m_pSeqNoTbl[i].CTCode == 0) {
            pSeq = createSequence(i, m_pSeqNoTbl[i].Layer);
            pSeq->addSeq(m_pLayoutName, m_pSeqNoTbl[i].In);
            pos.x = m_pSeqNoTbl[i].XPosi;
            pos.y = m_pSeqNoTbl[i].YPosi;
            pSeq->setPos(pos);
            pSeq->enableDisp(FALSE);
            pSeq->enableColorCtrl(TRUE);
            pSeqParent = pSeq;
        } else if (pSeqParent) {
            D2::C2DFnt* pFnt = new (com_malloc_d(sizeof(D2::C2DFnt), __FILE__ "(2945)",
                                                 ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })))
                D2::C2DFnt(0, 0, NULL, 15, CCtdMan::getText(m_pSeqNoTbl[i].CTCode), 0x40);
            if (pFnt) {
                m_pSeqFntArray[i] = pFnt;
                pSeqParent->addFntShift(m_pLayoutName, pFnt, m_pSeqNoTbl[i].XPosi, m_pSeqNoTbl[i].YPosi);
            }
        }
        if (m_pSeqCodeTbl) m_pSeqCodeTbl[i] = -1;
    }
}
void CMemo2DLayout::finalize() {
    for (INT i = 0; i < 32; i++) {
        delete m_pSeqCtrlArray[i];
        m_pSeqCtrlArray[i] = NULL;
    }
    if (m_pSeqNoTbl) com_free(m_pSeqNoTbl);
    m_pSeqNoTbl = NULL;
    if (m_pSeqCodeTbl) com_free(m_pSeqCodeTbl);
    m_pSeqCodeTbl = NULL;
    if (m_pLayoutNoTbl) com_free(m_pLayoutNoTbl);
    m_pLayoutNoTbl = NULL;
    destroyLayer();
}
void CMemo2DLayout::setState(INT inState) {
    m_iState = inState;
}
INT CMemo2DLayout::getState() {
    return m_iState;
}
__asm__(".rdata\n"); /* resync: gcc believes it is in .rdata after the previous placeholder */
void CMemo2DLayout::createLayer(INT inKind) {
    D2::C2DMan::setLayer(m_iLayerLevel);
    m_hLayer = D2::C2DMan::create(m_pLayoutFileName, m_pLayoutNoTbl[inKind]);
    if (!m_hLayer) TRACE_USER((USER)10, "CMemo2DLayout error: Don't create layer task(%d)\n", inKind);
}
void CMemo2DLayout::destroyLayer() {
    D2::C2DMan::destroy(m_hLayer);
    m_hLayer = 0;
}
D2::C2DSeqCtrl* CMemo2DLayout::createSequence(INT inSeqNo, INT inLayer) {
#line 3011
    if (!(inSeqNo < 32)) AssertApp(__FILE__, __LINE__, "inSeqNo < SEQCTRL_MAX", 0);
    D2::C2DSeqCtrl* pSeq = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(3017)",
                                             ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })))
        D2::C2DSeqCtrl;
    if (pSeq) {
        pSeq->setLayer(inLayer);
        m_pSeqCtrlArray[inSeqNo] = pSeq;
        m_iSeqCtrlNum++;
    }
    return pSeq;
}
D2::C2DSeqCtrl* CMemo2DLayout::getSeqCtrl(INT inSeqNo) {
    if (inSeqNo < 0 || inSeqNo >= m_iSeqNum) return NULL;
    return m_pSeqCtrlArray[inSeqNo];
}
D2::C2DFnt* CMemo2DLayout::getFnt(INT inSeqNo) {
    if (inSeqNo < 0 || inSeqNo >= m_iSeqNum) return NULL;
    return m_pSeqFntArray[inSeqNo];
}
D2::C2DSeqCtrl* CMemo2DLayout::setSeqState(UINT inSeqNo, INT inState) {
    if (inSeqNo >= m_iSeqNum) return NULL;
    D2::C2DSeqCtrl* pSeq = getSeqCtrl(inSeqNo);
    if (pSeq) {
        INT state;
        switch (inState) {
        case 0:
            state = m_pSeqNoTbl[inSeqNo].In;
            break;
        case 1:
            state = m_pSeqNoTbl[inSeqNo].Loop;
            break;
        case 2:
            state = m_pSeqNoTbl[inSeqNo].Out;
            break;
        default:
            state = -1;
            break;
        }
        if (state >= 0) {
            m_pSeqCodeTbl[inSeqNo] = state;
            pSeq->replaceSeq(0, m_pLayoutName, m_pSeqCodeTbl[inSeqNo]);
            pSeq->enableDisp(TRUE);
        } else {
            m_pSeqCodeTbl[inSeqNo] = -1;
            pSeq->enableDisp(FALSE);
        }
    }
    return pSeq;
}
INT CMemo2DLayout::getSeqState(UINT inSeqNo) {
    INT rval = -1;
    if (inSeqNo >= m_iSeqNum) return -1;
    if (m_pSeqCodeTbl[inSeqNo] == m_pSeqNoTbl[inSeqNo].In) {
        rval = 0;
    } else if (m_pSeqCodeTbl[inSeqNo] == m_pSeqNoTbl[inSeqNo].Loop) {
        rval = 1;
    } else if (m_pSeqCodeTbl[inSeqNo] == m_pSeqNoTbl[inSeqNo].Out) {
        rval = 2;
    }
    return rval;
}
D2::C2DFnt* CMemo2DLayout::setSeqText(UINT inSeqNo, const CHAR* inText) {
    if (inSeqNo >= m_iSeqNum) return NULL;
    D2::C2DFnt* pFnt = m_pSeqFntArray[inSeqNo];
    if (pFnt) pFnt->setText(inText);
    return pFnt;
}
CMemo2DSequence::CMemo2DSequence() {
    m_vBasePosition.init();
    m_vShiftPosition.init();
    m_vAimPosition.init();
    m_vPosition.init();
    m_fScale = 1.0f;
    m_fMoveTimer = 0.0f;
    m_bMoveSlideEnable = FALSE;
    m_iState = 0;
    m_iStateFlag = 0;
    m_LayoutName[0] = '\0';
    for (INT i = 4; i >= 0; i--) {
        m_SeqTbl[i] = 0;
    }
}
/* Parked: final group, see the note above getTaskPointer<T> at the end of the file (matches when the group is C) */
#if 0
CMemo2DSequence::~CMemo2DSequence() {}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", _$_15CMemo2DSequence);
#endif
void CMemo2DSequence::setup(const CHAR* inSeqName, const INT* inSeqTbl, INT inLayer) {
#line 3138
    if (!(strlen(inSeqName) < 32)) AssertApp(__FILE__, __LINE__, "strlen(inSeqName) < 32", 0);
    strcpy(m_LayoutName, inSeqName);
    m_iStateFlag |= 1;
    for (INT i = 0; i < 5; i++) {
        m_SeqTbl[i] = inSeqTbl[i];
    }
    setLayer(inLayer);
    setState(1);
    enableDisp(FALSE);
}
void CMemo2DSequence::setup(const CHAR* inSeqName, INT inSeqNo, INT inLayer) {
#line 3158
    if (!(strlen(inSeqName) < 32)) AssertApp(__FILE__, __LINE__, "strlen(inSeqName) < 32", 0);
    strcpy(m_LayoutName, inSeqName);
    setLayer(inLayer);
    setState(1);
    addSeq(m_LayoutName, inSeqNo);
    enableDisp(TRUE);
}
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
void SubVectorXYZ(FVECTOR& out, const FVECTOR& a, const FVECTOR& b);
extern "C" void sceVu0InterVectorXYZ(FVECTOR& out, FVECTOR& v0, FVECTOR& v1, float r);
extern "C" float powf(float x, float y);
void CMemo2DSequence::main(FLOAT inFrameRate) {
    if (m_iStateFlag & 1) {
        switch (m_iState) {
        case 1:
            if (isEnd(0)) {
                setState(2);
                replaceSequenceId(1);
            }
            break;
        case 3:
            if (isEnd(0)) {
                setState(4);
                enableDisp(FALSE);
            }
            break;
        }
    }
    if (m_bMoveSlideEnable == 1) {
        m_fMoveTimer += inFrameRate;
        if (m_fMoveTimer >= 30.0f) {
            m_fMoveTimer = 30.0f;
            m_bMoveSlideEnable = FALSE;
        }
        FVECTOR posi = m_vPosition;
        FVECTOR aim = m_vAimPosition;
        FVECTOR shift = m_vShiftPosition;
        SubVectorXYZ(aim, aim, shift);
        sceVu0InterVectorXYZ(m_vPosition, aim, posi, 1.0f - powf(1.0f - m_fMoveTimer / 30.0f, 2.5f));
        setLocalPos(m_vPosition);
    }
}
void CMemo2DSequence::setSlideMoveParam(CMemo2DCursor* inCursor) {
    if (inCursor) {
        m_bMoveSlideEnable = TRUE;
        m_vAimPosition = inCursor->getAimVector();
        m_vShiftPosition = inCursor->getShiftVector();
        if (m_iStateFlag & 4) {
            m_vPosition = m_vAimPosition;
            setLocalPos(m_vPosition);
            m_fMoveTimer = 30.0f;
        } else {
            m_fMoveTimer = 0.0f;
        }
    }
}
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
CMemo2DCursor::CMemo2DCursor(HANDLE inHandle) {
    m_iPosiNum = 0;
    m_pPosiTbl = NULL;
    m_iStartNo = 0;
    for (INT i = 3; i >= 0; i--) m_iLadTbl[i] = 0;
    for (INT i = 0; i < 4; i++) m_fSpacing[i] = 0.0f;
    m_iLadSeq = 0;
    m_lpctLayoutName = NULL;
    m_vPosi.init();
    m_vAimPosi.init();
    m_vShiftPosi.init();
    m_bMoveSlideEnable = FALSE;
    m_fMoveTimer = 0.0f;
    m_hParent = inHandle;
    m_iState = 0;
    m_ScreenLimit[0].x = 0.0f;
    m_ScreenLimit[0].y = 0.0f;
    m_ScreenLimit[1].x = 512.0f;
    m_ScreenLimit[1].y = *(INT*)&SCREEN_HEIGHT;
    m_fScale = 1.0f;
}
/* Parked: final group, see the note above getTaskPointer<T> at the end of the file (matches when the group is C) */
#if 0
CMemo2DCursor::~CMemo2DCursor() {
    finalise();
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", _$_13CMemo2DCursor);
#endif
__asm__(".rdata\n"); /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
void CMemo2DCursor::setup(CursorDataParam& inParam) {
    if (inParam.posiNum > 0) {
        if (inParam.posiTbl) {
            m_iPosiNum = inParam.posiNum;
            m_pPosiTbl = (CursorPosition*)com_malloc_d(
                sizeof(CursorPosition) * m_iPosiNum, __FILE__ "(3348)",
                ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; }));
            for (INT i = 0; i < m_iPosiNum; i++) m_pPosiTbl[i].copy(inParam.posiTbl[i]);
        } else {
            m_iPosiNum = 0;
        }
    }
    m_iStartNo = inParam.startNo;
    for (INT i = 0; i < 4; i++) m_iLadTbl[i] = inParam.ladTbl[i];
    m_lpctLayoutName = inParam.LayoutName;
    for (INT i = 0; i < 4; i++) m_fSpacing[i] = inParam.spacing[i];
    m_fScale = inParam.scale;
    m_vPosi.x = m_pPosiTbl[m_iStartNo].x * m_fScale;
    m_vPosi.y = m_pPosiTbl[m_iStartNo].y * m_fScale;
    m_vPosi.z = 0.0f;
    if (inParam.limitTbl) {
        for (INT i = 0; i < 2; i++) m_ScreenLimit[i].copy(inParam.limitTbl[i]);
    }
    m_iLadSeq = 0;
    enableColorCtrl(TRUE);
    addSeq(m_lpctLayoutName, m_iLadTbl[0]);
    addSeq(m_lpctLayoutName, m_iLadTbl[3]);
    enableDisp(FALSE);
    setPosiId(m_iStartNo);
}
void CMemo2DCursor::finalise() {
    if (m_pPosiTbl) com_free(m_pPosiTbl);
    m_pPosiTbl = NULL;
}
void CMemo2DCursor::setReplaceSeq(INT inSeq) {
    m_iLadSeq = inSeq;
    replaceSeq(0, m_lpctLayoutName, m_iLadTbl[inSeq]);
    replaceSeq(1, m_lpctLayoutName, m_iLadTbl[3]);
}
void CMemo2DCursor::setState(INT inState) {
    if (inState & 0x1) setReplaceSeq(1);
    if (inState & 0x2) setReplaceSeq(0);
    if (inState & 0x4) setReplaceSeq(2);
    if (inState & 0x10000) enableDisp(FALSE);
    if (inState & 0x20000) enableDisp(TRUE);
    if (inState & 0x1000000) m_iState |= 0x1000000;
    if (inState & 0x2000000) m_iState |= 0x2000000;
    if (inState & 0x4000000) m_iState |= 0x4000000;
}
INT CMemo2DCursor::setPosiId(INT inPosiId) {
    INT rval;
    if (inPosiId < 0) {
        rval = -1;
    } else if (inPosiId >= m_iPosiNum) {
        rval = 1;
    } else {
        rval = 0;
        m_vAimPosi.x = m_pPosiTbl[inPosiId].x * m_fScale;
        m_vAimPosi.y = m_pPosiTbl[inPosiId].y * m_fScale;
        m_bMoveSlideEnable = TRUE;
        if (m_iState & 0x1000000) {
            m_fMoveTimer = 0.0f;
        } else {
            m_fMoveTimer = 30.0f;
        }
        m_iPosiId = inPosiId;
    }
    return rval;
}
void CMemo2DCursor::main() {
    FVECTOR posi;
    FVECTOR aim;
    FVECTOR shift;
    FLOAT limx0;
    FLOAT limy0;
    if (!(m_iState & 0x2000000)) {
        switch (m_iLadSeq) {
        case 0:
            if (isEnd(0)) setReplaceSeq(1);
            break;
        case 2:
            if (isEnd(0)) {
                m_iLadSeq = 5;
                enableDisp(FALSE);
                return;
            }
            break;
        }
    }
    if (m_bMoveSlideEnable) {
        m_fMoveTimer += getFrameRateF();
        if (m_fMoveTimer >= 30.0f) {
            m_fMoveTimer = 30.0f;
            m_bMoveSlideEnable = FALSE;
        }
        posi = m_vPosi;
        aim = m_vAimPosi;
        shift = m_vShiftPosi;
        SubVectorXYZ(aim, aim, shift);
        sceVu0InterVectorXYZ(posi, aim, posi, 1.0f - powf(1.0f - m_fMoveTimer / 30.0f, 2.5f));
        limx0 = m_ScreenLimit[0].x + m_fSpacing[0] * m_fScale;
        FLOAT limx1 = m_ScreenLimit[1].x + m_fSpacing[2] * m_fScale;
        if (posi.x > limx1) {
            shift.x += posi.x - limx1;
            posi.x = limx1;
        } else if (posi.x < limx0) {
            shift.x -= limx0 - posi.x;
            posi.x = limx0;
        }
        limy0 = m_ScreenLimit[0].y + m_fSpacing[1] * m_fScale;
        FLOAT limy1 = m_ScreenLimit[1].y + m_fSpacing[3] * m_fScale;
        if (posi.y > limy1) {
            shift.y += posi.y - limy1;
            posi.y = limy1;
        } else if (posi.y < limy0) {
            shift.y -= limy0 - posi.y;
            posi.y = limy0;
        }
        m_vPosi = posi;
        m_vShiftPosi = shift;
        setLocalPos(m_vPosi);
    }
}
FLOAT CMemo2DCursor::getFrameRateF() {
    if (m_hParent) {
        CTask* pTask = CTaskMan::getTask(m_hParent);
        if (pTask && !pTask->isOut()) return pTask->getFrameRate() * 0.020000001f;
    }
    return 1.0000001f;
}
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
CMemo2DCard::CMemo2DCard(const CHAR* inEmptySeqName) {
    m_vBasePosition.init();
    m_vShiftPosition.init();
    m_iState = 0;
    m_fScale = 1.0f;
    m_iTexNo = 0;
    m_iTexPos = 0;
    m_iFrameSeqNo = -1;
    m_pEmptySeqName = (CHAR*)inEmptySeqName;
    setScl(m_fScale);
}
/* Parked: final group, see the note above getTaskPointer<T> at the end of the file (matches when the group is C) */
#if 0
CMemo2DCard::~CMemo2DCard() {}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", _$_11CMemo2DCard);
#endif
__asm__(".rdata\n"); /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
extern "C" CARD_DATA cardDataTbl_004EA4F8[]; /* CardKind.h's static table: still in the asm tail of .rodata */
/* CardKind.h's inline lookup (line 561 in the original header) */
static inline CARD_DATA* getCardData(CARD inCard) {
    CARD_DATA* p = cardDataTbl_004EA4F8;
    return &p[inCard.m_nKind];
}
void CMemo2DCard::create(CARD inCard, INT inMode) {
    if (inCard.m_nCategory0 == 0xC) return;
    CARD_DATA* pData = getCardData(inCard);
    createPicture(pData->nTexNo, pData->nTexPos);
    if (inMode > 0) {
        BYTE frameSeqTbl[15] = { 0, 7, 5, 5, 6, 6, 6, 4, 0x2f, 2, 1, 0, 3, 9, 7 };
        m_iFrameSeqNo = (inCard.m_nKind == 0xB9) ? 0x14 : frameSeqTbl[inCard.m_nCategory1];
        addSeq("card:01", m_iFrameSeqNo);
    }
}
void CMemo2DCard::createPicture(UINT inTexNo, UINT inTexPos) {
    m_iTexNo = inTexNo;
    m_iTexPos = inTexPos;
    if (!m_nSeq) addSeq(m_pEmptySeqName, JiminyMemo::CursorSeqTbl[0]);
    changePicture(addSeq("card:02", 0));
}
extern "C" int sprintf(char* buf, const char* fmt, ...);
void CMemo2DCard::changePicture(D2::C2DSeqObj* p2DSeqObj) {
    G_UV uvStart;
    G_UV uvEnd;
    char buf[16];
    if (!p2DSeqObj) return;
    UINT texNo = m_iTexNo;
    INT texPos = m_iTexPos;
    if (texNo == 21 && texPos == 1) {
        if (JiminyMemo::is100AcreGoodEnding()) texPos = 5;
    }
    uvStart.set(((texPos & 3) << 6) | 2, ((texPos & 0xC) << 4) | 1);
    uvEnd.set(uvStart.u | 0x3C, uvStart.v | 0x3E);
    if (texNo < 22) {
        sprintf(buf, "card:%02d", texNo);
    } else if (texNo < 26) {
        sprintf(buf, "card_b:%02d", texNo - 22);
    } else if (texNo < 27) {
        sprintf(buf, "card_p:%02d", texNo - 26);
    } else if (texNo < 100) {
        sprintf(buf, "card_n:%02d", texNo - 27);
    } else {
        sprintf(buf, "jminy_chara");
    }
    p2DSeqObj->changeTexture(0, buf, &uvStart, &uvEnd);
}
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
void CMemo2DCard::setState(INT inState) {
    m_iState = inState;
    switch (inState) {
    case 1:
        break;
    case 2:
        replaceSeq(0, m_pEmptySeqName, JiminyMemo::CursorSeqTbl[1]);
        break;
    case 3:
        replaceSeq(0, m_pEmptySeqName, JiminyMemo::CursorSeqTbl[2]);
        break;
    case 4:
        enableDisp(FALSE);
        break;
    }
}
void CMemo2DCard::main(FLOAT inFrameRate) {
    switch (m_iState) {
    case 1:
        if (isEnd(0)) setState(2);
        break;
    case 3:
        if (isEnd(0)) setState(4);
        break;
    }
}
CJiminyMemo::CJiminyMemo(CTask* inParent, INT inPlayerMode) : CMenu(inParent, -1) {
    initialize(inPlayerMode);
}
/* Parked: final group, see the note above getTaskPointer<T> at the end of the file (matches when the group is C) */
#if 0
CJiminyMemo::~CJiminyMemo() {
    finalize();
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", _$_11CJiminyMemo);
#endif
__asm__(".rdata\n"); /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
void CJiminyMemo::initialize(INT inPlayerMode) {
    INT resourceId;
    TRACE_USER((USER)10, "%s\n", "########## Jiminy memo is MainApp boot. ##########");
    m_iPlayerMode = inPlayerMode;
    setContentMode(0);
    m_uiStateFlags = 0;
    m_hLayout = 0;
    m_pMessSeq = NULL;
    m_pMessString = NULL;
    m_ContentCommon.init();
    m_ContentMenu.init();
    m_ContentStory.init();
    m_ContentStoryView.init();
    m_ContentCollect.init();
    m_ContentCard.init();
    m_ContentCardSelect.init();
    m_ContentCardView.init();
    m_ContentChara.init();
    m_ContentCharaSelect.init();
    m_ContentCharaView.init();
    m_ContentMinigame.init();
    if (isModeJiminyMemo()) {
        m_pLadName = "jiminy_top:00";
        m_pLadFileName = "jiminy_top.lad";
    } else {
        m_pLadName = "Drepo:00";
        m_pLadFileName = "Drepo.lad";
    }
    resourceId = JiminyMemo::RsrcId[getLoadResourceIndex()];
    if (!CRsrcDB::hasRsrcGroup(2, resourceId)) {
        CRcf::reqLoadRCFFront(2, resourceId, 0, NULL, 0);
        if (isModeJiminyMemo()) {
            CRcf::reqLoadRCF(JiminyMemo::getHostGroupId(), JiminyMemo::getHostRsrcId(), 0, NULL, 0);
        }
        m_iStep = 10;
    } else {
        m_iStep = 20;
    }
}
void CJiminyMemo::finalize() {
    CMemo2DLayout* pLay;
    INT groupId;
    INT rsrcId;
    setContentStep(90);
    (this->*pContentFunc)();
    if (m_pMessSeq) delete m_pMessSeq;
    m_pMessSeq = NULL;
    pLay = getTaskPointer<CMemo2DLayout*>(m_hLayout);
    if (pLay) delete pLay;
    if (isModeJiminyMemo()) {
        groupId = JiminyMemo::getHostGroupId();
        rsrcId = JiminyMemo::getHostRsrcId();
        if (CRsrcDB::hasRsrcGroup(groupId, rsrcId)) {
            if (!CRcf::isCompleteLoadRCF(groupId, rsrcId)) {
                CRcf::cancelLoadRCF(groupId, rsrcId);
            } else {
                CRsrcDB::removeGroup(groupId, rsrcId);
            }
        }
    }
    rsrcId = JiminyMemo::RsrcId[getLoadResourceIndex()];
    if (CRsrcDB::hasRsrcGroup(2, rsrcId)) {
        if (!CRcf::isCompleteLoadRCF(2, rsrcId)) {
            CRcf::cancelLoadRCF(2, rsrcId);
        } else {
            CRsrcDB::removeGroup(2, rsrcId);
        }
    }
}
void CJiminyMemo::main() {
    switch (m_iStep) {
    case 10:
        mainLoadWait();
        break;
    case 20:
        mainIn();
        break;
    case 30:
        mainInWait();
        break;
    case 40:
        mainLoop();
        break;
    case 50:
        mainOut();
        break;
    case 60:
        mainOutWait();
        break;
    case 80:
        break;
    }
    FLOAT time = getContentTimer() - getFrameRateF();
    if (time < 0.0f) time = 0.0f;
    setContentTimer(time);
    (this->*pContentFunc)();
    if (getContentStep() == 100) {
        if (m_iStep == 80) {
            out();
        } else if (m_iStep == 40) {
            switch (getNextContentMode()) {
            case 0:
                setContentMode(getNextContentMode());
                m_ContentMenu.init();
                break;
            case 1:
                setContentMode(getNextContentMode());
                m_ContentStory.init();
                break;
            case 6:
                setContentMode(getNextContentMode());
                m_ContentStoryView.init();
                break;
            case 2:
                setContentMode(getNextContentMode());
                m_ContentCollect.init();
                break;
            case 3:
                setContentMode(getNextContentMode());
                m_ContentCard.init();
                break;
            case 7:
                setContentMode(getNextContentMode());
                m_ContentCardSelect.init();
                break;
            case 8:
                setContentMode(getNextContentMode());
                m_ContentCardView.init();
                break;
            case 4:
                setContentMode(getNextContentMode());
                m_ContentChara.init();
                break;
            case 9:
                setContentMode(getNextContentMode());
                m_ContentCharaSelect.init();
                break;
            case 10:
                setContentMode(getNextContentMode());
                m_ContentCharaView.init();
                break;
            case 5:
                setContentMode(getNextContentMode());
                m_ContentMinigame.init();
                break;
            case -1:
                m_iStep = 50;
                break;
            default:
#line 3934
                AssertApp(__FILE__, __LINE__, "0", 0);
                break;
            }
        }
    }
}
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
void CJiminyMemo::setContentMode(INT inContentMode) {
    INT prev = m_iContentMode;
    m_iContentMode = inContentMode;
    pContentFunc = m_pContentTbl[inContentMode];
    m_ContentCommon.init();
    m_ContentCommon.iStep = 20;
    m_ContentCommon.iPrevContent = prev;
}
/* placeholder: rodata of the items before createSequence__11CJiminyMemoPCiiiiP15CMemo2DSequence */
__asm__(".rdata\n");
inline void _roPh13() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE(__FILE__ "(3948)");
    TRACE(__FILE__ "(4012)");
    TRACE(__FILE__ "(4022)");
}
CMemo2DSequence* CJiminyMemo::createSequence(const INT* inSeqData, INT inLayer, INT inLayoutNo, INT inLayoutSeqNo,
                                             CMemo2DSequence* inParent) {
    FVECTOR posi;
    CMemo2DSequence* pSeq = new (com_malloc_d(sizeof(CMemo2DSequence), __FILE__ "(4170)",
                                              ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })))
        CMemo2DSequence;
    if (pSeq) {
        pSeq->setup(m_pLadName, inSeqData, inLayer);
        D2::C2DMan::getLayPosition(m_pLadFileName, inLayoutNo, inLayoutSeqNo, posi, 0);
        pSeq->setBasePosition(posi);
        if (inParent) {
            pSeq->setParent(inParent);
            pSeq->setLocalPosition();
        } else {
            pSeq->setPosition();
        }
    }
    return pSeq;
}
CMemo2DSequence* CJiminyMemo::createSequence(INT inSeqNo, INT inLayer, INT inLayoutNo, INT inLayoutSeqNo,
                                             CMemo2DSequence* inParent) {
    FVECTOR posi;
    CMemo2DSequence* pSeq = new (com_malloc_d(sizeof(CMemo2DSequence), __FILE__ "(4194)",
                                              ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })))
        CMemo2DSequence;
    if (pSeq) {
        pSeq->setup(m_pLadName, inSeqNo, inLayer);
        D2::C2DMan::getLayPosition(m_pLadFileName, inLayoutNo, inLayoutSeqNo, posi, 0);
        pSeq->setBasePosition(posi);
        if (inParent) {
            pSeq->setParent(inParent);
            pSeq->setLocalPosition();
        } else {
            pSeq->setPosition();
        }
    }
    return pSeq;
}
INT CJiminyMemo::createListSequence(CMemo2DSequence* inList, INT inLayer, INT inLayoutNo, INT inLayoutSeqNo,
                                    D2::C2DSeqCtrl** inFontSeq, D2::C2DFnt** inFont, UINT inFontCol, INT inFontSize,
                                    INT inFontStyle, const CHAR* inStr) {
    FVECTOR posi;
    D2::C2DFnt* pFnt;
    D2::C2DSeqCtrl* pListFont;
    G_RGBA fontRGB;
    if (!inList || !inFontSeq || !inFont) return FALSE;
    fontRGB.col = inFontCol;
    pListFont = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(4228)",
                                  ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })))
        D2::C2DSeqCtrl;
    if (pListFont) {
        *inFontSeq = pListFont;
        pListFont->setLayer(inLayer);
        pListFont->enableColorCtrl(TRUE);
        pListFont->setParent(inList);
        pListFont->addSeq(m_pLadName, 0x24);
        D2::C2DMan::getLayPosition(m_pLadFileName, inLayoutNo, inLayoutSeqNo, posi, 0);
        pListFont->setLocalPos(posi);
        pFnt = new (com_malloc_d(sizeof(D2::C2DFnt), __FILE__ "(4240)",
                                 ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })))
            D2::C2DFnt(0, 0, &fontRGB, inFontSize, inStr, inFontStyle);
        if (pFnt) {
            *inFont = pFnt;
            pListFont->addFnt(m_pLadName, pFnt);
        }
    }
    return FALSE;
}
INT CJiminyMemo::createListSequenceTypewriterFont(CMemo2DSequence* inList, INT inLayer, INT inLayoutNo,
                                                  INT inLayoutSeqNo, D2::C2DSeqCtrl** inFontSeq, D2::C2DFnt** inFont,
                                                  UINT inFontCol, INT inFontSize, INT inFontStyle, const CHAR* inStr,
                                                  FLOAT inWidth) {
    if (!inList) return 0;
    if (!inFontSeq || !inFont || !inStr || !strlen(inStr)) return 0;
    createListSequence(inList, inLayer, inLayoutNo, inLayoutSeqNo, inFontSeq, inFont, inFontCol, inFontSize,
                       inFontStyle, inStr);
    return 1;
}
void CJiminyMemo::replaceNoteList(D2::C2DFnt** inFnt, const CHAR** inString, INT inNum) {
    if (inFnt) {
        for (INT i = 0; i < inNum; i++) {
            D2::C2DFnt* pFnt = inFnt[i];
            const CHAR* pString = inString[i];
            if (pFnt) pFnt->setText(pString);
        }
    }
}
void CJiminyMemo::replaceNoteListWithOpenCheck(D2::C2DFnt** inList, CT_LOWER inStartCtNo, INT inFlagId, INT inListNum,
                                               INT inListAllNum, INT inStartNo) {
    if (inList) {
        D2::C2DFnt** pList = inList;
        INT flagIndex = inFlagId + inStartNo;
        CT_LOWER CtNo = inStartCtNo + inStartNo;
        for (INT i = 0; i < inListNum; i++, pList++, flagIndex++) {
            replaceNoteListString(*pList, JiminyMemo::getOpenCloseStringData(JiminyMemo::getCT0014Code(CtNo),
                                                                             CJiminyMemoFlag::isContentOpen(flagIndex)));
            CtNo++;
        }
    }
}
void CJiminyMemo::replaceNoteListWithOpenCheck(D2::C2DFnt** inList, CT_LOWER inStartCtNo, BYTE* inFlagTbl,
                                               INT inListNum, INT inListAllNum, INT inStartNo) {
    if (inList) {
        D2::C2DFnt** pList = inList;
        CT_LOWER CtNo = inStartCtNo + inStartNo;
        for (INT i = 0; i < inListNum; i++, pList++) {
            replaceNoteListString(*pList, JiminyMemo::getOpenCloseStringData(JiminyMemo::getCT0014Code(CtNo),
                                                                             inFlagTbl[inStartNo + i]));
            CtNo++;
        }
    }
}
void CJiminyMemo::replaceNoteListWithOpenCheck(D2::C2DFnt** inList, CT_LOWER* inCtTbl, INT inFlagId, INT inListNum,
                                               INT inListAllNum, INT inStartNo) {
    if (inList) {
        D2::C2DFnt** pList = inList;
        INT flagIndex = inFlagId + inStartNo;
        CT_LOWER* pCtTbl = &inCtTbl[inStartNo];
        for (INT i = 0; i < inListNum; i++, pList++, flagIndex++, pCtTbl++) {
            replaceNoteListString(*pList, JiminyMemo::getOpenCloseStringData(JiminyMemo::getCT0014Code(*pCtTbl),
                                                                             CJiminyMemoFlag::isContentOpen(flagIndex)));
        }
    }
}
void CJiminyMemo::replaceNoteListMark(D2::C2DSeqCtrl** inMark, BYTE* inFlagTbl, INT inListNum, INT inListAllNum,
                                      INT inStartNo, INT inLayoutNo, INT inLayoutSeqNo) {
    if (inMark) {
        D2::C2DSeqCtrl** pMark = inMark;
        INT listIndex = -inStartNo;
        FVECTOR posi;
        for (INT i = 0; i < inListAllNum; i++, pMark++, listIndex++) {
            if (*pMark && (*pMark)->isEnableDisp()) {
                if (listIndex >= 0 && listIndex < inListNum && inFlagTbl[i]) {
                    D2::C2DMan::getLayPosition(m_pLadFileName, inLayoutNo, inLayoutSeqNo + listIndex, posi, 0);
                } else {
                    posi.set(0.0f, -1000.0f, 0.0f);
                }
                (*pMark)->setLocalPos(posi);
            }
        }
    }
}
__asm__(".rdata\n"); /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
void CJiminyMemo::replaceNoteListMark(D2::C2DSeqCtrl** inMark, INT inFlagId, INT inListNum, INT inListAllNum,
                                      INT inStartNo, INT inLayoutNo, INT inLayoutSeqNo) {
    INT flagIndex = inFlagId;
    BYTE* pFlagTbl = (BYTE*)com_malloc_d(inListAllNum, __FILE__ "(4394)", (UINT)-1);
    if (pFlagTbl) {
        for (INT i = 0; i < inListAllNum; i++, flagIndex++) {
            pFlagTbl[i] = CJiminyMemoFlag::isContentOpen(flagIndex) ? 1 : 0;
        }
        replaceNoteListMark(inMark, pFlagTbl, inListNum, inListAllNum, inStartNo, inLayoutNo, inLayoutSeqNo);
        if (pFlagTbl) com_free(pFlagTbl);
    }
}
namespace JiminyMemo {
namespace Collect {
extern INT CardMapSpacingTbl[4];
}
}
CMemo2DCursor* CJiminyMemo::createCollectionCursorSequence(const INT* inSeqTbl, const CursorPosition& inStartPosi,
                                                           const CursorPosition& inStepPosi, INT inStepNum,
                                                           INT inListNum, INT inOffset, INT inStart,
                                                           INT inPosiWidth, INT inLayer, INT inState, INT inPosiId,
                                                           FLOAT inScale, CMemo2DSequence* inParent) {
    CMemo2DCursor* pCur;
    CursorDataParam param;
    pCur = new (com_malloc_d(sizeof(CMemo2DCursor), __FILE__ "(4411)",
                             ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })))
        CMemo2DCursor(getHandle());
    if (pCur) {
        CursorPosition* cpTbl = (CursorPosition*)com_malloc_d(sizeof(CursorPosition) * (inListNum + 2),
                                                              __FILE__ "(4413)", -1);
        if (cpTbl) {
            CursorPosition* limitTbl = cpTbl + inListNum;
            createCursorPosiTbl(cpTbl, inStartPosi, inStepPosi, inListNum, inPosiWidth);
            limitTbl[0].x = -200.0f;
            limitTbl[0].y = -100.0f;
            limitTbl[1].x = 200.0f;
            limitTbl[1].y = 100.0f;
            param.init();
            param.posiNum = inListNum;
            param.posiTbl = cpTbl;
            param.set(m_pLadName, inListNum, inStart, inSeqTbl, limitTbl, inScale);
            param.setSpacing(JiminyMemo::Collect::CardMapSpacingTbl[0], JiminyMemo::Collect::CardMapSpacingTbl[1],
                             JiminyMemo::Collect::CardMapSpacingTbl[2], JiminyMemo::Collect::CardMapSpacingTbl[3]);
            pCur->setup(param);
            pCur->setLayer(inLayer);
            pCur->setState(inState);
            pCur->setPosiId(inPosiId);
            if (inParent) pCur->setParent(inParent);
            if (cpTbl) com_free(cpTbl);
        } else {
            delete pCur;
            pCur = NULL;
        }
    }
    return pCur;
}
void CJiminyMemo::createCursorPosiTbl(CursorPosition* inPosiTbl, const CursorPosition& inStartPosi,
                                      const CursorPosition& inStepPosi, INT inListNum, INT inWidth) {
    CursorPosition offset;
    INT height = inListNum / inWidth + (inListNum % inWidth > 0);
    offset.y = inStartPosi.y;
    for (INT i = 0; i < height; i++) {
        offset.x = inStartPosi.x;
        for (INT j = 0; j < inWidth; j++) {
            inPosiTbl[i * inWidth + j].x = offset.x;
            inPosiTbl[i * inWidth + j].y = offset.y;
            offset.x += inStepPosi.x;
        }
        offset.y += inStepPosi.y;
    }
}
CMemo2DCursor* CJiminyMemo::createCursorSequence(const INT* inSeqTbl, INT inLayoutNo, INT inStartSeq,
                                                 const CursorPosition& inPosiOffset, INT inStepNum, INT inListNum,
                                                 INT inOffset, INT inStart, INT inWidth, INT inLayer, INT inState,
                                                 INT inPosiId, CMemo2DSequence* inParent) {
    CMemo2DCursor* pCur;
    CursorDataParam param;
    FVECTOR posi0;
    FVECTOR posi1;
    FVECTOR interval;
    pCur = new (com_malloc_d(sizeof(CMemo2DCursor), __FILE__ "(4472)",
                             ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })))
        CMemo2DCursor(getHandle());
    if (pCur) {
        CursorPosition* cpTbl = (CursorPosition*)com_malloc_d(sizeof(CursorPosition) * (inListNum + 2),
                                                              __FILE__ "(4474)", -1);
        if (cpTbl) {
            CursorPosition* limitTbl = cpTbl + inListNum;
            D2::C2DMan::getLayPosition(m_pLadFileName, inLayoutNo, inStartSeq, posi0, 0);
            D2::C2DMan::getLayPosition(m_pLadFileName, inLayoutNo, inStartSeq + 1, posi1, 0);
            interval.x = posi1.x - posi0.x;
            interval.y = posi1.y - posi0.y;
            for (INT i = 0; i < inListNum; i++) {
                cpTbl[i].x = posi0.x + inPosiOffset.x + interval.x * i;
                cpTbl[i].y = posi0.y + inPosiOffset.y + interval.y * i;
            }
            limitTbl[0].copy(cpTbl[0]);
            limitTbl[1].copy(cpTbl[inStepNum - 1]);
            param.init();
            param.posiNum = inListNum;
            param.posiTbl = cpTbl;
            param.set(m_pLadName, inListNum, inStart, inSeqTbl, limitTbl);
            pCur->setup(param);
            pCur->setLayer(inLayer);
            pCur->setState(inState);
            pCur->setPosiId(inPosiId);
            if (inParent) pCur->setParent(inParent);
            if (cpTbl) com_free(cpTbl);
        } else {
            delete pCur;
            pCur = NULL;
        }
    }
    return pCur;
}
void CJiminyMemo::createMarkSequence(INT inMarkSeq, INT inNum, INT inLayoutNo, INT inLayoutSeqOffset, INT inLayer,
                                     D2::C2DSeqCtrl* inParent, D2::C2DSeqCtrl** inArray) {
    D2::C2DSeqCtrl* pMark;
    FVECTOR posi;
    INT i;
    if (!inArray) return;
    for (i = 0; i < inNum; i++) {
        pMark = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(4520)",
                                  ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })))
            D2::C2DSeqCtrl;
        if (pMark) {
            if (inArray) inArray[i] = pMark;
            if (inParent) pMark->setParent(inParent);
            pMark->setLayer(inLayer);
            pMark->enableColorCtrl(TRUE);
            pMark->addSeq(m_pLadName, 0x24);
            pMark->addSeq(m_pLadName, inMarkSeq);
            if (inLayoutNo >= 0) {
                D2::C2DMan::getLayPosition(m_pLadFileName, inLayoutNo, inLayoutSeqOffset + i, posi, 0);
                if (inParent) {
                    pMark->setLocalPos(posi);
                } else {
                    pMark->setPos(posi);
                }
            }
        }
    }
}
INT CJiminyMemo::changeStateSequence(D2::C2DSeqCtrl** inSeq, INT inNum, INT inMode, INT inSeqId) {
    if (!inSeq) return 0;
    for (INT i = 0; i < inNum; i++) {
        D2::C2DSeqCtrl* pSeq = inSeq[i];
        if (pSeq) {
            if (!inMode || pSeq->isEnableDisp()) pSeq->replaceSeq(0, m_pLadName, inSeqId);
        }
    }
    return 1;
}
INT CJiminyMemo::changeStateLoopSequence(D2::C2DSeqCtrl** inSeq, INT inNum) {
    return changeStateSequence(inSeq, inNum, 0, 0x25);
}
INT CJiminyMemo::changeStateOutSequence(D2::C2DSeqCtrl** inSeq, INT inNum) {
    return changeStateSequence(inSeq, inNum, 1, 0x49);
}
void CJiminyMemo::setTelopMessage(const CHAR* inMess) {
    if (m_pMessString) m_pMessString->setText(inMess);
}
INT CJiminyMemo::splitMessage(INT* outNum, CHAR*** outBuf, const CHAR* inStr, INT inWidth) {
    INT lineNum;
    const CHAR* pChar0;
    const CHAR* pChar1;
    INT len;
    if (!inStr) {
        *outNum = 0;
        return 0;
    }
    inWidth *= 2;
    inWidth += 10;
    for (INT i = 0; i < 2; i++) {
        pChar0 = inStr;
        lineNum = 0;
        len = 0;
        while (*pChar0 != '\0') {
            pChar1 = pChar0 + len;
#line 4612
            if (!(len < (inWidth - 1))) AssertApp(__FILE__, __LINE__, "len < (inWidth - 1)", 0);
            if (*pChar1 == '\n' || *pChar1 == '\0') {
                if (i == 1) {
                    (*outBuf)[lineNum] = (CHAR*)com_malloc_d(inWidth + 1, __FILE__ "(4617)", -1);
                    memcpy((*outBuf)[lineNum], pChar0, len + 1);
                    if (*pChar1 == '\n') {
                        (*outBuf)[lineNum][len] = '\0';
                    } else if (*pChar1 != '\0') {
                        (*outBuf)[lineNum][len + 1] = '\0';
                    }
                    TRACE_USER((USER)2, (*outBuf)[lineNum]);
                    TRACE_USER((USER)2, "  (%d)\n", strlen((*outBuf)[lineNum]));
                }
                lineNum++;
                len = 0;
                if (*pChar1 == '\n') {
                    pChar0 = pChar1 + 1;
                } else if (*pChar1 == '\0') {
                    break;
                } else {
                    pChar0 = pChar1 + 1;
                }
            } else {
                len++;
            }
            if (*pChar1 == '\0') break;
        }
        if (i == 0) {
            TRACE_USER((USER)2, "String line num = %d\n", lineNum);
            *outBuf = (CHAR**)com_malloc_d(sizeof(CHAR*) * (lineNum + 1), __FILE__ "(4660)", -1);
            for (INT j = 0; j < lineNum + 1; j++) (*outBuf)[j] = NULL;
            *outNum = lineNum;
        }
    }
    return 0;
}
void CJiminyMemo::setPlateState(INT inKind, INT inState) {
    CMemo2DLayout* seq = getTaskPointer<CMemo2DLayout*>(m_hLayout);
    if (seq) {
        switch (inKind) {
        case 0:
            seq->setSeqState(0, inState);
            seq->setSeqState(1, inState);
            seq->setSeqState(2, inState);
            break;
        case 1:
            seq->setSeqState(3, inState);
            seq->setSeqState(4, inState);
            seq->setSeqState(5, inState);
            break;
        case 2:
            seq->setSeqState(6, inState);
            seq->setSeqState(7, inState);
            break;
        case 3:
            seq->setSeqState(8, inState);
            break;
        case 4:
            seq->setSeqState(9, inState);
            break;
        case 5:
            seq->setSeqState(10, inState);
            break;
        case 6:
            seq->setSeqState(11, inState);
            break;
        }
    }
}
INT CJiminyMemo::getPlateState(INT inKind) {
    CMemo2DLayout* seq = getTaskPointer<CMemo2DLayout*>(m_hLayout);
    if (!seq || seq->isOut()) return -2;
    switch (inKind) {
    case 0:
        return seq->getSeqState(0);
    case 1:
        return seq->getSeqState(3);
    case 2:
        return seq->getSeqState(6);
    case 3:
        return seq->getSeqState(8);
    case 4:
        return seq->getSeqState(9);
    case 5:
        return seq->getSeqState(10);
    case 6:
        return seq->getSeqState(11);
    }
    return -2;
}
void CJiminyMemo::setPlateText(INT inKind, INT inCTNo) {
    CHAR* pText;
    if (inCTNo < 0 || !(pText = CCtdMan::getText(inCTNo))) pText = "";
    CMemo2DLayout* seq = getTaskPointer<CMemo2DLayout*>(m_hLayout);
    if (seq) {
        switch (inKind) {
        case 0:
            seq->setSeqText(1, pText);
            break;
        case 1:
            seq->setSeqText(4, pText);
            break;
        case 2:
            seq->setSeqText(7, pText);
            break;
        case 3:
            break;
        case 4:
            break;
        }
    }
}
namespace JiminyMemo {
namespace Base {
extern CursorPosition L1R1DigitNumOffset;
extern CursorPosition L1R1DigitDenomiOffset;
}
}
void CJiminyMemo::setPlateNumSeq(INT inKind, INT inParam) {
    D2::C2DSeqCtrl* pDig = NULL;
    INT n;
    INT offsetSeq;
    FLOAT offsetX;
    FLOAT offsetY;
    CMemo2DLayout* seq = getTaskPointer<CMemo2DLayout*>(m_hLayout);
    if (!seq) return;
    switch (inKind) {
    case 7:
    case 8:
        pDig = seq->getSeqCtrl(8);
        break;
    case 9:
    case 10:
        pDig = seq->getSeqCtrl(9);
        break;
    }
    switch (inKind) {
    case 7:
    case 9:
        offsetSeq = 1;
        offsetX = JiminyMemo::Base::L1R1DigitNumOffset.x;
        offsetY = JiminyMemo::Base::L1R1DigitNumOffset.y;
        break;
    case 8:
    case 10:
        offsetSeq = 5;
        offsetX = JiminyMemo::Base::L1R1DigitDenomiOffset.x;
        offsetY = JiminyMemo::Base::L1R1DigitDenomiOffset.y;
        break;
    }
    if (pDig) {
        n = inParam;
        for (INT i = 0; i < 3; i++) {
            pDig->replaceSeqShift(offsetSeq + i, m_pLadName, n % 10 + 0x95, offsetX + (2 - i) * 16.0f, offsetY);
            n /= 10;
        }
    }
}
void CJiminyMemo::getMenuContentFlag(INT inFlagId, INT inNum, INT* inFlagWork, BOOL* inCompWork) {
    *inFlagWork = 0;
    for (INT i = 0; i < inNum; i++) {
        if (CJiminyMemoFlag::isContentFlag(inFlagId + i, 4)) {
            *inFlagWork |= 4;
        } else if (CJiminyMemoFlag::isContentFlag(inFlagId + i, 2)) {
            *inFlagWork |= 2;
        } else if (CJiminyMemoFlag::isContentFlag(inFlagId + i, 1)) {
            *inFlagWork = 1;
            break;
        }
    }
    *inCompWork = TRUE;
    for (INT i = 0; i < inNum; i++) {
        if (!CJiminyMemoFlag::isContentOpen(inFlagId + i)) {
            *inCompWork = FALSE;
            break;
        }
    }
}
INT CJiminyMemo::getPrevContentId(INT inEventFlagStartId, INT inEventFlagNum, INT inCurrentNo) {
    INT rval = -1;
    for (INT i = inCurrentNo - 1; i >= 0; i--) {
        if (CJiminyMemoFlag::isContentOpen(inEventFlagStartId + i)) {
            rval = i;
            break;
        }
    }
    if (rval == -1) {
        for (INT i = inEventFlagNum - 1; i > inCurrentNo; i--) {
            if (CJiminyMemoFlag::isContentOpen(inEventFlagStartId + i)) {
                rval = i;
                break;
            }
        }
    }
    return rval;
}
INT CJiminyMemo::getNextContentId(INT inEventFlagStartId, INT inEventFlagNum, INT inCurrentNo) {
    INT rval = -1;
    for (INT i = inCurrentNo + 1; i < inEventFlagNum; i++) {
        if (CJiminyMemoFlag::isContentOpen(inEventFlagStartId + i)) {
            rval = i;
            break;
        }
    }
    if (rval == -1) {
        for (INT i = 0; i < inCurrentNo; i++) {
            if (CJiminyMemoFlag::isContentOpen(inEventFlagStartId + i)) {
                rval = i;
                break;
            }
        }
    }
    return rval;
}
namespace JiminyMemo {
extern USHORT NoteScrollBarTbl[3];
}
void CJiminyMemo::ScrollBarInit(D2::C2DSeqCtrl** inSeqCtrl, INT inLayer, FVECTOR& inPos, FLOAT inBaseHeight,
                                CMemo2DSequence* inParent) {
    FVECTOR vec;
    for (INT i = 0; i < 3; i++) {
        createMarkSequence(JiminyMemo::NoteScrollBarTbl[i], 1, -1, 0, inLayer, inParent, &inSeqCtrl[i]);
        D2::C2DSeqCtrl* p = inSeqCtrl[i];
        if (p) {
            switch (i) {
            case 0:
                p->setLocalPos(inPos);
                vec.set(1.0f, inBaseHeight * 0.01f, 1.0f, 0.0f);
                p->setScl(vec);
                break;
            case 1:
                vec.set(inPos.x, inPos.y + 1.0f, inPos.z, inPos.w);
                p->setLocalPos(vec);
                break;
            case 2:
                vec.set(inPos.x, inPos.y + inBaseHeight - 2.0f, inPos.z, inPos.w);
                p->setLocalPos(vec);
                break;
            }
        }
    }
}
/* Parked: 22 lines: ours hoists the 2.0f of case 2 out of the loop (an extra saved f24, frame +16, 0.01f moves
   from f23 to f24); the original loads it inside case 2 like ScrollBarInit does. Everything else matches.
   tried: y expression forms (temp, parenthesised, vec = vPos + .y), case order, INT i at the top, NULL compare, 0 vs
   0.0f w, fRatioPosY reused for the product; tools/permuter.py (fixed wrapper, target draft on, other drafts hidden)
   10 min, 6432 candidates: no improvement (earlier "score 1" runs were a wrapper bug). */
#if 0
void CJiminyMemo::ScrollBarChange(D2::C2DSeqCtrl** inSeqCtrl, FLOAT inBasePosY, FLOAT inBaseHeight,
                                  INT inCursorPosiNum, INT inNoteListNum, INT inStartNum) {
    FLOAT fRatioHeight;
    FLOAT fRatioPosY;
    FVECTOR vPos;
    FVECTOR vec;
    if (!inSeqCtrl) return;
    fRatioHeight = (FLOAT)inCursorPosiNum / (FLOAT)inNoteListNum;
    fRatioPosY = (1.0f - fRatioHeight) / ((FLOAT)inNoteListNum - (FLOAT)inCursorPosiNum) * (FLOAT)inStartNum;
    vPos = inSeqCtrl[0]->m_seqControl.vPos;
    vPos.y = inBasePosY + inBaseHeight * fRatioPosY;
    for (INT i = 0; i < 3; i++) {
        D2::C2DSeqCtrl* p = inSeqCtrl[i];
        if (p) {
            switch (i) {
            case 0:
                p->setLocalPos(vPos);
                vec.set(1.0f, inBaseHeight * 0.01f * fRatioHeight, 1.0f, 0.0f);
                p->setScl(vec);
                break;
            case 1:
                vec.set(vPos.x, vPos.y + 1.0f, vPos.z, vPos.w);
                p->setLocalPos(vec);
                break;
            case 2:
                vec.set(vPos.x, vPos.y + inBaseHeight * fRatioHeight - 2.0f, vPos.z, vPos.w);
                p->setLocalPos(vec);
                break;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", ScrollBarChange__11CJiminyMemoPPQ22D210C2DSeqCtrlffiii);
#endif
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
BOOL CJiminyMemo::nextPage(CMemo2DCursor* pCursor, INT* pCursorCurrent, INT iNoteListNum, INT iCursorPosiNum,
                           FLOAT fCursorIntervalY) {
    if (!pCursor || !pCursorCurrent) return FALSE;
    INT iCursorIntervalY = (INT)fCursorIntervalY;
    INT iScrollMax = iNoteListNum - iCursorPosiNum;
    if (*pCursorCurrent == iNoteListNum - 1) {
        *pCursorCurrent = 0;
    } else {
        FVECTOR posi = pCursor->getShiftVector();
        INT offset = (INT)posi.y / iCursorIntervalY;
        if (offset < iScrollMax) {
            INT next = offset + iCursorPosiNum;
            if (iScrollMax < next) {
                *pCursorCurrent = iScrollMax + (*pCursorCurrent - offset);
                offset = iScrollMax;
            } else {
                *pCursorCurrent += iCursorPosiNum;
                if (*pCursorCurrent >= iNoteListNum) *pCursorCurrent = iNoteListNum - 1;
                offset = next;
            }
            posi.y = offset * iCursorIntervalY;
            pCursor->setShiftVector(posi);
        } else {
            *pCursorCurrent = iNoteListNum - 1;
        }
    }
    return TRUE;
}
BOOL CJiminyMemo::prevPage(CMemo2DCursor* pCursor, INT* pCursorCurrent, INT iNoteListNum, INT iCursorPosiNum,
                           FLOAT fCursorIntervalY) {
    if (!pCursor || !pCursorCurrent) return FALSE;
    INT iCursorIntervalY = (INT)fCursorIntervalY;
    if (*pCursorCurrent == 0) {
        *pCursorCurrent = iNoteListNum - 1;
    } else {
        FVECTOR posi = pCursor->getShiftVector();
        INT offset = (INT)posi.y / iCursorIntervalY;
        if (offset > 0) {
            INT next = offset - iCursorPosiNum;
            if (next < 0) {
                *pCursorCurrent -= offset;
                offset = 0;
            } else {
                *pCursorCurrent -= iCursorPosiNum;
                if (*pCursorCurrent < 0) *pCursorCurrent = 0;
                offset = next;
            }
            posi.y = offset * iCursorIntervalY;
            pCursor->setShiftVector(posi);
        } else {
            *pCursorCurrent = 0;
        }
    }
    return TRUE;
}
BOOL CJiminyMemo::isButtonRptLimitStop(UINT inBit, UINT inPad, BOOL inStopCompare) {
    BOOL rval = FALSE;
    if (_isButtonRpt(inBit, inPad)) {
        if (inStopCompare) {
            rval = _isButtonTrg(inBit, inPad);
        } else {
            rval = TRUE;
        }
    }
    return rval;
}
BOOL CJiminyMemo::haveSpecialCard() {
    BOOL bRet = FALSE;
    if (!m_iPlayerMode) {
        if (CJiminyMemoFlag::isContentOpen(0x14D) || CJiminyMemoFlag::isContentOpen(0x14E)) bRet = TRUE;
    }
    return bRet;
}
namespace JiminyMemo {
namespace Menu {
extern INT NoteSeqData[5];
extern INT TalkSeqData[5];
extern INT CursorLightSeqTbl[5];
extern FVECTOR JiminyPosi;
extern CursorPosition CursorPosiOffset;
extern CursorPosition NewMarkPosiOffset;
extern FLOAT JiminyRotY __asm__("D_0068C400"); /* PARTIAL: .bss still from the original's assembly */
}
}
/* Parked: 129 lines: the stack slots of iFlag[5]/bComp[5] (and the pMarkSeq/pNewSeq spills) differ, which
   shifts every s-register from the prologue on (s7 vs s8 saves); the code itself matches in structure.
   tried: declaration orders of posi/telopNo/iFlag/bComp/pMarkSeq/pNewSeq (posi after bComp fixes the slots but
   breaks the registers, 270 lines), listNum statement order per arm, member vs local listNum, braces per
   statement; tools/permuter.py (fixed wrapper, other drafts hidden) 3 runs, 277 -> 141 -> 129 applied. */
#if 0
void CJiminyMemo::contentMenu() {
    BOOL bCursorShift = FALSE;
    switch (getContentStep()) {
    case 20: {
        FVECTOR posi;
        INT listNum;
        int iFlag[5];
        int bComp[5];
        INT telopNo;
        D2::C2DSeqCtrl** pNewSeq;
        D2::C2DSeqCtrl** pMarkSeq;
        CMemo2DSequence* pList;
        if (isModeJiminyMemo()) {
            getMenuContentFlag(0, 0x11, &iFlag[0], &bComp[0]);
            getMenuContentFlag(0xB7, 0x98, &iFlag[1], &bComp[1]);
            listNum = 5;
            iFlag[2] = iFlag[1];
            bComp[2] = bComp[1];
            telopNo = 0x700101;
            getMenuContentFlag(0x17, 0x68, &iFlag[3], &bComp[3]);
            getMenuContentFlag(0x18A, 6, &iFlag[4], &bComp[4]);
            if (bComp[4] == TRUE && CGameWork::getByte(0, 4) >= 2 && CGameWork::getByte(0, 5) >= 2 &&
                CGameWork::getByte(0, 6) >= 2 && CGameWork::getByte(0, 7) >= 2 && CGameWork::getByte(0, 8) >= 2 &&
                CJiminyMemoFlag::isContentOpen(0x18F)) {
                bComp[4] = TRUE;
            } else {
                bComp[4] = FALSE;
            }
            if (!iFlag[4]) listNum = 4;
        } else {
            telopNo = 0x700116;
            listNum = 4;
            getMenuContentFlag(0x11, 6, &iFlag[0], &bComp[0]);
            getMenuContentFlag(0x14F, 0x3B, &iFlag[1], &bComp[1]);
            iFlag[2] = iFlag[1];
            bComp[2] = bComp[1];
            getMenuContentFlag(0x7F, 0x38, &iFlag[3], &bComp[3]);
        }
        m_ContentMenu.iCursorPosiNum = listNum;
        m_ContentMenu.iTelopNo = telopNo;
        m_ContentMenu.iStringNum = listNum + 1;
        m_ContentMenu.pString = new (__FILE__ "(5252)") D2::C2DFnt*[m_ContentMenu.iStringNum];
        if (m_ContentMenu.pString) {
            for (INT i = 0; i < m_ContentMenu.iStringNum; i++) m_ContentMenu.pString[i] = NULL;
        }
        m_ContentMenu.iSeqCtrlNum = m_ContentMenu.iStringNum + 10;
        m_ContentMenu.pSeqCtrl = new (__FILE__ "(5258)") D2::C2DSeqCtrl*[m_ContentMenu.iSeqCtrlNum];
        if (m_ContentMenu.pSeqCtrl) {
            for (INT i = 0; i < m_ContentMenu.iSeqCtrlNum; i++) m_ContentMenu.pSeqCtrl[i] = NULL;
        }
        pMarkSeq = m_ContentMenu.pSeqCtrl + m_ContentMenu.iStringNum;
        pNewSeq = m_ContentMenu.pSeqCtrl + m_ContentMenu.iStringNum + 5;
        pList = createSequence(JiminyMemo::Menu::NoteSeqData, 0x10, 1, 0, NULL);
        if (pList) {
            m_ContentMenu.pList = pList;
            pList->setInState(pList->getSideInId(getPrevContentMode() < 0));
            for (INT i = 0; i < listNum; i++) {
                createListSequence(pList, 0x11, 0, 0xE + i, &m_ContentMenu.pSeqCtrl[i], &m_ContentMenu.pString[i],
                                   0x807F7F7F, 16, 0x40, CCtdMan::getText(i + 0x700201));
            }
        }
        m_ContentMenu.iCursorCurrent = getPrevContentMode() < 1 ? 0 : getPrevContentMode() - 1;
        m_ContentMenu.pCursor = createCursorSequence((const INT*)JiminyMemo::CursorSeqTbl, 0, 2,
                                                     JiminyMemo::Menu::CursorPosiOffset, listNum, listNum, 0, 0, 1,
                                                     0x11, 0x20002, m_ContentMenu.iCursorCurrent, pList);
        m_ContentMenu.pCursorLight = createSequence(JiminyMemo::Menu::CursorLightSeqTbl, 0x11, 0, 2, pList);
        if (m_ContentMenu.pCursorLight) m_ContentMenu.pCursorLight->setInState(0);
        createMarkSequence(0x92, listNum, 0, 8, 0x11, pList, pMarkSeq);
        bCursorShift = TRUE;
        for (INT i = 0; listNum > i; i++) {
            if (pMarkSeq[i]) pMarkSeq[i]->enableDisp(bComp[i]);
        }
        createMarkSequence(0x93, listNum, 0, 8, 0x11, pList, pNewSeq);
        for (INT i = 0; listNum > i; i++) {
            if (pNewSeq[i]) {
                if (1 & iFlag[i]) {
                    pNewSeq[i]->enableDisp(TRUE);
                } else if (iFlag[i] & 2) {
                    pNewSeq[i]->enableDisp(TRUE);
                    pNewSeq[i]->replaceSeq(1, m_pLadName, 0x94);
                } else {
                    pNewSeq[i]->enableDisp(FALSE);
                }
                pNewSeq[i]->shiftSeqPos(1, JiminyMemo::Menu::NewMarkPosiOffset.x, JiminyMemo::Menu::NewMarkPosiOffset.y);
            }
        }
        setContentTimer(getPrevContentMode() < 0 ? 45.0f : 16.0f);
        setContentStep(isModeDReport() ? 31 : 30);
        break;
    }
    case 30:
        if (getContentTimer() <= 15.0f) {
            CMemo3DObj* pObj;
            setContentStep(31);
            pObj = new (__FILE__ "(5350)")
                CMemo3DObj(this, JiminyMemo::getHostGroupId(), JiminyMemo::getHostRsrcId(), 0x12, FALSE);
            if (pObj) {
                CMemo2DSequence* pTalk;
                m_ContentMenu.hJiminyObj = pObj->getHandle();
                pObj->setPosition(JiminyMemo::Menu::JiminyPosi);
                pObj->setRotateY(JiminyMemo::Menu::JiminyRotY);
                pObj->setScale(8.0f);
                pTalk = new (__FILE__ "(5359)") CMemo2DSequence;
                if (pTalk) {
                    FVECTOR posi1;
                    FVECTOR posi0;
                    FVECTOR posi2;
                    D2::C2DFnt* pFnt;
                    INT no;
                    G_RGBA fontRGB;
                    m_ContentMenu.pTalk = pTalk;
                    pTalk->setup(m_pLadName, JiminyMemo::Menu::TalkSeqData, 0x11);
                    pTalk->setInState(pTalk->getSideInId(getPrevContentMode() < 0));
                    D2::C2DMan::getLayPosition(m_pLadFileName, 1, 1, posi0, 0);
                    pTalk->setBasePosition(posi0);
                    pTalk->setPosition();
                    no = m_ContentMenu.iCursorPosiNum;
                    fontRGB.col = 0x80000000;
                    pFnt = new (__FILE__ "(5377)") D2::C2DFnt(0, 0, &fontRGB, 18, CCtdMan::getText(0x700206), 0x21002);
                    if (pFnt) {
                        if (m_ContentMenu.pString) m_ContentMenu.pString[no] = pFnt;
                        D2::C2DMan::getLayPosition(m_pLadFileName, 1, 0, posi1, 0);
                        D2::C2DMan::getLayPosition(m_pLadFileName, 0, 0x13, posi2, 0);
                        __typeof__((posi0.x - posi1.x)) pt395 = (posi0.x - posi1.x); posi1.x = posi2.x - pt395;
                        posi1.y = posi2.y - (posi0.y - posi1.y);
                        pTalk->addFntShift(m_pLadName, pFnt, posi1.x, posi1.y);
                        pTalk->enableDisp(1, FALSE);
                    }
                }
            }
        }
        break;
    case 31:
        if (getContentTimer() <= 0.0f) {
            if (m_ContentMenu.pSeqCtrl) changeStateLoopSequence(m_ContentMenu.pSeqCtrl, m_ContentMenu.iSeqCtrlNum);
            if (m_ContentMenu.pTalk) m_ContentMenu.pTalk->enableDisp(1, TRUE);
            setTelopMessage(CCtdMan::getText(m_ContentMenu.iTelopNo + m_ContentMenu.iCursorCurrent));
            setContentStep(40);
        }
        break;
    case 40:
        if (!m_ContentMenu.pCursor) break;
        if (_isTrgOK(0)) {
            setContentStep(50);
            JiminyMemo::playSE(2);
            setNextContentMode(1 + m_ContentMenu.iCursorCurrent);
            if (m_ContentMenu.iCursorCurrent == 1) setContentParam(0, -1);
        } else if (_isTrgCANCEL(0) || _isButtonTrg(0x800, 0)) {
            JiminyMemo::playSE(4);
            setContentStep(50);
            setNextContentMode(-1);
        } else if (isButtonRptLimitStop(0x204000, 0,
                                        m_ContentMenu.iCursorCurrent == m_ContentMenu.iCursorPosiNum - 1)) {
            m_ContentMenu.iCursorCurrent++;
            if (m_ContentMenu.iCursorPosiNum <= m_ContentMenu.iCursorCurrent) m_ContentMenu.iCursorCurrent = 0;
            bCursorShift = TRUE;
        } else if (isButtonRptLimitStop(0x101000, 0, m_ContentMenu.iCursorCurrent == 0)) {
            m_ContentMenu.iCursorCurrent--;
            if (m_ContentMenu.iCursorCurrent < 0) m_ContentMenu.iCursorCurrent = m_ContentMenu.iCursorPosiNum - 1;
            bCursorShift = TRUE;
        }
        if (bCursorShift) {
            JiminyMemo::playSE(1);
            m_ContentMenu.pCursor->setPosiId(m_ContentMenu.iCursorCurrent);
            setTelopMessage(CCtdMan::getText(m_ContentMenu.iTelopNo + m_ContentMenu.iCursorCurrent));
        }
        break;
    case 50: {
        CMemo3DObj* pObj;
        if (m_ContentMenu.pList) {
            m_ContentMenu.pList->setOutState(m_ContentMenu.pList->getSideOutId(getNextContentMode() < 0));
        }
        if (m_ContentMenu.pTalk) {
            m_ContentMenu.pTalk->setOutState(m_ContentMenu.pTalk->getSideOutId(getNextContentMode() < 0));
        }
        if (m_ContentMenu.pCursor) m_ContentMenu.pCursor->setState(4);
        if (m_ContentMenu.pCursorLight) m_ContentMenu.pCursorLight->setOutState(2);
        if (m_ContentMenu.pSeqCtrl) changeStateOutSequence(m_ContentMenu.pSeqCtrl, m_ContentMenu.iSeqCtrlNum);
        pObj = getTaskPointer<CMemo3DObj*>(m_ContentMenu.hJiminyObj);
        if (pObj) pObj->setErase();
        setTelopMessage("");
        setContentStep(60);
        setContentTimer(16.0f);
        break;
    }
    case 60:
        if (getContentTimer() <= 0.0f) setContentStep(80);
        break;
    case 80:
    case 90: {
        CMemo3DObj* pObj = getTaskPointer<CMemo3DObj*>(m_ContentMenu.hJiminyObj);
        if (pObj) pObj->out();
        if (m_ContentMenu.pList) delete m_ContentMenu.pList;
        m_ContentMenu.pList = NULL;
        if (m_ContentMenu.pTalk) delete m_ContentMenu.pTalk;
        m_ContentMenu.pTalk = NULL;
        if (m_ContentMenu.pCursor) delete m_ContentMenu.pCursor;
        m_ContentMenu.pCursor = NULL;
        if (m_ContentMenu.pCursorLight) delete m_ContentMenu.pCursorLight;
        m_ContentMenu.pCursorLight = NULL;
        if (m_ContentMenu.pString) {
            delete[] m_ContentMenu.pString;
            m_ContentMenu.pString = NULL;
        }
        if (m_ContentMenu.pSeqCtrl) {
            for (INT i = 0; m_ContentMenu.iSeqCtrlNum > i; i++) {
                __typeof__(m_ContentMenu.pSeqCtrl[i]) pt87 = m_ContentMenu.pSeqCtrl[i]; if (m_ContentMenu.pSeqCtrl[i]) delete pt87;
                m_ContentMenu.pSeqCtrl[i] = NULL;
            }
            if (m_ContentMenu.pSeqCtrl) delete[] m_ContentMenu.pSeqCtrl;
            m_ContentMenu.pSeqCtrl = NULL;
        }
        setContentStep(100);
        break;
    }
    case 100:
        break;
    }
    FLOAT rate = getFrameRateF();
    if (m_ContentMenu.pList) m_ContentMenu.pList->main(rate);
    if (m_ContentMenu.pTalk) m_ContentMenu.pTalk->main(rate);
    if (m_ContentMenu.pCursor) {
        m_ContentMenu.pCursor->main();
        if (m_ContentMenu.pCursorLight) {
            if (bCursorShift == TRUE) m_ContentMenu.pCursorLight->setSlideMoveParam(m_ContentMenu.pCursor);
            m_ContentMenu.pCursorLight->main(rate);
        }
    }
}
#else
/* placeholder: rodata of contentMenu__11CJiminyMemo */
__asm__(".rdata\n");
inline void _roPh25() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE(__FILE__ "(5252)");
    TRACE(__FILE__ "(5258)");
    TRACE(__FILE__ "(5350)");
    TRACE(__FILE__ "(5359)");
    TRACE(__FILE__ "(5377)");
}
__asm__(".rdata\n.align 4\n.word .L0032AFAC\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032B520\n.word .L0032B794\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n");
__asm__(".word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032B830\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032B9B4\n.word .L0032BC08\n");
__asm__(".word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BA78\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n");
__asm__(".word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BA9C\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n");
__asm__(".word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BA9C\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n.word .L0032BC08\n");
__asm__(".word .L0032BC08\n.byte 0x00,0x00,0x00,0x00\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", contentMenu__11CJiminyMemo);
#endif
__asm__(".rdata\n"); /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
namespace JiminyMemo {
namespace Story {
extern INT NoteSeqData[5];
extern CursorPosition CursorPosiOffset;
extern FVECTOR NoteScrollBarPosi;
}
namespace StoryView {
extern const SHORT WorldPictureTbl[17];
namespace D {
extern const SHORT WorldPictureTbl[6];
}
}
}
void CJiminyMemo::contentStory() {
    INT eventFlagId = isModeJiminyMemo() ? 0 : 0x11;
    BOOL bCursorShift = FALSE;
    switch (getContentStep()) {
    case 20: {
        INT posiNum;
        INT listNum;
        INT ctNo;
        FVECTOR posi;
        D2::C2DSeqCtrl** pSeqCtrl;
        D2::C2DFnt** pString;
        CMemo2DSequence* pList;
        if (isModeJiminyMemo()) {
            posiNum = 8;
            listNum = 17;
            ctNo = 0x700401;
        } else {
            posiNum = 6;
            listNum = 6;
            ctNo = 0x700412;
        }
        m_ContentStory.iNoteListNum = listNum;
        m_ContentStory.iCursorPosiNum = posiNum;
        m_ContentStory.iCTCodeOffset = ctNo;
        m_ContentStory.iStringNum = posiNum;
        pString = new (__FILE__ "(5569)") D2::C2DFnt*[m_ContentStory.iStringNum];
        if (pString) {
            for (INT i = 0; i < m_ContentStory.iStringNum; i++) pString[i] = NULL;
            m_ContentStory.pString = pString;
        }
        m_ContentStory.iSeqCtrlNum = m_ContentStory.iStringNum + listNum + 3;
        pSeqCtrl = new (__FILE__ "(5576)") D2::C2DSeqCtrl*[m_ContentStory.iSeqCtrlNum];
        if (pSeqCtrl) {
            for (INT i = 0; i < m_ContentStory.iSeqCtrlNum; i++) pSeqCtrl[i] = NULL;
            m_ContentStory.pSeqCtrl = pSeqCtrl;
        }
        pList = createSequence(JiminyMemo::Story::NoteSeqData, 0x10, 0xB, 0, NULL);
        if (pList) {
            m_ContentStory.pList = pList;
            pList->setInState(pList->getSideInId(getPrevContentMode() == 0));
            for (INT i = 0; i < posiNum; i++) {
                createListSequence(pList, 0x12, 0x10, i + 1, pSeqCtrl++, pString++, 0x8049481F, 16, 0x21000,
                                   CCtdMan::getText(ctNo + i));
            }
        }
        m_ContentStory.iCursorCurrent = getPrevContentMode() == 6 ? getContentParam(0) : 0;
        m_ContentStory.pCursor = createCursorSequence((const INT*)JiminyMemo::CursorSeqTbl, 0x10, 0x11,
                                                      JiminyMemo::Story::CursorPosiOffset, posiNum, listNum, 0, 0, 1,
                                                      0x11, 0x20002, m_ContentStory.iCursorCurrent, pList);
        if (m_ContentStory.pCursor && getPrevContentMode()) {
            getContentScroll(0, posi);
            m_ContentStory.pCursor->setShiftVector(posi);
            m_ContentStory.pCursor->setPosiId(m_ContentStory.iCursorCurrent);
        }
        createMarkSequence(0x93, listNum, -1, 0, 0x11, pList, pSeqCtrl);
        for (INT i = 0; i < listNum; i++) {
            pSeqCtrl[i]->enableDisp(FALSE);
            if (pSeqCtrl[i]) {
                if (!CJiminyMemoFlag::isContentFlag(eventFlagId + i, 4) &&
                    CJiminyMemoFlag::isContentFlag(eventFlagId + i, 2)) {
                    pSeqCtrl[i]->replaceSeq(1, m_pLadName, 0x94);
                }
                pSeqCtrl[i]->enableDisp(CJiminyMemoFlag::isContentNew(eventFlagId + i) != 0);
            }
        }
        m_ContentStory.pNewMarkSeq = pSeqCtrl;
        pSeqCtrl += listNum;
        ScrollBarInit(pSeqCtrl, 0x10, JiminyMemo::Story::NoteScrollBarPosi, 150.0f, pList);
        m_ContentStory.pScrollBar = pSeqCtrl;
        if (m_ContentStory.pCursor) {
            CursorPosition cp;
            m_ContentStory.pCursor->getSubVector(cp, 1, 0);
            m_ContentStory.fCursorIntervalY = cp.y;
        }
        if (getPlateState(0) == -1) {
            setPlateState(0, 0);
            setPlateText(0, 0x700201);
        }
        bCursorShift = TRUE;
        setContentStep(30);
        setContentTimer(16.0f);
        break;
    }
    case 30:
        if (getContentTimer() <= 0.0f) {
            if (m_ContentStory.pSeqCtrl) changeStateLoopSequence(m_ContentStory.pSeqCtrl, m_ContentStory.iSeqCtrlNum);
            setTelopMessage(CCtdMan::getText(isModeJiminyMemo() ? 0x700106 : 0x70011A));
            setContentStep(40);
        }
        break;
    case 40:
        if (!m_ContentStory.pCursor) break;
        if (_isButtonTrg(0x800, 0)) {
            setContentStep(50);
            JiminyMemo::playSE(4);
            setNextContentMode(-1);
        } else if (_isTrgOK(0)) {
            if (CJiminyMemoFlag::isContentOpen(eventFlagId + m_ContentStory.iCursorCurrent)) {
                setContentStep(50);
                setNextContentMode(6);
                setContentParam(0, m_ContentStory.iCursorCurrent);
                setContentScroll(0, m_ContentStory.pCursor->getShiftVector());
                JiminyMemo::playSE(2);
            } else {
                JiminyMemo::playSE(5);
            }
        } else if (_isTrgCANCEL(0)) {
            setNextContentMode(0);
            setContentStep(50);
            JiminyMemo::playSE(4);
        } else if (isButtonRptLimitStop(0x204000, 0,
                                        m_ContentStory.iCursorCurrent == m_ContentStory.iNoteListNum - 1)) {
            m_ContentStory.iCursorCurrent++;
            if (m_ContentStory.iCursorCurrent >= m_ContentStory.iNoteListNum) {
                m_ContentStory.iCursorCurrent = 0;
            }
            bCursorShift = TRUE;
        } else if (isButtonRptLimitStop(0x101000, 0, m_ContentStory.iCursorCurrent == 0)) {
            m_ContentStory.iCursorCurrent--;
            if (m_ContentStory.iCursorCurrent < 0) m_ContentStory.iCursorCurrent = m_ContentStory.iNoteListNum - 1;
            bCursorShift = TRUE;
        } else if (_isButtonTrg(8, 0)) {
            nextPage(m_ContentStory.pCursor, &m_ContentStory.iCursorCurrent, m_ContentStory.iNoteListNum,
                     m_ContentStory.iCursorPosiNum, m_ContentStory.fCursorIntervalY);
            bCursorShift = TRUE;
        } else if (_isButtonTrg(4, 0)) {
            prevPage(m_ContentStory.pCursor, &m_ContentStory.iCursorCurrent, m_ContentStory.iNoteListNum,
                     m_ContentStory.iCursorPosiNum, m_ContentStory.fCursorIntervalY);
            bCursorShift = TRUE;
        }
        if (bCursorShift) {
            m_ContentStory.pCursor->setPosiId(m_ContentStory.iCursorCurrent);
            JiminyMemo::playSE(1);
        }
        break;
    case 50: {
        INT nextContent;
        if (m_ContentStory.pList) {
            m_ContentStory.pList->setOutState(m_ContentStory.pList->getSideOutId(getNextContentMode() == 0));
        }
        if (m_ContentStory.pCursor) m_ContentStory.pCursor->setState(4);
        if (m_ContentStory.pSeqCtrl) changeStateOutSequence(m_ContentStory.pSeqCtrl, m_ContentStory.iSeqCtrlNum);
        nextContent = getNextContentMode();
        if (nextContent == 6) {
            USHORT rsrcId;
            if (isModeJiminyMemo()) {
                INT flag = 0;
                if (m_ContentStoryView.iWorldNo == 13) flag = JiminyMemo::is100AcreGoodEnding() ? 0 : 5;
                rsrcId = JiminyMemo::StoryView::WorldPictureTbl[m_ContentStory.iCursorCurrent] + flag + 180;
            } else {
                rsrcId = JiminyMemo::StoryView::D::WorldPictureTbl[m_ContentStory.iCursorCurrent] + 180;
            }
            CRcf::reqLoadRCFFront(2, rsrcId, 0, NULL, 0);
        } else if (nextContent == 0) {
            setPlateState(0, 2);
        } else if (nextContent != -1) {
            setPlateState(0, 2);
        }
        setTelopMessage("");
        setContentStep(60);
        setContentTimer(16.0f);
        break;
    }
    case 60:
        if (getContentTimer() <= 0.0f) setContentStep(80);
        break;
    case 80:
    case 90:
        if (m_ContentStory.pList) delete m_ContentStory.pList;
        m_ContentStory.pList = NULL;
        if (m_ContentStory.pCursor) delete m_ContentStory.pCursor;
        m_ContentStory.pCursor = NULL;
        if (m_ContentStory.pString) {
            delete[] m_ContentStory.pString;
            m_ContentStory.pString = NULL;
        }
        if (m_ContentStory.pSeqCtrl) {
            for (INT i = 0; i < m_ContentStory.iSeqCtrlNum; i++) {
                if (m_ContentStory.pSeqCtrl[i]) delete m_ContentStory.pSeqCtrl[i];
                m_ContentStory.pSeqCtrl[i] = NULL;
            }
            if (m_ContentStory.pSeqCtrl) delete[] m_ContentStory.pSeqCtrl;
            m_ContentStory.pSeqCtrl = NULL;
        }
        setContentStep(100);
        break;
    case 100:
        break;
    }
    if (m_ContentStory.pList) m_ContentStory.pList->main(getFrameRateF());
    if (m_ContentStory.pCursor) m_ContentStory.pCursor->main();
    if (bCursorShift) {
        if (m_ContentStory.pCursor && m_ContentStory.pSeqCtrl) {
            INT top = (INT)m_ContentStory.pCursor->getShiftVector().y / (INT)m_ContentStory.fCursorIntervalY;
            replaceNoteListWithOpenCheck(m_ContentStory.pString, (CT_LOWER)m_ContentStory.iCTCodeOffset, eventFlagId,
                                         m_ContentStory.iCursorPosiNum, m_ContentStory.iNoteListNum, top);
            replaceNoteListMark(m_ContentStory.pNewMarkSeq, eventFlagId, m_ContentStory.iCursorPosiNum,
                                m_ContentStory.iNoteListNum, top, 0x10, 9);
            ScrollBarChange(m_ContentStory.pScrollBar, JiminyMemo::Story::NoteScrollBarPosi.y, 150.0f,
                            m_ContentStory.iCursorPosiNum, m_ContentStory.iNoteListNum, top);
        }
    }
}
namespace JiminyMemo {
extern CHAR* StoryLadName[13];
namespace StoryView {
extern INT NoteSeqData[5];
}
}
void CJiminyMemo::contentStoryView() {
    INT eventFlagId = isModeDReport() ? 0x11 : 0;
    switch (getContentStep()) {
    case 20: {
        USHORT groupId;
        USHORT rsrcId;
        m_ContentStoryView.iWorldNo = getContentParam(0);
        if (isModeJiminyMemo()) {
            INT flag = 0;
            if (m_ContentStoryView.iWorldNo == 13) flag = JiminyMemo::is100AcreGoodEnding() ? 0 : 5;
            rsrcId = JiminyMemo::StoryView::WorldPictureTbl[m_ContentStoryView.iWorldNo] + flag + 180;
        } else {
            rsrcId = JiminyMemo::StoryView::D::WorldPictureTbl[m_ContentStoryView.iWorldNo] + 180;
        }
        groupId = 2;
        if (!CRsrcDB::hasRsrcGroup(groupId, rsrcId)) CRcf::reqLoadRCFFront(groupId, rsrcId, 0, NULL, 0);
        m_ContentStoryView.iGroupId = groupId;
        m_ContentStoryView.iRsrcId = rsrcId;
        setContentStep(21);
        break;
    }
    case 21: {
        INT ctNo0;
        INT ctNo1;
        INT storyNum;
        INT pictureNo;
        FVECTOR posi;
        D2::C2DSeqCtrl** pSeqCtrl;
        D2::C2DFnt** pString;
        CMemo2DSequence* pList;
        if (!CRcf::isCompleteLoadRCF(m_ContentStoryView.iGroupId, m_ContentStoryView.iRsrcId)) break;
        if (isModeJiminyMemo()) {
            ctNo0 = 0x700401;
            ctNo1 = 0x700501;
            storyNum = 0x11;
            pictureNo = JiminyMemo::StoryView::WorldPictureTbl[m_ContentStoryView.iWorldNo];
        } else {
            ctNo0 = 0x700412;
            ctNo1 = 0x700512;
            storyNum = 6;
            pictureNo = JiminyMemo::StoryView::D::WorldPictureTbl[m_ContentStoryView.iWorldNo];
        }
        m_ContentStoryView.iAllStoryNum = storyNum;
        m_ContentStoryView.iPictureNo = pictureNo;
        m_ContentStoryView.iStringNum = 8;
        pString = new (__FILE__ "(5890)") D2::C2DFnt*[m_ContentStoryView.iStringNum];
        if (pString) {
            for (INT i = 0; i < m_ContentStoryView.iStringNum; i++) pString[i] = NULL;
            m_ContentStoryView.pString = pString;
        }
        m_ContentStoryView.iSeqCtrlNum = 11;
        pSeqCtrl = new (__FILE__ "(5897)") D2::C2DSeqCtrl*[m_ContentStoryView.iSeqCtrlNum];
        if (pSeqCtrl) {
            for (INT i = 0; i < m_ContentStoryView.iSeqCtrlNum; i++) pSeqCtrl[i] = NULL;
            m_ContentStoryView.pSeqCtrl = pSeqCtrl;
        }
        splitMessage(&m_ContentStoryView.iTextLineNum, &m_ContentStoryView.pTextLineBuf,
                     CCtdMan::getText(ctNo1 + m_ContentStoryView.iWorldNo), 14);
        pList = createSequence(JiminyMemo::StoryView::NoteSeqData, 0x10, 0xB, 0, NULL);
        if (pList) {
            m_ContentStoryView.pList = pList;
            pList->setInState(0);
            for (INT i = 0; i < 8; i++) {
                createListSequence(pList, 0x11, 0x19, i + 1, pSeqCtrl++, pString++, 0x8049481F, 16, 0x21000,
                                   m_ContentStoryView.pTextLineBuf[i]);
            }
        }
        if (pList) {
            createMarkSequence(0xD0, 1, 0x19, 0xB, 0x11, pList, pSeqCtrl);
            m_ContentStoryView.pTopArrowSeq = *pSeqCtrl++;
            createMarkSequence(0xC3, 1, 0x19, 9, 0x11, pList, pSeqCtrl);
            m_ContentStoryView.pBottomArrowSeq = *pSeqCtrl++;
            if (isModeJiminyMemo() && m_ContentStoryView.iWorldNo == 13) {
                INT flag = JiminyMemo::is100AcreGoodEnding() ? 1 : 2;
                sprintf(m_ContentStoryView.pPictFileName, "%s%02d.lad", JiminyMemo::StoryLadName[pictureNo], flag);
                sprintf(m_ContentStoryView.pPictSeqName, "%s%02d:00", JiminyMemo::StoryLadName[pictureNo], flag);
            } else {
                sprintf(m_ContentStoryView.pPictFileName, "%s.lad", JiminyMemo::StoryLadName[pictureNo]);
                sprintf(m_ContentStoryView.pPictSeqName, "%s:00", JiminyMemo::StoryLadName[pictureNo]);
            }
            D2::C2DSeqCtrl* pPictSeq = new (__FILE__ "(5949)") D2::C2DSeqCtrl;
            if (pPictSeq) {
                *pSeqCtrl = pPictSeq;
                m_ContentStoryView.pPictSeq = pPictSeq;
                pPictSeq->setLayer(0x11);
                pPictSeq->enableDisp(TRUE);
                pPictSeq->addSeq(m_ContentStoryView.pPictSeqName, 0);
                D2::C2DMan::getLayPosition(m_ContentStoryView.pPictFileName, 0, 0, posi, 0);
                posi.x += 12.0f;
                posi.y += 16.0f;
                pPictSeq->setPos(posi);
                pPictSeq->setScl(0.875f);
            }
        }
        if (getPlateState(1) == -1) setPlateState(1, 0);
        setPlateText(1, ctNo0 + m_ContentStoryView.iWorldNo);
        if (getPlateState(3) == -1) setPlateState(3, 0);
        setPlateNumSeq(7, m_ContentStoryView.iWorldNo + 1);
        setPlateNumSeq(8, storyNum);
        setContentStep(30);
        setContentTimer(16.0f);
        break;
    }
    case 30:
        if (m_ContentStoryView.pPictSeq->isEnd(0)) {
            m_ContentStoryView.pPictSeq->replaceSeq(0, m_ContentStoryView.pPictSeqName, 1);
            setContentStep(31);
        }
    case 31:
        if (getContentTimer() <= 0.0f) {
            if (m_ContentStoryView.pSeqCtrl) {
                changeStateLoopSequence(m_ContentStoryView.pSeqCtrl, m_ContentStoryView.iSeqCtrlNum);
            }
            CJiminyMemoFlag::setContentTouchFlag(eventFlagId + m_ContentStoryView.iWorldNo);
            m_ContentStoryView.pPictSeq->replaceSeq(0, m_ContentStoryView.pPictSeqName, 1);
            m_ContentStoryView.iPrevNo = getPrevStoryId(eventFlagId);
            m_ContentStoryView.iNextNo = getNextStoryId(eventFlagId);
            setContentStep(40);
        }
        break;
    case 40: {
        BOOL bTouch = FALSE;
        if (_isButtonTrg(0x800, 0)) {
            setContentStep(50);
            JiminyMemo::playSE(4);
            setNextContentMode(-1);
        } else if (_isTrgCANCEL(0)) {
            setContentStep(50);
            setNextContentMode(1);
            JiminyMemo::playSE(4);
        } else if (m_ContentStoryView.iPrevNo >= 0 && _isButtonTrg(1, 0)) {
            setContentStep(50);
            setNextContentMode(6);
            setContentParam(0, m_ContentStoryView.iPrevNo);
            JiminyMemo::playSE(2);
        } else if (m_ContentStoryView.iNextNo >= 0 && _isButtonTrg(2, 0)) {
            setContentStep(50);
            setNextContentMode(6);
            setContentParam(0, m_ContentStoryView.iNextNo);
            JiminyMemo::playSE(2);
        } else if (_isButtonRpt(0x204000, 0)) {
            if (m_ContentStoryView.iCurrentTextLine < m_ContentStoryView.iTextLineNum - 8) {
                m_ContentStoryView.iCurrentTextLine++;
                bTouch = TRUE;
            }
        } else if (_isButtonRpt(0x101000, 0)) {
            if (m_ContentStoryView.iCurrentTextLine > 0) {
                m_ContentStoryView.iCurrentTextLine--;
                bTouch = TRUE;
            }
        } else if (_isButtonTrg(8, 0)) {
            bTouch = nextPage(&m_ContentStoryView.iCurrentTextLine, m_ContentStoryView.iTextLineNum, 8);
        } else if (_isButtonTrg(4, 0)) {
            bTouch = prevPage(&m_ContentStoryView.iCurrentTextLine, 0, 8);
        }
        if (bTouch == TRUE) {
            JiminyMemo::playSE(1);
            replaceNoteList(m_ContentStoryView.pString,
                            (const CHAR**)m_ContentStoryView.pTextLineBuf + m_ContentStoryView.iCurrentTextLine, 8);
        }
        break;
    }
    case 50: {
        INT nextContent;
        if (m_ContentStoryView.pList) m_ContentStoryView.pList->setOutState(2);
        if (m_ContentStoryView.pSeqCtrl) {
            changeStateOutSequence(m_ContentStoryView.pSeqCtrl, m_ContentStoryView.iSeqCtrlNum);
        }
        m_ContentStoryView.pPictSeq->replaceSeq(0, m_ContentStoryView.pPictSeqName, 2);
        nextContent = getNextContentMode();
        if (nextContent == 1) {
            setPlateState(1, 2);
            setPlateState(3, 2);
        } else if (nextContent == -1) {
            setPlateState(0, 2);
            setPlateState(1, 2);
            setPlateState(3, 2);
        }
        setContentStep(60);
        setContentTimer(16.0f);
        break;
    }
    case 60:
        if (getContentTimer() <= 0.0f) setContentStep(80);
        break;
    case 80:
    case 90:
        if (m_ContentStoryView.pList) delete m_ContentStoryView.pList;
        m_ContentStoryView.pList = NULL;
        if (m_ContentStoryView.pString) {
            delete[] m_ContentStoryView.pString;
            m_ContentStoryView.pString = NULL;
        }
        if (m_ContentStoryView.pSeqCtrl) {
            for (INT i = 0; i < m_ContentStoryView.iSeqCtrlNum; i++) {
                if (m_ContentStoryView.pSeqCtrl[i]) delete m_ContentStoryView.pSeqCtrl[i];
                m_ContentStoryView.pSeqCtrl[i] = NULL;
            }
            if (m_ContentStoryView.pSeqCtrl) delete[] m_ContentStoryView.pSeqCtrl;
            m_ContentStoryView.pSeqCtrl = NULL;
            m_ContentStoryView.pBottomArrowSeq = NULL;
            m_ContentStoryView.pTopArrowSeq = NULL;
            m_ContentStoryView.pPictSeq = NULL;
        }
        if (m_ContentStoryView.pTextLineBuf) {
            for (INT i = 0; i < m_ContentStoryView.iTextLineNum; i++) {
                if (m_ContentStoryView.pTextLineBuf[i]) com_free(m_ContentStoryView.pTextLineBuf[i]);
                m_ContentStoryView.pTextLineBuf[i] = NULL;
            }
            if (m_ContentStoryView.pTextLineBuf) com_free(m_ContentStoryView.pTextLineBuf);
            m_ContentStoryView.pTextLineBuf = NULL;
        }
        if (getNextContentMode() != 6 ||
            JiminyMemo::StoryView::WorldPictureTbl[getContentParam(0)] != m_ContentStoryView.iPictureNo) {
            CRsrcDB::removeGroup(m_ContentStoryView.iGroupId, m_ContentStoryView.iRsrcId);
        }
        setContentStep(100);
        break;
    case 100:
        break;
    }
    if (m_ContentStoryView.pList) m_ContentStoryView.pList->main(getFrameRateF());
    if (m_ContentStoryView.pBottomArrowSeq) {
        m_ContentStoryView.pBottomArrowSeq->enableDisp(m_ContentStoryView.iCurrentTextLine <
                                                       m_ContentStoryView.iTextLineNum - 8);
    }
    if (m_ContentStoryView.pTopArrowSeq) {
        m_ContentStoryView.pTopArrowSeq->enableDisp(m_ContentStoryView.iCurrentTextLine > 0);
    }
}
INT CJiminyMemo::getPrevStoryId(INT inId) {
    return getPrevContentId(inId, work(0x178), work(0x170));
}
INT CJiminyMemo::getNextStoryId(INT inId) {
    return getNextContentId(inId, work(0x178), work(0x170));
}
static inline void _setCard(CARD& c, USHORT kind, BYTE num) {
    c.m_nId = 0;
    c.m_nNumber2 = num;
    c.m_nKind2 = kind;
    c.m_nCategory20 = cardDataTbl_004EA4F8[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl_004EA4F8[c.m_nKind].nCategory1;
}
namespace JiminyMemo {
extern const INT SoraCardNumTbl[];
extern const INT RikuCardNumTbl[];
namespace Collect {
extern INT BaseSeqData[5];
extern INT NoteSeqData[5];
extern const INT LightSeqTbl[5];
extern const INT FrameSeqTbl[5];
extern const INT FlickerSeqTbl[5];
extern UINT CursorMoveFlag[];
extern UINT CursorMoveFlagWithSpecial[];
extern CursorPosition CursorMapStart;
extern CursorPosition CursorMapOffset;
extern CursorPosition CardMapOffset;
namespace D {
extern UINT CursorMoveFlag[];
extern CursorPosition CursorMapStart;
extern CursorPosition CardMapOffset;
}
}
}
void CJiminyMemo::contentCollection() {
    const INT eventFlagId = isModeJiminyMemo() ? 0xB7 : 0x14F;
    BOOL bCursorShift = FALSE;
    BOOL bCardShift = FALSE;
    FLOAT frameRate = getFrameRateF();
    switch (getContentStep()) {
    case 20: {
        INT currentStartNo;
        INT cursorLimitX;
        INT cursorLimitY;
        FLOAT defaultScale;
        INT allCardNum;
        const INT* pCardNumTbl;
        INT kindNum;
        UINT* pCardFlagTbl;
        SHORT** pCardEncyclTbl;
        CursorPosition* pCardMapOffset;
        FVECTOR posi;
        D2::C2DSeqCtrl** pSeqCtrl;
        D2::C2DFnt** pString;
        CMemo2DSequence* pBase;
        CMemo2DCard** pCard;
        CMemo2DSequence* pList;
        currentStartNo = getContentParam(0);
        if (isModeJiminyMemo()) {
            if (currentStartNo < 0) currentStartNo = 1;
            cursorLimitX = 0x16;
            cursorLimitY = 7;
            defaultScale = 0.25f;
            allCardNum = 0x98;
            pCardNumTbl = JiminyMemo::SoraCardNumTbl;
            pCardFlagTbl = haveSpecialCard() ? JiminyMemo::Collect::CursorMoveFlagWithSpecial
                                             : JiminyMemo::Collect::CursorMoveFlag;
            pCardEncyclTbl = JiminyMemo::CardEncyclTbl;
            kindNum = 10;
            pCardMapOffset = &JiminyMemo::Collect::CardMapOffset;
        } else {
            if (currentStartNo < 0) currentStartNo = 0;
            cursorLimitX = 0x16;
            cursorLimitY = 3;
            defaultScale = 0.25f;
            allCardNum = 0x3B;
            kindNum = 4;
            pCardNumTbl = JiminyMemo::RikuCardNumTbl;
            pCardFlagTbl = JiminyMemo::Collect::D::CursorMoveFlag;
            pCardEncyclTbl = JiminyMemo::RikuCardEncyclTbl;
            pCardMapOffset = &JiminyMemo::Collect::D::CardMapOffset;
        }
        m_ContentCollect.iCursorLimitX = cursorLimitX;
        m_ContentCollect.iCursorLimitY = cursorLimitY;
        m_ContentCollect.fDefaultScale = defaultScale;
        m_ContentCollect.sCursorPosiX = currentStartNo % cursorLimitX;
        m_ContentCollect.sCursorPosiY = currentStartNo / cursorLimitX;
        m_ContentCollect.fScale = defaultScale;
        m_ContentCollect.iStringNum = 6;
        pString = new (__FILE__ "(6227)") D2::C2DFnt*[m_ContentCollect.iStringNum];
        if (pString) {
            for (INT i = 0; i < m_ContentCollect.iStringNum; i++) pString[i] = NULL;
            m_ContentCollect.pString = pString;
        }
        m_ContentCollect.iSeqCtrlNum = m_ContentCollect.iStringNum + 1;
        pSeqCtrl = new (__FILE__ "(6234)") D2::C2DSeqCtrl*[m_ContentCollect.iSeqCtrlNum];
        if (pSeqCtrl) {
            for (INT i = 0; i < m_ContentCollect.iSeqCtrlNum; i++) pSeqCtrl[i] = NULL;
            m_ContentCollect.pSeqCtrl = pSeqCtrl;
        }
        pBase = createSequence(JiminyMemo::Collect::BaseSeqData, 0xF, 0x28, 0, NULL);
        if (pBase) {
            m_ContentCollect.pBase = pBase;
            pBase->setInState(0);
            createMarkSequence(0xD7, 2, 0x2A, 1, 0x12, pBase, pSeqCtrl);
            pSeqCtrl += 2;
            createListSequence(pBase, 0x12, 0x2A, 3, pSeqCtrl++, pString++, 0x807F7F7F, 0x10, 0x42,
                               CCtdMan::getText(0x78000F));
            createListSequence(pBase, 0x12, 0x2A, 5, pSeqCtrl++, pString++, 0x807F7F7F, 0x10, 0x40,
                               CCtdMan::getText(0x700601));
        }
        m_ContentCollect.iCardNum = allCardNum;
        pCard = new (__FILE__ "(6280)") CMemo2DCard*[m_ContentCollect.iCardNum];
        if (pCard) {
            for (INT i = 0; i < m_ContentCollect.iCardNum; i++) pCard[i] = NULL;
            m_ContentCollect.pCard = pCard;
        }
        if (pBase && pCard) {
            INT no = 0;
            FVECTOR offset;
            for (INT kind = 0; kind < kindNum; kind++) {
                for (INT i = 0; i < pCardNumTbl[kind]; i++) {
                    if (CJiminyMemoFlag::isContentOpen(eventFlagId + no)) {
                        pCard[no] = new (__FILE__ "(6294)") CMemo2DCard(m_pLadName);
                        if (pCard[no]) {
                            CARD card;
                            pCard[no]->setLayer(0xF);
                            pCard[no]->enableColorCtrl(TRUE);
                            pCard[no]->setParent(pBase);
                            _setCard(card, pCardEncyclTbl[kind][i], 0xFF);
                            pCard[no]->create(card, 1);
                        }
                    }
                    no++;
                }
            }
            no = 0;
            offset.set(cursorLimitX * 64.0f * -0.5f + pCardMapOffset->x,
                       cursorLimitY * 96.0f * -0.5f + pCardMapOffset->y, 0.0f);
            for (INT y = 0; y < cursorLimitY; y++) {
                for (INT x = 0; x < cursorLimitX; x++) {
                    if (pCardFlagTbl[y] & (1 << x)) {
                        if (pCard[no]) {
                            posi.set(x * 64.0f + offset.x, y * 96.0f + offset.y, 0.0f);
                            pCard[no]->setScale(defaultScale);
                            pCard[no]->setBasePosition(posi);
                            pCard[no]->setLocalPosition();
                            pCard[no]->setState(1);
                        }
                        no++;
                    }
                }
            }
            bCardShift = TRUE;
        }
        if (pBase) {
            INT mode = getLoadResourceIndex();
            FLOAT percentage = CJiminyMemoFlag::getCardCollectionPercentage(mode, FALSE);
            createListSequence(pBase, 0x12, 0x2A, 4, pSeqCtrl++, pString++, 0x807F7F7F, 0xC, 0x42,
                               CJiminyMemoFlag::getCollecterRankName(mode, percentage));
            sprintf(m_ContentCollect.Percentage, "%3d%s", (INT)(percentage * 100.0f), CCtdMan::getText(0x700602));
            createListSequence(pBase, 0x12, 0x2A, 6, pSeqCtrl++, pString, 0x807F7F7F, 0x11, 0x41,
                               m_ContentCollect.Percentage);
        }
        if (pSeqCtrl) {
            createMarkSequence(0x8B, 1, 0x2B, 0, 0x10, NULL, pSeqCtrl);
            if (*pSeqCtrl) {
                m_ContentCollect.pMaskFG = *pSeqCtrl;
                (*pSeqCtrl)->enableColorCtrl(FALSE);
                (*pSeqCtrl)->enableDisp(FALSE);
            }
        }
        pList = createSequence(JiminyMemo::Collect::NoteSeqData, 0x11, 0x28, 0, NULL);
        if (pList) {
            m_ContentCollect.pList = pList;
            pList->setInState(1);
            pList->enableDisp(FALSE);
        }
        if (getPlateState(0) == -1) setPlateState(0, 0);
        setPlateText(0, 0x700202);
        if (getPlateState(2) == -1) setPlateState(2, 0);
        setPlateText(2, 0x700001);
        bCursorShift = TRUE;
        setContentTimer(16.0f);
        setContentStep(30);
        break;
    }
    case 30:
        if (getContentTimer() <= 0.0f) {
            CMemo2DSequence* pBase;
            if (m_ContentCollect.pSeqCtrl) {
                changeStateLoopSequence(m_ContentCollect.pSeqCtrl, m_ContentCollect.iSeqCtrlNum);
            }
            if (m_ContentCollect.pMaskFG) m_ContentCollect.pMaskFG->enableDisp(TRUE);
            if (m_ContentCollect.pList) m_ContentCollect.pList->enableDisp(TRUE);
            pBase = m_ContentCollect.pBase;
            if (pBase) {
                CursorPosition* pCardMapOffset;
                CursorPosition* pMapStart;
                INT cursorLimitX = m_ContentCollect.iCursorLimitX;
                INT cursorLimitY = m_ContentCollect.iCursorLimitY;
                CursorPosition StartPosi;
                INT current;
                if (isModeJiminyMemo()) {
                    pCardMapOffset = &JiminyMemo::Collect::CardMapOffset;
                    pMapStart = &JiminyMemo::Collect::CursorMapStart;
                } else {
                    pCardMapOffset = &JiminyMemo::Collect::D::CardMapOffset;
                    pMapStart = &JiminyMemo::Collect::D::CursorMapStart;
                }
                current = m_ContentCollect.sCursorPosiX + m_ContentCollect.sCursorPosiY * (SHORT)cursorLimitX;
                StartPosi.x = pCardMapOffset->x + pMapStart->x;
                StartPosi.y = pCardMapOffset->y + pMapStart->y;
                m_ContentCollect.pCursor = createCollectionCursorSequence(
                    (const INT*)JiminyMemo::CursorSeqTbl, StartPosi, JiminyMemo::Collect::CursorMapOffset,
                    cursorLimitX * cursorLimitY, cursorLimitX * cursorLimitY, 0, current, cursorLimitX, 0x13,
                    0x20002, current, m_ContentCollect.fScale, pBase);
                m_ContentCollect.pFrame = createSequence(JiminyMemo::Collect::FrameSeqTbl[3], 0x12, 0, 0, pBase);
                if (m_ContentCollect.pFrame) {
                    m_ContentCollect.pFrame->setStateFlag(4, 0);
                    m_ContentCollect.pFrame->setLocalPosition(m_ContentCollect.pCursor->getPosition());
                }
                m_ContentCollect.pLight = createSequence(JiminyMemo::Collect::LightSeqTbl[3], 0x12, 0, 0, pBase);
                if (m_ContentCollect.pLight) {
                    m_ContentCollect.pLight->setLocalPosition(m_ContentCollect.pCursor->getPosition());
                }
                m_ContentCollect.pFlicker =
                    createSequence(JiminyMemo::Collect::FlickerSeqTbl[3], 0x12, 0, 0, pBase);
                if (m_ContentCollect.pFlicker) {
                    m_ContentCollect.pFlicker->setStateFlag(2, 0);
                    m_ContentCollect.pFlicker->setStateFlag(4, 0);
                    m_ContentCollect.pFlicker->setLocalPosition(m_ContentCollect.pCursor->getPosition());
                }
                bCursorShift = TRUE;
            }
            setTelopMessage(CCtdMan::getText(isModeJiminyMemo() ? 0x700107 : 0x70011B));
            setPlateState(6, 0);
            setContentStep(40);
        }
        break;
    case 40:
        if (_isButtonTrg(0x800, 0)) {
            setContentStep(50);
            JiminyMemo::playSE(4);
            setNextContentMode(-1);
        } else if (m_ContentCollect.pCursor) {
            if (_isTrgCANCEL(0)) {
                setNextContentMode(0);
                setContentStep(50);
                JiminyMemo::playSE(4);
            } else if (_isButtonTrg(0x80, 0)) {
                if (setParamFromCollectToCardView()) {
                    setContentStep(50);
                    setNextContentMode(8);
                    FVECTOR shift = { 0.0f, 0.0f, 0.0f, 1.0f };
                    setContentScroll(0, shift);
                    setContentScroll(1, shift);
                    JiminyMemo::playSE(2);
                } else {
                    JiminyMemo::playSE(5);
                }
            } else if (_isButtonRpt(0x204000, 0)) {
                SHORT cx = m_ContentCollect.sCursorPosiX;
                SHORT cy = m_ContentCollect.sCursorPosiY;
                while (TRUE) {
                    m_ContentCollect.sCursorPosiY++;
                    if (m_ContentCollect.sCursorPosiY >= m_ContentCollect.iCursorLimitY) {
                        if (_isButtonTrg(0x204000, 0)) {
                            m_ContentCollect.sCursorPosiX++;
                            if (m_ContentCollect.sCursorPosiX >= m_ContentCollect.iCursorLimitX) {
                                m_ContentCollect.sCursorPosiX = cx;
                                m_ContentCollect.sCursorPosiY = cy;
                                break;
                            }
                            m_ContentCollect.sCursorPosiY = 0;
                        } else {
                            m_ContentCollect.sCursorPosiX = cx;
                            m_ContentCollect.sCursorPosiY = cy;
                            break;
                        }
                    }
                    if (setParamFromCollectToCardView()) {
                        bCursorShift = TRUE;
                        break;
                    }
                }
            } else if (_isButtonRpt(0x101000, 0)) {
                SHORT cx = m_ContentCollect.sCursorPosiX;
                SHORT cy = m_ContentCollect.sCursorPosiY;
                while (TRUE) {
                    m_ContentCollect.sCursorPosiY--;
                    if (m_ContentCollect.sCursorPosiY < 0) {
                        if (_isButtonTrg(0x101000, 0)) {
                            m_ContentCollect.sCursorPosiX--;
                            if (m_ContentCollect.sCursorPosiX < 0) {
                                m_ContentCollect.sCursorPosiX = cx;
                                m_ContentCollect.sCursorPosiY = cy;
                                break;
                            }
                            m_ContentCollect.sCursorPosiY = m_ContentCollect.iCursorLimitY - 1;
                        } else {
                            m_ContentCollect.sCursorPosiX = cx;
                            m_ContentCollect.sCursorPosiY = cy;
                            break;
                        }
                    }
                    if (setParamFromCollectToCardView()) {
                        bCursorShift = TRUE;
                        break;
                    }
                }
            } else if (_isButtonRpt(0x802000, 0)) {
                SHORT cx = m_ContentCollect.sCursorPosiX;
                SHORT cy = m_ContentCollect.sCursorPosiY;
                while (TRUE) {
                    m_ContentCollect.sCursorPosiX++;
                    if (m_ContentCollect.sCursorPosiX >= m_ContentCollect.iCursorLimitX) {
                        if (_isButtonTrg(0x802000, 0)) {
                            m_ContentCollect.sCursorPosiY++;
                            if (m_ContentCollect.sCursorPosiY >= m_ContentCollect.iCursorLimitY) {
                                m_ContentCollect.sCursorPosiX = cx;
                                m_ContentCollect.sCursorPosiY = cy;
                                break;
                            }
                            m_ContentCollect.sCursorPosiX = 0;
                        } else {
                            m_ContentCollect.sCursorPosiX = cx;
                            m_ContentCollect.sCursorPosiY = cy;
                            break;
                        }
                    }
                    if (setParamFromCollectToCardView()) {
                        bCursorShift = TRUE;
                        break;
                    }
                }
            } else if (_isButtonRpt(0x408000, 0)) {
                SHORT cx = m_ContentCollect.sCursorPosiX;
                SHORT cy = m_ContentCollect.sCursorPosiY;
                while (TRUE) {
                    m_ContentCollect.sCursorPosiX--;
                    if (m_ContentCollect.sCursorPosiX < 0) {
                        if (_isButtonTrg(0x408000, 0)) {
                            m_ContentCollect.sCursorPosiY--;
                            if (m_ContentCollect.sCursorPosiY < 0) {
                                m_ContentCollect.sCursorPosiX = cx;
                                m_ContentCollect.sCursorPosiY = cy;
                                break;
                            }
                            m_ContentCollect.sCursorPosiX = m_ContentCollect.iCursorLimitX - 1;
                        } else {
                            m_ContentCollect.sCursorPosiX = cx;
                            m_ContentCollect.sCursorPosiY = cy;
                            break;
                        }
                    }
                    if (setParamFromCollectToCardView()) {
                        bCursorShift = TRUE;
                        break;
                    }
                }
            } else if (_isButtonTrg(0x40, 0)) {
                if (m_ContentCollect.fScale < 1.0f) {
                    setContentStep(41);
                    JiminyMemo::playSE(0xF);
                }
                if (m_ContentCollect.fScale > m_ContentCollect.fDefaultScale) {
                    setContentStep(43);
                    JiminyMemo::playSE(0xF);
                }
            }
            if (bCursorShift) {
                m_ContentCollect.pCursor->setPosiId(m_ContentCollect.sCursorPosiX +
                                                    m_ContentCollect.sCursorPosiY * m_ContentCollect.iCursorLimitX);
                JiminyMemo::playSE(0xE);
            }
        }
        break;
    case 41: {
        FVECTOR shift;
        m_ContentCollect.fZoomTimer = 0.0f;
        if (m_ContentCollect.pCursor) m_ContentCollect.pCursor->enableDisp(FALSE);
        if (m_ContentCollect.pLight) {
            m_ContentCollect.pLight->enableDisp(FALSE);
            m_ContentCollect.pLight->setStateFlag(4, 0);
        }
        if (m_ContentCollect.pFrame) m_ContentCollect.pFrame->enableDisp(FALSE);
        if (m_ContentCollect.pFlicker) m_ContentCollect.pFlicker->enableDisp(FALSE);
        shift = m_ContentCollect.pCursor->getShiftVector();
        m_ContentCollect.vShiftPosi.x = shift.x;
        m_ContentCollect.vShiftPosi.y = shift.y;
        m_ContentCollect.pCursor->setScale(1.0f);
        m_ContentCollect.pCursor->main();
        shift = m_ContentCollect.pCursor->getShiftVector();
        m_ContentCollect.vShiftVec.x = shift.x;
        m_ContentCollect.vShiftVec.y = shift.y;
        m_ContentCollect.pCursor->setScale(m_ContentCollect.fScale);
        setContentStep(42);
    }
    case 42: {
        FLOAT rate;
        m_ContentCollect.fZoomTimer += getFrameRateF();
        if (m_ContentCollect.fZoomTimer >= 40.0f) {
            m_ContentCollect.fZoomTimer = 40.0f;
            if (m_ContentCollect.pCursor) {
                m_ContentCollect.pCursor->enableDisp(TRUE);
                m_ContentCollect.pCursor->setSeqNo(0xED);
                m_ContentCollect.pCursor->setState(1);
            }
            if (m_ContentCollect.pLight) {
                m_ContentCollect.pLight->enableDisp(TRUE);
                m_ContentCollect.pLight->replaceSequence(JiminyMemo::Collect::LightSeqTbl[1]);
                m_ContentCollect.pLight->setStateFlag(0, 4);
            }
            if (m_ContentCollect.pFrame) {
                m_ContentCollect.pFrame->enableDisp(TRUE);
                m_ContentCollect.pFrame->replaceSequence(JiminyMemo::Collect::FrameSeqTbl[1]);
            }
            if (m_ContentCollect.pFlicker) m_ContentCollect.pFlicker->enableDisp(TRUE);
            setContentStep(40);
        }
        rate = 1.0f - powf(1.0f - m_ContentCollect.fZoomTimer / 40.0f, 2.5f);
        m_ContentCollect.fScale = (1.0f - m_ContentCollect.fDefaultScale) * rate + m_ContentCollect.fDefaultScale;
        if (m_ContentCollect.fZoomTimer < 40.0f) {
            CursorPosition shiftPosi = m_ContentCollect.vShiftPosi;
            CursorPosition shiftAdd = m_ContentCollect.vShiftVec;
            FVECTOR posi;
            shiftAdd.x = (shiftAdd.x - shiftPosi.x) * rate;
            shiftAdd.y = (shiftAdd.y - shiftPosi.y) * rate;
            shiftPosi.x += shiftAdd.x;
            shiftPosi.y += shiftAdd.y;
            posi.set(shiftPosi.x, shiftPosi.y, 0.0f);
            m_ContentCollect.pCursor->setShiftVector(posi);
        } else {
            FVECTOR centerPosi;
            centerPosi.set(m_ContentCollect.vShiftVec.x, m_ContentCollect.vShiftVec.y, 0.0f);
            m_ContentCollect.pCursor->setShiftVector(centerPosi);
        }
        bCardShift = TRUE;
        if (m_ContentCollect.pLight) m_ContentCollect.pLight->addMoveTimer(frameRate);
        break;
    }
    case 43: {
        FVECTOR shift;
        m_ContentCollect.fZoomTimer = 0.0f;
        if (m_ContentCollect.pCursor) m_ContentCollect.pCursor->enableDisp(FALSE);
        if (m_ContentCollect.pLight) {
            m_ContentCollect.pLight->enableDisp(FALSE);
            m_ContentCollect.pLight->setStateFlag(4, 0);
        }
        if (m_ContentCollect.pFrame) m_ContentCollect.pFrame->enableDisp(FALSE);
        if (m_ContentCollect.pFlicker) m_ContentCollect.pFlicker->enableDisp(FALSE);
        shift = m_ContentCollect.pCursor->getShiftVector();
        m_ContentCollect.vShiftPosi.x = shift.x;
        m_ContentCollect.vShiftPosi.y = shift.y;
        m_ContentCollect.vShiftVec.x = 0.0f;
        m_ContentCollect.vShiftVec.y = 0.0f;
        CardScreenSideClip(TRUE);
        setContentStep(44);
    }
    case 44: {
        FLOAT rate;
        m_ContentCollect.fZoomTimer += getFrameRateF();
        if (m_ContentCollect.fZoomTimer >= 40.0f) {
            m_ContentCollect.fZoomTimer = 40.0f;
            if (m_ContentCollect.pCursor) {
                m_ContentCollect.pCursor->enableDisp(TRUE);
                m_ContentCollect.pCursor->setSeqNo(0x80);
                m_ContentCollect.pCursor->setState(1);
            }
            if (m_ContentCollect.pLight) {
                m_ContentCollect.pLight->enableDisp(TRUE);
                m_ContentCollect.pLight->replaceSequence(JiminyMemo::Collect::LightSeqTbl[3]);
                m_ContentCollect.pLight->setStateFlag(0, 4);
            }
            if (m_ContentCollect.pFrame) {
                m_ContentCollect.pFrame->enableDisp(TRUE);
                m_ContentCollect.pFrame->replaceSequence(JiminyMemo::Collect::FrameSeqTbl[3]);
            }
            if (m_ContentCollect.pFlicker) m_ContentCollect.pFlicker->enableDisp(TRUE);
            setContentStep(40);
        }
        rate = powf(1.0f - m_ContentCollect.fZoomTimer / 40.0f, 2.5f);
        m_ContentCollect.fScale = (1.0f - m_ContentCollect.fDefaultScale) * rate + m_ContentCollect.fDefaultScale;
        if (m_ContentCollect.fZoomTimer < 40.0f) {
            CursorPosition shiftPosi = m_ContentCollect.vShiftPosi;
            CursorPosition shiftAdd = m_ContentCollect.vShiftVec;
            FVECTOR posi;
            shiftAdd.x = (shiftAdd.x - shiftPosi.x) * (1.0f - rate);
            shiftAdd.y = (shiftAdd.y - shiftPosi.y) * (1.0f - rate);
            shiftPosi.x += shiftAdd.x;
            shiftPosi.y += shiftAdd.y;
            posi.set(shiftPosi.x, shiftPosi.y, 0.0f);
            m_ContentCollect.pCursor->setShiftVector(posi);
        } else {
            FVECTOR centerPosi;
            centerPosi.set(m_ContentCollect.vShiftVec.x, m_ContentCollect.vShiftVec.y, 0.0f);
            m_ContentCollect.pCursor->setShiftVector(centerPosi);
        }
        bCardShift = TRUE;
        if (m_ContentCollect.pLight) m_ContentCollect.pLight->addMoveTimer(frameRate);
        break;
    }
    case 50:
        if (m_ContentCollect.pList) m_ContentCollect.pList->setOutState(2);
        if (m_ContentCollect.pBase) m_ContentCollect.pBase->setOutState(2);
        if (m_ContentCollect.pCursor) m_ContentCollect.pCursor->setState(4);
        if (m_ContentCollect.pLight) {
            m_ContentCollect.pLight->setOutState(2);
            m_ContentCollect.pLight->enableDisp(FALSE);
        }
        if (m_ContentCollect.pFrame) m_ContentCollect.pFrame->setOutState(2);
        if (m_ContentCollect.pFlicker) {
            m_ContentCollect.pFlicker->setOutState(2);
            m_ContentCollect.pFlicker->enableDisp(FALSE);
        }
        if (m_ContentCollect.pCard) {
            for (INT i = 0; i < m_ContentCollect.iCardNum; i++) {
                if (m_ContentCollect.pCard[i]) m_ContentCollect.pCard[i]->setState(3);
            }
        }
        if (m_ContentCollect.pSeqCtrl) {
            changeStateOutSequence(m_ContentCollect.pSeqCtrl, m_ContentCollect.iSeqCtrlNum);
        }
        if (m_ContentCollect.pMaskFG) m_ContentCollect.pMaskFG->enableDisp(FALSE);
        if (m_ContentCollect.pList) m_ContentCollect.pList->enableDisp(FALSE);
        if (getNextContentMode() != 8) {
            setPlateState(0, 2);
            setPlateState(2, 2);
        }
        if (m_ContentCollect.pMaskFG) m_ContentCollect.pMaskFG->enableDisp(FALSE);
        if (m_ContentCollect.pList) m_ContentCollect.pList->enableDisp(FALSE);
        setTelopMessage("");
        setPlateState(6, -1);
        CardScreenOutClip();
        setContentTimer(16.0f);
        setContentStep(60);
        break;
    case 60:
        if (getContentTimer() <= 0.0f) setContentStep(80);
        break;
    case 80:
    case 90:
        if (m_ContentCollect.pList) delete m_ContentCollect.pList;
        m_ContentCollect.pList = NULL;
        if (m_ContentCollect.pBase) delete m_ContentCollect.pBase;
        m_ContentCollect.pBase = NULL;
        if (m_ContentCollect.pCursor) delete m_ContentCollect.pCursor;
        m_ContentCollect.pCursor = NULL;
        if (m_ContentCollect.pLight) delete m_ContentCollect.pLight;
        m_ContentCollect.pLight = NULL;
        if (m_ContentCollect.pFrame) delete m_ContentCollect.pFrame;
        m_ContentCollect.pFrame = NULL;
        if (m_ContentCollect.pFlicker) delete m_ContentCollect.pFlicker;
        m_ContentCollect.pFlicker = NULL;
        if (m_ContentCollect.pString) {
            delete[] m_ContentCollect.pString;
            m_ContentCollect.pString = NULL;
        }
        if (m_ContentCollect.pSeqCtrl) {
            for (INT i = 0; i < m_ContentCollect.iSeqCtrlNum; i++) {
                if (m_ContentCollect.pSeqCtrl[i]) delete m_ContentCollect.pSeqCtrl[i];
                m_ContentCollect.pSeqCtrl[i] = NULL;
            }
            if (m_ContentCollect.pSeqCtrl) delete[] m_ContentCollect.pSeqCtrl;
            m_ContentCollect.pSeqCtrl = NULL;
        }
        if (m_ContentCollect.pCard) {
            for (INT i = 0; i < m_ContentCollect.iCardNum; i++) {
                if (m_ContentCollect.pCard[i]) delete m_ContentCollect.pCard[i];
                m_ContentCollect.pCard[i] = NULL;
            }
            if (m_ContentCollect.pCard) delete[] m_ContentCollect.pCard;
            m_ContentCollect.pCard = NULL;
        }
        setContentStep(100);
        break;
    case 100:
        break;
    }
    if (m_ContentCollect.pList) m_ContentCollect.pList->main(frameRate);
    if (m_ContentCollect.pBase) m_ContentCollect.pBase->main(frameRate);
    if (bCardShift == TRUE || bCursorShift == TRUE) {
        CMemo2DCursor* pCursor = m_ContentCollect.pCursor;
        if (pCursor) {
            FLOAT scale = m_ContentCollect.fScale;
            pCursor->setScale(scale);
            pCursor->main();
            if (m_ContentCollect.pLight) {
                m_ContentCollect.pLight->setScale(scale);
                m_ContentCollect.pLight->setSlideMoveParam(pCursor);
                m_ContentCollect.pLight->main(frameRate);
            }
            if (m_ContentCollect.pFrame) {
                m_ContentCollect.pFrame->setScale(scale);
                m_ContentCollect.pFrame->setSlideMoveParam(pCursor);
                m_ContentCollect.pFrame->main(frameRate);
            }
            if (m_ContentCollect.pFlicker) {
                m_ContentCollect.pFlicker->setScale(scale);
                m_ContentCollect.pFlicker->setSlideMoveParam(pCursor);
                m_ContentCollect.pFlicker->main(frameRate);
            }
            if (scale == 1.0f) CardScreenSideClip(FALSE);
        }
    } else if (m_ContentCollect.pCursor) {
        m_ContentCollect.pCursor->main();
        if (m_ContentCollect.pLight) m_ContentCollect.pLight->main(frameRate);
        if (m_ContentCollect.pFrame) m_ContentCollect.pFrame->main(frameRate);
        if (m_ContentCollect.pFlicker) m_ContentCollect.pFlicker->main(frameRate);
    }
    if (bCardShift == TRUE || bCursorShift == TRUE) {
        if (m_ContentCollect.pCard && m_ContentCollect.pCursor) {
            FLOAT scale;
            FVECTOR shift;
            FVECTOR shiftVec;
            if (m_ContentCollect.fScale < 1e-05f) m_ContentCollect.fScale = m_ContentCollect.fDefaultScale;
            shift = m_ContentCollect.pCursor->getShiftVector();
            scale = m_ContentCollect.fScale;
            shiftVec.x = -shift.x / scale;
            shiftVec.y = -shift.y / scale;
            for (INT i = 0; i < m_ContentCollect.iCardNum; i++) {
                if (m_ContentCollect.pCard[i]) {
                    m_ContentCollect.pCard[i]->setScale(scale);
                    m_ContentCollect.pCard[i]->setShiftPosition(shiftVec);
                    m_ContentCollect.pCard[i]->setLocalPosition();
                    m_ContentCollect.pCard[i]->main(frameRate);
                }
            }
        }
    } else if (m_ContentCollect.pCard) {
        for (INT i = 0; i < m_ContentCollect.iCardNum; i++) {
            if (m_ContentCollect.pCard[i]) m_ContentCollect.pCard[i]->main(frameRate);
        }
    }
}
void CJiminyMemo::CardScreenSideClip(BOOL inAllDisp) {
    CMemo2DCard* pCard;
    if (*(FLOAT*)&work(0x1F0) < 1.0f || inAllDisp == TRUE) {
        for (INT i = 0; i < work(0x1D8); i++) {
            pCard = ((CMemo2DCard**)work(0x1D4))[i];
            if (pCard && pCard->isEnableDisp(2) != TRUE) pCard->enableDisp(2, TRUE);
        }
        return;
    }
    FVECTOR shift;
    FVECTOR shiftVec;
    FLOAT limitL1;
    FLOAT limitR1;
    FLOAT limitL2;
    FLOAT limitR2;
    FLOAT limitU;
    FLOAT limitD;
    FVECTOR base;
    shift = ((CMemo2DCursor*)work(0x1B4))->getShiftVector();
    limitL1 = shift.x - 192.0f;
    limitR1 = shift.x + 192.0f;
    limitL2 = shift.x - 256.0f;
    limitR2 = shift.x + 256.0f;
    limitU = shift.y - 96.0f;
    limitD = shift.y + 96.0f;
    shiftVec.x = -shift.x / *(FLOAT*)&work(0x1F0);
    shiftVec.y = -shift.y / *(FLOAT*)&work(0x1F0);
    for (INT i = 0; i < work(0x1D8); i++) {
        pCard = ((CMemo2DCard**)work(0x1D4))[i];
        if (!pCard) continue;
        base = pCard->getBasePosition();
        if (limitU < base.y || base.y < limitD) {
            if ((base.x < limitL1 && limitL2 < base.x) || (limitR1 < base.x && base.x < limitR2)) {
                if (pCard->isEnableDisp(2)) pCard->enableDisp(2, FALSE);
            } else if (limitL1 < base.x && base.x < limitR1) {
                if (pCard->isEnableDisp(2) != TRUE) pCard->enableDisp(2, TRUE);
            }
        }
    }
}
void CJiminyMemo::CardScreenOutClip() {
    FVECTOR shift;
    FVECTOR shiftVec;
    FLOAT limitL1;
    FLOAT limitR1;
    FLOAT limitU1;
    FLOAT limitD1;
    FLOAT limitL2;
    FLOAT limitR2;
    FLOAT limitU2;
    FLOAT limitD2;
    FVECTOR base;
    CMemo2DCard* pCard;
    if (*(FLOAT*)&work(0x1F0) < 1.0f) return;
    shift = ((CMemo2DCursor*)work(0x1B4))->getShiftVector();
    limitL1 = shift.x - 192.0f;
    limitR1 = shift.x + 192.0f;
    limitU1 = shift.y - 96.0f;
    limitD1 = shift.y + 96.0f;
    limitL2 = shift.x - 320.0f;
    limitR2 = shift.x + 320.0f;
    limitU2 = shift.y - 288.0f;
    limitD2 = shift.y + 288.0f;
    shiftVec.x = -shift.x / *(FLOAT*)&work(0x1F0);
    shiftVec.y = -shift.y / *(FLOAT*)&work(0x1F0);
    for (INT i = 0; i < work(0x1D8); i++) {
        pCard = ((CMemo2DCard**)work(0x1D4))[i];
        if (!pCard) continue;
        base = pCard->getBasePosition();
        if (base.x < limitL2 || limitR2 < base.x || base.y < limitU2 || limitD2 < base.y) continue;
        if ((base.x < limitL1 && limitL2 < base.x) || (limitR1 < base.x && base.x < limitR2) ||
            (base.y < limitU1 && limitU2 < base.y) || (limitD1 < base.y && base.y < limitD2)) {
            if (pCard->isEnableDisp(1)) pCard->enableDisp(1, FALSE);
            if (pCard->isEnableDisp(2)) pCard->enableDisp(2, FALSE);
        }
    }
}

namespace JiminyMemo {
extern const INT SoraCardNumTbl[];
extern const INT RikuCardNumTbl[];
namespace Collect {
extern UINT CursorMoveFlag[];
extern UINT CursorMoveFlagWithSpecial[];
namespace D {
extern UINT CursorMoveFlag[];
}
}
}
INT CJiminyMemo::setParamFromCollectToCardView() {
    INT cursorLimitX;
    INT cursorLimitY;
    INT current;
    INT currentCard;
    INT cardId;
    INT kindId;
    UINT* pCursorMoveFlag;
    INT cardKindNum;
    const INT* pCardKindTbl;
    if (!work(0x1D4)) return FALSE;
    current = -1;
    cursorLimitX = work(0x1FC);
    cursorLimitY = work(0x200);
    currentCard = *(SHORT*)&work(0x1DC) + *(SHORT*)&work(0x1DE) * cursorLimitX;
    if (isModeJiminyMemo()) {
        pCursorMoveFlag = haveSpecialCard() ? JiminyMemo::Collect::CursorMoveFlagWithSpecial
                                            : JiminyMemo::Collect::CursorMoveFlag;
        cardKindNum = 10;
        pCardKindTbl = JiminyMemo::SoraCardNumTbl;
    } else {
        pCursorMoveFlag = JiminyMemo::Collect::D::CursorMoveFlag;
        pCardKindTbl = JiminyMemo::RikuCardNumTbl;
        cardKindNum = 4;
    }
    for (INT i = 0; i < cursorLimitY; i++) {
        for (INT j = 0; j < cursorLimitX; j++) {
            if (pCursorMoveFlag[i] & (1 << j)) {
                current++;
                if (--currentCard < 0) goto found;
            } else {
                if (--currentCard < 0) return FALSE;
            }
        }
    }
    return FALSE;
found:
    if (!((CMemo2DCard**)work(0x1D4))[current]) return FALSE;
    for (kindId = 0; kindId < cardKindNum; kindId++) {
        for (cardId = 0; cardId < pCardKindTbl[kindId]; cardId++) {
            if (--current < 0) goto set;
        }
    }
    return FALSE;
set:
    setContentParam(0, kindId);
    setContentParam(1, cardId);
    return TRUE;
}
namespace JiminyMemo {
namespace Card {
extern INT NoteSeqData[5];
extern CursorPosition CursorPosiOffset;
extern CursorPosition KingMarkPosiOffset;
extern FVECTOR NoteScrollBarPosi;
}
}
void CJiminyMemo::contentCard() {
    INT eventFlagId = isModeDReport() ? 0x14F : 0xB7;
    BOOL bCursorShift = FALSE;
    switch (getContentStep()) {
    case 20: {
        INT listNum;
        INT posiNum;
        INT CTNo;
        const INT* cardNumTbl;
        FVECTOR posi;
        D2::C2DSeqCtrl** pSeqCtrl;
        D2::C2DFnt** pString;
        INT flagOrg;
        INT flagId;
        CMemo2DSequence* pList;
        if (isModeJiminyMemo()) {
            listNum = 10;
            if (!haveSpecialCard()) listNum = 9;
            CTNo = 0x700701;
            cardNumTbl = JiminyMemo::SoraCardNumTbl;
        } else {
            listNum = 4;
            CTNo = 0x70070B;
            cardNumTbl = JiminyMemo::RikuCardNumTbl;
        }
        posiNum = listNum < 9 ? listNum : 8;
        m_ContentCard.iCursorPosiNum = posiNum;
        m_ContentCard.iListNum = listNum;
        m_ContentCard.starCTNo = CTNo;
        m_ContentCard.iStringNum = posiNum;
        pString = new (__FILE__ "(7187)") D2::C2DFnt*[m_ContentCard.iStringNum];
        if (pString) {
            for (INT i = 0; i < m_ContentCard.iStringNum; i++) pString[i] = NULL;
            m_ContentCard.pString = pString;
        }
        m_ContentCard.iSeqCtrlNum = m_ContentCard.iStringNum + m_ContentCard.iListNum * 2 + 3;
        pSeqCtrl = new (__FILE__ "(7194)") D2::C2DSeqCtrl*[m_ContentCard.iSeqCtrlNum];
        if (pSeqCtrl) {
            for (INT i = 0; i < m_ContentCard.iSeqCtrlNum; i++) pSeqCtrl[i] = NULL;
            m_ContentCard.pSeqCtrl = pSeqCtrl;
        }
        flagOrg = eventFlagId;
        m_ContentCard.pCardKindFlagTbl = (BYTE*)com_malloc_d(listNum * 2, __FILE__ "(7203)", -1);
        m_ContentCard.pCompFlagTbl = m_ContentCard.pCardKindFlagTbl + listNum;
        for (INT i = 0; i < listNum; i++) {
            flagId = flagOrg;
            m_ContentCard.pCardKindFlagTbl[i] = 0;
            for (INT j = 0; j < cardNumTbl[i]; j++, flagId++) {
                if (CJiminyMemoFlag::isEqualContentFlag(flagId, 1)) {
                    m_ContentCard.pCardKindFlagTbl[i] = 1;
                    break;
                }
                if (CJiminyMemoFlag::isContentFlag(flagId, 2)) {
                    m_ContentCard.pCardKindFlagTbl[i] |= 2;
                } else if (CJiminyMemoFlag::isContentFlag(flagId, 4)) {
                    m_ContentCard.pCardKindFlagTbl[i] |= 4;
                }
            }
            flagId = flagOrg;
            m_ContentCard.pCompFlagTbl[i] = 0x11;
            for (INT j = 0; j < cardNumTbl[i]; j++, flagId++) {
                if (CJiminyMemoFlag::isEqualContentFlag(flagId, 0)) {
                    m_ContentCard.pCompFlagTbl[i] = 0x10;
                    break;
                }
            }
            flagOrg += cardNumTbl[i];
        }
        pList = createSequence(JiminyMemo::Card::NoteSeqData, 0x10, 0xB, 0, NULL);
        if (pList) {
            m_ContentCard.pList = pList;
            pList->setInState(pList->getSideInId(getPrevContentMode() == 0));
            for (INT i = 0; i < m_ContentCard.iCursorPosiNum; i++) {
                createListSequence(pList, 0x11, 0x10, i + 1, pSeqCtrl++, pString++, 0x8049481F, 16, 0x21000,
                                   JiminyMemo::getOpenCloseStringData(CTNo + i, m_ContentCard.pCardKindFlagTbl[i]));
            }
        }
        m_ContentCard.iCursorCurrent =
            (getPrevContentMode() == 7 || getPrevContentMode() == 8) ? getContentParam(0) : 0;
        m_ContentCard.pCursor = createCursorSequence((const INT*)JiminyMemo::CursorSeqTbl, 0x10, 0x11,
                                                     JiminyMemo::Card::CursorPosiOffset, posiNum, listNum, 0, 0, 1,
                                                     0x11, 0x20002, m_ContentCard.iCursorCurrent, pList);
        if (m_ContentCard.pCursor && getPrevContentMode()) {
            getContentScroll(0, posi);
            m_ContentCard.pCursor->setShiftVector(posi);
            m_ContentCard.pCursor->setPosiId(m_ContentCard.iCursorCurrent);
        }
        createMarkSequence(0x93, listNum, -1, 0, 0x11, pList, pSeqCtrl);
        if (m_ContentCard.pCardKindFlagTbl) {
            for (INT i = 0; i < listNum; i++) {
                if (pSeqCtrl[i]) {
                    if (m_ContentCard.pCardKindFlagTbl[i] & 1) {
                        pSeqCtrl[i]->enableDisp(TRUE);
                    } else if (m_ContentCard.pCardKindFlagTbl[i] & 2) {
                        pSeqCtrl[i]->replaceSeq(1, m_pLadName, 0x94);
                        pSeqCtrl[i]->enableDisp(TRUE);
                    } else {
                        pSeqCtrl[i]->enableDisp(FALSE);
                    }
                }
            }
        }
        m_ContentCard.pNewMarkSeq = pSeqCtrl;
        pSeqCtrl += listNum;
        createMarkSequence(0x108, listNum, -1, 0, 0x10, pList, pSeqCtrl);
        if (m_ContentCard.pCompFlagTbl) {
            for (INT i = 0; i < listNum; i++) {
                if (pSeqCtrl[i]) {
                    if (m_ContentCard.pCompFlagTbl[i] & 1) {
                        pSeqCtrl[i]->replaceSeq(1, m_pLadName, 0x92);
                        pSeqCtrl[i]->enableDisp(TRUE);
                    } else {
                        pSeqCtrl[i]->enableDisp(TRUE);
                    }
                    pSeqCtrl[i]->shiftSeqPos(1, JiminyMemo::Card::KingMarkPosiOffset.x,
                                             JiminyMemo::Card::KingMarkPosiOffset.y);
                }
            }
        }
        m_ContentCard.pKingMarkSeq = pSeqCtrl;
        pSeqCtrl += listNum;
        ScrollBarInit(pSeqCtrl, 0x10, JiminyMemo::Card::NoteScrollBarPosi, 150.0f, pList);
        m_ContentCard.pScrollBar = pSeqCtrl;
        if (m_ContentCard.pCursor) {
            CursorPosition cp;
            m_ContentCard.pCursor->getSubVector(cp, 1, 0);
            m_ContentCard.fCursorIntervalY = cp.y;
        }
        if (getPlateState(0) == -1) {
            setPlateState(0, 0);
            setPlateText(0, 0x700203);
        }
        bCursorShift = TRUE;
        setContentStep(30);
        setContentTimer(16.0f);
        break;
    }
    case 30:
        if (getContentTimer() <= 0.0f) {
            if (m_ContentCard.pSeqCtrl) changeStateLoopSequence(m_ContentCard.pSeqCtrl, m_ContentCard.iSeqCtrlNum);
            setTelopMessage(CCtdMan::getText(isModeJiminyMemo() ? 0x700108 : 0x70011C));
            setContentStep(40);
        }
        break;
    case 40:
        if (_isButtonTrg(0x800, 0)) {
            setContentStep(50);
            JiminyMemo::playSE(4);
            setNextContentMode(-1);
        } else if (m_ContentCard.pCursor) {
            if (_isTrgOK(0)) {
                if (m_ContentCard.pCardKindFlagTbl[m_ContentCard.iCursorCurrent]) {
                    setContentStep(50);
                    if (isModeJiminyMemo() && m_ContentCard.iCursorCurrent == 8) {
                        setNextContentMode(8);
                        setContentParam(1, 0);
                    } else {
                        setNextContentMode(7);
                    }
                    setContentParam(0, m_ContentCard.iCursorCurrent);
                    setContentScroll(0, m_ContentCard.pCursor->getShiftVector());
                    JiminyMemo::playSE(2);
                } else {
                    JiminyMemo::playSE(5);
                }
            } else if (_isTrgCANCEL(0)) {
                setNextContentMode(0);
                setContentStep(50);
                JiminyMemo::playSE(4);
            } else if (isButtonRptLimitStop(0x204000, 0, m_ContentCard.iCursorCurrent == m_ContentCard.iListNum - 1)) {
                m_ContentCard.iCursorCurrent++;
                if (m_ContentCard.iCursorCurrent >= m_ContentCard.iListNum) m_ContentCard.iCursorCurrent = 0;
                bCursorShift = TRUE;
            } else if (isButtonRptLimitStop(0x101000, 0, m_ContentCard.iCursorCurrent == 0)) {
                m_ContentCard.iCursorCurrent--;
                if (m_ContentCard.iCursorCurrent < 0) m_ContentCard.iCursorCurrent = m_ContentCard.iListNum - 1;
                bCursorShift = TRUE;
            } else if (_isButtonTrg(8, 0)) {
                bCursorShift = nextPage(m_ContentCard.pCursor, &m_ContentCard.iCursorCurrent, m_ContentCard.iListNum,
                                        m_ContentCard.iCursorPosiNum, m_ContentCard.fCursorIntervalY);
            } else if (_isButtonTrg(4, 0)) {
                bCursorShift = prevPage(m_ContentCard.pCursor, &m_ContentCard.iCursorCurrent, m_ContentCard.iListNum,
                                        m_ContentCard.iCursorPosiNum, m_ContentCard.fCursorIntervalY);
            }
            if (bCursorShift) {
                m_ContentCard.pCursor->setPosiId(m_ContentCard.iCursorCurrent);
                JiminyMemo::playSE(1);
            }
        }
        break;
    case 50: {
        INT nextContent;
        if (m_ContentCard.pList) {
            m_ContentCard.pList->setOutState(m_ContentCard.pList->getSideOutId(getNextContentMode() == 0));
        }
        if (m_ContentCard.pCursor) m_ContentCard.pCursor->setState(4);
        if (m_ContentCard.pSeqCtrl) changeStateOutSequence(m_ContentCard.pSeqCtrl, m_ContentCard.iSeqCtrlNum);
        nextContent = getNextContentMode();
        if (nextContent == 0) {
            setPlateState(0, 2);
        } else if (nextContent == -1) {
            setPlateState(0, 2);
        }
        setTelopMessage("");
        setContentStep(60);
        setContentTimer(16.0f);
        break;
    }
    case 60:
        if (getContentTimer() <= 0.0f) setContentStep(80);
        break;
    case 80:
    case 90:
        if (m_ContentCard.pList) delete m_ContentCard.pList;
        m_ContentCard.pList = NULL;
        if (m_ContentCard.pCursor) delete m_ContentCard.pCursor;
        m_ContentCard.pCursor = NULL;
        if (m_ContentCard.pCardKindFlagTbl) com_free(m_ContentCard.pCardKindFlagTbl);
        m_ContentCard.pCardKindFlagTbl = NULL;
        if (m_ContentCard.pString) {
            delete[] m_ContentCard.pString;
            m_ContentCard.pString = NULL;
        }
        if (m_ContentCard.pSeqCtrl) {
            for (INT i = 0; i < m_ContentCard.iSeqCtrlNum; i++) {
                if (m_ContentCard.pSeqCtrl[i]) delete m_ContentCard.pSeqCtrl[i];
                m_ContentCard.pSeqCtrl[i] = NULL;
            }
            if (m_ContentCard.pSeqCtrl) delete[] m_ContentCard.pSeqCtrl;
            m_ContentCard.pSeqCtrl = NULL;
        }
        setContentStep(100);
        break;
    case 100:
        break;
    }
    if (m_ContentCard.pList) m_ContentCard.pList->main(getFrameRateF());
    if (m_ContentCard.pCursor) m_ContentCard.pCursor->main();
    if (bCursorShift) {
        if (m_ContentCard.pCursor) {
            INT top = (INT)m_ContentCard.pCursor->getShiftVector().y / (INT)m_ContentCard.fCursorIntervalY;
            replaceNoteListWithOpenCheck(m_ContentCard.pString, (CT_LOWER)m_ContentCard.starCTNo,
                                         m_ContentCard.pCardKindFlagTbl, m_ContentCard.iCursorPosiNum,
                                         m_ContentCard.iListNum, top);
            replaceNoteListMark(m_ContentCard.pNewMarkSeq, m_ContentCard.pCardKindFlagTbl, m_ContentCard.iCursorPosiNum,
                                m_ContentCard.iListNum, top, 0x10, 9);
            replaceNoteListMark(m_ContentCard.pKingMarkSeq, m_ContentCard.pCompFlagTbl, m_ContentCard.iCursorPosiNum,
                                m_ContentCard.iListNum, top, 0x10, 9);
            ScrollBarChange(m_ContentCard.pScrollBar, JiminyMemo::Card::NoteScrollBarPosi.y, 150.0f,
                            m_ContentCard.iCursorPosiNum, m_ContentCard.iListNum, top);
        }
    }
}
namespace JiminyMemo {
namespace CardSelect {
extern INT NoteSeqData[5];
extern CursorPosition CursorPosiOffset;
extern FVECTOR NoteScrollBarPosi;
}
}
void CJiminyMemo::contentCardSelect() {
    INT eventFlagId = isModeDReport() ? 0x14F : 0xB7;
    BOOL bCursorShift = FALSE;
    switch (getContentStep()) {
    case 20: {
        INT kind;
        INT plateCTNo;
        INT posiNum;
        INT eventFlagOffset = 0;
        FVECTOR posi;
        D2::C2DSeqCtrl** pSeqCtrl;
        D2::C2DFnt** pString;
        CMemo2DSequence* pList;
        kind = getContentParam(0);
        if (isModeJiminyMemo()) {
            m_ContentCardSelect.iListNum = JiminyMemo::SoraCardNumTbl[kind];
            m_ContentCardSelect.pCTCodeTbl = JiminyMemo::CardNameTbl[kind];
            plateCTNo = 0x700701;
            for (INT i = 0; i < kind; i++) eventFlagOffset += JiminyMemo::SoraCardNumTbl[i];
        } else {
            m_ContentCardSelect.iListNum = JiminyMemo::RikuCardNumTbl[kind];
            m_ContentCardSelect.pCTCodeTbl = JiminyMemo::RikuCardNameTbl[kind];
            plateCTNo = 0x70070B;
            for (INT i = 0; i < kind; i++) eventFlagOffset += JiminyMemo::RikuCardNumTbl[i];
        }
        m_ContentCardSelect.iKindNo = kind;
        m_ContentCardSelect.iEventFlagId = eventFlagId + eventFlagOffset;
        posiNum = m_ContentCardSelect.iListNum < 9 ? m_ContentCardSelect.iListNum : 8;
        m_ContentCardSelect.iCursorPosiNum = posiNum;
        m_ContentCardSelect.fScrollBarLength = (FLOAT)m_ContentCardSelect.iCursorPosiNum / (FLOAT)m_ContentCardSelect.iListNum;
        if (m_ContentCardSelect.fScrollBarLength > 1.0f) m_ContentCardSelect.fScrollBarLength = 1.0f;
        if (m_ContentCardSelect.iListNum - m_ContentCardSelect.iCursorPosiNum > 0) {
            m_ContentCardSelect.fScrollBarParListNum =
                (1.0f - m_ContentCardSelect.fScrollBarLength) /
                ((FLOAT)m_ContentCardSelect.iListNum - (FLOAT)m_ContentCardSelect.iCursorPosiNum);
        } else {
            m_ContentCardSelect.fScrollBarParListNum = 0.0f;
        }
        m_ContentCardSelect.iStringNum = m_ContentCardSelect.iCursorPosiNum;
        pString = new (__FILE__ "(7546)") D2::C2DFnt*[m_ContentCardSelect.iStringNum];
        if (pString) {
            for (INT i = 0; i < m_ContentCardSelect.iStringNum; i++) pString[i] = NULL;
            m_ContentCardSelect.pString = pString;
        }
        m_ContentCardSelect.iSeqCtrlNum = m_ContentCardSelect.iStringNum + m_ContentCardSelect.iListNum + 3;
        pSeqCtrl = new (__FILE__ "(7553)") D2::C2DSeqCtrl*[m_ContentCardSelect.iSeqCtrlNum];
        if (pSeqCtrl) {
            for (INT i = 0; i < m_ContentCardSelect.iSeqCtrlNum; i++) pSeqCtrl[i] = NULL;
            m_ContentCardSelect.pSeqCtrl = pSeqCtrl;
        }
        pList = createSequence(JiminyMemo::CardSelect::NoteSeqData, 0x10, 0xB, 0, NULL);
        if (pList) {
            m_ContentCardSelect.pList = pList;
            pList->setInState(pList->getSideInId(getPrevContentMode() == 3));
            for (INT i = 0; i < m_ContentCardSelect.iCursorPosiNum; i++) {
                createListSequence(pList, 0x11, 0x10, i + 1, pSeqCtrl++, pString++, 0x8049481F, 16, 0x21000,
                                   CCtdMan::getText(JiminyMemo::getCT0014Code(m_ContentCardSelect.pCTCodeTbl[i])));
            }
        }
        m_ContentCardSelect.iCursorCurrent = getPrevContentMode() == 8 ? getContentParam(1) : 0;
        m_ContentCardSelect.pCursor = createCursorSequence(
            (const INT*)JiminyMemo::CursorSeqTbl, 0x10, 0x11, JiminyMemo::CardSelect::CursorPosiOffset,
            m_ContentCardSelect.iCursorPosiNum, m_ContentCardSelect.iListNum, 0, 0, 1, 0x11, 0x20002,
            m_ContentCardSelect.iCursorCurrent, pList);
        if (m_ContentCardSelect.pCursor && getPrevContentMode() == 8) {
            getContentScroll(1, posi);
            m_ContentCardSelect.pCursor->setShiftVector(posi);
            m_ContentCardSelect.pCursor->setPosiId(m_ContentCardSelect.iCursorCurrent);
        }
        createMarkSequence(0x93, m_ContentCardSelect.iListNum, -1, 0, 0x11, pList, pSeqCtrl);
        for (INT i = 0; i < m_ContentCardSelect.iListNum; i++) {
            if (pSeqCtrl[i]) {
                if (!CJiminyMemoFlag::isContentFlag(m_ContentCardSelect.iEventFlagId + i, 4) &&
                    CJiminyMemoFlag::isContentFlag(m_ContentCardSelect.iEventFlagId + i, 2)) {
                    pSeqCtrl[i]->replaceSeq(1, m_pLadName, 0x94);
                }
                pSeqCtrl[i]->enableDisp(CJiminyMemoFlag::isContentNew(m_ContentCardSelect.iEventFlagId + i) != 0);
            }
        }
        m_ContentCardSelect.pNewMarkSeq = pSeqCtrl;
        pSeqCtrl += m_ContentCardSelect.iListNum;
        ScrollBarInit(pSeqCtrl, 0x10, JiminyMemo::CardSelect::NoteScrollBarPosi, 150.0f, pList);
        m_ContentCardSelect.pScrollBar = pSeqCtrl;
        if (m_ContentCardSelect.pCursor) {
            CursorPosition cp;
            m_ContentCardSelect.pCursor->getSubVector(cp, 1, 0);
            m_ContentCardSelect.fCursorIntervalY = cp.y;
        }
        if (getPlateState(1) == -1) {
            setPlateState(1, 0);
            setPlateText(1, plateCTNo + kind);
        }
        bCursorShift = TRUE;
        setContentStep(30);
        setContentTimer(16.0f);
        break;
    }
    case 30:
        if (getContentTimer() <= 0.0f) {
            if (m_ContentCardSelect.pSeqCtrl) {
                changeStateLoopSequence(m_ContentCardSelect.pSeqCtrl, m_ContentCardSelect.iSeqCtrlNum);
            }
            setTelopMessage(
                CCtdMan::getText((isModeJiminyMemo() ? m_ContentCardSelect.iKindNo + 0x700109 : m_ContentCardSelect.iKindNo + 0x70011D)));
            setContentStep(40);
        }
        break;
    case 40:
        if (_isButtonTrg(0x800, 0)) {
            setContentStep(50);
            JiminyMemo::playSE(4);
            setNextContentMode(-1);
        } else if (m_ContentCardSelect.pCursor) {
            if (_isTrgOK(0)) {
                if (CJiminyMemoFlag::isContentOpen(m_ContentCardSelect.iEventFlagId +
                                                   m_ContentCardSelect.iCursorCurrent)) {
                    setContentStep(50);
                    setNextContentMode(8);
                    setContentParam(1, m_ContentCardSelect.iCursorCurrent);
                    setContentScroll(1, m_ContentCardSelect.pCursor->getShiftVector());
                    JiminyMemo::playSE(2);
                } else {
                    JiminyMemo::playSE(5);
                }
            } else if (_isTrgCANCEL(0)) {
                setContentStep(50);
                setNextContentMode(3);
                JiminyMemo::playSE(4);
            } else if (isButtonRptLimitStop(0x204000, 0,
                                            m_ContentCardSelect.iCursorCurrent == m_ContentCardSelect.iListNum - 1)) {
                m_ContentCardSelect.iCursorCurrent++;
                if (m_ContentCardSelect.iCursorCurrent >= m_ContentCardSelect.iListNum) {
                    m_ContentCardSelect.iCursorCurrent = 0;
                }
                bCursorShift = TRUE;
            } else if (isButtonRptLimitStop(0x101000, 0, m_ContentCardSelect.iCursorCurrent == 0)) {
                m_ContentCardSelect.iCursorCurrent--;
                if (m_ContentCardSelect.iCursorCurrent < 0) {
                    m_ContentCardSelect.iCursorCurrent = m_ContentCardSelect.iListNum - 1;
                }
                bCursorShift = TRUE;
            } else if (_isButtonTrg(8, 0)) {
                bCursorShift = nextPage(m_ContentCardSelect.pCursor, &m_ContentCardSelect.iCursorCurrent,
                                        m_ContentCardSelect.iListNum, m_ContentCardSelect.iCursorPosiNum,
                                        m_ContentCardSelect.fCursorIntervalY);
            } else if (_isButtonTrg(4, 0)) {
                bCursorShift = prevPage(m_ContentCardSelect.pCursor, &m_ContentCardSelect.iCursorCurrent,
                                        m_ContentCardSelect.iListNum, m_ContentCardSelect.iCursorPosiNum,
                                        m_ContentCardSelect.fCursorIntervalY);
            }
            if (bCursorShift) {
                m_ContentCardSelect.pCursor->setPosiId(m_ContentCardSelect.iCursorCurrent);
                JiminyMemo::playSE(1);
            }
        }
        break;
    case 50: {
        INT nextContent;
        if (m_ContentCardSelect.pList) {
            m_ContentCardSelect.pList->setOutState(m_ContentCardSelect.pList->getSideOutId(getNextContentMode() == 3));
        }
        if (m_ContentCardSelect.pCursor) m_ContentCardSelect.pCursor->setState(4);
        if (m_ContentCardSelect.pSeqCtrl) {
            changeStateOutSequence(m_ContentCardSelect.pSeqCtrl, m_ContentCardSelect.iSeqCtrlNum);
        }
        nextContent = getNextContentMode();
        if (nextContent == 3) {
            setPlateState(1, 2);
        } else if (nextContent == -1) {
            setPlateState(0, 2);
            setPlateState(1, 2);
        }
        setTelopMessage("");
        setContentStep(60);
        setContentTimer(16.0f);
        break;
    }
    case 60:
        if (getContentTimer() <= 0.0f) setContentStep(80);
        break;
    case 80:
    case 90:
        if (m_ContentCardSelect.pList) delete m_ContentCardSelect.pList;
        m_ContentCardSelect.pList = NULL;
        if (m_ContentCardSelect.pCursor) delete m_ContentCardSelect.pCursor;
        m_ContentCardSelect.pCursor = NULL;
        if (m_ContentCardSelect.pString) {
            delete[] m_ContentCardSelect.pString;
            m_ContentCardSelect.pString = NULL;
        }
        if (m_ContentCardSelect.pSeqCtrl) {
            for (INT i = 0; i < m_ContentCardSelect.iSeqCtrlNum; i++) {
                if (m_ContentCardSelect.pSeqCtrl[i]) delete m_ContentCardSelect.pSeqCtrl[i];
                m_ContentCardSelect.pSeqCtrl[i] = NULL;
            }
            if (m_ContentCardSelect.pSeqCtrl) delete[] m_ContentCardSelect.pSeqCtrl;
            m_ContentCardSelect.pSeqCtrl = NULL;
        }
        setContentStep(100);
        break;
    case 100:
        break;
    }
    if (m_ContentCardSelect.pList) m_ContentCardSelect.pList->main(getFrameRateF());
    if (m_ContentCardSelect.pCursor) m_ContentCardSelect.pCursor->main();
    if (bCursorShift) {
        if (m_ContentCardSelect.pCursor) {
            INT top = (INT)m_ContentCardSelect.pCursor->getShiftVector().y / (INT)m_ContentCardSelect.fCursorIntervalY;
            replaceNoteListWithOpenCheck(m_ContentCardSelect.pString, m_ContentCardSelect.pCTCodeTbl,
                                         m_ContentCardSelect.iEventFlagId, m_ContentCardSelect.iCursorPosiNum,
                                         m_ContentCardSelect.iListNum, top);
            replaceNoteListMark(m_ContentCardSelect.pNewMarkSeq, m_ContentCardSelect.iEventFlagId,
                                m_ContentCardSelect.iCursorPosiNum, m_ContentCardSelect.iListNum, top, 0x10, 9);
            ScrollBarChange(m_ContentCardSelect.pScrollBar, JiminyMemo::CardSelect::NoteScrollBarPosi.y, 150.0f,
                            m_ContentCardSelect.iCursorPosiNum, m_ContentCardSelect.iListNum, top);
        }
    }
}
namespace JiminyMemo {
namespace CardView {
extern INT NoteSeqData[5];
}
}
void CJiminyMemo::contentCardView() {
    const INT eventFlagId = isModeJiminyMemo() ? 0xB7 : 0x14F;
    BOOL bTouch = FALSE;
    switch (getContentStep()) {
    case 20: {
        INT cardNo;
        INT kindNo;
        INT cardManualNo;
        INT cardEncyclNo;
        BOOL bTextPositionChange;
        FVECTOR posi;
        D2::C2DSeqCtrl** pSeqCtrl;
        D2::C2DFnt** pString;
        INT* pListIndex;
        CMemo2DSequence* pList;
        INT eventFlagOffset;
        cardNo = getContentParam(1);
        kindNo = getContentParam(0);
        m_ContentCardView.iKindNo = kindNo;
        m_ContentCardView.iCardNo = cardNo;
        bTextPositionChange = FALSE;
        if (isModeJiminyMemo()) {
            m_ContentCardView.iKindCardMax = JiminyMemo::SoraCardNumTbl[kindNo];
            cardManualNo = JiminyMemo::getCT0014Code(JiminyMemo::CardManualTbl[kindNo][cardNo]);
            cardEncyclNo = JiminyMemo::CardEncyclTbl[kindNo][cardNo];
            if (kindNo == 5 || kindNo == 9) {
                bTextPositionChange = TRUE;
            }
        } else {
            m_ContentCardView.iKindCardMax = JiminyMemo::RikuCardNumTbl[kindNo];
            cardManualNo = JiminyMemo::getCT0014Code(JiminyMemo::RikuCardManualTbl[kindNo][cardNo]);
            cardEncyclNo = JiminyMemo::RikuCardEncyclTbl[kindNo][cardNo];
            bTextPositionChange = kindNo == 1;
        }
        m_ContentCardView.iCurrentTextLine = 0;
        splitMessage(&m_ContentCardView.iTextLineNum, &m_ContentCardView.pTextLineBuf, CCtdMan::getText(cardManualNo),
                     0xF);
        m_ContentCardView.iStringNum = m_ContentCardView.iTextLineNum + 0x42;
        pString = new (__FILE__ "(7836)") D2::C2DFnt*[m_ContentCardView.iStringNum];
        if (pString) {
            for (INT i = 0; i < m_ContentCardView.iStringNum; i++) pString[i] = NULL;
            m_ContentCardView.pString = pString;
        }
        m_ContentCardView.iSeqCtrlNum = m_ContentCardView.iTextLineNum + 9;
        pSeqCtrl = new (__FILE__ "(7844)") D2::C2DSeqCtrl*[m_ContentCardView.iSeqCtrlNum];
        if (pSeqCtrl) {
            for (INT i = 0; i < m_ContentCardView.iSeqCtrlNum; i++) pSeqCtrl[i] = NULL;
            m_ContentCardView.pSeqCtrl = pSeqCtrl;
        }
        pListIndex = (INT*)com_malloc_d(m_ContentCardView.iSeqCtrlNum * sizeof(INT), __FILE__ "(7851)", -1);
        if (pListIndex) {
            for (INT i = 0; i < m_ContentCardView.iSeqCtrlNum; i++) pListIndex[i] = -1;
            m_ContentCardView.pListIndex = pListIndex;
        }
        pList = createSequence(JiminyMemo::CardView::NoteSeqData, 0x10, 0xB, 0, NULL);
        if (pList) {
            INT lineNum;
            m_ContentCardView.pList = pList;
            pList->setInState(0);
            lineNum = 0;
            if (!bTextPositionChange) {
                for (INT i = 0; i < 6; i++) {
                    if (createListSequenceTypewriterFont(pList, 0x11, 0x15, 6, pSeqCtrl, pString, 0x8049481F, 0x10,
                                                         0x21000, m_ContentCardView.pTextLineBuf[i + 1], 14.0f)) {
                        while (*pSeqCtrl) {
                            (*pSeqCtrl++)->enableDisp(FALSE);
                            *pListIndex++ = lineNum;
                        }
                        lineNum++;
                    }
                    while (*pString) pString++;
                }
                for (INT i = 0; i < m_ContentCardView.iTextLineNum - 7; i++) {
                    createListSequence(pList, 0x11, 0x15, 1, pSeqCtrl, pString, 0x8049481F, 0x10, 0x21000,
                                       m_ContentCardView.pTextLineBuf[i + 7]);
                    (*pSeqCtrl)->enableDisp(FALSE);
                    pSeqCtrl++;
                    pString++;
                    *pListIndex++ = lineNum;
                    lineNum++;
                }
            } else {
                for (INT i = 0; i < m_ContentCardView.iTextLineNum - 7; i++) {
                    createListSequence(pList, 0x11, 0x15, 1, pSeqCtrl, pString, 0x8049481F, 0x10, 0x21000,
                                       m_ContentCardView.pTextLineBuf[i + 7]);
                    (*pSeqCtrl)->enableDisp(FALSE);
                    pSeqCtrl++;
                    pString++;
                    *pListIndex++ = lineNum;
                    lineNum++;
                }
                for (INT i = 0; i < 6; i++) {
                    if (createListSequenceTypewriterFont(pList, 0x11, 0x15, 6, pSeqCtrl, pString, 0x8049481F, 0x10,
                                                         0x21000, m_ContentCardView.pTextLineBuf[i + 1], 14.0f)) {
                        while (*pSeqCtrl) {
                            (*pSeqCtrl++)->enableDisp(FALSE);
                            *pListIndex++ = lineNum;
                        }
                        lineNum++;
                    }
                    while (*pString) pString++;
                }
            }
            m_ContentCardView.iListNum = lineNum;
            bTouch = TRUE;
            createListSequence(pList, 0x11, 0x15, 4, pSeqCtrl++, pString, 0x807F7F7F, 0x11, 0x42,
                               m_ContentCardView.pTextLineBuf[0]);
        }
        if (pList) {
            CMemo2DCard* pCard;
            createMarkSequence(0xD0, 1, 0x15, 0xC, 0x11, pList, pSeqCtrl);
            m_ContentCardView.pTopArrowSeq = *pSeqCtrl++;
            createMarkSequence(0xC3, 1, 0x15, 0xD, 0x11, pList, pSeqCtrl);
            m_ContentCardView.pBottomArrowSeq = *pSeqCtrl;
            pCard = new (__FILE__ "(7987)") CMemo2DCard(m_pLadName);
            if (pCard) {
                CARD card;
                m_ContentCardView.pCard = pCard;
                pCard->setLayer(0x11);
                pCard->enableColorCtrl(TRUE);
                pCard->setParent(pList);
                D2::C2DMan::getLayPosition(m_pLadFileName, 0x15, 5, posi, 0);
                pCard->setBasePosition(posi);
                pCard->setLocalPosition();
                pCard->enableDisp(TRUE);
                pCard->setState(1);
                _setCard(card, cardEncyclNo, 0xFF);
                pCard->create(card, 1);
                pCard->enableDisp(3, FALSE);
            }
        }
        if (getPlateState(2) == -1) setPlateState(2, 0);
        setPlateText(2, 0x700002);
        eventFlagOffset = 0;
        if (isModeJiminyMemo()) {
            for (INT i = 0; i < kindNo; i++) eventFlagOffset += JiminyMemo::SoraCardNumTbl[i];
        } else {
            for (INT i = 0; i < kindNo; i++) eventFlagOffset += JiminyMemo::RikuCardNumTbl[i];
        }
        m_ContentCardView.iPrevNo = getPrevCardId(eventFlagId + eventFlagOffset);
        m_ContentCardView.iNextNo = getNextCardId(eventFlagId + eventFlagOffset);
        if (m_ContentCardView.iKindNo != 8) {
            if (getPlateState(4) == -1) setPlateState(4, 0);
            setPlateNumSeq(9, cardNo + 1);
            setPlateNumSeq(10, m_ContentCardView.iKindCardMax);
        }
        if (getPlateState(0) == -1) setPlateState(0, 0);
        setPlateText(0, 0x700203);
        if (getPlateState(1) == -1) setPlateState(1, 0);
        setPlateText(1, isModeJiminyMemo() ? kindNo + 0x700701 : kindNo + 0x70070B);
        setContentTimer(16.0f);
        setContentStep(30);
        break;
    }
    case 30:
        if (getContentTimer() <= 0.0f) {
            INT eventFlagOffset = 0;
            INT kindNo = m_ContentCardView.iKindNo;
            if (isModeJiminyMemo()) {
                for (INT i = 0; i < kindNo; i++) eventFlagOffset += JiminyMemo::SoraCardNumTbl[i];
            } else {
                for (INT i = 0; i < kindNo; i++) eventFlagOffset += JiminyMemo::RikuCardNumTbl[i];
            }
            CJiminyMemoFlag::setContentTouchFlag(eventFlagId + eventFlagOffset + m_ContentCardView.iCardNo);
            if (m_ContentCardView.pSeqCtrl) {
                changeStateLoopSequence(m_ContentCardView.pSeqCtrl, m_ContentCardView.iSeqCtrlNum);
            }
            setContentStep(40);
        }
        break;
    case 40:
        if (_isButtonTrg(0x800, 0)) {
            setContentStep(50);
            JiminyMemo::playSE(4);
            setNextContentMode(-1);
        } else if (_isTrgCANCEL(0)) {
            setContentStep(50);
            if (m_ContentCardView.iKindNo != 8) {
                setNextContentMode(7);
            } else {
                setNextContentMode(3);
            }
            JiminyMemo::playSE(4);
        } else if (_isButtonTrg(0x80, 0)) {
            setContentStep(50);
            setNextContentMode(2);
            setContentParam(0, setParamFromCardViewToCollect());
            JiminyMemo::playSE(2);
        } else if (m_ContentCardView.iPrevNo >= 0 && _isButtonTrg(1, 0)) {
            setContentStep(50);
            setNextContentMode(8);
            setContentParam(1, m_ContentCardView.iPrevNo);
            JiminyMemo::playSE(2);
        } else if (m_ContentCardView.iNextNo >= 0 && _isButtonTrg(2, 0)) {
            setContentStep(50);
            setNextContentMode(8);
            setContentParam(1, m_ContentCardView.iNextNo);
            JiminyMemo::playSE(2);
        } else if (_isButtonRpt(0x204000, 0)) {
            INT limit = m_ContentCardView.iListNum - 6;
            if (limit > 0) {
                m_ContentCardView.iCurrentTextLine++;
                if (limit < m_ContentCardView.iCurrentTextLine) {
                    m_ContentCardView.iCurrentTextLine = limit;
                } else {
                    bTouch = TRUE;
                }
            }
        } else if (_isButtonRpt(0x101000, 0)) {
            m_ContentCardView.iCurrentTextLine--;
            if (m_ContentCardView.iCurrentTextLine < 0) {
                m_ContentCardView.iCurrentTextLine = 0;
            } else {
                bTouch = TRUE;
            }
        } else if (_isButtonTrg(8, 0)) {
            bTouch = nextPage(&m_ContentCardView.iCurrentTextLine, m_ContentCardView.iListNum, 6);
        } else if (_isButtonTrg(4, 0)) {
            bTouch = prevPage(&m_ContentCardView.iCurrentTextLine, 0, 6);
        }
        if (bTouch == TRUE) JiminyMemo::playSE(1);
        break;
    case 50: {
        INT nextContent;
        if (m_ContentCardView.pList) m_ContentCardView.pList->setOutState(2);
        if (m_ContentCardView.pCard) m_ContentCardView.pCard->setState(3);
        if (m_ContentCardView.pSeqCtrl) {
            changeStateOutSequence(m_ContentCardView.pSeqCtrl, m_ContentCardView.iSeqCtrlNum);
        }
        nextContent = getNextContentMode();
        if (nextContent == 7) {
            setPlateState(2, 2);
            setPlateState(4, 2);
        } else if (nextContent == 3) {
            setPlateState(2, 2);
            setPlateState(1, 2);
        } else if (nextContent == 2) {
            setPlateState(1, 2);
            if (getPlateState(4) != -1) setPlateState(4, 2);
        } else if (nextContent == -1) {
            setPlateState(0, 2);
            setPlateState(1, 2);
            setPlateState(2, 2);
            if (getPlateState(4) != -1) setPlateState(4, 2);
        }
        setContentTimer(16.0f);
        setContentStep(60);
        break;
    }
    case 60:
        if (getContentTimer() <= 0.0f) setContentStep(80);
        break;
    case 80:
    case 90:
        if (m_ContentCardView.pList) delete m_ContentCardView.pList;
        m_ContentCardView.pList = NULL;
        if (m_ContentCardView.pCard) delete m_ContentCardView.pCard;
        m_ContentCardView.pCard = NULL;
        if (m_ContentCardView.pListIndex) com_free(m_ContentCardView.pListIndex);
        m_ContentCardView.pListIndex = NULL;
        if (m_ContentCardView.pString) {
            delete[] m_ContentCardView.pString;
            m_ContentCardView.pString = NULL;
        }
        if (m_ContentCardView.pSeqCtrl) {
            for (INT i = 0; i < m_ContentCardView.iSeqCtrlNum; i++) {
                if (m_ContentCardView.pSeqCtrl[i]) delete m_ContentCardView.pSeqCtrl[i];
                m_ContentCardView.pSeqCtrl[i] = NULL;
            }
            if (m_ContentCardView.pSeqCtrl) delete[] m_ContentCardView.pSeqCtrl;
            m_ContentCardView.pSeqCtrl = NULL;
            m_ContentCardView.pBottomArrowSeq = NULL;
            m_ContentCardView.pTopArrowSeq = NULL;
        }
        if (m_ContentCardView.pTextLineBuf) {
            for (INT i = 0; i < m_ContentCardView.iTextLineNum; i++) {
                if (m_ContentCardView.pTextLineBuf[i]) com_free(m_ContentCardView.pTextLineBuf[i]);
                m_ContentCardView.pTextLineBuf[i] = NULL;
            }
            if (m_ContentCardView.pTextLineBuf) com_free(m_ContentCardView.pTextLineBuf);
            m_ContentCardView.pTextLineBuf = NULL;
        }
        setContentStep(100);
        break;
    case 100:
        break;
    }
    if (m_ContentCardView.pList) m_ContentCardView.pList->main(getFrameRateF());
    if (m_ContentCardView.pCard) m_ContentCardView.pCard->main(getFrameRateF());
    if (m_ContentCardView.pBottomArrowSeq) {
        m_ContentCardView.pBottomArrowSeq->enableDisp(m_ContentCardView.iCurrentTextLine < m_ContentCardView.iListNum - 6);
    }
    if (m_ContentCardView.pTopArrowSeq) {
        m_ContentCardView.pTopArrowSeq->enableDisp(m_ContentCardView.iCurrentTextLine > 0);
    }
    if (bTouch == TRUE) {
        FVECTOR posi;
        D2::C2DSeqCtrl** pSeqCtrl = m_ContentCardView.pSeqCtrl;
        INT* pListIndex = m_ContentCardView.pListIndex;
        INT seqNum = m_ContentCardView.iSeqCtrlNum;
        for (INT i = 0; i < seqNum; i++) {
            if (pSeqCtrl[i] && pListIndex[i] >= 0) pSeqCtrl[i]->enableDisp(FALSE);
        }
        for (INT i = 0; i < 6; i++) {
            D2::C2DMan::getLayPosition(m_pLadFileName, 0x15, i + 6, posi, 0);
            for (INT j = 0; j < seqNum; j++) {
                if (pListIndex[j] == m_ContentCardView.iCurrentTextLine + i) {
                    if (pSeqCtrl[j]) {
                        pSeqCtrl[j]->setLocalPos(posi);
                        pSeqCtrl[j]->enableDisp(TRUE);
                    }
                }
            }
        }
    }
}
INT CJiminyMemo::getPrevCardId(INT inId) {
    return getPrevContentId(inId, work(0x2D4), work(0x2D0));
}
INT CJiminyMemo::getNextCardId(INT inId) {
    return getNextContentId(inId, work(0x2D4), work(0x2D0));
}
INT CJiminyMemo::setParamFromCardViewToCollect() {
    INT rval = 0;
    INT cardNo = work(0x2D0);
    INT kindNo = work(0x2CC);
    INT cursorLimitX;
    INT cursorLimitY;
    UINT* pCursorMoveFlag;
    const INT* pCardNumTbl;
    if (m_iPlayerMode == 0) {
        cursorLimitX = 22;
        cursorLimitY = 7;
        pCursorMoveFlag = haveSpecialCard() ? JiminyMemo::Collect::CursorMoveFlagWithSpecial
                                            : JiminyMemo::Collect::CursorMoveFlag;
        pCardNumTbl = JiminyMemo::SoraCardNumTbl;
    } else {
        pCursorMoveFlag = JiminyMemo::Collect::D::CursorMoveFlag;
        cursorLimitX = 22;
        cursorLimitY = 3;
        pCardNumTbl = JiminyMemo::RikuCardNumTbl;
    }
    INT currentCardNo = cardNo;
    for (INT i = 0; i < kindNo; i++) currentCardNo += pCardNumTbl[i];
    for (INT i = 0; i < cursorLimitY; i++) {
        for (INT j = 0; j < cursorLimitX; j++) {
            if (pCursorMoveFlag[i] & (1 << j)) {
                if (--currentCardNo < 0) goto end;
            }
            rval++;
        }
    }
end:
    return rval;
}
namespace JiminyMemo {
extern const INT CharaNumTbl[];
extern const INT RikuCharaNumTbl[];
}
namespace JiminyMemo {
namespace Chara {
extern INT NoteSeqData[5];
extern CursorPosition CursorPosiOffset;
extern CursorPosition KingMarkPosiOffset;
extern FVECTOR NoteScrollBarPosi;
}
}
void CJiminyMemo::contentChara() {
    const INT eventFlagId = isModeJiminyMemo() ? 0x17 : 0x7F;
    BOOL bCursorShift = FALSE;
    switch (getContentStep()) {
    case 20: {
        INT posiNum;
        INT listNum;
        INT ctNo;
        const INT* charaNumTbl;
        FVECTOR posi;
        D2::C2DSeqCtrl** pSeqCtrl;
        D2::C2DFnt** pString;
        INT flagOrg;
        INT flagId;
        CMemo2DSequence* pList;
        if (isModeJiminyMemo()) {
            ctNo = 0x701C01;
            listNum = 15;
            charaNumTbl = JiminyMemo::CharaNumTbl;
        } else {
            ctNo = 0x701C11;
            listNum = 3;
            charaNumTbl = JiminyMemo::RikuCharaNumTbl;
        }
        posiNum = listNum < 9 ? listNum : 8;
        m_ContentChara.iNoteListNum = listNum;
        m_ContentChara.iCursorPosiNum = posiNum;
        m_ContentChara.iCTCodeOffset = ctNo;
        m_ContentChara.iStringNum = posiNum;
        pString = new (__FILE__ "(8341)") D2::C2DFnt*[m_ContentChara.iStringNum];
        if (pString) {
            for (INT i = 0; i < m_ContentChara.iStringNum; i++) pString[i] = NULL;
            m_ContentChara.pString = pString;
        }
        m_ContentChara.iSeqCtrlNum = m_ContentChara.iStringNum + listNum * 2 + 3;
        pSeqCtrl = new (__FILE__ "(8348)") D2::C2DSeqCtrl*[m_ContentChara.iSeqCtrlNum];
        if (pSeqCtrl) {
            for (INT i = 0; i < m_ContentChara.iSeqCtrlNum; i++) pSeqCtrl[i] = NULL;
            m_ContentChara.pSeqCtrl = pSeqCtrl;
        }
        flagOrg = eventFlagId;
        m_ContentChara.pCardKindFlagTbl = (BYTE*)com_malloc_d(listNum * 2, __FILE__ "(8357)", -1);
        m_ContentChara.pCompFlagTbl = m_ContentChara.pCardKindFlagTbl + listNum;
        for (INT i = 0; i < listNum; i++) {
            flagId = flagOrg;
            m_ContentChara.pCardKindFlagTbl[i] = 0;
            for (INT j = 0; j < charaNumTbl[i]; j++, flagId++) {
                if (CJiminyMemoFlag::isEqualContentFlag(flagId, 1)) {
                    m_ContentChara.pCardKindFlagTbl[i] = 1;
                    break;
                }
                if (CJiminyMemoFlag::isContentFlag(flagId, 2)) {
                    m_ContentChara.pCardKindFlagTbl[i] |= 2;
                } else if (CJiminyMemoFlag::isContentFlag(flagId, 4)) {
                    m_ContentChara.pCardKindFlagTbl[i] |= 4;
                }
            }
            flagId = flagOrg;
            m_ContentChara.pCompFlagTbl[i] = 0x11;
            for (INT j = 0; j < charaNumTbl[i]; j++, flagId++) {
                if (CJiminyMemoFlag::isEqualContentFlag(flagId, 0)) {
                    m_ContentChara.pCompFlagTbl[i] = 0x10;
                    break;
                }
            }
            flagOrg += charaNumTbl[i];
        }
        pList = createSequence(JiminyMemo::Chara::NoteSeqData, 0x10, 0xB, 0, NULL);
        if (pList) {
            m_ContentChara.pList = pList;
            pList->setInState(pList->getSideInId(getPrevContentMode() == 0));
            for (INT i = 0; i < posiNum; i++) {
                createListSequence(pList, 0x11, 0x10, i + 1, pSeqCtrl++, pString++, 0x8049481F, 16, 0x21000,
                                   CCtdMan::getText(ctNo + i));
            }
        }
        m_ContentChara.iCursorCurrent = getPrevContentMode() == 9 ? getContentParam(0) : 0;
        m_ContentChara.pCursor = createCursorSequence((const INT*)JiminyMemo::CursorSeqTbl, 0x10, 0x11,
                                                     JiminyMemo::Chara::CursorPosiOffset, posiNum, listNum, 0, 0, 1,
                                                     0x11, 0x20002, m_ContentChara.iCursorCurrent, pList);
        if (m_ContentChara.pCursor && getPrevContentMode()) {
            getContentScroll(0, posi);
            m_ContentChara.pCursor->setShiftVector(posi);
            m_ContentChara.pCursor->setPosiId(m_ContentChara.iCursorCurrent);
        }
        createMarkSequence(0x93, listNum, -1, 0, 0x11, pList, pSeqCtrl);
        if (m_ContentChara.pCardKindFlagTbl) {
            for (INT i = 0; i < listNum; i++) {
                if (pSeqCtrl[i]) {
                    if (m_ContentChara.pCardKindFlagTbl[i] & 1) {
                        pSeqCtrl[i]->enableDisp(TRUE);
                    } else if (m_ContentChara.pCardKindFlagTbl[i] & 2) {
                        pSeqCtrl[i]->replaceSeq(1, m_pLadName, 0x94);
                        pSeqCtrl[i]->enableDisp(TRUE);
                    } else {
                        pSeqCtrl[i]->enableDisp(FALSE);
                    }
                }
            }
        }
        m_ContentChara.pNewMarkSeq = pSeqCtrl;
        pSeqCtrl += listNum;
        createMarkSequence(0x108, listNum, -1, 0, 0x10, pList, pSeqCtrl);
        if (m_ContentChara.pCompFlagTbl) {
            for (INT i = 0; i < listNum; i++) {
                if (pSeqCtrl[i]) {
                    if (m_ContentChara.pCompFlagTbl[i] & 1) {
                        pSeqCtrl[i]->replaceSeq(1, m_pLadName, 0x92);
                        pSeqCtrl[i]->enableDisp(TRUE);
                    } else {
                        pSeqCtrl[i]->enableDisp(TRUE);
                    }
                    pSeqCtrl[i]->shiftSeqPos(1, JiminyMemo::Chara::KingMarkPosiOffset.x,
                                             JiminyMemo::Chara::KingMarkPosiOffset.y);
                }
            }
        }
        m_ContentChara.pKingMarkSeq = pSeqCtrl;
        pSeqCtrl += listNum;
        ScrollBarInit(pSeqCtrl, 0x10, JiminyMemo::Chara::NoteScrollBarPosi, 150.0f, pList);
        m_ContentChara.pScrollBar = pSeqCtrl;
        if (m_ContentChara.pCursor) {
            CursorPosition cp;
            m_ContentChara.pCursor->getSubVector(cp, 1, 0);
            m_ContentChara.fCursorIntervalY = cp.y;
        }
        if (getPlateState(0) == -1) {
            setPlateState(0, 0);
            setPlateText(0, 0x700204);
        }
        bCursorShift = TRUE;
        setContentStep(30);
        setContentTimer(16.0f);
        break;
    }
    case 30:
        if (getContentTimer() <= 0.0f) {
            if (m_ContentChara.pSeqCtrl) changeStateLoopSequence(m_ContentChara.pSeqCtrl, m_ContentChara.iSeqCtrlNum);
            setTelopMessage(CCtdMan::getText(isModeJiminyMemo() ? 0x700112 : 0x700121));
            setContentStep(40);
        }
        break;
    case 40:
        if (_isButtonTrg(0x800, 0)) {
            setContentStep(50);
            JiminyMemo::playSE(4);
            setNextContentMode(-1);
        } else if (m_ContentChara.pCursor) {
            if (_isTrgOK(0)) {
                if (m_ContentChara.pCardKindFlagTbl[m_ContentChara.iCursorCurrent]) {
                    setContentStep(50);
                    setNextContentMode(9);
                    setContentParam(0, m_ContentChara.iCursorCurrent);
                    setContentScroll(0, m_ContentChara.pCursor->getShiftVector());
                    JiminyMemo::playSE(2);
                } else {
                    JiminyMemo::playSE(5);
                }
            } else if (_isTrgCANCEL(0)) {
                setNextContentMode(0);
                setContentStep(50);
                JiminyMemo::playSE(4);
            } else if (isButtonRptLimitStop(0x204000, 0, m_ContentChara.iCursorCurrent == m_ContentChara.iNoteListNum - 1)) {
                m_ContentChara.iCursorCurrent++;
                if (m_ContentChara.iCursorCurrent >= m_ContentChara.iNoteListNum) m_ContentChara.iCursorCurrent = 0;
                bCursorShift = TRUE;
            } else if (isButtonRptLimitStop(0x101000, 0, m_ContentChara.iCursorCurrent == 0)) {
                m_ContentChara.iCursorCurrent--;
                if (m_ContentChara.iCursorCurrent < 0) m_ContentChara.iCursorCurrent = m_ContentChara.iNoteListNum - 1;
                bCursorShift = TRUE;
            } else if (_isButtonTrg(8, 0)) {
                bCursorShift = nextPage(m_ContentChara.pCursor, &m_ContentChara.iCursorCurrent, m_ContentChara.iNoteListNum,
                                        m_ContentChara.iCursorPosiNum, m_ContentChara.fCursorIntervalY);
            } else if (_isButtonTrg(4, 0)) {
                bCursorShift = prevPage(m_ContentChara.pCursor, &m_ContentChara.iCursorCurrent, m_ContentChara.iNoteListNum,
                                        m_ContentChara.iCursorPosiNum, m_ContentChara.fCursorIntervalY);
            }
            if (bCursorShift) {
                m_ContentChara.pCursor->setPosiId(m_ContentChara.iCursorCurrent);
                JiminyMemo::playSE(1);
            }
        }
        break;
    case 50: {
        INT nextContent;
        if (m_ContentChara.pList) {
            m_ContentChara.pList->setOutState(m_ContentChara.pList->getSideOutId(getNextContentMode() == 0));
        }
        if (m_ContentChara.pCursor) m_ContentChara.pCursor->setState(4);
        if (m_ContentChara.pSeqCtrl) changeStateOutSequence(m_ContentChara.pSeqCtrl, m_ContentChara.iSeqCtrlNum);
        nextContent = getNextContentMode();
        if (nextContent == 0) {
            setPlateState(0, 2);
        } else if (nextContent == 9) {
            INT rsrcId = isModeJiminyMemo() ? 0xC1 : 0xC2;
            if (!CRsrcDB::hasRsrcGroup(2, rsrcId)) CRcf::reqLoadRCFFront(2, rsrcId, 0, NULL, 0);
        } else if (nextContent == -1) {
            setPlateState(0, 2);
        }
        setTelopMessage("");
        setContentStep(60);
        setContentTimer(16.0f);
        break;
    }
    case 60:
        if (getContentTimer() <= 0.0f) setContentStep(80);
        break;
    case 80:
    case 90:
        if (m_ContentChara.pList) delete m_ContentChara.pList;
        m_ContentChara.pList = NULL;
        if (m_ContentChara.pCursor) delete m_ContentChara.pCursor;
        m_ContentChara.pCursor = NULL;
        if (m_ContentChara.pCardKindFlagTbl) com_free(m_ContentChara.pCardKindFlagTbl);
        m_ContentChara.pCardKindFlagTbl = NULL;
        if (m_ContentChara.pString) {
            delete[] m_ContentChara.pString;
            m_ContentChara.pString = NULL;
        }
        if (m_ContentChara.pSeqCtrl) {
            for (INT i = 0; i < m_ContentChara.iSeqCtrlNum; i++) {
                if (m_ContentChara.pSeqCtrl[i]) delete m_ContentChara.pSeqCtrl[i];
                m_ContentChara.pSeqCtrl[i] = NULL;
            }
            if (m_ContentChara.pSeqCtrl) delete[] m_ContentChara.pSeqCtrl;
            m_ContentChara.pSeqCtrl = NULL;
        }
        setContentStep(100);
        break;
    case 100:
        break;
    }
    if (m_ContentChara.pList) m_ContentChara.pList->main(getFrameRateF());
    if (m_ContentChara.pCursor) m_ContentChara.pCursor->main();
    if (bCursorShift) {
        if (m_ContentChara.pCursor) {
            INT top = (INT)m_ContentChara.pCursor->getShiftVector().y / (INT)m_ContentChara.fCursorIntervalY;
            replaceNoteListWithOpenCheck(m_ContentChara.pString, (CT_LOWER)m_ContentChara.iCTCodeOffset,
                                         m_ContentChara.pCardKindFlagTbl, m_ContentChara.iCursorPosiNum,
                                         m_ContentChara.iNoteListNum, top);
            replaceNoteListMark(m_ContentChara.pNewMarkSeq, m_ContentChara.pCardKindFlagTbl, m_ContentChara.iCursorPosiNum,
                                m_ContentChara.iNoteListNum, top, 0x10, 9);
            replaceNoteListMark(m_ContentChara.pKingMarkSeq, m_ContentChara.pCompFlagTbl, m_ContentChara.iCursorPosiNum,
                                m_ContentChara.iNoteListNum, top, 0x10, 9);
            ScrollBarChange(m_ContentChara.pScrollBar, JiminyMemo::Chara::NoteScrollBarPosi.y, 150.0f,
                            m_ContentChara.iCursorPosiNum, m_ContentChara.iNoteListNum, top);
        }
    }
}
namespace JiminyMemo {
extern const INT CharaNumTbl[];
extern const INT RikuCharaNumTbl[];
namespace CharaSelect {
extern INT NoteSeqData[5];
extern CursorPosition CursorPosiOffset;
extern FVECTOR NoteScrollBarPosi;
}
}
void CJiminyMemo::contentCharaSelect() {
    INT eventFlagId = isModeDReport() ? 0x7F : 0x17;
    BOOL bCursorShift = FALSE;
    switch (getContentStep()) {
    case 20: {
        INT kind;
        INT listNum;
        CT_LOWER* ctTbl;
        INT titleNo;
        INT eventFlagOffset = 0;
        FVECTOR posi;
        D2::C2DSeqCtrl** pSeqCtrl;
        D2::C2DFnt** pString;
        CMemo2DSequence* pList;
        kind = getContentParam(0);
        if (isModeJiminyMemo()) {
            listNum = JiminyMemo::CharaNumTbl[kind];
            ctTbl = JiminyMemo::CharacterNameTbl[kind];
            titleNo = 0x701C01;
            for (INT i = 0; i < kind; i++) eventFlagOffset += JiminyMemo::CharaNumTbl[i];
        } else {
            listNum = JiminyMemo::RikuCharaNumTbl[kind];
            ctTbl = JiminyMemo::RikuCharacterNameTbl[kind];
            titleNo = 0x701C11;
            for (INT i = 0; i < kind; i++) eventFlagOffset += JiminyMemo::RikuCharaNumTbl[i];
        }
        m_ContentCharaSelect.iKindNo = kind;
        m_ContentCharaSelect.iListNum = listNum;
        m_ContentCharaSelect.iCursorPosiNum = listNum < 9 ? listNum : 8;
        m_ContentCharaSelect.fScrollBarLength = (FLOAT)m_ContentCharaSelect.iCursorPosiNum / (FLOAT)listNum;
        if (m_ContentCharaSelect.fScrollBarLength > 1.0f) m_ContentCharaSelect.fScrollBarLength = 1.0f;
        if (m_ContentCharaSelect.iListNum - m_ContentCharaSelect.iCursorPosiNum > 0) {
            m_ContentCharaSelect.fScrollBarParListNum =
                (1.0f - m_ContentCharaSelect.fScrollBarLength) /
                ((FLOAT)m_ContentCharaSelect.iListNum - (FLOAT)m_ContentCharaSelect.iCursorPosiNum);
        } else {
            m_ContentCharaSelect.fScrollBarParListNum = 0.0f;
        }
        m_ContentCharaSelect.pCTCodeTbl = ctTbl;
        m_ContentCharaSelect.iEventFlagId = eventFlagId + eventFlagOffset;
        m_ContentCharaSelect.iStringNum = m_ContentCharaSelect.iCursorPosiNum;
        pString = new (__FILE__ "(8702)") D2::C2DFnt*[m_ContentCharaSelect.iStringNum];
        if (pString) {
            for (INT i = 0; i < m_ContentCharaSelect.iStringNum; i++) pString[i] = NULL;
            m_ContentCharaSelect.pString = pString;
        }
        m_ContentCharaSelect.iSeqCtrlNum = m_ContentCharaSelect.iCursorPosiNum + listNum + 3;
        pSeqCtrl = new (__FILE__ "(8709)") D2::C2DSeqCtrl*[m_ContentCharaSelect.iSeqCtrlNum];
        if (pSeqCtrl) {
            for (INT i = 0; i < m_ContentCharaSelect.iSeqCtrlNum; i++) pSeqCtrl[i] = NULL;
            m_ContentCharaSelect.pSeqCtrl = pSeqCtrl;
        }
        pList = createSequence(JiminyMemo::CharaSelect::NoteSeqData, 0x10, 0xB, 0, NULL);
        if (pList) {
            m_ContentCharaSelect.pList = pList;
            pList->setInState(pList->getSideInId(getPrevContentMode() == 4));
            for (INT i = 0; i < m_ContentCharaSelect.iCursorPosiNum; i++) {
                createListSequence(pList, 0x11, 0x10, i + 1, pSeqCtrl++, pString++, 0x8049481F, 16, 0x21000,
                                   CCtdMan::getText(JiminyMemo::getCT0014Code(ctTbl[i])));
            }
        }
        m_ContentCharaSelect.iCursorCurrent = getPrevContentMode() == 10 ? getContentParam(1) : 0;
        m_ContentCharaSelect.pCursor = createCursorSequence(
            (const INT*)JiminyMemo::CursorSeqTbl, 0x10, 0x11, JiminyMemo::CharaSelect::CursorPosiOffset,
            m_ContentCharaSelect.iCursorPosiNum, m_ContentCharaSelect.iListNum, 0, 0, 1, 0x11, 0x20002,
            m_ContentCharaSelect.iCursorCurrent, pList);
        if (m_ContentCharaSelect.pCursor && getPrevContentMode() == 10) {
            getContentScroll(1, posi);
            m_ContentCharaSelect.pCursor->setShiftVector(posi);
            m_ContentCharaSelect.pCursor->setPosiId(m_ContentCharaSelect.iCursorCurrent);
        }
        createMarkSequence(0x93, listNum, -1, 0, 0x11, pList, pSeqCtrl);
        for (INT i = 0; i < listNum; i++) {
            if (pSeqCtrl[i]) {
                if (!CJiminyMemoFlag::isContentFlag(m_ContentCharaSelect.iEventFlagId + i, 4) &&
                    CJiminyMemoFlag::isContentFlag(m_ContentCharaSelect.iEventFlagId + i, 2)) {
                    pSeqCtrl[i]->replaceSeq(1, m_pLadName, 0x94);
                }
                pSeqCtrl[i]->enableDisp(CJiminyMemoFlag::isContentNew(m_ContentCharaSelect.iEventFlagId + i) != 0);
            }
        }
        m_ContentCharaSelect.pNewMarkSeq = pSeqCtrl;
        pSeqCtrl += listNum;
        ScrollBarInit(pSeqCtrl, 0x10, JiminyMemo::CharaSelect::NoteScrollBarPosi, 150.0f, pList);
        m_ContentCharaSelect.pScrollBar = pSeqCtrl;
        if (m_ContentCharaSelect.pCursor) {
            CursorPosition cp;
            m_ContentCharaSelect.pCursor->getSubVector(cp, 1, 0);
            m_ContentCharaSelect.fCursorIntervalY = cp.y;
        }
        if (getPlateState(1) == -1) {
            setPlateState(1, 0);
            setPlateText(1, titleNo + kind);
        }
        bCursorShift = TRUE;
        setContentStep(30);
        setContentTimer(16.0f);
        break;
    }
    case 30:
        if (getContentTimer() <= 0.0f) {
            if (m_ContentCharaSelect.pSeqCtrl) {
                changeStateLoopSequence(m_ContentCharaSelect.pSeqCtrl, m_ContentCharaSelect.iSeqCtrlNum);
            }
            setTelopMessage(
                CCtdMan::getText(isModeJiminyMemo() ? 0x700113 : 0x700122));
            setContentStep(40);
        }
        break;
    case 40:
        if (_isButtonTrg(0x800, 0)) {
            setContentStep(50);
            JiminyMemo::playSE(4);
            setNextContentMode(-1);
        } else if (m_ContentCharaSelect.pCursor) {
            if (_isTrgOK(0)) {
                if (CJiminyMemoFlag::isContentOpen(m_ContentCharaSelect.iEventFlagId +
                                                   m_ContentCharaSelect.iCursorCurrent)) {
                    setContentStep(50);
                    setNextContentMode(10);
                    setContentParam(1, m_ContentCharaSelect.iCursorCurrent);
                    setContentScroll(1, m_ContentCharaSelect.pCursor->getShiftVector());
                    JiminyMemo::playSE(2);
                } else {
                    JiminyMemo::playSE(5);
                }
            } else if (_isTrgCANCEL(0)) {
                setContentStep(50);
                setNextContentMode(4);
                JiminyMemo::playSE(4);
            } else if (isButtonRptLimitStop(0x204000, 0,
                                            m_ContentCharaSelect.iCursorCurrent == m_ContentCharaSelect.iListNum - 1)) {
                m_ContentCharaSelect.iCursorCurrent++;
                if (m_ContentCharaSelect.iCursorCurrent >= m_ContentCharaSelect.iListNum) {
                    m_ContentCharaSelect.iCursorCurrent = 0;
                }
                bCursorShift = TRUE;
            } else if (isButtonRptLimitStop(0x101000, 0, m_ContentCharaSelect.iCursorCurrent == 0)) {
                m_ContentCharaSelect.iCursorCurrent--;
                if (m_ContentCharaSelect.iCursorCurrent < 0) {
                    m_ContentCharaSelect.iCursorCurrent = m_ContentCharaSelect.iListNum - 1;
                }
                bCursorShift = TRUE;
            } else if (_isButtonTrg(8, 0)) {
                bCursorShift = nextPage(m_ContentCharaSelect.pCursor, &m_ContentCharaSelect.iCursorCurrent,
                                        m_ContentCharaSelect.iListNum, m_ContentCharaSelect.iCursorPosiNum,
                                        m_ContentCharaSelect.fCursorIntervalY);
            } else if (_isButtonTrg(4, 0)) {
                bCursorShift = prevPage(m_ContentCharaSelect.pCursor, &m_ContentCharaSelect.iCursorCurrent,
                                        m_ContentCharaSelect.iListNum, m_ContentCharaSelect.iCursorPosiNum,
                                        m_ContentCharaSelect.fCursorIntervalY);
            }
            if (bCursorShift) {
                m_ContentCharaSelect.pCursor->setPosiId(m_ContentCharaSelect.iCursorCurrent);
                JiminyMemo::playSE(1);
            }
        }
        break;
    case 50: {
        INT nextContent;
        if (m_ContentCharaSelect.pList) {
            m_ContentCharaSelect.pList->setOutState(m_ContentCharaSelect.pList->getSideOutId(getNextContentMode() == 4));
        }
        if (m_ContentCharaSelect.pCursor) m_ContentCharaSelect.pCursor->setState(4);
        if (m_ContentCharaSelect.pSeqCtrl) {
            changeStateOutSequence(m_ContentCharaSelect.pSeqCtrl, m_ContentCharaSelect.iSeqCtrlNum);
        }
        nextContent = getNextContentMode();
        if (nextContent == 4) {
            setPlateState(1, 2);
        } else if (nextContent == -1) {
            setPlateState(0, 2);
            setPlateState(1, 2);
        }
        setTelopMessage("");
        setContentStep(60);
        setContentTimer(16.0f);
        break;
    }
    case 60:
        if (!CRcf::isCompleteLoadRCF(2, 0xC1)) break;
        if (getContentTimer() <= 0.0f) setContentStep(80);
        break;
    case 80:
    case 90:
        if (getNextContentMode() == 4) CRsrcDB::removeGroup(2, isModeJiminyMemo() ? 0xC1 : 0xC2);
        if (m_ContentCharaSelect.pList) delete m_ContentCharaSelect.pList;
        m_ContentCharaSelect.pList = NULL;
        if (m_ContentCharaSelect.pCursor) delete m_ContentCharaSelect.pCursor;
        m_ContentCharaSelect.pCursor = NULL;
        if (m_ContentCharaSelect.pString) {
            delete[] m_ContentCharaSelect.pString;
            m_ContentCharaSelect.pString = NULL;
        }
        if (m_ContentCharaSelect.pSeqCtrl) {
            for (INT i = 0; i < m_ContentCharaSelect.iSeqCtrlNum; i++) {
                if (m_ContentCharaSelect.pSeqCtrl[i]) delete m_ContentCharaSelect.pSeqCtrl[i];
                m_ContentCharaSelect.pSeqCtrl[i] = NULL;
            }
            if (m_ContentCharaSelect.pSeqCtrl) delete[] m_ContentCharaSelect.pSeqCtrl;
            m_ContentCharaSelect.pSeqCtrl = NULL;
        }
        setContentStep(100);
        break;
    case 100:
        break;
    }
    if (m_ContentCharaSelect.pList) m_ContentCharaSelect.pList->main(getFrameRateF());
    if (m_ContentCharaSelect.pCursor) m_ContentCharaSelect.pCursor->main();
    if (bCursorShift) {
        if (m_ContentCharaSelect.pCursor) {
            INT top = (INT)m_ContentCharaSelect.pCursor->getShiftVector().y / (INT)m_ContentCharaSelect.fCursorIntervalY;
            replaceNoteListWithOpenCheck(m_ContentCharaSelect.pString, m_ContentCharaSelect.pCTCodeTbl,
                                         m_ContentCharaSelect.iEventFlagId, m_ContentCharaSelect.iCursorPosiNum,
                                         m_ContentCharaSelect.iListNum, top);
            replaceNoteListMark(m_ContentCharaSelect.pNewMarkSeq, m_ContentCharaSelect.iEventFlagId,
                                m_ContentCharaSelect.iCursorPosiNum, m_ContentCharaSelect.iListNum, top, 0x10, 9);
            ScrollBarChange(m_ContentCharaSelect.pScrollBar, JiminyMemo::CharaSelect::NoteScrollBarPosi.y, 150.0f,
                            m_ContentCharaSelect.iCursorPosiNum, m_ContentCharaSelect.iListNum, top);
        }
    }
}
namespace JiminyMemo {
extern CT_LOWER* CharacterManualTbl[15];
extern CT_LOWER* RikuCharacterManualTbl[3];
extern ILLUST_TEX_DATA* CharaIllustTextureTbl[15];
extern ILLUST_TEX_DATA* RikuCharaIllustTextureTbl[3];
namespace CharaView {
extern INT NoteSeqData[5];
extern INT EnemyNoteSeqData[5];
}
}
void CJiminyMemo::contentCharaView() {
    INT eventFlagId = isModeDReport() ? 0x7F : 0x17;
    switch (getContentStep()) {
    case 20: {
        INT charaNo;
        CT_LOWER* ctTbl;
        INT kindNo;
        INT listNum;
        ILLUST_TEX_DATA* pictTbl;
        INT EnemyNoteKind;
        FVECTOR posi;
        INT EnemyDownNumId;
        D2::C2DSeqCtrl** pSeqCtrl;
        D2::C2DFnt** pString;
        CMemo2DSequence* pList;
        USHORT groupId;
        USHORT rsrcId;
        charaNo = getContentParam(1);
        kindNo = getContentParam(0);
        m_ContentCharaView.iKindNo = kindNo;
        m_ContentCharaView.iCharaNo = charaNo;
        if (isModeJiminyMemo()) {
            ctTbl = JiminyMemo::CharacterManualTbl[kindNo];
            listNum = JiminyMemo::CharaNumTbl[kindNo];
            pictTbl = JiminyMemo::CharaIllustTextureTbl[kindNo];
            EnemyNoteKind = 14;
            EnemyDownNumId = charaNo;
        } else {
            ctTbl = JiminyMemo::RikuCharacterManualTbl[kindNo];
            listNum = JiminyMemo::RikuCharaNumTbl[kindNo];
            pictTbl = JiminyMemo::RikuCharaIllustTextureTbl[kindNo];
            EnemyNoteKind = 2;
            EnemyDownNumId = charaNo + 0x23;
        }
        m_ContentCharaView.pCTCodeTbl = ctTbl;
        m_ContentCharaView.iListNum = listNum;
        m_ContentCharaView.iCurrentTextLine = 2;
        m_ContentCharaView.iStringNum = 9;
        pString = new (__FILE__ "(8999)") D2::C2DFnt*[m_ContentCharaView.iStringNum];
        if (pString) {
            for (INT i = 0; i < m_ContentCharaView.iStringNum; i++) pString[i] = NULL;
            m_ContentCharaView.pString = pString;
        }
        m_ContentCharaView.iSeqCtrlNum = m_ContentCharaView.iStringNum + 2;
        pSeqCtrl = new (__FILE__ "(9006)") D2::C2DSeqCtrl*[m_ContentCharaView.iSeqCtrlNum];
        if (pSeqCtrl) {
            for (INT i = 0; i < m_ContentCharaView.iSeqCtrlNum; i++) pSeqCtrl[i] = NULL;
            m_ContentCharaView.pSeqCtrl = pSeqCtrl;
        }
        splitMessage(&m_ContentCharaView.iTextLineNum, &m_ContentCharaView.pTextLineBuf,
                     CCtdMan::getText(JiminyMemo::getCT0014Code(ctTbl[charaNo])), 0x18);
        pList = createSequence(kindNo == EnemyNoteKind ? JiminyMemo::CharaView::EnemyNoteSeqData
                                                       : JiminyMemo::CharaView::NoteSeqData,
                               0x10, 0xB, 0, NULL);
        if (pList) {
            m_ContentCharaView.pList = pList;
            pList->setInState(0);
            for (INT i = 0; i < 5 && i + 2 < m_ContentCharaView.iTextLineNum; i++) {
                createListSequence(pList, 0x10, 0x23, i + 2, pSeqCtrl++, pString++, 0x8049481F, 0x11, 0x21000,
                                   m_ContentCharaView.pTextLineBuf[i + 2]);
            }
            if (m_ContentCharaView.iTextLineNum > 0) {
                createListSequence(pList, 0x10, 0x23, 7, pSeqCtrl++, pString++, 0x8049481F, 0x11, 0x21000,
                                   m_ContentCharaView.pTextLineBuf[0]);
            }
            if (kindNo == EnemyNoteKind) {
                createListSequence(pList, 0x10, 0x23, 9, pSeqCtrl++, pString++, 0x807F7F7F, 0xC, 0x40,
                                   CCtdMan::getText(0x702C01));
                sprintf(m_ContentCharaView.DefeatEnemyStr, "%4d", CJiminyMemoFlag::getDownEnemyNum(EnemyDownNumId));
                createListSequence(pList, 0x10, 0x23, 0xC, pSeqCtrl++, pString, 0x8000777F, 0xE, 0x41,
                                   m_ContentCharaView.DefeatEnemyStr);
            } else if (m_ContentCharaView.iTextLineNum >= 2) {
                createListSequence(pList, 0x10, 0x23, 9, pSeqCtrl++, pString++, 0x8049481F, 0xE, 0x21000,
                                   m_ContentCharaView.pTextLineBuf[1]);
            }
        }
        if (pList) {
            createMarkSequence(0xD0, 1, 0x23, 0xA, 0x10, pList, pSeqCtrl);
            m_ContentCharaView.pTopArrowSeq = *pSeqCtrl++;
            createMarkSequence(0xC3, 1, 0x23, 0xB, 0x10, pList, pSeqCtrl);
            m_ContentCharaView.pBottomArrowSeq = *pSeqCtrl;
            CMemo2DCard* pChara = new (__FILE__ "(9108)") CMemo2DCard(m_pLadName);
            if (pChara) {
                m_ContentCharaView.pChara = pChara;
                pChara->setLayer(0x10);
                pChara->enableColorCtrl(TRUE);
                pChara->setParent(pList);
                D2::C2DMan::getLayPosition(m_pLadFileName, 0x23, 8, posi, 0);
                pChara->setBasePosition(posi);
                pChara->setLocalPosition();
                pChara->enableDisp(TRUE);
                pChara->setState(1);
                pChara->createPicture(pictTbl[charaNo].No, pictTbl[charaNo].Posi);
            }
        }
        if (getPlateState(3) == -1) setPlateState(3, 0);
        setPlateNumSeq(7, charaNo + 1);
        setPlateNumSeq(8, listNum);
        m_ContentCharaModelView.init();
        groupId = 10;
        if (isModeJiminyMemo()) {
            USHORT hostGroupId = JiminyMemo::getHostGroupId();
            USHORT hostRsrcId = JiminyMemo::getHostRsrcId();
            rsrcId = JiminyMemo::CharaModelResourceTbl[kindNo][charaNo].ResourceNo & 0xFFF;
            if ((hostGroupId != groupId || rsrcId != hostRsrcId) && CRsrcDB::hasRsrcGroup(hostGroupId, hostRsrcId)) {
                CRsrcDB::removeGroup(hostGroupId, hostRsrcId);
            }
            if (!CRsrcDB::hasRsrcGroup(groupId, rsrcId)) CRcf::reqLoadRCFFront(groupId, rsrcId, 0, NULL, 0);
        } else {
            rsrcId = JiminyMemo::RikuCharaModelResourceTbl[kindNo][charaNo].ResourceNo & 0xFFF;
            CRcf::reqLoadRCFFront(groupId, rsrcId, 0, NULL, 0);
        }
        m_ContentCharaView.iGroupId = groupId;
        m_ContentCharaView.iRsrcId = rsrcId;
        setContentTimer(16.0f);
        setContentStep(30);
        break;
    }
    case 30:
        if (getContentTimer() <= 0.0f) {
            INT eventFlagOffset = 0;
            INT kindNo = m_ContentCharaView.iKindNo;
            if (isModeJiminyMemo()) {
                for (INT i = 0; i < kindNo; i++) eventFlagOffset += JiminyMemo::CharaNumTbl[i];
            } else {
                for (INT i = 0; i < kindNo; i++) eventFlagOffset += JiminyMemo::RikuCharaNumTbl[i];
            }
            CJiminyMemoFlag::setContentTouchFlag(eventFlagId + eventFlagOffset + m_ContentCharaView.iCharaNo);
            m_ContentCharaView.iPrevNo = getPrevCharaId(eventFlagId + eventFlagOffset);
            m_ContentCharaView.iNextNo = getNextCharaId(eventFlagId + eventFlagOffset);
            if (m_ContentCharaView.pSeqCtrl) {
                changeStateLoopSequence(m_ContentCharaView.pSeqCtrl, m_ContentCharaView.iSeqCtrlNum);
            }
            setTelopMessage(CCtdMan::getText(isModeJiminyMemo() ? 0x700114 : 0x700123));
            setPlateState(5, 0);
            setContentStep(40);
        }
        break;
    case 40: {
        INT ModViewState = contentCharaView_ModelView();
        INT bTouch;
        if (ModViewState == 1) break;
        bTouch = FALSE;
        if (_isButtonTrg(0x800, 0) || ModViewState == -1) {
            setContentStep(50);
            JiminyMemo::playSE(4);
            setNextContentMode(-1);
        } else if (_isTrgCANCEL(0)) {
            setContentStep(50);
            setNextContentMode(9);
            JiminyMemo::playSE(4);
        } else if (m_ContentCharaView.iPrevNo >= 0 && _isButtonTrg(1, 0)) {
            setContentStep(50);
            setNextContentMode(10);
            setContentParam(1, m_ContentCharaView.iPrevNo);
            JiminyMemo::playSE(2);
        } else if (m_ContentCharaView.iNextNo >= 0 && _isButtonTrg(2, 0)) {
            setContentStep(50);
            setNextContentMode(10);
            setContentParam(1, m_ContentCharaView.iNextNo);
            JiminyMemo::playSE(2);
        } else if (_isButtonRpt(0x204000, 0)) {
            INT limit = m_ContentCharaView.iTextLineNum - 5;
            if (limit >= 3) {
                m_ContentCharaView.iCurrentTextLine++;
                if (limit < m_ContentCharaView.iCurrentTextLine) {
                    m_ContentCharaView.iCurrentTextLine = limit;
                } else {
                    bTouch = TRUE;
                }
            }
        } else if (_isButtonRpt(0x101000, 0)) {
            m_ContentCharaView.iCurrentTextLine--;
            if (m_ContentCharaView.iCurrentTextLine < 2) {
                m_ContentCharaView.iCurrentTextLine = 2;
            } else {
                bTouch = TRUE;
            }
        } else if (_isButtonTrg(8, 0)) {
            bTouch = nextPage(&m_ContentCharaView.iCurrentTextLine, m_ContentCharaView.iTextLineNum, 5);
        } else if (_isButtonTrg(4, 0)) {
            bTouch = prevPage(&m_ContentCharaView.iCurrentTextLine, 2, 5);
        } else if (_isButtonTrg(0x10, 0)) {
            m_ContentCharaModelView.init();
            m_ContentCharaModelView.iStep = 20;
            JiminyMemo::playSE(2);
        }
        if (bTouch == TRUE) {
            INT textListNum = m_ContentCharaView.iTextLineNum - 2;
            if (textListNum > 5) textListNum = 5;
            replaceNoteList(m_ContentCharaView.pString,
                            (const CHAR**)m_ContentCharaView.pTextLineBuf + m_ContentCharaView.iCurrentTextLine,
                            textListNum);
            JiminyMemo::playSE(1);
        }
        break;
    }
    case 50: {
        INT nextContent;
        if (m_ContentCharaView.pList) m_ContentCharaView.pList->setOutState(2);
        if (m_ContentCharaView.pChara) m_ContentCharaView.pChara->setState(3);
        if (m_ContentCharaView.pSeqCtrl) {
            changeStateOutSequence(m_ContentCharaView.pSeqCtrl, m_ContentCharaView.iSeqCtrlNum);
        }
        nextContent = getNextContentMode();
        if (nextContent == 9) {
            setPlateState(3, 2);
        } else if (nextContent == -1) {
            setPlateState(0, 2);
            setPlateState(1, 2);
            setPlateState(3, 2);
        }
        setPlateState(5, -1);
        setTelopMessage("");
        setContentStep(60);
        setContentTimer(16.0f);
        break;
    }
    case 60:
        if (getContentTimer() <= 0.0f) setContentStep(80);
        break;
    case 90:
        m_ContentCharaModelView.iStep = 90;
        contentCharaView_ModelView();
    case 80:
        if (m_ContentCharaView.pList) delete m_ContentCharaView.pList;
        m_ContentCharaView.pList = NULL;
        if (m_ContentCharaView.pChara) delete m_ContentCharaView.pChara;
        m_ContentCharaView.pChara = NULL;
        if (m_ContentCharaView.pString) {
            delete[] m_ContentCharaView.pString;
            m_ContentCharaView.pString = NULL;
        }
        if (m_ContentCharaView.pSeqCtrl) {
            for (INT i = 0; i < m_ContentCharaView.iSeqCtrlNum; i++) {
                if (m_ContentCharaView.pSeqCtrl[i]) delete m_ContentCharaView.pSeqCtrl[i];
                m_ContentCharaView.pSeqCtrl[i] = NULL;
            }
            if (m_ContentCharaView.pSeqCtrl) delete[] m_ContentCharaView.pSeqCtrl;
            m_ContentCharaView.pSeqCtrl = NULL;
            m_ContentCharaView.pBottomArrowSeq = NULL;
            m_ContentCharaView.pTopArrowSeq = NULL;
        }
        if (m_ContentCharaView.pTextLineBuf) {
            for (INT i = 0; i < m_ContentCharaView.iTextLineNum; i++) {
                if (m_ContentCharaView.pTextLineBuf[i]) com_free(m_ContentCharaView.pTextLineBuf[i]);
                m_ContentCharaView.pTextLineBuf[i] = NULL;
            }
            if (m_ContentCharaView.pTextLineBuf) com_free(m_ContentCharaView.pTextLineBuf);
            m_ContentCharaView.pTextLineBuf = NULL;
        }
        setContentStep(81);
    case 81: {
        USHORT groupId = m_ContentCharaView.iGroupId;
        USHORT rsrcId = m_ContentCharaView.iRsrcId;
        if (isModeJiminyMemo()) {
            USHORT hostGroupId = JiminyMemo::getHostGroupId();
            USHORT hostRsrcId = JiminyMemo::getHostRsrcId();
            if (groupId == hostGroupId && rsrcId == hostRsrcId) {
                setContentStep(100);
            } else {
                if (!CRcf::isCompleteLoadRCF(groupId, rsrcId)) break;
                CRsrcDB::removeGroup(groupId, rsrcId);
                setContentStep(82);
            }
        } else {
            if (!CRcf::isCompleteLoadRCF(groupId, rsrcId)) break;
            CRsrcDB::removeGroup(groupId, rsrcId);
            setContentStep(82);
        }
        break;
    }
    case 82:
        if (isModeJiminyMemo() && getNextContentMode() == 9) {
            setContentStep(83);
        } else {
            setContentStep(100);
        }
        break;
    case 83:
        CRcf::reqLoadRCFFront((USHORT)JiminyMemo::getHostGroupId(), (USHORT)JiminyMemo::getHostRsrcId(), 0, NULL, 0);
        setContentStep(100);
        break;
    case 100:
        break;
    }
    if (m_ContentCharaView.pList) m_ContentCharaView.pList->main(getFrameRateF());
    if (m_ContentCharaView.pChara) m_ContentCharaView.pChara->main(getFrameRateF());
    if (m_ContentCharaView.pBottomArrowSeq) {
        m_ContentCharaView.pBottomArrowSeq->enableDisp(m_ContentCharaView.iCurrentTextLine <
                                                       m_ContentCharaView.iTextLineNum - 5);
    }
    if (m_ContentCharaView.pTopArrowSeq) {
        m_ContentCharaView.pTopArrowSeq->enableDisp(m_ContentCharaView.iCurrentTextLine > 2);
    }
}
INT CJiminyMemo::getPrevCharaId(INT inId) {
    return getPrevContentId(inId, work(0x3A0), work(0x39C));
}
INT CJiminyMemo::getNextCharaId(INT inId) {
    return getNextContentId(inId, work(0x3A0), work(0x39C));
}
namespace JiminyMemo {
namespace CharaView {
extern INT DuskyNoteSeqData[5];
extern FVECTOR ModelPosi;
}
}
/* Parked: 6 lines: three FP registers of case 40 (DEG2RAD(2.0f) constant, 1/4096 constant, the rotation load:
   original f4/f1/f3, ours f1/f3/f4). Everything else matches.
   tried: line-marker layout (both table arms written out, flags 0x8000 then 0x4000, setter order, ry split over
   3 statements), literal vs DEG2RAD constant, * 1/4096 vs / 4096, operand orders, ry as one or two statements;
   rotation as one/two/three statements, const ry;
   tools/permuter.py (fixed wrapper, target draft on, other drafts hidden) 8 min: no improvement. */
#if 0
INT CJiminyMemo::contentCharaView_ModelView() {
    INT rval = TRUE;
    INT charaNo;
    INT kindNo;
    USHORT groupId;
    USHORT rsrcId;
    FLOAT scale;
    FLOAT posiY;
    FLOAT rotY;
    USHORT resrcFlag;
    USHORT notSemiFlag;
    USHORT fade2PathFlag;
    CMemo2DSequence* pList;
    CMemo3DObj* pObj;
    switch (work(0x3C8)) {
    case 20:
        charaNo = getContentParam(1);
        kindNo = getContentParam(0);
        groupId = *(USHORT*)&work(0x3AC);
        rsrcId = *(USHORT*)&work(0x3AE);
        if (isModeJiminyMemo()) {
            scale = JiminyMemo::CharaModelResourceTbl[kindNo][charaNo].ScaleFix8 * 0.00390625f;
            posiY = JiminyMemo::CharaModelResourceTbl[kindNo][charaNo].PosiYFix8 * 0.00390625f;
            rotY = JiminyMemo::CharaModelResourceTbl[kindNo][charaNo].RotYDeg;
            resrcFlag = JiminyMemo::CharaModelResourceTbl[kindNo][charaNo].ResourceNo;
        } else {
            scale = JiminyMemo::RikuCharaModelResourceTbl[kindNo][charaNo].ScaleFix8 * 0.00390625f;
            posiY = JiminyMemo::RikuCharaModelResourceTbl[kindNo][charaNo].PosiYFix8 * 0.00390625f;
            rotY = JiminyMemo::RikuCharaModelResourceTbl[kindNo][charaNo].RotYDeg;
            resrcFlag = JiminyMemo::RikuCharaModelResourceTbl[kindNo][charaNo].ResourceNo;
        }
        notSemiFlag = resrcFlag & 0x8000;
        fade2PathFlag = resrcFlag & 0x4000;
        if ((isModeJiminyMemo() && groupId == JiminyMemo::getHostGroupId() && rsrcId == JiminyMemo::getHostRsrcId()) ||
            CRcf::isCompleteLoadRCF(groupId, rsrcId)) {
            CMemo2DSequence* pDusty = createSequence(JiminyMemo::CharaView::DuskyNoteSeqData, 0x11, 0x21, 0, NULL);
            if (pDusty) {
                work(0x3C4) = (INT)pDusty;
                pDusty->setInState(0);
            }
            pObj = new (com_malloc_d(sizeof(CMemo3DObj), __FILE__ "(9474)",
                                     ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })))
                CMemo3DObj(this, groupId, rsrcId, 0x12, FALSE);
            if (pObj) {
                FVECTOR posi;
                work(0x3C0) = pObj->getHandle();
                posi.set(JiminyMemo::CharaView::ModelPosi.x, posiY, JiminyMemo::CharaView::ModelPosi.z);
                pObj->setPosition(posi);
                pObj->setRotateY(DEG2RAD(rotY));
                pObj->setScale(scale);
                pObj->setFadeCountMax(12.0f);
                pObj->setNotSemiFlag(notSemiFlag);
                pObj->setFade2PathFlag(fade2PathFlag);
            }
            setContentTimer(16.0f);
            work(0x3C8) = 30;
        }
        break;
    case 30:
        if (getContentTimer() <= 0.0f) work(0x3C8) = 40;
        break;
    case 40:
        pObj = getTaskPointer<CMemo3DObj*>(work(0x3C0));
        if (_isButtonTrg(0x800, 0)) {
            work(0x3CC) = -1;
            work(0x3C8) = 50;
            JiminyMemo::playSE(4);
        } else if (_isButtonTrg(0x20, 0)) {
            work(0x3CC) = 0;
            work(0x3C8) = 50;
            JiminyMemo::playSE(4);
        } else if (pObj) {
            FLOAT ry = (FLOAT)_getAnalogLX(0) * DEG2RAD(2.0f) / 4096.0f * getFrameRateF();
            ry = Math3D::LimitPI(pObj->getRotateY() + ry);
            pObj->setRotateY(ry);
        }
        break;
    case 50: {
        CMemo3DObj* pObj = getTaskPointer<CMemo3DObj*>(work(0x3C0));
        if (pObj) pObj->setErase();
        if (work(0x3C4)) ((CMemo2DSequence*)work(0x3C4))->setOutState(2);
        setContentTimer(16.0f);
        work(0x3C8) = 60;
        break;
    }
    case 60:
        if (getContentTimer() <= 0.0f) work(0x3C8) = 80;
        break;
    case 80:
    case 90: {
        CMemo3DObj* pObj = getTaskPointer<CMemo3DObj*>(work(0x3C0));
        if (pObj) pObj->out();
        if (work(0x3C4)) delete (CMemo2DSequence*)work(0x3C4);
        work(0x3C4) = 0;
        work(0x3C8) = 100;
        break;
    }
    case 100:
        rval = work(0x3CC);
        break;
    default:
        rval = FALSE;
        break;
    }
    if (work(0x3C4)) ((CMemo2DSequence*)work(0x3C4))->main(getFrameRateF());
    return rval;
}
#else
/* placeholder: rodata of contentCharaView_ModelView__11CJiminyMemo */
__asm__(".rdata\n");
inline void _roPh35() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE(__FILE__ "(9474)");
}
__asm__(".rdata\n.align 4\n.word .L0033464C\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L0033480C\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n");
__asm__(".word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334830\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334918\n.word .L00334A00\n");
__asm__(".word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334960\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n");
__asm__(".word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334984\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n");
__asm__(".word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334984\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n.word .L00334A00\n");
__asm__(".word .L003349F8\n.byte 0x00,0x00,0x00,0x00\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", contentCharaView_ModelView__11CJiminyMemo);
#endif
__asm__(".rdata\n"); /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
namespace JiminyMemo {
namespace Minigame {
extern INT NoteSeqData[5];
}
}
/* Parked: 34 lines, all stack offsets: the six hoisted string addresses (strBGlider..strBPanic, &scoreStr) are
   spilled in another slot order (GCSE/PRE creates their pseudos in hash-table order: ours BPanic first, the
   original BGlider first). Everything else matches (time math via the Com.h(59) seconds inline, string table).
   tried: aggregate initializer for scoreStr, if vs ternary for the scores, unused posi removed (worse); RTL dump
   shows pseudos 567.. made by gcse; scoreStr declared
   with the other locals + element stores (34/99); tools/permuter.py (fixed wrapper, target on, others hidden)
   9 min: no gain (earlier "score 1" was a wrapper bug); gcse hash order (see memory gcse-pre-hash-order). */
#if 0
/* PARTIAL: the original's System/Com.h(59) inline: seconds to time quanta */
static inline INT _sec2tq(INT inSec) { return inSec * 3000; }
void CJiminyMemo::contentMinigame() {
    switch (getContentStep()) {
    case 20: {
        FVECTOR posi;
        D2::C2DSeqCtrl** pSeqCtrl;
        D2::C2DFnt** pString;
        CMemo2DSequence* pList;
        m_ContentMinigame.iStringNum = 13;
        pString = new (__FILE__ "(9647)") D2::C2DFnt*[m_ContentMinigame.iStringNum];
        if (pString) {
            for (INT i = 0; i < m_ContentMinigame.iStringNum; i++) pString[i] = NULL;
            m_ContentMinigame.pString = pString;
        }
        m_ContentMinigame.iSeqCtrlNum = m_ContentMinigame.iStringNum + 6;
        pSeqCtrl = new (__FILE__ "(9654)") D2::C2DSeqCtrl*[m_ContentMinigame.iSeqCtrlNum];
        if (pSeqCtrl) {
            for (INT i = 0; i < m_ContentMinigame.iSeqCtrlNum; i++) pSeqCtrl[i] = NULL;
            m_ContentMinigame.pSeqCtrl = pSeqCtrl;
        }
        pList = createSequence(JiminyMemo::Minigame::NoteSeqData, 0x10, 0x12, 0, NULL);
        if (pList) {
            int bFlag[6];
            CHAR* pPoint;
            CHAR* pMin;
            CHAR* pSec;
            CHAR* pTime;
            CHAR* pMask;
            INT bg;
            INT wd;
            INT eb;
            INT eb_min;
            INT eb_sec;
            INT tj;
            INT bp;
            m_ContentMinigame.pList = pList;
            pList->setInState(0);
            for (INT i = 0; i < 6; i++) {
                createListSequence(pList, 0x11, 0x11, i + 1, pSeqCtrl++, pString++, 0x8049481F, 16, 0x21000,
                                   JiminyMemo::getOpenCloseStringData(0x700301 + i,
                                                                      CJiminyMemoFlag::isContentOpen(0x18A + i)));
            }
            for (INT i = 0; i < 2; i++) {
                createListSequence(pList, 0x11, 0x11, i + 0x12, pSeqCtrl, pString, 0x807F7F7F, 16, 0x21000,
                                   CCtdMan::getText(0x700307 + i));
                pSeqCtrl++;
                pString++;
            }
            bFlag[0] = CGameWork::getByte(0, 4) >= 2;
            bFlag[1] = CGameWork::getByte(0, 5) >= 2;
            bFlag[2] = CGameWork::getByte(0, 6) >= 3;
            bFlag[3] = CGameWork::getByte(0, 7) != 0;
            bFlag[4] = CGameWork::getByte(0, 8) != 0;
            bFlag[5] = CJiminyMemoFlag::isContentOpen(0x18F);
            pPoint = CCtdMan::getText(0x700309);
            pMin = CCtdMan::getText(0x70030A);
            pSec = CCtdMan::getText(0x70030B);
            pTime = CCtdMan::getText(0x70030C);
            pMask = CCtdMan::getText(0x70030D);
            bg = bFlag[0] == 1 ? CGameWork::getInt(0, 0x2E) : 0;
            wd = bFlag[1] == 1 ? CGameWork::getInt(0, 0x2F) : 0;
            eb = CGameWork::getInt(0, 0x30);
            tj = bFlag[3] == 1 ? CGameWork::getInt(0, 0x31) : 0;
            bp = bFlag[4] == 1 ? CGameWork::getInt(0, 0x32) : 0;
            if (CJiminyMemoFlag::isContentOpen(0x18A)) {
                sprintf(m_ContentMinigame.strBGlider, "%d%s", bg, pPoint);
            } else {
                sprintf(m_ContentMinigame.strBGlider, "%s", pMask);
            }
            if (CJiminyMemoFlag::isContentOpen(0x18B)) {
                sprintf(m_ContentMinigame.strWDive, "%d%s", wd, pPoint);
            } else {
                sprintf(m_ContentMinigame.strWDive, "%s", pMask);
            }
            if (CJiminyMemoFlag::isContentOpen(0x18C) && bFlag[2] == 1 && eb > 0) {
                eb_min = eb / _sec2tq(60);
                eb_sec = (eb - eb_min * _sec2tq(60)) / _sec2tq(1);
                sprintf(m_ContentMinigame.strEBee, "%s%d%s%02d%s%02d", pTime, eb_min, pMin, eb_sec, pSec,
                        (eb - eb_min * _sec2tq(60) - eb_sec * _sec2tq(1)) * 1000 / _sec2tq(1) / 10);
            } else {
                sprintf(m_ContentMinigame.strEBee, "%s--%s--%s--", pTime, pMin, pSec);
            }
            if (CJiminyMemoFlag::isContentOpen(0x18D)) {
                sprintf(m_ContentMinigame.strTJump, "%d%s", tj, pPoint);
            } else {
                sprintf(m_ContentMinigame.strTJump, "%s", pMask);
            }
            if (CJiminyMemoFlag::isContentOpen(0x18E)) {
                sprintf(m_ContentMinigame.strBPanic, "%d%s", bp, pPoint);
            } else {
                sprintf(m_ContentMinigame.strBPanic, "%s", pMask);
            }
            CHAR* scoreStr[5] = {
                m_ContentMinigame.strBGlider,
                m_ContentMinigame.strWDive,
                m_ContentMinigame.strEBee,
                m_ContentMinigame.strTJump,
                m_ContentMinigame.strBPanic,
            };
            for (INT i = 0; i < 5; i++) {
                createListSequence(pList, 0x11, 0x11, i + 0xD, pSeqCtrl, pString, 0x8049481F, 16, 0x21001,
                                   scoreStr[i]);
                pSeqCtrl++;
                pString++;
            }
            bFlag[0] = CGameWork::getByte(0, 4) >= 2;
            bFlag[1] = CGameWork::getByte(0, 5) >= 2;
            bFlag[2] = CGameWork::getByte(0, 6) >= 2;
            bFlag[3] = CGameWork::getByte(0, 7) >= 2;
            bFlag[4] = CGameWork::getByte(0, 8) >= 2;
            bFlag[5] = CJiminyMemoFlag::isContentOpen(0x18F);
            createMarkSequence(0x92, 6, 0x11, 7, 0x11, pList, pSeqCtrl);
            for (INT i = 0; i < 6; i++) {
                if (pSeqCtrl[i]) pSeqCtrl[i]->enableDisp(bFlag[i] != 0);
            }
        }
        setPlateState(0, 0);
        setPlateText(0, 0x700300);
        setContentStep(30);
        setContentTimer(16.0f);
        break;
    }
    case 30:
        if (getContentTimer() <= 0.0f) {
            for (INT i = 0; i < 6; i++) {
                if (CJiminyMemoFlag::isContentNew(0x18A + i)) CJiminyMemoFlag::setContentTouchFlag(0x18A + i);
            }
            if (m_ContentMinigame.pSeqCtrl) {
                changeStateLoopSequence(m_ContentMinigame.pSeqCtrl, m_ContentMinigame.iSeqCtrlNum);
            }
            setTelopMessage(CCtdMan::getText(0x700115));
            setContentStep(40);
        }
        break;
    case 40:
        if (_isButtonTrg(0x800, 0)) {
            setContentStep(50);
            JiminyMemo::playSE(4);
            setNextContentMode(-1);
        } else if (_isTrgCANCEL(0)) {
            setNextContentMode(0);
            setContentStep(50);
            JiminyMemo::playSE(4);
        }
        break;
    case 50:
        if (m_ContentMinigame.pList) m_ContentMinigame.pList->setOutState(2);
        if (m_ContentMinigame.pSeqCtrl) {
            changeStateOutSequence(m_ContentMinigame.pSeqCtrl, m_ContentMinigame.iSeqCtrlNum);
        }
        setTelopMessage("");
        setPlateState(0, 2);
        setContentStep(60);
        setContentTimer(16.0f);
        break;
    case 60:
        if (getContentTimer() <= 0.0f) setContentStep(80);
        break;
    case 80:
    case 90:
        if (m_ContentMinigame.pList) delete m_ContentMinigame.pList;
        m_ContentMinigame.pList = NULL;
        if (m_ContentMinigame.pString) {
            delete[] m_ContentMinigame.pString;
            m_ContentMinigame.pString = NULL;
        }
        if (m_ContentMinigame.pSeqCtrl) {
            for (INT i = 0; i < m_ContentMinigame.iSeqCtrlNum; i++) {
                if (m_ContentMinigame.pSeqCtrl[i]) delete m_ContentMinigame.pSeqCtrl[i];
                m_ContentMinigame.pSeqCtrl[i] = NULL;
            }
            if (m_ContentMinigame.pSeqCtrl) delete[] m_ContentMinigame.pSeqCtrl;
            m_ContentMinigame.pSeqCtrl = NULL;
        }
        setContentStep(100);
        break;
    case 100:
        break;
    }
    if (m_ContentMinigame.pList) m_ContentMinigame.pList->main(getFrameRateF());
}
#else
/* placeholder: rodata of contentMinigame__11CJiminyMemo */
__asm__(".rdata\n");
inline void _roPh36() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE(__FILE__ "(9647)");
    TRACE(__FILE__ "(9654)");
    TRACE("%d%s");
    TRACE("%s");
    TRACE("%s%d%s%02d%s%02d");
    TRACE("%s--%s--%s--");
}
__asm__(".rdata\n.align 4\n.word .L00334AC4\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L0033527C\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n");
__asm__(".word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L0033532C\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L00335388\n.word .L003354D0\n");
__asm__(".word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003353EC\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n");
__asm__(".word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L00335410\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n");
__asm__(".word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L00335410\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n.word .L003354D0\n");
__asm__(".word .L003354D0\n.byte 0x00,0x00,0x00,0x00\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", contentMinigame__11CJiminyMemo);
#endif
/* Parked: final group (destructors = key functions, all weak class inlines, getTaskPointer<T>, static init, the
   .rodata after the vtables): gcc emits all of it at the end of the object, so it can only come from C at once.
   Drafted in full (build/jm/grp4.cpp; recipe in memory jiminymemo-unit-lessons): every function matches except
   mainInit/mainLoadWait/mainIn (tail store order/FP regs, 7/2/2 lines), and the ~175 namespace-scope const tables
   (static, emitted after the vtables only when C references them) need contentMenu, contentCollection,
   contentCharaView_ModelView and contentMinigame in C, which are parked. tried: see those functions' notes. */
#if 0
template <class T> T CJiminyMemo::getTaskPointer(HANDLE inHandle) {
    T p = NULL;
    if (inHandle) {
        p = (T)CTaskMan::getTask(inHandle);
        if (p && p->isOut()) p = NULL;
    }
    return p;
}
BOOL CMemo3DObj::isErase() { return m_bEraseModel; }
BOOL CMemo3DObj::isLoadEnable() { return m_bLoadEnable; }
BOOL CMemo3DObj::isLightOff() { return m_bLightOff; }
void CMemo3DObj::setErase() { m_bEraseModel = TRUE; }
FLOAT CMemo3DObj::getRotateY() { return m_fRotY; }
FVECTOR& CMemo3DObj::getPosition() { return m_vPosition; }
void CMemo3DObj::setScale(FLOAT inScale) { m_fScale = inScale; }
FLOAT CMemo3DObj::getScale() { return m_fScale; }
void CMemo3DObj::setLightOff(BOOL inFlag) { m_bLightOff = inFlag; }
void CMemo3DObj::setFadeCountMax(FLOAT inMax) { m_fFadeCountMax = inMax; }
void CMemo3DObj::setNotSemiFlag(BOOL inFlag) { m_bNotSemiFlag = inFlag; }
void CMemo3DObj::setFade2PathFlag(BOOL inFlag) { m_bFade2PathFlag = inFlag; }
BOOL CMemo2DLayout::isEndLayer() { return D2::C2DMan::isEnd(m_hLayer); }
void CMemo2DLayout::setStateFlag(INT inOn, INT inOff) { m_uiStateFlags = (m_uiStateFlags & ~inOff) | inOn; }
INT CMemo2DLayout::isStateFlag(INT inFlags) { return m_uiStateFlags & inFlags; }
BOOL CMemo2DLayout::isFontSeqTbl(INT inSeqNo) { return m_pSeqNoTbl[inSeqNo].CTCode != 0; }
INT CMemo2DLayout::getSeqCtrlNum() { return m_iSeqCtrlNum; }
INT CMemo2DLayout::getSeqNum() { return m_iSeqNum; }
void CMemo2DSequence::setBasePosition(const FVECTOR& inPosi) { m_vBasePosition = inPosi; }
FVECTOR& CMemo2DSequence::getBasePosition() { return m_vBasePosition; }
void CMemo2DSequence::setShiftPosition(const FVECTOR& inShift) { m_vShiftPosition = inShift; }
void CMemo2DSequence::setAimPosition(const FVECTOR& inAim) { m_vAimPosition = inAim; }
void CMemo2DSequence::setPosition(const FVECTOR& inPosi) {
    m_vPosition = inPosi;
    setPos(m_vPosition);
}
void CMemo2DSequence::setLocalPosition(const FVECTOR& inPosi) {
    m_vPosition = inPosi;
    setLocalPos(m_vPosition);
}
void CMemo2DSequence::setState(INT inState) { m_iState = inState; }
void CMemo2DSequence::setStateFlag(USHORT inOrFlag, USHORT inAndFlag) { m_iStateFlag = (m_iStateFlag & ~inAndFlag) | inOrFlag; }
USHORT CMemo2DSequence::isStateFlag(USHORT inFlag) { return m_iStateFlag & inFlag; }
void CMemo2DSequence::setMoveTimer(FLOAT inTimeQ) { m_fMoveTimer = inTimeQ; }
FLOAT CMemo2DSequence::getMoveTimer() { return m_fMoveTimer; }
void CMemo2DSequence::addMoveTimer(FLOAT inTimeQ) { m_fMoveTimer += inTimeQ; }
INT CMemo2DSequence::getSideInId(INT inId) { return inId == 1 ? 0 : 3; }
INT CMemo2DSequence::getSideOutId(INT inId) { return inId == 1 ? 2 : 4; }
void CMemo2DCursor::setShiftVector(const FVECTOR& inShiftPosi) { m_vShiftPosi = inShiftPosi; }
FVECTOR& CMemo2DCursor::getShiftVector() { return m_vShiftPosi; }
FVECTOR& CMemo2DCursor::getAimVector() { return m_vAimPosi; }
FVECTOR& CMemo2DCursor::getPosition() { return m_vPosi; }
void CMemo2DCursor::setSeqNo(INT inSeqNo) { m_iLadTbl[3] = inSeqNo; }
void CMemo2DCard::setBasePosition(const FVECTOR& inPosition) { m_vBasePosition = inPosition; }
FVECTOR& CMemo2DCard::getBasePosition() { return m_vBasePosition; }
void CMemo2DCard::setShiftPosition(const FVECTOR& inShift) { m_vShiftPosition = inShift; }
FVECTOR& CMemo2DCard::getShiftPosition() { return m_vShiftPosition; }
void CMemo2DCard::setScale(FLOAT inScale) {
    m_fScale = inScale;
    setScl(inScale);
}
void CJiminyMemo::setStateFlag(INT inOn, INT inOff) { m_uiStateFlags = (m_uiStateFlags & ~inOff) | inOn; }
INT CJiminyMemo::isStateFlag(INT inFlags) { return m_uiStateFlags & inFlags; }
BOOL CJiminyMemo::isModeJiminyMemo() { return m_iPlayerMode == 0; }
BOOL CJiminyMemo::isModeDReport() { return m_iPlayerMode != 0; }
INT CJiminyMemo::getLoadResourceIndex() { return m_iPlayerMode != 0; }
void CJiminyMemo::setContentStep(INT inStep) { m_ContentCommon.iStep = inStep; }
INT CJiminyMemo::getContentStep() { return m_ContentCommon.iStep; }
void CJiminyMemo::setContentTimer(FLOAT inTimer) { m_ContentCommon.fTimer = inTimer; }
FLOAT CJiminyMemo::getContentTimer() { return m_ContentCommon.fTimer; }
void CJiminyMemo::setNextContentMode(INT inMode) { m_ContentCommon.iNextContent = inMode; }
INT CJiminyMemo::getNextContentMode() { return m_ContentCommon.iNextContent; }
void CJiminyMemo::setPrevContentMode(INT inMode) { m_ContentCommon.iPrevContent = inMode; }
INT CJiminyMemo::getPrevContentMode() { return m_ContentCommon.iPrevContent; }
void CJiminyMemo::setContentParam(INT inId, INT inParam) {
    _jiminyMemoAssertParamId(inId, 1185);
    m_ContentCommon.param[inId] = inParam;
}
INT CJiminyMemo::getContentParam(INT inId) {
    _jiminyMemoAssertParamId(inId, 1193);
    return m_ContentCommon.param[inId];
}
void CJiminyMemo::setContentScroll(INT inIdx, const FVECTOR& inPosi) {
    m_ContentCommon.scroll[inIdx].x = inPosi.x;
    m_ContentCommon.scroll[inIdx].y = inPosi.y;
}
void CJiminyMemo::getContentScroll(INT inIdx, FVECTOR& outPosi) {
    outPosi.x = m_ContentCommon.scroll[inIdx].x;
    outPosi.y = m_ContentCommon.scroll[inIdx].y;
    outPosi.z = 0.0f;
    outPosi.w = 1.0f;
}
#else
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
#endif
namespace JiminyMemo {
namespace Base {
extern const INT LayoutData[];
extern const LayoutDataParam SeqData[];
extern FVECTOR TelopMessagePosition;
extern CursorPosition L1R1DigitSlashOffset;
}
}
/* Parked (final group): the inlined mainInit tail (posi._q = 0; posi.w = 1.0f; setContentStep(20); setContentScroll(0/1))
   schedules the step/w/scroll stores in another order: in ours setContentStep's store and the posi.w stack store are
   dependent (sched2 dump), in the original they are not (step lands before w in mainInit, after scroll[1] in mainIn).
   mainInit 7 lines standalone, mainLoadWait 2, mainIn 2; the rest (layout setup, digit seqs, telop font) matches.
   tried: all orders of the tail statements (scored on both callers), block-scoped tail FVECTOR (30), step first
   (contentCollection recipe); tools/permuter.py 2 runs on mainInit (12/10, no gain). The weak functions can only
   come from C together (key functions + deferred inline output), so they stay INCLUDE_ASM as a group. */
#if 0
inline void CJiminyMemo::mainInit() {
    CMemo2DLayout* pLay;
    D2::C2DSeqCtrl* pMess;
    FVECTOR posi;
    pLay = new (__FILE__ "(3948)") CMemo2DLayout(this);
    if (pLay) {
        D2::C2DSeqCtrl* pSeq;
        m_hLayout = pLay->getHandle();
        pLay->setup(m_pLadName, m_pLadFileName, JiminyMemo::Base::LayoutData, 0xC, JiminyMemo::Base::SeqData, 0xF);
        pLay->init();
        pSeq = pLay->getSeqCtrl(8);
        if (pSeq) {
            FLOAT offsetX;
            FLOAT offsetY;
            offsetX = JiminyMemo::Base::L1R1DigitNumOffset.x;
            offsetY = JiminyMemo::Base::L1R1DigitNumOffset.y;
            pSeq->addSeqShift(m_pLadName, 0x95, offsetX + 0.0f, offsetY);
            pSeq->addSeqShift(m_pLadName, 0x95, offsetX + 16.0f, offsetY);
            pSeq->addSeqShift(m_pLadName, 0x95, offsetX + 32.0f, offsetY);
            offsetX = JiminyMemo::Base::L1R1DigitSlashOffset.x;
            offsetY = JiminyMemo::Base::L1R1DigitSlashOffset.y;
            pSeq->addSeqShift(m_pLadName, 0xA1, offsetX + 0.0f, offsetY);
            offsetX = JiminyMemo::Base::L1R1DigitDenomiOffset.x;
            offsetY = JiminyMemo::Base::L1R1DigitDenomiOffset.y;
            pSeq->addSeqShift(m_pLadName, 0x95, offsetX + 0.0f, offsetY);
            pSeq->addSeqShift(m_pLadName, 0x95, offsetX + 16.0f, offsetY);
            pSeq->addSeqShift(m_pLadName, 0x95, offsetX + 32.0f, offsetY);
        }
        pSeq = pLay->getSeqCtrl(9);
        if (pSeq) {
            FLOAT offsetX;
            FLOAT offsetY;
            offsetX = JiminyMemo::Base::L1R1DigitNumOffset.x;
            offsetY = JiminyMemo::Base::L1R1DigitNumOffset.y;
            pSeq->addSeqShift(m_pLadName, 0x95, offsetX + 0.0f, offsetY);
            pSeq->addSeqShift(m_pLadName, 0x95, offsetX + 16.0f, offsetY);
            pSeq->addSeqShift(m_pLadName, 0x95, offsetX + 32.0f, offsetY);
            offsetX = JiminyMemo::Base::L1R1DigitSlashOffset.x;
            offsetY = JiminyMemo::Base::L1R1DigitSlashOffset.y;
            pSeq->addSeqShift(m_pLadName, 0xA1, offsetX + 0.0f, offsetY);
            offsetX = JiminyMemo::Base::L1R1DigitDenomiOffset.x;
            offsetY = JiminyMemo::Base::L1R1DigitDenomiOffset.y;
            pSeq->addSeqShift(m_pLadName, 0x95, offsetX + 0.0f, offsetY);
            pSeq->addSeqShift(m_pLadName, 0x95, offsetX + 16.0f, offsetY);
            pSeq->addSeqShift(m_pLadName, 0x95, offsetX + 32.0f, offsetY);
        }
        pSeq = pLay->getSeqCtrl(2);
        if (pSeq) {
            D2::C2DMan::getLayPosition(m_pLadFileName, 0x2C, 1, posi, 0);
            pSeq->setLocalPos(posi);
        }
        pSeq = pLay->getSeqCtrl(5);
        if (pSeq) {
            D2::C2DMan::getLayPosition(m_pLadFileName, 0x2C, 2, posi, 0);
            pSeq->setLocalPos(posi);
        }
    }
    pMess = new (__FILE__ "(4012)") D2::C2DSeqCtrl;
    if (pMess) {
        D2::C2DFnt* pFnt;
        m_pMessSeq = pMess;
        pMess->setLayer(0x11);
        pMess->enableDisp(TRUE);
        pMess->addSeq(m_pLadName, 1);
        pMess->setPos(JiminyMemo::Base::TelopMessagePosition);
        pFnt = new (__FILE__ "(4022)") D2::C2DFnt(0, 0, NULL, 0x10, JiminyMemo::Base::DefaultMessage, 0x40);
        if (pFnt) {
            m_pMessString = pFnt;
            pMess->addFnt(m_pLadName, pFnt);
        }
        pMess->enableDisp(TRUE);
    }
    JiminyMemo::playSE(3);
    setLayoutState(m_hLayout, 1);
    setContentStep(20);
    posi._q = 0;
    posi.w = 1.0f;
    setContentScroll(0, posi);
    setContentScroll(1, posi);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", mainInit__11CJiminyMemo);
#endif
#if 0
void CJiminyMemo::mainLoadWait() {
    if (CRcf::isCompleteLoadRCF(2, JiminyMemo::RsrcId[getLoadResourceIndex()])) {
        mainInit();
        m_iStep = 30;
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", mainLoadWait__11CJiminyMemo);
#endif
#if 0
void CJiminyMemo::mainIn() {
    mainInit();
    m_iStep = 30;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", mainIn__11CJiminyMemo);
#endif
/* Parked: final group, see the note above getTaskPointer<T> */
#if 0
inline void CJiminyMemo::mainInWait() {
    if (checkLayoutState(m_hLayout, 4)) {
        setLayoutState(m_hLayout, 3);
        m_iStep = 40;
    }
}
inline void CJiminyMemo::mainLoop() {}
inline void CJiminyMemo::mainOut() {
    setLayoutState(m_hLayout, 4);
    m_iStep = 60;
}
inline void CJiminyMemo::mainOutWait() {
    if (checkLayoutState(m_hLayout, 0x10)) {
        setLayoutState(m_hLayout, 7);
        CJiminyMemoFlag::checkMiniGameHiScore();
        CJiminyMemoFlag::cleanTouchFlag();
        m_iStep = 80;
    }
}
/* replaceNoteListString: inline before its first user (replaceNoteListWithOpenCheck); static init: the
   namespace statics `static FLOAT JiminyRotY = DEG2RAD(-120.0f)` (Menu) and `ModelRotY = DEG2RAD(180.0f)`
   (CharaView) with .bss from source */
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", mainInWait__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", mainLoop__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", mainOut__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", mainOutWait__11CJiminyMemo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", replaceNoteListString__11CJiminyMemoPQ22D26C2DFntPCc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/JiminyMemo", _GLOBAL_$I$c__KHCOM_MainApp_Form_Menu_JiminyMemo.cppbgbaaa);
#endif
/* Placeholder for the end of .rodata: the vtables (their key functions are still asm) and the static const
   tables gcc emits after them (cardDataTbl with its strings, the JiminyMemo:: tables). Bytes from the original,
   relocated words as symbols. */
__asm__(
    ".rdata\n"
    ".align 3\n"
    ".globl _vt$11CJiminyMemo\n"
    "_vt$11CJiminyMemo:\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".word _$_11CJiminyMemo\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word main__11CJiminyMemo\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word disp__5CTask\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word after__5CTask\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word onOut__5CTask\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word getTypeID__C5CTask\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _vt$11CMemo2DCard\n"
    "_vt$11CMemo2DCard:\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".word _$_11CMemo2DCard\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addSeq__Q22D210C2DSeqCtrlPCcUiUi\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addSeq__Q22D210C2DSeqCtrlPCcUi\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addFnt__Q22D210C2DSeqCtrlPCcUiPQ22D26C2DFnt\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addFnt__Q22D210C2DSeqCtrlPCcPQ22D26C2DFnt\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceSeq__Q22D210C2DSeqCtrlUiPCcUiUi\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceSeq__Q22D210C2DSeqCtrlUiPCcUi\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceFnt__Q22D210C2DSeqCtrlUiPCcUiPQ22D26C2DFnt\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceFnt__Q22D210C2DSeqCtrlUiPCcPQ22D26C2DFnt\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addSeqShift__Q22D210C2DSeqCtrlPCcUiUiff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addSeqShift__Q22D210C2DSeqCtrlPCcUiff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addFntShift__Q22D210C2DSeqCtrlPCcUiPQ22D26C2DFntff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addFntShift__Q22D210C2DSeqCtrlPCcPQ22D26C2DFntff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceSeqShift__Q22D210C2DSeqCtrlUiPCcUiUiff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceSeqShift__Q22D210C2DSeqCtrlUiPCcUiff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceFntShift__Q22D210C2DSeqCtrlUiPCcUiPQ22D26C2DFntff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceFntShift__Q22D210C2DSeqCtrlUiPCcPQ22D26C2DFntff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word shiftSeqPos__Q22D210C2DSeqCtrlUiff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word setFntPos__Q22D210C2DSeqCtrlUiff\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _vt$13CMemo2DCursor\n"
    "_vt$13CMemo2DCursor:\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".word _$_13CMemo2DCursor\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addSeq__Q22D210C2DSeqCtrlPCcUiUi\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addSeq__Q22D210C2DSeqCtrlPCcUi\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addFnt__Q22D210C2DSeqCtrlPCcUiPQ22D26C2DFnt\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addFnt__Q22D210C2DSeqCtrlPCcPQ22D26C2DFnt\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceSeq__Q22D210C2DSeqCtrlUiPCcUiUi\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceSeq__Q22D210C2DSeqCtrlUiPCcUi\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceFnt__Q22D210C2DSeqCtrlUiPCcUiPQ22D26C2DFnt\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceFnt__Q22D210C2DSeqCtrlUiPCcPQ22D26C2DFnt\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addSeqShift__Q22D210C2DSeqCtrlPCcUiUiff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addSeqShift__Q22D210C2DSeqCtrlPCcUiff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addFntShift__Q22D210C2DSeqCtrlPCcUiPQ22D26C2DFntff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addFntShift__Q22D210C2DSeqCtrlPCcPQ22D26C2DFntff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceSeqShift__Q22D210C2DSeqCtrlUiPCcUiUiff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceSeqShift__Q22D210C2DSeqCtrlUiPCcUiff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceFntShift__Q22D210C2DSeqCtrlUiPCcUiPQ22D26C2DFntff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceFntShift__Q22D210C2DSeqCtrlUiPCcPQ22D26C2DFntff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word shiftSeqPos__Q22D210C2DSeqCtrlUiff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word setFntPos__Q22D210C2DSeqCtrlUiff\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _vt$15CMemo2DSequence\n"
    "_vt$15CMemo2DSequence:\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".word _$_15CMemo2DSequence\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addSeq__Q22D210C2DSeqCtrlPCcUiUi\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addSeq__Q22D210C2DSeqCtrlPCcUi\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addFnt__Q22D210C2DSeqCtrlPCcUiPQ22D26C2DFnt\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addFnt__Q22D210C2DSeqCtrlPCcPQ22D26C2DFnt\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceSeq__Q22D210C2DSeqCtrlUiPCcUiUi\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceSeq__Q22D210C2DSeqCtrlUiPCcUi\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceFnt__Q22D210C2DSeqCtrlUiPCcUiPQ22D26C2DFnt\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceFnt__Q22D210C2DSeqCtrlUiPCcPQ22D26C2DFnt\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addSeqShift__Q22D210C2DSeqCtrlPCcUiUiff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addSeqShift__Q22D210C2DSeqCtrlPCcUiff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addFntShift__Q22D210C2DSeqCtrlPCcUiPQ22D26C2DFntff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word addFntShift__Q22D210C2DSeqCtrlPCcPQ22D26C2DFntff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceSeqShift__Q22D210C2DSeqCtrlUiPCcUiUiff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceSeqShift__Q22D210C2DSeqCtrlUiPCcUiff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceFntShift__Q22D210C2DSeqCtrlUiPCcUiPQ22D26C2DFntff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word replaceFntShift__Q22D210C2DSeqCtrlUiPCcPQ22D26C2DFntff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word shiftSeqPos__Q22D210C2DSeqCtrlUiff\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word setFntPos__Q22D210C2DSeqCtrlUiff\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _vt$13CMemo2DLayout\n"
    "_vt$13CMemo2DLayout:\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".word _$_13CMemo2DLayout\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word main__13CMemo2DLayout\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word disp__5CTask\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word after__5CTask\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word onOut__5CTask\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word getTypeID__C5CTask\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _vt$10CMemo3DObj\n"
    "_vt$10CMemo3DObj:\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".word _$_10CMemo3DObj\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word main__10CMemo3DObj\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word disp__10CMemo3DObj\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word after__5CTask\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word onOut__5CTask\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".word getTypeID__C5CTask\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x4b,0x69,0x6e,0x67,0x64,0x6f,0x6d,0x20,0x4b,0x65,0x79,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x54,0x68,0x72,0x65,0x65,0x20,0x57,0x69,0x73,0x68,0x65,0x73,0x00,0x00,0x00,0x00\n"
    ".byte 0x43,0x72,0x61,0x62,0x63,0x6c,0x61,0x77,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x50,0x75,0x6d,0x70,0x6b,0x69,0x6e,0x68,0x65,0x61,0x64,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x46,0x61,0x69,0x72,0x79,0x20,0x48,0x61,0x72,0x70,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x57,0x69,0x73,0x68,0x69,0x6e,0x67,0x20,0x53,0x74,0x61,0x72,0x00,0x00,0x00,0x00\n"
    ".byte 0x53,0x70,0x65,0x6c,0x6c,0x62,0x69,0x6e,0x64,0x65,0x72,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x4d,0x65,0x74,0x61,0x6c,0x20,0x43,0x68,0x6f,0x63,0x6f,0x62,0x6f,0x00,0x00,0x00\n"
    ".byte 0x4f,0x6c,0x79,0x6d,0x70,0x69,0x61,0x00\n"
    ".byte 0x4c,0x69,0x6f,0x6e,0x68,0x65,0x61,0x72,0x74,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x4c,0x61,0x64,0x79,0x20,0x4c,0x75,0x63,0x6b,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x44,0x69,0x76,0x69,0x6e,0x65,0x20,0x52,0x6f,0x73,0x65,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x4f,0x61,0x74,0x68,0x6b,0x65,0x65,0x70,0x65,0x72,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x4f,0x62,0x6c,0x69,0x76,0x69,0x6f,0x6e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x55,0x6c,0x74,0x69,0x6d,0x61,0x20,0x57,0x65,0x61,0x70,0x6f,0x6e,0x00,0x00,0x00\n"
    ".byte 0x44,0x69,0x61,0x6d,0x6f,0x6e,0x64,0x20,0x44,0x75,0x73,0x74,0x00,0x00,0x00,0x00\n"
    ".byte 0x4f,0x6e,0x65,0x2d,0x57,0x69,0x6e,0x67,0x65,0x64,0x20,0x41,0x6e,0x67,0x65,0x6c\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x53,0x6f,0x75,0x6c,0x20,0x45,0x61,0x74,0x65,0x72,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x53,0x74,0x61,0x72,0x20,0x53,0x65,0x65,0x6b,0x65,0x72,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x4d,0x6f,0x6e,0x6f,0x63,0x68,0x72,0x6f,0x6d,0x65,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x46,0x6f,0x6c,0x6c,0x6f,0x77,0x20,0x74,0x68,0x65,0x20,0x57,0x69,0x6e,0x64,0x00\n"
    ".byte 0x48,0x69,0x64,0x64,0x65,0x6e,0x20,0x44,0x72,0x61,0x67,0x6f,0x6e,0x00,0x00,0x00\n"
    ".byte 0x50,0x68,0x6f,0x74,0x6f,0x6e,0x20,0x44,0x65,0x62,0x75,0x67,0x67,0x65,0x72,0x00\n"
    ".byte 0x42,0x6f,0x6e,0x64,0x20,0x6f,0x66,0x20,0x46,0x6c,0x61,0x6d,0x65,0x00,0x00,0x00\n"
    ".byte 0x2d,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x46,0x69,0x72,0x65,0x00,0x00,0x00,0x00\n"
    ".byte 0x42,0x6c,0x69,0x7a,0x7a,0x61,0x72,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x54,0x68,0x75,0x6e,0x64,0x65,0x72,0x00\n"
    ".byte 0x43,0x75,0x72,0x65,0x00,0x00,0x00,0x00\n"
    ".byte 0x47,0x72,0x61,0x76,0x69,0x74,0x79,0x00\n"
    ".byte 0x53,0x74,0x6f,0x70,0x00,0x00,0x00,0x00\n"
    ".byte 0x41,0x65,0x72,0x6f,0x00,0x00,0x00,0x00\n"
    ".byte 0x53,0x69,0x6d,0x62,0x61,0x00,0x00,0x00\n"
    ".byte 0x47,0x65,0x6e,0x69,0x65,0x00,0x00,0x00\n"
    ".byte 0x42,0x61,0x6d,0x62,0x69,0x00,0x00,0x00\n"
    ".byte 0x44,0x75,0x6d,0x62,0x6f,0x00,0x00,0x00\n"
    ".byte 0x54,0x69,0x6e,0x6b,0x65,0x72,0x20,0x42,0x65,0x6c,0x6c,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x4d,0x75,0x73,0x68,0x75,0x00,0x00,0x00\n"
    ".byte 0x43,0x6c,0x6f,0x75,0x64,0x00,0x00,0x00\n"
    ".byte 0x50,0x6f,0x74,0x69,0x6f,0x6e,0x00,0x00\n"
    ".byte 0x48,0x69,0x2d,0x50,0x6f,0x74,0x69,0x6f,0x6e,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x4d,0x65,0x67,0x61,0x2d,0x50,0x6f,0x74,0x69,0x6f,0x6e,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x45,0x74,0x68,0x65,0x72,0x00,0x00,0x00\n"
    ".byte 0x4d,0x65,0x67,0x61,0x2d,0x45,0x74,0x68,0x65,0x72,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x45,0x6c,0x69,0x78,0x69,0x72,0x00,0x00\n"
    ".byte 0x4d,0x65,0x67,0x61,0x6c,0x69,0x78,0x69,0x72,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x44,0x6f,0x6e,0x61,0x6c,0x64,0x20,0x44,0x75,0x63,0x6b,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x47,0x6f,0x6f,0x66,0x79,0x00,0x00,0x00\n"
    ".byte 0x41,0x6c,0x61,0x64,0x64,0x69,0x6e,0x00\n"
    ".byte 0x41,0x72,0x69,0x65,0x6c,0x00,0x00,0x00\n"
    ".byte 0x4a,0x61,0x63,0x6b,0x00,0x00,0x00,0x00\n"
    ".byte 0x50,0x65,0x74,0x65,0x72,0x20,0x50,0x61,0x6e,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x54,0x68,0x65,0x20,0x42,0x65,0x61,0x73,0x74,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x54,0x68,0x65,0x20,0x4b,0x69,0x6e,0x67,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x50,0x6c,0x75,0x74,0x6f,0x00,0x00,0x00\n"
    ".byte 0x47,0x75,0x61,0x72,0x64,0x20,0x41,0x72,0x6d,0x6f,0x72,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x54,0x72,0x69,0x63,0x6b,0x6d,0x61,0x73,0x74,0x65,0x72,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x4a,0x61,0x66,0x61,0x72,0x00,0x00,0x00\n"
    ".byte 0x4f,0x6f,0x67,0x69,0x65,0x20,0x42,0x6f,0x6f,0x67,0x69,0x65,0x00,0x00,0x00,0x00\n"
    ".byte 0x50,0x61,0x72,0x61,0x73,0x69,0x74,0x65,0x20,0x43,0x61,0x67,0x65,0x00,0x00,0x00\n"
    ".byte 0x55,0x72,0x73,0x75,0x6c,0x61,0x00,0x00\n"
    ".byte 0x44,0x72,0x61,0x67,0x6f,0x6e,0x20,0x4d,0x61,0x6c,0x65,0x66,0x69,0x63,0x65,0x6e\n"
    ".byte 0x74,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x44,0x61,0x72,0x6b,0x73,0x69,0x64,0x65,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x53,0x68,0x61,0x64,0x6f,0x77,0x00,0x00\n"
    ".byte 0x53,0x6f,0x6c,0x64,0x69,0x65,0x72,0x00\n"
    ".byte 0x4c,0x61,0x72,0x67,0x65,0x20,0x42,0x6f,0x64,0x79,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x52,0x65,0x64,0x20,0x4e,0x6f,0x63,0x74,0x75,0x72,0x6e,0x65,0x00,0x00,0x00,0x00\n"
    ".byte 0x42,0x6c,0x75,0x65,0x20,0x52,0x68,0x61,0x70,0x73,0x6f,0x64,0x79,0x00,0x00,0x00\n"
    ".byte 0x59,0x65,0x6c,0x6c,0x6f,0x77,0x20,0x4f,0x70,0x65,0x72,0x61,0x00,0x00,0x00,0x00\n"
    ".byte 0x47,0x72,0x65,0x65,0x6e,0x20,0x52,0x65,0x71,0x75,0x69,0x65,0x6d,0x00,0x00,0x00\n"
    ".byte 0x50,0x6f,0x77,0x65,0x72,0x77,0x69,0x6c,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x42,0x6f,0x75,0x6e,0x63,0x79,0x77,0x69,0x6c,0x64,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x41,0x69,0x72,0x20,0x53,0x6f,0x6c,0x64,0x69,0x65,0x72,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x42,0x61,0x6e,0x64,0x69,0x74,0x00,0x00\n"
    ".byte 0x46,0x61,0x74,0x20,0x42,0x61,0x6e,0x64,0x69,0x74,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x42,0x61,0x72,0x72,0x65,0x6c,0x20,0x53,0x70,0x69,0x64,0x65,0x72,0x00,0x00,0x00\n"
    ".byte 0x53,0x65,0x61,0x72,0x63,0x68,0x20,0x47,0x68,0x6f,0x73,0x74,0x00,0x00,0x00,0x00\n"
    ".byte 0x53,0x65,0x61,0x20,0x4e,0x65,0x6f,0x6e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x53,0x63,0x72,0x65,0x77,0x64,0x69,0x76,0x65,0x72,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x41,0x71,0x75,0x61,0x74,0x61,0x6e,0x6b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x57,0x69,0x67,0x68,0x74,0x20,0x4b,0x6e,0x69,0x67,0x68,0x74,0x00,0x00,0x00,0x00\n"
    ".byte 0x47,0x61,0x72,0x67,0x6f,0x79,0x6c,0x65,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x50,0x69,0x72,0x61,0x74,0x65,0x00,0x00\n"
    ".byte 0x41,0x69,0x72,0x20,0x50,0x69,0x72,0x61,0x74,0x65,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x44,0x61,0x72,0x6b,0x62,0x61,0x6c,0x6c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x44,0x65,0x66,0x65,0x6e,0x64,0x65,0x72,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x57,0x79,0x76,0x65,0x72,0x6e,0x00,0x00\n"
    ".byte 0x57,0x69,0x7a,0x61,0x72,0x64,0x00,0x00\n"
    ".byte 0x4e,0x65,0x6f,0x73,0x68,0x61,0x64,0x6f,0x77,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x57,0x68,0x69,0x74,0x65,0x20,0x4d,0x75,0x73,0x68,0x72,0x6f,0x6f,0x6d,0x00,0x00\n"
    ".byte 0x42,0x6c,0x61,0x63,0x6b,0x20,0x46,0x75,0x6e,0x67,0x75,0x73,0x00,0x00,0x00,0x00\n"
    ".byte 0x43,0x72,0x65,0x65,0x70,0x65,0x72,0x20,0x50,0x6c,0x61,0x6e,0x74,0x00,0x00,0x00\n"
    ".byte 0x54,0x6f,0x72,0x6e,0x61,0x64,0x6f,0x20,0x53,0x74,0x65,0x70,0x00,0x00,0x00,0x00\n"
    ".byte 0x43,0x72,0x65,0x73,0x63,0x65,0x6e,0x64,0x6f,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x43,0x61,0x72,0x64,0x20,0x53,0x6f,0x6c,0x64,0x69,0x65,0x72,0x00,0x00,0x00,0x00\n"
    ".byte 0x48,0x61,0x64,0x65,0x73,0x00,0x00,0x00\n"
    ".byte 0x48,0x6f,0x6f,0x6b,0x00,0x00,0x00,0x00\n"
    ".byte 0x52,0x69,0x6b,0x75,0x00,0x00,0x00,0x00\n"
    ".byte 0x41,0x78,0x65,0x6c,0x00,0x00,0x00,0x00\n"
    ".byte 0x4c,0x61,0x72,0x78,0x65,0x6e,0x65,0x00\n"
    ".byte 0x56,0x65,0x78,0x65,0x6e,0x00,0x00,0x00\n"
    ".byte 0x4d,0x61,0x72,0x6c,0x75,0x78,0x69,0x61,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x4c,0x65,0x78,0x61,0x65,0x75,0x73,0x00\n"
    ".byte 0x41,0x6e,0x73,0x65,0x6d,0x00,0x00,0x00\n"
    ".byte 0x5a,0x65,0x78,0x69,0x6f,0x6e,0x00,0x00\n"
    ".byte 0x58,0x65,0x6d,0x6e,0x61,0x73,0x00,0x00\n"
    ".byte 0x58,0x69,0x67,0x62,0x61,0x72,0x00,0x00\n"
    ".byte 0x58,0x61,0x6c,0x64,0x69,0x6e,0x00,0x00\n"
    ".byte 0x53,0x61,0x83,0xc7,0x78,0x00,0x00,0x00\n"
    ".byte 0x44,0x65,0x6d,0x79,0x78,0x00,0x00,0x00\n"
    ".byte 0x4c,0x75,0x78,0x6f,0x72,0x64,0x00,0x00\n"
    ".byte 0x52,0x6f,0x78,0x61,0x73,0x00,0x00,0x00\n"
    ".byte 0x47,0x6f,0x6c,0x64,0x20,0x43,0x61,0x72,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x50,0x6c,0x61,0x74,0x69,0x6e,0x75,0x6d,0x20,0x43,0x61,0x72,0x64,0x00,0x00,0x00\n"
    ".byte 0x43,0x61,0x72,0x64,0x20,0x53,0x6f,0x6c,0x64,0x69,0x65,0x72,0x20,0x53,0x70,0x61\n"
    ".byte 0x64,0x65,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x43,0x61,0x72,0x64,0x20,0x53,0x6f,0x6c,0x64,0x69,0x65,0x72,0x20,0x48,0x65,0x61\n"
    ".byte 0x72,0x74,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x83,0x4e,0x83,0x89,0x83,0x45,0x83,0x68,0x41,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x83,0x4e,0x83,0x89,0x83,0x45,0x83,0x68,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x83,0x6e,0x83,0x66,0x83,0x58,0x41,0x00\n"
    ".byte 0x83,0x6e,0x83,0x66,0x83,0x58,0x42,0x00\n"
    ".byte 0x83,0x74,0x83,0x62,0x83,0x4e,0x41,0x00\n"
    ".byte 0x83,0x74,0x83,0x62,0x83,0x4e,0x42,0x00\n"
    ".byte 0x83,0x74,0x83,0x62,0x83,0x4e,0x43,0x00\n"
    ".byte 0x83,0x74,0x83,0x62,0x83,0x4e,0x44,0x00\n"
    ".byte 0x83,0x41,0x83,0x4e,0x83,0x5a,0x83,0x8b,0x41,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x83,0x41,0x83,0x4e,0x83,0x5a,0x83,0x8b,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x83,0x89,0x83,0x4e,0x83,0x56,0x81,0x5b,0x83,0x6b,0x41,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x83,0x89,0x83,0x4e,0x83,0x56,0x81,0x5b,0x83,0x6b,0x42,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x83,0x89,0x83,0x4e,0x83,0x56,0x81,0x5b,0x83,0x6b,0x43,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x83,0x8a,0x83,0x4e,0x41,0x00,0x00,0x00\n"
    ".byte 0x83,0x8a,0x83,0x4e,0x42,0x00,0x00,0x00\n"
    ".byte 0x83,0x8a,0x83,0x4e,0x43,0x00,0x00,0x00\n"
    ".byte 0x83,0x8a,0x83,0x4e,0x44,0x00,0x00,0x00\n"
    ".byte 0x83,0x94,0x83,0x42,0x83,0x4e,0x83,0x5a,0x83,0x93,0x41,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x83,0x94,0x83,0x42,0x83,0x4e,0x83,0x5a,0x83,0x93,0x42,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x83,0x7d,0x81,0x5b,0x83,0x8b,0x81,0x5b,0x83,0x56,0x83,0x83,0x41,0x00,0x00,0x00\n"
    ".byte 0x83,0x7d,0x81,0x5b,0x83,0x8b,0x81,0x5b,0x83,0x56,0x83,0x83,0x42,0x00,0x00,0x00\n"
    ".byte 0x83,0x7d,0x81,0x5b,0x83,0x8b,0x81,0x5b,0x83,0x56,0x83,0x83,0x43,0x00,0x00,0x00\n"
    ".byte 0x83,0x7d,0x81,0x5b,0x83,0x8b,0x81,0x5b,0x83,0x56,0x83,0x83,0x32,0x00,0x00,0x00\n"
    ".byte 0x83,0x7d,0x81,0x5b,0x83,0x8b,0x81,0x5b,0x83,0x56,0x83,0x83,0x33,0x00,0x00,0x00\n"
    ".byte 0x83,0x8c,0x83,0x4e,0x83,0x5a,0x83,0x45,0x83,0x58,0x41,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x83,0x8c,0x83,0x4e,0x83,0x5a,0x83,0x45,0x83,0x58,0x42,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x83,0x8c,0x83,0x4e,0x83,0x5a,0x83,0x45,0x83,0x58,0x43,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x83,0x41,0x83,0x93,0x83,0x5a,0x83,0x80,0x41,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x83,0x41,0x83,0x93,0x83,0x5a,0x83,0x80,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x83,0x5b,0x83,0x4e,0x83,0x56,0x83,0x49,0x83,0x93,0x41,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x83,0x8a,0x83,0x8d,0x81,0x5b,0x83,0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x54,0x72,0x61,0x6e,0x71,0x75,0x69,0x6c,0x20,0x44,0x61,0x72,0x6b,0x6e,0x65,0x73\n"
    ".byte 0x73,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x54,0x65,0x65,0x6d,0x69,0x6e,0x67,0x20,0x44,0x61,0x72,0x6b,0x6e,0x65,0x73,0x73\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x46,0x65,0x65,0x62,0x6c,0x65,0x20,0x44,0x61,0x72,0x6b,0x6e,0x65,0x73,0x73,0x00\n"
    ".byte 0x41,0x6c,0x6d,0x69,0x67,0x68,0x74,0x79,0x20,0x44,0x61,0x72,0x6b,0x6e,0x65,0x73\n"
    ".byte 0x73,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x53,0x6c,0x65,0x65,0x70,0x69,0x6e,0x67,0x20,0x44,0x61,0x72,0x6b,0x6e,0x65,0x73\n"
    ".byte 0x73,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x4c,0x6f,0x6f,0x6d,0x69,0x6e,0x67,0x20,0x44,0x61,0x72,0x6b,0x6e,0x65,0x73,0x73\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x50,0x72,0x65,0x6d,0x69,0x75,0x6d,0x20,0x52,0x6f,0x6f,0x6d,0x00,0x00,0x00,0x00\n"
    ".byte 0x57,0x68,0x69,0x74,0x65,0x20,0x52,0x6f,0x6f,0x6d,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x42,0x6c,0x61,0x63,0x6b,0x20,0x52,0x6f,0x6f,0x6d,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x42,0x6f,0x74,0x74,0x6f,0x6d,0x6c,0x65,0x73,0x73,0x20,0x44,0x61,0x72,0x6b,0x6e\n"
    ".byte 0x65,0x73,0x73,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x52,0x6f,0x75,0x6c,0x65,0x74,0x74,0x65,0x20,0x52,0x6f,0x6f,0x6d,0x00,0x00,0x00\n"
    ".byte 0x4d,0x61,0x72,0x74,0x69,0x61,0x6c,0x20,0x57,0x61,0x6b,0x69,0x6e,0x67,0x00,0x00\n"
    ".byte 0x53,0x6f,0x72,0x63,0x65,0x72,0x6f,0x75,0x73,0x20,0x57,0x61,0x6b,0x69,0x6e,0x67\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x41,0x6c,0x63,0x68,0x65,0x6d,0x69,0x63,0x20,0x57,0x61,0x6b,0x69,0x6e,0x67,0x00\n"
    ".byte 0x4d,0x65,0x65,0x74,0x69,0x6e,0x67,0x20,0x47,0x72,0x6f,0x75,0x6e,0x64,0x00,0x00\n"
    ".byte 0x53,0x74,0x61,0x67,0x6e,0x61,0x6e,0x74,0x20,0x53,0x70,0x61,0x63,0x65,0x00,0x00\n"
    ".byte 0x53,0x74,0x72,0x6f,0x6e,0x67,0x20,0x49,0x6e,0x69,0x74,0x69,0x61,0x74,0x69,0x76\n"
    ".byte 0x65,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x4c,0x61,0x73,0x74,0x69,0x6e,0x67,0x20,0x44,0x61,0x7a,0x65,0x00,0x00,0x00,0x00\n"
    ".byte 0x43,0x61,0x6c,0x6d,0x20,0x42,0x6f,0x75,0x6e,0x74,0x79,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x47,0x75,0x61,0x72,0x64,0x65,0x64,0x20,0x54,0x72,0x6f,0x76,0x65,0x00,0x00,0x00\n"
    ".byte 0x46,0x61,0x6c,0x73,0x65,0x20,0x42,0x6f,0x75,0x6e,0x74,0x79,0x00,0x00,0x00,0x00\n"
    ".byte 0x4d,0x6f,0x6d,0x65,0x6e,0x74,0x27,0x73,0x20,0x52,0x65,0x70,0x72,0x69,0x65,0x76\n"
    ".byte 0x65,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x4d,0x69,0x6e,0x67,0x6c,0x69,0x6e,0x67,0x20,0x57,0x6f,0x72,0x6c,0x64,0x73,0x00\n"
    ".byte 0x4d,0x6f,0x6f,0x67,0x6c,0x65,0x20,0x52,0x6f,0x6f,0x6d,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x52,0x61,0x6e,0x64,0x6f,0x6d,0x20,0x4a,0x6f,0x6b,0x65,0x72,0x00,0x00,0x00,0x00\n"
    ".byte 0x4b,0x65,0x79,0x20,0x6f,0x66,0x20,0x42,0x65,0x67,0x69,0x6e,0x6e,0x69,0x6e,0x67\n"
    ".byte 0x73,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x4b,0x65,0x79,0x20,0x6f,0x66,0x20,0x47,0x75,0x69,0x64,0x61,0x6e,0x63,0x65,0x00\n"
    ".byte 0x4b,0x65,0x79,0x20,0x74,0x6f,0x20,0x54,0x72,0x75,0x74,0x68,0x00,0x00,0x00,0x00\n"
    ".byte 0x4b,0x65,0x79,0x20,0x74,0x6f,0x20,0x52,0x65,0x77,0x61,0x72,0x64,0x73,0x00,0x00\n"
    ".byte 0x54,0x72,0x61,0x76,0x65,0x72,0x73,0x65,0x20,0x54,0x6f,0x77,0x6e,0x00,0x00,0x00\n"
    ".byte 0x41,0x67,0x72,0x61,0x62,0x61,0x68,0x00\n"
    ".byte 0x4f,0x6c,0x79,0x6d,0x70,0x75,0x73,0x20,0x43,0x6f,0x6c,0x69,0x73,0x65,0x75,0x6d\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x57,0x6f,0x6e,0x64,0x65,0x72,0x6c,0x61,0x6e,0x64,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x4d,0x6f,0x6e,0x73,0x74,0x72,0x6f,0x00\n"
    ".byte 0x48,0x61,0x6c,0x6c,0x6f,0x77,0x65,0x65,0x6e,0x20,0x54,0x6f,0x77,0x6e,0x00,0x00\n"
    ".byte 0x41,0x74,0x6c,0x61,0x6e,0x74,0x69,0x63,0x61,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x4e,0x65,0x76,0x65,0x72,0x6c,0x61,0x6e,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x48,0x6f,0x6c,0x6c,0x6f,0x77,0x20,0x42,0x61,0x73,0x74,0x69,0x6f,0x6e,0x00,0x00\n"
    ".byte 0x31,0x30,0x30,0x20,0x41,0x63,0x72,0x65,0x20,0x57,0x6f,0x6f,0x64,0x73,0x00,0x00\n"
    ".byte 0x54,0x77,0x69,0x6c,0x69,0x67,0x68,0x74,0x20,0x54,0x6f,0x77,0x6e,0x00,0x00,0x00\n"
    ".byte 0x44,0x65,0x73,0x74,0x69,0x6e,0x79,0x20,0x49,0x73,0x6c,0x61,0x6e,0x64,0x73,0x00\n"
    ".byte 0x43,0x61,0x73,0x74,0x6c,0x65,0x20,0x4f,0x62,0x6c,0x69,0x76,0x69,0x6f,0x6e,0x00\n"
    ".byte 0x45,0x58,0x50,0x90,0xc2,0x8f,0xac,0x00\n"
    ".byte 0x45,0x58,0x50,0x90,0xc2,0x92,0x86,0x00\n"
    ".byte 0x45,0x58,0x50,0x90,0xc2,0x91,0xe5,0x00\n"
    ".byte 0x45,0x58,0x50,0x90,0xd4,0x8f,0xac,0x00\n"
    ".byte 0x45,0x58,0x50,0x90,0xd4,0x92,0x86,0x00\n"
    ".byte 0x45,0x58,0x50,0x90,0xd4,0x91,0xe5,0x00\n"
    ".byte 0x45,0x58,0x50,0x8b,0xe2,0x00,0x00,0x00\n"
    ".byte 0x48,0x50,0x8f,0xac,0x00,0x00,0x00,0x00\n"
    ".byte 0x48,0x50,0x91,0xe5,0x00,0x00,0x00,0x00\n"
    ".byte 0x4d,0x50,0x8f,0xac,0x00,0x00,0x00,0x00\n"
    ".byte 0x4d,0x50,0x91,0xe5,0x00,0x00,0x00,0x00\n"
    ".byte 0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x55,0x6e,0x6b,0x6e,0x6f,0x77,0x6e,0x20,0x52,0x6f,0x6f,0x6d,0x00,0x00,0x00,0x00\n"
    ".byte 0x43,0x6f,0x6e,0x71,0x75,0x65,0x72,0x6f,0x72,0x27,0x73,0x20,0x52,0x65,0x73,0x70\n"
    ".byte 0x69,0x74,0x65,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x52,0x6f,0x6f,0x6d,0x20,0x6f,0x66,0x20,0x52,0x65,0x77,0x61,0x72,0x64,0x73,0x00\n"
    ".globl cardDataTbl_004EA4F8\n"
    "cardDataTbl_004EA4F8:\n"
    ".byte 0x00,0x00,0x00,0x00,0x30,0x78,0x4e,0x00,0x11,0x02,0x00,0x03,0xe0,0x99,0x4e,0x00\n"
    ".byte 0x11,0x02,0x01,0x03,0xf0,0x99,0x4e,0x00,0x11,0x02,0x02,0x03,0x00,0x9a,0x4e,0x00\n"
    ".byte 0x11,0x02,0x03,0x03,0x10,0x9a,0x4e,0x00,0x11,0x02,0x04,0x03,0x20,0x9a,0x4e,0x00\n"
    ".byte 0x11,0x02,0x05,0x03,0x30,0x9a,0x4e,0x00,0x11,0x02,0x06,0x03,0x40,0x9a,0x4e,0x00\n"
    ".byte 0x11,0x02,0x07,0x03,0x50,0x9a,0x4e,0x00,0x11,0x03,0x00,0x03,0x60,0x9a,0x4e,0x00\n"
    ".byte 0x11,0x03,0x01,0x03,0x68,0x9a,0x4e,0x00,0x11,0x03,0x02,0x03,0x78,0x9a,0x4e,0x00\n"
    ".byte 0x11,0x03,0x03,0x03,0x88,0x9a,0x4e,0x00,0x11,0x03,0x04,0x03,0x98,0x9a,0x4e,0x00\n"
    ".byte 0x11,0x03,0x05,0x03,0xa8,0x9a,0x4e,0x00,0x11,0x03,0x06,0x03,0xb8,0x9a,0x4e,0x00\n"
    ".byte 0x11,0x03,0x07,0x03,0xc8,0x9a,0x4e,0x00,0x11,0x04,0x00,0x03,0xd8,0x9a,0x4e,0x00\n"
    ".byte 0x11,0x04,0x01,0x04,0xf0,0x9a,0x4e,0x00,0x11,0x04,0x02,0x03,0x00,0x9b,0x4e,0x00\n"
    ".byte 0x11,0x04,0x03,0x03,0x10,0x9b,0x4e,0x00,0x11,0x04,0x04,0x03,0x20,0x9b,0x4e,0x00\n"
    ".byte 0x11,0x04,0x05,0x03,0x30,0x9b,0x4e,0x00,0x11,0x04,0x06,0x03,0x40,0x9b,0x4e,0x00\n"
    ".byte 0x11,0x04,0x07,0x03,0x50,0x9b,0x4e,0x00,0x11,0x1a,0x00,0x00,0x60,0x9b,0x4e,0x00\n"
    ".byte 0x22,0x05,0x00,0x03,0x68,0x9b,0x4e,0x00,0x22,0x05,0x01,0x03,0x70,0x9b,0x4e,0x00\n"
    ".byte 0x22,0x05,0x02,0x03,0x80,0x9b,0x4e,0x00,0x22,0x05,0x03,0x03,0x88,0x9b,0x4e,0x00\n"
    ".byte 0x22,0x05,0x04,0x03,0x90,0x9b,0x4e,0x00,0x22,0x05,0x05,0x03,0x98,0x9b,0x4e,0x00\n"
    ".byte 0x22,0x05,0x06,0x03,0xa0,0x9b,0x4e,0x00,0x22,0x1a,0x02,0x00,0x60,0x9b,0x4e,0x00\n"
    ".byte 0x22,0x1a,0x01,0x00,0x60,0x9b,0x4e,0x00,0x33,0x05,0x07,0x03,0xa8,0x9b,0x4e,0x00\n"
    ".byte 0x33,0x06,0x00,0x03,0xb0,0x9b,0x4e,0x00,0x33,0x06,0x01,0x03,0xb8,0x9b,0x4e,0x00\n"
    ".byte 0x33,0x06,0x02,0x03,0xc0,0x9b,0x4e,0x00,0x33,0x06,0x03,0x03,0xc8,0x9b,0x4e,0x00\n"
    ".byte 0x33,0x06,0x04,0x03,0xd8,0x9b,0x4e,0x00,0x33,0x06,0x05,0x03,0xe0,0x9b,0x4e,0x00\n"
    ".byte 0x44,0x06,0x06,0x06,0xe8,0x9b,0x4e,0x00,0x44,0x06,0x07,0x06,0xf0,0x9b,0x4e,0x00\n"
    ".byte 0x44,0x07,0x00,0x02,0x00,0x9c,0x4e,0x00,0x44,0x07,0x01,0x02,0x10,0x9c,0x4e,0x00\n"
    ".byte 0x44,0x07,0x02,0x02,0x18,0x9c,0x4e,0x00,0x44,0x07,0x03,0x02,0x28,0x9c,0x4e,0x00\n"
    ".byte 0x44,0x07,0x04,0x02,0x30,0x9c,0x4e,0x00,0x55,0x07,0x05,0x02,0x40,0x9c,0x4e,0x00\n"
    ".byte 0x55,0x07,0x06,0x02,0x50,0x9c,0x4e,0x00,0x55,0x07,0x07,0x02,0x58,0x9c,0x4e,0x00\n"
    ".byte 0x55,0x08,0x00,0x02,0x60,0x9c,0x4e,0x00,0x55,0x08,0x01,0x02,0x68,0x9c,0x4e,0x00\n"
    ".byte 0x55,0x08,0x02,0x02,0x70,0x9c,0x4e,0x00,0x55,0x08,0x03,0x02,0x80,0x9c,0x4e,0x00\n"
    ".byte 0x55,0x08,0x04,0x04,0x90,0x9c,0x4e,0x00,0x55,0x08,0x05,0x02,0xa0,0x9c,0x4e,0x00\n"
    ".byte 0x66,0x08,0x06,0x06,0xa8,0x9c,0x4e,0x00,0x66,0x08,0x06,0x06,0xb8,0x9c,0x4e,0x00\n"
    ".byte 0x66,0x08,0x06,0x06,0xc8,0x9c,0x4e,0x00,0x66,0x08,0x06,0x06,0xd0,0x9c,0x4e,0x00\n"
    ".byte 0x66,0x08,0x06,0x06,0xe0,0x9c,0x4e,0x00,0x66,0x08,0x06,0x06,0xf0,0x9c,0x4e,0x00\n"
    ".byte 0x66,0x08,0x06,0x06,0xf8,0x9c,0x4e,0x00,0x66,0x08,0x06,0x06,0x10,0x9d,0x4e,0x00\n"
    ".byte 0xe6,0x08,0x07,0x04,0x60,0x9b,0x4e,0x00,0xe6,0x09,0x00,0x00,0x60,0x9b,0x4e,0x00\n"
    ".byte 0x77,0x09,0x01,0x06,0x20,0x9d,0x4e,0x00,0x77,0x09,0x02,0x02,0x28,0x9d,0x4e,0x00\n"
    ".byte 0x77,0x09,0x03,0x06,0x30,0x9d,0x4e,0x00,0x77,0x09,0x04,0x02,0x40,0x9d,0x4e,0x00\n"
    ".byte 0x77,0x09,0x05,0x02,0x50,0x9d,0x4e,0x00,0x77,0x09,0x06,0x02,0x60,0x9d,0x4e,0x00\n"
    ".byte 0x77,0x09,0x07,0x02,0x70,0x9d,0x4e,0x00,0x77,0x0a,0x00,0x06,0x80,0x9d,0x4e,0x00\n"
    ".byte 0x77,0x0a,0x01,0x02,0x90,0x9d,0x4e,0x00,0x77,0x0a,0x02,0x02,0xa0,0x9d,0x4e,0x00\n"
    ".byte 0x77,0x0a,0x03,0x02,0xb0,0x9d,0x4e,0x00,0x77,0x0a,0x04,0x06,0xb8,0x9d,0x4e,0x00\n"
    ".byte 0x77,0x0a,0x05,0x02,0xc8,0x9d,0x4e,0x00,0x77,0x0a,0x06,0x06,0xd8,0x9d,0x4e,0x00\n"
    ".byte 0x77,0x0a,0x07,0x06,0xe8,0x9d,0x4e,0x00,0x77,0x0b,0x00,0x02,0xf8,0x9d,0x4e,0x00\n"
    ".byte 0x77,0x0b,0x01,0x02,0x08,0x9e,0x4e,0x00,0x77,0x0b,0x02,0x06,0x18,0x9e,0x4e,0x00\n"
    ".byte 0x77,0x0b,0x03,0x02,0x28,0x9e,0x4e,0x00,0x77,0x0b,0x04,0x06,0x38,0x9e,0x4e,0x00\n"
    ".byte 0x77,0x0b,0x05,0x02,0x40,0x9e,0x4e,0x00,0x77,0x0b,0x06,0x04,0x50,0x9e,0x4e,0x00\n"
    ".byte 0x77,0x0b,0x07,0x06,0x60,0x9e,0x4e,0x00,0x77,0x0c,0x00,0x02,0x70,0x9e,0x4e,0x00\n"
    ".byte 0x77,0x0c,0x01,0x02,0x78,0x9e,0x4e,0x00,0x77,0x0c,0x02,0x02,0x80,0x9e,0x4e,0x00\n"
    ".byte 0x77,0x0c,0x03,0x02,0x90,0x9e,0x4e,0x00,0x77,0x0c,0x04,0x02,0xa0,0x9e,0x4e,0x00\n"
    ".byte 0x77,0x0c,0x05,0x02,0xb0,0x9e,0x4e,0x00,0x77,0x0c,0x06,0x02,0xc0,0x9e,0x4e,0x00\n"
    ".byte 0x77,0x0c,0x07,0x02,0xd0,0x9e,0x4e,0x00,0x77,0x0d,0x00,0x06,0xa8,0x9c,0x4e,0x00\n"
    ".byte 0x77,0x0d,0x01,0x06,0xe0,0x9c,0x4e,0x00,0x77,0x0d,0x02,0x06,0xb8,0x9c,0x4e,0x00\n"
    ".byte 0x77,0x0d,0x03,0x06,0x10,0x9d,0x4e,0x00,0x77,0x0d,0x04,0x02,0xe0,0x9e,0x4e,0x00\n"
    ".byte 0x77,0x0d,0x05,0x06,0xf0,0x9e,0x4e,0x00,0x77,0x0d,0x06,0x06,0xc8,0x9c,0x4e,0x00\n"
    ".byte 0x77,0x0d,0x07,0x06,0xd0,0x9c,0x4e,0x00,0x77,0x0e,0x00,0x06,0xf0,0x9c,0x4e,0x00\n"
    ".byte 0x77,0x0e,0x01,0x06,0xf8,0x9e,0x4e,0x00,0x77,0x0e,0x02,0x06,0xf8,0x9c,0x4e,0x00\n"
    ".byte 0x77,0x0e,0x03,0x02,0x00,0x9f,0x4e,0x00,0x77,0x0e,0x04,0x02,0x08,0x9f,0x4e,0x00\n"
    ".byte 0x77,0x0e,0x05,0x02,0x10,0x9f,0x4e,0x00,0x77,0x0e,0x06,0x02,0x18,0x9f,0x4e,0x00\n"
    ".byte 0x77,0x0e,0x07,0x02,0x20,0x9f,0x4e,0x00,0x77,0x0f,0x00,0x06,0x30,0x9f,0x4e,0x00\n"
    ".byte 0x77,0x0f,0x01,0x02,0x38,0x9f,0x4e,0x00,0x77,0x0f,0x03,0x06,0x40,0x9f,0x4e,0x00\n"
    ".byte 0x77,0x0f,0x04,0x02,0x48,0x9f,0x4e,0x00,0x77,0x0f,0x05,0x02,0x50,0x9f,0x4e,0x00\n"
    ".byte 0x77,0x0f,0x06,0x02,0x58,0x9f,0x4e,0x00,0x77,0x0f,0x07,0x02,0x60,0x9f,0x4e,0x00\n"
    ".byte 0x77,0x10,0x00,0x02,0x68,0x9f,0x4e,0x00,0x77,0x10,0x01,0x02,0x70,0x9f,0x4e,0x00\n"
    ".byte 0x77,0x10,0x02,0x02,0x78,0x9f,0x4e,0x00,0x77,0x15,0x06,0x02,0x80,0x9f,0x4e,0x00\n"
    ".byte 0x77,0x15,0x07,0x02,0x90,0x9f,0x4e,0x00,0x77,0x0f,0x02,0x00,0xa0,0x9f,0x4e,0x00\n"
    ".byte 0x77,0x0d,0x04,0x00,0xb8,0x9f,0x4e,0x00,0x17,0x16,0x00,0x00,0xd0,0x9f,0x4e,0x00\n"
    ".byte 0x17,0x16,0x01,0x00,0xe0,0x9f,0x4e,0x00,0x17,0x16,0x02,0x00,0xf0,0x9f,0x4e,0x00\n"
    ".byte 0x27,0x16,0x03,0x00,0xf8,0x9f,0x4e,0x00,0x17,0x16,0x04,0x00,0x00,0xa0,0x4e,0x00\n"
    ".byte 0x17,0x16,0x05,0x00,0x08,0xa0,0x4e,0x00,0x17,0x16,0x06,0x00,0x10,0xa0,0x4e,0x00\n"
    ".byte 0x27,0x16,0x07,0x00,0x18,0xa0,0x4e,0x00,0x17,0x17,0x00,0x00,0x20,0xa0,0x4e,0x00\n"
    ".byte 0x17,0x17,0x01,0x00,0x30,0xa0,0x4e,0x00,0x17,0x17,0x02,0x00,0x40,0xa0,0x4e,0x00\n"
    ".byte 0x17,0x17,0x03,0x00,0x50,0xa0,0x4e,0x00,0x27,0x17,0x04,0x00,0x60,0xa0,0x4e,0x00\n"
    ".byte 0x17,0x17,0x05,0x00,0x70,0xa0,0x4e,0x00,0x17,0x17,0x06,0x00,0x78,0xa0,0x4e,0x00\n"
    ".byte 0x17,0x17,0x07,0x00,0x80,0xa0,0x4e,0x00,0x17,0x18,0x00,0x00,0x88,0xa0,0x4e,0x00\n"
    ".byte 0x17,0x18,0x01,0x00,0x90,0xa0,0x4e,0x00,0x27,0x18,0x02,0x00,0xa0,0xa0,0x4e,0x00\n"
    ".byte 0x17,0x18,0x03,0x00,0xb0,0xa0,0x4e,0x00,0x17,0x18,0x04,0x00,0xc0,0xa0,0x4e,0x00\n"
    ".byte 0x27,0x18,0x05,0x00,0xd0,0xa0,0x4e,0x00,0x77,0x18,0x06,0x00,0xe0,0xa0,0x4e,0x00\n"
    ".byte 0x77,0x18,0x07,0x00,0xf0,0xa0,0x4e,0x00,0x17,0x19,0x00,0x00,0x00,0xa1,0x4e,0x00\n"
    ".byte 0x17,0x19,0x01,0x00,0x10,0xa1,0x4e,0x00,0x27,0x19,0x02,0x00,0x20,0xa1,0x4e,0x00\n"
    ".byte 0x17,0x19,0x03,0x00,0x30,0xa1,0x4e,0x00,0x27,0x19,0x04,0x00,0x40,0xa1,0x4e,0x00\n"
    ".byte 0x27,0x19,0x05,0x00,0x50,0xa1,0x4e,0x00,0x77,0x1a,0x03,0x00,0x60,0x9b,0x4e,0x00\n"
    ".byte 0x88,0x00,0x00,0x06,0x60,0xa1,0x4e,0x00,0x99,0x10,0x03,0x06,0x70,0xa1,0x4e,0x00\n"
    ".byte 0x99,0x10,0x04,0x06,0x88,0xa1,0x4e,0x00,0x99,0x10,0x05,0x06,0xa0,0xa1,0x4e,0x00\n"
    ".byte 0x99,0x10,0x06,0x06,0xb0,0xa1,0x4e,0x00,0x99,0x10,0x07,0x06,0xc8,0xa1,0x4e,0x00\n"
    ".byte 0x99,0x11,0x00,0x06,0xe0,0xa1,0x4e,0x00,0x99,0x11,0x01,0x02,0xf8,0xa1,0x4e,0x00\n"
    ".byte 0x99,0x11,0x02,0x02,0x08,0xa2,0x4e,0x00,0x99,0x11,0x03,0x06,0x18,0xa2,0x4e,0x00\n"
    ".byte 0x99,0x11,0x04,0x06,0x28,0xa2,0x4e,0x00,0x99,0x11,0x05,0x06,0x40,0xa2,0x4e,0x00\n"
    ".byte 0xa9,0x11,0x06,0x06,0x50,0xa2,0x4e,0x00,0xa9,0x11,0x07,0x02,0x60,0xa2,0x4e,0x00\n"
    ".byte 0xa9,0x12,0x00,0x02,0x78,0xa2,0x4e,0x00,0xa9,0x12,0x01,0x06,0x88,0xa2,0x4e,0x00\n"
    ".byte 0xa9,0x12,0x02,0x06,0x98,0xa2,0x4e,0x00,0xa9,0x12,0x03,0x06,0xa8,0xa2,0x4e,0x00\n"
    ".byte 0xa9,0x12,0x04,0x06,0xc0,0xa2,0x4e,0x00,0xb9,0x12,0x05,0x02,0xd0,0xa2,0x4e,0x00\n"
    ".byte 0xb9,0x12,0x06,0x02,0xe0,0xa2,0x4e,0x00,0xb9,0x12,0x07,0x02,0xf0,0xa2,0x4e,0x00\n"
    ".byte 0xb9,0x13,0x00,0x06,0x00,0xa3,0x4e,0x00,0xb9,0x13,0x01,0x06,0x18,0xa3,0x4e,0x00\n"
    ".byte 0xb9,0x13,0x02,0x02,0x28,0xa3,0x4e,0x00,0xb9,0x13,0x03,0x06,0x38,0xa3,0x4e,0x00\n"
    ".byte 0xca,0x13,0x04,0x06,0x48,0xa3,0x4e,0x00,0xca,0x13,0x05,0x06,0x60,0xa3,0x4e,0x00\n"
    ".byte 0xca,0x13,0x06,0x06,0x70,0xa3,0x4e,0x00,0xca,0x13,0x07,0x02,0x80,0xa3,0x4e,0x00\n"
    ".byte 0xdb,0x14,0x00,0x06,0x90,0xa3,0x4e,0x00,0xdb,0x14,0x03,0x06,0xa0,0xa3,0x4e,0x00\n"
    ".byte 0xdb,0x14,0x02,0x06,0xa8,0xa3,0x4e,0x00,0xdb,0x14,0x01,0x06,0xc0,0xa3,0x4e,0x00\n"
    ".byte 0xdb,0x14,0x05,0x06,0xd0,0xa3,0x4e,0x00,0xdb,0x14,0x04,0x06,0xd8,0xa3,0x4e,0x00\n"
    ".byte 0xdb,0x14,0x06,0x06,0xe8,0xa3,0x4e,0x00,0xdb,0x14,0x07,0x06,0xf8,0xa3,0x4e,0x00\n"
    ".byte 0xdb,0x15,0x00,0x06,0x08,0xa4,0x4e,0x00,0xdb,0x15,0x01,0x02,0x18,0xa4,0x4e,0x00\n"
    ".byte 0xdb,0x15,0x02,0x06,0x28,0xa4,0x4e,0x00,0xdb,0x15,0x03,0x06,0x38,0xa4,0x4e,0x00\n"
    ".byte 0xdb,0x15,0x04,0x04,0x48,0xa4,0x4e,0x00,0x0d,0x00,0x00,0x00,0x58,0xa4,0x4e,0x00\n"
    ".byte 0x0d,0x00,0x01,0x00,0x60,0xa4,0x4e,0x00,0x0d,0x00,0x02,0x00,0x68,0xa4,0x4e,0x00\n"
    ".byte 0x0d,0x00,0x03,0x00,0x70,0xa4,0x4e,0x00,0x0d,0x00,0x04,0x00,0x78,0xa4,0x4e,0x00\n"
    ".byte 0x0d,0x00,0x05,0x00,0x80,0xa4,0x4e,0x00,0x0d,0x00,0x06,0x00,0x88,0xa4,0x4e,0x00\n"
    ".byte 0x0d,0x01,0x00,0x00,0x90,0xa4,0x4e,0x00,0x0d,0x01,0x01,0x00,0x98,0xa4,0x4e,0x00\n"
    ".byte 0x0d,0x02,0x00,0x00,0xa0,0xa4,0x4e,0x00,0x0d,0x02,0x01,0x00,0xa8,0xa4,0x4e,0x00\n"
    ".byte 0x0d,0x03,0x00,0x00,0xb0,0xa4,0x4e,0x00,0x0d,0x04,0x00,0x00,0xb8,0xa4,0x4e,0x00\n"
    ".byte 0x0c,0x00,0x00,0x00,0xc0,0xa4,0x4e,0x00,0x0c,0x00,0x00,0x00,0xd0,0xa4,0x4e,0x00\n"
    ".byte 0x0c,0x00,0x00,0x00,0xe8,0xa4,0x4e,0x00,0x0c,0x00,0x00,0x00,0x30,0x78,0x4e,0x00\n"
    ".byte 0x0c,0x00,0x00,0x00,0x30,0x78,0x4e,0x00,0x00,0x00,0x00,0x00,0x30,0x78,0x4e,0x00\n"
    ".byte 0x00,0x00,0x00,0x00,0x30,0x78,0x4e,0x00,0x00,0x00,0x00,0x00,0x30,0x78,0x4e,0x00\n"
    ".byte 0x30,0x31,0x63,0x61,0x00,0x00,0x00,0x00\n"
    ".byte 0x30,0x35,0x74,0x72,0x00,0x00,0x00,0x00\n"
    ".byte 0x30,0x36,0x77,0x6f,0x00,0x00,0x00,0x00\n"
    ".byte 0x30,0x37,0x6f,0x6c,0x00,0x00,0x00,0x00\n"
    ".byte 0x30,0x38,0x61,0x67,0x00,0x00,0x00,0x00\n"
    ".byte 0x30,0x39,0x68,0x61,0x00,0x00,0x00,0x00\n"
    ".byte 0x31,0x30,0x6d,0x6f,0x00,0x00,0x00,0x00\n"
    ".byte 0x31,0x31,0x61,0x74,0x00,0x00,0x00,0x00\n"
    ".byte 0x31,0x32,0x6e,0x65,0x00,0x00,0x00,0x00\n"
    ".byte 0x31,0x33,0x68,0x6f,0x00,0x00,0x00,0x00\n"
    ".byte 0x31,0x34,0x70,0x6f,0x00,0x00,0x00,0x00\n"
    ".byte 0x31,0x35,0x74,0x77,0x00,0x00,0x00,0x00\n"
    ".byte 0x31,0x36,0x64,0x65,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$StoryLadName\n"
    "_10JiminyMemo$StoryLadName:\n"
    ".word D_004EABF8\n"
    ".word D_004EAC00\n"
    ".word D_004EAC08\n"
    ".word D_004EAC10\n"
    ".word D_004EAC18\n"
    ".word D_004EAC20\n"
    ".word D_004EAC28\n"
    ".word D_004EAC30\n"
    ".word D_004EAC38\n"
    ".word D_004EAC40\n"
    ".word D_004EAC48\n"
    ".word D_004EAC50\n"
    ".word D_004EAC58\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$RsrcId\n"
    "_10JiminyMemo$RsrcId:\n"
    ".byte 0x6e,0x00,0x00,0x00,0x70,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$NoteScrollBarTbl\n"
    "_10JiminyMemo$NoteScrollBarTbl:\n"
    ".byte 0xe9,0x00,0xea,0x00,0xeb,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CursorSeqTbl\n"
    "_10JiminyMemo$CursorSeqTbl:\n"
    ".byte 0x24,0x00,0x00,0x00,0x25,0x00,0x00,0x00,0x49,0x00,0x00,0x00,0x80,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CardAttackTbl\n"
    "_10JiminyMemo$CardAttackTbl:\n"
    ".byte 0x01,0x00,0x02,0x00\n"
    ".word D_40000 + 0x3\n"
    ".byte 0x05,0x00,0x06,0x00,0x07,0x00,0x08,0x00,0x09,0x00,0x0a,0x00,0x0b,0x00,0x0c,0x00\n"
    ".byte 0x0d,0x00,0x0e,0x00,0x0f,0x00,0x10,0x00,0x11,0x00,0x13,0x00,0x14,0x00,0x15,0x00\n"
    ".byte 0x16,0x00,0x17,0x00,0x18,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CardMagicTbl\n"
    "_10JiminyMemo$CardMagicTbl:\n"
    ".byte 0x1a,0x00,0x1b,0x00,0x1c,0x00,0x1d,0x00,0x1e,0x00,0x1f,0x00,0x20,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CardSummonsTbl\n"
    "_10JiminyMemo$CardSummonsTbl:\n"
    ".byte 0x23,0x00,0x24,0x00,0x25,0x00,0x26,0x00,0x27,0x00,0x28,0x00,0x29,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CardItemTbl\n"
    "_10JiminyMemo$CardItemTbl:\n"
    ".byte 0x2a,0x00,0x2b,0x00,0x2c,0x00,0x2d,0x00,0x2e,0x00,0x2f,0x00,0x30,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CardFriendTbl\n"
    "_10JiminyMemo$CardFriendTbl:\n"
    ".byte 0x31,0x00,0x32,0x00,0x33,0x00,0x34,0x00,0x35,0x00,0x36,0x00,0x37,0x00,0x39,0x00\n"
    ".globl _10JiminyMemo$CardEnemyTbl\n"
    "_10JiminyMemo$CardEnemyTbl:\n"
    ".word D_00450044\n"
    ".byte 0x4b,0x00,0x4c,0x00,0x46,0x00,0x4f,0x00,0x52,0x00,0x4e,0x00,0x57,0x00,0x47,0x00\n"
    ".word D_00490048\n"
    ".word _60_GLOBAL_$N$c__KHCOM_MainApp_System_Graphic_PackMan.cpp33aaaa$s_pPktBufVif1 + 0xBAB0A\n"
    ".word _58_GLOBAL_$N$c__KHCOM_MainApp_System_DebugProcInfo.cpptWaaaa$_szBuf + 0xA5\n"
    ".word _60_GLOBAL_$N$c__KHCOM_MainApp_System_Graphic_PackMan.cpp33aaaa$s_pPktBufVif1 + 0x7AB15\n"
    ".word _60_GLOBAL_$N$c__KHCOM_MainApp_System_Graphic_PackMan.cpp33aaaa$s_pPktBufVif1 + 0xAB16\n"
    ".word _60_GLOBAL_$N$c__KHCOM_MainApp_System_Graphic_PackMan.cpp33aaaa$s_pPktBufVif1 + 0x2AB14\n"
    ".word _60_GLOBAL_$N$c__KHCOM_MainApp_System_Graphic_PackMan.cpp33aaaa$s_pPktBufVif1 + 0x9AB1B\n"
    ".word _60_GLOBAL_$N$c__KHCOM_MainApp_System_Graphic_PackMan.cpp33aaaa$s_pPktBufVif1 + 0xEAB1E\n"
    ".word _60_GLOBAL_$N$c__KHCOM_MainApp_System_Graphic_PackMan.cpp33aaaa$s_pPktBufVif1 + 0x10AB20\n"
    ".word _60_GLOBAL_$N$c__KHCOM_MainApp_System_Graphic_PackMan.cpp33aaaa$s_pPktBufVif1 + 0xCAB22\n"
    ".word _50_GLOBAL_$N$c__KHCOM_MainApp_System_RcfCD.cppvSaaaa$s_pCdBuff + 0x5B63\n"
    ".word D_00690065\n"
    ".word nowait_trans_buff + 0xC6B\n"
    ".byte 0x64,0x00,0x6c,0x00\n"
    ".word _59_GLOBAL_$N$c__KHCOM_MainApp_System_Sound_SoundMan.cppG7aaaa$s_pCrossBuffer + 0x18D6D\n"
    ".word _59_GLOBAL_$N$c__KHCOM_MainApp_System_Sound_SoundMan.cppG7aaaa$s_pCrossBuffer + 0x28D6E\n"
    ".byte 0x74,0x00,0x76,0x00,0x77,0x00,0x78,0x00,0x71,0x00,0x73,0x00,0x75,0x00,0x79,0x00\n"
    ".byte 0x6f,0x00,0x7a,0x00,0x7b,0x00,0x72,0x00,0x70,0x00,0x7c,0x00\n"
    ".globl _10JiminyMemo$CardMapTbl\n"
    "_10JiminyMemo$CardMapTbl:\n"
    ".byte 0xa1,0x00,0xa2,0x00,0xa3,0x00,0xa4,0x00,0xa5,0x00,0xa6,0x00,0xa7,0x00,0xa8,0x00\n"
    ".byte 0xa9,0x00,0xaa,0x00,0xab,0x00,0xac,0x00,0xad,0x00,0xae,0x00,0xaf,0x00,0xb0,0x00\n"
    ".byte 0xb1,0x00,0xb2,0x00,0xb3,0x00,0xb4,0x00,0xb5,0x00,0xb6,0x00,0xb7,0x00,0xb8,0x00\n"
    ".byte 0xbd,0x00,0xba,0x00,0xbb,0x00,0xbc,0x00,0xb9,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CardWorldTbl\n"
    "_10JiminyMemo$CardWorldTbl:\n"
    ".byte 0xbe,0x00,0xbf,0x00,0xc0,0x00,0xc1,0x00,0xc2,0x00,0xc3,0x00,0xc4,0x00,0xc5,0x00\n"
    ".byte 0xc6,0x00,0xc7,0x00,0xc8,0x00,0xc9,0x00\n"
    ".globl _10JiminyMemo$CardGimmickTbl\n"
    "_10JiminyMemo$CardGimmickTbl:\n"
    ".byte 0x3a,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CardSpecialTbl\n"
    "_10JiminyMemo$CardSpecialTbl:\n"
    ".byte 0x7d,0x00,0x7e,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$RikuCardBattleTbl\n"
    "_10JiminyMemo$RikuCardBattleTbl:\n"
    ".byte 0x12,0x00,0x2a,0x00,0x2b,0x00,0x38,0x00,0x3a,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$RikuCardEnemyTbl\n"
    "_10JiminyMemo$RikuCardEnemyTbl:\n"
    ".word _vt$6CGlare + 0x14\n"
    ".byte 0x4b,0x00,0x4f,0x00\n"
    ".word _60_GLOBAL_$N$c__KHCOM_MainApp_System_Graphic_PackMan.cpp33aaaa$s_pPktBufVif1 + 0x1AB11\n"
    ".word _60_GLOBAL_$N$c__KHCOM_MainApp_System_Graphic_PackMan.cpp33aaaa$s_pPktBufVif1 + 0x6AB15\n"
    ".word _60_GLOBAL_$N$c__KHCOM_MainApp_System_Graphic_PackMan.cpp33aaaa$s_pPktBufVif1 + 0x8AB1A\n"
    ".word D_00640063\n"
    ".word _59_GLOBAL_$N$c__KHCOM_MainApp_System_Sound_SoundMan.cppG7aaaa$s_pCrossBuffer + 0x18D65\n"
    ".word D_00690068\n"
    ".byte 0x6a,0x00,0x6b,0x00,0x6c,0x00,0x6d,0x00,0x73,0x00,0x75,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$RikuCardMapTbl\n"
    "_10JiminyMemo$RikuCardMapTbl:\n"
    ".byte 0xa1,0x00,0xa2,0x00,0xa3,0x00,0xa4,0x00,0xa5,0x00,0xa6,0x00,0xa9,0x00,0xaa,0x00\n"
    ".byte 0xab,0x00,0xac,0x00,0xaf,0x00,0xb0,0x00,0xb1,0x00,0xb2,0x00,0xb6,0x00,0xb7,0x00\n"
    ".byte 0xba,0x00,0xbb,0x00,0xbc,0x00,0xb9,0x00\n"
    ".globl _10JiminyMemo$RikuCardWorldTbl\n"
    "_10JiminyMemo$RikuCardWorldTbl:\n"
    ".byte 0xbe,0x00,0xbf,0x00,0xc0,0x00,0xc1,0x00,0xc2,0x00,0xc3,0x00,0xc4,0x00,0xc5,0x00\n"
    ".byte 0xc6,0x00,0xc8,0x00,0xc9,0x00,0xca,0x00\n"
    ".globl _10JiminyMemo$CardNameAttackTbl\n"
    "_10JiminyMemo$CardNameAttackTbl:\n"
    ".byte 0x01,0x08,0x02,0x08,0x03,0x08,0x04,0x08,0x05,0x08,0x06,0x08,0x07,0x08,0x08,0x08\n"
    ".byte 0x09,0x08,0x0a,0x08,0x0b,0x08,0x0c,0x08,0x0d,0x08,0x0e,0x08,0x0f,0x08,0x10,0x08\n"
    ".byte 0x11,0x08,0x12,0x08,0x13,0x08,0x14,0x08,0x15,0x08,0x16,0x08,0x17,0x08,0x00,0x00\n"
    ".globl _10JiminyMemo$CardNameMagicTbl\n"
    "_10JiminyMemo$CardNameMagicTbl:\n"
    ".byte 0x01,0x09,0x02,0x09,0x03,0x09,0x04,0x09,0x05,0x09,0x06,0x09,0x07,0x09,0x00,0x00\n"
    ".globl _10JiminyMemo$CardNameSummonsTbl\n"
    "_10JiminyMemo$CardNameSummonsTbl:\n"
    ".byte 0x08,0x09,0x09,0x09,0x0a,0x09,0x0b,0x09,0x0c,0x09,0x0d,0x09,0x0e,0x09,0x00,0x00\n"
    ".globl _10JiminyMemo$CardNameItemTbl\n"
    "_10JiminyMemo$CardNameItemTbl:\n"
    ".byte 0x01,0x0a,0x02,0x0a,0x03,0x0a,0x04,0x0a,0x05,0x0a,0x06,0x0a,0x07,0x0a,0x00,0x00\n"
    ".globl _10JiminyMemo$CardNameFriendTbl\n"
    "_10JiminyMemo$CardNameFriendTbl:\n"
    ".byte 0x01,0x0b,0x02,0x0b,0x03,0x0b,0x04,0x0b,0x05,0x0b,0x06,0x0b,0x07,0x0b,0x08,0x0b\n"
    ".globl _10JiminyMemo$CardNameEnemyTbl\n"
    "_10JiminyMemo$CardNameEnemyTbl:\n"
    ".byte 0x01,0x0c,0x02,0x0c,0x08,0x0c,0x09,0x0c,0x03,0x0c,0x0c,0x0c,0x0f,0x0c,0x0b,0x0c\n"
    ".byte 0x14,0x0c,0x04,0x0c,0x05,0x0c,0x06,0x0c,0x07,0x0c,0x18,0x0c,0x0a,0x0c,0x0d,0x0c\n"
    ".byte 0x12,0x0c,0x15,0x0c,0x13,0x0c,0x0e,0x0c,0x11,0x0c,0x10,0x0c,0x17,0x0c,0x16,0x0c\n"
    ".byte 0x1a,0x0c,0x1b,0x0c,0x1c,0x0c,0x1d,0x0c,0x1e,0x0c,0x19,0x0c,0x1f,0x0c,0x24,0x0c\n"
    ".byte 0x21,0x0c,0x25,0x0c,0x27,0x0c,0x26,0x0c,0x20,0x0c,0x28,0x0c,0x29,0x0c,0x22,0x0c\n"
    ".byte 0x2a,0x0c,0x23,0x0c,0x30,0x0c,0x32,0x0c,0x33,0x0c,0x34,0x0c,0x2d,0x0c,0x2f,0x0c\n"
    ".byte 0x31,0x0c,0x35,0x0c,0x2b,0x0c,0x36,0x0c,0x37,0x0c,0x2e,0x0c,0x2c,0x0c,0x38,0x0c\n"
    ".globl _10JiminyMemo$CardNameMapTbl\n"
    "_10JiminyMemo$CardNameMapTbl:\n"
    ".byte 0x01,0x0d,0x02,0x0d,0x03,0x0d,0x04,0x0d,0x05,0x0d,0x06,0x0d,0x07,0x0d,0x08,0x0d\n"
    ".byte 0x09,0x0d,0x0a,0x0d,0x0b,0x0d,0x0c,0x0d,0x0d,0x0d,0x0e,0x0d,0x0f,0x0d,0x10,0x0d\n"
    ".byte 0x11,0x0d,0x12,0x0d,0x13,0x0d,0x14,0x0d,0x15,0x0d,0x16,0x0d,0x17,0x0d,0x18,0x0d\n"
    ".byte 0x1d,0x0d,0x1a,0x0d,0x1b,0x0d,0x1c,0x0d,0x19,0x0d,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CardNameWorldTbl\n"
    "_10JiminyMemo$CardNameWorldTbl:\n"
    ".byte 0x01,0x0e,0x02,0x0e,0x03,0x0e,0x04,0x0e,0x05,0x0e,0x06,0x0e,0x07,0x0e,0x08,0x0e\n"
    ".byte 0x09,0x0e,0x0a,0x0e,0x0b,0x0e,0x0c,0x0e\n"
    ".globl _10JiminyMemo$CardNameGimmickTbl\n"
    "_10JiminyMemo$CardNameGimmickTbl:\n"
    ".byte 0x01,0x0f,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CardNameSpecialTbl\n"
    "_10JiminyMemo$CardNameSpecialTbl:\n"
    ".byte 0x01,0x10,0x02,0x10,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$RikuCardNameBattleTbl\n"
    "_10JiminyMemo$RikuCardNameBattleTbl:\n"
    ".byte 0x01,0x11,0x01,0x0a,0x02,0x0a,0x04,0x11,0x01,0x0f,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$RikuCardNameEnemyTbl\n"
    "_10JiminyMemo$RikuCardNameEnemyTbl:\n"
    ".byte 0x01,0x0c,0x03,0x0c,0x08,0x0c,0x0c,0x0c,0x0e,0x0c,0x0f,0x0c,0x12,0x0c,0x14,0x0c\n"
    ".byte 0x16,0x0c,0x3a,0x0c,0x1f,0x0c,0x20,0x0c,0x21,0x0c,0x22,0x0c,0x24,0x0c,0x25,0x0c\n"
    ".byte 0x26,0x0c,0x27,0x0c,0x28,0x0c,0x29,0x0c,0x2f,0x0c,0x31,0x0c,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$RikuCardNameMapTbl\n"
    "_10JiminyMemo$RikuCardNameMapTbl:\n"
    ".byte 0x01,0x0d,0x02,0x0d,0x03,0x0d,0x04,0x0d,0x05,0x0d,0x06,0x0d,0x09,0x0d,0x0a,0x0d\n"
    ".byte 0x0b,0x0d,0x0c,0x0d,0x0f,0x0d,0x10,0x0d,0x11,0x0d,0x12,0x0d,0x16,0x0d,0x17,0x0d\n"
    ".byte 0x1a,0x0d,0x1b,0x0d,0x1c,0x0d,0x19,0x0d\n"
    ".globl _10JiminyMemo$RikuCardNameWorldTbl\n"
    "_10JiminyMemo$RikuCardNameWorldTbl:\n"
    ".byte 0x01,0x0e,0x02,0x0e,0x03,0x0e,0x04,0x0e,0x05,0x0e,0x06,0x0e,0x07,0x0e,0x08,0x0e\n"
    ".byte 0x09,0x0e,0x0b,0x0e,0x0c,0x0e,0x0e,0x0e\n"
    ".globl _10JiminyMemo$CardManualAttackTbl\n"
    "_10JiminyMemo$CardManualAttackTbl:\n"
    ".byte 0x01,0x12,0x02,0x12,0x03,0x12,0x04,0x12,0x05,0x12,0x06,0x12,0x07,0x12,0x08,0x12\n"
    ".byte 0x09,0x12,0x0a,0x12,0x0b,0x12,0x0c,0x12,0x0d,0x12,0x0e,0x12,0x0f,0x12,0x10,0x12\n"
    ".byte 0x11,0x12,0x12,0x12,0x13,0x12,0x14,0x12,0x15,0x12,0x16,0x12,0x17,0x12,0x00,0x00\n"
    ".globl _10JiminyMemo$CardManualMagicTbl\n"
    "_10JiminyMemo$CardManualMagicTbl:\n"
    ".byte 0x01,0x13,0x02,0x13,0x03,0x13,0x04,0x13,0x05,0x13,0x06,0x13,0x07,0x13,0x00,0x00\n"
    ".globl _10JiminyMemo$CardManualSummonsTbl\n"
    "_10JiminyMemo$CardManualSummonsTbl:\n"
    ".byte 0x08,0x13,0x09,0x13,0x0a,0x13,0x0b,0x13,0x0c,0x13,0x0d,0x13,0x0e,0x13,0x00,0x00\n"
    ".globl _10JiminyMemo$CardManualItemTbl\n"
    "_10JiminyMemo$CardManualItemTbl:\n"
    ".byte 0x01,0x14,0x02,0x14,0x03,0x14,0x04,0x14,0x05,0x14,0x06,0x14,0x07,0x14,0x00,0x00\n"
    ".globl _10JiminyMemo$CardManualFriendTbl\n"
    "_10JiminyMemo$CardManualFriendTbl:\n"
    ".byte 0x01,0x15,0x02,0x15,0x03,0x15,0x04,0x15,0x05,0x15,0x06,0x15,0x07,0x15,0x08,0x15\n"
    ".globl _10JiminyMemo$CardManualEnemyTbl\n"
    "_10JiminyMemo$CardManualEnemyTbl:\n"
    ".byte 0x01,0x16,0x02,0x16,0x08,0x16,0x09,0x16,0x03,0x16,0x0c,0x16,0x0f,0x16,0x0b,0x16\n"
    ".byte 0x14,0x16,0x04,0x16,0x05,0x16,0x06,0x16,0x07,0x16,0x18,0x16,0x0a,0x16,0x0d,0x16\n"
    ".byte 0x12,0x16,0x15,0x16,0x13,0x16,0x0e,0x16,0x11,0x16,0x10,0x16,0x17,0x16,0x16,0x16\n"
    ".byte 0x1a,0x16,0x1b,0x16,0x1c,0x16,0x1d,0x16,0x1e,0x16,0x19,0x16,0x1f,0x16,0x24,0x16\n"
    ".byte 0x21,0x16,0x25,0x16,0x27,0x16,0x26,0x16,0x20,0x16,0x28,0x16,0x29,0x16,0x22,0x16\n"
    ".byte 0x2a,0x16,0x23,0x16,0x30,0x16,0x32,0x16,0x33,0x16,0x34,0x16,0x2d,0x16,0x2f,0x16\n"
    ".byte 0x31,0x16,0x35,0x16,0x2b,0x16,0x36,0x16,0x37,0x16,0x2e,0x16,0x2c,0x16,0x38,0x16\n"
    ".globl _10JiminyMemo$CardManualMapTbl\n"
    "_10JiminyMemo$CardManualMapTbl:\n"
    ".byte 0x01,0x17,0x02,0x17,0x03,0x17,0x04,0x17,0x05,0x17,0x06,0x17,0x07,0x17,0x08,0x17\n"
    ".byte 0x09,0x17,0x0a,0x17,0x0b,0x17,0x0c,0x17,0x0d,0x17,0x0e,0x17,0x0f,0x17,0x10,0x17\n"
    ".byte 0x11,0x17,0x12,0x17,0x13,0x17,0x14,0x17,0x15,0x17,0x16,0x17,0x17,0x17,0x18,0x17\n"
    ".byte 0x1d,0x17,0x1a,0x17,0x1b,0x17,0x1c,0x17,0x19,0x17,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CardManualWorldTbl\n"
    "_10JiminyMemo$CardManualWorldTbl:\n"
    ".byte 0x01,0x18,0x02,0x18,0x03,0x18,0x04,0x18,0x05,0x18,0x06,0x18,0x07,0x18,0x08,0x18\n"
    ".byte 0x09,0x18,0x0a,0x18,0x0b,0x18,0x0c,0x18\n"
    ".globl _10JiminyMemo$CardManualGimmickTbl\n"
    "_10JiminyMemo$CardManualGimmickTbl:\n"
    ".byte 0x01,0x19,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CardManualSpecialTbl\n"
    "_10JiminyMemo$CardManualSpecialTbl:\n"
    ".byte 0x01,0x1a,0x02,0x1a,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$RikuCardManualBattleTbl\n"
    "_10JiminyMemo$RikuCardManualBattleTbl:\n"
    ".byte 0x01,0x1b,0x01,0x14,0x02,0x14,0x02,0x1b,0x01,0x19,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$RikuCardManualEnemyTbl\n"
    "_10JiminyMemo$RikuCardManualEnemyTbl:\n"
    ".byte 0x01,0x16,0x03,0x16,0x08,0x16,0x0c,0x16,0x0e,0x16,0x0f,0x16,0x12,0x16,0x14,0x16\n"
    ".byte 0x16,0x16,0x39,0x16,0x1f,0x16,0x20,0x16,0x21,0x16,0x22,0x16,0x24,0x16,0x25,0x16\n"
    ".byte 0x26,0x16,0x27,0x16,0x28,0x16,0x29,0x16,0x2f,0x16,0x31,0x16,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$RikuCardManualMapTbl\n"
    "_10JiminyMemo$RikuCardManualMapTbl:\n"
    ".byte 0x01,0x17,0x02,0x17,0x03,0x17,0x04,0x17,0x05,0x17,0x06,0x17,0x09,0x17,0x0a,0x17\n"
    ".byte 0x0b,0x17,0x0c,0x17,0x0f,0x17,0x10,0x17,0x11,0x17,0x12,0x17,0x16,0x17,0x17,0x17\n"
    ".byte 0x1a,0x17,0x1b,0x17,0x1c,0x17,0x19,0x17\n"
    ".globl _10JiminyMemo$RikuCardManualWorldTbl\n"
    "_10JiminyMemo$RikuCardManualWorldTbl:\n"
    ".byte 0x03,0x1b,0x04,0x1b,0x05,0x1b,0x06,0x1b,0x07,0x1b,0x08,0x1b,0x09,0x1b,0x0a,0x1b\n"
    ".byte 0x0b,0x1b,0x0c,0x1b,0x0d,0x1b,0x0e,0x1b\n"
    ".globl _10JiminyMemo$SoraCardNumTbl\n"
    "_10JiminyMemo$SoraCardNumTbl:\n"
    ".byte 0x17,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x07,0x00,0x00,0x00\n"
    ".byte 0x08,0x00,0x00,0x00,0x38,0x00,0x00,0x00,0x1d,0x00,0x00,0x00,0x0c,0x00,0x00,0x00\n"
    ".byte 0x01,0x00,0x00,0x00,0x02,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$RikuCardNumTbl\n"
    "_10JiminyMemo$RikuCardNumTbl:\n"
    ".byte 0x05,0x00,0x00,0x00,0x16,0x00,0x00,0x00,0x14,0x00,0x00,0x00,0x0c,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaIllustMain1Tbl\n"
    "_10JiminyMemo$CharaIllustMain1Tbl:\n"
    ".word D_00640264\n"
    ".byte 0x64,0x01,0x1b,0x01,0x08,0x05,0x1b,0x03,0x1b,0x04,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaIllustMain2Tbl\n"
    "_10JiminyMemo$CharaIllustMain2Tbl:\n"
    ".byte 0x1f,0x06,0x0e,0x03,0x0e,0x06,0x0e,0x04,0x0e,0x07,0x0e,0x05,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaIllustEtcTbl\n"
    "_10JiminyMemo$CharaIllustEtcTbl:\n"
    ".byte 0x05,0x07,0x06,0x02,0x06,0x01,0x06,0x04,0x1b,0x02,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaIllustTRTbl\n"
    "_10JiminyMemo$CharaIllustTRTbl:\n"
    ".byte 0x1c,0x00,0x1c,0x01,0x1b,0x05,0x1b,0x06\n"
    ".globl _10JiminyMemo$CharaIllustWOTbl\n"
    "_10JiminyMemo$CharaIllustWOTbl:\n"
    ".byte 0x1c,0x02,0x1c,0x03,0x1c,0x04,0x0d,0x04,0x0f,0x02,0x1c,0x05,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaIllustOLTbl\n"
    "_10JiminyMemo$CharaIllustOLTbl:\n"
    ".byte 0x1e,0x00,0x1d,0x07,0x0d,0x05,0x06,0x05\n"
    ".globl _10JiminyMemo$CharaIllustAGTbl\n"
    "_10JiminyMemo$CharaIllustAGTbl:\n"
    ".byte 0x07,0x07,0x06,0x00,0x1d,0x04,0x1d,0x06,0x1d,0x05,0x0d,0x06,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaIllustHATbl\n"
    "_10JiminyMemo$CharaIllustHATbl:\n"
    ".byte 0x64,0x06,0x64,0x04,0x64,0x05,0x08,0x01,0x1d,0x02,0x1d,0x03,0x0d,0x07,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaIllustMOTbl\n"
    "_10JiminyMemo$CharaIllustMOTbl:\n"
    ".byte 0x1c,0x06,0x1c,0x07,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaIllustATTbl\n"
    "_10JiminyMemo$CharaIllustATTbl:\n"
    ".byte 0x08,0x00,0x1d,0x00,0x1d,0x01,0x0e,0x00\n"
    ".globl _10JiminyMemo$CharaIllustNETbl\n"
    "_10JiminyMemo$CharaIllustNETbl:\n"
    ".byte 0x08,0x02,0x06,0x03,0x1e,0x01,0x0e,0x01\n"
    ".globl _10JiminyMemo$CharaIllustHOTbl\n"
    "_10JiminyMemo$CharaIllustHOTbl:\n"
    ".byte 0x08,0x03,0x1e,0x02,0x1e,0x03,0x0e,0x02\n"
    ".globl _10JiminyMemo$CharaIllustPOTbl\n"
    "_10JiminyMemo$CharaIllustPOTbl:\n"
    ".byte 0x1e,0x07,0x1f,0x01,0x1f,0x04,0x1f,0x05,0x1f,0x02,0x1f,0x00,0x1f,0x03,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaIllustDETbl\n"
    "_10JiminyMemo$CharaIllustDETbl:\n"
    ".byte 0x1e,0x04,0x1e,0x05,0x1e,0x06,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaIllustHeartLessTbl\n"
    "_10JiminyMemo$CharaIllustHeartLessTbl:\n"
    ".byte 0x09,0x01,0x09,0x02,0x09,0x03,0x09,0x04,0x09,0x05,0x09,0x06,0x09,0x07,0x0a,0x00\n"
    ".byte 0x0a,0x01,0x0a,0x02,0x0a,0x03,0x0a,0x04,0x0a,0x05,0x0a,0x06,0x0a,0x07,0x0b,0x00\n"
    ".byte 0x0b,0x01,0x0b,0x02,0x0b,0x03,0x0b,0x04,0x0b,0x05,0x0b,0x06,0x0b,0x07,0x0c,0x00\n"
    ".byte 0x0c,0x01,0x0c,0x02,0x0c,0x03,0x0c,0x04,0x0c,0x05,0x0c,0x06,0x0c,0x07,0x0d,0x00\n"
    ".byte 0x0d,0x01,0x0d,0x02,0x0d,0x03,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaIllustRiku1Tbl\n"
    "_10JiminyMemo$CharaIllustRiku1Tbl:\n"
    ".byte 0x64,0x00,0x64,0x04,0x64,0x01,0x64,0x05,0x1b,0x00,0x1b,0x04,0x1f,0x06,0x0e,0x03\n"
    ".byte 0x1f,0x07,0x0f,0x01,0x0e,0x06,0x0f,0x00,0x0f,0x03,0x0e,0x04,0x0e,0x07,0x0e,0x05\n"
    ".globl _10JiminyMemo$CharaIllustRiku2Tbl\n"
    "_10JiminyMemo$CharaIllustRiku2Tbl:\n"
    ".byte 0x1e,0x03,0x0d,0x06,0x0e,0x00,0x0d,0x05,0x0d,0x07,0x0e,0x01,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaIllustRikuHeartLessTbl\n"
    "_10JiminyMemo$CharaIllustRikuHeartLessTbl:\n"
    ".byte 0x09,0x01,0x09,0x02,0x09,0x03,0x09,0x04,0x09,0x05,0x09,0x06,0x09,0x07,0x0a,0x00\n"
    ".byte 0x0a,0x01,0x0a,0x02,0x0a,0x03,0x0a,0x04,0x0a,0x05,0x0a,0x06,0x0a,0x07,0x0b,0x00\n"
    ".byte 0x0b,0x01,0x0b,0x02,0x0b,0x03,0x0b,0x04,0x0b,0x05,0x0b,0x06,0x0b,0x07,0x0c,0x00\n"
    ".byte 0x0c,0x01,0x0c,0x02,0x0c,0x04,0x0c,0x05,0x0c,0x06,0x0c,0x07,0x0d,0x00,0x0d,0x01\n"
    ".byte 0x0d,0x02,0x0d,0x03,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaNameMain1Tbl\n"
    "_10JiminyMemo$CharaNameMain1Tbl:\n"
    ".byte 0x01,0x1d,0x02,0x1d,0x03,0x1d,0x04,0x1d,0x0e,0x1d,0x05,0x1d,0x06,0x1d,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaNameMain2Tbl\n"
    "_10JiminyMemo$CharaNameMain2Tbl:\n"
    ".byte 0x07,0x1d,0x08,0x1d,0x0b,0x1d,0x09,0x1d,0x0c,0x1d,0x0a,0x1d,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaNameEtcTbl\n"
    "_10JiminyMemo$CharaNameEtcTbl:\n"
    ".byte 0x01,0x1e,0x02,0x1e,0x03,0x1e,0x04,0x1e,0x0d,0x1d,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaNameTRTbl\n"
    "_10JiminyMemo$CharaNameTRTbl:\n"
    ".byte 0x01,0x1f,0x03,0x1f,0x02,0x1f,0x04,0x1f\n"
    ".globl _10JiminyMemo$CharaNameWOTbl\n"
    "_10JiminyMemo$CharaNameWOTbl:\n"
    ".byte 0x01,0x20,0x02,0x20,0x03,0x20,0x04,0x20,0x05,0x20,0x06,0x20,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaNameOLTbl\n"
    "_10JiminyMemo$CharaNameOLTbl:\n"
    ".byte 0x01,0x21,0x02,0x21,0x03,0x21,0x04,0x21\n"
    ".globl _10JiminyMemo$CharaNameAGTbl\n"
    "_10JiminyMemo$CharaNameAGTbl:\n"
    ".byte 0x01,0x22,0x02,0x22,0x03,0x22,0x04,0x22,0x05,0x22,0x06,0x22,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaNameHATbl\n"
    "_10JiminyMemo$CharaNameHATbl:\n"
    ".byte 0x01,0x1d,0x02,0x1d,0x03,0x1d,0x01,0x23,0x02,0x23,0x03,0x23,0x04,0x23,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaNameMOTbl\n"
    "_10JiminyMemo$CharaNameMOTbl:\n"
    ".byte 0x01,0x24,0x02,0x24,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaNameATTbl\n"
    "_10JiminyMemo$CharaNameATTbl:\n"
    ".byte 0x01,0x25,0x02,0x25,0x03,0x25,0x04,0x25\n"
    ".globl _10JiminyMemo$CharaNameNETbl\n"
    "_10JiminyMemo$CharaNameNETbl:\n"
    ".byte 0x01,0x26,0x02,0x26,0x03,0x26,0x04,0x26\n"
    ".globl _10JiminyMemo$CharaNameHOTbl\n"
    "_10JiminyMemo$CharaNameHOTbl:\n"
    ".byte 0x01,0x27,0x02,0x27,0x03,0x27,0x04,0x27\n"
    ".globl _10JiminyMemo$CharaNamePOTbl\n"
    "_10JiminyMemo$CharaNamePOTbl:\n"
    ".byte 0x01,0x28,0x02,0x28,0x03,0x28,0x04,0x28,0x05,0x28,0x06,0x28,0x07,0x28,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaNameDETbl\n"
    "_10JiminyMemo$CharaNameDETbl:\n"
    ".byte 0x01,0x29,0x02,0x29,0x03,0x29,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaNameHeartLessTbl\n"
    "_10JiminyMemo$CharaNameHeartLessTbl:\n"
    ".byte 0x01,0x2a,0x02,0x2a,0x03,0x2a,0x04,0x2a,0x05,0x2a,0x06,0x2a,0x07,0x2a,0x08,0x2a\n"
    ".byte 0x09,0x2a,0x0a,0x2a,0x0b,0x2a,0x0c,0x2a,0x0d,0x2a,0x0e,0x2a,0x0f,0x2a,0x10,0x2a\n"
    ".byte 0x11,0x2a,0x12,0x2a,0x13,0x2a,0x14,0x2a,0x15,0x2a,0x16,0x2a,0x17,0x2a,0x18,0x2a\n"
    ".byte 0x19,0x2a,0x1a,0x2a,0x1b,0x2a,0x1c,0x2a,0x1d,0x2a,0x1e,0x2a,0x1f,0x2a,0x20,0x2a\n"
    ".byte 0x21,0x2a,0x22,0x2a,0x23,0x2a,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaNameRiku1Tbl\n"
    "_10JiminyMemo$CharaNameRiku1Tbl:\n"
    ".byte 0x01,0x3b,0x02,0x3b,0x03,0x3b,0x04,0x3b,0x05,0x3b,0x06,0x3b,0x07,0x3b,0x08,0x3b\n"
    ".byte 0x10,0x3b,0x09,0x3b,0x0a,0x3b,0x0b,0x3b,0x0c,0x3b,0x0d,0x3b,0x0e,0x3b,0x0f,0x3b\n"
    ".globl _10JiminyMemo$CharaNameRiku2Tbl\n"
    "_10JiminyMemo$CharaNameRiku2Tbl:\n"
    ".byte 0x01,0x3c,0x02,0x3c,0x03,0x3c,0x04,0x3c,0x05,0x3c,0x06,0x3c,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaNameRikuHeartLessTbl\n"
    "_10JiminyMemo$CharaNameRikuHeartLessTbl:\n"
    ".byte 0x01,0x2a,0x02,0x2a,0x03,0x2a,0x04,0x2a,0x05,0x2a,0x06,0x2a,0x07,0x2a,0x08,0x2a\n"
    ".byte 0x09,0x2a,0x0a,0x2a,0x0b,0x2a,0x0c,0x2a,0x0d,0x2a,0x0e,0x2a,0x0f,0x2a,0x10,0x2a\n"
    ".byte 0x11,0x2a,0x12,0x2a,0x13,0x2a,0x14,0x2a,0x15,0x2a,0x16,0x2a,0x17,0x2a,0x18,0x2a\n"
    ".byte 0x19,0x2a,0x1a,0x2a,0x1c,0x2a,0x1d,0x2a,0x1e,0x2a,0x1f,0x2a,0x20,0x2a,0x21,0x2a\n"
    ".byte 0x22,0x2a,0x23,0x2a,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaManualMain1Tbl\n"
    "_10JiminyMemo$CharaManualMain1Tbl:\n"
    ".byte 0x01,0x2d,0x02,0x2d,0x03,0x2d,0x04,0x2d,0x0e,0x2d,0x05,0x2d,0x06,0x2d,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaManualMain2Tbl\n"
    "_10JiminyMemo$CharaManualMain2Tbl:\n"
    ".byte 0x07,0x2d,0x08,0x2d,0x0b,0x2d,0x09,0x2d,0x0c,0x2d,0x0a,0x2d,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaManualEtcTbl\n"
    "_10JiminyMemo$CharaManualEtcTbl:\n"
    ".byte 0x01,0x2e,0x02,0x2e,0x03,0x2e,0x04,0x2e,0x0d,0x2d,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaManualTRTbl\n"
    "_10JiminyMemo$CharaManualTRTbl:\n"
    ".byte 0x01,0x2f,0x03,0x2f,0x02,0x2f,0x04,0x2f\n"
    ".globl _10JiminyMemo$CharaManualWOTbl\n"
    "_10JiminyMemo$CharaManualWOTbl:\n"
    ".byte 0x01,0x30,0x02,0x30,0x03,0x30,0x04,0x30,0x05,0x30,0x06,0x30,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaManualOLTbl\n"
    "_10JiminyMemo$CharaManualOLTbl:\n"
    ".byte 0x01,0x31,0x02,0x31,0x03,0x31,0x04,0x31\n"
    ".globl _10JiminyMemo$CharaManualAGTbl\n"
    "_10JiminyMemo$CharaManualAGTbl:\n"
    ".byte 0x01,0x32,0x02,0x32,0x03,0x32,0x04,0x32,0x05,0x32,0x06,0x32,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaManualHATbl\n"
    "_10JiminyMemo$CharaManualHATbl:\n"
    ".byte 0x01,0x2d,0x02,0x2d,0x03,0x2d,0x01,0x33,0x02,0x33,0x03,0x33,0x04,0x33,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaManualMOTbl\n"
    "_10JiminyMemo$CharaManualMOTbl:\n"
    ".byte 0x01,0x34,0x02,0x34,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaManualATTbl\n"
    "_10JiminyMemo$CharaManualATTbl:\n"
    ".byte 0x01,0x35,0x02,0x35,0x03,0x35,0x04,0x35\n"
    ".globl _10JiminyMemo$CharaManualNETbl\n"
    "_10JiminyMemo$CharaManualNETbl:\n"
    ".byte 0x01,0x36,0x02,0x36,0x03,0x36,0x04,0x36\n"
    ".globl _10JiminyMemo$CharaManualHOTbl\n"
    "_10JiminyMemo$CharaManualHOTbl:\n"
    ".byte 0x01,0x37,0x02,0x37,0x03,0x37,0x04,0x37\n"
    ".globl _10JiminyMemo$CharaManualPOTbl\n"
    "_10JiminyMemo$CharaManualPOTbl:\n"
    ".byte 0x01,0x38,0x02,0x38,0x03,0x38,0x04,0x38,0x05,0x38,0x06,0x38,0x07,0x38,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaManualDETbl\n"
    "_10JiminyMemo$CharaManualDETbl:\n"
    ".byte 0x01,0x39,0x02,0x39,0x03,0x39,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaManualHeartLessTbl\n"
    "_10JiminyMemo$CharaManualHeartLessTbl:\n"
    ".byte 0x01,0x3a,0x02,0x3a,0x03,0x3a,0x04,0x3a,0x05,0x3a,0x06,0x3a,0x07,0x3a,0x08,0x3a\n"
    ".byte 0x09,0x3a,0x0a,0x3a,0x0b,0x3a,0x0c,0x3a,0x0d,0x3a,0x0e,0x3a,0x0f,0x3a,0x10,0x3a\n"
    ".byte 0x11,0x3a,0x12,0x3a,0x13,0x3a,0x14,0x3a,0x15,0x3a,0x16,0x3a,0x17,0x3a,0x18,0x3a\n"
    ".byte 0x19,0x3a,0x1a,0x3a,0x1b,0x3a,0x1c,0x3a,0x1d,0x3a,0x1e,0x3a,0x1f,0x3a,0x20,0x3a\n"
    ".byte 0x21,0x3a,0x22,0x3a,0x23,0x3a,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaManualRiku1Tbl\n"
    "_10JiminyMemo$CharaManualRiku1Tbl:\n"
    ".byte 0x01,0x3d,0x0f,0x3d,0x02,0x3d,0x10,0x3d,0x03,0x3d,0x04,0x3d,0x05,0x3d,0x06,0x3d\n"
    ".byte 0x0e,0x3d,0x07,0x3d,0x08,0x3d,0x09,0x3d,0x0a,0x3d,0x0b,0x3d,0x0c,0x3d,0x0d,0x3d\n"
    ".globl _10JiminyMemo$CharaManualRiku2Tbl\n"
    "_10JiminyMemo$CharaManualRiku2Tbl:\n"
    ".byte 0x01,0x3e,0x02,0x3e,0x03,0x3e,0x04,0x3e,0x05,0x3e,0x06,0x3e,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaManualRikuHeartLessTbl\n"
    "_10JiminyMemo$CharaManualRikuHeartLessTbl:\n"
    ".byte 0x01,0x3a,0x02,0x3a,0x03,0x3a,0x04,0x3a,0x05,0x3a,0x06,0x3a,0x07,0x3a,0x08,0x3a\n"
    ".byte 0x09,0x3a,0x0a,0x3a,0x0b,0x3a,0x0c,0x3a,0x0d,0x3a,0x0e,0x3a,0x0f,0x3a,0x10,0x3a\n"
    ".byte 0x11,0x3a,0x12,0x3a,0x13,0x3a,0x14,0x3a,0x15,0x3a,0x16,0x3a,0x17,0x3a,0x18,0x3a\n"
    ".byte 0x19,0x3a,0x1a,0x3a,0x1c,0x3a,0x1d,0x3a,0x1e,0x3a,0x1f,0x3a,0x01,0x3f,0x02,0x3f\n"
    ".byte 0x03,0x3f,0x04,0x3f,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaNumTbl\n"
    "_10JiminyMemo$CharaNumTbl:\n"
    ".byte 0x07,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x04,0x00,0x00,0x00\n"
    ".byte 0x06,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x07,0x00,0x00,0x00\n"
    ".byte 0x02,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x04,0x00,0x00,0x00\n"
    ".byte 0x07,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x23,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$RikuCharaNumTbl\n"
    "_10JiminyMemo$RikuCharaNumTbl:\n"
    ".byte 0x10,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x22,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _10JiminyMemo$CharaModelMain1Tbl\n"
    "_10JiminyMemo$CharaModelMain1Tbl:\n"
    ".byte 0xd1,0xc7,0x73,0x01,0xda,0xff,0xb4,0x00,0xd2,0xc7,0xa6,0x01,0xe7,0xff,0xb4,0x00\n"
    ".byte 0xd3,0xc7,0x4c,0x01,0xda,0xff,0xb4,0x00,0xd4,0xc7,0x00,0x07,0xcd,0xff,0xb4,0x00\n"
    ".byte 0x3f,0xc8,0xcc,0x01,0xf4,0xff,0xb4,0x00,0xd5,0xc7,0x73,0x01,0xda,0xff,0xb4,0x00\n"
    ".byte 0xd6,0xc7,0x99,0x01,0xda,0xff,0xb4,0x00\n"
    ".globl _10JiminyMemo$CharaModelMain2Tbl\n"
    "_10JiminyMemo$CharaModelMain2Tbl:\n"
    ".byte 0xd7,0xc7,0x99,0x01,0xda,0xff,0xb4,0x00,0xd8,0xc7,0x73,0x01,0xcd,0xff,0xb4,0x00\n"
    ".byte 0xdb,0xc7,0x0c,0x01,0xda,0xff,0xb4,0x00,0xd9,0xc7,0x1e,0x01,0xcd,0xff,0xb4,0x00\n"
    ".byte 0xdc,0xc7,0x2e,0x01,0xc0,0xff,0xb4,0x00,0xda,0xc7,0x26,0x01,0xda,0xff,0xb4,0x00\n"
    ".globl _10JiminyMemo$CharaModelEtcTbl\n"
    "_10JiminyMemo$CharaModelEtcTbl:\n"
    ".byte 0xdd,0xc7,0x33,0x01,0xe7,0xff,0xb4,0x00,0xde,0xc7,0x99,0x01,0xda,0xff,0xb4,0x00\n"
    ".byte 0xdf,0xc7,0xe6,0x01,0xda,0xff,0xb4,0x00,0xe0,0xc7,0x66,0x03,0xcd,0xff,0xb4,0x00\n"
    ".byte 0xe1,0xc7,0x00,0x02,0xda,0xff,0xb4,0x00\n"
    ".globl _10JiminyMemo$CharaModelTRTbl\n"
    "_10JiminyMemo$CharaModelTRTbl:\n"
    ".byte 0xe2,0xc7,0x40,0x01,0xcd,0xff,0xb4,0x00,0xe3,0xc7,0x59,0x01,0xcd,0xff,0xb4,0x00\n"
    ".byte 0xe4,0xc7,0x40,0x01,0xcd,0xff,0xb4,0x00,0xe5,0xc7,0x4c,0x01,0xcd,0xff,0xb4,0x00\n"
    ".globl _10JiminyMemo$CharaModelWOTbl\n"
    "_10JiminyMemo$CharaModelWOTbl:\n"
    ".byte 0xe6,0xc7,0xd9,0x01,0xcd,0xff,0xb4,0x00,0xe7,0xc7,0x26,0x01,0xe7,0xff,0xb4,0x00\n"
    ".byte 0xe8,0xc7,0x40,0x02,0xda,0xff,0xb4,0x00,0xe9,0xc7,0x00,0x01,0xcd,0xff,0xb4,0x00\n"
    ".byte 0xea,0xc7,0x00,0x01,0xcd,0xff,0xb4,0x00,0xeb,0xc7,0x4c,0x02,0x26,0x00,0xb4,0x00\n"
    ".globl _10JiminyMemo$CharaModelOLTbl\n"
    "_10JiminyMemo$CharaModelOLTbl:\n"
    ".byte 0xec,0xc7,0x00,0x01,0xcd,0xff,0xb4,0x00,0xed,0xc7,0x99,0x01,0xcd,0xff,0xb4,0x00\n"
    ".byte 0xee,0xc7,0xd9,0x00,0xd2,0xff,0xb4,0x00,0xef,0xc7,0x40,0x01,0xcd,0xff,0xb4,0x00\n"
    ".globl _10JiminyMemo$CharaModelAGTbl\n"
    "_10JiminyMemo$CharaModelAGTbl:\n"
    ".byte 0xf0,0xc7,0x33,0x01,0xd2,0xff,0xb4,0x00,0xf1,0xc7,0x9e,0x00,0x3e,0xff,0xb4,0x00\n"
    ".byte 0xf2,0xc7,0x59,0x01,0xc0,0xff,0xb4,0x00,0xf3,0xc7,0x4c,0x03,0xda,0xff,0xb4,0x00\n"
    ".byte 0xf4,0xc7,0x00,0x01,0xcd,0xff,0xb4,0x00,0xf5,0xc7,0x4c,0x00,0x73,0x00,0xb4,0x00\n"
    ".globl _10JiminyMemo$CharaModelHATbl\n"
    "_10JiminyMemo$CharaModelHATbl:\n"
    ".byte 0xf6,0xc7,0x73,0x01,0xda,0xff,0xb4,0x00,0xf7,0xc7,0xa6,0x01,0xe7,0xff,0xb4,0x00\n"
    ".byte 0xf8,0xc7,0x4c,0x01,0xda,0xff,0xb4,0x00,0xf9,0xc7,0x1e,0x01,0xc0,0xff,0xb4,0x00\n"
    ".byte 0xfa,0xc7,0x40,0x01,0xc6,0xff,0xb4,0x00,0xfb,0xc7,0xa6,0x01,0xc0,0xff,0xb4,0x00\n"
    ".byte 0xfc,0xc7,0xf8,0x00,0xcd,0xff,0xb4,0x00\n"
    ".globl _10JiminyMemo$CharaModelMOTbl\n"
    "_10JiminyMemo$CharaModelMOTbl:\n"
    ".byte 0xfd,0xc7,0xe6,0x01,0xda,0xff,0xb4,0x00,0xfe,0xc7,0x4c,0x01,0xcd,0xff,0xb4,0x00\n"
    ".globl _10JiminyMemo$CharaModelATTbl\n"
    "_10JiminyMemo$CharaModelATTbl:\n"
    ".byte 0xff,0xc7,0x59,0x01,0xe7,0xff,0xb4,0x00,0x00,0xc8,0x00,0x05,0x33,0x00,0xb4,0x00\n"
    ".byte 0x01,0xc8,0x80,0x02,0x26,0x00,0xb4,0x00,0x02,0xc8,0x00,0x01,0xa7,0xff,0xb4,0x00\n"
    ".globl _10JiminyMemo$CharaModelNETbl\n"
    "_10JiminyMemo$CharaModelNETbl:\n"
    ".byte 0x03,0xc8,0x59,0x01,0xcd,0xff,0xb4,0x00,0x04,0xc8,0x00,0x08,0xf4,0xff,0xb4,0x00\n"
    ".byte 0x05,0xc8,0x66,0x01,0xc0,0xff,0xb4,0x00,0x06,0xc8,0x26,0x01,0xb4,0xff,0xb4,0x00\n"
    ".globl _10JiminyMemo$CharaModelHOTbl\n"
    "_10JiminyMemo$CharaModelHOTbl:\n"
    ".byte 0x07,0xc8,0x11,0x01,0xda,0xff,0xb4,0x00,0x08,0xc8,0x33,0x01,0xcd,0xff,0xb4,0x00\n"
    ".byte 0x09,0xc8,0x0c,0x01,0xcd,0xff,0xb4,0x00,0x0a,0xc8,0x3d,0x00,0x00,0x00,0xb4,0x00\n"
    ".globl _10JiminyMemo$CharaModelPOTbl\n"
    "_10JiminyMemo$CharaModelPOTbl:\n"
    ".byte 0x0b,0xc8,0x00,0x02,0xc0,0xff,0xb4,0x00,0x0c,0xc8,0x80,0x04,0xda,0xff,0xb4,0x00\n"
    ".byte 0x0d,0xc8,0xe6,0x01,0xda,0xff,0xb4,0x00,0x0e,0xc8,0x99,0x03,0xda,0xff,0xb4,0x00\n"
    ".byte 0x0f,0xc8,0x00,0x02,0xe7,0xff,0xb4,0x00,0x10,0xc8,0x33,0x02,0xcd,0xff,0xb4,0x00\n"
    ".byte 0x11,0xc8,0xe6,0x01,0xda,0xff,0xb4,0x00\n"
    ".globl _10JiminyMemo$CharaModelDETbl\n"
    "_10JiminyMemo$CharaModelDETbl:\n"
    ".byte 0x12,0xc8,0x73,0x01,0xcd,0xff,0xb4,0x00,0x13,0xc8,0x4c,0x01,0xcd,0xff,0xb4,0x00\n"
    ".byte 0x14,0xc8,0x73,0x01,0xcd,0xff,0xb4,0x00\n"
    ".globl _10JiminyMemo$CharaModelHeartLessTbl\n"
    "_10JiminyMemo$CharaModelHeartLessTbl:\n"
    ".byte 0x15,0xc8,0x0c,0x01,0xe7,0xff,0xb4,0x00,0x16,0xc8,0x19,0x01,0xe7,0xff,0xb4,0x00\n"
    ".byte 0x17,0xc8,0xe6,0x00,0xe7,0xff,0xb4,0x00,0x18,0xc8,0x80,0x01,0x00,0x00,0xb4,0x00\n"
    ".byte 0x19,0xc8,0x80,0x01,0x00,0x00,0xb4,0x00,0x1a,0xc8,0x80,0x01,0x00,0x00,0xb4,0x00\n"
    ".byte 0x1b,0xc8,0x80,0x01,0x00,0x00,0xb4,0x00,0x1c,0xc8,0x40,0x01,0xe7,0xff,0xb4,0x00\n"
    ".byte 0x1d,0xc8,0x4c,0x01,0xe7,0xff,0xb4,0x00,0x1e,0xc8,0x40,0x01,0x00,0x00,0xb4,0x00\n"
    ".byte 0x1f,0xc8,0x19,0x01,0xe7,0xff,0xb4,0x00,0x20,0xc8,0xe6,0x00,0xda,0xff,0xb4,0x00\n"
    ".byte 0x21,0xc8,0x00,0x01,0x19,0x00,0xb4,0x00,0x22,0xc8,0x33,0x01,0x00,0x00,0xb4,0x00\n"
    ".byte 0x23,0x48,0x59,0x01,0x00,0x00,0xb4,0x00,0x24,0xc8,0x19,0x01,0x00,0x00,0xb4,0x00\n"
    ".byte 0x25,0xc8,0x8c,0x00,0x74,0xff,0xb4,0x00,0x26,0xc8,0x19,0x01,0xda,0xff,0xb4,0x00\n"
    ".byte 0x27,0xc8,0x26,0x01,0xe7,0xff,0xb4,0x00,0x28,0xc8,0x33,0x01,0xe7,0xff,0xb4,0x00\n"
    ".byte 0x29,0xc8,0x19,0x01,0xb4,0xff,0xb4,0x00,0x2a,0xc8,0x0c,0x01,0x4d,0xff,0xb4,0x00\n"
    ".byte 0x2b,0xc8,0x00,0x01,0xda,0xff,0xb4,0x00,0x2c,0xc8,0x0c,0x01,0xcd,0xff,0xb4,0x00\n"
    ".byte 0x2d,0xc8,0x4c,0x01,0xc0,0xff,0xb4,0x00,0x2e,0xc8,0x33,0x01,0xf4,0xff,0xb4,0x00\n"
    ".byte 0x2f,0xc8,0x4c,0x01,0xe7,0xff,0xb4,0x00,0x30,0xc8,0x4c,0x01,0xe7,0xff,0xb4,0x00\n"
    ".byte 0x31,0xc8,0x0c,0x01,0x00,0x00,0xb4,0x00,0x32,0xc8,0x4c,0x01,0xe7,0xff,0xb4,0x00\n"
    ".byte 0x33,0xc8,0x19,0x01,0x00,0x00,0xb4,0x00,0x34,0xc8,0x99,0x00,0xb4,0xff,0xb4,0x00\n"
    ".word _59_GLOBAL_$N$c__KHCOM_MainApp_System_Sound_SoundMan.cppG7aaaa$s_pCrossBuffer + 0x25535\n"
    ".byte 0xe7,0xff,0xb4,0x00,0x36,0xc8,0x73,0x00,0xb4,0xff,0xb4,0x00\n"
    ".word sintable + 0x5F7\n"
    ".byte 0x8d,0xff,0xb4,0x00\n"
    ".globl _10JiminyMemo$CharaModelRiku1Tbl\n"
    "_10JiminyMemo$CharaModelRiku1Tbl:\n"
    ".byte 0xd5,0xc7,0x73,0x01,0xda,0xff,0xb4,0x00,0x38,0xc8,0x73,0x01,0xda,0xff,0xb4,0x00\n"
    ".byte 0x39,0xc8,0xcc,0x01,0xda,0xff,0xb4,0x00,0x3a,0xc8,0xcc,0x01,0xda,0xff,0xb4,0x00\n"
    ".byte 0xd1,0xc7,0x73,0x01,0xda,0xff,0xb4,0x00,0xd6,0xc7,0x99,0x01,0xda,0xff,0xb4,0x00\n"
    ".byte 0xd7,0xc7,0x99,0x01,0xda,0xff,0xb4,0x00,0xd8,0xc7,0x73,0x01,0xcd,0xff,0xb4,0x00\n"
    ".byte 0x3e,0xc8,0x33,0x01,0xda,0xff,0xb4,0x00,0x3b,0xc8,0x26,0x01,0xcd,0xff,0xb4,0x00\n"
    ".byte 0xdb,0xc7,0x0c,0x01,0xda,0xff,0xb4,0x00,0x3c,0xc8,0x0c,0x01,0xcd,0xff,0xb4,0x00\n"
    ".byte 0x3d,0xc8,0x1e,0x01,0xcd,0xff,0xb4,0x00,0xd9,0xc7,0x26,0x01,0xcd,0xff,0xb4,0x00\n"
    ".byte 0xdc,0xc7,0x2e,0x01,0xc0,0xff,0xb4,0x00,0xda,0xc7,0x26,0x01,0xda,0xff,0xb4,0x00\n"
    ".globl _10JiminyMemo$CharaModelRiku2Tbl\n"
    "_10JiminyMemo$CharaModelRiku2Tbl:\n"
    ".byte 0x09,0xc8,0x0c,0x01,0xcd,0xff,0xb4,0x00,0xf5,0xc7,0x4c,0x00,0x73,0x00,0xb4,0x00\n"
    ".byte 0x02,0xc8,0x00,0x01,0xa7,0xff,0xb4,0x00,0xee,0xc7,0xd9,0x00,0xd2,0xff,0xb4,0x00\n"
    ".byte 0xfc,0xc7,0xf8,0x00,0xcd,0xff,0xb4,0x00,0x06,0xc8,0x26,0x01,0xb4,0xff,0xb4,0x00\n"
    ".globl _10JiminyMemo$CharaModelRikuHeartLessTbl\n"
    "_10JiminyMemo$CharaModelRikuHeartLessTbl:\n"
    ".byte 0x15,0xc8,0x0c,0x01,0xe7,0xff,0xb4,0x00,0x16,0xc8,0x19,0x01,0xe7,0xff,0xb4,0x00\n"
    ".byte 0x17,0xc8,0xe6,0x00,0xe7,0xff,0xb4,0x00,0x18,0xc8,0x80,0x01,0x00,0x00,0xb4,0x00\n"
    ".byte 0x19,0xc8,0x80,0x01,0x00,0x00,0xb4,0x00,0x1a,0xc8,0x80,0x01,0x00,0x00,0xb4,0x00\n"
    ".byte 0x1b,0xc8,0x80,0x01,0x00,0x00,0xb4,0x00,0x1c,0xc8,0x40,0x01,0xe7,0xff,0xb4,0x00\n"
    ".byte 0x1d,0xc8,0x4c,0x01,0xe7,0xff,0xb4,0x00,0x1e,0xc8,0x40,0x01,0x00,0x00,0xb4,0x00\n"
    ".byte 0x1f,0xc8,0x19,0x01,0xe7,0xff,0xb4,0x00,0x20,0xc8,0xe6,0x00,0xda,0xff,0xb4,0x00\n"
    ".byte 0x21,0xc8,0x00,0x01,0x19,0x00,0xb4,0x00,0x22,0xc8,0x33,0x01,0x00,0x00,0xb4,0x00\n"
    ".byte 0x23,0x08,0x59,0x01,0x00,0x00,0xb4,0x00,0x24,0xc8,0x19,0x01,0x00,0x00,0xb4,0x00\n"
    ".byte 0x25,0xc8,0x8c,0x00,0x74,0xff,0xb4,0x00,0x26,0xc8,0x19,0x01,0xda,0xff,0xb4,0x00\n"
    ".byte 0x27,0xc8,0x26,0x01,0xe7,0xff,0xb4,0x00,0x28,0xc8,0x33,0x01,0xe7,0xff,0xb4,0x00\n"
    ".byte 0x29,0xc8,0x19,0x01,0xb4,0xff,0xb4,0x00,0x2a,0xc8,0x0c,0x01,0x4d,0xff,0xb4,0x00\n"
    ".byte 0x2b,0xc8,0x00,0x01,0xda,0xff,0xb4,0x00,0x2c,0xc8,0x0c,0x01,0xcd,0xff,0xb4,0x00\n"
    ".byte 0x2d,0xc8,0x4c,0x01,0xc0,0xff,0xb4,0x00,0x2e,0xc8,0x33,0x01,0xf4,0xff,0xb4,0x00\n"
    ".byte 0x30,0xc8,0x4c,0x01,0xe7,0xff,0xb4,0x00,0x31,0xc8,0x0c,0x01,0x00,0x00,0xb4,0x00\n"
    ".byte 0x32,0xc8,0x4c,0x01,0xe7,0xff,0xb4,0x00,0x33,0xc8,0x19,0x01,0x00,0x00,0xb4,0x00\n"
    ".byte 0x34,0xc8,0x99,0x00,0xb4,0xff,0xb4,0x00\n"
    ".word _59_GLOBAL_$N$c__KHCOM_MainApp_System_Sound_SoundMan.cppG7aaaa$s_pCrossBuffer + 0x25535\n"
    ".byte 0xe7,0xff,0xb4,0x00,0x36,0xc8,0x73,0x00,0xb4,0xff,0xb4,0x00\n"
    ".word sintable + 0x5F7\n"
    ".byte 0x8d,0xff,0xb4,0x00\n"
    ".globl _Q210JiminyMemo8Minigame$NoteSeqData\n"
    "_Q210JiminyMemo8Minigame$NoteSeqData:\n"
    ".byte 0xc4,0x00,0x00,0x00,0xc5,0x00,0x00,0x00,0xc6,0x00,0x00,0x00,0xc4,0x00,0x00,0x00\n"
    ".byte 0xc6,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo9CharaView$NoteSeqData\n"
    "_Q210JiminyMemo9CharaView$NoteSeqData:\n"
    ".byte 0xba,0x00,0x00,0x00,0xbb,0x00,0x00,0x00,0xbc,0x00,0x00,0x00,0xff,0x00,0x00,0x00\n"
    ".byte 0xf4,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo9CharaView$EnemyNoteSeqData\n"
    "_Q210JiminyMemo9CharaView$EnemyNoteSeqData:\n"
    ".byte 0xc0,0x00,0x00,0x00,0xc1,0x00,0x00,0x00,0xc2,0x00,0x00,0x00,0x01,0x01,0x00,0x00\n"
    ".byte 0xf6,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo9CharaView$DuskyNoteSeqData\n"
    "_Q210JiminyMemo9CharaView$DuskyNoteSeqData:\n"
    ".byte 0xbd,0x00,0x00,0x00,0xbe,0x00,0x00,0x00,0xbf,0x00,0x00,0x00,0x00,0x01,0x00,0x00\n"
    ".byte 0xf5,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo9CharaView$ModelPosi\n"
    "_Q210JiminyMemo9CharaView$ModelPosi:\n"
    ".byte 0x00,0x00,0x00,0x00,0xcd,0xcc,0xcc,0xbd,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x3f\n"
    ".globl _Q210JiminyMemo11CharaSelect$NoteSeqData\n"
    "_Q210JiminyMemo11CharaSelect$NoteSeqData:\n"
    ".byte 0xc7,0x00,0x00,0x00,0xc8,0x00,0x00,0x00,0xc9,0x00,0x00,0x00,0x02,0x01,0x00,0x00\n"
    ".byte 0xf7,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo11CharaSelect$CursorPosiOffset\n"
    "_Q210JiminyMemo11CharaSelect$CursorPosiOffset:\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo11CharaSelect$NoteScrollBarPosi\n"
    "_Q210JiminyMemo11CharaSelect$NoteScrollBarPosi:\n"
    ".byte 0x00,0x00,0x0c,0x43,0x00,0x00,0x8c,0xc2,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x3f\n"
    ".globl _Q210JiminyMemo5Chara$NoteSeqData\n"
    "_Q210JiminyMemo5Chara$NoteSeqData:\n"
    ".byte 0xc7,0x00,0x00,0x00,0xc8,0x00,0x00,0x00,0xc9,0x00,0x00,0x00,0x02,0x01,0x00,0x00\n"
    ".byte 0xf7,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo5Chara$CursorPosiOffset\n"
    "_Q210JiminyMemo5Chara$CursorPosiOffset:\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo5Chara$KingMarkPosiOffset\n"
    "_Q210JiminyMemo5Chara$KingMarkPosiOffset:\n"
    ".byte 0x00,0x00,0xa0,0x40,0x00,0x00,0x40,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo5Chara$NoteScrollBarPosi\n"
    "_Q210JiminyMemo5Chara$NoteScrollBarPosi:\n"
    ".byte 0x00,0x00,0x0c,0x43,0x00,0x00,0x8c,0xc2,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x3f\n"
    ".globl _Q210JiminyMemo8CardView$NoteSeqData\n"
    "_Q210JiminyMemo8CardView$NoteSeqData:\n"
    ".byte 0xb0,0x00,0x00,0x00,0xb1,0x00,0x00,0x00,0xb2,0x00,0x00,0x00,0x02,0x01,0x00,0x00\n"
    ".byte 0xf7,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo10CardSelect$NoteSeqData\n"
    "_Q210JiminyMemo10CardSelect$NoteSeqData:\n"
    ".byte 0xc7,0x00,0x00,0x00,0xc8,0x00,0x00,0x00,0xc9,0x00,0x00,0x00,0x02,0x01,0x00,0x00\n"
    ".byte 0xf7,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo10CardSelect$CursorPosiOffset\n"
    "_Q210JiminyMemo10CardSelect$CursorPosiOffset:\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo10CardSelect$NoteScrollBarPosi\n"
    "_Q210JiminyMemo10CardSelect$NoteScrollBarPosi:\n"
    ".byte 0x00,0x00,0x0c,0x43,0x00,0x00,0x8c,0xc2,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x3f\n"
    ".globl _Q210JiminyMemo4Card$NoteSeqData\n"
    "_Q210JiminyMemo4Card$NoteSeqData:\n"
    ".byte 0xc7,0x00,0x00,0x00,0xc8,0x00,0x00,0x00,0xc9,0x00,0x00,0x00,0x02,0x01,0x00,0x00\n"
    ".byte 0xf7,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo4Card$CursorPosiOffset\n"
    "_Q210JiminyMemo4Card$CursorPosiOffset:\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo4Card$KingMarkPosiOffset\n"
    "_Q210JiminyMemo4Card$KingMarkPosiOffset:\n"
    ".byte 0x00,0x00,0xa0,0x40,0x00,0x00,0x40,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo4Card$NoteScrollBarPosi\n"
    "_Q210JiminyMemo4Card$NoteScrollBarPosi:\n"
    ".byte 0x00,0x00,0x0c,0x43,0x00,0x00,0x8c,0xc2,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x3f\n"
    ".globl _Q210JiminyMemo7Collect$BaseSeqData\n"
    "_Q210JiminyMemo7Collect$BaseSeqData:\n"
    ".byte 0xb7,0x00,0x00,0x00,0xb8,0x00,0x00,0x00,0xb9,0x00,0x00,0x00,0xfe,0x00,0x00,0x00\n"
    ".byte 0xf3,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo7Collect$NoteSeqData\n"
    "_Q210JiminyMemo7Collect$NoteSeqData:\n"
    ".byte 0xb3,0x00,0x00,0x00,0xb4,0x00,0x00,0x00,0xb5,0x00,0x00,0x00,0xfd,0x00,0x00,0x00\n"
    ".byte 0xf2,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo7Collect$LightSeqTbl\n"
    "_Q210JiminyMemo7Collect$LightSeqTbl:\n"
    ".byte 0x82,0x00,0x00,0x00,0xee,0x00,0x00,0x00,0x84,0x00,0x00,0x00,0x83,0x00,0x00,0x00\n"
    ".byte 0x83,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo7Collect$FrameSeqTbl\n"
    "_Q210JiminyMemo7Collect$FrameSeqTbl:\n"
    ".byte 0x7b,0x00,0x00,0x00,0x7c,0x00,0x00,0x00,0x7d,0x00,0x00,0x00,0x7a,0x00,0x00,0x00\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo7Collect$FlickerSeqTbl\n"
    "_Q210JiminyMemo7Collect$FlickerSeqTbl:\n"
    ".byte 0x7e,0x00,0x00,0x00,0x7e,0x00,0x00,0x00,0x7e,0x00,0x00,0x00,0x7e,0x00,0x00,0x00\n"
    ".byte 0x7e,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo7Collect$CursorMoveFlag\n"
    "_Q210JiminyMemo7Collect$CursorMoveFlag:\n"
    ".byte 0xfe,0xff,0x1f,0x00,0xff,0xff,0x3f,0x00,0xff,0xff,0x3f,0x00,0xff,0xff,0x3f,0x00\n"
    ".byte 0xff,0xff,0x3f,0x00,0xff,0xff,0x3f,0x00,0xfe,0xff,0x1f,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo7Collect$CursorMoveFlagWithSpecial\n"
    "_Q210JiminyMemo7Collect$CursorMoveFlagWithSpecial:\n"
    ".byte 0xfe,0xff,0x1f,0x00,0xff,0xff,0x3f,0x00,0xff,0xff,0x3f,0x00,0xff,0xff,0x3f,0x00\n"
    ".byte 0xff,0xff,0x3f,0x00,0xff,0xff,0x3f,0x00,0xff,0xff,0x3f,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo7Collect$CursorMapStart\n"
    "_Q210JiminyMemo7Collect$CursorMapStart:\n"
    ".byte 0x00,0x00,0x30,0xc4,0x00,0x00,0xa8,0xc3\n"
    ".globl _Q210JiminyMemo7Collect$CursorMapOffset\n"
    "_Q210JiminyMemo7Collect$CursorMapOffset:\n"
    ".byte 0x00,0x00,0x80,0x42,0x00,0x00,0xc0,0x42\n"
    ".globl _Q210JiminyMemo7Collect$CardMapOffset\n"
    "_Q210JiminyMemo7Collect$CardMapOffset:\n"
    ".byte 0x00,0x00,0xf0,0x42,0x00,0x00,0xec,0x42\n"
    ".globl _Q210JiminyMemo7Collect$CardMapSpacingTbl\n"
    "_Q210JiminyMemo7Collect$CardMapSpacingTbl:\n"
    ".byte 0x40,0x00,0x00,0x00,0x4a,0x00,0x00,0x00,0xf0,0xff,0xff,0xff,0xe2,0xff,0xff,0xff\n"
    ".globl _Q310JiminyMemo7Collect1D$CursorMoveFlag\n"
    "_Q310JiminyMemo7Collect1D$CursorMoveFlag:\n"
    ".byte 0xff,0xff,0x3f,0x00,0xff,0xff,0x3f,0x00,0xf0,0xff,0x07,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q310JiminyMemo7Collect1D$CursorMapStart\n"
    "_Q310JiminyMemo7Collect1D$CursorMapStart:\n"
    ".byte 0x00,0x00,0x30,0xc4,0x00,0x00,0x10,0xc3\n"
    ".globl _Q310JiminyMemo7Collect1D$CardMapOffset\n"
    "_Q310JiminyMemo7Collect1D$CardMapOffset:\n"
    ".byte 0x00,0x00,0xf0,0x42,0x00,0x00,0xec,0x42\n"
    ".globl _Q210JiminyMemo9StoryView$NoteSeqData\n"
    "_Q210JiminyMemo9StoryView$NoteSeqData:\n"
    ".byte 0xcd,0x00,0x00,0x00,0xce,0x00,0x00,0x00,0xcf,0x00,0x00,0x00,0x04,0x01,0x00,0x00\n"
    ".byte 0xf9,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo9StoryView$WorldPictureTbl\n"
    "_Q210JiminyMemo9StoryView$WorldPictureTbl:\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x02,0x00,0x03,0x00,0x04,0x00\n"
    ".byte 0x05,0x00,0x06,0x00,0x07,0x00,0x08,0x00,0x09,0x00,0x0a,0x00,0x0b,0x00,0x0c,0x00\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q310JiminyMemo9StoryView1D$WorldPictureTbl\n"
    "_Q310JiminyMemo9StoryView1D$WorldPictureTbl:\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo5Story$NoteSeqData\n"
    "_Q210JiminyMemo5Story$NoteSeqData:\n"
    ".byte 0xc7,0x00,0x00,0x00,0xc8,0x00,0x00,0x00,0xc9,0x00,0x00,0x00,0x02,0x01,0x00,0x00\n"
    ".byte 0xf7,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo5Story$CursorPosiOffset\n"
    "_Q210JiminyMemo5Story$CursorPosiOffset:\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo5Story$NoteScrollBarPosi\n"
    "_Q210JiminyMemo5Story$NoteScrollBarPosi:\n"
    ".byte 0x00,0x00,0x0c,0x43,0x00,0x00,0x8c,0xc2,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x3f\n"
    ".globl _Q210JiminyMemo4Menu$NoteSeqData\n"
    "_Q210JiminyMemo4Menu$NoteSeqData:\n"
    ".byte 0x70,0x00,0x00,0x00,0x71,0x00,0x00,0x00,0x72,0x00,0x00,0x00,0xfa,0x00,0x00,0x00\n"
    ".byte 0xef,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo4Menu$TalkSeqData\n"
    "_Q210JiminyMemo4Menu$TalkSeqData:\n"
    ".byte 0x88,0x00,0x00,0x00,0x89,0x00,0x00,0x00,0x8a,0x00,0x00,0x00,0xfb,0x00,0x00,0x00\n"
    ".byte 0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo4Menu$CursorLightSeqTbl\n"
    "_Q210JiminyMemo4Menu$CursorLightSeqTbl:\n"
    ".byte 0x82,0x00,0x00,0x00,0x83,0x00,0x00,0x00,0x84,0x00,0x00,0x00,0x82,0x00,0x00,0x00\n"
    ".byte 0x84,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo4Menu$JiminyPosi\n"
    "_Q210JiminyMemo4Menu$JiminyPosi:\n"
    ".byte 0x33,0x33,0x33,0xbf,0xcd,0xcc,0x4c,0xbe,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x3f\n"
    ".globl _Q210JiminyMemo4Menu$CursorPosiOffset\n"
    "_Q210JiminyMemo4Menu$CursorPosiOffset:\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo4Menu$NewMarkPosiOffset\n"
    "_Q210JiminyMemo4Menu$NewMarkPosiOffset:\n"
    ".byte 0x00,0x00,0xa0,0xc0,0x00,0x00,0xa0,0x40\n"
    ".globl _Q210JiminyMemo4Base$LayoutData\n"
    "_Q210JiminyMemo4Base$LayoutData:\n"
    ".byte 0x04,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo4Base$SeqData\n"
    "_Q210JiminyMemo4Base$SeqData:\n"
    ".byte 0xde,0x00,0x00,0x00,0xdf,0x00,0x00,0x00,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x25,0x00,0x00,0x00\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x11,0x00,0x00,0x00,0x01,0x02,0x70,0x00,0x82,0x00,0x00,0x00,0x83,0x00,0x00,0x00\n"
    ".byte 0x84,0x00,0x00,0x00,0x00,0x00,0x16,0x43,0x00,0x00,0x48,0x42,0x12,0x00,0x00,0x00\n"
    ".byte 0x00,0x00,0x00,0x00,0xe1,0x00,0x00,0x00,0xe2,0x00,0x00,0x00,0xe3,0x00,0x00,0x00\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x25,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x02,0x02,0x70,0x00,0x82,0x00,0x00,0x00\n"
    ".byte 0x83,0x00,0x00,0x00,0x84,0x00,0x00,0x00,0x00,0x00,0xaf,0x43,0x00,0x00,0x48,0x42\n"
    ".byte 0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xdb,0x00,0x00,0x00,0xdc,0x00,0x00,0x00\n"
    ".byte 0xdd,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x00\n"
    ".byte 0x00,0x00,0x00,0x00,0x25,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x03,0x02,0x70,0x00\n"
    ".byte 0xd4,0x00,0x00,0x00,0xd5,0x00,0x00,0x00,0xd6,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xd1,0x00,0x00,0x00\n"
    ".byte 0xd2,0x00,0x00,0x00,0xd3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".byte 0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x85,0x00,0x00,0x00,0x85,0x00,0x00,0x00\n"
    ".byte 0x85,0x00,0x00,0x00,0x00,0x00,0x60,0x42,0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x00\n"
    ".byte 0x00,0x00,0x00,0x00,0x07,0x01,0x00,0x00,0x07,0x01,0x00,0x00,0x07,0x01,0x00,0x00\n"
    ".byte 0x00,0x00,0xf4,0x42,0x00,0x80,0xb7,0x43,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo4Base$TelopMessagePosition\n"
    "_Q210JiminyMemo4Base$TelopMessagePosition:\n"
    ".byte 0x00,0x00,0x70,0x42,0x00,0x80,0xbb,0x43,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    ".globl _Q210JiminyMemo4Base$L1R1DigitNumOffset\n"
    "_Q210JiminyMemo4Base$L1R1DigitNumOffset:\n"
    ".byte 0x00,0x00,0xc0,0x41,0x00,0x00,0x80,0xc1\n"
    ".globl _Q210JiminyMemo4Base$L1R1DigitSlashOffset\n"
    "_Q210JiminyMemo4Base$L1R1DigitSlashOffset:\n"
    ".byte 0x00,0x00,0x8c,0x42,0x00,0x00,0x80,0xc1\n"
    ".globl _Q210JiminyMemo4Base$L1R1DigitDenomiOffset\n"
    "_Q210JiminyMemo4Base$L1R1DigitDenomiOffset:\n"
    ".byte 0x00,0x00,0xa8,0x42,0x00,0x00,0x80,0xc1,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
);
// SOURCE DATA: .data .rodata
