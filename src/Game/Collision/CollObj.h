// PARTIAL: added for Game/Player/SummonSinba
#ifndef GAME_COLLISION_COLLOBJ_H
#define GAME_COLLISION_COLLOBJ_H

#include "khtypes.h"
#include "System/Vector.h"
#include "System/SJoint.h"

class CCollision {
public:
    FVECTOR m_inWS;      /* 0x00 */
    FVECTOR m_inWE;      /* 0x10 */
    FVECTOR m_outWE;     /* 0x20 */
    FVECTOR m_outNormal; /* 0x30 */
    FVECTOR m_outDetect; /* 0x40 */
    UBYTE _m_0x50[0x20]; /* 0x50 m_Type .. 0x6c m_outInfoO */

    virtual ~CCollision(); /* 0x70 */
    void setStartPos(const FVECTOR& inPos) { m_inWS = inPos; }
    void setEndPos(const FVECTOR& inPos) { m_inWE = inPos; }
};

class CCollObj : public CSJointT<CCollObj> {
protected:
    INT m_Enable;              /* 0x08 */
    CCollision* m_pCollision;  /* 0x0c */
    HANDLE m_hTask;            /* 0x10 */

public:
    virtual ~CCollObj();
    virtual void off();
    virtual void on();
    virtual INT detectEtc();
    virtual void setKind(INT inKind);
    virtual void init();

    void setStartPos(const FVECTOR& inPos) {
        if (m_pCollision) m_pCollision->setStartPos(inPos);
    }
    void setEndPos(const FVECTOR& inPos) {
        if (m_pCollision) m_pCollision->setEndPos(inPos);
    }
    FVECTOR* getDetectPos() {
        if (m_pCollision) return &m_pCollision->m_outDetect;
        return 0;
    }
};

#endif /* GAME_COLLISION_COLLOBJ_H */
