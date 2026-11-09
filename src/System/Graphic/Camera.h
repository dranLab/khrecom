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
    // PARTIAL: the ref's fields from here are private (ours public)
    union {
        struct {
            union {
                struct {
                    TIMEQ m_FixTimestamp; /* 0x40 */
                    INT m_Flag;           /* 0x44 */
                };
                ULONGLONG _ulFixTimestamp;
            };
            TIMEQ m_WorldTimestamp; /* 0x48 */
            CCoord* m_pParent;      /* 0x4c */
        };
        u_long128 _q40; // MATCH: Game/Atel/EventTrap: the constructor clears 0x40..0x4f at once
    };
    union {
        FMATRIX m_Rot;              /* 0x50 */
        struct {
            int m_hHandlePad___[12];
            FLOAT m_fScaleX;        /* 0x80 */
            FLOAT m_fScaleY;        /* 0x84 */
            FLOAT m_fScaleZ;        /* 0x88 */
            INT m_hHandle;          /* 0x8c */
        };
    };
    FMATRIX m_World;     /* 0x90 */
    void setLocal(const FMATRIX& matIn) {
        if (m_Local != matIn) {
            m_Local = matIn;
            touch();
        }
    }
    void updateLocal(/* parameters unknown */);
    void setTrans(/* parameters unknown */);
    void updateTrans(/* parameters unknown */);
    void setParent(CCoord* pParent) { m_pParent = pParent; }
    void touch(); // defined below CCamera
    CCoord() {
        _q40 = 0;
        m_fScaleX = 1.0f;
        m_fScaleY = 1.0f;
        m_fScaleZ = 1.0f;
        m_hHandle = 0;
    }
    void fix(INT n = 0);
    TIMEQ getWorldTimestamp(/* parameters unknown */);
    FMATRIX& getWorldMatrix() { return m_World; }
    FMATRIX& getWorldRot33Matrix(/* parameters unknown */);
    FVECTOR getWorldPos() { return m_World.v[3]; } // PARTIAL: Effect/effect and EffectMan (original name unknown)
    FMATRIX& getLocalMatrix() { return m_Local; }
    CCoord* getParent() { return m_pParent; }
    void recalcLocalMatrix();
    void getScale(FVECTOR& outScale) {
        outScale.x = m_fScaleX;
        outScale.y = m_fScaleY;
        outScale.z = m_fScaleZ;
    }
    void recalcWorldMatrix();
    void getRotTransPersMatrix(FMATRIX& outMat);
    void getRotTransPersMatrix_VF04_07();
    void RotTransPers(IVECTOR& outV, const FVECTOR& inV);
    void RotTransPers(FVECTOR& outV, const FVECTOR& inV);
    void resetTimestamp() {
        m_FixTimestamp = -1;
        m_WorldTimestamp = -1;
    }
    HANDLE getHandle(/* parameters unknown */);
    void setHandle(HANDLE h) { m_hHandle = h; }
    void setScale(const FVECTOR& inScale) {
        m_fScaleX = inScale.x;
        m_fScaleY = inScale.y;
        m_fScaleZ = inScale.z;
        m_Flag = 1;
    }

private:
    void _mulParent(/* parameters unknown */);
    void _checkParent(/* parameters unknown */);
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

inline void CCoord::touch() { m_Flag = 1; }

FLOAT ViewAngleToProjection(FLOAT fViewAngle);
FLOAT ProjectionToViewAngle(FLOAT fProjection);

#endif /* SYSTEM_GRAPHIC_CAMERA_H */
