#define FSM_H_PATH "c:/KHCOM/MainApp/System/Fsm.h" /* MATCH: this unit names Fsm.h so (header default "System/Fsm.h") */
#include "System/Fsm.h"
#include <string.h>

CFSM::CFSM() : m_pNewState(NULL) {
    m_StateInitial.set(this, &CFSM::beginStateInitial, &CFSM::stateInitial, &CFSM::endStateInitial);
    m_pCurrentState = &m_StateInitial;
}

void CFSM::update() {
    if (m_pNewState) {
        m_pCurrentState->executeEndState();
        endStateProc();
        m_pCurrentState = m_pNewState;
        m_pNewState = NULL;
        m_pCurrentState->executeBeginState();
        if (m_pNewState) return;
    }
    m_pCurrentState->executeState();
}

// SOURCE DATA: .rodata
