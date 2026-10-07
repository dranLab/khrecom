#ifndef GAME_DEBUG_DEBUGWORK_H
#define GAME_DEBUG_DEBUGWORK_H

#include "khtypes.h"

class CNakCamera;

/* FVECTOR (0x10, 16-byte aligned union) belongs to the math headers; only its layout is
 * needed here. Its inline members are left out. */
#ifndef FVECTOR_DEFINED
#define FVECTOR_DEFINED
union FVECTOR {
    float v[4];
    struct {
        FLOAT x;
        FLOAT y;
        FLOAT z;
        FLOAT w;
    };
    u_long128 _q;
    unsigned long _ul[2];
    unsigned int _ui[4];
};
#endif

struct DEBUG_WORK_SYS {
    SBYTE m_iDispCounter;
    INT m_iDagameNumber;
    SBYTE m_iBGM;
    INT m_iEffect;
    SBYTE m_HeapDisp;
    SBYTE m_fgAfterImageTrg;
    SBYTE m_fgAfterImage;
    SBYTE m_MemoryDisp;
    INT m_MDispLine;
    INT m_iMapDisp;
    INT m_iSkyDisp;
    FLOAT m_fMapBright;
    SBYTE m_HeapDispDetail;
    SBYTE m_iAllMapAll;
    SBYTE m_FontTest;
    SBYTE m_iDeFocus;
    SBYTE m_iDeFocusCol;
    SBYTE m_iBlurKind;
    SBYTE m_iBlurRate;
    SBYTE m_iBlurCh;
    FLOAT m_fDeFocusNear;
    FLOAT m_fDeFocusFar;
    FLOAT m_fBlurLen;
    SHORT m_iBlurCR;
    SHORT m_iBlurCG;
    SHORT m_iBlurCB;
    SHORT m_iBlurCA;
    INT m_iBlurAlpha;
    FLOAT m_fGlareMaxPer;
    SBYTE m_iFps;
    SBYTE m_iGlare;
    SBYTE m_iSnapShot;
    SBYTE m_iReportMemory;
    SBYTE m_iMapBack;
    SBYTE m_iEffectBBOX;
    SBYTE m_iEffectNoEntry;
    SBYTE m_iStopEffect;
    SHORT m_iDmgDisp;
    SHORT m_iFukidasiDisp;
    INT m_iRobotPadMode;
    int m_iRobotPadTrg[24];
    int m_iRobotPadPress[24];
};

struct DEBUG_WORK_NAKAMICHI {
    FVECTOR vCamPos;
    FVECTOR vCamLook;
    FLOAT fCamDist;
    SHORT iTest;
    SBYTE iCutType;
    SBYTE iBurnOrAlphaFlg;
    FLOAT fBurnTime;
    SHORT iBurnPar;
    SHORT iBurnNormPar;
    FLOAT fShiftLen;
    FLOAT fShiftTime;
    FLOAT fCutWait0;
    FLOAT fCutWait1;
    FLOAT fCutWait2;
    FLOAT fCutWait3;
    FLOAT fCutWait4;
    FLOAT fCutWait5;
    FLOAT fBombDelay0;
    FLOAT fBombDelay1;
    FLOAT fFallTimeOut;
    FLOAT fFlickTimeOut;
    CNakCamera *m_pDCam;
    SHORT iVramDisp;
    SHORT iVramDy;
    SHORT iVramDx;
    SBYTE iCutBox;
    SBYTE iMonType;
    SBYTE iCamDisp;
    SBYTE iCamSpeed;
    FLOAT fCamVAngle;
    FLOAT fCamRotZ;
    char szNameBuf[16];
};

struct DEBUG_WORK_SOUND {
    INT iSE_Handle;
    INT iSE_Id;
    INT iSE_FileId;
    INT iSE_SeId;
    BOOL bVoice;
};

struct DEBUG_WORK_COLLISION {
    UINT uiFlag;
};

struct DEBUG_WORK_PLAYER {
    FVECTOR vPos;
    FVECTOR vCamEye;
    FVECTOR vCamAim;
    FVECTOR vTemp;
    FLOAT fCamDist;
    FLOAT fCamSize;
    void *pCamData;
    void *pAtkData;
    char *m_szString[4];
    char *m_szState[4];
    char *m_szCamera[5];
    char *m_szAnim[2];
    char *m_szBlock[17];
    short int iAtkPrm[16];
    SHORT iAttackID;
    SHORT iSkillLock;
    SHORT iCardLock;
    SHORT iHP;
    unsigned char iAtkFlag[8];
    SBYTE iPlayerInfo;
    SBYTE iPlayerState;
    SBYTE iPlayerSubState;
    SBYTE iMuteki;
    SBYTE iDamage0;
    SBYTE iDP;
    BYTE iEnemyCardLock;
    SBYTE iDuel;
    SBYTE iHP1;
    SBYTE iDP1;
    SBYTE dum2;
    SBYTE dum1;
    SBYTE iTrgAuto;
    SBYTE iBlockInfo;
    SBYTE iPluto;
    SBYTE iFriend;
    SBYTE iDuelTime;
    SBYTE iCamRikuShiftY;
    SBYTE iDbgWindowUpdate;
    SBYTE iDbgWindow;
    SBYTE iDeckDbgDisp;
    SBYTE iCardNumLock;
    SBYTE iUseDbgDeck;
    SBYTE iCamMode;
    SBYTE iCameraInfo;
    SBYTE iHiJump;
    SBYTE iGlide;
    SBYTE iGlideSP;
    SBYTE iTemp;
    SBYTE iRenda;
    SBYTE iCardSpend;
    SBYTE iAutoReload;
};

struct DEBUG_WORK_EVENT {
    FVECTOR vPos;
    FVECTOR vRot;
    SBYTE iAtelCoord;
    SBYTE iNpcCoordDisp;
    SBYTE iNpcID;
    SBYTE iEventline;
    INT *pEventlineClass;
    INT *pClass;
};

struct DEBUG_ATEL_WORK_FLAG {
    INT iWorldIntro;
    INT iTutorialEnd;
    INT iPossibleCard;
    INT iKH2CardGet;
    INT iPossibleFriend;
    INT iPoohMiniGameIntro;
    INT iPcStanderdSkill;
    INT iEnSoraEventStep;
    INT iEnRikuEventStep;
    INT iSoraJiminyStep;
    INT iRikuJiminyStep;
    INT iSoraMovie0;
    INT iSoraMovie1;
    INT iSoraMovie2;
    INT iRikuMovie0;
    INT iRikuMovie1;
    INT iRikuMovie2;
    INT iPoohHiScoreOwl;
    INT iPoohHiScoreRoo;
    INT iPoohHiScoreEeyore;
    INT iPoohHiScoreTigger;
    INT iPoohHiScoreRabbit;
    BYTE iPlayMode;
    BYTE iPlayDifficultMode;
    BYTE iGameKH2clear;
    BYTE iMahluciaKilled;
    BYTE iRikuClear;
    BYTE iSoraClear;
    BYTE iDistinctionClear;
    BYTE iSoundMode;
    BYTE iVibrat;
    BYTE iPoohGamePiglet;
    BYTE iPoohGameOwl;
    BYTE iPoohGameRoo;
    BYTE iPoohGameEeyore;
    BYTE iPoohGameTigger;
    BYTE iPoohGameRabbit;
    BYTE iPoohClearIntoroPiglet;
    BYTE iPoohClearIntoroOwl;
    BYTE iPoohClearIntoroRoo;
    BYTE iPoohClearIntoroEeyore;
    BYTE iPoohClearIntoroTigger;
    BYTE iPoohClearIntoroRabbit;
    BYTE iPoohClearIntoroResetPiglet;
    BYTE iPoohClearIntoroResetOwl;
    BYTE iPoohClearIntoroResetRoo;
    BYTE iPoohClearIntoroResetEeyore;
    BYTE iPoohClearIntoroResetTigger;
    BYTE iPoohClearIntoroResetRabbit;
    BYTE TreTr;
    BYTE TreAg;
    BYTE TreOl;
    BYTE TreWo;
    BYTE TreMo;
    BYTE TreHa;
    BYTE TreAt;
    BYTE TreNe;
    BYTE TreHo;
    BYTE TrePo;
    BYTE TreTw;
    BYTE TreDe;
    BYTE TreCa;
    INT BtlMemo;
    INT KillEnemyCount;
    INT CardBreakCount;
    INT KeysTresur;
    INT StockKind;
    INT StockCount;
    INT Hour;
    INT minit;
    INT SpeedCount;
    FLOAT DuelWin;
    INT DarkCount;
    INT iCockpitDisp;
    INT iBossEventStep;
    BYTE iKindMenu;
    BYTE iPoohMinigameState;
    BYTE iPoohMinigameKind;
    BYTE iPlayerDispOff;
    BYTE iBossKilled;
    BYTE iNotEventSkip;
    BYTE iIsAtelEvent;
    BYTE iNotPcControl;
    BYTE iNotPause;
    BYTE iIsTutorial;
    BYTE iNotRoomJump;
    BYTE iNotRoomCreation;
    BYTE iNotHartBtl;
    BYTE iIsBtlTutorial;
    BYTE iIsWorldIntro;
};

struct DEBUG_JIMINY_MEMO {
    INT Sora_World;
    INT Sora_Floor;
    INT Riku_World;
    INT Riku_Floor;
    INT Sora_Etc;
    INT Sora_Enemy;
    INT Sora_ENum;
    INT Riku_Enemy;
    INT Riku_ENum;
    INT Sora_Card;
    INT Riku_Card;
    INT Sora_Flag;
    INT Riku_Flag;
};

struct DEBUG_FOG_TEST {
    SHORT Near;
    SHORT Far;
    SHORT r;
    SHORT g;
    SHORT b;
    SHORT max;
    SHORT min;
    SHORT inc;
};

struct DEBUG_THROW_TEST {
    FLOAT DirPow;
    FLOAT MaxAngle;
    FLOAT MinAngle;
};

struct DEBUG_GIM_PRIZE_TEST {
    FLOAT Pow;
    FLOAT MinSpeed;
    INT MaxRotX;
    INT MinRotX;
    INT MaxRotY;
    INT MinRotY;
    INT Kind;
    INT Num;
    INT *pClass;
    SBYTE iDisp;
};

struct DEBUG_GIM_DROP_PROB {
    BYTE ProbCard;
    BYTE ProbSoraGBall;
    BYTE ProbSoraRBall;
    BYTE ProbSoraBarel;
    BYTE ProbRikuGball;
    BYTE ProbRikuBarel;
    BYTE ProbRandomCategory;
    BYTE ProbCategoryKind;
    BYTE ProbRandomGBall;
    BYTE GBallType;
    BYTE GBallNum;
    BYTE ProbRandomRBall;
    BYTE RBallType;
    BYTE RBallNum;
    BYTE ProbRandomCardPossible;
    char PossibleCardName[32];
    BYTE PossibleCheck;
    BYTE ProbRandomCard;
    char CardName[32];
};

struct DEBUG_BLUR_TEST {
    SHORT Xofs;
    SHORT Yofs;
    SHORT Alpha;
    SHORT dummy;
};

struct DEBUG_SHADOW_TEST1 {
    SBYTE isDistance;
    SBYTE isHeight;
    char iPad[2];
    FLOAT fDistance;
    FLOAT fHeight;
    FLOAT fDensityColor;
    FLOAT fDensityAlpha;
};

struct DEBUG_SHADOW_TEST {
    SBYTE iDispArea;
    SBYTE iDispAreaHalf;
    char iPad[2];
    FLOAT fValuePlus;
    DEBUG_SHADOW_TEST1 p;
    DEBUG_SHADOW_TEST1 b;
};

struct DEBUG_WORK_ENEMY {
    UINT uiFlag;
    SHORT sFormation;
    SHORT sActNo;
    INT iHpPercent;
    INT iCardNumber;
};

struct DEBUG_WORK_FONT {
    INT _nBG_R;
    INT _nBG_G;
    INT _nBG_B;
};

struct DEBUG_WORK_MINI100ACRE {
    UINT unWD_Flag;
    FLOAT fWD_Gravity;
    FLOAT fWD_Rate;
    FLOAT fWD_AddSpeed;
    FLOAT fWD_MaxSpeed;
    UINT unBL_Flag;
    FLOAT fBL_Gravity;
    FLOAT fBL_RateY;
    FLOAT fBL_AddSpeedY;
    FLOAT fBL_MaxSpeedY;
    FLOAT fBL_RateXZ;
    FLOAT fBL_AddSpeedXZ;
    FLOAT fBL_MaxSpeedXZ;
    UINT unTJ_Flag;
    UINT unTJ_Mugen;
    float fTJ_SoraGravity[2];
    float fTJ_SoraFirstSpeedXZ[2];
    float fTJ_SoraAccelSpeedXZ[2];
    float fTJ_TiggerGravity[2][2];
    float fTJ_TiggerFirstSpeedXZ[2][2];
    float fTJ_TiggerAccelSpeedXZ[2][2];
    FLOAT fTJ_TiggerDown;
    UINT unVP_Flag;
    INT nVP_Seq1aPopCount;
    INT nVP_Seq2PopCount;
    INT nVP_Seq1bPopCount;
    FLOAT fVP_BigPumpkinRngMax;
    UINT unBB_Flag;
    FLOAT fBB_PooMaxHP;
    FLOAT fBB_BeeAtkPoint;
    INT nBB_BeeNum1st;
    INT nBB_BeeNum2nd;
    FLOAT fBB_CureL1;
    FLOAT fBB_CureL2;
    FLOAT fBB_CureL3;
    INT nBB_CureInterval;
};

struct DEBUG_WORK_MENU {
    UINT unFlag;
    INT nSoraCardIdx;
    INT nSoraCardRank;
    INT nSoraCardPremium;
    INT nSoraCardNum;
    INT nRikuCardIdx;
    INT nRikuCardRank;
    INT nRikuCardNum;
    INT nExpLevel;
};

struct DEBUG_WORK {
    DEBUG_WORK_SYS sys;
    DEBUG_WORK_NAKAMICHI nakamich;
    DEBUG_WORK_SOUND sound;
    DEBUG_WORK_COLLISION collision;
    DEBUG_WORK_PLAYER player;
    DEBUG_WORK_EVENT event;
    DEBUG_ATEL_WORK_FLAG atelwork;
    DEBUG_FOG_TEST fog;
    DEBUG_BLUR_TEST blur;
    DEBUG_THROW_TEST gimthrow;
    DEBUG_GIM_DROP_PROB gimDropProb;
    DEBUG_GIM_PRIZE_TEST gimPrize;
    DEBUG_SHADOW_TEST shadow;
    DEBUG_JIMINY_MEMO jiminy;
    DEBUG_WORK_ENEMY enemy;
    DEBUG_WORK_FONT font;
    DEBUG_WORK_MINI100ACRE acre;
    DEBUG_WORK_MENU menu;
};

namespace DebugWork {
extern DEBUG_WORK g_dw;
void initialize();
}

#endif // GAME_DEBUG_DEBUGWORK_H
