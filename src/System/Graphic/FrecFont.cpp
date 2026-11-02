#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/Graphic/FrecFont.h"
#include "System/Graphic/Fnt.h"
#include "System/Graphic/TexUtil.h"
#include "System/Buf.h"
#include "System/VramMan.h"
#include "include_asm.h"
#include <string.h>

/* The unit's .text ends with an unnamed copy of CBuf's inline destructor (and .rodata with one of its vtable): our
 * compiled copies are only kept with these lines in config/SLUS_217.99/symbol_addrs.txt
 *     _$_4CBuf_001274C0 = 0x001274C0; // type:func size:0x54
 *     _vt$4CBuf_0045FED0 = 0x0045FED0; // size:0x18 */


extern "C" {
TIM2_PICTUREHEADER* Tim2GetPictureHeader(const void* pTim2, int imgno);
void* Tim2GetClut(const TIM2_PICTUREHEADER* ph);
int Tim2CheckFileHeader(const void* pTim2);
}

#include "System/Graphic/TexInfo.h"
















































class CRsrcData;

#include "System/RsrcDB.h"





struct _FontInfo {
    unsigned long _nTex0[4][8];
    char _szFontName[24];
    INT _nClutPos;
    USHORT _nFontSize;
    USHORT _nFontCntMax;
    USHORT _nPageTimWidth;
    USHORT _nPageTimHeight;
    USHORT _nPageChrCntX;
    USHORT _nPageChrCntY;
    USHORT _nPageChrCnt;
    USHORT _nPageMax;
    BYTE* _pFontWidthTbl;
    USHORT* _pSjisCnvTbl;
};

struct _FontExtFileHeader {
    char _szSig[4];
    USHORT _nVer;
    USHORT _nDataCount;
    UINT _nCnvTblOffset;
    BYTE _cFontSize;
    unsigned char __cDummy[3];
    unsigned char _pWidthTbl[1];
};

static unsigned int s_fntFixColor[16] = {
    0x80080880, 0x80088008, 0x80787800, 0x80242424, 0x80202673, 0x807A4044, 0x803C671C, 0x805E4A80,
    0x80787878, 0x80048080, 0x8032337D, 0x807E4446, 0x80636338, 0x80306580, 0x8032527D, 0x80484848,
};

namespace {
ULONGLONG s_nTEX2_2;
UINT s_nAvailableFontTypes;
_FontInfo s_FontInfo[2];
}

template <class T> static inline T& _ref(T& v) { return v; }

#define GS_TEX2(psm, cbp, cpsm, csm, csa, cld)                                                              \
    ((ULONGLONG)(psm) << 20 | (ULONGLONG)(cbp) << 37 | (ULONGLONG)(cpsm) << 51 | (ULONGLONG)(csm) << 55 | \
     (ULONGLONG)(csa) << 56 | (ULONGLONG)(cld) << 61)

void CFrecFont::initialize() {
    s_nAvailableFontTypes = 0;
    for (INT i = 0; i < 2; i++) {
        s_FontInfo[i]._nFontCntMax = 0;
        s_FontInfo[i]._pFontWidthTbl = NULL;
        s_FontInfo[i]._pSjisCnvTbl = NULL;
    }
    s_nTEX2_2 = GS_TEX2(0x13, VRAM::_CLUT_FONT_START, 0, 0, 0, 1);
}

void CFrecFont::finalize() {
    s_nAvailableFontTypes = 0;
}

BOOL _setupFontClutExt(const RsrcInfo& info, const TIM2_PICTUREHEADER& head);

/* Matches except for the order of two loop-invariant CSA values that loop.c hoists ((nCSA + 6) << 56 is computed
 * before (nCSA + 2) << 56 in the original). */
#if 0
BOOL CFrecFont::setupFontTex(const RsrcInfo& info, const CBuf& src) {
    TIM2_PICTUREHEADER* pHead = Tim2GetPictureHeader(src.getBufPtr(), 0);
    if (!pHead || pHead->ImageType != 4) {
        return FALSE;
    }
    INT n = Tim2CheckFileHeader(src.getBufPtr());
    if (n == 2) {
        return _setupFontClutExt(info, *pHead);
    }
    if (n != 1) {
        return FALSE;
    }
    if (s_nAvailableFontTypes >= 2) {
        return FALSE;
    }
    n = _ref(s_nAvailableFontTypes)++;
    _FontInfo& font = s_FontInfo[n];
    strcpy(font._szFontName, info._pszName);
    CHAR* pExt = strrchr(font._szFontName, '.');
    if (pExt) {
        *pExt = '\0';
    }
    INT nTexPos;
    if (CVramMan::allocTex((pHead->ImageWidth * pHead->ImageHeight / 256) >> 1, 1, nTexPos, font._nClutPos, 2) < 0) {
        return FALSE;
    }
    TexInfo t;
    memset(&t, 0, sizeof(t));
    INT nDiv = pHead->ImageHeight > 256 ? 2 : 1;
    font._nPageMax = nDiv * 2;
    VTransInfo& tex = t._TexInfo;
    for (INT i = 0; i < nDiv; i++) {
        CTexUtil::getTransInfo(*pHead, tex, t._ClutInfo, nTexPos, font._nClutPos);
        tex._nFormat = i ? 0x2C : 0x24;
        if (!i) {
            t._ClutInfo._qwImage = pHead->ClutSize >> 4;
            t._ClutInfo._nDistW = 8;
            t._ClutInfo._nDistH = 8;
            t._ClutInfo._nTw = 3;
            t._ClutInfo._nTh = 3;
            font._nPageTimWidth = tex._nDistW;
            font._nPageTimHeight = tex._nDistH / nDiv;
        } else {
            tex._pImage += t._TexInfo._qwImage;
            t._ClutInfo._pImage = NULL;
        }
        tex._nDistH = font._nPageTimHeight;
        tex._qwImage /= nDiv;
        tex._nTh--;
        CBuf tmp;
        tmp.attachBuf((BYTE*)tex._pImage, -1);
        CVramMan::reqTransTex(t, tmp);
        ULONGLONG nCSA = n & 1;
        ULONGLONG nTex0 = (ULONGLONG)nTexPos | (ULONGLONG)tex._nDistBufW << 14 |
                          (ULONGLONG)tex._nFormat << 20 | (ULONGLONG)tex._nTw << 26 |
                          (ULONGLONG)tex._nTh << 30 | (ULONGLONG)1 << 34 |
                          (ULONGLONG)VRAM::_CLUT_FONT_START << 37 | (ULONGLONG)t._ClutInfo._nFormat << 51;
        font._nTex0[i * 2][0] = nTex0 | nCSA << 56;
        font._nTex0[i * 2 + 1][0] = nTex0 | (nCSA + 2) << 56;
        font._nTex0[i * 2][1] = nTex0 | (nCSA + 4) << 56;
        font._nTex0[i * 2 + 1][1] = nTex0 | (nCSA + 6) << 56;
        font._nTex0[i * 2][2] = nTex0 | (ULONGLONG)8 << 56;
        font._nTex0[i * 2 + 1][2] = nTex0 | (ULONGLONG)10 << 56;
        font._nTex0[i * 2][3] = nTex0 | (ULONGLONG)12 << 56;
        font._nTex0[i * 2 + 1][3] = nTex0 | (ULONGLONG)14 << 56;
        font._nTex0[i * 2][4] = nTex0 | (ULONGLONG)9 << 56;
        font._nTex0[i * 2 + 1][4] = nTex0 | (ULONGLONG)11 << 56;
        font._nTex0[i * 2][5] = nTex0 | (ULONGLONG)13 << 56;
        font._nTex0[i * 2 + 1][5] = nTex0 | (ULONGLONG)15 << 56;
    }
    return TRUE;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/FrecFont", setupFontTex__9CFrecFontRC8RsrcInfoRC4CBuf);
#endif

void CFrecFont::setupFonts() {
    for (INT i = 0; i < s_nAvailableFontTypes; i++) {
        _FontInfo& font = s_FontInfo[i];
        CRsrcData* pData = CRsrcDB::getRsrc(1, font._szFontName, NAME_CASE_SENSITIVE);
        _FontExtFileHeader* p = pData ? (_FontExtFileHeader*)CRsrcDB::getRsrcData(*pData) : NULL;
        if (p) {
            font._pFontWidthTbl = p->_pWidthTbl;
            font._pSjisCnvTbl = (USHORT*)((BYTE*)p + p->_nCnvTblOffset);
            font._nFontSize = p->_cFontSize;
            font._nFontCntMax = p->_nDataCount;
            font._nPageChrCntX = font._nPageTimWidth / font._nFontSize;
            font._nPageChrCntY = font._nPageTimHeight / font._nFontSize;
            font._nPageChrCnt = font._nPageChrCntX * font._nPageChrCntY;
        }
    }
}

BOOL _setupFontClutExt(const RsrcInfo& info, const TIM2_PICTUREHEADER& head) {
    if (strncmp(info._pszName, "evt", 3) != 0) {
        return FALSE;
    }
    INT nDistPos = VRAM::_CLUT_FONT_START + 3;
    CHAR* p = strrchr(info._pszName, '.');
    if (!p) {
        p = info._pszName + strlen(info._pszName);
    }
    if (strncmp(p - 4, "menu", 4) == 0) {
        nDistPos = VRAM::_CLUT_FONT_START + 2;
    }
    TexInfo t;
    memset(&t, 0, sizeof(t));
    VTransInfo& clut = t._ClutInfo;
    clut._nDistPos = nDistPos;
    t._TexInfo._pImage = NULL;
    clut._nFormat = 0;
    clut._nDistBufW = 1;
    clut._pImage = (ULONGLONG*)Tim2GetClut(&head);
    clut._qwImage = 16;
    clut._nDistX = 0;
    clut._nDistY = 0;
    clut._nDistW = 8;
    clut._nDistH = 8;
    clut._nTw = 3;
    clut._nTh = 3;
    CBuf tmp;
    tmp.attachBuf((BYTE*)clut._pImage, -1);
    CVramMan::reqTransTex(t, tmp);
    return TRUE;
}

USHORT _fromSJIS(const _FontInfo& font, USHORT nSJIS) {
    INT nIdx = 0;
    if (nSJIS >= 0x21 && nSJIS <= 0x7F) {
        nIdx = nSJIS - 0x20;
    } else if (nSJIS > 0x8100) {
        INT nH = nSJIS >> 8;
        INT nL = nSJIS & 0xFF;
        if (nH < 0x85) {
            nIdx = (nH - 0x81) * 0xC0 + nL + 0x20;
        } else if (nH >= 0x87 && nH <= 0x9F) {
            nIdx = (nH - 0x83) * 0xC0 + nL + 0x20;
        } else if (nH >= 0xE0 && nH <= 0xE5) {
            nIdx = (nH - 0xC3) * 0xC0 + nL + 0x20;
        }
    }
    return font._pSjisCnvTbl[nIdx];
}

USHORT CFrecFont::fromSJIS(USHORT nType, USHORT nSJIS) {
    if (nType < s_nAvailableFontTypes && s_FontInfo[nType]._pSjisCnvTbl) {
        return _fromSJIS(s_FontInfo[nType], nSJIS);
    }
    return 0;
}

USHORT CFrecFont::toSJIS(USHORT nType, USHORT nCode) {
    if (nType >= s_nAvailableFontTypes || !s_FontInfo[nType]._pSjisCnvTbl || nCode >= s_FontInfo[nType]._nFontCntMax) {
        return 0;
    }
    _FontInfo& font = s_FontInfo[nType];
    for (USHORT i = 0; i < 0x1AA0; i++) {
        if (font._pSjisCnvTbl[i] == nCode) {
            if (i < 0x60) {
                return i + 0x20;
            }
            i -= 0x60;
            USHORT nTop;
            if (i < 0x300) {
                nTop = 0x81;
            } else {
                i -= 0x300;
                if (i < 0x12C0) {
                    nTop = 0x87;
                } else {
                    i -= 0x12C0;
                    nTop = 0xE0;
                }
            }
            return ((nTop + i / 0xC0) << 8) | (i % 0xC0 + 0x40);
        }
    }
    return 0;
}

static inline void _setQW(u_long128*& qp, INT i, u_long data, u_long addr) {
    u_long128 out128;
    __asm__ volatile("pcpyld %0, %1, %0" : "=r"(out128) : "r"(addr), "0"(data));
    qp[i] = out128;
}

static inline u_long _uv(const G_UV& t) {
    return (u_long)((t.u << 4) | 8) | (u_long)((t.v << 4) | 8) << 16;
}

static inline u_long _xyz(const G_XYZ& p) {
    return (u_long)(p.x + 0x8000) | (u_long)(p.y + 0x8000) << 16 | (u_long)((long)p.z >> 4) << 32;
}

void _makeFontPkt(const _FontInfo& font, USHORT nCode, FIX4 x, FIX4 y, FIX4 w, FIX4 h, u_long128*& qp, USHORT s,
                  USHORT palette) {
    if (nCode < font._nFontCntMax) {
        INT nPage = nCode / font._nPageChrCnt;
        USHORT nFontSize = font._nFontSize;
        if (!s) {
            s = nFontSize;
        }
        nCode -= nPage * font._nPageChrCnt;
        USHORT v = nCode / font._nPageChrCntX;
        USHORT u = nCode - v * font._nPageChrCntX;
        v *= nFontSize;
        u *= nFontSize;
        if (!w) {
            w = h = nFontSize << 4;
        }
        G_XYZ p0;
        G_XYZ p1;
        G_UV t0;
        G_UV t1;
        p0.set(x, y);
        p1.set(x + w, y + h);
        t0.set(u, v);
        t1.set(u + s - 1, v + nFontSize);
        _setQW(qp, 0, font._nTex0[nPage][palette], 7);
        _setQW(qp, 1, _uv(t0), 3);
        _setQW(qp, 2, _xyz(p0), 5);
        _setQW(qp, 3, _uv(t1), 3);
        _setQW(qp, 4, _xyz(p1), 5);
        qp += 5;
    }
}

/* Parked: 331 of 749 lines - the draft has the original's structure (all locals in stack slots, contiguous MMI colour
 * sequences as volatile in-place asm, control-code switch, outline/shadow passes), but the spill slots differ: the
 * original's G_RGBA locals colExt/colBak/colSave get their (16-aligned) slots right after font, ours after nChar, so
 * every later offset and the register choices shift. tried: declaration order (ref order, all at the top, the three
 * colours first), .col copies instead of struct copies (worse), getDispSize adjust as if/else (worse), _setQW with
 * "r" data (worse), tmp = 0xFF forms for the clamp constant (the original rematerialises li 255 per multiply).
 * Next: find why gcc numbers the G_RGBA pseudos late (ADDRESSOF from struct copies?) before tuning anything else. */
#if 0
#include "System/Graphic/PackMan.h"
static inline u_long128 _expandColor(UINT iCol) {
    u_long128 ivCol;
    __asm__ volatile("pextlb %0, $0, %1" : "=r"(ivCol) : "r"(iCol));
    __asm__ volatile("pextlh %0, $0, %0" : "=r"(ivCol) : "0"(ivCol));
    return ivCol;
}
static inline u_long128 _clampColor(u_long128 ivCol) {
    u_long128 tmp;
    __asm__ volatile("pextlw %0, %0, %0" : "=r"(tmp) : "0"(0xFF));
    __asm__ volatile("pcpyld %0, %0, %0" : "=r"(tmp) : "0"(tmp));
    __asm__ volatile("psrlw %0, %0, 7" : "=r"(ivCol) : "0"(ivCol));
    __asm__ volatile("pcgtw %0, %1, %0" : "=r"(tmp) : "r"(ivCol), "0"(tmp));
    __asm__ volatile("por %0, %1, %0" : "=r"(tmp) : "r"(ivCol), "0"(tmp));
    return tmp;
}
static inline UINT _packColor(u_long128 ivCol) {
    UINT iCol;
    __asm__ volatile("ppach %0, $0, %0" : "=r"(ivCol) : "0"(ivCol));
    __asm__ volatile("ppacb %0, $0, %1" : "=r"(iCol) : "r"(ivCol));
    return iCol;
}
static inline UINT _mulColor(UINT iCol, UINT iMul) {
    u_long128 ivCol = _expandColor(iCol);
    u_long128 ivMul = _expandColor(iMul);
    __asm__ volatile("pmulth %0, %1, %2" : "=r"(ivCol) : "r"(ivCol), "r"(ivMul));
    return _packColor(_clampColor(ivCol));
}
static inline void _setRGBAQ(u_long128*& qp, UINT iCol) {
    u_long128 out128;
    __asm__ volatile("pcpyld %0, %1, %0" : "=r"(out128) : "r"((u_long)1), "0"((u_long)iCol));
    *qp++ = out128;
}

void CFrecFont::makePkt(CFntM2& prim) {
    UINT nStyle = prim.m_Style;
    INT nFontType = (nStyle & 0x1000) >> 12;
    BOOL bDebugMode = nStyle & 0x8000;
    USHORT nPalette = 0;
    if (nFontType) {
        if (!(nStyle & 0x10000)) {
            if (nStyle & 0x2000) {
                nPalette = 1;
            } else if (nStyle & 0x20000) {
                nPalette = 2;
            } else if (nStyle & 0x40000) {
                nPalette = 3;
            } else {
                nPalette = 4;
            }
        }
    }
    if (nFontType >= s_nAvailableFontTypes) return;
    BOOL bEnableColor = nFontType == 0;
    _FontInfo& font = s_FontInfo[nFontType];
    if (!font._pSjisCnvTbl) return;
    G_RGBA colBase;
    G_RGBA colExt;
    G_RGBA colBak;
    UINT brightness = prim.getBrightness();
    if (bEnableColor) {
        colBase.col = _mulColor(prim.m_C0.col, brightness);
        colExt.col = _mulColor(prim.m_exC0.col, brightness);
    } else {
        colExt.col = 0x80808080;
        INT a = (prim.m_C0.a * (brightness >> 24)) >> 7;
        if (a > 0xFF) a = 0xFF;
        colBase.col = (a << 24) | 0x808080;
    }
    BYTE* pAlphaTbl = (BYTE*)prim.getAlphaTbl();
    BYTE nExtAlpha;
    BYTE nExtAlphaOld;
    if (pAlphaTbl) {
        nExtAlpha = pAlphaTbl[0];
        colExt.col = (colExt.col & 0xFFFFFF) | (nExtAlpha << 24);
        colBase.col = (colBase.col & 0xFFFFFF) | (nExtAlpha << 24);
    }
    BYTE* pStr = (BYTE*)prim.getText();
    FIX4 x = prim.m_P0.x;
    FIX4 y;
    FIX4 w = prim.m_Size;
    FIX4 h = w;
    y = prim.m_P0.y - w;
    INT nBaseSize = font._nFontSize;
    BOOL bIsBaseW = (w >> 4) == nBaseSize;
    G_XY space = prim.m_Space;
    G_XY* pLineXY = prim.getLineXYPtr();
    G_XY* pIconXY = prim.getIconXYPtr();
    colBak = colBase;
    FIX4 x0;
    FIX4 xOld;
    if (!pLineXY) {
        if (nStyle & 3) {
            G_XY wh;
            getDispSize(wh, prim, NULL, NULL);
            FIX4 d = wh.x;
            if (nStyle & 2) d >>= 1;
            x -= d;
        }
        xOld = x;
        x0 = x;
    } else {
        xOld = x;
        x0 = x;
        x += pLineXY[0].x;
    }
    INT nIcon = 0;
    BOOL bHalfSpaceExt = !(nStyle & 0x9000);
    UINT nStyleBak = nStyle = nStyle & 0xD0;
    INT nLine = 0;
    INT nChar = 0;
    u_long128 tag;
    __asm__ volatile("pcpyld %0, %1, %0" : "=r"(tag) : "r"((u_long)0xE), "0"((u_long)0x1000800000008000UL));
    u_long128* qp = CPackMan::pkOpenGifTag(tag);
    _setQW(qp, 0, 5, 9);
    _setQW(qp, 1, s_nTEX2_2, 0x17);
    _setQW(qp, 2, 0x356, 0);
    _setQW(qp, 3, colBase.col, 1);
    qp += 4;
    while (*pStr) {
        USHORT c = *pStr++;
        USHORT nSJIS;
        if (pAlphaTbl) nExtAlpha = pAlphaTbl[nChar];
        if (c < 0x80) {
            if (c <= 0x20) {
                switch (c) {
                case 9:
                    {
                        FIX4 ww = prim.m_Size;
                        FIX4 sp = space.x;
                        if (!bHalfSpaceExt) {
                            x += ((ww >> 1) + sp) << 2;
                        } else {
                            x += (ww / 2 + sp) << 2;
                        }
                    }
                    break;
                case 0x20:
                    {
                        FIX4 ww = prim.m_Size;
                        FIX4 sp = space.x;
                        if (!bHalfSpaceExt) {
                            x += (ww >> 1) - (ww >> 3) + sp;
                        } else {
                            x += (ww >> 1) - (ww >> 3) + sp;
                        }
                    }
                    break;
                case 10:
                    nLine++;
                    x = x0;
                    if (pLineXY) x += pLineXY[nLine].x;
                    y += w + space.y;
                    break;
                }
                if (nLine >= prim.getDispLines()) break;
                continue;
            } else {
                if (bDebugMode) h = w >> 1;
                nSJIS = c;
            }
        } else {
            USHORT c2 = *pStr;
            if (c2) pStr++;
            if (c >= 0xF9) {
                switch (c2) {
                case 0x40:
                    x = xOld;
                    break;
                case 0x41:
                    if (bEnableColor) {
                        colBase = colBak;
                        _setRGBAQ(qp, colBase.col);
                    }
                    break;
                case 0x42:
                    {
                        FIX4 t = x - 2;
                        x = t - space.x;
                    }
                    break;
                case 0x43:
                case 0x44:
                case 0x45:
                    x += 0x10 << (c2 - 0x43);
                    break;
                case 0x46:
                case 0x47:
                    nPalette = c2 - 0x46;
                    break;
                case 0x48:
                    nStyle = nStyleBak;
                    break;
                case 0x49:
                    prim.resetDispCharsCtrl();
                    break;
                default:
                    if (c2 < 0x50) {
                    } else if (c2 < 0x60) {
                        if (bEnableColor) {
                            colBase.col = _mulColor(s_fntFixColor[c2 - 0x50], brightness);
                            _setRGBAQ(qp, colBase.col);
                        }
                    } else if (c2 < 0x70) {
                        nStyle = 0x10 << (c2 - 0x60);
                    } else if (c2 < 0x80) {
                        if (prim.getCurrentDispChars() < nChar) {
                            prim.setDispChars(nChar);
                            prim.setDispCharsCtrl(c2 - 0x6F);
                        }
                    } else {
                        if (pIconXY) pIconXY[nIcon++].set(x, y + w);
                        if (c2 < 0x98) x += 0x200;
                        else if (c2 < 0xA0) x += 0x280;
                        else x += 0x180;
                    }
                    break;
                }
                continue;
            }
            nSJIS = (c << 8) | c2;
            if (nSJIS == 0x8140) {
                x += ((prim.m_Size >> 1) + space.x) << 1;
                continue;
            }
            if (bDebugMode) h = w;
        }
        USHORT nCode = _fromSJIS(font, nSJIS);
        USHORT nPPW = font._pFontWidthTbl[nCode];
        if (nPPW < nBaseSize) nPPW++;
        USHORT s = 0;
        if (bDebugMode) s = nPPW;
        xOld = x;
        if (nStyle && bEnableColor) {
            if (nStyle & 0x10) {
                _makeFontPkt(font, nCode, x + (h >> 4), y, h, w, qp, s, 0);
                x = xOld;
            } else {
                FIX4 ySave = y;
                G_RGBA colSave = colBase;
                if (nStyle & 0x80) {
                    colBase = colExt;
                    _setRGBAQ(qp, colBase.col);
                    x -= h >> 4;
                    y -= w >> 4;
                    _makeFontPkt(font, nCode, x, y, h, w, qp, s, 0);
                    x += h >> 3;
                    _makeFontPkt(font, nCode, x, y, h, w, qp, s, 0);
                    y += w >> 3;
                    _makeFontPkt(font, nCode, x, y, h, w, qp, s, 0);
                    x -= h >> 3;
                } else {
                    colBase = colExt;
                    _setRGBAQ(qp, colBase.col);
                    x += h >> 3;
                    y += w >> 3;
                }
                _makeFontPkt(font, nCode, x, y, h, w, qp, s, 0);
                y = ySave;
                colBase = colSave;
                _setRGBAQ(qp, colBase.col);
                x = xOld;
            }
        }
        if (pAlphaTbl && nExtAlpha != nExtAlphaOld) {
            colBase.a = nExtAlpha;
            colExt.col = (colExt.col & 0xFFFFFF) | (nExtAlpha << 24);
            colBak = colBase;
            nExtAlphaOld = nExtAlpha;
            _setRGBAQ(qp, colBase.col);
        }
        _makeFontPkt(font, nCode, x, y, h, w, qp, s, nPalette);
        if (bDebugMode) {
            x += h;
        } else {
            FIX4 xNew;
            if (bIsBaseW) xNew = x + (nPPW << 4);
            else xNew = x + nPPW * h / nBaseSize;
            x = xNew;
        }
        nChar++;
        x += space.x;
        if (prim.getDispChars() && nChar >= prim.getDispChars()) break;
    }
    _setQW(qp, 0, CPackMan::getDefault_CLAMP_2(), 9);
    qp++;
    CPackMan::pkCloseGifTag(qp);
    prim.setCurrentDispChars(nChar);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/FrecFont", makePkt__9CFrecFontR6CFntM2);
#endif
/* placeholder for makePkt's data: the colour table it reads, then its jump table (labels of the included assembly) */
__asm__(".section .sdata\n");
static void __attribute__((section(".sdata"))) _makePktData() {
    *(volatile unsigned int*)&s_fntFixColor[0];
}
__asm__(
    ".rdata\n"
    ".align 4\n"
    ".word .L001269A8\n"
    ".word .L001269B0\n"
    ".word .L001269E8\n"
    ".word .L001269F8\n"
    ".word .L001269F8\n"
    ".word .L001269F8\n"
    ".word .L00126A0C\n"
    ".word .L00126A0C\n"
    ".word .L00126A1C\n"
    ".word .L00126A28\n"
    ".text\n");

INT CFrecFont::getDispSize(G_XY& outSize, const CFntM2& prim, G_XY* pLineSize, INT* pNumCharcter) {
    UINT nStyle = prim.m_Style;
    INT nFontType = (nStyle & 0x1000) >> 12;
    BOOL bDebugMode = nStyle & 0x8000;
    if (nFontType >= s_nAvailableFontTypes) {
        return 0;
    }
    _FontInfo& font = s_FontInfo[nFontType];
    if (!font._pSjisCnvTbl) {
        return 0;
    }
    BYTE* pStr = (BYTE*)prim.getText();
    if (!pStr) {
        return 0;
    }
    INT nLine = 1;
    INT nChar = 0;
    FIX4 xOld = 0;
    FIX4 xMax = 0;
    FIX4 x = 0;
    FIX4 w = prim.m_Size;
    FIX4 h = w;
    INT nBaseSize = font._nFontSize;
    BOOL bIsBaseW = (w >> 4) == nBaseSize;
    G_XY space = prim.m_Space;
    FIX4 y = h + space.y;
    G_XY* pLs = pLineSize;
    if (pLs) {
        pLs->x = 0x80000000;
    }
    BOOL bHalfSpaceExt = !(nStyle & 0x9000);
    while (*pStr) {
        USHORT c = *pStr++;
        USHORT nSJIS;
        if (c < 0x80) {
            if (c <= 0x20) {
                switch (c) {
                case 9:
                    {
                        FIX4 ww = prim.m_Size;
                        FIX4 sp = space.x;
                        if (!bHalfSpaceExt) {
                            x += ((ww >> 1) + sp) << 2;
                        } else {
                            x += (ww / 2 + sp) << 2;
                        }
                    }
                    break;
                case 0x20:
                    {
                        FIX4 ww = prim.m_Size;
                        FIX4 sp = space.x;
                        if (!bHalfSpaceExt) {
                            x += (ww >> 1) - (ww >> 3) + sp;
                        } else {
                            x += (ww >> 1) - (ww >> 3) + sp;
                        }
                    }
                    break;
                case 10:
                    if (xMax < x) xMax = x;
                    if (pLs) {
                        pLs->y = x;
                        pLs++;
                        pLs->x = 0x80000000;
                    }
                    x = 0;
                    nLine++;
                    y += h + space.y;
                    break;
                }
                continue;
            } else {
                if (bDebugMode) w = h >> 1;
                nSJIS = c;
            }
        } else {
            USHORT c2 = *pStr;
            if (c2) pStr++;
            if (c >= 0xF9) {
                switch (c2) {
                case 0x40:
                    x = xOld;
                    break;
                case 0x42:
                    {
                        FIX4 t = x - 2;
                        x = t - space.x;
                    }
                    break;
                case 0x43:
                case 0x44:
                case 0x45:
                    x += 0x10 << (c2 - 0x43);
                    break;
                default:
                    if (c2 >= 0x80) {
                        if (c2 < 0x98) x += 0x200;
                        else if (c2 < 0xA0) x += 0x280;
                        else x += 0x180;
                    }
                    break;
                }
                continue;
            }
            nSJIS = (c << 8) | c2;
            if (nSJIS == 0x8140) {
                x += ((prim.m_Size >> 1) + space.x) << 1;
                continue;
            }
            if (bDebugMode) w = h;
        }
        USHORT nCode = _fromSJIS(font, nSJIS);
        if (pLs && pLs->x == 0x80000000) pLs->x = x;
        xOld = x;
        if (bDebugMode) {
            x += w;
        } else {
            USHORT nPPW = font._pFontWidthTbl[nCode];
            if (nPPW < nBaseSize) nPPW++;
            FIX4 xNew;
            if (bIsBaseW) xNew = x + (nPPW << 4);
            else xNew = x + nPPW * w / nBaseSize;
            x = xNew;
        }
        nChar++;
        x += space.x;
    }
    if (!(x < xMax)) xMax = x;
    outSize.x = xMax;
    outSize.y = y - space.y;
    if (pLs) {
        pLs->y = x;
        nStyle &= 7;
        for (INT i = 0; i < nLine; i++) {
            switch (nStyle) {
            case 1:
                pLineSize[i].x = -outSize.x;
                pLineSize[i].y -= outSize.x;
                break;
            case 2:
            case 3:
                pLineSize[i].y -= outSize.x >> 1;
                pLineSize[i].x -= outSize.x >> 1;
                break;
            case 5:
                pLineSize[i].x = -pLineSize[i].y;
                pLineSize[i].y = 0;
                break;
            case 6:
            case 7: {
                FIX4 hv = pLineSize[i].y >> 1;
                pLineSize[i].x = -hv;
                pLineSize[i].y = hv;
                break;
            }
            }
        }
    }
    if (pNumCharcter) *pNumCharcter = nChar;
    return nLine;
}

UINT CFrecFont::getPacketSize(const CFntM2& prim) {
    INT nLen = prim.getLength();
    if (!nLen) {
        return 0;
    }
    UINT nStyle = prim.m_Style;
    INT nMul = 1;
    if (nStyle & 0x10) {
        nMul = 2;
    } else if (nStyle & 0x80) {
        nMul = 5;
    } else if (nStyle & 0x40) {
        nMul = 2;
    }
    return (nMul * 5 + 2) * nLen + 7;
}

USHORT CFrecFont::getFontSize(USHORT nType) {
    return nType < s_nAvailableFontTypes ? s_FontInfo[nType]._nFontSize : 0;
}

USHORT CFrecFont::getFontMaxCode(USHORT nType) {
    return nType < s_nAvailableFontTypes ? s_FontInfo[nType]._nFontCntMax : 0;
}

// SOURCE DATA: .data .rodata .bss
