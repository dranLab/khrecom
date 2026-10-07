#ifndef GAME_GIMMICK_GIMMICKSAVEPOINT_H
#define GAME_GIMMICK_GIMMICKSAVEPOINT_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickSavePoint : public CBaseGimmick {
private:
    CHAR m_Step;     /* 0x220 */
    BYTE m_OnceFlag; /* 0x221 */
    BYTE m_EfxFlag;  /* 0x222 */
    BYTE m_reqFlag;  /* 0x223 */

public:
    CGimmickSavePoint(CTask* inParent);
    virtual ~CGimmickSavePoint();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    BOOL isRideOnSavePoint();
    virtual INT getTypeID() const;
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
};

#endif /* GAME_GIMMICK_GIMMICKSAVEPOINT_H */
