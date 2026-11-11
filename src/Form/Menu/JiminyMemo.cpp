#define FORM_MENU_JIMINYMEMO_CPP /* MATCH: unit gate (its header gives this unit its own parts); without it the unit compiles only against its full header (tested 2026-11-02) */
#include "khtypes.h"
#include "Form/Menu/Menu.h"
#include "System/TaskMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/Graphic/LightUnit.h"
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














#include "System/RsrcDB.h"









namespace JiminyMemo {
INT getHostGroupId();
INT getHostRsrcId();
INT getCT0014Code(USHORT inLow);
void playSE(INT inSENo);
BOOL is100AcreGoodEnding();
CHAR* getOpenCloseStringData(INT inCTNo, BOOL inIsFlag);
}

class CLightUnit;
class CMemo3DObj : public CTask {
public:
    CMemo3DObj(CTask* p, INT inGroupId, INT inRsrcId, UINT inDispLayer, BOOL inLoadEnable);
    virtual ~CMemo3DObj();
    virtual void main();
    virtual void disp();
    void setRotateY(FLOAT inRotY);
    BOOL isErase() { return m_bEraseModel; }
    BOOL isLoadEnable() { return m_bLoadEnable; }
    BOOL isLightOff() { return m_bLightOff; }
    void setErase() { m_bEraseModel = TRUE; }
    FLOAT getRotateY() { return m_fRotY; }
    FVECTOR& getPosition() { return m_vPosition; }
    void setScale(FLOAT inScale) { m_fScale = inScale; }
    FLOAT getScale() { return m_fScale; }
    void setLightOff(BOOL inFlag) { m_bLightOff = inFlag; }
    void setFadeCountMax(FLOAT inMax) { m_fFadeCountMax = inMax; }
    void setNotSemiFlag(BOOL inFlag) { m_bNotSemiFlag = inFlag; }
    void setFade2PathFlag(BOOL inFlag) { m_bFade2PathFlag = inFlag; }
    void setPosition(const FVECTOR& inPosition);
    void calcLocalMatrix();

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
    INT isStateFlag(INT inFlags) { return m_uiStateFlags & inFlags; }
    BOOL isFontSeqTbl(INT inSeqNo) { return m_pSeqNoTbl[inSeqNo].CTCode != 0; }
    INT getSeqCtrlNum() { return m_iSeqCtrlNum; }
    INT getSeqNum() { return m_iSeqNum; }
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
    void setBasePosition(const FVECTOR& inPosi) { m_vBasePosition.set(inPosi); }
    FVECTOR& getBasePosition() { return m_vBasePosition; }
    void setShiftPosition(const FVECTOR& inShift) { m_vShiftPosition.set(inShift); }
    void setAimPosition(const FVECTOR& inAim) { m_vAimPosition.set(inAim); }
    void setPosition(const FVECTOR& inPosi) {
        m_vPosition.set(inPosi);
        setPos(m_vPosition);
    }
    void setLocalPosition(const FVECTOR& inPosi) {
        m_vPosition.set(inPosi);
        setLocalPos(m_vPosition);
    }
    void setState(INT inState) { m_iState = inState; }
    void setStateFlag(USHORT inOrFlag, USHORT inAndFlag) { m_iStateFlag = (m_iStateFlag & ~inAndFlag) | inOrFlag; }
    USHORT isStateFlag(USHORT inFlag) { return m_iStateFlag & inFlag; }
    void setMoveTimer(FLOAT inTimeQ) { m_fMoveTimer = inTimeQ; }
    FLOAT getMoveTimer() { return m_fMoveTimer; }
    void addMoveTimer(FLOAT inTimeQ) { m_fMoveTimer += inTimeQ; }
    static INT getSideInId(INT inId) { return inId == 1 ? 0 : 3; }
    static INT getSideOutId(INT inId) { return inId == 1 ? 2 : 4; }
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
    void setShiftVector(const FVECTOR& inShiftPosi) { m_vShiftPosi.set(inShiftPosi); }
    FVECTOR& getShiftVector() { return m_vShiftPosi; }
    FVECTOR& getAimVector() { return m_vAimPosi; }
    FVECTOR& getPosition() { return m_vPosi; }
    void setSeqNo(INT inSeqNo) { m_iLadTbl[3] = inSeqNo; }
    CursorPosition* getLimitPosi(INT inId);
    FLOAT getFrameRateF();
    void setReplaceSeq(INT inSeq);
    void setState(INT inState);
};

namespace JiminyMemo {
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
    void setBasePosition(const FVECTOR& inPosition) { m_vBasePosition.set(inPosition); }
    FVECTOR& getBasePosition() { return m_vBasePosition; }
    void setShiftPosition(const FVECTOR& inShift) { m_vShiftPosition.set(inShift); }
    FVECTOR& getShiftPosition() { return m_vShiftPosition; }
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
/* stand-in for direct work-area access (offsets into the content work areas) */
#define work(inOfs) (*(INT*)((UBYTE*)&m_ContentCommon + ((inOfs) - 0xbc)))
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

    void setStateFlag(INT inOn, INT inOff) { m_uiStateFlags = (m_uiStateFlags & ~inOff) | inOn; }
    INT isStateFlag(INT inFlags) { return m_uiStateFlags & inFlags; }
    BOOL isModeJiminyMemo() { return m_iPlayerMode == 0; }
    BOOL isModeDReport() { return m_iPlayerMode != 0; }
    INT getLoadResourceIndex() { return m_iPlayerMode != 0; }
    void setContentStep(INT inStep) { m_ContentCommon.iStep = inStep; }
    INT getContentStep() { return m_ContentCommon.iStep; }
    void setContentTimer(FLOAT inTimer) { m_ContentCommon.fTimer = inTimer; }
    FLOAT getContentTimer() { return m_ContentCommon.fTimer; }
    void setNextContentMode(INT inMode) { m_ContentCommon.iNextContent = inMode; }
    INT getNextContentMode() { return m_ContentCommon.iNextContent; }
    void setPrevContentMode(INT inMode) { m_ContentCommon.iPrevContent = inMode; }
    INT getPrevContentMode() { return m_ContentCommon.iPrevContent; }
    void setContentParam(INT inId, INT inParam) {
        _jiminyMemoAssertParamId(inId, 1185);
        m_ContentCommon.param[inId] = inParam;
    }
    INT getContentParam(INT inId) {
        _jiminyMemoAssertParamId(inId, 1193);
        return m_ContentCommon.param[inId];
    }
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
    void replaceNoteListString(D2::C2DFnt* inList, const CHAR* inString);
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
/* The original's file-scope const tables (and the two dynamically initialized floats): gcc emits them after
   the vtables at the end of .rodata, the global namespace first, then JiminyMemo's own, then its nested
   namespaces in reverse creation order; the namespaces are opened here in their original order. */
#line 1084
namespace JiminyMemo {
static CHAR* const StoryLadName[13] = {
    "01ca", "05tr", "06wo", "07ol", "08ag", "09ha", "10mo", "11at", "12ne", "13ho", "14po", "15tw", "16de"
};
static const INT RsrcId[2] = {
    110, 112
};
static const USHORT NoteScrollBarTbl[3] = {
    233, 234, 235
};
static const INT CursorSeqTbl[4] = {
    36, 37, 73, 128
};
static const SHORT CardAttackTbl[23] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23, 24
};
static const SHORT CardMagicTbl[7] = {
    26, 27, 28, 29, 30, 31, 32
};
static const SHORT CardSummonsTbl[7] = {
    35, 36, 37, 38, 39, 40, 41
};
static const SHORT CardItemTbl[7] = {
    42, 43, 44, 45, 46, 47, 48
};
static const SHORT CardFriendTbl[8] = {
    49, 50, 51, 52, 53, 54, 55, 57
};
static const SHORT CardEnemyTbl[56] = {
    68, 69, 75, 76, 70, 79, 82, 78, 87, 71, 72, 73, 74, 92, 77, 80, 85, 88, 86, 81, 84, 83, 91, 90, 94, 95, 96,
    97, 98, 93, 99, 104, 101, 105, 107, 106, 100, 108, 109, 102, 110, 103, 116, 118, 119, 120, 113, 115, 117,
    121, 111, 122, 123, 114, 112, 124
};
static const SHORT CardMapTbl[29] = {
    161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181,
    182, 183, 184, 189, 186, 187, 188, 185
};
static const SHORT CardWorldTbl[12] = {
    190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201
};
static const SHORT CardGimmickTbl[1] = {
    58
};
static const SHORT CardSpecialTbl[2] = {
    125, 126
};
static const SHORT RikuCardBattleTbl[5] = {
    18, 42, 43, 56, 58
};
static const SHORT RikuCardEnemyTbl[22] = {
    68, 70, 75, 79, 81, 82, 85, 87, 90, 89, 99, 100, 101, 102, 104, 105, 106, 107, 108, 109, 115, 117
};
static const SHORT RikuCardMapTbl[20] = {
    161, 162, 163, 164, 165, 166, 169, 170, 171, 172, 175, 176, 177, 178, 182, 183, 186, 187, 188, 185
};
static const SHORT RikuCardWorldTbl[12] = {
    190, 191, 192, 193, 194, 195, 196, 197, 198, 200, 201, 202
};
static const USHORT CardNameAttackTbl[23] = {
    2049, 2050, 2051, 2052, 2053, 2054, 2055, 2056, 2057, 2058, 2059, 2060, 2061, 2062, 2063, 2064, 2065, 2066,
    2067, 2068, 2069, 2070, 2071
};
static const USHORT CardNameMagicTbl[7] = {
    2305, 2306, 2307, 2308, 2309, 2310, 2311
};
static const USHORT CardNameSummonsTbl[7] = {
    2312, 2313, 2314, 2315, 2316, 2317, 2318
};
static const USHORT CardNameItemTbl[7] = {
    2561, 2562, 2563, 2564, 2565, 2566, 2567
};
static const USHORT CardNameFriendTbl[8] = {
    2817, 2818, 2819, 2820, 2821, 2822, 2823, 2824
};
static const USHORT CardNameEnemyTbl[56] = {
    3073, 3074, 3080, 3081, 3075, 3084, 3087, 3083, 3092, 3076, 3077, 3078, 3079, 3096, 3082, 3085, 3090, 3093,
    3091, 3086, 3089, 3088, 3095, 3094, 3098, 3099, 3100, 3101, 3102, 3097, 3103, 3108, 3105, 3109, 3111, 3110,
    3104, 3112, 3113, 3106, 3114, 3107, 3120, 3122, 3123, 3124, 3117, 3119, 3121, 3125, 3115, 3126, 3127, 3118,
    3116, 3128
};
static const USHORT CardNameMapTbl[29] = {
    3329, 3330, 3331, 3332, 3333, 3334, 3335, 3336, 3337, 3338, 3339, 3340, 3341, 3342, 3343, 3344, 3345, 3346,
    3347, 3348, 3349, 3350, 3351, 3352, 3357, 3354, 3355, 3356, 3353
};
static const USHORT CardNameWorldTbl[12] = {
    3585, 3586, 3587, 3588, 3589, 3590, 3591, 3592, 3593, 3594, 3595, 3596
};
static const USHORT CardNameGimmickTbl[1] = {
    3841
};
static const USHORT CardNameSpecialTbl[2] = {
    4097, 4098
};
static const USHORT RikuCardNameBattleTbl[5] = {
    4353, 2561, 2562, 4356, 3841
};
static const USHORT RikuCardNameEnemyTbl[22] = {
    3073, 3075, 3080, 3084, 3086, 3087, 3090, 3092, 3094, 3130, 3103, 3104, 3105, 3106, 3108, 3109, 3110, 3111,
    3112, 3113, 3119, 3121
};
static const USHORT RikuCardNameMapTbl[20] = {
    3329, 3330, 3331, 3332, 3333, 3334, 3337, 3338, 3339, 3340, 3343, 3344, 3345, 3346, 3350, 3351, 3354, 3355,
    3356, 3353
};
static const USHORT RikuCardNameWorldTbl[12] = {
    3585, 3586, 3587, 3588, 3589, 3590, 3591, 3592, 3593, 3595, 3596, 3598
};
static const USHORT CardManualAttackTbl[23] = {
    4609, 4610, 4611, 4612, 4613, 4614, 4615, 4616, 4617, 4618, 4619, 4620, 4621, 4622, 4623, 4624, 4625, 4626,
    4627, 4628, 4629, 4630, 4631
};
static const USHORT CardManualMagicTbl[7] = {
    4865, 4866, 4867, 4868, 4869, 4870, 4871
};
static const USHORT CardManualSummonsTbl[7] = {
    4872, 4873, 4874, 4875, 4876, 4877, 4878
};
static const USHORT CardManualItemTbl[7] = {
    5121, 5122, 5123, 5124, 5125, 5126, 5127
};
static const USHORT CardManualFriendTbl[8] = {
    5377, 5378, 5379, 5380, 5381, 5382, 5383, 5384
};
static const USHORT CardManualEnemyTbl[56] = {
    5633, 5634, 5640, 5641, 5635, 5644, 5647, 5643, 5652, 5636, 5637, 5638, 5639, 5656, 5642, 5645, 5650, 5653,
    5651, 5646, 5649, 5648, 5655, 5654, 5658, 5659, 5660, 5661, 5662, 5657, 5663, 5668, 5665, 5669, 5671, 5670,
    5664, 5672, 5673, 5666, 5674, 5667, 5680, 5682, 5683, 5684, 5677, 5679, 5681, 5685, 5675, 5686, 5687, 5678,
    5676, 5688
};
static const USHORT CardManualMapTbl[29] = {
    5889, 5890, 5891, 5892, 5893, 5894, 5895, 5896, 5897, 5898, 5899, 5900, 5901, 5902, 5903, 5904, 5905, 5906,
    5907, 5908, 5909, 5910, 5911, 5912, 5917, 5914, 5915, 5916, 5913
};
static const USHORT CardManualWorldTbl[12] = {
    6145, 6146, 6147, 6148, 6149, 6150, 6151, 6152, 6153, 6154, 6155, 6156
};
static const USHORT CardManualGimmickTbl[1] = {
    6401
};
static const USHORT CardManualSpecialTbl[2] = {
    6657, 6658
};
static const USHORT RikuCardManualBattleTbl[5] = {
    6913, 5121, 5122, 6914, 6401
};
static const USHORT RikuCardManualEnemyTbl[22] = {
    5633, 5635, 5640, 5644, 5646, 5647, 5650, 5652, 5654, 5689, 5663, 5664, 5665, 5666, 5668, 5669, 5670, 5671,
    5672, 5673, 5679, 5681
};
static const USHORT RikuCardManualMapTbl[20] = {
    5889, 5890, 5891, 5892, 5893, 5894, 5897, 5898, 5899, 5900, 5903, 5904, 5905, 5906, 5910, 5911, 5914, 5915,
    5916, 5913
};
static const USHORT RikuCardManualWorldTbl[12] = {
    6915, 6916, 6917, 6918, 6919, 6920, 6921, 6922, 6923, 6924, 6925, 6926
};
static const INT SoraCardNumTbl[10] = {
    23, 7, 7, 7, 8, 56, 29, 12, 1, 2
};
static const INT RikuCardNumTbl[4] = {
    5, 22, 20, 12
};
static const ILLUST_TEX_DATA CharaIllustMain1Tbl[7] = {
    { 100, 2 }, { 100, 0 }, { 100, 1 }, { 27, 1 }, { 8, 5 }, { 27, 3 }, { 27, 4 }
};
static const ILLUST_TEX_DATA CharaIllustMain2Tbl[6] = {
    { 31, 6 }, { 14, 3 }, { 14, 6 }, { 14, 4 }, { 14, 7 }, { 14, 5 }
};
static const ILLUST_TEX_DATA CharaIllustEtcTbl[5] = {
    { 5, 7 }, { 6, 2 }, { 6, 1 }, { 6, 4 }, { 27, 2 }
};
static const ILLUST_TEX_DATA CharaIllustTRTbl[4] = {
    { 28, 0 }, { 28, 1 }, { 27, 5 }, { 27, 6 }
};
static const ILLUST_TEX_DATA CharaIllustWOTbl[6] = {
    { 28, 2 }, { 28, 3 }, { 28, 4 }, { 13, 4 }, { 15, 2 }, { 28, 5 }
};
static const ILLUST_TEX_DATA CharaIllustOLTbl[4] = {
    { 30, 0 }, { 29, 7 }, { 13, 5 }, { 6, 5 }
};
static const ILLUST_TEX_DATA CharaIllustAGTbl[6] = {
    { 7, 7 }, { 6, 0 }, { 29, 4 }, { 29, 6 }, { 29, 5 }, { 13, 6 }
};
static const ILLUST_TEX_DATA CharaIllustHATbl[7] = {
    { 100, 6 }, { 100, 4 }, { 100, 5 }, { 8, 1 }, { 29, 2 }, { 29, 3 }, { 13, 7 }
};
static const ILLUST_TEX_DATA CharaIllustMOTbl[2] = {
    { 28, 6 }, { 28, 7 }
};
static const ILLUST_TEX_DATA CharaIllustATTbl[4] = {
    { 8, 0 }, { 29, 0 }, { 29, 1 }, { 14, 0 }
};
static const ILLUST_TEX_DATA CharaIllustNETbl[4] = {
    { 8, 2 }, { 6, 3 }, { 30, 1 }, { 14, 1 }
};
static const ILLUST_TEX_DATA CharaIllustHOTbl[4] = {
    { 8, 3 }, { 30, 2 }, { 30, 3 }, { 14, 2 }
};
static const ILLUST_TEX_DATA CharaIllustPOTbl[7] = {
    { 30, 7 }, { 31, 1 }, { 31, 4 }, { 31, 5 }, { 31, 2 }, { 31, 0 }, { 31, 3 }
};
static const ILLUST_TEX_DATA CharaIllustDETbl[3] = {
    { 30, 4 }, { 30, 5 }, { 30, 6 }
};
static const ILLUST_TEX_DATA CharaIllustHeartLessTbl[35] = {
    { 9, 1 }, { 9, 2 }, { 9, 3 }, { 9, 4 }, { 9, 5 }, { 9, 6 }, { 9, 7 }, { 10, 0 }, { 10, 1 }, { 10, 2 },
    { 10, 3 }, { 10, 4 }, { 10, 5 }, { 10, 6 }, { 10, 7 }, { 11, 0 }, { 11, 1 }, { 11, 2 }, { 11, 3 },
    { 11, 4 }, { 11, 5 }, { 11, 6 }, { 11, 7 }, { 12, 0 }, { 12, 1 }, { 12, 2 }, { 12, 3 }, { 12, 4 },
    { 12, 5 }, { 12, 6 }, { 12, 7 }, { 13, 0 }, { 13, 1 }, { 13, 2 }, { 13, 3 }
};
static const ILLUST_TEX_DATA CharaIllustRiku1Tbl[16] = {
    { 100, 0 }, { 100, 4 }, { 100, 1 }, { 100, 5 }, { 27, 0 }, { 27, 4 }, { 31, 6 }, { 14, 3 }, { 31, 7 },
    { 15, 1 }, { 14, 6 }, { 15, 0 }, { 15, 3 }, { 14, 4 }, { 14, 7 }, { 14, 5 }
};
static const ILLUST_TEX_DATA CharaIllustRiku2Tbl[6] = {
    { 30, 3 }, { 13, 6 }, { 14, 0 }, { 13, 5 }, { 13, 7 }, { 14, 1 }
};
static const ILLUST_TEX_DATA CharaIllustRikuHeartLessTbl[34] = {
    { 9, 1 }, { 9, 2 }, { 9, 3 }, { 9, 4 }, { 9, 5 }, { 9, 6 }, { 9, 7 }, { 10, 0 }, { 10, 1 }, { 10, 2 },
    { 10, 3 }, { 10, 4 }, { 10, 5 }, { 10, 6 }, { 10, 7 }, { 11, 0 }, { 11, 1 }, { 11, 2 }, { 11, 3 },
    { 11, 4 }, { 11, 5 }, { 11, 6 }, { 11, 7 }, { 12, 0 }, { 12, 1 }, { 12, 2 }, { 12, 4 }, { 12, 5 },
    { 12, 6 }, { 12, 7 }, { 13, 0 }, { 13, 1 }, { 13, 2 }, { 13, 3 }
};
static const USHORT CharaNameMain1Tbl[7] = {
    7425, 7426, 7427, 7428, 7438, 7429, 7430
};
static const USHORT CharaNameMain2Tbl[6] = {
    7431, 7432, 7435, 7433, 7436, 7434
};
static const USHORT CharaNameEtcTbl[5] = {
    7681, 7682, 7683, 7684, 7437
};
static const USHORT CharaNameTRTbl[4] = {
    7937, 7939, 7938, 7940
};
static const USHORT CharaNameWOTbl[6] = {
    8193, 8194, 8195, 8196, 8197, 8198
};
static const USHORT CharaNameOLTbl[4] = {
    8449, 8450, 8451, 8452
};
static const USHORT CharaNameAGTbl[6] = {
    8705, 8706, 8707, 8708, 8709, 8710
};
static const USHORT CharaNameHATbl[7] = {
    7425, 7426, 7427, 8961, 8962, 8963, 8964
};
static const USHORT CharaNameMOTbl[2] = {
    9217, 9218
};
static const USHORT CharaNameATTbl[4] = {
    9473, 9474, 9475, 9476
};
static const USHORT CharaNameNETbl[4] = {
    9729, 9730, 9731, 9732
};
static const USHORT CharaNameHOTbl[4] = {
    9985, 9986, 9987, 9988
};
static const USHORT CharaNamePOTbl[7] = {
    10241, 10242, 10243, 10244, 10245, 10246, 10247
};
static const USHORT CharaNameDETbl[3] = {
    10497, 10498, 10499
};
static const USHORT CharaNameHeartLessTbl[35] = {
    10753, 10754, 10755, 10756, 10757, 10758, 10759, 10760, 10761, 10762, 10763, 10764, 10765, 10766, 10767,
    10768, 10769, 10770, 10771, 10772, 10773, 10774, 10775, 10776, 10777, 10778, 10779, 10780, 10781, 10782,
    10783, 10784, 10785, 10786, 10787
};
static const USHORT CharaNameRiku1Tbl[16] = {
    15105, 15106, 15107, 15108, 15109, 15110, 15111, 15112, 15120, 15113, 15114, 15115, 15116, 15117, 15118,
    15119
};
static const USHORT CharaNameRiku2Tbl[6] = {
    15361, 15362, 15363, 15364, 15365, 15366
};
static const USHORT CharaNameRikuHeartLessTbl[34] = {
    10753, 10754, 10755, 10756, 10757, 10758, 10759, 10760, 10761, 10762, 10763, 10764, 10765, 10766, 10767,
    10768, 10769, 10770, 10771, 10772, 10773, 10774, 10775, 10776, 10777, 10778, 10780, 10781, 10782, 10783,
    10784, 10785, 10786, 10787
};
static const USHORT CharaManualMain1Tbl[7] = {
    11521, 11522, 11523, 11524, 11534, 11525, 11526
};
static const USHORT CharaManualMain2Tbl[6] = {
    11527, 11528, 11531, 11529, 11532, 11530
};
static const USHORT CharaManualEtcTbl[5] = {
    11777, 11778, 11779, 11780, 11533
};
static const USHORT CharaManualTRTbl[4] = {
    12033, 12035, 12034, 12036
};
static const USHORT CharaManualWOTbl[6] = {
    12289, 12290, 12291, 12292, 12293, 12294
};
static const USHORT CharaManualOLTbl[4] = {
    12545, 12546, 12547, 12548
};
static const USHORT CharaManualAGTbl[6] = {
    12801, 12802, 12803, 12804, 12805, 12806
};
static const USHORT CharaManualHATbl[7] = {
    11521, 11522, 11523, 13057, 13058, 13059, 13060
};
static const USHORT CharaManualMOTbl[2] = {
    13313, 13314
};
static const USHORT CharaManualATTbl[4] = {
    13569, 13570, 13571, 13572
};
static const USHORT CharaManualNETbl[4] = {
    13825, 13826, 13827, 13828
};
static const USHORT CharaManualHOTbl[4] = {
    14081, 14082, 14083, 14084
};
static const USHORT CharaManualPOTbl[7] = {
    14337, 14338, 14339, 14340, 14341, 14342, 14343
};
static const USHORT CharaManualDETbl[3] = {
    14593, 14594, 14595
};
static const USHORT CharaManualHeartLessTbl[35] = {
    14849, 14850, 14851, 14852, 14853, 14854, 14855, 14856, 14857, 14858, 14859, 14860, 14861, 14862, 14863,
    14864, 14865, 14866, 14867, 14868, 14869, 14870, 14871, 14872, 14873, 14874, 14875, 14876, 14877, 14878,
    14879, 14880, 14881, 14882, 14883
};
static const USHORT CharaManualRiku1Tbl[16] = {
    15617, 15631, 15618, 15632, 15619, 15620, 15621, 15622, 15630, 15623, 15624, 15625, 15626, 15627, 15628,
    15629
};
static const USHORT CharaManualRiku2Tbl[6] = {
    15873, 15874, 15875, 15876, 15877, 15878
};
static const USHORT CharaManualRikuHeartLessTbl[34] = {
    14849, 14850, 14851, 14852, 14853, 14854, 14855, 14856, 14857, 14858, 14859, 14860, 14861, 14862, 14863,
    14864, 14865, 14866, 14867, 14868, 14869, 14870, 14871, 14872, 14873, 14874, 14876, 14877, 14878, 14879,
    16129, 16130, 16131, 16132
};
static const INT CharaNumTbl[15] = {
    7, 6, 5, 4, 6, 4, 6, 7, 2, 4, 4, 4, 7, 3, 35
};
static const INT RikuCharaNumTbl[3] = {
    16, 6, 34
};
static const MODEL_VIEW_DATA CharaModelMain1Tbl[7] = {
    { 51153, 371, -38, 180 }, { 51154, 422, -25, 180 }, { 51155, 332, -38, 180 }, { 51156, 1792, -51, 180 },
    { 51263, 460, -12, 180 }, { 51157, 371, -38, 180 }, { 51158, 409, -38, 180 }
};
static const MODEL_VIEW_DATA CharaModelMain2Tbl[6] = {
    { 51159, 409, -38, 180 }, { 51160, 371, -51, 180 }, { 51163, 268, -38, 180 }, { 51161, 286, -51, 180 },
    { 51164, 302, -64, 180 }, { 51162, 294, -38, 180 }
};
static const MODEL_VIEW_DATA CharaModelEtcTbl[5] = {
    { 51165, 307, -25, 180 }, { 51166, 409, -38, 180 }, { 51167, 486, -38, 180 }, { 51168, 870, -51, 180 },
    { 51169, 512, -38, 180 }
};
static const MODEL_VIEW_DATA CharaModelTRTbl[4] = {
    { 51170, 320, -51, 180 }, { 51171, 345, -51, 180 }, { 51172, 320, -51, 180 }, { 51173, 332, -51, 180 }
};
static const MODEL_VIEW_DATA CharaModelWOTbl[6] = {
    { 51174, 473, -51, 180 }, { 51175, 294, -25, 180 }, { 51176, 576, -38, 180 }, { 51177, 256, -51, 180 },
    { 51178, 256, -51, 180 }, { 51179, 588, 38, 180 }
};
static const MODEL_VIEW_DATA CharaModelOLTbl[4] = {
    { 51180, 256, -51, 180 }, { 51181, 409, -51, 180 }, { 51182, 217, -46, 180 }, { 51183, 320, -51, 180 }
};
static const MODEL_VIEW_DATA CharaModelAGTbl[6] = {
    { 51184, 307, -46, 180 }, { 51185, 158, -194, 180 }, { 51186, 345, -64, 180 }, { 51187, 844, -38, 180 },
    { 51188, 256, -51, 180 }, { 51189, 76, 115, 180 }
};
static const MODEL_VIEW_DATA CharaModelHATbl[7] = {
    { 51190, 371, -38, 180 }, { 51191, 422, -25, 180 }, { 51192, 332, -38, 180 }, { 51193, 286, -64, 180 },
    { 51194, 320, -58, 180 }, { 51195, 422, -64, 180 }, { 51196, 248, -51, 180 }
};
static const MODEL_VIEW_DATA CharaModelMOTbl[2] = {
    { 51197, 486, -38, 180 }, { 51198, 332, -51, 180 }
};
static const MODEL_VIEW_DATA CharaModelATTbl[4] = {
    { 51199, 345, -25, 180 }, { 51200, 1280, 51, 180 }, { 51201, 640, 38, 180 }, { 51202, 256, -89, 180 }
};
static const MODEL_VIEW_DATA CharaModelNETbl[4] = {
    { 51203, 345, -51, 180 }, { 51204, 2048, -12, 180 }, { 51205, 358, -64, 180 }, { 51206, 294, -76, 180 }
};
static const MODEL_VIEW_DATA CharaModelHOTbl[4] = {
    { 51207, 273, -38, 180 }, { 51208, 307, -51, 180 }, { 51209, 268, -51, 180 }, { 51210, 61, 0, 180 }
};
static const MODEL_VIEW_DATA CharaModelPOTbl[7] = {
    { 51211, 512, -64, 180 }, { 51212, 1152, -38, 180 }, { 51213, 486, -38, 180 }, { 51214, 921, -38, 180 },
    { 51215, 512, -25, 180 }, { 51216, 563, -51, 180 }, { 51217, 486, -38, 180 }
};
static const MODEL_VIEW_DATA CharaModelDETbl[3] = {
    { 51218, 371, -51, 180 }, { 51219, 332, -51, 180 }, { 51220, 371, -51, 180 }
};
static const MODEL_VIEW_DATA CharaModelHeartLessTbl[35] = {
    { 51221, 268, -25, 180 }, { 51222, 281, -25, 180 }, { 51223, 230, -25, 180 }, { 51224, 384, 0, 180 },
    { 51225, 384, 0, 180 }, { 51226, 384, 0, 180 }, { 51227, 384, 0, 180 }, { 51228, 320, -25, 180 },
    { 51229, 332, -25, 180 }, { 51230, 320, 0, 180 }, { 51231, 281, -25, 180 }, { 51232, 230, -38, 180 },
    { 51233, 256, 25, 180 }, { 51234, 307, 0, 180 }, { 18467, 345, 0, 180 }, { 51236, 281, 0, 180 },
    { 51237, 140, -140, 180 }, { 51238, 281, -38, 180 }, { 51239, 294, -25, 180 }, { 51240, 307, -25, 180 },
    { 51241, 281, -76, 180 }, { 51242, 268, -179, 180 }, { 51243, 256, -38, 180 }, { 51244, 268, -51, 180 },
    { 51245, 332, -64, 180 }, { 51246, 307, -12, 180 }, { 51247, 332, -25, 180 }, { 51248, 332, -25, 180 },
    { 51249, 268, 0, 180 }, { 51250, 332, -25, 180 }, { 51251, 281, 0, 180 }, { 51252, 153, -76, 180 },
    { 51253, 102, -25, 180 }, { 51254, 115, -76, 180 }, { 51255, 69, -115, 180 }
};
static const MODEL_VIEW_DATA CharaModelRiku1Tbl[16] = {
    { 51157, 371, -38, 180 }, { 51256, 371, -38, 180 }, { 51257, 460, -38, 180 }, { 51258, 460, -38, 180 },
    { 51153, 371, -38, 180 }, { 51158, 409, -38, 180 }, { 51159, 409, -38, 180 }, { 51160, 371, -51, 180 },
    { 51262, 307, -38, 180 }, { 51259, 294, -51, 180 }, { 51163, 268, -38, 180 }, { 51260, 268, -51, 180 },
    { 51261, 286, -51, 180 }, { 51161, 294, -51, 180 }, { 51164, 302, -64, 180 }, { 51162, 294, -38, 180 }
};
static const MODEL_VIEW_DATA CharaModelRiku2Tbl[6] = {
    { 51209, 268, -51, 180 }, { 51189, 76, 115, 180 }, { 51202, 256, -89, 180 }, { 51182, 217, -46, 180 },
    { 51196, 248, -51, 180 }, { 51206, 294, -76, 180 }
};
static const MODEL_VIEW_DATA CharaModelRikuHeartLessTbl[34] = {
    { 51221, 268, -25, 180 }, { 51222, 281, -25, 180 }, { 51223, 230, -25, 180 }, { 51224, 384, 0, 180 },
    { 51225, 384, 0, 180 }, { 51226, 384, 0, 180 }, { 51227, 384, 0, 180 }, { 51228, 320, -25, 180 },
    { 51229, 332, -25, 180 }, { 51230, 320, 0, 180 }, { 51231, 281, -25, 180 }, { 51232, 230, -38, 180 },
    { 51233, 256, 25, 180 }, { 51234, 307, 0, 180 }, { 2083, 345, 0, 180 }, { 51236, 281, 0, 180 },
    { 51237, 140, -140, 180 }, { 51238, 281, -38, 180 }, { 51239, 294, -25, 180 }, { 51240, 307, -25, 180 },
    { 51241, 281, -76, 180 }, { 51242, 268, -179, 180 }, { 51243, 256, -38, 180 }, { 51244, 268, -51, 180 },
    { 51245, 332, -64, 180 }, { 51246, 307, -12, 180 }, { 51248, 332, -25, 180 }, { 51249, 268, 0, 180 },
    { 51250, 332, -25, 180 }, { 51251, 281, 0, 180 }, { 51252, 153, -76, 180 }, { 51253, 102, -25, 180 },
    { 51254, 115, -76, 180 }, { 51255, 69, -115, 180 }
};
namespace Base {
static const INT LayoutData[3] = {
    4, 5, 6
};
static const LayoutDataParam SeqData[12] = {
    { 222, 223, 224, 0.0f, 0.0f, 17, 0x0 }, { 37, 0, 0, 0.0f, 0.0f, 17, 0x700201 },
    { 130, 131, 132, 1.5e+02f, 5e+01f, 18, 0x0 }, { 225, 226, 227, 0.0f, 0.0f, 17, 0x0 },
    { 37, 0, 0, 0.0f, 0.0f, 17, 0x700202 }, { 130, 131, 132, 3.5e+02f, 5e+01f, 18, 0x0 },
    { 219, 220, 221, 0.0f, 0.0f, 17, 0x0 }, { 37, 0, 0, 0.0f, 0.0f, 17, 0x700203 },
    { 212, 213, 214, 0.0f, 0.0f, 17, 0x0 }, { 209, 210, 211, 0.0f, 0.0f, 17, 0x0 },
    { 133, 133, 133, 56.0f, 0.0f, 17, 0x0 }, { 263, 263, 263, 122.0f, 367.0f, 17, 0x0 }
};
static const FVECTOR TelopMessagePosition = { { 6e+01f, 375.0f, 0.0f, 0.0f } };
static const CursorPosition L1R1DigitNumOffset = { 24.0f, -16.0f };
static const CursorPosition L1R1DigitSlashOffset = { 7e+01f, -16.0f };
static const CursorPosition L1R1DigitDenomiOffset = { 84.0f, -16.0f };
}
namespace Menu {
static FLOAT JiminyRotY = DEG2RAD(-120.0f);
static const INT NoteSeqData[5] = {
    112, 113, 114, 250, 239
};
static const INT TalkSeqData[5] = {
    136, 137, 138, 251, 240
};
static const INT CursorLightSeqTbl[5] = {
    130, 131, 132, 130, 132
};
static const FVECTOR JiminyPosi = { { -0.7f, -0.2f, 0.0f, 1.0f } };
static const CursorPosition CursorPosiOffset = { 0.0f, 0.0f };
static const CursorPosition NewMarkPosiOffset = { -5.0f, 5.0f };
}
namespace Story {
static const INT NoteSeqData[5] = {
    199, 200, 201, 258, 247
};
static const CursorPosition CursorPosiOffset = { 0.0f, 0.0f };
static const FVECTOR NoteScrollBarPosi = { { 1.4e+02f, -7e+01f, 0.0f, 1.0f } };
}
namespace StoryView {
static const INT NoteSeqData[5] = {
    205, 206, 207, 260, 249
};
static const SHORT WorldPictureTbl[17] = {
    0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0
};
namespace D {
static const SHORT WorldPictureTbl[6] = {
    0, 0, 0, 0, 0, 0
};
}
}
namespace Collect {
static const INT BaseSeqData[5] = {
    183, 184, 185, 254, 243
};
static const INT NoteSeqData[5] = {
    179, 180, 181, 253, 242
};
static const INT LightSeqTbl[5] = {
    130, 238, 132, 131, 131
};
static const INT FrameSeqTbl[5] = {
    123, 124, 125, 122, 0
};
static const INT FlickerSeqTbl[5] = {
    126, 126, 126, 126, 126
};
static const UINT CursorMoveFlag[7] = {
    0x1FFFFE, 0x3FFFFF, 0x3FFFFF, 0x3FFFFF, 0x3FFFFF, 0x3FFFFF, 0x1FFFFE
};
static const UINT CursorMoveFlagWithSpecial[7] = {
    0x1FFFFE, 0x3FFFFF, 0x3FFFFF, 0x3FFFFF, 0x3FFFFF, 0x3FFFFF, 0x3FFFFF
};
static const CursorPosition CursorMapStart = { -704.0f, -336.0f };
static const CursorPosition CursorMapOffset = { 64.0f, 96.0f };
static const CursorPosition CardMapOffset = { 1.2e+02f, 118.0f };
static const INT CardMapSpacingTbl[4] = {
    64, 74, -16, -30
};
namespace D {
static const UINT CursorMoveFlag[3] = {
    0x3FFFFF, 0x3FFFFF, 0x7FFF0
};
static const CursorPosition CursorMapStart = { -704.0f, -144.0f };
static const CursorPosition CardMapOffset = { 1.2e+02f, 118.0f };
}
}
namespace Card {
static const INT NoteSeqData[5] = {
    199, 200, 201, 258, 247
};
static const CursorPosition CursorPosiOffset = { 0.0f, 0.0f };
static const CursorPosition KingMarkPosiOffset = { 5.0f, -3.0f };
static const FVECTOR NoteScrollBarPosi = { { 1.4e+02f, -7e+01f, 0.0f, 1.0f } };
}
namespace CardSelect {
static const INT NoteSeqData[5] = {
    199, 200, 201, 258, 247
};
static const CursorPosition CursorPosiOffset = { 0.0f, 0.0f };
static const FVECTOR NoteScrollBarPosi = { { 1.4e+02f, -7e+01f, 0.0f, 1.0f } };
}
namespace CardView {
static const INT NoteSeqData[5] = {
    176, 177, 178, 258, 247
};
}
namespace Chara {
static const INT NoteSeqData[5] = {
    199, 200, 201, 258, 247
};
static const CursorPosition CursorPosiOffset = { 0.0f, 0.0f };
static const CursorPosition KingMarkPosiOffset = { 5.0f, -3.0f };
static const FVECTOR NoteScrollBarPosi = { { 1.4e+02f, -7e+01f, 0.0f, 1.0f } };
}
namespace CharaSelect {
static const INT NoteSeqData[5] = {
    199, 200, 201, 258, 247
};
static const CursorPosition CursorPosiOffset = { 0.0f, 0.0f };
static const FVECTOR NoteScrollBarPosi = { { 1.4e+02f, -7e+01f, 0.0f, 1.0f } };
}
namespace CharaView {
static FLOAT ModelRotY = DEG2RAD(180.0f);
static const INT NoteSeqData[5] = {
    186, 187, 188, 255, 244
};
static const INT EnemyNoteSeqData[5] = {
    192, 193, 194, 257, 246
};
static const INT DuskyNoteSeqData[5] = {
    189, 190, 191, 256, 245
};
static const FVECTOR ModelPosi = { { 0.0f, -0.1f, 0.0f, 1.0f } };
}
namespace Minigame {
static const INT NoteSeqData[5] = {
    196, 197, 198, 196, 198
};
}
}
namespace JiminyMemo {
namespace Base {
static CHAR* DefaultMessage = "";
}
SHORT* CardEncyclTbl[10] = { (SHORT*)CardAttackTbl, (SHORT*)CardMagicTbl, (SHORT*)CardSummonsTbl, (SHORT*)CardItemTbl, (SHORT*)CardFriendTbl, (SHORT*)CardEnemyTbl, (SHORT*)CardMapTbl, (SHORT*)CardWorldTbl, (SHORT*)CardGimmickTbl, (SHORT*)CardSpecialTbl };
SHORT* RikuCardEncyclTbl[4] = { (SHORT*)RikuCardBattleTbl, (SHORT*)RikuCardEnemyTbl, (SHORT*)RikuCardMapTbl, (SHORT*)RikuCardWorldTbl };
CT_LOWER* CardNameTbl[10] = { (CT_LOWER*)CardNameAttackTbl, (CT_LOWER*)CardNameMagicTbl, (CT_LOWER*)CardNameSummonsTbl, (CT_LOWER*)CardNameItemTbl, (CT_LOWER*)CardNameFriendTbl, (CT_LOWER*)CardNameEnemyTbl, (CT_LOWER*)CardNameMapTbl, (CT_LOWER*)CardNameWorldTbl, (CT_LOWER*)CardNameGimmickTbl, (CT_LOWER*)CardNameSpecialTbl };
CT_LOWER* RikuCardNameTbl[4] = { (CT_LOWER*)RikuCardNameBattleTbl, (CT_LOWER*)RikuCardNameEnemyTbl, (CT_LOWER*)RikuCardNameMapTbl, (CT_LOWER*)RikuCardNameWorldTbl };
CT_LOWER* CardManualTbl[10] = { (CT_LOWER*)CardManualAttackTbl, (CT_LOWER*)CardManualMagicTbl, (CT_LOWER*)CardManualSummonsTbl, (CT_LOWER*)CardManualItemTbl, (CT_LOWER*)CardManualFriendTbl, (CT_LOWER*)CardManualEnemyTbl, (CT_LOWER*)CardManualMapTbl, (CT_LOWER*)CardManualWorldTbl, (CT_LOWER*)CardManualGimmickTbl, (CT_LOWER*)CardManualSpecialTbl };
CT_LOWER* RikuCardManualTbl[4] = { (CT_LOWER*)RikuCardManualBattleTbl, (CT_LOWER*)RikuCardManualEnemyTbl, (CT_LOWER*)RikuCardManualMapTbl, (CT_LOWER*)RikuCardManualWorldTbl };
ILLUST_TEX_DATA* CharaIllustTextureTbl[15] = { (ILLUST_TEX_DATA*)CharaIllustMain1Tbl, (ILLUST_TEX_DATA*)CharaIllustMain2Tbl, (ILLUST_TEX_DATA*)CharaIllustEtcTbl, (ILLUST_TEX_DATA*)CharaIllustTRTbl, (ILLUST_TEX_DATA*)CharaIllustWOTbl, (ILLUST_TEX_DATA*)CharaIllustOLTbl, (ILLUST_TEX_DATA*)CharaIllustAGTbl, (ILLUST_TEX_DATA*)CharaIllustHATbl, (ILLUST_TEX_DATA*)CharaIllustMOTbl, (ILLUST_TEX_DATA*)CharaIllustATTbl, (ILLUST_TEX_DATA*)CharaIllustNETbl, (ILLUST_TEX_DATA*)CharaIllustHOTbl, (ILLUST_TEX_DATA*)CharaIllustPOTbl, (ILLUST_TEX_DATA*)CharaIllustDETbl, (ILLUST_TEX_DATA*)CharaIllustHeartLessTbl };
ILLUST_TEX_DATA* RikuCharaIllustTextureTbl[3] = { (ILLUST_TEX_DATA*)CharaIllustRiku1Tbl, (ILLUST_TEX_DATA*)CharaIllustRiku2Tbl, (ILLUST_TEX_DATA*)CharaIllustRikuHeartLessTbl };
CT_LOWER* CharacterNameTbl[15] = { (CT_LOWER*)CharaNameMain1Tbl, (CT_LOWER*)CharaNameMain2Tbl, (CT_LOWER*)CharaNameEtcTbl, (CT_LOWER*)CharaNameTRTbl, (CT_LOWER*)CharaNameWOTbl, (CT_LOWER*)CharaNameOLTbl, (CT_LOWER*)CharaNameAGTbl, (CT_LOWER*)CharaNameHATbl, (CT_LOWER*)CharaNameMOTbl, (CT_LOWER*)CharaNameATTbl, (CT_LOWER*)CharaNameNETbl, (CT_LOWER*)CharaNameHOTbl, (CT_LOWER*)CharaNamePOTbl, (CT_LOWER*)CharaNameDETbl, (CT_LOWER*)CharaNameHeartLessTbl };
CT_LOWER* RikuCharacterNameTbl[3] = { (CT_LOWER*)CharaNameRiku1Tbl, (CT_LOWER*)CharaNameRiku2Tbl, (CT_LOWER*)CharaNameRikuHeartLessTbl };
CT_LOWER* CharacterManualTbl[15] = { (CT_LOWER*)CharaManualMain1Tbl, (CT_LOWER*)CharaManualMain2Tbl, (CT_LOWER*)CharaManualEtcTbl, (CT_LOWER*)CharaManualTRTbl, (CT_LOWER*)CharaManualWOTbl, (CT_LOWER*)CharaManualOLTbl, (CT_LOWER*)CharaManualAGTbl, (CT_LOWER*)CharaManualHATbl, (CT_LOWER*)CharaManualMOTbl, (CT_LOWER*)CharaManualATTbl, (CT_LOWER*)CharaManualNETbl, (CT_LOWER*)CharaManualHOTbl, (CT_LOWER*)CharaManualPOTbl, (CT_LOWER*)CharaManualDETbl, (CT_LOWER*)CharaManualHeartLessTbl };
CT_LOWER* RikuCharacterManualTbl[3] = { (CT_LOWER*)CharaManualRiku1Tbl, (CT_LOWER*)CharaManualRiku2Tbl, (CT_LOWER*)CharaManualRikuHeartLessTbl };
MODEL_VIEW_DATA* CharaModelResourceTbl[15] = { (MODEL_VIEW_DATA*)CharaModelMain1Tbl, (MODEL_VIEW_DATA*)CharaModelMain2Tbl, (MODEL_VIEW_DATA*)CharaModelEtcTbl, (MODEL_VIEW_DATA*)CharaModelTRTbl, (MODEL_VIEW_DATA*)CharaModelWOTbl, (MODEL_VIEW_DATA*)CharaModelOLTbl, (MODEL_VIEW_DATA*)CharaModelAGTbl, (MODEL_VIEW_DATA*)CharaModelHATbl, (MODEL_VIEW_DATA*)CharaModelMOTbl, (MODEL_VIEW_DATA*)CharaModelATTbl, (MODEL_VIEW_DATA*)CharaModelNETbl, (MODEL_VIEW_DATA*)CharaModelHOTbl, (MODEL_VIEW_DATA*)CharaModelPOTbl, (MODEL_VIEW_DATA*)CharaModelDETbl, (MODEL_VIEW_DATA*)CharaModelHeartLessTbl };
MODEL_VIEW_DATA* RikuCharaModelResourceTbl[3] = { (MODEL_VIEW_DATA*)CharaModelRiku1Tbl, (MODEL_VIEW_DATA*)CharaModelRiku2Tbl, (MODEL_VIEW_DATA*)CharaModelRikuHeartLessTbl };
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
KH_DEBUG(__asm__(".rdata\n");) /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
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
                m_pModels = new KH_AT("(2485)") CModel[m_iModelCount];
#line 2487
                KH_DEBUG(if (!m_pModels) AssertApp(__FILE__, __LINE__, "m_pModels", 0);)
                pTexAnimData = CRsrcDB::getRsrcTXA(nID);
                if (pTexAnimData) {
                    m_iTexAnimNum = pTexAnimData->getDataNum();
                    if (m_iTexAnimNum > 0) {
                        m_pTexAnim = new KH_AT("(2501)") CTexAnm[m_iTexAnimNum];
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
                m_pAnime = new KH_AT("(2525)") CAnim;
#line 2527
                KH_DEBUG(if (!m_pAnime) AssertApp(__FILE__, __LINE__, "m_pAnime", 0);)
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












void CMemo3DObj::calcLocalMatrix() {
    CCoord& crd = Crd(0);
    FVECTOR posi = m_vBasePosition;
    FLOAT rate = m_vBasePosition.z / 3.0f;
    posi.x += m_vPosition.x * -rate;
    posi.y += m_vPosition.y * rate;
    vu0UnitMatrix(crd.getLocalMatrix());
    RotMatrixY(crd.getLocalMatrix(), crd.getLocalMatrix(), m_fRotY);
    crd.getLocalMatrix().v[3].set(posi);
    CCamera* pCamera = CWorldEnv::getActiveCamera();
    if (pCamera && pCamera->getCoordNum() > 0) {
        MulMatrix(crd.getLocalMatrix(), pCamera->Crd(0).getLocalMatrix(), crd.getLocalMatrix());
    }
    scaleMatrixSelf(crd.getLocalMatrix(), m_fBaseScale * m_fScale);
    crd.touch();
}
void CMemo3DObj::setRotateY(FLOAT inRotY) {
    if (_fabsf(Math3D::LimitPI(m_fRotY - inRotY)) != 0.0f) {
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
CMemo2DLayout::~CMemo2DLayout() {
    finalize();
}
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
    m_pSeqNoTbl = new KH_AT("(2881)") LayoutDataParam[inSeqNum];


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
            D2::C2DFnt* pFnt = new KH_AT("(2945)")
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
KH_DEBUG(__asm__(".rdata\n");) /* resync: gcc believes it is in .rdata after the previous placeholder */
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
    KH_DEBUG(if (!(inSeqNo < 32)) AssertApp(__FILE__, __LINE__, "inSeqNo < SEQCTRL_MAX", 0);)
    D2::C2DSeqCtrl* pSeq = new KH_AT("(3017)")
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
CMemo2DSequence::~CMemo2DSequence() {}
void CMemo2DSequence::setup(const CHAR* inSeqName, const INT* inSeqTbl, INT inLayer) {
#line 3138
    KH_DEBUG(if (!(strlen(inSeqName) < 32)) AssertApp(__FILE__, __LINE__, "strlen(inSeqName) < 32", 0);)
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
    KH_DEBUG(if (!(strlen(inSeqName) < 32)) AssertApp(__FILE__, __LINE__, "strlen(inSeqName) < 32", 0);)
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
        InterVectorXYZ(m_vPosition, aim, posi, 1.0f - powf(1.0f - m_fMoveTimer / 30.0f, 2.5f));
        setLocalPos(m_vPosition);
    }
}
void CMemo2DSequence::setSlideMoveParam(CMemo2DCursor* inCursor) {
    if (inCursor) {
        m_bMoveSlideEnable = TRUE;
        m_vAimPosition.set(inCursor->getAimVector());
        m_vShiftPosition.set(inCursor->getShiftVector());
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
CMemo2DCursor::~CMemo2DCursor() {
    finalise();
}
KH_DEBUG(__asm__(".rdata\n");) /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
void CMemo2DCursor::setup(CursorDataParam& inParam) {
    if (inParam.posiNum > 0) {
        if (inParam.posiTbl) {
            m_iPosiNum = inParam.posiNum;
            m_pPosiTbl = new KH_AT("(3348)") CursorPosition[m_iPosiNum];


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
        InterVectorXYZ(posi, aim, posi, 1.0f - powf(1.0f - m_fMoveTimer / 30.0f, 2.5f));
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
        m_vPosi.set(posi);
        m_vShiftPosi.set(shift);
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
CMemo2DCard::~CMemo2DCard() {}
KH_DEBUG(__asm__(".rdata\n");) /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
#line 3432
void CMemo2DCard::create(CARD inCard, INT inMode) {
    if (inCard.m_nCategory0 == 0xC) return;
    createPicture(inCard.getTexNo(), inCard.getTexPos());
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
CJiminyMemo::~CJiminyMemo() {
    finalize();
}
KH_DEBUG(__asm__(".rdata\n");) /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
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
namespace JiminyMemo {
namespace Base {
}
}
inline void CJiminyMemo::mainInit() {
    CMemo2DLayout* pLay;
    D2::C2DSeqCtrl* pMess;
    pLay = new KH_AT("(3948)") CMemo2DLayout(this);
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
            FVECTOR posi;
            D2::C2DMan::getLayPosition(m_pLadFileName, 0x2C, 1, posi, 0);
            pSeq->setLocalPos(posi);
        }
        pSeq = pLay->getSeqCtrl(5);
        if (pSeq) {
            FVECTOR posi;
            D2::C2DMan::getLayPosition(m_pLadFileName, 0x2C, 2, posi, 0);
            pSeq->setLocalPos(posi);
        }
    }
    pMess = new KH_AT("(4012)") D2::C2DSeqCtrl;
    if (pMess) {
        D2::C2DFnt* pFnt;
        m_pMessSeq = pMess;
        pMess->setLayer(0x11);
        pMess->enableDisp(TRUE);
        pMess->addSeq(m_pLadName, 1);
        pMess->setPos((FVECTOR&)JiminyMemo::Base::TelopMessagePosition);
        pFnt = new KH_AT("(4022)") D2::C2DFnt(0, 0, NULL, 0x10, JiminyMemo::Base::DefaultMessage, 0x40);
        if (pFnt) {
            m_pMessString = pFnt;
            pMess->addFnt(m_pLadName, pFnt);
        }
        pMess->enableDisp(TRUE);
    }
    JiminyMemo::playSE(3);
    setLayoutState(m_hLayout, 1);
    FVECTOR posi = { 0.0f, 0.0f, 0.0f, 1.0f };
    setContentScroll(0, posi);
    setContentScroll(1, posi);
    setContentStep(20);
}
inline void CJiminyMemo::mainLoadWait() {
    if (CRcf::isCompleteLoadRCF(2, JiminyMemo::RsrcId[getLoadResourceIndex()])) {
        mainInit();
        m_iStep = 30;
    }
}
inline void CJiminyMemo::mainIn() {
    mainInit();
    m_iStep = 30;
}
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
template <class T> T CJiminyMemo::getTaskPointer(HANDLE inHandle) {
    T p = NULL;
    if (inHandle) {
        p = (T)CTaskMan::getTask(inHandle);
        if (p && p->isOut()) p = NULL;
    }
    return p;
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
KH_DEBUG(__asm__(".rdata\n");)
inline void _roPh13() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE(__FILE__ "(3948)");
    TRACE(__FILE__ "(4012)");
    TRACE(__FILE__ "(4022)");
}
CMemo2DSequence* CJiminyMemo::createSequence(const INT* inSeqData, INT inLayer, INT inLayoutNo, INT inLayoutSeqNo,
                                             CMemo2DSequence* inParent) {
    FVECTOR posi;
    CMemo2DSequence* pSeq = new KH_AT("(4170)")
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
    CMemo2DSequence* pSeq = new KH_AT("(4194)")
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
    pListFont = new KH_AT("(4228)")
        D2::C2DSeqCtrl;

    if (pListFont) {
        *inFontSeq = pListFont;
        pListFont->setLayer(inLayer);
        pListFont->enableColorCtrl(TRUE);
        pListFont->setParent(inList);
        pListFont->addSeq(m_pLadName, 0x24);
        D2::C2DMan::getLayPosition(m_pLadFileName, inLayoutNo, inLayoutSeqNo, posi, 0);
        pListFont->setLocalPos(posi);
        pFnt = new KH_AT("(4240)")
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
inline void CJiminyMemo::replaceNoteListString(D2::C2DFnt* inList, const CHAR* inString) {
    if (inList) inList->setText(inString);
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
KH_DEBUG(__asm__(".rdata\n");) /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
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
}
}
CMemo2DCursor* CJiminyMemo::createCollectionCursorSequence(const INT* inSeqTbl, const CursorPosition& inStartPosi,
                                                           const CursorPosition& inStepPosi, INT inStepNum,
                                                           INT inListNum, INT inOffset, INT inStart,
                                                           INT inPosiWidth, INT inLayer, INT inState, INT inPosiId,
                                                           FLOAT inScale, CMemo2DSequence* inParent) {
    CMemo2DCursor* pCur;
    CursorDataParam param;
    pCur = new KH_AT("(4411)")
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
    pCur = new KH_AT("(4472)")
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
        pMark = new KH_AT("(4520)")
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
            KH_DEBUG(if (!(len < (inWidth - 1))) AssertApp(__FILE__, __LINE__, "len < (inWidth - 1)", 0);)
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
        if (inSeqCtrl[i]) { /* MATCH: the test reads the array, p is set inside (one more loop insn: the 1 of the case test is hoisted in loop.c's second pass) */
            D2::C2DSeqCtrl* p = inSeqCtrl[i];
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
}
}
void CJiminyMemo::contentMenu() {
    BOOL bCursorShift = FALSE;
    switch (getContentStep()) {
    case 20: {
        INT listNum;
        INT telopNo;
        int iFlag[5];
        int bComp[5];
        FVECTOR posi;
        D2::C2DSeqCtrl** pMarkSeq;
        D2::C2DSeqCtrl** pNewSeq;
        CMemo2DSequence* pList;
        if (isModeJiminyMemo()) {
            listNum = 5;
            telopNo = 0x700101;
            getMenuContentFlag(0, 0x11, &iFlag[0], &bComp[0]);
            getMenuContentFlag(0xB7, 0x98, &iFlag[1], &bComp[1]);
            iFlag[2] = iFlag[1];
            bComp[2] = bComp[1];
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
            listNum = 4;
            telopNo = 0x700116;
            getMenuContentFlag(0x11, 6, &iFlag[0], &bComp[0]);
            getMenuContentFlag(0x14F, 0x3B, &iFlag[1], &bComp[1]);
            iFlag[2] = iFlag[1];
            bComp[2] = bComp[1];
            getMenuContentFlag(0x7F, 0x38, &iFlag[3], &bComp[3]);
        }
        m_ContentMenu.iCursorPosiNum = listNum;
        m_ContentMenu.iTelopNo = telopNo;
        m_ContentMenu.iStringNum = listNum + 1;
        m_ContentMenu.pString = new KH_AT("(5252)") D2::C2DFnt*[m_ContentMenu.iStringNum];
        if (m_ContentMenu.pString) {
            for (INT i = 0; i < m_ContentMenu.iStringNum; i++) m_ContentMenu.pString[i] = NULL;
        }
        m_ContentMenu.iSeqCtrlNum = m_ContentMenu.iStringNum + 10;
        m_ContentMenu.pSeqCtrl = new KH_AT("(5258)") D2::C2DSeqCtrl*[m_ContentMenu.iSeqCtrlNum];
        if (m_ContentMenu.pSeqCtrl) {
            for (INT i = 0; i < m_ContentMenu.iSeqCtrlNum; i++) m_ContentMenu.pSeqCtrl[i] = NULL;
        }
        pMarkSeq = m_ContentMenu.pSeqCtrl + m_ContentMenu.iStringNum;
        pNewSeq = m_ContentMenu.pSeqCtrl + (m_ContentMenu.iStringNum + 5);
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
        bCursorShift = TRUE;
        createMarkSequence(0x92, listNum, 0, 8, 0x11, pList, pMarkSeq);
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
            pObj = new KH_AT("(5350)")
                CMemo3DObj(this, JiminyMemo::getHostGroupId(), JiminyMemo::getHostRsrcId(), 0x12, FALSE);
            if (pObj) {
                CMemo2DSequence* pTalk;
                m_ContentMenu.hJiminyObj = pObj->getHandle();
                pObj->setPosition(JiminyMemo::Menu::JiminyPosi);
                pObj->setRotateY(JiminyMemo::Menu::JiminyRotY);
                pObj->setScale(8.0f);
                pTalk = new KH_AT("(5359)") CMemo2DSequence;
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
                    pFnt = new KH_AT("(5377)") D2::C2DFnt(0, 0, &fontRGB, 18, CCtdMan::getText(0x700206), 0x21002);
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
KH_DEBUG(__asm__(".rdata\n");) /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
namespace JiminyMemo {
namespace Story {
}
namespace StoryView {
namespace D {
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
        pString = new KH_AT("(5569)") D2::C2DFnt*[m_ContentStory.iStringNum];
        if (pString) {
            for (INT i = 0; i < m_ContentStory.iStringNum; i++) pString[i] = NULL;
            m_ContentStory.pString = pString;
        }
        m_ContentStory.iSeqCtrlNum = m_ContentStory.iStringNum + listNum + 3;
        pSeqCtrl = new KH_AT("(5576)") D2::C2DSeqCtrl*[m_ContentStory.iSeqCtrlNum];
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
        ScrollBarInit(pSeqCtrl, 0x10, (FVECTOR&)JiminyMemo::Story::NoteScrollBarPosi, 150.0f, pList);
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
namespace StoryView {
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
        pString = new KH_AT("(5890)") D2::C2DFnt*[m_ContentStoryView.iStringNum];
        if (pString) {
            for (INT i = 0; i < m_ContentStoryView.iStringNum; i++) pString[i] = NULL;
            m_ContentStoryView.pString = pString;
        }
        m_ContentStoryView.iSeqCtrlNum = 11;
        pSeqCtrl = new KH_AT("(5897)") D2::C2DSeqCtrl*[m_ContentStoryView.iSeqCtrlNum];
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
            D2::C2DSeqCtrl* pPictSeq = new KH_AT("(5949)") D2::C2DSeqCtrl;
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
#line 5671
namespace JiminyMemo {
namespace Collect {
namespace D {
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
        const UINT* pCardFlagTbl;
        SHORT** pCardEncyclTbl;
        const CursorPosition* pCardMapOffset;
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
        pString = new KH_AT("(6227)") D2::C2DFnt*[m_ContentCollect.iStringNum];
        if (pString) {
            for (INT i = 0; i < m_ContentCollect.iStringNum; i++) pString[i] = NULL;
            m_ContentCollect.pString = pString;
        }
        m_ContentCollect.iSeqCtrlNum = m_ContentCollect.iStringNum + 1;
        pSeqCtrl = new KH_AT("(6234)") D2::C2DSeqCtrl*[m_ContentCollect.iSeqCtrlNum];
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
        pCard = new KH_AT("(6280)") CMemo2DCard*[m_ContentCollect.iCardNum];
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
                        pCard[no] = new KH_AT("(6294)") CMemo2DCard(m_pLadName);
                        if (pCard[no]) {
                            CARD card;
                            pCard[no]->setLayer(0xF);
                            pCard[no]->enableColorCtrl(TRUE);
                            pCard[no]->setParent(pBase);
                            card.init(pCardEncyclTbl[kind][i], 0xFF);
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
                const CursorPosition* pCardMapOffset;
                const CursorPosition* pMapStart;
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
namespace Collect {
namespace D {
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
    const UINT* pCursorMoveFlag;
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
        pString = new KH_AT("(7187)") D2::C2DFnt*[m_ContentCard.iStringNum];
        if (pString) {
            for (INT i = 0; i < m_ContentCard.iStringNum; i++) pString[i] = NULL;
            m_ContentCard.pString = pString;
        }
        m_ContentCard.iSeqCtrlNum = m_ContentCard.iStringNum + m_ContentCard.iListNum * 2 + 3;
        pSeqCtrl = new KH_AT("(7194)") D2::C2DSeqCtrl*[m_ContentCard.iSeqCtrlNum];
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
        ScrollBarInit(pSeqCtrl, 0x10, (FVECTOR&)JiminyMemo::Card::NoteScrollBarPosi, 150.0f, pList);
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
        pString = new KH_AT("(7546)") D2::C2DFnt*[m_ContentCardSelect.iStringNum];
        if (pString) {
            for (INT i = 0; i < m_ContentCardSelect.iStringNum; i++) pString[i] = NULL;
            m_ContentCardSelect.pString = pString;
        }
        m_ContentCardSelect.iSeqCtrlNum = m_ContentCardSelect.iStringNum + m_ContentCardSelect.iListNum + 3;
        pSeqCtrl = new KH_AT("(7553)") D2::C2DSeqCtrl*[m_ContentCardSelect.iSeqCtrlNum];
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
        ScrollBarInit(pSeqCtrl, 0x10, (FVECTOR&)JiminyMemo::CardSelect::NoteScrollBarPosi, 150.0f, pList);
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
        pString = new KH_AT("(7836)") D2::C2DFnt*[m_ContentCardView.iStringNum];
        if (pString) {
            for (INT i = 0; i < m_ContentCardView.iStringNum; i++) pString[i] = NULL;
            m_ContentCardView.pString = pString;
        }
        m_ContentCardView.iSeqCtrlNum = m_ContentCardView.iTextLineNum + 9;
        pSeqCtrl = new KH_AT("(7844)") D2::C2DSeqCtrl*[m_ContentCardView.iSeqCtrlNum];
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
            pCard = new KH_AT("(7987)") CMemo2DCard(m_pLadName);
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
                card.init(cardEncyclNo, 0xFF);
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
    const UINT* pCursorMoveFlag;
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
}
namespace JiminyMemo {
namespace Chara {
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
        pString = new KH_AT("(8341)") D2::C2DFnt*[m_ContentChara.iStringNum];
        if (pString) {
            for (INT i = 0; i < m_ContentChara.iStringNum; i++) pString[i] = NULL;
            m_ContentChara.pString = pString;
        }
        m_ContentChara.iSeqCtrlNum = m_ContentChara.iStringNum + listNum * 2 + 3;
        pSeqCtrl = new KH_AT("(8348)") D2::C2DSeqCtrl*[m_ContentChara.iSeqCtrlNum];
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
        ScrollBarInit(pSeqCtrl, 0x10, (FVECTOR&)JiminyMemo::Chara::NoteScrollBarPosi, 150.0f, pList);
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
namespace CharaSelect {
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
        pString = new KH_AT("(8702)") D2::C2DFnt*[m_ContentCharaSelect.iStringNum];
        if (pString) {
            for (INT i = 0; i < m_ContentCharaSelect.iStringNum; i++) pString[i] = NULL;
            m_ContentCharaSelect.pString = pString;
        }
        m_ContentCharaSelect.iSeqCtrlNum = m_ContentCharaSelect.iCursorPosiNum + listNum + 3;
        pSeqCtrl = new KH_AT("(8709)") D2::C2DSeqCtrl*[m_ContentCharaSelect.iSeqCtrlNum];
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
        ScrollBarInit(pSeqCtrl, 0x10, (FVECTOR&)JiminyMemo::CharaSelect::NoteScrollBarPosi, 150.0f, pList);
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
        pString = new KH_AT("(8999)") D2::C2DFnt*[m_ContentCharaView.iStringNum];
        if (pString) {
            for (INT i = 0; i < m_ContentCharaView.iStringNum; i++) pString[i] = NULL;
            m_ContentCharaView.pString = pString;
        }
        m_ContentCharaView.iSeqCtrlNum = m_ContentCharaView.iStringNum + 2;
        pSeqCtrl = new KH_AT("(9006)") D2::C2DSeqCtrl*[m_ContentCharaView.iSeqCtrlNum];
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
            CMemo2DCard* pChara = new KH_AT("(9108)") CMemo2DCard(m_pLadName);
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
}
}
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
            pObj = new KH_AT("(9474)")
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
            const FLOAT k = DEG2RAD(2.0f); /* MATCH: const local: the constant is born first, so it gets the longest span (f4) */
            FLOAT ry = (FLOAT)_getAnalogLX(0) * k / 4096.0f * getFrameRateF();
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
KH_DEBUG(__asm__(".rdata\n");) /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
namespace JiminyMemo {
namespace Minigame {
}
}
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
        pString = new KH_AT("(9647)") D2::C2DFnt*[m_ContentMinigame.iStringNum];
        if (pString) {
            for (INT i = 0; i < m_ContentMinigame.iStringNum; i++) pString[i] = NULL;
            m_ContentMinigame.pString = pString;
        }
        m_ContentMinigame.iSeqCtrlNum = m_ContentMinigame.iStringNum + 6;
        pSeqCtrl = new KH_AT("(9654)") D2::C2DSeqCtrl*[m_ContentMinigame.iSeqCtrlNum];
        if (pSeqCtrl) {
            for (INT i = 0; i < m_ContentMinigame.iSeqCtrlNum; i++) pSeqCtrl[i] = NULL;
            m_ContentMinigame.pSeqCtrl = pSeqCtrl;
        }
        pList = createSequence(JiminyMemo::Minigame::NoteSeqData, 0x10, 0x12, 0, NULL);
        if (pList) {
            int bFlag[6];
            CHAR* pPoint = NULL;
            CHAR* pMin = NULL;
            CHAR* pSec = NULL;
            CHAR* pTime = NULL;
            CHAR* pMask = NULL;
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
// SOURCE DATA: .data .rodata .bss
