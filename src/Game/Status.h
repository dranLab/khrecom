#ifndef GAME_STATUS_H
#define GAME_STATUS_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"

#ifndef STATUS_H_PATH // MATCH: path override for Game/Status ("c:/KHCOM/MainApp/...")
#define STATUS_H_PATH "Game/Status.h"
#endif
#line 1 STATUS_H_PATH
/* MATCH: operator new for CStatus::initialize's allocation, declared only: the includers define it themselves */
inline void* operator new(size_t size, char* msg) throw();

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

typedef STATUS_INFO STATUS;

class CStatus {
private:
    static CStatus* m_pInstance;
    static STATUS_INFO* m_pStatusInfo;

    CStatus();
    ~CStatus();

public:
#line 36
    static CStatus& initialize() {
        KH_DEBUG(if (!(__null == m_pInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);)
        m_pInstance = new KH_AT("(38)") CStatus;
        return *m_pInstance;
    }
    static void finalize() {
        if (m_pInstance) delete m_pInstance;
        m_pInstance = __null;
    }
    static STATUS& getStaus() { return *m_pStatusInfo; }
    /* MATCH: the accessors read m_pStatusInfo through a reference: the original reloads it after every store into it */
    static INT getHP() { STATUS_INFO*& p = m_pStatusInfo; return p->HP; }
    static void setHP(INT n) { STATUS_INFO*& p = m_pStatusInfo; p->HP = n; }
    static INT getHPMax() { STATUS_INFO*& p = m_pStatusInfo; return p->HPMax; }
    static void setHPMax(INT n) { STATUS_INFO*& p = m_pStatusInfo; p->HPMax = n; }
    static INT getCP(/* parameters unknown */);
    static void setCP(/* parameters unknown */);
    static INT getCPMax() { STATUS_INFO*& p = m_pStatusInfo; return p->CPMax; }
    static void setCPMax(INT n) { STATUS_INFO*& p = m_pStatusInfo; p->CPMax = n; }
    static INT getAP() { STATUS_INFO*& p = m_pStatusInfo; return p->AP; }
    static void setAP(INT n) { STATUS_INFO*& p = m_pStatusInfo; p->AP = n; }
    static INT getDPMax() { STATUS_INFO*& p = m_pStatusInfo; return p->DPMax; }
    static void setDPMax(INT n) { STATUS_INFO*& p = m_pStatusInfo; p->DPMax = n; }
    static INT getEXP() { STATUS_INFO*& p = m_pStatusInfo; return p->EXP; }
    static void setEXP(INT inEXP) { STATUS_INFO*& p = m_pStatusInfo; p->EXP = inEXP; }
    static INT getLV() { STATUS_INFO*& p = m_pStatusInfo; return p->LV; }
    static void setLV(INT n) { STATUS_INFO*& p = m_pStatusInfo; p->LV = n; }
    static INT getMP() { STATUS_INFO*& p = m_pStatusInfo; return p->MP; }
    static BOOL setMP(INT inMP);
    static BOOL addMP(INT inMP);
    static INT getPlayTime(/* parameters unknown */);
    static void setPlayTime(/* parameters unknown */);
    static INT getLvExp(UINT nLv);
    static BOOL isLevelUp();
#ifdef GAME_SCENEINFO_H /* MATCH: only for includers that have SceneInfo.h (WORLD_ID) */
    static void initStatus(USHORT nDeck, WORLD_ID nWorld, SHORT nFloor);
#endif

private:
    static void setStatus(unsigned char (*pInitCardTbl)[2]);
};

#endif /* GAME_STATUS_H */
