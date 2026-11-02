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
    friend class CDispListShadowMap; // PARTIAL: added for System/Graphic/DispList
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
    // PARTIAL: added for Game/GameSubField
    static void setAutomatic() { m_iStatus &= ~4; }
    static BOOL isDraw() { return !((m_iStatus >> 2) & 1); } // PARTIAL: added for System/Graphic/ModelMan (bit 4 = shadows off, from the inlined test)
    static void setUpPer(FLOAT f) { m_fUpPer = f; }
    static FLOAT getUpPer() { return m_fUpPer; }     // PARTIAL: added for System/Graphic/Model
    static FLOAT getDownLen() { return m_fDownLen; }
    static FMATRIX& getWorldScreenMatrix() { return m_mWorldScreen; }
    // PARTIAL: added for System/Graphic/Model
    static FLOAT getColorDensity() { return m_fCDensity; }
    static FLOAT getAlphaDensity() { return m_fADensity; }
    static FMATRIX& getWorldTextureMatrix() { return m_mWorldTexture; }
    static FVECTOR& getShadowParam() { return m_vParam; }
    static ULONGLONG getTEX_0() { return m_lTex0; }
    static void setDownLen(FLOAT f) { m_fDownLen = f; }
    // PARTIAL: added for Game/Map (the stores go through a reference: the original reloads m_pFog after each)
    static void setColorDensity(FLOAT fDensity) { FLOAT& r = m_fCDensity; r = fDensity; }
    static void setAlphaDensity(FLOAT fDensity) { FLOAT& r = m_fADensity; r = fDensity; }
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
