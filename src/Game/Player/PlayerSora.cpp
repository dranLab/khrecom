#include "khtypes.h"
#include "Game/CardBattle.h"
#include "Game/Player/PlayerSora.h"
#include "Game/Player/PlayerManager.h"
#include "include_asm.h"

using namespace PL;

CPlayerSora::CPlayerSora(CTask* inParent) : CPlayer(inParent, PLM::g_uiPlayerID, 0) {
    g_Param = g_SoraParam;
    m_hMagicCore = 0;
    initMagicBullet();
    m_nHitCount = 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", _$_Q22PL11CPlayerSora);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", startStockSkillAction__Q22PL11CPlayerSora);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", initStockSkill__Q22PL11CPlayerSorai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exeStockSkill__Q22PL11CPlayerSora);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", initMagic__Q22PL11CPlayerSorai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exeMagic__Q22PL11CPlayerSora);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", setMagic__Q22PL11CPlayerSoraiii);
void CPlayerSora::initMagicBullet() {
    m_nMagicBullet = 0;
    m_nMagicBulletMax = 0;
    for (INT i = 15; i >= 0; i--) {
        m_hMagicBullet[i] = 0;
    }
}
INT CPlayerSora::checkMagicBullet() {
    INT n = 0;
    for (INT i = 0; i < 16; i++) {
        if (m_hMagicBullet[i] > 0) n++;
    }
    return n;
}
void CPlayerSora::setMagicBullet(HANDLE inBullet) {
    for (INT i = 0; i < 16; i++) {
        if (!m_hMagicBullet[i]) {
            m_hMagicBullet[i] = inBullet;
            m_nMagicBullet++;
            break;
        }
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", initFriend__Q22PL11CPlayerSorai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exeFriend__Q22PL11CPlayerSora);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", watchTarget__Q22PL11CPlayerSorai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", setAttack_inFriend__Q22PL11CPlayerSoraiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exeAttack_inFriend__Q22PL11CPlayerSora);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", endAttack_inFriend__Q22PL11CPlayerSora);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", initSummon__Q22PL11CPlayerSorai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exeSummon__Q22PL11CPlayerSora);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", setStateSE__Q22PL11CPlayerSoraii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", getStateSESeq__Q22PL11CPlayerSorai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", initGlide__Q22PL11CPlayerSorai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exeGlide__Q22PL11CPlayerSora);
void CPlayerSora::exeAvoid() {
    exeDogeRoll();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exeDogeRoll__Q22PL11CPlayerSora);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", initPronounce__Q22PL11CPlayerSorai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exePronounce__Q22PL11CPlayerSora);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", calcAppearPosFS_toAttackID__Q22PL11CPlayerSora);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", initDuel__Q22PL11CPlayerSorai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exeDuel__Q22PL11CPlayerSora);
