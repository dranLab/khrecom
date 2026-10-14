// PARTIAL: added for Game/Player/SummonDumbo
#ifndef GAME_PLAYER_SUMMON_H
#define GAME_PLAYER_SUMMON_H

#include "khtypes.h"
#include "System/Graphic/Anim.h"
#include "System/Vector.h"
#include "Game/Entity.h"
#include "System/PadHandler.h"
#include "Game/CollParam.h"
#include "Game/AttackMotion.h"

class CPlayer;
struct PATK_DATA;

namespace Summon {

class CSummon : public CEntity, public CPadHandler {
protected:
    UINT m_uiCharaID;           /* 0x1e0 */
    UINT m_uiRCID;              /* 0x1e4 */
    USHORT m_uiAnimID;          /* 0x1e8 */
    USHORT m_uiStartAnimID;     /* 0x1ea */
    USHORT m_uiSkillID;         /* 0x1ec */
    USHORT m_uiLevel;           /* 0x1ee */
    USHORT m_uiKind;            /* 0x1f0 */
    USHORT m_uiUpKind;          /* 0x1f2 */
    FLOAT m_fScale;             /* 0x1f4 */
    CPlayer* m_pPlayer;         /* 0x1f8 */
    FLOAT m_fDir;               /* 0x1fc */
    FLOAT m_fMoveSpdXZ;         /* 0x200 */
    FLOAT m_fCamHeight;         /* 0x204 */
    PATK_DATA* m_pAtkData;      /* 0x208 */
    INT m_iAttackNo;            /* 0x20c */
    UINT m_uiAttackID;          /* 0x210 */
    INT m_iAttackPower;         /* 0x214 */
    ATK_ATTR m_AtkAttr;         /* 0x218 */
    ATK_KIND m_AtkKind;         /* 0x21c */
    TIMEQ m_tqTrgFrame;         /* 0x220 */
    HANDLE m_hTarget;           /* 0x224 */
    HANDLE m_hLockon;           /* 0x228 */
    FVECTOR m_vAtkOffset;       /* 0x230 */
    INT m_iAtkBone;             /* 0x240 */
    FVECTOR m_vAimPos;          /* 0x250 */
    FLOAT m_fRotDir;            /* 0x260 */
    FLOAT m_fDist;              /* 0x264 */
    TIMEQ m_tqSubTimer;         /* 0x268 */
    FLOAT m_fBodyRviseCoefficient; /* 0x26c */
    INT m_iSubState;            /* 0x270 */
    UINT m_uiFlag;              /* 0x274 */
public:
    UBYTE m_State[6][0x20];     /* 0x278 CStateTemplate<Summon::CSummon>[6] */
    INT m_iState;               /* 0x338 */

public:
    CSummon(CTask* inParent, INT inID);
    virtual ~CSummon();
    void changeState(INT inState);
    void changeMotion(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo, INT inActNo);
protected:
    virtual void getCameraTargetPos(FVECTOR& outPos);
    virtual void updateBefore();
    virtual void updateAfter();
    virtual void lastUpdate();
    virtual void beginStateAppear();
    virtual void stateAppear();
    virtual void endStateAppear();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAction1();
    virtual void stateAction1();
    virtual void endStateAction1();
    virtual void beginStateAction2();
    virtual void stateAction2();
    virtual void endStateAction2();
    virtual void beginStateAction3();
    virtual void stateAction3();
    virtual void endStateAction3();
    virtual void beginStateExit();
    virtual void stateExit();
    virtual void endStateExit();
    void homingPlayerTargetY(HANDLE inTarget, FLOAT inRate);
    void homingPlayerTargetXZ(HANDLE inTarget, FLOAT inRate); // PARTIAL: added for Game/Player/SummonSinba
    void turnPlayerTargetY(HANDLE inTarget, FLOAT inRate);    // PARTIAL: added for Game/Player/SummonSinba
    void changeCamera(HANDLE inTarget, INT inMode, INT inReset); // PARTIAL: added for Game/Player/SummonSinba
    INT getPower(INT inSkillID, INT inKind);
    // PARTIAL: added for Game/Player/SummonMushu
    void getWeaponTopPos(FVECTOR& inPos);
    void setLockon(HANDLE inHandle);
    void setPushon(INT inFlag);
    void killChild(FLOAT inFadeTime); // PARTIAL: added for Game/Player/SummonTink
public:
    void setExit(INT inFlag);
    virtual INT setSkill(INT inID);
};

} // namespace Summon

#endif /* GAME_PLAYER_SUMMON_H */
