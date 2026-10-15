#ifndef GAME_GIMMICK_GIMMICKDOOR_H
#define GAME_GIMMICK_GIMMICKDOOR_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"
#include "Game/SceneInfo.h"
#include "Game/MapUnitTable.h"

class CGimmickDoor : public CBaseGimmick {
private:
    INT m_iStep;           /* 0x220 */
    int m_hEffect[2];      /* 0x224 */
    DOOR_ID m_DoorID;      /* 0x22c */
    DOOR_STAT m_DoorState; /* 0x230 */

public:
    CGimmickDoor(CTask* inParent, const ZONEPARTS& inParts);
    virtual ~CGimmickDoor();
    virtual INT getTypeID() const;
    DOOR_ID getDoorID() const { return m_DoorID; }

protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();

public:
    void killEffectCrown();
    void setLockOnMarker(BOOL in_flag);
};

extern MAPUNITTABLE g_DoorWorld1[12];
extern MAPUNITTABLE g_DoorWorld2[12];
extern MAPUNITTABLE g_DoorWorld3[12];
extern MAPUNITTABLE g_DoorWorld4[12];
extern MAPUNITTABLE g_DoorWorld5[12];
extern MAPUNITTABLE g_DoorWorld6[12];
extern MAPUNITTABLE g_DoorWorld7[12];
extern MAPUNITTABLE g_DoorWorld8[12];
extern MAPUNITTABLE g_DoorWorld9[12];
extern MAPUNITTABLE g_DoorWorld10[12];
extern MAPUNITTABLE g_DoorWorld11[12];
extern MAPUNITTABLE g_DoorWorld12[12];
extern MAPUNITTABLE g_DoorWorld13[12];
extern MAPUNITTABLE g_DoorWorld0[12];

#endif /* GAME_GIMMICK_GIMMICKDOOR_H */
