#ifndef FORM_MENU_MENUMODEL_H
#define FORM_MENU_MENUMODEL_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Vector.h"
#include "System/Graphic/Gprim.h"

class CCoord;
class CModel;
class CAnim;
class CTexAnm;

class CMenuModel : public CTask {
private:
    INT m_nRsrcID;           /* 0x040 */
    BOOL m_bDisp;            /* 0x044 */
    CCoord* m_pParentCoord;  /* 0x048 */
    CModel* m_pModels;       /* 0x04c */
    CAnim* m_pAnim;          /* 0x050 */
    CTexAnm* m_pTexAnm;      /* 0x054 */
    INT m_nTexAnmNum;        /* 0x058 */
    INT m_nStep;             /* 0x05c */
    INT m_nModelCount;       /* 0x060 */
    FVECTOR m_vPos;          /* 0x070 */
    FMATRIX m_mWS;           /* 0x080 */
    G_XYZ m_PosScreen;       /* 0x0c0 */
    FVECTOR m_vPosWSC;       /* 0x0d0 */
    FLOAT m_fAlphaScale;     /* 0x0e0 */
    FVECTOR m_colorTarget;   /* 0x0f0 */
    FVECTOR m_colorCurrent;  /* 0x100 */
    FVECTOR m_colorOld;      /* 0x110 */
    FLOAT m_fTime;           /* 0x120 */
    FLOAT m_fFadeTime;       /* 0x124 */

public:
    CMenuModel(CTask* p, INT nRsrcID, CCoord* pParentCoord, INT iPosX, INT iPosY);
    virtual ~CMenuModel();
    virtual void main();
    virtual void disp();
    CCoord* getCoord(const CHAR* pBoneName);
    void changeAnim(INT nID, BOOL bHokan);
    void changeAnimEx(INT nRsrcID, INT nID, BOOL bHokan);
    void enableDisp(BOOL bDisp) { m_bDisp = bDisp; }
    BOOL isAnimEnd();
    void setAlphaScale(FLOAT fAlphaScale);
    void setColorScale(FVECTOR& vColor);
    void setPosScreen(INT iPosX, INT iPosY);
    void setScale(FLOAT fScale);
    void setSpeed(FLOAT fSpeed);
    void setColorFade(FVECTOR& vColor, FVECTOR& vColor2, FLOAT fFadeTime);

private:
    void changeLocalWS();
};

#endif /* FORM_MENU_MENUMODEL_H */
