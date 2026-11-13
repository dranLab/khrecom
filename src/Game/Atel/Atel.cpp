#define ENTITY_H_SETFLAG_DIRECT /* MATCH: setEntityFlag(0, 0x1000) as a plain and (createAvatar) */

#include "Game/Atel/Atel.h"
#include "System/Graphic/Gprim.h"
#include "Game/Atel/Avatar.h"

#include "System/TaskMan.h"


#include "System/Task.h"
#include "Game/Card.h"
#include "Game/Technic.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Fsm.h"
#include "Game/Atel/EventTrap.h"
#include "System/Graphic/ScreenMask.h"
#include "System/TaskMan.h"
#include "System/MainApp.h"
#include "System/Rcf.h"
#include "Game/Game.h"
#include "Game/GameWork.h"
#include "Game/SceneInfo.h"
#include "Game/StorageWork.h"
#include "Form/Menu/JMemoFlag.h"
#include "Game/EntityFactory.h"
#include "Game/Entity.h"









#define ASSERT(e) KH_DEBUG(if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0))

#include "Game/Atel/VSME.h"
#include "System/HeapMan.h"
namespace ATEL {
















struct AvatarModelDispParam {
    INT _iNo;
    INT _nState;
};












CFadeIOMask* s_pFadeIOMask[2] = {NULL, NULL};

}  // namespace ATEL

using namespace ATEL;

CAtel::CAtel(CTask* pParent) : CTask(pParent, 0x8000), m_pVSM(NULL) {
    declareSysTask(0x1a);
    ignorePause(TRUE);
    CGameWork::setFlag(1, 6, FALSE);
    m_State[0].set(this, &CAtel::preNop, &CAtel::nop, NULL);
    m_State[1].set(this, &CAtel::preSleep, &CAtel::sleeping, NULL);
    m_State[2].set(this, &CAtel::preInitActor, &CAtel::initActor, NULL);
    m_State[3].set(this, &CAtel::preRun, &CAtel::running, NULL);
}

CAtel::~CAtel() {
    TRACE("CAtel::~CAtel()\n");
    if (m_pVSM) delete m_pVSM;
}

#line 97
static inline CFadeIOMask* _newFader(INT nLayer) {
    return new KH_AT("(98)") CFadeIOMask(*CTaskMan::getSysTask(0), nLayer);
}

void CAtel::loadFromMem(const BYTE* pAddr) {
    if (!pAddr) return;
    if (!m_pVSM) {
#line 106
        m_pVSM = new KH_AT("(106)") CVSM(*this, 0);
#line 107
        ASSERT(m_pVSM);
    }
    m_pVSM->loadFromMem(pAddr);
    CFadeIOMask* p = s_pFadeIOMask[1];
    if (p) p->off();
}

void CAtel::setupActor() { changeState(2); }

BOOL CAtel::canStart() const {
    if (m_pVSM) return !m_pVSM->isFlagOn(1);
    return FALSE;
}

void CAtel::start() {
    if (!m_pVSM) return;
    m_pVSM->clearFlag(2);
    if (isFadeActive()) {
        CFadeIOMask* p = s_pFadeIOMask[0];
        if (p && p->isSuspended()) p->resume();
        p = s_pFadeIOMask[1];
        if (p && p->isSuspended()) p->resume();
    }
}

BOOL CAtel::isStarted() const {
    if (m_pVSM) return !m_pVSM->isFlagOn(2);
    return FALSE;
}

void CAtel::main() { update(); }

void CAtel::changeState(INT nStat) {
#line 174
    KH_DEBUG(if (!((0 <= nStat) && (4 > nStat))) AssertApp(__FILE__, __LINE__, "(_STATE_NOP <= nStat) && ( _STATE_MAX > nStat)", 0);)
    if (m_pVSM) {
        m_nFsmState = nStat;
        gotoState(m_State[nStat]);
    }
}

void CAtel::preInitActor() {
#line 186
    ASSERT(m_pVSM);
}

void CAtel::initActor() {
    m_pVSM->execInit();
    changeState(3);
}

void CAtel::preRun() {
#line 202
    ASSERT(m_pVSM);
}

void CAtel::running() {
    int TQ = getFrameRate();
    BOOL bPause = FALSE;
    if (CMainApp::isWarningPauseNow() || CMainApp::isSysPauseNow() || CMainApp::isDbgUsrPauseNow()) bPause = TRUE;
    m_pVSM->exec(bPause ? 0 : TQ);
}

void CAtel::preNop() {}

void CAtel::nop() {}

void CAtel::preSleep() {}

void CAtel::sleeping() {}

#define AVATAR_ENTITY(p) ((CEntity*)((BYTE*)(p) + 0x10))
#define AVATAR_INT(p, ofs) (*(INT*)((BYTE*)(p) + (ofs)))

BOOL CAtel::createAvatar(INT nRsrcID, Actor& act, INT flag) {
    AvatarModelDispParam param;
    if (act._pAvatar) return FALSE;
    CEntity* pEntity = CEntityFactory::getInstance()->createEntity(10, nRsrcID, this, NULL);
    act._pAvatar = pEntity ? (CAvatar*)((BYTE*)pEntity - 0x10) : NULL;
    if (act._pAvatar) {
        AVATAR_INT(act._pAvatar, KH_VER(0x1e4, 0x1c4)) = act._ATP.id;
        AVATAR_INT(act._pAvatar, KH_VER(0x1e0, 0x1c0)) = m_pVSM->getActorIndex(act._ATP.name);
        act._pAvatar->InitAvatar(flag);
        param._iNo = 0;
        param._nState = 3;
        switch (nRsrcID) {
        case 0x3ea:
            if (CRcf::getMapID(CSceneInfo::getCurrentAreaNum(), CSceneInfo::getCurrentSceneNum()) == 0x927c5) {
                AVATAR_ENTITY(act._pAvatar)->createLegIK(-1.0f, -1.0f);
                AVATAR_ENTITY(act._pAvatar)->setGroundIK(0);
                AVATAR_ENTITY(act._pAvatar)->setFootEffectIK("asiato", 0);
            }
            break;
        case 0x3ed:
            if (CRcf::getMapID(CSceneInfo::getCurrentAreaNum(), CSceneInfo::getCurrentSceneNum()) == 0x927c5) {
                AVATAR_ENTITY(act._pAvatar)->createLegIK(-1.0f, -1.0f);
                AVATAR_ENTITY(act._pAvatar)->setGroundIK(0);
                AVATAR_ENTITY(act._pAvatar)->setFootEffectIK("asiato_poo", 0);
            }
            break;
        case 3:
            param._iNo = 4;
            act._pAvatar->reqCommand(7, &param, 0);
            param._iNo = 5;
            act._pAvatar->reqCommand(7, &param, 0);
            param._iNo = 6;
            act._pAvatar->reqCommand(7, &param, 0);
            break;
        case 0x517:
            param._iNo = 2;
            act._pAvatar->reqCommand(7, &param, 0);
            param._iNo = 3;
            act._pAvatar->reqCommand(7, &param, 0);
            param._iNo = 4;
            act._pAvatar->reqCommand(7, &param, 0);
            break;
        case 0x4bc:
            AVATAR_ENTITY(act._pAvatar)->setEntityFlag(0, 0x1000);
            break;
        case 0x525:
            AVATAR_INT(act._pAvatar, 0xf0) = 3;
            break;
        }
    }
    return act._pAvatar != NULL;
}

CEntity* CAtel::createAvatar(INT inRsrcID, CTask* inParent) {
#line 323
    CAvatar* p = new KH_AT("(323)") CAvatar(inParent);
    return p ? AVATAR_ENTITY(p) : NULL;
}

BOOL CAtel::createEventTrap(const LineTrapSrc& src, Actor& act) {
#line 331
    CEventLine* p = new KH_AT("(331)") CEventLine;
    if (p) {
        p->setData(src);
        act._pEventTrap = p;
        return TRUE;
    }
    return FALSE;
}

BOOL CAtel::createEventTrap(const BoxTrapSrc& src, Actor& act) {
#line 345
    CEventBox* pBox = new KH_AT("(345)") CEventBox;
    if (pBox) {
        pBox->setData(src);
        act._pEventTrap = pBox;
        return TRUE;
    }
    return FALSE;
}

void CAtel::skipEvent() {
    CAtel* p = (CAtel*)CTaskMan::getSysTask(0x1a);
    if (p && p->m_pVSM) p->m_pVSM->AtelEventSkip();
}

static inline BOOL _isActive(CFadeIOMask* p) { return p && !p->isInactive() && !p->isInvisible(); }

void CAtel::setScreenFader(INT nKind, FLOAT fRate, INT nLayer) {
    /* MATCH: TRUE and default written out alike; jump2 cross-jumps them after reload, which leaves the
     * original's dead `li v0,1` of the TRUE compare in front of an unconditional branch. */
    switch (nLayer) {
    case FALSE:
        nLayer = 1;
        break;
    case TRUE:
        nLayer = 10;
        break;
    default:
        nLayer = 10;
        break;
    }
    INT nFaderID = (nKind & 0x80) ? 1 : 0;
    CFadeIOMask*& pFader = s_pFadeIOMask[nFaderID];
    nKind &= 7;
    if (!pFader) {
        pFader = _newFader(nLayer);
    } else {
        pFader->setLayer(nLayer);
    }
    pFader->on();
    if (nFaderID == 1 && _isActive(s_pFadeIOMask[0])) {
        s_pFadeIOMask[0]->suspend();
        s_pFadeIOMask[0]->clearReq();
    }
    USHORT rate = fRate * 60.0f;
    switch (nKind) {
    case 0:
        pFader->reqBlackOut(rate, 1.0f);
        break;
    case 1:
        pFader->setBlack(0.0f);
        pFader->reqBlackIn(rate, 0.0f);
        break;
    case 2:
        pFader->reqWhiteOut(rate, 0.0f);
        break;
    case 3:
        pFader->setWhite(1.0f);
        pFader->reqWhiteIn(rate, 1.0f);
        break;
    case 4:
        pFader->setBlack(0.0f);
        break;
    case 5:
        pFader->setWhite(1.0f);
        break;
    }
    if (!m_pVSM || m_pVSM->isFlagOn(2)) pFader->suspend();
    if (CGame::getFadeMaskProcPrio() != 1) CGame::dispOffFadeMask();
}

void CAtel::reqWhiteFadeOut(USHORT nTime) {
    CFadeIOMask*& pFader = s_pFadeIOMask[0];
    if (!pFader) pFader = _newFader(10);
    else pFader->setLayer(10);
    pFader->on();
    pFader->reqWhiteOut(nTime, 0.0f);
    if (CGame::getFadeMaskProcPrio() != 1) CGame::dispOffFadeMask();
}

void CAtel::reqWhiteFadeIn(USHORT nTime) {
    CFadeIOMask*& pFader = s_pFadeIOMask[0];
    if (!pFader) pFader = _newFader(10);
    else pFader->setLayer(10);
    pFader->on();
    pFader->reqWhiteIn(nTime, 1.0f);
    if (CGame::getFadeMaskProcPrio() != 1) CGame::dispOffFadeMask();
}

void CAtel::clearScreenFader(INT nID) {
    if (nID < 0) {
        if (s_pFadeIOMask[0]) s_pFadeIOMask[0]->off();
        if (s_pFadeIOMask[1]) s_pFadeIOMask[1]->off();
    } else if (nID < 2) {
        if (s_pFadeIOMask[nID]) s_pFadeIOMask[nID]->off();
    }
}

BOOL CAtel::isFadeActive() {
    BOOL bRes = FALSE;
    if (_isActive(s_pFadeIOMask[1]) || _isActive(s_pFadeIOMask[0])) bRes = TRUE;
    return bRes;
}

static inline BOOL _isFadeBlack(CFadeIOMask* p) {
    return p && !p->isInactive() && !p->isInvisible() && p->isBlack();
}

static inline BOOL _isFadeWhite(CFadeIOMask* p) {
    return p && !p->isInactive() && !p->isInvisible() && p->isWhite();
}

BOOL CAtel::isFadeActiveBlack() {
    if (_isFadeBlack(s_pFadeIOMask[1])) return TRUE;
    BOOL bRes = FALSE;
    if (!_isActive(s_pFadeIOMask[1])) bRes = _isFadeBlack(s_pFadeIOMask[0]);
    return bRes;
}

BOOL CAtel::isFadeActiveWhite() {
    if (_isFadeWhite(s_pFadeIOMask[1])) return TRUE;
    BOOL bRes = FALSE;
    if (!_isActive(s_pFadeIOMask[1])) bRes = _isFadeWhite(s_pFadeIOMask[0]);
    return bRes;
}

void CAtel::createScreenFader() {
    if (!s_pFadeIOMask[0]) s_pFadeIOMask[0] = _newFader(1);
    if (!s_pFadeIOMask[1]) s_pFadeIOMask[1] = _newFader(1);
}

void CAtel::disposeScreenFader() {
    delete s_pFadeIOMask[0];
    s_pFadeIOMask[0] = NULL;
    delete s_pFadeIOMask[1];
    s_pFadeIOMask[1] = NULL;
}

BOOL CAtel::checkAtelPause() { return !CGameWork::getFlag(1, 6); }

KH_DEBUG(void CAtel::setWorldCompleteFlag(INT in_WorldID) {)
    KH_DEBUG(CJiminyMemoFlag::checkEvent(1, in_WorldID, 0);)
    KH_DEBUG(if (CGame::isSoraGame()) {)
        KH_DEBUG(switch (in_WorldID) {)
        KH_DEBUG(case 0:)
            KH_DEBUG(break;)
        KH_DEBUG(case 1:)
            KH_DEBUG(setWorldTitleEnd(1);)
            KH_DEBUG(setTutorialEnd(1);)
            KH_DEBUG(setTutorialEnd(0x10);)
            KH_DEBUG(setTutorialEnd(0x40);)
            KH_DEBUG(CCard::addCard(0x63, 3);)
            KH_DEBUG(break;)
        KH_DEBUG(case 2:)
            KH_DEBUG(setWorldTitleEnd(2);)
            KH_DEBUG(setFriendPossible(4);)
            KH_DEBUG(CTechnic::addTechnic(0x4b);)
            KH_DEBUG(CTechnic::addTechnic(0x4c);)
            KH_DEBUG(setCardPossible(0x400000);)
            KH_DEBUG(setCardPossible(0x4000);)
            KH_DEBUG(CTechnic::addTechnic(0x3d);)
            KH_DEBUG(CTechnic::addTechnic(0x3e);)
            KH_DEBUG(CCard::addCard(0x2d, 3);)
            KH_DEBUG(CCard::addCard(0x24, 6);)
            KH_DEBUG(CCard::addCard(0x69, 6);)
            KH_DEBUG(break;)
        KH_DEBUG(case 3:)
            KH_DEBUG(setWorldTitleEnd(4);)
            KH_DEBUG(setCardPossible(0x100000);)
            KH_DEBUG(setCardPossible(0x80000);)
            KH_DEBUG(CTechnic::addTechnic(0x41);)
            KH_DEBUG(CTechnic::addTechnic(0x42);)
            KH_DEBUG(CCard::addCard(0x2b, 0);)
            KH_DEBUG(CCard::addCard(0x29, 0);)
            KH_DEBUG(CCard::addCard(0x68, 0);)
            KH_DEBUG(break;)
        KH_DEBUG(case 4:)
            KH_DEBUG(setWorldTitleEnd(8);)
            KH_DEBUG(CCard::addCard(0x67, 0);)
            KH_DEBUG(CCard::addCard(0x65, 0);)
            KH_DEBUG(break;)
        KH_DEBUG(case 5:)
            KH_DEBUG(setWorldTitleEnd(0x10);)
            KH_DEBUG(CCard::addCard(0x64, 0);)
            KH_DEBUG(setCardPossible(0x10000);)
            KH_DEBUG(CTechnic::addTechnic(0x36);)
            KH_DEBUG(CTechnic::addTechnic(0x37);)
            KH_DEBUG(CCard::addCard(0x26, 3);)
            KH_DEBUG(break;)
        KH_DEBUG(case 6:)
            KH_DEBUG(setWorldTitleEnd(0x20);)
            KH_DEBUG(setFriendPossible(0x10);)
            KH_DEBUG(CTechnic::addTechnic(0x4d);)
            KH_DEBUG(CTechnic::addTechnic(0x4e);)
            KH_DEBUG(CCard::addCard(0x6a, 0);)
            KH_DEBUG(CTechnic::addTechnic(0x2f);)
            KH_DEBUG(break;)
        KH_DEBUG(case 7:)
            KH_DEBUG(setWorldTitleEnd(0x40);)
            KH_DEBUG(setFriendPossible(8);)
            KH_DEBUG(CTechnic::addTechnic(0x4f);)
            KH_DEBUG(CTechnic::addTechnic(0x50);)
            KH_DEBUG(CCard::addCard(0x6b, 0);)
            KH_DEBUG(break;)
        KH_DEBUG(case 8:)
            KH_DEBUG(setWorldTitleEnd(0x80);)
            KH_DEBUG(setFriendPossible(0x20);)
            KH_DEBUG(CTechnic::addTechnic(0x51);)
            KH_DEBUG(CTechnic::addTechnic(0x52);)
            KH_DEBUG(CCard::addCard(0x6c, 0);)
            KH_DEBUG(break;)
        KH_DEBUG(case 9:)
            KH_DEBUG(setWorldTitleEnd(0x100);)
            KH_DEBUG(setFriendPossible(0x40);)
            KH_DEBUG(CTechnic::addTechnic(0x53);)
            KH_DEBUG(CTechnic::addTechnic(0x54);)
            KH_DEBUG(CCard::addCard(0x6d, 0);)
            KH_DEBUG(break;)
        KH_DEBUG(case 10:)
            KH_DEBUG(setWorldTitleEnd(0x200);)
            KH_DEBUG(break;)
        KH_DEBUG(case 11:)
            KH_DEBUG(setWorldTitleEnd(0x400);)
            KH_DEBUG(CCard::addCard(0x71, 0);)
            KH_DEBUG(break;)
        KH_DEBUG(case 12:)
            KH_DEBUG(setWorldTitleEnd(0x800);)
            KH_DEBUG(setCardPossible(8);)
            KH_DEBUG(CCard::addCard(0x66, 0);)
            KH_DEBUG(CCard::addCard(0xd, 4);)
            KH_DEBUG(break;)
        KH_DEBUG(case 13:)
            KH_DEBUG(setWorldTitleEnd(0x1000);)
            KH_DEBUG(break;)
        KH_DEBUG(})
    KH_DEBUG(} else {)
        KH_DEBUG(switch (in_WorldID) {)
        KH_DEBUG(case 0:)
            KH_DEBUG(break;)
        KH_DEBUG(case 1:)
            KH_DEBUG(setWorldTitleEnd(0x10000);)
            KH_DEBUG(break;)
        KH_DEBUG(case 2:)
            KH_DEBUG(setWorldTitleEnd(0x20000);)
            KH_DEBUG(CCard::addCard(0x69, 6);)
            KH_DEBUG(break;)
        KH_DEBUG(case 3:)
            KH_DEBUG(setWorldTitleEnd(0x40000);)
            KH_DEBUG(CCard::addCard(0x68, 0);)
            KH_DEBUG(break;)
        KH_DEBUG(case 4:)
            KH_DEBUG(setWorldTitleEnd(0x80000);)
            KH_DEBUG(break;)
        KH_DEBUG(case 5:)
            KH_DEBUG(setWorldTitleEnd(0x100000);)
            KH_DEBUG(break;)
        KH_DEBUG(case 6:)
            KH_DEBUG(setWorldTitleEnd(0x200000);)
            KH_DEBUG(CCard::addCard(0x6a, 0);)
            KH_DEBUG(break;)
        KH_DEBUG(case 7:)
            KH_DEBUG(setWorldTitleEnd(0x400000);)
            KH_DEBUG(break;)
        KH_DEBUG(case 8:)
            KH_DEBUG(setWorldTitleEnd(0x800000);)
            KH_DEBUG(break;)
        KH_DEBUG(case 9:)
            KH_DEBUG(setWorldTitleEnd(0x1000000);)
            KH_DEBUG(CCard::addCard(0x6d, 0);)
            KH_DEBUG(break;)
        KH_DEBUG(case 10:)
            KH_DEBUG(setWorldTitleEnd(0x2000000);)
            KH_DEBUG(break;)
        KH_DEBUG(case 11:)
            KH_DEBUG(setWorldTitleEnd(0x4000000);)
            KH_DEBUG(setMoviePossible(0x2a, 0x20000000);)
            KH_DEBUG(setMoviePossible(0x2a, 0x40000000);)
            KH_DEBUG(setMoviePossible(0x2a, 0x80000000);)
            KH_DEBUG(setMoviePossible(0x2b, 1);)
            KH_DEBUG(break;)
        KH_DEBUG(case 12:)
            KH_DEBUG(setWorldTitleEnd(0x8000000);)
            KH_DEBUG(CCard::addCard(0x66, 0);)
            KH_DEBUG(break;)
        KH_DEBUG(case 13:)
            KH_DEBUG(setWorldTitleEnd(0x10000000);)
            KH_DEBUG(break;)
        KH_DEBUG(})
    KH_DEBUG(})
KH_DEBUG(})

template <class T> static inline T& _ref(T& v) { return v; }

KH_DEBUG(void CAtel::setFloorCompleteFlag(INT in_Floor, INT in_RoomNo) {)
    KH_DEBUG(INT flag = FALSE;)
    KH_DEBUG(INT Index;)

    KH_DEBUG(CGameWork::setByte(0, 0x61, 1);)
    KH_DEBUG(if (CGame::isSoraGame()) {)
        KH_DEBUG(switch (in_Floor) {)
        KH_DEBUG(case 13:)
        KH_DEBUG(case 12:)
            KH_DEBUG(CGameWork::setByte(0, 0xb, 12);)
            KH_DEBUG(break;)
        KH_DEBUG(case 11:)
            KH_DEBUG(CGameWork::setByte(0, 0xb, 11);)
            KH_DEBUG(break;)
        KH_DEBUG(case 10:)
            KH_DEBUG(CGameWork::setByte(0, 0xb, 10);)
            KH_DEBUG(break;)
        KH_DEBUG(case 9:)
        KH_DEBUG(case 8:)
        KH_DEBUG(case 7:)
        KH_DEBUG(case 6:)
            KH_DEBUG(CGameWork::setByte(0, 0xb, 6);)
            KH_DEBUG(break;)
        KH_DEBUG(case 5:)
        KH_DEBUG(case 4:)
        KH_DEBUG(case 3:)
        KH_DEBUG(case 2:)
        KH_DEBUG(case 1:)
            KH_DEBUG(CGameWork::setByte(0, 0xb, 1);)
            KH_DEBUG(break;)
        KH_DEBUG(case 0:)
            KH_DEBUG(CGameWork::setByte(0, 0xb, 0);)
            KH_DEBUG(break;)
        KH_DEBUG(})
    KH_DEBUG(} else {)
        KH_DEBUG(switch (in_Floor) {)
        KH_DEBUG(case 0:)
        KH_DEBUG(case -1:)
            KH_DEBUG(CGameWork::setByte(0, 0xb, 11);)
            KH_DEBUG(break;)
        KH_DEBUG(case -2:)
            KH_DEBUG(CGameWork::setByte(0, 0xb, 10);)
            KH_DEBUG(break;)
        KH_DEBUG(case -3:)
            KH_DEBUG(CGameWork::setByte(0, 0xb, 9);)
            KH_DEBUG(break;)
        KH_DEBUG(case -4:)
        KH_DEBUG(case -5:)
        KH_DEBUG(case -6:)
        KH_DEBUG(case -7:)
            KH_DEBUG(CGameWork::setByte(0, 0xb, 5);)
            KH_DEBUG(break;)
        KH_DEBUG(case -8:)
        KH_DEBUG(case -9:)
        KH_DEBUG(case -10:)
        KH_DEBUG(case -11:)
            KH_DEBUG(CGameWork::setByte(0, 0xb, 1);)
            KH_DEBUG(break;)
        KH_DEBUG(case -12:)
            KH_DEBUG(CGameWork::setByte(0, 0xb, 0);)
            KH_DEBUG(break;)
        KH_DEBUG(})
    KH_DEBUG(})

    KH_DEBUG(if (CGame::isSoraGame()) {)
    KH_DEBUG(switch (in_Floor) {)
    KH_DEBUG(case 13:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(13, TRUE);)
        KH_DEBUG(CGameWork::setInt(0, 0x33, 12);)
        KH_DEBUG(if (in_RoomNo == 0) {)
            KH_DEBUG(CGameWork::setInt(0, 0x25, 1300);)
        KH_DEBUG(} else {)
            KH_DEBUG(CGameWork::setInt(0, 0x25, 1310);)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x19;)
        KH_DEBUG(})
        KH_DEBUG(setFriendPossible(1);)
        KH_DEBUG(setFriendPossible(2);)
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case 12:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(12, TRUE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x33, 11);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 1200);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 1250);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x17;)
                KH_DEBUG(setFriendPossible(1);)
                KH_DEBUG(setFriendPossible(2);)
                KH_DEBUG(setMoviePossible(0x28, 0x400);)
                KH_DEBUG(CGameWork::setInt(0, 0x33, 11);)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 1210);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x17;)
                KH_DEBUG(setMoviePossible(0x28, 0x400);)
                KH_DEBUG(break;)
            KH_DEBUG(})
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x17;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x18;)
            KH_DEBUG(CCard::addCard(0x6e, 0);)
            KH_DEBUG(setMoviePossible(0x28, 0x400);)
            KH_DEBUG(setMoviePossible(0x28, 0x2000);)
            KH_DEBUG(setMoviePossible(0x28, 0x4000);)
            KH_DEBUG(setMoviePossible(0x28, 0x8000);)
        KH_DEBUG(})
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case 11:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(11, TRUE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x33, 10);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 1100);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 1150);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x15;)
                KH_DEBUG(setMoviePossible(0x28, 0x2);)
                KH_DEBUG(setMoviePossible(0x28, 0x4);)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 1110);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x15;)
                KH_DEBUG(setMoviePossible(0x28, 0x2);)
                KH_DEBUG(setMoviePossible(0x28, 0x4);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(setFriendPossible(1);)
            KH_DEBUG(setFriendPossible(2);)
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x15;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x16;)
            KH_DEBUG(setMoviePossible(0x28, 0x2);)
            KH_DEBUG(setMoviePossible(0x28, 0x4);)
            KH_DEBUG(setMoviePossible(0x28, 0x20);)
            KH_DEBUG(setMoviePossible(0x28, 0x40);)
            KH_DEBUG(setMoviePossible(0x28, 0x80);)
            KH_DEBUG(setMoviePossible(0x28, 0x100);)
            KH_DEBUG(CCard::addCard(0x2e, 4);)
            KH_DEBUG(setCardPossible(0x800000);)
        KH_DEBUG(})
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case 10:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(10, TRUE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x33, 9);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 1000);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 1050);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x13;)
                KH_DEBUG(setMoviePossible(0x27, 0x20000000);)
                KH_DEBUG(setMoviePossible(0x27, 0x40000000);)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 1010);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x13;)
                KH_DEBUG(setMoviePossible(0x27, 0x20000000);)
                KH_DEBUG(setMoviePossible(0x27, 0x40000000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(setFriendPossible(1);)
            KH_DEBUG(setFriendPossible(2);)
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x13;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x14;)
            KH_DEBUG(setMoviePossible(0x27, 0x20000000);)
            KH_DEBUG(setMoviePossible(0x27, 0x40000000);)
            KH_DEBUG(setMoviePossible(0x27, 0x80000000);)
            KH_DEBUG(setMoviePossible(0x28, 0x1);)
        KH_DEBUG(})
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case 9:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(9, TRUE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x33, 8);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 900);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 950);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x11;)
                KH_DEBUG(setMoviePossible(0x27, 0x4000000);)
                KH_DEBUG(setMoviePossible(0x27, 0x8000000);)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 910);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x11;)
                KH_DEBUG(setMoviePossible(0x27, 0x4000000);)
                KH_DEBUG(setMoviePossible(0x27, 0x8000000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(setFriendPossible(1);)
            KH_DEBUG(setFriendPossible(2);)
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x11;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x12;)
            KH_DEBUG(setMoviePossible(0x27, 0x4000000);)
            KH_DEBUG(setMoviePossible(0x27, 0x8000000);)
            KH_DEBUG(setMoviePossible(0x27, 0x10000000);)
        KH_DEBUG(})
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case 8:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(8, TRUE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x33, 7);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 800);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 850);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0xf;)
                KH_DEBUG(setMoviePossible(0x27, 0x800000);)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 810);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0xf;)
                KH_DEBUG(setMoviePossible(0x27, 0x800000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(setFriendPossible(1);)
            KH_DEBUG(setFriendPossible(2);)
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0xf;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x10;)
            KH_DEBUG(setMoviePossible(0x27, 0x800000);)
            KH_DEBUG(setMoviePossible(0x27, 0x1000000);)
            KH_DEBUG(setMoviePossible(0x27, 0x2000000);)
        KH_DEBUG(})
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case 7:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(7, TRUE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x33, 6);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 700);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 750);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0xd;)
                KH_DEBUG(setMoviePossible(0x27, 0x80000);)
                KH_DEBUG(setMoviePossible(0x27, 0x100000);)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 710);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0xd;)
                KH_DEBUG(setMoviePossible(0x27, 0x80000);)
                KH_DEBUG(setMoviePossible(0x27, 0x100000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(setFriendPossible(1);)
            KH_DEBUG(setFriendPossible(2);)
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0xd;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0xe;)
            KH_DEBUG(setMoviePossible(0x27, 0x80000);)
            KH_DEBUG(setMoviePossible(0x27, 0x100000);)
            KH_DEBUG(setMoviePossible(0x27, 0x200000);)
            KH_DEBUG(setMoviePossible(0x27, 0x400000);)
        KH_DEBUG(})
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case 6:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(6, TRUE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x33, 5);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 600);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 650);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0xb;)
                KH_DEBUG(setMoviePossible(0x27, 0x8000);)
                KH_DEBUG(setMoviePossible(0x27, 0x10000);)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 610);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0xb;)
                KH_DEBUG(setMoviePossible(0x27, 0x8000);)
                KH_DEBUG(setMoviePossible(0x27, 0x10000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(setFriendPossible(1);)
            KH_DEBUG(setFriendPossible(2);)
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0xb;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0xc;)
            KH_DEBUG(setMoviePossible(0x27, 0x8000);)
            KH_DEBUG(setMoviePossible(0x27, 0x10000);)
            KH_DEBUG(setMoviePossible(0x27, 0x20000);)
            KH_DEBUG(setMoviePossible(0x27, 0x40000);)
        KH_DEBUG(})
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case 5:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(5, TRUE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x33, 4);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 500);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 550);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x9;)
                KH_DEBUG(setMoviePossible(0x27, 0x1000);)
                KH_DEBUG(setMoviePossible(0x27, 0x2000);)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 510);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x9;)
                KH_DEBUG(setMoviePossible(0x27, 0x1000);)
                KH_DEBUG(setMoviePossible(0x27, 0x2000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(setFriendPossible(1);)
            KH_DEBUG(setFriendPossible(2);)
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x9;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0xa;)
            KH_DEBUG(setMoviePossible(0x27, 0x1000);)
            KH_DEBUG(setMoviePossible(0x27, 0x2000);)
            KH_DEBUG(setMoviePossible(0x27, 0x4000);)
        KH_DEBUG(})
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case 4:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(4, TRUE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x33, 3);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 400);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 450);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x7;)
                KH_DEBUG(setMoviePossible(0x27, 0x200);)
                KH_DEBUG(setMoviePossible(0x27, 0x400);)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 410);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x7;)
                KH_DEBUG(setMoviePossible(0x27, 0x200);)
                KH_DEBUG(setMoviePossible(0x27, 0x400);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(setFriendPossible(1);)
            KH_DEBUG(setFriendPossible(2);)
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x7;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x8;)
            KH_DEBUG(setMoviePossible(0x27, 0x200);)
            KH_DEBUG(setMoviePossible(0x27, 0x400);)
            KH_DEBUG(setMoviePossible(0x27, 0x800);)
        KH_DEBUG(})
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case 3:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(3, TRUE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x33, 2);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 300);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 350);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x5;)
                KH_DEBUG(setMoviePossible(0x27, 0x80);)
                KH_DEBUG(setMoviePossible(0x27, 0x40);)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 310);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x5;)
                KH_DEBUG(setMoviePossible(0x27, 0x80);)
                KH_DEBUG(setMoviePossible(0x27, 0x40);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(setFriendPossible(1);)
            KH_DEBUG(setFriendPossible(2);)
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x5;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x6;)
            KH_DEBUG(setMoviePossible(0x27, 0x40);)
            KH_DEBUG(setMoviePossible(0x27, 0x80);)
            KH_DEBUG(setMoviePossible(0x27, 0x100);)
        KH_DEBUG(})
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case 2:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(2, TRUE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x33, 1);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 200);)
                KH_DEBUG(setTutorialEnd(4);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 250);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x3;)
                KH_DEBUG(setMoviePossible(0x27, 0x10);)
                KH_DEBUG(setTutorialEnd(8);)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 210);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x3;)
                KH_DEBUG(setMoviePossible(0x27, 0x10);)
                KH_DEBUG(setTutorialEnd(8);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(setFriendPossible(1);)
            KH_DEBUG(setFriendPossible(2);)
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x3;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x4;)
            KH_DEBUG(setMoviePossible(0x27, 0x10);)
            KH_DEBUG(setMoviePossible(0x27, 0x20);)
            KH_DEBUG(setTutorialEnd(4);)
            KH_DEBUG(setTutorialEnd(8);)
        KH_DEBUG(})
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case 1:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(1, TRUE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 0);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 150);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x1;)
                KH_DEBUG(setMoviePossible(0x27, 0x2);)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x25, 0);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x1;)
                KH_DEBUG(setMoviePossible(0x27, 0x2);)
                KH_DEBUG(if (in_RoomNo >= 3) {)
                    KH_DEBUG(setTutorialEnd(0x20);)
                    KH_DEBUG(setWorldTitleEnd(1);)
                KH_DEBUG(})
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(setFriendPossible(1);)
            KH_DEBUG(setFriendPossible(2);)
            KH_DEBUG(setFriendPossible(0x100);)
            KH_DEBUG(CTechnic::addTechnic(0x55);)
            KH_DEBUG(CTechnic::addTechnic(0x56);)
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x1;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x2;)
            KH_DEBUG(setMoviePossible(0x27, 0x2);)
            KH_DEBUG(setMoviePossible(0x27, 0x4);)
            KH_DEBUG(setMoviePossible(0x27, 0x8);)
            KH_DEBUG(setTutorialEnd(0x10);)
            KH_DEBUG(setTutorialEnd(0x20);)
        KH_DEBUG(})
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case 0:)
        KH_DEBUG(CTechnic::addTechnic(0x44);)
        KH_DEBUG(CTechnic::addTechnic(0x45);)
        KH_DEBUG(CTechnic::addTechnic(0x47);)
        KH_DEBUG(CTechnic::addTechnic(0x48);)
        KH_DEBUG(CTechnic::addTechnic(0x49);)
        KH_DEBUG(CTechnic::addTechnic(0x4a);)
        KH_DEBUG(CTechnic::addTechnic(0x10);)
        KH_DEBUG(CTechnic::addTechnic(0x17);)
        KH_DEBUG(CTechnic::addTechnic(0xe);)
        KH_DEBUG(CTechnic::addTechnic(0x15);)
        KH_DEBUG(break;)
    KH_DEBUG(})
    KH_DEBUG(} else {)
    KH_DEBUG(switch (in_Floor) {)
    KH_DEBUG(case 0:)
    KH_DEBUG(case -1:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(-1, FALSE);)
        KH_DEBUG(CGameWork::setInt(0, 0x34, 11);)
        KH_DEBUG(switch (in_RoomNo) {)
        KH_DEBUG(case 0:)
            KH_DEBUG(CGameWork::setInt(0, 0x26, 1200);)
            KH_DEBUG(break;)
        KH_DEBUG(case 0x63:)
            KH_DEBUG(CGameWork::setInt(0, 0x26, 1250);)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x7b;)
            KH_DEBUG(setMoviePossible(0x2b, 0x4);)
            KH_DEBUG(break;)
        KH_DEBUG(default:)
            KH_DEBUG(CGameWork::setInt(0, 0x26, 1210);)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x7b;)
            KH_DEBUG(setMoviePossible(0x2b, 0x4);)
            KH_DEBUG(break;)
        KH_DEBUG(})
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case -2:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(-2, FALSE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x34, 10);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 1100);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 1150);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x79;)
                KH_DEBUG(setMoviePossible(0x2a, 0x10000000);)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 1110);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x79;)
                KH_DEBUG(setMoviePossible(0x2a, 0x10000000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x79;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x7a;)
            KH_DEBUG(setMoviePossible(0x2a, 0x10000000);)
            KH_DEBUG(setMoviePossible(0x2b, 0x2);)
        KH_DEBUG(})
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case -3:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(-3, FALSE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x34, 9);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 1000);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 1050);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x77;)
                KH_DEBUG(setMoviePossible(0x2a, 0x400000);)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 1010);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x77;)
                KH_DEBUG(setMoviePossible(0x2a, 0x400000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x77;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x78;)
            KH_DEBUG(setMoviePossible(0x2a, 0x400000);)
            KH_DEBUG(setMoviePossible(0x2a, 0x8000000);)
        KH_DEBUG(})
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case -4:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(-4, FALSE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x34, 8);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 900);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 950);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x75;)
                KH_DEBUG(setMoviePossible(0x2a, 0x10000);)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 910);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x75;)
                KH_DEBUG(setMoviePossible(0x2a, 0x10000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x75;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x76;)
            KH_DEBUG(CCard::addCard(0x73, 0);)
            KH_DEBUG(setMoviePossible(0x2a, 0x10000);)
            KH_DEBUG(setMoviePossible(0x2a, 0x20000);)
            KH_DEBUG(setMoviePossible(0x2a, 0x40000);)
            KH_DEBUG(setMoviePossible(0x2a, 0x80000);)
            KH_DEBUG(setMoviePossible(0x2a, 0x100000);)
            KH_DEBUG(setMoviePossible(0x2a, 0x200000);)
        KH_DEBUG(})
        KH_DEBUG(setMoviePossible(0x2a, 0x8000);)
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case -5:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(-5, FALSE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x34, 7);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 800);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 850);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x73;)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 800);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x73;)
                KH_DEBUG(break;)
            KH_DEBUG(})
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x73;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x74;)
        KH_DEBUG(})
        KH_DEBUG(setMoviePossible(0x2a, 0x4000);)
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case -6:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(-6, FALSE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x34, 6);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 700);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 750);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x71;)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 700);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x71;)
                KH_DEBUG(break;)
            KH_DEBUG(})
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x71;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x72;)
        KH_DEBUG(})
        KH_DEBUG(setMoviePossible(0x2a, 0x2000);)
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case -7:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(-7, FALSE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x34, 5);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 600);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 650);)
                KH_DEBUG(setMoviePossible(0x2a, 0x1000);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x6f;)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 610);)
                KH_DEBUG(setMoviePossible(0x2a, 0x1000);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x6f;)
                KH_DEBUG(break;)
            KH_DEBUG(})
        KH_DEBUG(} else {)
            KH_DEBUG(setMoviePossible(0x2a, 0x1000);)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x6f;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x70;)
        KH_DEBUG(})
        KH_DEBUG(setMoviePossible(0x2a, 0x800);)
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case -8:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(-8, FALSE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x34, 4);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 500);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 550);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x6d;)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 500);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x6d;)
                KH_DEBUG(break;)
            KH_DEBUG(})
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x6d;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x6e;)
            KH_DEBUG(setMoviePossible(0x2a, 0x200);)
            KH_DEBUG(setMoviePossible(0x2a, 0x400);)
        KH_DEBUG(})
        KH_DEBUG(setMoviePossible(0x2a, 0x100);)
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case -9:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(-9, FALSE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x34, 3);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 400);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 450);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x6b;)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 400);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x6b;)
                KH_DEBUG(break;)
            KH_DEBUG(})
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x6b;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x6c;)
        KH_DEBUG(})
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case -10:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(-10, FALSE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x34, 2);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 300);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 350);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x69;)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 300);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x69;)
                KH_DEBUG(break;)
            KH_DEBUG(})
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x69;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x6a;)
            KH_DEBUG(setMoviePossible(0x2a, 0x40);)
            KH_DEBUG(setMoviePossible(0x2a, 0x80);)
        KH_DEBUG(})
        KH_DEBUG(setMoviePossible(0x2a, 0x20);)
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case -11:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(-11, FALSE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(CGameWork::setInt(0, 0x34, 1);)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 200);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 250);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x67;)
                KH_DEBUG(setMoviePossible(0x2a, 0x10);)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 210);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x67;)
                KH_DEBUG(setMoviePossible(0x2a, 0x10);)
                KH_DEBUG(break;)
            KH_DEBUG(})
        KH_DEBUG(} else {)
            KH_DEBUG(setMoviePossible(0x2a, 0x10);)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x67;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x68;)
        KH_DEBUG(})
        KH_DEBUG(setMoviePossible(0x2a, 0x8);)
        KH_DEBUG(setPlayerSkill(8);)
        KH_DEBUG(setPlayerSkill(0x10);)
        KH_DEBUG(setPlayerSkill(0x20);)
        KH_DEBUG(CTechnic::addTechnic(0x6d);)
        KH_DEBUG(setFriendPossible(0x80);)
        KH_DEBUG(flag = TRUE;)
    KH_DEBUG(case -12:)
        KH_DEBUG(Index = CSceneInfo::getFloorIndex(-12, FALSE);)
        KH_DEBUG(if (!flag) {)
            KH_DEBUG(switch (in_RoomNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 0);)
                KH_DEBUG(break;)
            KH_DEBUG(case 0x63:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 150);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x65;)
                KH_DEBUG(setMoviePossible(0x2a, 0x1);)
                KH_DEBUG(break;)
            KH_DEBUG(default:)
                KH_DEBUG(CGameWork::setInt(0, 0x26, 10);)
                KH_DEBUG(CGameWork::setInt(0, 0x34, 1);)
                KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x65;)
                KH_DEBUG(setMoviePossible(0x2a, 0x1);)
                KH_DEBUG(break;)
            KH_DEBUG(})
        KH_DEBUG(} else {)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x65;)
            KH_DEBUG(_ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x66;)
            KH_DEBUG(setMoviePossible(0x2a, 0x2);)
            KH_DEBUG(setMoviePossible(0x2a, 0x4);)
        KH_DEBUG(})
        KH_DEBUG(CTechnic::addTechnic(0x66);)
        KH_DEBUG(CTechnic::addTechnic(0x67);)
        KH_DEBUG(CTechnic::addTechnic(0x68);)
        KH_DEBUG(CTechnic::addTechnic(0x60);)
        KH_DEBUG(CTechnic::addTechnic(0x61);)
        KH_DEBUG(CTechnic::addTechnic(0x5f);)
        KH_DEBUG(CTechnic::addTechnic(0x69);)
        KH_DEBUG(CTechnic::addTechnic(0x6a);)
        KH_DEBUG(CTechnic::addTechnic(0x6b);)
        KH_DEBUG(CTechnic::addTechnic(0x62);)
        KH_DEBUG(CTechnic::addTechnic(0x63);)
        KH_DEBUG(CTechnic::addTechnic(0x64);)
        KH_DEBUG(CTechnic::addTechnic(0x65);)
        KH_DEBUG(CGameWork::setInt(0, 0xc2, 0x7d);)
        KH_DEBUG(break;)
    KH_DEBUG(})
    KH_DEBUG(})
KH_DEBUG(})

KH_DEBUG(void CAtel::setWorldStepFlag(INT in_WorldID, INT in_EventNo) {)
    KH_DEBUG(if (CGame::isSoraGame()) {)
        KH_DEBUG(switch (in_WorldID) {)
        KH_DEBUG(case 0:)
            KH_DEBUG(break;)
        KH_DEBUG(case 1:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 4:)
            KH_DEBUG(case 3:)
            KH_DEBUG(case 2:)
                KH_DEBUG(setTutorialEnd(1);)
                KH_DEBUG(setFriendPossible(0x100);)
                KH_DEBUG(CTechnic::addTechnic(0x55);)
                KH_DEBUG(CTechnic::addTechnic(0x56);)
            KH_DEBUG(case 1:)
                KH_DEBUG(CCard::addCard(0x23, 6);)
                KH_DEBUG(CTechnic::addTechnic(0x34);)
                KH_DEBUG(CTechnic::addTechnic(0x35);)
            KH_DEBUG(case 0:)
                KH_DEBUG(setTutorialEnd(0x40);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 2:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
                KH_DEBUG(CCard::addCard(0x69, 6);)
            KH_DEBUG(case 3:)
                KH_DEBUG(setCardPossible(0x400000);)
                KH_DEBUG(CCard::addCard(0x2d, 3);)
            KH_DEBUG(case 2:)
                KH_DEBUG(setWorldTitleEnd(2);)
            KH_DEBUG(case 1:)
                KH_DEBUG(setFriendPossible(4);)
                KH_DEBUG(CTechnic::addTechnic(0x4b);)
                KH_DEBUG(CTechnic::addTechnic(0x4c);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 3:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
            KH_DEBUG(case 1:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
            KH_DEBUG(case 3:)
                KH_DEBUG(setCardPossible(0x100000);)
                KH_DEBUG(CCard::addCard(0x2b, 0);)
            KH_DEBUG(case 2:)
                KH_DEBUG(setWorldTitleEnd(4);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 4:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
            KH_DEBUG(case 1:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
                KH_DEBUG(CCard::addCard(0x65, 0);)
            KH_DEBUG(case 3:)
            KH_DEBUG(case 2:)
                KH_DEBUG(CCard::addCard(0x67, 0);)
                KH_DEBUG(setWorldTitleEnd(8);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 5:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
            KH_DEBUG(case 1:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
                KH_DEBUG(setCardPossible(0x10000);)
                KH_DEBUG(CTechnic::addTechnic(0x36);)
                KH_DEBUG(CTechnic::addTechnic(0x37);)
                KH_DEBUG(CCard::addCard(0x26, 3);)
            KH_DEBUG(case 3:)
                KH_DEBUG(CCard::addCard(0x64, 0);)
            KH_DEBUG(case 2:)
                KH_DEBUG(setWorldTitleEnd(0x10);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 6:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
                KH_DEBUG(CCard::addCard(0x6a, 0);)
            KH_DEBUG(case 3:)
            KH_DEBUG(case 2:)
                KH_DEBUG(setWorldTitleEnd(0x20);)
            KH_DEBUG(case 1:)
                KH_DEBUG(setFriendPossible(0x10);)
                KH_DEBUG(CTechnic::addTechnic(0x4d);)
                KH_DEBUG(CTechnic::addTechnic(0x4e);)
                KH_DEBUG(CTechnic::addTechnic(0x2f);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 7:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
            KH_DEBUG(case 1:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
                KH_DEBUG(CCard::addCard(0x6b, 0);)
            KH_DEBUG(case 3:)
                KH_DEBUG(setFriendPossible(8);)
                KH_DEBUG(CTechnic::addTechnic(0x4f);)
                KH_DEBUG(CTechnic::addTechnic(0x50);)
            KH_DEBUG(case 2:)
                KH_DEBUG(setWorldTitleEnd(0x40);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 8:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
            KH_DEBUG(case 1:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
                KH_DEBUG(CCard::addCard(0x6c, 0);)
                KH_DEBUG(CCard::addCard(0x27, 0);)
                KH_DEBUG(CTechnic::addTechnic(0x3f);)
                KH_DEBUG(CTechnic::addTechnic(0x40);)
            KH_DEBUG(case 3:)
            KH_DEBUG(case 2:)
                KH_DEBUG(setFriendPossible(0x20);)
                KH_DEBUG(CTechnic::addTechnic(0x51);)
                KH_DEBUG(CTechnic::addTechnic(0x52);)
                KH_DEBUG(setWorldTitleEnd(0x80);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 9:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
                KH_DEBUG(CCard::addCard(0x6d, 0);)
            KH_DEBUG(case 3:)
            KH_DEBUG(case 2:)
                KH_DEBUG(setWorldTitleEnd(0x100);)
            KH_DEBUG(case 1:)
                KH_DEBUG(setFriendPossible(0x40);)
                KH_DEBUG(CTechnic::addTechnic(0x53);)
                KH_DEBUG(CTechnic::addTechnic(0x54);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 10:)
            KH_DEBUG(break;)
        KH_DEBUG(case 11:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
            KH_DEBUG(case 1:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
            KH_DEBUG(case 3:)
            KH_DEBUG(case 2:)
                KH_DEBUG(setWorldTitleEnd(0x400);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 12:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
            KH_DEBUG(case 1:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
            KH_DEBUG(case 3:)
            KH_DEBUG(case 2:)
                KH_DEBUG(CCard::addCard(0x66, 0);)
                KH_DEBUG(setWorldTitleEnd(0x800);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 13:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
            KH_DEBUG(case 3:)
            KH_DEBUG(case 2:)
                KH_DEBUG(setWorldTitleEnd(0x1000);)
                KH_DEBUG(setMoviePossible(0x28, 0x20000);)
                KH_DEBUG(setMoviePossible(0x28, 0x40000);)
                KH_DEBUG(setMoviePossible(0xc8, 0x20000);)
                KH_DEBUG(setMoviePossible(0xc8, 0x40000);)
            KH_DEBUG(case 1:)
                KH_DEBUG(setMoviePossible(0x28, 0x20000);)
                KH_DEBUG(setMoviePossible(0x28, 0x40000);)
                KH_DEBUG(setMoviePossible(0xc8, 0x20000);)
                KH_DEBUG(setMoviePossible(0xc8, 0x40000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(})
    KH_DEBUG(} else {)
        KH_DEBUG(switch (in_WorldID) {)
        KH_DEBUG(case 0:)
            KH_DEBUG(break;)
        KH_DEBUG(case 1:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
            KH_DEBUG(case 1:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
            KH_DEBUG(case 3:)
            KH_DEBUG(case 2:)
                KH_DEBUG(setWorldTitleEnd(0x10000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 2:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
            KH_DEBUG(case 1:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
            KH_DEBUG(case 3:)
            KH_DEBUG(case 2:)
                KH_DEBUG(setWorldTitleEnd(0x20000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 3:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
            KH_DEBUG(case 1:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
            KH_DEBUG(case 3:)
            KH_DEBUG(case 2:)
                KH_DEBUG(setWorldTitleEnd(0x40000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 4:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
            KH_DEBUG(case 1:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
            KH_DEBUG(case 3:)
            KH_DEBUG(case 2:)
                KH_DEBUG(setWorldTitleEnd(0x80000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 5:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
            KH_DEBUG(case 1:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
            KH_DEBUG(case 3:)
            KH_DEBUG(case 2:)
                KH_DEBUG(setWorldTitleEnd(0x100000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 6:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
            KH_DEBUG(case 1:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
            KH_DEBUG(case 3:)
            KH_DEBUG(case 2:)
                KH_DEBUG(setWorldTitleEnd(0x200000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 7:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
            KH_DEBUG(case 1:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
            KH_DEBUG(case 3:)
            KH_DEBUG(case 2:)
                KH_DEBUG(setWorldTitleEnd(0x400000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 8:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
            KH_DEBUG(case 1:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
            KH_DEBUG(case 3:)
            KH_DEBUG(case 2:)
                KH_DEBUG(setWorldTitleEnd(0x800000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 9:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
            KH_DEBUG(case 1:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
                KH_DEBUG(CCard::addCard(0x6d, 0);)
            KH_DEBUG(case 3:)
            KH_DEBUG(case 2:)
                KH_DEBUG(setWorldTitleEnd(0x1000000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 10:)
            KH_DEBUG(break;)
        KH_DEBUG(case 11:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
            KH_DEBUG(case 3:)
                KH_DEBUG(setMoviePossible(0x2b, 1);)
            KH_DEBUG(case 2:)
                KH_DEBUG(setMoviePossible(0x2a, 0x40000000);)
                KH_DEBUG(setMoviePossible(0x2a, 0x80000000);)
                KH_DEBUG(setWorldTitleEnd(0x4000000);)
            KH_DEBUG(case 1:)
                KH_DEBUG(setMoviePossible(0x2a, 0x20000000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 12:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
            KH_DEBUG(case 1:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
            KH_DEBUG(case 3:)
            KH_DEBUG(case 2:)
                KH_DEBUG(CCard::addCard(0x66, 0);)
                KH_DEBUG(setWorldTitleEnd(0x8000000);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(case 13:)
            KH_DEBUG(switch (in_EventNo) {)
            KH_DEBUG(case 0:)
                KH_DEBUG(break;)
            KH_DEBUG(case 4:)
            KH_DEBUG(case 3:)
            KH_DEBUG(case 2:)
                KH_DEBUG(setWorldTitleEnd(0x10000000);)
            KH_DEBUG(case 1:)
                KH_DEBUG(setMoviePossible(0x2b, 8);)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(break;)
        KH_DEBUG(})
    KH_DEBUG(})
KH_DEBUG(})

KH_DEBUG(void CAtel::setSceneDeSelectInitFlags() {)
    KH_DEBUG(CGameWork::setByte(0, 0x61, 1);)
    KH_DEBUG(if (CGame::isSoraGame()) return;)
    KH_DEBUG(setPlayerSkill(8);)
    KH_DEBUG(setPlayerSkill(0x10);)
    KH_DEBUG(setPlayerSkill(0x20);)
    KH_DEBUG(CTechnic::addTechnic(0x66);)
    KH_DEBUG(CTechnic::addTechnic(0x67);)
    KH_DEBUG(CTechnic::addTechnic(0x68);)
    KH_DEBUG(CTechnic::addTechnic(0x60);)
    KH_DEBUG(CTechnic::addTechnic(0x61);)
    KH_DEBUG(CTechnic::addTechnic(0x5f);)
    KH_DEBUG(CTechnic::addTechnic(0x69);)
    KH_DEBUG(CTechnic::addTechnic(0x6a);)
    KH_DEBUG(CTechnic::addTechnic(0x6b);)
    KH_DEBUG(setFriendPossible(0x80);)
    KH_DEBUG(CTechnic::addTechnic(0x62);)
    KH_DEBUG(CTechnic::addTechnic(0x63);)
    KH_DEBUG(CTechnic::addTechnic(0x64);)
    KH_DEBUG(CTechnic::addTechnic(0x65);)
KH_DEBUG(})

KH_DEBUG(void CAtel::setCardPossible(INT in_Card) { CGameWork::setInt(0, 0x21, CGameWork::getInt(0, 0x21) | in_Card); })

KH_DEBUG(void CAtel::setTutorialEnd(INT in_End) { CGameWork::setInt(0, 0xc2, CGameWork::getInt(0, 0xc2) | in_End); })

KH_DEBUG(void CAtel::setWorldTitleEnd(INT in_World) { CGameWork::setInt(0, 0xc1, CGameWork::getInt(0, 0xc1) | in_World); })

KH_DEBUG(void CAtel::setPlayerSkill(INT in_Skill) { CGameWork::setInt(0, 0x24, CGameWork::getInt(0, 0x24) | in_Skill); })

KH_DEBUG(void CAtel::setFriendPossible(INT in_Friend) { CGameWork::setInt(0, 0x22, CGameWork::getInt(0, 0x22) | in_Friend); })

KH_DEBUG(void CAtel::setMoviePossible(INT MovieStorageFlag, INT in_MovieNo) {)
    KH_DEBUG(CGameWork::setInt(0, MovieStorageFlag, CGameWork::getInt(0, MovieStorageFlag) | in_MovieNo);)
KH_DEBUG(})
// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .data .rodata
