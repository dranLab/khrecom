#define COLLUTIL_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollUtil.h"
#include "System/Task.h"
#include "Game/Collision/CollUtilA.h"
#include "Game/Collision/CollUtil.h"
#include "Game/Collision/CollAttack.h"
#include "Game/Collision/CollBody.h"
#include "System/Graphic/Camera.h"
#include "System/TaskMan.h"
#include "Game/Entity.h"
#include <math.h>
#include "include_asm.h"

extern "C" float atan2f(float y, float x);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void Normalize(FVECTOR& v0, const FVECTOR& v1);

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

/* iterates a CSJointListT, the current entry may be removed (original macro name unknown) */
#define SJ_FOREACH(T, pEntry, list) \
    for (T *pEntry = (list).getHead(), *__pn = pEntry ? pEntry->getNext() : NULL; pEntry; \
         pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL)

class CSphereCollision : public CCollision {};
class CPointCollision : public CCollision {};
class CCircleCollision : public CCollision {};
class CPoly3Collision : public CCollision {
public:
    FVECTOR m_inP2; /* 0x80 */
};

static inline FLOAT _limitPI(FLOAT a) {
    FLOAT r;
    if (a < -3.1415927f) {
        r = a + 6.2831855f;
    } else {
        r = a;
        if (3.1415927f < r) r -= 6.2831855f;
    }
    return r;
}

static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}

namespace CollAttack {
FLOAT DelayHitEffectFrame = 3.0f;
}

UINT CCollAttack::m_uiID = 1;

CCollAttack::~CCollAttack() {
    setEnemyHandle(0);
    initEnemyPos();
}

void CCollAttack::init(COLL_KIND collKind, COLL_TYPE collType, HANDLE hTask, INT fgStatus) {
    if (collType == COLL_TYPE_POINT || collType == COLL_TYPE_SPHERE || collType == COLL_TYPE_POLY3 ||
        collType == COLL_TYPE_CAPSULE || collType == COLL_TYPE_CIRCLE || collType == COLL_TYPE_INFINITY) {
        CCollObj::init((COLL_KIND)(collKind | 1), collType, hTask, fgStatus);
        on();
    }
}

INT CCollAttack::collideAttack() {
    INT ret1;
    if (!m_Enable) return 0;
    m_fHitEffectWait = 0;
    if (m_hTask) {
        if (m_pCollListA && !m_pCollListA->hasMember(*this)) m_pCollListA->pushBack(*this);
    }
    if ((getKind() & 0xE) == COLL_KIND_PLAYER) {
        ret1 = attackSub(m_pCollListE);
        CTaskMan::getSysTask(1);
    } else {
        ret1 = attackSub(m_pCollListP);
    }
    if ((getKind() & 0xE) != COLL_KIND_OBJECT) return ret1;
    INT ret = attackSub(m_pCollListE);
    if (ret1) ret = ret1;
    return ret;
}

INT CCollAttack::attackSub(CCollisionList* pList) {
    INT attackType = getType() << 4;
    INT ret1 = 0;
    SJ_FOREACH(CCollObj, pEntry, *pList) {
        if (!pEntry->isEnable()) continue;
        if (pEntry->getHandle() == getHandle()) continue;
        if ((pEntry->getKind() & 0xE) == COLL_KIND_ITEM) continue;
        CCollision* pTarget = ((CCollAttack*)pEntry)->m_pCollision;
        INT type = pEntry->getType();
        INT ret0 = 0;
        switch (type | attackType) {
        case 0x12:
            ret0 = CCollUtil::detectSpherePoint(*(CSphereCollision*)pTarget,
                                                                 *(CPointCollision*)m_pCollision);
            break;
        case 0x22:
        case 0x82:
            ret0 = CCollUtil::detectSphereSphere(*(CSphereCollision*)pTarget,
                                                                  *(CSphereCollision*)m_pCollision);
            break;
        case 0x72:
            ret0 = CCollUtil::detectSpherePoly3(*(CSphereCollision*)pTarget,
                                                                 *(CPoly3Collision*)m_pCollision);
            break;
        case 0x14:
        case 0x15:
            ret0 = CCollUtil::detectColumnPoint(*(CColumnCollision*)pTarget,
                                                                 *(CPointCollision*)m_pCollision);
            break;
        case 0x24:
        case 0x25:
        case 0x84:
        case 0x85:
            ret0 = CCollUtil::detectColumnSphere(*(CColumnCollision*)pTarget,
                                                                  *(CSphereCollision*)m_pCollision, 0);
            break;
        case 0x74:
        case 0x75:
            ret0 = CCollUtil::detectColumnPoly3(*(CColumnCollision*)pTarget,
                                                                 *(CPoly3Collision*)m_pCollision);
            break;
        case 0xb2:
        case 0xb4:
        case 0xb5:
            ret0 = CCollUtil::detectSphereCircle(*(CSphereCollision*)pTarget,
                                                                  *(CCircleCollision*)m_pCollision);
            break;
        }
        if (attackType == 0xc0) {
            CCollision* pColl = ((CCollAttack*)pEntry)->m_pCollision;
            pColl->m_outWE = pColl->m_inWE;
            pColl->m_outDetect = pColl->m_inWE;
            pColl->m_outNormal.set(0.0f, 1.0f, 0.0f, 0.0f);
            ret0 = 1;
        }
        if (ret0) {
            INT ret = collideAttackFunc((CCollBody*)pEntry);
            if (!ret1) ret1 = ret;
        }
    }
    return ret1;
}

BOOL CCollAttack::collideAttackFunc(CCollBody* pEntry) {
    FVECTOR dir;
    FVECTOR rot;
    pEntry->setEnemyHandle(getHandle());
    setEnemyHandle(pEntry->getHandle());
    if (pEntry->checkAttackID(m_uiAttackID)) return FALSE;
    if (chkMutekiSuccess(pEntry)) {
        pEntry->setMutekiTrg(0x20);
        pEntry->setEnemyPos(m_vEnemyPos);
        pEntry->setAttackID(m_uiAttackID);
        pEntry->setDamageAttr(m_AttackAttr);
        pEntry->setDamageKind(m_AttackKind);
        pEntry->setDamage(0);
        return 2;
    }
    if (chkGuardSuccess(pEntry)) {
        pEntry->setGuardTrg(8);
        pEntry->setEnemyPos(m_vEnemyPos);
        pEntry->setAttackID(m_uiAttackID);
        pEntry->setDamageAttr(m_AttackAttr);
        pEntry->setDamageKind(m_AttackKind);
        pEntry->setDamage(0);
        return 3;
    }
    pEntry->setHit();
    pEntry->setDamageKind(m_AttackKind);
    if (chkBackAttack(pEntry)) {
        pEntry->setDamage(m_Attack + m_BackAttack);
    } else {
        pEntry->setDamage(m_Attack + m_Critical);
        if (m_Critical) pEntry->setCritical(0x10);
    }
    pEntry->setEnemyPos(m_vEnemyPos);
    pEntry->setDamageAttr(m_AttackAttr);
    if (getType() == COLL_TYPE_POLY3) {
        pEntry->setDamagePlane(m_pCollision->m_inWS, m_pCollision->m_inWE, ((CPoly3Collision*)m_pCollision)->m_inP2);
    } else {
        pEntry->clearDamagePlane();
    }
    pEntry->setDamagePos(m_pCollision->m_inWS, m_pCollision->m_inWE);
    pEntry->setAttackID(m_uiAttackID);
    SubVectorXYZ(dir, *getStartPos(), *getEndPos());
    Normalize(dir, dir);
    rot.x = -atan2f(dir.y, _sqrtf(dir.z * dir.z + dir.x * dir.x));
    rot.y = atan2f(dir.x, dir.z);
    rot.z = 0;
    rot.w = 1.0f;
    pEntry->setHitEffectParam(m_strHitEffectName, m_iHitEffectNo, m_HitEffectObjType, rot, m_fHitEffectWait);
    m_fHitEffectWait += CollAttack::DelayHitEffectFrame;
    return TRUE;
}

void CCollAttack::setBasePos(FVECTOR& vBasePos) {
    if (m_pCollision && m_pCollision->m_Type == COLL_TYPE_POLY3) ((CPoly3Collision*)m_pCollision)->m_inP2 = vBasePos;
}

FVECTOR* CCollAttack::getBasePos() {
    if (m_pCollision && m_pCollision->m_Type == COLL_TYPE_POLY3) return &((CPoly3Collision*)m_pCollision)->m_inP2;
    return NULL;
}

/* not matching: the original's last angle wrap branch has an empty delay slot (ours: bc1fl with the abs.s) */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollAttack", chkGuardSuccess__11CCollAttackP9CCollBody);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollAttack", chkMutekiSuccess__11CCollAttackP9CCollBody);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollAttack", chkBackAttack__11CCollAttackP9CCollBody);
#if 0
BOOL CCollAttack::chkGuardSuccess(CCollBody* pBody) {
    if (!pBody->isGuard()) return FALSE;
    if (!(pBody->getGuardAttrFlag() & (1 << m_AttackAttr))) return FALSE;
    FVECTOR vEnemy = m_vEnemyPos;
    CEntity* pObj = (CEntity*)CTaskMan::getTask(pBody->getHandle());
    if (!pObj->getCoords() || pObj->Crd(0).getParent()) return FALSE;
    FVECTOR vPos = pObj->Crd(0).getWorldMatrix().v[3];
    FVECTOR vDir;
    SubVectorXYZ(vDir, vEnemy, vPos);
    FLOAT fDir = _limitPI(atan2f(vDir.x, vDir.z));
    FLOAT fDiff = _limitPI(fDir - pObj->getRot().y);
    if (fabsf(fDiff) > pBody->getGuardDir()) return FALSE;
    return TRUE;
}

BOOL CCollAttack::chkMutekiSuccess(CCollBody* pBody) {
    if (!pBody->isMuteki()) return FALSE;
    if (!(pBody->getMutekiAttrFlag() & (1 << m_AttackAttr))) return FALSE;
    FVECTOR vEnemy = m_vEnemyPos;
    CEntity* pObj = (CEntity*)CTaskMan::getTask(pBody->getHandle());
    if (!pObj->getCoords() || pObj->Crd(0).getParent()) return FALSE;
    FVECTOR vPos = pObj->Crd(0).getWorldMatrix().v[3];
    FVECTOR vDir;
    SubVectorXYZ(vDir, vEnemy, vPos);
    FLOAT fDir = _limitPI(atan2f(vDir.x, vDir.z));
    FLOAT fDiff = _limitPI(fDir - pObj->getRot().y);
    if (fabsf(fDiff) > pBody->getMutekiDir()) return FALSE;
    return TRUE;
}

INT CCollAttack::chkBackAttack(CCollBody* pBody) {
    if (!m_BackAttack) return 0;
    FVECTOR vEnemy = m_vEnemyPos;
    CEntity* pObj = (CEntity*)CTaskMan::getTask(pBody->getHandle());
    if (!pObj->getCoords()) return 0;
    if (pObj->Crd(0).getParent()) return 0;
    FVECTOR vPos = pObj->Crd(0).getWorldMatrix().v[3];
    FVECTOR vDir;
    SubVectorXYZ(vDir, vEnemy, vPos);
    FLOAT fDir = _limitPI(atan2f(vDir.x, vDir.z));
    FLOAT fDiff = _limitPI(fDir - pObj->getRot().y);
    if (fabsf(fDiff) <= 1.5707964f) return 0;
    m_BackAttack = m_Attack;
    return 1;
}

#endif

void CCollAttack::updateAttackID() {
    if (++m_uiID == 0) m_uiID = 1;
    m_uiAttackID = m_uiID;
}

#line 440
void CCollAttack::setHitEffectParam(const CHAR* inName, INT inNo, ObjType::eObjType inType) {
    ASSERT(inName);
    for (INT i = 0; i < 15; i++) {
        if (inName[i] == '\0' || inName[i] == '.') {
            m_strHitEffectName[i] = '\0';
            break;
        }
        m_strHitEffectName[i] = inName[i];
    }
    m_HitEffectObjType = inType;
    m_iHitEffectNo = inNo;
    m_strHitEffectName[15] = '\0';
}

// SOURCE DATA: .data .rodata
