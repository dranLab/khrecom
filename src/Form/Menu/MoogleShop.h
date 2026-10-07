#ifndef FORM_MENU_MOOGLESHOP_H
#define FORM_MENU_MOOGLESHOP_H

#include "khtypes.h"
#include "Game/StorageWork.h"

class CMoogleShopMain {
private:
    static HANDLE g_hThis;

    CMoogleShopMain();
    virtual ~CMoogleShopMain();

public:
    static void initMcWork(STORAGE::MCWork::_RoomInfo* pInfo);
    static UINT getMcWorkFlag();
    static void setMcWorkFlag(UINT unFlag);
    static void finalize();
};

#endif // FORM_MENU_MOOGLESHOP_H
