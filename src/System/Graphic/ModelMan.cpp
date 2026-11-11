
#define MODELDATA_H_TEXINFO /* MATCH: TexInfoStub (the original has its assert strings) */
#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define MODELDATA_H_PATH "c:/KHCOM/MainApp/System/Graphic/modeldata.h" /* MATCH: this unit names modeldata.h so (header default "System/Graphic/modeldata.h") */
#define MODEL_H_PATH "c:/KHCOM/MainApp/System/Graphic/Model.h" /* MATCH: this unit names Model.h so (header default "System/Graphic/Model.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "System/Graphic/DispList.h"
#include "System/Graphic/ModelMan.h"
#include "System/Graphic/WorldEnv.h"
#include "System/HeapMan.h"
#include "System/Debug.h"




#define ASSERT(e) KH_DEBUG(if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0))

#include "System/Coord.h"
// from the sce libraries



#define TEXMAX 16

#include "System/RsrcDB.h"




#include "System/Graphic/PackMan.h"




















#include "System/Graphic/ShadowModel.h"







class CDispListSemiSingle : public CDispListSemiBase {
public:
    CDispListSemiSingle() {
        setupOTtagTbl(1);
        clear(FALSE);
    }
    virtual void insert(CDrawable& inPrim, FIX4 inZ) {
        getOTtagTbl()->addTail(&inPrim);
        inPrim.setZ(inZ);
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
        getOTtagTbl()->addTail(&inPrim);
        if (inZ > 0xffff280) inZ = 0xffff280;
        inPrim.setZ(inZ);
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
        getOTtagTbl()->addTail(&inPrim);
        if (inZ <= 0xffff7ff) inZ = 0xffff800;
        if (inZ > 0xffff9ff) inZ = 0xffff9ff;
        inPrim.setZ(inZ);
        m_nEntryCount++;
    }
};

class CDispListSemiLastSingleSort : public CDispListSemiSingleSort {
public:
    CDispListSemiLastSingleSort() {}
    virtual void insert(CDrawable& inPrim, FIX4 inZ) {
        getOTtagTbl()->addTail(&inPrim);
        if (inZ <= 0xffffbff) inZ = 0xffffc00;
        if (inZ > 0xffffdff) inZ = 0xffffdff;
        inPrim.setZ(inZ);
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

    ppList[2] = new KH_AT("(69)") CDispListBG;






    ppList[1] = new KH_AT("(76)") CDispListSemiSingle;
    ppList[7] = new KH_AT("(77)") CDispListShadowDraw;
    ppList[8] = new KH_AT("(78)") CDispListShadowMap;
    ppList[0] = new KH_AT("(79)") CDispListSemi;

    ppList[4] = new KH_AT("(81)") CDispListSemiSingleSort;
    ppList[5] = new KH_AT("(82)") CDispListSemiZSingleSort;
    ppList[6] = new KH_AT("(83)") CDispListSemiLastSingleSort;

    ppList[3] = new KH_AT("(85)") CDispListFG;
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











void CModelMan::reset() {
    TexInfo* pInfo = CRsrcDB::getRsrcTex("errer", NAME_CASE_SENSITIVE);
    if (pInfo) s_ToonTex0 = pInfo->getTPage();
    pInfo = CRsrcDB::getRsrcTex("speclar", NAME_CASE_SENSITIVE);
    if (pInfo) s_SpecTex0 = pInfo->getTPage();
    vu0SetClipRange();










}














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
    vu0LoadMatrixVF28(&matWS);





    vu0SetFogParam(CWorldEnv::getDispFar());








    FMATRIX wv_i;
    FMATRIX vs_i;
    FMATRIX ws_i;
    FMATRIX& matWV = CWorldEnv::getWorldViewMatrix();
    FMATRIX& matVS = CWorldEnv::getViewScreenMatrix();
    InverseMatrix(wv_i, matWV);
    vu0UnitMatrix(vs_i);
    vs_i.m[0][0] = 1.0f / matVS.m[0][0];
    vs_i.m[3][0] = -matVS.m[2][0] / matVS.m[0][0];
    vs_i.m[1][1] = 1.0f / matVS.m[1][1];
    vs_i.m[3][1] = -matVS.m[2][1] / matVS.m[1][1];
    vs_i.m[2][2] = 0.0f;
    vs_i.m[3][2] = 1.0f;
    vs_i.m[2][3] = 1.0f / matVS.m[3][2];
    vs_i.m[3][3] = -matVS.m[2][2] / matVS.m[3][2];
    ws_i = MulMatrix(wv_i, vs_i);
    FLOAT farZ = CWorldEnv::getDispFar();
    FLOAT XDISPMAX = 2304.0f;
    FLOAT XDISPMIN = 1792.0f;
    FLOAT YDISPMAX = 2240.0f;
    FLOAT YDISPMIN = 1856.0f;
    FLOAT ZBUFMAX = 16777000.0f;
    FVECTOR v[8] = {{XDISPMAX, YDISPMAX, ZBUFMAX, 1.0f}, {XDISPMIN, YDISPMAX, ZBUFMAX, 1.0f}, {XDISPMAX, YDISPMIN, ZBUFMAX, 1.0f}, {XDISPMIN, YDISPMIN, ZBUFMAX, 1.0f}, {XDISPMAX * farZ, YDISPMAX * farZ, farZ, farZ}, {XDISPMIN * farZ, YDISPMAX * farZ, farZ, farZ}, {XDISPMAX * farZ, YDISPMIN * farZ, farZ, farZ}, {XDISPMIN * farZ, YDISPMIN * farZ, farZ, farZ}};
    for (int i = 0; i < 8; i++) {
        g_tmpView[i] = ApplyMatrix(ws_i, v[i]);
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

void CModelMan::setupDrawMode(INT nInAlphaMode) {
    QWdata giftag;
    CPackMan::pkAddCode(0x10000000);
    CPackMan::pkOpenUpkCode(0xd, 0x6c, 4, 4);
    giftag.u64[0] = 0x4000000000008001;
    giftag.u64[1] = 0xeee6;
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
        if (CShadowModelMan::isDraw()) {
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
            pSemiList = new KH_AT("(622)") SEMI_PACKET_LIST[nSemiListNum];
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
        if (pTbl && n < (UINT)pList->getOTtagTblSize()) bUseWarning = pTbl[n]._pHeadPrim != NULL;
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

void CModelMan::changePrimMode(BOOL bOnlyPrim) { CGTexPrim::setTmpTexAreaForAllPrims(!bOnlyPrim); }

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
