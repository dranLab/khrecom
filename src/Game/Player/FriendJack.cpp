
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Player/FriendJack.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in FriendAladdin). */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}
inline void _emptyStringPlaceholder2() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}

#include "Game/Player/Player.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/Movement.h"
#include "System/Rnd.h"

namespace Friend {

static LPCSTR g_szBoneLxRot = "lx_rot";

unsigned char g_JackMagicID[3][4] = {
    { 34, 35, 36, 38 },
    { 41, 43, 46, 56 },
    { 42, 44, 47, 57 },
};

CFriendJack::CFriendJack(CTask* inParent) : CFriend(inParent, 5, 0) {
    LPCSTR& name = g_szBoneLxRot;
    m_iAtkBone = getBoneIndex(name, 0);
    m_vAtkOffset.set(0.0f, 0.0f, 0.0f);
}

CFriendJack::~CFriendJack() {
}

INT CFriendJack::setSkill(INT inID) {
    if (m_iState != 1) return 0;
    switch (inID) {
    case 89:
        m_uiSkillID = 13;
        m_uiLevel = 0;
        changeState(2);
        break;
    case 107:
        m_uiSkillID = 14;
        m_uiLevel = 1;
        changeState(2);
        break;
    case 108:
        m_uiSkillID = 15;
        m_uiLevel = 2;
        changeState(2);
        break;
    default:
        setExit(0);
        break;
    }
    return 1;
}

void CFriendJack::beginStateAction1() {
    m_iState = 2;
    m_uiKind = RND::genrand_int31() % 4;
    m_uiAttackID = g_JackMagicID[m_uiLevel][m_uiKind];
    BC_PARAM*& bc = CardBattle::g_pBattleCardList;
    m_AtkAttr = bc[g_JackMagicID[0][m_uiKind] - 8].ATTR;
    m_iSubState = 1;
    changeMotion(5, 0, 1, -1, m_uiKind + 1, 0, 0);
    m_pMove->enableGravity(1);
    m_pPlayer->getPlayerCamera()->changeNowPlayer(m_pPlayer->getHandle(), getHandle());
    m_pPlayer->getPlayerCamera()->changeCamMode(4, 0);
}

void CFriendJack::stateAction1() {
    if (exeJackMagic()) setExit(0);
}

INT CFriendJack::exeJackMagic() {
    switch (m_iSubState) {
    case 1:
        homingPlayerTargetY(0, 0.3f);
        if (isMotionEnd()) {
            m_iSubState = m_uiKind + 2;
            if (m_uiKind) {
                changeMotion(6, 0, 1, -1, m_uiKind + 3, 0, 0);
            } else {
                changeMotion(6, 0, 1, -1, m_uiLevel + 1, 0, 0);
            }
        }
        break;
    case 2:
        m_iSubState = 6;
        setMagic(34);
        break;
    case 3:
        m_iSubState = 6;
        setMagic(35);
        break;
    case 4:
        m_iSubState = 6;
        setMagic(36);
        break;
    case 5:
        m_iSubState = 6;
        setMagic(38);
        break;
    case 6:
        if (isMotionEnd()) {
            if (m_iAttackNo < m_uiLevel) {
                m_iAttackNo++;
                changeState(2);
            } else {
                changeMotion(1, 0, 1, -1, 0, 0, 0);
                m_iSubState = 7;
                setTimer(3000);
            }
        }
        break;
    case 7:
        if (getTimer() <= 0) return 1;
        break;
    }
    return 0;
}

} // namespace Friend

// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .data .rodata
