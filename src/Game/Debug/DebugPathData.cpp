#include "khtypes.h"
#define GAME_DEBUG_DEBUGPATHDATA_CPP
#define FORM_DEBUGWINDOW_FULL /* MATCH: CNakCamera; the original has Gprim.h's strings (PathData.h includes it first) */
#include "System/Graphic/PathData.h"
#include "System/Graphic/HermitePath.h"
#include "System/Graphic/Str.h"
#include "System/DebugProcInfo.h"
#include "Game/Debug/DebugPathData.h"
#define PATHDATACAMERA_H_PATH "System/Graphic/PathDataCamera.h" /* MATCH: the original includes it under this name */
#include "System/Graphic/PathDataCamera.h"
#include "Game/CardKind.h"
#include "Game/Player/Player.h"
#include "Form/DebugWindow.h"
#include "Form/DebugWindowTBL.h"
#include "Game/Debug/DebugWork.h"
#include "System/Graphic/WorldEnv.h"
#include "System/Coord.h"
#include "System/FileIO.h"
#include "System/TaskMan.h"
#include "include_asm.h"

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
inline void* operator new(unsigned int, void* p) throw() { return p; }
extern "C" char* strcpy(char*, const char*);
class CRsrcDB { /* PARTIAL: stand-in for System/RsrcDB CRsrcDB: strings: RsrcDB.h brings "System/RsrcData.h(156)", not in the original (queue: RsrcDB partition) */
public:
    static CHAR* getRsrcPTDName(INT nPtdNo);
};
#line 19
class CHermitePathMoveS : public CTask {
    CPathVector* m_pPath;        /* 0x40 */
    CHermitePath m_HPath;        /* 0x44 */
    FVECTOR m_vRot;              /* 0x80 */
    CDbgArrow m_DDPathCamArrow;  /* 0x90 */
public:
    CHermitePathMoveS(CTask* p, const CPathVector* pVec);
    virtual ~CHermitePathMoveS();
    virtual void main();
    virtual void disp();
    BOOL isEnd() { return m_HPath.isEnd(); }
    CHermitePath* getHermitePath() { return &m_HPath; }
};
#line 27
extern "C" char D_004870B0[];
extern "C" char D_00487120[];
extern "C" char D_00487158[];
extern "C" char D_00487168[];
extern "C" char D_00486F18[];

/* PARTIAL: .rodata still asm - the tables' strings by label */
extern "C" char D_00486138[];
extern "C" char D_00486280[];
extern "C" char D_00486298[];
extern "C" char D_004862F0[];
extern "C" char D_00486340[];
extern "C" char D_00486380[];
extern "C" char D_004863C0[];
extern "C" char D_00486410[];
extern "C" char D_00486460[];
extern "C" char D_004864B8[];
extern "C" char D_00486500[];
extern "C" char D_00486558[];
extern "C" char D_004865B0[];
extern "C" char D_00486608[];
extern "C" char D_00486648[];
extern "C" char D_00486688[];
extern "C" char D_004866D8[];
extern "C" char D_00486710[];
extern "C" char D_00486738[];
extern "C" char D_00486760[];
extern "C" char D_00486788[];
extern "C" char D_004867A0[];
extern "C" char D_004867C8[];
extern "C" char D_004867F8[];
extern "C" char D_00486820[];
extern "C" char D_00486848[];
extern "C" char D_00486858[];
extern "C" char D_00486880[];
extern "C" char D_004868A8[];
extern "C" char D_004868B8[];
extern "C" char D_004868E0[];
extern "C" char D_004868F0[];
extern "C" char D_00486918[];
extern "C" char D_00486940[];
extern "C" char D_00486950[];
extern "C" char D_00486978[];
extern "C" char D_004869A0[];
extern "C" char D_004869C8[];
extern "C" char D_004869F0[];
extern "C" char D_00486A18[];
extern "C" char D_00486A40[];
extern "C" char D_00486A68[];
extern "C" char D_00486A98[];
extern "C" char D_00486AA8[];
extern "C" char D_00486AB8[];
extern "C" char D_00486AC8[];
extern "C" char D_00486AD8[];
extern "C" char D_00486AE8[];
extern "C" char D_00486C78[];
extern "C" char D_00486C90[];
extern "C" char D_00486CD8[];
extern "C" char D_00486D20[];
extern "C" char D_00486D30[];
extern "C" char D_00486D40[];
extern "C" char D_00486D58[];
extern "C" char D_00486D70[];
extern "C" char D_00486D88[];
extern "C" char D_00486D98[];
extern "C" char D_00486DA8[];
extern "C" char D_00486DC8[];
extern "C" char D_00486DE0[];
extern "C" char D_00486DF8[];
extern "C" char D_00486E18[];
extern "C" char D_00486E38[];
extern "C" char D_00486E50[];
extern "C" char D_00486E68[];
extern "C" char D_00486E80[];
extern "C" char D_00486EA0[];
extern "C" char D_00486EB0[];
extern "C" char D_00486ED0[];
extern "C" char D_00486EE0[];
extern "C" char D_00486EF0[];
extern "C" char D_00486F08[];
extern "C" char D_00486F20[];
extern "C" char D_00486F58[];
extern "C" char D_00486FA8[];
extern "C" char D_00486FD0[];
extern "C" char D_00486FE8[];
extern "C" char D_00486FF0[];
BOOL debugWinPathEditAdd(CDebugWindow* pObj, INT inY, INT inParam);
INT debugWinPathEditAllCall(CDebugWindow* pWin, INT iMes);
BOOL debugWinPathEditDel(CDebugWindow* pObj, INT inY, INT inParam);
INT debugWinPathEditDispCall(char* pBuf, INT iIdx, const void* pParam);
INT debugWinPathEditEtcParamAllCall(CDebugWindow* pWin, INT iMes);
BOOL debugWinPathEditLineAddPoaCall(CDebugWindow* pObj, INT inY, INT inParam);
BOOL debugWinPathEditLineAddRotCall(CDebugWindow* pObj, INT inY, INT inParam);
BOOL debugWinPathEditLineAddSpdCall(CDebugWindow* pObj, INT inY, INT inParam);
BOOL debugWinPathEditLineCamPosCall(CDebugWindow* pObj, INT inY, INT inParam);
BOOL debugWinPathEditNum(CDebugWindow* pObj, INT inY, INT inParam);
BOOL debugWinPathEditPathAddLineCall(CDebugWindow* pObj, INT inY, INT inParam);
BOOL debugWinPathEditPathDel(CDebugWindow* pObj, INT inY, INT inParam);
BOOL debugWinPathEditPathNum(CDebugWindow* pObj, INT inY, INT inParam);
INT debugWinPathEditPosCamAllCall(CDebugWindow* pWin, INT iMes);
BOOL debugWinPathEditPosCamGround(CDebugWindow* pObj, INT inY, INT inParam);
BOOL debugWinPathEditPosCamReset(CDebugWindow* pObj, INT inY, INT inParam);
BOOL debugWinPathEditSimulateLineCall(CDebugWindow* pObj, INT inY, INT inParam);
BOOL debugWinPathEditSimulateSpeedCamLineCall(CDebugWindow* pObj, INT inY, INT inParam);
BOOL debugWinPathEditSimulateSpeedCamLineLookCall(CDebugWindow* pObj, INT inY, INT inParam);
BOOL debugWinPathEditSimulateSpeedLineCall(CDebugWindow* pObj, INT inY, INT inParam);
BOOL debugWinPathEndOneCall(CDebugWindow* pObj, INT inY, INT inParam);
BOOL debugWinPathEndOneYNCall(CDebugWindow* pObj, INT inY, INT inParam);
BOOL debugWinPathLineAddSubCall(CDebugWindow* pObj, INT inY, INT inParam);
BOOL debugWinPathLineNameLoadCall(CDebugWindow* pObj, INT inY, INT inParam);
BOOL debugWinPathLineNameSaveCall(CDebugWindow* pObj, INT inY, INT inParam);
BOOL debugWinPathLineNewCall(CDebugWindow* pObj, INT inY, INT inParam);
BOOL debugWinPathLineSaveCall(CDebugWindow* pObj, INT inY, INT inParam);
INT debugWinPathListAllCall(CDebugWindow* pWin, INT iMes);
INT debugWinPathListDispCall(char* pBuf, INT iIdx, const void* pParam);
BOOL debugWinPathListLineCall(CDebugWindow* pObj, INT inY, INT inParam);
extern DEBUG_WINDOW_TBL debugWinPathEditSimulate;
extern DEBUG_WINDOW_TBL debugWinPathEditEtcParam;
extern DEBUG_WINDOW_TBL debugWinPathEditPosCam;
extern DEBUG_WINDOW_TBL debugWinPathEditPathAdd;
extern DEBUG_WINDOW_TBL debugWinPathSave;
extern DEBUG_WINDOW_TBL debugWinPathEdit;
extern DEBUG_WINDOW_TBL debugWinPathList;
extern DEBUG_WINDOW_TBL debugWinPath;
extern DEBUG_WINDOW_TBL debugWinPathNoEnd;

CDebugPathData* CDebugPathData::m_pInstance = NULL;
INT CDebugPathData::m_iActivePath = 0;
INT CDebugPathData::m_iActivePathNum = 0;
FLOAT CDebugPathData::m_fValuePlus = 1.0f;
CPDFrameTime* CDebugPathData::m_pFrame = NULL;
UINT CDebugPathData::m_iCtrlFlg = 2;

/* Parked: s2/s3 swapped (pVec vs &m_DDPathCamArrow) and the original recomputes &m_DDPathCamArrow and
   &m_vRot for setCoord (ours reuses them); tried: statement order (m_pPath first), a pRot temporary - only
   3 variants, retry at the unit's end; tools/permuter.py does not find constructor definitions */
#if 0
CHermitePathMoveS::CHermitePathMoveS(CTask* p, const CPathVector* pVec) : CTask(p, 0x4000) {
    ignorePause(TRUE);
    m_pPath = (CPathVector*)pVec;
    m_HPath.makePathFromPathVector(pVec);
    addCoord(1);
    asm volatile("vmr32.xyzw $vf1, $vf0\n"
                 "sqc2 $vf0, 0x30(%0)\n"
                 "vmr32.xyzw $vf2, $vf1\n"
                 "sqc2 $vf1, 0x20(%0)\n"
                 "vmr32.xyzw $vf3, $vf2\n"
                 "sqc2 $vf2, 0x10(%0)\n"
                 "sqc2 $vf3, 0x0(%0)\n"
                 :
                 : "r"(&Crd(0).m_Local)
                 : "memory");
    Crd(0).m_Flag = 1;
    m_DDPathCamArrow.getCoord().setParent(&Crd(0));
    asm volatile("sqc2 $vf0, 0x0(%0)" : : "r"(&m_vRot) : "memory");
    m_vRot.w = 5.0f;
    m_DDPathCamArrow.setCoord(NULL, &m_vRot);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", __17CHermitePathMoveSP5CTaskPC11CPathVector);
#endif
CHermitePathMoveS::~CHermitePathMoveS() {}
void CHermitePathMoveS::main() {
    PATH_PARAM sParam;
    FVECTOR vPos;
    FLOAT fRate = getFrameRateF();
    m_pPath->getPathParam(sParam);
    if (sParam.iFlags & 0x4000000) {
        INT iBlock1 = m_HPath.getPathBlock();
        INT iBlock2 = iBlock1 + 1;
        FLOAT fVSpeed0 = m_pPath->getPathSpeed(iBlock1);
        FLOAT fVSpeed1 = m_pPath->getPathSpeed(iBlock2);
        FLOAT fSpeed = (fVSpeed1 - fVSpeed0) * m_HPath.getHermiteCoefficient(iBlock1, m_HPath.getProgress()) + fVSpeed0;
        m_HPath.setSpeed(fSpeed * 0.0046296297f);
    }
    m_HPath.update(fRate);
    m_HPath.getNowPos(vPos);
    m_HPath.getNowRot(m_vRot, 1.0f);
    m_vRot.z = m_HPath.getNowRoll();
    m_vRot.w = 5.0f;
    m_DDPathCamArrow.setCoord(NULL, &m_vRot);
    Crd(0).m_Local.v[3] = vPos;
    Crd(0).m_Flag = 1;
}
void CHermitePathMoveS::disp() { m_DDPathCamArrow.disp(); }
INT debugWinPathEndOneCall(CDebugWindow* pWin, INT iMes, INT iParam) {
    if (iParam & 0x20) pWin->sendMessage(2, 0);
    return 1;
}
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4083: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEndOneYNCall__FP12CDebugWindowii);
INT debugWinPathLineAddSubCall(CDebugWindow* pWin, INT iMes, INT iParam) {
    if (iParam & 0x80) pWin->addValue(-1, NULL, NULL, &CDebugPathData::m_fValuePlus, NULL);
    else if (iParam & 0x100) pWin->addValue(1, NULL, NULL, &CDebugPathData::m_fValuePlus, NULL);
    return 1;
}
void __simuCamPause(CPathDataCamera* pHPC) {
    if (dbgButtonTrg(USR_SYSTEM, 0x80, 1)) {
        if (pHPC->isStop()) {
            pHPC->run();
            CDebugPathData::m_pFrame->start();
        } else {
            pHPC->stop();
            CDebugPathData::m_pFrame->stop();
        }
    }
}
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4093: .word 0\n""pHPC.4094: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditSimulateLineCall__FP12CDebugWindowii);
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4098: .word 0\n""pHPC.4099: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditSimulateSpeedLineCall__FP12CDebugWindowii);
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4103: .word 0\n""pHPC.4104: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditSimulateSpeedCamLineCall__FP12CDebugWindowii);
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4108: .word 0\n""pHPC.4109: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditSimulateSpeedCamLineLookCall__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditEtcParamAllCall__FP12CDebugWindowi);
__asm__(".section .data"); /* PARTIAL: gcc doesn't know the asm functions above switched to .text */
DEBUG_WINDOW_LINE debugWinPathEditSimulateLine[17] = {
    {D_00486280, &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditSimulateSpeedLineCall, NULL},
    {D_00486298, &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {D_004862F0, &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditSimulateSpeedCamLineCall, NULL},
    {D_00486340, &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditSimulateSpeedCamLineLookCall, NULL},
    {D_00486380, &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditSimulateLineCall, NULL},
    {D_004863C0, &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditSimulateSpeedCamLineCall, NULL},
    {D_00486410, &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditSimulateSpeedCamLineCall, NULL},
    {D_00486460, &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditSimulateSpeedCamLineCall, NULL},
    {D_004864B8, &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditSimulateSpeedCamLineLookCall, NULL},
    {D_00486500, &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditSimulateSpeedCamLineCall, NULL},
    {D_00486558, &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditSimulateSpeedCamLineCall, NULL},
    {D_004865B0, &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {D_00486608, &CDebugPathData::m_iCtrlFlg, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x2, NULL, NULL},
    {D_00486648, &CDebugPathData::m_iCtrlFlg, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x4, NULL, NULL},
    {D_004865B0, &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {D_00486688, &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinPathEditSimulate = {D_004866D8, -240, -144, 40, 16, 12, 0, 2, debugWinPathEditSimulateLine, NULL, NULL};

DEBUG_WINDOW_LINE debugWinPathEditEtcParamLine[28] = {
    {D_00486710, &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {D_00486738, &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x4000000, NULL, NULL},
    {D_00486760, &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x1, NULL, NULL},
    {D_00486788, &CDebugPathData::m_sParam.fTime, 4, {0}, 1, 0.01f, 1e+04f, 0.0f, NULL, 0, &debugWinPathLineAddSubCall, NULL},
    {D_004867A0, &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x2, NULL, NULL},
    {D_004867C8, &CDebugPathData::m_sParam.fSpeed, 4, {0}, 1, 0.01f, 1e+04f, 0.0f, NULL, 0, &debugWinPathLineAddSubCall, NULL},
    {D_004867F8, &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {D_00486820, &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x4, NULL, NULL},
    {D_00486848, &CDebugPathData::m_sParam.fTargetDistance, 4, {0}, 1, 0.01f, 1e+04f, 0.0f, NULL, 0, &debugWinPathLineAddSubCall, NULL},
    {D_00486858, &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x8, NULL, NULL},
    {D_00486848, &CDebugPathData::m_sParam.fTargetOffsetY, 4, {0}, 1, -1e+04f, 1e+04f, 0.0f, NULL, 0, &debugWinPathLineAddSubCall, NULL},
    {D_00486880, &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x40, NULL, NULL},
    {D_004868A8, &CDebugPathData::m_sParam.fLookDelay, 4, {0}, 1, 0.01f, 1.0f, 0.0f, NULL, 0, &debugWinPathLineAddSubCall, NULL},
    {D_004868B8, &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x20, NULL, NULL},
    {D_004868E0, &CDebugPathData::m_sParam.fProj, 4, {0}, 1, 1.0f, 1.8e+02f, 0.0f, NULL, 0, &debugWinPathLineAddSubCall, NULL},
    {D_004868F0, &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x200, NULL, NULL},
    {D_004868E0, &CDebugPathData::m_sParam.fProj2, 4, {0}, 1, 1.0f, 1.8e+02f, 0.0f, NULL, 0, &debugWinPathLineAddSubCall, NULL},
    {D_00486918, &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x80, NULL, NULL},
    {D_00486940, &CDebugPathData::m_sParam.fLearpIn, 4, {0}, 1, 1.0f, 999.0f, 0.0f, NULL, 0, &debugWinPathLineAddSubCall, NULL},
    {D_00486950, &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x100, NULL, NULL},
    {D_00486940, &CDebugPathData::m_sParam.fLearpOut, 4, {0}, 1, 1.0f, 999.0f, 0.0f, NULL, 0, &debugWinPathLineAddSubCall, NULL},
    {D_00486978, &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x20000000, NULL, NULL},
    {D_004869A0, &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {D_004869C8, &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x40000000, NULL, NULL},
    {D_004869F0, &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x80000000, NULL, NULL},
    {D_00486A18, &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {D_00486A40, &CDebugPathData::m_fValuePlus, 4, {0}, 17, 0.01f, 1e+03f, 1e+01f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinPathEditEtcParam = {D_00486A68, -136, -176, 18, 26, 12, 0, 2, debugWinPathEditEtcParamLine, NULL, &debugWinPathEditEtcParamAllCall};
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4116: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditPosCamGround__FP12CDebugWindowii);
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4123: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditPosCamReset__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditPosCamAllCall__FP12CDebugWindowi);
__asm__(".section .data"); /* PARTIAL: gcc doesn't know the asm functions above switched to .text */
DEBUG_WINDOW_LINE debugWinPathEditPosCamLine[8] = {
    {D_00486A98, &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEndOneCall, NULL},
    {D_00486AA8, &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {D_00486AB8, &CDebugWindow::none, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOnOff, 0, &debugWinPathEditPosCamGround, NULL},
    {D_00486AC8, &CDebugWindow::none, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOnOff, 0, &debugWinPathEditPosCamReset, NULL},
    {D_00486AA8, &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {D_00486AD8, (char*)&DebugWork::g_dw + 0x18D, 1, {0}, 0, 0.0f, 3.0f, 1.0f, DWT::szCamSpeed, 0, NULL, NULL},
    {D_00486AE8, (char*)&DebugWork::g_dw + 0x18C, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOnOff, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinPathEditPosCam = {CDebugPathData::m_szPathActName, -248, -176, 7, 8, 12, 0, 6, debugWinPathEditPosCamLine, NULL, &debugWinPathEditPosCamAllCall};
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4130: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditLineCamPosCall__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditLineAddPoaCall__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditLineAddRotCall__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditLineAddSpdCall__FP12CDebugWindowii);
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4143: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathLineNameSaveCall__FP12CDebugWindowii);
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4147: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathLineSaveCall__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditNum__FP12CDebugWindowii);
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4154: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditAdd__FP12CDebugWindowii);
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4158: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditDel__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditPathNum__FP12CDebugWindowii);
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4165: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditPathAddLineCall__FP12CDebugWindowii);
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4169: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditPathDel__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditDispCall__FPciPCv);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditAllCall__FP12CDebugWindowi);
__asm__(".section .data"); /* PARTIAL: gcc doesn't know the asm functions above switched to .text */
DEBUG_WINDOW_LINE debugWinPathEditPathAddLine[5] = {
    {D_00486C78, &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditPathAddLineCall, NULL},
    {D_00486C90, &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditPathAddLineCall, NULL},
    {D_00486CD8, &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditPathAddLineCall, NULL},
    {D_00486D20, &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditPathAddLineCall, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinPathEditPathAdd = {D_00486D30, -216, -144, 33, 8, 12, 0, 2, debugWinPathEditPathAddLine, NULL, NULL};

DEBUG_WINDOW_LINE debugWinPathSaveLine[3] = {
    {D_00486D40, &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathLineSaveCall, NULL},
    {D_00486D58, &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathLineNameSaveCall, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinPathSave = {D_00486D70, -216, -144, 10, 2, 14, 0, 2, debugWinPathSaveLine, NULL, NULL};

DEBUG_WINDOW_LINE debugWinPathEditLine[23] = {
    {D_00486138, &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditPathNum, NULL},
    {D_00486D88, &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinPathEditPathAdd},
    {D_00486D98, &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditPathDel, NULL},
    {D_00486DA8, &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinPathEditEtcParam},
    {D_00486DC8, &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinPathEditSimulate},
    {D_00486138, &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditNum, NULL},
    {D_00486DE0, &CDebugPathData::m_iActivePathNum, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditAdd, NULL},
    {D_00486DF8, &CDebugPathData::m_iActivePathNum, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditLineCamPosCall, NULL},
    {D_00486E18, &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {D_00486E38, &CDebugPathData::m_vActivePathPos.x, 4, {0}, 1, -1e+05f, 1e+05f, 0.0f, NULL, 0, &debugWinPathEditLineAddPoaCall, NULL},
    {D_00486E50, &CDebugPathData::m_vActivePathPos.y, 4, {0}, 1, -1e+05f, 1e+05f, 0.0f, NULL, 0, &debugWinPathEditLineAddPoaCall, NULL},
    {D_00486E68, &CDebugPathData::m_vActivePathPos.z, 4, {0}, 1, -1e+05f, 1e+05f, 0.0f, NULL, 0, &debugWinPathEditLineAddPoaCall, NULL},
    {D_00486138, &CDebugPathData::m_vActivePathPos.w, 4, {0}, 1, -3.1415927f, 3.1415927f, 0.0f, NULL, 0, &debugWinPathEditLineAddRotCall, NULL},
    {D_00486138, &CDebugPathData::m_vActivePathPos.w, 4, {0}, 1, -3.1415927f, 3.1415927f, 0.0f, NULL, 0, &debugWinPathEditLineAddSpdCall, NULL},
    {D_00486E80, &CDebugPathData::m_fValuePlus, 4, {0}, 17, 0.01f, 1e+03f, 1e+01f, NULL, 0, NULL, NULL},
    {D_00486EA0, &CDebugPathData::m_iActivePathNum, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditDel, NULL},
    {D_00486EB0, &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {D_00486ED0, (char*)&DebugWork::g_dw + 0x18D, 1, {0}, 0, 0.0f, 3.0f, 1.0f, DWT::szCamSpeed, 0, NULL, NULL},
    {D_00486EE0, (char*)&DebugWork::g_dw + 0x18C, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOnOff, 0, NULL, NULL},
    {D_00486EB0, &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {D_00486EF0, &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinPathSave},
    {D_00486F08, &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEndOneYNCall, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinPathEdit = {CDebugPathData::m_szPathName, -248, -176, 13, 22, 12, 0, 6, debugWinPathEditLine, &debugWinPathEditDispCall, &debugWinPathEditAllCall};
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4179: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathListLineCall__FP12CDebugWindowii);
INT debugWinPathListDispCall(char* pBuf, INT iIdx, const void* pParam) {
    sprintf(pBuf, D_00486F18, CDebugPathData::m_szNames[iIdx]);
    return 1;
}
INT debugWinPathListAllCall(CDebugWindow* pWin, INT iMes) {
    if (iMes & 1) {
        INT i;
        for (i = 0; i < 128; i++) {
            CHAR* pName = CRsrcDB::getRsrcPTDName(i + 1);
            if (!pName) break;
            strcpy(CDebugPathData::m_szNames[i], pName);
        }
        return i;
    }
    return 1;
}
__asm__(".section .data"); /* PARTIAL: gcc doesn't know the asm functions above switched to .text */
DEBUG_WINDOW_LINE debugWinPathListLine[2] = {
    {D_00486138, &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathListLineCall, &DWT::debugWinYesNo},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinPathList = {D_00486F20, -208, -136, 7, 20, 14, 0, 8, debugWinPathListLine, &debugWinPathListDispCall, &debugWinPathListAllCall};
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4189: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathLineNewCall__FP12CDebugWindowii);
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4193: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathLineNameLoadCall__FP12CDebugWindowii);
__asm__(".section .data"); /* PARTIAL: gcc doesn't know the asm functions above switched to .text */
DEBUG_WINDOW_LINE debugWinPathLine[5] = {
    {D_00486F58, &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinPathList},
    {D_00486FA8, &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathLineNameLoadCall, NULL},
    {D_00486FD0, &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathLineNewCall, NULL},
    {D_00486FE8, &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEndOneCall, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinPath = {D_00486FF0, -240, -168, 34, 20, 12, 0, 20, debugWinPathLine, NULL, NULL};

DEBUG_WINDOW_LINE debugWinPathNoEndLine[4] = {
    {D_00486F58, &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinPathList},
    {D_00486FA8, &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathLineNameLoadCall, NULL},
    {D_00486FD0, &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathLineNewCall, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinPathNoEnd = {D_00486FF0, -240, -168, 34, 20, 12, 0, 22, debugWinPathNoEndLine, NULL, NULL};
extern "C" char D_00487008[];
extern "C" char D_00487040[];
extern "C" char D_00487078[];
/* Data step first: the inline CCPrim::create brings Gprim.h's "(613)"/"m_V" literals, and the unit's .rodata is
   still asm (objcopy: symbol .rodata required). Draft not compared yet. */
#if 0
CDebugPathData::CDebugPathData(CTask* p, BOOL bEnd) : CTask(p, 0x4000) {
    CTask* pWnd;
    m_pPath = NULL;
    ignorePause(TRUE);
    m_szFileName[0] = 0;
    if (bEnd) {
        pWnd = new (com_malloc_d(0x1C0, D_00487008, RET_ADDR())) CDebugWindow(this, debugWinPath);
    } else {
        pWnd = new (com_malloc_d(0x1C0, D_00487040, RET_ADDR())) CDebugWindow(this, debugWinPathNoEnd);
    }
    m_hChildWnd = pWnd ? pWnd->getHandle() : 0;
    m_iActivePathNum = 0;
    m_pFrame = new (com_malloc_d(0xD8, D_00487078, RET_ADDR())) CPDFrameTime;
    m_gpPArrowPoly.create(0xB, 3);
    m_gpPArrowPoly.setLayerFG(2);
    CDebugWindow::enableTopDebugWindow(FALSE);
    CPlayer::enableDbgPadControl(FALSE);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", __14CDebugPathDataP5CTaski);
#endif
/* Not attempted yet: the key function; defining it emits every in-class inline (the weak tail, still asm).
   Draft (needs Game/CardKind.h + Game/Player/Player.h after PathDataCamera.h, ~CPDFrameTime() {} inline):
   delete m_pPath; m_pPath = NULL; delete m_pFrame; m_pFrame = NULL; m_pInstance = NULL;
   CDebugWindow::enableTopDebugWindow(TRUE); CPlayer::enableDbgPadControl(TRUE); */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", _$_14CDebugPathData);
BOOL CDebugPathData::create(CTask* p, BOOL bEnd) {
    if (!m_pInstance) {
        m_pInstance = new (com_malloc_d(sizeof(CDebugPathData), D_004870B0, RET_ADDR())) CDebugPathData(p, bEnd);
        return TRUE;
    }
    return FALSE;
}
void CDebugPathData::main() {
    if (isEnd()) out();
}
extern "C" void sceVu0RotMatrixZ(FMATRIX& m0, FMATRIX& m1, float rz);
struct _ArrowVecs { FVECTOR v[3]; };
extern "C" const _ArrowVecs D_004870F0; /* {0,-64,0,1}, {16,0,0,1}, {-16,0,0,1} (PARTIAL: .rodata still asm) */
void CDebugPathData::disp() {
    m_pFrame->disp(getFrameRateF());
    if (m_pDCam == CWorldEnv::getActiveCamera()) {
        if (m_pPath) m_pPath->dispDebugLine();
        if (m_iCtrlFlg & 1) {
            FMATRIX mMat;
            _ArrowVecs v = D_004870F0;
            asm volatile("vmr32.xyzw $vf1, $vf0\n"
                         "sqc2 $vf0, 0x30(%0)\n"
                         "vmr32.xyzw $vf2, $vf1\n"
                         "sqc2 $vf1, 0x20(%0)\n"
                         "vmr32.xyzw $vf3, $vf2\n"
                         "sqc2 $vf2, 0x10(%0)\n"
                         "sqc2 $vf3, 0x0(%0)\n"
                         :
                         : "r"(&mMat)
                         : "memory");
            sceVu0RotMatrixZ(mMat, mMat, _getPathVectorConvWRoll(m_vActivePathPos.w));
            ApplyMatrix(*(FVECTOR*)&m_gpPArrowPoly.m_V[0], mMat, v.v[0]);
            ApplyMatrix(*(FVECTOR*)&m_gpPArrowPoly.m_V[1], mMat, v.v[1]);
            ApplyMatrix(*(FVECTOR*)&m_gpPArrowPoly.m_V[2], mMat, v.v[2]);
            m_gpPArrowPoly.m_V[0].C.col = 0x800000FF;
            m_gpPArrowPoly.m_V[1].C.col = 0x80C0C0C0;
            m_gpPArrowPoly.m_V[2].C.col = 0x80C0C0C0;
            m_gpPArrowPoly.disp();
        }
    }
}
BOOL CDebugPathData::isEnd() {
    if (m_hChildWnd) {
        CTask* pTask = CTaskMan::getTask(m_hChildWnd);
        if (!pTask || pTask->isOut()) {
            m_hChildWnd = 0;
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}
void CDebugPathData::entryCam() {
    CDebugPathData* p = getInstance();
    FVECTOR vPos;
    asm volatile("sqc2 $vf0, 0x0(%0)" : : "r"(&vPos) : "memory");
    p->m_pDCam = new (com_malloc_d(0x140, D_00487120, RET_ADDR())) CNakCamera(p, 1, 0x100, &vPos, 0.0f, (void**)&p->m_pDCam);
}
CNakCamera* CDebugPathData::getCam() { return getInstance()->m_pDCam; }
void CDebugPathData::removeCam() {
    CDebugPathData* pThis = getInstance();
    ((CTask*)pThis->m_pDCam)->out();
    pThis->m_pDCam = NULL;
}
void CDebugPathData::makeFileName(const CHAR* pszName, BOOL bExt) {
    CDebugPathData* pThis = getInstance();
    if (bExt) sprintf(pThis->m_szFileName, D_00487158, pszName);
    else sprintf(pThis->m_szFileName, D_00487168, pszName);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", newPath__14CDebugPathDataPCc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", entryPath__14CDebugPathDataPCc);
extern "C" char D_004871B0[]; /* "c:/KHCOM/MainApp/Game/Debug/DebugPathData.cpp" */
extern "C" char D_004871E0[]; /* "p->m_pPath" */
extern "C" char D_00487228[]; /* "c:/KHCOM/MainApp/Game/Debug/DebugPathData.cpp(2132)" */
extern "C" char D_00487260[]; /* "pBuf" */
extern "C" char D_00487268[]; /* "c:/KHCOM/MainApp/Game/Debug/DebugPathData.cpp(2141)" */
/* Weak tail first: this draft matches (V1), but `new CPathData` makes gcc emit the weak CVector<G_STRIP>
   destructor (the original's unnamed func_0021F178, which follows savePath) at the end of the unit. Needs
   CPathData(const PATH_DATA* pPathData, BOOL bAlloc) { init(pPathData, bAlloc); } in PathData.h. */
#if 0
BOOL CDebugPathData::loadPath(const CHAR* pName) {
    CDebugPathData* p = getInstance();
    INT iSize;
    INT iFD;
    void* pBuf;
    makeFileName(pName, TRUE);
    iSize = CFileIO::getFileSize(p->m_szFileName);
    if (iSize <= 0) return FALSE;
    iFD = CFileIO::openFile(p->m_szFileName, 1, 0x1FF);
    if (iFD < 0) return FALSE;
    pBuf = com_malloc_d(iSize, D_00487228, (UINT)-1);
    if (!pBuf) AssertApp(D_004871B0, 2133, D_00487260, 0);
    if (CFileIO::readFile(iFD, pBuf, iSize) < 0) {
        if (pBuf) com_free(pBuf);
        return FALSE;
    }
    delete p->m_pPath;
    p->m_pPath = NULL;
    p->m_pPath = new (com_malloc_d(0x5C, D_00487268, RET_ADDR())) CPathData((const PATH_DATA*)pBuf, TRUE);
    if (!p->m_pPath) AssertApp(D_004871B0, 2142, D_004871E0, 0);
    if (pBuf) com_free(pBuf);
    CFileIO::closeFile(iFD);
    return TRUE;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", loadPath__14CDebugPathDataPCc);
#endif
extern "C" void* memset(void*, int, unsigned int);
extern "C" char* strncpy(char*, const char*, unsigned int);
extern "C" int FlushCache(int op);
extern "C" char D_004872A0[]; /* "c:/KHCOM/MainApp/Game/Debug/DebugPathData.cpp(2163)" */
extern "C" char D_004872D8[];
extern "C" char D_00487318[];
extern "C" char D_00487358[]; /* ".BAK" */
extern "C" char D_00487360[];
extern "C" char D_00487398[];
BOOL __BackUpFile(const CHAR* szFileName) {
    void* pBuf = NULL;
    INT iRet;
    INT iSize = CFileIO::getFileSize(szFileName);
    INT iFD;
    INT i;
    char szOutFileName[64];
    INT iOutFD;
    if (iSize > 0) {
        iFD = CFileIO::openFile(szFileName, 1, 0x1FF);
        if (iFD >= 0) {
            pBuf = com_malloc_d(iSize, D_004872A0, (UINT)-1);
            if (pBuf) {
                iRet = CFileIO::readFile(iFD, pBuf, iSize);
                if (iRet < 0) {
                    TRACE(D_004872D8, iRet);
                    com_free(pBuf);
                    pBuf = NULL;
                }
            } else {
                TRACE(D_00487318);
            }
            CFileIO::closeFile(iFD);
        }
    }
    if (pBuf) {
        FlushCache(0);
        memset(szOutFileName, 0, sizeof(szOutFileName));
        strncpy(szOutFileName, szFileName, sizeof(szOutFileName) - 1);
        for (i = sizeof(szOutFileName) - 1; i >= 0; i--) {
            if (szOutFileName[i] == '.') break;
        }
        if (i >= 0) {
            sprintf(&szOutFileName[i], D_00487358);
            iOutFD = CFileIO::openFile(szOutFileName, 0x602, 0x1FF);
            if (iOutFD < 0) {
                TRACE(D_00487360, iOutFD);
            } else {
                iRet = CFileIO::writeFile(iOutFD, pBuf, iSize);
                if (iRet < 0) TRACE(D_00487398, iRet);
                CFileIO::closeFile(iOutFD);
            }
        }
        if (pBuf) com_free(pBuf);
    }
    return TRUE;
}
extern "C" char D_004873D0[];
extern "C" char D_004873F0[];
extern "C" char D_00487420[];
extern "C" char D_00487440[];
extern "C" char D_00487458[];
extern "C" char D_00487480[];
extern "C" char D_004874A0[];
extern "C" char D_004874C0[];
extern "C" char D_004874D8[];
extern "C" char D_004874F0[];
extern "C" char D_00487510[];
extern "C" char D_00487530[];
extern "C" char D_00487550[];
extern "C" char D_00487570[];
extern "C" char D_00487590[];
extern "C" char D_004875B0[];
extern "C" char D_004875D0[];
/* Parked: 5 lines off: the original hoists &sPathParam.fTime (the start of the parameter pointer) out of the
   paths loop into a stack slot, like &sVecParam and &vPath; ours computes it before the parameter loop.
   Worked: one j per inner loop, the 0xFFFFFFF0 mask, the _zeroVec helper (45 -> 5 lines). Tried for the
   pointer: init before iCount / at its declaration / (INT*)&s + 1 / &((INT*)&s)[1] / base + j inside the
   loop / indexed writeFile argument; permuter 15 min (42581 candidates, no gain). Needs
   CPathData::makeVertexSpeedAll (PathData.h). */
#if 0
static inline void _zeroVec(FVECTOR& v) { asm volatile("sqc2 $vf0, 0x0(%0)" : : "r"(&v) : "memory"); }
BOOL CDebugPathData::savePath(const CHAR* pName) {
    CDebugPathData* p = getInstance();
    INT iFD;
    INT iRet;
    INT iSize;
    PATH_DATA_HEAD sHead;
    if (pName) makeFileName(pName, TRUE);
    __BackUpFile(p->m_szFileName);
    iFD = CFileIO::openFile(p->m_szFileName, 0x602, 0x1FF);
    if (iFD < 0) return FALSE;
    p->m_pPath->makeVertexSpeedAll();
    memset(&sHead, 0, sizeof(sHead));
    sHead.iVer = 2;
    sHead.iId = 0x445450;
    sHead.iPathNum = p->m_pPath->getPathNum();
    TRACE(D_004873D0);
    iRet = CFileIO::writeFile(iFD, &sHead, sizeof(sHead));
    if (iRet < 0) {
        TRACE(D_004873F0, iRet);
        return FALSE;
    }
    iSize = sizeof(sHead);
    TRACE(D_00487420);
    TRACE(D_00487440, sHead.iPathNum);
    for (INT i = 0; i < sHead.iPathNum; i++) {
        PATH_VECTOR_PARAM sVecParam;
        PATH_PARAM sPathParam;
        FVECTOR vPath;
        INT* pParam;
        INT iCount;
        INT iZero;
        TRACE(D_00487458, i);
        memset(&sVecParam, 0, sizeof(sVecParam));
        sVecParam.iVer = 2;
        sVecParam.iVecNum = p->m_pPath->getPathVectorNum(i);
        iSize += sizeof(sVecParam) + sVecParam.iVecNum * sizeof(FVECTOR);
        p->m_pPath->getPathParam(i, sPathParam);
        for (INT j = 0; j < 10; j++) {
            if (sPathParam.iFlags & (1 << j)) iSize += 4;
        }
        iSize = (iSize + 0xC) & 0xFFFFFFF0;
        sVecParam.iFlags = sPathParam.iFlags;
        sVecParam.iNext = iSize;
        TRACE(D_00487480, i);
        iRet = CFileIO::writeFile(iFD, &sVecParam, sizeof(sVecParam));
        if (iRet < 0) {
            TRACE(D_004873F0, iRet);
            return FALSE;
        }
        TRACE(D_004874A0, i);
        _zeroVec(vPath);
        TRACE(D_004874C0, sVecParam.iVecNum);
        TRACE(D_004874D8, i);
        for (INT j = 0; j < sVecParam.iVecNum; j++) {
            vPath = p->m_pPath->getPathPosition(i, j);
            iRet = CFileIO::writeFile(iFD, &vPath, sizeof(vPath));
            if (iRet < 0) {
                TRACE(D_004873F0, iRet);
                return FALSE;
            }
        }
        TRACE(D_004874F0, i);
        iCount = 0;
        iZero = 0;
        TRACE(D_00487510, i);
        pParam = (INT*)&sPathParam.fTime;
        for (INT j = 0; j < 10; j++, pParam++) {
            if (sPathParam.iFlags & (1 << j)) {
                iRet = CFileIO::writeFile(iFD, pParam, sizeof(INT));
                if (iRet < 0) {
                    TRACE(D_004873F0, iRet);
                    return FALSE;
                }
                iCount++;
            }
        }
        TRACE(D_00487530, i);
        TRACE(D_00487550, iCount);
        iCount = -(iCount & 3) & 3;
        TRACE(D_00487570, iCount);
        TRACE(D_00487590, i);
        for (INT j = 0; j < iCount; j++) {
            iRet = CFileIO::writeFile(iFD, &iZero, sizeof(iZero));
            if (iRet < 0) {
                TRACE(D_004873F0, iRet);
                return FALSE;
            }
        }
        TRACE(D_004875B0, i);
    }
    iRet = CFileIO::closeFile(iFD);
    if (iRet < 0) {
        TRACE(D_004875D0, iRet);
        return FALSE;
    }
    return TRUE;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", savePath__14CDebugPathDataPCc);
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", func_0021F178);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", func_0021F208);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", func_0021F230);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", isDebugPathData__14CDebugPathData);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", getPathNum__14CDebugPathData);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", getPathVectorNum__14CDebugPathData);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", getPathVector__14CDebugPathData);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", getPathVectorData__14CDebugPathData);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", getPathPointer__14CDebugPathData);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", setActivePathNum__14CDebugPathDataii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", getActivePathNum__14CDebugPathData);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", addPath__14CDebugPathDataPC7FVECTORi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", delPath__14CDebugPathDatai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", limitClampActivePathNum__14CDebugPathData);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", limitRepeatActivePathNum__14CDebugPathData);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", incActivePathNum__14CDebugPathDatai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", decActivePathNum__14CDebugPathDatai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", setActivePathVecNum__14CDebugPathDatai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", setActivePathVecPosition__14CDebugPathDataRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", setActivePathVecPositionXYZ__14CDebugPathDataRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", getActivePathVecNum__14CDebugPathData);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", getActivePathVecPosition__14CDebugPathData);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", addPathVector__14CDebugPathDatai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", delPathVector__14CDebugPathDatai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", limitClampActivePathVecNum__14CDebugPathData);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", limitRepeatActivePathVecNum__14CDebugPathData);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", incActivePathVecNum__14CDebugPathDatai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", decActivePathVecNum__14CDebugPathDatai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", getActiveParam__14CDebugPathData);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", setActiveParam__14CDebugPathData);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", removePath__14CDebugPathData);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", getInstance__14CDebugPathData);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", func_00220AD0);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", func_00220B00);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", func_00220B30);
// SOURCE DATA: .data
