
#include "khtypes.h"
#define MODELDATA_H_TEXINFO /* MATCH: TexInfoStub (the original has its assert strings) */
/* MATCH: this unit's original names the headers with their full path (c:/KHCOM/MainApp/...) */
#define GPRIM_H_PATH "src/System/Graphic/Gprim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define MODELDATA_H_PATH "src/System/Graphic/modeldata.h" /* MATCH: this unit names modeldata.h so (header default "System/Graphic/modeldata.h") */
#define MODEL_H_PATH "src/System/Graphic/Model.h" /* MATCH: this unit names Model.h so (header default "System/Graphic/Model.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/Task.h"
#include "System/Coord.h"
#include "System/Math3D.h"
#include "System/Graphic/WorldEnv.h"
#include "System/Graphic/TexAnm.h"
#include "System/Graphic/TexUtil.h"
#include "System/Graphic/ShadowModel.h"
#include "System/Graphic/LightUnit.h"
#include "System/Graphic/ModelMan.h"
#include "include_asm.h"

class CRsrcData;
/* PARTIAL: stand-in for System/RsrcDB NAME_CASE: strings: RsrcData.h brings "System/RsrcData.h(156)", not in the original (queue: RsrcDB partition) */

#include "System/RsrcDB.h"







#include "System/Graphic/PackMan.h"






















FLOAT RotTransPers(IVECTOR& v0, const FMATRIX& m, const FVECTOR& v1);
extern const FMATRIX ONE_MATRIX;
extern const FVECTOR ONE_VECTOR;
INT checkBBox(const FMATRIX& mWorldMatrix, const BBOX* bbox);
SHADERDESC* searchShaderDesc(USHORT mode);
void getApplyMatrixBBox(FVECTOR& vOutMax, FVECTOR& vOutMin, const FMATRIX& Mat, const FVECTOR& vInMax, const FVECTOR& vInMin); /* static in the original (parked, asm glabel) */
namespace VRAM { /* PARTIAL: stand-in for System/VramMan VRAM: owner not written */
extern INT _TEX_TMP1_START;
extern INT _CLUT_TMP1_START;
}
inline void* operator new[](size_t, void* p) throw() { return p; }
extern "C" int strncmp(const char*, const char*, unsigned int);
class CShadowMapModel : public CFadeModel { /* PARTIAL: stand-in for CShadowMapModel: owner not written */
    HANDLE m_hHnd; /* 0x28 */
public:
    CShadowMapModel() {
        CDrawable::setSemiTrans(ALPHA_BLEND);
        setTestFlag(0x5000D);
        setLayer(0x18, 0);
        m_SemiDraw = FALSE;
        m_hHnd = 0;
    }
    virtual void changeLayerExt(BOOL bOn, INT nDepth) { setLayer(0x18, 0); }
    virtual FIX4 calcZ() { return 0; }
};
namespace Mad {
struct MODEL_ATTACH_DATA {
    char szBoneName[16]; /* 0x00 */
    UINT uiID;           /* 0x10 */
};
struct MAD_HEADER {
    char szCode[4];  /* 0x0 */
    UINT uiVersion;  /* 0x4 */
    UINT uiNum;      /* 0x8 */
    UINT uiOffset;   /* 0xc */
};
INT getModelAttachData(const MODEL_ATTACH_DATA** outMad, INT iRId);
}
inline void* operator new(size_t, void* p) throw() { return p; }
#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}
union FIVECTOR { /* PARTIAL: stand-in for FIVECTOR: owner not written */
    FVECTOR f;
    IVECTOR i;
};
extern "C" int strcmp(const char*, const char*);
extern "C" int strncasecmp(const char*, const char*, unsigned int);
struct TexInfo;

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

extern "C" u_long128 vu1mpg_start[];
extern "C" u_long128 vu1mpg2_start[];
static u_long128* g_mpg0 = vu1mpg_start;
static u_long128* g_mpg2 = vu1mpg2_start;
FMATRIX* CModel::m_pWSMatCache = NULL;
u_long128* CModel::m_pMpg = NULL;
INT CModel::m_iSendFlg = 0;

class CShader;

CShader* createDummyShader();
/* MATCH: the MPG_START initializer reads a symbol at address 0 (lui/addiu 0): an undefined weak symbol (as 2DSprObj) */
__asm__(".weak vu1mpg_zero\nvu1mpg_zero = 0\n");
extern u_long128 vu1mpg_zero;
static u_int MPG_START = (u_int)&vu1mpg_zero >> 3;
static SHADERDESC g_ShaderDescTbl[11] = {
    { 5, 0xFFFF, g_mpg0, 4, 0x86, createDummyShader },
    { 0x24, 0xFFFF, g_mpg0, 4, 0x86, createDummyShader },
    { 0, 0xFDFF, g_mpg0, 4, 0x86, createDummyShader },
    { 0x406E, 0xFF7F, g_mpg2, 0x11, 0x86, createDummyShader },
    { 6, 0xFFFF, g_mpg0, 4, 0, createDummyShader },
    { 0x406, 0xFFFF, g_mpg0, 4, 0x86, createDummyShader },
    { 0x400B, 0xFFFF, g_mpg0, 4, 0, createDummyShader },
    { 0x10, 0xFFFF, g_mpg0, 4, 0, createDummyShader },
    { 0x4005, 0xFDFF, g_mpg2, 7, 0, createDummyShader },
    { 2, 0xFFFF, g_mpg2, 7, 0, createDummyShader },
    { 0xFFFF, 0xFFFF },
};
CShader* createDummyShader() { return NULL; }
/* MATCH: the colour scale (1, 1, 1, 1) is built in a GPR (lui 1.0f, two pextlw) and stored with one sq */
static inline u_long128 _oneVector() {
    u_long128 ret;
    __asm__ volatile("lui %0, 0x3f80\n"
                     "pextlw %0, %0, %0\n"
                     "pextlw %0, %0, %0\n"
                     : "=r"(ret));
    return ret;
}
CModel::CModel(const MODELDATA* pData) {
    m_pData = pData;
    _ZeroInitializer = 0;
    m_iFlag = 0;
    _ZeroInitializer2 = 0;
    m_nZ = 0;
    m_pCrds = NULL;
    m_TexUV = NULL;
    m_pTexAnm = NULL;
    m_pTask = NULL;
    m_pShadowMapModel = NULL;
    *(u_long128*)&getColorScale() = _oneVector();
    setTestFlag(0x500FD);
    setLayer(0x1A, 0);
    m_pWSMat = &CWorldEnv::getWorldScreenMatrix();
    CModelMan::incModelCount();
}
CModel::~CModel() {
    releaseData();
    CModelMan::decModelCount();
}
void CModel::resetWorldScreenMatrix() { m_pWSMatCache = NULL; }
void CModel::resetMicroProgram() { m_pMpg = NULL; }
void CModel::resetSendCtrl(INT nMode) { m_iSendFlg &= ~nMode; }
void CModel::setSendCtrl(INT nMode) { m_iSendFlg |= nMode; }
u_long128* CModel::getMicroProg() {
    if (m_pShader) return *(u_long128**)((BYTE*)m_pShader + 4);
    return NULL;
}
/* Parked: 12 lines off, registers only: the original keeps the masked mode in a1, the table pointer in a2, 0xffff in a3 and the TRACE string's %hi in a0; ours mode a0, pointer a1, 0xffff a2, string a3. Tried: for/while/break loop shapes, a UINT copy of mode; permuter 12 min (24539 candidates, stays 12). */
#if 0
SHADERDESC* searchShaderDesc(USHORT mode) {
    SHADERDESC* ret = g_ShaderDescTbl;
    while (TRUE) {
        if (ret->m_Mode == 0xFFFF) {
            TRACE("\203V\203F\201[\203_\203f\203B\203X\203N\203\212\203v\203^\202\252\214\251\202\302\202\251\202\350\202\334\202\271\202\361\202\305\202\265\202\275(%04x).\n");
            return NULL;
        }
        if (ret->m_Mode == (mode & ret->m_Mask)) return ret;
        ret++;
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", searchShaderDesc__FUs);
#endif
/* MATCH: stripped placeholder (.sdata): the parked searchShaderDesc's string at its .rodata position */
__asm__(".section .sdata\n");
void __attribute__((section(".sdata"))) _searchShaderDesc_rodata() { TRACE("\203V\203F\201[\203_\203f\203B\203X\203N\203\212\203v\203^\202\252\214\251\202\302\202\251\202\350\202\334\202\271\202\361\202\305\202\265\202\275(%04x).\n", 0); }
void CModel::releaseData() {
    if (m_pSemiModel && isManagedSemiModel()) {
        delete m_pSemiModel;
        m_pSemiModel = NULL;
    }
    if (m_pFadeModel) {
        delete m_pFadeModel;
        m_pFadeModel = NULL;
    }
    if (m_pShadowMapModel) {
        delete m_pShadowMapModel;
        m_pShadowMapModel = NULL;
    }
    if (m_TexUV) delete m_TexUV;
    m_TexUV = NULL;
    if (m_pTexAnm) delete m_pTexAnm;
    m_pTexAnm = NULL;
}
INT CModel::link(CTask* p, const CHAR* pName) {
    if (!(p)) AssertApp(__FILE__, 451, "p", 0);
    CCoords* pCrds = p->getCoords();
    if (!pCrds) {
        p->createCoords();
        pCrds = p->getCoords();
    }
    m_pTask = p;
    return link(*pCrds, pName);
}
INT CModel::link(CCoords& crds, const CHAR* pName) {
    MODELDATA* m = CRsrcDB::getRsrcAMF(pName, NAME_CASE_SENSITIVE);
    if (!m) {
        TRACE("\203\202\203f\203\213\203f\201[\203^%s\202\252\202\240\202\350\202\334\202\271\202\361\n", pName);
        AssertApp(__FILE__, 469, "m", 0);
    }
    m_pData = m;
    return link(crds);
}
INT MODELDATA::getBoneIndex(const CHAR* pName) const {
    if (pName) {
        for (INT i = 0; i < m_nBoneNum; i++) {
            if (!strncasecmp(pName, (CHAR*)m_pBoneLabel + i * 0x14, 16)) return i;
        }
    }
    return -1;
}
INT CModel::getBoneIndex(const CHAR* pName) const {
    INT n = -1;
    if (m_pData) n = m_pData->getBoneIndex(pName);
    return n;
}
MODELDATA* AMFfromMDL(const CHAR* pszName, INT nIdx) {
    MODELDATA** ret = CRsrcDB::getRsrcMDL(pszName, NAME_CASE_SENSITIVE);
    if (!(ret)) AssertApp(__FILE__, 500, "ret", 0);
    return ret[nIdx];
}
MODELDATA* AMFfromMDL(INT nGroup, INT nIdx) {
    MODELDATA** ret = CRsrcDB::getRsrcMDL(nGroup);
    if (!(ret)) AssertApp(__FILE__, 508, "ret", 0);
    return ret[nIdx];
}
INT CModel::assign(CTask* p, const CHAR* pName, INT n) {
    if (!(p)) AssertApp(__FILE__, 516, "p", 0);
    CCoords* pCrds = p->getCoords();
    if (!(pCrds)) AssertApp(__FILE__, 518, "pCrds", 0);
    m_pTask = p;
    return assign(*pCrds, pName, n);
}
INT CModel::assign(CCoords& crds, const CHAR* pName, INT n) {
    MODELDATA* d = CRsrcDB::getRsrcAMF(pName, NAME_CASE_SENSITIVE);
    if (!d) {
        TRACE("\203\202\203f\203\213\203f\201[\203^%s\202\252\202\240\202\350\202\334\202\271\202\361\n", pName);
        AssertApp(__FILE__, 530, "d", 0);
    }
    m_pData = d;
    return assign(crds, n);
}
INT CModel::assign(CTask* p, INT n) {
    if (!(p)) AssertApp(__FILE__, 537, "p", 0);
    CCoords* pCrds = p->getCoords();
    if (!(pCrds)) AssertApp(__FILE__, 539, "pCrds", 0);
    m_pTask = p;
    return assign(*pCrds, n);
}
INT CModel::assign(CCoords& crds, INT crdidx) {
    INT i;
    if (!(m_pData)) AssertApp(__FILE__, 546, "m_pData", 0);
    m_pCrds = &crds;
    m_BoneStart = crdidx;
    m_BoneSkip = 0;
    m_BoneIdx = 0;
    m_TransTexCount = m_pData->m_nTexNum;
    if (!(!(m_TransTexCount > 16))) AssertApp(__FILE__, 579, "! (m_TransTexCount > TEXMAX)", 0);
    for (i = 0; i < m_TransTexCount; i++) {
        *(u_long*)&m_TexInfo[i] = *(u_long*)((BYTE*)m_pData->m_pTexParam + i * 0x20 + 0x10);
    }
    setupUVScroll(0);
    linksub();
    return 1;
}
void CModel::setModelData(const MODELDATA* pData) {
    releaseData();
    m_pData = pData;
    link(m_pTask);
}
INT CModel::link(CTask* p) {
    if (!(p)) AssertApp(__FILE__, 620, "p", 0);
    CCoords* pCrds = p->getCoords();
    if (!pCrds) {
        p->createCoords();
        pCrds = p->getCoords();
    }
    m_pTask = p;
    return link(*pCrds);
}
/* Parked: 25 lines off, registers only: the original gives the loop counter i s2 and nIdx s1 (ours i s1); the extra loop-head nop differs too. Worked: one getCoordNum() load in a local for the byte store and the compare, BONELABEL[i].m_ParentIndex, four Crd(i) inlines (four strength-reduced i*0xd0 pointers). Tried: UINT i, SHORT nIdx, nIdx declaration scope, m_pParent store; permuter 10 min (29465 candidates, stays 25). */
#if 0
INT CModel::link(CCoords& crds) {
    BOOL add;
    INT nBoneNum;
    INT i;
    if (!(m_pData)) AssertApp(__FILE__, 633, "m_pData", 0);
    m_pCrds = &crds;
    INT nCrd = crds.getCoordNum();
    m_BoneSkip = nCrd;
    nBoneNum = m_pData->m_nBoneNum;
    add = FALSE;
    if (nCrd < nBoneNum) {
        crds.addCoord(nBoneNum, NULL);
        add = TRUE;
    }
    if (add) {
        for (i = 0; i < nBoneNum; i++) {
            INT nIdx;
            crds.Crd(i).m_Local = ((const FMATRIX*)m_pData->m_pBones)[i];
            nIdx = ((const BONELABEL*)m_pData->m_pBoneLabel)[i].m_ParentIndex;
            if (nIdx >= 0) crds.Crd(i).setParent(&crds.Crd(nIdx));
            crds.Crd(i).recalcWorldMatrix();
            crds.Crd(i).m_Flag = 1;
        }
    }
    if (m_pData->m_BoneIndex == 0) {
        m_BoneStart = 0;
        m_BoneIdx = -1;
    } else {
        m_BoneIdx = m_BoneStart = m_pData->m_BoneIndex - 1;
    }
    m_TransTexCount = m_pData->m_nTexNum;
    if (!(!(m_TransTexCount > 16))) AssertApp(__FILE__, 707, "! (m_TransTexCount > TEXMAX)", 0);
    for (i = 0; i < m_TransTexCount; i++) {
        *(u_long*)&m_TexInfo[i] = *(u_long*)((BYTE*)m_pData->m_pTexParam + i * 0x20 + 0x10);
    }
    setupUVScroll(0);
    linksub();
    return 1;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", link__6CModelR7CCoords);
#endif
CCoord& CModel::getCrd(INT idx) {
    if (m_BoneIdx != -1) idx = 0;
    return m_pCrds->Crd(m_BoneStart + idx);
}
INT CModel::getCoordNum() const {
    if (!(m_pCrds)) AssertApp(__FILE__, 737, "m_pCrds", 0);
    return *(INT*)((BYTE*)m_pCrds + 4);
}
void CModel::setLocal(INT index) {
    if (!(m_pCrds != __null)) AssertApp(__FILE__, 743, "m_pCrds != __null", 0);
    if (!(0 <= index && index < m_pCrds->getCoordNum())) AssertApp(__FILE__, 744, "0 <= index && index < m_pCrds->getCoordNum()", 0);
    if (!(m_pData != __null)) AssertApp(__FILE__, 745, "m_pData != __null", 0);
    m_pCrds->Crd(index).m_Local = ((const FMATRIX*)m_pData->m_pBones)[index]; /* PARTIAL: CCoord::setLocal is Camera's compare+touch form */
}
INT CModel::getTextureIndex(const CHAR* pName) const {
    INT n = -1;
    if (pName) {
        for (INT i = 0; i < m_pData->m_nTexNum; i++) {
            if (!strcmp(pName, (CHAR*)m_pData->m_pTexParam + i * 0x20)) {
                n = i;
                break;
            }
        }
    }
    return n;
}
void CModel::changeTexture(const CHAR* inSrc, const CHAR* inDst) {
    INT nIdx = getTextureIndex(inSrc);
    if (nIdx >= 0) {
        TexInfo* pTex;
        if (inDst) pTex = CRsrcDB::getRsrcTex(inDst, NAME_CASE_SENSITIVE);
        else pTex = CRsrcDB::getRsrcTex((CHAR*)m_pData->m_pTexParam + nIdx * 0x20, NAME_CASE_SENSITIVE);
        changeTextureSub(nIdx, pTex);
    }
    setupUVScroll(0);
}
/* Parked: 4 lines off, scheduling: the original keeps the m_pData load after both zero stores of nTmpTex/nTmpClut, ours hoists it between them. addTexture (same loop) matches. Tried: store orders, chained assignment, i init outside the for, condition order. */
#if 0
void CModel::changeTextureSub(INT idx, const TexInfo* m) {
    SHORT nTmpTex;
    SHORT nTmpClut;
    INT i;
    if (idx >= 0 && idx < m_pData->m_nTexNum) {
        ((TexInfoStub*)&m_TexInfo[idx])->init(m, -1, -1);
        nTmpClut = 0;
        nTmpTex = 0;
        for (i = 0; i < m_pData->m_nTexNum; i++) {
            TexInfoStub* t = (TexInfoStub*)&m_TexInfo[i];
            t->update(nTmpTex, nTmpClut);
            if (!t->isFix()) CTexUtil::sumupTmpTexSize(*t->_pTexInfo, nTmpTex, nTmpClut);
        }
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", changeTextureSub__6CModeliPC7TexInfo);
#endif
void CModel::setShadowSize(INT nSize) {
    if (m_Mode == 0x10 && m_pFadeModel) *(INT*)((BYTE*)m_pFadeModel + 0x28) = nSize;
}
FIX4 CModel::getSemiZ() {
    FIX4 nZ;
    FIX4 nRetZ;
    FIVECTOR vScr;
    nRetZ = getZ();
    if (m_pFadeModel) {
        nZ = m_pFadeModel->getZ();
        if (nRetZ < nZ) nRetZ = nZ;
    }
    if (m_pSemiModel) {
        if (nRetZ < nZ) nRetZ = nZ;
    }
    if (nRetZ == 0) {
        getWorldBBoxScr(vScr.i, &CWorldEnv::getWorldScreenMatrix());
        nRetZ = vScr.i.z;
        if (nRetZ < 0) nRetZ = vScr.f.w <= 0.0f ? 0x0FFFF280 : 0x10;
    }
    return nRetZ;
}
FIX4 CModel::getOldSemiZ() {
    FIX4 nZ;
    FIX4 nRetZ;
    FIVECTOR vScr;
    nRetZ = getZ();
    if (m_pFadeModel) {
        nZ = m_pFadeModel->getOldZ();
        if (nRetZ < nZ) nRetZ = nZ;
    }
    if (m_pSemiModel) {
        if (nRetZ < nZ) nRetZ = nZ;
    }
    if (nRetZ == 0) {
        getWorldBBoxScr(vScr.i, &CWorldEnv::getWorldScreenMatrix());
        nRetZ = vScr.i.z;
        if (nRetZ < 0) nRetZ = vScr.f.w <= 0.0f ? 0x0FFFF280 : 0x10;
    }
    return nRetZ;
}
void CModel::getWorldBBoxScr(IVECTOR& vScr, const FMATRIX* pMWS) {
    FVECTOR vMax;
    FVECTOR vMin;
    if (pMWS) {
        asm volatile("lqc2 $vf28, 0x0(%0)\n"
                     "lqc2 $vf29, 0x10(%0)\n"
                     "lqc2 $vf30, 0x20(%0)\n"
                     "lqc2 $vf31, 0x30(%0)" : : "r"(pMWS));
    }
    if (!(m_nFlag & 0x2000)) {
        getWorldBBox(vMax, vMin);
        vMin.w = 0.5f;
        asm volatile("lqc2 $vf8, 0(%0)\n\tlqc2 $vf9, 0(%1)" : : "r"(&vMax), "r"(&vMin));
        asm volatile("vadd.xyz $vf8, $vf8, $vf9");
        asm volatile("vmulw.xyz $vf8, $vf8, $vf9w");
    } else {
        FVECTOR& vWorld = getCrd(m_BoneStart).getWorldMatrix().v[3];
        asm volatile("lqc2 $vf8, 0(%0)" : : "r"(&vWorld));
    }
    asm volatile("vmulax.xyzw $ACC, $vf28, $vf8x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf8y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf8z\n"
                 "vmaddw.xyzw $vf8, $vf31, $vf0w\n"
                 "vdiv $Q, $vf0w, $vf8w\n"
                 "vmax.w $vf8, $vf0, $vf0\n"
                 "vwaitq\n"
                 "vmulq.xyzw $vf8, $vf8, $Q\n"
                 "vftoi4.xyz $vf8, $vf8");
    asm volatile("sqc2 $vf8, 0(%0)" : : "r"(&vScr) : "memory");
}
/* Parked: 3 lines off: the original stores vMN[1].z/.y through the &vMN[1] operand register (t1), ours through sp. Worked: vMN[i].set(MIN/MAX, ...) (default w = 1.0f), one asm statement with all operands, D_ labels for the +-1.7e38 constants. Tried: field stores, chained assignment, pointer local; permuter 10 min (32256 candidates, stays 3). */
#if 0
static void getApplyMatrixBBox(FVECTOR& vOutMax, FVECTOR& vOutMin, const FMATRIX& Mat, const FVECTOR& vInMax, const FVECTOR& vInMin) {
    FVECTOR vMN[2];
    vMN[0].set(-1.70141183e+38f, -1.70141183e+38f, -1.70141183e+38f, 1.0f);
    vMN[1].set(1.70141183e+38f, 1.70141183e+38f, 1.70141183e+38f, 1.0f);
    asm volatile("lqc2 $vf4, 0x0(%0)\n"
                 "lqc2 $vf5, 0x10(%0)\n"
                 "lqc2 $vf6, 0x20(%0)\n"
                 "lqc2 $vf7, 0x30(%0)\n"
                 "lqc2 $vf8, 0x0(%1)\n"
                 "lqc2 $vf9, 0x0(%2)\n"
                 "lqc2 $vf10, 0x0(%3)\n"
                 "lqc2 $vf16, 0x0(%4)\n"
                 "vadd.yz $vf11, $vf10, $vf0\n"
                 "vadd.y $vf12, $vf10, $vf0\n"
                 "vadd.xy $vf13, $vf10, $vf0\n"
                 "vadd.xz $vf14, $vf10, $vf0\n"
                 "vadd.z $vf15, $vf10, $vf0\n"
                 "vadd.x $vf17, $vf10, $vf0\n"
                 "vmulw.xw $vf11, $vf16, $vf0w\n"
                 "vmulw.xzw $vf12, $vf16, $vf0w\n"
                 "vmulw.zw $vf13, $vf16, $vf0w\n"
                 "vmulw.yw $vf14, $vf16, $vf0w\n"
                 "vmulw.xyw $vf15, $vf16, $vf0w\n"
                 "vmulw.yzw $vf17, $vf16, $vf0w\n"
                 "vmulax.xyzw $ACC, $vf4, $vf10x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf10y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf10z\n"
                 "vmaddw.xyzw $vf10, $vf7, $vf0w\n"
                 "vmulax.xyzw $ACC, $vf4, $vf11x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf11y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf11z\n"
                 "vmaddw.xyzw $vf11, $vf7, $vf0w\n"
                 "vmax.xyz $vf8, $vf8, $vf10\n"
                 "vmini.xyz $vf9, $vf9, $vf10\n"
                 "vmulax.xyzw $ACC, $vf4, $vf12x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf12y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf12z\n"
                 "vmaddw.xyzw $vf12, $vf7, $vf0w\n"
                 "vmax.xyz $vf8, $vf8, $vf11\n"
                 "vmini.xyz $vf9, $vf9, $vf11\n"
                 "vmulax.xyzw $ACC, $vf4, $vf13x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf13y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf13z\n"
                 "vmaddw.xyzw $vf13, $vf7, $vf0w\n"
                 "vmax.xyz $vf8, $vf8, $vf12\n"
                 "vmini.xyz $vf9, $vf9, $vf12\n"
                 "vmulax.xyzw $ACC, $vf4, $vf14x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf14y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf14z\n"
                 "vmaddw.xyzw $vf14, $vf7, $vf0w\n"
                 "vmax.xyz $vf8, $vf8, $vf13\n"
                 "vmini.xyz $vf9, $vf9, $vf13\n"
                 "vmulax.xyzw $ACC, $vf4, $vf15x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf15y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf15z\n"
                 "vmaddw.xyzw $vf15, $vf7, $vf0w\n"
                 "vmax.xyz $vf8, $vf8, $vf14\n"
                 "vmini.xyz $vf9, $vf9, $vf14\n"
                 "vmulax.xyzw $ACC, $vf4, $vf16x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf16y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf16z\n"
                 "vmaddw.xyzw $vf16, $vf7, $vf0w\n"
                 "vmax.xyz $vf8, $vf8, $vf15\n"
                 "vmini.xyz $vf9, $vf9, $vf15\n"
                 "vmulax.xyzw $ACC, $vf4, $vf17x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf17y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf17z\n"
                 "vmaddw.xyzw $vf17, $vf7, $vf0w\n"
                 "vmax.xyz $vf8, $vf8, $vf16\n"
                 "vmini.xyz $vf9, $vf9, $vf16\n"
                 "vmax.xyz $vf8, $vf8, $vf17\n"
                 "vmini.xyz $vf9, $vf9, $vf17\n"
                 "sqc2 $vf8, 0x0(%5)\n"
                 "sqc2 $vf9, 0x0(%6)"
                 : : "r"(&Mat), "r"(&vMN[0]), "r"(&vMN[1]), "r"(&vInMax), "r"(&vInMin), "r"(&vOutMax), "r"(&vOutMin) : "memory");
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getApplyMatrixBBox__FR7FVECTORT0RC7FMATRIXRC7FVECTORT3);
#endif
/* MATCH: stripped placeholder (.sdata): the parked getApplyMatrixBBox's float constants at their .rodata position */
__asm__(".section .sdata\n");
BOOL __attribute__((section(".sdata"))) _getApplyMatrixBBox_rodata(FLOAT a, FLOAT b) { return a < -1.70141183e+38f && b > 1.70141183e+38f; }
/* Parked: 38 lines off: the original keeps &pCrd->m_World as the strength-reduced loop pointer (s3 += 0xd0, counter counted down), &vMax/&vMin in s8/s7 for the init stores and the VU0 block, and loads the final bbox into f4/f5/f0..f3 in max-then-min order. Tried: pCrd[i] indexing (keeps the counter, 41 lines), _x/_y/_z temporaries, set helpers. */
#if 0
void CModel::calcWorldBBox(BOOL bCalc) {
    INT i;
    FVECTOR v[2];
    FVECTOR vMax;
    FVECTOR vMin;
    CCoord* pCrd;
    INT iBoneNum;
    if ((m_nFlag & 0x200) && !bCalc) return;
    if (!(m_pData)) AssertApp(__FILE__, 1067, "m_pData", 0);
    pCrd = &getCrd(0);
    iBoneNum = m_pData->m_nBoneNum;
    vMax.set(-1.70141183e+38f, -1.70141183e+38f, -1.70141183e+38f, 1.0f);
    vMin.set(1.70141183e+38f, 1.70141183e+38f, 1.70141183e+38f, 1.0f);
    for (i = 0; i < iBoneNum; i++, pCrd++) {
        BBOX* bb = getBBox(i);
        if (bb->x[0] <= bb->x[1]) {
            v[0].set(bb->x[1], bb->y[1], bb->z[1], 1.0f);
            v[1].set(bb->x[0], bb->y[0], bb->z[0], 1.0f);
            getApplyMatrixBBox(v[0], v[1], pCrd->getWorldMatrix(), v[0], v[1]);
            asm volatile("lqc2 $vf4, 0x0(%0)\n"
                         "lqc2 $vf5, 0x0(%1)\n"
                         "lqc2 $vf6, 0x0(%2)\n"
                         "lqc2 $vf7, 0x0(%3)\n"
                         "vmax.xyz $vf4, $vf4, $vf6\n"
                         "vmini.xyz $vf5, $vf5, $vf7\n"
                         "sqc2 $vf4, 0x0(%0)\n"
                         "sqc2 $vf5, 0x0(%1)" : : "r"(&vMax), "r"(&vMin), "r"(&v[0]), "r"(&v[1]) : "memory");
        }
    }
    if (m_Mode == 0x10) {
        FLOAT _y = vMin.y;
        vMin.y = _y - CShadowModelMan::getDownLen();
        vMax.y = _y + (vMax.y - _y) * CShadowModelMan::getUpPer();
    }
    m_nFlag |= 0x200;
    m_BBox.x[1] = vMax.x;
    m_BBox.y[1] = vMax.y;
    m_BBox.z[1] = vMax.z;
    m_BBox.x[0] = vMin.x;
    m_BBox.y[0] = vMin.y;
    m_BBox.z[0] = vMin.z;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", calcWorldBBox__6CModeli);
#endif
/* MATCH: stripped placeholder (.sdata): the parked calcWorldBBox's float constants at their .rodata position */
__asm__(".section .sdata\n");
BOOL __attribute__((section(".sdata"))) _calcWorldBBox_rodata(FLOAT a, FLOAT b) { return a < -1.70141183e+38f && b > 1.70141183e+38f; }
void CModel::getWorldBBox(FVECTOR& vMax, FVECTOR& vMin) {
    calcWorldBBox(0);
    vMax.x = m_BBox.x[1];
    vMax.y = m_BBox.y[1];
    vMax.z = m_BBox.z[1];
    vMin.x = m_BBox.x[0];
    vMin.y = m_BBox.y[0];
    vMin.z = m_BBox.z[0];
}
INT checkBSphere(CCoord& crd, const FVECTOR& ctr, FLOAT rad) {
    FVECTOR b; // MATCH: the copy of ctr2 has the lowest stack slot (an inline's temp in the original)
    FVECTOR ctr2;
    FVECTOR frustum[6];
    BOUNDING_SPHERE sphere;
    ApplyMatrix(ctr2, crd.getWorldMatrix(), ctr);
    b = ctr2;
    Math3D::CalcFrustum(frustum);
    sphere.vCenter = b;
    sphere.fR = rad;
    return Math3D::CheckVisibleBoundingSphere(sphere, frustum) ? 0 : 0x80000000;
}
INT checkBSphere(CCoord& crd, const FVECTOR& ctr, FLOAT rad);
/* Parked: 10 lines off, FP registers only: the original keeps the shared 0.5f in f5 (x sum f6, y[1] f7), ours 0.5f in f7. Worked: x[0] > x[1] compare, a copy tmp (declared first) of ctr as the call argument, sqrt after the copy. Tried: statement orders, w first/last, 0.5f local; permuter 12 min (37389 candidates, stays 10). */
#if 0
INT checkBSphere(CCoord& crd, const BBOX* bbox) {
    FVECTOR tmp;
    FVECTOR ctr;
    FLOAT r;
    FLOAT ret;
    if (bbox->x[0] > bbox->x[1]) return 0xC0000000;
    ctr.x = (bbox->x[0] + bbox->x[1]) * 0.5f;
    ctr.y = (bbox->y[0] + bbox->y[1]) * 0.5f;
    ctr.z = (bbox->z[0] + bbox->z[1]) * 0.5f;
    ctr.w = 1.0f;
    r = (bbox->x[0] - bbox->x[1]) * (bbox->x[0] - bbox->x[1]) + (bbox->y[0] - bbox->y[1]) * (bbox->y[0] - bbox->y[1])
        + (bbox->z[0] - bbox->z[1]) * (bbox->z[0] - bbox->z[1]);
    tmp = ctr;
    r = _sqrtf(r) * 0.5f;
    return checkBSphere(crd, tmp, r);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", checkBSphere__FR6CCoordPC4BBOX);
#endif
/* Parked: 12 lines off, registers only: the original keeps the scratchpad pointer in v1 and the clip result in a0 (or a0,v0,a0; move v0,a0), ours v0/v1. Worked: the 0xC0000000 return last (if (!(x[1] < x[0])) {...}), v assigned inside the if, asm "nop" opening the if (ret) block (the original's extra nop after the beqz slot; same tail as the parked resultCheckBBox(CCoord&)). Tried: one/two-output cfc2 asm, 'm' operand lqc2 helpers, result local, decl order; permuter 15 min (45593 candidates, stays 16). */
#if 0
INT checkBBox(const FMATRIX& mWorldMatrix, const BBOX* bbox) {
    INT ret;
    INT clip;
    FVECTOR* v;
    if (!(bbox->x[1] < bbox->x[0])) {
        v = (FVECTOR*)0x70000000;
    v[0].x = bbox->x[0];
    v[0].y = bbox->y[0];
    v[0].z = bbox->z[0];
    v[1].x = bbox->x[1];
    v[1].y = bbox->y[1];
    v[1].z = bbox->z[1];
    asm volatile("lqc2 $vf20, 0(%0)" : : "r"(v));
    asm volatile("lqc2 $vf27, 0x10(%0)" : : "r"(v));
    asm volatile("lqc2 $vf21, 0x0(%0)\n"
                 "lqc2 $vf22, 0x10(%0)\n"
                 "lqc2 $vf23, 0x20(%0)\n"
                 "lqc2 $vf24, 0x30(%0)" : : "r"(&mWorldMatrix));
    asm volatile("vcallms 0x20");
    asm volatile("cfc2.i %0, $vi4" : "=r"(ret));
    asm volatile("cfc2.ni %0, $vi5" : "=r"(clip));
        if (ret) {
            asm volatile("nop");
            ret = (ret | 0x8000) << 16;
        }
        clip |= ret;
        return clip;
    }
    return 0xC0000000;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", checkBBox__FRC7FMATRIXPC4BBOX);
#endif
void resultCheckBBox(FVECTOR& min, FVECTOR& max) {
    asm volatile("qmfc2.i $0, $vf0");
    asm volatile("sqc2 $vf2, 0(%0)" : : "r"(&min) : "memory");
    asm volatile("sqc2 $vf3, 0(%0)" : : "r"(&max) : "memory");
}
/* Parked: 5 lines off: the original leaves the bc1t delay slot empty (lui 0xc000 in the final jr slot), has an extra nop after the beqz slot and builds the result in v1 (move v0,v1); ours fills bc1t with the lui. Tried: compare forms (>, <=, !<), result locals (clip |= ret, res), if/else return; permuter 15 min (24217 candidates, 16 -> 13). */
#if 0
INT resultCheckBBox(CCoord& crd, const BBOX* bbox) {
    INT ret;
    INT clip;
    if (bbox->x[1] < bbox->x[0]) return 0xC0000000;
    asm volatile("cfc2.i %0, $vi4" : "=r"(ret));
    asm volatile("cfc2.ni %0, $vi5" : "=r"(clip));
    if (ret) ret = (ret | 0x8000) << 16;
    return ret | clip;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", resultCheckBBox__FR6CCoordPC4BBOX);
#endif
INT CModel::checkBBox() {
    INT ret;
    if (m_pCrds->getCoordNum() <= 0) return -1;
    if (!m_pData) return -1;
    if (getLayerID() == 0x1b) return 0;
    if (m_BoneIdx != -1) {
        ret = ::checkBBox(m_pCrds->Crd(m_BoneStart).getWorldMatrix(), &m_pData->m_pBBox[m_BoneIdx]);
    } else {
        calcWorldBBox(0);
        ret = ::checkBBox(ONE_MATRIX, &m_BBox);
        if (m_pTask) m_pTask->setLastClip(ret);
    }
    m_BBoxFlag = ret;
    return ret;
}
void CModel::setColorScale(const FVECTOR& vColor) { getColorScale() = vColor; }
void CModel::setColorScale(FLOAT r, FLOAT g, FLOAT b, FLOAT a) { getColorScale().set(r, g, b, a); }
void CModel::setAlphaScale(FLOAT inScale) { m_ColorScale.w = inScale; }
void CModel::linksub() {
    UINT rawMode;
    BOOL bHasSemiData;
    m_Mode = m_pData->getMode();
    m_pShader = searchShaderDesc(m_Mode);
    if (!(m_pShader)) AssertApp(__FILE__, 1738, "m_pShader", 0);
    rawMode = m_pData->getRawMode();
    if (rawMode & 0x20000000) m_nTestFlag = (INT)m_nTestFlag & -2;
    else m_nTestFlag = ((INT)m_nTestFlag & -0x4000) | 0xFD;
    m_ShaderOption = m_pData->getShaderOption();
    bHasSemiData = FALSE;
    if (m_pData->m_pSemiData) bHasSemiData = *(UINT*)m_pData->m_pSemiData != 0x60000000;
    if (bHasSemiData) m_nFlag |= 2;
    else m_nFlag &= ~2;
    if (bHasSemiData) {
        if (!m_pSemiModel) {
            m_pSemiModel = createSemiModel();
            if (m_pSemiModel) {
                m_nFlag |= 1;
                m_pSemiModel->link(this);
            }
        } else {
            m_pSemiModel->link(this);
        }
    }
    if (m_Mode == 0x10 && !m_pFadeModel) {
        m_pFadeModel = new (com_malloc_d(sizeof(CShadowModel), __FILE__ "(1781)", RET_ADDR())) CShadowModel;
        if (m_pFadeModel) {
            m_pFadeModel->link(this);
            m_pWSMat = &CShadowModelMan::getWorldScreenMatrix();
            getColorScale().set(1.0f, 1.0f, 1.0f, 1.0f);
        }
    }
    if (m_pData->isDataEmpty() && m_pData->isSemiDataEmpty()) m_nPacketSize = 0;
    else m_nPacketSize = m_pData->m_nPacketSize;
    setShadowMapModel();
}
void CModel::addTexture(const CHAR* texName, INT idx) {
    SHORT nTmpTex;
    SHORT nTmpClut;
    INT i;
    if (m_TransTexCount < m_pData->m_nTexNum + 1) {
        m_TransTexCount = m_pData->m_nTexNum + 1;
        if (!(!(m_TransTexCount > 16))) AssertApp(__FILE__, 1867, "! (m_TransTexCount > TEXMAX)", 0);
    }
    ((TexInfoStub*)&m_TexInfo[m_pData->m_nTexNum])->init(CRsrcDB::getRsrcTex(texName, NAME_CASE_SENSITIVE), -1, -1);
    nTmpTex = 0;
    nTmpClut = 0;
    for (i = 0; i < m_TransTexCount; i++) {
        TexInfoStub* t = (TexInfoStub*)&m_TexInfo[i];
        t->update(nTmpTex, nTmpClut);
        if (!t->isFix()) CTexUtil::sumupTmpTexSize(*t->_pTexInfo, nTmpTex, nTmpClut);
    }
    setupUVScroll(0);
}
void CModel::setShadowMapModel() {
    UINT rawMode;
    if (m_pShadowMapModel) return;
    switch (m_pData->getMode()) {
    case 0:
    case 5:
    case 0x200:
    case 0x406:
        break;
    default:
        return;
    }
    rawMode = m_pData->getRawMode();
    if (!(rawMode & 0x100000)) return;
    m_pShadowMapModel = new (com_malloc_d(sizeof(CShadowMapModel), __FILE__ "(1909)", RET_ADDR())) CShadowMapModel;
    if (m_pShadowMapModel) {
        m_pShadowMapModel->link(this);
        m_ColorScale.w = 1.0f;
    }
}
void CModel::attachSemiModel(CSemiModel& inModel, BOOL bInManage) {
    if (m_pSemiModel && isManagedSemiModel()) delete m_pSemiModel;
    m_pSemiModel = &inModel;
    if (bInManage) m_nFlag |= 1;
    else m_nFlag &= ~1;
}
BOOL CModel::sendTexture(u_long* tex0) {
    u_long* tex;
    BOOL bSend;
    INT nTexMax;
    INT i;
    tex = tex0;
    bSend = FALSE;
    nTexMax = m_TransTexCount;
    if (nTexMax > 0) {
        INT nTmpTex = VRAM::_TEX_TMP1_START;
        INT nTmpClut = VRAM::_CLUT_TMP1_START;
        INT nTransIndex = 0;
        for (i = 0; i < nTexMax; i++) {
            TexInfoStub* t = (TexInfoStub*)&m_TexInfo[i];
            BOOL bUpdate;
            BOOL bTexAnm;
            UINT nTexPos;
            UINT nClutPos;
            if (t->isFix()) {
                if (tex) {
                    *tex = t->getTPage();
                    tex++;
                }
                continue;
            }
            bUpdate = TRUE;
            if (!(m_iSendFlg & 0x200)) bUpdate = CModelMan::updateTexCache(nTransIndex++, *t);
            bTexAnm = FALSE;
            if (m_pTexAnm && m_pTexAnm->check(i)) {
                bUpdate = TRUE;
                bTexAnm = TRUE;
            }
            bSend |= bUpdate;
            if (t->isFixTex()) nTexPos = t->_pTexInfo->_TexInfo._nDistPos;
            else nTexPos = nTmpTex + t->_nTexPos;
            if (t->isFixClut()) nClutPos = t->_pTexInfo->_ClutInfo._nDistPos;
            else nClutPos = nTmpClut + t->_nClutPos;
            if (bUpdate) {
                CPackMan::pkRefLoadImage(nClutPos, t->_pTexInfo->_ClutInfo);
                CPackMan::pkRefLoadImage(nTexPos, t->_pTexInfo->_TexInfo);
                CModelMan::incTexSendCount(0);
                if (bTexAnm) m_pTexAnm->sendTexture(i, nTexPos, t->_pTexInfo->_TexInfo, nClutPos, t->_pTexInfo->_ClutInfo);
            }
            if (tex) {
                *tex = t->_pTexInfo->getTPage() | (u_long)nTexPos | ((u_long)nClutPos << 37);
                tex++;
            }
        }
    }
    return bSend;
}
INT CModel::getTexParam(u_long* tex0) {
    INT nTexMax;
    INT i;
    nTexMax = m_TransTexCount;
    if (nTexMax > 0) {
        UINT nTexPos;
        UINT nClutPos;
        INT nTmpTex = VRAM::_TEX_TMP1_START;
        INT nTmpClut = VRAM::_CLUT_TMP1_START;
        for (i = 0; i < nTexMax; i++) {
            TexInfoStub* t = (TexInfoStub*)&m_TexInfo[i];
            if (t->isFix()) {
                tex0[i] = t->getTPage();
            } else {
                if (t->isFixTex()) nTexPos = t->_pTexInfo->_TexInfo._nDistPos;
                else nTexPos = nTmpTex + t->_nTexPos;
                if (t->isFixClut()) nClutPos = t->_pTexInfo->_ClutInfo._nDistPos;
                else nClutPos = nTmpClut + t->_nClutPos;
                tex0[i] = t->_pTexInfo->getTPage() | (u_long)nTexPos | ((u_long)nClutPos << 37);
            }
        }
    }
    return nTexMax;
}
void CModel::dispUpdate(FLOAT fR) {
    setZ(0);
    if (m_pFadeModel) {
        m_pFadeModel->setOldZ(m_pFadeModel->getZ());
        m_pFadeModel->setZ(0);
    }
    if (m_pSemiModel) {
        m_pSemiModel->setOldZ(m_pSemiModel->getZ());
        m_pSemiModel->setZ(0);
    }
    updateUVScroll(fR);
}
BOOL CModel::dispCheck() {
    if (m_pCrds->getCoordNum() <= 0) return FALSE;
    if (m_ColorScale.w <= 0.0f) return FALSE;
    return !CPackMan::isOverFlow(getPacketSize());
}
void CModel::disp() { disp(m_pTask ? m_pTask->getFrameRate() * 0.020000001f : 1.0f); }
void CModel::disp(FLOAT fR) {
    BOOL bUpdate = TRUE;
    m_BBoxFlag = 0;
    m_nFlag &= ~0x200;
    dispUpdate(fR);
    if (checkBBox() < 0) bUpdate = FALSE;
    else if (!dispCheck()) bUpdate = FALSE;
    if (bUpdate) dispImpl();
    else m_nFlag &= ~0x18;
}
void CModel::dispImpl() {
    setDispFlag(FALSE);
    if (!isDispOff()) {
        if (!isDataEmpty() && m_ColorScale.w > 0.0f) {
            if (isDispWireframe() && MODELDATA::isCharacter(m_Mode)) {
                disp0(-1, getTestFlag(), 0);
                m_ShaderOption |= 0x80;
                disp0(-1, getTestFlag(), 0);
                m_ShaderOption &= ~0x80;
            } else if (m_ColorScale.w >= 1.0f && m_Mode != 0x10) {
                INT id = getLayerID() & 0xF;
                if (id >= 2 && (id < 4 || id == 0xB)) {
                    CModelMan::add2DispList(*this, 0);
                } else if (CPackMan::getEnableDraw() & 2) {
                    disp0(-1, getTestFlag(), 0);
                    if (m_pShadowMapModel && isShadowMap()) m_pShadowMapModel->disp();
                }
            } else {
                if (!m_pFadeModel) {
                    m_pFadeModel = new (com_malloc_d(sizeof(CFadeModel), __FILE__ "(2262)", RET_ADDR())) CFadeModel;
                    if (m_pFadeModel) {
                        INT id;
                        m_pFadeModel->link(this);
                        id = getLayerID() & 0xF;
                        if (id >= 2) {
                            if (id >= 4) {
                                if (id == 0xB) m_pFadeModel->changeLayerExt(TRUE, getLayerDepth() + 1);
                            } else {
                                m_pFadeModel->setLayer(getLayerID(), getLayerDepth());
                            }
                        }
                    }
                }
                if (m_pFadeModel) {
                    if (m_Mode != 0x10) {
                        if (isAutoEffSemiSort()) m_pFadeModel->setSemiSort(TRUE);
                        else m_pFadeModel->setSemiSort(isSemiSortFlag());
                    }
                    m_pFadeModel->disp();
                }
            }
            setDispFlag(TRUE);
        }
        if (isAvailableSemiModel()) {
            if (m_pSemiModel) {
                if (m_pFadeModel) {
                    if (m_ColorScale.w >= 1.0f) m_pSemiModel->setSemiSort(isSemiSortFlag());
                    else if (isAutoEffSemiSort()) m_pSemiModel->setSemiSort(TRUE);
                    else m_pSemiModel->setSemiSort(isSemiSortFlag());
                }
                m_pSemiModel->disp();
            }
            setDispFlag(TRUE);
        }
    }
    m_nFlag &= ~0x10;
}
void CModel::makePkt() {
    CPackMan::pkCloseDirectCode();
    if (!m_pData->isDataEmpty()) {
        if (getLayerID() == 0x1b) disp0(-1, CPackMan::getDefault_TEST_1(), 0);
        else disp0(-1, getTestFlag(), 0);
    }
    CPackMan::pkOpenDirectCode(0);
}
void CModel::disp0(INT semikind, u_long nInTest, INT iFlags) {
    const int (*pData)[4];
    UINT nMode;
    u_long tex0[16];
    if (!(m_pShader)) AssertApp(__FILE__, 2446, "m_pShader", 0);
    pData = (const int (*)[4])_disp0_GetData(nMode, iFlags);
    if (pData) {
        if (!(m_iSendFlg & 1)) disp0_SetMicroCode(m_pShader->m_pMpg);
        CPackMan::pkCnt(0);
        CPackMan::pkAddCode(0x13000000);
        if (!(m_iSendFlg & 2)) _disp0_SetTexture(iFlags, tex0);
        _disp0_SetPacket(nMode, pData, semikind, nInTest, iFlags, tex0);
    }
}
/* Parked: 2 lines off: the original loads *pData before the lui 0x6000 of the tag compare, ours lui first. Tried: d/mode locals, store forms, int (*)[4]/u_long/u_long128 pData, test forms (!=, ^, ternary, tag local); permuter 12 min (22337 candidates, stays 2). */
#if 0
u_long* CModel::_disp0_GetData(UINT& nMode, INT iFlags) {
    UINT* pData;
    UINT mode;
    if (iFlags & 1) {
        pData = (UINT*)m_pData->m_pSemiData;
        mode = ((USHORT*)pData)[0x1c / 2];
    } else {
        mode = m_Mode;
        pData = (UINT*)m_pData->m_pData;
    }
    nMode = mode;
    if (*pData == 0x60000000) return NULL;
    return (u_long*)pData;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _disp0_GetData__6CModelRUii);
#endif
void CModel::disp0_SetMicroCode(u_long128* pMProg) {
    if (!((m_pMpg == g_mpg0) || (m_pMpg == g_mpg2) || (m_pMpg == __null))) AssertApp(__FILE__, 2539, "(m_pMpg==g_mpg0)||(m_pMpg==g_mpg2)||(m_pMpg == __null)", 0);
    if (pMProg != m_pMpg || (m_iSendFlg & 0x100)) {
        m_pMpg = pMProg;
        CPackMan::pkCall(pMProg, 0);
        CPackMan::pkAddCode(0x10000000);
        CPackMan::pkAddCode(0);
    }
}
void CModel::_disp0_SetTexture(INT nFlag, u_long* pTag) {
    if (!(nFlag & 6)) sendTexture(pTag);
}
/* Parked: 137 of 659 lines off, same size; mostly registers: the original gives this s5 and iFlags&2 s4 (ours swapped), the bone/light min() selects use movn (an if-form gives that but swaps s0/s1 globally), the no-light branch materializes zero twice. Worked: case 2 with default, (USHORT) mode compares inline, pcpyld tied to the tag word, _ref(m_iSendFlg) after the GIF stores, cache-hit close in the else arm, x/y/z/w colour stores, pextlw pack with a non-early-clobber temp. Tried: iFlags &= 2, a BOOL copy, zero variants; permuter 20 min (19316 candidates, stays 145). */
#if 0
template <class T> static inline T& _ref(T& v) { return v; }
static inline u_long128 _packWord(INT x, INT y, INT z, INT w) {
    u_long128 tmp;
    u_long128 out128;
    __asm__ volatile("pextlw %0, %3, %2\n"
                     "pextlw %1, %5, %4\n"
                     "pextlw %0, %1, %0\n"
                     : "=&r"(out128), "=r"(tmp)
                     : "r"(x), "r"(z), "r"(y), "r"(w));
    return out128;
}
static inline ULONGLONG _lo64(u_long128 x) {
    ULONGLONG out64;
    __asm__ volatile("por %0, $0, %1" : "=r"(out64) : "r"(x));
    return out64;
}
static inline ULONGLONG _hi64(u_long128 x) {
    ULONGLONG out64;
    __asm__ volatile("pcpyud %0, %1, $0" : "=r"(out64) : "r"(x));
    return out64;
}
void CModel::_disp0_SetPacket(UINT nMode, const int (*pData)[4], INT semikind, u_long nInTest, INT iFlags, u_long* tex0) {
    QWdata* pQw;
    ULONGLONG lTest;
    ULONGLONG lAlpha;
    ULONGLONG lClamp;
    UINT version;
    u_long128 qColorScale;
    u_long128 qOBLM;
    u_long128 qPP;
    INT i;
    u_long tex0Buf[16];
    u_long128 tag;

    version = pData[1][3];
    CPackMan::pkCall((u_long128*)pData, 0);
    CPackMan::pkAddCode(0x10000000);
    if (m_pWSMatCache != m_pWSMat || (m_iSendFlg & 0x400)) {
        pQw = (QWdata*)CPackMan::pkOpenUpkCodeV4_32(0, 4, 4);
        pQw[0].ul128 = ((QWdata*)m_pWSMat)[0].ul128;
        pQw[1].ul128 = ((QWdata*)m_pWSMat)[1].ul128;
        pQw[2].ul128 = ((QWdata*)m_pWSMat)[2].ul128;
        pQw[3].ul128 = ((QWdata*)m_pWSMat)[3].ul128;
        CPackMan::pkCloseUpkCodeV4_32(pQw + 4);
        m_pWSMatCache = m_pWSMat;
    }
    if (semikind <= 0) {
        lAlpha = 0x80000000A8;
        lTest = nInTest;
    } else {
        UINT rawMode = getMode();
        if ((USHORT)rawMode == 0x4005 || (USHORT)rawMode == 0x4205) lTest = 0x53001;
        else lTest = nInTest;
        switch (semikind) {
        case 1:
            lAlpha = 0x44;
            break;
        case 3:
            lAlpha = 0x42;
            break;
        case 2:
        default:
            if ((USHORT)rawMode == 0x4005 || (USHORT)rawMode == 0x4205) lAlpha = 0x58;
            else lAlpha = 0x48;
            break;
        }
    }
    lClamp = 5;
    if (isClampRepeat() && !(iFlags & 2)) lClamp = 0;

    CPackMan::pkOpenDirectCode(0);
    __asm__ volatile("pcpyld %0, %1, %2" : "=r"(tag) : "r"((u_long)0xE), "0"((u_long)(m_iBeforeDirectQNum + 4) | 0x8000 | 0x1000000000000000));
    pQw = (QWdata*)CPackMan::pkOpenGifTagManual(tag);
    pQw[0].ul64[0] = lTest;
    pQw[0].ul64[1] = 0x47;
    pQw[1].ul64[0] = lAlpha;
    pQw[1].ul64[1] = 0x42;
    pQw[2].ul64[0] = lClamp;
    pQw[2].ul64[1] = 8;
    if (_ref(m_iSendFlg) & 4) {
        pQw[3].ul64[1] = 3;
    } else {
        pQw[3].ul64[0] = CPackMan::getDefault_ZBUF_1();
        pQw[3].ul64[1] = 0x4E;
    }
    pQw += 4;
    if (m_pBeforeDirect) {
        QWdata* pQwSrc = (QWdata*)m_pBeforeDirect;
        for (i = 0; i < m_iBeforeDirectQNum; i++) {
            pQw->ul128 = pQwSrc->ul128;
            pQw++;
            pQwSrc++;
        }
    }
    CPackMan::pkCloseGifTagManual((UINT*)pQw);
    CPackMan::pkCloseDirectCode();

    if (iFlags & 2) {
        pQw = (QWdata*)CPackMan::pkOpenUpkCodeV4_32(0x18, 4, 4);
        pQw[0].ul64[0] = CShadowModelMan::getTEX_0();
        pQw[0].ul64[1] = 6;
        *(FMATRIX*)&pQw[1] = CShadowModelMan::getWorldTextureMatrix();
        pQw[5].ul128 = *(u_long128*)&CShadowModelMan::getShadowParam();
        CPackMan::pkCloseUpkCodeV4_32(pQw + 6);
    } else if (m_TransTexCount) {
        if (!tex0) {
            tex0 = tex0Buf;
            getTexParam(tex0Buf);
        }
        pQw = (QWdata*)CPackMan::pkOpenUpkCodeV4_32(0x18, 4, 4);
        for (i = 0; i < m_TransTexCount; i++) {
            if (!tex0[i]) {
                pQw->ul128 = 0;
            } else {
                if (version & 0x400000) tex0[i] &= ~((u_long)1 << 34);
                pQw->ul64[0] = tex0[i];
                if (m_TexUV) pQw->ul64[1] = m_TexUV[i].luv;
                else pQw->ul64[1] = 0;
            }
            pQw++;
        }
        CPackMan::pkCloseUpkCodeV4_32(pQw);
    }

    {
        const QWdata* pHead = (const QWdata*)pData;
        INT y;
        if (!pHead[1].s16[3]) {
            y = pHead[1].u16[7];
            if (!y) y = 0x92;
        } else {
            y = pHead[1].s16[5];
        }
        qPP = _packWord(m_BBoxFlag, y, iFlags & 2, m_ShaderOption);
    }
    if (iFlags & 2) {
        FVECTOR* pColor = (FVECTOR*)&qColorScale;
        pColor->x = 1.0f;
        pColor->y = 1.0f;
        pColor->z = 1.0f;
        pColor->w = 1.0f;
    } else if (m_Mode == 0x10) {
        u_long128 tmp;
        __asm__ volatile("lqc2 $vf4, 0x0(%2)\n"
                         "mfc1 %0, %3\n"
                         "pcpyld %0, %0, $0\n"
                         "mfc1 %0, %4\n"
                         "qmtc2.ni %0, $vf5\n"
                         "vmulz.xyz $vf4, $vf4, $vf5z\n"
                         "vmulx.w $vf4, $vf4, $vf5x\n"
                         "sqc2 $vf4, 0x0(%1)\n"
                         : "=&r"(tmp)
                         : "r"(&qColorScale), "r"(&getColorScale()), "f"(CShadowModelMan::getColorDensity()), "f"(CShadowModelMan::getAlphaDensity())
                         : "memory");
    } else {
        qColorScale = *(u_long128*)&getColorScale();
    }
    pQw = (QWdata*)CPackMan::pkOpenUpkCodeV4_32(0x12, 4, 4);
    pQw->ul128 = 0;
    pQw++;
    qOBLM = _packWord((INT)m_pCrds, m_BoneStart, (INT)m_pLightUnit, nMode);
    MODEL_SEND_CACHE& sModelCache = CModelMan::getModelCacheRef();
    if (sModelCache.lOBLM_L != _lo64(qOBLM) || sModelCache.lOBLM_H != _hi64(qOBLM) ||
        ((u_long*)&sModelCache.ivPP)[0] != _lo64(qPP) || ((u_long*)&sModelCache.ivPP)[1] != _hi64(qPP) ||
        ((u_long*)&sModelCache.fvColorScale)[0] != *(u_long*)&qColorScale ||
        ((u_long*)&sModelCache.fvColorScale)[1] != _hi64(qColorScale) || (m_iSendFlg & 0x800)) {
        INT boneNum;
        INT boneSize;
        INT w;
        CCoord* pCrd;
        INT iSize;
        INT bottom;
        UINT* p32;
        sModelCache.qOBLM = qOBLM;
        *(u_long128*)&sModelCache.ivPP = qPP;
        *(u_long128*)&sModelCache.fvColorScale = qColorScale;
        pQw[0].ul128 = qColorScale;
        pQw[1].ul64[1] = 3;
        pQw[2].ul64[1] = 3;
        pQw[3].ul128 = qPP;
        pQw[4].ul64[1] = 0x3F;
        CPackMan::pkCloseUpkCodeV4_32(pQw + 5);

        if (m_pLightUnit) {
            m_pLightUnit->calc(TRUE);
            pQw = (QWdata*)CPackMan::pkOpenUpkCodeV4_32(4, 4, 4);
            {
                FMATRIX& mLight = m_pLightUnit->getLightMatrix();
                pQw[0].ul128 = ((QWdata*)&mLight)[0].ul128;
                pQw[1].ul128 = ((QWdata*)&mLight)[1].ul128;
                pQw[2].ul128 = ((QWdata*)&mLight)[2].ul128;
            }
            pQw[3].ul128 = *(u_long128*)&m_pLightUnit->getAmbientVector();
            *(FMATRIX*)&pQw[4] = m_pLightUnit->getColorMatrix();
        } else {
            pQw = (QWdata*)CPackMan::pkOpenUpkCodeV4_32(4, 4, 4);
            pQw[0].ul128 = 0;
            pQw[1].ul128 = 0;
            pQw[2].ul128 = 0;
            pQw[3].ul128 = *(u_long128*)&ONE_VECTOR;
            pQw[4].ul128 = 0;
            pQw[5].ul128 = 0;
            pQw[6].ul128 = 0;
            pQw[7].ul128 = 0;
        }
        pQw += 8;
        if (m_BoneIdx == -1) boneNum = m_pCrds->getCoordNum();
        else boneNum = 1;
        boneSize = m_pShader->m_BoneSize;
        pQw->ul128 = _packWord(boneNum, 0, boneSize, m_Mode);
        CPackMan::pkCloseUpkCodeV4_32(pQw + 1);

        pCrd = &m_pCrds->Crd(m_BoneStart);
        {
            INT addr = 0x28;
            i = 0;
            w = boneNum;
            do {
                iSize = w >= 0x40 ? 0x3F : w;
                pQw = (QWdata*)CPackMan::pkOpenUpkCodeV4_32(addr, boneSize, 4);
                w -= iSize;
                do {
                    *(FMATRIX*)pQw = pCrd[i].m_World;
                    addr += boneSize;
                    i++;
                    pQw += 4;
                } while (--iSize > 0);
                CPackMan::pkCloseUpkCodeV4_32(pQw);
            } while (w > 0);
        }
        if ((nMode == 0x40EE || nMode == 0x406E) && m_pLightUnit) {
            u_long128* p = (u_long128*)m_pLightUnit->getPointLightData();
            if (p) {
                INT k = 0;
                w = boneNum;
                do {
                    iSize = w >= 0x1A ? 0x19 : w;
                    w -= iSize;
                    pQw = (QWdata*)CPackMan::pkOpenUpkCodeV4_32(k * boneSize + 0x2F, boneSize, 10);
                    do {
                        INT offset;
                        for (offset = 0; offset < 10; offset++) {
                            pQw->ul128 = p[offset];
                            pQw++;
                        }
                        k++;
                    } while (--iSize > 0);
                    CPackMan::pkCloseUpkCodeV4_32(pQw);
                } while (w > 0);
            }
        }
        iSize = boneSize * boneNum + 0x28;
        bottom = 0x400 - m_pShader->m_WorkSize;
        p32 = (UINT*)CPackMan::pkGetCurrentPointer();
        bottom = (bottom - iSize) / 2;
        p32[0] = iSize | 0x03000000;
        p32[1] = bottom | 0x02000000;
        p32[2] = MPG_START | 0x14000000;
        p32[3] = 0x11000000;
        CPackMan::pkSetCurrentPointer(p32 + 4);
    } else {
        CPackMan::pkCloseUpkCodeV4_32(pQw);
    }
    CPackMan::pkCnt(0);
    CPackMan::pkAddCode(0x11000000);
    if (m_pAfterDirect) {
        CPackMan::pkOpenDirectCode(0);
        __asm__ volatile("pcpyld %0, %1, %2" : "=r"(tag) : "r"((u_long)0xE), "0"((u_long)m_iAfterDirectQNum | 0x8000 | 0x1000000000000000));
        pQw = (QWdata*)CPackMan::pkOpenGifTagManual(tag);
        {
            QWdata* pQwSrc = (QWdata*)m_pAfterDirect;
            for (i = 0; i < m_iAfterDirectQNum; i++) {
                pQw->ul128 = pQwSrc->ul128;
                pQw++;
                pQwSrc++;
            }
        }
        CPackMan::pkCloseGifTagManual((UINT*)pQw);
        CPackMan::pkCloseDirectCode();
    }
    CModelMan::incDrawCount();
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _disp0_SetPacket__6CModelUiPA3_CiiUliPUl);
#endif
/* Parked: 13 lines off: after getRawMode the original computes isDataEmpty && isSemiDataEmpty into a1 and never uses it (dead code left after a cross-jump?); ours drops the semi half. Tried: unused BOOL local, empty if, ternary, statement, duplicated identical arms (24 lines). */
#if 0
void CSemiModel::link(CModel* m) {
    UINT rawMode;
    m_pModel = m;
    setPacketSize(m->getModelData()->m_nPacketSize);
    rawMode = m->getModelData()->getRawMode();
    BOOL bEmpty = m->getModelData()->isDataEmpty() && m->getModelData()->isSemiDataEmpty();
    if (getLayerID() != 0x18 && getLayerID() != 0x17) {
        if (rawMode & 0x10000000) setSemiTrans(ALPHA_BLEND);
        else setSemiTrans(ALPHA_PLUS);
    }
    initSemiZModel();
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", link__10CSemiModelP6CModel);
#endif
void CSemiModel::initSemiZModel() {
    USHORT mode;
    UINT rawMode;
    mode = m_pModel->getMode();
    rawMode = m_pModel->getModelData()->getRawMode();
    if (MODELDATA::isCharacterAndMap(mode) && !(rawMode & 0x1200000) && !(getSemiTrans() == ALPHA_BLEND)) {
        m_pSemiZ = new (com_malloc_d(sizeof(CSemiZModel), __FILE__ "(3441)", RET_ADDR())) CSemiZModel;
        if (m_pSemiZ) {
            m_pSemiZ->m_SemiDraw = m_SemiDraw;
            m_pSemiZ->m_pModel = m_pModel;
            m_pSemiZ->setPacketSize(m_pModel->getModelData()->m_nPacketSize);
        }
    } else {
        m_pSemiZ = NULL;
    }
    if (MODELDATA::isMap(mode) && !(rawMode & 0x200000) && getSemiTrans() == ALPHA_BLEND) {
        CCoord& crd = m_pModel->getCrd(0);
        FVECTOR vMax;
        FVECTOR vMin;
        FVECTOR& p = crd.getWorldMatrix().v[3];
        m_pModel->getWorldBBox(vMax, vMin);
        if (vMin.x < p.x && p.x < vMax.x && vMin.z < p.z && p.z < vMax.z && vMin.y - 0.1f <= p.y && p.y <= vMax.y + 0.1f
            && vMax.x - vMin.x > 3.0f && vMax.z - vMin.z > 3.0f) {
            m_pModel->m_nFlag |= 0x2000;
        }
    }
}
void CSemiModel::releaseSemiZModel() {
    if (m_pSemiZ) delete m_pSemiZ;
}
void CSemiModel::changeLayerExt(BOOL bOn, INT nDepth) {
    if (bOn) setLayer(0x1b, nDepth);
    else setLayer(0x10, 0);
    if (m_pSemiZ) m_pSemiZ->changeLayerExt(bOn, nDepth + 1);
}
FIX4 CSemiModel::calcZ() {
    BOOL bSemiZ;
    FIX4 nZ;
    IVECTOR vScr;
    if ((m_pModel->getModelData()->getRawMode() & 0x200000) && m_pModel->getAlphaScale() >= 1.0f) {
        m_LayerInfo._nID = 0x11;
        if (m_pSemiZ) m_pSemiZ->disp();
        return 0;
    }
    if (m_LayerInfo._nID == 0x11) {
        if (m_LayerInfo._nDepth) m_LayerInfo._nID = 0x1b;
        else m_LayerInfo._nID = 0x10;
    }
    bSemiZ = TRUE;
    nZ = 0;
    if (m_pModel->getAlphaScale() < 1.0f && m_SemiDraw && (m_iFlag & 1) && m_pModel->m_pFadeModel) {
        nZ = m_pModel->m_pFadeModel->getZ() + 1;
        if (m_pModel->m_nFlag & 0x20) {
            if (m_pModel->m_nFlag & 0x800) bSemiZ = FALSE;
        }
    }
    if (m_pSemiZ && bSemiZ) m_pSemiZ->disp();
    if (m_pModel->isOTZFlag()) return m_pModel->getOTZ();
    if (nZ > 0) return nZ;
    m_pModel->getWorldBBoxScr(vScr, NULL);
    m_pModel->updateOTZ(vScr.z);
    return vScr.z;
}
UINT CSemiModel::getPacketSize() const {
    return m_nPacketSize + m_pModel->getBeforeDirectNum() + m_pModel->getAfterDirectNum();
}
void CSemiModel::makePkt() {
    CPackMan::pkCloseDirectCode();
    if (m_pModel->m_nFlag & 2) dispDirect();
    CPackMan::pkOpenDirectCode(0);
}
void CSemiModel::setSemiTrans(ALPHA a) {
    UINT rawMode;
    if (!(a == ALPHA_PLUS) && !(a == ALPHA_MINUS) && !(a == ALPHA_BLEND)) {
        if (!(m_pModel)) AssertApp(__FILE__, 3588, "m_pModel", 0);
        rawMode = m_pModel->getModelData()->getRawMode();
        if (rawMode & 0x10000000) a = ALPHA_BLEND;
        else a = ALPHA_PLUS;
    }
    CDrawable::setSemiTrans(a);
}
void CSemiModel::dispDirect() {
    USHORT mode;
    UINT rawMode;
    INT nSemiKind;
    ULONGLONG nInTest;
    mode = m_pModel->getMode();
    rawMode = m_pModel->getModelData()->getRawMode();
    nSemiKind = getSemiTrans();
    nInTest = getTestFlag();
    if (m_pModel->getAlphaScale() < 1.0f && MODELDATA::isMap(mode)) nSemiKind = 2;
    if (nSemiKind == 1 && MODELDATA::isCharacterAndMap(mode) && !(rawMode & 0x1200000)) nInTest = 0x5000D;
    m_pModel->disp0(nSemiKind, nInTest, m_SemiDraw != 0);
}
void CSemiZModel::makePkt() {
    CPackMan::pkCloseDirectCode();
    CModel* m = getParentModel();
    m->disp0(getSemiTrans(), getTestFlag(), m_SemiDraw ? 5 : 4);
    if ((m->m_nFlag & 4) && MODELDATA::isCharacter(m->m_Mode)) {
        m->m_ShaderOption |= 0x80;
        m->disp0(getSemiTrans(), m->getTestFlag(), m_SemiDraw != 0);
        m->m_ShaderOption &= ~0x80;
    }
    CPackMan::pkOpenDirectCode(0);
}
void CFadeModel::makePkt() {
    USHORT mode;
    ULONGLONG lTest;
    INT iA;
    CPackMan::pkCloseDirectCode();
    mode = m_pModel->getMode();
    if (getLayerID() == 0x18) {
        m_pModel->disp0(getSemiTrans(), getTestFlag(), 2);
    } else if (getLayerID() == 0x17) {
        m_pModel->disp0(getSemiTrans(), getTestFlag(), 4);
    } else if (MODELDATA::isMap(mode)) {
        m_pModel->disp0(2, getTestFlag(), m_SemiDraw != 0);
    } else {
        iA = 0;
        if ((m_pModel->m_nFlag & 0x20)
            || (MODELDATA::isCharacter(m_pModel->getMode()) && !(m_pModel->getModelData()->getRawMode() & 0x1000000))) {
            iA = (INT)(m_pModel->getAlphaScale() * 96.0f);
            lTest = ((ULONGLONG)iA << 4) | 0x5000D;
        } else {
            lTest = getTestFlag();
        }
        if (m_pModel->m_nFlag & 0x20) {
            ULONGLONG lFrame;
            u_long* pQw;
            u_long128 tag;
            lFrame = CPackMan::getDefault_FRAME_1() | 0xFFFFFFFF00000000;
            CPackMan::pkCnt(0);
            CPackMan::pkAddCode(0x13000000);
            CPackMan::pkOpenDirectCode(0);
            __asm__ volatile("pcpyld %0, %2, %1" : "=r"(tag) : "r"((u_long)0x1000000000008001), "0"((u_long)0xE));
            pQw = (u_long*)CPackMan::pkOpenGifTagManual(tag);
            pQw[0] = lFrame;
            pQw[1] = 0x4C;
            CPackMan::pkCloseGifTagManual((UINT*)(pQw + 2));
            CPackMan::pkCloseDirectCode();
            m_pModel->disp0(2, lTest, m_SemiDraw != 0);
            if (m_pModel->m_nFlag & 0x800) {
                if (m_pModel->m_nFlag & 2) {
                    if (m_pModel->m_nFlag & 1) m_pModel->disp0(2, ((ULONGLONG)(iA >> 2) << 4) | 0x5000D, 1);
                }
            }
            CPackMan::pkOpenDirectCode(0);
            __asm__ volatile("pcpyld %0, %2, %1" : "=r"(tag) : "r"((u_long)0x1000000000008001), "0"((u_long)0xE));
            pQw = (u_long*)CPackMan::pkOpenGifTagManual(tag);
            pQw[0] = CPackMan::getDefault_FRAME_1();
            pQw[1] = 0x4C;
            CPackMan::pkCloseGifTagManual((UINT*)(pQw + 2));
            CPackMan::pkCloseDirectCode();
        }
        m_pModel->disp0(1, lTest, m_SemiDraw != 0);
    }
    CPackMan::pkOpenDirectCode(0);
}
UINT CFadeModel::getPacketSize() const {
    UINT nPacketSize = m_nPacketSize;
    if (m_pModel && (m_pModel->m_nFlag & 0x20)) nPacketSize *= 2;
    return nPacketSize + m_pModel->getBeforeDirectNum() + m_pModel->getAfterDirectNum();
}
void CModelBG::makePkt() {
    CPackMan::pkCloseDirectCode();
    if (!isDataEmpty()) disp0(-1, getTestFlag(), 0);
    if (isAvailableSemiModel() && getSemiModel()) getSemiModel()->dispDirect();
    CPackMan::pkOpenDirectCode(0);
}
FIX4 CBBox::calcZ() { return 0x640; }
/* Parked: 116 lines off, register allocation: structure matches (8-corner RotTransPers loop with min/max and W range, colour checks, four line-strip GIF packets), but the original keeps this in s4, &m_C0 in s1 and different caller-saved temporaries spilled around RotTransPers. Tried: x[0] > x[1] compare, a G_RGBA set helper (worse). */
#if 0
static inline u_long _xyz(const IVECTOR& v) { return (u_long)v.x | ((u_long)v.y << 16) | ((u_long)v.z << 32); }
void CBBox::makePkt() {
    int abort;
    FMATRIX rtp;
    FVECTOR v;
    IVECTOR c[2][2][2];
    float w[2][2][2];
    int x;
    int y;
    int z;
    INT xmin;
    INT xmax;
    INT ymin;
    INT ymax;
    FLOAT wmax;
    FLOAT wmin;
    QWdata giftag;
    if (m_bbox.x[0] > m_bbox.x[1]) return;
    abort = 0;
    m_pCrd->getRotTransPersMatrix(rtp);
    wmax = 0.0f;
    wmin = 1.0f;
    m_C0.r = 0xFF;
    m_C0.g = 0;
    m_C0.b = 0;
    xmin = 0x10000;
    xmax = 0;
    ymin = 0x10000;
    ymax = 0;
    for (x = 0; x < 2; x++) {
        for (y = 0; y < 2; y++) {
            for (z = 0; z < 2; z++) {
                v.x = m_bbox.x[x];
                v.y = m_bbox.y[y];
                v.z = m_bbox.z[z];
                v.w = 1.0f;
                w[x][y][z] = RotTransPers(c[x][y][z], rtp, v);
                if (w[x][y][z] < 0.0f) {
                    m_C0.r = 0;
                    abort = 1;
                    m_C0.g = 0;
                    m_C0.b = 0xFF;
                } else {
                    if (wmax < w[x][y][z]) wmax = w[x][y][z];
                    if (w[x][y][z] < wmin) wmin = w[x][y][z];
                }
                if (c[x][y][z].x < 0 || c[x][y][z].x > 0xFFFF || c[x][y][z].y < 0 || c[x][y][z].y > 0xFFFF) {
                    m_C0.r = 0;
                    abort = 1;
                    m_C0.g = 0xFF;
                    m_C0.b = 0xFF;
                }
                if (c[x][y][z].x < xmin) xmin = c[x][y][z].x;
                if (c[x][y][z].x > xmax) xmax = c[x][y][z].x;
                if (c[x][y][z].y < ymin) ymin = c[x][y][z].y;
                if (c[x][y][z].y > ymax) ymax = c[x][y][z].y;
            }
        }
    }
    if (abort) return;
    if (wmax < 1.0f / CWorldEnv::getDispFar()) {
        m_C0.r = 0xFF;
        m_C0.g = 0xFF;
        m_C0.b = 0;
    }
    if (1.0f / CWorldEnv::getFogNear() < wmin) {
        m_C0.r = 0xFF;
        m_C0.b = 0xFF;
        m_C0.g = 0;
    }
    if (xmin > 0x9000 || xmax < 0x7000 || ymin > 0x8C00 || ymax < 0x7400) {
        m_C0.r = 0xFF;
        m_C0.b = 0xFF;
        m_C0.g = 0xFF;
    }
    giftag.ul64[0] = 0x6400000000008000;
    giftag.ul64[1] = 0x444410;
        CPackMan::pkOpenGifTag(giftag.ul128);
        CPackMan::pkAddGsData(0x282 | ((u_long)(m_nAttribute & 1) << 6));
        CPackMan::pkAddGsData(m_C0.col);
        CPackMan::pkAddGsData(_xyz(c[0][0][0]));
        CPackMan::pkAddGsData(_xyz(c[1][0][0]));
        CPackMan::pkAddGsData(_xyz(c[1][1][0]));
        CPackMan::pkAddGsData(_xyz(c[1][1][1]));
        CPackMan::pkCloseGifTag();
        CPackMan::pkOpenGifTag(giftag.ul128);
        CPackMan::pkAddGsData(0x282 | ((u_long)(m_nAttribute & 1) << 6));
        CPackMan::pkAddGsData(m_C0.col);
        CPackMan::pkAddGsData(_xyz(c[1][1][0]));
        CPackMan::pkAddGsData(_xyz(c[0][1][0]));
        CPackMan::pkAddGsData(_xyz(c[0][0][0]));
        CPackMan::pkAddGsData(_xyz(c[0][0][1]));
        CPackMan::pkCloseGifTag();
        CPackMan::pkOpenGifTag(giftag.ul128);
        CPackMan::pkAddGsData(0x282 | ((u_long)(m_nAttribute & 1) << 6));
        CPackMan::pkAddGsData(m_C0.col);
        CPackMan::pkAddGsData(_xyz(c[1][0][1]));
        CPackMan::pkAddGsData(_xyz(c[1][1][1]));
        CPackMan::pkAddGsData(_xyz(c[0][1][1]));
        CPackMan::pkAddGsData(_xyz(c[0][1][0]));
        CPackMan::pkCloseGifTag();
        CPackMan::pkOpenGifTag(giftag.ul128);
        CPackMan::pkAddGsData(0x282 | ((u_long)(m_nAttribute & 1) << 6));
        CPackMan::pkAddGsData(m_C0.col);
        CPackMan::pkAddGsData(_xyz(c[0][1][1]));
        CPackMan::pkAddGsData(_xyz(c[0][0][1]));
        CPackMan::pkAddGsData(_xyz(c[1][0][1]));
        CPackMan::pkAddGsData(_xyz(c[1][0][0]));
        CPackMan::pkCloseGifTag();
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", makePkt__5CBBox);
#endif

void CModel::linkTexAnm(CTexAnm* pATbl, INT nANum) {
    INT nTexMax;
    if (pATbl && nANum > 0) {
        nTexMax = m_TransTexCount;
        if (!m_pTexAnm && nTexMax > 0 && CModelTexAnm::getTexAnmCount(pATbl, nANum, (TexInfoStub*)m_TexInfo, nTexMax) > 0) {
            m_pTexAnm = new (com_malloc_d(sizeof(CModelTexAnm), __FILE__ "(4066)", RET_ADDR())) CModelTexAnm;
            if (!(m_pTexAnm)) AssertApp(__FILE__, 4067, "m_pTexAnm", 0);
            m_pTexAnm->setup(pATbl, nANum, (TexInfoStub*)m_TexInfo, nTexMax);
        }
    }
}
void CModel::setupUVScroll(INT bVal) {
    INT i;
    m_nFlag &= ~0x80;
    if (m_TexUV) delete m_TexUV;
    m_TexUV = NULL;
    for (i = 0; i < m_TransTexCount; i++) {
        if ((m_TexInfo[i].m_pTex[0x55] & 0x80) || bVal) {
            m_nFlag |= 0x80;
            bVal = m_TransTexCount;
            m_TexUV = new (com_malloc_d(bVal * sizeof(_TEX_UV_OFFSET), __FILE__ "(4086)", RET_ADDR())) _TEX_UV_OFFSET[bVal];
            m_nFlag |= 0x40;
            break;
        }
    }
}
void CModel::updateUVScroll(FLOAT fr) {
    INT i;
    if (isUVScroll()) {
        for (i = 0; i < m_TransTexCount; i++) {
            if (m_TexInfo[i].m_pTex[0x55] & 0x80) {
                m_TexUV[i].fu += *(FLOAT*)(m_TexInfo[i].m_pTex + 0x58) * fr;
                m_TexUV[i].fv += *(FLOAT*)(m_TexInfo[i].m_pTex + 0x5c) * fr;
                if (m_TexUV[i].fu > 1.0f) m_TexUV[i].fu -= 1.0f;
                else if (m_TexUV[i].fu < 0.0f) m_TexUV[i].fu += 1.0f;
                if (m_TexUV[i].fv > 1.0f) m_TexUV[i].fv -= 1.0f;
                else if (m_TexUV[i].fv < 0.0f) m_TexUV[i].fv += 1.0f;
            }
        }
    }
}
void CModel::setUVScrollOffset(INT nNum, FLOAT fU, FLOAT fV) {
    if (isUVScroll() && nNum < m_TransTexCount) {
        ((FLOAT*)m_TexUV)[nNum * 2] = fU;
        ((FLOAT*)m_TexUV)[nNum * 2 + 1] = fV;
    }
}
void* CModel::getExtendClut(INT nTex, INT nType) {
    if (nTex >= 0 && nTex < m_pData->m_nTexNum) {
        BYTE* p = m_TexInfo[nTex].m_pTex;
        if (nType == 1) return *(void**)(p + 0x48);
        if (nType == 2) return *(void**)(p + 0x4c);
        if (nType == 3) goto ret3;
    }
    return NULL;
ret3:
    return *(void**)(m_TexInfo[nTex].m_pTex + 0x50);
}
/* Parked: 5 lines off, operand order only: the original adds this + idx*8 (addu a1,a0,v0), ours idx*8 + this. Worked: the if/else-if chain with `nInType == t && p[0x54] != t`, the direct p->ext arm first, raw this-relative addressing (m_TexInfo[inIdx] makes gcc pre-add this+0x68). Tried: switch, nested ifs, expression orders; permuter 10 min (34061 candidates, stays 5). */
#if 0
void CModel::getChangeClut(INT inIdx, INT nInType) {
    BYTE* p;
    if (inIdx < 0 || inIdx >= m_pData->m_nTexNum) return;
    p = (*(BYTE**)((BYTE*)this + inIdx * 8 + 0x68));
    if (!(p[0x55] & 0x40)) return;
    if (nInType == 0 && p[0x54] != 0) {
        (*(BYTE**)((BYTE*)this + inIdx * 8 + 0x68)) = *(BYTE**)(p + 0x48);
    } else if (nInType == 1 && p[0x54] != 1) {
        if (!p[0x54]) (*(BYTE**)((BYTE*)this + inIdx * 8 + 0x68)) = *(BYTE**)(p + 0x48);
        else (*(BYTE**)((BYTE*)this + inIdx * 8 + 0x68)) = *(BYTE**)(*(BYTE**)(p + 0x48) + 0x48);
    } else if (nInType == 2 && p[0x54] != 2) {
        if (!p[0x54]) (*(BYTE**)((BYTE*)this + inIdx * 8 + 0x68)) = *(BYTE**)(p + 0x4c);
        else (*(BYTE**)((BYTE*)this + inIdx * 8 + 0x68)) = *(BYTE**)(*(BYTE**)(p + 0x48) + 0x4c);
    } else if (nInType == 3 && p[0x54] != 3) {
        if (!p[0x54]) (*(BYTE**)((BYTE*)this + inIdx * 8 + 0x68)) = *(BYTE**)(p + 0x50);
        else (*(BYTE**)((BYTE*)this + inIdx * 8 + 0x68)) = *(BYTE**)(*(BYTE**)(p + 0x48) + 0x50);
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getChangeClut__6CModelii);
#endif
BOOL MODELDATA::enablePointLight() const {
    BOOL ret;
    if (getRawMode() & 0x2000000) return FALSE;
    ret = FALSE;
    if (getMode() == 0x40EE || getMode() == 0x406E) ret = TRUE;
    return ret;
}
BOOL MODELDATA::enableLight() const {
    if (getRawMode() & 0x2000000) return FALSE;
    USHORT mode = getMode();
    if (mode == 2 || mode == 6 || mode == 0x406 || mode == 0x400B || mode == 0x4005 || mode == 0x4205) return TRUE;
    return FALSE;
}
BOOL MODELDATA::isCharacter(UINT mode) {
    switch (mode) {
    case 5:
    case 6:
    case 0x406:
    case 0x400b:
    case 0x406e:
        return TRUE;
    }
    return FALSE;
}
BOOL MODELDATA::isMap(UINT mode) {
    switch (mode) {
    case 0:
    case 0x200:
    case 0x40ee:
        return TRUE;
    }
    return FALSE;
}
BOOL MODELDATA::isSky(UINT mode) { return mode == 0x24; }
BOOL MODELDATA::isCharacterAndMap(UINT mode) {
    switch (mode) {
    case 0:
    case 5:
    case 6:
    case 0x200:
    case 0x406:
    case 0x400b:
    case 0x406e:
    case 0x40ee:
        return TRUE;
    }
    return FALSE;
}
BOOL MODELDATA::isNormal(UINT mode) {
    switch (mode) {
    case 0:
    case 3:
    case 4:
    case 5:
    case 6:
    case 0x10:
    case 0x200:
    case 0x406:
    case 0x4009:
    case 0x400b:
    case 0x4109:
    case 0x4123:
        return FALSE;
    }
    return TRUE;
}
BOOL MODELDATA::isColorI(UINT mode) {
    switch (mode) {
    case 2:
    case 6:
    case 0x10:
    case 0x102:
    case 0x108:
    case 0x406:
    case 0x4005:
    case 0x4009:
    case 0x400b:
    case 0x4109:
    case 0x4123:
    case 0x4205:
        return FALSE;
    }
    return TRUE;
}
BOOL MODELDATA::isColorF(UINT mode) {
    switch (mode) {
    case 0x406:
    case 0x4009:
    case 0x400b:
    case 0x4109:
        return TRUE;
    }
    return FALSE;
}
BOOL MODELDATA::isTexture(UINT mode) {
    switch (mode) {
    case 0x10:
    case 0x4005:
    case 0x4009:
    case 0x4109:
    case 0x4205:
        return FALSE;
    }
    return TRUE;
}
INT Mad::getModelAttachData(const MODEL_ATTACH_DATA** outMad, INT iRId) {
    MAD_HEADER* header;
    MODEL_ATTACH_DATA* mad;
    *outMad = NULL;
    header = (MAD_HEADER*)CRsrcDB::getFirstData(0x23, iRId, TRUE);
    if (!header) return 0;
    if (strncmp(header->szCode, "@MAD", 4) != 0) {
        TRACE("MAD\203t\203@\203C\203\213\202\305\202\315\202\310\202\242\n");
        return 0;
    }
    if (header->uiVersion != 0x100) {
        TRACE("MAD\202\314\203o\201[\203W\203\207\203\223\202\252\210\341\202\244\n");
        return 0;
    }
    mad = (MODEL_ATTACH_DATA*)((BYTE*)header + header->uiOffset);
    if (!(mad)) AssertApp(__FILE__, 4422, "mad", 0);
    *outMad = mad;
    return header->uiNum;
}

// SOURCE DATA: .data .rodata .bss
