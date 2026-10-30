#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#define D2MAN_H_PATH "c:/KHCOM/MainApp/System/Graphic/2d/2DMan.h"
#include "System/Graphic/Gprim.h"

/* The original's .rodata has GPrim.h's inline strings a second time; string literals of an instantiated template
 * aren't merged with identical earlier ones, so this placeholder template re-emits them (no code). */
template <class T>
struct _GPrimCopy {
    T* m_V;
    void createFV(INT n) {
#line 613 "System/Graphic/GPrim.h"
        m_V = (T*)com_malloc_d(n, __FILE__ "(613)", (UINT)-1);
        if (!m_V) AssertApp(__FILE__, __LINE__, "m_V", 0);
    }
    void createFTV(INT n) {
#line 684
        m_V = (T*)com_malloc_d(n, __FILE__ "(684)", (UINT)-1);
        if (!m_V) AssertApp(__FILE__, __LINE__, "m_V", 0);
    }
};
inline void _gprimCopy(_GPrimCopy<FVERTEX>* p) {
    p->createFV(0);
    p->createFTV(0);
}
#line 19 "c:/KHCOM/MainApp/System/Graphic/2d/2DSeq.cpp"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/2d/2DSeq.h"
#include "System/Graphic/2d/2DSeqObj.h"
#include "System/Graphic/2d/2DFntObj.h"
#include "System/Graphic/2d/2DLay.h"
#include "System/Task.h"
#include <string.h>

inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })



#include "System/RsrcDB.h"




namespace D2 {

class C2DMan { /* System/Graphic/2d/2DMan.h */
public:
    static ISLCManagerT<C2DO>* get2DOSLCManager();
    static C2DO* get2DO(HANDLE hID) {
        SLContainerT<C2DO>* pContainer = get2DOSLCManager()->getContainer(hID);
        return pContainer ? pContainer->getValue() : NULL;
    }
};

/* the .sqd file header (original name unknown) */
struct SQD_HEADER {
    INT nAnimation;     /* 0x20 */
    UINT ofsAnimation;  /* 0x24 */
    INT nSequence;      /* 0x28 */
    UINT ofsSequence;   /* 0x2c */
};

C2DSeqObj* C2DSeq::m_pCurrent2DSeqObj = NULL;

C2DSeq::C2DSeq() {
    makeBaseHandle(D2BASE_TYPE_SEQ);
}

C2DSeq::~C2DSeq() {}

BOOL C2DSeq::init(void* pSqd, const CHAR* pBaseName) {
    if (!pSqd) return FALSE;
    if (!C2DSpr::init(pSqd, pBaseName)) return FALSE;
    SQD_HEADER* pHeader = (SQD_HEADER*)((CHAR*)pSqd + 0x20);
    m_pSeqAnimation = (SEQ_ANIMATION*)((CHAR*)pSqd + pHeader->ofsAnimation);
    m_pSeqSequence = (SEQ_SEQUENCE*)((CHAR*)pSqd + pHeader->ofsSequence);
    m_nAnimationMax = pHeader->nAnimation;
    m_nSequenceMax = pHeader->nSequence;
    return TRUE;
}

HANDLE C2DSeq::create(UINT inNo) {
    if (inNo >= (UINT)m_nSequenceMax) return 0;
    m_pTexInfo = CRsrcDB::getRsrcTex(m_pTexName, NAME_CASE_SENSITIVE);
    if (!m_pTexInfo) m_pTexInfo = CRsrcDB::getRsrcTex("error", NAME_CASE_SENSITIVE);
    SEQ_SEQUENCE* pSeqSequence = &m_pSeqSequence[inNo];
#line 73
    C2DSeqObj* p2DSeqObj = new (com_malloc_d(sizeof(C2DSeqObj), __FILE__ "(73)", RET_ADDR())) C2DSeqObj;
    HANDLE handle = p2DSeqObj->create(getBaseHandle(), m_pTexInfo, pSeqSequence, m_pSeqAnimation, m_pSprGroup,
                                      m_pSprParts, m_pSprSprite, NULL);
    if (!handle) {
        if (p2DSeqObj) delete p2DSeqObj;
        m_pCurrent2DSeqObj = NULL;
    } else {
        m_pCurrent2DSeqObj = p2DSeqObj;
    }
    return handle;
}

HANDLE C2DSeq::create(UINT inNo, SeqControl* pSeqControl) {
    if (inNo >= (UINT)m_nSequenceMax) return 0;
    m_pTexInfo = CRsrcDB::getRsrcTex(m_pTexName, NAME_CASE_SENSITIVE);
    if (!m_pTexInfo) m_pTexInfo = CRsrcDB::getRsrcTex("error", NAME_CASE_SENSITIVE);
    SEQ_SEQUENCE* pSeqSequence = &m_pSeqSequence[inNo];
#line 110
    C2DSeqObj* p2DSeqObj = new (com_malloc_d(sizeof(C2DSeqObj), __FILE__ "(110)", RET_ADDR())) C2DSeqObj(pSeqControl);
    HANDLE handle = p2DSeqObj->create(getBaseHandle(), m_pTexInfo, pSeqSequence, m_pSeqAnimation, m_pSprGroup,
                                      m_pSprParts, m_pSprSprite, NULL);
    if (!handle) {
        if (p2DSeqObj) delete p2DSeqObj;
        m_pCurrent2DSeqObj = NULL;
    }
    m_pCurrent2DSeqObj = p2DSeqObj;
    return handle;
}

HANDLE C2DSeq::create(LAY_SCREEN* pLayScreen) {
    if (pLayScreen->sequence_id >= (UINT)m_nSequenceMax) return 0;
    m_pTexInfo = CRsrcDB::getRsrcTex(m_pTexName, NAME_CASE_SENSITIVE);
    if (!m_pTexInfo) m_pTexInfo = CRsrcDB::getRsrcTex("error", NAME_CASE_SENSITIVE);
    SEQ_SEQUENCE* pSeqSequence = &m_pSeqSequence[pLayScreen->sequence_id];
#line 145
    C2DSeqObj* p2DSeqObj = new (com_malloc_d(sizeof(C2DSeqObj), __FILE__ "(145)", RET_ADDR())) C2DSeqObj;
    HANDLE handle = p2DSeqObj->create(getBaseHandle(), m_pTexInfo, pSeqSequence, m_pSeqAnimation, m_pSprGroup,
                                      m_pSprParts, m_pSprSprite, pLayScreen);
    if (!handle) {
        if (p2DSeqObj) delete p2DSeqObj;
        m_pCurrent2DSeqObj = NULL;
    } else {
        m_pCurrent2DSeqObj = p2DSeqObj;
    }
    return handle;
}

HANDLE C2DSeq::createSeq(UINT inUnitNo, UINT inNo, SeqControl* pSeqControl) {
    if (inNo >= (UINT)m_nSequenceMax) return 0;
    m_pTexInfo = CRsrcDB::getRsrcTex(m_pTexName, NAME_CASE_SENSITIVE);
    if (!m_pTexInfo) m_pTexInfo = CRsrcDB::getRsrcTex("error", NAME_CASE_SENSITIVE);
    SEQ_SEQUENCE* pSeqSequence = &m_pSeqSequence[inNo];
#line 184
    C2DSeqObj* p2DSeqObj = new (com_malloc_d(sizeof(C2DSeqObj), __FILE__ "(184)", RET_ADDR())) C2DSeqObj(pSeqControl);
    HANDLE handle = p2DSeqObj->create(getBaseHandle(), m_pTexInfo, pSeqSequence, m_pSeqAnimation, m_pSprGroup,
                                      m_pSprParts, m_pSprSprite, NULL);
    if (!handle) {
        if (p2DSeqObj) delete p2DSeqObj;
        m_pCurrent2DSeqObj = NULL;
    } else {
        m_pCurrent2DSeqObj = p2DSeqObj;
    }
    return handle;
}

HANDLE C2DSeq::createFnt(C2DFnt* p2DFnt, SeqControl* pSeqControl) {
#line 211
    C2DSeqObj* p2DSeqObj = new (com_malloc_d(sizeof(C2DSeqObj), __FILE__ "(211)", RET_ADDR())) C2DSeqObj(pSeqControl);
    HANDLE handle = p2DSeqObj->createFnt(getBaseHandle(), p2DFnt);
    if (!handle) {
        if (p2DSeqObj) delete p2DSeqObj;
        m_pCurrent2DSeqObj = NULL;
    } else {
        m_pCurrent2DSeqObj = p2DSeqObj;
    }
    return handle;
}

HANDLE C2DSeq::replaceSeq(HANDLE hCurrent, UINT inNo, SeqControl* pSeqControl) {
    if (inNo >= (UINT)m_nSequenceMax) return 0;
    if (!hCurrent) return 0;
    C2DSeqObj* pCurrent = (C2DSeqObj*)C2DMan::get2DO(hCurrent);
    if (!pCurrent) return 0;
    m_pTexInfo = CRsrcDB::getRsrcTex(m_pTexName, NAME_CASE_SENSITIVE);
    if (!m_pTexInfo) m_pTexInfo = CRsrcDB::getRsrcTex("error", NAME_CASE_SENSITIVE);
    SEQ_SEQUENCE* pSeqSequence = &m_pSeqSequence[inNo];
#line 245
    C2DSeqObj* p2DSeqObj = new (com_malloc_d(sizeof(C2DSeqObj), __FILE__ "(245)", RET_ADDR())) C2DSeqObj(pSeqControl);
    HANDLE handle = p2DSeqObj->create(getBaseHandle(), m_pTexInfo, pSeqSequence, m_pSeqAnimation, m_pSprGroup,
                                      m_pSprParts, m_pSprSprite, NULL);
    if (!handle) {
        if (p2DSeqObj) delete p2DSeqObj;
        m_pCurrent2DSeqObj = NULL;
    } else {
        m_pCurrent2DSeqObj = p2DSeqObj;
        handle = replaceList(hCurrent, *p2DSeqObj);
        if (handle == -1) {
            TRACE_COL(_TTY_YELLOW, "C2DSeq::replaceSeq\x8e\xb8\x94\x73\x81\x49\n");
            SLContainerT<C2DO>* pContainer = C2DMan::get2DOSLCManager()->getContainer(hCurrent);
            if (pContainer) {
                pContainer->setValue(NULL);
                if (p2DSeqObj) delete p2DSeqObj;
                m_pCurrent2DSeqObj = NULL;
            }
            handle = 0;
        } else {
            p2DSeqObj->enableDisp(pCurrent->isDisp());
        }
        if (pCurrent) delete pCurrent;
    }
    return handle;
}

HANDLE C2DSeq::replaceFnt(HANDLE hCurrent, C2DFnt* p2DFnt, SeqControl* pSeqControl) {
    if (!hCurrent) return 0;
    C2DSeqObj* pCurrent = (C2DSeqObj*)C2DMan::get2DO(hCurrent);
    if (!pCurrent) return 0;
#line 287
    C2DSeqObj* p2DSeqObj = new (com_malloc_d(sizeof(C2DSeqObj), __FILE__ "(287)", RET_ADDR())) C2DSeqObj(pSeqControl);
    HANDLE handle = p2DSeqObj->createFnt(getBaseHandle(), p2DFnt);
    if (!handle) {
        if (p2DSeqObj) delete p2DSeqObj;
        m_pCurrent2DSeqObj = NULL;
    } else {
        m_pCurrent2DSeqObj = p2DSeqObj;
        handle = replaceList(hCurrent, *p2DSeqObj);
        if (handle == -1) {
            TRACE_COL(_TTY_YELLOW, "C2DSeq::replaceFnt\x8e\xb8\x94\x73\x81\x49\n");
            SLContainerT<C2DO>* pContainer = C2DMan::get2DOSLCManager()->getContainer(hCurrent);
            if (pContainer) {
                pContainer->setValue(NULL);
                if (p2DSeqObj) delete p2DSeqObj;
                m_pCurrent2DSeqObj = NULL;
            }
            handle = 0;
        } else {
            p2DSeqObj->enableDisp(pCurrent->isDisp());
            C2DFntObj* pCurrentFnt = (C2DFntObj*)pCurrent->get2DObjList()->getHead();
            C2DFntObj* pFnt = (C2DFntObj*)p2DSeqObj->get2DObjList()->getHead();
            pCurrentFnt->clear2DFnt();
            pFnt->setPos(pCurrentFnt->getPos());
            pFnt->setCol(pCurrentFnt->getCol());
        }
        if (pCurrent) delete pCurrent;
    }
    return handle;
}

HANDLE C2DSeq::createSpr(UINT inUnitNo, UINT inNo) {
    return C2DSpr::createSpr(inUnitNo, inNo);
}

TIMEQ C2DSeq::getTime(HANDLE handle) {
    C2DSeqObj* p2DSeqObj = (C2DSeqObj*)C2DMan::get2DO(handle);
    return p2DSeqObj ? p2DSeqObj->getTime() : 0;
}

TIMEQ C2DSeq::getTimeMax(HANDLE handle) {
    C2DSeqObj* p2DSeqObj = (C2DSeqObj*)C2DMan::get2DO(handle);
    return p2DSeqObj ? p2DSeqObj->getTimeMax() : 0;
}

BOOL C2DSeq::isEnd(HANDLE handle) {
    C2DSeqObj* p2DSeqObj = (C2DSeqObj*)C2DMan::get2DO(handle);
    return p2DSeqObj ? p2DSeqObj->isEnd() : FALSE;
}

}  // namespace D2

// SOURCE DATA: .data .rodata
