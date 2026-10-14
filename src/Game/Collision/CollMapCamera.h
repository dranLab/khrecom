// PARTIAL: added for System/Graphic/PathCamera
#ifndef GAME_COLLISION_COLLMAPCAMERA_H
#define GAME_COLLISION_COLLMAPCAMERA_H

#include "khtypes.h"
#include "System/Vector.h"
#include "Game/Collision/CollMap.h"

class CCollMapCamera : public CCollMap {
private:
    BOOL m_fgListExist;

public:
    CCollMapCamera();
    virtual ~CCollMapCamera();
    virtual INT collideMap();
    INT collideMapTrueCamera(INT fgContinue);
    BOOL checkBGforCamera(FVECTOR& vPos, FVECTOR& vAim, FVECTOR& vOutPos, BOOL fgTransparency);
    // PARTIAL: added for Form/Menu/WorldSelect
    static BOOL m_fgForceStop;
    static void clearList();
};

#endif /* GAME_COLLISION_COLLMAPCAMERA_H */
