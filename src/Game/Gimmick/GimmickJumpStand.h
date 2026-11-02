#ifndef GAME_GIMMICK_GIMMICKJUMPSTAND_H
#define GAME_GIMMICK_GIMMICKJUMPSTAND_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickJumpStand : public CBaseGimmick {
private:
    FLOAT m_WaitCounter; /* 0x220 */
    SHORT m_ID;          /* 0x224 */
    CHAR m_Step;         /* 0x226 */
    BYTE m_ColiFlag;     /* 0x227 */
    FLOAT m_Wait;        /* 0x228 */

public:
    CGimmickJumpStand(CTask* inParent, INT in_GimID);
    virtual ~CGimmickJumpStand();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual INT getTypeID() const;
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    void main_Tr();
    void main_Ol();
    void main_Tw();
    INT getJumpStandPower() { return getParam(); }
};

#endif /* GAME_GIMMICK_GIMMICKJUMPSTAND_H */
