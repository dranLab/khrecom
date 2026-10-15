// PARTIAL: added for Game/Gimmick/GimmickSavePoint
#ifndef GAME_PLAYER_PLAYER_H
#define GAME_PLAYER_PLAYER_H

#include "khtypes.h"
#include "Game/Entity.h"

namespace CardBattle {
class CCardRevolver; // PARTIAL: added for Game/Player/FriendKing
}

namespace PlayerCamera {
class CPlayerCamera; // PARTIAL: added for Game/Player/FriendJack
}

namespace Gauge {
class CLockonMarker; // PARTIAL: added for Game/Player/Summon
}

namespace PlayerWeapon {
class CPlayerWeapon; // PARTIAL: added for Game/Player/FriendDonald
}

enum PlayerState { // complete (from types.h)
    INIT = 0,
    START_WAIT = 1,
    STAND = 2,
    WALK = 3,
    RUN = 4,
    WALL = 5,
    AVOID = 6,
    JUMP = 7,
    GLIDE = 8,
    HANG = 9,
    ATTACK_FLD = 10,
    ATTACK = 11,
    DAMAGE = 12,
    FLICKS = 13,
    ITEM = 14,
    HIDE = 15,
    LADDER = 16,
    TREASUREBOX = 17,
    CHARGE = 18,
    MAGIC = 19,
    ROOMCREATION = 20,
    WORLDSELECT = 21,
    SKILL = 22,
    ESCAPE = 23,
    BADCONDITION = 24,
    GIMMICKCARD = 25,
    FRIEND = 26,
    SUMMON = 27,
    END_BTL = 28,
    DEAD = 29,
    CARRY = 30,
    TALK = 31,
    TRANSFORM = 32,
    JUMP_2ND = 33,
    DUEL = 34,
    FOOTING = 35,
    PRONOUNCE = 36,
    DBGWAIT = 37
};

class CPlayer : public CEntity {
private:
    // PARTIAL: m_pCam split out for Game/Player/FriendJack
    // PARTIAL: m_pWeapon split out for Game/Player/FriendDonald
    UBYTE _m_0x1d0[0x110];       /* 0x1d0 .. 0x2dc */
    PlayerWeapon::CPlayerWeapon* m_pWeapon; /* 0x2e0 */
    PlayerCamera::CPlayerCamera* m_pCam; /* 0x2e4 */
    // PARTIAL: m_pRevolver split out for Game/Player/FriendKing
    UBYTE _m_0x2e8[0x8];         /* 0x2e8 m_pGauge, m_pCB */
    CardBattle::CCardRevolver* m_pRevolver; /* 0x2f0 */
    Gauge::CLockonMarker* m_pMarker; /* 0x2f4 */ // PARTIAL: split out for Game/Player/Summon
    UBYTE _m_0x2f8[0x8];         /* 0x2f8 .. 0x2fc */
    PlayerState m_PlayerState;   /* 0x300 */
    UBYTE _m_0x304[0x8];         /* 0x304 m_tqStateTimer, m_iSubState */
    UINT m_uiPlayerFlag;         /* 0x30c */ // PARTIAL: split out for Game/Player/FriendBeast
    UBYTE _m_0x310[0x4];         /* 0x310 m_uiActionFlag */
    UINT m_uiConditionFlag;      /* 0x314 */ // PARTIAL: split out for Game/Gimmick/GimmickWindowLid
    UINT m_uiTrgFlag;            /* 0x318 */ // PARTIAL: split out for Game/Player/SummonMushu
    UINT m_uiBattleFlag;         /* 0x31c */
    INT m_iHP;                   /* 0x320 */
    INT m_iMaxHP;                /* 0x324 */
    // PARTIAL: m_uiAttackID split out for Game/Player/FriendKing
    // PARTIAL: m_vCenterPos, m_vAppearPos, m_vJumpStartPos, m_hMark, m_vMarkPos split out for Game/Player/PlayerWeapon
    UBYTE _m_0x328[0x24];        /* 0x328 m_iCP .. 0x348 m_uiStep */
    INT m_iCount;                /* 0x34c */ // PARTIAL: split out for Game/Player/Friend
    UBYTE _m_0x350[0x50];        /* 0x350 .. 0x390 m_vWeaponTopPos */
    FVECTOR m_vCenterPos;        /* 0x3a0 */
    UBYTE _m_0x3b0[0x14];        /* 0x3b0 m_vStartPos, 0x3c0 m_fStartDir */
    FLOAT m_fDir;                /* 0x3c4 */ // PARTIAL: split out for Game/Player/SummonCloud
    UBYTE _m_0x3c8[0x28];        /* 0x3c8 m_fScale .. 0x3e0 m_vOldSafeField */
    FVECTOR m_vAppearPos;        /* 0x3f0 */
    UBYTE _m_0x400[0xc0];        /* 0x400 .. 0x4b0 */
    FVECTOR m_vJumpStartPos;     /* 0x4c0 */
    UBYTE _m_0x4d0[0x48];        /* 0x4d0 .. 0x514 */
    UINT m_uiAttackID;           /* 0x518 */
    UBYTE _m_0x51c[0x200];       /* 0x51c .. 0x718 */
    HANDLE m_hTarget;            /* 0x71c */
    HANDLE m_hLockon;            /* 0x720 */
    HANDLE m_hMark;              /* 0x724 */
    FVECTOR m_vMarkPos;          /* 0x730 */

public:
    CPlayer(CTask* inParent);
    CPlayer(CTask* inParent, UINT inID, INT inEx); // PARTIAL: added for Game/Player/PlayerManager
    void notifyDead(INT inFlag);                   // PARTIAL: added for Game/Player/PlayerManager
    // PARTIAL: added for Game/Player/PlayerWeapon
    FVECTOR& getCenterPos() { return m_vCenterPos; }
    FVECTOR& getAppearPos() { return m_vAppearPos; }
    FVECTOR& getJumpStartPos() { return m_vJumpStartPos; }
    FVECTOR& getMarkPos() { return m_vMarkPos; }
    PlayerCamera::CPlayerCamera* getPlayerCamera() { return m_pCam; } // PARTIAL: added for Game/Player/FriendJack
    INT getPlayerState() { return m_PlayerState; }
    INT getHP() { return m_iHP; }
    INT getMaxHP() { return m_iMaxHP; }
    void addHP(INT inHP, INT inFlag, INT inEffect);
    void setPrizeHealing(INT inTime); // PARTIAL: added for Game/Player/SummonTink
    void cancelLockon();
    BOOL cmdRevolver_isEmpty(); // PARTIAL: added for Game/Atel/AtelTutorial
    // PARTIAL: added for Game/Gimmick/GimmickWindowLid
    void setConditionFlag(UINT inSet, UINT inMask) { m_uiConditionFlag = (m_uiConditionFlag | inSet) & ~inMask; }
    // PARTIAL: added for Game/Player/FriendKing
    CardBattle::CCardRevolver* getRevolver() { return m_pRevolver; }
    INT isConditionFlag(UINT inFlag) { return m_uiConditionFlag & inFlag; }
    UINT getAttackID() { return m_uiAttackID; }
    void setCure(INT inID, INT inPow);
    INT getCount() { return m_iCount; } // PARTIAL: added for Game/Player/Friend
    INT isBattleFlag(UINT inFlag) { return m_uiBattleFlag & inFlag; } // PARTIAL: added for Game/Player/SummonMushu
    // PARTIAL: added for Game/Player/FriendDonald
    PlayerWeapon::CPlayerWeapon* getWeapon() { return m_pWeapon; }
    void getWeaponTopPos(FVECTOR& outPos);
    // PARTIAL: added for Game/Player/SummonCloud
    void setDir(FLOAT inDir) { m_fDir = inDir; }
    // PARTIAL: added for Game/Player/Summon
    FLOAT getDir() { return m_fDir; }
    Gauge::CLockonMarker* getMarker() { return m_pMarker; }
    void calcAppearPos(FLOAT inX, FLOAT inZ, FVECTOR* inTarget);
    HANDLE isNowMark() { return m_hMark; } // PARTIAL: added for Game/Player/FriendGoofy (original name/type unknown)
    // PARTIAL: added for Game/Gimmick/GimmickJumpStand
    INT getJumpState() const;
    static void notifyGimmickJump(HANDLE inHandle, INT inPower);
    static void notifyGetPrize(INT inKind, INT inNumber, INT inState); // PARTIAL: added for Game/Gimmick/GimmickTreasureBox
    // PARTIAL: added for Game/Player/FriendBeast (also out of line: setPlayerFlag__7CPlayerUiUi)
    void setPlayerFlag(UINT inSet, UINT inMask) { UINT& f = m_uiPlayerFlag; f = (f | inSet) & ~inMask; }
    void setTrgFlag(UINT inSet, UINT inMask) { UINT& f = m_uiTrgFlag; f = (f | inSet) & ~inMask; } // PARTIAL: added for Form/Menu/WorldSelect
    // PARTIAL: added for Game/Player/FriendBeast (CPlayer's virtuals up to vtable[36])
    virtual void beginStateField();
    virtual void stateField();
    virtual void endStateField();
    virtual void beginStateBattle();
    virtual void stateBattle();
    virtual void endStateBattle();
    virtual void beginStateWait();
    virtual void stateWait();
    virtual void endStateWait();
    virtual void beginStateHide();
    virtual void stateHide();
    virtual void endStateHide();
    virtual void onWeapon(INT inEffect);
    virtual void offWeapon();
    virtual void setStateSE(INT inState, INT inKind);
    virtual INT getStateSESeq(INT inState);
    virtual void setAttack_inFriend(INT inID, INT inStock, HANDLE inTarget);
    HANDLE getLockonHandle() {
        HANDLE handle = m_hLockon;
        if (handle == 0 || handle == -1) handle = m_hTarget;
        return handle;
    }
};

extern CPlayer* g_pPlayer;

// PARTIAL: added for Game/Player/PlayerManager
struct PL_PARAM {
    UINT USE_MAP_ACTION;
    UINT USE_BTL_ACTION;
    FLOAT BODY_COLL_RADIUS;
    FLOAT BODY_COLL_HEIGHT;
    FLOAT MAX_WALK_SPEED;
    FLOAT MAX_RUN_SPEED;
    FLOAT CARRY_MOVE_SPEED;
    FLOAT JUMP_POWER;
    FLOAT HIJUMP_POWER;
    TIMEQ JUMP_TQ_START;
    TIMEQ AVOID_TQ_MUTEKI;
    TIMEQ AVOID_TQ_CANCEL;
};

namespace PL {
extern FLOAT g_fWeight;
extern FLOAT g_fJumpStopRate;
extern FLOAT g_fGlideWeight;
extern FLOAT g_fGlideSpeedLow;
extern FLOAT g_fGlideSpeedHigh;
extern FLOAT g_fGlideTurnRate;
extern PL_PARAM g_Param;
} // namespace PL

extern char* szPlayerState[38];
extern char* szAttackData[188];
extern char* szPlayerSkill[110];
extern char* szPlayerCard[67];
extern char* szAtkAnimID[71];

#endif /* GAME_PLAYER_PLAYER_H */
