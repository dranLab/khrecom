#ifndef GAME_STORAGEWORK_H
#define GAME_STORAGEWORK_H

#include "khtypes.h"

/* Save data blocks (memory card work). Member types owned by other units
 * (_RoomInfo, _DoorInfo, CARD_INFO, ...) are opaque blocks of the original size. */
namespace STORAGE {

struct JIMINYMEMO_FLAGWORK { // 0x230
    unsigned char ViewState[400];
    unsigned short DownEnemyNum[70];
    unsigned int MinigameScore[5];
};

struct _EventDoorInfo { // 0x4
    unsigned char _nNormaNums[3];
    BYTE _nClearedNorma;
};

struct MCWork { // 0x2ad4
    // PARTIAL: added for Form/Menu/MoogleShop
    struct _RoomInfo { // 0xb8
        /* 0x00 */ UINT __opaque[0xb4 / 4]; // _nArea .. _mapEnemyInfo
        /* 0xb4 */ UINT _moogleShopFlag;
    };

    /* 0x0000 */ BYTE _nFloorIdx;
    /* 0x0001 */ BYTE _nRoomID;
    /* 0x0002 */ BYTE _nDoorID;
    /* 0x0003 */ BYTE _nNextRCNormaNum;
    /* 0x0004 */ BYTE _nPrevRoomID;
    /* 0x0005 */ unsigned char __nFiller[3];
    /* 0x0008 */ _RoomInfo _roomInfo[32];
    /* 0x1708 */ _RoomInfo _roomInfoStartEnt;
    /* 0x17c0 */ _RoomInfo _roomInfoGoalEnt;
    /* 0x1878 */ SHORT _nArea;
    /* 0x187a */ SHORT _nScene;
    /* 0x187c */ SHORT _nPrevArea;
    /* 0x187e */ SHORT _nPrevScene;
    /* 0x1880 */ short _GoalRoomSceneTbl[16];
    /* 0x18a0 */ unsigned char _KeyRoomsTresureBoxCounter[16];
    /* 0x18b0 */ unsigned char _KeyRoomsTresureBox[16];
    /* 0x18c0 */ BYTE _KeyRoomsCreateState;
    /* 0x18c1 */ unsigned char _OpenTresureBoxCounter[16];
    /* 0x18d1 */ _EventDoorInfo _nEventDoorInfoTbl[16][4];
    /* 0x19d1 */ unsigned char _nEventDoorStateTbl[16][6];
    /* 0x1a32 */ short _nEntranceSceneTbl[16][2];
    /* 0x1a74 */ UINT _cardInfo[0xda4 / 4];        // CARD_INFO
    /* 0x2818 */ INT _statusInfo[10];              // STATUS_INFO
    /* 0x2840 */ unsigned char _technicInfo[100];  // TECHNIC_INFO
    /* 0x28a4 */ JIMINYMEMO_FLAGWORK _JiminyMemoFlagWork;

    void initialize();
    void preSave();
    void postLoad();
};

struct _RoomInfo2 { // 0x2c
    unsigned char _doors[4][0xa]; // _DoorInfo
    BYTE _nCreatedCardNum;
    BYTE _nCreatedCardID;
    unsigned char __nFiller[2];
};

struct MCWork2 { // 0x62e
    /* 0x000 */ _RoomInfo2 _roomInfo2[32];
    /* 0x580 */ _RoomInfo2 _roomInfo2StartEnt;
    /* 0x5ac */ _RoomInfo2 _roomInfo2GoalEnt;
    /* 0x5d8 */ unsigned char _nEventDoorOpenTbl[16][3];
    /* 0x608 */ unsigned char _nWorldOnFloor[2][16];
    /* 0x628 */ USHORT _cardInfo2_nKeyTotal; // CARD_INFO2
    /* 0x62a */ char _cardInfo2_nKey[4];

    void initialize();
    void preSave();
    void postLoad();
};

} // namespace STORAGE

#endif // GAME_STORAGEWORK_H
