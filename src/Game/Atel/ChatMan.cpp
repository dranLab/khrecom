#define GPRIM_H_PATH "System/Graphic/gprim.h"
#define CHATMAN_H_PATH "c:/KHCOM/MainApp/Game/Atel/ChatMan.h"
#include "System/TaskMan.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/HeapMan.h"
#include "System/MainApp.h"
#include "System/SJoint.h"
#include "System/Component.h"
#include "System/Vector.h"

inline void* operator new(size_t, void* p) throw() { return p; }

#include "Game/Atel/ChatMan.h"

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new(size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}

/* The window classes (original headers unknown): only what this unit uses. */
#include "System/Wnd.h"














#include "System/Wnd.h"












































#include "System/Wnd.h"














template <class T> static inline T& _ref(T& v) { return v; }

#define SJ_FOREACH(T, pEntry, list) \
    for (T *pEntry = (list).getHead(), *__pn = pEntry ? pEntry->getNext() : NULL; pEntry; \
         pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL)

namespace ATEL {

class CChatWndDt : public CSJointT<CChatWndDt> {
public:
    CBaseWnd* m_pBaseWnd; /* 0x08 */
    INT m_nID;            /* 0x0c */
    INT m_status;         /* 0x10 */
    INT m_kind;           /* 0x14 */

    virtual ~CChatWndDt();
};

class CChatWndDtList : public CSJointListT<CChatWndDt> {
public:
    ~CChatWndDtList() {
        SJ_FOREACH(CChatWndDt, __p, *this) delete __p;
        resetParam();
    }
};

struct ChatWndParam {
    CPointI pos;         /* 0x00 */
    CSizeI size;         /* 0x08 */
    UINT a;              /* 0x10 */
    const CHAR* pText;   /* 0x14 */
    UINT b;              /* 0x18 */
    UINT c;              /* 0x1c */
    UINT d;              /* 0x20 */
    CPointI pt;          /* 0x24 */
    UINT e;              /* 0x2c */
    UINT f;              /* 0x30 */
};

struct ChatWndCTDParam {
    CPointI pos;   /* 0x00 */
    CPointI pos2;  /* 0x08 */
    UINT nCtdID;   /* 0x10 */
    UINT a;        /* 0x14 */
    UINT* pB;      /* 0x18 */
    UINT c;        /* 0x1c */
};

struct ImageWndParam {
    CPointI pos;       /* 0x00 */
    CSizeI size;       /* 0x08 */
    CPointI pos2;      /* 0x10 */
    CSizeI size2;      /* 0x18 */
    const CHAR* pName; /* 0x20 */
    INT nFlag;         /* 0x24 */
    FLOAT fRate;       /* 0x28 */
    G_RGBA col0;       /* 0x2c */
    G_RGBA col1;       /* 0x30 */
};

CChatMan* CChatMan::m_pInstance = NULL;
CChatWndDtList* CChatMan::m_pCChatWndDtList = NULL;
INT CChatMan::m_uniqueID = 256;

CChatMan::CChatMan() : CTask(CTaskMan::getSysTask(0), 0x4000) {
    setPriority(PRI_BIT_ALWAYS);
    ignorePause(TRUE);
    ignorePlaySpeed(TRUE);
}

CChatMan::~CChatMan() {
    dispose();
    m_pInstance = NULL;
}

void CChatMan::setup() {
    if (!m_pCChatWndDtList) {
        m_pCChatWndDtList = new (__FILE__ "(72)") CChatWndDtList;
    }
}

void CChatMan::dispose() {
    delete m_pCChatWndDtList;
    m_pCChatWndDtList = NULL;
}

CChatWndDt::~CChatWndDt() {}

INT CChatMan::onCreateChatWindow(INT nCmd, void* pParam, INT nID) {
    return createChatWindow(0, pParam, nID, 0);
}

INT CChatMan::onCreateChatWindowC(INT nCmd, void* pParam, INT nID) {
    return createChatWindow(0, pParam, nID, 1);
}

INT CChatMan::onCreateChatWindowCTD(INT nCmd, void* pParam, INT nID) {
    if (searchChatWndDt(nID)) {
        return -1;
    }
    CTextWnd* pTextWnd = new (__FILE__ "(156)") CTextWnd(*this, 0);
    ChatWndCTDParam* p = (ChatWndCTDParam*)pParam;
    pTextWnd->createWindowCTD(p->nCtdID, &p->pos, &p->pos2, p->a, p->pB, p->c);
    CChatWndDt* pChatWndDt = new (__FILE__ "(161)") CChatWndDt;
    pChatWndDt->m_pBaseWnd = pTextWnd;
    if (nID >= 0) {
        pChatWndDt->m_nID = nID;
    } else {
        if (m_uniqueID < 0) {
            m_uniqueID = 256;
        }
        pChatWndDt->m_nID = m_uniqueID++;
    }
    pChatWndDt->m_status = 0;
    pChatWndDt->m_kind = 0;
    _ref(m_pCChatWndDtList)->pushBack(*pChatWndDt);
    return pChatWndDt->m_nID;
}

INT CChatMan::createChatWindow(INT nCmd, void* pParam, INT nID, INT fg) {
    if (searchChatWndDt(nID)) {
        return -1;
    }
    CTextWnd* pTextWnd = new (__FILE__ "(237)") CTextWnd(*this, 0);
    ChatWndParam* p = (ChatWndParam*)pParam;
    if (!fg) {
        pTextWnd->createWindow(p->pos, p->size, p->a, p->pText, p->b, p->c, p->d, p->pt, p->e, p->f);
    } else {
        pTextWnd->createWindowC(p->pos, p->size, p->a, p->pText, p->b, p->c, p->d, p->pt, p->e, p->f);
    }
    CChatWndDt* pChatWndDt = new (__FILE__ "(246)") CChatWndDt;
    pChatWndDt->m_pBaseWnd = pTextWnd;
    if (nID >= 0) {
        pChatWndDt->m_nID = nID;
    } else {
        if (m_uniqueID < 0) {
            m_uniqueID = 256;
        }
        pChatWndDt->m_nID = m_uniqueID++;
    }
    pChatWndDt->m_status = 0;
    pChatWndDt->m_kind = 0;
    _ref(m_pCChatWndDtList)->pushBack(*pChatWndDt);
    return pChatWndDt->m_nID;
}

INT CChatMan::onDestroyChatWindow(INT nCmd, void* pParam, INT nID) {
    CChatWndDt* pChatWndDt = searchChatWndDt(nID);
    if (!pChatWndDt) {
        return -1;
    }
    m_pCChatWndDtList->remove(*pChatWndDt);
    if (pChatWndDt->m_kind < 2) {
        if (!pParam || !*(INT*)pParam) {
            delete pChatWndDt->m_pBaseWnd;
        } else {
            ((CTextWnd*)pChatWndDt->m_pBaseWnd)->closeDestroy();
        }
    } else {
        delete pChatWndDt->m_pBaseWnd;
    }
    delete pChatWndDt;
    return 1;
}

INT CChatMan::onDestroyChatWindowAll(INT nCmd, void* pParam, INT nID) {
    SJ_FOREACH(CChatWndDt, pEntry, *m_pCChatWndDtList) {
        m_pCChatWndDtList->remove(*pEntry);
        delete pEntry->m_pBaseWnd;
        delete pEntry;
    }
    return 1;
}

INT CChatMan::onSetLayerTutorial(INT nCmd, void* pParam, INT nID) {
    CBaseWnd::setLayerFG(nID ? 0x18 : 9);
    return 1;
}

CChatWndDt* CChatMan::searchChatWndDt(INT nID) {
    SJ_FOREACH(CChatWndDt, pEntry, *m_pCChatWndDtList) {
        if (pEntry->m_nID == nID) {
            return pEntry;
        }
    }
    return NULL;
}

INT CChatMan::onOpen(INT nCmd, void* pParam, INT nID) {
    CChatWndDt* pChatWndDt = searchChatWndDt(nID);
    if (!pChatWndDt || pChatWndDt->m_kind >= 2) {
        return -1;
    }
    ((CTextWnd*)pChatWndDt->m_pBaseWnd)->open();
    return 1;
}

INT CChatMan::onOpenDelay(INT nCmd, void* pParam, INT nID) {
    if (onOpen(0, NULL, nID) == -1) {
        return -1;
    }
    ((CTextWnd*)searchChatWndDt(nID)->m_pBaseWnd)->setOpenDelay(*(UINT*)pParam);
    return 1;
}

INT CChatMan::onClose(INT nCmd, void* pParam, INT nID) {
    CChatWndDt* pChatWndDt = searchChatWndDt(nID);
    if (!pChatWndDt || pChatWndDt->m_kind >= 2) {
        return -1;
    }
    ((CTextWnd*)pChatWndDt->m_pBaseWnd)->close();
    return 1;
}

INT CChatMan::onCheckStatus(INT nCmd, void* pParam, INT nID) {
    if (!pParam) {
        return -1;
    }
    CChatWndDt* pChatWndDt = searchChatWndDt(nID);
    if (!pChatWndDt) {
        return -1;
    }
    *(INT*)pParam = pChatWndDt->m_pBaseWnd->getStatus();
    return 1;
}

INT CChatMan::onCheckPhaseCnt(INT nCmd, void* pParam, INT nID) {
    if (!pParam) {
        return -1;
    }
    CChatWndDt* pChatWndDt = searchChatWndDt(nID);
    if (!pChatWndDt) {
        return -1;
    }
    *(INT*)pParam = pChatWndDt->m_pBaseWnd->getPhaseCnt();
    return 1;
}

#define TEXT_WND_COMMAND(name, call)                                  \
    INT CChatMan::name(INT nCmd, void* pParam, INT nID) {             \
        CChatWndDt* pChatWndDt = searchChatWndDt(nID);                \
        if (!pChatWndDt || pChatWndDt->m_kind != 0) {                 \
            return -1;                                                \
        }                                                             \
        ((CTextWnd*)pChatWndDt->m_pBaseWnd)->call;                    \
        return 1;                                                     \
    }

TEXT_WND_COMMAND(onClearText, clearText())
TEXT_WND_COMMAND(onSetText, setText((const CHAR*)pParam, NULL))
TEXT_WND_COMMAND(onSetTextCTD, setTextCTD(((ChatWndCTDParam*)pParam)->nCtdID, ((ChatWndCTDParam*)pParam)->pB))
TEXT_WND_COMMAND(onChangeTextNext, changeTextNext((UINT*)pParam))
TEXT_WND_COMMAND(onSetTextDelay, setTextDispDelay(*(INT*)pParam))
TEXT_WND_COMMAND(onSetWndStyle, setWndStyle(*(UINT*)pParam))
TEXT_WND_COMMAND(onChangeWndStyle, changeWndStyle(*(UINT*)pParam))
TEXT_WND_COMMAND(onSetWndRate, setWndRate(*(FVECTOR*)pParam))
TEXT_WND_COMMAND(onSetTextMargin, setTextOffset(*(CPointI*)pParam))
TEXT_WND_COMMAND(onSetFontSize, setFontSize(*(UINT*)pParam))
TEXT_WND_COMMAND(onSetFontSpace, setFontSpace(*(CSizeI*)pParam))
TEXT_WND_COMMAND(onSetTextStyle, setFontStyle(*(UINT*)pParam))
TEXT_WND_COMMAND(onSetTextColor, setTextColor(*(UINT*)pParam))
TEXT_WND_COMMAND(onSetTextEXColor, setTextExColor(*(UINT*)pParam))
TEXT_WND_COMMAND(onChangeTextSelect, changeTextSelect(*(INT*)pParam, 1))
TEXT_WND_COMMAND(onEnableLFDisp, enableLFDisp(*(INT*)pParam))
TEXT_WND_COMMAND(onChangeCursorColor, changeCursorColor(*(UINT*)pParam))
TEXT_WND_COMMAND(onSetDispInterval, setDispInterval(*(UINT*)pParam))
TEXT_WND_COMMAND(onSetSelectOffset, setSelectOffset(*(UINT*)pParam))
TEXT_WND_COMMAND(onChangeText, changeText((const CHAR*)pParam, (UINT*)NULL))
TEXT_WND_COMMAND(onChangeTextCTD, changeTextCTD(((ChatWndCTDParam*)pParam)->nCtdID, ((ChatWndCTDParam*)pParam)->pB))
TEXT_WND_COMMAND(onEnableDisp, enableWndDisp(*(UINT*)pParam))
TEXT_WND_COMMAND(onSetCharFadeInDelay, setCharFadeInDelay(*(FLOAT*)pParam))
TEXT_WND_COMMAND(onSetTextFadeOutTime, setTextFadeOutTime(*(FLOAT*)pParam))
TEXT_WND_COMMAND(onEnableDisp0, enableDisp(*(UINT*)pParam))
TEXT_WND_COMMAND(onSetWords, setWords((UINT*)pParam))

INT CChatMan::onSetSystemPause(INT nCmd, void* pParam, INT nID) {
    CMainApp::setSysPause(*(INT*)pParam);
    return 1;
}

INT CChatMan::onCreateImageWindow(INT nCmd, void* pParam, INT nID) {
    if (searchChatWndDt(nID)) {
        return -1;
    }
    CImageWnd* pImageWnd = new (__FILE__ "(742)") CImageWnd(*CTaskMan::getSysTask(0));
    ImageWndParam* p = (ImageWndParam*)pParam;
    pImageWnd->createWindow(p->pos, p->size, p->pos2, p->size2, p->pName, p->fRate, p->col0, p->col1,
                            (p->nFlag & 6) >> 1);
    if (p->nFlag & 1) {
        pImageWnd->setDestroy();
        return 1;
    }
    CChatWndDt* pChatWndDt = new (__FILE__ "(753)") CChatWndDt;
    pChatWndDt->m_pBaseWnd = pImageWnd;
    if (nID >= 0) {
        pChatWndDt->m_nID = nID;
    } else {
        if (m_uniqueID < 0) {
            m_uniqueID = 256;
        }
        pChatWndDt->m_nID = m_uniqueID++;
    }
    pChatWndDt->m_status = 0;
    pChatWndDt->m_kind = 2;
    _ref(m_pCChatWndDtList)->pushBack(*pChatWndDt);
    return pChatWndDt->m_nID;
}

INT CChatMan::onInitPhaseImageWindow(INT nCmd, void* pParam, INT nID) {
    CChatWndDt* pChatWndDt = searchChatWndDt(nID);
    if (!pChatWndDt) {
        return -1;
    }
    if (!pChatWndDt->m_kind) {
        return -1;
    }
    CImageWnd* pImageWnd = (CImageWnd*)pChatWndDt->m_pBaseWnd;
    ImageWndParam* p = (ImageWndParam*)pParam;
    pImageWnd->init(p->pos, p->size, p->pos2, p->size2, p->pName, p->fRate, p->col0, p->col1, (p->nFlag & 6) >> 1);
    if (p->nFlag & 1) {
        pImageWnd->setDestroy();
        _ref(m_pCChatWndDtList)->remove(*pChatWndDt);
        delete pChatWndDt;
    }
    return 1;
}

void CChatMan::_destroyAllWindow() {
    SJ_FOREACH(CChatWndDt, pEntry, *m_pCChatWndDtList) {
        m_pCChatWndDtList->remove(*pEntry);
        delete pEntry->m_pBaseWnd;
        delete pEntry;
    }
}

} // namespace ATEL

// SOURCE DATA: .data .rodata
