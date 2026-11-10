#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#define MAPENEMY_H_NO_COLL_ANIM /* MATCH: the original has no BodyCollision.h/Anim.h strings */
#include "System/TaskMan.h"
#include "Game/EntityFactory.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/BattleEnemy.h"
#include "Game/Enemy/HumanBoss.h"
#include "Game/OsdDef.h"
#include "System/Graphic/Anim.h"

/* MATCH: Placeholder: .rodata has "e123_062" after Anim.h's strings (from an unidentified header). */
inline void _createEnemyMotionName() {
    TRACE("e123_062");
}

#include "Game/Enemy/EnemyShadow.h"
#include "Game/Enemy/EnemySoldier.h"
#include "Game/Enemy/EnemyRednocturne.h"
#include "Game/Enemy/EnemyBluerhapsody.h"
#include "Game/Enemy/EnemyYellowopera.h"
#include "Game/Enemy/EnemyGreenrequiem.h"
#include "Game/Enemy/EnemyPowerwild.h"
#include "Game/Enemy/EnemyBouncywild.h"
#include "Game/Enemy/EnemyLargebody.h"
#include "Game/Enemy/EnemyFatbandit.h"
#include "Game/Enemy/EnemyBandit.h"
#include "Game/Enemy/EnemyWightnight.h"
#include "Game/Enemy/EnemyGargoyle.h"
#include "Game/Enemy/EnemySearchghost.h"
#include "Game/Enemy/EnemyAirsoldier.h"
#include "Game/Enemy/EnemyWhitemushroom.h"
#include "Game/Enemy/EnemyBlackfungus.h"
#include "Game/Enemy/EnemySeaneon.h"
#include "Game/Enemy/EnemyScrewdiver.h"
#include "Game/Enemy/EnemyAquatank.h"
#include "Game/Enemy/EnemyPirate.h"
#include "Game/Enemy/EnemyAirpirate.h"
#include "Game/Enemy/EnemyDarkball.h"
#include "Game/Enemy/EnemyWyvern.h"
#include "Game/Enemy/EnemyWizard.h"
#include "Game/Enemy/EnemyDefender.h"
#include "Game/Enemy/EnemyNeoshadow.h"
#include "Game/Enemy/EnemyBarrelspider.h"
#include "Game/Enemy/EnemyTornadostep.h"
#include "Game/Enemy/EnemyLoudness.h"
#include "Game/Enemy/EnemyCreapplant.h"
#include "Game/Enemy/EnemySpadesoldier.h"
#include "Game/Enemy/EnemyHeartsoldier.h"
#include "Game/Enemy/SampleEnemy.h"
#include "Game/Enemy/MapEnemy.h"
#include "Game/Enemy/MapEnemyShadow.h"
#include "Game/Enemy/MapEnemyGround.h"
#include "Game/Enemy/MapEnemyMushroom.h"
#include "Game/Enemy/MapEnemyAir.h"
#include "System/HeapMan.h"
#include "System/Math3D.h"
#include "System/Com.h"
#include "System/Debug.h"



/* Enemies whose code is not in the main executable (they are in DLLs): the original calls their constructors
 * through "jal 0". Only the constructor and the size matter here. Names: the DLL table's imports (tools/sndll.py
 * --names pairs each call site with its original name), sizes: types.h. */
class CEnemy100AcreBB_Beehive : public CBattleEnemy {
public:
    CEnemy100AcreBB_Beehive(CTask* inParent);

private:
    UBYTE _m_0x830[0x880 - 0x830];
};

class CEnemy100AcreBB_Bee : public CBattleEnemy {
public:
    CEnemy100AcreBB_Bee(CTask* inParent);

private:
    UBYTE _m_0x830[0x8a0 - 0x830];
};

class CTutorialHood : public CBattleEnemy {
public:
    CTutorialHood(CTask* inParent);

private:
    UBYTE _m_0x830[0x890 - 0x830];
};

class CTutorialLeon : public CBattleEnemy {
public:
    CTutorialLeon(CTask* inParent);

private:
    UBYTE _m_0x830[0x8a0 - 0x830];
};

class CBossGuardarmor : public CBattleEnemy {
public:
    CBossGuardarmor(CTask* inParent);

private:
    UBYTE _m_0x830[0x8c0 - 0x830];
};

class CBossCloud : public CBattleEnemy {
public:
    CBossCloud(CTask* inParent);

private:
    UBYTE _m_0x830[0x8c0 - 0x830];
};

class CBossHades : public CBattleEnemy {
public:
    CBossHades(CTask* inParent);

private:
    UBYTE _m_0x830[0x8f0 - 0x830];
};

class CBossParasiteCage : public CBattleEnemy {
public:
    CBossParasiteCage(CTask* inParent);

private:
    UBYTE _m_0x830[0x8a0 - 0x830];
};

class CBossHook : public CBattleEnemy {
public:
    CBossHook(CTask* inParent);

private:
    UBYTE _m_0x830[0x920 - 0x830];
};

class CBossVexen : public CBattleEnemy {
public:
    CBossVexen(CTask* inParent);

private:
    UBYTE _m_0x830[0x900 - 0x830];
};

class CBossRiku : public CBattleEnemy {
public:
    CBossRiku(CTask* inParent);

private:
    UBYTE _m_0x830[0x900 - 0x830];
};

class CBossLarxene : public CBattleEnemy {
public:
    CBossLarxene(CTask* inParent);

private:
    UBYTE _m_0x830[0x930 - 0x830];
};

class CBossAxel : public CBattleEnemy {
public:
    CBossAxel(CTask* inParent);

private:
    UBYTE _m_0x830[0x8f0 - 0x830];
};

class CBossTrickMaster : public CBattleEnemy {
public:
    CBossTrickMaster(CTask* inParent);

private:
    UBYTE _m_0x830[0x8b0 - 0x830];
};

class CBossJafar : public CBattleEnemy {
public:
    CBossJafar(CTask* inParent);

private:
    UBYTE _m_0x830[0xa50 - 0x830];
};

class CBossJafarBird : public CBattleEnemy {
public:
    CBossJafarBird(CTask* inParent);

private:
    UBYTE _m_0x830[0x8e0 - 0x830];
};

class CBossJafarLamp : public CBattleEnemy {
public:
    CBossJafarLamp(CTask* inParent);

private:
    UBYTE _m_0x830[0x870 - 0x830];
};

class CBossBoogie : public CBattleEnemy {
public:
    CBossBoogie(CTask* inParent);

private:
    UBYTE _m_0x830[0x8f0 - 0x830];
};

class CBossBoogieDice : public CBattleEnemy {
public:
    CBossBoogieDice(CTask* inParent);

private:
    UBYTE _m_0x830[0x870 - 0x830];
};

class CBossUrsula : public CBattleEnemy {
public:
    CBossUrsula(CTask* inParent);

private:
    UBYTE _m_0x830[0x910 - 0x830];
};

class CBossMDragon : public CBattleEnemy {
public:
    CBossMDragon(CTask* inParent);

private:
    UBYTE _m_0x830[0x8e0 - 0x830];
};

class CBossDarkSide : public CBattleEnemy {
public:
    CBossDarkSide(CTask* inParent);

private:
    UBYTE _m_0x830[0x9b0 - 0x830];
};

class CBossMarluxia2 : public CBattleEnemy {
public:
    CBossMarluxia2(CTask* inParent);

private:
    UBYTE _m_0x830[0x980 - 0x830];
};

class CBossMarluxia2Funnel : public CBattleEnemy {
public:
    CBossMarluxia2Funnel(CTask* inParent);

private:
    UBYTE _m_0x830[0x8b0 - 0x830];
};

class CBossMarluxia2Scythe : public CBattleEnemy {
public:
    CBossMarluxia2Scythe(CTask* inParent);

private:
    UBYTE _m_0x830[0x850 - 0x830];
};

class CBossMarluxia3 : public CBattleEnemy {
public:
    CBossMarluxia3(CTask* inParent);

private:
    UBYTE _m_0x830[0xa90 - 0x830];
};

class CBossMarluxia3Specter : public CBattleEnemy {
public:
    CBossMarluxia3Specter(CTask* inParent);

private:
    UBYTE _m_0x830[0x8e0 - 0x830];
};

class CBossMarluxia : public CBattleEnemy {
public:
    CBossMarluxia(CTask* inParent);

private:
    UBYTE _m_0x830[0x8e0 - 0x830];
};

class CBossLexaeus : public CBattleEnemy {
public:
    CBossLexaeus(CTask* inParent);

private:
    UBYTE _m_0x830[0x920 - 0x830];
};

class CBossZexion : public CBattleEnemy {
public:
    CBossZexion(CTask* inParent);

private:
    UBYTE _m_0x830[0x940 - 0x830];
};

class CBossZexionBook : public CBattleEnemy {
public:
    CBossZexionBook(CTask* inParent);

private:
    UBYTE _m_0x830[0x970 - 0x830];
};

class CBossAnsem : public CBattleEnemy {
public:
    CBossAnsem(CTask* inParent);

private:
    UBYTE _m_0x830[0x9b0 - 0x830];
};

class CBossUrsulaTentacle : public CBattleEnemy {
public:
    CBossUrsulaTentacle(CTask* inParent);

private:
    UBYTE _m_0x830[0x850 - 0x830];
};

class CBossUrsulaTentacle1 : public CBossUrsulaTentacle {
public:
    CBossUrsulaTentacle1(CTask* inParent) : CBossUrsulaTentacle(inParent) {}
    virtual ~CBossUrsulaTentacle1();
};

class CBossUrsulaTentacle2 : public CBossUrsulaTentacle {
public:
    CBossUrsulaTentacle2(CTask* inParent) : CBossUrsulaTentacle(inParent) {}
    virtual ~CBossUrsulaTentacle2();
};

class CBossUrsulaTentacle3 : public CBossUrsulaTentacle {
public:
    CBossUrsulaTentacle3(CTask* inParent) : CBossUrsulaTentacle(inParent) {}
    virtual ~CBossUrsulaTentacle3();
};

class CBossUrsulaTentacle4 : public CBossUrsulaTentacle {
public:
    CBossUrsulaTentacle4(CTask* inParent) : CBossUrsulaTentacle(inParent) {}
    virtual ~CBossUrsulaTentacle4();
};

/* DLL imports: undefined here, 0 for the link (config/SLUS_217.99/dll_imports.ld): the same "jal 0" as the
 * original, and each use keeps its relocation (tools/sndll.py); the Tentacles' vtables are imports too. */
/* DLL imports, 0 in config/SLUS_217.99/dll_imports.ld (the loader patches their uses):
 * __23CEnemy100AcreBB_BeehiveP5CTask
 * __19CEnemy100AcreBB_BeeP5CTask
 * __13CTutorialHoodP5CTask
 * __13CTutorialLeonP5CTask
 * __15CBossGuardarmorP5CTask
 * __10CBossCloudP5CTask
 * __10CBossHadesP5CTask
 * __17CBossParasiteCageP5CTask
 * __9CBossHookP5CTask
 * __10CBossVexenP5CTask
 * __9CBossRikuP5CTask
 * __12CBossLarxeneP5CTask
 * __9CBossAxelP5CTask
 * __16CBossTrickMasterP5CTask
 * __10CBossJafarP5CTask
 * __14CBossJafarBirdP5CTask
 * __14CBossJafarLampP5CTask
 * __11CBossBoogieP5CTask
 * __15CBossBoogieDiceP5CTask
 * __11CBossUrsulaP5CTask
 * __12CBossMDragonP5CTask
 * __13CBossDarkSideP5CTask
 * __14CBossMarluxia2P5CTask
 * __20CBossMarluxia2FunnelP5CTask
 * __20CBossMarluxia2ScytheP5CTask
 * __14CBossMarluxia3P5CTask
 * __21CBossMarluxia3SpecterP5CTask
 * __13CBossMarluxiaP5CTask
 * __12CBossLexaeusP5CTask
 * __11CBossZexionP5CTask
 * __15CBossZexionBookP5CTask
 * __10CBossAnsemP5CTask
 * __19CBossUrsulaTentacleP5CTask
 * _vt$20CBossUrsulaTentacle1
 * _vt$20CBossUrsulaTentacle1$4CFSM
 * _vt$20CBossUrsulaTentacle2
 * _vt$20CBossUrsulaTentacle2$4CFSM
 * _vt$20CBossUrsulaTentacle3
 * _vt$20CBossUrsulaTentacle3$4CFSM
 * _vt$20CBossUrsulaTentacle4
 * _vt$20CBossUrsulaTentacle4$4CFSM
 */











CEntity* CEntityFactory::createEnemy(INT inRsrcID, CTask* inParent, void* inExtraInfo) {
    CTask* parent;
    CBattleEnemy* enemy;

    if (inParent == NULL) {
        parent = CTaskMan::getSysTask(25);
        if (parent == NULL) parent = this;
    } else {
        parent = inParent;
    }

    switch (inRsrcID) {
    case 1: case 32:
        enemy = new KH_AT("(127)") CEnemyShadow(parent);
        break;
    case 2:
        enemy = new KH_AT("(131)") CEnemySoldier(parent);
        break;
    case 3:
        enemy = new KH_AT("(135)") CEnemyRednocturne(parent);
        break;
    case 4:
        enemy = new KH_AT("(139)") CEnemyBluerhapsody(parent);
        break;
    case 5:
        enemy = new KH_AT("(143)") CEnemyYellowopera(parent);
        break;
    case 6:
        enemy = new KH_AT("(147)") CEnemyGreenrequiem(parent);
        break;
    case 7:
        enemy = new KH_AT("(151)") CEnemyPowerwild(parent);
        break;
    case 8:
        enemy = new KH_AT("(155)") CEnemyBouncywild(parent);
        break;
    case 9:
        enemy = new KH_AT("(159)") CEnemyLargebody(parent);
        break;
    case 10:
        enemy = new KH_AT("(163)") CEnemyFatbandit(parent);
        break;
    case 11:
        enemy = new KH_AT("(167)") CEnemyBandit(parent);
        break;
    case 12:
        enemy = new KH_AT("(171)") CEnemyWightnight(parent);
        break;
    case 13:
        enemy = new KH_AT("(175)") CEnemyGargoyle(parent);
        break;
    case 14: case 33:
        enemy = new KH_AT("(180)") CEnemySearchghost(parent);
        break;
    case 15:
        enemy = new KH_AT("(184)") CEnemyAirsoldier(parent);
        break;
    case 16: case 34:
        enemy = new KH_AT("(189)") CEnemyWhitemushroom(parent);
        break;
    case 17: case 35:
        enemy = new KH_AT("(194)") CEnemyBlackfungus(parent);
        break;
    case 18:
        enemy = new KH_AT("(198)") CEnemySeaneon(parent);
        break;
    case 19:
        enemy = new KH_AT("(202)") CEnemyScrewdiver(parent);
        break;
    case 20:
        enemy = new KH_AT("(206)") CEnemyAquatank(parent);
        break;
    case 21:
        enemy = new KH_AT("(210)") CEnemyPirate(parent);
        break;
    case 22:
        enemy = new KH_AT("(214)") CEnemyAirpirate(parent);
        break;
    case 23:
        enemy = new KH_AT("(218)") CEnemyDarkball(parent);
        break;
    case 24:
        enemy = new KH_AT("(222)") CEnemyWyvern(parent);
        break;
    case 25:
        enemy = new KH_AT("(226)") CEnemyWizard(parent);
        break;
    case 26:
        enemy = new KH_AT("(230)") CEnemyDefender(parent);
        break;
    case 27:
        enemy = new KH_AT("(234)") CEnemyNeoshadow(parent);
        break;
    case 28:
        enemy = new KH_AT("(238)") CEnemyBarrelspider(parent);
        break;
    case 29:
        enemy = new KH_AT("(242)") CEnemyTornadostep(parent);
        break;
    case 30:
        enemy = new KH_AT("(246)") CEnemyLoudness(parent);
        break;
    case 31: case 36:
        enemy = new KH_AT("(251)") CEnemyCreapplant(parent);
        break;
    case 37:
        enemy = new KH_AT("(256)") CEnemy100AcreBB_Beehive(parent);
        break;
    case 38:
        enemy = new KH_AT("(260)") CEnemy100AcreBB_Bee(parent);
        break;
    case 100:
        enemy = new KH_AT("(265)") CTutorialHood(parent);
        break;
    case 101:
        enemy = new KH_AT("(269)") CTutorialLeon(parent);
        break;
    case 102:
        enemy = new KH_AT("(274)") CBossGuardarmor(parent);
        break;
    case 106:
        enemy = new KH_AT("(278)") CBossCloud(parent);
        break;
    case 107:
        enemy = new KH_AT("(282)") CBossHades(parent);
        break;
    case 110:
        enemy = new KH_AT("(286)") CBossParasiteCage(parent);
        break;
    case 112:
        enemy = new KH_AT("(290)") CBossHook(parent);
        break;
    case 114:
        enemy = new KH_AT("(294)") CBossVexen(parent);
        break;
    case 116:
        enemy = new KH_AT("(298)") CBossRiku(parent);
        break;
    case 117:
        enemy = new KH_AT("(302)") CBossLarxene(parent);
        break;
    case 118:
        enemy = new KH_AT("(306)") CBossAxel(parent);
        break;
    case 105:
        enemy = new KH_AT("(310)") CBossTrickMaster(parent);
        break;
    case 108:
        enemy = new KH_AT("(314)") CBossJafar(parent);
        break;
    case 125:
        enemy = new KH_AT("(317)") CBossJafarBird(parent);
        break;
    case 126:
        enemy = new KH_AT("(320)") CBossJafarLamp(parent);
        break;
    case 109:
        enemy = new KH_AT("(324)") CBossBoogie(parent);
        break;
    case 209:
        enemy = new KH_AT("(327)") CBossBoogieDice(parent);
        break;
    case 111:
        enemy = new KH_AT("(331)") CBossUrsula(parent);
        break;
    case 201:
        enemy = new KH_AT("(334)") CBossUrsulaTentacle1(parent);
        break;
    case 202:
        enemy = new KH_AT("(337)") CBossUrsulaTentacle2(parent);
        break;
    case 203:
        enemy = new KH_AT("(340)") CBossUrsulaTentacle3(parent);
        break;
    case 204:
        enemy = new KH_AT("(343)") CBossUrsulaTentacle4(parent);
        break;
    case 113:
        enemy = new KH_AT("(347)") CBossMDragon(parent);
        break;
    case 115:
        enemy = new KH_AT("(351)") CBossDarkSide(parent);
        break;
    case 120:
        enemy = new KH_AT("(355)") CBossMarluxia2(parent);
        break;
    case 135:
        enemy = new KH_AT("(358)") CBossMarluxia2Funnel(parent);
        break;
    case 136: case 137:
        enemy = new KH_AT("(362)") CBossMarluxia2Scythe(parent);
        break;
    case 132:
        enemy = new KH_AT("(366)") CBossMarluxia3(parent);
        break;
    case 138:
        enemy = new KH_AT("(369)") CBossMarluxia3Specter(parent);
        break;
    case 119:
        enemy = new KH_AT("(373)") CBossMarluxia(parent);
        break;
    case 121:
        enemy = new KH_AT("(377)") CBossLexaeus(parent);
        break;
    case 124:
        enemy = new KH_AT("(381)") CBossZexion(parent);
        break;
    case 133:
        enemy = new KH_AT("(384)") CBossZexionBook(parent);
        break;
    case 122:
        enemy = new KH_AT("(388)") CBossAnsem(parent);
        break;
    case 103:
        enemy = new KH_AT("(393)") CEnemySpadesoldier(parent);
        break;
    case 104:
        enemy = new KH_AT("(397)") CEnemyHeartsoldier(parent);
        break;
    default:
        enemy = new KH_AT("(401)") CSampleEnemy(parent);
        break;
    }

    if (enemy != NULL && inExtraInfo != NULL) {
        enemy->setParam(*(INT*)inExtraInfo);
    }
    return enemy;
}

CEntity* CEntityFactory::createMapEnemy(const OBJ_INFO& inObjInfo, CTask* inParent, void* inExtraInfo) {
    CTask* parent;
    CMapEnemy* enemy;

    if (inParent == NULL) {
        parent = CTaskMan::getSysTask(24);
        if (parent == NULL) parent = this;
    } else {
        parent = inParent;
    }

    switch (inObjInfo.sID) {
    case 1: case 27: case 32: case 302:
        enemy = new KH_AT("(440)") CMapEnemyShadow(parent);
        break;
    case 2: case 7: case 9: case 10: case 26: case 30:
        enemy = new KH_AT("(449)") CMapEnemyGround(parent);
        break;
    case 16: case 17: case 34: case 35:
        enemy = new KH_AT("(456)") CMapEnemyMushroom(parent);
        break;
    case 13: case 14: case 20: case 23: case 24: case 33:
        enemy = new KH_AT("(469)") CMapEnemyAir(parent);
        break;
    default:
        enemy = new KH_AT("(473)") CMapEnemy(parent);
        break;
    }

    if (enemy != NULL) {
        enemy->setMapEnemyParam(*(INT*)inExtraInfo);
        if (!enemy->initalize(inObjInfo.sGroupID, inObjInfo.sID)) {
            enemy->out();
            TRACE("Create entity failed!! ... GROUP_ID = 0x%08x, RSRC_ID = 0x%08x\n", inObjInfo.sGroupID, inObjInfo.sID);
            return NULL;
        }
        TRACE("Create entity ... GROUP_ID = 0x%08x, RSRC_ID = 0x%08x\n", inObjInfo.sGroupID, inObjInfo.sID);
        FLOAT dir = Math3D::LimitPI(DEG2RAD(inObjInfo.dir));
        FVECTOR pos;
        pos.set(inObjInfo.x, inObjInfo.y + inObjInfo.height, inObjInfo.z);
        if (inObjInfo.height == 0.0f) {
            enemy->warp(pos);
            ((FVECTOR&)enemy->getRot()).y = dir;
            enemy->land(5.0f, 1.0f, FALSE);
            enemy->updateCoord();
            enemy->coordFix();
            enemy->setStartPos(enemy->getPos());
            enemy->updateBodyColl();
        } else {
            enemy->positionFix(pos, dir);
            enemy->setStartPos(pos);
        }
    }
    return enemy;
}

// SOURCE DATA: .data .rodata
