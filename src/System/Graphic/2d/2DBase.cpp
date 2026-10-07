#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DBase.h"
#include "System/Debug.h"

namespace D2 {

char C2DBase::m_tblID[64];

C2DBase::C2DBase() {
    m_uiID = 0xFFFF;
    m_pBaseName[0] = 0;
}

C2DBase::~C2DBase() {
    if (m_uiID < 64) {
        m_tblID[m_uiID] = 0;
    }
    TRACE_COL(_TTY_YELLOW, "C2DBase:: %s - %x \x82\xf0\x94\x6a\x8a\xfc\n", m_pBaseName, m_hBase);
}

void C2DBase::initialize() {
    INT i;
    for (i = 0; i < 64; i++) {
        m_tblID[i] = 0;
    }
}

void C2DBase::makeBaseHandle(D2BASE_TYPE d2baseType) {
    INT i;
    UINT id;
    if (m_uiID < 64) return;
    for (i = 1; i < 64; i++) {
        if (m_tblID[i] == 0) break;
    }
    if (i < 64) {
        m_tblID[i] = 1;
        m_uiID = i;
    } else {
        AssertApp(__FILE__, 57, "i < 64", 0);
    }
    id = m_uiID & 0x3F;
    m_hBase = d2baseType | (id << 24);
}

D2BASE_TYPE C2DBase::getType() {
    return (D2BASE_TYPE)(m_hBase & 0xC0000000);
}

}  // namespace D2
// SOURCE DATA: .rodata
