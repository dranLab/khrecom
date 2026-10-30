// PARTIAL: added for Form/DebugWindow; CBlurPrim, m_BlurPrim, getBlur added for Form/DebugWindowTBL
#ifndef SYSTEM_GRAPHIC_DEFOCUS_H
#define SYSTEM_GRAPHIC_DEFOCUS_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"

#ifdef SYSTEM_GRAPHIC_DEFOCUS_CPP /* MATCH: DebugWindow/GameSubField/GameSubBattle still use the partial classes below (guessed setManual()/setShadow, public m_pInstance) */
/* The complete classes, for System/Graphic/DeFocus (which emits every inline member, weak, in declaration order).
 * Other includers use the partial classes below. */
class CZFill : public CGPrim { /* System/Graphic/Gprim.h */
public:
    INT m_nZ;
    CZFill() { m_nZ = 0; }
    virtual UINT getPacketSize() const;
private:
    virtual void makePkt();
};

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
    CDeFocusPrim();
    void setAutomatic() { m_iStatus &= ~4; }
    void setManual(FLOAT fNear, FLOAT fFar) {
        m_fFar = fFar;
        m_fNear = fNear;
        m_iStatus |= 4;
    }
    FLOAT getNear() { return m_fNear; }
    FLOAT getFar() { return m_fFar; }
    void push() {
        if (!(m_iStatus & 0x100)) {
            _Bak = _Now;
            m_iStatus |= 0x100;
        }
    }
    void pop() {
        if (m_iStatus & 0x100) _Now = _Bak;
    }
    void off() { m_iStatus |= 3; }
    void on() { m_iStatus &= ~3; }
    void offNear() { m_iStatus |= 2; }
    void onNear() { m_iStatus &= ~2; }
    void offFar() { m_iStatus |= 1; }
    void onFar() { m_iStatus &= ~1; }
    void stop() { m_iStatus |= 0x10000; }
    void start() { m_iStatus &= ~0x10000; }
    virtual UINT getPacketSize() const { return 0x1fd; }
private:
    void init() {}
    virtual FIX4 calcZ();
    virtual void makePkt();
    void _makePktDeFocus();
};

class CBlurPrim : public CGPrim {
public:
    INT m_iDebugID;

private:
    INT m_iKind;
    INT m_iRate;
    FLOAT m_fLen;
    G_RGBA m_iColor;
    UINT m_iAlpha;
    FIX4 m_nCalcZOfs;

public:
    CBlurPrim();
    void setKind(INT iKind) { m_iKind = iKind; }
    void setRate(INT iRate) { m_iRate = iRate; }
    void setDistance(FLOAT fDistance) { m_fLen = fDistance; }
    void setColor(UINT iColor) { m_iColor.col = iColor; }
    void setColor(UINT iR, UINT iG, UINT iB, UINT iA) {
        m_iColor.set(iR, iG, iB, iA);
    }
    void setAlphaMode(ULONGLONG lAlpha) { m_iAlpha = (lAlpha >> 16) | lAlpha; }
    INT getKind() { return m_iKind; }
    INT getRate() { return m_iRate; }
    FLOAT getDistance() { return m_fLen; }
    UINT getColor() { return m_iColor.col; }
    ULONGLONG getAlphaMode() { return ((ULONGLONG)m_iAlpha << 16) | m_iAlpha; }
    UINT getAlphaModeINT() { return m_iAlpha; }
    virtual UINT getPacketSize() const {
        UINT nSize = 0x48;
        if (!m_iKind) return 0;
        /* the original multiplies the 0x48 held in a register (not folded into the default arm): an empty asm
         * hides the constant from CSE (original form unknown) */
        __asm__("" : "=r"(nSize) : "0"(nSize));
        if (m_iKind == 1 || m_iKind == 2) {
            nSize = 0x120;
        } else if (m_iKind == 5 || m_iKind == 6) {
            nSize = 0x240;
        } else {
            UINT nNum = m_iRate * 3;
            UINT nTmp = nNum * nSize;
            nSize = nTmp;
        }
        return (nSize * 16 + 0x250) >> 4;
    }
    void setCalcZOfs(FIX4 nZ) { m_nCalcZOfs = nZ; }
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
    void _makePktBlur();
};

struct BLUR_PARAM {
    FLOAT m_fBlurTotalTime;
    FLOAT m_fBlurNowTime;
    FLOAT m_fBlurAimRate;
    FLOAT m_fBlurOldRate;
};

class CDeFocus : public CTask {
private:
    CDeFocusPrim m_DeFocusPrim;
    CBlurPrim m_BlurPrim[2];
    CZFill m_ZFill;
    BLUR_PARAM m_sBlurParam[2];
    static CDeFocus* m_pInstance;

public:
    CDeFocus(CTask* p);
    virtual ~CDeFocus();
    virtual void main();
    virtual void disp();
    static CDeFocusPrim* getDeFocus() {
        if (!m_pInstance) return NULL;
        return &m_pInstance->m_DeFocusPrim;
    }
    static CBlurPrim* getBlur(INT i) {
        if (!m_pInstance || i >= 2) return NULL;
        return &m_pInstance->m_BlurPrim[i];
    }
    static void setAutomatic() {
        if (m_pInstance) m_pInstance->m_DeFocusPrim.setAutomatic();
    }
    static void setManual(FLOAT fNear, FLOAT fFar) {
        CDeFocus*& p = m_pInstance; if (p) p->m_DeFocusPrim.setManual(fNear, fFar); // PARTIAL: reference read (Game/Atel/VSME: the load stays after stores)
    }
    static FLOAT getNear() {
        if (m_pInstance) return m_pInstance->m_DeFocusPrim.getNear();
        return 0.0f;
    }
    static FLOAT getFar() {
        if (m_pInstance) return m_pInstance->m_DeFocusPrim.getFar();
        return 0.0f;
    }
    static void push() {
        if (m_pInstance) m_pInstance->m_DeFocusPrim.push();
    }
    static void pop() {
        if (m_pInstance) m_pInstance->m_DeFocusPrim.pop();
    }
    static void off() {
        if (m_pInstance) m_pInstance->m_DeFocusPrim.off();
    }
    static void on() {
        if (m_pInstance) m_pInstance->m_DeFocusPrim.on();
    }
    static void offNear() {
        if (m_pInstance) m_pInstance->m_DeFocusPrim.offNear();
    }
    static void onNear() {
        if (m_pInstance) m_pInstance->m_DeFocusPrim.onNear();
    }
    static void offFar() {
        if (m_pInstance) m_pInstance->m_DeFocusPrim.offFar();
    }
    static void onFar() {
        if (m_pInstance) m_pInstance->m_DeFocusPrim.onFar();
    }
    static void BlurSet(INT iKind, FLOAT fRate, FLOAT fTime, INT iNum);
    static void BlurColorSet(INT iR, INT iG, INT iB, INT iA, ULONGLONG lAlpha, INT iNum);
    static void BlurDistanceSet(FLOAT fDistance, INT iNum);
    static void BlurSetEvent(INT iNum);
    static void BlurReturnEvent();
    static void create(CTask* p);
    static void remove() {
        if (m_pInstance) m_pInstance->out();
    }

private:
    INT _BlurUpdate(FLOAT fr);
};

#else
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

#endif /* SYSTEM_GRAPHIC_DEFOCUS_CPP */

#endif /* SYSTEM_GRAPHIC_DEFOCUS_H */
