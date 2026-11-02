
#define ATELFOG_H_PATH "c:/KHCOM/MainApp/Game/Atel/AtelFog.h"
#include "System/Task.h"
#include <string.h>

inline void* operator new(size_t, void* p) throw() { return p; }

#include "Game/Atel/AtelFog.h"
#include "System/Graphic/WorldEnv.h"
#include "include_asm.h"

extern INT VSYNC_TIMEQ;

void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT r);

namespace ATEL {

#line 20 "c:/KHCOM/MainApp/Game/Atel/AtelFog.cpp"
CAtelFog* CAtelFog::m_pInstance = NULL;

CAtelFog::CAtelFog(CTask* pParent) : CTask(pParent, 0x8000) {
    m_nState = 0;
    m_MinAnim._fTarget = 0;
    m_MinAnim._fcalBuf = 0;
    m_MaxAnim._fTarget = 255.0f;
    m_MaxAnim._fcalBuf = 255.0f;
    TRACE("CAtelFog Create\n");
}

CAtelFog::~CAtelFog() {
    TRACE("CAtelFog Delete\n");
    m_pInstance = NULL;
}

void CAtelFog::main() {
    if (m_nState & 1) AnimColor();
    if (m_nState & 2) AnimNear();
    if (m_nState & 4) AnimFar();
    if (m_nState & 8) AnimMax();
    if (m_nState & 0x10) AnimMin();
}

INT CAtelFog::onSetFogRGBA(INT nCmd, void* pParam, INT nID) {
    BYTE* p = (BYTE*)pParam;
    INT r = p[2];
    INT g = p[1];
    /* keeps p alive past the first two loads: without it the scheduler puts the load that kills p (p[0], a2)
     * first and the pointer needs a copy */
    __asm__ volatile("" : : "r"(p));
    INT b = p[0];
    CWorldEnv::setFogColor(r, g, b);
    return -1;
}

INT CAtelFog::onSetFogRGBA_Anim(INT nCmd, void* pParam, INT nID) {
    BYTE* p = (BYTE*)pParam;
    FVECTOR v;
    m_nState |= 1;
    m_ColorAnim._vTargetColor.x = p[2];
    m_ColorAnim._vTargetColor.y = p[1];
    m_ColorAnim._vTargetColor.z = p[0];
    m_ColorAnim._vTargetColor.w = 0;
    INT& rVsync = VSYNC_TIMEQ;
    m_ColorAnim._nColorTimeLimit = ((INT*)pParam)[1] * rVsync;
    m_ColorAnim._nColorTimeCount = 0;
    IVECTOR col = CWorldEnv::getFogColor();
    FLOAT fRate = 1.0f / m_ColorAnim._nColorTimeLimit;
    v.x = col.x;
    v.y = col.y;
    v.z = col.z;
    m_ColorAnim._vColorBuf = v;
    SubVector(v, m_ColorAnim._vTargetColor, v);
    ScaleVector(v, v, fRate);
    m_ColorAnim._vColorSpeed = v;
    m_ColorAnim._nColorTimeLimit += rVsync;
    return -1;
}

INT CAtelFog::onSetFogNear(INT nCmd, void* pParam, INT nID) {
    CWorldEnv::setFogParam(*(FLOAT*)pParam, CWorldEnv::getFogFar(), m_MaxAnim._fcalBuf, m_MinAnim._fcalBuf);
    return -1;
}

INT CAtelFog::onSetFogNear_Anim(INT nCmd, void* pParam, INT nID) {
    m_nState |= 2;
    m_NearAnim._fTarget = ((FLOAT*)pParam)[0];
    INT& rVsync = VSYNC_TIMEQ;
    m_NearAnim._nTimeLimit = ((INT*)pParam)[1] * rVsync;
    m_NearAnim._nTimeCount = 0;
    FLOAT fNear = CWorldEnv::getFogNear();
    m_NearAnim._fSpeed = (m_NearAnim._fTarget - fNear) * (1.0f / m_NearAnim._nTimeLimit);
    return -1;
}

INT CAtelFog::onSetFogFar(INT nCmd, void* pParam, INT nID) {
    CWorldEnv::setFogParam(CWorldEnv::getFogNear(), *(FLOAT*)pParam, m_MaxAnim._fcalBuf, m_MinAnim._fcalBuf);
    return -1;
}

INT CAtelFog::onSetFogFar_Anim(INT nCmd, void* pParam, INT nID) {
    m_nState |= 4;
    m_FarAnim._fTarget = ((FLOAT*)pParam)[0];
    INT& rVsync = VSYNC_TIMEQ;
    m_FarAnim._nTimeLimit = ((INT*)pParam)[1] * rVsync;
    m_FarAnim._nTimeCount = 0;
    FLOAT fFar = CWorldEnv::getFogFar();
    m_FarAnim._fSpeed = (m_FarAnim._fTarget - fFar) * (1.0f / m_FarAnim._nTimeLimit);
    return -1;
}

INT CAtelFog::onSetFogMax(INT nCmd, void* pParam, INT nID) {
    m_MaxAnim._fTarget = *(FLOAT*)pParam;
    m_MaxAnim._fcalBuf = *(FLOAT*)pParam;
    CWorldEnv::setFogParam(CWorldEnv::getFogNear(), CWorldEnv::getFogFar(), m_MaxAnim._fTarget, m_MinAnim._fcalBuf);
    return -1;
}

INT CAtelFog::onSetFogMax_Anim(INT nCmd, void* pParam, INT nID) {
    m_nState |= 8;
    INT& rVsync = VSYNC_TIMEQ;
    m_MaxAnim._nTimeLimit = ((INT*)pParam)[1] * rVsync;
    m_MaxAnim._fcalBuf = m_MaxAnim._fTarget;
    m_MaxAnim._nTimeCount = 0;
    m_MaxAnim._fTarget = ((FLOAT*)pParam)[0];
    m_MaxAnim._fSpeed = (m_MaxAnim._fTarget - m_MaxAnim._fcalBuf) * (1.0f / m_MaxAnim._nTimeLimit);
    return -1;
}

INT CAtelFog::onSetFogMin(INT nCmd, void* pParam, INT nID) {
    m_MinAnim._fTarget = *(FLOAT*)pParam;
    m_MinAnim._fcalBuf = *(FLOAT*)pParam;
    CWorldEnv::setFogParam(CWorldEnv::getFogNear(), CWorldEnv::getFogFar(), m_MaxAnim._fcalBuf, m_MinAnim._fTarget);
    return -1;
}

INT CAtelFog::onSetFogMin_Anim(INT nCmd, void* pParam, INT nID) {
    m_nState |= 0x10;
    INT& rVsync = VSYNC_TIMEQ;
    m_MinAnim._nTimeLimit = ((INT*)pParam)[1] * rVsync;
    m_MinAnim._fcalBuf = m_MinAnim._fTarget;
    m_MinAnim._nTimeCount = 0;
    m_MinAnim._fTarget = ((FLOAT*)pParam)[0];
    m_MinAnim._fTarget = ((FLOAT*)pParam)[0]; /* sic */
    m_MinAnim._fSpeed = (m_MinAnim._fTarget - m_MinAnim._fcalBuf) * (1.0f / m_MaxAnim._nTimeLimit); /* sic: Max */
    return -1;
}

INT CAtelFog::AnimColor() {
    m_ColorAnim._nColorTimeCount += getFrameRate();
    BOOL bRes = m_ColorAnim.isOverTimeLimit();
    IVECTOR iv;
    FVECTOR Color;
    FVECTOR ColorSpeed;
    if (!bRes) {
        ScaleVector(ColorSpeed, m_ColorAnim._vColorSpeed, getFrameRate());
        AddVector(m_ColorAnim._vColorBuf, m_ColorAnim._vColorBuf, ColorSpeed);
        CWorldEnv::setFogColor((INT)m_ColorAnim._vColorBuf.x, (INT)m_ColorAnim._vColorBuf.y, (INT)m_ColorAnim._vColorBuf.z);
    } else {
        CWorldEnv::setFogColor((INT)m_ColorAnim._vTargetColor.x, (INT)m_ColorAnim._vTargetColor.y,
                               (INT)m_ColorAnim._vTargetColor.z);
        m_nState ^= 1;
    }
    return bRes;
}

INT CAtelFog::AnimNear() {
    INT nFrame = getFrameRate();
    m_NearAnim._nTimeCount += nFrame;
    BOOL bOver = m_NearAnim.isOverTimeLimit();
    if (!bOver) {
        FLOAT fNear = CWorldEnv::getFogNear() + m_NearAnim._fSpeed * nFrame;
        CWorldEnv::setFogParam(fNear, CWorldEnv::getFogFar(), m_MaxAnim._fcalBuf, m_MinAnim._fcalBuf);
    } else {
        CWorldEnv::setFogParam(m_NearAnim._fTarget, CWorldEnv::getFogFar(), m_MaxAnim._fcalBuf, m_MinAnim._fcalBuf);
        m_nState ^= 2;
    }
    return bOver;
}

INT CAtelFog::AnimFar() {
    INT nFrame = getFrameRate();
    m_FarAnim._nTimeCount += nFrame;
    BOOL bOver = m_FarAnim.isOverTimeLimit();
    if (!bOver) {
        FLOAT fFar = CWorldEnv::getFogFar() + m_FarAnim._fSpeed * nFrame;
        CWorldEnv::setFogParam(CWorldEnv::getFogNear(), fFar, m_MaxAnim._fcalBuf, m_MinAnim._fcalBuf);
    } else {
        CWorldEnv::setFogParam(CWorldEnv::getFogNear(), m_FarAnim._fTarget, m_MaxAnim._fcalBuf, m_MinAnim._fcalBuf);
        m_nState ^= 4;
    }
    return bOver;
}

INT CAtelFog::AnimMax() {
    m_MaxAnim._nTimeCount += getFrameRate();
    BOOL bOver = m_MaxAnim.isOverTimeLimit();
    if (!bOver) {
        m_MaxAnim._fcalBuf += m_MaxAnim._fSpeed * getFrameRate();
        CWorldEnv::setFogParam(CWorldEnv::getFogNear(), CWorldEnv::getFogFar(), m_MaxAnim._fcalBuf, m_MinAnim._fcalBuf);
    } else {
        CWorldEnv::setFogParam(CWorldEnv::getFogNear(), CWorldEnv::getFogFar(), m_MaxAnim._fTarget, m_MinAnim._fcalBuf);
        m_MaxAnim._fcalBuf = m_MaxAnim._fTarget;
        m_nState ^= 8;
    }
    return bOver;
}

INT CAtelFog::AnimMin() {
    m_MinAnim._nTimeCount += getFrameRate();
    BOOL bOver = m_MinAnim.isOverTimeLimit();
    if (!bOver) {
        m_MinAnim._fcalBuf += m_MinAnim._fSpeed * getFrameRate();
        CWorldEnv::setFogParam(CWorldEnv::getFogNear(), CWorldEnv::getFogFar(), m_MaxAnim._fcalBuf, m_MinAnim._fcalBuf);
    } else {
        CWorldEnv::setFogParam(CWorldEnv::getFogNear(), CWorldEnv::getFogFar(), m_MaxAnim._fcalBuf, m_MinAnim._fTarget);
        m_MinAnim._fcalBuf = m_MinAnim._fTarget;
        m_nState ^= 0x10;
    }
    return bOver;
}

} // namespace ATEL

// SOURCE DATA: .data .rodata
