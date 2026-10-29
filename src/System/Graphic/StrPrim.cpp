#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h"
#include "System/Graphic/StrPrim.h"

#include "System/Graphic/FrecFont.h"

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
