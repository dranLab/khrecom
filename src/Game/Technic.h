// PARTIAL: added for Form/Menu/TechHelp
#ifndef GAME_TECHNIC_H
#define GAME_TECHNIC_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"

struct TECHNIC_INFO {
    unsigned char _technic[100];
};

/* Line numbers are the original ones (__LINE__ in messages); only line 38 (allocation) is known. */
#ifndef TECHNIC_H_PATH // PARTIAL: path override added for Game/Status ("c:/KHCOM/MainApp/...")
#define TECHNIC_H_PATH "Game/Technic.h"
#endif
#line 20 TECHNIC_H_PATH
class CTechnic {
private:
    friend struct MainAppSingletons; // PARTIAL: System/MainApp creates and deletes the instance
    static CTechnic* m_pInstance;
    static TECHNIC_INFO* m_pTechnicInfo;

    CTechnic();
    ~CTechnic();

public:
    static CTechnic& initialize() {
        UINT nRA = 0;
#line 37
        if (m_pInstance != NULL) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
        m_pInstance = (CTechnic*)com_malloc_d(sizeof(CTechnic), __FILE__ "(38)", nRA);
        return *m_pInstance;
    }
    static void finalize();
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
