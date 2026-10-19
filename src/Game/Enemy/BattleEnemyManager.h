#ifndef GAME_ENEMY_BATTLEENEMYMANAGER_H
#define GAME_ENEMY_BATTLEENEMYMANAGER_H

#include "khtypes.h"
#include "System/Debug.h"
#include "System/Task.h"

class CEntity;
struct OSD_HEADER;
struct OBJ_FORMATION;
struct OBJ_GROUP;

/* Other units still declare their own partial copies of this class (Game/GameSubField, Game/Enemy/Heartless, ...). */
class CBattleEnemyManager : public CTask {
private:
    INT m_iStep;                    /* 0x40 */
    TIMEQ m_tqTimer;                /* 0x44 */
    BOOL m_bDead;                   /* 0x48 */
    BOOL m_bAppearPremiumPrize;     /* 0x4c */
    BOOL m_bAppearRoulettePrize;    /* 0x50 */
    BOOL m_bPremiumPrizeGet;        /* 0x54 */
    BOOL m_bRoulettePrizeGet;       /* 0x58 */
    BOOL m_bCreateCard;             /* 0x5c */
    BOOL m_bFaintContinue;          /* 0x60 */
    BOOL m_bCheckBeforeCardBattle;  /* 0x64 */
    INT m_iNextGroup;               /* 0x68 */
    OSD_HEADER* m_pHeader;          /* 0x6c */
    OBJ_FORMATION* m_pFormation;    /* 0x70 */
    INT m_iTotalEnemyNum;           /* 0x74 */
    INT m_iGroupIndex;              /* 0x78 */
    INT m_iNextGroupNum;            /* 0x7c */
    INT m_iAppearGroupCount;        /* 0x80 */
    int m_iAppearGroup[16];         /* 0x84 */
    OBJ_GROUP* m_pGroup;            /* 0xc4 */
    HANDLE m_hDidCardBattleEnemy;   /* 0xc8 */
    HANDLE m_hMonstroEventBattle;   /* 0xcc */
    static INT m_iBeforeFormation;

public:
    CBattleEnemyManager(CTask* inParent);
    virtual ~CBattleEnemyManager();
    static void clearBeforeFormation();
    virtual void main();
    virtual void disp();
    void setHpEnemies(INT inHp);
    SLContainerT<CTask>* getFirstBattleEnemy();
    HANDLE getNearestBattleEnemy(CEntity& inEntity);
    HANDLE getLockOnBattleEnemy(CEntity& inEntity);
    INT getEnemyCount() const;
    INT getPrizeCount() const;
    BOOL canCardBattle(HANDLE inHandle);
    BOOL canCreateEnemyCard() const;
    BOOL isAppearEnd() const;
    BOOL checkBattleEnemyFlag(ULONGLONG inFlag);
    void notifyDead(HANDLE inHandle);
    INT removePrize();
    CEntity* createRandomFormationEnemy(const FVECTOR& inPos, INT inCheckID);
    CEntity* createEnemy(INT inGroupID, INT inID, const FVECTOR& inPos, FLOAT inDir, INT inParam);
    CEntity* createPremiumPrize(const FVECTOR& inPos, INT inEnemyID);
    CEntity* createRoulettePrize(const FVECTOR& inPos);
    void monstroEventBattleGaugeUp();
    void warpPrize(const FVECTOR& inCenter);
    void allGetPrize(UINT inFlag);
    void prizeFlyHigh(BOOL bAll);
    void deleteAllEnemies();
    void setDidCardBattleEnemy(HANDLE inHandle) { m_hDidCardBattleEnemy = inHandle; }
    INT getTotalEnemyNum() const { return m_iTotalEnemyNum; }
    BOOL isAppearPremiumPrize() const { return m_bAppearPremiumPrize; }
    BOOL isAppearRoulettePrize() const { return m_bAppearRoulettePrize; }
    BOOL isPremiumPrizeGet() const { return m_bPremiumPrizeGet; }
    BOOL isRoulettePrizeGet() const { return m_bRoulettePrizeGet; }
    INT getChildrenCount() const { return getChildren().getSize(); }
    void setAppearPremiumPrize(BOOL inSw) { m_bAppearPremiumPrize = inSw; }
    void setAppearRoulettePrize(BOOL inSw) { m_bAppearRoulettePrize = inSw; }
    void setCreateCard(BOOL inSw) { m_bCreateCard = inSw; }
    void setCheckBeforeCardBattle(BOOL inSw) { m_bCheckBeforeCardBattle = inSw; }
    void notifyPremiumPrizeGet() {
        TRACE("\x83v\x83\x8c\x83~\x83" "A\x83\x80\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
        m_bPremiumPrizeGet = TRUE;
    }
    void notifyRoulettePrizeGet() {
        TRACE("\x83\x8b\x81[\x83\x8c\x83" "b\x83g\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
        m_bRoulettePrizeGet = TRUE;
    }

private:
    OSD_HEADER* getOSD_();
    INT createBattleEnemies_();
    INT getAppearGroupCount_(INT inGroupNo);
    OBJ_FORMATION* getFormation_();
    void startNearestCardBattleEnemy_();
    void endBattle_(BYTE inStatus);
    BOOL isBadStatusContinued_();
    BOOL isEndBattle_();
    void exitBattle_();
};

#endif /* GAME_ENEMY_BATTLEENEMYMANAGER_H */
