#ifndef SYSTEM_GRAPHIC_HERMITEPATHCAMERA_H
#define SYSTEM_GRAPHIC_HERMITEPATHCAMERA_H

#include "khtypes.h"
#include "System/Graphic/Camera.h"

/* PARTIAL: added for System/Graphic/PathDataCamera (layout from the debug info; members as needed) */

class CPathVector;
class CHermitePath;

class CHermitePathCamera : public CCamera {
private:
    INT m_iStep;               /* 0x070 */
    UINT m_iStatus;            /* 0x074 */
    UBYTE _m_0x78[0x1e8];      /* 0x078 m_iKind .. 0x140 m_DDPathCamArrow */

public:
    CHermitePathCamera(CTask* p, const CPathVector* pPath, const CTask* pTarget, const CHermitePath* pHPath,
                       INT iType, const CTask* pParent);
    BOOL isEnd() const;
    void out();
    void initBGCheck(BOOL fgTransparency);
    void stop() { m_iStatus |= 2; }
    void run() { m_iStatus &= ~2; }
    void ignorePause(BOOL bInFlag) { CTask::ignorePause(bInFlag); }
    void ignorePlaySpeed(BOOL bInFlag) { CTask::ignorePlaySpeed(bInFlag); }
};

#endif /* SYSTEM_GRAPHIC_HERMITEPATHCAMERA_H */
