#ifndef SYSTEM_GRAPHIC_SCREENMASK_H
#define SYSTEM_GRAPHIC_SCREENMASK_H

#include "System/Graphic/Gprim.h"
#include "System/Graphic/ColorFader.h"
#include "System/Task.h"

class CFadeIOMask;

/* All inline members are emitted (weak) in ScreenMask.cpp, in declaration order: keep the order. */
class CScreenMaskPrim : public CGPrim {
private:
    G_RGBA m_col; /* 0x1c */
    INT m_nFlag;  /* 0x20 */
    friend class CFadeIOMask;

public:
    CScreenMaskPrim();
    virtual ~CScreenMaskPrim() {}
    virtual UINT getPacketSize() const;
    virtual FIX4 calcZ() { return 0xFFFF280; }
    void setColor(UINT col) { m_col.col = col; }
    void setColor(UBYTE r, UBYTE g, UBYTE b) { m_col.set(r, g, b); }
    void setColor(UBYTE r, UBYTE g, UBYTE b, UBYTE a) { m_col.set(r, g, b, a); }
    void setAlpha(UBYTE a) { m_col.a = a; }
    UINT getColor() const { return m_col.col; }
    void enableZbufClear(BOOL bSet) {
        if (bSet) {
            m_nFlag |= 1;
        } else {
            m_nFlag &= ~1;
        }
    }
private:
    virtual void makePkt();
};

class CFadeIOMask : public CColorFader, public CTask {
private:
    CScreenMaskPrim m_Prim; /* 0x60 */
    BOOL m_bSuspended;      /* 0x84 */

public:
    CFadeIOMask(CTask& inParent, USHORT nLayerDepth);
    virtual void setBlack(FLOAT fInAlpha);
    virtual void setWhite(FLOAT fInAlpha);
    void setLayer(USHORT nLayerDepth);
    void enableZbufClear(BOOL bSet) {
        if (bSet) {
            m_Prim.m_nFlag |= 1;
        } else {
            m_Prim.m_nFlag &= ~1;
        }
    }
    void on() {
        m_bSuspended = FALSE;
        m_nInfo &= ~(TASK_INFO_INACTIVE | TASK_INFO_INVISIBLE | TASK_INFO_INACTIVETRG);
    }
    void off() {
        m_bSuspended = FALSE;
        m_nInfo |= TASK_INFO_INACTIVE | TASK_INFO_INVISIBLE | TASK_INFO_INACTIVETRG;
    }
    void suspend() {
        if (!isInactive()) m_bSuspended = TRUE;
    }
    void resume() {
        if (!isInactive()) m_bSuspended = FALSE;
    }
    BOOL isSuspended() const { return m_bSuspended; }
    virtual void main();
    virtual void disp();
    UINT getColor() const { return m_Prim.getColor(); }
};

#endif /* SYSTEM_GRAPHIC_SCREENMASK_H */
