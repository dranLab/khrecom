// PARTIAL: added for Game/Gimmick/GimmickPlantChild
#ifndef SYSTEM_GRAPHIC_MODELDATA_H
#define SYSTEM_GRAPHIC_MODELDATA_H

#include "khtypes.h"
#include "System/MainApp.h"

/* Placeholder for the original header's inline function asserting _pTexInfo: its strings
 * ("System/Graphic/modeldata.h", "_pTexInfo") are in .rodata of every includer. The line number is not known. */
#ifndef MODELDATA_H_PATH // PARTIAL: path override added for System/Graphic/MapUnit
#define MODELDATA_H_PATH "System/Graphic/modeldata.h"
#endif
#line 1 MODELDATA_H_PATH
inline void _modelDataCheckTexInfo(void* _pTexInfo) {
    if (!_pTexInfo) AssertApp(__FILE__, __LINE__, "_pTexInfo", 0);
}

// PARTIAL: added for System/Graphic/LightUnit
struct BBOX {
    float x[2];
    float y[2];
    float z[2];
};

// PARTIAL: added for Game/ModelObj
struct MODELDATA {
    // PARTIAL: data members added for System/Graphic/LightUnit
    BYTE m_nBoneNum;
    BYTE m_BoneIndex;
    unsigned char m_Pad[2];
    void* m_pBoneLabel; /* BONELABEL* */
    void* m_pBones;     /* FMATRIXPARAM* */
    SHORT m_nTexNum;
    USHORT m_nPacketSize;
    void* m_pTexParam; /* TEXPARAM* */
    void* m_pData;     /* MODELTAG* */
    void* m_pSemiData; /* MODELTAG* */
    BBOX* m_pBBox;

    BOOL enablePointLight() const;
    BOOL enableLight() const;
};

#endif /* SYSTEM_GRAPHIC_MODELDATA_H */
