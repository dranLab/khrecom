#include "Game/Collision/CollUtilA.h"
#define COLLUTIL_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollUtil.h" /* MATCH: the original's path in CollUtil.h's strings */
#include "Game/Collision/CollUtil.h"
#include "System/HeapMan.h"
#include "System/Task.h"
#include "System/TaskMan.h"
#include "System/Graphic/Camera.h"
#include "System/Math3D.h"
#include "System/Debug.h"
#include "Game/Collision/Ride.h"

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

/* iterates a CSJointListT, the current entry may be removed (original macro name unknown) */
#define SJ_FOREACH(T, pEntry, list) \
    for (T *pEntry = (list).getHead(), *__pn = pEntry ? pEntry->getNext() : NULL; pEntry; \
         pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL)

extern "C" void sceVu0InversMatrix(FMATRIX& m0, const FMATRIX& m1);
extern "C" void sceVu0RotMatrixX(FMATRIX& m0, FMATRIX& m1, float rx);
extern "C" void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
extern "C" void sceVu0RotMatrixZ(FMATRIX& m0, FMATRIX& m1, float rz);
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT r);
FLOAT InnerProduct(const FVECTOR& v0, const FVECTOR& v1);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);


static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}

static inline void _unitMatrixVu0(FMATRIX& m) {
    asm volatile("vmr32.xyzw $vf1, $vf0\n"
                 "sqc2 $vf0, 0x30(%0)\n"
                 "vmr32.xyzw $vf2, $vf1\n"
                 "sqc2 $vf1, 0x20(%0)\n"
                 "vmr32.xyzw $vf3, $vf2\n"
                 "sqc2 $vf2, 0x10(%0)\n"
                 "sqc2 $vf3, 0x0(%0)\n"
                 :
                 : "r"(&m)
                 : "memory");
}

#line 20
CRide::CRide() {
    m_pRiderList = new (__FILE__ "(23)") CRiderList;
    m_fgOnLook = 0;
}

CRide::~CRide() {
    if (m_pRiderList) delete m_pRiderList;
}

void CRide::addRiderList(CRider& Rider) {
    m_pRiderList->pushBack(Rider);
}

void CRide::removeRiderList(CRider& Rider) {
    m_pRiderList->remove(Rider);
}

INT CRide::getRiderListSize() {
    return m_pRiderList->getListSize();
}

BOOL CRide::isRideOn(COLL_KIND inCheckKind) {
    SJ_FOREACH(CRider, pRider, *m_pRiderList) {
        if (pRider->m_Kind == inCheckKind && !pRider->isHang()) return TRUE;
    }
    return FALSE;
}

BOOL CRide::isHangOn(COLL_KIND inCheckKind) {
    SJ_FOREACH(CRider, pRider, *m_pRiderList) {
        if (pRider->m_Kind == inCheckKind && pRider->isHang()) return TRUE;
    }
    return FALSE;
}

CRideModel::CRideModel() {
    m_fgAdjust = 3;
    m_pMatIOld = NULL;
    m_pOCDWork = NULL;
    m_fgMove = 0;
}

CRideModel::~CRideModel() {
    if (m_pMatIOld) com_free(m_pMatIOld);
    m_pMatIOld = NULL;
    if (m_pOCDWork) com_free(m_pOCDWork);
    m_pOCDWork = NULL;
}

#line 109
void CRideModel::init(void* pOCDData, CTask* pBFObj) {
    FMATRIX* lMat = NULL;
    if (pBFObj) lMat = &pBFObj->Crd(0).m_World;
    if (!lMat) return;
    m_MCDInfo.pMatrix = lMat;
    if (m_pMatIOld) com_free(m_pMatIOld);
    m_pMatIOld = NULL;
    if (m_pOCDWork) com_free(m_pOCDWork);
    m_pOCDWork = NULL;
    CCollUtil::initializeData(pOCDData, &m_MCDInfo);
#line 124
    m_pMatIOld = (FMATRIX*)com_malloc_d((u_long)(UINT)(m_MCDInfo.iChipSuu + 1) * sizeof(FMATRIX), __FILE__ "(124)", RET_ADDR());
    ASSERT(m_pMatIOld);
    sceVu0InversMatrix(*m_pMatIOld, *lMat);
    m_fBSRadius2 = 0;
    m_MCDInfo.pMatrixIOld = m_pMatIOld;
    if (m_MCDInfo.iChipSuu > 0) {
#line 131
        m_pOCDWork = (OCD_WORK*)com_malloc_d(m_MCDInfo.iChipSuu, __FILE__ "(131)", RET_ADDR());
        FMATRIX* pPartsMat = m_pMatIOld + 1;
        MC_CHIP* pChip = m_MCDInfo.pMCChip;
        for (INT i = 0; i < m_MCDInfo.iChipSuu; i++, pPartsMat++, pChip++) {
            m_pOCDWork[i].fgEnable = 0;
            _unitMatrixVu0(*pPartsMat);
            sceVu0RotMatrixX(*pPartsMat, *pPartsMat, pChip->r.x);
            sceVu0RotMatrixY(*pPartsMat, *pPartsMat, pChip->r.y);
            sceVu0RotMatrixZ(*pPartsMat, *pPartsMat, pChip->r.z);
            pPartsMat->setTrans(pChip->t);
            pPartsMat->v[3].w = 1.0f;
            MC_DATA* pMCData = (MC_DATA*)pChip->data;
            FVECTOR vMin = *(FVECTOR*)&pMCData->x_min;
            FVECTOR vMax = *(FVECTOR*)&pMCData->x_w;
            AddVector(vMax, vMin, vMax);
            AddVector(vMin, vMin, pChip->t);
            AddVector(vMax, vMax, pChip->t);
            ScaleVector(vMin, vMin, m_MCDInfo.fMDVal);
            ScaleVector(vMax, vMax, m_MCDInfo.fMDVal);
            FLOAT fMin = _sqrtf(InnerProduct(vMin, vMin));
            FLOAT fMax = _sqrtf(InnerProduct(vMax, vMax));
            if (fMax < fMin) fMax = fMin;
            fMax += 2.0f;
            if (m_fBSRadius2 < fMax) m_fBSRadius2 = fMax;
            TRACE("CRideModel : BoundingSphere\x94\xbc\x8c" "a %f\n", fMax);
        }
        m_pOCDWork[0].fgEnable = 1;
        TRACE("CRideModel : BoundingSphere\x94\xbc\x8c" "a %f\x82\xc9\x90\xdd\x92\xe8\n", m_fBSRadius2);
        m_fBSRadius2 = m_fBSRadius2 * m_fBSRadius2;
    }
    if (m_MCDInfo.iChipSuu >= 2) {
        TRACE("CRideModel : \x93\x96\x82\xe8\x94\xbb\x92\xe8\x83" "f\x81[\x83^\x82\xaa %d \x8c\xc2 \x91\xb6\x8d\xdd\x82\xb5\x82\xdc\x82\xb7\x81" "B\n", m_MCDInfo.iChipSuu);
    }
    CCollObj::init(COLL_KIND_OBJECT, COLL_TYPE_MODEL, pBFObj->getHandle(), 0);
}

#line 184
void CRideModel::setAttachMatrix(const FMATRIX* inMat) {
    ASSERT(inMat&&m_pMatIOld);
    m_MCDInfo.pMatrix = (FMATRIX*)inMat;
    sceVu0InversMatrix(*m_pMatIOld, *inMat);
}

FMATRIX* CRideModel::getAttachMatrix() {
    if (!m_pMatIOld) return NULL;
    return m_MCDInfo.pMatrix;
}

#line 201
void CRideModel::setChipLocalMatrix(INT inIdx, const FMATRIX* inMat) {
    ASSERT(m_pMatIOld);
    ASSERT((inIdx>=0)&&(m_MCDInfo.iChipSuu > inIdx));
    FMATRIX* pMat = &m_pMatIOld[inIdx + 1];
    *pMat = *inMat;
}

#line 211
FMATRIX* CRideModel::getChipLocalMatrix(INT inIdx) {
    ASSERT(m_pMatIOld);
    ASSERT((inIdx>=0)&&(m_MCDInfo.iChipSuu > inIdx));
    return &m_pMatIOld[inIdx + 1];
}

#line 219
void CRideModel::enableChipCollision(INT inIdx, BOOL inSw) {
    ASSERT((m_pOCDWork)&&(inIdx>=0)&&(m_MCDInfo.iChipSuu>inIdx));
    m_pOCDWork[inIdx].fgEnable = (inSw != 0);
}

#line 227
void CRideModel::setOldMatrix() {
    ASSERT((m_pMatIOld)&&(m_MCDInfo.pMatrix));
    sceVu0InversMatrix(*m_pMatIOld, *m_MCDInfo.pMatrix);
}

void CRideModel::hoseiRiders(FVECTOR& diffRot) {
    FVECTOR lPos;
    FVECTOR oldPos;
    FMATRIX* pIOld = m_pMatIOld;
    FMATRIX* pNew = m_MCDInfo.pMatrix;
    FMATRIX invMat;
    INT fgAdjustTrans;
    INT fgAdjustRot;
    sceVu0InversMatrix(invMat, *pNew);
    if (*pIOld == invMat) {
        m_fgMove = 0;
        return;
    }
    m_fgMove = 1;
    fgAdjustTrans = m_fgAdjust & 1;
    fgAdjustRot = m_fgAdjust & 2;
    SJ_FOREACH(CRider, pRider, *m_pRiderList) {
        if (fgAdjustTrans) {
            oldPos = pRider->m_Position;
            pRider->m_Position.y += pRider->m_shiftY;
            ApplyMatrix(lPos, *pIOld, pRider->m_Position);
            ApplyMatrix(pRider->m_Position, *pNew, lPos);
            pRider->m_Position.y -= pRider->m_shiftY;
        }
        if (fgAdjustRot) {
            AddVectorXYZ(pRider->m_Rotate, pRider->m_Rotate, diffRot);
            Math3D::LimitPIVector(pRider->m_Rotate);
        }
        if (pRider->getHandle()) {
            CTask* pTask = CTaskMan::getTask(pRider->getHandle());
            pTask->dontSkipMainOnce();
        }
    }
}

void CRideModel::changeListR2() {
    if (m_pCollListR->remove(*this)) m_pCollListR2->pushBack(*this);
}

void CRideModel::changeListR() {
    if (m_pCollListR2->remove(*this)) m_pCollListR->pushBack(*this);
}

void CRideModel::changeListRC() {
    if (m_pCollListR->remove(*this)) m_pCollListRC->pushBack(*this);
}

CRider::CRider(HANDLE hTask, FLOAT shiftY, FLOAT radius, COLL_KIND kind, BOOL fgHang) {
    m_hTask = hTask;
    m_shiftY = shiftY;
    m_Rotate._q = 0;
    m_Radius = radius;
    m_Position._q = 0;
    m_Kind = kind;
    m_fgHang = fgHang;
}

void CRideEtc::init() {
    CCollObj::init(COLL_KIND_OBJECT, COLL_TYPE_ETC, 0, 0);
}

INT CRideEtc::detectEtc(CCollision& collision) {
    COLL_CUBE cube;
    return CCollUtil::instance()._detectBBoxPointEZ(cube, collision) != 0;
}

void CRideEtc::hoseiRiders() {
    SJ_FOREACH(CRider, pRider, *m_pRiderList) {
    }
}
// SOURCE DATA: .rodata
