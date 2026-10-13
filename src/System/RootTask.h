#ifndef SYSTEM_ROOTTASK_H
#define SYSTEM_ROOTTASK_H

#include "khtypes.h"
#include "System/Task.h"

class CMemocaAgent;

class CRootTask : public CTask {
private:
    USHORT m_nPhase;
    USHORT m_nStep;
    CMemocaAgent* m_pMemoca;

public:
    CRootTask();
    virtual ~CRootTask();
    virtual void main();

private:
    void _preparePhaseInit();
    void _phaseInit();
    void _preparePhaseChkPad();
    void _phaseChkPad();
    void _preparePhaseChkMemoca();
    void _phaseChkMemoca();
    void _preparePhaseMain();
    void _phaseMain();
    void _launchTitle(BOOL bDebug);
    void _checkHardError();
    void _openErrMessage();
    void _closeErrMessage();
};

#endif /* SYSTEM_ROOTTASK_H */
