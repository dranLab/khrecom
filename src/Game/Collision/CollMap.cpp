#define COLLUTIL_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollUtil.h" /* MATCH: this unit names CollUtil.h so (header default "Game/Collision/CollUtil.h") */

#include "System/Task.h"
#include "System/TaskMan.h"
#include "Game/Collision/CollUtilA.h"
#include "Game/Collision/CollUtil.h"
#include "Game/Collision/CollMap.h"
#include "Game/Collision/Ride.h"
#include "System/Debug.h"
#include "include_asm.h"




/* iterates a CSJointListT, the current entry may be removed (original macro name unknown) */
#define SJ_FOREACH(T, pEntry, list) \
    for (T *pEntry = (list).getHead(), *__pn = pEntry ? pEntry->getNext() : NULL; pEntry; \
         pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL)

#include "System/Coord.h"
#include "System/HeapMan.h"

INT CCollMap::m_fgSameFrame = 0;
BOOL CCollMap::m_bExceptBSCheck = 0;

CCollMap::CCollMap() {
    m_pCollision = NULL;
    m_Enable = 0;
    m_pLastRide = NULL;
    m_pNowRide = NULL;
    m_pNowHang = NULL;
    m_pRider = NULL;
    m_pHanger = NULL;
}

CCollMap::~CCollMap() {
    removeRide();
    removeHang();
    if (m_pCollision) delete m_pCollision;
    m_pCollision = NULL;
}

void CCollMap::init(COLL_KIND collKind, COLL_TYPE collType, INT fgStatus, HANDLE hTask) {
    if (getType() != collType) {
        if (m_pCollision) {
            delete m_pCollision;
            m_pCollision = NULL;
        }
        switch (collType) {
        case COLL_TYPE_POINT:
#line 55
            m_pCollision = new (__FILE__ "(55)") CPointCollision;
            break;
        case COLL_TYPE_SPHERE:
#line 58
            m_pCollision = new (__FILE__ "(58)") CSphereCollision;
            break;
        default:
            return;
        }
        setType(collType);
    }
    setKind(collKind);
    m_pCollision->m_fgStatus = fgStatus;
    m_hTask = hTask;
    on();
}

INT CCollMap::collideMap() {
    if (m_Enable && m_pCollision) return CCollUtil::detectMCD(*m_pCollision);
    return 0;
}

INT CCollMap::collideRideForCheck(BOOL fgExceptNowRide) {
    m_bExceptBSCheck = 1;
    INT ret = collideRide(fgExceptNowRide);
    m_bExceptBSCheck = 0;
    return ret;
}

/* Parked: 3 of 259 lines (2026-11-08): the store `pRideAtari = NULL` (sw zero,0x90(sp)) is issued first in the original, before the coll constructor's stores, last in ours; tried: see below */
#if 0
INT CCollMap::collideRide(BOOL fgExceptNowRide) {
    INT kind;
    CCollisionList* pList;
    INT rideType;
    INT ret0;
    CRide* pRideAtari;
    if (!m_Enable) return 0;
    if (!m_pCollision) return 0;
    kind = getKind() & 0xe;
    if (kind != (COLL_KIND_SHADOW & 0xe)) {
        TIMEQ nTimeq = CMainApp::getTickCount();
        if (m_nTimeq != nTimeq) {
            m_fgSameFrame = 0;
            m_nTimeq = nTimeq;
        } else {
            m_fgSameFrame = 1;
        }
    } else {
        m_fgSameFrame = 1;
    }
    pList = m_pCollListR2;
    rideType = getType() << 3;
    pRideAtari = NULL;
    CCollision coll;
    INT inLocal;
    FVECTOR* pTargetPos;
    FVECTOR vTmp;
    m_pCollision->m_outInfoO.info = 0;
    m_pCollision->m_outNormal.y = -1.0f;
    if (pList->getListSize()) {
        ret0 = collideRideR2(*m_pCollision, &pRideAtari, fgExceptNowRide);
        if (ret0) {
            if (pRideAtari->getType() == COLL_TYPE_MODELW) {
                m_pCollision->m_outNormal.y = 0;
                Normalize(m_pCollision->m_outNormal, m_pCollision->m_outNormal);
            } else {
                m_pLastRide = pRideAtari;
            }
            return ret0;
        }
    }
    pTargetPos = getStartPos();
    pList = m_pCollListR;
    ret0 = 0;
    SJ_FOREACH(CCollObj, pEntry, *pList) {
        if (!pEntry->isEnable()) continue;
        if (pEntry->getHandle() == getHandle()) continue;
        if (m_pNowHang == pEntry) continue;
        if (fgExceptNowRide && m_pNowRide == pEntry) continue;
        INT type = pEntry->getType();
        switch (type | rideType) {
        case COLL_TYPE_MODELW | (COLL_TYPE_POINT << 3):
        case COLL_TYPE_MODELW | (COLL_TYPE_SPHERE << 3):
            if (kind == (COLL_KIND_SHADOW & 0xe)) continue;
        case COLL_TYPE_MODEL | (COLL_TYPE_POINT << 3):
        case COLL_TYPE_MODEL | (COLL_TYPE_SPHERE << 3):
            if ((UINT)pEntry->getKind() <= (UINT)COLL_KIND_SHADOW + 3 && !m_bExceptBSCheck) {
                SubVector(vTmp, *pTargetPos, ((CRideModel*)pEntry)->m_MCDInfo.pMatrix->v[3]);
                if (InnerProduct(vTmp, vTmp) > ((CRideModel*)pEntry)->m_fBSRadius2) continue;
            }
            if (((CRideModel*)pEntry)->m_fgMove) inLocal = (m_pNowRide == pEntry);
            else inLocal = 1;
            ret0 = CCollUtil::detectOCD(*m_pCollision, &((CRideModel*)pEntry)->m_MCDInfo,
                                        ((CRideModel*)pEntry)->m_pOCDWork, inLocal);
            break;
        case COLL_TYPE_ETC | (COLL_TYPE_POINT << 3):
        case COLL_TYPE_ETC | (COLL_TYPE_SPHERE << 3):
            ret0 = pEntry->detectEtc(*getCollision());
            break;
        }
        if (ret0) {
            if (pEntry->getType() == COLL_TYPE_MODELW) {
                m_pCollision->m_outNormal.y = 0;
                Normalize(m_pCollision->m_outNormal, m_pCollision->m_outNormal);
            } else {
                m_pLastRide = (CRide*)pEntry;
            }
            return ret0;
        }
    }
    return 0;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollMap", collideRide__8CCollMapi);
/* placeholder for collideRide's local CCollision: a reference to its destructor from code emitted here makes gcc
 * output the (weak) destructor first, as in the original. The build strips .sdata (-G0 code has none). */
__attribute__((section(".sdata"))) void _collideRideLocals() { CCollision coll; }
/* collideRide's jump table (labels of the included assembly) */
__asm__(
    ".rdata\n"
    ".align 3\n"
    ".word .L00225960\n"
    ".word .L00225A34\n"
    ".word .L00225A50\n"
    ".word .L00225A50\n"
    ".word .L00225954\n"
    ".word .L00225A50\n"
    ".word .L00225A50\n"
    ".word .L00225A50\n"
    ".word .L00225A50\n"
    ".word .L00225A50\n"
    ".word .L00225A50\n"
    ".word .L00225A50\n"
    ".word .L00225A50\n"
    ".word .L00225A50\n"
    ".word .L00225A50\n"
    ".word .L00225A50\n"
    ".word .L00225960\n"
    ".word .L00225A34\n"
    ".word .L00225A50\n"
    ".word .L00225A50\n"
    ".word .L00225954\n"
    ".text\n");
#endif

INT CCollMap::collideRideR2(CCollision& coll, CRide** ppRideAtari, BOOL fgExceptNowRide) {
    INT ret0 = 0;
    INT ret1;
    INT inLocal;
    getKind(); /* MATCH: the original calls getKind() on this here and drops the result: its .mdebug has a second CCollObj inline block on this before the loop and the m_pCollision load carries getKind's line (CollObj.h:62); the call orders the list-head load before the pTargetPos spill */
    FVECTOR normalVec = coll.m_outNormal;
    FVECTOR* pTargetPos = getStartPos();
    FVECTOR vTmp;
    CCollisionList* pList = m_pCollListR2;
    SJ_FOREACH(CCollObj, pEntry, *pList) {
        if (!pEntry->isEnable()) continue;
        if (pEntry->getHandle() == getHandle()) continue;
        if (m_pNowHang == pEntry) continue;
        if (fgExceptNowRide && m_pNowRide == pEntry) continue;
        if ((UINT)pEntry->getKind() <= (UINT)COLL_KIND_SHADOW + 3 && !m_bExceptBSCheck) {
            SubVector(vTmp, *pTargetPos, ((CRideModel*)pEntry)->m_MCDInfo.pMatrix->v[3]);
            if (InnerProduct(vTmp, vTmp) > ((CRideModel*)pEntry)->m_fBSRadius2) continue;
        }
        if (((CRideModel*)pEntry)->m_fgMove && !m_fgSameFrame) inLocal = (m_pNowRide == pEntry);
        else inLocal = 1;
        ret1 = CCollUtil::detectOCD(coll, &((CRideModel*)pEntry)->m_MCDInfo, ((CRideModel*)pEntry)->m_pOCDWork, inLocal);
        if (ret1) {
            if (coll.m_outNormal.y >= normalVec.y) {
                normalVec = coll.m_outNormal;
                *ppRideAtari = (CRide*)pEntry;
            } else {
                coll.m_outNormal = normalVec;
            }
            coll.m_inWE = coll.m_outWE;
            if (m_pNowHang) {
                ret0 = ret1;
                break;
            }
        }
        ret0 |= ret1;
    }
    return ret0;
}

void CCollMap::registerLastRide(HANDLE hTask) {
    if (m_pRider) {
        if (m_pNowRide == m_pLastRide) return;
        removeRide();
    }
#line 298
    m_pRider = new (__FILE__ "(298)")
        CRider(hTask, getShiftY(), getRadius(), (COLL_KIND)getKind(), FALSE);
    m_pLastRide->addRiderList(*m_pRider);
    m_pNowRide = m_pLastRide;
    CTaskMan::getTask(m_pNowRide->getHandle())->setPriority((TASK_PRIORITY)0x8000);
}

void CCollMap::removeRide() {
    if (m_pNowRide && m_pRider) {
        CCollisionList* pList = m_pCollListR;
        INT fgExist = 0;
        SJ_FOREACH(CCollObj, pEntry, *pList) {
            if (m_pNowRide == pEntry) {
                fgExist = 1;
                break;
            }
        }
        if (!fgExist) {
            CCollisionList* pList = m_pCollListR2;
            SJ_FOREACH(CCollObj, pEntry, *pList) {
                if (m_pNowRide == pEntry) {
                    fgExist = 1;
                    break;
                }
            }
        }
        if (fgExist) {
            m_pNowRide->removeRiderList(*m_pRider);
            if (m_pRider) delete m_pRider;
        }
        m_pRider = NULL;
        m_pNowRide = NULL;
    }
}

void CCollMap::setRidePosition(FVECTOR& Rotate, FVECTOR& Position) {
#line 341
    if (!m_pRider) AssertApp(__FILE__, __LINE__, "m_pRider", 0);
    m_pRider->setPosition(Rotate, Position);
}

BOOL CCollMap::getRidePosition(FVECTOR& Rotate, FVECTOR& Position) {
    CCollisionList* pList = m_pCollListR;
    INT fgExist = 0;
    SJ_FOREACH(CCollObj, pEntry, *pList) {
        if (m_pNowRide == pEntry) {
            fgExist = 1;
            break;
        }
    }
    if (!fgExist) {
        CCollisionList* pList = m_pCollListR2;
        SJ_FOREACH(CCollObj, pEntry, *pList) {
            if (m_pNowRide == pEntry) {
                fgExist = 1;
                break;
            }
        }
    }
    if (m_pRider && fgExist) {
        m_pRider->getPosition(Rotate, Position);
        return 1;
    }
    TRACE("CCollMap:\x82\xb5\x82\xa9\x82\xaf\x96\xb3\x8c\xf8\n");
    m_pRider = NULL;
    m_pNowRide = NULL;
    return 0;
}

void CCollMap::registerLastHang(HANDLE hTask) {
    if (m_pHanger) {
        if (m_pNowHang == m_pLastRide) return;
        removeHang();
    }
#line 391
    m_pHanger = new (__FILE__ "(391)")
        CRider(hTask, getShiftY(), getRadius(), (COLL_KIND)getKind(), TRUE);
    m_pLastRide->addRiderList(*m_pHanger);
    m_pNowHang = m_pLastRide;
    CTaskMan::getTask(m_pNowHang->getHandle())->setPriority((TASK_PRIORITY)0x8000);
}

void CCollMap::removeHang() {
    if (m_pNowHang && m_pHanger) {
        CCollisionList* pList = m_pCollListR;
        INT fgExist = 0;
        SJ_FOREACH(CCollObj, pEntry, *pList) {
            if (m_pNowHang == pEntry) {
                fgExist = 1;
                break;
            }
        }
        if (!fgExist) {
            CCollisionList* pList = m_pCollListR2;
            SJ_FOREACH(CCollObj, pEntry, *pList) {
                if (m_pNowHang == pEntry) {
                    fgExist = 1;
                    break;
                }
            }
        }
        if (fgExist) {
            m_pNowHang->removeRiderList(*m_pHanger);
            if (m_pHanger) delete m_pHanger;
        }
        m_pHanger = NULL;
        m_pNowHang = NULL;
    }
}

void CCollMap::setHangPosition(FVECTOR& Rotate, FVECTOR& Position) {
#line 434
    if (!m_pHanger) AssertApp(__FILE__, __LINE__, "m_pHanger", 0);
    m_pHanger->setPosition(Rotate, Position);
}

BOOL CCollMap::getHangPosition(FVECTOR& Rotate, FVECTOR& Position) {
    CCollisionList* pList = m_pCollListR;
    INT fgExist = 0;
    SJ_FOREACH(CCollObj, pEntry, *pList) {
        if (m_pNowHang == pEntry) {
            fgExist = 1;
            break;
        }
    }
    if (!fgExist) {
        CCollisionList* pList = m_pCollListR2;
        SJ_FOREACH(CCollObj, pEntry, *pList) {
            if (m_pNowHang == pEntry) {
                fgExist = 1;
                break;
            }
        }
    }
    if (m_pHanger && fgExist) {
        m_pHanger->getPosition(Rotate, Position);
        return 1;
    }
    TRACE("CCollMap:\x82\xb5\x82\xa9\x82\xaf\x96\xb3\x8c\xf8\n");
    m_pHanger = NULL;
    m_pNowHang = NULL;
    return 0;
}

INT CCollMap::checkWaterSurface(FVECTOR& inPos, FLOAT inHeight) {
    FVECTOR pos0 = inPos;
    INT ret;
    if (CCollUtil::checkWaterSurface(pos0)) {
        FLOAT h = pos0.y - inPos.y;
        inPos.y = pos0.y;
        if (!(h < inHeight)) ret = 2;
        else ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

// SOURCE DATA: .data .rodata
