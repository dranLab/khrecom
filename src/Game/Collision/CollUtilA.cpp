#include "Game/Collision/CollUtilA.h"
#include "include_asm.h"

// PARTIAL: from System/Coord.h
void OuterProduct(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
FLOAT InnerProduct(const FVECTOR& v0, const FVECTOR& v1);
FLOAT InnerProduct(const FVECTOR& v0);
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);

INT CCollUtil::_detectActorTrapXZ(const ETMoveData& moveData, const EventTrapData& trapData) {
    FVECTOR* pVert;
    FLOAT h0;
    FLOAT h1;
    FLOAT height;
    INT fgHeight;
    INT ret;
    INT inCnt0;
    INT inCnt1;
    INT i;

    m_vCkS = moveData._vOldPos;
    m_vCkE = moveData._vNowPos;
    m_Radius = moveData._fRadius;
    if (m_Radius < 0.5f) {
        m_Radius = 0.5f;
    }
    pVert = trapData._pVert;
    h0 = pVert[0].y;
    h1 = pVert[1].y;
    height = h1 - h0;
    fgHeight = 0;
    if (height < 0.0f) {
        height = -height;
        h0 = h1;
    }
    h0 -= m_Radius;
    height += m_Radius * 2.0f;
    h1 = m_vCkS.y - h0;
    if (h1 >= 0.0f && h1 < height) {
        fgHeight = 1;
    }
    h1 = m_vCkE.y - h0;
    if (h1 >= 0.0f && h1 < height) {
        fgHeight |= 2;
    }
    if (!fgHeight) {
        return 0;
    }
    m_Radius2 = m_Radius * m_Radius;
    inCnt0 = 0;
    if (trapData._nType == 0) {
        ret = detect2DLineLine(pVert[0], pVert[1]);
    } else {
        inCnt1 = 0;
        ret = 0;
        for (i = 0; i < 4; i++) {
            ret |= detect2DLineLine(pVert[i], pVert[i == 3 ? 0 : i + 1]);
            inCnt0 += m_DetectStatus & 1;
            if (m_DetectStatus & 2) {
                inCnt1++;
            }
        }
        h0 += m_Radius;
        height -= m_Radius * 2.0f;
        h1 = m_vCkS.y - h0;
        if (h1 >= 0.0f && h1 < height) {
            fgHeight |= 4;
        }
        h1 = m_vCkE.y - h0;
        if (h1 >= 0.0f && h1 < height) {
            fgHeight |= 8;
        }
        if (inCnt0 == 4 && inCnt1 == inCnt0) {
            switch (fgHeight) {
            case 1:
                return 5;
            case 2:
                return 3;
            case 10:
            case 11:
                return 9;
            default:
                return 1;
            }
        }
        if (!ret) {
            return 0;
        }
        if ((ret & 6) == 6) {
            return 1;
        }
        if (ret & 1) {
            if (ret & 2) {
                ret &= ~2;
            } else if (ret & 4) {
                ret &= ~4;
            }
            if (fgHeight == 2) {
                ret |= 2;
            } else if (fgHeight == 1) {
                ret |= 4;
            }
        }
    }
    if (ret) {
        if (ret == 8) {
            ret = 14;
        } else {
            ret |= 1;
        }
    }
    return ret;
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollUtilA", detect2DLineLine__9CCollUtilRC7FVECTORT1);
// Nonmatching: the final 4/1 choice becomes "li v1,1; ... movn v0,v1,s5" instead of
// "beqz s5 (li v0,4); b (li v0,1)" (jump-pass conditional move); everything else matches.
// Tried (2026-10-20): all 6 orders of the three tests, nested ifs, else-if chain, a ret variable, the
// last value as an expression. "if (fg0 == 0 || fg1 == 0) return 0; return 1;" as the last test gives the
// branch form (no movn) but with the arms mirrored (bnez s5; li v0,1 / b; li v0,4); the mirrored source
// ("... return 1;" test then "if (fg0 == 0 || fg1 != 0) return 0; return 4;") is 8 bytes longer.
#if 0
INT CCollUtil::detect2DLineLine(const FVECTOR& vP0, const FVECTOR& vP1) {
    INT fg0;
    INT fg1;
    FLOAT normalY;

    fg0 = detect2DLinePoint(vP0, vP1, m_vCkS);
    normalY = m_vNormal.y;
    fg1 = detect2DLinePoint(vP0, vP1, m_vCkE);
    if (normalY < 0.0f) {
        m_DetectStatus = 1;
    } else {
        m_DetectStatus = 0;
    }
    if (m_vNormal.y < 0.0f) {
        m_DetectStatus |= 2;
    }
    if (m_DetectStatus == 1 || m_DetectStatus == 2) {
        detect2DLinePoint(m_vCkS, m_vCkE, vP0);
        detect2DLinePoint(m_vCkS, m_vCkE, vP1);
    }
    if (fg0 == 0 && fg1 != 0) {
        return 2;
    }
    if (fg0 != 0 && fg1 == 0) {
        return 4;
    }
    if (fg0 != 0 && fg1 != 0) {
        return 1;
    }
    return 0;
}
#endif

INT CCollUtil::detect2DLinePoint(const FVECTOR& vP0, const FVECTOR& vP1, const FVECTOR& vPC) {
    FVECTOR vec0;
    FVECTOR vec1;
    FVECTOR vec2;
    FLOAT f0;

    SubVector(vec0, vP1, vP0);
    SubVector(vec1, vPC, vP0);
    vec0.y = 0.0f;
    vec1.y = 0.0f;
    f0 = InnerProduct(vec0, vec1);
    FLOAT len = InnerProduct(vec0);
    f0 /= len;
    vec2.x = vec0.x * f0;
    vec2.y = 0.0f;
    vec2.z = vec0.z * f0;
    AddVector(vec2, vP0, vec2);
    vec2.y = 0.0f;
    OuterProduct(m_vNormal, vec0, vec1);
    SubVector(vec2, vPC, vec2);
    vec2.y = 0.0f;
    if (InnerProduct(vec2) > m_Radius2) {
        return 0;
    }
    if (f0 < 0.0f) {
        if (InnerProduct(vec1) > m_Radius2) {
            return 0;
        }
        return 1;
    }
    if (f0 > 1.0f) {
        SubVector(vec2, vPC, vP1);
        vec2.y = 0.0f;
        if (InnerProduct(vec2) > m_Radius2) {
            return 0;
        }
        return 1;
    }
    return 2;
}
