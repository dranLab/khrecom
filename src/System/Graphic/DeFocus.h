// PARTIAL: added for Form/DebugWindow
#ifndef SYSTEM_GRAPHIC_DEFOCUS_H
#define SYSTEM_GRAPHIC_DEFOCUS_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"

class CDeFocusPrim : public CGPrim {
private:
    union {
        u_long128 _Now;
        struct {
            UINT m_iStatus;
            FLOAT m_fNear;
            FLOAT m_fFar;
            int Pad32[1];
        };
    };
    u_long128 _Bak;

public:
    void setAutomatic() { m_iStatus &= ~0x10000; }
    void setManual() { m_iStatus |= 0x10000; }
};

class CDeFocus : public CTask {
public:
    CDeFocusPrim m_DeFocusPrim;
    static CDeFocus* m_pInstance;

    CDeFocus();
    static CDeFocusPrim* getDeFocus() {
        if (!m_pInstance) {
            return NULL;
        }
        return &m_pInstance->m_DeFocusPrim;
    }
    static void setAutomatic() {
        CDeFocusPrim* p = getDeFocus();
        if (p) {
            p->setAutomatic();
        }
    }
    static void setManual() {
        CDeFocusPrim* p = getDeFocus();
        if (p) {
            p->setManual();
        }
    }
};

#endif /* SYSTEM_GRAPHIC_DEFOCUS_H */
