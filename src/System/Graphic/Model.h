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
class CSemiModel;
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
    u_long128* m_pBeforeDirect;  /* 0x040 */
    u_long128* m_pAfterDirect;   /* 0x044 */
    USHORT m_iBeforeDirectQNum;  /* 0x048 */
    USHORT m_iAfterDirectQNum;   /* 0x04a */
    FIX4 m_iOTZ;                 /* 0x04c */
    SHORT m_BoneIdx;             /* 0x050 */
    SHORT m_BoneStart;           /* 0x052 */
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
    virtual void dispUpdate(FLOAT fR);
    virtual BOOL dispCheck();
    virtual void makePkt(); // PARTIAL: added for System/Graphic/Model
    BOOL isDataEmpty() const { return m_pData->isDataEmpty(); }
    CSemiModel* getSemiModel() { return m_pSemiModel; }
protected:
    BOOL isManagedSemiModel() const { return m_nFlag & 1; }
    BOOL isAvailableSemiModel() const { return m_nFlag & 2; }
public:
    void attachSemiModel(CSemiModel& inModel, BOOL bInManage);
    FIX4 getSemiZ();
    void getWorldBBoxScr(IVECTOR& vScr, const FMATRIX* pWS);
    void disp0(INT nFlag, u_long nTest, INT nPass);
    virtual void changeLayerExt(INT a, INT b);        // PARTIAL: parameters guessed, for Form/Menu/MenuModel
    FLOAT getAlphaScale() { return m_fAlphaScale; }
    INT getBeforeDirectNum() const { return m_iBeforeDirectQNum; } // PARTIAL: added for System/Graphic/Model
    INT getAfterDirectNum() const { return m_iAfterDirectQNum; }
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
    INT getTextureIndex(const CHAR* pName) const;
    INT link(CTask* p, const CHAR* pName);
    INT link(CCoords& crds, const CHAR* pName);
    INT assign(CTask* p, INT n);
    INT assign(CCoords& crds, INT n);
    INT assign(CCoords& crds, const CHAR* pName, INT n);
    INT assign(CTask* p, const CHAR* pName, INT n);
    void changeTextureSub(INT nIdx, const struct TexInfo* pTex);
    void setupUVScroll(INT n);
    void setUVScrollOffset(INT nNum, FLOAT fU, FLOAT fV); // PARTIAL: added for System/Graphic/Model
    void updateUVScroll(FLOAT fR);
#ifdef FORM_MENU_JIMINYMEMO_CPP
    inline void setSemiTrans(ALPHA nAlpha); // PARTIAL: added for Form/Menu/JiminyMemo (defined there)
#endif
};

#ifdef GAME_ENTITY_H
inline CModel* CEntity::getModel() { return m_pModels; }
inline CModel* CEntity::getModel(INT inIndex) { return &m_pModels[inIndex]; }
#endif

#ifdef SYSTEM_GRAPHIC_MODEL_CPP
class CShader;
struct SHADERDESC {
    USHORT m_Mode;                 /* 0x00 */
    USHORT m_Mask;                 /* 0x02 */
    u_long128* m_pMpg;             /* 0x04 */
    INT m_BoneSize;                /* 0x08 */
    INT m_WorkSize;                /* 0x0c */
    CShader* (*createInstance)();  /* 0x10 */
    ALPHA m_BaseAlpha;             /* 0x14 */
    ALPHA m_SemiAlpha;             /* 0x16 */
    ULONGLONG m_BaseTest;          /* 0x18 */
    ULONGLONG m_SemiTest;          /* 0x20 */
};

/* PARTIAL: CSemiModel, CSemiZModel, CFadeModel and CBBox for System/Graphic/Model only (System/Graphic/DispList
   and Form/Menu/JiminyMemo still declare CSemiModel stand-ins) */
class CSemiZModel;
class CSemiModel : public CDrawable {
protected:
    CModel* m_pModel;        /* 0x18 */
    CSemiZModel* m_pSemiZ;   /* 0x1c */
    FIX4 m_nOldZ;            /* 0x20 */
    USHORT m_nPacketSize;    /* 0x24 */
    BYTE m_SemiDraw;         /* 0x26 */
    BYTE __pad;              /* 0x27 */

public:
    CSemiModel();
    virtual ~CSemiModel();
    void reset();
    virtual void disp();
    virtual FIX4 calcZ();
    virtual void makePkt();
    void link(CModel* m);
    void relink(CModel* m);
    void setSemiTrans(ALPHA a);
    void setSemiSort(BOOL bVal);
    virtual UINT getPacketSize() const;
    void dispDirect();
    CModel* getParentModel();
    void initSemiZModel();
    void releaseSemiZModel();
    virtual void changeLayerExt(BOOL bOn, INT nDepth);
    void setOldZ(FIX4 nZ) { m_nOldZ = nZ; }
    FIX4 getOldZ() { return m_nOldZ; }
protected:
    void setDefaultSemiTrans();
    void replaceParentModel(CModel* pInModel);
    void setPacketSize(USHORT nSize);
};

class CSemiZModel : public CSemiModel {
public:
    CSemiZModel();
    virtual ~CSemiZModel();
    virtual void changeLayerExt(BOOL bOn, INT nDepth);
    virtual FIX4 calcZ();
    virtual void makePkt();
};

class CFadeModel : public CSemiModel {
public:
    CFadeModel();
    virtual ~CFadeModel();
    virtual void makePkt();
    virtual UINT getPacketSize() const;
};

class CModelBG : public CModel {
public:
    CModelBG(USHORT nInDepth);
    virtual ~CModelBG();
    virtual void disp();
private:
    virtual void makePkt();
    void setLayerFG(BYTE nInDepth);
};

class CBBox : public CGPrim {
    CCoord* m_pCrd;  /* 0x1c */
    G_RGBA m_C0;     /* 0x20 */
    BBOX m_bbox;     /* 0x24 */

public:
    CBBox();
    virtual ~CBBox();
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};
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
