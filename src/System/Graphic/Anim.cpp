

#define ANIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Anim.h" /* MATCH: this unit names Anim.h so (header default "System/Graphic/Anim.h") */
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



#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)







MODELDATA* AMFfromMDL(INT nGrpID, INT index);

































extern FVECTOR ONE_VECTOR;
extern "C" float atan2f(float y, float x);
















static inline FLOAT _limitRot(FLOAT r) {
    return (r < -3.1415927f) ? r + 6.2831855f : (r > 3.1415927f) ? r - 6.2831855f : r; // MATCH: one ternary (the if/return form keeps the sum in another FP register)
}




static inline void _setOne(FVECTOR& v) {
    v.set(1.0f, 1.0f, 1.0f, 1.0f);



}






void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void MulMatrix(FMATRIX& m0, const FMATRIX& m1, const FMATRIX& m2);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);



#include "System/RsrcDB.h"





/* Placeholders (in .sdata, stripped) for the strings of INCLUDE_ASM functions, at their position */
__asm__(".section .sdata\n");














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
    m_BufRootMtx.init();
}
CAnim::~CAnim() {
    _dispose();
}
void CAnim::_create(INT nSklNum) {
#line 227
    m_pQuatHokan = new KH_AT("(227)") QUATERNION_DT[nSklNum];
    m_pQuatNow = new KH_AT("(228)") QUATERNION_DT[nSklNum];
    m_pFixQuat = new KH_AT("(229)") QUATERNION_DT[nSklNum];
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
void CAnim::_change(ANIMID id, TIMEQ tq, TIMEQ gr, BOOL inExFg) {
    if (!m_pCrds) {
        return;
    }
    setExFlag(inExFg);
    if (gr < 0) {
        if (!inExFg) {
            m_pMotData = m_pANN->getMotData(id);
        } else {
            if (!m_pANNEx) {
                return;
            }
            m_pMotData = m_pANNEx->getMotData(id);
        }
        if (!m_pMotData) {
            return;
        }
        gr = m_pMotData->GetHokan() * 50;
    }
    m_HokanTq = gr;
    m_HokanMaxTq = gr;
    memcpy(m_pQuatHokan, m_pQuatNow, m_nSkl * sizeof(QUATERNION_DT));
    setup(id, tq);
}
void CAnim::change(ANIMID id, TIMEQ tq, TIMEQ gr) {
    _change(id, tq, gr, FALSE);
}
void CAnim::save_hokan_by_mtx() {
    QUATERNION_DT* q0;
    INT i;
    FVECTOR vScale;

    if (!isLinked()) {
        return;
    }
    q0 = m_pQuatNow;
    for (i = 0; i < m_nSkl; i++, q0++) {
        if (i == 0) {
            continue;
        }
        CCoord& crd = m_pCrds->Crd(i);
        crd.getScale(vScale);
        Quaternion::MatrixToQuaternion(q0->vQuatRotate, crd.getLocalMatrix());
        FVECTOR vTrans = crd.getLocalMatrix().getTrans();
        q0->vTrans.set(vTrans);
        q0->vScale.set(vScale.x, vScale.y, vScale.z);
    }
}
void CAnim::change_by_mtx(ANIMID id, TIMEQ tq, TIMEQ gr) {
    save_hokan_by_mtx();
    change(id, tq, gr);
}
void CAnim::_start(ANIMID id, TIMEQ tq, BOOL inExFg) {
    if (!m_pCrds) {
        return;
    }
    if (!inExFg) {
        m_pMotData = m_pANN->getMotData(id);
    } else {
        if (!m_pANNEx) {
            return;
        }
        m_pMotData = m_pANNEx->getMotData(id);
    }
    if (!m_pMotData) {
        return;
    }
    setExFlag(inExFg);
    if (m_pMotData->GetHokan() > 0) {
        _change(id, tq, m_pMotData->GetHokan() * 50, inExFg);
    } else {
        _change(id, tq, 0, inExFg);
    }
}
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
void CAnim::setup(ANIMID id, TIMEQ tq) {
    INT i;
    CMotData* md;
    INT nFixAll;

    if (!isLinked()) {
        return;
    }
    m_noId = id;
    m_Tq = (TIMEQ)(tq * m_Speed);
    if (isNowExAnim()) {
#line 702
        ASSERT(m_pANNEx);
        m_pMotData = m_pANNEx->getMotData(id);
    } else {
#line 706
        ASSERT(m_pANN);
        m_pMotData = m_pANN->getMotData(id);
    }
    if (!isNowEffective()) {
        return;
    }
    if (m_pMotData->GetNbSkl() != m_nSkl) {
        TRACE("\x83\x82\x81[\x83V\x83\x87\x83\x93\x83" "f\x81[\x83^\x82\xcc\x83{\x81[\x83\x93\x90\x94\x82\xaa\x83\x82\x83" "f\x83\x8b\x82\xcc\x83{\x81[\x83\x93\x82\xc6\x88\xea\x92v\x82\xb5\x82\xdc\x82\xb9\x82\xf1\n");
        TRACE("\x8d\xc4\x90\xb6\x83" "A\x83j\x83\x81\x94\xd4\x8d\x86%d \x83\x82\x81[\x83V\x83\x87\x83\x93\x83{\x81[\x83\x93\x90\x94%d \x83\x82\x83" "f\x83\x8b\x83{\x81[\x83\x93\x90\x94%d\n", id, m_pMotData->GetNbSkl(), m_nSkl);
        return;
    }
    m_nFps = m_pMotData->GetFps();
    m_nMaxFrame = m_pMotData->GetNbFrame();
    if (m_pMotData->IsLoop()) {
        m_nMaxTq = m_nMaxFrame * 3000 / m_nFps;
    } else {
        m_nMaxTq = (m_nMaxFrame - 1) * 3000 / m_nFps;
    }
    m_FlagsB[1].l = 0;
    m_Flags[0] = 0;
    setNowFirst();
    m_ZeroHokan = FALSE;
    _setRootMoveFg(FALSE);
    for (i = 0; i < m_nSkl; i++) {
        m_pFixQuat[i].vQuatRotate._q = 0;
    }
    m_pMotData->PrepareGetDataHS(0.0f);
    nFixAll = 0;
    md = m_pMotData;
    for (i = 0; i < m_nSkl; i++) {
        if (md->IsFixSkl(i)) {
            nFixAll++;
        }
    }
    if (nFixAll > 0) {
        for (i = 0; i < m_nSkl; i++) {
            if (md->IsFixSkl(i)) {
                QUATERNION_DT& quat0 = m_pFixQuat[i];
                Quaternion::MakeQuaternion(quat0.vQuatRotate, md->GetCmpData(3, i, 0.0f), md->GetCmpData(4, i, 0.0f),
                                           md->GetCmpData(5, i, 0.0f));
                quat0.vTrans.set(md->GetCmpData(0, i, 0.0f), md->GetCmpData(1, i, 0.0f), md->GetCmpData(2, i, 0.0f));
                if (md->DoScaleAnime()) {
                    quat0.vScale.set(md->GetCmpData(6, i, 0.0f), md->GetCmpData(7, i, 0.0f),
                                     md->GetCmpData(8, i, 0.0f));
                } else {
                    quat0.vScale.set(ONE_VECTOR);
                }
            }
        }
    }
    setMotion(tq, 0, FALSE);
}
void CAnim::setRootMove(BOOL inRootMove, BOOL bContinue) {
    if (isLinked() && isNowEffective() && m_pMotData->GetNbSkl() == getSklNum()) {
        _setupRootMove(inRootMove, bContinue);
    }
}
void CAnim::_setupRootMove(BOOL inRootMove, BOOL bContinue) {
    QUATERNION_DT* q0;
    QUATERNION_DT* q1;

    _setRootMoveFg(inRootMove);
    if (inRootMove) {
        q0 = m_pQuatNow;
        q0->vQuatRotate.init();
        q0->vTrans.init();
        q0->vScale.set(ONE_VECTOR);
        q1 = m_pQuatHokan;
        *q1 = *q0;
        m_BufRootMtx = m_pCrds->Crd(0).getLocalMatrix();
        if (bContinue) {
            setNowFirstRootMoveContinue();
        } else {
            setNowFirstRootMove();
        }
        _setUpdate(1, 0, FALSE);
    } else {
        clearNowFirstRootMove();
    }
}
void CAnim::play(TIMEQ inAddTq, BOOL bClipped) {
    TIMEQ HoAddTq;

    if (!isNowEffective()) {
        return;
    }
    if (m_Speed == 0.0f) {
        return;
    }
    HoAddTq = inAddTq;
    if (isNowFirst()) {
        clearNowFirst();
        inAddTq = 0;
        if (m_HokanTq <= 0) {
            return;
        }
    }
    inAddTq = (TIMEQ)(inAddTq * m_Speed);
    if (m_HokanTq > 0) {
        HoAddTq = (TIMEQ)(HoAddTq * m_Speed);
        if (HoAddTq >= 0.0f) {
            m_HokanTq -= HoAddTq;
        } else {
            m_HokanTq += HoAddTq;
        }
        if (m_HokanTq < 0) {
            m_HokanTq = 0;
        }
    }
    if (m_HokanTq <= 0) {
        m_ZeroHokan = FALSE;
    }
    setMotion(m_Tq, inAddTq, bClipped);
}
void CAnim::setMotion(TIMEQ inTq, TIMEQ inPlus, BOOL bClipped) {
    INT nFrame;

    if (!isLinked()) {
        return;
    }
    if (!isNowEffective()) {
        return;
    }
    if (m_pMotData->GetNbSkl() != m_nSkl) {
        return;
    }
    m_Flags[0] = 0;
    inTq += inPlus; /* MATCH: the parameter is the running time (no tq local) */
    if (m_ZeroHokan) {
        inTq = 0;
    }
    if (m_pMotData->IsLoop()) {
        if (m_Speed >= 0.0f) {
            nFrame = (m_nMaxFrame - 1) * 3000 / m_nFps;
            if (inTq >= nFrame) {
                if (!(m_Flags[1] & 0x40)) {
                    m_Flags[0] = 1;
                    m_Flags[1] |= 0x41;
                    inTq = nFrame;
                }
            } else {
                m_Flags[1] &= ~0x40;
            }
            if (inTq >= m_nMaxTq) {
                { INT t = (SHORT)(m_pMotData->m_nRetFrame - 1) * 3000 / m_nFps; inTq = inTq % m_nMaxTq + t; } /* MATCH: loop term first */
                if (inTq >= m_nMaxTq) {
                    inTq = m_nMaxTq - 3000 / m_nFps;
                }
                m_Flags[0] |= 0x80;
                m_Flags[1] |= 0x80;
            } else if (inTq < 0) {
                inTq = m_nMaxTq - (-inTq % m_nMaxTq);
            }
        } else {
            if (inTq < 0) {
                inTq = m_nMaxTq - (-inTq % m_nMaxTq);
                m_Flags[0] = 0x81;
                m_Flags[1] |= 0x81;
            } else if (inTq >= m_nMaxTq) {
                inTq = inTq % m_nMaxTq;
            }
        }
    } else {
        if (m_Speed >= 0.0f) {
            if (inTq < 0) {
                inTq = 0;
            } else if (inTq >= m_nMaxTq) {
                m_Flags[0] = 2;
                m_Flags[1] |= 2;
                inTq = m_nMaxTq;
            }
        } else {
            if (inTq < 0) {
                m_Flags[0] = 2;
                m_Flags[1] |= 2;
                inTq = 0;
            } else if (inTq >= m_nMaxTq) {
                inTq = m_nMaxTq;
            }
        }
    }
    m_Tq = inTq;
    _setUpdate(m_nSkl, inPlus, bClipped);
}
void CAnim::_setUpdate(int nSkl, TIMEQ tqFr, BOOL bClipped) {
    FLOAT frm0;
    CMotData* md;
    FMATRIX t0;
    FMATRIX t1;
    FMATRIX t2;
    FLOAT racio;
    INT start_skl;
    QUATERNION_DT* pQuat;
    QUATERNION_DT* pQuatHokan;
    BOOL bDoScaleAnime;
    BOOL bClipOut;
    INT end_skl;

    md = m_pMotData;
    frm0 = (FLOAT)m_Tq * (FLOAT)m_nFps / 3000.0f;
    start_skl = isRootMove() ? 0 : 1;
    pQuat = &m_pQuatNow[start_skl];
    pQuatHokan = &m_pQuatHokan[start_skl];
    bDoScaleAnime = md->DoScaleAnime();
    bClipOut = isNowClipCheck() && bClipped;
    if (!bClipOut) {
        end_skl = nSkl;
    } else {
        m_HokanTq = 0;
        end_skl = isRootMove();
    }
    if (m_HokanTq <= 0) {
        m_Flags[1] &= ~0x800;
    } else {
        m_Flags[1] |= 0x800;
    }
    if (isRootMove()) {
        m_BufRootMtx = m_pCrds->Crd(0).getLocalMatrix();
    }
    for (INT i = start_skl; i < end_skl; i++, pQuat++, pQuatHokan++) {
        CCoord& crd = m_pCrds->Crd(i);
        FVECTOR vScale;
        INT fix_no = -1;
        if (m_pFixQuat[i].isEffective()) {
            fix_no = i;
        }
        if (fix_no != -1) {
            if (m_HokanTq <= 0) {
                QUATERNION_DT* pFix = m_pFixQuat + fix_no; /* MATCH: ptr + n (not &ptr[n]) adds base first */
                vScale = pFix->vScale;
                Quaternion::QuaternionToMatrix(crd.getLocalMatrix(), pFix->vQuatRotate, pFix->vTrans);
                *pQuat = m_pFixQuat[fix_no];
                crd.setScale(vScale);
                continue;
            }
            *pQuat = m_pFixQuat[fix_no];
        } else {
            md->GetQuaternionDt(*pQuat, i, frm0);
        }
        if (i == 0 || m_HokanTq <= 0) {
            Quaternion::QuaternionToMatrix(crd.getLocalMatrix(), pQuat->vQuatRotate, pQuat->vTrans);
            if (bDoScaleAnime) {
                vScale = pQuat->vScale;
            } else {
                _setOne(vScale);
            }
        } else {
            racio = (FLOAT)m_HokanTq / (FLOAT)m_HokanMaxTq;
            if (racio >= 1.0f) {
                Quaternion::QuaternionToMatrix(crd.getLocalMatrix(), pQuatHokan->vQuatRotate, pQuatHokan->vTrans);
                vScale = pQuatHokan->vScale;
                *pQuat = *pQuatHokan;
            } else {
                Quaternion::QuaternionSlerp(pQuat->vQuatRotate, pQuat->vQuatRotate, pQuatHokan->vQuatRotate, racio);
                pQuat->vTrans = pQuat->vTrans + racio * (pQuatHokan->vTrans - pQuat->vTrans);
                pQuat->vScale = pQuat->vScale + racio * (pQuatHokan->vScale - pQuat->vScale);
                Quaternion::QuaternionToMatrix(crd.getLocalMatrix(), pQuat->vQuatRotate, pQuat->vTrans);
                vScale = pQuat->vScale;
            }
        }
        crd.setScale(vScale);
    }
    if (!isNowFirstRootMove()) {
        _updateLookPosition((FLOAT)tqFr * 0.020000001f);
    }
    _exeRootMove();
}
void CAnim::_exeRootMove() {
    FVECTOR trs1;
    FLOAT rot1;
    FLOAT rot0;
    FVECTOR trs0;
    INT nRM;
    FVECTOR trs2;
    FLOAT rot2;

    if (!isRootMove()) {
        return;
    }
    CCoord& crd = m_pCrds->Crd(0);
    trs1 = crd.getLocalMatrix().v[3]; /* the original: getTrans() (Coord.h:158) here and for trs0; our by-value getTrans adds a stack copy */
    rot1 = -atan2f(crd.getLocalMatrix().m[0][2], crd.getLocalMatrix().m[0][0]);
    rot0 = -atan2f(m_BufRootMtx.m[0][2], m_BufRootMtx.m[0][0]);
    trs0 = m_BufRootMtx.v[3];
    nRM = isNowFirstRootMove();
    if (nRM == 1) {
        m_vRootMovePreTrs.init();
        m_vRootMovePreRot.init();
        m_vRootMoveRot.init();
    } else if (nRM == 2) {
        m_vRootMovePreTrs = trs1;
        m_vRootMoveRot.init();
        m_vRootMovePreRot.y = rot1;
    }
    trs2 = (trs1 - m_vRootMovePreTrs);
    rot2 = rot0 + (rot1 - m_vRootMovePreRot.y);
    m_vRootMoveRot.y = _limitRot(rot2);
    FMATRIX tmp0;
    FMATRIX tmp1;
    UnitMatrix(tmp0);
    RotMatrixY(tmp1, tmp0, _limitRot(rot0 - m_vRootMovePreRot.y));
    ApplyMatrix(m_vRootMoveTrs, tmp1, trs2);
    m_vRootMoveTrs += trs0;
    m_vRootMovePreTrs = trs1;
    m_vRootMovePreRot.y = rot1;
    crd.getLocalMatrix() = m_BufRootMtx;
}
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
static inline float _limitPI(float fSa) {
    if (fSa > 3.1415927f) {
        fSa -= 6.2831855f;
    } else if (fSa < -3.1415927f) {
        fSa += 6.2831855f;
    }
    return fSa;
}

static float _hokanRot(float fData0, float fData1, float fRate);

static float _hokanRot(float fData0, float fData1, float fRate) {
    float fT = 1.0f / fRate;
    float fSa = fData1 - fData0;
    /* MATCH: _limitPI written out: the result stays apart from the difference, the constants in f0 */
    register float fR asm("$f1") = fSa;
    if (fSa > 3.1415927f) {
        register float fK asm("$f0") = 6.2831855f;
        fR = fSa - fK;
    } else if (fSa < -3.1415927f) {
        register float fK asm("$f0") = 6.2831855f;
        fR = fSa + fK;
    }
    return fData0 + fR * fT;
}
void CAnim::_updateLookPosition(FLOAT fR) {
    FVECTOR fvAng;
    FVECTOR fvDir;
    FVECTOR vMax;
    FVECTOR vMin;

    if (m_nLookBoneNum < 0) {
        return;
    }
    FLOAT& fRate = m_fRate;
    CCoord& crd = m_pCrds->Crd(m_nLookBoneNum);
    CCoord& crdRoot = m_pCrds->Crd(0);
    if (!(m_iLookStatus & 1)) {
        m_fTimer += fR * (1.0f / fRate);
        if (m_fTimer >= 1.0f) {
            m_fTimer = 0.0f;
            m_nLookBoneNum = -1;
            return;
        }
        fvAng.x = m_fLookRot[0] * (1.0f - m_fTimer);
        fvAng.y = m_fLookRot[1] * (1.0f - m_fTimer);
    } else {
        FMATRIX* pmRoot = &crdRoot.getWorldMatrix();
        if (!(m_iLookFlags & 1)) {
            fvDir = crd.m_World.v[3];
            SubVector(fvDir, m_vLookPos, fvDir);
            Normalize(fvDir, fvDir);
            fvAng.x = -atan2f(fvDir.y, _sqrtf(fvDir.z * fvDir.z + fvDir.x * fvDir.x));
            fvAng.y = atan2f(fvDir.x, fvDir.z);
            fvAng.z = 0.0f;
            fvAng.w = 1.0f;
            FLOAT fRy = -atan2f(pmRoot->m[0][2], pmRoot->m[0][0]);
            fvAng.y = Math3D::LimitPI(fvAng.y - fRy);
        } else {
            fvAng.set(m_fLookPos[0], m_fLookPos[1], 0.0f);
        }
        vMax.set(m_vLookMaxMin.x, m_vLookMaxMin.y, 0.0f);
        vMin.set(m_vLookMaxMin.z, m_vLookMaxMin.w, 0.0f);
        Math3D::VectorMaximizeXYZ(fvAng, fvAng, vMin);
        Math3D::VectorMinimizeXYZ(fvAng, fvAng, vMax);
        fvAng.x = Math3D::LimitPI(_hokanRot(m_fLookRot[0], fvAng.x, fRate));
        fvAng.y = Math3D::LimitPI(_hokanRot(m_fLookRot[1], fvAng.y, fRate));
        m_fLookRot[0] = fvAng.x;
        m_fLookRot[1] = fvAng.y;
    }
    FMATRIX mMat;
    mMat.init();
    RotMatrixZ(mMat, mMat, fvAng.x);
    RotMatrixX(mMat, mMat, fvAng.y);
    MulMatrix(crd.getLocalMatrix(), crd.getLocalMatrix(), mMat);
    crdRoot.touch();
}
BOOL CAnim::createLegIK(FLOAT fFootLen, FLOAT fToesLen) {
    if (m_pModelData && !m_pLegIK) {
        if (CLegIK::isIK(*m_pModelData)) {
#line 1831
            m_pLegIK = new KH_AT("(1831)") CLegIK;
            ASSERT(m_pLegIK);
            m_pLegIK->init(*m_pCrds, *m_pModelData, fFootLen, fToesLen);
            m_Flags[1] |= 0x400;
        }
    }
    return FALSE;
}
BOOL CAnim::createArmIK() {
    if (m_pModelData && !m_pArmIK) {
        if (CArmIK::isIK(*m_pModelData)) {
#line 1847
            m_pArmIK = new KH_AT("(1847)") CArmIK;
            ASSERT(m_pArmIK);
            m_pArmIK->init(*m_pCrds, *m_pModelData);
            m_Flags[1] |= 0x400;
        }
    }
    return FALSE;
}
void CAnim::updateIK(BOOL bBeforeCoordFix, BOOL bAfterCoordFix, FLOAT fr) {
    FVECTOR* pvPosL;
    FVECTOR* pvPosR;
    FVECTOR vPos[2];

    if (bBeforeCoordFix && (m_pLegIK || m_pArmIK) && (m_Flags[1] & 0x3800)) {
        INT nNum = m_pCrds->getCoordNum();
        CCoord* pCrd = &m_pCrds->Crd(0);
        TIMEQ tq = CMainApp::getTickCount();
        for (INT i = 0; i < nNum; i++) {
            pCrd[i].fix(tq);
        }
    }
    pvPosL = __null;
    if (m_pLegIK) {
        pvPosR = __null;
        if (m_Flags[1] & 0x2000) {
            pvPosL = &vPos[0];
            pvPosR = &vPos[1];
            m_pLegIK->getFootPos(pvPosL, pvPosR, *m_pCrds, fr);
        } else {
            m_pLegIK->clearOldFootPos();
        }
        if (m_Flags[1] & 0x3800) {
            m_pLegIK->update(*m_pCrds, pvPosL, pvPosR);
        }
        m_pLegIK->updateFootEffect(*m_pCrds);
    }
    if (m_pArmIK && (m_Flags[1] & 0x1800)) {
        m_pArmIK->update(*m_pCrds, __null, __null);
    }
    if (bAfterCoordFix && (m_pLegIK || m_pArmIK) && (m_Flags[1] & 0x3800)) {
        INT nNum = m_pCrds->getCoordNum();
        CCoord* pCrd = &m_pCrds->Crd(0);
        TIMEQ tq = CMainApp::getTickCount();
        for (INT i = 0; i < nNum; i++) {
            pCrd[i].fix(tq);
        }
    }
}

// SOURCE DATA: .data .rodata
