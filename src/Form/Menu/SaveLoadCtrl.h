// PARTIAL: added for Form/SystemMessage
#ifndef FORM_MENU_SAVELOADCTRL_H
#define FORM_MENU_SAVELOADCTRL_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Task.h"
#include "System/Graphic/2d/2DSeqCtrl.h"

/* __FILE__ of this header in the original differs per includer ("Form/Menu/SaveLoadCtrl.h" in
 * Form/SystemMessage, "c:/KHCOM/MainApp/Form/Menu/SaveLoadCtrl.h" in Form/Menu/SaveLoad): define
 * SAVELOADCTRL_H_PATH before including it to pick another one. */
#ifndef SAVELOADCTRL_H_PATH
#define SAVELOADCTRL_H_PATH "Form/Menu/SaveLoadCtrl.h"
#endif














class C2DSeqAgent : public CTask {
public:
    enum STATE {
        STATE_IN = 0,
        STATE_LOOP = 1,
        STATE_OUT = 2,
        _STATE_MAX = 3,
        _STATE_NOP = 3,
        _STATE_NOT_START = 4
    };
    struct SeqTbl {
        short _tbl[3];
    };

private:
    CHAR* m_pszLad;                    /* 0x40 */
    CHAR* m_pszSqd;                    /* 0x44 */
    HANDLE m_hSeqSingle;               /* 0x48 */
    D2::C2DSeqCtrl* m_pSeqGroup;       /* 0x4c */
    SHORT m_nSeqUnit;                  /* 0x50 */
    SHORT m_nSeqGroupTarget;           /* 0x52 */
    void (*m_pcbEndIN)(C2DSeqAgent&);  /* 0x54 */
    void (*m_pcbEndOUT)(C2DSeqAgent&); /* 0x58 */
    STATE m_nState;                    /* 0x5c */
    SeqTbl m_SeqTbl;                   /* 0x60 */
    SHORT m_nCurrentSeq;               /* 0x66 */
    SHORT m_nLayer;                    /* 0x68 */

public:
    /* The inline members are emitted (weak) in Form/Menu/SaveLoadCtrl.cpp, in this order: keep it. */
    C2DSeqAgent(CTask* pParent);
    virtual ~C2DSeqAgent() {
        if (m_hSeqSingle) D2::C2DMan::destroy(m_hSeqSingle);
    }
    virtual void dispose();
    void setupSeq(const CHAR* pszLad, SHORT nLayer) {
        m_nLayer = nLayer;
        m_pszLad = (CHAR*)pszLad;
        m_pszSqd = NULL;
    }
    void setupSeq(D2::C2DSeqCtrl& seq, const CHAR* pszLad, SHORT nUnit, SHORT nTargetIdx);
    void setupSeq(D2::C2DSeqCtrl& seq, const CHAR* pszSqd, SHORT nTargetIdx);
    void setupState(const SeqTbl& t, void (*cbEndIN)(C2DSeqAgent&), void (*cbEndOUT)(C2DSeqAgent&)) {
        setupState(t._tbl[0], t._tbl[1], t._tbl[2], cbEndIN, cbEndOUT);
    }
    void setupState(const SeqTbl& t) { setupState(t._tbl[0], t._tbl[1], t._tbl[2]); }
    void setupState(SHORT nIN, SHORT nLOOP, SHORT nOUT, void (*cbEndIN)(C2DSeqAgent&),
                    void (*cbEndOUT)(C2DSeqAgent&));
    void setupState(SHORT nIN, SHORT nLOOP, SHORT nOUT);
    void start();
    void startOrRestart() {
        if (isNotStarted()) {
            start();
        } else {
            changeState(STATE_IN, TRUE, -1);
        }
    }
    virtual void main();
    virtual void procLoop() {}
    void changeState(STATE nState, BOOL bSkipIfSame, SHORT nExtSeq);
    void changeSeq(SHORT nSeqID);
    STATE getState() const { return m_nState; }
#ifdef FORM_MENU_SAVELOADCTRL_CPP /* MATCH: FORM_MENU_SAVELOADCTRL_CPP gate, only for its own unit (tested 2026-11-02) */
    BOOL isNotStarted() const { return m_nState == _STATE_NOT_START; }
#else
    /* Inline in the original, but its callers call it (not inlined there): declared only, here. */
    BOOL isNotStarted() const;
#endif
    SHORT getCurerntSeq() const { return m_nCurrentSeq; }
    D2::C2DSeqCtrl* getSeqCtrl() { return m_pSeqGroup; }
    HANDLE getSeqHandle() const { return m_hSeqSingle; }
    void dispOff() {
        if (m_pSeqGroup) {
            m_pSeqGroup->enableDisp(FALSE);
            m_nState = _STATE_NOP;
        }
    }
    BOOL isSeqCtrlVisible() const { return m_pSeqGroup && m_pSeqGroup->isEnableDisp(); }

protected:
    void outSeq();
    virtual void onOutSeq() {}
    CHAR* getLadName() const { return m_pszLad; }
    SHORT getSeqUnit() const { return m_nSeqUnit; }
};

struct FPOINT { /* POINT_T<float> in the original */
    FLOAT x;
    FLOAT y;
};

struct G_RGBA;

class C2DSeqCursorCtrl : public D2::C2DSeqCtrl {
private:
    CHAR* m_pszLad;      /* 0x160 */
    SHORT m_nSeqUnit;    /* 0x164 */
    SHORT m_nState;      /* 0x166 */
    short m_nSeqTbl[2];  /* 0x168 */

public:
    void setupSeq(const CHAR* pszLad, SHORT nUnit, SHORT nSeqNormal, SHORT nSeqHistory);
    // PARTIAL: added for Form/Menu/SaveLoad
    void setNormalSeq() { replaceSeq(0, m_pszLad, m_nSeqUnit, m_nSeqTbl[0]); }
    void setHistorySeq() { replaceSeq(0, m_pszLad, m_nSeqUnit, m_nSeqTbl[1]); }
};

class C2DSeqLightMarkAgent : public C2DSeqAgent {
private:
    D2::C2DSeqCtrl* m_pCtrl; /* 0x70 */
    FVECTOR m_vBasePos;      /* 0x80 */
    FVECTOR m_vTargetPos;    /* 0x90 */
    TIMEQ m_nMoveTime;       /* 0xa0 */
    INT m_nFlag;             /* 0xa4 */
    friend class CSaveLoadUI; // PARTIAL: added for Form/Menu/SaveLoad

public:
#ifdef FORM_MENU_SAVELOAD_CPP /* MATCH: FORM_MENU_SAVELOAD_CPP gate, only for its own unit (tested 2026-11-02) */
    /* Inline in the original: emitted (weak, with the vtable) in Form/Menu/SaveLoad, in this order. */
    C2DSeqLightMarkAgent(CTask* pParent) : C2DSeqAgent(pParent) {
        m_nMoveTime = -1;
        m_pCtrl = NULL;
        m_nFlag = 0;
    }
    virtual ~C2DSeqLightMarkAgent() {
        delete m_pCtrl;
        m_pCtrl = NULL;
    }
    virtual void procLoop() {
        if (m_nFlag & 1) updatePos();
    }
    virtual void onOutSeq() {
        if (m_nFlag & 1) m_nMoveTime = -1;
    }
#else
    C2DSeqLightMarkAgent(CTask* pParent);
#endif
    void setupSeq(SeqTbl stbl, const CHAR* pszLad, SHORT nUnit, SHORT nLayer, BOOL bFollow);
    void setPos(FVECTOR& vPos);
    void setFollow(BOOL bOn) {
        if (bOn) {
            m_nFlag |= 1;
        } else {
            m_nFlag &= ~1;
        }
    }
    // PARTIAL: added for Form/Menu/SaveLoad
    void setSleep(BOOL bSleep) {
        if (bSleep) {
            m_nFlag |= 2;
        } else {
            m_nFlag &= ~2;
        }
    }

protected:
    void updatePos();
};

class C2DSeqSelectable : public C2DSeqAgent {
private:
    D2::C2DSeqCtrl* m_pCtrl;          /* 0x70 */
    D2::C2DFnt* m_pCaption;           /* 0x74 */
    C2DSeqCursorCtrl* m_pCursor;      /* 0x78 */
    FPOINT m_fptCursorOfs;            /* 0x7c */
    C2DSeqLightMarkAgent* m_pLightMark; /* 0x84 */
    FPOINT m_fptLightOfs;             /* 0x88 */
    SHORT m_nActiveLoopSeq;           /* 0x90 */
    friend class CSaveLoadUI; // PARTIAL: added for Form/Menu/SaveLoad

public:
#ifdef FORM_MENU_SAVELOAD_CPP /* MATCH: FORM_MENU_SAVELOAD_CPP gate, only for its own unit (tested 2026-11-02) */
    /* Inline in the original: emitted (weak, with the vtable) in Form/Menu/SaveLoad, in this order. */
    C2DSeqSelectable(CTask* pParent) : C2DSeqAgent(pParent) {
        m_pCtrl = NULL;
        m_pCaption = NULL;
        m_pCursor = NULL;
        m_pLightMark = NULL;
        m_nActiveLoopSeq = 0;
        m_fptCursorOfs.x = 0.0f;
        m_fptCursorOfs.y = 0.0f;
        m_fptLightOfs.x = 0.0f;
        m_fptLightOfs.y = 0.0f;
    }
    virtual ~C2DSeqSelectable() {
        delete m_pCtrl;
        m_pCtrl = NULL;
    }
    virtual void dispose() {
        delete m_pCtrl;
        m_pCtrl = NULL;
        C2DSeqAgent::dispose();
    }
#else
    C2DSeqSelectable(CTask* pParent);
#endif
    void setupSeq(SeqTbl stbl, const CHAR* pszLad, SHORT nUnit, SHORT nActiveSeq, SHORT nLayer);
    void select(BOOL bSel);
    void setCaptionParam(const CHAR* pszText, INT nSize, const G_RGBA* pCol, UINT nStyle);
    void getCursorPos(FVECTOR& vOut) const;
    void getLightMarkPos(FVECTOR& vOut) const;
    // PARTIAL: added for Form/Menu/SaveLoad
    void attachCursor(C2DSeqCursorCtrl* pCursor) { m_pCursor = pCursor; }
    void attachLightMark(C2DSeqLightMarkAgent* pLight) { m_pLightMark = pLight; }
    void enableDisp(BOOL bDisp) {
        if (m_pCtrl) m_pCtrl->enableDisp(bDisp);
    }
    void enableDispCaption(BOOL bDisp) {
        if (m_pCtrl) m_pCtrl->enableDisp(1, bDisp);
    }
    void setupActiveSeq(SHORT nSeq) { m_nActiveLoopSeq = nSeq; }
    void setCaptionShift(FLOAT fX, FLOAT fY) {
        if (m_pCtrl) m_pCtrl->setFntPos(1, fX, fY);
    }
    inline void setCaptionSize(INT nSize); // defined where D2::C2DFnt is complete (Form/Menu/SaveLoad)
    void setCtrlPos(FVECTOR& vPos) {
        if (m_pCtrl) m_pCtrl->setPos(vPos);
    }
    void setCtrlLocalPos(FVECTOR& vPos) {
        if (m_pCtrl) m_pCtrl->setLocalPos(vPos);
    }
    void setCtrlParent(D2::C2DSeqCtrl* pParent) {
        if (m_pCtrl) m_pCtrl->setParent(pParent);
    }
    inline void setCaptionText(const CHAR* pszText); // defined where D2::C2DFnt is complete (Form/Menu/SaveLoad)
    inline void setCaptionSpace(INT nX, INT nY);     // likewise
    void setCursorOfs(FLOAT fX, FLOAT fY) {
        m_fptCursorOfs.x = fX;
        m_fptCursorOfs.y = fY;
    }
    void setLightMarkOfs(FLOAT fX, FLOAT fY) {
        m_fptLightOfs.x = fX;
        m_fptLightOfs.y = fY;
    }
};

class C2DSeqScrollbarCtrl {
private:
    D2::C2DSeqCtrl* m_pCtrls[3];  /* 0x00 */
    C2DSeqAgent* m_pParts[3];     /* 0x0c */
    D2::C2DSeqCtrl* m_pParentCtrl; /* 0x18 */
    FLOAT m_fBasePosY;            /* 0x1c */
    FLOAT m_fBaseBodySize;        /* 0x20 */
    FLOAT m_fMaxBodySize;         /* 0x24 */
    FLOAT m_fBodyRatio;           /* 0x28 */
    FLOAT m_fScrollPos;           /* 0x2c */
    USHORT m_nAllItemCount;       /* 0x30 */
    USHORT m_nDispItemCount;      /* 0x32 */
    friend class CSaveLoadUI; // PARTIAL: added for Form/Menu/SaveLoad

public:
#ifdef FORM_MENU_SAVELOAD_CPP /* MATCH: FORM_MENU_SAVELOAD_CPP gate, only for its own unit (tested 2026-11-02) */
    inline C2DSeqScrollbarCtrl(CTask* pParent); // PARTIAL: added for Form/Menu/SaveLoad (defined at the end)
    ~C2DSeqScrollbarCtrl() {
        for (INT i = 0; i < 3; i++) {
            delete m_pCtrls[i];
            m_pCtrls[i] = NULL;
        }
        for (INT j = 0; j < 3; j++) {
            if (m_pParts[j]) {
                m_pParts[j]->out();
                m_pParts[j] = NULL;
            }
        }
    }
#endif
    void setupSeq(D2::C2DSeqCtrl* pParent, const C2DSeqAgent::SeqTbl* pTbl, const CHAR* pszLad, SHORT nUnit,
                  SHORT nLayer, SHORT nBodySize);
    void setBasePos(FVECTOR& vPos);
    void setScrollRange(FLOAT fMaxBodySize, USHORT nItemCount, USHORT nDispCount, BOOL bUpdateForm);
    void setScrollPos(USHORT nPos, BOOL bUpdateForm);
    // PARTIAL: added for Form/Menu/SaveLoad
    void startOrRestart() {
        for (INT i = 0; i < 3; i++) {
            m_pParts[i]->startOrRestart();
        }
    }

protected:
    void updateForm();
};

union HM {
    struct {
        USHORT _nHour;
        USHORT _nMinute;
    };
    UINT _nValue;

    void setTimeSec(UINT nSec) {
        _nHour = nSec / 3600;
        if (_nHour >= 1000) {
            _nHour = 999;
            _nMinute = 59;
        } else {
            _nMinute = (nSec - _nHour * 3600) / 60;
        }
    }
};

class C2DSeqTimeCtrl : public D2::C2DSeqCtrl {
private:
    CHAR* m_pszLad;    /* 0x160 */
    SHORT m_nSeqUnit;  /* 0x164 */
    SHORT m_nState;    /* 0x166 */
    SHORT m_nSeqNum0;  /* 0x168 */
    SHORT m_nSeqColon; /* 0x16a */
    SHORT m_nSeqBlank; /* 0x16c */
    SHORT __nFiller;   /* 0x16e */
    HM m_Time;         /* 0x170 */

public:
#ifdef FORM_MENU_SAVELOAD_CPP /* MATCH: FORM_MENU_SAVELOAD_CPP gate, only for its own unit (tested 2026-11-02) */
    C2DSeqTimeCtrl() : C2DSeqCtrl(1) { /* inline in the original (Form/Menu/SaveLoad) */
        m_pszLad = NULL;
        m_nSeqUnit = 0;
        m_nState = 0;
        m_nSeqNum0 = 0;
        m_nSeqColon = 0;
        m_nSeqBlank = 0;
        m_Time._nValue = 0;
    }
#endif
    void setupSeq(const CHAR* pszLad, SHORT nUnit, SHORT nSeqNum0, SHORT nSeqColon, SHORT nSeqBlank);
    void show();
    void hide() { // PARTIAL: added for Form/Menu/SaveLoad
        if (m_nState == 2) enableDisp(FALSE);
        if (m_nState) m_nState = 3;
    }
    BOOL update();
};

class C2DSeqNumericCtrl : public D2::C2DSeqCtrl {
private:
    CHAR* m_pszLad;      /* 0x160 */
    SHORT m_nSeqUnit;    /* 0x164 */
    SHORT m_nState;      /* 0x166 */
    SHORT m_nSeqNum0;    /* 0x168 */
    SHORT m_nSeqBlank;   /* 0x16a */
    UINT m_nValue;       /* 0x16c */
    UINT m_nNewValue;    /* 0x170 */
    UINT m_nMaxValue;    /* 0x174 */
    USHORT m_nMaxFigNum; /* 0x178 */
    SHORT __nFiller;     /* 0x17a */

public:
#ifdef FORM_MENU_SAVELOAD_CPP /* MATCH: FORM_MENU_SAVELOAD_CPP gate, only for its own unit (tested 2026-11-02) */
    C2DSeqNumericCtrl() : C2DSeqCtrl(1) { /* inline in the original (Form/Menu/SaveLoad) */
        m_pszLad = NULL;
        m_nSeqUnit = 0;
        m_nState = 0;
        m_nSeqNum0 = 0;
        m_nSeqBlank = 0;
        m_nValue = 0;
        m_nNewValue = 0;
        m_nMaxValue = 0;
        m_nMaxFigNum = 0;
    }
#endif
    void setupSeq(const CHAR* pszLad, SHORT nUnit, SHORT nSeqNum0, SHORT nSeqBlank, UINT nMaxValue);
    void show();
    void hide() { // PARTIAL: added for Form/Menu/SaveLoad
        if (m_nState == 2) enableDisp(FALSE);
        if (m_nState) m_nState = 3;
    }
    BOOL updateValue(UINT nValue);
};

#ifdef FORM_MENU_SAVELOAD_CPP /* MATCH: FORM_MENU_SAVELOAD_CPP gate, only for its own unit (tested 2026-11-02) */
/* The original header's line 310 is the allocation in this constructor (inlined in Form/Menu/SaveLoad). */
#line 300 SAVELOADCTRL_H_PATH
inline C2DSeqScrollbarCtrl::C2DSeqScrollbarCtrl(CTask* pParent)
    : m_pParentCtrl(NULL), m_fBaseBodySize(1.0f), m_fMaxBodySize(1.0f), m_fBodyRatio(1.0f), m_fScrollPos(0.0f),
      m_nAllItemCount(1), m_nDispItemCount(1) {
    for (INT i = 0; i < 3; i++) {
        m_pCtrls[i] = NULL;
    }
    for (INT j = 0; j < 3; j++) {
        UINT nRA;
        __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA));
        m_pParts[j] = new (com_malloc_d(sizeof(C2DSeqAgent), __FILE__ "(310)", nRA)) C2DSeqAgent(pParent);
    }
}

#else
/* Placeholder for an inline function of the original header at line 310 (an allocation message). */
#line 310 SAVELOADCTRL_H_PATH
inline void* _saveLoadCtrlAlloc310() {
    return com_malloc_d(4, __FILE__ "(310)", (UINT)-1);
}
#endif

#endif /* FORM_MENU_SAVELOADCTRL_H */
