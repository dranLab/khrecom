// PARTIAL: added for Form/DebugWindow; CBlurPrim, m_BlurPrim, getBlur added for Form/DebugWindowTBL
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
    void setShadow() { m_iStatus |= 2; } // PARTIAL: added for Game/GameSubField (name unknown)
};

class CBlurPrim : public CGPrim {
public:
    INT m_iDebugID;

private:
    INT m_iKind;
    INT m_iRate;
    FLOAT m_fLen;
    UINT m_iColor;
    UINT m_iAlpha;
    INT m_nCalcZOfs;

public:
    INT getKind() { return m_iKind; }
    INT getRate() { return m_iRate; }
    FLOAT getDistance() { return m_fLen; }
    UINT getColor() { return m_iColor; }
    UINT getAlphaModeINT() { return m_iAlpha; }
};

class CDeFocus : public CTask {
public:
    CDeFocusPrim m_DeFocusPrim;
    CBlurPrim m_BlurPrim[2];
    static CDeFocus* m_pInstance;

    CDeFocus();
    static void create(CTask* pParent); // PARTIAL: added for Game/GameSubField
    static void setShadow() {           // PARTIAL: added for Game/GameSubField (name unknown)
        if (m_pInstance) {
            m_pInstance->m_DeFocusPrim.setShadow();
        }
    }
    static CDeFocusPrim* getDeFocus() {
        if (!m_pInstance) {
            return NULL;
        }
        return &m_pInstance->m_DeFocusPrim;
    }
    static CBlurPrim* getBlur(INT i) {
        if (!m_pInstance || i >= 2) {
            return NULL;
        }
        return &m_pInstance->m_BlurPrim[i];
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
