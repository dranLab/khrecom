#define TASK_H_NO_SLC_OVERFLOW_MSG
#define ATELCROSSFADE_H_PATH "c:/KHCOM/MainApp/Game/Atel/AtelCrossFade.h"
#include "System/Graphic/Gprim.h"

inline void* operator new(size_t, void* p) throw() { return p; }

#include "Game/Atel/AtelCrossFade.h"

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

extern INT VSYNC_TIMEQ;
extern INT SCREEN_HEIGHT;

namespace VRAM { /* System/VramMan.h */
extern INT _FRAME0_START;
extern INT _TEX_TMP1_START;
}

void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT r);

#include "System/Graphic/PackMan.h"





extern "C" {
void FlushCache(int operation);
#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/draw/draw_w.h"
int sceGsSetDefStoreImage(sceGsStoreImage* sp, short sbp, short sbw, short spsm, short x, short y, short w, short h);
int sceGsExecStoreImage(sceGsStoreImage* sp, u_long128* dstaddr);













sceDmaChan* sceDmaGetChan(int id);
void sceDmaSendN(sceDmaChan* d, void* addr, int size);
}

#include "include_asm.h"

namespace ATEL {

/* (original form unknown) */
static inline void _setIVector(IVECTOR& v, INT x, INT y, INT z, INT w) {
    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;
}

/* a qword built from two 64-bit or four 32-bit halves (original form unknown) */
union _QW {
    u_long128 q;
    u_long l[2];
    INT i[4];
};

CAtelCrossFade::CAtelCrossFade(CTask* pParent) : CTask(pParent, 0x8000) {
    m_nState = 0;
    m_pCrossFadeImage = NULL;
    m_CrossFadeFlag = 0;
    _setIVector(m_Color, 0x80, 0x80, 0x80, 0x50);
    TRACE("CAtelCrossFade Create\n");
}

CAtelCrossFade::~CAtelCrossFade() {
    TRACE("CAtelCrossFade Delete\n");
    CAtelCrossFade*& rInstance = m_pInstance; /* through a reference: stored before the m_pCrossFadeImage load */
    rInstance = NULL;
    if (m_pCrossFadeImage) {
        com_free(m_pCrossFadeImage);
        m_pCrossFadeImage = NULL;
    }
}

void CAtelCrossFade::main() {
    if (CPackMan::getDrawMode() != 1 || CPackMan::isStartStillMode()) {
        if (m_nState & 1) AnimColor();
    }
}

#line 99
/* Placeholder: re-emits onCreateTexMemory's string literals (it is INCLUDE_ASM'd), in their place in .rodata. */
inline void _crossFadeTexMemoryStrings() {
    com_malloc_d(0, __FILE__ "(104)", 0);
    TRACE("\x83N\x83\x8d\x83X\x83t\x83" "F\x81[\x83h\x97p\x83\x81\x83\x82\x83\x8a\x8am\x95\xdb\x8e\xb8\x94s\x81I\x81I\n");
    TRACE("\x83N\x83\x8d\x83X\x83t\x83" "F\x81[\x83h\x97p\x83\x81\x83\x82\x83\x8a\x8am\x95\xdb\n");
    TRACE("\x83N\x83\x8d\x83X\x83t\x83" "F\x81[\x83h\x97p\x83\x81\x83\x82\x83\x8a\x82\xf0\x8d\xc4\x8am\x95\xdb\x82\xb5\x82\xe6\x82\xa4\x82\xc6\x82\xb5\x82\xdc\x82\xb5\x82\xbd\x81" "B\n");
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/AtelCrossFade", onCreateTexMemory__Q24ATEL14CAtelCrossFadeiPvi);

/* Near-match: the three TRACE/return paths are tail-merged here (one jal TRACE), separate in the original. */
#if 0
INT CAtelCrossFade::onCreateTexMemory(INT nCmd, void* pParam, INT nID) {
    if (!m_pCrossFadeImage) {
        m_pCrossFadeImage = (u_long128*)com_malloc_d(SCREEN_HEIGHT << 11, __FILE__ "(104)", RET_ADDR());
        if (!m_pCrossFadeImage) {
            TRACE("\x83N\x83\x8d\x83X\x83t\x83" "F\x81[\x83h\x97p\x83\x81\x83\x82\x83\x8a\x8am\x95\xdb\x8e\xb8\x94s\x81I\x81I\n");
            return -1;
        } else {
            TRACE("\x83N\x83\x8d\x83X\x83t\x83" "F\x81[\x83h\x97p\x83\x81\x83\x82\x83\x8a\x8am\x95\xdb\n");
            return -1;
        }
    } else {
        TRACE("\x83N\x83\x8d\x83X\x83t\x83" "F\x81[\x83h\x97p\x83\x81\x83\x82\x83\x8a\x82\xf0\x8d\xc4\x8am\x95\xdb\x82\xb5\x82\xe6\x82\xa4\x82\xc6\x82\xb5\x82\xdc\x82\xb5\x82\xbd\x81" "B\n");
        return -1;
    }
}

#endif

INT CAtelCrossFade::onStoreTexImage(INT nCmd, void* pParam, INT nID) {
    setCrossFadeFlag(1, 0);
    return -1;
}

INT CAtelCrossFade::onDrawTexImage(INT nCmd, void* pParam, INT nID) {
    setCrossFadeFlag(2, 0);
    return -1;
}

INT CAtelCrossFade::onSetCrossFadeRGBA(INT nCmd, void* pParam, INT nID) {
    INT c = *(INT*)pParam;
    _setIVector(m_Color, (UINT)c >> 24, (c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF);
    return -1;
}

INT CAtelCrossFade::onSetCrossFadeRGBA_Anim(INT nCmd, void* pParam, INT nID) {
    UBYTE* p = (UBYTE*)pParam;
    FVECTOR vCur;
    m_nState |= 1;
    IVECTOR col = m_Color;
    vCur.x = col.x;
    vCur.y = col.y;
    vCur.z = col.z;
    vCur.w = col.w;
    m_ColorAnim._vTargetColor.x = p[3];
    m_ColorAnim._vTargetColor.y = p[2];
    m_ColorAnim._vTargetColor.z = p[1];
    m_ColorAnim._vTargetColor.w = p[0];
    INT& rVsync = VSYNC_TIMEQ; /* through a reference: loaded where used, and again after the calls */
    m_ColorAnim._nColorTimeLimit = ((INT*)pParam)[1] * rVsync;
    m_ColorAnim._nColorTimeCount = 0;
    m_ColorAnim._vColorBuf = vCur;
    FLOAT fRate = 1.0f / m_ColorAnim._nColorTimeLimit;
    SubVector(vCur, m_ColorAnim._vTargetColor, vCur);
    ScaleVector(vCur, vCur, fRate);
    m_ColorAnim._vColorSpeed = vCur;
    m_ColorAnim._nColorTimeLimit += rVsync;
    return -1;
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/AtelCrossFade", AnimColor__Q24ATEL14CAtelCrossFade);

/* Near-match: the original keeps the temporary vector at sp+0x20 in s0 (a 0x70 frame); here it is at sp. */
#if 0
INT CAtelCrossFade::AnimColor() {
    m_ColorAnim._nColorTimeCount += getFrameRate();
    BOOL bOver = m_ColorAnim.isOverTimeLimit();
    if (!bOver) {
        FVECTOR v;
        ScaleVector(v, m_ColorAnim._vColorSpeed, getFrameRate());
        AddVector(m_ColorAnim._vColorBuf, m_ColorAnim._vColorBuf, v);
        _setIVector(m_Color, (INT)m_ColorAnim._vColorBuf.x, (INT)m_ColorAnim._vColorBuf.y,
                    (INT)m_ColorAnim._vColorBuf.z, (INT)m_ColorAnim._vColorBuf.w);
    } else {
        _setIVector(m_Color, (INT)m_ColorAnim._vColorBuf.x, (INT)m_ColorAnim._vColorBuf.y,
                    (INT)m_ColorAnim._vColorBuf.z, (INT)m_ColorAnim._vColorBuf.w);
        m_nState ^= 1;
        setCrossFadeFlag(0, 2);
    }
    return bOver;
}

#endif

void CAtelCrossFade::storeCrossFadeImage() {
    INT i;
    if (!m_pCrossFadeImage) return;
    setCrossFadeFlag(0, 1);
    for (i = 0; i < SCREEN_HEIGHT / 128; i++) {
        u_long128* p = (u_long128*)((char*)m_pCrossFadeImage + (i << 18));
        sceGsStoreImage si;
        sceGsSetDefStoreImage(&si, VRAM::_FRAME0_START, 8, 0, 0, i * 128, 512, 128);
        FlushCache(0);
        sceGsExecStoreImage(&si, p);
        sceGsSyncPath(0, 0);
    }
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/AtelCrossFade", drawCrossFadeImage__Q24ATEL14CAtelCrossFade);

/* Near-match: the second loop schedules the packet stores differently and hoists the GIF tag constant. */
#if 0
void CAtelCrossFade::drawCrossFadeImage() {
    INT i;
    sceGsLoadImage li;
    _QW qw;
    u_long128 pkt[9];
    for (i = 0; i < SCREEN_HEIGHT / 128; i++) {
        u_long128* p = (u_long128*)((char*)m_pCrossFadeImage + (i << 18));
        sceGsSetDefLoadImage(&li, VRAM::_TEX_TMP1_START, 8, 0, 0, i * 128, 512, 128);
        FlushCache(0);
        sceGsExecLoadImage(&li, p);
        sceGsSyncPath(0, 0);
    }
    qw.l[0] = 0x40AB400000008001UL;
    qw.l[1] = 0x16EE;
    pkt[0] = qw.q;
    qw.l[0] = 0x44;
    qw.l[1] = 0x42;
    pkt[1] = qw.q;
    qw.l[0] = 0x3000B;
    qw.l[1] = 0x47;
    pkt[2] = qw.q;
    qw.l[0] = (u_long)(VRAM::_TEX_TMP1_START | 0x24020000) | ((u_long)0x9000 << 18);
    qw.l[1] = 0;
    pkt[3] = qw.q;
    qw.i[0] = m_Color.x;
    qw.i[1] = m_Color.y;
    qw.i[2] = m_Color.z;
    qw.i[3] = m_Color.w;
    pkt[4] = qw.q;
    FlushCache(0);
    sceDmaSendN(sceDmaGetChan(2), pkt, 5);
    sceGsSyncPath(0, 0);
    for (INT j = 0; j < 8; j++) {
        INT h = SCREEN_HEIGHT;
        i = j;
        qw.l[0] = 0x40AB400000008001UL;
        qw.l[1] = 0x5353;
        pkt[0] = qw.q;
        qw.i[0] = i << 10;
        qw.i[1] = 0;
        qw.i[2] = 0;
        qw.i[3] = 0;
        pkt[1] = qw.q;
        qw.i[0] = (i << 10) + 0x6FF8;
        qw.i[1] = ((0x800 - h / 2) << 4) - 8;
        qw.i[2] = 0xFFFFFF0;
        qw.i[3] = 0;
        pkt[2] = qw.q;
        qw.i[0] = (i + 1) << 10;
        qw.i[1] = h << 4;
        qw.i[2] = 0;
        qw.i[3] = 0;
        pkt[3] = qw.q;
        qw.i[0] = ((i + 1) << 10) + 0x6FF8;
        qw.i[1] = ((h + 0x800 - h / 2) << 4) - 8;
        qw.i[2] = 0xFFFFFF0;
        qw.i[3] = 0;
        pkt[4] = qw.q;
        FlushCache(0);
        sceDmaSendN(sceDmaGetChan(2), pkt, 5);
        sceGsSyncPath(0, 0);
    }
}

#endif

BOOL CAtelCrossFade::isCrossFadeStoreImage() {
    if (m_CrossFadeFlag & 1) {
        if (m_pCrossFadeImage) return TRUE;
    }
    return FALSE;
}

BOOL CAtelCrossFade::isCrossFadeDrawImage() {
    if (m_CrossFadeFlag & 2) {
        if (m_pCrossFadeImage) return TRUE;
    }
    return FALSE;
}

} // namespace ATEL

// SOURCE DATA: .rodata
