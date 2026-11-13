/* CEffect's inline members (the original defines them in effect.h), in the order effect.cpp emits them. */
#ifndef SYSTEM_GRAPHIC_EFFECT_EFFECT_INL_H
#define SYSTEM_GRAPHIC_EFFECT_EFFECT_INL_H

inline HANDLE CEffect::_getHandle() const { return m_hHandle; }
inline void CEffect::_setHandle(HANDLE h) { m_hHandle = h; }
inline void CEffect::setType(ObjType::eObjType eType) { m_eType = eType; }
inline ObjType::eObjType CEffect::getType() { return m_eType; }
inline CCoord& CEffect::Crd(INT nNum) { return m_pCrds->Crd(nNum); }
inline CCoord& CEffect::Crd(INT nNum) const { return m_pCrds->Crd(nNum); }
inline INT CEffect::getCoordNum() const { return m_pCrds->getCoordNum(); }
inline FVECTOR& CEffect::scale() { return m_vScale; }
inline void CEffect::setScale(const FVECTOR& vScale) { m_vScale = vScale; }
inline void CEffect::setScale(FLOAT fScale) {
    m_vScale.x = m_vScale.y = m_vScale.z = fScale;
}
inline FVECTOR& CEffect::color() { return m_vColor; }
inline void CEffect::setAttachNoRot(BOOL bVal) {
    if (bVal) {
        m_iFlag |= 4;
    } else {
        m_iFlag &= ~4;
    }
}
inline BOOL CEffect::isAttachNoRot() const { return m_iFlag & 4; }
inline void CEffect::setAttachReverseTrs(BOOL bVal) {
    if (bVal) {
        m_iFlag |= 8;
    } else {
        m_iFlag &= ~8;
    }
}
inline BOOL CEffect::isAttachReverseTrs() const { return m_iFlag & 8; }
inline void CEffect::setAttachNoScale(BOOL bVal) {
    if (bVal) {
        m_iFlag |= 0x80;
    } else {
        m_iFlag &= ~0x80;
    }
}
inline BOOL CEffect::isAttachNoScale() const { return m_iFlag & 0x80; }
inline void CEffect::setZOffset(FIX4 nZOffset) { m_nZOffset = nZOffset; }
inline FIX4 CEffect::getZOffset() { return m_nZOffset; }
inline FIX4 CEffect::getZSortEntityOffset() { return m_nEntZOfs; }
inline BOOL CEffect::isEntityAlpha() { return m_iFlag & 0x40; }

#line 500 EFFECT_H_PATH
inline BOOL CEffect::isStop() {
    KH_DEBUG(if (!(isValid())) AssertApp(__FILE__, __LINE__, "isValid()", 0);)
    return (m_iFlag & 0x200000) != 0;
}

#line 511 EFFECT_H_PATH
inline BOOL CEffect::isDrawing() {
    KH_DEBUG(if (!(isValid())) AssertApp(__FILE__, __LINE__, "isValid()", 0);)
    return (m_iFlag & 0x800000) == 0;
}

#line 526 EFFECT_H_PATH
inline BOOL CEffect::isActive() {
    KH_DEBUG(if (!(isValid())) AssertApp(__FILE__, __LINE__, "isValid()", 0);)
    return m_pPartMng != NULL;
}

#line 547 EFFECT_H_PATH
inline BOOL CEffect::isAlive() {
    KH_DEBUG(if (!(isValid())) AssertApp(__FILE__, __LINE__, "isValid()", 0);)
    return (m_iFlag & 0x10000) == 0;
}

inline void CEffect::setAttachFlag(BOOL bVal) {
    if (bVal) {
        m_iFlag |= 0x10;
    } else {
        m_iFlag &= ~0x10;
    }
}
inline BOOL CEffect::isAttachFlag() const { return m_iFlag & 0x10; }

#endif /* SYSTEM_GRAPHIC_EFFECT_EFFECT_INL_H */
