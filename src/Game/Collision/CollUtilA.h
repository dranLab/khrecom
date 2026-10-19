#ifndef GAME_COLLISION_COLLUTILA_H
#define GAME_COLLISION_COLLUTILA_H

#include "khtypes.h"
#include "System/Vector.h"

struct ETMoveData {
    /* 0x00 */ FVECTOR _vOldPos;
    /* 0x10 */ FVECTOR _vNowPos;
    /* 0x20 */ FLOAT _fRadius;
};

struct EventTrapData {
    UINT _nType;
    FVECTOR* _pVert;
};

class CCollision;

struct MC_PRIM {
    /* 0x00 */ INT info;
    /* 0x04 */ SHORT n0;
    /* 0x06 */ SHORT n1;
    /* 0x08 */ SHORT n2;
    /* 0x0a */ SHORT n3;
    /* 0x0c */ SHORT n4;
    /* 0x0e */ SHORT v0;
    /* 0x10 */ SHORT v1;
    /* 0x12 */ SHORT v2;
    /* 0x14 */ SHORT v3;
    /* 0x16 */ SHORT kind;
};

struct MC_DATA {
    /* 0x00 */ FLOAT x_min;
    /* 0x04 */ FLOAT y_min;
    /* 0x08 */ FLOAT z_min;
    /* 0x0c */ USHORT id;
    /* 0x0e */ USHORT div;
    /* 0x10 */ FLOAT x_w;
    /* 0x14 */ FLOAT y_w;
    /* 0x18 */ FLOAT z_w;
    /* 0x1c */ SHORT xw;
    /* 0x1e */ SHORT yw;
    /* 0x20 */ USHORT (*tbl)[2];
    /* 0x24 */ FVECTOR* norm;
    /* 0x28 */ FVECTOR* vert;
    /* 0x2c */ FLOAT* d;
    /* 0x30 */ MC_PRIM* prim3;
    /* 0x34 */ MC_PRIM* prim4;
    /* 0x38 */ USHORT* prim;
    /* 0x3c */ UINT total;
};

struct MC_CHIP {
    /* 0x00 */ UINT id;
    /* 0x04 */ UINT flag;
    /* 0x08 */ UINT idx;
    /* 0x0c */ BYTE* data;
    /* 0x10 */ FVECTOR t;
    /* 0x20 */ FVECTOR r;
    /* 0x30 */ FVECTOR s;
    /* 0x40 */ FMATRIX matLW;
    /* 0x80 */ FMATRIX matWL;
};

// PARTIAL: CCollUtil members used by CollUtilA/CollUtilF (CollUtil.h's inline strings must not be in these objects)
class CCollUtil {
public:
    INT _detectMCD(CCollision& collision);
    INT _checkHangEdge(CCollision& collision, FVECTOR& inDir, FVECTOR& inHeightDepth);
    INT _checkLadderUp(FVECTOR& inPos, FVECTOR& inDir, FVECTOR& inHeightDepth);
    void _getPassingPosition(FVECTOR& outPos, FVECTOR& outNorm);
    FLOAT _getLadderPosition(FVECTOR& outPos, FVECTOR& outNorm);
    INT _checkWaterSurface(FVECTOR& inPos);
    INT _detectActorTrapXZ(const ETMoveData& moveData, const EventTrapData& trapData);
    INT detect2DLineLine(const FVECTOR& vP0, const FVECTOR& vP1);
    INT detect2DLinePoint(const FVECTOR& vP0, const FVECTOR& vP1, const FVECTOR& vPC);
    // PARTIAL: added for Game/Atel/EventTrap (defined inline in CollUtil.h)
    static CCollUtil* m_pInstance;
    static CCollUtil* getInstance();
    // PARTIAL: added for Game/Collision/CollMapCamera (defined inline in CollUtil.h)
    INT _detectOCD(CCollision& collision, struct MCD_INFO* pMCDInfo, struct OCD_WORK* pOCDWork, INT inFlag);
    static INT detectMCD(CCollision& collision);
    // PARTIAL: added for Game/Collision/CollBody
    INT _detectColumnColumnEz(class CColumnCollision& coll0, class CColumnCollision& coll1);
    INT _detectColumnColumn(class CColumnCollision& coll0, class CColumnCollision& coll1);
    static INT detectColumnColumnEz(class CColumnCollision& coll0, class CColumnCollision& coll1);
    static INT detectColumnColumn(class CColumnCollision& coll0, class CColumnCollision& coll1);
    static INT detectOCD(CCollision& collision, struct MCD_INFO* pMCDInfo, struct OCD_WORK* pOCDWork);
    // PARTIAL: added for Game/Collision/CollMap
    static INT detectOCD(CCollision& collision, struct MCD_INFO* pMCDInfo, struct OCD_WORK* pOCDWork, INT fgLocal);
    static INT checkWaterSurface(FVECTOR& inPos);
    // PARTIAL: added for Game/Collision/Ride
    void _initializeData(void* pData, struct MCD_INFO* pMCDInfo);
    static void initializeData(void* pData, struct MCD_INFO* pMCDInfo);
    INT _detectBBoxPointEZ(struct COLL_CUBE& cube, CCollision& collision);
    // PARTIAL: added for Game/Collision/CollAttack
    INT _detectSpherePoint(class CSphereCollision& coll0, class CPointCollision& coll1);
    INT _detectSphereSphere(class CSphereCollision& coll0, class CSphereCollision& coll1);
    INT _detectSpherePoly3(class CSphereCollision& coll0, class CPoly3Collision& coll1);
    INT _detectSphereCircle(class CSphereCollision& coll0, class CCircleCollision& coll1);
    INT _detectColumnPoint(class CColumnCollision& coll0, class CPointCollision& coll1);
    INT _detectColumnSphere(class CColumnCollision& coll0, class CSphereCollision& coll1, INT inFlag);
    INT _detectColumnPoly3(class CColumnCollision& coll0, class CPoly3Collision& coll1);
    static INT detectSpherePoint(class CSphereCollision& coll0, class CPointCollision& coll1);
    static INT detectSphereSphere(class CSphereCollision& coll0, class CSphereCollision& coll1);
    static INT detectSpherePoly3(class CSphereCollision& coll0, class CPoly3Collision& coll1);
    static INT detectSphereCircle(class CSphereCollision& coll0, class CCircleCollision& coll1);
    static INT detectColumnPoint(class CColumnCollision& coll0, class CPointCollision& coll1);
    static INT detectColumnSphere(class CColumnCollision& coll0, class CSphereCollision& coll1, INT inFlag);
    static INT detectColumnPoly3(class CColumnCollision& coll0, class CPoly3Collision& coll1);
    // PARTIAL: added for Game/Collision/CollUtilO
    INT distanceLineO(FVECTOR* pP0, FVECTOR* pP1, INT fg);
    INT distanceSphereO(FVECTOR* pP0);
    INT _detectColumnColumnR(class CColumnCollision& coll0, class CColumnCollision& coll1);

    /* 0x000 */ char _pad000[0x30];
    /* 0x030 */ FVECTOR m_vCkS;
    /* 0x040 */ FLOAT m_fMDVal;
    /* 0x044 */ FLOAT m_Radius;
    /* 0x048 */ INT m_SphereDetectFg;
    /* 0x04c */ INT m_fgCamera;
    /* 0x050 */ FVECTOR m_vCkE;
    /* 0x060 */ FVECTOR m_vCkS0;
    /* 0x070 */ FVECTOR m_vCkE0;
    /* 0x080 */ FVECTOR m_vCkS00;
    /* 0x090 */ FVECTOR m_vCk0;
    /* 0x0a0 */ FVECTOR m_vCk1;
    /* 0x0b0 */ FVECTOR m_vCk2;
    /* 0x0c0 */ FVECTOR m_vCkVec;
    /* 0x0d0 */ FVECTOR m_vNormal;
    /* 0x0e0 */ FVECTOR m_vNormal2;
    /* 0x0f0 */ FVECTOR m_vNormalP;
    /* 0x100 */ FVECTOR m_vNormalA;
    /* 0x110 */ FVECTOR m_vDetect;
    /* 0x120 */ FVECTOR m_vDetect2;
    /* 0x130 */ FVECTOR m_vDetectP;
    /* 0x140 */ FVECTOR m_vChipSearchPos;
    /* 0x150 */ FLOAT m_CkVecl;
    /* 0x154 */ FLOAT m_CkVecl2;
    /* 0x158 */ INT m_fgSliding;
    /* 0x15c */ INT m_iKind;
    /* 0x160 */ FLOAT m_RadiusL;
    /* 0x164 */ FLOAT m_Radius0;
    /* 0x168 */ FLOAT m_Radius1;
    /* 0x16c */ FLOAT m_Radius2;
    /* 0x170 */ FLOAT m_RadiusHosei;
    /* 0x174 */ FLOAT m_RadiusPlus;
    /* 0x178 */ INT m_DetectStatus;
    /* 0x17c */ FLOAT m_ShiftY;
    /* 0x180 */ INT m_iChipSearchIndex;
    /* 0x184 */ UINT* m_pChipStatus;
    /* 0x188 */ struct PRIM_INFO* m_pPrimInfo;
    /* 0x18c */ INT m_PrimInfoIdx;
    /* 0x190 */ struct PRIM_INFO2* m_pPrimInfo2;
    /* 0x194 */ INT m_PrimInfoIdx2;
    /* 0x198 */ INT m_PrimInfoMax;
    /* 0x19c */ struct PRIM_INFO* m_pPrimInfoCurrent;
    /* 0x1a0 */ FMATRIX* m_pSemiTransMat[32]; // PARTIAL: split out for Game/Collision/CollMapCamera
    /* 0x220 */ INT m_SemiTransMatIdx;
    /* 0x224 */ char _pad224[0x8];
    /* 0x22c */ FMATRIX* m_pDetectMatP;
    /* 0x230 */ FMATRIX* m_pDetectMatA;
    /* 0x234 */ MC_CHIP* m_pDetectChipA;
    /* 0x238 */ CCollision* m_pCheckColl;
    /* 0x23c */ void* m_pCollDbg;
};

#endif // GAME_COLLISION_COLLUTILA_H
