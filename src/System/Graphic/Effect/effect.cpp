#define EFFECT_CPP
#define EFFECT_H_PATH "c:/KHCOM/MainApp/System/Graphic/Effect/effect.h"
#include <string.h>

#include "System/Graphic/Effect/effect.h"
#include "System/HeapMan.h"
#include "System/Coord.h"
#include "System/TaskMan.h"
#include "System/Graphic/Effect/effect_inl.h"
#include "System/Graphic/LightUnit.h"
#include "System/Sound/SoundMan.h"
#include "System/Debug.h"


#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new(size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}
inline void* operator new[](size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}

struct G_RGBA { /* System/Graphic/Gprim.h */
    UBYTE r;
    UBYTE g;
    UBYTE b;
    UBYTE a;
};

struct _DPX_HEADER {
    u_short DpxVer;
    u_short Head;
    int Reserve1;
    int Reserve2;
    int ParNum;
};

struct _DPX_PAR {
    u_long128* Dpd;
    u_int DpdID;
    u_int EffectNo;
    int Reserve1;
    u_int CategoryFlag1;
    u_int CategoryFlag2;
    u_long CategoryFlag3;
};

typedef struct {
    int num;
    int offset[1];
} EFFECT_DPD_OFFSETS;

typedef struct {
    u_char ver;
    u_char mcode;
    u_short head;
} EFFECT_DPD_HEAD;

typedef struct {
    EFFECT_DPD_HEAD sHead;
    EFFECT_DPD_OFFSETS sData;
} EFFECT_DPD_DATA;

extern "C" {
void pppInitDpd(char* pchDpdData);
void pppMoveDpd(char* pchDpdData, int nOffset);
void pppFreeParticle(void* mng);
void pppDeleteMng(void* mng);
void* pppGetPartMng();
int pppStartPartMng(void* mng, u_long128* dpd, u_int dpd_id, int priority, float wait);
float* pppPartMngGetPos(void* mng);
float* pppPartMngGetScl(void* mng);
float* pppPartMngGetMatrix(void* mng);
float* pppPartMngGetCol(void* mng);
void pppPartMngSetSkipFrame(void* mng, int nFrame);
int pppPartMngGetTotalFrame(void* mng);
float pppPartMngGetNowFrame(void* mng);
BOOL pppPartMngIsEnd(void* mng);
int pppPartMngIsWaiting(void* mng);
int pppIsLoop(void* mng);
void pppLoopEnd(void* mng);
float* pppGetBbox(void* mng);
void pppSetReferencePos(void* list, int num);
void pppSetReferenceMatrix(void* list, int num);
void pppSetDrawFirst(void* mng, int priority);
void pppSetDrawLast(void* mng, int priority);
char* pppPrintProf(void* mng);
void sceVu0ClampVector(float* v0, float* v1, float min, float max);
int strncasecmp(const char* s1, const char* s2, size_t n);
}

void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void MulVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
BOOL ClipDispBBox(const FMATRIX& m, const FVECTOR* bbox);

static UINT s_nMagicCode = 0;

#include "include_asm.h"

void EFFECT_DATA_DPD::set(u_long128* data) {
    Data = data;
}

void EFFECT_DATA_DPD::Setup(u_long128* data) {
    pppInitDpd((char*)data);
}

void EFFECT_DATA_DPD::Shift(u_long128* to, u_long128* from) {
    pppMoveDpd((char*)to, (char*)to - (char*)from);
}

void EFFECT_DATA::set(u_long128* data) {
    Data = data;
}

BOOL EFFECT_DATA::no_search(u_int effect_no, u_long128*& dpd, u_int& dpd_id) const {
    _DPX_HEADER* dpx_header = (_DPX_HEADER*)Data;
    _DPX_PAR* dpx_par = (_DPX_PAR*)(dpx_header + 1);
    BOOL find = FALSE;
    for (int i = 0; i < dpx_header->ParNum; i++, dpx_par++) {
        if (dpx_par->EffectNo == effect_no) {
            dpd = dpx_par->Dpd;
            dpd_id = dpx_par->DpdID;
            find = TRUE;
            break;
        }
    }
    if (!find) {
        TRACE("EFFECT_DATA::no_search() error! invalid effect_no: %d\n", effect_no);
        return FALSE;
    }
    return TRUE;
}

#line 99
int EFFECT_DATA::idx_to_no(int idx) const {
    _DPX_HEADER* dpx_header = (_DPX_HEADER*)Data;
    ASSERT(0 <= idx);
    ASSERT(idx < dpx_header->ParNum);
    _DPX_PAR* dpx_par = (_DPX_PAR*)(dpx_header + 1);
    return dpx_par[idx].EffectNo;
}

int EFFECT_DATA::get_num() const {
    _DPX_HEADER* dpx_header = (_DPX_HEADER*)Data;
    return dpx_header->ParNum;
}

#line 124
__asm__(".section .sdata\n");
/* Placeholder for the INCLUDE_ASM'd Setup below (emits its strings in place). Near match: p and i swap s1/s2. */
static void __attribute__((section(".sdata"))) _Setup(u_long128* data, BOOL bDownVer) {
    u_char* p = (u_char*)data;
    _DPX_HEADER* dpx_header = (_DPX_HEADER*)p;
    int dpd_num;
    int i;
#line 127
    ASSERT(0 == (int)data % 16);
    TRACE_USER((USER)6, "EFFECT_DATA::Setup() -> DpxVer:%d\n", ((_DPX_HEADER*)data)->DpxVer);
    p += sizeof(_DPX_HEADER);
    TRACE_USER((USER)6, "EFFECT_DATA::Setup() -> ParNum:%d\n", ((_DPX_HEADER*)data)->ParNum);
#line 133
    ASSERT(dpx_header->DpxVer >= 130);
    ((_DPX_HEADER*)data)->Head = 0x5844;
    for (i = 0; i < dpx_header->ParNum; i++, p += sizeof(_DPX_PAR)) {
        if (((_DPX_PAR*)p)->Dpd) ((_DPX_PAR*)p)->Dpd = (u_long128*)((u_char*)((_DPX_PAR*)p)->Dpd + (int)data);
    }
    dpd_num = *(int*)p;
    p += sizeof(int);
    TRACE_USER((USER)6, "EFFECT_DATA::Setup() -> DpdNum:%d\n", dpd_num);
    for (int j = 0; j < dpd_num; j++) {
        EFFECT_DPD_DATA* dpd = (EFFECT_DPD_DATA*)(*(int*)p + (int)data);
        pppInitDpd((char*)dpd);
        if (bDownVer) dpd->sHead.ver = 140;
        dpd->sHead.mcode = s_nMagicCode;
        dpd->sHead.head = 0x5044;
        p += sizeof(int);
        s_nMagicCode++;
        TRACE_USER((USER)6, "EFFECT_DATA::Setup() -> pppInitDpd() -> DpdVer:%08x\n", *(int*)dpd);
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Effect/effect", Setup__11EFFECT_DATAPUxi);

#line 181
void EFFECT_DATA::Shift(u_long128* to, u_long128* from) {
    int offset = (u_char*)to - (u_char*)from;
    u_char* p = (u_char*)to;
    _DPX_HEADER* dpx_header = (_DPX_HEADER*)p;
    int dpd_num;
    int i;
#line 185
    ASSERT(0 == (int)from % 16);
    p += sizeof(_DPX_HEADER);
    for (i = 0; i < dpx_header->ParNum; i++, p += sizeof(_DPX_PAR)) {
        if (((_DPX_PAR*)p)->Dpd) ((_DPX_PAR*)p)->Dpd = (u_long128*)((u_char*)((_DPX_PAR*)p)->Dpd + offset);
    }
    dpd_num = *(int*)p;
    p += sizeof(int);
    for (i = 0; i < dpd_num; i++) {
        pppMoveDpd((char*)(*(int*)p + (int)to), offset);
        p += sizeof(int);
    }
}

#line 216
void EFFECT_SE::check() const {
    ASSERT(this);
    if (!(!(this->sHead.nId != (0x00455345)))) AssertApp(__FILE__, __LINE__, "! (this->sHead.nId != (0x00455345))", 0);
    if (!(!(this->sHead.nNum <= 0))) AssertApp(__FILE__, __LINE__, "! (this->sHead.nNum <= 0)", 0);
}

const EFFECT_SE_DATA* EFFECT_SE::getData(INT nNo) const {
    check();
    const EFFECT_SE_DATA* pData = NULL;
    INT nNum = sHead.nNum;
    for (INT i = 0; i < nNum; i++) {
        if (sData[i].nENo == nNo) {
            pData = &sData[i];
            break;
        }
    }
    return pData;
}

INT EFFECT_SE::getSeId(INT nNo) const {
    const EFFECT_SE_DATA* pData = getData(nNo);
    if (pData) return pData->nFile * 1000 + pData->nNo;
    return 0;
}

INT EFFECT_SE::getSeFade(INT nNo) const {
    INT nFadeTime = 30;
    const EFFECT_SE_DATA* pData = getData(nNo);
    if (pData && pData->nFadeTime) nFadeTime = pData->nFadeTime;
    return nFadeTime;
}

INT EFFECT_SE::getNum() const {
    check();
    return sHead.nNum;
}

INT EFFECT_SE::getVer() const {
    check();
    return sHead.nVer;
}

INT EFFECT_SE::getDataSize() const {
    check();
    return sizeof(EFFECT_SE_HEAD) + getNum() * sizeof(EFFECT_SE_DATA);
}

CEffect::CEffect() {
    m_hHandle = 0;
    m_pCrds = NULL;
    m_pVecLst = NULL;
    m_eType = (ObjType::eObjType)0;
    m_iFlag = 0;
    m_pData = NULL;
    m_pPartMng = NULL;
    m_fFade = 1.0f;
    m_fFadeAdd = 0.0f;
    m_hSe = 0;
    m_nSeID = 0;
    m_nSeFade = 30;
    m_nMCode = 0;
    m_nEffNo = 0;
    m_pNLight = NULL;
    m_nZOffset = 0;
    m_nEntZOfs = 1;
    m_hEnt = 0;
    m_vScale.setAll(1.0f);
    m_vColor.setAll(1.0f);
    *(u_int*)m_pEffName = 0x4C4C554E; /* "NULL" */
    m_pEffName[4] = 0;
    m_pEffName[19] = 0;
}

CEffect::~CEffect() {
    _dispose();
    delete m_pCrds;
    m_pCrds = NULL;
    if (m_pVecLst) delete m_pVecLst;
    m_pVecLst = NULL;
    delete m_pNLight;
    m_pNLight = NULL;
}

void CEffect::_dispose() {
    if (m_pPartMng) {
        pppFreeParticle(m_pPartMng);
        pppDeleteMng(m_pPartMng);
        m_pPartMng = NULL;
        detachCoord();
        _fadeoutSE(0.0f);
    }
}

void CEffect::_playSE(const EFFECT_SE* pSeData, UINT nNo) {
    if (pSeData) {
        m_nSeID = pSeData->getSeId(nNo);
        m_nSeFade = pSeData->getSeFade(nNo);
        _updateSE();
    }
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Effect/effect", _updateSE__7CEffect);
/* Near match: "li v0,-1" and "lw s0,36(s1)" swap in the first block. */
#if 0
void CEffect::_updateSE() {
    if (!m_hSe && m_nSeID > 0 && !isWaiting()) {
        INT nVol = -1;
        INT nPan = CSound::getPanAndVol(Crd(0).getWorldPos(), nVol);
        m_hSe = CSound::playSE(m_nSeID, nVol, nPan);
    }
    if (m_hSe && m_nSeID > 0) {
        if (CSound::isSEPlaying(m_hSe, m_nSeID)) {
            INT nVol = -1;
            INT nPan = CSound::getPanAndVol(Crd(0).getWorldPos(), nVol);
            CSound::setSEPanAndVol(m_hSe, m_nSeID, nPan, nVol, 1);
        }
    }
}
#endif

void CEffect::_fadeoutSE(FLOAT fFrame) {
    if (m_hSe && m_nSeID > 0) {
        FLOAT fSeFadeMin = m_nSeFade;
        if (fFrame < fSeFadeMin) fFrame = fSeFadeMin;
        CSound::fadeoutSE(m_hSe, m_nSeID, (INT)fFrame);
        m_hSe = 0;
        m_nSeID = 0;
    }
}

int CEffect::_setData(const EFFECT_DATA& effect_data, u_int no) {
    UINT nID;
    if (!effect_data.no_search(no, *(u_long128**)&m_pData, nID)) return -1;
    return nID;
}

void strcpyEffName(char* pszDstName, const char* pszSrcName, INT nNum);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Effect/effect", strcpyEffName__FPcPCci);
/* Near match: the original keeps the destination in a second register (t1/a3). */
#if 0
static void strcpyEffName(char* pszDstName, const char* pszSrcName, INT nNum) {
    if (!pszDstName || !pszSrcName) return;
    for (INT i = 0; i < nNum; i++) {
        *pszDstName = pszSrcName[i];
        if (*pszDstName == '\0' || *pszDstName == '.') {
            *pszDstName = '\0';
            break;
        }
        pszDstName++;
    }
}
#endif

INT CEffect::_strcmpName(const char* pszEffName) {
    char szName[20];
    if (pszEffName) {
        strcpyEffName(szName, pszEffName, 19);
        return strncasecmp(szName, m_pEffName, 19);
    }
    return 1;
}

void CEffect::_setName(const char* pszEffName) {
    strcpyEffName(m_pEffName, pszEffName, 19);
}

BOOL CEffect::_checkData() {
    EFFECT_DPD_DATA* pData = (EFFECT_DPD_DATA*)m_pData;
    if (!pData) return FALSE;
    if (pData->sHead.ver >= 140 && pData->sHead.mcode == m_nMCode && pData->sHead.head == 0x5044) return TRUE;
    TRACE("CEffect::_checkData() Err!! %02x, %02x, %04x MCode:%02x\n", pData->sHead.ver, pData->sHead.mcode, pData->sHead.head, m_nMCode);
    return FALSE;
}

BOOL CEffect::start(const EFFECT_DATA& Data, UINT nNo, ObjType::eObjType eType, INT nCrdNum, FLOAT fWait) {
    BOOL result = FALSE;
    INT nID = _setData(Data, nNo);
    if (nID >= 0) {
        m_nMCode = ((EFFECT_DPD_DATA*)m_pData)->sHead.mcode;
        m_nEffNo = nNo;
        setType(eType);
#line 511
        m_pCrds = new (__FILE__ "(511)") CCoords(CCoords::_PP_SECONDARY);
        if (m_pCrds && m_pCrds->addCoord(nCrdNum, NULL) && m_pCrds->setSysFix(TRUE, CCoords::_PP_SECONDARY)) {
            m_pCrds->touch();
            if (nCrdNum > 1) {
#line 517
                m_pVecLst = new (__FILE__ "(517)") FVECTOR[nCrdNum];
                if (!m_pVecLst) goto end;
            }
            result = TRUE;
            m_pPartMng = pppGetPartMng();
            if (!m_pPartMng) {
                result = FALSE;
                TRACE("CEffect::start() error! create effect failed!\n");
            } else if (!pppStartPartMng(m_pPartMng, (u_long128*)m_pData, nID, 0, fWait)) {
                result = FALSE;
                TRACE("CEffect::start() error! DPD data is broken.\n");
            }
        }
    }
end:
    if (!result) _dispose();
    return result;
}

#line 558
FVECTOR& CEffect::_pos() {
    ASSERT(isValid());
#line 561
    ASSERT(isActive());
    return *(FVECTOR*)pppPartMngGetPos(m_pPartMng);
}

#line 570
FVECTOR& CEffect::_scale() {
    ASSERT(isValid());
#line 573
    ASSERT(isActive());
    return *(FVECTOR*)pppPartMngGetScl(m_pPartMng);
}

#line 596
FMATRIX& CEffect::_matrix() {
    ASSERT(isValid());
#line 599
    ASSERT(isActive());
    return *(FMATRIX*)pppPartMngGetMatrix(m_pPartMng);
}

#line 609
FVECTOR& CEffect::_color() {
    ASSERT(isValid());
#line 612
    ASSERT(isActive());
    return *(FVECTOR*)pppPartMngGetCol(m_pPartMng);
}

void CEffect::setLightCalc(BOOL bVal) {
    if (bVal) {
        m_iFlag |= 0x20;
        if (!m_pNLight) {
#line 629
            m_pNLight = new (__FILE__ "(629)") CLightUnit;
            ASSERT(m_pNLight);
            if (m_pCrds) m_pNLight->setAnchor(&Crd(0), 0);
        }
    } else {
        m_iFlag &= ~0x20;
    }
}

BOOL CEffect::isLightCalc() const {
    return m_iFlag & 0x20;
}

BOOL CEffect::calcLight(FVECTOR& outColor, const FVECTOR& inColor) {
    if (!isLightCalc()) return FALSE;
    if (!m_pNLight) return FALSE;
    m_pNLight->reCalc();
    CLightUnit* light = m_pNLight;
    FVECTOR vC = light->getAmbient();
    AddVector(vC, vC, light->getColor().v[0]);
    AddVector(vC, vC, light->getColor().v[1]);
    AddVector(vC, vC, light->getColor().v[2]);
    AddVector(vC, vC, light->getColor().v[3]);
    vC.w = 1.0f;
    MulVector(outColor, inColor, vC);
    sceVu0ClampVector(outColor.v, outColor.v, 0.0f, 2.0f);
    return TRUE;
}

#line 710
void CEffect::setIColor(const G_RGBA& color) {
    ASSERT(isValid());
    FVECTOR fcolor;
    for (int i = 0; i < 4; i++) {
        fcolor.v[i] = ((const UBYTE*)&color)[i] * (1.0f / 128.0f);
    }
    this->color() = fcolor;
}

#line 728
void CEffect::detachCoord() {
    ASSERT(isValid());
    if (m_pCrds) {
        setAttachFlag(FALSE);
        for (INT i = 0; i < getCoordNum(); i++) {
            CCoord& crd = Crd(i);
            if (crd.m_pParent) {
                crd.m_pParent = NULL;
                if (isAttachNoRot()) {
                    crd.getLocalMatrix().v[3] = crd.getWorldMatrix().v[3];
                } else {
                    crd.getLocalMatrix() = crd.getWorldMatrix();
                }
            }
        }
    }
}

#line 757
void CEffect::setSkipFrame(FLOAT fFrame) {
    ASSERT(isValid());
#line 760
    ASSERT(isActive());
    pppPartMngSetSkipFrame(m_pPartMng, (int)(fFrame * 0.5f));
}

#line 770
INT CEffect::getTotalFrame() {
    ASSERT(isValid());
#line 773
    ASSERT(isActive());
    return pppPartMngGetTotalFrame(m_pPartMng);
}

#line 783
FLOAT CEffect::getNowFrame() {
    ASSERT(isValid());
#line 786
    ASSERT(isActive());
    return pppPartMngGetNowFrame(m_pPartMng);
}

#line 796
BOOL CEffect::isEndFrame() {
    ASSERT(isValid());
#line 799
    ASSERT(isActive());
    return pppPartMngIsEnd(m_pPartMng);
}

#line 810
void CEffect::kill() {
    ASSERT(isValid());
    m_iFlag |= 0x10000;
    _dispose();
}

#line 822
void CEffect::loopEnd() {
    ASSERT(isValid());
#line 826
    ASSERT(isActive());
    pppLoopEnd(m_pPartMng);
    _fadeoutSE(0.0f);
}

#line 835
void CEffect::fadeout(FLOAT fFrame) {
    ASSERT(isValid());
    if (fFrame > 0.0f) {
        if (!(m_iFlag & 0x20000)) {
            m_iFlag |= 0x20000;
            m_fFadeAdd = -(m_fFade / fFrame);
            _fadeoutSE(fFrame);
        }
    } else {
        kill();
    }
}

BOOL CEffect::_fade(FLOAT fFR) {
    BOOL bRet = TRUE;
    if (m_fFadeAdd != 0.0f) {
        m_fFade += m_fFadeAdd * fFR;
        if (m_fFade < 0.0f) {
            m_fFade = 0.0f;
            m_fFadeAdd = 0.0f;
        }
        if (m_fFade > 0.0f) _color().w *= m_fFade;
    }
    if (m_iFlag & 0x20000) {
        if (m_fFade <= 0.0f) bRet = FALSE;
    }
    return bRet;
}

#line 879
void CEffect::setStop(BOOL b) {
    ASSERT(isValid());
    if (b) {
        m_iFlag |= 0x200000;
    } else {
        m_iFlag &= ~0x200000;
    }
}

#line 900
void CEffect::flipStop() {
    ASSERT(isValid());
    if (m_iFlag & 0x200000) {
        m_iFlag &= ~0x200000;
    } else {
        m_iFlag |= 0x200000;
    }
}

#line 974
void CEffect::setDrawing(BOOL b) {
    ASSERT(isValid());
    if (b) {
        m_iFlag &= ~0x800000;
    } else {
        m_iFlag |= 0x800000;
    }
}

#line 1005
void CEffect::setViewclip(BOOL b) {
    ASSERT(isValid());
    if (b) {
        m_iFlag &= ~0x1000000;
    } else {
        m_iFlag |= 0x1000000;
    }
}

BOOL CEffect::_viewclip(const FMATRIX& mLC, BOOL frustumClip) {
    BOOL result = FALSE;
    if (m_iFlag & 0x1000000) return FALSE;
    if (frustumClip) result = ClipDispBBox(mLC, (FVECTOR*)pppGetBbox(m_pPartMng));
    return result;
}

#line 1035
BOOL CEffect::isLoop() {
    ASSERT(isValid());
#line 1039
    ASSERT(isActive());
    return pppIsLoop(m_pPartMng);
}

#line 1073
BOOL CEffect::isWaiting() {
    ASSERT(isValid());
#line 1077
    ASSERT(isActive());
    return pppPartMngIsWaiting(m_pPartMng);
}

#line 1085
BOOL CEffect::isDelete() {
    ASSERT(isValid());
    return m_iFlag & 0x100000;
}

#line 1096
BOOL CEffect::isSuicide() {
    ASSERT(isValid());
    return !(m_iFlag & 1);
}

void CEffect::setReferenceCoordPos(INT nNum) {
    if (getCoordNum() < nNum) return;
    if (!m_pVecLst) return;
    for (INT i = 0; i < nNum; i++) {
        m_pVecLst[i] = Crd(i).getWorldPos();
    }
    setReferencePos(m_pVecLst, nNum);
}

void CEffect::setReferencePos(FVECTOR* list, int num) {
    ReferencePos.List = list;
    ReferencePos.Num = num;
    m_iFlag |= 0x40000;
}

void CEffect::setReferenceMatrix(FMATRIX* list, int num) {
    ReferenceMatrix.List = list;
    ReferenceMatrix.Num = num;
    m_iFlag |= 0x80000;
}

void CEffect::_setSpecialData() {
    if (m_iFlag & 0x40000) {
        pppSetReferencePos(ReferencePos.List, ReferencePos.Num);
    } else {
        pppSetReferencePos(NULL, 0);
    }
    if (m_iFlag & 0x80000) {
        pppSetReferenceMatrix(ReferenceMatrix.List, ReferenceMatrix.Num);
    } else {
        pppSetReferenceMatrix(NULL, 0);
    }
}

void CEffect::setDrawFirst(u_int priority) {
    pppSetDrawFirst(m_pPartMng, priority);
}

void CEffect::setDrawLast(u_int priority) {
    pppSetDrawLast(m_pPartMng, priority);
}

void CEffect::setZSortEntity(CEntity* pEnt, FIX4 nEntZOfs, BOOL bAlpha) {
    if (pEnt) {
        m_hEnt = ((CTask*)pEnt)->getHandle();
        m_nEntZOfs = nEntZOfs;
    }
    if (bAlpha) {
        m_iFlag |= 0x40;
    } else {
        m_iFlag &= ~0x40;
    }
}

CEntity* CEffect::getZSortEntity() {
    CEntity* pEnt = (CEntity*)CTaskMan::getTask(m_hEnt);
    if (!pEnt) m_hEnt = 0;
    return pEnt;
}

#line 1191
char* CEffect::print_prof() {
    ASSERT(isActive());
    if (isWaiting()) return "waiting...\n";
    return pppPrintProf(m_pPartMng);
}

#line 1204
BOOL CEffect::isValid() {
    ASSERT(this != 0);
    if (m_eType >= 32) {
        TRACE("CEffect::isValid(): %p is not valid. %x\n", this, m_eType);
        return FALSE;
    }
    return TRUE;
}

// SOURCE DATA: .data .rodata
