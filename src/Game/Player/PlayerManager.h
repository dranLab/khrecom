#ifndef GAME_PLAYER_PLAYERMANAGER_H
#define GAME_PLAYER_PLAYERMANAGER_H

#include "khtypes.h"
#include "System/Vector.h"
#include "System/Task.h"

class CPlayer;
class CEntity;
class CDebugWindow;
class CPathCamera;
class CCollMapCamera;
namespace CardBattle {
class CCardBattle;
}
namespace Summon {
class CSummon;
}
namespace Friend {
class CFriend;
}

namespace PLM {

struct RB_DATA {
    UINT nState;
    USHORT nData;
    USHORT nRCID;
    BYTE* pAddr;
    UINT nSize;

    void init();
};

extern UINT g_uiPlayerID;
extern UINT g_uiPlayerExID;
extern UINT g_uiBattleCommonRCID;
extern BOOL g_bIsLastSceneBattle;
extern UINT g_uiRsrcSizeTotal;
extern void* g_pRsrcFirst;
extern UINT g_nRCDataBlockSize[57];
extern const UINT g_uiPlayerRcid[7];
extern const UINT g_uiFriendRcid[14];
extern const UINT g_uiSummonRcid[8];

class CPlayerManager : public CTask {
private:
    static CPlayerManager* m_pInstance;
    CardBattle::CCardBattle* m_pCB;     /* 0x040 */
    CPlayer* m_pPlayer;                 /* 0x044 */
    CPlayer* m_pPlayerEx;               /* 0x048 */
    Friend::CFriend* m_pFriend;         /* 0x04c */
    Friend::CFriend* m_pFriendEx;       /* 0x050 */
    Summon::CSummon* m_pSummon;         /* 0x054 */
    UINT m_uiFlag;                      /* 0x058 */
    INT m_iStep;                        /* 0x05c */
    TIMEQ m_tqTimer;                    /* 0x060 */
    BYTE* m_pRsrcCurrent;               /* 0x064 */
    BYTE* m_pRsrcEnd;                   /* 0x068 */
    UINT m_uiRemain;                    /* 0x06c */
    UINT m_uiCurrentBlock;              /* 0x070 */
    RB_DATA m_blockData[16];            /* 0x074 */
    UINT m_uiReserve;                   /* 0x174 */
    UINT m_uiReserveBuf[6];             /* 0x178 */
    CPathCamera* m_pPathCamera;         /* 0x190 */
    CCollMapCamera* m_pCollMapCamera;   /* 0x194 */
    HANDLE m_hDeadSE;                   /* 0x198 */
    FVECTOR m_vWind;                    /* 0x1a0 */
    char m_strBlock[17][64];            /* 0x1b0 */
    CDebugWindow* m_pWindow;            /* 0x5f0 */

public:
    CPlayerManager(CTask* inParent);
    CPlayerManager();
    virtual ~CPlayerManager();
    void init();
    virtual void main();

private:
    void stateBattle();
    UINT getRCData_toCard(UINT inID);
    UINT getRCData_toSkill(UINT inID);
    UINT getRCID_toRCData(UINT inID);
    void initRsrcBlock();
    INT getRsrcBlock(UINT inID);
    void setReserveLoad();
    void procPlayerDead();
    void setUseRsrcBlockRCID(UINT inRCID, INT inFlag);
    void exeWind();
    UINT getPlayerRsrcGrpSize(INT nMemberID);

public:
    static CPlayerManager* getPLM() { return m_pInstance; }
    void setReqLoad(UINT inID, INT inReady);
    void setReqLoadFirst();
    INT isCompleteLoad(UINT inID);
    INT isCompleteLoadRCID(UINT inRCID);
    INT isCompleteLoadCard(UINT inID);
    UINT getRCID_toSkill(UINT inID);
    INT isTraverseTown() { return m_uiFlag & 0x80; }
    INT isHalloweenTown() { return m_uiFlag & 0x100; }
    INT is100AcreWood() { return m_uiFlag & 0x200; }
    INT isZexionBattle() { return m_uiFlag & 0x800; }
    INT isJaferBattle() { return m_uiFlag & 0x2000; }
    INT isHookBattle() { return m_uiFlag & 0x4000; }
    INT isMarluxia3Battle() { return m_uiFlag & 0x8000; }
    INT isTricksterBattle() { return m_uiFlag & 0x10000; }
    void dbgDispTTY_RsrcBlock();
    void setUseRsrcBlockSkill(UINT inSkillID, INT inFlag);
    INT removeRsrcBlock(RB_DATA& inData);
    void notifyPlayer(CPlayer* inPlayer, INT inEx);
    CPlayer* getPlayer() const { return m_pPlayer; }
    CPlayer* getPlayerEx() const { return m_pPlayerEx; }
    void notifyPlayerDead(INT inFlag);
    void notifyDeadSE(HANDLE inHandle) { m_hDeadSE = inHandle; }
    void calcFieldStartPos();
    CEntity* createFriend(UINT inID, CTask* inParent);
    void notifyFriend(Friend::CFriend* inFriend, INT inFlag, INT inEx);
    INT getAppearFriendCard(INT inNum, INT inFlag);
    UINT getFriendID_toCard(UINT inID);
    UINT getFriendID_toSkill(UINT inID);
    Friend::CFriend* getFriend() const { return m_pFriend; }
    INT isFriendReady() { return m_uiFlag & 0x2; }
    Friend::CFriend* getFriendEx() const { return m_pFriendEx; }
    INT isFriendExReady() { return m_uiFlag & 0x40; }
    CEntity* createSummon(UINT inID, CTask* inParent);
    void notifySummon(Summon::CSummon* inSummon, INT inFlag);
    UINT getSummonID_toCard(UINT inID);
    UINT getSummonID_toSkill(UINT inID);
    Summon::CSummon* getSummon() const { return m_pSummon; }
    INT isSummonReady() { return m_uiFlag & 0x20; }
    void enableWind(INT inSet);
    INT isEnableWind() { return m_uiFlag & 0x1000; }
    void setWind(FVECTOR* inVel);
    void addWindXYZ(FVECTOR* inVel);
    FVECTOR& getWind() { return m_vWind; }

    UINT getFlag() const; // not in the original: Game/Player/Friend reads m_uiFlag & 0x100 (isHalloweenTown)
};

/* Not emitted in PlayerManager.cpp (all inline members of the class are emitted there, and the original has no
 * getFlag). */
#ifndef GAME_PLAYER_PLAYERMANAGER_CPP
inline UINT CPlayerManager::getFlag() const {
    return m_uiFlag;
}
#endif

void* getBattleRsrcFirst();
UINT getBattleRsrcSize();
void mallocBattleRsrcBuffer(BOOL inAssert);
void freeBattleRsrcBuffer();
void setReqLoadPlayer(UINT inID);
void setReqLoadBattlePlayer(UINT inID);
void removePlayerRsrc(UINT inID);
void removeBattlePlayerRsrc(UINT inID);
CEntity* createPlayer(UINT inID, CTask* inParent, INT inEx);
void destroyPlayer();
void addIntBR(UINT nIdx);
void addStockBR(UINT nSkillID);
void calcFavoriteStockCount();
void calcDuelWin();
void setVibration(INT inID);
INT isTaskEntity(HANDLE inHandle);
INT isEntityTypeID(INT inID);
INT getWorldRotY(FLOAT& outRotY, const FMATRIX& inMat);

} // namespace PLM

#endif /* GAME_PLAYER_PLAYERMANAGER_H */
