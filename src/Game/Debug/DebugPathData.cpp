#include "khtypes.h"
#define GAME_DEBUG_DEBUGPATHDATA_CPP
#define FORM_DEBUGWINDOW_FULL /* MATCH: CNakCamera; the original has Gprim.h's strings (PathData.h includes it first) */
#define COLLOBJ_H_COLLISION_CLASSES /* MATCH: CPointCollision (debugWinPathEditPosCamGround) */
#define TASK_H_NO_SLC_OVERFLOW_MSG /* MATCH: the message comes after DebugPathData.h's strings (placeholder below) */
#include "System/Graphic/PathData.h"
#include "System/Graphic/HermitePath.h"
#include "System/Graphic/Str.h"
#include "System/DebugProcInfo.h"
#include "Game/Debug/DebugPathData.h"
/* placeholder: the task container overflow message (System/TaskMan.h in the original) comes here */
inline void _taskSLCOverflowDebugPathData() {
    TRACE("\x83\x5e\x83\x58\x83\x4e\x97\x70\x83\x52\x83\x93\x83\x65\x83\x69\x82\xaa MAX \x82\xc9\x93\x9e\x92\x42\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
}
#define PATHDATACAMERA_H_PATH "System/Graphic/PathDataCamera.h" /* MATCH: the original includes it under this name */
#include "System/Graphic/PathDataCamera.h"
#include "Game/Collision/CollUtilA.h"
#include "Game/Collision/CollUtil.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
/* placeholder: an empty string follows CardBattle.h's strings in .rodata; which header it comes from is not known */
inline void _emptyStringPlaceholderDebugPathData() { TRACE(""); }
#include "Game/CardKind.h"
#include "Game/Player/Player.h"
#include "Form/DebugWindow.h"
#include "Form/DebugWindowTBL.h"
#include "Game/Debug/DebugWork.h"
#include "System/Graphic/WorldEnv.h"
#include "System/Coord.h"
#include "System/Math3D.h"
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

/* PARTIAL: .rodata still asm - the tables' strings by label */
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
BOOL debugWinPathEndOneYNCall(CDebugWindow* pObj, INT inY, INT inParam) {
    static INT iStep = 0;
    if (inParam & 5) iStep = 0;
    switch (iStep) {
    case 0:
        if (inParam & 0x20) {
            pObj->sendMessage(1, &DWT::debugWinYesNo);
            iStep++;
        }
        break;
    case 1:
        if (inParam & 0x800) {
            if (pObj->isChildAnswer()) {
                if (pObj->getChildAnswer() == 1) pObj->sendMessage(2, 0);
            }
            iStep = 0;
        }
        break;
    }
    return TRUE;
}
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
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x6D61472F\n.word 0x65442F65\n.word 0x2F677562\n.word 0x75626544\n.word 0x74615067\n.word 0x74614468\n.word 0x70632E61\n.word 0x31332870\n.word 0x00002939\n.word 0x00000000\n.word 0x8C837683\n.word 0x84834383\n.word 0xAA825B81\n.word 0xDD8DB691\n.word 0xC882B582\n.word 0xBD82A282\n.word 0xC08EDF82\n.word 0xC582738D\n.word 0xDC82AB82\n.word 0xF182B982\n.word 0x00000021\n.word 0x00000000\n.text\n");
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4098: .word 0\n""pHPC.4099: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditSimulateSpeedLineCall__FP12CDebugWindowii);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x6D61472F\n.word 0x65442F65\n.word 0x2F677562\n.word 0x75626544\n.word 0x74615067\n.word 0x74614468\n.word 0x70632E61\n.word 0x30342870\n.word 0x00002934\n.word 0x00000000\n.text\n");
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4103: .word 0\n""pHPC.4104: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditSimulateSpeedCamLineCall__FP12CDebugWindowii);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x6D61472F\n.word 0x65442F65\n.word 0x2F677562\n.word 0x75626544\n.word 0x74615067\n.word 0x74614468\n.word 0x70632E61\n.word 0x30352870\n.word 0x00002932\n.word 0x00000000\n.word 0xC68F518E\n.word 0xD782B782\n.word 0x6590AB82\n.word 0x58837083\n.word 0xB691AA82\n.word 0xB582DD8D\n.word 0xA282C882\n.word 0xDF82BD82\n.word 0x738DC08E\n.word 0xAB82C582\n.word 0xB982DC82\n.word 0x0021F182\n.text\n");
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4108: .word 0\n""pHPC.4109: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditSimulateSpeedCamLineLookCall__FP12CDebugWindowii);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x6D61472F\n.word 0x65442F65\n.word 0x2F677562\n.word 0x75626544\n.word 0x74615067\n.word 0x74614468\n.word 0x70632E61\n.word 0x30362870\n.word 0x00002933\n.word 0x00000000\n.text\n");
INT debugWinPathEditEtcParamAllCall(CDebugWindow* pObj, INT inParam) {
    if (inParam & 1) {
        CDebugPathData::getActiveParam();
        CDebugPathData::m_sParam.fProj = ProjectionToViewAngle(CDebugPathData::m_sParam.fProj);
        CDebugPathData::m_sParam.fProj2 = ProjectionToViewAngle(CDebugPathData::m_sParam.fProj2);
    }
    if (inParam & 2) {
        CDebugPathData::m_sParam.fProj = ViewAngleToProjection(CDebugPathData::m_sParam.fProj);
        CDebugPathData::m_sParam.fProj2 = ViewAngleToProjection(CDebugPathData::m_sParam.fProj2);
        CDebugPathData::setActiveParam();
    }
    return TRUE;
}
__asm__(".section .data"); /* PARTIAL: gcc doesn't know the asm functions above switched to .text */
DEBUG_WINDOW_LINE debugWinPathEditSimulateLine[17] = {
    {"\x83p\x83X%d\x8f\xe3\x82\xf0\x95\xa8\x91\xcc\x82\xaa\x88\xda\x93\xae", &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditSimulateSpeedLineCall, NULL},
    {"---------------------------\xf9V\x81\xab" "ATEL\x82\xc9\x91\xce\x89\x9e\x82\xb5\x82\xbd\x93\xae\x8d\xec\x81\xab\xf9" "A-------------------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x90" "e(\x92\x8d\x8e\x8b)\x83p\x83X\x8f\xe3\x82\xf0\x95\xa8\x91\xcc\x82\xaa\x88\xda\x93\xae \x83p\x83X%d\x8f\xe3\x82\xf0\x91\xce\x89\x9e\x82\xb5\x82\xc4\x83J\x83\x81\x83\x89\x82\xaa\x88\xda\x93\xae(\x88\xda\x93\xae\x92l\x82\xcd\x83J\x83\x81\x83\x89\x83p\x83X\x91\xa4)", &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditSimulateSpeedCamLineCall, NULL},
    {"\x83p\x83X%d\x8f\xe3\x82\xf0\x83J\x83\x81\x83\x89\x82\xaa\x83^\x81[\x83Q\x83" "b\x83g(\x8d\xa1\x82\xcd\x83v\x83\x8c\x83" "C\x83\x84\x81[)\x82\xf0\x92\x8d\x8e\x8b\x82\xb5\x82\xc4\x88\xda\x93\xae", &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditSimulateSpeedCamLineLookCall, NULL},
    {"\x83p\x83X%d\x8f\xe3\x82\xf0\x83J\x83\x81\x83\x89\x82\xaa\x83^\x81[\x83Q\x83" "b\x83g(\x8d\xa1\x82\xcd\x83v\x83\x8c\x83" "C\x83\x84\x81[)\x82\xf0\x83g\x83\x8c\x81[\x83X\x82\xb5\x82\xc4\x88\xda\x93\xae", &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditSimulateLineCall, NULL},
    {"\x90" "e(\x92\x8d\x8e\x8b)\x83p\x83X\x8f\xe3\x82\xf0\x95\xa8\x91\xcc\x82\xaa\x88\xda\x93\xae \x83p\x83X%d\x8f\xe3\x82\xf0\x91\xce\x89\x9e\x82\xb5\x82\xc4\x83J\x83\x81\x83\x89\x82\xaa\x88\xda\x93\xae(\x88\xda\x93\xae\x92l\x82\xcd\x88\xda\x93\xae\x83p\x83X\x91\xa4)", &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditSimulateSpeedCamLineCall, NULL},
    {"\x90" "e(\x92\x8d\x8e\x8b)\x83p\x83X\x8f\xe3\x82\xf0\x95\xa8\x91\xcc\x82\xaa\x88\xda\x93\xae \x83p\x83X%d\x8f\xe3\x82\xf0\x83J\x83\x81\x83\x89\x82\xaa\x88\xda\x93\xae(\x83J\x83\x81\x83\x89\x83p\x83X,\x88\xda\x93\xae\x83p\x83X,\x93\xc6\x97\xa7\x88\xda\x93\xae)", &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditSimulateSpeedCamLineCall, NULL},
    {"\xf9TL:\xf9" "A\x90" "e(\x92\x8d\x8e\x8b)\x83p\x83X\x8f\xe3\x82\xf0\x95\xa8\x91\xcc\x82\xaa\x88\xda\x93\xae \x83p\x83X%d\x8f\xe3\x82\xf0\x91\xce\x89\x9e\x82\xb5\x82\xc4\x83J\x83\x81\x83\x89\x82\xaa\x88\xda\x93\xae(\x88\xda\x93\xae\x92l\x82\xcd\x83J\x83\x81\x83\x89\x83p\x83X\x91\xa4)", &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditSimulateSpeedCamLineCall, NULL},
    {"\xf9TL:\xf9" "A\x83p\x83X%d\x8f\xe3\x82\xf0\x83J\x83\x81\x83\x89\x82\xaa\x83^\x81[\x83Q\x83" "b\x83g(\x8d\xa1\x82\xcd\x83v\x83\x8c\x83" "C\x83\x84\x81[)\x82\xf0\x92\x8d\x8e\x8b\x82\xb5\x82\xc4\x88\xda\x93\xae", &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditSimulateSpeedCamLineLookCall, NULL},
    {"\xf9TL:\xf9" "A\x90" "e(\x92\x8d\x8e\x8b)\x83p\x83X\x8f\xe3\x82\xf0\x95\xa8\x91\xcc\x82\xaa\x88\xda\x93\xae \x83p\x83X%d\x8f\xe3\x82\xf0\x91\xce\x89\x9e\x82\xb5\x82\xc4\x83J\x83\x81\x83\x89\x82\xaa\x88\xda\x93\xae(\x88\xda\x93\xae\x92l\x82\xcd\x88\xda\x93\xae\x83p\x83X\x91\xa4)", &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditSimulateSpeedCamLineCall, NULL},
    {"\xf9TL:\xf9" "A\x90" "e(\x92\x8d\x8e\x8b)\x83p\x83X\x8f\xe3\x82\xf0\x95\xa8\x91\xcc\x82\xaa\x88\xda\x93\xae \x83p\x83X%d\x8f\xe3\x82\xf0\x83J\x83\x81\x83\x89\x82\xaa\x88\xda\x93\xae(\x83J\x83\x81\x83\x89\x83p\x83X,\x88\xda\x93\xae\x83p\x83X,\x93\xc6\x97\xa7\x88\xda\x93\xae)", &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditSimulateSpeedCamLineCall, NULL},
    {"--------------------------------------------------------------------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83V\x83~\x83\x85\x83\x8c\x81[\x83g\x92\x86\x82\xcc \x83J\x83\x81\x83\x89\x88\xca\x92u \x92\x8d\x8e\x8b\x88\xca\x92u \x83p\x83X\x83\x89\x83" "C\x83\x93\x82\xcc\x95\\\x8e\xa6 : %s", &CDebugPathData::m_iCtrlFlg, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x2, NULL, NULL},
    {"\x83V\x83~\x83\x85\x83\x8c\x81[\x83g\x92\x86\x82\xcc \x83J\x83\x81\x83\x89\x82\xcc\x93\x96\x82\xbd\x82\xe8\x94\xbb\x92\xe8\x82\xf0\x8ds\x82\xa4             : %s", &CDebugPathData::m_iCtrlFlg, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x4, NULL, NULL},
    {"--------------------------------------------------------------------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"                   \xf9T(\x83p\x83\x89\x83\x81\x81[\x83^\x90\xdd\x92\xe8\x82\xcc\x93\xe0\x97" "e\x82\xf0\x8c\xb3\x82\xc9\x88\xda\x93\xae\x82\xb5\x82\xdc\x82\xb7)                   ", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinPathEditSimulate = {"\x83V\x83~\x83\x85\x83\x8c\x81[\x83g(PAD2\x82\xccSTART\x82\xc5\x8fI\x97\xb9)(PAD2\x82\xcc\x81\xa1\x82\xc5\x83|\x81[\x83Y)", -240, -144, 40, 16, 12, 0, 2, debugWinPathEditSimulateLine, NULL, NULL};

DEBUG_WINDOW_LINE debugWinPathEditEtcParamLine[28] = {
    {"----\x83p\x83X\x88\xda\x93\xae\x97p\x81" "A\x83J\x83\x81\x83\x89\x88\xda\x93\xae\x97p\x90\xdd\x92\xe8----", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x92\xb8\x93_\x92P\x88\xca\x82\xcc\x88\xda\x93\xae\x91\xac\x93x\x82\xf0\x97L\x8c\xf8\x82\xc9\x82\xb7\x82\xe9: %s", &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x4000000, NULL, NULL},
    {"\x83p\x83X\x92P\x88\xca\x82\xcc\x8e\x9e\x8a\xd4\x8e\xae\x88\xda\x93\xae\x8fo\x97\xcd      : %s", &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x1, NULL, NULL},
    {"      :% 9.2f Sec", &CDebugPathData::m_sParam.fTime, 4, {0}, 1, 0.01f, 1e+04f, 0.0f, NULL, 0, &debugWinPathLineAddSubCall, NULL},
    {"\x83p\x83X\x92P\x88\xca\x82\xcc\x91\xac\x93x\x8e\xae\x88\xda\x93\xae\x8fo\x97\xcd      : %s", &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x2, NULL, NULL},
    {"      :% 9.2f km/h \xf9T(2\x92\xb8\x93_\x82\xcc\x8e\x9e\x82\xcd\x96\xa2\x83T\x83|\x81[\x83g)", &CDebugPathData::m_sParam.fSpeed, 4, {0}, 1, 0.01f, 1e+04f, 0.0f, NULL, 0, &debugWinPathLineAddSubCall, NULL},
    {"-----------\x83J\x83\x81\x83\x89\x8c\xc5\x97L\x90\xdd\x92\xe8-----------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x92\x8d\x8e\x8b\x92\xc7\x8f]\x83^\x81[\x83Q\x83" "b\x83g\x82\xc6\x82\xcc\x8b\x97\x97\xa3\x8fo\x97\xcd: %s", &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x4, NULL, NULL},
    {"      :% 9.2f", &CDebugPathData::m_sParam.fTargetDistance, 4, {0}, 1, 0.01f, 1e+04f, 0.0f, NULL, 0, &debugWinPathLineAddSubCall, NULL},
    {"\x92\x8d\x8e\x8b\x83^\x81[\x83Q\x83" "b\x83g\x82\xccYOffset\x8fo\x97\xcd   : %s", &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x8, NULL, NULL},
    {"      :% 9.2f", &CDebugPathData::m_sParam.fTargetOffsetY, 4, {0}, 1, -1e+04f, 1e+04f, 0.0f, NULL, 0, &debugWinPathLineAddSubCall, NULL},
    {"\x92\x8d\x8e\x8b\x92\xc7\x8f]\x92x\x89\x84\x8cW\x90\x94\x8fo\x97\xcd          : %s", &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x40, NULL, NULL},
    {"      :% 1.2f", &CDebugPathData::m_sParam.fLookDelay, 4, {0}, 1, 0.01f, 1.0f, 0.0f, NULL, 0, &debugWinPathLineAddSubCall, NULL},
    {"\x89\xe6\x8ap\x8fo\x97\xcd                      : %s", &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x20, NULL, NULL},
    {"      :% 3.2f\x93x", &CDebugPathData::m_sParam.fProj, 4, {0}, 1, 1.0f, 1.8e+02f, 0.0f, NULL, 0, &debugWinPathLineAddSubCall, NULL},
    {"\x8fI\x97\xb9\x8e\x9e\x82\xcc\x89\xe6\x8ap\x8fo\x97\xcd              : %s", &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x200, NULL, NULL},
    {"      :% 3.2f\x93x", &CDebugPathData::m_sParam.fProj2, 4, {0}, 1, 1.0f, 1.8e+02f, 0.0f, NULL, 0, &debugWinPathLineAddSubCall, NULL},
    {"\x83p\x83X\x8aJ\x8en\x8e\x9e\x82\xcc\x95\xe2\x8a\xd4\x8cW\x90\x94\x8fo\x97\xcd      : %s", &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x80, NULL, NULL},
    {"      :% 6.2f", &CDebugPathData::m_sParam.fLearpIn, 4, {0}, 1, 1.0f, 999.0f, 0.0f, NULL, 0, &debugWinPathLineAddSubCall, NULL},
    {"\x83p\x83X\x8fI\x97\xb9\x8e\x9e\x82\xcc\x95\xe2\x8a\xd4\x8cW\x90\x94\x8fo\x97\xcd      : %s", &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x100, NULL, NULL},
    {"      :% 6.2f", &CDebugPathData::m_sParam.fLearpOut, 4, {0}, 1, 1.0f, 999.0f, 0.0f, NULL, 0, &debugWinPathLineAddSubCall, NULL},
    {"\x92\x8d\x8e\x8b\x83p\x83X\x82\xc6\x82\xcc\x8a\xd6\x8cW\x82\xf0\x88\xea\x92\xe8\x82\xc9\x82\xb7\x82\xe9  : %s", &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x20000000, NULL, NULL},
    {"--------------\x82\xbb\x82\xcc\x91\xbc----------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83p\x83X\x82\xf0\x83\x8b\x81[\x83v\x82\xc9\x82\xb7\x82\xe9            : %s", &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x40000000, NULL, NULL},
    {"\x83p\x83X\x88\xda\x93\xae\x82\xcd\x90\xfc\x8c`\x95\xe2\x8a\xd4\x82\xc9\x82\xb7\x82\xe9      : %s", &CDebugPathData::m_sParam.iFlags, 4, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0x80000000, NULL, NULL},
    {"------------------------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\xf9U\x83p\x83\x89\x83\x81\x81[\x83^\x82\xcc\x95\xcf\x8dX\x97p\x91\x9d\x8c\xb8\x92l :% 8.2f\xf9" "A", &CDebugPathData::m_fValuePlus, 4, {0}, 17, 0.01f, 1e+03f, 1e+01f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinPathEditEtcParam = {"\x82\xbb\x82\xcc\x91\xbc\x83p\x83\x89\x83\x81\x81[\x83^\x90\xdd\x92\xe8", -136, -176, 18, 26, 12, 0, 2, debugWinPathEditEtcParamLine, NULL, &debugWinPathEditEtcParamAllCall};
BOOL debugWinPathEditPosCamGround(CDebugWindow* pObj, INT inY, INT inParam) {
    static INT iStep = 0;
    if (inParam & 5) iStep = 0;
    switch (iStep) {
    case 0:
        if (inParam & 0x20) {
            pObj->sendMessage(1, &DWT::debugWinYesNo);
            iStep++;
        }
        break;
    case 1:
        if (inParam & 0x800) {
            if (pObj->isChildAnswer() && pObj->getChildAnswer() == 1) {
                FVECTOR vPos = CDebugPathData::m_vActivePathPos;
                FVECTOR v1;
                FVECTOR v2;
                FVECTOR vo;
                CPointCollision cColl;
                vPos.w = 1.0f;
                v1 = vPos;
                v2 = vPos;
                v1.y += 0.5f;
                v2.y -= 10.0f;
                cColl.setStartPos(v1);
                cColl.setEndPos(v2);
                if (CCollUtil::detectMCD(cColl)) {
                    vo = cColl.m_outDetect;
                    vPos.y = vo.y;
                    CDebugPathData::setActivePathVecPositionXYZ(vPos);
                    CDebugPathData::getCam()->SetCameraLook(CDebugPathData::m_vActivePathPos);
                } else {
                    pObj->sendMessage(1, "\x92n\x96\xca\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1!");
                }
            }
            iStep = 0;
        }
        break;
    }
    return TRUE;
}
BOOL debugWinPathEditPosCamReset(CDebugWindow* pObj, INT inY, INT inParam) {
    static INT iStep = 0;
    if (inParam & 5) iStep = 0;
    switch (iStep) {
    case 0:
        if (inParam & 0x20) {
            pObj->sendMessage(1, &DWT::debugWinYesNo);
            iStep++;
        }
        break;
    case 1:
        if (inParam & 0x800) {
            if (pObj->isChildAnswer() && pObj->getChildAnswer() == 1) {
                CDebugPathData::setActivePathVecPosition(CDebugPathData::m_vBackUpPos);
                CDebugPathData::getCam()->SetCameraLook(CDebugPathData::m_vActivePathPos);
            }
            iStep = 0;
        }
        break;
    }
    return TRUE;
}
INT debugWinPathEditPosCamAllCall(CDebugWindow* pObj, INT inParam) {
    if (inParam & 1) {
        CDebugPathData::m_vBackUpPos = CDebugPathData::m_vActivePathPos;
        CDebugPathData::getCam()->SetCameraLook(CDebugPathData::m_vActivePathPos);
    }
    if (!CDebugPathData::getCam()->GetType()) {
        FVECTOR vLook;
        CDebugPathData::getCam()->GetCameraLook(vLook);
        CDebugPathData::setActivePathVecPositionXYZ(vLook);
    }
    return TRUE;
}
__asm__(".section .data"); /* PARTIAL: gcc doesn't know the asm functions above switched to .text */
DEBUG_WINDOW_LINE debugWinPathEditPosCamLine[8] = {
    {"\x88\xca\x92u\x95\xcf\x8dX\x82\xcc\x8fI\x97\xb9", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEndOneCall, NULL},
    {"--------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x92n\x96\xca\x82\xc9\x8bz\x92\x85\x82\xb7\x82\xe9", &CDebugWindow::none, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOnOff, 0, &debugWinPathEditPosCamGround, NULL},
    {"\x88\xca\x92u\x83\x8a\x83Z\x83" "b\x83g", &CDebugWindow::none, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOnOff, 0, &debugWinPathEditPosCamReset, NULL},
    {"--------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"CAM\x91\xac\x93x:%s", (char*)&DebugWork::g_dw + 0x18D, 1, {0}, 0, 0.0f, 3.0f, 1.0f, DWT::szCamSpeed, 0, NULL, NULL},
    {"CAM\x8f\xee\x95\xf1:%s", (char*)&DebugWork::g_dw + 0x18C, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOnOff, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinPathEditPosCam = {CDebugPathData::m_szPathActName, -248, -176, 7, 8, 12, 0, 6, debugWinPathEditPosCamLine, NULL, &debugWinPathEditPosCamAllCall};
BOOL debugWinPathEditLineCamPosCall(CDebugWindow* pObj, INT inY, INT inParam) {
    static INT iStep = 0;
    if (inParam & 5) iStep = 0;
    switch (iStep) {
    case 0:
        if (inParam & 0x20) {
            sprintf(CDebugPathData::m_szPathActName, "%s-%d-%d", CDebugPathData::m_szPathName,
                    CDebugPathData::m_iActivePath, CDebugPathData::m_iActivePathNum);
            pObj->sendMessage(1, &debugWinPathEditPosCam);
            pObj->dispOff(0);
            CDebugPathData::getCam()->Action(0x2F);
            iStep++;
        }
        break;
    case 1:
        if (inParam & 0x800) {
            pObj->dispOn(0);
            CDebugPathData::getCam()->Action(0x1B);
            iStep = 0;
        }
        break;
    }
    return TRUE;
}
BOOL debugWinPathEditLineAddPoaCall(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x80) {
        pObj->addValue(-1, NULL, NULL, &CDebugPathData::m_fValuePlus, NULL);
        CDebugPathData::setActivePathVecPosition(CDebugPathData::m_vActivePathPos);
        CDebugPathData::getCam()->SetCameraLook(CDebugPathData::m_vActivePathPos);
    } else if (inParam & 0x100) {
        pObj->addValue(1, NULL, NULL, &CDebugPathData::m_fValuePlus, NULL);
        CDebugPathData::setActivePathVecPosition(CDebugPathData::m_vActivePathPos);
        CDebugPathData::getCam()->SetCameraLook(CDebugPathData::m_vActivePathPos);
    }
    return TRUE;
}
BOOL debugWinPathEditLineAddRotCall(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 4) CDebugPathData::m_iCtrlFlg |= 1;
    if (inParam & 0xA) CDebugPathData::m_iCtrlFlg &= ~1;
    FLOAT fRoll = _getPathVectorConvWRoll(CDebugPathData::m_vActivePathPos.w);
    FLOAT fPlus = CDebugPathData::m_fValuePlus * 0.017453292f;
    if (inParam & 0x80) {
        fRoll = Math3D::LimitPI(fRoll - fPlus);
        _setPathVectorConvWRoll(&CDebugPathData::m_vActivePathPos.w, fRoll);
        CDebugPathData::setActivePathVecPosition(CDebugPathData::m_vActivePathPos);
        CDebugPathData::getCam()->SetCameraLook(CDebugPathData::m_vActivePathPos);
    } else if (inParam & 0x100) {
        fRoll = Math3D::LimitPI(fRoll + fPlus);
        _setPathVectorConvWRoll(&CDebugPathData::m_vActivePathPos.w, fRoll);
        CDebugPathData::setActivePathVecPosition(CDebugPathData::m_vActivePathPos);
        CDebugPathData::getCam()->SetCameraLook(CDebugPathData::m_vActivePathPos);
    }
    return TRUE;
}
BOOL debugWinPathEditLineAddSpdCall(CDebugWindow* pObj, INT inY, INT inParam) {
    FLOAT fSpeed = _getPathVectorConvWSpeed(CDebugPathData::m_vActivePathPos.w);
    FLOAT fPlus = CDebugPathData::m_fValuePlus;
    if (inParam & 0x80) {
        fSpeed -= fPlus;
        if (fSpeed < 0.01f) fSpeed = 0.0f;
        _setPathVectorConvWSpeed(&CDebugPathData::m_vActivePathPos.w, fSpeed);
        CDebugPathData::setActivePathVecPosition(CDebugPathData::m_vActivePathPos);
        CDebugPathData::getCam()->SetCameraLook(CDebugPathData::m_vActivePathPos);
    } else if (inParam & 0x100) {
        fSpeed += fPlus;
        if (fSpeed > 1023.0f) fSpeed = 1023.0f;
        _setPathVectorConvWSpeed(&CDebugPathData::m_vActivePathPos.w, fSpeed);
        CDebugPathData::setActivePathVecPosition(CDebugPathData::m_vActivePathPos);
        CDebugPathData::getCam()->SetCameraLook(CDebugPathData::m_vActivePathPos);
    }
    return TRUE;
}
__asm__(".section .data"); /* PARTIAL: gcc doesn't know the asm functions above switched to .text (its static) */
BOOL debugWinPathLineNameSaveCall(CDebugWindow* pObj, INT inY, INT inParam) {
    static INT iStep = 0;
    if (inParam & 5) iStep = 0;
    switch (iStep) {
    case 0:
        if (inParam & 0x20) {
            pObj->sendMessage(1, &DWT::debugWinName);
            iStep++;
        }
        break;
    case 1:
        if (inParam & 0x800) {
            if (pObj->isChildAnswer()) {
                strcpy(CDebugPathData::m_szPathName, (char*)pObj->getChildAnswer());
                if (CDebugPathData::savePath(CDebugPathData::m_szPathName)) {
                    pObj->sendMessage(1, "\x83Z\x81[\x83u\x82\xb5\x82\xdc\x82\xb5\x82\xbd!");
                    iStep++;
                } else {
                    pObj->sendMessage(1, "\x83Z\x81[\x83u\x82\xc9\x8e\xb8\x94s\x82\xb5\x82\xdc\x82\xb5\x82\xbd!");
                    iStep--;
                }
            } else {
                iStep--;
            }
        }
        break;
    case 2:
        iStep = 0;
        break;
    }
    return TRUE;
}
BOOL debugWinPathLineSaveCall(CDebugWindow* pObj, INT inY, INT inParam) {
    static INT iStep = 0;
    if (inParam & 5) iStep = 0;
    switch (iStep) {
    case 0:
        if (inParam & 0x20) {
            pObj->sendMessage(1, &DWT::debugWinYesNo);
            iStep++;
        }
        break;
    case 1:
        if (inParam & 0x800) {
            if (pObj->isChildAnswer() && pObj->getChildAnswer() == 1) {
                if (CDebugPathData::savePath(NULL)) {
                    pObj->sendMessage(1, "\x83Z\x81[\x83u\x82\xb5\x82\xdc\x82\xb5\x82\xbd!");
                    iStep++;
                } else {
                    pObj->sendMessage(1, "\x83Z\x81[\x83u\x82\xc9\x8e\xb8\x94s\x82\xb5\x82\xdc\x82\xb5\x82\xbd!");
                    iStep--;
                }
            } else {
                iStep--;
            }
        }
        break;
    case 2:
        iStep = 0;
        break;
    }
    return TRUE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditNum__FP12CDebugWindowii);
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4154: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditAdd__FP12CDebugWindowii);
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4158: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditDel__FP12CDebugWindowii);
__asm__(".rdata\n.align 3\n.word 0x5F93B892\n.word 0xC58DCD82\n.word 0xEA88E192\n.word 0x4B95C282\n.word 0xC5827697\n.word 0x4281B782\n.word 0x9C8FED8D\n.word 0xAB82C582\n.word 0xB982DC82\n.word 0x0021F182\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditPathNum__FP12CDebugWindowii);
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4165: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditPathAddLineCall__FP12CDebugWindowii);
__asm__(".rdata\n.align 3\n.word 0x218D923C\n.word 0x8DBB8C3E\n.word 0x914991DD\n.word 0x828692F0\n.word 0x837083CC\n.word 0x81CD8258\n.word 0x82F98A41\n.word 0x826590C9\n.word 0x8DB691AA\n.word 0x82B782DD\n.word 0x82BD82E9\n.word 0x8DC08EDF\n.word 0x82B78273\n.word 0x90C682E9\n.word 0x95AA8265\n.word 0x82588DCF\n.word 0x82EA82B3\n.word 0x21B782DC\n.word 0x00000000\n.word 0x00000000\n.text\n");
/* PARTIAL: the local statics of the asm function below (data from source, function still asm) */
__asm__(".data\n.align 2\n" "iStep.4169: .word 0\n" ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditPathDel__FP12CDebugWindowii);
__asm__(".rdata\n.align 3\n.word 0x58837083\n.word 0xC58DCD82\n.word 0xEA88E192\n.word 0x4B95C282\n.word 0xC5827697\n.word 0x4281B782\n.word 0x9C8FED8D\n.word 0xAB82C582\n.word 0xB982DC82\n.word 0x0021F182\n.text\n");
INT debugWinPathEditDispCall(char* pStr, INT iLineY, const void* pValue) {
    if (iLineY == 0) {
        if (CDebugPathData::m_sParam.iFlags & 0x10) {
            sprintf(pStr, "-\xf9V\x82\xb1\x82\xcc\x83p\x83X : %d/%d \x90" "e:%2d\xf9" "A(\x83J)-", CDebugPathData::getActivePathNum(), CDebugPathData::getPathNum(),
                    CDebugPathData::m_sParam.iParentLink);
        } else {
            sprintf(pStr, "-\xf9V\x82\xb1\x82\xcc\x83p\x83X : %d/%d \x90" "e:\x96\xb3\xf9" "A-----", CDebugPathData::getActivePathNum(), CDebugPathData::getPathNum());
        }
    } else if (iLineY == 5) {
        sprintf(pStr, "-\xf9V\x82\xb1\x82\xcc\x92\xb8\x93_ :%2d/%2d (L2-R2)\xf9" "A--", CDebugPathData::getActivePathVecNum(), CDebugPathData::getPathVectorNum());
    } else if (iLineY == 12) {
        sprintf(pStr, "  |Roll:% 7.2f(% 6.3f)|", _getPathVectorConvWRoll(CDebugPathData::m_vActivePathPos.w) * 57.295776f,
                _getPathVectorConvWRoll(CDebugPathData::m_vActivePathPos.w));
    } else if (iLineY == 13) {
        sprintf(pStr, "  |\x88\xda\x93\xae\x91\xac\x93x:%7.2fKm/h|", _getPathVectorConvWSpeed(CDebugPathData::m_vActivePathPos.w));
    }
    return TRUE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditAllCall__FP12CDebugWindowi);
__asm__(".section .data"); /* PARTIAL: gcc doesn't know the asm functions above switched to .text */
DEBUG_WINDOW_LINE debugWinPathEditPathAddLine[5] = {
    {"\x83p\x83X%d\x82\xcc\x83R\x83s\x81[\x82\xf0\x92\xc7\x89\xc1", &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditPathAddLineCall, NULL},
    {"\x83p\x83X%d\x82\xf0\x90" "e(\x92\x8d\x8e\x8b\x93_)\x82\xc6\x82\xb5\x82\xc4\x83\x8a\x83\x93\x83N\x82\xf0\x92\xa3\x82\xc1\x82\xc4\x83R\x83s\x81[\x82\xf0\x8eq(\x83J\x83\x81\x83\x89)\x82\xc6\x82\xb5\x82\xc4\x92\xc7\x89\xc1", &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditPathAddLineCall, NULL},
    {"\x83p\x83X%d\x82\xf0\x8eq(\x83J\x83\x81\x83\x89)\x82\xc6\x82\xb5\x82\xc4\x83\x8a\x83\x93\x83N\x82\xf0\x92\xa3\x82\xc1\x82\xc4\x83R\x83s\x81[\x82\xf0\x90" "e(\x92\x8d\x8e\x8b\x93_)\x82\xc6\x82\xb5\x82\xc4\x92\xc7\x89\xc1", &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditPathAddLineCall, NULL},
    {"\x83p\x83X\x82\xf0\x92\xc7\x89\xc1", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditPathAddLineCall, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinPathEditPathAdd = {"\x83p\x83X\x82\xcc\x92\xc7\x89\xc1", -216, -144, 33, 8, 12, 0, 2, debugWinPathEditPathAddLine, NULL, NULL};

DEBUG_WINDOW_LINE debugWinPathSaveLine[3] = {
    {"    \x8f\xe3\x8f\x91\x82\xab\x83Z\x81[\x83u", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathLineSaveCall, NULL},
    {" \x96\xbc\x91O\x82\xf0\x95t\x82\xaf\x82\xc4\x83Z\x81[\x83u", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathLineNameSaveCall, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinPathSave = {"\x83p\x83X\x83" "f\x81[\x83^\x82\xcc\x83Z\x81[\x83u", -216, -144, 10, 2, 14, 0, 2, debugWinPathSaveLine, NULL, NULL};

DEBUG_WINDOW_LINE debugWinPathEditLine[23] = {
    {"", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditPathNum, NULL},
    {" \x83p\x83X\x82\xf0\x92\xc7\x89\xc1", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinPathEditPathAdd},
    {" \xf9T\x83p\x83X%d\x82\xcc\x8d\xed\x8f\x9c", &CDebugPathData::m_iActivePath, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditPathDel, NULL},
    {" \x82\xbb\x82\xcc\x91\xbc\x83p\x83\x89\x83\x81\x81[\x83^\x82\xcc\x90\xdd\x92\xe8\x82\xd6", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinPathEditEtcParam},
    {" \x83V\x83~\x83\x85\x83\x8c\x81[\x83g\x90\xdd\x92\xe8\x82\xd6", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinPathEditSimulate},
    {"", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditNum, NULL},
    {" \x92\xb8\x93_%d\x82\xcc\x8e\x9f\x82\xcc\x88\xca\x92u\x82\xd6\x92\xc7\x89\xc1", &CDebugPathData::m_iActivePathNum, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditAdd, NULL},
    {" \x92\xb8\x93_%d\x82\xf0\x92\x8d\x8e\x8b\x93_\x88\xda\x93\xae\x82\xc5\x95\xcf\x8dX", &CDebugPathData::m_iActivePathNum, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditLineCamPosCall, NULL},
    {"  +--\x8d\xc0\x95W\x92l\x82\xf0\x92\xbc\x90\xda\x95\xcf\x8dX--+  ", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"  |      X :% 9.2f  |", &CDebugPathData::m_vActivePathPos.x, 4, {0}, 1, -1e+05f, 1e+05f, 0.0f, NULL, 0, &debugWinPathEditLineAddPoaCall, NULL},
    {"  |      Y :% 9.2f  |", &CDebugPathData::m_vActivePathPos.y, 4, {0}, 1, -1e+05f, 1e+05f, 0.0f, NULL, 0, &debugWinPathEditLineAddPoaCall, NULL},
    {"  |      Z :% 9.2f  |", &CDebugPathData::m_vActivePathPos.z, 4, {0}, 1, -1e+05f, 1e+05f, 0.0f, NULL, 0, &debugWinPathEditLineAddPoaCall, NULL},
    {"", &CDebugPathData::m_vActivePathPos.w, 4, {0}, 1, -3.1415927f, 3.1415927f, 0.0f, NULL, 0, &debugWinPathEditLineAddRotCall, NULL},
    {"", &CDebugPathData::m_vActivePathPos.w, 4, {0}, 1, -3.1415927f, 3.1415927f, 0.0f, NULL, 0, &debugWinPathEditLineAddSpdCall, NULL},
    {"  | \xf9U\x91\x9d\x8c\xb8\x92l :% 9.2f\xf9" "A  |", &CDebugPathData::m_fValuePlus, 4, {0}, 17, 0.01f, 1e+03f, 1e+01f, NULL, 0, NULL, NULL},
    {" \xf9T\x92\xb8\x93_%d\x82\xcc\x8d\xed\x8f\x9c", &CDebugPathData::m_iActivePathNum, 4, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEditDel, NULL},
    {"--------------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {" CAM\x91\xac\x93x : %s", (char*)&DebugWork::g_dw + 0x18D, 1, {0}, 0, 0.0f, 3.0f, 1.0f, DWT::szCamSpeed, 0, NULL, NULL},
    {" CAM\x8f\xee\x95\xf1 : %s", (char*)&DebugWork::g_dw + 0x18C, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOnOff, 0, NULL, NULL},
    {"--------------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {" \x83p\x83X\x83" "f\x81[\x83^\x82\xcc\x83Z\x81[\x83u", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinPathSave},
    {" \x83p\x83X\x90\xdd\x92\xe8\x82\xcc\x8fI\x97\xb9", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEndOneYNCall, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinPathEdit = {CDebugPathData::m_szPathName, -248, -176, 13, 22, 12, 0, 6, debugWinPathEditLine, &debugWinPathEditDispCall, &debugWinPathEditAllCall};
BOOL debugWinPathListLineCall(CDebugWindow* pObj, INT iLineY, INT inParam) {
    static INT iStep = 0;
    if (inParam & 5) iStep = 0;
    switch (iStep) {
    case 0:
        if ((inParam & 0x800) && pObj->isChildAnswer() && pObj->getChildAnswer() == 1) {
            strcpy(CDebugPathData::m_szPathName, CDebugPathData::m_szNames[iLineY]);
            if (CDebugPathData::entryPath(CDebugPathData::m_szPathName)) {
                for (INT i = 0; i < 32; i++) {
                    if (CDebugPathData::m_szPathName[i] == '\0' || CDebugPathData::m_szPathName[i] == '.') {
                        CDebugPathData::m_szPathName[i] = '\0';
                        break;
                    }
                }
                iStep++;
            }
        }
        break;
    case 1:
        pObj->dispOff(1);
        pObj->sendMessage(1, &debugWinPathEdit);
        iStep++;
        break;
    case 2:
        CDebugPathData::removePath();
        pObj->dispOn(1);
        iStep = 0;
        break;
    }
    return TRUE;
}
INT debugWinPathListDispCall(char* pBuf, INT iIdx, const void* pParam) {
    sprintf(pBuf, " %s", CDebugPathData::m_szNames[iIdx]);
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
    {"", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathListLineCall, &DWT::debugWinYesNo},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinPathList = {"\x83p\x83X\x83\x8a\x83X\x83g", -208, -136, 7, 20, 14, 0, 8, debugWinPathListLine, &debugWinPathListDispCall, &debugWinPathListAllCall};
BOOL debugWinPathLineNewCall(CDebugWindow* pObj, INT inY, INT inParam) {
    static INT iStep = 0;
    if (inParam & 5) iStep = 0;
    switch (iStep) {
    case 0:
        if (inParam & 0x20) {
            pObj->sendMessage(1, &DWT::debugWinName);
            iStep++;
        }
        break;
    case 1:
        if (inParam & 0x800) {
            if (pObj->isChildAnswer()) {
                strcpy(CDebugPathData::m_szPathName, (char*)pObj->getChildAnswer());
                if (CDebugPathData::newPath(CDebugPathData::m_szPathName)) iStep++;
            } else {
                iStep--;
            }
        }
        break;
    case 2:
        pObj->dispOff(0);
        pObj->sendMessage(1, &debugWinPathEdit);
        iStep++;
        break;
    case 3:
        CDebugPathData::removePath();
        pObj->dispOn(0);
        iStep = 0;
        break;
    }
    return TRUE;
}
BOOL debugWinPathLineNameLoadCall(CDebugWindow* pObj, INT inY, INT inParam) {
    static INT iStep = 0;
    if (inParam & 5) iStep = 0;
    switch (iStep) {
    case 0:
        if (inParam & 0x20) {
            pObj->sendMessage(1, &DWT::debugWinName);
            iStep++;
        }
        break;
    case 1:
        if (inParam & 0x800) {
            if (pObj->isChildAnswer()) {
                strcpy(CDebugPathData::m_szPathName, (char*)pObj->getChildAnswer());
                if (CDebugPathData::loadPath(CDebugPathData::m_szPathName)) {
                    pObj->sendMessage(1, "\x83\x8d\x81[\x83h\x82\xb5\x82\xdc\x82\xb5\x82\xbd!");
                    iStep++;
                } else {
                    pObj->sendMessage(1, "\x83\x8d\x81[\x83h\x82\xc9\x8e\xb8\x94s\x82\xb5\x82\xdc\x82\xb5\x82\xbd!");
                    iStep--;
                }
            } else {
                iStep--;
            }
        }
        break;
    case 2:
        pObj->dispOff(0);
        pObj->sendMessage(1, &debugWinPathEdit);
        iStep++;
        break;
    case 3:
        CDebugPathData::removePath();
        pObj->dispOn(0);
        iStep = 0;
        break;
    }
    return TRUE;
}
__asm__(".section .data"); /* PARTIAL: gcc doesn't know the asm functions above switched to .text */
DEBUG_WINDOW_LINE debugWinPathLine[5] = {
    {"\x8d\xa1\x82\xcc\x83V\x81[\x83\x93\x82\xcc\x93\xc7\x8d\x9e\x8d\xcf\x83\x8a\x83\\\x81[\x83X\x88\xea\x97\x97\x82\xa9\x82\xe7\x83\x8d\x81[\x83h\xf9T(\x83J\x83" "e\x83S\x83\x8a\x83G\x83" "f\x83" "B\x83^\x82\xc5\x82\xcc\x8dX\x90V\x82\xaa\x95K\x97v)\xf9" "A", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinPathList},
    {"\xf9T\x82o\x82" "b\xf9" "A\x8f\xe3\x82\xcc\x83t\x83@\x83" "C\x83\x8b\x96\xbc\x82\xf0\x8ew\x92\xe8\x82\xb5\x82\xc4\x83\x8d\x81[\x83h", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathLineNameLoadCall, NULL},
    {"\x83t\x83@\x83" "C\x83\x8b\x82\xcc\x90V\x8bK\x8d\xec\x90\xac", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathLineNewCall, NULL},
    {"\x8fI\x82\xed\x82\xe9", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathEndOneCall, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinPath = {"\x83p\x83X\x90\xdd\x92\xe8\x95\xd2\x8fW\x83g\x83" "b\x83v", -240, -168, 34, 20, 12, 0, 20, debugWinPathLine, NULL, NULL};

DEBUG_WINDOW_LINE debugWinPathNoEndLine[4] = {
    {"\x8d\xa1\x82\xcc\x83V\x81[\x83\x93\x82\xcc\x93\xc7\x8d\x9e\x8d\xcf\x83\x8a\x83\\\x81[\x83X\x88\xea\x97\x97\x82\xa9\x82\xe7\x83\x8d\x81[\x83h\xf9T(\x83J\x83" "e\x83S\x83\x8a\x83G\x83" "f\x83" "B\x83^\x82\xc5\x82\xcc\x8dX\x90V\x82\xaa\x95K\x97v)\xf9" "A", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinPathList},
    {"\xf9T\x82o\x82" "b\xf9" "A\x8f\xe3\x82\xcc\x83t\x83@\x83" "C\x83\x8b\x96\xbc\x82\xf0\x8ew\x92\xe8\x82\xb5\x82\xc4\x83\x8d\x81[\x83h", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathLineNameLoadCall, NULL},
    {"\x83t\x83@\x83" "C\x83\x8b\x82\xcc\x90V\x8bK\x8d\xec\x90\xac", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, &debugWinPathLineNewCall, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinPathNoEnd = {"\x83p\x83X\x90\xdd\x92\xe8\x95\xd2\x8fW\x83g\x83" "b\x83v", -240, -168, 34, 20, 12, 0, 22, debugWinPathNoEndLine, NULL, NULL};
/* Data step first: the inline CCPrim::create brings Gprim.h's "(613)"/"m_V" literals, and the unit's .rodata is
   still asm (objcopy: symbol .rodata required). Draft not compared yet. */
CDebugPathData::CDebugPathData(CTask* p, BOOL bEnd) : CTask(p, 0x4000) {
    CTask* pWnd;
    m_pPath = NULL;
    ignorePause(TRUE);
    m_szFileName[0] = 0;
    if (bEnd) {
        pWnd = new (com_malloc_d(0x1C0, __FILE__ "(1928)", RET_ADDR())) CDebugWindow(this, debugWinPath);
    } else {
        pWnd = new (com_malloc_d(0x1C0, __FILE__ "(1929)", RET_ADDR())) CDebugWindow(this, debugWinPathNoEnd);
    }
    m_hChildWnd = pWnd ? pWnd->getHandle() : 0;
    m_iActivePathNum = 0;
    m_pFrame = new (com_malloc_d(0xD8, __FILE__ "(1938)", RET_ADDR())) CPDFrameTime;
    m_gpPArrowPoly.create(0xB, 3);
    m_gpPArrowPoly.setLayerFG(2);
    CDebugWindow::enableTopDebugWindow(FALSE);
    CPlayer::enableDbgPadControl(FALSE);
}
CDebugPathData::~CDebugPathData() {
    delete m_pPath;
    m_pPath = NULL;
    CPDFrameTime*& pFrame = m_pFrame; /* MATCH: a reference keeps its load after the m_pPath store */
    delete pFrame;
    pFrame = NULL;
    m_pInstance = NULL;
    CDebugWindow::enableTopDebugWindow(TRUE);
    CPlayer::enableDbgPadControl(TRUE);
}
BOOL CDebugPathData::create(CTask* p, BOOL bEnd) {
    if (!m_pInstance) {
        m_pInstance = new (com_malloc_d(sizeof(CDebugPathData), __FILE__ "(1963)", RET_ADDR())) CDebugPathData(p, bEnd);
        return TRUE;
    }
    return FALSE;
}
void CDebugPathData::main() {
    if (isEnd()) out();
}
extern "C" void sceVu0RotMatrixZ(FMATRIX& m0, FMATRIX& m1, float rz);
struct _ArrowVecs { FVECTOR v[3]; };
void CDebugPathData::disp() {
    m_pFrame->disp(getFrameRateF());
    if (m_pDCam == CWorldEnv::getActiveCamera()) {
        if (m_pPath) m_pPath->dispDebugLine();
        if (m_iCtrlFlg & 1) {
            FMATRIX mMat;
            _ArrowVecs v = {{{0.0f, -64.0f, 0.0f, 1.0f}, {16.0f, 0.0f, 0.0f, 1.0f}, {-16.0f, 0.0f, 0.0f, 1.0f}}};
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
    p->m_pDCam = new (com_malloc_d(0x140, __FILE__ "(2054)", RET_ADDR())) CNakCamera(p, 1, 0x100, &vPos, 0.0f, (void**)&p->m_pDCam);
}
CNakCamera* CDebugPathData::getCam() { return getInstance()->m_pDCam; }
void CDebugPathData::removeCam() {
    CDebugPathData* pThis = getInstance();
    ((CTask*)pThis->m_pDCam)->out();
    pThis->m_pDCam = NULL;
}
void CDebugPathData::makeFileName(const CHAR* pszName, BOOL bExt) {
    CDebugPathData* pThis = getInstance();
    if (bExt) sprintf(pThis->m_szFileName, "host0:/%s.PTD", pszName);
    else sprintf(pThis->m_szFileName, "host0:/%s", pszName);
}
BOOL CDebugPathData::newPath(const CHAR* pName) {
    CDebugPathData* p = getInstance();
    makeFileName(pName, TRUE);
    delete p->m_pPath;
    p->m_pPath = NULL;
    p->m_pPath = new (com_malloc_d(0x5C, __FILE__ "(2088)", RET_ADDR())) CPathData((const PATH_DATA*)NULL, TRUE);
    if (!p->m_pPath) AssertApp(__FILE__, 2089, "p->m_pPath", 0);
    setActivePathNum(0, 0);
    return TRUE;
}
BOOL CDebugPathData::entryPath(const CHAR* pName) {
    CDebugPathData* p = getInstance();
    makeFileName(pName, FALSE);
    delete p->m_pPath;
    p->m_pPath = NULL;
    p->m_pPath = new (com_malloc_d(0x5C, __FILE__ "(2106)", RET_ADDR())) CPathData(pName, TRUE);
    if (!p->m_pPath) AssertApp(__FILE__, 2107, "p->m_pPath", 0);
    setActivePathNum(0, 0);
    return TRUE;
}
/* Weak tail first: this draft matches (V1), but `new CPathData` makes gcc emit the weak CVector<G_STRIP>
   destructor (the original's unnamed func_0021F178, which follows savePath) at the end of the unit. Needs
   CPathData(const PATH_DATA* pPathData, BOOL bAlloc) { init(pPathData, bAlloc); } in PathData.h. */
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
    pBuf = com_malloc_d(iSize, __FILE__ "(2132)", (UINT)-1);
    if (!pBuf) AssertApp(__FILE__, 2133, "pBuf", 0);
    if (CFileIO::readFile(iFD, pBuf, iSize) < 0) {
        if (pBuf) com_free(pBuf);
        return FALSE;
    }
    delete p->m_pPath;
    p->m_pPath = NULL;
    p->m_pPath = new (com_malloc_d(0x5C, __FILE__ "(2141)", RET_ADDR())) CPathData((const PATH_DATA*)pBuf, TRUE);
    if (!p->m_pPath) AssertApp(__FILE__, 2142, "p->m_pPath", 0);
    if (pBuf) com_free(pBuf);
    CFileIO::closeFile(iFD);
    return TRUE;
}
extern "C" void* memset(void*, int, unsigned int);
extern "C" char* strncpy(char*, const char*, unsigned int);
extern "C" int FlushCache(int op);
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
            pBuf = com_malloc_d(iSize, __FILE__ "(2163)", (UINT)-1);
            if (pBuf) {
                iRet = CFileIO::readFile(iFD, pBuf, iSize);
                if (iRet < 0) {
                    TRACE("%d:\x83o\x83" "b\x83N\x83" "A\x83" "b\x83v\x82\xb7\x82\xe9\x83t\x83@\x83" "C\x83\x8b\x82\xcc\x93\xc7\x82\xdd\x8d\x9e\x82\xdd\x82\xc9\x8e\xb8\x94s!!!!!!!!!!!!!!\n", iRet);
                    com_free(pBuf);
                    pBuf = NULL;
                }
            } else {
                TRACE("\x83o\x83" "b\x83N\x83" "A\x83" "b\x83v\x82\xf0\x8e\xe6\x82\xe9\x82\xbd\x82\xdf\x82\xcc\x83\x81\x83\x82\x83\x8a\x82\xaa\x91\xab\x82\xe8\x82\xdc\x82\xb9\x82\xf1!!!!!!!!!!!!!!\n");
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
            sprintf(&szOutFileName[i], ".BAK");
            iOutFD = CFileIO::openFile(szOutFileName, 0x602, 0x1FF);
            if (iOutFD < 0) {
                TRACE("%d:\x83o\x83" "b\x83N\x83" "A\x83" "b\x83v\x83t\x83@\x83" "C\x83\x8b\x82\xcc\x83I\x81[\x83v\x83\x93\x82\xc9\x8e\xb8\x94s!!!!!!!!!!!!!!\n", iOutFD);
            } else {
                iRet = CFileIO::writeFile(iOutFD, pBuf, iSize);
                if (iRet < 0) TRACE("%d:\x83o\x83" "b\x83N\x83" "A\x83" "b\x83v\x83t\x83@\x83" "C\x83\x8b\x82\xcc\x8f\x91\x82\xab\x8d\x9e\x82\xdd\x82\xc9\x8e\xb8\x94s!!!!!!!!!!!!!!\n", iRet);
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
__asm__(".rdata\n.align 3\n.word 0x40837483\n.word 0x8B834383\n.word 0x62837783\n.word 0xCC825F83\n.word 0xAB82918F\n.word 0xDD829E8D\n.word 0x6E8E4A8A\n.word 0x0000000A\n.word 0x833A6425\n.word 0x83408374\n.word 0x828B8343\n.word 0x82918FCC\n.word 0x829E8DAB\n.word 0x8EC982DD\n.word 0x217394B8\n.word 0x21212121\n.word 0x21212121\n.word 0x21212121\n.word 0x00000A21\n.word 0x00000000\n.word 0x40837483\n.word 0x8B834383\n.word 0x62837783\n.word 0xCC825F83\n.word 0xAB82918F\n.word 0xDD829E8D\n.word 0xED82498F\n.word 0x000AE882\n.word 0xAB82918F\n.word 0xDE829E8D\n.word 0x58837083\n.word 0x9490CC82\n.word 0x000A6425\n.word 0x00000000\n.word 0x2D2D2D2D\n.word 0x2D2D2D2D\n.word 0x8F3A6425\n.word 0x8DAB8291\n.word 0x82DD829E\n.word 0x82B782DC\n.word 0x2D2D2DE6\n.word 0x2D2D2D2D\n.word 0x00000A2D\n.word 0x00000000\n.word 0x833A6425\n.word 0x83588370\n.word 0x83628377\n.word 0x8FCC825F\n.word 0x8DAB8291\n.word 0x8ADD829E\n.word 0x0A6E8E4A\n.word 0x00000000\n.word 0x833A6425\n.word 0x83588370\n.word 0x83628377\n.word 0x8FCC825F\n.word 0x8DAB8291\n.word 0x8FDD829E\n.word 0x82ED8249\n.word 0x00000AE8\n.word 0xAB82918F\n.word 0xDE829E8D\n.word 0x5F93B892\n.word 0x9490CC82\n.word 0x000A6425\n.word 0x00000000\n.word 0x923A6425\n.word 0x825F93B8\n.word 0x82918FCC\n.word 0x829E8DAB\n.word 0x8E4A8ADD\n.word 0x00000A6E\n.word 0x923A6425\n.word 0x825F93B8\n.word 0x82918FCC\n.word 0x829E8DAB\n.word 0x82498FDD\n.word 0x0AE882ED\n.word 0x00000000\n.word 0x00000000\n.word 0x833A6425\n.word 0x83898370\n.word 0x835B8181\n.word 0x8FCC825E\n.word 0x8DAB8291\n.word 0x8ADD829E\n.word 0x0A6E8E4A\n.word 0x00000000\n.word 0x833A6425\n.word 0x83898370\n.word 0x835B8181\n.word 0x8FCC825E\n.word 0x8DAB8291\n.word 0x8FDD829E\n.word 0x82ED8249\n.word 0x00000AE8\n.word 0xAB82918F\n.word 0xF1829E8D\n.word 0x7083BE82\n.word 0x81838983\n.word 0x5E835B81\n.word 0x9490CC82\n.word 0x000A6425\n.word 0x00000000\n.word 0xAB82918F\n.word 0xDE829E8D\n.word 0x66837083\n.word 0x93834283\n.word 0xCC824F83\n.word 0x64259490\n.word 0x0000000A\n.word 0x00000000\n.word 0x833A6425\n.word 0x83668370\n.word 0x83938342\n.word 0x8FCC824F\n.word 0x8DAB8291\n.word 0x8ADD829E\n.word 0x0A6E8E4A\n.word 0x00000000\n.word 0x833A6425\n.word 0x83668370\n.word 0x83938342\n.word 0x8FCC824F\n.word 0x8DAB8291\n.word 0x8FDD829E\n.word 0x82ED8249\n.word 0x00000AE8\n.word 0x833A6425\n.word 0x83408374\n.word 0x828B8343\n.word 0x834E83CC\n.word 0x835B818D\n.word 0x8EC98259\n.word 0x217394B8\n.word 0x21212121\n.word 0x21212121\n.word 0x21212121\n.word 0x00000A21\n.word 0x00000000\n.text\n");
// SOURCE DATA: .data .rodata
