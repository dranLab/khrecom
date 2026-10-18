#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h"
#define MAPENEMY_H_NO_COLL_ANIM
#include "Game/EntityFactory.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/BattleEnemy.h"
#include "Game/Enemy/HumanBoss.h"
#include "Game/OsdDef.h"
#include "System/Graphic/Anim.h"

/* Placeholder: .rodata has "e123_062" after Anim.h's strings (from an unidentified header). */
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
#include "System/TaskMan.h"
#include "System/HeapMan.h"
#include "System/Math3D.h"
#include "System/Com.h"
#include "System/Debug.h"

inline void* operator new(size_t, void* p) { return p; }

/* Enemies whose code is not in the main executable (map overlays): the original calls their constructors through
 * "jal 0". Only the constructor and the size matter here; the names are guesses from the debug info (classes and
 * sizes), unknown ones are named after their resource ID. */
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

class CEnemyOverlay110 : public CBattleEnemy {
public:
    CEnemyOverlay110(CTask* inParent);

private:
    UBYTE _m_0x830[0x8a0 - 0x830];
};

class CEnemyOverlay112 : public CBattleEnemy {
public:
    CEnemyOverlay112(CTask* inParent);

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

class CEnemyOverlay105 : public CBattleEnemy {
public:
    CEnemyOverlay105(CTask* inParent);

private:
    UBYTE _m_0x830[0x8b0 - 0x830];
};

class CEnemyOverlay108 : public CBattleEnemy {
public:
    CEnemyOverlay108(CTask* inParent);

private:
    UBYTE _m_0x830[0xa50 - 0x830];
};

class CEnemyOverlay125 : public CBattleEnemy {
public:
    CEnemyOverlay125(CTask* inParent);

private:
    UBYTE _m_0x830[0x8e0 - 0x830];
};

class CEnemyOverlay126 : public CBattleEnemy {
public:
    CEnemyOverlay126(CTask* inParent);

private:
    UBYTE _m_0x830[0x870 - 0x830];
};

class CEnemyOverlay109 : public CBattleEnemy {
public:
    CEnemyOverlay109(CTask* inParent);

private:
    UBYTE _m_0x830[0x8f0 - 0x830];
};

class CEnemyOverlay209 : public CBattleEnemy {
public:
    CEnemyOverlay209(CTask* inParent);

private:
    UBYTE _m_0x830[0x870 - 0x830];
};

class CBossUrsula : public CBattleEnemy {
public:
    CBossUrsula(CTask* inParent);

private:
    UBYTE _m_0x830[0x910 - 0x830];
};

class CEnemyOverlay113 : public CBattleEnemy {
public:
    CEnemyOverlay113(CTask* inParent);

private:
    UBYTE _m_0x830[0x8e0 - 0x830];
};

class CEnemyOverlay115 : public CBattleEnemy {
public:
    CEnemyOverlay115(CTask* inParent);

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

class CEnemyOverlay122 : public CBattleEnemy {
public:
    CEnemyOverlay122(CTask* inParent);

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

/* Ps2EeAs makes undefined weak symbols global (the link would fail), so define them as weak absolute 0: the
 * same "jal 0" as the original (whose relocation names func_0); the Tentacles' vtables are also at 0. */
__asm__(
    ".weak __23CEnemy100AcreBB_BeehiveP5CTask\n__23CEnemy100AcreBB_BeehiveP5CTask = 0\n"
    ".weak __19CEnemy100AcreBB_BeeP5CTask\n__19CEnemy100AcreBB_BeeP5CTask = 0\n"
    ".weak __13CTutorialHoodP5CTask\n__13CTutorialHoodP5CTask = 0\n"
    ".weak __13CTutorialLeonP5CTask\n__13CTutorialLeonP5CTask = 0\n"
    ".weak __15CBossGuardarmorP5CTask\n__15CBossGuardarmorP5CTask = 0\n"
    ".weak __10CBossCloudP5CTask\n__10CBossCloudP5CTask = 0\n"
    ".weak __10CBossHadesP5CTask\n__10CBossHadesP5CTask = 0\n"
    ".weak __16CEnemyOverlay110P5CTask\n__16CEnemyOverlay110P5CTask = 0\n"
    ".weak __16CEnemyOverlay112P5CTask\n__16CEnemyOverlay112P5CTask = 0\n"
    ".weak __10CBossVexenP5CTask\n__10CBossVexenP5CTask = 0\n"
    ".weak __9CBossRikuP5CTask\n__9CBossRikuP5CTask = 0\n"
    ".weak __12CBossLarxeneP5CTask\n__12CBossLarxeneP5CTask = 0\n"
    ".weak __9CBossAxelP5CTask\n__9CBossAxelP5CTask = 0\n"
    ".weak __16CEnemyOverlay105P5CTask\n__16CEnemyOverlay105P5CTask = 0\n"
    ".weak __16CEnemyOverlay108P5CTask\n__16CEnemyOverlay108P5CTask = 0\n"
    ".weak __16CEnemyOverlay125P5CTask\n__16CEnemyOverlay125P5CTask = 0\n"
    ".weak __16CEnemyOverlay126P5CTask\n__16CEnemyOverlay126P5CTask = 0\n"
    ".weak __16CEnemyOverlay109P5CTask\n__16CEnemyOverlay109P5CTask = 0\n"
    ".weak __16CEnemyOverlay209P5CTask\n__16CEnemyOverlay209P5CTask = 0\n"
    ".weak __11CBossUrsulaP5CTask\n__11CBossUrsulaP5CTask = 0\n"
    ".weak __16CEnemyOverlay113P5CTask\n__16CEnemyOverlay113P5CTask = 0\n"
    ".weak __16CEnemyOverlay115P5CTask\n__16CEnemyOverlay115P5CTask = 0\n"
    ".weak __14CBossMarluxia2P5CTask\n__14CBossMarluxia2P5CTask = 0\n"
    ".weak __20CBossMarluxia2FunnelP5CTask\n__20CBossMarluxia2FunnelP5CTask = 0\n"
    ".weak __20CBossMarluxia2ScytheP5CTask\n__20CBossMarluxia2ScytheP5CTask = 0\n"
    ".weak __14CBossMarluxia3P5CTask\n__14CBossMarluxia3P5CTask = 0\n"
    ".weak __21CBossMarluxia3SpecterP5CTask\n__21CBossMarluxia3SpecterP5CTask = 0\n"
    ".weak __13CBossMarluxiaP5CTask\n__13CBossMarluxiaP5CTask = 0\n"
    ".weak __12CBossLexaeusP5CTask\n__12CBossLexaeusP5CTask = 0\n"
    ".weak __11CBossZexionP5CTask\n__11CBossZexionP5CTask = 0\n"
    ".weak __15CBossZexionBookP5CTask\n__15CBossZexionBookP5CTask = 0\n"
    ".weak __16CEnemyOverlay122P5CTask\n__16CEnemyOverlay122P5CTask = 0\n"
    ".weak __19CBossUrsulaTentacleP5CTask\n__19CBossUrsulaTentacleP5CTask = 0\n"
    ".weak _vt$20CBossUrsulaTentacle1\n_vt$20CBossUrsulaTentacle1 = 0\n"
    ".weak _vt$20CBossUrsulaTentacle1$4CFSM\n_vt$20CBossUrsulaTentacle1$4CFSM = 0\n"
    ".weak _vt$20CBossUrsulaTentacle2\n_vt$20CBossUrsulaTentacle2 = 0\n"
    ".weak _vt$20CBossUrsulaTentacle2$4CFSM\n_vt$20CBossUrsulaTentacle2$4CFSM = 0\n"
    ".weak _vt$20CBossUrsulaTentacle3\n_vt$20CBossUrsulaTentacle3 = 0\n"
    ".weak _vt$20CBossUrsulaTentacle3$4CFSM\n_vt$20CBossUrsulaTentacle3$4CFSM = 0\n"
    ".weak _vt$20CBossUrsulaTentacle4\n_vt$20CBossUrsulaTentacle4 = 0\n"
    ".weak _vt$20CBossUrsulaTentacle4$4CFSM\n_vt$20CBossUrsulaTentacle4$4CFSM = 0\n"
);

/* Debug allocation: com_malloc_d gets the message "<file>(<line>)" and whatever $ra holds here. */
#define NEW_D(p, T, line, args)                                                 \
    {                                                                           \
        char* msg = __FILE__ "(" #line ")";                                     \
        UINT nRA;                                                               \
        __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA));                      \
        p = (T*)com_malloc_d(sizeof(T), msg, nRA);                              \
        if (p != NULL) p = new (p) T args;                                      \
    }

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
        NEW_D(enemy, CEnemyShadow, 127, (parent));
        break;
    case 2:
        NEW_D(enemy, CEnemySoldier, 131, (parent));
        break;
    case 3:
        NEW_D(enemy, CEnemyRednocturne, 135, (parent));
        break;
    case 4:
        NEW_D(enemy, CEnemyBluerhapsody, 139, (parent));
        break;
    case 5:
        NEW_D(enemy, CEnemyYellowopera, 143, (parent));
        break;
    case 6:
        NEW_D(enemy, CEnemyGreenrequiem, 147, (parent));
        break;
    case 7:
        NEW_D(enemy, CEnemyPowerwild, 151, (parent));
        break;
    case 8:
        NEW_D(enemy, CEnemyBouncywild, 155, (parent));
        break;
    case 9:
        NEW_D(enemy, CEnemyLargebody, 159, (parent));
        break;
    case 10:
        NEW_D(enemy, CEnemyFatbandit, 163, (parent));
        break;
    case 11:
        NEW_D(enemy, CEnemyBandit, 167, (parent));
        break;
    case 12:
        NEW_D(enemy, CEnemyWightnight, 171, (parent));
        break;
    case 13:
        NEW_D(enemy, CEnemyGargoyle, 175, (parent));
        break;
    case 14: case 33:
        NEW_D(enemy, CEnemySearchghost, 180, (parent));
        break;
    case 15:
        NEW_D(enemy, CEnemyAirsoldier, 184, (parent));
        break;
    case 16: case 34:
        NEW_D(enemy, CEnemyWhitemushroom, 189, (parent));
        break;
    case 17: case 35:
        NEW_D(enemy, CEnemyBlackfungus, 194, (parent));
        break;
    case 18:
        NEW_D(enemy, CEnemySeaneon, 198, (parent));
        break;
    case 19:
        NEW_D(enemy, CEnemyScrewdiver, 202, (parent));
        break;
    case 20:
        NEW_D(enemy, CEnemyAquatank, 206, (parent));
        break;
    case 21:
        NEW_D(enemy, CEnemyPirate, 210, (parent));
        break;
    case 22:
        NEW_D(enemy, CEnemyAirpirate, 214, (parent));
        break;
    case 23:
        NEW_D(enemy, CEnemyDarkball, 218, (parent));
        break;
    case 24:
        NEW_D(enemy, CEnemyWyvern, 222, (parent));
        break;
    case 25:
        NEW_D(enemy, CEnemyWizard, 226, (parent));
        break;
    case 26:
        NEW_D(enemy, CEnemyDefender, 230, (parent));
        break;
    case 27:
        NEW_D(enemy, CEnemyNeoshadow, 234, (parent));
        break;
    case 28:
        NEW_D(enemy, CEnemyBarrelspider, 238, (parent));
        break;
    case 29:
        NEW_D(enemy, CEnemyTornadostep, 242, (parent));
        break;
    case 30:
        NEW_D(enemy, CEnemyLoudness, 246, (parent));
        break;
    case 31: case 36:
        NEW_D(enemy, CEnemyCreapplant, 251, (parent));
        break;
    case 37:
        NEW_D(enemy, CEnemy100AcreBB_Beehive, 256, (parent));
        break;
    case 38:
        NEW_D(enemy, CEnemy100AcreBB_Bee, 260, (parent));
        break;
    case 100:
        NEW_D(enemy, CTutorialHood, 265, (parent));
        break;
    case 101:
        NEW_D(enemy, CTutorialLeon, 269, (parent));
        break;
    case 102:
        NEW_D(enemy, CBossGuardarmor, 274, (parent));
        break;
    case 106:
        NEW_D(enemy, CBossCloud, 278, (parent));
        break;
    case 107:
        NEW_D(enemy, CBossHades, 282, (parent));
        break;
    case 110:
        NEW_D(enemy, CEnemyOverlay110, 286, (parent));
        break;
    case 112:
        NEW_D(enemy, CEnemyOverlay112, 290, (parent));
        break;
    case 114:
        NEW_D(enemy, CBossVexen, 294, (parent));
        break;
    case 116:
        NEW_D(enemy, CBossRiku, 298, (parent));
        break;
    case 117:
        NEW_D(enemy, CBossLarxene, 302, (parent));
        break;
    case 118:
        NEW_D(enemy, CBossAxel, 306, (parent));
        break;
    case 105:
        NEW_D(enemy, CEnemyOverlay105, 310, (parent));
        break;
    case 108:
        NEW_D(enemy, CEnemyOverlay108, 314, (parent));
        break;
    case 125:
        NEW_D(enemy, CEnemyOverlay125, 317, (parent));
        break;
    case 126:
        NEW_D(enemy, CEnemyOverlay126, 320, (parent));
        break;
    case 109:
        NEW_D(enemy, CEnemyOverlay109, 324, (parent));
        break;
    case 209:
        NEW_D(enemy, CEnemyOverlay209, 327, (parent));
        break;
    case 111:
        NEW_D(enemy, CBossUrsula, 331, (parent));
        break;
    case 201:
        NEW_D(enemy, CBossUrsulaTentacle1, 334, (parent));
        break;
    case 202:
        NEW_D(enemy, CBossUrsulaTentacle2, 337, (parent));
        break;
    case 203:
        NEW_D(enemy, CBossUrsulaTentacle3, 340, (parent));
        break;
    case 204:
        NEW_D(enemy, CBossUrsulaTentacle4, 343, (parent));
        break;
    case 113:
        NEW_D(enemy, CEnemyOverlay113, 347, (parent));
        break;
    case 115:
        NEW_D(enemy, CEnemyOverlay115, 351, (parent));
        break;
    case 120:
        NEW_D(enemy, CBossMarluxia2, 355, (parent));
        break;
    case 135:
        NEW_D(enemy, CBossMarluxia2Funnel, 358, (parent));
        break;
    case 136: case 137:
        NEW_D(enemy, CBossMarluxia2Scythe, 362, (parent));
        break;
    case 132:
        NEW_D(enemy, CBossMarluxia3, 366, (parent));
        break;
    case 138:
        NEW_D(enemy, CBossMarluxia3Specter, 369, (parent));
        break;
    case 119:
        NEW_D(enemy, CBossMarluxia, 373, (parent));
        break;
    case 121:
        NEW_D(enemy, CBossLexaeus, 377, (parent));
        break;
    case 124:
        NEW_D(enemy, CBossZexion, 381, (parent));
        break;
    case 133:
        NEW_D(enemy, CBossZexionBook, 384, (parent));
        break;
    case 122:
        NEW_D(enemy, CEnemyOverlay122, 388, (parent));
        break;
    case 103:
        NEW_D(enemy, CEnemySpadesoldier, 393, (parent));
        break;
    case 104:
        NEW_D(enemy, CEnemyHeartsoldier, 397, (parent));
        break;
    default:
        NEW_D(enemy, CSampleEnemy, 401, (parent));
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
        NEW_D(enemy, CMapEnemyShadow, 440, (parent));
        break;
    case 2: case 7: case 9: case 10: case 26: case 30:
        NEW_D(enemy, CMapEnemyGround, 449, (parent));
        break;
    case 16: case 17: case 34: case 35:
        NEW_D(enemy, CMapEnemyMushroom, 456, (parent));
        break;
    case 13: case 14: case 20: case 23: case 24: case 33:
        NEW_D(enemy, CMapEnemyAir, 469, (parent));
        break;
    default:
        NEW_D(enemy, CMapEnemy, 473, (parent));
        break;
    }

    if (enemy != NULL) {
        enemy->setParam(*(INT*)inExtraInfo);
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
