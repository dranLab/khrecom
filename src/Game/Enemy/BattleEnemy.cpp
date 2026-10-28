#define GAME_ENEMY_BATTLEENEMY_CPP
#include "khtypes.h"
#define BATTLEENEMY_H_STATE_TEMPLATE
#include "Game/Enemy/BattleEnemy.h"
#include "System/Debug.h"
#include "System/Fsm.h"
#include "Game/GameWork.h"
#include "Game/Enemy/EnemyParam.h"
#include "System/TaskMan.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "System/Graphic/Effect/effect.h"
#include "Game/Bullet/BulletManager.h"
#include "Game/Debug/DebugWork.h"
#include "Game/Movement.h"
#include "Game/CardBattle.h"
#include "Game/Player/Player.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Anim.h"
#include "System/Coord.h"
#include "Game/CardKind.h"
#include "Game/Enemy/BattleEnemyManager.h"
#include "Game/Card.h"
#include "Game/SceneInfo.h"
#include "Game/Game.h"
#include "Game/SceneEnv.h"
#include "Game/EffectMotion.h"
#include "System/Sound/SoundMan.h"
#include "System/Math3D.h"
#include "Game/MessageManager.h"
#include "Game/Collision/CollMap.h"
#include "System/Buf.h"
#include "Game/Enemy/MonstroEventBattle.h"
#include "Game/Gimmick/GimmickManager.h"
#include "System/Com.h"
#include "System/Rnd.h"
#include "Game/EntityFactory.h"
#include "include_asm.h"

extern "C" char D_004AD680[];
extern "C" char D_004AD3E0[];
extern "C" char D_004ACBC0[];
extern "C" char D_004AD3C0[];
extern "C" char D_004AD3C8[];
extern "C" char D_004AD3B8[];
extern "C" char D_004AD388[];
extern "C" char D_004AD578[];
/* PARTIAL: until SOURCE DATA, the unit's copy of the header static BattleEnemy::BadStatusEffectName (its .data
 * comes from the original assembly) */
template <class T> static inline T& _ref(T& x) { return x; }
struct CB_ENCHANT_VIEW {
    BYTE _pad[0x17C];
    CARD m_cardEnchant[2];
};
static inline INT _getEnchantKind(CardBattle::CCardBattle* p, INT inUser) {
    return ((CB_ENCHANT_VIEW*)p)->m_cardEnchant[inUser - 1].m_nKind < 0x44 ? 0 : ((CB_ENCHANT_VIEW*)p)->m_cardEnchant[inUser - 1].m_nKind - 0x43;
}
namespace BattleEnemy {
extern CHAR* FreezeEffectName;
extern CHAR* DuelMotionParamName;
extern CHAR* DisionEffectName;
extern CHAR* FileIDParamName;
extern CHAR* AlwaysEffectName;
extern CHAR* LargebodyGuardEffectEffectName;
extern CHAR* VexenGuardEffectEffectName;
extern CHAR* BindEffectName;
extern CHAR* MemoIDParamName;
extern CHAR* SpeedParamName;
extern CHAR* RikuParamName;
extern CHAR* MagnetParamName;
extern CHAR* MagnetEffectName;
extern CHAR* QuakeEffectName;
extern CHAR* DuelEffectName;
extern const FLOAT g_vSpotSizeTbl[9];
extern const unsigned char g_byHeartlessSpotSizeIndexTbl[39];
extern const unsigned char g_byBossSpotSizeIndexTbl[38];
extern CHAR* FreezeParamName;
extern const FVECTOR g_vIceSizeTbl[8];
extern const unsigned char g_byHeartlessIceSizeIndexTbl[39];
extern const unsigned char g_byBossIceSizeIndexTbl[38];
}
extern "C" char D_004AD928[];
extern "C" char D_004AD2A0[];
extern "C" char D_004AD568[];
extern "C" char D_004AD3D8[];
extern "C" char D_004AD430[];
extern "C" char D_004AD920[];
extern "C" char D_004AD2B0[];
extern "C" char D_004AD2E0[];
extern "C" char D_004AD2F8[];
extern "C" char D_004AD310[];
extern "C" char D_004AD328[];
extern "C" char D_004AD340[];
extern "C" char D_004AD358[];
extern "C" char D_004AD570[];
extern "C" char D_004AD638[];
extern "C" char D_004AD810[];
extern "C" char D_004AD840[];
extern "C" char D_004AD858[];
extern "C" char D_004AD870[];
extern "C" char D_004AD888[];
extern "C" char D_004AD588[];
extern "C" char D_004AC8E8[];
extern "C" char D_004AC8F0[];
extern "C" char D_004AC920[];
class CRsrcData { /* PARTIAL: System/RsrcData.h declares the class only with RSRCDATA_H_FULL */
    CBuf* m_pDataSrc; /* 0x00 */
public:
    BYTE* getRsrcData() const {
        BYTE* p = NULL;
        if (m_pDataSrc) p = m_pDataSrc->getBufPtr();
        return p;
    }
};
class CRsrcDB { /* PARTIAL: System/RsrcDB.h needs RsrcData.h internals (as in Form/Menu/SaveLoad) */
public:
    static void initSeekRsrc(INT nRsrcType, INT nGroupID);
    static CRsrcData* seekNextRsrc();
};
/* ENEMY_PARAM's inline relocation (as in Game/Enemy/MapEnemy): an already relocated file has the top bit of its
 * version set. */
static inline BOOL _setupEnemyParam(ENEMY_PARAM* inParam) {
    INT* p = (INT*)inParam;
    if (p[1] < 0) return TRUE;
    if (strncmp((const char*)inParam, D_004AC8E8, 4)) return FALSE;
    if (p[1] != 1) return FALSE;
    p[1] = 0x80000001;
    p[2] = (INT)inParam + p[2];
    p[0x48 / 4] = (INT)inParam + p[0x48 / 4];
    p[0x50 / 4] = (INT)inParam + p[0x50 / 4];
    p[0x94 / 4] = (INT)inParam + p[0x94 / 4];
    return TRUE;
}
/* ENEMY_PARAM's floor accessor (Game/Enemy/EnemyParam.h line 229 in the original) */
static inline BASIC_ENEMY_PARAM* _getBasicParam(ENEMY_PARAM* inParam, INT inFloor) {
    if (!(0 <= inFloor && inFloor < 13)) AssertApp(D_004AC8F0, 0xE5, D_004AC920, 0);
    return &inParam->pBasic[inFloor];
}
extern int g_iDuelCardNum[3] asm("g_iDuelCardNum.6149");
struct PRIZE_DATA {
    INT PrizeKind; /* CARD_KIND */
    INT iPoint;
};
extern PRIZE_DATA g_PrizeDataTbl[7] asm("g_PrizeDataTbl.6082"); // PARTIAL: makeExpPrize's static table, from the asm .data until SOURCE DATA // PARTIAL: setupDuelCard's static table, from the asm .data until SOURCE DATA
struct DropPrize { // PARTIAL: Game/Gimmick/GimmickManager.h only forward-declares it outside its own unit
    INT StockKind;
    CARD_KIND Kind;
    SHORT Num;
    SHORT No;
};
extern "C" void sceVu0InversMatrix(FMATRIX& m0, const FMATRIX& m1);
struct STOP_DAMAGE {
    FVECTOR vDetectPos;
    char szHitEffectName[16];
    SHORT sHitEffectNo;
    SHORT sHitIndex;
    eObjType hitEffectObjType;
    ATK_KIND dmgKind;
    INT iDamage;
};
class CJiminyMemoFlag {
public:
    static void checkEvent(INT inEvent, INT inParam0, INT inParam1);
};
extern ATK_ATTR g_AtkAttrTbl[] asm("g_AtkAttrTbl.5784"); // PARTIAL: getAttackAttr's static table, from the asm .data until SOURCE DATA
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT t);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void ApplyMatrix33(FVECTOR& v0, const FMATRIX& m, const FVECTOR& v1);
FLOAT InnerProduct(const FVECTOR& v0, const FVECTOR& v1);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
extern "C" void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
extern "C" void sceVu0RotMatrixX(FMATRIX& m0, FMATRIX& m1, float rx);
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
extern "C" float sinf(float);
extern "C" float cosf(float);
extern "C" float atan2f(float, float);
extern CHAR* g_BadStatusEffectName asm("_11BattleEnemy$BadStatusEffectName_003A85F0");
FVECTOR getPlayerPos(INT inIndex);
namespace BattleEnemy {
FLOAT GameModeAp(FLOAT inAp);
INT GameModeHp(INT inHp);
extern FLOAT GameModeParamAp[];
}
namespace Gauge {
class CCommonGauge { /* PARTIAL: Game/Player/PlayerGauge.h */
public:
    static void setDispSeq(HANDLE inOwner, INT inSeq);
};
class CEnemyGauge { /* PARTIAL: Game/Player/PlayerGauge.h declares the class only for its own unit */
public:
    static void notifyHP(HANDLE inHandle, INT inHp, INT inHpMax);
};
}

INT BattleEnemy::GameModeHp(INT inHp) {
    return inHp * GameModeParamHp[CGameWork::getByte(0, 0x61)] / 100;
}
FLOAT BattleEnemy::GameModeAp(FLOAT inAp) {
    return inAp * GameModeParamAp[CGameWork::getByte(0, 0x61)];
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", __12CBattleEnemyP5CTaskP6CStateT2);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", _$_12CBattleEnemy);
INT CBattleEnemy::getTypeID() const {
    return 0x1004;
}
void CBattleEnemy::main() {
    if (!(DebugWork::g_dw.enemy.uiFlag & 0x20)) CEntity::main();
}
void CBattleEnemy::update() {
    if (DebugWork::g_dw.enemy.uiFlag & 0x400) clearSpeed();
    else CFSM::update();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", initalize__12CBattleEnemyiiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onMsg__12CBattleEnemyiPv);
void CBattleEnemy::onOut() {
    if (!(m_ullBtlEnemyFlag & 0x40000000)) notifyDead();
}
INT CBattleEnemy::getHpMax() const {
    if (m_pBasicParam) return BattleEnemy::GameModeHp(m_pBasicParam->iHP);
    return 1;
}
INT CBattleEnemy::getSize() const {
    if (m_pEnemyParam) return m_pEnemyParam->uiSize;
    return 1;
}
HANDLE CBattleEnemy::getNearestChild(FLOAT& outDist, CEntity& inEntity) {
    FLOAT nearest = 1e8f;
    HANDLE handle = -1;
    for (SLContainerT<CTask>* pContainer = getChildren().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (!task) continue;
        if (task->getTypeID() != 0x1004) continue;
        FLOAT dist = ((CEntity*)task)->calcDistanceSq(inEntity);
        FLOAT childDist;
        HANDLE childHandle = ((CBattleEnemy*)task)->getNearestChild(childDist, inEntity);
        if (childHandle != -1 && childDist < dist) {
            dist = childDist;
            task = CTaskMan::getTask(childHandle);
        }
        if (dist < nearest) {
            handle = task->getHandle();
            nearest = dist;
        }
    }
    return handle;
}
FLOAT CBattleEnemy::getExtraParam(const CHAR* inParamName) {
    if (m_pEnemyParam) return m_pEnemyParam->getExtraParam((char*)inParamName);
    return 0.0f;
}
FLOAT CBattleEnemy::getAttackPoint(INT inIndex, ATK_KIND inAtkKind, USHORT inUpFlag, ATK_KIND* outAtkKind, INT inFinish) {
    if (!m_pEnemyParam) AssertApp(D_004ACBC0, 0x414, D_004AD2A0, 0);
    ENEMY_ACT* act = NULL;
    for (INT i = 0; i < m_pEnemyParam->iActCount; i++) {
        if (m_pEnemyParam->pActs[i].iActNo == inIndex) {
            act = &m_pEnemyParam->pActs[i];
            break;
        }
    }
    if (act) {
        CardBattle::ENCHANT_PARAM param;
        param.init(getHandle(), 2);
        param.AtkKind = inAtkKind;
        param.uiState = 1;
        param.AtkAttr = getAttackAttr(inIndex);
        param.iAttackPoint = (INT)(BattleEnemy::GameModeAp((FLOAT)m_pBasicParam->iAP) * act->fPower + 0.5f);
        param.iHp = getHp();
        param.iHpMax = getHpMax();
        param.iFinish = inFinish;
        if (!inUpFlag) {
            switch (param.AtkAttr) {
            case ATK_ATTR_FIRE:
                param.UpKind = 8;
                break;
            case ATK_ATTR_ICE:
                param.UpKind = 0x10;
                break;
            case ATK_ATTR_THUNDER:
                param.UpKind = 0x20;
                break;
            }
        } else {
            param.UpKind = inUpFlag;
        }
        CardBattle::CCardBattle* cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
        if (cb) {
            cb->calcEnchantParam(param);
            if (outAtkKind) *outAtkKind = param.AtkKind;
            TRACE_USER((USER)5, D_004AD2B0);
            TRACE_USER((USER)5, D_004AD2E0, getID());
            TRACE_USER((USER)5, D_004AD2F8, param.iAttackPoint);
            TRACE_USER((USER)5, D_004AD310, param.AtkKind);
            TRACE_USER((USER)5, D_004AD328, param.AtkAttr);
            TRACE_USER((USER)5, D_004AD340, act->iActNo);
            TRACE_USER((USER)5, D_004AD358);
            return param.iAttackPoint;
        }
    }
    return 0.0f;
}
ATK_ATTR CBattleEnemy::getAttackAttr(INT inIndex) const {
    if (!m_pEnemyParam) AssertApp(D_004ACBC0, 0x465, D_004AD2A0, 0);
    ENEMY_ACT* act = NULL;
    for (INT i = 0; i < m_pEnemyParam->iActCount; i++) {
        if (m_pEnemyParam->pActs[i].iActNo == inIndex) {
            act = &m_pEnemyParam->pActs[i];
            break;
        }
    }
    if (!act) return ATK_ATTR_PHYSICAL;
    return g_AtkAttrTbl[act->iAttribute];
}
INT CBattleEnemy::getAttackRate() const {
    if (!m_pEnemyParam) return 10000;
    INT power = 1;
    if (m_tqConfuseTimer > 0) power = 10;
    if ((UINT)getID() % 10000 < 100) {
        CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
        if (bem) {
            power *= bem->getEnemyCount();
            if (power <= 0) power = 1;
        }
    }
    return m_pEnemyParam->iAttackRate[m_iActiveAttackRate] * power;
}
void CBattleEnemy::getUseCardPos(FVECTOR& outPos) {
    FLOAT offset = getExtraParam(BattleEnemy::CamTrgOfsParamName);
    outPos = Crd(0).m_World.getTrans();
    outPos.y += offset;
}
BOOL CBattleEnemy::isPlayingCardBattle() {
    if (CTaskMan::getSysTask(0x16) && CardBattle::CCardBattle::getPlayingHandle(2) == getHandle() &&
        CardBattle::CCardBattle::isPlayingEnemyCard()) {
        return TRUE;
    }
    return FALSE;
}
BOOL CBattleEnemy::isCurrentState(INT inState) {
    if (!((UINT)inState < 31)) AssertApp(D_004ACBC0, 0x4AF, D_004AD388, 0);
    CState& State = *(CState*)((inState * 0x20 + (INT)this) + 0x450);
    return isState(State);
}
BOOL CBattleEnemy::isValidSynchro() {
    return (INT)getExtraParam(BattleEnemy::SynchroParamName) != 0;
}
BOOL CBattleEnemy::canCardBattle() {
    if (CardBattle::CCardBattle::isPlayingEnemyCard()) return FALSE;
    CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
    if (!bem) AssertApp(D_004ACBC0, 0x4C8, D_004AD3B8, 0);
    return bem->canCardBattle(getHandle()) != FALSE;
}
BOOL CBattleEnemy::canKill() {
    if (CardBattle::CCardBattle::getPlayingHandle(2) == getHandle() && CardBattle::CCardBattle::isPlayingEnemyCard()) return FALSE;
    return !isCurrentState(15);
}
void CBattleEnemy::changeMainState(INT inState) {
    if (m_ullBtlEnemyFlag & 4) return;
    if (m_pMainState) m_pNewState = &((CStateTemplate<CBattleEnemy>*)m_pMainState)[inState];
}
void CBattleEnemy::changeMotion(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo) {
    CEntity::changeMotion(inIndex, inTq, inRootMove, inGr, inEffectNo, inSENo);
    if (m_pAttack && m_pAttack->set(inIndex)) m_pAttack->setAttackParamForBattleEnemy();
}
BOOL CBattleEnemy::changeMotionIndex(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo) {
    if (!m_pAnim) return FALSE;
    if (m_pAnim->getAnimID() == inIndex) return FALSE;
    changeMotion(inIndex, inTq, inRootMove, inGr, inEffectNo, inSENo);
    return TRUE;
}
BOOL CBattleEnemy::startCardBattle() {
    m_ullBtlEnemyFlag &= ~0x802;
    return FALSE;
}
void CBattleEnemy::endCardBattle(CState* inState, TIMEQ inNextAttackWait) {
    CardBattle::CCardBattle* cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
    if (!cb) AssertApp(D_004ACBC0, 0x56C, D_004AD3C0, 0);
    if (_getEnchantKind(cb, 2) == 0x2F && isBtlEnemyFlag(0x20000000)) {
        changeBattleEnemyState(m_iAttackState);
        m_ullBtlEnemyFlag &= ~0x20000000;
        return;
    }
    destroyCard();
    m_tqCancelTimer = inNextAttackWait;
    m_ullBtlEnemyFlag &= ~1;
    if (inState) {
        CState& NewState = *inState;
        m_pNewState = &NewState;
    } else {
        changeDefaultState();
    }
}
INT CBattleEnemy::checkHitPos() {
    CCollBody* hit = m_pBodyColl->getHitCollBody();
    if (!hit) return -1;
    FVECTOR* detect = hit->getDetectPos();
    INT result = 2;
    FVECTOR n;
    n.x = 0.0f;
    n.y = 0.0f;
    n.z = 1.0f;
    n.w = 1.0f;
    ApplyMatrix33(n, Crd(0).m_World, n);
    Normalize(n, n);
    n.w = InnerProduct(n, *hit->getStartPos());
    if (InnerProduct(n, *detect) - n.w > 0.0f) result = 1;
    FVECTOR center = Crd(1).m_World.getTrans();
    if (center.x == 0.0f && center.y == 0.0f && center.z == 0.0f) center.y = 1.0f;
    Normalize(n, center);
    ScaleVector(center, center, 0.5f);
    n.w = InnerProduct(n, center);
    if (InnerProduct(n, *detect) - n.w > 0.0f) {
        result |= 4;
    } else {
        result |= 8;
    }
    return result;
}
void CBattleEnemy::playerHomingY(FLOAT inRate) {
    FVECTOR target;
    FVECTOR vel;
    if (!g_pPlayer) return;
    getPlayerPosition(target);
    ScaleVectorXYZ(vel, g_pPlayer->getVel(), 12.5f);
    vel.y = 0.0f;
    AddVectorXYZ(target, target, vel);
    m_pMove->homingY(target, inRate);
}
void CBattleEnemy::homingToPlayer(FLOAT inRate) {
    FVECTOR vPos;
    getPlayerPosition(vPos);
    m_pMove->homing(vPos, inRate);
}
void CBattleEnemy::homingToPlayerY(FLOAT inRate) {
    FVECTOR vPos;
    getPlayerPosition(vPos);
    m_pMove->homingY(vPos, inRate);
}
BOOL CBattleEnemy::turnToPlayer(FLOAT inLimit) {
    FVECTOR vPos;
    getPlayerPosition(vPos);
    return m_pMove->turnToTarget(vPos, inLimit);
}
BOOL CBattleEnemy::turnToPlayerY(FLOAT inLimit) {
    FVECTOR vPos;
    getPlayerPosition(vPos);
    return m_pMove->turnToTargetY(vPos, inLimit);
}
void CBattleEnemy::homingMoveToPlayer(FLOAT inRate, FLOAT inSpeed) {
    FVECTOR vPos;
    getPlayerPosition(vPos);
    m_pMove->homingMove(vPos, inRate, inSpeed);
}
void CBattleEnemy::homingMoveXZToPlayer(FLOAT inRate, FLOAT inSpeed) {
    FVECTOR pos;
    getPlayerPosition(pos);
    m_pMove->homingMoveXZ(pos, inRate, inSpeed);
}
void CBattleEnemy::getPlayerPosition(FVECTOR& outPos) {
    if (m_ullBtlEnemyFlag & 0x40000) outPos = m_vConfusePlayerPos;
    else outPos = getPlayerPos(0);
}
HANDLE CBattleEnemy::getPlayerHandle() {
    if (isBtlEnemyFlag(0x40000)) return 0;
    if (!g_pPlayer) AssertApp(D_004ACBC0, 0x611, D_004AD3C8, 0);
    return g_pPlayer->getHandle();
}
void CBattleEnemy::makeExpPrize() {
    if (m_pBodyColl->getHitCollBody()) {
        makeExpPrize(m_pBodyColl->getHitCollBody()->getDetectPos());
    } else {
        makeExpPrize(&Crd(1).m_World.getTrans());
    }
}
void CBattleEnemy::makePrize() {
    if (m_pBodyColl->getHitCollBody()) makePrize(m_pBodyColl->getHitCollBody()->getDetectPos());
}
void CBattleEnemy::bossDeadDemoDispOff() {
    m_pEffect->kill(0.0f);
    m_pSE->stop();
    setEntityFlag(0, 0x80000000);
    setModelAlphaScale(0.0f, -1);
    bodyCollOffBody(-1);
    loadModelColor_();
    killLoopEffect(0);
    killBadStatusEffect();
}
INT CBattleEnemy::getPlayerState() {
    if (g_pPlayer) return g_pPlayer->getPlayerState();
    return 2;
}
void CBattleEnemy::setBossKilledFlag() {
    CGameWork::setFlag(1, 2, 1);
}
void CBattleEnemy::updateBefore() {
    enemyCardDash_();
    if (m_pAttack) {
        if (m_pAttack->test()) {
            m_ullBtlEnemyFlag |= 0x40;
        } else {
            m_ullBtlEnemyFlag &= ~0x40;
        }
    }
    hitTest_();
    if (isBtlEnemyFlag(0x20000)) m_pMove->setOldPos2(getPos());
}
void CBattleEnemy::updateAfter() {
    cardBattleProc_();
    reviseObject_();
    updateBindTimer_();
    updateConfuseTimer_();
    updateHitBackTimer_();
    updateDamageTimer_();
    if (m_tqCancelTimer > 0) m_tqCancelTimer -= getFrameRate();
    if (isBtlEnemyFlag(0x20000)) {
        FVECTOR pos = getOldPos();
        pos.y += 0.01f;
        setOldPos(pos);
        pos.y -= 0.02f;
        setPos(pos);
        m_vBackupVel = getVel();
        FVECTOR vel;
        vel.init();
        setVel(vel);
    }
}
void CBattleEnemy::updatePositionAfter() {}
void CBattleEnemy::lastUpdate() {
    if (m_pBodyColl) m_pBodyColl->update();
    if (m_pAttack) m_pAttack->update();
    if (isBtlEnemyFlag(0x20000)) {
        setVel(m_vBackupVel);
        m_pMove->updateVelocity();
        if (!isBtlEnemyFlag(0x100)) m_pMove->setCollisionFlag(1, TRUE);
    }
    updateBadStatusEffect(m_hBadStatus);
}
void CBattleEnemy::changeDefaultState() {
    m_tqCancelTimer = 250;
    if (m_pDefaultState) {
        gotoState(*m_pDefaultState);
        m_ullBtlEnemyFlag |= 0x10;
    }
}
void CBattleEnemy::beginStateAppear() {
    if (isBtlEnemyFlag(8) || isBtlEnemyFlag(0x20000000000ULL)) {
        setEntityFlag(0x10000, 0);
        if (m_pEnemyParam) changeMotion(m_pEnemyParam->iFaintMotionIndex[1], 0, 0, 0, 0, 0);
        if (isBtlEnemyFlag(8)) {
            setTimer(15000);
            setBadStatusEffect(1, 0.0f);
        } else {
            setTimer(18000);
            setBadStatusEffect(5, 0.0f);
        }
        m_iSysStep = 0;
    } else if (m_pEnemyParam && m_pEnemyParam->iAppearMotionIndex != -1) {
        changeMotion(m_pEnemyParam->iAppearMotionIndex, 0, 0, 0, 0, 0);
        m_iSysStep = 0;
    } else {
        m_iSysStep = 1;
    }
    setEntityFlag(0x40000000, 0);
    setupFade(0.0f, 1.0f, 3000, -1);
    Message::CMessageManager::getInstance().sendMessage(150, getHandle(), getHandle(), 0x70F0F0F0, NULL);
}
void CBattleEnemy::stateAppear() {
    if (isBtlEnemyFlag(8) || isBtlEnemyFlag(0x20000000000ULL)) {
        if (m_pEnemyParam) {
            switch (m_iSysStep) {
            case 0:
                if (getTimer() <= 0) {
                    changeMotion(m_pEnemyParam->iFaintMotionIndex[2], 0, 0, -1, 0, 0);
                    killBadStatusEffect();
                    m_iSysStep++;
                }
                break;
            case 1:
                if (isMotionEnd()) changeDefaultState();
                break;
            }
        } else if (getTimer() <= 0) {
            changeDefaultState();
        }
    } else {
        m_pMove->homingY(g_pPlayer, 0.1f);
        switch (m_iSysStep) {
        case 0:
            if (isMotionEnd()) changeDefaultState();
            break;
        case 1:
            if (getTimer() <= 0) changeDefaultState();
            break;
        }
    }
}
void CBattleEnemy::endStateAppear() {}
void CBattleEnemy::beginStateSmallDamage() {
    if (m_pEnemyParam) {
        if (!isBtlEnemyFlag(0x100) && !(m_pMove->getCollisionFlag() & 5)) m_ullBtlEnemyFlag |= 0x10000000;
        changeDamageMotion(selectDamageMotion_(), 0, 0, -1);
    } else {
        setTimer(3000);
    }
    setupHitBack();
    playSmallDamageSE();
    if (isQuickRecover_()) {
        m_iSysStep = 1;
    } else {
        m_iSysStep = 0;
    }
}
void CBattleEnemy::stateSmallDamage() {
    if (m_pEnemyParam) {
        switch (m_iSysStep) {
        case 0:
            if (getMotionCurrentTime() >= getMotionTotalTime() / 4) {
                if (!isBtlEnemyFlag(8) && !isBtlEnemyFlag(0x10000000)) setBattleEnemyFlag(0x1010, 1);
                m_iSysStep = 1;
            }
        case 1:
            if (isBtlEnemyFlag(0x10000000) && (m_pMove->getCollisionFlag() & 5) && m_iLandMotionIndex != -1) {
                changeMotion(m_iLandMotionIndex, 0, 0, -1, 0, 0);
                m_iSysStep = 3;
            } else if (isMotionEnd()) {
                if (isBtlEnemyFlag(8)) {
                    changeBattleEnemyState(17);
                } else if (isBtlEnemyFlag(0x100) || (m_pMove->getCollisionFlag() & 5)) {
                    changeDefaultState();
                } else {
                    if (m_iFallMotionIndex != -1) changeMotion(m_iFallMotionIndex, 0, 0, -1, 0, 0);
                    m_iSysStep = 2;
                }
            }
            break;
        case 2:
            if (m_pMove->getCollisionFlag() & 5) {
                if (m_iLandMotionIndex == -1) {
                    changeDefaultState();
                } else {
                    changeMotion(m_iLandMotionIndex, 0, 0, -1, 0, 0);
                    clearSpeed();
                    m_iSysStep = 3;
                }
            }
            break;
        case 3:
            if (isMotionEnd()) changeDefaultState();
            break;
        }
    } else if (getTimer() <= 0) {
        if (isBtlEnemyFlag(8)) {
            changeBattleEnemyState(17);
        } else {
            changeDefaultState();
        }
    }
}
void CBattleEnemy::endStateSmallDamage() {
    m_ullBtlEnemyFlag &= ~0x10000000;
}
void CBattleEnemy::beginStateBigDamage() {
    if (!isBtlEnemyFlag(0x200000000ULL)) {
        setupFlick();
        m_ullBackupFlag = m_pMove->getFlag();
        m_pMove->enableGravity(TRUE);
        m_iSysStep = 0;
    } else {
        if (checkHitPos() & 1) {
            changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[6], 0, 0, -1);
        } else {
            changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[8], 0, 0, -1);
        }
        m_iSysStep = 1;
    }
    playBigDamageSE();
}
void CBattleEnemy::stateBigDamage() {
    if (m_pEnemyParam) {
        switch (m_iSysStep) {
        case 0:
            if (m_pMove->getVel().y < 0.0f && (m_pMove->getCollisionFlag() & 5)) {
                if (m_pAnim->getAnimID() == m_pEnemyParam->iDamageMotionIndex[6]) {
                    changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[7], 0, 1, -1);
                } else {
                    changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[9], 0, 1, -1);
                }
                clearSpeed();
                m_iSysStep++;
            }
            break;
        case 1:
            if (isMotionEnd()) {
                if (isBtlEnemyFlag(8)) {
                    changeBattleEnemyState(17);
                } else {
                    CCollBody* hitBody = m_pBodyColl->getHitCollBody();
                    if (!hitBody) AssertApp(D_004ACBC0, 0x860, D_004AD3D8, 0);
                    if (hitBody->getDamageKind() == ATK_KIND_SHOCK && m_pEnemyParam->getSpecialEffect(1)) {
                        changeBattleEnemyState(18);
                    } else {
                        changeDefaultState();
                    }
                }
            }
            break;
        }
    } else if (m_pMove->getVel().y < 0.0f && (m_pMove->getCollisionFlag() & 5)) {
        if (isBtlEnemyFlag(8)) {
            changeBattleEnemyState(17);
        } else {
            changeDefaultState();
        }
    }
}
void CBattleEnemy::endStateBigDamage() {
    if (!(m_ullBackupFlag & 4)) m_pMove->enableGravity(FALSE);
}
void CBattleEnemy::beginStateCardBreak() {
    if (m_pEnemyParam) {
        if (isBtlEnemyFlag(0x100) || (m_pMove->getCollisionFlag() & 5)) {
            changeMotion(m_pEnemyParam->iDamageMotionIndex[0], 0, 0, -1, 0, m_iCardBreakSeIdentifyNo);
        } else {
            changeMotion(m_pEnemyParam->iDamageMotionIndex[4], 0, 0, -1, 0, m_iCardBreakSeIdentifyNo);
            m_ullBtlEnemyFlag |= 0x10000000;
        }
    } else {
        setTimer(3000);
    }
    CBulletManager* p = (CBulletManager*)CTaskMan::getSysTask(0x1B);
    if (p) p->kill(getHandle());
    onCardBreak();
    m_iSysStep = 0;
}
void CBattleEnemy::stateCardBreak() {
    if (m_pEnemyParam) {
        switch (m_iSysStep) {
        case 0:
            if (isBtlEnemyFlag(0x10000000) && (m_pMove->getCollisionFlag() & 5) && m_iLandMotionIndex != -1) {
                changeMotion(m_iLandMotionIndex, 0, 0, -1, 0, 0);
                m_iSysStep = 2;
            } else if (isMotionEnd()) {
                if (isBtlEnemyFlag(0x100) || (m_pMove->getCollisionFlag() & 5)) {
                    changeDefaultState();
                } else {
                    if (m_iFallMotionIndex != -1) changeMotion(m_iFallMotionIndex, 0, 0, -1, 0, 0);
                    m_iSysStep = 1;
                }
            }
            break;
        case 1:
            if (m_pMove->getCollisionFlag() & 5) {
                if (m_iLandMotionIndex == -1) {
                    changeDefaultState();
                } else {
                    changeMotion(m_iLandMotionIndex, 0, 0, -1, 0, 0);
                    clearSpeed();
                    m_iSysStep = 2;
                }
            }
            break;
        case 2:
            if (isMotionEnd()) changeDefaultState();
            break;
        }
    } else if (getTimer() <= 0) {
        changeDefaultState();
    }
}
void CBattleEnemy::endStateCardBreak() {
    m_ullBtlEnemyFlag &= ~0x10000000;
}
void CBattleEnemy::beginStateZero() {
    CBulletManager* p = (CBulletManager*)CTaskMan::getSysTask(0x1B);
    if (p) p->kill(getHandle());
}
void CBattleEnemy::stateZero() {
    if (isBtlEnemyFlag(1)) TRACE(D_004AD3E0);
    changeDefaultState();
}
void CBattleEnemy::endStateZero() {}
void CBattleEnemy::beginStateDuel() {
    setupDuelCard();
    INT no = (INT)getExtraParam(BattleEnemy::DuelMotionParamName);
    if (no <= 0) {
        changeMotion(1, 0, 0, -1, 0, 0);
    } else {
        changeMotion(no, 0, 0, -1, 0, 0);
    }
    CBulletManager* p = (CBulletManager*)CTaskMan::getSysTask(0x1B);
    if (p) p->kill(getHandle());
    m_iSysStep = 0;
    setTimer(0);
    bodyCollOffBody(-1);
}
void CBattleEnemy::stateDuel() {
    if (!isBtlEnemyFlag(0x4000000000ULL)) m_pMove->turnToTargetY(g_pPlayer, 0.17453292f);
    switch (m_iSysStep) {
    case 0:
        if (isBtlEnemyFlag(0x100000000ULL)) {
            INT id = (UINT)getID() % 10000;
            INT idx = id % 100;
            FLOAT scale;
            if (id < 100) {
                scale = BattleEnemy::g_vSpotSizeTbl[BattleEnemy::g_byHeartlessSpotSizeIndexTbl[idx]];
            } else {
                scale = BattleEnemy::g_vSpotSizeTbl[BattleEnemy::g_byBossSpotSizeIndexTbl[idx]];
            }
            if (scale > 0.0f) {
                FVECTOR pos;
                pos.init();
                m_hSpotLight[0] = attachEffect(BattleEnemy::DuelEffectName, 2, &Crd(0), &pos, NULL, 0.0f);
                if (!isBtlEnemyFlag(0x100)) m_hSpotLight[1] = setEffect(BattleEnemy::DuelEffectName, 3, &getPos(), NULL, 0.0f);
                for (INT i = 0; i < 2; i++) {
                    CEffect* effect = CEffectMan::getEffect(m_hSpotLight[i]);
                    if (effect) effect->setScale(scale, scale, scale);
                }
            }
            m_iSysStep++;
        }
        break;
    case 1:
        if (getTimer() <= 0) {
            CardBattle::CCardBattle* cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
            if (!cb) AssertApp(D_004ACBC0, 0x96B, D_004AD3C0, 0);
            if (m_iDuelCardNum > 0 && m_iDuelCardPtr < m_iDuelCardNum) {
                if (cb->setEnemyDuelCard(getHandle(), m_DuelCardKind, m_iDuelCardArray[m_iDuelCardPtr])) m_iDuelCardPtr++;
            }
            setTimer(200);
        }
        break;
    }
    updateSpotLight_();
}
void CBattleEnemy::endStateDuel() {
    for (INT i = 0; i < 2; i++) {
        CEffect* effect = CEffectMan::getEffect(m_hSpotLight[i]);
        if (effect) {
            effect->detachCoord();
            effect->loopEnd();
        }
        m_hSpotLight[i] = 0;
    }
    bodyCollOnBody(-1);
}
void CBattleEnemy::beginStateDuelSuccess() {
    INT no = (INT)getExtraParam(BattleEnemy::DuelMotionParamName);
    if (no <= 0) {
        changeMotion(1, 0, 0, -1, 0, 0);
    } else {
        changeMotion(no, 0, 0, -1, 0, 0);
    }
    m_iSysStep = 0;
    setTimer(15000);
}
void CBattleEnemy::stateDuelSuccess() {
    if (!isBtlEnemyFlag(0x4000000000ULL)) m_pMove->turnToTargetY(g_pPlayer, 0.17453292f);
    if (getTimer() <= 0) changeDefaultState();
}
void CBattleEnemy::endStateDuelSuccess() {}
void CBattleEnemy::beginStateFaint() {
    if (m_pEnemyParam) {
        changeMotion(m_pEnemyParam->iFaintMotionIndex[0], 0, 0, -1, 0, 0);
    } else {
        setTimer(15000);
    }
    m_iSysStep = 0;
    setBadStatusEffect(1, 0.0f);
    m_ullBtlEnemyFlag |= 0x80000000000ULL;
}
void CBattleEnemy::stateFaint() {
    if (m_pEnemyParam) {
        switch (m_iSysStep) {
        case 0:
            if (isMotionEnd()) {
                if (m_pEnemyParam->getSpecialEffect(0) == TRUE) {
                    setTimer(15000);
                } else {
                    setTimer(5000);
                }
                changeMotion(m_pEnemyParam->iFaintMotionIndex[1], 0, 0, -1, 0, 0);
                m_iSysStep++;
            }
            break;
        case 1:
            if (getTimer() <= 0) {
                changeMotion(m_pEnemyParam->iFaintMotionIndex[2], 0, 0, -1, 0, 0);
                m_ullBtlEnemyFlag &= ~0x80000000000ULL;
                killBadStatusEffect();
                m_iSysStep++;
            }
            break;
        case 2:
            if (isMotionEnd()) changeDefaultState();
            break;
        }
    } else if (getTimer() <= 0) {
        changeDefaultState();
    }
}
void CBattleEnemy::endStateFaint() {
    m_ullBtlEnemyFlag &= ~0x80000000000ULL;
    killBadStatusEffect();
}
void CBattleEnemy::beginStateStop() {
    m_ullBackupFlag = m_ullBtlEnemyFlag;
    CCollBody* hitBody = m_pBodyColl->getHitCollBody();
    if (hitBody) {
        switch (hitBody->getDamageKind()) {
        case ATK_KIND_STOP:
        default:
            setBadStatusEffect(4, hitBody->getBadStatusParam());
            break;
        case ATK_KIND_ABSOLUTE_STOP:
            m_ullBtlEnemyFlag |= 0x4000000;
            break;
        }
    } else {
        setBadStatusEffect(4, 0.0f);
    }
    setTimer(m_tqTime);
    m_pAnim->setSpeed(0.0f);
    m_iSysStep = 0;
    m_ullBtlEnemyFlag |= 0x84000;
    m_sStopDamageCount = 0;
    m_sStopDamagePtr = 0;
    setEntityFlag(0x8000, 0);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateStop__12CBattleEnemy);
void CBattleEnemy::endStateStop() {
    setEntityFlag(0, 0x8000);
    m_pAnim->setSpeed(1.0f);
    killBadStatusEffect();
    if (!(m_ullBackupFlag & 0x80000)) m_ullBtlEnemyFlag &= ~0x80000;
    if (!(m_ullBackupFlag & 0x4000)) {
        m_ullBtlEnemyFlag &= ~0x4000;
    } else if (!(m_ullBtlEnemyFlag & 0x4000)) {
        m_ullBtlEnemyFlag |= 0x4000;
    }
    if (!(m_ullBackupFlag & 0x4000000)) {
        m_ullBtlEnemyFlag &= ~0x4000000;
    } else if (!(m_ullBtlEnemyFlag & 0x4000000)) {
        m_ullBtlEnemyFlag |= 0x4000000;
    }
}
void CBattleEnemy::beginStateTerror() {
    CCollBody* hitBody = m_pBodyColl->getHitCollBody();
    if (!hitBody) AssertApp(D_004ACBC0, 0xBA0, D_004AD3D8, 0);
    setTimer(18000);
    setBadStatusEffect(3, hitBody->getBadStatusParam());
    m_ullBtlEnemyFlag |= 0x40000000000ULL;
    FLOAT speed = getExtraParam(_ref(BattleEnemy::SpeedParamName));
    if (speed == 0.0f) speed = 0.05f;
    startTargetEvade(g_pPlayer, speed, 0.1f);
    if (getMotionName(2)) {
        changeMotion(2, 0, 0, -1, 0, 0);
    } else {
        changeMotion(1, 0, 0, -1, 0, 0);
    }
}
void CBattleEnemy::stateTerror() {
    if (getTimer() <= 0) changeDefaultState();
}
void CBattleEnemy::endStateTerror() {
    endTargetEvade();
    m_ullBtlEnemyFlag &= ~0x40000000000ULL;
    killBadStatusEffect();
}
void CBattleEnemy::beginStateFreeze() {
    FVECTOR pos;
    setTimer(24000 / (INT)getExtraParam(BattleEnemy::FreezeParamName));
    m_pAnim->setSpeed(0.0f);
    m_pAnim->setRootMove(FALSE, FALSE);
    m_pMove->enableGravity(TRUE);
    pos.init();
    m_hFreeze = attachEffect(BattleEnemy::FreezeEffectName, 3, &Crd(0), &pos, NULL, 0.0f);
    CEffect* effect = CEffectMan::getEffect(m_hFreeze);
    if (effect) {
        INT id = (UINT)getID() % 10000;
        INT idx = id % 100;
        if (id < 100) {
            effect->setScale(BattleEnemy::g_vIceSizeTbl[BattleEnemy::g_byHeartlessIceSizeIndexTbl[idx]]);
        } else {
            effect->setScale(BattleEnemy::g_vIceSizeTbl[BattleEnemy::g_byBossIceSizeIndexTbl[idx]]);
        }
        FLOAT base = effect->getScale().y < effect->getScale().x ? effect->getScale().x : effect->getScale().y;
        m_pBodyColl->setRadiusScale(base * 0.5f / m_pBodyColl->getLargeRadius());
    }
}
void CBattleEnemy::stateFreeze() {
    if (getTimer() > 0) return;
    CEffect* effect = CEffectMan::getEffect(m_hFreeze);
    if (effect) effect->loopEnd();
    m_hFreeze = 0;
    FVECTOR pos;
    getLockOnPos(pos);
    setEffect(BattleEnemy::FreezeEffectName, 4, &pos, NULL, 0.0f);
    changeDefaultState();
}
void CBattleEnemy::endStateFreeze() {
    FVECTOR pos;
    CEffect* effect = CEffectMan::getEffect(m_hFreeze);
    if (effect) {
        effect->loopEnd();
        getLockOnPos(pos);
        setEffect(BattleEnemy::FreezeEffectName, 5, &pos, NULL, 0.0f);
    }
    m_pAnim->setSpeed(1.0f);
    if (isBtlEnemyFlag(0x100)) m_pMove->enableGravity(FALSE);
    m_pBodyColl->setRadiusScale(1.0f);
    m_hFreeze = 0;
}
void CBattleEnemy::beginStateShock() {
    if (!isBtlEnemyFlag(0x200000000ULL)) {
        setupFlick();
        m_ullBackupFlag = m_pMove->getFlag();
        m_pMove->enableGravity(TRUE);
        m_iSysStep = 0;
    } else {
        changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[0], 0, 0, -1);
        m_iSysStep = 1;
    }
}
void CBattleEnemy::stateShock() {
    switch (m_iSysStep) {
    case 0:
        if (m_pMove->getVel().y < 0.0f && (m_pMove->getCollisionFlag() & 5)) {
            if (m_pAnim->getAnimID() == m_pEnemyParam->iDamageMotionIndex[6]) {
                changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[7], 0, 1, -1);
            } else {
                changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[9], 0, 1, -1);
            }
            clearSpeed();
            m_iSysStep++;
        }
        break;
    case 1:
        if (isMotionEnd()) {
            if (m_pEnemyParam->getSpecialEffect(1) == 1) {
                changeBattleEnemyState(18);
            } else {
                Gauge::CCommonGauge::setDispSeq(getHandle(), 6);
                changeDefaultState();
            }
        }
        break;
    }
}
void CBattleEnemy::endStateShock() {
    if (!(m_ullBackupFlag & 4)) m_pMove->enableGravity(FALSE);
}
void CBattleEnemy::beginStateTornado() {
    if (!m_pEnemyParam) AssertApp(D_004ACBC0, 0xC5B, D_004AD2A0, 0);
    changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[6], 0, 0, -1);
    m_pMove->enableGravity(FALSE);
    m_iSysStep = 0;
    bodyCollOff(-1);
    bodyCollOffBody(-1);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateTornado__12CBattleEnemy);
void CBattleEnemy::endStateTornado() {
    bodyCollOn(-1);
    bodyCollOnBody(-1);
    m_pAnim->setSpeed(1.0f);
    if (isBtlEnemyFlag(0x100)) {
        m_pMove->enableGravity(FALSE);
    } else {
        m_pMove->enableGravity(TRUE);
    }
}
void CBattleEnemy::beginStateDegenerator() {
    FVECTOR pos;
    bodyCollOff(-1);
    bodyCollOffBody(-1);
    getLockOnPos(pos);
    if (!m_pBodyColl->getHitCollBody()) AssertApp(D_004ACBC0, 0xCC3, D_004AD3D8, 0);
    m_hDision = setEffect(BattleEnemy::DisionEffectName, 0, &pos, NULL, 0.0f);
    m_iHp = 0;
    m_pMove->enableGravity(FALSE);
    m_iSysStep = 0;
}
void CBattleEnemy::stateDegenerator() {
    CEffect* effect = CEffectMan::getEffect(m_hDision);
    switch (m_iSysStep) {
    case 0: {
        if (!effect) break;
        FLOAT t = effect->getNowFrame() / (effect->getTotalFrame() * 0.5f);
        if (t >= 1.0f) {
            t = 1.0f;
            setEntityFlag(1, 0);
            m_iSysStep++;
        }
        FVECTOR s2;
        FVECTOR s0;
        FVECTOR s1;
        s0.set(1.0f, 1.0f, 1.0f, 1.0f);
        s1.set(0.0f, 2.0f, 0.0f, 1.0f);
        Math3D::VectorLerp(s2, s0, s1, t);
        setScale(s2);
        break;
    }
    case 1:
        if (effect) break;
        notifyDead();
        switch (m_pBodyColl->getHitCollBody()->getDamageKind()) {
        case ATK_KIND_DEGENERATOR:
        case ATK_KIND_DEGENERATE_BREAK: {
            FVECTOR pos;
            getLockOnPos(pos);
            makePrize(&pos);
            break;
        }
        case ATK_KIND_DEGENERATE:
            if (!isBtlEnemyFlag(0x400000)) {
                FVECTOR pos;
                getLockOnPos(pos);
                makePremiumPrize(&pos);
                makeRoulettePrize(&pos);
                makeCard(&pos);
            }
            break;
        }
        out();
        break;
    }
}
void CBattleEnemy::endStateDegenerator() {}
void CBattleEnemy::beginStateGravity() {
    m_iSysStep = 0;
    setTimer(250);
    bodyCollOff(-1);
    bodyCollOffBody(-1);
    m_pMove->enableGravity(TRUE);
    m_pMove->setMass(5.0f);
}
void CBattleEnemy::stateGravity() {
    switch (m_iSysStep) {
    case 0: {
        FVECTOR scale = getScale();
        scale.y = getTimer() / 250.0f;
        if (scale.y < 0.1f) scale.y = 0.1f;
        setScale(scale);
        if (getTimer() > 0) break;
        setTimer(9000);
        m_iSysStep++;
        break;
    }
    case 1: {
        if (getTimer() > 0) break;
        FVECTOR scale = getScale();
        scale.y = 1.0f;
        setScale(scale);
        changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[0], 0, 0, -1);
        m_iSysStep++;
        break;
    }
    case 2:
        if (isMotionEnd()) changeDefaultState();
        break;
    }
}
void CBattleEnemy::endStateGravity() {
    bodyCollOn(-1);
    bodyCollOnBody(-1);
    if (isBtlEnemyFlag(0x100)) m_pMove->enableGravity(FALSE);
    m_pMove->setMass(1.0f);
}
void CBattleEnemy::beginStateToss() {
    const FVECTOR& enemyPos = m_pBodyColl->getHitCollBody()->getEnemyPos();
    FLOAT dist = calcDistanceXZ(enemyPos);
    getRot().y = calcDirection(enemyPos);
    m_pMove->calcOrbitVelocityFromSpeedY((1.0f - m_pMove->getForce()) * 0.175f + 0.25f, dist);
    m_pMove->enableGravity(TRUE);
    if (!m_pEnemyParam) AssertApp(D_004ACBC0, 0xD87, D_004AD2A0, 0);
    if (checkHitPos() & 1) {
        changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[6], 0, 0, -1);
        getRot().y = calcDirection(enemyPos);
    } else {
        changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[8], 0, 0, -1);
        FVECTOR dir;
        SubVectorXYZ(dir, enemyPos, getPos());
        getRot().y = atan2f(dir.x, dir.z);
    }
    playBigDamageSE();
    m_iSysStep = 0;
    bodyCollOffBody(-1);
}
void CBattleEnemy::stateToss() {
    switch (m_iSysStep) {
    case 0:
        if (m_pMove->getVel().y < 0.0f && (m_pMove->getCollisionFlag() & 5)) {
            if (m_pAnim->getAnimID() == m_pEnemyParam->iDamageMotionIndex[6]) {
                changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[7], 0, 1, -1);
            } else {
                changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[9], 0, 1, -1);
            }
            clearSpeed();
            bodyCollOnBody(-1);
            m_iSysStep++;
        }
        break;
    case 1:
        if (isMotionEnd()) changeDefaultState();
        break;
    }
}
void CBattleEnemy::endStateToss() {
    if (m_ullBtlEnemyFlag & 0x100) m_pMove->enableGravity(FALSE);
    bodyCollOnBody(-1);
}
void CBattleEnemy::beginStateBeat() {
    FVECTOR vel;
    if (checkHitPos() & 1) {
        changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[6], 0, 0, -1);
    } else {
        changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[8], 0, 0, -1);
    }
    vel.x = 0.0f;
    vel.y = -0.2f;
    vel.z = 0.0f;
    vel.w = 1.0f;
    m_pMove->setVel(vel);
    m_pMove->enableGravity(TRUE);
    playBigDamageSE();
    m_iSysStep = 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateBeat__12CBattleEnemy);
void CBattleEnemy::endStateBeat() {
    if (m_ullBtlEnemyFlag & 0x100) m_pMove->enableGravity(FALSE);
}
void CBattleEnemy::beginStateMagnet() {
    m_pMove->enableGravity(FALSE);
    m_iSysStep = 0;
    setTimer(m_tqTime);
}
void CBattleEnemy::stateMagnet() {
    FVECTOR dir = getPlayerPos(0);
    SubVectorXYZ(dir, dir, getPos());
    Normalize(dir, dir);
    if ((INT)getExtraParam(BattleEnemy::MagnetParamName) == 1) {
        ScaleVectorXYZ(dir, dir, 0.1f);
    } else {
        ScaleVectorXYZ(dir, dir, 0.033333335f);
    }
    m_pMove->setVel(dir);
    if (isMotionEnd() && !m_pAnim->isLoop(-1)) {
        if (getMotionName(1)) changeMotion(1, 0, 0, -1, 0, 0);
    }
    if (arriveTarget(getPlayerPos(0), 1.25f)) {
        FVECTOR v;
        v.init();
        m_pMove->setAcc(v);
        m_pMove->setVel(v);
        if (m_iSysStep == 0) {
            setEffect(BattleEnemy::MagnetEffectName, 5, &Crd(1).m_World.getTrans(), NULL, 0.0f);
            m_iSysStep++;
        }
    }
    if (getTimer() <= 0) changeDefaultState();
}
void CBattleEnemy::endStateMagnet() {
    FVECTOR v;
    v.init();
    m_pMove->setAcc(v);
    if (!isBtlEnemyFlag(0x100)) m_pMove->enableGravity(TRUE);
}
void CBattleEnemy::beginStateSuspend() {
    m_pAnim->setSpeed(0.0f);
    m_ullBackupFlag = m_ullBtlEnemyFlag;
    setBtlEnemyFlag(0x84000);
}
void CBattleEnemy::stateSuspend() {
    CTask* cb = CTaskMan::getSysTask(0x16);
    if (!cb) AssertApp(D_004ACBC0, 0xE6B, D_004AD3C0, 0);
    if (!CardBattle::CCardBattle::isSuspend()) changeDefaultState();
}
void CBattleEnemy::endStateSuspend() {
    m_pAnim->setSpeed(1.0f);
    if (!(m_ullBackupFlag & 0x80000)) clearBtlEnemyFlag(0x80000);
    if (!(m_ullBackupFlag & 0x4000)) clearBtlEnemyFlag(0x4000);
}
void CBattleEnemy::beginStateVolley() {
    CCollBody* hitBody = m_pBodyColl->getHitCollBody();
    if (!hitBody) AssertApp(D_004ACBC0, 0xE83, D_004AD3D8, 0);
    const FVECTOR& enemyPos = hitBody->getEnemyPos();
    if (!m_pEnemyParam) AssertApp(D_004ACBC0, 0xE86, D_004AD2A0, 0);
    if (checkHitPos() & 1) {
        changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[6], 0, 0, -1);
        getRot().y = calcDirection(enemyPos);
    } else {
        changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[8], 0, 0, -1);
        FVECTOR dir;
        SubVectorXYZ(dir, enemyPos, getPos());
        getRot().y = atan2f(dir.x, dir.z);
    }
    m_pMove->calcVelocityXZ(0.25f);
    m_pMove->enableGravity(TRUE);
    playBigDamageSE();
    bodyCollOffBody(-1);
    m_iSysStep = 0;
}
void CBattleEnemy::stateVolley() {
    switch (m_iSysStep) {
    case 0:
        if (m_pMove->getCollisionFlag() & 0xA) {
            if (m_pAnim->getAnimID() == m_pEnemyParam->iDamageMotionIndex[6]) {
                changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[7], 0, 1, -1);
            } else {
                changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[9], 0, 1, -1);
            }
            clearSpeed();
            bodyCollOnBody(-1);
            m_iSysStep++;
        }
        break;
    case 1:
        if (isMotionEnd()) changeDefaultState();
        break;
    }
}
void CBattleEnemy::endStateVolley() {
    if (m_ullBtlEnemyFlag & 0x100) m_pMove->enableGravity(FALSE);
    bodyCollOnBody(-1);
}
/* Parked: registers only (3 lines) - the second hit->getDetectPos() (for makePrize) loads m_pCollision into v0 in
 * the original, v1 in ours; tried: dmgType declared/initialized before or after the calls, a pos local, _ref(hit),
 * a second inline (getDitectPos), comma expression, permuter 4 min (5076 candidates, no gain) */
#if 0
void CBattleEnemy::beginStateDead() {
    m_ullBtlEnemyFlag &= ~0x200;
    setEntityFlag(0, 0x10000);
    setEntityFlag(0, 0x7C);
    bodyCollOff(-1);
    m_pMove->enableGravity(TRUE);
    enableBind_(FALSE);
    notifyDead();
    setTimer(1500);
    CCollBody* hit = m_pBodyColl->getHitCollBody();
    if (hit) {
        setDeathEffect(hit->getDetectPos());
        makePrize(hit->getDetectPos());
        INT dmgType = 0;
        if (!isBtlEnemyFlag(0x100000)) {
            switch (hit->getDamageKind()) {
            case ATK_KIND_DMG_BIG:
                dmgType = getSize() < 1;
                break;
            case ATK_KIND_DMG_BLOWN:
                dmgType = 1;
                break;
            }
        }
        if (dmgType) {
            setupFlick();
            playBigDamageSE();
            if (!isBtlEnemyFlag(0x100)) {
                m_iSysStep = 0;
            } else {
                FVECTOR vel = m_pMove->getVel();
                vel.y = 0.0f;
                m_pMove->setVel(vel);
                m_pMove->enableGravity(FALSE);
                m_iSysStep = 1;
            }
        } else {
            changeMotion(selectDamageMotion_(), 0, 0, -1, 0, 0);
            playSmallDamageSE();
            m_iSysStep = 1;
        }
    } else {
        FVECTOR pos;
        getLockOnPos(pos);
        setDeathEffect(&pos);
        makePrize(&pos);
        changeMotion(m_pEnemyParam->iDamageMotionIndex[2], 0, 0, -1, 0, 0);
        playSmallDamageSE();
        m_iSysStep = 1;
    }
    setEntityFlag(0x40000000, 0);
    setupFade(1.0f, 0.0f, 1500, -1);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateDead__12CBattleEnemy);
#endif
void CBattleEnemy::stateDead() {
    if (m_pEnemyParam && m_iSysStep == 0 && m_pMove->getVel().y < 0.0f && (m_pMove->getCollisionFlag() & 5)) {
        clearSpeed();
        m_iSysStep++;
    }
    if (getTimer() <= 0) out();
}
void CBattleEnemy::endStateDead() {}
CBattleEnemyManager* CBattleEnemy::getBattleEnemyManager() const {
    CTask* parent = (CTask*)getParent();
    while (parent) {
        if (parent->getTypeID() == 0x1003) return (CBattleEnemyManager*)parent;
        parent = parent->getParent();
    }
    return NULL;
}
void CBattleEnemy::makePrize(const FVECTOR* inPos) {
    if (!inPos) AssertApp(D_004ACBC0, 0xF4B, D_004AD568, 0);
    if (isBtlEnemyFlag(0x400000)) return;
    makeExpPrize(inPos);
    makePremiumPrize(inPos);
    makeRoulettePrize(inPos);
    makeCard(inPos);
}
void CBattleEnemy::makeExpPrize(const FVECTOR* inPos) {
    PRIZE_PARAM param;
    if (!m_pBasicParam) return;
    param.vPos = *inPos;
    param.fSpeed = 0.25f;
    param.uiFlag = 1;
    param.uiPrizeFlag = 0;
    INT exp = m_iExp;
    for (INT i = 0; i < sizeof(g_PrizeDataTbl) / sizeof(g_PrizeDataTbl[0]); i++) {
        INT num = exp / g_PrizeDataTbl[i].iPoint;
        if (num > 0) {
            param.iNumber = g_PrizeDataTbl[i].iPoint;
            param.PrizeKind = g_PrizeDataTbl[i].PrizeKind;
            for (INT j = 0; j < num; j++) {
                param.vRot.set(DEG2RAD(-70.0f - RND::genrand_real1() * 10.0f), DEG2RAD(RND::genrand_real1() * 360.0f), 0.0f);
                Math3D::LimitPIVector(param.vRot);
                CEntityFactory::getInstance()->createPrize(param, NULL);
            }
        }
        exp %= g_PrizeDataTbl[i].iPoint;
    }
}
void CBattleEnemy::makePremiumPrize(const FVECTOR* inPos) {
    CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
    if (!bem) AssertApp(D_004ACBC0, 0xF9A, D_004AD3B8, 0);
    bem->createPremiumPrize(*inPos, (UINT)getID() % 10000);
}
void CBattleEnemy::makeRoulettePrize(const FVECTOR* inPos) {
    CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
    if (!bem) AssertApp(D_004ACBC0, 0xFA4, D_004AD3B8, 0);
    bem->createRoulettePrize(*inPos);
}
void CBattleEnemy::makeCard(const FVECTOR* inPos) {
    if (!m_pBasicParam) return;
    if (isBtlEnemyFlag(0x20)) return;
    CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
    if (!bem) AssertApp(D_004ACBC0, 0xFB4, D_004AD3B8, 0);
    if (!bem->canCreateEnemyCard()) return;
    if (bem->isRoulettePrizeGet()) return;
    if (bem->isAppearRoulettePrize()) return;
    PRIZE_PARAM param;
    param.vPos = *inPos;
    param.fSpeed = 0.2f;
    param.uiFlag = 1;
    param.uiPrizeFlag = 0;
    param.vRot.set(DEG2RAD(-45.0f - RND::genrand_real1() * 15.0f), DEG2RAD(RND::genrand_real1() * 360.0f), 0.0f);
    Math3D::LimitPIVector(param.vRot);
    if (CMonstroEventBattle::isMonstroEventBattleScene()) {
        param.PrizeKind = 0x26;
        param.iNumber = 4;
    } else {
        CARD_KIND kind = (CARD_KIND)CGameWork::getInt(1, 0xE3);
        if (kind) {
            param.PrizeKind = kind;
            param.iNumber = CGameWork::getInt(1, 0xE4);
            if (CEntityFactory::getInstance()->createPrize(param, NULL)) {
                bem->setCreateCard(TRUE);
                CGameWork::setInt(1, 0xE3, 0);
            }
            return;
        }
        if (canCreateEnemyCard_()) {
            param.PrizeKind = m_CardKind;
            param.iNumber = 0;
        } else {
            if (canBlackroomCard_()) {
                if (CGame::isSoraGame()) {
                    param.PrizeKind = 0xB3;
                } else {
                    param.PrizeKind = 0xAB;
                }
                param.iNumber = RND::genrand_int31() % 10;
            } else {
                CGimmickManager* gm = (CGimmickManager*)CTaskMan::getSysTask(0x11);
                if (!gm) AssertApp(D_004ACBC0, 0x103B, D_004AD570, 0);
                DropPrize drop = gm->DecideMapCardPrize(TRUE);
                param.PrizeKind = drop.Kind;
                param.iNumber = drop.No;
            }
            if (!param.PrizeKind) return;
        }
    }
    if (CEntityFactory::getInstance()->createPrize(param, NULL)) bem->setCreateCard(TRUE);
}
void CBattleEnemy::setBossDeadEffect() {
    m_hDeath = setDeathEffect();
}
void CBattleEnemy::killBossDeadEffect() {
    CEffect* pEffect = CEffectMan::getEffect(m_hDeath);
    if (pEffect) pEffect->kill();
    m_hDeath = 0;
}
/* Parked: 3 lines - the original keeps the hp <= 0 `return 0` as a stub right after the bgtz (the count >= 8 and
 * inParent paths jump to it) and a separate return-0 copy after ApplyMatrix; ours (this goto form) merges the stub
 * into the ApplyMatrix copy; tried: early returns, nested if/else with final return 0, if (hp > 0) {...} return 0,
 * ApplyMatrix path falling to a final return, goto fail (closest), permuter 5 min (5739 candidates, no gain) */
#if 0
HANDLE CBattleEnemy::recover(INT inPoint, const CHAR* inEffectName, INT inEffectNo, const FVECTOR& inPos, const CCoord* inParent) {
    if (m_iHp <= 0) {
    fail:
        return 0;
    }
    CEffectMan::killEffect(m_hRecover, 0.0f);
    m_hRecover = 0;
    if (isCurrentState(18) && m_iSysStep == 0) {
        if (m_sStopDamageCount >= 8) goto fail;
        STOP_DAMAGE* stopDmg = &((STOP_DAMAGE*)m_StopDamage)[m_sStopDamageCount++];
        stopDmg->iDamage = -inPoint;
        stopDmg->dmgKind = ATK_KIND_DMG_SMALL;
        stopDmg->sHitEffectNo = inEffectNo;
        stopDmg->sHitIndex = 0;
        stopDmg->hitEffectObjType = getEntityType();
        FMATRIX m;
        if (inParent) {
            sceVu0InversMatrix(m, inParent->m_World);
            stopDmg->vDetectPos = m.getTrans();
            goto fail;
        }
        stopDmg->vDetectPos = inPos;
        stopDmg->vDetectPos.w = 1.0f;
        sceVu0InversMatrix(m, Crd(0).m_World);
        ApplyMatrix(stopDmg->vDetectPos, m, stopDmg->vDetectPos);
        return 0;
    }
    m_iHp += inPoint;
    if (getHpMax() < m_iHp) m_iHp = getHpMax();
    if (inEffectName) {
        if (inParent) {
            m_hRecover = attachEffect(inEffectName, inEffectNo, inParent, &inPos, NULL, 0.0f);
        } else {
            m_hRecover = setEffect(inEffectName, inEffectNo, &inPos, NULL, 0.0f);
        }
    }
    notifyHP();
    Gauge::CCommonGauge::setDispSeq(getHandle(), 1);
    return m_hRecover;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", recover__12CBattleEnemyiPCciRC7FVECTORPC6CCoord);
#endif
void CBattleEnemy::kill() {
    if (isState(m_EnemyState[30])) return;
    m_iHp = 0;
    m_ullBtlEnemyFlag |= 0x200000000000ULL;
    destroyCard();
    changeBattleEnemyState(30);
}
/* Parked: registers only - after destroyCard() the original loads m_ullBtlEnemyFlag into v1 and the ~1 mask into
 * v0 (ours: v0/v1; in our RTL the -2 constant is set before the load, so it gets the other register); the same ld
 * v1/li v0 pattern follows destroyCard() in endCardBattle and hitTest_; tried: clearBtlEnemyFlag(1), &= ~1 / -2 /
 * ~1ULL / (INT)~1, reversed operands, a flag local, a reference local, setBattleEnemyFlag(0, 1) (the weak (on,
 * off) inline), if (TRUE), permuter 6 min (20513 candidates, no gain) */
#if 0
void CBattleEnemy::attackStateCardDestroy(BOOL inColl) {
    destroyCard();
    clearBtlEnemyFlag(1);
    if (inColl) bodyCollOn(-1);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", attackStateCardDestroy__12CBattleEnemyi);
#endif
void CBattleEnemy::onBio(INT inSubHp) {
    if (m_iHp > 0) {
        if (CSceneEnv::getStopTypeBit() & (1 << getEntityType())) return;
        if (getFlag() & 0x80000000) return;
        if (isCurrentState(18)) return;
        m_iHp -= inSubHp;
        if (m_iHp <= 0) m_iHp = 1;
        notifyHP();
    }
}
void CBattleEnemy::onGift(INT inAddHp) {
    if (m_iHp > 0) {
        FVECTOR pos;
        getLockOnPos(pos);
        recover(inAddHp, D_004AD578, 0, pos, NULL);
    }
}
void CBattleEnemy::setBadStatusEffect(INT inNo, FLOAT inWait) {
    CEffectMan::killEffect(m_hBadStatus, 0.0f);
    m_hBadStatus = 0;
    m_hBadStatus = CEntity::setBadStatusEffect(_ref(g_BadStatusEffectName), inNo, inWait);
}
void CBattleEnemy::killBadStatusEffect() {
    CEffectMan::killEffect(m_hBadStatus, 0.0f);
    m_hBadStatus = 0;
    if (isBtlEnemyFlag(0x40000000000ULL)) {
        m_hBadStatus = CEntity::setBadStatusEffect(g_BadStatusEffectName, 3, 0.0f);
    } else if (isBtlEnemyFlag(0x80000000000ULL)) {
        m_hBadStatus = CEntity::setBadStatusEffect(g_BadStatusEffectName, 1, 0.0f);
    } else if (isBtlEnemyFlag(0x20000)) {
        if (isBtlEnemyFlag(0x40000) && m_tqConfuseTimer < m_tqBindTimer) {
            m_hBadStatus = CEntity::setBadStatusEffect(g_BadStatusEffectName, 2, 0.0f);
        } else {
            m_hBadStatus = CEntity::setBadStatusEffect(g_BadStatusEffectName, 0, 0.0f);
        }
    } else if (isBtlEnemyFlag(0x40000)) {
        m_hBadStatus = CEntity::setBadStatusEffect(g_BadStatusEffectName, 2, 0.0f);
    }
}
/* Parked: the 30-degree constant (0.5235988f) is loaded once into $f20 and reused for sinf, cosf and
 * calcOrbitSpeedFromDistance in ours; the original reloads it (lui/ori/mtc1) before each call and keeps the sinf
 * result in $f20 instead (vel.y stored after cosf); tried: field stores vs vel.set(), DEG2RAD(30.0f) inline,
 * double literal, PI / 6, sin result in a local, speed local, permuter 6 min (11226 candidates, no gain) */
#if 0
void CBattleEnemy::setupFlick() {
    FVECTOR vel;
    if (!m_pEnemyParam) return;
    if (checkHitPos() & 1) {
        changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[6], 0, 0, -1);
        m_pMove->getRot().y = calcDirection(g_pPlayer);
        vel.set(0.0f, sinf(0.5235988f), -cosf(0.5235988f));
        m_pMove->calcVelocity(m_pMove->calcOrbitSpeedFromDistance(0.5235988f, 4.0f), vel);
    } else {
        changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[8], 0, 0, -1);
        m_pMove->getRot().y = g_pPlayer->calcDirection(*this);
        m_pMove->calcOrbitVelocityFromDistance(0.5235988f, 4.0f);
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setupFlick__12CBattleEnemy);
#endif
void CBattleEnemy::commonBeginAttack() {
    bodyCollOff(-1);
}
void CBattleEnemy::commonEndAttack() {
    m_ullBtlEnemyFlag &= ~5;
    bodyCollOn(-1);
}
void CBattleEnemy::notifyDead() {
    if (isBtlEnemyFlag(0x40000000)) return;
    CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
    if (!bem) AssertApp(D_004ACBC0, 0x116A, D_004AD3B8, 0);
    bem->notifyDead(getHandle());
    m_ullBtlEnemyFlag |= 0x40000000;
}
void CBattleEnemy::setupHitBack() {
    if (isBtlEnemyFlag(0x8000)) return;
    FVECTOR dir;
    CCollBody* hit = m_pBodyColl->getHitCollBody();
    CTask* task;
    if (hit && (task = CTaskMan::getTask(hit->getEnemyHandle()))) {
        dir = task->Crd(0).m_World.getTrans();
    } else {
        dir = getPlayerPos(0);
    }
    SubVector(dir, getPos(), dir);
    dir.y = 0.0f;
    Normalize(dir, dir);
    ScaleVectorXYZ(dir, dir, 0.15f);
    FVECTOR vel = m_pMove->getVel();
    vel.x = dir.x;
    vel.z = dir.z;
    m_pMove->setHitBackVel(vel);
    m_tqHitBackTimer = 500;
    m_vHitBackVel = vel;
}
/* Parked: 34 lines - the Riku-game scan loop: the original places the getRsrcData() ternary before the inline
 * getExtraParam's found block and keeps the parameter in s0 (ours: s2, blocks swapped); the rest (seek loops,
 * relocation, floor mapping, basic param) matches; tried: while/for/continue loop forms, value local, implicit
 * != 0 test, function-scope param, param = NULL placement, combined !param || !setup test, permuter 5 min
 * (6235 candidates, no gain) */
#if 0
BOOL CBattleEnemy::initEnemyParam(INT inGroupID, INT inRsrcID, INT inIndex) {
    INT start = 0;
    if (CGame::isRikuGame()) {
        INT count = 0;
        CRsrcDB::initSeekRsrc(0x15, inGroupID * 10000 + inRsrcID);
        CRsrcData* rsrc;
        while ((rsrc = CRsrcDB::seekNextRsrc())) {
            ENEMY_PARAM* param = (ENEMY_PARAM*)rsrc->getRsrcData();
            if (!param) {
                count++;
                continue;
            }
            _setupEnemyParam(param);
            if (param->getExtraParam(BattleEnemy::RikuParamName) != 0.0f) {
                start = count;
                break;
            }
            count++;
        }
    }
    ENEMY_PARAM* param = NULL;
    CRsrcDB::initSeekRsrc(0x15, inGroupID * 10000 + inRsrcID);
    for (INT i = 0; i < start; i++) CRsrcDB::seekNextRsrc();
    for (INT i = 0; i < inIndex + 1; i++) {
        CRsrcData* rsrc = CRsrcDB::seekNextRsrc();
        if (!rsrc) return FALSE;
        param = (ENEMY_PARAM*)rsrc->getRsrcData();
    }
    if (!param || !_setupEnemyParam(param)) return FALSE;
    m_pEnemyParam = param;
    SHORT floor = CSceneInfo::getCurrentFloorNumber();
    if (floor >= 0) {
        switch (floor) {
        case 15:
            floor = 2;
            break;
        case 14:
        case 100:
            floor = 0;
            break;
        default:
            floor--;
            if (floor < 0) floor = 0;
            break;
        }
    } else {
        floor += 12;
    }
    if (!(0 <= floor && floor < 13)) AssertApp(D_004ACBC0, 0x1203, D_004AD588, 0);
    m_pBasicParam = _getBasicParam(param, floor);
    m_iHp = BattleEnemy::GameModeHp(m_pBasicParam->iHP);
    m_iExp = m_pBasicParam->iExp;
    if (DebugWork::g_dw.enemy.uiFlag & 0x800) m_iHp = m_iHp * DebugWork::g_dw.enemy.iHpPercent / 100;
    return TRUE;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", initEnemyParam__12CBattleEnemyiii);
#endif
/* Parked: the load of hitBody->m_pCollision (getDetectPos) is scheduled before the vol = -1 store in ours, after
 * it in the original (8 lines: registers a2/v1, order): our vol store is a fixed scalar MEM that cannot alias the
 * varying struct load; `INT vol[1]` matches but the ref says INT vol (same open issue: MapEnemy::hitTest_); tried:
 * no hitBody local (getHitCollBody() twice), separate pos local, `INT vol; vol = -1`, _ref(vol) store/arg,
 * playSE(.., getPanAndVol(..)) args, full Game/Collision/CollBody.h class, if/return getDetectPos, permuter 6 min
 * (9052 candidates, no gain); sched dump: insn r135=[r129+0xc] has no dependence on [$1]=r132 */
#if 0
void CBattleEnemy::playSmallDamageSE() {
    if (isBtlEnemyFlag(0x400000000ULL)) return;
    INT fileID = (INT)getExtraParam(BattleEnemy::FileIDParamName);
    INT seID = m_iDamageCounter ? 1 : 2;
    CCollBody* hitBody = m_pBodyColl->getHitCollBody();
    if (hitBody) {
        INT vol = -1;
        INT pan = CSound::getPanAndVol(*hitBody->getDetectPos(), vol);
        CSound::playSE(fileID * 1000 | seID, vol, pan);
    } else {
        playSEPanAndVol(fileID, seID, 0, NULL, 0);
    }
    m_iDamageCounter ^= 1;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", playSmallDamageSE__12CBattleEnemy);
#endif
/* Parked: the load of hitBody->m_pCollision (getDetectPos) is scheduled before the vol = -1 store in ours, after
 * it in the original (8 lines: registers a2/v1, order): our vol store is a fixed scalar MEM that cannot alias the
 * varying struct load; `INT vol[1]` matches but the ref says INT vol (same open issue: MapEnemy::hitTest_); tried:
 * no hitBody local (getHitCollBody() twice), separate pos local, `INT vol; vol = -1`, _ref(vol) store/arg,
 * playSE(.., getPanAndVol(..)) args, full Game/Collision/CollBody.h class, if/return getDetectPos, permuter 6 min
 * (9052 candidates, no gain); sched dump: insn r135=[r129+0xc] has no dependence on [$1]=r132 */
#if 0
void CBattleEnemy::playBigDamageSE() {
    if (isBtlEnemyFlag(0x400000000ULL)) return;
    INT fileID = (INT)getExtraParam(BattleEnemy::FileIDParamName);
    INT seID = m_iDamageCounter ? 3 : 4;
    CCollBody* hitBody = m_pBodyColl->getHitCollBody();
    if (hitBody) {
        INT vol = -1;
        INT pan = CSound::getPanAndVol(*hitBody->getDetectPos(), vol);
        CSound::playSE(fileID * 1000 | seID, vol, pan);
    } else {
        playSEPanAndVol(fileID, seID, 0, NULL, 0);
    }
    m_iDamageCounter ^= 1;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", playBigDamageSE__12CBattleEnemy);
#endif
/* Not attempted: its switch emits a jump table in .rodata, which needs the unit's data from source (draft:
 * build/progress/Game/Enemy/pdse_draft.c) */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", playDamageSE__12CBattleEnemy8ATK_KIND);
BOOL CBattleEnemy::setupDuelCard() {
    ENEMY_CARD* cards = NULL;
    BOOL result;
    INT cardNum;
    for (INT i = 0; i < m_pEnemyParam->iCardCount; i++) {
        if (m_pEnemyParam->pCards[i].iCardKind == 0xE) {
            cards = &m_pEnemyParam->pCards[i];
            break;
        }
    }
    if (cards) {
        cardNum = cards->bNumber[0];
        result = TRUE;
    } else {
        TRACE(D_004AD638, (UINT)getID() % 10000);
        result = FALSE;
        cardNum = g_iDuelCardNum[(UINT)RND::genrand_int31() % 3];
    }
    for (INT i = 0; i < 10; i++) m_iDuelCardArray[i] = 0;
    if (!g_pPlayer) AssertApp(D_004ACBC0, 0x128E, D_004AD3C8, 0);
    CardBattle::CCardRevolver* revolver = g_pPlayer->getRevolver();
    if (!revolver) return FALSE;
    INT number = 9;
    for (INT i = 0; i < cardNum; i++) {
        for (INT n = number; n >= 0; n--) {
            if (revolver->checkBreakCard(n, 0, TRUE)) {
                m_iDuelCardArray[i] = n;
                number = n - 1;
                if (number < 0) number = 0;
                break;
            }
        }
    }
    shuffleArray(m_iDuelCardArray, cardNum);
    m_iDuelCardNum = cardNum;
    m_iDuelCardPtr = 0;
    CardBattle::CCardBattle* cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
    if (!cb) AssertApp(D_004ACBC0, 0x12AC, D_004AD3C0, 0);
    cb->setDuelBreakCount(0, cardNum);
    return result;
}
BOOL CBattleEnemy::destroyCard() {
    CardBattle::CCardBattle* cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
    if (!cb) AssertApp(D_004ACBC0, 0x12F9, D_004AD3C0, 0);
    if (CardBattle::CCardBattle::getPlayingHandle(2) == getHandle()) {
        cb->destroyEnemyCard(getHandle());
        return TRUE;
    }
    return FALSE;
}
void CBattleEnemy::changeDamageMotion(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr) {
    INT identifyNo = 0;
    if (m_pBodyColl->getHitCollBody()) {
        identifyNo = m_sAttrDmgSeIdentifyNo[m_pBodyColl->getHitCollBody()->getDamageAttr()];
    }
    changeMotion(inIndex, inTq, inRootMove, inGr, 0, identifyNo);
}
void CBattleEnemy::beginStateAppear_() {
    beginStateAppear();
}
void CBattleEnemy::stateAppear_() {
    stateAppear();
}
void CBattleEnemy::endStateAppear_() {
    if (isBtlEnemyFlag(8) || isBtlEnemyFlag(0x20000000000ULL)) killBadStatusEffect();
    if (isBtlEnemyFlag(0x1000000000ULL)) enableConfuse_(TRUE);
    setEntityFlag(0x10000, 0);
    endStateAppear();
    m_ullBtlEnemyFlag &= ~0x20000000008ULL;
    if ((UINT)getID() % 10000 < 100 && (DebugWork::g_dw.enemy.uiFlag & 0x1000)) {
        m_iHp = 0;
        changeBattleEnemyState(30);
    }
}
void CBattleEnemy::beginStateMain_() {
    beginStateMain();
}
void CBattleEnemy::stateMain_() {
    stateMain();
}
void CBattleEnemy::endStateMain_() {
    endStateMain();
}
void CBattleEnemy::beginStateAttack1_() {
    beginStateAttack1();
    commonBeginAttack();
}
void CBattleEnemy::stateAttack1_() {
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE(D_004AD680, getHandle(), getID());
    stateAttack1();
}
void CBattleEnemy::endStateAttack1_() {
    endStateAttack1();
    commonEndAttack();
}
void CBattleEnemy::beginStateAttack2_() {
    beginStateAttack2();
    commonBeginAttack();
}
void CBattleEnemy::stateAttack2_() {
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE(D_004AD680, getHandle(), getID());
    stateAttack2();
}
void CBattleEnemy::endStateAttack2_() {
    endStateAttack2();
    commonEndAttack();
}
void CBattleEnemy::beginStateAttack3_() {
    beginStateAttack3();
    commonBeginAttack();
}
void CBattleEnemy::stateAttack3_() {
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE(D_004AD680, getHandle(), getID());
    stateAttack3();
}
void CBattleEnemy::endStateAttack3_() {
    endStateAttack3();
    commonEndAttack();
}
void CBattleEnemy::beginStateAttack4_() {
    beginStateAttack4();
    commonBeginAttack();
}
void CBattleEnemy::stateAttack4_() {
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE(D_004AD680, getHandle(), getID());
    stateAttack4();
}
void CBattleEnemy::endStateAttack4_() {
    endStateAttack4();
    commonEndAttack();
}
void CBattleEnemy::beginStateAttack5_() {
    beginStateAttack5();
    commonBeginAttack();
}
void CBattleEnemy::stateAttack5_() {
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE(D_004AD680, getHandle(), getID());
    stateAttack5();
}
void CBattleEnemy::endStateAttack5_() {
    endStateAttack5();
    commonEndAttack();
}
void CBattleEnemy::beginStateAttack6_() {
    beginStateAttack6();
    commonBeginAttack();
}
void CBattleEnemy::stateAttack6_() {
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE(D_004AD680, getHandle(), getID());
    stateAttack6();
}
void CBattleEnemy::endStateAttack6_() {
    endStateAttack6();
    commonEndAttack();
}
void CBattleEnemy::beginStateAttack7_() {
    beginStateAttack7();
    commonBeginAttack();
}
void CBattleEnemy::stateAttack7_() {
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE(D_004AD680, getHandle(), getID());
    stateAttack7();
}
void CBattleEnemy::endStateAttack7_() {
    endStateAttack7();
    commonEndAttack();
}
void CBattleEnemy::beginStateAttack8_() {
    beginStateAttack8();
    commonBeginAttack();
}
void CBattleEnemy::stateAttack8_() {
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE(D_004AD680, getHandle(), getID());
    stateAttack8();
}
void CBattleEnemy::endStateAttack8_() {
    endStateAttack8();
    commonEndAttack();
}
void CBattleEnemy::beginStateItem_() {
    beginStateItem();
    commonBeginAttack();
}
void CBattleEnemy::stateItem_() {
    stateItem();
}
void CBattleEnemy::endStateItem_() {
    endStateItem();
    commonEndAttack();
}
void CBattleEnemy::beginStateSmallDamage_() {
    beginStateSmallDamage();
}
void CBattleEnemy::stateSmallDamage_() {
    stateSmallDamage();
}
void CBattleEnemy::endStateSmallDamage_() {
    endStateSmallDamage();
    m_ullBtlEnemyFlag &= ~0x1008;
}
void CBattleEnemy::beginStateBigDamage_() {
    beginStateBigDamage();
}
void CBattleEnemy::stateBigDamage_() {
    stateBigDamage();
}
void CBattleEnemy::endStateBigDamage_() {
    endStateBigDamage();
    m_ullBtlEnemyFlag &= ~8;
}
void CBattleEnemy::beginStateCardBreak_() {
    onAttackCancel();
    beginStateCardBreak();
}
void CBattleEnemy::stateCardBreak_() {
    stateCardBreak();
}
void CBattleEnemy::endStateCardBreak_() {
    endStateCardBreak();
}
void CBattleEnemy::beginStateZero_() {
    onAttackCancel();
    beginStateZero();
}
void CBattleEnemy::stateZero_() {
    stateZero();
}
void CBattleEnemy::endStateZero_() {
    endStateZero();
}
void CBattleEnemy::beginStateDuel_() {
    beginStateDuel();
}
void CBattleEnemy::stateDuel_() {
    stateDuel();
}
void CBattleEnemy::endStateDuel_() {
    endStateDuel();
    m_ullBtlEnemyFlag &= ~0x100000000ULL;
}
void CBattleEnemy::beginStateDuelSuccess_() {
    beginStateDuelSuccess();
}
void CBattleEnemy::stateDuelSuccess_() {
    stateDuelSuccess();
}
void CBattleEnemy::endStateDuelSuccess_() {
    endStateDuelSuccess();
}
void CBattleEnemy::beginStateFaint_() {
    beginStateFaint();
}
void CBattleEnemy::stateFaint_() {
    stateFaint();
}
void CBattleEnemy::endStateFaint_() {
    endStateFaint();
}
void CBattleEnemy::beginStateStop_() {
    beginStateStop();
}
void CBattleEnemy::stateStop_() {
    stateStop();
}
void CBattleEnemy::endStateStop_() {
    endStateStop();
}
void CBattleEnemy::beginStateTerror_() {
    beginStateTerror();
}
void CBattleEnemy::stateTerror_() {
    stateTerror();
}
void CBattleEnemy::endStateTerror_() {
    endStateTerror();
}
void CBattleEnemy::beginStateFreeze_() {
    beginStateFreeze();
}
void CBattleEnemy::stateFreeze_() {
    stateFreeze();
}
void CBattleEnemy::endStateFreeze_() {
    endStateFreeze();
}
void CBattleEnemy::beginStateShock_() {
    beginStateShock();
}
void CBattleEnemy::stateShock_() {
    stateShock();
}
void CBattleEnemy::endStateShock_() {
    endStateShock();
}
void CBattleEnemy::beginStateTornado_() {
    beginStateTornado();
}
void CBattleEnemy::stateTornado_() {
    stateTornado();
}
void CBattleEnemy::endStateTornado_() {
    endStateTornado();
}
void CBattleEnemy::beginStateDegenerator_() {
    beginStateDegenerator();
    addEnemyKillCount_();
}
void CBattleEnemy::stateDegenerator_() {
    stateDegenerator();
}
void CBattleEnemy::endStateDegenerator_() {
    endStateDegenerator();
}
void CBattleEnemy::beginStateGravity_() {
    beginStateGravity();
}
void CBattleEnemy::stateGravity_() {
    stateGravity();
}
void CBattleEnemy::endStateGravity_() {
    endStateGravity();
}
void CBattleEnemy::beginStateToss_() {
    beginStateToss();
}
void CBattleEnemy::stateToss_() {
    stateToss();
}
void CBattleEnemy::endStateToss_() {
    endStateToss();
}
void CBattleEnemy::beginStateBeat_() {
    beginStateBeat();
}
void CBattleEnemy::stateBeat_() {
    stateBeat();
}
void CBattleEnemy::endStateBeat_() {
    endStateBeat();
}
void CBattleEnemy::beginStateMagnet_() {
    beginStateMagnet();
}
void CBattleEnemy::stateMagnet_() {
    stateMagnet();
}
void CBattleEnemy::endStateMagnet_() {
    endStateMagnet();
}
void CBattleEnemy::beginStateSuspend_() {
    beginStateSuspend();
}
void CBattleEnemy::stateSuspend_() {
    stateSuspend();
}
void CBattleEnemy::endStateSuspend_() {
    endStateSuspend();
}
void CBattleEnemy::beginStateVolley_() {
    beginStateVolley();
}
void CBattleEnemy::stateVolley_() {
    stateVolley();
}
void CBattleEnemy::endStateVolley_() {
    endStateVolley();
}
void CBattleEnemy::beginStateDead_() {
    beginStateDead();
    addEnemyKillCount_();
}
void CBattleEnemy::stateDead_() {
    stateDead();
}
void CBattleEnemy::endStateDead_() {
    endStateDead();
}
CEntity* CBattleEnemy::createFriendCard(CARD_KIND inCardKind, INT inCardNumber) {
    PRIZE_PARAM param;
    if (isBtlEnemyFlag(0x2000000000ULL)) return NULL;
    param.iNumber = inCardNumber;
    param.PrizeKind = inCardKind;
    param.fSpeed = 0.2f;
    param.uiFlag = 1;
    param.uiPrizeFlag = 0;
    getHeadPos(param.vPos);
    param.vPos.y += 2.0f;
    param.vRot.set(DEG2RAD(-70.0f), DEG2RAD(RND::genrand_real1() * 360.0f), 0.0f);
    Math3D::LimitPIVector(param.vRot);
    return CEntityFactory::getInstance()->createPrize(param, NULL);
}
void CBattleEnemy::notifyHP() {
    INT nHp = m_iHp;
    HANDLE hThis = getHandle();
    Gauge::CEnemyGauge::notifyHP(hThis, nHp, getHpMax());
}
/* Not attempted: CStateTemplate::set()'s assert strings (System/Fsm.h) need the unit's .rodata from source (draft:
 * build/progress/Game/Enemy/initFSM_draft.c) */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", initFSM___12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", initRoom___12CBattleEnemy);
void CBattleEnemy::changeBattleEnemyState(INT inState) {
    if (!((UINT)inState < 31)) AssertApp(D_004ACBC0, 0x162B, D_004AD388, 0);
    CState& NewState = *(CState*)((inState * 0x20 + (INT)this) + 0x450);
    clearBtlEnemyFlag(0x10);
    m_pNewState = &NewState;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", hitTest___12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", computeDamage___12CBattleEnemyff8ATK_KIND);
INT CBattleEnemy::computeDamage_(FLOAT inDamage, FLOAT inAttr) {
    INT result = (INT)(inDamage * inAttr * getDamageRate_() / 100.0f);
    if (!isBtlEnemyFlag(0x400) && !isBtlEnemyFlag(0x4000)) {
        if (isBtlEnemyFlag(0x1000000)) {
            CCollBody* hitBody = m_pBodyColl->getHitCollBody();
            if (!hitBody) AssertApp(D_004ACBC0, 0x1846, D_004AD3D8, 0);
            INT bodyHp = hitBody->getHp();
            if (!(DebugWork::g_dw.enemy.uiFlag & 1)) {
                if (!isLastLeave_(bodyHp - result)) {
                    bodyHp -= result;
                } else {
                    result = 0;
                }
            }
            if (result >= 2) {
                CardBattle::CCardBattle* cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
                if (!cb) AssertApp(D_004ACBC0, 0x1864, D_004AD3C0, 0);
                CardBattle::CCardBattle::decEnchantTime(2, 0x27, 0);
                TRACE_USER((USER)5, D_004AD810);
                TRACE_USER((USER)5, D_004AD840, getID());
                TRACE_USER((USER)5, D_004AD858, result);
                TRACE_USER((USER)5, D_004AD870, hitBody->getDamageKind());
                TRACE_USER((USER)5, D_004AD888, hitBody->getDamageAttr());
                TRACE_USER((USER)5, D_004AD358);
            }
            hitBody->setHp(bodyHp);
            if (hitBody->getMaxHp() < bodyHp) hitBody->setHp(hitBody->getMaxHp());
            notifyHP();
            if (m_pBodyColl->getHitCollBodyIndex() == 0) {
                m_iHp = bodyHp;
                if (getHpMax() < m_iHp) m_iHp = getHpMax();
            }
        } else {
            if (!(DebugWork::g_dw.enemy.uiFlag & 1)) {
                if (!isLastLeave_(m_iHp - result)) {
                    m_iHp -= result;
                } else {
                    result = 0;
                }
            }
            if (result >= 2) {
                CardBattle::CCardBattle* cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
                if (!cb) AssertApp(D_004ACBC0, 0x18A3, D_004AD3C0, 0);
                CardBattle::CCardBattle::decEnchantTime(2, 0x27, 0);
                CCollBody* hitBody = m_pBodyColl->getHitCollBody();
                if (!hitBody) AssertApp(D_004ACBC0, 0x18A8, D_004AD3D8, 0);
                TRACE_USER((USER)5, D_004AD810);
                TRACE_USER((USER)5, D_004AD840, getID());
                TRACE_USER((USER)5, D_004AD858, result);
                TRACE_USER((USER)5, D_004AD870, hitBody->getDamageKind());
                TRACE_USER((USER)5, D_004AD888, hitBody->getDamageAttr());
                TRACE_USER((USER)5, D_004AD358);
            }
            if (getHpMax() < m_iHp) m_iHp = getHpMax();
            notifyHP();
        }
    }
    CCollBody* hitBody = m_pBodyColl->getHitCollBody();
    if (!hitBody) AssertApp(D_004ACBC0, 0x18BD, D_004AD3D8, 0);
    setDamageNumber(*hitBody->getDetectPos(), result);
    return result;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", changeDamageState___12CBattleEnemy8ATK_KIND);
BOOL CBattleEnemy::canAttack_(const ENEMY_ACT* inAct) {
    if (!inAct) return FALSE;
    FLOAT viewAngle = inAct->fViewAngle;
    FLOAT dist = inAct->fDistance;
    if (viewAngle != 0.0f) {
        if (dist != 0.0f) {
            if (entityVisibleAxisY(*g_pPlayer, viewAngle) && calcDistanceSq(*g_pPlayer) <= dist * dist) return TRUE;
            return FALSE;
        }
        if (entityVisibleAxisY(*g_pPlayer, viewAngle)) return TRUE;
        return FALSE;
    }
    if (dist != 0.0f) {
        if (calcDistanceSq(*g_pPlayer) <= dist * dist) return TRUE;
        return FALSE;
    }
    return TRUE;
}
INT CBattleEnemy::selectDamageMotion_() {
    INT res = checkHitPos();
    if (isBtlEnemyFlag(0x100)) {
        if (res & 1) {
            if (res & 4) {
                INT index = m_pEnemyParam->iDamageMotionIndex[0];
                if (m_pAnim->getAnimID() == index) {
                    INT newIndex = m_pEnemyParam->iDamageMotionIndex[1];
                    if (newIndex == index) newIndex = m_pEnemyParam->iDamageMotionIndex[2];
                    return newIndex;
                }
                m_iDamageCounter = 0;
                return index;
            } else {
                INT index = m_pEnemyParam->iDamageMotionIndex[1];
                if (m_pAnim->getAnimID() == index) {
                    INT newIndex = m_pEnemyParam->iDamageMotionIndex[0];
                    if (newIndex == index) newIndex = m_pEnemyParam->iDamageMotionIndex[3];
                    return newIndex;
                }
                m_iDamageCounter = 0;
                return index;
            }
        } else {
            if (res & 4) {
                INT index = m_pEnemyParam->iDamageMotionIndex[2];
                if (m_pAnim->getAnimID() == index) {
                    INT newIndex = m_pEnemyParam->iDamageMotionIndex[3];
                    if (newIndex == index) newIndex = m_pEnemyParam->iDamageMotionIndex[0];
                    return newIndex;
                }
                m_iDamageCounter = 0;
                return index;
            } else {
                INT index = m_pEnemyParam->iDamageMotionIndex[3];
                if (m_pAnim->getAnimID() == index) {
                    INT newIndex = m_pEnemyParam->iDamageMotionIndex[2];
                    if (newIndex == index) newIndex = m_pEnemyParam->iDamageMotionIndex[1];
                    return newIndex;
                }
                m_iDamageCounter = 0;
                return index;
            }
        }
    } else if (res & 1) {
        if (m_pMove->getCollisionFlag() & 5) {
            if (res & 4) {
                INT index = m_pEnemyParam->iDamageMotionIndex[0];
                if (m_pAnim->getAnimID() == index) {
                    INT newIndex = m_pEnemyParam->iDamageMotionIndex[1];
                    if (newIndex == index) newIndex = m_pEnemyParam->iDamageMotionIndex[2];
                    return newIndex;
                }
                m_iDamageCounter = 0;
                return index;
            } else {
                INT index = m_pEnemyParam->iDamageMotionIndex[1];
                if (m_pAnim->getAnimID() == index) {
                    INT newIndex = m_pEnemyParam->iDamageMotionIndex[0];
                    if (newIndex == index) newIndex = m_pEnemyParam->iDamageMotionIndex[3];
                    return newIndex;
                }
                m_iDamageCounter = 0;
                return index;
            }
        } else {
            INT index = m_pEnemyParam->iDamageMotionIndex[4];
            if (m_pAnim->getAnimID() == index) return m_pEnemyParam->iDamageMotionIndex[5];
            m_iDamageCounter = 0;
            return index;
        }
    } else {
        if (m_pMove->getCollisionFlag() & 5) {
            if (res & 4) {
                INT index = m_pEnemyParam->iDamageMotionIndex[2];
                if (m_pAnim->getAnimID() == index) {
                    INT newIndex = m_pEnemyParam->iDamageMotionIndex[3];
                    if (newIndex == index) newIndex = m_pEnemyParam->iDamageMotionIndex[0];
                    return newIndex;
                }
                m_iDamageCounter = 0;
                return index;
            } else {
                INT index = m_pEnemyParam->iDamageMotionIndex[3];
                if (m_pAnim->getAnimID() == index) {
                    INT newIndex = m_pEnemyParam->iDamageMotionIndex[2];
                    if (newIndex == index) newIndex = m_pEnemyParam->iDamageMotionIndex[1];
                    return newIndex;
                }
                m_iDamageCounter = 0;
                return index;
            }
        } else {
            INT index = m_pEnemyParam->iDamageMotionIndex[5];
            if (m_pAnim->getAnimID() == index) return m_pEnemyParam->iDamageMotionIndex[4];
            m_iDamageCounter = 0;
            return index;
        }
    }
}
void CBattleEnemy::enableBind_(BOOL inSw) {
    if (inSw) {
        CCollBody* hitBody = m_pBodyColl->getHitCollBody();
        if (!hitBody) AssertApp(D_004ACBC0, 0x1B8A, D_004AD3D8, 0);
        setEntityFlag(0x8000, 0);
        m_ullBtlEnemyFlag |= 0x20000;
        setBadStatusEffect(0, hitBody->getBadStatusParam());
        m_pMove->setFlag(8, TRUE);
        m_tqBindTimer = 33000;
        setEffect(BattleEnemy::BindEffectName, 1, &getPos(), NULL, hitBody->getBadStatusParam());
    } else {
        setEntityFlag(0, 0x8000);
        m_ullBtlEnemyFlag &= ~0x20000;
        killBadStatusEffect();
        m_pMove->setFlag(8, FALSE);
        m_tqBindTimer = 0;
    }
}
void CBattleEnemy::enableConfuse_(BOOL inSw) {
    FMATRIX m;
    FVECTOR start;
    if (inSw) {
        CCollBody* hitBody = m_pBodyColl->getHitCollBody();
        if (hitBody) {
            setBadStatusEffect(2, hitBody->getBadStatusParam());
        } else {
            setBadStatusEffect(2, 0.0f);
        }
        m_ullBtlEnemyFlag |= 0x40000;
        m_tqConfuseTimer = 21000;
        _unitMatrixVu0(m);
        sceVu0RotMatrixY(m, m, Math3D::LimitPI(DEG2RAD(RND::genrand_real1() * 360.0f)));
        m_vConfusePlayerPos.set(0.0f, 0.0f, RND::genrand_real1() * 3.0f + 2.0f);
        ApplyMatrix(m_vConfusePlayerPos, m, m_vConfusePlayerPos);
        AddVectorXYZ(m_vConfusePlayerPos, m_vConfusePlayerPos, getPos());
        start = getPos();
        CCollMap* coll = m_pMove->getMapCollision();
        coll->setStartPos(start);
        coll->setEndPos(m_vConfusePlayerPos);
        if (coll->collideMap()) m_vConfusePlayerPos = *coll->getOutPos();
    } else {
        m_ullBtlEnemyFlag &= ~0x40000;
        killBadStatusEffect();
        m_tqConfuseTimer = 0;
    }
}
void CBattleEnemy::setGuardEffect_(const FVECTOR* inPos) {
    switch ((UINT)getID() % 10000) {
    case 9:
        setEffect(BattleEnemy::LargebodyGuardEffectEffectName, 15, inPos, NULL, 0.0f);
        break;
    case 114:
        setEffect(BattleEnemy::VexenGuardEffectEffectName, 6, inPos, NULL, 0.0f);
        break;
    default:
        if (getSize() <= 0) {
            setEffect(BattleEnemy::AlwaysEffectName, 10, inPos, NULL, 0.0f);
        } else {
            setEffect(BattleEnemy::AlwaysEffectName, 11, inPos, NULL, 0.0f);
        }
        break;
    }
}
FLOAT CBattleEnemy::getDamageRate_() const {
    return m_fDamageRate;
}
BOOL CBattleEnemy::isLastLeave_(INT inResultHp) {
    CardBattle::CCardBattle* cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
    if (cb && _getEnchantKind(cb, 2) == 0x29 && m_iHp >= 2 && inResultHp <= 0) {
        m_iHp = 1;
        notifyHP();
        CardBattle::CCardBattle::decEnchantTime(2, 0x28, 0x29);
        return TRUE;
    }
    return FALSE;
}
BOOL CBattleEnemy::isReraise_() {
    CardBattle::CCardBattle* cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
    if (cb && _getEnchantKind(cb, 2) == 0x2E) {
        m_iHp = getHpMax() / 4;
        notifyHP();
        Gauge::CCommonGauge::setDispSeq(getHandle(), 1);
        CardBattle::CCardBattle::decEnchantTime(2, 0x28, 0x2E);
        return TRUE;
    }
    return FALSE;
}
/* CCardBattle's m_cardEnchant as the original's CARD[2] (Game/CardBattle.h declares bytes), and its
 * getEnchantKind(INT inUser) inline (1-based user), as in Game/CardRevolver */
BOOL CBattleEnemy::isQuickRecover_() {
    CardBattle::CCardBattle* cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
    if (cb && _getEnchantKind(cb, 2) == 0x2C) {
        setBattleEnemyFlag(0x10, 1);
        return TRUE;
    }
    return FALSE;
}
void CBattleEnemy::enemyCardDash_() {
    CardBattle::CCardBattle* cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
    if (cb && _getEnchantKind(cb, 2) == 0x2D) {
        m_pMove->setPower(1.7f);
    } else {
        m_pMove->setPower(1.0f);
    }
}
void CBattleEnemy::createFriendCard_(INT inRate) {
    CARD card;
    CardBattle::getDropFriendCard(card, inRate);
    if (card.m_nKind) createFriendCard((CARD_KIND)card.m_nKind, card.m_nNumber);
}
void CBattleEnemy::setHitEffect_() {
    if (isBtlEnemyFlag(0x4000000)) return;
    CCollBody* hit = m_pBodyColl->getHitCollBody();
    if (!hit) AssertApp(D_004ACBC0, 0x1C50, D_004AD430, 0);
    switch (hit->getDamageKind()) {
    case ATK_KIND_QUAKE: {
        CCollMap* coll = m_pMove->getMapCollision();
        if (!coll) AssertApp(D_004ACBC0, 0x1C57, D_004AD920, 0);
        FVECTOR pos = getPos();
        coll->setStartPos(pos);
        pos.y -= 1.0f;
        coll->setEndPos(pos);
        if (coll->collideMap()) {
            pos = *coll->getOutPos();
        } else {
            pos.y += 1.0f;
        }
        CEffectMan::setEffect(BattleEnemy::QuakeEffectName, 1, (ObjType::eObjType)hit->getHitEffectObjType(), &pos, NULL, 0.0f);
        break;
    }
    case ATK_KIND_CONFUSE:
    case ATK_KIND_TERROR: {
        FVECTOR pos;
        pos.init();
        CEffectMan::killEffect(m_hHitEffect, 0.0f);
        m_hHitEffect = CEffectMan::attachEffect(hit->getHitEffectName(), hit->getHitEffectNo(),
                                                (ObjType::eObjType)hit->getHitEffectObjType(), &Crd(1), &pos, NULL,
                                                hit->getBadStatusParam());
        CEffect* effect = CEffectMan::getEffect(m_hHitEffect);
        if (effect) effect->m_iFlag |= 4;
        break;
    }
    default:
        hit->setHitEffect();
        break;
    }
}
void CBattleEnemy::cardBattleProc_() {
    if (!isBtlEnemyFlag(0x800000000ULL) && isBtlEnemyFlag(0x10) && m_iHp > 0 && m_tqCancelTimer <= 0 &&
        !(DebugWork::g_dw.enemy.uiFlag & 0x400)) {
        if (isBtlEnemyFlag(0x800)) {
            startCardBattle();
            return;
        }
        if (DebugWork::g_dw.enemy.uiFlag & 0x10) return;
        if (isBtlEnemyFlag(1)) return;
        aiCardBattle();
        return;
    }
    m_ullBtlEnemyFlag &= ~0x800;
}
void CBattleEnemy::reviseObject_() {
    FVECTOR result;
    FVECTOR pos;
    FVECTOR v;
    FLOAT len;
    if (!m_pBodyColl) return;
    if (m_pBodyColl->collideBody(&result, NULL, NULL)) {
        pos = getPos();
        setOldPos(pos);
        SubVectorXYZ(v, result, pos);
        v.y = 0.0f;
        len = Math3D::VectorLength(v);
        Normalize(v, v);
        ScaleVectorXYZ(v, v, len * m_fBodyRviseCoefficient);
        AddVectorXYZ(pos, pos, v);
        setPos(pos);
        m_ullBtlEnemyFlag |= 0x800000;
    } else {
        m_ullBtlEnemyFlag &= ~0x800000;
    }
}
void CBattleEnemy::updateBindTimer_() {
    if (m_tqBindTimer > 0) {
        m_tqBindTimer -= getFrameRate();
        if (m_tqBindTimer <= 0) enableBind_(FALSE);
    }
}
void CBattleEnemy::updateConfuseTimer_() {
    if (m_tqConfuseTimer > 0) {
        m_tqConfuseTimer -= getFrameRate();
        if (m_tqConfuseTimer <= 0) enableConfuse_(FALSE);
    }
}
void CBattleEnemy::updateHitBackTimer_() {
    if (m_tqHitBackTimer > 0) {
        m_tqHitBackTimer -= getFrameRate();
        if (m_tqHitBackTimer <= 0) {
            FVECTOR v;
            v.init();
            m_pMove->setHitBackVel(v);
        } else {
            m_pMove->setHitBackVel(m_vHitBackVel);
        }
    }
}
void CBattleEnemy::updateDamageTimer_() {
    if (m_tqDamageTimer > 0) {
        m_tqDamageTimer -= getFrameRate();
        if (m_tqDamageTimer <= 0) {
            m_iHitCount = 0;
            m_fDamageRate = 1.0f;
        }
    }
}
BOOL CBattleEnemy::isNowPlayDamageMotion_() {
    if (!m_pAnim) AssertApp(D_004ACBC0, 0x1D28, D_004AD928, 0);
    return m_pEnemyParam->isDamageMotion(m_pAnim->getAnimID());
}
BOOL CBattleEnemy::addEnemyKillCount_() {
    if (isBtlEnemyFlag(0x200000000000ULL)) return FALSE;
    CJiminyMemoFlag::checkEvent(3, (INT)getExtraParam(BattleEnemy::MemoIDParamName), 1);
    if ((UINT)getID() / 10000 != 12) return FALSE;
    INT id = (UINT)getID() % 10000;
    if (id >= 200) return FALSE;
    switch (id) {
    case 0x25:
    case 0x7D:
    case 0x7E:
    case 0x85:
    case 0x87:
    case 0x88:
    case 0x89:
    case 0x8A:
    case 0x8B:
    case 0xD1:
        return FALSE;
    }
    INT count = CGameWork::getInt(0, 0xB3) + 1;
    if (count > 99999) count = 99999;
    CGameWork::setInt(0, 0xB3, count);
    return TRUE;
}
BOOL CBattleEnemy::changeSmallDamageState_(ATK_KIND inDmgKind) {
    if (!isBtlEnemyFlag(0x200000)) {
        changeBattleEnemyState(11);
        return TRUE;
    }
    playDamageSE(inDmgKind);
    return FALSE;
}
void CBattleEnemy::updateSpotLight_() {
    CEffect* effect = CEffectMan::getEffect(m_hSpotLight[1]);
    if (!effect) return;
    FVECTOR n = m_pMove->getDetectNormal();
    FMATRIX m;
    _unitMatrixVu0(m);
    sceVu0RotMatrixX(m, m, atan2f(_sqrtf(n.z * n.z + n.x * n.x), n.y));
    sceVu0RotMatrixY(m, m, atan2f(n.x, n.z));
    FVECTOR pos = getPos();
    pos.w = 1.0f;
    m.getTrans() = pos;
    effect->getCoords()->Crd(0).m_Local = m;
    effect->getCoords()->Crd(0).m_Flag = 1;
}
BOOL CBattleEnemy::canCreateEnemyCard_() {
    if (CGame::isRikuGame()) return FALSE;
    if (CGameWork::getFlag(1, 7)) return FALSE;
    CARD_KIND kind = m_CardKind;
    if (kind == 0x59) return FALSE;
    if (!CCard::canAddCard((USHORT)kind, 0)) return FALSE;
    if (CCard::m_pCardInfo->_nMapTotal < 5) return FALSE;
    if (DebugWork::g_dw.enemy.uiFlag & 0x2000) return TRUE;
    INT rate = m_pEnemyParam->uiEnemyCardDrop;
    RoomInfo* room = CSceneInfo::getRoomInfo(-1);
    if (room) {
        switch (room->getCardID()) {
        case 0xA2:
        case 0xA4:
        case 0xA6:
            rate = (INT)(rate * 2.5f);
            break;
        }
    }
    return RND::genrand_int31() % 100 < rate;
}
void CBattleEnemy::continuousDamageProc_() {
    if ((UINT)getID() % 10000 - 100 < 100) {
        if (++m_iHitCount >= 6) {
            m_fDamageRate -= 0.25f;
            if (m_fDamageRate < 0.05f) m_fDamageRate = 0.05f;
        }
        m_tqDamageTimer = 1500;
    }
}
BOOL CBattleEnemy::canBlackroomCard_() {
    if (CCard::isMapCardFull()) return FALSE;
    RoomInfo* room = CSceneInfo::getRoomInfo(-1);
    if (!room) return FALSE;
    if (room->getCardID() != 0xA9) return FALSE;
    UINT id = (UINT)getID() % 10000;
    if (id != 17 && id != 35) return FALSE;
    if (CGame::isSoraGame() && CSceneInfo::getCurrentWorldID() == 1) return FALSE;
    return TRUE;
}
BOOL CBattleEnemy::checkResist_(ATK_KIND inDmgKind) {
    BOOL resist = FALSE;
    switch (inDmgKind) {
    case 7:
        resist = !m_pEnemyParam->getSpecialEffect(0);
        break;
    case 10:
        resist = !m_pEnemyParam->getSpecialEffect(4);
        break;
    }
    if (resist) Gauge::CCommonGauge::setDispSeq(getHandle(), 6);
    return resist;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", _$_t14CStateTemplate1Z12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", initalize__12CBattleEnemyii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setBattleEnemyFlag__12CBattleEnemyUlUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setHp__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setGroupNo__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setDefaultState__12CBattleEnemyP6CState);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setFallMotionIndex__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setLandMotionIndex__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setParam__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setBodyRviseCoefficient__12CBattleEnemyf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setAttrDmgSeIdentifyNo__12CBattleEnemyii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setCardBreakSeIdentifyNo__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setExp__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getBattleEnemyFlag__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getHp__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getGroupNo__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getCardKind__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getDuelCardKind__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getParam__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getEnemyParam__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getHpPercent__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getMesCardInfo__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getStopTime__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getExp__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isCardBattle__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isCardBattleFailed__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isAttack__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isStartFaint__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isPermissionCardBattle__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoCreateCard__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isHitAttack__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isReload__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isFly__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isPutCardReady__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoHpSub__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isStartCardBattle__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isCounter__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isInvincible__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isMatchless__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoHitBack__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoPlayDamageMotion__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isBind__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isConfuse__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoDmgResponse__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoBigDamage__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoSmallDmgResponse__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoMakePrize__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isHitBody__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isHitPoints__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isOnDamageEndCardBattle__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoSetHitEffect__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isAlwaysSetCard__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isDamageAir__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isStockAct__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNotifyDead__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isRotMove__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isStartDuel__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoFlick__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoPlayDamageSE__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoCardBattle__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isStartConfuse__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoMakeFriendCard__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoDuelPlayerTurn__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoResponseTornado__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoDmgResponseOnFreeze__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isStartSleep__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isTerror__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isFaint__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoDmgResponseOnMagnet__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoKillCount__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", canFlick__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", changeAttackRate__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", bossDeadDemoCallback__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", bossCardRevolverDestroy__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", bossDeadDemoSpecial__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", bossDeadDemoEnd__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateMain__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateMain__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateMain__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateAttack1__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateAttack1__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateAttack1__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateAttack2__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateAttack2__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateAttack2__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateAttack3__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateAttack3__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateAttack3__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateAttack4__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateAttack4__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateAttack4__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateAttack5__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateAttack5__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateAttack5__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateAttack6__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateAttack6__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateAttack6__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateAttack7__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateAttack7__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateAttack7__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateAttack8__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateAttack8__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateAttack8__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateItem__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateItem__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateItem__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", aiCardBattle__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onGuard__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onDamage__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onAbsorb__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onInvalid__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onGimmickCard__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onCardBreak__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onReflect__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onAttackCancel__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onCardBattleFailed__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getDuelAttackState__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setAttackState__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", canAttack__12CBattleEnemyPC9ENEMY_ACT);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", cancelTimeOver___C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", executeBeginState__t14CStateTemplate1Z12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", executeState__t14CStateTemplate1Z12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", executeEndState__t14CStateTemplate1Z12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", func_002B8070);
typedef char _chk_be[sizeof(CBattleEnemy) == 0x830 ? 1 : -1];
