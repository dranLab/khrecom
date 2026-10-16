#ifndef GAME_COLLISION_COLLMAPCAMERA_H
#define GAME_COLLISION_COLLMAPCAMERA_H

#include "khtypes.h"
#include "System/Vector.h"
#include "Game/Collision/CollMap.h"

class CCMCSemiModelList;
class CCollisionList;

class CCollMapCamera : public CCollMap {
public:
    static CCMCSemiModelList* m_pSemiModelList;
    static BOOL m_fgForceStop; // PARTIAL: public for Form/Menu/WorldSelect (private in the original)

private:
    BOOL m_fgListExist; /* 0x2c */

public:
    CCollMapCamera();
    virtual ~CCollMapCamera();
    static void setFgForceStop(BOOL fg) {
        m_fgForceStop = fg;
        if (fg) clearList();
    }
    virtual INT collideMap();
    INT collideMapTrueCamera(INT fgContinue);
    BOOL checkBGforCamera(FVECTOR& vPos, FVECTOR& vAim, FVECTOR& vOutPos, BOOL fgTransparency);

private:
    INT collideRide(CCollisionList* pList, INT fgTrue);
public: // PARTIAL: public for Form/Menu/WorldSelect (private in the original)
    static void clearList();
};

#endif /* GAME_COLLISION_COLLMAPCAMERA_H */
