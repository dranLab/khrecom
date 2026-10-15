#ifndef FORM_MENU_JMEMOFLAG_H
#define FORM_MENU_JMEMOFLAG_H

#include "khtypes.h"
#include "Game/GameWork.h"

class CTask;
class CJiminyMemo;

class CJiminyMemoFlag {
private:
    static CJiminyMemo* s_pTask;
    static HANDLE s_hTask;
    static BOOL s_bLoad;

public:
    static INT createTask(CTask* inParent, INT inMode);
    static INT checkTaskOut();
    static void initTask();
    static void outTask();

protected:
    static STORAGE::JIMINYMEMO_FLAGWORK& getMCWork() { return CGameWork::progMCW()._JiminyMemoFlagWork; }
    static FLOAT getOpenFlagPercentage(INT inStartFlag, INT inEndFlag, BOOL inSpecialCount);
    static FLOAT getOpenStoryPercentage(INT inMode);
    static FLOAT getOpenCharacterPercentage(INT inMode);
    static FLOAT getCardCollectionPercentage(INT inMode, BOOL inSpecialCount);

public:
    static INT getContentOpenPercentage(INT inMode);
    static CHAR* getCollecterRankName(INT inMode);
    static CHAR* getCollecterRankName(INT inMode, FLOAT inPercentage);
    static INT getDownHeartlessNum(INT inMode);
    static void checkEvent(INT inEvent, INT inParam0, INT inParam1);
    static void checkMiniGameHiScore();
    static void cleanTouchFlag();

private:
    static void checkSoraEvent_Default();
    static void checkSoraEvent_WorldClear(INT inWorldId);
    static void checkSoraEvent_FloorClear(INT inFloorId);
    static void checkSoraEvent_Monstro();
    static void checkSoraEvent_Minigame(INT inKind);
    static void checkSoraEvent_MoogleShop();
    static void checkSoraEvent_DownEnemy(INT inKind, INT inNum);
    static void checkSoraEvent_GetCard(INT inKind);
    static void checkSoraEvent_13BossBattle(INT inKind);
    static void checkSoraEvent_100AcreTalk(INT inKind);
    static void checkSoraEvent_100AcreExit();
    static void checkRikuEvent_Default();
    static void checkRikuEvent_WorldClear(INT inWorldId);
    static void checkRikuEvent_FloorClear(INT inFloorId);
    static void checkRikuEvent_DownEnemy(INT inKind, INT inNum);
    static void checkRikuEvent_GetCard(INT inKind);
    static void setContentFlag(INT inIndex, INT inOrFlag, INT inAndFlag);
    static void setContentNewFlag(INT inIndex);
    static void setDownEnemyNum(INT inIndex, USHORT inValue);
    static void setContentScore(INT inIndex, UINT inValue);
    static void setContentTalkFlag(INT inIndex);
    static INT isContentTalk(INT inIndex);

protected:
    static INT isContentFlag(INT inIndex, INT inFlag);
    static INT isEqualContentFlag(INT inIndex, INT inFlag);
    static INT isContentOpen(INT inIndex);
    static INT isContentNew(INT inIndex);
    static void setContentTouchFlag(INT inIndex);
    static UINT getContentScore(INT inIndex);
    static USHORT getDownEnemyNum(INT inIndex);

public:
    static void debugEvent(BOOL inIsSet, INT inPlayer, INT inEvent, INT inParam0, INT inParam1);

private:
    static void setDebugNewFlag(BOOL inIsSet, INT inIndex);
    static void setDebugTalkFlag(BOOL inIsSet, INT inIndex);
    static void debugSoraEvent_Default(BOOL inIsSet);
    static void debugSoraEvent_WorldClear(BOOL inIsSet, INT inWorldId);
    static void debugSoraEvent_FloorClear(BOOL inIsSet, INT inFloor);
    static void debugSoraEvent_Monstro(BOOL inIsSet);
    static void debugSoraEvent_Minigame(BOOL inIsSet, INT inKind);
    static void debugSoraEvent_MoogleShop(BOOL inIsSet);
    static void debugSoraEvent_DownEnemy(BOOL inIsSet, INT inKind, INT inNum);
    static void debugSoraEvent_GetCard(BOOL inIsSet, INT inKind);
    static void debugSoraEvent_13BossBattle(BOOL inIsSet, INT inKind);
    static void debugSoraEvent_100AcreExit(BOOL inIsSet);
    static void debugSoraEvent_100AcreTalk(BOOL inIsSet, INT inKind);
    static void debugRikuEvent_Default(BOOL inIsSet);
    static void debugRikuEvent_WorldClear(BOOL inIsSet, INT inWorldId);
    static void debugRikuEvent_FloorClear(BOOL inIsSet, INT inFloor);
    static void debugRikuEvent_DownEnemy(BOOL inIsSet, INT inKind, INT inNum);
    static void debugRikuEvent_GetCard(BOOL inIsSet, INT inKind);
    static void debugSoraEvent_All(INT inFlag);
    static void debugRikuEvent_All(INT inFlag);
};

#endif // FORM_MENU_JMEMOFLAG_H
