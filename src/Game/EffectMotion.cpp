#define ANIM_H_GETFLAG_DECL_ONLY /* MATCH: the original calls CAnim::getFlag out of line */
#line 1

#include "include_asm.h"

/* The unit's .rodata ends with an unnamed copy of CAttachMotion's vtable (and .text with one of its inline destructor):
 * our compiled copies are kept by these lines in config/SLUS_217.99/symbol_addrs.txt
 *     _$_13CAttachMotion_001EDB28 = 0x001EDB28; // type:func size:0x30
 *     _vt$13CAttachMotion_0047CDF8 = 0x0047CDF8; // size:0x30
 * (the data comes from source: the functions with string literals are C). */


#include "System/HeapMan.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/Buf.h"
#include "System/Debug.h"
#include "Game/EffectMotion.h"



/* MATCH: Placeholder for an unknown header's inline function: its empty string literal follows "System/RsrcData.h(156)" in
 * the original .rodata. */
inline void _emptyTrace() {
    TRACE("");
}

/* EET_HEADER::addrMapping is inline, with its strings in the original .rodata after the empty string. */
inline BOOL EET_HEADER::addrMapping() {
    if (!(uiVersion & 0x80000000)) {
        if (strncmp(szCode, "@EET", 4) != 0) {
            return FALSE;
        }
        if ((uiVersion & 0x7FFFFFFF) != 2) {
            TRACE("****************** EET\x82\xcc\x83\x6f\x81\x5b\x83\x57\x83\x87\x83\x93\x82\xaa\x8c\xc3\x82\xa2\x82\xc5\x82\xb7 ******************\n");
            return FALSE;
        }
        uiVersion |= 0x80000000;
        pDataTbl = (ATTACH_EFFECT_DATA_TBL*)((UINT)this + (UINT)pDataTbl);
        for (INT i = 0; i < iDataTblNum; i++) {
            pDataTbl[i].pAttachData = (ATTACH_EFFECT_DATA*)((UINT)this + (UINT)pDataTbl[i].pAttachData);
            pDataTbl[i].pLocusData = (LOCUS_EFFECT_DATA*)((UINT)this + (UINT)pDataTbl[i].pLocusData);
        }
    }
    return TRUE;
}

#include "System/Graphic/Anim.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "System/Graphic/Camera.h"
#include "System/Math3D.h"
#include "Game/Entity.h"

#include "System/Graphic/MotData.h"




































#include "System/RsrcDB.h"






void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void MulMatrix(FMATRIX& m0, const FMATRIX& m1, const FMATRIX& m2);

inline void _unitMatrixVu0(FMATRIX& m) {
    __asm__ volatile(
        "vmr32.xyzw $vf1, $vf0\n"
        "sqc2 $vf0, 0x30(%0)\n"
        "vmr32.xyzw $vf2, $vf1\n"
        "sqc2 $vf1, 0x20(%0)\n"
        "vmr32.xyzw $vf3, $vf2\n"
        "sqc2 $vf2, 0x10(%0)\n"
        "sqc2 $vf3, 0x0(%0)\n"
        : : "r"(&m) : "memory");
}

inline FLOAT DegToRad(FLOAT d) { return d * 0.017453292f; }

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

CEffectMotionCtrl::CEffectMotionCtrl(CEffectMotion* inParent) {
    m_iStep = 3;
    m_pParent = inParent;
    m_pAttachEffectData = NULL;
    m_pLocusEffectData = NULL;
    m_hHandle = 0;
}

CEffectMotionCtrl::~CEffectMotionCtrl() {
    kill();
}


#line 44
BOOL CEffectMotionCtrl::initalize(CEntity* inOwner) {
    ASSERT(inOwner);
    m_pOwner = inOwner;
    return TRUE;
}




void CEffectMotionCtrl::set(const ATTACH_EFFECT_DATA* inData) {
    m_iStep = 0;
    m_pAttachEffectData = inData;
    m_pLocusEffectData = NULL;
    m_hHandle = 0;
    m_iCount = inData->iSetCount;
    if (m_iCount <= 0) {
        m_iCount = -1;
    }
}

void CEffectMotionCtrl::set(const LOCUS_EFFECT_DATA* inData) {
    m_iStep = 0;
    m_pAttachEffectData = NULL;
    m_pLocusEffectData = inData;
    m_hHandle = 0;
    m_iCount = -1;
}

void CEffectMotionCtrl::update(TIMEQ inCurrentTime) {
    if (!updateAttachEffect_(inCurrentTime)) {
        updateLocusEffect_(inCurrentTime);
    }
}

void CEffectMotionCtrl::stop() {
    if (m_pAttachEffectData) {
        if (m_pAttachEffectData->isLoopEnd()) {
            CEffect* effect = CEffectMan::getEffect(m_hHandle);
            if (effect) {
                effect->loopEnd();
                if (!m_pAttachEffectData->isAttachOff()) {
                    m_pParent->setObserveEffect(m_hHandle, m_pAttachEffectData, m_pLocusEffectData);
                }
            }
        } else if (!m_pAttachEffectData->isLoopContinue() && !m_pAttachEffectData->isAttachOff()) {
            fadeoutEffect_(m_pAttachEffectData->fFadeTime);
            m_pParent->setObserveEffect(m_hHandle, m_pAttachEffectData, m_pLocusEffectData);
        }
    } else if (m_pLocusEffectData) {
        fadeoutEffect_(m_pLocusEffectData->fFadeTime);
        m_pParent->setObserveEffect(m_hHandle, m_pAttachEffectData, m_pLocusEffectData);
    }
    m_hHandle = 0;
    m_pAttachEffectData = NULL;
    m_pLocusEffectData = NULL;
}

void CEffectMotionCtrl::detach() {
    if (m_pAttachEffectData && !m_pAttachEffectData->isAttachOff()) {
        CEffect* effect = CEffectMan::getEffect(m_hHandle);
        if (effect) {
            effect->detachCoord();
        }
    }
}

void CEffectMotionCtrl::kill() {
    CEffect* effect = CEffectMan::getEffect(m_hHandle);
    if (effect) {
        if (m_pAttachEffectData) {
            if (effect->isLoop()) {
                effect->fadeout(m_pAttachEffectData->fFadeTime);
            }
            effect->detachCoord();
        } else if (m_pLocusEffectData) {
            if (effect->isLoop()) {
                effect->fadeout(m_pLocusEffectData->fFadeTime);
            }
            effect->detachCoord();
        }
    }
    m_hHandle = 0;
    m_pAttachEffectData = NULL;
    m_pLocusEffectData = NULL;
}

void CEffectMotionCtrl::kill(FLOAT inFadeTime) {
    CEffect* effect = CEffectMan::getEffect(m_hHandle);
    if (effect) {
        effect->fadeout(inFadeTime);
        m_pParent->setObserveEffect(m_hHandle, m_pAttachEffectData, m_pLocusEffectData);
    }
    m_hHandle = 0;
    m_pAttachEffectData = NULL;
    m_pLocusEffectData = NULL;
}

HANDLE CEffectMotionCtrl::fadeoutEffect_(FLOAT inTime) {
    CEffect* effect = CEffectMan::getEffect(m_hHandle);
    if (!effect) {
        return 0;
    }
    if (effect->isLoop()) {
        effect->fadeout(inTime);
    }
    return m_hHandle;
}

BOOL CEffectMotionCtrl::updateAttachEffect_(TIMEQ inCurrentTime) {
    CEffect* effect;
    if (!m_pAttachEffectData) {
        return FALSE;
    }
    switch (m_iStep) {
    case 1:
        if (inCurrentTime < m_pAttachEffectData->getEnd()) {
            return TRUE;
        }
        m_iStep = 2;
        {
        CEffect* effect = CEffectMan::getEffect(m_hHandle);
        if (effect) {
            if (m_pAttachEffectData->isLoopEnd()) {
                effect->loopEnd();
            } else {
                effect->fadeout(m_pAttachEffectData->fFadeTime);
            }
            if (!m_pAttachEffectData->isAttachOff()) {
                m_pParent->setObserveEffect(m_hHandle, m_pAttachEffectData, m_pLocusEffectData);
            }
        }
        }
        m_hHandle = 0;
        return TRUE;
    case 0:
        if (inCurrentTime >= m_pAttachEffectData->getStart() || m_pOwner->isMotionEnd()) {
            FVECTOR offset;
            FVECTOR rot;
            offset.set(m_pAttachEffectData->fOffsetX, m_pAttachEffectData->fOffsetY, m_pAttachEffectData->fOffsetZ);
            rot.set(DegToRad(m_pAttachEffectData->fRotX), DegToRad(m_pAttachEffectData->fRotY),
                    DegToRad(m_pAttachEffectData->fRotZ));
            Math3D::LimitPIVector(rot);
            if (m_pAttachEffectData->isAttachOff()) {
                if (m_pAttachEffectData->isParentRotOff()) {
                    ApplyMatrix(offset, m_pOwner->Crd(m_pAttachEffectData->iBoneIndex).getWorldMatrix(), offset);
                    m_hHandle = m_pOwner->setEffect(m_pAttachEffectData->szDpxName, m_pAttachEffectData->iEffectNo,
                                                    &offset, &rot, 0.0f);
                } else {
                    FMATRIX m;
                    _unitMatrixVu0(m);
                    RotMatrixX(m, m, rot.x);
                    RotMatrixY(m, m, rot.y);
                    RotMatrixZ(m, m, rot.z);
                    m.setTrans(offset);
                    MulMatrix(m, m_pOwner->Crd(m_pAttachEffectData->iBoneIndex).getWorldMatrix(), m);
                    m_hHandle = m_pOwner->setEffect(m_pAttachEffectData->szDpxName, m_pAttachEffectData->iEffectNo,
                                                    &m, 0.0f);
                }
            } else {
                m_hHandle = m_pOwner->attachEffect(m_pAttachEffectData->szDpxName, m_pAttachEffectData->iEffectNo,
                                                   &m_pOwner->Crd(m_pAttachEffectData->iBoneIndex), &offset, &rot,
                                                   0.0f);
            }
            if (m_pAttachEffectData->tqEnd < 0) {
                m_iStep = 2;
            } else {
                m_iStep = 1;
            }
            effect = CEffectMan::getEffect(m_hHandle);
            if (effect) {
                effect->scale().set(m_pAttachEffectData->fScaleX, m_pAttachEffectData->fScaleY,
                                    m_pAttachEffectData->fScaleZ);
                effect->setAttachNoRot(m_pAttachEffectData->isParentRotOff());
                effect->setAttachReverseTrs(m_pAttachEffectData->isAttachReverseTrs());
                effect->setLightCalc(m_pAttachEffectData->isLightOn() != 0);
                effect->setViewclip(m_pAttachEffectData->isViewClipOff() == 0);
                effect->setAttachNoScale(m_pAttachEffectData->isNoAttachScale());
                FLOAT skip = (FLOAT)(inCurrentTime - m_pAttachEffectData->getStart()) / 50.0f;
                effect->setSkipFrame(skip + m_pAttachEffectData->fSkipFrame);
                if (m_pAttachEffectData->isLoopContinue()) {
                    m_pParent->setObserveEffect(m_hHandle, m_pAttachEffectData, m_pLocusEffectData);
                    m_iStep = 3;
                }
                if (m_pAttachEffectData->iZOffset) {
                    effect->setZSortEntity(m_pOwner, m_pAttachEffectData->iZOffset,
                                           m_pAttachEffectData->isNoGetOwnerAlpha() == 0);
                }
            }
        }
    case 2:
        if (m_pOwner->getAnim()->isLoop(-1) && (m_pOwner->getAnim()->getFlag(0) & 0x80)) {
            CEffect* effect = CEffectMan::getEffect(m_hHandle);
            if (effect) {
                if (effect->isLoop()) {
                    return TRUE;
                }
                if (m_iCount <= 0) {
                    if (!m_pAttachEffectData->isAttachOff()) {
                        m_pParent->setObserveEffect(m_hHandle, m_pAttachEffectData, m_pLocusEffectData);
                    }
                    m_iStep = 0;
                } else if (--m_iCount > 0) {
                    if (m_pAttachEffectData->isAttachOff()) {
                        m_iStep = 0;
                    } else {
                        m_pParent->setObserveEffect(m_hHandle, m_pAttachEffectData, m_pLocusEffectData);
                        goto zero;
                    }
                } else {
                    m_iStep = 3;
                }
            } else {
                if (m_iCount <= 0 || --m_iCount > 0) {
                zero: /* MATCH: the kept m_iStep = 0 tail (cross-jump keeps this copy) */
                    m_iStep = 0;
                } else {
                    m_iStep = 3;
                }
            }
        }
        break;
    }
    return TRUE;
}

BOOL CEffectMotionCtrl::updateLocusEffect_(TIMEQ inCurrentTime) {
    CEffect* effect;
    if (!m_pLocusEffectData) {
        return FALSE;
    }
    switch (m_iStep) {
    case 1:
        if (inCurrentTime < m_pLocusEffectData->getEnd()) {
            return TRUE;
        }
        m_iStep = 2;
        effect = CEffectMan::getEffect(m_hHandle);
        if (effect) {
            effect->fadeout(m_pLocusEffectData->fFadeTime);
            m_pParent->setObserveEffect(m_hHandle, m_pAttachEffectData, m_pLocusEffectData);
        }
        m_hHandle = 0;
        return TRUE;
    case 0:
        if (inCurrentTime >= m_pLocusEffectData->getStart() || m_pOwner->isMotionEnd()) {
            FVECTOR offset[3];
            offset[0].set(m_pLocusEffectData->fOffsetX1, m_pLocusEffectData->fOffsetY1, m_pLocusEffectData->fOffsetZ1);
            offset[1].set(m_pLocusEffectData->fOffsetX2, m_pLocusEffectData->fOffsetY2, m_pLocusEffectData->fOffsetZ2);
            offset[2].set(m_pLocusEffectData->fOffsetX3, m_pLocusEffectData->fOffsetY3, m_pLocusEffectData->fOffsetZ3);
            m_hHandle = m_pOwner->attachEffect3(m_pLocusEffectData->szDpxName, m_pLocusEffectData->iEffectNo,
                                                &m_pOwner->Crd(m_pLocusEffectData->iBoneIndex1),
                                                &m_pOwner->Crd(m_pLocusEffectData->iBoneIndex2),
                                                &m_pOwner->Crd(m_pLocusEffectData->iBoneIndex3), &offset[0],
                                                &offset[1], &offset[2], 0.0f);
            if (m_pLocusEffectData->tqEnd < 0) {
                m_iStep = 2;
            } else {
                m_iStep = 1;
            }
        }
    case 2:
        if (m_pOwner->isMotionLoop()) {
            CEffect* effect = CEffectMan::getEffect(m_hHandle);
            if (effect) {
                if (!effect->isLoop()) {
                    m_pParent->setObserveEffect(m_hHandle, m_pAttachEffectData, m_pLocusEffectData);
                    m_iStep = 0;
                } else {
                    m_iStep = 3;
                }
            } else {
                m_iStep = 0;
            }
        }
        break;
    }
    return TRUE;
}

CEffectMotion::CEffectMotion(CEntity* inOwner) : CAttachMotion(inOwner) {
    m_iID = 0;
    m_pHeader = NULL;
    m_iEffMotCtrlCount = 0;
    m_iNowCtrlCount = 0;
    m_iObserveEffectCount = 0;
    m_pEffMotCtrl = NULL;
}

CEffectMotion::~CEffectMotion() {
    releaseObserveEffect_();
    delete[] m_pEffMotCtrl;
    m_pEffMotCtrl = NULL;
}

BOOL CEffectMotion::initalize(INT inID) {
    EET_HEADER* header = (EET_HEADER*)CRsrcDB::getFirstData(0x1A, inID, 0);
    if (!header) {
        releaseObserveEffect_();
        m_iEffMotCtrlCount = 0;
        m_iNowCtrlCount = 0;
        delete[] m_pEffMotCtrl;
        m_pEffMotCtrl = NULL;
        m_pHeader = NULL;
        return FALSE;
    }
    m_iID = inID;
    return initialize_(header);
}

BOOL CEffectMotion::initalize(const CHAR* inRsrcName) {
    EET_HEADER* header = (EET_HEADER*)CRsrcDB::getRsrcEET(inRsrcName);
    if (!header) {
        return FALSE;
    }
    return initialize_(header);
}

void CEffectMotion::stop() {
    INT n = m_iNowCtrlCount;
    CEffectMotionCtrl* ctrl = m_pEffMotCtrl;
    for (INT i = 0; i < n; i++, ctrl++) {
        ctrl->stop();
    }
    m_iNowCtrlCount = 0;
}

void CEffectMotion::detach() {
    INT n = m_iNowCtrlCount;
    CEffectMotionCtrl* ctrl = m_pEffMotCtrl;
    for (INT i = 0; i < n; i++, ctrl++) {
        ctrl->detach();
    }
}

void CEffectMotion::kill(FLOAT inFadeTime) {
    INT n = m_iNowCtrlCount;
    CEffectMotionCtrl* ctrl = m_pEffMotCtrl;
    for (INT i = 0; i < n; i++, ctrl++) {
        ctrl->kill(inFadeTime);
    }
    m_iNowCtrlCount = 0;
}

void CEffectMotion::update() {
    TIMEQ current = m_pOwner->getMotionCurrentTime();
    INT n = m_iNowCtrlCount;
    CEffectMotionCtrl* ctrl = m_pEffMotCtrl;
    for (INT i = 0; i < n; i++, ctrl++) {
        ctrl->update(current);
    }
    updateObserveEffect_();
}

void CEffectMotion::killLoopEffect(BOOL inLoopEnd, INT inNo) {
    INT n = m_iObserveEffectCount;
    OBSERVE_EFFECT* observe = m_ObserveEffect;
    for (INT i = 0; i < n; i++, observe++) {
        if (observe->pAttachEffectData && observe->pAttachEffectData->isLoopContinue() && !observe->bStop) {
            CEffect* effect = CEffectMan::getEffect(observe->hHandle);
            if (effect) {
                if (inLoopEnd) {
                    effect->loopEnd();
                } else {
                    effect->fadeout(observe->pAttachEffectData->fFadeTime);
                }
            }
            observe->bStop = TRUE;
        }
    }
}


void CEffectMotion::setObserveEffect(HANDLE inHandle, const ATTACH_EFFECT_DATA* inAttach,
                                     const LOCUS_EFFECT_DATA* inLocus) {
    CEffect* effect = CEffectMan::getEffect(inHandle);
    if (!effect) {
        return;
    }
    INT n = m_iObserveEffectCount;
    OBSERVE_EFFECT* observe = m_ObserveEffect;
    for (INT i = 0; i < n; i++, observe++) {
        if (observe->hHandle == inHandle) {
            return;
        }
    }
    observe = getObserveEffect_();
    if (observe) {
        observe->hHandle = inHandle;
        observe->pAttachEffectData = inAttach;
        observe->pLocusEffectData = inLocus;
        observe->bStop = FALSE;
    } else {
        TRACE("warning : CEffectMotion::getObserveEffect_() failed.\n");
        effect->detachCoord();
    }
}




void CEffectMotion::setColor(const FVECTOR& inColor) {
    INT n = m_iObserveEffectCount;
    OBSERVE_EFFECT* observe = m_ObserveEffect;
    INT i;
    for (i = 0; i < n; i++, observe++) {
        CEffect* effect = CEffectMan::getEffect(observe->hHandle);
        if (effect) {
            effect->color() = inColor;
        }
    }
    n = m_iNowCtrlCount;
    CEffectMotionCtrl* ctrl = m_pEffMotCtrl;
    for (i = 0; i < n; i++, ctrl++) {
        CEffect* effect = CEffectMan::getEffect(ctrl->getHandle());
        if (effect) {
            effect->color() = inColor;
        }
    }
}

void CEffectMotion::setAlpha(FLOAT inAlpha) {
    INT n = m_iObserveEffectCount;
    OBSERVE_EFFECT* observe = m_ObserveEffect;
    INT i;
    for (i = 0; i < n; i++, observe++) {
        CEffect* effect = CEffectMan::getEffect(observe->hHandle);
        if (effect) {
            effect->color().w = inAlpha;
        }
    }
    n = m_iNowCtrlCount;
    CEffectMotionCtrl* ctrl = m_pEffMotCtrl;
    for (i = 0; i < n; i++, ctrl++) {
        CEffect* effect = CEffectMan::getEffect(ctrl->getHandle());
        if (effect) {
            effect->color().w = inAlpha;
        }
    }
}


BOOL CEffectMotion::initialize_(EET_HEADER* inHeader) {
    if (!inHeader->addrMapping()) {
        return FALSE;
    }
    CRsrcANN* ann = CRsrcDB::getRsrcANN(m_iID);
    if (!ann) {
        return FALSE;
    }
    stop();
    delete[] m_pEffMotCtrl;
    m_pEffMotCtrl = NULL;
    m_pHeader = inHeader;
    for (INT i = 0; i < m_pHeader->iDataTblNum; i++) {
        INT index = -1;
        for (INT j = 0; j < ann->getMotCount(); j++) {
            CMotData* motData = ann->getMotData(j);
            if (motData && strcmp(motData->GetName(), m_pHeader->pDataTbl[i].szMotionName) == 0) {
                index = j;
                break;
            }
        }
        m_pHeader->pDataTbl[i].iMotionIndex = index;
    }
    m_iEffMotCtrlCount = 32;
#line 1248
    m_pEffMotCtrl = new (__FILE__ "(1248)") CEffectMotionCtrl[32](this);
    if (!m_pEffMotCtrl) {
        m_iEffMotCtrlCount = 0;
        m_iNowCtrlCount = 0;
        return FALSE;
    }
    for (INT i = 0; i < m_iEffMotCtrlCount; i++) {
        m_pEffMotCtrl[i].initalize(m_pOwner);
    }
    return TRUE;
}




BOOL CEffectMotion::set_(INT inIndex, INT inNo, BOOL inStop) {
    if (!m_pHeader) {
        return FALSE;
    }
    if (inStop) {
        stop();
    }
    INT count = m_iNowCtrlCount;
    INT n = m_pHeader->iDataTblNum;
    ATTACH_EFFECT_DATA_TBL* tbl = m_pHeader->pDataTbl;
    for (INT i = 0; i < n; i++, tbl++) {
        if (inIndex != tbl->iMotionIndex) {
            continue;
        }
        ATTACH_EFFECT_DATA* attachData = tbl->pAttachData;
        for (SHORT j = 0; j < tbl->sAttachDataNum; j++, attachData++) {
            if (attachData->iNo != inNo) {
                continue;
            }
            if (attachData->isLoopContinue() && doLoopEffect_(attachData)) {
                continue;
            }
            m_pEffMotCtrl[count++].set(attachData);
            if (count >= m_iEffMotCtrlCount) {
                m_iNowCtrlCount = count;
                return TRUE;
            }
        }
        m_iNowCtrlCount = count;
        LOCUS_EFFECT_DATA* locusData = tbl->pLocusData;
        for (SHORT j = 0; j < tbl->sLocusDataNum; j++, locusData++) {
            if (locusData->iNo != inNo) {
                continue;
            }
            m_pEffMotCtrl[count++].set(locusData);
            if (count >= m_iEffMotCtrlCount) {
                m_iNowCtrlCount = count;
                return TRUE;
            }
        }
    }
    m_iNowCtrlCount = count;
    return count > 0;
}

void CEffectMotion::updateObserveEffect_() {
    INT n = m_iObserveEffectCount;
    for (INT i = 0; i < n; i++) {
        if (!CEffectMan::getEffect(m_ObserveEffect[i].hHandle)) {
            for (INT j = i; j < n - 1; j++) {
                m_ObserveEffect[j] = m_ObserveEffect[j + 1];
            }
            n--;
        }
    }
    m_iObserveEffectCount = n;
}

BOOL CEffectMotion::doLoopEffect_(const ATTACH_EFFECT_DATA* inData) {
    INT n = m_iObserveEffectCount;
    OBSERVE_EFFECT* observe = m_ObserveEffect;
    for (INT i = 0; i < n; i++, observe++) {
        if (observe->pAttachEffectData == inData && !observe->bStop) {
            return TRUE;
        }
    }
    return FALSE;
}

OBSERVE_EFFECT* CEffectMotion::getObserveEffect_() {
    if (m_iObserveEffectCount >= 64) {
        return NULL;
    }
    return &m_ObserveEffect[m_iObserveEffectCount++];
}

void CEffectMotion::releaseObserveEffect_() {
    INT n = m_iObserveEffectCount;
    OBSERVE_EFFECT* observe = m_ObserveEffect;
    for (INT i = 0; i < n; i++, observe++) {
        CEffect* effect = CEffectMan::getEffect(observe->hHandle);
        if (!effect) {
            continue;
        }
        if (observe->pAttachEffectData) {
            if (observe->pAttachEffectData->isLoopEnd()) {
                effect->loopEnd();
            } else {
                effect->fadeout(observe->pAttachEffectData->fFadeTime);
            }
            effect->detachCoord();
        } else if (observe->pLocusEffectData) {
            effect->fadeout(observe->pLocusEffectData->fFadeTime);
            effect->detachCoord();
        }
    }
}

// SOURCE DATA: .rodata
