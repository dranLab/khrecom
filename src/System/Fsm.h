// PARTIAL: added for Game/Gimmick/BaseGimmick (CState, CStateTemplate<T>; CFSM itself is still in Game/Entity.h)
#ifndef SYSTEM_FSM_H
#define SYSTEM_FSM_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"

class CState {
public:
    virtual ~CState() {}
    virtual void executeBeginState() = 0;
    virtual void executeState() = 0;
    virtual void executeEndState() = 0;
};

/* Line numbers are the original ones (assert messages). The original asserts pfnBeginState three times in set(). */
template <class T>
class CStateTemplate : public CState {
protected:
    T* m_pInstance;
    void (T::*m_pfnBeginState)();
    void (T::*m_pfnState)();
    void (T::*m_pfnEndState)();

public:
    CStateTemplate() {
        m_pInstance = __null;
        memset(&m_pfnBeginState, 0, sizeof(m_pfnBeginState));
        memset(&m_pfnState, 0, sizeof(m_pfnState));
        memset(&m_pfnEndState, 0, sizeof(m_pfnEndState));
    }
    void set(T* pInstance, void (T::*pfnBeginState)(), void (T::*pfnState)(), void (T::*pfnEndState)()) {
#line 41 "System/Fsm.h"
        if (!(pInstance)) AssertApp(__FILE__, __LINE__, "pInstance", 0);
        m_pInstance = pInstance;
#line 44 "System/Fsm.h"
        if (!(pfnBeginState)) AssertApp(__FILE__, __LINE__, "pfnBeginState", 0);
        m_pfnBeginState = pfnBeginState;
        if (!(pfnBeginState)) AssertApp(__FILE__, __LINE__, "pfnBeginState", 0);
        m_pfnState = pfnState;
        if (!(pfnBeginState)) AssertApp(__FILE__, __LINE__, "pfnBeginState", 0);
        m_pfnEndState = pfnEndState;
    }

    virtual void executeBeginState() {
#line 57 "System/Fsm.h"
        if (!(m_pInstance)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
        if (m_pfnBeginState) (m_pInstance->*m_pfnBeginState)();
    }

    virtual void executeState() {
#line 65 "System/Fsm.h"
        if (!(m_pInstance)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
        if (m_pfnState) (m_pInstance->*m_pfnState)();
    }

    virtual void executeEndState() {
#line 73 "System/Fsm.h"
        if (!(m_pInstance)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
        if (m_pfnEndState) (m_pInstance->*m_pfnEndState)();
    }
};

/* PARTIAL: added for Game/MinigameManager. CFSM is still declared (opaquely) in Game/Entity.h too: this one is
 * skipped when Entity.h came first, and Entity.h must not be included after it. */
#ifndef GAME_ENTITY_H
class CFSM {
protected:
    CState* m_pCurrentState;
    CState* m_pNewState;
    CStateTemplate<CFSM> m_StateInitial;

public:
    CFSM();
    virtual ~CFSM() {}
    virtual void update();
    BOOL isState(CState& inState) { return m_pCurrentState == &inState; }
    BOOL isNewState(CState& inState) { return m_pNewState == &inState; }
    void gotoState(CState& inState) { m_pNewState = &inState; }
    virtual void beginStateInitial() {}
    virtual void stateInitial() {}
    virtual void endStateInitial() {}
    virtual void endStateProc() {}
};
#endif

#endif /* SYSTEM_FSM_H */
