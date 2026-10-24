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

// PARTIAL: added for Game/Player/PlayerRiku
#include "Game/Player/PlayerControl.h"
#include "Game/CollParam.h"
#include "Game/AttackMotion.h"
namespace PLM {
class CPlayerManager;
struct CARD; // PARTIAL: added for Game/Prize
}
namespace Gauge {
class CPlayerGauge;
class CEscapeGauge;
}
namespace CardBattle {
class CCardBattle;
}
namespace PL {
class CPlayerRiku;
}
struct PL_PARAM;
struct PATK_DATA;
class CCollMap;
class CPlayerDebug;

// PARTIAL: moved into namespace PL for Game/Player/PlayerRiku (setPlayerState__7CPlayerQ22PL11PlayerState)
namespace PL {
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
} // namespace PL
using PL::PlayerState;
using PL::INIT;
using PL::START_WAIT;
using PL::STAND;
using PL::WALK;
using PL::RUN;
using PL::WALL;
using PL::AVOID;
using PL::JUMP;
using PL::GLIDE;
using PL::HANG;
using PL::ATTACK_FLD;
using PL::ATTACK;
using PL::DAMAGE;
using PL::FLICKS;
using PL::ITEM;
using PL::HIDE;
using PL::LADDER;
using PL::TREASUREBOX;
using PL::CHARGE;
using PL::MAGIC;
using PL::ROOMCREATION;
using PL::WORLDSELECT;
using PL::SKILL;
using PL::ESCAPE;
using PL::BADCONDITION;
using PL::GIMMICKCARD;
using PL::FRIEND;
using PL::SUMMON;
using PL::END_BTL;
using PL::DEAD;
using PL::CARRY;
using PL::TALK;
using PL::TRANSFORM;
using PL::JUMP_2ND;
using PL::DUEL;
using PL::FOOTING;
using PL::PRONOUNCE;
using PL::DBGWAIT;

// PARTIAL: full member layout (and the CPlayerPad base) added for Game/Player/PlayerRiku
class CPlayer : public CEntity, public CPlayerPad {
public: // PARTIAL: members public for Game/Player/PlayerRiku
    void cmdState_setEncountWait(); // PARTIAL: added for Game/Enemy/MapEnemy
    void setCardSnatch(INT inFlag); // PARTIAL: added for Game/CardRevolver
#ifdef GAME_PLAYER_PLAYER_CPP
    CStateTemplate<CPlayer> m_State[4]; /* 0x21c */
#else
    UBYTE _m_0x21c[0x80];        /* 0x21c CStateTemplate<CPlayer> m_State[4] */
#endif
public:
    INT m_iState;                /* 0x29c */
public: // PARTIAL: members public for Game/Player/PlayerRiku
    FLOAT m_fAnalogPower;        /* 0x2a0 */
    FLOAT m_fAnalogDir;          /* 0x2a4 */
    FLOAT m_fAnalogViewDir;      /* 0x2a8 */
    BOOL m_bIsBattleScene;       /* 0x2ac */
    BOOL m_bIsMinigameScene;     /* 0x2b0 */
    UINT m_uiCharaID;            /* 0x2b4 */
    UINT m_uiRCID;               /* 0x2b8 */
    INT m_iBoneWeapon;           /* 0x2bc */
    INT m_iBoneCenter;           /* 0x2c0 */
    INT m_iBoneChest;            /* 0x2c4 */
    INT m_iBoneHead;             /* 0x2c8 */
    INT m_iBoneLFoot;            /* 0x2cc */
    INT m_iBoneRFoot;            /* 0x2d0 */
    PLM::CPlayerManager* m_pPLM; /* 0x2d4 */
    PL_PARAM* m_pParam;          /* 0x2d8 */
    PATK_DATA* m_pAtkData;       /* 0x2dc */
    PlayerWeapon::CPlayerWeapon* m_pWeapon; /* 0x2e0 */
    PlayerCamera::CPlayerCamera* m_pCam;    /* 0x2e4 */
    Gauge::CPlayerGauge* m_pGauge;          /* 0x2e8 */
    CardBattle::CCardBattle* m_pCB;         /* 0x2ec */
    CardBattle::CCardRevolver* m_pRevolver; /* 0x2f0 */
    Gauge::CLockonMarker* m_pMarker;        /* 0x2f4 */
    Gauge::CEscapeGauge* m_pEscape;         /* 0x2f8 */
    PlayerState m_PrePlayerState; /* 0x2fc */
    PlayerState m_PlayerState;   /* 0x300 */
    TIMEQ m_tqStateTimer;        /* 0x304 */
    INT m_iSubState;             /* 0x308 */
    UINT m_uiPlayerFlag;         /* 0x30c */
    UINT m_uiActionFlag;         /* 0x310 */
    UINT m_uiConditionFlag;      /* 0x314 */
    UINT m_uiTrgFlag;            /* 0x318 */
    UINT m_uiBattleFlag;         /* 0x31c */
    INT m_iHP;                   /* 0x320 */
    INT m_iMaxHP;                /* 0x324 */
    INT m_iCP;                   /* 0x328 */
    INT m_iMaxCP;                /* 0x32c */
    INT m_iAP;                   /* 0x330 */
    INT m_iEXP;                  /* 0x334 */
    INT m_iLV;                   /* 0x338 */
    INT m_iMP;                   /* 0x33c */
    INT m_iDP;                   /* 0x340 */
    INT m_iMaxDP;                /* 0x344 */
    UINT m_uiStep;               /* 0x348 */
    INT m_iCount;                /* 0x34c */
    TIMEQ m_tqTime;              /* 0x350 */
    TIMEQ m_tqFrameRate;         /* 0x354 */
    FLOAT m_fFrameRate;          /* 0x358 */
    FLOAT m_fWeight;             /* 0x35c */
    FLOAT m_fGravity;            /* 0x360 */
    INT m_iPreAnimID;            /* 0x364 */
    INT m_iNowAnimID;            /* 0x368 */
    INT m_iAerialAnimShift;      /* 0x36c */
    FLOAT m_fAnimSpeed;          /* 0x370 */
public:
    INT m_iNowFaceID;            /* 0x374 */
public: // PARTIAL: members public for Game/Player/PlayerRiku
    INT m_nVoiceID;              /* 0x378 */
    int m_hEffAttack[4];         /* 0x37c */
    LPCSTR m_szEffectName;       /* 0x38c */
    FVECTOR m_vWeaponTopPos;     /* 0x390 */
    FVECTOR m_vCenterPos;        /* 0x3a0 */
    FVECTOR m_vStartPos;         /* 0x3b0 */
    FLOAT m_fStartDir;           /* 0x3c0 */
    FLOAT m_fDir;                /* 0x3c4 */
    FLOAT m_fScale;              /* 0x3c8 */
    FLOAT m_fWalkSpeed;          /* 0x3cc */
    FLOAT m_fMoveSpdXZ;          /* 0x3d0 */
    FLOAT m_fMovePower;          /* 0x3d4 */
    FVECTOR m_vOldSafeField;     /* 0x3e0 */
    FVECTOR m_vAppearPos;        /* 0x3f0 */
    FVECTOR m_vHeadPos;          /* 0x400 */
    FVECTOR m_vEnemyPos;         /* 0x410 */
    CCollMap* m_pCollMap;        /* 0x420 */
    BOOL m_bAirCheck;            /* 0x424 */
    BOOL m_bAir;                 /* 0x428 */
    BOOL m_bPreAir;              /* 0x42c */
    FLOAT m_fWallRadius;         /* 0x430 */
    FLOAT m_fFootRadius;         /* 0x434 */
    FLOAT m_fPrizeDraw;          /* 0x438 */
    FLOAT m_fCardDraw;           /* 0x43c */
    HANDLE m_hEffPrize;          /* 0x440 */
    HANDLE m_hEffLevelUp;        /* 0x444 */
    TIMEQ m_tqPrize;             /* 0x448 */
    INT m_WalkMode;              /* 0x44c WalkMode */
    TIMEQ m_tqEscape;            /* 0x450 */
    FVECTOR m_vWallPos;          /* 0x460 */
    FLOAT m_fWallDir;            /* 0x470 */
    FVECTOR* m_pLadderPos;       /* 0x474 */
    FVECTOR* m_pLadderNormal;    /* 0x478 */
    FVECTOR m_vLadderPos;        /* 0x480 */
    FLOAT m_fLadderDir;          /* 0x490 */
    INT m_iLadderStep;           /* 0x494 */
    INT m_iLadderStepMax;        /* 0x498 */
    FLOAT m_fLadderSpdY;         /* 0x49c */
    FLOAT m_fJumpDir;            /* 0x4a0 */
    FLOAT m_fJumpSpdXZ;          /* 0x4a4 */
    FLOAT m_fJumpPow;            /* 0x4a8 */
    FLOAT m_fJumpSpdY;           /* 0x4ac */
    FLOAT m_fJumpGravity;        /* 0x4b0 */
    TIMEQ m_tqJumpTimer;         /* 0x4b4 */
    FVECTOR m_vJumpStartPos;     /* 0x4c0 */
    FLOAT m_fJumpHeight;         /* 0x4d0 */
    FLOAT m_fJumpTop;            /* 0x4d4 */
    FLOAT m_fJumpLandLimit;      /* 0x4d8 */
    HANDLE m_hGimmickJump;       /* 0x4dc */
    FLOAT m_fGimmickJumpPow;     /* 0x4e0 */
    INT m_JumpState;             /* 0x4e4 */
    FVECTOR m_vHangHandPos;      /* 0x4f0 */
    FVECTOR m_vHangPos;          /* 0x500 */
    UINT m_uiAttackKind;         /* 0x510 */
    UINT m_uiPreAttackKind;      /* 0x514 */
    UINT m_uiAttackID;           /* 0x518 */
    INT m_iAttackNo;             /* 0x51c */
    INT m_iMaxCombo;             /* 0x520 */
    BOOL m_bIsAttackHit;         /* 0x524 */
    BOOL m_bIsCanCombo;          /* 0x528 */
    BOOL m_bNextTrg;             /* 0x52c */
    INT m_iNextAttack;           /* 0x530 */
    TIMEQ m_tqAttackTimer;       /* 0x534 */
    TIMEQ m_tqAttackResponse;    /* 0x538 */
    TIMEQ m_tqCardBreakTime;     /* 0x53c */
    INT m_iAttackPower;          /* 0x540 */
    INT m_iComboBoost;           /* 0x544 */
    ATK_KIND m_AtkKind;          /* 0x548 */
    ATK_ATTR m_AtkAttr;          /* 0x54c */
    FLOAT m_fDistXZ;             /* 0x550 */
    FLOAT m_fDistY;              /* 0x554 */
public:
    HANDLE m_hGimmick;           /* 0x558 */
    HANDLE m_hFooting;           /* 0x55c */
    HANDLE m_hSnatch;            /* 0x560 */
public: // PARTIAL: members public for Game/Player/PlayerRiku
#ifdef GAME_PLAYER_PLAYER_CPP
    CCollBody m_collBody;        /* 0x570 */
#else
    UBYTE m_collBody[0x110] __attribute__((aligned(16))); /* 0x570 CCollBody */
#endif
    BOOL m_bIsDead;              /* 0x680 */
    BOOL m_bDmgMuteki;           /* 0x684 */
    BOOL m_bDmgInAir;            /* 0x688 */
    TIMEQ m_tqMutekiTimer;       /* 0x68c */
    FLOAT m_fBodyRviseCoefficient; /* 0x690 */
    float m_fDefAttr[6];         /* 0x694 */
    TIMEQ m_tqBanish;            /* 0x6ac */
    TIMEQ m_tqConfuse;           /* 0x6b0 */
    TIMEQ m_tqBarrier;           /* 0x6b4 */
    TIMEQ m_tqGuard;             /* 0x6b8 */
    HANDLE m_hEffBad;            /* 0x6bc */
    HANDLE m_hEffGuard;          /* 0x6c0 */
    UINT m_uiCardID;             /* 0x6c4 */
    INT m_iCardNum;              /* 0x6c8 */
    INT m_recvMsg;               /* 0x6cc */
    UINT m_recvCardID;           /* 0x6d0 */
    INT m_recvCardNum;           /* 0x6d4 */
    UBYTE m_recvCardInfo[0x24];  /* 0x6d8 MES_CARD_INFO */
    INT m_iStockSkill;           /* 0x6fc */
    INT m_iExtraSkill;           /* 0x700 */
    INT m_iFirstSkill;           /* 0x704 */
    INT m_iStepStockSkill;       /* 0x708 */
    TIMEQ m_tqRevolverRoll;      /* 0x70c */
    TIMEQ m_tqRevolverSpeed;     /* 0x710 */
    HANDLE m_hEffCharge;         /* 0x714 */
    TIMEQ m_tqEffCharge;         /* 0x718 */
    HANDLE m_hTarget;            /* 0x71c */
    HANDLE m_hLockon;            /* 0x720 */
    HANDLE m_hMark;              /* 0x724 */
    FVECTOR m_vMarkPos;          /* 0x730 */
    FVECTOR m_vLockonPos;        /* 0x740 */
    HANDLE m_hOldLockon;         /* 0x750 */
    INT m_iOldCamMode;           /* 0x754 */
    HANDLE m_hEffCast;           /* 0x758 */
    HANDLE m_hFriend;            /* 0x75c */
    HANDLE m_hSummon;            /* 0x760 */
public:
    CPlayerDebug* m_pDebug;      /* 0x764 */

public:
    CPlayer(CTask* inParent);
    CPlayer(CTask* inParent, UINT inID, INT inEx); // PARTIAL: added for Game/Player/PlayerManager
    void notifyDead(INT inFlag);                   // PARTIAL: added for Game/Player/PlayerManager
    // PARTIAL: added for Game/Player/PlayerWeapon
#ifdef GAME_PLAYER_PLAYER_CPP
    FVECTOR& getCenterPos(); // the weak copies are emitted in the original order: defined after the class
#else
    FVECTOR& getCenterPos() { return m_vCenterPos; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    FVECTOR& getAppearPos(); // the weak copies are emitted in the original order: defined after the class
#else
    FVECTOR& getAppearPos() { return m_vAppearPos; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    FVECTOR& getJumpStartPos(); // the weak copies are emitted in the original order: defined after the class
#else
    FVECTOR& getJumpStartPos() { return m_vJumpStartPos; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    FVECTOR& getMarkPos(); // the weak copies are emitted in the original order: defined after the class
#else
    FVECTOR& getMarkPos() { return m_vMarkPos; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    PlayerCamera::CPlayerCamera* getPlayerCamera() const; // the weak copies are emitted in the original order: defined after the class
#else
    PlayerCamera::CPlayerCamera* getPlayerCamera() { return m_pCam; } // PARTIAL: added for Game/Player/FriendJack
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    INT getPlayerState() const; // the weak copies are emitted in the original order: defined after the class
#else
    INT getPlayerState() { return m_PlayerState; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    INT getHP(); // the weak copies are emitted in the original order: defined after the class
#else
    INT getHP() { return m_iHP; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    INT getLV(); // the weak copies are emitted in the original order: defined after the class
#else
    INT getLV() { return m_iLV; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    INT getEXP(); // the weak copies are emitted in the original order: defined after the class
#else
    INT getEXP() { return m_iEXP; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    void setHP(INT inHP); // the weak copies are emitted in the original order: defined after the class
#else
    void setHP(INT inHP) { m_iHP = inHP; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    void setMaxHP(INT inMaxHP); // the weak copies are emitted in the original order: defined after the class
#else
    void setMaxHP(INT inHP) { m_iMaxHP = inHP; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    void setAP(INT inAP); // the weak copies are emitted in the original order: defined after the class
#else
    void setAP(INT inAP) { m_iAP = inAP; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    void setLV(INT inLV); // the weak copies are emitted in the original order: defined after the class
#else
    void setLV(INT inLV) { m_iLV = inLV; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    void setEXP(INT inEXP); // the weak copies are emitted in the original order: defined after the class
#else
    void setEXP(INT inEXP) { m_iEXP = inEXP; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    void setMP(INT inMP); // the weak copies are emitted in the original order: defined after the class
#else
    void setMP(INT inMP) { m_iMP = inMP; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    void setMaxDP(INT inDP); // the weak copies are emitted in the original order: defined after the class
#else
    void setMaxDP(INT inDP) { m_iMaxDP = inDP; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    INT getMaxHP(); // the weak copies are emitted in the original order: defined after the class
#else
    INT getMaxHP() { return m_iMaxHP; }
#endif
    void addHP(INT inHP, INT inFlag, INT inEffect);
    void setPrizeHealing(TIMEQ inTime); // PARTIAL: added for Game/Player/SummonTink
    void exePrizeHealing();
    void fadeEffCharge();
    INT getMotIndex(INT inChara, INT inIndex);
    void setReflectGuard(INT inFlag); void setQuickBarrier(INT inFlag);
    void setTransOff(INT inFlag);
    INT chkAtelHide(INT inFlag); void _init(); void initFSM_(); static void (CPlayer::*m_pBeginStateTbl[4])(); static void (CPlayer::*m_pStateTbl[4])(); static void (CPlayer::*m_pEndStateTbl[4])(); void chkAtelControl(); void calcEnchant_DamagePoint(INT& inPoint, ATK_ATTR inAttr);
    void initRoomCreation(); void initWorldSelect();
    void cmdState_initWorldSelect(HANDLE inDoor);
    void exeEscape(); void exeGimmickRide(); INT chkGimmick(UINT inID); INT chkEscape(); void initTreasureBox(); void exeTreasureBox(); void setBanish(INT inFlag); void exeBanish(); INT exeWarp(INT inSet); void exeWind();
    void initCarry();
    void cancelLockon();
    void autoLockon();
    BOOL cmdRevolver_isEmpty(); // PARTIAL: added for Game/Atel/AtelTutorial
    INT cmdRevolver_isActive();
    BOOL cmdRevolver_rollDeck(INT inNum);
    INT chkLadderStepNow(BOOL inAir); INT getLadderStepNow(FLOAT inHeight); void initLadder(INT inStep);
    void exeQuickBarrier(); void exeAeroGuard(); void setAeroGuard(INT inFlag);
    INT isNowCardActionState();
    INT chkWalkMode();
    void exeWalk(); void setWalkAnim(); void exeStartWait(); INT isStartLadderJump(); void procDropMP(); INT chkBattleIdle(); FLOAT getAirHeight(FVECTOR* inPos, FLOAT inHeight, INT inLimit); FLOAT getFrontAirHeight(FVECTOR* inPos, FLOAT inLength, FLOAT inHeight, FLOAT inDir);
    void recoverCollMap();
    // PARTIAL: added for Game/Gimmick/GimmickWindowLid
#ifdef GAME_PLAYER_PLAYER_CPP
    void setConditionFlag(UINT inSet, UINT inMask); // the weak copies are emitted in the original order: defined after the class
#else
    void setConditionFlag(UINT inSet, UINT inMask) { m_uiConditionFlag = (m_uiConditionFlag | inSet) & ~inMask; }
#endif
    // PARTIAL: added for Game/Player/FriendKing
#ifdef GAME_PLAYER_PLAYER_CPP
    CardBattle::CCardRevolver* getRevolver() const; // the weak copies are emitted in the original order: defined after the class
#else
    CardBattle::CCardRevolver* getRevolver() { return m_pRevolver; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    INT isConditionFlag(UINT inFlag) const; // the weak copies are emitted in the original order: defined after the class
#else
    INT isConditionFlag(UINT inFlag) { return m_uiConditionFlag & inFlag; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    UINT getAttackID(); // the weak copies are emitted in the original order: defined after the class
#else
    UINT getAttackID() { return m_uiAttackID; }
#endif
    void setCure(INT inID, INT inPow);
#ifdef GAME_PLAYER_PLAYER_CPP
    INT getCount() const; // the weak copies are emitted in the original order: defined after the class
#else
    INT getCount() { return m_iCount; } // PARTIAL: added for Game/Player/Friend
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    INT isBattleFlag(UINT inFlag) const; // the weak copies are emitted in the original order: defined after the class
#else
    INT isBattleFlag(UINT inFlag) { return m_uiBattleFlag & inFlag; } // PARTIAL: added for Game/Player/SummonMushu
#endif
    // PARTIAL: added for Game/Player/FriendDonald
#ifdef GAME_PLAYER_PLAYER_CPP
    PlayerWeapon::CPlayerWeapon* getWeapon() const; // the weak copies are emitted in the original order: defined after the class
#else
    PlayerWeapon::CPlayerWeapon* getWeapon() { return m_pWeapon; }
#endif
    void getWeaponTopPos(FVECTOR& outPos); void getCaskPos(FVECTOR& inVec);
    // PARTIAL: added for Game/Player/SummonCloud
#ifdef GAME_PLAYER_PLAYER_CPP
    void setDir(const FLOAT& inDir); // the weak copies are emitted in the original order: defined after the class
#else
    void setDir(FLOAT inDir) { m_fDir = inDir; }
#endif
    // PARTIAL: added for Game/Player/Summon
#ifdef GAME_PLAYER_PLAYER_CPP
    FLOAT getDir(); // the weak copies are emitted in the original order: defined after the class
#else
    FLOAT getDir() { return m_fDir; }
#endif
#ifndef GAME_PLAYER_PLAYER_CPP /* not in the original class: emitted as weak copies if defined in the key function's unit */
    Gauge::CLockonMarker* getMarker() { return m_pMarker; }
#endif
    INT calcAppearPos(FLOAT inDist, FLOAT inRadius, FVECTOR* inCenter);
    void calcAppearPosFS(FLOAT inDist, FLOAT inRadius);
#ifdef GAME_PLAYER_PLAYER_CPP
    HANDLE isNowMark(); // the weak copies are emitted in the original order: defined after the class
#else
    HANDLE isNowMark() { return m_hMark; } // PARTIAL: added for Game/Player/FriendGoofy (original name/type unknown)
#endif
    // PARTIAL: added for Game/Player/PlayerCamera
#ifdef GAME_PLAYER_PLAYER_CPP
    HANDLE isNowLockon(); // the weak copies are emitted in the original order: defined after the class
#else
    HANDLE isNowLockon() { return m_hLockon; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    FVECTOR& getLockonPos(); // the weak copies are emitted in the original order: defined after the class
#else
    FVECTOR& getLockonPos() { return m_vLockonPos; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    FVECTOR& getStartPos(); // the weak copies are emitted in the original order: defined after the class
#else
    FVECTOR& getStartPos() { return m_vStartPos; }
#endif
    BOOL isNowIdleAnim();
    // PARTIAL: added for Game/Gimmick/GimmickJumpStand
    INT getJumpState() const;
    static void notifyGimmickJump(HANDLE inHandle, INT inPower);
    static void notifyGetPrize(INT inKind, INT inNumber, INT inState);
    // PARTIAL: added for Game/Prize (getter names unknown)
    void notifyReturnCard(const CARD* inCard);
#ifndef GAME_PLAYER_PLAYER_CPP /* not in the original class: emitted as weak copies if defined in the key function's unit */
    FLOAT getPrizeDraw() const { return m_fPrizeDraw; }
#endif
#ifndef GAME_PLAYER_PLAYER_CPP /* not in the original class: emitted as weak copies if defined in the key function's unit */
    FLOAT getCardDraw() const { return m_fCardDraw; }
#endif
    static BOOL notifyBossDead(INT inStep);   // PARTIAL: added for Game/Enemy/BossDeadDemo
    static HANDLE getLevelUpEffect(); // PARTIAL: added for Game/Gimmick/GimmickTreasureBox
    // PARTIAL: added for Game/Player/FriendBeast (also out of line: setPlayerFlag__7CPlayerUiUi)
#ifdef GAME_PLAYER_PLAYER_CPP
    void setPlayerFlag(UINT inSet, UINT inMask); // the weak copies are emitted in the original order: defined after the class
#else
    void setPlayerFlag(UINT inSet, UINT inMask) { UINT& f = m_uiPlayerFlag; f = (f | inSet) & ~inMask; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    void setTrgFlag(UINT inSet, UINT inMask); // the weak copies are emitted in the original order: defined after the class
#else
    void setTrgFlag(UINT inSet, UINT inMask) { UINT& f = m_uiTrgFlag; f = (f | inSet) & ~inMask; } // PARTIAL: added for Form/Menu/WorldSelect
#endif
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
    // PARTIAL: added for Game/Player/PlayerRiku (CPlayer's virtuals vtable[37..62])
    virtual void calcAppearPosFS_toAttackID();
    virtual void setAttack();
    virtual INT startStockSkillAction();
    virtual void initStockSkill(INT inNo);
    virtual void exeStockSkill();
    virtual void initMagic(INT inFlag);
    virtual void exeMagic();
    virtual void setMagic(INT inState, INT inCardID, INT inSkillID);
    virtual void initFriend(INT inNo);
    virtual void exeFriend();
    virtual void initSummon(INT inNo);
    virtual void exeSummon();
    virtual void exeStandUp_inFriend();
    virtual INT exeAttack_inFriend();
    virtual INT endAttack_inFriend();
    virtual void initTransform(INT inFlag);
    virtual void exeTransform();
    virtual void initGlide(INT inFlag);
    virtual void exeGlide();
    virtual void initJump2nd();
    virtual void exeJump2nd();
    virtual void exeAvoid();
    virtual void initDuel(INT inFlag);
    virtual void exeDuel();
    virtual void initPronounce(INT inFlag);
    virtual void exePronounce();
    // PARTIAL: added for Game/Player/PlayerRiku (overrides, for the vtables)
    virtual ~CPlayer();
    virtual void stateInitial();
    virtual void onMsg(INT inMsg, void* inExtraInfo);
    virtual void getLockOnPos(FVECTOR& outPos);
    virtual void getHeadPos(FVECTOR& outPos) const;
    virtual void getUseCardPos(FVECTOR& outPos);
    virtual void getCameraTargetPos(FVECTOR& outPos);
    virtual void updateBefore();
    virtual void updateAfter();
    virtual void lastUpdate();
    // PARTIAL: added for Game/Player/PlayerRiku
#ifdef GAME_PLAYER_PLAYER_CPP
    void setActionFlag(UINT inSet, UINT inMask); // the weak copies are emitted in the original order: defined after the class
#else
    void setActionFlag(UINT inSet, UINT inMask) { UINT& f = m_uiActionFlag; f = (f | inSet) & ~inMask; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    void setBattleFlag(UINT inSet, UINT inMask); // the weak copies are emitted in the original order: defined after the class
#else
    void setBattleFlag(UINT inSet, UINT inMask) { UINT& f = m_uiBattleFlag; f = (f | inSet) & ~inMask; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    INT isPlayerFlag(UINT inFlag) const; // the weak copies are emitted in the original order: defined after the class
#else
    INT isPlayerFlag(UINT inFlag) { return m_uiPlayerFlag & inFlag; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    INT isActionFlag(UINT inFlag) const; // the weak copies are emitted in the original order: defined after the class
#else
    INT isActionFlag(UINT inFlag) { return m_uiActionFlag & inFlag; }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    INT isTrgFlag(UINT inFlag) const; // the weak copies are emitted in the original order: defined after the class
#else
    INT isTrgFlag(UINT inFlag) { return m_uiTrgFlag & inFlag; }
#endif
    static CPlayer* m_pInstance;
#ifndef GAME_PLAYER_PLAYER_CPP /* not in the original class: emitted as weak copies if defined in the key function's unit */
    static CPlayer*& _instance() { return m_pInstance; } // PARTIAL: reference accessor (as CStatus::_info): the original reloads m_pInstance after every store
#endif
    static void enablePadControl(INT inFlag);
    static void enableDbgPadControl(INT inFlag);
    void changeState(INT inState);
    void initActionSkill();
    void setPlayerState(PlayerState inState);
    void setAnimForce(INT inAnimID, TIMEQ inChangeTime, BOOL inRootMove, INT inEffSeq, INT inSESeq, TIMEQ inStart);
    void setAnim(INT inID, INT inTq, INT inGr);
    void setAnimSkillEx(INT inAnimID, TIMEQ inChangeTime, BOOL inRootMove, INT inEffSeq, INT inSESeq);
    void setAnimRsrcEx(INT inRsrc, INT inID, INT inP2, INT inP3, INT inP4, INT inP5, INT inP6);
    void setFaceAnim(INT inID, INT inFlag);
    void initStartWait();
    void initStandUp(INT inFlag);
    void exeEnd(); void exeRoomCreation(); void exeWorldSelect(); void exeDead(); void exeItem(); void exeStandUp(); void exeGimmickCard(); void exeCarry(); void initFall(); void exeHang(); void exeWall(); INT chkWall(INT inFlag); void exeJump(); BOOL chkHang(); void setControl(); void exeAttack(); void exeFooting(); void exeJumpLand(); void initDead(); void initWall(); INT chkWallBottom(); void initHang(); void initEscape(); void initAttackField(INT inNo); void initAttack(INT inNo); void initAvoid(); void initFooting(INT inFlag); void initCharge(); void exeCharge(); void initGimmickCard(INT inNo); void initItem(INT inNo); void initJump(INT inFlag); void initFlip(INT inFlag); void setFlipForce(const FVECTOR* inPos);
    INT initIdle(INT inAnim, INT inFlag);
    INT procFlowIdle(INT inFlag);
    void initEnd();
    void initBadCondition();
    void setConfuse(INT inFlag, INT inTime);
    INT setMDFooting(HANDLE inFooting);
    void fadeEffAttack(INT inFlag);
    void fadeEffect(INT& ioHandle, INT inTime);
    void procSkillEnd(INT inFlag);
    void onAttack();
    void offAttack();
    void setBGFade(FLOAT inRate, FLOAT inTime, INT inFlag);
    void setBGFade(FVECTOR& inColor, FLOAT inTime, INT inFlag);
    void setObjStop(UINT inFlag);
    void setEnemyStop(HANDLE inEnemy, UINT inSet);
    void setQuake(FLOAT inCycle, FLOAT inPower, UINT inTime);
    void setLand(FLOAT inP0, FLOAT inP1, INT inFlag); void setDownForce();
    void updateSpotLight(HANDLE inHandle);
    void resetAction(INT inFlag);
    void procAttackMove(INT inFlag, FLOAT inSpeed, FLOAT inRate, INT inP3, FLOAT inP4);
    INT procAttackCollision(TIMEQ inNowFrame);
    void procTargetMark(); void exeAttackField(); void chkPlayCard(); INT chkPlayCardCombo(); void chkLockon();
    void procGravity(TIMEQ inNowFrame, TIMEQ inEndFrame, INT inFlag);
    void calcEnchant_AttackPoint(INT& ioPower, ATK_KIND& ioKind, ATK_ATTR inAttr, UINT inFlag);
    void setUseRsrcBlock(INT inBlock);
    INT exeUseStock(INT inFlag); INT startSingleAction();
    INT chkChargeMove(INT inFlag);
    void setPushon(INT inFlag);
    void setCastEffect(const CHAR* inName, INT inNo, CCoord* inCoord, FVECTOR* inPos); void setEffAttack(INT inNum, const CHAR* inName, INT inNo, INT inBone, FVECTOR* inOfs);
    // PARTIAL: added for Game/Player/PlayerDRiku
    static INT importStatus();
    static INT exportStatus();
    static void addMP(INT inMP); static void addEXP(INT inEXP, INT inMode); void setTrapDamage(INT inPower, const CHAR* lpszInName, UINT nNo);
    static void notifyExterminate();
    void setDefAttr(FLOAT* inAttr);
    // PARTIAL: added for Game/Player/Player
    static INT calcNextEXP(INT inLv) __attribute__((const));
    static INT getNowNextEXP();
    BOOL isMuteki() const;
    void killBullet();
    void exeConfuse();
    static BOOL isNowExterminate();
    static BOOL isEnableBattleEnd();
    BOOL chkExtraSkill();
    void beginStateBattle_();
    void stateBattle_();
    void endStateBattle_();
    void beginStateField_();
    void stateField_();
    void endStateField_();
    void beginStateHide_();
    void stateHide_();
    void endStateHide_();
    void beginStateWait_();
    void stateWait_();
    void endStateWait_();
    void exeAttackTimer();
    void exeMutekiTimer();
    FLOAT getDefAttr(ATK_ATTR inAttr) const;
    BOOL isNowInAir();
    BOOL isAir();
    void addWind(FVECTOR* inVel);
    static INT getNowTotalMP();
    void setTrgFlagRendaNG(const CHAR* inName);
    void cmdMove_WaterSurfaceCheck(INT inFlag);
    void setFallFloat();
    void updateBodyColl(); void updateBodyCollPos(); void chkBodyRevise();
    void updateFooting();
    void makeAnimIndex(INT inID);
    void initStartPos();
    void makeWeapon();
    void makeCockpit();
    void makeRevolver();
    void setAnimSpeed(FLOAT inSpeed);
    INT chkTrgAction(INT inFlag, UINT inMask);
#ifndef GAME_PLAYER_PLAYER_CPP
    HANDLE getLockonHandle() {
        HANDLE handle = m_hLockon;
        if (handle == 0 || handle == -1) handle = m_hTarget;
        return handle;
    }
#endif
#ifdef GAME_PLAYER_PLAYER_CPP
    INT getCurrentState();
    UINT getCharaID();
    PL_PARAM* getParam();
    Gauge::CLockonMarker* getLockonMarker() const;
    Gauge::CPlayerGauge* getGauge() const;
    TIMEQ getStateTQ();
    void clearStateTQ();
    void setSubState(INT inSubState);
    INT getSubState() const;
    void setCP(INT inCP);
    void setMaxCP(INT inMaxCP);
    INT getCP();
    INT getMaxCP();
    INT getAP();
    INT getMP();
    void setDP(INT inDP);
    INT getDP();
    INT getMaxDP();
    INT getNowAnimID();
    INT getAnimID();
    void setStartPos(FLOAT inX, FLOAT inY, FLOAT inZ);
    void setStartDir(const FLOAT& inDir);
    FLOAT getStartDir();
    FLOAT getMovePower();
    FLOAT getWalkSpeed();
    void setDrawRadius(FLOAT inPrize, FLOAT inCard);
    FLOAT getPrizeDrawRadius();
    FLOAT getCardDrawRadius();
    FVECTOR* getLadderPos();
    INT getLadderStep();
    INT getLadderStepMax();
    FLOAT getJumpSpdY();
    FLOAT getJumpTop();
    FLOAT getJumpLandLimit() const;
    FVECTOR& getHangHandPos();
    void setAttackTimer(INT inTime);
    void setMaxCombo(INT inMax);
    void setMutekiTimer(INT inTime);
    BOOL isDead() const;
    void setBodyRviseCoefficient(FLOAT inCoef);
    INT getConfuse() const;
    void setMarkPos(FLOAT inX, FLOAT inY, FLOAT inZ);
    HANDLE isNowTarget();
    HANDLE getOldLockon();
    INT getOldCamMode();
#endif
};

extern CPlayer* g_pPlayer;
FVECTOR getPlayerPos(BOOL flag);

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
extern FLOAT g_fWeight; extern FLOAT g_fGravity; extern FLOAT g_fAntiGravity; extern UINT g_uiStartCondition;
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


#ifdef GAME_PLAYER_PLAYER_CPP
static inline CPlayer*& _instance() { return CPlayer::m_pInstance; }
/* CPlayer's weak inline functions, defined here in the order the original emitted them (the key function beginStateField is in Player.cpp) */
inline INT CPlayer::getCurrentState() { return m_iState; }
inline void CPlayer::stateWait() {  }
inline void CPlayer::endStateWait() {  }
inline UINT CPlayer::getCharaID() { return m_uiCharaID; }
inline PL_PARAM* CPlayer::getParam() { return m_pParam; }
inline PlayerCamera::CPlayerCamera* CPlayer::getPlayerCamera() const { return m_pCam; }
inline Gauge::CLockonMarker* CPlayer::getLockonMarker() const { return m_pMarker; }
inline PlayerWeapon::CPlayerWeapon* CPlayer::getWeapon() const { return m_pWeapon; }
inline CardBattle::CCardRevolver* CPlayer::getRevolver() const { return m_pRevolver; }
inline Gauge::CPlayerGauge* CPlayer::getGauge() const { return m_pGauge; }
inline INT CPlayer::getPlayerState() const { return m_PlayerState; }
inline TIMEQ CPlayer::getStateTQ() { return m_tqStateTimer; }
inline void CPlayer::clearStateTQ() { m_tqStateTimer = 0; }
inline void CPlayer::setSubState(INT inSubState) { m_iSubState = inSubState; }
inline INT CPlayer::getSubState() const { return m_iSubState; }
inline void CPlayer::setPlayerFlag(UINT inSet, UINT inMask) { UINT& f = m_uiPlayerFlag; f = (f | inSet) & ~inMask; }
inline void CPlayer::setActionFlag(UINT inSet, UINT inMask) { UINT& f = m_uiActionFlag; f = (f | inSet) & ~inMask; }
inline void CPlayer::setConditionFlag(UINT inSet, UINT inMask) { UINT& f = m_uiConditionFlag; f = (f | inSet) & ~inMask; }
inline void CPlayer::setTrgFlag(UINT inSet, UINT inMask) { UINT& f = m_uiTrgFlag; f = (f | inSet) & ~inMask; }
inline void CPlayer::setBattleFlag(UINT inSet, UINT inMask) { UINT& f = m_uiBattleFlag; f = (f | inSet) & ~inMask; }
inline INT CPlayer::isPlayerFlag(UINT inFlag) const { return m_uiPlayerFlag & inFlag; }
inline INT CPlayer::isActionFlag(UINT inFlag) const { return m_uiActionFlag & inFlag; }
inline INT CPlayer::isConditionFlag(UINT inFlag) const { return m_uiConditionFlag & inFlag; }
inline INT CPlayer::isTrgFlag(UINT inFlag) const { return m_uiTrgFlag & inFlag; }
inline INT CPlayer::isBattleFlag(UINT inFlag) const { return m_uiBattleFlag & inFlag; }
inline void CPlayer::setHP(INT inHP) { m_iHP = inHP; }
inline void CPlayer::setMaxHP(INT inMaxHP) { m_iMaxHP = inMaxHP; }
inline void CPlayer::setCP(INT inCP) { m_iCP = inCP; }
inline void CPlayer::setMaxCP(INT inMaxCP) { m_iMaxCP = inMaxCP; }
inline INT CPlayer::getHP() { return m_iHP; }
inline INT CPlayer::getMaxHP() { return m_iMaxHP; }
inline INT CPlayer::getCP() { return m_iCP; }
inline INT CPlayer::getMaxCP() { return m_iMaxCP; }
inline void CPlayer::setAP(INT inAP) { m_iAP = inAP; }
inline INT CPlayer::getAP() { return m_iAP; }
inline void CPlayer::setLV(INT inLV) { m_iLV = inLV; }
inline INT CPlayer::getLV() { return m_iLV; }
inline void CPlayer::setEXP(INT inEXP) { m_iEXP = inEXP; }
inline INT CPlayer::getEXP() { return m_iEXP; }
inline void CPlayer::setMP(INT inMP) { m_iMP = inMP; }
inline INT CPlayer::getMP() { return m_iMP; }
inline void CPlayer::setDP(INT inDP) { m_iDP = inDP; }
inline void CPlayer::setMaxDP(INT inDP) { m_iMaxDP = inDP; }
inline INT CPlayer::getDP() { return m_iDP; }
inline INT CPlayer::getMaxDP() { return m_iMaxDP; }
inline INT CPlayer::getCount() const { return m_iCount; }
inline INT CPlayer::getNowAnimID() { return m_iNowAnimID; }
inline INT CPlayer::getAnimID() { return m_pAnim->getAnimID(); }
inline void CPlayer::setStartPos(FLOAT inX, FLOAT inY, FLOAT inZ) { m_vStartPos.set(inX, inY, inZ); }
inline void CPlayer::setStartDir(const FLOAT& inDir) { m_fStartDir = inDir; }
inline void CPlayer::setDir(const FLOAT& inDir) { m_fDir = inDir; }
inline FVECTOR& CPlayer::getStartPos() { return m_vStartPos; }
inline FLOAT CPlayer::getStartDir() { return m_fStartDir; }
inline FLOAT CPlayer::getDir() { return m_fDir; }
inline FVECTOR& CPlayer::getCenterPos() { return m_vCenterPos; }
inline FLOAT CPlayer::getMovePower() { return m_fMovePower; }
inline FLOAT CPlayer::getWalkSpeed() { return m_fWalkSpeed; }
inline void CPlayer::getLockOnPos(FVECTOR& outPos) { outPos = m_vCenterPos; }
inline void CPlayer::getHeadPos(FVECTOR& outPos) const { outPos = m_vHeadPos; }
inline void CPlayer::getUseCardPos(FVECTOR& outPos) { outPos = m_vCenterPos; }
inline FVECTOR& CPlayer::getAppearPos() { return m_vAppearPos; }
inline void CPlayer::setDrawRadius(FLOAT inPrize, FLOAT inCard) { m_fPrizeDraw = inPrize; m_fCardDraw = inCard; }
inline FLOAT CPlayer::getPrizeDrawRadius() { return m_fPrizeDraw; }
inline FLOAT CPlayer::getCardDrawRadius() { return m_fCardDraw; }
inline FVECTOR* CPlayer::getLadderPos() { return m_pLadderPos; }
inline INT CPlayer::getLadderStep() { return m_iLadderStep; }
inline INT CPlayer::getLadderStepMax() { return m_iLadderStepMax; }
inline FLOAT CPlayer::getJumpSpdY() { return m_fJumpSpdY; }
inline FLOAT CPlayer::getJumpTop() { return m_fJumpTop; }
inline FVECTOR& CPlayer::getJumpStartPos() { return m_vJumpStartPos; }
inline FLOAT CPlayer::getJumpLandLimit() const { return m_fJumpLandLimit; }
inline FVECTOR& CPlayer::getHangHandPos() { return m_vHangHandPos; }
inline void CPlayer::setAttackTimer(INT inTime) { m_tqAttackTimer = inTime; }
inline void CPlayer::setMaxCombo(INT inMax) { m_iMaxCombo = inMax; }
inline UINT CPlayer::getAttackID() { return m_uiAttackID; }
inline void CPlayer::setMutekiTimer(INT inTime) { m_tqMutekiTimer = inTime; }
inline BOOL CPlayer::isDead() const { return m_bIsDead; }
inline void CPlayer::setBodyRviseCoefficient(FLOAT inCoef) { m_fBodyRviseCoefficient = inCoef; }
inline INT CPlayer::getConfuse() const { return m_tqConfuse; }
inline void CPlayer::setMarkPos(FLOAT inX, FLOAT inY, FLOAT inZ) { m_vMarkPos.set(inX, inY, inZ); }
inline FVECTOR& CPlayer::getMarkPos() { return m_vMarkPos; }
inline HANDLE CPlayer::isNowLockon() { return m_hLockon; }
inline FVECTOR& CPlayer::getLockonPos() { return m_vLockonPos; }
inline HANDLE CPlayer::isNowMark() { return m_hMark; }
inline HANDLE CPlayer::isNowTarget() { return m_hTarget; }
inline HANDLE CPlayer::getOldLockon() { return m_hOldLockon; }
inline INT CPlayer::getOldCamMode() { return m_iOldCamMode; }
inline void CPlayer::setStateSE(INT inState, INT inKind) {  }
inline INT CPlayer::getStateSESeq(INT inState) { return 0; }
inline void CPlayer::setAttack_inFriend(INT inID, INT inStock, HANDLE inTarget) {  }
inline INT CPlayer::startStockSkillAction() { return 0; }
inline void CPlayer::initStockSkill(INT inNo) {  }
inline void CPlayer::exeStockSkill() {  }
inline void CPlayer::initMagic(INT inFlag) {  }
inline void CPlayer::exeMagic() {  }
inline void CPlayer::setMagic(INT inState, INT inCardID, INT inSkillID) {  }
inline void CPlayer::initFriend(INT inNo) {  }
inline void CPlayer::exeFriend() {  }
inline void CPlayer::initSummon(INT inNo) {  }
inline void CPlayer::exeSummon() {  }
inline INT CPlayer::exeAttack_inFriend() { return -1; }
inline INT CPlayer::endAttack_inFriend() { return -1; }
inline void CPlayer::initTransform(INT inFlag) {  }
inline void CPlayer::exeTransform() {  }
inline void CPlayer::initGlide(INT inFlag) {  }
inline void CPlayer::exeGlide() {  }
inline void CPlayer::initJump2nd() {  }
inline void CPlayer::exeJump2nd() {  }
inline void CPlayer::exeAvoid() {  }
inline void CPlayer::initPronounce(INT inFlag) {  }
inline void CPlayer::exePronounce() {  }
#endif

#endif /* GAME_PLAYER_PLAYER_H */
