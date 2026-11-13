#ifndef SYSTEM_GRAPHIC_TEXINFO_H
#define SYSTEM_GRAPHIC_TEXINFO_H

#include <string.h>
#include "khtypes.h"

/* PARTIAL: the ref declares both in System/Graphic/Gprim.h (VTransInfo: System/Graphic/TexUtil.h); string-free, shared by
 * Gprim.h and modeldata.h, whose copies were gated per includer. No original object has a weak copy of the methods
 * (they are all inlined). */
class CTexAnm;

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
    CTexAnm* _pTexAnm;          /* 0x64 */
    KH_DEBUG(INT _nTexSize;              /* 0x68 */)
    KH_DEBUG(INT _nClutSize;             /* 0x6c */)

    BOOL isFixTex() const { return !_TexInfo._pImage || (_nFlag & 1); }
    BOOL isFixClut() const { return !_ClutInfo._pImage || (_nFlag & 2); }
    BOOL isFix() const { return isFixTex() && isFixClut(); }
    INT isTexW(/* parameters unknown */);
    ULONGLONG getTPage() const { return _nTPage; }
    ULONGLONG getTPage(UINT nTexPos, UINT nClutPos) const {
        return _nTPage | nTexPos | ((ULONGLONG)nClutPos << 37);
    }
    FLOAT getUVScrollOfs(/* parameters unknown */);
    BOOL isUVScroll(/* parameters unknown */);
    CHAR* getName() const { return _pName; }
#ifdef MODELDATA_H_TEXINFO_CTOR /* MATCH: only System/RsrcData (new TexInfo) has the constructor; the other units memset their locals themselves */
    TexInfo() { memset(this, 0, sizeof(TexInfo)); }
#endif
};

#endif /* SYSTEM_GRAPHIC_TEXINFO_H */
