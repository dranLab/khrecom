// PARTIAL: added for System/Graphic/2d/2DFnt
#ifndef SYSTEM_GRAPHIC_STRPRIM_H
#define SYSTEM_GRAPHIC_STRPRIM_H

#include "System/Graphic/Fnt.h"

class CStrPrim : public CFntM2 {
public:
    CStrPrim() {}
    CStrPrim(INT nX, INT nY, const G_RGBA* pColor, INT nSize, const CHAR* pszText, UINT nStyle, INT nZ)
        : CFntM2(nX, nY, pColor, nSize, pszText, nStyle, nZ) {}
    virtual ~CStrPrim() {}
    virtual UINT getPacketSize() const;
    virtual INT getSize(G_XY& outSize, G_XY* pLineSize, INT* pNumCharcter) const;
    void setFontType(INT nType) { m_Style = (m_Style & ~0x1000) | (nType & 0x1000); }
    INT getFontType() const { return (m_Style & 0x1000) >> 12; }
private:
    virtual void makePkt();
};

#endif /* SYSTEM_GRAPHIC_STRPRIM_H */
