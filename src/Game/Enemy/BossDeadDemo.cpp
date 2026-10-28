#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/BattleEnemy.h"

/* Placeholders for an unknown header's inline functions (prize messages) that come next in .rodata. */
inline void _premiumPrizeMessage() {
    TRACE("\x83v\x83\x8c\x83~\x83" "A\x83\x80\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}
inline void _roulettePrizeMessage() {
    TRACE("\x83\x8b\x81[\x83\x8c\x83" "b\x83g\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}

#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _bossDeadDemoEmptyString() {
    TRACE("");
}

#include "Game/Card.h"
#include "System/Graphic/Effect/effect.h"
#include "Game/Enemy/BossDeadDemo.h"
#include "Game/Player/Player.h"
#include "Game/Game.h"
#include "Game/GameWork.h"
#include "System/Graphic/PathCamera.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "System/Sound/SoundMan.h"
#include "System/PlaySpeedCtrl.h"
#include "System/TaskMan.h"
#include "System/HeapMan.h"
#include "System/Math3D.h"
#include "System/Debug.h"

inline void* operator new(size_t, void* p) throw() { return p; }

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)
template <class T> static inline T& _ref(T& v) { return v; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

class CPackMan { /* System/Graphic/PackMan.h */
public:
    static void setFrameEffect(INT inMode, INT inParam0, INT inParam1, INT inParam2);
};

class CBattleEnemyManager { /* a CTask (Game/Enemy/BattleEnemyManager.h) */
public:
    INT getPrizeCount() const;
    void warpPrize(const FVECTOR& inPos);
};

#include "Game/Player/PlayerGauge.h"

struct BOSS_DEAD_DEMO_DATA {
    INT iEnemyID;
    INT iCardID;
    INT CardKind; /* CARD_KIND */
    INT iCardNumber;
    INT iAppearNo;
    BOOL bRiku;
};

namespace BossDeadDemo {
namespace Sora {
static CHAR* TexAnmEyeDefault = "EYE_DEFAULT";
static CHAR* TexAnmLipSmile = "LIP_SMILE";
}
namespace Riku {
static CHAR* TexAnmEyeDefault = "EYE_DEFAULT";
static CHAR* TexAnmLipSmile = "LIP_SMILE";
}
}

BOOL CBossDeadDemo::m_bPermissionWhiteIn = TRUE;
BOOL CBossDeadDemo::m_bStart = FALSE;
BOOL CBossDeadDemo::m_bFade = FALSE;

namespace BossDeadDemo {
static const BOSS_DEAD_DEMO_DATA g_BossDeadDemoDataTbl[25] = {
    {102, 111, 99, 0, -1, 1},
    {103, 415, 103, 0, -1, 1},
    {104, 415, 103, 0, -1, 1},
    {105, 416, 101, 0, -1, 1},
    {106, 310, 43, 3, -1, 1},
    {107, 311, 104, 0, -1, 1},
    {108, 232, 105, 0, -1, 1},
    {109, 610, 106, 0, -1, 1},
    {110, 508, 100, 0, -1, 1},
    {111, 709, 107, 0, -1, 1},
    {112, 832, 108, 0, -1, 1},
    {113, 908, 109, 0, -1, 1},
    {114, 1106, 113, 0, 1, 0},
    {114, 1106, 113, 0, 2, 0},
    {115, 1209, 102, 0, -1, 1},
    {116, 1327, 32, 6, 0, 1},
    {116, 1326, 44, 2, 2, 1},
    {116, 1328, 110, 0, 3, 0},
    {117, 1322, 28, 7, 0, 0},
    {117, 1329, 112, 0, 1, 0},
    {118, 1321, 26, 5, 0, 0},
    {118, 1323, 111, 0, 1, 0},
    {119, 1324, 114, 0, -1, 0},
    {121, 1330, 115, 0, -1, 1},
    {124, 1213, 117, 0, -1, 1},
};

INT GetCardID(INT inEnemyID, INT inAppearNo) {
    const BOSS_DEAD_DEMO_DATA* data = g_BossDeadDemoDataTbl;
    for (INT i = 0; i < sizeof(g_BossDeadDemoDataTbl) / sizeof(g_BossDeadDemoDataTbl[0]); i++, data++) {
        if (data->iEnemyID == inEnemyID) {
            if (data->iAppearNo < 0) return data->iCardID;
            if (data->iAppearNo == inAppearNo) return data->iCardID;
        }
    }
    return -1;
}

INT GetCardKind(INT inEnemyID, INT inAppearNo) {
    const BOSS_DEAD_DEMO_DATA* data = g_BossDeadDemoDataTbl;
    for (INT i = 0; i < sizeof(g_BossDeadDemoDataTbl) / sizeof(g_BossDeadDemoDataTbl[0]); i++, data++) {
        if (data->iEnemyID == inEnemyID) {
            if (data->iAppearNo < 0) return data->CardKind;
            if (data->iAppearNo == inAppearNo) return data->CardKind;
        }
    }
    return 0;
}

INT GetCardNumber(INT inEnemyID, INT inAppearNo) {
    const BOSS_DEAD_DEMO_DATA* data = g_BossDeadDemoDataTbl;
    for (INT i = 0; i < sizeof(g_BossDeadDemoDataTbl) / sizeof(g_BossDeadDemoDataTbl[0]); i++, data++) {
        if (data->iEnemyID == inEnemyID) {
            if (data->iAppearNo < 0) return data->iCardNumber;
            if (data->iAppearNo == inAppearNo) return data->iCardNumber;
        }
    }
    return 0;
}

BOOL IsRikuGet(INT inEnemyID, INT inAppearNo) {
    const BOSS_DEAD_DEMO_DATA* data = g_BossDeadDemoDataTbl;
    for (INT i = 0; i < sizeof(g_BossDeadDemoDataTbl) / sizeof(g_BossDeadDemoDataTbl[0]); i++, data++) {
        if (data->iEnemyID == inEnemyID) {
            if (data->iAppearNo < 0) return data->bRiku;
            if (data->iAppearNo == inAppearNo) return data->bRiku;
        }
    }
    return FALSE;
}

#line 189
void NotifyGetCard(const CBattleEnemy* inEnemy) {
    ASSERT(inEnemy);
    INT nEnemyID = (UINT)((CBattleEnemy*)inEnemy)->getID() % 10000;
    CPlayer::notifyGetPrize(GetCardKind(nEnemyID, ((CBattleEnemy*)inEnemy)->getParam()),
                            GetCardNumber(nEnemyID, ((CBattleEnemy*)inEnemy)->getParam()), 1);
}
}

CBossDeadDemo::CBossDeadDemo(CTask* inParent, BOOL inMonstroEventBattle) : CTask(inParent, 0x4000) {
    m_iCardID = -1;
    m_bMonstroEventBattle = inMonstroEventBattle;
    m_bPlayerPos = FALSE;
    m_iStep = 0;
    m_tqTimer = 0;
    m_tqDeadDemoSETimer = 0;
    m_hBossHandle = 0;
    m_hCard = 0;
    m_pPathCamera = NULL;
}

CBossDeadDemo::~CBossDeadDemo() {
    m_bPermissionWhiteIn = TRUE;
    m_bStart = FALSE;
    m_bFade = FALSE;
    CGameWork::setFlag(1, 6, FALSE);
}

void CBossDeadDemo::setup(HANDLE inBossHandle, const FVECTOR* inPlayerPos, INT inCardID) {
    m_hBossHandle = inBossHandle;
    m_iCardID = inCardID;
    if (inCardID == -1) {
        CBattleEnemy* enemy = (CBattleEnemy*)CTaskMan::getTask(inBossHandle);
        if (enemy) {
            if (CGame::isRikuGame()) {
                if (BossDeadDemo::IsRikuGet((UINT)enemy->getID() % 10000, enemy->getParam())) {
                    m_iCardID = BossDeadDemo::GetCardID((UINT)enemy->getID() % 10000, enemy->getParam());
                }
            } else {
                m_iCardID = BossDeadDemo::GetCardID((UINT)enemy->getID() % 10000, enemy->getParam());
            }
        }
    }
    if (inPlayerPos) {
        m_vPlayerPos = *inPlayerPos;
        m_bPlayerPos = TRUE;
    }
}

void CBossDeadDemo::main() {
    if (CGame::isRikuGame()) {
        rikuBossDeadDemoMain_();
    } else {
        soraBossDeadDemoMain_();
    }
    if ((m_tqTimer -= getFrameRate()) <= 0) m_tqTimer = 0;
    if (m_tqDeadDemoSETimer > 0) {
        if ((m_tqDeadDemoSETimer -= getFrameRate()) <= 0) {
            CSound::fadeoutAllSE(60, FALSE);
            CSound::setSEVolume(m_hDeadDemoSE, 30, 127, 0);
        }
    }
}

void CBossDeadDemo::permissionWhiteIn(BOOL inSw) { m_bPermissionWhiteIn = inSw; }

BOOL CBossDeadDemo::isStart() { return m_bStart; }

BOOL CBossDeadDemo::isFade() { return m_bFade; }

BOOL CBossDeadDemo::timeOver() { return m_tqTimer <= 0; }

#line 386
void CBossDeadDemo::soraBossDeadDemoMain_() {
    CBattleEnemy* enemy = (CBattleEnemy*)CTaskMan::getTask(m_hBossHandle);
    if (!enemy) return;
    enemy->bossDeadDemoCallback();
    switch (m_iStep) {
    case 0:
        if (!CPlayer::notifyBossDead(0)) break;
        setTimer(300);
        CGame::reqWhiteFadeOut(1);
        m_hDeadDemoSE = CSound::playSE(30, -1, 64);
        m_bStart = TRUE;
        m_iStep = 1;
    case 1:
        if (!timeOver()) break;
        CGame::reqWhiteFadeIn(10);
        CPackMan::setFrameEffect(2, 0, 0, 0);
        CPlaySpeedCtrl::setPlaySpeed(0.2f);
        CGameWork::setInt(1, 0xe2, -1);
        CGameWork::setFlag(1, 6, TRUE);
        CGameWork::setFlag(1, 0x21, TRUE);
        enemy->bossCardRevolverDestroy();
        enemy->setEntityFlag(0x80000000, 0x10000);
        enemy->makeExpPrize();
#line 389
        m_pPathCamera = new (com_malloc_d(sizeof(CPathCamera), __FILE__ "(389)", RET_ADDR())) CPathCamera(g_pPlayer);
        ASSERT(m_pPathCamera);
        m_pPathCamera->setLocal(TRUE);
        m_pPathCamera->init("cut001_01", TRUE, FALSE, 0x80);
        m_pPathCamera->ignorePlaySpeed(FALSE);
        m_pPathCamera->initBGCheck(TRUE);
        m_pPathCamera->run();
        m_iStep = 2;
        CSound::invalidateSE(TRUE);
        m_tqDeadDemoSETimer = 550;
        break;
    case 2:
        if (m_pPathCamera->isEnd()) {
            m_pPathCamera->init("cut001_02", TRUE, FALSE, 0x80);
            m_pPathCamera->run();
            m_iStep = 3;
        }
        break;
    case 3:
        if (m_pPathCamera->isEnd()) {
            m_pPathCamera->init("cut001_03", TRUE, FALSE, 0x80);
            m_pPathCamera->run();
            CPlayer::notifyBossDead(1);
            CSound::fadeoutAllBGM(m_pPathCamera->getTotalFrame(FALSE) * 5);
            m_iStep = 4;
        }
        break;
    case 4:
        if (m_pPathCamera->getCurrentFrame(FALSE) >= 30.0f) {
            CGame::reqWhiteFadeOut(150);
            m_bFade = TRUE;
            m_iStep = 5;
        }
        break;
    case 5:
        if (!CGame::isFadeDone()) break;
        enemy->bossDeadDemoEnd();
        if (m_bPermissionWhiteIn) {
            setTimer(1500);
            m_iStep = 6;
        }
        break;
    case 6:
        if (!timeOver()) break;
        CGame::reqWhiteFadeIn(40);
        CPackMan::setFrameEffect(0, 0, 0, 0);
        CPlaySpeedCtrl::setPlaySpeed(1.0f);
        m_pPathCamera->init("cut001", TRUE, FALSE, 0x80);
        m_pPathCamera->run();
        enemy->bossDeadDemoDispOff();
        enemy->killBossDeadEffect();
        CSound::invalidateSE(FALSE);
        movePlayer_();
        CPlayer::notifyBossDead(2);
        CGameWork::setFlag(1, 0x21, FALSE);
        m_iStep = 7;
        break;
    case 7:
        if (CGame::isFadeDone()) {
            m_bFade = FALSE;
            m_iStep = 8;
        }
        break;
    case 8: {
        CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(25);
#line 491
        ASSERT(bem);
        if (bem->getPrizeCount() > 0) break;
        m_iStep = 9;
        break;
    }
    case 9:
        if (CEffectMan::getEffect(CPlayer::getLevelUpEffect())) break;
        if (m_iCardID < 0) {
            m_iStep = 19;
            break;
        }
        CPlayer::notifyBossDead(3);
        m_iStep = 10;
        break;
    case 10:
        if (g_pPlayer->isMotionEnd()) {
            setTimer(1500);
            m_iStep = 11;
        }
        break;
    case 11:
        if (timeOver()) m_iStep = 12;
        break;
    case 12:
        if (enemy->bossDeadDemoSpecial()) {
            CPlayer::notifyBossDead(4);
            g_pPlayer->updateCoord();
            g_pPlayer->coordFix();
            CEntity* card = createGetCard_("WeaponR", 1);
            if (!card) {
                TRACE("warning:\x93\xfc\x8e\xe8\x83J\x81[\x83h\x90\xb6\x90\xac\x8e\xb8\x94s\x81I\x81I ... %d\n", m_iCardID + 100000);
                m_iStep = 19;
                break;
            }
            m_hCard = card->getHandle();
            m_pPathCamera->init("cut002", TRUE, FALSE, 0x80);
            m_pPathCamera->run();
            g_pPlayer->setTexAnmSeq(0, BossDeadDemo::Sora::TexAnmEyeDefault);
            g_pPlayer->setTexAnmSeq(1, BossDeadDemo::Sora::TexAnmLipSmile);
            m_iStep = 13;
        }
        break;
    case 13:
        if (m_pPathCamera->getCurrentFrame(FALSE) >= 20.0f) {
            CEntity* card = (CEntity*)CTaskMan::getTask(m_hCard);
            if (card) card->setupFade(0.0f, 1.0f, 2000, -1);
            m_iStep = 14;
        }
        break;
    case 14:
        if (g_pPlayer->getMotionCurrentTime() >= 11100) {
            BossDeadDemo::NotifyGetCard(enemy);
            m_iStep = 15;
        }
        break;
    case 15:
        if (!Gauge::CPrizeGauge::isNowDispCard()) {
            CPlayer::notifyBossDead(5);
            CEntity* card = (CEntity*)CTaskMan::getTask(m_hCard);
            if (card) card->changeMotion(2, 0, 0, -1, 0, 0);
            m_iStep = 19;
        }
        break;
    case 19:
        if (g_pPlayer->isMotionEnd()) {
            CBattleEnemy::setBossKilledFlag();
            m_iStep++;
        }
        break;
    }
}

#line 661
void CBossDeadDemo::rikuBossDeadDemoMain_() {
    CBattleEnemy* enemy = (CBattleEnemy*)CTaskMan::getTask(m_hBossHandle);
    if (!enemy) return;
    enemy->bossDeadDemoCallback();
    switch (m_iStep) {
    case 0:
        if (!CPlayer::notifyBossDead(0)) break;
        setTimer(300);
        CGame::reqWhiteFadeOut(1);
        m_hDeadDemoSE = CSound::playSE(30, -1, 64);
        m_bStart = TRUE;
        m_iStep = 1;
    case 1:
        if (!timeOver()) break;
        CGame::reqWhiteFadeIn(10);
        CPackMan::setFrameEffect(2, 0, 0, 0);
        CPlaySpeedCtrl::setPlaySpeed(0.2f);
        CGameWork::setInt(1, 0xe2, -1);
        CGameWork::setFlag(1, 6, TRUE);
        CGameWork::setFlag(1, 0x21, TRUE);
        enemy->bossCardRevolverDestroy();
        enemy->setEntityFlag(0x80000000, 0x10000);
        enemy->makeExpPrize();
#line 664
        m_pPathCamera = new (com_malloc_d(sizeof(CPathCamera), __FILE__ "(664)", RET_ADDR())) CPathCamera(g_pPlayer);
        ASSERT(m_pPathCamera);
        m_pPathCamera->setLocal(TRUE);
        m_pPathCamera->init("cut001_01", TRUE, FALSE, 0x80);
        m_pPathCamera->ignorePlaySpeed(FALSE);
        m_pPathCamera->initBGCheck(TRUE);
        m_pPathCamera->run();
        m_iStep = 2;
        CSound::invalidateSE(TRUE);
        m_tqDeadDemoSETimer = 550;
        break;
    case 2:
        if (m_pPathCamera->isEnd()) {
            m_pPathCamera->init("cut001_02", TRUE, FALSE, 0x80);
            m_pPathCamera->run();
            m_iStep = 3;
        }
        break;
    case 3:
        if (m_pPathCamera->isEnd()) {
            m_pPathCamera->init("cut001_03", TRUE, FALSE, 0x80);
            m_pPathCamera->run();
            CPlayer::notifyBossDead(1);
            CSound::fadeoutAllBGM(m_pPathCamera->getTotalFrame(FALSE) * 5);
            m_iStep = 4;
        }
        break;
    case 4:
        if (m_pPathCamera->getCurrentFrame(FALSE) >= 30.0f) {
            CGame::reqWhiteFadeOut(150);
            m_bFade = TRUE;
            m_iStep = 5;
        }
        break;
    case 5:
        if (!CGame::isFadeDone()) break;
        enemy->bossDeadDemoEnd();
        if (m_bPermissionWhiteIn) {
            setTimer(1500);
            m_iStep = 6;
        }
        break;
    case 6:
        if (!timeOver()) break;
        CGame::reqWhiteFadeIn(40);
        CPackMan::setFrameEffect(0, 0, 0, 0);
        CPlaySpeedCtrl::setPlaySpeed(1.0f);
        m_pPathCamera->init("cut001", TRUE, FALSE, 0x80);
        m_pPathCamera->run();
        enemy->bossDeadDemoDispOff();
        enemy->killBossDeadEffect();
        CSound::invalidateSE(FALSE);
        movePlayer_();
        CPlayer::notifyBossDead(2);
        CGameWork::setFlag(1, 0x21, FALSE);
        m_iStep = 7;
        break;
    case 7:
        if (CGame::isFadeDone()) {
            m_bFade = FALSE;
            m_iStep = 8;
        }
        break;
    case 8: {
        CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(25);
#line 766
        ASSERT(bem);
        if (bem->getPrizeCount() > 0) break;
        m_iStep = 9;
        break;
    }
    case 9:
        if (CEffectMan::getEffect(CPlayer::getLevelUpEffect())) break;
        if (m_iCardID < 0) {
            m_iStep = 19;
            break;
        }
        CPlayer::notifyBossDead(3);
        m_iStep = 10;
        break;
    case 10:
        if (g_pPlayer->isMotionEnd()) {
            setTimer(1500);
            m_iStep = 11;
        }
        break;
    case 11:
        if (timeOver()) m_iStep = 12;
        break;
    case 12:
        if (enemy->bossDeadDemoSpecial()) {
            CPlayer::notifyBossDead(4);
            g_pPlayer->updateCoord();
            g_pPlayer->coordFix();
            CEntity* card = createGetCard_("wep0", 3);
            if (!card) {
                TRACE("warning:\x93\xfc\x8e\xe8\x83J\x81[\x83h\x90\xb6\x90\xac\x8e\xb8\x94s\x81I\x81I ... %d\n", m_iCardID + 100000);
                m_iStep = 19;
                break;
            }
            m_hCard = card->getHandle();
            m_pPathCamera->init("riku_cardget02", TRUE, FALSE, 0x80);
            m_pPathCamera->run();
            g_pPlayer->setTexAnmSeq(0, BossDeadDemo::Riku::TexAnmEyeDefault);
            g_pPlayer->setTexAnmSeq(1, BossDeadDemo::Riku::TexAnmLipSmile);
            g_pPlayer->setDispOffFlag(3, 0);
            m_iStep = 13;
        }
        break;
    case 13:
        if (m_pPathCamera->getCurrentFrame(FALSE) >= 20.0f) {
            CEntity* card = (CEntity*)CTaskMan::getTask(m_hCard);
            if (card) card->setupFade(0.0f, 1.0f, 2000, -1);
            m_iStep = 14;
        }
        break;
    case 14:
        if (g_pPlayer->getMotionCurrentTime() >= 7500) {
            BossDeadDemo::NotifyGetCard(enemy);
            m_iStep = 15;
        }
        break;
    case 15:
        if (!Gauge::CPrizeGauge::isNowDispCard()) {
            CPlayer::notifyBossDead(5);
            CEntity* card = (CEntity*)CTaskMan::getTask(m_hCard);
            if (card) card->changeMotion(4, 0, 0, -1, 0, 0);
            m_iStep = 19;
        }
        break;
    case 19:
        if (g_pPlayer->isMotionEnd()) {
            CBattleEnemy::setBossKilledFlag();
            m_iStep++;
        }
        break;
    }
}

#line 884
CEntity* CBossDeadDemo::createGetCard_(const CHAR* inAttachBoneName, INT inMotNo) {
    CEntity* card = new (com_malloc_d(sizeof(CEntity), __FILE__ "(886)", RET_ADDR())) CEntity(g_pPlayer, (ObjType::eObjType)2);
    if (card && card->initalize(10, m_iCardID)) {
        card->Crd(0).setParent(&g_pPlayer->Crd(g_pPlayer->getBoneIndex(inAttachBoneName, 0)));
        card->changeMotion(inMotNo, 0, 0, 0, 0, 0);
        card->updateCoord();
        card->coordFix();
        card->setModelAlphaScale(0.0f, -1);
        card->loadModelColor_();
        return card;
    }
    return NULL;
}

#line 908
void CBossDeadDemo::movePlayer_() {
    if (m_bPlayerPos) {
        CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(25);
        ASSERT(bem);
        _ref(g_pPlayer)->warp(m_vPlayerPos);
        bem->warpPrize(m_vPlayerPos);
        FLOAT dir = Math3D::LimitPI(m_vPlayerPos.w);
        _ref(g_pPlayer)->setDir(dir);
        ((FVECTOR&)_ref(g_pPlayer)->getRot()).y = dir;
        _ref(g_pPlayer)->land(5.0f, 1.0f, FALSE);
    }
}

// SOURCE DATA: .data .rodata
