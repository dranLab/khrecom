
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define D2MAN_H_PATH "c:/KHCOM/MainApp/System/Graphic/2d/2DMan.h" /* MATCH: this unit names 2DMan.h so (header default "System/Graphic/2D/2DMan.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/2d/2DLayObj.h"
#include "System/Graphic/2d/2DBase.h"
#include "System/Task.h"

#include "System/Graphic/2d/2DLay.h"
#include "System/Graphic/2d/2DSeq.h"
#include "System/HeapMan.h"



namespace D2 {


















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
        C2DO* p2DO = C2DMan::get2DOSLCManager().get2DO(hSeqObj);
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


HANDLE C2DLayObj::create(HANDLE handle, UINT nScreen, LAY_SCREEN* pLayScreen, HANDLE* pSeqHandle) {
    UINT i;
    m_nSeqObj = nScreen;
    m_pSeqObjHandle = (HANDLE*)com_malloc_d(nScreen * sizeof(HANDLE), __FILE__ "(65)", RET_ADDR());
    for (i = 0; i < nScreen; i++, pLayScreen++) {
        C2DBase* p2DBase = C2DMan::getBase(pSeqHandle[pLayScreen->sequence_unit]);
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
        C2DO* p2DO = C2DMan::get2DOSLCManager().get2DO(hSeqObj);
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
        C2DO* p2DO = C2DMan::get2DOSLCManager().get2DO(hSeqObj);
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
        C2DO* p2DO = C2DMan::get2DOSLCManager().get2DO(hSeqObj);
        if (p2DO) {
            p2DO->enableDisp(bEnable);
        }
    }
}

}  // namespace D2

// SOURCE DATA: .data .rodata
