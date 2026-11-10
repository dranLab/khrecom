#ifndef SYSTEM_GRAPHIC_HERMITEPATHCAMERA_H
#define SYSTEM_GRAPHIC_HERMITEPATHCAMERA_H

#include "khtypes.h"
#include "System/Graphic/Camera.h"
#include "System/Graphic/PathData.h"

class CPathVector;
class CHermitePath;
class CCollMapCamera;

struct PATH_TARGET_PARAM {
    PATH_PARAM sPDParam;  /* 0x00 */
    CHermitePath* pHPath; /* 0x30 */
    INT iMoveMode;        /* 0x34 */
    CTask* pParent;       /* 0x38 */
};

/* The inline members are emitted (weak) in System/Graphic/HermitePathCamera.cpp, in this order: keep it. */
class CHermitePathCamera : public CCamera {
private:
    INT m_iStep;               /* 0x070 */
    UINT m_iStatus;            /* 0x074 */
    INT m_iKind;               /* 0x078 */
    CPathVector* m_pPath;      /* 0x07c */
    CHermitePath* m_pHPath;    /* 0x080 */
    CHermitePath* m_pMyHPath;  /* 0x084 */
    const CTask* m_pTarget;    /* 0x088 */
    FLOAT m_fDistance;         /* 0x08c */
    FLOAT m_fOffsetY;          /* 0x090 */
    UINT m_iPathFlags;         /* 0x094 */
    FLOAT m_fLearpIn;          /* 0x098 */
    FLOAT m_fLearpOut;         /* 0x09c */
    FLOAT m_fLookDelay;        /* 0x0a0 */
    FLOAT m_fStep;             /* 0x0a4 */
    FLOAT m_fStartProj;        /* 0x0a8 */
    FLOAT m_fEndProj;          /* 0x0ac */
    FLOAT m_fProj;             /* 0x0b0 */
    FVECTOR m_vPos;            /* 0x0c0 */
    FVECTOR m_vWPos;           /* 0x0d0 */
    FVECTOR m_vTargetPos;      /* 0x0e0 */
    FVECTOR m_vLearpAimPos;    /* 0x0f0 */
    FVECTOR m_vLearpOldPos;    /* 0x100 */
    FVECTOR m_vLearpAimLook;   /* 0x110 */
    FVECTOR m_vLearpOldLook;   /* 0x120 */
    FLOAT m_fLearpAimProj;     /* 0x130 */
    FLOAT m_fLearpOldProj;     /* 0x134 */
    CCollMapCamera* m_pCollMapCamera; /* 0x138 */
    CDbgArrow m_DDPathCamArrow;       /* 0x140 */

public:
    CHermitePathCamera(CTask* p, const FVECTOR* pPath, INT iPathMax, const CTask* pTarget,
                       const PATH_TARGET_PARAM& sTargetParam);
    CHermitePathCamera(CTask* p, const CPathVector* pPath, const CTask* pTarget, const CHermitePath* pHPath,
                       INT iType, const CTask* pParent);
    virtual ~CHermitePathCamera();
    const FVECTOR& getCameraPos() const { return ((CCoord&)Crd(0)).getWorldMatrix().getTransRef(); }
    const FVECTOR* getTargetPos() const { return getLook(); }
    BOOL isEnd() const;
    void ignorePlaySpeed(BOOL bInFlag) { CTask::ignorePlaySpeed(bInFlag); }
    void stop() { m_iStatus |= 2; }
    void run() { m_iStatus &= ~2; }
    void initBGCheck(BOOL fgTransparency);
    void noLearpOut();
    virtual void main();
    virtual void disp();
    void out();

private:
    void _init(const CTask* pTarget, const PATH_TARGET_PARAM& sTargetParam);
    void _setMyCamPath(const PATH_TARGET_PARAM& sTargetParam);
    FLOAT updatePos(FVECTOR& vPos, FVECTOR& vLook, FLOAT fr, BOOL bUpdate);
    void updateMatrix(FVECTOR& vPos, FVECTOR& vLook, FLOAT fRoll);
};

#endif /* SYSTEM_GRAPHIC_HERMITEPATHCAMERA_H */
