#ifndef GAME_INTERPOLATEMOVE_H
#define GAME_INTERPOLATEMOVE_H

#include "khtypes.h"

#include "System/Vector.h"

struct CInterpolateMove {
protected:
    FLOAT m_fRate;
    FLOAT m_fAdd;
    UINT m_nType;

    void _setup(INT nType, FLOAT fFrame);
    FLOAT _update(FLOAT fR, BOOL bNoLimit);
};

struct CInterpolateVector : CInterpolateMove {
private:
    FVECTOR m_vPos;
    FVECTOR m_vAim;
    FVECTOR m_vOld;

public:
    void setupFrame(INT nType, const FVECTOR& vAim, const FVECTOR& vOld, FLOAT fFrame);
    void setupSpeed(INT nType, const FVECTOR& vAim, const FVECTOR& vOld, FLOAT fSpeed);
    BOOL update(FLOAT fR, BOOL bNoLimit);
    FVECTOR& getPos() { return m_vPos; }
    FVECTOR& getAim() { return m_vAim; }
    FVECTOR& getOld() { return m_vOld; }
    void getRot(FVECTOR& vRot);
};

struct CInterpolateScalar : CInterpolateMove {
private:
    FLOAT m_fPos;
    FLOAT m_fAim;
    FLOAT m_fOld;

public:
    void setupFrame(INT nType, FLOAT fAim, FLOAT fOld, FLOAT fFrame);
    void setupSpeed(INT nType, FLOAT fAim, FLOAT fOld, FLOAT fSpeed);
    BOOL update(FLOAT fR, BOOL bNoLimit);
    FLOAT getPos() { return m_fPos; }
    FLOAT getAim() { return m_fAim; }
    FLOAT getOld() { return m_fOld; }
};

#endif // GAME_INTERPOLATEMOVE_H
