#define CTDMAN_H_PATH "System/Ctd/CtdMan.h" /* MATCH: this unit names CtdMan.h so (header default "System/CTD/CtdMan.h") */
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Task.h"
#include "System/TaskMan.h"
#include "System/Ctd/CtdMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Form/Menu/JiminyMemo.h"
#include "Form/Menu/JMemoFlag.h"
#include "System/HeapMan.h"
#include "System/MainApp.h"
#include "Game/GameWork.h"
#include "Game/Game.h"

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new(size_t, void* p) throw() { return p; }

#include "System/RsrcDB.h"





#include "System/Rcf.h"






namespace PLM {
BYTE* getBattleRsrcFirst();
}

CJiminyMemo* CJiminyMemoFlag::s_pTask = NULL;
HANDLE CJiminyMemoFlag::s_hTask = 0;
BOOL CJiminyMemoFlag::s_bLoad = 0;

namespace JiminyMemoFlag {
static const int RsrcId[2] = { 110, 112 };
static const float SoraRankingTbl[8] = { 0.25f, 0.45f, 0.55f, 0.68f, 0.8f, 0.85f, 0.92f, 1.f };
static const float RikuRankingTbl[5] = { 0.33f, 0.58f, 0.75f, 0.9f, 1.f };
}

template <class T> static inline T& _ref(T& v) { return v; }

INT CJiminyMemoFlag::createTask(CTask* inParent, INT inMode) {
    if (s_pTask != NULL || s_hTask != 0) return -1;
#line 61
    CJiminyMemo* pMemo = new (com_malloc_d(sizeof(CJiminyMemo), __FILE__ "(61)", RET_ADDR())) CJiminyMemo(inParent, inMode);
    if (pMemo != NULL) {
        _ref(s_pTask) = pMemo;
        s_hTask = pMemo->getHandle();
        INT resourceId = JiminyMemoFlag::RsrcId[inMode != 0];
        if (!CRsrcDB::hasRsrcGroup(2, resourceId)) {
            BYTE* loadAddress = PLM::getBattleRsrcFirst();
            if (loadAddress != NULL) {
                CRcf::reqLoadRCFFront(2, resourceId, 0x140, loadAddress + 0x98000, 0);
            } else {
                CRcf::reqLoadRCFFront(2, resourceId, 0, NULL, 0);
            }
            s_bLoad = TRUE;
        } else {
            s_bLoad = FALSE;
        }
        return 1;
    }
    return 0;
}

INT CJiminyMemoFlag::checkTaskOut() {
    if (s_pTask == NULL || s_hTask == 0) return -1;
    CTask* pTask = CTaskMan::getTask(s_hTask);
    if (pTask == NULL || pTask->isOut()) {
        if (s_bLoad == TRUE) {
            for (INT i = 0; i < 2; i++) {
                INT resourceId = JiminyMemoFlag::RsrcId[i];
                if (CRsrcDB::hasRsrcGroup(2, resourceId)) {
                    CRsrcDB::removeGroup(2, resourceId);
                } else if (!CRcf::isCompleteLoadRCF(2, resourceId)) {
                    CRcf::cancelLoadRCF(2, resourceId);
                }
            }
        }
        initTask();
        return 1;
    }
    return 0;
}

void CJiminyMemoFlag::initTask() {
    s_pTask = NULL;
    s_hTask = 0;
    s_bLoad = 0;
}

void CJiminyMemoFlag::outTask() {
    if (CRsrcDB::hasRsrcGroup(2, JiminyMemoFlag::RsrcId[0]) || CRsrcDB::hasRsrcGroup(2, JiminyMemoFlag::RsrcId[1])) {
        CTask* pTask = CTaskMan::getTask(s_hTask);
        if (pTask != NULL && !pTask->isOut()) {
            pTask->out();
            delete pTask;
            initTask();
        }
    }
}

INT CJiminyMemoFlag::getContentOpenPercentage(INT inMode) {
    int flagId[3][2];
    if (inMode == 0) {
        flagId[0][0] = 0;
        flagId[0][1] = 16;
        flagId[1][0] = 23;
        flagId[1][1] = 126;
        flagId[2][0] = 183;
        flagId[2][1] = 334;
    } else {
        flagId[0][0] = 17;
        flagId[0][1] = 22;
        flagId[1][0] = 127;
        flagId[1][1] = 182;
        flagId[2][0] = 335;
        flagId[2][1] = 393;
    }
    INT allFlagNum = 0;
    INT openFlagNum = 0;
    for (INT i = 0; i < 3; i++) {
        for (INT id = flagId[i][0]; id <= flagId[i][1]; id++) {
            if (id == 333 || id == 334) continue;
            allFlagNum++;
            if (isContentOpen(id)) openFlagNum++;
        }
    }
    return openFlagNum * 100 / allFlagNum;
}

FLOAT CJiminyMemoFlag::getOpenFlagPercentage(INT inStartFlag, INT inEndFlag, BOOL inSpecialCount) {
    INT openFlagNum = 0;
    INT allFlagNum = inEndFlag - inStartFlag + 1;
    if (inSpecialCount == TRUE) {
        for (INT i = inStartFlag; i <= inEndFlag; i++) {
            if (isContentOpen(i)) openFlagNum++;
        }
    } else {
        for (INT i = inStartFlag; i <= inEndFlag; i++) {
            if (i != 333 && i != 334) {
                if (isContentOpen(i)) openFlagNum++;
            } else {
                allFlagNum--;
            }
        }
    }
    return (FLOAT)openFlagNum / (FLOAT)allFlagNum;
}

FLOAT CJiminyMemoFlag::getOpenStoryPercentage(INT inMode) {
    INT startStoryId, endStoryId;
    if (inMode == 0) {
        startStoryId = 0;
        endStoryId = 16;
    } else {
        startStoryId = 17;
        endStoryId = 22;
    }
    return getOpenFlagPercentage(startStoryId, endStoryId, FALSE);
}

FLOAT CJiminyMemoFlag::getOpenCharacterPercentage(INT inMode) {
    INT startCharaId, endCharaId;
    if (inMode == 0) {
        startCharaId = 23;
        endCharaId = 126;
    } else {
        startCharaId = 127;
        endCharaId = 182;
    }
    return getOpenFlagPercentage(startCharaId, endCharaId, FALSE);
}

FLOAT CJiminyMemoFlag::getCardCollectionPercentage(INT inMode, BOOL inSpecialCount) {
    INT startCardId, endCardId;
    if (inMode == 0) {
        startCardId = 183;
        endCardId = 334;
    } else {
        startCardId = 335;
        endCardId = 393;
    }
    return getOpenFlagPercentage(startCardId, endCardId, inSpecialCount);
}

CHAR* CJiminyMemoFlag::getCollecterRankName(INT inMode) {
    return getCollecterRankName(inMode, getCardCollectionPercentage(inMode, FALSE));
}

CHAR* CJiminyMemoFlag::getCollecterRankName(INT inMode, FLOAT inPercentage) {
    const FLOAT* pRankTbl;
    INT rankNum;
    INT ctNo;
    if (inMode == 0) {
        pRankTbl = JiminyMemoFlag::SoraRankingTbl;
        ctNo = 0x780000;
        rankNum = 8;
    } else {
        ctNo = 0x780009;
        pRankTbl = JiminyMemoFlag::RikuRankingTbl;
        rankNum = 5;
    }
    INT rank;
    for (rank = 0; rank < rankNum; rank++) {
        if (inPercentage < pRankTbl[rank]) break;
    }
    UINT ctdID = ctNo + rank;
    return CCtdMan::getText(ctdID);
}

INT CJiminyMemoFlag::getDownHeartlessNum(INT inMode) {
    INT downEnemyNum;
    INT startEnemyId, endEnemyId;
    if (inMode == 0) {
        startEnemyId = 0;
        endEnemyId = 34;
    } else {
        startEnemyId = 35;
        endEnemyId = 68;
    }
    downEnemyNum = 0;
    for (INT i = startEnemyId; i <= endEnemyId; i++) {
        downEnemyNum += getDownEnemyNum(i);
    }
    if (downEnemyNum > 999999) downEnemyNum = 999999;
    return downEnemyNum;
}

void CJiminyMemoFlag::checkMiniGameHiScore() {
    INT bg = CGameWork::getInt(0, 46);
    INT wd = CGameWork::getInt(0, 47);
    INT eb = CGameWork::getInt(0, 48);
    INT tj = CGameWork::getInt(0, 49);
    INT bp = CGameWork::getInt(0, 50);
    if (getContentScore(0) < (UINT)bg) setContentScore(0, bg);
    if (getContentScore(1) < (UINT)wd) setContentScore(1, wd);
    if (getContentScore(2) < (UINT)eb) setContentScore(2, eb);
    if (getContentScore(3) < (UINT)tj) setContentScore(3, tj);
    if (getContentScore(4) < (UINT)bp) setContentScore(4, bp);
}

void CJiminyMemoFlag::cleanTouchFlag() {
    for (INT i = 0; i < 400; i++) {
        if (isContentFlag(i, 2)) setContentFlag(i, 4, 3);
    }
}

void CJiminyMemoFlag::checkEvent(INT inEvent, INT inParam0, INT inParam1) {
    if (CGame::isSoraGame()) {
        switch (inEvent) {
        case 0: checkSoraEvent_Default(); break;
        case 1: checkSoraEvent_WorldClear(inParam0); break;
        case 2: checkSoraEvent_FloorClear(inParam0); break;
        case 3: checkSoraEvent_DownEnemy(inParam0, inParam1); break;
        case 4: checkSoraEvent_GetCard(inParam0); break;
        case 5: checkSoraEvent_MoogleShop(); break;
        case 6: checkSoraEvent_Monstro(); break;
        case 7: checkSoraEvent_13BossBattle(inParam0); break;
        case 8: checkSoraEvent_Minigame(inParam0); break;
        case 9: checkSoraEvent_100AcreExit(); break;
        case 10: checkSoraEvent_100AcreTalk(inParam0); break;
        }
    } else {
        switch (inEvent) {
        case 0: checkRikuEvent_Default(); break;
        case 1: checkRikuEvent_WorldClear(inParam0); break;
        case 2: checkRikuEvent_FloorClear(inParam0); break;
        case 3: checkRikuEvent_DownEnemy(inParam0, inParam1); break;
        case 4: checkRikuEvent_GetCard(inParam0); break;
        }
    }
}
void CJiminyMemoFlag::checkSoraEvent_Default() {
    setContentNewFlag(23);
    setContentNewFlag(24);
    setContentNewFlag(25);
    setContentNewFlag(26);
    setContentNewFlag(27);
    setContentNewFlag(28);
    setContentNewFlag(29);
    setContentNewFlag(183);
}

void CJiminyMemoFlag::checkSoraEvent_WorldClear(INT inWorldId) {
    switch (inWorldId) {
    case 1:
        setContentNewFlag(4);
        setContentNewFlag(41);
        setContentNewFlag(42);
        setContentNewFlag(43);
        setContentNewFlag(44);
        break;
    case 2:
        setContentNewFlag(7);
        setContentNewFlag(55);
        setContentNewFlag(56);
        setContentNewFlag(57);
        setContentNewFlag(58);
        setContentNewFlag(59);
        setContentNewFlag(60);
        break;
    case 3:
        setContentNewFlag(6);
        setContentNewFlag(51);
        setContentNewFlag(52);
        setContentNewFlag(53);
        setContentNewFlag(54);
        break;
    case 4:
        setContentNewFlag(5);
        setContentNewFlag(45);
        setContentNewFlag(46);
        setContentNewFlag(47);
        setContentNewFlag(48);
        setContentNewFlag(49);
        setContentNewFlag(50);
        break;
    case 5:
        setContentNewFlag(9);
        setContentNewFlag(68);
        setContentNewFlag(69);
        break;
    case 6:
        setContentNewFlag(8);
        setContentNewFlag(61);
        setContentNewFlag(62);
        setContentNewFlag(63);
        setContentNewFlag(64);
        setContentNewFlag(65);
        setContentNewFlag(66);
        setContentNewFlag(67);
        break;
    case 7:
        setContentNewFlag(10);
        setContentNewFlag(70);
        setContentNewFlag(71);
        setContentNewFlag(72);
        setContentNewFlag(73);
        break;
    case 8:
        setContentNewFlag(11);
        setContentNewFlag(74);
        setContentNewFlag(75);
        setContentNewFlag(76);
        setContentNewFlag(77);
        break;
    case 9:
        setContentNewFlag(12);
        setContentNewFlag(78);
        setContentNewFlag(79);
        setContentNewFlag(80);
        setContentNewFlag(81);
        break;
    case 10:
        setContentNewFlag(13);
        setContentNewFlag(82);
        checkSoraEvent_100AcreExit();
        break;
    case 11:
        setContentNewFlag(14);
        break;
    case 12:
        setContentNewFlag(15);
        setContentNewFlag(89);
        setContentNewFlag(90);
        setContentNewFlag(91);
        break;
    case 13:
        setContentNewFlag(16);
        break;
    }
}

void CJiminyMemoFlag::checkSoraEvent_FloorClear(INT inFloorId) {
    switch (inFloorId) {
    case 1:
        setContentNewFlag(0);
        setContentNewFlag(33);
        break;
    case 6:
        setContentNewFlag(1);
        setContentNewFlag(35);
        break;
    case 9:
        setContentNewFlag(2);
        break;
    case 10:
        setContentNewFlag(32);
        break;
    case 12:
        setContentNewFlag(3);
        setContentNewFlag(30);
        setContentNewFlag(31);
        break;
    case 13:
        setContentNewFlag(34);
        break;
    }
}

void CJiminyMemoFlag::checkSoraEvent_Monstro() {
    if (CGameWork::getByte(1, 2) == 1) setContentNewFlag(399);
}
void CJiminyMemoFlag::checkSoraEvent_DownEnemy(INT inKind, INT inNum) {
    INT charaFlag = -1;
    INT enemyFlag = -1;
    switch (inKind) {
    case 0:
        break;
    case 1:
        charaFlag = 92;
        enemyFlag = 0;
        break;
    case 2:
        charaFlag = 93;
        enemyFlag = 1;
        break;
    case 3:
        charaFlag = 95;
        enemyFlag = 3;
        break;
    case 4:
        charaFlag = 96;
        enemyFlag = 4;
        break;
    case 5:
        charaFlag = 97;
        enemyFlag = 5;
        break;
    case 6:
        charaFlag = 98;
        enemyFlag = 6;
        break;
    case 7:
        charaFlag = 99;
        enemyFlag = 7;
        break;
    case 8:
        charaFlag = 100;
        enemyFlag = 8;
        break;
    case 9:
        charaFlag = 94;
        enemyFlag = 2;
        break;
    case 10:
        charaFlag = 103;
        enemyFlag = 11;
        break;
    case 11:
        charaFlag = 102;
        enemyFlag = 10;
        break;
    case 12:
        charaFlag = 109;
        enemyFlag = 17;
        break;
    case 13:
        charaFlag = 110;
        enemyFlag = 18;
        break;
    case 14:
        charaFlag = 105;
        enemyFlag = 13;
        break;
    case 15:
        charaFlag = 101;
        enemyFlag = 9;
        break;
    case 16:
        charaFlag = 118;
        enemyFlag = 26;
        break;
    case 17:
        charaFlag = 119;
        enemyFlag = 27;
        break;
    case 18:
        charaFlag = 106;
        enemyFlag = 14;
        break;
    case 19:
        charaFlag = 107;
        enemyFlag = 15;
        break;
    case 20:
        charaFlag = 108;
        enemyFlag = 16;
        break;
    case 21:
        charaFlag = 111;
        enemyFlag = 19;
        break;
    case 22:
        charaFlag = 112;
        enemyFlag = 20;
        break;
    case 23:
        charaFlag = 113;
        enemyFlag = 21;
        break;
    case 24:
        charaFlag = 115;
        enemyFlag = 23;
        break;
    case 25:
        charaFlag = 116;
        enemyFlag = 24;
        break;
    case 26:
        charaFlag = 114;
        enemyFlag = 22;
        break;
    case 27:
        charaFlag = 117;
        enemyFlag = 25;
        break;
    case 28:
        charaFlag = 104;
        enemyFlag = 12;
        break;
    case 29:
        charaFlag = 121;
        enemyFlag = 29;
        break;
    case 30:
        charaFlag = 122;
        enemyFlag = 30;
        break;
    case 31:
        charaFlag = 120;
        enemyFlag = 28;
        break;
    case 100:
        charaFlag = 123;
        enemyFlag = 31;
        break;
    case 108:
        charaFlag = 124;
        enemyFlag = 32;
        break;
    case 103:
        charaFlag = 125;
        enemyFlag = 33;
        break;
    case 113:
        charaFlag = 126;
        enemyFlag = 34;
        break;
    default:
        checkSoraEvent_13BossBattle(inKind);
        break;
    }
    if (charaFlag >= 0) setContentNewFlag(charaFlag);
    if (enemyFlag >= 0 && inNum > 0) {
        INT num = getDownEnemyNum(enemyFlag) + inNum;
        if (num >= 10000) num = 9999;
        setDownEnemyNum(enemyFlag, num);
    }
}
void CJiminyMemoFlag::checkSoraEvent_GetCard(INT inKind) {
    INT cardFlag = -1;
    INT charaFlag = -1;
    switch ((BYTE)inKind) {
    case 1:
        cardFlag = 183;
        break;
    case 2:
        cardFlag = 184;
        break;
    case 3:
        cardFlag = 185;
        break;
    case 4:
        cardFlag = 186;
        break;
    case 5:
        cardFlag = 187;
        break;
    case 6:
        cardFlag = 188;
        break;
    case 7:
        cardFlag = 189;
        break;
    case 8:
        cardFlag = 190;
        break;
    case 9:
        cardFlag = 191;
        break;
    case 10:
        cardFlag = 192;
        break;
    case 11:
        cardFlag = 193;
        break;
    case 12:
        cardFlag = 194;
        break;
    case 13:
        cardFlag = 195;
        break;
    case 14:
        cardFlag = 196;
        break;
    case 15:
        cardFlag = 197;
        break;
    case 16:
        cardFlag = 198;
        break;
    case 17:
        cardFlag = 199;
        break;
    case 19:
        cardFlag = 200;
        break;
    case 20:
        cardFlag = 201;
        break;
    case 21:
        cardFlag = 202;
        break;
    case 22:
        cardFlag = 203;
        break;
    case 23:
        cardFlag = 204;
        break;
    case 24:
        cardFlag = 205;
        break;
    case 26:
        cardFlag = 206;
        break;
    case 27:
        cardFlag = 207;
        break;
    case 28:
        cardFlag = 208;
        break;
    case 29:
        cardFlag = 209;
        break;
    case 30:
        cardFlag = 210;
        break;
    case 31:
        cardFlag = 211;
        break;
    case 32:
        cardFlag = 212;
        break;
    case 35:
        cardFlag = 213;
        charaFlag = 36;
        break;
    case 36:
        cardFlag = 214;
        break;
    case 37:
        cardFlag = 215;
        charaFlag = 38;
        break;
    case 38:
        cardFlag = 216;
        charaFlag = 37;
        break;
    case 39:
        cardFlag = 217;
        break;
    case 40:
        cardFlag = 218;
        charaFlag = 39;
        break;
    case 41:
        cardFlag = 219;
        break;
    case 42:
        cardFlag = 220;
        break;
    case 43:
        cardFlag = 221;
        break;
    case 44:
        cardFlag = 222;
        break;
    case 45:
        cardFlag = 223;
        break;
    case 46:
        cardFlag = 224;
        break;
    case 47:
        cardFlag = 225;
        break;
    case 48:
        cardFlag = 226;
        break;
    case 49:
        cardFlag = 227;
        break;
    case 50:
        cardFlag = 228;
        break;
    case 51:
        cardFlag = 229;
        break;
    case 52:
        cardFlag = 230;
        break;
    case 53:
        cardFlag = 231;
        break;
    case 54:
        cardFlag = 232;
        break;
    case 55:
        cardFlag = 233;
        break;
    case 57:
        cardFlag = 234;
        break;
    case 68:
        cardFlag = 235;
        break;
    case 69:
        cardFlag = 236;
        break;
    case 75:
        cardFlag = 237;
        break;
    case 76:
        cardFlag = 238;
        break;
    case 70:
        cardFlag = 239;
        break;
    case 79:
        cardFlag = 240;
        break;
    case 82:
        cardFlag = 241;
        break;
    case 78:
        cardFlag = 242;
        break;
    case 87:
        cardFlag = 243;
        break;
    case 71:
        cardFlag = 244;
        break;
    case 72:
        cardFlag = 245;
        break;
    case 73:
        cardFlag = 246;
        break;
    case 74:
        cardFlag = 247;
        break;
    case 92:
        cardFlag = 248;
        break;
    case 77:
        cardFlag = 249;
        break;
    case 80:
        cardFlag = 250;
        break;
    case 85:
        cardFlag = 251;
        break;
    case 88:
        cardFlag = 252;
        break;
    case 86:
        cardFlag = 253;
        break;
    case 81:
        cardFlag = 254;
        break;
    case 84:
        cardFlag = 255;
        break;
    case 83:
        cardFlag = 256;
        break;
    case 91:
        cardFlag = 257;
        break;
    case 90:
        cardFlag = 258;
        break;
    case 94:
        cardFlag = 259;
        break;
    case 95:
        cardFlag = 260;
        break;
    case 96:
        cardFlag = 261;
        break;
    case 97:
        cardFlag = 262;
        break;
    case 98:
        cardFlag = 263;
        break;
    case 93:
        cardFlag = 264;
        break;
    case 99:
        cardFlag = 265;
        break;
    case 104:
        cardFlag = 266;
        break;
    case 101:
        cardFlag = 267;
        break;
    case 105:
        cardFlag = 268;
        break;
    case 107:
        cardFlag = 269;
        break;
    case 106:
        cardFlag = 270;
        break;
    case 100:
        cardFlag = 271;
        break;
    case 108:
        cardFlag = 272;
        break;
    case 109:
        cardFlag = 273;
        break;
    case 102:
        cardFlag = 274;
        break;
    case 110:
        cardFlag = 275;
        break;
    case 103:
        cardFlag = 276;
        break;
    case 116:
        cardFlag = 277;
        break;
    case 118:
        cardFlag = 278;
        break;
    case 119:
        cardFlag = 279;
        break;
    case 120:
        cardFlag = 280;
        break;
    case 113:
        cardFlag = 281;
        break;
    case 115:
        cardFlag = 282;
        break;
    case 117:
        cardFlag = 283;
        break;
    case 121:
        cardFlag = 284;
        break;
    case 111:
        cardFlag = 285;
        break;
    case 122:
        cardFlag = 286;
        break;
    case 123:
        cardFlag = 287;
        break;
    case 114:
        cardFlag = 288;
        break;
    case 112:
        cardFlag = 289;
        break;
    case 124:
        cardFlag = 290;
        break;
    case 125:
        cardFlag = 333;
        break;
    case 126:
        cardFlag = 334;
        break;
    case 161:
        cardFlag = 291;
        break;
    case 162:
        cardFlag = 292;
        break;
    case 163:
        cardFlag = 293;
        break;
    case 164:
        cardFlag = 294;
        break;
    case 165:
        cardFlag = 295;
        break;
    case 166:
        cardFlag = 296;
        break;
    case 167:
        cardFlag = 297;
        break;
    case 168:
        cardFlag = 298;
        break;
    case 169:
        cardFlag = 299;
        break;
    case 170:
        cardFlag = 300;
        break;
    case 171:
        cardFlag = 301;
        break;
    case 172:
        cardFlag = 302;
        break;
    case 173:
        cardFlag = 303;
        break;
    case 174:
        cardFlag = 304;
        break;
    case 175:
        cardFlag = 305;
        break;
    case 176:
        cardFlag = 306;
        break;
    case 177:
        cardFlag = 307;
        break;
    case 178:
        cardFlag = 308;
        break;
    case 179:
        cardFlag = 309;
        break;
    case 180:
        cardFlag = 310;
        break;
    case 181:
        cardFlag = 311;
        break;
    case 182:
        cardFlag = 312;
        break;
    case 183:
        cardFlag = 313;
        break;
    case 184:
        cardFlag = 314;
        break;
    case 189:
        cardFlag = 315;
        break;
    case 186:
        cardFlag = 316;
        break;
    case 187:
        cardFlag = 317;
        break;
    case 188:
        cardFlag = 318;
        break;
    case 185:
        cardFlag = 319;
        break;
    case 190:
        cardFlag = 320;
        break;
    case 191:
        cardFlag = 321;
        break;
    case 192:
        cardFlag = 322;
        break;
    case 193:
        cardFlag = 323;
        break;
    case 194:
        cardFlag = 324;
        break;
    case 195:
        cardFlag = 325;
        break;
    case 196:
        cardFlag = 326;
        break;
    case 197:
        cardFlag = 327;
        break;
    case 198:
        cardFlag = 328;
        break;
    case 199:
        cardFlag = 329;
        break;
    case 200:
        cardFlag = 330;
        break;
    case 201:
        cardFlag = 331;
        break;
    case 58:
    case 59:
    case 60:
    case 61:
    case 62:
    case 63:
    case 64:
    case 65:
        cardFlag = 332;
        break;
    }
    if (cardFlag >= 0) setContentNewFlag(cardFlag);
    if (charaFlag >= 0) setContentNewFlag(charaFlag);
}
void CJiminyMemoFlag::checkSoraEvent_13BossBattle(INT inKind) {
    INT bossFlag = -1;
    switch (inKind) {
    case 112:
        bossFlag = 32;
        break;
    case 116:
        bossFlag = 33;
        break;
    case 117:
        bossFlag = 34;
        break;
    case 115:
        bossFlag = 35;
        break;
    }
    if (bossFlag >= 0) setContentNewFlag(bossFlag);
}

void CJiminyMemoFlag::checkSoraEvent_Minigame(INT inKind) {
    switch (inKind) {
    case 0:
        if (CGameWork::getInt(0, 35) & 0x20) setContentNewFlag(398);
        break;
    case 1:
        if (CGameWork::getInt(0, 35) & 0x2) setContentNewFlag(394);
        break;
    case 2:
        if (CGameWork::getInt(0, 35) & 0x10) setContentNewFlag(397);
        break;
    case 3:
        if (CGameWork::getInt(0, 35) & 0x4) setContentNewFlag(395);
        break;
    case 4:
        if (CGameWork::getInt(0, 35) & 0x8) setContentNewFlag(396);
        break;
    }
}

void CJiminyMemoFlag::checkSoraEvent_MoogleShop() {
    setContentNewFlag(40);
}

void CJiminyMemoFlag::checkSoraEvent_100AcreExit() {
    if (isContentOpen(13)) {
        if (isContentTalk(83)) setContentNewFlag(83);
        if (isContentTalk(84)) setContentNewFlag(84);
        if (isContentTalk(85)) setContentNewFlag(85);
        if (isContentTalk(86)) setContentNewFlag(86);
        if (isContentTalk(87)) setContentNewFlag(87);
        if (isContentTalk(88)) setContentNewFlag(88);
    }
}

void CJiminyMemoFlag::checkSoraEvent_100AcreTalk(INT inKind) {
    switch (inKind) {
    case 11:
        setContentTalkFlag(83);
        break;
    case 12:
        setContentTalkFlag(84);
        break;
    case 13:
        setContentTalkFlag(85);
        break;
    case 14:
        setContentTalkFlag(86);
        break;
    case 15:
        setContentTalkFlag(87);
        break;
    case 16:
        setContentTalkFlag(88);
        break;
    }
}
void CJiminyMemoFlag::checkRikuEvent_Default() {
    setContentNewFlag(127);
    setContentNewFlag(129);
    setContentNewFlag(131);
    setContentNewFlag(132);
    setContentNewFlag(136);
    setContentNewFlag(17);
    setContentNewFlag(335);
}

void CJiminyMemoFlag::checkRikuEvent_WorldClear(INT inWorldId) {
    switch (inWorldId) {
    case 2:
        setContentNewFlag(144);
        break;
    case 3:
        setContentNewFlag(146);
        break;
    case 6:
        setContentNewFlag(128);
        setContentNewFlag(130);
        setContentNewFlag(147);
        break;
    case 7:
        setContentNewFlag(145);
        break;
    case 8:
        setContentNewFlag(148);
        break;
    case 9:
        setContentNewFlag(143);
        break;
    case 1:
    case 13:
        break;
    }
}

void CJiminyMemoFlag::checkRikuEvent_FloorClear(INT inFloorId) {
    switch (inFloorId) {
    case -12:
        setContentNewFlag(18);
        break;
    case -10:
        setContentNewFlag(137);
        break;
    case -8:
        setContentNewFlag(19);
        setContentNewFlag(134);
        break;
    case -4:
        setContentNewFlag(20);
        setContentNewFlag(138);
        break;
    case -3:
        setContentNewFlag(21);
        setContentNewFlag(139);
        break;
    case -2:
        setContentNewFlag(22);
        setContentNewFlag(133);
        setContentNewFlag(140);
        setContentNewFlag(141);
        setContentNewFlag(142);
        setContentNewFlag(135);
        break;
    case -1:
        break;
    }
}
void CJiminyMemoFlag::checkRikuEvent_DownEnemy(INT inKind, INT inNum) {
    INT charaFlag = -1;
    INT enemyFlag = -1;
    switch (inKind) {
    case 0:
    case 16:
        break;
    case 1:
        charaFlag = 149;
        enemyFlag = 35;
        break;
    case 2:
        charaFlag = 150;
        enemyFlag = 36;
        break;
    case 3:
        charaFlag = 152;
        enemyFlag = 38;
        break;
    case 4:
        charaFlag = 153;
        enemyFlag = 39;
        break;
    case 5:
        charaFlag = 154;
        enemyFlag = 40;
        break;
    case 6:
        charaFlag = 155;
        enemyFlag = 41;
        break;
    case 7:
        charaFlag = 156;
        enemyFlag = 42;
        break;
    case 8:
        charaFlag = 157;
        enemyFlag = 43;
        break;
    case 9:
        charaFlag = 151;
        enemyFlag = 37;
        break;
    case 10:
        charaFlag = 160;
        enemyFlag = 46;
        break;
    case 11:
        charaFlag = 159;
        enemyFlag = 45;
        break;
    case 12:
        charaFlag = 166;
        enemyFlag = 52;
        break;
    case 13:
        charaFlag = 167;
        enemyFlag = 53;
        break;
    case 14:
        charaFlag = 162;
        enemyFlag = 48;
        break;
    case 15:
        charaFlag = 158;
        enemyFlag = 44;
        break;
    case 17:
        charaFlag = 175;
        enemyFlag = 61;
        break;
    case 18:
        charaFlag = 163;
        enemyFlag = 49;
        break;
    case 19:
        charaFlag = 164;
        enemyFlag = 50;
        break;
    case 20:
        charaFlag = 165;
        enemyFlag = 51;
        break;
    case 21:
        charaFlag = 168;
        enemyFlag = 54;
        break;
    case 22:
        charaFlag = 169;
        enemyFlag = 55;
        break;
    case 23:
        charaFlag = 170;
        enemyFlag = 56;
        break;
    case 24:
        charaFlag = 172;
        enemyFlag = 58;
        break;
    case 25:
        charaFlag = 173;
        enemyFlag = 59;
        break;
    case 26:
        charaFlag = 171;
        enemyFlag = 57;
        break;
    case 27:
        charaFlag = 174;
        enemyFlag = 60;
        break;
    case 28:
        charaFlag = 161;
        enemyFlag = 47;
        break;
    case 29:
        charaFlag = 177;
        enemyFlag = 63;
        break;
    case 30:
        charaFlag = 178;
        enemyFlag = 64;
        break;
    case 31:
        charaFlag = 176;
        enemyFlag = 62;
        break;
    case 100:
        charaFlag = 179;
        enemyFlag = 65;
        break;
    case 108:
        charaFlag = 180;
        enemyFlag = 66;
        break;
    case 103:
        charaFlag = 181;
        enemyFlag = 67;
        break;
    case 113:
        charaFlag = 182;
        enemyFlag = 68;
        break;
    }
    if (charaFlag >= 0) setContentNewFlag(charaFlag);
    if (enemyFlag >= 0 && inNum > 0) {
        INT num = getDownEnemyNum(enemyFlag) + inNum;
        if (num >= 10000) num = 9999;
        setDownEnemyNum(enemyFlag, num);
    }
}
void CJiminyMemoFlag::checkRikuEvent_GetCard(INT inKind) {
    INT cardFlag = -1;
    switch ((BYTE)inKind) {
    case 18:
        cardFlag = 335;
        break;
    case 42:
        cardFlag = 336;
        break;
    case 43:
        cardFlag = 337;
        break;
    case 56:
        cardFlag = 338;
        break;
    case 68:
        cardFlag = 340;
        break;
    case 75:
        cardFlag = 342;
        break;
    case 70:
        cardFlag = 341;
        break;
    case 79:
        cardFlag = 343;
        break;
    case 82:
        cardFlag = 345;
        break;
    case 87:
        cardFlag = 347;
        break;
    case 85:
        cardFlag = 346;
        break;
    case 81:
        cardFlag = 344;
        break;
    case 90:
        cardFlag = 348;
        break;
    case 89:
        cardFlag = 349;
        break;
    case 99:
        cardFlag = 350;
        break;
    case 104:
        cardFlag = 354;
        break;
    case 101:
        cardFlag = 352;
        break;
    case 105:
        cardFlag = 355;
        break;
    case 107:
        cardFlag = 357;
        break;
    case 106:
        cardFlag = 356;
        break;
    case 100:
        cardFlag = 351;
        break;
    case 108:
        cardFlag = 358;
        break;
    case 109:
        cardFlag = 359;
        break;
    case 102:
        cardFlag = 353;
        break;
    case 115:
        cardFlag = 360;
        break;
    case 117:
        cardFlag = 361;
        break;
    case 161:
        cardFlag = 362;
        break;
    case 162:
        cardFlag = 363;
        break;
    case 163:
        cardFlag = 364;
        break;
    case 164:
        cardFlag = 365;
        break;
    case 165:
        cardFlag = 366;
        break;
    case 166:
        cardFlag = 367;
        break;
    case 169:
        cardFlag = 368;
        break;
    case 170:
        cardFlag = 369;
        break;
    case 171:
        cardFlag = 370;
        break;
    case 172:
        cardFlag = 371;
        break;
    case 175:
        cardFlag = 372;
        break;
    case 176:
        cardFlag = 373;
        break;
    case 177:
        cardFlag = 374;
        break;
    case 178:
        cardFlag = 375;
        break;
    case 182:
        cardFlag = 376;
        break;
    case 183:
        cardFlag = 377;
        break;
    case 186:
        cardFlag = 378;
        break;
    case 187:
        cardFlag = 379;
        break;
    case 188:
        cardFlag = 380;
        break;
    case 185:
        cardFlag = 381;
        break;
    case 190:
        cardFlag = 382;
        break;
    case 191:
        cardFlag = 383;
        break;
    case 192:
        cardFlag = 384;
        break;
    case 193:
        cardFlag = 385;
        break;
    case 194:
        cardFlag = 386;
        break;
    case 195:
        cardFlag = 387;
        break;
    case 196:
        cardFlag = 388;
        break;
    case 197:
        cardFlag = 389;
        break;
    case 198:
        cardFlag = 390;
        break;
    case 200:
        cardFlag = 391;
        break;
    case 201:
        cardFlag = 392;
        break;
    case 202:
        cardFlag = 393;
        break;
    case 58:
    case 59:
    case 60:
    case 61:
    case 62:
    case 63:
    case 64:
    case 65:
        cardFlag = 339;
        break;
    }
    if (cardFlag >= 0) setContentNewFlag(cardFlag);
}
#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)
#define CFW_FLAG_SIZE 400
#define CFW_SCORE_SIZE 5
#define CFW_NUM_SIZE 69

void CJiminyMemoFlag::setContentFlag(INT inIndex, INT inOrFlag, INT inAndFlag) {
#line 1138
    ASSERT(inIndex < CFW_FLAG_SIZE);
    BYTE* work = &getMCWork().ViewState[inIndex];
    *work = (*work & ~inAndFlag) | inOrFlag;
}

INT CJiminyMemoFlag::isContentFlag(INT inIndex, INT inFlag) {
#line 1149
    ASSERT(inIndex < CFW_FLAG_SIZE);
    BYTE* work = &getMCWork().ViewState[inIndex];
    return *work & inFlag;
}

INT CJiminyMemoFlag::isEqualContentFlag(INT inIndex, INT inFlag) {
#line 1160
    ASSERT(inIndex < CFW_FLAG_SIZE);
    BYTE* work = &getMCWork().ViewState[inIndex];
    return (*work & 7) == inFlag;
}

INT CJiminyMemoFlag::isContentOpen(INT inIndex) {
#line 1171
    ASSERT(inIndex < CFW_FLAG_SIZE);
    BYTE* work = &getMCWork().ViewState[inIndex];
    return (*work & 5) != 0;
}

INT CJiminyMemoFlag::isContentNew(INT inIndex) {
#line 1182
    ASSERT(inIndex < CFW_FLAG_SIZE);
    if (isEqualContentFlag(inIndex, 1) || isEqualContentFlag(inIndex, 3)) return TRUE;
    return FALSE;
}

void CJiminyMemoFlag::setContentNewFlag(INT inIndex) {
#line 1192
    ASSERT(inIndex < CFW_FLAG_SIZE);
    if (!isContentFlag(inIndex, 4)) setContentFlag(inIndex, 1, 0);
}

void CJiminyMemoFlag::setContentTouchFlag(INT inIndex) {
#line 1204
    ASSERT(inIndex < CFW_FLAG_SIZE);
    if (!isContentFlag(inIndex, 4)) setContentFlag(inIndex, 2, 0);
}

void CJiminyMemoFlag::setContentTalkFlag(INT inIndex) {
#line 1216
    ASSERT(inIndex < CFW_FLAG_SIZE);
    if (!isContentOpen(inIndex)) setContentFlag(inIndex, 16, 0);
}

INT CJiminyMemoFlag::isContentTalk(INT inIndex) {
#line 1228
    ASSERT(inIndex < CFW_FLAG_SIZE);
    return isContentFlag(inIndex, 16) != 0;
}

void CJiminyMemoFlag::setContentScore(INT inIndex, UINT inValue) {
#line 1239
    ASSERT(inIndex < CFW_SCORE_SIZE);
    getMCWork().MinigameScore[inIndex] = inValue;
}

UINT CJiminyMemoFlag::getContentScore(INT inIndex) {
#line 1250
    ASSERT(inIndex < CFW_SCORE_SIZE);
    return getMCWork().MinigameScore[inIndex];
}

void CJiminyMemoFlag::setDownEnemyNum(INT inIndex, USHORT inValue) {
#line 1261
    ASSERT(inIndex < CFW_NUM_SIZE);
    getMCWork().DownEnemyNum[inIndex] = inValue;
}

USHORT CJiminyMemoFlag::getDownEnemyNum(INT inIndex) {
#line 1272
    ASSERT(inIndex < CFW_NUM_SIZE);
    return getMCWork().DownEnemyNum[inIndex];
}

void CJiminyMemoFlag::setDebugNewFlag(BOOL inIsSet, INT inIndex) {
    if (inIsSet) {
        setContentFlag(inIndex, 1, 6);
    } else {
        setContentFlag(inIndex, 0, 7);
    }
}

void CJiminyMemoFlag::setDebugTalkFlag(BOOL inIsSet, INT inIndex) {
    if (inIsSet) {
        setContentFlag(inIndex, 16, 0);
    } else {
        setContentFlag(inIndex, 0, 16);
    }
}
void CJiminyMemoFlag::debugEvent(BOOL inIsSet, INT inPlayer, INT inEvent, INT inParam0, INT inParam1) {
    if (inPlayer == 0) {
        switch (inEvent) {
        case 0: debugSoraEvent_Default(inIsSet); break;
        case 1: debugSoraEvent_WorldClear(inIsSet, inParam0); break;
        case 2: debugSoraEvent_FloorClear(inIsSet, inParam0); break;
        case 3: debugSoraEvent_DownEnemy(inIsSet, inParam0, inParam1); break;
        case 4: debugSoraEvent_GetCard(inIsSet, inParam0); break;
        case 5: debugSoraEvent_MoogleShop(inIsSet); break;
        case 6: debugSoraEvent_Monstro(inIsSet); break;
        case 8: debugSoraEvent_Minigame(inIsSet, inParam0); break;
        case 9: debugSoraEvent_100AcreExit(inIsSet); break;
        case 10: debugSoraEvent_100AcreTalk(inIsSet, inParam0); break;
        case 11: debugSoraEvent_All(1); break;
        case 12: debugSoraEvent_All(4); break;
        case 13: debugSoraEvent_All(0); break;
        }
    } else {
        switch (inEvent) {
        case 0: debugRikuEvent_Default(inIsSet); break;
        case 1: debugRikuEvent_WorldClear(inIsSet, inParam0); break;
        case 2: debugRikuEvent_FloorClear(inIsSet, inParam0); break;
        case 3: debugRikuEvent_DownEnemy(inIsSet, inParam0, inParam1); break;
        case 4: debugRikuEvent_GetCard(inIsSet, inParam0); break;
        case 11: debugRikuEvent_All(1); break;
        case 12: debugRikuEvent_All(4); break;
        case 13: debugRikuEvent_All(0); break;
        }
    }
}
void CJiminyMemoFlag::debugSoraEvent_Default(BOOL inIsSet) {
    setDebugNewFlag(inIsSet, 23);
    setDebugNewFlag(inIsSet, 24);
    setDebugNewFlag(inIsSet, 25);
    setDebugNewFlag(inIsSet, 26);
    setDebugNewFlag(inIsSet, 28);
    setDebugNewFlag(inIsSet, 29);
    setDebugNewFlag(inIsSet, 183);
}
void CJiminyMemoFlag::debugSoraEvent_WorldClear(BOOL inIsSet, INT inWorldId) {
    switch (inWorldId) {
    case 1:
        setDebugNewFlag(inIsSet, 4);
        setDebugNewFlag(inIsSet, 41);
        setDebugNewFlag(inIsSet, 42);
        setDebugNewFlag(inIsSet, 43);
        setDebugNewFlag(inIsSet, 44);
        break;
    case 2:
        setDebugNewFlag(inIsSet, 7);
        setDebugNewFlag(inIsSet, 55);
        setDebugNewFlag(inIsSet, 56);
        setDebugNewFlag(inIsSet, 57);
        setDebugNewFlag(inIsSet, 58);
        setDebugNewFlag(inIsSet, 59);
        setDebugNewFlag(inIsSet, 60);
        break;
    case 3:
        setDebugNewFlag(inIsSet, 6);
        setDebugNewFlag(inIsSet, 51);
        setDebugNewFlag(inIsSet, 52);
        setDebugNewFlag(inIsSet, 53);
        setDebugNewFlag(inIsSet, 54);
        break;
    case 4:
        setDebugNewFlag(inIsSet, 5);
        setDebugNewFlag(inIsSet, 45);
        setDebugNewFlag(inIsSet, 46);
        setDebugNewFlag(inIsSet, 47);
        setDebugNewFlag(inIsSet, 48);
        setDebugNewFlag(inIsSet, 49);
        setDebugNewFlag(inIsSet, 50);
        break;
    case 5:
        setDebugNewFlag(inIsSet, 9);
        setDebugNewFlag(inIsSet, 68);
        setDebugNewFlag(inIsSet, 69);
        break;
    case 6:
        setDebugNewFlag(inIsSet, 8);
        setDebugNewFlag(inIsSet, 61);
        setDebugNewFlag(inIsSet, 62);
        setDebugNewFlag(inIsSet, 63);
        setDebugNewFlag(inIsSet, 64);
        setDebugNewFlag(inIsSet, 65);
        setDebugNewFlag(inIsSet, 66);
        setDebugNewFlag(inIsSet, 67);
        break;
    case 7:
        setDebugNewFlag(inIsSet, 10);
        setDebugNewFlag(inIsSet, 70);
        setDebugNewFlag(inIsSet, 71);
        setDebugNewFlag(inIsSet, 72);
        setDebugNewFlag(inIsSet, 73);
        break;
    case 8:
        setDebugNewFlag(inIsSet, 11);
        setDebugNewFlag(inIsSet, 74);
        setDebugNewFlag(inIsSet, 75);
        setDebugNewFlag(inIsSet, 76);
        setDebugNewFlag(inIsSet, 77);
        break;
    case 9:
        setDebugNewFlag(inIsSet, 12);
        setDebugNewFlag(inIsSet, 78);
        setDebugNewFlag(inIsSet, 79);
        setDebugNewFlag(inIsSet, 80);
        setDebugNewFlag(inIsSet, 81);
        break;
    case 10:
        setDebugNewFlag(inIsSet, 13);
        setDebugNewFlag(inIsSet, 82);
        break;
    case 11:
        setDebugNewFlag(inIsSet, 14);
        break;
    case 12:
        setDebugNewFlag(inIsSet, 15);
        setDebugNewFlag(inIsSet, 89);
        setDebugNewFlag(inIsSet, 90);
        setDebugNewFlag(inIsSet, 91);
        break;
    case 13:
        setDebugNewFlag(inIsSet, 16);
        break;
    }
}
void CJiminyMemoFlag::debugSoraEvent_FloorClear(BOOL inIsSet, INT inFloor) {
    switch (inFloor) {
    case 1:
        setDebugNewFlag(inIsSet, 0);
        setDebugNewFlag(inIsSet, 33);
        break;
    case 6:
        setDebugNewFlag(inIsSet, 1);
        setDebugNewFlag(inIsSet, 35);
        break;
    case 9:
        setDebugNewFlag(inIsSet, 2);
        break;
    case 10:
        setDebugNewFlag(inIsSet, 32);
        break;
    case 12:
        setDebugNewFlag(inIsSet, 3);
        setDebugNewFlag(inIsSet, 30);
        setDebugNewFlag(inIsSet, 31);
        break;
    case 13:
        setDebugNewFlag(inIsSet, 34);
        break;
    }
}
void CJiminyMemoFlag::debugSoraEvent_Monstro(BOOL inIsSet) {
    setDebugNewFlag(inIsSet, 399);
}
void CJiminyMemoFlag::debugSoraEvent_DownEnemy(BOOL inIsSet, INT inKind, INT inNum) {
    INT charaFlag = -1;
    INT enemyFlag = -1;
    switch (inKind) {
    case 0:
        break;
    case 1:
        charaFlag = 92;
        enemyFlag = 0;
        break;
    case 2:
        charaFlag = 93;
        enemyFlag = 1;
        break;
    case 3:
        charaFlag = 95;
        enemyFlag = 3;
        break;
    case 4:
        charaFlag = 96;
        enemyFlag = 4;
        break;
    case 5:
        charaFlag = 97;
        enemyFlag = 5;
        break;
    case 6:
        charaFlag = 98;
        enemyFlag = 6;
        break;
    case 7:
        charaFlag = 99;
        enemyFlag = 7;
        break;
    case 8:
        charaFlag = 100;
        enemyFlag = 8;
        break;
    case 9:
        charaFlag = 94;
        enemyFlag = 2;
        break;
    case 10:
        charaFlag = 103;
        enemyFlag = 11;
        break;
    case 11:
        charaFlag = 102;
        enemyFlag = 10;
        break;
    case 12:
        charaFlag = 109;
        enemyFlag = 17;
        break;
    case 13:
        charaFlag = 110;
        enemyFlag = 18;
        break;
    case 14:
        charaFlag = 105;
        enemyFlag = 13;
        break;
    case 15:
        charaFlag = 101;
        enemyFlag = 9;
        break;
    case 16:
        charaFlag = 118;
        enemyFlag = 26;
        break;
    case 17:
        charaFlag = 119;
        enemyFlag = 27;
        break;
    case 18:
        charaFlag = 106;
        enemyFlag = 14;
        break;
    case 19:
        charaFlag = 107;
        enemyFlag = 15;
        break;
    case 20:
        charaFlag = 108;
        enemyFlag = 16;
        break;
    case 21:
        charaFlag = 111;
        enemyFlag = 19;
        break;
    case 22:
        charaFlag = 112;
        enemyFlag = 20;
        break;
    case 23:
        charaFlag = 113;
        enemyFlag = 21;
        break;
    case 24:
        charaFlag = 115;
        enemyFlag = 23;
        break;
    case 25:
        charaFlag = 116;
        enemyFlag = 24;
        break;
    case 26:
        charaFlag = 114;
        enemyFlag = 22;
        break;
    case 27:
        charaFlag = 117;
        enemyFlag = 25;
        break;
    case 28:
        charaFlag = 104;
        enemyFlag = 12;
        break;
    case 29:
        charaFlag = 121;
        enemyFlag = 29;
        break;
    case 30:
        charaFlag = 122;
        enemyFlag = 30;
        break;
    case 31:
        charaFlag = 120;
        enemyFlag = 28;
        break;
    case 100:
        charaFlag = 123;
        enemyFlag = 31;
        break;
    case 108:
        charaFlag = 124;
        enemyFlag = 32;
        break;
    case 103:
        charaFlag = 125;
        enemyFlag = 33;
        break;
    case 113:
        charaFlag = 126;
        enemyFlag = 34;
        break;
    default:
        debugSoraEvent_13BossBattle(inIsSet, inKind);
        break;
    }
    if (charaFlag >= 0) {
        setDebugNewFlag(inIsSet, charaFlag);
        if (enemyFlag >= 0 && inNum > 0) setDownEnemyNum(enemyFlag, inNum);
    }
}
void CJiminyMemoFlag::debugSoraEvent_GetCard(BOOL inIsSet, INT inKind) {
    INT cardFlag = -1;
    INT charaFlag = -1;
    switch ((BYTE)inKind) {
    case 1:
        cardFlag = 183;
        break;
    case 2:
        cardFlag = 184;
        break;
    case 3:
        cardFlag = 185;
        break;
    case 4:
        cardFlag = 186;
        break;
    case 5:
        cardFlag = 187;
        break;
    case 6:
        cardFlag = 188;
        break;
    case 7:
        cardFlag = 189;
        break;
    case 8:
        cardFlag = 190;
        break;
    case 9:
        cardFlag = 191;
        break;
    case 10:
        cardFlag = 192;
        break;
    case 11:
        cardFlag = 193;
        break;
    case 12:
        cardFlag = 194;
        break;
    case 13:
        cardFlag = 195;
        break;
    case 14:
        cardFlag = 196;
        break;
    case 15:
        cardFlag = 197;
        break;
    case 16:
        cardFlag = 198;
        break;
    case 17:
        cardFlag = 199;
        break;
    case 19:
        cardFlag = 200;
        break;
    case 20:
        cardFlag = 201;
        break;
    case 21:
        cardFlag = 202;
        break;
    case 22:
        cardFlag = 203;
        break;
    case 23:
        cardFlag = 204;
        break;
    case 24:
        cardFlag = 205;
        break;
    case 26:
        cardFlag = 206;
        break;
    case 27:
        cardFlag = 207;
        break;
    case 28:
        cardFlag = 208;
        break;
    case 29:
        cardFlag = 209;
        break;
    case 30:
        cardFlag = 210;
        break;
    case 31:
        cardFlag = 211;
        break;
    case 32:
        cardFlag = 212;
        break;
    case 35:
        cardFlag = 213;
        charaFlag = 36;
        break;
    case 36:
        cardFlag = 214;
        break;
    case 37:
        cardFlag = 215;
        charaFlag = 38;
        break;
    case 38:
        cardFlag = 216;
        charaFlag = 37;
        break;
    case 39:
        cardFlag = 217;
        break;
    case 40:
        cardFlag = 218;
        charaFlag = 39;
        break;
    case 41:
        cardFlag = 219;
        break;
    case 42:
        cardFlag = 220;
        break;
    case 43:
        cardFlag = 221;
        break;
    case 44:
        cardFlag = 222;
        break;
    case 45:
        cardFlag = 223;
        break;
    case 46:
        cardFlag = 224;
        break;
    case 47:
        cardFlag = 225;
        break;
    case 48:
        cardFlag = 226;
        break;
    case 49:
        cardFlag = 227;
        break;
    case 50:
        cardFlag = 228;
        break;
    case 51:
        cardFlag = 229;
        break;
    case 52:
        cardFlag = 230;
        break;
    case 53:
        cardFlag = 231;
        break;
    case 54:
        cardFlag = 232;
        break;
    case 55:
        cardFlag = 233;
        break;
    case 57:
        cardFlag = 234;
        break;
    case 68:
        cardFlag = 235;
        break;
    case 69:
        cardFlag = 236;
        break;
    case 75:
        cardFlag = 237;
        break;
    case 76:
        cardFlag = 238;
        break;
    case 70:
        cardFlag = 239;
        break;
    case 79:
        cardFlag = 240;
        break;
    case 82:
        cardFlag = 241;
        break;
    case 78:
        cardFlag = 242;
        break;
    case 87:
        cardFlag = 243;
        break;
    case 71:
        cardFlag = 244;
        break;
    case 72:
        cardFlag = 245;
        break;
    case 73:
        cardFlag = 246;
        break;
    case 74:
        cardFlag = 247;
        break;
    case 92:
        cardFlag = 248;
        break;
    case 77:
        cardFlag = 249;
        break;
    case 80:
        cardFlag = 250;
        break;
    case 85:
        cardFlag = 251;
        break;
    case 88:
        cardFlag = 252;
        break;
    case 86:
        cardFlag = 253;
        break;
    case 81:
        cardFlag = 254;
        break;
    case 84:
        cardFlag = 255;
        break;
    case 83:
        cardFlag = 256;
        break;
    case 91:
        cardFlag = 257;
        break;
    case 90:
        cardFlag = 258;
        break;
    case 94:
        cardFlag = 259;
        break;
    case 95:
        cardFlag = 260;
        break;
    case 96:
        cardFlag = 261;
        break;
    case 97:
        cardFlag = 262;
        break;
    case 98:
        cardFlag = 263;
        break;
    case 93:
        cardFlag = 264;
        break;
    case 99:
        cardFlag = 265;
        break;
    case 104:
        cardFlag = 266;
        break;
    case 101:
        cardFlag = 267;
        break;
    case 105:
        cardFlag = 268;
        break;
    case 107:
        cardFlag = 269;
        break;
    case 106:
        cardFlag = 270;
        break;
    case 100:
        cardFlag = 271;
        break;
    case 108:
        cardFlag = 272;
        break;
    case 109:
        cardFlag = 273;
        break;
    case 102:
        cardFlag = 274;
        break;
    case 110:
        cardFlag = 275;
        break;
    case 103:
        cardFlag = 276;
        break;
    case 116:
        cardFlag = 277;
        break;
    case 118:
        cardFlag = 278;
        break;
    case 119:
        cardFlag = 279;
        break;
    case 120:
        cardFlag = 280;
        break;
    case 113:
        cardFlag = 281;
        break;
    case 115:
        cardFlag = 282;
        break;
    case 117:
        cardFlag = 283;
        break;
    case 121:
        cardFlag = 284;
        break;
    case 111:
        cardFlag = 285;
        break;
    case 122:
        cardFlag = 286;
        break;
    case 123:
        cardFlag = 287;
        break;
    case 114:
        cardFlag = 288;
        break;
    case 112:
        cardFlag = 289;
        break;
    case 124:
        cardFlag = 290;
        break;
    case 125:
        cardFlag = 333;
        break;
    case 126:
        cardFlag = 334;
        break;
    case 161:
        cardFlag = 291;
        break;
    case 162:
        cardFlag = 292;
        break;
    case 163:
        cardFlag = 293;
        break;
    case 164:
        cardFlag = 294;
        break;
    case 165:
        cardFlag = 295;
        break;
    case 166:
        cardFlag = 296;
        break;
    case 167:
        cardFlag = 297;
        break;
    case 168:
        cardFlag = 298;
        break;
    case 169:
        cardFlag = 299;
        break;
    case 170:
        cardFlag = 300;
        break;
    case 171:
        cardFlag = 301;
        break;
    case 172:
        cardFlag = 302;
        break;
    case 173:
        cardFlag = 303;
        break;
    case 174:
        cardFlag = 304;
        break;
    case 175:
        cardFlag = 305;
        break;
    case 176:
        cardFlag = 306;
        break;
    case 177:
        cardFlag = 307;
        break;
    case 178:
        cardFlag = 308;
        break;
    case 179:
        cardFlag = 309;
        break;
    case 180:
        cardFlag = 310;
        break;
    case 181:
        cardFlag = 311;
        break;
    case 182:
        cardFlag = 312;
        break;
    case 183:
        cardFlag = 313;
        break;
    case 184:
        cardFlag = 314;
        break;
    case 189:
        cardFlag = 315;
        break;
    case 186:
        cardFlag = 316;
        break;
    case 187:
        cardFlag = 317;
        break;
    case 188:
        cardFlag = 318;
        break;
    case 185:
        cardFlag = 319;
        break;
    case 190:
        cardFlag = 320;
        break;
    case 191:
        cardFlag = 321;
        break;
    case 192:
        cardFlag = 322;
        break;
    case 193:
        cardFlag = 323;
        break;
    case 194:
        cardFlag = 324;
        break;
    case 195:
        cardFlag = 325;
        break;
    case 196:
        cardFlag = 326;
        break;
    case 197:
        cardFlag = 327;
        break;
    case 198:
        cardFlag = 328;
        break;
    case 199:
        cardFlag = 329;
        break;
    case 200:
        cardFlag = 330;
        break;
    case 201:
        cardFlag = 331;
        break;
    case 58:
    case 59:
    case 60:
    case 61:
    case 62:
    case 63:
    case 64:
    case 65:
        cardFlag = 332;
        break;
    }
    if (cardFlag >= 0) setDebugNewFlag(inIsSet, cardFlag);
    if (charaFlag >= 0) setDebugNewFlag(inIsSet, charaFlag);
}
void CJiminyMemoFlag::debugSoraEvent_13BossBattle(BOOL inIsSet, INT inKind) {
    INT bossFlag = -1;
    switch (inKind) {
    case 112:
        bossFlag = 32;
        break;
    case 116:
        bossFlag = 33;
        break;
    case 117:
        bossFlag = 34;
        break;
    case 115:
        bossFlag = 35;
        break;
    }
    if (bossFlag >= 0) setDebugNewFlag(inIsSet, bossFlag);
}

void CJiminyMemoFlag::debugSoraEvent_Minigame(BOOL inIsSet, INT inKind) {
    switch (inKind) {
    case 0:
        setDebugNewFlag(inIsSet, 398);
        break;
    case 1:
        setDebugNewFlag(inIsSet, 394);
        break;
    case 2:
        setDebugNewFlag(inIsSet, 397);
        break;
    case 3:
        setDebugNewFlag(inIsSet, 395);
        break;
    case 4:
        setDebugNewFlag(inIsSet, 396);
        break;
    }
}

void CJiminyMemoFlag::debugSoraEvent_MoogleShop(BOOL inIsSet) {
    setDebugNewFlag(inIsSet, 40);
}

void CJiminyMemoFlag::debugSoraEvent_100AcreExit(BOOL inIsSet) {
    setDebugNewFlag(isContentTalk(83), 83);
    setDebugNewFlag(isContentTalk(84), 84);
    setDebugNewFlag(isContentTalk(85), 85);
    setDebugNewFlag(isContentTalk(86), 86);
    setDebugNewFlag(isContentTalk(87), 87);
    setDebugNewFlag(isContentTalk(88), 88);
}

void CJiminyMemoFlag::debugSoraEvent_100AcreTalk(BOOL inIsSet, INT inKind) {
    switch (inKind) {
    case 11:
        setDebugTalkFlag(inIsSet, 83);
        break;
    case 12:
        setDebugTalkFlag(inIsSet, 84);
        break;
    case 13:
        setDebugTalkFlag(inIsSet, 85);
        break;
    case 14:
        setDebugTalkFlag(inIsSet, 86);
        break;
    case 15:
        setDebugTalkFlag(inIsSet, 87);
        break;
    case 16:
        setDebugTalkFlag(inIsSet, 88);
        break;
    }
}

void CJiminyMemoFlag::debugRikuEvent_Default(BOOL inIsSet) {
    setDebugNewFlag(inIsSet, 127);
    setDebugNewFlag(inIsSet, 129);
    setDebugNewFlag(inIsSet, 131);
    setDebugNewFlag(inIsSet, 132);
    setDebugNewFlag(inIsSet, 136);
    setDebugNewFlag(inIsSet, 17);
    setDebugNewFlag(inIsSet, 335);
}
void CJiminyMemoFlag::debugRikuEvent_WorldClear(BOOL inIsSet, INT inWorldId) {
    switch (inWorldId) {
    case 2:
        setDebugNewFlag(inIsSet, 144);
        break;
    case 3:
        setDebugNewFlag(inIsSet, 146);
        break;
    case 6:
        setDebugNewFlag(inIsSet, 128);
        setDebugNewFlag(inIsSet, 130);
        setDebugNewFlag(inIsSet, 147);
        break;
    case 7:
        setDebugNewFlag(inIsSet, 145);
        break;
    case 8:
        setDebugNewFlag(inIsSet, 148);
        break;
    case 9:
        setDebugNewFlag(inIsSet, 143);
        break;
    case 1:
    case 13:
        break;
    }
}
void CJiminyMemoFlag::debugRikuEvent_FloorClear(BOOL inIsSet, INT inFloor) {
    switch (inFloor) {
    case -12:
        setDebugNewFlag(inIsSet, 18);
        break;
    case -10:
        setDebugNewFlag(inIsSet, 137);
        break;
    case -8:
        setDebugNewFlag(inIsSet, 19);
        setDebugNewFlag(inIsSet, 134);
        break;
    case -4:
        setDebugNewFlag(inIsSet, 20);
        setDebugNewFlag(inIsSet, 138);
        break;
    case -3:
        setDebugNewFlag(inIsSet, 21);
        setDebugNewFlag(inIsSet, 139);
        break;
    case -2:
        setDebugNewFlag(inIsSet, 22);
        setDebugNewFlag(inIsSet, 133);
        setDebugNewFlag(inIsSet, 140);
        setDebugNewFlag(inIsSet, 141);
        setDebugNewFlag(inIsSet, 142);
        setDebugNewFlag(inIsSet, 135);
        break;
    case -1:
        break;
    }
}
void CJiminyMemoFlag::debugRikuEvent_DownEnemy(BOOL inIsSet, INT inKind, INT inNum) {
    INT charaFlag = -1;
    INT enemyFlag = -1;
    switch (inKind) {
    case 0:
    case 16:
        break;
    case 1:
        charaFlag = 149;
        enemyFlag = 35;
        break;
    case 2:
        charaFlag = 150;
        enemyFlag = 36;
        break;
    case 3:
        charaFlag = 152;
        enemyFlag = 38;
        break;
    case 4:
        charaFlag = 153;
        enemyFlag = 39;
        break;
    case 5:
        charaFlag = 154;
        enemyFlag = 40;
        break;
    case 6:
        charaFlag = 155;
        enemyFlag = 41;
        break;
    case 7:
        charaFlag = 156;
        enemyFlag = 42;
        break;
    case 8:
        charaFlag = 157;
        enemyFlag = 43;
        break;
    case 9:
        charaFlag = 151;
        enemyFlag = 37;
        break;
    case 10:
        charaFlag = 160;
        enemyFlag = 46;
        break;
    case 11:
        charaFlag = 159;
        enemyFlag = 45;
        break;
    case 12:
        charaFlag = 166;
        enemyFlag = 52;
        break;
    case 13:
        charaFlag = 167;
        enemyFlag = 53;
        break;
    case 14:
        charaFlag = 162;
        enemyFlag = 48;
        break;
    case 15:
        charaFlag = 158;
        enemyFlag = 44;
        break;
    case 17:
        charaFlag = 175;
        enemyFlag = 61;
        break;
    case 18:
        charaFlag = 163;
        enemyFlag = 49;
        break;
    case 19:
        charaFlag = 164;
        enemyFlag = 50;
        break;
    case 20:
        charaFlag = 165;
        enemyFlag = 51;
        break;
    case 21:
        charaFlag = 168;
        enemyFlag = 54;
        break;
    case 22:
        charaFlag = 169;
        enemyFlag = 55;
        break;
    case 23:
        charaFlag = 170;
        enemyFlag = 56;
        break;
    case 24:
        charaFlag = 172;
        enemyFlag = 58;
        break;
    case 25:
        charaFlag = 173;
        enemyFlag = 59;
        break;
    case 26:
        charaFlag = 171;
        enemyFlag = 57;
        break;
    case 27:
        charaFlag = 174;
        enemyFlag = 60;
        break;
    case 28:
        charaFlag = 161;
        enemyFlag = 47;
        break;
    case 29:
        charaFlag = 177;
        enemyFlag = 63;
        break;
    case 30:
        charaFlag = 178;
        enemyFlag = 64;
        break;
    case 31:
        charaFlag = 176;
        enemyFlag = 62;
        break;
    case 100:
        charaFlag = 179;
        enemyFlag = 65;
        break;
    case 108:
        charaFlag = 180;
        enemyFlag = 66;
        break;
    case 103:
        charaFlag = 181;
        enemyFlag = 67;
        break;
    case 113:
        charaFlag = 182;
        enemyFlag = 68;
        break;
    }
    if (charaFlag >= 0) {
        setDebugNewFlag(inIsSet, charaFlag);
        if (enemyFlag >= 0) setDownEnemyNum(enemyFlag, inNum);
    }
}
void CJiminyMemoFlag::debugRikuEvent_GetCard(BOOL inIsSet, INT inKind) {
    INT cardFlag = -1;
    switch ((BYTE)inKind) {
    case 18:
        cardFlag = 335;
        break;
    case 42:
        cardFlag = 336;
        break;
    case 43:
        cardFlag = 337;
        break;
    case 56:
        cardFlag = 338;
        break;
    case 68:
        cardFlag = 340;
        break;
    case 75:
        cardFlag = 342;
        break;
    case 70:
        cardFlag = 341;
        break;
    case 79:
        cardFlag = 343;
        break;
    case 82:
        cardFlag = 345;
        break;
    case 87:
        cardFlag = 347;
        break;
    case 85:
        cardFlag = 346;
        break;
    case 81:
        cardFlag = 344;
        break;
    case 90:
        cardFlag = 348;
        break;
    case 89:
        cardFlag = 349;
        break;
    case 99:
        cardFlag = 350;
        break;
    case 104:
        cardFlag = 354;
        break;
    case 101:
        cardFlag = 352;
        break;
    case 105:
        cardFlag = 355;
        break;
    case 107:
        cardFlag = 357;
        break;
    case 106:
        cardFlag = 356;
        break;
    case 100:
        cardFlag = 351;
        break;
    case 108:
        cardFlag = 358;
        break;
    case 109:
        cardFlag = 359;
        break;
    case 102:
        cardFlag = 353;
        break;
    case 115:
        cardFlag = 360;
        break;
    case 117:
        cardFlag = 361;
        break;
    case 161:
        cardFlag = 362;
        break;
    case 162:
        cardFlag = 363;
        break;
    case 163:
        cardFlag = 364;
        break;
    case 164:
        cardFlag = 365;
        break;
    case 165:
        cardFlag = 366;
        break;
    case 166:
        cardFlag = 367;
        break;
    case 169:
        cardFlag = 368;
        break;
    case 170:
        cardFlag = 369;
        break;
    case 171:
        cardFlag = 370;
        break;
    case 172:
        cardFlag = 371;
        break;
    case 175:
        cardFlag = 372;
        break;
    case 176:
        cardFlag = 373;
        break;
    case 177:
        cardFlag = 374;
        break;
    case 178:
        cardFlag = 375;
        break;
    case 182:
        cardFlag = 376;
        break;
    case 183:
        cardFlag = 377;
        break;
    case 186:
        cardFlag = 378;
        break;
    case 187:
        cardFlag = 379;
        break;
    case 188:
        cardFlag = 380;
        break;
    case 185:
        cardFlag = 381;
        break;
    case 190:
        cardFlag = 382;
        break;
    case 191:
        cardFlag = 383;
        break;
    case 192:
        cardFlag = 384;
        break;
    case 193:
        cardFlag = 385;
        break;
    case 194:
        cardFlag = 386;
        break;
    case 195:
        cardFlag = 387;
        break;
    case 196:
        cardFlag = 388;
        break;
    case 197:
        cardFlag = 389;
        break;
    case 198:
        cardFlag = 390;
        break;
    case 200:
        cardFlag = 391;
        break;
    case 201:
        cardFlag = 392;
        break;
    case 202:
        cardFlag = 393;
        break;
    case 58:
    case 59:
    case 60:
    case 61:
    case 62:
    case 63:
    case 64:
    case 65:
        cardFlag = 339;
        break;
    }
    if (cardFlag >= 0) setDebugNewFlag(inIsSet, cardFlag);
}
void CJiminyMemoFlag::debugSoraEvent_All(INT inFlag) {
    memset(&getMCWork().ViewState[0], inFlag, 17);
    memset(&getMCWork().ViewState[183], inFlag, 152);
    memset(&getMCWork().ViewState[23], inFlag, 104);
    memset(&getMCWork().ViewState[394], inFlag, 6);
}

void CJiminyMemoFlag::debugRikuEvent_All(INT inFlag) {
    memset(&getMCWork().ViewState[17], inFlag, 6);
    memset(&getMCWork().ViewState[335], inFlag, 59);
    memset(&getMCWork().ViewState[127], inFlag, 56);
}
// SOURCE DATA: .data .rodata
