#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#define D2MAN_H_PATH "c:/KHCOM/MainApp/System/Graphic/2d/2DMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/2d/2DLayObj.h"
#include "System/Graphic/2d/2DBase.h"
#include "System/Task.h"

namespace D2 {

struct LAY_SCREEN {
    INT _00;
    INT nSeqIdx; /* 0x4 */
    INT _08[4];
};

class C2DSeq : public C2DBase {
public:
    HANDLE create(LAY_SCREEN* pLayScreen);
};

class C2DSpr {
public:
    static void addList(C2DO& r2DO);
    static BOOL deleteList(C2DO& r2DO);
};

class C2DMan {
public:
    static C2DBase* getBase(HANDLE handle);
    static ISLCManagerT<C2DO>* get2DOSLCManager();
    static C2DO* get2DO(HANDLE hID) {
        SLContainerT<C2DO>* pContainer = get2DOSLCManager()->getContainer(hID);
        return pContainer ? pContainer->getValue() : NULL;
    }
};

UINT C2DLayObj::m_uiID = 0;

C2DLayObj::C2DLayObj() {
    /* through a reference: keeps the load after the base class stores (scheduling) */
    UINT& rID = m_uiID;
    rID++;
    m_pSeqObjHandle = NULL;
    m_nSeqObj = 0;
    m_qTimeMax = 0;
}

C2DLayObj::~C2DLayObj() {
    INT i;
    for (i = 0; i < m_nSeqObj; i++) {
        HANDLE hSeqObj = m_pSeqObjHandle[i];
        C2DO* p2DO = C2DMan::get2DO(hSeqObj);
        if (p2DO) {
            if (C2DSpr::deleteList(*p2DO)) {
                delete p2DO;
            }
        }
    }
    if (m_pSeqObjHandle) {
        com_free(m_pSeqObjHandle);
    }
    m_pSeqObjHandle = NULL;
}

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

HANDLE C2DLayObj::create(HANDLE handle, UINT nScreen, LAY_SCREEN* pLayScreen, HANDLE* pSeqHandle) {
    UINT i;
    m_nSeqObj = nScreen;
    m_pSeqObjHandle = (HANDLE*)com_malloc_d(nScreen * sizeof(HANDLE), __FILE__ "(65)", RET_ADDR());
    for (i = 0; i < nScreen; i++, pLayScreen++) {
        C2DBase* p2DBase = C2DMan::getBase(pSeqHandle[pLayScreen->nSeqIdx]);
        if (p2DBase) {
            HANDLE hSeq = ((C2DSeq*)p2DBase)->create(pLayScreen);
            TIMEQ qTimeMax = p2DBase->getTimeMax(hSeq);
            if (m_qTimeMax < qTimeMax) m_qTimeMax = qTimeMax;
            m_pSeqObjHandle[i] = hSeq;
        }
    }
    m_hBaseHandle = handle;
    C2DSpr::addList(*this);
    return m_hHandle;
}

TIMEQ C2DLayObj::getTime() {
    TIMEQ qTime = 0;
    INT i;
    for (i = 0; i < m_nSeqObj; i++) {
        HANDLE hSeqObj = m_pSeqObjHandle[i];
        C2DO* p2DO = C2DMan::get2DO(hSeqObj);
        if (p2DO) {
            if (qTime < ((C2DSeqObj*)p2DO)->getTime()) qTime = ((C2DSeqObj*)p2DO)->getTime();
        }
    }
    return qTime;
}

BOOL C2DLayObj::isEnd() {
    INT i;
    for (i = 0; i < m_nSeqObj; i++) {
        HANDLE hSeqObj = m_pSeqObjHandle[i];
        C2DO* p2DO = C2DMan::get2DO(hSeqObj);
        if (p2DO && !((C2DSeqObj*)p2DO)->isEnd()) {
            return FALSE;
        }
    }
    return TRUE;
}

void C2DLayObj::enableDisp(HANDLE handle, BOOL bEnable) {
    INT i;
    for (i = 0; i < m_nSeqObj; i++) {
        HANDLE hSeqObj = m_pSeqObjHandle[i];
        C2DO* p2DO = C2DMan::get2DO(hSeqObj);
        if (p2DO) {
            p2DO->enableDisp(bEnable);
        }
    }
}

}  // namespace D2

// SOURCE DATA: .data .rodata
