#ifndef GAME_GIMMICK_BASEGIMMICK_H
#define GAME_GIMMICK_BASEGIMMICK_H

#include "khtypes.h"
#include "System/Graphic/Anim.h"
#include "Game/Entity.h"
#include "System/Fsm.h"

struct ZONEPARTS;

struct GimParam {
    SHORT GimID;      /* 0x0 */
    BYTE AtkPow;      /* 0x2 */
    BYTE AtkAttr;     /* 0x3 */
    CHAR Hp;          /* 0x4 */
    BYTE LunchType;   /* 0x5 */
    BYTE B_Prob;      /* 0x6 */
    BYTE G_Index;     /* 0x7 */
    BYTE R_Index;     /* 0x8 */
    BYTE G_Prob;      /* 0x9 */
    BYTE R_Prob;      /* 0xa */
    BYTE C_Prob;      /* 0xb */
    BYTE Riku_GIndex; /* 0xc */
    BYTE Riku_GProb;  /* 0xd */
};

enum {
    GM_STATE_MAIN = 0,
    GM_STATE_HIT_ATTACK = 1,
    GM_STATE_MAX = 2
};

class CBaseGimmick : public CEntity {
private:
    UINT m_uiGimmickFlag;                          /* 0x1d0 */
    INT m_iParam;                                  /* 0x1d4 */
    INT m_iUniqueID;                               /* 0x1d8 */
    HANDLE m_hEffectHandle;                        /* 0x1dc */
    CStateTemplate<CBaseGimmick> m_GimmickState[2]; /* 0x1e0 */

public:
    CBaseGimmick(CTask* inParent);
    CBaseGimmick(CTask* inParent, const ZONEPARTS& inParts);
    virtual ~CBaseGimmick();
    virtual INT getTypeID() const;
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual BOOL initalize(INT inGroupID, INT inRsrcID, UINT inFlag);
    void setGimmickFlag(UINT inSet, UINT inMask) { m_uiGimmickFlag = (m_uiGimmickFlag | inSet) & ~inMask; }
#ifndef GAME_GIMMICK_BASEGIMMICK_CPP
    /* Not in the original: older Gimmick units call setGimmickFlag(flag, TRUE/FALSE); it would be emitted as an
     * extra weak function in Game/Gimmick/BaseGimmick, so that unit leaves it out. */
    void setGimmickFlag(UINT inFlag, BOOL inOn) {
        if (inOn) m_uiGimmickFlag |= inFlag;
        else m_uiGimmickFlag &= ~inFlag;
    }
#endif
    void setParam(INT inParam) { m_iParam = inParam; }
    void setUniqueID(INT inID) { m_iUniqueID = inID; }
    UINT getGimmickFlag() const { return m_uiGimmickFlag; }
    INT getParam() const { return m_iParam; }
    INT getUniqueID() const { return m_iUniqueID; }
    virtual void changeMotion(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo);
    BOOL changeMotionIndex(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo);
    static INT effectIDtoI(UINT inEffID);

protected:
    virtual void updateBefore();
    virtual void updateAfter();
    virtual void lastUpdate();
    void changeGimmickState(INT inState);
    void makeCoord(const ZONEPARTS& inParts);
    BOOL initGimmickParam(INT inGroupID, INT inRsrcID);
    GimParam* getGimParam();
    INT createPrize(INT in_Flag, INT in_NotDropCardFlag);
    INT saveStat(INT in_Stat);
    virtual void beginStateMain() {}
    virtual void stateMain() {}
    virtual void endStateMain() {}
    virtual void beginStateHitAttack() {}
    virtual void stateHitAttack() { changeGimmickState(GM_STATE_MAIN); }
    virtual void endStateHitAttack() {}

private:
    void initFSM_();
    BOOL hitTest_();
};

#endif /* GAME_GIMMICK_BASEGIMMICK_H */
