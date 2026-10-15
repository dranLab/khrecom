// PARTIAL: added for System/Graphic/LightUnit
#ifndef SYSTEM_GRAPHIC_WORLDENV_H
#define SYSTEM_GRAPHIC_WORLDENV_H

#include "khtypes.h"

class CEffectLight;
class CCamera; // PARTIAL: added for Form/DebugWindow

class CWorldEnv {
public:
    static CEffectLight* getEffLight(UINT nIndex);
    static void initSeekActiveEffLight();
    static CEffectLight* seekNextActiveEffLight();
    // PARTIAL: added for System/Sound/SoundMan
    static const union FVECTOR& getViewVector();
    static BOOL isReadyMike();
    static const union FVECTOR& getMikePos();
    static void readyMike(INT nMode, INT nParam);
    static void setFogColor(INT r, INT g, INT b); // PARTIAL: added for Form/DebugMenu
    // PARTIAL: added for Form/DebugWindow
    static CCamera* getActiveCamera();
    static CCamera* getOldCamera();
};

#endif /* SYSTEM_GRAPHIC_WORLDENV_H */
