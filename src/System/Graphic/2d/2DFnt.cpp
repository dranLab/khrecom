#include "System/Graphic/2d/2DFnt.h"

namespace D2 {

C2DFnt::C2DFnt() {}

C2DFnt::C2DFnt(INT nInX, INT nInY, const G_RGBA* pInColor, INT nInSize, const CHAR* lpszInText, UINT nInStyle)
    : CStrPrim(nInX, nInY, pInColor, nInSize, lpszInText, nInStyle, 0xFFFF28) {
    m_seqFntSize = nInSize << 4;
}

C2DFnt::~C2DFnt() {}

void C2DFnt::drawFont(INT nInCode) {
    CFntM2::drawFont(nInCode);
}

}  // namespace D2

// SOURCE DATA: .rodata
