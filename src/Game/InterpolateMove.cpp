#include "Game/InterpolateMove.h"

extern "C" {
float sinf(float x);
float cosf(float x);
float atan2f(float y, float x);
}

#include "System/Coord.h"
// from System/Math3D.h
#include "System/Math3D.h"















void CInterpolateMove::_setup(INT nType, FLOAT fFrame) {
    m_nType = nType;
    if (fFrame < 1.0f) {
        fFrame = 1.0f;
    }
    m_fRate = 0.0f;
    m_fAdd = 1.0f / fFrame;
}

FLOAT CInterpolateMove::_update(FLOAT fR, BOOL bNoLimit) {
    m_fRate += m_fAdd * fR;
    if (m_fRate >= 1.0f && !bNoLimit) {
        m_fRate = 1.0f;
        return 1.0f;
    }
    FLOAT fRate;
    switch (m_nType) {
    case 0:
    default:
        fRate = m_fRate;
        break;
    case 1:
        fRate = sinf(m_fRate * 1.5707964f);
        break;
    case 2:
        fRate = 1.0f - cosf(m_fRate * 1.5707964f);
        break;
    case 3:
        fRate = 1.0f - cosf(m_fRate * 3.1415927f);
        fRate *= 0.5f;
        break;
    }
    return fRate;
}

void CInterpolateVector::setupFrame(INT nType, const FVECTOR& vAim, const FVECTOR& vOld, FLOAT fFrame) {
    _setup(nType, fFrame);
    m_vPos = vOld;
    m_vOld = vOld;
    m_vAim = vAim;
}

void CInterpolateVector::setupSpeed(INT nType, const FVECTOR& vAim, const FVECTOR& vOld, FLOAT fSpeed) {
    FLOAT fFrame = Math3D::PointToPointDistance(vAim, vOld);
    setupFrame(nType, vAim, vOld, fFrame / fSpeed);
}

BOOL CInterpolateVector::update(FLOAT fR, BOOL bNoLimit) {
    BOOL bRet;
    FLOAT fRate = _update(fR, bNoLimit);
    if (fRate >= 1.0f && !bNoLimit) {
        m_vPos = m_vAim;
        bRet = TRUE;
    } else {
        InterVector(m_vPos, m_vAim, m_vOld, fRate);
        bRet = FALSE;
    }
    return bRet;
}

void CInterpolateVector::getRot(FVECTOR& vRot) {
    FVECTOR fvPos1;
    FVECTOR fvPos2;
    SubVectorXYZ(fvPos1, m_vAim, m_vOld);
    Normalize(fvPos1, fvPos1);
    MulVectorXYZ(fvPos2, fvPos1, fvPos1);
    FLOAT ret = fvPos2.x + fvPos2.z;
    ret = _sqrtf(ret);
    vRot.x = -atan2f(fvPos1.y, ret);
    vRot.y = atan2f(fvPos1.x, fvPos1.z);
    vRot.z = 0.0f;
}

void CInterpolateScalar::setupFrame(INT nType, FLOAT fAim, FLOAT fOld, FLOAT fFrame) {
    _setup(nType, fFrame);
    m_fPos = fOld;
    m_fOld = fOld;
    m_fAim = fAim;
}

void CInterpolateScalar::setupSpeed(INT nType, FLOAT fAim, FLOAT fOld, FLOAT fSpeed) {
    FLOAT ret = fAim - fOld;
    ret = _fabsf(ret);
    setupFrame(nType, fAim, fOld, ret / fSpeed);
}

BOOL CInterpolateScalar::update(FLOAT fR, BOOL bNoLimit) {
    BOOL bRet;
    FLOAT fRate = _update(fR, bNoLimit);
    if (fRate >= 1.0f && !bNoLimit) {
        m_fPos = m_fAim;
        bRet = TRUE;
    } else {
        m_fPos = (m_fAim - m_fOld) * fRate + m_fOld;
        bRet = FALSE;
    }
    return bRet;
}
