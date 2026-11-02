// PARTIAL: added for Game/Gimmick/GimmickBreakable
#ifndef SYSTEM_GRAPHIC_2D_2DMAN_H
#define SYSTEM_GRAPHIC_2D_2DMAN_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Debug.h"

/* MATCH: path override for System/Graphic/2d/2DFntObj ("c:/KHCOM/MainApp/System/Graphic/2d/2DMan.h") */
#ifndef D2MAN_H_PATH
#define D2MAN_H_PATH "System/Graphic/2D/2DMan.h"
#endif
#line 1 D2MAN_H_PATH
#include "System/Task.h"
#include "System/SJoint.h"
#include "System/FifoFpl.h"
#include "System/Graphic/2d/2DBase.h"
#include "System/Graphic/2d/2DSeqObj.h"

/* PARTIAL: placement new for C2DMan::initialize, declared only: the includers define it themselves (or never call it) */
inline void* operator new(size_t, void* p) throw();

namespace D2 {

struct SeqControl;
class C2DFnt;
class C2DSeqObj;

class C2DOSLCManager;

class C2DBaseList : public CSJointListT<C2DBase> {
public:
    virtual ~C2DBaseList() {
        C2DBase* pEntry = getHead();
        C2DBase* __pn = pEntry ? pEntry->getNext() : NULL;
        for (; pEntry; pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL) {
            delete pEntry;
        }
        resetParam();
    }
};

class C2DMan : public CTaskSecondary {
public:
    enum D2MAN_LEVEL {
        D2MAN_LEVEL0 = 0,
        D2MAN_LEVEL1 = 1,
        D2MAN_LEVEL2 = 2,
        D2MAN_LEVEL_SYSTEM = 3
    };

private:
    static C2DMan* m_pInstance;

public:
    static TIMEQ m_qTimeD;
    static INT m_nLayer;
    static INT m_fg2DViewer;

private:
    static C2DBaseList* m_p2DBaseList;
    static D2MAN_LEVEL m_nLevel;
    static D2MAN_LEVEL m_nLevelReq;
    static C2DOSLCManager* m_p2DOSLCManager;

    C2DMan(CTask* p);
    virtual ~C2DMan();

public:
#line 47 D2MAN_H_PATH
    static C2DMan& initialize(CTask* p) {
        if (!(__null == m_pInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
        m_pInstance = new (com_malloc_d(sizeof(C2DMan), __FILE__ "(49)", ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; }))) C2DMan(p);
        return *m_pInstance;
    }
    static void dispose() {
        delete m_pInstance;
        m_pInstance = NULL;
    }
    static HANDLE init(void* inFile, const CHAR* pBaseName);
    static HANDLE create(const CHAR* pBaseName, UINT inNo);
    static HANDLE create(const CHAR* pBaseName, UINT inNo, SeqControl* pSeqControl);
    static HANDLE create(HANDLE hBaseHandle, UINT inNo);
    static HANDLE createSeq(const CHAR* pBaseName, UINT inUnitNo, UINT inNo, SeqControl* pSeqControl);
    static HANDLE createFnt(const CHAR* pBaseName, C2DFnt* p2DFnt, SeqControl* pSeqControl);
    static HANDLE replaceSeq(HANDLE hCurrent, const CHAR* pBaseName, UINT inNo, SeqControl* pSeqControl);
    static HANDLE replaceFnt(HANDLE hCurrent, const CHAR* pBaseName, C2DFnt* p2DFnt, SeqControl* pSeqControl);
    static HANDLE createSpr(const CHAR* pBaseName, UINT inUnitNo, UINT inNo);
    static void destroy(HANDLE handle);
    static void destroyBase(HANDLE handleBase);
    static C2DBase* getBase(HANDLE handleBase);
    static void report();
    static INT getIdMax(const CHAR* pBaseName, UINT inType, UINT inUnitNo);
    static TIMEQ getTime(HANDLE handle);
    static TIMEQ getTimeMax(HANDLE handle);
    static BOOL isEnd(HANDLE handle);
    static void enableDisp(HANDLE handle, BOOL bEnable);
    static void setLayer(INT inLayer);
    static void restartSeq(HANDLE handle, FLOAT fStart);
    static C2DSeqObj* getSeqObj(HANDLE handle);
    static BOOL getLayPosition(const CHAR* pLadName, UINT inNo, UINT inIdx, FVECTOR& outPos, BOOL bLocal);
    static void changeSeqTime(HANDLE handle, UINT inTime);
    static void changeSeqLayer(HANDLE handle, INT nLayer);
    static void changeSeqTexture(HANDLE handle, INT inSprIdx, CHAR* pTexName, G_UV* pStart, G_UV* pEnd);
    static void changeSeqTexture(HANDLE handle, INT inSeqIdx, INT inSprIdx, CHAR* pTexName, G_UV* pStart,
                                 G_UV* pEnd);
    virtual void main();
    virtual void disp();
    static void changeLevel(D2MAN_LEVEL nLevel);
    static D2MAN_LEVEL getCurrentLevel() { return m_nLevel; }
    static void resetTexInfo();
    static C2DOSLCManager& get2DOSLCManager();
};

/* Declared after C2DMan: its vtables come first in .rodata. */
class C2DOSLCManager : public ISLCManagerT<C2DO>, private CFifoFpl {
public:
    C2DOSLCManager() { initialize(sizeof(SLContainerT<C2DO>), 0x800, NULL); }
    /* implicit destructor: synthesized where it is first needed (weak function order) */
    /* defined below, after ~ISLCManagerT (weak function order) */
    virtual SLContainerT<C2DO>* createContainer();
    virtual void releaseContainer(SLContainerT<C2DO>& inContainer);
    virtual SLContainerT<C2DO>* getContainer(HANDLE hID);
    C2DO* get2DO(HANDLE hID) {
        SLContainerT<C2DO>* pContainer = getContainer(hID);
        return pContainer ? pContainer->getValue() : NULL;
    }
    void debugDump(UINT nStartIndex, UINT nEndIndex) { CFifoFpl::debugDump(nStartIndex, nEndIndex); }
};

}  // namespace D2

namespace D2 {
inline SLContainerT<C2DO>* C2DOSLCManager::createContainer() {
    SLContainerT<C2DO>* pContainer = (SLContainerT<C2DO>*)allocBlock();
    if (!pContainer) TRACE("2D\x97\x70\x83\x52\x83\x93\x83\x65\x83\x69\x82\xaa MAX \x82\xc9\x93\x9e\x92\x42\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
    return pContainer;
}
inline void C2DOSLCManager::releaseContainer(SLContainerT<C2DO>& inContainer) { releaseBlock(&inContainer); }
inline SLContainerT<C2DO>* C2DOSLCManager::getContainer(HANDLE hID) {
    return (SLContainerT<C2DO>*)getValidActiveBlock(hID);
}
}  // namespace D2


#endif /* SYSTEM_GRAPHIC_2D_2DMAN_H */
