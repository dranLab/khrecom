#ifndef SYSTEM_GRAPHIC_FNT_H
#define SYSTEM_GRAPHIC_FNT_H

#include "System/Graphic/Gprim.h"

extern INT SCREEN_HEIGHT;

/* All inline members are emitted (weak) in Fnt.cpp, in declaration order: keep the order. */
class CFntM2 : public CGPrim {
public:
    G_XYZ m_P0;          /* 0x1c */
    G_RGBA m_C0;         /* 0x28 */
    G_RGBA m_exC0;       /* 0x2c */
    FIX4 m_Size;         /* 0x30 */
    UINT m_Style;        /* 0x34 */
    G_XY m_Space;        /* 0x38 */
protected:
    G_RGBA m_brightness; /* 0x40 */
    G_RGBA m_cC0;        /* 0x44 */
    LPCSTR m_pText;      /* 0x48 */
    LPCSTR* m_ppWord;    /* 0x4c */
    CHAR* m_pTextBuf;    /* 0x50 */
    INT m_oldCode;       /* 0x54 */
    FIX4 m_X;            /* 0x58 */
    FIX4 m_Y;            /* 0x5c */
    G_XY* m_pLineXY;     /* 0x60 */
    G_XY* m_pIconXY;     /* 0x64 */
    BYTE* m_pAlphaTbl;   /* 0x68 */
    SHORT m_nDispLines;  /* 0x6c */
    SHORT m_nDispChars;  /* 0x6e */
    SHORT m_fgReplaceWord;     /* 0x70 */
    SHORT m_fgDispCharsCtrl;   /* 0x72 */
    SHORT m_nCurrentDispChars; /* 0x74 */

public:
    CFntM2(INT nInX, INT nInY, const G_RGBA* pInColor, INT nInSize, const CHAR* lpszInText, UINT nInStyle,
           INT nInZ);
    CFntM2();
    virtual ~CFntM2();
    void setPosition(INT x, INT y, INT z) { m_P0.set(x << 4, y << 4, z << 4); }
    void setPositionF(FLOAT x, FLOAT y) { m_P0.set((FIX4)x, (FIX4)y, 0xFFFF280); }
    void setScreenPosition(INT x, INT y, INT z) { m_P0.set((x - 256) << 4, (y - SCREEN_HEIGHT / 2) << 4, z << 4); }
    void setColor(UBYTE r, UBYTE g, UBYTE b) { m_C0.set(r, g, b); }
    void setColor(UBYTE r, UBYTE g, UBYTE b, UBYTE a) { m_C0.set(r, g, b, a); }
    void setColor(const G_RGBA& inColor) { m_C0 = inColor; }
    void setBrightness(UBYTE r, UBYTE g, UBYTE b, UBYTE a) { m_brightness.set(r, g, b, a); }
    void setBrightness(const G_RGBA& inBrightness) { m_brightness = inBrightness; }
    UINT getBrightness() const { return m_brightness.col; }
    void setExColor(const G_RGBA& inColor) { m_exC0 = inColor; }
    void setText(const CHAR* lpszInStr);
    void allocText(const CHAR* lpszInStr);
    void setStyle(UINT nInStyle) { m_Style = nInStyle; }
    void addStyle(UINT nInStyle) { m_Style |= nInStyle; }
    void deleteStyle(UINT nInStyle) { m_Style &= ~nInStyle; }
    void setSize(INT nInSize) { m_Size = nInSize << 4; }
    virtual INT getSize(G_XY& outSize, G_XY* pLineSize, INT* pNumCharcter) const;
    void getCharSize(G_XY& outSize) const;
    virtual UINT getPacketSize() const;
    void setDispLines(INT nDispLines) { m_nDispLines = nDispLines; }
    SHORT getDispLines() const { return m_nDispLines; }
    void setDispChars(INT nDispChars) { m_nDispChars = nDispChars; }
    SHORT getDispChars() const { return m_nDispChars; }
    void enableWordReplace() { m_fgReplaceWord = 1; }
    void setWordList(const CHAR** ppWord) { m_ppWord = (LPCSTR*)ppWord; }
    void setDispCharsCtrl(SHORT fg) { m_fgDispCharsCtrl = fg; }
    SHORT getDispCharsCtrl() const { return m_fgDispCharsCtrl; }
    void resetDispCharsCtrl() { m_fgDispCharsCtrl = -1; }
    void setLineXYPtr(G_XY* pLineXY) { m_pLineXY = pLineXY; }
    G_XY* getLineXYPtr() const { return m_pLineXY; }
    void setIconXYPtr(G_XY* pIconXY) { m_pIconXY = pIconXY; }
    G_XY* getIconXYPtr() const { return m_pIconXY; }
    LPCSTR getText() const { return m_pText; }
    void setCurrentDispChars(SHORT n) { m_nCurrentDispChars = n; }
    SHORT getCurrentDispChars() const { return m_nCurrentDispChars; }
    INT getLength() const;
    void setAlphaTbl(const UBYTE* p) { m_pAlphaTbl = (BYTE*)p; }
    const UBYTE* getAlphaTbl() const { return (const UBYTE*)m_pAlphaTbl; }
    void setSpace(INT x, INT y) {
        m_Space.set(x, y);

    }
private:
    virtual FIX4 calcZ() { return m_P0.z; }
    virtual void makePkt();
    void initialize();
protected:
    virtual void drawFont(INT inCode);
};

#endif /* SYSTEM_GRAPHIC_FNT_H */
