

#define COLLDISP_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollDisp.h" /* MATCH: this unit names CollDisp.h so (header default "Game/Collision/CollDisp.h") */

/* .rodata starts with the strings of the included headers' inline functions, in this order. */
#include "System/Graphic/Gprim.h"
#include "Game/Collision/CollUtilA.h"
#include "Game/Collision/CollUtil.h"
#include "System/Task.h"

#include "Game/Collision/CollObj.h"
#include "Game/Collision/Ride.h"



#include "Game/Collision/CollDisp.h"
#include "System/Graphic/Camera.h"
#include "System/Graphic/WorldEnv.h"
#include "System/Tsin.h"
#include "Game/Collision/CollBody.h"
#include "Game/Debug/DebugWork.h"
#include "Game/GameWork.h"
#include "System/HeapMan.h"

template <class T> static inline T& _ref(T& v) { return v; }

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

namespace Math3D {
FLOAT VectorLength(const FVECTOR& inVector);
}
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
FLOAT InnerProduct(const FVECTOR& v0);
void OuterProduct(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
extern "C" float atan2f(float y, float x);

















static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}

CCollDisp* CCollDisp::m_pInstance = NULL;
INT CCollDisp::m_fgDisp = 0;
UINT CCollDisp::m_fgMapZoneDisp = 0;
SHORT CCollDisp::m_bMapZoneColR = 255;
SHORT CCollDisp::m_bMapZoneColG = 255;
SHORT CCollDisp::m_bMapZoneColB = 255;
SHORT CCollDisp::m_bMapZoneColA = 128;

CCollDisp::CCollDisp(CTask* pInParent, INT fgDisp, INT inMapZoneDisp) : CTaskSecondary(pInParent) {
    m_pLines = NULL;
    m_nDispAtkNum = 0;
    m_nDispBodyNum = 0;
    m_pMapZone = NULL;
    init();
    m_fgDisp = fgDisp;
    m_fgMapZoneDisp = inMapZoneDisp;
}

CCollDisp::~CCollDisp() {
    delete[] m_pLines;
    m_pLines = NULL;
    if (m_pMapZone) delete m_pMapZone;
    m_pMapZone = NULL;
    m_nDispBodyNum = 0;
    m_nDispAtkNum = 0;
    if (_ref(m_pInstance) == this) m_pInstance = NULL;
}

void CCollDisp::init() {
    INT i;
    addCoord(40);
    for (i = 0; i < 40; i++) {
        vu0UnitMatrix(Crd(i).m_Local);
        Crd(i).recalcWorldMatrix();
    }
#line 121
    m_pLines = new KH_AT("(121)") C3DCPrim[40];
    for (i = 0; i < 40; i++) {
        m_pLines[i].create(2, 76, &Crd(i), 0);
        m_pLines[i].m_fGeta = 0.0f;
        m_pLines[i].setSemiTrans(ALPHA_OFF);
        m_pLines[i].setLayer(6, 0);
        UINT uiCol = i < 20 ? 0x8000C000 : 0x800808FF;
        G_STRIP* pLine = m_pLines[i].m_V;
        for (INT j = 0; j < 76; j++, pLine++) {
            pLine->P.init0();
            pLine->C.col = uiCol;
            pLine->Bone = 0;
            pLine->Term = 1;
        }
    }
}

void CCollDisp::main() {
    m_fvLookPos.init0();
    if (CWorldEnv::getActiveCamera() && CWorldEnv::getActiveCamera()->getLook()) {
        m_fvLookPos = *CWorldEnv::getActiveCamera()->getLook();
    }
    if (!(DebugWork::g_dw.collision.uiFlag & 4)) {
        CTask* pPlayer = getSysTask(3);
        if (pPlayer && pPlayer->getCoordNum() && !CGameWork::getFlag(1, 0)) {
            m_fvLookPos = pPlayer->Crd(0).getWorldMatrix().getTrans();
        }
    }
    if (m_fgMapZoneDisp) {
        if (!m_pMapZone) {
#line 163
            m_pMapZone = new KH_AT("(163)") CCollDispMapZone(this, *(MCD_INFO**)((UBYTE*)&CCollUtil::instance() + 0x20));
        }
        UINT uiCol = (m_bMapZoneColA << 24) | (m_bMapZoneColB << 16) | (m_bMapZoneColG << 8) | m_bMapZoneColR;
        if (m_pMapZone->getLineColor() != uiCol) m_pMapZone->changeLineColor(uiCol);
    } else if (m_pMapZone) {
        m_pMapZone->QuitReq();
        m_pMapZone = NULL;
    }
    if (m_fgDisp) {
        m_nDispBodyNum = 0;
        m_nDispAtkNum = 0;
        if (m_pLines) {
            setBodyLines(CCollObj::m_pCollListP);
            setBodyLines(CCollObj::m_pCollListE);
            setBodyLines(CCollObj::m_pCollListI);
            CCollisionList* pList = CCollObj::m_pCollListA;
            if (pList) {
                setAttackLines(pList);
                pList->removeAll();
            }
        }
    }
}

void CCollDisp::setAttackLines(CCollisionList* pList) {
    for (CCollObj *pEntry = pList->getHead(), *__pn = pEntry ? pEntry->getNext() : NULL; pEntry;
         pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL) {
        INT type = pEntry->getType();
        if (!((1 << type) & 0x9B6U)) continue;
        CCollision* pColl = pEntry->getCollision();
        if (!checkRange(pColl->m_inWE)) continue;
        switch (type) {
        case 1:
            drawColumn(m_pLines[m_nDispAtkNum + 20], pColl->m_inWS, pColl->m_inWE, pColl->m_shiftY, 0.0f, 0x800808FF);
            m_nDispAtkNum++;
            break;
        case 4:
        case 5:
            drawColumn(m_pLines[m_nDispAtkNum + 20], pColl->m_inWS, pColl->m_inWE, pColl->m_shiftY, pColl->m_Radius,
                       0x800808FF);
            m_nDispAtkNum++;
            break;
        case 7:
            if (m_nDispAtkNum < 18) {
                /* a block-local cast: the original's .mdebug has a block here (its local optimized away) */
                CPoly3Collision* pPoly3 = (CPoly3Collision*)pColl;
                drawCapsule(m_pLines[m_nDispAtkNum + 20], pColl->m_inWS, pColl->m_inWE, pColl->m_shiftY,
                            pColl->m_Radius, 0x800808FF);
                drawCapsule(m_pLines[m_nDispAtkNum + 21], pColl->m_inWE, pPoly3->m_BasePos, pColl->m_shiftY,
                            pColl->m_Radius, 0x800808FF);
                drawCapsule(m_pLines[m_nDispAtkNum + 22], pPoly3->m_BasePos, pColl->m_inWS, pColl->m_shiftY,
                            pColl->m_Radius, 0x800808FF);
                m_nDispAtkNum += 3;
            }
            break;
        case 11:
            drawColumn(m_pLines[m_nDispAtkNum + 20], pColl->m_inWS, pColl->m_inWS, pColl->m_shiftY, pColl->m_Radius,
                       0x800808FF);
            m_nDispAtkNum++;
            break;
        default:
            drawCapsule(m_pLines[m_nDispAtkNum + 20], pColl->m_inWS, pColl->m_inWE, pColl->m_shiftY, pColl->m_Radius,
                        0x800808FF);
            m_nDispAtkNum++;
            break;
        }
        if (m_nDispAtkNum >= 20) break;
    }
}

void CCollDisp::setBodyLines(CCollisionList* pList) {
    static unsigned int rgba_tbl[4] = {0x800808FF, 0x80C06060, 0x8000C000, 0x8000C0C0};
    for (CCollObj *pEntry = pList->getHead(), *__pn = pEntry ? pEntry->getNext() : NULL; pEntry;
         pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL) {
        INT fg = pEntry->isEnable() | (((CCollBody*)pEntry)->isEnableBody() << 1);
        if (!fg) continue;
        INT type = pEntry->getType();
        if (!((1 << type) & 0x34U)) continue;
        /* pColl after the type test: the ref has no pColl (its pseudo is copy-propagated away, as in the original). */
        CCollision* pColl = pEntry->getCollision();
        if (!checkRange(pColl->m_inWE)) continue;
        UINT uiCol = rgba_tbl[fg];
        switch (type) {
        case 4:
        case 5:
            drawColumn(m_pLines[m_nDispBodyNum], pColl->m_inWS, pColl->m_inWE, pColl->m_shiftY, pColl->m_Radius, uiCol);
            break;
        case 2:
        default:
            drawSphere(m_pLines[m_nDispBodyNum], pColl->m_inWE, pColl->m_shiftY, pColl->m_Radius, uiCol);
            break;
        }
        if (++m_nDispBodyNum >= 20) break;
    }
}

BOOL CCollDisp::checkRange(const FVECTOR& refPos) {
    FVECTOR tmp;
    unsigned int flag = DebugWork::g_dw.collision.uiFlag;
    if (!(flag & 2)) {
        SubVectorXYZ(tmp, refPos, m_fvLookPos);
        tmp.y = 0.0f;
        if (InnerProduct(tmp) > 100.0f) return FALSE;
    }
    return TRUE;
}

void CCollDisp::setBBMatrix(CCoord* inpCrd, const FVECTOR& inrefPos, FLOAT infShiftY) {
    FVECTOR tmp;
    FVECTOR pos;
    if (!inpCrd) return;
    pos = inrefPos;
    pos.y += infShiftY;
    FMATRIX& refMat = inpCrd->m_Local;
    if (CWorldEnv::getActiveCamera()) {
        SubVectorXYZ(tmp, CWorldEnv::getActiveCamera()->Crd(0).m_Local.v[3], pos);
        if (tmp.x == 0.0f && tmp.z == 0.0f) {
            if (tmp.y == 0.0f) tmp.set(0.0f, 1.0f, 0.0f, 1.0f);
            refMat.v[1].set(0.0f, 0.0f, 1.0f, 1.0f);
        } else {
            refMat.v[1].set(0.0f, 1.0f, 0.0f, 1.0f);
        }
        Normalize(refMat.v[2], tmp);
        OuterProduct(refMat.v[0], refMat.v[1], refMat.v[2]);
        Normalize(refMat.v[0], refMat.v[0]);
        OuterProduct(refMat.v[1], refMat.v[2], refMat.v[0]);
        Normalize(refMat.v[1], refMat.v[1]);
        refMat.v[2].w = 1.0f;
        refMat.v[0].w = 1.0f;
    } else {
        vu0UnitMatrix(refMat);
    }
    refMat.setTrans(pos);
    refMat.v[3].w = 1.0f;
    inpCrd->recalcWorldMatrix();
}

FLOAT CCollDisp::setColmMatrix(CCoord* inpCrd, const FVECTOR& refPos1, const FVECTOR& refPos2, FLOAT infShiftY) {
    FVECTOR tmp;
    SubVectorXYZ(tmp, refPos2, refPos1);
    FLOAT fHeight = Math3D::VectorLength(tmp);
    if (!inpCrd) return fHeight;
    vu0UnitMatrix(inpCrd->m_Local);
    if (fHeight > 0.0f) {
        FLOAT rx = atan2f(_sqrtf(tmp.x * tmp.x + tmp.z * tmp.z), tmp.y);
        FLOAT ry = atan2f(tmp.x, tmp.z);
        RotMatrixX(inpCrd->m_Local, inpCrd->m_Local, rx);
        RotMatrixY(inpCrd->m_Local, inpCrd->m_Local, ry);
    }
    inpCrd->m_Local.setTrans(refPos1);
    inpCrd->m_Local.v[3].w = 1.0f;
    inpCrd->m_Local.v[3].y += infShiftY;
    inpCrd->recalcWorldMatrix();
    return fHeight;
}

void CCollDisp::drawSphere(C3DCPrim& refPrim, const FVECTOR& refPos, FLOAT infShiftY, FLOAT infRadius, UINT inCol) {
    G_STRIP* pLine1;
    INT i;
    INT j;
    setBBMatrix(refPrim.getCoordPtr(), refPos, infShiftY);
    refPrim.m_fGeta = infRadius;
    pLine1 = refPrim.m_V;
    for (j = 0; j < 76; j++, pLine1++) pLine1->Term = 1;
    pLine1 = refPrim.m_V;
    for (j = 0; j < 16; j++, pLine1++) {
        FLOAT _x = infRadius * tcos(j * 225);
        pLine1->P.set(_x, infRadius * tsin(j * 225), 0.0f);
        pLine1->Term = 0;
        pLine1->C.col = inCol;
    }
    pLine1->P.set(infRadius, 0.0f, 0.0f);
    pLine1->C.col = inCol;
    pLine1++;
    for (i = 0; i < 3; i++) {
        FLOAT xx = infRadius * tcos(i * 350 + 550);
        FLOAT zz = infRadius * tsin(i * 350 + 550);
        for (j = 0; j < 8; j++, pLine1++) {
            FLOAT ts = tsin(j * 225);
            pLine1->P.set(xx * ts, infRadius * tcos(j * 225), zz * ts);
            pLine1->Term = 0;
            pLine1->C.col = inCol;
        }
        pLine1->P.set(0.0f, -infRadius, 0.0f);
        pLine1->C.col = inCol;
        pLine1++;
    }
    for (i = 0; i < 3; i++) {
        FLOAT yy = infRadius * tcos((TDEG)(i * 350 + 550));
        FLOAT zz = infRadius * tsin((TDEG)(i * 350 + 550));
        for (j = 0; j < 8; j++, pLine1++) {
            FLOAT ts = tsin(j * 225);
            pLine1->P.set(infRadius * tcos(j * 225), yy * ts, zz * ts);
            pLine1->Term = 0;
            pLine1->C.col = inCol;
        }
        pLine1->P.set(-infRadius, 0.0f, 0.0f);
        pLine1->C.col = inCol;
        pLine1++;
    }
}

void CCollDisp::drawColumn(C3DCPrim& refPrim, const FVECTOR& refPos1, const FVECTOR& refPos2, FLOAT infShiftY,
                           FLOAT infRadius, UINT inCol) {
    G_STRIP* pLine1;
    G_STRIP* pLine2;
    FLOAT fHeight;
    INT i;
    fHeight = setColmMatrix(refPrim.getCoordPtr(), refPos1, refPos2, infShiftY);
    refPrim.m_fGeta = infRadius * 0.25f;
    pLine1 = refPrim.m_V;
    for (i = 0; i < 76; i++, pLine1++) pLine1->Term = 1;
    pLine1 = refPrim.m_V;
    pLine2 = pLine1 + 17;
    for (i = 0; i < 16; i++, pLine1++, pLine2++) {
        pLine1->P.x = pLine2->P.x = infRadius * tcos(i * 225);
        pLine1->P.z = pLine2->P.z = infRadius * tsin(i * 225);
        pLine1->P.y = 0.0f;
        pLine2->P.y = fHeight;
        pLine2->Term = 0;
        pLine1->Term = 0;
        pLine2->C.col = inCol;
        pLine1->C.col = inCol;
    }
    pLine1->P.x = pLine2->P.x = infRadius * tcos(0);
    pLine1->P.z = pLine2->P.z = infRadius * tsin(0);
    pLine1->P.y = 0.0f;
    pLine2->P.y = fHeight;
    pLine2->C.col = inCol;
    pLine1->C.col = inCol;
    pLine1 = pLine2 + 1;
    pLine2 = pLine2 + 2;
    for (i = 0; i < 8; i++, pLine1 += 2, pLine2 += 2) {
        pLine1->P.x = pLine2->P.x = infRadius * tcos(i * 450);
        pLine1->P.z = pLine2->P.z = infRadius * tsin(i * 450);
        pLine1->P.y = 0.0f;
        pLine2->P.y = fHeight;
        pLine1->Term = 0;
        pLine2->C.col = inCol;
        pLine1->C.col = inCol;
    }
}

void CCollDisp::drawCapsule(C3DCPrim& refPrim, const FVECTOR& refPos1, const FVECTOR& refPos2, FLOAT infShiftY,
                            FLOAT infRadius, UINT inCol) {
    G_STRIP* pLine1;
    G_STRIP* pLine2;
    FLOAT fHeight;
    INT i;
    INT j;
    fHeight = setColmMatrix(refPrim.getCoordPtr(), refPos1, refPos2, infShiftY);
    refPrim.m_fGeta = infRadius * 0.25f;
    pLine1 = refPrim.m_V;
    for (i = 0; i < 76; i++, pLine1++) pLine1->Term = 1;
    pLine1 = refPrim.m_V;
    for (i = 0; i < 4; i++) {
        FLOAT xx = infRadius * tcos((TDEG)(i * 450));
        FLOAT zz = infRadius * tsin((TDEG)(i * 450));
        pLine2 = pLine1 + 9;
        for (j = 0; j < 9; j++, pLine1++, pLine2++) {
            FLOAT tc = tcos(j * 225);
            FLOAT ts = infRadius * tsin(j * 225);
            pLine1->P.x = xx * tc;
            pLine1->P.z = zz * tc;
            pLine1->P.y = -ts;
            pLine2->P.y = ts + fHeight;
            pLine2->P.x = -(xx * tc);
            pLine2->P.z = -(zz * tc);
            pLine2->Term = 0;
            pLine1->Term = 0;
            pLine2->C.col = inCol;
            pLine1->C.col = inCol;
        }
        pLine2->P.x = xx;
        pLine2->P.z = zz;
        pLine2->P.y = 0.0f;
        pLine2->C.col = inCol;
        pLine1 = pLine2 + 1;
    }
}

void CCollDisp::disp() {
    INT i;
    if (!m_fgDisp || !m_pLines) return;
    for (i = 0; i < m_nDispBodyNum; i++) m_pLines[i].disp();
    for (i = 0; i < m_nDispAtkNum; i++) m_pLines[i + 20].disp();
}

CCollDispMapZone::CCollDispMapZone(CTask* inParentObj, MCD_INFO* inInfo) : CTaskSecondary(inParentObj) {
    m_uiLineCol = 0x80FFFFFF;
    m_pLine = NULL;
    m_pPoly = NULL;
    m_pInfo = inInfo;
    m_iChipNum = 0;
    m_uiQuitReq = 0;
    if (inInfo && (m_iChipNum = inInfo->iChipSuu) > 0) {
        addCoord(m_iChipNum + 1);
        vu0UnitMatrix(Crd(0).m_Local);
        Crd(0).recalcWorldMatrix();
        MC_CHIP* pChip = m_pInfo->pMCChip;
        for (INT i = 0; i < m_iChipNum; i++, pChip++) {
            Crd(i + 1).m_pParent = &Crd(0);
            Crd(i + 1).m_Local = pChip->matLW;
            Crd(i + 1).recalcWorldMatrix();
        }
        _makeModelPrim();
    }
}

CCollDispMapZone::~CCollDispMapZone() {
    delete[] m_pLine;
    m_pLine = NULL;
    delete[] m_pPoly;
    m_pPoly = NULL;
}

void CCollDispMapZone::QuitReq() {
    if (!m_uiQuitReq) m_uiQuitReq = 1;
}

void CCollDispMapZone::_makeModelPrim() {
    INT nPoly3Sum;
    INT nPoly4Sum;
    if (!m_pInfo) return;
#line 898
    m_pLine = new KH_AT("(898)") C3DCPrim[m_iChipNum];
    ASSERT(m_pLine);
    nPoly3Sum = 0;
    nPoly4Sum = 0;
    MC_CHIP* pMCChip = m_pInfo->pMCChip;
    for (INT i = 0; i < m_iChipNum; i++, pMCChip++) {
        MC_DATA* pMCData = (MC_DATA*)pMCChip->data;
        INT nTotal = ((UINT)pMCData->prim - (UINT)pMCData->prim4) / sizeof(MC_PRIM);
        INT nPrim3 = 0;
        INT nPrim4 = 0;
        MC_PRIM* pPrim = pMCData->prim4;
        for (INT j = 0; j < nTotal; j++, pPrim++) {
            if (pPrim->kind == 0) {
                nPrim3++;
            } else {
                nPrim4++;
            }
        }
        INT nLineNum;
        INT nPolyNum;
        nPolyNum = nPrim3 * 3 + nPrim4 * 4;
        nLineNum = nPrim3 * 4 + nPrim4 * 5;
        nPoly4Sum += nPrim4;
        nPoly3Sum += nPrim3;
        if (nLineNum <= 0 || nPolyNum <= 0) continue;
        m_pLine[i].create(0x82, nLineNum, &Crd(i + 1), 0);
        m_pLine[i].m_fGeta = 0.1f;
        m_pLine[i].setSemiTrans(ALPHA_OFF);
        m_pLine[i].setLayer(6, 0);
        G_STRIP* pLine = m_pLine[i].m_V;
        pPrim = pMCData->prim4;
        for (INT j = 0; j < nTotal; j++, pPrim++) {
            if (pPrim->kind == 0) {
                for (INT kk = 0; kk < 4; kk++) {
                    pLine[kk].P.init0();
                    pLine[kk].C.col = m_uiLineCol;
                    pLine[kk].Term = 0;
                    pLine[kk].Bone = 0;
                }
                pLine[3].Term = 1;
                pLine += 4;
            } else {
                for (INT kk = 0; kk < 5; kk++) {
                    pLine[kk].P.init0();
                    pLine[kk].C.col = m_uiLineCol;
                    pLine[kk].Term = 0;
                    pLine[kk].Bone = 0;
                }
                pLine[4].Term = 1;
                pLine += 5;
            }
        }
    }
    TRACE("MAP COLLISION >> CHIP:%d POLY3:%d POLY4:%d\n", m_pInfo->iChipSuu, nPoly3Sum, nPoly4Sum);
    _updateModelPrim();
}

void CCollDispMapZone::_updateModelPrim() {
    if (!m_pLine || !m_pInfo) return;
    MC_CHIP* pMCChip = m_pInfo->pMCChip;
    for (INT i = 0; i < m_iChipNum; i++, pMCChip++) {
        MC_DATA* pMCData = (MC_DATA*)pMCChip->data;
        FVECTOR* pVertTop = pMCData->vert;
        MC_PRIM* pPrim = pMCData->prim4;
        G_STRIP* pLine = m_pLine[i].m_V;
        for (INT j = 0; j < m_pLine[i].m_iMax; pPrim++) {
            pLine[0].P.set(pVertTop[pPrim->v0].x, pVertTop[pPrim->v0].y, pVertTop[pPrim->v0].z);
            pLine[1].P.set(pVertTop[pPrim->v1].x, pVertTop[pPrim->v1].y, pVertTop[pPrim->v1].z);
            pLine[2].P.set(pVertTop[pPrim->v2].x, pVertTop[pPrim->v2].y, pVertTop[pPrim->v2].z);
            if (pPrim->kind == 0) {
                pLine[3].P.set(pVertTop[pPrim->v0].x, pVertTop[pPrim->v0].y, pVertTop[pPrim->v0].z);
                j += 4;
                pLine += 4;
            } else {
                pLine[3].P.set(pVertTop[pPrim->v3].x, pVertTop[pPrim->v3].y, pVertTop[pPrim->v3].z);
                pLine[4].P.set(pVertTop[pPrim->v0].x, pVertTop[pPrim->v0].y, pVertTop[pPrim->v0].z);
                j += 5;
                pLine += 5;
            }
        }
    }
}

void CCollDispMapZone::changeLineColor(UINT inRGBA) {
    m_uiLineCol = inRGBA;
    if (!m_pLine) return;
    for (INT i = 0; i < m_iChipNum; i++) {
        G_STRIP* pLine = m_pLine[i].m_V;
        for (INT j = 0; j < m_pLine[i].m_iMax; j++, pLine++) pLine->C.col = m_uiLineCol;
    }
}

void CCollDispMapZone::main() {
    if (m_uiQuitReq >= 2) out();
}

void CCollDispMapZone::disp() {
    if (m_uiQuitReq) {
        m_uiQuitReq++;
        return;
    }
    if (!m_pLine) return;
    for (INT i = 0; i < m_iChipNum; i++) m_pLine[i].disp();
}

CCollRideModelDisp::CCollRideModelDisp(CTask* inBo, CRideModel* inRideModel, BOOL inAutoUpdate) : CTaskSecondary(inBo) {
    m_uiLineCol = 0x80FFFFFF;
    m_pLine = NULL;
    m_pPoly = NULL;
    m_pRideModel = inRideModel;
    m_uiStateFlag = 0;
    m_iChipNum = 0;
#line 1090
    ASSERT(__null != m_pRideModel);
    m_uiLineCol = (CCollDisp::m_bMapZoneColA << 24) | (CCollDisp::m_bMapZoneColB << 16) |
                  (CCollDisp::m_bMapZoneColG << 8) | CCollDisp::m_bMapZoneColR;
    addCoord(2);
    vu0UnitMatrix(Crd(0).m_Local);
    Crd(0).recalcWorldMatrix();
    vu0UnitMatrix(Crd(1).m_Local);
    Crd(1).setParent(&Crd(0));
    _makeModelPrim();
    if (inAutoUpdate) m_uiStateFlag |= 1;
}

CCollRideModelDisp::~CCollRideModelDisp() {
    delete[] m_pLine;
    m_pLine = NULL;
    delete[] m_pPoly;
    m_pPoly = NULL;
}

void CCollRideModelDisp::_makeModelPrim() {
    if (!m_pRideModel) return;
    m_pInfo = &m_pRideModel->m_MCDInfo;
    if ((m_iChipNum = m_pInfo->iChipSuu) <= 0) return;
#line 1123
    m_pLine = new KH_AT("(1123)") C3DCPrim[m_iChipNum];
    MC_CHIP* pMCChip = m_pInfo->pMCChip;
    for (INT i = 0; i < m_iChipNum; i++, pMCChip++) {
        MC_DATA* pMCData = (MC_DATA*)pMCChip->data;
        INT nTotal = ((UINT)pMCData->prim - (UINT)pMCData->prim4) / sizeof(MC_PRIM);
        INT nPrim3 = 0;
        INT nPrim4 = 0;
        MC_PRIM* pPrim = pMCData->prim4;
        for (INT j = 0; j < nTotal; j++, pPrim++) {
            if (pPrim->kind == 0) {
                nPrim3++;
            } else {
                nPrim4++;
            }
        }
        INT nLineNum = nPrim3 * 4 + nPrim4 * 5;
        INT nPolyNum = nPrim3 * 3 + nPrim4 * 4;
        if (nLineNum <= 0 || nPolyNum <= 0) continue;
        m_pLine[i].create(0x82, nLineNum, &Crd(1), 0);
        m_pLine[i].m_fGeta = 0.1f;
        m_pLine[i].setSemiTrans(ALPHA_OFF);
        m_pLine[i].setLayer(6, 0);
        G_STRIP* pLine = m_pLine[i].m_V;
        pPrim = pMCData->prim4;
        for (INT j = 0; j < nTotal; j++, pPrim++) {
            if (pPrim->kind == 0) {
                for (INT kk = 0; kk < 4; kk++) {
                    pLine[kk].P.init0();
                    pLine[kk].C.col = m_uiLineCol;
                    pLine[kk].Term = 0;
                    pLine[kk].Bone = 0;
                }
                pLine[3].Term = 1;
                pLine += 4;
            } else {
                for (INT kk = 0; kk < 5; kk++) {
                    pLine[kk].P.init0();
                    pLine[kk].C.col = m_uiLineCol;
                    pLine[kk].Term = 0;
                    pLine[kk].Bone = 0;
                }
                pLine[4].Term = 1;
                pLine += 5;
            }
        }
    }
    updateModelPrim();
}

void CCollRideModelDisp::main() {
    if (m_uiStateFlag & 1) updateModelPrim();
    Crd(0).m_Local = *m_pRideModel->m_MCDInfo.pMatrix;
    Crd(0).recalcWorldMatrix();
}

void CCollRideModelDisp::updateModelPrim() {
    if (!m_pLine || !m_pRideModel) return;
    MC_CHIP* pMCChip = m_pInfo->pMCChip;
    for (INT i = 0; i < m_iChipNum; i++, pMCChip++) {
        MC_DATA* pMCData = (MC_DATA*)pMCChip->data;
        FVECTOR* pVertTop = pMCData->vert;
        MC_PRIM* pPrim = pMCData->prim4;
        G_STRIP* pLine = m_pLine[i].m_V;
        for (INT j = 0; j < m_pLine[i].m_iMax; pPrim++) {
            pLine[0].P.set(pVertTop[pPrim->v0].x, pVertTop[pPrim->v0].y, pVertTop[pPrim->v0].z);
            pLine[1].P.set(pVertTop[pPrim->v1].x, pVertTop[pPrim->v1].y, pVertTop[pPrim->v1].z);
            pLine[2].P.set(pVertTop[pPrim->v2].x, pVertTop[pPrim->v2].y, pVertTop[pPrim->v2].z);
            if (pPrim->kind == 0) {
                pLine[3].P.set(pVertTop[pPrim->v0].x, pVertTop[pPrim->v0].y, pVertTop[pPrim->v0].z);
                j += 4;
                pLine += 4;
            } else {
                pLine[3].P.set(pVertTop[pPrim->v3].x, pVertTop[pPrim->v3].y, pVertTop[pPrim->v3].z);
                pLine[4].P.set(pVertTop[pPrim->v0].x, pVertTop[pPrim->v0].y, pVertTop[pPrim->v0].z);
                j += 5;
                pLine += 5;
            }
        }
    }
}

void CCollRideModelDisp::disp() {
    if (!m_pLine || !m_pRideModel || !m_pRideModel->isEnable()) return;
    for (INT i = 0; i < m_pRideModel->m_MCDInfo.iChipSuu; i++) {
        if (m_pRideModel->m_pOCDWork[i].fgEnable) {
            Crd(1).m_Local = m_pRideModel->m_pMatIOld[i + 1];
            Crd(1).recalcWorldMatrix();
            m_pLine[i].disp();
        }
    }
}

// SOURCE DATA: .data .rodata
