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
#include "Form/DebugWindow.h"
#include "System/Graphic/WorldEnv.h"
#include "System/Coord.h"
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditSimulateLineCall__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditSimulateSpeedLineCall__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditSimulateSpeedCamLineCall__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditSimulateSpeedCamLineLookCall__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditEtcParamAllCall__FP12CDebugWindowi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditPosCamGround__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditPosCamReset__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditPosCamAllCall__FP12CDebugWindowi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditLineCamPosCall__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditLineAddPoaCall__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditLineAddRotCall__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditLineAddSpdCall__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathLineNameSaveCall__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathLineSaveCall__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditNum__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditAdd__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditDel__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditPathNum__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditPathAddLineCall__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditPathDel__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditDispCall__FPciPCv);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathEditAllCall__FP12CDebugWindowi);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathLineNewCall__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", debugWinPathLineNameLoadCall__FP12CDebugWindowii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", __14CDebugPathDataP5CTaski);
/* Not attempted yet: the key function; defining it emits every in-class inline (the weak tail, still asm).
   Draft (needs Game/CardKind.h + Game/Player/Player.h after PathDataCamera.h, ~CPDFrameTime() {} inline):
   delete m_pPath; m_pPath = NULL; delete m_pFrame; m_pFrame = NULL; m_pInstance = NULL;
   CDebugWindow::enableTopDebugWindow(TRUE); CPlayer::enableDbgPadControl(TRUE); */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", _$_14CDebugPathData);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", create__14CDebugPathDataP5CTaski);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", loadPath__14CDebugPathDataPCc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", __BackUpFile__FPCc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Debug/DebugPathData", savePath__14CDebugPathDataPCc);
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
