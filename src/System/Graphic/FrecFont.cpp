#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/PackMan.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/Graphic/FrecFont.h"
#include "System/Graphic/Fnt.h"
#include "System/Graphic/TexUtil.h"
#include "System/Buf.h"
#include "System/VramMan.h"
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
    for (INT i = 0; i < nDiv; i++) {
        VTransInfo& tex = t._TexInfo;
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
        ULONGLONG nCSA = n & 1U;
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
        qp[0] = _makeQW(font._nTex0[nPage][palette], 7);
        qp[1] = _makeQW(_uv(t0), 3);
        qp[2] = _makeQW(_xyz(p0), 5);
        qp[3] = _makeQW(_uv(t1), 3);
        qp[4] = _makeQW(_xyz(p1), 5);
        qp += 5;
    }
}

#line 405
/* The original's inlines at 408-428 (.mdebug: w, space, bHalfSpaceExt; also in getDispSize): x moved by a tab, a space or
 * SJIS 0x8140. Names ours. x by reference (an ADDRESSOF, so not in .mdebug): makePkt's copy after the width ternary and
 * the missing _x of its G_XY::set need x's address taken (tested with a bare &x); n is the half-space count. */
static inline void _addHalfSpace(FIX4& x, FIX4 w, FIX4 space, BOOL bHalfSpaceExt, INT n) {
    if (!bHalfSpaceExt) {

        x += ((w >> 1) + space) * n;
    } else {


        x += (w / 2 + space) * n;
    }
}

static inline void _addSpace(FIX4& x, FIX4 w, FIX4 space, BOOL bHalfSpaceExt) {
    if (!bHalfSpaceExt) {

        x += (w >> 1) - (w >> 3) + space;
    } else {


        x += (w >> 1) - (w >> 3) + space;
    }
}






void CFrecFont::makePkt(CFntM2& prim) {
    UINT nStyle = prim.m_Style;
    INT nFontType = (nStyle & 0x1000) >> 12;
    BOOL bDebugMode = nStyle & 0x8000;
    USHORT nPalette = 0;

    if (nFontType) {

        if (!(nStyle & 0x10000)) {
            if (nStyle & 0x2000) nPalette = 1;
            else if (nStyle & 0x20000) nPalette = 2;
            else if (nStyle & 0x40000) nPalette = 3;
            else nPalette = 4;
        }
    }


    if (nFontType >= s_nAvailableFontTypes) return;
    BOOL bEnableColor = nFontType == 0;

    _FontInfo& font = s_FontInfo[nFontType];
    if (!font._pSjisCnvTbl) return;
    G_RGBA colBase, colExt, colBak;

    UINT brightness = prim.getBrightness();
    if (bEnableColor) {

        colBase.col = _mulColor(prim.m_C0.col, brightness);
        colExt.col = _mulColor(prim.m_exC0.col, brightness);
    } else {


        INT a = (prim.m_C0.a * (brightness >> 24)) >> 7;
        if (a > 0xFF) a = 0xFF;

        colBase.col = (a << 24) | 0x808080;
        colExt.col = 0x80808080;
    }
    BYTE* pAlphaTbl = (BYTE*)prim.getAlphaTbl();
    BYTE nExtAlpha, nExtAlphaOld;
    if (pAlphaTbl) {

        nExtAlpha = pAlphaTbl[0];
        colBase.a = nExtAlpha;
        colExt.a = nExtAlpha;
    }
    colBak.col = colBase.col;
    BYTE* pStr = (BYTE*)prim.getText();

    FIX4 x = prim.m_P0.x;
    FIX4 y = prim.m_P0.y - prim.m_Size;
    FIX4 w = prim.m_Size;
    FIX4 h = w;
    INT nBaseSize = font._nFontSize;
    BOOL bIsBaseW = (w >> 4) == nBaseSize;
    G_XY space = prim.m_Space;
    G_XY* pLineXY = prim.getLineXYPtr();
    G_XY* pIconXY = prim.getIconXYPtr();
    FIX4 x0, xOld;
    INT nIcon = 0;

    if (!pLineXY) {

        if (nStyle & 3) {
            G_XY wh;

            getDispSize(wh, prim, NULL, NULL);
            if (nStyle & 2) x -= wh.x >> 1;
            else x -= wh.x;
        }
        x0 = xOld = x;

    } else {

        x0 = xOld = x;
        x += pLineXY[0].x;
    }

    BOOL bHalfSpaceExt = !(nStyle & 0x9000);

    nStyle &= 0xD0;
    UINT nStyleBak = nStyle;
    INT nLine = 0;
    INT nChar = 0;
    u_long128* qp;
    USHORT nSJIS;




    qp = CPackMan::pkOpenGifTag(_makeQW(0x1000800000008000UL, 0xE));
    qp[0] = _makeQW(5, 9);
    qp[1] = _makeQW(s_nTEX2_2, 0x17);
    qp[2] = _makeQW(0x356, 0);
    qp[3] = _makeQW(colBase.col, 1);
    qp += 4;


    while (*pStr) {

        USHORT nH = *pStr++;
        if (pAlphaTbl) nExtAlpha = pAlphaTbl[nChar];

        if (nH < 0x80) {

            if (nH <= 0x20) {

                switch (nH) {
                case 9:
                    _addHalfSpace(x, prim.m_Size, space.x, bHalfSpaceExt, 4); break;
                case 0x20:
                    _addSpace(x, prim.m_Size, space.x, bHalfSpaceExt); break;
                case 10:
                    nLine++;
                    x = x0;
                    if (pLineXY) x += pLineXY[nLine].x;
                    y += h + space.y;
                    break;
                }
                if (nLine >= prim.getDispLines()) break;
                continue;
            }



            if (bDebugMode) w = h >> 1;
            nSJIS = nH;
        } else {

            USHORT nL = *pStr;
            if (nL) pStr++;

            if (nH >= 0xF9) {

                switch (nL) {
                case 0x40:
                    x = xOld; break;

                case 0x41:
                    if (bEnableColor) {

                        colBase.col = colBak.col;
                        *qp = _makeQW(colBase.col, 1);
                        qp++;
                    }
                    break;
                case 0x42:
                    x -= 2 + space.x; break;

                case 0x43: case 0x44:
                case 0x45:
                    x += 0x10 << (nL - 0x43); break;

                case 0x46: case 0x47:
                    nPalette = nL - 0x46; break;
                case 0x48:
                    nStyle = nStyleBak; break;
                case 0x49:
                    prim.resetDispCharsCtrl(); break;
                default:

                    if (nL < 0x50) {

                    } else if (nL < 0x60) {

                        if (bEnableColor) {

                            colBase.col = _mulColor(*(s_fntFixColor + nL - 0x50), brightness);
                            *qp = _makeQW(colBase.col, 1);
                            qp++;
                        }

                    } else if (nL < 0x70) {

                        nStyle = 0x10 << (nL - 0x60);

                    } else if (nL < 0x80) {

                        if (prim.getCurrentDispChars() < nChar) {



                            prim.setDispCharsCtrl(nL - 0x6F);
                            prim.setDispChars(nChar); }
                    } else {
                        if (pIconXY) pIconXY[nIcon++].set(x, y + h);

                        if (nL < 0x98) x += 0x200;

                        else if (nL < 0xA0) x += 0x280;

                        else x += 0x180;
                    }
                    break;
                }
                continue;
            }

            nSJIS = (nH << 8) | nL;

            if (nSJIS == 0x8140) {

                _addHalfSpace(x, prim.m_Size, space.x, FALSE, 2);
                continue;
            }
            if (bDebugMode) w = h;
        }
        USHORT nCode = _fromSJIS(font, nSJIS);
        USHORT nPPW = font._pFontWidthTbl[nCode];
        if (nPPW < nBaseSize) nPPW++;
        USHORT s = 0; if (bDebugMode) s = nPPW;
        xOld = x;
        if (nStyle && bEnableColor) {

            if (nStyle & 0x10) {

                _makeFontPkt(font, nCode, x + (w >> 4), y, w, h, qp, s, 0);

            } else {

                FIX4 ySave = y;
                G_RGBA colSave = colBase;
                if (nStyle & 0x80) {

                    colBase.col = colExt.col;
                    *qp = _makeQW(colBase.col, 1);
                    qp++;
                    x -= w >> 4;
                    y -= h >> 4; _makeFontPkt(font, nCode, x, y, w, h, qp, s, 0);
                    x += w >> 3; _makeFontPkt(font, nCode, x, y, w, h, qp, s, 0);
                    y += h >> 3; _makeFontPkt(font, nCode, x, y, w, h, qp, s, 0);
                    x -= w >> 3;
                } else {


                    colBase.col = colExt.col;
                    *qp = _makeQW(colBase.col, 1);
                    qp++;
                    x += w >> 3;
                    y += h >> 3;
                }
                _makeFontPkt(font, nCode, x, y, w, h, qp, s, 0);
                y = ySave;
                colBase.col = colSave.col;
                *qp = _makeQW(colBase.col, 1);
                qp++;
            }
            x = xOld;
        }
        if (pAlphaTbl) {

            if (nExtAlpha != nExtAlphaOld) {

                nExtAlphaOld = nExtAlpha;
                colBase.a = nExtAlpha;
                colExt.a = nExtAlpha;
                colBak.col = colBase.col;
                *qp = _makeQW(colBase.col, 1);
                qp++;
            }
        }

        _makeFontPkt(font, nCode, x, y, w, h, qp, s, nPalette);


        if (bDebugMode) x += w;
        else x = bIsBaseW ? x + (nPPW << 4) : x + nPPW * w / nBaseSize;
        x += space.x;

        nChar++;
        if (prim.getDispChars() && nChar >= prim.getDispChars()) break;
    }

    *qp = _makeQW(CPackMan::getDefault_CLAMP_2(), 9);
    qp++;
    CPackMan::pkCloseGifTag(qp);

    prim.setCurrentDispChars(nChar);





}
#line 675

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
    FIX4 x, xMax, xOld;
    x = xMax = xOld = 0;
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
    USHORT nSJIS;
    while (*pStr) {
        USHORT nH = *pStr++;
        if (nH < 0x80) {
            if (nH <= 0x20) {
                switch (nH) {
                case 9:
                    _addHalfSpace(x, prim.m_Size, space.x, bHalfSpaceExt, 4);
                    break;
                case 0x20:
                    _addSpace(x, prim.m_Size, space.x, bHalfSpaceExt);
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
                nSJIS = nH;
            }
        } else {
            USHORT nL = *pStr;
            if (nL) pStr++;
            if (nH >= 0xF9) {
                switch (nL) {
                case 0x40:
                    x = xOld;
                    break;
                case 0x42:
                    x -= 2 + space.x;
                    break;
                case 0x43:
                case 0x44:
                case 0x45:
                    x += 0x10 << (nL - 0x43);
                    break;
                default:
                    if (nL >= 0x80) {
                        if (nL < 0x98) x += 0x200;
                        else if (nL < 0xA0) x += 0x280;
                        else x += 0x180;
                    }
                    break;
                }
                continue;
            }
            nSJIS = (nH << 8) | nL;
            if (nSJIS == 0x8140) {
                _addHalfSpace(x, prim.m_Size, space.x, FALSE, 2);
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
            x = bIsBaseW ? x + (nPPW << 4) : x + nPPW * w / nBaseSize;
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
            case 7:
                pLineSize[i].y >>= 1;
                pLineSize[i].x = -pLineSize[i].y;
                break;
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
