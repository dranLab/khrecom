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
