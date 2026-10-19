#ifndef GAME_GAMESUBBATTLE_H
#define GAME_GAMESUBBATTLE_H

#include "khtypes.h"
#include "Game/GameSub.h"

class CGameSubBattle : public CGameSub {
private:
    CStateTemplate<CGameSubBattle> m_State[10]; /* 0x044 */
    INT m_nStep;                                /* 0x184 */
    HANDLE m_hLevelUp;                          /* 0x188 */

public:
    CGameSubBattle();
    virtual ~CGameSubBattle();
    virtual INT getGameSubState() const;
    virtual BOOL reqChangeGameSubState(INT nState);
    virtual void terminate();

private:
    void preLoadField();
    void loadField();
    void preWaitEncountEffect();
    void waitEncountEffect();
    void preSetupAtel();
    void setupAtel();
    void preSetupTasks();
    void setupTasks();
    void preWaitStartAtel();
    void waitStartAtel();
    void prePlay();
    void playing();
    void preToField();
    void toField();
    void preToFieldDispose();
    void toFieldDispose();
    void preLevelUp();
    void levelUp();
    void preSleep();
    void sleeping();
    void disposeTasks();
};

class CEncountEffect {
public:
    static void initialize();
    static void start();
    static void reset();
    static BOOL isStart();
    static BOOL isEnd();
};

#endif /* GAME_GAMESUBBATTLE_H */
