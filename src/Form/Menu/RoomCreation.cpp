#include "Form/Menu/Menu.h"
#include "System/TaskMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/Anim.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in WorldSelect); which header it comes
 * from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}


#include "Game/Card.h"

#include "System/Graphic/Effect/effect.h"
#include "System/Ctd/CtdMan.h"
#include "Form/Menu/RoomCreation.h"
#include "System/HeapMan.h"
#include "System/MainApp.h"
#include "System/Math3D.h"
#include "System/Graphic/PathCamera.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DFnt.h"
#include "System/Graphic/2d/2DCard.h"
#include "Game/Entity.h"
#include "Game/Gimmick/GimmickDoor.h"
#include "Game/Collision/CollMapCamera.h"
#include "Game/Player/Player.h"
#include "Game/SceneInfo.h"
#include "Game/SceneEnv.h"
#include "Game/GameWork.h"
#include "Game/Game.h"
#include "Form/Menu/MenuParts.h"
#include "Game/AttachCard.h"
#include "include_asm.h"




#line 273


#include "System/Sound/SoundMan.h"
#line 283

extern "C" {
int sprintf(char*, const char*, ...);
char* strcpy(char*, const char*);
char* strcat(char*, const char*);
char* strchr(const char*, int);
float powf(float, float);
}

using namespace D2;























void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
extern INT SCREEN_HEIGHT;

#include "System/RsrcDB.h"





#include "System/Rcf.h"





#include "System/Graphic/Glare.h"




















INT CRoomCreation::m_iWorldRsrc = 0;
INT CRoomCreation::m_iPlayerRsrc = 0;
INT CRoomCreation::m_iEffectRsrc = 0;

CRoomCreation::CRoomCreation(CTask* p, HANDLE hDoor) : CMenu(p, 1) {

    declareSysTask(0x1D);
    CGame::enablePause(FALSE);
    m_pLadName = "room01.lad";
    m_pSeqName = "room01:00";
    m_hDoor = hDoor;
    m_pPathCamera = NULL;
    m_nMenuStep = 0;
    for (INT i = 0; i < 4; i++) {
        m_pCardNorma[i] = NULL;
        m_pCardNormaCtrl[i] = NULL;
    }
    for (INT i = 0; i < 6; i++) {
        m_pCardWinHolder[i] = NULL;
    }
    m_pScrollBar = NULL;
    for (INT i = 0; i < 30; i++) {
        m_pCardWinCard[i] = NULL;
    }
    for (INT i = 0; i < 2; i++) {
        m_pExplainCtrl[i] = NULL;
    }
    m_pCardWinInfo = NULL;
    for (INT i = 0; i < 10; i++) {
        m_pCardNumHolder[i] = NULL;
        m_pCardWinInfoNum[i] = NULL;
        m_pCardNumFnt[i] = NULL;
    }
    m_pCursorCtrl = NULL;
    m_pCardNumCtrl = NULL;
    m_pCardWinCtrl = NULL;
    m_pCardSample = NULL;
    m_pCardSampleCtrl = NULL;
    m_pCurrentRoomNameCtrl = NULL;
    m_pTitleCtrl = NULL;
    m_pFntInstruction = NULL;
    m_pAttachCard = NULL;
    m_fgEnd = 0;
    m_nNumSelect = 0;
    m_nCardSelect = 0;
    m_nCardSelectMax = 0;
    m_hCrownEffect = 0;
    m_hSpotEffect = 0;
    m_nCategorySelect = 0;
    CGameWork::setInt(1, 0xE2, -1);
    m_pGimmickDoor = (CGimmickDoor*)CTaskMan::getTask(m_hDoor);
    m_pRoomInfo = CSceneInfo::getRoomInfo(-1);
    m_pDoorInfo = &m_pRoomInfo->_doors[m_pGimmickDoor->getDoorID()];
    m_doorStat = m_pDoorInfo->getDoorState();
    int iDoorType[4] = { 0x80, 0x100, 0x200, 0x400 };
    m_iObjType = iDoorType[m_pDoorInfo->_id] ^ 0x3FF8;
    CSceneEnv::setStop((ObjType::eObjBit)(m_iObjType | 4));
    CGameWork::setByte(1, 0x140, 2);
    CSound::pauseAllSE();
    CMainApp::setFpsHalf();
}


CRoomCreation::~CRoomCreation() {
    delete m_pPathCamera;
    m_pPathCamera = NULL;
    for (INT i = 0; i < 4; i++) {
        delete m_pCardNorma[i];
        m_pCardNorma[i] = NULL;
        delete m_pCardNormaCtrl[i];
        m_pCardNormaCtrl[i] = NULL;
    }
    for (INT i = 0; i < 6; i++) {
        delete m_pCardWinHolder[i];
        m_pCardWinHolder[i] = NULL;
    }
    delete m_pScrollBar;
    m_pScrollBar = NULL;
    for (INT i = 0; i < 30; i++) {
        delete m_pCardWinCard[i];
        m_pCardWinCard[i] = NULL;
    }
    for (INT i = 0; i < 2; i++) {
        delete m_pExplainCtrl[i];
        m_pExplainCtrl[i] = NULL;
    }
    delete m_pCardWinInfo;
    m_pCardWinInfo = NULL;
    for (INT i = 0; i < 10; i++) {
        delete m_pCardNumHolder[i];
        m_pCardNumHolder[i] = NULL;
        delete m_pCardWinInfoNum[i];
        m_pCardWinInfoNum[i] = NULL;
    }
    delete m_pCardWinCtrl;
    m_pCardWinCtrl = NULL;
    delete m_pCardNumCtrl;
    m_pCardNumCtrl = NULL;
    delete m_pCardSample;
    m_pCardSample = NULL;
    delete m_pCardSampleCtrl;
    m_pCardSampleCtrl = NULL;
    delete m_pCursorCtrl;
    m_pCursorCtrl = NULL;
    delete m_pTitleCtrl;
    m_pTitleCtrl = NULL;
    delete m_pCurrentRoomNameCtrl;
    m_pCurrentRoomNameCtrl = NULL;
    delete m_pFntInstruction;
    m_pFntInstruction = NULL;
    dispose();
    CGame::enablePause(TRUE);
    CSound::startAllPausedSE();
    CGameWork::setByte(1, 0x140, 0);
    CGameWork::setFlag(1, 0x1E, FALSE);
}

/* CSceneEnv::setColor through an inline function: a constant frame argument then goes straight into $f12. */
static inline void _setColor(ObjType::eObjBit iBit, FLOAT fFrame, const FVECTOR& v, const FVECTOR* pv, INT n) {
    CSceneEnv::setColor(iBit, fFrame, v, pv, n);
}

void CRoomCreation::dispose() {
    FVECTOR vColor;
    vColor.set(1.0f, 1.0f, 1.0f, 1.0f);
    _setColor((ObjType::eObjBit)0x187C, 0.0f, vColor, NULL, 0);
    CSceneEnv::setColor((ObjType::eObjBit)4, 0.0f, vColor, NULL, 1);
    CSceneEnv::setStop((ObjType::eObjBit)0);
    CGlare::on(0.0f);
    CGameWork::setInt(1, 0xE2, 0);
    CCollMapCamera::setFgForceStop(FALSE);
}

void CRoomCreation::removeRsrc() {
    CRsrcDB::removeGroup(2, 0x66);
    if (m_iWorldRsrc) {
        CRsrcDB::removeGroup(2, m_iWorldRsrc);
        m_iWorldRsrc = 0;
    }
    if (m_iPlayerRsrc) {
        CRsrcDB::removeGroup(1, m_iPlayerRsrc);
        m_iPlayerRsrc = 0;
    }
    if (m_iEffectRsrc) {
        CRsrcDB::removeGroup(3, m_iEffectRsrc);
        m_iEffectRsrc = 0;
    }
}
static inline void _setCard(CARD& c, USHORT kind, BYTE num) {
    c.m_nId = 0;
    c.m_nNumber2 = num;
    c.m_nKind2 = kind;
    c.m_nCategory20 = cardDataTbl[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl[c.m_nKind].nCategory1;
}

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)
/* g_pPlayer read through a reference: its load stays after the preceding member stores. */
#define ASSERT_REF(e) if (!_ref(e)) AssertApp(__FILE__, __LINE__, #e, 0)





template <class T> static inline T& _ref(T& v) { return v; }

/* CDrawable::setLayer on the fonts, written through a USHORT pointer as in GameOver. */
static inline void _setFntLayer(C2DFnt* p, USHORT nID, USHORT nDepth) {
    ((USHORT*)p)[8] = nID;
    ((USHORT*)p)[9] = nDepth;
}

/* CMap's map flags (m_uiMapFlag, 0x68): Game/Map.h leaves CMap's base and members out. */
static inline INT& _mapFlag(CTask* pMap) {
    return *(INT*)((char*)pMap + 0x68);
}

void CRoomCreation::main() {
    FVECTOR vColor;
    FVECTOR vColor2;
    FVECTOR vPos;
    CCoord camCrd;
    CCoord* pCoord;
    G_RGBA rgba;
    static LPCTSTR pSeqName2 = "world_title:00";
    static char* pEffName[2] = { "rec_mon_he", "rec_mon_spot" };
    static char* pCameraName[4] = { "tobira01", "tobira03", "tobira02", "tobira04" };
    static char* pBoneName[2] = { "WeaponR", "wep0" };
    int iDoorType2[4] = { 7, 8, 9, 10 };
    short iEffctRsrc[14] = { 0x1779, 0x1779, 0x1783, 0x178D, 0x1797, 0x17A1, 0x17AB,
                             0x17B5, 0x17BF, 0x17C9, 0x17D3, 0x17DD, 0x17E7, 0x17F1 };
    CTask* pMap;
    CARD card;
    FLOAT fFrame = getFrameRateF();

    switch (m_nStep) {
    case 0: {
        INT iWorldRsrc = CSceneInfo::getCurrentWorldID(FALSE);
        INT iEffectRsrc = iEffctRsrc[iWorldRsrc];
        if (iWorldRsrc == 0 || iWorldRsrc >= 14) {
            iWorldRsrc = 1;
        }
        INT iPlayerRsrc = CGame::isSoraGame() ? 0xFE6 : 0xFE8;
        iWorldRsrc += 0x95;
        if (iWorldRsrc != m_iWorldRsrc) {
            CRsrcDB::removeGroup(2, m_iWorldRsrc);
        }
        if (iEffectRsrc != m_iEffectRsrc) {
            CRsrcDB::removeGroup(2, m_iEffectRsrc);
        }
        if (iPlayerRsrc != m_iPlayerRsrc) {
            CRsrcDB::removeGroup(2, m_iPlayerRsrc);
        }
        m_iWorldRsrc = iWorldRsrc;
        m_iEffectRsrc = iEffectRsrc;
        m_iPlayerRsrc = iPlayerRsrc;
        if (!CRsrcDB::getRsrcTex(_ref(m_pSeqName), NAME_CASE_SENSITIVE)) {
            CRcf::reqLoadRCFFront(2, 0x66, 0x100, NULL, 0);
            CRcf::reqLoadRCFFront(2, m_iWorldRsrc, 0x100, NULL, 0);
            CRcf::reqLoadRCFFront(1, m_iPlayerRsrc, 0x100, NULL, 0);
            CRcf::reqLoadRCFFront(3, m_iEffectRsrc, 0x100, NULL, 0);
            m_fgLoadRCF = TRUE;
        }
        if (!m_pGimmickDoor) {
            m_nStep = 999;
            break;
        }
        camCrd = m_pGimmickDoor->Crd(0);
        addCoord(1);
        Crd(0).setParent(NULL);
        Crd(0).m_Local = camCrd.m_Local;
        Crd(0).recalcWorldMatrix();
        vPos.set(0.0f, 0.01f, 0.0f, 1.0f);
        m_hSpotEffect = CEffectMan::attachEffect(pEffName[0], 0, (ObjType::eObjType)iDoorType2[m_pDoorInfo->_id],
                                                 &m_pGimmickDoor->Crd(0), &vPos, NULL, 0.0f);
        vColor.set(0.0f, 0.0f, 0.0f, 1.0f);
        _setColor((ObjType::eObjBit)m_iObjType, 30.0f, vColor, NULL, 0);
        vColor.set(1.0f, 1.0f, 1.0f, 0.0f);
        CSceneEnv::setColor((ObjType::eObjBit)4, 30.0f, vColor, NULL, 1);
        CGlare::off(30.0f);
        CCollMapCamera::setFgForceStop(TRUE);
        m_nStep++;
        break;
    }
    case 1:
        if (!CSceneEnv::isColorEnd(0)) {
            return;
        }
        if (m_fgLoadRCF) {
            if (!CRcf::isCompleteLoadRCF(2, 0x66)) break;
            if (!CRcf::isCompleteLoadRCF(2, m_iWorldRsrc)) break;
            if (!CRcf::isCompleteLoadRCF(1, m_iPlayerRsrc)) break;
            if (!CRcf::isCompleteLoadRCF(3, m_iEffectRsrc)) return;
        }
        vColor.zero();
        CSceneEnv::setColor((ObjType::eObjBit)m_iObjType, 15.0f, vColor, NULL, 0);
        m_nStep++;
        break;
    case 2:
        m_hLay = C2DMan::create(m_pLadName, 4);
#line 330
        m_pTitleCtrl = new (__FILE__ "(330)") C2DSeqCtrl;
        m_pTitleCtrl->setLayer(12);
        m_pTitleCtrl->addSeq(pSeqName2, 0);
        m_pTitleCtrl->addSeq(m_pSeqName, 0x7A);
        vPos.init();
        m_pTitleCtrl->setPos(vPos);
#line 337
        m_pCurrentRoomNameCtrl = new (__FILE__ "(337)") C2DSeqCtrl;
        m_pCurrentRoomNameCtrl->setLayer(12);
        m_pCurrentRoomNameCtrl->enableColorCtrl(TRUE);
        m_pCurrentRoomNameCtrl->addSeq(m_pSeqName, 0x7A);
        vPos.init();
        m_pCurrentRoomNameCtrl->setPos(vPos);
        _setCard(card, m_pRoomInfo->getRoomNameCardID(), 0);
        rgba.col = 0x807C7C7C;
#line 346
        m_pCurrentRoomNameFnt = new (__FILE__ "(346)") C2DFnt(0, 0, &rgba, 15, card.getName(), 0x241);
        _setFntLayer(m_pCurrentRoomNameFnt, 12, 0);
        m_pCurrentRoomNameCtrl->addFnt(m_pSeqName, m_pCurrentRoomNameFnt);
        CSound::playSE(0xC, -1, 0x40);
        m_nStep++;
        break;
    case 3: {
        if (!C2DMan::isEnd(m_hLay)) {
            return;
        }
        pMap = getSysTask(2);
        if (pMap) {
            _mapFlag(pMap) |= 1;
        }
        CSound::setBGMMasterVolume(0x40, -1);
        C2DMan::destroy(m_hLay);
        m_hLay = C2DMan::create(m_pLadName, 5);
        m_pTitleCtrl->replaceSeq(0, pSeqName2, 1);
        m_pCurrentRoomNameCtrl->replaceSeq(0, m_pSeqName, 0x7B);
        m_nStep++;
        break;
    }
    case 4:
        if (!menuMain()) {
            break;
        }
        if (m_fgEnd == 2) {
            m_nStep = 5;
        } else {
            m_nStep = 7;
        }
        break;
    case 5:
    case 7: {
        pMap = getSysTask(2);
        if (pMap) {
            _mapFlag(pMap) &= ~1;
        }
        C2DMan::destroy(m_hLay);
        m_hLay = C2DMan::create(m_pLadName, 6);
        m_pTitleCtrl->replaceSeq(0, pSeqName2, 2);
        m_pCurrentRoomNameCtrl->replaceSeq(0, m_pSeqName, 0x7C);
        CSound::setBGMMasterVolume(-1, -1);
        m_nStep++;
        break;
    }
    case 6:
    case 8:
        if (!C2DMan::isEnd(m_hLay)) {
            return;
        }
        C2DMan::destroy(m_hLay);
        m_hLay = 0;

#line 405
        ASSERT_REF(g_pPlayer);
        if (m_nStep == 6) {
            g_pPlayer->setTrgFlag(0x10000, 0);
            m_nStep = 9;
        } else {
            vColor.set(1.0f, 1.0f, 1.0f, 1.0f);
            vColor2.init();
            _setColor((ObjType::eObjBit)m_iObjType, 30.0f, vColor, &vColor2, 0);
            CSceneEnv::setColor((ObjType::eObjBit)4, 30.0f, vColor, &vColor2, 1);
            CGlare::on(30.0f);
            m_nStep = 16;
        }
        break;
    case 9: {
#line 423
        m_pPathCamera = new (__FILE__ "(423)") CPathCamera(this);
        m_pPathCamera->setLocal(TRUE);
        m_pPathCamera->init(m_pDoorInfo->_id >= 2 ? pCameraName[0] : pCameraName[1], TRUE, FALSE, 0x80);
        m_pPathCamera->run();
#line 430
        ASSERT(g_pPlayer);
        INT iBone = g_pPlayer->getBoneIndex(CGame::isSoraGame() ? pBoneName[0] : pBoneName[1], 0);
        pCoord = &g_pPlayer->Crd(iBone);
#line 433
        m_pAttachCard = new (__FILE__ "(433)") CAttachCard(g_pPlayer);
        if (CGame::isSoraGame()) {
            vColor.set(0.1f, -0.08f, 0.12f, 1.0f);
            FVECTOR* pColor2 = &vColor2;
            vColor2.x = 2.1816616f;
            vColor2.y = 1.3089969f;
            vColor2.z = 0.34906584f;
            pColor2->w = 1.0f;
        } else {
            vColor.init();
            vColor2.zero();
        }
        m_pAttachCard->setup((CARD_KIND)m_card.m_nKind, pCoord, &vColor, &vColor2, 0.25f);
        CSound::playSE(m_doorStat == 1 ? 0x29 : 0x1F, -1, 0x40);
        m_fTimer = 180.0f;
        m_nStep++;
        break;
    }
    case 10:
        if (m_pPathCamera->isEnd()) {
            if (m_doorStat == 1) {
                m_pGimmickDoor->changeMotion(2, 0, FALSE, -1, 0, 0);
                if (m_fgEventDoor) {
                    m_pGimmickDoor->killEffectCrown();
                }
            } else {
                m_pGimmickDoor->changeMotion(5, 0, FALSE, -1, 0, 0);
            }
            m_nStep++;
        }
    case 11:
    case 12:
        m_fTimer -= fFrame;
        if (m_fTimer < 101.0f && m_fTimer >= 90.0f) {
            if (m_fgKeyCard > 0) {
                vPos.set(0.0f, 0.0f, 0.0f, 1.0f);
                m_fgKeyCard = 0;
                CEffectMan::attachEffect(pEffName[1], 6, (ObjType::eObjType)iDoorType2[m_pDoorInfo->_id],
                                         &m_pGimmickDoor->Crd(0), &vPos, NULL, 0.0f);
            }
            return;
        }
        if (m_fTimer < 50.0f && m_fTimer >= 40.0f) {
            if (m_nStep == 11) {
                m_nStep = 12;
                CGlare::fade(30.0f, 1.0f, -1.0f);
                m_pAttachCard->setupFade(1.0f, 0.0f, 50, -1);
            }
            return;
        }
        if (m_fTimer < 0.0f) {
            m_pAttachCard->out();
            m_pPathCamera->init(m_pDoorInfo->_id >= 2 ? pCameraName[2] : pCameraName[3], TRUE, FALSE, 0x80);
            m_pPathCamera->run();
            m_fTimer = 120.0f;
            m_nStep++;
        }
        break;
    case 13:
        m_fTimer -= fFrame;
        if (m_fTimer < 0.0f) {
            m_fTimer = 0.0f;
            m_nStep++;
        }
        break;
    case 14:
        m_fTimer -= fFrame;
        if (m_fTimer < 0.0f) {
            CSceneInfo::makeRoom(*m_pDoorInfo, 0);
            CSceneInfo::reqEnterDoor(m_pGimmickDoor->getDoorID());
            m_pDoorInfo->resetNorma();
            m_nStep = 15;
        }
        break;
    case 15:
        break;
    case 16:
        if (CSceneEnv::isColorEnd(0)) {
            CEffectMan::killEffect(m_hSpotEffect, 0.0f);
#line 528
            ASSERT(g_pPlayer);
            g_pPlayer->setTrgFlag(0x8000, 0);
            CGameWork::setInt(1, 0xE2, 0);
            CSceneEnv::setStop((ObjType::eObjBit)0);
            m_nStep = 17;
        } else if (m_hSpotEffect) {
            FVECTOR& vCol = CEffectMan::getEffect(m_hSpotEffect)->color();
            vCol.w -= fFrame * 0.033333335f;
            if (vCol.w < 0.0f) {
                vCol.w = 0.0f;
            }
        }
        break;
    case 17:
    default:
        out();
        break;
    }
}
/* MATCH: _NORMA_PIECES (System/Graphic/2d/2DCard.h) with every piece in the register the original uses at the replaceNorma
   call (lo a0, number a3, color a1, top v1; the createNorma call keeps lo in v0): the pointer argument then takes v0
   and the argument loads of getNormaOperation issue a0 first. */
#define _NORMA_PIECES_A0(t) ({ \
    register ULONGLONG lo asm("$4") = (USHORT)(t)._nOp; \
    register ULONGLONG num asm("$7") = (USHORT)(t)._nNumber; \
    register ULONGLONG hi asm("$5") = (USHORT)(t)._nColor; \
    lo <<= 16; \
    register ULONGLONG top asm("$3") = (USHORT)(t).__nDummy; \
    lo |= num; \
    hi <<= 32; \
    hi |= lo; \
    top <<= 48; \
    top | hi; })
INT CRoomCreation::menuMain() {
    CARD card;
    FVECTOR vec;
    FVECTOR vec2;
    FVECTOR vScale;
    C2DSeqCtrl* pCardWinTmp;
    G_RGBA rgba;
    BOOL fgCursorMove = FALSE;
    BOOL fgCategoryMove = FALSE;
    INT nNumSelect;
    INT nNorma;
    FLOAT fWidth;
    unsigned char categorySeqTbl[5] = { 6, 5, 3, 2, 4 };
    static USHORT scrollBarSeqTbl[3] = { 105, 102, 108 };
    static LPCTSTR pKeyStr = "\xf9" "GThe cards you see shall be the key\x81\x5c\x81\xf4";
    RCNORMA::NormaSrc normaSrc;

    switch (m_nMenuStep) {
    case 0:
        m_nNorma = m_pDoorInfo->getNormaCount();
        m_idxNorma = m_pDoorInfo->getNextNormaIndex();
        m_nNorma -= m_idxNorma;
        m_fgKeyCard = m_pDoorInfo->getNormaKeyCardID();
        m_fgEventDoor = m_pDoorInfo->isSpecialDoor();
        if (m_fgKeyCard) {
            rgba.col = 0x807E7E7E;
#line 602
            m_pFntInstruction = new (__FILE__ "(602)") CStrPrim(1, -(SCREEN_HEIGHT / 2) + 100, &rgba, 0x16, pKeyStr, 0x1202, 0xFFFF28);
            m_pFntInstruction->m_P0.set(0x18, -0x5C0, 0xFFFF280);
            m_pFntInstruction->setLayerFG(13);
            CGameWork::setByte(1, 0x140, 4);
        }
        fWidth = m_nNorma < 4 ? 44.0f : 34.0f;
        vec.set(256.0f - fWidth * (m_nNorma - 1), SCREEN_HEIGHT / 2 - 22, 0.0f, 1.0f);
        nNorma = m_nNorma - 1;
        for (INT i = nNorma; i >= 0; i--) {
#line 618
            C2DSeqCtrl* pCtrl = m_pCardNormaCtrl[i] = new (__FILE__ "(618)") C2DSeqCtrl;
            pCardWinTmp = pCtrl;
            pCardWinTmp->enableColorCtrl(TRUE);
            pCardWinTmp->setPos(vec);
            pCardWinTmp->addSeq(m_pSeqName, 0x38);
#line 623
            C2DCard* pCard = m_pCardNorma[i] = new (__FILE__ "(623)") C2DCard;
            pCard->enableColorCtrl(TRUE);
            pCard->setLayer(12);
            if (i == 0 && m_fgKeyCard) {
                _setCard(card, m_fgKeyCard, 0);
                pCard->create(card);
            } else {
                INT idx = m_idxNorma + nNorma - i;
                normaSrc._nColor = m_pDoorInfo->getNormaColor(idx);
                normaSrc._nNumber = m_pDoorInfo->getNormaNumber(idx);
                normaSrc._nOp = m_pDoorInfo->getNormaOperation(idx);
                {
                    RCNORMA::NormaSrc t = normaSrc;
                    _createNormaRaw(pCard, _NORMA_PIECES(t), 0);
                }
            }
            pCard->setParent(pCtrl);
            vec.x += fWidth + fWidth;
        }
#line 647
        m_pCardWinCtrl = new (__FILE__ "(647)") C2DSeqCtrl;
        m_pCardWinCtrl->setLayer(12);
        m_pCardWinCtrl->addSeq(m_pSeqName, 0x80);
        C2DMan::getLayPosition(m_pLadName, 0, 2, vec, 0);
        m_pCardWinCtrl->setPos(vec);
        m_pCardWinCtrl->addSeqShift(m_pSeqName, 6, -3.0f, -45.0f);
        vec2.init();
        for (INT i = 0; i < 6; i++) {
#line 657
            pCardWinTmp = m_pCardWinHolder[i] = new (__FILE__ "(657)") C2DSeqCtrl;
            pCardWinTmp->setLayer(12);
            pCardWinTmp->enableColorCtrl(TRUE);
            pCardWinTmp->addSeq(m_pSeqName, 8);
            pCardWinTmp->setParent(m_pCardWinCtrl);
            C2DMan::getLayPosition(m_pLadName, 2, i + 2, vec2, 0);
            pCardWinTmp->setLocalPos(vec2);
        }
#line 667
        pCardWinTmp = m_pCardWinInfo = new (__FILE__ "(667)") C2DSeqCtrl;
        pCardWinTmp->setLayer(12);
        pCardWinTmp->addSeq(m_pSeqName, 0x7D);
        C2DMan::getLayPosition(m_pLadName, 0, 4, vec2, 0);
        pCardWinTmp->setPos(vec2);
        m_pCardWinInfo->addSeq(m_pSeqName, 0x30);
        m_pCardWinInfo->addSeq(m_pSeqName, 0x30);
#line 676
        pCardWinTmp = m_pExplainCtrl[0] = new (__FILE__ "(676)") C2DSeqCtrl;
        pCardWinTmp->setLayer(12);
        pCardWinTmp->addSeq(m_pSeqName, 0x59);
        C2DMan::getLayPosition(m_pLadName, 0, 5, vec2, 0);
        pCardWinTmp->setPos(vec2);
        rgba.col = 0x807C7C7C;
#line 682
        m_pExplainFnt[0] = new (__FILE__ "(682)") C2DFnt(0, 0, &rgba, 14, NULL, 0x240);
        m_pExplainFnt[0]->setLayer(12, 0);
        m_pExplainFnt[0]->setSpace(-2, 0);
        pCardWinTmp->addFntShift(m_pSeqName, m_pExplainFnt[0], 12.0f, 6.0f);
        pCardWinTmp->enableColorCtrl(TRUE);
#line 691
        pCardWinTmp = m_pExplainCtrl[1] = new (__FILE__ "(691)") C2DSeqCtrl;
        pCardWinTmp->setLayer(12);
        pCardWinTmp->addSeq(m_pSeqName, 0x5F);
        C2DMan::getLayPosition(m_pLadName, 0, 6, vec2, 0);
        pCardWinTmp->setPos(vec2);
#line 696
        m_pExplainFnt[1] = new (__FILE__ "(696)") C2DFnt(0, 0, &rgba, 15, NULL, 0x240);
        m_pExplainFnt[1]->setLayer(12, 0);
        pCardWinTmp->addFntShift(m_pSeqName, m_pExplainFnt[1], 0.0f, 8.0f);
        pCardWinTmp->enableColorCtrl(TRUE);
        initInfoNum(vec);
        rgba.col = 0x807C7C7C;
#line 707
        m_pRoomNameFnt = new (__FILE__ "(707)") C2DFnt(0, 0, &rgba, 15, "", 0x240);
        m_pRoomNameFnt->setLayer(12, 0);
        m_pCardWinCtrl->addFntShift(m_pSeqName, m_pRoomNameFnt, -176.0f, 48.0f);
        rgba.col = 0x80007878;
#line 712
        m_pCardVolFnt = new (__FILE__ "(712)") C2DFnt(0, 0, &rgba, 15, "", 0x241);
        m_pCardVolFnt->setLayer(12, 0);
        m_pCardWinCtrl->addFntShift(m_pSeqName, m_pCardVolFnt, 22.0f, 48.0f);
        initCard();
#line 736
        m_pScrollBar = new (__FILE__ "(736)") CScrollBar;
        vec2.set(180.0f, -29.0f, 0.0f, 1.0f);
        m_pScrollBar->init(m_pSeqName, 12, scrollBarSeqTbl, vec2, 59.0f, m_pCardWinCtrl);
        selectDispCard();
        changeCursor(0, 0);
#line 746
        m_pCardNumCtrl = new (__FILE__ "(746)") C2DSeqCtrl;
        m_pCardNumCtrl->setLayer(13);
        m_pCardNumCtrl->addSeq(m_pSeqName, 0x26);
        C2DMan::getLayPosition(m_pLadName, 3, 0, vec, 0);
        m_pCardNumCtrl->setPos(vec);
        m_pCardNumCtrl->enableDisp(FALSE);
        m_pCardNumCtrl->enableColorCtrl(TRUE);
        initNumHolder(vec);
        m_nMenuStep++;
        break;
    case 1:
        if (!m_pCardWinCtrl->isEnd(0)) {
            break;
        }
        m_pCardWinCtrl->replaceSeq(0, m_pSeqName, 0x81);
        m_pCardWinInfo->replaceSeq(0, m_pSeqName, 0x7E);
        m_pExplainCtrl[0]->replaceSeq(0, m_pSeqName, 0x5A);
        m_pExplainCtrl[1]->replaceSeq(0, m_pSeqName, 0x60);
        changeCursor(0, 0);
        selectDispInfoNum();
        m_pExplainFnt[1]->setText(getNormaInfo());
        m_nMenuStep++;
        break;
    case 2:
        if (_isTrgOK(0)) {
            if (!m_nCardSelectMax) {
                CSound::playSE(5, -1, 0x40);
                break;
            }
            card = m_pCardWinCard[m_nCardSelect]->m_card;
            if (card.m_nCategory0 == 10 || card.m_nKind == 0xB9) {
                INT fgExist = CCard::searchCard(card.m_nKind, 0);
#line 790
                ASSERT(fgExist > 0);
                m_card = card;
                DOOR_RC_RESULT result = m_pDoorInfo->useNormaCard(card);
                if (result == DOOR_RC_RESULT_INVALID) {
                    CSound::playSE(5, -1, 0x40);
                    break;
                }
                CSound::playSE(0x43, -1, 0x40);
                CCard::deleteCard(card.m_nKind, 0);
                if (result == DOOR_RC_RESULT_CLEAR_ALL) {
                    m_fgEnd = 2;
                    m_pCardNormaCtrl[0]->replaceSeq(0, m_pSeqName, 0x37);
                    delete m_pCursorCtrl;
                    m_pCursorCtrl = NULL;
                    m_pCardWinCtrl->replaceSeq(0, m_pSeqName, 0x82);
                    m_pCardWinInfo->replaceSeq(0, m_pSeqName, 0x7F);
                    m_pExplainCtrl[0]->replaceSeq(0, m_pSeqName, 0x5B);
                    m_pExplainCtrl[1]->replaceSeq(0, m_pSeqName, 0x61);
                    m_pCardSampleCtrl->replaceSeq(0, m_pSeqName, 0xB);
                    m_nMenuStep = 7;
                } else {
                    m_pCardNormaCtrl[m_nNorma - 1]->replaceSeq(0, m_pSeqName, 0x37);
                    m_idxNorma++;
                    m_nNorma--;
                    initCardData(1);
                    selectDispInfoNum();
                    selectDispNumHolder();
                    if (searchNumExist(0, m_nNumSelect) < 0) {
                        INT i;
                        delete m_pCardWinCard[m_nCardSelect];
                        m_pCardWinCard[m_nCardSelect] = NULL;
                        for (i = m_nCardSelect; i < m_nCardSelectMax - 1; i++) {
                            m_pCardWinCard[i] = m_pCardWinCard[i + 1];
                            m_pCardWinCard[i]->setParent(m_pCardWinHolder[i % 6]);
                        }
                        m_pCardWinCard[i] = NULL;
                        m_nCardSelectMax--;
                        if (m_nCardSelect >= m_nCardSelectMax) {
                            m_nCardSelect--;
                            changeCursor(0, m_nCardSelect);
                        }
                        selectDispCard();
                        changeCursor(0, m_nCardSelect);
                    }
                    m_pExplainFnt[1]->setText(getNormaInfo());
                }
            } else {
                if (CGameWork::getFlag(1, 7) && card.m_nKind != 0xB6) {
                    CSound::playSE(5, -1, 0x40);
                    break;
                }
                m_nNumSelect = searchNumExist(0, 0);
                if (m_nNumSelect < 0) {
                    break;
                }
                CSound::playSE(2, -1, 0x40);
                m_nMenuStep = 3;
            }
        } else if (_isTrgCANCEL(0)) {
            if (CGameWork::getFlag(1, 0x1E)) {
                break;
            }
            CSound::playSE(4, -1, 0x40);
            m_fgEnd = 1;
            for (INT i = m_nNorma - 1; i >= 0; i--) {
                if (m_pCardNormaCtrl[i]) {
                    m_pCardNormaCtrl[i]->replaceSeq(0, m_pSeqName, 0x3A);
                }
            }
            m_pCardWinCtrl->replaceSeq(0, m_pSeqName, 0x82);
            m_pCardWinInfo->replaceSeq(0, m_pSeqName, 0x7F);
            m_pExplainCtrl[0]->replaceSeq(0, m_pSeqName, 0x5B);
            m_pExplainCtrl[1]->replaceSeq(0, m_pSeqName, 0x61);
            m_pCardSampleCtrl->replaceSeq(0, m_pSeqName, 0xB);
            delete m_pCursorCtrl;
            m_pCursorCtrl = NULL;
            m_nMenuStep = 7;
        } else if (_isButtonRpt(0x802000, 0) && m_nCardSelectMax) {
            m_nCardSelect++;
            if (m_nCardSelect % 6 == 0 || m_nCardSelect >= m_nCardSelectMax) {
                m_nCardSelect--;
                if (!m_buttonTrg || m_nCardSelect % 6 == 0) break;
                m_nCardSelect -= m_nCardSelect % 6;
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x408000, 0) && m_nCardSelectMax) {
            m_nCardSelect--;
            if (m_nCardSelect < 0 || m_nCardSelect % 6 == 5) {
                m_nCardSelect++;
                if (!m_buttonTrg || m_nCardSelectMax - m_nCardSelect == 1) break;
                m_nCardSelect += 5;
                if (m_nCardSelect >= m_nCardSelectMax) {
                    m_nCardSelect = m_nCardSelectMax - 1;
                }
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x204000, 0) && m_nCardSelectMax) {
            if (m_nCardSelect / 6 < (m_nCardSelectMax - 1) / 6) {
                m_nCardSelect += 6;
            } else {
                if (!m_buttonTrg || m_nCardSelectMax < 7) break;
                m_nCardSelect = m_nCardSelect % 6;
            }
            fgCursorMove = TRUE;
            if (m_nCardSelect >= m_nCardSelectMax) {
                m_nCardSelect = m_nCardSelectMax - 1;
            }
        } else if (_isButtonRpt(0x101000, 0) && m_nCardSelectMax) {
            m_nCardSelect -= 6;
            if (m_nCardSelect < 0) {
                m_nCardSelect += 6;
                if (!m_buttonTrg || m_nCardSelectMax < 7) break;
                m_nCardSelect = (m_nCardSelectMax - 1) / 6 * 6 + m_nCardSelect % 6;
                if (m_nCardSelect >= m_nCardSelectMax) {
                    m_nCardSelect = m_nCardSelectMax - 1;
                }
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(4, 0)) {
            if (--m_nCategorySelect < 0) {
                m_nCategorySelect = 4;
            }
            fgCategoryMove = TRUE;
        } else if (_isButtonRpt(8, 0)) {
            if (++m_nCategorySelect >= 5) {
                m_nCategorySelect = 0;
            }
            fgCategoryMove = TRUE;
        }
        if (fgCursorMove) {
            selectDispCard();
            CSound::playSE(0xE, -1, 0x40);
            changeCursor(0, m_nCardSelect);
            m_pCardSampleCtrl->replaceSeq(0, m_pSeqName, 0xA);
        }
        if (fgCategoryMove) {
            CSound::playSE(1, -1, 0x40);
            m_pCardWinCtrl->replaceSeqShift(1, m_pSeqName, categorySeqTbl[m_nCategorySelect], -3.0f, -45.0f);
            m_nCardSelect = 0;
            initCard();
            selectDispCard();
            changeCursor(0, m_nCardSelect);
        }
        break;
    case 3:
        m_pCardNumCtrl->replaceSeq(0, m_pSeqName, 0x26);
        m_pCardNumCtrl->enableDisp(TRUE);
        for (INT i = 0; i < 10; i++) {
            m_pCardNumHolder[i]->enableDisp(TRUE);
        }
        selectDispNumHolder();
        changeCursor(1, m_nNumSelect);
        CGameWork::setByte(1, 0x140, 3);
        m_nMenuStep++;
        break;
    case 4:
        m_nMenuStep++;
        break;
    case 5:
        if (_isTrgOK(0)) {
            if (m_nNumSelect < 0) {
                break;
            }
            _setCard(card, m_pCardWinCard[m_nCardSelect]->m_card.m_nKind, m_nNumSelect);
            INT fgExist = CCard::searchCard(card.m_nKind, m_nNumSelect);
#line 1020
            ASSERT(fgExist > 0);
            m_card = card;
            DOOR_RC_RESULT result = m_pDoorInfo->useNormaCard(card);
            if (result == DOOR_RC_RESULT_INVALID) {
                CSound::playSE(5, -1, 0x40);
                break;
            }
            CSound::playSE(0xF, -1, 0x40);
            CCard::deleteCard(card.m_nKind, card.m_nNumber);
            if (result == DOOR_RC_RESULT_CLEAR_ALL) {
                m_fgEnd = 2;
                delete m_pCursorCtrl;
                m_pCursorCtrl = NULL;
                m_pCardNormaCtrl[m_nNorma - 1]->replaceSeq(0, m_pSeqName, 0x37);
                m_pCardWinCtrl->replaceSeq(0, m_pSeqName, 0x82);
                m_pCardWinInfo->replaceSeq(0, m_pSeqName, 0x7F);
                m_pExplainCtrl[0]->replaceSeq(0, m_pSeqName, 0x5B);
                m_pExplainCtrl[1]->replaceSeq(0, m_pSeqName, 0x61);
                m_pCardSampleCtrl->replaceSeq(0, m_pSeqName, 0xB);
                CSound::playSE(0x43, -1, 0x40);
                m_nMenuStep = 7;
            }
            if (m_nMenuStep == 5) {
                if (result == DOOR_RC_RESULT_CLEAR_ONE) {
                    m_pCardNormaCtrl[m_nNorma - 1]->replaceSeq(0, m_pSeqName, 0x37);
                    m_idxNorma++;
                    m_nNorma--;
                    CSound::playSE(0x43, -1, 0x40);
                    m_nMenuStep = 6;
                } else {
                    normaSrc._nColor = m_pDoorInfo->getNormaColor(m_idxNorma);
                    normaSrc._nNumber = m_pDoorInfo->getNormaNumber(m_idxNorma);
                    normaSrc._nOp = m_pDoorInfo->getNormaOperation(m_idxNorma);
                    {
                        RCNORMA::NormaSrc t = normaSrc;
                        _replaceNormaRaw(m_pCardNorma[m_nNorma - 1], _NORMA_PIECES_A0(t), 0);
                    }
                }
                m_pExplainFnt[1]->setText(getNormaInfo());
                initCardData(1);
                selectDispInfoNum();
                selectDispNumHolder();
                nNumSelect = searchNumExist(3, m_nNumSelect);
                if (nNumSelect < 0) {
                    nNumSelect = searchNumExist(3, m_nNumSelect < 5 ? m_nNumSelect + 5 : m_nNumSelect - 5);
                    if (nNumSelect < 0) {
                        INT i;
                        m_nNumSelect = -1;
                        delete m_pCardWinCard[m_nCardSelect];
                        m_pCardWinCard[m_nCardSelect] = NULL;
                        for (i = m_nCardSelect; i < m_nCardSelectMax - 1; i++) {
                            m_pCardWinCard[i] = m_pCardWinCard[i + 1];
                            m_pCardWinCard[i]->setParent(m_pCardWinHolder[i % 6]);
                        }
                        m_pCardWinCard[i] = NULL;
                        m_nCardSelectMax--;
                        if (m_nCardSelect >= m_nCardSelectMax) {
                            m_nCardSelect--;
                            changeCursor(0, m_nCardSelect);
                        }
                        selectDispCard();
                        m_nMenuStep = 6;
                    }
                }
                m_nNumSelect = nNumSelect;
                changeCursor(1, nNumSelect);
            }
        } else if (_isTrgCANCEL(0)) {
            CSound::playSE(4, -1, 0x40);
            m_nMenuStep++;
        } else if (_isButtonRpt(0x802000, 0)) {
            nNumSelect = m_nNumSelect + 1;
            if (nNumSelect % 5 == 0) {
                if (!m_buttonTrg) break;
                if (nNumSelect >= 10) nNumSelect = 0;
                nNumSelect = searchNumExist(5, nNumSelect);
            } else {
                nNumSelect = searchNumExist(1, nNumSelect);
                if (nNumSelect < 0) {
                    if (!m_buttonTrg) break;
                    nNumSelect = searchNumExist(5, m_nNumSelect + 1);
                }
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x408000, 0)) {
            nNumSelect = m_nNumSelect - 1;
            if (nNumSelect < 0 || nNumSelect % 5 == 4) {
                if (!m_buttonTrg) break;
                if (nNumSelect < 0) nNumSelect = 9;
                nNumSelect = searchNumExist(6, nNumSelect);
            } else {
                nNumSelect = searchNumExist(2, nNumSelect);
                if (nNumSelect < 0) {
                    if (!m_buttonTrg) break;
                    nNumSelect = searchNumExist(6, m_nNumSelect - 1);
                }
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x204000, 0)) {
            nNumSelect = m_nNumSelect + 5;
            if (nNumSelect >= 10) {
                if (!m_buttonTrg) break;
                nNumSelect = searchNumExist(4, m_nNumSelect - 5);
            } else {
                nNumSelect = searchNumExist(4, nNumSelect);
                if (nNumSelect < 0) {
                    if (!m_buttonTrg) break;
                }
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x101000, 0)) {
            nNumSelect = m_nNumSelect - 5;
            if (nNumSelect < 0) {
                if (!m_buttonTrg) break;
                nNumSelect = searchNumExist(3, m_nNumSelect + 5);
            } else {
                nNumSelect = searchNumExist(3, nNumSelect);
                if (nNumSelect < 0) {
                    if (!m_buttonTrg) break;
                }
            }
            fgCursorMove = TRUE;
        }
        if (fgCursorMove) {
            if (nNumSelect < 0) break;
            if (nNumSelect == m_nNumSelect) break;
            m_nNumSelect = nNumSelect;
            CSound::playSE(0xE, -1, 0x40);
            changeCursor(1, nNumSelect);
        }
        if (m_nMenuStep == 5) {
            break;
        }
        m_pCardNumCtrl->replaceSeq(0, m_pSeqName, 0x27);
        changeCursor(0, m_nCardSelect);
        break;
    case 6:
        m_nMenuStep = 2;
        break;
    case 7:
        if (!m_pCardWinCtrl->isEnd(0)) {
            break;
        }
        for (INT i = 0; i < 4; i++) {
            delete m_pCardNorma[i];
            m_pCardNorma[i] = NULL;
            delete m_pCardNormaCtrl[i];
            m_pCardNormaCtrl[i] = NULL;
        }
        for (INT i = 0; i < 6; i++) {
            delete m_pCardWinHolder[i];
            m_pCardWinHolder[i] = NULL;
        }
        delete m_pScrollBar;
        m_pScrollBar = NULL;
        for (INT i = 0; i < 30; i++) {
            delete m_pCardWinCard[i];
            m_pCardWinCard[i] = NULL;
        }
        for (INT i = 0; i < 2; i++) {
            delete m_pExplainCtrl[i];
            m_pExplainCtrl[i] = NULL;
        }
        delete m_pCardWinInfo;
        m_pCardWinInfo = NULL;
        for (INT i = 0; i < 10; i++) {
            delete m_pCardNumHolder[i];
            m_pCardNumHolder[i] = NULL;
            delete m_pCardWinInfoNum[i];
            m_pCardWinInfoNum[i] = NULL;
        }
        delete m_pCardWinCtrl;
        m_pCardWinCtrl = NULL;
        delete m_pCardNumCtrl;
        m_pCardNumCtrl = NULL;
        delete m_pCardSampleCtrl;
        m_pCardSampleCtrl = NULL;
        delete m_pCardSample;
        m_pCardSample = NULL;
        delete m_pFntInstruction;
        m_pFntInstruction = NULL;
        m_nMenuStep++;
        break;
    default:
        return 1;
    }
    moveCursor();
    return 0;
}
#line 1206
void CRoomCreation::initInfoNum(FVECTOR& vec) {
    FVECTOR vec2;
    for (INT i = 0; i < 10; i++) {
        C2DSeqCtrl* pCtrl = m_pCardWinInfoNum[i] = new (__FILE__ "(1288)") C2DSeqCtrl;
        pCtrl->setLayer(12);
        pCtrl->addSeq("room01:00", i * 2 + 0x11);
        pCtrl->setParent(m_pCardWinCtrl);
        C2DMan::getLayPosition(m_pLadName, 2, i + 8, vec2, 0);
        pCtrl->setLocalPos(vec2);
    }
}
void CRoomCreation::selectDispInfoNum() {
    CARD card;
    INT idx;
    INT nExist = 0;
    char buf[16];
    static LPCTSTR pStr = "\x81\x7e\xf9" "D%d";
    if (m_nCardSelectMax) {
        card = m_pCardWinCard[m_nCardSelect]->m_card;
        for (idx = 0; idx < m_nCardKindMax; idx++) {
            if (m_nCardKind[idx] == card.m_nKind) break;
        }
    }
    for (INT i = 0; i < 10; i++) {
        INT fgExist = 1;
        if (m_nCardSelectMax) {
            fgExist = card.m_nCategory0 == 10 || card.m_nKind == 0xB9;
            if (!fgExist) {
                fgExist = m_nCardKind10[idx][i] == 0;
            }
            nExist += m_nCardKind10[idx][i];
        }
        m_pCardWinInfoNum[i]->replaceSeq(0, "room01:00", i * 2 + 0x10 + fgExist);
    }
    sprintf(buf, pStr, nExist);
    m_pCardVolFnt->allocText(m_nCardSelectMax ? buf : NULL);
}

void CRoomCreation::initCard() {
    CARD card;
    FVECTOR vec;
    vec.set(0.25f, 0.25f, 0.25f, 1.0f);
    if (m_pCardWinCard[0]) {
        for (INT i = 0; i < 30; i++) {
            delete m_pCardWinCard[i];
            m_pCardWinCard[i] = NULL;
        }
        m_nCardSelectMax = 0;
        m_nCardSelect = 0;
    }
    for (INT i = 0; i < m_nCardKindMax; i++) {
        INT cardKind = m_nCardKind[i];
        if (m_nCategorySelect == 0 || cardDataTbl[cardKind].nCategory1 - 8 == m_nCategorySelect) {
            _setCard(card, cardKind, -1);
            C2DCard* pCard = m_pCardWinCard[m_nCardSelectMax] = new (__FILE__ "(1355)") C2DCard;
            pCard->setLayer(12);
            pCard->enableColorCtrl(TRUE);
            pCard->create(card);
            if (CGameWork::getFlag(1, 7) && card.m_nKind != 0xB6) {
                pCard->setCol(vec);
            }
            pCard->setParent(m_pCardWinHolder[m_nCardSelectMax % 6]);
            pCard->enableDisp(FALSE);
            m_nCardSelectMax++;
        }
    }
    if (!m_pCardSampleCtrl) {
        m_pCardSampleCtrl = new (__FILE__ "(1370)") C2DSeqCtrl;
        vec.set(0.0f, -20.0f, 0.0f, 1.0f);
        m_pCardSampleCtrl->setLocalPos(vec);
        m_pCardSampleCtrl->addSeq("room01:00", 9);
        m_pCardSampleCtrl->setParent(m_pCardWinInfo);
        if (m_pCardWinCard[m_nCardSelect]) {
            m_pCardSample = new (__FILE__ "(1376)") C2DCard;
            m_pCardSample->setLayer(12);
            m_pCardSample->create(m_pCardWinCard[m_nCardSelect]->m_card);
            m_pCardSample->setParent(m_pCardSampleCtrl);
        }
    }
}
INT CRoomCreation::initCardCheck(INT nCategorySelect) {
    INT nCardSelectMax = 0;
    for (INT i = 0; i < m_nCardKindMax; i++) {
        INT cardKind = m_nCardKind[i];
        if (nCategorySelect == 0 || cardDataTbl[cardKind].nCategory1 - 8 == nCategorySelect) {
            nCardSelectMax++;
        }
    }
    return nCardSelectMax;
}
void CRoomCreation::selectDispCard() {
    for (INT i = 0; i < m_nCardSelectMax; i++) {
        if (m_nCardSelect / 6 != i / 6) {
            m_pCardWinCard[i]->enableDisp(FALSE);
        } else {
            m_pCardWinCard[i]->enableDisp(TRUE);
        }
    }
    INT nLine = m_nCardSelectMax / 6 + (m_nCardSelectMax % 6 != 0);
    INT nPos = m_nCardSelect / 6;
    if (nLine == 0) {
        nLine = 1;
    }
    FLOAT fRatioHeight = 1.0f / nLine;
    if (fRatioHeight > 1.0f) {
        fRatioHeight = 1.0f;
    }
    FLOAT fRatioPosY = (FLOAT)nPos / nLine;
    if (fRatioPosY > 1.0f) {
        fRatioPosY = 1.0f;
    }
    m_pScrollBar->change(fRatioPosY, fRatioHeight);
}
void CRoomCreation::initNumHolder(FVECTOR& vec) {
    char chrBuf[16];
    FVECTOR vec2;
    G_RGBA rgba;
    static LPCTSTR pStr = "\x81\x7e\xf9" "D%d";
    rgba.col = 0x80006060;
    vec2.init();
    for (INT i = 0; i < 10; i++) {
        C2DSeqCtrl* pCtrl = m_pCardNumHolder[i] = new (__FILE__ "(1462)") C2DSeqCtrl;
        pCtrl->setLayer(13);
        pCtrl->addSeq("room01:00", 0x24);
        pCtrl->setParent(m_pCardNumCtrl);
        pCtrl->enableColorCtrl(TRUE);
        C2DMan::getLayPosition(m_pLadName, 3, i + 1, vec2, 0);
        SubVectorXYZ(vec2, vec2, vec);
        pCtrl->setLocalPos(vec2);
        pCtrl->addSeqShift("room01:00", i * 2 + 0x3C, 10.0f, 24.0f);
        pCtrl->enableDisp(FALSE);
        sprintf(chrBuf, pStr, i);
        m_pCardNumFnt[i] = new (__FILE__ "(1474)") C2DFnt(0, 0, &rgba, 15, "", 0x40);
        m_pCardNumFnt[i]->allocText(chrBuf);
        pCtrl->addFntShift("room01:00", m_pCardNumFnt[i], 34.0f, 40.0f);
    }
}
void CRoomCreation::selectDispNumHolder() {
    INT rgb;
    INT idx;
    CARD card;
    INT nSeq = 0x24;
    BYTE category1 = m_pCardWinCard[m_nCardSelect]->m_card.m_nCategory1;
    if (category1 == 10) {
        nSeq = 0xE;
    } else if (category1 == 11) {
        nSeq = 0xC;
    }
    card = m_pCardWinCard[m_nCardSelect]->m_card;
    for (idx = 0; idx < m_nCardKindMax; idx++) {
        if (m_nCardKind[idx] == card.m_nKind) break;
    }
    for (INT i = 0; i < 10; i++) {
        C2DSeqCtrl* pCtrl = m_pCardNumHolder[i];
        INT nNum = m_nCardKind10[idx][i];
        pCtrl->replaceSeq(0, "room01:00", nNum ? nSeq + 1 : nSeq);
        pCtrl->replaceSeqShift(1, "room01:00", i * 2 + 0x3B + (nNum == 0), 10.0f, 24.0f);
        ((CHAR*)m_pCardNumFnt[i]->getText())[4] = nNum + '0';
        rgb = nNum ? 0x80006060 : 0x80202020;
        m_pCardNumFnt[i]->m_C0.col = rgb;
    }
}
INT CRoomCreation::searchNumExist(INT nMethod, INT idx) {
    INT index;
    CARD card = m_pCardWinCard[m_nCardSelect]->m_card;
    for (index = 0; index < m_nCardKindMax; index++) {
        if (m_nCardKind[index] == card.m_nKind) break;
    }
    BYTE* pNum10 = m_nCardKind10[index];
    switch (nMethod) {
    case 0:
        for (INT i = 0; i < 10; i++) {
            if (pNum10[i]) return i;
        }
        return -1;
    case 1:
        for (INT i = idx; idx < 5 ? i < 5 : i < 10; i++) {
            if (pNum10[i]) return i;
        }
        return -1;
    case 2:
        for (INT i = idx; idx >= 5 ? i >= 5 : i >= 0; i--) {
            if (pNum10[i]) return i;
        }
        return -1;
    case 3:
    case 4: {
        if (pNum10[idx]) return idx;
        INT nNearL = searchNumExist(1, idx);
        INT nNearS = searchNumExist(2, idx);
        if (nNearL < 0) {
            INT r = -1; if (nNearS > r) r = nNearS; return r;
        }
        if (nNearS < 0) {
            return nNearL;
        }
        if (nMethod == 3) {
            return nNearL - idx >= idx - nNearS ? nNearS : nNearL;
        }
        return idx - nNearS < nNearL - idx ? nNearS : nNearL;
    }
    case 5:
        for (INT i = 0; i < 10; i++) {
            INT j = (i + idx) % 10;
            if (pNum10[j]) return j;
        }
        return -1;
    case 6:
        for (INT i = 10; i > 0; i--) {
            INT j = (i + idx) % 10;
            if (pNum10[j]) return j;
        }
        return -1;
    }
    return -1;
}
void CRoomCreation::moveCursor() {
    if (m_fgCursorMove && m_pCursorCtrl) {
        m_fCursorTimer += getFrameRateF();
        if (m_fCursorTimer >= 10.0f) {
            m_fCursorTimer = 10.0f;
            m_fgCursorMove = 0;
        }
        FLOAT t = 1.0f - powf(1.0f - m_fCursorTimer / 10.0f, 2.5f);
        InterVectorXYZ(m_vPosCursorCurrent, m_vPosCursorTarget, m_vPosCursorOld, t);
        m_pCursorCtrl->setPos(m_vPosCursorCurrent);
    }
}
void CRoomCreation::changeCursor(INT nSel, INT nIdx) {
    static float cursorPosTbl0[12] = {
        150.0f, 291.0f, 210.0f, 291.0f, 270.0f, 291.0f, 330.0f, 291.0f, 390.0f, 291.0f, 450.0f, 291.0f,
    };
    static float cursorPosTbl1[20] = {
        161.0f, 279.0f, 235.0f, 279.0f, 309.0f, 279.0f, 383.0f, 279.0f, 457.0f, 279.0f,
        161.0f, 329.0f, 235.0f, 329.0f, 309.0f, 329.0f, 383.0f, 329.0f, 457.0f, 329.0f,
    };
    static LPCTSTR pCardSeqName = "card:00";
    FLOAT* fPos;
    if (!m_pCursorCtrl) {
        m_pCursorCtrl = new (__FILE__ "(1662)") C2DSeqCtrl;
        m_pCursorCtrl->setLayer(14);
        m_pCursorCtrl->addSeq(m_pSeqName, 0);
        m_pCursorCtrl->addSeqShift(m_pSeqName, 1, 0.0f, -32.0f);
        m_pCursorCtrl->addSeqShift(pCardSeqName, 7, 5.0f, 2.0f);
        m_pCursorCtrl->addSeqShift(m_pSeqName, 0x2B, -20.0f, -44.0f);
        m_vPosCursorCurrent.set(cursorPosTbl0[0], cursorPosTbl0[1], 0.0f, 1.0f);
        m_vPosCursorOld = m_vPosCursorTarget = m_vPosCursorCurrent;
        m_pCursorCtrl->setPos(m_vPosCursorCurrent);
        m_pCursorCtrl->enableDispEach(FALSE);
        m_fgCursorMove = 0;
        return;
    }
    if (nSel == 0) {
        if (m_nCardSelectMax) {
            m_pCursorCtrl->enableDisp(0, TRUE);
            m_pCursorCtrl->enableDisp(1, FALSE);
            m_pCursorCtrl->enableDisp(3, TRUE);
            INT rgb = 0;
            BYTE category1 = m_pCardWinCard[nIdx]->m_card.m_nCategory1;
            INT kind = m_pCardWinCard[nIdx]->m_card.m_nKind;
            if (category1 == 9) {
                rgb = 7;
            } else if (category1 == 10) {
                rgb = 5;
            } else if (kind == 0xB9) {
                rgb = 0x49;
            } else if (category1 == 11) {
                rgb = 3;
            }
            if (rgb) {
                m_pCursorCtrl->enableDisp(2, TRUE);
                m_pCursorCtrl->replaceSeqShift(2, pCardSeqName, rgb, 5.0f, 2.0f);
            } else {
                m_pCursorCtrl->enableDisp(2, FALSE);
            }
            nIdx %= 6;
            m_pRoomNameFnt->setText(m_pCardWinCard[m_nCardSelect]->m_card.getName());
            selectDispInfoNum();
            m_pCardSample->replace(m_pCardWinCard[m_nCardSelect]->m_card);
            m_pCardSample->enableDisp(TRUE);
            RCRoomInfo* pRCRoomInfo = getRoomInfo(m_pCardWinCard[m_nCardSelect]->m_card.m_nKind);
            if (pRCRoomInfo) {
                UINT ctdID = pRCRoomInfo->nRoomExplane;
                m_pExplainFnt[0]->allocText(CCtdMan::getText(ctdID));
                LPTSTR pText = (LPTSTR)m_pExplainFnt[0]->getText();
                if (pText) {
                    LPTSTR pLF = strchr(pText, '\n');
                    if (pLF) {
                        *pLF = '\0';
                        strcat(pText, pLF + 1);
                    }
                }
                m_pCardWinInfo->replaceSeq(1, m_pSeqName, pRCRoomInfo->nEnemyVolume + 0x6E);
                m_pCardWinInfo->replaceSeq(2, m_pSeqName, pRCRoomInfo->nRoomVolume + 0x74);
                m_pCardWinInfo->enableDisp(1, TRUE);
                m_pCardWinInfo->enableDisp(2, TRUE);
            }
        } else {
            m_pCursorCtrl->enableDisp(0, FALSE);
            m_pCursorCtrl->enableDisp(1, FALSE);
            m_pCursorCtrl->enableDisp(2, FALSE);
            m_pCursorCtrl->enableDisp(3, FALSE);
            m_pRoomNameFnt->setText("");
            selectDispInfoNum();
            if (m_pCardSample) {
                m_pCardSample->enableDisp(FALSE);
            }
            m_pExplainFnt[0]->setText("");
            m_pCardWinInfo->enableDisp(1, FALSE);
            m_pCardWinInfo->enableDisp(2, FALSE);
            if (nIdx < 0) return;
        }
    } else {
        if (nIdx < 0) {
            m_pCursorCtrl->enableDispEach(FALSE);
            return;
        }
        m_pCursorCtrl->enableDisp(0, -1);
        m_pCursorCtrl->enableDisp(1, TRUE);
        m_pCursorCtrl->enableDisp(2, FALSE);
        m_pCursorCtrl->enableDisp(3, TRUE);
    }
    m_vPosCursorOld = m_vPosCursorCurrent;
    fPos = (nSel ? cursorPosTbl1 : cursorPosTbl0) + nIdx * 2;
    m_vPosCursorTarget.set(fPos[0], fPos[1], 0.0f, 1.0f);
    m_fgCursorMove = 1;
    m_fCursorTimer = 0.0f;
}
void CRoomCreation::disp() {
    if (m_pFntInstruction) {
        m_pFntInstruction->disp();
    }
}
RCRoomInfo* CRoomCreation::getRoomInfo(USHORT kind) {
    static RCRoomInfo roomCreationeRoomInfo[29] = {
        { 1, 2, 0x400 }, { 3, 3, 0x401 }, { 1, 2, 0x402 }, { 2, 2, 0x403 }, { 1, 1, 0x404 }, { 2, 3, 0x405 },
        { 2, 2, 0x406 }, { 1, 3, 0x407 }, { 1, 1, 0x408 }, { 3, 3, 0x409 }, { 2, 3, 0x40A }, { 2, 2, 0x40B },
        { 2, 2, 0x40C }, { 1, 2, 0x40D }, { 2, 2, 0x40E }, { 1, 2, 0x40F }, { 2, 3, 0x410 }, { 2, 3, 0x411 },
        { 0, 1, 0x412 }, { 2, 1, 0x413 }, { 1, 1, 0x414 }, { 0, 1, 0x415 }, { 4, 4, 0x416 }, { 0, 1, 0x417 },
        { 4, 4, 0x418 }, { 5, 5, 0x419 }, { 5, 5, 0x41A }, { 5, 5, 0x41B }, { 5, 5, 0x41C },
    };
    if (kind < 0xA1 || kind > 0xBD) {
        return NULL;
    }
    return &roomCreationeRoomInfo[kind - 0xA1];
}
LPCTSTR CRoomCreation::getNormaInfo() {
    static char* wordEtc[8] = {
        "\xf9" "YCriteria:\xf9" "A\xf9" "D %s",
        "\xf9" "YCriteria:\xf9" "A\xf9" "D ",
        "ard",
        "ard with the number %d or lower",
        "ard with the number %d or higher",
        "ard with the number %d",
        "ards with numbers totalling %d or higher",
        " or 0",
    };
    static char* wordColor[8] = {
        "A c", "A red c", "A green c", "A blue c", "C", "Red c", "Green c", "Blue c",
    };
    char buf[0x60];
    CARD card;
    RCNORMA::NormaSrc normaSrc;
    if (!m_pDoorInfo) {
        return NULL;
    }
    if (m_fgKeyCard && m_nNorma == 1) {
        _setCard(card, m_fgKeyCard, 0);
        sprintf(m_normaInfoBuf, wordEtc[0], card.getName());
    } else {
        strcpy(m_normaInfoBuf, wordEtc[1]);
        normaSrc._nColor = m_pDoorInfo->getNormaColor(m_idxNorma);
        normaSrc._nNumber = m_pDoorInfo->getNormaNumber(m_idxNorma);
        normaSrc._nOp = m_pDoorInfo->getNormaOperation(m_idxNorma);
        strcat(m_normaInfoBuf, wordColor[normaSrc._nColor + (normaSrc._nOp == RCNORMA_OP_TOTAL ? 4 : 0)]);
        if (normaSrc._nOp == RCNORMA_OP_NONE) {
            strcat(m_normaInfoBuf, wordEtc[2]);
        } else {
            sprintf(buf, wordEtc[normaSrc._nOp + 2], normaSrc._nNumber);
            strcat(m_normaInfoBuf, buf);
            if (normaSrc._nOp == RCNORMA_OP_EQ_GREATER && normaSrc._nNumber && !m_fgKeyCard) {
                strcat(m_normaInfoBuf, wordEtc[7]);
            }
        }
    }
    return m_normaInfoBuf;
}
// SOURCE DATA: .data .rodata
