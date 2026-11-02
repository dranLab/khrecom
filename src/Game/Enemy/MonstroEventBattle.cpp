#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include <string.h>
inline void* operator new(size_t, void* p) throw() { return p; }
#include "System/Task.h"
#include "System/TaskMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "Game/OsdDef.h"
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
inline void _monstroEventBattleEmptyString() {
    TRACE("");
}

#include "Game/Enemy/EnemyParam.h"
#include "Game/Enemy/MonstroEventBattle.h"
#include "Game/EntityFactory.h"
#include "Game/GameWork.h"
#include "Game/SceneInfo.h"
#include "Form/Menu/MonstroGauge.h"

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)
#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

#include "Game/Player/Player.h"

#include "Game/Player/PlayerGauge.h"

#include "Game/Enemy/BattleEnemyManager.h"




#include "System/Buf.h"












#include "System/RsrcDB.h"






/* the enemy parameter file (original name unknown) */
struct ENEMY_PARAM_VALUE {
    CHAR szName[12];
    FLOAT fValue;
};
struct ENEMY_PARAM_FILE {
    CHAR szCode[4];         /* 0x00 */
    UINT uiVersion;         /* 0x04 */
    UINT ofs08;             /* 0x08 */
    UBYTE _m_0x0c[0x3c];
    UINT ofs48;             /* 0x48 */
    UBYTE _m_0x4c[4];
    UINT ofs50;             /* 0x50 */
    UBYTE _m_0x54[0x3c];
    INT iValueNum;          /* 0x90 */
    ENEMY_PARAM_VALUE* pValue; /* 0x94 */
    FLOAT getValue(const char* inName) {
        for (INT i = 0; i < iValueNum; i++) {
            if (strcmp(pValue[i].szName, inName) == 0) return pValue[i].fValue;
        }
        return 0.0f;
    }
};

namespace MonstroEventBattle {
static CHAR* LimitTimeParamName = "LimitTime";
static CHAR* TotalDeadParamName = "TotalDead";
static CHAR* GaugeDownParamName = "GaugeDown";
/* read through references: the loads stay ordered after the preceding stores */
static inline CHAR*& _limitTime() { return LimitTimeParamName; }
static inline CHAR*& _totalDead() { return TotalDeadParamName; }
static inline CHAR*& _gaugeDown() { return GaugeDownParamName; }
static const INT EnemyCountTbl[4] = { 0, 7, 10, 5 };
}

BOOL CMonstroEventBattle::m_bHelp = 1;
INT CMonstroEventBattle::m_iAppearEnemyCount = 0;

#line 40 "c:/KHCOM/MainApp/Game/Enemy/MonstroEventBattle.cpp"
CMonstroEventBattle::CMonstroEventBattle(CTask* inParent) : CTask(inParent, 0x4000) {
    m_tqGaugeDownTimer = 30000;
    m_pHeader = NULL;
    m_iStep = 0;
    m_iGauge = 0;
    m_fGauge = 0;
    m_iGroup = 0;
    m_iEnemyNum = 0;
    m_bReraise = FALSE;
    m_bHelp = TRUE;
    m_iAppearEnemyCount = 0;
}

CMonstroEventBattle::~CMonstroEventBattle() {}

BOOL CMonstroEventBattle::setup() {
    OSD_HEADER* header = (OSD_HEADER*)CRsrcDB::getFirstData_Scene(0x20);
    if (!header) return FALSE;
    if (!header->addrMapping()) return FALSE;
    m_pHeader = header;
    CRsrcDB::initSeekRsrc(0x15, 120001);
    ENEMY_PARAM_FILE* param;
    for (INT i = 0; i < 2; i++) {
        CRsrcData* rsrc = CRsrcDB::seekNextRsrc();
        if (!rsrc) return FALSE;
        param = (ENEMY_PARAM_FILE*)rsrc->getRsrcData();
    }
#line 91
    ASSERT(param);
    if (!(param->uiVersion & 0x80000000) && strncmp(param->szCode, "@ENP", 4) == 0 && param->uiVersion == 1) {
        param->uiVersion |= 0x80000000;
        param->ofs08 = (UINT)param + param->ofs08;
        param->ofs48 = (UINT)param + param->ofs48;
        param->ofs50 = (UINT)param + param->ofs50;
        param->pValue = (ENEMY_PARAM_VALUE*)((CHAR*)param + (UINT)param->pValue);
    }
    m_tqLimitTime = (TIMEQ)(param->getValue(MonstroEventBattle::_limitTime()) * 60.0f * 50.0f);
    m_iTotalDead = (INT)param->getValue(MonstroEventBattle::_totalDead());
    m_tqGaugeDownTimer = m_tqGaugeDown = (TIMEQ)(param->getValue(MonstroEventBattle::_gaugeDown()) * 60.0f * 50.0f);
#line 99
    CMostroGauge* gauge = new (com_malloc_d(sizeof(CMostroGauge), __FILE__ "(99)", RET_ADDR())) CMostroGauge(this);
    if (!gauge) return FALSE;
    m_hGauge = gauge->getHandle();
    CGameWork::setFlag(1, 5, TRUE);
    CGameWork::setFlag(1, 6, TRUE);
    CGameWork::setInt(1, 0xE2, -1);
    return TRUE;
}

void CMonstroEventBattle::main() {
    CBattleEnemyManager* bem = (CBattleEnemyManager*)getParent();
#line 120
    ASSERT(bem);
    switch (m_iStep) {
    case 0:
        m_iEnemyNum = createEnemies_();
        m_iStep = 1;
        break;
    case 1:
        if (m_iAppearEnemyCount >= m_iEnemyNum) {
            CMostroGauge* gauge = (CMostroGauge*)CTaskMan::getTask(m_hGauge);
            if (gauge) gauge->exec((CMostroGauge::EXEC_CMD)6, 0);
            m_iStep = 2;
        }
        break;
    case 2: {
        CMostroGauge* gauge = (CMostroGauge*)CTaskMan::getTask(m_hGauge);
        if (gauge && gauge->exec((CMostroGauge::EXEC_CMD)0, m_tqLimitTime)) m_iStep = 3;
        break;
    }
    case 3: {
        CMostroGauge* gauge = (CMostroGauge*)CTaskMan::getTask(m_hGauge);
        if (gauge && gauge->exec((CMostroGauge::EXEC_CMD)1, 0)) {
            CGameWork::setFlag(1, 5, FALSE);
            CGameWork::setFlag(1, 6, FALSE);
            m_bHelp = FALSE;
            m_iStep = 4;
        }
        break;
    }
    case 4:
        setLife_();
        if (isTimeOver_()) {
            CGameWork::setFlag(1, 6, TRUE);
            CPlayer::notifyExterminate();
            m_iStep = 9;
        } else if (isFullGauge()) {
            CMostroGauge* gauge = (CMostroGauge*)CTaskMan::getTask(m_hGauge);
#line 185
            ASSERT(gauge);
            gauge->exec((CMostroGauge::EXEC_CMD)3, 0);
            gauge->exec((CMostroGauge::EXEC_CMD)2, 0);
            m_iStep = 5;
        } else {
            if (bem->getEnemyCount() < MonstroEventBattle::EnemyCountTbl[m_iGroup]) createEnemies_();
            gaugeDown();
        }
        break;
    case 5:
        if (bem->getEnemyCount() <= 0) {
            CGameWork::setFlag(1, 6, TRUE);
            CPlayer::notifyExterminate();
            m_iStep = 6;
        }
        break;
    case 6:
        if (Gauge::CPrizeGauge::isNowDispCard()) m_iStep = 7;
        break;
    case 7:
        if (!Gauge::CPrizeGauge::isNowDispCard() && CPlayer::isEnableBattleEnd()) {
            CGameWork::setByte(1, 2, 1);
            m_iStep = 10;
        }
        break;
    case 8:
        if (g_pPlayer && !g_pPlayer->m_bIsDead) {
            m_bReraise = FALSE;
            m_iStep = 4;
        }
        break;
    case 9:
        if (CPlayer::isEnableBattleEnd()) {
            CGameWork::setByte(1, 2, 2);
            m_iStep = 10;
        }
        break;
    case 10:
        break;
    }
    CardBattle::CCardBattle* cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
#line 254
    ASSERT(cb);
    if (cb->getEnchantKind() == 0x2E) m_bReraise = TRUE;
    if (g_pPlayer && g_pPlayer->m_bIsDead) {
        if (!m_bReraise) {
            CMostroGauge* gauge = (CMostroGauge*)CTaskMan::getTask(m_hGauge);
            if (gauge) {
                gauge->out();
                m_hGauge = 0;
                m_iStep = 10;
            }
        } else {
            m_iStep = 8;
        }
    }
}

BOOL CMonstroEventBattle::isFullGauge() const {
    return m_iGauge >= m_iTotalDead;
}

void CMonstroEventBattle::gaugeUp() {
    m_iGauge++;
    m_fGauge = m_iGauge;
    m_tqGaugeDownTimer = m_tqGaugeDown;
    TRACE("-------------------------------------------\n");
    TRACE("         \x83Q\x81[\x83W\x8f\xe3\x8f\xb8 = %d\n", m_iGauge);
    TRACE("-------------------------------------------\n");
}

void CMonstroEventBattle::gaugeDown() {
    TIMEQ tqFr = getFrameRate();
    m_fGauge -= 1.0f / (m_tqGaugeDown / 50) * (tqFr * 0.020000001f);
    m_tqGaugeDownTimer -= tqFr;
    if (m_tqGaugeDownTimer <= 0) {
        if (--m_iGauge < 0) m_iGauge = 0;
        m_fGauge = m_iGauge;
        m_tqGaugeDownTimer = m_tqGaugeDown;
        TRACE("-------------------------------------------\n");
        TRACE("         \x83Q\x81[\x83W\x83_\x83" "E\x83\x93 = %d\n", m_iGauge);
        TRACE("-------------------------------------------\n");
    }
}

BOOL CMonstroEventBattle::isMonstroEventBattleScene() {
    if (CSceneInfo::getCurrentAreaNum() == 5 && CSceneInfo::getCurrentSceneNum() == 0x41B) return TRUE;
    return FALSE;
}

BOOL CMonstroEventBattle::isHelp() {
    return m_bHelp;
}

void CMonstroEventBattle::addAppearEnemyCount() {
    m_iAppearEnemyCount++;
}

INT CMonstroEventBattle::createEnemies_() {
    OBJ_GROUP* group = m_pHeader->pFormation[0]->getEnemyGroup(m_iGroup);
#line 352
    ASSERT(group);
    CBattleEnemyManager* bem = (CBattleEnemyManager*)getParent();
#line 355
    ASSERT(bem);
    INT createNum = m_iTotalDead - m_iGauge - bem->getEnemyCount();
    if (createNum <= 0) return 0;
    if (group->iObjCount < createNum) createNum = group->iObjCount;
    INT count = 0;
    OBJ_INFO* info = group->pObjInfo;
    for (INT i = 0; i < createNum; i++, info++) {
        CEntity* p = CEntityFactory::getInstance()->createEntity(*info, NULL, &info->iParam);
        if (p) {
            count++;
            p->AfterInit();
        }
    }
    if (++m_iGroup >= 4) m_iGroup = 1;
    return count;
}

void CMonstroEventBattle::setLife_() {
    CMostroGauge* gauge = (CMostroGauge*)CTaskMan::getTask(m_hGauge);
#line 391
    ASSERT(gauge);
    FLOAT f = m_fGauge * 100.0f / m_iTotalDead;
    gauge->exec((CMostroGauge::EXEC_CMD)4, (INT)f);
}

BOOL CMonstroEventBattle::isTimeOver_() {
    CMostroGauge* gauge = (CMostroGauge*)CTaskMan::getTask(m_hGauge);
#line 402
    ASSERT(gauge);
    return gauge->exec((CMostroGauge::EXEC_CMD)5, 0);
}

// SOURCE DATA: .data .rodata
