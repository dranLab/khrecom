#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define D2MAN_H_PATH "c:/KHCOM/MainApp/System/Graphic/2d/2DMan.h" /* MATCH: this unit names 2DMan.h so (header default "System/Graphic/2D/2DMan.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DSeqObj.h"
#include "System/Graphic/2d/2DSeq.h"
#include "System/Math3D.h"
#include <string.h>
extern "C" int sprintf(char* s, const char* fmt, ...);

extern INT SCREEN_HEIGHT;

namespace D2 {



















unsigned char C2DSeqCtrl::m_bitTbl[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

template <class T> static inline T& _ref(T& v) { return v; }


















/* FVECTOR::set as the original orders its stores (name unknown) */







/* SeqControl's inline constructor (original name unknown) */
static inline void _initSeqControl(SeqControl& sc) {
    vu0UnitMatrix(sc.matrix);
    sc.parentMatrix.m[3][3] = 0.0f;
    sc.controlMatrix.m[3][3] = 0.0f;
    sc.pParentSeqControl = NULL;
    sc.fg3D = 0;
    sc.fgFntScale = 0;
    sc.fgBilinear = 0;
    sc.fgColor = 0;
    sc.parentColor.setAll(1.0f);
    sc.controlColor.setAll(1.0f);
    sc.vPos.init0();
    sc.vScl.set(1.0f, 1.0f, 1.0f, 0.0f);
    sc.vRot.zero();
    sc.vCol.setAll(1.0f);
}

C2DSeqCtrl::C2DSeqCtrl() {
    _initSeqControl(m_seqControl);
    for (INT i = 7; i >= 0; i--) {
        m_hSeqHandle[i] = 0;
    }
    m_bEnable = 1;
    m_nLayer = -1;
    m_bitEnable = 0;
    m_nType = 0;
    m_nSeq = 0;
}

C2DSeqCtrl::C2DSeqCtrl(INT nType) {
    _initSeqControl(m_seqControl);
    for (INT i = 7; i >= 0; i--) {
        m_hSeqHandle[i] = 0;
    }
    if (nType < 2) {
        m_nType = nType;
    } else {
        m_nType = 0;
        m_seqControl.fg3D = 1;
    }
    m_bEnable = 1;
    m_nLayer = -1;
    m_bitEnable = 0;
    m_nSeq = 0;
}

C2DSeqCtrl::~C2DSeqCtrl() { destroy(); }

void C2DSeqCtrl::initialize() {
    m_bEnable = 1;
    m_bitEnable = 0;
    destroy();
}

void C2DSeqCtrl::destroy() {
    for (INT i = 0; i < 8; i++) {
        if (m_hSeqHandle[i]) {
            C2DMan::destroy(m_hSeqHandle[i]);
            m_hSeqHandle[i] = 0;
        }
    }
    m_nSeq = 0;
    m_nLayer = -1;
}

void C2DSeqCtrl::destroy(UINT nIdx) {
    if (nIdx == m_nSeq - 1 && m_hSeqHandle[nIdx]) {
        C2DMan::destroy(m_hSeqHandle[nIdx]);
        m_hSeqHandle[nIdx] = 0;
        m_nSeq--;
    }
}

void C2DSeqCtrl::destroyLast() {
    if (m_nSeq) destroy(m_nSeq - 1);
}

void C2DSeqCtrl::destroyIndexToLast(UINT nIdx) {
    if (nIdx < m_nSeq) {
        for (INT i = m_nSeq - 1; i >= nIdx; i--) {
            destroy(i);
        }
    }
}

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

#line 116
C2DSeqObj* C2DSeqCtrl::addSeq(const CHAR* pLadName, UINT idxUnit, UINT idxSeq) {
    char seqNameBase[32];
    char seqName[32];
    strcpy(seqNameBase, pLadName);
    *strstr(seqNameBase, ".lad") = '\0';
    sprintf(seqName, "%s:%02d", seqNameBase, idxUnit);
    return addSeq(seqName, idxSeq);
}

C2DSeqObj* C2DSeqCtrl::addSeq(const CHAR* pSedName, UINT idxSeq) {
    if (!(m_nSeq < 8)) {
        ASSERT(FALSE);
        return NULL;
    }
    HANDLE handle = C2DMan::create(pSedName, idxSeq, &m_seqControl);
    if (!handle) return NULL;
    C2DSeqObj* p2DSeqObj = C2DSeq::getCurrent2DSeqObj();
    if (!p2DSeqObj) return NULL;
    if (!m_nType && !m_nSeq) p2DSeqObj->setParent();
    if (m_nLayer >= 0) p2DSeqObj->changeLayer(m_nLayer);
    if (!m_bEnable) p2DSeqObj->enableDisp(FALSE);
    m_bitEnable |= m_bitTbl[m_nSeq];
    m_hSeqHandle[m_nSeq] = handle;
    m_nSeq++;
    return p2DSeqObj;
}

#line 161
C2DSeqObj* C2DSeqCtrl::addFnt(const CHAR* pLadName, UINT idxUnit, C2DFnt* p2DFnt) {
    char seqNameBase[32];
    char seqName[32];
    strcpy(seqNameBase, pLadName);
    *strstr(seqNameBase, ".lad") = '\0';
    sprintf(seqName, "%s:%02d", seqNameBase, idxUnit);
    return addFnt(seqName, p2DFnt);
}

C2DSeqObj* C2DSeqCtrl::addFnt(const CHAR* pSqdName, C2DFnt* p2DFnt) {
    if (!(m_nSeq < 8)) {
        ASSERT(FALSE);
        return NULL;
    }
    HANDLE handle = C2DMan::createFnt(pSqdName, p2DFnt, &m_seqControl);
    if (!handle) return NULL;
    C2DSeqObj* p2DSeqObj = C2DSeq::getCurrent2DSeqObj();
    if (!p2DSeqObj) return NULL;
    if (!m_nType && !m_nSeq) p2DSeqObj->setParent();
    if (m_nLayer >= 0) p2DSeqObj->changeLayer(m_nLayer);
    if (!m_bEnable) p2DSeqObj->enableDisp(FALSE);
    m_bitEnable |= m_bitTbl[m_nSeq];
    m_hSeqHandle[m_nSeq] = handle;
    m_nSeq++;
    return p2DSeqObj;
}

C2DSeqObj* C2DSeqCtrl::replaceSeq(UINT nIdx, const CHAR* pLadName, UINT idxUnit, UINT idxSeq) {
    char seqNameBase[32];
    char seqName[32];
    strcpy(seqNameBase, pLadName);
    *strstr(seqNameBase, ".lad") = '\0';
    sprintf(seqName, "%s:%02d", seqNameBase, idxUnit);
    return replaceSeq(nIdx, seqName, idxSeq);
}

C2DSeqObj* C2DSeqCtrl::replaceSeq(UINT nIdx, const CHAR* pSedName, UINT idxSeq) {
    HANDLE handle;
    C2DSeqObj* p2DSeqObj;
    if (!(nIdx < m_nSeq)) return NULL;
    handle = C2DMan::replaceSeq(m_hSeqHandle[nIdx], pSedName, idxSeq, &m_seqControl);
    if (!handle) return NULL;
    p2DSeqObj = C2DSeq::getCurrent2DSeqObj();
    if (!p2DSeqObj) return NULL;
    if (!m_nType && !nIdx) p2DSeqObj->setParent();
    if (m_nLayer >= 0) p2DSeqObj->changeLayer(m_nLayer);
    if (!m_bEnable) p2DSeqObj->enableDisp(FALSE);
    m_hSeqHandle[nIdx] = handle;
    return p2DSeqObj;
}

C2DSeqObj* C2DSeqCtrl::replaceFnt(UINT nIdx, const CHAR* pLadName, UINT idxUnit, C2DFnt* p2DFnt) {
    char seqNameBase[32];
    char seqName[32];
    if (!(nIdx < m_nSeq)) return NULL;
    strcpy(seqNameBase, pLadName);
    *strstr(seqNameBase, ".lad") = '\0';
    sprintf(seqName, "%s:%02d", seqNameBase, idxUnit);
    return replaceFnt(nIdx, seqName, p2DFnt);
}

C2DSeqObj* C2DSeqCtrl::replaceFnt(UINT nIdx, const CHAR* pSedName, C2DFnt* p2DFnt) {
    HANDLE handle;
    C2DSeqObj* p2DSeqObj;
    if (!(nIdx < m_nSeq)) return NULL;
    handle = C2DMan::replaceFnt(m_hSeqHandle[nIdx], pSedName, p2DFnt, &m_seqControl);
    if (!handle) return NULL;
    p2DSeqObj = C2DSeq::getCurrent2DSeqObj();
    if (!p2DSeqObj) return NULL;
    if (!m_nType && !nIdx) p2DSeqObj->setParent();
    if (m_nLayer >= 0) p2DSeqObj->changeLayer(m_nLayer);
    if (!m_bEnable) p2DSeqObj->enableDisp(FALSE);
    m_hSeqHandle[nIdx] = handle;
    return p2DSeqObj;
}

C2DSeqObj* C2DSeqCtrl::addSeqShift(const CHAR* pLadName, UINT idxUnit, UINT idxSeq, FLOAT fShiftX, FLOAT fShiftY) {
    C2DSeqObj* p2DSeqObj = addSeq(pLadName, idxUnit, idxSeq);
    if (p2DSeqObj) p2DSeqObj->shiftBase(fShiftX, fShiftY);
    return p2DSeqObj;
}

C2DSeqObj* C2DSeqCtrl::addSeqShift(const CHAR* pSedName, UINT idxSeq, FLOAT fShiftX, FLOAT fShiftY) {
    C2DSeqObj* p2DSeqObj = addSeq(pSedName, idxSeq);
    if (p2DSeqObj) p2DSeqObj->shiftBase(fShiftX, fShiftY);
    return p2DSeqObj;
}

C2DSeqObj* C2DSeqCtrl::addFntShift(const CHAR* pLadName, UINT idxUnit, C2DFnt* p2DFnt, FLOAT fShiftX, FLOAT fShiftY) {
    C2DSeqObj* p2DSeqObj = addFnt(pLadName, idxUnit, p2DFnt);
    if (p2DSeqObj) p2DSeqObj->shiftBase(fShiftX, fShiftY);
    return p2DSeqObj;
}

C2DSeqObj* C2DSeqCtrl::addFntShift(const CHAR* pSqdName, C2DFnt* p2DFnt, FLOAT fShiftX, FLOAT fShiftY) {
    C2DSeqObj* p2DSeqObj = addFnt(pSqdName, p2DFnt);
    if (p2DSeqObj) p2DSeqObj->shiftBase(fShiftX, fShiftY);
    return p2DSeqObj;
}

C2DSeqObj* C2DSeqCtrl::replaceSeqShift(UINT nIdx, const CHAR* pLadName, UINT idxUnit, UINT idxSeq, FLOAT fShiftX,
                                       FLOAT fShiftY) {
    C2DSeqObj* p2DSeqObj = replaceSeq(nIdx, pLadName, idxUnit, idxSeq);
    if (p2DSeqObj) p2DSeqObj->shiftBase(fShiftX, fShiftY);
    return p2DSeqObj;
}

C2DSeqObj* C2DSeqCtrl::replaceSeqShift(UINT nIdx, const CHAR* pSedName, UINT idxSeq, FLOAT fShiftX, FLOAT fShiftY) {
    C2DSeqObj* p2DSeqObj = replaceSeq(nIdx, pSedName, idxSeq);
    if (p2DSeqObj) p2DSeqObj->shiftBase(fShiftX, fShiftY);
    return p2DSeqObj;
}

C2DSeqObj* C2DSeqCtrl::replaceFntShift(UINT nIdx, const CHAR* pLadName, UINT idxUnit, C2DFnt* p2DFnt, FLOAT fShiftX,
                                       FLOAT fShiftY) {
    C2DSeqObj* p2DSeqObj = replaceFnt(nIdx, pLadName, idxUnit, p2DFnt);
    if (p2DSeqObj) p2DSeqObj->shiftBase(fShiftX, fShiftY);
    return p2DSeqObj;
}

C2DSeqObj* C2DSeqCtrl::replaceFntShift(UINT nIdx, const CHAR* pSedName, C2DFnt* p2DFnt, FLOAT fShiftX, FLOAT fShiftY) {
    C2DSeqObj* p2DSeqObj = replaceFnt(nIdx, pSedName, p2DFnt);
    if (p2DSeqObj) p2DSeqObj->shiftBase(fShiftX, fShiftY);
    return p2DSeqObj;
}

C2DSeqObj* C2DSeqCtrl::shiftSeqPos(UINT nIdx, FLOAT fShiftX, FLOAT fShiftY) {
    if (!(nIdx < m_nSeq)) return NULL;
    C2DSeqObj* p2DSeqObj = C2DMan::getSeqObj(m_hSeqHandle[nIdx]);
    if (p2DSeqObj) p2DSeqObj->shiftBase(fShiftX, fShiftY);
    return p2DSeqObj;
}

C2DSeqObj* C2DSeqCtrl::setFntPos(UINT nIdx, FLOAT fShiftX, FLOAT fShiftY) { return shiftSeqPos(nIdx, fShiftX, fShiftY); }

void C2DSeqCtrl::enableDisp(BOOL bEnable) {
    m_bEnable = bEnable;
    for (INT i = 0; i < m_nSeq; i++) {
        C2DMan::enableDisp(m_hSeqHandle[i], (m_bitEnable & m_bitTbl[i]) ? bEnable & 1 : FALSE);
    }
}

void C2DSeqCtrl::enableDispForce(BOOL bEnable) {
    m_bEnable = bEnable;
    enableDispEach(bEnable);
}

void C2DSeqCtrl::enableDispEach(BOOL bEnable) {
    for (INT i = 0; i < m_nSeq; i++) {
        if (bEnable) {
            m_bitEnable |= m_bitTbl[i];
        } else {
            m_bitEnable &= ~m_bitTbl[i];
        }
        C2DMan::enableDisp(m_hSeqHandle[i], bEnable);
    }
}

void C2DSeqCtrl::enableDisp(INT nIdx, BOOL bEnable) {
    if (nIdx < m_nSeq) {
        if (bEnable > 0) {
            m_bitEnable |= m_bitTbl[nIdx];
        } else {
            m_bitEnable &= ~m_bitTbl[nIdx];
        }
        C2DMan::enableDisp(m_hSeqHandle[nIdx], bEnable < 0 ? bEnable : m_bEnable & bEnable);
    }
}

BOOL C2DSeqCtrl::isEnableDisp(INT nIdx) {
    if (!(nIdx < m_nSeq)) return FALSE;
    return m_bitEnable & m_bitTbl[nIdx];
}

void C2DSeqCtrl::setMatrix() {
    FMATRIX mat;
    vu0UnitMatrix(mat);
    mat.m[0][0] = m_seqControl.vScl.x;
    mat.m[1][1] = m_seqControl.vScl.y;
    mat.m[2][2] = m_seqControl.vScl.z;
    RotMatrix(m_seqControl.matrix, mat, m_seqControl.vRot);
    m_seqControl.matrix.setTrans(m_seqControl.vPos);
}

void C2DSeqCtrl::setPos(FVECTOR& vPos) {
    m_seqControl.vPos.x = vPos.x - 256.0f;
    m_seqControl.vPos.y = vPos.y - _ref(SCREEN_HEIGHT) / 2;
    m_seqControl.matrix.setTrans(m_seqControl.vPos);
}

void C2DSeqCtrl::setLocalPos(FVECTOR& vPos) {
    m_seqControl.vPos.x = vPos.x;
    m_seqControl.vPos.y = vPos.y;
    m_seqControl.matrix.setTrans(m_seqControl.vPos);
}

void C2DSeqCtrl::set3DPos(FVECTOR& vPos) {
    m_seqControl.vPos = vPos;
    m_seqControl.vPos.w = 1.0f;
}

void C2DSeqCtrl::setRot(FVECTOR& vRot) {
    m_seqControl.vRot = vRot;
    setMatrix();
}

void C2DSeqCtrl::setScl(FVECTOR& vScl) {
    m_seqControl.vScl = vScl;
    setMatrix();
}

void C2DSeqCtrl::setScl(FLOAT fScl) {
    m_seqControl.vScl.set(fScl, fScl, fScl, 0.0f);
    setMatrix();
}

void C2DSeqCtrl::setCol(FVECTOR& vCol) { m_seqControl.vCol = vCol; }

void C2DSeqCtrl::addPosX(FLOAT fPosX) {
    m_seqControl.vPos.x += fPosX;
    m_seqControl.matrix.setTrans(m_seqControl.vPos);
}

void C2DSeqCtrl::addPosY(FLOAT fPosY) {
    m_seqControl.vPos.y += fPosY;
    m_seqControl.matrix.setTrans(m_seqControl.vPos);
}

void C2DSeqCtrl::addRotX(FLOAT fRotX) {
    m_seqControl.vRot.x += fRotX;
    m_seqControl.vRot.x = Math3D::LimitPI(m_seqControl.vRot.x);
    setMatrix();
}

void C2DSeqCtrl::addRotY(FLOAT fRotY) {
    m_seqControl.vRot.y += fRotY;
    m_seqControl.vRot.y = Math3D::LimitPI(m_seqControl.vRot.y);
    setMatrix();
}

void C2DSeqCtrl::addRotZ(FLOAT fRotZ) {
    m_seqControl.vRot.z += fRotZ;
    m_seqControl.vRot.z = Math3D::LimitPI(m_seqControl.vRot.z);
    setMatrix();
}

void C2DSeqCtrl::changeTime(UINT nIdx, UINT inTime) {
    if (nIdx < m_nSeq) C2DMan::changeSeqTime(m_hSeqHandle[nIdx], inTime);
}

TIMEQ C2DSeqCtrl::getTime(UINT nIdx) {
    if (!(nIdx < m_nSeq)) return 0;
    return C2DMan::getTime(m_hSeqHandle[nIdx]);
}

void C2DSeqCtrl::changeTime(UINT inTime) {
    for (INT i = 0; i < m_nSeq; i++) {
        C2DMan::changeSeqTime(m_hSeqHandle[i], inTime);
    }
}

void C2DSeqCtrl::changeLayer(UINT nIdx, INT nLayer) {
    if (nIdx < m_nSeq) C2DMan::changeSeqLayer(m_hSeqHandle[nIdx], nLayer);
}

void C2DSeqCtrl::changeLayer(INT nLayer) {
    m_nLayer = nLayer;
    for (INT i = 0; i < m_nSeq; i++) {
        C2DMan::changeSeqLayer(m_hSeqHandle[i], nLayer);
    }
}

void C2DSeqCtrl::restart(UINT nIdx, FLOAT fStart) {
    if (nIdx < m_nSeq) C2DMan::restartSeq(m_hSeqHandle[nIdx], fStart);
}

BOOL C2DSeqCtrl::isEnd(UINT nIdx) {
    if (!(nIdx < m_nSeq)) return FALSE;
    return C2DMan::isEnd(m_hSeqHandle[nIdx]);
}

void C2DSeqCtrl::changeTexture(UINT nIdx, INT inSprIdx, CHAR* pTexName, G_UV* pStart, G_UV* pEnd) {
    if (nIdx < m_nSeq) C2DMan::changeSeqTexture(m_hSeqHandle[nIdx], inSprIdx, pTexName, pStart, pEnd);
}

void C2DSeqCtrl::changeTexture(UINT nIdx, INT inSeqIdx, INT inSprIdx, CHAR* pTexName, G_UV* pStart, G_UV* pEnd) {
    if (nIdx < m_nSeq) C2DMan::changeSeqTexture(m_hSeqHandle[nIdx], inSeqIdx, inSprIdx, pTexName, pStart, pEnd);
}

void C2DSeqCtrl::setParent(C2DSeqCtrl* p2DSeqCtrl) { m_seqControl.pParentSeqControl = &p2DSeqCtrl->m_seqControl; }

}  // namespace D2

// SOURCE DATA: .data .rodata
