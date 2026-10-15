#ifndef GAME_MINIGAMEMANAGER_H
#define GAME_MINIGAMEMANAGER_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Fsm.h"

class CMiniGameBase;

class CMinigameManager : public CTask, private CFSM {
private:
    enum {
        STATE_MAIN,
        STATE_HELP,
        STATE_END_MENU,
        STATE_MAX
    };
    enum {
        PD_FLAG_IDX_MAX = 32
    };

    CStateTemplate<CMinigameManager> m_State[STATE_MAX]; /* 0x6c */
    SHORT m_nStep;                                       /* 0xcc */
    SHORT m_nNextScene;                                  /* 0xce */
    SHORT m_nClearEventFlagIdx;                          /* 0xd0 */
    SHORT m_nClearNextScene;                             /* 0xd2 */
    HANDLE m_hEndMenu;                                   /* 0xd4 */
    UINT m_uiPauseDisableFlag;                           /* 0xd8 */
    static HANDLE m_hHelp;
    static CMiniGameBase* m_pGame;

public:
    CMinigameManager(CTask* inpParent);
    virtual ~CMinigameManager();
    virtual void main();
    static CMiniGameBase* getGame() { return m_pGame; }
    static UINT sendEvent(UINT nEvent, void* pParam);
    static void reqFinish(SHORT inNextScene);
    static void reqRetry();
    static void reqEndMenu(SHORT inNextScene);
    static BOOL reqDispHelp(CTask* inParent, INT nLayer, BOOL inAppearSE);
    static BOOL isEndDispHelp();
    static BOOL isAlreadyClear();
    static SHORT getClearNextSceneNo();
    static void enablePauseIdx(INT nIdx, BOOL inSw);
    static void enablePause(BOOL inSw) { enablePauseIdx(0, inSw); }

private:
    static CMinigameManager* getManager();
    static void _reqFinish();
    INT getMinigameType();
    void changeState(INT inState);
    BOOL isMainState();
    void initialize();
    BOOL isEndMenu();
    void _enablePause(INT nIdx, BOOL inSw);
    void beginStateMain();
    void stateMain();
    void endStateMain();
    void beginStateHelp();
    void stateHelp();
    void endStateHelp();
    void beginStateEndMenu();
    void stateEndMenu();
    void endStateEndMenu();
};

class CMiniGameBase : public CTask, public CFSM {
public:
    CMiniGameBase(CTask* p);
    virtual ~CMiniGameBase();
    virtual void main();
    virtual void initialize();
    virtual HANDLE doDispHelpScreen(CTask* inParent, INT inLayer, BOOL inAppearSE);
    virtual UINT sendEvent(UINT nEvent, void* pParam);
    void reqFinish(SHORT inNextScene) { CMinigameManager::reqFinish(inNextScene); }
    BOOL isAlreadyClear() { return CMinigameManager::isAlreadyClear(); }
    SHORT getClearNextSceneNo() { return CMinigameManager::getClearNextSceneNo(); }
    void enablePause(BOOL inSw) { CMinigameManager::enablePauseIdx(0, inSw); }
};

typedef struct {
    INT nType;
    INT nLevel;
} EVENT_PARAM;

namespace MG_SKILL {
HANDLE createMGSkill(INT inType, INT inLevel);
BOOL isMGSkillAlive();
void killMGSkill();
}

#endif /* GAME_MINIGAMEMANAGER_H */
