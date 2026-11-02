#ifndef GAME_ATEL_EVENTTRAP_H
#define GAME_ATEL_EVENTTRAP_H

#include "khtypes.h"
#include "System/Vector.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Camera.h"
#include "Game/Collision/CollUtilA.h"

struct LineTrapSrc {
    FVECTOR _vP[2];
};

struct BoxTrapSrc {
    FVECTOR _vPos;
    FLOAT _fWidth;
    FLOAT _fDepth;
    FLOAT _fHeight;
    FLOAT _fRotY;

    /* MATCH: member-wise copy as the original's CEventBox::setData does it (with the implicit one setData is 0x3c bytes instead of 0x58) */
    BoxTrapSrc& operator=(const BoxTrapSrc& inSrc) {
        _vPos = inSrc._vPos;
        _fWidth = inSrc._fWidth;
        _fDepth = inSrc._fDepth;
        _fHeight = inSrc._fHeight;
        _fRotY = inSrc._fRotY;
        return *this;
    }
};

namespace ATEL {

class CEventTrap {
private:
    EventTrapData m_chkData;
    USHORT m_nType;
    USHORT m_nFlag;

public:
    CEventTrap() {
        m_nType = 0;
        m_nFlag = 0;
    }
    virtual ~CEventTrap() {}
    virtual INT checkTrap(const ETMoveData& inData) = 0;
    void setActive(BOOL b) {
        if (b) {
            m_nFlag |= 1;
        } else {
            m_nFlag &= ~1;
        }
        onSetActive(b);
    }
    BOOL isActive() { return m_nFlag & 1; }
    USHORT getType() { return m_nType; }
    virtual void dispShape() {}

protected:
    virtual void onSetActive(BOOL b) {}
    void setType(USHORT nType) { m_nType = nType; }
    void setCheckData(UINT nType, FVECTOR& vTbl) {
        m_chkData._nType = nType;
        m_chkData._pVert = &vTbl;
    }
    EventTrapData& getCheckData() { return m_chkData; }
};

class CEventLine : public CEventTrap {
private:
    LineTrapSrc m_srcData;
    C3DLines m_Shape;
    CCoord m_Crd;

public:
    CEventLine() { setType(1); }
    void setData(const LineTrapSrc& inData);
    virtual INT checkTrap(const ETMoveData& inData);
    virtual void dispShape();
};

class CEventBox : public CEventTrap {
private:
    FMATRIX m_matVert;
    BoxTrapSrc m_srcData;
    C3DPolyG4 m_Shape[2];
    C3DLines m_Lines;
    CCoord m_Crd;

public:
    CEventBox() { setType(2); }
    void setData(const BoxTrapSrc& inData);
    virtual INT checkTrap(const ETMoveData& inData);

protected:
    void setupVertexes();
    const BoxTrapSrc& getTrapSrc() const { return m_srcData; }
    virtual void dispShape();
};

} // namespace ATEL

#endif /* GAME_ATEL_EVENTTRAP_H */
