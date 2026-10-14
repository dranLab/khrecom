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
    UBYTE _m_0x1d0[0x114];       /* 0x1d0 .. 0x2e0 */
    PlayerCamera::CPlayerCamera* m_pCam; /* 0x2e4 */
    // PARTIAL: m_pRevolver split out for Game/Player/FriendKing
    UBYTE _m_0x2e8[0x8];         /* 0x2e8 m_pGauge, m_pCB */
    CardBattle::CCardRevolver* m_pRevolver; /* 0x2f0 */
    UBYTE _m_0x2f4[0xc];         /* 0x2f4 .. 0x2fc */
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
    UBYTE _m_0x328[0x1f0];       /* 0x328 .. 0x514 */
    UINT m_uiAttackID;           /* 0x518 */
    UBYTE _m_0x51c[0x200];       /* 0x51c .. 0x718 */
    HANDLE m_hTarget;            /* 0x71c */
    HANDLE m_hLockon;            /* 0x720 */

public:
    CPlayer(CTask* inParent);
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
    INT isBattleFlag(UINT inFlag) { return m_uiBattleFlag & inFlag; } // PARTIAL: added for Game/Player/SummonMushu
    // PARTIAL: added for Game/Gimmick/GimmickJumpStand
    INT getJumpState() const;
    static void notifyGimmickJump(HANDLE inHandle, INT inPower);
    // PARTIAL: added for Game/Player/FriendBeast (also out of line: setPlayerFlag__7CPlayerUiUi)
    void setPlayerFlag(UINT inSet, UINT inMask) { UINT& f = m_uiPlayerFlag; f = (f | inSet) & ~inMask; }
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

#endif /* GAME_PLAYER_PLAYER_H */
