
#ifndef SYSTEM_GRAPHIC_MODEL_H
#define SYSTEM_GRAPHIC_MODEL_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Debug.h"
#include "System/Graphic/modeldata.h"
#include "System/Vector.h"
#include "System/Graphic/Gprim.h"

/* MATCH: path override MODEL_H_PATH for System/VramMan, whose original includes it as "System/Graphic/model.h" */
#ifndef MODEL_H_PATH
#define MODEL_H_PATH "System/Graphic/Model.h"
#endif
#line 20 MODEL_H_PATH
class CTask;
class CCoord;
class CCoords;
class CTexAnm;
class CSemiModel;
class CFadeModel;
class CModel;
union _TEX_UV_OFFSET {
    struct {
        FLOAT fu;
        FLOAT fv;
    };
    ULONGLONG luv;
    _TEX_UV_OFFSET() { luv = 0; }
};
class CLightUnit;
struct MODELDATA;

#include "System/Graphic/ModelMan.h"

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
    CSemiModel() {
        m_iFlag = 0;
        m_nZ = 0;
        m_pModel = NULL;
        m_pSemiZ = NULL;
        m_nOldZ = 0;
        m_nPacketSize = 0;
        m_SemiDraw = TRUE;
        reset();
    }
    virtual ~CSemiModel() { releaseSemiZModel(); }
    void reset() {
        setDefaultSemiTrans();
        setTestFlag(0x51001);
        setLayer(0x10, 0);
    }
    virtual void disp() { CModelMan::add2DispList(*this, calcZ()); }
    virtual FIX4 calcZ();
    virtual void makePkt();
    void link(CModel* m);
    void relink(CModel* m) { link(m); }
    void setSemiTrans(ALPHA a);
    void setSemiSort(BOOL bVal) {
        if (bVal) m_iFlag |= 1;
        else m_iFlag &= ~1;
    }
    virtual UINT getPacketSize() const;
    void dispDirect();
    CModel* getParentModel() { return m_pModel; }
    void initSemiZModel();
    void releaseSemiZModel();
    virtual void changeLayerExt(BOOL bOn, INT nDepth);
    void setOldZ(FIX4 nZ) { m_nOldZ = nZ; }
    FIX4 getOldZ() { return m_nOldZ; }
protected:
    void setDefaultSemiTrans() { setSemiTrans(ALPHA_PLUS); }
    void replaceParentModel(CModel* pInModel) { m_pModel = pInModel; }
    void setPacketSize(USHORT nSize) { m_nPacketSize = nSize; }
};

class CFadeModel : public CSemiModel {
public:
    CFadeModel() { m_SemiDraw = FALSE; }
    virtual void makePkt();
    virtual UINT getPacketSize() const;
};

class CSemiZModel : public CSemiModel {
public:
    CSemiZModel() {
        CDrawable::setSemiTrans(ALPHA_PLUS);
        setTestFlag(0x52001);
        setLayer(0x15, 0);
    }
    virtual void changeLayerExt(BOOL bOn, INT nDepth) {
        if (bOn) setLayer(0x1B, nDepth);
        else setLayer(0x15, 0);
    }
    virtual FIX4 calcZ() { return 0x0FFFF800; }
    virtual void makePkt();
};

// PARTIAL: differs from the ref in m_pData (const), m_TexInfo (stand-in struct), the guessed friends and the public
// 0x030 union
class CModel : public CDrawable {
    friend class CSemiZModel; // MATCH: CSemiZModel::makePkt sets m_ShaderOption, no accessor exists (friends are not in the debug info)
    friend class CSemiModel;  // MATCH: CSemiModel::calcZ reads m_pFadeModel, no accessor exists
    friend class CFadeModel;  // MATCH: CFadeModel::makePkt calls the protected disp0
private:
    FVECTOR m_ColorScale; /* 0x020 */
public:
    // MATCH: the 0x030 union is public for Game/Gimmick/GimmickDominoPillar, which sets bit 0x100 of m_nFlag
    // directly (an inline CModel setter adds an addiu per model and does not match)
    union {
        u_long128 _ZeroInitializer;
        struct {
            SHORT m_nFlag;                  /* 0x030 */
            USHORT m_nPacketSize;           /* 0x032 */
            class CSemiModel* m_pSemiModel; /* 0x034 */
            class CFadeModel* m_pFadeModel; /* 0x038 */
            CLightUnit* m_pLightUnit;       /* 0x03c */
        };
    };
private:
    union {
        u_long128 _ZeroInitializer2;
        struct {
            u_long128* m_pBeforeDirect; /* 0x040 */
            u_long128* m_pAfterDirect;  /* 0x044 */
            USHORT m_iBeforeDirectQNum; /* 0x048 */
            USHORT m_iAfterDirectQNum;  /* 0x04a */
            FIX4 m_iOTZ;                /* 0x04c */
        };
    };
    SHORT m_BoneIdx;             /* 0x050 */
    SHORT m_BoneStart;           /* 0x052 */
    CCoords* m_pCrds;    /* 0x054 */
    const MODELDATA* m_pData; /* 0x058 */
    USHORT m_Mode;       /* 0x05c */
    USHORT m_ShaderOption; /* 0x05e */
    INT m_BBoxFlag;        /* 0x060 */
    BYTE m_TransTexCount;  /* 0x064 */
    SBYTE m_BoneSkip;      /* 0x065 */
    unsigned char m_Pad[2];
    struct { BYTE* m_pTex; INT m_n; } m_TexInfo[16]; /* 0x068 TexInfoStub */
    union _TEX_UV_OFFSET* m_TexUV; /* 0x0e8 */
    class CModelTexAnm* m_pTexAnm; /* 0x0ec */
    CTask* m_pTask;      /* 0x0f0 */
    FMATRIX* m_pWSMat;   /* 0x0f4 */
    class CShadowMapModel* m_pShadowMapModel; /* 0x0f8 */
    BBOX m_BBox;         /* 0x0fc */
    struct SHADERDESC* m_pShader; /* 0x114 */
    static FMATRIX* m_pWSMatCache;
    static u_long128* m_pMpg;
    static INT m_iSendFlg;

public:
    CModel(const MODELDATA* pData = NULL);
    virtual ~CModel();
    INT link(CTask* p);
    INT link(CTask* p, const MODELDATA& d) {
        m_pData = &d;
        return link(p);
    }
    INT link(CTask* p, const MODELDATA* d) {
        m_pData = d;
        return link(p);
    }
    INT link(CTask* p, const CHAR* pName);
    INT link(CCoords& crds);
    INT link(CCoords& crds, const MODELDATA& d) {
        m_pData = &d;
        return link(crds);
    }
    INT link(CCoords& crds, const MODELDATA* d) {
        m_pData = d;
        return link(crds);
    }
    INT link(CCoords& crds, const CHAR* pName);
    INT assign(CTask* p, INT n);
    INT assign(CTask* p, MODELDATA& d, INT n) {
        m_pData = &d;
        return assign(p, n);
    }
    INT assign(CTask* p, MODELDATA* d, INT n) {
        m_pData = d;
        return assign(p, n);
    }
    INT assign(CTask* p, const CHAR* pName, INT n);
    INT assign(CCoords& crds, INT n);
    INT assign(CCoords& crds, MODELDATA& d, INT n) {
        m_pData = &d;
        return assign(crds, n);
    }
    INT assign(CCoords& crds, MODELDATA* d, INT n) {
        m_pData = d;
        return assign(crds, n);
    }
    INT assign(CCoords& crds, const CHAR* pName, INT n);
    void setSemiTrans(ALPHA nAlpha) {
        if (m_pSemiModel) m_pSemiModel->setSemiTrans(nAlpha);
    }
    void attachSemiModel(CSemiModel& inModel, BOOL bInManage);
    CSemiModel* getSemiModel() { return m_pSemiModel; }
    void setSemiSort(BOOL bVal) {
        if (m_pSemiModel) {
            m_pSemiModel->setSemiSort(bVal);
            setSemiSortFlag(bVal);
        }
    }
    void setLightUnit(CLightUnit& inLight) { m_pLightUnit = &inLight; }
    void setLightUnit(CLightUnit* pInLight) { m_pLightUnit = pInLight; }
    CCoord& getCrd(INT nIndex);
    INT getCoordNum() const;
    CCoords* getCoords() const { return m_pCrds; }
    virtual void disp();
    void disp(FLOAT fRate);
    virtual void dispImpl();
    virtual void dispUpdate(FLOAT fR);
    virtual BOOL dispCheck();
    virtual UINT getPacketSize() const { return m_nPacketSize + getBeforeDirectNum() + getAfterDirectNum(); }
    BOOL isDataEmpty() const { return m_pData->isDataEmpty(); }
    BOOL isSemiDataEmpty() const { return m_pData->isSemiDataEmpty(); }
    INT getTextureIndex(const CHAR* pName) const;
    void changeTexture(const CHAR* inSrc, const CHAR* inDst);
    void changeTexture(INT nIdx, const struct TexInfo* pTex) { changeTextureSub(nIdx, pTex); }
    INT getBoneIndex(const CHAR* pName) const;
    LPCSTR getBoneName(INT nIndex) const { return (LPCSTR)((const BONELABEL*)m_pData->m_pBoneLabel)[nIndex].m_Name; }
    LPCSTR getParentBoneName(INT nIndex) const {
        const BONELABEL* pLabel = (const BONELABEL*)m_pData->m_pBoneLabel;
        return (LPCSTR)pLabel[pLabel[nIndex].m_ParentIndex].m_Name;
    }
    UINT getMode() { return m_Mode; }
    INT checkBBox();
    INT getBBoxFlag() { return m_BBoxFlag; }
    void setBBoxFlag(INT nFlag) { m_BBoxFlag = nFlag; }
    BBOX* getBBox(INT nIndex) {
        if (!(m_pData != NULL)) AssertApp(__FILE__, 262, "m_pData != __null", 0);
        return &m_pData->m_pBBox[nIndex];
    }
    MODELDATA* getModelData() { return (MODELDATA*)m_pData; }
    void setModelData(const MODELDATA* pData);
    struct TexInfoStub* getTextureInfo(INT nIndex) {
        if (!m_pData || nIndex < 0 || nIndex >= m_pData->m_nTexNum) return NULL;
        return (struct TexInfoStub*)&m_TexInfo[nIndex];
    }
    BOOL setTextureInfo(INT nIndex, const struct TexInfoStub* pInfo) {
        if (!m_pData || nIndex < 0 || nIndex >= m_pData->m_nTexNum || !pInfo) return FALSE;
        *(u_long*)&m_TexInfo[nIndex] = *(const u_long*)pInfo;
        return TRUE;
    }
    void updateTextureInfo() {
        if (m_pData) {
            INT nNum = m_pData->m_nTexNum;
            for (INT i = 0; i < nNum; i++) {
                *(u_long*)&m_TexInfo[i] = *(u_long*)((BYTE*)m_pData->m_pTexParam + i * 0x20 + 0x10);
            }
        }
    }
    INT getTextureNum() const {
        if (!m_pData) return -1;
        return m_pData->m_nTexNum;
    }
    INT getTransTextureNum() const { return m_TransTexCount; }
    void addTexture(const CHAR* texName, INT idx);
    void fade2Path(BOOL bOn) {
        if (bOn) m_nFlag |= 0x20;
        else m_nFlag &= ~0x20;
    }
    BOOL isFade2Path() const { return m_nFlag & 0x20; }
    void notSemi(BOOL bOn) {
        if (bOn) m_nFlag |= 0x800;
        else m_nFlag &= ~0x800;
    }
    BOOL isNotSemi() const { return (USHORT)m_nFlag & 0x800; }
    void dispOff(BOOL bOn) {
        if (bOn) m_nFlag |= 0x4000;
        else m_nFlag &= ~0x4000;
    }
    BOOL isDispOff() const { return (USHORT)m_nFlag & 0x4000; }
    BOOL isDisp() const { return m_nFlag & 8; }
    void setBeforeDirect(const u_long128* pDirect, INT nQNum) {
        m_pBeforeDirect = (u_long128*)pDirect;
        m_iBeforeDirectQNum = nQNum;
    }
    void clearBeforeDirect(const u_long128* pDirect, INT nQNum) {
        m_pBeforeDirect = NULL;
        m_iBeforeDirectQNum = 0;
    }
    INT getBeforeDirectNum() const { return m_iBeforeDirectQNum; }
    void setAfterDirect(const u_long128* pDirect, INT nQNum) {
        m_pAfterDirect = (u_long128*)pDirect;
        m_iAfterDirectQNum = nQNum;
    }
    void clearAfterDirect(const u_long128* pDirect, INT nQNum) {
        m_pAfterDirect = NULL;
        m_iAfterDirectQNum = 0;
    }
    INT getAfterDirectNum() const { return m_iAfterDirectQNum; }
    FIX4 getOTZ() const { return m_iOTZ; }
    void setOTZ(FIX4 nZ) {
        m_iOTZ = nZ;
        setOTZFlag(TRUE);
    }
    void setColorScale(const FVECTOR& vColor);
    void setColorScale(FLOAT r, FLOAT g, FLOAT b, FLOAT a);
    void setAlphaScale(FLOAT inScale);
    void linkTexAnm(CTexAnm* pTexAnm, INT nTexAnm);
    void setLocal(INT index);
    FVECTOR& getColorScale() { return m_ColorScale; }
    FLOAT getAlphaScale() { return m_ColorScale.w; }
    void dispWireframe(BOOL bOn) {
        if (bOn) m_nFlag |= 4;
        else m_nFlag &= ~4;
    }
    BOOL isDispWireframe() const { return m_nFlag & 0x4; }
    void setWorldScreenMatrix(const FMATRIX* pWSMat) { m_pWSMat = (FMATRIX*)pWSMat; }
    void setShadowMapModel();
    void defaultShaderOption() {
        if (m_pData) m_ShaderOption = m_pData->getShaderOption();
    }
    void setShaderOption(USHORT nOpt) { m_ShaderOption |= nOpt; }
    void resetShaderOption(USHORT nOpt) { m_ShaderOption &= ~nOpt; }
    virtual void changeLayerExt(BOOL bOn, INT nDepth) {
        if (bOn) setLayer(0x1B, nDepth);
        else setLayer(0x1A, 0);
        if (m_pFadeModel) m_pFadeModel->changeLayerExt(bOn, nDepth + 1);
        if (m_pSemiModel) m_pSemiModel->changeLayerExt(bOn, nDepth + 1);
    }
    void* getExtendClut(INT nTex, INT nType);
    void getChangeClut(INT inIdx, INT nInType);
    BOOL isClampRepeat() const { return m_nFlag & 0x40; }
    void setClampRepeat(BOOL bOn) {
        if (bOn) m_nFlag |= 0x40;
        else m_nFlag &= ~0x40;
    }
    BOOL isShadowMap() const { return !((USHORT)m_nFlag & 0x100); }
    void setShadowMap(BOOL bOn) {
        if (!bOn) m_nFlag |= 0x100;
        else m_nFlag &= ~0x100;
    }
    BOOL isAutoEffSemiSort() const { return !((USHORT)m_nFlag & 0x1000); }
    void setAutoEffSemiSort(BOOL bOn) {
        if (!bOn) m_nFlag |= 0x1000;
        else m_nFlag &= ~0x1000;
    }
    BOOL isRootZSort() const { return (USHORT)m_nFlag & 0x2000; }
    void setRootZSort(BOOL bOn) {
        if (bOn) m_nFlag |= 0x2000;
        else m_nFlag &= ~0x2000;
    }
    BOOL isShadow() const {
        if (m_pData) return m_pData->isShadow();
        return FALSE;
    }
    void setShadowSize(INT nSize);
    BOOL isMap() { return MODELDATA::isMap(m_Mode); }
    FIX4 getSemiZ();
    FIX4 getOldSemiZ();
    void getWorldBBox(FVECTOR& vMax, FVECTOR& vMin);
    void calcWorldBBox(INT n);
    void getWorldBBoxScr(IVECTOR& vScr, const FMATRIX* pWS);
    u_long128* getMicroProg();
private:
    void releaseData();
    void linksub();
    BOOL sendTexture(u_long* tex0);
    INT getTexParam(u_long* tex0);
    void changeTextureSub(INT nIdx, const struct TexInfo* pTex);
    virtual void makePkt();
    u_long* _disp0_GetData(UINT& nSize, INT nFlag);
public:
    static void disp0_SetMicroCode(u_long128* pMProg);
private:
    void _disp0_SetTexture(INT nFlag, u_long* pTag);
    void _disp0_SetPacket(UINT nMode, const int (*pData)[4], INT semikind, u_long nInTest, INT iFlags, u_long* tex0);
protected:
    void disp0(INT semikind, u_long nInTest, INT iFlags);
    virtual CSemiModel* createSemiModel() { return new (__FILE__ "(569)") CSemiModel; }
    BOOL isManagedSemiModel() const { return m_nFlag & 1; }
    void manageSemiModel(BOOL bOn) {
        if (bOn) m_nFlag |= 1;
        else m_nFlag &= ~1;
    }
    BOOL isAvailableSemiModel() const { return m_nFlag & 2; }
    void availableSemiModel(BOOL bOn) {
        if (bOn) m_nFlag |= 2;
        else m_nFlag &= ~2;
    }
    void setDispFlag(BOOL bOn) {
        if (bOn) m_nFlag |= 8;
        else m_nFlag &= ~8;
    }
    BOOL isOTZFlag() const { return m_nFlag & 0x10; }
    void setOTZFlag(BOOL bOn) {
        if (bOn) m_nFlag |= 0x10;
        else m_nFlag &= ~0x10;
    }
    void updateOTZ(FIX4 nZ) { m_iOTZ = nZ; }
    /* MATCH: the weak copy loads the flag byte into one register and shifts it into another (dsrl: 64-bit) */
    u_long isUVScroll() const {
        u_long b = 0;
        if (m_TexUV) {
            u_long nFlag = *(const UBYTE*)&m_nFlag;
            b = nFlag >> 7;
        }
        return b;
    }
    void setUVScroll(BOOL bOn) {
        if (bOn) m_nFlag |= 0x80;
        else m_nFlag &= ~0x80;
    }
    void updateUVScroll(FLOAT fR);
    void setupUVScroll(INT n);
    void setUVScrollOffset(INT nNum, FLOAT fU, FLOAT fV);
    BOOL isWBBox() const { return (USHORT)m_nFlag & 0x200; }
    void setWBBox(BOOL bOn) {
        if (bOn) m_nFlag |= 0x200;
        else m_nFlag &= ~0x200;
    }
    BOOL isSemiSortFlag() const { return (USHORT)m_nFlag & 0x400; }
    void setSemiSortFlag(BOOL bOn) {
        if (bOn) m_nFlag |= 0x400;
        else m_nFlag &= ~0x400;
    }
public:
    static void resetWorldScreenMatrix();
    static void resetMicroProgram();
    static void resetSendCtrl(INT nMode);
    static void setSendCtrl(INT nMode);
};

#ifdef GAME_ENTITY_H /* MATCH: only for includers that have CEntity (see below) */
/* PARTIAL: getModel(INT) is not in the original CEntity (callers use it as getModel()[i]); kept out of Game/Entity,
 * whose key function would emit it. */
#ifndef GAME_ENTITY_CPP /* MATCH: GAME_ENTITY_CPP gate, only for its own unit (tested 2026-11-02) */
inline CModel* CEntity::getModel(INT inIndex) { return &m_pModels[inIndex]; }
#endif
#endif

class CModelBG : public CModel {
public:
    CModelBG(USHORT nInDepth = 0) : CModel(NULL) { // MATCH: default argument for Game/Map's new CModelBG[n]
        setLayer(0x12, nInDepth);
        setTestFlag(0x300FD);
        setClampRepeat(TRUE);
        setBBoxFlag(1);
    }
    virtual void disp() {
        if (dispCheck()) CModelMan::add2DispList(*this, 0);
    }
private:
    virtual void makePkt();
    void setLayerFG(BYTE nInDepth) { AssertApp(__FILE__, 675, "0", 0); }
};

class CBBox : public CGPrim {
public: // as in the ref (struct)
    CCoord* m_pCrd;  /* 0x1c */
    G_RGBA m_C0;     /* 0x20 */
    BBOX m_bbox;     /* 0x24 */

public:
    CBBox() { m_C0.set(0xFF, 0, 0, 0x80); }
    virtual UINT getPacketSize() const { return 0x10; }
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

struct CShader {
    virtual ~CShader();
    virtual void setExtendedWork();
};

class CShadowModel : public CFadeModel { 
    INT m_nSize; /* 0x28 */
public:
    CShadowModel() {
        setTestFlag(0x70000);
        m_LayerInfo._nID = 0x17;
        CDrawable::setSemiTrans(ALPHA_OFF);
        m_LayerInfo._nDepth = 0;
        m_SemiDraw = FALSE;
        m_nSize = 0;
    }
    void setSize(INT nSize);
    INT getSize() { return m_nSize; }
    virtual void changeLayerExt(BOOL bOn, INT nDepth) { setLayer(0x17, 0); }
    virtual FIX4 calcZ() { return 0; }
};

/* Placeholders for the original header's inline functions: their strings ("System/Graphic/Model.h",
 * "m_pData != __null", "System/Graphic/Model.h(569)", "0", and CMCShader's "create CMCShader\n" /
 * "delete CMCShader\n") are in .rodata of every includer, in this order. Only line 569 is known. */
#line 1 MODEL_H_PATH
inline void _modelCheckData(void* inData) {
    if (!(inData != __null)) AssertApp(__FILE__, __LINE__, "m_pData != __null", 0);
}
inline void* _modelCreate() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    return com_malloc_d(4, __FILE__ "(569)", (UINT)-1);
}
inline void _modelAssertZero() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    AssertApp(__FILE__, __LINE__, "0", 0);
}
inline void _mcShaderCreate() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("create CMCShader\n");
}
inline void _mcShaderDelete() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("delete CMCShader\n");
}

#endif /* SYSTEM_GRAPHIC_MODEL_H */
