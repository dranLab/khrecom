#ifndef GAME_ENEMY_MONSTROEVENTBATTLE_H
#define GAME_ENEMY_MONSTROEVENTBATTLE_H

#include "khtypes.h"
#include "System/Task.h"

struct OSD_HEADER;

class CMonstroEventBattle : public CTask {
private:
    OSD_HEADER* m_pHeader;    /* 0x40 */
    INT m_iStep;              /* 0x44 */
    INT m_iGauge;             /* 0x48 */
    FLOAT m_fGauge;           /* 0x4c */
    TIMEQ m_tqGaugeDownTimer; /* 0x50 */
    TIMEQ m_tqWait;           /* 0x54 */
    INT m_iGroup;             /* 0x58 */
    INT m_iEnemyNum;          /* 0x5c */
    TIMEQ m_tqLimitTime;      /* 0x60 */
    INT m_iTotalDead;         /* 0x64 */
    TIMEQ m_tqGaugeDown;      /* 0x68 */
    BOOL m_bReraise;          /* 0x6c */
    HANDLE m_hGauge;          /* 0x70 */
    static BOOL m_bHelp;
    static INT m_iAppearEnemyCount;

public:
    CMonstroEventBattle(CTask* inParent);
    virtual ~CMonstroEventBattle();
    BOOL setup();
    virtual void main();
    BOOL isFullGauge() const;
    void gaugeUp();
    void gaugeDown();
    INT isEnd() const { return m_iStep == 10; }
    static BOOL isMonstroEventBattleScene();
    static BOOL isHelp();
    static void addAppearEnemyCount();

private:
    INT createEnemies_();
    void setLife_();
    BOOL isTimeOver_();
};

#endif /* GAME_ENEMY_MONSTROEVENTBATTLE_H */
