
#define ATELBLUR_H_PATH "c:/KHCOM/MainApp/Game/Atel/AtelBlur.h" /* MATCH: this unit names AtelBlur.h so (header default "c:/KHCOM/MainApp/Game/ATEL/AtelBlur.h") */
#define GPRIM_H_PATH "System/../System/Graphic/Gprim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Task.h"
#include <string.h>

inline void* operator new(size_t, void* p) throw() { return p; }

#include "Game/Atel/AtelBlur.h"
#include "System/Graphic/Gprim.h"
#include <string.h>

extern INT VSYNC_TIMEQ;

void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT r);

#include "System/Graphic/PackMan.h"





namespace ATEL {
CAtelBlur* CAtelBlur::m_pInstance = NULL;

/* the parameter of the animation commands (original name unknown) */
struct ATEL_BLUR_ANIM_PARAM {
    FVECTOR vTarget;
    INT nTime;
};

CAtelBlur::CAtelBlur(CTask* pParent) : CTask(pParent, 0x8000) {
    m_nState = 0;
    m_Xofs = 3;
    m_Yofs = 3;
    m_Alpha = 0x40;
    memset(&m_Anim, 0, sizeof(m_Anim));
    TRACE("CAtelBlur Create\n");
}

CAtelBlur::~CAtelBlur() {
    TRACE("CAtelBlur Delete\n");
    if (CPackMan::getTransFrameKind() == 3) {
        CPackMan::setFrameEffect(0, 0, 0, 0);
    }
    m_pInstance = NULL;
}

void CAtelBlur::main() {
    if (m_nState & 1) Anim();
}

INT CAtelBlur::onSetBlurAlpha(INT nCmd, void* pParam, INT nID) {
    m_Alpha = *(INT*)pParam;
    CPackMan::setFrameEffect(3, m_Xofs, m_Yofs, m_Alpha);
    return -1;
}

INT CAtelBlur::onSetBlurXofs(INT nCmd, void* pParam, INT nID) {
    m_Xofs = *(INT*)pParam;
    CPackMan::setFrameEffect(3, m_Xofs, m_Yofs, m_Alpha);
    return -1;
}

INT CAtelBlur::onSetBlurYofs(INT nCmd, void* pParam, INT nID) {
    m_Yofs = *(INT*)pParam;
    CPackMan::setFrameEffect(3, m_Xofs, m_Yofs, m_Alpha);
    return -1;
}

INT CAtelBlur::onSetBlurAlphaAnim(INT nCmd, void* pParam, INT nID) {
    ATEL_BLUR_ANIM_PARAM param;
    param.vTarget.x = m_Xofs;
    param.vTarget.y = m_Yofs;
    param.vTarget.z = ((INT*)pParam)[0];
    param.nTime = ((INT*)pParam)[1];
    onSetBlurAnim(0, &param, 0);
    return -1;
}

INT CAtelBlur::onSetBlurXofsAnim(INT nCmd, void* pParam, INT nID) {
    ATEL_BLUR_ANIM_PARAM param;
    param.vTarget.x = ((INT*)pParam)[0];
    param.vTarget.y = m_Yofs;
    param.vTarget.z = m_Alpha;
    param.nTime = ((INT*)pParam)[1];
    onSetBlurAnim(0, &param, 0);
    return -1;
}

INT CAtelBlur::onSetBlurYofsAnim(INT nCmd, void* pParam, INT nID) {
    ATEL_BLUR_ANIM_PARAM param;
    param.vTarget.x = m_Xofs;
    param.vTarget.y = ((INT*)pParam)[0];
    param.vTarget.z = m_Alpha;
    param.nTime = ((INT*)pParam)[1];
    onSetBlurAnim(0, &param, 0);
    return -1;
}

INT CAtelBlur::onSetBlurAnim(INT nCmd, void* pParam, INT nID) {
    ATEL_BLUR_ANIM_PARAM* p = (ATEL_BLUR_ANIM_PARAM*)pParam;
    FVECTOR vCur;
    m_nState |= 1;
    vCur.x = m_Xofs;
    vCur.y = m_Yofs;
    vCur.z = m_Alpha;
    m_Anim._fTarget.x = p->vTarget.x;
    m_Anim._fTarget.y = p->vTarget.y;
    m_Anim._fTarget.z = p->vTarget.z;
    m_Anim._fTarget.w = 0;
    INT& rVsync = VSYNC_TIMEQ; /* through a reference: loaded where used, and again after the calls */
    m_Anim._nTimeLimit = p->nTime * rVsync;
    m_Anim._nTimeCount = 0;
    m_Anim._Calc = vCur;
    FLOAT fRate = 1.0f / m_Anim._nTimeLimit;
    SubVector(vCur, m_Anim._fTarget, vCur);
    ScaleVector(vCur, vCur, fRate);
    m_Anim._fSpeed = vCur;
    m_Anim._nTimeLimit += rVsync;
    return -1;
}

INT CAtelBlur::onSetBlurEnd(INT nCmd, void* pParam, INT nID) {
    CPackMan::setFrameEffect(0, 0, 0, 0);
    m_nState = 0;
    return -1;
}

INT CAtelBlur::Anim() {
    m_Anim._nTimeCount += getFrameRate();
    BOOL bOver = m_Anim.isOverTimeLimit();
    if (!bOver) {
        FVECTOR v;
        ScaleVector(v, m_Anim._fSpeed, getFrameRate());
        AddVector(m_Anim._Calc, m_Anim._Calc, v);
        CPackMan::setFrameEffect(3, (INT)m_Anim._Calc.x, (INT)m_Anim._Calc.y, (INT)m_Anim._Calc.z);
    } else {
        m_Xofs = (INT)m_Anim._fTarget.x;
        m_Yofs = (INT)m_Anim._fTarget.y;
        m_Alpha = (INT)m_Anim._fTarget.z;
        CPackMan::setFrameEffect(3, m_Xofs, m_Yofs, m_Alpha);
        m_nState ^= 1;
    }
    return bOver;
}

} // namespace ATEL

// SOURCE DATA: .data .rodata
