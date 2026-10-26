#define TASK_H_NO_SLC_OVERFLOW_MSG
#define CAMERAMAN_H_PATH "c:/KHCOM/MainApp/Game/Atel/CameraMan.h"
#define CHATMAN_H_PATH "c:/KHCOM/MainApp/Game/Atel/ChatMan.h"
#define ATELFOG_H_PATH "c:/KHCOM/MainApp/Game/Atel/AtelFog.h"
#define ATELCROSSFADE_H_PATH "c:/KHCOM/MainApp/Game/Atel/AtelCrossFade.h"
#define ATELBLUR_H_PATH "c:/KHCOM/MainApp/Game/Atel/AtelBlur.h"
#define GAME_ATEL_AVATAR_CPP /* the full CAvatar class (the other includers get a stub) */
#include "khtypes.h"
#include "System/Debug.h"
#include "System/HeapMan.h"
inline void* operator new(size_t, void* p) throw() { return p; }
/* PARTIAL: the original System/HeapMan.h(134) has it inline */
inline void* operator new(size_t size) throw() { return com_malloc(size); }
/* PARTIAL: the original System/HeapMan.h(150-153) has the debug allocation inline */
#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
inline void* operator new(size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}
/* .rodata starts with the strings of the included headers' inline functions, in this order. */
#include "System/Graphic/Gprim.h"
#include "Game/Atel/Avatar.h"
#include "Game/Collision/CollUtil.h"
#include "Game/Atel/CameraMan.h"
#include "Game/Atel/ChatMan.h"
#include "System/Ctd/CtdMan.h"
inline void _taskSLCOverflowMsg() {
    TRACE("\x83\x5e\x83\x58\x83\x4e\x97\x70\x83\x52\x83\x93\x83\x65\x83\x69\x82\xaa MAX \x82\xc9\x93\x9e\x92\x42\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
}
#include "System/Graphic/Effect/effect.h"
#include "Game/Atel/AtelFog.h"
#include "Game/Atel/AtelCrossFade.h"
#include "Game/Atel/AtelBlur.h"
#include "Game/Card.h"
#include "Game/Technic.h"
#include "Game/Enemy/BattleEnemyManager.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/Status.h"
/* file-local statics of an original header (every includer has its own copy in .data) */
namespace MoogleShop {
static CHAR* MoogleShopLad = "m_shop.lad";
static CHAR* MSHOP_SEQ_UNIT0 = "m_shop:00";
static CHAR* MSHOP_SEQ_UNIT1 = "m_shop:01";
static CHAR* MoogleShopFloor = "MogFloor";
static CHAR* MoogleShopKaku = "MogCardKaku";
static CHAR* MoogleShopPack = "MogCardPack";
}
#include "Game/CardBattle.h"
#include "Form/Menu/SaveLoadCtrl.h"
#include "Game/Enemy/HumanBoss.h"
#include "System/Graphic/Anim.h"
/* Placeholder: .rodata has "e123_062" after Anim.h's strings (from an unidentified header, as in Game/Enemy/CreateEnemy). */
inline void _vsmeMotionName() {
    TRACE("e123_062");
}
#include "System/Graphic/Str.h"
#include "Game/Atel/VSME.h"
#include "Game/Atel/Atel.h"
#include "Game/Atel/EventTrap.h"
#include "Form/Menu/WorldCard.h"
#include "Form/Menu/LevelUp.h"
#include "System/Rcf.h"
#include "Game/GameWork.h"
#include "Game/SceneInfo.h"
#include "System/TaskMan.h"
#include "Form/DebugWindow.h"
#include "Game/Debug/DebugWork.h"
#include "Game/Player/Player.h"
#include "System/Sound/SoundMan.h"
#include "System/Com.h"
#include "Game/SceneEnv.h"
#include "Form/Menu/JMemoFlag.h"
#include "System/Component.h"
#include "System/Graphic/WorldEnv.h"
#define SYSTEM_GRAPHIC_DEFOCUS_CPP /* the complete DeFocus classes (other includers get partial ones) */
#include "System/Graphic/DeFocus.h"
#include "System/PadIO.h"
#include "Game/Enemy/MapEnemyManager.h"
#include "Game/EntityFactory.h"
#include "System/Graphic/ShadowModel.h"
#include "System/PlaySpeedCtrl.h"
#include "Game/Atel/AtelTutorial.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/Gimmick/BaseGimmick.h"
#include "System/Graphic/Glare.h"
#include "Game/Game.h"
#include "Game/Player/PlayerManager.h"
#include "Game/CardKind.h"
#include <string.h>
#include "include_asm.h"

typedef char* va_list;
#define va_start(ap, last)                                    \
    (ap = ((va_list)__builtin_next_arg(last)                  \
           - (__builtin_args_info(2) >= 8 ? 0                 \
              : (8 - __builtin_args_info(2)) * 8)))
#define va_end(ap) ((void)0)
extern "C" {
int sprintf(char* buf, const char* fmt, ...);
int printf(const char* fmt, ...);
int vprintf(const char* fmt, va_list ap);
int vsprintf(char* buf, const char* fmt, va_list ap);
}

using namespace ATEL;

static void dispActorTag(INT nMax, const Actor* pTbl);

static inline void _setRTP(RTP& r, USHORT lv, USHORT actor, SHORT tag, SHORT mactor) {
    r.rq_lv = lv;
    r.rq_actor = actor;
    r.rq_tag = tag;
    r.rq_m_actor = mactor;
}
namespace Gauge {
class CPrizeGauge { /* PARTIAL: Game/Player/PlayerGauge.h declares the class only for its own unit */
public:
    static BOOL isNowDispCard();
};
}

namespace DWTEvent {
static BOOL DebugSetNpcIDWindow(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x20) {
        DebugWork::g_dw.event.iNpcID = inY;
        CActorDebug* p;
        if (DebugWork::g_dw.event.pClass) {
            p = (CActorDebug*)DebugWork::g_dw.event.pClass;
            delete p;
        }
        p = new CActorDebug(CTaskMan::getSysTask(1));
        DebugWork::g_dw.event.pClass = (INT*)p;
        DebugWork::g_dw.event.iNpcCoordDisp = 1;
    }
    return 1;
}
}
/* the asm .data (debug window tables) refers to it by name until the data comes from source */
__asm__(".globl DebugSetNpcIDWindow__8DWTEventP12CDebugWindowii");
namespace DWTEvent {
static INT debugWinNPCListAllCall(CDebugWindow* pObj, INT inParam) {
    CTask* pA;
    INT ret;
    if (inParam & 1) {
        pA = CTaskMan::getSysTask(0x1A);
        ret = 0;
        if (pA && ((CAtel*)pA)->getVSM()) ret = ((CAtel*)pA)->getVSM()->getActorNum();
        return ret;
    }
    return 1;
}
}
/* the asm .data (debugWinNPCList table) refers to it by name until the data comes from source */
__asm__(".globl debugWinNPCListAllCall__8DWTEventP12CDebugWindowi");
/* placeholder: rodata of the tables before debugWinNPCListDispCall__8DWTEventPciPCv */
__asm__(".rdata\n");
inline void _roPh0() {
    TRACE("");
    TRACE("\x83\\\x83\x89");
    TRACE("\x83\x8a\x83N");
    TRACE("\x83r\x83M\x83i\x81[");
    TRACE("\x83X\x83^\x83\x93\x83_\x81[\x83h");
    TRACE("\x83v\x83\x89\x83" "E\x83h");
    TRACE("\x83G\x83N\x83X\x83g\x83\x8a\x81[\x83\x80");
    TRACE("\x83X\x83" "e\x83\x8c\x83I");
    TRACE("\x83v\x83\x8d\x83\x8d\x83W\x83" "b\x83N\x87U");
    TRACE("\x83\x82\x83m\x83\x89\x83\x8b");
    TRACE("NotOpen");
    TRACE("Camp");
    TRACE("RoomCreation_1");
    TRACE("RoomCreation_2");
    TRACE("RoomCreation_EV");
    TRACE("WorldSelect");
    TRACE("MogShop");
    TRACE("SaveMenu");
    TRACE("\x82\xb5\x82\xe5\x82\xab\x82\xb6\x82\xe5\x82\xa4\x82\xbd\x82\xa2");
    TRACE("\x82P\x89\xf1\x96\xda\x82\xbf\x82\xe5\x82\xa4\x82\xb9\x82\xf1");
    TRACE("\x82P\x89\xf1\x96\xda\x83N\x83\x8a\x83" "A\x82\xb5\x82\xbd");
    TRACE("\x82Q\x89\xf1\x96\xda\x82\xbf\x82\xe5\x82\xa4\x82\xb9\x82\xf1");
    TRACE("\x82Q\x89\xf1\x96\xda\x83N\x83\x8a\x83" "A\x82\xb5\x82\xbd");
    TRACE("\x8d" "D\x82\xab\x82\xc9\x97V\x82\xd7");
    TRACE("\x83Q\x81[\x83\x80\x8aJ\x8en");
    TRACE("\x83Q\x81[\x83\x80\x92\x86");
    TRACE("\x83Q\x81[\x83\x80\x8fI\x97\xb9");
    TRACE("\x96\xec\x8d\xd8\x83p\x83j\x83" "b\x83N");
    TRACE("\x83o\x83\x8b\x81[\x83\x93\x83O\x83\x89\x83" "C\x83_\x81[");
    TRACE("\x83" "e\x83" "B\x83K\x81[\x83W\x83\x83\x83\x93\x83v");
    TRACE("\x83\x8f\x83" "C\x83\x8b\x83h\x83_\x83" "C\x83u");
    TRACE("\x82\xcd\x82\xbf\x91\xde\x8e\xa1");
    TRACE("-----\x82O");
    TRACE("-----\x82P");
    TRACE("-----\x82Q");
    TRACE("-----\x82R");
    TRACE("\x83X\x83\x89\x83" "C\x83h\x83_\x83" "b\x83V\x83\x85");
    TRACE("\x83t\x83@\x83" "C\x83i\x83\x8b\x83u\x83\x8c\x83" "C\x83N");
    TRACE("\x83X\x83^\x83\x93\x83" "C\x83\x93\x83p\x83N\x83g");
    TRACE("\x83U\x83\x93\x83" "e\x83" "c\x83P\x83\x93");
    TRACE("\x83X\x83g\x83\x89\x83" "C\x83N\x83\x8c\x83" "C\x83h");
    TRACE("\x83\\\x83j\x83" "b\x83N\x83\x8c\x83" "C\x83\x94");
    TRACE("\x83\x89\x83X\x83g\x83" "A\x83\x8b\x83J\x83i\x83\x80");
    TRACE("\x83\x89\x83O\x83i\x83\x8d\x83N");
    TRACE("\x83\x8a\x83j\x83" "e\x83" "B\x83\x8a\x83~\x83" "b\x83g");
    TRACE("\x83t\x83@\x83" "C\x83\x89");
    TRACE("\x83u\x83\x8a\x83U\x83\x89");
    TRACE("\x83T\x83\x93\x83_\x83\x89");
    TRACE("\x83P\x83" "A\x83\x8b\x83\x89");
    TRACE("\x83O\x83\x89\x83r\x83\x89");
    TRACE("\x83X\x83g\x83v\x83\x89");
    TRACE("\x83G\x83" "A\x83\x8d\x83\x89");
    TRACE("\x83t\x83@\x83" "C\x83K");
    TRACE("\x83u\x83\x8a\x83U\x83K");
    TRACE("\x83T\x83\x93\x83_\x83K");
    TRACE("\x83P\x83" "A\x83\x8b\x83K");
    TRACE("\x83O\x83\x89\x83r\x83K");
    TRACE("\x83X\x83g\x83v\x83K");
    TRACE("\x83G\x83" "A\x83\x8d\x83K");
    TRACE("\x83t\x83@\x83" "C\x83" "A\x83\x8c\x83" "C\x83h");
    TRACE("\x83u\x83\x8a\x83U\x83h\x83\x8c\x83" "C\x83h");
    TRACE("\x83T\x83\x93\x83_\x81[\x83\x8c\x83" "C\x83h");
    TRACE("\x83\x8a\x83t\x83\x8c\x83N\x83g\x83\x8c\x83" "C\x83h");
    TRACE("\x83W\x83\x83\x83" "b\x83W\x83\x81\x83\x93\x83g");
    TRACE("\x83t\x83@\x83" "C\x83K\x83o\x81[\x83X\x83g");
    TRACE("\x83\x8c\x83" "C\x83W\x83\x93\x83O\x83X\x83g\x81[\x83\x80");
    TRACE("\x83\x81\x83K\x83t\x83\x8c\x83" "A");
    TRACE("\x83t\x83\x8a\x81[\x83Y");
    TRACE("\x83z\x81[\x83~\x83\x93\x83O\x83u\x83\x8a\x83U\x83\x89\t");
    TRACE("\x83" "A\x83N\x83" "A\x83X\x83v\x83\x89\x83" "b\x83V\x83\x85");
    TRACE("\x83}\x83O\x83l\x83X\x83p\x83" "C\x83\x89\x83\x8b");
    TRACE("\x83\x8a\x81[\x83T\x83\x8b\x83t\x83\x8c\x81[\x83\x80");
    TRACE("\x83V\x83\x87\x83" "b\x83N\x83" "C\x83\x93\x83p\x83N\x83g");
    TRACE("\x83g\x83\x8b\x83l\x83h");
    TRACE("\x83N\x83G\x83" "C\x83N");
    TRACE("\x83" "f\x83W\x83\x87\x83l\x81[\x83^\x81[");
    TRACE("\x83" "f\x83W\x83\x87\x83\x93");
    TRACE("\x83o\x83" "C\x83\x93\x83h");
    TRACE("\x83R\x83\x93\x83t\x83\x85");
    TRACE("\x83" "e\x83\x89\x81[");
    TRACE("\x83V\x83\x93\x83N\x83\x8d");
    TRACE("\x83~\x83\x89\x83N\x83\x8b\x83M\x83t\x83g");
    TRACE("\x83" "e\x83\x8c\x83|");
    TRACE("\x83z\x81[\x83\x8a\x81[");
    TRACE("\x83v\x83\x89\x83" "E\x83h\x83\x8d\x83" "A");
    TRACE("\x83X\x83v\x83\x89\x83" "b\x83V\x83\x85");
    TRACE("\x83p\x83\x89\x83_\x83" "C\x83X");
    TRACE("\x83p\x83\x89\x83_\x83" "C\x83X\x83X\x83^\x83\x93\x83v");
    TRACE("\x83t\x83\x8c\x83" "A\x83u\x83\x8c\x83X");
    TRACE("\x83V\x83\x87\x81[\x83^\x83" "C\x83\x80");
    TRACE("\x83" "c\x83" "C\x83\x93\x83N\x83\x8b");
    TRACE("\x8b\xa5\x8e" "a\x82\xe8");
    TRACE("\x92\xb4\x8b\x86\x95\x90\x90_\x94" "e\x8e" "a");
    TRACE("\x8b\xa5\x8e" "a\x82\xe8\x81" "E\x89\xfc");
    TRACE("\x83}\x83W\x83" "b\x83N");
    TRACE("\x83X\x83^\x81[\x83_\x83X\x83g\x83u\x83\x8a\x83" "b\x83" "c");
    TRACE("\x83O\x81[\x83t\x83" "B\x81[\x83g\x83\x8b\x83l\x83h");
    TRACE("\x83O\x81[\x83t\x83" "B\x81[\x83X\x83}\x83" "b\x83V\x83\x85");
    TRACE("\x83\x8f\x83" "C\x83\x8b\x83h\x83N\x83\x89\x83" "b\x83V\x83\x83\x81[");
    TRACE("\x83X\x83g\x81[\x83\x80\x83\x8a\x81[\x83p\x81[");
    TRACE("\x83T\x83v\x83\x89\x83" "C\x83Y");
    TRACE("\x83X\x83p\x83" "C\x83\x89\x83\x8b\x83" "E\x83" "F\x83" "C\x83u");
    TRACE("\x83X\x83\x89\x83X\x83g\x83\x89\x83" "b\x83V\x83\x85");
    TRACE("\x83t\x83\x85\x83\x8a\x83" "A\x83X\x83{\x83\x8c\x81[");
    TRACE("\x83\x89\x83" "b\x83L\x81[\x83o\x83" "E\x83\x93\x83" "e\x83" "B");
}
namespace DWTEvent {
static INT debugWinNPCListDispCall(LPSTR pStr, INT iLineY, const void* pValue) {
    CTask* pA = CTaskMan::getSysTask(0x1A);
    if (pA && ((CAtel*)pA)->getVSM()) sprintf(pStr, " %s", ((CAtel*)pA)->getVSM()->getAvatarName(iLineY));
    return 1;
}
}
/* the asm .data (debugWinNPCList table) refers to it by name until the data comes from source */
__asm__(".globl debugWinNPCListDispCall__8DWTEventPciPCv");
namespace DWTEvent {
static BOOL DebugCreateNpcWindow(CDebugWindow* pObj, INT inY, INT inParam) {
    CTask* pA = CTaskMan::getSysTask(0x1A);
    if (pA && ((CAtel*)pA)->getVSM()) {
        if (inParam & 0x180) {
            if (DebugWork::g_dw.event.iNpcCoordDisp == 1) {
                CActorDebug* p = new CActorDebug(CTaskMan::getSysTask(1));
                DebugWork::g_dw.event.pClass = (INT*)p;
            }
        }
    }
    return 1;
}
}
/* the asm .data (debug window tables) refers to it by name until the data comes from source */
__asm__(".globl DebugCreateNpcWindow__8DWTEventP12CDebugWindowii");
namespace DWTEvent {
static BOOL DebugCreateEventLineDisp(CDebugWindow* pObj, INT inY, INT inParam) {
    CTask* pA = CTaskMan::getSysTask(0x1A);
    if (pA && ((CAtel*)pA)->getVSM()) {
        if (inParam & 0x180) {
            if (DebugWork::g_dw.event.iEventline == 1) {
                CEventLineDisp* p = new CEventLineDisp(CTaskMan::getSysTask(1));
                DebugWork::g_dw.event.pEventlineClass = (INT*)p;
            } else {
                if (DebugWork::g_dw.event.pEventlineClass) {
                    ((CTask*)DebugWork::g_dw.event.pEventlineClass)->out();
                }
                DebugWork::g_dw.event.pEventlineClass = NULL;
            }
        }
    }
    return 1;
}
}
/* the asm .data (debug window tables) refers to it by name until the data comes from source */
__asm__(".globl DebugCreateEventLineDisp__8DWTEventP12CDebugWindowii");
namespace DWTEvent {
static INT DebugAtelWorkDef(CDebugWindow* pObj, INT inParam) {
    DebugWork::g_dw.atelwork.iWorldIntro = CGameWork::getInt(0, 0xC1);
    DebugWork::g_dw.atelwork.iTutorialEnd = CGameWork::getInt(0, 0xC2);
    DebugWork::g_dw.atelwork.iPossibleCard = CGameWork::getInt(0, 0x21);
    DebugWork::g_dw.atelwork.iPossibleFriend = CGameWork::getInt(0, 0x22);
    DebugWork::g_dw.atelwork.iPoohMiniGameIntro = CGameWork::getInt(0, 0x23);
    DebugWork::g_dw.atelwork.iPcStanderdSkill = CGameWork::getInt(0, 0x24);
    DebugWork::g_dw.atelwork.iEnSoraEventStep = CGameWork::getInt(0, 0x25);
    DebugWork::g_dw.atelwork.iEnRikuEventStep = CGameWork::getInt(0, 0x26);
    DebugWork::g_dw.atelwork.iSoraJiminyStep = CGameWork::getInt(0, 0x33);
    DebugWork::g_dw.atelwork.iRikuJiminyStep = CGameWork::getInt(0, 0x34);
    DebugWork::g_dw.atelwork.iKH2CardGet = CGameWork::getInt(0, 0x2D);
    DebugWork::g_dw.atelwork.iPoohHiScoreOwl = CGameWork::getInt(0, 0x2E);
    DebugWork::g_dw.atelwork.iPoohHiScoreRoo = CGameWork::getInt(0, 0x2F);
    DebugWork::g_dw.atelwork.iPoohHiScoreEeyore = CGameWork::getInt(0, 0x30);
    DebugWork::g_dw.atelwork.iPoohHiScoreTigger = CGameWork::getInt(0, 0x31);
    DebugWork::g_dw.atelwork.iPoohHiScoreRabbit = CGameWork::getInt(0, 0x32);
    DebugWork::g_dw.atelwork.iDistinctionClear = CGameWork::getFlag(0, 0x83);
    DebugWork::g_dw.atelwork.iPlayMode = CGameWork::getByte(0, 0x60);
    DebugWork::g_dw.atelwork.iPlayDifficultMode = CGameWork::getByte(0, 0x61);
    DebugWork::g_dw.atelwork.iSoundMode = CGameWork::getByte(0, 0x62);
    DebugWork::g_dw.atelwork.iVibrat = CGameWork::getByte(0, 0x63);
    DebugWork::g_dw.atelwork.iPoohGamePiglet = CGameWork::getByte(0, 0x3);
    DebugWork::g_dw.atelwork.iPoohGameOwl = CGameWork::getByte(0, 0x4);
    DebugWork::g_dw.atelwork.iPoohGameRoo = CGameWork::getByte(0, 0x5);
    DebugWork::g_dw.atelwork.iPoohGameEeyore = CGameWork::getByte(0, 0x6);
    DebugWork::g_dw.atelwork.iPoohGameTigger = CGameWork::getByte(0, 0x7);
    DebugWork::g_dw.atelwork.iPoohGameRabbit = CGameWork::getByte(0, 0x8);
    DebugWork::g_dw.atelwork.iPoohClearIntoroPiglet = CGameWork::getFlag(0, 0xA);
    DebugWork::g_dw.atelwork.iPoohClearIntoroOwl = CGameWork::getFlag(0, 0xB);
    DebugWork::g_dw.atelwork.iPoohClearIntoroRoo = CGameWork::getFlag(0, 0xC);
    DebugWork::g_dw.atelwork.iPoohClearIntoroEeyore = CGameWork::getFlag(0, 0xD);
    DebugWork::g_dw.atelwork.iPoohClearIntoroTigger = CGameWork::getFlag(0, 0xE);
    DebugWork::g_dw.atelwork.iPoohClearIntoroRabbit = CGameWork::getFlag(0, 0xF);
    DebugWork::g_dw.atelwork.iPoohClearIntoroResetPiglet = CGameWork::getFlag(0, 0x13);
    DebugWork::g_dw.atelwork.iPoohClearIntoroResetOwl = CGameWork::getFlag(0, 0x14);
    DebugWork::g_dw.atelwork.iPoohClearIntoroResetRoo = CGameWork::getFlag(0, 0x15);
    DebugWork::g_dw.atelwork.iPoohClearIntoroResetEeyore = CGameWork::getFlag(0, 0x16);
    DebugWork::g_dw.atelwork.iPoohClearIntoroResetTigger = CGameWork::getFlag(0, 0x17);
    DebugWork::g_dw.atelwork.iPoohClearIntoroRabbit = CGameWork::getFlag(0, 0x18);
    DebugWork::g_dw.atelwork.iGameKH2clear = CGameWork::getFlag(0, 0x11);
    DebugWork::g_dw.atelwork.iMahluciaKilled = CGameWork::getFlag(0, 0x82);
    DebugWork::g_dw.atelwork.iRikuClear = CGameWork::getFlag(0, 0x12);
    DebugWork::g_dw.atelwork.iSoraClear = CGameWork::getFlag(0, 0x9);
    DebugWork::g_dw.atelwork.iSoraMovie0 = CGameWork::getInt(0, 0x27);
    DebugWork::g_dw.atelwork.iSoraMovie1 = CGameWork::getInt(0, 0x28);
    DebugWork::g_dw.atelwork.iSoraMovie2 = CGameWork::getInt(0, 0x29);
    DebugWork::g_dw.atelwork.iRikuMovie0 = CGameWork::getInt(0, 0x2A);
    DebugWork::g_dw.atelwork.iRikuMovie1 = CGameWork::getInt(0, 0x2B);
    DebugWork::g_dw.atelwork.iRikuMovie2 = CGameWork::getInt(0, 0x2C);
    DebugWork::g_dw.atelwork.iBossEventStep = CGameWork::getInt(1, 0xE1);
    DebugWork::g_dw.atelwork.iCockpitDisp = CGameWork::getInt(1, 0xE2);
    DebugWork::g_dw.atelwork.iKindMenu = CGameWork::getByte(1, 0x140);
    DebugWork::g_dw.atelwork.iPoohMinigameState = CGameWork::getByte(1, 0x141);
    DebugWork::g_dw.atelwork.iPoohMinigameKind = CGameWork::getByte(1, 0x142);
    DebugWork::g_dw.atelwork.iPlayerDispOff = CGameWork::getFlag(1, 0x0);
    DebugWork::g_dw.atelwork.iBossKilled = CGameWork::getFlag(1, 0x2);
    DebugWork::g_dw.atelwork.iNotEventSkip = CGameWork::getFlag(1, 0x3);
    DebugWork::g_dw.atelwork.iIsAtelEvent = CGameWork::getFlag(1, 0x4);
    DebugWork::g_dw.atelwork.iNotPcControl = CGameWork::getFlag(1, 0x5);
    DebugWork::g_dw.atelwork.iNotPause = CGameWork::getFlag(1, 0x6);
    DebugWork::g_dw.atelwork.iIsTutorial = CGameWork::getFlag(1, 0x7);
    DebugWork::g_dw.atelwork.iNotRoomJump = CGameWork::getFlag(1, 0x20);
    DebugWork::g_dw.atelwork.iNotRoomCreation = CGameWork::getFlag(1, 0x24);
    DebugWork::g_dw.atelwork.iNotHartBtl = CGameWork::getFlag(1, 0x25);
    DebugWork::g_dw.atelwork.iIsBtlTutorial = CGameWork::getFlag(1, 0x2C);
    DebugWork::g_dw.atelwork.iIsWorldIntro = CGameWork::getFlag(1, 0x37);
    return 1;
}
}
/* placeholder: rodata of the tables before debugBtlReportCallBackParam__8DWTEventP12CDebugWindowii */
__asm__(".rdata\n");
inline void _roPh1() {
    TRACE("ACTOR\x83\x8a\x83X\x83g");
}
namespace DWTEvent {
static BOOL debugBtlReportCallBackParam(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        switch (inY) {
        case 6: CGameWork::setInt(0, 0x3C, DebugWork::g_dw.atelwork.CardBreakCount); break;
        case 7: CGameWork::setInt(0, 0x3D, DebugWork::g_dw.atelwork.StockKind); break;
        case 8: CGameWork::setInt(0, 0x3E, DebugWork::g_dw.atelwork.StockCount); break;
        case 15: CGameWork::setInt(0, 0x3F, DebugWork::g_dw.atelwork.SpeedCount); break;
        case 4:
        case 16: CGameWork::setInt(0, 0xB3, DebugWork::g_dw.atelwork.KillEnemyCount); break;
        case 18: CGameWork::setInt(0, 0x41, DebugWork::g_dw.atelwork.DarkCount); break;
        }
    }
    return 1;
}
}
namespace DWTEvent {
static INT debugBtlReportCallBack(CDebugWindow* pObj, INT inParam) {
    INT i;
    DebugWork::g_dw.atelwork.BtlMemo = CJiminyMemoFlag::getContentOpenPercentage(CGameWork::getByte(0, 0x60));
    DebugWork::g_dw.atelwork.KillEnemyCount = CGameWork::getInt(0, 0xB3);
    DebugWork::g_dw.atelwork.KeysTresur = 0;
    for (i = 0; i < 16; i++) {
        DebugWork::g_dw.atelwork.KeysTresur += CGameWork::progMCW()._KeyRoomsTresureBoxCounter[i];
    }
    DebugWork::g_dw.atelwork.CardBreakCount = CGameWork::getInt(0, 0x3C);
    DebugWork::g_dw.atelwork.StockKind = CGameWork::getInt(0, 0x3D);
    DebugWork::g_dw.atelwork.StockCount = CGameWork::getInt(0, 0x3E);
    UINT nTime = CMainApp::getPlayTime();
    DebugWork::g_dw.atelwork.Hour = nTime / 3600;
    DebugWork::g_dw.atelwork.minit = nTime % 3600 / 60;
    if (DebugWork::g_dw.atelwork.Hour >= 1000) {
        DebugWork::g_dw.atelwork.Hour = 999;
        DebugWork::g_dw.atelwork.minit = 59;
    }
    F_UL win;
    win.ul = CGameWork::getInt(0, 0x40);
    DebugWork::g_dw.atelwork.DuelWin = win.f * 100.0f;
    DebugWork::g_dw.atelwork.DarkCount = CGameWork::getInt(0, 0x41);
    DebugWork::g_dw.atelwork.SpeedCount = CGameWork::getInt(0, 0x3F);
    return 1;
}
}
namespace DWTEvent {
static INT debugKeyTresurCallBack(CDebugWindow* pObj, INT inParam) {
    DebugWork::g_dw.atelwork.TreTr = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[1];
    DebugWork::g_dw.atelwork.TreAg = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[2];
    DebugWork::g_dw.atelwork.TreOl = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[3];
    DebugWork::g_dw.atelwork.TreWo = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[4];
    DebugWork::g_dw.atelwork.TreMo = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[5];
    DebugWork::g_dw.atelwork.TreHa = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[6];
    DebugWork::g_dw.atelwork.TreAt = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[7];
    DebugWork::g_dw.atelwork.TreNe = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[8];
    DebugWork::g_dw.atelwork.TreHo = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[9];
    DebugWork::g_dw.atelwork.TrePo = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[10];
    DebugWork::g_dw.atelwork.TreTw = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[11];
    DebugWork::g_dw.atelwork.TreDe = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[12];
    DebugWork::g_dw.atelwork.TreCa = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[13];
    return 1;
}
}
/* placeholder: rodata of the tables before DebugSetWorldTitleFlag__8DWTEventP12CDebugWindowii */
__asm__(".rdata\n");
inline void _roPh3() {
    TRACE("\x83g\x83\x89\x83\x94\x83@\x81[\x83X       :%d");
    TRACE("\x83" "A\x83O\x83\x89\x83o           :%d");
    TRACE("\x83I\x83\x8a\x83\x93\x83|\x83X         :%d");
    TRACE("\x83\x8f\x83\x93\x83_\x81[\x83\x89\x83\x93\x83h     :%d");
    TRACE("\x83\x82\x83\x93\x83X\x83g\x83\x8d         :%d");
    TRACE("\x83n\x83\x8d\x83" "E\x83" "B\x83\x93         :%d");
    TRACE("\x83" "A\x83g\x83\x89\x83\x93\x83" "e\x83" "B\x83J     :%d");
    TRACE("\x83l\x83o\x81[\x83\x89\x83\x93\x83h       :%d");
    TRACE("\x83z\x83\x8d\x83" "E\x83o\x83X\x83" "e\x83" "B\x83I\x83\x93 :%d");
    TRACE("100\x83G\x81[\x83J\x81[        :%d");
    TRACE("\x83g\x83\x8f\x83" "C\x83\x89\x83" "C\x83g       :%d");
    TRACE("\x83" "f\x83X\x83" "e\x83j\x81[         :%d");
    TRACE("\x96Y\x8bp\x82\xcc\x8f\xe9           :%d");
}
namespace DWTEvent {
static BOOL DebugSetWorldTitleFlag(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT world = CGameWork::getInt(0, 0xC1);
        switch (inY) {
        case 1: world ^= 0x1; break;
        case 2: world ^= 0x2; break;
        case 3: world ^= 0x4; break;
        case 4: world ^= 0x8; break;
        case 5: world ^= 0x10; break;
        case 6: world ^= 0x20; break;
        case 7: world ^= 0x40; break;
        case 8: world ^= 0x80; break;
        case 9: world ^= 0x100; break;
        case 10: world ^= 0x200; break;
        case 11: world ^= 0x400; break;
        case 12: world ^= 0x800; break;
        case 13: world ^= 0x1000; break;
        case 15: world ^= 0x10000; break;
        case 16: world ^= 0x20000; break;
        case 17: world ^= 0x40000; break;
        case 18: world ^= 0x80000; break;
        case 19: world ^= 0x100000; break;
        case 20: world ^= 0x200000; break;
        case 21: world ^= 0x400000; break;
        case 22: world ^= 0x800000; break;
        case 23: world ^= 0x1000000; break;
        case 24: world ^= 0x2000000; break;
        case 25: world ^= 0x4000000; break;
        case 26: world ^= 0x8000000; break;
        case 27: world ^= 0x10000000; break;
        }
        DebugWork::g_dw.atelwork.iWorldIntro = world;
        CGameWork::setInt(0, 0xC1, world);
    }
    return 1;
}
}
/* the asm .data (debug window tables) refers to it by name until the data comes from source */
__asm__(".globl DebugSetWorldTitleFlag__8DWTEventP12CDebugWindowii");
namespace DWTEvent {
static BOOL DebugSetTutorialFlag(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT tuto = CGameWork::getInt(0, 0xC2);
        switch (inY) {
        case 0: tuto ^= 0x1; break;
        case 1: tuto ^= 0x2; break;
        case 2: tuto ^= 0x4; break;
        case 3: tuto ^= 0x8; break;
        case 4: tuto ^= 0x10; break;
        case 5: tuto ^= 0x20; break;
        case 6: tuto ^= 0x40; break;
        }
        DebugWork::g_dw.atelwork.iTutorialEnd = tuto;
        CGameWork::setInt(0, 0xC2, tuto);
    }
    return 1;
}
}
/* the asm .data (debug window tables) refers to it by name until the data comes from source */
__asm__(".globl DebugSetTutorialFlag__8DWTEventP12CDebugWindowii");
namespace DWTEvent {
static BOOL DebugSetCardPossibleFlag(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT card = CGameWork::getInt(0, 0x21);
        switch (inY) {
        case 0: card ^= 0x1; break;
        case 1: card ^= 0x2; break;
        case 2: card ^= 0x4; break;
        case 3: card ^= 0x8; break;
        case 4: card ^= 0x10; break;
        case 5: card ^= 0x20; break;
        case 6: card ^= 0x40; break;
        case 7: card ^= 0x80; break;
        case 8: card ^= 0x100; break;
        case 9: card ^= 0x200; break;
        case 10: card ^= 0x400; break;
        case 11: card ^= 0x800; break;
        case 12: card ^= 0x1000; break;
        case 13: card ^= 0x2000; break;
        case 14: card ^= 0x4000; break;
        case 15: card ^= 0x8000; break;
        case 16: card ^= 0x10000; break;
        case 17: card ^= 0x20000; break;
        case 18: card ^= 0x40000; break;
        case 19: card ^= 0x80000; break;
        case 20: card ^= 0x100000; break;
        case 21: card ^= 0x200000; break;
        case 22: card ^= 0x400000; break;
        case 23: card ^= 0x800000; break;
        case 24: card ^= 0x1000000; break;
        case 25: card ^= 0x2000000; break;
        }
        DebugWork::g_dw.atelwork.iPossibleCard = card;
        CGameWork::setInt(0, 0x21, card);
    }
    return 1;
}
}
/* the asm .data (debug window tables) refers to it by name until the data comes from source */
__asm__(".globl DebugSetCardPossibleFlag__8DWTEventP12CDebugWindowii");
namespace DWTEvent {
static BOOL DebugSetFriendPossibleFlag(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT fri = CGameWork::getInt(0, 0x22);
        switch (inY) {
        case 0: fri ^= 0x1; break;
        case 1: fri ^= 0x2; break;
        case 2: fri ^= 0x4; break;
        case 3: fri ^= 0x8; break;
        case 4: fri ^= 0x10; break;
        case 5: fri ^= 0x20; break;
        case 6: fri ^= 0x40; break;
        case 7: fri ^= 0x80; break;
        case 8: fri ^= 0x100; break;
        }
        DebugWork::g_dw.atelwork.iPossibleFriend = fri;
        CGameWork::setInt(0, 0x22, fri);
    }
    return 1;
}
}
/* the asm .data (debug window tables) refers to it by name until the data comes from source */
__asm__(".globl DebugSetFriendPossibleFlag__8DWTEventP12CDebugWindowii");
namespace DWTEvent {
static BOOL DebugSetPoohFlag(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT intro = CGameWork::getInt(0, 0x23);
        switch (inY) {
        case 4: intro ^= 0x1; break;
        case 5: intro ^= 0x2; break;
        case 6: intro ^= 0x4; break;
        case 7: intro ^= 0x8; break;
        case 8: intro ^= 0x10; break;
        case 9: intro ^= 0x20; break;
        case 10: intro ^= 0x40; break;
        case 11: intro ^= 0x80; break;
        }
        DebugWork::g_dw.atelwork.iPoohMiniGameIntro = intro;
        CGameWork::setInt(0, 0x23, intro);
        CGameWork::setInt(0, 0x2E, DebugWork::g_dw.atelwork.iPoohHiScoreOwl);
        CGameWork::setInt(0, 0x2F, DebugWork::g_dw.atelwork.iPoohHiScoreRoo);
        CGameWork::setInt(0, 0x30, DebugWork::g_dw.atelwork.iPoohHiScoreEeyore);
        CGameWork::setInt(0, 0x31, DebugWork::g_dw.atelwork.iPoohHiScoreTigger);
        CGameWork::setInt(0, 0x32, DebugWork::g_dw.atelwork.iPoohHiScoreRabbit);
        CGameWork::setByte(0, 3, DebugWork::g_dw.atelwork.iPoohGamePiglet);
        CGameWork::setByte(0, 4, DebugWork::g_dw.atelwork.iPoohGameOwl);
        CGameWork::setByte(0, 5, DebugWork::g_dw.atelwork.iPoohGameRoo);
        CGameWork::setByte(0, 6, DebugWork::g_dw.atelwork.iPoohGameEeyore);
        CGameWork::setByte(0, 7, DebugWork::g_dw.atelwork.iPoohGameTigger);
        CGameWork::setByte(0, 8, DebugWork::g_dw.atelwork.iPoohGameRabbit);
    }
    return 1;
}
}
namespace DWTEvent {
static BOOL DebugSetPcSkillFlag(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT skill = CGameWork::getInt(0, 0x24);
        switch (inY) {
        case 0: skill ^= 0x1; break;
        case 1: skill ^= 0x2; break;
        case 2: skill ^= 0x4; break;
        case 3: skill ^= 0x8; break;
        case 4: skill ^= 0x10; break;
        case 5: skill ^= 0x20; break;
        }
        DebugWork::g_dw.atelwork.iPcStanderdSkill = skill;
        CGameWork::setInt(0, 0x24, skill);
    }
    return 1;
}
}
/* the asm .data (debug window tables) refers to it by name until the data comes from source */
__asm__(".globl DebugSetPcSkillFlag__8DWTEventP12CDebugWindowii");
namespace DWTEvent {
static BOOL DebugSetSoraMovieFlag(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT movie0 = CGameWork::getInt(0, 0x27);
        INT movie1 = CGameWork::getInt(0, 0x28);
        INT movie2 = CGameWork::getInt(0, 0x29);
        switch (inY) {
        case 0: movie0 ^= 0x1; break;
        case 1: movie0 ^= 0x2; break;
        case 2: movie0 ^= 0x4; break;
        case 3: movie0 ^= 0x8; break;
        case 4: movie0 ^= 0x10; break;
        case 5: movie0 ^= 0x20; break;
        case 6: movie0 ^= 0x40; break;
        case 7: movie0 ^= 0x80; break;
        case 8: movie0 ^= 0x100; break;
        case 9: movie0 ^= 0x200; break;
        case 10: movie0 ^= 0x400; break;
        case 11: movie0 ^= 0x800; break;
        case 12: movie0 ^= 0x1000; break;
        case 13: movie0 ^= 0x2000; break;
        case 14: movie0 ^= 0x4000; break;
        case 15: movie0 ^= 0x8000; break;
        case 16: movie0 ^= 0x10000; break;
        case 17: movie0 ^= 0x20000; break;
        case 18: movie0 ^= 0x40000; break;
        case 19: movie0 ^= 0x80000; break;
        case 20: movie0 ^= 0x100000; break;
        case 21: movie0 ^= 0x200000; break;
        case 22: movie0 ^= 0x400000; break;
        case 23: movie0 ^= 0x800000; break;
        case 24: movie0 ^= 0x1000000; break;
        case 25: movie0 ^= 0x2000000; break;
        case 26: movie0 ^= 0x4000000; break;
        case 27: movie0 ^= 0x8000000; break;
        case 28: movie0 ^= 0x10000000; break;
        case 29: movie0 ^= 0x20000000; break;
        case 30: movie0 ^= 0x40000000; break;
        case 31: movie0 ^= 0x80000000; break;
        case 32: movie1 ^= 0x1; break;
        case 33: movie1 ^= 0x2; break;
        case 34: movie1 ^= 0x4; break;
        case 35: movie1 ^= 0x8; break;
        case 36: movie1 ^= 0x10; break;
        case 37: movie1 ^= 0x20; break;
        case 38: movie1 ^= 0x40; break;
        case 39: movie1 ^= 0x80; break;
        case 40: movie1 ^= 0x100; break;
        case 41: movie1 ^= 0x200; break;
        case 42: movie1 ^= 0x400; break;
        case 43: movie1 ^= 0x800; break;
        case 44: movie1 ^= 0x1000; break;
        case 45: movie1 ^= 0x2000; break;
        case 46: movie1 ^= 0x4000; break;
        case 47: movie1 ^= 0x8000; break;
        case 48: movie1 ^= 0x10000; break;
        case 49: movie1 ^= 0x20000; break;
        case 50: movie1 ^= 0x40000; break;
        case 51: movie1 ^= 0x80000; break;
        case 52: movie1 ^= 0x100000; break;
        case 53: movie1 ^= 0x200000; break;
        case 54: movie1 ^= 0x400000; break;
        case 55: movie1 ^= 0x800000; break;
        case 56: movie1 ^= 0x1000000; break;
        case 57: movie1 ^= 0x2000000; break;
        case 58: movie1 ^= 0x4000000; break;
        }
        DebugWork::g_dw.atelwork.iSoraMovie0 = movie0;
        CGameWork::setInt(0, 0x27, movie0);
        DebugWork::g_dw.atelwork.iSoraMovie1 = movie1;
        CGameWork::setInt(0, 0x28, movie1);
        DebugWork::g_dw.atelwork.iSoraMovie2 = movie2;
        CGameWork::setInt(0, 0x29, movie2);
    }
    return 1;
}
}
/* the asm .data (debug window tables) refers to it by name until the data comes from source */
__asm__(".globl DebugSetSoraMovieFlag__8DWTEventP12CDebugWindowii");
namespace DWTEvent {
static BOOL DebugSetRikuMovieFlag(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT movie0 = CGameWork::getInt(0, 0x2A);
        INT movie1 = CGameWork::getInt(0, 0x2B);
        INT movie2 = CGameWork::getInt(0, 0x2C);
        switch (inY) {
        case 0: movie0 ^= 0x1; break;
        case 1: movie0 ^= 0x2; break;
        case 2: movie0 ^= 0x4; break;
        case 3: movie0 ^= 0x8; break;
        case 4: movie0 ^= 0x10; break;
        case 5: movie0 ^= 0x20; break;
        case 6: movie0 ^= 0x40; break;
        case 7: movie0 ^= 0x80; break;
        case 8: movie0 ^= 0x100; break;
        case 9: movie0 ^= 0x200; break;
        case 10: movie0 ^= 0x400; break;
        case 11: movie0 ^= 0x800; break;
        case 12: movie0 ^= 0x1000; break;
        case 13: movie0 ^= 0x2000; break;
        case 14: movie0 ^= 0x4000; break;
        case 15: movie0 ^= 0x8000; break;
        case 16: movie0 ^= 0x10000; break;
        case 17: movie0 ^= 0x20000; break;
        case 18: movie0 ^= 0x40000; break;
        case 19: movie0 ^= 0x80000; break;
        case 20: movie0 ^= 0x100000; break;
        case 21: movie0 ^= 0x200000; break;
        case 22: movie0 ^= 0x400000; break;
        case 23: movie0 ^= 0x800000; break;
        case 24: movie0 ^= 0x1000000; break;
        case 25: movie0 ^= 0x2000000; break;
        case 26: movie0 ^= 0x4000000; break;
        case 27: movie0 ^= 0x8000000; break;
        case 28: movie0 ^= 0x10000000; break;
        case 29: movie0 ^= 0x20000000; break;
        case 30: movie0 ^= 0x40000000; break;
        case 31: movie0 ^= 0x80000000; break;
        case 32: movie1 ^= 0x1; break;
        case 33: movie1 ^= 0x2; break;
        case 34: movie1 ^= 0x4; break;
        case 35: movie1 ^= 0x8; break;
        case 36: movie1 ^= 0x10; break;
        case 37: movie1 ^= 0x20; break;
        case 38: movie1 ^= 0x40; break;
        case 39: movie1 ^= 0x80; break;
        case 40: movie1 ^= 0x100; break;
        }
        DebugWork::g_dw.atelwork.iRikuMovie0 = movie0;
        CGameWork::setInt(0, 0x2A, movie0);
        DebugWork::g_dw.atelwork.iRikuMovie1 = movie1;
        CGameWork::setInt(0, 0x2B, movie1);
        DebugWork::g_dw.atelwork.iRikuMovie2 = movie2;
        CGameWork::setInt(0, 0x2C, movie2);
    }
    return 1;
}
}
/* the asm .data (debug window tables) refers to it by name until the data comes from source */
__asm__(".globl DebugSetRikuMovieFlag__8DWTEventP12CDebugWindowii");
namespace DWTEvent {
static BOOL DebugSetKH2CardGetFlag(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT card = CGameWork::getInt(0, 0x2D);
        switch (inY) {
        case 0: card ^= 0x1; break;
        case 1: card ^= 0x2; break;
        case 2: card ^= 0x4; break;
        case 3: card ^= 0x8; break;
        case 4: card ^= 0x10; break;
        case 5: card ^= 0x20; break;
        case 6: card ^= 0x40; break;
        case 7: card ^= 0x80; break;
        case 8: card ^= 0x100; break;
        case 9: card ^= 0x200; break;
        case 10: card ^= 0x400; break;
        case 11: card ^= 0x800; break;
        case 12: card ^= 0x1000; break;
        }
        DebugWork::g_dw.atelwork.iKH2CardGet = card;
        CGameWork::setInt(0, 0x2D, card);
    }
    return 1;
}
}
/* the asm .data (debug window tables) refers to it by name until the data comes from source */
__asm__(".globl DebugSetKH2CardGetFlag__8DWTEventP12CDebugWindowii");
namespace DWTEvent {
extern BYTE SoraEnemyMemoIdTbl[];
extern BYTE RikuEnemyMemoIdTbl[];
extern USHORT SoraCardKindTbl[];
extern USHORT RikuCardKindTbl[];
static BOOL DebugSetJiminyMemoFlag(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT SoraWorld = DebugWork::g_dw.jiminy.Sora_World;
        INT SoraFloor = DebugWork::g_dw.jiminy.Sora_Floor;
        INT RikuFloor = DebugWork::g_dw.jiminy.Riku_Floor;
        INT RikuWorld = DebugWork::g_dw.jiminy.Riku_World;
        INT SoraEtc = DebugWork::g_dw.jiminy.Sora_Etc;
        INT shiftY;
        switch (inY) {
        case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 13:
            shiftY = 1 << (inY - 1);
            SoraWorld ^= shiftY;
            CJiminyMemoFlag::debugEvent(SoraWorld & shiftY, 0, 1, inY, 0);
            break;
        case 14: case 15: case 16: case 17: case 18: case 19: case 20: case 21: case 22: case 23: case 24: case 25: case 26:
            shiftY = 1 << (inY - 14);
            SoraFloor ^= shiftY;
            CJiminyMemoFlag::debugEvent(SoraFloor & shiftY, 0, 2, inY - 13, 0);
            break;
        case 28: case 29: case 30: case 31: case 32: case 33: case 34: case 35: case 36: case 37: case 38: case 39:
            shiftY = 1 << (inY - 28);
            RikuWorld ^= shiftY;
            CJiminyMemoFlag::debugEvent(RikuWorld & shiftY, 1, 1, inY - 27, 0);
            break;
        case 40: case 41: case 42: case 43: case 44: case 45: case 46: case 47: case 48: case 49: case 50: case 51:
            shiftY = 1 << (inY - 40);
            RikuFloor ^= shiftY;
            CJiminyMemoFlag::debugEvent(RikuFloor & shiftY, 1, 2, inY - 52, 0);
            break;
        case 53: SoraEtc ^= 0x1; CJiminyMemoFlag::debugEvent(SoraEtc & 0x1, 0, 8, 0, 0); break;
        case 54: SoraEtc ^= 0x2; CJiminyMemoFlag::debugEvent(SoraEtc & 0x2, 0, 8, 1, 0); break;
        case 55: SoraEtc ^= 0x4; CJiminyMemoFlag::debugEvent(SoraEtc & 0x4, 0, 8, 2, 0); break;
        case 56: SoraEtc ^= 0x8; CJiminyMemoFlag::debugEvent(SoraEtc & 0x8, 0, 8, 3, 0); break;
        case 57: SoraEtc ^= 0x10; CJiminyMemoFlag::debugEvent(SoraEtc & 0x10, 0, 8, 4, 0); break;
        case 58: SoraEtc ^= 0x20; CJiminyMemoFlag::debugEvent(SoraEtc & 0x20, 0, 6, 0, 0); break;
        case 59: SoraEtc ^= 0x40; CJiminyMemoFlag::debugEvent(SoraEtc & 0x40, 0, 5, 0, 0); break;
        case 60: SoraEtc ^= 0x80; CJiminyMemoFlag::debugEvent(SoraEtc & 0x80, 0, 10, 11, 0); break;
        case 61: SoraEtc ^= 0x100; CJiminyMemoFlag::debugEvent(SoraEtc & 0x100, 0, 10, 12, 0); break;
        case 62: SoraEtc ^= 0x200; CJiminyMemoFlag::debugEvent(SoraEtc & 0x200, 0, 10, 13, 0); break;
        case 63: SoraEtc ^= 0x400; CJiminyMemoFlag::debugEvent(SoraEtc & 0x400, 0, 10, 14, 0); break;
        case 64: SoraEtc ^= 0x800; CJiminyMemoFlag::debugEvent(SoraEtc & 0x800, 0, 10, 15, 0); break;
        case 65: SoraEtc ^= 0x1000; CJiminyMemoFlag::debugEvent(SoraEtc & 0x1000, 0, 10, 16, 0); break;
        case 66: SoraEtc ^= 0x2000; CJiminyMemoFlag::debugEvent(SoraEtc & 0x2000, 0, 9, 0, 0); break;
        }
        DebugWork::g_dw.jiminy.Sora_World = SoraWorld;
        DebugWork::g_dw.jiminy.Sora_Floor = SoraFloor;
        DebugWork::g_dw.jiminy.Riku_World = RikuWorld;
        DebugWork::g_dw.jiminy.Riku_Floor = RikuFloor;
        DebugWork::g_dw.jiminy.Sora_Etc = SoraEtc;
    } else if (inParam & 0x20) {
        INT SoraFlag = DebugWork::g_dw.jiminy.Sora_Flag;
        INT SoraEKind = SoraEnemyMemoIdTbl[DebugWork::g_dw.jiminy.Sora_Enemy];
        INT SoraENum = DebugWork::g_dw.jiminy.Sora_ENum;
        INT SoraCard = DebugWork::g_dw.jiminy.Sora_Card;
        USHORT SoraCKind = SoraCardKindTbl[SoraCard];
        INT RikuFlag = DebugWork::g_dw.jiminy.Riku_Flag;
        INT RikuEKind = RikuEnemyMemoIdTbl[DebugWork::g_dw.jiminy.Riku_Enemy];
        INT RikuENum = DebugWork::g_dw.jiminy.Riku_ENum;
        INT RikuCard = DebugWork::g_dw.jiminy.Riku_Card;
        USHORT RikuCKind = RikuCardKindTbl[RikuCard];
        switch (inY) {
        case 68: case 69: case 70:
            SoraFlag |= 0x701;
            CJiminyMemoFlag::debugEvent(TRUE, 0, 3, SoraEKind, SoraENum);
            break;
        case 72: case 73: case 74:
            RikuFlag |= 0x701;
            CJiminyMemoFlag::debugEvent(TRUE, 1, 3, RikuEKind, RikuENum);
            break;
        case 76: case 77:
            SoraFlag |= 0x7002;
            CJiminyMemoFlag::debugEvent(TRUE, 0, 4, SoraCKind, 0);
            break;
        case 79: case 80:
            RikuFlag |= 0x7002;
            CJiminyMemoFlag::debugEvent(TRUE, 1, 4, RikuCKind, 0);
            break;
        }
        DebugWork::g_dw.jiminy.Sora_Flag = SoraFlag;
        DebugWork::g_dw.jiminy.Riku_Flag = RikuFlag;
    } else {
        INT SoraFlag = DebugWork::g_dw.jiminy.Sora_Flag;
        INT RikuFlag = DebugWork::g_dw.jiminy.Riku_Flag;
        INT n;
        for (INT i = 0; i < 2; i++) {
            if ((SoraFlag >> i) & 1) {
                n = (SoraFlag >> (i * 4 + 8)) & 0xF;
                n--;
                if (n == 0) SoraFlag &= ~(1 << i);
                SoraFlag = (SoraFlag & ~(0xF << (i * 4 + 8))) | (n << (i * 4 + 8));
            }
        }
        for (INT i = 0; i < 2; i++) {
            if ((RikuFlag >> i) & 1) {
                n = (RikuFlag >> (i * 4 + 8)) & 0xF;
                n--;
                if (n == 0) RikuFlag &= ~(1 << i);
                RikuFlag = (RikuFlag & ~(0xF << (i * 4 + 8))) | (n << (i * 4 + 8));
            }
        }
        DebugWork::g_dw.jiminy.Sora_Flag = SoraFlag;
        DebugWork::g_dw.jiminy.Riku_Flag = RikuFlag;
    }
    return 1;
}
}
/* the asm .data (debug window tables) refers to it by name until the data comes from source */
__asm__(".globl DebugSetJiminyMemoFlag__8DWTEventP12CDebugWindowii");
/* placeholder: rodata of the tables before debugGameInfomationParam__8DWTEventP12CDebugWindowii */
__asm__(".rdata\n");
inline void _roPh14() {
    TRACE("\x83V\x83\x83\x83h\x83" "E");
    TRACE("\x83\\\x83\x8b\x83W\x83\x83\x81[");
    TRACE("\x83\x8c\x83" "b\x83h\x83m\x83N\x83^\x81[\x83\x93");
    TRACE("\x83u\x83\x8b\x81[\x83\x89\x83v\x83\\\x83" "f\x83" "B");
    TRACE("\x83" "C\x83G\x83\x8d\x81[\x83I\x83y\x83\x89");
    TRACE("\x83O\x83\x8a\x81[\x83\x93\x83\x8c\x83N\x83" "C\x83G\x83\x80");
    TRACE("\x83p\x83\x8f\x81[\x83\x8f\x83" "C\x83\x8b\x83h");
    TRACE("\x83o\x83" "E\x83\x93\x83V\x81[\x83\x8f\x83" "C\x83\x8b\x83h");
    TRACE("\x83\x89\x81[\x83W\x83{\x83" "f\x83" "B");
    TRACE("\x83t\x83@\x83" "b\x83g\x83o\x83\x93\x83" "f\x83" "B\x83" "b\x83g");
    TRACE("\x83o\x83\x93\x83" "f\x83" "B\x83" "b\x83g");
    TRACE("\x83\x8f\x83" "C\x83g\x83i\x83" "C\x83g");
    TRACE("\x83K\x81[\x83S\x83" "C\x83\x8b");
    TRACE("\x83T\x81[\x83`\x83S\x81[\x83X\x83g");
    TRACE("\x83G\x83" "A\x83\\\x83\x8b\x83W\x83\x83\x81[");
    TRACE("\x83z\x83\x8f\x83" "C\x83g\x83}\x83" "b\x83V\x83\x85\x83\x8b\x81[\x83\x80");
    TRACE("\x83u\x83\x89\x83" "b\x83N\x83t\x83@\x83\x93\x83K\x83X");
    TRACE("\x83V\x81[\x83l\x83I\x83\x93");
    TRACE("\x83X\x83N\x83\x8a\x83\x85\x81[\x83_\x83" "C\x83o\x81[");
    TRACE("\x83" "A\x83N\x83" "A\x83^\x83\x93\x83N");
    TRACE("\x83p\x83" "C\x83\x8c\x81[\x83" "c");
    TRACE("\x83G\x83" "A\x83p\x83" "C\x83\x8c\x81[\x83" "c");
    TRACE("\x83_\x81[\x83N\x83{\x81[\x83\x8b");
    TRACE("\x83\x8f\x83" "C\x83o\x81[\x83\x93");
    TRACE("\x83" "E\x83" "B\x83U\x81[\x83h");
    TRACE("\x83" "f\x83" "B\x83t\x83" "F\x83\x93\x83_\x81[");
    TRACE("\x83l\x83I\x83V\x83\x83\x83h\x83" "E");
    TRACE("\x83o\x83\x8c\x83\x8b\x83X\x83p\x83" "C\x83_\x81[");
    TRACE("\x83g\x83\x8b\x83l\x81[\x83h\x83X\x83" "e\x83" "b\x83v");
    TRACE("\x83\x89\x83" "E\x83h\x83l\x83X");
    TRACE("\x83N\x83\x8a\x81[\x83v\x83v\x83\x89\x83\x93\x83g");
    TRACE("\x83K\x81[\x83h\x83" "A\x81[\x83}\x81[");
    TRACE("\x83p\x83\x89\x83T\x83" "C\x83g\x83P\x83" "C\x83W");
    TRACE("\x83g\x83\x8a\x83" "b\x83N\x83}\x83X\x83^\x81[");
    TRACE("\x83_\x81[\x83N\x83T\x83" "C\x83h");
    TRACE("\x83L\x83\x93\x83O\x83_\x83\x80\x83`\x83" "F\x81[\x83\x93");
    TRACE("\x83" "f\x83U\x83" "C\x83" "A\x81[\x83\x89\x83\x93\x83v");
    TRACE("\x83g\x83\x8c\x83W\x83\x83\x81[\x83I\x83u\x83V\x81[");
    TRACE("\x83p\x83\x93\x83v\x83L\x83\x93\x83w\x83" "b\x83h");
    TRACE("\x83t\x83" "F\x83" "A\x83\x8a\x81[\x83n\x81[\x83v");
    TRACE("\x83" "E\x83" "B\x83" "b\x83V\x83\x85\x83X\x83^\x81[");
    TRACE("\x83G\x83O\x83U\x83~\x83l\x81[\x83V\x83\x87\x83\x93");
    TRACE("\x83\x81\x83^\x83\x8b\x83`\x83\x87\x83R\x83{");
    TRACE("\x83p\x83\x8f\x81[\x83I\x83u\x83q\x81[\x83\x8d\x81[");
    TRACE("\x83\x89\x83" "C\x83I\x83\x93\x83n\x81[\x83g");
    TRACE("\x83\x89\x83X\x83g\x83\x8a\x83]\x81[\x83g");
    TRACE("\x83\x89\x83\x94\x83" "B\x83" "A\x83\x93\x83\x8d\x81[\x83Y");
    TRACE("\x96\xf1\x91\xa9\x82\xcc\x82\xa8\x8e\xe7\x82\xe8");
    TRACE("\x89\xdf\x82\xac\x8b\x8e\x82\xe8\x82\xb5\x8ev\x82\xa2\x8fo");
    TRACE("\x83" "A\x83\x8b\x83" "e\x83}\x83" "E\x83" "F\x83|\x83\x93");
    TRACE("\x83_\x83" "C\x83\x84\x83\x82\x83\x93\x83h\x83_\x83X\x83g");
    TRACE("\x95\xd0\x97\x83\x82\xcc\x93V\x8eg");
    TRACE("\x83X\x83^\x81[\x83V\x81[\x83J\x81[");
    TRACE("\x83\x82\x83m\x83N\x83\x8d\x81[\x83\x80");
    TRACE("\x83t\x83H\x83\x8d\x81[\x83U\x83" "E\x83" "B\x83\x93\x83h");
    TRACE("\x83q\x83h\x83" "D\x83\x93\x83h\x83\x89\x83S\x83\x93");
    TRACE("\x83t\x83H\x83g\x83\x93\x83" "f\x83o\x83" "b\x83K\x81[");
    TRACE("\x83{\x83\x93\x83h\x83I\x83u\x83t\x83\x8c\x83" "C\x83\x80");
    TRACE("\x83t\x83@\x83" "C\x83" "A");
    TRACE("\x83u\x83\x8a\x83U\x83h");
    TRACE("\x83T\x83\x93\x83_\x81[");
    TRACE("\x83P\x83" "A\x83\x8b");
    TRACE("\x83O\x83\x89\x83r\x83" "f");
    TRACE("\x83X\x83g\x83" "b\x83v");
    TRACE("\x83G\x83" "A\x83\x8d");
    TRACE("\x83V\x83\x93\x83o");
    TRACE("\x83W\x81[\x83j\x81[");
    TRACE("\x83o\x83\x93\x83r");
    TRACE("\x83_\x83\x93\x83{");
    TRACE("\x83" "e\x83" "B\x83\x93\x83N");
    TRACE("\x83\x80\x81[\x83V\x83\x85\x81[");
    TRACE("\x83N\x83\x89\x83" "E\x83h");
    TRACE("\x83|\x81[\x83V\x83\x87\x83\x93");
    TRACE("\x83n\x83" "C\x83|\x81[\x83V\x83\x87\x83\x93");
    TRACE("\x83\x81\x83K\x83|\x81[\x83V\x83\x87\x83\x93");
    TRACE("\x83G\x81[\x83" "e\x83\x8b");
    TRACE("\x83\x81\x83K\x83G\x81[\x83" "e\x83\x8b");
    TRACE("\x83G\x83\x8a\x83N\x83T\x81[");
    TRACE("\x83\x89\x83X\x83g\x83G\x83\x8a\x83N\x83T\x81[");
    TRACE("\x83h\x83i\x83\x8b\x83h");
    TRACE("\x83O\x81[\x83t\x83" "B\x81[");
    TRACE("\x83" "A\x83\x89\x83W\x83\x93");
    TRACE("\x83" "A\x83\x8a\x83G\x83\x8b");
    TRACE("\x83W\x83\x83\x83" "b\x83N");
    TRACE("\x83s\x81[\x83^\x81[\x83p\x83\x93");
    TRACE("\x83r\x81[\x83X\x83g");
    TRACE("\x83v\x83\x8b\x81[\x83g");
    TRACE("\x83n\x83" "f\x83X");
    TRACE("\x83W\x83\x83\x83t\x83@\x81[");
    TRACE("\x83" "A\x81[\x83X\x83\x89");
    TRACE("\x83u\x83M\x81[");
    TRACE("\x83t\x83" "b\x83N");
    TRACE("\x83}\x83\x8c\x83t\x83" "B\x83Z\x83\x93\x83g\x83h\x83\x89\x83S\x83\x93");
    TRACE("\x83g\x83\x89\x83\x93\x83v\x95\xba");
    TRACE("\x83" "A\x83\x93\x83Z\x83\x80");
    TRACE("\x83[\x83\x80\x83i\x83X");
    TRACE("\x83V\x83O\x83o\x81[\x83\x8b");
    TRACE("\x83U\x83\x8b\x83" "f\x83" "B\x83\x93");
    TRACE("\x83\x94\x83" "B\x83N\x83Z\x83\x93");
    TRACE("\x83\x8c\x83N\x83Z\x83" "E\x83X");
    TRACE("\x83[\x83N\x83V\x83I\x83\x93");
    TRACE("\x83T\x83" "C\x83N\x83X");
    TRACE("\x83" "A\x83N\x83Z\x83\x8b");
    TRACE("\x83" "f\x83~\x83" "b\x83N\x83X");
    TRACE("\x83\x8b\x83N\x83\\\x81[\x83h");
    TRACE("\x83}\x81[\x83\x8b\x81[\x83V\x83\x83");
    TRACE("\x83\x89\x83N\x83V\x81[\x83k");
    TRACE("\x83\x8d\x83N\x83T\x83X");
    TRACE("\x83S\x81[\x83\x8b\x83h");
    TRACE("\x83v\x83\x89\x83`\x83i");
    TRACE("\x90\xc3\x82\xb6\x82\xe1\x82\xad\x82\xc8\x82\xe9\x88\xc5");
    TRACE("\x82\xd0\x82\xb5\x82\xdf\x82\xad\x88\xc5");
    TRACE("\x82\xbf\x82\xa9\x82\xe7\x82\xc8\x82\xab\x88\xc5");
    TRACE("\x82\xa8\x82\xa8\x82\xa2\x82\xc8\x82\xe9\x88\xc5");
    TRACE("\x96\xb0\x82\xe8\x82\xcc\x88\xc5");
    TRACE("\x82\xb9\x82\xdc\x82\xe8\x82\xad\x82\xe9\x88\xc5");
    TRACE("\x83v\x83\x8c\x83~\x83" "A\x83\x8b\x81[\x83\x80");
    TRACE("\x83z\x83\x8f\x83" "C\x83g\x83\x8b\x81[\x83\x80");
    TRACE("\x83u\x83\x89\x83" "b\x83N\x83\x8b\x81[\x83\x80");
    TRACE("\x92\xea\x92m\x82\xea\x82\xca\x88\xc5");
    TRACE("\x83\x8b\x81[\x83\x8c\x83" "b\x83g\x83\x8b\x81[\x83\x80");
    TRACE("\x95\x90\x97\xcd\x82\xd6\x82\xcc\x82\xdf\x82\xb4\x82\xdf");
    TRACE("\x96\x82\x97\xcd\x82\xd6\x82\xcc\x82\xdf\x82\xb4\x82\xdf");
    TRACE("\x98" "B\x8b\xe0\x82\xd6\x82\xcc\x82\xdf\x82\xb4\x82\xdf");
    TRACE("\x82\xdf\x82\xae\x82\xe8\x82\xa0\x82\xa2\x82\xcc\x92n");
    TRACE("\x82\xc4\x82\xa2\x82\xbd\x82\xa2\x82\xcc\x8a\xd4");
    TRACE("\x83_\x83\x81\x81[\x83W\x83" "A\x83" "b\x83v\x83\x8b\x81[\x83\x80");
    TRACE("\x83X\x83^\x83\x93\x83v\x83\x89\x83X\x83\x8b\x81[\x83\x80");
    TRACE("\x82\xe2\x82\xb7\x82\xe7\x82\xac\x82\xcc\x95\xf3");
    TRACE("\x8e\xe7\x82\xe7\x82\xea\x82\xb5\x95\xf3");
    TRACE("\x82\xa2\x82\xc2\x82\xed\x82\xe8\x82\xcc\x95\xf3");
    TRACE("\x88\xea\x8e\x9e\x82\xcc\x8bx\x91\xa7");
    TRACE("\x8c\xf0\x82\xb6\x82\xe8\x8d\x87\x82\xa4\x90\xa2\x8a" "E");
    TRACE("\x83\x82\x81[\x83O\x83\x8a\x83\x8b\x81[\x83\x80");
    TRACE("\x96\xa2\x92m\x82\xc8\x82\xe9\x95\xf3\x82\xcc\x83L\x81[\x83J\x81[\x83h");
    TRACE("\x82\xcd\x82\xb6\x82\xdc\x82\xe8\x82\xcc\x83L\x81[\x83J\x81[\x83h");
    TRACE("\x82\xdd\x82\xbf\x82\xd1\x82\xab\x82\xcc\x83L\x81[\x83J\x81[\x83h");
    TRACE("\x90^\x8e\xc0\x82\xd6\x82\xcc\x83L\x81[\x83J\x81[\x83h");
    TRACE("\x83W\x83\x87\x81[\x83J\x81[");
    TRACE("\x83g\x83\x89\x83o\x81[\x83X\x83^\x83" "E\x83\x93");
    TRACE("\x83" "A\x83O\x83\x89\x83o\x81[");
    TRACE("\x83I\x83\x8a\x83\x93\x83|\x83X\x81" "E\x83R\x83\x8d\x83V\x83" "A\x83\x80");
    TRACE("\x83\x8f\x83\x93\x83_\x81[\x83\x89\x83\x93\x83h");
    TRACE("\x83\x82\x83\x93\x83X\x83g\x83\x8d");
    TRACE("\x83n\x83\x8d\x83" "E\x83" "B\x83\x93\x83^\x83" "E\x83\x93");
    TRACE("\x83" "A\x83g\x83\x89\x83\x93\x83" "e\x83" "B\x83J");
    TRACE("\x83l\x83o\x81[\x83\x89\x83\x93\x83h");
    TRACE("\x83z\x83\x8d\x83" "E\x83o\x83X\x83" "e\x83" "B\x83I\x83\x93");
    TRACE("100\x83G\x81[\x83J\x81[\x82\xcc\x90X");
    TRACE("\x83g\x83\x8f\x83" "C\x83\x89\x83" "C\x83g\x83^\x83" "E\x83\x93");
    TRACE("\x83" "f\x83X\x83" "e\x83" "B\x83j\x81[\x83" "A\x83" "C\x83\x89\x83\x93\x83h");
    TRACE("\x83M\x83~\x83" "b\x83N-\x83K\x81[\x83h\x83" "A\x81[\x83}\x81[");
    TRACE("\x83M\x83~\x83" "b\x83N-\x83g\x83\x8a\x83" "b\x83N\x83X\x83^\x81[");
    TRACE("\x83M\x83~\x83" "b\x83N-\x83W\x83\x83\x83t\x83@\x81[");
    TRACE("\x83M\x83~\x83" "b\x83N-\x83u\x83M\x81[");
    TRACE("\x83M\x83~\x83" "b\x83N-\x83p\x83\x89\x83T\x83" "C\x83g\x83P\x83" "C\x83W");
    TRACE("\x83M\x83~\x83" "b\x83N-\x83" "A\x81[\x83X\x83\x89");
    TRACE("\x83M\x83~\x83" "b\x83N-\x83}\x83\x8c\x83t\x83" "B\x83Z\x83\x93\x83g\x83h\x83\x89\x83S\x83\x93");
    TRACE("\x83M\x83~\x83" "b\x83N-\x83_\x81[\x83N\x83T\x83" "C\x83h");
    TRACE("\x83\\\x83" "E\x83\x8b\x83" "C\x81[\x83^\x81[");
    TRACE("\x89\xa4\x97l");
    TRACE("\x96Y\x8bp\x82\xcc\x8f\xe9");
    TRACE("\x8dX\x90V");
    TRACE("\xf9S----------\xf9V\x83\\\x83\x89\xf9S---------");
    TRACE("\x83g\x83\x89\x83\x94\x83@\x81[\x83X       :%s");
    TRACE("\x83" "A\x83O\x83\x89\x83o\x81[         :%s");
    TRACE("\x83I\x83\x8a\x83\x93\x83|\x83X         :%s");
    TRACE("\x83\x8f\x83\x93\x83_\x81[\x83\x89\x83\x93\x83h     :%s");
    TRACE("\x83\x82\x83\x93\x83X\x83g\x83\x8d         :%s");
    TRACE("\x83n\x83\x8d\x83" "E\x83" "B\x83\x93\x83^\x83" "E\x83\x93   :%s");
    TRACE("\x83" "A\x83g\x83\x89\x83\x93\x83" "e\x83" "B\x83J     :%s");
    TRACE("\x83l\x83o\x81[\x83\x89\x83\x93\x83h       :%s");
    TRACE("\x83z\x83\x8d\x83" "E\x83o\x83X\x83" "e\x83" "B\x83I\x83\x93 :%s");
    TRACE("\x82P\x82O\x82O\x83G\x81[\x83J\x81[     :%s");
    TRACE("\x83g\x83\x8f\x83" "C\x83\x89\x83" "C\x83g       :%s");
    TRACE("\x83" "f\x83X\x83" "e\x83j\x81[         :%s");
    TRACE("\x82\xda\x82\xa4\x82\xab\x82\xe1\x82\xad\x82\xcc\x82\xb5\x82\xeb   :%s");
    TRACE("\x81@\x82P\x82" "e             :%s");
    TRACE("\x81@\x82Q\x82" "e             :%s");
    TRACE("\x81@\x82R\x82" "e             :%s");
    TRACE("\x81@\x82S\x82" "e             :%s");
    TRACE("\x81@\x82T\x82" "e             :%s");
    TRACE("\x81@\x82U\x82" "e             :%s");
    TRACE("\x81@\x82V\x82" "e             :%s");
    TRACE("\x81@\x82W\x82" "e             :%s");
    TRACE("\x81@\x82X\x82" "e             :%s");
    TRACE("\x82P\x82O\x82" "e             :%s");
    TRACE("\x82P\x82P\x82" "e             :%s");
    TRACE("\x82P\x82Q\x82" "e             :%s");
    TRACE("\x82P\x82R\x82" "e             :%s");
    TRACE("\xf9S----------\xf9V\x83\x8a\x83N\xf9S---------");
    TRACE("\x82" "a\x82P\x82Q\x82" "e           :%s");
    TRACE("\x82" "a\x82P\x82P\x82" "e           :%s");
    TRACE("\x82" "a\x82P\x82O\x82" "e           :%s");
    TRACE("\x82" "a\x81@\x82X\x82" "e           :%s");
    TRACE("\x82" "a\x81@\x82W\x82" "e           :%s");
    TRACE("\x82" "a\x81@\x82V\x82" "e           :%s");
    TRACE("\x82" "a\x81@\x82U\x82" "e           :%s");
    TRACE("\x82" "a\x81@\x82T\x82" "e           :%s");
    TRACE("\x82" "a\x81@\x82S\x82" "e           :%s");
    TRACE("\x82" "a\x81@\x82R\x82" "e           :%s");
    TRACE("\x82" "a\x81@\x82Q\x82" "e           :%s");
    TRACE("\x82" "a\x81@\x82P\x82" "e           :%s");
    TRACE("\xf9S------\xf9V\x83\\\x83\x89 \x82\xbb\x82\xcc\x91\xbc\xf9S------");
    TRACE("\x83~\x83j\x83Q\x81[\x83\x80\x81" "E\x96\xec\x8d\xd8   :%s");
    TRACE("\x83~\x83j\x83Q\x81[\x83\x80\x81" "E\x83o\x83\x8b   :%s");
    TRACE("\x83~\x83j\x83Q\x81[\x83\x80\x81" "E\x83W\x83\x83   :%s");
    TRACE("\x83~\x83j\x83Q\x81[\x83\x80\x81" "E\x83\x8f\x83" "C   :%s");
    TRACE("\x83~\x83j\x83Q\x81[\x83\x80\x81" "E\x83n\x83`   :%s");
    TRACE("\x83\x82\x83\x93\x83X\x83g\x83\x8d\x82\xc5\x91\xe5\x96\\\x82\xea :%s");
    TRACE("\x83\x82\x81[\x83O\x83\x8a\x83V\x83\x87\x83" "b\x83v   :%s");
    TRACE("\x83s\x83O\x83\x8c\x83" "b\x83g\x82\xc6\x98" "b\x82\xb5\x82\xbd :%s");
    TRACE("\x83I\x83" "E\x83\x8b\x82\xc6\x98" "b\x82\xb5\x82\xbd     :%s");
    TRACE("\x83\x8b\x81[\x82\xc6\x98" "b\x82\xb5\x82\xbd       :%s");
    TRACE("\x83" "C\x81[\x83\x88\x81[\x82\xc6\x98" "b\x82\xb5\x82\xbd   :%s");
    TRACE("\x83" "e\x83" "B\x83K\x81[\x82\xc6\x98" "b\x82\xb5\x82\xbd   :%s");
    TRACE("\x83\x89\x83r\x83" "b\x83g\x82\xc6\x98" "b\x82\xb5\x82\xbd   :%s");
    TRACE("100\x83G\x81[\x83J\x81[\x82\xcc\x90X\x8fo\x82\xbd:%s");
    TRACE("\xf9S------\xf9V\x83\\\x83\x89 \x93G\x82\xcc\x90\x94\xf9S------");
    TRACE("\x93G\x82\xcc\x8e\xed\x97\xde           :%s");
    TRACE("\x93G\x82\xcc\x90\x94             :%5d");
    TRACE("\x81\x9b\x83{\x83^\x83\x93\x82\xc5\x90\xdd\x92\xe8     :%s");
    TRACE("\xf9S------\xf9V\x83\x8a\x83N \x93G\x82\xcc\x90\x94\xf9S------");
    TRACE("\xf9S------\xf9V\x83\\\x83\x89 \x83J\x81[\x83h\xf9S------");
    TRACE("\x83J\x81[\x83h\x82\xcc\x8e\xed\x97\xde       :%s");
    TRACE("\x81\x9b\x83{\x83^\x83\x93\x82\xc5\x8e\xe6\x93\xbe     :%s");
    TRACE("\xf9S------\xf9V\x83\x8a\x83N \x83J\x81[\x83h\xf9S------");
    TRACE("\x83[\x83\x80\x83i\x83X           :%s");
    TRACE("\x83V\x83\x8b\x83o\x81[\x83\x8b         :%s");
    TRACE("\x83U\x83\x8b\x83" "f\x83" "B\x83\x93         :%s");
    TRACE("\x83T\x83" "C\x83N\x83X           :%s");
    TRACE("\x83" "f\x83~\x83" "b\x83N\x83X         :%s");
    TRACE("\x83\x8b\x83N\x83\\\x81[\x83h         :%s");
    TRACE("\x83\x8d\x83N\x83T\x83X           :%s");
    TRACE("\x83X\x83^\x81[\x83V\x81[\x83J\x81[     :%s");
    TRACE("\x83\x82\x83m\x83N\x83\x8d\x81[\x83\x80\t    :%s");
    TRACE("\x83t\x83H\x83\x8d\x81[\x83U\x83" "E\x83" "B\x83\x93\x83h :%s");
    TRACE("\x83q\x83h\x83" "D\x83\x93\x83h\x83\x89\x83S\x83\x93   :%s");
    TRACE("\x83t\x83H\x83g\x83\x93\x83" "f\x83o\x83" "b\x83K\x81[ :%s");
    TRACE("\x83{\x83\x93\x83h\x83I\x83u\x83t\x83\x8c\x83" "C\x83\x80 :%s");
}
namespace DWTEvent {
static BOOL debugGameInfomationParam(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        switch (inY) {
        case 1: CGameWork::setFlag(0, 0x83, DebugWork::g_dw.atelwork.iDistinctionClear); break;
        case 2: CGameWork::setFlag(0, 0x11, DebugWork::g_dw.atelwork.iGameKH2clear); break;
        case 3: CGameWork::setFlag(0, 0x9, DebugWork::g_dw.atelwork.iSoraClear); break;
        case 4: CGameWork::setFlag(0, 0x12, DebugWork::g_dw.atelwork.iRikuClear); break;
        case 7: CGameWork::setFlag(0, 0x82, DebugWork::g_dw.atelwork.iMahluciaKilled); break;
        case 8: CGameWork::setByte(0, 0x61, DebugWork::g_dw.atelwork.iPlayDifficultMode); break;
        }
    }
    return 1;
}
}
/* placeholder: rodata of the tables before DebugSetGameWorkParam__8DWTEventP12CDebugWindowii */
__asm__(".rdata\n");
inline void _roPh16() {
    TRACE("\x83L\x81[\x83\x8b\x81[\x83\x80        :%s");
    TRACE("\x83\x82\x81[\x83O\x83\x8a\x83V\x83\x87\x83" "b\x83v  :%s");
    TRACE("\x83t\x83\x8d\x83" "A\x88\xda\x93\xae        :%s");
    TRACE("\x83\x8f\x81[\x83v\x83|\x83" "C\x83\x93\x83g    :%s");
    TRACE("\x83Z\x81[\x83u\x83|\x83" "C\x83\x93\x83g    :%s");
    TRACE("\x83t\x83" "B\x81[\x83\x8b\x83h        :%s");
    TRACE("\x83\x8f\x81[\x83\x8b\x83h\x83Z\x83\x8c\x83N\x83g  :%s");
    TRACE("501   :%s");
    TRACE("502   :%s");
    TRACE("503   :%s");
    TRACE("504   :%s");
    TRACE("511   :%s");
    TRACE("512   :%s");
    TRACE("521   :%s");
    TRACE("522   :%s");
    TRACE("531   :%s");
    TRACE("541   :%s");
    TRACE("542   :%s");
    TRACE("543   :%s");
    TRACE("551   :%s");
    TRACE("552   :%s");
    TRACE("561   :%s");
    TRACE("571   :%s");
    TRACE("581   :%s");
    TRACE("582   :%s");
    TRACE("583   :%s");
    TRACE("584   :%s");
    TRACE("585   :%s");
    TRACE("586   :%s");
    TRACE("591   :%s");
    TRACE("592   :%s");
    TRACE("593   :%s");
    TRACE("594   :%s");
    TRACE("595   :%s");
    TRACE("596   :%s");
    TRACE("601   :%s");
    TRACE("602   :%s");
    TRACE("603   :%s");
    TRACE("604   :%s");
    TRACE("605   :%s");
    TRACE("606   :%s");
    TRACE("611   :%s");
    TRACE("612   :%s");
    TRACE("613   :%s");
    TRACE("614   :%s");
    TRACE("615   :%s");
    TRACE("621   :%s");
    TRACE("631   :%s");
    TRACE("001   :%s");
    TRACE("011   :%s");
    TRACE("012   :%s");
    TRACE("013   :%s");
    TRACE("021   :%s");
    TRACE("022   :%s");
    TRACE("023   :%s");
    TRACE("031   :%s");
    TRACE("032   :%s");
    TRACE("033   :%s");
    TRACE("041   :%s");
    TRACE("042   :%s");
    TRACE("043   :%s");
    TRACE("051   :%s");
    TRACE("052   :%s");
    TRACE("053   :%s");
    TRACE("061   :%s");
    TRACE("062   :%s");
    TRACE("063   :%s");
    TRACE("064   :%s");
    TRACE("071   :%s");
    TRACE("072   :%s");
    TRACE("073   :%s");
    TRACE("081   :%s");
    TRACE("082   :%s");
    TRACE("083   :%s");
    TRACE("084   :%s");
    TRACE("091   :%s");
    TRACE("092   :%s");
    TRACE("093   :%s");
    TRACE("101   :%s");
    TRACE("102   :%s");
    TRACE("103   :%s");
    TRACE("104   :%s");
    TRACE("111   :%s");
    TRACE("112   :%s");
    TRACE("113   :%s");
    TRACE("114   :%s");
    TRACE("115   :%s");
    TRACE("116   :%s");
    TRACE("117   :%s");
    TRACE("118   :%s");
    TRACE("121   :%s");
    TRACE("122   :%s");
    TRACE("123   :%s");
    TRACE("124   :%s");
    TRACE("125   :%s");
    TRACE("126   :%s");
    TRACE("131   :%s");
    TRACE("132   :%s");
    TRACE("133   :%s");
    TRACE("134   :%s");
    TRACE("135   :%s");
    TRACE("136   :%s");
    TRACE("137   :%s");
    TRACE("138   :%s");
    TRACE("139   :%s");
    TRACE("140   :%s");
    TRACE("151   :%s");
    TRACE("\x83n\x83" "C\x83W\x83\x83\x83\x93\x83v     :%s");
    TRACE("\x83O\x83\x89\x83" "C\x83h         :%s");
    TRACE("\x83X\x81[\x83p\x81[\x83O\x83\x89\x83" "C\x83h :%s");
    TRACE("\x83_\x81[\x83N\x83\x8a\x83N\x89\xbb     :%s");
    TRACE("\x83X\x83s\x81[\x83h\x83u\x83\x8c\x83" "C\x83N :%s");
    TRACE("\x83" "f\x83\x85\x83G\x83\x8b         :%s");
    TRACE("\x83h\x83i\x83\x8b\x83h     :%s");
    TRACE("\x83O\x81[\x83t\x83" "B\x81[   :%s");
    TRACE("\x83" "A\x83\x89\x83W\x83\x93     :%s");
    TRACE("\x83" "A\x83\x8a\x83G\x83\x8b     :%s");
    TRACE("\x83W\x83\x83\x83" "b\x83N     :%s");
    TRACE("\x83s\x81[\x83^\x81[\x83p\x83\x93 :%s");
    TRACE("\x83r\x81[\x83X\x83g     :%s");
    TRACE("\x89\xa4\x97l         :%s");
    TRACE("\x83v\x83\x8b\x81[\x83g     :%s");
    TRACE("\x83G\x83O\x83U\x83~\x83l\x81[\x83V\x83\x87\x83\x93 :%s");
    TRACE("\x83\x81\x83^\x83\x8b\x83`\x83\x87\x83R\x83{     :%s");
    TRACE("\x83\x89\x83" "C\x83I\x83\x93\x83n\x81[\x83g     :%s");
    TRACE("\x96\xf1\x91\xa9\x82\xcc\x82\xa8\x8e\xe7\x82\xe8       :%s");
    TRACE("\x89\xdf\x82\xac\x8b\x8e\x82\xe8\x82\xb5\x8ev\x82\xa2\x8fo   :%s");
    TRACE("\x83" "A\x83\x8b\x83" "e\x83}\x83" "E\x83G\x83|\x83\x93   :%s");
    TRACE("\x83_\x83" "C\x83" "A\x83\x82\x83\x93\x83h\x83_\x83X\x83g :%s");
    TRACE("\x95\xfb\x97\x83\x82\xcc\x93V\x8eg         :%s");
    TRACE("\x83t\x83@\x83" "C\x83" "A           :%s");
    TRACE("\x83T\x83\x93\x83_\x81[           :%s");
    TRACE("\x83O\x83\x89\x83r\x83" "f           :%s");
    TRACE("\x83X\x83g\x83" "b\x83v           :%s");
    TRACE("\x83G\x83" "A\x83\x8d             :%s");
    TRACE("\x83V\x83\x93\x83o             :%s");
    TRACE("\x83W\x81[\x83j\x81[           :%s");
    TRACE("\x83o\x83\x93\x83r             :%s");
    TRACE("\x83_\x83\x93\x83{             :%s");
    TRACE("\x83" "e\x83" "B\x83\x93\x83N           :%s");
    TRACE("\x83\x80\x81[\x83V\x83\x85\x81[         :%s");
    TRACE("\x83N\x83\x89\x83" "E\x83h           :%s");
    TRACE("\x83n\x83" "C\x83|\x81[\x83V\x83\x87\x83\x93     :%s");
    TRACE("\x83\x81\x83K\x83|\x81[\x83V\x83\x87\x83\x93     :%s");
    TRACE("\x83G\x81[\x83" "e\x83\x8b           :%s");
    TRACE("\x83\x81\x83K\x83G\x81[\x83" "e\x83\x8b       :%s");
    TRACE("\x83G\x83\x8a\x83N\x83T\x81[         :%s");
    TRACE("\x83\x89\x83X\x83g\x83G\x83\x8a\x83N\x83T\x81[   :%s");
    TRACE("\x83" "A\x83O\x83\x89\x83o           :%s");
    TRACE("\x83n\x83\x8d\x83" "E\x83" "B\x83\x93         :%s");
    TRACE("100\x83G\x81[\x83J\x81[        :%s");
    TRACE("\x96Y\x8bp\x82\xcc\x8f\xe9           :%s");
}
namespace DWTEvent {
static BOOL DebugSetGameWorkParam(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        switch (inY) {
        case 17: CGameWork::setFlag(1, 0x0, DebugWork::g_dw.atelwork.iPlayerDispOff); break;
        case 18: CGameWork::setFlag(1, 0x2, DebugWork::g_dw.atelwork.iBossKilled); break;
        case 21: CGameWork::setFlag(1, 0x5, DebugWork::g_dw.atelwork.iNotPcControl); break;
        case 22: CGameWork::setFlag(1, 0x6, DebugWork::g_dw.atelwork.iNotPause); break;
        case 26: CGameWork::setFlag(1, 0x25, DebugWork::g_dw.atelwork.iNotHartBtl); break;
        case 27: CGameWork::setFlag(1, 0x2C, DebugWork::g_dw.atelwork.iIsBtlTutorial); break;
        case 28: CGameWork::setFlag(1, 0x37, DebugWork::g_dw.atelwork.iIsWorldIntro); break;
        }
    }
    return 1;
}
}
namespace DWTEvent {
static INT debugWinNPCMainlCall(CDebugWindow* pObj, INT inParam) {
    if (inParam & 1) {
        DebugWork::g_dw.event.pClass = NULL;
        DebugWork::g_dw.event.pEventlineClass = NULL;
    }
    return 1;
}
}
/* placeholder: rodata of the tables before _TRACE__4ATELiPCce */
__asm__(".rdata\n");
inline void _roPh18() {
    TRACE("coord axis\x81" "F%s");
    TRACE("actor pos \x81" "F%s");
    TRACE("actor list...");
    TRACE("\x83" "C\x83x\x83\x93\x83g\x83\x89\x83" "C\x83\x93\x81" "F%s");
    TRACE("\x83t\x83\x89\x83O\x83`\x83" "F\x83" "b\x83N...");
    TRACE("PosX:  %.2f");
    TRACE("PosY:  %.2f");
    TRACE("PosZ:  %.2f");
    TRACE("RotX:  %.2f");
    TRACE("RotY:  %.2f");
    TRACE("RotZ:  %.2f");
}
#include "Form/Menu/TitleReportDllStarter.h"
__asm__(".rdata\n"); /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
void ATEL::_TRACE(BOOL bDisp, const CHAR* fmt, ...) {
    va_list args;

    if (bDisp) {
        printf("\x1b[%dm[ATEL]: ", 31);
        va_start(args, fmt);
        if (g_bTraceOnScreen) {
            vsprintf(_szTraceBuf, fmt, args);
            addTraceString(_szTraceBuf);
        } else {
            vprintf(fmt, args);
        }
        printf("\x1b[0m");
        va_end(args);
    }
}
CVSM::CVSM(CAtel& atel, BOOL bTrace)
    : m_Atel(atel), m_GAptr(0), m_FAptr(0), m_Pctr(0), m_SP(0), m_Freg(0), m_InsCount(0), m_MaxSD(0), m_MinFP(0x4000),
      m_MaxPC(0), m_CallC(0), m_MaxActorCount(0), m_ActorCount(0), m_pIseg(NULL), m_pDseg(NULL), m_pCurrentActor(NULL) {
    m_nFlag = 0;
    m_bTrace = bTrace;
    initActor();
    m_fEventTrapStartWait = 20.0f;
    m_fTalkWait = 0.0f;
}
CVSM::~CVSM() {
    TRACE("CVSM::~CVSM()\n");
    if (m_pIseg) com_free(m_pIseg);
    if (m_pDseg) com_free(m_pDseg);
    CChatMan::instance().reqCommand(0x40F, NULL, 0);
    for (int i = 0; i < m_ActorCount; i++) {
        delete m_ActorTbl[i]._pEventTrap;
    }
}
void CVSM::initActor() {
    Actor* p = m_ActorTbl;
    for (INT i = 0; i < 64; i++, p++) {
        p->_nID = -1;
        p->_nRsrcID = -1;
        p->_pAvatar = NULL;
        p->_pEventTrap = NULL;
        p->_fTalkableRadius = 0.0f;
    }
    m_ActorStateFnc[0] = &CVSM::acStatNop;
    m_ActorStateFnc[1] = &CVSM::acStatExec;
    m_ActorStateFnc[2] = &CVSM::acStatNop;
    m_ActorStateFnc[3] = &CVSM::acStatWait;
    m_ActorStateFnc[4] = &CVSM::acStatRWait;
    m_ActorStateFnc[5] = &CVSM::acStatCWait;
    m_ActorStateFnc[6] = &CVSM::acStatIWait;
    m_ActorStateFnc[7] = &CVSM::acStatPWait;
    m_ActorStateFnc[8] = &CVSM::acStatSWait;
    m_ActorStateFnc[9] = &CVSM::acStatMWait;
    m_ActorStateFnc[10] = &CVSM::acStatCPWait;
    m_ActorStateFnc[11] = &CVSM::acStatSNWait;
    m_ActorStateFnc[12] = &CVSM::acStatMSWait;
    m_ActorStateFnc[13] = &CVSM::acStatShopWait;
    m_ActorStateFnc[14] = &CVSM::acStatDropWait;
    m_ActorStateFnc[15] = &CVSM::acStatEnemyLoadWait;
    m_ActorStateFnc[16] = &CVSM::acStatRCWait;
    m_ActorStateFnc[17] = &CVSM::acStatEQWait;
    m_ActorStateFnc[18] = &CVSM::acStatWKWait;
    m_ActorStateFnc[19] = &CVSM::acStatXWait;
    m_ActorStateFnc[20] = &CVSM::acStatBSWait;
    m_ActorStateFnc[21] = &CVSM::acStatBAWait;
    m_ActorStateFnc[22] = &CVSM::acStatDLWait;
    m_ActorStateFnc[23] = &CVSM::acStatBGWait;
    m_ActorStateFnc[24] = &CVSM::acStatActorWait;
    m_ActorStateFnc[25] = &CVSM::acStatEternityWait;
    m_ActorStateFnc[26] = &CVSM::acStatRsrcLoadWait;
    m_ActorStateFnc[27] = &CVSM::acStatCardWndWait;
    m_ActorStateFnc[28] = &CVSM::acStatTechWndWait;
    m_ActorStateFnc[29] = &CVSM::acStatWorldCardWndWait;
    m_ActorStateFnc[30] = &CVSM::acStatCrossFadeWait;
    CGameWork::setFlag(1, 0x26, FALSE);
    CGameWork::setFlag(1, 0x38, FALSE);
}
__asm__(".rdata\n"); /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
void CVSM::loadFromMem(const BYTE* pAddr) {
    INT* pIntPtr;
    BYTE* p;
    INT i;
    _TRACE_(TRUE, "Load I-Seg\n");
    pIntPtr = (INT*)pAddr;
    INT nMagic = *pIntPtr;
    pAddr += 4;
    if (nMagic != 0x6362612F) {
        _TRACE(TRUE, "ATEL \x83R\x83\x93\x83p\x83" "C\x83\x89\x82\xcc\x83o\x81[\x83W\x83\x87\x83\x93\x82\xaa\x88\xe1\x82\xa2\x82\xdc\x82\xb7\n");
        return;
    }
    pIntPtr = (INT*)pAddr;
    m_MaxActorCount = *pIntPtr++;
    if (m_MaxActorCount > 64) {
        _TRACE(TRUE, "Actor-ID \x93o\x98^\x90\x94\x83I\x81[\x83o\x81[\x83t\x83\x8d\x81[\n");
#line 343
        AssertApp(__FILE__, __LINE__, "0", 0);
    }
    for (INT n = 0; n < m_MaxActorCount; n++) {
        m_ActorTbl[n]._nID = *pIntPtr++;
    }
    pAddr += 4 + 64 * 4;
    pIntPtr = (INT*)pAddr;
    m_MaxPC = *pIntPtr;
    pAddr += 4;
    _TRACE_(m_bTrace, "MaxPC:%x\n", m_MaxPC);
    if (m_pIseg) com_free(m_pIseg);
    m_pIseg = (INSTR*)com_malloc_d(m_MaxPC * sizeof(INSTR), __FILE__ "(357)", (UINT)-1);
#line 358
    if (!(m_pIseg)) AssertApp(__FILE__, __LINE__, "m_pIseg", 0);
    p = (BYTE*)m_pIseg;
    for (INT j = 0; j < m_MaxPC * sizeof(INSTR); j++) {
        *p++ = *pAddr++;
    }
    pIntPtr = (INT*)pAddr;
    m_GAptr = *pIntPtr;
    _TRACE_(m_bTrace, "GAptr:%x\n", m_GAptr);
    pAddr += 4;
    if (m_GAptr > 0x1000) {
        _TRACE(TRUE, "\x83" "f\x81[\x83^\x83Z\x83O\x83\x81\x83\x93\x83g \x83I\x81[\x83o\x81[\x83t\x83\x8d\x81[\n");
#line 372
        AssertApp(__FILE__, __LINE__, "0", 0);
    }
    if (m_pDseg) com_free(m_pDseg);
    m_pDseg = (BYTE*)com_malloc_d(0x4000, __FILE__ "(376)", (UINT)-1);
#line 377
    if (!(m_pDseg)) AssertApp(__FILE__, __LINE__, "m_pDseg", 0);
    p = m_pDseg;
    for (INT j = 0; j < m_GAptr; j++) {
        *p++ = *pAddr++;
    }
    m_ActorCount = *pAddr;
    pAddr += 4;
    if (m_ActorCount > 64) {
        _TRACE(TRUE, "Actor \x93o\x98^\x90\x94\x83I\x81[\x83o\x81[\x83t\x83\x8d\x81[\n");
#line 390
        AssertApp(__FILE__, __LINE__, "0", 0);
    }
    for (INT n = 0; n < m_ActorCount; n++) {
        p = (BYTE*)&m_ActorTbl[n]._ATP;
        for (UINT k = 0; k < sizeof(ATP); k++) {
            *p++ = *pAddr++;
        }
    }
    setFlag(3);
    if (m_bTrace) dispActorTag(m_ActorCount, m_ActorTbl);
}
void CVSM::deleteActorAll() {}
void CVSM::checkEvents() {
    ETMoveData data;
    if (CMainApp::isPauseNow()) return;
    if (CGameWork::getByte(1, 0x140)) return;
    if (m_fEventTrapStartWait <= 0.0f) {
        if (!CGameWork::getFlag(1, 0x26) && g_pPlayer) {
            data._fRadius = 0.5f;
            data._vNowPos = g_pPlayer->getPos();
            data._vOldPos = g_pPlayer->getOldPos();
            for (int i = 0; i < m_ActorCount; i++) {
                CEventTrap* p = m_ActorTbl[i]._pEventTrap;
                if (!p) continue;
                INT ret = p->checkTrap(data);
                if (!ret) continue;
                switch (ret) {
                case 3: reqSSI("@_to", i, 3); break;
                case 5: reqSSI("@_tf", i, 2); break;
                case 9: reqSSI("@_as", i, 5); break;
                case 1: reqSSI("@_tc", i, 4); break;
                default:
                    if (ret & 8) reqSSI("@_as", i, 5);
                    if (ret & 2) reqSSI("@_to", i, 3);
                    if (ret & 4) reqSSI("@_tf", i, 2);
                    break;
                }
            }
        }
    } else {
        m_fEventTrapStartWait -= CTaskMan::getSysTask(0x1A)->getFrameRate() * 0.020000001f;
    }
    if (m_fTalkWait <= 0.0f) {
        if (!g_pPlayer) return;
        if (g_pPlayer->getPlayerState() == 1 || g_pPlayer->getPlayerState() == 2 ||
            g_pPlayer->getPlayerState() == 3 || g_pPlayer->getPlayerState() == 4) {
            if (CPadHandler::isTrgTalk() && (m_nFlag & 8)) {
                INT Index = searchLockOnNpc();
                if (Index != -1 && reqSSI("@_tk", Index, 5)) {
                    TRACE("\x89\xef\x98" "b\x8aJ\x8en\x81" "F%s\n", m_ActorTbl[Index]._ATP.name);
                    CGameWork::setFlag(1, 4, TRUE);
                    CGameWork::setFlag(1, 3, TRUE);
                    CGameWork::setFlag(1, 5, TRUE);
                    m_fTalkWait = 10.0f;
                }
            }
        }
    } else {
        m_fTalkWait -= CTaskMan::getSysTask(0x1A)->getFrameRate() * 0.020000001f;
    }
}
/* Parked: 8 bytes short; only the first return differs: the original keeps `if (!g_pPlayer) return -1;` as
   bnez over "b end; li -1", ours lets jump2 cross-jump it into the loop's final `li -1` (beqz v1 to it; -dJ dump:
   all four -1 returns share one label). $LM tags: 642 the test, 643 the return, 645-647 isNowLockon (Player.h)
   and the 0/-1 test, 650 getTask, 651/653 the early returns, 655 the cast, 658-662 the loop, 665 return -1.
   tried: wrapping the body in if (g_pPlayer) {}, getLockonHandle() vs isNowLockon() + m_hTarget, a break loop
   with a separate found test (longer), separate-line returns; retry: a CPlayer* local (also assigned in the test),
   if/else return, the loop in an inline helper (then only the inner -1 stays apart), tools/permuter.py 15 min (8318
   candidates, score 8 unchanged). */
#if 0
INT CVSM::searchLockOnNpc() {
    if (!g_pPlayer)
        return -1;
    HANDLE handle = g_pPlayer->isNowLockon();
    if (handle == 0 || handle == -1) handle = g_pPlayer->m_hTarget;
    CTask* lockTarget = CTaskMan::getTask(handle);
    if (!lockTarget) return -1;
    if (lockTarget->getTypeID() != 0x4000) return -1;
    CAvatar* avater = static_cast<CAvatar*>(lockTarget);
    for (int i = 0; i < m_ActorCount; i++) {
        if (m_ActorTbl[i]._pAvatar == avater) return i;
    }
    return -1;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", searchLockOnNpc__Q24ATEL4CVSM);
#endif
void CVSM::clearReq() {
    RTP* p = m_ReqBuf;
    for (INT i = 0; i < 64; i++, p++) {
        p->rq_lv = 0;
        p->rq_actor = 0;
        p->rq_tag = 0;
        p->rq_m_actor = 0xFF;
        p->status = 0;
        p->wait_key = 0;
    }
    m_nReqBufPtr = 0;
}
void CVSM::setReq(const RTP& rq) {
    INT nPtr = -1;
    RTP* p = m_ReqBuf;
    for (INT i = 0; i < m_nReqBufPtr; i++, p++) {
        if (rq.rq_actor == p->rq_actor) {
            if (rq.rq_lv < p->rq_lv) {
                TRACE("\x8c\xbb\x8d\xdd\x82\xcc\x83\x8a\x83N\x83G\x83X\x83g\x82\xe6\x82\xe8\x8e\xe3\x82\xa2\x83\x8a\x83N\x83G\x83X\x83g\x94\xad\x8ds\x82\xb3\x82\xea\x82\xdc\x82\xb5\x82\xbd\x81" "B\x96\xb3\x8e\x8b\x82\xb5\x82\xdc\x82\xb7\x81" "B\n");
                _TRACE(m_bTrace, ">>> buf_pass a:%d l:%d\n", rq.rq_actor, rq.rq_lv);
                return;
            }
            nPtr = i;
            break;
        }
    }
    if (m_nReqBufPtr >= 64) {
        _TRACE(TRUE, "ReqBuf MAX\n");
        return;
    }
    if (nPtr < 0) {
        p = &m_ReqBuf[m_nReqBufPtr];
        m_nReqBufPtr++;
    }
    p->rq_actor = rq.rq_actor;
    p->rq_tag = rq.rq_tag;
    p->rq_lv = rq.rq_lv;
    p->rq_m_actor = rq.rq_m_actor;
    p->wait_key = rq.wait_key;
}
void CVSM::tuneReq() {
    if (m_nReqBufPtr > 0) {
        RTP* p = m_ReqBuf;
        for (INT i = 0; i < m_nReqBufPtr; i++, p++) {
            UINT nActIdx = p->rq_actor;
            ATP& atp = m_ActorTbl[nActIdx]._ATP;
            INT nTag = p->rq_tag;
            UINT nStat = atp.status;
            if (nStat == 4 && nTag == 0xFF) {
                atp.status = 1;
                _TRACE(m_bTrace, "mode-exec\n");
            } else if (nTag == atp.rq_tag) {
                if (p->rq_m_actor != 0xFF) {
                    TRACE("\x93\xaf\x82\xb6\x83\x8a\x83N\x83G\x83X\x83g\x82\xaa\x94\xad\x8ds\x82\xb3\x82\xea\x82\xdc\x82\xb5\x82\xbd\x81" "B\x96\xb3\x8e\x8b\x82\xb5\x82\xdc\x82\xb7\x81" "B\n");
                    atp.status = 1;
                    atp.rq_tag = nTag;
                    atp.rq_lv = p->rq_lv;
                    atp.address = atp.loc[nTag];
                    atp.rq_m_actor = p->rq_m_actor;
                    atp.wait_key = p->wait_key;
                    _TRACE(m_bTrace, "EW--!!tune");
                }
            } else if (nStat == 11 && atp.tname[nTag] == 0x72665F40) {
                atp.wait_timer = 0;
                _TRACE(m_bTrace, "snatch wait PASS !!\n");
            } else {
                if (nStat == 3 || nStat == 20) atp.status = 1;
                atp.rq_tag = nTag;
                atp.rq_lv = p->rq_lv;
                atp.address = atp.loc[nTag];
                atp.rq_m_actor = p->rq_m_actor;
                atp.wait_key = p->wait_key;
            }
        }
        m_nReqBufPtr = 0;
    }
}
void CVSM::exec(TIMEQ nTQ) {
    m_nFrameRate = nTQ;
    if (m_WinCloseTimer > 0) {
        if ((m_WinCloseTimer -= nTQ) < 0) {
            m_WinCloseTimer = 0;
        }
    }
    clearFlag(4);
    for (m_pCurrentActor = m_ActorTbl, m_nCurrentActorIdx = 0; m_nCurrentActorIdx < m_ActorCount; m_nCurrentActorIdx++, m_pCurrentActor++) {
        execActor();
    }
    if (!(m_nFlag & 4)) checkEvents();
    tuneReq();
    clearReq();
}
void CVSM::execInit() {
    _TRACE(m_bTrace, "execInit ActorCount = %d\n", m_ActorCount);
    m_ReadyActorCount = 0;
    m_WinCloseTimer = 0;
    m_nFlag |= 8;
    clearReq();
    m_pCurrentActor = m_ActorTbl;
    for (INT i = 0; i < m_ActorCount; i++, m_pCurrentActor++) {
        reqN(*m_pCurrentActor, 0, 3, i == 0);
    }
}
/* Parked: 3 lines, one store: the original issues the m_Freg store first after the Freg/prev-actor join, ours
   last (sched2 -fsched-verbose: our store gets priority 7 against 8 for the m_SP store and the m_Pctr load, so
   reorg puts it into the execCode jal slot instead of `move a0,s0`). Everything else matches: the PMF state call
   (m_ActorStateFnc as BOOL (CVSM::*[31])()), #line 844 assert, the switch tree needs a case ATEL_RET_REQ.
   tried: if/ternary/assign-in-test for m_Freg, status before/after it, m_SP before it, getPC() for the
   argument, a split nRet declaration, tools/permuter.py 5 min (loose score 1); retry: sched2 (forward list
   scheduler) has 4 insns ready at t=1 after the join (m_Freg store, li 1, m_Pctr load, a0 copy) and gives the
   memory unit to the m_Pctr argument load; the original picks the m_Freg store. tools/permuter.py 15 min (8163
   candidates, score 1 unchanged). */
#if 0
void CVSM::execActor() {
    ATP& atp = m_pCurrentActor->_ATP;
    INT nState = atp.status;
#line 844
    if (!((ATEL_REQ <= nState) && (_ATEL_REQ_MAX > nState))) AssertApp(__FILE__, __LINE__, "(ATEL_REQ <= nState) && (_ATEL_REQ_MAX > nState)", 0);
    if ((this->*m_ActorStateFnc[nState])()) {
        atp.status = 1;
        INT f;
        if ((f = atp.Freg) == 0) f = m_pCurrentActor[-1]._ATP.Freg;
        m_Freg = f;
        m_SP = 1;
        eATEL_REQ_RET nRet;
        nRet = execCode(getPC());
        atp.Freg = m_Freg;
        switch (nRet) {
        case ATEL_RET_MAIN: {
            if (atp.rq_tag == 0) {
                atp.status = 6;
                reqSSD(1, m_nCurrentActorIdx, 5);
                if (++m_ReadyActorCount == m_ActorCount) {
                    clearFlag(1);
                    _TRACE(m_bTrace, "mada->ok pc:%d\n", atp.next_address);
                }
            } else if (atp.rq_tag != 1) {
                reqSSD(1, m_nCurrentActorIdx, 1);
            }
            INT nActw = atp.rq_m_actor;
            if (nActw != 0xFF) {
                atp.rq_m_actor = 0xFF;
                m_ActorTbl[nActw]._ATP.wait_timer = 0;
            }
            break;
        }
        case ATEL_RET_REQ:
            break;
        case ATEL_RET_ERROR:
            atp.status = 2;
            break;
        }
    }
}
#else
/* placeholder: rodata of execActor__Q24ATEL4CVSM */
__asm__(".rdata\n");
inline void _roPhExecActor() {
    TRACE("(ATEL_REQ <= nState) && (_ATEL_REQ_MAX > nState)");
    TRACE("mada->ok pc:%d\n");
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", execActor__Q24ATEL4CVSM);
#endif
__asm__(".rdata\n"); /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
/* addr goes through a reference: its stack store then is no fixed scalar for the scheduler, which keeps it
   before the _cReg load as in the original */
static inline void setAddr(INT& a, INT v) { a = v; }
eATEL_REQ_RET CVSM::execCode(INT nAddr) {
    INT addr;
    INT op;
    eATEL_REQ_RET reqR;
    INT execR;
    m_Pctr = nAddr;
    while (TRUE) {
        if (m_Pctr < 0 || m_Pctr >= m_MaxPC) {
            _TRACE(TRUE, "Illegal instr. address : %d\n", m_Pctr);
            return ATEL_RET_ERROR;
        }
        if ((UINT)m_SP >= 100) {
            _TRACE(TRUE, "Illegal Stack pointer %d\n", m_SP);
            return ATEL_RET_STACK_ERROR;
        }
        op = m_pIseg[m_Pctr]._cOp;
        setAddr(addr, m_pIseg[m_Pctr]._nAddr);
        if (m_pIseg[m_Pctr++]._cReg) {
            addr += m_Freg;
        }
        m_InsCount++;
        if (m_MaxSD < m_SP) m_MaxSD = m_SP;
        if (op < 0) execR = 4;
        else if (op < 0x3E) execR = execStdCode(op, addr, reqR);
        else if (op < 100) execR = execExt1Code(op, reqR);
        else if (op < 110) execR = 4;
        else if (op < 0xA8) execR = execExt2Code(op, reqR);
        else if (op < 0x113) execR = execExt3Code(op, reqR);
        else execR = 4;
        switch (execR) {
        case 1:
            continue;
        case 2:
            m_SP--;
            continue;
        case 3:
            return reqR;
        }
        _TRACE(TRUE, "Illegal Op. code at loc. %d\n", m_Pctr);
        return ATEL_RET_ERROR;
    }
}
INT CVSM::execStdCode(INT op, INT& addr, eATEL_REQ_RET& ret) {
    INT* ip;
    FLOAT* fp;
    char mes[128];
    INT r;
    INT i1;
    INT i2;
    INT i3;
    INT c;
    BYTE* to;
    BYTE* from;
    INT d;

    do {
        r = 1;
        switch (op) {
        case 1:
            addr = m_S[m_SP - 1].I;
            m_S[m_SP - 1].I = m_S[m_SP].I;
            m_pDseg[addr] = m_S[m_SP].I;
            goto pop;
        case 2:
            ip = (INT*)(m_pDseg + m_S[m_SP - 1].I);
            m_S[m_SP - 1].I = m_S[m_SP].I;
            *ip = m_S[m_SP].I;
            goto pop;
        case 3:
            fp = (FLOAT*)(m_pDseg + m_S[m_SP - 1].I);
            m_S[m_SP - 1].F = m_S[m_SP].F;
            *fp = m_S[m_SP].F;
            goto pop;
        case 4:
            m_S[m_SP - 1].I += m_S[m_SP].I;
            goto pop;
        case 5:
            m_S[m_SP - 1].F += m_S[m_SP].F;
            goto pop;
        case 6:
            m_S[m_SP].I += addr;
            break;
        case 7:
            m_S[m_SP - 1].I -= m_S[m_SP].I;
            goto pop;
        case 8:
            m_S[m_SP - 1].F -= m_S[m_SP].F;
            goto pop;
        case 9:
            m_S[m_SP].I -= addr;
            break;
        case 10:
            m_S[m_SP - 1].I *= m_S[m_SP].I;
            goto pop;
        case 11:
            m_S[m_SP - 1].F *= m_S[m_SP].F;
            goto pop;
        case 12:
            m_S[m_SP].I *= addr;
            break;
        case 13:
            if (m_S[m_SP].I == 0) {
                _TRACE(TRUE, "zero divide\n");
                return -1;
            }
            m_S[m_SP - 1].I /= m_S[m_SP].I;
            goto pop;
        case 14:
            if (m_S[m_SP].F == 0.0f) {
                _TRACE(TRUE, "zero divide\n");
                return -1;
            }
            m_S[m_SP - 1].F /= m_S[m_SP].F;
            goto pop;
        case 15:
            m_S[m_SP].I /= addr;
            break;
        case 16:
            if (m_S[m_SP].I == 0) {
                _TRACE(TRUE, "zero divide\n");
                return -1;
            }
            m_S[m_SP - 1].I %= m_S[m_SP].I;
            goto pop;
        case 18:
            m_S[m_SP].I %= addr;
            break;
        case 19:
            m_S[m_SP].I = -m_S[m_SP].I;
            break;
        case 20:
            m_S[m_SP].F = -m_S[m_SP].F;
            break;
        case 21:
            m_S[m_SP - 1].I = m_S[m_SP - 1].I < m_S[m_SP].I ? -1 : m_S[m_SP - 1].I > m_S[m_SP].I;
            goto pop;
        case 22:
            m_S[m_SP - 1].I = m_S[m_SP - 1].F < m_S[m_SP].F ? -1 : m_S[m_SP - 1].F > m_S[m_SP].F ? 1 : 0;
            goto pop;
        case 23:
            m_S[m_SP].I = m_S[m_SP].I < addr ? -1 : m_S[m_SP].I > addr;
            break;
        case 24:
            m_SP++;
            m_S[m_SP].I = m_S[m_SP - 1].I;
            break;
        case 25:
            m_S[++m_SP].I = m_pDseg[addr];
            break;
        case 26:
            m_S[++m_SP].I = *(INT*)(m_pDseg + addr);
            break;
        case 27:
            m_S[++m_SP].F = *(FLOAT*)(m_pDseg + addr);
            break;
        case 28:
            m_S[++m_SP].I = addr;
            break;
        case 29:
            m_S[m_SP].I = m_pDseg[m_S[m_SP].I];
            break;
        case 30:
            m_S[m_SP].I = *(INT*)(m_pDseg + m_S[m_SP].I);
            break;
        case 31:
            m_S[m_SP].F = *(FLOAT*)(m_pDseg + m_S[m_SP].I);
            break;
        case 33:
            m_pDseg[addr] = m_S[m_SP].I;
            goto pop;
        case 34:
            *(INT*)(m_pDseg + addr) = m_S[m_SP].I;
            goto pop;
        case 35:
            *(FLOAT*)(m_pDseg + addr) = m_S[m_SP].F;
            goto pop;
        case 36:
            m_Freg = addr;
            break;
        case 37:
            m_Freg += addr;
            if (m_Freg > 0x4000) {
                r = 3;
                _TRACE(TRUE, "Run-time stack  overflow Freg:%x addr:%x pc:%d\n", m_Freg, addr, m_Pctr);
                ret = ATEL_RET_ERROR;
            }
            break;
        case 38:
            m_Freg -= addr;
            if (m_Freg < m_MinFP) {
                if (m_Freg <= m_GAptr) {
                    r = 3;
                    _TRACE(TRUE, "Run-time stack underflow\n");
                    ret = ATEL_RET_STACK_ERROR;
                } else {
                    m_MinFP = m_Freg;
                }
            }
            break;
        case 39:
            m_S[m_SP].F = (FLOAT)m_S[m_SP].I;
            break;
        case 40:
            m_S[m_SP].I = (INT)m_S[m_SP].F;
            break;
        case 42:
            if (m_S[m_SP--].I < 0) m_Pctr = addr;
            break;
        case 43:
            if (m_S[m_SP--].I <= 0) m_Pctr = addr;
            break;
        case 44:
            if (m_S[m_SP--].I == 0) m_Pctr = addr;
            break;
        case 45:
            if (m_S[m_SP--].I != 0) m_Pctr = addr;
            break;
        case 46:
            if (m_S[m_SP--].I >= 0) m_Pctr = addr;
            break;
        case 47:
            if (m_S[m_SP--].I > 0) m_Pctr = addr;
            break;
        case 41:
            m_Pctr = addr;
            break;
        case 48:
            m_S[++m_SP].I = m_Pctr;
            m_Pctr = addr;
            m_CallC++;
            break;
        case 49:
            m_Pctr = m_S[m_SP--].I;
            break;
        case 50:
            r = 3;
            ret = ATEL_RET_HALT;
            break;
        case 57:
            ret = ATEL_RET_MAIN;
            r = 3;
            break;
        case 116:
            r = 3;
            ret = ATEL_RET_MAINS;
            break;
        case 58:
            r = 3;
            reqNS(*m_pCurrentActor, (CHAR*)(m_pDseg + m_S[m_SP--].I), 5);
            ret = ATEL_RET_REQ;
            break;
        case 59:
            acWaitReq(*m_pCurrentActor, m_S[m_SP--].I, m_Pctr);
            ret = ATEL_RET_WAIT;
            r = 3;
            break;
        case 60:
            i1 = m_S[m_SP--].I;
            i2 = m_S[m_SP--].I;
            i3 = m_S[m_SP--].I;
            TRACE("REQ:%s,%s\n", m_pDseg + i1, m_pDseg + i2);
            reqSS((CHAR*)(m_pDseg + i1), (CHAR*)(m_pDseg + i2), i3);
            break;
        case 61:
            reqSSW((CHAR*)(m_pDseg + m_S[m_SP--].I), (CHAR*)(m_pDseg + m_S[m_SP--].I), m_S[m_SP--].I, m_Pctr);
            ret = ATEL_RET_RWAIT;
            r = 3;
            break;
        case 51:
            c = m_S[m_SP--].I;
            to = (BYTE*)m_S[m_SP--].I;
            from = (BYTE*)m_S[m_SP--].I;
            for (; c > 0; c--) *to++ = *from++;
            break;
        case 52:
            m_S[m_SP].I = (BYTE*)com_malloc_d(m_S[m_SP].I, __FILE__ "(1121)", (UINT)-1) - m_pDseg;
#line 1122
            if (!(m_S[m_SP].I)) AssertApp(__FILE__, __LINE__, "(m_S[m_SP].I)", 0);
            break;
        case 53:
            com_free(m_pDseg + m_S[m_SP--].I);
            break;
        case 55:
            d = m_S[m_SP--].I;
            sprintf(mes, (CHAR*)(m_pDseg + m_S[m_SP--].I), d);
            _TRACE(TRUE, "%s", mes);
            break;
        case 56:
            sprintf(mes, (CHAR*)(m_pDseg + m_S[m_SP--].I));
            _TRACE(TRUE, "%s", mes);
            break;
        default:
            return 4;
        case 32:
            goto pop;
        case 0:
            break;
        }
        continue;
    pop:
        r = 2;
        break;
    } while (r == 0);
    return r;
}
struct AvatarInitParam {
    FVECTOR _vPos;
    FVECTOR _vAngle;
    BOOL _bOnGround;
};
struct AvatarMoveParam {
    union {
        FVECTOR _vTarget;
        struct {
            FLOAT _fCx;
            FLOAT _fCz;
            FLOAT _fTargetRotY;
        };
        struct {
            CHAR* _pszPathFile;
            USHORT _nMotion;
        };
    };
    UINT _nType;
    union {
        FLOAT _fTime;
        FLOAT _fSpeed;
    };
};
struct AvatarRotateParam {
    union {
        FVECTOR _vTarget;
        UINT _nTargetID;
    };
    UINT _nType;
    FLOAT _fTime;
};
struct AvatarMotionParam {
    FLOAT _fPlaySpeed;
    INT _iChangeHokanRate;
    SHORT _nMotionID;
    SHORT _nFlag;
};
struct ChatWndCreateParam {
    CPointI pos;
    CSizeI size;
    UINT wndColor;
    LPCTSTR pText;
    UINT textColor;
    UINT fontSize;
    UINT wndArrangement;
    CPointI cyobiPos;
    UINT waitFrame;
    UINT fontStyle;
    ChatWndCreateParam() { cyobiPos.m_y = 0; cyobiPos.m_x = 0; }
};
struct ImageWndCreateParam {
    CPointI pos0;
    CSizeI size0;
    CPointI pos1;
    CSizeI size1;
    LPCTSTR pTexName;
    INT kind;
    FLOAT fRate;
    UINT color0;
    UINT color1;
    ImageWndCreateParam() { kind = 0; }
};
FLOAT RotTransPers(FVECTOR& outV, const FMATRIX& inM, const FVECTOR& inV);
INT CVSM::execExt1Code(INT op, eATEL_REQ_RET& ret) {
    INT nID;
    INT c;
    INT t;
    INT r;

    do {
        r = 1;
        switch (op) {
        case 0x3E:
            nID = m_S[m_SP].I;
            if (m_pCurrentActor->_pAvatar) {
                m_pCurrentActor->_pAvatar->reqCommand(0x38, NULL, nID);
                _TRACE(m_bTrace, "--- ACTCMD_SET_FACE --- %d\n", nID);
            }
            break;
        case 0x3F:
            acWaitPad(*m_pCurrentActor, m_S[m_SP].I, m_Pctr);
            ret = ATEL_RET_PWAIT; r = 3;
            break;
        case 0x40:
            nID = m_S[m_SP].I;
            ret = ATEL_RET_EWAIT; r = 3;
            break;
        case 0x41:
            break;
        case 0x43: {
            c = m_S[m_SP - 1].I;
            t = m_S[m_SP].I;
            _TRACE(m_bTrace, "ACC c:%d t:%d id:%d\n", c, t, nID);
            ATP& atp = m_pCurrentActor->_ATP;
            m_Atel.createAvatar(c, *m_pCurrentActor, t);
            atp.status = 5;
            atp.next_address = m_Pctr;
            ret = ATEL_RET_CWAIT; r = 3;
            break;
        }
        case 0x45: {
            AvatarInitParam param;
            param._bOnGround = m_S[m_SP--].I;
            param._vAngle.w = 1.0f;
            param._vAngle.z = DEG2RAD(m_S[m_SP--].F);
            param._vAngle.y = DEG2RAD(m_S[m_SP--].F);
            param._vAngle.x = DEG2RAD(m_S[m_SP--].F);
            param._vPos.w = 1.0f;
            param._vPos.z = m_S[m_SP--].F;
            param._vPos.y = m_S[m_SP--].F;
            param._vPos.x = m_S[m_SP].F;
            if (m_pCurrentActor->_pAvatar) {
                _TRACE(m_bTrace, "Avatar init ACI %.1f %.1f %.1f \n", param._vPos.x, param._vPos.y, param._vPos.z);
                m_pCurrentActor->_pAvatar->reqCommand(2, &param, 0);
            }
            break;
        }
        case 0x46: {
            FVECTOR vPos;
            vPos.w = 1.0f;
            vPos.z = m_S[m_SP--].F;
            vPos.y = m_S[m_SP--].F;
            vPos.x = m_S[m_SP].F;
            if (m_pCurrentActor->_pAvatar) {
                _TRACE(m_bTrace, "ACP %.1f %.1f %.1f \n", vPos.x, vPos.y, vPos.z);
                m_pCurrentActor->_pAvatar->reqCommand(10, &vPos, 0);
            }
            break;
        }
        case 0x47:
        case 0x48:
            c = m_S[m_SP - 1].I;
            t = m_S[m_SP].I;
            TRACE("ATEL:\x83\x8a\x83\\\x81[\x83X\x83\x8d\x81[\x83h: %d, %d\n", c, t);
            m_pCurrentActor->_nRsrcID = c * 10000 + t;
            CRcf::reqLoadRCF(c, t, 0, NULL, 0);
            break;
        case 0x49: {
            FVECTOR v;
            AvatarMoveParam param;
            v.w = m_S[m_SP--].F;
            v.z = m_S[m_SP--].F;
            v.y = m_S[m_SP--].F;
            v.x = m_S[m_SP--].F;
            c = m_S[m_SP].I;
            if (m_pCurrentActor->_pAvatar) {
                param._nType = c;
                param._fTime = v.w;
                switch (c & 0xF) {
                case 3:
                case 4:
                    param._fCx = v.x;
                    param._fCz = v.z;
                    param._fTargetRotY = v.y * 0.017453292f;
                    break;
                default:
                    param._vTarget = v;
                    param._vTarget.w = 0.0f;
                    break;
                }
                if (m_pCurrentActor->_pAvatar) {
                    _TRACE(m_bTrace, "--- START MOVE-ACTOR Type::%x ---\n", c);
                    m_pCurrentActor->_pAvatar->reqCommand(0xD, &param, 0);
                }
            }
            break;
        }
        case 0x4A:
            m_S[m_SP].I = m_pCurrentActor->_pAvatar ? isAvatarBusy(m_S[m_SP].I, m_pCurrentActor->_pAvatar) : 0;
            break;
        case 0x4B: {
            c = m_S[m_SP - 1].I;
            AvatarMotionParam param;
            param._nMotionID = c & 0xFFF;
            param._fPlaySpeed = m_S[m_SP].F;
            param._nFlag = (c >> 12) & 3;
            if (m_pCurrentActor->_pAvatar) m_pCurrentActor->_pAvatar->reqCommand(0x13, &param, 0);
            break;
        }
        case 0x4C: {
            LineTrapSrc src;
            src._vP[1].w = 1.0f;
            src._vP[1].z = m_S[m_SP--].F;
            src._vP[1].y = m_S[m_SP--].F;
            src._vP[1].x = m_S[m_SP--].F;
            src._vP[0].w = 1.0f;
            src._vP[0].z = m_S[m_SP--].F;
            src._vP[0].y = m_S[m_SP--].F;
            src._vP[0].x = m_S[m_SP].F;
            m_Atel.createEventTrap(src, *m_pCurrentActor);
            break;
        }
        case 0x4D: {
            BoxTrapSrc src;
            src._fRotY = DEG2RAD(m_S[m_SP--].F);
            src._fHeight = m_S[m_SP--].F;
            src._fDepth = m_S[m_SP--].F;
            src._fWidth = m_S[m_SP--].F;
            src._vPos.w = 1.0f;
            src._vPos.z = m_S[m_SP--].F;
            src._vPos.y = m_S[m_SP--].F;
            src._vPos.x = m_S[m_SP].F;
            m_Atel.createEventTrap(src, *m_pCurrentActor);
            break;
        }
        case 0x4E:
            CSceneInfo::reqChangeScene(m_S[m_SP - 1].I, m_S[m_SP].I, -1, 1, 1);
            break;
        case 0x4F: {
            AvatarRotateParam param;
            param._vTarget.w = 0.0f;
            param._vTarget.z = m_S[m_SP--].F;
            param._vTarget.y = m_S[m_SP--].F;
            param._vTarget.x = m_S[m_SP--].F;
            param._fTime = m_S[m_SP--].F;
            param._nType = m_S[m_SP].I;
            if (param._nType & 0x100) {
                param._nType ^= 0x100;
                param._fTime = param._fTime * 0.016666668f;
            }
            if (param._nType < 3 || param._nType == 5 || param._nType == 7 || param._nType == 9 || param._nType == 6 || param._nType == 8 || param._nType == 10) {
                param._vTarget.z = DEG2RAD(param._vTarget.z);
                param._vTarget.y = DEG2RAD(param._vTarget.y);
                param._vTarget.x = DEG2RAD(param._vTarget.x);
            }
            if (param._nType == 2) param._nTargetID = 1;
            if (m_pCurrentActor->_pAvatar) m_pCurrentActor->_pAvatar->reqCommand(0xE, &param, 0);
            break;
        }
        case 0x50:
            m_pCurrentActor->_fTalkableRadius = m_S[m_SP].F;
            _TRACE(m_bTrace, "--- ACTOR-SET-TALKRAD ---\n");
            break;
        case 0x51: {
            AvatarRotateParam param;
            c = m_S[m_SP].I;
            param._fTime = m_S[m_SP - 1].F;
            param._nTargetID = c & 0xFFFFFF;
            param._nType = (c & 0x80000000) ? 4 : 2;
            if (m_pCurrentActor->_pAvatar) {
                m_pCurrentActor->_pAvatar->reqCommand(0xE, &param, 0);
                _TRACE(m_bTrace, "--- START ROATE-ACTOR-Target ---TYPE[%d]\n", param._nType);
            }
            break;
        }
        case 0x52: {
            FVECTOR vDir;
            vDir.w = 0.0f;
            vDir.z = DEG2RAD(m_S[m_SP--].F);
            vDir.y = DEG2RAD(m_S[m_SP--].F);
            vDir.x = DEG2RAD(m_S[m_SP].F);
            if (m_pCurrentActor->_pAvatar) m_pCurrentActor->_pAvatar->reqCommand(0xC, &vDir, 0);
            break;
        }
        case 0x53:
            c = CGameWork::getInt(0, m_S[m_SP].I);
            m_S[++m_SP].I = c;
            break;
        case 0x54:
            c = m_S[m_SP - 1].I;
            t = m_S[m_SP].I;
            CGameWork::setInt(0, c, t);
            break;
        case 0x55:
            c = CGameWork::getByte(0, m_S[m_SP].I);
            m_S[++m_SP].I = (BYTE)c;
            break;
        case 0x56:
            c = m_S[m_SP - 1].I;
            t = m_S[m_SP].I;
            CGameWork::setByte(0, c, t);
            break;
        case 0x57:
            c = CGameWork::getFlag(0, m_S[m_SP].I);
            m_S[++m_SP].I = c;
            break;
        case 0x58:
            c = m_S[m_SP - 1].I;
            t = m_S[m_SP].I;
            CGameWork::setFlag(0, c, t);
            break;
        case 0x59: {
            FVECTOR vPos;
            c = m_S[m_SP--].I;
            DEG2RAD(m_S[m_SP--].F);
            DEG2RAD(m_S[m_SP--].F);
            vPos.w = 1.0f;
            vPos.z = m_S[m_SP--].F;
            vPos.y = m_S[m_SP--].F;
            vPos.x = m_S[m_SP].F;
            break;
        }
        case 0x5A:
            m_S[m_SP].I = player_ctrl(m_S[m_SP].I);
            break;
        case 0x5B: {
            INT t;
            INT i11 = m_S[m_SP--].I;
            INT i10 = m_S[m_SP--].I;
            INT i9 = m_S[m_SP--].I;
            INT i8 = m_S[m_SP--].I;
            INT i7 = m_S[m_SP--].I;
            INT i6 = m_S[m_SP--].I;
            INT i5 = m_S[m_SP--].I;
            INT i4 = m_S[m_SP--].I;
            INT i3 = m_S[m_SP--].I;
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP--].I;
            LPCSTR pText = (LPCSTR)(m_pDseg + m_S[m_SP].I);
            FVECTOR vPos;
            ATP& atp = m_pCurrentActor->_ATP;
            ChatWndCreateParam cpar;
            cpar.pos.setPoint(i2, i3);
            cpar.size.setSize(i4, i5);
            cpar.wndColor = i6;
            cpar.pText = "";
            cpar.textColor = i7;
            cpar.fontSize = i8 & 0xFF;
            cpar.wndArrangement = i9 & 3;
            cpar.waitFrame = i10;
            cpar.fontStyle = i11;
            if (i9 >= 0x80 && m_pCurrentActor) {
                FVECTOR vWorld;
                FVECTOR vPos;
                getAvatar(m_pCurrentActor->_nID)->getNowPosition(vPos);
                RotTransPers(vPos, CWorldEnv::getWorldScreenMatrix(), vWorld);
                UINT inX = (UINT)(vPos.x - 1792.0f);
                UINT inY = (UINT)(vPos.y - 1880.0f);
                TRACE("chibi-pos %d : %d\n", inX, inY);
                cpar.cyobiPos.setPoint(inX, inY);
            } else {
                cpar.cyobiPos.setPoint(0, 0);
            }
            close_dest_se(i1);
            if (i9 < 0x10) {
                CChatMan::instance().reqCommand(0x3E8, &cpar, i1);
            } else {
                CChatMan::instance().reqCommand(0x3E9, &cpar, i1);
            }
            c = (i9 >> 4) & 3;
            if (i9 >= 0x10) {
                INT nIdx;
                switch (c) {
                case 1:
                case 3:
                    nIdx = 1;
                    break;
                case 2:
                case 4:
                    nIdx = 2;
                    break;
                default:
                    nIdx = 0;
                    break;
                }
                CChatMan::instance().reqCommand(0x3F1, &nIdx, i1);
            }
            if (i8 > 0x10000) {
                CPointI pos((UINT)i8 >> 24, (i8 >> 16) & 0xFF);
                CChatMan::instance().reqCommand(0x3F3, &pos, i1);
            }
            CChatMan::instance().reqCommand(0x3EF, (void*)pText, i1);
            WinOpenSE(0);
            CChatMan::instance().reqCommand(0x3EA, NULL, i1);
            atp.wait_key = i1;
            atp.status = 12;
            atp.next_address = m_Pctr;
            ret = ATEL_RET_MS_WAIT; r = 3;
            break;
        }
        case 0x5C:
        case 0x5D: {
            ImageWndCreateParam m_IWCP;
            FLOAT f3 = m_S[m_SP].F;
            INT i3 = m_S[m_SP--].I;
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP].I;
            switch (i1) {
            case 0:
                TRACE("MESS_CMD OPEN \n");
                WinOpenSE(0);
                CChatMan::instance().reqCommand(0x3EA, NULL, i2);
                break;
            case 1:
                TRACE("MESS_CMD CLOSE \n");
                WinCloseSE();
                CChatMan::instance().reqCommand(0x3EB, NULL, i2);
                break;
            case 2:
                TRACE("MESS_CMD DESTROY \n");
                close_dest_se(i2);
                break;
            case 3:
                TRACE("MESS_CMD CLR TEXT \n");
                CChatMan::instance().reqCommand(0x3EE, NULL, i2);
                break;
            case 4:
                TRACE("MESS_CMD TEXT DELEY \n");
                CChatMan::instance().reqCommand(0x3F0, &i3, i2);
                break;
            case 5:
                TRACE("MESS_CMD WND STYLE \n");
                CChatMan::instance().reqCommand(0x3F1, &i3, i2);
                break;
            case 6:
                TRACE("MESS_CMD CHG WND STYLE\n");
                CChatMan::instance().reqCommand(0x3F2, &i3, i2);
                break;
            case 7: {
                TRACE("MESS_CMD SET TEXT MARGIN\n");
                CPointI pos;
                pos.setPoint((i3 >> 8) & 0xFF, i3 & 0xFF);
                CChatMan::instance().reqCommand(0x3F3, &pos, i2);
                break;
            }
            case 8:
                TRACE("MESS_CMD SET FONT SIZE\n");
                CChatMan::instance().reqCommand(0x3F4, &i3, i2);
                break;
            case 9: {
                TRACE("MESS_CMD SET FONT SPACE\n");
                CSizeI size;
                size.setSize((i3 >> 8) & 0xFF, i3 & 0xFF);
                CChatMan::instance().reqCommand(0x3F5, &size, i2);
                break;
            }
            case 10:
                TRACE("MESS_CMD SET TEXT STYLE\n");
                CChatMan::instance().reqCommand(0x3F6, &i3, i2);
                break;
            case 11:
                TRACE("MESS_CMD SET TEXT COLOR\n");
                CChatMan::instance().reqCommand(0x3F7, &i3, i2);
                break;
            case 12:
                TRACE("MESS_CMD SET TEXT EXCOLOR\n");
                CChatMan::instance().reqCommand(0x3F8, &i3, i2);
                break;
            case 13:
                TRACE("MESS_CMD SETSELECTOFS\n");
                CChatMan::instance().reqCommand(0x402, &i3, i2);
                break;
            case 14:
                TRACE("MESS_CMD CHGCURSORCOLOR\n");
                CChatMan::instance().reqCommand(0x403, &i3, i2);
                break;
            case 15:
                TRACE("MESS_CMD SETDISPINTERVAL\n");
                CChatMan::instance().reqCommand(0x404, &i3, i2);
                break;
            case 16:
                TRACE("MESS_CMD CHANGETEXT_NEXT\n");
                CChatMan::instance().reqCommand(0x406, NULL, i2);
                break;
            case 17:
                TRACE("MESS_CMD CHAT_ENABLEDISP\n");
                CChatMan::instance().reqCommand(0x407, &i3, i2);
                break;
            case 18:
                TRACE("MESS_CMD IMAGE_PHASEINI\n");
                m_IWCP.pos0.m_x = 0x80000000;
                m_IWCP.pos1.m_x = 0x80000000;
                m_IWCP.size0.m_w = -1;
                m_IWCP.size1.m_w = -1;
                m_IWCP.color0 = 0x80FFFFFF;
                m_IWCP.color1 = 0xFFFFFF;
                m_IWCP.kind = 1;
                m_IWCP.fRate = f3;
                CChatMan::instance().reqCommand(0x401, &m_IWCP, i2);
                break;
            case 20:
                TRACE("MESS_CMD IMAGE_PHASEINI2\n");
                m_IWCP.pos0.m_x = 0x80000000;
                m_IWCP.pos1.m_x = 0x80000000;
                m_IWCP.size0.m_w = -1;
                m_IWCP.size1.m_w = -1;
                m_IWCP.color0 = 0x80808080;
                m_IWCP.color1 = 0x808080;
                m_IWCP.kind = 1;
                m_IWCP.fRate = f3;
                CChatMan::instance().reqCommand(0x401, &m_IWCP, i2);
                break;
            case 21:
                TRACE("MES_DEST_ALL\n");
                CChatMan::instance().reqCommand(0x40F, NULL, 0);
                break;
            case 22:
                TRACE("MES_LAYER_CHANGE\n");
                CChatMan::instance().reqCommand(0x410, NULL, i3);
                break;
            case 23:
                TRACE("MES_SET_C_FADEINDELAY\n");
                CChatMan::instance().reqCommand(0x411, &f3, i2);
                break;
            case 24:
                TRACE("MES_SET_T_FADEOUTTIME\n");
                CChatMan::instance().reqCommand(0x412, &f3, i2);
                break;
            default:
                TRACE("MESS_CMD bad param \n");
                break;
            }
            break;
        }
        case 0x5E:
            break;
        case 0x5F:
            c = CGameWork::getInt(1, m_S[m_SP].I);
            m_S[++m_SP].I = c;
            break;
        case 0x60:
            c = m_S[m_SP - 1].I;
            t = m_S[m_SP].I;
            CGameWork::setInt(1, c, t);
            break;
        case 0x61:
            c = CGameWork::getByte(1, m_S[m_SP].I);
            m_S[++m_SP].I = (BYTE)c;
            break;
        case 0x62:
            c = m_S[m_SP - 1].I;
            t = m_S[m_SP].I;
            CGameWork::setByte(1, c, t);
            break;
        case 0x63:
            c = CGameWork::getFlag(1, m_S[m_SP].I);
            m_S[++m_SP].I = c;
            break;
        default:
            return 4;
        }
    } while (r == 0);
    return r;
}
namespace ATEL {
struct ChatWndSetWordsParam;
struct ChatWndCreateCTDParam {
    CPointI pos;
    CPointI cyobiPos;
    UINT ctdID;
    UINT waitFrame;
    ChatWndSetWordsParam* pCwswp;
    UINT nLine;
    ChatWndCreateCTDParam() { nLine = 0; }
};
}
struct CameraManCreateParam {
    FVECTOR cameraPos;
    FVECTOR targetPos;
    CHAR* pPathName;
    FLOAT fChangeRate;
    SHORT changeKind;
    SHORT changeKind2;
    SHORT changeFlag;
    SHORT pathDateKind;
    SHORT pathDateTargetID;
    SHORT pathDateLocalID;
    FLOAT fFullFrame;
    FLOAT fMidFrame;
    FLOAT fStartEndFrame;
    FLOAT fOffsetX;
    FLOAT fOffsetY;
    FLOAT fOffsetZ;
    CVSM* pCvsm;
};
struct AvatarMotionParamEx2 {
    UINT _nRsrcID;
    UINT _nInfo;
    UINT _nMotionID;
    FLOAT _fPlaySpeed;
    UINT _nHokanFrame;
    AvatarMotionParamEx2() { _nRsrcID = 0; _nInfo = 0; _nMotionID = 0; _fPlaySpeed = 1.0f; _nHokanFrame = 0; }
};
/* Math3D.h:137 and coord.h:730 in the original ($LM tags); the volatile abs.s keeps the original order (a scheduling barrier) */
static inline FLOAT FABS(FLOAT f) { FLOAT r; __asm__ volatile("abs.s %0, %1" : "=f"(r) : "f"(f)); return r; }
static inline void setFVector(FVECTOR& v, FLOAT _x, FLOAT _y, FLOAT _z) { v.x = _x; v.y = _y; v.z = _z; v.w = 1.0f; }
INT CVSM::execExt2Code(INT op, eATEL_REQ_RET& ret) {
    INT nID;
    INT c;
    INT t;
    INT r;

    do {
        r = 1;
        switch (op) {
        case 0x5F:
            c = CGameWork::getInt(1, m_S[m_SP].I);
            m_S[++m_SP].I = c;
            break;
        case 0x60:
            t = m_S[m_SP - 1].I;
            c = m_S[m_SP].I;
            CGameWork::setInt(1, t, c);
            break;
        case 0x61:
            c = CGameWork::getByte(1, m_S[m_SP].I);
            m_S[++m_SP].I = c & 0xFF;
            break;
        case 0x62:
            t = m_S[m_SP - 1].I;
            c = m_S[m_SP].I;
            CGameWork::setByte(1, t, c);
            break;
        case 0x63:
            c = CGameWork::getFlag(1, m_S[m_SP].I);
            m_S[++m_SP].I = c;
            break;
        case 0x6E:
            t = m_S[m_SP - 1].I;
            c = m_S[m_SP].I;
            CGameWork::setFlag(1, t, c);
            break;
        case 0x6F: {
            float f6 = m_S[m_SP--].F;
            int i5 = m_S[m_SP--].I;
            float f4 = m_S[m_SP--].F;
            float f3 = m_S[m_SP--].F;
            float f2 = m_S[m_SP--].F;
            int i1 = m_S[m_SP].I;
            CameraManCreateParam CMCP;
            TRACE("CCC %d %f %f %f\n", i1, f2, f3, f4);
            if (CCameraMan::instance().reqCommand(0x452, NULL, i1) == 1) CCameraMan::instance().reqCommand(0x44F, NULL, i1);
            CMCP.pathDateTargetID = 0;
            CMCP.fOffsetY = 1.5f;
            setFVector(CMCP.cameraPos, f2, f3, f4);
            CMCP.changeKind = i5 & 0xFF;
            CMCP.changeKind2 = (i5 >> 8) & 0xFF;
            CMCP.pCvsm = this;
            if (op == 0xA0) f6 += 1000.0f;
            CMCP.fChangeRate = f6;
            CMCP.pathDateLocalID = 0;
            CMCP.changeFlag = 0;
            CCameraMan::instance().reqCommand(0x44C, &CMCP, i1);
            break;
        }
        case 0x70:
            nID = m_S[m_SP--].I;
            if (cam_ctrl(m_S[m_SP].I, nID, c)) {
                m_S[m_SP].I = c;
            }
            break;
        case 0x71: {
            INT c = m_S[m_SP - 1].I;
            INT t = m_S[m_SP].I;
            INT n = 0;
            if (t & 0x1000) n = (t & 0x1000) >> 12;
            CGameWork::setByte(1, 0, n);
            t &= 0xFFF;
            if (t < 0x100) n++;
            CChatMan::instance().reqCommand(0x3F9, &n, c);
            acWaitSelectPad(*m_pCurrentActor, c, t, m_Pctr);
            ret = ATEL_RET_SWAIT; r = 3;
            break;
        }
        case 0x72: {
            INT i1 = m_S[m_SP--].I;
            INT i2 = m_S[m_SP--].I;
            CHAR* pText = (CHAR*)(m_pDseg + m_S[m_SP].I);
            CameraManCreateParam CMCP;
            if (CCameraMan::instance().reqCommand(0x452, NULL, i2) == 1) CCameraMan::instance().reqCommand(0x44F, NULL, i2);
            CMCP.pPathName = pText;
            CMCP.changeKind = i1 & 0xFF;
            CMCP.changeKind2 = (i1 >> 8) & 0xFF;
            CMCP.pathDateTargetID = 0;
            CMCP.pathDateLocalID = 0;
            CMCP.changeFlag = 0;
            CCameraMan::instance().reqCommand(0x44E, &CMCP, i2);
            break;
        }
        case 0x74:
            ret = ATEL_RET_MAINS; r = 3;
            break;
        case 0x75:
            c = searchActor((CHAR*)(m_pDseg + m_S[m_SP].I));
            m_S[m_SP].I = c;
            break;
        case 0x76:
        case 0xA1: {
            float f9 = m_S[m_SP--].F;
            int i1 = m_S[m_SP--].I;
            float f7 = m_S[m_SP--].F;
            float f6 = m_S[m_SP--].F;
            float f5 = m_S[m_SP--].F;
            float f4 = m_S[m_SP--].F;
            float f3 = m_S[m_SP--].F;
            float f2 = m_S[m_SP--].F;
            int i0 = m_S[m_SP].I;
            CameraManCreateParam CMCP;
            TRACE("CCT %d %f %f %f\n", i0, f2, f3, f4);
            TRACE("    %f %f %f %d %f\n", f5, f6, f7, i1, f9);
            if (CCameraMan::instance().reqCommand(0x452, NULL, i0) == 1) CCameraMan::instance().reqCommand(0x44F, NULL, i0);
            CMCP.fOffsetY = 1.5f;
            CMCP.pathDateTargetID = 0;
            setFVector(CMCP.cameraPos, f2, f3, f4);
            setFVector(CMCP.targetPos, f5, f6, f7);
            CMCP.changeKind = i1 & 0xFF;
            CMCP.changeKind2 = (i1 >> 8) & 0xFF;
            if (op == 0xA1) f9 += 1000.0f;
            CMCP.fChangeRate = f9;
            CMCP.pathDateLocalID = 0;
            CMCP.changeFlag = 0;
            CMCP.pCvsm = this;
            CCameraMan::instance().reqCommand(0x44D, &CMCP, i0);
            break;
        }
        case 0x78: {
            INT nPan = m_S[m_SP--].I;
            INT nVol = m_S[m_SP--].I;
            INT nSeq = m_S[m_SP].I;
            if (nPan == -1) nPan = 0x40;
            m_S[m_SP].I = CSound::playSE(nSeq, nVol, nPan);
            m_SeInf._nHanle = m_S[m_SP].I;
            m_SeInf._nSeq = nSeq;
            m_SeInf._nVol = nVol;
            break;
        }
        case 0x79: {
            INT nSeq = m_S[m_SP--].I;
            INT nHandle = m_S[m_SP].I;
            CSound::stopSE(nHandle, nSeq);
            m_SeInf._nHanle = 0;
            m_SeInf._nSeq = 0;
            m_SeInf._nVol = 0;
            break;
        }
        case 0x7A: {
            INT nVol = m_S[m_SP--].I;
            CHAR* pszFile = (CHAR*)(m_pDseg + m_S[m_SP].I);
            TRACE("BGM_PLAY %s %d\n", pszFile, nVol);
            CSound::playBGM(pszFile, nVol, 0);
            break;
        }
        case 0x7D:
            acWaitMes(*m_pCurrentActor, m_S[m_SP].I, m_Pctr);
            ret = ATEL_RET_MWAIT; r = 3;
            break;
        case 0x7E: {
            FLOAT f = m_S[m_SP--].F;
            c = m_S[m_SP].I;
            m_S[m_SP].I = player_ctrl(c, f);
            break;
        }
        case 0x7F: {
            INT i4 = m_S[m_SP--].I;
            FLOAT f3 = m_S[m_SP--].F;
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP].I;
            TRACE("MOE %d : %d : %f : %d\n", i1, i2, f3, i4);
            AvatarMotionParamEx2 param;
            param._nRsrcID = i1;
            param._nMotionID = i2;
            param._fPlaySpeed = f3;
            param._nInfo = (i4 >> 12) & 7;
            m_pCurrentActor->_pAvatar->reqCommand(0x15, &param, 0);
            break;
        }
        case 0x80:
            acWaitCam(*m_pCurrentActor, m_S[m_SP].I, m_Pctr);
            ret = ATEL_RET_CWAIT; r = 3;
            break;
        case 0x82:
            acWaitSnatch(*m_pCurrentActor, m_S[m_SP].I, m_Pctr);
            ret = ATEL_RET_SWAIT; r = 3;
            break;
        case 0x83: {
            FLOAT f = m_S[m_SP].F;
            CCameraMan::instance().reqCommand(0x456, &f, 0);
            TRACE("CAMCMD_CHANGE_GAKAKU %f\n", f);
            break;
        }
        case 0x85:
            info_disp(m_S[m_SP].I);
            break;
        case 0x86: {
            INT nVol = m_S[m_SP--].I;
            INT nSeq = m_S[m_SP].I;
            FVECTOR vPos;
            m_pCurrentActor->_pAvatar->getNowPosition(vPos);
            m_S[m_SP].I = CSound::playSE(nSeq, vPos, nVol);
            break;
        }
        case 0x87: {
            INT nTime = m_S[m_SP--].I;
            INT nVol = m_S[m_SP--].I;
            INT nSeq = m_S[m_SP--].I;
            INT nHandle = m_S[m_SP].I;
            CSound::setSEVolume(nHandle, nSeq, nVol, nTime);
            m_SeInf._nHanle = nHandle;
            m_SeInf._nSeq = nSeq;
            m_SeInf._nVol = nVol;
            break;
        }
        case 0x88: {
            INT nTime = m_S[m_SP--].I;
            INT nPan = m_S[m_SP--].I;
            INT nSeq = m_S[m_SP--].I;
            INT nHandle = m_S[m_SP].I;
            if (nPan == -1) nPan = 0x40;
            CSound::setSEPan(nHandle, nSeq, nPan, nTime);
            break;
        }
        case 0x89: {
            INT nTime = m_S[m_SP--].I;
            INT nVol = m_S[m_SP--].I;
            INT nPan = m_S[m_SP--].I;
            INT nSeq = m_S[m_SP--].I;
            INT nHandle = m_S[m_SP].I;
            if (nVol == -1) nPan = 0x40;
            CSound::setSEPanAndVol(nHandle, nSeq, nPan, nVol, nTime);
            break;
        }
        case 0x8A: {
            INT nTime = m_S[m_SP--].I;
            INT nSeq = m_S[m_SP--].I;
            INT nHandle = m_S[m_SP].I;
            CSound::fadeoutSE(nHandle, nSeq, nTime);
            break;
        }
        case 0x8B: {
            INT nVol = m_S[m_SP--].I;
            INT nSeq = m_S[m_SP--].I;
            INT nName = m_S[m_SP].I;
            FVECTOR vPos;
            INT nIdx = searchActor((CHAR*)(m_pDseg + nName));
            m_ActorTbl[nIdx]._pAvatar->getNowPosition(vPos);
            m_S[m_SP].I = CSound::playSE(nSeq, vPos, nVol);
            break;
        }
        case 0x8C:
        case 0x8D:
        case 0x8E: {
            FLOAT fRate = m_S[m_SP--].F;
            INT nKind = m_S[m_SP].I;
            m_Atel.setScreenFader(nKind, fRate, 2);
            TRACE("--- WNDCMD_FADE_CREATE ---\n");
            break;
        }
        case 0x8F:
            CAtel::clearScreenFader(-1);
            TRACE("--- WNDCMD_FADE_DESTROY ---\n");
            break;
        case 0x90: {
            INT nVol = m_S[m_SP--].I;
            INT nTime = m_S[m_SP--].I;
            CHAR* pszFile = (CHAR*)(m_pDseg + m_S[m_SP].I);
            TRACE("BGM_FADE_IN %s %d %d\n", pszFile, nTime, nVol);
            CSound::fadeinBGM(pszFile, nTime, nVol, 0);
            break;
        }
        case 0x91: {
            INT nTime = m_S[m_SP--].I;
            TRACE("BGM_FADE_OUT %d \n", nTime);
            CSound::fadeoutBGM(0, nTime);
            CSound::getCurrentBGMHandle();
            break;
        }
        case 0x92: {
            INT nVol = m_S[m_SP--].I;
            INT nTime = m_S[m_SP--].I;
            CHAR* pszFile = (CHAR*)(m_pDseg + m_S[m_SP].I);
            TRACE("BGM_CROSS_FADE %s %d %d\n", pszFile, nTime, nVol);
            if (CSound::getStateBGM(CSound::getCurrentBGMHandle())) {
                CSound::crossFadeinBGM(pszFile, nTime, nVol, 0);
            } else {
                CSound::playBGM(pszFile, nTime, 0);
                TRACE("\x82" "a\x82" "f\x82l\x8d\xc4\x90\xb6\x82\xb5\x82\xc4\x82\xa2\x82\xc8\x82\xa2\x82\xcc\x82\xc9\x83N\x83\x8d\x83X\x83t\x83" "F\x81[\x83h\x97v\x8b\x81\x81" "F\x83N\x83\x8d\x83X\x83t\x83" "F\x81[\x83h\x83L\x83\x83\x83\x93\x83Z\x83\x8b\x81" "FBGM\x8d\xc4\x90\xb6\n");
            }
            break;
        }
        case 0x98: {
            FLOAT fTime = m_S[m_SP--].F;
            nID = searchActor((CHAR*)(m_pDseg + m_S[m_SP].I));
            if (nID > 0 && m_pCurrentActor->_pAvatar) {
                AvatarRotateParam param;
                param._nType = 2;
                param._fTime = fTime;
                param._nTargetID = nID;
                m_pCurrentActor->_pAvatar->reqCommand(0xE, &param, 0);
            }
            break;
        }
        case 0x9A:
            m_pCurrentActor->_ATP.status = 13;
            m_pCurrentActor->_ATP.next_address = m_Pctr;
            ret = ATEL_RET_SHOP; r = 3;
            break;
        case 0x9C:
            t = m_S[m_SP].I;
            if (t == 0) c = m_pCurrentActor->_ATP.wait_key;
            else if (t != 1) _TRACE(m_bTrace, "NG AWK param\n");
            m_S[m_SP].I = c;
            break;
        case 0x9D: {
            FLOAT z2 = m_S[m_SP--].F;
            FLOAT y2 = m_S[m_SP--].F;
            FLOAT x2 = m_S[m_SP--].F;
            FLOAT z1 = m_S[m_SP--].F;
            FLOAT y1 = m_S[m_SP--].F;
            FLOAT x1 = m_S[m_SP].F;
            BoxTrapSrc src;
            src._fWidth = FABS(x2 - x1);
            src._fHeight = FABS(y2 - y1);
            src._fDepth = FABS(z2 - z1);
            src._vPos.x = (x1 + x2) * 0.5f;
            src._vPos.y = y1;
            src._vPos.z = (z1 + z2) * 0.5f;
            src._vPos.w = 1.0f;
            src._fRotY = 0.0f;
            m_Atel.createEventTrap(src, *m_pCurrentActor);
            break;
        }
        case 0x9E:
            c = m_S[m_SP--].I;
            m_S[m_SP].I = actor_ref(m_S[m_SP].I, c);
            break;
        case 0xA2: {
            INT i3 = m_S[m_SP--].I;
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP].I;
            ChatWndCreateCTDParam param;
            param.pos.setPoint(-1, -1);
            param.ctdID = i2;
            param.waitFrame = i3 & 0x8000FFFF;
            param.pCwswp = NULL;
            ctd_cal(m_pCurrentActor->_pAvatar, i2, i3, &param);
            close_dest_se(i1);
            CChatMan::instance().reqCommand(0x405, &param, i1);
            WinOpenSE(i2);
            CChatMan::instance().reqCommand(0x3EA, &param, i1);
            if (i3 & 0x20000000) {
                INT n = 0;
                CChatMan::instance().reqCommand(0x413, &n, i1);
            }
            m_pCurrentActor->_ATP.wait_key = i1;
            m_pCurrentActor->_ATP.status = 12;
            m_pCurrentActor->_ATP.next_address = m_Pctr;
            ret = ATEL_RET_MS_WAIT; r = 3;
            break;
        }
        case 0xA3: {
            INT i5 = m_S[m_SP--].I;
            INT i4 = m_S[m_SP--].I;
            INT i3 = m_S[m_SP--].I;
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP--].I;
            INT nIdx = getActorIndex((CHAR*)(m_pDseg + m_S[m_SP].I));
            ChatWndCreateCTDParam param;
            if (i4 < 0) {
                i5 = -1;
                i4 = -1;
            }
            param.ctdID = i2;
            param.waitFrame = i3 & 0x8000FFFF;
            param.pos.setPoint(i4, i5);
            param.pCwswp = NULL;
            if (nIdx != -1) ctd_cal(getAvatar(nIdx), i2, i3, &param);
            close_dest_se(i1);
            CChatMan::instance().reqCommand(0x405, &param, i1);
            WinOpenSE(i2);
            CChatMan::instance().reqCommand(0x3EA, &param, i1);
            if (i3 & 0x20000000) {
                INT n = 0;
                CChatMan::instance().reqCommand(0x413, &n, i1);
            }
            if (i3 & 0x40000000) {
                INT n = 0;
                CChatMan::instance().reqCommand(0x404, &n, i1);
            } else {
                INT n = 60;
                CChatMan::instance().reqCommand(0x404, &n, i1);
            }
            m_pCurrentActor->_ATP.wait_key = i1;
            m_pCurrentActor->_ATP.status = 12;
            m_pCurrentActor->_ATP.next_address = m_Pctr;
            ret = ATEL_RET_MS_WAIT; r = 3;
            break;
        }
        case 0xA4: {
            INT i4 = m_S[m_SP--].I;
            INT i3 = m_S[m_SP--].I;
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP--].I;
            CHAR* pText = (CHAR*)(m_pDseg + m_S[m_SP].I);
            CameraManCreateParam CMCP;
            if (i2 >= 0) i2 = searchActor((CHAR*)(m_pDseg + i2));
            if (i3 >= 0) i3 = searchActor((CHAR*)(m_pDseg + i3));
            if (CCameraMan::instance().reqCommand(0x452, NULL, i1) == 1) CCameraMan::instance().reqCommand(0x44F, NULL, i1);
            CMCP.fOffsetY = 1.5f;
            CMCP.pPathName = pText;
            CMCP.pathDateTargetID = i2;
            CMCP.changeKind = (i4 >> 16) & 0xFF;
            CMCP.changeKind2 = (UINT)i4 >> 24;
            CMCP.pathDateKind = i4;
            CMCP.pathDateLocalID = i3;
            CMCP.pCvsm = this;
            CCameraMan::instance().reqCommand(0x459, &CMCP, i1);
            TRACE("CAMCMD_CREATE_PATHDATA %s\n", pText);
            break;
        }
        case 0xA5: {
            t = m_S[m_SP--].I;
            c = m_S[m_SP].I;
            m_pCurrentActor->_ATP.wait_key = t;
            m_pCurrentActor->_ATP.status = 12;
            m_pCurrentActor->_ATP.next_address = m_Pctr;
            ret = ATEL_RET_MS_WAIT; r = 3;
            break;
        }
        case 0xA6: {
            FLOAT fCycle = m_S[m_SP--].F;
            FLOAT fPower = m_S[m_SP--].F;
            INT nTime = m_S[m_SP--].I;
            INT nType = m_S[m_SP].I;
            TRACE("CAMCMD_ADD_QUAKE \n");
            CameraQuakeParam param;
            param = (CameraQuakeParam){ nTime, fPower, fCycle };
            if (nType == 0) {
                CCameraMan::instance().reqCommand(0x45A, &param, 0);
            } else {
                CCameraMan::instance().reqCommand(0x45B, &param, 0);
            }
            break;
        }
        case 0x77:
        case 0x7B:
        case 0x7C:
        case 0x93:
        case 0x94:
        case 0x95:
        case 0x96:
        case 0x97:
        case 0x9B:
        case 0x9F:
        case 0xA0:
        case 0xA7:
            break;
        default:
            return 4;
        }
    } while (r == 0);
    return r;
}
namespace ATEL {
struct ChatWndSetWordsParam {
    union {
        UINT ctdID[4];
        char* pWord[4];
    };
};
}
/* Prize.h:30 in the original: PRIZE_PARAM's constructor */
struct PRIZE_PARAM_C : PRIZE_PARAM {
    PRIZE_PARAM_C() { uiFlag = 0; uiPrizeFlag = 0; }
};
struct FogColorStruct {
    INT color;
    INT AnimFrame;
};
struct FogNomalStruct {
    FLOAT Param;
    INT AnimFrame;
};
struct BlurStruct {
    INT param;
    INT AnimFrame;
};
struct BlurAnimStruct {
    FVECTOR param;
    INT AnimFrame;
};
struct CrossFadeColorStruct {
    INT color;
    INT AnimFrame;
};
struct AvatarMotionParamC {
    FLOAT _fPlaySpeed;
    INT _iChangeHokanRate;
    SHORT _nMotionID;
    SHORT _nFlag;
    AvatarMotionParamC() { _fPlaySpeed = 1.0f; _nFlag = 0; }
};
struct AvatarMotionParamEx : AvatarMotionParamC {
    UINT _nRsrcID;
    UINT _nInfo;
    UINT _nDakkoRsrcID;
    UINT _nDakkoMotionID;
    AvatarMotionParamEx() { _nInfo = 0; _nDakkoRsrcID = 0; }
};
struct AvatarMotionFrameParam {
    INT _iFrame;
    INT _nFrag;
    AvatarMotionFrameParam() { _iFrame = 0; _nFrag = 0; }
};
struct AvatarFadeParam {
    FLOAT _fTarget;
    UINT _nTime;
};
struct AvatarMotionParamEfx {
    UINT _nRsrcID;
    UINT _nMotionID;
    FLOAT _fPlaySpeed;
    UINT _StartFrame;
    UINT _nHokanFrame;
    UINT _nEfxNo;
    UINT _nFlag;
    AvatarMotionParamEfx() {
        _nRsrcID = 0;
        _nMotionID = 0;
        _fPlaySpeed = 1.0f;
        _StartFrame = 0;
        _nHokanFrame = 0;
        _nEfxNo = 0;
        _nFlag = 0;
    }
};
struct AvatarScaleParam {
    FVECTOR _vTarget;
    UINT _nTime;
};
struct AvatarSetLook {
    FVECTOR LookMin;
    FVECTOR LookMax;
    FLOAT Rate;
    FLOAT OffsetY;
    INT TargetID;
    char* BoneName;
};
struct AvatarSetLookAngle {
    FVECTOR LookMin;
    FVECTOR LookMax;
    FVECTOR LookAngle;
    FLOAT Rate;
    char* BoneName;
};
struct AvatarChangeTexture {
    char* DstTexName;
    char* SrcTexName;
};
struct AvatarEffectAttach {
    char* EffectName;
    INT EfxNo;
    char* BoneName;
    INT Type;
    AvatarEffectAttach() { EffectName = NULL; EfxNo = 0; BoneName = NULL; Type = 0; }
};
struct AvatarModelDispParam {
    INT _iNo;
    INT _nState;
    AvatarModelDispParam() { _iNo = 0; _nState = 0; }
};
struct FadeCreateParam {
    INT kind;
    FLOAT fRate;
    INT layer;
};
namespace MoogleShop {
/* A DLL class: its constructor is not linked into the main program (the original calls address 0). */
class CMoogleShopCore : public CTask {
    BYTE _pad[0x2F0 - sizeof(CTask)];
public:
    CMoogleShopCore(CTask* pParent);
};
}
__asm__(".weak __Q210MoogleShop15CMoogleShopCoreP5CTask\n__Q210MoogleShop15CMoogleShopCoreP5CTask = 0\n");
/* Game/CardKind's card table (a static in the original's header; the same as in Game/Card.cpp): emitted with
 * its strings at the end of .rodata. */
static const CARD_DATA cardDataTbl[224] = {
    { { 0x00 }, 0, 0, 0, "" }, // 0
    { { 0x11 }, 2, 0, 3, "Kingdom Key" }, // 1
    { { 0x11 }, 2, 1, 3, "Three Wishes" }, // 2
    { { 0x11 }, 2, 2, 3, "Crabclaw" }, // 3
    { { 0x11 }, 2, 3, 3, "Pumpkinhead" }, // 4
    { { 0x11 }, 2, 4, 3, "Fairy Harp" }, // 5
    { { 0x11 }, 2, 5, 3, "Wishing Star" }, // 6
    { { 0x11 }, 2, 6, 3, "Spellbinder" }, // 7
    { { 0x11 }, 2, 7, 3, "Metal Chocobo" }, // 8
    { { 0x11 }, 3, 0, 3, "Olympia" }, // 9
    { { 0x11 }, 3, 1, 3, "Lionheart" }, // 10
    { { 0x11 }, 3, 2, 3, "Lady Luck" }, // 11
    { { 0x11 }, 3, 3, 3, "Divine Rose" }, // 12
    { { 0x11 }, 3, 4, 3, "Oathkeeper" }, // 13
    { { 0x11 }, 3, 5, 3, "Oblivion" }, // 14
    { { 0x11 }, 3, 6, 3, "Ultima Weapon" }, // 15
    { { 0x11 }, 3, 7, 3, "Diamond Dust" }, // 16
    { { 0x11 }, 4, 0, 3, "One-Winged Angel" }, // 17
    { { 0x11 }, 4, 1, 4, "Soul Eater" }, // 18
    { { 0x11 }, 4, 2, 3, "Star Seeker" }, // 19
    { { 0x11 }, 4, 3, 3, "Monochrome" }, // 20
    { { 0x11 }, 4, 4, 3, "Follow the Wind" }, // 21
    { { 0x11 }, 4, 5, 3, "Hidden Dragon" }, // 22
    { { 0x11 }, 4, 6, 3, "Photon Debugger" }, // 23
    { { 0x11 }, 4, 7, 3, "Bond of Flame" }, // 24
    { { 0x11 }, 26, 0, 0, "-" }, // 25
    { { 0x22 }, 5, 0, 3, "Fire" }, // 26
    { { 0x22 }, 5, 1, 3, "Blizzard" }, // 27
    { { 0x22 }, 5, 2, 3, "Thunder" }, // 28
    { { 0x22 }, 5, 3, 3, "Cure" }, // 29
    { { 0x22 }, 5, 4, 3, "Gravity" }, // 30
    { { 0x22 }, 5, 5, 3, "Stop" }, // 31
    { { 0x22 }, 5, 6, 3, "Aero" }, // 32
    { { 0x22 }, 26, 2, 0, "-" }, // 33
    { { 0x22 }, 26, 1, 0, "-" }, // 34
    { { 0x33 }, 5, 7, 3, "Simba" }, // 35
    { { 0x33 }, 6, 0, 3, "Genie" }, // 36
    { { 0x33 }, 6, 1, 3, "Bambi" }, // 37
    { { 0x33 }, 6, 2, 3, "Dumbo" }, // 38
    { { 0x33 }, 6, 3, 3, "Tinker Bell" }, // 39
    { { 0x33 }, 6, 4, 3, "Mushu" }, // 40
    { { 0x33 }, 6, 5, 3, "Cloud" }, // 41
    { { 0x44 }, 6, 6, 6, "Potion" }, // 42
    { { 0x44 }, 6, 7, 6, "Hi-Potion" }, // 43
    { { 0x44 }, 7, 0, 2, "Mega-Potion" }, // 44
    { { 0x44 }, 7, 1, 2, "Ether" }, // 45
    { { 0x44 }, 7, 2, 2, "Mega-Ether" }, // 46
    { { 0x44 }, 7, 3, 2, "Elixir" }, // 47
    { { 0x44 }, 7, 4, 2, "Megalixir" }, // 48
    { { 0x55 }, 7, 5, 2, "Donald Duck" }, // 49
    { { 0x55 }, 7, 6, 2, "Goofy" }, // 50
    { { 0x55 }, 7, 7, 2, "Aladdin" }, // 51
    { { 0x55 }, 8, 0, 2, "Ariel" }, // 52
    { { 0x55 }, 8, 1, 2, "Jack" }, // 53
    { { 0x55 }, 8, 2, 2, "Peter Pan" }, // 54
    { { 0x55 }, 8, 3, 2, "The Beast" }, // 55
    { { 0x55 }, 8, 4, 4, "The King" }, // 56
    { { 0x55 }, 8, 5, 2, "Pluto" }, // 57
    { { 0x66 }, 8, 6, 6, "Guard Armor" }, // 58
    { { 0x66 }, 8, 6, 6, "Trickmaster" }, // 59
    { { 0x66 }, 8, 6, 6, "Jafar" }, // 60
    { { 0x66 }, 8, 6, 6, "Oogie Boogie" }, // 61
    { { 0x66 }, 8, 6, 6, "Parasite Cage" }, // 62
    { { 0x66 }, 8, 6, 6, "Ursula" }, // 63
    { { 0x66 }, 8, 6, 6, "Dragon Maleficent" }, // 64
    { { 0x66 }, 8, 6, 6, "Darkside" }, // 65
    { { 0xe6 }, 8, 7, 4, "-" }, // 66
    { { 0xe6 }, 9, 0, 0, "-" }, // 67
    { { 0x77 }, 9, 1, 6, "Shadow" }, // 68
    { { 0x77 }, 9, 2, 2, "Soldier" }, // 69
    { { 0x77 }, 9, 3, 6, "Large Body" }, // 70
    { { 0x77 }, 9, 4, 2, "Red Nocturne" }, // 71
    { { 0x77 }, 9, 5, 2, "Blue Rhapsody" }, // 72
    { { 0x77 }, 9, 6, 2, "Yellow Opera" }, // 73
    { { 0x77 }, 9, 7, 2, "Green Requiem" }, // 74
    { { 0x77 }, 10, 0, 6, "Powerwild" }, // 75
    { { 0x77 }, 10, 1, 2, "Bouncywild" }, // 76
    { { 0x77 }, 10, 2, 2, "Air Soldier" }, // 77
    { { 0x77 }, 10, 3, 2, "Bandit" }, // 78
    { { 0x77 }, 10, 4, 6, "Fat Bandit" }, // 79
    { { 0x77 }, 10, 5, 2, "Barrel Spider" }, // 80
    { { 0x77 }, 10, 6, 6, "Search Ghost" }, // 81
    { { 0x77 }, 10, 7, 6, "Sea Neon" }, // 82
    { { 0x77 }, 11, 0, 2, "Screwdiver" }, // 83
    { { 0x77 }, 11, 1, 2, "Aquatank" }, // 84
    { { 0x77 }, 11, 2, 6, "Wight Knight" }, // 85
    { { 0x77 }, 11, 3, 2, "Gargoyle" }, // 86
    { { 0x77 }, 11, 4, 6, "Pirate" }, // 87
    { { 0x77 }, 11, 5, 2, "Air Pirate" }, // 88
    { { 0x77 }, 11, 6, 4, "Darkball" }, // 89
    { { 0x77 }, 11, 7, 6, "Defender" }, // 90
    { { 0x77 }, 12, 0, 2, "Wyvern" }, // 91
    { { 0x77 }, 12, 1, 2, "Wizard" }, // 92
    { { 0x77 }, 12, 2, 2, "Neoshadow" }, // 93
    { { 0x77 }, 12, 3, 2, "White Mushroom" }, // 94
    { { 0x77 }, 12, 4, 2, "Black Fungus" }, // 95
    { { 0x77 }, 12, 5, 2, "Creeper Plant" }, // 96
    { { 0x77 }, 12, 6, 2, "Tornado Step" }, // 97
    { { 0x77 }, 12, 7, 2, "Crescendo" }, // 98
    { { 0x77 }, 13, 0, 6, "Guard Armor" }, // 99
    { { 0x77 }, 13, 1, 6, "Parasite Cage" }, // 100
    { { 0x77 }, 13, 2, 6, "Trickmaster" }, // 101
    { { 0x77 }, 13, 3, 6, "Darkside" }, // 102
    { { 0x77 }, 13, 4, 2, "Card Soldier" }, // 103
    { { 0x77 }, 13, 5, 6, "Hades" }, // 104
    { { 0x77 }, 13, 6, 6, "Jafar" }, // 105
    { { 0x77 }, 13, 7, 6, "Oogie Boogie" }, // 106
    { { 0x77 }, 14, 0, 6, "Ursula" }, // 107
    { { 0x77 }, 14, 1, 6, "Hook" }, // 108
    { { 0x77 }, 14, 2, 6, "Dragon Maleficent" }, // 109
    { { 0x77 }, 14, 3, 2, "Riku" }, // 110
    { { 0x77 }, 14, 4, 2, "Axel" }, // 111
    { { 0x77 }, 14, 5, 2, "Larxene" }, // 112
    { { 0x77 }, 14, 6, 2, "Vexen" }, // 113
    { { 0x77 }, 14, 7, 2, "Marluxia" }, // 114
    { { 0x77 }, 15, 0, 6, "Lexaeus" }, // 115
    { { 0x77 }, 15, 1, 2, "Ansem" }, // 116
    { { 0x77 }, 15, 3, 6, "Zexion" }, // 117
    { { 0x77 }, 15, 4, 2, "Xemnas" }, // 118
    { { 0x77 }, 15, 5, 2, "Xigbar" }, // 119
    { { 0x77 }, 15, 6, 2, "Xaldin" }, // 120
    { { 0x77 }, 15, 7, 2, "Sa\203\307x" }, // 121
    { { 0x77 }, 16, 0, 2, "Demyx" }, // 122
    { { 0x77 }, 16, 1, 2, "Luxord" }, // 123
    { { 0x77 }, 16, 2, 2, "Roxas" }, // 124
    { { 0x77 }, 21, 6, 2, "Gold Card" }, // 125
    { { 0x77 }, 21, 7, 2, "Platinum Card" }, // 126
    { { 0x77 }, 15, 2, 0, "Card Soldier Spade" }, // 127
    { { 0x77 }, 13, 4, 0, "Card Soldier Heart" }, // 128
    { { 0x17 }, 22, 0, 0, "\203N\203\211\203E\203hA" }, // 129
    { { 0x17 }, 22, 1, 0, "\203N\203\211\203E\203hB" }, // 130
    { { 0x17 }, 22, 2, 0, "\203n\203f\203XA" }, // 131
    { { 0x27 }, 22, 3, 0, "\203n\203f\203XB" }, // 132
    { { 0x17 }, 22, 4, 0, "\203t\203b\203NA" }, // 133
    { { 0x17 }, 22, 5, 0, "\203t\203b\203NB" }, // 134
    { { 0x17 }, 22, 6, 0, "\203t\203b\203NC" }, // 135
    { { 0x27 }, 22, 7, 0, "\203t\203b\203ND" }, // 136
    { { 0x17 }, 23, 0, 0, "\203A\203N\203Z\203\213A" }, // 137
    { { 0x17 }, 23, 1, 0, "\203A\203N\203Z\203\213B" }, // 138
    { { 0x17 }, 23, 2, 0, "\203\211\203N\203V\201[\203kA" }, // 139
    { { 0x17 }, 23, 3, 0, "\203\211\203N\203V\201[\203kB" }, // 140
    { { 0x27 }, 23, 4, 0, "\203\211\203N\203V\201[\203kC" }, // 141
    { { 0x17 }, 23, 5, 0, "\203\212\203NA" }, // 142
    { { 0x17 }, 23, 6, 0, "\203\212\203NB" }, // 143
    { { 0x17 }, 23, 7, 0, "\203\212\203NC" }, // 144
    { { 0x17 }, 24, 0, 0, "\203\212\203ND" }, // 145
    { { 0x17 }, 24, 1, 0, "\203\224\203B\203N\203Z\203\223A" }, // 146
    { { 0x27 }, 24, 2, 0, "\203\224\203B\203N\203Z\203\223B" }, // 147
    { { 0x17 }, 24, 3, 0, "\203}\201[\203\213\201[\203V\203\203A" }, // 148
    { { 0x17 }, 24, 4, 0, "\203}\201[\203\213\201[\203V\203\203B" }, // 149
    { { 0x27 }, 24, 5, 0, "\203}\201[\203\213\201[\203V\203\203C" }, // 150
    { { 0x77 }, 24, 6, 0, "\203}\201[\203\213\201[\203V\203\2032" }, // 151
    { { 0x77 }, 24, 7, 0, "\203}\201[\203\213\201[\203V\203\2033" }, // 152
    { { 0x17 }, 25, 0, 0, "\203\214\203N\203Z\203E\203XA" }, // 153
    { { 0x17 }, 25, 1, 0, "\203\214\203N\203Z\203E\203XB" }, // 154
    { { 0x27 }, 25, 2, 0, "\203\214\203N\203Z\203E\203XC" }, // 155
    { { 0x17 }, 25, 3, 0, "\203A\203\223\203Z\203\200A" }, // 156
    { { 0x27 }, 25, 4, 0, "\203A\203\223\203Z\203\200B" }, // 157
    { { 0x27 }, 25, 5, 0, "\203[\203N\203V\203I\203\223A" }, // 158
    { { 0x77 }, 26, 3, 0, "-" }, // 159
    { { 0x88 }, 0, 0, 6, "\203\212\203\215\201[\203h" }, // 160
    { { 0x99 }, 16, 3, 6, "Tranquil Darkness" }, // 161
    { { 0x99 }, 16, 4, 6, "Teeming Darkness" }, // 162
    { { 0x99 }, 16, 5, 6, "Feeble Darkness" }, // 163
    { { 0x99 }, 16, 6, 6, "Almighty Darkness" }, // 164
    { { 0x99 }, 16, 7, 6, "Sleeping Darkness" }, // 165
    { { 0x99 }, 17, 0, 6, "Looming Darkness" }, // 166
    { { 0x99 }, 17, 1, 2, "Premium Room" }, // 167
    { { 0x99 }, 17, 2, 2, "White Room" }, // 168
    { { 0x99 }, 17, 3, 6, "Black Room" }, // 169
    { { 0x99 }, 17, 4, 6, "Bottomless Darkness" }, // 170
    { { 0x99 }, 17, 5, 6, "Roulette Room" }, // 171
    { { 0xa9 }, 17, 6, 6, "Martial Waking" }, // 172
    { { 0xa9 }, 17, 7, 2, "Sorcerous Waking" }, // 173
    { { 0xa9 }, 18, 0, 2, "Alchemic Waking" }, // 174
    { { 0xa9 }, 18, 1, 6, "Meeting Ground" }, // 175
    { { 0xa9 }, 18, 2, 6, "Stagnant Space" }, // 176
    { { 0xa9 }, 18, 3, 6, "Strong Initiative" }, // 177
    { { 0xa9 }, 18, 4, 6, "Lasting Daze" }, // 178
    { { 0xb9 }, 18, 5, 2, "Calm Bounty" }, // 179
    { { 0xb9 }, 18, 6, 2, "Guarded Trove" }, // 180
    { { 0xb9 }, 18, 7, 2, "False Bounty" }, // 181
    { { 0xb9 }, 19, 0, 6, "Moment's Reprieve" }, // 182
    { { 0xb9 }, 19, 1, 6, "Mingling Worlds" }, // 183
    { { 0xb9 }, 19, 2, 2, "Moogle Room" }, // 184
    { { 0xb9 }, 19, 3, 6, "Random Joker" }, // 185
    { { 0xca }, 19, 4, 6, "Key of Beginnings" }, // 186
    { { 0xca }, 19, 5, 6, "Key of Guidance" }, // 187
    { { 0xca }, 19, 6, 6, "Key to Truth" }, // 188
    { { 0xca }, 19, 7, 2, "Key to Rewards" }, // 189
    { { 0xdb }, 20, 0, 6, "Traverse Town" }, // 190
    { { 0xdb }, 20, 3, 6, "Agrabah" }, // 191
    { { 0xdb }, 20, 2, 6, "Olympus Coliseum" }, // 192
    { { 0xdb }, 20, 1, 6, "Wonderland" }, // 193
    { { 0xdb }, 20, 5, 6, "Monstro" }, // 194
    { { 0xdb }, 20, 4, 6, "Halloween Town" }, // 195
    { { 0xdb }, 20, 6, 6, "Atlantica" }, // 196
    { { 0xdb }, 20, 7, 6, "Neverland" }, // 197
    { { 0xdb }, 21, 0, 6, "Hollow Bastion" }, // 198
    { { 0xdb }, 21, 1, 2, "100 Acre Woods" }, // 199
    { { 0xdb }, 21, 2, 6, "Twilight Town" }, // 200
    { { 0xdb }, 21, 3, 6, "Destiny Islands" }, // 201
    { { 0xdb }, 21, 4, 4, "Castle Oblivion" }, // 202
    { { 0x0d }, 0, 0, 0, "EXP\220\302\217\254" }, // 203
    { { 0x0d }, 0, 1, 0, "EXP\220\302\222\206" }, // 204
    { { 0x0d }, 0, 2, 0, "EXP\220\302\221\345" }, // 205
    { { 0x0d }, 0, 3, 0, "EXP\220\324\217\254" }, // 206
    { { 0x0d }, 0, 4, 0, "EXP\220\324\222\206" }, // 207
    { { 0x0d }, 0, 5, 0, "EXP\220\324\221\345" }, // 208
    { { 0x0d }, 0, 6, 0, "EXP\213\342" }, // 209
    { { 0x0d }, 1, 0, 0, "HP\217\254" }, // 210
    { { 0x0d }, 1, 1, 0, "HP\221\345" }, // 211
    { { 0x0d }, 2, 0, 0, "MP\217\254" }, // 212
    { { 0x0d }, 2, 1, 0, "MP\221\345" }, // 213
    { { 0x0d }, 3, 0, 0, "P" }, // 214
    { { 0x0d }, 4, 0, 0, "R" }, // 215
    { { 0x0c }, 0, 0, 0, "Unknown Room" }, // 216
    { { 0x0c }, 0, 0, 0, "Conqueror's Respite" }, // 217
    { { 0x0c }, 0, 0, 0, "Room of Rewards" }, // 218
    { { 0x0c }, 0, 0, 0, "" }, // 219
    { { 0x0c }, 0, 0, 0, "" }, // 220
    { { 0x00 }, 0, 0, 0, "" }, // 221
    { { 0x00 }, 0, 0, 0, "" }, // 222
    { { 0x00 }, 0, 0, 0, "" }, // 223
};
/* CardKind.h:554-558 in the original (a CARD constructor) */
static inline void setCard(CARD& card, USHORT nKind, CHAR nNumber) {
    card.m_nId = 0;
    card.m_nNumber = nNumber;
    card.m_nKind2 = nKind;
    card.m_nCategory0 = cardDataTbl[card.m_nKind].nCategory0;
    card.m_nCategory1 = cardDataTbl[card.m_nKind].nCategory1;
}
/* CollUtil.h:194/245 in the original */
static inline CCollUtil* collUtilInstance() {
    CCollUtil*& instance = CCollUtil::m_pInstance;
    if (!instance) AssertApp(COLLUTIL_H_PATH, 194, "m_pInstance", 0);
    return instance;
}
static inline void enableChipCollision(FVECTOR& vChipCenter, BOOL fg) { collUtilInstance()->_enableChipCollision(vChipCenter, fg); }
/* effect.h:433 in the original */
static inline void effectSetFlag4(CEffect* p) { p->m_iFlag |= 4; }
/* Entity.h:176 in the original */
static inline void setEntityFlagBit(CEntity* p, UINT nFlag, BOOL bSet) {
    if (bSet) p->setEntityFlag(nFlag, 0); else p->setEntityFlag(0, nFlag);
}
/* SceneEnv.h:51 in the original */
static inline void sceneSetColor(INT nType, INT nFrame, const FVECTOR& vCol) {
    CSceneEnv::setColor((ObjType::eObjBit)(nType & 0xFFFF), (FLOAT)nFrame, vCol, NULL, (UINT)nType >> 16);
}
class CRsrcDB {
public:
    static void removeGroup(INT nGroup, INT nMember);
};
/* RsrcDef.h:171 in the original */
static inline INT makeRsrcID(INT nGroup, INT nMember) { return nGroup * 10000 + nMember; }
/* SceneInfo.h:223 in the original */
static inline BOOL roomHasStorageInfo(const RoomInfo* p) { return (UINT)p->_pSrcInfo | (UINT)p->_pStorageInfo | (UINT)p->_pStorageInfo2; }
INT CVSM::execExt3Code(INT op, eATEL_REQ_RET& ret) {
    INT nID;
    INT c;
    INT t;
    INT r;
    INT i7;
    INT i6;
    INT i5;
    INT i4;
    INT i3;
    INT i2;
    INT i1;

    do {
        r = 1;
        switch (op) {
        case 0xAA: {
            i7 = m_S[m_SP--].I;
            i6 = m_S[m_SP--].I;
            i5 = m_S[m_SP--].I;
            i4 = m_S[m_SP--].I;
            i3 = m_S[m_SP--].I;
            i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP].I;
            ATP& atp = m_pCurrentActor->_ATP;
            ChatWndCreateCTDParam CWCP;
            ChatWndSetWordsParam CWP;
            CWCP.pos.setPoint(-1, -1);
            CWCP.ctdID = i2;
            CWCP.waitFrame = i3 & 0xFFFF;
            CWP.ctdID[0] = i4;
            CWP.ctdID[1] = i5;
            CWP.ctdID[2] = i6;
            CWP.ctdID[3] = i7;
            CWCP.pCwswp = &CWP;
            ctd_cal(m_pCurrentActor->_pAvatar, i2, i3, &CWCP);
            if (i3 < 0) {
                CChatMan::instance().reqCommand(0x409, &CWCP, i1);
            } else {
                close_dest_se(i1);
                CChatMan::instance().reqCommand(0x405, &CWCP, i1);
                WinOpenSE(i2);
                CChatMan::instance().reqCommand(0x3EA, &CWCP, i1);
                if (i3 & 0x20000000) {
                    INT n = 0;
                    CChatMan::instance().reqCommand(0x413, &n, i1);
                }
            }
            if (i3 & 0x10000000) break;
            if (i3 & 0x40000000) {
                INT n = 0;
                CChatMan::instance().reqCommand(0x404, &n, i1);
            } else {
                INT n = 60;
                CChatMan::instance().reqCommand(0x404, &n, i1);
            }
            atp.wait_key = i1;
            atp.status = 12;
            atp.next_address = m_Pctr;
            ret = ATEL_RET_MS_WAIT; r = 3;
            break;
        }
        case 0xAC: {
            BOOL bFlag = m_S[m_SP--].I;
            FVECTOR vPos;
            vPos.w = 0.0f;
            vPos.z = m_S[m_SP--].F;
            vPos.y = m_S[m_SP--].F;
            vPos.x = m_S[m_SP].F;
            enableChipCollision(vPos, bFlag);
            break;
        }
        case 0xAD: {
            OpStack param2;
            OpStack param1;
            param2.I = m_S[m_SP--].I;
            param1.I = m_S[m_SP--].I;
            switch (m_S[m_SP].I) {
            case 0:
                TRACE("%d\n", param1.I);
                break;
            case 1:
                TRACE("%f\n", param1.F);
                break;
            case 2:
                CPadIO::resetVibration();
                break;
            case 3:
                CSceneEnv::setStop((ObjType::eObjBit)param1.I);
                break;
            case 4:
                CSceneEnv::setStop((ObjType::eObjBit)0);
                break;
            case 5:
                if (!CGameWork::getFlag(1, 0x33)) {
                    CARD card;
                    setCard(card, param1.I, param2.I);
                    card = CCard::correctCardRank(card);
                    if (card.m_nKind) CPlayer::notifyGetPrize(param1.I, param2.I, 2);
                }
                break;
            case 6:
                if (!CGameWork::getFlag(1, 0x33)) {
                    CCard::deleteCard(param1.I, param2.I);
                }
                break;
            case 7:
                if (!CGameWork::getFlag(1, 0x33)) {
                    if (CGameWork::getByte(0, 0x61) == 3) break;
                    TRACE("\x83X\x83g\x83" "b\x83N\x8bZ\x8fK\x93\xbe:%d\n", param1.I);
                    CTechnic::addTechnic(param1.I);
                    if (param2.I) new (__FILE__ "(2623)") CLevelUp(CTaskMan::getSysTask(1), param1.I);
                }
                break;
            case 8:
                CSound::fadeoutAllSE(param1.I, 1);
                break;
            case 9:
                CSound::fadeAllSE(param1.I, param2.I);
                break;
            case 10:
                CMapEnemyManager::setEncountEventEnemy(param1.I, 0);
                CSceneInfo::reqEnterBattle(0);
                break;
            case 11: {
                PRIZE_PARAM_C param;
                param.PrizeKind = 0x31;
                setFVector(param.vPos, 0.0f, 0.8f, 3.0f);
                param.iNumber = 0;
                param.uiFlag = 0;
                setFVector(param.vRot, 0.0f, 0.0f, 0.0f);
                param.fSpeed = 0.0f;
                CEntityFactory::getInstance()->createPrize(param, NULL);
                break;
            }
            case 15:
                CShadowModelMan::setADensity(param1.F);
                break;
            case 16:
                new (__FILE__ "(2672)") CAtelTutorial(CTaskMan::getSysTask(0x1A));
                break;
            case 17:
                m_S[m_SP].I = CSceneInfo::getCurrentFloorNumber();
                break;
            case 18:
                CPlaySpeedCtrl::setPlaySpeed(param1.F);
                TRACE("Speed %f\n", param1.F);
                break;
            case 19:
                if (!CGameWork::getFlag(1, 0x33)) {
                    CTechnic::clearAll();
                }
                break;
            case 20:
                if (!CGameWork::getFlag(1, 0x33)) {
                    TRACE("CARD WINDW CLOSE WAIT START\n");
                    acWaitCardWind(*m_pCurrentActor, m_Pctr);
                    ret = ATEL_RET_CARD_WND_WAIT; r = 3;
                }
                break;
            case 21:
                if (!CGameWork::getFlag(1, 0x33)) {
                    TRACE("reqEnterDorr:ATEL sys_ctrl\n");
                    CSceneInfo::reqEnterDoor((DOOR_ID)param1.I);
                }
                break;
            case 22:
                CShadowModelMan::setCDensity(param1.F);
                break;
            case 23:
                m_pAtelCard = new (__FILE__ "(2713)") CAtelCardSeq(CTaskMan::getSysTask(0x1A), 0, param1.I, param2.I);
                break;
            case 24:
                m_pAtelCard = new (__FILE__ "(2716)") CAtelCardSeq(CTaskMan::getSysTask(0x1A), 1, param1.I, param2.I);
                break;
            case 25:
                m_pAtelCard = new (__FILE__ "(2719)") CAtelCardSeq(CTaskMan::getSysTask(0x1A), 2, param1.I, param2.I);
                break;
            case 26:
                m_pAtelCard->out();
                break;
            case 27:
                g_pPlayer->cmdRevolver_rollDeck(param1.I);
                break;
            case 28:
                if (!CGameWork::getFlag(1, 0x33)) {
                    PLM::setReqLoadPlayer(0);
                }
                break;
            case 29:
                if (!CGameWork::getFlag(1, 0x33)) {
                    PLM::setReqLoadBattlePlayer(0);
                }
                break;
            case 30:
                TRACE("ATEL:cancelLoadRCF:%d,%d\n", param1.I, param2.I);
                CRcf::cancelLoadRCF(param1.I, param2.I);
                break;
            case 31:
                m_S[m_SP].I = CSceneInfo::getPrevAreaNum();
                break;
            case 32:
                m_S[m_SP].I = CSceneInfo::getPrevSceneNum();
                break;
            case 33:
                m_S[m_SP].I = CSceneInfo::getPrevAreaScene();
                break;
            case 34:
                CEntityFactory::getInstance()->createEntity(0xB, 0x86, NULL, NULL);
                break;
            case 35:
                CEntityFactory::getInstance()->createEntity(0xB, 0x87, NULL, NULL);
                break;
            case 36:
                ((CMapEnemyManager*)CTaskMan::getSysTask(0x18))->deleteEnemies();
                break;
            case 37:
                CEntityFactory::getInstance()->createEntity(0xB, 0x72, NULL, NULL);
            case 38:
                g_pPlayer->getPlayerCamera()->initCamera(1, 1, NULL);
                break;
            case 39:
                g_pPlayer->getPlayerCamera()->initCamera(3, 0, NULL);
                break;
            case 40:
                TRACE("ATEL:OPMEN DOOR: %d, %d\n", param1.I, param2.I);
                CSceneInfo::setDoorStat((DOOR_STAT)2, (DOOR_ID)param1.I, param2.I);
                break;
            case 41:
                m_S[m_SP].I = CSceneInfo::getCurrentRoomID();
                break;
            case 42:
                m_S[m_SP].I = CSceneInfo::getPrevRoomID();
                break;
            case 43:
                m_S[m_SP].I = CSceneInfo::getEnteredDoorID();
                break;
            case 44: {
                INT flag = CGameWork::getInt(0, param1.I);
                CGameWork::setInt(0, param1.I, flag | param2.I);
                break;
            }
            case 45: {
                INT flag = CGameWork::getInt(0, param1.I);
                flag &= param2.I;
                if (flag) {
                    m_S[m_SP].I = 1;
                } else {
                    m_S[m_SP].I = 0;
                }
                break;
            }
            case 46:
                if (g_pPlayer) g_pPlayer->changeMotion(param1.I, 0, 0, 0, 0, 0);
                break;
            case 47:
                if (g_pPlayer) g_pPlayer->changeMotionEx(param1.I, param2.I, 0, 0, -1, 0, 0);
                break;
            case 48: {
                INT n = getRand() % (param2.I - param1.I) + param1.I;
                TRACE("MIN:%d, MAX:%d,A:%d\n", param1.I, param2.I, n);
                m_S[m_SP].I = n;
                break;
            }
            case 49:
                CSceneInfo::showAllHiddenDoorsOfCurrentFloor();
                TRACE("\x83\x89\x83X\x83g\x83t\x83\x8d\x83" "A\x82\xd6\x82\xcc\x94\xe0\x95\\\x8e\xa6\n");
                break;
            case 50: {
                CTask* pTask = CTaskMan::getSysTask(0x19);
                if (!pTask) break;
                SLContainerT<CTask>* pContainer = ((CBattleEnemyManager*)pTask)->getFirstBattleEnemy();
                while (pContainer) {
                    CTask* p = pContainer->getValue();
                    if (p) {
                        if (p->getTypeID() == 0x1004) {
                            if ((UINT)((CEntity*)p)->getID() % 10000 == 122) {
                                ((CEntity*)p)->changeMotion(param1.I, 0, 0, 0, 0, 0);
                                break;
                            }
                        }
                    }
                    pContainer = pContainer->getNext();
                }
                break;
            }
            case 51: {
                TRACE("ATEL:\x83\x8f\x81[\x83\x8b\x83h\x83Z\x83\x8c\x83N\x83g\x8c\xc4\x82\xd1\x8fo\x82\xb5\n");
                RoomInfo* pRoom = CSceneInfo::getRoomInfo(-1);
                if (!pRoom) break;
                CGimmickManager* pGM = (CGimmickManager*)CTaskMan::getSysTask(0x11);
                CEntity* pDoor = pGM->getDoor(pRoom->_doors[0]._id, NULL, NULL);
                g_pPlayer->cmdState_initWorldSelect(pDoor->getHandle());
                break;
            }
            case 52:
                TRACE("ATEL:UNAVAILABLE DOOR: %d, %d\n", param1.I, param2.I);
                CSceneInfo::setDoorStat((DOOR_STAT)4, (DOOR_ID)param1.I, param2.I);
                break;
            case 53:
                TRACE("ATEL:\x83S\x81[\x83\x8b\x95\x94\x89\xae\x8aJ\x82\xa2\x82\xbd\n");
                CSceneInfo::openGoalDoor();
                break;
            case 54: {
                INT n = CStatus::isLevelUp();
                TRACE("\x83\x8c\x83x\x83\x8b\x83" "A\x83" "b\x83v\x81H %d\n", n);
                m_S[m_SP].I = n;
                break;
            }
            case 55: {
                CGimmickManager* pGM = (CGimmickManager*)CTaskMan::getSysTask(0x11);
                SLContainerT<CTask>* pContainer = pGM->getChildren().getHead();
                while (pContainer) {
                    CTask* p = pContainer->getValue();
                    if (p) {
                        if (p->getTypeID() == 0x1005) {
                            switch ((UINT)((CEntity*)p)->getID() % 10000) {
                            case 0x43E:
                            case 0x43F:
                            case 0x440:
                                if (param1.I == 1) {
                                    setEntityFlagBit((CEntity*)p, 0x10000, FALSE);
                                    g_pPlayer->cancelLockon();
                                    ((CEntity*)p)->bodyCollOff(-1);
                                } else {
                                    INT st = pGM->getStorageGimmickState(((CBaseGimmick*)p)->getUniqueID());
                                    if (st <= 0 || (UINT)((CEntity*)p)->getID() % 10000 == 0x43E) {
                                        setEntityFlagBit((CEntity*)p, 0x10000, TRUE);
                                        ((CEntity*)p)->bodyCollOn(-1);
                                    }
                                }
                                break;
                            case 0x43D:
                                if (param1.I == 0) {
                                    setEntityFlagBit((CEntity*)p, 0x10000, FALSE);
                                    g_pPlayer->cancelLockon();
                                    ((CEntity*)p)->bodyCollOff(-1);
                                } else {
                                    setEntityFlagBit((CEntity*)p, 0x10000, TRUE);
                                    ((CEntity*)p)->bodyCollOn(-1);
                                }
                                break;
                            }
                        }
                    }
                    pContainer = pContainer->getNext();
                }
                break;
            }
            case 56:
                new (__FILE__ "(3000)") MoogleShop::CMoogleShopCore(CTaskMan::getSysTask(1));
                break;
            case 57:
                if (!CGameWork::getFlag(1, 0x33)) {
                    if (CGameWork::getByte(0, 0x61) == 3) break;
                    TRACE("TECH WINDW CLOSE WAIT START\n");
                    acWaitTechWind(*m_pCurrentActor, m_Pctr);
                    ret = ATEL_RET_TECH_WND_WAIT; r = 3;
                }
                break;
            case 58:
                if (!CGameWork::getFlag(1, 0x33)) {
                    CCard::addCard(param1.I, param2.I);
                }
                break;
            case 59: {
                INT world;
                if (CGame::isSoraGame())
                    world = CSceneInfo::getWorldID(CGameWork::getInt(0, 0x33) - 1);
                else
                    world = CSceneInfo::getWorldID(CGameWork::getInt(0, 0x34) - 1);
                TRACE("ATEL:\x83\x8f\x81[\x83\x8b\x83h\x83N\x83\x8a\x83" "A\x81" "F%d\n", world);
                CJiminyMemoFlag::checkEvent(1, world, 0);
                break;
            }
            case 60:
                CJiminyMemoFlag::checkEvent(2, CSceneInfo::getCurrentFloorNumber() - 1, 0);
                TRACE("ATEL:\x83t\x83\x8d\x83" "A\x83N\x83\x8a\x83" "A\n");
                break;
            case 61:
                CJiminyMemoFlag::checkEvent(6, 0, 0);
                TRACE("ATEL:\x83\x82\x83\x93\x83X\x83g\x83\x8d\x83U\x83R\x83o\x83g\x83\x8b\x83N\x83\x8a\x83" "A\n");
                break;
            case 62: {
                SLContainerT<CTask>* pContainer = ((CGimmickManager*)CTaskMan::getSysTask(0x11))->getChildren().getHead();
                while (pContainer) {
                    CTask* p = pContainer->getValue();
                    if (p) {
                        if (p->getTypeID() == 0x1006) {
                            setEntityFlagBit((CEntity*)p, 0x10000, FALSE);
                            g_pPlayer->cancelLockon();
                        }
                    }
                    pContainer = pContainer->getNext();
                }
                break;
            }
            case 63:
                if (g_pPlayer) {
                    m_S[m_SP].I = g_pPlayer->getPlayerState();
                }
                break;
            case 64:
                CTitleReportDllStarter::create(CTaskMan::getSysTask(1), 2, 0);
                break;
            case 65:
                TRACE("\x83G\x83N\x83X\x83g\x83\x8a\x81[\x83\x80\x81" "F\x83X\x83g\x83" "b\x83N\x91S\x95\x94\x8ao\x82\xa6\x82\xe9\x81" "A\x83J\x81[\x83h\x91S\x95\x94\x89\xf0\x8b\xd6\x81" "A\x95W\x8f\x80\x83X\x83L\x83\x8b\x91S\x8eg\x97p\x89\xc2\n");
                CTechnic::setAll();
                CGameWork::setInt(0, 0x21, -1);
                CGameWork::setInt(0, 0x2D, -1);
                CGameWork::setInt(0, 0x24, -1);
                break;
            case 66:
                g_pPlayer->getPlayerCamera()->initCamera(1, 0, NULL);
                break;
            case 67:
                CGame::dispOffFadeMask();
                break;
            case 68:
                WorldCard::CWorldCardSeq::create(CTaskMan::getSysTask(1), param1.I);
                break;
            case 69:
                acWaitWorldCardWind(*m_pCurrentActor, m_Pctr);
                ret = ATEL_RET_WORLD_CARD_WND_WAIT; r = 3;
                break;
            case 70:
                CCard::setCardPossible(param1.I);
                break;
            case 71:
                TRACE("ATEL:CGlare::fadein\n");
                CGlare::off(0.0f);
                CGlare::fadein(param1.F);
                break;
            case 72:
                TRACE("ATEL:CGlare::fadeout\n");
                CGlare::off(0.0f);
                CGlare::fadeout(param1.F);
                break;
            case 73:
                CGlare::on(0.0f);
                break;
            case 74:
                TRACE("ATEL:\x82g\x82o\x89\xf1\x95\x9c\n");
                if (g_pPlayer) {
                    g_pPlayer->addHP(g_pPlayer->m_iMaxHP, 0, 0);
                } else {
                    TRACE("\x83v\x83\x8c\x83" "C\x83\x84\x81[\x82\xa2\x82\xc8\x82\xa2\n");
                }
                break;
            case 75: {
                INT n = 0;
                switch (param1.I) {
                case 0:
                    n = 11;
                    break;
                case 1:
                    n = 12;
                    break;
                case 2:
                    n = 13;
                    break;
                case 3:
                    n = 14;
                    break;
                case 4:
                    n = 15;
                    break;
                case 5:
                    n = 16;
                    break;
                }
                CJiminyMemoFlag::checkEvent(10, n, 0);
                break;
            }
            case 76:
                acWaitCrossFade(*m_pCurrentActor, m_Pctr);
                ret = ATEL_RET_CLOSS_FADE_WAIT; r = 3;
                break;
            case 77:
                TRACE("ATEL:\x83o\x83" "b\x83N\x83" "A\x83" "b\x83v\n");
                CGameWork::backup((CGameWork::BACKUP)0);
                break;
            case 78:
                CJiminyMemoFlag::checkEvent(9, 0, 0);
                break;
            case 79:
                TRACE("ATEL:CLOSE DOOR: %d, %d\n", param1.I, param2.I);
                CSceneInfo::setDoorStat((DOOR_STAT)1, (DOOR_ID)param1.I, param2.I);
                break;
            }
            break;
        }
        case 0xAE: {
            F_UL fl;
            fl.ul = m_S[m_SP--].I;
            nID = m_S[m_SP--].I;
            cam_ajt(m_S[m_SP].I, fl, nID);
            break;
        }
        case 0xAF: {
            i1 = m_S[m_SP--].I;
            c = m_S[m_SP].I;
            nID = searchActor((CHAR*)(m_pDseg + c));
            FLOAT f = 8000.0f;
            if (nID < 0) {
                _TRACE(m_bTrace, "GPS NG01:%s\n", m_pDseg + c);
            } else {
                CAvatar* p = getAvatar(nID);
                if (p) {
                    FVECTOR vPos;
                    p->getNowPosition(vPos);
                    switch (i1) {
                    case 0:
                        f = vPos.x;
                        break;
                    case 1:
                        f = vPos.y;
                        break;
                    case 2:
                        f = vPos.z;
                        break;
                    case 3:
                        f = RAD2DEG(p->getDirection());
                        break;
                    }
                }
            }
            m_S[++m_SP].F = f;
            break;
        }
        case 0xE5: {
            INT i1 = m_S[m_SP--].I;
            c = m_S[m_SP].I;
            INT nIdx = searchActor((CHAR*)(m_pDseg + c));
            switch (i1) {
            case 2:
                getAvatar(nIdx)->reqCommand(6, NULL, 0);
                TRACE("NAME TO SHOW :%s\n", m_ActorTbl[nIdx]._ATP.name);
                break;
            case 3:
                getAvatar(nIdx)->reqCommand(6, NULL, 1);
                TRACE("NAME TO HIDE :%s\n", m_ActorTbl[nIdx]._ATP.name);
                break;
            case 6:
                getAvatar(nIdx)->reqCommand(0x1D, NULL, 1);
                TRACE("GRAVITY ON :%s\n", m_ActorTbl[nIdx]._ATP.name);
                break;
            case 7:
                getAvatar(nIdx)->reqCommand(0x1D, NULL, 0);
                TRACE("GRAVITY OFF :%s\n", m_ActorTbl[nIdx]._ATP.name);
                break;
            case 8:
                getAvatar(nIdx)->reqCommand(0x1E, NULL, 1);
                TRACE("MAP_COLL_ON :%s\n", m_ActorTbl[nIdx]._ATP.name);
                break;
            case 9:
                getAvatar(nIdx)->reqCommand(0x1E, NULL, 0);
                TRACE("MAP_COLL_OFF :%s\n", m_ActorTbl[nIdx]._ATP.name);
                break;
            case 10:
                getAvatar(nIdx)->reqCommand(0x1F, NULL, 1);
                TRACE("BODY_COLL_ON :%s\n", m_ActorTbl[nIdx]._ATP.name);
                break;
            case 11:
                getAvatar(nIdx)->reqCommand(0x1F, NULL, 0);
                TRACE("BODY_COLL_OFF :%s\n", m_ActorTbl[nIdx]._ATP.name);
                break;
            case 39:
                getAvatar(nIdx)->reqCommand(1, NULL, 0);
                m_pCurrentActor->_ATP.status = 25;
                m_pCurrentActor->_pAvatar = NULL;
                m_ActorTbl[nIdx]._pAvatar = NULL;
                TRACE("REMOVE NPC :%s\n", m_ActorTbl[nIdx]._ATP.name);
                break;
            case 40:
                getAvatar(nIdx)->reqCommand(0x1A, NULL, 1);
                TRACE("MAKEUP_MOTION_INVALID :%s\n", m_ActorTbl[nIdx]._ATP.name);
                break;
            case 41:
                getAvatar(nIdx)->reqCommand(0x1A, NULL, 0);
                TRACE("MAKEUP_MOTION_VALID :%s\n", m_ActorTbl[nIdx]._ATP.name);
                break;
            case 42:
                getAvatar(nIdx)->reqCommand(0x34, NULL, 0);
                TRACE("AVTCMD_SET_LOOK_START :%s\n", m_ActorTbl[nIdx]._ATP.name);
                break;
            case 43:
                getAvatar(nIdx)->reqCommand(0x35, NULL, 0);
                TRACE("AVTCMD_SET_LOOK_STOP :%s\n", m_ActorTbl[nIdx]._ATP.name);
                break;
            case 44: {
                FLOAT f = 16.0f;
                getAvatar(nIdx)->reqCommand(0x35, NULL, 0);
                getAvatar(nIdx)->reqCommand(0x36, &f, 0);
                TRACE("AVTCMD_SET_LOOK_END :%s\n", m_ActorTbl[nIdx]._ATP.name);
                break;
            }
            case 45:
                getAvatar(nIdx)->reqCommand(0x24, NULL, 0);
                TRACE("AVTCMD_CREATE_LEGIK :%s\n", m_ActorTbl[nIdx]._ATP.name);
                break;
            case 46:
                getAvatar(nIdx)->reqCommand(0x21, NULL, 1);
                TRACE("AVTCMD_SET_IK  ON :%s\n", m_ActorTbl[nIdx]._ATP.name);
                break;
            case 47:
                getAvatar(nIdx)->reqCommand(0x21, NULL, 0);
                TRACE("AVTCMD_SET_IK  OFF :%s\n", m_ActorTbl[nIdx]._ATP.name);
                break;
            case 48:
                getAvatar(nIdx)->reqCommand(0x22, NULL, 1);
                TRACE("AVTCMD_SET_LOCK_ON  ON :%s\n", m_ActorTbl[nIdx]._ATP.name);
                break;
            case 49: {
                getAvatar(nIdx)->reqCommand(0x22, NULL, 0);
                TRACE("AVTCMD_SET_LOCK_ON  OFF :%s\n", m_ActorTbl[nIdx]._ATP.name);
                INT index = searchLockOnNpc();
                if (index != -1) {
                    if (m_ActorTbl[index]._pAvatar == getAvatar(nIdx)) {
                        g_pPlayer->cancelLockon();
                    }
                }
                break;
            }
            case 50:
                getAvatar(nIdx)->reqCommand(0x23, NULL, 1);
                TRACE("AVTCMD_SET_NEAR_CLIP  OFF :%s\n", m_ActorTbl[nIdx]._ATP.name);
                break;
            case 51:
                getAvatar(nIdx)->reqCommand(0x23, NULL, 0);
                TRACE("AVTCMD_SET_NEAR_CLIP  OFF :%s\n", m_ActorTbl[nIdx]._ATP.name);
                break;
            }
            break;
        }
        case 0xE6: {
            INT i6 = m_S[m_SP--].I;
            INT i5 = m_S[m_SP--].I;
            FLOAT f4 = m_S[m_SP--].F;
            FLOAT f3 = m_S[m_SP--].F;
            FLOAT f2 = m_S[m_SP--].F;
            INT i1 = m_S[m_SP--].I;
            c = m_S[m_SP].I;
            INT nID = searchActor((CHAR*)(m_pDseg + c));
            CameraManCreateParam param;
            TRACE("CHT\n");
            param.pathDateTargetID = nID;
            param.changeFlag = i1;
            param.fOffsetX = f2;
            param.fOffsetY = f3;
            param.fOffsetZ = f4;
            param.fFullFrame = (FLOAT)i5;
            param.fStartEndFrame = (FLOAT)i6;
            param.pCvsm = this;
            CCameraMan::instance().reqCommand(0x464, &param, 0);
            break;
        }
        case 0xE7: {
            INT HokanRate = m_S[m_SP--].I;
            FLOAT PlaySpeed = m_S[m_SP--].F;
            INT MotionID = m_S[m_SP].I;
            AvatarMotionParamC param;
            param._nMotionID = MotionID & 0xFFF;
            param._fPlaySpeed = PlaySpeed;
            param._nFlag = (MotionID >> 12) & 7;
            param._iChangeHokanRate = HokanRate;
            if (m_pCurrentActor->_pAvatar) m_pCurrentActor->_pAvatar->reqCommand(0x14, &param, 0);
            break;
        }
        case 0xE8: {
            INT HokanRate = m_S[m_SP--].I;
            AvatarMotionParamC param;
            param._fPlaySpeed = m_S[m_SP--].F;
            i1 = m_S[m_SP--].I;
            c = m_S[m_SP].I;
            nID = searchActor((CHAR*)(m_pDseg + c));
            if (nID < 0) {
                _TRACE(m_bTrace, "MTO NG01:%s\n", m_pDseg + c);
                break;
            }
            CAvatar* p = getAvatar(nID);
            if (p) {
                param._nMotionID = i1 & 0xFFF;
                param._nFlag = (i1 >> 12) & 7;
                param._iChangeHokanRate = HokanRate;
                p->reqCommand(0x14, &param, 0);
            }
            break;
        }
        case 0xE9: {
            AvatarMotionFrameParam param;
            param._nFrag = m_S[m_SP--].I;
            param._iFrame = m_S[m_SP].I;
            m_pCurrentActor->_pAvatar->reqCommand(0x19, &param, 0);
            break;
        }
        case 0xEA: {
            AvatarMotionFrameParam param;
            param._nFrag = m_S[m_SP--].I;
            param._iFrame = m_S[m_SP--].I;
            c = m_S[m_SP].I;
            nID = searchActor((CHAR*)(m_pDseg + c));
            if (nID < 0) {
                _TRACE(m_bTrace, "MTO NG01:%s\n", m_pDseg + c);
                break;
            }
            CAvatar* p = getAvatar(nID);
            if (p) p->reqCommand(0x19, &param, 0);
            break;
        }
        case 0xEB: {
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP--].I;
            nID = searchActor((CHAR*)(m_pDseg + m_S[m_SP].I));
            if (nID > 0 && m_pCurrentActor->_pAvatar) {
                AvatarRotateParam pr;
                if (i2 == 9) {
                    pr._nType = i2;
                } else if (i2 == 10) {
                    pr._nType = i2;
                } else {
                    pr._nType = 2;
                }
                pr._fTime = (FLOAT)i1 * 0.016666668f;
                pr._nTargetID = nID;
                m_pCurrentActor->_pAvatar->reqCommand(0xE, &pr, 0);
            }
            break;
        }
        case 0xEC: {
            INT i1 = m_S[m_SP--].I;
            FLOAT f = m_S[m_SP].F;
            AvatarFadeParam param;
            param._fTarget = f;
            param._nTime = i1;
            m_pCurrentActor->_pAvatar->reqCommand(0x10, &param, -1);
            break;
        }
        case 0xF2: {
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP--].I;
            FLOAT f = m_S[m_SP].F;
            AvatarFadeParam param;
            param._fTarget = f;
            param._nTime = i1;
            m_pCurrentActor->_pAvatar->reqCommand(0x10, &param, i2 - 1);
            break;
        }
        case 0xF3: {
            FLOAT f = m_S[m_SP--].F;
            INT i1 = m_S[m_SP].I;
            if (i1) {
                CDeFocus::setManual(CDeFocus::getNear(), f);
            } else {
                CDeFocus::setManual(f, CDeFocus::getFar());
            }
            break;
        }
        case 0xF4: {
            INT Flag = m_S[m_SP--].I;
            INT HokanFrame = m_S[m_SP--].I;
            FLOAT Speed = m_S[m_SP--].F;
            INT MotionID = m_S[m_SP--].I;
            INT ResrcID = m_S[m_SP].I;
            TRACE("AED %d : %d : %f : %d : %d\n", ResrcID, MotionID, Speed, HokanFrame, Flag);
            AvatarMotionParamEx2 mot;
            mot._nRsrcID = ResrcID;
            mot._nMotionID = MotionID;
            mot._fPlaySpeed = Speed;
            mot._nHokanFrame = HokanFrame;
            mot._nInfo = Flag;
            m_pCurrentActor->_pAvatar->reqCommand(0x16, &mot, 0);
            break;
        }
        case 0xF5: {
            INT Frame = m_S[m_SP--].I;
            OpStack Param = m_S[m_SP--];
            INT type = m_S[m_SP].I;
            FogColorStruct Color;
            FogNomalStruct Nomal;
            switch (type) {
            case 0:
                Color.color = Param.I;
                TRACE("FOG %d : R:%d : G:%d : B:%d : %d\n", type, (Param.I >> 16) & 0xFF, (Param.I >> 8) & 0xFF, Param.I & 0xFF, Frame);
                CAtelFog::m_pInstance->reqCommand(0x5DC, &Color, 0);
                break;
            case 1:
                Color.color = Param.I;
                TRACE("FOG %d : R:%d : G:%d : B:%d : %d\n", type, (Param.I >> 16) & 0xFF, (Param.I >> 8) & 0xFF, Param.I & 0xFF, Frame);
                Color.AnimFrame = Frame;
                CAtelFog::m_pInstance->reqCommand(0x5DD, &Color, 0);
                break;
            case 2:
                TRACE("FOG %d : %d : %d\n", type, Param.F, Frame);
                Nomal.Param = Param.F;
                CAtelFog::m_pInstance->reqCommand(0x5DE, &Nomal, 0);
                break;
            case 3:
                TRACE("FOG %d : %d : %d\n", type, Param.F, Frame);
                Nomal.Param = Param.F;
                Nomal.AnimFrame = Frame;
                CAtelFog::m_pInstance->reqCommand(0x5DF, &Nomal, 0);
                break;
            case 4:
                TRACE("FOG %d : %d : %d\n", type, Param.F, Frame);
                Nomal.Param = Param.F;
                CAtelFog::m_pInstance->reqCommand(0x5E0, &Nomal, 0);
                break;
            case 5:
                TRACE("FOG %d : %d : %d\n", type, Param.F, Frame);
                Nomal.Param = Param.F;
                Nomal.AnimFrame = Frame;
                CAtelFog::m_pInstance->reqCommand(0x5E1, &Nomal, 0);
                break;
            case 6:
                TRACE("FOG %d : %d : %d\n", type, Param.F, Frame);
                Nomal.Param = Param.F;
                CAtelFog::m_pInstance->reqCommand(0x5E2, &Nomal, 0);
                break;
            case 7:
                TRACE("FOG %d : %d : %d\n", type, Param.F, Frame);
                Nomal.Param = Param.F;
                Nomal.AnimFrame = Frame;
                CAtelFog::m_pInstance->reqCommand(0x5E3, &Nomal, 0);
                break;
            case 8:
                TRACE("FOG %d : %d : %d\n", type, Param.F, Frame);
                Nomal.Param = Param.F;
                CAtelFog::m_pInstance->reqCommand(0x5E4, &Nomal, 0);
                break;
            case 9:
                TRACE("FOG %d : %d : %d\n", type, Param.F, Frame);
                Nomal.Param = Param.F;
                Nomal.AnimFrame = Frame;
                CAtelFog::m_pInstance->reqCommand(0x5E5, &Nomal, 0);
                break;
            }
            break;
        }
        case 0xF6: {
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP].I;
            TRACE("SVR:%d,%d\n", i1, i2);
            CSceneInfo::setEventRoomSceneNum(i1, i2);
            break;
        }
        case 0xF7: {
            INT i3 = m_S[m_SP--].I;
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP].I;
            if (CGameWork::getFlag(1, 0x33)) {
                CGameWork::setFlag(1, 0x34, TRUE);
                m_pCurrentActor->_ATP.status = 25;
                break;
            }
            TRACE("CER:%d,%d,%d\n", i1, i2, i3);
            if (i2 == -1) i2 = CSceneInfo::getEnteredDoorID();
            CSceneInfo::reqChangeEventRoom(i1, (DOOR_ID)i2, i3);
            break;
        }
        case 0xF8:
            TRACE("EBT\n");
            CSceneInfo::reqExitBattle();
            break;
        case 0xFA: {
            INT i3 = m_S[m_SP--].I;
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP].I;
            TRACE("scene_set_color:type:%d,frame:%d,color:%d\n", i1, i2, i3);
            FVECTOR vCol;
            vCol.w = (FLOAT)(i3 & 0xFF) / 255.0f;
            vCol.z = (FLOAT)((i3 >> 8) & 0xFF) / 255.0f;
            vCol.y = (FLOAT)((i3 >> 16) & 0xFF) / 255.0f;
            vCol.x = (FLOAT)((i3 >> 24) & 0xFF) / 255.0f;
            sceneSetColor(i1, i2, vCol);
            break;
        }
        case 0xFB: {
            INT i4 = m_S[m_SP--].I;
            INT i3 = m_S[m_SP--].I;
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP].I;
            AvatarEffectAttach param;
            param.BoneName = (char*)(m_pDseg + i3);
            param.EffectName = (char*)(m_pDseg + i1);
            param.EfxNo = i2;
            param.Type = i4;
            m_hEffect = m_pCurrentActor->_pAvatar->reqCommand(0x3A, &param, 0);
            break;
        }
        case 0xFC: {
            FLOAT f = m_S[m_SP--].F;
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP].I;
#line 3698
            if (!(CWorldEnv::getActiveCamera())) AssertApp(__FILE__, __LINE__, "CWorldEnv::getActiveCamera()", 0);
            FVECTOR vPos;
            FVECTOR vRot;
            setFVector(vPos, 0.0f, 0.0f, f);
            setFVector(vRot, 0.0f, 3.1415927f, 0.0f);
            m_hEffect = CEffectMan::attachEffect((CHAR*)(m_pDseg + i1), i2, (ObjType::eObjType)0xD, &CWorldEnv::getActiveCamera()->Crd(0), &vPos, &vRot, 0.0f);
            CEffect* pEffect = CEffectMan::getEffect(m_hEffect);
            if (pEffect) pEffect->setDrawLast(0);
            break;
        }
        case 0xFD: {
            OpStack param2 = m_S[m_SP--];
            OpStack param1 = m_S[m_SP--];
            INT type = m_S[m_SP].I;
            CEffect* effect;
            TRACE("EffectCtrl  %d, %d, %d\n", type, param1.I, param2.I);
            switch (type) {
            case 0:
                m_S[m_SP].I = m_hEffect;
                break;
            case 1: {
                effect = CEffectMan::getEffect(param1.I);
                if (effect) effect->fadeout(param2.F);
                break;
            }
            case 2: {
                effect = CEffectMan::getEffect(param1.I);
                if (effect) effect->fadeout(0.0f);
                break;
            }
            case 3:
                effect = CEffectMan::getEffect(param1.I);
                effectSetFlag4(effect);
                break;
            case 4:
                CEffectMan::getEffect(param1.I)->setSkipFrame(param2.F);
                break;
            case 5: {
                effect = CEffectMan::getEffect(param1.I);
                if (effect) effect->setLightCalc(1);
                break;
            }
            }
            break;
        }
        case 0xFE: {
            INT i3 = m_S[m_SP--].I;
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP].I;
            INT r;
            switch (i2) {
            case 0:
                r = i1 | i3;
                break;
            case 1:
                r = i1 & i3;
                break;
            case 2:
                r = i1 ^ i3;
                break;
            case 3:
                r = i1 >> i3;
                break;
            case 4:
                r = i1 << i3;
                break;
            case 5:
                r = i1 & ~i3;
                break;
            }
            m_S[m_SP].I = r;
            break;
        }
        case 0xFF: {
            INT frame = m_S[m_SP--].I;
            INT param = m_S[m_SP--].I;
            INT type = m_S[m_SP].I;
            BlurStruct blur;
            TRACE("BLUR %d : :%d : %d\n", type, param, frame);
            switch (type) {
            case 0:
                CAtelBlur::m_pInstance->reqCommand(0x640, &param, 0);
                break;
            case 1:
                blur.param = param;
                blur.AnimFrame = frame;
                CAtelBlur::m_pInstance->reqCommand(0x641, &param, 0);
                break;
            case 2:
                CAtelBlur::m_pInstance->reqCommand(0x642, &param, 0);
                break;
            case 3:
                blur.param = param;
                blur.AnimFrame = frame;
                CAtelBlur::m_pInstance->reqCommand(0x643, &param, 0);
                break;
            case 4:
                CAtelBlur::m_pInstance->reqCommand(0x644, &param, 0);
                break;
            case 5:
                blur.param = param;
                blur.AnimFrame = frame;
                CAtelBlur::m_pInstance->reqCommand(0x645, &param, 0);
                break;
            case 6:
                CAtelBlur::m_pInstance->reqCommand(0x647, NULL, 0);
                break;
            }
            break;
        }
        case 0x100: {
            BlurAnimStruct blur;
            blur.AnimFrame = m_S[m_SP--].I;
            blur.param.w = 0.0f;
            blur.param.z = (FLOAT)m_S[m_SP--].I;
            blur.param.y = (FLOAT)m_S[m_SP--].I;
            blur.param.x = (FLOAT)m_S[m_SP].I;
            CAtelBlur*& pBlur = CAtelBlur::m_pInstance;
            pBlur->reqCommand(0x646, &blur, 0);
            break;
        }
        case 0x101: {
            AvatarMotionParamEfx param;
            param._nFlag = m_S[m_SP--].I;
            param._nEfxNo = m_S[m_SP--].I;
            param._nHokanFrame = m_S[m_SP--].I;
            param._StartFrame = m_S[m_SP--].I;
            param._fPlaySpeed = m_S[m_SP--].F;
            param._nMotionID = m_S[m_SP--].I;
            param._nRsrcID = m_S[m_SP--].I;
            INT i1 = m_S[m_SP].I;
            TRACE("MEX:%d,%d,%d,%f,%d,%d,%d,%d\n", i1, param._nRsrcID, param._nMotionID, param._fPlaySpeed, param._StartFrame, param._nHokanFrame, param._nEfxNo, param._nFlag);
            if (i1) {
                INT nIdx = searchActor((CHAR*)(m_pDseg + i1));
                if (nIdx < 0) {
                    _TRACE(m_bTrace, "MTO NG01:%s\n", m_pDseg + c);
                    break;
                }
                CAvatar* p = getAvatar(nIdx);
                if (p) p->reqCommand(0x17, &param, 0);
            } else {
                if (m_pCurrentActor->_pAvatar) m_pCurrentActor->_pAvatar->reqCommand(0x17, &param, 0);
            }
            break;
        }
        case 0x102: {
            i1 = m_S[m_SP - 1].I;
            c = m_S[m_SP].I;
            _TRACE(m_bTrace, "CRC %d %d\n", i1, c);
            m_pCurrentActor->_GroupID = i1;
            m_pCurrentActor->_nMembarID = c;
            m_pCurrentActor->_nRsrcID = makeRsrcID(i1, c);
            acWaitLoadRsrc(*m_pCurrentActor, m_Pctr);
            ret = ATEL_RET_RSRC_LOAD_WAIT; r = 3;
            break;
        }
        case 0x103: {
            i1 = m_S[m_SP - 1].I;
            c = m_S[m_SP].I;
            _TRACE(m_bTrace, "DRC %d %d\n", i1, c);
            m_pCurrentActor->_nRsrcID = makeRsrcID(i1, c);
            CRsrcDB::removeGroup(i1, c);
            break;
        }
        case 0x104: {
            FLOAT f27 = m_S[m_SP--].F;
            FLOAT f26 = m_S[m_SP--].F;
            FLOAT f25 = m_S[m_SP--].F;
            FLOAT f24 = m_S[m_SP--].F;
            FLOAT f23 = m_S[m_SP--].F;
            FLOAT f22 = m_S[m_SP--].F;
            FLOAT f21 = m_S[m_SP--].F;
            FLOAT f20 = m_S[m_SP--].F;
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP].I;
            TRACE("LOK:%s,%s,%f,%f,%f,%f,%f,%f,%f,%f\n", m_pDseg + i1, m_pDseg + i2, f20, f21, f22, f23, f24, f25, f26, f27);
            AvatarSetLook look;
            look.TargetID = searchActor((CHAR*)(m_pDseg + i2));
            look.BoneName = (char*)(m_pDseg + i1);
            setFVector(look.LookMin, DEG2RAD(f20), DEG2RAD(f21), DEG2RAD(f22));
            setFVector(look.LookMax, DEG2RAD(f23), DEG2RAD(f24), DEG2RAD(f25));
            look.OffsetY = f26;
            look.Rate = f27;
            m_pCurrentActor->_pAvatar->reqCommand(0x30, &look, 0);
            break;
        }
        case 0x105: {
            FLOAT f21 = m_S[m_SP--].F;
            FLOAT f20 = m_S[m_SP--].F;
            INT i1 = m_S[m_SP].I;
            TRACE("UDL:%s,%f,%f\n", m_pDseg + i1, f20, f21);
            AvatarSetLook look;
            look.TargetID = searchActor((CHAR*)(m_pDseg + i1));
            look.OffsetY = f20;
            look.Rate = f21;
            m_pCurrentActor->_pAvatar->reqCommand(0x31, &look, 0);
            break;
        }
        case 0x106: {
            FLOAT f29 = m_S[m_SP--].F;
            FLOAT f28 = m_S[m_SP--].F;
            FLOAT f27 = m_S[m_SP--].F;
            FLOAT f26 = m_S[m_SP--].F;
            FLOAT f25 = m_S[m_SP--].F;
            FLOAT f24 = m_S[m_SP--].F;
            FLOAT f23 = m_S[m_SP--].F;
            FLOAT f22 = m_S[m_SP--].F;
            FLOAT f21 = m_S[m_SP--].F;
            FLOAT f20 = m_S[m_SP--].F;
            INT i1 = m_S[m_SP].I;
            TRACE("LAG:%s,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", m_pDseg + i1, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29);
            AvatarSetLookAngle look;
            look.BoneName = (char*)(m_pDseg + i1);
            setFVector(look.LookAngle, DEG2RAD(f20), DEG2RAD(f21), DEG2RAD(f22));
            setFVector(look.LookMin, DEG2RAD(f23), DEG2RAD(f24), DEG2RAD(f25));
            setFVector(look.LookMax, DEG2RAD(f26), DEG2RAD(f27), DEG2RAD(f28));
            look.Rate = f29;
            m_pCurrentActor->_pAvatar->reqCommand(0x32, &look, 0);
            break;
        }
        case 0x107: {
            FLOAT f23 = m_S[m_SP--].F;
            FLOAT f22 = m_S[m_SP--].F;
            FLOAT f21 = m_S[m_SP--].F;
            FLOAT f20 = m_S[m_SP].F;
            TRACE("UDA:%f,%f,%f,%f\n", f20, f21, f22, f23);
            AvatarSetLookAngle look;
            look.Rate = f23;
            setFVector(look.LookAngle, DEG2RAD(f20), DEG2RAD(f21), DEG2RAD(f22));
            m_pCurrentActor->_pAvatar->reqCommand(0x33, &look, 0);
            break;
        }
        case 0x108: {
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP].I;
            AvatarChangeTexture param;
            param.DstTexName = (char*)(m_pDseg + i1);
            param.SrcTexName = (char*)(m_pDseg + i2);
            TRACE("CTX:%s,%s\n", param.DstTexName, param.SrcTexName);
            m_pCurrentActor->_pAvatar->reqCommand(0x37, &param, 0);
            break;
        }
        case 0x109: {
            FLOAT f1 = m_S[m_SP].F;
            /* a scalar read of _pAvatar: keeps the store of f1 before it (struct-field reads may pass a fixed scalar store) */
            (*(CAvatar**)&m_pCurrentActor->_pAvatar)->reqCommand(0x35, NULL, 0);
            m_pCurrentActor->_pAvatar->reqCommand(0x36, &f1, 0);
            TRACE("ELK:AVTCMD_SET_LOOK_END\n");
            break;
        }
        case 0x10A: {
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP].I;
            CHAR* pName = (CHAR*)(m_pDseg + i2);
            TRACE("STA\x81" "F%d, %s\n", i1, pName);
            m_pCurrentActor->_pAvatar->reqCommand(0x38, pName, i1);
            break;
        }
        case 0x10B: {
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP--].I;
            CHAR* pName = (CHAR*)(m_pDseg + i2);
            INT nIdx = searchActor((CHAR*)(m_pDseg + m_S[m_SP].I));
            CAvatar* p = getAvatar(nIdx);
            TRACE("STX\x81" "F%d, %s\n", i1, pName);
            p->reqCommand(0x38, pName, i1);
            break;
        }
        case 0x10C: {
            FVECTOR vPos;
            vPos.z = m_S[m_SP--].F;
            vPos.y = m_S[m_SP--].F;
            vPos.x = m_S[m_SP--].F;
            INT i1 = m_S[m_SP].I;
            TRACE("SUP:%d,%f,%f,%f\n", i1, vPos.x, vPos.y, vPos.z);
            switch (i1) {
            case 0:
                g_pPlayer->warp(vPos);
                break;
            case 1:
                break;
            case 2: {
                CTask* pTask = CTaskMan::getSysTask(0x19);
                if (!pTask) break;
                SLContainerT<CTask>* pContainer = ((CBattleEnemyManager*)pTask)->getFirstBattleEnemy();
                while (pContainer) {
                    CTask* p = pContainer->getValue();
                    if (p) {
                        if (p->getTypeID() == 0x1004) {
                            if ((UINT)((CEntity*)p)->getID() % 10000 == 122) {
                                ((CEntity*)p)->warp(vPos);
                                break;
                            }
                        }
                    }
                    pContainer = pContainer->getNext();
                }
                break;
            }
            default:
                TRACE("ATEL:\x83G\x83\x89\x81[\x81" "FsetUnknownPos\x81@\x91\xb6\x8d\xdd\x82\xb5\x82\xc8\x82\xa2\x83p\x83\x89\x83\x81\x81[\x83^\n");
                break;
            }
            break;
        }
        case 0x10D: {
            FVECTOR vRot;
            vRot.z = m_S[m_SP--].F;
            vRot.y = m_S[m_SP--].F;
            vRot.x = m_S[m_SP--].F;
            INT i1 = m_S[m_SP].I;
            TRACE("SUR:%d,%f,%f,%f\n", i1, vRot.x, vRot.y, vRot.z);
            switch (i1) {
            case 0:
                vRot.z = DEG2RAD(vRot.z);
                vRot.y = DEG2RAD(vRot.y);
                vRot.x = DEG2RAD(vRot.x);
                g_pPlayer->setRot(vRot);
                break;
            case 1:
                g_pPlayer->getPlayerCamera()->initCamera(1, 1, &vRot);
                break;
            case 2: {
                CTask* pTask = CTaskMan::getSysTask(0x19);
                if (!pTask) break;
                SLContainerT<CTask>* pContainer = ((CBattleEnemyManager*)pTask)->getFirstBattleEnemy();
                while (pContainer) {
                    CTask* p = pContainer->getValue();
                    if (p) {
                        if (p->getTypeID() == 0x1004) {
                            if ((UINT)((CEntity*)p)->getID() % 10000 == 122) {
                                ((CEntity*)p)->setRot(vRot);
                                break;
                            }
                        }
                    }
                    pContainer = pContainer->getNext();
                }
                break;
            }
            default:
                TRACE("ATEL:\x83G\x83\x89\x81[\x81" "FsetUnknownRot\x81@\x91\xb6\x8d\xdd\x82\xb5\x82\xc8\x82\xa2\x83p\x83\x89\x83\x81\x81[\x83^\n");
                break;
            }
            break;
        }
        case 0x10E: {
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP].I;
            TRACE("enableNotSemiModel: %d,%d", i1, i2);
            m_pCurrentActor->_pAvatar->reqCommand(0x39, &i1, i2);
            break;
        }
        case 0x10F: {
            INT i2 = m_S[m_SP--].I;
            CHAR* pName = (CHAR*)(m_pDseg + m_S[m_SP].I);
            CGame::reqPlayMovie(pName, 1);
            TRACE("ATEL:playMovie %s, %d\n", pName, i2);
            break;
        }
        case 0x110: {
            INT frame = m_S[m_SP--].I;
            INT param = m_S[m_SP--].I;
            INT flag = m_S[m_SP].I;
            CrossFadeColorStruct Color;
            TRACE("ATEL:CrossFade %d, %d\n", flag, param);
            switch (flag) {
            case 0:
                CAtelCrossFade::m_pInstance->reqCommand(0x640, NULL, 0);
                break;
            case 1:
                CAtelCrossFade::m_pInstance->reqCommand(0x641, NULL, 0);
                TRACE("\x83N\x83\x8d\x83X\x83t\x83" "F\x81[\x83h\x97p\x83" "C\x83\x81\x81[\x83W\x82r\x82s\x82n\x82q\x82" "d\n");
                break;
            case 2:
                CAtelCrossFade::m_pInstance->reqCommand(0x642, NULL, 0);
                TRACE("\x83N\x83\x8d\x83X\x83t\x83" "F\x81[\x83h\x97p\x83" "C\x83\x81\x81[\x83W\x82" "c\x82q\x82`\x82v\n");
                break;
            case 3:
                Color.color = param;
                TRACE("CROSS_FADE : R:%d : G:%d : B:%d : A:%d\n", (UINT)param >> 24, (param >> 16) & 0xFF, (param >> 8) & 0xFF, param & 0xFF);
                CAtelCrossFade::m_pInstance->reqCommand(0x643, &Color, 0);
                break;
            case 4:
                Color.color = param;
                TRACE("CROSS_FADE : R:%d : G:%d : B:%d : A:%d : Frame:%d\n", (UINT)param >> 24, (param >> 16) & 0xFF, (param >> 8) & 0xFF, param & 0xFF, frame);
                Color.AnimFrame = frame;
                CAtelCrossFade::m_pInstance->reqCommand(0x644, &Color, 0);
                break;
            }
            break;
        }
        case 0xED: {
            INT i3 = m_S[m_SP--].I;
            INT door = m_S[m_SP--].I;
            INT room = m_S[m_SP].I;
            TRACE("ChangeRoom:room:%d,door:%d,scene:%d\n", room, door, i3);
            if (CGameWork::getFlag(1, 0x33)) {
                CGameWork::setFlag(1, 0x34, TRUE);
                m_pCurrentActor->_ATP.status = 25;
                break;
            }
            if (room < 0 || door < 0) {
                if (room == -2) {
                    INT mode = CGame::getDbgGameMode();
                    if (mode < 9) {
                        if (mode >= 7) {
                            CGame::reqCompleteGame();
                            break;
                        }
                    }
                    CSceneInfo::reqChangeGoalRoom();
                    break;
                } else if (room == -3) {
                    CGame::reqCompleteGame();
                    break;
                } else {
                    RoomInfo* pRoom = CSceneInfo::getRoomInfo(-1);
                    BOOL bFound = FALSE;
                    if (room == -1) {
                        if (!pRoom || !roomHasStorageInfo(pRoom)) {
                            CSceneInfo::reqChangeScene(CSceneInfo::getCurrentAreaNum(), CSceneInfo::getCurrentSceneNum(), -1, 0, 1);
                            break;
                        }
                        INT i;
                        for (i = 0; i < 4; i++) {
                            if (pRoom->_doors[i].getDoorState() == 1) {
                                door = pRoom->_doors[i]._id;
                                bFound = TRUE;
                                break;
                            }
                        }
                    }
                    if (bFound) {
                        CSceneInfo::reqEnterDoor((DOOR_ID)door);
                        break;
                    }
                    if (room == -1) room = CSceneInfo::getPrevRoomID();
                    if (door == -1) door = CSceneInfo::getEnteredDoorID();
                    switch (CSceneInfo::getEnteredDoorID()) {
                    default:
                        door = 2;
                        break;
                    case 1:
                        door = 3;
                        break;
                    case 2:
                        door = 0;
                        break;
                    case 3:
                        door = 1;
                        break;
                    }
                }
            }
            if (!CSceneInfo::reqChangeRoom(room, (DOOR_ID)door, -1)) {
                TRACE("\x95\x94\x89\xae\x82\xcc\x83V\x81[\x83\x93\x82\xaa\x82\xc8\x82\xa2\x82\xa9\x82\xe7\x82\xc6\x82\xe8\x82\xa0\x82\xa6\x82\xb8\x93K\x93\x96\x82\xc9\x8ew\x92\xe8\x82\xb5\x82\xc4\x94\xf2\x82\xce\x82\xb7\n");
                CSceneInfo::reqChangeRoom(room, (DOOR_ID)door, 3);
            }
            break;
        }
        case 0xEE: {
            INT i1 = m_S[m_SP].I;
            TRACE("EnterBattle:%d\n", i1);
            if (CGameWork::getFlag(1, 0x33)) {
                CGameWork::setFlag(1, 0x34, TRUE);
                m_pCurrentActor->_ATP.status = 25;
                break;
            }
            if (CSceneInfo::getCurrentSceneNum() < 2000) CGameWork::setFlag(1, 5, TRUE);
            CSceneInfo::reqEnterBattle(i1);
            break;
        }
        case 0xB1: {
            FLOAT fFar = m_S[m_SP--].F;
            FLOAT fNear = m_S[m_SP].F;
            CDeFocus::setManual(fNear, fFar);
            _TRACE(m_bTrace, "deFocus_man %f %f\n", fNear, fFar);
            break;
        }
        case 0xB3: {
            AvatarScaleParam param;
            param._nTime = m_S[m_SP--].I;
            param._vTarget.w = 0.0f;
            param._vTarget.z = m_S[m_SP--].F;
            param._vTarget.y = m_S[m_SP--].F;
            param._vTarget.x = m_S[m_SP].F;
            if (m_pCurrentActor->_pAvatar) {
                m_pCurrentActor->_pAvatar->reqCommand(0xF, &param, 0);
                _TRACE(m_bTrace, "--- SCALING - player ---\n");
            }
            break;
        }
        case 0xBA: {
            AvatarMotionParamEx param;
            param._fPlaySpeed = m_S[m_SP--].F;
            param._nDakkoRsrcID = m_S[m_SP--].I;
            param._nDakkoMotionID = m_S[m_SP--].I;
            param._nRsrcID = m_S[m_SP--].I;
            i1 = m_S[m_SP].I;
            param._nMotionID = i1 & 0xFFF;
            param._nFlag = (i1 >> 12) & 3;
            if (m_pCurrentActor->_pAvatar) m_pCurrentActor->_pAvatar->reqCommand(0x15, &param, 0);
            break;
        }
        case 0xBB: {
            FadeCreateParam param;
            param.layer = m_S[m_SP--].I;
            param.fRate = m_S[m_SP--].F;
            param.kind = m_S[m_SP].I;
            CAtel* pAtel = (CAtel*)CTaskMan::getSysTask(0x1A);
            pAtel->setScreenFader(param.kind, param.fRate, param.layer);
            TRACE("--- WNDCMD_FADER ---\n");
            break;
        }
        case 0xBC: {
            AvatarMotionParamC param;
            param._fPlaySpeed = m_S[m_SP--].F;
            i1 = m_S[m_SP--].I;
            c = m_S[m_SP].I;
            nID = searchActor((CHAR*)(m_pDseg + c));
            if (nID < 0) {
                _TRACE(m_bTrace, "MTO NG01:%s\n", m_pDseg + c);
                break;
            }
            CAvatar* p = getAvatar(nID);
            if (p) {
                param._nMotionID = i1 & 0xFFF;
                param._nFlag = (i1 >> 12) & 3;
                p->reqCommand(0x13, &param, 0);
            }
            break;
        }
        case 0xBD: {
            FLOAT fRate = m_S[m_SP--].F;
            INT i5 = m_S[m_SP--].I;
            FLOAT f3 = m_S[m_SP--].F;
            FLOAT f2 = m_S[m_SP--].F;
            FLOAT f1 = m_S[m_SP--].F;
            INT i1 = m_S[m_SP--].I;
            INT nIdx = searchActor((CHAR*)(m_pDseg + m_S[m_SP].I));
            if (nIdx < 0) {
                TRACE("MTO NG01:%d\n", nIdx);
                break;
            }
            TRACE("CAR %d %f %f %f\n", i1, f1, f2, f3);
            TRACE("    %d %f\n", i5, fRate);
            if (CCameraMan::instance().reqCommand(0x452, NULL, i1) == 1) CCameraMan::instance().reqCommand(0x44F, NULL, i1);
            CameraManCreateParam param;
            param.fOffsetY = 1.5f;
            param.pathDateTargetID = nIdx;
            if (i5 >= 0) param.changeFlag = 0; else param.changeFlag = 1;
            param.pathDateLocalID = 0;
            setFVector(param.cameraPos, f1, f2, f3);
            param.changeKind = i5 & 0xFF;
            param.changeKind2 = (i5 >> 8) & 0xFF;
            param.fChangeRate = fRate + 1000.0f;
            param.pCvsm = this;
            CCameraMan::instance().reqCommand(0x44C, &param, i1);
            break;
        }
        case 0xBE: {
            FLOAT fRate = m_S[m_SP--].F;
            INT i5 = m_S[m_SP--].I;
            FLOAT f3 = m_S[m_SP--].F;
            FLOAT f2 = m_S[m_SP--].F;
            FLOAT f1 = m_S[m_SP--].F;
            INT i1 = m_S[m_SP--].I;
            INT nIdx = searchActor((CHAR*)(m_pDseg + m_S[m_SP].I));
            if (nIdx < 0) {
                TRACE("MTO NG01:%d\n", nIdx);
                break;
            }
            TRACE("REL %d %f %f %f\n", i1, f1, f2, f3);
            if (CCameraMan::instance().reqCommand(0x452, NULL, i1) == 1) CCameraMan::instance().reqCommand(0x44F, NULL, i1);
            CameraManCreateParam param;
            param.fOffsetY = 1.5f;
            param.pathDateTargetID = nIdx;
            setFVector(param.cameraPos, f1, f2, f3);
            param.changeKind = i5 & 0xFF;
            param.changeKind2 = (i5 >> 8) & 0xFF;
            param.fChangeRate = fRate + 1000.0f;
            param.pathDateLocalID = 0;
            param.changeFlag = 0;
            param.pCvsm = this;
            CCameraMan::instance().reqCommand(0x45F, &param, i1);
            break;
        }
        case 0xBF: {
            FLOAT fRate = m_S[m_SP--].F;
            INT i5 = m_S[m_SP--].I;
            FLOAT f3 = m_S[m_SP--].F;
            FLOAT f2 = m_S[m_SP--].F;
            FLOAT f1 = m_S[m_SP--].F;
            INT i1 = m_S[m_SP--].I;
            INT nIdx = searchActor((CHAR*)(m_pDseg + m_S[m_SP].I));
            if (nIdx < 0) {
                TRACE("MTO NG01:%d\n", nIdx);
                break;
            }
            TRACE("CLK %d %f %f %f\n", i1, f1, f2, f3);
            if (CCameraMan::instance().reqCommand(0x452, NULL, i1) == 1) CCameraMan::instance().reqCommand(0x44F, NULL, i1);
            CameraManCreateParam param;
            param.fOffsetY = 1.5f;
            param.pathDateTargetID = nIdx;
            setFVector(param.cameraPos, f1, f2, f3);
            param.changeKind = i5 & 0xFF;
            param.changeKind2 = (i5 >> 8) & 0xFF;
            param.fChangeRate = fRate + 1000.0f;
            param.pathDateLocalID = 0;
            param.changeFlag = 0;
            param.pCvsm = this;
            CCameraMan::instance().reqCommand(0x463, &param, i1);
            break;
        }
        case 0xC1: {
            c = m_S[m_SP--].I;
            i1 = m_S[m_SP].I;
            m_S[++m_SP].I = player_rsrc(i1, c);
            break;
        }
        case 0xC2: {
            c = m_S[m_SP--].I;
            i1 = m_S[m_SP].I;
            _TRACE(m_bTrace, "wk_CMD %x : %x \n", i1, c);
            acWaitChangeWork(*m_pCurrentActor, i1, c, m_Pctr);
            ret = ATEL_RET_WKWAIT; r = 3;
            break;
        }
        case 0xC3: {
            AvatarMoveParam param;
            param._fTime = m_S[m_SP--].F;
            param._nType = m_S[m_SP--].I;
            param._nMotion = m_S[m_SP--].I;
            param._pszPathFile = (CHAR*)(m_pDseg + m_S[m_SP].I);
            if (m_pCurrentActor->_pAvatar) {
                if (param._fTime != -1.0f) {
                    m_pCurrentActor->_pAvatar->reqCommand(0xD, &param, 0);
                } else {
                    m_pCurrentActor->_pAvatar->reqCommand(0xB, param._pszPathFile, 0);
                }
                _TRACE(m_bTrace, "--- START MOVE-ACTOR-PATH Type::%x ---\n", param._nType);
            }
            break;
        }
        case 0xC6: {
            INT i3 = m_S[m_SP--].I;
            INT i2 = m_S[m_SP--].I;
            CHAR* pName = (CHAR*)(m_pDseg + m_S[m_SP].I);
            TRACE("voice_play %s %d %d\n", pName, i2, i3);
            m_S[++m_SP].I = CSound::playVoice(pName, i2, i3);
            break;
        }
        case 0xC7:
            CSound::stopVoice(m_S[m_SP].I);
            break;
        case 0xC8: {
            INT n = m_S[m_SP].I; /* unused: its address base is reused (rematerialized) for the push, as in the original */
            m_S[++m_SP].I = CSound::isVoicePlaying();
            break;
        }
        case 0xC9: {
            INT i3 = m_S[m_SP--].I;
            INT i2 = m_S[m_SP--].I;
            i1 = m_S[m_SP].I;
            CSound::setVoiceVolume(i1, i2, i3);
            break;
        }
        case 0xCA: {
            INT i3 = m_S[m_SP--].I;
            INT i2 = m_S[m_SP--].I;
            i1 = m_S[m_SP].I;
            if (i2 == -1) i2 = 0x40;
            CSound::setVoicePan(i1, i2, i3);
            break;
        }
        case 0xCB: {
            INT i4 = m_S[m_SP--].I;
            INT i3 = m_S[m_SP--].I;
            INT i2 = m_S[m_SP--].I;
            i1 = m_S[m_SP].I;
            if (i3 == -1) i3 = 0x40;
            CSound::setVoicePanAndVol(i1, i2, i3, i4);
            break;
        }
        case 0xCC: {
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP].I;
            TRACE("voice_fadeout  %d %d\n", i1, i2);
            CSound::fadeoutVoice(i1, i2);
            break;
        }
        case 0xCD: {
            FLOAT f2 = m_S[m_SP--].F;
            FLOAT f1 = m_S[m_SP--].F;
            c = m_S[m_SP].I;
            _TRACE(m_bTrace, "defoucus %d : %f : %f \n", c, f1, f2);
            CDeFocus::BlurSet(c, f1, f2, 0);
            break;
        }
        case 0xCE: {
            i1 = m_S[m_SP--].I;
            c = m_S[m_SP--].I;
            INT nL = m_S[m_SP--].I;
            INT nS = m_S[m_SP].I;
            _TRACE(m_bTrace, "VIB \n");
            CPadIO::startVibration(nS, nL, c * 50, i1);
            break;
        }
        case 0xCF:
            c = m_S[m_SP].I;
            m_SP -= 2;
            break;
        case 0xD0: {
            INT i3 = m_S[m_SP--].I;
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP].I;
            TRACE("voice_play_id:playVsb %d %d %d\n", i1, i2, i3);
            INT n = CSound::playVsb(i1 | 0x10000000, i2, i3);
            m_S[++m_SP].I = n;
            break;
        }
        case 0xD1: {
            INT i1 = m_S[m_SP - 1].I;
            INT i2 = m_S[m_SP].I;
            INT n = 0;
            CGameWork::setByte(1, 0, n);
            if (i2 < 0x100) n++;
            CChatMan::instance().reqCommand(0x3F9, &n, i1);
            acWaitSelectPadEx(*m_pCurrentActor, i1, i2, m_Pctr);
            ret = ATEL_RET_XWAIT; r = 3;
            break;
        }
        case 0xD3:
            acWaitBusy(*m_pCurrentActor, m_S[m_SP].I, m_Pctr);
            ret = ATEL_RET_BWAIT; r = 3;
            break;
        case 0xD4:
            _TRACE(m_bTrace, "BlurDistanceSet %f\n", m_S[m_SP].F);
            CDeFocus::BlurDistanceSet(m_S[m_SP].F, 0);
            break;
        case 0xD5: {
            i1 = m_S[m_SP--].I;
            c = m_S[m_SP].I;
            INT nIdx = searchActor((CHAR*)(m_pDseg + c));
            acWaitBusyActor(*m_pCurrentActor, i1, nIdx, m_Pctr);
            ret = ATEL_RET_BAWAIT; r = 3;
            break;
        }
        case 0xEF: {
            c = m_S[m_SP].I;
            INT nIdx = searchActor((CHAR*)(m_pDseg + c));
            acWaitActor(*m_pCurrentActor, 7, nIdx, m_Pctr);
            ret = ATEL_RET_ACWAIT; r = 3;
            break;
        }
        case 0xF0: {
            AvatarModelDispParam param;
            INT i2 = m_S[m_SP--].I;
            INT i1 = m_S[m_SP].I;
            param._iNo = i2 - 1;
            param._nState = i1;
            m_pCurrentActor->_pAvatar->reqCommand(7, &param, 0);
            break;
        }
        case 0xF1: {
            FLOAT f3 = m_S[m_SP--].F;
            FLOAT f2 = m_S[m_SP--].F;
            FLOAT f1 = m_S[m_SP--].F;
            INT i1 = m_S[m_SP--].I;
            FVECTOR vPos;
            setFVector(vPos, f1, f2, f3);
            m_hEffect = CEffectMan::setEffect((CHAR*)(m_pDseg + m_S[m_SP].I), i1, (ObjType::eObjType)4, &vPos, NULL, 0.0f);
            break;
        }
        case 0xD7: {
            CHAR* pszAP = (CHAR*)(m_pDseg + m_S[m_SP--].I);
            INT name = m_S[m_SP].I;
            TRACE("%s,%s\n", pszAP, m_pDseg + name);
            nID = searchActor((CHAR*)(m_pDseg + name));
            if (m_pCurrentActor->_pAvatar) {
                m_pCurrentActor->_pAvatar->reqCommand(5, pszAP, nID);
                _TRACE(m_bTrace, "--- ATTACH - %s(%d)\n", pszAP, nID);
            }
            break;
        }
        case 0xD8: {
            i1 = m_S[m_SP].I;
            _TRACE(m_bTrace, "BlurSetEvent %f \n", i1);
            if (i1 >= 0) CDeFocus::BlurSetEvent(i1); else CDeFocus::BlurReturnEvent();
            break;
        }
        case 0xDA: {
            i1 = m_S[m_SP--].I;
            nID = searchActor((CHAR*)(m_pDseg + m_S[m_SP].I));
            if (nID < 0) {
                _TRACE(m_bTrace, "face set NG01:%s\n", m_pDseg + m_S[m_SP].I);
                break;
            }
            getAvatar(nID)->reqCommand(0x38, NULL, i1);
            break;
        }
        case 0xA9:
        case 0xAB:
        case 0xB0:
        case 0xB2:
        case 0xB7:
        case 0xB9:
        case 0xD9:
        case 0xDC:
        case 0xDD:
        case 0xDE:
        case 0xDF:
        case 0xE0:
        case 0xE2:
        case 0xE4:
        case 0xF9:
        case 0x111:
        case 0x112:
            break;
        default:
            return 4;
        }
    } while (r == 0);
    return r;
}
INT CVSM::getActorIndex(const CHAR* pszName) const {
    for (INT i = 0; i < m_ActorCount; i++) {
        if (strcmp(pszName, m_ActorTbl[i]._ATP.name) == 0) return i;
    }
    return -1;
}
INT CVSM::getActorIndex(INT nID) const {
    for (INT i = 0; i < m_ActorCount; i++) {
        if (nID == m_ActorTbl[i]._ATP.id) return i;
    }
    return -1;
}
BOOL CVSM::reqN(Actor& act, INT nTag, INT nLv, BOOL bTop) {
    ATP& atp = act._ATP;
    atp.rq_lv = nLv;
    atp.status = 1;
    atp.rq_tag = nTag;
    atp.address = atp.loc[nTag];
    atp.rq_m_actor = 0xFF;
    if (!bTop) atp.Freg = 0;
    return TRUE;
}
static inline INT _findTag(const ATP& atp, INT n) {
    for (INT i = 0; i < atp.tag_count; i++) {
        if (n == atp.tname[i]) return i;
    }
    return -1;
}
BOOL CVSM::reqNS(Actor& act, const CHAR* pszTag, INT nLv) {
    ATP& atp = act._ATP;
    INT n;
    n = pszTag[3];
    n = (n << 8) + pszTag[2];
    n = (n << 8) + pszTag[1];
    n = (n << 8) + pszTag[0];
    INT i = _findTag(atp, n);
    if (i >= 0) {
        atp.rq_tag = i;
        atp.rq_lv = nLv;
        atp.status = 1;
        atp.address = atp.loc[i];
        return TRUE;
    }
    _TRACE(TRUE, "reqNS_TAG not found\n");
    return FALSE;
}
BOOL CVSM::reqSS(const CHAR* pszTag, const CHAR* pszActor, INT nLv) {
    INT nActIdx;
    INT i;
    INT n;
    const CHAR* pszTagName = pszTag + 4;
    n = pszTag[3];
    n = (n << 8) + pszTag[2];
    n = (n << 8) + pszTag[1];
    n = (n << 8) + pszTag[0];
    _TRACE(m_bTrace, "reqSS %s %x %d\n", pszActor, n, nLv);
    nActIdx = getActorIndex(pszActor);
    if (nActIdx < 0) {
        _TRACE(TRUE, "reqSS no-actor %s:%s\n", pszActor, pszTagName);
        return FALSE;
    }
    ATP& atp = m_ActorTbl[nActIdx]._ATP;
    if (atp.tag_count == 0) {
        _TRACE(TRUE, "reqSS no-tag\n");
        return FALSE;
    }
    i = _findTag(atp, n);
    if (i >= 0) {
        RTP req;
        _setRTP(req, nLv, nActIdx, i, 0xFF);
        setReq(req);
        return TRUE;
    }
    _TRACE(TRUE, "reqSS_TAG not found\n");
    return FALSE;
}
__asm__(".rdata\n"); /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
BOOL CVSM::reqSSI(const CHAR* pszTag, INT nActIdx, INT nLv) {
    INT i;
    INT n;
    n = pszTag[3];
    n = (n << 8) + pszTag[2];
    n = (n << 8) + pszTag[1];
    n = (n << 8) + pszTag[0];
    _TRACE(m_bTrace, "reqSSI actor idx:%d %x %d\n", nActIdx, n, nLv);
    ATP& atp = m_ActorTbl[nActIdx]._ATP;
    if (atp.tag_count == 0) {
        _TRACE(TRUE, "reqSSI no-tag\n");
        return FALSE;
    }
    i = _findTag(atp, n);
    if (i >= 0) {
        RTP req;
        _setRTP(req, nLv, nActIdx, i, 0xFF);
        setReq(req);
        return TRUE;
    }
    _TRACE(TRUE, "reqSSI_TAG not found\n");
    return FALSE;
}
BOOL CVSM::reqSSW(const CHAR* pszTag, const CHAR* pszActor, INT nLv, int nAddr) {
    INT nActIdx;
    INT i;
    INT n;
    const CHAR* pszTagName = pszTag + 4;
    n = pszTag[3];
    n = (n << 8) + pszTag[2];
    n = (n << 8) + pszTag[1];
    n = (n << 8) + pszTag[0];
    nActIdx = getActorIndex(pszActor);
    if (nActIdx < 0) {
        _TRACE(TRUE, "reqSSW no-actor %s:%s\n", pszActor, pszTagName);
        return FALSE;
    }
    ATP& atp = m_ActorTbl[nActIdx]._ATP;
    if (atp.tag_count == 0) {
        _TRACE(TRUE, "reqSSW no-tag\n");
        return FALSE;
    }
    i = _findTag(atp, n);
    if (i >= 0) {
        RTP req;
        _setRTP(req, nLv, nActIdx, i, m_nCurrentActorIdx);
        setReq(req);
        _TRACE(m_bTrace, "reqSSW actor:%d tag:%d\n", nActIdx, i);
        ATP& cur = m_pCurrentActor->_ATP;
        cur.status = 4;
        cur.next_address = nAddr;
        cur.wait_timer = 1;
        return TRUE;
    }
    _TRACE(TRUE, "reqSSI_TAG not found\n");
    return FALSE;
}
BOOL CVSM::reqSSD(INT nTagIdx, INT nActIdx, INT nLv) {
    RTP rq;
    _setRTP(rq, nLv, nActIdx, nTagIdx, 0xFF);
    setReq(rq);
    return TRUE;
}
INT CVSM::searchActor(const CHAR* pszActor) const {
    TRACE("searchActor:%s\n", pszActor);
    INT i = getActorIndex(pszActor);
    if (i < 0) {
        _TRACE(TRUE, "searchActor NG\n");
        return -1;
    }
    return i;
}
INT CVSM::searchActorID(const CHAR* pszActor) const {
    TRACE("searchActorID\n");
    INT i = getActorIndex(pszActor);
    if (i < 0) {
        _TRACE(TRUE, "searchActorID NG\n");
        return -1;
    }
    return m_ActorTbl[i]._ATP.id;
}
/* Parked: same instructions and order except the wait_timer product: the original multiplies into LO ("mult $zero,s1,v1"
   + mflo before the sh), ours with the 3-operand mult into v1 (gcc's mulsi3_mult3 "l" vs "d" alternative, chosen by
   regclass: LO only when the product feeds a plain register copy, as for a call argument in PathData::delPath); ours
   is 4 bytes shorter. Line tags: status (5114) before next_address (5115), the whole product on the isNTSC line. tried:
   ternary inside/outside the product and with commuted arms, if/else, INT/USHORT/TIMEQ product locals, an INT
   multiplier local, (UINT)/(SHORT)/float operands, an inline USHORT setter, statement orders, tools/permuter.py
   10 min (50056 candidates, no better than 4); retry: an inline INT-returning product helper, an inline INT-parameter
   setter, a widening (long) product (gives mult + mflo, but also mfhi and the 64-bit merge), a register asm v1
   temporary (combine folds it into the mult3). Every original "mult $zero" + mflo in the game feeds a call argument. */
#if 0
void CVSM::acWaitReq(Actor& act, INT nTime, INT nAddr) {
    ATP& atp = act._ATP;
    atp.status = 3;
    atp.next_address = nAddr;
    atp.wait_timer = nTime * (CMainApp::isNTSC() ? 25 : 25);
    _TRACE(m_bTrace, "acWaitReq act:%d time:%d address:%d \n", atp.id, nTime, nAddr);
}
#else
/* placeholder: rodata of acWaitReq__Q24ATEL4CVSMRQ24ATEL5Actorii */
__asm__(".rdata\n");
inline void _roPh35() {
    TRACE("acWaitReq act:%d time:%d address:%d \n");
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", acWaitReq__Q24ATEL4CVSMRQ24ATEL5Actorii);
#endif
__asm__(".rdata\n"); /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
void CVSM::acWaitLoadRsrc(Actor& act, INT nAddr) {
    ATP& atp = act._ATP;
    atp.next_address = nAddr;
    atp.status = 26;
    atp.wait_key = 0;
    _TRACE(m_bTrace, "acWaitLoadRsrc act:%d address:%d \n", atp.id, nAddr);
}
void CVSM::acWaitCardWind(Actor& act, INT nAddr) {
    ATP& atp = act._ATP;
    atp.next_address = nAddr;
    atp.status = 27;
    atp.wait_key = 0;
    _TRACE(m_bTrace, "acWaitCardWind act:%d address:%d \n", atp.id, nAddr);
}
void CVSM::acWaitTechWind(Actor& act, INT nAddr) {
    ATP& atp = act._ATP;
    atp.next_address = nAddr;
    atp.status = 28;
    atp.wait_key = 0;
    _TRACE(m_bTrace, "acWaitTechWind act:%d address:%d \n", atp.id, nAddr);
}
void CVSM::acWaitWorldCardWind(Actor& act, INT nAddr) {
    ATP& atp = act._ATP;
    atp.next_address = nAddr;
    atp.status = 29;
    atp.wait_key = 0;
    _TRACE(m_bTrace, "acWaitWorldCardWind act:%d address:%d \n", atp.id, nAddr);
}
void CVSM::acWaitCrossFade(Actor& act, INT nAddr) {
    ATP& atp = act._ATP;
    atp.next_address = nAddr;
    atp.status = 30;
    atp.wait_key = 0;
    _TRACE(m_bTrace, "acWaitCrossFade act:%d address:%d \n", atp.id, nAddr);
}
void CVSM::acWaitPad(Actor& act, INT nKey, INT nAddr) {
    ATP& atp = act._ATP;
    atp.status = 7;
    atp.wait_key = nKey;
    atp.next_address = nAddr;
    m_PadWaitTime = 1;
    _TRACE(m_bTrace, "acWaitPad act:%d key:%d address:%d \n", atp.id, nKey, nAddr);
}
void CVSM::acWaitMes(Actor& act, INT nKey, INT nAddr) {
    ATP& atp = act._ATP;
    atp.wait_key = nKey;
    atp.status = 9;
    atp.next_address = nAddr;
    _TRACE(m_bTrace, "acWaitMes act:%d key:%d address:%d \n", atp.id, nKey, nAddr);
}
void CVSM::acWaitCam(Actor& act, INT nKey, INT nAddr) {
    ATP& atp = act._ATP;
    atp.wait_key = nKey;
    atp.status = 10;
    atp.next_address = nAddr;
    _TRACE(m_bTrace, "acWaitCam act:%d key:%d address:%d \n", atp.id, nKey, nAddr);
}
void CVSM::acWaitBusy(Actor& act, INT nKey, INT nAddr) {
    ATP& atp = act._ATP;
    atp.wait_key = nKey;
    atp.status = 20;
    atp.next_address = nAddr;
    _TRACE(m_bTrace, "acWaitBusy act:%d key:%d address:%d \n", atp.id, nKey, nAddr);
}
void CVSM::acWaitBusyActor(Actor& act, INT nKey, INT nWin, INT nAddr) {
    ATP& atp = act._ATP;
    atp.next_address = nAddr;
    atp.wait_key = nKey;
    atp.status = 21;
    atp.wait_timer = nWin;
    _TRACE(m_bTrace, "acWaitBusyActor act:%d key:%d win:%d address:%d \n", atp.id, nKey, nWin, nAddr);
}
void CVSM::acWaitActor(Actor& act, INT nKey, INT nWin, INT nAddr) {
    ATP& atp = act._ATP;
    atp.next_address = nAddr;
    atp.wait_key = nKey;
    atp.status = 24;
    atp.wait_timer = nWin;
    _TRACE(m_bTrace, "actor_wait act:%d key:%d win:%d address:%d \n", atp.id, nKey, nWin, nAddr);
}
void CVSM::acWaitRsrc(Actor& act, INT nKey, INT nAddr) {
    ATP& atp = act._ATP;
    atp.wait_key = nKey;
    atp.status = 16;
    atp.next_address = nAddr;
    _TRACE(m_bTrace, "acWaitRsrc act:%d key:%d address:%d \n", atp.id, nKey, nAddr);
}
void CVSM::acWaitSnatch(Actor& act, INT nKey, INT nAddr) {
    ATP& atp = act._ATP;
    atp.wait_key = nKey;
    atp.status = 11;
    atp.wait_timer = 1;
    atp.next_address = nAddr;
    _TRACE(m_bTrace, "acWaitSnatch act:%d key:%d address:%d \n", atp.id, nKey, nAddr);
}
void CVSM::acWaitEquip(Actor& act, INT nKey, INT nAddr) {
    ATP& atp = act._ATP;
    atp.wait_key = nKey;
    atp.status = 17;
    atp.next_address = nAddr;
    _TRACE(m_bTrace, "acWaitEquip act:%d key:%d address:%d \n", atp.id, nKey, nAddr);
}
void CVSM::acWaitDelLeftSword(Actor& act, INT nKey, INT nAddr) {
    ATP& atp = act._ATP;
    atp.wait_key = nKey;
    atp.status = 22;
    atp.next_address = nAddr;
    _TRACE(m_bTrace, "acWaitDelLeftSword act:%d key:%d address:%d \n", atp.id, nKey, nAddr);
}
void CVSM::acWaitLoadEnemy(Actor& act, INT nAddr) {
    ATP& atp = act._ATP;
    atp.next_address = nAddr;
    atp.status = 15;
    atp.wait_key = 0;
    _TRACE(m_bTrace, "acWaitLoadEnemy act:%d address:%d \n", atp.id, nAddr);
}
void CVSM::acWaitSelectPad(Actor& act, INT nKey, INT nWin, INT nAddr) {
    ATP& atp = act._ATP;
    atp.wait_key = nWin;
    atp.wait_timer = nKey;
    atp.status = 8;
    atp.next_address = nAddr;
    _TRACE(m_bTrace, "acWaitSelectPad select:%d time:%d address:%d \n", atp.id, nKey, nAddr);
}
void CVSM::acWaitSelectPadEx(Actor& act, INT nKey, INT nWin, INT nAddr) {
    ATP& atp = act._ATP;
    atp.wait_key = nWin;
    atp.wait_timer = nKey;
    atp.status = 19;
    atp.next_address = nAddr;
    _TRACE(m_bTrace, "acWaitSelectPadEx select:%d time:%d address:%d \n", atp.id, nKey, nAddr);
}
void CVSM::acWaitChangeWork(Actor& act, INT nKey, INT nVal, INT nAddr) {
    ATP& atp = act._ATP;
    atp.wait_timer = nVal;
    atp.next_address = nAddr;
    atp.wait_key = nKey;
    atp.status = 18;
    _TRACE(m_bTrace, "acWaitChangeWork wait:%d time:%d address:%d \n", nKey, nVal, nAddr);
}
BOOL CVSM::acStatNop() {
    return FALSE;
}
BOOL CVSM::acStatExec() {
    m_Pctr = m_pCurrentActor->_ATP.address;
    return TRUE;
}
BOOL CVSM::acStatWait() {
    ATP& atp = m_pCurrentActor->_ATP;
    TIMEQ timer = atp.wait_timer;
    TIMEQ nTQ = m_nFrameRate;
    if (CMainApp::isNTSC()) {
        nTQ >>= 1;
    } else {
        nTQ >>= 1;
    }
    if (timer < nTQ) {
        _TRACE(m_bTrace, " wait_mode change = %d\n", atp.id);
        setPC(atp.next_address);
        return TRUE;
    }
    atp.wait_timer = timer - nTQ;
    return FALSE;
}
BOOL CVSM::acStatRWait() {
    ATP& atp = m_pCurrentActor->_ATP;
    if (atp.wait_timer) return FALSE;
    _TRACE(m_bTrace, "rwait mode change %d\n", atp.id);
    m_Pctr = atp.next_address;
    return TRUE;
}
BOOL CVSM::acStatCWait() {
    ATP& atp = m_pCurrentActor->_ATP;
    if (!m_pCurrentActor->_pAvatar->isAvatarBusy()) {
        _TRACE(m_bTrace, "rcf_wait ok =%d\n", atp.id);
        m_Pctr = atp.next_address;
        return TRUE;
    }
    return FALSE;
}
BOOL CVSM::acStatIWait() {
    ATP& atp = m_pCurrentActor->_ATP;
    if (!(m_nFlag & 2)) {
        _TRACE(m_bTrace, "sys_wait ok no=%d adr:%d\n", atp.id, atp.address);
        m_Pctr = atp.address;
        return TRUE;
    }
    return FALSE;
}
BOOL CVSM::acStatPWait() {
    if (CMainApp::isPauseNow()) return FALSE;
    if (m_PadWaitTime == 1) {
        m_PadWaitTime = 0;
        return FALSE;
    }
    ATP& atp = m_pCurrentActor->_ATP;
    INT n = atp.wait_key;
    switch (n) {
    case 0:
    case 1:
    case 16:
        if (CPadHandler::isTrgTalkMesNext()) {
            m_Pctr = atp.next_address;
            setFlag(4);
            CSound::playSE(10, -1, 0x40);
            return TRUE;
        }
        break;
    default:
        _TRACE(m_bTrace, "pad_wait parm error\n");
        break;
    }
    return FALSE;
}
BOOL CVSM::acStatSWait() {
    ATP& atp = m_pCurrentActor->_ATP;
    if (CPadHandler::isTrgSelect()) {
        _TRACE(m_bTrace, "pad_sel_wait_mode change = %d\n", atp.id);
        CSound::playSE(2, -1, 0x40);
        m_Pctr = atp.next_address;
        return TRUE;
    }
    INT nSelMax = (UBYTE)atp.wait_key;
    INT nSelIdx = CGameWork::getByte(1, 0);
    BOOL bUpdate = FALSE;
    if (CPadHandler::isButtonTrg(0x101000) || CPadHandler::isButtonRpt(0x101000)) {
        if (nSelIdx - 1 >= 0) {
            nSelIdx--;
            bUpdate = TRUE;
        }
    } else if (CPadHandler::isButtonTrg(0x204000) || CPadHandler::isButtonRpt(0x204000)) {
        if (nSelIdx + 1 < nSelMax) {
            nSelIdx++;
            bUpdate = TRUE;
        }
    }
    if (bUpdate) {
        _TRACE(m_bTrace, "SELECT STR %d\n", nSelIdx);
        CGameWork::setByte(1, 0, nSelIdx);
        if (atp.wait_key < 0x100) nSelIdx++;
        CChatMan::instance().reqCommand(0x3F9, &nSelIdx, atp.wait_timer);
    }
    return FALSE;
}
__asm__(".rdata\n"); /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
BOOL CVSM::acStatMWait() {
    ATP& atp = m_pCurrentActor->_ATP;
    INT nWin = atp.wait_key;
    INT nFlag;
    if (CChatMan::instance().reqCommand(0x3ED, &nFlag, nWin) == -1)
        nFlag = 9;
    if (nFlag == 5 || nFlag == 0 || nFlag == 9) {
        _TRACE(m_bTrace, "win_wait_mode change = %d\n", atp.id);
        m_Pctr = atp.next_address;
        return TRUE;
    }
    return FALSE;
}
__asm__(".rdata\n"); /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
BOOL CVSM::acStatCPWait() {
    ATP& atp = m_pCurrentActor->_ATP;
    INT nID = atp.wait_key;
    INT nFlag;
    INT nRet = CCameraMan::instance().reqCommand(0x453, &nFlag, nID);
    if (nRet == 1) {
        if (nFlag == 1) {
            _TRACE(m_bTrace, "cam_wait_mode change = %d\n", atp.id);
            m_Pctr = atp.next_address;
            return TRUE;
        }
    }
    return FALSE;
}
BOOL CVSM::acStatSNWait() {
    return FALSE;
}
BOOL CVSM::acStatMSWait() {
    ATP& atp = m_pCurrentActor->_ATP;
    INT nID = atp.wait_key;
    INT nFlag;
    if (CChatMan::instance().reqCommand(0x3ED, &nFlag, nID) == -1) nFlag = 5;
    if (nFlag == 5) {
        _TRACE(m_bTrace, "mes_disp_wait = %d\n", atp.id);
        setPC(atp.next_address);
        return TRUE;
    }
    return FALSE;
}
BOOL CVSM::acStatShopWait() {
    return FALSE;
}
BOOL CVSM::acStatDropWait() {
    return FALSE;
}
BOOL CVSM::acStatRsrcLoadWait() {
    if (CRcf::isCompleteLoadRCF(m_pCurrentActor->_GroupID, m_pCurrentActor->_nMembarID)) {
        _TRACE(m_bTrace, "  -->load Rsrc data ok\n");
        m_Pctr = m_pCurrentActor->_ATP.next_address;
        return TRUE;
    }
    return FALSE;
}
BOOL CVSM::acStatCardWndWait() {
    if (Gauge::CPrizeGauge::isNowDispCard()) return FALSE;
    TRACE("  -->Card Wnd Closed ok\n");
    m_Pctr = m_pCurrentActor->_ATP.next_address;
    return TRUE;
}
BOOL CVSM::acStatTechWndWait() {
    if (CLevelUp::isEnd()) {
        TRACE("  -->Tech Wnd Closed ok\n");
        m_Pctr = m_pCurrentActor->_ATP.next_address;
        return TRUE;
    }
    return FALSE;
}
BOOL CVSM::acStatWorldCardWndWait() {
    if (WorldCard::CWorldCardSeq::isNowDispCard()) return FALSE;
    TRACE("  -->World Card Wnd Closed ok\n");
    m_Pctr = m_pCurrentActor->_ATP.next_address;
    return TRUE;
}
BOOL CVSM::acStatCrossFadeWait() {
    CAtelCrossFade* p = CAtelCrossFade::m_pInstance;
    if (p && p->isCrossFadeDrawImage()) return FALSE;
    TRACE("  -->Cross Fade End\n");
    m_Pctr = m_pCurrentActor->_ATP.next_address;
    return TRUE;
}
BOOL CVSM::acStatEnemyLoadWait() {
    return FALSE;
}
BOOL CVSM::acStatRCWait() {
    m_Pctr = m_pCurrentActor->_ATP.next_address;
    return TRUE;
}
BOOL CVSM::acStatEQWait() {
    m_Pctr = m_pCurrentActor->_ATP.next_address;
    return TRUE;
}
BOOL CVSM::acStatWKWait() {
    m_Pctr = m_pCurrentActor->_ATP.next_address;
    return TRUE;
}
BOOL CVSM::acStatXWait() {
    if (CPadHandler::isTrgCANCEL()) {
        CGameWork::setByte(1, 0, 0xFF);
        _TRACE(m_bTrace, " pad_sel_xwait_mode change = %d\n", m_pCurrentActor->_ATP.id);
        m_Pctr = m_pCurrentActor->_ATP.next_address;
        return TRUE;
    }
    return acStatSWait();
}
BOOL CVSM::isAvatarBusy(INT nType, CAvatar* pAvatar) {
    BOOL bRes = FALSE;
    switch (nType) {
    case 0: bRes = pAvatar->getAvatarState() & 0x2; break;
    case 1: bRes = pAvatar->getAvatarState() & 0x10; break;
    case 2: bRes = pAvatar->getAvatarState() & 0x40; break;
    case 3: bRes = pAvatar->getAvatarState() & 0x100; break;
    case 7:
        /* here the "avatar" argument is an actor index (see acStatActorWait) */
        return m_ActorTbl[(INT)pAvatar]._ATP.rq_tag == 1;
    case 8: bRes = pAvatar->getAvatarState() & 0x400; break;
    case 9: bRes = pAvatar->getAvatarState() & 0x1000; break;
    case 10: bRes = pAvatar->getAvatarState() & 0x4000; break;
    default:
        _TRACE(m_bTrace, "NG Buzy param\n");
        break;
    }
    return bRes;
}
__asm__(".rdata\n"); /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
BOOL CVSM::acStatBSWait() {
    ATP& atp = m_pCurrentActor->_ATP;
    if (isAvatarBusy(atp.wait_key, m_pCurrentActor->_pAvatar)) return FALSE;
    _TRACE(m_bTrace, " busy_wait_mode change = %d:%d\n", atp.id, atp.wait_key);
    m_Pctr = atp.next_address;
    return TRUE;
}
BOOL CVSM::acStatBAWait() {
    ATP& atp = m_pCurrentActor->_ATP;
    if (isAvatarBusy(atp.wait_key, getAvatar(atp.wait_timer))) return FALSE;
    _TRACE(m_bTrace, " busy_wait_actor_mode change = %d:%d\n", atp.id, atp.wait_key);
    m_Pctr = atp.next_address;
    return TRUE;
}
BOOL CVSM::acStatActorWait() {
    ATP& atp = m_pCurrentActor->_ATP;
    /* the original passes wait_timer (lhu 0x1b2) as the avatar argument */
    if (isAvatarBusy(atp.wait_key, (CAvatar*)atp.wait_timer) != TRUE) return FALSE;
    _TRACE(m_bTrace, "actor_wait _mode change = %d:%d\n", atp.id, atp.wait_key);
    m_Pctr = atp.next_address;
    return TRUE;
}
BOOL CVSM::acStatEternityWait() {
    return FALSE;
}
BOOL CVSM::acStatDLWait() {
    m_Pctr = m_pCurrentActor->_ATP.next_address;
    return FALSE;
}
BOOL CVSM::acStatBGWait() {
    return FALSE;
}
/* f goes through a reference: its stack store then is no fixed scalar for the scheduler (see execCode) */
static inline void setFloat(FLOAT& r, FLOAT v) { r = v; }
INT CVSM::player_ctrl(INT nCode) {
    FLOAT f;
    INT index;
    switch (nCode) {
    case 2:
        m_pCurrentActor->_pAvatar->reqCommand(0x6, NULL, 0);
        TRACE("SHOW :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 3:
        m_pCurrentActor->_pAvatar->reqCommand(0x6, NULL, 1);
        TRACE("HIDE :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 6:
        m_pCurrentActor->_pAvatar->reqCommand(0x1D, NULL, 1);
        TRACE("GRAVITY ON :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 7:
        m_pCurrentActor->_pAvatar->reqCommand(0x1D, NULL, 0);
        TRACE("GRAVITY OFF :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 8:
        m_pCurrentActor->_pAvatar->reqCommand(0x1E, NULL, 1);
        TRACE("MAP_COLL_ON :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 9:
        m_pCurrentActor->_pAvatar->reqCommand(0x1E, NULL, 0);
        TRACE("MAP_COLL_OFF :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 10:
        m_pCurrentActor->_pAvatar->reqCommand(0x1F, NULL, 1);
        TRACE("BODY_COLL_ON :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 11:
        m_pCurrentActor->_pAvatar->reqCommand(0x1F, NULL, 0);
        TRACE("BODY_COLL_OFF :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 39:
        m_pCurrentActor->_pAvatar->reqCommand(0x1, NULL, 0);
        m_pCurrentActor->_ATP.status = 25;
        index = searchActor(m_pCurrentActor->_ATP.name);
        m_ActorTbl[index]._pAvatar = NULL;
        m_pCurrentActor->_pAvatar = NULL;
        TRACE("REMOVE NPC :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 40:
        m_pCurrentActor->_pAvatar->reqCommand(0x1A, NULL, 1);
        TRACE("MAKEUP_MOTION_INVALID :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 41:
        m_pCurrentActor->_pAvatar->reqCommand(0x1A, NULL, 0);
        TRACE("MAKEUP_MOTION_VALID :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 42:
        m_pCurrentActor->_pAvatar->reqCommand(0x34, NULL, 0);
        TRACE("AVTCMD_SET_LOOK_START :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 43:
        m_pCurrentActor->_pAvatar->reqCommand(0x35, NULL, 0);
        TRACE("AVTCMD_SET_LOOK_STOP :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 44:
        setFloat(f, 16.0f);
        m_pCurrentActor->_pAvatar->reqCommand(0x35, NULL, 0);
        m_pCurrentActor->_pAvatar->reqCommand(0x36, &f, 0);
        TRACE("AVTCMD_SET_LOOK_END :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 45:
        m_pCurrentActor->_pAvatar->reqCommand(0x24, NULL, 0);
        TRACE("AVTCMD_CREATE_LEGIK :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 46:
        m_pCurrentActor->_pAvatar->reqCommand(0x21, NULL, 1);
        TRACE("AVTCMD_SET_IK  ON :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 47:
        m_pCurrentActor->_pAvatar->reqCommand(0x21, NULL, 0);
        TRACE("AVTCMD_SET_IK  OFF :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 48:
        m_pCurrentActor->_pAvatar->reqCommand(0x22, NULL, 1);
        TRACE("AVTCMD_SET_LOCK_ON  ON :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 49:
        m_pCurrentActor->_pAvatar->reqCommand(0x22, NULL, 0);
        index = searchLockOnNpc();
        if (index != -1 && m_ActorTbl[index]._pAvatar == m_pCurrentActor->_pAvatar) g_pPlayer->cancelLockon();
        TRACE("AVTCMD_SET_LOCK_ON  OFF :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 50:
        m_pCurrentActor->_pAvatar->reqCommand(0x23, NULL, 1);
        TRACE("AVTCMD_SET_NEAR_CLIP  OFF :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 51:
        m_pCurrentActor->_pAvatar->reqCommand(0x23, NULL, 0);
        TRACE("AVTCMD_SET_NEAR_CLIP  OFF :%s\n", m_pCurrentActor->_ATP.name);
        break;
    case 52:
        m_pCurrentActor->_pAvatar->reqCommand(0x2F, NULL, 0);
        TRACE("AVTCMD_START_AVATAR  :%s\n", m_pCurrentActor->_ATP.name);
        break;
    default:
        _TRACE(m_bTrace, "bad param player_ctrl :%s\n", m_pCurrentActor->_ATP.name);
        break;
    }
    return 0;
}
INT CVSM::player_ctrl(INT nCode, FLOAT fArg) {
    if (m_pCurrentActor->_pAvatar) _TRACE(m_bTrace, "bad param player_control\n");
    return -2;
}
BOOL CVSM::cam_ctrl(INT nCmnd, INT nID, INT& nRet) {
    switch (nCmnd) {
    case 0: CCameraMan::instance().reqCommand(0x44F, NULL, nID); break;
    case 1: CCameraMan::instance().reqCommand(0x450, NULL, nID); break;
    case 2: CCameraMan::instance().reqCommand(0x451, NULL, nID); break;
    case 3:
        if (CCameraMan::instance().reqCommand(0x453, &nRet, nID) != 1) nRet = 0;
        break;
    case 4: nRet = CCameraMan::instance().reqCommand(0x452, &nRet, nID); break;
    case 5: CCameraMan::instance().reqCommand(0x455, NULL, 0); break;
    case 6: CCameraMan::instance().reqCommand(0x457, NULL, 0); break;
    case 7: CDeFocus::off(); break;
    case 8: CDeFocus::on(); break;
    case 9: CDeFocus::offNear(); break;
    case 10: CDeFocus::onNear(); break;
    case 11: CDeFocus::offFar(); break;
    case 12: CDeFocus::setAutomatic(); break;
    case 13: CDeFocus::push(); break;
    case 14: CDeFocus::pop();
    case 15: CCameraMan::instance().reqCommand(0x45D, NULL, nID); break;
    case 16: CCameraMan::instance().reqCommand(0x45E, NULL, nID); break;
    case 17: CCameraMan::instance().reqCommand(0x460, NULL, 0); break;
    case 19: CCameraMan::instance().reqCommand(0x465, NULL, nID); break;
    default:
        _TRACE(m_bTrace, "MESS_CMD bad param \n");
        break;
    }
    return FALSE;
}
__asm__(".rdata\n"); /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
void CVSM::info_disp(INT nCmnd) {
    _TRACE(m_bTrace, "NG INF-param\n");
}
BOOL CVSM::actor_ref(INT nCmnd, INT nArg) {
    _TRACE(m_bTrace, "bad param Actor_REF\n");
    return FALSE;
}
void CVSM::cam_ajt(INT nCmnd, F_UL& arg, INT nID) {
    _TRACE(m_bTrace, "CAM_AJT bad param \n");
}
BOOL CVSM::player_rsrc(INT nCmnd, INT nArg) {
    _TRACE(m_bTrace, "--- player_rsrc %d : %d \n ---\n", nCmnd, nArg);
    _TRACE(m_bTrace, "bad case RSC_CMD %d\n", nArg);
    return FALSE;
}
FLOAT RotTransPers(FVECTOR& outV, const FMATRIX& inM, const FVECTOR& inV); /* System/Graphic/Gprim.h in the original */
extern INT SCREEN_HEIGHT;
void CVSM::ctd_cal(const CAvatar* in_pAvatar, int i2, int i3, ChatWndCreateCTDParam* CWCP) {
    i3 &= 0x7FFFFFFF;
    if (i3 > 0xFFFFF) {
        int i4;
        int ix;
        int iy;
        int x;
        int y;
        int hx;
        int hy;
        i4 = i3 >> 20;
        TRACE("chobi-man %d\n", i4);
        CtdBlock* pBlock = CCtdMan::getCtdBlock(i2, NULL);
        INT style = (UBYTE)pBlock->wndStyle;
        CSizeI size(pBlock->sizeX, pBlock->sizeY);
        CPointI targetPos;
        targetPos.setPoint(pBlock->posX, pBlock->posY);
        switch (style) {
        case 0: break;
        case 1: targetPos.m_x = 0x20; break;
        case 2: targetPos.m_x = 0x100 - (size.m_w >> 1); break;
        case 3: targetPos.m_x = 0x1D0 - size.m_w; break;
        }
        x = size.m_w >> 1;
        y = size.m_h >> 1;
        ix = targetPos.m_x + x;
        iy = targetPos.m_y + y;
        hx = (UINT)(x * 1.6);
        hy = size.m_h + 0x20;
        switch (i4) {
        case 1: ix -= hx; iy -= hy; break;
        case 2: iy -= hy; break;
        case 3: ix += hx; iy -= hy; break;
        case 4: ix -= hx; iy += hy; break;
        case 5: iy += hy; break;
        case 6: ix += hx; iy += hy; break;
        }
        if (ix < 0) ix = 0;
        if (iy < 0) iy = 0;
        TRACE(">>>>>chibi(CMA) pos %d : %d\n", ix, iy);
        CWCP->cyobiPos.setPoint(ix, iy);
    } else if (i3 & 0x10000) {
        if (in_pAvatar) {
            INT BoneIndex = in_pAvatar->getBoneIndex("neck", 0);
            if (BoneIndex == -1) BoneIndex = in_pAvatar->getBoneIndex("neck1", 0);
            if (BoneIndex == -1) BoneIndex = in_pAvatar->getBoneIndex("head", 0);
            FVECTOR vWorld;
            FVECTOR vPos;
            if (BoneIndex == -1) {
                TRACE("ATEL:CVSM::ctd_cal\x81" "F\x83\x81\x83" "b\x83Z\x81[\x83W\x83" "E\x83" "C\x83\x93\x83h\x83" "E\x81" "F\x83`\x83\x87\x83r\x82\xf0\x8c\xfc\x82\xaf\x82\xe9\x83{\x81[\x83\x93\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81" "E\x81" "E\x81" "E\n");
                in_pAvatar->getNowPosition(vWorld);
                RotTransPers(vPos, CWorldEnv::getWorldScreenMatrix(), vWorld);
                UINT x = (UINT)(vPos.x - 1792.0f);
                UINT y = (UINT)(vPos.y - 1880.0f);
                CWCP->cyobiPos.setPoint(x, y);
                return;
            }
            RotTransPers(vPos, CWorldEnv::getWorldScreenMatrix(), in_pAvatar->Crd(BoneIndex).m_World.v[3]);
            UINT x = (UINT)(vPos.x - 1792.0f);
            UINT y = (UINT)(vPos.y - (FLOAT)(2048 - SCREEN_HEIGHT / 2));
            CWCP->cyobiPos.setPoint(x, y);
            if (x == 0 && y == 0) CWCP->cyobiPos.setPoint(1, 1);
        } else {
            CWCP->cyobiPos.setPoint(1, 1);
        }
    } else {
        CWCP->cyobiPos.setPoint(0, 0);
    }
}
__asm__(".rdata\n"); /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
void CVSM::AtelEventSkip() {
    TRACE("Event Skip Start\n");
    CGameWork::setFlag(1, 0x38, TRUE);
    CSceneEnv::setStop((ObjType::eObjBit)0);
    CSceneEnv::setStop((ObjType::eObjBit)0x581C);
    clearReq();
    CCameraMan::instance().reqCommand(0x45D, NULL, 0);
    for (int i = 0; i < m_ActorCount; i++) {
        if (m_ActorTbl[i]._pAvatar) m_ActorTbl[i]._pAvatar->reqCommand(0x2E, NULL, 0);
        if (!reqSSI("@_sk", i, 7)) {
            m_ActorTbl[i]._ATP.status = 25;
            TRACE("      GOTO_ EternityWait:%s\n", m_ActorTbl[i]._ATP.name);
        } else {
            tuneReq();
            m_ActorTbl[i]._ATP.status = 1;
            m_Pctr = m_ActorTbl[i]._ATP.address;
            TRACE("[ATEL]   GOTO_Skip:%s\n", m_ActorTbl[i]._ATP.name);
        }
    }
}
void close_dest_se(INT win_num) {
    INT nState;
    INT ans;
    INT mode;
    ans = CChatMan::instance().reqCommand(0x3ED, &nState, win_num);
    if (ans == 1) {
        WinCloseSE();
        mode = ans;
        CChatMan::instance().reqCommand(0x3EC, &mode, win_num);
    }
}
void WinOpenSE(INT ctd) {}
void WinCloseSE() {}
static void dispActorTag(INT nMax, const Actor* pTbl) {}
namespace DWTEvent {
extern DEBUG_WINDOW_LINE debugNpcInfoLine[];
}
extern INT SCREEN_HEIGHT;
CActorDebug::CActorDebug(CTask* pParent) : CTask(pParent, 0x8000), m_pWindow(NULL) {
    CTask* pA = CTaskMan::getSysTask(0x1A);
    DEBUG_WINDOW_TBL debugActorInfo = {
        ((CAtel*)pA)->getVSM()->getAvatarName(DebugWork::g_dw.event.iNpcID),
        0x90, -(SCREEN_HEIGHT >> 1) + 0x18, 7, 12, 14, 0, 6, DWTEvent::debugNpcInfoLine, NULL, NULL
    };
    m_pWindow = new (__FILE__ "(6871)") CDebugWindow(this, debugActorInfo);
    m_pWindow->dispOn(0);
}
CActorDebug::~CActorDebug() {
    if (m_pWindow) m_pWindow->out();
}
void CActorDebug::main() {
    CAtel* pA = (CAtel*)CTaskMan::getSysTask(0x1A);
    if (DebugWork::g_dw.event.iNpcCoordDisp) {
        if (!pA) return;
        CAvatar* p = pA->getVSM()->getAvatar(DebugWork::g_dw.event.iNpcID);
        if (!p) return;
        if (p->getMovement()) {
            DebugWork::g_dw.event.vPos = p->getPos();
            DebugWork::g_dw.event.vRot = p->getRot();
            DebugWork::g_dw.event.vRot.x = RAD2DEG(DebugWork::g_dw.event.vRot.x);
            DebugWork::g_dw.event.vRot.y = RAD2DEG(DebugWork::g_dw.event.vRot.y);
            DebugWork::g_dw.event.vRot.z = RAD2DEG(DebugWork::g_dw.event.vRot.z);
        }
    } else {
        DebugWork::g_dw.event.pClass = NULL;
        delete this;
    }
}
CEventLineDisp::CEventLineDisp(CTask* pParent) : CTask(pParent, 0x8000) {
    CTaskMan::getSysTask(0x1A);
}
CEventLineDisp::~CEventLineDisp() {}
void CEventLineDisp::main() {}
void CEventLineDisp::disp() {
    CAtel* at = (CAtel*)CTaskMan::getSysTask(0x1A);
    if (at && at->getVSM()) {
        for (int i = 0; i < at->getVSM()->getActorNum(); i++) {
            CEventTrap* p = at->getVSM()->getEventTrap(i);
            if (p) p->dispShape();
        }
    }
}
/* Initialized at run time (SCREEN_HEIGHT, value-name lookups by the current g_dw values): these make up the
   static initializer. */
namespace DWTEvent {
extern DEBUG_WINDOW_LINE debugKeyTresurLine[];
extern DEBUG_WINDOW_LINE debugWinCardPossibleLine[];
extern DEBUG_WINDOW_LINE debugWinEventSelectPlayerLine[];
extern DEBUG_WINDOW_LINE debugWinFriendPossibleLine[];
extern DEBUG_WINDOW_LINE debugWinJiminyLine[];
extern DEBUG_WINDOW_LINE debugWinKH2CardGetLine[];
extern DEBUG_WINDOW_LINE debugWinMovieLine[];
extern DEBUG_WINDOW_LINE debugWinMovieLineRiku[];
extern DEBUG_WINDOW_LINE debugWinPcSkillLine[];
extern DEBUG_WINDOW_LINE debugWinTuEndLine[];
extern DEBUG_WINDOW_LINE debugWinWorldIntroLine[];
extern LPCSTR szDifficultMode[];
extern LPCSTR szMenuKind[];
extern LPCSTR szMiniGameKind[];
extern LPCSTR szMiniGamePlayState[];
extern LPCSTR szMiniGameState[];
extern LPCSTR szPlayMode[];
extern LPCSTR szSoundMode[];
extern LPCSTR szStockKindName[];
static DEBUG_WINDOW_LINE debugBtlReportLine[22] = {
    { "\xf9S----------\xf9V\x83\\\x83\x89\xf9S---------", &CDebugWindow::none, 1, { 0 }, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83Q\x81[\x83\x80\x83\x8c\x83x\x83\x8b       \x81@:%s", &DebugWork::g_dw.atelwork.iPlayDifficultMode, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, &szDifficultMode[DebugWork::g_dw.atelwork.iPlayDifficultMode], 0, NULL, NULL },
    { "\x83W\x83~\x83j\x81[\x92" "B\x90\xac\x97\xa6     \x81@:%d", &DebugWork::g_dw.atelwork.BtlMemo, 4, { 0 }, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83R\x83\x8c\x83N\x83^\x81[\x83Y\x83\x89\x83\x93\x83N \x81@:--", &CDebugWindow::none, 1, { 0 }, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x93|\x82\xb5\x82\xbd\x93G\x82\xcc\x90\x94\x81@\x81@\x81@ \x81@:%d", &DebugWork::g_dw.atelwork.KillEnemyCount, 4, { 0 }, 0, 0.0f, 999999.0f, 1000.0f, NULL, 0, debugBtlReportCallBackParam, NULL },
    { "\x96\xa2\x92m\x82\xc8\x82\xe9\x95\xf3\x82\xcc\x90\x94     \x81@:%d/24", &DebugWork::g_dw.atelwork.KeysTresur, 4, { 0 }, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83J\x81[\x83h\x83u\x83\x8c\x83" "C\x83N\x89\xf1\x90\x94 \x81@:%d", &DebugWork::g_dw.atelwork.CardBreakCount, 4, { 0 }, 0, 0.0f, 99999.0f, 100.0f, NULL, 0, debugBtlReportCallBackParam, NULL },
    { "\x8d\xc5\x82\xe0\x8eg\x82\xc1\x82\xbd\x83X\x83g\x83" "b\x83N\x8bZ :%s", &DebugWork::g_dw.atelwork.StockKind, 4, { 0 }, 0, 0.0f, 78.0f, 1.0f, &szStockKindName[DebugWork::g_dw.atelwork.StockKind], 0, debugBtlReportCallBackParam, NULL },
    { "        \x81@\x81@\x81@\x81@\x89\xf1\x90\x94 :%d", &DebugWork::g_dw.atelwork.StockCount, 4, { 0 }, 0, 0.0f, 99999.0f, 100.0f, NULL, 0, debugBtlReportCallBackParam, NULL },
    { "\x83N\x83\x8a\x83" "A\x8e\x9e\x8a\xd4\x81@\x81@\x81@     :%d", &DebugWork::g_dw.atelwork.Hour, 4, { 0 }, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x81@\x81@  \x81@\x95\xaa\x81@\x81@\x81@\x81@   :%d", &DebugWork::g_dw.atelwork.minit, 4, { 0 }, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\xf9S----------\xf9V\x83\x8a\x83N\xf9S---------", &CDebugWindow::none, 1, { 0 }, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83Q\x81[\x83\x80\x83\x8c\x83x\x83\x8b\x81@\x81@\x81@\x81@ :%s", &DebugWork::g_dw.atelwork.iPlayDifficultMode, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, &szDifficultMode[DebugWork::g_dw.atelwork.iPlayDifficultMode], 0, NULL, NULL },
    { "\x82" "c\x83\x8c\x83|\x81[\x83g\x92" "B\x90\xac\x97\xa6     :%d", &DebugWork::g_dw.atelwork.BtlMemo, 4, { 0 }, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83R\x83\x8c\x83N\x83^\x81[\x83Y\x83\x89\x83\x93\x83N   :--", &CDebugWindow::none, 1, { 0 }, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83X\x83s\x81[\x83h\x83u\x83\x8c\x83" "C\x83N     :%d", &DebugWork::g_dw.atelwork.SpeedCount, 4, { 0 }, 0, 0.0f, 99999.0f, 100.0f, NULL, 0, debugBtlReportCallBackParam, NULL },
    { "\x93|\x82\xb5\x82\xbd\x93G\x82\xcc\x90\x94\x81@\x81@     :%d", &DebugWork::g_dw.atelwork.KillEnemyCount, 4, { 0 }, 0, 0.0f, 99999.0f, 100.0f, NULL, 0, debugBtlReportCallBackParam, NULL },
    { "\x83" "f\x83\x85\x83G\x83\x8b\x82\xcc\x8f\x9f\x97\xa6       :%5.2f", &DebugWork::g_dw.atelwork.DuelWin, 4, { 0 }, 1, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x82" "c\x83\x82\x81[\x83h\x94\xad\x93\xae\x89\xf1\x90\x94     :%d", &DebugWork::g_dw.atelwork.DarkCount, 4, { 0 }, 0, 0.0f, 99999.0f, 100.0f, NULL, 0, debugBtlReportCallBackParam, NULL },
    { "\x83N\x83\x8a\x83" "A\x8e\x9e\x8a\xd4\x81@\x81@\x81@     :%d", &DebugWork::g_dw.atelwork.Hour, 4, { 0 }, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x81@\x81@  \x81@\x95\xaa\x81@\x81@\x81@\x81@   :%d", &DebugWork::g_dw.atelwork.minit, 4, { 0 }, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL },
    { NULL, NULL, 0, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
};
static DEBUG_WINDOW_TBL debugBtlReport = { "\x83o\x83g\x83\x8b\x83\x8c\x83|\x81[\x83g", -106, -(SCREEN_HEIGHT >> 1) + 150, 20, 29, 12, 0, 0, debugBtlReportLine, NULL, debugBtlReportCallBack };
static DEBUG_WINDOW_TBL debugKeyTresur = { "\x96\xa2\x92m\x82\xc8\x82\xe9\x95\xf3\x82\xa0\x82\xaf\x82\xbd\x90\x94", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 29, 12, 0, 0, debugKeyTresurLine, NULL, debugKeyTresurCallBack };
static DEBUG_WINDOW_TBL debugWinJiminy = { "\x83W\x83~\x83j\x81[\x83\x81\x83\x82", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 25, 12, 0, 0, debugWinJiminyLine, NULL, NULL };
static DEBUG_WINDOW_TBL debugWinKH2CardGet = { "\x82j\x82g\x82Q\x83J\x81[\x83h\x93\xfc\x8e\xe8", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 13, 12, 0, 0, debugWinKH2CardGetLine, NULL, DebugAtelWorkDef };
static DEBUG_WINDOW_LINE debugGameInfomationLine[12] = {
    { "\x83v\x83\x8c\x83" "C\x83\x82\x81[\x83h         : %s", &DebugWork::g_dw.atelwork.iPlayMode, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, &szPlayMode[DebugWork::g_dw.atelwork.iPlayMode], 0, NULL, NULL },
    { "\x83R\x83x\x83" "c\x83Q\x81[\x83\x80\x83N\x83\x8a\x83" "A   : %s", &DebugWork::g_dw.atelwork.iDistinctionClear, 1, { 0 }, 0, 0.0f, 1.0f, 1.0f, (LPCSTR*)DWT::szOffOn, 0, debugGameInfomationParam, NULL },
    { "\x82j\x82g\x82Q\x83N\x83\x8a\x83" "A         : %s", &DebugWork::g_dw.atelwork.iGameKH2clear, 1, { 0 }, 0, 0.0f, 1.0f, 1.0f, (LPCSTR*)DWT::szOffOn, 0, debugGameInfomationParam, NULL },
    { "\x83\\\x83\x89\x83N\x83\x8a\x83" "A           : %s", &DebugWork::g_dw.atelwork.iSoraClear, 1, { 0 }, 0, 0.0f, 1.0f, 1.0f, (LPCSTR*)DWT::szOffOn, 0, debugGameInfomationParam, NULL },
    { "\x83\x8a\x83N\x83N\x83\x8a\x83" "A           : %s", &DebugWork::g_dw.atelwork.iRikuClear, 1, { 0 }, 0, 0.0f, 1.0f, 1.0f, (LPCSTR*)DWT::szOffOn, 0, debugGameInfomationParam, NULL },
    { "\x83\\\x83\x89" "EN\x83t\x83\x89\x83O         : %d", &DebugWork::g_dw.atelwork.iEnSoraEventStep, 4, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83\x8a\x83NEN\x83t\x83\x89\x83O         : %d", &DebugWork::g_dw.atelwork.iEnRikuEventStep, 4, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83}\x81[\x83\x8b\x81[\x83V\x83\x83KILL     : %s", &DebugWork::g_dw.atelwork.iMahluciaKilled, 1, { 0 }, 0, 0.0f, 1.0f, 1.0f, (LPCSTR*)DWT::szOffOn, 0, debugGameInfomationParam, NULL },
    { "\x83v\x83\x8c\x83" "C\x82\xc8\x82\xf1\x82\xa2\x82\xc7       : %s", &DebugWork::g_dw.atelwork.iPlayDifficultMode, 1, { 0 }, 0, 0.0f, 3.0f, 1.0f, &szDifficultMode[DebugWork::g_dw.atelwork.iPlayDifficultMode], 0, debugGameInfomationParam, NULL },
    { "\x83T\x83" "E\x83\x93\x83h\x83\x82\x81[\x83h       : %s", &DebugWork::g_dw.atelwork.iSoundMode, 1, { 0 }, 0, 0.0f, 3.0f, 1.0f, &szSoundMode[DebugWork::g_dw.atelwork.iSoundMode], 0, NULL, NULL },
    { "\x83o\x83" "C\x83u\x83\x8c\x81[\x83V\x83\x87\x83\x93     : %s", &DebugWork::g_dw.atelwork.iVibrat, 1, { 0 }, 0, 0.0f, 3.0f, 1.0f, (LPCSTR*)DWT::szOnOff, 0, NULL, NULL },
    { NULL, NULL, 0, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
};
static DEBUG_WINDOW_TBL debugGameInfomation = { "\x83Q\x81[\x83\x80\x8f\xee\x95\xf1", -106, -(SCREEN_HEIGHT >> 1) + 150, 20, 13, 12, 0, 0, debugGameInfomationLine, NULL, DebugAtelWorkDef };
static DEBUG_WINDOW_TBL debugWinTuEnd = { "\x83`\x83\x85\x81[\x83g\x83\x8a\x83" "A\x83\x8b\x8fI\x97\xb9", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 7, 12, 0, 0, debugWinTuEndLine, NULL, DebugAtelWorkDef };
static DEBUG_WINDOW_TBL debugWinMovieRiku = { "\x83\x80\x81[\x83r\x81[", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 20, 12, 0, 0, debugWinMovieLineRiku, NULL, DebugAtelWorkDef };
static DEBUG_WINDOW_TBL debugWinMovie = { "\x83\x80\x81[\x83r\x81[", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 20, 12, 0, 0, debugWinMovieLine, NULL, DebugAtelWorkDef };
static DEBUG_WINDOW_TBL debugWinPcSkill = { "\x83v\x83\x8c\x83" "C\x83\x84\x81[\x83X\x83L\x83\x8b", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 6, 12, 0, 0, debugWinPcSkillLine, NULL, DebugAtelWorkDef };
static DEBUG_WINDOW_LINE debugWinPoohLine[40] = {
    { "\xf9S----------\xf9V\x83Q\x81[\x83\x80\xf9S---------", &CDebugWindow::none, 1, { 0 }, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x82\xb6\x82\xe5\x82\xa4\x82\xbd\x82\xa2  :%s", &DebugWork::g_dw.atelwork.iPoohMinigameState, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, &szMiniGamePlayState[DebugWork::g_dw.atelwork.iPoohMinigameState], 0, NULL, NULL },
    { "\x82\xb5\x82\xe3\x82\xe9\x82\xa2    :%s", &DebugWork::g_dw.atelwork.iPoohMinigameKind, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, &szMiniGameKind[DebugWork::g_dw.atelwork.iPoohMinigameKind], 0, NULL, NULL },
    { "\xf9S----------\xf9V\x82\xc7\x82\xa4\x82\xc9\x82\xe3\x82\xa4\xf9S---------", &CDebugWindow::none, 1, { 0 }, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83s\x83O\x83\x8c\x83" "b\x83g  :%s", &DebugWork::g_dw.atelwork.iPoohMiniGameIntro, 4, { 0 }, 0, 0.0f, 0.0f, 0.0f, (LPCSTR*)DWT::szOffOn, 1, DebugSetPoohFlag, NULL },
    { "\x83I\x83" "E\x83\x8b      :%s", &DebugWork::g_dw.atelwork.iPoohMiniGameIntro, 4, { 0 }, 0, 0.0f, 0.0f, 0.0f, (LPCSTR*)DWT::szOffOn, 2, DebugSetPoohFlag, NULL },
    { "\x83\x8b\x81[        :%s", &DebugWork::g_dw.atelwork.iPoohMiniGameIntro, 4, { 0 }, 0, 0.0f, 0.0f, 0.0f, (LPCSTR*)DWT::szOffOn, 4, DebugSetPoohFlag, NULL },
    { "\x83" "C\x81[\x83\x88\x81[    :%s", &DebugWork::g_dw.atelwork.iPoohMiniGameIntro, 4, { 0 }, 0, 0.0f, 0.0f, 0.0f, (LPCSTR*)DWT::szOffOn, 8, DebugSetPoohFlag, NULL },
    { "\x83" "e\x83" "B\x83K\x81[    :%s", &DebugWork::g_dw.atelwork.iPoohMiniGameIntro, 4, { 0 }, 0, 0.0f, 0.0f, 0.0f, (LPCSTR*)DWT::szOffOn, 16, DebugSetPoohFlag, NULL },
    { "\x83\x89\x83r\x83" "b\x83g    :%s", &DebugWork::g_dw.atelwork.iPoohMiniGameIntro, 4, { 0 }, 0, 0.0f, 0.0f, 0.0f, (LPCSTR*)DWT::szOffOn, 32, DebugSetPoohFlag, NULL },
    { "\x83S\x81[\x83\x8b\x82P    :%s", &DebugWork::g_dw.atelwork.iPoohMiniGameIntro, 4, { 0 }, 0, 0.0f, 0.0f, 0.0f, (LPCSTR*)DWT::szOffOn, 64, DebugSetPoohFlag, NULL },
    { "\x83S\x81[\x83\x8b\x82Q    :%s", &DebugWork::g_dw.atelwork.iPoohMiniGameIntro, 4, { 0 }, 0, 0.0f, 0.0f, 0.0f, (LPCSTR*)DWT::szOffOn, 128, DebugSetPoohFlag, NULL },
    { "\xf9S----------\xf9V\x82\xb6\x82\xe5\x82\xa4\x82\xbd\x82\xa2\xf9S---------", &CDebugWindow::none, 1, { 0 }, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83s\x83O\x83\x8c\x83" "b\x83g  :%s", &DebugWork::g_dw.atelwork.iPoohGamePiglet, 1, { 0 }, 0, 0.0f, 3.0f, 1.0f, &szMiniGameState[DebugWork::g_dw.atelwork.iPoohGamePiglet], 0, DebugSetPoohFlag, NULL },
    { "\x83I\x83" "E\x83\x8b      :%s", &DebugWork::g_dw.atelwork.iPoohGameOwl, 1, { 0 }, 0, 0.0f, 3.0f, 1.0f, &szMiniGameState[DebugWork::g_dw.atelwork.iPoohGameOwl], 0, DebugSetPoohFlag, NULL },
    { "\x83\x8b\x81[        :%s", &DebugWork::g_dw.atelwork.iPoohGameRoo, 1, { 0 }, 0, 0.0f, 3.0f, 1.0f, &szMiniGameState[DebugWork::g_dw.atelwork.iPoohGameRoo], 0, DebugSetPoohFlag, NULL },
    { "\x83" "C\x81[\x83\x88\x81[    :%s", &DebugWork::g_dw.atelwork.iPoohGameEeyore, 1, { 0 }, 0, 0.0f, 3.0f, 1.0f, &szMiniGameState[DebugWork::g_dw.atelwork.iPoohGameEeyore], 0, DebugSetPoohFlag, NULL },
    { "\x83" "e\x83" "B\x83K\x81[    :%s", &DebugWork::g_dw.atelwork.iPoohGameTigger, 1, { 0 }, 0, 0.0f, 3.0f, 1.0f, &szMiniGameState[DebugWork::g_dw.atelwork.iPoohGameTigger], 0, DebugSetPoohFlag, NULL },
    { "\x83\x89\x83r\x83" "b\x83g    :%s", &DebugWork::g_dw.atelwork.iPoohGameRabbit, 1, { 0 }, 0, 0.0f, 3.0f, 1.0f, &szMiniGameState[DebugWork::g_dw.atelwork.iPoohGameRabbit], 0, DebugSetPoohFlag, NULL },
    { "\xf9S----------\xf9V\x83N\x83\x8a\x83" "A\x97p\xf9S---------", &CDebugWindow::none, 1, { 0 }, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83s\x83O\x83\x8c\x83" "b\x83g  :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroPiglet, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83I\x83" "E\x83\x8b      :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroOwl, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83\x8b\x81[        :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroRoo, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83" "C\x81[\x83\x88\x81[    :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroEeyore, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83" "e\x83" "B\x83K\x81[    :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroTigger, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83\x89\x83r\x83" "b\x83g    :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroRabbit, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\xf9S----------\xf9V\x8eg\x82\xa2\x8e\xcc\x82\xc4\x83N\x83\x8a\x83" "A\x97p\xf9S---------", &CDebugWindow::none, 1, { 0 }, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83s\x83O\x83\x8c\x83" "b\x83g  :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroResetPiglet, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83I\x83" "E\x83\x8b      :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroResetOwl, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83\x8b\x81[        :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroResetRoo, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83" "C\x81[\x83\x88\x81[    :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroResetEeyore, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83" "e\x83" "B\x83K\x81[    :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroResetTigger, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83\x89\x83r\x83" "b\x83g    :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroResetRabbit, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\xf9S----------\xf9V\x83n\x83" "C\x83X\x83R\x83" "A\xf9S---------", &CDebugWindow::none, 1, { 0 }, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83I\x83" "E\x83\x8b      :%d", &DebugWork::g_dw.atelwork.iPoohHiScoreOwl, 4, { 0 }, 0, 0.0f, 10000.0f, 100.0f, NULL, 0, DebugSetPoohFlag, NULL },
    { "\x83\x8b\x81[        :%d", &DebugWork::g_dw.atelwork.iPoohHiScoreRoo, 4, { 0 }, 0, 0.0f, 10000.0f, 100.0f, NULL, 0, DebugSetPoohFlag, NULL },
    { "\x83" "C\x81[\x83\x88\x81[    :%d", &DebugWork::g_dw.atelwork.iPoohHiScoreEeyore, 4, { 0 }, 0, 0.0f, 10000.0f, 100.0f, NULL, 0, DebugSetPoohFlag, NULL },
    { "\x83" "e\x83" "B\x83K\x81[    :%d", &DebugWork::g_dw.atelwork.iPoohHiScoreTigger, 4, { 0 }, 0, 0.0f, 10000.0f, 100.0f, NULL, 0, DebugSetPoohFlag, NULL },
    { "\x83\x89\x83r\x83" "b\x83g    :%d", &DebugWork::g_dw.atelwork.iPoohHiScoreRabbit, 4, { 0 }, 0, 0.0f, 10000.0f, 100.0f, NULL, 0, DebugSetPoohFlag, NULL },
    { NULL, NULL, 0, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
};
static DEBUG_WINDOW_TBL debugWinPooh = { "\x83v\x81[", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 25, 12, 0, 0, debugWinPoohLine, NULL, DebugAtelWorkDef };
static DEBUG_WINDOW_TBL debugWinFriendPossible = { "\x83t\x83\x8c\x83\x93\x83h\x82\xa9\x82\xa2\x82\xab\x82\xf1", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 29, 12, 0, 0, debugWinFriendPossibleLine, NULL, DebugAtelWorkDef };
static DEBUG_WINDOW_TBL debugWinCardPossible = { "\x83J\x81[\x83h\x82\xa9\x82\xa2\x82\xab\x82\xf1", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 27, 12, 0, 0, debugWinCardPossibleLine, NULL, DebugAtelWorkDef };
static DEBUG_WINDOW_TBL debugWinWorldIntro = { "\x83\x8f\x81[\x83\x8b\x83h\x83^\x83" "C\x83g\x83\x8b", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 29, 12, 0, 0, debugWinWorldIntroLine, NULL, DebugAtelWorkDef };
static DEBUG_WINDOW_LINE debugWinAtelWorkDefLine[30] = {
    { "\xf9S----------\xf9VWLV_STORAGE\xf9S---------", &CDebugWindow::none, 1, { 0 }, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83\x8f\x81[\x83\x8b\x83h\x83^\x83" "C\x83g\x83\x8b...", &CDebugWindow::none, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinWorldIntro },
    { "\x83`\x83\x85\x81[\x83g\x83\x8a\x83" "A\x83\x8b...", &CDebugWindow::none, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinTuEnd },
    { "\x83J\x81[\x83h\x89\xf0\x8b\xd6...", &CDebugWindow::none, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinCardPossible },
    { "\x83t\x83\x8c\x83\x93\x83h\x89\xf0\x8b\xd6...", &CDebugWindow::none, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinFriendPossible },
    { "\x82P\x82O\x82O\x83G\x81[\x83J\x81[...", &CDebugWindow::none, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinPooh },
    { "\x95W\x8f\x80\x83X\x83L\x83\x8b...", &CDebugWindow::none, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinPcSkill },
    { "\x83\x80\x81[\x83r\x81[\x83\\\x83\x89...", &CDebugWindow::none, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinMovie },
    { "\x83\x80\x81[\x83r\x81[\x83\x8a\x83N...", &CDebugWindow::none, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinMovieRiku },
    { "\x82j\x82g\x82Q\x83J\x81[\x83h\x93\xfc\x8e\xe8...", &CDebugWindow::none, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinKH2CardGet },
    { "\x83W\x83~\x83j\x81[\x83\x81\x83\x82...", &CDebugWindow::none, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinJiminy },
    { "\x83Q\x81[\x83\x80\x8f\xee\x95\xf1...", &CDebugWindow::none, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugGameInfomation },
    { "\x95\xf3\x94\xa0...", &CDebugWindow::none, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugKeyTresur },
    { "\x83o\x83g\x83\x8b\x83\x8c\x83|\x81[\x83g...", &CDebugWindow::none, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugBtlReport },
    { "\xf9S----------\xf9VWLV_GLOBAL\xf9S---------", &CDebugWindow::none, 1, { 0 }, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "BOSS\x83" "C\x83x\x83\x93\x83g\x83X\x83" "e\x83" "b\x83v : %d", &DebugWork::g_dw.atelwork.iBossEventStep, 4, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
    { "\x83\x81\x83j\x83\x85\x81[\x82\xcc\x8e\xed\x97\xde       : %s", &DebugWork::g_dw.atelwork.iKindMenu, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, &szMenuKind[DebugWork::g_dw.atelwork.iKindMenu], 0, NULL, NULL },
    { "\x83v\x83\x8c\x83" "C\x83\x84\x81[\x94\xf1\x95\\\x8e\xa6     : %s", &DebugWork::g_dw.atelwork.iPlayerDispOff, 1, { 0 }, 0, 0.0f, 1.0f, 1.0f, (LPCSTR*)DWT::szOffOn, 0, DebugSetGameWorkParam, NULL },
    { "\x82" "a\x82n\x82r\x82r\x8e\x80\x96S         : %s", &DebugWork::g_dw.atelwork.iBossKilled, 1, { 0 }, 0, 0.0f, 1.0f, 1.0f, (LPCSTR*)DWT::szOffOn, 0, DebugSetGameWorkParam, NULL },
    { "\x83" "C\x83x\x83\x93\x83g\x83X\x83L\x83" "b\x83v\x95s\x89\xc2 : %s", &DebugWork::g_dw.atelwork.iNotEventSkip, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, (LPCSTR*)DWT::szOffOn, 0, NULL, NULL },
    { "ATEL\x83" "C\x83x\x83\x93\x83g\x92\x86       : %s", &DebugWork::g_dw.atelwork.iIsAtelEvent, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, (LPCSTR*)DWT::szOffOn, 0, NULL, NULL },
    { "PC\x91\x80\x8d\xec\x95s\x89\xc2           : %s", &DebugWork::g_dw.atelwork.iNotPcControl, 1, { 0 }, 0, 0.0f, 1.0f, 1.0f, (LPCSTR*)DWT::szOffOn, 0, DebugSetGameWorkParam, NULL },
    { "PAUSE\x8b\xd6\x8e~            : %s", &DebugWork::g_dw.atelwork.iNotPause, 1, { 0 }, 0, 0.0f, 1.0f, 1.0f, (LPCSTR*)DWT::szOffOn, 0, DebugSetGameWorkParam, NULL },
    { "\x83`\x83\x85\x81[\x83g\x83\x8a\x83" "A\x83\x8b\x92\x86     : %s", &DebugWork::g_dw.atelwork.iIsTutorial, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, (LPCSTR*)DWT::szOffOn, 0, NULL, NULL },
    { "\x83\x8b\x81[\x83\x80\x88\xda\x93\xae\x8b\xd6\x8e~       : %s", &DebugWork::g_dw.atelwork.iNotRoomJump, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, (LPCSTR*)DWT::szOffOn, 0, NULL, NULL },
    { "\x83\x8b\x81[\x83\x80" "Creation\x8b\xd6\x8e~   : %s", &DebugWork::g_dw.atelwork.iNotRoomCreation, 1, { 0 }, 0, 0.0f, 0.0f, 0.0f, (LPCSTR*)DWT::szOffOn, 0, NULL, NULL },
    { "\x83n\x81[\x83g\x8fo\x82\xb3\x82\xc8\x82\xa2       : %s", &DebugWork::g_dw.atelwork.iNotHartBtl, 1, { 0 }, 0, 0.0f, 1.0f, 1.0f, (LPCSTR*)DWT::szOffOn, 0, DebugSetGameWorkParam, NULL },
    { "BTL\x83`\x83\x85\x81[\x83g\x83\x8a\x83" "A\x83\x8b\x92\x86  : %s", &DebugWork::g_dw.atelwork.iIsBtlTutorial, 1, { 0 }, 0, 0.0f, 1.0f, 1.0f, (LPCSTR*)DWT::szOffOn, 0, DebugSetGameWorkParam, NULL },
    { "\x83\x8f\x81[\x83\x8b\x83h\x83^\x83" "C\x83g\x83\x8b\x92\x86   : %s", &DebugWork::g_dw.atelwork.iIsWorldIntro, 1, { 0 }, 0, 0.0f, 1.0f, 1.0f, (LPCSTR*)DWT::szOffOn, 0, DebugSetGameWorkParam, NULL },
    { NULL, NULL, 0, { 0 }, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL },
};
static DEBUG_WINDOW_TBL debugWinAtelWorkDef = { "ATEL WORK", -106, -(SCREEN_HEIGHT >> 1) + 150, 20, 30, 12, 0, 0, debugWinAtelWorkDefLine, NULL, DebugAtelWorkDef };
static DEBUG_WINDOW_TBL debugWinEventPlayer = { "ATEL\x81@CONFIG", -206, -(SCREEN_HEIGHT >> 1) + 50, 12, 16, 14, 0, 0, debugWinEventSelectPlayerLine, NULL, debugWinNPCMainlCall };
}

// SOURCE DATA: .rodata .bss
