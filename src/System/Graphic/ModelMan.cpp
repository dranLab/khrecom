#define MODELDATA_H_TEXINFO
#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h"
#define MODELDATA_H_PATH "c:/KHCOM/MainApp/System/Graphic/modeldata.h"
#define MODEL_H_PATH "c:/KHCOM/MainApp/System/Graphic/Model.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "System/Graphic/DispList.h"
#include "System/Graphic/ModelMan.h"
#include "System/Graphic/WorldEnv.h"
#include "System/HeapMan.h"
#include "System/Debug.h"
#include "include_asm.h"

inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

// PARTIAL: from System/Coord.h and the sce libraries
void MulMatrix(FMATRIX& m0, const FMATRIX& m1, const FMATRIX& m2);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
extern "C" void sceVu0InversMatrix(FMATRIX& m0, const FMATRIX& m1);



#define TEXMAX 16

#include "System/RsrcDB.h"




#include "System/Graphic/PackMan.h"




















class CShadowModelMan { /* System/Graphic/ShadowModelMan.h */
public:
    static INT m_iStatus;
    static void initialize();
    static void finalize();
    static void updateMatrix();
};

class CDispListSemiSingle : public CDispListSemiBase {
public:
    CDispListSemiSingle() {
        setupOTtagTbl(1);
        clear(FALSE);
    }
    virtual void insert(CDrawable& inPrim, FIX4 inZ) {
        _insertTail(*getOTtagTbl(), inPrim);
        _setZ(inPrim, inZ);
    }
};

class CDispListSemiSingleSort : public CDispListSemiBase {
protected:
    UINT m_nEntryCount; /* 0x20 */

public:
    CDispListSemiSingleSort() {
        setupOTtagTbl(1);
        clear(FALSE);
    }
    virtual void insert(CDrawable& inPrim, FIX4 inZ) {
        _insertTail(*getOTtagTbl(), inPrim);
        if (inZ > 0xffff280) inZ = 0xffff280;
        _setZ(inPrim, inZ);
        m_nEntryCount++;
    }
    INT draw(SEMI_PACKET_LIST* pList);
    virtual void clear(BOOL bUncached) {
        CDispListBase::clear(bUncached);
        m_nEntryCount = 0;
    }
    virtual UINT getEntryCount() const { return m_nEntryCount; }
};

class CDispListSemiZSingleSort : public CDispListSemiSingleSort {
public:
    CDispListSemiZSingleSort() {}
    virtual void insert(CDrawable& inPrim, FIX4 inZ) {
        _insertTail(*getOTtagTbl(), inPrim);
        if (inZ <= 0xffff7ff) inZ = 0xffff800;
        if (inZ > 0xffff9ff) inZ = 0xffff9ff;
        _setZ(inPrim, inZ);
        m_nEntryCount++;
    }
};

class CDispListSemiLastSingleSort : public CDispListSemiSingleSort {
public:
    CDispListSemiLastSingleSort() {}
    virtual void insert(CDrawable& inPrim, FIX4 inZ) {
        _insertTail(*getOTtagTbl(), inPrim);
        if (inZ <= 0xffffbff) inZ = 0xffffc00;
        if (inZ > 0xffffdff) inZ = 0xffffdff;
        _setZ(inPrim, inZ);
        m_nEntryCount++;
    }
};

FVECTOR g_tmpView[8];

namespace {
ULONGLONG s_ToonTex0;
ULONGLONG s_SpecTex0;
u_long s_TexCache[16];
unsigned int s_TexSizeCache[16];
CDispListBase* s_pDispListTbl[9];
SEMI_PACKET_LIST* s_pSemiList;
INT s_nSemiListNum;
G_LAYER s_sLayerInfo;
INT s_iModelCount;
INT s_iDrawCount;
int s_iTexSendCount[2];
MODEL_SEND_CACHE s_sModelCache __attribute__((aligned(64)));
}

#line 66
void CModelMan::initialize() {
    CDispListBase** ppList = s_pDispListTbl;

    ppList[2] = new (com_malloc_d(sizeof(CDispListBG), __FILE__ "(69)", RET_ADDR())) CDispListBG;






    ppList[1] = new (com_malloc_d(sizeof(CDispListSemiSingle), __FILE__ "(76)", RET_ADDR())) CDispListSemiSingle;
    ppList[7] = new (com_malloc_d(sizeof(CDispListShadowDraw), __FILE__ "(77)", RET_ADDR())) CDispListShadowDraw;
    ppList[8] = new (com_malloc_d(sizeof(CDispListShadowMap), __FILE__ "(78)", RET_ADDR())) CDispListShadowMap;
    ppList[0] = new (com_malloc_d(sizeof(CDispListSemi), __FILE__ "(79)", RET_ADDR())) CDispListSemi;

    ppList[4] = new (com_malloc_d(sizeof(CDispListSemiSingleSort), __FILE__ "(81)", RET_ADDR())) CDispListSemiSingleSort;
    ppList[5] = new (com_malloc_d(sizeof(CDispListSemiZSingleSort), __FILE__ "(82)", RET_ADDR())) CDispListSemiZSingleSort;
    ppList[6] = new (com_malloc_d(sizeof(CDispListSemiLastSingleSort), __FILE__ "(83)", RET_ADDR())) CDispListSemiLastSingleSort;

    ppList[3] = new (com_malloc_d(sizeof(CDispListFG), __FILE__ "(85)", RET_ADDR())) CDispListFG;
    s_pSemiList = NULL;
    s_nSemiListNum = 0;
    s_sLayerInfo._nInfo = 0;
    s_iModelCount = 0;
    changePrimMode(FALSE);
    CShadowModelMan::initialize();
    s_iDrawCount = 0;
    s_iTexSendCount[0] = 0;
    s_iTexSendCount[1] = 0;
}

void CModelMan::finalize() {
    for (INT i = 0; i < 9; i++) {
        if (s_pDispListTbl[i]) delete s_pDispListTbl[i];
        s_pDispListTbl[i] = NULL;
    }
    if (s_pSemiList) com_free(s_pSemiList);
    s_pSemiList = NULL;
    CShadowModelMan::finalize();
}

static inline void _vu0SetDisp(FVECTOR& v0, FVECTOR& v1) {
    asm volatile("lqc2 $vf1, 0x0(%0)\n"
                 "lqc2 $vf2, 0x0(%1)\n"
                 "viaddi $vi1, $vi0, 1\n"
                 "vsqi.xyzw $vf1, ($vi1++)\n"
                 "vsqi.xyzw $vf2, ($vi1++)\n"
                 :
                 : "r"(&v0), "r"(&v1));
}

/* reset: near-match. The original reloads the 0.0f arguments (and stack addresses) after each call to FVECTOR::set
 * (called out of line here); this build keeps them in callee-saved registers across the call. */
#if 0
void CModelMan::reset() {
    TexInfo* pInfo = CRsrcDB::getRsrcTex("errer", NAME_CASE_SENSITIVE);
    if (pInfo) s_ToonTex0 = pInfo->getTPage();
    pInfo = CRsrcDB::getRsrcTex("speclar", NAME_CASE_SENSITIVE);
    if (pInfo) s_SpecTex0 = pInfo->getTPage();
    FVECTOR vMax;
    FVECTOR vMin;
    vMax.set(4096.0f, 4096.0f, 0.0f, 65535.0f);
    vMin.set(0.0f, 0.0f, 0.0f, 0.0f);
    asm volatile("lqc2 $vf1, 0x0(%0)\n"
                 "lqc2 $vf2, 0x0(%1)\n"
                 "viaddi $vi1, $vi0, 1\n"
                 "vsqi.xyzw $vf1, ($vi1++)\n"
                 "vsqi.xyzw $vf2, ($vi1++)\n"
                 :
                 : "r"(&vMax), "r"(&vMin));
}
#endif
/* placeholder for reset's .rodata (its resource names): code in .sdata, which the build strips */
__attribute__((section(".sdata"))) void _resetPlaceholder() {
    CRsrcDB::getRsrcTex("errer", NAME_CASE_SENSITIVE);
    CRsrcDB::getRsrcTex("speclar", NAME_CASE_SENSITIVE);
}
/* a reference to FVECTOR::set makes gcc output its weak copy (that the included assembly calls); stripped as well */
__attribute__((section(".sdata"))) void (FVECTOR::*_pSetRef)(FLOAT, FLOAT, FLOAT, FLOAT) = &FVECTOR::set;
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/ModelMan", reset__9CModelMan);

static inline void _unitMatrix(FMATRIX& m) {
    asm volatile("vmr32.xyzw $vf1, $vf0\n"
                 "sqc2 $vf0, 0x30(%0)\n"
                 "vmr32.xyzw $vf2, $vf1\n"
                 "sqc2 $vf1, 0x20(%0)\n"
                 "vmr32.xyzw $vf3, $vf2\n"
                 "sqc2 $vf2, 0x10(%0)\n"
                 "sqc2 $vf3, 0x0(%0)\n"
                 :
                 : "r"(&m)
                 : "memory");
}

static inline FMATRIX _mulMatrix(const FMATRIX& m0, const FMATRIX& m1) {
    FMATRIX b;
    MulMatrix(b, m0, m1);
    return b;
}

static inline FVECTOR _applyMatrix(const FMATRIX& m, const FVECTOR& v) {
    FVECTOR b;
    ApplyMatrix(b, m, v);
    return b;
}

/* setupDrawEnv: near-match. The original reloads the 0.0f arguments (and stack addresses) after each call to FVECTOR::set
 * (called out of line here); this build keeps them in callee-saved registers across the call. */
#if 0
void CModelMan::setupDrawEnv() {
    FMATRIX& matWS = CWorldEnv::getWorldScreenMatrix();
    CShadowModelMan::updateMatrix();
    clearTexCache();
    CGTexPrim::clearTexCache();
    CGTexPrim2D::clearTexCache();
    CModel::resetSendCtrl(-1);
    CModel::resetWorldScreenMatrix();
    CModel::resetMicroProgram();
    setupDrawMode(0);
    asm volatile("lqc2 $vf28, 0x0(%0)\n"
                 "lqc2 $vf29, 0x10(%0)\n"
                 "lqc2 $vf30, 0x20(%0)\n"
                 "lqc2 $vf31, 0x30(%0)\n"
                 :
                 : "r"(&matWS));
    {
        FVECTOR vFogParam;
        vFogParam.set(0.0f, CWorldEnv::getDispFar(), 0.0f, 0.0f);
        asm volatile("lqc2 $vf1, 0x0(%0)\n"
                     "viaddi $vi1, $vi0, 0\n"
                     "vsqi.xyzw $vf1, ($vi1++)\n"
                     :
                     : "r"(&vFogParam));
    }
    FMATRIX wv_i;
    FMATRIX vs_i;
    FMATRIX ws_i;
    FMATRIX& matWV = CWorldEnv::getWorldViewMatrix();
    FMATRIX& matVS = CWorldEnv::getViewScreenMatrix();
    sceVu0InversMatrix(wv_i, matWV);
    _unitMatrix(vs_i);
    vs_i.m[0][0] = 1.0f / matVS.m[0][0];
    vs_i.m[3][0] = -matVS.m[2][0] / matVS.m[0][0];
    vs_i.m[1][1] = 1.0f / matVS.m[1][1];
    vs_i.m[3][1] = -matVS.m[2][1] / matVS.m[1][1];
    vs_i.m[2][2] = 0.0f;
    vs_i.m[3][2] = 1.0f;
    vs_i.m[2][3] = 1.0f / matVS.m[3][2];
    vs_i.m[3][3] = -matVS.m[2][2] / matVS.m[3][2];
    ws_i = _mulMatrix(wv_i, vs_i);
    {
        FLOAT farZ = CWorldEnv::getDispFar();
        const FLOAT XDISPMAX = 2304.0f;
        const FLOAT XDISPMIN = 1792.0f;
        const FLOAT YDISPMAX = 2240.0f;
        const FLOAT YDISPMIN = 1856.0f;
        const FLOAT ZBUFMAX = 16777000.0f;
        FVECTOR v[8];
        v[0].x = XDISPMAX;
        v[0].y = YDISPMAX;
        v[0].z = ZBUFMAX;
        v[0].w = 1.0f;
        v[1].x = XDISPMIN;
        v[1].y = YDISPMAX;
        v[1].z = ZBUFMAX;
        v[1].w = 1.0f;
        v[2].x = XDISPMAX;
        v[2].y = YDISPMIN;
        v[2].z = ZBUFMAX;
        v[2].w = 1.0f;
        v[3].x = XDISPMIN;
        v[3].y = YDISPMIN;
        v[3].z = ZBUFMAX;
        v[3].w = 1.0f;
        v[4].x = XDISPMAX * farZ;
        v[4].y = YDISPMAX * farZ;
        v[4].z = farZ;
        v[4].w = farZ;
        v[5].x = XDISPMIN * farZ;
        v[5].y = YDISPMAX * farZ;
        v[5].z = farZ;
        v[5].w = farZ;
        v[6].x = XDISPMAX * farZ;
        v[6].y = YDISPMIN * farZ;
        v[6].z = farZ;
        v[6].w = farZ;
        v[7].x = XDISPMIN * farZ;
        v[7].y = YDISPMIN * farZ;
        v[7].z = farZ;
        v[7].w = farZ;
        for (INT i = 0; i < 8; i++) {
            g_tmpView[i] = _applyMatrix(ws_i, v[i]);
        }
    }
    asm volatile("lqc2 $vf1, 0x0(%0)\n"
                 "lqc2 $vf2, 0x10(%0)\n"
                 "lqc2 $vf3, 0x20(%0)\n"
                 "lqc2 $vf4, 0x30(%0)\n"
                 "lqc2 $vf5, 0x40(%0)\n"
                 "lqc2 $vf6, 0x50(%0)\n"
                 "lqc2 $vf7, 0x60(%0)\n"
                 "lqc2 $vf8, 0x70(%0)\n"
                 "viaddi $vi1, $vi0, 4\n"
                 "vsqi.xyzw $vf1, ($vi1++)\n"
                 "vsqi.xyzw $vf2, ($vi1++)\n"
                 "vsqi.xyzw $vf3, ($vi1++)\n"
                 "vsqi.xyzw $vf4, ($vi1++)\n"
                 "vsqi.xyzw $vf5, ($vi1++)\n"
                 "vsqi.xyzw $vf6, ($vi1++)\n"
                 "vsqi.xyzw $vf7, ($vi1++)\n"
                 "vsqi.xyzw $vf8, ($vi1++)\n"
                 :
                 : "r"(g_tmpView));
}
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/ModelMan", setupDrawEnv__9CModelMan);

/* setupDrawMode: near-match. The two halves of the first GIF tag get swapped registers ($v0/$v1). */
#if 0
void CModelMan::setupDrawMode(INT nInAlphaMode) {
    QWdata giftag;
    CPackMan::pkAddCode(0x10000000);
    CPackMan::pkOpenUpkCode(0xd, 0x6c, 4, 4);
    giftag.u64[1] = 0xeee6;
    giftag.u64[0] = 0x4000000000008001;
    CPackMan::pkAddUpkData128(giftag.u128);
    giftag.u64[0] = 0x1000000000000000;
    giftag.u64[1] = 0xe;
    CPackMan::pkAddUpkData128(giftag.u128);
    CPackMan::pkAddUpkData64(0x303ec00000008000);
    CPackMan::pkAddUpkData64(0x412);
    CPackMan::pkAddUpkData128(CWorldEnv::getFogParam()._q);
    CPackMan::pkAddUpkData64(s_SpecTex0);
    CPackMan::pkAddUpkData64(6);
    CPackMan::pkAddUpkData64((((u_long)nInAlphaMode << 6) | 0x1b) << 47 | 0x9000400000008000);
    CPackMan::pkAddUpkData64(0x412412412);
    CPackMan::pkCloseUpkCode();
}
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/ModelMan", setupDrawMode__9CModelMani);

void CModelMan::clearDispList() {
    for (INT i = 0; i < 9; i++) {
        CDispListBase* pList = s_pDispListTbl[i];
        if (pList) pList->clear(FALSE);
    }
}

#line 405
void CModelMan::add2DispList(CDrawable& inPrim, FIX4 inZ) {
    USHORT nID = inPrim.m_LayerInfo._nID & 0xf;
    if (nID < 9) {
        if (nID == 0) nID = (inPrim.m_iFlag & 1) << 2;
        CDispListBase* pList = s_pDispListTbl[nID];
        ASSERT(pList);
        pList->insert(inPrim, inZ);
    } else if (nID == 11) {
        CDispListBase* pList = s_pDispListTbl[3];
#line 418
        ASSERT(pList);
        pList->insert(inPrim, inZ);
    } else {
        if (!CPackMan::isOverFlow(inPrim.getPacketSize())) {
            CPackMan::pkCnt(0);
            CPackMan::pkOpenDirectCode(0);
            inPrim.makePkt();
            CPackMan::pkCloseDirectCode();
        }
    }
}

CDispListBase* CModelMan::getDispList(INT nDid) {
    if (!((UINT)nDid < 9)) return NULL;
    return s_pDispListTbl[nDid];
}

void CModelMan::drawDispList() {
    if (CPackMan::getEnableDraw() & 2) {
        CDispListBase* pList;
        if ((pList = s_pDispListTbl[1])) pList->draw(-1, -1);
        if (!((CShadowModelMan::m_iStatus >> 2) & 1)) {
            CDispListBase* pListSdw = s_pDispListTbl[8];
            if (pListSdw && pListSdw->getEntryCount()) {
                CDispListBase* pListSmp = s_pDispListTbl[7];
                if (pListSmp && pListSmp->getEntryCount()) pListSmp->draw(-1, -1);
            }
        }
        if ((pList = s_pDispListTbl[0])) pList->draw(-1, -1);
        if ((pList = s_pDispListTbl[2]) && pList->getEntryCount()) pList->draw(-1, -1);
    }
}

#line 600
void CModelMan::drawDispListSemi() {
    SEMI_PACKET_LIST*& pSemiList = s_pSemiList;
    INT& nSemiListNum = s_nSemiListNum;
    INT nSemiListSemiNum;
    INT nSemiListSemiZNum;
    INT nSemiListSemiLastNum;
    if (pSemiList) com_free(pSemiList);
    pSemiList = NULL;
    nSemiListSemiNum = 0;
    nSemiListNum = 0;
    nSemiListSemiZNum = 0;
    nSemiListSemiLastNum = 0;
    if (CPackMan::getEnableDraw() & 2) {
        CDispListBase* pList;
        if ((pList = s_pDispListTbl[4])) nSemiListSemiNum = pList->getEntryCount();
        if ((pList = s_pDispListTbl[5])) nSemiListSemiZNum = pList->getEntryCount();
        if ((pList = s_pDispListTbl[6])) nSemiListSemiLastNum = pList->getEntryCount();
        nSemiListNum = nSemiListSemiNum + nSemiListSemiZNum + nSemiListSemiLastNum;
        if (nSemiListNum > 0) {
            INT i = 0;
            CPackMan::initSemiPacket();
#line 622
            pSemiList = (SEMI_PACKET_LIST*)com_malloc_d(nSemiListNum * sizeof(SEMI_PACKET_LIST), __FILE__ "(622)", RET_ADDR());
            ASSERT(pSemiList);
            if (nSemiListSemiNum > 0) {
                i = ((CDispListSemiSingleSort*)s_pDispListTbl[4])->draw(pSemiList);
            }
            if (nSemiListSemiZNum > 0) {
                i += ((CDispListSemiSingleSort*)s_pDispListTbl[5])->draw(&pSemiList[i]);
            }
            if (nSemiListSemiLastNum > 0) {
                i += ((CDispListSemiSingleSort*)s_pDispListTbl[6])->draw(&pSemiList[i]);
            }
            if (i < nSemiListNum) nSemiListNum = i;
            CPackMan::terminateSemiPacket();
        }
        if (dbgCheckUser((USER)6) && dbgButtonDown((USER)6, 0x80, 1)) {
            TRACE("SEMI : %d (%d/%d/%d)\n", nSemiListNum, nSemiListSemiNum, nSemiListSemiZNum, nSemiListSemiLastNum);
            if (pSemiList) com_free(pSemiList);
            pSemiList = NULL;
            nSemiListNum = 0;
        }
    }
}

SEMI_PACKET_LIST* CModelMan::getSemiPktList() { return s_pSemiList; }

INT CModelMan::getSemiPktListNum() { return s_nSemiListNum; }

void CModelMan::drawDispListFG() {
    CPackMan::initFGPacket();
    CPackMan::setFGPacketMaxNorml();
    CPackMan::changeLayer(2);
    clearTexCache();
    CGTexPrim::clearTexCache();
    CGTexPrim2D::clearTexCache();
    CModel::resetWorldScreenMatrix();
    CModel::resetMicroProgram();
    setupDrawMode(0);
    CDispListBase* pList = s_pDispListTbl[3];
    BOOL bUseWarning;
    if (pList) {
        OTtag* pTbl = pList->getOTtagTbl();
        UINT n = 25;
        bUseWarning = FALSE;
        if (pTbl && n < (UINT)pList->getOTtagTblSize()) bUseWarning = pTbl[n].m_pHead != NULL;
    } else {
        bUseWarning = FALSE;
    }
    if (!bUseWarning) CPackMan::extFGPacketMaxForFlex();
    pList = s_pDispListTbl[3];
    if (pList) {
        if (CPackMan::getEnableDraw() & 8) pList->draw(0, 25);
        if (bUseWarning) CPackMan::extFGPacketMaxForFlex();
        pList->draw(25, -1);
    }
    CPackMan::setFGPacketMaxNorml();
    CPackMan::terminateFGPacket();
}

void CModelMan::clearTexCache() {
    s_sModelCache.pObject = NULL;
    s_sLayerInfo._nID = 16;
    for (INT i = 15; i >= 0; i--) {
        s_TexSizeCache[i] = 0;
    }
}

#line 878
BOOL CModelMan::updateTexCache(UINT nInIndex, const TexInfoStub& inTex) {

    ASSERT(TEXMAX > nInIndex);
    ULONGLONG& cacheTex = s_TexCache[nInIndex];
    UINT& cacheSize = s_TexSizeCache[nInIndex];
    ULONGLONG checkTex = inTex._nValue;
    UINT checkSize = inTex._pTexInfo->_TexInfo._nTransSize;
    if (checkSize == cacheSize && checkTex == cacheTex) return FALSE;
    INT nUseSize = checkSize;
    INT TexPosS = inTex._nTexPos;
    INT TexPosE = TexPosS + checkSize;
    for (INT i = nInIndex; i < TEXMAX; i++) {
        INT nCacheSize = s_TexSizeCache[i];
        if (nCacheSize > 0) {
            INT TexS = ((TexInfoStub*)&s_TexCache[i])->_nTexPos;
            INT TexE = TexS + nCacheSize;
            if ((TexS <= TexPosS && TexPosS < TexE) || (TexS < TexPosE && TexPosE <= TexE) ||
                (TexPosS <= TexS && TexS < TexPosE) || (TexPosS < TexE && TexE <= TexPosE)) {
                nUseSize -= nCacheSize;
                s_TexSizeCache[i] = 0;
                if (nUseSize <= 0) break;
            }
        }
    }
    cacheTex = checkTex;
    cacheSize = checkSize;
    return TRUE;
}

USHORT CModelMan::getPacketSize(USHORT nInBoneCount, USHORT nInTexCount, USHORT nInTransTexCount,
                                const MODELDATA* pModelData) {
    USHORT nSize;
    USHORT value;
    if (nInTexCount) {
        value = nInTransTexCount * 0x300 + nInTexCount * 16 + 8;
    } else {
        value = 0x18;
    }
    nSize = value + nInBoneCount * 64;
    nSize = nSize + (nInBoneCount / 63 + 1) * 16;
    if (pModelData && pModelData->enablePointLight()) {
        nSize = nInBoneCount * 0xa0 + nSize + (nInBoneCount / 25 + 1) * 16;
    }
    nSize = nSize + 0x700;
    return (nSize + 15) >> 4;
}

INT CModelMan::getModelCount() { return s_iModelCount; }

void CModelMan::incModelCount() { s_iModelCount++; }

void CModelMan::decModelCount() { s_iModelCount--; }

void CModelMan::changePrimMode(BOOL bOnlyPrim) { CGTexPrim::m_nTmpTexArea = !bOnlyPrim; }

G_LAYER CModelMan::getLayerInfo() { return s_sLayerInfo; }

void CModelMan::setLayerInfo(G_LAYER sLayerInfo) { s_sLayerInfo = sLayerInfo; }

INT CModelMan::getDrawCount() { return s_iDrawCount; }

void CModelMan::incDrawCount() { s_iDrawCount++; }

void CModelMan::clearDrawCount() { s_iDrawCount = 0; }

INT CModelMan::getTexSendCount(INT i) { return s_iTexSendCount[i]; }

void CModelMan::incTexSendCount(INT i) { s_iTexSendCount[i]++; }

void CModelMan::clearTexSendCount(INT i) { s_iTexSendCount[i] = 0; }

MODEL_SEND_CACHE& CModelMan::getModelCacheRef() { return s_sModelCache; }

void TexInfoStub::init(const TexInfo* pInInfo, SHORT nInTexPos, SHORT nInClutPos) {
    _pTexInfo = (TexInfo*)pInInfo;
    _nTexPos = nInTexPos;
    _nClutPos = nInClutPos;
    if (pInInfo) {
        if (pInInfo->isFixTex()) _nTexPos = pInInfo->_TexInfo._nDistPos;
        if (pInInfo->isFixClut()) _nClutPos = pInInfo->_ClutInfo._nDistPos;
    }
}

void TexInfoStub::update(SHORT nInTexPos, SHORT nInClutPos) {
    if (!isFixTex()) _nTexPos = nInTexPos;
    if (!isFixClut()) _nClutPos = nInClutPos;
}


// SOURCE DATA: .rodata .bss
