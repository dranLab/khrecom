#ifndef SYSTEM_GRAPHIC_COLORFADER_H
#define SYSTEM_GRAPHIC_COLORFADER_H

#include "khtypes.h"

/* Fades the screen to/from black or white.
 * The inline virtuals have no out-of-line home, so IColorFader gets no vtable of its own; they are
 * emitted (weak) wherever a derived class's vtable needs them. */
class IColorFader {
public:
    virtual void setBlack(FLOAT fAlpha) = 0;
    virtual void setWhite(FLOAT fAlpha) = 0;
    virtual BOOL reqBlackFade(USHORT nTime, FLOAT fStart, FLOAT fEnd) = 0;
    virtual BOOL reqWhiteFade(USHORT nTime, FLOAT fStart, FLOAT fEnd) = 0;
    virtual BOOL reqBlackIn(USHORT nTime, FLOAT fStart) { return reqBlackFade(nTime, fStart, 1.f); }
    virtual BOOL reqBlackOut(USHORT nTime, FLOAT fStart) { return reqBlackFade(nTime, fStart, 0.f); }
    virtual BOOL reqWhiteIn(USHORT nTime, FLOAT fStart) { return reqWhiteFade(nTime, fStart, 0.f); }
    virtual BOOL reqWhiteOut(USHORT nTime, FLOAT fStart) { return reqWhiteFade(nTime, fStart, 1.f); }
};

/* Moves an alpha value in [0, 1] towards a target over time. */
class CAlphaChanger {
private:
    enum {
        FLAG_REQ = 1 << 0,   // a target was requested and is being approached
        FLAG_DONE = 1 << 1,  // the target was reached
    };

    FLOAT m_fAlpha;
    FLOAT m_fAlphaSpeed;
    FLOAT m_fAlphaTarget;
    SHORT m_nTimer;
    USHORT m_nFlag;

public:
    CAlphaChanger() { m_nFlag = 0; }
    void setAlpha(FLOAT fAlpha) {
        if (fAlpha < 0.f) {
            fAlpha = 0.f;
        } else if (fAlpha > 1.f) {
            fAlpha = 1.f;
        }
        m_fAlpha = fAlpha;
    }
    FLOAT getAlpha() const { return m_fAlpha; }
    BOOL reqTargetAlpha(FLOAT fTarget, INT nTime);
    BOOL hasReqTargetAlpha() const { return m_nFlag & FLAG_REQ; }
    BOOL isTargetAlphaDone() const { return (m_nFlag & FLAG_DONE) != 0; }
    void clearReq() { m_nFlag &= ~(FLAG_REQ | FLAG_DONE); }

protected:
    void updateAlpha(TIMEQ nTime);
    void doneReq() {
        m_nFlag &= ~FLAG_REQ;
        m_nFlag |= FLAG_DONE;
    }
};

class CColorFader : public IColorFader, public CAlphaChanger {
private:
    UINT m_nCol;

public:
    virtual void setBlack(FLOAT fAlpha);
    virtual void setWhite(FLOAT fAlpha);
    virtual BOOL reqBlackFade(USHORT nTime, FLOAT fStart, FLOAT fEnd);
    virtual BOOL reqWhiteFade(USHORT nTime, FLOAT fStart, FLOAT fEnd);
    UINT getColor() const { return m_nCol; }
    BOOL hasReqFade() const { return hasReqTargetAlpha(); }
    BOOL isFadeDone() const { return isTargetAlphaDone(); }
    BOOL isBlack() const;
    BOOL isWhite() const;
    void main(TIMEQ nTime) {
        if (hasReqTargetAlpha()) {
            updateAlpha(nTime);
        }
    }
};

#endif /* SYSTEM_GRAPHIC_COLORFADER_H */
