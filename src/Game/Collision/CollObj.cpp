#define COLLUTIL_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollUtil.h"
#define COLLOBJ_H_COLLISION_CLASSES /* MATCH: the key unit of the collision classes (new CPointCollision, ...) */
#include "Game/Collision/CollUtilA.h"
#include "Game/Collision/CollUtil.h"
#include "Game/Collision/CollObj.h"
#include "System/Debug.h"

inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

CCollisionList* CCollObj::m_pCollListP = NULL;
CCollisionList* CCollObj::m_pCollListE = NULL;
CCollisionList* CCollObj::m_pCollListI = NULL;
CCollisionList* CCollObj::m_pCollListR = NULL;
CCollisionList* CCollObj::m_pCollListR2 = NULL;
CCollisionList* CCollObj::m_pCollListRC = NULL;
CCollisionList* CCollObj::m_pCollListA = NULL;

CCollObj::CCollObj() {
    m_pCollision = NULL;
    m_hTask = 0;
    m_Enable = 0;
}

CCollObj::~CCollObj() {
    unlink();
    if (m_pCollision) delete m_pCollision;
    m_pCollision = NULL;
}

void CCollObj::init(COLL_KIND collKind, COLL_TYPE collType, HANDLE hTask, INT fgStatus) {
    if (getType() == collType) return;
    if (m_pCollision) {
        if (!(getKind() & 1)) unlink();
        if (m_pCollision) delete m_pCollision;
        m_pCollision = NULL;
    }
    switch (collType) {
    case COLL_TYPE_POINT:
#line 51
        m_pCollision = new (com_malloc_d(sizeof(CPointCollision), __FILE__ "(51)", RET_ADDR())) CPointCollision;
        break;
    case COLL_TYPE_SPHERE:
    case COLL_TYPE_CAPSULE: {
#line 55
        CSphereCollision* pSphere = new (com_malloc_d(sizeof(CSphereCollision), __FILE__ "(55)", RET_ADDR())) CSphereCollision;
        m_pCollision = pSphere;
        if (collKind & 1) pSphere->m_shiftY = 0;
        break;
    }
    case COLL_TYPE_COLUMN:
    case COLL_TYPE_COLUMNR:
#line 65
        m_pCollision = new (com_malloc_d(sizeof(CColumnCollision), __FILE__ "(65)", RET_ADDR())) CColumnCollision;
        break;
    case COLL_TYPE_POLY3:
#line 71
        m_pCollision = new (com_malloc_d(sizeof(CPoly3Collision), __FILE__ "(71)", RET_ADDR())) CPoly3Collision;
        break;
    case COLL_TYPE_MODEL:
#line 74
        m_pCollision = new (com_malloc_d(sizeof(CModelCollision), __FILE__ "(74)", RET_ADDR())) CModelCollision;
        break;
    case COLL_TYPE_ETC:
#line 77
        m_pCollision = new (com_malloc_d(sizeof(CEtcCollision), __FILE__ "(77)", RET_ADDR())) CEtcCollision;
        break;
    case COLL_TYPE_CIRCLE:
#line 80
        m_pCollision = new (com_malloc_d(sizeof(CCircleCollision), __FILE__ "(80)", RET_ADDR())) CCircleCollision;
        break;
    case COLL_TYPE_INFINITY:
#line 83
        m_pCollision = new (com_malloc_d(sizeof(CInfinityCollision), __FILE__ "(83)", RET_ADDR())) CInfinityCollision;
        break;
    default:
        return;
    }
    setType(collType);
    setKind(collKind);
    if (!(collKind & 1)) {
        link(hTask);
    } else {
        m_hTask = hTask;
    }
    m_pCollision->m_fgStatus = fgStatus;
}

#line 110
void CCollObj::link(HANDLE hTask) {
    if (m_hTask) return;
    m_hTask = hTask;
    CCollisionList* pList = NULL;
    switch (getKind() & 0xE) {
    case COLL_KIND_PLAYER:
        pList = m_pCollListP;
        break;
    case COLL_KIND_ENEMY:
        pList = m_pCollListE;
        break;
    case COLL_KIND_OBJECT:
        pList = m_pCollListR;
        break;
    case COLL_KIND_ITEM:
        pList = m_pCollListI;
        break;
    }
#line 121
    if (!pList) AssertApp(__FILE__, __LINE__, "pList", 0);
    pList->pushBack(*this);
}

void CCollObj::unlink() {
    if (!m_hTask) return;
    CCollisionList* pList = NULL;
    if (getKind() & 1) {
        if (!m_pCollListA) return;
        pList = m_pCollListA;
    } else {
        switch (getKind() & 0xE) {
        case COLL_KIND_PLAYER:
            pList = m_pCollListP;
            break;
        case COLL_KIND_ENEMY:
            pList = m_pCollListE;
            m_pCollListR2->remove(*this);
            m_pCollListRC->remove(*this);
            break;
        case COLL_KIND_OBJECT:
            pList = m_pCollListR;
            m_pCollListR2->remove(*this);
            m_pCollListRC->remove(*this);
            break;
        case COLL_KIND_ITEM:
            pList = m_pCollListI;
            break;
        }
    }
    if (pList) {
        pList->remove(*this);
    } else {
        TRACE("CCollObj::\x96\xa2\x92\x6d\x82\xcc\x83\x4a\x83\x43\x83\x93\x83\x68\x81\x48\n");
    }
    m_hTask = 0;
}

void CCollObj::setRadius(FLOAT inRadius) {
    switch (getType()) {
    case COLL_TYPE_SPHERE:
        m_pCollision->m_Radius = inRadius;
        if (!(getKind() & 1)) m_pCollision->m_shiftY = inRadius;
        break;
    case COLL_TYPE_COLUMN:
    case COLL_TYPE_COLUMNR:
    case COLL_TYPE_POLY3:
    case COLL_TYPE_CAPSULE:
    case COLL_TYPE_CIRCLE:
        m_pCollision->m_Radius = inRadius;
        break;
    }
}

void CCollObj::setShiftY(FLOAT inShiftY) {
    switch (getType()) {
    case COLL_TYPE_POINT:
    case COLL_TYPE_SPHERE:
        m_pCollision->m_shiftY = inShiftY;
        break;
    }
}

#line 215
void CCollObj::collisionInit() {
    m_pCollListP = new (com_malloc_d(sizeof(CCollisionList), __FILE__ "(216)", RET_ADDR())) CCollisionList;
    m_pCollListE = new (com_malloc_d(sizeof(CCollisionList), __FILE__ "(217)", RET_ADDR())) CCollisionList;
    m_pCollListI = new (com_malloc_d(sizeof(CCollisionList), __FILE__ "(218)", RET_ADDR())) CCollisionList;
    m_pCollListR = new (com_malloc_d(sizeof(CCollisionList), __FILE__ "(219)", RET_ADDR())) CCollisionList;
    m_pCollListR2 = new (com_malloc_d(sizeof(CCollisionList), __FILE__ "(220)", RET_ADDR())) CCollisionList;
    m_pCollListRC = new (com_malloc_d(sizeof(CCollisionList), __FILE__ "(221)", RET_ADDR())) CCollisionList;
    m_pCollListA = new (com_malloc_d(sizeof(CCollisionList), __FILE__ "(223)", RET_ADDR())) CCollisionList;
}

void CCollObj::collisionDelete() {
    if (m_pCollListP) delete m_pCollListP;
    m_pCollListP = NULL;
    if (m_pCollListE) delete m_pCollListE;
    m_pCollListE = NULL;
    if (m_pCollListI) delete m_pCollListI;
    m_pCollListI = NULL;
    if (m_pCollListR) delete m_pCollListR;
    m_pCollListR = NULL;
    if (m_pCollListR2) delete m_pCollListR2;
    m_pCollListR2 = NULL;
    if (m_pCollListRC) delete m_pCollListRC;
    m_pCollListRC = NULL;
    if (m_pCollListA) delete m_pCollListA;
    m_pCollListA = NULL;
}

// SOURCE DATA: .data .rodata
