#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholders: empty strings follow CollUtil.h's strings in .rodata (as in Game/Player/FriendKing); the middle one
 * is CCollAttack's constructor's (Game/Collision/CollAttack.h). */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}

#include "Game/Player/PlayerWeapon.h"

inline void _emptyStringPlaceholder2() {
    TRACE("");
}

#include "Game/Player/Player.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/Player/PlayerManager.h"
#include "Game/Player/FriendDonald.h"
#include "Game/Movement.h"
#include "Game/SceneInfo.h"
#include "Game/EffectMotion.h"
#include "Game/EntityFactory.h"
#include "Game/Collision/CollMap.h"
#include "System/Graphic/Camera.h"
#include "System/HeapMan.h"
#include "System/MainApp.h"

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new(size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}

/* A store through a reference is not an in-struct access: the scheduler keeps a following global load after it. */
template <class T> static inline T& _ref(T& v) { return v; }

void ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);

namespace PL {
extern BULLET_EFFECT_INFO g_EffTbl_HitMark[27]; /* Game/Player/PlayerAttack.h */
}

namespace PlayerWeapon {

static LPCSTR g_szDpxKBAppear = "kb_appear";
static LPCSTR g_szDpxSEAppear = "souleater";

FVECTOR g_fWeaponTopOffset = { { -0.8f, 0.0f, 0.0f, 1.0f } };

CPlayerWeapon::CPlayerWeapon(CTask* inParent, INT inID, INT inBone)
    : CEntity(inParent, ObjType::OBJ_TYPE_PLAYER), m_iBoneID(inBone), m_uiAttackID(0) {
    m_iHit = 0;
    m_uiFlag = 0;
    m_pPlayer = (CPlayer*)inParent;
    switch (inID) {
    case 1:
    case 2:
        initalize(1, PLM::g_uiPlayerRcid[inID] + 1);
        Crd(0).setParent(&m_pPlayer->Crd(m_iBoneID));
        Crd(0).touch();
        m_nHitEffWall = 2;
        break;
    case 3:
    case 4:
    case 5:
    case 6:
        m_uiFlag = 4;
        addCoord(1, &m_pPlayer->Crd(m_iBoneID));
        m_pMove = new (__FILE__ "(100)") CMovement(this);
        m_nHitEffWall = 19;
        break;
    default:
        m_uiFlag |= 4;
        addCoord(1, &m_pPlayer->Crd(m_iBoneID));
        m_pMove = new (__FILE__ "(106)") CMovement(this);
        m_nHitEffWall = 0;
        break;
    }
    m_collAttack.init(COLL_KIND_PLAYER, COLL_TYPE_POLY3, inParent->getHandle(), 0);
    m_collAttack.setAttack(0, 0);
    m_collAttack.off();
    m_pMove->setFlag(1, 0);
    if (CSceneInfo::isFieldScene()) {
        m_uiFlag |= 0x40;
        m_pCollMap = m_pMove->getMapCollision();
        m_pCollMap->setKind(COLL_KIND_SHOT);
        m_pCollMap->setType(COLL_TYPE_POINT);
        m_pCollMap->enableSliding(0);
        m_pCollMap->setRadius(0.25f);
        m_pCollMap->setShiftY(0.0f);
    } else {
        m_pCollMap = 0;
    }
    m_vFreePos.init();
    m_hEnemyHandle = 0;
    m_uiDoubleAttackID = 0;
}

CPlayerWeapon::~CPlayerWeapon() {
}

void CPlayerWeapon::stateInitial() {
}

void CPlayerWeapon::updateBefore() {
}

void CPlayerWeapon::updateAfter() {
    FVECTOR old;
    FVECTOR ofs;
    FVECTOR pos;
    FMATRIX m;
    PATK_DATA* pAtkData;

    if (!m_uiAttackID) return;

    if (m_uiFlag & 8) {
        old = getPos();
        old.x = Movement::Approach(old.x, m_vFreePos.x, 0.1f, getFrameRateF());
        old.y = Movement::Approach(old.y, m_vFreePos.y, 0.1f, getFrameRateF());
        old.z = Movement::Approach(old.z, m_vFreePos.z, 0.1f, getFrameRateF());
        setPos(old);
    }

    old = *m_collAttack.getEndPos();
    m_collAttack.setStartPos(old);
    ofs = g_fWeaponTopOffset;
    m = getMatrixWep0();
    if (m_uiFlag & 1) ofs.x = -1.2f;

    switch (m_uiAttackID) {
    case 115:
        if (PLM::CPlayerManager::getPLM()->getFriend()) {
            pos = PLM::CPlayerManager::getPLM()->getFriend()->getPos();
            m_collAttack.setEnemyPos(pos);
            pos = m_pPlayer->getPos();
        } else {
            pos = m_pPlayer->getPos();
            m_collAttack.setEnemyPos(pos);
        }
        ApplyMatrixXYZ(pos, m, ofs);
        m_collAttack.setEndPos(pos);
        break;
    case 183:
        ofs.x = 1.2f;
        ApplyMatrixXYZ(old, m, ofs);
        m_collAttack.setStartPos(old);
        ofs.x = -ofs.x;
        pos = m_pPlayer->getPos();
        m_collAttack.setEnemyPos(pos);
        ApplyMatrixXYZ(pos, m, ofs);
        m_collAttack.setEndPos(pos);
        break;
    case 184:
        pos = m_pPlayer->getAppearPos();
        m_collAttack.setStartPos(pos);
        ofs.x = -1.6f;
        pos = m_pPlayer->getPos();
        m_collAttack.setEnemyPos(pos);
        ApplyMatrixXYZ(pos, m, ofs);
        m_collAttack.setEndPos(pos);
        break;
    case 177:
    case 179:
    case 186:
        old = m_pPlayer->getCenterPos();
        m_collAttack.setStartPos(old);
        m_collAttack.setEnemyPos(m_pPlayer->getAppearPos());
        pos = m_pPlayer->getMarkPos();
        m_collAttack.setEndPos(pos);
        break;
    case 180:
        ofs.x = -1.2f;
        pos = m_pPlayer->getPos();
        m_collAttack.setEnemyPos(pos);
        ApplyMatrixXYZ(pos, m, ofs);
        m_collAttack.setEndPos(pos);
        break;
    default:
        pos = m_pPlayer->getPos();
        m_collAttack.setEnemyPos(pos);
        ApplyMatrixXYZ(pos, m, ofs);
        m_collAttack.setEndPos(pos);
        break;
    }

    switch (m_uiAttackID) {
    case 13:
    case 48:
    case 49:
    case 50:
    case 51:
    case 52:
    case 180:
    case 187:
        pos = m.v[3];
        break;
    default:
        pos = Crd(0).getWorldMatrix().v[3];
        break;
    }
    m_collAttack.setBasePos(pos);

    switch (m_uiAttackID) {
    case 97:
    case 98:
    case 99:
        m_collAttack.setRadius(0.1f);
        m_collAttack.setShiftY(0.0f);
        _ref(m_iHit) = m_collAttack.collideAttack();
        {
            Friend::CFriend* pFriend = PLM::CPlayerManager::getPLM()->getFriend();
            if (pFriend) {
                m_iHit = ((Friend::CFriendDonald*)pFriend)->checkBlitzHit(0);
                if (m_iHit) {
                    m_iHit = 4;
                    m_pPlayer->setStateSE(1, 0);
                    return;
                }
            }
        }
        break;
    default:
        m_iHit = m_collAttack.collideAttack();
        break;
    }

    pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    /* Dead here (reloaded in case 0), but it leaves %hi(g_EffTbl_HitMark) in a saved register for the later uses. */
    LPCSTR szFile = PL::g_EffTbl_HitMark[m_nHitEffWall].pEffectName;
    INT no = PL::g_EffTbl_HitMark[m_nHitEffWall].iNo;
    switch (m_iHit) {
    case 0:
        if (!m_pCollMap) return;
        if (!(m_uiFlag & 2)) return;
        setCollMapPos_toAttackID();
        if (m_pCollMap->collideRide(0) || m_pCollMap->collideMap()) {
            if (m_pCollMap->getDetectNormal()->y < 0.64278764f) {
                m_iHit = 3;
                m_pPlayer->setStateSE(2, 0);
                szFile = PL::g_EffTbl_HitMark[m_nHitEffWall].pEffectName;
                if (szFile) {
                    no = PL::g_EffTbl_HitMark[m_nHitEffWall].iNo;
                    pos = *m_pCollMap->getDetectPos();
                    setEffect(szFile, no, &pos, 0, 0.0f);
                }
            }
        }
        return;
    case 1:
        pos = *m_collAttack.getDetectPos();
        m_uiFlag &= ~2;
        m_hEnemyHandle = m_collAttack.getEnemyHandle();
        m_pPlayer->setStateSE(1, 0);
        break;
    case 2:
        pos = *m_collAttack.getDetectPos();
        m_uiFlag &= ~2;
        m_hEnemyHandle = m_collAttack.getEnemyHandle();
        break;
    case 3:
        pos = *m_collAttack.getDetectPos();
        m_hEnemyHandle = m_collAttack.getEnemyHandle();
        break;
    case 4:
        break;
    }
    if (m_uiFlag & 0x20) setDoubleHit(0, &pos);
    if (pAtkData->EFFECT == 12) {
        pos.y = m_pPlayer->getJumpStartPos().y;
        setEffect(PL::g_EffTbl_HitMark[13].pEffectName, PL::g_EffTbl_HitMark[13].iNo, &pos, 0, 0.0f);
    }
}

FMATRIX& CPlayerWeapon::getMatrixWep0() {
    if (m_uiFlag & 4) return m_pPlayer->Crd(m_iBoneID).getWorldMatrix();
    return Crd(2).getWorldMatrix();
}

void CPlayerWeapon::onAttack(INT inID, INT inAP, INT inKind, INT inAttr) {
    FVECTOR pos;
    PATK_DATA* pAtkData;

    m_uiAttackID = inID;
    pos = m_pPlayer->getCenterPos();
    m_collAttack.setEnemyPos(pos);
    pos = Crd(0).getWorldMatrix().v[3];
    m_collAttack.setAttack(inAP, 0);
    m_collAttack.setAttackKind((ATK_KIND)inKind);
    m_collAttack.setAttackAttr((ATK_ATTR)inAttr);
    m_collAttack.setBackAttack(0);
    m_collAttack.setRadius(0.5f);
    m_collAttack.setShiftY(0.0f);
    m_collAttack.setStartPos(pos);
    m_collAttack.setEndPos(pos);
    m_collAttack.setBasePos(pos);
    m_collAttack.updateAttackID();
    m_collAttack.on();
    pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    if (pAtkData->EFFECT) {
        BULLET_EFFECT_INFO* info = &PL::g_EffTbl_HitMark[pAtkData->EFFECT];
        m_collAttack.setHitEffectParam(info->pEffectName, info->iNo, (ObjType::eObjType)getEntityType());
    }
    m_iHit = 0;
    if (m_uiFlag & 0x40) m_uiFlag |= 2;
}

void CPlayerWeapon::updateAttackID() {
    m_collAttack.updateAttackID();
}

INT CPlayerWeapon::getCollAttackID() {
    return m_collAttack.getAttackID();
}

void CPlayerWeapon::setCollAttackID(UINT inID) {
    m_collAttack.setAttackID(inID);
}

void CPlayerWeapon::offAttack() {
    m_uiAttackID = 0;
    m_iHit = 0;
    m_uiFlag &= ~2;
    m_collAttack.off();
    setFreeMode(0, 0, 0, 0);
    m_uiFlag &= ~1;
    m_uiFlag &= ~0x10;
    m_uiFlag &= ~0x20;
}

void CPlayerWeapon::onWeapon(INT inEffect) {
    if (m_uiFlag & 4) return;
    if (!isDispOFF()) return;
    setEntityFlag(0, 1);
    if (inEffect) {
        FVECTOR rot;
        FVECTOR r = { { 0.0f, 0.0f, -1.5707964f, 1.0f } };
        rot = r;
        attachEffect(g_szDpxKBAppear, 0, &Crd(2), 0, &rot, 0.0f);
    }
    setupFade(0.0f, 1.0f, 2000, -1);
}

void CPlayerWeapon::offWeapon() {
    if (m_uiFlag & 4) return;
    setEntityFlag(1, 0);
}

void CPlayerWeapon::setWideAttack(INT inFlag) {
    if (!inFlag) {
        m_uiFlag &= ~1;
        m_collAttack.setRadius(0.5f);
        m_collAttack.setShiftY(0.0f);
    } else {
        m_uiFlag |= 1;
        m_collAttack.setRadius(1.0f);
        m_collAttack.setShiftY(0.0f);
    }
}

void CPlayerWeapon::setBackAttack(INT inFlag) {
    if (!inFlag) {
        m_collAttack.setBackAttack(0);
        m_uiFlag &= ~0x10;
    } else {
        m_collAttack.setBackAttack(inFlag);
        m_uiFlag |= 0x10;
    }
}

void CPlayerWeapon::setCollMapPos_toAttackID() {
    if (!m_pCollMap) return;
    switch (m_uiAttackID) {
    case 13:
    case 48:
    case 49:
    case 50:
    case 51:
    case 52:
    case 180:
    case 187:
        m_pCollMap->setStartPos(*m_collAttack.getBasePos());
        m_pCollMap->setEndPos(*m_collAttack.getEndPos());
        break;
    default:
        m_pCollMap->setStartPos(m_pPlayer->getCenterPos());
        m_pCollMap->setEndPos(*m_collAttack.getEndPos());
        break;
    }
}

void CPlayerWeapon::killEffect(FLOAT inFadeTime) {
    if (m_pEffect) m_pEffect->kill(inFadeTime);
}

void CPlayerWeapon::getEnemyPos(FVECTOR& outPos) {
    outPos = *m_collAttack.getDetectPos();
}

void CPlayerWeapon::setFreeMode(INT inFlag, INT inAnim, FVECTOR* inPos, FVECTOR* inRot) {
    if (inFlag) {
        m_uiFlag |= 8;
        if (!inPos) inPos = &Crd(2).getWorldMatrix().v[3];
        m_vFreePos = *inPos;
        Crd(0).setParent(0);
        Crd(0).touch();
        setOldPos(m_vFreePos);
        setPos(m_vFreePos);
        if (inRot) setRot(*inRot);
        if (inAnim > 0) changeMotion(inAnim, 0, 0, 0, 0, 0);
    } else if (m_uiFlag & 8) {
        m_uiFlag &= ~8;
        m_vFreePos.init();
        setPos(m_vFreePos);
        setRot(m_vFreePos);
        clearSpeed();
        Crd(0).setParent(&m_pPlayer->Crd(m_iBoneID));
        Crd(0).touch();
    }
}

void CPlayerWeapon::setDoubleHit(INT inFlag, const FVECTOR* inPos) {
    BULLET_PARAM param;
    PATK_DATA* pAtkData;

    if (inFlag) {
        m_uiFlag |= 0x20;
        m_uiDoubleAttackID = m_collAttack.getAttackID();
        m_collAttack.updateAttackID();
        return;
    }
    if (!inPos) return;
    param.init(1, 3);
    param.CollKind = 0x80000000;
    param.uiFlag = 0x4000004;
    param.AtkKind = m_collAttack.getAttackKind();
    param.AtkAttr = m_collAttack.getAttackAttr();
    param.iAttackPoint = m_collAttack.getAttack();
    param.fSpeed = 0.1f;
    param.tqHomingTime = 100;
    param.vPos = *inPos;
    param.fMaxDist = 0.0f;
    param.vRot = g_pPlayer->getRot();
    param.vRot.x = 1.5707964f;
    param.fCollRadius = m_collAttack.getRadius();
    param.uiAttackID = m_uiDoubleAttackID;
    param.tqFadeTime = 1000;
    param.fShiftY = 0.0f;
    pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    if (pAtkData->EFFECT) {
        BULLET_EFFECT_INFO* info = &PL::g_EffTbl_HitMark[pAtkData->EFFECT];
        static BULLET_EFFECT_INFO effTbl[4] = {
            { 0, 0, -1, 0 },
            { info->pEffectName, info->iNo, -1, 0 },
            { 0, 0, -1, 0 },
            { 0, 0, -1, 0 },
        };
        param.pEffectTbl = effTbl;
    }
    CEntityFactory::getInstance()->createBullet(param, this);
}

} // namespace PlayerWeapon
// SOURCE DATA: .data .rodata .bss
