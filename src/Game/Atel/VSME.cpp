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
#include "System/TaskMan.h"
#include "Form/DebugWindow.h"
#include "Game/Debug/DebugWork.h"
#include "Game/Player/Player.h"
#include "System/Sound/SoundMan.h"
#include "System/Com.h"
#include "Game/SceneEnv.h"
#include "Form/Menu/JMemoFlag.h"
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", DebugAtelWorkDef__8DWTEventP12CDebugWindowi);
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
/* placeholder: rodata of DebugSetSoraMovieFlag__8DWTEventP12CDebugWindowii */
__asm__(".rdata\n");
__asm__(".rdata\n.align 4\n.word .L00242088\n.word .L00242090\n.word .L00242098\n.word .L002420A0\n.word .L002420A8\n.word .L002420B0\n.word .L002420B8\n.word .L002420C0\n.word .L002420C8\n.word .L002420D0\n.word .L002420D8\n.word .L002420E0\n.word .L002420E8\n.word .L002420F0\n.word .L002420F8\n.word .L00242100\n.word .L00242108\n.word .L00242114\n.word .L00242120\n.word .L0024212C\n.word .L00242138\n.word .L00242144\n.word .L00242150\n.word .L0024215C\n.word .L00242168\n.word .L00242174\n.word .L00242180\n.word .L0024218C\n.word .L00242198\n.word .L002421A4\n.word .L002421B0\n.word .L002421BC\n.word .L002421C8\n.word .L002421D0\n.word .L002421D8\n.word .L002421E0\n.word .L002421E8\n.word .L002421F0\n.word .L002421F8\n.word .L00242200\n.word .L00242208\n.word .L00242210\n.word .L00242218\n.word .L00242220\n.word .L00242228\n.word .L00242230\n.word .L00242238\n.word .L00242240\n.word .L00242248\n.word .L00242250\n.word .L00242258\n.word .L00242260\n.word .L00242268\n.word .L00242270\n.word .L00242278\n.word .L00242280\n.word .L00242288\n.word .L00242290\n.word .L00242298\n.word func_0\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", DebugSetSoraMovieFlag__8DWTEventP12CDebugWindowii);
/* placeholder: rodata of DebugSetRikuMovieFlag__8DWTEventP12CDebugWindowii */
__asm__(".rdata\n");
__asm__(".rdata\n.align 4\n.word .L00242380\n.word .L00242388\n.word .L00242390\n.word .L00242398\n.word .L002423A0\n.word .L002423A8\n.word .L002423B0\n.word .L002423B8\n.word .L002423C0\n.word .L002423C8\n.word .L002423D0\n.word .L002423D8\n.word .L002423E0\n.word .L002423E8\n.word .L002423F0\n.word .L002423F8\n.word .L00242400\n.word .L0024240C\n.word .L00242418\n.word .L00242424\n.word .L00242430\n.word .L0024243C\n.word .L00242448\n.word .L00242454\n.word .L00242460\n.word .L0024246C\n.word .L00242478\n.word .L00242484\n.word .L00242490\n.word .L0024249C\n.word .L002424A8\n.word .L002424B4\n.word .L002424C0\n.word .L002424C8\n.word .L002424D0\n.word .L002424D8\n.word .L002424E0\n.word .L002424E8\n.word .L002424F0\n.word .L002424F8\n.word .L00242500\n.word func_0\n.word func_0\n.word func_0\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", DebugSetRikuMovieFlag__8DWTEventP12CDebugWindowii);
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
/* placeholder: rodata of DebugSetJiminyMemoFlag__8DWTEventP12CDebugWindowii */
__asm__(".rdata\n");
__asm__(".rdata\n.align 4\n.word .L002426C0\n.word .L002426C0\n.word .L002426C0\n.word .L002426C0\n.word .L002426C0\n.word .L002426C0\n.word .L002426C0\n.word .L002426C0\n.word .L002426C0\n.word .L002426C0\n.word .L002426C0\n.word .L002426C0\n.word .L002426C0\n.word .L002426E0\n.word .L002426E0\n.word .L002426E0\n.word .L002426E0\n.word .L002426E0\n.word .L002426E0\n.word .L002426E0\n.word .L002426E0\n.word .L002426E0\n.word .L002426E0\n.word .L002426E0\n.word .L002426E0\n.word .L002426E0\n.word .L002428AC\n.word .L00242704\n.word .L00242704\n.word .L00242704\n.word .L00242704\n.word .L00242704\n.word .L00242704\n.word .L00242704\n.word .L00242704\n.word .L00242704\n.word .L00242704\n.word .L00242704\n.word .L00242704\n.word .L00242728\n.word .L00242728\n.word .L00242728\n.word .L00242728\n.word .L00242728\n.word .L00242728\n.word .L00242728\n.word .L00242728\n.word .L00242728\n.word .L00242728\n.word .L00242728\n.word .L00242728\n.word .L002428AC\n.word .L0024274C\n.word .L00242764\n.word .L0024277C\n.word .L00242794\n.word .L002427AC\n.word .L002427C4\n.word .L002427DC\n.word .L002427F4\n.word .L0024280C\n.word .L00242824\n.word .L0024283C\n.word .L00242854\n.word .L0024286C\n.word .L00242890\n.word func_0\n.word func_0\n.text\n");
__asm__(".rdata\n.align 4\n.word .L0024296C\n.word .L0024296C\n.word .L0024296C\n.word .L002429E8\n.word .L00242984\n.word .L00242984\n.word .L00242984\n.word .L002429E8\n.word .L002429A8\n.word .L002429A8\n.word .L002429E8\n.word .L002429CC\n.word .L002429CC\n.word func_0\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", DebugSetJiminyMemoFlag__8DWTEventP12CDebugWindowii);
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
/* placeholder: rodata of _$_Q24ATEL4CVSM */
__asm__(".rdata\n");
inline void _roPh19() {
    TRACE("CVSM::~CVSM()\n");
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", _$_Q24ATEL4CVSM);
/* placeholder: rodata of initActor__Q24ATEL4CVSM */
__asm__(".rdata\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatNop__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatExec__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatRWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatCWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatIWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatPWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatSWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatMWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatCPWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatSNWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatMSWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatShopWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatDropWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatEnemyLoadWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatRCWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatEQWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatWKWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatXWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatBSWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatBAWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatDLWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatBGWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatActorWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatEternityWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatRsrcLoadWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatCardWndWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatTechWndWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatWorldCardWndWait__Q24ATEL4CVSM\n.text\n");
__asm__(".rdata\n.align 3\n.half 0\n.half -1\n.word acStatCrossFadeWait__Q24ATEL4CVSM\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", initActor__Q24ATEL4CVSM);
/* placeholder: rodata of loadFromMem__Q24ATEL4CVSMPCUc */
__asm__(".rdata\n");
inline void _roPh21() {
    TRACE("Load I-Seg\n");
    TRACE("ATEL \x83R\x83\x93\x83p\x83" "C\x83\x89\x82\xcc\x83o\x81[\x83W\x83\x87\x83\x93\x82\xaa\x88\xe1\x82\xa2\x82\xdc\x82\xb7\n");
    TRACE("Actor-ID \x93o\x98^\x90\x94\x83I\x81[\x83o\x81[\x83t\x83\x8d\x81[\n");
    TRACE(__FILE__);
    TRACE("MaxPC:%x\n");
    TRACE(__FILE__ "(357)");
    TRACE("m_pIseg");
    TRACE("GAptr:%x\n");
    TRACE("\x83" "f\x81[\x83^\x83Z\x83O\x83\x81\x83\x93\x83g \x83I\x81[\x83o\x81[\x83t\x83\x8d\x81[\n");
    TRACE(__FILE__ "(376)");
    TRACE("m_pDseg");
    TRACE("Actor \x93o\x98^\x90\x94\x83I\x81[\x83o\x81[\x83t\x83\x8d\x81[\n");
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", loadFromMem__Q24ATEL4CVSMPCUc);
void CVSM::deleteActorAll() {}
/* placeholder: rodata of checkEvents__Q24ATEL4CVSM */
__asm__(".rdata\n");
inline void _roPh22() {
    TRACE("@_to");
    TRACE("@_tf");
    TRACE("@_as");
    TRACE("@_tc");
    TRACE("@_tk");
    TRACE("\x89\xef\x98" "b\x8aJ\x8en\x81" "F%s\n");
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", checkEvents__Q24ATEL4CVSM);
/* Parked: 8 bytes short; only the first return differs: the original keeps `if (!g_pPlayer) return -1;` as
   bnez over "b end; li -1", ours lets jump2 cross-jump it into the loop's final `li -1` (beqz v1 to it; -dJ dump:
   all four -1 returns share one label). $LM tags: 642 the test, 643 the return, 645-647 isNowLockon (Player.h)
   and the 0/-1 test, 650 getTask, 651/653 the early returns, 655 the cast, 658-662 the loop, 665 return -1.
   tried: wrapping the body in if (g_pPlayer) {}, getLockonHandle() vs isNowLockon() + m_hTarget, a break loop
   with a separate found test (longer), separate-line returns. */
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
/* placeholder: rodata of execActor__Q24ATEL4CVSM */
__asm__(".rdata\n");
inline void _roPh25() {
    TRACE("(ATEL_REQ <= nState) && (_ATEL_REQ_MAX > nState)");
    TRACE("mada->ok pc:%d\n");
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", execActor__Q24ATEL4CVSM);
/* placeholder: rodata of execCode__Q24ATEL4CVSMi */
__asm__(".rdata\n");
inline void _roPh26() {
    TRACE("Illegal instr. address : %d\n");
    TRACE("Illegal Stack pointer %d\n");
    TRACE("Illegal Op. code at loc. %d\n");
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", execCode__Q24ATEL4CVSMi);
/* placeholder: rodata of execStdCode__Q24ATEL4CVSMiRiRQ24ATEL13eATEL_REQ_RET */
__asm__(".rdata\n");
inline void _roPh27() {
    TRACE("zero divide\n");
    TRACE("Run-time stack  overflow Freg:%x addr:%x pc:%d\n");
    TRACE("Run-time stack underflow\n");
    TRACE("REQ:%s,%s\n");
    TRACE(__FILE__ "(1121)");
    TRACE("(m_S[m_SP].I)");
    TRACE("%s");
}
__asm__(".rdata\n.align 4\n.word .L00244ED0\n.word .L00244334\n.word .L00244394\n.word .L002443DC\n.word .L00244424\n.word .L00244454\n.word .L00244484\n.word .L002444A8\n.word .L002444D8\n.word .L00244508\n.word .L0024452C\n.word .L0024455C\n.word .L0024458C\n.word .L002445B0\n.word .L002445F0\n.word .L00244634\n.word .L00244664\n.word .L00244EC0\n.word .L002446BC\n.word .L002446EC\n.word .L0024470C\n.word .L0024472C\n.word .L00244768\n.word .L002447B8\n.word .L002447E8\n.word .L00244814\n.word .L00244840\n.word .L0024486C\n.word .L00244898\n.word .L002448B8\n.word .L002448E0\n.word .L00244908\n.word .L00244EC8\n.word .L00244930\n.word .L00244954\n.word .L00244978\n.word .L0024499C\n.word .L002449A8\n.word .L002449E8\n.word .L00244A34\n.word .L00244A54\n.word .L00244B48\n.word .L00244A78\n.word .L00244A9C\n.word .L00244AC0\n.word .L00244AE4\n.word .L00244B08\n.word .L00244B2C\n.word .L00244B54\n.word .L00244B88\n.word .L00244BA8\n.word .L00244D28\n.word .L00244D90\n.word .L00244E08\n.word .L00244EC0\n.word .L00244E34\n.word .L00244E7C\n.word .L00244BB0\n.word .L00244BCC\n.word .L00244C0C\n.word .L00244C44\n.word .L00244CC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244EC0\n.word .L00244BBC\n.word func_0\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", execStdCode__Q24ATEL4CVSMiRiRQ24ATEL13eATEL_REQ_RET);
/* placeholder: rodata of execExt1Code__Q24ATEL4CVSMiRQ24ATEL13eATEL_REQ_RET */
__asm__(".rdata\n");
inline void _roPh28() {
    TRACE("--- ACTCMD_SET_FACE --- %d\n");
    TRACE("ACC c:%d t:%d id:%d\n");
    TRACE("Avatar init ACI %.1f %.1f %.1f \n");
    TRACE("ACP %.1f %.1f %.1f \n");
    TRACE("ATEL:\x83\x8a\x83\\\x81[\x83X\x83\x8d\x81[\x83h: %d, %d\n");
    TRACE("--- START MOVE-ACTOR Type::%x ---\n");
    TRACE("--- ACTOR-SET-TALKRAD ---\n");
    TRACE("--- START ROATE-ACTOR-Target ---TYPE[%d]\n");
    TRACE("chibi-pos %d : %d\n");
    TRACE("MESS_CMD OPEN \n");
    TRACE("MESS_CMD CLOSE \n");
    TRACE("MESS_CMD DESTROY \n");
    TRACE("MESS_CMD CLR TEXT \n");
    TRACE("MESS_CMD TEXT DELEY \n");
    TRACE("MESS_CMD WND STYLE \n");
    TRACE("MESS_CMD CHG WND STYLE\n");
    TRACE("MESS_CMD SET TEXT MARGIN\n");
    TRACE("MESS_CMD SET FONT SIZE\n");
    TRACE("MESS_CMD SET FONT SPACE\n");
    TRACE("MESS_CMD SET TEXT STYLE\n");
    TRACE("MESS_CMD SET TEXT COLOR\n");
    TRACE("MESS_CMD SET TEXT EXCOLOR\n");
    TRACE("MESS_CMD SETSELECTOFS\n");
    TRACE("MESS_CMD CHGCURSORCOLOR\n");
    TRACE("MESS_CMD SETDISPINTERVAL\n");
    TRACE("MESS_CMD CHANGETEXT_NEXT\n");
    TRACE("MESS_CMD CHAT_ENABLEDISP\n");
    TRACE("MESS_CMD IMAGE_PHASEINI\n");
    TRACE("MESS_CMD IMAGE_PHASEINI2\n");
    TRACE("MES_DEST_ALL\n");
    TRACE("MES_LAYER_CHANGE\n");
    TRACE("MES_SET_C_FADEINDELAY\n");
    TRACE("MES_SET_T_FADEOUTTIME\n");
    TRACE("MESS_CMD bad param \n");
}
__asm__(".rdata\n.align 4\n.word .L00244F74\n.word .L00244FD0\n.word .L00245004\n.word .L002467E8\n.word .L002467E0\n.word .L0024502C\n.word .L002467E0\n.word .L002450A4\n.word .L002451D8\n.word .L00245290\n.word .L00245290\n.word .L002452FC\n.word .L00245404\n.word .L0024543C\n.word .L00245498\n.word .L00245550\n.word .L0024562C\n.word .L00245668\n.word .L002457B0\n.word .L002457E0\n.word .L00245878\n.word .L002458FC\n.word .L00245920\n.word .L00245930\n.word .L00245958\n.word .L00245968\n.word .L00245978\n.word .L002459AC\n.word .L00245A10\n.word .L00245A40\n.word .L00245ECC\n.word .L00245ECC\n.word .L002467E8\n.word .L002466F8\n.word .L0024671C\n.word .L00246750\n.word .L00246778\n.word .L002467AC\n.word func_0\n.word func_0\n.text\n");
__asm__(".rdata\n.align 4\n.word .L00245F3C\n.word .L00245F90\n.word .L00245FE4\n.word .L00246000\n.word .L0024604C\n.word .L00246098\n.word .L002460E4\n.word .L00246130\n.word .L00246188\n.word .L002461D4\n.word .L0024622C\n.word .L00246278\n.word .L002462C4\n.word .L00246310\n.word .L0024635C\n.word .L002463A8\n.word .L002463F4\n.word .L00246440\n.word .L0024648C\n.word .L002466E4\n.word .L00246514\n.word .L0024659C\n.word .L002465E8\n.word .L00246634\n.word .L0024667C\n.word func_0\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", execExt1Code__Q24ATEL4CVSMiRQ24ATEL13eATEL_REQ_RET);
/* placeholder: rodata of execExt2Code__Q24ATEL4CVSMiRQ24ATEL13eATEL_REQ_RET */
__asm__(".rdata\n");
inline void _roPh29() {
    TRACE("CCC %d %f %f %f\n");
    TRACE("CCT %d %f %f %f\n");
    TRACE("    %f %f %f %d %f\n");
    TRACE("BGM_PLAY %s %d\n");
    TRACE("MOE %d : %d : %f : %d\n");
    TRACE("CAMCMD_CHANGE_GAKAKU %f\n");
    TRACE("--- WNDCMD_FADE_CREATE ---\n");
    TRACE("--- WNDCMD_FADE_DESTROY ---\n");
    TRACE("BGM_FADE_IN %s %d %d\n");
    TRACE("BGM_FADE_OUT %d \n");
    TRACE("BGM_CROSS_FADE %s %d %d\n");
    TRACE("\x82" "a\x82" "f\x82l\x8d\xc4\x90\xb6\x82\xb5\x82\xc4\x82\xa2\x82\xc8\x82\xa2\x82\xcc\x82\xc9\x83N\x83\x8d\x83X\x83t\x83" "F\x81[\x83h\x97v\x8b\x81\x81" "F\x83N\x83\x8d\x83X\x83t\x83" "F\x81[\x83h\x83L\x83\x83\x83\x93\x83Z\x83\x8b\x81" "FBGM\x8d\xc4\x90\xb6\n");
    TRACE("NG AWK param\n");
    TRACE("CAMCMD_CREATE_PATHDATA %s\n");
    TRACE("CAMCMD_ADD_QUAKE \n");
}
__asm__(".rdata\n.align 4\n.word .L002468B0\n.word .L002468D4\n.word .L00246914\n.word .L00246954\n.word .L00246994\n.word .L002482EC\n.word .L002482EC\n.word .L002482EC\n.word .L002482EC\n.word .L002482EC\n.word .L002482EC\n.word .L002482EC\n.word .L002482EC\n.word .L002482EC\n.word .L002482EC\n.word .L002469D0\n.word .L00246A10\n.word .L00246C1C\n.word .L00246C60\n.word .L00246D3C\n.word .L002482EC\n.word .L00246E90\n.word .L00246EA4\n.word .L00246ED4\n.word .L002482F4\n.word .L00247178\n.word .L002471FC\n.word .L00247238\n.word .L002482F4\n.word .L002482F4\n.word .L00247294\n.word .L002472C8\n.word .L00247308\n.word .L002473B8\n.word .L002482EC\n.word .L002473EC\n.word .L00247420\n.word .L002482EC\n.word .L002474A8\n.word .L002474C8\n.word .L00247524\n.word .L00247594\n.word .L002475FC\n.word .L00247678\n.word .L002476C0\n.word .L0024773C\n.word .L0024773C\n.word .L0024773C\n.word .L00247784\n.word .L002477A0\n.word .L00247818\n.word .L0024785C\n.word .L002482F4\n.word .L002482F4\n.word .L002482F4\n.word .L002482F4\n.word .L002482F4\n.word .L00247908\n.word .L002482EC\n.word .L00247980\n.word .L002482F4\n.word .L0024799C\n.word .L002479F8\n.word .L00247AE0\n.word .L002482F4\n.word .L002482F4\n.word .L00246ED4\n.word .L00247B2C\n.word .L00247CA4\n.word .L00247F88\n.word .L00248168\n.word .L002481C8\n.word .L002482F4\n.word func_0\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", execExt2Code__Q24ATEL4CVSMiRQ24ATEL13eATEL_REQ_RET);
/* placeholder: rodata of execExt3Code__Q24ATEL4CVSMiRQ24ATEL13eATEL_REQ_RET */
__asm__(".rdata\n");
inline void _roPh30() {
    TRACE("%d\n");
    TRACE("%f\n");
    TRACE("\x83X\x83g\x83" "b\x83N\x8bZ\x8fK\x93\xbe:%d\n");
    TRACE(__FILE__ "(2623)");
    TRACE(__FILE__ "(2672)");
    TRACE("Speed %f\n");
    TRACE("CARD WINDW CLOSE WAIT START\n");
    TRACE("reqEnterDorr:ATEL sys_ctrl\n");
    TRACE(__FILE__ "(2713)");
    TRACE(__FILE__ "(2716)");
    TRACE(__FILE__ "(2719)");
    TRACE("ATEL:cancelLoadRCF:%d,%d\n");
    TRACE("ATEL:OPMEN DOOR: %d, %d\n");
    TRACE("MIN:%d, MAX:%d,A:%d\n");
    TRACE("\x83\x89\x83X\x83g\x83t\x83\x8d\x83" "A\x82\xd6\x82\xcc\x94\xe0\x95\\\x8e\xa6\n");
    TRACE("ATEL:\x83\x8f\x81[\x83\x8b\x83h\x83Z\x83\x8c\x83N\x83g\x8c\xc4\x82\xd1\x8fo\x82\xb5\n");
    TRACE("ATEL:UNAVAILABLE DOOR: %d, %d\n");
    TRACE("ATEL:\x83S\x81[\x83\x8b\x95\x94\x89\xae\x8aJ\x82\xa2\x82\xbd\n");
    TRACE("\x83\x8c\x83x\x83\x8b\x83" "A\x83" "b\x83v\x81H %d\n");
    TRACE(__FILE__ "(3000)");
    TRACE("TECH WINDW CLOSE WAIT START\n");
    TRACE("ATEL:\x83\x8f\x81[\x83\x8b\x83h\x83N\x83\x8a\x83" "A\x81" "F%d\n");
    TRACE("ATEL:\x83t\x83\x8d\x83" "A\x83N\x83\x8a\x83" "A\n");
    TRACE("ATEL:\x83\x82\x83\x93\x83X\x83g\x83\x8d\x83U\x83R\x83o\x83g\x83\x8b\x83N\x83\x8a\x83" "A\n");
    TRACE("\x83G\x83N\x83X\x83g\x83\x8a\x81[\x83\x80\x81" "F\x83X\x83g\x83" "b\x83N\x91S\x95\x94\x8ao\x82\xa6\x82\xe9\x81" "A\x83J\x81[\x83h\x91S\x95\x94\x89\xf0\x8b\xd6\x81" "A\x95W\x8f\x80\x83X\x83L\x83\x8b\x91S\x8eg\x97p\x89\xc2\n");
    TRACE("ATEL:CGlare::fadein\n");
    TRACE("ATEL:CGlare::fadeout\n");
    TRACE("ATEL:\x82g\x82o\x89\xf1\x95\x9c\n");
    TRACE("\x83v\x83\x8c\x83" "C\x83\x84\x81[\x82\xa2\x82\xc8\x82\xa2\n");
    TRACE("ATEL:\x83o\x83" "b\x83N\x83" "A\x83" "b\x83v\n");
    TRACE("ATEL:CLOSE DOOR: %d, %d\n");
    TRACE("GPS NG01:%s\n");
    TRACE("NAME TO SHOW :%s\n");
    TRACE("NAME TO HIDE :%s\n");
    TRACE("GRAVITY ON :%s\n");
    TRACE("GRAVITY OFF :%s\n");
    TRACE("MAP_COLL_ON :%s\n");
    TRACE("MAP_COLL_OFF :%s\n");
    TRACE("BODY_COLL_ON :%s\n");
    TRACE("BODY_COLL_OFF :%s\n");
    TRACE("REMOVE NPC :%s\n");
    TRACE("MAKEUP_MOTION_INVALID :%s\n");
    TRACE("MAKEUP_MOTION_VALID :%s\n");
    TRACE("AVTCMD_SET_LOOK_START :%s\n");
    TRACE("AVTCMD_SET_LOOK_STOP :%s\n");
    TRACE("AVTCMD_SET_LOOK_END :%s\n");
    TRACE("AVTCMD_CREATE_LEGIK :%s\n");
    TRACE("AVTCMD_SET_IK  ON :%s\n");
    TRACE("AVTCMD_SET_IK  OFF :%s\n");
    TRACE("AVTCMD_SET_LOCK_ON  ON :%s\n");
    TRACE("AVTCMD_SET_LOCK_ON  OFF :%s\n");
    TRACE("AVTCMD_SET_NEAR_CLIP  OFF :%s\n");
    TRACE("CHT\n");
    TRACE("MTO NG01:%s\n");
    TRACE("AED %d : %d : %f : %d : %d\n");
    TRACE("FOG %d : R:%d : G:%d : B:%d : %d\n");
    TRACE("FOG %d : %d : %d\n");
    TRACE("SVR:%d,%d\n");
    TRACE("CER:%d,%d,%d\n");
    TRACE("EBT\n");
    TRACE("scene_set_color:type:%d,frame:%d,color:%d\n");
    TRACE("CWorldEnv::getActiveCamera()");
    TRACE("EffectCtrl  %d, %d, %d\n");
    TRACE("BLUR %d : :%d : %d\n");
    TRACE("MEX:%d,%d,%d,%f,%d,%d,%d,%d\n");
    TRACE("CRC %d %d\n");
    TRACE("DRC %d %d\n");
    TRACE("LOK:%s,%s,%f,%f,%f,%f,%f,%f,%f,%f\n");
    TRACE("UDL:%s,%f,%f\n");
    TRACE("LAG:%s,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n");
    TRACE("UDA:%f,%f,%f,%f\n");
    TRACE("CTX:%s,%s\n");
    TRACE("ELK:AVTCMD_SET_LOOK_END\n");
    TRACE("STA\x81" "F%d, %s\n");
    TRACE("STX\x81" "F%d, %s\n");
    TRACE("SUP:%d,%f,%f,%f\n");
    TRACE("ATEL:\x83G\x83\x89\x81[\x81" "FsetUnknownPos\x81@\x91\xb6\x8d\xdd\x82\xb5\x82\xc8\x82\xa2\x83p\x83\x89\x83\x81\x81[\x83^\n");
    TRACE("SUR:%d,%f,%f,%f\n");
    TRACE("ATEL:\x83G\x83\x89\x81[\x81" "FsetUnknownRot\x81@\x91\xb6\x8d\xdd\x82\xb5\x82\xc8\x82\xa2\x83p\x83\x89\x83\x81\x81[\x83^\n");
    TRACE("enableNotSemiModel: %d,%d");
    TRACE("ATEL:playMovie %s, %d\n");
    TRACE("ATEL:CrossFade %d, %d\n");
    TRACE("\x83N\x83\x8d\x83X\x83t\x83" "F\x81[\x83h\x97p\x83" "C\x83\x81\x81[\x83W\x82r\x82s\x82n\x82q\x82" "d\n");
    TRACE("\x83N\x83\x8d\x83X\x83t\x83" "F\x81[\x83h\x97p\x83" "C\x83\x81\x81[\x83W\x82" "c\x82q\x82`\x82v\n");
    TRACE("CROSS_FADE : R:%d : G:%d : B:%d : A:%d\n");
    TRACE("CROSS_FADE : R:%d : G:%d : B:%d : A:%d : Frame:%d\n");
    TRACE("ChangeRoom:room:%d,door:%d,scene:%d\n");
    TRACE("\x95\x94\x89\xae\x82\xcc\x83V\x81[\x83\x93\x82\xaa\x82\xc8\x82\xa2\x82\xa9\x82\xe7\x82\xc6\x82\xe8\x82\xa0\x82\xa6\x82\xb8\x93K\x93\x96\x82\xc9\x8ew\x92\xe8\x82\xb5\x82\xc4\x94\xf2\x82\xce\x82\xb7\n");
    TRACE("EnterBattle:%d\n");
    TRACE("deFocus_man %f %f\n");
    TRACE("--- SCALING - player ---\n");
    TRACE("--- WNDCMD_FADER ---\n");
    TRACE("MTO NG01:%d\n");
    TRACE("CAR %d %f %f %f\n");
    TRACE("    %d %f\n");
    TRACE("REL %d %f %f %f\n");
    TRACE("CLK %d %f %f %f\n");
    TRACE("wk_CMD %x : %x \n");
    TRACE("--- START MOVE-ACTOR-PATH Type::%x ---\n");
    TRACE("voice_play %s %d %d\n");
    TRACE("voice_fadeout  %d %d\n");
    TRACE("defoucus %d : %f : %f \n");
    TRACE("VIB \n");
    TRACE("voice_play_id:playVsb %d %d %d\n");
    TRACE("BlurDistanceSet %f\n");
    TRACE("%s,%s\n");
    TRACE("--- ATTACH - %s(%d)\n");
    TRACE("BlurSetEvent %f \n");
    TRACE("face set NG01:%s\n");
}
__asm__(".rdata\n.align 4\n.word .L0024D734\n.word .L002483E8\n.word .L0024D734\n.word .L00248740\n.word .L002487E4\n.word .L002497BC\n.word .L00249814\n.word .L0024D734\n.word .L0024C2B0\n.word .L0024D734\n.word .L0024C330\n.word .L0024D6E4\n.word .L0024D6E4\n.word .L0024D6E4\n.word .L0024D734\n.word .L0024D6E4\n.word .L0024D734\n.word .L0024C3D8\n.word .L0024C49C\n.word .L0024C514\n.word .L0024C5E4\n.word .L0024C844\n.word .L0024CA74\n.word .L0024D6E4\n.word .L0024CCBC\n.word .L0024CD10\n.word .L0024CD84\n.word .L0024D6E4\n.word .L0024D6E4\n.word .L0024CE70\n.word .L0024CEE4\n.word .L0024CF00\n.word .L0024CF28\n.word .L0024CF70\n.word .L0024CFC4\n.word .L0024D02C\n.word .L0024D07C\n.word .L0024D10C\n.word .L0024D198\n.word .L0024D1B8\n.word .L0024D23C\n.word .L0024D6E4\n.word .L0024D30C\n.word .L0024D344\n.word .L0024D394\n.word .L0024D6E4\n.word .L0024D54C\n.word .L0024D5F0\n.word .L0024D734\n.word .L0024D63C\n.word .L0024D6E4\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D6E4\n.word .L0024D734\n.word .L0024D6E4\n.word .L0024D734\n.word .L0024992C\n.word .L00249F64\n.word .L0024A090\n.word .L0024A11C\n.word .L0024A1E0\n.word .L0024A230\n.word .L0024A2C8\n.word .L0024A388\n.word .L0024BFF8\n.word .L0024C22C\n.word .L0024D400\n.word .L0024D454\n.word .L0024D4A8\n.word .L0024A3D0\n.word .L0024A42C\n.word .L0024A4C4\n.word .L0024A5A0\n.word .L0024A8AC\n.word .L0024A900\n.word .L0024A99C\n.word .L0024D734\n.word .L0024A9B8\n.word .L0024AAC4\n.word .L0024AB74\n.word .L0024AC6C\n.word .L0024ADAC\n.word .L0024AE5C\n.word .L0024AF88\n.word .L0024B018\n.word .L0024B1C0\n.word .L0024B254\n.word .L0024B2C0\n.word .L0024B4B8\n.word .L0024B560\n.word .L0024B788\n.word .L0024B870\n.word .L0024B8D8\n.word .L0024B950\n.word .L0024B9AC\n.word .L0024BA48\n.word .L0024BBC4\n.word .L0024BD94\n.word .L0024BDEC\n.word .L0024BE44\n.word .L0024D734\n.word .L0024D734\n.word func_0\n.word func_0\n.text\n");
__asm__(".rdata\n.align 4\n.word .L00248848\n.word .L00248860\n.word .L00248870\n.word .L00248880\n.word .L00248890\n.word .L002488A0\n.word .L00248950\n.word .L0024897C\n.word .L00248A04\n.word .L00248A18\n.word .L00248A2C\n.word .L00248A48\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L00248AC0\n.word .L00248AD0\n.word .L00248B0C\n.word .L00248B1C\n.word .L00248B4C\n.word .L00248B70\n.word .L00248BB8\n.word .L00248BE8\n.word .L00248BF8\n.word .L00248C38\n.word .L00248C78\n.word .L00248CC8\n.word .L00248D00\n.word .L00248D18\n.word .L00248D3C\n.word .L00248D60\n.word .L00248D88\n.word .L00248D98\n.word .L00248DA8\n.word .L00248DB8\n.word .L00248DD0\n.word .L00248DF8\n.word .L00248E10\n.word .L00248E30\n.word .L00248E48\n.word .L00248E60\n.word .L00248E8C\n.word .L00248E9C\n.word .L00248EAC\n.word .L00248EC8\n.word .L00248EEC\n.word .L00248F20\n.word .L00248F60\n.word .L00248F94\n.word .L00248FD4\n.word .L00248FF0\n.word .L00249074\n.word .L002490C8\n.word .L002490F4\n.word .L00249110\n.word .L00249140\n.word .L00249278\n.word .L002492B4\n.word .L0024930C\n.word .L00249338\n.word .L0024938C\n.word .L002493B8\n.word .L002493DC\n.word .L0024944C\n.word .L00249468\n.word .L0024950C\n.word .L00249558\n.word .L00249570\n.word .L00249580\n.word .L0024959C\n.word .L002495C4\n.word .L002495D4\n.word .L0024961C\n.word .L00249670\n.word .L00249694\n.word .L002496D8\n.word .L00249734\n.word .L0024975C\n.word .L00249778\n.word .L00249790\n.text\n");
__asm__(".rdata\n.align 4\n.word .L00249700\n.word .L00249708\n.word .L00249710\n.word .L00249718\n.word .L00249720\n.word .L00249728\n.word func_0\n.word func_0\n.text\n");
__asm__(".rdata\n.align 4\n.word .L00249994\n.word .L002499D4\n.word .L0024D734\n.word .L0024D734\n.word .L00249A14\n.word .L00249A54\n.word .L00249A94\n.word .L00249AD4\n.word .L00249B14\n.word .L00249B54\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L0024D734\n.word .L00249B94\n.word .L00249BFC\n.word .L00249C3C\n.word .L00249C7C\n.word .L00249CBC\n.word .L00249CFC\n.word .L00249D7C\n.word .L00249DBC\n.word .L00249DFC\n.word .L00249E3C\n.word .L00249E7C\n.word .L00249F04\n.word .L00249F18\n.word func_0\n.word func_0\n.text\n");
__asm__(".rdata\n.align 4\n.word .L0024A600\n.word .L0024A63C\n.word .L0024A68C\n.word .L0024A6D4\n.word .L0024A714\n.word .L0024A75C\n.word .L0024A79C\n.word .L0024A7E4\n.word .L0024A824\n.word .L0024A86C\n.word func_0\n.word func_0\n.text\n");
__asm__(".rdata\n.align 4\n.word .L0024ACE8\n.word .L0024AD00\n.word .L0024AD28\n.word .L0024AD50\n.word .L0024AD6C\n.word .L0024AD88\n.word func_0\n.word func_0\n.text\n");
__asm__(".rdata\n.align 4\n.word .L0024AE10\n.word .L0024AE18\n.word .L0024AE20\n.word .L0024AE28\n.word .L0024AE30\n.word .L0024AE38\n.word func_0\n.word func_0\n.text\n");
__asm__(".rdata\n.align 4\n.word .L0024AED8\n.word .L0024AEEC\n.word .L0024AF0C\n.word .L0024AF20\n.word .L0024AF40\n.word .L0024AF54\n.word .L0024AF74\n.word func_0\n.text\n");
__asm__(".rdata\n.align 4\n.word .L0024BEB8\n.word .L0024BECC\n.word .L0024BF0C\n.word .L0024BF4C\n.word .L0024BFA8\n.word func_0\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", execExt3Code__Q24ATEL4CVSMiRQ24ATEL13eATEL_REQ_RET);
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
   10 min (50056 candidates, no better than 4). */
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
/* placeholder: rodata of player_ctrl__Q24ATEL4CVSMi */
__asm__(".rdata\n");
inline void _roPh42() {
    TRACE("SHOW :%s\n");
    TRACE("HIDE :%s\n");
    TRACE("AVTCMD_START_AVATAR  :%s\n");
    TRACE("bad param player_ctrl :%s\n");
}
__asm__(".rdata\n.align 4\n.word .L0024F080\n.word .L0024F0B8\n.word .L0024F604\n.word .L0024F604\n.word .L0024F0F0\n.word .L0024F128\n.word .L0024F160\n.word .L0024F198\n.word .L0024F1D0\n.word .L0024F208\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F604\n.word .L0024F240\n.word .L0024F2AC\n.word .L0024F2E4\n.word .L0024F31C\n.word .L0024F354\n.word .L0024F38C\n.word .L0024F3F8\n.word .L0024F430\n.word .L0024F468\n.word .L0024F4A0\n.word .L0024F4D8\n.word .L0024F550\n.word .L0024F588\n.word .L0024F5C0\n.word func_0\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", player_ctrl__Q24ATEL4CVSMi);
__asm__(".rdata\n"); /* gcc believes it is in .rdata (placeholder), INCLUDE_ASM left .text */
INT CVSM::player_ctrl(INT nCode, FLOAT fArg) {
    if (m_pCurrentActor->_pAvatar) _TRACE(m_bTrace, "bad param player_control\n");
    return -2;
}
/* placeholder: rodata of cam_ctrl__Q24ATEL4CVSMiiRi */
__asm__(".rdata\n");
__asm__(".rdata\n.align 4\n.word .L0024F6AC\n.word .L0024F6E8\n.word .L0024F724\n.word .L0024F760\n.word .L0024F7C8\n.word .L0024F824\n.word .L0024F864\n.word .L0024F8A4\n.word .L0024F8C4\n.word .L0024F8E4\n.word .L0024F904\n.word .L0024F924\n.word .L0024F944\n.word .L0024F96C\n.word .L0024F9A0\n.word .L0024F9C8\n.word .L0024FA04\n.word .L0024FA40\n.word .L0024FADC\n.word .L0024FA80\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", cam_ctrl__Q24ATEL4CVSMiiRi);
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
/* placeholder: rodata of ctd_cal__Q24ATEL4CVSMPCQ24ATEL7CAvatariiPQ24ATEL21ChatWndCreateCTDParam */
__asm__(".rdata\n");
inline void _roPh44() {
    TRACE("chobi-man %d\n");
    TRACE(">>>>>chibi(CMA) pos %d : %d\n");
    TRACE("neck");
    TRACE("neck1");
    TRACE("head");
    TRACE("ATEL:CVSM::ctd_cal\x81" "F\x83\x81\x83" "b\x83Z\x81[\x83W\x83" "E\x83" "C\x83\x93\x83h\x83" "E\x81" "F\x83`\x83\x87\x83r\x82\xf0\x8c\xfc\x82\xaf\x82\xe9\x83{\x81[\x83\x93\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81" "E\x81" "E\x81" "E\n");
}
__asm__(".rdata\n.align 4\n.word .L0024FD70\n.word .L0024FD74\n.word .L0024FD7C\n.word .L0024FD8C\n.word .L0024FD98\n.word .L0024FD94\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", ctd_cal__Q24ATEL4CVSMPCQ24ATEL7CAvatariiPQ24ATEL21ChatWndCreateCTDParam);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", __static_initialization_and_destruction_0_002505D8);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", _TRACE___4ATELiPCce);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", isInitComplete__CQ24ATEL4CVSM);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", startMain__Q24ATEL4CVSM);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", isMainStarted__CQ24ATEL4CVSM);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", getAvatar__Q24ATEL4CVSMi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", getEventTrap__Q24ATEL4CVSMi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", getAvatarName__Q24ATEL4CVSMi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", getActorNum__Q24ATEL4CVSM);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", setPC__Q24ATEL4CVSMi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", getPC__CQ24ATEL4CVSM);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", clearFlag__Q24ATEL4CVSMi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", setFlag__Q24ATEL4CVSMi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", isFlagOn__CQ24ATEL4CVSMi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/VSME", _GLOBAL_$I$c__KHCOM_MainApp_Game_Atel_VSME.cppEPaaaa);
/* Placeholder for the end of .rodata, which gcc emits after the vtables (a deferred static const): CVSM's vtable
   (its key function ~CVSM is still asm), the static initializer's constants and strings (the initializer is asm)
   and execExt3Code's cardDataTbl (asm). Bytes from the original, pointers as relocations. */
extern "C" char _rt0[] __asm__("D_0048DF10");
extern "C" char _rt1[] __asm__("D_0048E1A0");
extern "C" char _rt2[] __asm__("D_00491D80");
extern "C" char _rt3[] __asm__("D_00491DC8");
extern "C" char _rt4[] __asm__("D_00491E10");
extern "C" char _rt5[] __asm__("D_00491E58");
extern "C" char _rt6[] __asm__("D_00491EA0");
extern "C" char _rt7[] __asm__("D_00491F08");
extern "C" char _rt8[] __asm__("D_00491F50");
extern "C" char _rt9[] __asm__("D_00491F98");
extern "C" char _rt10[] __asm__("D_00492028");
extern "C" char _rt11[] __asm__("D_00492070");
extern "C" char _rt12[] __asm__("D_004920B8");
extern "C" char _rt13[] __asm__("D_00492100");
extern "C" char _rt14[] __asm__("D_00492148");
extern "C" char _rt15[] __asm__("D_00492190");
extern "C" char _rt16[] __asm__("D_00492248");
extern "C" char _rt17[] __asm__("D_00492290");
extern "C" char _rt18[] __asm__("D_004922D8");
extern "C" char _rt19[] __asm__("D_00492320");
extern "C" char _rt20[] __asm__("D_004923A8");
extern "C" char _rt21[] __asm__("D_00492430");
extern "C" char _rt22[] __asm__("D_004924C8");
extern "C" char _rt23[] __asm__("D_00492530");
extern "C" char _rt24[] __asm__("D_00492580");
extern "C" char _rt25[] __asm__("D_004925B8");
extern "C" char _rt26[] __asm__("D_004925F0");
extern "C" char _rt27[] __asm__("D_00492628");
extern "C" char _rt28[] __asm__("D_00492660");
extern "C" char _rt29[] __asm__("D_00492698");
extern "C" char _rt30[] __asm__("D_004926D0");
extern "C" char _rt31[] __asm__("D_00492708");
extern "C" char _rt32[] __asm__("D_00492740");
extern "C" char _rt33[] __asm__("D_00492790");
extern "C" char _rt34[] __asm__("D_004927F0");
extern "C" char _rt35[] __asm__("D_00492800");
extern "C" char _rt36[] __asm__("D_00492810");
extern "C" char _rt37[] __asm__("D_00492820");
extern "C" char _rt38[] __asm__("D_00492830");
extern "C" char _rt39[] __asm__("D_00492840");
extern "C" char _rt40[] __asm__("D_00492898");
extern "C" char _rt41[] __asm__("D_004929F8");
extern "C" char _rt42[] __asm__("D_00492A48");
extern "C" char _rt43[] __asm__("D_00492A88");
extern "C" char _rt44[] __asm__("D_00492AC8");
extern "C" char _rt45[] __asm__("D_00492B00");
extern "C" char _rt46[] __asm__("D_00492B38");
extern "C" char _rt47[] __asm__("D_00492B78");
extern "C" char _rt48[] __asm__("D_00492BB0");
extern "C" char _rt49[] __asm__("D_00492BE8");
extern "C" char _rt50[] __asm__("D_00492C20");
extern "C" char _rt51[] __asm__("D_00492C60");
extern "C" char _rt52[] __asm__("D_00492C98");
extern "C" char _rt53[] __asm__("D_00492CD0");
extern "C" char _rt54[] __asm__("D_00492D00");
extern "C" char _rt55[] __asm__("D_00492D40");
extern "C" char _rt56[] __asm__("D_00492DD0");
extern "C" char _rt57[] __asm__("D_00492E18");
extern "C" char _rt58[] __asm__("D_00492E60");
extern "C" char _rt59[] __asm__("D_00492EA8");
extern "C" char _rt60[] __asm__("D_00492EF0");
extern "C" char _rt61[] __asm__("D_00492F38");
extern "C" char _rt62[] __asm__("D_00492F80");
extern "C" char _rt63[] __asm__("D_00492FC8");
extern "C" char _rt64[] __asm__("D_00493010");
extern "C" char _rt65[] __asm__("D_00493058");
extern "C" char _rt66[] __asm__("D_004930A0");
extern "C" char _rt67[] __asm__("D_004930E8");
extern "C" char _rt68[] __asm__("D_006884E0");
extern "C" char _rt69[] __asm__("D_00688508");
extern "C" char _rt70[] __asm__("D_00688530");
extern "C" char _rt71[] __asm__("D_00688558");
extern "C" char _rt72[] __asm__("D_00688760");
extern "C" char _rt73[] __asm__("D_00688788");
extern "C" char _rt74[] __asm__("D_006887B0");
extern "C" char _rt75[] __asm__("D_006887D8");
extern "C" char _rt76[] __asm__("D_00688800");
extern "C" char _rt77[] __asm__("D_00688E68");
extern "C" char _rt78[] __asm__("D_00688E90");
extern "C" char _rt79[] __asm__("D_00688EB8");
extern "C" char _rt80[] __asm__("D_00688EE0");
extern "C" char _rt81[] __asm__("DebugSetGameWorkParam__8DWTEventP12CDebugWindowii");
extern "C" char _rt82[] __asm__("DebugSetPoohFlag__8DWTEventP12CDebugWindowii");
extern "C" char _rt83[] __asm__("_$_Q24ATEL4CVSM");
extern "C" char _rt84[] __asm__("_12CDebugWindow$none");
extern "C" char _rt85[] __asm__("_3DWT$szOffOn");
extern "C" char _rt86[] __asm__("_3DWT$szOnOff");
extern "C" char _rt87[] __asm__("_9DebugWork$g_dw");
extern "C" char _rt88[] __asm__("debugBtlReportCallBackParam__8DWTEventP12CDebugWindowii");
extern "C" char _rt89[] __asm__("debugGameInfomationParam__8DWTEventP12CDebugWindowii");
static const struct {
    char b0[12];
    const void* p1;
    char b2[8];
    const void* p3;
    const void* p4;
    char b5[96];
    const void* p6;
    const void* p7;
    char b8[64];
    const void* p9;
    const void* p10;
    char b11[64];
    const void* p12;
    const void* p13;
    char b14[24];
    const void* p15;
    char b16[36];
    const void* p17;
    const void* p18;
    char b19[64];
    const void* p20;
    const void* p21;
    char b22[24];
    const void* p23;
    char b24[68];
    const void* p25;
    const void* p26;
    char b27[24];
    const void* p28;
    char b29[36];
    const void* p30;
    const void* p31;
    char b32[64];
    const void* p33;
    const void* p34;
    char b35[32];
    const void* p36;
    const void* p37;
    char b38[96];
    const void* p39;
    const void* p40;
    char b41[64];
    const void* p42;
    const void* p43;
    char b44[64];
    const void* p45;
    const void* p46;
    char b47[24];
    const void* p48;
    char b49[36];
    const void* p50;
    const void* p51;
    char b52[24];
    const void* p53;
    char b54[36];
    const void* p55;
    const void* p56;
    char b57[64];
    const void* p58;
    const void* p59;
    char b60[24];
    const void* p61;
    char b62[148];
    const void* p63;
    const void* p64;
    unsigned char b65[16];
    const void* p66;
    char b67[4];
    const void* p68;
    char b69[36];
    const void* p70;
    const void* p71;
    unsigned char b72[16];
    const void* p73;
    char b74[4];
    const void* p75;
    char b76[36];
    const void* p77;
    const void* p78;
    unsigned char b79[16];
    const void* p80;
    char b81[4];
    const void* p82;
    char b83[36];
    const void* p84;
    const void* p85;
    unsigned char b86[16];
    const void* p87;
    char b88[4];
    const void* p89;
    char b90[100];
    const void* p91;
    const void* p92;
    unsigned char b93[16];
    const void* p94;
    char b95[4];
    const void* p96;
    char b97[100];
    const void* p98;
    const void* p99;
    unsigned char b100[16];
    const void* p101;
    char b102[124];
    const void* p103;
    const void* p104;
    char b105[104];
    const void* p106;
    const void* p107;
    char b108[48];
    const void* p109;
    const void* p110;
    char b111[16];
    const void* p112;
    char b113[4];
    const void* p114;
    char b115[20];
    const void* p116;
    const void* p117;
    char b118[16];
    const void* p119;
    char b120[4];
    const void* p121;
    char b122[20];
    const void* p123;
    const void* p124;
    char b125[16];
    const void* p126;
    char b127[4];
    const void* p128;
    char b129[20];
    const void* p130;
    const void* p131;
    char b132[16];
    const void* p133;
    char b134[4];
    const void* p135;
    char b136[20];
    const void* p137;
    const void* p138;
    char b139[16];
    const void* p140;
    char b141[4];
    const void* p142;
    char b143[20];
    const void* p144;
    const void* p145;
    char b146[16];
    const void* p147;
    char b148[4];
    const void* p149;
    char b150[20];
    const void* p151;
    const void* p152;
    char b153[16];
    const void* p154;
    char b155[4];
    const void* p156;
    char b157[20];
    const void* p158;
    const void* p159;
    char b160[16];
    const void* p161;
    char b162[4];
    const void* p163;
    char b164[44];
    const void* p165;
    const void* p166;
    char b167[72];
    const void* p168;
    const void* p169;
    char b170[176];
    const void* p171;
    const void* p172;
    char b173[72];
    const void* p174;
    const void* p175;
    char b176[32];
    const void* p177;
    const void* p178;
    char b179[24];
    const void* p180;
    char b181[4];
    const void* p182;
    const void* p183;
    char b184[24];
    const void* p185;
    char b186[4];
    const void* p187;
    const void* p188;
    char b189[24];
    const void* p190;
    char b191[4];
    const void* p192;
    const void* p193;
    char b194[24];
    const void* p195;
    char b196[4];
    const void* p197;
    const void* p198;
    char b199[24];
    const void* p200;
    char b201[116];
    const void* p202;
    const void* p203;
    char b204[56];
    const void* p205;
    const void* p206;
    char b207[28];
    const void* p208;
    char b209[24];
    const void* p210;
    const void* p211;
    char b212[28];
    const void* p213;
    char b214[16];
    const void* p215;
    const void* p216;
    char b217[28];
    const void* p218;
    char b219[16];
    const void* p220;
    const void* p221;
    char b222[28];
    const void* p223;
    char b224[24];
    const void* p225;
    const void* p226;
    char b227[28];
    const void* p228;
    char b229[16];
    const void* p230;
    const void* p231;
    char b232[28];
    const void* p233;
    char b234[16];
    const void* p235;
    const void* p236;
    char b237[28];
    const void* p238;
    char b239[16];
    const void* p240;
    const void* p241;
    char b242[28];
    const void* p243;
    char b244[24];
    const void* p245;
    const void* p246;
    char b247[28];
    const void* p248;
    char b249[16];
    const void* p250;
    const void* p251;
    char b252[28];
    const void* p253;
    char b254[16];
    const void* p255;
    const void* p256;
    char b257[28];
    const void* p258;
    char b259[8];
    const void* p260;
    const void* p261;
    char b262[28];
    const void* p263;
    char b264[24];
    const void* p265;
    const void* p266;
    char b267[28];
    const void* p268;
    char b269[40];
    const void* p270;
    const void* p271;
    char b272[128];
    const void* p273;
    const void* p274;
    unsigned char b275[16];
    const void* p276;
    char b277[4];
    const void* p278;
    char b279[36];
    const void* p280;
    const void* p281;
    unsigned char b282[16];
    const void* p283;
    char b284[4];
    const void* p285;
    char b286[36];
    const void* p287;
    const void* p288;
    char b289[16];
    const void* p290;
    char b291[44];
    const void* p292;
    const void* p293;
    char b294[16];
    const void* p295;
    char b296[44];
    const void* p297;
    const void* p298;
    unsigned char b299[16];
    const void* p300;
    char b301[4];
    const void* p302;
    char b303[36];
    const void* p304;
    const void* p305;
    unsigned char b306[16];
    const void* p307;
    char b308[4];
    const void* p309;
    char b310[36];
    const void* p311;
    const void* p312;
    char b313[16];
    const void* p314;
    char b315[44];
    const void* p316;
    const void* p317;
    char b318[16];
    const void* p319;
    char b320[44];
    const void* p321;
    const void* p322;
    char b323[16];
    const void* p324;
    char b325[44];
    const void* p326;
    const void* p327;
    unsigned char b328[16];
    const void* p329;
    char b330[4];
    const void* p331;
    char b332[36];
    const void* p333;
    const void* p334;
    unsigned char b335[16];
    const void* p336;
    char b337[4];
    const void* p338;
    char b339[36];
    const void* p340;
    const void* p341;
    unsigned char b342[16];
    const void* p343;
    char b344[4];
    const void* p345;
    char b346[4676];
} _roTail __asm__("_vt$Q24ATEL4CVSM") = {
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt83,
    "\x00\x00\x00\x00\x00\x00\x00",
    _rt0,
    _rt84,
    "\x01\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x83Q\x81[\x83\x80\x83\x8c\x83x\x83\x8b       \x81@:%s\x00\x00\x00\x00\x00\x00\x00\x00\x83W\x83~\x83j\x81[\x92" "B\x90\xac\x97\xa6     \x81@:%d\x00\x00\x00\x00\x00\x00\x00",
    _rt2,
    _rt87 + 0x3a0,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80?\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x83R\x83\x8c\x83N\x83^\x81[\x83Y\x83\x89\x83\x93\x83N \x81@:--\x00\x00\x00\x00\x00\x00\x00",
    _rt3,
    _rt84,
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80?\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x93|\x82\xb5\x82\xbd\x93G\x82\xcc\x90\x94\x81@\x81@\x81@ \x81@:%d\x00\x00\x00\x00\x00\x00\x00",
    _rt4,
    _rt87 + 0x3a4,
    "\x04\x00\x00\x00\x00\x00\x00\x00\xf0#tI\x00\x00zD\x00\x00\x00\x00\x00\x00\x00",
    _rt88,
    "\x00\x00\x00\x00\x96\xa2\x92m\x82\xc8\x82\xe9\x95\xf3\x82\xcc\x90\x94     \x81@:%d/24\x00\x00\x00\x00",
    _rt5,
    _rt87 + 0x3ac,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80?\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x83J\x81[\x83h\x83u\x83\x8c\x83" "C\x83N\x89\xf1\x90\x94 \x81@:%d\x00\x00\x00\x00\x00\x00\x00",
    _rt6,
    _rt87 + 0x3a8,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x80O\xc3G\x00\x00\xc8" "B\x00\x00\x00\x00\x00\x00\x00",
    _rt88,
    "\x00\x00\x00\x00\x8d\xc5\x82\xe0\x8eg\x82\xc1\x82\xbd\x83X\x83g\x83" "b\x83N\x8bZ :%s\x00\x00\x00\x00\x00\x00\x00\x00        \x81@\x81@\x81@\x81@\x89\xf1\x90\x94 :%d\x00\x00\x00\x00\x00\x00\x00",
    _rt7,
    _rt87 + 0x3b4,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x80O\xc3G\x00\x00\xc8" "B\x00\x00\x00\x00\x00\x00\x00",
    _rt88,
    "\x00\x00\x00\x00\x83N\x83\x8a\x83" "A\x8e\x9e\x8a\xd4\x81@\x81@\x81@     :%d\x00\x00\x00\x00\x00\x00\x00",
    _rt8,
    _rt87 + 0x3b8,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80?\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x81@\x81@  \x81@\x95\xaa\x81@\x81@\x81@\x81@   :%d\x00\x00\x00\x00\x00\x00\x00",
    _rt9,
    _rt87 + 0x3bc,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80?\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt1,
    _rt84,
    "\x01\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x83Q\x81[\x83\x80\x83\x8c\x83x\x83\x8b\x81@\x81@\x81@\x81@ :%s\x00\x00\x00\x00\x00\x00\x00\x00\x82" "c\x83\x8c\x83|\x81[\x83g\x92" "B\x90\xac\x97\xa6     :%d\x00\x00\x00\x00\x00\x00\x00",
    _rt10,
    _rt87 + 0x3a0,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80?\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x83R\x83\x8c\x83N\x83^\x81[\x83Y\x83\x89\x83\x93\x83N   :--\x00\x00\x00\x00\x00\x00\x00",
    _rt11,
    _rt84,
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80?\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x83X\x83s\x81[\x83h\x83u\x83\x8c\x83" "C\x83N     :%d\x00\x00\x00\x00\x00\x00\x00",
    _rt12,
    _rt87 + 0x3c0,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x80O\xc3G\x00\x00\xc8" "B\x00\x00\x00\x00\x00\x00\x00",
    _rt88,
    "\x00\x00\x00\x00\x93|\x82\xb5\x82\xbd\x93G\x82\xcc\x90\x94\x81@\x81@     :%d\x00\x00\x00\x00\x00\x00\x00",
    _rt13,
    _rt87 + 0x3a4,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x80O\xc3G\x00\x00\xc8" "B\x00\x00\x00\x00\x00\x00\x00",
    _rt88,
    "\x00\x00\x00\x00\x83" "f\x83\x85\x83G\x83\x8b\x82\xcc\x8f\x9f\x97\xa6       :%5.2f\x00\x00\x00\x00",
    _rt14,
    _rt87 + 0x3c4,
    "\x04\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x82" "c\x83\x82\x81[\x83h\x94\xad\x93\xae\x89\xf1\x90\x94     :%d\x00\x00\x00\x00\x00\x00\x00",
    _rt15,
    _rt87 + 0x3c8,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x80O\xc3G\x00\x00\xc8" "B\x00\x00\x00\x00\x00\x00\x00",
    _rt88,
    "\x00\x00\x00\x00\x83o\x83g\x83\x8b\x83\x8c\x83|\x81[\x83g\x00\x00\x96\xa2\x92m\x82\xc8\x82\xe9\x95\xf3\x82\xa0\x82\xaf\x82\xbd\x90\x94\x00\x00\x00\x00\x00\x00\x83W\x83~\x83j\x81[\x83\x81\x83\x82\x00\x00\x00\x00\x82j\x82g\x82Q\x83J\x81[\x83h\x93\xfc\x8e\xe8\x00\x00\x00\x00\x00\x00\x00\x00\x83v\x83\x8c\x83" "C\x83\x82\x81[\x83h         : %s\x00\x00\x00\x00\x00\x00\x00\x83R\x83x\x83" "c\x83Q\x81[\x83\x80\x83N\x83\x8a\x83" "A   : %s\x00\x00\x00\x00\x00\x00",
    _rt16,
    _rt87 + 0x37e,
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x80, 0x3f },
    _rt85,
    "\x00\x00\x00",
    _rt89,
    "\x00\x00\x00\x00\x82j\x82g\x82Q\x83N\x83\x8a\x83" "A         : %s\x00\x00\x00\x00\x00\x00",
    _rt17,
    _rt87 + 0x37a,
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x80, 0x3f },
    _rt85,
    "\x00\x00\x00",
    _rt89,
    "\x00\x00\x00\x00\x83\\\x83\x89\x83N\x83\x8a\x83" "A           : %s\x00\x00\x00\x00\x00\x00",
    _rt18,
    _rt87 + 0x37d,
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x80, 0x3f },
    _rt85,
    "\x00\x00\x00",
    _rt89,
    "\x00\x00\x00\x00\x83\x8a\x83N\x83N\x83\x8a\x83" "A           : %s\x00\x00\x00\x00\x00\x00",
    _rt19,
    _rt87 + 0x37c,
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x80, 0x3f },
    _rt85,
    "\x00\x00\x00",
    _rt89,
    "\x00\x00\x00\x00\x83\\\x83\x89" "EN\x83t\x83\x89\x83O         : %d\x00\x00\x00\x00\x00\x00\x00\x83\x8a\x83NEN\x83t\x83\x89\x83O         : %d\x00\x00\x00\x00\x00\x00\x00\x83}\x81[\x83\x8b\x81[\x83V\x83\x83KILL     : %s\x00\x00\x00\x00\x00\x00",
    _rt20,
    _rt87 + 0x37b,
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x80, 0x3f },
    _rt85,
    "\x00\x00\x00",
    _rt89,
    "\x00\x00\x00\x00\x83v\x83\x8c\x83" "C\x82\xc8\x82\xf1\x82\xa2\x82\xc7       : %s\x00\x00\x00\x00\x00\x00\x00\x83T\x83" "E\x83\x93\x83h\x83\x82\x81[\x83h       : %s\x00\x00\x00\x00\x00\x00\x00\x83o\x83" "C\x83u\x83\x8c\x81[\x83V\x83\x87\x83\x93     : %s\x00\x00\x00\x00\x00\x00",
    _rt21,
    _rt87 + 0x380,
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x80, 0x3f },
    _rt86,
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x83Q\x81[\x83\x80\x8f\xee\x95\xf1\x00\x00\x00\x00\x00\x00\x83`\x83\x85\x81[\x83g\x83\x8a\x83" "A\x83\x8b\x8fI\x97\xb9\x00\x00\x00\x00\x00\x00\x83\x80\x81[\x83r\x81[\x00\x00\x00\x00\x00\x00\x00\x00\x83v\x83\x8c\x83" "C\x83\x84\x81[\x83X\x83L\x83\x8b\x00\x00\x00\x00\x00\x00\x00\x00\xf9S----------\xf9V\x83Q\x81[\x83\x80\xf9S---------",
    _rt22,
    _rt84,
    "\x01\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x82\xb6\x82\xe5\x82\xa4\x82\xbd\x82\xa2  :%s\x00\x82\xb5\x82\xe3\x82\xe9\x82\xa2    :%s\x00\xf9S----------\xf9V\x82\xc7\x82\xa4\x82\xc9\x82\xe3\x82\xa4\xf9S---------\x00\x00\x00\x00",
    _rt23,
    _rt84,
    "\x01\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x83s\x83O\x83\x8c\x83" "b\x83g  :%s",
    _rt24,
    _rt87 + 0x334,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt85,
    "\x01\x00\x00",
    _rt82,
    "\x00\x00\x00\x00\x83I\x83" "E\x83\x8b      :%s",
    _rt25,
    _rt87 + 0x334,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt85,
    "\x02\x00\x00",
    _rt82,
    "\x00\x00\x00\x00\x83\x8b\x81[        :%s",
    _rt26,
    _rt87 + 0x334,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt85,
    "\x04\x00\x00",
    _rt82,
    "\x00\x00\x00\x00\x83" "C\x81[\x83\x88\x81[    :%s",
    _rt27,
    _rt87 + 0x334,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt85,
    "\x08\x00\x00",
    _rt82,
    "\x00\x00\x00\x00\x83" "e\x83" "B\x83K\x81[    :%s",
    _rt28,
    _rt87 + 0x334,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt85,
    "\x10\x00\x00",
    _rt82,
    "\x00\x00\x00\x00\x83\x89\x83r\x83" "b\x83g    :%s",
    _rt29,
    _rt87 + 0x334,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt85,
    " \x00\x00",
    _rt82,
    "\x00\x00\x00\x00\x83S\x81[\x83\x8b\x82P    :%s",
    _rt30,
    _rt87 + 0x334,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt85,
    "@\x00\x00",
    _rt82,
    "\x00\x00\x00\x00\x83S\x81[\x83\x8b\x82Q    :%s",
    _rt31,
    _rt87 + 0x334,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt85,
    "\x80\x00\x00",
    _rt82,
    "\x00\x00\x00\x00\xf9S----------\xf9V\x82\xb6\x82\xe5\x82\xa4\x82\xbd\x82\xa2\xf9S---------\x00\x00\x00\x00",
    _rt32,
    _rt84,
    "\x01\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xf9S----------\xf9V\x83N\x83\x8a\x83" "A\x97p\xf9S---------\x00\x00\x00\x00\x00\x00",
    _rt33,
    _rt84,
    "\x01\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x83s\x83O\x83\x8c\x83" "b\x83g  :%d\x00\x83I\x83" "E\x83\x8b      :%d\x00\x83\x8b\x81[        :%d\x00\x83" "C\x81[\x83\x88\x81[    :%d\x00\x83" "e\x83" "B\x83K\x81[    :%d\x00\x83\x89\x83r\x83" "b\x83g    :%d\x00\xf9S----------\xf9V\x8eg\x82\xa2\x8e\xcc\x82\xc4\x83N\x83\x8a\x83" "A\x97p\xf9S---------\x00\x00\x00\x00\x00\x00",
    _rt39,
    _rt84,
    "\x01\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xf9S----------\xf9V\x83n\x83" "C\x83X\x83R\x83" "A\xf9S---------\x00\x00\x00\x00",
    _rt40,
    _rt84,
    "\x01\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt34,
    _rt87 + 0x364,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x00@\x1c" "F\x00\x00\xc8" "B\x00\x00\x00\x00\x00\x00\x00",
    _rt82,
    "\x00\x00\x00",
    _rt35,
    _rt87 + 0x368,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x00@\x1c" "F\x00\x00\xc8" "B\x00\x00\x00\x00\x00\x00\x00",
    _rt82,
    "\x00\x00\x00",
    _rt36,
    _rt87 + 0x36c,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x00@\x1c" "F\x00\x00\xc8" "B\x00\x00\x00\x00\x00\x00\x00",
    _rt82,
    "\x00\x00\x00",
    _rt37,
    _rt87 + 0x370,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x00@\x1c" "F\x00\x00\xc8" "B\x00\x00\x00\x00\x00\x00\x00",
    _rt82,
    "\x00\x00\x00",
    _rt38,
    _rt87 + 0x374,
    "\x04\x00\x00\x00\x00\x00\x00\x00\x00@\x1c" "F\x00\x00\xc8" "B\x00\x00\x00\x00\x00\x00\x00",
    _rt82,
    "\x00\x00\x00\x00\x83v\x81[\x00\x00\x00\x00\x83t\x83\x8c\x83\x93\x83h\x82\xa9\x82\xa2\x82\xab\x82\xf1\x00\x00\x00\x00\x00\x00\x00\x00\x83J\x81[\x83h\x82\xa9\x82\xa2\x82\xab\x82\xf1\x00\x00\x83\x8f\x81[\x83\x8b\x83h\x83^\x83" "C\x83g\x83\x8b\x00\x00\x00\x00\x00\x00\x00\x00\xf9S----------\xf9VWLV_STORAGE\xf9S---------\x00\x00\x00",
    _rt41,
    _rt84,
    "\x01\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x83\x8f\x81[\x83\x8b\x83h\x83^\x83" "C\x83g\x83\x8b...\x00\x00\x00\x00",
    _rt42,
    _rt84,
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt80,
    "\x83`\x83\x85\x81[\x83g\x83\x8a\x83" "A\x83\x8b...\x00\x00\x00\x00\x00\x00",
    _rt43,
    _rt84,
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt73,
    "\x83J\x81[\x83h\x89\xf0\x8b\xd6...\x00\x00",
    _rt44,
    _rt84,
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt79,
    "\x83t\x83\x8c\x83\x93\x83h\x89\xf0\x8b\xd6...",
    _rt45,
    _rt84,
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt78,
    "\x82P\x82O\x82O\x83G\x81[\x83J\x81[...\x00\x00\x00\x00\x00\x00",
    _rt46,
    _rt84,
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt77,
    "\x95W\x8f\x80\x83X\x83L\x83\x8b...\x00\x00",
    _rt47,
    _rt84,
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt76,
    "\x83\x80\x81[\x83r\x81[\x83\\\x83\x89...",
    _rt48,
    _rt84,
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt75,
    "\x83\x80\x81[\x83r\x81[\x83\x8a\x83N...",
    _rt49,
    _rt84,
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt74,
    "\x82j\x82g\x82Q\x83J\x81[\x83h\x93\xfc\x8e\xe8...\x00\x00\x00\x00",
    _rt50,
    _rt84,
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt71,
    "\x83W\x83~\x83j\x81[\x83\x81\x83\x82...",
    _rt51,
    _rt84,
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt70,
    "\x83Q\x81[\x83\x80\x8f\xee\x95\xf1...\x00\x00",
    _rt52,
    _rt84,
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt72,
    "\x95\xf3\x94\xa0...",
    _rt53,
    _rt84,
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt69,
    "\x83o\x83g\x83\x8b\x83\x8c\x83|\x81[\x83g...\x00\x00\x00\x00\x00\x00",
    _rt54,
    _rt84,
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt68,
    "\xf9S----------\xf9VWLV_GLOBAL\xf9S---------\x00\x00\x00\x00",
    _rt55,
    _rt84,
    "\x01\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" "BOSS\x83" "C\x83x\x83\x93\x83g\x83X\x83" "e\x83" "b\x83v : %d\x00\x00\x00\x00\x00\x00\x00\x83\x81\x83j\x83\x85\x81[\x82\xcc\x8e\xed\x97\xde       : %s\x00\x00\x00\x00\x00\x00\x00\x83v\x83\x8c\x83" "C\x83\x84\x81[\x94\xf1\x95\\\x8e\xa6     : %s\x00\x00\x00\x00\x00\x00",
    _rt56,
    _rt87 + 0x3d7,
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x80, 0x3f },
    _rt85,
    "\x00\x00\x00",
    _rt81,
    "\x00\x00\x00\x00\x82" "a\x82n\x82r\x82r\x8e\x80\x96S         : %s\x00\x00\x00\x00\x00\x00",
    _rt57,
    _rt87 + 0x3d8,
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x80, 0x3f },
    _rt85,
    "\x00\x00\x00",
    _rt81,
    "\x00\x00\x00\x00\x83" "C\x83x\x83\x93\x83g\x83X\x83L\x83" "b\x83v\x95s\x89\xc2 : %s\x00\x00\x00\x00\x00\x00",
    _rt58,
    _rt87 + 0x3d9,
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt85,
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" "ATEL\x83" "C\x83x\x83\x93\x83g\x92\x86       : %s\x00\x00\x00\x00\x00\x00",
    _rt59,
    _rt87 + 0x3da,
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt85,
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00PC\x91\x80\x8d\xec\x95s\x89\xc2           : %s\x00\x00\x00\x00\x00\x00",
    _rt60,
    _rt87 + 0x3db,
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x80, 0x3f },
    _rt85,
    "\x00\x00\x00",
    _rt81,
    "\x00\x00\x00\x00PAUSE\x8b\xd6\x8e~            : %s\x00\x00\x00\x00\x00\x00",
    _rt61,
    _rt87 + 0x3dc,
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x80, 0x3f },
    _rt85,
    "\x00\x00\x00",
    _rt81,
    "\x00\x00\x00\x00\x83`\x83\x85\x81[\x83g\x83\x8a\x83" "A\x83\x8b\x92\x86     : %s\x00\x00\x00\x00\x00\x00",
    _rt62,
    _rt87 + 0x3dd,
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt85,
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x83\x8b\x81[\x83\x80\x88\xda\x93\xae\x8b\xd6\x8e~       : %s\x00\x00\x00\x00\x00\x00",
    _rt63,
    _rt87 + 0x3de,
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt85,
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x83\x8b\x81[\x83\x80" "Creation\x8b\xd6\x8e~   : %s\x00\x00\x00\x00\x00\x00",
    _rt64,
    _rt87 + 0x3df,
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    _rt85,
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x83n\x81[\x83g\x8fo\x82\xb3\x82\xc8\x82\xa2       : %s\x00\x00\x00\x00\x00\x00",
    _rt65,
    _rt87 + 0x3e0,
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x80, 0x3f },
    _rt85,
    "\x00\x00\x00",
    _rt81,
    "\x00\x00\x00\x00" "BTL\x83`\x83\x85\x81[\x83g\x83\x8a\x83" "A\x83\x8b\x92\x86  : %s\x00\x00\x00\x00\x00\x00",
    _rt66,
    _rt87 + 0x3e1,
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x80, 0x3f },
    _rt85,
    "\x00\x00\x00",
    _rt81,
    "\x00\x00\x00\x00\x83\x8f\x81[\x83\x8b\x83h\x83^\x83" "C\x83g\x83\x8b\x92\x86   : %s\x00\x00\x00\x00\x00\x00",
    _rt67,
    _rt87 + 0x3e2,
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x80, 0x3f },
    _rt85,
    "\x00\x00\x00",
    _rt81,
    "\x00\x00\x00\x00" "ATEL WORK\x00\x00\x00\x00\x00\x00\x00" "ATEL\x81@CONFIG\x00\x00\x00\x00Kingdom Key\x00\x00\x00\x00\x00Three Wishes\x00\x00\x00\x00" "Crabclaw\x00\x00\x00\x00\x00\x00\x00\x00Pumpkinhead\x00\x00\x00\x00\x00" "Fairy Harp\x00\x00\x00\x00\x00\x00Wishing Star\x00\x00\x00\x00Spellbinder\x00\x00\x00\x00\x00Metal Chocobo\x00\x00\x00Olympia\x00Lionheart\x00\x00\x00\x00\x00\x00\x00Lady Luck\x00\x00\x00\x00\x00\x00\x00" "Divine Rose\x00\x00\x00\x00\x00Oathkeeper\x00\x00\x00\x00\x00\x00Oblivion\x00\x00\x00\x00\x00\x00\x00\x00Ultima Weapon\x00\x00\x00" "Diamond Dust\x00\x00\x00\x00One-Winged Angel\x00\x00\x00\x00\x00\x00\x00\x00Soul Eater\x00\x00\x00\x00\x00\x00Star Seeker\x00\x00\x00\x00\x00Monochrome\x00\x00\x00\x00\x00\x00" "Follow the Wind\x00Hidden Dragon\x00\x00\x00Photon Debugger\x00" "Bond of Flame\x00\x00\x00-\x00\x00\x00\x00\x00\x00\x00" "Fire\x00\x00\x00\x00" "Blizzard\x00\x00\x00\x00\x00\x00\x00\x00Thunder\x00" "Cure\x00\x00\x00\x00Gravity\x00Stop\x00\x00\x00\x00" "Aero\x00\x00\x00\x00Simba\x00\x00\x00Genie\x00\x00\x00" "Bambi\x00\x00\x00" "Dumbo\x00\x00\x00Tinker Bell\x00\x00\x00\x00\x00Mushu\x00\x00\x00" "Cloud\x00\x00\x00Potion\x00\x00Hi-Potion\x00\x00\x00\x00\x00\x00\x00Mega-Potion\x00\x00\x00\x00\x00" "Ether\x00\x00\x00Mega-Ether\x00\x00\x00\x00\x00\x00" "Elixir\x00\x00Megalixir\x00\x00\x00\x00\x00\x00\x00" "Donald Duck\x00\x00\x00\x00\x00Goofy\x00\x00\x00" "Aladdin\x00" "Ariel\x00\x00\x00Jack\x00\x00\x00\x00Peter Pan\x00\x00\x00\x00\x00\x00\x00The Beast\x00\x00\x00\x00\x00\x00\x00The King\x00\x00\x00\x00\x00\x00\x00\x00Pluto\x00\x00\x00Guard Armor\x00\x00\x00\x00\x00Trickmaster\x00\x00\x00\x00\x00Jafar\x00\x00\x00Oogie Boogie\x00\x00\x00\x00Parasite Cage\x00\x00\x00Ursula\x00\x00" "Dragon Maleficent\x00\x00\x00\x00\x00\x00\x00" "Darkside\x00\x00\x00\x00\x00\x00\x00\x00Shadow\x00\x00Soldier\x00Large Body\x00\x00\x00\x00\x00\x00Red Nocturne\x00\x00\x00\x00" "Blue Rhapsody\x00\x00\x00Yellow Opera\x00\x00\x00\x00Green Requiem\x00\x00\x00Powerwild\x00\x00\x00\x00\x00\x00\x00" "Bouncywild\x00\x00\x00\x00\x00\x00" "Air Soldier\x00\x00\x00\x00\x00" "Bandit\x00\x00" "Fat Bandit\x00\x00\x00\x00\x00\x00" "Barrel Spider\x00\x00\x00Search Ghost\x00\x00\x00\x00Sea Neon\x00\x00\x00\x00\x00\x00\x00\x00Screwdiver\x00\x00\x00\x00\x00\x00" "Aquatank\x00\x00\x00\x00\x00\x00\x00\x00Wight Knight\x00\x00\x00\x00Gargoyle\x00\x00\x00\x00\x00\x00\x00\x00Pirate\x00\x00" "Air Pirate\x00\x00\x00\x00\x00\x00" "Darkball\x00\x00\x00\x00\x00\x00\x00\x00" "Defender\x00\x00\x00\x00\x00\x00\x00\x00Wyvern\x00\x00Wizard\x00\x00Neoshadow\x00\x00\x00\x00\x00\x00\x00White Mushroom\x00\x00" "Black Fungus\x00\x00\x00\x00" "Creeper Plant\x00\x00\x00Tornado Step\x00\x00\x00\x00" "Crescendo\x00\x00\x00\x00\x00\x00\x00" "Card Soldier\x00\x00\x00\x00Hades\x00\x00\x00Hook\x00\x00\x00\x00Riku\x00\x00\x00\x00" "Axel\x00\x00\x00\x00Larxene\x00Vexen\x00\x00\x00Marluxia\x00\x00\x00\x00\x00\x00\x00\x00Lexaeus\x00" "Ansem\x00\x00\x00Zexion\x00\x00Xemnas\x00\x00Xigbar\x00\x00Xaldin\x00\x00Sa\x83\xc7x\x00\x00\x00" "Demyx\x00\x00\x00Luxord\x00\x00Roxas\x00\x00\x00Gold Card\x00\x00\x00\x00\x00\x00\x00Platinum Card\x00\x00\x00" "Card Soldier Spade\x00\x00\x00\x00\x00\x00" "Card Soldier Heart\x00\x00\x00\x00\x00\x00\x83N\x83\x89\x83" "E\x83hA\x00\x00\x00\x00\x00\x00\x00\x83N\x83\x89\x83" "E\x83hB\x00\x00\x00\x00\x00\x00\x00\x83n\x83" "f\x83XA\x00\x83n\x83" "f\x83XB\x00\x83t\x83" "b\x83NA\x00\x83t\x83" "b\x83NB\x00\x83t\x83" "b\x83NC\x00\x83t\x83" "b\x83ND\x00\x83" "A\x83N\x83Z\x83\x8b" "A\x00\x00\x00\x00\x00\x00\x00\x83" "A\x83N\x83Z\x83\x8b" "B\x00\x00\x00\x00\x00\x00\x00\x83\x89\x83N\x83V\x81[\x83kA\x00\x00\x00\x00\x00\x83\x89\x83N\x83V\x81[\x83kB\x00\x00\x00\x00\x00\x83\x89\x83N\x83V\x81[\x83kC\x00\x00\x00\x00\x00\x83\x8a\x83NA\x00\x00\x00\x83\x8a\x83NB\x00\x00\x00\x83\x8a\x83NC\x00\x00\x00\x83\x8a\x83ND\x00\x00\x00\x83\x94\x83" "B\x83N\x83Z\x83\x93" "A\x00\x00\x00\x00\x00\x83\x94\x83" "B\x83N\x83Z\x83\x93" "B\x00\x00\x00\x00\x00\x83}\x81[\x83\x8b\x81[\x83V\x83\x83" "A\x00\x00\x00\x83}\x81[\x83\x8b\x81[\x83V\x83\x83" "B\x00\x00\x00\x83}\x81[\x83\x8b\x81[\x83V\x83\x83" "C\x00\x00\x00\x83}\x81[\x83\x8b\x81[\x83V\x83\x83" "2\x00\x00\x00\x83}\x81[\x83\x8b\x81[\x83V\x83\x83" "3\x00\x00\x00\x83\x8c\x83N\x83Z\x83" "E\x83XA\x00\x00\x00\x00\x00\x83\x8c\x83N\x83Z\x83" "E\x83XB\x00\x00\x00\x00\x00\x83\x8c\x83N\x83Z\x83" "E\x83XC\x00\x00\x00\x00\x00\x83" "A\x83\x93\x83Z\x83\x80" "A\x00\x00\x00\x00\x00\x00\x00\x83" "A\x83\x93\x83Z\x83\x80" "B\x00\x00\x00\x00\x00\x00\x00\x83[\x83N\x83V\x83I\x83\x93" "A\x00\x00\x00\x00\x00\x83\x8a\x83\x8d\x81[\x83h\x00\x00\x00\x00\x00\x00\x00\x00Tranquil Darkness\x00\x00\x00\x00\x00\x00\x00Teeming Darkness\x00\x00\x00\x00\x00\x00\x00\x00" "Feeble Darkness\x00" "Almighty Darkness\x00\x00\x00\x00\x00\x00\x00Sleeping Darkness\x00\x00\x00\x00\x00\x00\x00Looming Darkness\x00\x00\x00\x00\x00\x00\x00\x00Premium Room\x00\x00\x00\x00White Room\x00\x00\x00\x00\x00\x00" "Black Room\x00\x00\x00\x00\x00\x00" "Bottomless Darkness\x00\x00\x00\x00\x00Roulette Room\x00\x00\x00Martial Waking\x00\x00Sorcerous Waking\x00\x00\x00\x00\x00\x00\x00\x00" "Alchemic Waking\x00Meeting Ground\x00\x00Stagnant Space\x00\x00Strong Initiative\x00\x00\x00\x00\x00\x00\x00Lasting Daze\x00\x00\x00\x00" "Calm Bounty\x00\x00\x00\x00\x00Guarded Trove\x00\x00\x00" "False Bounty\x00\x00\x00\x00Moment's Reprieve\x00\x00\x00\x00\x00\x00\x00Mingling Worlds\x00Moogle Room\x00\x00\x00\x00\x00Random Joker\x00\x00\x00\x00Key of Beginnings\x00\x00\x00\x00\x00\x00\x00Key of Guidance\x00Key to Truth\x00\x00\x00\x00Key to Rewards\x00\x00Traverse Town\x00\x00\x00" "Agrabah\x00Olympus Coliseum\x00\x00\x00\x00\x00\x00\x00\x00Wonderland\x00\x00\x00\x00\x00\x00Monstro\x00Halloween Town\x00\x00" "Atlantica\x00\x00\x00\x00\x00\x00\x00Neverland\x00\x00\x00\x00\x00\x00\x00Hollow Bastion\x00\x00" "100 Acre Woods\x00\x00Twilight Town\x00\x00\x00" "Destiny Islands\x00" "Castle Oblivion\x00" "EXP\x90\xc2\x8f\xac\x00" "EXP\x90\xc2\x92\x86\x00" "EXP\x90\xc2\x91\xe5\x00" "EXP\x90\xd4\x8f\xac\x00" "EXP\x90\xd4\x92\x86\x00" "EXP\x90\xd4\x91\xe5\x00" "EXP\x8b\xe2\x00\x00\x00HP\x8f\xac\x00\x00\x00\x00HP\x91\xe5\x00\x00\x00\x00MP\x8f\xac\x00\x00\x00\x00MP\x91\xe5\x00\x00\x00\x00P\x00\x00\x00\x00\x00\x00\x00R\x00\x00\x00\x00\x00\x00\x00Unknown Room\x00\x00\x00\x00" "Conqueror's Respite\x00\x00\x00\x00\x00Room of Rewards\x00\x00\x00\x00\x00" "0\xc6H\x00\x11\x02\x00\x03P1I\x00\x11\x02\x01\x03`1I\x00\x11\x02\x02\x03p1I\x00\x11\x02\x03\x03\x80" "1I\x00\x11\x02\x04\x03\x90" "1I\x00\x11\x02\x05\x03\xa0" "1I\x00\x11\x02\x06\x03\xb0" "1I\x00\x11\x02\x07\x03\xc0" "1I\x00\x11\x03\x00\x03\xd0" "1I\x00\x11\x03\x01\x03\xd8" "1I\x00\x11\x03\x02\x03\xe8" "1I\x00\x11\x03\x03\x03\xf8" "1I\x00\x11\x03\x04\x03\x08" "2I\x00\x11\x03\x05\x03\x18" "2I\x00\x11\x03\x06\x03(2I\x00\x11\x03\x07\x03" "82I\x00\x11\x04\x00\x03H2I\x00\x11\x04\x01\x04`2I\x00\x11\x04\x02\x03p2I\x00\x11\x04\x03\x03\x80" "2I\x00\x11\x04\x04\x03\x90" "2I\x00\x11\x04\x05\x03\xa0" "2I\x00\x11\x04\x06\x03\xb0" "2I\x00\x11\x04\x07\x03\xc0" "2I\x00\x11\x1a\x00\x00\xd0" "2I\x00\"\x05\x00\x03\xd8" "2I\x00\"\x05\x01\x03\xe0" "2I\x00\"\x05\x02\x03\xf0" "2I\x00\"\x05\x03\x03\xf8" "2I\x00\"\x05\x04\x03\x00" "3I\x00\"\x05\x05\x03\x08" "3I\x00\"\x05\x06\x03\x10" "3I\x00\"\x1a\x02\x00\xd0" "2I\x00\"\x1a\x01\x00\xd0" "2I\x00" "3\x05\x07\x03\x18" "3I\x00" "3\x06\x00\x03 3I\x00" "3\x06\x01\x03(3I\x00" "3\x06\x02\x03" "03I\x00" "3\x06\x03\x03" "83I\x00" "3\x06\x04\x03H3I\x00" "3\x06\x05\x03P3I\x00" "D\x06\x06\x06X3I\x00" "D\x06\x07\x06`3I\x00" "D\x07\x00\x02p3I\x00" "D\x07\x01\x02\x80" "3I\x00" "D\x07\x02\x02\x88" "3I\x00" "D\x07\x03\x02\x98" "3I\x00" "D\x07\x04\x02\xa0" "3I\x00U\x07\x05\x02\xb0" "3I\x00U\x07\x06\x02\xc0" "3I\x00U\x07\x07\x02\xc8" "3I\x00U\x08\x00\x02\xd0" "3I\x00U\x08\x01\x02\xd8" "3I\x00U\x08\x02\x02\xe0" "3I\x00U\x08\x03\x02\xf0" "3I\x00U\x08\x04\x04\x00" "4I\x00U\x08\x05\x02\x10" "4I\x00" "f\x08\x06\x06\x18" "4I\x00" "f\x08\x06\x06(4I\x00" "f\x08\x06\x06" "84I\x00" "f\x08\x06\x06@4I\x00" "f\x08\x06\x06P4I\x00" "f\x08\x06\x06`4I\x00" "f\x08\x06\x06h4I\x00" "f\x08\x06\x06\x80" "4I\x00\xe6\x08\x07\x04\xd0" "2I\x00\xe6\t\x00\x00\xd0" "2I\x00w\t\x01\x06\x90" "4I\x00w\t\x02\x02\x98" "4I\x00w\t\x03\x06\xa0" "4I\x00w\t\x04\x02\xb0" "4I\x00w\t\x05\x02\xc0" "4I\x00w\t\x06\x02\xd0" "4I\x00w\t\x07\x02\xe0" "4I\x00w\n\x00\x06\xf0" "4I\x00w\n\x01\x02\x00" "5I\x00w\n\x02\x02\x10" "5I\x00w\n\x03\x02 5I\x00w\n\x04\x06(5I\x00w\n\x05\x02" "85I\x00w\n\x06\x06H5I\x00w\n\x07\x06X5I\x00w\x0b\x00\x02h5I\x00w\x0b\x01\x02x5I\x00w\x0b\x02\x06\x88" "5I\x00w\x0b\x03\x02\x98" "5I\x00w\x0b\x04\x06\xa8" "5I\x00w\x0b\x05\x02\xb0" "5I\x00w\x0b\x06\x04\xc0" "5I\x00w\x0b\x07\x06\xd0" "5I\x00w\x0c\x00\x02\xe0" "5I\x00w\x0c\x01\x02\xe8" "5I\x00w\x0c\x02\x02\xf0" "5I\x00w\x0c\x03\x02\x00" "6I\x00w\x0c\x04\x02\x10" "6I\x00w\x0c\x05\x02 6I\x00w\x0c\x06\x02" "06I\x00w\x0c\x07\x02@6I\x00w\x0d\x00\x06\x18" "4I\x00w\x0d\x01\x06P4I\x00w\x0d\x02\x06(4I\x00w\x0d\x03\x06\x80" "4I\x00w\x0d\x04\x02P6I\x00w\x0d\x05\x06`6I\x00w\x0d\x06\x06" "84I\x00w\x0d\x07\x06@4I\x00w\x0e\x00\x06`4I\x00w\x0e\x01\x06h6I\x00w\x0e\x02\x06h4I\x00w\x0e\x03\x02p6I\x00w\x0e\x04\x02x6I\x00w\x0e\x05\x02\x80" "6I\x00w\x0e\x06\x02\x88" "6I\x00w\x0e\x07\x02\x90" "6I\x00w\x0f\x00\x06\xa0" "6I\x00w\x0f\x01\x02\xa8" "6I\x00w\x0f\x03\x06\xb0" "6I\x00w\x0f\x04\x02\xb8" "6I\x00w\x0f\x05\x02\xc0" "6I\x00w\x0f\x06\x02\xc8" "6I\x00w\x0f\x07\x02\xd0" "6I\x00w\x10\x00\x02\xd8" "6I\x00w\x10\x01\x02\xe0" "6I\x00w\x10\x02\x02\xe8" "6I\x00w\x15\x06\x02\xf0" "6I\x00w\x15\x07\x02\x00" "7I\x00w\x0f\x02\x00\x10" "7I\x00w\x0d\x04\x00(7I\x00\x17\x16\x00\x00@7I\x00\x17\x16\x01\x00P7I\x00\x17\x16\x02\x00`7I\x00'\x16\x03\x00h7I\x00\x17\x16\x04\x00p7I\x00\x17\x16\x05\x00x7I\x00\x17\x16\x06\x00\x80" "7I\x00'\x16\x07\x00\x88" "7I\x00\x17\x17\x00\x00\x90" "7I\x00\x17\x17\x01\x00\xa0" "7I\x00\x17\x17\x02\x00\xb0" "7I\x00\x17\x17\x03\x00\xc0" "7I\x00'\x17\x04\x00\xd0" "7I\x00\x17\x17\x05\x00\xe0" "7I\x00\x17\x17\x06\x00\xe8" "7I\x00\x17\x17\x07\x00\xf0" "7I\x00\x17\x18\x00\x00\xf8" "7I\x00\x17\x18\x01\x00\x00" "8I\x00'\x18\x02\x00\x10" "8I\x00\x17\x18\x03\x00 8I\x00\x17\x18\x04\x00" "08I\x00'\x18\x05\x00@8I\x00w\x18\x06\x00P8I\x00w\x18\x07\x00`8I\x00\x17\x19\x00\x00p8I\x00\x17\x19\x01\x00\x80" "8I\x00'\x19\x02\x00\x90" "8I\x00\x17\x19\x03\x00\xa0" "8I\x00'\x19\x04\x00\xb0" "8I\x00'\x19\x05\x00\xc0" "8I\x00w\x1a\x03\x00\xd0" "2I\x00\x88\x00\x00\x06\xd0" "8I\x00\x99\x10\x03\x06\xe0" "8I\x00\x99\x10\x04\x06\xf8" "8I\x00\x99\x10\x05\x06\x10" "9I\x00\x99\x10\x06\x06 9I\x00\x99\x10\x07\x06" "89I\x00\x99\x11\x00\x06P9I\x00\x99\x11\x01\x02h9I\x00\x99\x11\x02\x02x9I\x00\x99\x11\x03\x06\x88" "9I\x00\x99\x11\x04\x06\x98" "9I\x00\x99\x11\x05\x06\xb0" "9I\x00\xa9\x11\x06\x06\xc0" "9I\x00\xa9\x11\x07\x02\xd0" "9I\x00\xa9\x12\x00\x02\xe8" "9I\x00\xa9\x12\x01\x06\xf8" "9I\x00\xa9\x12\x02\x06\x08:I\x00\xa9\x12\x03\x06\x18:I\x00\xa9\x12\x04\x06" "0:I\x00\xb9\x12\x05\x02@:I\x00\xb9\x12\x06\x02P:I\x00\xb9\x12\x07\x02`:I\x00\xb9\x13\x00\x06p:I\x00\xb9\x13\x01\x06\x88:I\x00\xb9\x13\x02\x02\x98:I\x00\xb9\x13\x03\x06\xa8:I\x00\xca\x13\x04\x06\xb8:I\x00\xca\x13\x05\x06\xd0:I\x00\xca\x13\x06\x06\xe0:I\x00\xca\x13\x07\x02\xf0:I\x00\xdb\x14\x00\x06\x00;I\x00\xdb\x14\x03\x06\x10;I\x00\xdb\x14\x02\x06\x18;I\x00\xdb\x14\x01\x06" "0;I\x00\xdb\x14\x05\x06@;I\x00\xdb\x14\x04\x06H;I\x00\xdb\x14\x06\x06X;I\x00\xdb\x14\x07\x06h;I\x00\xdb\x15\x00\x06x;I\x00\xdb\x15\x01\x02\x88;I\x00\xdb\x15\x02\x06\x98;I\x00\xdb\x15\x03\x06\xa8;I\x00\xdb\x15\x04\x04\xb8;I\x00\x0d\x00\x00\x00\xc8;I\x00\x0d\x00\x01\x00\xd0;I\x00\x0d\x00\x02\x00\xd8;I\x00\x0d\x00\x03\x00\xe0;I\x00\x0d\x00\x04\x00\xe8;I\x00\x0d\x00\x05\x00\xf0;I\x00\x0d\x00\x06\x00\xf8;I\x00\x0d\x01\x00\x00\x00<I\x00\x0d\x01\x01\x00\x08<I\x00\x0d\x02\x00\x00\x10<I\x00\x0d\x02\x01\x00\x18<I\x00\x0d\x03\x00\x00 <I\x00\x0d\x04\x00\x00(<I\x00\x0c\x00\x00\x00" "0<I\x00\x0c\x00\x00\x00@<I\x00\x0c\x00\x00\x00X<I\x00\x0c\x00\x00\x00" "0\xc6H\x00\x0c\x00\x00\x00" "0\xc6H\x00\x00\x00\x00\x00" "0\xc6H\x00\x00\x00\x00\x00" "0\xc6H\x00\x00\x00\x00\x00" "0\xc6H\x00\x00\x00\x00\x00\x00\x00\x00"
};
const void* _roTailKeep = &_roTail; /* keeps the deferred table (this .data is not from source) */
__asm__(".globl _vt$Q24ATEL4CVSM"); /* other objects' data refer to CVSM's vtable */

// SOURCE DATA: .rodata
