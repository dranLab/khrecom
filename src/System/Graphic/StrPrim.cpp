#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h"
#include "System/Graphic/StrPrim.h"

class CFrecFont {
public:
    static void makePkt(CFntM2& fnt);
    static INT getDispSize(G_XY& outSize, const CFntM2& fnt, G_XY* pLineSize, INT* pNumCharcter);
    static UINT getPacketSize(const CFntM2& fnt);
};

UINT CStrPrim::getPacketSize() const {
    return CFrecFont::getPacketSize(*this);
}

void CStrPrim::makePkt() {
    if (m_pText && *m_pText) {
        CFrecFont::makePkt(*this);
    }
}

INT CStrPrim::getSize(G_XY& outSize, G_XY* pLineSize, INT* pNumCharcter) const {
    return CFrecFont::getDispSize(outSize, *this, pLineSize, pNumCharcter);
}

// SOURCE DATA: .rodata
