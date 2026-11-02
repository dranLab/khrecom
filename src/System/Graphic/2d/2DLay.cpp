
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#define D2MAN_H_PATH "c:/KHCOM/MainApp/System/Graphic/2d/2DMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/2d/2DLay.h"
#include "System/Graphic/2d/2DLayObj.h"
#include "System/Task.h"
#include <string.h>

extern "C" int sprintf(char* buf, const char* fmt, ...);

inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

namespace D2 {













/* the .lad file header (original name unknown) */
struct LAD_HEADER {
    CHAR id[8];
    INT nScreen;          /* 0x08 */
    UINT ofsScreen;       /* 0x0c */
    INT nLayout;          /* 0x10 */
    UINT ofsLayout;       /* 0x14 */
    INT nSequenceUnit;    /* 0x18 */
    UINT ofsSequenceUnit; /* 0x1c */
};

C2DLay::C2DLay() {
    makeBaseHandle(D2BASE_TYPE_LAY);
    m_pLayScreen = NULL;
    m_pLayLayout = NULL;
    m_pLaySequenceUnit = NULL;
    m_pSeqHandle = NULL;
}

C2DLay::~C2DLay() {
    INT i;
    for (i = 0; i < m_nSequenceUnitMax; i++) {
        C2DMan::destroyBase(m_pSeqHandle[i]);
    }
    if (m_pSeqHandle) {
        com_free(m_pSeqHandle);
    }
    m_pSeqHandle = NULL;
}

#line 40
BOOL C2DLay::init(void* pLad, const CHAR* pBaseName) {
    LAD_HEADER* pHeader = (LAD_HEADER*)pLad;
    CHAR szName[32];
    CHAR szSeqName[32];
    CHAR* p;
    INT i;

    if (!pLad) return FALSE;
    strcpy(m_pBaseName, pBaseName);
    m_pLayScreen = (LAY_SCREEN*)((CHAR*)pLad + pHeader->ofsScreen);
    m_pLayLayout = (LAY_LAYOUT*)((CHAR*)pLad + pHeader->ofsLayout);
    m_pLaySequenceUnit = (LAY_SEQUENCE_UNIT*)((CHAR*)pLad + pHeader->ofsSequenceUnit);
    m_nScreenMax = pHeader->nScreen;
    m_nLayoutMax = pHeader->nLayout;
    m_nSequenceUnitMax = pHeader->nSequenceUnit;
    strcpy(szName, pBaseName);
    p = strstr(szName, ".lad");
    if (!p) return FALSE;
    *p = '\0';

    m_pSeqHandle = (HANDLE*)com_malloc_d(m_nSequenceUnitMax * sizeof(HANDLE), __FILE__ "(63)", RET_ADDR());
    for (i = 0; i < m_nSequenceUnitMax; i++) {
        void* pData = (CHAR*)pLad + m_pLaySequenceUnit[i].ofs;
        sprintf(szSeqName, "%s:%02d", szName, i);
        m_pSeqHandle[i] = C2DMan::init(pData, szSeqName);
    }
    return TRUE;
}

HANDLE C2DLay::create(UINT inNo) {
    if (inNo >= (UINT)m_nLayoutMax) return 0;
    LAY_LAYOUT* pLayout = &m_pLayLayout[inNo];
    UINT nPos = pLayout->pos;
    UINT nMax = pLayout->max;
    C2DLayObj* p2DLayObj = new (com_malloc_d(sizeof(C2DLayObj), __FILE__ "(84)", RET_ADDR())) C2DLayObj;
    HANDLE handle = p2DLayObj->create(m_hBase, nMax, &m_pLayScreen[nPos], m_pSeqHandle);
    if (!handle && p2DLayObj) delete p2DLayObj;
    return handle;
}

HANDLE C2DLay::createSeq(UINT inUnitNo, UINT inNo, SeqControl* pSeqControl) {
    if (inUnitNo >= (UINT)m_nSequenceUnitMax) return 0;
    return C2DMan::getBase(m_pSeqHandle[inUnitNo])->createSeq(inUnitNo, inNo, pSeqControl);
}

HANDLE C2DLay::createSpr(UINT inUnitNo, UINT inNo) {
    if (inUnitNo >= (UINT)m_nSequenceUnitMax) return 0;
    return C2DMan::getBase(m_pSeqHandle[inUnitNo])->createSpr(inUnitNo, inNo);
}

LAY_SCREEN* C2DLay::getLayScreen(UINT inNo) {
    if (inNo >= (UINT)m_nLayoutMax) return NULL;
    LAY_LAYOUT* pLayout = &m_pLayLayout[inNo];
    return &m_pLayScreen[pLayout->pos];
}

INT C2DLay::getSeqIdMax(UINT inUnitNo) {
    if (inUnitNo < (UINT)m_nSequenceUnitMax) {
        C2DBase* p2DBase = C2DMan::getBase(m_pSeqHandle[inUnitNo]);
        if (p2DBase) return p2DBase->getIdMax();
    }
    return 0;
}

INT C2DLay::getSprIdMax(UINT inUnitNo) {
    if (inUnitNo < (UINT)m_nSequenceUnitMax) {
        C2DBase* p2DBase = C2DMan::getBase(m_pSeqHandle[inUnitNo]);
        if (p2DBase) return p2DBase->getSprIdMax(0);
    }
    return 0;
}

TIMEQ C2DLay::getTime(HANDLE handle) {
    C2DLayObj* p2DLayObj = (C2DLayObj*)C2DMan::get2DOSLCManager().get2DO(handle);
    return p2DLayObj ? p2DLayObj->getTime() : 0;
}

TIMEQ C2DLay::getTimeMax(HANDLE handle) {
    C2DLayObj* p2DLayObj = (C2DLayObj*)C2DMan::get2DOSLCManager().get2DO(handle);
    return p2DLayObj ? p2DLayObj->getTimeMax() : 0;
}

BOOL C2DLay::isEnd(HANDLE handle) {
    C2DLayObj* p2DLayObj = (C2DLayObj*)C2DMan::get2DOSLCManager().get2DO(handle);
    return p2DLayObj ? p2DLayObj->isEnd() : FALSE;
}

void C2DLay::enableDisp(HANDLE handle, BOOL bEnable) {
    C2DLayObj* p2DLayObj = (C2DLayObj*)C2DMan::get2DOSLCManager().get2DO(handle);
    if (p2DLayObj) p2DLayObj->enableDisp(handle, bEnable);
}

#line 267
void C2DLay::destroy(HANDLE handle) {
    if (!(FALSE)) AssertApp(__FILE__, __LINE__, "FALSE", 0);
}

void C2DLay::_main() {}

void C2DLay::_disp() {}

}  // namespace D2

// SOURCE DATA: .rodata
