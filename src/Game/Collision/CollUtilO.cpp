#include "Game/Collision/CollUtilA.h"
#include "System/HeapMan.h"
#include "System/MainApp.h"
#define COLLUTIL_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollUtil.h" /* MATCH: the original's path in CollUtil.h's strings */
#include "Game/Collision/CollUtil.h"
#include "Game/Collision/CollObj.h"
#include "include_asm.h"

#include "System/Coord.h"

void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);

#include "System/Math3D.h"





static inline void _scaleAdd(FVECTOR& v, const FVECTOR& d, FLOAT t, const FVECTOR& o) {
    ScaleVector(v, d, t);
    AddVector(v, o, v);
}

static inline FLOAT _distance(FVECTOR& v, const FVECTOR& p0, const FVECTOR& p1) {
    SubVector(v, p0, p1);
    return _sqrtf(InnerProduct(v));
}

INT CCollUtil::_detectSpherePoly3(CSphereCollision& sphereColl0, CPoly3Collision& poly3Coll1) {
    INT ret;
    FVECTOR sphereWS;
    FVECTOR sphereWE;
    FVECTOR vec;
    FVECTOR vec0;
    FVECTOR vec2; // MATCH: vec2/norm2 declared before vec1/norm1 (the ref's order swaps their stack slots)
    FVECTOR vec1;
    FVECTOR norm;
    FVECTOR norm0;
    FVECTOR norm2;
    FVECTOR norm1;
    FLOAT d;
    FLOAT t1;

    poly3Coll1.m_outWE = poly3Coll1.m_inWE;
    m_pCheckColl->m_inWS = poly3Coll1.m_inWS;
    m_pCheckColl->m_inWE = poly3Coll1.m_inWE;
    m_pCheckColl->m_Radius = poly3Coll1.m_Radius;
    m_fgSliding = 0;
    while (!(ret = _detectSphereSphere(sphereColl0, *(CSphereCollision*)m_pCheckColl)) && (m_pCheckColl->m_inWS = poly3Coll1.m_BasePos, !(ret = _detectSphereSphere(sphereColl0, *(CSphereCollision*)m_pCheckColl))) &&
           (m_pCheckColl->m_inWE = poly3Coll1.m_inWS, !(ret = _detectSphereSphere(sphereColl0, *(CSphereCollision*)m_pCheckColl)))) {
        sphereWS = sphereColl0.m_inWE;
        sphereWS.y += sphereColl0.m_shiftY;
        sphereWE = sphereWS;
        sphereWS.y -= sphereColl0.m_Radius;
        sphereWE.y += sphereColl0.m_Radius;
        SubVector(vec0, poly3Coll1.m_inWS, poly3Coll1.m_BasePos);
        SubVector(vec1, poly3Coll1.m_BasePos, poly3Coll1.m_inWE);
        OuterProduct(norm, vec1, vec0);
        if (norm.x == 0.0f && norm.y == 0.0f && norm.z == 0.0f) goto end;
        d = InnerProduct(norm, poly3Coll1.m_BasePos);
        t1 = InnerProduct(norm, sphereWS) - d;
        t1 = -t1 / InnerProduct(norm);
        ScaleVector(vec, norm, t1);
        AddVector(vec, sphereWS, vec);
        OuterProduct(norm0, norm, vec0);
        SubVector(vec0, vec, poly3Coll1.m_BasePos);
        if (InnerProduct(norm0, vec0) < 0.0f) goto end;
        OuterProduct(norm1, norm, vec1);
        SubVector(vec1, vec, poly3Coll1.m_inWE);
        if (InnerProduct(norm1, vec1) < 0.0f) goto end;
        SubVector(vec2, poly3Coll1.m_inWE, poly3Coll1.m_inWS);
        OuterProduct(norm2, norm, vec2);
        SubVector(vec2, vec, poly3Coll1.m_inWS);
        if (InnerProduct(norm2, vec2) < 0.0f) goto end;
        m_pCheckColl->m_inWE = vec;
        ret = _detectSphereSphere(sphereColl0, *(CSphereCollision*)m_pCheckColl);
        break;
    }
end:
    if (!ret) return 0;
hit:
    m_vCk2.w = 1.0f;
    m_vNormal.w = 1.0f;
    poly3Coll1.m_outWE = m_vCk2;
    poly3Coll1.m_outNormal = m_vNormal;
    sphereColl0.m_outNormal = m_vNormal;
    poly3Coll1.m_outDetect = m_vDetect;
    sphereColl0.m_outDetect = m_vDetect;
    return 1;
}
INT CCollUtil::_detectColumnPoly3(CColumnCollision& columnColl0, CPoly3Collision& poly3Coll1) {
    INT ret;
    FVECTOR columnWS;
    FVECTOR columnWE;
    FVECTOR vec;
    FVECTOR vec0;
    FVECTOR vec2; // MATCH: vec2/norm2 declared before vec1/norm1 (the ref's order swaps their stack slots)
    FVECTOR vec1;
    FVECTOR norm;
    FVECTOR norm0;
    FVECTOR norm2;
    FVECTOR norm1;
    FLOAT d;
    FLOAT t1;

    poly3Coll1.m_outWE = poly3Coll1.m_inWE;
    m_pCheckColl->m_inWS = poly3Coll1.m_inWS;
    m_pCheckColl->m_inWE = poly3Coll1.m_inWE;
    m_pCheckColl->m_Radius = poly3Coll1.m_Radius;
    while (!(ret = _detectColumnSphere(columnColl0, *(CSphereCollision*)m_pCheckColl, 0)) && (m_pCheckColl->m_inWS = poly3Coll1.m_BasePos, !(ret = _detectColumnSphere(columnColl0, *(CSphereCollision*)m_pCheckColl, 0))) &&
           (m_pCheckColl->m_inWE = poly3Coll1.m_inWS, !(ret = _detectColumnSphere(columnColl0, *(CSphereCollision*)m_pCheckColl, 0)))) {
        columnWS = columnColl0.m_inWS;
        columnWE = columnColl0.m_inWE;
        columnWS.y += columnColl0.m_shiftY;
        columnWE.y += columnColl0.m_shiftY;
        SubVector(vec0, poly3Coll1.m_inWS, poly3Coll1.m_BasePos);
        SubVector(vec1, poly3Coll1.m_BasePos, poly3Coll1.m_inWE);
        OuterProduct(norm, vec1, vec0);
        if (norm.x == 0.0f && norm.y == 0.0f && norm.z == 0.0f) goto end;
        d = InnerProduct(norm, poly3Coll1.m_BasePos);
        SubVector(vec, columnWE, columnWS);
        t1 = InnerProduct(norm, columnWS) - d;
        t1 = -t1 / InnerProduct(norm, vec);
        ScaleVector(vec, vec, t1);
        AddVector(vec, columnWS, vec);
        OuterProduct(norm0, norm, vec0);
        SubVector(vec0, vec, poly3Coll1.m_BasePos);
        if (InnerProduct(norm0, vec0) < 0.0f) goto end;
        OuterProduct(norm1, norm, vec1);
        SubVector(vec1, vec, poly3Coll1.m_inWE);
        if (InnerProduct(norm1, vec1) < 0.0f) goto end;
        SubVector(vec2, poly3Coll1.m_inWE, poly3Coll1.m_inWS);
        OuterProduct(norm2, norm, vec2);
        SubVector(vec2, vec, poly3Coll1.m_inWS);
        if (InnerProduct(norm2, vec2) < 0.0f) goto end;
        m_pCheckColl->m_inWE = vec;
        ret = _detectColumnSphere(columnColl0, *(CSphereCollision*)m_pCheckColl, 0);
        break;
    }
end:
    if (!ret) return 0;
    m_vCk2.w = 1.0f;
    m_vNormal.w = 1.0f;
    poly3Coll1.m_outWE = m_vCk2;
    poly3Coll1.m_outNormal = m_vNormal;
    columnColl0.m_outNormal = m_vNormal;
    poly3Coll1.m_outDetect = m_vDetect;
    columnColl0.m_outDetect = m_vDetect;
    return 1;
}
INT CCollUtil::_detectColumnPoint(CColumnCollision& columnColl0, CPointCollision& pointColl1) {
    FVECTOR p0;
    FVECTOR p1;
    pointColl1.m_outWE = pointColl1.m_inWE;
    m_fgSliding = 0;
    m_vCk0 = pointColl1.m_inWS;
    m_vCk1 = pointColl1.m_inWE;
    SubVector(m_vCkVec, m_vCk1, m_vCk0);
    m_Radius0 = columnColl0.m_Radius;
    m_Radius1 = columnColl0.m_Radius;
    m_Radius = m_Radius0;
    m_Radius2 = m_Radius * m_Radius;
    m_RadiusPlus = m_Radius + 0.0035f;
    m_RadiusHosei = m_RadiusPlus / m_Radius;
    m_CkVecl = InnerProduct(m_vCkVec);
    m_CkVecl2 = _sqrtf(m_CkVecl);
    p0 = columnColl0.m_inWS;
    p1 = columnColl0.m_inWE;
    p0.y += columnColl0.m_shiftY;
    p1.y += columnColl0.m_shiftY;
    m_pPrimInfoCurrent = 0;
    do {} while (0); // MATCH: scheduling barrier (loop notes) before the distanceLineO arguments, an empty macro in the original?
    if (distanceLineO(&p0, &p1, 0) || distanceSphereO(&p1)) {
        m_vCk2.w = 1.0f;
        m_vNormal.w = 1.0f;
        pointColl1.m_outWE = m_vCk2;
        pointColl1.m_outNormal = m_vNormal;
        columnColl0.m_outNormal = m_vNormal;
        pointColl1.m_outDetect = m_vDetect;
        columnColl0.m_outDetect = m_vDetect;
        return 1;
    }
    return 0;
}
INT CCollUtil::_detectColumnSphere(CColumnCollision& columnColl0, CSphereCollision& sphereColl1, INT fg) {
    FVECTOR p0;
    FVECTOR p1;
    INT ret;
    sphereColl1.m_outWE = sphereColl1.m_inWE;
    m_fgSliding = 0;
    m_vCk0 = sphereColl1.m_inWS;
    m_vCk1 = sphereColl1.m_inWE;
    m_vCk0.y += sphereColl1.m_shiftY;
    m_vCk1.y += sphereColl1.m_shiftY;
    SubVector(m_vCkVec, m_vCk1, m_vCk0);
    m_Radius0 = columnColl0.m_Radius;
    m_Radius1 = sphereColl1.m_Radius;
    m_Radius = m_Radius0 + m_Radius1;
    m_Radius2 = m_Radius * m_Radius;
    m_RadiusPlus = m_Radius + 0.0035f;
    m_RadiusHosei = m_RadiusPlus / m_Radius;
    m_CkVecl = InnerProduct(m_vCkVec);
    m_CkVecl2 = _sqrtf(m_CkVecl);
    p0 = columnColl0.m_inWS;
    p1 = columnColl0.m_inWE;
    p0.y += columnColl0.m_shiftY;
    p1.y += columnColl0.m_shiftY;
    m_pPrimInfoCurrent = 0;
    do {} while (0); // MATCH: scheduling barrier, as in _detectColumnPoint
    ret = distanceLineO(&p0, &p1, 1);
    if (!ret && !fg) {
        ret = distanceSphereO(&p1);
    }
    if (ret) {
        m_vCk2.w = 1.0f;
        m_vNormal.w = 1.0f;
        sphereColl1.m_outWE = m_vCk2;
        sphereColl1.m_outNormal = m_vNormal;
        columnColl0.m_outNormal = m_vNormal;
        sphereColl1.m_outDetect = m_vDetect;
        columnColl0.m_outDetect = m_vDetect;
        return 1;
    }
    return 0;
}
INT CCollUtil::_detectSpherePoint(CSphereCollision& sphereColl0, CPointCollision& pointColl1) {
    FVECTOR p0;
    pointColl1.m_outWE = pointColl1.m_inWE;
    m_fgSliding = 0;
    m_vCk0 = pointColl1.m_inWS;
    m_vCk1 = pointColl1.m_inWE;
    SubVector(m_vCkVec, m_vCk1, m_vCk0);
    m_Radius0 = sphereColl0.m_Radius;
    m_Radius = m_Radius0;
    m_Radius1 = 0.0f;
    m_Radius2 = m_Radius * m_Radius;
    m_RadiusPlus = m_Radius + 0.0035f;
    m_RadiusHosei = m_RadiusPlus / m_Radius;
    m_CkVecl = InnerProduct(m_vCkVec);
    m_CkVecl2 = _sqrtf(m_CkVecl);
    p0 = sphereColl0.m_inWE;
    p0.y += sphereColl0.m_shiftY;
    if (distanceSphereO(&p0)) {
    m_vCk2.w = 1.0f;
    m_vNormal.w = 1.0f;
    pointColl1.m_outWE = m_vCk2;
    pointColl1.m_outNormal = m_vNormal;
    sphereColl0.m_outNormal = m_vNormal;
    pointColl1.m_outDetect = m_vDetect;
    sphereColl0.m_outDetect = m_vDetect;
    return 1;
    }
    return 0;
}
INT CCollUtil::_detectSphereSphere(CSphereCollision& sphereColl0, CSphereCollision& sphereColl1) {
    FVECTOR p0;
    sphereColl1.m_outWE = sphereColl1.m_inWE;
    m_fgSliding = 0;
    m_vCk0 = sphereColl1.m_inWS;
    m_vCk1 = sphereColl1.m_inWE;
    SubVector(m_vCkVec, m_vCk1, m_vCk0);
    m_Radius0 = sphereColl0.m_Radius;
    m_Radius1 = sphereColl1.m_Radius;
    m_Radius = m_Radius0 + m_Radius1;
    m_Radius2 = m_Radius * m_Radius;
    m_RadiusPlus = m_Radius + 0.0035f;
    m_RadiusHosei = m_RadiusPlus / m_Radius;
    m_CkVecl = InnerProduct(m_vCkVec);
    m_CkVecl2 = _sqrtf(m_CkVecl);
    p0 = sphereColl0.m_inWE;
    p0.y += sphereColl0.m_shiftY;
    if (distanceSphereO(&p0)) {
    m_vCk2.w = 1.0f;
    m_vNormal.w = 1.0f;
    sphereColl1.m_outWE = m_vCk2;
    sphereColl1.m_outNormal = m_vNormal;
    sphereColl0.m_outNormal = m_vNormal;
    sphereColl1.m_outDetect = m_vDetect;
    sphereColl0.m_outDetect = m_vDetect;
    return 1;
    }
    return 0;
}
/* MATCH: Math3D.h:129's inline with an early-clobber output (the header's _sqrtf changes distanceLineO's registers) */
static inline FLOAT _sqrtfE(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=&f"(ret) : "f"(x));
    return ret;
}

static inline FLOAT _distanceE(FVECTOR& v, const FVECTOR& p0, const FVECTOR& p1) {
    SubVector(v, p0, p1);
    return _sqrtfE(InnerProduct(v));
}

static inline INT _normalizeQ(FVECTOR& v) {
    FLOAT l = _sqrtfE(InnerProduct(v));
    DivVector(v, v, l);
    return l != 0.0f;
}

INT CCollUtil::distanceLineO(FVECTOR* pP0, FVECTOR* pP1, INT fg) {
    FVECTOR p;
    FVECTOR v0;
    FVECTOR s0;
    FVECTOR s1;
    FVECTOR vec0;
    FLOAT pv0;
    FLOAT pv1;
    FLOAT v00;
    FLOAT v11;
    FLOAT v01;
    FLOAT denom;
    FLOAT s;
    FLOAT t;
    FVECTOR v;
    FVECTOR u;
    FVECTOR su;
    INT ret;
    FVECTOR s2;
    FLOAT ss;
    FVECTOR qs;
    FVECTOR pq;
    FVECTOR nvec;
    FVECTOR vSurf;
    FLOAT f0;
    FLOAT f1;
    FLOAT m;
    FLOAT l2;

    SubVector(v0, *pP1, *pP0);
    SubVector(p, m_vCk0, *pP0);
    pv0 = InnerProduct(p, v0);
    pv1 = InnerProduct(p, m_vCkVec);
    v00 = InnerProduct(v0);
    if (v00 == 0.0f) return 0;
    v11 = m_CkVecl;
    v01 = InnerProduct(v0, m_vCkVec);
    denom = v00 * v11 - v01 * v01;
    ret = 1;
    for (;;) {
    if (denom < -1e-05f || 1e-05f <= denom) {
        s = (pv0 * v01 - pv1 * v00) / denom;
        if (s < 0.0f) break;
        t = (pv0 * v11 - pv1 * v01) / denom;
    } else {
        t = pv0 / v00;
        s = 0.0f;
    }
    ScaleVector(vec0, v0, t);
    AddVector(s0, *pP0, vec0);
    ScaleVector(vec0, m_vCkVec, s);
    AddVector(s1, m_vCk0, vec0);
    f0 = _distanceE(qs, s1, s0);
    if (m_Radius < f0) break;
    if (!fg) {
        m = _sqrtfE(m_Radius2 - f0 * f0);
        SubVector(pq, s1, s0);
        OuterProduct(qs, v0, pq);
        if (_normalizeQ(qs) && 1e-05f <= f0) {
            if (0.0f < InnerProduct(qs, m_vCkVec)) ScaleVector(qs, qs, -1.0f);
            ScaleVector(qs, qs, m);
            Normalize(su, v0);
            if (0.0f < InnerProduct(su, m_vCkVec)) ScaleVector(su, su, -1.0f);
            f1 = _sqrtfE(v00);
            l2 = m_CkVecl2;
            if (1e-05f <= v01 || v01 < -1e-05f) {
                f1 = f1 * l2 / v01;
                ScaleVector(su, su, m / _sqrtfE(f1 * f1 - 1.0f));
            } else {
                su._q = 0;
            }
        } else {
            Normalize(v, v0);
            if (0.0f < InnerProduct(v, m_vCkVec)) ScaleVector(v, v, -1.0f);
            f1 = _sqrtfE(v00);
            l2 = m_CkVecl2;
            if (1e-05f <= v01 || v01 < -1e-05f) {
                f1 = f1 * l2 / v01;
                m = m / _sqrtfE(f1 * f1 - 1.0f);
                ScaleVector(su, v, m);
                m = m * f1;
            } else {
                m = m_Radius;
                su.zero();
            }
            Normalize(qs, m_vCkVec);
            ScaleVector(qs, qs, -m);
            SubVector(qs, qs, su);
        }
    } else {
        qs.zero();
        su.zero();
    }
    AddVector(vec0, qs, su);
    AddVector(u, s1, vec0);
    SubVector(vec0, u, m_vCk0);
    if (InnerProduct(m_vCkVec, vec0) < 0.0f) break;
    SubVector(vec0, u, m_vCk1);
    if (0.0f < InnerProduct(m_vCkVec, vec0)) break;
    AddVector(v, s0, su);
    SubVector(vec0, v, *pP0);
    if (InnerProduct(v0, vec0) < 0.0f) break;
    SubVector(vec0, v, *pP1);
    if (0.0f < InnerProduct(v0, vec0)) break;
    SubVector(nvec, v, u);
    Normalize(nvec, nvec);
    m_vCk2 = u;
    m_vCk2.w = 1.0f;
    m_vNormal = nvec;
    m_vDetect2 = m_vCk2;
    m_vDetect = m_vCk2;
    return 1;
    }
    for (;;) {
        SubVector(vec0, m_vCk0, *pP0);
        ss = InnerProduct(vec0, v0) / v00;
        ScaleVector(vec0, v0, ss);
        AddVector(s2, *pP0, vec0);
        if (_distanceE(qs, m_vCk0, s2) <= m_Radius && ss <= 1.0f && 0.0f <= ss) {
            m_vCk2 = m_vCk0;
            break;
        }
        SubVector(vec0, m_vCk1, *pP0);
        ss = InnerProduct(vec0, v0) / v00;
        ScaleVector(vec0, v0, ss);
        AddVector(s2, *pP0, vec0);
        if (_distanceE(qs, m_vCk1, s2) <= m_Radius && ss <= 1.0f && 0.0f <= ss) {
            m_vCk2 = m_vCk1;
            break;
        }
        SubVector(vec0, *pP0, m_vCk0);
        ss = InnerProduct(vec0, m_vCkVec) / m_CkVecl;
        ScaleVector(vec0, m_vCkVec, ss);
        AddVector(s2, m_vCk0, vec0);
        if (_distanceE(qs, *pP0, s2) <= m_Radius && ss <= 1.0f && 0.0f <= ss) {
            m_vCk2 = s2;
            break;
        }
        SubVector(vec0, *pP1, m_vCk0);
        ss = InnerProduct(vec0, m_vCkVec) / m_CkVecl;
        ScaleVector(vec0, m_vCkVec, ss);
        AddVector(s2, m_vCk0, vec0);
        if (_distanceE(qs, *pP1, s2) <= m_Radius && ss <= 1.0f && 0.0f <= ss) {
            m_vCk2 = s2;
            break;
        }
        ret = 0;
        break;
    }
    if (ret) {
        m_vDetect2 = m_vCk2;
        SubVector(qs, m_vCk2, s2);
        Normalize(qs, qs);
        m_vNormal = qs;
        ScaleVector(qs, qs, m_Radius);
        AddVector(m_vCk2, s2, qs);
        m_vDetect = m_vCk2;
        return 1;
    }
    return 0;
}
INT CCollUtil::distanceSphereO(FVECTOR* pP0) {
    FVECTOR vc0;
    FVECTOR vc1;
    FLOAT l;
    FVECTOR p;
    FVECTOR s;
    FVECTOR vec0;
    SubVector(vc0, *pP0, m_vCk0);
    SubVector(vc1, *pP0, m_vCk1);
    if (_sqrtf(InnerProduct(vc0)) < m_Radius) {
        SubVector(vc0, m_vCk0, *pP0);
        Normalize(m_vNormal, vc0);
        ScaleVector(vc1, m_vNormal, m_RadiusHosei);
        AddVector(m_vCk2, *pP0, vc1);
        ScaleVector(vc1, m_vNormal, m_Radius0);
        AddVector(m_vDetect, *pP0, vc1);
        m_vDetect2 = *pP0;
        return 1;
    }
    l = InnerProduct(vc0, m_vCkVec) / m_CkVecl;
    ScaleVector(p, m_vCkVec, l);
    AddVector(p, m_vCk0, p);
    l = _distance(s, *pP0, p);
    if (m_Radius <= l) {
        return 0;
    }
    FLOAT len = _sqrtf(m_Radius2 - l * l);
    Normalize(s, m_vCkVec);
    ScaleVector(s, s, len);
    SubVector(s, p, s);
    SubVector(vec0, s, m_vCk0);
    if (InnerProduct(m_vCkVec, vec0) < 0.0f) {
        return 0;
    }
    SubVector(vec0, s, m_vCk1);
    if (InnerProduct(m_vCkVec, vec0) >= 0.0f) {
        return 0;
    }
    SubVector(p, s, *pP0);
    Normalize(m_vNormal, p);
    ScaleVector(vec0, m_vNormal, m_RadiusHosei);
    AddVector(m_vCk2, *pP0, vec0);
    ScaleVector(vec0, m_vNormal, m_Radius0);
    AddVector(m_vDetect, *pP0, vec0);
    m_vDetect2 = s;
    return 1;
}
INT CCollUtil::_detectColumnColumnEz(CColumnCollision& columnColl0, CColumnCollision& columnColl1) {
    FVECTOR v0;
    FLOAT r0;
    FLOAT r1;
    FLOAT y00;
    FLOAT y01;
    FLOAT y10;
    FLOAT y11;
    FVECTOR v1;
    v0.x = columnColl1.m_inWE.x - columnColl0.m_inWE.x;
    v0.z = columnColl1.m_inWE.z - columnColl0.m_inWE.z;
    r0 = v0.x * v0.x + v0.z * v0.z;
    r1 = columnColl0.m_Radius + columnColl1.m_Radius;
    if (r1 * r1 < r0) {
        return 0;
    }
    if (columnColl0.m_Type == COLL_TYPE_SPHERE) {
        y00 = columnColl0.m_inWE.y - columnColl0.m_Radius + columnColl0.m_shiftY;
        y01 = columnColl0.m_inWE.y + columnColl0.m_Radius + columnColl0.m_shiftY;
    } else {
        y00 = columnColl0.m_inWS.y + columnColl0.m_shiftY;
        y01 = columnColl0.m_inWE.y + columnColl0.m_shiftY;
    }
    if (columnColl1.m_Type == COLL_TYPE_SPHERE) {
        y10 = columnColl1.m_inWE.y - columnColl1.m_Radius + columnColl1.m_shiftY;
        y11 = columnColl1.m_inWE.y + columnColl1.m_Radius + columnColl1.m_shiftY;
    } else {
        y10 = columnColl1.m_inWS.y + columnColl1.m_shiftY;
        y11 = columnColl1.m_inWE.y + columnColl1.m_shiftY;
    }
    if (y11 < y00 || y01 < y10) {
        return 0;
    }
    v0.y = 0.0f;
    r1 += 0.0035f;
    Normalize(v0, v0);
    if (v0.x == 0.0f && v0.z == 0.0f) {
        v0.z = 1.0f;
    }
    ScaleVector(v0, v0, r1);
    if (columnColl0.m_Type == COLL_TYPE_SPHERE) {
        v1 = columnColl0.m_inWE;
    } else {
        v1 = columnColl0.m_inWS;
    }
    if (columnColl1.m_Type == COLL_TYPE_SPHERE) {
        y10 = columnColl1.m_inWE.y;
    } else {
        y10 = columnColl1.m_inWS.y;
    }
    columnColl1.m_outWE.x = v1.x + v0.x;
    columnColl1.m_outWE.y = y10;
    columnColl1.m_outWE.z = v1.z + v0.z;
    columnColl1.m_outWE.w = 1.0f;
    return 1;
}
INT CCollUtil::_detectColumnColumn(CColumnCollision& columnColl0, CColumnCollision& columnColl1) {
    CSphereCollision sphereColl0 = (CSphereCollision&)columnColl0;
    INT ret;
    ret = _detectColumnSphere(columnColl1, sphereColl0, 1);
    if (ret) {
        sphereColl0.m_inWE = sphereColl0.m_inWS = m_vDetect2;
        sphereColl0.m_inWS.y = columnColl1.m_inWS.y;
        sphereColl0.m_inWE.y = columnColl1.m_inWE.y;
        ret = _detectColumnColumnEz((CColumnCollision&)sphereColl0, columnColl1);
    }
    return ret;
}
INT CCollUtil::_detectColumnColumnR(CColumnCollision& columnColl0, CColumnCollision& columnColl1) {
    INT ret;
    FVECTOR v0;
    ret = _detectColumnColumn(columnColl1, columnColl0);
    if (ret) {
        SubVector(v0, columnColl0.m_inWE, columnColl0.m_outWE);
        AddVector(columnColl1.m_outWE, columnColl1.m_inWE, v0);
    }
    return ret;
}
INT CCollUtil::_detectBBoxPointEZ(COLL_CUBE& collCube, CCollision& collision) {
    FVECTOR v0;
    FVECTOR v1;
    INT ret;
    FMATRIX invMat;
    v0 = collision.m_inWS;
    v1 = collision.m_inWE;
    if (collCube.pMatrix) {
        InverseMatrix(invMat, *collCube.pMatrix);
        ApplyMatrix(v0, invMat, v0);
        ApplyMatrix(v1, invMat, v1);
    }
    ret = 0;
    if (collCube.vMin.x <= v1.x && v1.y > collCube.vMin.y && collCube.vMin.x <= v1.z && v1.x <= collCube.vMax.x &&
        v1.y <= collCube.vMax.y && v1.z <= collCube.vMax.x) {
        ret = 1;
    }
    if (collCube.vMin.x <= v0.x && v0.y > collCube.vMin.y && collCube.vMin.x <= v0.z && v0.x <= collCube.vMax.x &&
        v0.y <= collCube.vMax.y && v0.z <= collCube.vMax.x) {
        ret |= 2;
    }
    return ret;
}
INT CCollUtil::_detectSphereCircle(CSphereCollision& sphereColl0, CCircleCollision& circleColl1) {
    FVECTOR v0;
    FLOAT r0;
    FLOAT r1;
    FVECTOR sphereCenter;
    INT fgColumn;
    FLOAT y0;
    FLOAT d;
    FLOAT t1;
    FLOAT t2;
    fgColumn = 0;
    if (!(sphereColl0.m_Type == COLL_TYPE_COLUMN || sphereColl0.m_Type == COLL_TYPE_COLUMNR)) {
        sphereCenter = (sphereColl0.m_Type == COLL_TYPE_SPHERE) ? sphereColl0.m_inWE : sphereColl0.m_inWS;
    } else {
        fgColumn = 1;
        y0 = circleColl1.m_inWS.y;
        SubVector(sphereCenter, sphereColl0.m_inWE, sphereColl0.m_inWS);
        d = -(sphereColl0.m_inWS.y - y0) / sphereCenter.y;
        ScaleVector(sphereCenter, sphereCenter, d);
        AddVector(sphereCenter, sphereColl0.m_inWS, sphereCenter);
    }
    v0.x = sphereCenter.x - circleColl1.m_inWS.x;
    v0.z = sphereCenter.z - circleColl1.m_inWS.z;
    r1 = circleColl1.m_Radius;
    r0 = sphereColl0.m_Radius;
    if ((r1 + r0) * (r1 + r0) <= v0.x * v0.x + v0.z * v0.z) {
        return 0;
    }
    if (!fgColumn) {
        d = sphereCenter.y + sphereColl0.m_shiftY - circleColl1.m_inWS.y;
        if (d < 0.0f) d = -d;
        if (r0 < d) {
            return 0;
        }
    } else {
        t1 = sphereColl0.m_inWS.y;
        t2 = sphereColl0.m_inWE.y;
        if (t1 < t2) {
            if (sphereCenter.y < t1 || t2 < sphereCenter.y) {
                return 0;
            }
        } else {
            if (sphereCenter.y < t2 || t1 < sphereCenter.y) {
                return 0;
            }
        }
    }
    circleColl1.m_outDetect = sphereCenter;
    circleColl1.m_outDetect.y = circleColl1.m_inWS.y;
    sphereColl0.m_outDetect = circleColl1.m_outDetect;
    return 1;
}
// SOURCE DATA: .rodata
