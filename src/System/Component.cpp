
#include "System/Component.h"

// Build stamp at the head of .rodata: no symbol in the original, pointed to by __BUILD (.sdata)
asm(".section .rodata\n.align 3\n.globl D_0045AB00\nD_0045AB00:\n.asciz \"EEDO 2008/10/01 18:29:10.65 xxx\"\n.previous");

INT CComponentObj::getTypeID() const { return 0x100; }

void CComponentObj::setPosition(const CPointI& inPos) {
    CComponent::setPosition(inPos);
    for (SLContainerT<CTask>* pContainer = getChildren().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CTask* p = pContainer->getValue();
        if (pContainer && p->getTypeID() == 0x100) {
            setPosition(getPosition() + ((CComponentObj*)p)->getCompPosition());
        }
    }
}

void CComponentObj::setCompPosition(const CPointI& inPos) {
    m_offset = inPos;
    CTask* p = getParent();
    if (p && p->getTypeID() == 0x100) {
        CComponentObj* pObj = (CComponentObj*)p;
        setPosition(m_offset + pObj->getPosition());
    }
}

void CComponentObj::main() {
    if (isActive() && isVisible()) {
        CTask* p = getParent();
        if (p) {
            if (p->getTypeID() == 0x100) {
                if (p->isInactive() || p->isInvisible()) {
                    return;
                }
            }
            mainImpl();
        }
    }
}

void CComponentObj::disp() {
    if (m_bIgnoreKey) {
        m_bIgnoreKey = FALSE;
    }
    if (isVisible()) {
        CTask* p = getParent();
        if (p) {
            if (p->getTypeID() == 0x100) {
                if (p->isInvisible()) {
                    return;
                }
            }
            dispImpl();
        }
    }
}

// SOURCE DATA: .rodata
