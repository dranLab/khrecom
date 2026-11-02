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
    static void setAutomatic() { m_iStatus &= ~4; }
    static void setAutomaticDistance(/* parameters unknown */);
    static void setAutomaticHeight(/* parameters unknown */);
    static void setManualDistance(/* parameters unknown */);
    static void setManualHeight(/* parameters unknown */);
    /* The stores go through a reference: the original reloads m_pFog after each (Game/Map). */
    static void setColorDensity(FLOAT fDensity) { FLOAT& r = m_fCDensity; r = fDensity; }
    static void setAlphaDensity(FLOAT fDensity) { FLOAT& r = m_fADensity; r = fDensity; }
    static void setDraw(/* parameters unknown */);
    static void setUpPer(FLOAT f) { m_fUpPer = f; }
    static void setDownLen(FLOAT f) { m_fDownLen = f; }
    static void setClipDistance(/* parameters unknown */);
    static void initialize();
    static void finalize();
    static BOOL isDraw() { return !((m_iStatus >> 2) & 1); } // bit 4 = shadows off (the inlined test in ModelMan)
    static FLOAT getColorDensity() { return m_fCDensity; }
    static FLOAT getAlphaDensity() { return m_fADensity; }
    static FLOAT getUpPer() { return m_fUpPer; }
    static FLOAT getDownLen() { return m_fDownLen; }
    static FLOAT getClipDistanceSq() { return m_fClipDistSq; }
    static FMATRIX& getWorldTextureMatrix() { return m_mWorldTexture; }
    static FMATRIX& getWorldScreenMatrix() { return m_mWorldScreen; }
    static FVECTOR& getShadowParam() { return m_vParam; }
    static FVECTOR& getUpdateCameraPos() { return m_vCamPos; }
    static ULONGLONG getTEX_0() { return m_lTex0; }
    static void updateMatrix();
    static void updateShadowMatrix(const FVECTOR& vMax, const FVECTOR& vMin, INT nSize);
    static void drawBefore();
    static void drawAfter();
};

void getScreenAreaClipToWorldPos(FVECTOR& sv, FVECTOR& ev, FVECTOR* av, const FMATRIX& wvm, const FMATRIX& wsm,
                                 const SCREEN* scr, const IRECT* clips, float height, float limit);

#endif /* SYSTEM_GRAPHIC_SHADOWMODEL_H */
