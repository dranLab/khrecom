// PARTIAL: added for Game/Gimmick/GimmickPlantChild
#ifndef SYSTEM_GRAPHIC_MODELDATA_H
#define SYSTEM_GRAPHIC_MODELDATA_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/Graphic/TexInfo.h"

/* Placeholder for the original header's inline function asserting _pTexInfo: its strings
 * ("System/Graphic/modeldata.h", "_pTexInfo") are in .rodata of every includer. The line number is not known. */
#ifndef MODELDATA_H_PATH // MATCH: path override for System/Graphic/MapUnit
#define MODELDATA_H_PATH "System/Graphic/modeldata.h"
#endif
#line 1 MODELDATA_H_PATH
inline void _modelDataCheckTexInfo(void* _pTexInfo) {
    if (!_pTexInfo) AssertApp(__FILE__, __LINE__, "_pTexInfo", 0);
}


struct BBOX {
    float x[2];
    float y[2];
    float z[2];
};

struct BONELABEL {
    char m_Name[16];     /* 0x00 */
    SHORT m_ParentIndex; /* 0x10 */
    SHORT m_BuddyIndex;  /* 0x12 */
};


struct MODELDATA {
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

    BOOL isDataEmpty() const { return !m_pData || *(UINT*)m_pData == 0x60000000; }
    BOOL isSemiDataEmpty() const { return !m_pSemiData || *(UINT*)m_pSemiData == 0x60000000; }
    UINT getRawMode() const {
        if (!isDataEmpty()) return ((UINT*)m_pData)[7];
        if (!isSemiDataEmpty()) return ((UINT*)m_pSemiData)[7];
        return 0;
    }
    USHORT getMode() const { return getRawMode(); }
    BOOL isShadow() const { if (getMode() == 0x10) return TRUE; return FALSE; } // MATCH: if-form for Game/Entity translucence_ (li 1; movn)
    USHORT getShaderOption() const {
        const SHORT* tag = (const SHORT*)(!isDataEmpty() ? m_pData : m_pSemiData);
        INT version = tag[0x16 / 2];
        if (!version) return 0;
        return ((const USHORT*)tag)[0x1e / 2];
    }
    BOOL enablePointLight() const;
    BOOL enableLight() const;
    INT getBoneIndex(const CHAR* pName) const;
    static BOOL isNormal(UINT mode);
    static BOOL isColorI(UINT mode);
    static BOOL isColorF(UINT mode);
    static BOOL isTexture(UINT mode);
    static BOOL isSky(UINT mode);
    static BOOL isMap(UINT mode);
    static BOOL isCharacter(UINT mode);
    static BOOL isCharacterAndMap(UINT mode);
};

/* TexInfoStub's inline asserts put this header's path and "_pTexInfo" into the .rodata of every includer: only
 * the units whose original has those strings define MODELDATA_H_TEXINFO. */
#ifdef MODELDATA_H_TEXINFO // MATCH: only includers whose original .rodata has TexInfoStub's assert strings
#line 190
struct TexInfoStub {
    union {
        struct {
            TexInfo* _pTexInfo;
            SHORT _nTexPos;
            SHORT _nClutPos;
        };
        ULONGLONG _nValue;
    };
    void init(const TexInfo* pInInfo, SHORT nInTexPos, SHORT nInClutPos); void update(SHORT nInTexPos, SHORT nInClutPos);
    BOOL isFix() { if (!_pTexInfo) AssertApp(__FILE__, __LINE__, "_pTexInfo", 0); return _pTexInfo->isFix(); } // PARTIAL: isFix/getTPage added for System/Graphic/Model
    BOOL isFixTex() { if (!_pTexInfo) AssertApp(__FILE__, __LINE__, "_pTexInfo", 0); return _pTexInfo->isFixTex(); }
    BOOL isFixClut() { if (!_pTexInfo) AssertApp(__FILE__, __LINE__, "_pTexInfo", 0); return _pTexInfo->isFixClut(); }

    ULONGLONG getTPage() { if (!_pTexInfo) AssertApp(__FILE__, __LINE__, "_pTexInfo", 0); return _pTexInfo->getTPage(); }
};
#endif

#endif /* SYSTEM_GRAPHIC_MODELDATA_H */
