#ifndef GAME_ATEL_ATELCAMERA_H
#define GAME_ATEL_ATELCAMERA_H

#include "System/Graphic/Camera.h"

class CAtelCamera : public CCamera, public ICamera {
public:
    CAtelCamera(CTask* pInParent, INT nInPri);
    virtual ~CAtelCamera();
    virtual BOOL setEnable(BOOL bInFlag) { return CCamera::setEnable(bInFlag); }
    virtual void lookAt(const FVECTOR& vInTarget, const FVECTOR* pvInUp) { CCamera::lookAt(vInTarget, pvInUp); }
    virtual void setPos(const FVECTOR& vecInPos) { CCamera::setPos(vecInPos); }
    virtual void out() { CTask::out(); }
    virtual void ignorePause(BOOL bInFlag) { CTask::ignorePause(bInFlag); }
    virtual void ignorePlaySpeed(BOOL bInFlag) { CTask::ignorePlaySpeed(bInFlag); }
};

#endif /* GAME_ATEL_ATELCAMERA_H */
