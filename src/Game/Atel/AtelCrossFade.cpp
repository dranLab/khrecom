
#define ATELCROSSFADE_H_PATH "c:/KHCOM/MainApp/Game/Atel/AtelCrossFade.h" /* MATCH: this unit names AtelCrossFade.h so (header default "c:/KHCOM/MainApp/Game/ATEL/AtelCrossFade.h") */
#include "System/Graphic/Gprim.h"



#include "Game/Atel/AtelCrossFade.h"


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

#include "System/HeapMan.h"

typedef union {
    u_long128 u128;
    IVECTOR iv;
    FVECTOR fv;
    u_long u64[2];
} CROSS_VECTOR;

namespace ATEL {
CAtelCrossFade* CAtelCrossFade::m_pInstance = NULL;

/* (original form unknown) */
static inline void _setIVector(IVECTOR& v, INT x, INT y, INT z, INT w) {
    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;
}


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


INT CAtelCrossFade::onCreateTexMemory(INT nCmd, void* pParam, INT nID) {
    if (!m_pCrossFadeImage) {
        m_pCrossFadeImage = (u_long128*)com_malloc_d(SCREEN_HEIGHT << 11, __FILE__ "(104)", RET_ADDR());
        if (!m_pCrossFadeImage) {
            TRACE("\x83N\x83\x8d\x83X\x83t\x83" "F\x81[\x83h\x97p\x83\x81\x83\x82\x83\x8a\x8am\x95\xdb\x8e\xb8\x94s\x81I\x81I\n");
            return -1; /* MATCH: return here, goto ret in the other two: keeps the three TRACE calls apart (no cross-jump) */
        } else {
            TRACE("\x83N\x83\x8d\x83X\x83t\x83" "F\x81[\x83h\x97p\x83\x81\x83\x82\x83\x8a\x8am\x95\xdb\n");
            goto ret;
        }
    } else {
        TRACE("\x83N\x83\x8d\x83X\x83t\x83" "F\x81[\x83h\x97p\x83\x81\x83\x82\x83\x8a\x82\xf0\x8d\xc4\x8am\x95\xdb\x82\xb5\x82\xe6\x82\xa4\x82\xc6\x82\xb5\x82\xdc\x82\xb5\x82\xbd\x81" "B\n");
        goto ret;
    }
ret:
    return -1;
}

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

/* MATCH: the original's inline at Com.h line 60 (name unknown; the ref's local t is its parameter, the multiply is on
   its line) reads VSYNC_TIMEQ through a reference: loaded where used, and again after the calls (as TaskMan.h) */
static inline TIMEQ _VSyncTQ(INT t) { INT& r = VSYNC_TIMEQ; return t * r; }
INT CAtelCrossFade::onSetCrossFadeRGBA_Anim(INT nCmd, void* pParam, INT nID) {
    FVECTOR v;
    IVECTOR iv;
    FLOAT fRate;
    m_nState |= 1;
    m_ColorAnim._vTargetColor.x = ((UBYTE*)pParam)[3];
    m_ColorAnim._vTargetColor.y = ((UBYTE*)pParam)[2];
    m_ColorAnim._vTargetColor.z = ((UBYTE*)pParam)[1];
    m_ColorAnim._vTargetColor.w = ((UBYTE*)pParam)[0];
    m_ColorAnim._nColorTimeLimit = _VSyncTQ(((INT*)pParam)[1]);
    m_ColorAnim._nColorTimeCount = 0;
    iv = m_Color;
    v.x = iv.x;
    v.y = iv.y;
    v.z = iv.z;
    v.w = iv.w;
    m_ColorAnim._vColorBuf = v;
    fRate = 1.0f / m_ColorAnim._nColorTimeLimit;
    SubVector(v, m_ColorAnim._vTargetColor, v);
    ScaleVector(v, v, fRate);
    m_ColorAnim._vColorSpeed = v;
    m_ColorAnim._nColorTimeLimit += _VSyncTQ(1);
    return -1;
}



INT CAtelCrossFade::AnimColor() {
    m_ColorAnim._nColorTimeCount += getFrameRate();
    BOOL bRes = m_ColorAnim.isOverTimeLimit();
    IVECTOR iv; /* MATCH: the ref's iv/Color (unused) give ColorSpeed its sp+0x20 slot */
    FVECTOR Color;
    FVECTOR ColorSpeed;
    if (!bRes) {
        ScaleVector(ColorSpeed, m_ColorAnim._vColorSpeed, getFrameRate());
        AddVector(m_ColorAnim._vColorBuf, m_ColorAnim._vColorBuf, ColorSpeed);
        _setIVector(m_Color, (INT)m_ColorAnim._vColorBuf.x, (INT)m_ColorAnim._vColorBuf.y,
                    (INT)m_ColorAnim._vColorBuf.z, (INT)m_ColorAnim._vColorBuf.w);
    } else {
        _setIVector(m_Color, (INT)m_ColorAnim._vColorBuf.x, (INT)m_ColorAnim._vColorBuf.y,
                    (INT)m_ColorAnim._vColorBuf.z, (INT)m_ColorAnim._vColorBuf.w);
        m_nState ^= 1;
        setCrossFadeFlag(0, 2);
    }
    return bRes;
}


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

void CAtelCrossFade::drawCrossFadeImage() {
    u_long128* p;
    sceGsLoadImage lp;
    CROSS_VECTOR cross;
    u_long128 DmaBuf[9];
    for (INT i = 0; i < SCREEN_HEIGHT / 128; i++) {
        p = (u_long128*)((char*)m_pCrossFadeImage + (i << 18));
        sceGsSetDefLoadImage(&lp, VRAM::_TEX_TMP1_START, 8, 0, 0, i * 128, 512, 128);
        FlushCache(0);
        sceGsExecLoadImage(&lp, p);
        sceGsSyncPath(0, 0);
    }
    cross.u64[0] = 0x40AB400000008001UL;
    cross.u64[1] = 0x16EE;
    DmaBuf[0] = cross.u128;
    cross.u64[0] = 0x44;
    cross.u64[1] = 0x42;
    DmaBuf[1] = cross.u128;
    cross.u64[0] = 0x3000B;
    cross.u64[1] = 0x47;
    DmaBuf[2] = cross.u128;
    cross.u64[0] = (u_long)(VRAM::_TEX_TMP1_START | 0x24020000) | ((u_long)0x9000 << 18);
    cross.u64[1] = 0;
    DmaBuf[3] = cross.u128;
    cross.iv.set(m_Color.x, m_Color.y, m_Color.z, m_Color.w);
    DmaBuf[4] = cross.u128;
    FlushCache(0);
    sceDmaSendN(sceDmaGetChan(2), DmaBuf, 5);
    sceGsSyncPath(0, 0);
    for (INT i = 0; i < 8; i++) {
        cross.u64[0] = 0x40AB400000008001UL;
        cross.u64[1] = 0x5353;
        DmaBuf[0] = cross.u128;
        cross.iv.set(i << 10, 0, 0, 0);
        DmaBuf[1] = cross.u128;
        cross.iv.x = (i * 64 + 0x800 - 512 / 2) * 16 - 8;
        cross.iv.y = (0x800 - SCREEN_HEIGHT / 2) * 16 - 8;
        cross.iv.z = 0xFFFFFF0;
        cross.iv.w = 0;
        DmaBuf[2] = cross.u128;
        cross.iv.set((i + 1) << 10, SCREEN_HEIGHT << 4, 0, 0);
        DmaBuf[3] = cross.u128;
        cross.iv.x = ((i + 1) * 64 + 0x800 - 512 / 2) * 16 - 8;
        cross.iv.y = (SCREEN_HEIGHT + (0x800 - SCREEN_HEIGHT / 2)) * 16 - 8;
        cross.iv.z = 0xFFFFFF0;
        cross.iv.w = 0;
        DmaBuf[4] = cross.u128;
        FlushCache(0);
        sceDmaSendN(sceDmaGetChan(2), DmaBuf, 5);
        sceGsSyncPath(0, 0);
    }
}


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

// SOURCE DATA: .data .rodata
