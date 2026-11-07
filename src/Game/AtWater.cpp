
#include "Game/AtWater.h"
#include "System/Math3D.h"

inline void* operator new(size_t, void* p) throw() { return p; }


extern "C" float cosf(float);
extern "C" float sinf(float);

#include "Game/SceneEnv.h"
#line 16

#include "System/Graphic/PackMan.h"
#include "System/HeapMan.h"





namespace VRAM {
extern INT _FRAME0_START;
extern INT _ZBUF_START;
extern INT _TEX_AREA_START;
}

extern INT SCREEN_HEIGHT;

QWdata* setTSspriteScissor32(QWdata* qp, FIX4 ofsx, FIX4 ofsy, INT width, INT height, FIX4 u0, FIX4 v0, FIX4 u1, FIX4 v1);

CAtWater* CAtWater::m_pInstance = NULL;

#line 25
void CAtWater::create(CTask* p) {
    if (!m_pInstance) {
        m_pInstance =
            new (com_malloc_d(sizeof(CAtWater), __FILE__ "(28)", RET_ADDR())) CAtWater(p);
        if (!m_pInstance) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
    }
}

#line 38
CAtWater::CAtWater(CTask* p) : CTask(p, 0x4000) {
    m_pAtWater =
        new (com_malloc_d(sizeof(CAtWaterPrim), __FILE__ "(40)", RET_ADDR())) CAtWaterPrim();
    if (!m_pAtWater) AssertApp(__FILE__, __LINE__, "m_pAtWater", 0);
}

CAtWater::~CAtWater() {
    delete m_pAtWater;
    m_pAtWater = NULL;
    CAtWater*& r = m_pInstance; /* through a reference: keeps the m_pAtWater store before the lui */
    if (r == this) r = NULL;
}

void CAtWater::main() {
    if (!(CSceneEnv::getStopTypeBit() & 0x800)) {
        m_pAtWater->update(getFrameRateF());
    }
}

void CAtWater::disp() {
    m_pAtWater->disp();
}

CAtWaterPrim::CAtWaterPrim() {
    m_LayerInfo._nID = 6;
    m_fRate = 0.0f;
}

void CAtWaterPrim::update(FLOAT fr) {
    m_fRate += fr * 0.017453292f;
    m_fRate = Math3D::LimitPI(m_fRate);
}

void CAtWaterPrim::makePkt() {
    FIX4 su;
    FIX4 sv;
    FIX4 eu;
    FIX4 ev;
    QWdata* qp;

    su = (INT)(cosf(m_fRate) * 48.0f) + 0x40;
    sv = (INT)(sinf(m_fRate) * 48.0f) + 0x40;
    eu = 0x2000 - su;
    ev = (SCREEN_HEIGHT << 4) - sv;

    /* GIF tag (NREG 1, EOP, A+D) built with a volatile pcpyld: it also stops sched1 hoisting the later
     * constants above the call; the packet is written through u_long* (not QWdata members), so the
     * VRAM/SCREEN_HEIGHT loads stay ordered after the stores */
    u_long128 tag;
    __asm__ volatile("pcpyld %0, %2, %1" : "=r"(tag) : "r"((u_long)0x1000000000008000UL), "0"((u_long)0xE));
    qp = (QWdata*)CPackMan::pkOpenGifTag(tag);

    ((u_long*)qp)[0] = (u_long)(VRAM::_ZBUF_START / 32) | ((u_long)0x31 << 24) | ((u_long)1 << 32);
    ((u_long*)qp)[1] = 0x4F;
    qp++;
    ((u_long*)qp)[0] = 0x31001;
    ((u_long*)qp)[1] = 0x48;
    qp++;
    ((u_long*)qp)[0] = 5;
    ((u_long*)qp)[1] = 9;
    qp++;
    ((u_long*)qp)[0] = (u_long)(VRAM::_TEX_AREA_START / 32) | ((u_long)8 << 16);
    ((u_long*)qp)[1] = 0x4D;
    qp++;
    ((u_long*)qp)[0] = 1;
    ((u_long*)qp)[1] = 0x15;
    qp++;
    ((u_long*)qp)[0] = (u_long)VRAM::_FRAME0_START | ((u_long)8 << 14) | ((u_long)9 << 26) | ((u_long)9 << 30) |
                 ((u_long)1 << 34) | ((u_long)1 << 61);
    ((u_long*)qp)[1] = 7;
    qp++;
    ((u_long*)qp)[0] = 0x316;
    ((u_long*)qp)[1] = 0;
    qp++;
    ((u_long*)qp)[0] = 0x80808080;
    ((u_long*)qp)[1] = 1;
    qp++;
    qp = setTSspriteScissor32(qp, 0, 0, 0x200, SCREEN_HEIGHT, 0, 0, 0x2000, SCREEN_HEIGHT << 4);

    ((u_long*)qp)[0] = (u_long)(VRAM::_FRAME0_START / 32) | ((u_long)8 << 16);
    ((u_long*)qp)[1] = 0x4D;
    qp++;
    ((u_long*)qp)[0] = 0x61;
    ((u_long*)qp)[1] = 0x15;
    qp++;
    ((u_long*)qp)[0] = (u_long)VRAM::_TEX_AREA_START | ((u_long)8 << 14) | ((u_long)9 << 26) | ((u_long)9 << 30) |
                 ((u_long)1 << 34) | ((u_long)1 << 61);
    ((u_long*)qp)[1] = 7;
    qp++;
    ((u_long*)qp)[0] = 0x316;
    ((u_long*)qp)[1] = 0;
    qp++;
    ((u_long*)qp)[0] = 0x80808080;
    ((u_long*)qp)[1] = 1;
    qp++;
    qp = setTSspriteScissor32(qp, 0, 0, 0x200, SCREEN_HEIGHT, su, sv, eu, ev);

    ((u_long*)qp)[0] = (u_long)(VRAM::_ZBUF_START / 32) | ((u_long)0x31 << 24);
    ((u_long*)qp)[1] = 0x4F;
    qp++;
    ((u_long*)qp)[0] = 0x5000D;
    ((u_long*)qp)[1] = 0x48;
    qp++;
    CPackMan::pkCloseGifTag(qp);
}

// SOURCE DATA: .data .rodata
