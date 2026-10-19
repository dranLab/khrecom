#ifndef GAME_ENEMY_MAPENEMYMANAGER_H
#define GAME_ENEMY_MAPENEMYMANAGER_H

#include "khtypes.h"
#include "System/Task.h"

struct MAP_ENEMY_PARAM {
    char szCode[4];
    UINT uiVersion;
    FLOAT fAppearDist;
    FLOAT fDisappearDist;
    FLOAT fFaintTime;
    FLOAT fStopTime;
    FLOAT fConfuseTime;
    FLOAT fDeepDarknessAppearDist;
    FLOAT fDeepDarknessDisappearDist;
    FLOAT fLoomingDarknessAppearDist;
    FLOAT fLoomingDarknessDisappearDist;
};

class CMapEnemyManager : public CTask {
    friend class CMapEnemy; // PARTIAL: added for Game/Enemy/MapEnemy (reads the distances directly)
private:
    INT m_iStep;                        /* 0x40 */
    INT m_iEnemyAppearNum;              /* 0x44 */
    MAP_ENEMY_PARAM* m_pMapEnemyParam;  /* 0x48 */
    FLOAT m_fAppearDistance;            /* 0x4c */
    FLOAT m_fDisappearDistance;         /* 0x50 */

public:
    CMapEnemyManager(CTask* inParent);
    virtual ~CMapEnemyManager();
    virtual void main();
    virtual void disp();
    void setup();
    void initRoomCreate();
    FLOAT getAppearDistance() const;
    FLOAT getDisappearDistance() const;
    TIMEQ getFaintTime() const;
    TIMEQ getStopTime() const;
    TIMEQ getConfuseTime() const;
    void setEntityFlagAllEnemies(UINT inSet, UINT inMask);
    BOOL canAppear() const;
    void deleteEnemies();
    static void setEncountEnemy(SHORT inEnemyID, SHORT inUniqueID, BYTE inHitPlayerAttack, BYTE inAppearType);
    static void setEncountEventEnemy(INT inFormationNo, BYTE inHitPlayerAttack);
    static void cleanupRoomMapEnemyInfo();

private:
    BOOL init_();
    BOOL initMapCardParam_();
    BOOL initMapEnemyParam_();
    BOOL canCreateMapEnemy_(BYTE inUniqueID);
};

#endif /* GAME_ENEMY_MAPENEMYMANAGER_H */
