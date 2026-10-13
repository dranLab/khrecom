// PARTIAL: added for System/Graphic/2d/2DFnt
#ifndef SYSTEM_GRAPHIC_FNT_H
#define SYSTEM_GRAPHIC_FNT_H

#include "System/Graphic/Gprim.h"

class CFntM2 : public CGPrim {
public:
    G_XYZ m_P0;
    G_RGBA m_C0;
    G_RGBA m_exC0;
    FIX4 m_Size;
    UINT m_Style;
    G_XY m_Space;
protected:
    G_RGBA m_brightness;
    G_RGBA m_cC0;
    LPCSTR m_pText;
    LPCSTR* m_ppWord;
    CHAR* m_pTextBuf;
    INT m_oldCode;
    FIX4 m_X;
    FIX4 m_Y;
    G_XY* m_pLineXY;
    G_XY* m_pIconXY;
    BYTE* m_pAlphaTbl;
    SHORT m_nDispLines;
    SHORT m_nDispChars;
    SHORT m_fgReplaceWord;
    SHORT m_fgDispCharsCtrl;
    SHORT m_nCurrentDispChars;

public:
    CFntM2(INT nInX, INT nInY, const G_RGBA* pInColor, INT nInSize, const CHAR* lpszInText, UINT nInStyle,
           INT nInZ);
    CFntM2();
    virtual ~CFntM2();
    /* PARTIAL: added for Form/Menu/MenuParts */
    void allocText(const CHAR* pText);
    LPCSTR getText() { return m_pText; }
    void setSpace(SHORT x, SHORT y) {
        m_Space.x = x;
        m_Space.y = y;
    }
    virtual INT getSize(G_XY& outSize, G_XY* pLineSize, INT* pNumCharcter) const;
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
protected:
    virtual void drawFont(INT inCode);
};

#endif /* SYSTEM_GRAPHIC_FNT_H */
