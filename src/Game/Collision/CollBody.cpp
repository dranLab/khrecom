#define COLLUTIL_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollUtil.h" /* MATCH: this unit names CollUtil.h so (header default "Game/Collision/CollUtil.h") */
#include "Game/Collision/CollUtilA.h"
#include "Game/Collision/CollUtil.h"
#include "System/Graphic/Effect/effect.h"
#include "Game/Collision/CollBody.h"
#include <string.h>
#include "include_asm.h"

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT r);
FLOAT InnerProduct(const FVECTOR& v0, const FVECTOR& v1);
void OuterProduct(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void Normalize(FVECTOR& v0, const FVECTOR& v1);

/* iterates a CSJointListT, the current entry may be removed (original macro name unknown) */
#define SJ_FOREACH(T, pEntry, list) \
    for (T *pEntry = (list).getHead(), *__pn = pEntry ? pEntry->getNext() : NULL; pEntry; \
         pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL)

#line 20 "c:/KHCOM/MainApp/Game/Collision/CollBody.cpp"
CCollision CCollBody::m_collTmp;

CCollBody::CCollBody() {
    m_Damage = 0;
    m_DamageKind = ATK_KIND_NONE;
    m_enableBody = 0;
    clearDamagePlane();
    m_nPriority = 0;
    m_hEnemyTask = 0;
    m_uiFlag = 0;
    initEnemyPos();
    m_MutekiAttrFlag = 0xFFFFFFFF;
    m_fMutekiDir = 3.1415927f;
    m_fDir = 0;
    m_GuardAttrFlag = 0;
    m_fGuardDir = 0;
    m_iSaveAttackIDPtr = 0;
    for (INT i = 0; i < 16; i++) m_uiSaveAttackID[i] = 0;
    strcpy(m_strHitEffectName, "");
    m_iHitEffectNo = -1;
    m_fHitEffectWait = 0;
}

CCollBody::~CCollBody() {}

void CCollBody::init(COLL_KIND collKind, COLL_TYPE collType, HANDLE hTask, INT fgStatus) {
    CCollObj::init(collKind, collType, hTask, fgStatus);
    on();
}

/* not matching: the original copies the results at the end relative to m_collTmp.m_outWE (ours: relative to
 * m_collTmp, or with the registers of ret1 and m_collTmp swapped) */
#if 0
INT CCollBody::collideBody(INT iTargetType) {
    CCollisionList* pCollList[2];
    INT bodyType;
    INT ret1;
    FVECTOR vTmp;
    if (!m_enableBody) return 0;
    pCollList[1] = NULL;
    pCollList[0] = NULL;
    switch (iTargetType) {
    case 0:
        pCollList[0] = m_pCollListE;
        break;
    case 1:
        pCollList[0] = m_pCollListP;
        break;
    case 2:
        pCollList[0] = m_pCollListP;
        pCollList[1] = m_pCollListE;
        break;
    case 3:
        pCollList[0] = m_pCollListI;
        break;
    }
    CCollision* pColl = m_pCollision;
    bodyType = getType() << 4;
    ret1 = 0;
    m_collTmp = *pColl;
    for (INT i = 0; i < 2; i++) {
        if (!pCollList[i]) continue;
        SJ_FOREACH(CCollObj, pEntry, *pCollList[i]) {
            if (!((CCollBody*)pEntry)->isEnableBody()) continue;
            if (pEntry->getHandle() == getHandle()) continue;
            SubVector(vTmp, *(FVECTOR*)pColl, *(FVECTOR*)((CCollBody*)pEntry)->m_pCollision);
            if (InnerProduct(vTmp, vTmp) > 100.0f) continue;
            CColumnCollision* pTarget = (CColumnCollision*)((CCollBody*)pEntry)->m_pCollision;
            INT type = pEntry->getType();
            INT ret0 = 0;
            switch (type | bodyType) {
            case 0x22:
            case 0x24:
            case 0x42:
            case 0x44:
                ret0 = CCollUtil::detectColumnColumnEz(*pTarget, (CColumnCollision&)m_collTmp);
                break;
            case 0x25:
            case 0x45:
                ret0 = CCollUtil::detectColumnColumn(*pTarget, (CColumnCollision&)m_collTmp);
                break;
            default:
#line 158
                ASSERT(0);
                break;
            }
            if (ret0) {
                ret1 |= collideBodyFunc((CCollBody*)pEntry);
                FVECTOR vec;
                SubVector(vec, m_collTmp.m_inWE, m_collTmp.m_inWS);
                AddVector(vec, m_collTmp.m_outWE, vec);
                vec.w = 1.0f;
                m_collTmp.m_inWS = m_collTmp.m_outWE;
                m_collTmp.m_inWE = vec;
            }
        }
    }
    if (ret1) {
        m_pCollision->m_outWE = m_collTmp.m_outWE;
        m_pCollision->m_outDetect = m_collTmp.m_outDetect;
        m_pCollision->m_outInfo.info = m_collTmp.m_outInfo.info;
        m_pCollision->m_outNormal = m_collTmp.m_outNormal;
        m_pCollision->m_outStatus = m_collTmp.m_outStatus;
    }
    return ret1;
}
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollBody", collideBody__9CCollBodyi);
/* placeholder for collideBody's .rodata: its strings, then its jump table (labels of the included assembly) */
#line 158
inline void _collideBodyStrings() { ASSERT(0); }
__asm__(
    ".rdata\n"
    ".align 4\n"
    ".word .L00222330\n"
    ".word .L002223B8\n"
    ".word .L00222330\n"
    ".word .L00222374\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L002223B8\n"
    ".word .L00222330\n"
    ".word .L002223B8\n"
    ".word .L00222330\n"
    ".word .L00222374\n"
    ".text\n");

BOOL CCollBody::collideBodyFunc(CCollBody* pEntry) {
    switch (pEntry->getKind() & 0xE) {
    case COLL_KIND_PLAYER:
    case COLL_KIND_ENEMY:
        break;
    case COLL_KIND_ITEM:
        pEntry->setDamage(1);
        break;
    }
    return TRUE;
}

INT CCollBody::getDamage() {
    return m_Damage;
}

void CCollBody::setDamagePlane(FVECTOR& v0, FVECTOR& v1, FVECTOR& v2) {
    FVECTOR vec;
    FVECTOR vec0;
    FVECTOR vec1;
    FVECTOR norm;
    FLOAT d;
    SubVector(vec0, v1, v0);
    SubVector(vec1, v2, v0);
    OuterProduct(norm, vec0, vec1);
    if (norm.x == 0.0f && norm.y == 0.0f && norm.z == 0.0f) {
        norm.y = 1.0f;
    } else {
        Normalize(norm, norm);
    }
    d = InnerProduct(norm, v2);
    m_DamagePlane = norm;
    m_DamagePlane.w = -d;
}

void CCollBody::setAttackID(UINT inID) {
    for (INT i = 0; i < 16; i++) {
        if (m_uiSaveAttackID[i] == inID) {
            m_uiSaveAttackID[i] = 0;
            break;
        }
    }
    if (++m_iSaveAttackIDPtr >= 16) m_iSaveAttackIDPtr = 0;
    m_uiSaveAttackID[m_iSaveAttackIDPtr] = inID;
}

BOOL CCollBody::checkAttackID(UINT inID) {
    for (INT i = 0; i < 16; i++) {
        if (m_uiSaveAttackID[i] == inID) return TRUE;
    }
    return FALSE;
}

#line 351
void CCollBody::setHitEffectParam(const CHAR* inName, INT inNo, ObjType::eObjType inType, const FVECTOR& inRot, FLOAT inWait) {
    ASSERT(inName);
    for (INT i = 0; i < 15; i++) {
        if (inName[i] == '\0' || inName[i] == '.') {
            m_strHitEffectName[i] = '\0';
            break;
        }
        m_strHitEffectName[i] = inName[i];
    }
    m_iHitEffectNo = inNo;
    m_HitEffectObjType = inType;
    m_fHitEffectWait = inWait;
    m_strHitEffectName[15] = '\0';
    m_vHitEffectRot = inRot;
}

HANDLE CCollBody::setHitEffect() {
    if (m_iHitEffectNo < 0) return 0;
    FVECTOR vPos;
    switch (m_DamageKind) {
    case 10:
    case 11:
    case 19:
    case 28:
    case 29:
    case 30:
        switch (getType()) {
        case COLL_TYPE_SPHERE:
            vPos = *getEndPos();
            break;
        case COLL_TYPE_COLUMN:
        case COLL_TYPE_COLUMNR:
            AddVectorXYZ(vPos, *getEndPos(), *getStartPos());
            ScaleVectorXYZ(vPos, vPos, 0.5f);
            break;
        }
        break;
    default:
        vPos = *getDetectPos();
        break;
    }
    return CEffectMan::setEffect(m_strHitEffectName, m_iHitEffectNo, m_HitEffectObjType, &vPos, &m_vHitEffectRot,
                                 m_fHitEffectWait);
}

// SOURCE DATA: .rodata .bss
