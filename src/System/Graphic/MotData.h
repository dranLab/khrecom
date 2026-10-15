#ifndef SYSTEM_GRAPHIC_MOTDATA_H
#define SYSTEM_GRAPHIC_MOTDATA_H

#include "khtypes.h"
#include "System/Vector.h"

struct MOTVEC {
    FLOAT x;
    FLOAT y;
    FLOAT z;
};

typedef FVECTOR QUATERNION;

struct QUATERNION_DT {
    QUATERNION vQuatRotate;
    FVECTOR vTrans;
    FVECTOR vScale;
};

typedef struct {
    char m_Name[16];
    SHORT m_nSkl;
    SHORT m_nFrame;
    SHORT m_nRetFrame;
    BYTE m_Flag;
    BYTE m_Ver;
} MOTDATA_BIN_HEADER;

typedef struct {
    LONG m_Hokan;
} MOTDATA_BIN_EXT;

class CCmpData {
private:
    BYTE m_Flag;
    BYTE m_HS_Mode;
    SHORT m_nKey;
    BYTE* m_pKeyFrame;
    USHORT* m_pCode;
    FLOAT m_Min;
    FLOAT m_ValRange;
    FLOAT m_HS_Frame1;
    FLOAT m_HS_Frame2;
    FLOAT m_HS_Val1;
    FLOAT m_HS_Val2;
    SHORT m_HS_KeyNo1;
    SHORT m_HS_KeyNo2;

public:
    CCmpData();
    ~CCmpData();
    FLOAT ConvVal(USHORT inCode) { return inCode * m_ValRange * (1.0f / 65535.0f) + m_Min; }
    USHORT ConvCode(INT inNo) { return m_pCode[inNo]; }
    FLOAT GetVal(INT inNo) { return ConvVal(ConvCode(inNo)); }
    FLOAT GetData(FLOAT inFrame);
    FLOAT GetDataHermite(FLOAT inFrame);
    FLOAT GetDataLinear(FLOAT inFrame);
    FLOAT GetDataHS(FLOAT inFrame);
    void PrepareGetDataHS(FLOAT inFrame);
    void Next(FLOAT inFrame);
    void ResetHS() {
        m_HS_Frame1 = 0.0f;
        m_HS_KeyNo1 = 0;
        m_HS_Val1 = GetVal(0);
        m_HS_KeyNo2 = 1;
    }
    BYTE* getInterpolate() { return (BYTE*)(m_pCode + m_nKey); }
    BOOL IsInterpolateLinear(INT inNo) {
        if (inNo < 0 || inNo >= m_nKey) return TRUE;
        return getInterpolate()[inNo >> 3] & (1 << (inNo & 7));
    }
    void SetKeyFrameShort() { m_Flag |= 1; }
    void SetKeyFrameByte() { m_Flag &= ~1; }
    void SetLoop() { m_Flag |= 2; }
    void ResetLoop() { m_Flag &= ~2; }
    BOOL IsLoop() { return m_Flag & 2; }
    SHORT GetNbKeyFrame() { return m_nKey; }
    INT GetKeyFrame(INT inNo) {
        return IsKeyFrameShort() ? ((SHORT*)m_pKeyFrame)[inNo] : m_pKeyFrame[inNo];
    }
    FLOAT GetKeyVal(INT inNo) { return GetVal(inNo); }
    INT _GetKeyNo(FLOAT inFrame);
    INT GetKeyNo(FLOAT inFrame);
    void DEG_TO_RAD_MINMAX() {
        m_Min *= 0.017453292f;
        m_ValRange *= 0.017453292f;
    }
    void DecodeBin(BYTE* pData, const BYTE* inBuf);

private:
    BOOL IsKeyFrameShort();

public:
    static LONG getDataSize(const BYTE* inBuf, BOOL bShort);

private:
    static INT getKeyNum(const BYTE* inBuf);
    static LONG getKeyFrameDataSize(const BYTE* inBuf, BOOL bShort);
    static LONG getCodeDataSize(const BYTE* inBuf);
    static LONG getInterpolateDataSize(const BYTE* inBuf);
};

union CMPFIX {
    CCmpData* p;
    FLOAT f;
    UINT i;

    BOOL isFix() {
        if (p == NULL || (i & 1)) return TRUE;
        return FALSE;
    }
    void setFix() {
        if (i) i |= 1;
    }
};

class CMotData {
private:
    char m_Name[16];
    SHORT m_nSkl;
    SHORT m_nFrame;
    SHORT m_nRetFrame;
    USHORT m_Flag;
    SHORT m_Hokan;
    BYTE m_Fps;
    BYTE m_nAtr;
    CMPFIX* m_pCmp;
    BYTE* m_pCmpData;

    CMPFIX* GetCmpPtr(INT inAtr, INT inSkl) { return &m_pCmp[inSkl * m_nAtr + inAtr]; }

public:
    CMotData();
    ~CMotData();
    void Cleanup();
    void CleanupCmp();
    CHAR* GetName() { return m_Name; }
    INT GetNbSkl() { return m_nSkl; }
    INT GetNbFrame() { return m_nFrame; }
    INT GetFps() { return m_Fps; }
    INT GetHokan() { return m_Hokan; }
    BOOL IsKeyFrameShort() { return m_nFrame >= 256; }
    BOOL IsLoop() { return m_nRetFrame > 0; }
    FLOAT GetCmpData(INT inAtr, INT inSkl, FLOAT inFrame);
    void DecodeBin(const BYTE* inBuf);
    void _GetQuaternionDt(QUATERNION_DT& outDt, INT inSkl, FLOAT inFrame);
    void GetQuaternionDt(QUATERNION_DT& outDt, INT inSkl, FLOAT inFrame);
    void PrepareGetDataHS(FLOAT inFrame);
    BOOL IsFixSkl(INT inSkl);
    BOOL DoScaleAnime() { return m_Flag & 2; }
};

#endif /* SYSTEM_GRAPHIC_MOTDATA_H */
