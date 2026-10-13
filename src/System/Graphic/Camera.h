// PARTIAL: added for Game/Atel/AtelCamera
#ifndef SYSTEM_GRAPHIC_CAMERA_H
#define SYSTEM_GRAPHIC_CAMERA_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Vector.h"


// PARTIAL: added for Game/Atel/AtelCamera
class CCoord {
public:
    FMATRIX m_Local;
    // PARTIAL: 0x40..0x8f split for Game/Gimmick/GimmickFountain
    TIMEQ m_FixTimestamp;   /* 0x40 */
    INT m_Flag;             /* 0x44 */
    TIMEQ m_WorldTimestamp; /* 0x48 */
    CCoord* m_pParent;      /* 0x4c */
    UBYTE _pad50[0x40];     /* 0x50 m_Rot */
    FMATRIX m_World;     /* 0x90 */
    FMATRIX& getWorldMatrix() { return m_World; } // PARTIAL: added for Game/Gimmick/GimmickBell
    FMATRIX& getLocalMatrix() { return m_Local; } // PARTIAL: added for Game/Gimmick/GimmickFountain
    CCoord* getParent() { return m_pParent; }     // PARTIAL: added for Game/Gimmick/GimmickFountain
};

class CCamera : public CTaskSecondary {
private:
    FVECTOR m_vOffset;
    FVECTOR m_vLook;
    INT m_nCameraPriority;
    FLOAT m_fProjectionDist;

public:
    CCamera(CTask* pParent, INT nPri);
    virtual ~CCamera();
    void setPos(const FVECTOR& vPos) { Crd(0).m_Local.setTrans(vPos); }
    BOOL setEnable(BOOL bFlag);
    void lookAt(const FVECTOR& vTarget, const FVECTOR* pvUp);
};

#endif /* SYSTEM_GRAPHIC_CAMERA_H */
