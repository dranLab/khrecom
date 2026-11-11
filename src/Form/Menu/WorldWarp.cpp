
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Graphic/Gprim.h"
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

#include "Game/Status.h"
#include "Game/Technic.h"
#include "System/Graphic/Model.h"
#include "System/Ctd/CtdMan.h"
#include "Game/CardKind.h"
#include "Form/Menu/WorldWarp.h"
#include "Form/Menu/MenuParts.h"
#include "System/HeapMan.h"
#include "System/MainApp.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DSeqObj.h"
#include "System/Graphic/2d/2DFnt.h"
#include "System/Graphic/2d/2DCard.h"
#include "Game/SceneEnv.h"
#include "Game/SceneInfo.h"
#include "Game/Game.h"
#include "Game/GameWork.h"

#line 262



























#include "System/RsrcDB.h"





#include "System/Rcf.h"





#include "System/Graphic/PackMan.h"




#include "System/Sound/SoundMan.h"
#line 314

extern "C" {
float powf(float, float);
}

using namespace D2;

/* Stand-in: in menuMain the original keeps m_pWinConfirm in $v1 next to CCtdMan::getText's instance check (the
 * local allocator gave the instance pointer $v0 first); the construct that did it is not known. Pinning the
 * pointer to $3 through an empty asm reproduces the allocation and emits nothing. */
#ifdef KH_RETAIL
#define PIN_V1(p) (p) /* retail: the plain pointer matches, only the debug build needs the pin */
#else
#define PIN_V1(p) ({ register typeof(p) _p asm("$3") = (p); __asm__("" : "+r"(_p)); _p; })
#endif

#line 333

CWorldWarp::CWorldWarp(CTask* p, BOOL bWarpOrCard) : CMenu(p, -1) {
    m_pLadName = "wwarp.lad";
    m_pSeqName = "wwarp:00";
    for (INT i = 0; i < 12; i++) {
        m_pRouteCtrl[i] = NULL;
    }
    for (INT i = 0; i < 13; i++) {
        m_pCardCtrl[i] = NULL;
    }
    m_pCardDistCtrl = NULL;
    m_pCardSrcCtrl = NULL;
    m_pWinConfirm = NULL;
    for (INT i = 0; i < 2; i++) {
        m_pCursorCtrl[i] = NULL;
        m_pLightCtrl[i] = NULL;
        m_pButtonYenNo[i] = NULL;
    }
    m_bWarpOrCard = bWarpOrCard;
    m_pWorldCard = NULL;
    m_pHelpButton = NULL;
    m_bPlayerKind = CGame::isRikuGame();
    m_fgLightMove[1] = 0;
    m_fgLightMove[0] = 0;
    m_nSelYesNo = 1;
    INT n = CGameWork::getInt(0, 0x35);
    m_nMaxFloor = !m_bPlayerKind ? n - 1 : n + 12;
    m_bPooClear = (CGameWork::getInt(0, 0x23) >> 6) & 1;
    m_nCardLevel = CGameWork::getByte(0, 0xB);
    if (!m_nCardLevel) {
        m_nCardLevel = m_nMaxFloor;
    }
    if (!m_bWarpOrCard) {
        CGame::enablePause(FALSE);
        CSceneEnv::setStop((ObjType::eObjBit)0xFFFEFFFF);
        CSceneEnv::setStopGauge(TRUE);
    }
}

CWorldWarp::~CWorldWarp() {
    dispose();
    if (!m_bWarpOrCard) {
        CGame::enablePause(TRUE);
        if (m_nSelYesNo) {
            CSceneEnv::setStop((ObjType::eObjBit)0);
        }
        CSceneEnv::setStopGauge(FALSE);
        C2DMan::changeLevel(C2DMan::D2MAN_LEVEL0);
        CSound::startAllPausedSE();
        CPackMan::changeDrawMode(0);
    }
}

void CWorldWarp::dispose() {
    for (INT i = 0; i < 12; i++) {
        delete m_pRouteCtrl[i];
        m_pRouteCtrl[i] = NULL;
    }
    for (INT i = 0; i < 13; i++) {
        delete m_pCardCtrl[i];
        m_pCardCtrl[i] = NULL;
    }
    delete m_pCardSrcCtrl;
    m_pCardSrcCtrl = NULL;
    delete m_pCardDistCtrl;
    m_pCardDistCtrl = NULL;
    delete m_pWinConfirm;
    m_pWinConfirm = NULL;
    for (INT i = 0; i < 2; i++) {
        delete m_pCursorCtrl[i];
        m_pCursorCtrl[i] = NULL;
        delete m_pLightCtrl[i];
        m_pLightCtrl[i] = NULL;
        delete m_pButtonYenNo[i];
        m_pButtonYenNo[i] = NULL;
    }
    delete m_pWorldCard;
    m_pWorldCard = NULL;
    delete m_pHelpButton;
    m_pHelpButton = NULL;
    CRsrcDB::removeGroup(2, 0x6C);
}

void CWorldWarp::main() {
    FVECTOR vec;
    FVECTOR vec2;
    switch (m_nStep) {
    case 0:
        if (!m_bWarpOrCard) {
            CSound::pauseAllSE();
            C2DMan::changeLevel(C2DMan::D2MAN_LEVEL1);
            CPackMan::changeDrawMode(1);
        }
        m_nMenuStep = 0;
        m_nStep++;
        if (!CRsrcDB::getRsrcTex(m_pSeqName, NAME_CASE_SENSITIVE)) {
            CRcf::reqLoadRCFFront(2, 0x6C, 0, NULL, 0);
            m_nStep = 1;
            break;
        }
        m_nStep = 2;
        break;
    case 1:
        if (CRcf::isCompleteLoadRCF(2, 0x6C)) {
            m_nStep = 2;
        }
        break;
    case 2:
        m_nStep = 3;
        break;
    case 3:
        m_hLay = C2DMan::create(m_pLadName, !m_bWarpOrCard ? 3 : 6);
        CSound::playSE(3, -1, 0x40);
        m_nStatusLad = 0;
        m_nStep++;
        break;
    case 4:
        if (!m_nStatusLad && C2DMan::isEnd(m_hLay)) {
            if (!m_bWarpOrCard) {
                CSound::setBGMMasterVolume(0x40, -1);
            }
            C2DMan::destroy(m_hLay);
            m_hLay = C2DMan::create(m_pLadName, !m_bWarpOrCard ? 4 : 7);
            m_nStatusLad = 1;
        }
        if ((m_nStatusLad != 0) == TRUE) {
            m_nStep = 5;
        }
        break;
    case 5:
        if (menuMain()) {
            m_nStep = 6;
        }
        break;
    case 6:
        m_nStep = 7;
        break;
    case 7:
        if (!m_bWarpOrCard) {
            CSound::setBGMMasterVolume(-1, -1);
        }
        C2DMan::destroy(m_hLay);
        m_hLay = C2DMan::create(m_pLadName, !m_bWarpOrCard ? 5 : 8);
        m_nStep++;
        break;
    case 8:
        if (C2DMan::isEnd(m_hLay)) {
            C2DMan::destroy(m_hLay);
            if (!m_nSelYesNo) {
                CGameWork::setFlag(1, 0x380, TRUE);
                CSceneInfo::reqChangeFloor(CSceneInfo::getFloorNumber(m_idxDistWorld, m_bPlayerKind ^ 1), TRUE);
                CMainApp::setFpsHalf();
                m_nStep = 9;
            } else {
                m_nStep = 16;
            }
            dispose();
        }
        break;
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        m_nStep++;
        break;
    case 16:
    default:
        out();
        break;
    }
}

INT CWorldWarp::menuMain() {
    INT ret = 0;
    CARD card;
    FVECTOR vec;
    FVECTOR vec2;
    FVECTOR vScale;
    C2DSeqCtrl* pSeqCtrlTmp;
    G_RGBA rgba;
    static LPCTSTR pSeqName2 = "wwarp:01";
    static LPCTSTR pNullName = "";
    LPCTSTR pCardName;
    INT nSelectAdd = 0;
    INT nSeq;
    static USHORT winConfirmSeqTbl[3] = { 4, 5, 6 };
    static USHORT buttonYesNoSeqTbl[4] = { 1, 2, 3, 0 };
    static USHORT winInfoSeqTbl[3] = { 101, 102, 103 };
    static USHORT warpHelpSeqTbl[3] = { 94, 95, 96 };
    static USHORT cardHelpSeqTbl[3] = { 91, 92, 93 };

    switch (m_nMenuStep) {
    case 0:
        initWorldIdx();
        m_nMenuStep = 1;
        break;
    case 1:
        if (m_idxWorld[0] < 0) {
            m_nMenuStep = 3;
            break;
        }
        for (INT i = 0; i < 12 && m_idxWorld[i + 1] >= 0 && (!m_bWarpOrCard || i < m_nMaxFloorWorld); i++) {
#line 324
            pSeqCtrlTmp = m_pRouteCtrl[i] = new KH_AT("(324)") C2DSeqCtrl;
            pSeqCtrlTmp->addSeq(m_pSeqName, (i & 3) != 3 ? 0x44 : (i != 7 ? 0x47 : 0x4A));
            C2DMan::getLayPosition(m_pLadName, 9, i + 3, vec, 0);
            pSeqCtrlTmp->setPos(vec);
        }
        for (INT i = 0; i < 13 && m_idxWorld[i] >= 0; i++) {
#line 334
            pSeqCtrlTmp = m_pCardCtrl[i] = new KH_AT("(334)") C2DSeqCtrl;
            pSeqCtrlTmp->enableColorCtrl(TRUE);
            pSeqCtrlTmp->setLayer(13);
            nSeq = (m_bWarpOrCard && m_nMaxFloorWorld < i) ? 0x53 : (i != m_idxSrcWorld ? 0x15 : 0x19);
            pSeqCtrlTmp->addSeq(m_pSeqName, nSeq);
            if (!(m_bWarpOrCard && m_nMaxFloorWorld < i)) {
                nSeq = !m_bPlayerKind ? i + 0x2E : 0x2D - i;
                pSeqCtrlTmp->addSeqShift(m_pSeqName, nSeq, 28.0f, -7.0f);
            }
            pSeqCtrlTmp->addSeqShift(pSeqName2, m_idxIcon[i], 5.0f, -20.0f);
            C2DMan::getLayPosition(m_pLadName, 9, i + 15, vec, 0);
            pSeqCtrlTmp->setPos(vec);
        }
        rgba.col = 0x807C7C7C;
        if (!m_bWarpOrCard) {
#line 354
            m_pCardSrcCtrl = new KH_AT("(354)") C2DSeqCtrl;
            m_pCardSrcCtrl->enableColorCtrl(TRUE);
            m_pCardSrcCtrl->addSeq(m_pSeqName, 0x1F);
            nSeq = !m_bPlayerKind ? m_idxSrcWorld + 0x2E : 0x2D - m_idxSrcWorld;
            m_pCardSrcCtrl->addSeqShift(m_pSeqName, nSeq, -14.0f, -14.0f);
            m_pCardSrcCtrl->addSeqShift(pSeqName2, m_idxIcon[m_idxSrcWorld], -51.0f, -10.0f);
            m_pCardSrcCtrl->addSeqShift(m_pSeqName, 0xC, -10.0f, 19.0f);
            C2DMan::getLayPosition(m_pLadName, 9, 0x1D, vec, 0);
            m_pCardSrcCtrl->setPos(vec);
            if (m_idxWorld[m_idxSrcWorld] < 14) {
                pCardName = cardDataTbl[m_idxWorld[m_idxSrcWorld] + 0xBE].pName;
            } else {
                pCardName = pNullName;
            }
#line 368
            m_pCardSrcFnt[0] = new KH_AT("(368)") C2DFnt(0, 0, &rgba, 15, pCardName, 0x240);
            m_pCardSrcCtrl->addFntShift(m_pSeqName, m_pCardSrcFnt[0], -14.0f, 15.0f);
#line 373
            m_pCardSrcFnt[1] = new KH_AT("(373)") C2DFnt(0, 0, &rgba, 13, CCtdMan::getText(0x900000), 0x240);
            m_pCardSrcCtrl->addFntShift(m_pSeqName, m_pCardSrcFnt[1], -138.0f, 0.0f);
        }
#line 382
        m_pCardDistCtrl = new KH_AT("(382)") C2DSeqCtrl;
        m_pCardDistCtrl->setLayer(16);
        m_pCardDistCtrl->enableColorCtrl(TRUE);
        m_pCardDistCtrl->addSeq(m_pSeqName, !m_bWarpOrCard ? 0x1C : 0x56);
        nSeq = !m_bPlayerKind ? m_idxDistWorld + 0x2E : 0x2D - m_idxDistWorld;
        m_pCardDistCtrl->addSeqShift(m_pSeqName, nSeq, -14.0f, -14.0f);
        m_pCardDistCtrl->addSeqShift(pSeqName2, m_idxIcon[m_idxSrcWorld], -51.0f, -10.0f);
        C2DMan::getLayPosition(m_pLadName, 9, 0x1E - m_bWarpOrCard, vec, 0);
        m_pCardDistCtrl->setPos(vec);
        if (m_bWarpOrCard && m_idxDistWorld > m_nMaxFloorWorld) {
            m_pCardDistCtrl->enableDisp(1, FALSE);
        }
        if (m_idxWorld[m_idxSrcWorld] < 14) {
            pCardName = cardDataTbl[m_idxWorld[m_idxSrcWorld] + 0xBE].pName;
        } else {
            pCardName = pNullName;
        }
#line 399
        m_pCardDistFnt[0] = new KH_AT("(399)") C2DFnt(0, 0, &rgba, 15, pCardName, 0x240);
        m_pCardDistCtrl->addFntShift(m_pSeqName, m_pCardDistFnt[0], -14.0f, 15.0f);
        if (!m_bWarpOrCard) {
#line 405
            m_pCardDistFnt[1] = new KH_AT("(405)") C2DFnt(0, 0, &rgba, 13, CCtdMan::getText(0x900001), 0x240);
            m_pCardDistCtrl->addFntShift(m_pSeqName, m_pCardDistFnt[1], -138.0f, 0.0f);
        }
#line 414
        m_pHelpButton = new KH_AT("(414)") CButton;
        C2DMan::getLayPosition(m_pLadName, 10, 0x1C, vec, 0);
        vec.x -= 256.0f;
        vec.y -= 192.0f;
        m_pHelpButton->init(m_pSeqName, 12, !m_bWarpOrCard ? warpHelpSeqTbl : cardHelpSeqTbl, vec, NULL,
                            CCtdMan::getText(0x900002 + m_bWarpOrCard), 0x10, 0.0f, 0.0f, 0x40, NULL, -1);
        m_nMenuStep = 2;
        break;
    case 2:
        m_pHelpButton->checkEndAfterSet(CButton::BS_LOOP);
        if (m_pCardDistCtrl->isEnd(0)) {
            for (INT i = 0; i < 12; i++) {
                if (m_pRouteCtrl[i]) {
                    m_pRouteCtrl[i]->replaceSeq(0, m_pSeqName, (i & 3) != 3 ? 0x45 : (i != 7 ? 0x48 : 0x4B));
                }
            }
            for (INT i = 0; i < 13; i++) {
                if (m_pCardCtrl[i]) {
                    nSeq = (m_bWarpOrCard && m_nMaxFloorWorld < i) ? 0x54 : (i != m_idxSrcWorld ? 0x16 : 0x1A);
                    m_pCardCtrl[i]->replaceSeq(0, m_pSeqName, nSeq);
                }
            }
            if (m_pCardSrcCtrl) {
                m_pCardSrcCtrl->replaceSeq(0, m_pSeqName, 0x20);
            }
            m_pCardDistCtrl->replaceSeq(0, m_pSeqName, !m_bWarpOrCard ? 0x1D : 0x57);
            m_pCardCtrl[m_idxSrcWorld]->replaceSeq(0, m_pSeqName, 0x18);
            m_fgLightMove[0] = -1;
            m_nMenuStep = 3;
        }
        break;
    case 3:
        if (_isTrgOK(0)) {
            if (m_idxWorld[0] < 0) {
                break;
            }
            if (!m_bWarpOrCard) {
                if (m_idxSrcWorld == m_idxDistWorld) {
                    CSound::playSE(5, -1, 0x40);
                    break;
                }
                if (m_pWinConfirm == NULL) {
                    vec.set(0.0f, 64.0f, 0.0f);
#line 475
                    m_pWinConfirm = new KH_AT("(475)") CButton;
                    m_pWinConfirm->init(m_pSeqName, 15, winConfirmSeqTbl, vec, NULL, CCtdMan::getText(0x900004), 0x10, 0.0f, -20.0f, 0x242, NULL, -1);
                    m_pWinConfirm->get2DFnt()->setSpace(0, 0x40);
#line 481
                    m_pButtonYenNo[0] = new KH_AT("(481)") CButton;
                    C2DMan::getLayPosition(m_pLadName, 0, 1, vec, 0);
                    m_pButtonYenNo[0]->init(m_pSeqName, 15, buttonYesNoSeqTbl, vec, PIN_V1(m_pWinConfirm)->get2DSeqCtrl(), CCtdMan::getText(0x900005), 0x10, 0.0f, 9.0f, 0x242, NULL, -1);
#line 487
                    m_pButtonYenNo[1] = new KH_AT("(487)") CButton;
                    C2DMan::getLayPosition(m_pLadName, 0, 2, vec, 0);
                    m_pButtonYenNo[1]->init(m_pSeqName, 15, buttonYesNoSeqTbl, vec, PIN_V1(m_pWinConfirm)->get2DSeqCtrl(), CCtdMan::getText(0x900006), 0x10, 0.0f, 9.0f, 0x242, NULL, -1);
                } else {
                    m_pWinConfirm->setSequence(CButton::BS_IN, FALSE);
                    m_pButtonYenNo[0]->setSequence(CButton::BS_IN, FALSE);
                    m_pButtonYenNo[1]->setSequence(CButton::BS_IN, FALSE);
                }
                m_fgLightMove[0] = -2;
                m_nMenuStep = 4;
            } else {
                INT nCtdId;
                if (m_pWinConfirm == NULL) {
                    C2DMan::getLayPosition(m_pLadName, 10, 0x1D, vec, 0);
#line 507
                    m_pWinConfirm = new KH_AT("(507)") CButton;
                    vec.x -= 256.0f;
                    vec.y -= 192.0f;
                    m_pWinConfirm->init(m_pSeqName, 15, winInfoSeqTbl, vec, NULL, "", 0xE, -47.0f, -2.0f, 0x40, NULL, -1);
                    m_pWinConfirm->get2DFnt()->setSpace(-2, 0x40);
                } else {
                    m_pWinConfirm->setSequence(CButton::BS_IN, TRUE);
                }
                delete m_pWorldCard;
                m_pWorldCard = NULL;
#line 523
                m_pWorldCard = new KH_AT("(523)") C2DCard;
                m_pWorldCard->setLayer(16);
                m_pWorldCard->enableColorCtrl(TRUE);
                m_pWorldCard->setParent(m_pWinConfirm->get2DSeqCtrl());
                m_pWorldCard->addSeqShift(m_pSeqName, 100, -103.0f, 18.0f);
                card.init(m_idxWorld[m_idxDistWorld] + 0xBE, 0xFF);
                m_pWorldCard->create(card);
                if (card.m_nKind == 0xC7 && (m_bPooClear & 1)) {
                    G_UV start;
                    G_UV end;
                    start.set(0x42, 0x41);
                    end.set(0x7E, 0x7F);
                    m_pWorldCard->changeTexture(1, 0, NULL, &start, &end);
                }
                nCtdId = (m_bPlayerKind && m_idxWorld[m_idxDistWorld] == 12) ? 0x430 : m_idxWorld[m_idxDistWorld] + 0x41D;
                m_pWinConfirm->changeFont(CCtdMan::getText(nCtdId));
                m_pCardDistCtrl->replaceSeq(0, m_pSeqName, 0x58);
                m_fgLightMove[0] = -2;
                m_nMenuStep = 7;
            }
            CSound::playSE(2, -1, 0x40);
        } else if (_isTrgCANCEL(0)) {
            if (m_idxWorld[0] >= 0) {
                m_nMenuStep = 11;
            } else {
                m_nMenuStep = 13;
            }
            CSound::playSE(4, -1, 0x40);
        } else if (_isButtonRpt(0x802000, 0)) {
            nSelectAdd = 1;
        } else if (_isButtonRpt(0x408000, 0)) {
            nSelectAdd = -1;
        } else if (_isButtonRpt(0x204000, 0)) {
            nSelectAdd = -4;
        } else if (_isButtonRpt(0x101000, 0)) {
            nSelectAdd = 4;
        }
        if (nSelectAdd && searchSelect(nSelectAdd) && m_idxWorld[0] >= 0) {
            CSound::playSE(1, -1, 0x40);
            m_fgLightMove[0] = -1;
        }
        break;
    case 4:
        if (m_pWinConfirm->checkEndAfterSet(CButton::BS_LOOP)) {
            m_pButtonYenNo[0]->setSequence(CButton::BS_LOOP, FALSE);
            m_pButtonYenNo[1]->setSequence(CButton::BS_ACTIVE, FALSE);
            m_nSelYesNo = 1;
            m_fgLightMove[1] = -1;
            m_nMenuStep = 5;
        }
        break;
    case 5:
        if (_isTrgOK(0) || _isTrgCANCEL(0)) {
            if (_isTrgCANCEL(0)) {
                m_nSelYesNo = 1;
                CSound::playSE(4, -1, 0x40);
            } else {
                CSound::playSE(2, -1, 0x40);
            }
            m_pWinConfirm->setSequence(CButton::BS_OUT, FALSE);
            m_pButtonYenNo[0]->setSequence(CButton::BS_OUT, FALSE);
            m_pButtonYenNo[1]->setSequence(CButton::BS_OUT, FALSE);
            m_fgLightMove[1] = -4;
            m_nMenuStep = 6;
        } else if (_isButtonRpt(0xC0A000, 0)) {
            m_nSelYesNo ^= 1;
            if (m_nSelYesNo == 0) {
                m_pButtonYenNo[0]->setSequence(CButton::BS_ACTIVE, FALSE);
                m_pButtonYenNo[1]->setSequence(CButton::BS_LOOP, FALSE);
            } else {
                m_pButtonYenNo[0]->setSequence(CButton::BS_LOOP, FALSE);
                m_pButtonYenNo[1]->setSequence(CButton::BS_ACTIVE, FALSE);
            }
            m_fgLightMove[1] = -1;
            CSound::playSE(1, -1, 0x40);
        }
        break;
    case 6:
        if (m_pWinConfirm->checkEndAfterSet(CButton::BS_NON)) {
            if (m_nSelYesNo == 0) {
                m_nMenuStep = 10;
            } else {
                m_fgLightMove[0] = -3;
                m_fgLightMove[1] = -5;
                m_nMenuStep = 3;
            }
        }
        break;
    case 7:
        if (m_pWinConfirm->checkEndAfterSet(CButton::BS_LOOP)) {
            m_nMenuStep = 8;
        }
        break;
    case 8:
        if (_isTrgOK(0) || _isTrgCANCEL(0)) {
            m_pWinConfirm->setSequence(CButton::BS_OUT, FALSE);
            m_pCardDistCtrl->replaceSeq(0, m_pSeqName, 0x59);
            CSound::playSE(4, -1, 0x40);
            m_nMenuStep = 9;
        }
        break;
    case 9:
        if (m_pWinConfirm->checkEndAfterSet(CButton::BS_DISPOFF)) {
            m_fgLightMove[0] = -3;
            m_pCardDistCtrl->replaceSeq(0, m_pSeqName, 0x57);
            m_nMenuStep = 3;
        }
        break;
    case 10:
        m_nMenuStep = 11;
        break;
    case 11:
        for (INT i = 0; i < 12; i++) {
            if (m_pRouteCtrl[i]) {
                m_pRouteCtrl[i]->replaceSeq(0, m_pSeqName, (i & 3) != 3 ? 0x46 : (i != 7 ? 0x49 : 0x4C));
            }
        }
        for (INT i = 0; i < 13; i++) {
            if (m_pCardCtrl[i]) {
                nSeq = (m_bWarpOrCard && m_nMaxFloorWorld < i) ? 0x55 : (i != m_idxSrcWorld ? 0x17 : 0x1B);
                m_pCardCtrl[i]->replaceSeq(0, m_pSeqName, nSeq);
            }
        }
        if (m_pCardSrcCtrl) {
            m_pCardSrcCtrl->replaceSeq(0, m_pSeqName, 0x21);
        }
        m_pCardDistCtrl->replaceSeq(0, m_pSeqName, !m_bWarpOrCard ? 0x1E : 0x5A);
        m_fgLightMove[0] = -4;
        m_pHelpButton->setSequence(CButton::BS_OUT, FALSE);
        m_nMenuStep = 12;
        break;
    case 12:
        m_pHelpButton->checkEndAfterSet(CButton::BS_DISPOFF);
        if (m_pCardDistCtrl->isEnd(0)) {
            m_nMenuStep = 13;
        }
        break;
    case 13:
        ret = 1;
        break;
    }
    moveCursorAndLight(0, (FLOAT)getFrameRate() * 0.0200000014f);
    moveCursorAndLight(1, (FLOAT)getFrameRate() * 0.0200000014f);
    return ret;
}

void CWorldWarp::moveCursorAndLight(INT id, FLOAT fFrameRate) {
    static LPCTSTR pSeqName2 = "wwarp:01";
    static LPCTSTR pNullName = "";
    LPCTSTR pCardName;
    FVECTOR vec;
    C2DSeqCtrl* pSeqCtrlTmp;
    INT nSeq;

    if (m_fgLightMove[id] == 0) {
        return;
    }
    if (m_fgLightMove[id] == -1) {
        if (id == 0) {
            nSeq = (m_bWarpOrCard && m_idxDistWorldOld > m_nMaxFloorWorld) ? 0x54
                : (m_idxDistWorldOld != m_idxSrcWorld ? 0x16 : 0x1A);
            m_pCardCtrl[m_idxDistWorldOld]->replaceSeq(0, m_pSeqName, nSeq);
            nSeq = (m_bWarpOrCard && m_idxDistWorld > m_nMaxFloorWorld) ? 0x52
                : (m_idxDistWorld != m_idxSrcWorld ? 0x14 : 0x18);
            m_pCardCtrl[m_idxDistWorld]->replaceSeq(0, m_pSeqName, nSeq);
            nSeq = (USHORT)(!m_bPlayerKind ? m_idxDistWorld + 0x2E : 0x2D - m_idxDistWorld);
            m_pCardDistCtrl->replaceSeqShift(1, m_pSeqName, nSeq, -14.0f, -14.0f);
            if (m_bWarpOrCard && m_idxDistWorld > m_nMaxFloorWorld) {
                m_pCardDistCtrl->enableDisp(1, FALSE);
            } else {
                m_pCardDistCtrl->enableDisp(1, TRUE);
            }
            m_pCardDistCtrl->replaceSeqShift(2, pSeqName2, m_idxIcon[m_idxDistWorld], -51.0f, -10.0f);
            if (m_idxWorld[m_idxDistWorld] < 14) {
                pCardName = cardDataTbl[m_idxWorld[m_idxDistWorld] + 0xBE].pName;
            } else {
                pCardName = pNullName;
            }
            m_pCardDistFnt[0]->setText(pCardName);
        }
        if (id == 0) {
            pSeqCtrlTmp = m_pCardCtrl[m_idxDistWorld];
        } else {
            pSeqCtrlTmp = m_pButtonYenNo[m_nSelYesNo]->get2DSeqCtrl();
        }
        if (m_pCursorCtrl[id] == NULL) {
#line 762
            m_pCursorCtrl[id] = new KH_AT("(762)") C2DSeqCtrl;
            m_pCursorCtrl[id]->setLayer(id + 15);
            m_pCursorCtrl[id]->addSeqShift(m_pSeqName, 14, !id ? -8.0f : -48.0f, -4.0f);
        }
        m_pCursorCtrl[id]->setParent(pSeqCtrlTmp);
        vec = pSeqCtrlTmp->m_seqControl.vPos;
        vec.x += !id ? 64.0f : 90.0f;
        vec.y += !id ? -12.0f : 60.0f;
        m_vPosLightTarget[id] = vec;
        if (m_pLightCtrl[id] == NULL) {
#line 775
            m_pLightCtrl[id] = new KH_AT("(775)") C2DSeqCtrl;
            m_pLightCtrl[id]->setLayer(id + 15);
            m_pLightCtrl[id]->addSeq(m_pSeqName, 0x11);
            m_vPosLightCurrent[id] = m_vPosLightTarget[id];
        }
        m_vPosLightOld[id] = m_vPosLightCurrent[id];
        m_fLightTimer[id] = 0.0f;
    } else if (m_fgLightMove[id] == -2) {
        m_pCursorCtrl[id]->replaceSeqShift(0, m_pSeqName, 0xD, -8.0f, -4.0f);
        m_pLightCtrl[id]->replaceSeq(0, m_pSeqName, 0x12);
    } else if (m_fgLightMove[id] == -3) {
        m_pCursorCtrl[id]->replaceSeqShift(0, m_pSeqName, 0xE, -8.0f, -4.0f);
        m_pLightCtrl[id]->replaceSeq(0, m_pSeqName, 0x11);
    } else if (m_fgLightMove[id] == -4) {
        delete m_pCursorCtrl[id];
        m_pCursorCtrl[id] = NULL;
        if (m_pLightCtrl[id]) {
            m_pLightCtrl[id]->replaceSeq(0, m_pSeqName, 0x12);
        }
    } else if (m_fgLightMove[id] == -5) {
        delete m_pCursorCtrl[id];
        m_pCursorCtrl[id] = NULL;
        delete m_pLightCtrl[id];
        m_pLightCtrl[id] = NULL;
    }
    m_fgLightMove[id] = 1;
    if (m_pLightCtrl[id]) {
        m_fLightTimer[id] += fFrameRate;
        if (m_fLightTimer[id] >= 40.0f) {
            m_fLightTimer[id] = 40.0f;
            m_fgLightMove[id] = 0;
        }
        FLOAT t = 1.0f - powf(1.0f - m_fLightTimer[id] / 40.0f, 2.5f);
        InterVectorXYZ(m_vPosLightCurrent[id], m_vPosLightTarget[id], m_vPosLightOld[id], t);
        m_pLightCtrl[id]->setLocalPos(m_vPosLightCurrent[id]);
    }
}

void CWorldWarp::initWorldIdx() {
    char worldIDTbl[16];
    char worldClearTbl[16];
    unsigned char sora15Tbl[5] = { 2, 3, 4, 5, 6 };
    unsigned char sora69Tbl[4] = { 7, 8, 9, 10 };
    unsigned char sora1113Tbl[3] = { 11, 12, 13 };
    unsigned char riku14Tbl[4] = { 2, 5, 8, 1 };
    unsigned char riku58Tbl[4] = { 7, 3, 4, 6 };
    unsigned char riku1012Tbl[3] = { 12, 11, 13 };
    INT nMax = -1;

    for (INT i = 0; i < 16; i++) {
        worldClearTbl[i] = 0;
    }
    for (INT i = 0; i < 16; i++) {
        worldIDTbl[i] = CSceneInfo::getWorldID(i);
        worldClearTbl[worldIDTbl[i]] = worldIDTbl[i] != 0;
        if (nMax < 0 && worldIDTbl[i] == 0) {
            nMax = i;
        }
    }
    m_idxMaxWorld = -1;
    if (m_bWarpOrCard) {
        if (!m_bPlayerKind) {
            if (m_nCardLevel == 0) {
                worldIDTbl[0] = 1;
                worldClearTbl[1] = 1;
                m_idxMaxWorld = 0;
            } else if (m_nCardLevel >= 1 && m_nCardLevel <= 5) {
                for (INT i = nMax; i < 6; i++) {
                    for (INT j = 0; j < 5; j++) {
                        if (!worldClearTbl[sora15Tbl[j]]) {
                            worldIDTbl[i] = sora15Tbl[j];
                            worldClearTbl[sora15Tbl[j]] = 1;
                            break;
                        }
                    }
                }
                m_idxMaxWorld = 5;
            } else if (m_nCardLevel >= 6 && m_nCardLevel <= 9) {
                for (INT i = nMax; i < 10; i++) {
                    for (INT j = 0; j < 4; j++) {
                        if (!worldClearTbl[sora69Tbl[j]]) {
                            worldIDTbl[i] = sora69Tbl[j];
                            worldClearTbl[sora69Tbl[j]] = 1;
                            break;
                        }
                    }
                }
                m_idxMaxWorld = 9;
            } else {
                worldIDTbl[m_nCardLevel] = sora1113Tbl[m_nCardLevel - 10];
                m_idxMaxWorld = m_nCardLevel;
                worldClearTbl[sora1113Tbl[m_nCardLevel - 10]] = 1;
            }
        } else {
            if (m_nCardLevel == 0) {
                worldIDTbl[0] = 9;
                worldClearTbl[9] = 1;
                m_idxMaxWorld = 0;
            } else if (m_nCardLevel >= 1 && m_nCardLevel <= 4) {
                for (INT i = nMax; i < 5; i++) {
                    for (INT j = 0; j < 4; j++) {
                        if (!worldClearTbl[riku14Tbl[j]]) {
                            worldIDTbl[i] = riku14Tbl[j];
                            worldClearTbl[riku14Tbl[j]] = 1;
                            break;
                        }
                    }
                }
                m_idxMaxWorld = 4;
            } else if (m_nCardLevel >= 5 && m_nCardLevel <= 8) {
                for (INT i = nMax; i < 9; i++) {
                    for (INT j = 0; j < 4; j++) {
                        if (!worldClearTbl[riku58Tbl[j]]) {
                            worldIDTbl[i] = riku58Tbl[j];
                            worldClearTbl[riku58Tbl[j]] = 1;
                            break;
                        }
                    }
                }
                m_idxMaxWorld = 8;
            } else {
                worldIDTbl[m_nCardLevel] = riku1012Tbl[m_nCardLevel - 9];
                m_idxMaxWorld = m_nCardLevel;
                worldClearTbl[riku1012Tbl[m_nCardLevel - 9]] = 1;
            }
        }
    }
    for (INT i = 0; i < 16; i++) {
        CHAR worldID = worldIDTbl[i];
        m_idxWorld[i] = worldID - 1;
        if (worldID < 10) {
            m_idxIcon[i] = worldID - 1;
        } else if (worldID == 10) {
            m_idxIcon[i] = m_bPooClear + 9;
        } else if (worldID < 14) {
            m_idxIcon[i] = worldID;
        } else {
            m_idxIcon[i] = 0;
        }
    }
    m_nMaxFloorWorld = nMax - 1;
    USHORT nCurrentFloor = CSceneInfo::getCurrentFloorIndex();
    if (m_nMaxFloor < 0 || (nMax - 1 < m_nMaxFloor && !m_bWarpOrCard)) {
        m_idxWorld[m_nMaxFloor] = 14;
        m_idxIcon[m_nMaxFloor] = 14;
    }
    m_nSelect = nCurrentFloor;
    m_idxDistWorldOld = nCurrentFloor;
    m_idxDistWorld = nCurrentFloor;
    changeWorldIdx(nCurrentFloor, FALSE);
    changeWorldIdx(m_idxDistWorld, FALSE);
    m_idxDistWorldOld = m_idxDistWorld;
    m_idxSrcWorld = m_idxDistWorld;
    if (m_idxMaxWorld < 0) {
        m_idxMaxWorld = m_idxDistWorld;
    }
}

BOOL CWorldWarp::searchSelect(INT nAdd) {
    INT nSelect = m_nSelect;
    if (nAdd == 1 || nAdd == -1) {
        do {
            nSelect = (nSelect & 0xFFFC) + ((nSelect + nAdd) & 3);
        } while (m_idxWorld[changeWorldIdx(nSelect, TRUE)] < 0);
    } else {
        while (TRUE) {
            nSelect += nAdd;
            if (nSelect < 0) {
                nSelect += 16;
            }
            if (nSelect >= 16) {
                nSelect -= 16;
            }
            if (m_idxWorld[changeWorldIdx(nSelect, TRUE)] >= 0) {
                break;
            }
            INT i;
            for (i = nSelect - 1; (i & 3) < 3 && m_idxWorld[changeWorldIdx(i, TRUE)] < 0; i--) {
            }
            if ((i & 3) != 3) {
                nSelect = i;
                break;
            }
        }
    }
    if (nSelect == m_nSelect) {
        return FALSE;
    }
    m_nSelect = nSelect;
    changeWorldIdx(nSelect, FALSE);
    return TRUE;
}

INT CWorldWarp::changeWorldIdx(INT nSelect, BOOL bChangeOnly) {
    char idChangeTbl[16] = { 3, 2, 1, 0, 4, 5, 6, 7, 11, 10, 9, 8, 12, 15, 15, 15 };
    if (!bChangeOnly) {
        m_idxDistWorldOld = m_idxDistWorld;
        m_nSelect = nSelect;
        m_idxDistWorld = idChangeTbl[nSelect];
        return m_idxDistWorld;
    }
    return idChangeTbl[nSelect];
}

// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .data .rodata
