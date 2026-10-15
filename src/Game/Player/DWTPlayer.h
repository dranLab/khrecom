// PARTIAL: added for Game/Player/PlayerManager. The player debug windows; included by Game/Player/Player,
// PlayerCamera, PlayerGauge, PlayerManager (namespace DWTPlayer) and Form/DebugWindowTBL (namespace DWT::DWTPlayer):
// every includer has its own static copy of the functions and tables. The tables' strings follow the included
// headers' strings in .rodata.
#ifndef GAME_PLAYER_DWTPLAYER_H
#define GAME_PLAYER_DWTPLAYER_H

#include "khtypes.h"
#include "Form/DebugWindow.h"
#include "Game/Debug/DebugWork.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/CardBattle.h"

extern INT SCREEN_HEIGHT;

/* PARTIAL: added for Game/Player/PlayerCamera (its stopgap emits the titles itself, see there) */
#ifndef DWTPLAYER_TITLE
#define DWTPLAYER_TITLE(i, s) s
#endif

namespace DWTPlayer {

static BOOL DebugPlayerCamera(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 1) {
        if (!DebugWork::g_dw.player.iCamMode) {
            pObj->sendMessage(2, 0);
            return FALSE;
        }
        DebugWork::g_dw.player.iCamMode = PlayerCamera::CPlayerCamera::getNowCamMode();
        PCAM_DATA* pData = PlayerCamera::pTestCamData[DebugWork::g_dw.player.iCamMode];
        DebugWork::g_dw.player.vCamEye = pData->vEye;
        DebugWork::g_dw.player.vCamEye.w = pData->fViewAngle;
        DebugWork::g_dw.player.vCamAim = pData->vAim;
        DebugWork::g_dw.player.vCamAim.w = pData->fAdjust;
        DebugWork::g_dw.player.fCamDist = pData->fDistance;
        DebugWork::g_dw.player.fCamSize = pData->fSize;
        DebugWork::g_dw.player.pCamData = pData;
    }
    if (inParam & 0x10) {
        if (inY == 0) {
            PCAM_DATA* pData = PlayerCamera::pTestCamData[DebugWork::g_dw.player.iCamMode];
            DebugWork::g_dw.player.vCamEye = pData->vEye;
            DebugWork::g_dw.player.vCamEye.w = pData->fViewAngle;
            DebugWork::g_dw.player.vCamAim = pData->vAim;
            DebugWork::g_dw.player.vCamAim.w = pData->fAdjust;
            DebugWork::g_dw.player.fCamDist = pData->fDistance;
            DebugWork::g_dw.player.fCamSize = pData->fSize;
            DebugWork::g_dw.player.pCamData = pData;
        }
    }
    return TRUE;
}

static BOOL DebugPlayerAttack(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 1) {
        if (!g_pPlayer) {
            pObj->sendMessage(2, 0);
            return FALSE;
        }
        DebugWork::g_dw.player.iAttackID = g_pPlayer->getAttackID();
        PATK_DATA* pData = &PL::g_AttackDataList[DebugWork::g_dw.player.iAttackID];
        DebugWork::g_dw.player.iAtkPrm[0] = pData->ANIM_ID;
        DebugWork::g_dw.player.iAtkPrm[1] = pData->ANIM_ESQ;
        DebugWork::g_dw.player.iAtkPrm[2] = pData->EFFECT;
        DebugWork::g_dw.player.iAtkPrm[3] = pData->TRG_FRAME;
        DebugWork::g_dw.player.iAtkPrm[4] = pData->COLL_START_FRAME;
        DebugWork::g_dw.player.iAtkPrm[5] = pData->COLL_END_FRAME;
        DebugWork::g_dw.player.iAtkPrm[6] = pData->MOVE_START_FRAME;
        DebugWork::g_dw.player.iAtkPrm[7] = pData->MOVE_END_FRAME;
        DebugWork::g_dw.player.iAtkPrm[8] = pData->RADIUS;
        DebugWork::g_dw.player.iAtkPrm[9] = pData->MAX_SPEED;
        DebugWork::g_dw.player.iAtkPrm[10] = pData->POWER;
        DebugWork::g_dw.player.iAtkPrm[11] = pData->DAMAGE;
        DebugWork::g_dw.player.iAtkPrm[12] = pData->WORK0;
        DebugWork::g_dw.player.iAtkPrm[13] = pData->WORK1;
        DebugWork::g_dw.player.iAtkPrm[14] = pData->WORK2;
        DebugWork::g_dw.player.iAtkFlag[0] = pData->FLAG & 1;
        DebugWork::g_dw.player.iAtkFlag[1] = (pData->FLAG >> 1) & 1;
        DebugWork::g_dw.player.iAtkFlag[2] = (pData->FLAG >> 2) & 1;
        DebugWork::g_dw.player.iAtkFlag[3] = (pData->FLAG >> 3) & 1;
        DebugWork::g_dw.player.iAtkFlag[4] = (pData->FLAG >> 4) & 1;
        DebugWork::g_dw.player.iAtkFlag[5] = (pData->FLAG >> 5) & 1;
        DebugWork::g_dw.player.iAtkFlag[6] = (pData->FLAG >> 6) & 1;
        DebugWork::g_dw.player.iAtkFlag[7] = (pData->FLAG >> 7) & 1;
        DebugWork::g_dw.player.pAtkData = pData;
    }
    if (inParam & 2) {
        DebugWork::g_dw.player.pAtkData = NULL;
    }
    if (inParam & 0x10) {
        if (inY == 0) {
            PATK_DATA* pData = &PL::g_AttackDataList[DebugWork::g_dw.player.iAttackID];
            DebugWork::g_dw.player.iAtkPrm[0] = pData->ANIM_ID;
            DebugWork::g_dw.player.iAtkPrm[1] = pData->ANIM_ESQ;
            DebugWork::g_dw.player.iAtkPrm[2] = pData->EFFECT;
            DebugWork::g_dw.player.iAtkPrm[3] = pData->TRG_FRAME;
            DebugWork::g_dw.player.iAtkPrm[4] = pData->COLL_START_FRAME;
            DebugWork::g_dw.player.iAtkPrm[5] = pData->COLL_END_FRAME;
            DebugWork::g_dw.player.iAtkPrm[6] = pData->MOVE_START_FRAME;
            DebugWork::g_dw.player.iAtkPrm[7] = pData->MOVE_END_FRAME;
            DebugWork::g_dw.player.iAtkPrm[8] = pData->RADIUS;
            DebugWork::g_dw.player.iAtkPrm[9] = pData->MAX_SPEED;
            DebugWork::g_dw.player.iAtkPrm[10] = pData->POWER;
            DebugWork::g_dw.player.iAtkPrm[11] = pData->DAMAGE;
            DebugWork::g_dw.player.iAtkPrm[12] = pData->WORK0;
            DebugWork::g_dw.player.iAtkPrm[13] = pData->WORK1;
            DebugWork::g_dw.player.iAtkPrm[14] = pData->WORK2;
            DebugWork::g_dw.player.iAtkFlag[0] = pData->FLAG & 1;
            DebugWork::g_dw.player.iAtkFlag[1] = (pData->FLAG >> 1) & 1;
            DebugWork::g_dw.player.iAtkFlag[2] = (pData->FLAG >> 2) & 1;
            DebugWork::g_dw.player.iAtkFlag[3] = (pData->FLAG >> 3) & 1;
            DebugWork::g_dw.player.iAtkFlag[4] = (pData->FLAG >> 4) & 1;
            DebugWork::g_dw.player.iAtkFlag[5] = (pData->FLAG >> 5) & 1;
            DebugWork::g_dw.player.iAtkFlag[6] = (pData->FLAG >> 6) & 1;
            DebugWork::g_dw.player.iAtkFlag[7] = (pData->FLAG >> 7) & 1;
            DebugWork::g_dw.player.pAtkData = pData;
        }
    }
    return TRUE;
}

static BOOL DebugWinPlayer(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 1) DebugWork::g_dw.player.iDbgWindow = 1;
    if (inParam & 2) DebugWork::g_dw.player.iDbgWindow = 0;
    DebugWork::g_dw.player.iDbgWindowUpdate = 0;
    if (inParam & 0x10) DebugWork::g_dw.player.iDbgWindowUpdate = 1;
    return TRUE;
}


static DEBUG_WINDOW_LINE debugPlayerCameraLine[14] = {
    {"Mode %s", &DebugWork::g_dw.player.iCamMode, 1, {0}, 0, 1.0f, 5.0f, 1.0f, PlayerCamera::szTestCamMode, 0, DebugPlayerCamera, NULL},
    {"ANGLE:  %5.1f\x81\x8b", &DebugWork::g_dw.player.vCamEye.w, 4, {0}, 1, 0.1f, 179.9f, 0.1f, NULL, 0, DebugPlayerCamera, NULL},
    {"SIZE:   %5.2f", &DebugWork::g_dw.player.fCamSize, 4, {0}, 1, 0.01f, 5.0f, 0.01f, NULL, 0, DebugPlayerCamera, NULL},
    {"ADJUST: %5.2f", &DebugWork::g_dw.player.vCamAim.w, 4, {0}, 1, 0.01f, 1.0f, 0.01f, NULL, 0, DebugPlayerCamera, NULL},
    {"DIST:   %5.1f", &DebugWork::g_dw.player.fCamDist, 4, {0}, 1, 0.0f, 10.0f, 0.1f, NULL, 0, DebugPlayerCamera, NULL},
    {"\xf9S------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"EYE-X:  %5.1f", &DebugWork::g_dw.player.vCamEye.x, 4, {0}, 1, -10.0f, 10.0f, 0.1f, NULL, 0, DebugPlayerCamera, NULL},
    {"EYE-Y:  %5.1f", &DebugWork::g_dw.player.vCamEye.y, 4, {0}, 1, -10.0f, 10.0f, 0.1f, NULL, 0, DebugPlayerCamera, NULL},
    {"EYE-Z:  %5.1f", &DebugWork::g_dw.player.vCamEye.z, 4, {0}, 1, -10.0f, 10.0f, 0.1f, NULL, 0, DebugPlayerCamera, NULL},
    {"\xf9S------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"AIM-X:  %5.1f", &DebugWork::g_dw.player.vCamAim.x, 4, {0}, 1, -10.0f, 10.0f, 0.1f, NULL, 0, DebugPlayerCamera, NULL},
    {"AIM-Y:  %5.1f", &DebugWork::g_dw.player.vCamAim.y, 4, {0}, 1, -10.0f, 10.0f, 0.1f, NULL, 0, DebugPlayerCamera, NULL},
    {"AIM-Z:  %5.1f", &DebugWork::g_dw.player.vCamAim.z, 4, {0}, 1, -10.0f, 10.0f, 0.1f, NULL, 0, DebugPlayerCamera, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugPlayerCamera = {DWTPLAYER_TITLE(0, "\x83v\x83\x8c\x83" "C\x83\x84\x81[\x83J\x83\x81\x83\x89"), 0x66, (SCREEN_HEIGHT >> 1) - 0xEE, 10, 13, 14, 0, 0, debugPlayerCameraLine, NULL, NULL};

static DEBUG_WINDOW_LINE debugPlayerAttackLine[25] = {
    {"ID %s", &DebugWork::g_dw.player.iAttackID, 2, {0}, 0, 0.0f, 187.0f, 1.0f, szAttackData, 0, DebugPlayerAttack, NULL},
    {"ANIM: %s", &DebugWork::g_dw.player.iAtkPrm[0], 2, {0}, 0, 0.0f, 70.0f, 1.0f, szAtkAnimID, 0, DebugPlayerAttack, NULL},
    {"ANIM_ESQ:       % 3d", &DebugWork::g_dw.player.iAtkPrm[1], 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, DebugPlayerAttack, NULL},
    {"EFFECT:         % 3d", &DebugWork::g_dw.player.iAtkPrm[2], 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, DebugPlayerAttack, NULL},
    {"\x83g\x83\x8a\x83K\x81[Frame:  % 3d", &DebugWork::g_dw.player.iAtkPrm[3], 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, DebugPlayerAttack, NULL},
    {"\x8dU\x83" "A\x83^\x83\x8a\x8aJ\x8enFr: % 3d", &DebugWork::g_dw.player.iAtkPrm[4], 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, DebugPlayerAttack, NULL},
    {"\x8dU\x83" "A\x83^\x83\x8a\x8fI\x97\xb9" "Fr: % 3d", &DebugWork::g_dw.player.iAtkPrm[5], 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, DebugPlayerAttack, NULL},
    {"\x88\xda\x93\xae\x8aJ\x8enFrame:  % 3d", &DebugWork::g_dw.player.iAtkPrm[6], 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, DebugPlayerAttack, NULL},
    {"\x88\xda\x93\xae\x8fI\x97\xb9" "Frame:  % 3d", &DebugWork::g_dw.player.iAtkPrm[7], 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, DebugPlayerAttack, NULL},
    {"RADIUS:         % 3d", &DebugWork::g_dw.player.iAtkPrm[8], 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, DebugPlayerAttack, NULL},
    {"SPEED:          % 3d", &DebugWork::g_dw.player.iAtkPrm[9], 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, DebugPlayerAttack, NULL},
    {"POWER:          % 3d", &DebugWork::g_dw.player.iAtkPrm[10], 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, DebugPlayerAttack, NULL},
    {"DAMAGE:         % 3d", &DebugWork::g_dw.player.iAtkPrm[11], 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, DebugPlayerAttack, NULL},
    {"WORK0:          % 3d", &DebugWork::g_dw.player.iAtkPrm[12], 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, DebugPlayerAttack, NULL},
    {"WORK1:          % 3d", &DebugWork::g_dw.player.iAtkPrm[13], 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, DebugPlayerAttack, NULL},
    {"WORK2:          % 3d", &DebugWork::g_dw.player.iAtkPrm[14], 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, DebugPlayerAttack, NULL},
    {"Flag:COMBO      %s", &DebugWork::g_dw.player.iAtkFlag[0], 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, DebugPlayerAttack, NULL},
    {"Flag:MUTEKI     %s", &DebugWork::g_dw.player.iAtkFlag[1], 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, DebugPlayerAttack, NULL},
    {"Flag:SLIDE      %s", &DebugWork::g_dw.player.iAtkFlag[2], 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, DebugPlayerAttack, NULL},
    {"Flag:AERIAL     %s", &DebugWork::g_dw.player.iAtkFlag[3], 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, DebugPlayerAttack, NULL},
    {"Flag:EXMOVE     %s", &DebugWork::g_dw.player.iAtkFlag[4], 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, DebugPlayerAttack, NULL},
    {"Flag:LOAD       %s", &DebugWork::g_dw.player.iAtkFlag[5], 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, DebugPlayerAttack, NULL},
    {"Flag:ANIMEX     %s", &DebugWork::g_dw.player.iAtkFlag[6], 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, DebugPlayerAttack, NULL},
    {"Flag:-          %s", &DebugWork::g_dw.player.iAtkFlag[7], 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, DebugPlayerAttack, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugPlayerAttack = {DWTPLAYER_TITLE(1, "\x8dU\x8c\x82\x83" "f\x81[\x83^"), 0x57, (SCREEN_HEIGHT >> 1) - 0x16C, 12, 24, 13, 0, 0, debugPlayerAttackLine, NULL, NULL};

static DEBUG_WINDOW_LINE debugPlayerJumpLine[14] = {
    {"\x8f" "d\x97\xcd\x8cW\x90\x94       \x81@%2.2f", &PL::g_fWeight, 4, {0}, 1, 0.0f, 10.0f, 0.01f, NULL, 0, NULL, NULL},
    {"\x83W\x83\x83\x83\x93\x83v\x97\xcd     \x81@%2.1f", &PL::g_Param.JUMP_POWER, 4, {0}, 1, 0.1f, 99.9f, 0.1f, NULL, 0, NULL, NULL},
    {"\x8f\xac\x83W\x83\x83\x83\x93\x83v\x8c\xb8\x91\xac   %2.2f", &PL::g_fJumpStopRate, 4, {0}, 1, 0.0f, 1.0f, 0.01f, NULL, 0, NULL, NULL},
    {"\xf9S------------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83n\x83" "C\x83W\x83\x83\x83\x93\x83v     %s", &DebugWork::g_dw.player.iHiJump, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, NULL, NULL},
    {"  \x83W\x83\x83\x83\x93\x83v\x97\xcd     %2.1f", &PL::g_Param.HIJUMP_POWER, 4, {0}, 1, 0.1f, 99.9f, 0.1f, NULL, 0, NULL, NULL},
    {"\xf9S------------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83O\x83\x89\x83" "C\x83h         %s", &DebugWork::g_dw.player.iGlide, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, NULL, NULL},
    {"  \x8f" "d\x97\xcd\x8cW\x90\x94       %1.3f", &PL::g_fGlideWeight, 4, {0}, 1, 0.0f, 1.0f, 0.001f, NULL, 0, NULL, NULL},
    {"  \x90\xf9\x89\xf1 Rate      %1.3f", &PL::g_fGlideTurnRate, 4, {0}, 1, 0.1f, 1.0f, 0.001f, NULL, 0, NULL, NULL},
    {"  \x91\xac\x93x Low       %2.2f", &PL::g_fGlideSpeedLow, 4, {0}, 1, 0.0f, 10.0f, 0.01f, NULL, 0, NULL, NULL},
    {"\x83X\x81[\x83p\x81[\x83O\x83\x89\x83" "C\x83h %s", &DebugWork::g_dw.player.iGlideSP, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, NULL, NULL},
    {"  \x91\xac\x93x High      %2.2f", &PL::g_fGlideSpeedHigh, 4, {0}, 1, 0.0f, 10.0f, 0.01f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugPlayerJump = {DWTPLAYER_TITLE(2, "\x83W\x83\x83\x83\x93\x83v\x8a\xd6\x98" "A"), -0xD0, -(SCREEN_HEIGHT >> 1) + 0x30, 12, 13, 12, 0, 0, debugPlayerJumpLine, NULL, NULL};

static DEBUG_WINDOW_LINE debugPlayerTempLine[6] = {
    {"\x90\x94\x92l   %4d", &DebugWork::g_dw.player.iTemp, 1, {0}, 0, -1.0f, 100.0f, 1.0f, NULL, 0, NULL, NULL},
    {"Vec-X  %3.3f", &DebugWork::g_dw.player.vTemp.x, 4, {0}, 1, -100.0f, 100.0f, 0.01f, NULL, 0, NULL, NULL},
    {"Vec-Y  %3.3f", &DebugWork::g_dw.player.vTemp.y, 4, {0}, 1, -100.0f, 100.0f, 0.01f, NULL, 0, NULL, NULL},
    {"Vec-Z  %3.3f", &DebugWork::g_dw.player.vTemp.z, 4, {0}, 1, -100.0f, 100.0f, 0.01f, NULL, 0, NULL, NULL},
    {"Vec-W  %3.3f", &DebugWork::g_dw.player.vTemp.w, 4, {0}, 1, -100.0f, 100.0f, 0.01f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugPlayerTemp = {DWTPLAYER_TITLE(3, "\x88\xea\x8e\x9e\x93I\x82\xc8\x90\x94\x92l\x90\xdd\x92\xe8"), -0xD0, -(SCREEN_HEIGHT >> 1) + 0x30, 8, 5, 12, 0, 0, debugPlayerTempLine, NULL, NULL};

static DEBUG_WINDOW_LINE debugPlayerCardBattleLine[12] = {
    {"\x83" "f\x83" "b\x83L\x83V\x83\x88\x83" "E     %s", &DebugWork::g_dw.player.iUseDbgDeck, 1, {0}, 0, 0.0f, 9.0f, 1.0f, CardBattle::g_szDbgDeckName, 0, NULL, NULL},
    {"\x83" "f\x83" "b\x83L\x83q\x83\x87\x83" "E\x83W   %s", &DebugWork::g_dw.player.iDeckDbgDisp, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, NULL, NULL},
    {"\x83J\x81[\x83h\x83V\x83\x87\x83" "E\x83q   %s", &DebugWork::g_dw.player.iCardSpend, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOnOff, 0, NULL, NULL},
    {"\x83I\x81[\x83g\x83\x8a\x83\x8d\x81[\x83h   %s", &DebugWork::g_dw.player.iAutoReload, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, NULL, NULL},
    {"\x83X\x83g\x83" "b\x83N LOCK    %s", &DebugWork::g_dw.player.iSkillLock, 2, {0}, 0, 0.0f, 109.0f, 1.0f, szPlayerSkill, 0, NULL, NULL},
    {"  \x83\x8c\x83\x93\x83_\x83L\x83\x93\x83V   %s", &DebugWork::g_dw.player.iRenda, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOnOff, 0, NULL, NULL},
    {"\x83J\x81[\x83h TEC LOCK  %s", &DebugWork::g_dw.player.iCardLock, 2, {0}, 0, 0.0f, 66.0f, 1.0f, szPlayerCard, 0, NULL, NULL},
    {"\x83J\x81[\x83h NUM LOCK  %d", &DebugWork::g_dw.player.iCardNumLock, 1, {0}, 0, -1.0f, 10.0f, 1.0f, NULL, 0, NULL, NULL},
    {"\x83G\x83l\x83~\x81[\x83J\x81[\x83h   %s", &DebugWork::g_dw.player.iEnemyCardLock, 1, {0}, 0, 0.0f, 59.0f, 1.0f, CardBattle::g_szEnemyCardEffName, 0, NULL, NULL},
    {"\x83t\x83\x8c\x83\x93\x83h100%%    %s", &DebugWork::g_dw.player.iFriend, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, NULL, NULL},
    {"\x83v\x83\x8b\x81[\x83g\x8fo\x8c\xbb\x8f\xf0\x8c\x8f %s", &DebugWork::g_dw.player.iPluto, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOnOff, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugPlayerCardBattle = {DWTPLAYER_TITLE(4, "\x83J\x81[\x83h\x83o\x83g\x83\x8b\x8a\xd6\x98" "A"), -0xD0, -(SCREEN_HEIGHT >> 1) + 0x30, 18, 11, 12, 0, 0, debugPlayerCardBattleLine, NULL, NULL};

static DEBUG_WINDOW_LINE debugWinPlayerLine[20] = {
    {"\x83v\x83\x8c\x83" "C\x83\x84INFO     %s", &DebugWork::g_dw.player.iPlayerInfo, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, NULL, NULL},
    {"\x83v\x83\x8c\x83" "C\x83\x84\x83\x80\x83" "e\x83L   %s", &DebugWork::g_dw.player.iMuteki, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, NULL, NULL},
    {"HP\x83_\x83\x81\x81[\x83W0      %s", &DebugWork::g_dw.player.iDamage0, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, NULL, NULL},
    {"\x83v\x83\x8c\x83" "C\x83\x84HP       %d", &DebugWork::g_dw.player.iHP, 2, {0}, 0, 0.0f, 499.0f, 1.0f, NULL, 0, NULL, NULL},
    {"\x83v\x83\x8c\x83" "C\x83\x84HP->1    %s", &DebugWork::g_dw.player.iHP1, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, NULL, NULL},
    {"DP\x83}\x83\x93\x83^\x83\x93       %s", &DebugWork::g_dw.player.iDP, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, NULL, NULL},
    {"DP->1            %s", &DebugWork::g_dw.player.iDP1, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, NULL, NULL},
    {"\x81\x9b\x83{\x83^\x83\x93\x83\x8c\x83\x93\x83V\x83\x83 %s", &DebugWork::g_dw.player.iTrgAuto, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, NULL, NULL},
    {"\xf9S------------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83J\x83\x81\x83\x89INFO       %s", &DebugWork::g_dw.player.iCameraInfo, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, NULL, NULL},
    {"\x83J\x83\x81\x83\x89\x83\x8a\x83N\x83V\x83t\x83g %s", &DebugWork::g_dw.player.iCamRikuShiftY, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, NULL, NULL},
    {"\x83J\x83\x81\x83\x89Setting ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugPlayerCamera},
    {"\xf9S------------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"LOAD\x83o\x83" "b\x83t\x83@INFO %s", &DebugWork::g_dw.player.iBlockInfo, 1, {0}, 0, 0.0f, 1.0f, 1.0f, DWT::szOffOn, 0, NULL, NULL},
    {"\xf9S------------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83" "C\x83`\x83W\x83" "e\x83L\x82\xc8\x83Z\x83" "b\x83" "e\x83" "C ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugPlayerTemp},
    {"Jump Setting ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugPlayerJump},
    {"Attack Setting ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugPlayerAttack},
    {"Card Battle Debug ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugPlayerCardBattle},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugWinPlayer = {DWTPLAYER_TITLE(5, "\x83v\x83\x8c\x83" "C\x83\x84\x81[DEBUG"), -0xE0, -(SCREEN_HEIGHT >> 1) + 0x20, 12, 19, 12, 0, 0, debugWinPlayerLine, NULL, NULL};

static DEBUG_WINDOW_LINE debugPlayerInfoLine[13] = {
    {"PosX:   % 9.4f", &DebugWork::g_dw.player.vPos.x, 4, {0}, 3, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"PosY:   % 9.4f", &DebugWork::g_dw.player.vPos.y, 4, {0}, 3, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"PosZ:   % 9.4f", &DebugWork::g_dw.player.vPos.z, 4, {0}, 3, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"DirY:   % 9.4f", &DebugWork::g_dw.player.vPos.w, 4, {0}, 3, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\xf9U---\x83X\x83" "e\x81[\x83g-------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"PState: % 9s", &DebugWork::g_dw.player.iPlayerState, 1, {0}, 2, 0.0f, 0.0f, 0.0f, szPlayerState, 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szState[0], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szState[1], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szState[2], 0, NULL, NULL},
    {"\xf9U--\x83\x82\x81[\x83V\x83\x87\x83\x93------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szAnim[0], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szAnim[1], 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugPlayerInfo = {DWTPLAYER_TITLE(6, "\x83v\x83\x8c\x83" "C\x83\x84\x81[\x8f\xee\x95\xf1"), -0xF8, -(SCREEN_HEIGHT >> 1) + 0x60, 9, 12, 12, 0, 6, debugPlayerInfoLine, NULL, NULL};

static DEBUG_WINDOW_LINE debugPlayerCameraInfoLine[6] = {
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szCamera[0], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szCamera[1], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szCamera[2], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szCamera[3], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szCamera[4], 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugPlayerCameraInfo = {DWTPLAYER_TITLE(7, "\x83v\x83\x8c\x83" "C\x83\x84\x81[\x83J\x83\x81\x83\x89\x8f\xee\x95\xf1"), 0x2E, -(SCREEN_HEIGHT >> 1) + 0x3C, 20, 5, 10, 0, 6, debugPlayerCameraInfoLine, NULL, NULL};

static DEBUG_WINDOW_LINE debugPlayerBlockInfoLine[18] = {
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szBlock[0], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szBlock[1], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szBlock[2], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szBlock[3], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szBlock[4], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szBlock[5], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szBlock[6], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szBlock[7], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szBlock[8], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szBlock[9], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szBlock[10], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szBlock[11], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szBlock[12], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szBlock[13], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szBlock[14], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szBlock[15], 0, NULL, NULL},
    {"%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &DebugWork::g_dw.player.m_szBlock[16], 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugPlayerBlockInfo = {DWTPLAYER_TITLE(8, "PLM::RB_DATA\x8f\xee\x95\xf1"), -0x8, -(SCREEN_HEIGHT >> 1) + 0x3C, 21, 17, 12, 0, 6, debugPlayerBlockInfoLine, NULL, NULL};

} // namespace DWTPlayer

#endif /* GAME_PLAYER_DWTPLAYER_H */
