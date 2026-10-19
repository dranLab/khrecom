#ifndef GAME_ATEL_CAMERAMAN_H
#define GAME_ATEL_CAMERAMAN_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/HeapMan.h"
#include "System/Vector.h"
#include "System/SJoint.h"
#include "Game/Atel/CommandAccepter.h"

/* the original's file is Game/ATEL/CameraMan.h; Game/Atel/CameraMan.cpp names it Game/Atel/CameraMan.h */
#ifndef CAMERAMAN_H_PATH
#define CAMERAMAN_H_PATH "c:/KHCOM/MainApp/Game/ATEL/CameraMan.h"
#endif

class ICamera;
class CCamera;

namespace ATEL {

class CVSM;

class CCameraDt;
class CCameraDtList;

/* The inline members are emitted (weak) in Game/Atel/CameraMan.cpp, in this order: keep it. */
class CCameraMan : public CTask, public ICommandAccepter {
public:
    static CCameraMan* m_pInstance;

private:
    static CCameraDtList* m_pCCameraDtList;
    CCameraDt* m_pActiveCameraDt; /* 0x50 */
    INT m_timerIgnorePause;       /* 0x54 */
    INT m_fgWipe;                 /* 0x58 */
    INT m_frameEffKindSave;       /* 0x5c */
    INT m_fgGakaku;               /* 0x60 */
    FLOAT m_targetGakaku;         /* 0x64 */
    FLOAT m_currentGakaku;        /* 0x68 */

public:
    CCameraMan(CTask* pParent);
    virtual ~CCameraMan();
    void setup();
    void dispose();

#line 64 CAMERAMAN_H_PATH
    static void initialize(CTask* pParent) {
        if (m_pInstance) return;
        TRACE("CCameraMan::initialize\n");
        if (!(__null == m_pInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
        m_pInstance = new (com_malloc_d(sizeof(CCameraMan), __FILE__ "(68)", ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; }))) CCameraMan(pParent);
        if (!(m_pInstance)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
        m_pInstance->setup();
    }
    static void finalize() {
        CCameraMan*& p = m_pInstance;
        if (p) delete p;
        p = __null;
    }
#line 79 CAMERAMAN_H_PATH
    static CCameraMan& instance() {
        CCameraMan*& p = m_pInstance;
        if (!(p)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
        return *p;
    }
    virtual void main();
    virtual INT reqCommand(INT nCmd, void* pParam, INT nID) {
        if (nCmd == 0x44C) return onCreateCameraFix(nCmd, pParam, nID);
        if (nCmd == 0x44D) return onCreateCameraFixTarget(nCmd, pParam, nID);
        if (nCmd == 0x44E) return onCreateCameraPath(nCmd, pParam, nID);
        if (nCmd == 0x44F) return onDestroyCamera(nCmd, pParam, nID);
        if (nCmd == 0x450) return onDestroyCameraAll(nCmd, pParam, nID);
        if (nCmd == 0x451) return onSetActiveCamera(nCmd, pParam, nID);
        if (nCmd == 0x452) return onChaeckExistCamera(nCmd, pParam, nID);
        if (nCmd == 0x453) return onChaeckEndCamera(nCmd, pParam, nID);
        if (nCmd == 0x454) return onSetPlayerCamera(nCmd, pParam, nID);
        if (nCmd == 0x455) return onResetPlayerCamera(nCmd, pParam, nID);
        if (nCmd == 0x456) return onChangeGakaku(nCmd, pParam, nID);
        if (nCmd == 0x457) return onResetGakaku(nCmd, pParam, nID);
        if (nCmd == 0x458) return onCreateCameraChat(nCmd, pParam, nID);
        if (nCmd == 0x459) return onCreateCameraPathData(nCmd, pParam, nID);
        if (nCmd == 0x45A) return onAddCameraQuakeV(nCmd, pParam, nID);
        if (nCmd == 0x45B) return onAddCameraQuakeH(nCmd, pParam, nID);
        if (nCmd == 0x45C) return onSetOffsetY(nCmd, pParam, nID);
        if (nCmd == 0x45D) return onStop(nCmd, pParam, nID);
        if (nCmd == 0x45E) return onRun(nCmd, pParam, nID);
        if (nCmd == 0x45F) return onCreateCameraRelative(nCmd, pParam, nID);
        if (nCmd == 0x460) return onSetPlayerCameraNormal(nCmd, pParam, nID);
        if (nCmd == 0x461) return onIgnorePause(nCmd, pParam, nID);
        if (nCmd == 0x462) return onIgnorePlaySpeed(nCmd, pParam, nID);
        if (nCmd == 0x463) return onCreateCameraLookAt(nCmd, pParam, nID);
        if (nCmd == 0x464) return onChangeCamTarget(nCmd, pParam, nID);
        if (nCmd == 0x465) return onCamEffect(nCmd, pParam, nID);
        return 0;
    }

private:
    CCameraDt* searchCameraDt(INT nID);
    INT onCreateCameraFix(INT nCmd, void* pParam, INT nID);
    INT onCreateCameraFixTarget(INT nCmd, void* pParam, INT nID);
    INT onCreateCameraPath(INT nCmd, void* pParam, INT nID);
    INT onDestroyCamera(INT nCmd, void* pParam, INT nID);
    INT onDestroyCameraAll(INT nCmd, void* pParam, INT nID);
    INT onSetActiveCamera(INT nCmd, void* pParam, INT nID);
    INT onChaeckExistCamera(INT nCmd, void* pParam, INT nID);
    INT onChaeckEndCamera(INT nCmd, void* pParam, INT nID);
    INT onSetPlayerCamera(INT nCmd, void* pParam, INT nID);
    INT onResetPlayerCamera(INT nCmd, void* pParam, INT nID);
    INT onChangeGakaku(INT nCmd, void* pParam, INT nID);
    INT onResetGakaku(INT nCmd, void* pParam, INT nID);
    INT onCreateCameraChat(INT nCmd, void* pParam, INT nID);
    INT onCreateCameraPathData(INT nCmd, void* pParam, INT nID);
    INT onAddCameraQuakeV(INT nCmd, void* pParam, INT nID);
    INT onAddCameraQuakeH(INT nCmd, void* pParam, INT nID);
    INT onSetOffsetY(INT nCmd, void* pParam, INT nID);
    INT onStop(INT nCmd, void* pParam, INT nID);
    INT onRun(INT nCmd, void* pParam, INT nID);
    INT onCreateCameraRelative(INT nCmd, void* pParam, INT nID);
    INT onCreateCameraLookAt(INT nCmd, void* pParam, INT nID);
    INT onSetPlayerCameraNormal(INT nCmd, void* pParam, INT nID);
    INT onIgnorePause(INT nCmd, void* pParam, INT nID);
    INT onIgnorePlaySpeed(INT nCmd, void* pParam, INT nID);
    INT onChangeCamTarget(INT nCmd, void* pParam, INT nID);
    INT onCamEffect(INT nCmd, void* pParam, INT nID);
    INT createCameraFix(void* pParam, INT nID);
    void wipeSet(INT kind);
    void wipeReset();
    void makeRot(FVECTOR& outRot, FVECTOR& inCameraPos, FVECTOR& inTargetPos);
    INT getDefaultGakaku(FLOAT& outGakaku);
    CTask* getObjFromId(INT id, CVSM* pVsm);
};

class CCameraDt : public CSJointT<CCameraDt> {
public:
    FVECTOR m_cameraPos;        /* 0x10 */
    FVECTOR m_targetPos;        /* 0x20 */
    FVECTOR m_currentCameraPos; /* 0x30 */
    FVECTOR m_currentTargetPos; /* 0x40 */
    FVECTOR m_relativePos;      /* 0x50 */
    FVECTOR m_camMidSpeed;      /* 0x60 */
    FVECTOR m_camStEdSpeed;     /* 0x70 */
    FVECTOR m_camStEdAddSpeed;  /* 0x80 */
    CTask* m_pTargetObj;        /* 0x90 CBfObj */
    ICamera* m_pCamera;         /* 0x94 */
    FLOAT m_fChangeRate;        /* 0x98 */
    FLOAT m_fTargetFrame;       /* 0x9c */
    FLOAT m_fFrameCnt;          /* 0xa0 */
    FLOAT m_targetHeight;       /* 0xa4 */
    FLOAT m_targetSide;         /* 0xa8 */
    FLOAT m_targetFront;        /* 0xac */
    FLOAT m_fFullFrame;         /* 0xb0 */
    FLOAT m_fMidFrame;          /* 0xb4 */
    FLOAT m_fStartEndFrame;     /* 0xb8 */
    SHORT m_nID;                /* 0xbc */
    SHORT m_kind;               /* 0xbe */
    SHORT m_status;             /* 0xc0 */
    SHORT m_changeKind;         /* 0xc2 */
    SHORT m_changeKind2;        /* 0xc4 */

    virtual ~CCameraDt();
};

class CCameraDtList : public CSJointListT<CCameraDt> {
public:
    virtual ~CCameraDtList() {
        for (CCameraDt *__p = getHead(), *__pn = __p ? __p->getNext() : NULL; __p;
             __p = __pn, __pn = __pn ? __pn->getNext() : NULL) {
            delete __p;
        }
        resetParam();
    }
};

} // namespace ATEL

#endif /* GAME_ATEL_CAMERAMAN_H */
