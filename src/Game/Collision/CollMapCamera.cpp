#define COLLUTIL_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollUtil.h"
#include "Game/Collision/CollMapCamera.h"
#include "Game/Collision/CollUtilA.h"
#include "Game/Collision/CollUtil.h"
#include "System/HeapMan.h"
#include <string.h>

inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

/* iterates a CSJointListT, the current entry may be removed (original macro name unknown) */
#define SJ_FOREACH(T, pEntry, list) \
    for (T *pEntry = (list).getHead(), *__pn = pEntry ? pEntry->getNext() : NULL; pEntry; \
         pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL)

#include "Game/Collision/Ride.h"

/* PARTIAL: stand-in for Game/Movement COLL_INFO_UNION: owner not written */
union COLL_INFO_UNION {
    INT info;
    struct {
        BYTE infoID;
        BYTE infoATTR;
        BYTE infoPID;
        BYTE infoSTAT;
    };
};

class CMCSemiModel : public CSJointT<CMCSemiModel> {
public:
    FVECTOR center;     /* 0x10 */
    FLOAT transparency; /* 0x20 */
    INT flag;           /* 0x24 */
};

class CCMCSemiModelList : public CSJointListT<CMCSemiModel> {
public:
    virtual ~CCMCSemiModelList() {
        SJ_FOREACH(CMCSemiModel, __p, *this) delete __p;
        resetParam();
    }
};

#line 15 "c:/KHCOM/MainApp/Game/Collision/CollMapCamera.cpp"
CCMCSemiModelList* CCollMapCamera::m_pSemiModelList = NULL;
BOOL CCollMapCamera::m_fgForceStop = 0;

CCollMapCamera::CCollMapCamera() {
    CCMCSemiModelList*& rList = m_pSemiModelList;
    if (!rList) {
#line 24
        rList = new (com_malloc_d(sizeof(CCMCSemiModelList), __FILE__ "(24)", RET_ADDR())) CCMCSemiModelList;
        m_fgListExist = FALSE;
    } else {
        m_fgListExist = TRUE;
    }
    m_fgForceStop = FALSE;
}

CCollMapCamera::~CCollMapCamera() {
    if (!m_fgListExist) {
        if (m_pSemiModelList) delete m_pSemiModelList;
        m_pSemiModelList = NULL;
    }
}

void CCollMapCamera::clearList() {
    if (m_pSemiModelList) {
        SJ_FOREACH(CMCSemiModel, pEntry, *m_pSemiModelList) {
            m_pSemiModelList->remove(*pEntry);
            delete pEntry;
        }
        SJ_FOREACH(CCollObj, pEntry, *m_pCollListRC) ((CRide*)pEntry)->setFgOnLook(0);
        SJ_FOREACH(CCollObj, pEntry, *m_pCollListR2) ((CRide*)pEntry)->setFgOnLook(0);
        SJ_FOREACH(CCollObj, pEntry, *m_pCollListR) ((CRide*)pEntry)->setFgOnLook(0);
    }
}

INT CCollMapCamera::collideMap() {
    INT ret = 0;
    if (m_Enable && m_pCollision) {
        ret = collideRide(m_pCollListRC, FALSE);
        if (!ret) ret = collideRide(m_pCollListR2, FALSE);
        if (!ret) ret = collideRide(m_pCollListR, FALSE);
        if (ret) setEndPos(*getOutPos());
        ret |= CCollUtil::detectMCD(*m_pCollision);
    }
    return ret;
}

INT CCollMapCamera::collideMapTrueCamera(INT fgContinue) {
    INT ret;
    if (!m_Enable || !m_pCollision || m_fgListExist || m_pCollision->m_Kind != COLL_KIND_CAMERA) return 0;
    ret = 0;
    if (!m_fgForceStop) {
        if (!fgContinue) {
            m_pCollision->m_Kind = COLL_KIND_CAMERAT;
            CCollisionList*& rListRC = m_pCollListRC;
            ret = collideRide(rListRC, TRUE);
            if (!ret) ret = collideRide(m_pCollListR2, TRUE);
            if (!ret) ret = collideRide(m_pCollListR, TRUE);
            if (ret) setEndPos(*getOutPos());
            ret |= CCollUtil::detectMCD(*m_pCollision);
            SJ_FOREACH(CMCSemiModel, pEntry, *m_pSemiModelList) pEntry->flag = 0;
            if (((COLL_INFO_UNION*)&m_pCollision->m_outInfo)->infoPID == 1) {
                FMATRIX** pMat = CCollUtil::getInstance()->m_pSemiTransMat;
                INT stSuu = CCollUtil::getInstance()->m_SemiTransMatIdx;
                for (INT i = 0; i < stSuu; i++) {
                    FVECTOR& vTrans = pMat[i]->getTrans();
                    INT fgExist = 0;
                    SJ_FOREACH(CMCSemiModel, pEntry, *m_pSemiModelList) {
                        if (vTrans == pEntry->center) {
                            fgExist = 1;
                            pEntry->flag = 1;
                            break;
                        }
                    }
                    if (!fgExist) {
#line 134
                        CMCSemiModel* pEntry = new (com_malloc_d(sizeof(CMCSemiModel), __FILE__ "(134)", RET_ADDR())) CMCSemiModel;
                        pEntry->center = vTrans;
                        pEntry->transparency = 1.0f;
                        pEntry->flag = 1;
                        CCMCSemiModelList*& rList = m_pSemiModelList;
                        rList->pushBack(*pEntry);
                    }
                }
            }
            m_pCollision->m_Kind = COLL_KIND_CAMERA;
        }
    } else {
        SJ_FOREACH(CMCSemiModel, pEntry, *m_pSemiModelList) pEntry->flag = 0;
    }
    SJ_FOREACH(CMCSemiModel, pEntry, *m_pSemiModelList) {
        if (!pEntry->flag) {
            pEntry->transparency += 0.1f;
            if (pEntry->transparency >= 1.0f) {
                m_pSemiModelList->remove(*pEntry);
                delete pEntry;
            }
        } else {
            pEntry->transparency -= 0.1f;
            if (pEntry->transparency < 0.3f) pEntry->transparency = 0.3f;
        }
    }
    return ret;
}

INT CCollMapCamera::collideRide(CCollisionList* pList, INT fgTrue) {
    INT ret0 = 0;
    COLL_INFO_UNION& collInfoO = *(COLL_INFO_UNION*)&m_pCollision->m_outInfoO;
    collInfoO.info = 0;
    SJ_FOREACH(CCollObj, pEntry, *pList) {
        if (!pEntry->isEnable()) continue;
        INT type = pEntry->getType();
        switch (type) {
        case COLL_TYPE_MODEL:
        case COLL_TYPE_MODELW:
            ret0 = CCollUtil::detectOCD(*m_pCollision, &((CRideModel*)pEntry)->m_MCDInfo, ((CRideModel*)pEntry)->m_pOCDWork);
            break;
        case COLL_TYPE_ETC:
            ret0 = pEntry->detectEtc(*m_pCollision);
            break;
        }
        if (!m_fgForceStop) {
            if (fgTrue) {
                if (type == COLL_TYPE_MODEL || type == COLL_TYPE_MODELW) {
                    if (collInfoO.infoPID == 1) {
                        ((CRide*)pEntry)->setFgOnLook(1);
                        collInfoO.info = 0;
                    } else {
                        ((CRide*)pEntry)->setFgOnLook(0);
                    }
                } else if (ret0) {
                    ((CRide*)pEntry)->setFgOnLook(1);
                } else {
                    ((CRide*)pEntry)->setFgOnLook(0);
                }
            }
            if (ret0) break;
        } else {
            ((CRide*)pEntry)->setFgOnLook(0);
        }
    }
    return ret0;
}

BOOL CCollMapCamera::checkBGforCamera(FVECTOR& vPos, FVECTOR& vAim, FVECTOR& vOutPos, BOOL fgTransparency) {
    INT ret;
    if (!getStartPos()) {
        CCollMap::init(COLL_KIND_CAMERA, COLL_TYPE_SPHERE, 0, 0);
        setRadius(0.25f);
    }
    setStartPos(vAim);
    setEndPos(vPos);
    ret = 0;
    if (collideMap()) {
        vOutPos = *getOutPos();
        ret = 1;
        setEndPos(vOutPos);
    }
    if (fgTransparency) collideMapTrueCamera(0);
    return ret;
}

// SOURCE DATA: .data .rodata
