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
    UBYTE _m_0x21c[0x80];        /* 0x21c CStateTemplate<CPlayer> m_State[4] */
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
    UBYTE m_collBody[0x110] __attribute__((aligned(16))); /* 0x570 CCollBody */
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
    // PARTIAL: added for Game/Player/PlayerCamera
    HANDLE isNowLockon() { return m_hLockon; }
    FVECTOR& getLockonPos() { return m_vLockonPos; }
    FVECTOR& getStartPos() { return m_vStartPos; }
    BOOL isNowIdleAnim();
    // PARTIAL: added for Game/Gimmick/GimmickJumpStand
    INT getJumpState() const;
    static void notifyGimmickJump(HANDLE inHandle, INT inPower);
    static void notifyGetPrize(INT inKind, INT inNumber, INT inState);
    // PARTIAL: added for Game/Prize (getter names unknown)
    void notifyReturnCard(const CARD* inCard);
    FLOAT getPrizeDraw() const { return m_fPrizeDraw; }
    FLOAT getCardDraw() const { return m_fCardDraw; }
    static BOOL notifyBossDead(INT inStep);   // PARTIAL: added for Game/Enemy/BossDeadDemo
    static HANDLE getLevelUpEffect(); // PARTIAL: added for Game/Gimmick/GimmickTreasureBox
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
    void setActionFlag(UINT inSet, UINT inMask) { UINT& f = m_uiActionFlag; f = (f | inSet) & ~inMask; }
    void setBattleFlag(UINT inSet, UINT inMask) { UINT& f = m_uiBattleFlag; f = (f | inSet) & ~inMask; }
    INT isPlayerFlag(UINT inFlag) { return m_uiPlayerFlag & inFlag; }
    INT isActionFlag(UINT inFlag) { return m_uiActionFlag & inFlag; }
    INT isTrgFlag(UINT inFlag) { return m_uiTrgFlag & inFlag; }
    static CPlayer* m_pInstance;
    void changeState(INT inState);
    void initActionSkill();
    void setPlayerState(PlayerState inState);
    void setAnimForce(INT inID, INT inTq, INT inGr, INT inP3, INT inP4, INT inP5);
    void setAnim(INT inID, INT inTq, INT inGr);
    void setAnimSkillEx(INT inID, INT inP1, INT inP2, INT inP3, INT inP4);
    void setAnimRsrcEx(INT inRsrc, INT inID, INT inP2, INT inP3, INT inP4, INT inP5, INT inP6);
    void setFaceAnim(INT inID, INT inFlag);
    void initStartWait();
    void initStandUp(INT inFlag);
    INT initIdle(INT inAnim, INT inFlag);
    INT procFlowIdle(INT inFlag);
    void initEnd();
    void initBadCondition();
    void setConfuse(INT inFlag, INT inTime);
    INT setMDFooting(INT inFlag);
    void fadeEffAttack(INT inFlag);
    void fadeEffect(INT& ioHandle, INT inTime);
    void procSkillEnd(INT inFlag);
    void onAttack();
    void offAttack();
    BOOL isAir();
    void setBGFade(FLOAT inRate, FLOAT inTime, INT inFlag);
    void setBGFade(FVECTOR& inColor, FLOAT inTime, INT inFlag);
    void setObjStop(UINT inFlag);
    void setEnemyStop(INT inFlag, UINT inMask);
    void setQuake(FLOAT inPow, FLOAT inTime, UINT inFlag);
    void setLand(FLOAT inP0, FLOAT inP1, INT inFlag);
    void updateSpotLight(INT inFlag);
    void resetAction(INT inFlag);
    void procAttackMove(INT inFlag, FLOAT inSpeed, FLOAT inRate, INT inP3, FLOAT inP4);
    INT procAttackCollision(INT inFlag);
    void procTargetMark();
    void procGravity(INT inP0, INT inP1, INT inP2);
    void calcEnchant_AttackPoint(INT& ioPower, ATK_KIND& ioKind, ATK_ATTR inAttr, UINT inFlag);
    void setUseRsrcBlock(INT inBlock);
    INT exeUseStock(INT inFlag);
    INT chkChargeMove(INT inFlag);
    void setPushon(INT inFlag);
    void setCastEffect(const CHAR* inName, INT inNo, CCoord* inCoord, FVECTOR* inPos);
    // PARTIAL: added for Game/Player/PlayerDRiku
    static INT importStatus();
    void setDefAttr(FLOAT* inAttr);
    void makeAnimIndex(INT inID);
    void initStartPos();
    void makeWeapon();
    void makeCockpit();
    void makeRevolver();
    void setAnimSpeed(FLOAT inSpeed);
    INT chkTrgAction(INT inFlag, UINT inMask);
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
