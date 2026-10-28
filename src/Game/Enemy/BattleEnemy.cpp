#define GAME_ENEMY_BATTLEENEMY_CPP
#include "khtypes.h"
#define BATTLEENEMY_H_STATE_TEMPLATE
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h"
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
/* .rodata starts with the strings of the included headers' inline functions, in this order. */
#include "Game/Enemy/BattleEnemyManager.h"
#include "Game/Collision/CollUtil.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/BattleEnemy.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
/* Placeholder: an empty string follows Str.h's strings in .rodata; which header it comes from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
#include "Game/Enemy/EnemyParam.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/Graphic/Effect/effect.h"
#include "Game/Card.h"
#include "System/Debug.h"
#include "System/Fsm.h"
#include "Game/GameWork.h"
#include "System/TaskMan.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "Game/Bullet/BulletManager.h"
#include "Game/Debug/DebugWork.h"
#include "Game/Movement.h"
#include "Game/Player/Player.h"
#include "System/Coord.h"
#include "Game/CardKind.h"
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

/* PARTIAL: until SOURCE DATA, the unit's copy of the header static BattleEnemy::BadStatusEffectName (its .data
 * comes from the original assembly) */
template <class T> static inline T& _ref(T& x) { return x; }
/* ENEMY_PARAM's inline attribute rate (name unknown) */
static inline FLOAT _getAttrRate(ENEMY_PARAM* inParam, INT inAttr) {
    FLOAT value = 100.0f;
    switch (inAttr) {
    case 0:
        value = inParam->fAttr[0];
        break;
    case 1:
        value = inParam->fAttr[1];
        break;
    case 2:
        value = inParam->fAttr[2];
        break;
    case 3:
        value = inParam->fAttr[3];
        break;
    case 4:
        value = inParam->fAttr[4];
        break;
    }
    return value;
}
struct MES_CARD_INFO {
    CARD m_cardData[7];
    SHORT m_nStock;
    BYTE m_nExtra;
    BYTE m_nFirst;
    BYTE m_nBreak;
    BYTE m_nBonus;
    BYTE m_nWork0;
    BYTE m_nWork1;
};
struct CB_ENCHANT_VIEW {
    BYTE _pad[0x17C];
    CARD m_cardEnchant[2];
};
static inline INT _getEnchantKind(CardBattle::CCardBattle* p, INT inUser) {
    return ((CB_ENCHANT_VIEW*)p)->m_cardEnchant[inUser - 1].m_nKind < 0x44 ? 0 : ((CB_ENCHANT_VIEW*)p)->m_cardEnchant[inUser - 1].m_nKind - 0x43;
}
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
struct PRIZE_DATA {
    INT PrizeKind; /* CARD_KIND */
    INT iPoint;
};
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
FVECTOR getPlayerPos(INT inIndex);
namespace BattleEnemy {
FLOAT GameModeAp(FLOAT inAp);
INT GameModeHp(INT inHp);
}
namespace PlayerCamera {
class CPlayerCamera { /* PARTIAL: Game/Player/PlayerCamera.h is not included here */
public:
    static void setBossCamID(INT inID);
};
}
namespace LockOnManager {
class CLockOnManager { /* PARTIAL: Game/LockOnManager.h is not included here */
public:
    static BOOL registerLockOnObject(HANDLE inHandle);
};
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

namespace BattleEnemy {
CARD_KIND g_HeartlessCardKindTbl[39] = {
    (CARD_KIND)0, (CARD_KIND)68, (CARD_KIND)69, (CARD_KIND)71, (CARD_KIND)72, (CARD_KIND)73, (CARD_KIND)74, (CARD_KIND)75,
    (CARD_KIND)76, (CARD_KIND)70, (CARD_KIND)79, (CARD_KIND)78, (CARD_KIND)85, (CARD_KIND)86, (CARD_KIND)81, (CARD_KIND)77,
    (CARD_KIND)94, (CARD_KIND)95, (CARD_KIND)82, (CARD_KIND)83, (CARD_KIND)84, (CARD_KIND)87, (CARD_KIND)88, (CARD_KIND)89,
    (CARD_KIND)91, (CARD_KIND)92, (CARD_KIND)90, (CARD_KIND)93, (CARD_KIND)80, (CARD_KIND)97, (CARD_KIND)98, (CARD_KIND)96,
    (CARD_KIND)68, (CARD_KIND)81, (CARD_KIND)94, (CARD_KIND)95, (CARD_KIND)96, (CARD_KIND)159, (CARD_KIND)159,
};
CARD_KIND g_BossCardKindTbl[38] = {
    (CARD_KIND)0, (CARD_KIND)0, (CARD_KIND)99, (CARD_KIND)127, (CARD_KIND)128, (CARD_KIND)101, (CARD_KIND)41, (CARD_KIND)104,
    (CARD_KIND)105, (CARD_KIND)106, (CARD_KIND)100, (CARD_KIND)107, (CARD_KIND)108, (CARD_KIND)109, (CARD_KIND)113, (CARD_KIND)102,
    (CARD_KIND)110, (CARD_KIND)112, (CARD_KIND)111, (CARD_KIND)114, (CARD_KIND)151, (CARD_KIND)115, (CARD_KIND)116, (CARD_KIND)116,
    (CARD_KIND)117, (CARD_KIND)105, (CARD_KIND)105, (CARD_KIND)111, (CARD_KIND)111, (CARD_KIND)111, (CARD_KIND)111, (CARD_KIND)112,
    (CARD_KIND)152, (CARD_KIND)117, (CARD_KIND)151, (CARD_KIND)114, (CARD_KIND)151, (CARD_KIND)151,
};
CARD_KIND g_BossDuelCardKindTbl[38] = {
    (CARD_KIND)0, (CARD_KIND)1, (CARD_KIND)99, (CARD_KIND)127, (CARD_KIND)128, (CARD_KIND)101, (CARD_KIND)129, (CARD_KIND)131,
    (CARD_KIND)105, (CARD_KIND)106, (CARD_KIND)100, (CARD_KIND)107, (CARD_KIND)133, (CARD_KIND)109, (CARD_KIND)146, (CARD_KIND)102,
    (CARD_KIND)142, (CARD_KIND)139, (CARD_KIND)137, (CARD_KIND)148, (CARD_KIND)151, (CARD_KIND)153, (CARD_KIND)156, (CARD_KIND)156,
    (CARD_KIND)158, (CARD_KIND)105, (CARD_KIND)105, (CARD_KIND)137, (CARD_KIND)137, (CARD_KIND)137, (CARD_KIND)137, (CARD_KIND)139,
    (CARD_KIND)152, (CARD_KIND)158, (CARD_KIND)151, (CARD_KIND)114, (CARD_KIND)151, (CARD_KIND)151,
};
static CHAR* CamTrgOfsParamName = "CamTrgOfs";
static CHAR* DuelMotionParamName = "DuelMotion";
static CHAR* SpeedParamName = "Speed";
static CHAR* FreezeParamName = "Freeze";
static CHAR* MagnetParamName = "Magnet";
static CHAR* FileIDParamName = "FileID";
static CHAR* SynchroParamName = "Synchro";
static CHAR* MemoIDParamName = "MemoID";
static CHAR* RikuParamName = "riku";
static CHAR* DuelEffectName = "duel";
static CHAR* FreezeEffectName = "ice";
static CHAR* DisionEffectName = "dision";
static CHAR* GuffyEffectName = "guffy";
static CHAR* MagnetEffectName = "magnespiral";
static CHAR* SynchroEffectName = "synchro";
static CHAR* BindEffectName = "bind";
static CHAR* AlwaysEffectName = "always";
static CHAR* QuakeEffectName = "quake";
static CHAR* LargebodyGuardEffectEffectName = "_ex_2050";
static CHAR* VexenGuardEffectEffectName = "vexen";
extern const FVECTOR g_vIceSizeTbl[8];
extern const unsigned char g_byHeartlessIceSizeIndexTbl[39];
extern const unsigned char g_byBossIceSizeIndexTbl[38];
extern const FLOAT g_vSpotSizeTbl[9];
extern const unsigned char g_byHeartlessSpotSizeIndexTbl[39];
extern const unsigned char g_byBossSpotSizeIndexTbl[38];
extern const INT GameModeParamHp[4];
extern const FLOAT GameModeParamAp[4];
}
void (CBattleEnemy::*CBattleEnemy::m_pBeginStateTbl[])() = {
    &CBattleEnemy::beginStateAppear_,
    &CBattleEnemy::beginStateMain_,
    &CBattleEnemy::beginStateAttack1_,
    &CBattleEnemy::beginStateAttack2_,
    &CBattleEnemy::beginStateAttack3_,
    &CBattleEnemy::beginStateAttack4_,
    &CBattleEnemy::beginStateAttack5_,
    &CBattleEnemy::beginStateAttack6_,
    &CBattleEnemy::beginStateAttack7_,
    &CBattleEnemy::beginStateAttack8_,
    &CBattleEnemy::beginStateItem_,
    &CBattleEnemy::beginStateSmallDamage_,
    &CBattleEnemy::beginStateBigDamage_,
    &CBattleEnemy::beginStateCardBreak_,
    &CBattleEnemy::beginStateZero_,
    &CBattleEnemy::beginStateDuel_,
    &CBattleEnemy::beginStateDuelSuccess_,
    &CBattleEnemy::beginStateFaint_,
    &CBattleEnemy::beginStateStop_,
    &CBattleEnemy::beginStateTerror_,
    &CBattleEnemy::beginStateFreeze_,
    &CBattleEnemy::beginStateShock_,
    &CBattleEnemy::beginStateTornado_,
    &CBattleEnemy::beginStateDegenerator_,
    &CBattleEnemy::beginStateGravity_,
    &CBattleEnemy::beginStateToss_,
    &CBattleEnemy::beginStateBeat_,
    &CBattleEnemy::beginStateMagnet_,
    &CBattleEnemy::beginStateSuspend_,
    &CBattleEnemy::beginStateVolley_,
    &CBattleEnemy::beginStateDead_,
};
void (CBattleEnemy::*CBattleEnemy::m_pStateTbl[])() = {
    &CBattleEnemy::stateAppear_,
    &CBattleEnemy::stateMain_,
    &CBattleEnemy::stateAttack1_,
    &CBattleEnemy::stateAttack2_,
    &CBattleEnemy::stateAttack3_,
    &CBattleEnemy::stateAttack4_,
    &CBattleEnemy::stateAttack5_,
    &CBattleEnemy::stateAttack6_,
    &CBattleEnemy::stateAttack7_,
    &CBattleEnemy::stateAttack8_,
    &CBattleEnemy::stateItem_,
    &CBattleEnemy::stateSmallDamage_,
    &CBattleEnemy::stateBigDamage_,
    &CBattleEnemy::stateCardBreak_,
    &CBattleEnemy::stateZero_,
    &CBattleEnemy::stateDuel_,
    &CBattleEnemy::stateDuelSuccess_,
    &CBattleEnemy::stateFaint_,
    &CBattleEnemy::stateStop_,
    &CBattleEnemy::stateTerror_,
    &CBattleEnemy::stateFreeze_,
    &CBattleEnemy::stateShock_,
    &CBattleEnemy::stateTornado_,
    &CBattleEnemy::stateDegenerator_,
    &CBattleEnemy::stateGravity_,
    &CBattleEnemy::stateToss_,
    &CBattleEnemy::stateBeat_,
    &CBattleEnemy::stateMagnet_,
    &CBattleEnemy::stateSuspend_,
    &CBattleEnemy::stateVolley_,
    &CBattleEnemy::stateDead_,
};
void (CBattleEnemy::*CBattleEnemy::m_pEndStateTbl[])() = {
    &CBattleEnemy::endStateAppear_,
    &CBattleEnemy::endStateMain_,
    &CBattleEnemy::endStateAttack1_,
    &CBattleEnemy::endStateAttack2_,
    &CBattleEnemy::endStateAttack3_,
    &CBattleEnemy::endStateAttack4_,
    &CBattleEnemy::endStateAttack5_,
    &CBattleEnemy::endStateAttack6_,
    &CBattleEnemy::endStateAttack7_,
    &CBattleEnemy::endStateAttack8_,
    &CBattleEnemy::endStateItem_,
    &CBattleEnemy::endStateSmallDamage_,
    &CBattleEnemy::endStateBigDamage_,
    &CBattleEnemy::endStateCardBreak_,
    &CBattleEnemy::endStateZero_,
    &CBattleEnemy::endStateDuel_,
    &CBattleEnemy::endStateDuelSuccess_,
    &CBattleEnemy::endStateFaint_,
    &CBattleEnemy::endStateStop_,
    &CBattleEnemy::endStateTerror_,
    &CBattleEnemy::endStateFreeze_,
    &CBattleEnemy::endStateShock_,
    &CBattleEnemy::endStateTornado_,
    &CBattleEnemy::endStateDegenerator_,
    &CBattleEnemy::endStateGravity_,
    &CBattleEnemy::endStateToss_,
    &CBattleEnemy::endStateBeat_,
    &CBattleEnemy::endStateMagnet_,
    &CBattleEnemy::endStateSuspend_,
    &CBattleEnemy::endStateVolley_,
    &CBattleEnemy::endStateDead_,
};
INT CBattleEnemy::m_iDuelCardNum = 0;
INT CBattleEnemy::m_iDuelCardPtr = 0;

INT BattleEnemy::GameModeHp(INT inHp) {
    return inHp * GameModeParamHp[CGameWork::getByte(0, 0x61)] / 100;
}
FLOAT BattleEnemy::GameModeAp(FLOAT inAp) {
    return inAp * GameModeParamAp[CGameWork::getByte(0, 0x61)];
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", __12CBattleEnemyP5CTaskP6CStateT2);
__asm__(".rdata\n.align 3\n.word 0x74614243\n.word 0x45656C74\n.word 0x796D656E\n.word 0x7A697320\n.word 0x203D2065\n.word 0x78257830\n.word 0x0000000A\n.word 0x00000000\n.text\n");
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
BOOL CBattleEnemy::initalize(INT inGroupID, INT inRsrcID, UINT iFlag) {
    if (!CEntity::initalize(inGroupID, inRsrcID, iFlag)) return FALSE;
    if (!initEnemyParam(inGroupID, inRsrcID, m_iParam)) initEnemyParam(inGroupID, inRsrcID, 0);
    if (m_pBodyColl) m_pBodyColl->setMaxHp(m_iHp, -1);
    INT cardIndex = inRsrcID % 100;
    if (inRsrcID / 100 > 0) {
        PlayerCamera::CPlayerCamera::setBossCamID(inRsrcID);
        if (inRsrcID < 200) {
            if ((UINT)cardIndex < sizeof(BattleEnemy::g_BossCardKindTbl) / sizeof(BattleEnemy::g_BossCardKindTbl[0])) {
                m_CardKind = BattleEnemy::g_BossCardKindTbl[cardIndex];
                m_DuelCardKind = BattleEnemy::g_BossDuelCardKindTbl[cardIndex];
            }
        } else {
            m_CardKind = (CARD_KIND)0;
            m_DuelCardKind = (CARD_KIND)0;
        }
    } else {
        if (!(0 <= cardIndex && cardIndex < (sizeof(BattleEnemy::g_HeartlessCardKindTbl) / sizeof(BattleEnemy::g_HeartlessCardKindTbl[0]))))
            AssertApp(__FILE__, 0x2A9, "0 <= cardIndex && cardIndex < (sizeof(BattleEnemy::g_HeartlessCardKindTbl)/sizeof((BattleEnemy::g_HeartlessCardKindTbl)[0]))", 0);
        CARD_KIND kind = BattleEnemy::g_HeartlessCardKindTbl[cardIndex];
        setEntityFlag(0, 0x1000);
        m_CardKind = kind;
        m_DuelCardKind = kind;
    }
    initRoom_();
    switch (inRsrcID) {
    case 9:
    case 10:
        m_pBodyColl->setGuard(0);
        m_pBodyColl->setGuardDir(1.5707964f);
        break;
    case 26:
        m_pBodyColl->setGuard(0);
        m_pBodyColl->setGuard(2);
        m_pBodyColl->setGuard(3);
        m_pBodyColl->setGuardDir(1.5707964f);
        break;
    case 100:
    case 101:
        m_pBodyColl->setGuard(0);
        m_pBodyColl->setGuard(1);
        m_pBodyColl->setGuard(2);
        m_pBodyColl->setGuard(3);
        m_pBodyColl->setGuard(4);
        m_pBodyColl->setGuardDir(3.1415927f);
        break;
    case 108:
        m_pBodyColl->setGuard(0);
        m_pBodyColl->setGuard(1);
        m_pBodyColl->setGuard(2);
        m_pBodyColl->setGuard(3);
        m_pBodyColl->setGuard(4);
        m_pBodyColl->setGuardDir(3.1415927f);
        break;
    }
    switch (inRsrcID) {
    case 37: case 38: case 101: case 103: case 104: case 106: case 107: case 108: case 109: case 111: case 112:
    case 113: case 114: case 116: case 117: case 118: case 119: case 120: case 121: case 124: case 132: case 133:
    case 135: case 136: case 137: case 138: case 139: case 201: case 202: case 203: case 204: case 209:
        break;
    default:
        setEntityFlag(0x20000, 0);
        break;
    }
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    return TRUE;
}
void CBattleEnemy::onMsg(INT inMsg, void* inExtraInfo) {
    if (inMsg == 0x70F0F0F0) setAppearEffect();
    if (m_iHp <= 0) return;
    if (inExtraInfo) *(MES_CARD_INFO*)m_MesCardInfo = *(MES_CARD_INFO*)inExtraInfo;
    switch (inMsg) {
    case 1:
        TRACE("CBattleEnemy::onMsg = CB_MES_START, EnemyID = %d\n", (UINT)getID() % 10000);
        break;
    case 2:
        clearBtlEnemyFlag(0x200);
        if (isPlayingCardBattle()) {
            setBtlEnemyFlag(4);
            if (!(2 <= m_iAttackState && m_iAttackState <= 10)) AssertApp(__FILE__, 0x32E, "BTL_ENEMY_STATE_ATTACK1 <= m_iAttackState && m_iAttackState <= BTL_ENEMY_STATE_ITEM", 0);
            changeBattleEnemyState(m_iAttackState);
        }
        if (!inExtraInfo) return;
        switch (((MES_CARD_INFO*)inExtraInfo)->m_nStock) {
        case 111: case 112: case 113: case 114: case 115: case 116: case 117: case 118: case 119: case 120:
        case 122: case 123: case 124: case 125: case 126: case 127: case 128: case 129: case 130: case 131:
        case 140: case 141: case 142:
            setBtlEnemyFlag(0x20000000);
            break;
        default:
            clearBtlEnemyFlag(0x20000000);
            break;
        }
        TRACE("CBattleEnemy::onMsg = CB_MES_SET, EnemyID = %d\n", (UINT)getID() % 10000);
        break;
    case 3:
        if (!isBtlEnemyFlag(4)) onCardBattleFailed();
        clearBtlEnemyFlag(0x201);
        if (m_pEffect) m_pEffect->kill(10.0f);
        changeBattleEnemyState(0xD);
        TRACE("CBattleEnemy::onMsg = CB_MES_BREAK, EnemyID = %d\n", (UINT)getID() % 10000);
        break;
    case 4:
        clearBtlEnemyFlag(0x201);
        m_tqCancelTimer = 3000;
        onReflect();
        onCardBattleFailed();
        TRACE("CBattleEnemy::onMsg = CB_MES_REFRECT, EnemyID = %d\n", (UINT)getID() % 10000);
        break;
    case 7:
        clearBtlEnemyFlag(0x201);
        if (m_pEffect) m_pEffect->kill(10.0f);
        changeBattleEnemyState(0xE);
        onCardBattleFailed();
        TRACE("CBattleEnemy::onMsg = CB_MES_ZERO, EnemyID = %d\n", (UINT)getID() % 10000);
        break;
    case 6:
        clearBtlEnemyFlag(0x201);
        onGimmickCard();
        TRACE("CBattleEnemy::onMsg = CB_MES_GIMMICK, EnemyID = %d\n", (UINT)getID() % 10000);
        break;
    case 8:
        clearBtlEnemyFlag(0x201);
        if (m_pEffect) m_pEffect->kill(10.0f);
        changeBattleEnemyState(0xF);
        onCardBattleFailed();
        TRACE("CBattleEnemy::onMsg = CB_MES_DUEL, EnemyID = %d\n", (UINT)getID() % 10000);
        break;
    case 9:
        clearBtlEnemyFlag(0x201);
        changeDefaultState();
        TRACE("CBattleEnemy::onMsg = CB_MES_DUEL_CANCEL, EnemyID = %d\n", (UINT)getID() % 10000);
        break;
    case 10:
        setBtlEnemyFlag(0x100000000ULL);
        TRACE("CBattleEnemy::onMsg = CB_MES_DUEL_START, EnemyID = %d\n", (UINT)getID() % 10000);
        break;
    case 11:
        if (isPlayingCardBattle()) {
            INT state = getDuelAttackState();
            if (2 <= state && state < 10) {
                m_iAttackState = state;
                setBtlEnemyFlag(5);
                changeBattleEnemyState(state);
            } else {
                changeDefaultState();
            }
        } else {
            changeDefaultState();
        }
        TRACE("CBattleEnemy::onMsg = CB_MES_DUEL_TOOSLOW, EnemyID = %d\n", (UINT)getID() % 10000);
        break;
    case 12:
        changeBattleEnemyState(0x10);
        TRACE("CBattleEnemy::onMsg = CB_MES_DUEL_SUCCESS, EnemyID = %d\n", (UINT)getID() % 10000);
        break;
    }
}
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
    if (!m_pEnemyParam) AssertApp(__FILE__, 0x414, "m_pEnemyParam", 0);
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
            TRACE_USER((USER)5, "======== Enemy attack infomation ========\n");
            TRACE_USER((USER)5, "ID         = %d\n", getID());
            TRACE_USER((USER)5, "\x8dU\x8c\x82\x97\xcd     = %d\n", param.iAttackPoint);
            TRACE_USER((USER)5, "\x8dU\x8c\x82\x82\xcc\x8e\xed\x97\xde = %d\n", param.AtkKind);
            TRACE_USER((USER)5, "\x91\xae\x90\xab       = %d\n", param.AtkAttr);
            TRACE_USER((USER)5, "\x8bZ\x94\xd4\x8d\x86     = %d\n", act->iActNo);
            TRACE_USER((USER)5, "=========================================\n");
            return param.iAttackPoint;
        }
    }
    return 0.0f;
}
ATK_ATTR CBattleEnemy::getAttackAttr(INT inIndex) const {
    static ATK_ATTR g_AtkAttrTbl[6] = {(ATK_ATTR)0, (ATK_ATTR)1, (ATK_ATTR)2, (ATK_ATTR)3, (ATK_ATTR)4, (ATK_ATTR)5};
    if (!m_pEnemyParam) AssertApp(__FILE__, 0x465, "m_pEnemyParam", 0);
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
    if (!((UINT)inState < 31)) AssertApp(__FILE__, 0x4AF, "0 <= inState && inState < BTL_ENEMY_STATE_MAX", 0);
    CState& State = *(CState*)((inState * 0x20 + (INT)this) + 0x450);
    return isState(State);
}
BOOL CBattleEnemy::isValidSynchro() {
    return (INT)getExtraParam(BattleEnemy::SynchroParamName) != 0;
}
BOOL CBattleEnemy::canCardBattle() {
    if (CardBattle::CCardBattle::isPlayingEnemyCard()) return FALSE;
    CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
    if (!bem) AssertApp(__FILE__, 0x4C8, "bem", 0);
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
    if (!cb) AssertApp(__FILE__, 0x56C, "cb", 0);
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
    if (!g_pPlayer) AssertApp(__FILE__, 0x611, "g_pPlayer", 0);
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
                    if (!hitBody) AssertApp(__FILE__, 0x860, "hitBody", 0);
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
    if (isBtlEnemyFlag(1)) TRACE("waring : BATTLE_ENEMY_FLAG_CARD_BATTLE\x83t\x83\x89\x83O\x82\xaa\x82\xbd\x82\xc1\x82\xc4\x82\xa2\x82\xdc\x82\xb7\n");
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
            if (!cb) AssertApp(__FILE__, 0x96B, "cb", 0);
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
void CBattleEnemy::stateStop() {
    switch (m_iSysStep) {
    case 0:
        if (getTimer() <= 0) {
            if (m_sStopDamageCount > 0) {
                setEntityFlag(0, 0x8000);
                m_ullBtlEnemyFlag &= ~0x4004000;
                m_pAnim->setSpeed(1.0f);
                killBadStatusEffect();
                m_iSysStep = 1;
            } else {
                changeDefaultState();
            }
            break;
        }
        if (!m_pBodyColl) AssertApp(__FILE__, 0xA2D, "m_pBodyColl", 0);
        if (!m_pBodyColl->isHit()) break;
        if (m_pBodyColl->isGuard()) break;
        {
            CCollBody* hit = m_pBodyColl->getHitCollBody();
            if (!hit) AssertApp(__FILE__, 0xA36, "hit", 0);
            switch (hit->getDamageKind()) {
            case ATK_KIND_DMG_SMALL:
            case ATK_KIND_DMG_BIG:
            case ATK_KIND_DMG_BLOWN:
            case ATK_KIND_DMG_TOSS:
            case ATK_KIND_DMG_BEAT:
            case ATK_KIND_GRAVITY:
            case ATK_KIND_TORNADO:
            case ATK_KIND_DRAIN: {
                if (m_sStopDamageCount >= 8) break;
                if (!m_pEnemyParam) AssertApp(__FILE__, 0xA44, "m_pEnemyParam", 0);
                FLOAT attr = _getAttrRate(m_pEnemyParam, hit->getDamageAttr());
                if (attr >= 1000.0f) attr = 100.0f;
                STOP_DAMAGE* stopDmg = &((STOP_DAMAGE*)m_StopDamage)[m_sStopDamageCount];
                if (hit->getDamageKind() == ATK_KIND_GRAVITY) {
                    INT dmg = m_iHp * (FLOAT)m_pBodyColl->getDamage() / 100.0f;
                    stopDmg->iDamage = dmg * attr / 100.0f;
                } else {
                    stopDmg->iDamage = m_pBodyColl->getDamage() * attr / 100.0f;
                }
                stopDmg->dmgKind = hit->getDamageKind();
                stopDmg->vDetectPos = *hit->getDetectPos();
                stopDmg->vDetectPos.w = 1.0f;
                FMATRIX m;
                sceVu0InversMatrix(m, Crd(0).m_World);
                ApplyMatrix(stopDmg->vDetectPos, m, stopDmg->vDetectPos);
                if (hit->getHitEffectNo() >= 0) {
                    if (CGame::isSoraGame()) {
                        strcpy(stopDmg->szHitEffectName, "ex_5010");
                        stopDmg->sHitEffectNo = 0;
                    } else {
                        strcpy(stopDmg->szHitEffectName, "weapon");
                        stopDmg->sHitEffectNo = 12;
                    }
                    stopDmg->sHitIndex = m_pBodyColl->getHitCollBodyIndex();
                    stopDmg->hitEffectObjType = (eObjType)hit->getHitEffectObjType();
                } else {
                    strcpy(stopDmg->szHitEffectName, "");
                    stopDmg->sHitEffectNo = -1;
                    stopDmg->sHitIndex = m_pBodyColl->getHitCollBodyIndex();
                    stopDmg->hitEffectObjType = (eObjType)0;
                }
                m_sStopDamageCount++;
                break;
            }
            case ATK_KIND_DEGENERATOR:
            case ATK_KIND_DEGENERATE:
            case ATK_KIND_DEGENERATE_BREAK:
                if (m_pEnemyParam->getSpecialEffect(2)) changeBattleEnemyState(0x17);
                break;
            }
        }
        break;
    case 1: {
        if (m_sStopDamagePtr >= m_sStopDamageCount) {
            if (isBtlEnemyFlag(0x100) || (m_pMove->getCollisionFlag() & 5)) {
                changeDefaultState();
            } else {
                if (m_iFallMotionIndex != -1) changeMotion(m_iFallMotionIndex, 0, 0, -1, 0, 0);
                m_iSysStep = 3;
            }
            break;
        }
        STOP_DAMAGE* stopDmg = &((STOP_DAMAGE*)m_StopDamage)[m_sStopDamagePtr];
        if (!isBtlEnemyFlag(0x1000000)) {
            if (!(DebugWork::g_dw.enemy.uiFlag & 1)) m_iHp -= stopDmg->iDamage;
            if (getHp() >= getHpMax()) m_iHp = getHpMax();
            notifyHP();
            if (m_iHp <= 0 && !isReraise_()) {
                changeBattleEnemyState(0x1E);
                break;
            }
        } else {
            CCollBody* body = m_pBodyColl->getCollBody(stopDmg->sHitIndex);
            if (!body) AssertApp(__FILE__, 0xABC, "body", 0);
            body->setHp(body->getHp() - stopDmg->iDamage);
            m_pBodyColl->setHitCollBody(body);
            m_pBodyColl->setHitCollBodyIndex(stopDmg->sHitIndex);
            onDamage();
            notifyHP();
            if (stopDmg->sHitIndex == 0) {
                m_iHp -= stopDmg->iDamage;
                if (m_iHp <= 0 && !isReraise_()) {
                    changeBattleEnemyState(0x1E);
                    break;
                }
            } else if (body->getHp() <= 0) {
                m_sStopDamagePtr++;
                break;
            }
        }
        if (stopDmg->iDamage <= 0) {
            if (stopDmg->iDamage < 0) {
                Gauge::CCommonGauge::setDispSeq(getHandle(), 1);
            } else {
                Gauge::CCommonGauge::setDispSeq(getHandle(), 6);
            }
            m_sStopDamagePtr++;
            break;
        }
        CCollBody* hit = m_pBodyColl->getHitCollBody();
        if (!hit) AssertApp(__FILE__, 0xAEE, "hit", 0);
        if (!hit->getCollision()) AssertApp(__FILE__, 0xAEF, "hit->getCollision()", 0);
        FVECTOR rot;
        rot.init();
        ApplyMatrix(stopDmg->vDetectPos, Crd(0).m_World, stopDmg->vDetectPos);
        hit->getCollisionDetect() = stopDmg->vDetectPos;
        hit->setHitEffectParam(stopDmg->szHitEffectName, stopDmg->sHitEffectNo, (ObjType::eObjType)stopDmg->hitEffectObjType, rot, 0.0f);
        INT no = selectDamageMotion_();
        if (no > 0 && !(m_ullBackupFlag & 0x80000) && !((m_ullBackupFlag & 0x200000) && stopDmg->dmgKind == ATK_KIND_DMG_SMALL)) {
            changeMotion(no, 0, 0, -1, 0, 0);
            setTimer(getMotionTotalTime() / 2);
        } else {
            setTimer(750);
        }
        playSmallDamageSE();
        hit->setHitEffect();
        if (stopDmg->dmgKind != ATK_KIND_GRAVITY) {
            m_iSysStep = 2;
        } else {
            m_iSysStep = 5;
            setTimer(250);
            bodyCollOffBody(-1);
            m_pMove->enableGravity(TRUE);
            m_pMove->setMass(5.0f);
            bodyCollOff(-1);
            bodyCollOffBody(-1);
        }
        break;
    }
    case 2:
        if (getTimer() > 0) break;
        m_iSysStep = 1;
        m_sStopDamagePtr++;
        break;
    case 3:
        if (m_pMove->getCollisionFlag() & 5) {
            if (m_iLandMotionIndex != -1) {
                changeMotion(m_iLandMotionIndex, 0, 0, -1, 0, 0);
                clearSpeed();
                m_iSysStep = 4;
            } else {
                changeDefaultState();
            }
        }
        break;
    case 4:
        if (isMotionEnd()) changeDefaultState();
        break;
    case 5: {
        FVECTOR scale = getScale();
        scale.y = getTimer() / 250.0f;
        if (scale.y < 0.1f) scale.y = 0.1f;
        setScale(scale);
        if (getTimer() > 0) break;
        setTimer(9000);
        m_iSysStep = 6;
        break;
    }
    case 6: {
        if (getTimer() > 0) break;
        FVECTOR scale = getScale();
        scale.y = 1.0f;
        setScale(scale);
        changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[0], 0, 0, -1);
        m_iSysStep = 7;
        break;
    }
    case 7:
        if (isMotionEnd()) {
            bodyCollOn(-1);
            bodyCollOnBody(-1);
            if (isBtlEnemyFlag(0x100)) m_pMove->enableGravity(FALSE);
            m_pMove->setMass(1.0f);
            m_iSysStep = 1;
            m_sStopDamagePtr++;
        }
        break;
    }
}
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
    if (!hitBody) AssertApp(__FILE__, 0xBA0, "hitBody", 0);
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
    if (!m_pEnemyParam) AssertApp(__FILE__, 0xC5B, "m_pEnemyParam", 0);
    changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[6], 0, 0, -1);
    m_pMove->enableGravity(FALSE);
    m_iSysStep = 0;
    bodyCollOff(-1);
    bodyCollOffBody(-1);
}
void CBattleEnemy::stateTornado() {
    if (!m_pBodyColl) AssertApp(__FILE__, 0xC65, "m_pBodyColl", 0);
    CCollBody* hitBody = m_pBodyColl->getHitCollBody();
    if (!hitBody) AssertApp(__FILE__, 0xC67, "hitBody", 0);
    switch (m_iSysStep) {
    case 0: {
        CEntity* tornado = (CEntity*)CTaskMan::getTask(hitBody->getEnemyHandle());
        if (!tornado) {
            m_pMove->enableGravity(TRUE);
            m_iSysStep++;
            break;
        }
        getRot().y += 0.34906584f;
        getRot().y = Math3D::LimitPI(getRot().y);
        FVECTOR pos = tornado->getPos();
        FLOAT r = RND::genrand_real1();
        pos.y += 2.0f;
        pos.x += r * cosf(getRot().y);
        pos.z += r * sinf(getRot().y);
        setPos(pos);
        break;
    }
    case 1:
        if (m_pMove->getCollisionFlag() & 5) {
            if (!m_pEnemyParam) AssertApp(__FILE__, 0xC8A, "m_pEnemyParam", 0);
            computeDamage_(m_fDamage, _getAttrRate(m_pEnemyParam, hitBody->getDamageAttr()));
            if (m_iHp <= 0 && !isReraise_()) {
                m_pBodyColl->setHitCollBody(NULL);
                changeBattleEnemyState(30);
                break;
            }
            if (hitBody->getHitEffectNo() != -1) {
                FVECTOR pos;
                getLockOnPos(pos);
                CEffectMan::setEffect(hitBody->getHitEffectName(), hitBody->getHitEffectNo(),
                                      (ObjType::eObjType)hitBody->getHitEffectObjType(), &pos, NULL, 0.0f);
            }
            changeBattleEnemyState(17);
        }
        break;
    }
}
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
    if (!m_pBodyColl->getHitCollBody()) AssertApp(__FILE__, 0xCC3, "hitBody", 0);
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
    if (!m_pEnemyParam) AssertApp(__FILE__, 0xD87, "m_pEnemyParam", 0);
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
void CBattleEnemy::stateBeat() {
    switch (m_iSysStep) {
    case 0:
        if (m_pMove->getCollisionFlag() & 5) {
            if (!m_pEnemyParam) AssertApp(__FILE__, 0xDEA, "m_pEnemyParam", 0);
            CCollBody* hit = m_pBodyColl->getHitCollBody();
            if (!hit) AssertApp(__FILE__, 0xDEC, "hit", 0);
            FLOAT damage = hit->getDamage();
            FLOAT value = _getAttrRate(m_pEnemyParam, hit->getDamageAttr());
            computeDamage_(damage, value, hit->getDamageKind());
            if (m_iHp <= 0) {
                if (!isReraise_()) changeBattleEnemyState(30);
            } else {
                setEffect(BattleEnemy::GuffyEffectName, 4, &getPos(), NULL, 0.0f);
                if (m_pAnim->getAnimID() == m_pEnemyParam->iDamageMotionIndex[6]) {
                    changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[7], 0, 0, -1);
                } else {
                    changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[9], 0, 0, -1);
                }
                m_iSysStep++;
            }
        }
        break;
    case 1:
        if (isMotionEnd()) changeDefaultState();
        break;
    }
}
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
    if (!cb) AssertApp(__FILE__, 0xE6B, "cb", 0);
    if (!CardBattle::CCardBattle::isSuspend()) changeDefaultState();
}
void CBattleEnemy::endStateSuspend() {
    m_pAnim->setSpeed(1.0f);
    if (!(m_ullBackupFlag & 0x80000)) clearBtlEnemyFlag(0x80000);
    if (!(m_ullBackupFlag & 0x4000)) clearBtlEnemyFlag(0x4000);
}
void CBattleEnemy::beginStateVolley() {
    CCollBody* hitBody = m_pBodyColl->getHitCollBody();
    if (!hitBody) AssertApp(__FILE__, 0xE83, "hitBody", 0);
    const FVECTOR& enemyPos = hitBody->getEnemyPos();
    if (!m_pEnemyParam) AssertApp(__FILE__, 0xE86, "m_pEnemyParam", 0);
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
    if (!inPos) AssertApp(__FILE__, 0xF4B, "inPos", 0);
    if (isBtlEnemyFlag(0x400000)) return;
    makeExpPrize(inPos);
    makePremiumPrize(inPos);
    makeRoulettePrize(inPos);
    makeCard(inPos);
}
void CBattleEnemy::makeExpPrize(const FVECTOR* inPos) {
    static PRIZE_DATA g_PrizeDataTbl[7] = {{209, 1400}, {208, 199}, {205, 60}, {207, 30}, {204, 10}, {206, 5}, {203, 1}};
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
    if (!bem) AssertApp(__FILE__, 0xF9A, "bem", 0);
    bem->createPremiumPrize(*inPos, (UINT)getID() % 10000);
}
void CBattleEnemy::makeRoulettePrize(const FVECTOR* inPos) {
    CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
    if (!bem) AssertApp(__FILE__, 0xFA4, "bem", 0);
    bem->createRoulettePrize(*inPos);
}
void CBattleEnemy::makeCard(const FVECTOR* inPos) {
    if (!m_pBasicParam) return;
    if (isBtlEnemyFlag(0x20)) return;
    CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
    if (!bem) AssertApp(__FILE__, 0xFB4, "bem", 0);
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
                if (!gm) AssertApp(__FILE__, 0x103B, "gm", 0);
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
        recover(inAddHp, "cure_all", 0, pos, NULL);
    }
}
void CBattleEnemy::setBadStatusEffect(INT inNo, FLOAT inWait) {
    CEffectMan::killEffect(m_hBadStatus, 0.0f);
    m_hBadStatus = 0;
    m_hBadStatus = CEntity::setBadStatusEffect(_ref(BattleEnemy::BadStatusEffectName), inNo, inWait);
}
void CBattleEnemy::killBadStatusEffect() {
    CEffectMan::killEffect(m_hBadStatus, 0.0f);
    m_hBadStatus = 0;
    if (isBtlEnemyFlag(0x40000000000ULL)) {
        m_hBadStatus = CEntity::setBadStatusEffect(BattleEnemy::BadStatusEffectName, 3, 0.0f);
    } else if (isBtlEnemyFlag(0x80000000000ULL)) {
        m_hBadStatus = CEntity::setBadStatusEffect(BattleEnemy::BadStatusEffectName, 1, 0.0f);
    } else if (isBtlEnemyFlag(0x20000)) {
        if (isBtlEnemyFlag(0x40000) && m_tqConfuseTimer < m_tqBindTimer) {
            m_hBadStatus = CEntity::setBadStatusEffect(BattleEnemy::BadStatusEffectName, 2, 0.0f);
        } else {
            m_hBadStatus = CEntity::setBadStatusEffect(BattleEnemy::BadStatusEffectName, 0, 0.0f);
        }
    } else if (isBtlEnemyFlag(0x40000)) {
        m_hBadStatus = CEntity::setBadStatusEffect(BattleEnemy::BadStatusEffectName, 2, 0.0f);
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
    if (!bem) AssertApp(__FILE__, 0x116A, "bem", 0);
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
    if (!(0 <= floor && floor < 13)) AssertApp(__FILE__, 0x1203, D_004AD588, 0);
    m_pBasicParam = _getBasicParam(param, floor);
    m_iHp = BattleEnemy::GameModeHp(m_pBasicParam->iHP);
    m_iExp = m_pBasicParam->iExp;
    if (DebugWork::g_dw.enemy.uiFlag & 0x800) m_iHp = m_iHp * DebugWork::g_dw.enemy.iHpPercent / 100;
    return TRUE;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", initEnemyParam__12CBattleEnemyiii);
#endif
__asm__(".rdata\n.align 3\n.word 0x3D3C2030\n.word 0x6F6C6620\n.word 0x2620726F\n.word 0x6C662026\n.word 0x20726F6F\n.word 0x4E45203C\n.word 0x5F594D45\n.word 0x41524150\n.word 0x423A3A4D\n.word 0x43495341\n.word 0x58414D5F\n.word 0x00000000\n.word 0x00000000\n.word 0x00000000\n.text\n");
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
void CBattleEnemy::playDamageSE(ATK_KIND inDmgKind) {
    switch (inDmgKind) {
    case ATK_KIND_DMG_BIG:
    case ATK_KIND_THUNDRA:
        if (getSize() > 0) {
            playSmallDamageSE();
        } else {
            playBigDamageSE();
        }
        break;
    case ATK_KIND_DMG_BLOWN:
    case ATK_KIND_QUAKE:
    case ATK_KIND_THUNDAGA:
        playBigDamageSE();
        break;
    default:
        playSmallDamageSE();
        break;
    }
}
BOOL CBattleEnemy::setupDuelCard() {
    static int g_iDuelCardNum[3] = {3, 5, 7};
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
        TRACE("warning : \x83" "f\x83\x85\x83G\x83\x8b\x83J\x81[\x83h\x83p\x83\x89\x83\x81\x81[\x83^\x82\xaa\x90\xdd\x92\xe8\x82\xb3\x82\xea\x82\xc4\x82\xa8\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81" "BID = %d\n", (UINT)getID() % 10000);
        result = FALSE;
        cardNum = g_iDuelCardNum[(UINT)RND::genrand_int31() % 3];
    }
    for (INT i = 0; i < 10; i++) m_iDuelCardArray[i] = 0;
    if (!g_pPlayer) AssertApp(__FILE__, 0x128E, "g_pPlayer", 0);
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
    if (!cb) AssertApp(__FILE__, 0x12AC, "cb", 0);
    cb->setDuelBreakCount(0, cardNum);
    return result;
}
BOOL CBattleEnemy::destroyCard() {
    CardBattle::CCardBattle* cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
    if (!cb) AssertApp(__FILE__, 0x12F9, "cb", 0);
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
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE("warning:\x83J\x81[\x83h\x92\xf1\x8e\xa6\x82\xb5\x82\xc4\x82\xa2\x82\xc8\x82\xa2\x82\xcc\x82\xc9\x8dU\x8c\x82\x82\xb5\x82\xc4\x82\xa2\x82\xdc\x82\xb7\x81I\x81I... handle = %x, ID = %d\n", getHandle(), getID());
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
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE("warning:\x83J\x81[\x83h\x92\xf1\x8e\xa6\x82\xb5\x82\xc4\x82\xa2\x82\xc8\x82\xa2\x82\xcc\x82\xc9\x8dU\x8c\x82\x82\xb5\x82\xc4\x82\xa2\x82\xdc\x82\xb7\x81I\x81I... handle = %x, ID = %d\n", getHandle(), getID());
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
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE("warning:\x83J\x81[\x83h\x92\xf1\x8e\xa6\x82\xb5\x82\xc4\x82\xa2\x82\xc8\x82\xa2\x82\xcc\x82\xc9\x8dU\x8c\x82\x82\xb5\x82\xc4\x82\xa2\x82\xdc\x82\xb7\x81I\x81I... handle = %x, ID = %d\n", getHandle(), getID());
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
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE("warning:\x83J\x81[\x83h\x92\xf1\x8e\xa6\x82\xb5\x82\xc4\x82\xa2\x82\xc8\x82\xa2\x82\xcc\x82\xc9\x8dU\x8c\x82\x82\xb5\x82\xc4\x82\xa2\x82\xdc\x82\xb7\x81I\x81I... handle = %x, ID = %d\n", getHandle(), getID());
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
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE("warning:\x83J\x81[\x83h\x92\xf1\x8e\xa6\x82\xb5\x82\xc4\x82\xa2\x82\xc8\x82\xa2\x82\xcc\x82\xc9\x8dU\x8c\x82\x82\xb5\x82\xc4\x82\xa2\x82\xdc\x82\xb7\x81I\x81I... handle = %x, ID = %d\n", getHandle(), getID());
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
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE("warning:\x83J\x81[\x83h\x92\xf1\x8e\xa6\x82\xb5\x82\xc4\x82\xa2\x82\xc8\x82\xa2\x82\xcc\x82\xc9\x8dU\x8c\x82\x82\xb5\x82\xc4\x82\xa2\x82\xdc\x82\xb7\x81I\x81I... handle = %x, ID = %d\n", getHandle(), getID());
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
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE("warning:\x83J\x81[\x83h\x92\xf1\x8e\xa6\x82\xb5\x82\xc4\x82\xa2\x82\xc8\x82\xa2\x82\xcc\x82\xc9\x8dU\x8c\x82\x82\xb5\x82\xc4\x82\xa2\x82\xdc\x82\xb7\x81I\x81I... handle = %x, ID = %d\n", getHandle(), getID());
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
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE("warning:\x83J\x81[\x83h\x92\xf1\x8e\xa6\x82\xb5\x82\xc4\x82\xa2\x82\xc8\x82\xa2\x82\xcc\x82\xc9\x8dU\x8c\x82\x82\xb5\x82\xc4\x82\xa2\x82\xdc\x82\xb7\x81I\x81I... handle = %x, ID = %d\n", getHandle(), getID());
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
void CBattleEnemy::initFSM_() {
    for (INT i = 0; i < 31; i++) {
        m_EnemyState[i].set(this, m_pBeginStateTbl[i], m_pStateTbl[i], m_pEndStateTbl[i]);
    }
    if (!m_pDefaultState) m_pDefaultState = &m_EnemyState[1];
}
void CBattleEnemy::initRoom_() {
    _MapEnemyRoomInfo* info = CSceneInfo::getCurrentRoomMapEnemyInfo();
    RoomInfo* room;
    if (!info || !(room = CSceneInfo::getRoomInfo(-1))) {
        changeBattleEnemyState(0);
        return;
    }
    switch (info->_byHitPlayerAttack) {
    case 1:
    case 7:
        m_iHp >>= 1;
        if (room->getCardID() == 0xB1) m_iHp = (INT)(m_iHp * 0.2f);
    case 0:
    case 6:
        if (room->getCardID() == 0xAF) createFriendCard_(1000);
        if (info->_byHitPlayerAttack == 6 || info->_byHitPlayerAttack == 7) m_ullBtlEnemyFlag |= 0x1000000000ULL;
        setEntityFlag(0, 0x10000);
        changeBattleEnemyState(0);
        break;
    case 3:
        m_iHp >>= 1;
    case 2:
        if (room->getCardID() == 0xB1) {
            m_iHp = (INT)(m_iHp * 0.2f);
        } else {
            m_iHp = (INT)(m_iHp * 0.9f);
        }
        createFriendCard_(room->getCardID() == 0xAF ? 1000 : -1);
        m_ullBtlEnemyFlag |= 8;
        setEntityFlag(0, 0x10000);
        changeBattleEnemyState(0);
        break;
    case 5:
        m_iHp >>= 1;
        if (room->getCardID() == 0xB1) m_iHp = (INT)(m_iHp * 0.2f);
    case 4:
        changeMotion(1, 500, 0, 0, 0, 0);
        setEntityFlag(0x40000000, 0);
        setupFade(0.0f, 1.0f, 3000, -1);
        Message::CMessageManager::getInstance().sendMessage(150, getHandle(), getHandle(), 0x70F0F0F0, NULL);
        m_tqTime = 12000;
        setEntityFlag(0x10000, 0);
        changeBattleEnemyState(18);
        break;
    case 9:
        m_iHp >>= 1;
        if (room->getCardID() == 0xB1) m_iHp = (INT)(m_iHp * 0.2f);
    case 8:
        m_ullBtlEnemyFlag |= 0x20000000000ULL;
        setEntityFlag(0, 0x10000);
        changeBattleEnemyState(0);
        break;
    }
}
void CBattleEnemy::changeBattleEnemyState(INT inState) {
    if (!((UINT)inState < 31)) AssertApp(__FILE__, 0x162B, "0 <= inState && inState < BTL_ENEMY_STATE_MAX", 0);
    CState& NewState = *(CState*)((inState * 0x20 + (INT)this) + 0x450);
    clearBtlEnemyFlag(0x10);
    m_pNewState = &NewState;
}
BOOL CBattleEnemy::hitTest_() {
    FVECTOR detectPos;
    if (!m_pBodyColl) return FALSE;
    m_pBodyColl->detect();
    if (m_pBodyColl->isGuard() || m_pBodyColl->isHitMuteki()) {
        if (!m_pBodyColl->getHitCollBody()) AssertApp(__FILE__, 0x164A, "m_pBodyColl->getHitCollBody()", 0);
        setGuardEffect_(m_pBodyColl->getHitCollBody()->getDetectPos());
        Gauge::CCommonGauge::setDispSeq(getHandle(), 3);
        onGuard();
        return FALSE;
    }
    if (!m_pBodyColl->isHit()) return FALSE;
    CCollBody* hit = m_pBodyColl->getHitCollBody();
    if (!hit) AssertApp(__FILE__, 0x165D, "hit", 0);
    FLOAT attr;
    if (m_pEnemyParam) {
        attr = _getAttrRate(m_pEnemyParam, hit->getDamageAttr());
    } else {
        attr = 100.0f;
    }
    if (attr >= 1000.0f) {
        setBtlEnemyFlag(8);
        attr = 100.0f;
    }
    if (isBtlEnemyFlag(0x2000)) {
        setGuardEffect_(hit->getDetectPos());
        return FALSE;
    }
    if (isBtlEnemyFlag(0x2000000) && isBtlEnemyFlag(1)) {
        destroyCard();
        clearBtlEnemyFlag(1);
    }
    if (isBtlEnemyFlag(1)) return FALSE;
    FLOAT dmgPoint = m_pBodyColl->getDamage();
    if (m_hFreeze) {
        switch (hit->getDamageKind()) {
        case ATK_KIND_STOP:
        case ATK_KIND_GRAVITY:
        case ATK_KIND_SHOCK:
        case ATK_KIND_MAGNET:
        case ATK_KIND_ABSOLUTE_STOP:
            break;
        default:
            if (attr > 0.0f) {
                INT dmg = (INT)dmgPoint;
                CardBattle::calcFreezeDamage(dmg);
                dmgPoint = dmg;
            }
            break;
        }
    }
    INT damage = computeDamage_(dmgPoint, attr, hit->getDamageKind());
    if (damage <= 0) {
        if (isCurrentState(0x12)) return FALSE;
        if (damage < 0) {
            Gauge::CCommonGauge::setDispSeq(getHandle(), 1);
            if (hit->getDamageKind() == ATK_KIND_RECOVER) {
                if (hit->getHitEffectNo() >= 0) setEffect(hit->getHitEffectName(), hit->getHitEffectNo(), &getPos(), NULL, 0.0f);
            } else {
                onAbsorb();
            }
        } else {
            Gauge::CCommonGauge::setDispSeq(getHandle(), 6);
            onInvalid();
        }
        if (isCurrentState(0x14)) changeDefaultState();
        return FALSE;
    }
    if (!isCurrentState(0x12)) checkResist_(hit->getDamageKind());
    if (DebugWork::g_dw.enemy.uiFlag & 2) m_iHp = 0;
    setHitEffect_();
    if (m_iHp <= 0) {
        if (!isReraise_()) changeBattleEnemyState(0x1E);
        return TRUE;
    }
    continuousDamageProc_();
    createFriendCard_(-1);
    onDamage();
    changeDamageState_(hit->getDamageKind());
    return TRUE;
}
INT CBattleEnemy::computeDamage_(FLOAT inDamage, FLOAT inAttr, ATK_KIND inAtkKind) {
    FLOAT damage = inDamage;
    BOOL special = TRUE;
    CCollBody* hitBody = m_pBodyColl->getHitCollBody();
    if (!hitBody) AssertApp(__FILE__, 0x1703, "hitBody", 0);
    switch (inAtkKind) {
    case ATK_KIND_STOP:
        if (m_pEnemyParam->getSpecialEffect(1)) {
            m_tqTime = (INT)damage;
            return TRUE;
        }
        return FALSE;
    case ATK_KIND_BIND:
        if (!m_pEnemyParam->getSpecialEffect(3)) return FALSE;
        if (!(m_pMove->getCollisionFlag() & 5)) return FALSE;
        break;
    case ATK_KIND_FREEZE:
        if ((INT)getExtraParam(BattleEnemy::FreezeParamName) <= 0) return FALSE;
        break;
    case ATK_KIND_TERROR:
        if (!m_pEnemyParam->getSpecialEffect(5)) return FALSE;
        break;
    case ATK_KIND_GRAVITY:
        if (!m_pEnemyParam->getSpecialEffect(6)) return FALSE;
        damage = m_iHp * inDamage / 100.0f;
        if (m_hFreeze) {
            INT dmg = (INT)damage;
            CardBattle::calcFreezeDamage(dmg);
            damage = dmg;
        }
        break;
    case ATK_KIND_DEGENERATOR:
        if (!m_pEnemyParam->getSpecialEffect(2)) return FALSE;
        if (RND::genrand_int31() & 1) return FALSE;
        break;
    case ATK_KIND_DEGENERATE:
    case ATK_KIND_DEGENERATE_BREAK:
        if (!m_pEnemyParam->getSpecialEffect(2)) return FALSE;
        break;
    case ATK_KIND_SYNCHRO: {
        CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
        if (!bem) AssertApp(__FILE__, 0x176E, "bem", 0);
        INT hp = m_iHp;
        FLOAT wait = 0.0f;
        for (SLContainerT<CTask>* container = bem->getFirstBattleEnemy(); container; container = container->getNext()) {
            CBattleEnemy* enemy = (CBattleEnemy*)container->getValue();
            if (!enemy || enemy == this) continue;
            if (enemy->getTypeID() != 0x1004) continue;
            if (enemy->isValidSynchro()) {
                enemy->m_iHp = hp;
                FVECTOR pos;
                enemy->getLockOnPos(pos);
                enemy->setEffect(BattleEnemy::SynchroEffectName, 3, &pos, NULL, wait);
                wait += 3.0f;
            }
        }
        break;
    }
    case ATK_KIND_TORNADO:
        damage = inDamage * 0.1f;
        break;
    case ATK_KIND_QUAKE:
        if (!(m_pMove->getCollisionFlag() & 5)) return FALSE;
        break;
    case ATK_KIND_MAGNET:
        if ((INT)getExtraParam(BattleEnemy::MagnetParamName)) {
            m_tqTime = (INT)damage;
            return TRUE;
        }
        return FALSE;
    case ATK_KIND_DRAIN: {
        m_iExp = m_iExp * 3 / 4;
        INT addPlayerHp = m_iHp / 8;
        if (addPlayerHp > 20) {
            addPlayerHp = 20;
        } else if (addPlayerHp <= 0) {
            addPlayerHp = 1;
        }
        g_pPlayer->addHP(addPlayerHp, 0, 0);
        break;
    }
    case ATK_KIND_SHOCK:
    case ATK_KIND_ABSOLUTE_STOP:
        m_tqTime = (INT)damage;
        return TRUE;
    case ATK_KIND_SUSPEND:
        return TRUE;
    case ATK_KIND_CONFUSE:
    default:
        special = FALSE;
        break;
    }
    m_fDamage = damage;
    INT result = computeDamage_(damage, inAttr);
    if (result <= 0 && special) result = 1;
    return result;
}
INT CBattleEnemy::computeDamage_(FLOAT inDamage, FLOAT inAttr) {
    INT result = (INT)(inDamage * inAttr * getDamageRate_() / 100.0f);
    if (!isBtlEnemyFlag(0x400) && !isBtlEnemyFlag(0x4000)) {
        if (isBtlEnemyFlag(0x1000000)) {
            CCollBody* hitBody = m_pBodyColl->getHitCollBody();
            if (!hitBody) AssertApp(__FILE__, 0x1846, "hitBody", 0);
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
                if (!cb) AssertApp(__FILE__, 0x1864, "cb", 0);
                CardBattle::CCardBattle::decEnchantTime(2, 0x27, 0);
                TRACE_USER((USER)5, "======== Enemy damage infomation ========\n");
                TRACE_USER((USER)5, "ID             = %d\n", getID());
                TRACE_USER((USER)5, "\x83_\x83\x81\x81[\x83W\x97\xca     = %d\n", result);
                TRACE_USER((USER)5, "\x83_\x83\x81\x81[\x83W\x82\xcc\x8e\xed\x97\xde = %d\n", hitBody->getDamageKind());
                TRACE_USER((USER)5, "\x83_\x83\x81\x81[\x83W\x91\xae\x90\xab   = %d\n", hitBody->getDamageAttr());
                TRACE_USER((USER)5, "=========================================\n");
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
                if (!cb) AssertApp(__FILE__, 0x18A3, "cb", 0);
                CardBattle::CCardBattle::decEnchantTime(2, 0x27, 0);
                CCollBody* hitBody = m_pBodyColl->getHitCollBody();
                if (!hitBody) AssertApp(__FILE__, 0x18A8, "hitBody", 0);
                TRACE_USER((USER)5, "======== Enemy damage infomation ========\n");
                TRACE_USER((USER)5, "ID             = %d\n", getID());
                TRACE_USER((USER)5, "\x83_\x83\x81\x81[\x83W\x97\xca     = %d\n", result);
                TRACE_USER((USER)5, "\x83_\x83\x81\x81[\x83W\x82\xcc\x8e\xed\x97\xde = %d\n", hitBody->getDamageKind());
                TRACE_USER((USER)5, "\x83_\x83\x81\x81[\x83W\x91\xae\x90\xab   = %d\n", hitBody->getDamageAttr());
                TRACE_USER((USER)5, "=========================================\n");
            }
            if (getHpMax() < m_iHp) m_iHp = getHpMax();
            notifyHP();
        }
    }
    CCollBody* hitBody = m_pBodyColl->getHitCollBody();
    if (!hitBody) AssertApp(__FILE__, 0x18BD, "hitBody", 0);
    setDamageNumber(*hitBody->getDetectPos(), result);
    return result;
}
void CBattleEnemy::changeDamageState_(ATK_KIND inDmgKind) {
    if (isBtlEnemyFlag(0x80000)) {
        switch (inDmgKind) {
        case ATK_KIND_FREEZE:
            if (isBtlEnemyFlag(0x10000000000ULL)) {
                changeBattleEnemyState(0x14);
                return;
            }
            break;
        case ATK_KIND_MAGNET:
            if (isBtlEnemyFlag(0x100000000000ULL)) {
                changeBattleEnemyState(0x1B);
                return;
            }
            break;
        case ATK_KIND_ABSOLUTE_STOP:
            changeBattleEnemyState(0x12);
            return;
        }
        playDamageSE(inDmgKind);
        return;
    }
    switch (inDmgKind) {
    case ATK_KIND_DMG_BIG:
    case ATK_KIND_THUNDRA:
        if (getSize() > 0 || isBtlEnemyFlag(0x100000)) {
            changeSmallDamageState_(inDmgKind);
        } else if (!isBtlEnemyFlag(0x200000000ULL)) {
            changeBattleEnemyState(0xC);
        } else {
            changeBattleEnemyState(0xB);
        }
        break;
    case ATK_KIND_DMG_BLOWN:
    case ATK_KIND_QUAKE:
    case ATK_KIND_THUNDAGA:
        if (isBtlEnemyFlag(0x100000)) {
            changeSmallDamageState_(inDmgKind);
        } else if (!isBtlEnemyFlag(0x200000000ULL)) {
            changeBattleEnemyState(0xC);
        } else {
            changeBattleEnemyState(0xB);
        }
        break;
    case ATK_KIND_DMG_TOSS:
        if (isBtlEnemyFlag(0x100000)) {
            changeSmallDamageState_(inDmgKind);
        } else if (!isBtlEnemyFlag(0x200000000ULL)) {
            changeBattleEnemyState(0x19);
        } else {
            changeBattleEnemyState(0xB);
        }
        break;
    case ATK_KIND_DMG_BEAT:
        if (isBtlEnemyFlag(0x100000)) {
            changeSmallDamageState_(inDmgKind);
        } else if (!isBtlEnemyFlag(0x200000000ULL)) {
            changeBattleEnemyState(0x1A);
        } else {
            changeBattleEnemyState(0xB);
        }
        break;
    case ATK_KIND_DMG_VOLLEY:
        if (isBtlEnemyFlag(0x100000)) {
            changeSmallDamageState_(inDmgKind);
        } else if (!isBtlEnemyFlag(0x200000000ULL)) {
            changeBattleEnemyState(0x1D);
        } else {
            changeBattleEnemyState(0xB);
        }
        break;
    case ATK_KIND_STUN:
        if (m_pEnemyParam->getSpecialEffect(0)) {
            changeBattleEnemyState(0x11);
            break;
        }
    case ATK_KIND_DMG_SMALL:
    default:
        changeSmallDamageState_(inDmgKind);
        break;
    case ATK_KIND_STOP:
    case ATK_KIND_ABSOLUTE_STOP:
        changeBattleEnemyState(0x12);
        break;
    case ATK_KIND_BIND:
        enableBind_(TRUE);
        if (isBtlEnemyFlag(0x80000000000ULL) || isBtlEnemyFlag(8) || isBtlEnemyFlag(0x20000000000ULL) || isCurrentState(0x14)) changeDefaultState();
        break;
    case ATK_KIND_CONFUSE:
        if (m_pEnemyParam->getSpecialEffect(4)) {
            enableConfuse_(TRUE);
        } else {
            changeSmallDamageState_(inDmgKind);
        }
        if (isBtlEnemyFlag(0x80000000000ULL) || isBtlEnemyFlag(8) || isBtlEnemyFlag(0x20000000000ULL) || isCurrentState(0x14)) changeDefaultState();
        break;
    case ATK_KIND_TERROR:
        changeBattleEnemyState(0x13);
        break;
    case ATK_KIND_FREEZE:
        changeBattleEnemyState(0x14);
        break;
    case ATK_KIND_DEGENERATOR:
    case ATK_KIND_DEGENERATE:
        changeBattleEnemyState(0x17);
        break;
    case ATK_KIND_DEGENERATE_BREAK:
        if (m_pEnemyParam->getSpecialEffect(2) == 1) {
            changeBattleEnemyState(0x17);
        } else {
            changeBattleEnemyState(0x11);
        }
        break;
    case ATK_KIND_SHOCK:
        changeBattleEnemyState(0x15);
        break;
    case ATK_KIND_TORNADO:
        m_fDamage = m_pBodyColl->getDamage();
        if (isBtlEnemyFlag(0x8000000000ULL) || isBtlEnemyFlag(0x20000)) {
            changeSmallDamageState_(ATK_KIND_DMG_BLOWN);
        } else {
            changeBattleEnemyState(0x16);
        }
        break;
    case ATK_KIND_GRAVITY:
        changeBattleEnemyState(0x18);
        break;
    case ATK_KIND_MAGNET:
        changeBattleEnemyState(0x1B);
        break;
    case ATK_KIND_SUSPEND:
        changeBattleEnemyState(0x1C);
        break;
    }
}
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
        if (!hitBody) AssertApp(__FILE__, 0x1B8A, "hitBody", 0);
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
    if (!hit) AssertApp(__FILE__, 0x1C50, "hit", 0);
    switch (hit->getDamageKind()) {
    case ATK_KIND_QUAKE: {
        CCollMap* coll = m_pMove->getMapCollision();
        if (!coll) AssertApp(__FILE__, 0x1C57, "coll", 0);
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
    if (!m_pAnim) AssertApp(__FILE__, 0x1D28, "m_pAnim", 0);
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
/* The vtables, the CStateTemplate assert string and its vtable: their key functions are still assembly. */
__asm__(".rdata\n.align 3\n.globl _vt$12CBattleEnemy$4CFSM\n_vt$12CBattleEnemy$4CFSM:\n.word 0x0000FFC0\n.word 0\n.word 0x0000FFC0\n.word _$_12CBattleEnemy\n.word 0x0000FFC0\n.word update__12CBattleEnemy\n.word 0\n.word beginStateInitial__4CFSM\n.word 0\n.word stateInitial__4CFSM\n.word 0\n.word endStateInitial__4CFSM\n.word 0x0000FFC0\n.word endStateProc__7CEntity\n.word 0\n.word 0\n.globl _vt$12CBattleEnemy\n_vt$12CBattleEnemy:\n.word 0\n.word 0\n.word 0\n.word _$_12CBattleEnemy\n.word 0\n.word main__12CBattleEnemy\n.word 0\n.word disp__7CEntity\n.word 0\n.word after__5CTask\n.word 0\n.word onOut__12CBattleEnemy\n.word 0\n.word getTypeID__C12CBattleEnemy\n.word 0\n.word initalize__12CBattleEnemyii\n.word 0\n.word initalize__12CBattleEnemyiiUi\n.word 0\n.word AfterInit__7CEntity\n.word 0\n.word onMsg__12CBattleEnemyiPv\n.word 0\n.word getLockOnPos__7CEntityR7FVECTOR\n.word 0\n.word getHeadPos__C7CEntityR7FVECTOR\n.word 0\n.word getUseCardPos__12CBattleEnemyR7FVECTOR\n.word 0\n.word getCameraTargetPos__7CEntityR7FVECTOR\n.word 0\n.word changeMotion__12CBattleEnemyiiiiii\n.word 0\n.word updateBefore__12CBattleEnemy\n.word 0\n.word updateAfter__12CBattleEnemy\n.word 0\n.word updatePositionAfter__12CBattleEnemy\n.word 0\n.word lastUpdate__12CBattleEnemy\n.word 0\n.word startCardBattle__12CBattleEnemy\n.word 0\n.word bossDeadDemoCallback__12CBattleEnemy\n.word 0\n.word setBossDeadEffect__12CBattleEnemy\n.word 0\n.word killBossDeadEffect__12CBattleEnemy\n.word 0\n.word bossCardRevolverDestroy__12CBattleEnemy\n.word 0\n.word bossDeadDemoDispOff__12CBattleEnemy\n.word 0\n.word bossDeadDemoSpecial__12CBattleEnemy\n.word 0\n.word bossDeadDemoEnd__12CBattleEnemy\n.word 0\n.word onBio__12CBattleEnemyi\n.word 0\n.word onGift__12CBattleEnemyi\n.word 0\n.word beginStateAppear__12CBattleEnemy\n.word 0\n.word stateAppear__12CBattleEnemy\n.word 0\n.word endStateAppear__12CBattleEnemy\n.word 0\n.word beginStateMain__12CBattleEnemy\n.word 0\n.word stateMain__12CBattleEnemy\n.word 0\n.word endStateMain__12CBattleEnemy\n.word 0\n.word beginStateAttack1__12CBattleEnemy\n.word 0\n.word stateAttack1__12CBattleEnemy\n.word 0\n.word endStateAttack1__12CBattleEnemy\n.word 0\n.word beginStateAttack2__12CBattleEnemy\n.word 0\n.word stateAttack2__12CBattleEnemy\n.word 0\n.word endStateAttack2__12CBattleEnemy\n.word 0\n.word beginStateAttack3__12CBattleEnemy\n.word 0\n.word stateAttack3__12CBattleEnemy\n.word 0\n.word endStateAttack3__12CBattleEnemy\n.word 0\n.word beginStateAttack4__12CBattleEnemy\n.word 0\n.word stateAttack4__12CBattleEnemy\n.word 0\n.word endStateAttack4__12CBattleEnemy\n.word 0\n.word beginStateAttack5__12CBattleEnemy\n.word 0\n.word stateAttack5__12CBattleEnemy\n.word 0\n.word endStateAttack5__12CBattleEnemy\n.word 0\n.word beginStateAttack6__12CBattleEnemy\n.word 0\n.word stateAttack6__12CBattleEnemy\n.word 0\n.word endStateAttack6__12CBattleEnemy\n.word 0\n.word beginStateAttack7__12CBattleEnemy\n.word 0\n.word stateAttack7__12CBattleEnemy\n.word 0\n.word endStateAttack7__12CBattleEnemy\n.word 0\n.word beginStateAttack8__12CBattleEnemy\n.word 0\n.word stateAttack8__12CBattleEnemy\n.word 0\n.word endStateAttack8__12CBattleEnemy\n.word 0\n.word beginStateItem__12CBattleEnemy\n.word 0\n.word stateItem__12CBattleEnemy\n.word 0\n.word endStateItem__12CBattleEnemy\n.word 0\n.word beginStateSmallDamage__12CBattleEnemy\n.word 0\n.word stateSmallDamage__12CBattleEnemy\n.word 0\n.word endStateSmallDamage__12CBattleEnemy\n.word 0\n.word beginStateBigDamage__12CBattleEnemy\n.word 0\n.word stateBigDamage__12CBattleEnemy\n.word 0\n.word endStateBigDamage__12CBattleEnemy\n.word 0\n.word beginStateCardBreak__12CBattleEnemy\n.word 0\n.word stateCardBreak__12CBattleEnemy\n.word 0\n.word endStateCardBreak__12CBattleEnemy\n.word 0\n.word beginStateZero__12CBattleEnemy\n.word 0\n.word stateZero__12CBattleEnemy\n.word 0\n.word endStateZero__12CBattleEnemy\n.word 0\n.word beginStateDuel__12CBattleEnemy\n.word 0\n.word stateDuel__12CBattleEnemy\n.word 0\n.word endStateDuel__12CBattleEnemy\n.word 0\n.word beginStateDuelSuccess__12CBattleEnemy\n.word 0\n.word stateDuelSuccess__12CBattleEnemy\n.word 0\n.word endStateDuelSuccess__12CBattleEnemy\n.word 0\n.word beginStateFaint__12CBattleEnemy\n.word 0\n.word stateFaint__12CBattleEnemy\n.word 0\n.word endStateFaint__12CBattleEnemy\n.word 0\n.word beginStateStop__12CBattleEnemy\n.word 0\n.word stateStop__12CBattleEnemy\n.word 0\n.word endStateStop__12CBattleEnemy\n.word 0\n.word beginStateTerror__12CBattleEnemy\n.word 0\n.word stateTerror__12CBattleEnemy\n.word 0\n.word endStateTerror__12CBattleEnemy\n.word 0\n.word beginStateFreeze__12CBattleEnemy\n.word 0\n.word stateFreeze__12CBattleEnemy\n.word 0\n.word endStateFreeze__12CBattleEnemy\n.word 0\n.word beginStateShock__12CBattleEnemy\n.word 0\n.word stateShock__12CBattleEnemy\n.word 0\n.word endStateShock__12CBattleEnemy\n.word 0\n.word beginStateTornado__12CBattleEnemy\n.word 0\n.word stateTornado__12CBattleEnemy\n.word 0\n.word endStateTornado__12CBattleEnemy\n.word 0\n.word beginStateDegenerator__12CBattleEnemy\n.word 0\n.word stateDegenerator__12CBattleEnemy\n.word 0\n.word endStateDegenerator__12CBattleEnemy\n.word 0\n.word beginStateGravity__12CBattleEnemy\n.word 0\n.word stateGravity__12CBattleEnemy\n.word 0\n.word endStateGravity__12CBattleEnemy\n.word 0\n.word beginStateToss__12CBattleEnemy\n.word 0\n.word stateToss__12CBattleEnemy\n.word 0\n.word endStateToss__12CBattleEnemy\n.word 0\n.word beginStateBeat__12CBattleEnemy\n.word 0\n.word stateBeat__12CBattleEnemy\n.word 0\n.word endStateBeat__12CBattleEnemy\n.word 0\n.word beginStateMagnet__12CBattleEnemy\n.word 0\n.word stateMagnet__12CBattleEnemy\n.word 0\n.word endStateMagnet__12CBattleEnemy\n.word 0\n.word beginStateSuspend__12CBattleEnemy\n.word 0\n.word stateSuspend__12CBattleEnemy\n.word 0\n.word endStateSuspend__12CBattleEnemy\n.word 0\n.word beginStateVolley__12CBattleEnemy\n.word 0\n.word stateVolley__12CBattleEnemy\n.word 0\n.word endStateVolley__12CBattleEnemy\n.word 0\n.word beginStateDead__12CBattleEnemy\n.word 0\n.word stateDead__12CBattleEnemy\n.word 0\n.word endStateDead__12CBattleEnemy\n.word 0\n.word aiCardBattle__12CBattleEnemy\n.word 0\n.word onGuard__12CBattleEnemy\n.word 0\n.word onDamage__12CBattleEnemy\n.word 0\n.word onAbsorb__12CBattleEnemy\n.word 0\n.word onInvalid__12CBattleEnemy\n.word 0\n.word onGimmickCard__12CBattleEnemy\n.word 0\n.word onCardBreak__12CBattleEnemy\n.word 0\n.word onReflect__12CBattleEnemy\n.word 0\n.word onAttackCancel__12CBattleEnemy\n.word 0\n.word onCardBattleFailed__12CBattleEnemy\n.word 0\n.word getDuelAttackState__12CBattleEnemy\n.word 0\n.word createFriendCard__12CBattleEnemy9CARD_KINDi\n.word 0\n.word notifyHP__12CBattleEnemy\n.word 0\n.word commonBeginAttack__12CBattleEnemy\n.word 0\n.word commonEndAttack__12CBattleEnemy\n.word 0\n.word 0\n.word 0x49705F6D\n.word 0x6174736E\n.word 0x0065636E\n.word 0x00000000\n.globl _vt$t14CStateTemplate1Z12CBattleEnemy\n_vt$t14CStateTemplate1Z12CBattleEnemy:\n.word 0\n.word 0\n.word 0\n.word _$_t14CStateTemplate1Z12CBattleEnemy\n.word 0\n.word executeBeginState__t14CStateTemplate1Z12CBattleEnemy\n.word 0\n.word executeState__t14CStateTemplate1Z12CBattleEnemy\n.word 0\n.word executeEndState__t14CStateTemplate1Z12CBattleEnemy\n.word 0\n.word 0\n.word 0\n.word 0\n.text\n");
namespace BattleEnemy {
const FVECTOR g_vIceSizeTbl[8] = {
    {{2.1f, 3.5f, 2.1f, 1.0f}},
    {{1.5f, 2.5f, 1.5f, 1.0f}},
    {{1.2f, 2.0f, 1.2f, 1.0f}},
    {{4.0f, 4.0f, 4.0f, 1.0f}},
    {{2.5f, 2.5f, 2.5f, 1.0f}},
    {{1.0f, 1.0f, 1.0f, 1.0f}},
    {{4.0f, 3.0f, 4.0f, 1.0f}},
    {{2.66f, 2.0f, 2.66f, 1.0f}},
};
const unsigned char g_byHeartlessIceSizeIndexTbl[39] = {
    0, 1, 1, 2, 0, 2, 2, 1, 1, 6, 6, 1, 0,
    4, 1, 1, 2, 2, 2, 4, 3, 1, 4, 4, 4, 0,
    6, 1, 7, 7, 2, 1, 1, 1, 2, 2, 1, 1, 1,
};
const unsigned char g_byBossIceSizeIndexTbl[38] = {
    0, 0, 0, 1, 1, 0, 4, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
const FLOAT g_vSpotSizeTbl[9] = {1.4f, 1.0f, 0.8f, 2.0f, 1.0f, 0.6f, 2.0f, 1.2f, 0.0f};
const unsigned char g_byHeartlessSpotSizeIndexTbl[39] = {
    0, 1, 1, 2, 0, 2, 2, 1, 1, 6, 6, 1, 0,
    4, 1, 1, 2, 2, 2, 4, 8, 1, 4, 4, 4, 0,
    6, 1, 7, 7, 2, 1, 1, 1, 2, 2, 1, 1, 1,
};
const unsigned char g_byBossSpotSizeIndexTbl[38] = {
    8, 8, 8, 1, 1, 8, 4, 0, 8, 8, 8, 8, 0,
    8, 0, 8, 1, 0, 0, 0, 8, 0, 0, 0, 0, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
};
const INT GameModeParamHp[4] = {75, 100, 100, 100};
const FLOAT GameModeParamAp[4] = {0.8f, 1.0f, 1.2f, 1.0f};
}
/* CState's vtable (all its virtuals are inline or pure): this object's copy is the one the original kept. */
__asm__(".rdata\n.align 3\n.word 0\n.word 0\n.word 0\n.word _$_6CState\n.word 0\n.word __pure_virtual\n.word 0\n.word __pure_virtual\n.word 0\n.word __pure_virtual\n.word 0\n.word 0\n.text\n");
// SOURCE DATA: .data .rodata
