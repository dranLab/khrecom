// PARTIAL: added for Game/Gimmick/CreateGimmick
#ifndef GAME_ENEMY_HUMANBOSS_H
#define GAME_ENEMY_HUMANBOSS_H

#include "khtypes.h"
#include "Game/Enemy/BattleEnemy.h"

/* Sequence/parameter names: file-local statics of the header (every includer has its own copy in .data). */
namespace HumanBoss {
static CHAR* NormalEyeSeqName = "EYE_BLINK";
static CHAR* NormalLipSeqName = "LIP_DEFAULT";
static CHAR* DamageEyeSeqName = "EYE_DAMAGE";
static CHAR* DamageLipSeqName = "LIP_DAMAGE";
static CHAR* FaintEyeSeqName = "EYE_DAMAGE";
static CHAR* FaintLipSeqName = "LIP_DAMAGE";
static CHAR* DeadEyeSeqName = "EYE_DAMAGE";
static CHAR* DeadLipSeqName = "LIP_DAMAGE";
static CHAR* ItemMotionParamName = "ItemMot";
static CHAR* VoiceIDParamName = "VoiceID";
static CHAR* MyHpRemainParamName = "MyHpRemain";
static CHAR* MyHpVoiceParamName = "MyHpVoice";
static CHAR* PlHpRemainParamName = "PlHpRemain";
static CHAR* PlHpVoiceParamName = "PlHpVoice";
static CHAR* ReloadVoiceParamName = "ReloadVoice";
static CHAR* ECardVoiceParamName = "ECardVoice";
static CHAR* UseItemCardParamName = "UseItemCard";
}

#endif /* GAME_ENEMY_HUMANBOSS_H */
