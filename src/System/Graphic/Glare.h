#ifndef SYSTEM_GRAPHIC_GLARE_H
#define SYSTEM_GRAPHIC_GLARE_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"

class CGlarePrim : public CGPrim {
private:
    INT m_iFlags; /* 0x1c */
    INT m_iAlpha; /* 0x20 */
    INT m_iKey;   /* 0x24 */

public:
    CGlarePrim();
    virtual ~CGlarePrim() {}
    virtual UINT getPacketSize() const { return 0x1FD; }
    void setKey(INT iKey) { m_iKey = iKey; }
    void setAlpha(INT iA) { m_iAlpha = iA; }
    void setFlag(INT iFlags) { m_iFlags |= iFlags; }
    void resetFlag(INT iFlags) { m_iFlags &= ~iFlags; }
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

class CGlare : public CTask {
private:
    INT m_iFlags;          /* 0x40 */
    FLOAT m_fRate;         /* 0x44 */
    INT m_iKey;            /* 0x48 */
    CGlarePrim* m_pGlare;  /* 0x4c */
    FLOAT m_fPar;          /* 0x50 */
    FLOAT m_fAlpha;        /* 0x54 */
    FLOAT m_fTime;         /* 0x58 */
    FLOAT m_fAOld;         /* 0x5c */
    FLOAT m_fAAim;         /* 0x60 */
    static CGlare* m_pInstance;

public:
    CGlare(CTask* p);
    virtual ~CGlare();
    virtual void main();
    virtual void disp();
    static void fade(FLOAT fTime, FLOAT fAim, FLOAT fOld) {
        CGlare*& rInstance = m_pInstance;
        if (rInstance) rInstance->_fade(fTime, fAim, fOld);
    }
    static void fadein(FLOAT fTime) { fade(fTime, 1.0f, -1.0f); }
    static void fadeout(FLOAT fTime) { fade(fTime, 0.0f, -1.0f); }
    static BOOL isFade() { return m_pInstance && m_pInstance->m_fPar < 1.0f; }
    static void off(FLOAT fTime) {
        if (m_pInstance) m_pInstance->_off(fTime);
    }
    static void on(FLOAT fTime) {
        if (m_pInstance) m_pInstance->_on(fTime);
    }
    static void checkZ(BOOL bVal) {
        if (m_pInstance) m_pInstance->_checkZ(bVal);
    }
    static void setAlphaKey(INT iKey) {
        if (m_pInstance) m_pInstance->_setAlphaKey(iKey);
    }
    static void create(CTask* p);
    static void remove() {
        if (m_pInstance) m_pInstance->out();
    }
    static void sysOn(FLOAT fRate) {
        if (m_pInstance) m_pInstance->_sysOn(fRate);
    }

private:
    void _fade(FLOAT fTime, FLOAT fAim, FLOAT fOld);
    void _off(FLOAT fTime);
    void _on(FLOAT fTime);
    void _sysOn(FLOAT fRate);
    void _checkZ(BOOL bVal);
    void _setAlphaKey(INT iKey);
    void _request();
};

#endif /* SYSTEM_GRAPHIC_GLARE_H */
