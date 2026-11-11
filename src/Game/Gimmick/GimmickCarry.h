#ifndef GAME_GIMMICK_GIMMICKCARRY_H
#define GAME_GIMMICK_GIMMICKCARRY_H

#include "khtypes.h"
#include "System/Vector.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickCarry : public CBaseGimmick {
protected:
    CHAR m_Stat;          /* 0x220 */
    CHAR m_AlphaStep;     /* 0x221 */
    CHAR m_FlagSetInfo;   /* 0x222 */
    BYTE m_StartCal;      /* 0x223 */
    BYTE m_HitObj;        /* 0x224 */
    FLOAT m_PowY;         /* 0x228 */
    FLOAT m_Wait;         /* 0x22c */
    FVECTOR m_throwDir;   /* 0x230 */
    FVECTOR m_targetPos;  /* 0x240 */
    FVECTOR m_rotQtn;     /* 0x250 QUATERNION */

public:
    CGimmickCarry(CTask* inParent);

private:
    void CarryliftStart();
    void CarryWalkStart();
    void CarryStandStart();
    void CarryThrowStart();

public:
    virtual ~CGimmickCarry();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual INT getTypeID() const;
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    BOOL moveThrow();
    virtual BOOL checkCarryPermission();
    void lift();
    void CarryWalk();
    void CarryStand();
    void CarryThrow();
    void ThrowGimmick(const FVECTOR& in_TargetPos);
    void CancelCarry();
    virtual void lastUpdate();
    void setFlagSetInfo(INT in_flag) { m_FlagSetInfo = in_flag; }
};

#endif /* GAME_GIMMICK_GIMMICKCARRY_H */
