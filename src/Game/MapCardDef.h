#ifndef GAME_MAPCARDDEF_H
#define GAME_MAPCARDDEF_H

#include "khtypes.h"

struct MAP_CARD_PARAM {
    INT iFormation1;
    INT iFormation2;
    INT iEnemyNum;
    INT pad;
};

struct MAP_CARD_PARAM_HEADER {
    char szCode[4];
    UINT uiVersion;
    INT iParamNum;
    MAP_CARD_PARAM* pParam;

    BOOL addrMapping();
    MAP_CARD_PARAM* getMapCardParam(BYTE inCardID);
};

#endif /* GAME_MAPCARDDEF_H */
