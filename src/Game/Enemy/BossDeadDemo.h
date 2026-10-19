#ifndef GAME_ENEMY_BOSSDEADDEMO_H
#define GAME_ENEMY_BOSSDEADDEMO_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Vector.h"

class CEntity;
class CBattleEnemy;
class CPathCamera;

/* All inline members are emitted (weak) in Game/Enemy/BossDeadDemo.cpp, in declaration order: keep the order. */
class CBossDeadDemo : public CTask {
private:
    FVECTOR m_vPlayerPos;       /* 0x40 */
    BOOL m_bPlayerPos;          /* 0x50 */
    INT m_iStep;                /* 0x54 */
    TIMEQ m_tqTimer;            /* 0x58 */
    TIMEQ m_tqDeadDemoSETimer;  /* 0x5c */
    HANDLE m_hDeadDemoSE;       /* 0x60 */
    HANDLE m_hBossHandle;       /* 0x64 */
    INT m_iCardID;              /* 0x68 */
    HANDLE m_hCard;             /* 0x6c */
    HANDLE m_hLevelUp;          /* 0x70 */
    CPathCamera* m_pPathCamera; /* 0x74 */
    BOOL m_bMonstroEventBattle; /* 0x78 */
    static BOOL m_bPermissionWhiteIn;
    static BOOL m_bStart;
    static BOOL m_bFade;

public:
    CBossDeadDemo(CTask* inParent, BOOL inMonstroEventBattle);
    virtual ~CBossDeadDemo();
    void setup(HANDLE inBossHandle, const FVECTOR* inPlayerPos, INT inCardID);
    virtual void main();
    static void permissionWhiteIn(BOOL inSw);
    static BOOL isStart();
    static BOOL isFade();

private:
    void setTimer(TIMEQ inTime) { m_tqTimer = inTime; }
    BOOL timeOver();
    void soraBossDeadDemoMain_();
    void rikuBossDeadDemoMain_();
    CEntity* createGetCard_(const CHAR* inAttachBoneName, INT inMotNo);
    void movePlayer_();
};

namespace BossDeadDemo {
INT GetCardID(INT inEnemyID, INT inAppearNo);
INT GetCardKind(INT inEnemyID, INT inAppearNo); /* CARD_KIND */
INT GetCardNumber(INT inEnemyID, INT inAppearNo);
BOOL IsRikuGet(INT inEnemyID, INT inAppearNo);
void NotifyGetCard(const CBattleEnemy* inEnemy);
}

#endif /* GAME_ENEMY_BOSSDEADDEMO_H */
