#define GAME_PLAYER_PLAYERCAMERA_CPP

#include "System/Fsm.h" /* the full CFSM (before Game/Entity.h) */
/* .rodata starts with the strings of the included headers' inline functions, in this order. */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"

/* Placeholders: two empty strings follow CardBattle.h's strings in .rodata. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() {
    TRACE("");
}

/* System/TaskMan.h's container overflow message comes after them here. */
#include "System/TaskMan.h"



#include "Game/Player/PlayerCamera.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerManager.h"
#include "Game/Game.h"
#include "Game/GameWork.h"
#include "Game/SceneInfo.h"
#include "Game/SceneEnv.h"
#include "Game/Movement.h"
#include "Game/Debug/DebugWork.h"
#include "System/TaskMan.h"
#include "System/HeapMan.h"
#include "Form/DebugWindow.h"
#include "Game/Player/DWTPlayer.h"
#include "include_asm.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" float atan2f(float y, float x);
extern "C" void sceVu0UnitMatrix(FMATRIX& m);
extern "C" void sceVu0RotMatrixX(FMATRIX& m0, FMATRIX& m1, float rx);
extern "C" void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, float s);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
float InnerProduct(const FVECTOR& v0, const FVECTOR& v1);
void ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void ApplyMatrix33(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
extern "C" void sceVu0InversMatrix(FMATRIX& m0, const FMATRIX& m1);
extern "C" float sinf(float x);
extern "C" float cosf(float x);

inline void* operator new(size_t, void* p) throw() { return p; }

static inline UINT RET_ADDR() {
    UINT nRA;
    __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA));
    return nRA;
}

static inline FLOAT _fabsf(FLOAT x) {
    FLOAT ret;
    asm volatile("abs.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}

static inline FLOAT _limitPI(FLOAT inValue) {
    FLOAT ret;
    if (inValue < -3.1415927f) ret = inValue + 6.2831855f;
    else {
        ret = inValue;
        if (ret > 3.1415927f) ret -= 6.2831855f;
    }
    return ret;
}

/* Through inline wrappers (a header's, in the original): called directly, cse keeps the matrix address in a saved
 * register across the calls; the original recomputes it for each call. */
static inline void _unitMatrix(FMATRIX& m) {
    sceVu0UnitMatrix(m);
}
static inline void _rotMatrixX(FMATRIX& m, FLOAT r) {
    sceVu0RotMatrixX(m, m, r);
}
static inline void _inversMatrix(FMATRIX& m0, const FMATRIX& m1) {
    sceVu0InversMatrix(m0, m1);
}
static inline void _rotMatrixY(FMATRIX& m, FLOAT r) {
    sceVu0RotMatrixY(m, m, r);
}

static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}







class CRsrcData;

#include "System/RsrcDB.h"





#include "System/Graphic/PackMan.h"




static inline void* _getRsrcData(const char* inName) {
    CRsrcData* pData = CRsrcDB::getRsrc(1, inName, NAME_CASE_SENSITIVE);
    return pData ? CRsrcDB::getRsrcData(*pData) : NULL;
}

namespace PlayerCamera {

static LPCSTR g_szBinBCamXXXX = "BCam%04d";
static LPCSTR g_szBinPCamData = "PCamData";
static char g_szBinBCam[16] = "BCam0000";

CPlayerCamera* CPlayerCamera::m_pInstance = NULL;
INT CPlayerCamera::m_nBossCamID = 0;

char g_strtmp[256] = "";

extern PCAM_PARAM g_ParamTest;

char* szTestCamMode[6] = {
    "OFF", "1:FLD-Normal", "2:(FLD-Far)", "3:BTL-Normal", "4:BTL-Lockon", "5:BTL-Loading",
};

PCAM_DATA* pTestCamData[6] = {
    NULL,
    &g_ParamTest.FieldNormal,
    &g_ParamTest.FieldFar,
    &g_ParamTest.BattleNormal,
    &g_ParamTest.BattleLockon,
    &g_ParamTest.BattleLoading,
};

PCAM_PARAM g_ParamTest;
PCAM_DATA* g_pCamData[6];

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerCamera", getPlayerCameraDir__Q212PlayerCamera13CPlayerCamera);
#if 0
/* Only the scheduling of the first block differs: the original zeroes dir (por/sq) before loading m_pInstance
 * (the zero gets $v1, then m_pInstance), ours loads m_pInstance first. */
FVECTOR CPlayerCamera::getPlayerCameraDir() {
    FVECTOR dir;
    dir.init();
    if (m_pInstance) {
        dir = m_pInstance->getNowCamDir();
        if (m_pInstance->m_iMode == 2) {
            dir.y += 0.7853982f;
            dir.x += 0.50265485f;
        } else {
            dir.x = 0.50265485f;
        }
    }
    return dir;
}
#endif

INT CPlayerCamera::getPlayerCameraPos(FVECTOR& outPos) {
    if (m_pInstance) {
        outPos = m_pInstance->getNowCamEye();
        return 1;
    }
    return 0;
}

void CPlayerCamera::setBossCamFile(INT inID) {
    sprintf(g_szBinBCam, g_szBinBCamXXXX, inID);
}

void CPlayerCamera::setBossCamID(INT inID) {
    if (inID > 0) {
        setBossCamFile(inID);
        CRsrcData* pData = (CRsrcData*)_getRsrcData(g_szBinBCam);
        if (pData) m_nBossCamID = inID;
    }
}

void CPlayerCamera::setBattleCamData(HANDLE inHandle, const BCAM_PARAM* inParam) {
    if (!m_pInstance) return;
    if (inHandle > 0) {
        m_pInstance->m_hOwnerBCAM = inHandle;
        g_pCamData[3] = (PCAM_DATA*)&inParam->Normal;
        g_pCamData[4] = (PCAM_DATA*)&inParam->Lockon;
    } else {
        if (m_pInstance->m_hOwnerBCAM <= 0) return;
        g_pCamData[3] = &m_pInstance->m_pParam->BattleNormal;
        g_pCamData[4] = &m_pInstance->m_pParam->BattleLockon;
        if (m_nBossCamID > 0) {
            setBossCamFile(m_nBossCamID);
            BCAM_PARAM* pData = (BCAM_PARAM*)_getRsrcData(g_szBinBCam);
            if (pData) {
                g_pCamData[3] = &pData->Normal;
                g_pCamData[4] = &pData->Lockon;
            }
        }
        m_pInstance->m_hOwnerBCAM = 0;
    }
    pTestCamData[3] = g_pCamData[3];
    pTestCamData[4] = g_pCamData[4];
}

void CPlayerCamera::getBattleCamData(BCAM_PARAM& outParam) {
    if (m_pInstance && m_pInstance->m_hOwnerBCAM > 0) {
        outParam.Normal = *g_pCamData[3];
        outParam.Lockon = *g_pCamData[4];
    }
}

#line 228
CPlayerCamera::CPlayerCamera(CTask* pParent) : CCamera(pParent, -1), m_uiFlag(0) {
    m_pInstance = this;
    m_pParam = (PCAM_PARAM*)_getRsrcData(g_szBinPCamData);
    if (!(m_pParam)) AssertApp(__FILE__, __LINE__, "m_pParam", 0);

    m_pPlayer = (CPlayer*)pParent;
    if (!(m_pPlayer)) AssertApp(__FILE__, __LINE__, "m_pPlayer", 0);

#line 241
    m_hNowPlayer = 0;
    m_pNowPlayer = NULL;
    m_hNowTarget = 0;
    m_pNowTarget = NULL;
    m_vPlayer.init();
    m_vTarget.init();

#line 249
    m_hOwnerBCAM = 0;

    m_iState = -1;

    m_pLines = NULL;

    g_ParamTest = *m_pParam;
    m_pParam = &g_ParamTest;

    sprintf(m_strCamera0, " ");
    sprintf(m_strCamera1, " ");
    sprintf(m_strCamera2, " ");
    sprintf(m_strCamera3, " ");
    sprintf(m_strCamera4, " ");
    DebugWork::g_dw.player.m_szCamera[0] = m_strCamera0;
    DebugWork::g_dw.player.m_szCamera[1] = m_strCamera1;
    DebugWork::g_dw.player.m_szCamera[2] = m_strCamera2;
    DebugWork::g_dw.player.m_szCamera[3] = m_strCamera3;
    DebugWork::g_dw.player.m_szCamera[4] = m_strCamera4;
#line 269
    m_pWindow = new (com_malloc_d(sizeof(CDebugWindow), __FILE__ "(269)", RET_ADDR())) CDebugWindow(this, DWTPlayer::debugPlayerCameraInfo);
    m_pWindow->dispOff(0);
    DebugWork::g_dw.player.iCamMode = 1;
    g_pCamData[0] = NULL;
    g_pCamData[1] = &m_pParam->FieldNormal;
    g_pCamData[2] = &m_pParam->FieldFar;
    g_pCamData[3] = &m_pParam->BattleNormal;
    g_pCamData[4] = &m_pParam->BattleLockon;
    g_pCamData[5] = &m_pParam->BattleLoading;
    if (m_nBossCamID > 0) {
        setBossCamFile(m_nBossCamID);
        BCAM_PARAM* pData = (BCAM_PARAM*)_getRsrcData(g_szBinBCam);
        if (pData) {
            g_pCamData[3] = &pData->Normal;
            g_pCamData[4] = &pData->Lockon;
            m_uiFlag |= 0x100;
        }
    }
    pTestCamData[3] = g_pCamData[3];
    pTestCamData[4] = g_pCamData[4];
    m_iModeFieldNear = 1;
    m_iModeFieldFar = 2;
    m_fRikuShiftY = 0.0f;
    m_fRikuShiftY = (DebugWork::g_dw.player.iCamRikuShiftY && CGame::isRikuGame()) ? 0.4f : 0.0f;
    _initialize();
}

CPlayerCamera::~CPlayerCamera() {
    if (m_pLines) delete m_pLines;
    m_pLines = NULL;
    DebugWork::g_dw.player.iCamMode = 0;
    m_pInstance = NULL;
    m_nBossCamID = 0;
    m_tqEffect = 0;
    CPackMan::setFrameEffect(0, 0, 0, 0);
    setFlag(0, 0x10);
}

#line 347
void CPlayerCamera::changeState(INT inState) {
    if (!(0 <= inState && inState < STATE_MAX)) AssertApp(__FILE__, __LINE__, "0 <= inState && inState < STATE_MAX", 0);
    gotoState(m_State[inState]);
}

#line 349
void CPlayerCamera::_initialize() {
    m_State[STATE_FIELD].set(this, &CPlayerCamera::preStateField, &CPlayerCamera::stateField, NULL);
    m_State[STATE_BATTLE].set(this, &CPlayerCamera::preStateBattle, &CPlayerCamera::stateBattle, NULL);
    m_iState = 0;
    m_hLockon = 0;
    m_collMap.init((COLL_KIND)0x8000000C, COLL_TYPE_SPHERE, 0, getHandle());
    m_collMap.enableSliding(0);
    m_tqEffect = 0;
    m_fDist = 0.0f;
    m_iPreMode = 0;
    m_iMode = 0;
    m_fReverseY = !CGameWork::getByte(0, 0x65) ? 1.0f : -1.0f;
    m_fReverseX = !CGameWork::getByte(0, 0x66) ? 1.0f : -1.0f;
    BOOL bIsBattleScene = CSceneInfo::isBattleScene() | CSceneInfo::isMinigameScene();
    m_fYLimitHi = 3.1415927f;
    if (CGame::isRikuGame() && !(bIsBattleScene && (USHORT)(CSceneInfo::getCurrentSceneNum() - 1050) < 149)) {
        DebugWork::g_dw.player.iCamRikuShiftY = 1;
        m_fRikuShiftY = 0.4f;
    } else {
        DebugWork::g_dw.player.iCamRikuShiftY = 0;
        m_fRikuShiftY = 0.0f;
    }
    if (!bIsBattleScene) {
        m_fXLimitHi = -0.87266463f;
        m_fXLimitLow = 0.20943952f;
        changeState(STATE_FIELD);
        INT worldID = CSceneInfo::getCurrentWorldID();
        INT roomID = CSceneInfo::getCurrentRoomID();
        if (worldID == 10 || roomID == 0 || roomID == 99 || (worldID == 13 && roomID == 20)) {
            initCamera(1, 0, NULL);
        } else {
            initCamera(1, 1, NULL);
        }
    } else {
        m_fXLimitHi = -0.7853982f;
        m_fXLimitLow = 0.5235988f;
        if (g_pCamData[3]->fDistance > 0.0f) m_fXLimitHi = -0.87266463f;
        changeState(STATE_BATTLE);
        initCamera(3, 0, NULL);
    }
    m_iPreMode = m_iMode;
}
#line 454
void CPlayerCamera::initCamera(INT inMode, INT inReset, FVECTOR* inDir) {
    FVECTOR pos;
    FVECTOR eye;
    FVECTOR aim;
    FVECTOR vec;
    FVECTOR dir;
    FLOAT ret;

    m_iMode = inMode;
    m_pData = g_pCamData[inMode];

#line 460
    if (!(m_pData)) AssertApp(__FILE__, __LINE__, "m_pData", 0);

    setGakaku(m_pData->fViewAngle);
    m_vOffset.init();
    m_vPos.init();
    m_tqTimer = 0;
    m_tqNear = 0;
    if (inReset == 1) {
        m_pPlayer->updateCoord();
        m_pPlayer->coordFix();
    }
    m_fRikuShiftY = (DebugWork::g_dw.player.iCamRikuShiftY && CGame::isRikuGame()) ? 0.4f : 0.0f;
    pos = getNowPlayerPos();
    eye = m_pData->vEye;
    eye.y += m_fRikuShiftY;
    aim = m_pData->vAim;
    aim.y += m_fRikuShiftY;
    SubVectorXYZ(m_vLocal, eye, aim);
    ret = _sqrtf(InnerProduct(m_vLocal, m_vLocal));
    m_fDist = ret;
    switch (m_iMode) {
    case 1:
        if (inReset == 1) {
            FMATRIX mat;
            if (inDir) {
                dir = *inDir;
            } else {
                dir.x = 0.0f;
                dir.y = 0.7853982f;
                dir.z = 0.0f;
                dir.w = 1.0f;
            }
            _unitMatrix(mat);
            _rotMatrixX(mat, dir.x);
            _rotMatrixY(mat, dir.y);
            ApplyMatrixXYZ(m_vFinalAim, mat, aim);
            AddVectorXYZ(m_vFinalAim, pos, m_vFinalAim);
            m_vAim.set(m_vFinalAim.x, m_vFinalAim.y, m_vFinalAim.z);
            _unitMatrix(mat);
            _rotMatrixX(mat, dir.x);
            _rotMatrixY(mat, dir.y);
            ApplyMatrixXYZ(m_vFinalEye, mat, m_vLocal);
            AddVectorXYZ(m_vFinalEye, m_vFinalAim, m_vFinalEye);
            m_vEye.set(m_vFinalEye.x, m_vFinalEye.y, m_vFinalEye.z);
            calcDirection(m_vFinalDir, m_vAim, m_vEye);
            m_vDir.set(0.0f, m_vFinalDir.y, m_vFinalDir.z);
            break;
        }
    case 3:
    case 5: {
        FMATRIX mat;
        FLOAT r = _limitPI(getNowPlayerDir() - 3.1415927f);
        _unitMatrix(mat);
        _rotMatrixY(mat, r);
        ApplyMatrixXYZ(m_vFinalAim, mat, aim);
        AddVectorXYZ(m_vFinalAim, pos, m_vFinalAim);
        m_vAim.set(m_vFinalAim.x, m_vFinalAim.y, m_vFinalAim.z);
        _unitMatrix(mat);
        _rotMatrixY(mat, r);
        ApplyMatrixXYZ(m_vFinalEye, mat, m_vLocal);
        AddVectorXYZ(m_vFinalEye, m_vFinalAim, m_vFinalEye);
        m_vEye.set(m_vFinalEye.x, m_vFinalEye.y, m_vFinalEye.z);
        calcDirection(m_vFinalDir, m_vAim, m_vEye);
        m_vDir.set(0.0f, m_vFinalDir.y, m_vFinalDir.z);
        break;
    }
    case 4: {
        FMATRIX mat;
        m_vLocal = eye;
        FVECTOR loc = getNowTargetPos();
        AddVectorXYZ(m_vFinalAim, loc, pos);
        ScaleVectorXYZ(m_vFinalAim, m_vFinalAim, 0.5f);
        AddVectorXYZ(m_vFinalAim, m_vFinalAim, aim);
        m_vAim.set(m_vFinalAim.x, m_vFinalAim.y, m_vFinalAim.z);
        calcDirection(m_vDir, m_vAim, m_vEye);
        FVECTOR dir;
        calcDirection(dir, aim, eye);
        _unitMatrix(mat);
        _rotMatrixY(mat, m_vDir.y);
        ApplyMatrixXYZ(eye, mat, eye);
        AddVectorXYZ(eye, pos, eye);
        FVECTOR rot;
        calcDirection(rot, m_vAim, eye);
        SubVectorXYZ(rot, rot, dir);
        m_vDir.x = -rot.x;
        _unitMatrix(mat);
        _rotMatrixX(mat, m_vDir.x);
        _rotMatrixY(mat, m_vDir.y);
        ApplyMatrixXYZ(m_vFinalEye, mat, m_vLocal);
        AddVectorXYZ(m_vFinalEye, pos, m_vFinalEye);
        m_vEye.set(m_vFinalEye.x, m_vFinalEye.y, m_vFinalEye.z);
        calcDirection(m_vFinalDir, m_vAim, m_vEye);
        m_vDir.set(m_vFinalDir.x, m_vFinalDir.y, m_vFinalDir.z);
        break;
    }
    default:
        AddVectorXYZ(m_vFinalAim, pos, aim);
        AddVectorXYZ(m_vFinalEye, pos, eye);
        m_vAim.set(m_vFinalAim.x, m_vFinalAim.y, m_vFinalAim.z);
        m_vEye.set(m_vFinalEye.x, m_vFinalEye.y, m_vFinalEye.z);
        m_vFinalDir.init();
        m_vDir.set(m_vFinalDir.x, m_vFinalDir.y, m_vFinalDir.z);
        break;
    }
    m_collMap.setRadius(m_pData->fSize);
    m_collMap.setShiftY(0.0f);
    m_vEye.w = 1.0f;
    m_vAim.w = 1.0f;
    setPos(m_vEye);
    lookAt(m_vAim, (const FVECTOR*)NULL);
    m_vPreEye = m_vEye;
    m_vPreAim = m_vAim;
    if (DebugWork::g_dw.player.iCameraInfo) {
        FLOAT deltaRingAngle;
        G_LINE line;
        FVECTOR ofs;
        FMATRIX m;
        if (m_pLines) delete m_pLines;
        m_pLines = NULL;
#line 608
        m_pLines = new (com_malloc_d(sizeof(C3DLines), __FILE__ "(608)", RET_ADDR())) C3DLines;
        if (!(m_pLines)) AssertApp(__FILE__, __LINE__, "m_pLines", 0);
        m_pLines->m_pCrd = &Crd(0);
        _inversMatrix(m, Crd(0).getWorldMatrix());
        ApplyMatrix33(ofs, m, m_vEye);
        line.C0.col = 0xFF00FF00;
        line.C1.col = 0xFF00FF00;
        deltaRingAngle = 0.62831855f;
        INT ring;
        INT segNum = 11;
        for (ring = 0; ring < 5; ring++) {
            FLOAT r0 = sinf(ring * deltaRingAngle);
            FLOAT r1 = sinf((ring + 1) * deltaRingAngle);
            FLOAT y0 = cosf(ring * deltaRingAngle);
            FLOAT y1 = cosf((ring + 1) * deltaRingAngle);
            INT seg;
            for (seg = 0; seg < segNum; seg++) {
                FLOAT theta = seg * deltaRingAngle;
                FLOAT x0 = r0 * sinf(theta);
                FLOAT z0 = r0 * cosf(theta);
                FLOAT x1 = r1 * sinf(theta);
                FLOAT z1 = r1 * cosf(theta);
                line.P0.set(x0, y0, z0);
                AddVectorXYZ(line.P0, line.P0, ofs);
                ScaleVectorXYZ(line.P0, line.P0, m_pData->fSize);
                line.P1.set(x1, y1, z1);
                AddVectorXYZ(line.P1, line.P1, ofs);
                ScaleVectorXYZ(line.P1, line.P1, m_pData->fSize);
                m_pLines->m_vLINE.push_back(line);
            }
        }
    } else {
        if (m_pLines) delete m_pLines;
        m_pLines = NULL;
    }
}

void CPlayerCamera::preStateField() {
    m_iState = 0;
}

void CPlayerCamera::stateField() {
    if (!isFlag(1) && isButtonTrg(0x400)) initCamera(m_iMode, 0, NULL);
    if (m_tqTimer >= 3000 && m_iMode == 2 && !m_pPlayer->isNowIdleAnim()) {
        setFrameEffect(1, 1000);
        initCamera(m_iModeFieldNear, 0, NULL);
    }
    switch (m_iMode) {
    case 2:
        procFieldParallel();
        break;
    case 1:
        procBattleNormal();
        break;
    default:
        procFieldNormal();
        break;
    }
    m_vEye.w = 1.0f;
    m_vAim.w = 1.0f;
    collideMapTrueCamera(m_vEye, m_vAim);
    setPos(m_vEye);
    lookAt(m_vAim, (const FVECTOR*)NULL);
}

void CPlayerCamera::preStateBattle() {
    m_iState = 1;
}

void CPlayerCamera::stateBattle() {
    if (m_hOwnerBCAM > 0 && !CTaskMan::getTask(m_hOwnerBCAM)) setBattleCamData(0, NULL);
    if (!m_pNowPlayer) {
        if (!isFlag(1) && isButtonTrg(0x400)) initCamera(m_iMode, 0, NULL);
        m_hLockon = m_pPlayer->isNowLockon();
        switch (m_iMode) {
        case 5:
            break;
        case 4:
            if (m_hLockon <= 0) changeModeBattle(3);
            break;
        default:
            if (m_hLockon > 0) changeModeBattle(4);
            break;
        }
    }
    if (m_iMode == 4) {
        procBattleLockon();
    } else {
        procBattleNormal();
    }
    m_vEye.w = 1.0f;
    m_vAim.w = 1.0f;
    collideMapTrueCamera(m_vEye, m_vAim);
    setPos(m_vEye);
    lookAt(m_vAim, (const FVECTOR*)NULL);
}

void CPlayerCamera::main() {
    if (CSceneEnv::getStopTypeBit() & 0x8000) return;
    m_vPreAim = m_vAim;
    m_vPreEye = m_vEye;
    checkNowPlayer();
    m_fReverseY = !CGameWork::getByte(0, 0x65) ? 1.0f : -1.0f;
    m_fReverseX = !CGameWork::getByte(0, 0x66) ? 1.0f : -1.0f;
    update();
    exeFrameEffect();
    m_tqTimer += getFrameRate();
    if (DebugWork::g_dw.player.iCameraInfo) {
        sprintf(m_strCamera0, "Mode %s", szTestCamMode[m_iMode]);
        sprintf(m_strCamera1, "Aim:  % 10.6f,% 10.6f,% 10.6f", m_vAim.x, m_vAim.y, m_vAim.z);
        sprintf(m_strCamera2, "Eye:  % 10.6f,% 10.6f,% 10.6f", m_vEye.x, m_vEye.y, m_vEye.z);
        sprintf(m_strCamera3, "Dir:  % 10.6f,% 10.6f,% 10.6f", m_vDir.x, m_vDir.y, m_vDir.z);
        sprintf(m_strCamera4, "F-Dir:% 10.6f,% 10.6f,% 10.6f", m_vFinalDir.x, m_vFinalDir.y, m_vFinalDir.z);
        m_pWindow->dispOn(0);
    } else {
        m_pWindow->dispOff(0);
    }
}

void CPlayerCamera::disp() {
    if (m_pLines) m_pLines->disp();
}

void CPlayerCamera::calcDirection(FVECTOR& outDir, const FVECTOR& inAim, const FVECTOR& inEye) {
    FVECTOR vec;
    SubVectorXYZ(vec, inAim, inEye);
    Normalize(vec, vec);
    FLOAT ret = _sqrtf(vec.z * vec.z + vec.x * vec.x);
    outDir.x = _limitPI(-atan2f(vec.y, ret));
    outDir.y = _limitPI(atan2f(vec.x, vec.z) - 3.1415927f);
    outDir.z = 0.0f;
}

void CPlayerCamera::adjustCamera(FVECTOR& outPos, const FVECTOR& inFinal, FLOAT inRate) {
    FLOAT scale = inRate * getFrameRateF();
    if (scale <= 0.0f) return;
    if (scale >= 1.0f) {
        outPos = inFinal;
        return;
    }
    FVECTOR vec;
    SubVectorXYZ(vec, inFinal, outPos);
    vec.w = 1.0f;
    ScaleVectorXYZ(vec, vec, scale);
    FLOAT ret = _fabsf(InnerProduct(vec, vec));
    if (ret < scale * 1.0000001e-06f) {
        outPos = inFinal;
    } else {
        AddVectorXYZ(outPos, outPos, vec);
    }
}

INT CPlayerCamera::calcMapHit(FVECTOR& outPos, const FVECTOR& inStart, FLOAT inSize, FLOAT inDist) {
    INT result = 0;
    m_collMap.setStartPos((FVECTOR&)inStart);
    m_collMap.setEndPos((FVECTOR&)outPos);
    m_collMap.setType(COLL_TYPE_SPHERE);
    m_collMap.setRadius(inSize);
    m_collMap.setShiftY(0.0f);
    if (m_collMap.collideMap()) {
        FVECTOR* pOut = m_collMap.getDetectPos();
        if (!pOut) return 0;
        outPos = *pOut;
        m_collMap.setEndPos((FVECTOR&)outPos);
        m_uiFlag |= 0x400;
        result = 1;
        if (inDist > 0.0f) {
            FVECTOR vec;
            SubVectorXYZ(vec, m_vAim, outPos);
            vec.w = 1.0f;
            if (InnerProduct(vec, vec) < inDist * inDist) {
                m_tqNear = 1500;
                m_uiFlag |= 0x80;
            }
        }
    }
    return result;
}

void CPlayerCamera::recalcNearMove() {
    if (!isFlag(0x80)) return;
    FLOAT y = getFrameRateF() * -3.0f;
    m_vDir.x = _limitPI(m_vDir.x + y * 3.1415927f / 180.0f);
    if (m_vDir.x < m_fXLimitHi) m_vDir.x = m_fXLimitHi;
    FMATRIX mat;
    sceVu0UnitMatrix(mat);
    sceVu0RotMatrixX(mat, mat, m_vDir.x);
    sceVu0RotMatrixY(mat, mat, m_vDir.y);
    ApplyMatrixXYZ(m_vEye, mat, m_vLocal);
    AddVectorXYZ(m_vEye, m_vFinalAim, m_vEye);
    calcMapHit(m_vEye, m_vAim, m_pData->fSize, 0.0f);
}

INT CPlayerCamera::calcMapHit2(FVECTOR& outPos, const FVECTOR& inStart, FLOAT inSize, FLOAT inDist) {
    INT result = 0;
    FVECTOR spos = inStart;
    FVECTOR* dpos;
    FVECTOR vec;
    FVECTOR nvec;
    m_collMap.setStartPos((FVECTOR&)spos);
    m_collMap.setEndPos((FVECTOR&)outPos);
    m_collMap.setType(COLL_TYPE_POINT);
    if (m_collMap.collideMap()) {
        dpos = m_collMap.getDetectPos();
        if (dpos) outPos = *dpos;
    }
    SubVectorXYZ(vec, outPos, spos);
    Normalize(nvec, vec);
    ScaleVectorXYZ(vec, nvec, inSize * 1.5f);
    SubVectorXYZ(spos, outPos, vec);
    m_collMap.setType(COLL_TYPE_SPHERE);
    m_collMap.setRadius(inSize);
    m_collMap.setShiftY(0.0f);
    m_collMap.setStartPos((FVECTOR&)spos);
    m_collMap.setEndPos((FVECTOR&)outPos);
    if (m_collMap.collideMap()) {
        dpos = m_collMap.getDetectPos();
        if (!dpos) return 0;
        outPos = *dpos;
        m_collMap.setEndPos((FVECTOR&)outPos);
        result = 1;
        if (inDist > 0.0f) {
            SubVectorXYZ(vec, m_vAim, outPos);
            vec.w = 1.0f;
            if (InnerProduct(vec, vec) < inDist * inDist) {
                m_tqNear = 1500;
                m_uiFlag |= 0x80;
            }
        }
    }
    spos.x = outPos.x;
    spos.y = outPos.y;
    spos.z = outPos.z;
    spos.y = (nvec.y < 0.0f) ? spos.y + m_pData->fSize : spos.y - m_pData->fSize;
    m_collMap.setStartPos((FVECTOR&)spos);
    m_collMap.setEndPos((FVECTOR&)outPos);
    if (m_collMap.collideMap()) {
        FVECTOR* dpos2 = m_collMap.getDetectPos();
        if (!dpos2) return 0;
        outPos = *dpos2;
        m_collMap.setEndPos((FVECTOR&)outPos);
        result = 1;
    }
    return result;
}

void CPlayerCamera::collideMapTrueCamera(const FVECTOR& inEye, const FVECTOR& inAim) {
    if (isFlag(0x20)) return;
    m_collMap.setType(COLL_TYPE_SPHERE);
    m_collMap.setStartPos((FVECTOR&)inAim);
    m_collMap.setEndPos((FVECTOR&)inEye);
    m_collMap.setRadius(m_pData->fSize * 0.1f);
    m_collMap.setShiftY(0.0f);
    m_collMap.collideMapTrueCamera(0);
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerCamera", procFieldNormal__Q212PlayerCamera13CPlayerCamera);
#if 0
/* Only the first block's scheduling differs: the original zeroes ofs (por $v0/sq) after the copy of eye, ours
 * between eye's load and store (por $v1), so the zero gets another register (as in getPlayerCameraDir). */
#line 1106
void CPlayerCamera::procFieldNormal() {
    FVECTOR pos = getNowPlayerPos();
    FVECTOR aim = m_pData->vAim;
    aim.y += m_fRikuShiftY;
    FVECTOR eye = m_pData->vEye;
    eye.y += m_fRikuShiftY;
    FLOAT base = 0.7853982f;
    BOOL bTouch = FALSE;

#line 1116
    FVECTOR ofs;
    ofs._q = 0;
    ofs.w = 1.0f;
    FVECTOR rot;
    calcDirection(rot, ofs, eye);

#line 1122
    AddVectorXYZ(m_vFinalAim, pos, aim);
    adjustCamera(m_vAim, m_vFinalAim, m_pData->fAdjust);

#line 1128
    if (!isFlag(1)) {
        FLOAT x = CPadHandler::getAnalogRX() * (1.0f / 4096.0f) * m_fReverseX;
        FLOAT y = -(CPadHandler::getAnalogRY() * (1.0f / 4096.0f)) * m_fReverseY;
        if (_fabsf(x) > 0.25f) {
            x *= getFrameRateF() * 4.0f;
            m_vDir.y = _limitPI(m_vDir.y + x * 3.1415927f / 180.0f);
#line 1138
            x = _fabsf(m_vDir.y);
            if (x > base) m_vDir.y = (m_vDir.y < 0.0f) ? -base : base;
            bTouch = TRUE;
        }
#line 1142
        if (_fabsf(y) > 0.25f) {
            y *= getFrameRateF() * 2.0f;
            m_vDir.x = _limitPI(m_vDir.x + y * 3.1415927f / 180.0f);
#line 1147
            if (m_vDir.x < rot.x - base) {
                m_vDir.x = rot.x - base;
            } else if (m_vDir.x > rot.x * 0.5f) {
                m_vDir.x = rot.x * 0.5f;
            }
            bTouch = TRUE;
        }
    }
#line 1155
    if (!bTouch) {
        m_vDir.y = Movement::Homing(m_vDir.y, m_vFinalDir.y, 0.1f, getFrameRateF());
        m_vDir.x = Movement::Homing(m_vDir.x, 0.0f, 0.1f, getFrameRateF());
    }

#line 1163
    FMATRIX mat;
    _unitMatrix(mat);
    _rotMatrixX(mat, m_vDir.x);
    _rotMatrixY(mat, m_vDir.y + rot.y);
    eye.x = 0.0f;
    eye.z *= 1.41421f;
    ApplyMatrixXYZ(eye, mat, eye);
    AddVectorXYZ(m_vFinalEye, pos, eye);

#line 1175
    adjustCamera(m_vEye, m_vFinalEye, m_pData->fAdjust);
    if (calcMapHit(m_vEye, m_vPreEye, m_pData->fSize, 0.0f)) {
        calcDirection(rot, m_vAim, m_vEye);
        if (_fabsf(rot.y - base) > base) {
            setFrameEffect(1, 1000);
            initCamera(m_iMode, 0, NULL);
        } else {
            SubVectorXYZ(ofs, m_vAim, m_vEye);
            FLOAT ret = _sqrtf(InnerProduct(ofs, ofs));
            if (ret < m_pData->fDistance) {
                setFrameEffect(1, 1000);
                initCamera(m_iMode, 0, NULL);
            }
        }
    }
}
#endif
#line 1204
void CPlayerCamera::procFieldParallel() {
    FVECTOR pos = getNowPlayerPos();
    FVECTOR aim = m_pData->vAim;
    aim.y += m_fRikuShiftY;
    FVECTOR eye = m_pData->vEye;
    eye.y += m_fRikuShiftY;
    BOOL bTurn = FALSE;

    pos.w = 1.0f;
    aim.w = 1.0f;
    eye.w = 1.0f;

#line 1217
    AddVectorXYZ(m_vFinalAim, pos, aim);
    adjustCamera(m_vAim, m_vFinalAim, m_pData->fAdjust);

#line 1222
    AddVectorXYZ(m_vFinalEye, pos, eye);
    FVECTOR aim2;
    FVECTOR eye2;

#line 1230
    FLOAT r = 2.0f / eye.x;
    ScaleVectorXYZ(aim2, eye, r);
    AddVectorXYZ(aim2, pos, aim2);
    eye2 = m_vFinalEye;
    aim2.w = 1.0f;
    eye2.w = 1.0f;
    calcMapHit(eye2, aim2, m_pData->fSize, 0.0f);

#line 1240
    m_vOffset.init();
    if (!isFlag(1)) {
        FLOAT x = CPadHandler::getAnalogRX() * (1.0f / 4096.0f) * m_fReverseX;
        FLOAT y = CPadHandler::getAnalogRY() * (1.0f / 4096.0f) * m_fReverseY;

        if (_fabsf(x) > 0.25f) {
            bTurn = TRUE;
            m_vOffset.x = x * m_pData->fDistance;
            m_vOffset.z = -m_vOffset.x;
        }
        if (_fabsf(y) > 0.25f) {
            bTurn = TRUE;
            m_vOffset.y = -y * m_pData->fDistance;
        }
    }
    if (bTurn) {
        FVECTOR start;
        FVECTOR end;
        FVECTOR ofs;
        SubVectorXYZ(start, eye2, m_vOffset);
        AddVectorXYZ(end, eye2, m_vOffset);
        calcMapHit(end, start, m_pData->fSize, 0.0f);
        SubVectorXYZ(ofs, eye2, end);
        if (_fabsf(ofs.x) > m_pData->fDistance || _fabsf(ofs.z) > m_pData->fDistance) {
            end.x = eye2.x;
            end.z = eye2.z;
        }
        if (_fabsf(ofs.y) > m_pData->fDistance) end.y = eye2.y;
        m_vFinalEye = end;
    } else {
        m_vFinalEye = eye2;
    }
    adjustCamera(m_vEye, m_vFinalEye, m_pData->fAdjust);
    SubVectorXYZ(m_vAim, m_vEye, m_vLocal);
}
#line 1299
void CPlayerCamera::changeModeBattle(INT inMode) {
    m_iMode = inMode;
    m_pData = g_pCamData[inMode];
    if (!(m_pData)) AssertApp(__FILE__, __LINE__, "m_pData", 0);

    setGakaku(m_pData->fViewAngle);
    m_vOffset.init();
    m_fRikuShiftY = (DebugWork::g_dw.player.iCamRikuShiftY && CGame::isRikuGame()) ? 0.4f : 0.0f;
    FVECTOR eye = m_pData->vEye;
    eye.y += m_fRikuShiftY;
    FVECTOR aim = m_pData->vAim;
    aim.y += m_fRikuShiftY;
    SubVectorXYZ(m_vLocal, eye, aim);
    m_fDist = _sqrtf(InnerProduct(m_vLocal, m_vLocal));
    m_collMap.setRadius(m_pData->fSize);
    m_collMap.setShiftY(0.0f);
}
#line 1325
void CPlayerCamera::procBattleNormal() {
    FVECTOR pos = getNowPlayerPos();
    FVECTOR aim = m_pData->vAim;
    aim.y += m_fRikuShiftY;
    FVECTOR eye = m_pData->vEye;
    eye.y += m_fRikuShiftY;
    BOOL bTouch;
    FMATRIX mat;

#line 1337
    FLOAT r = _limitPI(getNowPlayerDir() - 3.1415927f);
    _unitMatrix(mat);
    _rotMatrixY(mat, r);
    ApplyMatrixXYZ(m_vFinalAim, mat, aim);
    AddVectorXYZ(m_vFinalAim, pos, m_vFinalAim);

#line 1347
    adjustCamera(m_vAim, m_vFinalAim, m_pData->fAdjust);

#line 1351
    calcDirection(m_vFinalDir, m_vFinalAim, m_vEye);
    bTouch = isFlag(0x400) ? TRUE : FALSE;
    if (isFlag(0x80)) {
        m_tqNear -= getFrameRate();
        if (m_tqNear > 0) {
            bTouch = TRUE;
        } else {
            setFlag(0, 0x80);
        }
    }

#line 1361
    if (!isFlag(1)) {
        FLOAT x = CPadHandler::getAnalogRX() * (1.0f / 4096.0f) * m_fReverseX;
        FLOAT y = -(CPadHandler::getAnalogRY() * (1.0f / 4096.0f)) * m_fReverseY;
        if (_fabsf(x) > 0.25f) {
            x *= getFrameRateF() * 3.0f;
            m_vDir.y = _limitPI(m_vDir.y + x * 3.1415927f / 180.0f);
            bTouch = TRUE;
        }
#line 1376
        if (isFlag(0x80A)) {
            FLOAT d = _limitPI(m_vDir.y - r);
            if (isFlag(2)) {
                m_fYLimitHi = 1.0471976f;
            } else if (isFlag(8)) {
                m_fYLimitHi = 1.5707964f;
            } else if (isFlag(0x800)) {
                m_fYLimitHi = 0.5235988f;
            }
            if (d < -m_fYLimitHi) {
                bTouch = TRUE;
                m_vDir.y = _limitPI(r - m_fYLimitHi);
            } else if (d > m_fYLimitHi) {
                bTouch = TRUE;
                m_vDir.y = _limitPI(r + m_fYLimitHi);
            }
#line 1416
            switch (m_pPlayer->getPlayerState()) {
            case 9:
                setFlag(0, 0x1808);
                break;
            case 16:
                setFlag(0, 0x1802);
                break;
            default:
                setFlag(0, 0x180A);
                m_fYLimitHi = 3.1415927f;
                break;
            }
        }
#line 1441
        if (_fabsf(y) > 0.25f) {
            y *= getFrameRateF() * 1.5f;
            m_vDir.x = _limitPI(m_vDir.x + y * 3.1415927f / 180.0f);
            if (m_vDir.x < m_fXLimitHi) {
                m_vDir.x = m_fXLimitHi;
            } else if (m_vDir.x > m_fXLimitLow) {
                m_vDir.x = m_fXLimitLow;
            }
            bTouch = TRUE;
        }
    }
#line 1452
    if (!bTouch) {
        m_vDir.y = Movement::Homing(m_vDir.y, m_vFinalDir.y, 0.1f, getFrameRateF());
        if (!m_pPlayer->isNowIdleAnim()) {
            m_vDir.x = Movement::Homing(m_vDir.x, 0.0f, 0.02f, getFrameRateF());
        }
    }
#line 1465
    _unitMatrix(mat);
    _rotMatrixX(mat, m_vDir.x);
    _rotMatrixY(mat, m_vDir.y);
    ApplyMatrixXYZ(m_vFinalEye, mat, m_vLocal);
    AddVectorXYZ(m_vFinalEye, m_vFinalAim, m_vFinalEye);
    if (isFlag(0x1000)) {
        if (g_pPlayer) {
            m_vFinalEye = g_pPlayer->getStartPos();
            m_vFinalEye.y += 3.0f;
            m_vEye = m_vFinalEye;
        }
    } else {
        adjustCamera(m_vEye, m_vFinalEye, m_pData->fAdjust);
    }
#line 1479
    if (!bTouch) {
        if (_fabsf(m_vEye.x - m_vFinalEye.x) < 0.003f) m_vEye.x = m_vFinalEye.x;
        if (_fabsf(m_vEye.y - m_vFinalEye.y) < 0.003f) m_vEye.y = m_vFinalEye.y;
    }
#line 1489
    setFlag(0, 0x400);
    calcMapHit(m_vEye, m_vAim, m_pData->fSize, m_pData->fDistance);
    recalcNearMove();
}
#line 1504
void CPlayerCamera::procBattleLockon() {
    FVECTOR pos = getNowPlayerPos();

    FVECTOR loc = getNowTargetPos();
    FVECTOR aim = m_pData->vAim;
    aim.y += m_fRikuShiftY;
    FVECTOR eye = m_pData->vEye;
    eye.y += m_fRikuShiftY;

    FMATRIX mat;
    BOOL bTouch = FALSE;

#line 1518
    AddVectorXYZ(m_vFinalAim, loc, pos);
    ScaleVectorXYZ(m_vFinalAim, m_vFinalAim, 0.5f);
    AddVectorXYZ(m_vFinalAim, m_vFinalAim, aim);
    adjustCamera(m_vAim, m_vFinalAim, m_pData->fAdjust);

#line 1527
    FVECTOR ofs;
    SubVectorXYZ(ofs, m_vFinalAim, pos);
    FLOAT ret = _sqrtf(InnerProduct(ofs, ofs));
    ofs.set(0.0f, eye.y, eye.z + ret);
    m_fDist = ret;

    calcDirection(m_vFinalDir, m_vFinalAim, m_vEye);

#line 1539
    FVECTOR dir;
    calcDirection(dir, aim, eye);
    _unitMatrix(mat);
    _rotMatrixY(mat, m_vDir.y);
    ApplyMatrixXYZ(eye, mat, eye);
    AddVectorXYZ(eye, pos, eye);
    FVECTOR rot;
    calcDirection(rot, m_vAim, eye);
    SubVectorXYZ(rot, rot, dir);
    m_vFinalDir.x = -rot.x;

#line 1557
    if (!isFlag(1) && !m_pPlayer->isTrgFlag(0x800)) {
        FLOAT x = CPadHandler::getAnalogRX() * (1.0f / 4096.0f) * m_fReverseX;
        FLOAT y = -(CPadHandler::getAnalogRY() * (1.0f / 4096.0f)) * m_fReverseY;
        if (_fabsf(x) > 0.25f) {
            x *= getFrameRateF() * 3.0f;
            m_vDir.y = _limitPI(m_vDir.y + x * 3.1415927f / 180.0f);
            bTouch = TRUE;
        }
#line 1568
        if (_fabsf(y) > 0.25f) {
            y *= getFrameRateF() * 1.5f;
            m_vDir.x = _limitPI(m_vDir.x + y * 3.1415927f / 180.0f);
            if (m_vDir.x < m_fXLimitHi) {
                m_vDir.x = m_fXLimitHi;
            } else if (m_vDir.x > m_fXLimitLow) {
                m_vDir.x = m_fXLimitLow;
            }
            bTouch = TRUE;
        }
    }
#line 1579
    if (m_fDist * 2.0f >= m_pData->fDistance) {
        m_vFinalDir.set(-rot.x, rot.y, 0.0f);
        bTouch = FALSE;
    }
#line 1587
    if (!bTouch) {
        m_vDir.y = Movement::Homing(m_vDir.y, m_vFinalDir.y, 0.1f, getFrameRateF());
        if (!m_pPlayer->isNowIdleAnim()) {
            m_vDir.x = Movement::Homing(m_vDir.x, m_vFinalDir.x, 0.05f, getFrameRateF());
        }
    }
#line 1600
    _unitMatrix(mat);
    _rotMatrixX(mat, m_vDir.x);
    _rotMatrixY(mat, m_vDir.y);
    ApplyMatrixXYZ(m_vFinalEye, mat, ofs);
    AddVectorXYZ(m_vFinalEye, m_vFinalAim, m_vFinalEye);
    adjustCamera(m_vEye, m_vFinalEye, m_pData->fAdjust);
    if (!bTouch) {
        if (_fabsf(m_vEye.x - m_vFinalEye.x) < 0.003f) m_vEye.x = m_vFinalEye.x;
        if (_fabsf(m_vEye.y - m_vFinalEye.y) < 0.003f) m_vEye.y = m_vFinalEye.y;
    }
#line 1614
    setFlag(0, 0x400);
    _unitMatrix(mat);
    _rotMatrixY(mat, m_vDir.y);
    ApplyMatrixXYZ(aim, mat, aim);
    AddVectorXYZ(aim, pos, aim);
#line 1628
    if (aim.y < pos.y) {
        eye.x = aim.x;
        eye.y = pos.y + m_pData->fSize;
        eye.z = aim.z;
        calcMapHit(aim, eye, m_pData->fSize, 0.0f);
        if (m_vEye.y < pos.y) {
            eye.x = m_vEye.x;
            eye.y = pos.y + m_pData->fSize;
            eye.z = m_vEye.z;
            calcMapHit(m_vEye, eye, m_pData->fSize, 0.0f);
        }
    }
#line 1647
    calcMapHit2(m_vEye, aim, m_pData->fSize, 0.0f);
}

void CPlayerCamera::setFrameEffect(INT inMode, TIMEQ inTime) {
    m_tqEffect = inTime;
    CPackMan::setFrameEffect(inMode, 0, 0, 0);
    m_uiFlag |= 0x10;
}

void CPlayerCamera::exeFrameEffect() {
    if (!isFlag(0x10)) return;
    m_tqEffect -= getFrameRate();
    if (m_tqEffect <= 0) {
        m_tqEffect = 0;
        CPackMan::setFrameEffect(0, 0, 0, 0);
        setFlag(0, 0x10);
    }
}

void CPlayerCamera::changeNowPlayer(HANDLE inPlayer, HANDLE inTarget) {
    if (inPlayer > 0) {
        m_hNowPlayer = inPlayer;
        m_pNowPlayer = (CEntity*)CTaskMan::getTask(inPlayer);
        if (!m_pNowPlayer) {
            m_hNowPlayer = 0;
            m_hNowTarget = 0;
            m_pNowTarget = NULL;
            return;
        }
        if (inTarget > 0) {
            m_hNowTarget = inTarget;
            m_pNowTarget = (CEntity*)CTaskMan::getTask(inTarget);
        }
    } else {
        m_hNowPlayer = 0;
        m_pNowPlayer = NULL;
        m_hNowTarget = inTarget;
        if (inTarget > 0) {
            m_pNowTarget = (CEntity*)CTaskMan::getTask(inTarget);
        } else {
            m_pNowTarget = NULL;
        }
    }
}

void CPlayerCamera::changeCamMode(INT inMode, INT inReset) {
    if (isFlag(1)) return;
    if (inMode >= 0) {
        if (!isFlag(0x2000)) m_iPreMode = m_iMode;
        m_iMode = inMode;
        m_uiFlag |= 0x2000;
    } else {
        if (m_iMode == 5 || isFlag(0x2000)) m_iMode = m_iPreMode;
        setFlag(0, 0x2000);
    }
    if (inReset) {
        initCamera(m_iMode, 0, NULL);
    } else {
        m_hLockon = m_hNowTarget;
        changeModeBattle(m_iMode);
    }
}

void CPlayerCamera::checkNowPlayer() {
    m_pPlayer = g_pPlayer;
    if (m_hNowPlayer > 0) {
        m_pNowPlayer = (CEntity*)CTaskMan::getTask(m_hNowPlayer);
        if (!m_pNowPlayer) {
            m_hNowPlayer = 0;
            m_hNowTarget = 0;
            if (!isFlag(1)) initCamera(m_iMode, 0, NULL);
        }
    }
    if (m_hNowTarget > 0) {
        m_pNowTarget = (CEntity*)CTaskMan::getTask(m_hNowTarget);
        if (!m_pNowTarget) m_hNowTarget = 0;
    }
}

CEntity* CPlayerCamera::getNowPlayer() {
    return m_pNowPlayer ? m_pNowPlayer : (CEntity*)m_pPlayer;
}

CEntity* CPlayerCamera::getNowTarget() {
    return m_pNowTarget;
}

FVECTOR& CPlayerCamera::getNowPlayerPos() {
    if (m_hNowPlayer) {
        CEntity* pEntity = (CEntity*)CTaskMan::getTask(m_hNowPlayer);
        if (pEntity) {
            pEntity->getLockOnPos(m_vPlayer);
            return m_vPlayer;
        }
    }
    m_pPlayer->getCameraTargetPos(m_vPlayer);
    return m_vPlayer;
}

FLOAT CPlayerCamera::getNowPlayerDir() {
    FLOAT rotY;
    if (m_hNowPlayer) {
        CEntity* pEntity = (CEntity*)CTaskMan::getTask(m_hNowPlayer);
        if (pEntity && PLM::getWorldRotY(rotY, pEntity->Crd(0).getWorldMatrix())) return rotY;
    }
    return m_pPlayer->getDir();
}

FVECTOR& CPlayerCamera::getNowTargetPos() {
    if (m_hNowTarget) {
        CEntity* pEntity = (CEntity*)CTaskMan::getTask(m_hNowTarget);
        if (pEntity) {
            pEntity->getCameraTargetPos(m_vTarget);
            return m_vTarget;
        }
    }
    return m_pPlayer->getLockonPos();
}

FVECTOR& CPlayerCamera::getNowPlayerCamPos() {
    if (m_hNowPlayer) {
        CEntity* pEntity = (CEntity*)CTaskMan::getTask(m_hNowPlayer);
        if (pEntity) {
            pEntity->getCameraTargetPos(m_vTarget);
            return m_vTarget;
        }
    }
    return m_pPlayer->getLockonPos();
}

void CPlayerCamera::enableRikuShiftY() {
    if (CGame::isRikuGame()) {
        DebugWork::g_dw.player.iCamRikuShiftY = 1;
        m_fRikuShiftY = 0.4f;
    }
}

} // namespace PlayerCamera

// SOURCE DATA: .data .rodata .bss
