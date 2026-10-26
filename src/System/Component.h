#ifndef SYSTEM_COMPONENT_H
#define SYSTEM_COMPONENT_H

#include "khtypes.h"
#include "System/HeapMan.h"
#include "System/Task.h"
#include "System/PadHandler.h"
#include "System/Debug.h"

template <class T> struct POINT_T {
    T m_x;
    T m_y;
};

template <class T> struct SIZE_T {
    T m_w;
    T m_h;
};

template <class T> struct CPointT : POINT_T<T> {
    CPointT() {}
    CPointT(T x, T y) { m_x = x; m_y = y; } void setPoint(T x, T y) { m_x = x; m_y = y; } // PARTIAL: setPoint added for Game/Atel/VSME
    CPointT<T> operator+(const POINT_T<T>& pt) const { return CPointT<T>(m_x + pt.m_x, m_y + pt.m_y); }
};

template <class T> struct CSizeT : SIZE_T<T> {
    CSizeT() {}
    CSizeT(T w, T h) { m_w = w; m_h = h; } void setSize(T w, T h) { m_w = w; m_h = h; } // PARTIAL: setSize added for Game/Atel/VSME
    BOOL operator==(const SIZE_T<T>& sz) const { return m_w == sz.m_w && m_h == sz.m_h; } // PARTIAL: added for System/Wnd
};

typedef CPointT<int> CPointI;
typedef CSizeT<int> CSizeI;

class CComponent {
private:
    CPointI m_pos;
    CSizeI m_size;

public:
    CComponent() : m_pos(0, 0), m_size(0, 0) {}
    virtual ~CComponent() {}
    virtual void setPosition(const CPointI& inPos) { m_pos = inPos; }
    virtual void setSize(const CSizeI& inSize) { m_size = inSize; }
    virtual const CPointI& getPosition() const { return m_pos; }
    virtual const CSizeI& getSize() const { return m_size; }
};

class CComponentObj : public CComponent, public CTask, public CPadHandler {
private:
    union {
        struct {
            unsigned int m_bVisible : 1;
            unsigned int m_bActive : 1;
            unsigned int m_bIgnoreKey : 1;
        };
        INT m_nStatus;
    };
    CPointI m_offset;

public:
    CComponentObj(CTask& inParent) : CTask(&inParent, 0x4000) {
        m_bVisible = TRUE;
        setActive(TRUE);
    }
    virtual ~CComponentObj() {}
    virtual INT getTypeID() const;
    BOOL isVisible() const { return m_bVisible; }
    BOOL isActive() const { return m_bActive; }
    INT getStatus() const { return m_nStatus; }
    void setVisible(BOOL bInFlag) { m_bVisible = bInFlag; }
    void setActive(BOOL bInFlag) {
        m_bActive = bInFlag;
        m_bIgnoreKey = bInFlag;
    }
    virtual void mainImpl() {}
    virtual void dispImpl() {}
    virtual void setPosition(const CPointI& inPos);
    void setCompPosition(const CPointI& inPos);
    const CPointI& getCompPosition() const { return m_offset; }

protected:
    INT isButtonTrg(UINT b, UINT nInPad = 0) const {
        return m_bIgnoreKey ? 0 : CPadHandler::isButtonTrg(b, nInPad);
    }
    INT isButtonRTrg(UINT b, UINT nInPad = 0) const {
        return m_bIgnoreKey ? 0 : CPadHandler::isButtonRTrg(b, nInPad);
    }
    INT dbgButtonTrg(USER uid, UINT button, INT padId = 0) {
        return m_bIgnoreKey ? 0 : ::dbgButtonTrg(uid, button, padId);
    }
    INT dbgButtonRTrg(USER uid, UINT button, INT padId = 0) {
        return m_bIgnoreKey ? 0 : ::dbgButtonRTrg(uid, button, padId);
    }

public:
    virtual void main();
    virtual void disp();
};

#endif // SYSTEM_COMPONENT_H
