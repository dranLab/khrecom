// PARTIAL: added for Game/Gimmick/GimmickPlantChild
#ifndef SYSTEM_GRAPHIC_MODEL_H
#define SYSTEM_GRAPHIC_MODEL_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Debug.h"
#include "System/Graphic/modeldata.h"
#include "System/Vector.h"
#include "System/Graphic/Gprim.h"

class CTask;
class CCoord;
class CCoords; // PARTIAL: added for System/Graphic/DispList
class CTexAnm;
class CLightUnit; // PARTIAL: added for Game/ModelObj
struct MODELDATA;

// PARTIAL: CModel derives from CDrawable (System/Graphic/Gprim.h, which every includer includes first) and
// its virtual functions added for Form/Menu/MenuModel
class CModel : public CDrawable {
public:
    // PARTIAL: added for System/Graphic/ModelMan
    static void resetSendCtrl(INT nMode);
    static void setSendCtrl(INT nMode); // PARTIAL: added for System/Graphic/ShadowModel
    static void resetWorldScreenMatrix();
    static void resetMicroProgram();
    static void disp0_SetMicroCode(u_long128* pMProg); // PARTIAL: added for System/Graphic/Effect/EffectMan
    FIX4 getOldSemiZ(); // PARTIAL: added for System/Graphic/Effect/EffectMan
private:
    UBYTE _m_0x18[0x14]; /* 0x018 .. m_ColorScale.z */
    FLOAT m_fAlphaScale; /* 0x02c m_ColorScale.w */
public:
    // PARTIAL: m_nFlag split out (public) for Game/Gimmick/GimmickDominoPillar, which sets bit 0x100
    // directly (an inline CModel setter adds an addiu per model and does not match)
    SHORT m_nFlag;       /* 0x030 */
private:
    USHORT m_nPacketSize;     /* 0x032 */ // PARTIAL: split out for System/Graphic/Model
    class CSemiModel* m_pSemiModel; /* 0x034 */
    class CFadeModel* m_pFadeModel; /* 0x038 */
    CLightUnit* m_pLightUnit; /* 0x03c PARTIAL: split out for Game/ModelObj */
    UBYTE _m_0x40[0x14]; /* 0x040 .. 0x054 */
    CCoords* m_pCrds;    /* 0x054 PARTIAL: split out for System/Graphic/DispList */
    const MODELDATA* m_pData; /* 0x058 */
    USHORT m_Mode;       /* 0x05c */ // PARTIAL: split out for System/Graphic/Model
    USHORT m_ShaderOption; /* 0x05e */
    INT m_BBoxFlag;        /* 0x060 */
    BYTE m_TransTexCount;  /* 0x064 */
    SBYTE m_BoneSkip;      /* 0x065 */
    unsigned char m_Pad[2];
    struct { BYTE* m_pTex; INT m_n; } m_TexInfo[16]; /* 0x068 TexInfoStub */
    void* m_TexUV;         /* 0x0e8 _TEX_UV_OFFSET* */
    void* m_pTexAnm;       /* 0x0ec CModelTexAnm* */
    CTask* m_pTask;      /* 0x0f0 */
    FMATRIX* m_pWSMat;   /* 0x0f4 */
    void* m_pShadowMapModel; /* 0x0f8 */
    BBOX m_BBox;         /* 0x0fc */
    struct SHADERDESC* m_pShader; /* 0x114 */
    UBYTE _m_0x118[0x8];
    static FMATRIX* m_pWSMatCache;
    static u_long128* m_pMpg;
    static INT m_iSendFlg;

public:
    CModel(const MODELDATA* pData = NULL); // PARTIAL: added for Form/Menu/MenuModel
    virtual ~CModel();                     // PARTIAL: added for Form/Menu/MenuModel
    INT link(CTask* p);                    // PARTIAL: added for Form/Menu/MenuModel
    void disp(); // PARTIAL: added for System/Graphic/MapUnit
    // PARTIAL: added for Form/Menu/MenuModel
    INT link(CTask* p, const MODELDATA* d) {
        m_pData = d;
        return link(p);
    }
    CCoord& getCrd(INT nIndex);                       // PARTIAL: added for Form/Menu/MenuModel
    MODELDATA* getModelData() { return (MODELDATA*)m_pData; } // PARTIAL: added for Game/Map
    CCoords* getCoords() { return m_pCrds; }          // PARTIAL: added for System/Graphic/DispList
    void getWorldBBox(FVECTOR& vMax, FVECTOR& vMin);  // PARTIAL: added for System/Graphic/DispList
    u_long128* getMicroProg();                        // PARTIAL: added for System/Graphic/DispList
    INT getBoneIndex(const CHAR* pName) const;        // PARTIAL: added for Form/Menu/MenuModel
    void linkTexAnm(CTexAnm* pTexAnm, INT nTexAnm);   // PARTIAL: added for Form/Menu/MenuModel
    void setColorScale(const FVECTOR& vColor);        // PARTIAL: added for Form/Menu/MenuModel
    void setWorldScreenMatrix(FMATRIX* pWSMat) { m_pWSMat = pWSMat; } // PARTIAL: added for Form/Menu/MenuModel
    virtual void dispImpl();                          // PARTIAL: added for Form/Menu/MenuModel
    virtual void dispUpdate();
    virtual BOOL dispCheck();
    virtual void changeLayerExt(INT a, INT b);        // PARTIAL: parameters guessed, for Form/Menu/MenuModel
    FLOAT getAlphaScale() { return m_fAlphaScale; }
    // PARTIAL: added for Game/ModelObj (m_ColorScale is the FVECTOR at 0x020)
    FVECTOR& getColorScale() { return *(FVECTOR*)&_m_0x18[8]; }
    void setLightUnit(CLightUnit* p) { m_pLightUnit = p; } // PARTIAL: added for Game/ModelObj
    void setAlphaScale(FLOAT inScale);
    void changeTexture(const CHAR* inSrc, const CHAR* inDst); // PARTIAL: added for Game/Enemy/MapEnemyShadow
    // PARTIAL: added for System/Graphic/Model
    void setColorScale(FLOAT r, FLOAT g, FLOAT b, FLOAT a);
    void setShadowSize(INT nSize);
    void setModelData(const MODELDATA* pData);
    void releaseData();
    void sendTexture(u_long* pTag);
    void _disp0_SetTexture(INT nFlag, u_long* pTag);
    u_long* _disp0_GetData(UINT& nSize, INT nFlag);
    void disp(FLOAT fRate);
    INT getCoordNum() const;
    INT link(CCoords& crds);
    void calcWorldBBox(INT n);
    void* getExtendClut(INT nTex, INT nType);
};

#ifdef GAME_ENTITY_H
inline CModel* CEntity::getModel() { return m_pModels; }
inline CModel* CEntity::getModel(INT inIndex) { return &m_pModels[inIndex]; }
#endif

/* Placeholders for the original header's inline functions: their strings ("System/Graphic/Model.h",
 * "m_pData != __null", "System/Graphic/Model.h(569)", "0", and CMCShader's "create CMCShader\n" /
 * "delete CMCShader\n") are in .rodata of every includer, in this order. Only line 569 is known. */
/* PARTIAL: MODEL_H_PATH added for System/VramMan, whose original includes it as "System/Graphic/model.h" */
#ifndef MODEL_H_PATH
#define MODEL_H_PATH "System/Graphic/Model.h"
#endif
#line 1 MODEL_H_PATH
inline void _modelCheckData(void* inData) {
    if (!(inData != __null)) AssertApp(__FILE__, __LINE__, "m_pData != __null", 0);
}
inline void* _modelCreate() {
    return com_malloc_d(4, __FILE__ "(569)", (UINT)-1);
}
inline void _modelAssertZero() {
    AssertApp(__FILE__, __LINE__, "0", 0);
}
inline void _mcShaderCreate() {
    TRACE("create CMCShader\n");
}
inline void _mcShaderDelete() {
    TRACE("delete CMCShader\n");
}

#endif /* SYSTEM_GRAPHIC_MODEL_H */
