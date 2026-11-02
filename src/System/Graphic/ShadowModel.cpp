#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h"
#define MODELDATA_H_PATH "c:/KHCOM/MainApp/System/Graphic/modeldata.h"
#define MODEL_H_PATH "c:/KHCOM/MainApp/System/Graphic/Model.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "System/Task.h"
#include "System/TaskMan.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"
/* Placeholders: two empty strings follow CollUtil.h's strings in .rodata (as in Game/Enemy/MapEnemyShadow); which
 * headers they come from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() {
    TRACE("");
}

#include "System/Graphic/ShadowModel.h"
#include "System/Graphic/WorldEnv.h"
#include "System/Graphic/Camera.h"
#include "System/VramMan.h"
#include "System/Math3D.h"
#include "System/Debug.h"
#include "Game/Debug/DebugWork.h"
#include "include_asm.h"

/* PARTIAL: from System/Coord.h */
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void DivVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void MulMatrix(FMATRIX& m0, const FMATRIX& m1, const FMATRIX& m2);
void FTOI4Vector(IVECTOR& v0, const FVECTOR& v1);
extern "C" {
void sceVu0CameraMatrix(float m[4][4], float p[4], float zd[4], float yd[4]);
void sceVu0UnitMatrix(float m[4][4]);
void sceVu0InversMatrix(float m0[4][4], float m1[4][4]);
}

#include "System/Graphic/PackMan.h"







extern INT SCREEN_HEIGHT;

/* one qword of an A+D packet (data, register address) */
static inline void _setAD(void* qp, u_long data, u_long addr) {
    u_long128 out128;
    __asm__ volatile("pcpyld %0, %1, %0" : "=r"(out128) : "r"(addr), "0"(data));
    *(u_long128*)qp = out128;
}

static inline void _setADnv(void* qp, u_long data, u_long addr) {
    u_long128 out128;
    __asm__("pcpyld %0, %1, %0" : "=r"(out128) : "r"(addr), "0"(data));
    *(u_long128*)qp = out128;
}

/* the same for a constant value: tied to the output, a constant is first zero-extended to 128 bits (por) */
static inline void _setADConst(void* qp, u_long data, u_long addr) {
    u_long128 out128;
    __asm__ volatile("pcpyld %0, %2, %1" : "=r"(out128) : "r"(data), "r"(addr));
    *(u_long128*)qp = out128;
}

union PRIMIVECTOR {
    struct {
        SHORT x;
        SHORT y;
        UINT z;
    };
    ULONGLONG ul64;
};

struct SCREEN {
    float ax;
    float ay;
    float cx;
    float cy;
    float scrz;
};

static INT sdw_SCREEN_WIDTH;
static INT sdw_SCREEN_HEIGHT;
static INT sdw_TEXW;
static INT sdw_TEXH;

FMATRIX CShadowModelMan::m_mWorldTexture;
FMATRIX CShadowModelMan::m_mWorldScreen;
FMATRIX CShadowModelMan::m_mProjectionTexture;
FMATRIX CShadowModelMan::m_mProjectionScreen;
UINT CShadowModelMan::m_iStatus;
FLOAT CShadowModelMan::m_fCDensity;
FLOAT CShadowModelMan::m_fADensity;
FLOAT CShadowModelMan::m_fUpPer;
FLOAT CShadowModelMan::m_fDownLen;
FLOAT CShadowModelMan::m_fClipDistSq;
ULONGLONG CShadowModelMan::m_lTex0;
FVECTOR CShadowModelMan::m_vParam;
FVECTOR CShadowModelMan::m_vCamPos;

static inline void _initVf0(FVECTOR& v) {
    asm volatile("sqc2 $vf0, 0x0(%0)" : : "r"(&v));
}

template <class T> static inline T& _ref(T& v) {
    return v;
}

static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}

void CShadowModelMan::initialize() {
    m_iStatus = 0;
    m_fCDensity = 0.2f;
    m_fADensity = 0.3f;
    m_fUpPer = 0.5f;
    m_fDownLen = 2.8f;
    m_fClipDistSq = 1225.0f;
    m_mProjectionTexture.m[0][0] = 0.5f;
    m_mProjectionTexture.m[0][1] = 0.0f;
    m_mProjectionTexture.m[0][2] = 0.0f;
    m_mProjectionTexture.m[0][3] = 0.0f;
    m_mProjectionTexture.m[1][0] = 0.0f;
    m_mProjectionTexture.m[1][1] = 0.5f;
    m_mProjectionTexture.m[1][2] = 0.0f;
    m_mProjectionTexture.m[1][3] = 0.0f;
    m_mProjectionTexture.m[2][0] = 0.0f;
    m_mProjectionTexture.m[2][1] = 0.0f;
    m_mProjectionTexture.m[2][2] = 0.0f;
    m_mProjectionTexture.m[2][3] = 0.0f;
    m_mProjectionTexture.m[3][0] = 0.5f;
    m_mProjectionTexture.m[3][1] = 0.5f;
    m_mProjectionTexture.m[3][2] = 1.0f;
    m_mProjectionTexture.m[3][3] = 1.0f;
}

void CShadowModelMan::finalize() {}

void CShadowModelMan::updateMatrix() {
    CCamera* pCamera = CWorldEnv::getActiveCamera();
    if (pCamera) {
        m_vCamPos = pCamera->Crd(0).getWorldMatrix().getTrans();
    } else {
        _initVf0(m_vCamPos);
    }
}

void CShadowModelMan::updateShadowMatrix(const FVECTOR& vMax, const FVECTOR& vMin, INT nSize) {
    FMATRIX mProjectionScreen;
    FMATRIX mWorldProjection;
    FMATRIX mViewProjection;
    FMATRIX mWorldView;
    FVECTOR vEye;
    FVECTOR vLook;
    FVECTOR vUp;
    FVECTOR vTmp;
    FVECTOR vCenter;
    FLOAT fLenX;
    FLOAT fLenY;
    FLOAT fLenZ;
    FLOAT orgMinY;
    FLOAT orgMaxY;
    if (nSize == 2) {
        sdw_SCREEN_WIDTH = 0x200;
        sdw_SCREEN_HEIGHT = 0x200;
        sdw_TEXW = 9;
        sdw_TEXH = 9;
    } else if (nSize == 1) {
        sdw_SCREEN_WIDTH = 0x100;
        sdw_SCREEN_HEIGHT = 0x100;
        sdw_TEXW = 8;
        sdw_TEXH = 8;
    } else {
        sdw_SCREEN_WIDTH = 0x80;
        sdw_SCREEN_HEIGHT = 0x80;
        sdw_TEXW = 7;
        sdw_TEXH = 7;
    }
    _ref(m_lTex0) = (ULONGLONG)VRAM::_TEX_AREA_START | ((ULONGLONG)(sdw_SCREEN_WIDTH / 64) << 14) |
              ((ULONGLONG)sdw_TEXW << 26) | ((ULONGLONG)sdw_TEXH << 30) | ((ULONGLONG)1 << 34) | ((ULONGLONG)1 << 61);
    orgMinY = vMin.y + m_fDownLen;
    orgMaxY = orgMinY + (vMax.y - orgMinY) * (1.0f / m_fUpPer);
    AddVectorXYZ(vCenter, vMax, vMin);
    ScaleVectorXYZ(vCenter, vCenter, 0.5f);
    vEye.x = vCenter.x;
    vEye.y = orgMaxY;
    vEye.z = vCenter.z;
    vEye.w = 1.0f;
    vLook.x = vCenter.x;
    vLook.y = orgMinY;
    vLook.z = vCenter.z;
    vLook.w = 1.0f;
    vUp.x = 0.0f;
    vUp.y = 0.0f;
    vUp.z = 1.0f;
    vUp.w = 1.0f;
    SubVectorXYZ(vTmp, vLook, vEye);
    Normalize(vTmp, vTmp);
    sceVu0CameraMatrix(mWorldView.m, vEye.v, vTmp.v, vUp.v);
    fLenX = (vMax.x - vMin.x) * 0.5f;
    fLenY = orgMaxY - orgMinY;
    fLenZ = (vMax.z - vMin.z) * 0.5f;
    getOrthoMatrix(mViewProjection, -fLenX, fLenX, -fLenZ, fLenZ, -0.5f, fLenY + 5.0f);
    getViewportMatrix(mProjectionScreen, 2048.0f, 2048.0f, (FLOAT)sdw_SCREEN_WIDTH, (FLOAT)sdw_SCREEN_HEIGHT, 16777000.0f, 1.0f);
    MulMatrix(mWorldProjection, mViewProjection, mWorldView);
    MulMatrix(m_mWorldTexture, m_mProjectionTexture, mWorldProjection);
    MulMatrix(m_mWorldScreen, mProjectionScreen, mWorldProjection);
    m_vParam.x = 1.0f / (orgMinY - vMin.y);
    m_vParam.y = 1.0f / (vMax.y - orgMinY);
    m_vParam.z = -vMin.y;
    m_vParam.w = -orgMinY;
    CModel::resetWorldScreenMatrix();
}

/* Near match (two instructions: the original updates the i * 64 + 63 induction register at the end of the loop
 * body, not after i++), kept for reference: */
#if 0
void CShadowModelMan::drawBefore() {
    QWdata* qp;
    u_long128 tag;
    CModel::setSendCtrl(4);
    __asm__ volatile("pcpyld %0, %2, %1" : "=r"(tag) : "r"((u_long)0x1000000000008000UL), "0"((u_long)0xE));
    qp = CPackMan::pkOpenGifTag(tag);
    ((u_long*)qp)[0] = (u_long)(VRAM::_TEX_AREA_START / 32) | ((u_long)(sdw_SCREEN_WIDTH / 64) << 16);
    ((u_long*)qp)[1] = 0x4C;
    qp++;
    ((u_long*)qp)[0] = (u_long)(VRAM::_ZBUF_START / 32) | 0x32000000 | 0x100000000UL;
    ((u_long*)qp)[1] = 0x4E;
    qp++;
    ((u_long*)qp)[0] = (u_long)((0x800 - (sdw_SCREEN_WIDTH >> 1)) << 4) | ((u_long)((0x800 - (sdw_SCREEN_HEIGHT >> 1)) << 4) << 32);
    ((u_long*)qp)[1] = 0x18;
    qp++;
    ((u_long*)qp)[0] = 0x30000;
    ((u_long*)qp)[1] = 0x47;
    qp++;
    ((u_long*)qp)[0] = ((u_long)(sdw_SCREEN_WIDTH - 1) << 16) | ((u_long)(sdw_SCREEN_HEIGHT - 1) << 48);
    ((u_long*)qp)[1] = 0x40;
    qp++;
    _setADConst(&qp[0], 6, 0);
    _setADConst(&qp[1], 0, 1);
    if (DebugWork::g_dw.shadow.iDispArea) {
        _setADConst(&qp[1], 0x60000080, 1);
    }
    qp += 2;
    for (INT i = 0; i < sdw_SCREEN_WIDTH / 64;) {
        u_long d = (u_long)(i * 64) | ((u_long)(i * 64 + 63) << 16) | ((u_long)(sdw_SCREEN_HEIGHT - 1) << 48);
        i++;
        _setAD(&qp[0], d, 0x40);
        _setAD(&qp[1], (u_long)(0x8000 - ((sdw_SCREEN_WIDTH / 2) << 4)) | ((u_long)(0x8000 - ((sdw_SCREEN_HEIGHT / 2) << 4)) << 16), 5);
        _setAD(&qp[2], (u_long)(0x8000 + ((sdw_SCREEN_WIDTH / 2) << 4)) | ((u_long)(0x8000 + ((sdw_SCREEN_HEIGHT / 2) << 4)) << 16), 5);
        qp += 3;
    }
    ((u_long*)qp)[0] = 0x3000D;
    ((u_long*)qp)[1] = 0x47;
    qp++;
    ((u_long*)qp)[0] = ((u_long)(sdw_SCREEN_WIDTH - 1) << 16) | ((u_long)(sdw_SCREEN_HEIGHT - 1) << 48);
    ((u_long*)qp)[1] = 0x40;
    qp++;
    ((u_long*)qp)[0] = 0x8000008080UL;
    ((u_long*)qp)[1] = 0x3B;
    qp++;
    CPackMan::pkCloseGifTag(qp);
}
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/ShadowModel", drawBefore__15CShadowModelMan);
static QWdata* setSdwTSprite32(QWdata* qp, FIX4 plsx, FIX4 plsy, UINT z, FLOAT fScale) {
    PRIMIVECTOR pv[2];
    PRIMIVECTOR uv[2];
    int i;
    int cnt;
    SHORT ofsXM;
    SHORT ofsXP;
    IVECTOR ivUVPosUL;
    IVECTOR ivUVRect;
    INT iNextSize;
    ULONGLONG* pul64;
    FLOAT fRate;
    FVECTOR fvUVPosUL;
    FVECTOR fvUVRect;
    pv[0].x = plsx;
    pv[0].y = plsy;
    pv[1].z = z;
    ofsXM = 0;
    ofsXP = 0;
    if (pv[0].x < 0) {
        ofsXM = pv[0].x;
        pv[0].x = 0;
    } else if (pv[0].x > 0) {
        ofsXP = pv[0].x;
    }
    pv[0].x += (0x800 - sdw_SCREEN_WIDTH / 2) << 4;
    pv[0].y += (0x800 - sdw_SCREEN_HEIGHT / 2) << 4;
    pv[1].x = pv[0].x + (32 << 4) - ofsXP;
    pv[1].y = pv[0].y + (sdw_SCREEN_HEIGHT << 4);
    if (fScale != 1.0f) {
        fRate = 1.0f / fScale;
        fvUVPosUL.x = sdw_SCREEN_WIDTH / 2;
        fvUVPosUL.y = sdw_SCREEN_HEIGHT / 2;
        fvUVPosUL.z = 0.0f;
        fvUVPosUL.w = 0.0f;
        ScaleVector(fvUVPosUL, fvUVPosUL, 1.0f - fRate);
        fvUVRect.x = sdw_SCREEN_WIDTH;
        fvUVRect.y = sdw_SCREEN_HEIGHT;
        fvUVRect.z = 0.0f;
        fvUVRect.w = 0.0f;
        ScaleVector(fvUVRect, fvUVRect, fRate);
        FTOI4Vector(ivUVPosUL, fvUVPosUL);
        fvUVRect.x *= 0.0625f;
        FTOI4Vector(ivUVRect, fvUVRect);
    } else {
        ivUVRect.x = 32 << 4;
        ivUVRect.y = sdw_SCREEN_HEIGHT << 4;
        ivUVPosUL.x = 0;
        ivUVPosUL.y = 0;
        ivUVPosUL.z = 0;
        ivUVPosUL.w = 0;
        ivUVRect.z = 0;
        ivUVRect.w = 0;
    }
    iNextSize = ivUVRect.x;
    uv[0].x = ivUVPosUL.x;
    uv[0].y = ivUVPosUL.y;
    if (ofsXM) {
        uv[0].x -= ofsXM;
    }
    uv[1].x = uv[0].x + iNextSize;
    if (ofsXP) {
        uv[1].x -= ofsXP;
    }
    uv[1].y = uv[0].y + ivUVRect.y;
    cnt = (sdw_SCREEN_WIDTH << 4) / (32 << 4);
    if (qp) {
        CPackMan::pkCloseGifTag(qp);
    }
    u_long128 tag;
    __asm__ volatile("pcpyld %0, %2, %1" : "=r"(tag) : "r"((u_long)cnt | 0x4400000000000000UL), "r"((u_long)0x4343));
    pul64 = (ULONGLONG*)CPackMan::pkOpenGifTagManual(tag);
    for (i = 0; i < cnt; i++) {
        pul64[0] = uv[0].ul64;
        pul64[1] = pv[0].ul64;
        pul64[2] = uv[1].ul64;
        pul64[3] = pv[1].ul64;
        pul64 += 4;
        pv[0].x = pv[1].x;
        pv[1].x += 32 << 4;
        uv[0].x = uv[1].x;
        uv[1].x += iNextSize;
    }
    CPackMan::pkCloseGifTagManual((UINT*)pul64);
    if (qp) {
        __asm__ volatile("pcpyld %0, %2, %1" : "=r"(tag) : "r"((u_long)0x1000000000000000UL), "0"((u_long)0xE));
        qp = (QWdata*)CPackMan::pkOpenGifTag(tag);
    }
    return qp;
}
void CShadowModelMan::drawAfter() {
    QWdata* qp;
    static int cnt = 1;
    u_long128 tag;
    __asm__ volatile("pcpyld %0, %2, %1" : "=r"(tag) : "r"((u_long)0x1000000000008000UL), "0"((u_long)0xE));
    qp = (QWdata*)CPackMan::pkOpenGifTag(tag);
    _setADConst(&qp[0], 0x30000, 0x47);
    _setADConst(&qp[1], 5, 8);
    _setADConst(&qp[2], 0, 0x3F);
    _setADConst(&qp[3], 0x61, 0x14);
    _setAD(&qp[4], m_lTex0, 6);
    _setADConst(&qp[5], 0x156, 0);
    _setADConst(&qp[6], 0x80000000A8UL, 0x42);
    _setADConst(&qp[7], 0x80808080, 1);
    CPackMan::pkCloseGifTag(&qp[8]);
    if (dbgCheckUser(NAKAMICH)) {
        if (dbgButtonDown(NAKAMICH, 0x10, 1)) {
            if (dbgButtonTrg(NAKAMICH, 0x2000, 1)) {
                cnt++;
            } else if (dbgButtonTrg(NAKAMICH, 0x8000, 1)) {
                cnt--;
            }
            if (cnt > 16) cnt = 16;
            if (cnt < 0) cnt = 0;
            TRACE("Shadow %d\n", cnt);
        }
    }
    for (int i = 0; i < cnt; i++) {
        setSdwTSprite32(NULL, -16, -16, 0, 1.0f);
    }
    __asm__ volatile("pcpyld %0, %2, %1" : "=r"(tag) : "r"((u_long)0x1000000000008000UL), "0"((u_long)0xE));
    qp = (QWdata*)CPackMan::pkOpenGifTag(tag);
    _setADConst(&qp[0], 2, 0);
    _setADConst(&qp[1], 0, 1);
    _setAD(&qp[2], (u_long)(0x8000 + (-(sdw_SCREEN_WIDTH / 2) << 4)) | ((u_long)(0x8000 + ((sdw_SCREEN_HEIGHT / 2 - 1) << 4)) << 16), 5);
    _setAD(&qp[3], (u_long)(0x8000 - ((sdw_SCREEN_WIDTH / 2) << 4)) | ((u_long)(0x8000 - ((sdw_SCREEN_HEIGHT / 2) << 4)) << 16), 5);
    _setAD(&qp[4], (u_long)(0x8000 + ((sdw_SCREEN_WIDTH / 2 - 1) << 4)) | ((u_long)(0x8000 - ((sdw_SCREEN_HEIGHT / 2) << 4)) << 16), 5);
    _setAD(&qp[5], (u_long)(0x8000 + ((sdw_SCREEN_WIDTH / 2 - 1) << 4)) | ((u_long)(0x8000 + ((sdw_SCREEN_HEIGHT / 2 - 1) << 4)) << 16), 5);
    _setAD(&qp[6], (u_long)(0x8000 - ((sdw_SCREEN_WIDTH / 2) << 4)) | ((u_long)(0x8000 + ((sdw_SCREEN_HEIGHT / 2 - 1) << 4)) << 16), 5);
    qp += 7;
    ((u_long*)qp)[0] = (u_long)(VRAM::_FRAME0_START / 32) | 0x80000;
    ((u_long*)qp)[1] = 0x4C;
    qp++;
    ((u_long*)qp)[0] = (u_long)(VRAM::_ZBUF_START / 32) | 0x31000000;
    ((u_long*)qp)[1] = 0x4E;
    qp++;
    ((u_long*)qp)[0] = 0x7000 | ((u_long)((0x800 - (SCREEN_HEIGHT >> 1)) << 4) << 32);
    ((u_long*)qp)[1] = 0x18;
    qp++;
    ((u_long*)qp)[0] = 0x5000D;
    ((u_long*)qp)[1] = 0x47;
    qp++;
    ((u_long*)qp)[0] = 0x1FF0000 | ((u_long)(SCREEN_HEIGHT - 1) << 48);
    ((u_long*)qp)[1] = 0x40;
    qp++;
    ((u_long*)qp)[0] = 0x8000008080UL;
    ((u_long*)qp)[1] = 0x3B;
    qp++;
    ((u_long*)qp)[0] = 0;
    ((u_long*)qp)[1] = 0x3F;
    qp++;
    CPackMan::pkCloseGifTag(qp);
    CModel::resetSendCtrl(4);
}
/* Placeholder for the INCLUDE_ASM'd function below: its float constant (1.7014118e38f) in .rodata. */
__asm__(".section .sdata\n");
static FLOAT __attribute__((section(".sdata"))) _getScreenAreaClipToWorldPosConst() {
    return 1.7014118e38f;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/ShadowModel", getScreenAreaClipToWorldPos__FR7FVECTORT0P7FVECTORRC7FMATRIXT3PC6SCREENPCt6RECT_T1Ziff);

static int _getCrossPlanePoint(FVECTOR& ww, const FVECTOR& pw0, const FVECTOR& vw0, float height, float limit) {
    FVECTOR vPlaneNormal;
    FVECTOR vPlanePos;
    FVECTOR pw;
    FVECTOR vw;
    FVECTOR vTmp;
    pw = pw0;
    vw = vw0;
    vPlaneNormal.set(0.0f, 1.0f, 0.0f);
    vPlanePos.set(0.0f, height, 0.0f);
    Math3D::GetIntersectionTimeOfPlaneAndLine(ww, vPlaneNormal, vPlanePos, pw, vw);
    ww.w = 1.0f;
    return 0;
}

static int _getCrossPoint(FVECTOR& ww, const FVECTOR& pw0, const FVECTOR& vw0, float height, float limit) {
    int ret = 0;
    FVECTOR pw;
    FVECTOR vw;
    float lenge;
    float xsa;
    float zsa;
    float limit2;
    float pwy2;
    pw = pw0;
    vw = vw0;
    pw.y -= height;
    vw.y -= height;
    if (vw.y - pw.y >= 0.0f) {
        ww.x = pw.x + pw.y * (vw.x - pw.x);
        ww.z = pw.z + pw.y * (vw.z - pw.z);
    } else {
        ww.x = pw.x - pw.y * (vw.x - pw.x) / (vw.y - pw.y);
        ww.z = pw.z - pw.y * (vw.z - pw.z) / (vw.y - pw.y);
    }
    ww.y = height;
    ww.w = 1.0f;
    if (limit > 0.0f) {
        limit2 = limit * limit;
        pwy2 = pw.y * pw.y;
        xsa = ww.x - pw.x;
        zsa = ww.z - pw.z;
        if (xsa < 0.0f) xsa = -xsa;
        if (zsa < 0.0f) zsa = -zsa;
        lenge = _sqrtf(limit2 - pwy2);
        if (limit < xsa || limit2 < pwy2 + xsa * xsa) {
            ww.x = (pw.x < ww.x) ? pw.x + lenge : pw.x - lenge;
            ret = 1;
        }
        if (limit < zsa || limit2 < pwy2 + zsa * zsa) {
            ww.z = (pw.z < ww.z) ? pw.z + lenge : pw.z - lenge;
            ret = 1;
        }
    }
    return ret;
}

static inline void _unitMatrix(FMATRIX& m1) {
    sceVu0UnitMatrix(m1.m);
}

static void _ScreenToWorld(FVECTOR& w, const FMATRIX& wvm, const SCREEN* scr, const FVECTOR& s) {
    FMATRIX rotm;
    FMATRIX mat;
    _unitMatrix(mat);
    mat.m[0][0] = 1.0f / scr->ax;
    mat.m[1][1] = 1.0f / scr->ay;
    sceVu0InversMatrix(rotm.m, ((FMATRIX&)wvm).m);
    MulMatrix(rotm, rotm, mat);
    ApplyMatrix(w, rotm, s);
    w.w = 1.0f;
}

static void _WorldToScreen(FVECTOR& s, const FMATRIX& wsm, const SCREEN* scr, const FVECTOR& w) {
    ApplyMatrix(s, wsm, w);
    if (s.w != 0.0f) {
        DivVectorXYZ(s, s, s.w);
    }
    s.x -= scr->cx;
    s.y -= scr->cy;
    if (s.w < 0.0f) {
        s.x = -s.x;
        s.y = -s.y;
    }
    s.z = scr->scrz;
    s.w = 1.0f;
}

static FLOAT _HitLineLine2D(FVECTOR& p0, FVECTOR& p1, FVECTOR& l0, FVECTOR& l1, INT iXYZ) {
    FLOAT s;
    FLOAT t;
    FLOAT bottom;
    FVECTOR m;
    FVECTOR n;
    FVECTOR p;
    FVECTOR q;
    if (iXYZ == 0) {
        m.x = p0.y;
        m.y = p0.z;
        n.x = p1.y;
        n.y = p1.z;
        p.x = l0.y;
        p.y = l0.z;
        q.x = l1.y;
        q.y = l1.z;
    } else if (iXYZ == 1) {
        m.x = p0.x;
        m.y = p0.z;
        n.x = p1.x;
        n.y = p1.z;
        p.x = l0.x;
        p.y = l0.z;
        q.x = l1.x;
        q.y = l1.z;
    } else {
        m = p0;
        n = p1;
        p = l0;
        q = l1;
    }
    bottom = (n.x - m.x) * (q.y - p.y) - (n.y - m.y) * (q.x - p.x);
    if (bottom != 0.0f) {
        bottom = 1.0f / bottom;
        s = ((m.y - p.y) * (q.x - p.x) - (m.x - p.x) * (q.y - p.y)) * bottom;
        t = ((m.y - p.y) * (n.x - m.x) - (m.x - p.x) * (n.y - m.y)) * bottom;
        if (s >= -0.05f && s <= 1.05f && t >= -0.05f && t <= 1.05f) {
            return t;
        }
    }
    return 1.7014118e38f;
}
// SOURCE DATA: .data .rodata .bss
