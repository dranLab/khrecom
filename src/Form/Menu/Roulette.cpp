
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Card.h"
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
#include "Game/CardKind.h"
#include "Form/Menu/Roulette.h"
#include "Form/Menu/MenuParts.h"
#include "System/HeapMan.h"
#include "System/Vector.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DSeqObj.h"
#include "System/Graphic/2d/2DFnt.h"
#include "System/Graphic/2d/2DCard.h"
#include "Game/SceneEnv.h"
#include "Game/Game.h"
#include "Game/GameWork.h"
#include "Game/Gimmick/GimmickManager.h"
#include "include_asm.h"

#line 263



























#include "System/RsrcDB.h"





#include "System/Rcf.h"





#include "System/Graphic/PackMan.h"




#include "System/Sound/SoundMan.h"
#line 314

extern "C" {
int sprintf(char*, const char*, ...);
float sinf(float);
float cosf(float);
float powf(float, float);
}

#include "Game/Gimmick/GimmickManager.h"






using namespace D2;

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

CRoulette::CRoulette(CTask* p, BOOL fgRoulette) : CMenu(p, -1) {
    declareSysTask(0x22);
    m_fgRoulette = fgRoulette;
    m_fgRotate = 0;
    CGame::enablePause(FALSE);
    m_pLadName = "premium.lad";
    m_pSeqName = "premium:00";
    m_pBaseCtrl = NULL;
    m_pCardInfoCtrl = NULL;
    for (INT i = 0; i < 11; i++) {
        m_pCardCtrl[i] = NULL;
    }
    for (INT i = 0; i < 99; i++) {
        m_pCard[i] = NULL;
    }
    m_pGetCardCtrl = NULL;
    m_pCardGetCtrl = NULL;
    m_pEffectCtrl = NULL;
    m_pBaloonCtrl = NULL;
    m_pCursorCtrl = NULL;
    for (INT i = 0; i < 30; i++) {
        m_pNumButton[i] = NULL;
    }
    m_pHelpButton = NULL;
    CSound::pauseAllSE();
    CSceneEnv::setStop((ObjType::eObjBit)0xFFFEFFFF);
    CSceneEnv::setStopGauge(TRUE);
}

CRoulette::~CRoulette() {
    dispose();
    CSound::startAllPausedSE();
    CGame::enablePause(TRUE);
    CSceneEnv::setStop((ObjType::eObjBit)0);
    CSceneEnv::setStopGauge(FALSE);
    C2DMan::changeLevel(C2DMan::D2MAN_LEVEL0);
    CRsrcDB::removeGroup(2, 0x69);
    CPackMan::changeDrawMode(0);
}

void CRoulette::dispose() {
    delete m_pBaseCtrl;
    m_pBaseCtrl = NULL;
    delete m_pCardInfoCtrl;
    m_pCardInfoCtrl = NULL;
    for (INT i = 0; i < 99; i++) {
        delete m_pCard[i];
        m_pCard[i] = NULL;
    }
    for (INT i = 0; i < 11; i++) {
        delete m_pCardCtrl[i];
        m_pCardCtrl[i] = NULL;
    }
    delete m_pCursorCtrl;
    m_pCursorCtrl = NULL;
    delete m_pBaloonCtrl;
    m_pBaloonCtrl = NULL;
    delete m_pEffectCtrl;
    m_pEffectCtrl = NULL;
    delete m_pCardGetCtrl;
    m_pCardGetCtrl = NULL;
    delete m_pGetCardCtrl;
    m_pGetCardCtrl = NULL;
    for (INT i = 0; i < 30; i++) {
        delete m_pNumButton[i];
        m_pNumButton[i] = NULL;
    }
    delete m_pHelpButton;
    m_pHelpButton = NULL;
}

#line 411

void CRoulette::main() {
    static LPCTSTR pStr[2] = { "Upgrade a card to a Premium Card.         : Cancel", "Choose a Map Card." };
    FVECTOR vec;
    C2DCard* p2DCard;
    CARD* pDeck;
    UINT nCard;
    static USHORT helpSeqTbl[3] = { 56, 57, 58 };

    switch (m_nStep) {
    case 0:
        CPackMan::changeDrawMode(1);
        m_nMenuStep = 0;
        if (!CRsrcDB::getRsrcTex(m_pSeqName, NAME_CASE_SENSITIVE)) {
            CRcf::reqLoadRCFFront(2, 0x69, 0, NULL, 0);
            m_nStep = 1;
            break;
        }
        m_nStep = 2;
        break;
    case 1:
        if (CRcf::isCompleteLoadRCF(2, 0x69)) {
            m_nStep = 2;
        }
        break;
    case 2:
        C2DMan::changeLevel(C2DMan::D2MAN_LEVEL1);
        m_nStep = 3;
        break;
    case 3: {
        vec.set(-200.0f, -100.0f, 0.0f, 1.0f);
        if (!m_fgRoulette) {
            USHORT nDeck = CCard::getCurrentDeckNo();
            nCard = CCard::getDeckMax(nDeck);
            pDeck = CCard::getDeck(nDeck);
        } else {
            CARD card[22];
            CGimmickManager* pGimMan = (CGimmickManager*)getSysTask(0x11);
            if (!pGimMan) {
                out();
                return;
            }
            DropPrize dorpPrize;
            dorpPrize.StockKind = 0;
            dorpPrize.Kind = CARD_KIND_None;
            dorpPrize.Num = 0;
            dorpPrize.No = 0;
            for (INT i = 0; i < 22; i++) {
                dorpPrize = pGimMan->DecideMapCardPrize(0);
                if ((UINT)(dorpPrize.Kind - 0xA1) < 0x19) {
                    card[i].init(dorpPrize.Kind, dorpPrize.No);
                } else {
                    card[i].init(0xB6, 1);
                }
            }
            pDeck = card;
            nCard = 22;
        }
        m_nCardMax = 0;
        for (INT i = 0; i < 99 && m_nCardMax < nCard; i++) {
            if (m_fgRoulette || (CCard::canPremium(pDeck[i].m_nKind) && !pDeck[i].m_fgPremium)) {
                m_idxDeck[m_nCardMax] = i;
#line 201
                p2DCard = m_pCard[m_nCardMax++] = new KH_AT("(201)") C2DCard;
                p2DCard->setLayer(12);
                p2DCard->create(pDeck[i]);
                p2DCard->setScl(0.9f);
                p2DCard->enableColorCtrl(TRUE);
                p2DCard->enableDisp(FALSE);
            }
        }
#line 208
        ASSERT(m_nCardMax);
        m_nCardDummyMax = m_nCardMax;
        if (m_nCardDummyMax) {
            while (m_nCardDummyMax < 11) {
                for (INT j = 0; j < m_nCardMax; j++) {
#line 213
                    p2DCard = m_pCard[m_nCardDummyMax++] = new KH_AT("(213)") C2DCard;
                    p2DCard->setLayer(12);
                    p2DCard->create(m_pCard[j]->m_card);
                    p2DCard->setScl(0.9f);
                    p2DCard->enableColorCtrl(TRUE);
                    p2DCard->enableDisp(FALSE);
                }
            }
        }
        for (INT i = 0; i < 11; i++) {
#line 223
            C2DSeqCtrl* pCtrl = m_pCardCtrl[i] = new KH_AT("(223)") C2DSeqCtrl;
            pCtrl->enableColorCtrl(TRUE);
            pCtrl->addSeq(m_pSeqName, 0x1B);
            if (m_pCard[10 - i]) {
                m_pCard[10 - i]->setParent(pCtrl);
            }
            pCtrl->enableColorCtrl(TRUE);
        }
        CSound::playSE(3, -1, 0x40);
        m_nStep = 4;
        break;
    }
    case 4:
#line 239
        m_pBaseCtrl = new KH_AT("(239)") C2DSeqCtrl;
        m_pBaseCtrl->addSeq(m_pSeqName, 0);
        vec.init();
        m_pBaseCtrl->setPos(vec);
        m_hLay = C2DMan::create(m_pLadName, m_fgRoulette ? 0 : 5);
#line 245
        m_pHelpButton = new KH_AT("(245)") CButton;
        vec.set(-256.0f, -192.0f, 0.0f, 1.0f);
        m_pHelpButton->init(m_pSeqName, 0xE, helpSeqTbl, vec, NULL, pStr[m_fgRoulette], 0xF, 0.0f, 0.0f, 0x40, NULL,
                            -1);
        m_nStep++;
        m_nStatusLad = 0;
        break;
    case 5:
        if (!m_nStatusLad && C2DMan::isEnd(m_hLay)) {
            m_pBaseCtrl->replaceSeq(0, m_pSeqName, 1);
            C2DMan::destroy(m_hLay);
            C2DMan::setLayer(13);
            m_hLay = C2DMan::create(m_pLadName, !m_fgRoulette ? 6 : 1);
            m_pHelpButton->setSequence(CButton::BS_LOOP, FALSE);
            m_nStatusLad = 1;
        }
        if ((m_nStatusLad != 0) == TRUE) {
            setCardInfo(0);
            m_nStep = 6;
        }
        break;
    case 6:
        if (setCardInfo(1)) {
            m_nStep = 7;
        }
        break;
    case 7:
        if (menuMain()) {
            m_fgRotate = 0;
            m_nStep = 8;
        }
        break;
    case 8:
        setCardInfo(2);
        m_nStep = 9;
        break;
    case 9:
        m_pBaseCtrl->replaceSeq(0, m_pSeqName, 2);
        C2DMan::destroy(m_hLay);
        m_hLay = C2DMan::create(m_pLadName, !m_fgRoulette ? 7 : 2);
        m_pHelpButton->setSequence(CButton::BS_OUT, FALSE);
        CSound::playSE(4, -1, 0x40);
        m_nStep++;
        break;
    case 10:
        if (!C2DMan::isEnd(m_hLay)) {
            return;
        }
        C2DMan::destroy(m_hLay);
        dispose();
        m_nStep = 12;
        break;
    case 11:
    case 12:
    default:
        out();
        break;
    }
}

template <class T> static inline T& _ref(T& v) { return v; }

INT CRoulette::menuMain() {
    static LPCTSTR pStr = "%s is now\na Premium Card!";
    INT ret = 0;
    FVECTOR vec;
    FVECTOR vec2;
    FVECTOR vScale;
    G_RGBA rgba;
    static LPCTSTR pSeqName2 = "get_card:00";
    char buf[64];

    switch (m_nMenuStep) {
    case 0:
        m_nMenuStep = 1;
        break;
    case 1:
        m_fgRotate = 1;
        m_nMenuStep = 2;
        m_fRouletteRotY = 0.0f;
        m_fSpeed = 0.0f;
        m_fTimer = 0.0f;
        m_idxStart = 0;
        m_fgReplace = 0;
        m_fgSelect = 0;
        break;
    case 2:
        m_fTimer += (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_fTimer > 120.0f) {
#line 371
            m_pCursorCtrl = new KH_AT("(371)") C2DSeqCtrl;
            m_pCursorCtrl->setLayer(14);
            m_pCursorCtrl->addSeq(m_pSeqName, 3);
            m_pCursorCtrl->setScl(0.9f);
            if (!m_fgRoulette) {
                vec.set(244.0f, 184.0f, 0.0f, 1.0f);
            } else {
                vec.set(184.0f, 206.0f, 0.0f, 1.0f);
            }
            m_pCursorCtrl->setPos(vec);
#line 381
            m_pEffectCtrl = new KH_AT("(381)") C2DSeqCtrl;
            m_pEffectCtrl->setLayer(11);
            m_pEffectCtrl->addSeq(m_pSeqName, 0x20);
            m_pEffectCtrl->setPos(vec);
#line 387
            m_pBaloonCtrl = new KH_AT("(387)") C2DSeqCtrl;
            m_pBaloonCtrl->setLayer(14);
            m_pBaloonCtrl->addSeq(m_pSeqName, 7);
            vec.x += 16.0f;
            vec.y += -36.0f;
            m_pBaloonCtrl->setPos(vec);
            m_fSpeed = 1.0f;
            m_nMenuStep = 3;
        } else {
            m_fSpeed = m_fTimer / 120.0f;
        }
        break;
    case 3:
        m_fSpeed = (FLOAT)getFrameRate() * 0.0200000014f;
        if (_isTrgOK(0)) {
            m_fTimer = 0.0f;
            m_fgSelect = 1;
            m_pBaloonCtrl->enableDisp(FALSE);
            if (!m_fgRoulette) {
                m_nSelect = (m_idxStart + 5) % m_nCardMax;
                CCard::makePremiereDeck(CCard::getCurrentDeckNo(), m_idxDeck[m_nSelect]);
                m_nSelect = (m_idxStart + 5) % m_nCardDummyMax;
            } else {
                m_nSelect = (m_idxStart + 5) % m_nCardDummyMax;
                CCard::addCard(m_pCard[m_nSelect]->m_card.m_nKind2, m_pCard[m_nSelect]->m_card.m_nNumber2);
                m_idxGetCard =
                    (m_pCard[m_nSelect]->m_card.m_nCategory21 - 9) * 10 + m_pCard[m_nSelect]->m_card.m_nNumber2;
            }
            CSound::playSE(2, -1, 0x40);
            m_nMenuStep = 4;
        } else if (!m_fgRoulette && _isTrgCANCEL(0)) {
            m_pCursorCtrl->enableDisp(FALSE);
            m_pBaloonCtrl->enableDisp(FALSE);
            m_fTimer = 0.0f;
            CSound::playSE(4, -1, 0x40);
            m_nMenuStep = 13;
        }
        break;
    case 4:
        m_fTimer += (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_fTimer > 60.0f) {
            m_nMenuStep = 5;
            m_fTimer = 0.0f;
        } else {
            m_pCardCtrl[4]->m_seqControl.vCol.w -= 0.05f;
            m_pCardCtrl[6]->m_seqControl.vCol.w -= 0.05f;
            if (m_fTimer >= 10.0f) {
                m_pCardCtrl[3]->m_seqControl.vCol.w -= 0.05f;
                m_pCardCtrl[7]->m_seqControl.vCol.w -= 0.05f;
            }
            if (m_fTimer >= 20.0f) {
                m_pCardCtrl[2]->m_seqControl.vCol.w -= 0.05f;
                m_pCardCtrl[8]->m_seqControl.vCol.w -= 0.05f;
            }
            if (m_fTimer >= 30.0f) {
                m_pCardCtrl[1]->m_seqControl.vCol.w -= 0.05f;
                m_pCardCtrl[9]->m_seqControl.vCol.w -= 0.05f;
            }
            if (m_fTimer >= 40.0f) {
                m_pCardCtrl[0]->m_seqControl.vCol.w -= 0.05f;
                m_pCardCtrl[10]->m_seqControl.vCol.w -= 0.05f;
            }
            for (INT i = 0; i < 11; i++) {
                if (m_pCardCtrl[i]->m_seqControl.vCol.w < 0.0f) {
                    m_pCardCtrl[i]->m_seqControl.vCol.w = 0.0f;
                }
            }
        }
        break;
    case 5:
        m_fTimer += (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_fTimer > 30.0f) {
            m_pCursorCtrl->enableDisp(FALSE);
            m_pEffectCtrl->replaceSeq(0, m_pSeqName, 0x21);
            m_fTimer = 0.0f;
            m_nMenuStep = 6;
        }
        break;
    case 6:
    case 13:
        m_fTimer += (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_fTimer > 30.0f) {
            m_nMenuStep = (m_nMenuStep == 13) ? 14 : 7;
        } else {
            for (INT i = 0; i < 11; i++) {
                m_pCardCtrl[i]->m_seqControl.vCol.w -= 0.05f;
                if (m_pCardCtrl[i]->m_seqControl.vCol.w < 0.0f) {
                    m_pCardCtrl[i]->m_seqControl.vCol.w = 0.0f;
                }
            }
        }
        break;
    case 7:
        m_fTimer = 0.0f;
#line 495
        m_pCardGetCtrl = new KH_AT("(495)") C2DSeqCtrl;
        m_pCardGetCtrl->setLayer(14);
        m_pCardGetCtrl->enableColorCtrl(TRUE);
        m_pCardGetCtrl->addSeq(pSeqName2, m_fgRoulette ? 2 : 9);
        vec.init();
        m_pCardGetCtrl->setPos(vec);
        rgba.col = 0x807C7C7C;
#line 503
        m_pGetCardFnt[0] = new KH_AT("(503)") C2DFnt(0, 0, &rgba, 16, m_pCard[m_nSelect]->m_card.getName(), 0x42);
        m_pCardGetCtrl->addFnt(pSeqName2, m_pGetCardFnt[0]);
        sprintf(buf, pStr, m_pCard[m_nSelect]->m_card.getName());
        if (!m_fgRoulette) {
#line 509
            m_pGetCardFnt[1] = new KH_AT("(509)") C2DFnt(0, 0, &rgba, 16, NULL, 0x42);
            m_pGetCardFnt[1]->allocText(buf);
            m_pGetCardFnt[1]->setSpace(16, 0x60);
            m_pCardGetCtrl->addFntShift(_ref(pSeqName2), m_pGetCardFnt[1], 0.0f, 243.0f);
            m_pCard[m_nSelect]->m_card.m_fgPremium = 1;
        }
#line 516
        m_pGetCardCtrl = new KH_AT("(516)") C2DSeqCtrl;
        m_pGetCardCtrl->addSeq(_ref(pSeqName2), 0);
        m_pGetCardCtrl->setPos(vec);
        m_pCard[m_nSelect]->changeLayer(14);
        m_pCard[m_nSelect]->replace(m_pCard[m_nSelect]->m_card);
        m_pCard[m_nSelect]->setLocalPos(vec);
        m_pCard[m_nSelect]->setScl(1.0f);
        m_pCard[m_nSelect]->setParent(m_pGetCardCtrl);
        CSound::playSE(0x3A, -1, 0x40);
        m_nMenuStep = 9;
        break;
    case 9:
        m_fTimer += (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_fTimer > 180.0f) {
            m_pCardGetCtrl->replaceSeq(0, pSeqName2, m_fgRoulette ? 3 : 10);
            m_pGetCardCtrl->replaceSeq(0, pSeqName2, 1);
            m_fTimer = 0.0f;
            m_nMenuStep = 8;
        }
        break;
    case 8:
        m_fTimer += (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_fTimer > 30.0f) {
            if (!m_fgRoulette || m_pCard[m_nSelect]->m_card.m_nKind == 0xB9) {
                m_nMenuStep = 14;
            } else {
                m_nMenuStep = 10;
            }
        }
        break;
    case 10:
        m_pNumButton[m_idxGetCard]->get2DSeqCtrl()->addSeq(m_pSeqName, 0x34);
        CSound::playSE(0x55, -1, 0x40);
        m_fTimer = 0.0f;
        m_nMenuStep = 11;
        break;
    case 11:
        m_fTimer += (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_fTimer > 30.0f) {
            setCardInfo(4);
            m_fTimer = 0.0f;
            m_nMenuStep = 12;
        }
        break;
    case 12:
        m_fTimer += (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_fTimer > 60.0f) {
            m_nMenuStep = 14;
        }
        break;
    case 14:
        ret = 1;
        break;
    }

    if (m_fgRotate && m_pCard[0]) {
        FLOAT fBaseRad;
        FLOAT fStartRad;
        FLOAT fRateX;
        if (!m_fgRoulette) {
            fBaseRad = 0.17453292f;
            fStartRad = -fBaseRad;
            fRateX = 1.5f;
        } else {
            fBaseRad = 0.2617994f;
            fStartRad = -0.5235988f;
            fRateX = 1.0f;
        }
        FLOAT fRate = m_fSpeed;
        FLOAT fRadian = fBaseRad * (fRate * 0.5f + 0.5f) * (!m_fgRoulette ? 0.15f : 0.1f);
        if (fRate > 1.0f) {
            fRate = 1.0f;
        }
        FLOAT fRadius = (1.0f - powf(1.0f - fRate, 2.0f)) * 225.0f;
        if (m_fgSelect) {
            fRadius = 225.0f;
            fRadian = 0.0f;
            m_fgReplace = 0;
            m_fRouletteRotY = fBaseRad * 0.5f;
        }
        m_fRouletteRotY += fRadian;
        if (m_fRouletteRotY >= fBaseRad) {
            m_fRouletteRotY -= fBaseRad;
            m_fgReplace = 0;
            m_pCard[m_idxStart]->enableDisp(FALSE);
            if (++m_idxStart >= m_nCardDummyMax) {
                m_idxStart = 0;
            }
            INT j = m_idxStart;
            for (INT i = 0; i < 11; i++) {
                m_pCard[j]->setParent(m_pCardCtrl[i]);
                m_pCard[j]->replace(m_pCard[j]->m_card);
                m_pCard[j]->enableDisp(TRUE);
                if (++j >= m_nCardDummyMax) {
                    j = 0;
                }
            }
            CSound::playSE(1, -1, 0x40);
        }
        FLOAT fRotStart = m_fRouletteRotY + fStartRad;
        vec.init();
        for (INT i = 10; i >= 0; i--) {
            vec.x = sinf(fRotStart) * fRadius * fRateX;
            vec.y = -cosf(fRotStart) * fRadius;
            vec.x -= 250.0f;
            vec.y += 150.0f;
            m_pCardCtrl[i]->setLocalPos(vec);
            fRotStart += fBaseRad;
        }
    }
    return ret;
}
INT CRoulette::setCardInfo(INT nCmd) {
    INT ret;
    FVECTOR vec;
    FVECTOR vec2;
    FVECTOR vScale;
    CButton* pButton;
    C2DSeqObj* p2DSeqObj;
    static USHORT mapKindTbl[6] = { 161, 172, 172, 179, 179, 185 };
    static USHORT numSeqTbl[15] = { 50, 50, 50, 50, 51, 48, 48, 48, 48, 49, 46, 46, 46, 46, 47 };

    if (!m_fgRoulette) {
        return TRUE;
    }
    const char* pFmt = "%02d";
    ret = 0;
    switch (nCmd) {
    case 0:
#line 693
        m_pCardInfoCtrl = new KH_AT("(693)") C2DSeqCtrl;
        m_pCardInfoCtrl->setLayer(11);
        m_pCardInfoCtrl->enableColorCtrl(TRUE);
        m_pCardInfoCtrl->addSeq(m_pSeqName, 0x35);
        C2DMan::getLayPosition(m_pLadName, 4, 2, vec, 0);
        m_pCardInfoCtrl->setPos(vec);
        C2DMan::getLayPosition(m_pLadName, 3, 0x1F, vec, 0);
        p2DSeqObj = m_pCardInfoCtrl->addSeqShift(m_pSeqName, 0x2D, vec.x, vec.y);
        p2DSeqObj->changeLayer(13);
    case 4:
        for (INT i = 0; i < 30; i++) {
            m_nMapCard[i] = 0;
        }
        for (INT i = 0; i < 3; i++) {
            for (INT j = mapKindTbl[i * 2]; j < mapKindTbl[i * 2 + 1]; j++) {
                for (INT k = 0; k < 10; k++) {
                    INT num = CCard::searchCardKind(j, k);
                    if (num > 0) {
                        m_nMapCard[i * 10 + k] += num;
                    }
                }
            }
        }
        for (INT i = 0; i < 30; i++) {
            char buf[16];
            if (!m_pNumButton[i]) {
#line 722
                pButton = m_pNumButton[i] = new KH_AT("(722)") CButton;
                C2DMan::getLayPosition(m_pLadName, 3, i + 1, vec, 0);
                sprintf(buf, pFmt, m_nMapCard[i]);
                pButton->init(m_pSeqName, 12, &numSeqTbl[(i / 10) * 5], vec, m_pCardInfoCtrl, buf, 14, 0.0f, 0.0f,
                              0x40, NULL, -1);
            } else {
                pButton = m_pNumButton[i];
                sprintf(buf, pFmt, m_nMapCard[i]);
                pButton->changeFont(buf);
            }
            pButton->setActive(m_nMapCard[i] != 0);
        }
        break;
    case 1:
        if (m_pCardInfoCtrl->isEnd(0)) {
            m_pCardInfoCtrl->replaceSeq(0, m_pSeqName, 0x36);
            ret = 1;
        }
        break;
    case 2:
        m_pCardInfoCtrl->replaceSeq(0, m_pSeqName, 0x37);
        break;
    }
    return ret;
}
// SOURCE DATA: .data .rodata
