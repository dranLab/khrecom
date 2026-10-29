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

struct BONELABEL { /* PARTIAL: added for System/Graphic/Model */
    char m_Name[16];     /* 0x00 */
    SHORT m_ParentIndex; /* 0x10 */
    SHORT m_BuddyIndex;  /* 0x12 */
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

    BOOL isDataEmpty() const { return !m_pData || *(UINT*)m_pData == 0x60000000; } // PARTIAL: added for System/Graphic/Model
    BOOL isSemiDataEmpty() const { return !m_pSemiData || *(UINT*)m_pSemiData == 0x60000000; }
    UINT getRawMode() const {
        if (!isDataEmpty()) return ((UINT*)m_pData)[7];
        if (!isSemiDataEmpty()) return ((UINT*)m_pSemiData)[7];
        return 0;
    }
    USHORT getMode() const { return getRawMode(); }
    BOOL isShadow() const { if (getMode() == 0x10) return TRUE; return FALSE; } // PARTIAL: added for System/Graphic/Model; if-form for Game/Entity translucence_ (li 1; movn)
    USHORT getShaderOption() const {
        const SHORT* tag = (const SHORT*)(!isDataEmpty() ? m_pData : m_pSemiData);
        INT version = tag[0x16 / 2];
        if (!version) return 0;
        return ((const USHORT*)tag)[0x1e / 2];
    }
    BOOL enablePointLight() const;
    BOOL enableLight() const;
    INT getBoneIndex(const CHAR* pName) const; // PARTIAL: added for System/Graphic/TriangleIK
    // PARTIAL: added for Game/AttachCard
    static BOOL isNormal(UINT mode);
    static BOOL isColorI(UINT mode);
    static BOOL isColorF(UINT mode);
    static BOOL isTexture(UINT mode);
    // PARTIAL: added for System/Graphic/Model
    static BOOL isSky(UINT mode);
    static BOOL isMap(UINT mode);
    static BOOL isCharacter(UINT mode);
    static BOOL isCharacterAndMap(UINT mode);
};

/* PARTIAL: added for System/Graphic/ModelMan, which defines MODELDATA_H_TEXINFO (System/Graphic/TexCtrl and
 * System/Graphic/TexAnm have local copies). */
#ifdef MODELDATA_H_TEXINFO
struct VTransInfo {
    UINT _nDistPos;      /* 0x00 */
    USHORT _nFormat;     /* 0x04 */
    USHORT _nDistBufW;   /* 0x06 */
    ULONGLONG* _pImage;  /* 0x08 */
    UINT _qwImage;       /* 0x0c */
    USHORT _nDistX;      /* 0x10 */
    USHORT _nDistY;      /* 0x12 */
    USHORT _nDistW;      /* 0x14 */
    USHORT _nDistH;      /* 0x16 */
    USHORT _nTw;         /* 0x18 */
    USHORT _nTh;         /* 0x1a */
    UINT _nTransSize;    /* 0x1c */
};

struct TexInfo {
    ULONGLONG _nTPage;          /* 0x00 */
    VTransInfo _TexInfo;        /* 0x08 */
    VTransInfo _ClutInfo;       /* 0x28 */
    TexInfo* _pExtendInfo0;     /* 0x48 */
    TexInfo* _pExtendInfo1;     /* 0x4c */
    TexInfo* _pExtendInfo2;     /* 0x50 */
    UBYTE _nInfoType;           /* 0x54 */
    UBYTE _nType;               /* 0x55 */
    UBYTE _nParam;              /* 0x56 */
    UBYTE _nFlag;               /* 0x57 */
    float _fOfs[2];             /* 0x58 */
    CHAR* _pName;               /* 0x60 */
    void* _pTexAnm;             /* 0x64 CTexAnm* */
    INT _nTexSize;              /* 0x68 */
    INT _nClutSize;             /* 0x6c */

    ULONGLONG getTPage() { return _nTPage; }
    BOOL isFixTex() const { return !_TexInfo._pImage || (_nFlag & 1); }
    BOOL isFixClut() const { return !_ClutInfo._pImage || (_nFlag & 2); }
    BOOL isFix() const { return isFixTex() && isFixClut(); } // PARTIAL: added for System/RsrcData
#ifdef MODELDATA_H_TEXINFO_CTOR // PARTIAL: added for System/RsrcData (new TexInfo)
    TexInfo() { memset(this, 0, sizeof(TexInfo)); }
#endif
};

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
