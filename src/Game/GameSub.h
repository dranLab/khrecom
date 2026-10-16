#ifndef GAME_GAMESUB_H
#define GAME_GAMESUB_H

#include "khtypes.h"
#include "System/Fsm.h"

class CAtel;

class CGameSub : public CFSM {
private:
    CStateTemplate<CGameSub>* m_pStateTbl; /* 0x2c */
    INT m_nNewStateID;                     /* 0x30 */
    INT m_nStateID;                        /* 0x34 */
    INT m_nMapID;                          /* 0x38 */
    CAtel* m_pAtel;                        /* 0x3c */
    INT m_nAtelTimeOut;                    /* 0x40 */

public:
    CGameSub(CStateTemplate<CGameSub>* pStateTbl);
    virtual ~CGameSub();
    virtual INT getGameSubState() = 0;
    virtual BOOL reqChangeGameSubState() = 0;
    virtual void terminate() {}
    virtual void endStateProc();
    INT getMapID() const { return m_nMapID; }
    BOOL isFadeActive() const;

protected:
    void changeState(INT nState);
    INT getCurrentStateID() const { return m_nStateID; }
    CAtel* getAtel() { return m_pAtel; }
    void preSetupAtel();
    BOOL setupAtel();
    void startAtel();
    void exitAtel();
    BOOL isAtelStarted() const;
    void suspendAtel();
    void resumeAtel();
    void setMapID(INT nID) { m_nMapID = nID; }
};

#endif /* GAME_GAMESUB_H */
