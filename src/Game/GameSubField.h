#ifndef GAME_GAMESUBFIELD_H
#define GAME_GAMESUBFIELD_H

#include "khtypes.h"
#include "Game/GameSub.h"

class CGameSubField : public CGameSub {
private:
    CStateTemplate<CGameSubField> m_State[13]; /* 0x044 */
    INT m_nStep;                               /* 0x1e4 */

public:
    CGameSubField();
    virtual ~CGameSubField();
    virtual INT getGameSubState() const;
    virtual BOOL reqChangeGameSubState(INT nState);
    virtual void terminate();

private:
    void preLoadField();
    void loadField();
    void preLoadPlayer();
    void loadPlayer();
    INT getPreEventIndex() const;
    void preProcPreEvent();
    void procPreEvent();
    void preSetupAtel();
    void setupAtel();
    void preSetupTasks();
    void setupTasks();
    void preWaitStartAtel();
    void waitStartAtel();
    void prePlay();
    void playing();
    void preToBattle();
    void toBattle();
    void preToBattleDispose();
    void toBattleDispose();
    void preFromBattle();
    void fromBattle();
    void preToNextRoom();
    void toNextRoom();
    void preToNextRoomDispose();
    void toNextRoomDispose();
    void preSleep();
    void sleeping();
    void disposeTasks();
    void setupMapCollision();
};

#endif /* GAME_GAMESUBFIELD_H */
