#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define CTDMAN_H_PATH "System/Ctd/CtdMan.h" /* MATCH: this unit names CtdMan.h so (header default "System/CTD/CtdMan.h") */
#include "System/Task.h"
#include "System/TaskMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/Card.h"
#include "System/Ctd/CtdMan.h"
#include "Form/Menu/DecknameEdit.h"
#include "System/Fsm.h"
#include "System/PadHandler.h"
#include "System/HeapMan.h"
#include "System/Vector.h"
#include "System/Sound/SoundMan.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DFnt.h"
#include "include_asm.h"

extern INT SCREEN_HEIGHT;
static inline INT& _screenHeight() { return SCREEN_HEIGHT; }
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);








inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
#define NEW(line, T) new (com_malloc_d(sizeof(T), __FILE__ "(" #line ")", RET_ADDR())) T
#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

using namespace D2;

template <class T> void LIMIT_RANGE(T& a, T min, T max) {
    if (a > max) {
        a = max;
    } else if (a < min) {
        a = min;
    }
}

static inline void _setColor(CFntM2* p, const G_RGBA& c) { p->m_C0 = c; }

static inline void _unitVectorVu0(FVECTOR& v) {
    asm volatile("sqc2 $vf0, 0x0(%0)" : : "r"(&v));
}

namespace DECKNAME_EDIT {
typedef struct {
    CHAR* pszChar[1];
    INT m_nLayIdx;
} DN_EDIT_CHAR_DATA;

enum { PLATE_TYPE_MAX = 1 };
enum { _STATE_APPEAR, _STATE_MAIN, _STATE_LEAVE, _STATE_MAX };

static LPCSTR _pszLayoutName = "camp.lad";
static LPCSTR _pszSeqName = "camp:00";
static LPCSTR _pszSpace = "\x81\x40";
static LPCSTR _pszEndName = "END";
}

using namespace DECKNAME_EDIT;

class CDnEditBase : public CTask, public CFSM {
protected:
    FVECTOR m_fvBasePos;
    C2DSeqCtrl* m_pSeq;
    INT m_nStep;

public:
    CDnEditBase(CTask* inpParent, C2DSeqCtrl* inpParentSeq, INT inSw);
    virtual ~CDnEditBase();
    virtual void main();
    C2DSeqCtrl* _getSeq() const { return m_pSeq; }

private:
    void _init(INT inSw, C2DSeqCtrl* inpParentSeq);

protected:
    void setBasePos(INT inIdx, const CHAR* pszLayoutName, INT inLayoutIdx);
    void setBasePos2(INT inIdx, const CHAR* pszLayoutName, INT inLayoutIdx);
    void getLayPos(FVECTOR& refPos, INT inIdx, const CHAR* pszLayoutName, INT inLayoutIdx);
};

class CDnEditNameCursor : public CDnEditBase {
public:
    CDnEditNameCursor(CTask* inpParent, C2DSeqCtrl* pParent);
    virtual ~CDnEditNameCursor();
    void setCurPos(FLOAT inX);

private:
    void _initialize();
};

class CDnEditNamePlate : public CDnEditBase {
private:
    CStateTemplate<CDnEditNamePlate> m_State[3];
    C2DFnt* m_pDeckNoBase;
    C2DFnt* m_pDeckEquip;
    C2DFnt* m_pName;
    CDnEditNameCursor* m_pCursor;
    BOOL m_bAppearEnd;
    BOOL m_bLeaveEnd;
    INT m_nEquipNo;
    INT m_nDeckNo;

public:
    CDnEditNamePlate(CTask* inpParent, INT inIdx);
    virtual ~CDnEditNamePlate();
    BOOL isAppearEnd() const { return m_bAppearEnd; }
    BOOL isLeaveEnd() const { return m_bLeaveEnd; }
    void startLeave();
    void chgDeckNo(INT inIdx);
    void setName(BYTE* inpBuf);

private:
    void _initialize();
    void beginStateAppear();
    void stateAppear();
    void endStateAppear();
    void beginStateMain();
    void stateMain();
    void endStateMain();
    void beginStateLeave();
    void stateLeave();
    void endStateLeave();
    void changeState(INT inState);
    INT _getDispType();
};

class CDnEditCharLabel : public CDnEditBase {
private:
    const DN_EDIT_CHAR_DATA* m_pstData;
    C2DFnt* m_pFnt[5];
    INT m_nType;

public:
    CDnEditCharLabel(CTask* inpParent, C2DSeqCtrl* pParent, const DN_EDIT_CHAR_DATA* pstData, INT inType);
    virtual ~CDnEditCharLabel();
    void chgType(INT inType);

private:
    void _initialize(const DN_EDIT_CHAR_DATA* pstData, INT inType);
};

class CDnEditWordPlate : public CDnEditBase {
private:
    CStateTemplate<CDnEditWordPlate> m_State[3];
    INT m_nPlateType;
    C2DFnt* m_pTabLabel[1];
    CDnEditCharLabel* m_pLabel[21];
    BOOL m_bAppearEnd;
    BOOL m_bLeaveEnd;

public:
    CDnEditWordPlate(CTask* inpParent);
    virtual ~CDnEditWordPlate();
    void chgType(INT inType, BOOL bSeOn);
    BOOL isAppearEnd() const { return m_bAppearEnd; }
    BOOL isLeaveEnd() const { return m_bLeaveEnd; }
    void startLeave();

private:
    void _initialize();
    void beginStateAppear();
    void stateAppear();
    void endStateAppear();
    void beginStateMain();
    void stateMain();
    void endStateMain();
    void beginStateLeave();
    void stateLeave();
    void endStateLeave();
    void changeState(INT inState);
};

class CDnEditCursor1 : public CDnEditBase {
private:
    INT m_nCurType;

public:
    CDnEditCursor1(CTask* inpParent, C2DSeqCtrl* pParent);
    virtual ~CDnEditCursor1();
    void setCurPos(INT inX, INT inY);

private:
    void _initialize();
    void _chgCurType(INT inType);
};

class CDnEditCursor2 : public CDnEditBase {
private:
    FVECTOR m_fvPos;
    FVECTOR m_fvTargetPos;
    FLOAT m_fInterTimer;

public:
    CDnEditCursor2(CTask* inpParent, C2DSeqCtrl* pParent);
    virtual ~CDnEditCursor2();
    virtual void main();
    void setCurPos(INT inX, INT inY);
    void setCurPos2(INT inType);
    void setHokanOn();
    void setHokanOff();

private:
    void _initialize();
};

class CDecknameEdit : public CTask, public CFSM, public CPadHandler {
private:
    static HANDLE m_hHandle;
    static INT m_nDeckNo;
    CStateTemplate<CDecknameEdit> m_State[3];
    CDnEditNamePlate* m_pDeckName;
    CDnEditWordPlate* m_pWordPlate;
    CDnEditCursor1* m_pCursor1;
    CDnEditCursor2* m_pCursor2;
    INT m_nStep;
    unsigned char m_szNameBuf[3][11];
    BOOL m_bEnd;
    BOOL m_bUpdateName;
    INT m_nCharType;
    INT m_nCurX;
    INT m_nCurY;
    INT m_nSelMode;

public:
    static HANDLE startEdit(CTask* inpParent, INT inDeckNo);
    static BOOL isFinish();
    static INT getLastEditDeckNo();
    CDecknameEdit(CTask* inpParent, INT inDeckNo);
    virtual ~CDecknameEdit();
    virtual void main();

private:
    void _initialize();
    void beginStateAppear();
    void stateAppear();
    void endStateAppear();
    void beginStateMain();
    void stateMain();
    void endStateMain();
    void beginStateLeave();
    void stateLeave();
    void endStateLeave();
    void changeState(INT inState);
    void _getDeckName();
    void _setDeckName();
    BOOL _addDeckName(BYTE inCode);
    void _delDeckName();
    void _chgDeckNo();
    void _doMoveCursor();
    void __doMoveCursorMode0();
    void __doMoveCursorMode1();
    void _adjustCursor(INT nAddX, INT nAddY, BOOL bTrg, BOOL bSe);
    BOOL _isCursorEndPos();
    void _setCursorEndPos();
    BYTE _getCursorPosChar();
};

HANDLE CDecknameEdit::m_hHandle = 0;
INT CDecknameEdit::m_nDeckNo = 0;

namespace DECKNAME_EDIT {
static const DN_EDIT_CHAR_DATA _stDnEditCharData[105] = {
    { { "A" }, 7 }, { { "B" }, 8 }, { { "C" }, 9 }, { { "D" }, 10 }, { { "E" }, 11 },
    { { "a" }, 12 }, { { "b" }, 13 }, { { "c" }, 14 }, { { "d" }, 15 }, { { "e" }, 16 },
    { { "0" }, 17 }, { { "1" }, 18 }, { { "2" }, 19 }, { { "3" }, 20 }, { { "4" }, 21 },
    { { "F" }, 23 }, { { "G" }, 24 }, { { "H" }, 25 }, { { "I" }, 26 }, { { "J" }, 27 },
    { { "f" }, 28 }, { { "g" }, 29 }, { { "h" }, 30 }, { { "i" }, 31 }, { { "j" }, 32 },
    { { "5" }, 33 }, { { "6" }, 34 }, { { "7" }, 35 }, { { "8" }, 36 }, { { "9" }, 37 },
    { { "K" }, 39 }, { { "L" }, 40 }, { { "M" }, 41 }, { { "N" }, 42 }, { { "O" }, 43 },
    { { "k" }, 44 }, { { "l" }, 45 }, { { "m" }, 46 }, { { "n" }, 47 }, { { "o" }, 48 },
    { { " " }, 49 }, { { "!" }, 50 }, { { "?" }, 51 }, { { "#" }, 52 }, { { "&" }, 53 },
    { { "P" }, 55 }, { { "Q" }, 56 }, { { "R" }, 57 }, { { "S" }, 58 }, { { "T" }, 59 },
    { { "p" }, 60 }, { { "q" }, 61 }, { { "r" }, 62 }, { { "s" }, 63 }, { { "t" }, 64 },
    { { "*" }, 65 }, { { "+" }, 66 }, { { "-" }, 67 }, { { "/" }, 68 }, { { "=" }, 69 },
    { { "U" }, 71 }, { { "V" }, 72 }, { { "W" }, 73 }, { { "X" }, 74 }, { { "Y" }, 75 },
    { { "u" }, 76 }, { { "v" }, 77 }, { { "w" }, 78 }, { { "x" }, 79 }, { { "y" }, 80 },
    { { "\"" }, 81 }, { { "'" }, 82 }, { { "," }, 83 }, { { "." }, 84 }, { { ":" }, 85 },
    { { "Z" }, 87 }, { { NULL }, 88 }, { { NULL }, 89 }, { { NULL }, 90 }, { { NULL }, 91 },
    { { "z" }, 92 }, { { NULL }, 93 }, { { NULL }, 94 }, { { NULL }, 95 }, { { NULL }, 96 },
    { { ";" }, 97 }, { { "<" }, 98 }, { { ">" }, 99 }, { { "[" }, 100 }, { { "]" }, 101 },
    { { NULL }, 103 }, { { NULL }, 104 }, { { NULL }, 105 }, { { NULL }, 106 }, { { NULL }, 107 },
    { { NULL }, 108 }, { { NULL }, 109 }, { { NULL }, 110 }, { { NULL }, 111 }, { { NULL }, 112 },
    { { "@" }, 113 }, { { "_" }, 114 }, { { NULL }, 115 }, { { NULL }, 116 }, { { " " }, 117 },
};

static const short int _DnEditPlateAnim[1][3] = { { 266, 267, 268 } };
static const unsigned int _DnEditNamePlateDeckNoCtdIdx[4] = { 0x500000, 0x500001, 0x500002, 0x500003 };
static const unsigned int _DnEditNamePlateDeckNoCol[2] = { 0x80404080, 0x80406331 };
static const short int _DnEditNamePlateBasePlateAnimNo[3] = { 276, 277, 278 };
static const short int _DnEditNamePlateSubPlateAnimNo[2][2] = { { 201, 202 }, { 204, 205 } };
}

static void _convertHan2Zen(CHAR* pszOut, const CHAR* pszSrc) {
    INT j;
    INT nLen;
    INT i;

    j = 0;
    nLen = strlen(pszSrc);
    for (i = 0; i < nLen; i++) {
        pszOut[j++] = pszSrc[i];
    }
}

HANDLE CDecknameEdit::startEdit(CTask* inpParent, INT inDeckNo) {
    CTask* pTask;

    if (isFinish()) {
        m_hHandle = 0;
        pTask = NEW(601, CDecknameEdit)(inpParent, inDeckNo);
        if (pTask) {
            m_hHandle = pTask->getHandle();
        }
    }
    return m_hHandle;
}

BOOL CDecknameEdit::isFinish() {
    CTask* pTask;

    if (m_hHandle) {
        pTask = CTaskMan::getTask(m_hHandle);
        if (pTask) {
            return pTask->isOut();
        }
    }
    return TRUE;
}

INT CDecknameEdit::getLastEditDeckNo() { return m_nDeckNo; }

CDecknameEdit::CDecknameEdit(CTask* inpParent, INT inDeckNo) : CTask(inpParent, 0x4000) {
    m_pDeckName = NULL;
    m_pWordPlate = NULL;
    m_pCursor1 = NULL;
    m_pCursor2 = NULL;
    m_bEnd = FALSE;
    m_bUpdateName = FALSE;
    m_nCharType = 0;
    m_nCurX = 0;
    m_nCurY = 0;
    m_nSelMode = 0;
    TRACE_USER(HMATSUOK, "CDecknameEdit : initialize..\n");
    m_nDeckNo = inDeckNo;
    LIMIT_RANGE(m_nDeckNo, 0, 2);
    _initialize();
}

CDecknameEdit::~CDecknameEdit() {
    TRACE_USER(HMATSUOK, "CDecknameEdit : destruct .\n");
    m_hHandle = 0;
}

void CDecknameEdit::_initialize() {
    m_State[0].set(this, &CDecknameEdit::beginStateAppear, &CDecknameEdit::stateAppear, &CDecknameEdit::endStateAppear);
    m_State[1].set(this, &CDecknameEdit::beginStateMain, &CDecknameEdit::stateMain, &CDecknameEdit::endStateMain);
    m_State[2].set(this, &CDecknameEdit::beginStateLeave, &CDecknameEdit::stateLeave, &CDecknameEdit::endStateLeave);
    _getDeckName();
    m_pDeckName = NEW(677, CDnEditNamePlate)(this, m_nDeckNo);
#line 678
    ASSERT(m_pDeckName);
    m_pDeckName->setName(m_szNameBuf[m_nDeckNo]);
    m_pWordPlate = NEW(681, CDnEditWordPlate)(this);
#line 682
    ASSERT(m_pWordPlate);
    m_pCursor1 = NEW(684, CDnEditCursor1)(this, m_pWordPlate->_getSeq());
#line 685
    ASSERT(m_pCursor1);
    m_pCursor2 = NEW(687, CDnEditCursor2)(this, m_pWordPlate->_getSeq());
#line 688
    ASSERT(m_pCursor2);
    changeState(0);
}

void CDecknameEdit::changeState(INT inState) {
#line 697
    ASSERT(0 <= inState && inState < _STATE_MAX);
    gotoState(m_State[inState]);
}

void CDecknameEdit::_getDeckName() {
    {
        for (INT i = 0; i < 3; i++) {
            for (INT j = 0; j < 11; j++) {
                m_szNameBuf[i][j] = 0;
            }
        }
    }
    for (INT i = 0; i < 3; i++) {
        _convertHan2Zen((CHAR*)m_szNameBuf[i], CCard::getDeckName(i));
        TRACE_USER(HMATSUOK, "%d [%s]\n", i, m_szNameBuf[i]);
    }
}

void CDecknameEdit::_setDeckName() {
    INT i;

    for (i = 0; i < 3; i++) {
        CCard::setDeckName(i, (CHAR*)m_szNameBuf[i]);
        TRACE_USER(HMATSUOK, "%d [%s] -> [%s]\n", i, m_szNameBuf[i], CCard::getDeckName(i));
    }
}

BOOL CDecknameEdit::_addDeckName(BYTE inCode) {
    BYTE* p;
    INT nLen;

    p = m_szNameBuf[m_nDeckNo];
    nLen = 0;
    if (*p) {
        do {
            nLen++;
            p++;
            if (nLen >= 10) break;
        } while (*p);
    }
    if (nLen >= 10) {
        CSound::playSE(5, -1, 64);
        _setCursorEndPos();
        return FALSE;
    }
    p[0] = inCode;
    p[1] = 0;
    CSound::playSE(2, -1, 64);
    m_pDeckName->setName(m_szNameBuf[m_nDeckNo]);
    return TRUE;
}

void CDecknameEdit::_delDeckName() {
    BYTE* p;
    INT nLen;

    p = m_szNameBuf[m_nDeckNo];
    nLen = 0;
    if (*p) {
        do {
            nLen++;
            p++;
            if (nLen >= 10) break;
        } while (*p);
    }
    if (nLen <= 0) {
        CSound::playSE(5, -1, 64);
    } else {
        p[-1] = 0;
        CSound::playSE(4, -1, 64);
        m_pDeckName->setName(m_szNameBuf[m_nDeckNo]);
    }
}

void CDecknameEdit::_chgDeckNo() {
    CSound::playSE(3, -1, 64);
    m_pDeckName->chgDeckNo(m_nDeckNo);
    m_pDeckName->setName(m_szNameBuf[m_nDeckNo]);
}

void CDecknameEdit::main() { update(); }

void CDecknameEdit::beginStateAppear() { m_nStep = 0; }

void CDecknameEdit::stateAppear() {
    if (m_pDeckName->isAppearEnd() && m_pWordPlate->isAppearEnd()) {
        changeState(1);
    }
}

void CDecknameEdit::endStateAppear() {}

void CDecknameEdit::beginStateMain() { m_nStep = 0; }

void CDecknameEdit::stateMain() {
    _doMoveCursor();
    if (m_bEnd) {
        changeState(2);
    }
}

void CDecknameEdit::endStateMain() {}

void CDecknameEdit::_doMoveCursor() {
    if (m_nSelMode == 0) {
        __doMoveCursorMode0();
    } else {
        __doMoveCursorMode1();
    }
}

void CDecknameEdit::__doMoveCursorMode0() {
    INT nAddX = 0;
    INT nAddY = 0;
    BOOL bTrg = FALSE;
    BOOL bSe = FALSE;
    BYTE c;

    if (isTrgOK()) {
        if (_isCursorEndPos()) {
            CSound::playSE(2, -1, 64);
            m_bUpdateName = TRUE;
            m_bEnd = TRUE;
            return;
        }
        c = _getCursorPosChar();
        if (c) {
            _addDeckName(c);
            return;
        }
    } else if (isTrgCANCEL()) {
        CSound::playSE(4, -1, 64);
        m_bUpdateName = FALSE;
        m_bEnd = TRUE;
        return;
    } else if (isTrgACTION()) {
        _delDeckName();
        return;
    }
    if (isButtonTrg(1)) {
        m_nDeckNo--;
        if (m_nDeckNo < 0) m_nDeckNo = 2;
        _chgDeckNo();
        return;
    }
    if (isButtonTrg(2)) {
        m_nDeckNo++;
        if (m_nDeckNo >= 3) m_nDeckNo = 0;
        _chgDeckNo();
        return;
    }
    if (isButtonRpt(4)) {
        m_nCharType--;
        if (m_nCharType < 0) m_nCharType = 0;
        m_pWordPlate->chgType(m_nCharType, TRUE);
    } else if (isButtonRpt(8)) {
        m_nCharType++;
        if (m_nCharType >= PLATE_TYPE_MAX) m_nCharType = 0;
        m_pWordPlate->chgType(m_nCharType, TRUE);
    }
    if (isButtonTrg(0x800)) {
        CSound::playSE(14, -1, 64);
        _setCursorEndPos();
    } else if (isButtonRpt(0x101000)) {
        bSe = TRUE;
        bTrg = FALSE;
        if (isButtonTrg(0x101000)) bTrg = TRUE;
        nAddY = -1;
    } else if (isButtonRpt(0x408000)) {
        bSe = TRUE;
        bTrg = FALSE;
        if (isButtonTrg(0x408000)) bTrg = TRUE;
        nAddX = -1;
    } else if (isButtonRpt(0x204000)) {
        bSe = TRUE;
        bTrg = FALSE;
        if (isButtonTrg(0x204000)) bTrg = TRUE;
        nAddY = 1;
    } else if (isButtonRpt(0x802000)) {
        bSe = TRUE;
        bTrg = FALSE;
        if (isButtonTrg(0x802000)) bTrg = TRUE;
        nAddX = 1;
    }
    _adjustCursor(nAddX, nAddY, bTrg, bSe);
    m_pCursor1->setCurPos(m_nCurX, m_nCurY);
    m_pCursor2->setCurPos(m_nCurX, m_nCurY);
}

void CDecknameEdit::__doMoveCursorMode1() {
    INT nAddY = 0;
    BOOL bTrg = FALSE;

    if (isTrgCANCEL()) {
        CSound::playSE(4, -1, 64);
        m_bUpdateName = FALSE;
        m_bEnd = TRUE;
        return;
    }
    if (isTrgACTION()) {
        _delDeckName();
        return;
    }
    if (isButtonTrg(1)) {
        m_nDeckNo--;
        if (m_nDeckNo < 0) m_nDeckNo = 2;
        _chgDeckNo();
        return;
    }
    if (isButtonTrg(2)) {
        m_nDeckNo++;
        if (m_nDeckNo >= 3) m_nDeckNo = 0;
        _chgDeckNo();
        return;
    }
    if (isButtonTrg(0x204000)) {
        CSound::playSE(14, -1, 64);
        bTrg = TRUE;
        m_pCursor2->setHokanOn();
        m_nSelMode = 0;
    } else if (isButtonTrg(0x101000)) {
        CSound::playSE(14, -1, 64);
        nAddY = -1;
        bTrg = TRUE;
        m_pCursor2->setHokanOn();
        m_nSelMode = 0;
    } else if (isButtonRpt(0x408004)) {
        m_nCharType--;
        if (m_nCharType < 0) m_nCharType = 0;
        m_pWordPlate->chgType(m_nCharType, TRUE);
    } else if (isButtonRpt(0x802008)) {
        m_nCharType++;
        if (m_nCharType >= PLATE_TYPE_MAX) m_nCharType = 0;
        m_pWordPlate->chgType(m_nCharType, TRUE);
    }
    _adjustCursor(0, nAddY, bTrg, FALSE);
    m_pCursor1->setCurPos(m_nCurX, m_nCurY);
    m_pCursor2->setCurPos2(m_nCharType);
}

BOOL CDecknameEdit::_isCursorEndPos() {
    if (m_nCurX == 14 && m_nCurY == 6) {
        return TRUE;
    }
    return FALSE;
}

void CDecknameEdit::_setCursorEndPos() {
    m_nCurX = 14;
    m_nCurY = 6;
}

BYTE CDecknameEdit::_getCursorPosChar() {
    INT nID;

    if (_isCursorEndPos()) {
        return 0;
    }
    nID = m_nCurY * 15 + m_nCurX;
    if (_stDnEditCharData[nID].pszChar[m_nCharType]) {
        return *_stDnEditCharData[nID].pszChar[m_nCharType];
    }
    return 0;
}

void CDecknameEdit::_adjustCursor(INT nAddX, INT nAddY, BOOL bTrg, BOOL bSe) {
    BOOL bNowEnd;
    INT nCount;
    INT nOldX;
    INT nOldY;
    INT nID;

    nCount = 0;
    bNowEnd = _isCursorEndPos();
    nOldX = m_nCurX;
    nOldY = m_nCurY;
    while (TRUE) {
        m_nCurX += nAddX;
        m_nCurY += nAddY;
        if (m_nCurX < 0) m_nCurX = 14;
        if (m_nCurX >= 15) m_nCurX = 0;
        if (m_nCurY < 0) m_nCurY = 6;
        if (m_nCurY >= 7) m_nCurY = 0;
        nID = m_nCurY * 15 + m_nCurX;
        if (_stDnEditCharData[nID].pszChar[m_nCharType]) break;
        if (nCount++ >= 51) break;
        if (nAddX == 0 && nAddY == 0) {
            nAddX = -1;
            continue;
        }
        if (m_nCurY >= 6 && m_nCurX >= 10 && nAddX >= 0 && !bNowEnd) {
            _setCursorEndPos();
            nAddY = 0;
            nAddX = 0;
        }
    }
    if (!bTrg) {
        if (nAddX < 0 && nOldX < m_nCurX) m_nCurX = nOldX;
        if (nAddX > 0 && m_nCurX < nOldX) m_nCurX = nOldX;
        if (nAddY < 0 && nOldY < m_nCurY) m_nCurY = nOldY;
        if (nAddY > 0 && m_nCurY < nOldY) m_nCurY = nOldY;
    }
    if (bSe && (nOldX != m_nCurX || nOldY != m_nCurY)) {
        CSound::playSE(14, -1, 64);
    }
}

void CDecknameEdit::beginStateLeave() {
    if (m_bUpdateName) {
        _setDeckName();
    }
    m_pDeckName->startLeave();
    m_pWordPlate->startLeave();
    m_nStep = 0;
    m_hHandle = 0;
}

void CDecknameEdit::stateLeave() {
    if (m_nStep == 0 && m_pDeckName->isLeaveEnd() && m_pWordPlate->isLeaveEnd()) {
        m_nStep = 1;
        out();
    }
}

void CDecknameEdit::endStateLeave() {}

CDnEditBase::CDnEditBase(CTask* inpParent, C2DSeqCtrl* inpParentSeq, INT inSw) : CTask(inpParent, 0x4000) {
    m_pSeq = NULL;
    m_nStep = 0;
    _init(inSw, inpParentSeq);
}

CDnEditBase::~CDnEditBase() {
    if (m_pSeq) {
        delete m_pSeq;
    }
    m_pSeq = NULL;
}

void CDnEditBase::_init(INT inSw, C2DSeqCtrl* inpParentSeq) {
    FVECTOR fvPos;

    m_pSeq = NEW(1208, C2DSeqCtrl)(inSw);
#line 1209
    ASSERT(m_pSeq);
    m_pSeq->setLayer(16);
    _unitVectorVu0(m_fvBasePos);
    if (inpParentSeq) {
        fvPos.set(256.0f, SCREEN_HEIGHT >> 1, 0.0f, 1.0f);
        m_pSeq->setPos(fvPos);
        m_pSeq->setParent(inpParentSeq);
    } else {
        m_pSeq->setPos(m_fvBasePos);
    }
    m_pSeq->enableColorCtrl(TRUE);
}

void CDnEditBase::setBasePos(INT inIdx, const CHAR* pszLayoutName, INT inLayoutIdx) {
    if (pszLayoutName == NULL) pszLayoutName = _pszLayoutName;
    if (inLayoutIdx <= -1) inLayoutIdx = 12;
    C2DMan::getLayPosition(pszLayoutName, inLayoutIdx, inIdx, m_fvBasePos, 0);
}

void CDnEditBase::setBasePos2(INT inIdx, const CHAR* pszLayoutName, INT inLayoutIdx) {
    setBasePos(inIdx, pszLayoutName, inLayoutIdx);
    m_fvBasePos.x -= 256.0f;
    m_fvBasePos.y -= _screenHeight() >> 1;
}

void CDnEditBase::getLayPos(FVECTOR& refPos, INT inIdx, const CHAR* pszLayoutName, INT inLayoutIdx) {
    if (pszLayoutName == NULL) pszLayoutName = _pszLayoutName;
    if (inLayoutIdx <= -1) inLayoutIdx = 12;
    C2DMan::getLayPosition(pszLayoutName, inLayoutIdx, inIdx, refPos, 0);
    SubVectorXYZ(refPos, refPos, m_fvBasePos);
}

void CDnEditBase::main() { update(); }

CDnEditNamePlate::CDnEditNamePlate(CTask* inpParent, INT inIdx) : CDnEditBase(inpParent, NULL, 0) {
    m_nDeckNo = inIdx;
    m_pDeckNoBase = NULL;
    m_pDeckEquip = NULL;
    m_pName = NULL;
    m_pCursor = NULL;
    m_bAppearEnd = FALSE;
    m_bLeaveEnd = FALSE;
    m_nEquipNo = 0;
    _initialize();
}

CDnEditNamePlate::~CDnEditNamePlate() {}

void CDnEditNamePlate::_initialize() {
    m_State[0].set(this, &CDnEditNamePlate::beginStateAppear, &CDnEditNamePlate::stateAppear, &CDnEditNamePlate::endStateAppear);
    m_State[1].set(this, &CDnEditNamePlate::beginStateMain, &CDnEditNamePlate::stateMain, &CDnEditNamePlate::endStateMain);
    m_State[2].set(this, &CDnEditNamePlate::beginStateLeave, &CDnEditNamePlate::stateLeave, &CDnEditNamePlate::endStateLeave);
    INT nType;
    FVECTOR fvPos;
    G_RGBA stRGBA;

    m_nEquipNo = CCard::getCurrentDeckNo();
    LIMIT_RANGE(m_nDeckNo, 0, 2);
    nType = _getDispType();
    setBasePos(2, NULL, -1);
    m_pSeq->addSeqShift(_pszSeqName, _DnEditNamePlateBasePlateAnimNo[0], m_fvBasePos.x, m_fvBasePos.y);
    getLayPos(fvPos, 4, NULL, -1);
    m_pSeq->addSeqShift(_pszSeqName, _DnEditNamePlateSubPlateAnimNo[0][nType], fvPos.x, fvPos.y);
    getLayPos(fvPos, 5, NULL, -1);
    m_pSeq->addSeqShift(_pszSeqName, _DnEditNamePlateSubPlateAnimNo[1][nType], fvPos.x, fvPos.y);
    stRGBA.col = 0x80808080;
    m_pDeckNoBase = NEW(1307, C2DFnt)(0, 0, &stRGBA, 14, NULL, 0x42);
    m_pSeq->addFntShift(_pszSeqName, m_pDeckNoBase, 46.0f, 7.0f);
    stRGBA.col = 0x80007878;
    m_pDeckEquip = NEW(1311, C2DFnt)(0, 0, &stRGBA, 13, NULL, 0x42);
    m_pSeq->addFntShift(_pszSeqName, m_pDeckEquip, 276.0f, 6.0f);
    chgDeckNo(m_nDeckNo);
    stRGBA.col = 0x80808080;
    m_pName = NEW(1318, C2DFnt)(0, 0, &stRGBA, 15, NULL, 0x40);
    getLayPos(fvPos, 6, NULL, -1);
    m_pSeq->addFntShift(_pszSeqName, m_pName, fvPos.x + -10.0f, fvPos.y + 0.0f);
    m_pCursor = NEW(1323, CDnEditNameCursor)(this, m_pSeq);
    changeState(0);
}

INT CDnEditNamePlate::_getDispType() { return m_nDeckNo != m_nEquipNo; }

void CDnEditNamePlate::chgDeckNo(INT inIdx) {
    INT nOld;
    INT nNew;
    FVECTOR fvPos;

    nOld = _getDispType();
    m_nDeckNo = inIdx;
    nNew = _getDispType();
    if (nOld != nNew) {
        getLayPos(fvPos, 4, NULL, -1);
        m_pSeq->replaceSeqShift(1, _pszSeqName, _DnEditNamePlateSubPlateAnimNo[0][nNew], fvPos.x, fvPos.y);
        getLayPos(fvPos, 5, NULL, -1);
        m_pSeq->replaceSeqShift(2, _pszSeqName, _DnEditNamePlateSubPlateAnimNo[1][nNew], fvPos.x, fvPos.y);
    }
    m_pDeckNoBase->m_C0.col = _DnEditNamePlateDeckNoCol[nNew];
    m_pDeckNoBase->setText(CCtdMan::getText(_DnEditNamePlateDeckNoCtdIdx[m_nDeckNo]));
    if (nNew) {
        m_pDeckEquip->setText(_pszSpace);
    } else {
        m_pDeckEquip->setText(CCtdMan::getText(_DnEditNamePlateDeckNoCtdIdx[3]));
    }
}

void CDnEditNamePlate::setName(BYTE* inpBuf) {
    G_XY stSize;

    m_pName->setText((CHAR*)inpBuf);
    m_pName->getSize(stSize, NULL, NULL);
    m_pCursor->setCurPos(stSize.x >> 4);
}

void CDnEditNamePlate::changeState(INT inState) {
#line 1383
    ASSERT(0 <= inState && inState < _STATE_MAX);
    gotoState(m_State[inState]);
}

void CDnEditNamePlate::startLeave() {
    if (!m_bLeaveEnd) {
        changeState(2);
    }
}

void CDnEditNamePlate::beginStateAppear() {}

void CDnEditNamePlate::stateAppear() {
    if (m_pSeq->isEnd(0)) {
        m_pSeq->replaceSeqShift(0, _pszSeqName, _DnEditNamePlateBasePlateAnimNo[1], m_fvBasePos.x, m_fvBasePos.y);
        changeState(1);
    }
}

void CDnEditNamePlate::endStateAppear() {}

void CDnEditNamePlate::beginStateMain() { m_bAppearEnd = TRUE; }

void CDnEditNamePlate::stateMain() {}

void CDnEditNamePlate::endStateMain() {}

void CDnEditNamePlate::beginStateLeave() {
    m_pSeq->replaceSeqShift(0, _pszSeqName, _DnEditNamePlateBasePlateAnimNo[2], m_fvBasePos.x, m_fvBasePos.y);
}

void CDnEditNamePlate::stateLeave() {
    if (!m_bLeaveEnd && m_pSeq->isEnd(0)) {
        m_bLeaveEnd = TRUE;
    }
}

void CDnEditNamePlate::endStateLeave() {}

CDnEditNameCursor::CDnEditNameCursor(CTask* inpParent, C2DSeqCtrl* pParent) : CDnEditBase(inpParent, pParent, 0) {
    _initialize();
}

CDnEditNameCursor::~CDnEditNameCursor() {}

void CDnEditNameCursor::_initialize() {
    setBasePos2(2, NULL, -1);
    m_pSeq->addSeq(_pszSeqName, 0x10F);
    setCurPos(0.0f);
}

void CDnEditNameCursor::setCurPos(FLOAT inX) {
    FVECTOR fvPos;

    _unitVectorVu0(fvPos);
    getLayPos(fvPos, 6, NULL, -1);
    fvPos.x += inX + -6.0f;
    m_pSeq->setPos(fvPos);
}

CDnEditCharLabel::CDnEditCharLabel(CTask* inpParent, C2DSeqCtrl* pParent, const DN_EDIT_CHAR_DATA* pstData, INT inType)
    : CDnEditBase(inpParent, pParent, 0) {
    m_pstData = pstData;
    m_nType = inType;
#line 1482
    ASSERT(m_pstData);
    ASSERT(inType >= 0 && inType < PLATE_TYPE_MAX);
    _initialize(pstData, inType);
}

CDnEditCharLabel::~CDnEditCharLabel() {}

void CDnEditCharLabel::_initialize(const DN_EDIT_CHAR_DATA* pstData, INT inType) {
    FVECTOR fvPos;
    G_RGBA stRGBA;
    INT i;

    m_pSeq->addSeqShift(_pszSeqName, 0x118, 0.0f, 0.0f);
    setBasePos(3, NULL, -1);
    stRGBA.col = 0x80808080;
    for (i = 0; i < 5; i++) {
        m_pFnt[i] = NEW(1499, C2DFnt)(0, 0, &stRGBA, 18, pstData->pszChar[inType], 0x42);
#line 1500
        ASSERT(m_pFnt[i]);
        getLayPos(fvPos, pstData->m_nLayIdx, NULL, -1);
        m_pSeq->addFntShift(_pszSeqName, m_pFnt[i], fvPos.x + 9.0f, fvPos.y);
        pstData++;
    }
}

void CDnEditCharLabel::chgType(INT inType) {
    const DN_EDIT_CHAR_DATA* pstData;
    INT i;

#line 1507
    ASSERT(m_pstData);
    ASSERT(inType >= 0 && inType < PLATE_TYPE_MAX);
    if (inType != m_nType) {
        pstData = m_pstData;
        for (i = 0; i < 5; i++, pstData++) {
            if (pstData->pszChar[inType]) {
                m_pFnt[i]->setText(pstData->pszChar[inType]);
            } else {
                m_pFnt[i]->setText(_pszSpace);
            }
        }
        m_nType = inType;
    }
}

CDnEditWordPlate::CDnEditWordPlate(CTask* inpParent) : CDnEditBase(inpParent, NULL, 0) {
    INT i;

    m_nPlateType = 0;
    m_bAppearEnd = FALSE;
    m_bLeaveEnd = FALSE;
    for (i = 0; i < 1; i++) {
        m_pTabLabel[i] = NULL;
    }
    for (i = 0; i < 21; i++) {
        m_pLabel[i] = NULL;
    }
    _initialize();
}

CDnEditWordPlate::~CDnEditWordPlate() {}

void CDnEditWordPlate::_initialize() {
    m_State[0].set(this, &CDnEditWordPlate::beginStateAppear, &CDnEditWordPlate::stateAppear, &CDnEditWordPlate::endStateAppear);
    m_State[1].set(this, &CDnEditWordPlate::beginStateMain, &CDnEditWordPlate::stateMain, &CDnEditWordPlate::endStateMain);
    m_State[2].set(this, &CDnEditWordPlate::beginStateLeave, &CDnEditWordPlate::stateLeave, &CDnEditWordPlate::endStateLeave);
    FVECTOR fvPos;
    G_RGBA stRGBA;
    C2DFnt* pFnt;
    INT i;

    setBasePos(3, NULL, -1);
    m_pSeq->addSeqShift(_pszSeqName, _DnEditPlateAnim[m_nPlateType][0], m_fvBasePos.x, m_fvBasePos.y);
    for (i = 0; i < 21; i++) {
        m_pLabel[i] = NEW(1573, CDnEditCharLabel)(this, m_pSeq, &_stDnEditCharData[i * 5], m_nPlateType);
#line 1574
        ASSERT(m_pLabel[i]);
    }
    stRGBA.col = 0x80808080;
    getLayPos(fvPos, 117, NULL, -1);
    pFnt = NEW(1580, C2DFnt)(0, 0, &stRGBA, 16, _pszEndName, 0x42);
    m_pSeq->addFntShift(_pszSeqName, pFnt, fvPos.x, fvPos.y + 4.0f);
    changeState(0);
}

void CDnEditWordPlate::changeState(INT inState) {
#line 1589
    ASSERT(0 <= inState && inState < _STATE_MAX);
    gotoState(m_State[inState]);
}

void CDnEditWordPlate::startLeave() {
    if (!m_bLeaveEnd) {
        changeState(2);
    }
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/DecknameEdit", chgType__16CDnEditWordPlateii);
/* Scheduling only: same instructions, but the original fills the bSeOn branch's delay slot with the playSE
 * "li a0,3" (ours: "li a1,-1") and allocates the tab label pointer to v1 (ours: a1). */
#if 0
void CDnEditWordPlate::chgType(INT inType, BOOL bSeOn) {
    G_RGBA stRGBA;
    INT i;

    if (inType != m_nPlateType) {
        if (bSeOn) {
            CSound::playSE(3, -1, 64);
        }
        stRGBA.col = 0x80202020;
        _setColor(m_pTabLabel[m_nPlateType], stRGBA);
        m_pSeq->replaceSeqShift(0, _pszSeqName, _DnEditPlateAnim[inType][1], m_fvBasePos.x, m_fvBasePos.y);
        for (i = 0; i < 21; i++) {
            m_pLabel[i]->chgType(inType);
        }
        m_nPlateType = inType;
        stRGBA.col = 0x80808080;
        _setColor(m_pTabLabel[m_nPlateType], stRGBA);
    }
}
#endif

void CDnEditWordPlate::beginStateAppear() {}

void CDnEditWordPlate::stateAppear() {
    if (m_pSeq->isEnd(0)) {
        m_pSeq->replaceSeqShift(0, _pszSeqName, _DnEditPlateAnim[m_nPlateType][1], m_fvBasePos.x, m_fvBasePos.y);
        changeState(1);
    }
}

void CDnEditWordPlate::endStateAppear() {}

void CDnEditWordPlate::beginStateMain() { m_bAppearEnd = TRUE; }

void CDnEditWordPlate::stateMain() {}

void CDnEditWordPlate::endStateMain() {}

void CDnEditWordPlate::beginStateLeave() {
    m_pSeq->replaceSeqShift(0, _pszSeqName, _DnEditPlateAnim[m_nPlateType][2], m_fvBasePos.x, m_fvBasePos.y);
}

void CDnEditWordPlate::stateLeave() {
    if (m_pSeq->isEnd(0)) {
        m_bLeaveEnd = TRUE;
    }
}

void CDnEditWordPlate::endStateLeave() {}

CDnEditCursor1::CDnEditCursor1(CTask* inpParent, C2DSeqCtrl* pParent) : CDnEditBase(inpParent, pParent, 0) {
    m_nCurType = 0;
    _initialize();
}

CDnEditCursor1::~CDnEditCursor1() {}

void CDnEditCursor1::_initialize() {
    setBasePos2(3, NULL, -1);
    m_pSeq->addSeq(_pszSeqName, 0x10D);
    setCurPos(0, 0);
}

void CDnEditCursor1::_chgCurType(INT inType) {
    static short int _cur[2] = { 269, 270 };

    if (inType != m_nCurType) {
        m_nCurType = inType;
        m_pSeq->replaceSeq(0, _pszSeqName, _cur[inType]);
    }
}

void CDnEditCursor1::setCurPos(INT inX, INT inY) {
    static float _curposx[2] = { 8.0f, 0.0f };
    static float _curposy[2] = { -8.0f, -3.0f };
    INT nID;
    FVECTOR fvPos;

    LIMIT_RANGE(inX, 0, 14);
    LIMIT_RANGE(inY, 0, 6);
    nID = inY * 15 + inX;
    if (nID == 104) {
        _chgCurType(1);
    } else {
        _chgCurType(0);
    }
    getLayPos(fvPos, _stDnEditCharData[nID].m_nLayIdx, NULL, -1);
    fvPos.x += _curposx[m_nCurType];
    fvPos.y += _curposy[m_nCurType];
    m_pSeq->setPos(fvPos);
}

CDnEditCursor2::CDnEditCursor2(CTask* inpParent, C2DSeqCtrl* pParent) : CDnEditBase(inpParent, pParent, 0) {
    m_fInterTimer = 0.0f;
    _initialize();
}

CDnEditCursor2::~CDnEditCursor2() {}

void CDnEditCursor2::_initialize() {
    setBasePos2(3, NULL, -1);
    m_pSeq->addSeq(_pszSeqName, 0x12);
    _unitVectorVu0(m_fvPos);
    _unitVectorVu0(m_fvTargetPos);
    setCurPos(0, 0);
}

void CDnEditCursor2::main() {
    m_fInterTimer -= getFrameRateF();
    if (m_fInterTimer < 0.0f) {
        m_fInterTimer = 0.0f;
    }
    if (m_fInterTimer > 0.0f) {
        m_fvPos.x += (m_fvTargetPos.x - m_fvPos.x) * 0.5f;
        m_fvPos.y += (m_fvTargetPos.y - m_fvPos.y) * 0.5f;
    } else {
        m_fvPos = m_fvTargetPos;
    }
    if (m_pSeq) {
        m_pSeq->setPos(m_fvPos);
    }
}

void CDnEditCursor2::setCurPos(INT inX, INT inY) {
    static float _curposx[2] = { 0.0f, -20.0f };
    static float _curposy[2] = { -14.0f, -10.0f };
    INT nIdx;
    INT nID;
    FVECTOR fvPos;

    LIMIT_RANGE(inX, 0, 14);
    LIMIT_RANGE(inY, 0, 6);
    nID = inY * 15 + inX;
    nIdx = (nID == 104);
    getLayPos(fvPos, _stDnEditCharData[nID].m_nLayIdx, NULL, -1);
    m_fvTargetPos.x = fvPos.x + _curposx[nIdx];
    m_fvTargetPos.y = fvPos.y + _curposy[nIdx];
}

void CDnEditCursor2::setCurPos2(INT inType) {
    static float _curposx[1] = { 86.0f };
    static float _curposy[1] = { 78.0f };
    FVECTOR fvPos;

    LIMIT_RANGE(inType, 0, 0);
    getLayPos(fvPos, 3, NULL, -1);
    m_fvTargetPos.x = fvPos.x + _curposx[inType];
    m_fvTargetPos.y = fvPos.y + _curposy[inType];
}

void CDnEditCursor2::setHokanOn() { m_fInterTimer = 8.0f; }

void CDnEditCursor2::setHokanOff() { m_fInterTimer = 0.0f; }

HANDLE CDecknameEditWnd::startEdit(CTask* inpParent, INT inDeckNo) { return CDecknameEdit::startEdit(inpParent, inDeckNo); }

BOOL CDecknameEditWnd::isFinish() { return CDecknameEdit::isFinish(); }

INT CDecknameEditWnd::getLastEditDeckNo() { return CDecknameEdit::getLastEditDeckNo(); }
// SOURCE DATA: .data .rodata
