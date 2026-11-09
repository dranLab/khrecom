#define SYSTEM_GRAPHIC_ANIM_H /* MATCH: the original's includes don't bring in Anim.h (no "2 > idx" in .rodata) */
#include "System/TaskMan.h"
#include "Game/EntityFactory.h"
#include "Game/Collision/CollUtil.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Gimmick/../Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "Game/Enemy/BattleEnemy.h"
#include "Game/Enemy/HumanBoss.h"
#include "Game/OsdDef.h"
#include "System/HeapMan.h"
#include "Game/Gimmick/BaseGimmick.h"
#include "Game/Gimmick/GimmickTreasureBox.h"
#include "Game/Gimmick/GimmickRideOn.h"
#include "Game/Gimmick/GimmickAngelArrow.h"
#include "Game/Gimmick/GimmickTutorial.h"
#include "Game/Gimmick/GimmickHitReturn.h"
#include "Game/Gimmick/GimmickDominoPillar.h"
#include "Game/Gimmick/GimmickUpFloor.h"
#include "Game/Gimmick/GimmickGuillotine.h"
#include "Game/Gimmick/GimmickJackBox.h"
#include "Game/Gimmick/GimmickRideDest.h"
#include "Game/Gimmick/GimmickFallTomb.h"
#include "Game/Gimmick/GimmickFountain.h"
#include "Game/Gimmick/GimmickTimeBomb.h"
#include "Game/Gimmick/GimmickToldPipe.h"
#include "Game/Gimmick/GimmickStatue.h"
#include "Game/Gimmick/GimmickThunderTrap.h"
#include "Game/Gimmick/GimmickSteamTrap.h"
#include "Game/Gimmick/GimmickDeceive.h"
#include "Game/Gimmick/GimmickDeceiveEnemy.h"
#include "Game/Gimmick/GimmickCarry.h"
#include "Game/Gimmick/GimmickBreakable.h"
#include "Game/Gimmick/GimmickHitRepetition.h"
#include "Game/Gimmick/GimmickOnceReaction.h"
#include "Game/Gimmick/GimmickJumpStand.h"
#include "Game/Gimmick/GimmickPlantParent.h"
#include "Game/Gimmick/GimmickPlantChild.h"
#include "Game/Gimmick/GimmickPlantBullet.h"
#include "Game/Gimmick/GimmickLotusLeaf.h"
#include "Game/Gimmick/GimmickFlowerLump.h"
#include "Game/Gimmick/GimmickSearchLaser.h"
#include "Game/Gimmick/GimmickWallHole.h"
#include "Game/Gimmick/GimmickBell.h"
#include "Game/Gimmick/GimmickHitMore.h"
#include "Game/Gimmick/GimmickSavePoint.h"
#include "Game/Gimmick/GimmickWarpPoint.h"
#include "Game/Gimmick/GimmickWindowLid.h"
#include "Game/Gimmick/SampleGimmick.h"



/* Gimmicks whose code is not in the main executable (map overlays): the original calls their constructors
 * through "jal 0". Only the constructor and the size matter here; the names are guesses from the debug info
 * (classes and sizes). */
class CGimmickTreasureBoxPooh : public CBaseGimmick {
public:
    CGimmickTreasureBoxPooh(CTask* inParent);

private:
    UBYTE _m_0x220[0x230 - 0x220];
};

class CGimmickBJFStand1 : public CBaseGimmick {
public:
    CGimmickBJFStand1(CTask* inParent);

private:
    UBYTE _m_0x220[0x280 - 0x220];
};

class CGimmickBJFStand2 : public CBaseGimmick {
public:
    CGimmickBJFStand2(CTask* inParent);

private:
    UBYTE _m_0x220[0x280 - 0x220];
};

class CGimmickBJFStand3 : public CBaseGimmick {
public:
    CGimmickBJFStand3(CTask* inParent);

private:
    UBYTE _m_0x220[0x280 - 0x220];
};

class CGimmickBJFStand4 : public CBaseGimmick {
public:
    CGimmickBJFStand4(CTask* inParent);

private:
    UBYTE _m_0x220[0x280 - 0x220];
};

class CGimmickBJFStand5 : public CBaseGimmick {
public:
    CGimmickBJFStand5(CTask* inParent);

private:
    UBYTE _m_0x220[0x280 - 0x220];
};

class CGimmickBJFStand6 : public CBaseGimmick {
public:
    CGimmickBJFStand6(CTask* inParent);

private:
    UBYTE _m_0x220[0x280 - 0x220];
};

class CGimmickBBGKnife : public CBaseGimmick {
public:
    CGimmickBBGKnife(CTask* inParent);

private:
    UBYTE _m_0x220[0x270 - 0x220];
};

class CGimmickBBGSaw : public CBaseGimmick {
public:
    CGimmickBBGSaw(CTask* inParent);

private:
    UBYTE _m_0x220[0x260 - 0x220];
};

class CGimmickBBGRecovery : public CBaseGimmick {
public:
    CGimmickBBGRecovery(CTask* inParent);

private:
    UBYTE _m_0x220[0x270 - 0x220];
};

class CGimmickBBGHedge : public CBaseGimmick {
public:
    CGimmickBBGHedge(CTask* inParent);

private:
    UBYTE _m_0x220[0x250 - 0x220];
};

class CGimmickBDSStand1 : public CBaseGimmick {
public:
    CGimmickBDSStand1(CTask* inParent);

private:
    UBYTE _m_0x220[0x240 - 0x220];
};

class CGimmickBDSStand2 : public CBaseGimmick {
public:
    CGimmickBDSStand2(CTask* inParent);

private:
    UBYTE _m_0x220[0x240 - 0x220];
};

class CGimmickBDSStand3 : public CBaseGimmick {
public:
    CGimmickBDSStand3(CTask* inParent);

private:
    UBYTE _m_0x220[0x240 - 0x220];
};

class CGimmickBDSStand4 : public CBaseGimmick {
public:
    CGimmickBDSStand4(CTask* inParent);

private:
    UBYTE _m_0x220[0x240 - 0x220];
};

class CGimmickBDSStand5 : public CBaseGimmick {
public:
    CGimmickBDSStand5(CTask* inParent);

private:
    UBYTE _m_0x220[0x240 - 0x220];
};

class CGimmickBDSStand6 : public CBaseGimmick {
public:
    CGimmickBDSStand6(CTask* inParent);

private:
    UBYTE _m_0x220[0x240 - 0x220];
};

class CGimmickBTMChair : public CBaseGimmick {
public:
    CGimmickBTMChair(CTask* inParent);

private:
    UBYTE _m_0x220[0x240 - 0x220];
};

class CGimmickBTMTable : public CBaseGimmick {
public:
    CGimmickBTMTable(CTask* inParent);

private:
    UBYTE _m_0x220[0x250 - 0x220];
};

class CGimmickIsLand : public CBaseGimmick {
public:
    CGimmickIsLand(CTask* inParent, INT inID);

private:
    UBYTE _m_0x220[0x240 - 0x220];
};

class CGimmickBossIsLand : public CBaseGimmick {
public:
    CGimmickBossIsLand(CTask* inParent);

private:
    UBYTE _m_0x220[0x230 - 0x220];
};

class CGimmickAcid : public CBaseGimmick {
public:
    CGimmickAcid(CTask* inParent);

private:
    UBYTE _m_0x220[0x230 - 0x220];
};

class CGimmickPinokio : public CBaseGimmick {
public:
    CGimmickPinokio(CTask* inParent);

private:
    UBYTE _m_0x220[0x230 - 0x220];
};

class CGimmickShip : public CBaseGimmick {
public:
    CGimmickShip(CTask* inParent);

private:
    UBYTE _m_0x220[0x230 - 0x220];
};

class CGimmickBottle : public CBaseGimmick {
public:
    CGimmickBottle(CTask* inParent);

private:
    UBYTE _m_0x220[0x220 - 0x220];
};

class CGimmickBombBarrel : public CBaseGimmick {
public:
    CGimmickBombBarrel(CTask* inParent);

private:
    UBYTE _m_0x220[0x240 - 0x220];
};

class CGimmickMDFooting : public CBaseGimmick {
public:
    CGimmickMDFooting(CTask* inParent);

private:
    UBYTE _m_0x220[0x240 - 0x220];
};

class CGimmick100Acre01Bush : public CBaseGimmick {
public:
    CGimmick100Acre01Bush(CTask* inParent);

private:
    UBYTE _m_0x220[0x220 - 0x220];
};

class CGimmickChasePoo : public CBaseGimmick {
public:
    CGimmickChasePoo(CTask* inParent);

private:
    UBYTE _m_0x220[0x290 - 0x220];
};

class CGimmick100Acre02Veg : public CBaseGimmick {
public:
    CGimmick100Acre02Veg(CTask* inParent);

private:
    UBYTE _m_0x220[0x220 - 0x220];
};

class CGimmick100Acre02Coll : public CBaseGimmick {
public:
    CGimmick100Acre02Coll(CTask* inParent);

private:
    UBYTE _m_0x220[0x220 - 0x220];
};

class CGimmick100AcreBBWall : public CBaseGimmick {
public:
    CGimmick100AcreBBWall(CTask* inParent);

private:
    UBYTE _m_0x220[0x220 - 0x220];
};

/* Ps2EeAs makes undefined weak symbols global (the link would fail), so define them as weak absolute 0: the
 * same "jal 0" as the original (whose relocation names func_0, hence the overview's "NOT matching"). */
__asm__(
    ".weak __23CGimmickTreasureBoxPoohP5CTask\n__23CGimmickTreasureBoxPoohP5CTask = 0\n"
    ".weak __17CGimmickBJFStand1P5CTask\n__17CGimmickBJFStand1P5CTask = 0\n"
    ".weak __17CGimmickBJFStand2P5CTask\n__17CGimmickBJFStand2P5CTask = 0\n"
    ".weak __17CGimmickBJFStand3P5CTask\n__17CGimmickBJFStand3P5CTask = 0\n"
    ".weak __17CGimmickBJFStand4P5CTask\n__17CGimmickBJFStand4P5CTask = 0\n"
    ".weak __17CGimmickBJFStand5P5CTask\n__17CGimmickBJFStand5P5CTask = 0\n"
    ".weak __17CGimmickBJFStand6P5CTask\n__17CGimmickBJFStand6P5CTask = 0\n"
    ".weak __16CGimmickBBGKnifeP5CTask\n__16CGimmickBBGKnifeP5CTask = 0\n"
    ".weak __14CGimmickBBGSawP5CTask\n__14CGimmickBBGSawP5CTask = 0\n"
    ".weak __19CGimmickBBGRecoveryP5CTask\n__19CGimmickBBGRecoveryP5CTask = 0\n"
    ".weak __16CGimmickBBGHedgeP5CTask\n__16CGimmickBBGHedgeP5CTask = 0\n"
    ".weak __17CGimmickBDSStand1P5CTask\n__17CGimmickBDSStand1P5CTask = 0\n"
    ".weak __17CGimmickBDSStand2P5CTask\n__17CGimmickBDSStand2P5CTask = 0\n"
    ".weak __17CGimmickBDSStand3P5CTask\n__17CGimmickBDSStand3P5CTask = 0\n"
    ".weak __17CGimmickBDSStand4P5CTask\n__17CGimmickBDSStand4P5CTask = 0\n"
    ".weak __17CGimmickBDSStand5P5CTask\n__17CGimmickBDSStand5P5CTask = 0\n"
    ".weak __17CGimmickBDSStand6P5CTask\n__17CGimmickBDSStand6P5CTask = 0\n"
    ".weak __16CGimmickBTMChairP5CTask\n__16CGimmickBTMChairP5CTask = 0\n"
    ".weak __16CGimmickBTMTableP5CTask\n__16CGimmickBTMTableP5CTask = 0\n"
    ".weak __14CGimmickIsLandP5CTaski\n__14CGimmickIsLandP5CTaski = 0\n"
    ".weak __18CGimmickBossIsLandP5CTask\n__18CGimmickBossIsLandP5CTask = 0\n"
    ".weak __12CGimmickAcidP5CTask\n__12CGimmickAcidP5CTask = 0\n"
    ".weak __15CGimmickPinokioP5CTask\n__15CGimmickPinokioP5CTask = 0\n"
    ".weak __12CGimmickShipP5CTask\n__12CGimmickShipP5CTask = 0\n"
    ".weak __14CGimmickBottleP5CTask\n__14CGimmickBottleP5CTask = 0\n"
    ".weak __18CGimmickBombBarrelP5CTask\n__18CGimmickBombBarrelP5CTask = 0\n"
    ".weak __17CGimmickMDFootingP5CTask\n__17CGimmickMDFootingP5CTask = 0\n"
    ".weak __21CGimmick100Acre01BushP5CTask\n__21CGimmick100Acre01BushP5CTask = 0\n"
    ".weak __16CGimmickChasePooP5CTask\n__16CGimmickChasePooP5CTask = 0\n"
    ".weak __20CGimmick100Acre02VegP5CTask\n__20CGimmick100Acre02VegP5CTask = 0\n"
    ".weak __21CGimmick100Acre02CollP5CTask\n__21CGimmick100Acre02CollP5CTask = 0\n"
    ".weak __21CGimmick100AcreBBWallP5CTask\n__21CGimmick100AcreBBWallP5CTask = 0\n"
);











CEntity* CEntityFactory::createGimmick(INT inRsrcID, CTask* inParent, void* inExtraInfo) {
    CTask* parent;
    CEntity* gimmick;

    if (inParent == NULL) {
        parent = CTaskMan::getSysTask(17);
        if (parent == NULL) parent = this;
    } else {
        parent = inParent;
    }

    switch (inRsrcID) {
    case 0x2: case 0x5: case 0xc4 ... 0xc5: case 0x128 ... 0x129: case 0x18c ... 0x18d: case 0x1e1 ... 0x1e2: case 0x254 ... 0x255: case 0x2b8 ... 0x2b9: case 0x31c ... 0x31d: case 0x380 ... 0x381: case 0x3a1 ... 0x3a2: case 0x3e4 ... 0x3e5: case 0x4ac ... 0x4ad: case 0x510 ... 0x511: case 0x52c ... 0x52d: case 0x574 ... 0x575:
        gimmick = new (__FILE__ "(138)") CGimmickTreasureBox(parent);
        break;
    case 0x448:
        gimmick = new (__FILE__ "(143)") CGimmickTreasureBoxPooh(parent);
        break;
    case 0x139: case 0x203: case 0x206 ... 0x20a: case 0x2328:
        gimmick = new (__FILE__ "(157)") CGimmickRideOn(parent);
        break;
    case 0x13a:
        gimmick = new (__FILE__ "(161)") CGimmickAngelArrow(parent);
        break;
    case 0x86:
        gimmick = new (__FILE__ "(169)") CGimmickBreakableTu(parent);
        break;
    case 0x87:
        gimmick = new (__FILE__ "(172)") CGimmickCarryTu(parent);
        break;
    case 0x72:
        gimmick = new (__FILE__ "(175)") CGimmickShadowTu(parent);
        break;
    case 0xfa:
        gimmick = new (__FILE__ "(183)") CGimmickBJFStand1(parent);
        break;
    case 0xfb:
        gimmick = new (__FILE__ "(186)") CGimmickBJFStand2(parent);
        break;
    case 0xfc:
        gimmick = new (__FILE__ "(189)") CGimmickBJFStand3(parent);
        break;
    case 0xfd:
        gimmick = new (__FILE__ "(192)") CGimmickBJFStand4(parent);
        break;
    case 0xfe:
        gimmick = new (__FILE__ "(195)") CGimmickBJFStand5(parent);
        break;
    case 0xff:
        gimmick = new (__FILE__ "(198)") CGimmickBJFStand6(parent);
        break;
    case 0x28a:
        gimmick = new (__FILE__ "(205)") CGimmickBBGKnife(parent);
        break;
    case 0x28b:
        gimmick = new (__FILE__ "(208)") CGimmickBBGSaw(parent);
        break;
    case 0x28c:
        gimmick = new (__FILE__ "(211)") CGimmickBBGRecovery(parent);
        break;
    case 0x28d:
        gimmick = new (__FILE__ "(214)") CGimmickBBGHedge(parent);
        break;
    case 0x4e2:
        gimmick = new (__FILE__ "(221)") CGimmickBDSStand1(parent);
        break;
    case 0x4e3:
        gimmick = new (__FILE__ "(224)") CGimmickBDSStand2(parent);
        break;
    case 0x4e4:
        gimmick = new (__FILE__ "(227)") CGimmickBDSStand3(parent);
        break;
    case 0x4e5:
        gimmick = new (__FILE__ "(230)") CGimmickBDSStand4(parent);
        break;
    case 0x4e6:
        gimmick = new (__FILE__ "(233)") CGimmickBDSStand5(parent);
        break;
    case 0x4e7:
        gimmick = new (__FILE__ "(236)") CGimmickBDSStand6(parent);
        break;
    case 0x1f2:
        gimmick = new (__FILE__ "(243)") CGimmickBTMChair(parent);
        break;
    case 0x1f3:
        gimmick = new (__FILE__ "(246)") CGimmickBTMTable(parent);
        break;
    case 0x226 ... 0x227:
        gimmick = new (__FILE__ "(254)") CGimmickIsLand(parent, inRsrcID);
        break;
    case 0x22b:
        gimmick = new (__FILE__ "(258)") CGimmickBossIsLand(parent);
        break;
    case 0x228 ... 0x229:
        gimmick = new (__FILE__ "(263)") CGimmickAcid(parent);
        break;
    case 0x22a:
        gimmick = new (__FILE__ "(267)") CGimmickPinokio(parent);
        break;
    case 0x352:
        gimmick = new (__FILE__ "(275)") CGimmickShip(parent);
        break;
    case 0x353:
        gimmick = new (__FILE__ "(279)") CGimmickBottle(parent);
        break;
    case 0x356:
        gimmick = new (__FILE__ "(283)") CGimmickBombBarrel(parent);
        break;
    case 0x3b6:
        gimmick = new (__FILE__ "(290)") CGimmickMDFooting(parent);
        break;
    case 0x68 ... 0x6f: case 0xa0 ... 0xa4: case 0xd2: case 0x19c ... 0x19d: case 0x1a7 ... 0x1a8: case 0x202: case 0x20e: case 0x25c ... 0x25e: case 0x265 ... 0x268: case 0x26a ... 0x270: case 0x272: case 0x28f ... 0x292: case 0x327 ... 0x328: case 0x32b: case 0x35b: case 0x389: case 0x38b: case 0x394 ... 0x39d: case 0x3f5: case 0x429 ... 0x42a: case 0x43d ... 0x43e: case 0x450 ... 0x452: case 0x458 ... 0x45a: case 0x45c: case 0x47f ... 0x481: case 0x485 ... 0x487: case 0x489: case 0x4ba ... 0x4bc: case 0x4be ... 0x4c0: case 0x4c5 ... 0x4c7: case 0x51d: case 0x528:
        gimmick = new (__FILE__ "(383)") CGimmickHitReturn(parent, inRsrcID);
        break;
    case 0x13c ... 0x13e:
        gimmick = new (__FILE__ "(392)") CGimmickDominoPillar(parent, inRsrcID);
        break;
    case 0x38e ... 0x38f:
        gimmick = new (__FILE__ "(400)") CGimmickUpFloorSwitch(parent, inRsrcID);
        break;
    case 0x38c ... 0x38d: case 0x390:
        gimmick = new (__FILE__ "(405)") CGimmickUpFloor(parent, inRsrcID);
        break;
    case 0x260: case 0x273:
        gimmick = new (__FILE__ "(413)") CGimmickGuillotine(parent);
        break;
    case 0x2c3: case 0x2cc:
        gimmick = new (__FILE__ "(422)") CGimmickJackBox(parent);
        break;
    case 0xd3 ... 0xdf: case 0x1ff ... 0x201: case 0x453 ... 0x455: case 0x482 ... 0x484: case 0x4c2 ... 0x4c3: case 0x4c8 ... 0x4c9:
        gimmick = new (__FILE__ "(456)") CGimmickRideDest(parent);
        break;
    case 0x261 ... 0x263:
        gimmick = new (__FILE__ "(465)") CGimmickFallTomb(parent);
        break;
    case 0x269:
        gimmick = new (__FILE__ "(472)") CGimmickFountain(parent);
        break;
    case 0x20b ... 0x20d: case 0x271: case 0x2c4: case 0x2c6: case 0x2cd:
        gimmick = new (__FILE__ "(485)") CGimmickTimeBomb(parent);
        break;
    case 0x329 ... 0x32a: case 0x35c ... 0x35d:
        gimmick = new (__FILE__ "(495)") CGimmickToldPipe(parent);
        break;
    case 0x13b:
        gimmick = new (__FILE__ "(502)") CGimmickStatue(parent);
        break;
    case 0x393: case 0x3a0:
        gimmick = new (__FILE__ "(510)") CGimmickThunderTrap(parent);
        break;
    case 0x392: case 0x39f:
        gimmick = new (__FILE__ "(514)") CGimmickThunderLaser(parent);
        break;
    case 0x391:
        gimmick = new (__FILE__ "(521)") CGimmickSteamTrap(parent);
        break;
    case 0x3: case 0x82: case 0xe6: case 0x14a: case 0x212: case 0x33e: case 0x35e: case 0x46a: case 0x48a: case 0x4c1: case 0x4ce:
        gimmick = new (__FILE__ "(539)") CGimmickDeceive(parent);
        break;
    case 0x4: case 0x6 ... 0xe: case 0x83: case 0xe7: case 0x14b: case 0x213: case 0x33f: case 0x362: case 0x46b: case 0x48b: case 0x4cf: case 0x2332:
        gimmick = new (__FILE__ "(564)") CGimmickDeceiveEnemy(parent, inRsrcID);
        break;
    case 0x88 ... 0x8a: case 0xea ... 0xec: case 0x13f: case 0x14e ... 0x14f: case 0x216 ... 0x218: case 0x25f: case 0x2ce ... 0x2d0: case 0x2de ... 0x2e0: case 0x342 ... 0x344: case 0x35f ... 0x361: case 0x46e ... 0x470: case 0x48d ... 0x48f: case 0x4d2 ... 0x4d4:
        gimmick = new (__FILE__ "(607)") CGimmickCarry(parent);
        break;
    case 0x84 ... 0x85: case 0xe2: case 0xe9: case 0x137: case 0x14d: case 0x215: case 0x2c1 ... 0x2c2: case 0x2ca ... 0x2cb: case 0x325 ... 0x326: case 0x359 ... 0x35a: case 0x426 ... 0x428: case 0x42c ... 0x42e: case 0x431 ... 0x434: case 0x440: case 0x45b: case 0x46c: case 0x488: case 0x48c: case 0x4bd: case 0x51e: case 0x529: case 0x2329:
        gimmick = new (__FILE__ "(647)") CGimmickBreakable(parent);
        break;
    case 0x136: case 0x2c7: case 0x38a: case 0x232a:
        gimmick = new (__FILE__ "(665)") CGimmickHitRepetition(parent, inRsrcID);
        break;
    case 0x19a ... 0x19b: case 0x1a4 ... 0x1a6: case 0x2c5: case 0x2c9: case 0x324: case 0x358: case 0x388: case 0x42b: case 0x43f: case 0x51a: case 0x51c: case 0x51f ... 0x520: case 0x525: case 0x527: case 0x52a ... 0x52b: case 0x232c:
        gimmick = new (__FILE__ "(692)") CGimmickOnceReaction(parent, inRsrcID);
        break;
    case 0x67: case 0xcb: case 0x12f: case 0x193: case 0x1f7: case 0x25b: case 0x2bf: case 0x323: case 0x357: case 0x387: case 0x39e: case 0x44f: case 0x47e: case 0x4b3: case 0x517: case 0x522:
        gimmick = new (__FILE__ "(714)") CGimmickJumpStand(parent, inRsrcID);
        break;
    case 0x19e:
        gimmick = new (__FILE__ "(721)") CGimmickPlantParent(parent);
        break;
    case 0x19f:
        gimmick = new (__FILE__ "(724)") CGimmickPlantChild(parent);
        break;
    case 0x1a0:
        gimmick = new (__FILE__ "(731)") CGimmickPlantBullet(parent);
        break;
    case 0x1a1:
        gimmick = new (__FILE__ "(738)") CGimmickKLotusLeaf(parent);
        break;
    case 0x51b: case 0x526:
        gimmick = new (__FILE__ "(746)") CGimmickFlowerLump(parent);
        break;
    case 0x519: case 0x524:
        gimmick = new (__FILE__ "(754)") CGimmickSearchLaser(parent);
        break;
    case 0x521:
        gimmick = new (__FILE__ "(757)") CGimmickSearchLaserCircle(parent);
        break;
    case 0x204 ... 0x205:
        gimmick = new (__FILE__ "(765)") CGimmickWallHole(parent);
        break;
    case 0x518: case 0x523:
        gimmick = new (__FILE__ "(773)") CGimmickBell(parent);
        break;
    case 0x232d:
        gimmick = new (__FILE__ "(781)") CGimmickHitMore(parent);
        break;
    case 0x14:
        gimmick = new (__FILE__ "(785)") CGimmickSavePoint(parent);
        break;
    case 0x15:
        gimmick = new (__FILE__ "(789)") CGimmickWarpPoint(parent);
        break;
    case 0xe0: case 0x4c4:
        gimmick = new (__FILE__ "(794)") CGimmickWindowLid(parent);
        break;
    case 0x423:
        gimmick = new (__FILE__ "(798)") CGimmick100Acre01Bush(parent);
        break;
    case 0x424:
        gimmick = new (__FILE__ "(801)") CGimmickChasePoo(parent);
        break;
    case 0x435:
        gimmick = new (__FILE__ "(804)") CGimmick100Acre02Veg(parent);
        break;
    case 0x442 ... 0x443:
        gimmick = new (__FILE__ "(808)") CGimmick100Acre02Coll(parent);
        break;
    case 0x444:
        gimmick = new (__FILE__ "(811)") CGimmick100AcreBBWall(parent);
        break;
    default:
        gimmick = new (__FILE__ "(815)") CSampleGimmick(parent);
        break;
    }

    if (gimmick != NULL && inExtraInfo != NULL) {
        ((CBaseGimmick*)gimmick)->setUniqueID(*(INT*)inExtraInfo);
    }
    return gimmick;
}
// SOURCE DATA: .data .rodata
