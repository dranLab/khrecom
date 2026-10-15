// PARTIAL: added for Game/Player/FriendAladdin
#ifndef SYSTEM_MATH3D_H
#define SYSTEM_MATH3D_H

#include "khtypes.h"
#include "System/Vector.h"

namespace Math3D {
FLOAT LimitPI(FLOAT inRad);
void LimitPIVector(FVECTOR& pai);
FLOAT VectorLength(const FVECTOR& inV); // PARTIAL: added for Game/Player/SummonCloud
void VectorLerp(FVECTOR& outResult, const FVECTOR& inP1, const FVECTOR& inP2, FLOAT inT); // PARTIAL: added for Game/Enemy/MapEnemyShadow
}

// PARTIAL: added for Game/Gimmick/GimmickCarry (QUATERNION is an FVECTOR)
namespace Quaternion {
void QuaternionNormalize(FVECTOR& outQ, const FVECTOR& inQ);
void QuaternionRotationAxis(FVECTOR& outQuat, const FVECTOR& inAxis, FLOAT inRad);
void QuaternionToMatrix(FMATRIX& outMatrix, const FVECTOR& inQuat);
void MakeQuaternion(FVECTOR& outQuat, FLOAT inX, FLOAT inY, FLOAT inZ); // PARTIAL: added for System/Graphic/MotData
void QuaternionSlerp(FVECTOR& outQuat, const FVECTOR& inQ1, const FVECTOR& inQ2, FLOAT inT); // PARTIAL: added for System/Graphic/MotData
}

#endif /* SYSTEM_MATH3D_H */
