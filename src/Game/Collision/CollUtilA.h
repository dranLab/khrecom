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

    /* 0x000 */ char _pad000[0x30];
    /* 0x030 */ FVECTOR m_vCkS;
    /* 0x040 */ FLOAT m_fMDVal;
    /* 0x044 */ FLOAT m_Radius;
    /* 0x048 */ INT m_SphereDetectFg;
    /* 0x04c */ INT m_fgCamera;
    /* 0x050 */ FVECTOR m_vCkE;
    /* 0x060 */ char _pad060[0x70];
    /* 0x0d0 */ FVECTOR m_vNormal;
    /* 0x0e0 */ FVECTOR m_vNormal2;
    /* 0x0f0 */ FVECTOR m_vNormalP;
    /* 0x100 */ FVECTOR m_vNormalA;
    /* 0x110 */ FVECTOR m_vDetect;
    /* 0x120 */ FVECTOR m_vDetect2;
    /* 0x130 */ FVECTOR m_vDetectP;
    /* 0x140 */ char _pad140[0x2c];
    /* 0x16c */ FLOAT m_Radius2;
    /* 0x170 */ FLOAT m_RadiusHosei;
    /* 0x174 */ FLOAT m_RadiusPlus;
    /* 0x178 */ INT m_DetectStatus;
    /* 0x17c */ char _pad17c[0xb0];
    /* 0x22c */ FMATRIX* m_pDetectMatP;
    /* 0x230 */ FMATRIX* m_pDetectMatA;
    /* 0x234 */ MC_CHIP* m_pDetectChipA;
    /* 0x238 */ CCollision* m_pCheckColl;
    /* 0x23c */ void* m_pCollDbg;
};

#endif // GAME_COLLISION_COLLUTILA_H
