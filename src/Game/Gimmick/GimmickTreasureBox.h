#ifndef GAME_GIMMICK_GIMMICKTREASUREBOX_H
#define GAME_GIMMICK_GIMMICKTREASUREBOX_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CPathCamera;

class CGimmickTreasureBox : public CBaseGimmick {
private:
    CHAR m_Step;                /* 0x220 */
    CHAR m_Stat;                /* 0x221 */
    BYTE m_LocalStep;           /* 0x222 */
    FLOAT m_WaitCounter;        /* 0x224 */
    CPathCamera* m_pPathCamera; /* 0x228 */

public:
    CGimmickTreasureBox(CTask* inParent);
    virtual ~CGimmickTreasureBox();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual INT getTypeID() const;
    void setState(INT in_State);
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    void NormalMain();
    void EnemyMain();
    BOOL CardMaxWarningMain();
    void EnemyPop();
    BOOL checkOpenTresureBoxPermission();
    void setLock(BOOL in_Disp);
    void OpenTreasureBox();
    BOOL isOpenTresureBox() { return m_Stat & 1; }
    INT isTresureBoxState() { return m_Step; }
};

class CGimmickTreasureBoxForEffectID : public CBaseGimmick {
public:
    CGimmickTreasureBoxForEffectID(CTask* inParent, const ZONEPARTS& inParts);
    virtual ~CGimmickTreasureBoxForEffectID();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_GIMMICKTREASUREBOX_H */
