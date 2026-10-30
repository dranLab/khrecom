#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h"
#define SYSTEM_GRAPHIC_DEFOCUS_CPP
#include "System/Graphic/Gprim.h"
#include "System/Graphic/DeFocus.h"
#include "Game/Debug/DebugWork.h"
#include "System/Graphic/PackMan.h"
#include "include_asm.h"

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)
#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new(size_t, void* p) throw() { return p; }

CDeFocus* CDeFocus::m_pInstance = NULL;

void CDeFocus::create(CTask* p) {
    if (!m_pInstance) {
#line 28
        m_pInstance = new (com_malloc_d(sizeof(CDeFocus), __FILE__ "(28)", RET_ADDR())) CDeFocus(p);
        ASSERT(m_pInstance);
    }
}
CDeFocus::CDeFocus(CTask* p) : CTask(p, 0x4000) {
    for (INT i = 0; i < 2; i++) {
        m_sBlurParam[i].m_fBlurTotalTime = 1.0f;
        m_sBlurParam[i].m_fBlurNowTime = 0.0f;
        m_sBlurParam[i].m_fBlurAimRate = 0.0f;
        m_sBlurParam[i].m_fBlurOldRate = 0.0f;
        m_BlurPrim[i].m_iDebugID = i;
    }
    m_ZFill.setLayerFG(0);
}
CDeFocus::~CDeFocus() {
    CDeFocus*& rInstance = m_pInstance;
    if (rInstance == this) rInstance = NULL;
}
void CDeFocus::main() {
    _BlurUpdate(getFrameRate() * 0.020000001f);
}
void CDeFocus::disp() {
    m_DeFocusPrim.CGPrim::disp();
    INT i;
    INT j;
    for (i = 1, j = 0; i >= 0; i--) {
        m_BlurPrim[i].setCalcZOfs(j);
        m_BlurPrim[i].disp();
        j++;
    }
    m_ZFill.CGPrim::disp();
}
void CDeFocus::BlurSet(INT iKind, FLOAT fRate, FLOAT fTime, INT iNum) {
    if (!m_pInstance || iNum >= 2) return;
    CBlurPrim& BlurPrim = m_pInstance->m_BlurPrim[iNum];
    BLUR_PARAM& p = m_pInstance->m_sBlurParam[iNum];
    if (fTime < 1.0f) fTime = 1.0f;
    p.m_fBlurTotalTime = fTime;
    p.m_fBlurNowTime = 0.0f;
    if (iKind == 0) {
        p.m_fBlurOldRate = 0.0f;
        p.m_fBlurAimRate = 0.0f;
    } else {
        p.m_fBlurOldRate = p.m_fBlurAimRate;
        p.m_fBlurAimRate = fRate;
    }
    BlurPrim.setKind(iKind);
    BlurPrim.setRate(p.m_fBlurOldRate);
}
/* Parked: 2 of 52 lines - only the preheader order: the original sets the hoisted `1` (for 1 << i) before
 * p = m_sBlurParam, ours after. `p++, i++` in the for header fixed the increments; p init as declaration,
 * statement before the loop or first in the for header, a local for the 1, permuter 5 min: no change. */
#if 0
INT CDeFocus::_BlurUpdate(FLOAT fr) {
    INT iRet = 0;
    INT i;
    BLUR_PARAM* p;
    for (i = 0, p = m_sBlurParam; i < 2; p++, i++) {
        if (!m_BlurPrim[i].getKind()) continue;
        if (!(p->m_fBlurNowTime <= p->m_fBlurTotalTime)) continue;
        p->m_fBlurNowTime += fr;
        FLOAT t = p->m_fBlurNowTime / p->m_fBlurTotalTime;
        if (t >= 1.0f) t = 1.0f;
        INT iRate = (INT)(p->m_fBlurOldRate + (p->m_fBlurAimRate - p->m_fBlurOldRate) * t);
        m_BlurPrim[i].setRate(iRate);
        if (p->m_fBlurAimRate == 0.0f && iRate == 0) m_BlurPrim[i].setKind(0);
        iRet |= 1 << i;
    }
    return iRet;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DeFocus", _BlurUpdate__8CDeFocusf);
#endif
void CDeFocus::BlurDistanceSet(FLOAT fDistance, INT iNum) {
    if (!m_pInstance || iNum >= 2) return;
    m_pInstance->m_BlurPrim[iNum].setDistance(fDistance);
}
void CDeFocus::BlurColorSet(INT iR, INT iG, INT iB, INT iA, ULONGLONG lAlpha, INT iNum) {
    if (!m_pInstance || iNum >= 2) return;
    CBlurPrim& BlurPrim = m_pInstance->m_BlurPrim[iNum];
    BlurPrim.setColor(iR, iG, iB, iA);
    BlurPrim.setAlphaMode(lAlpha);
}
void CDeFocus::BlurSetEvent(INT iNum) {
    switch (iNum) {
    case 0:
        BlurSet(2, 2.0f, 0.0f, 1);
        BlurColorSet(0x80, 0x80, 0x80, 6, 0x44, 1);
        break;
    case 1:
        BlurSet(5, 2.0f, 0.0f, 1);
        BlurColorSet(0xc0, 0xc0, 0xc0, 8, 0x44, 1);
        break;
    case 2:
        BlurSet(1, 2.0f, 0.0f, 1);
        BlurColorSet(0x80, 0x80, 0x80, 10, 0x44, 1);
        break;
    default:
        BlurSet(1, 3.0f, 0.0f, 1);
        BlurColorSet(0x80, 0x80, 0x80, 0x10, 0x48, 1);
        break;
    }
}
void CDeFocus::BlurReturnEvent() {
    BlurSet(0, 0.0f, 0.0f, 1);
    BlurColorSet(0x80, 0x80, 0x80, 0x40, 0x44, 1);
}
CDeFocusPrim::CDeFocusPrim() {
    setLayer(6, 0);
    m_fNear = 3.0f;
    m_fFar = 20.0f;
    m_iStatus = 0;
}
FIX4 CDeFocusPrim::calcZ() {
    if (m_iStatus & 0x10000) return 0xf0000000;
    if ((m_iStatus & 3) == 3) return 0xf0000000;
    return 0xffffc06;
}
CBlurPrim::CBlurPrim() {
    setLayer(6, 0);
    m_iKind = 0;
    m_iRate = 0;
    m_fLen = 0.0f;
    m_iColor.col = 0x40808080;
    m_iAlpha = 0x44;
    m_nCalcZOfs = 0;
    m_iDebugID = 0;
}
FIX4 CBlurPrim::calcZ() {
    if (!(DebugWork::g_dw.sys.m_iBlurKind && DebugWork::g_dw.sys.m_iBlurCh == m_iDebugID)) {
        if (!m_iKind || m_iRate <= 0) return 0xf0000000;
    }
    return m_nCalcZOfs + 0xffffc08;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DeFocus", setTSspriteScissor32__FP6QWdataiiiiiiii);
union PRIMIVECTOR {
    struct {
        SHORT x;
        SHORT y;
        UINT z;
    };
    ULONGLONG ul64;
};
extern INT SCREEN_HEIGHT;
QWdata* set_csprite64(QWdata* qp, FIX4 plsx, FIX4 plsy, UINT z, INT ScrW, INT ScrH) {
    PRIMIVECTOR pv[2];
    int i;
    int cnt;
    SHORT ofsXP;
    ULONGLONG* pul64;
    pv[0].x = plsx;
    pv[0].y = plsy;
    pv[1].z = z;
    ofsXP = 0;
    if (pv[0].x < 0) {
        pv[0].x = 0;
    } else if (pv[0].x > 0) {
        ofsXP = pv[0].x;
    }
    pv[0].x += (0x800 - 0x200 / 2) << 4;
    pv[0].y += (0x800 - SCREEN_HEIGHT / 2) << 4;
    pv[1].x = pv[0].x + (64 << 4) - ofsXP;
    pv[1].y = pv[0].y + (ScrH << 4);
    cnt = (ScrW << 4) / (64 << 4);
    if (qp) {
        CPackMan::pkCloseGifTag(qp);
    }
    u_long128 tag;
    __asm__ volatile("pcpyld %0, %2, %1" : "=r"(tag) : "r"((u_long)cnt | 0x2400000000000000UL), "r"((u_long)0x44));
    pul64 = (ULONGLONG*)CPackMan::pkOpenGifTagManual(tag);
    for (i = 0; i < cnt; i++) {
        pul64[0] = pv[0].ul64;
        pul64[1] = pv[1].ul64;
        pul64 += 2;
        pv[0].x = pv[1].x;
        pv[1].x += 64 << 4;
    }
    CPackMan::pkCloseGifTagManual((UINT*)pul64);
    if (qp) {
        __asm__ volatile("pcpyld %0, %2, %1" : "=r"(tag) : "r"((u_long)0x1000000000008000UL), "0"((u_long)0xE));
        qp = (QWdata*)CPackMan::pkOpenGifTag(tag);
    }
    return qp;
}
QWdata* set_tsprite32(QWdata* qp, FIX4 plsx, FIX4 plsy, UINT z, INT ScrW, INT ScrH, INT uvW, INT uvH) {
    PRIMIVECTOR pv[2];
    PRIMIVECTOR uv[2];
    int i;
    int cnt;
    SHORT ofsXM;
    SHORT ofsXP;
    IVECTOR ivUVPosUL;
    IVECTOR ivUVRect;
    ULONGLONG* pul64;
    cnt = (ScrW << 4) / (32 << 4);
    INT nStep = (uvW << 4) / cnt; /* MATCH: not in the ref's locals; the original keeps the UV step in s5 from before isPAL() */
    if (CMainApp::isPAL()) {
        plsy += 8;
    }
    pv[0].x = plsx;
    pv[0].y = plsy;
    pv[1].z = z;
    ofsXM = 0;
    ofsXP = 0;
    if (pv[0].x < 0) {
        ofsXM = pv[0].x;
        pv[0].x = 0;
    } else if (pv[0].x > 0) {
        ofsXP = pv[0].x;
    }
    pv[0].x += (0x800 - 0x200 / 2) << 4;
    pv[0].y += (0x800 - SCREEN_HEIGHT / 2) << 4;
    pv[1].x = pv[0].x + (32 << 4) - ofsXP;
    pv[1].y = pv[0].y + (ScrH << 4);
    ivUVRect.y = uvH << 4;
    ivUVPosUL.x = 0;
    ivUVPosUL.y = 0;
    ivUVPosUL.z = 0;
    ivUVPosUL.w = 0;
    ivUVRect.x = nStep;
    ivUVRect.z = 0;
    ivUVRect.w = 0;
    uv[0].x = ivUVPosUL.x;
    uv[0].y = ivUVPosUL.y;
    if (ofsXM) {
        uv[0].x -= ofsXM;
    }
    uv[1].x = uv[0].x + nStep;
    if (ofsXP) {
        uv[1].x -= ofsXP;
    }
    uv[1].y = uv[0].y + ivUVRect.y;
    if (qp) {
        CPackMan::pkCloseGifTag(qp);
    }
    u_long128 tag;
    __asm__ volatile("pcpyld %0, %2, %1" : "=r"(tag) : "r"((u_long)cnt | 0x4400000000000000UL), "r"((u_long)0x4343));
    pul64 = (ULONGLONG*)CPackMan::pkOpenGifTagManual(tag);
    for (i = 0; i < cnt; i++) {
        pul64[0] = uv[0].ul64;
        pul64[1] = pv[0].ul64;
        pul64[2] = uv[1].ul64;
        pul64[3] = pv[1].ul64;
        pul64 += 4;
        pv[0].x = pv[1].x;
        pv[1].x += 32 << 4;
        uv[0].x = uv[1].x;
        uv[1].x += nStep;
    }
    CPackMan::pkCloseGifTagManual((UINT*)pul64);
    if (qp) {
        __asm__ volatile("pcpyld %0, %2, %1" : "=r"(tag) : "r"((u_long)0x1000000000008000UL), "0"((u_long)0xE));
        qp = (QWdata*)CPackMan::pkOpenGifTag(tag);
    }
    return qp;
}
void CDeFocusPrim::makePkt() { _makePktDeFocus(); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DeFocus", _makePktDeFocus__12CDeFocusPrim);
void CBlurPrim::makePkt() { _makePktBlur(); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DeFocus", _makePktBlur__9CBlurPrim);
__asm__(".rdata\n.align 3\n.word 0x001232F0\n.word 0x001233A4\n.word 0x00123480\n.word 0x00123534\n.word 0x001235C0\n.word 0x001236A4\n.text\n");

// SOURCE DATA: .data .rodata
