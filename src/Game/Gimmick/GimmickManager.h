#ifndef GAME_GIMMICK_GIMMICKMANAGER_H
#define GAME_GIMMICK_GIMMICKMANAGER_H

#include "khtypes.h"
#include "System/Task.h"
#include "Game/SceneInfo.h"

class CEntity;
union FVECTOR;
class CDebugWindow;

/* DropPrize and CARD_KIND: other includers (decompiled before this unit) define DropPrize / CARD_KIND themselves,
 * so the full definitions are only seen by Game/Gimmick/GimmickManager.cpp. */
#ifdef GAME_GIMMICK_GIMMICKMANAGER_CPP
enum CARD_KIND {
    CARD_KIND_None = 0 /* values not needed yet */
};

struct DropPrize {
    INT StockKind;
    CARD_KIND Kind;
    SHORT Num;
    SHORT No;
};
#else
struct DropPrize;
#endif

struct GimParam; /* Game/Gimmick/BaseGimmick.h */

struct GimmickEveryCardProb {
    SHORT CardID;
    SHORT Prob;
};

struct MapCardProb {
    short int CardNumProb[10];
    SHORT KeyProb;
    SHORT JokerProb;
    SHORT ProbNum;
    GimmickEveryCardProb* pProb;
};

struct GimmickDropCard {
    SHORT RoomID;
    SHORT MaxNum;
};

struct BallDropTable {
    unsigned char type[6];
    unsigned char prob[6];
    unsigned char num[6];
};

class CGimmickManager : public CTask {
private:
    INT m_iStep;                                    /* 0x040 */
    INT m_NomalCardDropProbNum;                     /* 0x044 */
    INT m_TreasureCardDropProbNum;                  /* 0x048 */
    short int m_CardProbNum[10];                    /* 0x04c */
    SHORT m_WorldGimNum;                            /* 0x060 */
    BYTE m_GPrizeTableNum;                          /* 0x062 */
    BYTE m_RPrizeTableNum;                          /* 0x063 */
    GimmickDropCard m_CardDropRoomMaxTable[28];     /* 0x064 */
    GimmickEveryCardProb* m_pNomalCardDropProbTable;   /* 0x0d4 */
    GimmickEveryCardProb* m_pTreasureCardDropProbTable; /* 0x0d8 */
    GimParam* m_pGimParam;                          /* 0x0dc */
    BallDropTable* m_pGBallDropTable;               /* 0x0e0 */
    BallDropTable* m_pRBallDropTable;               /* 0x0e4 */
    MapCardProb m_MapCardProb;                      /* 0x0e8 */
    FLOAT m_ThrowDirPow;                            /* 0x108 */
    FLOAT m_ThrowMaxAngle;                          /* 0x10c */
    FLOAT m_ThrowMinAngle;                          /* 0x110 */

public:
    CGimmickManager(CTask* inParent);
    virtual ~CGimmickManager();
    CTaskSLCList& getChildren() { return CTask::getChildren(); }
    virtual void main();
    virtual void disp();
    HANDLE getNearestGimmick(CEntity& inEntity);
    void initRoomCreate();
    INT getStorageGimmickIndex(INT inUniqueID);
    INT getStorageGimmickState(INT inUniqueID);
    BOOL setStorageGimmickState(INT inUniqueID, BYTE inState);
    CEntity* getDoor(DOOR_ID inDoorID, FVECTOR* outPos, FVECTOR* outRot);
    DropPrize DecideMapCardPrize(BOOL in_enableKeyCard);
    DropPrize DecideTreasurePrize();
    BOOL DecideTrapTresureBox();
    void createPrize(const DropPrize& in_Prize, INT in_Style, const FVECTOR* in_pPos, const FVECTOR* in_pRot);
    GimParam* getGimParam(INT in_GimmickID);
    INT DecidePrize(INT in_GimmickID, const FVECTOR* Pos, const FVECTOR* Rot, INT NotCardDropFlag);
    FLOAT getThrowDirPow() { return m_ThrowDirPow; }
    FLOAT getThrowMaxAngle() { return m_ThrowMaxAngle; }
    FLOAT getThrowMinAngle() { return m_ThrowMinAngle; }
    MapCardProb* getDecideUseMapCard() { return &m_MapCardProb; }
    void deleteAllGimmicks();
    BOOL getSavePointPos(FVECTOR& outPos);
    CEntity* getGimClassEntityFirst(INT in_ClassType);
    void changeTreasureBoxLock(BOOL in_Enable);
    void setThrowDirPow(FLOAT in_dirpow) { m_ThrowDirPow = in_dirpow; }
    void setThrowMaxAngle(FLOAT in_max) { m_ThrowMaxAngle = in_max; }
    void setThrowMinAngle(FLOAT in_min) { m_ThrowMinAngle = in_min; }
    void GimDebugTrace(const DropPrize* prize);

private:
    BOOL init_();
    BOOL AfterInit_();
    BOOL LoadCommonResrc();
    BOOL LoadWorldResrc();
    BOOL LoadCommonResrcMapCard();
    BOOL LoadWorldResrcMapCard();
    BOOL LoadFloorPrizeDropTable();
    DropPrize DecideGimmickCard(INT DropProbNum, GimmickEveryCardProb* pDropDataTable, SHORT* pCardProbTable);
    DropPrize DecideGimmickCard_PossibleCheck(INT DropProbNum, GimmickEveryCardProb* pDropDataTable, SHORT* pCardProbTable);
    DropPrize DecideGimmickCard_PossibleNoCheck(INT DropProbNum, GimmickEveryCardProb* pDropDataTable, SHORT* pCardProbTable);
    DropPrize DecideMapCard(INT DropProbNum, GimmickEveryCardProb* pDropDataTable, SHORT* pCardProbTable);
    INT DecideGimmickCardNo(SHORT* pCardProbTable);
    DropPrize DecideKeyTreasurePrize();
    DropPrize DecideOpenCountTreasurePrize();
#ifdef GAME_GIMMICK_GIMMICKMANAGER_CPP
    BOOL checkPossibleCard(CARD_KIND in_Kind);
    CARD_KIND changePossibleCard(CARD_KIND in_Kind);
#endif
    INT DecidePrizeKind(const GimParam* in_pParam);
    DropPrize DecideGPrize(INT in_DataIndex);
    DropPrize DecideRPrize(INT in_DataIndex);
    DropPrize checkAddCardTresureBox(DropPrize in_Prize);
    DropPrize checkAddCardGimmick(DropPrize in_Prize);
    DropPrize checkAddCardMapCard(DropPrize in_Prize);
    void createPrizeType_00(const DropPrize& in_Prize, const FVECTOR* in_pPos);
    void createPrizeType_01(const DropPrize& in_Prize, const FVECTOR* in_pPos);
    void createPrizeType_02(const DropPrize& in_Prize, const FVECTOR* in_pPos, const FVECTOR* in_pRot);
    void createPrizeType_03(const DropPrize& in_Prize, const FVECTOR* in_pPos, const FVECTOR* in_pRot);
    void createPrizeType_04(const DropPrize& in_Prize, const FVECTOR* in_pPos);
};

class CGimmickProbDebug : public CTask {
public:
    CDebugWindow* m_pWindow; /* 0x40 */

    CGimmickProbDebug(CTask* pParent);
    virtual ~CGimmickProbDebug();
    virtual void main();
};

#endif /* GAME_GIMMICK_GIMMICKMANAGER_H */
