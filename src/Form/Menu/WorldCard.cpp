
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define SAVELOADCTRL_H_PATH "c:/KHCOM/MainApp/Form/Menu/SaveLoadCtrl.h" /* MATCH: this unit names SaveLoadCtrl.h so (header default "Form/Menu/SaveLoadCtrl.h") */
#include <string.h>
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Form/Menu/SaveLoadCtrl.h"


#include "System/TaskMan.h"



#include "System/Ctd/CtdMan.h"
#include "Game/CardBattle.h"
#include "Game/Technic.h"

/* MATCH: Placeholder: an empty string follows Technic.h's strings in .rodata; which header it comes from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

#include "Game/CardKind.h"
#include "Game/Game.h"
#include "Game/GameWork.h"
#include "Form/Menu/JMemoFlag.h"
#include "Form/Menu/BtlReportCtrl.h"
#include "Form/Menu/WorldCard.h"
#include "System/MainApp.h"
#include "System/Sound/SoundMan.h"
#include "System/Graphic/2d/2DCard.h"
#include "System/Fsm.h"

#line 262



#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

#include "System/RsrcDB.h"




#include "System/Rcf.h"
#include "System/HeapMan.h"





#line 286

namespace WorldCard {

struct _LayoutInfo {
    SHORT nLayout;
    SHORT nIdx;
};

#line 70 "c:/KHCOM/MainApp/Form/Menu/WorldCard.cpp"
CWorldCardSeq* CWorldCardSeq::m_pInstance = NULL;

void CWorldCardSeq::create(CTask* in_pCTask, INT in_CreateType) {
    if (!m_pInstance) {
#line 74
        new KH_AT("(74)") CWorldCardSeq(in_pCTask, in_CreateType);
    }
}

CWorldCardSeq::CWorldCardSeq(CTask* pParent, INT in_CreateType) : CTask(pParent, 0x8000) {
    m_State[0].set(this, &CWorldCardSeq::beginStateInit, &CWorldCardSeq::stateInit, &CWorldCardSeq::endStateInit);
    m_State[1].set(this, &CWorldCardSeq::beginStateMain, &CWorldCardSeq::stateMain, &CWorldCardSeq::endStateMain);
    m_State[2].set(this, &CWorldCardSeq::beginStateExit, &CWorldCardSeq::stateExit, &CWorldCardSeq::endStateExit);
    m_State[3].set(this, &CWorldCardSeq::beginStateEnd, &CWorldCardSeq::stateEnd, &CWorldCardSeq::endStateEnd);
    m_bFpsBackup = CMainApp::isFpsHalf();
    CMainApp::setFpsFull();
    changeState(STATE_INIT);
    m_pInstance = this;
    m_Type = in_CreateType;
    for (INT i = 0; i < 12; i++) m_pStrings[i] = NULL;
    for (INT i = 0; i < 5; i++) m_pCard[i] = NULL;
}

/* MATCH: Placeholder: __FILE__ precedes the destructor's message in .rodata (used later by changeState's assert). */
inline void _filePlaceholder() {
    AssertApp(__FILE__, 0, 0, 0);
}

CWorldCardSeq::~CWorldCardSeq() {
    TRACE("CWorldCardSeq::~CWorldCardSeq()\n");
    m_pInstance = NULL;
    for (INT i = 0; i < 5; i++) {
        if (m_pCard[i]) {
            delete m_pCard[i];
            m_pCard[i] = NULL;
        }
    }
    for (INT i = 0; i < 12; i++) {
        if (m_pStrings[i]) {
            delete m_pStrings[i];
            m_pStrings[i] = NULL;
        }
    }
    CRsrcDB::removeGroup(2, 63);
    if (m_bFpsBackup) CMainApp::setFpsHalf();
}

void CWorldCardSeq::main() {
    update();
}

void CWorldCardSeq::disp() {}

void CWorldCardSeq::beginStateInit() {
    CRcf::reqLoadRCF(2, 63, 0, NULL, 0);
}

void CWorldCardSeq::stateInit() {
    if (CRcf::isCompleteLoadRCF(0, 0)) changeState(STATE_MAIN);
}

void CWorldCardSeq::endStateInit() {}

void CWorldCardSeq::beginStateMain() {
    createGetWorldCardWnd();
    m_Step = 0;
    m_fWait = 0.0f;
    if (m_pStrings[0]) CSound::playSE(0x3A, -1, 0x40);
}

void CWorldCardSeq::stateMain() {
    if (!m_pStrings[0]) {
        changeState(STATE_EXIT);
        return;
    }
    m_fWait += getFrameRateF();
    if (CPadHandler::isTrgOK() && !m_Step && 60.0f <= m_fWait) {
        m_Step = 1;
        for (INT i = 0; i < 12; i++) {
            if (m_pStrings[i] && m_pStrings[i]->getState() == C2DSeqAgent::STATE_IN) {
                m_pStrings[i]->changeState(C2DSeqAgent::STATE_LOOP, TRUE, -1);
            }
        }
    }
    if (m_pStrings[0]->getState() == C2DSeqAgent::STATE_LOOP) {
        D2::C2DSeqCtrl* pCtrl = m_pStrings[0]->C2DSeqAgent::getSeqCtrl();
        if (pCtrl) {
            HANDLE* pHandle = pCtrl->getHandle();
            if (pHandle && D2::C2DMan::isEnd(*pHandle)) {
                for (INT i = 0; i < 12; i++) {
                    if (m_pStrings[i]) m_pStrings[i]->dispOff();
                }
                changeState(STATE_EXIT);
            }
        }
    }
}

void CWorldCardSeq::endStateMain() {}

void CWorldCardSeq::beginStateExit() {}

void CWorldCardSeq::stateExit() {
    out();
    changeState(STATE_END);
}

void CWorldCardSeq::endStateExit() {}

void CWorldCardSeq::beginStateEnd() {}

void CWorldCardSeq::stateEnd() {}

void CWorldCardSeq::endStateEnd() {}

void CWorldCardSeq::changeState(INT state) {
#line 298
    ASSERT((0 <= state) && (state < STATE_MAX));
    TRACE_USER(KYOSHIOK, "StateChange:--->%d\n", state);
    gotoState(m_State[state]);
}

INT CWorldCardSeq::isNowDispCard() {
    return m_pInstance != NULL;
}

C2DSeqStrings* CWorldCardSeq::createSeq(INT in_SeqIndex, INT in_SeqType, INT in_LayoutDef, INT in_DispLayer,
                                        const CHAR* in_pStr) {
    C2DSeqAgent::SeqTbl tblSeqType[4] = { { { 0, 1, 0 } }, { { 2, 3, 0 } }, { { 4, 5, 0 } }, { { 6, 7, 0 } } };
    C2DSeqAgent::SeqTbl& t = tblSeqType[in_SeqType];
    _LayoutInfo info;
    FVECTOR vPos;

    info.nLayout = in_LayoutDef;
    info.nIdx = in_SeqIndex;
    D2::C2DMan::getLayPosition("get_wcard.lad", info.nLayout, info.nIdx, vPos, FALSE);
#line 342
    C2DSeqStrings* pAgent = new KH_AT("(342)") C2DSeqStrings(this);
    pAgent->setupSeq(t, "get_wcard.lad", 0, in_pStr, in_DispLayer, 16);
    pAgent->setupState(t);
    pAgent->setPos(vPos);
    return pAgent;
}

void CWorldCardSeq::createGetWorldCardWnd() {
    switch (m_Type) {
    case 0:
        WorldWndOne(0xBE, CCtdMan::getText(0x380000) ?: "");
        CJiminyMemoFlag::checkEvent(4, 0xBE, 0);
        CGameWork::setByte(0, 11, 0);
        break;
    case 1: {
        int KindTable[5] = { 0xBF, 0xC0, 0xC1, 0xC2, 0xC3 };
        WorldWndFive(KindTable, CCtdMan::getText(0x380001) ?: "");
        CJiminyMemoFlag::checkEvent(4, 0xBF, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC0, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC1, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC2, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC3, 0);
        CGameWork::setByte(0, 11, 1);
        break;
    }
    case 2: {
        int KindTable[4] = { 0xC4, 0xC5, 0xC6, 0xC7 };
        WorldWndFour(KindTable, CCtdMan::getText(0x380002) ?: "");
        CJiminyMemoFlag::checkEvent(4, 0xC4, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC5, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC6, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC7, 0);
        CGameWork::setByte(0, 11, 6);
        break;
    }
    case 3:
        WorldWndOne(0xC8, CCtdMan::getText(0x380003) ?: "");
        CJiminyMemoFlag::checkEvent(4, 0xC8, 0);
        CGameWork::setByte(0, 11, 10);
        break;
    case 4:
        WorldWndOne(0xC9, CCtdMan::getText(0x380004) ?: "");
        CJiminyMemoFlag::checkEvent(4, 0xC9, 0);
        if (CGame::isSoraGame()) {
            CGameWork::setByte(0, 11, 11);
        } else {
            CGameWork::setByte(0, 11, 9);
        }
        break;
    case 5:
        if (CGame::isRikuGame()) {
            WorldWndOne(0xCA, CCtdMan::getText(0x380005) ?: "");
            CJiminyMemoFlag::checkEvent(4, 0xCA, 0);
            CGameWork::setByte(0, 11, 11);
        } else {
            CGameWork::setByte(0, 11, 12);
        }
        break;
    case 6:
        WorldWndOne(0xC6, CCtdMan::getText(0x380006) ?: "");
        CJiminyMemoFlag::checkEvent(4, 0xC6, 0);
        CGameWork::setByte(0, 11, 0);
        break;
    case 7: {
        int KindTable[4] = { 0xBE, 0xBF, 0xC2, 0xC5 };
        WorldWndFour(KindTable, CCtdMan::getText(0x380007) ?: "");
        CJiminyMemoFlag::checkEvent(4, 0xBE, 0);
        CJiminyMemoFlag::checkEvent(4, 0xBF, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC2, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC5, 0);
        CGameWork::setByte(0, 11, 1);
        break;
    }
    case 8: {
        int KindTable[4] = { 0xC0, 0xC1, 0xC3, 0xC4 };
        WorldWndFour(KindTable, CCtdMan::getText(0x380008) ?: "");
        CJiminyMemoFlag::checkEvent(4, 0xC0, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC1, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC3, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC4, 0);
        CGameWork::setByte(0, 11, 5);
        break;
    }
    }
}

void CWorldCardSeq::WorldWndOne(INT in_CardKind, const CHAR* in_pStr) {
    CARD card;

    m_pStrings[0] = createSeq(0, 2, 0, 11, in_pStr);
    m_pStrings[0]->start();
    m_pStrings[1] = createSeq(1, 0, 0, 11, NULL);
    m_pStrings[1]->start();
    card.init(in_CardKind, 0);
#line 473
    m_pCard[0] = new KH_AT("(473)") C2DCard;
    m_pCard[0]->create(card);
    m_pCard[0]->setParent(m_pStrings[1]->getSeqCtrl());
}

void CWorldCardSeq::WorldWndFour(INT* in_CardKindTable, const CHAR* in_pStr) {
    CARD card;

    m_pStrings[0] = createSeq(0, 3, 1, 11, in_pStr);
    m_pStrings[0]->start();
    for (INT i = 0; i < 4; i++) {
        m_pStrings[i + 1] = createSeq(i + 1, 1, 1, 11, NULL);
        m_pStrings[i + 1]->start();
        card.init(in_CardKindTable[i], 0);
#line 497
        m_pCard[i] = new KH_AT("(497)") C2DCard;
        m_pCard[i]->create(card);
        m_pCard[i]->setParent(m_pStrings[i + 1]->getSeqCtrl());
    }
}

void CWorldCardSeq::WorldWndFive(INT* in_CardKindTable, const CHAR* in_pStr) {
    CARD card;

    m_pStrings[0] = createSeq(0, 3, 2, 11, in_pStr);
    m_pStrings[0]->start();
    for (INT i = 0; i < 5; i++) {
        m_pStrings[i + 1] = createSeq(i + 1, 1, 2, 11, NULL);
        m_pStrings[i + 1]->start();
        card.init(in_CardKindTable[i], 0);
#line 522
        m_pCard[i] = new KH_AT("(522)") C2DCard;
        m_pCard[i]->create(card);
        m_pCard[i]->setParent(m_pStrings[i + 1]->getSeqCtrl());
    }
}

}  // namespace WorldCard
// SOURCE DATA: .data .rodata
