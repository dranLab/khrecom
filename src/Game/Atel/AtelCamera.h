#ifndef GAME_ATEL_ATELCAMERA_H
#define GAME_ATEL_ATELCAMERA_H

#include "System/Graphic/Camera.h"

class ICamera {
public:
    virtual BOOL setEnable(BOOL bFlag) = 0;
    virtual void lookAt(const FVECTOR& vTarget, const FVECTOR* pvUp) {}
    virtual void setPos(const FVECTOR& vPos) {}
    virtual void out() = 0;
    virtual INT isEnd() { return TRUE; }
    virtual void run() {}
    virtual void stop() {}
    virtual void ignorePause(BOOL bFlag) = 0;
    virtual void ignorePlaySpeed(BOOL bFlag) = 0;
};

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
