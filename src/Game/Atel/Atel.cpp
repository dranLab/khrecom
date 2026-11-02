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
#include "include_asm.h"

#ifndef KH_PLACEMENT_NEW_DEFINED // MATCH: placement new is not in the ref headers (the units define it themselves); defined once per unit
#define KH_PLACEMENT_NEW_DEFINED
inline void* operator new(size_t, void* p) throw() { return p; }
#endif

#define ATEL_RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
#define ATEL_NEW(line, T) new (com_malloc_d(sizeof(T), __FILE__ "(" #line ")", ATEL_RET_ADDR())) T
#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

#include "Game/Atel/VSME.h"
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
    return ATEL_NEW(98, CFadeIOMask)(*CTaskMan::getSysTask(0), nLayer);
}

void CAtel::loadFromMem(const BYTE* pAddr) {
    if (!pAddr) return;
    if (!m_pVSM) {
#line 106
        m_pVSM = ATEL_NEW(106, CVSM)(*this, 0);
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
    if (!((0 <= nStat) && (4 > nStat))) AssertApp(__FILE__, __LINE__, "(_STATE_NOP <= nStat) && ( _STATE_MAX > nStat)", 0);
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
        AVATAR_INT(act._pAvatar, 0x1e4) = act._ATP.id;
        AVATAR_INT(act._pAvatar, 0x1e0) = m_pVSM->getActorIndex(act._ATP.name);
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
    CAvatar* p = new (com_malloc_d(0x3b0, __FILE__ "(323)", ATEL_RET_ADDR())) CAvatar(inParent);
    return p ? AVATAR_ENTITY(p) : NULL;
}

BOOL CAtel::createEventTrap(const LineTrapSrc& src, Actor& act) {
#line 331
    CEventLine* p = ATEL_NEW(331, CEventLine);
    if (p) {
        p->setData(src);
        act._pEventTrap = p;
        return TRUE;
    }
    return FALSE;
}

BOOL CAtel::createEventTrap(const BoxTrapSrc& src, Actor& act) {
#line 345
    CEventBox* pBox = ATEL_NEW(345, CEventBox);
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

/* Parked: 37 of 206 lines, 16 bytes short - logic, switch and jump table match. Left: the original computes the
 * layer as `beqz; b then (li v0,1); else: b end (li 1); then: li 10` (a jump-evaluated boolean threaded away) and
 * keeps nLayer in s1; ours branches once, which rotates the s-registers (nLayer/&pFader/nFaderID/nKind). Tried:
 * ternary, if/else, negated, bool/BOOL temps, a separate layer local, (nKind & 0x80) ? 1 : 0 (37, best). */
#if 0
void CAtel::setScreenFader(INT nKind, FLOAT fRate, INT nLayer) {
    if (nLayer) {
        nLayer = 10;
    } else {
        nLayer = 1;
    }
    INT nFaderID = (nKind & 0x80) ? 1 : 0;
    CFadeIOMask*& pFader = s_pFadeIOMask[nFaderID];
    nKind &= 7;
    if (!pFader) {
#line 98
        pFader = ATEL_NEW(98, CFadeIOMask)(*CTaskMan::getSysTask(0), nLayer);
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
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Atel", setScreenFader__5CAtelifi);
__asm__(".rdata\n.align 3\n.word .L002301D8\n.word .L002301F8\n.word .L00230234\n.word .L00230250\n.word .L0023029C\n.word .L002302C0\n.text\n");
#endif

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

void CAtel::setWorldCompleteFlag(INT in_WorldID) {
    CJiminyMemoFlag::checkEvent(1, in_WorldID, 0);
    if (CGame::isSoraGame()) {
        switch (in_WorldID) {
        case 0:
            break;
        case 1:
            setWorldTitleEnd(1);
            setTutorialEnd(1);
            setTutorialEnd(0x10);
            setTutorialEnd(0x40);
            CCard::addCard(0x63, 3);
            break;
        case 2:
            setWorldTitleEnd(2);
            setFriendPossible(4);
            CTechnic::addTechnic(0x4b);
            CTechnic::addTechnic(0x4c);
            setCardPossible(0x400000);
            setCardPossible(0x4000);
            CTechnic::addTechnic(0x3d);
            CTechnic::addTechnic(0x3e);
            CCard::addCard(0x2d, 3);
            CCard::addCard(0x24, 6);
            CCard::addCard(0x69, 6);
            break;
        case 3:
            setWorldTitleEnd(4);
            setCardPossible(0x100000);
            setCardPossible(0x80000);
            CTechnic::addTechnic(0x41);
            CTechnic::addTechnic(0x42);
            CCard::addCard(0x2b, 0);
            CCard::addCard(0x29, 0);
            CCard::addCard(0x68, 0);
            break;
        case 4:
            setWorldTitleEnd(8);
            CCard::addCard(0x67, 0);
            CCard::addCard(0x65, 0);
            break;
        case 5:
            setWorldTitleEnd(0x10);
            CCard::addCard(0x64, 0);
            setCardPossible(0x10000);
            CTechnic::addTechnic(0x36);
            CTechnic::addTechnic(0x37);
            CCard::addCard(0x26, 3);
            break;
        case 6:
            setWorldTitleEnd(0x20);
            setFriendPossible(0x10);
            CTechnic::addTechnic(0x4d);
            CTechnic::addTechnic(0x4e);
            CCard::addCard(0x6a, 0);
            CTechnic::addTechnic(0x2f);
            break;
        case 7:
            setWorldTitleEnd(0x40);
            setFriendPossible(8);
            CTechnic::addTechnic(0x4f);
            CTechnic::addTechnic(0x50);
            CCard::addCard(0x6b, 0);
            break;
        case 8:
            setWorldTitleEnd(0x80);
            setFriendPossible(0x20);
            CTechnic::addTechnic(0x51);
            CTechnic::addTechnic(0x52);
            CCard::addCard(0x6c, 0);
            break;
        case 9:
            setWorldTitleEnd(0x100);
            setFriendPossible(0x40);
            CTechnic::addTechnic(0x53);
            CTechnic::addTechnic(0x54);
            CCard::addCard(0x6d, 0);
            break;
        case 10:
            setWorldTitleEnd(0x200);
            break;
        case 11:
            setWorldTitleEnd(0x400);
            CCard::addCard(0x71, 0);
            break;
        case 12:
            setWorldTitleEnd(0x800);
            setCardPossible(8);
            CCard::addCard(0x66, 0);
            CCard::addCard(0xd, 4);
            break;
        case 13:
            setWorldTitleEnd(0x1000);
            break;
        }
    } else {
        switch (in_WorldID) {
        case 0:
            break;
        case 1:
            setWorldTitleEnd(0x10000);
            break;
        case 2:
            setWorldTitleEnd(0x20000);
            CCard::addCard(0x69, 6);
            break;
        case 3:
            setWorldTitleEnd(0x40000);
            CCard::addCard(0x68, 0);
            break;
        case 4:
            setWorldTitleEnd(0x80000);
            break;
        case 5:
            setWorldTitleEnd(0x100000);
            break;
        case 6:
            setWorldTitleEnd(0x200000);
            CCard::addCard(0x6a, 0);
            break;
        case 7:
            setWorldTitleEnd(0x400000);
            break;
        case 8:
            setWorldTitleEnd(0x800000);
            break;
        case 9:
            setWorldTitleEnd(0x1000000);
            CCard::addCard(0x6d, 0);
            break;
        case 10:
            setWorldTitleEnd(0x2000000);
            break;
        case 11:
            setWorldTitleEnd(0x4000000);
            setMoviePossible(0x2a, 0x20000000);
            setMoviePossible(0x2a, 0x40000000);
            setMoviePossible(0x2a, 0x80000000);
            setMoviePossible(0x2b, 1);
            break;
        case 12:
            setWorldTitleEnd(0x8000000);
            CCard::addCard(0x66, 0);
            break;
        case 13:
            setWorldTitleEnd(0x10000000);
            break;
        }
    }
}

template <class T> static inline T& _ref(T& v) { return v; }

void CAtel::setFloorCompleteFlag(INT in_Floor, INT in_RoomNo) {
    INT flag = FALSE;
    INT Index;

    CGameWork::setByte(0, 0x61, 1);
    if (CGame::isSoraGame()) {
        switch (in_Floor) {
        case 13:
        case 12:
            CGameWork::setByte(0, 0xb, 12);
            break;
        case 11:
            CGameWork::setByte(0, 0xb, 11);
            break;
        case 10:
            CGameWork::setByte(0, 0xb, 10);
            break;
        case 9:
        case 8:
        case 7:
        case 6:
            CGameWork::setByte(0, 0xb, 6);
            break;
        case 5:
        case 4:
        case 3:
        case 2:
        case 1:
            CGameWork::setByte(0, 0xb, 1);
            break;
        case 0:
            CGameWork::setByte(0, 0xb, 0);
            break;
        }
    } else {
        switch (in_Floor) {
        case 0:
        case -1:
            CGameWork::setByte(0, 0xb, 11);
            break;
        case -2:
            CGameWork::setByte(0, 0xb, 10);
            break;
        case -3:
            CGameWork::setByte(0, 0xb, 9);
            break;
        case -4:
        case -5:
        case -6:
        case -7:
            CGameWork::setByte(0, 0xb, 5);
            break;
        case -8:
        case -9:
        case -10:
        case -11:
            CGameWork::setByte(0, 0xb, 1);
            break;
        case -12:
            CGameWork::setByte(0, 0xb, 0);
            break;
        }
    }

    if (CGame::isSoraGame()) {
    switch (in_Floor) {
    case 13:
        Index = CSceneInfo::getFloorIndex(13, TRUE);
        CGameWork::setInt(0, 0x33, 12);
        if (in_RoomNo == 0) {
            CGameWork::setInt(0, 0x25, 1300);
        } else {
            CGameWork::setInt(0, 0x25, 1310);
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x19;
        }
        setFriendPossible(1);
        setFriendPossible(2);
        flag = TRUE;
    case 12:
        Index = CSceneInfo::getFloorIndex(12, TRUE);
        if (!flag) {
            CGameWork::setInt(0, 0x33, 11);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x25, 1200);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x25, 1250);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x17;
                setFriendPossible(1);
                setFriendPossible(2);
                setMoviePossible(0x28, 0x400);
                CGameWork::setInt(0, 0x33, 11);
                break;
            default:
                CGameWork::setInt(0, 0x25, 1210);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x17;
                setMoviePossible(0x28, 0x400);
                break;
            }
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x17;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x18;
            CCard::addCard(0x6e, 0);
            setMoviePossible(0x28, 0x400);
            setMoviePossible(0x28, 0x2000);
            setMoviePossible(0x28, 0x4000);
            setMoviePossible(0x28, 0x8000);
        }
        flag = TRUE;
    case 11:
        Index = CSceneInfo::getFloorIndex(11, TRUE);
        if (!flag) {
            CGameWork::setInt(0, 0x33, 10);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x25, 1100);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x25, 1150);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x15;
                setMoviePossible(0x28, 0x2);
                setMoviePossible(0x28, 0x4);
                break;
            default:
                CGameWork::setInt(0, 0x25, 1110);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x15;
                setMoviePossible(0x28, 0x2);
                setMoviePossible(0x28, 0x4);
                break;
            }
            setFriendPossible(1);
            setFriendPossible(2);
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x15;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x16;
            setMoviePossible(0x28, 0x2);
            setMoviePossible(0x28, 0x4);
            setMoviePossible(0x28, 0x20);
            setMoviePossible(0x28, 0x40);
            setMoviePossible(0x28, 0x80);
            setMoviePossible(0x28, 0x100);
            CCard::addCard(0x2e, 4);
            setCardPossible(0x800000);
        }
        flag = TRUE;
    case 10:
        Index = CSceneInfo::getFloorIndex(10, TRUE);
        if (!flag) {
            CGameWork::setInt(0, 0x33, 9);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x25, 1000);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x25, 1050);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x13;
                setMoviePossible(0x27, 0x20000000);
                setMoviePossible(0x27, 0x40000000);
                break;
            default:
                CGameWork::setInt(0, 0x25, 1010);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x13;
                setMoviePossible(0x27, 0x20000000);
                setMoviePossible(0x27, 0x40000000);
                break;
            }
            setFriendPossible(1);
            setFriendPossible(2);
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x13;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x14;
            setMoviePossible(0x27, 0x20000000);
            setMoviePossible(0x27, 0x40000000);
            setMoviePossible(0x27, 0x80000000);
            setMoviePossible(0x28, 0x1);
        }
        flag = TRUE;
    case 9:
        Index = CSceneInfo::getFloorIndex(9, TRUE);
        if (!flag) {
            CGameWork::setInt(0, 0x33, 8);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x25, 900);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x25, 950);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x11;
                setMoviePossible(0x27, 0x4000000);
                setMoviePossible(0x27, 0x8000000);
                break;
            default:
                CGameWork::setInt(0, 0x25, 910);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x11;
                setMoviePossible(0x27, 0x4000000);
                setMoviePossible(0x27, 0x8000000);
                break;
            }
            setFriendPossible(1);
            setFriendPossible(2);
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x11;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x12;
            setMoviePossible(0x27, 0x4000000);
            setMoviePossible(0x27, 0x8000000);
            setMoviePossible(0x27, 0x10000000);
        }
        flag = TRUE;
    case 8:
        Index = CSceneInfo::getFloorIndex(8, TRUE);
        if (!flag) {
            CGameWork::setInt(0, 0x33, 7);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x25, 800);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x25, 850);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0xf;
                setMoviePossible(0x27, 0x800000);
                break;
            default:
                CGameWork::setInt(0, 0x25, 810);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0xf;
                setMoviePossible(0x27, 0x800000);
                break;
            }
            setFriendPossible(1);
            setFriendPossible(2);
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0xf;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x10;
            setMoviePossible(0x27, 0x800000);
            setMoviePossible(0x27, 0x1000000);
            setMoviePossible(0x27, 0x2000000);
        }
        flag = TRUE;
    case 7:
        Index = CSceneInfo::getFloorIndex(7, TRUE);
        if (!flag) {
            CGameWork::setInt(0, 0x33, 6);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x25, 700);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x25, 750);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0xd;
                setMoviePossible(0x27, 0x80000);
                setMoviePossible(0x27, 0x100000);
                break;
            default:
                CGameWork::setInt(0, 0x25, 710);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0xd;
                setMoviePossible(0x27, 0x80000);
                setMoviePossible(0x27, 0x100000);
                break;
            }
            setFriendPossible(1);
            setFriendPossible(2);
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0xd;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0xe;
            setMoviePossible(0x27, 0x80000);
            setMoviePossible(0x27, 0x100000);
            setMoviePossible(0x27, 0x200000);
            setMoviePossible(0x27, 0x400000);
        }
        flag = TRUE;
    case 6:
        Index = CSceneInfo::getFloorIndex(6, TRUE);
        if (!flag) {
            CGameWork::setInt(0, 0x33, 5);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x25, 600);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x25, 650);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0xb;
                setMoviePossible(0x27, 0x8000);
                setMoviePossible(0x27, 0x10000);
                break;
            default:
                CGameWork::setInt(0, 0x25, 610);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0xb;
                setMoviePossible(0x27, 0x8000);
                setMoviePossible(0x27, 0x10000);
                break;
            }
            setFriendPossible(1);
            setFriendPossible(2);
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0xb;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0xc;
            setMoviePossible(0x27, 0x8000);
            setMoviePossible(0x27, 0x10000);
            setMoviePossible(0x27, 0x20000);
            setMoviePossible(0x27, 0x40000);
        }
        flag = TRUE;
    case 5:
        Index = CSceneInfo::getFloorIndex(5, TRUE);
        if (!flag) {
            CGameWork::setInt(0, 0x33, 4);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x25, 500);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x25, 550);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x9;
                setMoviePossible(0x27, 0x1000);
                setMoviePossible(0x27, 0x2000);
                break;
            default:
                CGameWork::setInt(0, 0x25, 510);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x9;
                setMoviePossible(0x27, 0x1000);
                setMoviePossible(0x27, 0x2000);
                break;
            }
            setFriendPossible(1);
            setFriendPossible(2);
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x9;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0xa;
            setMoviePossible(0x27, 0x1000);
            setMoviePossible(0x27, 0x2000);
            setMoviePossible(0x27, 0x4000);
        }
        flag = TRUE;
    case 4:
        Index = CSceneInfo::getFloorIndex(4, TRUE);
        if (!flag) {
            CGameWork::setInt(0, 0x33, 3);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x25, 400);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x25, 450);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x7;
                setMoviePossible(0x27, 0x200);
                setMoviePossible(0x27, 0x400);
                break;
            default:
                CGameWork::setInt(0, 0x25, 410);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x7;
                setMoviePossible(0x27, 0x200);
                setMoviePossible(0x27, 0x400);
                break;
            }
            setFriendPossible(1);
            setFriendPossible(2);
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x7;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x8;
            setMoviePossible(0x27, 0x200);
            setMoviePossible(0x27, 0x400);
            setMoviePossible(0x27, 0x800);
        }
        flag = TRUE;
    case 3:
        Index = CSceneInfo::getFloorIndex(3, TRUE);
        if (!flag) {
            CGameWork::setInt(0, 0x33, 2);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x25, 300);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x25, 350);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x5;
                setMoviePossible(0x27, 0x80);
                setMoviePossible(0x27, 0x40);
                break;
            default:
                CGameWork::setInt(0, 0x25, 310);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x5;
                setMoviePossible(0x27, 0x80);
                setMoviePossible(0x27, 0x40);
                break;
            }
            setFriendPossible(1);
            setFriendPossible(2);
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x5;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x6;
            setMoviePossible(0x27, 0x40);
            setMoviePossible(0x27, 0x80);
            setMoviePossible(0x27, 0x100);
        }
        flag = TRUE;
    case 2:
        Index = CSceneInfo::getFloorIndex(2, TRUE);
        if (!flag) {
            CGameWork::setInt(0, 0x33, 1);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x25, 200);
                setTutorialEnd(4);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x25, 250);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x3;
                setMoviePossible(0x27, 0x10);
                setTutorialEnd(8);
                break;
            default:
                CGameWork::setInt(0, 0x25, 210);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x3;
                setMoviePossible(0x27, 0x10);
                setTutorialEnd(8);
                break;
            }
            setFriendPossible(1);
            setFriendPossible(2);
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x3;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x4;
            setMoviePossible(0x27, 0x10);
            setMoviePossible(0x27, 0x20);
            setTutorialEnd(4);
            setTutorialEnd(8);
        }
        flag = TRUE;
    case 1:
        Index = CSceneInfo::getFloorIndex(1, TRUE);
        if (!flag) {
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x25, 0);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x25, 150);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x1;
                setMoviePossible(0x27, 0x2);
                break;
            default:
                CGameWork::setInt(0, 0x25, 0);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x1;
                setMoviePossible(0x27, 0x2);
                if (in_RoomNo >= 3) {
                    setTutorialEnd(0x20);
                    setWorldTitleEnd(1);
                }
                break;
            }
            setFriendPossible(1);
            setFriendPossible(2);
            setFriendPossible(0x100);
            CTechnic::addTechnic(0x55);
            CTechnic::addTechnic(0x56);
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x1;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x2;
            setMoviePossible(0x27, 0x2);
            setMoviePossible(0x27, 0x4);
            setMoviePossible(0x27, 0x8);
            setTutorialEnd(0x10);
            setTutorialEnd(0x20);
        }
        flag = TRUE;
    case 0:
        CTechnic::addTechnic(0x44);
        CTechnic::addTechnic(0x45);
        CTechnic::addTechnic(0x47);
        CTechnic::addTechnic(0x48);
        CTechnic::addTechnic(0x49);
        CTechnic::addTechnic(0x4a);
        CTechnic::addTechnic(0x10);
        CTechnic::addTechnic(0x17);
        CTechnic::addTechnic(0xe);
        CTechnic::addTechnic(0x15);
        break;
    }
    } else {
    switch (in_Floor) {
    case 0:
    case -1:
        Index = CSceneInfo::getFloorIndex(-1, FALSE);
        CGameWork::setInt(0, 0x34, 11);
        switch (in_RoomNo) {
        case 0:
            CGameWork::setInt(0, 0x26, 1200);
            break;
        case 0x63:
            CGameWork::setInt(0, 0x26, 1250);
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x7b;
            setMoviePossible(0x2b, 0x4);
            break;
        default:
            CGameWork::setInt(0, 0x26, 1210);
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x7b;
            setMoviePossible(0x2b, 0x4);
            break;
        }
        flag = TRUE;
    case -2:
        Index = CSceneInfo::getFloorIndex(-2, FALSE);
        if (!flag) {
            CGameWork::setInt(0, 0x34, 10);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x26, 1100);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x26, 1150);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x79;
                setMoviePossible(0x2a, 0x10000000);
                break;
            default:
                CGameWork::setInt(0, 0x26, 1110);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x79;
                setMoviePossible(0x2a, 0x10000000);
                break;
            }
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x79;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x7a;
            setMoviePossible(0x2a, 0x10000000);
            setMoviePossible(0x2b, 0x2);
        }
        flag = TRUE;
    case -3:
        Index = CSceneInfo::getFloorIndex(-3, FALSE);
        if (!flag) {
            CGameWork::setInt(0, 0x34, 9);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x26, 1000);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x26, 1050);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x77;
                setMoviePossible(0x2a, 0x400000);
                break;
            default:
                CGameWork::setInt(0, 0x26, 1010);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x77;
                setMoviePossible(0x2a, 0x400000);
                break;
            }
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x77;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x78;
            setMoviePossible(0x2a, 0x400000);
            setMoviePossible(0x2a, 0x8000000);
        }
        flag = TRUE;
    case -4:
        Index = CSceneInfo::getFloorIndex(-4, FALSE);
        if (!flag) {
            CGameWork::setInt(0, 0x34, 8);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x26, 900);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x26, 950);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x75;
                setMoviePossible(0x2a, 0x10000);
                break;
            default:
                CGameWork::setInt(0, 0x26, 910);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x75;
                setMoviePossible(0x2a, 0x10000);
                break;
            }
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x75;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x76;
            CCard::addCard(0x73, 0);
            setMoviePossible(0x2a, 0x10000);
            setMoviePossible(0x2a, 0x20000);
            setMoviePossible(0x2a, 0x40000);
            setMoviePossible(0x2a, 0x80000);
            setMoviePossible(0x2a, 0x100000);
            setMoviePossible(0x2a, 0x200000);
        }
        setMoviePossible(0x2a, 0x8000);
        flag = TRUE;
    case -5:
        Index = CSceneInfo::getFloorIndex(-5, FALSE);
        if (!flag) {
            CGameWork::setInt(0, 0x34, 7);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x26, 800);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x26, 850);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x73;
                break;
            default:
                CGameWork::setInt(0, 0x26, 800);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x73;
                break;
            }
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x73;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x74;
        }
        setMoviePossible(0x2a, 0x4000);
        flag = TRUE;
    case -6:
        Index = CSceneInfo::getFloorIndex(-6, FALSE);
        if (!flag) {
            CGameWork::setInt(0, 0x34, 6);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x26, 700);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x26, 750);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x71;
                break;
            default:
                CGameWork::setInt(0, 0x26, 700);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x71;
                break;
            }
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x71;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x72;
        }
        setMoviePossible(0x2a, 0x2000);
        flag = TRUE;
    case -7:
        Index = CSceneInfo::getFloorIndex(-7, FALSE);
        if (!flag) {
            CGameWork::setInt(0, 0x34, 5);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x26, 600);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x26, 650);
                setMoviePossible(0x2a, 0x1000);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x6f;
                break;
            default:
                CGameWork::setInt(0, 0x26, 610);
                setMoviePossible(0x2a, 0x1000);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x6f;
                break;
            }
        } else {
            setMoviePossible(0x2a, 0x1000);
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x6f;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x70;
        }
        setMoviePossible(0x2a, 0x800);
        flag = TRUE;
    case -8:
        Index = CSceneInfo::getFloorIndex(-8, FALSE);
        if (!flag) {
            CGameWork::setInt(0, 0x34, 4);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x26, 500);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x26, 550);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x6d;
                break;
            default:
                CGameWork::setInt(0, 0x26, 500);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x6d;
                break;
            }
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x6d;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x6e;
            setMoviePossible(0x2a, 0x200);
            setMoviePossible(0x2a, 0x400);
        }
        setMoviePossible(0x2a, 0x100);
        flag = TRUE;
    case -9:
        Index = CSceneInfo::getFloorIndex(-9, FALSE);
        if (!flag) {
            CGameWork::setInt(0, 0x34, 3);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x26, 400);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x26, 450);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x6b;
                break;
            default:
                CGameWork::setInt(0, 0x26, 400);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x6b;
                break;
            }
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x6b;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x6c;
        }
        flag = TRUE;
    case -10:
        Index = CSceneInfo::getFloorIndex(-10, FALSE);
        if (!flag) {
            CGameWork::setInt(0, 0x34, 2);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x26, 300);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x26, 350);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x69;
                break;
            default:
                CGameWork::setInt(0, 0x26, 300);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x69;
                break;
            }
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x69;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x6a;
            setMoviePossible(0x2a, 0x40);
            setMoviePossible(0x2a, 0x80);
        }
        setMoviePossible(0x2a, 0x20);
        flag = TRUE;
    case -11:
        Index = CSceneInfo::getFloorIndex(-11, FALSE);
        if (!flag) {
            CGameWork::setInt(0, 0x34, 1);
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x26, 200);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x26, 250);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x67;
                setMoviePossible(0x2a, 0x10);
                break;
            default:
                CGameWork::setInt(0, 0x26, 210);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x67;
                setMoviePossible(0x2a, 0x10);
                break;
            }
        } else {
            setMoviePossible(0x2a, 0x10);
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x67;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x68;
        }
        setMoviePossible(0x2a, 0x8);
        setPlayerSkill(8);
        setPlayerSkill(0x10);
        setPlayerSkill(0x20);
        CTechnic::addTechnic(0x6d);
        setFriendPossible(0x80);
        flag = TRUE;
    case -12:
        Index = CSceneInfo::getFloorIndex(-12, FALSE);
        if (!flag) {
            switch (in_RoomNo) {
            case 0:
                CGameWork::setInt(0, 0x26, 0);
                break;
            case 0x63:
                CGameWork::setInt(0, 0x26, 150);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x65;
                setMoviePossible(0x2a, 0x1);
                break;
            default:
                CGameWork::setInt(0, 0x26, 10);
                CGameWork::setInt(0, 0x34, 1);
                _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x65;
                setMoviePossible(0x2a, 0x1);
                break;
            }
        } else {
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][0] = 0x65;
            _ref(CGameWork::m_pMCWork)->_nEntranceSceneTbl[Index][1] = 0x66;
            setMoviePossible(0x2a, 0x2);
            setMoviePossible(0x2a, 0x4);
        }
        CTechnic::addTechnic(0x66);
        CTechnic::addTechnic(0x67);
        CTechnic::addTechnic(0x68);
        CTechnic::addTechnic(0x60);
        CTechnic::addTechnic(0x61);
        CTechnic::addTechnic(0x5f);
        CTechnic::addTechnic(0x69);
        CTechnic::addTechnic(0x6a);
        CTechnic::addTechnic(0x6b);
        CTechnic::addTechnic(0x62);
        CTechnic::addTechnic(0x63);
        CTechnic::addTechnic(0x64);
        CTechnic::addTechnic(0x65);
        CGameWork::setInt(0, 0xc2, 0x7d);
        break;
    }
    }
}

void CAtel::setWorldStepFlag(INT in_WorldID, INT in_EventNo) {
    if (CGame::isSoraGame()) {
        switch (in_WorldID) {
        case 0:
            break;
        case 1:
            switch (in_EventNo) {
            case 4:
            case 3:
            case 2:
                setTutorialEnd(1);
                setFriendPossible(0x100);
                CTechnic::addTechnic(0x55);
                CTechnic::addTechnic(0x56);
            case 1:
                CCard::addCard(0x23, 6);
                CTechnic::addTechnic(0x34);
                CTechnic::addTechnic(0x35);
            case 0:
                setTutorialEnd(0x40);
                break;
            }
            break;
        case 2:
            switch (in_EventNo) {
            case 0:
                break;
            case 4:
                CCard::addCard(0x69, 6);
            case 3:
                setCardPossible(0x400000);
                CCard::addCard(0x2d, 3);
            case 2:
                setWorldTitleEnd(2);
            case 1:
                setFriendPossible(4);
                CTechnic::addTechnic(0x4b);
                CTechnic::addTechnic(0x4c);
                break;
            }
            break;
        case 3:
            switch (in_EventNo) {
            case 0:
            case 1:
                break;
            case 4:
            case 3:
                setCardPossible(0x100000);
                CCard::addCard(0x2b, 0);
            case 2:
                setWorldTitleEnd(4);
                break;
            }
            break;
        case 4:
            switch (in_EventNo) {
            case 0:
            case 1:
                break;
            case 4:
                CCard::addCard(0x65, 0);
            case 3:
            case 2:
                CCard::addCard(0x67, 0);
                setWorldTitleEnd(8);
                break;
            }
            break;
        case 5:
            switch (in_EventNo) {
            case 0:
            case 1:
                break;
            case 4:
                setCardPossible(0x10000);
                CTechnic::addTechnic(0x36);
                CTechnic::addTechnic(0x37);
                CCard::addCard(0x26, 3);
            case 3:
                CCard::addCard(0x64, 0);
            case 2:
                setWorldTitleEnd(0x10);
                break;
            }
            break;
        case 6:
            switch (in_EventNo) {
            case 0:
                break;
            case 4:
                CCard::addCard(0x6a, 0);
            case 3:
            case 2:
                setWorldTitleEnd(0x20);
            case 1:
                setFriendPossible(0x10);
                CTechnic::addTechnic(0x4d);
                CTechnic::addTechnic(0x4e);
                CTechnic::addTechnic(0x2f);
                break;
            }
            break;
        case 7:
            switch (in_EventNo) {
            case 0:
            case 1:
                break;
            case 4:
                CCard::addCard(0x6b, 0);
            case 3:
                setFriendPossible(8);
                CTechnic::addTechnic(0x4f);
                CTechnic::addTechnic(0x50);
            case 2:
                setWorldTitleEnd(0x40);
                break;
            }
            break;
        case 8:
            switch (in_EventNo) {
            case 0:
            case 1:
                break;
            case 4:
                CCard::addCard(0x6c, 0);
                CCard::addCard(0x27, 0);
                CTechnic::addTechnic(0x3f);
                CTechnic::addTechnic(0x40);
            case 3:
            case 2:
                setFriendPossible(0x20);
                CTechnic::addTechnic(0x51);
                CTechnic::addTechnic(0x52);
                setWorldTitleEnd(0x80);
                break;
            }
            break;
        case 9:
            switch (in_EventNo) {
            case 0:
                break;
            case 4:
                CCard::addCard(0x6d, 0);
            case 3:
            case 2:
                setWorldTitleEnd(0x100);
            case 1:
                setFriendPossible(0x40);
                CTechnic::addTechnic(0x53);
                CTechnic::addTechnic(0x54);
                break;
            }
            break;
        case 10:
            break;
        case 11:
            switch (in_EventNo) {
            case 0:
            case 1:
                break;
            case 4:
            case 3:
            case 2:
                setWorldTitleEnd(0x400);
                break;
            }
            break;
        case 12:
            switch (in_EventNo) {
            case 0:
            case 1:
                break;
            case 4:
            case 3:
            case 2:
                CCard::addCard(0x66, 0);
                setWorldTitleEnd(0x800);
                break;
            }
            break;
        case 13:
            switch (in_EventNo) {
            case 0:
                break;
            case 4:
            case 3:
            case 2:
                setWorldTitleEnd(0x1000);
                setMoviePossible(0x28, 0x20000);
                setMoviePossible(0x28, 0x40000);
                setMoviePossible(0xc8, 0x20000);
                setMoviePossible(0xc8, 0x40000);
            case 1:
                setMoviePossible(0x28, 0x20000);
                setMoviePossible(0x28, 0x40000);
                setMoviePossible(0xc8, 0x20000);
                setMoviePossible(0xc8, 0x40000);
                break;
            }
            break;
        }
    } else {
        switch (in_WorldID) {
        case 0:
            break;
        case 1:
            switch (in_EventNo) {
            case 0:
            case 1:
                break;
            case 4:
            case 3:
            case 2:
                setWorldTitleEnd(0x10000);
                break;
            }
            break;
        case 2:
            switch (in_EventNo) {
            case 0:
            case 1:
                break;
            case 4:
            case 3:
            case 2:
                setWorldTitleEnd(0x20000);
                break;
            }
            break;
        case 3:
            switch (in_EventNo) {
            case 0:
            case 1:
                break;
            case 4:
            case 3:
            case 2:
                setWorldTitleEnd(0x40000);
                break;
            }
            break;
        case 4:
            switch (in_EventNo) {
            case 0:
            case 1:
                break;
            case 4:
            case 3:
            case 2:
                setWorldTitleEnd(0x80000);
                break;
            }
            break;
        case 5:
            switch (in_EventNo) {
            case 0:
            case 1:
                break;
            case 4:
            case 3:
            case 2:
                setWorldTitleEnd(0x100000);
                break;
            }
            break;
        case 6:
            switch (in_EventNo) {
            case 0:
            case 1:
                break;
            case 4:
            case 3:
            case 2:
                setWorldTitleEnd(0x200000);
                break;
            }
            break;
        case 7:
            switch (in_EventNo) {
            case 0:
            case 1:
                break;
            case 4:
            case 3:
            case 2:
                setWorldTitleEnd(0x400000);
                break;
            }
            break;
        case 8:
            switch (in_EventNo) {
            case 0:
            case 1:
                break;
            case 4:
            case 3:
            case 2:
                setWorldTitleEnd(0x800000);
                break;
            }
            break;
        case 9:
            switch (in_EventNo) {
            case 0:
            case 1:
                break;
            case 4:
                CCard::addCard(0x6d, 0);
            case 3:
            case 2:
                setWorldTitleEnd(0x1000000);
                break;
            }
            break;
        case 10:
            break;
        case 11:
            switch (in_EventNo) {
            case 0:
                break;
            case 4:
            case 3:
                setMoviePossible(0x2b, 1);
            case 2:
                setMoviePossible(0x2a, 0x40000000);
                setMoviePossible(0x2a, 0x80000000);
                setWorldTitleEnd(0x4000000);
            case 1:
                setMoviePossible(0x2a, 0x20000000);
                break;
            }
            break;
        case 12:
            switch (in_EventNo) {
            case 0:
            case 1:
                break;
            case 4:
            case 3:
            case 2:
                CCard::addCard(0x66, 0);
                setWorldTitleEnd(0x8000000);
                break;
            }
            break;
        case 13:
            switch (in_EventNo) {
            case 0:
                break;
            case 4:
            case 3:
            case 2:
                setWorldTitleEnd(0x10000000);
            case 1:
                setMoviePossible(0x2b, 8);
                break;
            }
            break;
        }
    }
}

void CAtel::setSceneDeSelectInitFlags() {
    CGameWork::setByte(0, 0x61, 1);
    if (CGame::isSoraGame()) return;
    setPlayerSkill(8);
    setPlayerSkill(0x10);
    setPlayerSkill(0x20);
    CTechnic::addTechnic(0x66);
    CTechnic::addTechnic(0x67);
    CTechnic::addTechnic(0x68);
    CTechnic::addTechnic(0x60);
    CTechnic::addTechnic(0x61);
    CTechnic::addTechnic(0x5f);
    CTechnic::addTechnic(0x69);
    CTechnic::addTechnic(0x6a);
    CTechnic::addTechnic(0x6b);
    setFriendPossible(0x80);
    CTechnic::addTechnic(0x62);
    CTechnic::addTechnic(0x63);
    CTechnic::addTechnic(0x64);
    CTechnic::addTechnic(0x65);
}

void CAtel::setCardPossible(INT in_Card) { CGameWork::setInt(0, 0x21, CGameWork::getInt(0, 0x21) | in_Card); }

void CAtel::setTutorialEnd(INT in_End) { CGameWork::setInt(0, 0xc2, CGameWork::getInt(0, 0xc2) | in_End); }

void CAtel::setWorldTitleEnd(INT in_World) { CGameWork::setInt(0, 0xc1, CGameWork::getInt(0, 0xc1) | in_World); }

void CAtel::setPlayerSkill(INT in_Skill) { CGameWork::setInt(0, 0x24, CGameWork::getInt(0, 0x24) | in_Skill); }

void CAtel::setFriendPossible(INT in_Friend) { CGameWork::setInt(0, 0x22, CGameWork::getInt(0, 0x22) | in_Friend); }

void CAtel::setMoviePossible(INT MovieStorageFlag, INT in_MovieNo) {
    CGameWork::setInt(0, MovieStorageFlag, CGameWork::getInt(0, MovieStorageFlag) | in_MovieNo);
}
// SOURCE DATA: .data .rodata
