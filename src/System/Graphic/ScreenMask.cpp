#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */

#include "System/Graphic/ScreenMask.h"

#include "System/Graphic/PackMan.h"






namespace VRAM {
extern INT _ZBUF_START;
}

extern INT SCREEN_HEIGHT;

/* one qword of an A+D packet (data, register address) */
static inline void _setAD(u_long128* qp, u_long data, u_long addr) {
    u_long128 v;
    __asm__ volatile("pcpyld %0, %1, %0" : "=r"(v) : "r"(addr), "0"(data));
    *qp = v;
}

/* (original form unknown: a plain -h folds into a subtraction) */
static inline INT _neg(INT v) {
    return -v;
}

/* the same for a constant value: tied to the output, a constant is first zero-extended to 128 bits (por) */
static inline void _setADConst(u_long128* qp, u_long data, u_long addr) {
    u_long128 v;
    __asm__ volatile("pcpyld %0, %2, %1" : "=r"(v) : "r"(data), "r"(addr));
    *qp = v;
}

CFadeIOMask::CFadeIOMask(CTask& inParent, USHORT nLayerDepth) : CTask(&inParent, 0x8000) {
    m_bSuspended = FALSE;
    m_nInfo |= TASK_INFO_IGNOREPLAYSPEED;
    m_Prim.setColor(0x80808080);
    m_Prim.setLayerFG(nLayerDepth);
    off();
}

void CFadeIOMask::setLayer(USHORT nLayerDepth) {
    m_Prim.setLayerFG(nLayerDepth);
}

void CFadeIOMask::setBlack(FLOAT fInAlpha) {
    CColorFader::setBlack(fInAlpha);
    doneReq();
    m_Prim.setSemiTrans(ALPHA_MUL);
    on();
}

void CFadeIOMask::setWhite(FLOAT fInAlpha) {
    CColorFader::setWhite(fInAlpha);
    doneReq();
    m_Prim.setSemiTrans(ALPHA_PLUS);
    on();
}

void CFadeIOMask::main() {
    if (m_bSuspended) return;
    BOOL bDone = isFadeDone();
    CColorFader::main(getFrameRate());
    if (bDone && isFadeDone()) {
        if (m_Prim.m_nAlphaMode == ALPHA_PLUS) {
            if (getAlpha() < 0.001f) off();
        } else if (m_Prim.m_nAlphaMode == ALPHA_MUL) {
            if (getAlpha() > 0.999f) off();
        }
    }
}

void CFadeIOMask::disp() {
    /* the compare before the constant and the barrier keep 255.0f after the xori (the scheduler otherwise hoists
     * it to the top); the register variables give the constant f0 and the alpha f1; the barrier on a keeps the
     * andi and makes the store use the a0 the call gets */
    INT x = m_Prim.m_nAlphaMode ^ ALPHA_MUL;
    __asm__ volatile("" : : "r"(x));
    register FLOAT fScale __asm__("$f0") = 255.0f;
    if (x == 0) fScale = 128.0f;
    register FLOAT fAlpha __asm__("$f1") = getAlpha();
    INT a = (INT)(fAlpha * fScale) & 0xFF;
    __asm__ volatile("" : : "r"(a));
    m_Prim.setAlpha(a);
    m_Prim.CGPrim::disp();
}

CScreenMaskPrim::CScreenMaskPrim() {
    m_nFlag = 0;
}

UINT CScreenMaskPrim::getPacketSize() const {
    UINT nSize = 0x10;
    if (m_nFlag & 1) nSize = 0x11;
    return nSize + 5;
}

void CScreenMaskPrim::makePkt() {
    u_long128 tag;
    __asm__ volatile("pcpyld %0, %2, %1" : "=r"(tag) : "r"((u_long)0x1000800000008000UL), "0"((u_long)0xE));
    u_long128* qp = CPackMan::pkOpenGifTag(tag);
    if (m_nFlag & 1) {
        _setAD(qp, (VRAM::_ZBUF_START / 32) | 0x31000000, 0x4F);
        qp++;
    }
    _setADConst(qp, 0x30000, 0x48);
    qp++;
    _setAD(qp, ((u_long)(m_nAttribute & 1) << 6) | 0x206, 0);
    qp++;
    _setAD(qp, (u_long)m_col.col, 1);
    qp++;
    UINT y0 = (_neg(SCREEN_HEIGHT) << 3) + 0x8000;
    UINT y1 = (SCREEN_HEIGHT << 3) + 0x8000;
    UINT xEnd = 0x9000;
    UINT x = 0x7000;
    UINT nx = 0x7400;
    do {
        _setAD(qp, (u_long)x | ((u_long)y0 << 16), 5);
        qp++;
        _setAD(qp, (u_long)nx | ((u_long)y1 << 16), 5);
        qp++;
        x = nx;
        nx = x + 0x400;
    } while (nx <= xEnd);
    if (x < xEnd) {
        _setAD(qp, (u_long)x | ((u_long)y0 << 16), 5);
        qp++;
        _setAD(qp, (u_long)xEnd | ((u_long)y1 << 16), 5);
        qp++;
    }
    _setAD(qp, CPackMan::getDefault_TEST_2(), 0x48);
    CPackMan::pkCloseGifTag(qp + 1);
}

// SOURCE DATA: .rodata
