// PARTIAL: added for Game/Enemy/MapEnemyManager
#ifndef GAME_OSDDEF_H
#define GAME_OSDDEF_H

#include <string.h>

#include "khtypes.h"
#include "System/Debug.h"
#include "System/MainApp.h"

enum OSD_CATEGORY {
    OSD_CATEGORY_PLAYER = 0,
    OSD_CATEGORY_ENEMY = 1,
    OSD_CATEGORY_GIMMICK = 2,
    OSD_CATEGORY_NPC = 3,
    OSD_CATEGORY_MAX = 4
};

struct OBJ_INFO {
    FLOAT x;
    FLOAT y;
    FLOAT z;
    FLOAT height;
    FLOAT dir;
    SHORT sGroupID;
    SHORT sID;
    UINT uiFlag;
    SHORT sFormation1;
    SHORT sFormation2;
    INT iParam;
    INT iUniqueID;
    FLOAT fRotX;
    FLOAT fRotZ;

    void setFlag(UINT inFlag, BOOL inOn) {
        if (inOn) uiFlag |= inFlag;
        else uiFlag &= ~inFlag;
    }
};

struct OBJ_GROUP {
    INT iGroupNo;
    INT iObjCount;
    OBJ_INFO* pObjInfo;
    INT iNextGroupNum;

    void clear() {
        for (INT i = 0; i < iObjCount; i++) {
            pObjInfo[i].setFlag(1, FALSE);
        }
    }
};

struct OBJ_FORMATION {
    OBJ_GROUP* pGroup[OSD_CATEGORY_MAX][16];

    OBJ_GROUP* getObjGroup(INT inCategory, INT inGroupNo);
    OBJ_GROUP* getEnemyGroup(INT inGroupNo) { return getObjGroup(OSD_CATEGORY_ENEMY, inGroupNo); }
    OBJ_GROUP* getGimmickGroup(INT inGroupNo) { return getObjGroup(OSD_CATEGORY_GIMMICK, inGroupNo); } // PARTIAL: added for Game/Gimmick/GimmickManager
    void addrMapping(void* inBase) {
        for (UINT i = 0; i < OSD_CATEGORY_MAX; i++) {
            for (UINT j = 0; j < 16; j++) {
                if (pGroup[i][j]) {
                    pGroup[i][j] = (OBJ_GROUP*)((char*)inBase + (INT)pGroup[i][j]);
                    pGroup[i][j]->pObjInfo = (OBJ_INFO*)((char*)inBase + (INT)pGroup[i][j]->pObjInfo);
                }
            }
        }
    }
    void clear() {
        for (UINT i = 0; i < OSD_CATEGORY_MAX; i++) {
            for (UINT j = 0; j < 16; j++) {
                if (pGroup[i][j]) {
                    pGroup[i][j]->clear();
                }
            }
        }
    }
};

#line 126 "Game/OsdDef.h"
inline OBJ_GROUP* OBJ_FORMATION::getObjGroup(INT inCategory, INT inGroupNo) {
    if (!(0 <= inCategory && inCategory < OSD_CATEGORY_MAX)) {
        AssertApp(__FILE__, __LINE__, "0 <= inCategory && inCategory < OSD_CATEGORY_MAX", 0);
    }
    if (0 <= inGroupNo && inGroupNo < 16) {
        return pGroup[inCategory][inGroupNo];
    }
    return 0;
}

struct OSD_HEADER {
    char szCode[4];
    UINT uiVersion;
    OBJ_FORMATION* pFormation[32];

    BOOL addrMapping() {
        if (uiVersion & 0x80000000) {
            clear();
            return TRUE;
        }
        if (strncmp(szCode, "@OSD", 4) != 0) {
            return FALSE;
        }
        if ((uiVersion & 0x7FFFFFFF) != 4) {
            TRACE("****************** ESD\x82\xcc\x83o\x81[\x83W\x83\x87\x83\x93\x82\xaa\x8c\xc3\x82\xa2\x82\xc5\x82\xb7 ******************\n");
            TRACE("****************** Version = %d ******************\n", uiVersion & 0x7FFFFFFF);
            return FALSE;
        }
        for (UINT i = 0; i < 32; i++) {
            if (pFormation[i]) {
                pFormation[i] = (OBJ_FORMATION*)((char*)this + (INT)pFormation[i]);
                pFormation[i]->addrMapping(this);
            }
        }
        uiVersion |= 0x80000000;
        return TRUE;
    }
    void clear() {
        for (UINT i = 0; i < 32; i++) {
            if (pFormation[i]) {
                pFormation[i]->clear();
            }
        }
    }
};

struct EVENT_BOX_DATA {
    FLOAT x;
    FLOAT y;
    FLOAT z;
    FLOAT width;
    FLOAT height;
    FLOAT depth;
    FLOAT dir;
    INT param;
};

struct EVB_HEADER {
    char szCode[4];
    UINT uiVersion;
    INT iNum;
    EVENT_BOX_DATA* pData;

    BOOL addrMapping() {
        if (uiVersion & 0x80000000) {
            return TRUE;
        }
        if (strncmp(szCode, "@EVB", 4) != 0) {
            return FALSE;
        }
        if ((uiVersion & 0x7FFFFFFF) != 1) {
            TRACE("****************** EVB\x82\xcc\x83o\x81[\x83W\x83\x87\x83\x93\x82\xaa\x8c\xc3\x82\xa2\x82\xc5\x82\xb7 ******************\n");
            return FALSE;
        }
        uiVersion |= 0x80000000;
        pData = (EVENT_BOX_DATA*)((char*)this + (INT)pData);
        return TRUE;
    }
};

#endif /* GAME_OSDDEF_H */
