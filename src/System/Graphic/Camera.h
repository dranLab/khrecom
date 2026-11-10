// PARTIAL: added for Game/Atel/AtelCamera
#ifndef SYSTEM_GRAPHIC_CAMERA_H
#define SYSTEM_GRAPHIC_CAMERA_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Coord.h"


class CCamera : public CTaskSecondary {
private:
    FVECTOR m_vOffset;
    FVECTOR m_vLook;
    INT m_nCameraPriority;
    FLOAT m_fProjectionDist;

public:
    CCamera(CTask* pParent, INT nPri);
    virtual ~CCamera();
    void setCameraPriority(INT nPri) {
        if (nPri < 0) {
            m_nCameraPriority = 0x80;
        } else {
            m_nCameraPriority = nPri;
        }
    }
    INT getCameraPriority() const { return m_nCameraPriority; }
    void setProjectionDist(FLOAT fDist) { *(FLOAT*)&m_fProjectionDist = fDist; } /* MATCH: cast store keeps the dependence on a following global load */
    FLOAT getProjectionDist() const { return m_fProjectionDist; }
    void setGakaku(FLOAT fGakaku);
    void setOffsetX(FLOAT fX) { m_vOffset.x = fX; }
    FLOAT getOffsetX() const { return m_vOffset.x; }
    void setOffsetY(FLOAT fY) { m_vOffset.y = fY; }
    FLOAT getOffsetY() const { return m_vOffset.y; }
    void setOffsetXY(FLOAT fX, FLOAT fY) {
        m_vOffset.x = fX;
        m_vOffset.y = fY;
    }
    void setPos(const FVECTOR& vPos) { Crd(0).m_Local.setTrans(vPos); }
    void setPos(FLOAT fX, FLOAT fY, FLOAT fZ) {
        FVECTOR vPos;
        vPos.set(fX, fY, fZ);
        setPos(vPos);
    }
    FMATRIX& getLocalMat() { return Crd(0).getLocalMatrix(); }
    void setLocalMat(const FMATRIX& mat) { Crd(0).setLocal(mat); }
    BOOL setEnable(BOOL bFlag);
    void setAnchor(CCoord* pParentCrd);
    void lookAt(const FVECTOR& vTarget, const FVECTOR* pvUp);
    void lookAt(const FVECTOR& vTarget, FLOAT fBank);
    void setLook(const FVECTOR& vLook) {
        m_vLook = vLook;
        m_vLook.w = 1.0f;
    }
    const FVECTOR* getLook() const {
        if (m_vLook.w == 0.0f) return NULL; return &m_vLook; /* two returns, not a ternary: the original's labels end a cse block in DeFocus's _makePkt* */
    }

protected:
    void init(INT nPri);
};

// PARTIAL: moved here from Game/Atel/AtelCamera.h for System/Graphic/PathCamera
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


struct CameraQuakeParam {
    UINT _nTime;
    FLOAT _fPower;
    FLOAT _fCycle;
};

class CCameraQuake {
private:
    FLOAT m_fStep;
    FLOAT m_fSpeed;
    FLOAT m_fPower;
    FLOAT m_fAbsorb;
    FLOAT m_fOffset;
    FLOAT m_fGetaPer;
    FLOAT m_fGeta;
    TIMEQ m_nTime;

public:
    CCameraQuake(const CameraQuakeParam* pParam, FLOAT fGeta);
    void setup(const CameraQuakeParam& inParam, FLOAT fGeta);
    BOOL isEnd() { return m_nTime < 0; }
    FLOAT getOffset() { return m_fOffset; }
    BOOL updateParam(TIMEQ nTQ);
};

class CCameraQuakeV : public CCameraQuake, public CTask {
public:
    virtual ~CCameraQuakeV() {}
    CCameraQuakeV(CCamera& inParent, const CameraQuakeParam* pParam, FLOAT inOffsetY)
        : CCameraQuake(pParam, inOffsetY), CTask(&inParent, 0x4000) {}
    virtual void main();
};

class CCameraQuakeH : public CCameraQuake, public CTask {
public:
    virtual ~CCameraQuakeH() {}
    CCameraQuakeH(CCamera& inParent, const CameraQuakeParam* pParam)
        : CCameraQuake(pParam, 0.0f), CTask(&inParent, 0x4000) {}
    virtual void main();
};


FLOAT ViewAngleToProjection(FLOAT fViewAngle);
FLOAT ProjectionToViewAngle(FLOAT fProjection);

#endif /* SYSTEM_GRAPHIC_CAMERA_H */
