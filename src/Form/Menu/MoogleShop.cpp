#include "Form/Menu/MoogleShop.h"
#include "System/Task.h"
#include "System/TaskMan.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Debug.h"
#include "Game/SceneInfo.h"
#include "Game/GameWork.h"

HANDLE CMoogleShopMain::g_hThis = 0;

CMoogleShopMain::CMoogleShopMain() {}

CMoogleShopMain::~CMoogleShopMain() {}

#line 48
void CMoogleShopMain::initMcWork(STORAGE::MCWork::_RoomInfo* pInfo) {
    if (!pInfo) AssertApp(__FILE__, __LINE__, "pInfo", 0);
    pInfo->_moogleShopFlag = 0;
}

UINT CMoogleShopMain::getMcWorkFlag() {
    INT nRoomID;
    INT nIdx;

    nRoomID = CSceneInfo::getCurrentRoomID();
    if (nRoomID >= 0) {
        nIdx = CSceneInfo::getRoomIndex(nRoomID);
        if (nIdx >= 0) {
            return CGameWork::progMCW()._roomInfo[nIdx]._moogleShopFlag;
        }
    }
    return 0x10000;
}

void CMoogleShopMain::setMcWorkFlag(UINT unFlag) {
    INT nRoomID;
    INT nIdx;

    nRoomID = CSceneInfo::getCurrentRoomID();
    if (nRoomID >= 0) {
        nIdx = CSceneInfo::getRoomIndex(nRoomID);
        if (nIdx >= 0) {
            CGameWork::progMCW()._roomInfo[nIdx]._moogleShopFlag = unFlag;
        }
    }
}

void CMoogleShopMain::finalize() {
    CTask* pTask;

    if (g_hThis) {
        pTask = CTaskMan::getTask(g_hThis);
        if (pTask) {
            delete pTask;
        } else {
            TRACE("CMoogleShopMain::finalize Warning!!\n");
            g_hThis = 0;
        }
    }
}
// SOURCE DATA: .data .rodata
