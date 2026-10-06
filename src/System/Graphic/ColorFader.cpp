#include "ColorFader.h"

#define ALPHA_TIME_MAX 600  // longest fade
#define ALPHA_TIME_UNIT 50  // timer ticks per unit of nTime

BOOL CAlphaChanger::reqTargetAlpha(FLOAT fTarget, INT nTime) {
    if (nTime == 0) {
        m_fAlpha = fTarget;
        return TRUE;
    }
    if (fTarget < 0.f) {
        fTarget = 0.f;
    } else if (fTarget > 1.f) {
        fTarget = 1.f;
    }
    if (nTime > ALPHA_TIME_MAX) {
        nTime = ALPHA_TIME_MAX;
    }
    INT f = nTime * ALPHA_TIME_UNIT;
    clearReq();
    m_nFlag |= FLAG_REQ;
    m_fAlphaTarget = fTarget;
    m_nTimer = f;
    m_fAlphaSpeed = (fTarget - m_fAlpha) / m_nTimer;
    return TRUE;
}

void CAlphaChanger::updateAlpha(TIMEQ nTime) {
    m_nTimer -= nTime;
    if (m_nTimer > 0) {
        m_fAlpha += m_fAlphaSpeed * nTime;
    } else {
        m_fAlpha = m_fAlphaTarget;
        doneReq();
    }
}

void CColorFader::setBlack(FLOAT fAlpha) {
    m_nCol = 0xFF000000;
    setAlpha(fAlpha);
}

BOOL CColorFader::isBlack() const {
    return (m_nCol & 0xFFFFFF) == 0;
}

void CColorFader::setWhite(FLOAT fAlpha) {
    m_nCol = 0xFFFFFFFF;
    setAlpha(fAlpha);
}

BOOL CColorFader::isWhite() const {
    return (m_nCol & 0xFFFFFF) == 0xFFFFFF;
}

BOOL CColorFader::reqBlackFade(USHORT nTime, FLOAT fStart, FLOAT fEnd) {
    setBlack(fStart);
    return reqTargetAlpha(fEnd, nTime);
}

BOOL CColorFader::reqWhiteFade(USHORT nTime, FLOAT fStart, FLOAT fEnd) {
    setWhite(fStart);
    return reqTargetAlpha(fEnd, nTime);
}

// SOURCE DATA: .rodata
