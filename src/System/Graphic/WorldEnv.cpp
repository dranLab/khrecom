
#define EFFECT_H_PATH "c:/KHCOM/MainApp/System/Graphic/Effect/effect.h" /* MATCH: this unit names effect.h so (header default "System/Graphic/Effect/effect.h") */
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/WorldEnv.h"
#include "System/Graphic/Camera.h"
#include "System/Graphic/Billboard.h"
#include "System/Graphic/LightUnit.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "System/Math3D.h"
#include "System/HeapMan.h"
#include "System/MainApp.h"
#include "System/Debug.h"
#include "System/Sound/SoundMan.h"
#include "Game/SceneEnv.h"
#include "Game/SceneInfo.h"
#include "include_asm.h"




#include "System/Coord.h"
// from the sce libraries
void RotTransPers(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
extern "C" {
float atan2f(float y, float x);
float tanf(float x);
float sinf(float x);
float cosf(float x);
}
extern "C" {
void sceVu0UnitMatrix(FMATRIX& m);
void sceVu0InversMatrix(FMATRIX& m0, const FMATRIX& m1);
void sceVu0RotMatrixX(FMATRIX& m0, FMATRIX& m1, float rx);
void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
void sceVu0RotMatrixZ(FMATRIX& m0, FMATRIX& m1, float rz);
void sceVu0CameraMatrix(FMATRIX& m, FVECTOR& p, FVECTOR& zd, FVECTOR& yd);
void sceVu0ViewScreenMatrix(FMATRIX& m, float scrz, float ax, float ay, float cx, float cy, float zmin, float zmax,
                            float nearz, float farz);
}
extern INT SCREEN_HEIGHT;
extern FLOAT ASPECT_RATE_X;
extern FLOAT ASPECT_RATE_Y;

namespace {
FMATRIX _matWorldView;
FMATRIX _matViewScreen;
FMATRIX _matWorldScreen;
FMATRIX _matViewClip;
FMATRIX _matWorldClip;
FVECTOR _vecViewDir;
FVECTOR _vecFogParam;
IVECTOR _ivecFogColor;
IVECTOR _ivecCalcFogColor;
FVECTOR _vecFixCameraPos;
FVECTOR _vecFixCameraZd;
FVECTOR _vecFixCameraYd;
FVECTOR _vecFixCameraRot;
FVECTOR _vecMikePos;
FVECTOR _vecSunPos;
TIMEQ _nWorldScreenTQ;
FLOAT _fScreenZ;
FLOAT _fDefScreenZ;
FLOAT _fDispFar;
FLOAT _fMikeOffsetZ;
BOOL _bMikeReady;
CFrustum* _pFrustum;
CCamera* _pCamera[16];
CEffectLightMan* _pEffLightMan;
CBillboardHelper* _pBBHelper;
}

static void _makeViewScreenMatrix();

static inline void _setViewScreenMatrix(FLOAT fInScrz, FLOAT fInAx, FLOAT fInAy, FLOAT fInFar) {
    sceVu0ViewScreenMatrix(_matViewScreen, fInScrz, fInAx, fInAy, 2048.0f, 2048.0f, 1.0f, 16777000.0f, 0.1f, fInFar);
}

static inline void _cameraMatrix(FMATRIX& matOut, FVECTOR& vInPos, FVECTOR& vInZd, FVECTOR& vInYd, FVECTOR& vInRot) {
    FMATRIX matWork;
    FVECTOR vecDir;
    FVECTOR vecV;
    FVECTOR vecPos;
    sceVu0UnitMatrix(matWork);
    sceVu0RotMatrixX(matWork, matWork, vInRot.x);
    sceVu0RotMatrixY(matWork, matWork, vInRot.y);
    sceVu0RotMatrixZ(matWork, matWork, vInRot.z);
    ApplyMatrix(vecDir, matWork, vInZd);
    ApplyMatrix(vecV, matWork, vInYd);
    ApplyMatrix(vecPos, matWork, vInPos);
    sceVu0CameraMatrix(matOut, vecPos, vecDir, vecV);
}

/* Parked: 0-8 of 177 lines (2026-11-08): this draft is the original source. Compiled with the original's own
 * anonymous-namespace name (c:/KHCOM/MainApp/System/Graphic/WorldEnv.cpp + suffix UPaaaa = pid 2588, via a rewritten .ii
 * line marker and a chosen pid) it matches every time; our build names the namespace src_..._WorldEnv.cpp + a random
 * pid suffix, which changes gcse PRE's hash order of the hoisted lui's (_vecFixCameraPos/Yd/Zd -> s0/s1/s3 rotate per
 * compile: MATCH in about 1 of 6 builds, else 5 or 8 lines; tools/drafts.py's copy has another path: 37-45).
 * Needs the original anonymous-namespace name in the build (tools change). tried: .mdebug block tree (equal except operator new's nRA scope, no code change), fixed pid alone. */
#if 0
#line 84
void CWorldEnv::initialize() {
    for (INT i = 0; i < 16; i++)
        _pCamera[i] = NULL;

    setFogParam(300.0f, 500.0f, 255.0f, 0.0f);
    _fDispFar = 520.0f;

    _ivecFogColor.set(0, 0, 0, 0);
    _vecFixCameraPos.set(0.0f, -1.0f, -4.0f, 0.0f);
    _vecFixCameraZd.set(0.0f, 0.0f, 1.0f, 1.0f);
    _vecFixCameraYd.set(0.0f, 1.0f, 0.0f, 1.0f);
    _vecFixCameraRot.set(0.0f, 0.0f, 3.1415927f, 0.0f);

    _fScreenZ = 0.0f;
    _fDefScreenZ = 0.0f;
    _fMikeOffsetZ = 0.25f;
    _nWorldScreenTQ = 0;
    _bMikeReady = FALSE;

    _pFrustum = new (__FILE__ "(103)") CFrustum;
    _pBBHelper = new (__FILE__ "(104)") CBillboardHelper;


    _pEffLightMan = new (__FILE__ "(107)") CEffectLightMan;
}
#else
/* MATCH: placeholder for initialize's .rodata (its allocation messages) */
inline void _initializeStrings() {
    com_malloc_d(0, __FILE__ "(103)", 0);
    com_malloc_d(0, __FILE__ "(104)", 0);
    com_malloc_d(0, __FILE__ "(107)", 0);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/WorldEnv", initialize__9CWorldEnv);
#endif

void CWorldEnv::finalize() {
    if (_pFrustum) delete _pFrustum;
    _pFrustum = NULL;
    if (_pBBHelper) delete _pBBHelper;
    _pBBHelper = NULL;
    if (_pEffLightMan) delete _pEffLightMan;
    _pEffLightMan = NULL;
}

static void _makeWorldViewMatrix(const FMATRIX* pInWorldMat, FLOAT fInXofs, FLOAT fInYofs);
static void _makeViewClipMatrix();
static void _makeBillboardMatrix(const FMATRIX* pWorldMat);

void CWorldEnv::mainProc() {
    CCamera* pCamera = getActiveCamera();
    BOOL bInit;
    FMATRIX* pMatWorld;
    FLOAT e;
    if (pCamera) {
        pMatWorld = &pCamera->Crd(0).getWorldMatrix();
        bInit = TRUE;
        _makeWorldViewMatrix(pMatWorld, pCamera->getOffsetX(), pCamera->getOffsetY());
        setScreenZ(pCamera->getProjectionDist());
        FVECTOR v;
        v.init0();
        v.z = _fMikeOffsetZ;
        ApplyMatrix(_vecMikePos, *pMatWorld, v);
    } else {
        pMatWorld = NULL;
        bInit = FALSE;
        _makeWorldViewMatrix(NULL, 0.0f, 0.0f);
        setScreenZ(0.0f);
        _vecMikePos.init0(); // MATCH: init0 (no "memory" clobber) - the volatile asm makes cse recompute %hi(_vecMikePos + 8)
        _vecMikePos.z = _fMikeOffsetZ;
    }
    _makeBillboardMatrix(pMatWorld);
    _makeViewScreenMatrix();
    MulMatrix(_matWorldScreen, _matViewScreen, _matWorldView);
    _makeViewClipMatrix();
    MulMatrix(_matWorldClip, _matViewClip, _matWorldView);
    e = 1.0f / tanf(atan2f(256.0f, _fScreenZ + 512.0f));
    _pFrustum->initialize(e, SCREEN_HEIGHT * (1.0f / 512.0f), 1.0f, getDispFar(), bInit);
    _nWorldScreenTQ = CMainApp::getTickCount();
}

BOOL CWorldEnv::pushCamera(CCamera& inCamera) {
    INT i;
    INT nPri;
    for (i = 0; i < 16; i++) {
        if (&inCamera == _pCamera[i]) return FALSE;
    }
    nPri = inCamera.getCameraPriority();
    for (i = 0; i < 16; i++) {
        if (!_pCamera[i]) {
            _pCamera[i] = &inCamera;
            break;
        }
        if (nPri >= _pCamera[i]->getCameraPriority()) {
            for (INT j = 15; j > i; j--) {
                _pCamera[j] = _pCamera[j - 1];
            }
            _pCamera[i] = &inCamera;
            break;
        }
    }
    return TRUE;
}

BOOL CWorldEnv::popCamera(CCamera& inCamera) {
    INT i;
    INT j;
    for (i = 0; i < 16; i++) {
        if (&inCamera == _pCamera[i]) {
            for (j = i; j < 15; j++) {
                _pCamera[j] = _pCamera[j + 1];
            }
            _pCamera[j] = NULL;
            return TRUE;
        }
    }
    return FALSE;
}

void CWorldEnv::removeAllCamera() {
    INT i;
    for (i = 0; i < 16; i++) {
        CCamera* pCamera = _pCamera[i];
        if (pCamera) {
            pCamera->out();
            _pCamera[i] = NULL;
        }
    }
}

CCamera* CWorldEnv::getActiveCamera() { return _pCamera[0]; }

CCamera* CWorldEnv::getOldCamera() { return _pCamera[1]; }

static void _makeWorldViewMatrix(const FMATRIX* pInWorldMat, FLOAT fInXofs, FLOAT fInYofs) {
    FMATRIX& matWV = _matWorldView;
    CCamera* pCamera;
    if (pInWorldMat) {
        sceVu0InversMatrix(matWV, *pInWorldMat);
        applySignWorldViewMatrix(matWV, matWV);
        matWV.m[3][0] += fInXofs;
        matWV.m[3][1] += fInYofs;
    } else {
        _cameraMatrix(matWV, _vecFixCameraPos, _vecFixCameraZd, _vecFixCameraYd, _vecFixCameraRot);
    }
    pCamera = CWorldEnv::getActiveCamera();
    if (pCamera) {
        matWV.m[3][0] += pCamera->getOffsetX();
        matWV.m[3][1] += pCamera->getOffsetY();
    }
    _vecViewDir.x = matWV.m[2][0];
    _vecViewDir.y = 0.0f;
    _vecViewDir.z = matWV.m[2][2];
    _vecViewDir.w = 0.0f;
    Normalize(_vecViewDir, _vecViewDir);
}

#line 334
static void _makeViewClipMatrix() {
    FLOAT fSrrz = _fScreenZ + 512.0f;

    FLOAT fFar = CWorldEnv::getDispFar();
    FLOAT fScrW = 512.0f / ASPECT_RATE_X;
    FLOAT fScrH = SCREEN_HEIGHT / ASPECT_RATE_Y;
    ViewClipMatrix(_matViewClip, fScrW, fScrH, fSrrz, 0.1f, fFar);
}

void CWorldEnv::setFogParam(FLOAT fNear, FLOAT fFar, FLOAT fMax, FLOAT fMin) {
    _vecFogParam.x = fNear;
    _vecFogParam.y = fFar;
    _vecFogParam.z = (fMin - fMax) / (fFar - fNear);
    _vecFogParam.w = (fMax - fMin) * fFar / (fFar - fNear) + fMin;
    CEffectMan::setFogRange(fNear, fFar);
    CEffectMan::setFogClamp((INT)fMin, (INT)fMax);
}

FVECTOR& CWorldEnv::getFogParam() { return _vecFogParam; }

FLOAT CWorldEnv::getFogNear() { return _vecFogParam.x; }

FLOAT CWorldEnv::getFogFar() { return _vecFogParam.y; }

void CWorldEnv::setFogColor(INT r, INT g, INT b) {
    _ivecFogColor.x = r;
    _ivecFogColor.y = g;
    _ivecFogColor.z = b;
    _ivecFogColor.w = 0;
}

IVECTOR& CWorldEnv::getFogColor() {
    FVECTOR* pSceneColor = CSceneEnv::getTypeColor((ObjType::eObjType)12);
    if (pSceneColor) {
        FVECTOR fvColor;
        ITOF0Vector(fvColor, _ivecFogColor);
        MulVector(fvColor, fvColor, *pSceneColor);
        FTOI0Vector(_ivecCalcFogColor, fvColor);
    } else {
        _ivecCalcFogColor = _ivecFogColor;
    }
    return _ivecCalcFogColor;
}

FLOAT CWorldEnv::getDispFar() {
    if (_fDispFar != 0.0f) return _fDispFar;
    return getFogFar() + 20.0f;
}

void CWorldEnv::setDispFar(FLOAT far) { _fDispFar = far; }

void CWorldEnv::setScreenZ(FLOAT z) { _fScreenZ = z; }

FLOAT CWorldEnv::getScreenZ() { return _fScreenZ; }

void CWorldEnv::setDefScreenZ(FLOAT z) { _fDefScreenZ = z; }

FLOAT CWorldEnv::getDefScreenZ() { return _fDefScreenZ; }

void CWorldEnv::getViewVector(FVECTOR& vecOut) { vecOut = _vecViewDir; }

const FVECTOR& CWorldEnv::getViewVector() { return _vecViewDir; }

FMATRIX& CWorldEnv::getWorldViewMatrix() { return _matWorldView; }

FMATRIX& CWorldEnv::getViewScreenMatrix() { return _matViewScreen; }

FMATRIX& CWorldEnv::getWorldScreenMatrix() { return _matWorldScreen; }

FMATRIX& CWorldEnv::getViewClipMatrix() { return _matViewClip; }

FMATRIX& CWorldEnv::getWorldClipMatrix() { return _matWorldClip; }

TIMEQ CWorldEnv::getWorldScreenTimestamp() { return _nWorldScreenTQ; }

FVECTOR& CWorldEnv::getEyePos() { return (FVECTOR&)getBBHelper().getEyePos(); }

const FVECTOR& CWorldEnv::getMikePos() { return _vecMikePos; }

void CWorldEnv::readyMike(BOOL bReady, BOOL bSetup) {
    _bMikeReady = bReady;
    if (bReady && bSetup) {
        UINT nRange = CSceneInfo::getSeRangeRatio() * 10;
        CSound::setSeRangeRatio(nRange);
        INT nReverb = CSceneInfo::getReverbVal();
        CSound::setReverbDepth(nReverb);
        TRACE_COL((TtyColor)0x1f, "=*=*=* SE_RANGE->[%d%%], REVERB->[%d] *=*=*=\n", nRange, nReverb);
    }
}

BOOL CWorldEnv::isReadyMike() { return _bMikeReady; }

void CWorldEnv::setSunPos(const FVECTOR& vecInPos) { _vecSunPos = vecInPos; }

FVECTOR& CWorldEnv::getSunPos() { return _vecSunPos; }

void CWorldEnv::getSunPosScr(FVECTOR& vecOut) {
    FVECTOR& vecSun = getSunPos();
    RotTransPers(vecOut, _matWorldScreen, vecSun);
    FVECTOR vecToEye;
    SubVector(vecToEye, _pBBHelper->getEyePos(), vecSun);
    vecOut.w = InnerProduct(vecToEye, _vecViewDir);
}

BOOL CWorldEnv::sphereVisible(FVECTOR& inCenter, FLOAT inRadius) { return _pFrustum->sphereVisible(inCenter, inRadius); }

BOOL CWorldEnv::ellipsoidVisible(FVECTOR& inCenter, FVECTOR& inR, FVECTOR& inS, FVECTOR& inT) {
    return _pFrustum->ellipsoidVisible(inCenter, inR, inS, inT);
}

BOOL CWorldEnv::cylinderVisible(FVECTOR& inP0, FVECTOR& inP1, FLOAT inRadius) {
    return _pFrustum->cylinderVisible(inP0, inP1, inRadius);
}

CEffectLight* CWorldEnv::getBlankEffLight() { return _pEffLightMan->getBlankLight(); }

CEffectLight* CWorldEnv::getEffLight(UINT nIndex) { return _pEffLightMan->getLight(nIndex); }

void CWorldEnv::initSeekActiveEffLight() { _pEffLightMan->initSeekActiveLight(); }

CEffectLight* CWorldEnv::seekNextActiveEffLight() { return _pEffLightMan->seekNextActiveLight(); }

void CWorldEnv::resetEffLight() { _pEffLightMan->reset(); }

static void _makeBillboardMatrix(const FMATRIX* pWorldMat) {
    if (pWorldMat) {
        FVECTOR vecPos;
        FVECTOR vecView;
        vecPos.set(-pWorldMat->m[3][0], -pWorldMat->m[3][1], pWorldMat->m[3][2]);
        vecView.set(pWorldMat->m[2][0], pWorldMat->m[2][1], -pWorldMat->m[2][2]);
        _pBBHelper->setEyePos(vecPos, &vecView);
    } else {
        _pBBHelper->setEyePos(_vecFixCameraPos, &_vecFixCameraZd);
    }
}

#line 711
CBillboardHelper& CWorldEnv::getBBHelper() {
    if (!_pBBHelper) AssertApp(__FILE__, __LINE__, "_pBBHelper", 0);
    return *_pBBHelper;
}

void CWorldEnv::getBillboardMatrix(FMATRIX& matOut, const FVECTOR& vecPos, INT nFlag) {
    if (nFlag < 0) nFlag = 1; /* MATCH: the parameter defaulted in place (slt against a -1 register + movn of the shared 1) */
    _pBBHelper->getBillboardMatrix(matOut, vecPos, nFlag);
}

void applySignWorldViewMatrix(FMATRIX& m0, const FMATRIX& m1) {
    asm volatile("qmtc2.ni %2, $vf8\n"
                 "lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x10(%1)\n"
                 "lqc2 $vf6, 0x20(%1)\n"
                 "lqc2 $vf7, 0x30(%1)\n"
                 "vmulx.xy $vf4, $vf4, $vf8x\n"
                 "vmulx.xy $vf5, $vf5, $vf8x\n"
                 "vmulx.xy $vf6, $vf6, $vf8x\n"
                 "vmulx.xy $vf7, $vf7, $vf8x\n"
                 "sqc2 $vf4, 0x0(%0)\n"
                 "sqc2 $vf5, 0x10(%0)\n"
                 "sqc2 $vf6, 0x20(%0)\n"
                 "sqc2 $vf7, 0x30(%0)\n"
                 :
                 : "r"(&m0), "r"(&m1), "r"(-1.0f)
                 : "memory");
}

static inline FMATRIX& _unitMatrix(FMATRIX& m) {
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
    return m;
}

void getViewportMatrix(FMATRIX& matViewport, float cx, float cy, float width, float height, float maxz, float minz) {
    FMATRIX& res = _unitMatrix(matViewport);
    res.m[3][0] = cx;
    res.m[3][1] = cy;
    res.m[0][0] = width * 0.5f;
    res.m[1][1] = height * 0.5f;
    maxz -= minz;
    maxz *= 0.5f;
    res.m[3][2] = maxz + minz;
    res.m[2][2] = -maxz;
}

void getPerspectiveMatrix(FMATRIX& m, float scrz, float ax, float ay, float width, float height, float r_near,
                          float r_far) {
    float nf1 = 1.0f / (r_near - r_far);
    float nf2 = r_near + r_far;
    m.m[0][0] = ax * scrz * (1.0f / (width * 0.5f));
    m.m[0][1] = 0.0f;
    m.m[0][2] = 0.0f;
    m.m[0][3] = 0.0f;
    m.m[1][0] = 0.0f;
    m.m[1][1] = ay * scrz * (1.0f / (height * 0.5f));
    m.m[1][2] = 0.0f;
    m.m[1][3] = 0.0f;
    m.m[2][0] = 0.0f;
    m.m[2][1] = 0.0f;
    m.m[2][2] = -nf1 * nf2;
    m.m[2][3] = 1.0f;
    m.m[3][0] = 0.0f;
    m.m[3][1] = 0.0f;
    m.m[3][2] = (r_near + r_near) * r_far * nf1;
    m.m[3][3] = 0.0f;
}

void getPerspectiveMatrix(FMATRIX& m, float fovy, float aspect, float r_near, float r_far) {
    float f = fovy * 0.5f;
    f = cosf(f) / sinf(f);
    float asp = 1.0f / aspect;
    float nf1 = 1.0f / (r_near - r_far);
    float nf2 = r_near + r_far;
    m.m[0][0] = f;
    m.m[0][1] = 0.0f;
    m.m[0][2] = 0.0f;
    m.m[0][3] = 0.0f;
    m.m[1][0] = 0.0f;
    m.m[1][1] = f * asp;
    m.m[1][2] = 0.0f;
    m.m[1][3] = 0.0f;
    m.m[2][0] = 0.0f;
    m.m[2][1] = 0.0f;
    m.m[2][2] = -nf1 * nf2;
    m.m[2][3] = 1.0f;
    m.m[3][0] = 0.0f;
    m.m[3][1] = 0.0f;
    m.m[3][2] = (r_near + r_near) * r_far * nf1;
    m.m[3][3] = 0.0f;
}

void getOrthoMatrix(FMATRIX& m, float left, float right, float bottom, float top, float r_near, float r_far) {
    float rl1 = 1.0f / (right - left);
    float tb1 = 1.0f / (top - bottom);
    float fn1 = 1.0f / (r_far - r_near);
    float rl2 = right + left;
    float tb2 = top + bottom;
    float fn2 = r_far + r_near;
    m.m[0][0] = rl1 + rl1;
    m.m[0][1] = 0.0f;
    m.m[0][2] = 0.0f;
    m.m[0][3] = 0.0f;
    m.m[1][0] = 0.0f;
    m.m[1][1] = tb1 + tb1;
    m.m[1][2] = 0.0f;
    m.m[1][3] = 0.0f;
    m.m[2][0] = 0.0f;
    m.m[2][1] = 0.0f;
    m.m[2][2] = fn1 + fn1;
    m.m[2][3] = 0.0f;
    m.m[3][0] = -rl2 * rl1;
    m.m[3][1] = -tb2 * tb1;
    m.m[3][2] = -fn2 * fn1;
    m.m[3][3] = 1.0f;
}

#line 951
static void _makeViewScreenMatrix() {
    FLOAT fSrrz = _fScreenZ + 512.0f;

    _setViewScreenMatrix(fSrrz, ASPECT_RATE_X, ASPECT_RATE_Y, CWorldEnv::getDispFar());
}

// SOURCE DATA: .rodata .bss
