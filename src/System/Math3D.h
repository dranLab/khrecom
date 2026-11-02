#ifndef SYSTEM_MATH3D_H
#define SYSTEM_MATH3D_H

#include "khtypes.h"
#include "System/Vector.h"

typedef FVECTOR QUATERNION;

struct BOUNDING_SPHERE {
    FVECTOR vCenter;
    FLOAT fR;
};

struct BOUNDING_CYLINDER {
    FVECTOR vQ1;
    FVECTOR vQ2;
    FLOAT fR;
};

struct CFrustum {
private:
    BOOL m_bInit;
    FVECTOR m_vFrustum[6];

public:
    CFrustum() { m_bInit = FALSE; }
    void initialize(FLOAT inE, FLOAT inA, FLOAT inN, FLOAT inF, BOOL inInit);
    BOOL isInit() { return m_bInit; }
    BOOL sphereVisible(const FVECTOR& inCenter, FLOAT inRadius);
    BOOL ellipsoidVisible(const FVECTOR& inCenter, const FVECTOR& inR, const FVECTOR& inS, const FVECTOR& inT);
    BOOL cylinderVisible(const FVECTOR& inP1, const FVECTOR& inP2, FLOAT inRadius);
};

namespace Quaternion {
void QuaternionNormalize(QUATERNION& outQ, const QUATERNION& inQ);
void QuaternionMultiply(QUATERNION& outQ, const QUATERNION& inQ0, const QUATERNION& inQ1);
void QuaternionRotationAxis(QUATERNION& outQuat, const FVECTOR& inAxis, FLOAT inRad);
void QuaternionToMatrix(FMATRIX& outMatrix, const QUATERNION& inQuat);
void QuaternionToMatrix(FMATRIX& outMatrix, const QUATERNION& inQuat, const FVECTOR& inTrs);
void MatrixToQuaternion(QUATERNION& outQuat, const FMATRIX& inMatrix);
void QuaternionLerp(QUATERNION& outResult, const QUATERNION& inQuat0, const QUATERNION& inQuat1, FLOAT inT);
void QuaternionSlerp(QUATERNION& outResult, const QUATERNION& inQuat0, const QUATERNION& inQuat1, FLOAT inT);
void RotationArc(QUATERNION& outQuat, const FVECTOR& inV0, const FVECTOR& inV1);
void MakeQuaternion(QUATERNION& outQuat, FLOAT rx, FLOAT ry, FLOAT rz);
void QuaternionConjugate(QUATERNION& outQuat, const QUATERNION& inQuat);
void QuaternionMulVector(QUATERNION& outQuat, const QUATERNION& inQuat, const FVECTOR& inVect);
}

namespace Math3D {
FLOAT VectorLength(const FVECTOR& inVector);
FLOAT VectorLengthSq(const FVECTOR& inVector);
FLOAT VectorLengthXZ(const FVECTOR& inVector);
FLOAT VectorLengthXZSq(const FVECTOR& inVector);
void VectorLerp(FVECTOR& outResult, const FVECTOR& inP1, const FVECTOR& inP2, FLOAT inT);
FLOAT PointToPointDistance(const FVECTOR& inP0, const FVECTOR& inP1);
FLOAT PointToPointDistanceSq(const FVECTOR& inP0, const FVECTOR& inP1);
FLOAT PointToPointDistanceSqXZ(const FVECTOR& inP0, const FVECTOR& inP1);
void VectorMaximize(FVECTOR& outVec, const FVECTOR& inVec0, const FVECTOR& inVec1);
void VectorMinimize(FVECTOR& outVec, const FVECTOR& inVec0, const FVECTOR& inVec1);
void VectorMaximizeXYZ(FVECTOR& outVec, const FVECTOR& inVec0, const FVECTOR& inVec1);
void VectorMinimizeXYZ(FVECTOR& outVec, const FVECTOR& inVec0, const FVECTOR& inVec1);
void VectorMaxMinXYZ(FVECTOR& inoutMax, FVECTOR& inoutMin, const FVECTOR& inVec);
void VectorMaxMinXYZ(FVECTOR& inoutMax, FVECTOR& inoutMin, const FVECTOR& inMax, const FVECTOR& inMin);
void CalcReflectionVector(FVECTOR& outVec, const FVECTOR& inL, const FVECTOR& inN);
FLOAT PointToPlaneDistance(const FVECTOR& inPoint, const FVECTOR& inPlane);
BOOL PlaneIntersectLine(FVECTOR& outIntersect, const FVECTOR& inPlane, const FVECTOR& inV0, const FVECTOR& inV1);
INT SphereIntersectRay(FVECTOR* outIntersect, const FVECTOR& inSphere, const FVECTOR& inV, const FVECTOR& inQ);
void CalcFrustum(FVECTOR* inFrustum);
BOOL CheckVisibleBoundingSphere(BOUNDING_SPHERE& inBSphere, FVECTOR* inFrustum);
BOOL CheckVisibleBoundingCylinder(BOUNDING_CYLINDER& inBCylinder, FVECTOR* inFrustum);
void HermiteInterpolation(FVECTOR& vecOut, const FMATRIX& matIn, FLOAT fInT);
INT NearVector(const FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void HermiteMatrix(FMATRIX& outMat, const FVECTOR* v0, const FVECTOR* v1, const FVECTOR* v2, const FVECTOR* v3, FLOAT fAlpha);
void HermiteTCBMatrix(FMATRIX& outMat, const FVECTOR* v0, const FVECTOR* v1, const FVECTOR* v2, const FVECTOR* v3, FLOAT fAlpha);
void HermiteVector(FVECTOR& outVec, FLOAT t);
void HermitePosition(FVECTOR& outVec, const FVECTOR* v0, const FVECTOR* v1, const FVECTOR* v2, const FVECTOR* v3, FLOAT t, FLOAT fAlpha);
void HermiteTCBPosition(FVECTOR& outVec, const FVECTOR* v0, const FVECTOR* v1, const FVECTOR* v2, const FVECTOR* v3, FLOAT t, FLOAT fAlpha);
INT checkFloat(FLOAT pai);
FLOAT GetIntersectionTimeOfFrontPlaneAndLine(FVECTOR& vOutHitPos, const FVECTOR& vPlaneNormal, const FVECTOR& vPlanePos, const FVECTOR& vSLine, const FVECTOR& vELine);
FLOAT GetIntersectionTimeOfPlaneAndLine(FVECTOR& vOutHitPos, const FVECTOR& vPlaneNormal, const FVECTOR& vPlanePos, const FVECTOR& vSLine, const FVECTOR& vELine);
BOOL CheckInsidePlane3(const FVECTOR& vHitPos, const FVECTOR& vPlaneNormal, const FVECTOR& vPlanePos0, const FVECTOR& vPlanePos1, const FVECTOR& vPlanePos2);
BOOL CheckInsidePlane4(const FVECTOR& vHitPos, const FVECTOR& vPlaneNormal, const FVECTOR& vPlanePos0, const FVECTOR& vPlanePos1, const FVECTOR& vPlanePos2, const FVECTOR& vPlanePos3);
BOOL CheckInsidePlane4Neighborhood2(const FVECTOR& vHitPos, const FVECTOR& vPlaneNormal, const FVECTOR& vPlanePos0, const FVECTOR& vPlanePos1, const FVECTOR& vPlanePos2, const FVECTOR& vPlanePos3);
BOOL EqualVector(const FVECTOR& vec0, const FVECTOR& vec1);
BOOL EqualVector(const FVECTOR& vec0, const FVECTOR& vec1, FLOAT fParam);
BOOL EqualVector(const FVECTOR& vec0);
BOOL EqualVector(const FVECTOR& vec0, FLOAT fParam);
FLOAT GetDistance2OfLineAndPoint(const FVECTOR& p0, const FVECTOR& p1, const FVECTOR& q);
FLOAT GetDistance2OfLimitedLineAndPoint(const FVECTOR& p0, const FVECTOR& p1, const FVECTOR& q);
FLOAT GetIntersectionOfLineAndPoint(FVECTOR& vOutI, const FVECTOR& p0, const FVECTOR& p1, const FVECTOR& q);
FLOAT LimitPI(FLOAT pai);
void LimitPIVector(FVECTOR& pai);
INT GetMaxVectorElement(const FVECTOR& vNorm);
BOOL HitPointBall(const FVECTOR& p, const FVECTOR& c, FLOAT cr);
BOOL HitBallBall(const FVECTOR& c0, const FVECTOR& c1, FLOAT cr0, FLOAT cr1);
BOOL HitPillarPoint(const FVECTOR& m, const FVECTOR& n, const FVECTOR& p, FLOAT mnr);
BOOL HitInfinityPillarPoint(const FVECTOR& m, const FVECTOR& n, const FVECTOR& p, FLOAT mnr);
BOOL HitLineBall(const FVECTOR& m, const FVECTOR& n, const FVECTOR& c, FLOAT cr);
BOOL HitInfinityLineBall(const FVECTOR& m, const FVECTOR& n, const FVECTOR& c, FLOAT cr);
BOOL HitPillarBall(const FVECTOR& m, const FVECTOR& n, const FVECTOR& c, FLOAT mnr, FLOAT cr);
void InvMatrix(FMATRIX& outMat, const FMATRIX& inMat);
BOOL checkMaxMinOutsideXYZ(const FVECTOR& v, const FVECTOR& vMax, const FVECTOR& vMin);
void MatrixToAngleXYZ(FVECTOR& vOut, const FMATRIX& mIn);
void MatrixToAngleZYX(FVECTOR& outvAngle, const FMATRIX& mMat);
}

extern "C" float __ieee754_sqrtf(float x);

#endif /* SYSTEM_MATH3D_H */
