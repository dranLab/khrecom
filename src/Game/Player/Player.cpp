#define GAME_PLAYER_PLAYER_CPP
#include "khtypes.h"
#include "Game/Collision/CollBody.h"
#include "Game/CardBattle.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerManager.h"
#include "System/Debug.h"
#include "Game/Player/PlayerGauge.h"
#include "Game/Player/PlayerWeapon.h"
#include "Game/Debug/DebugWork.h"
#include "Game/Movement.h"
#include "Game/Status.h"
#include "Game/SceneEnv.h"
#include "Game/Bullet/BulletManager.h"
#include "System/TaskMan.h"
#include "Game/SEMotion.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Coord.h"
#include "Game/Player/PlayerAnim.h"
#include "Game/GameWork.h"
#include "Form/Menu/WorldSelect.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/SceneInfo.h"
#include "Game/Gimmick/BaseGimmick.h"
#include "Game/Collision/CollMap.h"
#include "System/HeapMan.h"
#include "Game/Enemy/BattleEnemyManager.h"
#include "Game/CardKind.h"
#include "System/Graphic/Camera.h"
#include "System/Graphic/WorldEnv.h"
#include "Game/Game.h"
#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"
#include "System/Math3D.h"
#include "System/Rnd.h"
#include "Game/Player/PlayerAttack.h"
#include "System/Graphic/Anim.h"
#include "Game/Card.h"
#include "Game/LockOnManager.h"
#include "include_asm.h"

extern "C" char D_004997C0[];
extern "C" char D_00499828[];
extern "C" char D_00499A18[];
void ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void Normalize(FVECTOR& outV, const FVECTOR& inV);
FLOAT InnerProduct(const FVECTOR& inV0, const FVECTOR& inV1);
extern "C" char D_0049B308[];
extern "C" char D_004997F8[]; extern "C" char D_00499808[]; extern "C" char D_00499818[]; /* "System/Fsm.h", "pInstance", "pfnBeginState" */
extern "C" char D_00499898[]; extern "C" char D_004998C8[]; extern "C" char D_004998F8[]; /* "...Player.cpp(1103)", (1106), (1112) */
extern "C" char D_00499928[]; extern "C" char D_00499958[]; /* "...Player.cpp(1117)", (1124) */
extern "C" char D_0049AFC0[]; /* "c:/KHCOM/MainApp/Game/Player/Player.cpp(12257)" */
extern "C" char D_0049B238[]; /* TRACE message (Shift-JIS) of notifyExterminate */
extern "C" char D_0049AEC0[]; /* TRACE message of initGimmickCard */
extern "C" char D_0049A888[]; /* TRACE message of procSkillEnd */
extern "C" char D_00499F48[]; extern "C" char D_00499F70[]; extern "C" char D_00499F80[]; extern "C" char D_00499FA0[]; /* TRACE messages of chkPlayCard */
extern "C" char D_00499988[]; extern "C" char D_004999B8[]; extern "C" char D_004999E8[]; /* "...Player.cpp(1156)", (1172), (1181) */
extern "C" const CARD_DATA cardDataTbl_0049E190[224];
extern "C" char D_0049AFA8[]; /* TRACE messages of setTrapDamage */
extern "C" char D_0049A920[];
extern "C" char D_0049A988[];
extern "C" char D_0049AA00[];
extern "C" char D_0049AC10[]; /* TRACE messages of initFlip */
extern "C" char D_0049AC30[];
inline void* operator new(size_t, void* p) throw() { return p; }
static inline FLOAT _abs(FLOAT inValue) {
    FLOAT ret;
    __asm__ volatile("abs.s %0, %1" : "=f"(ret) : "f"(inValue));
    return ret;
}
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
extern "C" void sceVu0RotMatrixX(FMATRIX& m0, FMATRIX& m1, float rx);
extern "C" void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
extern "C" void sceVu0UnitMatrix(FMATRIX& m);
/* Through inline wrappers: called directly, cse keeps the matrix address in a saved register across the
 * calls; the original recomputes it for each call (as in Game/Player/FriendKing). */
static inline void _unitMatrix(FMATRIX& m) {
    sceVu0UnitMatrix(m);
}
static inline void _rotMatrixY(FMATRIX& m, FLOAT r) {
    sceVu0RotMatrixY(m, m, r);
}
extern "C" float atan2f(float y, float x);
static inline FLOAT _limitPI(FLOAT r) {
    if (r < -3.1415927f) r += 6.2831855f;
    else if (r > 3.1415927f) r -= 6.2831855f;
    return r;
}
/* System/RsrcDB.h parses only with RSRCDATA_H_FULL: local declarations of what makeAnimIndex needs */
enum NAME_CASE { NAME_CASE_SENSITIVE = 0, NAME_CASE_UPPER = 1, NAME_CASE_LOWER = 2 };
class CRsrcDB {
public:
    static INT getMotionIdxFromMotName(const CHAR* pszName, const CHAR* pszMotName, NAME_CASE nc);
};
namespace PL {
extern FVECTOR g_vDummyPlayerPos; // data from the asm until SOURCE DATA
extern FVECTOR g_vBanishPos;
extern BULLET_EFFECT_INFO g_EffTbl_Skill[67][4]; // PlayerAttack.h in the original
extern LPCSTR g_szDpxAlways; // static in the original; data from the asm until SOURCE DATA
extern LPCSTR g_szDpxReload; // static in the original
extern INT g_CardBreakTime[8]; // PlayerAttack.h in the original
extern LPCSTR g_szTexEyeDamage; extern LPCSTR g_szTexEyeShut; extern LPCSTR g_szTexEyeKizetsu; // static in the original
extern LPCSTR g_szTexLipDamage; extern LPCSTR g_szTexLipDefault;
extern LPCSTR g_szDpxAbnormal; // static in the original
extern FVECTOR g_vFieldPlayerPos; // static in the original
extern FVECTOR g_vBattlePlayerPos; // static in the original
extern FVECTOR HANG_HAND_LOCAL_OFS; // static in the original
extern const FVECTOR LADDER_LOCAL_OFS; // static in the original
extern LPCSTR g_szDpxAsiato __asm__("_2PL$g_szDpxAsiato_003A2298"); // static in the original (PlayerDRiku has one too)
extern LPCSTR g_szDpxSpeedBreak __asm__("_2PL$g_szDpxSpeedBreak_003A229C"); // static in the original
extern LPCSTR g_szDpxBreak; // static in the original
extern LPCSTR g_szDpxHitWall; // static in the original
extern FLOAT g_fDefAttr[];
extern FLOAT g_fGimmickJumpPower[8];
}
class CPlayerDebug { /* PARTIAL: size and ctor for makeCockpit; CPlayerDebug's members are still INCLUDE_ASM */
    UBYTE _m_0x00[0x120];
public:
    CPlayerDebug(CTask* inParent);
};
namespace Gauge {
class CPlayerGauge { /* PARTIAL: Game/Player/PlayerGauge.h declares the class only for its own unit */
    UBYTE _m_0x00[0x80];
public:
    CPlayerGauge(CTask* p);
    void setMaxHP(INT n); void setDamage(INT inHP, INT inFlag);
    void setHP(INT inHP, INT inFlag);
    void setDP(INT inDP, INT inFlag);
};
class CEscapeGauge { /* PARTIAL: Game/Player/PlayerGauge.h declares the class only for its own unit */
    UBYTE _m_0x00[0x50];
public:
    CEscapeGauge(CTask* p);
    void destroy(); void setCharge(FLOAT inCharge);
};
class CPrizeGauge { /* PARTIAL: Game/Player/PlayerGauge.h declares the class only for its own unit */
public:
    static void setPrizeMp(INT inMP); static void setDropMp(INT inMP); static void setPrizeExp(INT inEXP);
};
}
class CSound { /* PARTIAL: System/Sound.h is not written yet */
public:
    static HANDLE playSE(INT nID, INT nCh, INT nVol);
};
#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", DebugPlayerCamera__9DWTPlayerP12CDebugWindowii_0025F688);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", DebugPlayerAttack__9DWTPlayerP12CDebugWindowii_0025F798);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", DebugWinPlayer__9DWTPlayerP12CDebugWindowii_0025FA48);
FVECTOR getPlayerPos(BOOL flag) {
    if (g_pPlayer) {
        if (!flag && g_pPlayer->isConditionFlag(0x40)) return PL::g_vBanishPos;
        return g_pPlayer->getPos();
    }
    return PL::g_vDummyPlayerPos;
}
INT CPlayer::calcNextEXP(INT inLv) {
    INT nExp = 25;
    for (INT i = 3; i <= inLv + 1; i++) {
        nExp += i * i * 4;
    }
    return nExp;
}
INT CPlayer::getNowNextEXP() {
    INT next;
    if (m_pInstance) {
        next = calcNextEXP(m_pInstance->getLV());
        __asm__ __volatile__("" : : : "memory"); /* matching lever: the original re-reads m_pInstance after the const call */
        return next - m_pInstance->getEXP();
    } else {
        next = calcNextEXP(CStatus::getLV());
        return next - CStatus::getEXP();
    }
}
INT CPlayer::getNowTotalMP() {
    if (!m_pInstance) return CStatus::getMP();
    return m_pInstance->m_iMP;
}
void CPlayer::enablePadControl(INT inFlag) {
    if (m_pInstance) {
        UINT bit = 1;
        m_pInstance->m_uiPadFlag = (m_pInstance->m_uiPadFlag | inFlag) & ~(inFlag ^ bit);
    }
}
void CPlayer::enableDbgPadControl(INT inFlag) {
    if (m_pInstance) {
        UINT bit = 2;
        inFlag <<= 1;
        m_pInstance->m_uiPadFlag = (m_pInstance->m_uiPadFlag | inFlag) & ~(inFlag ^ bit);
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", __7CPlayerP5CTaskUii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", _$_7CPlayer);
/* CStateTemplate<CPlayer> (System/Fsm.h) layout; set() is copied here with the original's string labels
 * because the unit's .rodata still comes from the assembly. */
struct PLAYER_STATE {
    void* m_pVtbl; // CState vptr
    CPlayer* m_pInstance;
    void (CPlayer::*m_pfnBeginState)();
    void (CPlayer::*m_pfnState)();
    void (CPlayer::*m_pfnEndState)();
};
static inline void _setState(PLAYER_STATE& s, CPlayer* pInstance, void (CPlayer::*pfnBeginState)(), void (CPlayer::*pfnState)(), void (CPlayer::*pfnEndState)()) {
    if (!pInstance) AssertApp(D_004997F8, 41, D_00499808, 0);
    s.m_pInstance = pInstance;
    if (!pfnBeginState) AssertApp(D_004997F8, 44, D_00499818, 0);
    s.m_pfnBeginState = pfnBeginState;
    if (!pfnBeginState) AssertApp(D_004997F8, 46, D_00499818, 0);
    s.m_pfnState = pfnState;
    if (!pfnBeginState) AssertApp(D_004997F8, 48, D_00499818, 0);
    s.m_pfnEndState = pfnEndState;
}
void CPlayer::initFSM_() {
    for (INT i = 0; i < 4; i++) {
        _setState(((PLAYER_STATE*)_m_0x21c)[i], this, m_pBeginStateTbl[i], m_pStateTbl[i], m_pEndStateTbl[i]);
    }
    m_iState = -1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", _init__7CPlayer);
void CPlayer::updateBefore() {
    *(BOOL*)&m_bPreAir = m_bAir;
    m_fWeight = PL::g_fWeight;
    PL::g_fAntiGravity = PL::g_fWeight * 0.008166667f;
    if (CardBattle::CCardBattle::isLevitate()) m_fWeight *= 0.5f;
    PL::g_fGravity = m_fWeight * -0.008166667f;
}
void CPlayer::updateAfter() {}
void CPlayer::lastUpdate() {
    updateBodyColl();
    m_hGimmickJump = 0;
    updateFooting();
}
void CPlayer::changeState(INT inState) {
    if (!((UINT)inState < 4)) AssertApp(D_004997C0, 941, D_00499828, 0);
    gotoState(*(CState*)&_m_0x21c[inState * 0x20]);
}
void CPlayer::stateInitial() {
    setPlayerState(PL::INIT);
    m_uiStep = 0;
    importStatus();
    setDefAttr(PL::g_fDefAttr);
    m_fPrizeDraw = 5.0f;
    m_fCardDraw = 5.0f;
    createLegIK(-1.0f, -1.0f);
    setFootEffectIK(PL::g_szDpxAsiato, 0);
    setGroundIK(1);
    makeAnimIndex(m_uiCharaID);
    initStartPos();
    makeWeapon();
    if (!isPlayerFlag(0x20000000)) makeCockpit();
    if (!m_bIsBattleScene) {
        setAnimForce(1, 0, 0, 0, 0, 0);
    } else {
        setAnimForce(0x2A, 0, 0, 0, 0, 0);
        if (!isPlayerFlag(0x20000000)) makeRevolver();
    }
    initActionSkill();
    if (CGameWork::getFlag(1, 0)) changeState(3);
}
void CPlayer::initStartPos() {
    FVECTOR v;
    if (!m_bIsBattleScene) {
        if (m_pPLM) m_pPLM->calcFieldStartPos();
        m_vStartPos.set(PL::g_vFieldPlayerPos.x, PL::g_vFieldPlayerPos.y, PL::g_vFieldPlayerPos.z);
        m_fStartDir = PL::g_vFieldPlayerPos.w * 0.017453292f;
        setOldPos(m_vStartPos);
        setPos(m_vStartPos);
        m_fDir = m_fStartDir;
        changeState(0);
        m_pMove->setFlag(2, TRUE);
    } else {
        if (m_bIsMinigameScene) {
            m_vStartPos.set(-19.54f, 3.01f, -8.96f);
            m_fStartDir = -1.2217305f;
        } else {
            m_vStartPos.set(PL::g_vBattlePlayerPos.x, PL::g_vBattlePlayerPos.y, PL::g_vBattlePlayerPos.z);
            m_fStartDir = PL::g_vBattlePlayerPos.w * 0.017453292f;
        }
        setOldPos(m_vStartPos);
        setPos(m_vStartPos);
        m_fDir = m_fStartDir;
        changeState(1);
        setBattleFlag(1, 0);
    }
    {
        FVECTOR r;
        r._q = 0;
        r.y = m_fDir;
        v = r;
    }
    m_pMove->getRot() = v;
    updateCoord();
    coordFix();
    m_pMove->setFlag(0x40, TRUE);
    m_pMove->setFlag(0x10, TRUE);
    m_pMove->setFootRadius(0.25f);
    m_pMove->setWallRadius(0.5f);
    m_pMove->land(5.0f, 1.0f, 0);
    m_vCenterPos = Crd(m_iBoneCenter).getWorldMatrix().v[3];
    m_vHeadPos = Crd(m_iBoneHead).getWorldMatrix().v[3];
    m_vJumpStartPos = getPos();
    m_pCollMap = m_pMove->getMapCollision();
    m_collBody.init((COLL_KIND)0, (COLL_TYPE)4, getHandle(), 0);
    m_collBody.setRadius(m_pParam->BODY_COLL_RADIUS);
    m_collBody.off();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", makeWeapon__7CPlayer);
void CPlayer::makeCockpit() {
    m_pCam = new (com_malloc_d(sizeof(PlayerCamera::CPlayerCamera), D_00499898, RET_ADDR())) PlayerCamera::CPlayerCamera(this);
    m_pMarker = new (com_malloc_d(sizeof(Gauge::CLockonMarker), D_004998C8, RET_ADDR())) Gauge::CLockonMarker(this, FALSE);
    if (m_bIsBattleScene) {
        m_pEscape = new (com_malloc_d(sizeof(Gauge::CEscapeGauge), D_004998F8, RET_ADDR())) Gauge::CEscapeGauge(this);
    }
    m_pGauge = new (com_malloc_d(sizeof(Gauge::CPlayerGauge), D_00499928, RET_ADDR())) Gauge::CPlayerGauge(this);
    m_pGauge->setMaxHP(m_iMaxHP);
    m_pGauge->setHP(m_iHP, 1);
    m_pGauge->setDP(m_iDP, 30);
    m_pDebug = new (com_malloc_d(sizeof(CPlayerDebug), D_00499958, RET_ADDR())) CPlayerDebug(this);
}
static inline void _setCard(CARD& c, USHORT kind, BYTE num) {
    c.m_nId = 0;
    c.m_nKind2 = kind;
    c.m_nNumber2 = num;
    c.m_nCategory20 = cardDataTbl_0049E190[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl_0049E190[c.m_nKind].nCategory1;
}
void CPlayer::makeRevolver() {
    if (!m_pRevolver) {
        CARD* pDeck = NULL;
        CARD* deck = CCard::getCurrentDeck();
        if (!DebugWork::g_dw.player.iUseDbgDeck) pDeck = deck;
        if (CGameWork::getFlag(1, 0x2C)) pDeck = NULL;
        if (m_bIsMinigameScene) pDeck = NULL;
        if (!pDeck) {
            m_pRevolver = new (com_malloc_d(sizeof(CardBattle::CCardRevolver), D_00499988, RET_ADDR())) CardBattle::CCardRevolver(this, NULL, 1, -1);
        } else {
            INT max = CCard::m_pCardInfo->_nDeck[CCard::m_pCardInfo->_nCurrentDeck];
            INT sc = CCard::getCurrentShortCut();
            if (dbgCheckUser(KKIKUCHI) && sc == -1) sc = DebugWork::g_dw.player.iTemp;
            deck = (CARD*)com_malloc_d((max + 1) * sizeof(CARD), D_004999B8, RET_ADDR());
            for (INT i = 0; i < max; i++) {
                deck[i] = pDeck[i];
                if (deck[i].m_nCategory1 == 7) deck[i].m_nNumber = -1;
            }
            _setCard(deck[max], 0xFF, 0);
            m_pRevolver = new (com_malloc_d(sizeof(CardBattle::CCardRevolver), D_004999E8, RET_ADDR())) CardBattle::CCardRevolver(this, deck, 1, sc);
            if (deck) com_free(deck);
        }
    }
    m_pCB = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
}
void CPlayer::initActionSkill() {
    UINT uiActionSkill = CGameWork::getInt(0, 0x24);
    if (uiActionSkill & 1) setActionFlag(0x200000, 0);
    else setActionFlag(0, 0x200000);
    DebugWork::g_dw.player.iHiJump = isActionFlag(0x200000) != 0;
    if (CGame::isSoraGame()) {
        if (uiActionSkill & 4) setActionFlag(0x800000, 0);
        else setActionFlag(0, 0x800000);
        if (uiActionSkill & 2) setActionFlag(0x400000, 0);
        else setActionFlag(0, 0x400000);
        DebugWork::g_dw.player.iGlide = isActionFlag(0x400000) != 0;
        DebugWork::g_dw.player.iGlideSP = isActionFlag(0x800000) != 0;
    }
    setActionFlag(0, 0x4000000);
    if (CGame::isRikuGame() && m_bIsBattleScene) {
        if ((CGameWork::getInt(0, 0x24) & 8) && !m_pPLM->isZexionBattle()) setActionFlag(0x4000000, 0);
    }
    if (!m_bIsBattleScene) {
        if (m_pPLM->isTraverseTown()) setActionFlag(0x10000, 0);
        else setActionFlag(0, 0x10000);
    }
    if (CSceneInfo::getCurrentWorldID(0) == 10) {
        setActionFlag(0, 0x20);
    } else if (m_bIsBattleScene && CGameWork::getFlag(1, 0x31)) {
        setActionFlag(0x20, 0);
    }
}
INT CPlayer::importStatus() {
    if (!_instance()) return 0;
    _instance()->setMaxHP(CStatus::getStaus().HPMax);
    _instance()->setHP(CStatus::getStaus().HP);
    _instance()->setAP(CStatus::getStaus().AP);
    _instance()->setEXP(CStatus::getStaus().EXP);
    _instance()->setLV(CStatus::getStaus().LV);
    _instance()->setMP(CStatus::getStaus().MP);
    _instance()->setMaxDP(CStatus::getStaus().DPMax);
    return 1;
}
INT CPlayer::exportStatus() {
    if (!_instance()) return 0;
    CStatus::setHPMax(_instance()->m_iMaxHP);
    CStatus::setHP(_instance()->m_iHP);
    CStatus::setAP(_instance()->m_iAP);
    CStatus::setEXP(_instance()->m_iEXP);
    CStatus::setMP(_instance()->m_iMP);
    return 1;
}
void CPlayer::setPlayerState(PlayerState inState) {
    m_PrePlayerState = m_PlayerState;
    m_PlayerState = inState;
    m_iSubState = 0;
    m_tqStateTimer = 0;
}
void CPlayer::makeAnimIndex(INT inChara) {
    if (inChara) {
        MOT_NAME_TBL* tbl = PL_ANIM::g_MotNameTbl[inChara];
        LPCSTR file = PL_ANIM::g_pPlayerModelName[inChara];
        while (tbl->pFileName) {
            tbl->iIndex = CRsrcDB::getMotionIdxFromMotName(file, tbl->pFileName, NAME_CASE_SENSITIVE);
            tbl++;
        }
    }
}
INT CPlayer::getMotIndex(INT inChara, INT inIndex) {
    if (!(0 <= inIndex && inIndex < ANIM_MAX)) AssertApp(D_004997C0, 1361, D_00499A18, 0);
    return PL_ANIM::g_MotNameTbl[inChara][inIndex].iIndex;
}
void CPlayer::setAnimForce(INT inAnimID, TIMEQ inChangeTime, BOOL inRootMove, INT inEffSeq, INT inSESeq, TIMEQ inStart) {
    m_iPreAnimID = m_iNowAnimID;
    m_iNowAnimID = inAnimID;
    INT animID = getMotIndex(m_uiCharaID, inAnimID);
    changeMotion(animID, inStart, inRootMove, inChangeTime, inEffSeq, inSESeq);
    if (isPlayerFlag(0x40000000)) {
        m_pWeapon->changeMotion(animID, inStart, FALSE, inChangeTime, inEffSeq, inSESeq);
    }
    setAnimSpeed(1.0f);
}
void CPlayer::setAnim(INT inID, INT inTq, INT inGr) {
    if (m_iNowAnimID != inID) setAnimForce(inID, inTq, inGr, 0, 0, 0);
}
void CPlayer::setAnimSpeed(FLOAT inSpeed) {
    if (m_fAnimSpeed != inSpeed) {
        m_fAnimSpeed = inSpeed;
        CEntity::setAnimSpeed(inSpeed);
        if (isPlayerFlag(0x40000000)) {
            m_pWeapon->setAnimSpeed(m_fAnimSpeed);
        }
    }
}
void CPlayer::setAnimSkillEx(INT inAnimID, TIMEQ inChangeTime, BOOL inRootMove, INT inEffSeq, INT inSESeq) {
    UINT rcid = m_pPLM->getRCID_toSkill(m_iStockSkill);
    if (rcid) {
        m_iNowAnimID = -1;
        changeMotionEx(rcid + 10000, inAnimID, 0, inRootMove, inChangeTime, inEffSeq, inSESeq);
        if (isPlayerFlag(0x40000000)) {
            m_pWeapon->changeMotionEx(rcid + 11000, inAnimID, 0, FALSE, inChangeTime, inEffSeq, inSESeq);
        }
        setAnimSpeed(1.0f);
    }
}
void CPlayer::setAnimRsrcEx(INT inRcid, INT inAnimID, TIMEQ inChangeTime, BOOL inRootMove, INT inEffSeq, INT inSESeq, INT inWep) {
    m_iNowAnimID = -1;
    changeMotionEx(inRcid + 10000, inAnimID, 0, inRootMove, inChangeTime, inEffSeq, inSESeq);
    if (isPlayerFlag(0x40000000) && inWep > 0) {
        m_pWeapon->changeMotionEx(inRcid + inWep + 10000, inAnimID, 0, FALSE, inChangeTime, inEffSeq, inSESeq);
    }
    setAnimSpeed(1.0f);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isNowIdleAnim__7CPlayer);
void CPlayer::setFaceAnim(INT inID, INT inFlag) {
    if (!inFlag && m_iNowFaceID == inID) return;
    m_iNowFaceID = inID;
    switch (inID) {
    case 1:
        setTexAnmSeq(0, PL::g_szTexEyeDamage);
        setTexAnmSeq(1, PL::g_szTexLipDamage);
        break;
    case 2:
        setTexAnmSeq(0, PL::g_szTexEyeShut);
        setTexAnmSeq(1, PL::g_szTexLipDamage);
        break;
    case 3:
        setTexAnmSeq(0, PL::g_szTexEyeKizetsu);
        setTexAnmSeq(1, PL::g_szTexLipDefault);
        break;
    case 0:
    default:
        m_iNowFaceID = 0;
        setTexAnmSeq(0, -1);
        setTexAnmSeq(1, -1);
        break;
    }
}
void CPlayer::setTransOff(INT inFlag) {
    if (inFlag) {
        setEntityFlag(0x1000, 0);
        if (isPlayerFlag(0x40000000)) m_pWeapon->setEntityFlag(0x1000, 0);
    } else {
        setEntityFlag(0, 0x1000);
        if (isPlayerFlag(0x40000000)) m_pWeapon->setEntityFlag(0, 0x1000);
    }
}
void CPlayer::onWeapon(INT inEffect) {
    if (m_pWeapon) m_pWeapon->onWeapon(inEffect);
    m_uiPlayerFlag = (INT)m_uiPlayerFlag & ~2;
}
void CPlayer::offWeapon() {
    if (m_pWeapon) m_pWeapon->offWeapon();
    m_uiPlayerFlag |= 2;
}
void CPlayer::onAttack() {
    if (m_pWeapon) {
        m_pWeapon->onAttack(m_uiAttackID, m_iAttackPower, m_AtkKind, m_AtkAttr);
        onWeapon(1);
        if (m_uiAttackID < 8 || m_uiAttackID - 0x97 < 14) {
            if (isConditionFlag(0x80)) m_pWeapon->setWideAttack(1);
            if (isConditionFlag(0x100000)) m_pWeapon->setBackAttack(1);
            if (isConditionFlag(0x20000000)) m_pWeapon->setDoubleHit(1, NULL);
        }
    }
}
void CPlayer::offAttack() {
    if (m_pWeapon) m_pWeapon->offAttack();
}
void CPlayer::getWeaponTopPos(FVECTOR& inVec) {
    if (m_pWeapon) {
        FVECTOR ofs = PlayerWeapon::g_fWeaponTopOffset;
        FMATRIX m = m_pWeapon->getMatrixWep0();
        ApplyMatrixXYZ(inVec, m, ofs);
    }
}
void CPlayer::getCaskPos(FVECTOR& inVec) {
    if (m_pWeapon) {
        FVECTOR ofs = { { -0.06f, -0.27f, 0.03f, 1.0f } };
        if (DebugWork::g_dw.player.iTemp == 1) ofs = DebugWork::g_dw.player.vTemp;
        FMATRIX m = m_pWeapon->getMatrixWep0();
        ApplyMatrixXYZ(inVec, m, ofs);
    }
}
void CPlayer::updateBodyColl() {
    updateBodyCollPos();
    m_collBody.m_uiFlag = (INT)m_collBody.m_uiFlag & ~8;
    m_collBody.m_uiFlag = (INT)m_collBody.m_uiFlag & ~0x20;
    CCollBody& body = m_collBody;
    if (isPlayerFlag(1)) body.CCollBody::off();
    else body.CCollBody::on();
    if (isPlayerFlag(0x1000)) m_collBody.m_enableBody = 0;
    else m_collBody.m_enableBody = 1;
}
void CPlayer::updateBodyCollPos() {
    FVECTOR top;
    FVECTOR bottom;
    m_vCenterPos = Crd(m_iBoneCenter).getWorldMatrix().v[3];
    m_vHeadPos = Crd(m_iBoneHead).getWorldMatrix().v[3];
    FLOAT _x = m_vCenterPos.x;
    FLOAT _y = m_vHeadPos.y + 0.2f;
    FLOAT _z = m_vCenterPos.z;
    m_vHeadPos.x = _x;
    m_vHeadPos.y = _y;
    m_vHeadPos.z = _z;
    top.set(_x, _y, _z);
    FLOAT y = (Crd(m_iBoneLFoot).getWorldMatrix().getTrans().y + Crd(m_iBoneRFoot).getWorldMatrix().getTrans().y) * 0.5f;
    bottom.set(m_vCenterPos.x, y, m_vCenterPos.z);
    m_collBody.setTopPos(top);
    m_collBody.setBottomPos(bottom);
    m_collBody.setRadius(m_pParam->BODY_COLL_RADIUS);
    m_collBody.setDir(m_fDir);
}
BOOL CPlayer::isMuteki() const {
    if (m_uiPlayerFlag & 1) return TRUE;
    if (m_tqMutekiTimer > 0) return TRUE;
    return m_bDmgMuteki != 0;
}
void CPlayer::exeMutekiTimer() {
    m_tqMutekiTimer -= m_tqFrameRate;
    if (m_tqMutekiTimer < 0) m_tqMutekiTimer = 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setControl__7CPlayer);
INT CPlayer::chkAtelHide(INT inFlag) {
    if (inFlag) {
        if (CGameWork::getFlag(1, 0)) {
            changeState(3);
            return TRUE;
        }
    } else {
        if (!CGameWork::getFlag(1, 0)) {
            if (!m_bIsBattleScene) changeState(0);
            else changeState(1);
            return TRUE;
        }
    }
    return FALSE;
}
void CPlayer::chkAtelControl() {
    if (CGameWork::getFlag(1, 5)) {
        if (isControl()) {
            m_uiPadFlag = (INT)m_uiPadFlag & ~1;
            m_pCam->setFlag(1, TRUE);
        }
    } else {
        if (!isControl()) {
            enableControl();
            m_pCam->setFlag(1, FALSE);
        }
    }
}
void CPlayer::getCameraTargetPos(FVECTOR& outPos) {
    if (m_PlayerState == PL::TRANSFORM) {
        outPos = Crd(m_iBoneCenter).getWorldMatrix().v[3];
    } else {
        outPos = Crd(0).getWorldMatrix().v[3];
        outPos.y += 0.5f;
    }
}
void CPlayer::chkBodyRevise() {
    setPlayerFlag(0, 0x200000);
    if (!isPlayerFlag(0x9000) && m_collBody.collideBody(0) && !isPlayerFlag(0x400000)) {
        FVECTOR pos = getPos();
        setOldPos(pos);
        pos.x += (m_collBody.getOutPos()->x - m_vCenterPos.x) * 0.5f;
        pos.z += (m_collBody.getOutPos()->z - m_vCenterPos.z) * 0.5f;
        setPos(pos);
        setPlayerFlag(0x200000, 0);
    }
    setPlayerFlag(0, 0x400000);
}
void CPlayer::recoverCollMap() {
    if (m_pCollMap) {
        m_pCollMap->setType(COLL_TYPE_SPHERE);
        m_pCollMap->enableSliding(1);
        m_pMove->setFootRadius(m_fFootRadius);
        m_pMove->setWallRadius(m_fWallRadius);
    }
}
void CPlayer::setDownForce() {
    if (!isConditionFlag(0x10000000)) {
        if (!getJumpState()) {
            FVECTOR vel = m_pMove->getVel();
            vel.y = -0.05f;
            m_pMove->setVel(vel);
        }
    } else {
        FVECTOR vel = m_pMove->getVel();
        FVECTOR pos = m_pMove->getPos();
        pos.y += vel.y;
        if (pos.y <= m_fJumpLandLimit) {
            pos.y = m_fJumpLandLimit;
            if (vel.y < 0.0f) vel.y = 0.0f;
            m_pMove->getPos() = pos;
            m_pMove->setVel(vel);
            m_bAir = FALSE;
        }
    }
}
void CPlayer::setLand(FLOAT inDownLEngth, FLOAT inUpLength, BOOL inFirstHit) {
    recoverCollMap();
    land(inDownLEngth, inUpLength, inFirstHit);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", beginStateField__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", stateField__7CPlayer);
void CPlayer::endStateField() {
    setActionFlag(0, m_pParam->USE_MAP_ACTION);
    setBattleFlag(0, 1);
    if (CGameWork::getFlag(1, 0)) offWeapon();
    m_collBody.offBody();
    offAttack();
    m_uiAttackKind = 0;
    m_uiPreAttackKind = 0;
    m_uiAttackID = 0;
    m_pMarker->setFlag(FALSE);
}
void CPlayer::beginStateBattle() {
    m_iState = 1;
    m_uiStep = 0;
    setActionFlag(m_pParam->USE_BTL_ACTION, 0);
    initActionSkill();
    setBattleFlag(1, 0);
    setAnimForce(0x2A, -1, 0, 0, 0, 0);
    onWeapon(0);
    m_pPLM->setReqLoadFirst();
    initStartWait();
    if (PL::g_uiStartCondition & 8) setConfuse(1, 0);
    PL::g_uiStartCondition = 0;
    m_szEffectName = NULL;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", stateBattle__7CPlayer);
void CPlayer::endStateBattle() {
    setActionFlag(0, m_pParam->USE_BTL_ACTION);
    m_collBody.offBody();
    offAttack();
    setBattleFlag(0, 1);
    offWeapon();
    m_uiAttackKind = 0;
    m_uiPreAttackKind = 0;
    m_uiAttackID = 0;
    m_pMarker->setFlag(FALSE);
}
void CPlayer::beginStateWait() {
    m_iState = 2;
}
void CPlayer::beginStateHide() {
    m_iState = 3;
    setPlayerState(PL::HIDE);
    clearSpeed();
    setEntityFlag(1, 0);
    m_WalkMode = 0;
    if (m_bIsBattleScene) setAnimForce(0x2A, 0, 0, 0, 0, 0);
    else setAnimForce(1, 0, 0, 0, 0, 0);
    setPlayerFlag(0x401001, 0);
    m_pCam->setFlag(1, TRUE);
}
void CPlayer::stateHide() {
    setPlayerFlag(0x400000, 0);
    if (dbgCheckUser(KKIKUCHI) && dbgButtonTrg(USR_SYSTEM, 0x10, 1)) {
        CGameWork::setInt(1, 0xE2, 0);
    }
    chkAtelHide(0);
}
void CPlayer::endStateHide() {
    setEntityFlag(0, 1);
    setPlayerFlag(0, 0x1001);
    setLand(5.0f, 0.5f, 1);
    clearSpeed();
    m_pCam->setFlag(1, FALSE); /* the partial CPlayerCamera keeps (flag, on/off): the original is setFlag(0, 1) */
}
BOOL CPlayer::isNowInAir() {
    m_bAirCheck = FALSE;
    return isAir();
}
BOOL CPlayer::isAir() {
    if (m_bAirCheck) return m_bAir;
    m_bAirCheck = TRUE;
    if (m_pMove->isGround()) {
        m_bAir = FALSE;
        return FALSE;
    }
    FVECTOR pos = m_pMove->getPos();
    if (pos.y <= m_fJumpLandLimit) {
        pos.y = m_fJumpLandLimit;
        m_pMove->getPos() = pos;
        m_bAir = FALSE;
        return FALSE;
    }
    m_bAir = TRUE;
    return TRUE;
}
FLOAT CPlayer::getAirHeight(FVECTOR* inPos, FLOAT inHeight, INT inLimit) {
    FVECTOR pos;
    if (inPos) pos = *inPos;
    else pos = getPos();
    FLOAT fHeight = inHeight;
    FVECTOR vStart = {{ pos.x, pos.y, pos.z, 1.0f }};
    FVECTOR vEnd = {{ pos.x, pos.y - fHeight, pos.z, 1.0f }};
    m_pCollMap->setType(COLL_TYPE_POINT);
    m_pCollMap->enableSliding(0);
    m_pCollMap->setShiftY(0.0f);
    m_pCollMap->setStartPos(vStart);
    m_pCollMap->setEndPos(vEnd);
    FLOAT fRideH = fHeight;
    if (m_pCollMap->collideRide(0)) fRideH = pos.y - m_pCollMap->getDetectPos()->y;
    if (m_pCollMap->collideMap()) fHeight = pos.y - m_pCollMap->getDetectPos()->y;
    if (fHeight < fRideH) fHeight = fRideH;
    if (inLimit && pos.y + fHeight <= m_fJumpLandLimit) fHeight = pos.y - (FLOAT)inLimit;
    return fHeight;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getFrontAirHeight__7CPlayerP7FVECTORfff);
void CPlayer::exeAttackTimer() {
    m_tqAttackTimer -= m_tqFrameRate;
    if (m_tqAttackTimer < 0) m_tqAttackTimer = 0;
}
void CPlayer::initStartWait() {
    setPlayerState(START_WAIT);
    setTimer(1500);
    setLand(2.0f, 0.5f, 1);
    m_bAirCheck = FALSE;
    m_bAir = FALSE;
    m_fJumpSpdY = 0.0f;
    m_uiPreAttackKind = 0;
    m_uiAttackKind = 0;
    m_uiAttackID = 0;
    m_tqAttackTimer = 0;
    setPlayerFlag(0x400001, 0);
    offAttack();
    m_WalkMode = 0;
    m_uiPadFlag = (INT)m_uiPadFlag & ~1;
    m_hTarget = 0;
    m_hLockon = 0;
    m_hGimmick = 0;
    m_pCam->setFlag(1, TRUE);
}
void CPlayer::exeStartWait() {
    if (m_bIsBattleScene) {
        setPlayerFlag(0x400000, 0);
    } else if (getTimer() < 1100) {
        if (isAir()) setAnim(6, -1, 0);
        else setAnim(1, -1, 0);
    }
    if (m_pRevolver) setBattleFlag(0, 8);
    if (getTimer() > 0) return;
    if (CGameWork::getFlag(1, 5)) return;
    enableControl();
    m_pCam->setFlag(1, FALSE); // partial CPlayerCamera: (flag, on)
    setPlayerFlag(0, 1);
    m_pMarker->setFlag(TRUE);
    if (!isAir()) initStandUp(0);
    else initFall();
    if (isBattleFlag(1) && !isBattleFlag(0x80000000)) {
        m_vStartPos = getPos();
        setBattleFlag(0x80000000, 0);
    }
}
void CPlayer::initStandUp(INT inFlag) {
    if (isConditionFlag(0x10000000)) {
        initFooting(0);
        return;
    }
    if (!inFlag) {
        setPlayerState(PL::STAND);
        setAnimForce(chkBattleIdle() ? 0x2A : 1, -1, 0, 0, 0, 0);
    } else {
        m_WalkMode = chkWalkMode();
        switch (m_WalkMode) {
        case 0:
            setPlayerState(PL::STAND);
            break;
        case 1:
            setPlayerState(PL::WALK);
            break;
        case 2:
            setPlayerState(PL::RUN);
            break;
        }
        setWalkAnim();
        m_fMovePower = 0.5f;
        m_fWalkSpeed = m_fWalkSpeed / 60.0f * m_fMovePower;
    }
    offAttack();
    setPlayerFlag(0, 0x024C2000);
    m_JumpState = 0;
    m_fJumpSpdY = 0.0f;
    setConditionFlag(0, 0x08810023);
    setBattleFlag(0, 0x00443F90);
    setFaceAnim(0, 0);
    m_pMove->setFlag(0x40, TRUE); // partial CMovement: (flag, on)
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeStandUp__7CPlayer);
INT CPlayer::chkWalkMode() {
    FLOAT aim_power = m_fAnalogPower;
    if (aim_power < 0.3f) {
        m_fWalkSpeed = 0.0f;
        return 0;
    }
    FLOAT dash = 1.0f;
    if (m_pCB && m_pCB->getEnchantKind() == 0x2D) dash = 1.7f;
    if (aim_power < 0.9f) {
        m_fWalkSpeed = m_pParam->MAX_WALK_SPEED * dash;
        return 1;
    }
    m_fWalkSpeed = aim_power * m_pParam->MAX_RUN_SPEED * dash;
    return 2;
}
/* Parked: only the angle-wrap register differs: the original keeps r (aim_dir - dirY, wrapped to +-PI) in $f1,
   ours in $f13. Cause (greg dump): aim_dir dies at the subtraction (sched1 issues the v.y store first), so
   global.c's expand_preferences hands r aim_dir's $f13 preference (from the Homing argument copy). tried:
   _limitPI/_abs helper bodies (return forms, ret local, if/if, volatile/non-volatile asm, fabsf), r as an
   explicit local first/last, the ref's locals at the top, folded calcVelocityXZ argument, the else-block and
   in-place wrap forms, r computed before/between the v stores; best 6 lines. tools/permuter.py can't run on
   C++ methods. */
#if 0
void CPlayer::exeWalk() {
    FLOAT aim_dir = m_fAnalogViewDir;
    FLOAT d0 = Movement::Homing(m_fDir, aim_dir, 0.2f, m_fFrameRate);
    if (m_WalkMode) m_fDir = d0;
    setWalkAnim();
    FLOAT dirY = m_fDir;
    FVECTOR v;
    v._q = 0;
    v.y = aim_dir;
    m_pMove->getRot() = v;
    m_fMovePower = 1.0f - _abs(_limitPI(aim_dir - dirY)) / 3.1415927f;
    m_fWalkSpeed = m_fWalkSpeed / 60.0f * m_fMovePower;
    m_pMove->calcVelocityXZ(m_fWalkSpeed);
    v.y = dirY;
    m_pMove->getRot() = v;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeWalk__7CPlayer);
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setWalkAnim__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkBattleIdle__7CPlayer);
INT CPlayer::procFlowIdle(INT inFlag) {
    if (isAir()) {
        setAnim(6, 800, 0);
        setPlayerFlag(0x800000, 0);
        FVECTOR vel = m_pMove->getVel();
        m_fJumpSpdY += m_fGravity * m_fFrameRate;
        vel.y = m_fJumpSpdY;
        m_pMove->setVel(vel);
        return FALSE;
    }
    if (isPlayerFlag(0x800000)) {
        setPlayerFlag(0, 0x800000);
        setAnim(7, -1, 0);
        return FALSE;
    }
    if (m_iNowAnimID == 7 && !isMotionEnd()) return FALSE;
    setAnim(inFlag ? 0x2A : 1, -1, 0);
    clearSpeed();
    return TRUE;
}
INT CPlayer::initIdle(INT inAir, INT inStand) {
    INT bAir = inAir;
    if (bAir < 0) bAir = isAir();
    if (!bAir) {
        initStandUp(inStand);
    } else {
        initFall();
    }
    return bAir;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkWall__7CPlayeri);
INT CPlayer::chkWallBottom() {
    FVECTOR vStart;
    FVECTOR vEnd;
    FVECTOR vNormal;
    FVECTOR ofs;
    FMATRIX m;
    FLOAT _x = m_vWallPos.x;
    FLOAT _z = m_vWallPos.z;
    vStart.set(_x, m_vWallPos.y + 0.25f, _z);
    vEnd.set(_x, m_vWallPos.y - 0.25f, _z);
    m_pCollMap->setStartPos(vStart);
    m_pCollMap->setEndPos(vEnd);
    if (!m_pCollMap->collideMap()) return FALSE;
    vNormal = *m_pCollMap->getDetectNormal();
    if (vNormal.y < 0.64278764f) return FALSE;
    m_vWallPos.y = m_pCollMap->getDetectPos()->y + 0.0035f;
    ofs.set(0.0f, 0.0f, 0.375f);
    _unitMatrix(m);
    _rotMatrixY(m, m_fWallDir);
    ApplyMatrixXYZ(ofs, m, ofs);
    vStart.set(m_vWallPos.x + ofs.x, m_vWallPos.y + 0.25f, m_vWallPos.z + ofs.z);
    vEnd.set(m_vWallPos.x + ofs.x, m_vWallPos.y - 0.25f, m_vWallPos.z + ofs.z);
    m_pCollMap->setStartPos(vStart);
    m_pCollMap->setEndPos(vEnd);
    return !m_pCollMap->collideMap();
}
void CPlayer::initWall() {
    setPlayerState(PL::WALL);
    clearSpeed();
    offWeapon();
    m_iSubState = 0;
    if (CGame::isSoraGame()) setAnimRsrcEx(0xFD2, 0, -1, 0, 0, 0, 1);
    else setAnimRsrcEx(0xFD4, 0, -1, 0, 0, 0, 1);
    setPos(m_vWallPos);
    m_JumpState = 6;
    m_pMove->setFlag(0x40, FALSE); // partial CMovement: (flag, on)
    m_fFootRadius = 0.5f;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeWall__7CPlayer);
/* FVECTOR::operator-(const FVECTOR&) of the original (ref locals: FVECTOR* this, FVECTOR vec). */
static inline FVECTOR _subVec(const FVECTOR& a, const FVECTOR& inV) {
    FVECTOR vec = a;
    vec.x -= inV.x;
    vec.y -= inV.y;
    vec.z -= inV.z;
    vec.w -= inV.w;
    return vec;
}
INT CPlayer::chkLadderStepNow(BOOL inAir) {
    const FVECTOR* pos;
    const FVECTOR* nor;
    FVECTOR dif;
    INT step;
    INT max;
    if (!isActionFlag(0x1000)) return -1;
    if (!inAir && m_fAnalogPower < 0.3f) return -1;
    pos = m_pMove->getLadderPos();
    nor = m_pMove->getLadderNormal();
    if (!pos || !nor) return -1;
    dif = _subVec(*pos, getPos());
    dif.y = 0.0f;
    dif.w = 1.0f;
    if (Math3D::VectorLength(dif) > 0.75f) return -1;
    setPlayerFlag(0x2000, 0);
    FLOAT r = atan2f(nor->x, nor->z) - m_fDir;
    if (r < -3.1415927f) r += 6.2831855f;
    else if (r > 3.1415927f) r -= 6.2831855f;
    if (getHoukouFromAngle(r) != 1) return -1;
    step = (INT)((getPos().y - pos->y) / 0.375f);
    max = (INT)(m_pMove->getLadderHeight() / 0.375f) - 4;
    if (step >= max) return -1;
    dbgCheckUser(KKIKUCHI);
    return step;
}
INT CPlayer::getLadderStepNow(FLOAT inHeight) {
    return (INT)((inHeight - m_vLadderPos.y + 0.0035f) / 0.375f);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initLadder__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeLadder__7CPlayer);
/* Parked: everything but the ofs block matches (wrap: r wrapped in place, then dir = r); left: after getPos the
   original issues lq pos (v1) before lui LADDER_LOCAL_OFS (v0), ours schedules the lui first (sched1 priority:
   hi->lwc1->swc1 chain) and allocates pos to a1. tried: all orders of pos/ofs statements (perm.py), set(),
   brace init (adds a temp copy), x = y = 0 chains, FVECTOR pos = getPos(), extern const; best 10 lines.
   tools/permuter.py can't run on C++ methods. */
#if 0
INT CPlayer::isStartLadderJump() {
    FLOAT dir;
    FVECTOR pos;
    FVECTOR ofs;
    FMATRIX m;
    if (!isTrgFlag(1)) return FALSE;
    dir = m_fDir;
    pos = getPos();
    ofs.x = 0.0f;
    ofs.y = 0.0f;
    ofs.z = PL::LADDER_LOCAL_OFS.z;
    ofs.w = 1.0f;
    _unitMatrix(m);
    _rotMatrixY(m, dir);
    ApplyMatrixXYZ(ofs, m, ofs);
    AddVectorXYZ(pos, pos, ofs);
    setPos(pos);
    {
        FVECTOR rot = getRot();
        FLOAT r = dir - 3.1415927f;
        if (r < -3.1415927f) r += 6.2831855f;
        else if (r > 3.1415927f) r -= 6.2831855f;
        dir = r;
        m_fDir = dir;
        rot.y = dir;
        setRot(rot);
    }
    setAnimForce(5, 0, 0, 0, 0, 0);
    initFall();
    m_fWalkSpeed = m_pParam->MAX_WALK_SPEED / 60.0f;
    m_pMove->calcVelocityXZ(m_fWalkSpeed);
    {
        FVECTOR vel = m_pMove->getVel();
        vel.y = PL::g_fAntiGravity * 18.0f;
        m_fJumpSpdY = vel.y;
        m_pMove->setVel(vel);
    }
    m_pMove->setFlag(0x40, TRUE); // partial CMovement: (flag, on)
    return TRUE;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isStartLadderJump__7CPlayer);
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getJumpState__C7CPlayer);
void CPlayer::initJump(INT inFlag) {
    setPlayerState(PL::JUMP);
    if (DebugWork::g_dw.player.iHiJump) setActionFlag(0x200000, 0);
    else setActionFlag(0, 0x200000);
    if (CGame::isSoraGame()) {
        UINT uiActionSkill = CGameWork::getInt(0, 0x24);
        if (uiActionSkill & 4) setActionFlag(0x800000, 0);
        else setActionFlag(0, 0x800000);
        if (uiActionSkill & 2) setActionFlag(0x400000, 0);
        else setActionFlag(0, 0x400000);
        if (DebugWork::g_dw.player.iGlide) setActionFlag(0x400000, 0);
        if (DebugWork::g_dw.player.iGlideSP) setActionFlag(0x800000, 0);
    }
    if (m_fAnalogPower >= 0.3f) {
        m_fDir = m_fAnalogViewDir;
        FVECTOR rot = getRot();
        rot.y = m_fDir;
        setRot(rot);
    }
    setPlayerFlag(0, 0x1000000);
    if (!inFlag) {
        if (isActionFlag(0x20000) && chkGimmick(0x2002) == 7) {
            setPlayerFlag(0x1000000, 0);
            setAnimForce(0x14, -1, 0, 0, 0, 0);
            m_fJumpPow = m_fGimmickJumpPow;
        } else if (isActionFlag(0x200000)) {
            setAnimForce(0x14, -1, 0, 0, 0, 0);
            m_fJumpPow = m_pParam->HIJUMP_POWER;
        } else {
            setAnimForce(4, -1, 0, 0, 0, 0);
            m_fJumpPow = m_pParam->JUMP_POWER;
        }
        m_vJumpStartPos = getPos();
        m_fJumpSpdXZ = m_fWalkSpeed * 0.8f;
        m_fJumpTop = 0.0f;
        m_fJumpSpdY = 0.0f;
    } else {
        if (m_fJumpSpdY > 0.0f) m_fJumpSpdY = 0.0f;
        m_fJumpPow = 0.0f;
        m_fJumpSpdXZ = m_fWalkSpeed;
    }
    m_fJumpDir = m_fDir;
    m_JumpState = 1;
    m_fJumpGravity = 0.0f;
    m_tqJumpTimer = 0;
    m_fJumpHeight = 0.0f;
}
void CPlayer::initFall() {
    initJump(1);
    *(INT*)&m_JumpState = 3;
    m_fJumpGravity = PL::g_fGravity;
    setAnimForce(6, 800, 0, 0, 0, 0);
    offAttack();
    setPlayerFlag(0, 0x020C2000);
    setBattleFlag(0, 0x00443F90);
    setFaceAnim(0, 0);
    m_pMove->setFlag(0x40, TRUE); // partial CMovement: (flag, on)
}
void CPlayer::setFallFloat() {
    FVECTOR v = m_pMove->getVel();
    m_fJumpSpdY = 0.0f;
    v.y = m_fJumpSpdY;
    m_pMove->getVel() = v;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeJump__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeJumpLand__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkHang__7CPlayer);
void CPlayer::initHang() {
    FVECTOR rot;
    FVECTOR ofs;
    FMATRIX m;
    m_vOldSafeField = getPos();
    setPlayerState(PL::HANG);
    m_iSubState = 0;
    setPlayerFlag(0x400000, 0);
    setBattleFlag(0, 8);
    rot = getRot();
    FLOAT dir = m_fDir;
    rot.y = dir;
    setRot(rot);
    _unitMatrix(m);
    _rotMatrixY(m, dir);
    ApplyMatrixXYZ(ofs, m, PL::HANG_HAND_LOCAL_OFS);
    SubVectorXYZ(m_vHangPos, m_vHangPos, ofs);
    m_vHangPos.w = 1.0f;
    if (isPlayerFlag(0x8000)) {
        setPos(m_vHangPos);
        setConditionFlag(0, 0x40000);
        m_pCollMap->registerLastHang(getHandle());
        m_pCollMap->setHangPosition(rot, m_vHangPos);
    } else {
        warp(m_vHangPos);
    }
    setAnimForce(0xF, 0, 0, 0, 0, 100);
    updateCoord();
    coordFix();
    m_pMove->setFlag(0x40, FALSE); // partial CMovement: (flag, on)
    m_pCam->setFlag(2, TRUE);      // partial CPlayerCamera: (flag, on)
    m_JumpState = 6;
    clearSpeed();
    m_fJumpSpdY = 0.0f;
    if (m_pMarker) m_pMarker->cancelLockon(m_hLockon, 0);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeHang__7CPlayer);
void CPlayer::initAvoid() {
    if (isConditionFlag(0x10000000) && !isConditionFlag(0x400000)) {
        initFooting(2);
        return;
    }
    setPlayerState(PL::AVOID);
    setBattleFlag(0, 8);
    setPlayerFlag(1, 0);
    clearSpeed();
    if (m_fAnalogPower >= 0.3f) {
        m_fDir = m_fAnalogViewDir;
        FVECTOR rot = getRot();
        rot.y = m_fDir;
        setRot(rot);
    }
    updateCoord();
    setAnimForce(0x16, -1, TRUE, 0, 0, 0);
    m_fJumpSpdY = 0.0f;
    m_JumpState = 3;
    m_fJumpGravity = *(FLOAT*)&PL::g_fGravity;
}
void CPlayer::exeGimmickRide() {
    if (m_pMove->isObjRide() && chkGimmick(0x2002) == 7 && !CGameWork::getFlag(1, 0x27)) {
        CEntity* pEntity = (CEntity*)CTaskMan::getTask(m_hGimmick);
        if (pEntity) {
            FVECTOR target = pEntity->getPos();
            FVECTOR pos = getPos();
            if (calcDistanceXZSq(target) < 0.010000001f) {
                pos.x = target.x;
                pos.z = target.z;
            } else {
                pos.x = Movement::Approach(pos.x, target.x, 0.1f, m_fFrameRate);
                pos.z = Movement::Approach(pos.z, target.z, 0.1f, m_fFrameRate);
            }
            setPos(pos);
        }
    }
}
void CPlayer::notifyGimmickJump(HANDLE inHandle, INT inPow) {
    if (!_instance()) return;
    _instance()->m_hGimmickJump = inHandle;
    if (inPow == -1) {
        _instance()->m_fGimmickJumpPow = PL::g_fGimmickJumpPower[((CBaseGimmick*)CTaskMan::getTask(inHandle))->getParam()];
    } else {
        _instance()->m_fGimmickJumpPow = PL::g_fGimmickJumpPower[inPow];
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkGimmick__7CPlayerUi);
void CPlayer::initTreasureBox() {
    CEntity* pTarget;
    FVECTOR rot;
    FVECTOR pos;
    setPlayerState(TREASUREBOX);
    setPlayerFlag(0x400001, 0);
    clearSpeed();
    m_collBody.off();
    onWeapon(1);
    m_pMarker->setFlag(FALSE);
    setAnimRsrcEx(0xFDC, 0, 0, 0, 0, 0, 1);
    m_bDmgMuteki = TRUE;
    pTarget = (CEntity*)CTaskMan::getTask(m_hGimmick);
    FLOAT d = pTarget->getRot().y + 3.1415927f;
    FLOAT r;
    if (d < -3.1415927f) r = d + 6.2831855f;
    else {
        r = d;
        if (r > 3.1415927f) r -= 6.2831855f;
    }
    m_fDir = r;
    rot = getRot();
    rot.y = m_fDir;
    setRot(rot);
    pos = pTarget->getPos();
    FVECTOR ofs = {{0.0f, 0.0f, -1.457f, 1.0f}};
    FMATRIX m;
    _unitMatrix(m);
    _rotMatrixY(m, m_fDir);
    ApplyMatrixXYZ(ofs, m, ofs);
    AddVectorXYZ(pos, pos, ofs);
    pos.y += 0.0035f;
    setPos(pos);
    setOldPos(pos);
}
void CPlayer::exeTreasureBox() {
    setPlayerFlag(0x400000, 0);
    if (isMotionEnd()) {
        m_bDmgMuteki = FALSE;
        setPlayerFlag(0, 1);
        m_collBody.on();
        initStandUp(0);
        m_pMarker->setFlag(TRUE);
        m_pMarker->holdTarget(0);
    }
}
void CPlayer::initRoomCreation() {
    CEntity* pTarget;
    FVECTOR rot;
    FVECTOR pos;
    setTrgFlag(0, 0x18000);
    setPlayerState(ROOMCREATION);
    m_iSubState = 0;
    clearSpeed();
    m_collBody.off();
    m_bDmgMuteki = TRUE;
    offWeapon();
    m_pMarker->setFlag(FALSE);
    setAnimForce(1, -1, 0, 0, 0, 0);
    pTarget = (CEntity*)CTaskMan::getTask(m_hGimmick);
    FLOAT d = pTarget->getRot().y + 3.1415927f;
    FLOAT r;
    if (d < -3.1415927f) r = d + 6.2831855f;
    else {
        r = d;
        if (r > 3.1415927f) r -= 6.2831855f;
    }
    m_fDir = r;
    rot = getRot();
    rot.y = m_fDir;
    setRot(rot);
    pos = pTarget->getPos();
    FVECTOR ofs = {{0.0f, 0.0f, -2.0f, 1.0f}};
    FMATRIX m;
    _unitMatrix(m);
    _rotMatrixY(m, m_fDir);
    ApplyMatrixXYZ(ofs, m, ofs);
    AddVectorXYZ(pos, pos, ofs);
    pos.y += 0.0035f;
    setOldPos(pos);
    setPos(pos);
    setLand(1.0f, 0.5f, 1);
    m_pCam->initCamera(1, 1, NULL);
    m_pCam->setFlag(1, TRUE);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeRoomCreation__7CPlayer);
void CPlayer::initWorldSelect() {
    CEntity* pTarget;
    FVECTOR rot;
    FVECTOR pos;
    setTrgFlag(0, 0x18000);
    setPlayerState(WORLDSELECT);
    m_iSubState = 0;
    clearSpeed();
    m_collBody.off();
    m_bDmgMuteki = TRUE;
    offWeapon();
    m_pMarker->setFlag(FALSE);
    setAnimForce(1, -1, 0, 0, 0, 0);
    pTarget = (CEntity*)CTaskMan::getTask(m_hGimmick);
    FLOAT d = pTarget->getRot().y + 3.1415927f;
    FLOAT r;
    if (d < -3.1415927f) r = d + 6.2831855f;
    else {
        r = d;
        if (r > 3.1415927f) r -= 6.2831855f;
    }
    m_fDir = r;
    rot = getRot();
    rot.y = m_fDir;
    setRot(rot);
    pos = pTarget->getPos();
    FVECTOR ofs = {{0.0f, 0.0f, -2.0f, 1.0f}};
    FMATRIX m;
    _unitMatrix(m);
    _rotMatrixY(m, m_fDir);
    ApplyMatrixXYZ(ofs, m, ofs);
    AddVectorXYZ(pos, pos, ofs);
    pos.y += 0.0035f;
    setPos(pos);
    setOldPos(pos);
    m_pCam->initCamera(1, 1, NULL);
    m_pCam->setFlag(1, TRUE);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeWorldSelect__7CPlayer);
void CPlayer::initCarry() {
    setPlayerState(PL::CARRY);
    m_iSubState = 0;
    clearSpeed();
    offWeapon();
    setAnimForce(0x17, 0, 0, 0, 0, 0);
    m_pCam->setFlag(0x800, TRUE); /* the partial CPlayerCamera keeps (flag, on/off): the original is setFlag(0x800, 0) */
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeCarry__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", resetAction__7CPlayeri);
/* Parked: registers match; only the stores after the g_fGravity load are scheduled differently (the original
   stores m_JumpState right after the load and m_fJumpGravity before the g_CardBreakTime load; ours stores
   them later). tried: all 24 orders of the JumpState/CardBreakTime/JumpGravity/JumpSpdY block, cast loads
   (*(FLOAT*)&g_fGravity, *(INT*)&g_CardBreakTime[7]) and cast stores, gravity via a local / *=, moving
   statements before isAir(), asm barriers; best 6 lines. The *(FLOAT*)& store on m_fMoveSpdXZ and the
   chained m_fDistXZ = m_fDistY = 0.0f fixed the rest. tools/permuter.py can't run on C++ methods. */
#if 0
void CPlayer::initAttackField(INT inNo) {
    setPlayerState(PL::ATTACK_FLD);
    m_iAerialAnimShift = 0;
    setTrgFlag(0, 4);
    setPlayerFlag(0x100000, 0);
    m_pMarker->holdTarget(8);
    m_uiCardID = 0;
    m_iAttackNo = inNo;
    if (!inNo) m_uiPreAttackKind = 0;
    m_bIsAttackHit = FALSE;
    m_bNextTrg = FALSE;
    m_iNextAttack = 0;
    *(FLOAT*)&m_fMoveSpdXZ = isAir() ? m_fJumpSpdXZ : m_fWalkSpeed;
    m_JumpState = 3;
    m_tqCardBreakTime = PL::g_CardBreakTime[7];
    m_fJumpGravity = PL::g_fGravity * 0.16f;
    m_fJumpSpdY = 0.0f;
    setAttack();
    m_fDistXZ = m_fDistY = 0.0f;
    if (m_pAtkData->FLAG & 8) {
        setPlayerFlag(0x2800000, 0);
    } else {
        setPlayerFlag(0, 0x2800000);
        m_fJumpGravity = PL::g_fGravity;
    }
    onWeapon(1);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initAttackField__7CPlayeri);
#endif
void CPlayer::exeAttackField() {
    TIMEQ frameTQ = getMotionCurrentTime();
    TIMEQ frame = frameTQ / 50;
    procGravity(frame, m_pAtkData->MOVE_END_FRAME, 1);
    if (frame <= m_pAtkData->MOVE_START_FRAME) {
        procTargetMark();
        procAttackMove(frame, 1.0f, 4.0f, 2, 1.0f);
    } else {
        FLOAT rate = 0.0f;
        if (frame < 6 && (m_pAtkData->FLAG & 8)) {
            procTargetMark();
            rate = 1.0f;
        }
        procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, !m_bIsAttackHit, rate);
    }
    if (frame > m_pAtkData->COLL_START_FRAME && frameTQ > 500) {
        if (m_iNextAttack) {
            offAttack();
            initAttackField(m_iAttackNo);
            return;
        }
        if (m_bIsCanCombo && m_bIsAttackHit && m_iAttackNo < m_iMaxCombo && isTrgAttack()) {
            m_iNextAttack = 1;
            return;
        }
    }
    if (procAttackCollision(frame)) return;
    if (isPlayerFlag(0x800000) && !isAir() && frame > m_pAtkData->COLL_END_FRAME) {
        procSkillEnd(1);
        return;
    }
    if (isMotionEnd()) procSkillEnd(-1);
}
void CPlayer::chkLockon() {
    HANDLE near;
    INT bCheck;
    if (!m_pMarker->m_bFlag) return;
    if (isPlayerFlag(0x10000)) return;
    if (m_PlayerState == 0x1E) near = LockOnManager::CLockOnManager::getLockOnMapEnemy(*this);
    else near = LockOnManager::CLockOnManager::getLockOnObject(*this);
    if (near == -1) near = 0;
    m_hTarget = near;
    bCheck = FALSE;
    if (m_bIsBattleScene && m_PlayerState != HANG && !CGameWork::getFlag(1, 0xC)) {
        if (!m_pMarker->m_iMode) {
            if (isTrgLockon()) {
                m_pMarker->changeMode(m_pMarker->m_iMode ^ 1);
                bCheck = TRUE;
                m_pMarker->chkLockon(m_hTarget);
                if (m_hTarget > 0) CSound::playSE(7, -1, 0x40);
                setTrgFlag(0, 0x1000);
            }
        } else if (isRTrgLockon() && (m_uiTrgFlag & 0x1000)) {
            m_pMarker->changeMode(m_pMarker->m_iMode ^ 1);
            bCheck = TRUE;
            m_pMarker->chkLockon(m_hTarget);
            if (m_hTarget > 0) CSound::playSE(8, -1, 0x40);
        } else if (isDownLockon()) {
            if (isTrgLockon()) setTrgFlag(0x1000, 0);
            if (isChangeHoukouR()) {
                HANDLE hChange = LockOnManager::CLockOnManager::changeLockOnObject(m_hLockon);
                if (hChange > 0) {
                    m_hTarget = hChange;
                    m_pMarker->setMode(1);
                    bCheck = TRUE;

                    m_pMarker->chkLockon(m_hTarget);
                    CSound::playSE(7, -1, 0x40);
                }
            }
            setTrgFlag(0x800, 0);
        } else {
            setTrgFlag(0, 0x1000);
        }
    }
    if (!bCheck) m_pMarker->chkLockon(m_hTarget);
    m_hLockon = m_pMarker->m_hLockon;
    m_vLockonPos = m_pMarker->m_vCameraTargetPos;
    if (m_hLockon > 0) m_hTarget = m_hLockon;
}
void CPlayer::cancelLockon() {
    m_pMarker->cancelLockon(m_hLockon, TRUE);
}
void CPlayer::autoLockon() {
    if (m_bIsBattleScene && !m_hLockon) m_pMarker->setMode(1);
}
void CPlayer::setPushon(INT inFlag) {
    if (m_bIsBattleScene) {
        if (inFlag) {
            m_pMarker->onPush(inFlag);
        } else {
            m_pMarker->offPush();
            m_uiTrgFlag &= ~0x40000;
        }
    }
}
void CPlayer::setTrgFlagRendaNG(const CHAR* inName) {
    if (!DebugWork::g_dw.player.iRenda) m_uiTrgFlag |= 0x40000;
}
void CPlayer::chkPlayCard() {
    if (m_bIsDead) return;
    if (!m_pRevolver) return;
    BOOL bExeSkill = FALSE;
    INT iStock = m_pRevolver->getStock(1);
    if (isTrgStock()) {
        if (iStock < 3) m_pRevolver->setStockCard();
        else bExeSkill = TRUE;
    }
    if (!isBattleFlag(8)) {
        setBattleFlag(8, 0);
        return;
    }
    if (bExeSkill || (isTrgStockSkill() && iStock > 0)) {
        INT result = m_pRevolver->setPlayStock(0, 1);
        if (result) {
            TRACE(D_00499F48);
            setBattleFlag(0, 8);
            if (result == 1) {
                setBattleFlag(0x10200000, 0x20);
            } else {
                TRACE(D_00499F70);
                setBattleFlag(0, 0x10200000); setBattleFlag(0x20, 0);
            }
            return;
        }
    }
    if (!isTrgAttack()) return;
    if (isBattleFlag(0x10)) return;
    if (isBattleFlag(0x100)) return;
    if (isBattleFlag(0x80)) return;
    if (isBattleFlag(0x200)) return;
    if (isBattleFlag(0x400)) return;
    if (isBattleFlag(0x800)) return;
    if (m_tqAttackTimer > 0) return;
    if (!isActionFlag(0x40)) return;
    INT mode = m_pRevolver->getMode();
    INT result = m_pRevolver->setPlayCard(0, 1);
    if (!result) return;
    setBattleFlag(0, 8);
    if (mode) return;
    if (result == 1) {
        TRACE(D_00499F80);
        setBattleFlag(0x10200000, 0x20);
    } else {
        TRACE(D_00499FA0);
        setBattleFlag(0, 0x10200000); setBattleFlag(0x20, 0);
    }
}
/* Parked: the switch's jump table goes to .rodata, which is stripped until the unit's data comes from source
   (objcopy: symbol `.rodata' required but not present); tried: C switch over PL::ATTACK/ITEM/MAGIC/SKILL/
   GIMMICKCARD/FRIEND/SUMMON/DUEL (the code apart from the table is the original's shape) */
#if 0
INT CPlayer::isNowCardActionState() {
    switch (m_PlayerState) {
    case PL::ATTACK:
    case PL::ITEM:
    case PL::MAGIC:
    case PL::SKILL:
    case PL::GIMMICKCARD:
    case PL::FRIEND:
    case PL::SUMMON:
    case PL::DUEL:
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isNowCardActionState__7CPlayer);
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkPlayCardCombo__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", onMsg__7CPlayeriPv);
INT CPlayer::exeUseStock(INT inFlag) {
    if (isBattleFlag(4) || m_bIsDead) return FALSE;
    switch (m_iStepStockSkill) {
    case 0:
        if (m_iFirstSkill <= 0 && startStockSkillAction()) return TRUE;
        return startSingleAction();
    case 1:
        if (!inFlag) return FALSE;
        if (startStockSkillAction()) return TRUE;
        return startSingleAction();
    case 2:
        if (!inFlag) return FALSE;
        return startSingleAction();
    default:
        return FALSE;
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", startSingleAction__7CPlayer);
/* Parked: same spot as initAttackField: around the g_fGravity load the original keeps the %hi in v0 (later
   s0) and stores m_JumpState after the load; ours uses v1 and stores it before, and the g_CardBreakTime %hi
   moves. The flag block (battle 0x10/0x80000/8, then trg 4) and the ternary m_iComboBoost match.
   tried: block perms (perm.py, hillmove.py), cast loads/stores on g_fGravity/g_CardBreakTime/
   m_JumpState/m_fJumpGravity, gravity via a local; best 13 lines. tools/permuter.py can't run on C++ methods. */
#if 0
void CPlayer::initAttack(INT inNo) {
    setPlayerState(PL::ATTACK);
    setBattleFlag(0, 0x10);
    setBattleFlag(0, 0x80000);
    setBattleFlag(0, 8);
    setTrgFlag(0, 4);
    setPlayerFlag(0x100000, 0);
    fadeEffAttack(0);
    m_iAerialAnimShift = 0;
    m_pMarker->holdTarget(8);
    m_iAttackNo = inNo;
    if (!inNo) m_uiPreAttackKind = 0;
    m_bIsAttackHit = FALSE;
    m_bNextTrg = FALSE;
    m_iNextAttack = 0;
    *(FLOAT*)&m_fMoveSpdXZ = isAir() ? m_fJumpSpdXZ : m_fWalkSpeed;
    m_JumpState = 3;
    m_fJumpGravity = PL::g_fGravity * 0.16f;
    m_tqCardBreakTime = PL::g_CardBreakTime[(*(BC_PARAM**)&CardBattle::g_pBattleCardList)[m_uiCardID].BREAK];
    m_iComboBoost = isConditionFlag(0x40000000) ? m_iComboBoost + 20 : 0;
    setAttack();
    m_fDistXZ = m_fDistY = 0.0f;
    if (m_pAtkData->FLAG & 8) {
        setPlayerFlag(0x2800000, 0);
    } else {
        setPlayerFlag(0, 0x2800000);
        m_fJumpGravity = PL::g_fGravity;
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initAttack__7CPlayeri);
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setAttack__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeAttack__7CPlayer);
void CPlayer::procTargetMark() {
    CEntity* pTarget = m_hTarget > 0 ? (CEntity*)CTaskMan::getTask(m_hTarget) : NULL;
    if (pTarget) {
        m_hMark = m_hTarget;
        pTarget->getLockOnPos(m_vMarkPos);
    } else {
        m_hMark = 0;
        m_vMarkPos.set(0.0f, 0.0f, 0.0f);
    }
}
void CPlayer::procAttackMove(TIMEQ inNowFrame, FLOAT inRadius, FLOAT inMaxSpeed, INT inHomingFlag, FLOAT inRate) {
    FLOAT speed = 0.0f;
    FVECTOR vec;
    FVECTOR dir;
    if (inHomingFlag && m_hMark > 0) {
        SubVectorXYZ(vec, m_vMarkPos, m_vCenterPos);
        Normalize(dir, vec);
        vec.y = 0.0f;
        m_fDistXZ = _sqrtf(InnerProduct(vec, vec));
        FLOAT d = Movement::Homing(m_fDir, atan2f(dir.x, dir.z), inRate, m_fFrameRate);
        vec.y = d;
        vec.x = 0.0f;
        vec.z = 0.0f;
        vec.w = 1.0f;
        m_fDir = d;
        m_pMove->getRot() = vec;
        if (inHomingFlag != 2 && inNowFrame >= m_pAtkData->MOVE_START_FRAME && inNowFrame < m_pAtkData->MOVE_END_FRAME) {
            if (inRadius < m_fDistXZ) speed = inMaxSpeed / 60.0f;
            else if (m_pAtkData->FLAG & 4) m_hMark = 0;
        }
    }
    if ((m_pAtkData->FLAG & 0x10) && inNowFrame < m_pAtkData->MOVE_END_FRAME) {
        speed += (SBYTE)m_pAtkData->WORK0 / 60.0f;
    }
    if (isPlayerFlag(0x800000)) {
        if (speed > 0.0f) m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, speed, 0.2f, m_fFrameRate);
        else m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, speed, 0.1f, m_fFrameRate);
    } else if (m_pAtkData->FLAG & 4) {
        if (speed > 0.0f) m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, speed, 0.2f, m_fFrameRate);
        else m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, speed, 0.05f, m_fFrameRate);
    } else {
        if (speed > 0.0f) m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, speed, 0.2f, m_fFrameRate);
        else m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, speed, 0.4f, m_fFrameRate);
    }
    m_pMove->calcVelocityXZ(m_fMoveSpdXZ);
}
INT CPlayer::procAttackCollision(TIMEQ inNowFrame) {
    if (inNowFrame >= m_pAtkData->COLL_START_FRAME) {
        if (inNowFrame < m_pAtkData->COLL_END_FRAME) {
            if (!m_pWeapon->isOnAttack()) onAttack();
            switch (m_pWeapon->isHit()) {
            case 1:
            case 2:
                m_bIsAttackHit = TRUE;
                setPlayerFlag(0, 0x100000);
                break;
            case 3:
                if (m_pCB) {
                    m_pCB->destroyPlayCard(1, 0);
                    setBattleFlag(0, 0x10200000);
                }
                initFlip(0);
                return TRUE;
            }
        } else {
            offAttack();
            setPlayerFlag(0, 0x100000);
        }
    }
    return FALSE;
}
void CPlayer::procGravity(TIMEQ inNowFrame, TIMEQ inEndFrame, INT inFlag) {
    FVECTOR vel = m_pMove->getVel();
    m_fJumpSpdY += m_fJumpGravity * m_fFrameRate;
    if (inFlag && isPlayerFlag(0x2000000)) m_fJumpGravity = PL::g_fGravity * 0.16f;
    else m_fJumpGravity = PL::g_fGravity;
    if (isPlayerFlag(0x800000) && inNowFrame < inEndFrame) {
        BOOL bFloat = TRUE;
        if (m_hMark > 0) {
            FVECTOR vec;
            SubVectorXYZ(vec, m_vMarkPos, m_vCenterPos);
            m_fDistY = _abs(vec.y);
            if (vec.y < 0.0f) {
                if (m_fJumpSpdY > 0.0f) {
                    if (m_fDistY < 0.5f) m_fJumpSpdY *= PL::g_fJumpStopRate;
                    else m_fJumpSpdY = 0.0f;
                }
                bFloat = FALSE;
            }
        }
        if (bFloat && m_fJumpSpdY < 0.0f) m_fJumpSpdY = 0.0f;
    }
    if (!(inNowFrame < inEndFrame) && m_fJumpSpdY > 0.0f) m_fJumpSpdY = 0.0f;
    vel.y = m_fJumpSpdY;
    m_pMove->setVel(vel);
}
void CPlayer::procSkillEnd(INT inAir) {
    INT bAir = inAir;
    if (bAir < 0) bAir = isAir();
    offAttack();
    setPushon(0);
    m_pMarker->holdTarget(0);
    if (isPlayerFlag(0x80000000)) setBGFade(1.0f, 20.0f, 0);
    fadeEffAttack(0);
    setBattleFlag(0, 0x00480F90);
    setBattleFlag(0, 0x00041000);
    setBattleFlag(0, 0x10200000);
    setBattleFlag(0, 0x00130000);
    setBattleFlag(0, 0x03000000);
    m_fWalkSpeed = m_fJumpSpdXZ = m_fMoveSpdXZ;
    if (m_PlayerState == 0x22) setPlayerFlag(0, 1);
    setUseRsrcBlock(0);
    if (isBattleFlag(4)) {
        initEnd();
    } else if (chkExtraSkill()) {
    } else if (isBattleFlag(0x2000)) {
        initIdle(bAir, 0);
        setBattleFlag(0x41000, 0x2000);
        m_iStepStockSkill = 0;
        TRACE(D_0049A888);
    } else {
        if (m_pCB) {
            m_pCB->destroyPlayCard(1, 0);
            setBattleFlag(0, 0x10200000);
        }
        initIdle(bAir, 0);
        m_uiAttackKind = 0;
        m_uiAttackID = 0;
        if (!m_iNextAttack) m_iAttackNo = 0;
        m_bIsAttackHit = FALSE;
        m_bNextTrg = FALSE;
        m_iNextAttack = 0;
        m_bIsCanCombo = FALSE;
    }
}
BOOL CPlayer::chkExtraSkill() {
    if (m_iExtraSkill) {
        m_iStepStockSkill = 2;
        m_iExtraSkill = 0;
        if (exeUseStock(1)) return TRUE;
    }
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", calcEnchant_AttackPoint__7CPlayerRiR8ATK_KIND8ATK_ATTRUi);
void CPlayer::calcEnchant_DamagePoint(INT& inPoint, ATK_ATTR inAttr) {
    if (m_pCB) {
        CardBattle::ENCHANT_PARAM param;
        param.init(getHandle(), 1);
        param.uiState = 2;
        param.AtkAttr = inAttr;
        param.iAttackPoint = inPoint;
        m_pCB->calcEnchantParam(param);
        inPoint = param.iAttackPoint;
        if (param.iGuard > 0) Gauge::CCommonGauge::setDispSeq(getHandle(), 2);
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setUseRsrcBlock__7CPlayeri);
void CPlayer::setDefAttr(FLOAT* inAttr) {
    for (INT i = 0; i < 6; i++) {
        m_fDefAttr[i] = inAttr[i];
    }
}
FLOAT CPlayer::getDefAttr(ATK_ATTR inAttr) const {
    if ((UINT)inAttr < 6) return m_fDefAttr[inAttr];
    return m_fDefAttr[5];
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkDamage__7CPlayer);
void CPlayer::initFlip(INT inBreak) {
    FLOAT speed;
    INT animID;
    setBattleFlag(0, 8);
    setPlayerState(FLICKS);
    if (inBreak) {
        TRACE(D_0049AC10);
        speed = -8.0f;
        m_iComboBoost = 0;
        m_iSubState = 2;
    } else {
        TRACE(D_0049AC30);
        speed = -4.0f;
        m_iSubState = 1;
        m_tqCardBreakTime = PL::g_CardBreakTime[5];
    }
    setBattleFlag(0, 0x40);
    PLM::setVibration(3);
    resetAction(5);
    m_fWalkSpeed = 0.0f;
    m_bDmgInAir = isAir();
    if (m_bDmgInAir) {
        if (getAirHeight(NULL, 10.0f, 1) < 0.7f) {
            setLand(1.0f, 0.5f, 1);
            animID = inBreak ? 0x25 : 0x24;
            m_bDmgInAir = FALSE;
        } else {
            animID = 0x26;
        }
    } else {
        animID = inBreak ? 0x25 : 0x24;
    }
    switch (m_uiAttackID) {
    case 0xD:
    case 0x30:
    case 0x31:
    case 0x32:
    case 0x33:
    case 0x34:
        setAnimForce(0x42, 0, 0, 0, 0, 0);
        speed = 0.0f;
        break;
    default:
        setAnimForce(animID, 0, 1, 0, 0, 0);
        break;
    }
    m_fJumpSpdY = 0.0f;
    m_fJumpSpdXZ = speed / 60.0f;
    m_fJumpGravity = *(FLOAT*)&PL::g_fGravity;
    m_pMove->calcVelocityXZ(m_fJumpSpdXZ);
    m_JumpState = 3;
}
void CPlayer::setFlipForce(const FVECTOR* inPos) {
    FVECTOR pos;
    if (inPos) {
        pos = *inPos;
    } else {
        pos = m_vStartPos;
        pos.y = getPos().y + 0.0035f;
    }
    warp(pos);
    updateCoord();
    coordFix();
    clearSpeed();
    initFlip(0);
    CardBattle::CCardBattle::notifyCompleteGimmick();
    PLM::setVibration(3);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeDamage__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkTrgAction__7CPlayeriUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initDead__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeDead__7CPlayer);
void CPlayer::notifyDead(INT inCmd) {
    switch (inCmd) {
    case 0:
        clearSpeed();
        setBattleFlag(0, 0x00443F90);
        setBattleFlag(0, 0x10200000);
        setBattleFlag(0, 0x4060);
        setBattleFlag(0, 0x00130000);
        setPlayerFlag(0, 0x020C2000);
        setBattleFlag(0, 0x03000000);
        CEffectMan::killAll((ObjType::eObjBit)2, 0.0f);
        m_pMove->setFlag(0x10, TRUE);
        CEffectMan::killAll(*(LPCSTR*)&PL::g_szDpxBreak, 0.0f);

        CEffectMan::killAll(PL::g_szDpxSpeedBreak, 0.0f);
        CEffectMan::killAll(PL::g_szDpxHitWall, 0.0f);
        if (isConditionFlag(0x20000)) setAnimForce(0x11, 0, 0, 0, 0, 0);
        else setAnimForce(0x11, 0, 0, 1, 0, 0);
        importStatus();
        m_iHP = m_iMaxHP;
        if (m_pCB) {
            CardBattle::CCardBattle::setBattleEnd();
            m_pRevolver = NULL;
        }
        m_iSubState = 1;
        m_bDmgMuteki = TRUE;
        m_pMove->setFlag(1, TRUE);
        setPlayerFlag(1, 0);
        m_collBody.off();
        m_collBody.offBody();
        break;
    case 1:
        m_iSubState = 3;
        setAnimForce(0x27, -1, 0, 0, 0, 0);
        setPlayerFlag(1, 0);
        m_collBody.off();
        m_pCB->destroyPlayCard(1, 0);
        setBattleFlag(0, 0x10200000);
        onWeapon(0);
        setLand(5.0f, 0.5f, 1);
        CSceneEnv::setStop((ObjType::eObjBit)4);
        break;
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initBadCondition__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeBadCondition__7CPlayer);
/* Parked: in the set arm the original loads ConditionFlag (v1), li 21000 (v0), PadFlag (a2); this order gets
   the registers right but schedules the PadFlag load first; source order cond/tq/pad gives the load order but
   swaps v0/a2 (tq/pad). tried: all 6 statement orders x tq forms (ternary, param default, ==0), UINT& pad,
   (INT) casts, temporaries in both load orders, nested-if structure, asm barriers on tq; best 6 lines.
   tools/permuter.py can't run on C++ methods. */
#if 0
void CPlayer::setConfuse(INT inFlag, TIMEQ inTime) {
    if (!inFlag) {
        setConditionFlag(0, 8);
        m_tqConfuse = 0;
        m_uiPadFlag = (INT)m_uiPadFlag & ~4;
        fadeEffect(m_hEffBad, 0);
    } else if (m_tqConfuse <= 0) {
        m_tqConfuse = inTime ? inTime : 21000;
        m_uiPadFlag |= 4;
        setConditionFlag(8, 0);
        FVECTOR pos;
        pos.set(0.0f, 0.65f, 0.0f);
        m_hEffBad = attachEffect(PL::g_szDpxAbnormal, 2, &Crd(m_iBoneCenter), &pos, NULL, 0.0f);
        CEffect* effect = CEffectMan::getEffect(m_hEffBad);
        if (effect) effect->m_iFlag |= 4;
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setConfuse__7CPlayerii);
#endif
void CPlayer::exeConfuse() {
    if (m_uiConditionFlag & 8) {
        m_tqConfuse -= m_tqFrameRate;
        if (m_tqConfuse <= 0) setConfuse(0, 0);
    }
}
void CPlayer::initItem(INT inNo) {
    setPlayerState(PL::ITEM);
    clearSpeed();
    setBattleFlag(0, 8);
    setBattleFlag(0, 0x80);
    setBattleFlag(0, 0x80000);
    setTrgFlag(0, 8);
    setPlayerFlag(0, 0x02800000);
    m_iAttackNo = inNo;
    m_uiPreAttackKind = 0;
    m_bIsAttackHit = FALSE;
    m_bNextTrg = FALSE;
    m_iNextAttack = 0;
    m_bDmgMuteki = FALSE;
    m_bIsCanCombo = FALSE;
    m_fJumpGravity = *(FLOAT*)&PL::g_fGravity;
    m_JumpState = 3;
    m_iSubState = 1;
    m_fMoveSpdXZ = 0.0f;
    m_tqCardBreakTime = PL::g_CardBreakTime[(*(BC_PARAM**)&CardBattle::g_pBattleCardList)[m_uiCardID].BREAK];
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeItem__7CPlayer);
void CPlayer::exeStandUp_inFriend() {
    INT oldMode = m_WalkMode;
    INT mode = chkWalkMode();
    m_WalkMode = mode;
    if (!mode && oldMode && m_pSE) ((CSEMotion*)m_pSE)->playFootStopSE();
    exeWalk();
}
void CPlayer::initCharge() {
    setPlayerState(PL::CHARGE);
    clearSpeed();
    setTrgFlag(0, 0x10);
    setAnimForce(0x2E, -1, 0, 0, 0, 0);
    m_tqEffCharge = 1000;
    m_bDmgMuteki = FALSE;
    if (m_hEffCharge <= 0) {
        FVECTOR pos;
        pos._q = 0;
        pos.w = 1.0f;
        CCoord* crd = &Crd(0);
        m_hEffCharge = attachEffect(PL::g_szDpxReload, 0, crd, &pos, NULL, 0.0f);
        CEffect* effect = CEffectMan::getEffect(m_hEffCharge);
        if (effect) effect->m_iFlag |= 4;
    }
}
void CPlayer::exeCharge() {
    INT result;
    m_tqEffCharge = 1000;
    switch (m_iSubState) {
    case 0:
        if (isTrgFlag(0x10)) {
            if (!isActionFlag(0x2000000) && isAir()) {
                initFall();
                return;
            }
            result = m_pRevolver->chargeReloadCard(0);
            if (result == 1) {
                FVECTOR pos;
                pos._q = 0;
                pos.w = 1.0f;
                CCoord* crd = &Crd(0);
                CEffect* effect = CEffectMan::getEffect(attachEffect(PL::g_szDpxReload, 1, crd, &pos, NULL, 0.0f));
                if (effect) effect->m_iFlag |= 4;
                setAnimForce(0x2F, -1, 0, 0, 0, 0);
                m_iSubState = result;
                fadeEffect(m_hEffCharge, 1);
            } else if (result < 0) {
                initStandUp(0);
            }
        } else {
            initStandUp(0);
        }
        break;
    case 1:
        if (isMotionEnd()) initStandUp(0);
        break;
    }
}
/* Parked: only the block before chargeReloadCard is scheduled differently: the original zeroes pos with v1
   after the flag 'and' and loads m_pRevolver after the flag store; ours zeroes pos in a0 first and loads
   m_pRevolver before the store. tried: all orders of setPlayerFlag/pos._q/pos.w (the 5-line ones zero w and
   are wrong), pos.init()/set()/copy, direct and cast flag stores, cast and volatile-reference m_pRevolver
   loads, memory asm barriers (best 6); best clean draft 8 lines. tools/permuter.py can't run on C++ methods. */
#if 0
INT CPlayer::chkChargeMove(INT inFlag) {
    FVECTOR pos;
    CEffect* effect;
    if (CGameWork::getFlag(1, 8)) return TRUE;
    if (!inFlag && CGame::isSoraGame()) {
        if (!m_pCB) return FALSE;
        if (m_pCB->getEnchantKind() != 10) return FALSE;
    }
    setPlayerFlag(0, 0x10);
    pos._q = 0;
    pos.w = 1.0f;
    if (m_pRevolver->chargeReloadCard(0) != 1) {
        m_tqEffCharge = 1000;
        if (m_hEffCharge) return TRUE;
        m_hEffCharge = attachEffect(PL::g_szDpxReload, 0, &Crd(0), &pos, NULL, 0.0f);
        effect = CEffectMan::getEffect(m_hEffCharge);
        if (effect) effect->m_iFlag |= 4;
    } else {
        fadeEffect(m_hEffCharge, 1);
        effect = CEffectMan::getEffect(attachEffect(PL::g_szDpxReload, 1, &Crd(0), &pos, NULL, 0.0f));
        if (effect) effect->m_iFlag |= 4;
    }
    return TRUE;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkChargeMove__7CPlayeri);
#endif
void CPlayer::fadeEffCharge() {
    if (m_hEffCharge > 0) {
        m_tqEffCharge -= getFrameRate();
        if (m_tqEffCharge <= 0) {
            m_tqEffCharge = 0;
            CEffect* effect = CEffectMan::getEffect(m_hEffCharge);
            if (effect) effect->fadeout(8.0f);
            m_hEffCharge = 0;
        }
    }
}
void CPlayer::setCure(INT inID, INT inPow) {
    FVECTOR pos;
    INT no = inID - 14;
    if (inID == 0x33) no = 0;
    pos.set(0.0f, 0.85f, 0.0f);
    CEffect* effect = CEffectMan::getEffect(attachEffect(PL::g_EffTbl_Skill[inID][1].pEffectName, no, &Crd(0), &pos, NULL, 0.0f));
    if (effect) effect->m_iFlag |= 4;
    addHP(inPow, 0, 1);
}
void CPlayer::setCastEffect(const CHAR* inName, INT inNo, CCoord* inCrd, FVECTOR* inOfs) {
    FVECTOR pos;
    if (inOfs) {
        pos = *inOfs;
    } else {
        pos._q = 0;
        pos.w = 1.0f;
    }
    m_hEffCast = attachEffect(inName, inNo, inCrd, &pos, NULL, 0.0f);
    CEffect* effect = CEffectMan::getEffect(m_hEffCast);
    if (effect) effect->m_iFlag |= 4;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", calcAppearPos__7CPlayerffP7FVECTOR);
void CPlayer::calcAppearPosFS(FLOAT inDist, FLOAT inRadius) {
    FVECTOR pos;
    BOOL hit;
    FLOAT height;
    pos = getPos();
    FVECTOR zero = {{m_vStartPos.x, pos.y, m_vStartPos.z, 1.0f}};
    FLOAT dir0 = calcDirection(zero);
    FMATRIX m;
    _unitMatrix(m);
    _rotMatrixY(m, dir0);
    FVECTOR start = {{0.0f, 0.0f, inDist, 1.0f}};
    ApplyMatrixXYZ(start, m, start);
    AddVectorXYZ(pos, pos, start);
    FVECTOR vStart = {{pos.x, pos.y + 0.5f, pos.z, 1.0f}};
    FVECTOR vEnd = {{pos.x, -2.0f, pos.z, 1.0f}};
    if (m_pPLM->isJaferBattle()) vStart.y = 7.0f;
    else if (m_pPLM->isTricksterBattle()) vStart.y = 3.0f;
    m_pCollMap->setType(COLL_TYPE_SPHERE);
    m_pCollMap->enableSliding(0);
    m_pCollMap->setRadius(inRadius);
    m_pCollMap->setStartPos(vStart);
    m_pCollMap->setEndPos(vEnd);
    hit = FALSE;
    height = vEnd.y;
    if (m_pCollMap->collideRide(0)) {
        FLOAT detectH = m_pCollMap->getDetectPos()->y;
        hit = TRUE;
        if (height < detectH) height = detectH;
    }
    if (m_pCollMap->collideMap()) {
        FLOAT detectH = m_pCollMap->getDetectPos()->y;
        hit = TRUE;
        if (height < detectH) height = detectH;
    }
    if (!hit) height = 0.0f;
    m_vAppearPos.set(pos.x, height + 0.0035f, pos.z);
}
void CPlayer::calcAppearPosFS_toAttackID() {
    if (m_uiAttackID < 0x78) {
        if (m_uiAttackID >= 0x76) {
            calcAppearPosFS(0.0f, m_fWallRadius);
            return;
        }
    }
    calcAppearPosFS(2.0f, m_fWallRadius);
}
INT CPlayer::chkEscape() {
    BOOL bPlay;
    FVECTOR vNormal;
    if (m_PlayerState == PL::ESCAPE) return FALSE;
    bPlay = FALSE;
    if (m_pCB && (CardBattle::CCardBattle::isPlayingCard(1) || isBattleFlag(0x10200000))) bPlay = TRUE;
    if (m_fAnalogPower < 0.3f) bPlay = TRUE;
    if (bPlay || isBattleFlag(4) || m_pMove->getWallCollATTR() != 5 || m_PlayerState == 0x20) {
        if (isBattleFlag(2)) {
            m_tqEscape = 0;
            setBattleFlag(0, 2);
            if (m_pEscape) m_pEscape->destroy();
            setBGFade(1.0f, 0.0f, 0);
        }
        return FALSE;
    }
    if (!isBattleFlag(2)) {
        setBattleFlag(2, 0);
        m_tqEscape = 0;
        if (m_pEscape) m_pEscape->setCharge(0.0f);
        setBGFade(0.5f, 120.0f, 0);
    }
    vNormal = m_pMove->getEscapeNormal();
    if (_abs(vNormal.x) > _abs(vNormal.z)) vNormal.z = 0.0f;
    else vNormal.x = 0.0f;
    FLOAT d = atan2f(vNormal.x, vNormal.z) + 3.1415927f;
    FLOAT r;
    if (d < -3.1415927f) r = d + 6.2831855f;
    else {
        r = d;
        if (r > 3.1415927f) r -= 6.2831855f;
    }
    m_fWallDir = r;
    m_tqEscape += m_tqFrameRate;
    if (m_tqEscape < 6000) {
        FLOAT charge = m_tqEscape * 100.0f / 6000.0f;
        if (m_pEscape) m_pEscape->setCharge(charge);
        return FALSE;
    }
    m_tqEscape = 6000;
    if (m_pEscape) m_pEscape->setCharge(100.0f);
    if (isAir()) return FALSE;
    initEscape();
    return TRUE;
}
void CPlayer::initEscape() {
    FVECTOR rot;
    setPlayerState(PL::ESCAPE);
    setBattleFlag(0, 8);
    setBattleFlag(0, 0x00443F90);
    setBattleFlag(0, 0x10200000);
    setBattleFlag(0, 0x4060);
    setBattleFlag(0, 0x00130000);
    setPlayerFlag(0, 0x020C2000);
    setBattleFlag(0, 0x03000000);
    clearSpeed();
    m_collBody.setDamage(0);
    m_collBody.off();
    m_collBody.offBody();
    m_bDmgMuteki = TRUE;
    resetAction(7);
    m_fDir = m_fWallDir;
    rot = getRot();
    rot.y = m_fDir;
    setRot(rot);
    m_pMove->setFlag(1, TRUE); // partial CMovement: (flag, on)
    setPlayerFlag(0x400001, 0);
    setAnim(0x2C, -1, 0);
    m_WalkMode = 2;
    m_fMovePower = 1.0f;
    m_fWalkSpeed = m_pParam->MAX_RUN_SPEED / 60.0f;
    m_pMove->calcVelocityXZ(m_fWalkSpeed);
    m_tqEscape = 0;
    m_pEscape->destroy();
    CardBattle::CCardBattle::setBattleEnd();
    m_pRevolver = NULL;
    CGameWork::setInt(1, 0xE2, -1);
    setBGFade(0.0f, 60.0f, 1);
    m_pCam->setFlag(1, TRUE);      // partial CPlayerCamera: (flag, on)
    m_pCam->setFlag(0x1000, TRUE);
    if (getPos().z < 0.0f) m_vStartPos.z = 0.0f;
}
void CPlayer::exeEscape() {
    setPlayerFlag(0x400000, 0);
    m_pCam->setFlag(0x1000, TRUE); /* the partial CPlayerCamera keeps (flag, on/off): the original is setFlag(0x1000, 0) */
    if (!m_iSubState) {
        m_tqEscape += m_tqFrameRate;
        if (m_tqEscape >= 2501) {
            if (CSceneInfo::reqExitBattle()) m_iSubState = 1;
        }
    }
}
void CPlayer::setBGFade(FLOAT inRate, FLOAT inTime, INT inFlag) {
    FVECTOR v;
    v.set(inRate, inRate, inRate, 1.0f);
    setBGFade(v, inTime, inFlag);
}
void CPlayer::setBGFade(FVECTOR& inColor, FLOAT inFrame, INT inFlag) {
    INT iObjType = 0x1808;
    if (inFlag & 1) iObjType = 0x187C;
    if (inColor.x + inColor.y + inColor.z == 3.0f) setPlayerFlag(0, 0x80000000);
    else setPlayerFlag(0x80000000, 0);
    CSceneEnv::setColor((ObjType::eObjBit)iObjType, inFrame, inColor, NULL, 0);
    if (inFlag & 2) CSceneEnv::setStop((ObjType::eObjBit)iObjType);
}
void CPlayer::setObjStop(UINT inFlag) {
    if (!inFlag) CSceneEnv::setStop((ObjType::eObjBit)0);
    else CSceneEnv::setStop((ObjType::eObjBit)0x7C);
}
INT CPlayer::exeWarp(INT inSet) {
    if (inSet) {
        if (isDispOFF()) return TRUE;
        if (!(m_fScale > 0.0f)) return TRUE;
        m_fScale -= getFrameRateF() * 0.05f;
        if (m_fScale <= 0.0f) {
            setEntityFlag(1, 0);
            m_fScale = 1.0f;
            offWeapon();
        }
        FVECTOR vScale = getScale();
        vScale.x = m_fScale;
        vScale.y = 2.0f - m_fScale;
        vScale.z = m_fScale;
        setScale(vScale);
        setConditionFlag(0x1000000, 0);
        setPlayerFlag(1, 0);
        return FALSE;
    }
    if (isDispOFF()) {
        m_fScale = 0.0f;
        setEntityFlag(0, 1);
        onWeapon(0);
    }
    if (m_fScale < 1.0f) {
        m_fScale += getFrameRateF() * 0.05f;
        if (m_fScale > 1.0f) m_fScale = 1.0f;
        FVECTOR vScale = getScale();
        vScale.x = m_fScale;
        vScale.y = 2.0f - m_fScale;
        vScale.z = m_fScale;
        setScale(vScale);
        return FALSE;
    }
    setConditionFlag(0, 0x1000000);
    setPlayerFlag(0, 1);
    return TRUE;
}
void CPlayer::setBanish(INT inFlag) {
    if (!inFlag) {
        setConditionFlag(0, 0x40);
        FVECTOR vColor;
        vColor.set(1.0f, 1.0f, 1.0f);
        CSceneEnv::setColor((ObjType::eObjBit)2, 30.0f, vColor, NULL, 2);
    } else {
        setConditionFlag(0x40, 0);
        m_tqBanish = 0;
        PL::g_vBanishPos = getPos();
    }
}
void CPlayer::exeBanish() {
    if (isConditionFlag(0x40)) {
        FVECTOR vColor;
        vColor.set(1.0f, 1.0f, 1.0f, 0.5f);
        vColor.w = (cosf((FLOAT)m_tqBanish * 0.0006f) + 1.0f) * 0.5f * 0.85f + 0.1f;
        CSceneEnv::setColor((ObjType::eObjBit)2, 0.0f, vColor, NULL, 2);
        m_tqBanish += m_tqFrameRate;
        if (vColor.w > 0.8f) {
            m_tqBanish += m_tqFrameRate;
            if (vColor.w > 0.9f) PL::g_vBanishPos = getPos();
        }
    }
}
void CPlayer::setReflectGuard(INT inFlag) {
    if (!inFlag) {
        setConditionFlag(0, 0x200);
        m_collBody.m_GuardAttrFlag = 0;
        m_collBody.m_uiFlag = (INT)m_collBody.m_uiFlag & ~2;
    } else {
        setConditionFlag(0x200, 0);
        m_collBody.m_uiFlag |= 2;
        CCollBody& body = m_collBody;
        body.setGuardDir(1.5707964f);
        body.m_GuardAttrFlag |= 1;
    }
}
void CPlayer::setQuickBarrier(INT inFlag) {
    if (m_tqBarrier > 0) {
        m_collBody.m_uiFlag = (INT)m_collBody.m_uiFlag & ~2;
        m_tqBarrier = 0;
        m_collBody.m_GuardAttrFlag = 0;
    } else {
        m_tqBarrier = 6000;
        CCollBody& body = m_collBody;
        m_collBody.m_uiFlag |= 2;
        body.m_GuardAttrFlag |= 1;
        m_collBody.m_uiFlag |= 2;
        body.m_GuardAttrFlag |= 2;
        m_collBody.m_uiFlag |= 2;
        body.m_GuardAttrFlag |= 4;
        m_collBody.m_uiFlag |= 2;
        body.m_GuardAttrFlag |= 8;
        m_collBody.m_uiFlag |= 2;
        body.m_GuardAttrFlag |= 0x10;
        m_collBody.m_uiFlag |= 2;
        body.setGuardDir(3.1415927f);
        body.m_GuardAttrFlag |= 0x20;
    }
}
void CPlayer::exeQuickBarrier() {
    if (m_tqBarrier) {
        m_tqBarrier -= m_tqFrameRate;
        if (m_tqBarrier <= 0) {
            m_collBody.m_uiFlag = (INT)m_collBody.m_uiFlag & ~2;
            m_tqBarrier = 0;
            m_collBody.m_GuardAttrFlag = 0;
        }
    }
}
void CPlayer::setAeroGuard(INT inFlag) {
    CBullet* pBullet;
    BULLET_PARAM param;
    if (!inFlag) {
        setConditionFlag(0, 0x80000000);
        m_tqGuard = 0;
        pBullet = (CBullet*)CTaskMan::getTask(m_hEffGuard);
        if (pBullet) pBullet->out();
    } else {
        setConditionFlag(0x80000000, 0);
        m_tqGuard = 6000;
        param.init(1, 3);
        param.fSpeed = 0.0f;
        param.CollKind = 0x80000000;
        param.uiFlag = 3;
        param.AtkKind = 1;
        param.AtkAttr = 1;
        param.iAttackPoint = m_iAP;
        param.fMaxDist = 50.0f;
        param.vPos = m_vCenterPos;
        param.fCollRadius = 0.8f;
        param.fShiftY = 0.0f;
        param.pEffectTbl = PL::g_EffTbl_Skill[66];
        pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        m_hEffGuard = pBullet->getHandle();
        pBullet->setBodyRadius(0.0f, 0.0f, 0, (COLL_KIND)0);
        pBullet->userBodyCollOffBody(-1);
        pBullet->userBodyCollOff(-1);
    }
}
void CPlayer::exeAeroGuard() {
    if (isConditionFlag(0x80000000)) {
        CBullet* pBullet = (CBullet*)CTaskMan::getTask(m_hEffGuard);
        if (isConditionFlag(0x1000000)) {
            if (pBullet) {
                pBullet->out();
                m_hEffGuard = 0;
            }
        } else if (!pBullet) {
            setAeroGuard(1);
        } else {
            m_tqBarrier -= m_tqFrameRate;
            if (m_tqBarrier <= 0) {
                m_tqBarrier = 6000;
                pBullet->updateAttackID();
            }
            pBullet->warp(m_vCenterPos);
        }
    }
}
void CPlayer::setPrizeHealing(TIMEQ inTime) {
    if (!m_pInstance) return;
    if (!inTime) {
        if (m_hEffPrize > 0) {
            CEffect* effect = CEffectMan::getEffect(m_hEffPrize);
            if (effect) effect->loopEnd();
            m_hEffPrize = 0;
        }
        m_tqPrize = 0;
    } else {
        if (!m_tqPrize) {
            m_hEffPrize = attachEffect(PL::g_szDpxAlways, 0x16, &Crd(m_iBoneCenter), NULL, NULL, 0.0f);
            CEffect* effect = CEffectMan::getEffect(m_hEffPrize);
            if (effect) effect->m_iFlag |= 4;
        }
        m_tqPrize = inTime;
    }
}
void CPlayer::exePrizeHealing() {
    if (m_hEffPrize) {
        m_tqPrize -= m_tqFrameRate;
        if (m_tqPrize <= 0) {
            if (m_hEffPrize > 0) {
                CEffect* effect = CEffectMan::getEffect(m_hEffPrize);
                if (effect) effect->loopEnd();
                m_hEffPrize = 0;
            }
            m_tqPrize = 0;
        }
    }
}
void CPlayer::initGimmickCard(INT inNo) {
    TRACE(D_0049AEC0);
    setPlayerState(PL::GIMMICKCARD);
    m_iSubState = 1;
    clearSpeed();
    setPlayerFlag(1, 0);
    offAttack();
    setBattleFlag(0, 8);
    setBattleFlag(0, 0x200);
    setBattleFlag(0, 0x80000);
    setTrgFlag(0, 0x40);
    m_iAttackNo = inNo;
    m_uiPreAttackKind = 0;
    m_uiAttackKind = 0;
    m_bIsAttackHit = FALSE;
    m_bNextTrg = FALSE;
    m_iNextAttack = 0;
    m_bDmgMuteki = FALSE;
    m_bIsCanCombo = FALSE;
    m_fJumpGravity = *(FLOAT*)&PL::g_fGravity;
    m_JumpState = 3;
    m_fMoveSpdXZ = 0.0f;
    m_tqCardBreakTime = 0;
    m_fWalkSpeed = 0.0f;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeGimmickCard__7CPlayer);
void CPlayer::setTrapDamage(INT inPower, const CHAR* lpszInName, UINT nNo) {
    INT face;
    INT hp0;
    TRACE(D_0049AFA8);
    if (DebugWork::g_dw.player.iMuteki) return;
    if (lpszInName) setEffect(lpszInName, nNo, &m_vCenterPos, NULL, 0.0f);
    face = 1;
    hp0 = m_iHP;
    hp0 -= inPower;
    if (m_pCB && m_pCB->getEnchantKind() == 0x3B) {
        hp0 += inPower;
    } else {
        hp0 += (DebugWork::g_dw.player.iDamage0 > 0) * inPower;
        setDamageNumber(m_vHeadPos, inPower);
    }
    if (m_pCB) {
        if (hp0 <= 0 && m_iHP >= 2 && m_pCB->getEnchantKind() == 0x29) {
            hp0 = 1;
            CardBattle::CCardBattle::decEnchantTime(1, 0x28, 0x29);
        }
        CardBattle::CCardBattle::decEnchantTime(1, 0x27, 0);
    }
    if (hp0 < 0) hp0 = 0;
    m_iHP = hp0;
    TRACE(D_0049A920);
    TRACE(D_0049A988, m_iHP, m_iMaxHP);
    TRACE(D_0049AA00);
    if (hp0 <= 0) {
        m_bIsDead = TRUE;
        face = 2;
        m_pPLM->notifyDeadSE(CSound::playSE(0x4C, -1, 0x40));
        CGameWork::setFlag(1, 6, TRUE);
    }
    if (m_pGauge) m_pGauge->setDamage(m_iHP, 0);
    setFaceAnim(face, 1);
}
void CPlayer::fadeEffect(INT& ioHandle, INT inLoop) {
    if (ioHandle > 0) {
        CEffect* effect = CEffectMan::getEffect(ioHandle);
        if (effect) {
            if (inLoop && effect->isLoop()) effect->loopEnd();
            else effect->fadeout(8.0f);
        }
        ioHandle = 0;
    }
}
void CPlayer::fadeEffAttack(INT inLoop) {
    INT i;
    for (i = 0; i < 4; i++) {
        if (m_hEffAttack[i] > 0) {
            CEffect* effect = CEffectMan::getEffect(m_hEffAttack[i]);
            if (effect) {
                if (inLoop && effect->isLoop()) effect->loopEnd();
                else effect->fadeout(8.0f);
            }
            m_hEffAttack[i] = 0;
        }
    }
}
void CPlayer::setEffAttack(INT inNum, const CHAR* inName, INT inNo, INT inBone, FVECTOR* inOfs) {
    FVECTOR pos;
    if (inOfs) {
        pos = *inOfs;
    } else {
        pos._q = 0;
        pos.w = 1.0f;
    }
    m_hEffAttack[inNum] = attachEffect(inName, inNo, &Crd(inBone), &pos, NULL, 0.0f);
    CEffect* effect = CEffectMan::getEffect(m_hEffAttack[inNum]);
    if (effect) effect->m_iFlag |= 4;
}
void CPlayer::updateSpotLight(HANDLE inHandle) {
    CEffect* effect = CEffectMan::getEffect(inHandle);
    if (effect) {
        FVECTOR n = m_pMove->getDetectNormal();
        FMATRIX m;
        _unitMatrixVu0(m);
        sceVu0RotMatrixX(m, m, atan2f(_sqrtf(n.z * n.z + n.x * n.x), n.y));
        sceVu0RotMatrixY(m, m, atan2f(n.x, n.z));
        FVECTOR pos = getPos();
        pos.w = 1.0f;
        m.v[3] = pos;
        effect->getCoords()->getLocalMatrix(0) = m;
        effect->getCoords()->Crd(0).m_Flag = 1;
    }
}
void CPlayer::setQuake(FLOAT inCycle, FLOAT inPower, UINT inTime) {
    CCamera* cam = CWorldEnv::getActiveCamera();
    CameraQuakeParam cqp;
    cqp._fCycle = inCycle;
    cqp._nTime = inTime;
    cqp._fPower = inPower;
    new (com_malloc_d(sizeof(CCameraQuakeV), D_0049AFC0, RET_ADDR())) CCameraQuakeV(*cam, &cqp, inPower);
}
void CPlayer::notifyReturnCard(const CARD* inCard) {
    if (!m_pInstance) return;
    if (m_pRevolver) {
        if (!inCard) {
            m_pRevolver->resetCardOwner(2, 0);
            CBattleEnemyManager* pBEM = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
            if (pBEM) pBEM->allGetPrize(2);
        } else if (inCard->m_nKind < 0xCB) {
            INT cat = inCard->m_nCategory0;
            if (cat && (cat < 7 || cat == 8)) m_pRevolver->addCard((CARD*)inCard, 1);
        }
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", notifyGetPrize__7CPlayeriii);
static inline BOOL _isLevelMax() {
    BOOL ret = FALSE;
    if (CPlayer::_instance()) {
        if (CPlayer::_instance()->m_iLV >= 99) {
            CPlayer::_instance()->m_iLV = 99;
            ret = TRUE;
        }
    } else {
        ret = CStatus::getLV() >= 99;
    }
    return ret;
}
static inline void _addEXP(INT inEXP) {
    if (CPlayer::_instance()) CPlayer::_instance()->m_iEXP += inEXP;
    else CStatus::getStaus().EXP += inEXP;
}
static inline void _addLV(INT inLV) {
    if (CPlayer::_instance()) CPlayer::_instance()->m_iLV += inLV;
    else CStatus::getStaus().LV += inLV;
}
void CPlayer::addEXP(INT inEXP, INT inMode) {
    BOOL bLevelMax = _isLevelMax();
    BOOL levelup = FALSE;
    if (!bLevelMax) {
        _addEXP(inEXP);
        while (getNowNextEXP() <= 0) {
            _addLV(1);
            levelup = TRUE;
        }
    }
    if (levelup) {
        CGameWork::setFlag(1, 0x2D, TRUE);
        if (inMode && _instance()) {
            Gauge::CCommonGauge::setDispSeq(_instance()->getHandle(), 4);
            CSound::playSE(0x36, -1, 0x40);
            FVECTOR pos = {{0.0f, 0.85f, 0.0f, 1.0f}};
            _instance()->m_hEffLevelUp = m_pInstance->attachEffect(PL::g_szDpxAlways, 0x15, &_instance()->Crd(0), &pos, NULL, 0.0f);
            CEffect* effect = CEffectMan::getEffect(_instance()->m_hEffLevelUp);
            if (effect) effect->m_iFlag |= 4;
        }
    }
    Gauge::CPrizeGauge::setPrizeExp(inEXP);
}
void CPlayer::addHP(INT inHP, TIMEQ inDispEFF, INT inDispREC) {
    if (_instance()) {
        if (inDispEFF > 0) _instance()->setPrizeHealing(inDispEFF);
        if (inDispREC > 0) Gauge::CCommonGauge::setDispSeq(_instance()->getHandle(), 0);
        _instance()->m_iHP += inHP;
        CPlayer* p = _instance();
        if (p->m_iHP > p->m_iMaxHP) p->m_iHP = p->m_iMaxHP;
        if (_instance()->m_pGauge) _instance()->m_pGauge->setHP(m_iHP, 0);
    } else {
        STATUS_INFO& st = CStatus::getStaus();
        INT hp = st.HP + inHP;
        if (hp > st.HPMax) hp = st.HPMax;
        st.HP = hp;
    }
}
void CPlayer::addMP(INT inMP) {
    if (_instance()) {
        _instance()->m_iMP += inMP;
        if (_instance()->m_iMP > 99999) _instance()->m_iMP = 99999;
    } else {
        INT mp = CStatus::getMP() + inMP;
        if (mp > 99999) mp = 99999;
        CStatus::setHP(mp);
    }
    Gauge::CPrizeGauge::setPrizeMp(inMP);
}
void CPlayer::procDropMP() {
    INT dropMP;
    PRIZE_PARAM param;
    INT mp;
    dropMP = m_iMP / 10;
    if (dropMP > 500) dropMP = 500;
    else if (dropMP < 4 && m_iMP >= 4) dropMP = 4;
    param.vPos = m_vCenterPos;
    param.fSpeed = 0.2f;
    param.uiFlag = 0;
    param.uiPrizeFlag = 0;
    for (mp = dropMP; mp >= 4;) {
        if (mp >= 10) {
            param.PrizeKind = 0xD5;
            mp -= 10;
        } else if (mp >= 4) {
            param.PrizeKind = 0xD4;
            mp -= 4;
        } else {
            param.PrizeKind = 0;
        }
        if (param.PrizeKind) {
            param.vRot.set((-50.0f - RND::genrand_real1() * 10.0f) * 0.017453292f, RND::genrand_real1() * 360.0f * 0.017453292f, 0.0f);
            Math3D::LimitPIVector(param.vRot);
            CEntityFactory::getInstance()->createPrize(param, NULL);
        }
    }
    m_iMP -= dropMP - mp;
    Gauge::CPrizeGauge::setDropMp(dropMP - mp);
}
void CPlayer::notifyExterminate() {
    if (!_instance()) return;
    if (_instance()->m_bIsDead) {
        TRACE(D_0049B238);
        return;
    }
    if (_instance()->m_PlayerState == PL::ESCAPE) return;
    PLM::calcFavoriteStockCount();
    _instance()->setBattleFlag(4, 0);
    _instance()->setConditionFlag(0x2000, 0);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", notifyBossDead__7CPlayeri);
HANDLE CPlayer::getLevelUpEffect() {
    if (!m_pInstance) return 0;
    return m_pInstance->m_hEffLevelUp;
}
void CPlayer::initEnd() {
    setPlayerState(PL::END_BTL);
    if (isBattleFlag(0x4000000)) {
        resetAction(9);
        m_iSubState = 1;
    } else {
        resetAction(8);
        m_iSubState = 2;
        m_pAnim->setRootMove(FALSE, FALSE);
        setConditionFlag(0x2000, 0);
        CardBattle::CCardBattle::setBattleEnd();
        if (m_pRevolver) {
            m_pRevolver->destroy();
            m_pRevolver = NULL;
        }
    }
    setPlayerFlag(0, 0x02800000);
    clearSpeed();
    m_bDmgMuteki = TRUE;
    m_fJumpGravity = *(FLOAT*)&PL::g_fGravity;
    m_fMoveSpdXZ = 0.0f;
    m_JumpState = 3;
    if (m_fJumpSpdY > m_fMoveSpdXZ) m_fJumpSpdY = m_fMoveSpdXZ;
    if (isBattleFlag(2)) {
        setBattleFlag(0, 2);
        m_tqEscape = 0;
        if (m_pEscape) m_pEscape->destroy();
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeEnd__7CPlayer);
BOOL CPlayer::isEnableBattleEnd() {
    if (!m_pInstance) return TRUE;
    if (m_pInstance->m_bIsDead) return FALSE;
    if (m_pInstance->m_PlayerState != 0x1C) return FALSE;
    return m_pInstance->m_iSubState >= 3;
}
BOOL CPlayer::isNowExterminate() {
    if (!m_pInstance) return TRUE;
    if (!m_pInstance->m_bIsDead) return FALSE;
    UINT m = 4;
    return (m_pInstance->m_uiBattleFlag & m) != 0;
}
void CPlayer::killBullet() {
    CBulletManager* p = (CBulletManager*)CTaskMan::getSysTask(0x1B);
    if (p) p->kill(getHandle());
}
void CPlayer::setEnemyStop(HANDLE inEnemy, UINT inSet) {
    SLContainerT<CTask>* container;
    CTask* task;
    CEntity* pEntity;
    CBattleEnemyManager* pBEM = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
    if (!pBEM) return;
    container = pBEM->getFirstBattleEnemy();
    if (inSet) {
        for (; container; container = container->getNext()) {
            task = container->getValue();
            if (task && task->getTypeID() == 0x1004) {
                if (task->getHandle() != inEnemy) {
                    pEntity = (CEntity*)task;
                    pEntity->setEntityFlag(0x80000000, 0);
                }
            }
        }
        CSceneEnv::setStop((ObjType::eObjBit)8);
    } else {
        for (; container; container = container->getNext()) {
            task = container->getValue();
            if (task && task->getTypeID() == 0x1004) {
                pEntity = (CEntity*)task;
                pEntity->setEntityFlag(0, 0x80000000);
            }
        }
        CSceneEnv::setStop((ObjType::eObjBit)0);
    }
}
INT CPlayer::setMDFooting(HANDLE inFooting) {
    if (!inFooting) {
        setConditionFlag(0, 0x10000000);
        setTrgFlag(0, 0x400);
        m_fJumpLandLimit = -100.0f;
        m_hFooting = 0;
        m_pMove->setFlag(0x200, FALSE); // partial CMovement: (flag, on)
        return 0;
    }
    switch (m_PlayerState) {
    case PL::STAND:
    case PL::JUMP:
    case PL::FOOTING:
        break;
    default:
        return 0;
    }
    if (!isConditionFlag(0x10000000)) {
        setTrgFlag(0x400, 0);
        m_hFooting = inFooting;
        m_fJumpLandLimit = 2.0f;
        return 0;
    }
    m_pMove->setFlag(0x200, TRUE);
    return 1;
}
void CPlayer::initFooting(INT inFlag) {
    setPlayerState(PL::FOOTING);
    setTrgFlag(0, 0x400);
    if (inFlag == 1) {
        clearSpeed();
        setBattleFlag(0, 8);
        setBattleFlag(0, 0x80000);
        setPlayerFlag(0, 0x02800000);
        m_iAttackNo = 0;
        m_uiPreAttackKind = 0;
        m_bIsAttackHit = FALSE;
        m_bNextTrg = FALSE;
        m_iNextAttack = 0;
        m_bDmgMuteki = FALSE;
        m_bIsCanCombo = FALSE;
        m_fJumpGravity = *(FLOAT*)&PL::g_fGravity;
        m_JumpState = 3;
        m_fMoveSpdXZ = 0.0f;
    } else if (inFlag == 2) {
        setBattleFlag(0, 8);
        m_iSubState = inFlag;
        setPlayerFlag(1, 0);
        clearSpeed();
        if (m_fAnalogPower >= 0.3f) {
            m_fDir = m_fAnalogViewDir;
            FVECTOR rot = getRot();
            rot.y = m_fDir;
            setRot(rot);
        }
        updateCoord();
        if (CGame::isSoraGame()) setAnimRsrcEx(0xFAA, 0, -1, 0, 0, 0, 1);
        else if (!isConditionFlag(0x400000)) setAnimRsrcEx(0xFAC, 0, -1, 0, 0, 0, 1);
        else setAnimRsrcEx(0xFAD, 0, -1, 0, 0, 0, 1);
        m_fJumpSpdY = 0.0f;
        m_JumpState = 3;
        m_fJumpGravity = *(FLOAT*)&PL::g_fGravity;
    } else {
        m_iSubState = 1;
        m_WalkMode = chkWalkMode();
        setAnim(0x2A, -1, 0);
        m_fMovePower = 0.5f;
        m_fWalkSpeed = m_fWalkSpeed / 60.0f * m_fMovePower;
        offAttack();
        setPlayerFlag(0, 0x024C2000);
        m_JumpState = 0;
        m_fJumpSpdY = 0.0f;
        setConditionFlag(0, 0x08810023);
        setBattleFlag(0, 0x00443F90);
        setFaceAnim(0, 0);
        m_pMove->setFlag(0x40, TRUE);
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeFooting__7CPlayer);
void CPlayer::updateFooting() {
    if (isConditionFlag(0x10000000)) {
        CEntity* pEntity = m_hFooting > 0 ? (CEntity*)CTaskMan::getTask(m_hFooting) : NULL;
        if (pEntity) {
            FVECTOR pos = Crd(0).getWorldMatrix().v[3];
            pos.y = 2.0f;
            pEntity->warp(pos);
            FVECTOR rot = getRot();
            pEntity->setRot(rot);
            pEntity->updateCoord();
            pEntity->coordFix();
        }
    }
}
void CPlayer::setCardSnatch(INT inFlag) {
    if (m_pRevolver) {
        m_hSnatch = inFlag;
        if (inFlag > 0) {
            m_uiBattleFlag = (INT)m_uiBattleFlag & ~8;
            m_pRevolver->setSnatch(inFlag);
        }
    }
}
void CPlayer::addWind(FVECTOR* inVel) {
    if (m_pPLM) m_pPLM->addWindXYZ(inVel);
}
void CPlayer::exeWind() {
    if (dbgCheckUser(KKIKUCHI) && DebugWork::g_dw.player.iTemp == 5) {
        if (!m_pPLM) return;
        m_pPLM->addWindXYZ(&DebugWork::g_dw.player.vTemp);
    }
    if (m_pPLM && m_pPLM->isEnableWind() && m_PlayerState != PL::GLIDE) {
        FVECTOR vel = getVel();
        FVECTOR wind = m_pPLM->getWind();
        AddVectorXYZ(vel, vel, wind);
        setVel(vel);
    }
}
BOOL CPlayer::cmdRevolver_rollDeck(INT inNum) {
    if (!m_pRevolver) return FALSE;
    m_pRevolver->rollDeck(inNum, 0, 300);
    return TRUE;
}
BOOL CPlayer::cmdRevolver_isEmpty() {
    if (!m_pRevolver) return TRUE;
    return m_pRevolver->getRemain(0, 0, 0) == 0;
}
INT CPlayer::cmdRevolver_isActive() {
    if (!m_pRevolver) return 0;
    return m_pRevolver->isActive();
}
void CPlayer::cmdMove_WaterSurfaceCheck(INT inFlag) {
    if (inFlag) m_pMove->setFlag(0x80, TRUE);
    else m_pMove->setFlag(0x80, FALSE);
}
void CPlayer::cmdState_initWorldSelect(HANDLE inDoor) {
    m_hGimmick = inDoor;
    m_pMarker->holdTarget(8);
    initWorldSelect();
    new (com_malloc_d(sizeof(CWorldSelect), D_0049B308, RET_ADDR())) CWorldSelect(CTaskMan::getSysTask(1), m_hGimmick);
}
void CPlayer::cmdState_setEncountWait() {
    setPlayerFlag(0x100, 0);
    changeState(2);
    setPlayerFlag(0x1001, 0);
    m_collBody.off();
    m_collBody.offBody();
}
void CPlayer::beginStateField_() {
    beginStateField();
}
void CPlayer::stateField_() {
    stateField();
}
void CPlayer::endStateField_() {
    endStateField();
}
void CPlayer::beginStateBattle_() {
    beginStateBattle();
}
void CPlayer::stateBattle_() {
    stateBattle();
}
void CPlayer::endStateBattle_() {
    endStateBattle();
}
void CPlayer::beginStateWait_() {
    beginStateWait();
}
void CPlayer::stateWait_() {
    stateWait();
}
void CPlayer::endStateWait_() {
    endStateWait();
}
void CPlayer::beginStateHide_() {
    beginStateHide();
}
void CPlayer::stateHide_() {
    stateHide();
}
void CPlayer::endStateHide_() {
    endStateHide();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", __12CPlayerDebugP5CTask);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", _$_12CPlayerDebug);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", main__12CPlayerDebug);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", __static_initialization_and_destruction_0_00276330);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", _$_t14CStateTemplate1Z7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getCurrentState__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", stateWait__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", endStateWait__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getCharaID__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getParam__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getPlayerCamera__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getLockonMarker__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getWeapon__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getRevolver__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getGauge__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getPlayerState__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getStateTQ__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", clearStateTQ__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setSubState__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getSubState__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setPlayerFlag__7CPlayerUiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setActionFlag__7CPlayerUiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setConditionFlag__7CPlayerUiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setTrgFlag__7CPlayerUiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setBattleFlag__7CPlayerUiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isPlayerFlag__C7CPlayerUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isActionFlag__C7CPlayerUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isConditionFlag__C7CPlayerUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isTrgFlag__C7CPlayerUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isBattleFlag__C7CPlayerUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setHP__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setMaxHP__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setCP__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setMaxCP__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getHP__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getMaxHP__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getCP__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getMaxCP__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setAP__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getAP__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setLV__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getLV__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setEXP__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getEXP__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setMP__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getMP__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setDP__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setMaxDP__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getDP__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getMaxDP__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getCount__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getNowAnimID__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getAnimID__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setStartPos__7CPlayerfff);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setStartDir__7CPlayerRCf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setDir__7CPlayerRCf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getStartPos__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getStartDir__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getDir__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getCenterPos__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getMovePower__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getWalkSpeed__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getLockOnPos__7CPlayerR7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getHeadPos__C7CPlayerR7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getUseCardPos__7CPlayerR7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getAppearPos__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setDrawRadius__7CPlayerff);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getPrizeDrawRadius__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getCardDrawRadius__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getLadderPos__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getLadderStep__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getLadderStepMax__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getJumpSpdY__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getJumpTop__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getJumpStartPos__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getJumpLandLimit__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getHangHandPos__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setAttackTimer__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setMaxCombo__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getAttackID__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setMutekiTimer__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isDead__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setBodyRviseCoefficient__7CPlayerf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getConfuse__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setMarkPos__7CPlayerfff);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getMarkPos__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isNowLockon__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getLockonPos__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isNowMark__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isNowTarget__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getOldLockon__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getOldCamMode__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setStateSE__7CPlayerii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getStateSESeq__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setAttack_inFriend__7CPlayeriii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", startStockSkillAction__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initStockSkill__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeStockSkill__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initMagic__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeMagic__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setMagic__7CPlayeriii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initFriend__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeFriend__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initSummon__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeSummon__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeAttack_inFriend__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", endAttack_inFriend__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initTransform__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeTransform__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initGlide__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeGlide__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initJump2nd__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeJump2nd__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeAvoid__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initPronounce__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exePronounce__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", executeBeginState__t14CStateTemplate1Z7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", executeState__t14CStateTemplate1Z7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", executeEndState__t14CStateTemplate1Z7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", func_00277080);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", _GLOBAL_$I$c__KHCOM_MainApp_Game_Player_Player.cppopbaaa);
