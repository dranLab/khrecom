#ifndef GAME_TECHNIC_H
#define GAME_TECHNIC_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"

struct TECHNIC_INFO {
    unsigned char _technic[100];
};

#ifndef TECHNIC_H_PATH // MATCH: path override for Game/Status ("c:/KHCOM/MainApp/...")
#define TECHNIC_H_PATH "Game/Technic.h"
#endif
#line 1 TECHNIC_H_PATH
/* MATCH: operator new for CTechnic::initialize's allocation, declared only: the includers define it themselves */
inline void* operator new(size_t size, char* msg) throw();

class CTechnic {
private:
    static CTechnic* m_pInstance;
    static TECHNIC_INFO* m_pTechnicInfo;

    CTechnic();
    ~CTechnic();

public:
#line 36
    static CTechnic& initialize() {
        if (!(__null == m_pInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
        m_pInstance = new (__FILE__ "(38)") CTechnic;
        return *m_pInstance;
    }
    static void finalize() {
        if (m_pInstance) delete m_pInstance;
        m_pInstance = __null;
    }
    static void clearAll();
    static void setAll();
    static BOOL addTechnic(USHORT inKind);
    static BOOL canUse(USHORT inKind);
    static BOOL isNew(USHORT inKind);
    static BOOL setNewFinish(USHORT inKind);
    static BOOL isValid(USHORT inKind);
    static BOOL isSoraValid(USHORT inKind);
    static BOOL isRikuValid(USHORT inKind);
    static LPCTSTR getName(INT nTechKind);

private:
    static INT getAppearance(USHORT inKind);
    static void setAppearance(USHORT inKind, INT nAppear);
};

#endif /* GAME_TECHNIC_H */
