#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h"
#define SYSTEM_GRAPHIC_DEFOCUS_CPP
#include "System/Graphic/Gprim.h"
#include "System/Graphic/DeFocus.h"
#include "Game/Debug/DebugWork.h"
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DeFocus", _BlurUpdate__8CDeFocusf);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DeFocus", set_csprite64__FP6QWdataiiUiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DeFocus", set_tsprite32__FP6QWdataiiUiiiii);
void CDeFocusPrim::makePkt() { _makePktDeFocus(); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DeFocus", _makePktDeFocus__12CDeFocusPrim);
void CBlurPrim::makePkt() { _makePktBlur(); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DeFocus", _makePktBlur__9CBlurPrim);
__asm__(".rdata\n.align 3\n.word 0x001232F0\n.word 0x001233A4\n.word 0x00123480\n.word 0x00123534\n.word 0x001235C0\n.word 0x001236A4\n.text\n");

// SOURCE DATA: .data .rodata
