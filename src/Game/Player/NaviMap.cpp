
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"
#include "System/Graphic/LightUnit.h"
#include "Game/Player/NaviMap.h"
#include "System/Graphic/Camera.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DFnt.h"
#include "System/Graphic/2d/2DCard.h"
#include "Game/Player/Player.h"
#include "Game/SceneInfo.h"
#include "Game/SceneEnv.h"
#include "Game/GameWork.h"
#include "Game/Game.h"
#include "Game/Debug/DebugWork.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"




#define ASSERT(e) KH_DEBUG(if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0))

#include "System/RsrcDB.h"


















void MulMatrix(FMATRIX& res, const FMATRIX& m0, const FMATRIX& m1);

extern FLOAT ASPECT_RATE_X;
extern FLOAT ASPECT_RATE_Y;

using namespace D2;

#line 291


















static inline FLOAT _deg2rad(FLOAT r) { return r * 3.1415927f / 180.0f; }

static inline FLOAT _roundRot(FLOAT fInRot) {
    return (fInRot < -3.1415927f) ? fInRot + 6.2831855f : (3.1415927f < fInRot) ? fInRot - 6.2831855f : fInRot;
}

namespace Gauge {

BOOL CNaviMap::m_bBattleEnd = 0;

static LPCSTR szNaviMap2DFile[31] = {
    "", "tr_navi.lad", "ag_navi.lad", "ol_navi.lad", "wo_navi.lad", "mo_navi.lad", "ha_navi.lad", "at_navi.lad",
    "ne_navi.lad", "ho_navi.lad", "", "tw_navi.lad", "de_navi.lad", "ca_navi.lad", "tr_navi.lad", "ag_navi.lad",
    "ol_navi.lad", "wo_navi.lad", "mo_navi.lad", "ha_navi.lad", "at_navi.lad", "ne_navi.lad", "ho_navi.lad", "",
    "tw_navi.lad", "de_navi.lad", "ca_navi.lad", "", "", "", "",
};

UINT g_uiFlag_WorldStart[27] = {
    0,       1,       2,       4,        8,        0x10,      0x20,      0x40,      0x80,
    0x100,   0x200,   0x400,   0x800,    0x1000,   0x10000,   0x20000,   0x40000,   0x80000,
    0x100000, 0x200000, 0x400000, 0x800000, 0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000,
};

CNaviMap::CNaviMap(CTask* pParent) : CTask(pParent, 0x4000) {
    _initialize();
}

CNaviMap::~CNaviMap() {
    if (m_pModels) {
        delete[] m_pModels;
    }
    m_pModels = NULL;
    if (m_pLight) {
        delete m_pLight;
    }
    m_pLight = NULL;
    if (m_hBack > 0) {
        C2DMan::destroy(m_hBack);
    }
    if (m_hFront > 0) {
        C2DMan::destroy(m_hFront);
    }
    if (m_pRoomName) {
        delete[] m_pRoomName;
    }
    m_pRoomName = NULL;
    if (m_pCard) {
        delete m_pCard;
    }
    m_pCard = NULL;
    m_bBattleEnd = FALSE;
}

void CNaviMap::_initialize() {
    m_uiState = 2;
    m_pModels = NULL;
    m_pLight = NULL;
    m_uiDispFlag = 0;
    m_hBack = 0;
    m_hFront = 0;
    m_pRoomName = NULL;
    m_vPos.init0();
    m_fDeg = 0.0f;
    m_fRotOfs = 0.0f;
    INT worldID = CSceneInfo::getCurrentWorldID(0);
    INT sceneID = CSceneInfo::getCurrentSceneNum();
    m_pCard = NULL;
    m_uiDispFlag |= 0x1000000;
    if (worldID >= 14) {
        worldID = 13;
    }
    m_uiRoomState = 1;
    m_uiCardState = 1;
    INT roomID = CSceneInfo::getCurrentRoomID();
    if (roomID == 0 || roomID == 99) {
        worldID = 13;
    }
    m_nWorldID = worldID;
    if (CGame::isRikuGame()) {
        m_nWorldID += 13;
        if (worldID == 12 && sceneID == 30) {
            m_fRotOfs = 2.3561945f;
        }
    }
    LPCSTR file = szNaviMap2DFile[m_nWorldID];
    INT nID = worldID + 0x32C8;
    if (!CRsrcDB::getFirstRsrc(12, nID, 1)) {
        m_uiState = 0;
        m_uiDispFlag |= 0x800000;
    } else {
        m_uiState = 1;
        m_uiDispFlag |= 0x800000;
    }
    m_nModelCount = CRsrcDB::getModelCount(nID);
    if (m_nModelCount) {
        MODELDATA** pModelTbl = CRsrcDB::getRsrcMDL(nID);
        if (pModelTbl) {
            INT i;
#line 194
            m_pModels = new KH_AT("(194)") CModel[m_nModelCount];
            ASSERT(m_pModels);
            for (i = 0; i < m_nModelCount; i++) {
                if (pModelTbl[i]->enableLight()) {
#line 201
                    m_pLight = new KH_AT("(201)") CLightUnit;
                    ASSERT(m_pLight);
                    break;
                }
            }
            for (i = 0; i < m_nModelCount; i++) {
                m_pModels[i].link(this, pModelTbl[i]);
                if (pModelTbl[i]->enableLight()) {
                    m_pModels[i].setLightUnit(m_pLight);
                }
            }
            CCoord& crd = Crd(0);
            vu0UnitMatrix(crd.m_Local);
            RotMatrixY(crd.m_Local, crd.m_Local, 3.1415927f);
            crd.m_Local.m[3][0] = m_vPos.x;
            crd.m_Local.m[3][1] = m_vPos.y;
            crd.m_Local.m[3][2] = m_vPos.z;
            if (m_pLight) {
                m_pLight->setAnchor(&crd, 0);
                FVECTOR vAmbient;
                vAmbient.x = 1.0f;
                vAmbient.y = 1.0f;
                vAmbient.z = 1.0f;
                vAmbient.w = 0.0f;
                m_pLight->setAmbient(vAmbient);
            }
            m_vPosWSC.set(0.0f, 0.0f, 0.4f, 1.0f);
            FVECTOR pos;
            C2DMan::getLayPosition(file, 1, 0, pos, 0);
            m_PosScreen.x = (INT)pos.x - 256;
            m_PosScreen.y = 192 - (INT)pos.y;
            changeLocalWS();
        }
    }
    calcDispFlag();
    m_cardRoom.init(0, 0);
    RoomInfo* pRoom = CSceneInfo::getRoomInfo(-1);
    if (pRoom) {
        m_cardRoom.init(pRoom->getRoomNameCardID(), 0xFF);
#line 254
        m_pRoomName = new KH_AT("(254)") C2DSeqCtrl[2];
    }
    if (m_cardRoom.m_nCategory20 == 9) {
#line 257
        m_pCard = new KH_AT("(257)") C2DCard;
    }
}

void CNaviMap::main() {
    if (CSceneEnv::isStopGauge()) {
        return;
    }
    if (m_uiState == 1) {
        if (CGameWork::getFlag(1, 0x37)) {
            m_uiDispFlag |= 0x2000000;
            CGameWork::setFlag(1, 0x37, 0);
        }
    } else if (m_uiState != 2) {
        if (CGameWork::getByte(0, 0x67) || (CGameWork::getInt(1, 0xE2) & 8)) {
            if (!(m_uiDispFlag & 0x800000)) {
                m_uiDispFlag |= 0x800000;
                if (m_hBack > 0) {
                    C2DMan::enableDisp(m_hBack, FALSE);
                }
            }
        } else {
            if (m_uiDispFlag & 0x800000) {
                m_uiDispFlag &= ~0x800000;
                if (m_hBack > 0) {
                    C2DMan::enableDisp(m_hBack, TRUE);
                }
            }
        }
    }
    if (m_cardRoom.m_nKind) {
        if (CGameWork::getInt(1, 0xE2) & 8) {
            if (!(m_uiDispFlag & 0x1000000)) {
                m_uiDispFlag |= 0x1000000;
                if (m_pRoomName) {
                    m_pRoomName->enableDisp(FALSE);
                    if (m_pCard) {
                        m_pRoomName[1].enableDisp(FALSE);
                        m_pCard->enableDisp(FALSE);
                    }
                }
            }
        } else {
            if (m_uiDispFlag & 0x1000000) {
                m_uiDispFlag &= ~0x1000000;
                if (m_pRoomName) {
                    m_pRoomName->enableDisp(TRUE);
                    if (m_pCard) {
                        m_pRoomName[1].enableDisp(TRUE);
                        m_pCard->enableDisp(TRUE);
                    }
                }
            }
        }
    }
    switch (m_uiState) {
    case 0:
        break;
    case 1:
        if (!CGameWork::getByte(0, 0x67) && !(CGameWork::getInt(1, 0xE2) & 8)) {
            LPCSTR file = szNaviMap2DFile[m_nWorldID];
            C2DMan::setLayer(2);
            if (m_uiDispFlag & 0x2000000) {
                m_uiState = 2;
                m_hBack = C2DMan::create(file, 0);
            } else {
                m_uiState = 3;
                m_hBack = C2DMan::create(file, 1);
            }
            m_uiDispFlag &= ~0x2000000;
            m_uiDispFlag &= ~0x800000;
        }
        break;
    case 2:
        if (m_hBack && C2DMan::isEnd(m_hBack)) {
            LPCSTR file = szNaviMap2DFile[m_nWorldID];
            C2DMan::destroy(m_hBack);
            C2DMan::setLayer(2);
            m_hBack = C2DMan::create(file, 1);
            m_uiState = 3;
        }
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    }
    switch (m_uiRoomState) {
    case 0:
        break;
    case 1:
        if (!(m_uiDispFlag & 0x800000) || CGameWork::getByte(0, 0x67)) {
            m_uiDispFlag &= ~0x2000000;
            m_uiDispFlag &= ~0x1000000;
            if (m_cardRoom.m_nKind) {
                LPCSTR file = szNaviMap2DFile[m_nWorldID];
                m_uiRoomState = 2;
                m_uiCardState = 2;
                initRoomName(file, 4);
            }
        }
        break;
    case 2:
        if (!(m_uiDispFlag & 0x1000000) && m_cardRoom.m_nKind) {
            if (m_pRoomName && m_pRoomName->isEnd(0)) {
                LPCSTR file = szNaviMap2DFile[m_nWorldID];
                m_uiCardState = 3;
                m_uiRoomState = 3;
                initRoomName(file, 5);
            }
            if (m_uiCardState >= 2 && m_pCard) {
                m_pCard->checkBack();
            }
        }
        break;
    case 3:
        if (!(m_uiDispFlag & 0x1000000) && m_cardRoom.m_nKind && m_uiCardState == 3 && m_pCard) {
            m_pCard->checkBack();
        }
        break;
    case 4:
        break;
    case 5:
        break;
    }
    if (m_pModels) {
        CCoord& crd = Crd(0);
        vu0UnitMatrix(crd.m_Local);
        FVECTOR rot;
        rot.x = _roundRot(_deg2rad(55.0f));
        rot.y = 0.0f;
        if (2 == KH_DW(DebugWork::g_dw.player.iTemp)) {
            rot = DebugWork::g_dw.player.vTemp;
            rot.x = _roundRot(_deg2rad(rot.x * 100.0f));
        }
        RotMatrixY(crd.m_Local, crd.m_Local, rot.y);
        RotMatrixX(crd.m_Local, crd.m_Local, rot.x);
        crd.m_Local.setTrans(m_vPos);
        crd.touch();
        if (m_nModelCount >= 22) {
            CCoord& crd1 = Crd(1);
            vu0UnitMatrix(crd1.m_Local);
            rot.y = _roundRot(g_pPlayer->getRot().y - 0.7853982f + m_fRotOfs);
            RotMatrixY(crd1.m_Local, crd1.m_Local, rot.y);
            crd1.m_Local.setTrans(m_vPos);
            Crd(1).touch();
        }
    }
}

void CNaviMap::disp() {
    if (CGameWork::getByte(0, 0x67)) {
        return;
    }
    if (CGameWork::getInt(1, 0xE2) & 8) {
        return;
    }
    if (m_uiDispFlag & 0x800000) {
        return;
    }
    if (!KH_DW(DebugWork::g_dw.sys.m_iFukidasiDisp)) {
        return;
    }
    if (!m_pModels) {
        return;
    }
    if (m_pLight) {
        m_pLight->calc();
    }
    for (INT i = 0; i < m_nModelCount; i++) {
        if (m_uiDispFlag & (1 << i)) {
            m_pModels[i].disp();
        }
    }
}

void CNaviMap::changeLocalWS() {
    FMATRIX mVS;
    FMATRIX mWV;
    mWV.init();
    RotMatrixY(mWV, mWV, 3.1415927f);
    mWV.m[3][2] = m_vPosWSC.z;
    InversMatrix(mWV, mWV);
    mWV.m[0][0] = -mWV.m[0][0];
    mWV.m[1][0] = -mWV.m[1][0];
    mWV.m[2][0] = -mWV.m[2][0];
    mWV.m[3][0] = -mWV.m[3][0];
    mWV.m[0][1] = -mWV.m[0][1];
    mWV.m[1][1] = -mWV.m[1][1];
    mWV.m[2][1] = -mWV.m[2][1];
    mWV.m[3][1] = -mWV.m[3][1];
    ViewScreenMatrix(mVS, 512.0f, ASPECT_RATE_X, ASPECT_RATE_Y, m_PosScreen.x + 0x800, 0x800 - m_PosScreen.y, 100.0f);
    MulMatrix(m_mWS, mVS, mWV);
    for (INT i = 0; i < m_nModelCount; i++) {
        m_pModels[i].setWorldScreenMatrix(&m_mWS);
        m_pModels[i].changeLayerExt(1, 2);
    }
}

void CNaviMap::calcDispFlag() {
    m_uiDispFlag = (m_uiDispFlag & 0x800000) | 1;
    if (CGame::isSoraGame()) {
        m_uiDispFlag |= 0x200000;
    } else if (CGame::isRikuGame()) {
        m_uiDispFlag |= 0x400000;
    }
    for (INT i = 0; i < 4; i++) {
        DoorInfo* pDoorInfo = CSceneInfo::getDoorInfo((DOOR_ID)i, -1);
        if (!pDoorInfo) {
            continue;
        }
        switch (pDoorInfo->getDoorState()) {
        case DOOR_STAT_CLOSED:
            if (pDoorInfo->isSpecialDoor()) {
                m_uiDispFlag |= 0x20000 << i;
            } else {
                m_uiDispFlag |= 2 << i;
            }
            break;
        case DOOR_STAT_OPENED:
            m_uiDispFlag |= 0x2000 << i;
            if (pDoorInfo->isSpecialDoor()) {
                m_uiDispFlag |= 0x20000 << i;
            } else if (pDoorInfo->isWorldEntrance()) {
                m_uiDispFlag |= 0x200 << i;
            } else {
                m_uiDispFlag |= 0x20 << i;
            }
            break;
        }
    }
}

void CNaviMap::initRoomName(const CHAR* inFile, UINT inIdxSeq) {
    if (!m_pRoomName) {
        return;
    }
    if (strlen(inFile) < 4) {
        return;
    }
    C2DSeqCtrl* pSeq = m_pRoomName;
    FVECTOR pos = {0.0f, 0.0f, 0.0f, 1.0f};

    pSeq->destroy();
    pSeq->setLayer(2);
    pSeq->addSeq((LPCTSTR)inFile, 0, inIdxSeq);
    pSeq->setPos(pos);
    G_RGBA rgba;
    rgba.col = 0x80808080;
#line 689
    C2DFnt* p2DFnt = new KH_AT("(689)") C2DFnt(0, 0, &rgba, 13, m_cardRoom.getName(), 0x42);
    pSeq->addFnt((LPCTSTR)inFile, 0, p2DFnt);
    pSeq->enableColorCtrl(TRUE);
    if (!m_pCard) {
        return;
    }
    switch (m_uiCardState) {
    case 2:
        pSeq = &m_pRoomName[1];
        pSeq->setLayer(2);
        pSeq->addSeq((LPCTSTR)inFile, 0, inIdxSeq - 2);
        pSeq->setPos(pos);
        m_pCard->setLayer(3);
        m_pCard->create(m_cardRoom);
        m_pCard->setParent(pSeq);
        m_pCard->enableColorCtrl(TRUE);
        break;
    case 3:
        pSeq = &m_pRoomName[1];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq((LPCTSTR)inFile, 0, inIdxSeq - 2);
        pSeq->setPos(pos);
        m_pCard->destroy();
        m_pCard->setLayer(3);
        m_pCard->create(m_cardRoom);
        m_pCard->setParent(pSeq);
        m_pCard->enableColorCtrl(TRUE);
        break;
    }
}


}  // namespace Gauge

// SOURCE DATA: .data .rodata
