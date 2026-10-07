// PARTIAL: added for Game/Gimmick/GimmickPlantChild
#ifndef SYSTEM_GRAPHIC_MODEL_H
#define SYSTEM_GRAPHIC_MODEL_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Debug.h"
#include "System/Graphic/modeldata.h"

class CModel {
private:
    UBYTE _m_0x00[0x2c]; /* 0x000 CDrawable .. m_ColorScale.z */
    FLOAT m_fAlphaScale; /* 0x02c m_ColorScale.w */
    UBYTE _m_0x30[0xf0]; /* 0x030 m_nFlag .. 0x120 */

public:
    FLOAT getAlphaScale() { return m_fAlphaScale; }
    void setAlphaScale(FLOAT inScale);
};

#ifdef GAME_ENTITY_H
inline CModel* CEntity::getModel(INT inIndex) { return &m_pModels[inIndex]; }
#endif

/* Placeholders for the original header's inline functions: their strings ("System/Graphic/Model.h",
 * "m_pData != __null", "System/Graphic/Model.h(569)", "0", and CMCShader's "create CMCShader\n" /
 * "delete CMCShader\n") are in .rodata of every includer, in this order. Only line 569 is known. */
#line 1 "System/Graphic/Model.h"
inline void _modelCheckData(void* inData) {
    if (!(inData != __null)) AssertApp(__FILE__, __LINE__, "m_pData != __null", 0);
}
inline void* _modelCreate() {
    return com_malloc_d(4, __FILE__ "(569)", (UINT)-1);
}
inline void _modelAssertZero() {
    AssertApp(__FILE__, __LINE__, "0", 0);
}
inline void _mcShaderCreate() {
    TRACE("create CMCShader\n");
}
inline void _mcShaderDelete() {
    TRACE("delete CMCShader\n");
}

#endif /* SYSTEM_GRAPHIC_MODEL_H */
