#include "khtypes.h"
#define SYSTEM_GRAPHIC_MODEL_CPP
#define MODELDATA_H_TEXINFO /* PARTIAL: modeldata.h's TexInfo/TexInfoStub (gated for System/Graphic/ModelMan) */
#include "System/Graphic/Model.h"
#include "System/Task.h"
#include "System/Coord.h"
#include "System/Math3D.h"
#include "System/Graphic/WorldEnv.h"
#include "System/Graphic/TexAnm.h"
#include "System/Graphic/TexUtil.h"
#include "System/Graphic/ShadowModel.h"
#include "include_asm.h"

class CRsrcData;
/* PARTIAL: stand-in for System/RsrcDB NAME_CASE: strings: RsrcData.h brings "System/RsrcData.h(156)", not in the original (queue: RsrcDB partition) */
enum NAME_CASE { NAME_CASE_SENSITIVE = 0 };
class CRsrcDB { /* PARTIAL: stand-in for System/RsrcDB CRsrcDB: strings: RsrcDB.h brings "System/RsrcData.h(156)", not in the original (queue: RsrcDB partition) */
public:
    static MODELDATA** getRsrcMDL(const CHAR* pszName, NAME_CASE nc);
    static MODELDATA** getRsrcMDL(INT nGroup);
    static TexInfo* getRsrcTex(const CHAR* pszName, NAME_CASE nc);
    static void* getFirstData(INT nRsrcType, INT nGroupID, BOOL bFollowLink);
    static MODELDATA* getRsrcAMF(const CHAR* pszName, NAME_CASE nc);
};
class CPackMan { /* PARTIAL: stand-in for System/Graphic/PackMan CPackMan: owner not written */
public:
    static void pkCloseDirectCode();
    static void pkOpenDirectCode(INT stall);
    static BOOL isOverFlow(UINT nSize);
    static u_long getDefault_TEST_1();
    static void pkCall(u_long128* pAddr, UINT nFlag);
    static void pkAddCode(UINT nCode);
    static void pkCnt(UINT nCnt);
};
extern const FMATRIX ONE_MATRIX;
INT checkBBox(const FMATRIX& mWorldMatrix, const BBOX* bbox);
void getApplyMatrixBBox(FVECTOR& vOutMax, FVECTOR& vOutMin, const FMATRIX& Mat, const FVECTOR& vInMax, const FVECTOR& vInMin); /* static in the original (parked, asm glabel) */
extern "C" const FLOAT D_00460FB0; /* -FLT_MAX / 2 */
extern "C" const FLOAT D_00460FB4; /* FLT_MAX / 2 */
extern "C" char D_00460DD8[]; /* "System/Graphic/Model.h" */
extern "C" const FLOAT D_00460FA8; /* -FLT_MAX / 2 */
extern "C" const FLOAT D_00460FAC; /* FLT_MAX / 2 */
namespace VRAM { /* PARTIAL: stand-in for System/VramMan VRAM: owner not written */
extern INT _TEX_TMP1_START;
extern INT _CLUT_TMP1_START;
}
extern "C" char D_00460D98[]; /* "System/Graphic/modeldata.h" */
extern "C" char D_00460DC8[];
extern "C" char D_00460F30[];
extern "C" char D_00460F60[];
extern "C" char D_00460F78[];
extern "C" char D_00460E00[];
extern "C" char D_00460F38[];
extern "C" char D_00460FB8[];
extern "C" char D_00461110[];
inline void* operator new[](size_t, void* p) throw() { return p; }
extern u_long128* g_mpg0;
extern u_long128* g_mpg2;
extern "C" char D_00461058[];
extern "C" char D_004610C0[];
extern "C" char D_004610D0[];
extern "C" char D_00461100[];
extern "C" char D_00461140[];
extern "C" char D_00461148[];
extern "C" char D_00461160[];
extern "C" char D_00461178[];
extern "C" int strncmp(const char*, const char*, unsigned int);
class CShadowMapModel : public CFadeModel { /* PARTIAL: stand-in for CShadowMapModel: owner not written */
    HANDLE m_hHnd; /* 0x28 */
public:
    virtual ~CShadowMapModel();
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
extern "C" SHADERDESC D_00505118[]; /* the shader table */
extern "C" char D_00460E80[];
extern "C" char D_00460EB8[]; /* __FILE__ */
extern "C" char D_00460EE8[];
extern "C" char D_00460F18[];
extern "C" char D_00460F58[];
extern "C" char D_00460EF0[];
extern "C" char D_00460F10[];
extern "C" char D_00460F20[];
extern "C" char D_00460F28[];
extern "C" int strcmp(const char*, const char*);
extern "C" int strncasecmp(const char*, const char*, unsigned int);
struct TexInfo;

class CShader;

CShader* createDummyShader() { return NULL; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", __6CModelPC9MODELDATA);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _$_6CModel);
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
    SHADERDESC* ret = D_00505118;
    while (TRUE) {
        if (ret->m_Mode == 0xFFFF) {
            TRACE(D_00460E80);
            return NULL;
        }
        if (ret->m_Mode == (mode & ret->m_Mask)) return ret;
        ret++;
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", searchShaderDesc__FUs);
#endif
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
    if (!(p)) AssertApp(D_00460EB8, 451, D_00460EE8, 0);
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
        TRACE(D_00460EF0, pName);
        AssertApp(D_00460EB8, 469, D_00460F10, 0);
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
    if (!(ret)) AssertApp(D_00460EB8, 500, D_00460F18, 0);
    return ret[nIdx];
}
MODELDATA* AMFfromMDL(INT nGroup, INT nIdx) {
    MODELDATA** ret = CRsrcDB::getRsrcMDL(nGroup);
    if (!(ret)) AssertApp(D_00460EB8, 508, D_00460F18, 0);
    return ret[nIdx];
}
INT CModel::assign(CTask* p, const CHAR* pName, INT n) {
    if (!(p)) AssertApp(D_00460EB8, 516, D_00460EE8, 0);
    CCoords* pCrds = p->getCoords();
    if (!(pCrds)) AssertApp(D_00460EB8, 518, D_00460F20, 0);
    m_pTask = p;
    return assign(*pCrds, pName, n);
}
INT CModel::assign(CCoords& crds, const CHAR* pName, INT n) {
    MODELDATA* d = CRsrcDB::getRsrcAMF(pName, NAME_CASE_SENSITIVE);
    if (!d) {
        TRACE(D_00460EF0, pName);
        AssertApp(D_00460EB8, 530, D_00460F28, 0);
    }
    m_pData = d;
    return assign(crds, n);
}
INT CModel::assign(CTask* p, INT n) {
    if (!(p)) AssertApp(D_00460EB8, 537, D_00460EE8, 0);
    CCoords* pCrds = p->getCoords();
    if (!(pCrds)) AssertApp(D_00460EB8, 539, D_00460F20, 0);
    m_pTask = p;
    return assign(*pCrds, n);
}
INT CModel::assign(CCoords& crds, INT crdidx) {
    INT i;
    if (!(m_pData)) AssertApp(D_00460EB8, 546, D_00460F30, 0);
    m_pCrds = &crds;
    m_BoneStart = crdidx;
    m_BoneSkip = 0;
    m_BoneIdx = 0;
    m_TransTexCount = m_pData->m_nTexNum;
    if (!(!(m_TransTexCount > 16))) AssertApp(D_00460EB8, 579, D_00460F38, 0);
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
    if (!(p)) AssertApp(D_00460EB8, 620, D_00460EE8, 0);
    CCoords* pCrds = p->getCoords();
    if (!pCrds) {
        p->createCoords();
        pCrds = p->getCoords();
    }
    m_pTask = p;
    return link(*pCrds);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", link__6CModelR7CCoords);
CCoord& CModel::getCrd(INT idx) {
    if (m_BoneIdx != -1) idx = 0;
    return m_pCrds->Crd(m_BoneStart + idx);
}
INT CModel::getCoordNum() const {
    if (!(m_pCrds)) AssertApp(D_00460EB8, 737, D_00460F58, 0);
    return *(INT*)((BYTE*)m_pCrds + 4);
}
void CModel::setLocal(INT index) {
    if (!(m_pCrds != __null)) AssertApp(D_00460EB8, 743, D_00460F60, 0);
    if (!(0 <= index && index < m_pCrds->getCoordNum())) AssertApp(D_00460EB8, 744, D_00460F78, 0);
    if (!(m_pData != __null)) AssertApp(D_00460EB8, 745, D_00460E00, 0);
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
            if (!t->_pTexInfo) AssertApp(D_00460D98, 200, D_00460DC8, 0);
            if (!t->_pTexInfo->isFix()) CTexUtil::sumupTmpTexSize(*t->_pTexInfo, nTmpTex, nTmpClut);
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
    vMN[0].set(D_00460FA8, D_00460FA8, D_00460FA8, 1.0f);
    vMN[1].set(D_00460FAC, D_00460FAC, D_00460FAC, 1.0f);
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
    if (!(m_pData)) AssertApp(D_00460EB8, 1067, D_00460F30, 0);
    pCrd = &getCrd(0);
    iBoneNum = m_pData->m_nBoneNum;
    vMax.set(D_00460FB0, D_00460FB0, D_00460FB0, 1.0f);
    vMin.set(D_00460FB4, D_00460FB4, D_00460FB4, 1.0f);
    for (i = 0; i < iBoneNum; i++, pCrd++) {
        if (!(m_pData != __null)) AssertApp(D_00460DD8, 262, D_00460E00, 0);
        BBOX* bb = &m_pData->m_pBBox[i];
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
void CModel::setAlphaScale(FLOAT inScale) { m_fAlphaScale = inScale; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", linksub__6CModel);
void CModel::addTexture(const CHAR* texName, INT idx) {
    SHORT nTmpTex;
    SHORT nTmpClut;
    INT i;
    if (m_TransTexCount < m_pData->m_nTexNum + 1) {
        m_TransTexCount = m_pData->m_nTexNum + 1;
        if (!(!(m_TransTexCount > 16))) AssertApp(D_00460EB8, 1867, D_00460F38, 0);
    }
    ((TexInfoStub*)&m_TexInfo[m_pData->m_nTexNum])->init(CRsrcDB::getRsrcTex(texName, NAME_CASE_SENSITIVE), -1, -1);
    nTmpTex = 0;
    nTmpClut = 0;
    for (i = 0; i < m_TransTexCount; i++) {
        TexInfoStub* t = (TexInfoStub*)&m_TexInfo[i];
        t->update(nTmpTex, nTmpClut);
        if (!t->_pTexInfo) AssertApp(D_00460D98, 200, D_00460DC8, 0);
        if (!t->_pTexInfo->isFix()) CTexUtil::sumupTmpTexSize(*t->_pTexInfo, nTmpTex, nTmpClut);
    }
    setupUVScroll(0);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setShadowMapModel__6CModel);
void CModel::attachSemiModel(CSemiModel& inModel, BOOL bInManage) {
    if (m_pSemiModel && isManagedSemiModel()) delete m_pSemiModel;
    m_pSemiModel = &inModel;
    if (bInManage) m_nFlag |= 1;
    else m_nFlag &= ~1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", sendTexture__6CModelPUl);
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
            if (!t->_pTexInfo) AssertApp(D_00460D98, 200, D_00460DC8, 0);
            if (t->_pTexInfo->isFix()) {
                if (!t->_pTexInfo) AssertApp(D_00460D98, 204, D_00460DC8, 0);
                tex0[i] = t->_pTexInfo->getTPage();
            } else {
                if (!t->_pTexInfo) AssertApp(D_00460D98, 201, D_00460DC8, 0);
                if (t->_pTexInfo->isFixTex()) nTexPos = t->_pTexInfo->_TexInfo._nDistPos;
                else nTexPos = nTmpTex + t->_nTexPos;
                if (!t->_pTexInfo) AssertApp(D_00460D98, 202, D_00460DC8, 0);
                if (t->_pTexInfo->isFixClut()) nClutPos = t->_pTexInfo->_ClutInfo._nDistPos;
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
    if (m_fAlphaScale <= 0.0f) return FALSE;
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", dispImpl__6CModel);
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
    if (!(m_pShader)) AssertApp(D_00460EB8, 2446, D_00460FB8, 0);
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
    if (!((m_pMpg == g_mpg0) || (m_pMpg == g_mpg2) || (m_pMpg == __null))) AssertApp(D_00460EB8, 2539, D_00461058, 0);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _disp0_SetPacket__6CModelUiPA3_CiiUliPUl);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", initSemiZModel__10CSemiModel);
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
        if (!(m_pModel)) AssertApp(D_00460EB8, 3588, D_004610C0, 0);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", makePkt__10CFadeModel);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", makePkt__5CBBox);
void CModel::linkTexAnm(CTexAnm* pATbl, INT nANum) {
    INT nTexMax;
    if (pATbl && nANum > 0) {
        nTexMax = m_TransTexCount;
        if (!m_pTexAnm && nTexMax > 0 && CModelTexAnm::getTexAnmCount(pATbl, nANum, (TexInfoStub*)m_TexInfo, nTexMax) > 0) {
            m_pTexAnm = new (com_malloc_d(sizeof(CModelTexAnm), D_004610D0, RET_ADDR())) CModelTexAnm;
            if (!(m_pTexAnm)) AssertApp(D_00460EB8, 4067, D_00461100, 0);
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
            m_TexUV = new (com_malloc_d(bVal * sizeof(_TEX_UV_OFFSET), D_00461110, RET_ADDR())) _TEX_UV_OFFSET[bVal];
            m_nFlag |= 0x40;
            break;
        }
    }
}
/* Parked: 2 lines off: the original loads the flag byte into v0 and shifts into v1 (lbu v0; dsrl v1,v0,7, as the weak isUVScroll copy), ours into one register. Worked: isUVScroll() as a member inline returning the u_long bit-field. Tried: static helper with BOOL/u_long return, ternary; permuter 10 min (17866 candidates, stays 2). */
#if 0
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
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", updateUVScroll__6CModelf);
#endif
void CModel::setUVScrollOffset(INT nNum, FLOAT fU, FLOAT fV) {
    /* MATCH: isUVScroll() as the weak copy shows it (lbu + dsrl: a 64-bit bit-field, result in a 64-bit local) */
    struct _flag { u_long b0 : 7; u_long b7 : 1; };
    u_long b = 0;
    if (m_TexUV) b = ((_flag*)&m_nFlag)->b7;
    if (b && nNum < m_TransTexCount) {
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
    if (strncmp(header->szCode, D_00461140, 4) != 0) {
        TRACE(D_00461148);
        return 0;
    }
    if (header->uiVersion != 0x100) {
        TRACE(D_00461160);
        return 0;
    }
    mad = (MODEL_ATTACH_DATA*)((BYTE*)header + header->uiOffset);
    if (!(mad)) AssertApp(D_00460EB8, 4422, D_00461178, 0);
    *outMad = mad;
    return header->uiNum;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", __static_initialization_and_destruction_0_00133620);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", __10CSemiModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _$_10CSemiModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", reset__10CSemiModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", disp__10CSemiModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", relink__10CSemiModelP6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setSemiSort__10CSemiModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getParentModel__10CSemiModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setOldZ__10CSemiModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getOldZ__10CSemiModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setDefaultSemiTrans__10CSemiModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", replaceParentModel__10CSemiModelP6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setPacketSize__10CSemiModelUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _$_10CFadeModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", __10CFadeModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _$_11CSemiZModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", __11CSemiZModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", changeLayerExt__11CSemiZModelii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", calcZ__11CSemiZModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", link__6CModelP5CTaskRC9MODELDATA);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", link__6CModelP5CTaskPC9MODELDATA);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", link__6CModelR7CCoordsRC9MODELDATA);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", link__6CModelR7CCoordsPC9MODELDATA);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", assign__6CModelP5CTaskR9MODELDATAi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", assign__6CModelP5CTaskP9MODELDATAi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", assign__6CModelR7CCoordsR9MODELDATAi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", assign__6CModelR7CCoordsP9MODELDATAi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setSemiTrans__6CModelG5ALPHA);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getSemiModel__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setSemiSort__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setLightUnit__6CModelR10CLightUnit);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setLightUnit__6CModelP10CLightUnit);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getCoords__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getPacketSize__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isDataEmpty__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isSemiDataEmpty__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", changeTexture__6CModeliPC7TexInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getBoneName__C6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getParentBoneName__C6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getMode__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getBBoxFlag__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setBBoxFlag__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getBBox__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getModelData__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getTextureInfo__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setTextureInfo__6CModeliPC11TexInfoStub);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", updateTextureInfo__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getTextureNum__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getTransTextureNum__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", fade2Path__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isFade2Path__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", notSemi__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isNotSemi__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", dispOff__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isDispOff__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isDisp__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setBeforeDirect__6CModelPCUxi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", clearBeforeDirect__6CModelPCUxi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getBeforeDirectNum__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setAfterDirect__6CModelPCUxi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", clearAfterDirect__6CModelPCUxi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getAfterDirectNum__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getOTZ__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setOTZ__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getColorScale__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getAlphaScale__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", dispWireframe__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isDispWireframe__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setWorldScreenMatrix__6CModelPC7FMATRIX);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", defaultShaderOption__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setShaderOption__6CModelUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", resetShaderOption__6CModelUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", changeLayerExt__6CModelii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isClampRepeat__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setClampRepeat__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isShadowMap__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setShadowMap__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isAutoEffSemiSort__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setAutoEffSemiSort__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isRootZSort__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setRootZSort__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isShadow__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isMap__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", createSemiModel__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isManagedSemiModel__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", manageSemiModel__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isAvailableSemiModel__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", availableSemiModel__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setDispFlag__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isOTZFlag__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setOTZFlag__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", updateOTZ__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isUVScroll__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setUVScroll__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isWBBox__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setWBBox__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isSemiSortFlag__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setSemiSortFlag__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _$_8CModelBG);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", __8CModelBGUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", disp__8CModelBG);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setLayerFG__8CModelBGUc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _$_5CBBox);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", __5CBBox);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getPacketSize__C5CBBox);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _$_12CShadowModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", changeLayerExt__12CShadowModelii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", calcZ__12CShadowModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _$_15CShadowMapModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", changeLayerExt__15CShadowMapModelii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", calcZ__15CShadowMapModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _GLOBAL_$I$c__KHCOM_MainApp_System_Graphic_Model.cppI3aaaa);
