#define EFFECT_H_PATH "c:/KHCOM/MainApp/System/Graphic/Effect/effect.h" /* MATCH: this unit names effect.h so (header default "System/Graphic/Effect/effect.h") */
#include "System/Graphic/Effect/effect.h"
#include "System/TaskMan.h"
#include "System/Task.h"
#include "System/Coord.h"
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Graphic/Gprim.h"
#include "Game/Entity.h"
#include "System/Graphic/Model.h"
#include "System/Graphic/ModelMan.h"
#include "System/Graphic/Camera.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "System/FifoFpl.h"
#include "System/HeapMan.h"
#include "System/Debug.h"
#include "System/Graphic/WorldEnv.h"
#include "Game/Debug/DebugWork.h"
#include "System/Spad.h"
#include "include_asm.h"
#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)


#include "Game/SceneEnv.h"
#line 36

#include "System/RsrcDB.h"






namespace VRAM {
extern INT _FRAME0_START;
extern INT _FRAME1_START;
extern INT _ZBUF_START;
}
extern INT SCREEN_HEIGHT;
extern INT DISP_HEIGHT;

extern "C" {
void pppInitPartRender(void* work_memory, int work_memory_size, void* packet_memory, int packet_memory_size,
                       int disp_width, int disp_height, int draw_width, int draw_height, int z_frame_base,
                       int draw_frame_base, int disp_frame_base, u_long gs_frame, u_long gs_zbuf, u_long gs_xyoffset,
                       u_long gs_scissor, u_long gs_test, u_long gs_dthe, u_long gs_color_clamp);
void pppInitTexCommon(char* pchDpdData);
void pppSetFogParam(float near, float far);
void pppSetUp(void* mng);
void pppInitDraw(BOOL bDraw);
void pppPartDrawReady(BOOL bDraw);
int pppPartDraw(void* mng, BOOL bDraw);
void pppPartDrawEnd();
int sprintf(char* buf, const char* fmt, ...);
int pppIsDrawFirst(void* mng);
void pppSetPartMatrix(const FMATRIX& cm, const FMATRIX& sm, const FVECTOR* eye, const FVECTOR* at);
void pppSetFrameTime(float frame_time);
void pppPartCalcReady();
int pppPartCalc(void* mng, int view_clip);
void pppPartCalcEnd();
void pppStopParticle(void* mng, int b);
FVECTOR* pppGetBbox(void* mng);
int pppIsDrawLast(void* mng);
void qsort(void* base, size_t num, size_t size, int (*cmp)(const void*, const void*));
void pppSetFogColor(u_char r, u_char g, u_char b);
void pppSetFogClamp(u_char min, u_char max);
}

class CEffectSLCManager : public ISLCManagerT<CEffect>, private CFifoFpl {
public:
    CEffectSLCManager() { CFifoFpl::initialize(sizeof(SLContainerT<CEffect>), 0x400, NULL); }
    virtual SLContainerT<CEffect>* createContainer() {
        SLContainerT<CEffect>* pContainer = (SLContainerT<CEffect>*)allocBlock();
        if (!pContainer) {
            TRACE_COL(_TTY_RED, "CEffect \x97p\x83R\x83\x93\x83" "e\x83i\x82\xaa MAX \x82\xc9\x93\x9e\x92" "B\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
        }
        return pContainer;
    }
    virtual void releaseContainer(SLContainerT<CEffect>& inContainer) { releaseBlock(&inContainer); }
    virtual SLContainerT<CEffect>* getContainer(HANDLE hID) { return (SLContainerT<CEffect>*)getValidActiveBlock(hID); }
};

typedef CSLCListT<CEffect> CEffectList;

class CEffectTask : public CTask {
private:
    C3DLineStrip* m_pDebugLine;

public:
    CEffectTask(CTask* p);
    virtual ~CEffectTask();
    virtual void main();
    virtual void disp();
    C3DLineStrip* getDebugPrim() { return m_pDebugLine; }
};


static inline void _unitMatrixVu0(FMATRIX& m) {
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
}

typedef struct {
    u_int DIR : 1;
    u_int p0 : 1;
    u_int MOD : 2;
    u_int ASP : 2;
    u_int TTE : 1;
    u_int TIE : 1;
    u_int STR : 1;
    u_int p1 : 7;
    u_int TAG : 16;
} tD_CHCR;
typedef struct {
    tD_CHCR chcr;
} sceDmaChan;
#include "System/Graphic/PackMan.h"












extern "C" {
void sceDmaSend(sceDmaChan* d, void* tag);
sceDmaChan* sceDmaGetChan(int id);
int sceGsSyncPath(int mode, u_short timeout);
}














void MulVector(FVECTOR& p0, const FVECTOR& p1, const FVECTOR& p2);
void MulMatrix(FMATRIX& res, const FMATRIX& m0, const FMATRIX& m1);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void DivVector(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void ApplyMatrix33(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);

struct EFFMAN_ZSORT_BUF {
    union {
        u_long128 i128;
        struct {
            CEffect* pEff;
            void* pPkt;
            FIX4 nZ;
            u_long128* pMProg;
        };
        struct {
            INT ix;
            INT iy;
            INT iz;
            FLOAT fw;
        };
    };
};

namespace {
CEffectSLCManager* s_pSLCManager = __null;
CEffectList* s_pEffectList = __null;
CEffect* s_pExceptEffect = __null;
void* s_pWorkMemory = __null;
void* s_pPacketMemory = __null;
INT s_nPacketMemSize = 0;
UINT s_iStatus = 0;
FLOAT s_fFrameRate = 0.0f;
FMATRIX* s_pArgWorldView = __null;
FMATRIX* s_pArgViewScreen = __null;
CCamera* s_pArgCamera = __null;
CEffectTask* s_pTask = __null;
static char* TEXCOMMON_FNAME = "texcommon";
static const FVECTOR s_vEffKhComScale = {0.01f, -0.01f, -0.01f, 1.0f};
FMATRIX s_mWorldView;
FMATRIX s_mWorldScreen;
static INT s_nDrawNum;
static INT s_nEffNum;
static EFFMAN_ZSORT_BUF* s_pEffList;
static BOOL s_bDrawALL;
static ObjType::eObjBit s_eDrawBit;
} // namespace

CEffectTask::CEffectTask(CTask* p) : CTask(p, 0x4000) {
    declareSysTask(0x14);
    s_pTask = this;
#line 371
    m_pDebugLine = new (__FILE__ "(371)") C3DLineStrip;
    ASSERT(m_pDebugLine);
    m_pDebugLine->setLayerFG(0);
}
CEffectTask::~CEffectTask() {
    CEffectTask*& rTask = s_pTask;
    if (rTask == this) {
        rTask = __null;
    }
    if (m_pDebugLine) delete m_pDebugLine;
    m_pDebugLine = __null;
}
void CEffectTask::main() {
    if (CSceneEnv::getStopTypeBit() != ObjType::OBJ_BIT_ALL || CEffectMan::isLastDraw()) {
        CEffectMan::play(getFrameRateF());
    }
}
void CEffectTask::disp() {
    m_pDebugLine->disp();
}
void CEffectMan::_effInitialize(int work_memory_size, int packet_memory_size) {
    SBYTE* pAddr;

    s_nPacketMemSize = packet_memory_size;
#line 438
    s_pWorkMemory = com_malloc_d(work_memory_size, __FILE__ "(438)", (UINT)-1);
    ASSERT(s_pWorkMemory);
    s_pPacketMemory = com_malloc_d(packet_memory_size, __FILE__ "(440)", (UINT)-1);
    ASSERT(s_pPacketMemory);
    pppInitPartRender(s_pWorkMemory, work_memory_size, (void*)(((UINT)s_pPacketMemory + 63) & ~63),
                      s_nPacketMemSize - 64, 512, DISP_HEIGHT, 512, SCREEN_HEIGHT, VRAM::_ZBUF_START / 32,
                      VRAM::_FRAME0_START / 32, VRAM::_FRAME1_START / 32,
                      (u_long)(VRAM::_FRAME0_START / 32) | ((u_long)8 << 16),
                      (u_long)(VRAM::_ZBUF_START / 32) | ((u_long)0x31 << 24),
                      (u_long)0x7000 | ((u_long)((2048 - (SCREEN_HEIGHT >> 1)) << 4) << 32),
                      ((u_long)511 << 16) | ((u_long)(SCREEN_HEIGHT - 1) << 48), 0x5000D, 0, 1);
    s_fFrameRate = 0.0f;
    s_pArgWorldView = __null;
    s_pArgViewScreen = __null;
    s_pArgCamera = __null;
    s_iStatus = 0;
    pAddr = (SBYTE*)CRsrcDB::getRsrcDPD(TEXCOMMON_FNAME);
#line 467
    ASSERT(pAddr);
    pppInitTexCommon((char*)pAddr);
    s_iStatus |= 1;
}
void CEffectMan::_effFinalize() {
    killAll((ObjType::eObjBit)0xFFFFFFFFU, 0.0f);
    destroyer();
    destroyer();
    TRACE("CEffectMan::_effFinalize() : end effect all instance.\n");
    if (s_pWorkMemory) com_free(s_pWorkMemory);
    s_pWorkMemory = __null;
    if (s_pPacketMemory) com_free(s_pPacketMemory);
    s_pPacketMemory = __null;
    s_iStatus = 0;
    s_fFrameRate = 0.0f;
    s_pArgWorldView = __null;
    s_pArgViewScreen = __null;
    s_pArgCamera = __null;
}
HANDLE CEffectMan::_setEffect(const EFFECT_DATA& effect_data, UINT no, ObjType::eObjType eType, INT nNum,
                              const CCoord* pParent, const FMATRIX* pMat, const FVECTOR* pvPos, const FVECTOR* pvRot,
                              FLOAT fWait, const EFFECT_SE* pSeData) {
    HANDLE hEff;
    CEffect* pEff = __null;

    if (s_pSLCManager) {
#line 515
        pEff = new (__FILE__ "(515)") CEffect;
        if (!pEff) {
            hEff = 0;
            goto end;
        }
        if (pEff->start(effect_data, no, eType, nNum, fWait)) {
            _setCoord(pEff, 0, pParent, pMat, pvPos, pvRot);
            pEff->_playSE(pSeData, no);
            hEff = add(*pEff);
            if (hEff != -1) goto end;
        }
    }
    if (pEff) delete pEff;
    hEff = 0;
end:
    return hEff;
}
void CEffectMan::_setCoord(CEffect* pEff, INT nCNum, const CCoord* pParent, const FMATRIX* pMat, const FVECTOR* pvPos,
                           const FVECTOR* pvRot) {
    CCoord& crd = pEff->Crd(nCNum);
    FMATRIX* pLocal;

    if (pParent) pEff->setAttachFlag(TRUE);
    crd.setParent((CCoord*)pParent);
    pLocal = &crd.m_Local;
    if (pMat) {
        *pLocal = *pMat;
    } else {
        _unitMatrixVu0(*pLocal);
        if (pvRot) {
            RotMatrixX(*pLocal, *pLocal, pvRot->x);
            RotMatrixY(*pLocal, *pLocal, pvRot->y);
            RotMatrixZ(*pLocal, *pLocal, pvRot->z);
        }
        if (pvPos) TransMatrix(*pLocal, *pLocal, *pvPos);
    }
    crd.touch();
    crd.fix();
    crd.touch();
}
HANDLE CEffectMan::setEffect(const CHAR* lpszInName, UINT nNo, ObjType::eObjType eType, const FVECTOR* pvPos, const FVECTOR* pvRot, FLOAT fWait) {
    EFFECT_DATA* pEffectData;
    HANDLE hEff;
    CEffect* pEff;

    pEffectData = CRsrcDB::getRsrcDPX(lpszInName);
    if (!pEffectData) {
        TRACE("CEffectMan::setEffect \x83G\x83t\x83" "F\x83N\x83g %s %d \x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\n", lpszInName, nNo);
        return 0;
    }
    hEff = _setEffect(*pEffectData, nNo, eType, 1, __null, __null, pvPos, pvRot, fWait, CRsrcDB::getRsrcESE(lpszInName));
    pEff = getEffect(hEff);
    if (pEff) pEff->_setName(lpszInName);
    return hEff;
}
HANDLE CEffectMan::setEffect(const EFFECT_DATA& pEffectData, UINT nNo, ObjType::eObjType eType, const FVECTOR* pvPos, const FVECTOR* pvRot, FLOAT fWait,
                             const EFFECT_SE* pSeData) {
    return _setEffect(pEffectData, nNo, eType, 1, __null, __null, pvPos, pvRot, fWait, pSeData);
}
HANDLE CEffectMan::setEffect(const CHAR* lpszInName, UINT nNo, ObjType::eObjType eType, const FMATRIX* pMat, FLOAT fWait) {
    EFFECT_DATA* pEffectData;
    HANDLE hEff;
    CEffect* pEff;

    pEffectData = CRsrcDB::getRsrcDPX(lpszInName);
    if (!pEffectData) {
        TRACE("CEffectMan::setEffect \x83G\x83t\x83" "F\x83N\x83g %s %d \x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\n", lpszInName, nNo);
        return 0;
    }
    hEff = _setEffect(*pEffectData, nNo, eType, 1, __null, pMat, __null, __null, fWait, CRsrcDB::getRsrcESE(lpszInName));
    pEff = getEffect(hEff);
    if (pEff) pEff->_setName(lpszInName);
    return hEff;
}
HANDLE CEffectMan::setEffect(const EFFECT_DATA& pEffectData, UINT nNo, ObjType::eObjType eType, const FMATRIX* pMat, FLOAT fWait,
                             const EFFECT_SE* pSeData) {
    return _setEffect(pEffectData, nNo, eType, 1, __null, pMat, __null, __null, fWait, pSeData);
}
HANDLE CEffectMan::attachEffect(const CHAR* lpszInName, UINT nNo, ObjType::eObjType eType, const CCoord* pParent, const FVECTOR* pvPos, const FVECTOR* pvRot, FLOAT fWait) {
    EFFECT_DATA* pEffectData;
    HANDLE hEff;
    CEffect* pEff;

    pEffectData = CRsrcDB::getRsrcDPX(lpszInName);
    if (!pEffectData) {
        TRACE("CEffectMan::attachEffect \x83G\x83t\x83" "F\x83N\x83g %s %d \x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\n", lpszInName, nNo);
        return 0;
    }
    hEff = _setEffect(*pEffectData, nNo, eType, 1, pParent, __null, pvPos, pvRot, fWait, CRsrcDB::getRsrcESE(lpszInName));
    pEff = getEffect(hEff);
    if (pEff) pEff->_setName(lpszInName);
    return hEff;
}
HANDLE CEffectMan::attachEffect(const EFFECT_DATA& pEffectData, UINT nNo, ObjType::eObjType eType, const CCoord* pParent, const FVECTOR* pvPos, const FVECTOR* pvRot, FLOAT fWait,
                             const EFFECT_SE* pSeData) {
    return _setEffect(pEffectData, nNo, eType, 1, pParent, __null, pvPos, pvRot, fWait, pSeData);
}
HANDLE CEffectMan::attachEffect(const CHAR* lpszInName, UINT nNo, ObjType::eObjType eType, const CCoord* pParent, const FMATRIX* pMat, FLOAT fWait) {
    EFFECT_DATA* pEffectData;
    HANDLE hEff;
    CEffect* pEff;

    pEffectData = CRsrcDB::getRsrcDPX(lpszInName);
    if (!pEffectData) {
        TRACE("CEffectMan::attachEffect \x83G\x83t\x83" "F\x83N\x83g %s %d \x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\n", lpszInName, nNo);
        return 0;
    }
    hEff = _setEffect(*pEffectData, nNo, eType, 1, pParent, pMat, __null, __null, fWait, CRsrcDB::getRsrcESE(lpszInName));
    pEff = getEffect(hEff);
    if (pEff) pEff->_setName(lpszInName);
    return hEff;
}
HANDLE CEffectMan::attachEffect(const EFFECT_DATA& pEffectData, UINT nNo, ObjType::eObjType eType, const CCoord* pParent, const FMATRIX* pMat, FLOAT fWait,
                             const EFFECT_SE* pSeData) {
    return _setEffect(pEffectData, nNo, eType, 1, pParent, pMat, __null, __null, fWait, pSeData);
}
HANDLE CEffectMan::attachEffect2(const CHAR* lpszInName, UINT nNo, ObjType::eObjType eType, const CCoord* pParent, const CCoord* pParent1, const FVECTOR* pvPos, const FVECTOR* pvPos1, FLOAT fWait) {
    EFFECT_DATA* pEffectData;
    HANDLE hEff;
    CEffect* pEff;

    pEffectData = CRsrcDB::getRsrcDPX(lpszInName);
    if (!pEffectData) {
        TRACE("CEffectMan::attachEffect2 \x83G\x83t\x83" "F\x83N\x83g %s %d \x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\n", lpszInName, nNo);
        return 0;
    }
    hEff = _setEffect(*pEffectData, nNo, eType, 2, pParent, __null, pvPos, __null, fWait, CRsrcDB::getRsrcESE(lpszInName));
    if (hEff) {
        _setCoord(getEffect(hEff), 1, pParent1, __null, pvPos1, __null);
    }
    pEff = getEffect(hEff);
    if (pEff) pEff->_setName(lpszInName);
    return hEff;
}
HANDLE CEffectMan::attachEffect2(const EFFECT_DATA& pEffectData, UINT nNo, ObjType::eObjType eType, const CCoord* pParent, const CCoord* pParent1, const FVECTOR* pvPos, const FVECTOR* pvPos1, FLOAT fWait,
                             const EFFECT_SE* pSeData) {
    HANDLE hEff;

    hEff = _setEffect(pEffectData, nNo, eType, 2, pParent, __null, pvPos, __null, fWait, pSeData);
    if (hEff) {
        _setCoord(getEffect(hEff), 1, pParent1, __null, pvPos1, __null);
    }
    return hEff;
}
HANDLE CEffectMan::attachEffect2(const CHAR* lpszInName, UINT nNo, ObjType::eObjType eType, const CCoord* pParent, const CCoord* pParent1, const FMATRIX* pMat, const FMATRIX* pMat1, FLOAT fWait) {
    EFFECT_DATA* pEffectData;
    HANDLE hEff;
    CEffect* pEff;

    pEffectData = CRsrcDB::getRsrcDPX(lpszInName);
    if (!pEffectData) {
        TRACE("CEffectMan::attachEffect2 \x83G\x83t\x83" "F\x83N\x83g %s %d \x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\n", lpszInName, nNo);
        return 0;
    }
    hEff = _setEffect(*pEffectData, nNo, eType, 2, pParent, pMat, __null, __null, fWait, CRsrcDB::getRsrcESE(lpszInName));
    if (hEff) {
        _setCoord(getEffect(hEff), 1, pParent1, pMat1, __null, __null);
    }
    pEff = getEffect(hEff);
    if (pEff) pEff->_setName(lpszInName);
    return hEff;
}
HANDLE CEffectMan::attachEffect2(const EFFECT_DATA& pEffectData, UINT nNo, ObjType::eObjType eType, const CCoord* pParent, const CCoord* pParent1, const FMATRIX* pMat, const FMATRIX* pMat1, FLOAT fWait,
                             const EFFECT_SE* pSeData) {
    HANDLE hEff;

    hEff = _setEffect(pEffectData, nNo, eType, 2, pParent, pMat, __null, __null, fWait, pSeData);
    if (hEff) {
        _setCoord(getEffect(hEff), 1, pParent1, pMat1, __null, __null);
    }
    return hEff;
}
HANDLE CEffectMan::attachEffect3(const CHAR* lpszInName, UINT nNo, ObjType::eObjType eType, const CCoord* pParent, const CCoord* pParent1, const CCoord* pParent2, const FVECTOR* pvPos, const FVECTOR* pvPos1, const FVECTOR* pvPos2, FLOAT fWait) {
    EFFECT_DATA* pEffectData;
    HANDLE hEff;
    CEffect* pEff;

    pEffectData = CRsrcDB::getRsrcDPX(lpszInName);
    if (!pEffectData) {
        TRACE("CEffectMan::attachEffect3 \x83G\x83t\x83" "F\x83N\x83g %s %d \x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\n", lpszInName, nNo);
        return 0;
    }
    hEff = _setEffect(*pEffectData, nNo, eType, 3, pParent, __null, pvPos, __null, fWait, CRsrcDB::getRsrcESE(lpszInName));
    if (hEff) {
        _setCoord(getEffect(hEff), 1, pParent1, __null, pvPos1, __null);
        _setCoord(getEffect(hEff), 2, pParent2, __null, pvPos2, __null);
    }
    pEff = getEffect(hEff);
    if (pEff) pEff->_setName(lpszInName);
    return hEff;
}
HANDLE CEffectMan::attachEffect3(const EFFECT_DATA& pEffectData, UINT nNo, ObjType::eObjType eType, const CCoord* pParent, const CCoord* pParent1, const CCoord* pParent2, const FVECTOR* pvPos, const FVECTOR* pvPos1, const FVECTOR* pvPos2, FLOAT fWait,
                             const EFFECT_SE* pSeData) {
    HANDLE hEff;

    hEff = _setEffect(pEffectData, nNo, eType, 3, pParent, __null, pvPos, __null, fWait, pSeData);
    if (hEff) {
        _setCoord(getEffect(hEff), 1, pParent1, __null, pvPos1, __null);
        _setCoord(getEffect(hEff), 2, pParent2, __null, pvPos2, __null);
    }
    return hEff;
}
HANDLE CEffectMan::attachEffect3(const CHAR* lpszInName, UINT nNo, ObjType::eObjType eType, const CCoord* pParent, const CCoord* pParent1, const CCoord* pParent2, const FMATRIX* pMat, const FMATRIX* pMat1, const FMATRIX* pMat2, FLOAT fWait) {
    EFFECT_DATA* pEffectData;
    HANDLE hEff;
    CEffect* pEff;

    pEffectData = CRsrcDB::getRsrcDPX(lpszInName);
    if (!pEffectData) {
        TRACE("CEffectMan::attachEffect3 \x83G\x83t\x83" "F\x83N\x83g %s %d \x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\n", lpszInName, nNo);
        return 0;
    }
    hEff = _setEffect(*pEffectData, nNo, eType, 3, pParent, pMat, __null, __null, fWait, CRsrcDB::getRsrcESE(lpszInName));
    if (hEff) {
        _setCoord(getEffect(hEff), 1, pParent1, pMat1, __null, __null);
        _setCoord(getEffect(hEff), 2, pParent2, pMat2, __null, __null);
    }
    pEff = getEffect(hEff);
    if (pEff) pEff->_setName(lpszInName);
    return hEff;
}
HANDLE CEffectMan::attachEffect3(const EFFECT_DATA& pEffectData, UINT nNo, ObjType::eObjType eType, const CCoord* pParent, const CCoord* pParent1, const CCoord* pParent2, const FMATRIX* pMat, const FMATRIX* pMat1, const FMATRIX* pMat2, FLOAT fWait,
                             const EFFECT_SE* pSeData) {
    HANDLE hEff;

    hEff = _setEffect(pEffectData, nNo, eType, 3, pParent, pMat, __null, __null, fWait, pSeData);
    if (hEff) {
        _setCoord(getEffect(hEff), 1, pParent1, pMat1, __null, __null);
        _setCoord(getEffect(hEff), 2, pParent2, pMat2, __null, __null);
    }
    return hEff;
}
void CEffectMan::killAll(ObjType::eObjBit eBit, FLOAT fFrame) {
    SLContainerT<CEffect>* pContainer;

    if (!s_pSLCManager) return;
#line 859
    ASSERT(( s_pEffectList));
    for (pContainer = s_pEffectList->getHead()->getNext(); pContainer; pContainer = pContainer->getNext()) {
        CEffect* effect = pContainer->getValue();
        if (effect && (eBit == (ObjType::eObjBit)0xFFFFFFFFU || ((1 << effect->getType()) & eBit))) {
            effect->fadeout(fFrame);
        }
    }
}
void CEffectMan::killAll(const CHAR* pszName, FLOAT fFrame) {
    INT nNum;
    SLContainerT<CEffect>* pContainer;

    if (!s_pSLCManager || !pszName) return;
    nNum = 0;
#line 885
    ASSERT(( s_pEffectList));
    for (pContainer = s_pEffectList->getHead()->getNext(); pContainer; pContainer = pContainer->getNext()) {
        CEffect* pEff = pContainer->getValue();
        if (pEff && !pEff->_strcmpName(pszName)) {
            pEff->fadeout(fFrame);
            nNum++;
        }
    }
    TRACE("CEffectMan::killAll(\x96\xbc\x91O\x94\xc5) : %s %d\n", pszName, nNum);
}
INT CEffectMan::getEffectNameNum(const CHAR* pszName) {
    INT nNum;
    SLContainerT<CEffect>* pContainer;

    if (!s_pSLCManager || !pszName) return 0;
    nNum = 0;
#line 915
    ASSERT(( s_pEffectList));
    for (pContainer = s_pEffectList->getHead()->getNext(); pContainer; pContainer = pContainer->getNext()) {
        if (pContainer->getValue() && !pContainer->getValue()->_strcmpName(pszName)) {
            nNum++;
        }
    }
    TRACE("CEffectMan::getEffectNameNum() : %s %d\n", pszName, nNum);
    return nNum;
}
INT CEffectMan::getEffectNameHandles(HANDLE* pOutHandle, const CHAR* pszName, INT nSize) {
    INT nNum;
    SLContainerT<CEffect>* pContainer;

    if (!s_pSLCManager || !pszName) return 0;
    if (nSize <= 0) nSize = 0x10000;
    nNum = 0;
#line 948
    ASSERT(( s_pEffectList));
    for (pContainer = s_pEffectList->getHead()->getNext(); pContainer; pContainer = pContainer->getNext()) {
        CEffect* pEff = pContainer->getValue();
        if (pEff && !pEff->_strcmpName(pszName)) {
            pOutHandle[nNum++] = pEff->_getHandle();
            if (nNum >= nSize) break;
        }
    }
    TRACE("CEffectMan::getEffectNameHandles() : %s %d\n", pszName, nNum);
    return nNum;
}
void CEffectMan::setFogRange(float near, float far) { pppSetFogParam(near, far); }
void CEffectMan::setFogClamp(u_char min, u_char max) { pppSetFogClamp(min, max); }
void CEffectMan::setFogColor(u_char r, u_char g, u_char b) { pppSetFogColor(r, g, b); }
static inline void _loadMatrixVf4(const FMATRIX& in_m) {
    asm volatile("lqc2 $vf4, 0x0(%0)\n"
                 "lqc2 $vf5, 0x10(%0)\n"
                 "lqc2 $vf6, 0x20(%0)\n"
                 "lqc2 $vf7, 0x30(%0)\n"
                 :
                 : "r"(&in_m));
}

static inline u_long128 _rotTransPersVf4(const FVECTOR& v0) {
    u_long128 ret;
    asm volatile("lqc2 $vf8, 0x0(%1)\n"
                 "vmulax.xyzw $ACC, $vf4, $vf8x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf8y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
                 "vmaddw.xyzw $vf8, $vf7, $vf8w\n"
                 "vdiv $Q, $vf0w, $vf8w\n"
                 "vmax.w $vf8, $vf0, $vf0\n"
                 "vwaitq\n"
                 "vmulq.xyzw $vf8, $vf8, $Q\n"
                 "vftoi4.xyz $vf8, $vf8\n"
                 "qmfc2.ni %0, $vf8\n"
                 : "=r"(ret)
                 : "r"(&v0));
    return ret;
}



static inline FLOAT _lengthVf4(const FVECTOR& v0) {
    INT iTmp;
    register FLOAT ret asm("$f0");
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vaddy.x $vf6, $vf0, $vf4y\n"
                 "vaddz.x $vf7, $vf0, $vf4z\n"
                 "vmulax.x $ACC, $vf4, $vf4x\n"
                 "vmadday.x $ACC, $vf6, $vf4y\n"
                 "vmaddz.x $vf5, $vf7, $vf4z\n"
                 "qmfc2.ni %0, $vf5\n"
                 : "+r"(iTmp) /* MATCH: read before written: live from entry, so it crosses calls (s3 in _updateCoord) */
                 : "r"(&v0));
    asm volatile("mtc1 %1, %0\n"
                 "nop\n"
                 "nop\n"
                 "sqrt.s %0, %0\n"
                 : "=f"(ret)
                 : "r"(iTmp));
    return ret;
}

static int EffMan_QSortComp(const void* ax, const void* ay) {
    return ((EFFMAN_ZSORT_BUF*)ax)->nZ - ((EFFMAN_ZSORT_BUF*)ay)->nZ;
}
INT CEffectMan::_sortEffect(EFFMAN_ZSORT_BUF* elist, const FMATRIX& mWS, INT nEffNum, INT nSemiNum) {
    INT i = 0;
    SLContainerT<CEffect>* pContainer;

    if (nEffNum > 0) {
        _loadMatrixVf4(mWS);
#line 1130
        ASSERT(( s_pEffectList));
        for (pContainer = s_pEffectList->getHead()->getNext(); pContainer; pContainer = pContainer->getNext()) {
            CEffect* effect = pContainer->getValue();
            if (!effect) continue;
            if (effect->isAlive()) {
                int first = pppIsDrawFirst(effect->m_pPartMng);
                if (first) {
                    elist[i].nZ = first;
                } else {
                    int last = pppIsDrawLast(effect->m_pPartMng);
                    if (last) {
                        elist[i].nZ = 0xFFFFFF0 - last;
                    } else {
                        FIX4 nZ;
                        CEntity* pEnt;
                        if (effect->m_hEnt && (pEnt = effect->getZSortEntity()) != __null) {
                            FIX4 nZOfs = effect->getZSortEntityOffset();
                            CModel* pModel = pEnt->getModel();
                            INT nModel = pEnt->getModelCount();
                            FIX4 nModelZ;
                            if (nZOfs >= 0) {
                                nZ = 0x10;
                                for (INT j = 0; j < nModel; j++) {
                                    nModelZ = pModel[j].getOldSemiZ();
                                    if (nZ < nModelZ) nZ = nModelZ;
                                }
                            } else {
                                nZ = 0xFFFF280;
                                for (INT j = 0; j < nModel; j++) {
                                    nModelZ = pModel[j].getOldSemiZ();
                                    if (nModelZ < nZ) nZ = nModelZ;
                                }
                            }
                            nZ += nZOfs;
                            elist[i].fw = 0.0f;
                        } else {
                            u_long128 outV = _rotTransPersVf4(effect->_pos());
                            elist[i].i128 = outV;
                            nZ = elist[i].iz;
                        }
                        nZ += effect->getZOffset();
                        if (nZ <= 0) {
                            if (elist[i].fw < 0.0f) {
                                nZ = 0xFFFF280;
                            } else {
                                nZ = 0x10;
                            }
                        }
                        elist[i].nZ = nZ;
                    }
                }
            } else {
                elist[i].nZ = 0;
            }
            elist[i].pEff = effect;
            elist[i].pPkt = __null;
            elist[i].pMProg = __null;
            i++;
        }
    }
    if (nSemiNum > 0) {
        SEMI_PACKET_LIST* pSemi = CModelMan::getSemiPktList();
        if (pSemi) {
            for (INT j = 0; j < nSemiNum; j++) {
                elist[i].pEff = __null;
                elist[i].pPkt = pSemi[j].pPkt;
                elist[i].nZ = pSemi[j].nZ;
                elist[i].pMProg = pSemi[j].pMProg;
                if (elist[i].nZ < 0) elist[i].nZ = 0xFFFF280;
                i++;
            }
        }
    }
    if (i >= 2) qsort(elist, i, sizeof(EFFMAN_ZSORT_BUF), EffMan_QSortComp);
    return i;
}
BOOL CEffectMan::_updateCoord(CEffect* effect) {
    CCoord* pCrd;
    CCoord* pParent;
    INT i;
    INT nNum = effect->getCoordNum();

    if (effect->isAttachFlag()) {
        pCrd = &effect->Crd(0);
        for (i = 0; i < nNum; i++) {
            if (pCrd[i].getParent()) break;
        }
        if (i >= nNum) {
            char* pszEffName = (char*)"\x83G\x83t\x83" "F\x83N\x83g\x96\xbc\x95s\x96\xbe";
            if (effect->m_pEffName) pszEffName = effect->m_pEffName;
            TRACE("CEffectMan::_updateCoord \x83" "A\x83^\x83" "b\x83`\x90\xe6\x82\xaa\x82\xa2\x82\xc8\x82\xad\x82\xc8\x82\xc1\x82\xc4\x82\xa2\x82\xdc\x82\xb7\x81" "B\x8e\xa9\x8e" "E\x82\xb5\x82\xdc\x82\xb7 eff:%08x crd:%08x (%s:%d)\n",
                  effect, &effect->Crd(0), pszEffName, effect->m_nEffNo);
            return FALSE;
        }
    }
    if (nNum >= 2) {
        effect->setReferenceCoordPos(2);
        if (nNum >= 3) {
            pCrd = &effect->Crd(2);
        } else {
            pCrd = &effect->Crd(0);
        }
    } else {
        pCrd = &effect->Crd(0);
    }
    {
        FMATRIX* pMat = (effect->m_iFlag & 4) ? &pCrd->m_Local : &pCrd->m_World;
        if (effect->m_iFlag & 0x80) {
            FMATRIX mMat = *pMat;
            float fDist[3];
            INT i = 0; // MATCH: the ref's second, block-local i (the first loop's i gets another register)
            __asm__ volatile(""); // MATCH: keeps the hoisted lui after the mMat copy (it fills the `b` slot)
            goto test;
            do {} while (0); // MATCH: the loop note aligns the next label (the original's nop after the `b`)
        inc:
            i++;
        test:
            if (i < 3) {
                fDist[i] = _lengthVf4(mMat.v[i]);
                if (!(fDist[i] < 0.0001f)) goto inc;
                goto skip;
            }
            DivVector(mMat.v[0], mMat.v[0], fDist[0]);
            DivVector(mMat.v[1], mMat.v[1], fDist[1]);
            DivVector(mMat.v[2], mMat.v[2], fDist[2]);
            effect->_matrix() = mMat;
        } else {
            effect->_matrix() = *pMat;
        }
    skip:
        pParent = pCrd->getParent();
        if (pParent && (effect->m_iFlag & 8)) {
            FVECTOR vPos;
            ApplyMatrix33(vPos, *pMat, pCrd->m_Local.v[3]);
            AddVectorXYZ(effect->_pos(), pParent->getWorldPos(), vPos);
        } else {
            effect->_pos() = pCrd->getWorldPos();
        }
    }
    MulVector(effect->_scale(), effect->m_vScale, s_vEffKhComScale);
    effect->_updateSE();
    effect->Crd(0).touch();
    return TRUE;
}
void CEffectMan::_updateColor(CEffect* effect) {
    FVECTOR* pSceneColor = CSceneEnv::getTypeColor(effect->getType());

    if (pSceneColor) {
        MulVector(effect->_color(), effect->m_vColor, *pSceneColor);
    } else {
        effect->_color() = effect->m_vColor;
    }
    effect->calcLight(effect->_color(), effect->_color());
    if (effect->m_hEnt) {
        CEntity* pEnt = effect->getZSortEntity();
        if (pEnt && effect->isEntityAlpha()) {
            CModel* pModel = pEnt->getModel();
            if (pModel) effect->_color().w *= pModel->getAlphaScale();
        }
    }
}
void CEffectMan::_calc(FLOAT fFrameRate, const FMATRIX& mWV, const FMATRIX& mVS, CCamera* pCam, BOOL frustumClip) {
    FMATRIX mCam;
    FVECTOR* pLook = __null;
    FVECTOR* pPos = __null;
    FVECTOR vPosTmp;
    FVECTOR vLookTmp;
    FMATRIX mWC;
    ObjType::eObjBit eStopBit;
    SLContainerT<CEffect>* pContainer;

    vPosTmp.init0();
    vLookTmp.init0();
    if (pCam) {
        pPos = &pCam->Crd(0).m_World.v[3];
        // MATCH: a ternary (with an if, pPos and pLook get each other's s-registers)
        pLook = (((FVECTOR*)((char*)pCam + 0x50))->w != 0.0f) ? (FVECTOR*)((char*)pCam + 0x50) : __null;
    }
    if (!pPos || !pLook) {
        applySignWorldViewMatrix(mCam, mWV);
        InversMatrix(mCam, mCam);
    }
    if (!pPos) {
        vPosTmp = mCam.v[3];
        pPos = &vPosTmp;
    }
    if (!pLook) {
        vLookTmp.set(0.0f, 0.0f, 5.0f, 1.0f);
        ApplyMatrix(vLookTmp, mCam, vLookTmp);
        pLook = &vLookTmp;
    }
    pppSetPartMatrix(mWV, mVS, pPos, pLook);
    s_mWorldView = mWV;
    MulMatrix(s_mWorldScreen, mVS, mWV);
    if (frustumClip) {
        FMATRIX& mVC = CWorldEnv::getViewClipMatrix();
        MulMatrix(mWC, mVC, mWV);
    }
    {
        IVECTOR& col = CWorldEnv::getFogColor();
        setFogColor(col.x, col.y, col.z);
    }
    eStopBit = CSceneEnv::getStopTypeBit();
    s_pTask->getDebugPrim()->m_vVERT.clear();
    pppSetFrameTime(fFrameRate);
    SPAD::allocSpad(0x4000);
    pppPartCalcReady();
#line 1462
    ASSERT(( s_pEffectList));
    for (pContainer = s_pEffectList->getHead()->getNext(); pContainer; pContainer = pContainer->getNext()) {
        CEffect* effect = pContainer->getValue();
        int viewclip;
        if (!effect) continue;
        if (!effect->isAlive()) continue;
        if (!effect->isDrawing()) continue;
        if (!effect->m_pPartMng) continue;
        if (!effect->_checkData()) {
            char str[128];
            if (effect->m_pEffName) {
                sprintf(str, "Data Err!! calcEffect %s %d\n", effect->m_pEffName, effect->m_nEffNo);
            } else {
                sprintf(str, "Data Err!! calcEffect %08x %d\n", effect, effect->m_nEffNo);
            }
            TRACE(str);
#line 1479
            AssertApp(__FILE__, __LINE__, "(0)", str);
        }
        if (_updateCoord(effect)) {
            _updateColor(effect);
            if (!(s_iStatus & 4) || ((1 << effect->getType()) & eStopBit)) {
                pppStopParticle(effect->m_pPartMng, 1);
            } else {
                pppStopParticle(effect->m_pPartMng, effect->isStop());
            }
            effect->_setSpecialData();
            pppSetUp(effect->m_pPartMng);
            viewclip = effect->_viewclip(mWC, frustumClip);
            if (!viewclip && ((s_iStatus & 0x80000000) || DebugWork::g_dw.sys.m_iEffectBBOX)) {
                _setDebugBBOXLine(pppGetBbox(effect->m_pPartMng));
            }
            effect->m_iFlag |= 2;
            if (effect->_fade(fFrameRate) && pppPartCalc(effect->m_pPartMng, viewclip)) continue;
        }
        effect->kill();
    }
    pppPartCalcEnd();
    SPAD::freeSpad(0x4000);
}
FLOAT CEffectMan::getFrameRate() { return s_fFrameRate; }
void CEffectMan::play(FLOAT fFrameRate) {
    s_fFrameRate = fFrameRate;
    s_iStatus |= 4;
}
static inline BOOL _isPause() { return s_iStatus & 2; }
void CEffectMan::update() {
    BOOL bPlay;
    FMATRIX* pMWV;
    FMATRIX* pMVS;
    CCamera* pCam;

    if (DebugWork::g_dw.sys.m_iStopEffect) return;
    bPlay = !_isPause();
    if (!(s_iStatus & 1)) bPlay = FALSE;
    if (bPlay) {
        if (s_pArgWorldView) pMWV = s_pArgWorldView; else pMWV = &CWorldEnv::getWorldViewMatrix();
        if (s_pArgViewScreen) pMVS = s_pArgViewScreen; else pMVS = &CWorldEnv::getViewScreenMatrix();
        if (s_pArgCamera) pCam = s_pArgCamera; else pCam = CWorldEnv::getActiveCamera();
        _calc(s_fFrameRate, *pMWV, *pMVS, pCam, TRUE);
    }
    s_fFrameRate = 0.0f;
    s_iStatus &= ~4;
}
void CEffectMan::_sendModel(void* pDmaChanVif1, EFFMAN_ZSORT_BUF* pEffList, INT nIdx, BOOL bReInit) {
    pacVif1Packet pktVif1;
    u_long128* pPkt;
    INT nPkt;

    nPkt = (s_nPacketMemSize - 64) >> 5;
    pPkt = (u_long128*)(((UINT)s_pPacketMemory + 63) & ~63);
    pPkt += nPkt * nIdx;
    if (!bReInit) sceGsSyncPath(0, 0);
    CPackMan::pkInit(pPkt, &pktVif1, nPkt);
    CPackMan::pkReset();
    CPackMan::pkCnt(0);
    CPackMan::pkAddCode(0x13000000);
    if (!bReInit) {
        ((sceDmaChan*)pDmaChanVif1)->chcr.TTE = 1;
        CModel::resetMicroProgram();
        CModelMan::setupDrawMode(0);
    }
    if (pEffList->pMProg) CModel::disp0_SetMicroCode(pEffList->pMProg);
    CPackMan::pkCall((u_long128*)pEffList->pPkt, 0);
    CPackMan::pkEnd(0);
    CPackMan::pkTerminate();
    if (bReInit) sceGsSyncPath(0, 0);
    sceDmaSend((sceDmaChan*)pDmaChanVif1, (void*)((UINT)pktVif1.pBase & 0x0FFFFFFF));
}
INT CEffectMan::setup() {
    INT nSemiNum;
    INT iDrawFlags;

    s_nDrawNum = 0;
    if (DebugWork::g_dw.sys.m_iStopEffect) return 0;
    nSemiNum = CModelMan::getSemiPktListNum();
    s_nEffNum = getNum();
    if (s_iStatus & 2) s_nEffNum = 0;
    if (!(s_iStatus & 1)) s_nEffNum = 0;
    s_eDrawBit = (ObjType::eObjBit)0xFFFFFFFFU;
    s_bDrawALL = FALSE;
    iDrawFlags = CPackMan::getEnableDraw();
    if (iDrawFlags & 4) {
        s_bDrawALL = TRUE;
    } else if (isLastDraw() && (iDrawFlags & 8)) {
        s_bDrawALL = TRUE;
        s_eDrawBit = ObjType::OBJ_BIT_2D;
    }
    if (CPackMan::isSkipPacketDMA()) {
        s_bDrawALL = FALSE;
        nSemiNum = 0;
    }
    s_nDrawNum = s_nEffNum + nSemiNum;
    if (s_nDrawNum <= 0) return 0;
#line 1691
    s_pEffList = (EFFMAN_ZSORT_BUF*)com_malloc_d(s_nDrawNum * sizeof(EFFMAN_ZSORT_BUF), __FILE__ "(1691)", (UINT)-1);
    ASSERT(s_pEffList);
    s_nDrawNum = _sortEffect(s_pEffList, s_mWorldScreen, s_nEffNum, nSemiNum);
    if (s_nDrawNum <= 0) com_free(s_pEffList);
    return s_nDrawNum;
}
void CEffectMan::draw() {
    INT nEffNum;
    EFFMAN_ZSORT_BUF* pEffList;
    BOOL bDrawALL;
    ObjType::eObjBit eDrawBit;
    BOOL bReInit;
    INT nIdx;
    sceDmaChan* pDmaChanVif1;
    int i;
    INT nDrawNum = s_nDrawNum;

    if (nDrawNum <= 0) return;
    if (DebugWork::g_dw.sys.m_iStopEffect) return;
    nEffNum = s_nEffNum;
    pEffList = s_pEffList;
    bDrawALL = s_bDrawALL;
    eDrawBit = s_eDrawBit;
    bReInit = FALSE;
    nIdx = 0;
    pDmaChanVif1 = sceDmaGetChan(1);
    if (nEffNum > 0) {
        SPAD::allocSpad(0x4000);
        pppPartDrawReady(bDrawALL);
    }
    for (i = 0; i < nDrawNum; i++) {
        CEffect* effect = pEffList[i].pEff;
        if (!effect) {
            _sendModel(pDmaChanVif1, &pEffList[i], nIdx, bReInit);
            nIdx ^= 1;
            bReInit = TRUE;
        } else if (effect->m_iFlag & 2) {
            effect->m_iFlag &= ~2;
            if (effect->isAlive() && effect->m_pPartMng) {
                BOOL bDraw;
                if (!effect->_checkData()) {
                    char str[128];
                    if (effect->m_pEffName) {
                        sprintf(str, "Data Err!! drawEffect %s %d\n", effect->m_pEffName, effect->m_nEffNo);
                    } else {
                        sprintf(str, "Data Err!! drawEffect %08x %d\n", effect, effect->m_nEffNo);
                    }
                    TRACE(str);
#line 1760
                    AssertApp(__FILE__, __LINE__, "(0)", str);
                }
                if (bReInit) {
                    sceGsSyncPath(0, 0);
                    bReInit = FALSE;
                    pppInitDraw(bDrawALL);
                }
                effect->_setSpecialData();
                pppSetUp(effect->m_pPartMng);
                bDraw = effect->isDrawing();
                if (!((1 << effect->getType()) & eDrawBit)) bDraw = FALSE;
                bDraw &= bDrawALL;
                if (DebugWork::g_dw.sys.m_iEffect) bDraw = FALSE;
                if (!pppPartDraw(effect->m_pPartMng, bDraw)) effect->kill();
            }
        }
    }
    com_free(pEffList);
    sceGsSyncPath(0, 0);
    pDmaChanVif1->chcr.TTE = 1;
    if (nEffNum > 0) {
        pppPartDrawEnd();
        SPAD::freeSpad(0x4000);
    }
}
void CEffectMan::destroyer() {
    SLContainerT<CEffect>* pContainer;

    if (getNum() <= 0) return;
#line 1826
    ASSERT(( s_pEffectList));
    for (pContainer = s_pEffectList->getHead()->getNext(); pContainer; pContainer = pContainer->getNext()) {
        CEffect* effect = pContainer->getValue();
        if (effect && !effect->isAlive()) {
            if (effect->isDelete()) {
                pContainer->setValue(__null);
                delete effect;
            } else if (effect->isSuicide()) {
                effect->m_iFlag |= 0x100000;
            }
        }
    }
    s_pEffectList->sweep();
}
void CEffectMan::initialize(CTask* p) {
#line 1864
    ASSERT((__null == s_pSLCManager) && (__null == s_pEffectList) && (__null == s_pExceptEffect));
#line 1866
    s_pSLCManager = new (__FILE__ "(1866)") CEffectSLCManager;
    ASSERT(s_pSLCManager);
#line 1869
    s_pEffectList = new (__FILE__ "(1869)") CEffectList(*s_pSLCManager);
    TRACE("sizeof(CEffect) == %dbyte\n", sizeof(CEffect));
#line 1872
    s_pExceptEffect = new (__FILE__ "(1872)") CEffect;
    ASSERT(s_pEffectList && s_pExceptEffect);
    s_pEffectList->pushBack(*s_pExceptEffect);
    _effInitialize(0x80000, 0x30000);
    if (!s_pTask) {
#line 1881
        new (__FILE__ "(1881)") CEffectTask(p);
    }
}
void CEffectMan::finalize() {
    if (s_pTask) s_pTask->out();
    _effFinalize();
    if (s_pEffectList) delete s_pEffectList;
    s_pEffectList = __null;
    if (s_pExceptEffect) delete s_pExceptEffect;
    s_pExceptEffect = __null;
    if (s_pSLCManager) delete s_pSLCManager;
    s_pSLCManager = __null;
}
HANDLE CEffectMan::add(CEffect& eff) {
    CEffectList* pList;
    SLContainerT<CEffect>* pNew;
    HANDLE h;

    if (eff._getHandle()) return -1;
    pList = s_pEffectList;
#line 1912
    ASSERT(pList);
    pNew = pList->pushBack(eff);
    if (!pNew) return -1;
    h = pNew->getHandle();
    eff._setHandle(h);
    return h;
}
BOOL CEffectMan::del(CEffect& eff) {
    HANDLE h;
    BOOL bResult;

    h = eff._getHandle();
    if (!h) return FALSE;
#line 1935
    ASSERT(s_pEffectList);
    bResult = s_pEffectList->remove(h);
    if (bResult) eff._setHandle(0);
    return bResult;
}
BOOL CEffectMan::del(HANDLE hEffect) {
    CEffect* pEff = getEffect(hEffect);
    if (pEff) return del(*pEff);
    return FALSE;
}
INT CEffectMan::getNum() {
    return s_pEffectList ? s_pEffectList->getSize() - 1 : 0;
}
CEffect* CEffectMan::getEffect(HANDLE hEffect) {
    CEffect* pEff = __null;

    if (s_pSLCManager && hEffect) {
        SLContainerT<CEffect>* pContainer = s_pSLCManager->getContainer(hEffect);
        if (pContainer) pEff = pContainer->getValue();
    }
    if (pEff && !pEff->isActive()) {
        pEff->kill();
        pEff = __null;
    }
    return pEff;
}
void CEffectMan::killEffect(HANDLE hEffect, FLOAT fFrame) {
    CEffect* pEff = getEffect(hEffect);
    if (pEff) pEff->fadeout(fFrame);
}
void CEffectMan::loopEndEffect(HANDLE hEffect) {
    CEffect* pEff = getEffect(hEffect);
    if (pEff) pEff->loopEnd();
}
CEffect& CEffectMan::getExceptEffect() {
#line 2006
    ASSERT(s_pExceptEffect);
    return *s_pExceptEffect;
}
void CEffectMan::setWorldViewScreenMatrix(const FMATRIX* pMWV, const FMATRIX* pMVS, CCamera* pCam) {
    s_pArgWorldView = (FMATRIX*)pMWV;
    s_pArgViewScreen = (FMATRIX*)pMVS;
    s_pArgCamera = pCam;
}
void CEffectMan::setLastDraw(BOOL bLast) {
    if (bLast) {
        s_iStatus |= 8;
    } else {
        s_iStatus &= ~8;
    }
}
BOOL CEffectMan::isLastDraw() { return s_iStatus & 8; }
void CEffectMan::dispDebugBBOXLine(BOOL bDisp) {
    if (bDisp) {
        s_iStatus |= 0x80000000;
    } else {
        s_iStatus &= ~0x80000000;
    }
}
void CEffectMan::_setDebugBBOXLine(const FVECTOR* pV) {
    int idx[2][8] = {{0, 2, 4, 1, 0, 3, 6, 1}, {5, 3, 6, 7, 5, 2, 4, 7}};
    G_STRIP Line;
    INT j;
    INT i;

    Line.C.r = 0xFF;
    Line.C.g = 0;
    Line.C.b = 0;
    Line.C.a = 0x80;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 8; j++) {
            Line.P = pV[idx[i][j]];
            if (j >= 7) {
                Line.Term = 1;
            } else {
                Line.Term = 0;
            }
            s_pTask->getDebugPrim()->m_vVERT.push_back(Line);
        }
    }
}

// SOURCE DATA: .data .rodata .bss
