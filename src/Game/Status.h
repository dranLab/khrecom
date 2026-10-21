// PARTIAL: added for Form/Menu/MonstroGauge
#ifndef GAME_STATUS_H
#define GAME_STATUS_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"

/* Placeholder for CStatus's inline singleton creation: its strings ("Game/Status.h",
 * "__null == m_pInstance", "Game/Status.h(38)") are in .rodata of every includer.
 * Only line 38 is known (it is part of the allocation message). */
#ifndef STATUS_H_PATH // PARTIAL: path override added for Game/Status ("c:/KHCOM/MainApp/...")
#define STATUS_H_PATH "Game/Status.h"
#endif
#line 1 STATUS_H_PATH
inline void* _statusCreate(void* inInstance) {
    if (!(__null == inInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
    return com_malloc_d(4, __FILE__ "(38)", (UINT)-1);
}

/* PARTIAL: added for Form/Menu/LevelUp - accessors inline, as the callers read m_pStatusInfo directly. */
struct STATUS_INFO {
    INT HP;       /* 0x00 */
    INT HPMax;    /* 0x04 */
    INT CP;       /* 0x08 */
    INT CPMax;    /* 0x0c */
    INT AP;       /* 0x10 */
    INT DPMax;    /* 0x14 */
    INT EXP;      /* 0x18 */
    INT LV;       /* 0x1c */
    INT MP;       /* 0x20 */
    INT playTime; /* 0x24 */
};

class CStatus {
private:
    friend struct MainAppSingletons; // PARTIAL: System/MainApp creates and deletes the instance
    friend class CCard; // PARTIAL: added for Game/Card
    static CStatus* m_pInstance;
    static STATUS_INFO* m_pStatusInfo;
    /* read through a reference: the original reloads m_pStatusInfo after every store into it */
    static STATUS_INFO*& _info() { return m_pStatusInfo; }
    CStatus(); // PARTIAL: added for Game/Status
    ~CStatus();
    static void setStatus(unsigned char (*pInitCardTbl)[2]);

public:
    static STATUS_INFO& getStaus() { return *m_pStatusInfo; } // PARTIAL: added for Game/Player/Player (name as in the original; plain read, no reload)
    static INT getHP() { return _info()->HP; }
    static void setHP(INT n) { _info()->HP = n; }
    static INT getHPMax() { return _info()->HPMax; }
    static void setHPMax(INT n) { _info()->HPMax = n; }
    static INT getCPMax() { return _info()->CPMax; }
    static void setCPMax(INT n) { _info()->CPMax = n; }
    static INT getAP() { return _info()->AP; }
    static void setAP(INT n) { _info()->AP = n; }
    static INT getDPMax() { return _info()->DPMax; }
    static void setDPMax(INT n) { _info()->DPMax = n; }
    static INT getEXP() { return _info()->EXP; }
    static INT getLV() { return _info()->LV; }
    static void setLV(INT n) { _info()->LV = n; }
    static INT getMP() { return _info()->MP; } // PARTIAL: added for Form/Menu/Camp
    static INT getLvExp(UINT nLv);
    static void setEXP(INT inEXP) { _info()->EXP = inEXP; } // PARTIAL: added for Form/DebugWindowTBL
    static BOOL setMP(INT inMP);                             // PARTIAL: added for Form/DebugWindowTBL
    static BOOL addMP(INT inMP);
    static BOOL isLevelUp();
#ifdef GAME_SCENEINFO_H
    static void initStatus(USHORT nDeck, WORLD_ID nWorld, SHORT nFloor); // PARTIAL: added for Form/DebugMenu
#endif
};

#endif /* GAME_STATUS_H */
