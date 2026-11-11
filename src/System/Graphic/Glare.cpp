
#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Graphic/Glare.h"
#include "System/VramMan.h"
#include "Game/Debug/DebugWork.h"





#include "System/Graphic/PackMan.h"
#include "System/HeapMan.h"





extern INT SCREEN_HEIGHT;
QWdata* set_csprite64(QWdata* qp, INT x, INT y, UINT col, INT w, INT h);
QWdata* set_tsprite32(QWdata* qp, INT x, INT y, UINT col, INT u, INT v, INT w, INT h);

CGlare* CGlare::m_pInstance = NULL;

#line 26
void CGlare::create(CTask* p) {
    if (!m_pInstance) {
        m_pInstance = new KH_AT("(28)") CGlare(p);
        KH_DEBUG(if (!m_pInstance) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);)
    }
}

#line 39
CGlare::CGlare(CTask* p) : CTask(p, 0x4000) {
    m_pGlare = new KH_AT("(40)") CGlarePrim;
    KH_DEBUG(if (!m_pGlare) AssertApp(__FILE__, __LINE__, "m_pGlare", 0);)
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
    if (KH_DW(DebugWork::g_dw.sys.m_iGlare) == 1) fAlphaPer = KH_DW(DebugWork::g_dw.sys.m_fGlareMaxPer);
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

static QWdata* setTspriteNoSlice(QWdata* qp, FIX4 ofsx, FIX4 ofsy, FIX4 u0, FIX4 v0, FIX4 u1, FIX4 v1) {
    *(u_long128*)qp = _makeQW((u_long)u0 | ((u_long)v0 << 16), 3);
    qp++;
    *(u_long128*)qp = _makeQW((u_long)(ofsx + 0x7000) | ((u_long)(((0x800 - SCREEN_HEIGHT / 2) << 4) + ofsy) << 16), 4);
    qp++;
    *(u_long128*)qp = _makeQW((u_long)u1 | ((u_long)v1 << 16), 3);
    qp++;
    *(u_long128*)qp = _makeQW((u_long)(ofsx + 0x8000) | ((u_long)(ofsy + 0x8000) << 16), 4);
    qp++;
    return qp;
}

/* The original's line numbers (its .mdebug line markers) are kept with #line; the packet is written through
 * u_long* as in Game/AtWater (MATCH: with QWdata member stores the -1 of the four divisions by 32 loses its saved
 * register and the constants 0x43/0x356 take them: 361 of 613 lines). */
#line 296
void CGlarePrim::makePkt() {
    INT iAlpha;
    QWdata* qp;
    static FIX4 ofsX = -80;
    static FIX4 ofsY = -16;
#line 297
    iAlpha = m_iAlpha;
    if (iAlpha >= 256) iAlpha = 255;

    if (KH_DW(DebugWork::g_dw.sys.m_iGlare) == 2) iAlpha = 0;

    if (iAlpha <= 0) return;
#line 312
    qp = (QWdata*)CPackMan::pkOpenGifTag(_makeQW(0x1000000000008000UL, 0xE));

    ((u_long*)qp)[0] = (u_long)((VRAM::_ZBUF_START / 32) | 0x31000000) | 0x100000000UL;
    ((u_long*)qp)[1] = 0x4F; qp++;
    ((u_long*)qp)[0] = 0x61;
    ((u_long*)qp)[1] = 0x15; qp++;
    ((u_long*)qp)[0] = 5;
    ((u_long*)qp)[1] = 9; qp++;

    if (m_iFlags & 1) {
#line 339
        ((u_long*)qp)[0] = 0x51001;
        ((u_long*)qp)[1] = 0x48; qp++;
        ((u_long*)qp)[0] = 0x6A;
        ((u_long*)qp)[1] = 0x43; qp++;
        ((u_long*)qp)[0] = 0x246;
        ((u_long*)qp)[1] = 0; qp++;
        ((u_long*)qp)[0] = ((u_long)m_iKey << 24) | 0x808080;
        ((u_long*)qp)[1] = 1;
        qp = set_csprite64(qp + 1, 0, 0, 0, 0x200, SCREEN_HEIGHT);
    }


    ((u_long*)qp)[0] = (u_long)((VRAM::_TEX_AREA_START / 32) | 0x40000);
    ((u_long*)qp)[1] = 0x4D; qp++;
    ((u_long*)qp)[0] = ((u_long)(SCREEN_HEIGHT / 2 - 1) << 48) | 0xFF0000;
    ((u_long*)qp)[1] = 0x41; qp++;


    ((u_long*)qp)[0] = 0x31001;
    ((u_long*)qp)[1] = 0x48; qp++;
    ((u_long*)qp)[0] = 0x206;
    ((u_long*)qp)[1] = 0; qp++;
    ((u_long*)qp)[0] = 0;
    ((u_long*)qp)[1] = 1;
    qp = set_csprite64(qp + 1, 0, 0, 0, 0x100, SCREEN_HEIGHT / 2);


    ((u_long*)qp)[0] = ((u_long)m_iKey << 4) | 0x30009;
    ((u_long*)qp)[1] = 0x48; qp++;
    ((u_long*)qp)[0] = (u_long)(VRAM::_FRAME0_START | 0x24020000) | 0x2000000640000000UL;
    ((u_long*)qp)[1] = 7; qp++;
    ((u_long*)qp)[0] = 0x316;
    ((u_long*)qp)[1] = 0; qp++;
    ((u_long*)qp)[0] = 0x80808080;
    ((u_long*)qp)[1] = 1; qp++;
    ((u_long*)qp)[0] = 0;
    ((u_long*)qp)[1] = 0x3F;
    qp = set_tsprite32(qp + 1, 0, 0, 0, 0x100, SCREEN_HEIGHT / 2, 0x200, SCREEN_HEIGHT);


    ((u_long*)qp)[0] = 0x30000;
    ((u_long*)qp)[1] = 0x48; qp++;
    ((u_long*)qp)[0] = (u_long)(VRAM::_TEX_AREA_START | 0x20010000) | 0x2000000200000000UL;
    ((u_long*)qp)[1] = 7; qp++;
    ((u_long*)qp)[0] = 0x4000000064UL;
    ((u_long*)qp)[1] = 0x43; qp++;
    ((u_long*)qp)[0] = 0x356;
    ((u_long*)qp)[1] = 0; qp++;
    ((u_long*)qp)[0] = 0x40808080;
    ((u_long*)qp)[1] = 1;

    qp = setTspriteNoSlice(qp + 1, 0, 0x10, 0, 0, 0x1000, (SCREEN_HEIGHT / 2 - 1) << 4);
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
#line 421
    ((u_long*)qp)[0] = (u_long)((VRAM::_FRAME0_START / 32) | 0x80000);
    ((u_long*)qp)[1] = 0x4D; qp++;
    ((u_long*)qp)[0] = ((u_long)(SCREEN_HEIGHT - 1) << 48) | 0x1FF0000;
    ((u_long*)qp)[1] = 0x41; qp++;


    ((u_long*)qp)[0] = ((u_long)iAlpha << 32) | 0x68;
    ((u_long*)qp)[1] = 0x43; qp++;
    ((u_long*)qp)[0] = 0x356;
    ((u_long*)qp)[1] = 0; qp++;
    ((u_long*)qp)[0] = 0x80808080;
    ((u_long*)qp)[1] = 1; qp++;
    ((u_long*)qp)[0] = 0;
    ((u_long*)qp)[1] = 0x3F;
#line 457
    qp = set_tsprite32(qp + 1, ofsX, ofsY, 0, 0x200, SCREEN_HEIGHT, 0x100, SCREEN_HEIGHT / 2);



    ((u_long*)qp)[0] = (u_long)((VRAM::_ZBUF_START / 32) | 0x31000000);
    ((u_long*)qp)[1] = 0x4F; qp++;
    ((u_long*)qp)[0] = 0x5000D;
    ((u_long*)qp)[1] = 0x48;

    CPackMan::pkCloseGifTag(qp + 1);
#line 474
}

// SOURCE DATA: .data .rodata
