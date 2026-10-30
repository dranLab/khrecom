#ifndef GAME_ATEL_ATEL_H
#define GAME_ATEL_ATEL_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Fsm.h"

class CEntity;
class CFadeIOMask;
struct LineTrapSrc;
struct BoxTrapSrc;

namespace ATEL {
class CVSM;
struct Actor;
extern CFadeIOMask* s_pFadeIOMask[2];
}  // namespace ATEL

class CAtel : public CTask, public CFSM {
    friend class CEntityFactory; // MATCH: the original's CEntityFactory::createEntity calls the protected static createAvatar
private:
    INT m_nFsmState;                 /* 0x6c */
    ATEL::CVSM* m_pVSM;              /* 0x70 */
    CStateTemplate<CAtel> m_State[4]; /* 0x74 */

public:
    CAtel(CTask* pParent);
    virtual ~CAtel();
    void loadFromMem(const BYTE* pAddr);
    void setupActor();
    BOOL canStart() const;
    void start();
    BOOL isStarted() const;
    virtual void main();
    BOOL createAvatar(INT nRsrcID, ATEL::Actor& act, INT flag);
    BOOL createEventTrap(const LineTrapSrc& src, ATEL::Actor& act);
    BOOL createEventTrap(const BoxTrapSrc& src, ATEL::Actor& act);
    ATEL::CVSM* getVSM() { return m_pVSM; }
    static void skipEvent();
    void setScreenFader(INT nKind, FLOAT fRate, INT nLayer);
    static void createScreenFader();
    static void disposeScreenFader();
    static void clearScreenFader(INT nID);
    static BOOL isFadeActive();
    static BOOL isFadeActiveBlack();
    static BOOL isFadeActiveWhite();
    static void reqWhiteFadeOut(USHORT nTime);
    static void reqWhiteFadeIn(USHORT nTime);
    static void setWorldCompleteFlag(INT in_WorldID);
    static void setFloorCompleteFlag(INT in_Floor, INT in_RoomNo);
    static void setWorldStepFlag(INT in_WorldID, INT in_EventNo);
    static void setCardPossible(INT in_Card);
    static void setFriendPossible(INT in_Friend);
    static void setWorldTitleEnd(INT in_World);
    static void setMoviePossible(INT MovieStorageFlag, INT in_MovieNo);
    static void setTutorialEnd(INT in_End);
    static void setPlayerSkill(INT in_Skill);
    static void setSceneDeSelectInitFlags();

protected:
    INT getState() const { return m_nFsmState; }
    void changeState(INT nStat);
    static CEntity* createAvatar(INT inRsrcID, CTask* inParent);

private:
    void preInitActor();
    void initActor();
    void preRun();
    void running();
    void preNop();
    void nop();
    void preSleep();
    void sleeping();

public:
    static BOOL checkAtelPause();
};

#endif /* GAME_ATEL_ATEL_H */
