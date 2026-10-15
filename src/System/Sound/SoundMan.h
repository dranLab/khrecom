#ifndef SYSTEM_SOUND_SOUNDMAN_H
#define SYSTEM_SOUND_SOUNDMAN_H

#include "khtypes.h"
#include "System/Vector.h"

class CBuf;

class CSound {
public:
    static void coldReset();
    static void initialize();
    static void finalize();
    static void mainProc();
    static INT setupRsrcSND(const CHAR* lpszFile, const CBuf& inData, INT nArg);
    static void disposeRsrcSND(const CHAR* lpszFile, INT nWaveID);
    static void clearForBattleScene();
    static void clearForEventScene();
    static INT getPanAndVol(const FVECTOR& vPos, INT& nVol);
    static INT getPan(const FVECTOR& vPos);
    static void setSeRangeRatio(UINT nRate);
    static HANDLE playSE(INT nSeq, INT nVol, INT nPan);
    static HANDLE playSE(INT nSeq, const FVECTOR& vPos, INT nVol);
    static void stopSE(HANDLE nHandle, INT nSeq);
    static void stopAllSE();
    static void pauseAllSE();
    static void startAllPausedSE();
    static void setSEVolume(HANDLE nHandle, INT nSeq, INT nVol, INT nTime);
    static void fadeoutSE(HANDLE nHandle, INT nSeq, INT nTime);
    static void fadeoutAllSE(INT nTime, BOOL bOutConstEnvSE);
    static void fadeAllSE(INT nTime, INT nVol);
    static void muteAllSE(INT nTime);
    static void fadeoutAllVoice(INT nTime);
    static void setSEPan(HANDLE nHandle, INT nSeq, INT nPan, INT nTime);
    static void setSEPanAndVol(HANDLE nHandle, INT nSeq, INT nPan, INT nVol, INT nTime);
    static BOOL isSEPlaying(HANDLE nHandle, INT nSeq);
    static void playConstEnvSE(INT nSeq, INT nVol, INT nTime);
    static void stopConstEnvSE(INT nTime);
    static void invalidateSE(BOOL bInvalid);
    static CHAR* getCurrentBGMName();
    static HANDLE getCurrentBGMHandle();
    static INT getStateBGM(HANDLE hBGM);
    static HANDLE playBGM(const CHAR* pszFile, INT nVol, INT nFlag);
    static void stopBGM(HANDLE hBGM);
    static HANDLE fadeinBGM(const CHAR* pszFile, UINT nTime, INT nVol, INT nFlag);
    static void fadeoutBGM(HANDLE hBGM, UINT nTime);
    static void fadeoutAllBGM(UINT nTime);
    static HANDLE crossFadeinBGM(const CHAR* pszName, UINT nTime, INT nVol, INT nFlag);
    static void setBGMVolume(HANDLE hBGM, INT nVol, INT nTime);
    static void setBGMMasterVolume(INT nVol, INT nTime);
    static UINT getBgmID_withName(const CHAR* pszName);
    static HANDLE playVsb(const CHAR* pszVsb, INT nType, INT nSeq, INT nVol, INT nPan, INT nFileID);
    static HANDLE playVsb(INT nSeq, INT nVol, INT nPan);
    static void stopVsb(HANDLE nHandle);
    static void setVsbPanAndVol(HANDLE nHandle, INT nSeq, INT nPan, INT nVol);
    static BOOL isVsbPlaying(HANDLE nHandle, INT nSeq);
    static HANDLE playVoice(const CHAR* pszVoice, INT nVol, INT nPan);
    static HANDLE playVoiceEx(const CHAR* pszVoice, INT nVol, INT nPan);
    static void stopVoice(HANDLE hHandle);
    static BOOL isVoicePlaying();
    static void setVoiceVolume(HANDLE hHandle, INT nVol, INT nTime);
    static void fadeoutVoice(HANDLE hHandle, INT nTime);
    static void setVoicePan(HANDLE hHandle, INT nPan, INT nTime);
    static void setVoicePanAndVol(HANDLE hHandle, INT nPan, INT nVol, INT nTime);
    static INT getVoiceTime();
    static void setPlayMode(INT nMode);
    static void setReverbMode(INT nMode);
    static void setReverbAttribute(INT nCore, INT nMode, INT nDepth, INT nDelay, INT nFeedBack);
    static void setReverbDepth(INT nDepth);
    static void setReverbVolume(INT nCore, INT nVol, INT nTime);
    static void endScene();
    static void setWarningPause(BOOL bPause, BOOL bOnlyBGM);
};

#endif // SYSTEM_SOUND_SOUNDMAN_H
