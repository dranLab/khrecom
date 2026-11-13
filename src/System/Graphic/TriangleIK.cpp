
#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define MODELDATA_H_PATH "c:/KHCOM/MainApp/System/Graphic/modeldata.h" /* MATCH: this unit names modeldata.h so (header default "System/Graphic/modeldata.h") */
#define MODEL_H_PATH "c:/KHCOM/MainApp/System/Graphic/Model.h" /* MATCH: this unit names Model.h so (header default "System/Graphic/Model.h") */
#define EFFECT_H_PATH "c:/KHCOM/MainApp/System/Graphic/Effect/effect.h" /* MATCH: this unit names effect.h so (header default "System/Graphic/Effect/effect.h") */
#define COLLOBJ_H_INFO_AS_INT /* MATCH: getDetectInfo as INT (see CollObj.h) */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "Game/Collision/CollUtil.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/TriangleIK.h"
#include "System/Graphic/Effect/EffectMan.h"
// MATCH: no Camera.h here (nothing uses it): it brings Task.h, whose getDebugInfo "" retail's .rodata lacks
#include "Game/Collision/CollMap.h"
#include "System/Math3D.h"
#include "System/Debug.h"

#include "System/Coord.h"
extern "C" float acosf(float x);

#define FOOT_POS_NONE 1.70141183e+38f

static CHAR* pszArmNames[2][3] = {
    { "l_uparm", "l_foarm", "lx_rot" },
    { "r_uparm", "r_foarm", "rx_rot" },
};

static CHAR* pszLegNames[2][3] = {
    { "l_femur", "l_tibia", "l_foot" },
    { "r_femur", "r_tibia", "r_foot" },
};

static CHAR* pszLegNames2[2] = { "l_toes", "r_toes" };

BOOL CTriangleIK::setBoneIdx(INT nIdx0, INT nIdx1, INT nIdx2) {
    if (nIdx0 < 0 || nIdx1 < 0 || nIdx2 < 0) {
        m_nIdxAll = -1;
        return FALSE;
    }
    m_nIdx[0] = nIdx0;
    m_nIdx[1] = nIdx1;
    m_nIdx[2] = nIdx2;
    return TRUE;
}

void CTriangleIK::calc(CCoords& Crds, BOOL bDirF, const FVECTOR& vPos) {
    FLOAT fLenA;
    FLOAT fLenB;
    FLOAT fLenC;
    FVECTOR vWA;
    FVECTOR vWB;
    FVECTOR vWC;
    FVECTOR vTmp[2];
    FLOAT fRotZ;
    FVECTOR vAxisX;
    FVECTOR vAxisY;
    FVECTOR vAxisZ;
    QUATERNION quat;
    FMATRIX mMat;
    FVECTOR vScale;

    if (m_nIdx[0] == 0xFF) return;
    CCoord& rCrd0 = Crds.Crd(m_nIdx[0]);
    CCoord& rCrd2 = Crds.Crd(m_nIdx[2]);
    CCoord& rCrd1 = Crds.Crd(m_nIdx[1]);

    fLenB = m_fLen[1];
    fLenA = m_fLen[0];
    vWA = rCrd0.getWorldMatrix().v[3];
    vWB = vPos;
    vWC = rCrd1.getWorldMatrix().v[3];

    SubVectorXYZ(vTmp[0], vWB, vWA);
    fLenC = Math3D::VectorLength(vTmp[0]);
    Normalize(vTmp[0], vTmp[0]);
    ScaleVectorXYZ(vWC, vTmp[0], fLenA);
    vWC.w = 1.0f;
    if (fLenB + fLenA - 0.001f < fLenC) {
        fLenC = fLenB + fLenA - 0.001f;
        ScaleVectorXYZ(vWB, vTmp[0], fLenC);
        AddVectorXYZ(vWB, vWB, vWA);
        vWB.w = 1.0f;
    }

    fRotZ = acosf((fLenA * fLenA + fLenC * fLenC - fLenB * fLenB) / (2.0f * fLenA * fLenC));
    if (bDirF) fRotZ = -fRotZ;

    vAxisZ.set(0.0f, 0.0f, 1.0f, 1.0f);
    ApplyMatrix33(vAxisZ, rCrd0.m_Rot, vAxisZ);
    Quaternion::QuaternionRotationAxis(quat, vAxisZ, fRotZ);
    Quaternion::QuaternionToMatrix(mMat, quat, vWA);
    ApplyMatrix(vWC, mMat, vWC);
    SubVectorXYZ(vTmp[0], vWA, vWC);
    SubVectorXYZ(vTmp[1], vWB, vWC);
    if (bDirF) {
        OuterProduct(vAxisZ, vTmp[1], vTmp[0]);
    } else {
        OuterProduct(vAxisZ, vTmp[0], vTmp[1]);
    }
    Normalize(vAxisZ, vAxisZ);

    rCrd0.getScale(vScale);
    SubVector(vAxisX, vWC, vWA);
    Normalize(vAxisX, vAxisX);
    OuterProduct(vAxisY, vAxisZ, vAxisX);
    Normalize(vAxisY, vAxisY);
    ScaleVectorXYZ(rCrd0.getWorldMatrix().v[0], vAxisX, vScale.x);
    ScaleVectorXYZ(rCrd0.getWorldMatrix().v[1], vAxisY, vScale.y);
    ScaleVectorXYZ(rCrd0.getWorldMatrix().v[2], vAxisZ, vScale.z);
    rCrd0.getWorldMatrix().m[0][3] = rCrd0.getWorldMatrix().m[1][3] = rCrd0.getWorldMatrix().m[2][3] = 0.0f;

    rCrd1.getScale(vScale);
    SubVector(vAxisX, vWB, vWC);
    Normalize(vAxisX, vAxisX);
    OuterProduct(vAxisY, vAxisZ, vAxisX);
    Normalize(vAxisY, vAxisY);
    ScaleVectorXYZ(rCrd1.getWorldMatrix().v[0], vAxisX, vScale.x);
    ScaleVectorXYZ(rCrd1.getWorldMatrix().v[1], vAxisY, vScale.y);
    ScaleVectorXYZ(rCrd1.getWorldMatrix().v[2], vAxisZ, vScale.z);
    rCrd1.getWorldMatrix().m[0][3] = rCrd1.getWorldMatrix().m[1][3] = rCrd1.getWorldMatrix().m[2][3] = 0.0f;
    rCrd1.getWorldMatrix().v[3] = vWC;

    rCrd2.getWorldMatrix().v[3] = vWB;

    rCrd0.recalcLocalMatrix();
    rCrd0.touch();
    rCrd1.recalcLocalMatrix();
    rCrd1.touch();
    rCrd2.recalcLocalMatrix();
    rCrd2.touch();
}

BOOL CBaseIK::init(CCoords& Crds, int (*nIdx)[3]) {
    BOOL bRet = FALSE;
    INT i;

    for (i = 0; i < 2; i++) {
        if (m_cTIK[i].setBoneIdx(nIdx[i][0], nIdx[i][1], nIdx[i][2])) {
            bRet = TRUE;
            FLOAT fLen0 = Math3D::PointToPointDistance(Crds.Crd(nIdx[i][1]).getWorldMatrix().v[3],
                                                       Crds.Crd(nIdx[i][0]).getWorldMatrix().v[3]);
            FLOAT fLen1 = Math3D::PointToPointDistance(Crds.Crd(nIdx[i][2]).getWorldMatrix().v[3],
                                                       Crds.Crd(nIdx[i][1]).getWorldMatrix().v[3]);
            m_cTIK[i].setBoneLength(fLen0, fLen1);
        }
    }
    return bRet;
}

void CBaseIK::update(CCoords& Crds, BOOL bDirF, const FVECTOR* pvPosL, const FVECTOR* pvPosR) {
    const FVECTOR* pvPos;
    INT i;

    if (m_cTIK[0].getBoneIdx(0) < 0 && m_cTIK[1].getBoneIdx(0) < 0) return;
    for (i = 0; i < 2; i++) {
        CTriangleIK& rTIK = m_cTIK[i];
        INT nIdx = rTIK.getBoneIdx(2);
        if (nIdx < 0) continue;
        if (i == 0) {
            if (pvPosL) pvPos = pvPosL;
            else pvPos = &Crds.Crd(nIdx).getWorldMatrix().v[3];
        } else {
            if (pvPosR) pvPos = pvPosR;
            else pvPos = &Crds.Crd(nIdx).getWorldMatrix().v[3];
        }
        rTIK.calc(Crds, bDirF, *pvPos);
    }
}

BOOL CArmIK::isIK(const MODELDATA& Model) {
    INT i;
    INT j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            if (Model.getBoneIndex(pszArmNames[i][j]) < 0) return FALSE;
        }
    }
    return TRUE;
}

BOOL CArmIK::init(CCoords& Crds, const MODELDATA& Model) {
    int nIdx[2][3];
    INT i;
    INT j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            nIdx[i][j] = Model.getBoneIndex(pszArmNames[i][j]);
        }
    }
    return CBaseIK::init(Crds, nIdx);
}

void CArmIK::update(CCoords& Crds, const FVECTOR* pvPosL, const FVECTOR* pvPosR) {
    CBaseIK::update(Crds, FALSE, pvPosL, pvPosR);
}

BOOL CLegIK::isIK(const MODELDATA& Model) {
    INT i;
    INT j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            if (Model.getBoneIndex(pszLegNames[i][j]) < 0) return FALSE;
        }
    }
    return TRUE;
}

BOOL CLegIK::init(CCoords& Crds, const MODELDATA& Model, FLOAT fFootLen, FLOAT fToesLen) {
    BOOL bRet;
    int nIdx[2][3];
    INT i;
    INT j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            nIdx[i][j] = Model.getBoneIndex(pszLegNames[i][j]);
        }
    }
    bRet = CBaseIK::init(Crds, nIdx);
    if (bRet) {
        for (INT i = 0; i < 2; i++) {
            m_fFootLen[i] = fFootLen;
            m_fToesLen[i] = fToesLen;
            if (m_cTIK[i].getBoneIdx(2) < 0) continue;
            if (fFootLen < 0.0f) {
                m_fFootLen[i] = Crds.Crd(nIdx[i][2]).getWorldMatrix().v[3].y - Crds.Crd(0).getWorldMatrix().v[3].y;
            }
            m_nToesIdx[i] = Model.getBoneIndex(pszLegNames2[i]);
            if (m_nToesIdx[i] >= 0 && fToesLen < 0.0f) {
                m_fToesLen[i] =
                    Crds.Crd(m_nToesIdx[i]).getWorldMatrix().v[3].y - Crds.Crd(0).getWorldMatrix().v[3].y;
            }
        }
    }
    return bRet;
}

void CLegIK::update(CCoords& Crds, const FVECTOR* pvPosL, const FVECTOR* pvPosR) {
    CBaseIK::update(Crds, TRUE, pvPosL, pvPosR);
}

void CLegIK::getFootPos(FVECTOR* pvOutPosL, FVECTOR* pvOutPosR, CCoords& Crds, FLOAT fr) {
    FLOAT fLenFoot;
    FVECTOR vTmp[2];
    INT nIdx1;
    INT nIdx0;
    INT i;
    BOOL bRHit;
    BOOL bRHit2;

    if (m_cTIK[0].getBoneIdx(0) < 0 && m_cTIK[1].getBoneIdx(0) < 0) return;
    for (i = 0; i < 2; i++) {
        nIdx1 = m_cTIK[i].getBoneIdx(2);
        nIdx0 = m_cTIK[i].getBoneIdx(0);
        if (nIdx1 < 0 || nIdx0 < 0) continue;
        bRHit = FALSE;
        bRHit2 = FALSE;
        FVECTOR vPos = Crds.Crd(nIdx1).getWorldMatrix().v[3];
        fLenFoot = m_fFootLen[i];
        CCollMap stColl;
        stColl.init(COLL_KIND_FOOT, COLL_TYPE_POINT, 0, 0);
        if (m_nToesIdx[i] >= 0) {
            FVECTOR vPosT = Crds.Crd(m_nToesIdx[i]).getWorldMatrix().v[3];
            FVECTOR vDir;
            vDir.set(m_fToesLen[i], 0.0f, 0.0f, 1.0f);
            ApplyMatrix33(vDir, Crds.Crd(m_nToesIdx[i]).m_Rot, vDir);
            vTmp[0] = vPosT;
            vTmp[1] = vPosT;
            FLOAT _x = Crds.Crd(nIdx0).getWorldMatrix().v[3].y;
            vTmp[0].y = _x - 0.1f;
            if (vTmp[1].y < vTmp[0].y) {
                AddVectorXYZ(vTmp[1], vTmp[1], vDir);
                vTmp[1].y -= 0.0067499997f;
                stColl.setStartPos(vTmp[0]);
                stColl.setEndPos(vTmp[1]);
                if (stColl.collideMap() || (bRHit = stColl.collideRide(FALSE))) {
                    vTmp[0] = *stColl.getOutPos();
                    vPos.y += vTmp[0].y - (vPosT.y + vDir.y);
                }
            }
        }
        vTmp[0] = vPos;
        vTmp[1] = vPos;
        FLOAT _x = Crds.Crd(nIdx0).getWorldMatrix().v[3].y;
        vTmp[0].y = _x - 0.1f;
        if (vTmp[1].y < vTmp[0].y) {
            vTmp[1].y -= fLenFoot + 0.0067499997f;
            stColl.setStartPos(vTmp[0]);
            stColl.setEndPos(vTmp[1]);
            if (stColl.collideMap() || (bRHit2 = stColl.collideRide(FALSE))) {
                vTmp[0] = *stColl.getOutPos();
                vPos.y = vTmp[0].y + fLenFoot;
                bRHit = FALSE;
            }
        }
        if (!bRHit && !bRHit2) {
            if (m_fPosY[i] != FOOT_POS_NONE) {
                FLOAT fOfsY = vPos.y - m_fPosY[i];
                FLOAT fRate = fr * 0.33333334f;
                if (fOfsY < 0.0f) fRate = 1.0f;
                if (fRate < 1.0f) vPos.y = m_fPosY[i] + fOfsY * fRate;
            }
            m_fPosY[i] = vPos.y;
        } else {
            m_fPosY[i] = FOOT_POS_NONE;
        }
        if (i == 0) {
            if (pvOutPosL) *pvOutPosL = vPos;
        } else {
            if (pvOutPosR) *pvOutPosR = vPos;
        }
    }
}

void CLegIK::clearOldFootPos() {
    m_fPosY[1] = m_fPosY[0] = FOOT_POS_NONE;
}

void CLegIK::updateFootEffect(CCoords& Crds) {
    FLOAT fLenFoot;
    FVECTOR vTmp[2];
    INT nIdx;
    INT i;

    if (!m_pszEffName) return;
    if (m_cTIK[0].getBoneIdx(0) < 0 && m_cTIK[1].getBoneIdx(0) < 0) return;
    for (i = 0; i < 2; i++) {
        nIdx = m_cTIK[i].getBoneIdx(2);
        INT nIdx0 = m_cTIK[i].getBoneIdx(0);
        if (nIdx < 0 || nIdx0 < 0) continue;
        FVECTOR vPos = Crds.Crd(nIdx).getWorldMatrix().v[3];
        fLenFoot = m_fFootLen[i];
        CCollMap stColl;
        stColl.init(COLL_KIND_FOOT, COLL_TYPE_POINT, 0, 0);
        m_iStatus &= ~(4 << i);
        vTmp[0] = vPos;
        vTmp[1] = vPos;
        vTmp[1].y -= fLenFoot + 0.0067499997f;
        stColl.setStartPos(vTmp[0]);
        stColl.setEndPos(vTmp[1]);
        if (stColl.collideMap() || stColl.collideRide(FALSE)) {
            if (!(m_iStatus & (1 << i))) m_iStatus |= 4 << i;
            m_iStatus |= 1 << i;
        } else if (m_iStatus & (1 << i)) {
            vTmp[0] = vPos;
            vTmp[1] = vPos;
            vTmp[1].y -= fLenFoot + 0.5f;
            stColl.setStartPos(vTmp[0]);
            stColl.setEndPos(vTmp[1]);
            if (stColl.collideMap() || stColl.collideRide(FALSE)) {
                if ((((UINT)stColl.getDetectInfo() >> 8) & 0xFF) == 3) {
                    FMATRIX mMat;
                    FVECTOR vForward;
                    FVECTOR vUp;
                    FVECTOR vRight;
                    vTmp[1] = *stColl.getOutPos();
                    vTmp[0].y = vTmp[1].y;
                    vRight.set(0.0f, 0.0f, -1.0f, 1.0f);
                    ApplyMatrix33(vRight, Crds.Crd(nIdx).getWorldMatrix(), vRight);
                    vUp = *stColl.getDetectNormal();
                    vUp.w = 0.0f;
                    OuterProduct(vForward, vRight, vUp);
                    Normalize(vForward, vForward);
                    OuterProduct(vRight, vUp, vForward);
                    Normalize(vRight, vRight);
                    mMat.v[0] = vRight;
                    mMat.v[1] = vUp;
                    mMat.v[2] = vForward;
                    mMat.v[3] = vTmp[0];
                    CEffectMan::setEffect(m_pszEffName, m_nEffNum, (ObjType::eObjType)11, &mMat, 0.0f);
                }
            } else {
                TRACE_USER_COL((USER)6, (TtyColor)0x1F, "CLegIK::updateFootEffect()  \x91\xab\x8c\xb3\x82\xc9\x82\xa0\x82\xbd\x82\xe8\x82\xaa\x96\xb3\x82\xa2\x81\x49\x81\x49\n");
            }
            m_iStatus &= ~(1 << i);
        }
    }
}
// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .data .rodata
