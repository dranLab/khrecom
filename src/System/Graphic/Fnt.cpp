#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Graphic/Fnt.h"
#include "System/Ctd/CtdMan.h"
#include <string.h>

CFntM2::CFntM2() {
    m_Style = 0;
    m_pText = NULL;
    m_Size = 0x100;
    initialize();
}

CFntM2::CFntM2(INT nInX, INT nInY, const G_RGBA* pInColor, INT nInSize, const CHAR* lpszInText, UINT nInStyle,
               INT nInZ)
    : m_pText(NULL) {
    initialize();
    setText(lpszInText);
    m_P0.set(nInX << 4, nInY << 4, 0xFFFF280);
    m_Size = nInSize << 4;
    if (pInColor) {
        m_C0 = *pInColor;
    } else {
        m_C0.set(0x80, 0x80, 0x80, 0x80);
    }
    m_Style = nInStyle;
}

CFntM2::~CFntM2() {
    if (m_pTextBuf) com_free(m_pTextBuf);
    m_pTextBuf = NULL;
}

void CFntM2::initialize() {
    static LPCSTR p[] = {"W0", "W1", "W2", "W3"};
    m_ppWord = p;
    m_C0.set(0x80, 0x80, 0x80, 0x80);
    m_brightness.set(0x80, 0x80, 0x80, 0x80);
    m_P0.set(0, 0, 0xFFFF280);
    m_exC0.set(1, 1, 1, 0x70);
    m_pAlphaTbl = NULL;
    m_Space.x = 0;
    m_Space.y = 0;
    m_nDispChars = 0;
    m_pTextBuf = NULL;
    m_fgReplaceWord = 0;
    m_fgDispCharsCtrl = 0;
    m_nCurrentDispChars = 0;
    m_pLineXY = NULL;
    m_pIconXY = NULL;
    m_nDispLines = 0x100;
}

UINT CFntM2::getPacketSize() const {
    if (!m_pText || !*m_pText) return 0;
    INT nPrim = 1;
    INT nLength = getLength();
    if (m_Style & 0x10) {
        nPrim = 2;
    } else if (m_Style & 0x80) {
        nPrim = 5;
    } else if (m_Style & 0x40) {
        nPrim = 2;
    }
    return nLength * (nPrim * 6 + 18) + 10;
}

void CFntM2::makePkt() {}

void CFntM2::drawFont(INT inCode) {}

INT CFntM2::getSize(G_XY& outSize, G_XY* pLineSize, INT* pNumCharcter) const {
    return 0;
}

void CFntM2::getCharSize(G_XY& outSize) const {
    outSize.x = m_Size + m_Space.x;
    outSize.y = m_Size + m_Space.y;
}

void CFntM2::setText(const CHAR* lpszInStr) {
    if (!lpszInStr) return;
    if (!m_fgReplaceWord) {
        m_pText = (LPCSTR)lpszInStr;
        return;
    }
    if (CCtdMan::sprintf(NULL, 0, lpszInStr, *(const CHAR*(*)[4])m_ppWord, &m_pText) > 0) {
        if (m_pTextBuf) com_free(m_pTextBuf);
        m_pTextBuf = NULL;
        if (m_pText != lpszInStr) m_pTextBuf = m_pText;
    }
}

#line 622
void CFntM2::allocText(const CHAR* lpszInStr) {
    if (m_pTextBuf) com_free(m_pTextBuf);
    m_pTextBuf = NULL;
    if (lpszInStr && *lpszInStr) {
        m_pTextBuf = (CHAR*)com_malloc_d(strlen(lpszInStr) + 1, __FILE__ "(630)", (UINT)-1);
        strcpy(m_pTextBuf, lpszInStr);
        m_pText = m_pTextBuf;
    } else {
        m_pText = "";
    }
}

INT CFntM2::getLength() const {
    INT n = 0;
    if (m_pText) {
        const UBYTE* p = (const UBYTE*)m_pText;
        for (;;) {
            if (!*p) break;
            if ((SBYTE)*p >= 0) {
                if (*p > ' ') n++;
                p++;
            } else {
                n++;
                p += 2;
            }
        }
    }
    return n;
}

// SOURCE DATA: .data .rodata
