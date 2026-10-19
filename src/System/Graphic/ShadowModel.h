#ifndef SYSTEM_GRAPHIC_SHADOWMODEL_H
#define SYSTEM_GRAPHIC_SHADOWMODEL_H

#include "khtypes.h"
#include "System/Vector.h"

template <class T>
struct RECT_T {
    T m_left;
    T m_top;
    T m_right;
    T m_bottom;
};
typedef RECT_T<int> IRECT;

struct SCREEN;

class CShadowModelMan {
private:
    static FMATRIX m_mWorldTexture;
    static FMATRIX m_mWorldScreen;
    static FMATRIX m_mProjectionTexture;
    static FMATRIX m_mProjectionScreen;
    static UINT m_iStatus;
    static FLOAT m_fCDensity;
    static FLOAT m_fADensity;
    static FLOAT m_fUpPer;
    static FLOAT m_fDownLen;
    static FLOAT m_fClipDistSq;
    static ULONGLONG m_lTex0;
    static FVECTOR m_vParam;
    static FVECTOR m_vCamPos;

public:
    static void initialize();
    static void finalize();
    static void updateMatrix();
    static void updateShadowMatrix(const FVECTOR& vMax, const FVECTOR& vMin, INT nSize);
    static void drawBefore();
    static void drawAfter();
};

void getScreenAreaClipToWorldPos(FVECTOR& sv, FVECTOR& ev, FVECTOR* av, const FMATRIX& wvm, const FMATRIX& wsm,
                                 const SCREEN* scr, const IRECT* clips, float height, float limit);

#endif /* SYSTEM_GRAPHIC_SHADOWMODEL_H */
