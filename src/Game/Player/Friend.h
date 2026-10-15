#ifndef GAME_PLAYER_FRIEND_H
#define GAME_PLAYER_FRIEND_H

#include "khtypes.h"
#include "System/Graphic/Anim.h"
#include "System/Vector.h"
#include "Game/Entity.h"
#include "System/PadHandler.h"
#include "System/Fsm.h"

class CPlayer;
struct PATK_DATA;

namespace Friend {

/* Flag updates fold ~inMask at the RTL level (li -0x4001, not lui/ori 0xffffbfff): an inline function. */
static inline void _setFlag(UINT& ioFlag, UINT inSet, UINT inMask) {
    UINT flag = ioFlag;
    ioFlag = (flag | inSet) & ~inMask;
}

class CFriend : public CEntity, public CPadHandler {
public:
    enum {
        STATE_APPEAR = 0,
        STATE_MAIN = 1,
        STATE_ACTION1 = 2,
        STATE_ACTION2 = 3,
        STATE_ACTION3 = 4,
        STATE_EXIT = 5,
        STATE_MAX = 6
    };

protected:
    UINT m_uiCharaID;           /* 0x1e0 */
    UINT m_uiRCID;              /* 0x1e4 */
    USHORT m_uiAnimID;          /* 0x1e8 */
    USHORT m_uiStartAnimID;     /* 0x1ea */
    USHORT m_uiGroundAnimID;    /* 0x1ec */
    USHORT m_uiSkillID;         /* 0x1ee */
    USHORT m_uiLevel;           /* 0x1f0 */
    USHORT m_uiKind;            /* 0x1f2 */
    FLOAT m_fScale;             /* 0x1f4 */
    CPlayer* m_pPlayer;         /* 0x1f8 */
    FLOAT m_fDir;               /* 0x1fc */
    FLOAT m_fMoveSpdXZ;         /* 0x200 */
    FLOAT m_fCamHeight;         /* 0x204 */
    PATK_DATA* m_pAtkData;      /* 0x208 */
    INT m_iAttackNo;            /* 0x20c */
    UINT m_uiAttackID;          /* 0x210 */
    INT m_iAttackPower;         /* 0x214 */
    INT m_AtkAttr;              /* 0x218 ATK_ATTR */
    TIMEQ m_tqTrgFrame;         /* 0x21c */
    HANDLE m_hTarget;           /* 0x220 */
    HANDLE m_hLockon;           /* 0x224 */
    FVECTOR m_vAtkOffset;       /* 0x230 */
    INT m_iAtkBone;             /* 0x240 */
    FVECTOR m_vAimPos;          /* 0x250 */
    FLOAT m_fRotDir;            /* 0x260 */
    FLOAT m_fDist;              /* 0x264 */
    TIMEQ m_tqSubTimer;         /* 0x268 */
    UINT m_uiCollAttackID;      /* 0x26c */
    FLOAT m_fBodyRviseCoefficient; /* 0x270 */
    INT m_iSubState;            /* 0x274 */
    UINT m_uiFlag;              /* 0x278 */
public:
    CStateTemplate<CFriend> m_State[STATE_MAX]; /* 0x27c */
    INT m_iState;               /* 0x33c */
private:
    static void (CFriend::*m_pBeginStateTbl[STATE_MAX])();
    static void (CFriend::*m_pStateTbl[STATE_MAX])();
    static void (CFriend::*m_pEndStateTbl[STATE_MAX])();

public:
    CFriend(CTask* inParent, INT inID, INT inEx);
    virtual ~CFriend();
    BOOL initalize();
    void changeState(INT inState);
    INT getCurrentState() { return m_iState; }
    void changeMotion(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo, INT inActNo);
protected:
    virtual void updateBefore();
    virtual void updateAfter();
    virtual void lastUpdate();
    virtual void beginStateAppear();
    virtual void stateAppear();
    virtual void endStateAppear();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAction1() {}
    virtual void stateAction1() {}
    virtual void endStateAction1() {}
    virtual void beginStateAction2() {}
    virtual void stateAction2() {}
    virtual void endStateAction2() {}
    virtual void beginStateAction3() {}
    virtual void stateAction3() {}
    virtual void endStateAction3() {}
    virtual void beginStateExit();
    virtual void stateExit();
    virtual void endStateExit();
    virtual void getCameraTargetPos(FVECTOR& outPos);
    void homingPlayerTargetY(HANDLE inTarget, FLOAT inRate);
    void setMagic(INT inID);
    INT getPower(INT inSkillID, INT inKind);
    void getWeaponTopPos(FVECTOR& inPos);
    void calcCirclePos(FVECTOR& outPos, FLOAT inRadius, FLOAT inDir);
    void changeCamera(HANDLE inTarget, INT inMode, INT inReset);
    void setLockon(HANDLE inHandle);
    void setPushon(INT inFlag);
    INT entryChainTrg(INT inMode);
    INT checkChainTrg();
    INT isSucceedChain();
    void resetLockon();
    void killChild(FLOAT inFadeTime);
private:
    void beginStateAppear_();
    void stateAppear_();
    void endStateAppear_();
    void beginStateMain_();
    void stateMain_();
    void endStateMain_();
    void beginStateAction1_();
    void stateAction1_();
    void endStateAction1_();
    void beginStateAction2_();
    void stateAction2_();
    void endStateAction2_();
    void beginStateAction3_();
    void stateAction3_();
    void endStateAction3_();
    void beginStateExit_();
    void stateExit_();
    void endStateExit_();
    void initFSM_();
public:
    UINT getRCID() const { return m_uiRCID; }
    void setAppear();
    void setExit(INT inFlag);
    virtual INT setSkill(INT inID);
    CEntity* getTarget();
    INT isCombo() { return m_uiFlag & 0x1000; }
    void setSuccess() { _setFlag(m_uiFlag, 0x4000, 0x8000); }
    void setFailed() { _setFlag(m_uiFlag, 0x8000, 0x4000); }
    void setBodyRviseCoefficient(FLOAT inValue) { m_fBodyRviseCoefficient = inValue; }
};

} // namespace Friend

#endif /* GAME_PLAYER_FRIEND_H */
