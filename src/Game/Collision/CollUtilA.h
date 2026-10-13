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

// PARTIAL: CCollUtil members used by CollUtilA (CollUtil.h's inline strings must not be in this object)
class CCollUtil {
public:
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
    /* 0x0e0 */ char _pad0e0[0x8c];
    /* 0x16c */ FLOAT m_Radius2;
    /* 0x170 */ FLOAT m_RadiusHosei;
    /* 0x174 */ FLOAT m_RadiusPlus;
    /* 0x178 */ INT m_DetectStatus;
    /* 0x17c */ char _pad17c[0xc4];
};

#endif // GAME_COLLISION_COLLUTILA_H
