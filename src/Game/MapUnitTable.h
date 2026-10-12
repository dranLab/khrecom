#ifndef GAME_MAPUNITTABLE_H
#define GAME_MAPUNITTABLE_H

#include "khtypes.h"

class CTask;
struct ZONEPARTS;

union MAPUNITID {
    UINT ui;
    char c[4];
    /* PARTIAL: added for Game/Gimmick/SampleGimmick - the {"xxxx", NewClass<T>, kind} table initializers
     * construct the id in place (memset of the table, then the id word loaded from the string literal) */
    MAPUNITID(const char* inID) { ui = *(UINT*)inID; }
};

struct MAPUNITTABLE {
    MAPUNITID id;
    CTask* (*maptable)(CTask*, const ZONEPARTS&); // const: NewClass<T>(CTask*, const ZONEPARTS&) (Game/Gimmick/SampleGimmick)
    INT kind;
};

extern MAPUNITTABLE* mapUnitTable_G[1];
extern MAPUNITTABLE* mapUnitTable_0[10];
extern MAPUNITTABLE* mapUnitTable_1[13];
extern MAPUNITTABLE* mapUnitTable_2[13];
extern MAPUNITTABLE* mapUnitTable_3[13];
extern MAPUNITTABLE* mapUnitTable_4[13];
extern MAPUNITTABLE* mapUnitTable_5[13];
extern MAPUNITTABLE* mapUnitTable_6[13];
extern MAPUNITTABLE* mapUnitTable_7[13];
extern MAPUNITTABLE* mapUnitTable_8[13];
extern MAPUNITTABLE* mapUnitTable_9[13];
extern MAPUNITTABLE* mapUnitTable_10[18];
extern MAPUNITTABLE* mapUnitTable_11[13];
extern MAPUNITTABLE* mapUnitTable_12[13];
extern MAPUNITTABLE* mapUnitTable_13[13];

MAPUNITTABLE* mapUnitTableSearch(const ZONEPARTS* parts);

#endif // GAME_MAPUNITTABLE_H
