#define TASK_H_NO_SLC_OVERFLOW_MSG
#define ANIM_H_FULL_CLASS
#define ANIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Anim.h"
#include "System/Graphic/Anim.h"
#include "System/HeapMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/Coord.h"
#include "System/Task.h"
#include "System/Buf.h"
#include "System/Graphic/MotData.h"
#include "System/Graphic/TriangleIK.h"
#include "System/Debug.h"
#include "System/Math3D.h"
#include "include_asm.h"

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new(size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}
inline void* operator new[](size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}
inline void operator delete[](void* mem) { com_free(mem); }

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

enum NAME_CASE {
    NAME_CASE_SENSITIVE = 0,
    NAME_CASE_UPPER = 1,
    NAME_CASE_LOWER = 2
};

MODELDATA* AMFfromMDL(INT nGrpID, INT index);

class CRsrcData {
private:
    CBuf* m_pDataSrc;
    UBYTE _m_0x04[0x40];

public:
    virtual ~CRsrcData();
    BYTE* getRsrcData() {
        BYTE* p = NULL;
        if (m_pDataSrc) p = m_pDataSrc->getBufPtr();
        return p;
    }
};

class CRsrcANN : public CRsrcData {
private:
    INT m_nCount;

public:
    INT getMotCount() const { return m_nCount; }
    CMotData* getMotData(INT nInMotNo) {
        if (nInMotNo < 0 || nInMotNo >= m_nCount) {
            return NULL;
        }
        CMotData** p = (CMotData**)getRsrcData();
        if (!p) {
            return NULL;
        }
        return p[nInMotNo];
    }
};

class CRsrcDB {
public:
    static CRsrcANN* getRsrcANN(const CHAR* name, NAME_CASE nc);
    static CRsrcANN* getRsrcANN(INT nGroup);
};

/* Placeholders (in .sdata, stripped) for the strings of INCLUDE_ASM functions, at their position */
__asm__(".section .sdata\n");

static inline void _unitMatrixVu0(FMATRIX& m) {
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

void interpolateVector(FVECTOR& local, FVECTOR& from, FVECTOR& to, FLOAT racio) {
    Math3D::VectorLerp(local, from, to, racio);
}
CAnim::CAnim() {
    m_pCrds = __null;
    m_pModelData = __null;
    m_pANN = __null;
    m_pANNEx = __null;
    m_pMotData = __null;
    m_noId = 0;
    m_Tq = 0;
    m_nSkl = 0;
    m_nFps = 30;
    m_ZeroHokan = 0;
    m_nLookBoneNum = -1;
    m_nMaxTq = 0;
    m_Speed = 1.0f;
    m_HokanMaxTq = 0;
    m_HokanTq = 0;
    m_pQuatHokan = __null;
    m_pQuatNow = __null;
    m_pFixQuat = __null;
    m_pLegIK = __null;
    m_pArmIK = __null;
    m_iLookStatus = 0;
    m_Flags[0] = 0;
    m_Flags[1] = 0x100;
    m_vRootMovePreTrs.init();
    m_vRootMovePreRot.init();
    m_vRootMoveTrs.init();
    m_vRootMoveRot.init();
    _unitMatrixVu0(m_BufRootMtx);
}
CAnim::~CAnim() {
    _dispose();
}
void CAnim::_create(INT nSklNum) {
    size_t size = nSklNum * sizeof(QUATERNION_DT);
#line 227
    m_pQuatHokan = (QUATERNION_DT*)com_malloc_d(size, __FILE__ "(227)", RET_ADDR());
    m_pQuatNow = (QUATERNION_DT*)com_malloc_d(size, __FILE__ "(228)", RET_ADDR());
    m_pFixQuat = (QUATERNION_DT*)com_malloc_d(size, __FILE__ "(229)", RET_ADDR());
}
void CAnim::_dispose() {
    if (m_pQuatHokan) delete[] m_pQuatHokan;
    m_pQuatHokan = __null;
    if (m_pQuatNow) delete[] m_pQuatNow;
    m_pQuatNow = __null;
    if (m_pFixQuat) delete[] m_pFixQuat;
    m_pFixQuat = __null;
    if (m_pLegIK) delete m_pLegIK;
    m_pLegIK = __null;
    if (m_pArmIK) delete m_pArmIK;
    m_pArmIK = __null;
}
void CAnim::link(CTask* p, const CHAR* name, MODELDATA* model) {
#line 290
    ASSERT(p);
    CCoords* pCrds = p->getCoords();
#line 292
    ASSERT(pCrds);
    link(*pCrds, name, model);
}
void CAnim::link(CCoords& crds, const CHAR* name, MODELDATA* model) {
    link(crds, CRsrcDB::getRsrcANN(name, NAME_CASE_SENSITIVE), model);
}
void CAnim::link(CTask* p, const CRsrcANN* ann, MODELDATA* model) {
#line 309
    ASSERT(p);
    CCoords* pCrds = p->getCoords();
#line 311
    ASSERT(pCrds);
    link(*pCrds, ann, model);
}
void CAnim::link(CCoords& crds, const CRsrcANN* ann, MODELDATA* model) {
    static CHAR* pszIKNames[2][2] = { { "l_foot", "r_foot" }, { "lx_rot", "rx_rot" } };
    int bAct[2];
    INT i;
    INT j;
    INT nIdx;

    if (!ann) {
        return;
    }
    m_pANN = (CRsrcANN*)ann;
    m_pModelData = model;
    m_pCrds = &crds;
    m_nSkl = crds.getCoordNum();
    _dispose();
    _create(m_nSkl);
    if (!m_pModelData) {
        return;
    }
    for (i = 0; i < 2; i++) {
        bAct[i] = FALSE;
        for (j = 0; j < 2; j++) {
            nIdx = m_pModelData->getBoneIndex(pszIKNames[i][j]);
            if (nIdx >= 0 && m_pCrds->Crd(nIdx).getParent() == &m_pCrds->Crd(0)) {
                bAct[i] = TRUE;
                break;
            }
        }
    }
    if (bAct[0]) {
        createLegIK(-1.0f, -1.0f);
    }
    if (bAct[1]) {
        createArmIK();
    }
}
void CAnim::link(CTask* p, INT nGrpID) {
#line 372
    ASSERT(p);
    CCoords* pCrds = p->getCoords();
#line 374
    ASSERT(pCrds);
    link(*pCrds, nGrpID);
}
void CAnim::link(CCoords& crds, INT nGrpID) {
    CRsrcANN* pEntry = CRsrcDB::getRsrcANN(nGrpID);
    link(crds, pEntry, AMFfromMDL(nGrpID, 0));
}
void CAnim::_init(ANIMID id, TIMEQ tq, BOOL inExFg) {
    if (!m_pCrds) {
        return;
    }
    setExFlag(inExFg);
    m_HokanTq = 0;
    m_HokanMaxTq = 0;
    setup(id, tq);
    save_hokan_by_mtx();
}
void CAnim::init(ANIMID id, TIMEQ tq) {
    _init(id, tq, FALSE);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Anim", _change__5CAnimUiiii);
void CAnim::change(ANIMID id, TIMEQ tq, TIMEQ gr) {
    _change(id, tq, gr, FALSE);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Anim", save_hokan_by_mtx__5CAnim);
void CAnim::change_by_mtx(ANIMID id, TIMEQ tq, TIMEQ gr) {
    save_hokan_by_mtx();
    change(id, tq, gr);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Anim", _start__5CAnimUiii);
void CAnim::start(ANIMID id, TIMEQ tq) {
    _start(id, tq, FALSE);
}
void CAnim::start_by_mtx(ANIMID id, TIMEQ tq) {
    if (!m_pCrds) {
        return;
    }
    m_pMotData = m_pANN->getMotData(id);
    if (m_pMotData) {
        if (m_pMotData->GetHokan() > 0) {
            change_by_mtx(id, tq, m_pMotData->GetHokan() * 50);
        } else {
            init(id, tq);
        }
    }
}
BOOL CAnim::linkEx(const CRsrcANN* inAnnEx) {
    m_pANNEx = (CRsrcANN*)inAnnEx;
    return inAnnEx != __null;
}
BOOL CAnim::linkEx(const CHAR* name) {
    return linkEx(CRsrcDB::getRsrcANN(name, NAME_CASE_SENSITIVE));
}
BOOL CAnim::linkEx(INT inRsrcId) {
    return linkEx(CRsrcDB::getRsrcANN(inRsrcId));
}
BOOL CAnim::initEx(const CHAR* rsrc_name, ANIMID id, TIMEQ tq) {
    if (!linkEx(rsrc_name)) {
        return FALSE;
    }
    _init(id, tq, TRUE);
    return TRUE;
}
BOOL CAnim::initEx(const CRsrcANN* inAnnEx, ANIMID id, TIMEQ tq) {
    if (!linkEx(inAnnEx)) {
        return FALSE;
    }
    _init(id, tq, TRUE);
    return TRUE;
}
BOOL CAnim::initEx(INT inRsrcId, ANIMID id, TIMEQ tq) {
    if (!linkEx(inRsrcId)) {
        return FALSE;
    }
    _init(id, tq, TRUE);
    return TRUE;
}
BOOL CAnim::startEx(const CHAR* rsrc_name, ANIMID id, TIMEQ tq) {
    if (!linkEx(rsrc_name)) {
        return FALSE;
    }
    _start(id, tq, TRUE);
    return TRUE;
}
BOOL CAnim::startEx(const CRsrcANN* inAnnEx, ANIMID id, TIMEQ tq) {
    if (!linkEx(inAnnEx)) {
        return FALSE;
    }
    _start(id, tq, TRUE);
    return TRUE;
}
BOOL CAnim::startEx(INT inRsrcId, ANIMID id, TIMEQ tq) {
    if (!linkEx(inRsrcId)) {
        return FALSE;
    }
    _start(id, tq, TRUE);
    return TRUE;
}
BOOL CAnim::changeEx(const CHAR* rsrc_name, ANIMID id, TIMEQ tq, TIMEQ gr) {
    if (!linkEx(rsrc_name)) {
        return FALSE;
    }
    _change(id, tq, gr, TRUE);
    return TRUE;
}
BOOL CAnim::changeEx(const CRsrcANN* inAnnEx, ANIMID id, TIMEQ tq, TIMEQ gr) {
    if (!linkEx(inAnnEx)) {
        return FALSE;
    }
    _change(id, tq, gr, TRUE);
    return TRUE;
}
BOOL CAnim::changeEx(INT inRsrcId, ANIMID id, TIMEQ tq, TIMEQ gr) {
    if (!linkEx(inRsrcId)) {
        return FALSE;
    }
    _change(id, tq, gr, TRUE);
    return TRUE;
}
__attribute__((section(".sdata"))) void _setup_placeholder(INT n) {
    if (!n) AssertApp(__FILE__, 0, "m_pANNEx", 0);
    if (!n) AssertApp(__FILE__, 0, "m_pANN", 0);
    TRACE("\x83\x82\x81[\x83V\x83\x87\x83\x93\x83" "f\x81[\x83^\x82\xcc\x83{\x81[\x83\x93\x90\x94\x82\xaa\x83\x82\x83" "f\x83\x8b\x82\xcc\x83{\x81[\x83\x93\x82\xc6\x88\xea\x92v\x82\xb5\x82\xdc\x82\xb9\x82\xf1\n");
    TRACE("\x8d\xc4\x90\xb6\x83" "A\x83j\x83\x81\x94\xd4\x8d\x86%d \x83\x82\x81[\x83V\x83\x87\x83\x93\x83{\x81[\x83\x93\x90\x94%d \x83\x82\x83" "f\x83\x8b\x83{\x81[\x83\x93\x90\x94%d\n");
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Anim", setup__5CAnimUii);
void CAnim::setRootMove(BOOL inRootMove, BOOL bContinue) {
    if (isLinked() && isNowEffective() && m_pMotData->GetNbSkl() == getSklNum()) {
        _setupRootMove(inRootMove, bContinue);
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Anim", _setupRootMove__5CAnimii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Anim", play__5CAnimii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Anim", setMotion__5CAnimiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Anim", _setUpdate__5CAnimiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Anim", _exeRootMove__5CAnim);
void CAnim::getRootMove(FVECTOR& outTrs, FLOAT& outRotY) {
    outTrs = m_vRootMoveTrs;
    outRotY = m_vRootMoveRot.y;
    clearNowFirstRootMove();
}
BOOL CAnim::isLoop(INT inAnimID) {
    CMotData* md;

    if (!m_pANN) {
        return FALSE;
    }
    if (isNowExAnim() && !m_pANNEx) {
        return FALSE;
    }
    md = m_pMotData;
    if (inAnimID != -1) {
        md = m_pANN->getMotData(inAnimID);
    }
    if (!md) {
        return FALSE;
    }
    return md->IsLoop();
}
BOOL CAnim::isLoopEx(const CRsrcANN* inAnnEx, INT inAnimID) {
    CMotData* md;

    if (!inAnnEx) {
        return FALSE;
    }
    md = m_pMotData;
    if (inAnimID != -1) {
        md = ((CRsrcANN*)inAnnEx)->getMotData(inAnimID);
    }
    if (!md) {
        return FALSE;
    }
    return md->IsLoop();
}
BOOL CAnim::isLoopEx(const CHAR* rsrc_name, INT inAnimID) {
    CRsrcANN* ann = CRsrcDB::getRsrcANN(rsrc_name, NAME_CASE_SENSITIVE);
    if (!ann) {
        return FALSE;
    }
    return isLoopEx(ann, inAnimID);
}
CHAR* CAnim::getMotFilename(INT inAnimID) {
    CMotData* md;

    if (!m_pANN) {
        return __null;
    }
    md = m_pMotData;
    if (inAnimID != -1) {
        md = m_pANN->getMotData(inAnimID);
    }
    return md->GetName();
}
INT CAnim::getMotCount() {
    if (m_pANN) {
        return m_pANN->getMotCount();
    }
    return 0;
}
INT CAnim::checkID(ANIMID id, BOOL bEx) {
    CRsrcANN* pANN;

    if (!isLinked()) {
        return 0;
    }
    pANN = bEx ? m_pANNEx : m_pANN;
    if (!pANN) {
        return 0;
    }
    if (pANN->getMotCount() < id) {
        return -1;
    }
    if (pANN->getMotData(id)) {
        return 1;
    }
    return 2;
}
void CAnim::setLookPosition(const FVECTOR& vLook, const FVECTOR& vMax, const FVECTOR& vMin, INT nBoneNum, FLOAT fRate,
                            UINT iFlags) {
    m_fLookPos[0] = vLook.x;
    m_fLookPos[1] = vLook.y;
    m_fLookPos[2] = vLook.z;
    if (fRate < 1.0f) {
        fRate = 1.0f;
    }
    m_fRate = fRate;
    m_vLookMaxMin.set(vMax.x, vMax.y, vMin.x, vMin.y);
    m_iLookFlags = iFlags;
    m_nLookBoneNum = nBoneNum;
    m_iLookStatus |= 1;
    m_fLookRot[1] = 0.0f;
    m_fTimer = 0.0f;
    m_fLookRot[0] = 0.0f;
}
void CAnim::endLookPosition(FLOAT fFrame) {
    if (fFrame < 1.0f) {
        fFrame = 1.0f;
    }
    m_fRate = fFrame;
    m_fTimer = 0.0f;
    m_iLookStatus &= ~1;
}
void CAnim::updateLookPosition(const FVECTOR& vLook, FLOAT fRate, UINT iFlags) {
    m_fLookPos[0] = vLook.x;
    m_fLookPos[1] = vLook.y;
    m_fLookPos[2] = vLook.z;
    if (fRate >= 1.0f) {
        m_fRate = fRate;
    }
    m_iLookFlags = iFlags;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Anim", _hokanRot__Ffff);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Anim", _updateLookPosition__5CAnimf);
__attribute__((section(".sdata"))) void _createLegIK_placeholder(INT n) {
    TRACE(__FILE__ "(1831)");
    if (!n) AssertApp(__FILE__, 0, "m_pLegIK", 0);
    CLegIK* pLeg = new CLegIK;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Anim", createLegIK__5CAnimff);
__attribute__((section(".sdata"))) void _createArmIK_placeholder(INT n) {
    TRACE(__FILE__ "(1847)");
    if (!n) AssertApp(__FILE__, 0, "m_pArmIK", 0);
    CArmIK* pArm = new CArmIK;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Anim", createArmIK__5CAnim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Anim", updateIK__5CAnimiif);

// SOURCE DATA: .data .rodata
