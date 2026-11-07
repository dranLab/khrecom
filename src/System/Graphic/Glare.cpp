
#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Graphic/Glare.h"
#include "System/VramMan.h"
#include "Game/Debug/DebugWork.h"
#include "include_asm.h"

inline void* operator new(size_t, void* p) throw() { return p; }


#include "System/Graphic/PackMan.h"
#include "System/HeapMan.h"





extern INT SCREEN_HEIGHT;
QWdata* set_csprite64(QWdata* qp, INT x, INT y, UINT col, INT w, INT h);
QWdata* set_tsprite32(QWdata* qp, INT x, INT y, UINT col, INT u, INT v, INT w, INT h);

CGlare* CGlare::m_pInstance = NULL;

#line 26
void CGlare::create(CTask* p) {
    if (!m_pInstance) {
        m_pInstance = new (__FILE__ "(28)") CGlare(p);
        if (!m_pInstance) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
    }
}

#line 39
CGlare::CGlare(CTask* p) : CTask(p, 0x4000) {
    m_pGlare = new (__FILE__ "(40)") CGlarePrim;
    if (!m_pGlare) AssertApp(__FILE__, __LINE__, "m_pGlare", 0);
    m_iFlags = 8;
    m_iKey = 0xFF;
    m_fRate = 0;
    m_fPar = 1.0f;
    m_fAlpha = 0;
    m_fTime = 1.0f;
    m_fAOld = 1.0f;
    m_fAAim = 1.0f;
}

CGlare::~CGlare() {
    if (m_pGlare) delete m_pGlare;
    m_pGlare = NULL;
    CGlare*& rInstance = m_pInstance;
    if (rInstance == this) rInstance = NULL;
}

void CGlare::_sysOn(FLOAT fRate) {
    m_fRate = fRate;
    m_iFlags |= 1;
    m_fPar = 1.0f;
    m_fAlpha = fRate;
}

void CGlare::_checkZ(BOOL bVal) {
    if (m_iFlags & 4) {
        if (bVal) m_pGlare->setFlag(1);
        else m_pGlare->resetFlag(1);
    } else {
        if (bVal) m_iFlags |= 8;
        else m_iFlags &= ~8;
    }
}

void CGlare::_setAlphaKey(INT iKey) {
    if (m_iFlags & 4) m_pGlare->setKey(iKey);
    else m_iKey = iKey;
}

void CGlare::_request() {
    if (m_iFlags & 2) {
        m_iFlags = (m_iFlags & ~2) | 4;
        _checkZ(TRUE);
        _setAlphaKey(0xFF);
    }
}

void CGlare::_off(FLOAT fTime) {
    m_iFlags |= 2;
    fadeout(fTime);
}

void CGlare::_on(FLOAT fTime) {
    INT iFlags = m_iFlags;
    m_iFlags = iFlags & ~6;
    if (iFlags & 1) {
        fade(fTime, m_fRate, -1.0f);
    } else {
        fadeout(fTime);
    }
}

void CGlare::_fade(FLOAT fTime, FLOAT fAim, FLOAT fOld) {
    if (fTime < 1.0f) {
        fTime = 1.0f;
        m_fAlpha = fAim;
        m_fPar = fTime;
        _request();
    } else {
        m_fPar = 0;
    }
    m_fTime = 1.0f / fTime;
    m_fAAim = fAim;
    if (fOld >= 0.0f) {
        m_fAOld = fOld;
    } else {
        m_fAOld = m_fAlpha;
    }
}

void CGlare::main() {
    if (m_fPar < 1.0f) {
        m_fPar += m_fTime * getFrameRateF();
        if (m_fPar >= 1.0f) {
            m_fPar = 1.0f;
            m_fAlpha = m_fAAim;
            _request();
        } else {
            m_fAlpha = (m_fAAim - m_fAOld) * m_fPar + m_fAOld;
        }
    }
}

void CGlare::disp() {
    FLOAT fAlphaPer = m_fAlpha;
    if (DebugWork::g_dw.sys.m_iGlare == 1) fAlphaPer = DebugWork::g_dw.sys.m_fGlareMaxPer;
    m_pGlare->setAlpha((INT)(fAlphaPer * 128.0f));
    if (!(m_iFlags & 4)) {
        if (m_iFlags & 8) m_pGlare->setFlag(1);
        else m_pGlare->resetFlag(1);
        m_pGlare->setKey(m_iKey);
    }
    m_pGlare->disp();
}

CGlarePrim::CGlarePrim() {
    m_iFlags = 1;
    m_iKey = 0xFF;
    m_iAlpha = 0;
    setLayer(6, 0);
}

FIX4 CGlarePrim::calcZ() {
    if (m_iAlpha < 1) return (FIX4)0xF0000000;
    return 0x0FFFFC00;
}

static inline void _setQW(QWdata* qp, u_long data, u_long addr) {
    u_long128 out128;
    __asm__ volatile("pcpyld %0, %1, %0" : "=r"(out128) : "r"(addr), "0"(data));
    *(volatile u_long128*)qp = out128;
}

static QWdata* setTspriteNoSlice(QWdata* qp, FIX4 ofsx, FIX4 ofsy, FIX4 u0, FIX4 v0, FIX4 u1, FIX4 v1) {
    _setQW(qp, (u_long)u0 | ((u_long)v0 << 16), 3);
    qp++;
    _setQW(qp, (u_long)(ofsx + 0x7000) | ((u_long)(((0x800 - SCREEN_HEIGHT / 2) << 4) + ofsy) << 16), 4);
    qp++;
    _setQW(qp, (u_long)u1 | ((u_long)v1 << 16), 3);
    qp++;
    _setQW(qp, (u_long)(ofsx + 0x8000) | ((u_long)(ofsy + 0x8000) << 16), 4);
    qp++;
    return qp;
}

/* not matching, 382 of 629 lines (2026-11-07; needs `qp = (QWdata*)pkOpenGifTag` and the two extern statics below dropped to
 * build): register allocation differs (the original keeps the -1 of its divisions by 32 in a saved register from the start); with it the function-local statics ofsX/ofsY, so .data comes from the original assembly */
/* makePkt's function-local statics, for its INCLUDE_ASM; remove with it. */
FIX4 ofsX_asm __asm__("ofsX.1914") = -80;
FIX4 ofsY_asm __asm__("ofsY.1915") = -16;
#line 173
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Glare", makePkt__10CGlarePrim);
#if 0
void CGlarePrim::makePkt() {
    INT iAlpha;
    QWdata* qp;
    static FIX4 ofsX = -80;
    static FIX4 ofsY = -16;
    iAlpha = m_iAlpha;
    if (iAlpha >= 256) iAlpha = 255;
    if (DebugWork::g_dw.sys.m_iGlare == 2) iAlpha = 0;
    if (iAlpha <= 0) return;
    u_long128 tag;
    __asm__ volatile("pcpyld %0, %1, %0" : "=r"(tag) : "r"((u_long)0xE), "0"((u_long)0x1000000000008000UL));
    qp = CPackMan::pkOpenGifTag(tag);
    qp->ul64[1] = 0x4F;
    qp->ul64[0] = (u_long)((VRAM::_ZBUF_START / 32) | 0x31000000) | 0x100000000UL;
    qp++;
    qp->ul64[0] = 0x61;
    qp->ul64[1] = 0x15;
    qp++;
    qp->ul64[0] = 5;
    qp->ul64[1] = 9;
    qp++;
    if (m_iFlags & 1) {
        qp->ul64[0] = 0x51001;
        qp->ul64[1] = 0x48;
        qp++;
        qp->ul64[0] = 0x6A;
        qp->ul64[1] = 0x43;
        qp++;
        qp->ul64[0] = 0x246;
        qp->ul64[1] = 0;
        qp++;
        qp->ul64[1] = 1;
        qp->ul64[0] = ((u_long)m_iKey << 24) | 0x808080;
        qp++;
        qp = set_csprite64(qp, 0, 0, 0, 0x200, SCREEN_HEIGHT);
    }
    qp->ul64[1] = 0x4D;
    qp->ul64[0] = (u_long)((VRAM::_TEX_AREA_START / 32) | 0x40000);
    qp++;
    qp->ul64[1] = 0x41;
    qp->ul64[0] = ((u_long)(SCREEN_HEIGHT / 2 - 1) << 48) | 0xFF0000;
    qp++;
    qp->ul64[0] = 0x31001;
    qp->ul64[1] = 0x48;
    qp++;
    qp->ul64[0] = 0x206;
    qp->ul64[1] = 0;
    qp++;
    qp->ul64[0] = 0;
    qp->ul64[1] = 1;
    qp++;
    qp = set_csprite64(qp, 0, 0, 0, 0x100, SCREEN_HEIGHT / 2);
    qp->ul64[1] = 0x48;
    qp->ul64[0] = ((u_long)m_iKey << 4) | 0x30009;
    qp++;
    qp->ul64[1] = 7;
    qp->ul64[0] = (u_long)(VRAM::_FRAME0_START | 0x24020000) | 0x2000000064000000UL;
    qp++;
    qp->ul64[0] = 0x316;
    qp->ul64[1] = 0;
    qp++;
    qp->ul64[0] = 0x80808080;
    qp->ul64[1] = 1;
    qp++;
    qp->ul64[1] = 0x3F;
    qp->ul64[0] = 0;
    qp++;
    qp = set_tsprite32(qp, 0, 0, 0, 0x100, SCREEN_HEIGHT / 2, 0x200, SCREEN_HEIGHT);
    qp->ul64[1] = 0x48;
    qp->ul64[0] = 0x30000;
    qp++;
    qp->ul64[1] = 7;
    qp->ul64[0] = (u_long)(VRAM::_TEX_AREA_START | 0x20010000) | 0x2000000200000000UL;
    qp++;
    qp->ul64[1] = 0x43;
    qp->ul64[0] = 0x4000000064UL;
    qp++;
    qp->ul64[0] = 0x356;
    qp->ul64[1] = 0;
    qp++;
    qp->ul64[0] = 0x40808080;
    qp->ul64[1] = 1;
    qp++;
    qp = setTspriteNoSlice(qp, 0, 0x10, 0, 0, 0x1000, (SCREEN_HEIGHT / 2 - 1) << 4);
    qp = setTspriteNoSlice(qp, 0x10, 0, 0, 0, 0xFF0, (SCREEN_HEIGHT / 2) << 4);
    qp = setTspriteNoSlice(qp, 0, -0x10, 0, 0x10, 0x1000, (SCREEN_HEIGHT / 2) << 4);
    qp = setTspriteNoSlice(qp, -0x10, 0, 0x10, 0, 0x1000, (SCREEN_HEIGHT / 2) << 4);
    qp = setTspriteNoSlice(qp, 0, 0x20, 0, 0, 0x1000, (SCREEN_HEIGHT / 2 - 2) << 4);
    qp = setTspriteNoSlice(qp, 0x20, 0, 0, 0, 0xFE0, (SCREEN_HEIGHT / 2) << 4);
    qp = setTspriteNoSlice(qp, 0, -0x20, 0, 0x20, 0x1000, (SCREEN_HEIGHT / 2) << 4);
    qp = setTspriteNoSlice(qp, -0x20, 0, 0x20, 0, 0x1000, (SCREEN_HEIGHT / 2) << 4);
    qp = setTspriteNoSlice(qp, 0, 0, 0, 0, 0x1000, (SCREEN_HEIGHT / 2 - 6) << 4);
    qp = setTspriteNoSlice(qp, 0, 0, 0, 0, 0xFA0, (SCREEN_HEIGHT / 2) << 4);
    qp = setTspriteNoSlice(qp, 0, 0, 0, 0x60, 0x1000, (SCREEN_HEIGHT / 2) << 4);
    qp = setTspriteNoSlice(qp, 0, 0, 0x60, 0, 0x1000, (SCREEN_HEIGHT / 2) << 4);
    qp->ul64[1] = 0x4D;
    qp->ul64[0] = (u_long)((VRAM::_FRAME0_START / 32) | 0x80000);
    qp++;
    qp->ul64[1] = 0x41;
    qp->ul64[0] = ((u_long)(SCREEN_HEIGHT - 1) << 48) | 0x1FF0000;
    qp++;
    qp->ul64[0] = ((u_long)iAlpha << 32) | 0x68;
    qp->ul64[1] = 0x43;
    qp++;
    qp->ul64[0] = 0x356;
    qp->ul64[1] = 0;
    qp++;
    qp->ul64[0] = 0x80808080;
    qp->ul64[1] = 1;
    qp++;
    qp->ul64[1] = 0x3F;
    qp->ul64[0] = 0;
    qp++;
    qp = set_tsprite32(qp, ofsX, ofsY, 0, 0x200, SCREEN_HEIGHT, 0x100, SCREEN_HEIGHT / 2);
    qp->ul64[1] = 0x4F;
    qp->ul64[0] = (u_long)((VRAM::_ZBUF_START / 32) | 0x31000000);
    qp++;
    qp->ul64[0] = 0x5000D;
    qp->ul64[1] = 0x48;
    CPackMan::pkCloseGifTag(qp + 1);
}

#endif

// SOURCE DATA: .data .rodata
