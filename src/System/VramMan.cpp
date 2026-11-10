#define MODELDATA_H_TEXINFO_CTOR /* MATCH: reqTransTex declares a TexInfo local (constructor memsets it) */
#include "System/VramMan.h"
#include "System/Graphic/Gprim.h"
#define MODEL_H_PATH "System/Graphic/model.h" /* MATCH: this unit names Model.h so (header default "System/Graphic/Model.h") */
#include "System/Graphic/Model.h"
#include "System/HeapMan.h"
#include "System/MainApp.h"
#include "System/Debug.h"
#include "System/Thread.h"
#include "System/Buf.h"
#include "System/SJoint.h"

#line 24





struct SemaParam {
    int currentCount;
    int maxCount;
    int initCount;
    int numWaitThreads;
    unsigned int attr;
    unsigned int option;
};

extern "C" {
int CreateSema(SemaParam* param);
int DeleteSema(int sid);
int SignalSema(int sid);
int WaitSema(int sid);
}

extern INT SCREEN_HEIGHT;
extern INT DISP_HEIGHT;

#include "System/Graphic/TexInfo.h"














class CTexAnm;





















#include "System/Graphic/PackMan.h"




#line 30
class CVtransReq : public CSJointT<CVtransReq> {
private:
    TexInfo m_TexInfo;
    CBuf* m_pSrcData;
    BOOL m_bTrans;

public:
    CVtransReq(const TexInfo& inTexInfo, CBuf& inSrc) : m_bTrans(FALSE) {
        m_TexInfo = inTexInfo;
        m_pSrcData = new KH_AT("(43)") CBuf;
        if (m_pSrcData) inSrc.copyTo(*m_pSrcData);
    }
    virtual ~CVtransReq() {
        delete m_pSrcData;
        m_pSrcData = NULL;
    }
    TexInfo& getTexInfo() { return m_TexInfo; }
    void transStarted() { m_bTrans = TRUE; }
    BOOL isNewReq() { return !m_bTrans; }
};

typedef CSJointListT<CVtransReq> CVtransQueue;

#define FOREACH(T, p, list) \
    for (T* p = (list).getHead(), *__pn = (p ? p->getNext() : NULL); p != NULL; \
         __pn = ((p = __pn) != NULL ? p->getNext() : NULL))

namespace {

struct _MallocUnit {
    USHORT _nStartPos;
    USHORT _nEndPos;
    USHORT _nSize;
    USHORT _nPadding;
    _MallocUnit* _pPrev;
    _MallocUnit* _pNext;
};

CVtransQueue s_TransQueue;
_MallocUnit* s_pPreFixedTexPtr;
_MallocUnit* s_pFixedTexPtr;
_MallocUnit* s_pFontTexPtr;
_MallocUnit* s_pPreFixedClutPtr;
_MallocUnit* s_pFixedClutPtr;
_MallocUnit* s_pFontClutPtr;
/* volatile: the debug info drops the qualifier, but the code keeps these accesses out of delay slots */
volatile INT s_nAllocSema;
volatile INT s_nTransSema;
BOOL s_bHalt;
INT s_nThreadID;
volatile INT s_nTexTransStatus;

}

static void _release(_MallocUnit** ppInPtr);
static void _release(_MallocUnit** ppInPtr, UINT nPos);
static _MallocUnit* _createUnit(UINT nStartPos, UINT nSize, _MallocUnit* pPrev, _MallocUnit* pNext);
static INT _allocSub(_MallocUnit*& pAddr, UINT nStartPos, UINT nEndPos, UINT nSize, const CHAR* pszType);
static void _compact(_MallocUnit** ppUnit);
static void _dbgReportAllocatedUnit(const _MallocUnit* pUnit);
#line 146

INT VRAM::_FRAME_SIZE;
INT VRAM::_FRAME_AREA_END;
INT VRAM::_FRAME0_START;
INT VRAM::_FRAME0_END;
INT VRAM::_FRAME1_START;
INT VRAM::_FRAME1_END;
INT VRAM::_ZBUF_START;
INT VRAM::_ZBUF_SIZE;
INT VRAM::_ZBUF_END;
INT VRAM::_FONT_AREA_START;
INT VRAM::_FONT_AREA_END;
INT VRAM::_TEX_AREA_START;
INT VRAM::_TEX_TMP1_START;
INT VRAM::_TEX_TMP1_SIZE;
INT VRAM::_TEX_TMP1_END;
INT VRAM::_TEX_TMP2_START;
INT VRAM::_TEX_TMP2_SIZE;
INT VRAM::_TEX_TMP2_END;
INT VRAM::_TEX_PREFIX_START;
INT VRAM::_TEX_PREFIX_SIZE;
INT VRAM::_TEX_PREFIX_END;
INT VRAM::_TEX_FIX_START;
INT VRAM::_TEX_FIX_SIZE;
INT VRAM::_TEX_FIX_END;
INT VRAM::_TEX_AREA_END;
INT VRAM::_TEX_AREA_SIZE;
INT VRAM::_CLUT_AREA_START;
INT VRAM::_CLUT_AREA_END;
INT VRAM::_CLUT_AREA_SIZE;
INT VRAM::_CLUT_TMP1_START;
INT VRAM::_CLUT_TMP1_SIZE;
INT VRAM::_CLUT_TMP1_END;
INT VRAM::_CLUT_TMP2_START;
INT VRAM::_CLUT_TMP2_SIZE;
INT VRAM::_CLUT_TMP2_END;
INT VRAM::_CLUT_FONT_START;
INT VRAM::_CLUT_FONT_SIZE;
INT VRAM::_CLUT_FONT_END;
INT VRAM::_CLUT_PREFIX_START;
INT VRAM::_CLUT_PREFIX_SIZE;
INT VRAM::_CLUT_PREFIX_END;
INT VRAM::_CLUT_FIX_START;
INT VRAM::_CLUT_FIX_END;
INT VRAM::_CLUT_FIX_SIZE;
INT VRAM::_FRAME0_SIZE;
INT VRAM::_FRAME1_SIZE;

void CVramMan::initialize(INT nThreadID) {
    SemaParam sparam;

    s_nThreadID = nThreadID;
    s_pPreFixedTexPtr = NULL;
    s_pFixedTexPtr = NULL;
    s_pFontTexPtr = NULL;
    s_pPreFixedClutPtr = NULL;
    s_pFixedClutPtr = NULL;
    s_pFontClutPtr = NULL;
    s_nTexTransStatus = 0;
    s_bHalt = FALSE;
    sparam.initCount = 1;
    sparam.maxCount = 1;
    s_nAllocSema = CreateSema(&sparam);
    s_nTransSema = CreateSema(&sparam);
}

static void _clearTransQueue() {
    WaitSema(s_nTransSema);
    FOREACH(CVtransReq, __p, s_TransQueue) {
        delete __p;
    }
    s_TransQueue.resetParam();
    SignalSema(s_nTransSema);
}

void CVramMan::finalize() {
    s_bHalt = TRUE;
    _clearTransQueue();
    releaseAll();
    DeleteSema(s_nTransSema);
    DeleteSema(s_nAllocSema);
}

void CVramMan::halt(BOOL bFlag) {
    s_bHalt = bFlag;
}

static INT _allocTex(UINT nSize, INT nType);
static INT _allocClut(UINT nSize, INT nType);

INT CVramMan::allocTex(UINT nTexSize, UINT nClutSize, INT& nOutTex, INT& nOutClut, INT nType) {
    if (nType == 3) {
        nOutTex = 0;
        nOutClut = 0;
    } else {
        if ((nTexSize | nClutSize) == 0) return -1;
        if (nTexSize != 0) {
            nOutTex = _allocTex(nTexSize, nType);
            if (nOutTex < 0) return -1;
        }
        if (nClutSize != 0) {
            nOutClut = _allocClut(nClutSize, nType);
            if (nOutClut < 0) {
                if (nTexSize != 0) {
                    releaseTex(nOutTex, nType);
                    nOutTex = -1;
                    return -1;
                }
            }
        }
    }
    return 0;
}

void CVramMan::release(UINT nVpos, UINT nCpos, INT nType) {
    releaseTex(nVpos, nType);
    releaseClut(nCpos, nType);
}

static void _release(_MallocUnit** ppInPtr) {
    if (ppInPtr) {
        WaitSema(s_nAllocSema);
        _MallocUnit* pUnit = *ppInPtr;
        while (pUnit) {
            _MallocUnit* pNext = pUnit->_pNext;
            delete pUnit;
            pUnit = pNext;
        }
        *ppInPtr = NULL;
        SignalSema(s_nAllocSema);
    }
}

static void _release(_MallocUnit** ppInPtr, UINT nPos) {
    if (ppInPtr) {
        WaitSema(s_nAllocSema);
        _MallocUnit* pUnit;
        _MallocUnit* pPrev;
        _MallocUnit* pPrevPrev;
        for (pUnit = *ppInPtr; pUnit; pUnit = pUnit->_pNext) {
            if (nPos == pUnit->_nStartPos) {
                if (pUnit->_pNext == NULL) {
                    if (pUnit->_pPrev == NULL) {
                        *ppInPtr = NULL;
                    } else if (pUnit == *ppInPtr) {
                        *ppInPtr = NULL;
                    } else {
                        pPrev = pUnit->_pPrev;
                        pPrev->_pNext = NULL;
                        while (pPrev->_nSize == 0) {
                            _MallocUnit* pPrevPrev = pPrev->_pPrev;
                            if (pPrevPrev) pPrevPrev->_pNext = NULL;
                            delete pPrev;
                            if (pPrevPrev == NULL) {
                                *ppInPtr = NULL;
                                break;
                            }
                            if (pPrev == *ppInPtr) {
                                *ppInPtr = NULL;
                                break;
                            }
                            pPrev = pPrevPrev;
                        }
                    }
                    delete pUnit;
                } else {
                    pUnit->_nSize = 0;
                }
                break;
            }
        }
        SignalSema(s_nAllocSema);
    }
}

void CVramMan::releaseAll() {
    _release(&s_pPreFixedTexPtr);
    _release(&s_pFixedTexPtr);
    _release(&s_pFontTexPtr);
    _release(&s_pPreFixedClutPtr);
    _release(&s_pFixedClutPtr);
    _release(&s_pFontClutPtr);
}

void CVramMan::releaseTex(UINT nPos, INT nType) {
    switch (nType) {
    case -1:
        _release(&s_pPreFixedTexPtr, nPos);
        _release(&s_pFixedTexPtr, nPos);
        _release(&s_pFontTexPtr, nPos);
        break;
    case 1:
        _release(&s_pPreFixedTexPtr, nPos);
        break;
    case 0:
        _release(&s_pFixedTexPtr, nPos);
        break;
    case 2:
        _release(&s_pFontTexPtr, nPos);
        break;
    }
}

void CVramMan::releaseClut(UINT nPos, INT nType) {
    switch (nType) {
    case -1:
        _release(&s_pPreFixedClutPtr, nPos);
        _release(&s_pFixedClutPtr, nPos);
        _release(&s_pFontClutPtr, nPos);
        break;
    case 1:
        _release(&s_pPreFixedClutPtr, nPos);
        break;
    case 0:
        _release(&s_pFixedClutPtr, nPos);
        break;
    case 2:
        _release(&s_pFontClutPtr, nPos);
        break;
    }
}

#line 359
static _MallocUnit* _createUnit(UINT nStartPos, UINT nSize, _MallocUnit* pPrev, _MallocUnit* pNext) {
    _MallocUnit* pUnit;
    pUnit = new KH_AT("(361)") _MallocUnit;
    if (!pUnit) AssertApp(__FILE__, __LINE__, "pUnit", 0);
    pUnit->_nStartPos = nStartPos;
    pUnit->_nEndPos = nStartPos + nSize;
    pUnit->_nSize = nSize;
    pUnit->_pNext = pNext;
    pUnit->_pPrev = pPrev;
    return pUnit;
}

static INT _allocSub(_MallocUnit*& pAddr, UINT nStartPos, UINT nEndPos, UINT nSize, const CHAR* pszType) {
    if (nSize == 0) return -1;
    INT nPos = -1;
    _MallocUnit* pUnit;
    WaitSema(s_nAllocSema);
    if (pAddr == NULL) {
        pAddr = _createUnit(nStartPos, nSize, NULL, NULL);
        nPos = nStartPos;
    } else {
        for (pUnit = pAddr; pUnit->_pNext; pUnit = pUnit->_pNext) {
            if (pUnit->_nSize == 0 && (UINT)(pUnit->_nEndPos - pUnit->_nStartPos) >= nSize) {
                INT nOdd = pUnit->_pNext->_nStartPos - pUnit->_nEndPos;
                if (nOdd > 0) pUnit->_pNext = _createUnit(pUnit->_nEndPos, nOdd, pUnit, pUnit->_pNext);
                pUnit->_nSize = nSize;
                nPos = pUnit->_nStartPos;
                break;
            }
        }
        if (nPos < 0 && pUnit->_nEndPos + nSize <= nEndPos) {
            pUnit->_pNext = _createUnit(pUnit->_nEndPos, nSize, pUnit, NULL);
            nPos = pUnit->_nEndPos;
        }
    }
    SignalSema(s_nAllocSema);
    if (nPos < 0) {
        TRACE("\x81\x49\x81\x49\x81\x49\x81\x49 %s \x97\xcc\x88\xe6\x8a\x6d\x95\xdb(size = 0x%x) \x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\x81\x42\x81\x49\x81\x49\x81\x49\x81\x49\n",
              pszType, nSize);
    }
    return nPos;
}

#line 450
static INT _allocTex(UINT nSize, INT nType) {
    INT nStartPos;
    INT nEndPos;
    _MallocUnit** ppAddr = NULL;
    switch (nType) {
    case 1:
        ppAddr = &s_pPreFixedTexPtr;
        nStartPos = VRAM::_TEX_PREFIX_START;
        nEndPos = VRAM::_TEX_PREFIX_END;
        break;
    case 0:
        ppAddr = &s_pFixedTexPtr;
        nStartPos = VRAM::_TEX_FIX_START;
        nEndPos = VRAM::_TEX_FIX_END;
        break;
    case 2:
        ppAddr = &s_pFontTexPtr;
        nStartPos = VRAM::_FRAME1_START;
        nEndPos = VRAM::_ZBUF_END;
        nSize *= 4;
        break;
    }
#line 466
    if (!ppAddr) AssertApp(__FILE__, __LINE__, "ppAddr", 0);
    return _allocSub(*ppAddr, nStartPos, nEndPos, nSize, "TEX");
}

#line 485
static INT _allocClut(UINT nSize, INT nType) {
    INT nStartPos;
    INT nEndPos;
    _MallocUnit** ppAddr = NULL;
    switch (nType) {
    case 1:
        ppAddr = &s_pPreFixedClutPtr;
        nStartPos = VRAM::_CLUT_PREFIX_START;
        nEndPos = VRAM::_CLUT_PREFIX_END;
        break;
    case 0:
        ppAddr = &s_pFixedClutPtr;
        nStartPos = VRAM::_CLUT_FIX_START;
        nEndPos = VRAM::_CLUT_FIX_END;
        break;
    case 2:
        ppAddr = &s_pFontClutPtr;
        nStartPos = VRAM::_CLUT_FONT_START;
        nEndPos = VRAM::_CLUT_FONT_END;
        break;
    }
#line 503
    if (!ppAddr) AssertApp(__FILE__, __LINE__, "ppAddr", 0);
    return _allocSub(*ppAddr, nStartPos, nEndPos, nSize, "CLUT");
}

static void _compact(_MallocUnit** ppUnit) {
    if (ppUnit) {
        WaitSema(s_nAllocSema);
        _MallocUnit* pUnit = *ppUnit;
        while (pUnit) {
            _MallocUnit* pNext = pUnit->_pNext;
            if (pUnit->_nSize == 0) {
                if (pNext != NULL) {
                    if (pNext->_nSize == 0) {
                        if (pUnit->_pPrev) pUnit->_pPrev->_pNext = pNext;
                        pNext->_pPrev = pUnit->_pPrev;
                        pNext->_nStartPos = pUnit->_nStartPos;
                        pNext->_nSize += pUnit->_nSize;
                        delete pUnit;
                    }
                } else {
                    if (pUnit == *ppUnit) {
                        *ppUnit = NULL;
                    } else if (pUnit->_pPrev) {
                        pUnit->_pPrev->_pNext = NULL;
                    }
                    delete pUnit;
                }
            }
            pUnit = pNext;
        }
        SignalSema(s_nAllocSema);
    }
}

void CVramMan::compact(INT nType) {
    switch (nType) {
    case -1:
        _compact(&s_pPreFixedTexPtr);
        _compact(&s_pFixedTexPtr);
        _compact(&s_pFontTexPtr);
        _compact(&s_pPreFixedClutPtr);
        _compact(&s_pFixedClutPtr);
        _compact(&s_pFontClutPtr);
        break;
    case 1:
        _compact(&s_pPreFixedTexPtr);
        _compact(&s_pPreFixedClutPtr);
        break;
    case 0:
        _compact(&s_pFixedTexPtr);
        _compact(&s_pFixedClutPtr);
        break;
    case 2:
        _compact(&s_pFontTexPtr);
        _compact(&s_pFontClutPtr);
        break;
    }
}

INT CVramMan::getUsedRatio() {
    _MallocUnit* pUnit = s_pFixedTexPtr;
    if (!pUnit) return 0;
    while (pUnit->_pNext) pUnit = pUnit->_pNext;
    INT nNowSize = pUnit->_nEndPos - VRAM::_TEX_FIX_START;
    return nNowSize * 100 / VRAM::_TEX_FIX_SIZE;
}

void CVramMan::transTexProc() {
    WaitSema(s_nTransSema);
    switch (s_nTexTransStatus) {
    case 0:
        if (s_TransQueue.getListSize() == 0) break;
        s_nTexTransStatus = 1;
    case 1:
        if (s_TransQueue.getListSize() != 0) {
            FOREACH(CVtransReq, pReq, s_TransQueue) {
                pReq->transStarted();
                if (pReq->getTexInfo()._ClutInfo._pImage) CPackMan::pkRefLoadImage(pReq->getTexInfo()._ClutInfo);
                if (pReq->getTexInfo()._TexInfo._pImage) CPackMan::pkRefLoadImage(pReq->getTexInfo()._TexInfo);
            }
            s_nTexTransStatus = 2;
            break;
        }
        s_nTexTransStatus = 2;
    case 2:
        FOREACH(CVtransReq, pReq, s_TransQueue) {
            if (pReq->isNewReq()) break;
            s_TransQueue.popFront();
            delete pReq;
        }
        s_nTexTransStatus = s_TransQueue.getListSize() != 0;
        break;
    }
    SignalSema(s_nTransSema);
}

BOOL CVramMan::isTransTexDone() {
    return s_nTexTransStatus == 0 && s_TransQueue.getListSize() == 0;
}

BOOL CVramMan::waitTransTexDone(INT nThreadID) {
    if (nThreadID == s_nThreadID) {
        if (!isTransTexDone()) return FALSE;
    } else {
        while (!isTransTexDone() && !s_bHalt) {
            Thread::delayThread_1Frame();
        }
    }
    return TRUE;
}

#line 688
BOOL CVramMan::reqTransTex(const TexInfo& texInfo, CBuf& data) {
    BOOL bResult = FALSE;
    if (data.getBufPtr() != NULL &&
        (texInfo._TexInfo._pImage != NULL || texInfo._ClutInfo._pImage != NULL)) {
        WaitSema(s_nTransSema);
#line 701
        CVtransReq* pReq = new KH_AT("(701)") CVtransReq(texInfo, data);
        if (pReq) {
            s_TransQueue.pushBack(*pReq);
            bResult = TRUE;
        }
        SignalSema(s_nTransSema);
    }
    return bResult;
}

INT CVramMan::getHeapSize() {
    return VRAM::_TEX_FIX_SIZE;
}

static void _dbgReportAllocatedUnit(const _MallocUnit* pUnit) {
    if (!pUnit) {
        TRACE(":  ** NO DATA **\n");
        return;
    }
    TRACE("\n");
    for (; pUnit; pUnit = pUnit->_pNext) {
        TRACE("    UNIT[%07X] -> START:[%04X] END:[%04X] SIZE:[%04X] PREV:[%07X] NEXT:[%07X]\n", pUnit,
              pUnit->_nStartPos, pUnit->_nEndPos, pUnit->_nSize, pUnit->_pPrev, pUnit->_pNext);
    }
}

void CVramMan::dbgReportAllocatedUnit() {
    TRACE("------------------- occupied-vram report ------------------\n");
    TRACE("< _pPreFixedTexPtr >  ");
    _dbgReportAllocatedUnit(s_pPreFixedTexPtr);
    TRACE("< _pFixedTexPtr >     ");
    _dbgReportAllocatedUnit(s_pFixedTexPtr);
    TRACE("< _pFontTexPtr >      ");
    _dbgReportAllocatedUnit(s_pFontTexPtr);
    TRACE("< _pPreFixedClutPtr > ");
    _dbgReportAllocatedUnit(s_pPreFixedClutPtr);
    TRACE("< _pFixedClutPtr >    ");
    _dbgReportAllocatedUnit(s_pFixedClutPtr);
    TRACE("< _pFontClutPtr >     ");
    _dbgReportAllocatedUnit(s_pFontClutPtr);
    TRACE("----------------- occupied-vram report end -----------------\n");
}

void CVramMan::mapping(BOOL bNTSC) {
    using namespace VRAM;
    _FRAME0_START = 0;
    _FRAME0_SIZE = SCREEN_HEIGHT * 8;
    _FRAME0_END = _FRAME0_START + _FRAME0_SIZE;
    _FRAME1_START = _FRAME0_END;
    _FRAME1_SIZE = DISP_HEIGHT * 8;
    _FRAME1_END = _FRAME1_START + _FRAME1_SIZE;
    _FRAME_SIZE = _FRAME0_SIZE + _FRAME1_SIZE;
    _FRAME_AREA_END = _FRAME_SIZE;
    _ZBUF_START = _FRAME_AREA_END;
    _ZBUF_SIZE = _FRAME0_SIZE;
    _ZBUF_END = _ZBUF_START + _ZBUF_SIZE;
    _FONT_AREA_START = _FRAME1_START;
    _TEX_AREA_START = _ZBUF_END;
    _FONT_AREA_END = _FRAME1_END;
    _TEX_TMP1_START = _TEX_AREA_START;
    _TEX_TMP1_SIZE = 0x700;
    _TEX_TMP1_END = _TEX_TMP1_START + _TEX_TMP1_SIZE;
    _TEX_TMP2_SIZE = 0x100;
    _TEX_TMP2_START = _TEX_TMP1_END;
    _TEX_TMP2_END = _TEX_TMP2_START + _TEX_TMP2_SIZE;
    _TEX_PREFIX_START = _TEX_TMP2_END;
    if (bNTSC) {
        _TEX_PREFIX_SIZE = 0xF00;
    } else {
        _TEX_PREFIX_SIZE = 0xA00;
    }
    _TEX_FIX_SIZE = 0x100;
    _TEX_PREFIX_END = _TEX_PREFIX_START + _TEX_PREFIX_SIZE;
    _TEX_FIX_START = _TEX_PREFIX_END;
    _TEX_FIX_END = _TEX_FIX_START + _TEX_FIX_SIZE;
    _TEX_AREA_SIZE = _TEX_FIX_END - _TEX_AREA_START;
    _TEX_AREA_END = _TEX_FIX_END;
    _CLUT_AREA_START = _TEX_FIX_END;
    _CLUT_AREA_END = 0x4000;
    _CLUT_AREA_SIZE = _CLUT_AREA_END - _CLUT_AREA_START;
    _CLUT_TMP1_START = _CLUT_AREA_START;
    _CLUT_TMP1_SIZE = 0xF0;
    _CLUT_TMP1_END = _CLUT_TMP1_START + _CLUT_TMP1_SIZE;
    _CLUT_TMP2_START = _CLUT_TMP1_END;
    _CLUT_TMP2_SIZE = 0x10;
    _CLUT_TMP2_END = _CLUT_TMP2_START + _CLUT_TMP2_SIZE;
    _CLUT_FONT_START = _CLUT_TMP2_END;
    _CLUT_FONT_SIZE = 4;
    _CLUT_FONT_END = _CLUT_FONT_START + _CLUT_FONT_SIZE;
    _CLUT_PREFIX_START = _CLUT_FONT_END;
    _CLUT_PREFIX_SIZE = 0xE4;
    _CLUT_PREFIX_END = _CLUT_PREFIX_START + _CLUT_PREFIX_SIZE;
    _CLUT_FIX_START = _CLUT_PREFIX_END;
    _CLUT_FIX_END = 0x4000;
    _CLUT_FIX_SIZE = _CLUT_AREA_END - _CLUT_FIX_START;
}
// SOURCE DATA: .rodata .bss
