// PARTIAL: added for System/Graphic/LightUnit
#ifndef SYSTEM_GRAPHIC_WORLDENV_H
#define SYSTEM_GRAPHIC_WORLDENV_H

#include "khtypes.h"

class CEffectLight;

class CWorldEnv {
public:
    static CEffectLight* getEffLight(UINT nIndex);
    static void initSeekActiveEffLight();
    static CEffectLight* seekNextActiveEffLight();
};

#endif /* SYSTEM_GRAPHIC_WORLDENV_H */
