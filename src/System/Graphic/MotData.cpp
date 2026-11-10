#include "System/Graphic/MotData.h"
#include "System/HeapMan.h"
#include "System/Math3D.h"

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

extern "C" void (AssertApp)(char* pszFile, int nLine, char* pszMsg, char* pszExp);


extern FVECTOR ONE_VECTOR;




static FLOAT Hermite(FLOAT t0, FLOAT k1, FLOAT k2, FLOAT m1, FLOAT m2);

CMotData::CMotData() {
    memset(m_Name, 0, sizeof(m_Name));
    m_Fps = 30;
    m_nAtr = 6;
    m_Hokan = 8;
    m_nSkl = 0;
    m_nFrame = 0;
    m_pCmp = NULL;
    m_pCmpData = NULL;
    m_Flag = 0;
}

CMotData::~CMotData() {
    Cleanup();
}

void CMotData::CleanupCmp() {
    INT i;

    if (m_pCmp) {
        for (i = 0; i < m_nSkl * m_nAtr; i++) {
            if (!m_pCmp[i].isFix()) {
                if (m_pCmp[i].p) {
                    delete[] m_pCmp[i].p;
                }
                m_pCmp[i].p = NULL;
                break;
            }
        }
    }
    if (m_pCmp) {
        com_free(m_pCmp);
    }
    m_pCmp = NULL;
    if (m_pCmpData) {
        com_free(m_pCmpData);
    }
    m_pCmpData = NULL;
}

void CMotData::Cleanup() {
    memset(m_Name, 0, sizeof(m_Name));
    CleanupCmp();
    m_Fps = 30;
    m_nSkl = 0;
    m_nFrame = 0;
    m_Hokan = 8;
}

FLOAT CMotData::GetCmpData(INT inAtr, INT inSkl, FLOAT inFrame) {
    CMPFIX* p = GetCmpPtr(inAtr, inSkl);
    if (p->isFix()) {
        return p->f;
    }
    return p->p->GetData(inFrame);
}

void CMotData::PrepareGetDataHS(FLOAT inFrame) {
    INT i;
    INT q;

    for (i = 0; i < m_nSkl; i++) {
        for (q = 0; q < m_nAtr; q++) {
            CMPFIX* p = GetCmpPtr(q, i);
            if (!p->isFix()) {
                p->p->PrepareGetDataHS(inFrame);
            }
        }
    }
}

void CMotData::DecodeBin(const BYTE* inBuf) {
    const BYTE* p;
    INT nVer;
    INT nDataByteSize;
    INT ttl;
    INT szFixFg;
    const BYTE* pFixFg;
    const FLOAT* pFix;
    const INT* pSiz;
    const BYTE* pData;
    CCmpData* pCmp;
    INT nCmp;

    p = inBuf;
    Cleanup();
    strncpy(m_Name, (const char*)p, 16);
    m_nSkl = ((MOTDATA_BIN_HEADER*)p)->m_nSkl;
    m_nFrame = ((MOTDATA_BIN_HEADER*)p)->m_nFrame;
    m_nRetFrame = ((MOTDATA_BIN_HEADER*)p)->m_nRetFrame;
    if (m_nFrame < m_nRetFrame) {
        m_nRetFrame = m_nFrame;
    }
    m_Fps = 60 / (((((MOTDATA_BIN_HEADER*)p)->m_Flag & 0x1C) >> 2) + 1);
    m_Flag = ((MOTDATA_BIN_HEADER*)p)->m_Flag;
    nVer = ((MOTDATA_BIN_HEADER*)p)->m_Ver;
    m_nAtr = DoScaleAnime() ? 9 : 6;
    p += sizeof(MOTDATA_BIN_HEADER);
    if (nVer > 0) {
        MOTDATA_BIN_EXT* pExt = (MOTDATA_BIN_EXT*)p;
        p += sizeof(MOTDATA_BIN_EXT);
        m_Hokan = pExt->m_Hokan;
    } else {
        m_Hokan = 8;
    }
    ttl = m_nSkl * m_nAtr;
    szFixFg = ((ttl + 7) / 8 + 3) & ~3;
    pFixFg = p;
    p += szFixFg;
    pFix = (const FLOAT*)p;
    p += ttl * sizeof(FLOAT);
    pSiz = (const INT*)p;
    p += ttl * sizeof(INT);
#line 195
    m_pCmp = new KH_AT("(195)") CMPFIX[ttl];
    ASSERT(m_pCmp);

    pCmp = NULL;
    nCmp = 0;
    pData = p;
    nDataByteSize = 0;
    for (int i = 0; i < ttl; i++) {
        if (!((pFixFg[i / 8] >> (i & 7)) & 1)) {
            nCmp++;
            nDataByteSize += CCmpData::getDataSize(pData, IsKeyFrameShort());
            pData += pSiz[i];
        }
    }
    if (nCmp > 0) {
#line 211
        pCmp = new KH_AT("(211)") CCmpData[nCmp];
        ASSERT(pCmp);
    }
    if (nDataByteSize > 0) {
        m_pCmpData = new KH_AT("(215)") BYTE[nDataByteSize];
        ASSERT(m_pCmpData);
    }

    pData = p;
    nDataByteSize = 0;
    {
    int i;
    int j;
    j = 0;
    for (i = 0; i < ttl; i++) {
        if ((pFixFg[i / 8] >> (i & 7)) & 1) {
            INT q;
            m_pCmp[i].f = pFix[i];
            q = i % m_nAtr;
            if (q < 6) {
                if (q >= 3) {
                    m_pCmp[i].f *= 0.017453292f;
                }
            }
            m_pCmp[i].setFix();
        } else {
            INT q;
            m_pCmp[i].p = &pCmp[j++];
            if (IsKeyFrameShort()) {
                m_pCmp[i].p->SetKeyFrameShort();
            } else {
                m_pCmp[i].p->SetKeyFrameByte();
            }
            if (IsLoop()) {
                m_pCmp[i].p->SetLoop();
            } else {
                m_pCmp[i].p->ResetLoop();
            }
            m_pCmp[i].p->DecodeBin(m_pCmpData + nDataByteSize, pData);
            q = i % m_nAtr;
            if (q < 6) {
                if (q >= 3) {
                    m_pCmp[i].p->DEG_TO_RAD_MINMAX();
                }
            }
            nDataByteSize += CCmpData::getDataSize(pData, IsKeyFrameShort());
            pData += pSiz[i];
        }
    }
    }
}

static FLOAT MotRadianNormal(FLOAT inRadian) {
    while (inRadian > 3.1415927f) {
        inRadian -= 6.2831855f;
    }
    while (inRadian <= -3.1415927f) {
        inRadian += 6.2831855f;
    }
    if (inRadian > 3.1415927f) {
        inRadian = 3.1415927f;
    }
    return inRadian;
}

void CMotData::_GetQuaternionDt(QUATERNION_DT& outDt, INT inSkl, FLOAT inFrame) {
    CMPFIX* pp;
    INT i;
    float val[9];

    pp = GetCmpPtr(0, inSkl);
    for (i = 0; i < m_nAtr; i++, pp++) {
        val[i] = pp->isFix() ? pp->f : pp->p->GetDataHS(inFrame);
    }
    Quaternion::MakeQuaternion(outDt.vQuatRotate, val[3], val[4], val[5]);
    outDt.vTrans.set(val[0], val[1], val[2]);
    if (DoScaleAnime()) {
        outDt.vScale.set(val[6], val[7], val[8]);
    } else {
        outDt.vScale.set(ONE_VECTOR);
    }
}

void CMotData::GetQuaternionDt(QUATERNION_DT& outDt, INT inSkl, FLOAT inFrame) {
    INT f0 = (INT)inFrame;
    FLOAT amari = inFrame - f0;

    if (amari == 0.0f) {
        _GetQuaternionDt(outDt, inSkl, inFrame);
    } else {
        QUATERNION_DT d0;
        QUATERNION_DT d1;
        INT f1 = f0 + 1;
        QUATERNION_DT q0;

        if (f1 >= m_nFrame && IsLoop()) {
            f1 = (SHORT)(m_nRetFrame - 1);
        }
        _GetQuaternionDt(d0, inSkl, f0);
        _GetQuaternionDt(d1, inSkl, f1);
        Quaternion::QuaternionSlerp(q0.vQuatRotate, d0.vQuatRotate, d1.vQuatRotate, amari);
        q0.vTrans = d0.vTrans + (d1.vTrans - d0.vTrans) * amari;
        q0.vScale = d0.vScale + (d1.vScale - d0.vScale) * amari;
        outDt = q0;
    }
}

BOOL CMotData::IsFixSkl(INT inSkl) {
    INT q;

    for (q = 0; q < m_nAtr; q++) {
        if (!GetCmpPtr(q, inSkl)->isFix()) {
            return FALSE;
        }
    }
    return TRUE;
}

CCmpData::CCmpData() {
    m_nKey = 0;
    m_pKeyFrame = NULL;
    m_pCode = NULL;
    m_Min = 0.0f;
    m_HS_Frame2 = m_HS_Frame1 = -1.0f;
    m_HS_Val1 = 0.0f;
    m_HS_Val2 = 0.0f;
    m_HS_KeyNo1 = 0;
    m_HS_KeyNo2 = 0;
}

CCmpData::~CCmpData() {
    m_pKeyFrame = NULL;
    m_pCode = NULL;
}

INT CCmpData::_GetKeyNo(FLOAT inFrame) {
    INT i;

    if (m_nKey == 0) {
        return -1;
    }
    if (m_nKey == 1) {
        return -1;
    }
    if (m_nKey == 2) {
        return 0;
    }
    for (i = 0;; i++) {
        if (i >= m_nKey) {
            break;
        }
        if (inFrame < GetKeyFrame(i)) {
            break;
        }
    }
    if (i >= m_nKey) {
        return m_nKey;
    }
    if (i == 0) {
        return -1;
    }
    return i - 1;
}

void CCmpData::PrepareGetDataHS(FLOAT inFrame) {
    Next(0.0f);
}

FLOAT CCmpData::GetDataHermite(FLOAT inFrame) {
    INT n0;
    INT n1;
    INT n2;
    INT n3;
    FLOAT f0;
    FLOAT f1;
    FLOAT f2;
    FLOAT f3;
    FLOAT v0;
    FLOAT v1;
    FLOAT v2;
    FLOAT v3;
    INT i;
    BOOL bIsKeyFrameShort = m_Flag & 1;
    SHORT* pKFs;
    BYTE* pKFb;

    if (m_nKey == 0) {
        return 0.0f;
    }
    if (m_nKey == 1) {
        return GetVal(0);
    }
    if (m_nKey == 2) {
        if (bIsKeyFrameShort) {
            f0 = ((SHORT*)m_pKeyFrame)[0];
            f1 = ((SHORT*)m_pKeyFrame)[1];
        } else {
            f0 = m_pKeyFrame[0];
            f1 = m_pKeyFrame[1];
        }
        v0 = GetVal(0);
        v1 = GetVal(1);
        return v0 + (v1 - v0) * (inFrame - f0) / (f1 - f0);
    }
    if (bIsKeyFrameShort) {
        pKFs = (SHORT*)m_pKeyFrame;
        for (i = 0; i < m_nKey; i++) {
            FLOAT f = *pKFs++;
            if (f == inFrame) {
                return GetVal(i);
            }
            if (inFrame < f) {
                break;
            }
        }
    } else {
        pKFb = m_pKeyFrame;
        for (i = 0; i < m_nKey; i++) {
            FLOAT f = *pKFb++;
            if (f == inFrame) {
                return GetVal(i);
            }
            if (inFrame < f) {
                break;
            }
        }
    }
    if (i >= m_nKey) {
        if (IsLoop()) {
            return GetDataLinear(inFrame);
        }
        return GetVal(m_nKey - 1);
    }
    pKFs = (SHORT*)m_pKeyFrame;
    pKFb = m_pKeyFrame;
    if (i == m_nKey - 1) {
        n0 = m_nKey - 3;
        n1 = m_nKey - 2;
        n2 = i;
        if (bIsKeyFrameShort) {
            f0 = pKFs[n0];
            f1 = pKFs[n1];
            f2 = pKFs[n2];
        } else {
            f0 = pKFb[n0];
            f1 = pKFb[n1];
            f2 = pKFb[n2];
        }
        v0 = GetVal(n0);
        v1 = GetVal(n1);
        v2 = GetVal(n2);
        return Hermite((inFrame - f1) / (f2 - f1), v1, v2, (v1 - v0) / (f1 - f0), (v2 - v1) / (f2 - f1));
    }
    if (i == 0) {
        return GetVal(0);
    }
    if (i == 1) {
        n1 = i - 1;
        n2 = i;
        n3 = i + 1;
        if (bIsKeyFrameShort) {
            f1 = pKFs[n1];
            f2 = pKFs[n2];
            f3 = pKFs[n3];
        } else {
            f1 = pKFb[n1];
            f2 = pKFb[n2];
            f3 = pKFb[n3];
        }
        v1 = GetVal(n1);
        v2 = GetVal(n2);
        v3 = GetVal(n3);
        return Hermite((inFrame - f1) / (f2 - f1), v1, v2, (v2 - v1) / (f2 - f1), (v3 - v2) / (f3 - f2));
    }
    n0 = i - 2;
    n1 = i - 1;
    n2 = i;
    n3 = i + 1;
    if (bIsKeyFrameShort) {
        f0 = pKFs[n0];
        f1 = pKFs[n1];
        f2 = pKFs[n2];
        f3 = pKFs[n3];
    } else {
        f0 = pKFb[n0];
        f1 = pKFb[n1];
        f2 = pKFb[n2];
        f3 = pKFb[n3];
    }
    v0 = GetVal(n0);
    v1 = GetVal(n1);
    v2 = GetVal(n2);
    v3 = GetVal(n3);
    return Hermite((inFrame - f1) / (f2 - f1), v1, v2, (v1 - v0) / (f1 - f0), (v3 - v2) / (f3 - f2));
}

FLOAT CCmpData::GetData(FLOAT inFrame) {
    if (IsInterpolateLinear(GetKeyNo(inFrame))) {
        return GetDataLinear(inFrame);
    }
    return GetDataHermite(inFrame);
}

FLOAT CCmpData::GetDataLinear(FLOAT inFrame) {
    INT n0;
    FLOAT f0;
    FLOAT f1;
    FLOAT v0;
    FLOAT v1;
    INT i;

    if (m_nKey == 0) {
        return 0.0f;
    }
    if (m_nKey == 1) {
        return GetVal(0);
    }
    if (m_nKey == 2) {
        f0 = GetKeyFrame(0);
        f1 = GetKeyFrame(1);
        v0 = GetVal(0);
        v1 = GetVal(1);
        return v0 + (v1 - v0) * (inFrame - f0) / (f1 - f0);
    }
    for (i = 0; i < m_nKey; i++) {
        FLOAT f = GetKeyFrame(i);
        if (f == inFrame) {
            return GetVal(i);
        }
        if (inFrame < f) {
            break;
        }
    }
    if (i >= m_nKey) {
        if (IsLoop()) {
            n0 = m_nKey - 1;
            f0 = GetKeyFrame(n0);
            v0 = GetVal(m_nKey - 1);
            v1 = GetVal(0);
            return v0 + (v1 - v0) * (inFrame - f0);
        }
        return GetVal(m_nKey - 1);
    }
    if (i == 0) {
        return GetVal(0);
    }
    n0 = i - 1;
    f0 = GetKeyFrame(n0);
    f1 = GetKeyFrame(i);
    v0 = GetVal(n0);
    v1 = GetVal(i);
    return v0 + (v1 - v0) * (inFrame - f0) / (f1 - f0);
}

INT CCmpData::getKeyNum(const BYTE* inBuf) {
    return (inBuf[3] << 24) | (inBuf[2] << 16) | (inBuf[1] << 8) | inBuf[0];
}

LONG CCmpData::getDataSize(const BYTE* inBuf, BOOL bShort) {
    LONG nDataSize = getKeyFrameDataSize(inBuf, bShort);
    nDataSize += getCodeDataSize(inBuf);
    return nDataSize + getInterpolateDataSize(inBuf);
}

LONG CCmpData::getKeyFrameDataSize(const BYTE* inBuf, BOOL bShort) {
    INT nKey = getKeyNum(inBuf);
    LONG szKeyFrame;
    if (bShort) {
        szKeyFrame = nKey * 2;
    } else {
        szKeyFrame = (nKey + 1) & ~1;
    }
    return szKeyFrame;
}

LONG CCmpData::getCodeDataSize(const BYTE* inBuf) {
    return getKeyNum(inBuf) * 2;
}

LONG CCmpData::getInterpolateDataSize(const BYTE* inBuf) {
    INT nKey = getKeyNum(inBuf);
    return ((nKey + 7) / 8 + 1) & ~1;
}

static inline FLOAT getFloat(const BYTE* p) {
    union {
        INT i;
        FLOAT f;
    } u;
    u.i = (p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0];
    return u.f;
}

void CCmpData::DecodeBin(BYTE* pData, const BYTE* inBuf) {
    const BYTE* p = inBuf;
    LONG szKeyFrame;
    LONG szCode;

    m_pKeyFrame = NULL;
    m_pCode = NULL;
    m_nKey = getKeyNum(p);
    p += 4;
    szKeyFrame = getKeyFrameDataSize(inBuf, m_Flag & 1);
    szCode = getCodeDataSize(inBuf);
    szCode = szKeyFrame + szCode + getInterpolateDataSize(inBuf);
    m_Min = getFloat(p);
    p += 4;
    m_ValRange = getFloat(p);
    p += 4;
    m_pKeyFrame = pData;
    memcpy(pData, p, szCode);
    m_pCode = (USHORT*)(m_pKeyFrame + szKeyFrame);
}

void CCmpData::Next(FLOAT inFrame) {
    if (inFrame == 0.0f) {
        m_HS_Frame1 = 0.0f;
        m_HS_KeyNo1 = 0;
        m_HS_Val1 = GetVal(0);
        m_HS_KeyNo2 = 1;
    } else {
        FLOAT fFrame2 = m_HS_Frame2;
        if (inFrame < fFrame2) {
            m_HS_Frame1 = 0.0f;
            m_HS_KeyNo1 = 0;
            m_HS_Val1 = GetVal(0);
            m_HS_KeyNo2 = 1;
            fFrame2 = GetKeyFrame(m_HS_KeyNo2);
        }
        if (fFrame2 < inFrame) {
            INT iFrame = (INT)fFrame2;
            INT iFrame1;
            INT iFrame2 = (INT)(inFrame + 0.999999f);
            INT iKeyNo2 = m_HS_KeyNo2;
            do {
                iFrame1 = iFrame;
                if (++iKeyNo2 >= m_nKey) {
                    break;
                }
            } while ((iFrame = GetKeyFrame(iKeyNo2)) < iFrame2);
            m_HS_Frame1 = iFrame1;
            m_HS_KeyNo2 = iKeyNo2;
            m_HS_KeyNo1 = iKeyNo2 - 1;
            m_HS_Val2 = m_HS_Val1 = GetVal(m_HS_KeyNo1);
        }
    }
    if (m_HS_KeyNo2 >= m_nKey) {
        if (IsLoop()) {
            m_HS_Mode = 1;
            m_HS_Val2 = GetVal(0);
            m_HS_Frame2 = m_HS_Frame1 + 1.0f;
        } else {
            m_HS_Mode = 2;
            m_HS_Frame2 = m_HS_Frame1 + 1.0f;
        }
    } else {
        m_HS_Frame2 = GetKeyFrame(m_HS_KeyNo2);
        m_HS_Val2 = GetVal(m_HS_KeyNo2);
        if (IsInterpolateLinear(m_HS_KeyNo1)) {
            m_HS_Mode = 0;
        } else if (m_HS_KeyNo1 == 0) {
            m_HS_Mode = 4;
        } else if (m_HS_KeyNo1 == m_nKey - 1) {
            m_HS_Mode = 5;
        } else {
            m_HS_Mode = 3;
        }
    }
}
FLOAT CCmpData::GetDataHS(FLOAT inFrame) {
    FLOAT f0;
    FLOAT f3;
    FLOAT v0;
    FLOAT v1;
    FLOAT v2;
    FLOAT v3;
    FLOAT f1;
    FLOAT f2;
    int HS_KeyNo;

#line 961
    ASSERT(inFrame != -1.0f);
    if (m_nKey == 2) {
        f0 = GetKeyFrame(0);
        f1 = GetKeyFrame(1);
        v0 = GetVal(0);
        v1 = GetVal(1);
        return v0 + (v1 - v0) * (inFrame - f0) / (f1 - f0);
    }
    if (m_HS_Frame1 == inFrame) {
        return m_HS_Val1;
    }
    if (m_HS_Frame2 == inFrame) {
        return m_HS_Val2;
    }
    if (inFrame < m_HS_Frame1 || m_HS_Frame2 < inFrame) {
        Next(inFrame);
        return GetDataHS(inFrame);
    }
    switch (m_HS_Mode) {
    case 0:
        f1 = m_HS_Frame1;
        f2 = m_HS_Frame2;
        v1 = m_HS_Val1;
        v2 = m_HS_Val2;
        return v1 + (v2 - v1) * (inFrame - f1) / (f2 - f1);
    case 1:
        f1 = m_HS_Frame1;
        f2 = m_HS_Frame2;
        v1 = m_HS_Val1;
        return v1 + (m_HS_Val2 - v1) * (inFrame - f1);
    case 2:
        return m_HS_Val1;
    case 3:
        f1 = m_HS_Frame1;
        f2 = m_HS_Frame2;
        v1 = m_HS_Val1;
        v2 = m_HS_Val2;
        HS_KeyNo = m_HS_KeyNo1 - 1;
        f0 = GetKeyFrame(HS_KeyNo);
        v0 = GetVal(HS_KeyNo);
        HS_KeyNo = m_HS_KeyNo2 + 1;
        f3 = GetKeyFrame(HS_KeyNo);
        v3 = GetVal(HS_KeyNo);
        return Hermite((inFrame - f1) / (f2 - f1), v1, v2,
                       (v1 - v0) / (f1 - f0), (v3 - v2) / (f3 - f2));
    case 4:
        f1 = m_HS_Frame1;
        f2 = m_HS_Frame2;
        v1 = m_HS_Val1;
        v2 = m_HS_Val2;
        HS_KeyNo = m_HS_KeyNo2 + 1;
        f3 = GetKeyFrame(HS_KeyNo);
        v3 = GetVal(HS_KeyNo);
        return Hermite((inFrame - f1) / (f2 - f1), v1, v2,
                       (v2 - v1) / (f2 - f1), (v3 - v2) / (f3 - f2));
    case 5:
        f1 = m_HS_Frame1;
        f2 = m_HS_Frame2;
        v1 = m_HS_Val1;
        v2 = m_HS_Val2;
        HS_KeyNo = m_HS_KeyNo1 - 1;
        f0 = GetKeyFrame(HS_KeyNo);
        v0 = GetVal(HS_KeyNo);
        return Hermite((inFrame - f1) / (f2 - f1), v1, v2,
                       (v1 - v0) / (f1 - f0), (v2 - v1) / (f2 - f1));
    }
    return 0.0f;
}

static FLOAT Hermite(FLOAT t0, FLOAT k1, FLOAT k2, FLOAT m1, FLOAT m2) {
    FLOAT t2 = t0 * t0;
    FLOAT t3 = t2 * t0;
    FLOAT H00 = 2.0f * t3 - 3.0f * t2 + 1.0f;
    FLOAT H01 = -2.0f * t3 + 3.0f * t2;
    FLOAT H10 = t3 - 2.0f * t2 + t0;
    FLOAT H11 = t3 - t2;
    return H00 * k1 + H01 * k2 + H10 * m1 + H11 * m2;
}

inline INT CCmpData::GetKeyNo(FLOAT inFrame) {
    return _GetKeyNo(inFrame);
}

inline BOOL CCmpData::IsKeyFrameShort() {
    return m_Flag & 1;
}
// SOURCE DATA: .rodata
