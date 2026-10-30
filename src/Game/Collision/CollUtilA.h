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
    /* 0x00 */ union {
        INT info;
        unsigned char infoB[4];
    };
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

struct MC_CHIP;
struct PRIM_INFO2 { // 0x40
    FVECTOR vLS;
    FVECTOR vLE;
    FVECTOR vLVec;
    FMATRIX* pMatLW;
    FVECTOR* pNorm;
    FVECTOR* pVert;
    MC_CHIP* pMcChip;
};

struct PRIM_INFO { // 0x10
    INT kind;
    INT fgLine;
    PRIM_INFO2* pInfo2;
    MC_PRIM* pPrim;
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
    /* 0x00 */ union {
        UINT id;
        UINT fgDisable;
    };
    /* 0x04 */ UINT flag;
    /* 0x08 */ UINT idx;
    /* 0x0c */ BYTE* data;
    /* 0x10 */ FVECTOR t;
    /* 0x20 */ FVECTOR r;
    /* 0x30 */ FVECTOR s;
    /* 0x40 */ FMATRIX matLW;
    /* 0x80 */ FMATRIX matWL;
};

struct MCD_INFO {
    MC_DATA* pMCData;     /* 0x00 */
    MC_CHIP* pMCChip;     /* 0x04 */
    FMATRIX* pMatrix;     /* 0x08 */
    FMATRIX* pMatrixIOld; /* 0x0c */
    INT iChipSuu;         /* 0x10 */
    FLOAT fScale;         /* 0x14 */
    FLOAT fMDVal;         /* 0x18 */
    FLOAT fMDVal2;        /* 0x1c */
};

struct OCD_WORK {
    BYTE fgEnable;
};

struct COLL_CUBE {
    FVECTOR vMin;
    FVECTOR vMax;
    FMATRIX* pMatrix;
};

// PARTIAL: all members public (the ref's private ones are called directly where CollUtil.h's static inline
// wrappers stand expanded: its inline strings must not be in those objects); getInstance() for the ref's instance()
class CCollUtil {
public:
    static CCollUtil* m_pInstance;
    static CCollUtil* getInstance();
    CCollUtil();
    ~CCollUtil();

    static CCollUtil& initialize();
    static void finalize();
    static CCollUtil& instance();
    static void init();
    static void clean();
    static void initializeData(void* pDtHead, MCD_INFO* pMCDInfo);
    static INT detectMCD(CCollision& collision);
    static INT detectSpherePoint(class CSphereCollision& sphereColl0, class CPointCollision& pointColl1);
    static INT detectSphereSphere(class CSphereCollision& sphereColl0, class CSphereCollision& sphereColl1);
    static INT detectSpherePoly3(class CSphereCollision& sphereColl0, class CPoly3Collision& poly3Coll1);
    static INT detectColumnPoint(class CColumnCollision& columnColl0, class CPointCollision& pointColl1);
    static INT detectColumnSphere(class CColumnCollision& columnColl0, class CSphereCollision& sphereColl1, INT fg);
    static INT detectColumnPoly3(class CColumnCollision& columnColl0, class CPoly3Collision& poly3Coll1);
    static INT detectColumnColumnEz(class CColumnCollision& columnColl0, class CColumnCollision& columnColl1);
    static INT detectColumnColumn(class CColumnCollision& columnColl0, class CColumnCollision& columnColl1);
    static INT detectColumnColumnR(class CColumnCollision& columnColl0, class CColumnCollision& columnColl1);
    static INT detectSphereCircle(class CSphereCollision& sphereColl0, class CCircleCollision& circleColl1);
    static INT detectBBoxPointEZ(COLL_CUBE& collCube, CCollision& collision);
    static INT detectOCD(CCollision& collision, MCD_INFO* pMCDInfo, OCD_WORK* pOCDWork);
    static INT detectOCD(CCollision& collision, MCD_INFO* pMCDInfo, OCD_WORK* pOCDWork, INT fgLocal);
    static void initDbg();
    static void setCollDbg();
    static PRIM_INFO* getPrimInfo();
    static INT getPrimInfoIdx();
    static INT detectActorTrapXZ(const ETMoveData& moveData, const EventTrapData& trapData);
    static INT checkHangEdge(CCollision& collision, FVECTOR& inDir, FVECTOR& inHeightDepth);
    static INT checkLadderUp(FVECTOR& inPos, FVECTOR& inDir, FVECTOR& inHeightDepth);
    static void getPassingPosition(FVECTOR& outPos, FVECTOR& outNorm);
    static FLOAT getLadderPosition(FVECTOR& outPos, FVECTOR& outNorm);
    static INT getSemiTransMat();
    static INT checkWaterSurface(FVECTOR& inPos);
    static MCD_INFO* getMapMCDInfo();
    static void enableChipCollision(FVECTOR& vChipCenter, BOOL fg);
    static void backupChipStatus();
    static void restoreChipStatus();
    static MC_CHIP* searchChipCollisionFirst(FVECTOR& vChipCenter, FLOAT fRange);
    static MC_CHIP* searchChipCollisionNext();
    static void recalcChipCollision(MC_CHIP* pChip);

    void setup();
    void dispose();
    void _initializeData(void* pDtHead, MCD_INFO* pMCDInfo);
    INT mappingData(MC_DATA* pDtHead);
    void _enableChipCollision(FVECTOR& vChipCenter, BOOL fg);
    void _backupChipStatus();
    void _restoreChipStatus();
    MC_CHIP* _searchChipCollisionFirst(FVECTOR& vChipCenter, FLOAT fRange);
    MC_CHIP* _searchChipCollisionNext();
    INT _detectMCD(CCollision& collision);
    INT _detectSpherePoint(class CSphereCollision& sphereColl0, class CPointCollision& pointColl1);
    INT _detectSphereSphere(class CSphereCollision& sphereColl0, class CSphereCollision& sphereColl1);
    INT _detectSpherePoly3(class CSphereCollision& sphereColl0, class CPoly3Collision& poly3Coll1);
    INT _detectColumnPoint(class CColumnCollision& columnColl0, class CPointCollision& pointColl1);
    INT _detectColumnSphere(class CColumnCollision& columnColl0, class CSphereCollision& sphereColl1, INT fg);
    INT _detectColumnPoly3(class CColumnCollision& columnColl0, class CPoly3Collision& poly3Coll1);
    INT _detectColumnColumnEz(class CColumnCollision& columnColl0, class CColumnCollision& columnColl1);
    INT _detectColumnColumn(class CColumnCollision& columnColl0, class CColumnCollision& columnColl1);
    INT _detectColumnColumnR(class CColumnCollision& columnColl0, class CColumnCollision& columnColl1);
    INT _detectSphereCircle(class CSphereCollision& sphereColl0, class CCircleCollision& circleColl1);
    INT _detectBBoxPointEZ(COLL_CUBE& collCube, CCollision& collision);
    INT _checkHangEdge(CCollision& collision, FVECTOR& inDir, FVECTOR& inHeightDepth);
    INT _checkLadderUp(FVECTOR& inPos, FVECTOR& inDir, FVECTOR& inHeightDepth);
    void _initDbg();
    INT _detectOCD(CCollision& collision, MCD_INFO* pMCDInfo, OCD_WORK* pOCDWork, INT fgLocal);
    void _getPassingPosition(FVECTOR& outPos, FVECTOR& outNorm);
    FLOAT _getLadderPosition(FVECTOR& outPos, FVECTOR& outNorm);
    INT _checkWaterSurface(FVECTOR& inPos);
    INT _detectActorTrapXZ(const ETMoveData& moveData, const EventTrapData& trapData);
    INT detect2DLineLine(const FVECTOR& vP0, const FVECTOR& vP1);
    INT detect2DLinePoint(const FVECTOR& vP0, const FVECTOR& vP1, const FVECTOR& vPC);
    FLOAT distancePlane();
    FLOAT distancePoint();
    FLOAT innerProduct();
    void outerProduct();
    void vectorNormalize0();
    INT vectorNormalize();
    INT vectorNormalizeXZ();
    FLOAT closestPointOnLine();
    INT slidingHosei(FVECTOR& vCenter, FVECTOR& vSurface, FVECTOR& vNormal);
    INT detectMC();
    INT detectPolygon();
    INT distanceSphere(FVECTOR* pP0);
    INT distanceSphereO(FVECTOR* pP0);
    INT distanceLine(FVECTOR* pP0, FVECTOR* pP1);
    INT distanceLineO(FVECTOR* pP0, FVECTOR* pP1, INT fg);
    INT detectPLine();
    INT detectPSphere();
    INT __detectMCInitInit(MC_CHIP* pChip);
    void detectMCInit(MC_CHIP* pChip, INT iArea);
    INT detectMap(FVECTOR* pWS, FVECTOR* pWE);

    /* 0x000 */ MCD_INFO m_MCDInfo;
    /* 0x020 */ MCD_INFO* m_pMCDInfo;
    /* 0x024 */ MCD_INFO* m_pOCDInfo;
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
    /* 0x1a0 */ FMATRIX* m_pSemiTransMat[32];
    /* 0x220 */ INT m_SemiTransMatIdx;
#ifdef COLLUTILA_H_INFO_MEMBERS /* MATCH: COLL_INFO_UNION has no header yet (ref owner Game/Movement); CollUtil, CollUtilF, CollMapCamera, MapEnemy, SEMotion, Player and Movement define their own copy */
    /* 0x224 */ COLL_INFO_UNION m_iInfo;
    /* 0x228 */ COLL_INFO_UNION m_iInfo2;
#else
    /* 0x224 */ char _pad224[0x8];
#endif
    /* 0x22c */ FMATRIX* m_pDetectMatP;
    /* 0x230 */ FMATRIX* m_pDetectMatA;
    /* 0x234 */ MC_CHIP* m_pDetectChipA;
    /* 0x238 */ CCollision* m_pCheckColl;
    /* 0x23c */ void* m_pCollDbg;
};

#endif // GAME_COLLISION_COLLUTILA_H
