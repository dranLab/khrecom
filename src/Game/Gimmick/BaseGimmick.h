// PARTIAL: added for Game/Gimmick/GimmickHitMore
#ifndef GAME_GIMMICK_BASEGIMMICK_H
#define GAME_GIMMICK_BASEGIMMICK_H

#include "khtypes.h"
#include "System/Graphic/Anim.h"
#include "Game/Entity.h"

struct ZONEPARTS;

// PARTIAL: added for Game/Gimmick/GimmickGuillotine
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

class CBaseGimmick : public CEntity {
private:
    UINT m_uiGimmickFlag;       /* 0x1d0 */
    INT m_iParam;               /* 0x1d4 */
    INT m_iUniqueID;            /* 0x1d8 */
    HANDLE m_hEffectHandle;     /* 0x1dc */
    UBYTE m_GimmickState[0x40]; /* 0x1e0 CStateTemplate<CBaseGimmick>[2] */

public:
    CBaseGimmick(CTask* inParent);
    CBaseGimmick(CTask* inParent, const ZONEPARTS& inParts); // PARTIAL: added for Game/Gimmick/SampleGimmick
    virtual ~CBaseGimmick();
    virtual INT getTypeID() const;
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual BOOL initalize(INT inGroupID, INT inRsrcID, UINT iFlag);
    INT getParam() { return m_iParam; } // PARTIAL: added for Game/Gimmick/SampleGimmick
    UINT getGimmickFlag() { return m_uiGimmickFlag; } // PARTIAL: added for Game/Gimmick/GimmickHitRepetition
    INT getUniqueID() { return m_iUniqueID; }
    // PARTIAL: added for Game/Gimmick/GimmickBreakable
    void setGimmickFlag(UINT inFlag, BOOL inOn) {
        if (inOn) m_uiGimmickFlag |= inFlag;
        else m_uiGimmickFlag &= ~inFlag;
    }
    virtual void changeMotion(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo);

protected:
    virtual void updateBefore();
    virtual void updateAfter();
    virtual void lastUpdate();
    void changeGimmickState(INT inState); // PARTIAL: added for Game/Gimmick/GimmickBreakable
    INT createPrize(INT inType, INT inParam); // PARTIAL: added for Game/Gimmick/GimmickBreakable
    INT saveStat(INT in_Stat);
    GimParam* getGimParam(); // PARTIAL: added for Game/Gimmick/GimmickGuillotine
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_BASEGIMMICK_H */
