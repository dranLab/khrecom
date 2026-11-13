#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/Sound/SoundMan.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Debug.h"
#include "System/Buf.h"
#include "System/Graphic/WorldEnv.h"
#include "System/FileIO.h"
#include <string.h>

extern "C" {
int atoi(const char*);
int sprintf(char*, const char*, ...);
float atan2f(float, float);
float cosf(float);
void TsSdInitDriver();
void TsSdCheckDriver();
int TsSdCheckDriverSetParam2();
void TsSdSetWd(int flag, u_int addr);
void TsSdSetWdClear(int wd_num);
void TsSdSetSeb(int flag, u_int addr);
void TsSdSetSebClear(int seb_num);
void TsSdSetRevDepth(int depth);
void TsSdSebPlaySI2(int seb_id, int num, int vol, int pan, int se_id);
void TsSdSebStopSI2(int seb_id, int num, int se_id);
void TsSdSebSetVPSI2(int seb_id, int num, int vol, int pan, int se_id);
void TsSdSebSetPanSI2(int seb_id, int num, int pan, int se_id);
void TsSdSebFadeVPSI2(int seb_id, int num, int vol, int pan, int step, int flag, int turn, int se_id);
void TsSdSebFadeVolSI2(int seb_id, int num, int vol, int step, int flag, int se_id);
int TsSdSebPlayCheckSI2(int seb_id, int num, int se_id);
void TsSdSebStartSI(int seb_id, int num);
void TsSdSebPauseSI(int seb_id, int num);
void TsSdSebFadeVolSI(int seb_id, int num, int vol, int step, int flag);
void TsSdSebPlaySN(int seb_num, int num, int vol, int pan, int se_id);
void TsSdSebStopSN(int seb_num, int num, int se_id);
void TsSdSebSetVPSN(int seb_num, int num, int vol, int pan, int se_id);
void TsSdSebFadeVPSN(int seb_num, int num, int vol, int pan, int step, int flag, int turn, int se_id);
void TsSdSebFadeVolSN(int seb_num, int num, int vol, int step, int flag, int se_id);
int TsSdSebPlayCheckSN(int seb_num, int num, int se_id);
void TsSdSeStopAll();
void TsSdSetMasterVol2_StrMusic(int mvol);
void TsSdSetModeOutput(int type);
void TsSdStrFadeVol(int type, int id, int vol, int step, int flag);
int TsSdStrGetStatus(int type, int id);
void TsSdStrPause(int type, int id);
void TsSdStrPlayMusic(int sect, int play_sect, char* filename, int id);
void TsSdStrPlayMusicCrossFade(int sect, int play_sect, char* filename, int id, u_int addr, int read_sect, int fade_step);
void TsSdStrPlayVoice(u_int addr);
int TsSdStrPlayVoiceCheck(int addr);
void TsSdStrStart(int type, int id);
void TsSdStrStop(int type, int id);
void TsSdStrVoiceSetPan(u_int addr, int pan);
void TsSdStrVolSet2(int type, int id, int vol);
void TsSdVsbPlay(u_int addr, int vsb_id, int num, int vol, int pan);
void TsSdVsbSetVP(int vsb_id, int num, int vol, int pan);
void TsSdVsbStop(int vsb_id);
}

void SubVector(FVECTOR& outV, const FVECTOR& inV0, const FVECTOR& inV1);
FLOAT InnerProduct(const FVECTOR& inV0, const FVECTOR& inV1);


#define ASSERT(e) KH_DEBUG(if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0))

#define SEQ_VSB 0x10000000
#define SEQ_ID(n) (((n) & 0x0FFFFFFF) % 1000)
#define SEB_ID(n) (((n) & 0x0FFFFFFF) / 1000)
#define IS_MONSTER(n) ((n) >= 200 && (n) < 600)

extern "C" const char _ctype_[];
#define ISUPPER(c) ((_ctype_ + 1)[c] & 1)











































#include "System/RsrcDB.h"





class CSoundAgent {
private:
    HANDLE m_hHandle;
    SHORT m_nVolume;
    USHORT m_nID;
    SHORT m_nStep;
    SHORT m_nFlag;
    TIMEQ m_nTimer;
    CHAR m_szName[24];

public:
    CSoundAgent(USHORT nID) : m_hHandle(-1), m_nID(nID) {
        m_nVolume = 0;
        m_nStep = 0;
        m_nFlag = 0;
    }
    virtual ~CSoundAgent() {}
    virtual void mainProc(TIMEQ nTQ) = 0;
    virtual HANDLE play(const CHAR* pszName) { return 0; }
    virtual void stop(BOOL bDummy) = 0;
    virtual void fadeout(UINT nTime, BOOL bDummy) = 0;
    virtual void setVolume(INT nVol, INT nTime) = 0;
    HANDLE getHandle() { return m_hHandle; }
    BOOL isPlaying() { return getFlag(1); }
    CHAR* getName() { return m_szName; }
    BOOL isMatchName(const CHAR* pszName) {
        const CHAR* p = pszName;
        int c1;
        int c0;
        int i;
        if (!p) {
            return FALSE;
        }
        for (i = 0; (c0 = *p) != '\0' && i < 23 && c0 != '.';) {
            c1 = m_szName[i];
            if (ISUPPER(c0)) {
                c0 += 0x20;
            }
            if (ISUPPER(c1)) {
                c1 += 0x20;
            }
            p++;
            i++;
            if (c0 != c1) {
                return FALSE;
            }
        }
        c1 = m_szName[i];
        return c1 == '\0' || c1 == '.';
    }

protected:
    USHORT getID() { return m_nID; }
    void changeStep(SHORT nStep) { m_nStep = nStep; }
    SHORT getStep() const { return m_nStep; }
    virtual HANDLE generateHandle() { return 0; }
    void setHandle(HANDLE h) { m_hHandle = h; }
    void clearHandle() { m_hHandle = 0; }
    void setName(const CHAR* pszName) {
        const CHAR* p = pszName;
        int i;
        clearName();
        for (i = 0; p && *p && i < 23; i++, p++) {
            m_szName[i] = *p;
        }
        m_szName[i] = 0;
    }
    void clearName() { m_szName[0] = 0; }
    void setFlag(SHORT nFlag) { m_nFlag |= nFlag; }
    BOOL getFlag(SHORT nFlag) const { return (SHORT)(m_nFlag & nFlag); }
    void clearFlag() { m_nFlag = 0; }
    void setTimer(TIMEQ n) { m_nTimer = n; }
    TIMEQ getTimer() { return m_nTimer; }
    BOOL decTimer(TIMEQ nTQ) {
        TIMEQ n = m_nTimer - nTQ;
        m_nTimer = n;
        if (n > 0) {
            return FALSE;
        }
        m_nTimer = 0;
        return TRUE;
    }
    void setVolumeVal(INT nVol) { m_nVolume = nVol; }
    INT getVolumeVal() { return m_nVolume; }
};

class CBGMAgent : public CSoundAgent {
private:
    UINT m_nFadeinTimer;

public:
    CBGMAgent(USHORT nID);
    virtual ~CBGMAgent();
    virtual void mainProc(TIMEQ nTQ);
    virtual INT getState() const;
    virtual HANDLE play(const CHAR* pszName, INT nVol, SHORT nFlag);
    virtual HANDLE play(const CHAR* pszName) { return play(pszName, -1, 0); }
    virtual BOOL start() { return 0; }
    virtual void stop(BOOL bDummy);
    virtual HANDLE fadein(const CHAR* pszName, UINT nTime, INT nVol, SHORT nFlag);
    virtual void fadeout(UINT nTime, BOOL bDummy);
    virtual void setVolume(INT nVol, INT nTime);
    void reset();

protected:
    void setFadeinTimer(UINT n) { m_nFadeinTimer = n; }
    UINT getFadeinTimer() const { return m_nFadeinTimer; }
};

class CBGMAgentStream : public CBGMAgent {
public:
    CBGMAgentStream(USHORT nID);
    virtual ~CBGMAgentStream();
    virtual void mainProc(TIMEQ nTQ);
    virtual INT getState() const;
    virtual HANDLE play(const CHAR* pszName, INT nVol, SHORT nFlag);
    virtual BOOL start();
    virtual void stop(BOOL bDummy);
    virtual HANDLE fadein(const CHAR* pszName, UINT nTime, INT nVol, SHORT nFlag);
    virtual void fadeout(UINT nTime, BOOL bDummy);
    virtual void setVolume(INT nVol, INT nTime);

protected:
    void cancel();
};

class CVoiceAgent : public CSoundAgent {
private:
    INT m_nPlayID;
    INT m_nPan;
    INT m_nTime;
    INT m_nAddr;

public:
    CVoiceAgent(USHORT nID);
    virtual ~CVoiceAgent();
    virtual void mainProc(TIMEQ nTQ);
    HANDLE play(const CHAR* pszVoice, INT nVol, INT nPan);
    virtual HANDLE play(const CHAR* pszVoice) { return play(pszVoice, -1, 0x40); }
    HANDLE playEx(const CHAR* pszVoice, INT nVol, INT nPan);
    virtual void stop(BOOL bDummy);
    virtual void fadeout(UINT nTime, BOOL bDummy);
    virtual void setVolume(INT nVol, INT nTime);
    void setPan(INT nPan, INT nTime);
    void setPanAndVol(INT nPan, INT nVol, INT nTime);
    void reset();
    INT getPlayTime() const { return m_nTime; }

protected:
    virtual HANDLE generateHandle();
};

class CMasterVolAgent {
private:
    INT m_nVolume;
    INT m_nVolumeDelta;
    INT m_nTargetVolume;
    TIMEQ m_nTimer;

public:
    CMasterVolAgent();
    ~CMasterVolAgent();
    void mainProc(TIMEQ nTQ);
    void setVolume(INT nVol, INT nTime);
};

namespace {
unsigned char s_pCrossBuffer[196608];
BOOL s_bCrossPlaying;
HANDLE s_hVoiceHandleGenerator;
CBGMAgent* s_pBGMAgents[2];
CVoiceAgent* s_pVoiceAgent;
CMasterVolAgent* s_pBGMMasterVolAgent;
HANDLE s_hCurrentBGMHandle;
HANDLE s_hCurrentVoiceHandle;
HANDLE s_hSeHandleGenerator;
HANDLE s_hVsbHandleGenerator;
char s_szCurrentBGM[24];
char s_szCurrentVoice[24];
HANDLE s_hCurrentConstEnvSE;
INT s_nCurrentConstEnvSEID;
FLOAT s_fSEAreaMin2;
FLOAT s_fSEAreaMax2;
FLOAT s_fSEAreaRate;
UINT s_nFlagSE;
CBuf* s_pTestBuf;
/* The original's .bss ends with 0x8C bytes that nothing references (0x80 after alignment): an unused object whose
 * name is not known. */
char s_aUnused[0x80];
}

static void _setBGMMasterVolume(INT nVol);

static inline void _setFlag(UINT& rFlag, INT nFlag, BOOL bOn) {
    if (bOn) {
        rFlag |= nFlag;
    } else {
        rFlag &= ~nFlag;
    }
}

void CSound::coldReset() {
    TsSdInitDriver();
}

void CSound::initialize() {
    INT i;
    s_bCrossPlaying = FALSE;
    s_hVoiceHandleGenerator = 0;
    s_hCurrentBGMHandle = 0;
    s_szCurrentBGM[0] = 0;
    s_hCurrentVoiceHandle = 0;
    s_szCurrentVoice[0] = 0;
    s_hSeHandleGenerator = 0;
    s_hVsbHandleGenerator = 0;
    s_hCurrentConstEnvSE = 0;
    s_nCurrentConstEnvSEID = 0;
    s_nFlagSE = 0;
    setPlayMode(0);
    setSeRangeRatio(100);
    _setBGMMasterVolume(128);
    for (i = 0; i < 2; i++) {
#line 408
        s_pBGMAgents[i] = new KH_AT("(408)") CBGMAgentStream(i);
        ASSERT(s_pBGMAgents[i]);
    }
#line 417
    s_pVoiceAgent = new KH_AT("(417)") CVoiceAgent(i);
    ASSERT(s_pVoiceAgent);
#line 424
    s_pBGMMasterVolAgent = new KH_AT("(424)") CMasterVolAgent;
    ASSERT(s_pBGMMasterVolAgent);
}

void CSound::finalize() {
    INT i;
    stopBGM(0);
    stopAllSE();
    stopVoice(0);
    for (i = 0; i < 2; i++) {
        if (s_pBGMAgents[i]) {
            delete s_pBGMAgents[i];
        }
    }
    if (s_pVoiceAgent) {
        delete s_pVoiceAgent;
    }
    if (s_pBGMMasterVolAgent) {
        delete s_pBGMMasterVolAgent;
    }
    clearForBattleScene();
    TsSdCheckDriverSetParam2();
}

static INT _getWaveFileID(const CHAR* lpszFile) {
    char szWD[10];
    strncpy(szWD, lpszFile, 8);
    szWD[8] = 0;
    return atoi(szWD + 4);
}

static BOOL _isMonsterWave(INT nFileID) {
    return IS_MONSTER(nFileID);
}

static INT _setupWave(const CHAR* lpszFile, const CBuf& inData, INT nArg) {
    INT nFileID = _getWaveFileID(lpszFile);
    if (nArg && !_isMonsterWave(nFileID) && nFileID != 4) {
        return 0;
    }
    TsSdSetWd(0, (u_int)inData.getBufPtr());
    return nFileID;
}

static HANDLE _generateSeHandle() {
    s_hSeHandleGenerator = (s_hSeHandleGenerator + 1) & 0xFF;
    if (s_hSeHandleGenerator == 0) {
        s_hSeHandleGenerator = 1;
    }
    return s_hSeHandleGenerator;
}

static INT _getSebFileID(const CHAR* pszFile) {
    char szSeb[8];
    strncpy(szSeb, pszFile, 6);
    szSeb[6] = 0;
    return atoi(szSeb + 2);
}

static INT _getSebID(INT nID) {
    INT nSebID = SEB_ID(nID);
    INT nRet = -1;
    do {
        if (IS_MONSTER(nSebID)) {
            nRet = nSebID;
            break;
        }
        if (nSebID >= 600 && nSebID < 800) {
        switch (nSebID % 5) {
        case 2:
            nRet = 13;
            break;
        case 3:
            nRet = 14;
            break;
        case 4:
            nRet = 15;
            break;
        default:
            nRet = 12;
            break;
        }
            break;
        }
        if (nSebID == 0) {
            nRet = 0;
            break;
        }
        if (nSebID == 1) {
            nRet = 1;
            break;
        }
        if (nSebID == 2) {
            nRet = 3;
            break;
        }
        if (nSebID == 3) {
            nRet = 3;
            break;
        }
        if (nSebID >= 100 && nSebID < 200) {
            nRet = 2;
            break;
        }
        if (!(nSebID >= 800 && nSebID < 1000)) {
            if (nSebID >= 1000 && nSebID < 8000) {
                nRet = 4;
            }
            break;
        }
        nRet = 4;
    } while (0);
    return nRet;
}

static void _setupSEB(const CHAR* pszFile, const CBuf& inData, INT nArg) {
    if (nArg) {
        INT nID = _getSebFileID(pszFile);
        if (!IS_MONSTER(nID)) {
            return;
        }
    }
    TsSdSetSeb(0, (u_int)inData.getBufPtr());
}

INT CSound::setupRsrcSND(const CHAR* lpszFile, const CBuf& inData, INT nArg) {
    const CHAR* lpszExp = strchr(lpszFile, '.');
    if (lpszExp) {
        if (strncmp(lpszExp, ".wd", 3) == 0) {
            return _setupWave(lpszFile, inData, nArg);
        }
        if (strncmp(lpszExp, ".seb", 4) == 0) {
            _setupSEB(lpszFile, inData, nArg);
        }
    }
    return -1;
}

static INT _getSEFileID(const CHAR* lpszFile) {
    char szSEB[10];
    CHAR* p;
    strncpy(szSEB, lpszFile, 8);
    szSEB[8] = 0;
    p = strchr(szSEB, '.');
    if (p) {
        *p = 0;
    }
    return atoi(szSEB + 2);
}

static void _removeSEB(const CHAR* lpszFile) {
    TsSdSetSebClear(_getSEFileID(lpszFile));
}

static void _clearWave(INT nWaveID) {
    TsSdSetWdClear(nWaveID);
}

void CSound::disposeRsrcSND(const CHAR* lpszFile, INT nWaveID) {
    const CHAR* lpszExp = strchr(lpszFile, '.');
    if (lpszExp) {
        if (strncmp(lpszExp, ".seb", 4) == 0) {
            _removeSEB(lpszFile);
        } else if (strncmp(lpszExp, ".wd", 3) == 0) {
            _clearWave(nWaveID);
        }
    }
}

void CSound::clearForBattleScene() {
    TsSdSetWdClear(0x1000001);
    TsSdSetSebClear(0x1000001);
}

void CSound::clearForEventScene() {
    TsSdSetWdClear(0x1000002);
}

void CSound::setSeRangeRatio(UINT nRate) {
    FLOAT fRate = nRate;
    FLOAT f = fRate * 0.04f;
    s_fSEAreaMin2 = f * f;
    f = fRate * 0.2f;
    s_fSEAreaMax2 = f * f;
    s_fSEAreaRate = 1.0f / (s_fSEAreaMax2 - s_fSEAreaMin2);
}

static void _clearCurrentVoiceInfo() {
    s_hCurrentVoiceHandle = 0;
    s_szCurrentVoice[0] = 0;
}

void CSound::mainProc() {
    TIMEQ nTQ = CMainApp::getDispFrameRate();
    INT i;
    for (i = 0; i < 2; i++) {
        CBGMAgent* pAgent = s_pBGMAgents[i];
        if (pAgent) {
            pAgent->mainProc(nTQ);
        }
    }
    if (s_pVoiceAgent) {
        s_pVoiceAgent->mainProc(nTQ);
        if (s_hCurrentVoiceHandle && !s_pVoiceAgent->isPlaying()) {
            _clearCurrentVoiceInfo();
        }
    }
    if (s_pBGMMasterVolAgent) {
        s_pBGMMasterVolAgent->mainProc(nTQ);
    }
    TsSdCheckDriver();
}

static INT _getPanSub(const FVECTOR& vPos) {
    FLOAT fY = atan2f(vPos.x, vPos.z);
    const FVECTOR& vView = CWorldEnv::getViewVector();
    FLOAT r = fY - atan2f(vView.x, vView.z);
    FLOAT fCos;
    FLOAT fPos;
    INT nPan;
    if (r < -3.1415927f) {
        r += 6.2831855f;
    } else if (r > 3.1415927f) {
        r -= 6.2831855f;
    }
    fY = r;
    fCos = cosf(r);
    fPos = fCos * 64.0f;
    if (fY < 0.0f) {
        nPan = (INT)(128.0f - fPos);
    } else if (fCos < 0.0f) {
        nPan = (INT)(fPos + 256.0f);
    } else {
        nPan = (INT)fPos;
    }
    return nPan;
}

INT CSound::getPanAndVol(const FVECTOR& vPos, INT& nVol) {
    FVECTOR v;
    FLOAT fDist2;
    if (!CWorldEnv::isReadyMike()) {
        return -1;
    }
    SubVector(v, vPos, CWorldEnv::getMikePos());
    fDist2 = InnerProduct(v, v);
    if (fDist2 > s_fSEAreaMax2) {
        return -1;
    }
    if ((UINT)nVol > 127) {
        nVol = 127;
    }
    if (fDist2 > s_fSEAreaMin2) {
        FLOAT fRate = 1.0f - (fDist2 - s_fSEAreaMin2) * s_fSEAreaRate;
        nVol = (INT)(nVol * fRate);
    }
    return _getPanSub(v);
}

INT CSound::getPan(const FVECTOR& vPos) {
    FVECTOR v;
    if (!CWorldEnv::isReadyMike()) {
        return 0x40;
    }
    SubVector(v, vPos, CWorldEnv::getMikePos());
    return _getPanSub(v);
}

HANDLE CSound::playSE(INT nSeq, INT nVol, INT nPan) {
    INT nSebID;
    HANDLE h;
    INT nSeID;
    if (s_nFlagSE & 1) {
        return 0;
    }
    if (nPan < 0) {
        return 0;
    }
    nSebID = _getSebID(nSeq);
    if (nSebID < 0) {
        return 0;
    }
    if (nSeq & SEQ_VSB) {
        return playVsb(nSeq, nVol, nPan);
    }
    nSeID = SEQ_ID(nSeq);
    h = _generateSeHandle();
    if (nVol < 0) {
        nVol = 127;
    }
    if (IS_MONSTER(nSebID)) {
        TsSdSebPlaySN(nSebID, nSeID, nVol, nPan, h);
    } else {
        TsSdSebPlaySI2(nSebID, nSeID, nVol, nPan, h);
    }
    return h;
}

HANDLE CSound::playSE(INT nSeq, const FVECTOR& vPos, INT nVol) {
    INT nPan;
    if (s_nFlagSE & 1) {
        return 0;
    }
    nPan = getPanAndVol(vPos, nVol);
    if (nPan >= 0) {
        return playSE(nSeq, nVol, nPan);
    }
    return -1;
}

void CSound::stopSE(HANDLE nHandle, INT nSeq) {
    INT nSebID;
    INT nSeID;
    if (nSeq & SEQ_VSB) {
        stopVsb(nHandle);
        return;
    }
    nSebID = _getSebID(nSeq);
    if (nSebID >= 0) {
        nSeID = SEQ_ID(nSeq);
        if (IS_MONSTER(nSebID)) {
            TsSdSebStopSN(nSebID, nSeID, nHandle);
        } else {
            TsSdSebStopSI2(nSebID, nSeID, nHandle);
        }
    }
}

void CSound::stopAllSE() {
    TsSdSeStopAll();
    TsSdStrStop(2, 0);
    s_hCurrentConstEnvSE = 0;
    s_nCurrentConstEnvSEID = 0;
}

void CSound::pauseAllSE() {
    TRACE_USER(TGOTO, "CSound::pauseAllSE()\n");
    TsSdSebPauseSI(0, 0);
    TsSdStrPause(2, 0);
    s_nFlagSE |= 2;
}

void CSound::startAllPausedSE() {
    TRACE_USER(TGOTO, "CSound::startAllPausedSE()\n");
    TsSdSebStartSI(0, 0);
    TsSdStrStart(2, 0);
    s_nFlagSE &= ~2;
}

static void _setSEMasterVolume(INT nVol) {
}

void CSound::setSEVolume(HANDLE nHandle, INT nSeq, INT nVol, INT nTime) {
    INT nSebID;
    INT nSeID;
    if (nSeq & SEQ_VSB) {
        setVsbPanAndVol(nHandle, SEQ_ID(nSeq), 0x40, nVol);
        return;
    }
    nSebID = _getSebID(nSeq);
    if (nSebID >= 0) {
        nSeID = SEQ_ID(nSeq);
        if (IS_MONSTER(nSebID)) {
            TsSdSebFadeVolSN(nSebID, nSeID, nVol, nTime, 0, nHandle);
        } else {
            TsSdSebFadeVolSI2(nSebID, nSeID, nVol, nTime, 0, nHandle);
        }
    }
}

void CSound::fadeoutSE(HANDLE nHandle, INT nSeq, INT nTime) {
    INT nSebID;
    INT nSeID;
    if (nTime <= 0) {
        stopSE(nHandle, nSeq);
        return;
    }
    nSebID = _getSebID(nSeq);
    if (nSebID >= 0) {
        nSeID = SEQ_ID(nSeq);
        if (IS_MONSTER(nSebID)) {
            TsSdSebFadeVolSN(nSebID, nSeID, 0, nTime, 1, nHandle);
        } else {
            TsSdSebFadeVolSI2(nSebID, nSeID, 0, nTime, 1, nHandle);
        }
    }
}

void CSound::fadeoutAllSE(INT nTime, BOOL bOutConstEnvSE) {
    if (nTime <= 0) {
        stopAllSE();
    }
    TsSdSebFadeVolSI(0, -1, 0, nTime, 1);
    if (bOutConstEnvSE) {
        stopConstEnvSE(nTime);
    } else if (s_hCurrentConstEnvSE) {
        setSEVolume(s_hCurrentConstEnvSE, s_nCurrentConstEnvSEID, 127, 1);
    }
    CWorldEnv::readyMike(0, 0);
}

void CSound::fadeoutAllVoice(INT nTime) {
    if (nTime <= 0) {
        TsSdStrStop(2, 0);
    }
    TsSdStrFadeVol(2, 0, 0, nTime, 1);
}

void CSound::fadeAllSE(INT nTime, INT nVol) {
    if ((UINT)nVol > 127) {
        nVol = 127;
    }
    TsSdSebFadeVolSI(0, 0, nVol, nTime, 0);
    TsSdStrFadeVol(2, 0, nVol, nTime, 0);
    if (s_hCurrentConstEnvSE) {
        setSEVolume(s_hCurrentConstEnvSE, s_nCurrentConstEnvSEID, nVol, nTime);
    }
}

void CSound::muteAllSE(INT nTime) {
    fadeAllSE(nTime, 0);
}

void CSound::setSEPan(HANDLE nHandle, INT nSeq, INT nPan, INT nTime) {
    INT nSebID;
    INT nSeID;
    if (nSeq & SEQ_VSB) {
        setVsbPanAndVol(nHandle, SEQ_ID(nSeq), nPan, -1);
        return;
    }
    nSebID = _getSebID(nSeq);
    if (nSebID >= 0) {
        nSeID = SEQ_ID(nSeq);
        if (!IS_MONSTER(nSebID)) {
            TsSdSebSetPanSI2(nSebID, nSeID, nPan, nHandle);
        }
    }
}

void CSound::setSEPanAndVol(HANDLE nHandle, INT nSeq, INT nPan, INT nVol, INT nTime) {
    INT nSebID;
    INT nSeID;
    if (!CWorldEnv::isReadyMike()) {
        return;
    }
    if (nSeq & SEQ_VSB) {
        setVsbPanAndVol(nHandle, SEQ_ID(nSeq), nPan, nVol);
        return;
    }
    nSebID = _getSebID(nSeq);
    if (nSebID < 0) {
        return;
    }
    nSeID = SEQ_ID(nSeq);
    if (nPan < 0) {
        nPan = 0x40;
        nVol = 0;
    } else if ((UINT)nVol > 127) {
        nVol = 127;
    }
    if (nTime < 2) {
        if (IS_MONSTER(nSebID)) {
            TsSdSebSetVPSN(nSebID, nSeID, nVol, nPan, nHandle);
        } else {
            TsSdSebSetVPSI2(nSebID, nSeID, nVol, nPan, nHandle);
        }
    } else {
        if (IS_MONSTER(nSebID)) {
            TsSdSebFadeVPSN(nSebID, nSeID, nVol, nPan, nTime, 0, 1, nHandle);
        } else {
            TsSdSebFadeVPSI2(nSebID, nSeID, nVol, nPan, nTime, 0, 1, nHandle);
        }
    }
}

BOOL CSound::isSEPlaying(HANDLE nHandle, INT nSeq) {
    INT nSebID;
    INT nSeID;
    if (nSeq & SEQ_VSB) {
        return isVsbPlaying(nHandle, SEQ_ID(nSeq));
    }
    if (nHandle < 0) {
        return FALSE;
    }
    nSebID = _getSebID(nSeq);
    if (nSebID < 0) {
        return FALSE;
    }
    nSeID = SEQ_ID(nSeq);
    if (IS_MONSTER(nSebID)) {
        return TsSdSebPlayCheckSN(nSebID, nSeID, nHandle) == 1;
    }
    return TsSdSebPlayCheckSI2(nSebID, nSeID, nHandle) == 1;
}

void CSound::playConstEnvSE(INT nSeq, INT nVol, INT nTime) {
    if (nSeq != s_nCurrentConstEnvSEID) {
        stopConstEnvSE(nTime);
        s_hCurrentConstEnvSE = playSE(nSeq, 1, 0x40);
        if (s_hCurrentConstEnvSE) {
            if ((UINT)nVol > 127) {
                nVol = 127;
            }
            s_nCurrentConstEnvSEID = nSeq;
            setSEVolume(s_hCurrentConstEnvSE, nSeq, nVol, nTime);
        }
    }
}

void CSound::stopConstEnvSE(INT nTime) {
    if (s_hCurrentConstEnvSE) {
        if (nTime) {
            fadeoutSE(s_hCurrentConstEnvSE, s_nCurrentConstEnvSEID, (nTime < 0) ? 1 : nTime);
        } else {
            stopSE(s_hCurrentConstEnvSE, s_nCurrentConstEnvSEID);
        }
        s_hCurrentConstEnvSE = 0;
        s_nCurrentConstEnvSEID = 0;
    }
}

void CSound::invalidateSE(BOOL bInvalid) {
    _setFlag(s_nFlagSE, 1, bInvalid);
    if (!bInvalid) {
        CWorldEnv::readyMike(1, 0);
    }
}

static CBGMAgent* _getBGMAgent(const CHAR* pszName) {
    INT i;
    for (i = 0; i < 2; i++) {
        CBGMAgent* p = s_pBGMAgents[i];
        if (p && p->isMatchName(pszName)) {
            return p;
        }
    }
    return NULL;
}

static CBGMAgent* _getBGMAgent(HANDLE hHandle) {
    INT i;
    for (i = 0; i < 2; i++) {
        CBGMAgent* p = s_pBGMAgents[i];
        if (p && p->getHandle() == hHandle) {
            return p;
        }
    }
    return NULL;
}

CHAR* CSound::getCurrentBGMName() {
    return s_szCurrentBGM;
}

HANDLE CSound::getCurrentBGMHandle() {
    return s_hCurrentBGMHandle;
}

INT CSound::getStateBGM(HANDLE hBGM) {
    CBGMAgent* pAgent = _getBGMAgent(hBGM);
    if (pAgent) {
        return pAgent->getState();
    }
    return 0;
}

static CBGMAgent* _getFreeBGMAgentStream() {
    INT i;
    for (i = 0; i < 2; i++) {
        CBGMAgent* pAgent = s_pBGMAgents[i];
        if (pAgent && !pAgent->isPlaying()) {
            return pAgent;
        }
    }
    return NULL;
}

static void _clearCurrentBGMInfo() {
    s_hCurrentBGMHandle = 0;
    s_szCurrentBGM[0] = 0;
    s_bCrossPlaying = FALSE;
}

static HANDLE _playBGM(const CHAR* pszName, INT nVol, UINT nFadeinTime, INT nFlag) {
    BOOL bPlaying;
    HANDLE hBGM;
    CBGMAgent* pAgent;
    INT i;
    if (!pszName || !*pszName) {
        return 0;
    }
    bPlaying = FALSE;
    for (i = 0; i < 2; i++) {
        CBGMAgent* pAgent = s_pBGMAgents[i];
        if (pAgent && pAgent->isPlaying()) {
            if (pAgent->isMatchName(pszName)) {
                return 0;
            }
            bPlaying = TRUE;
        }
    }
    if (bPlaying && !(nFlag & 0x600)) {
        CSound::stopBGM(0);
    }
    hBGM = 0;
    pAgent = _getFreeBGMAgentStream();
    if (pAgent) {
        if (nFadeinTime) {
            hBGM = pAgent->fadein(pszName, nFadeinTime, nVol, nFlag);
        } else {
            hBGM = pAgent->play(pszName, nVol, nFlag);
        }
    }
    s_hCurrentBGMHandle = hBGM;
    if (hBGM) {
        strncpy(s_szCurrentBGM, pszName, 23);
        s_szCurrentBGM[23] = 0;
    } else {
        s_szCurrentBGM[0] = 0;
    }
    return hBGM;
}

HANDLE CSound::playBGM(const CHAR* pszFile, INT nVol, INT nFlag) {
    return _playBGM(pszFile, nVol, 0, nFlag);
}

void CSound::stopBGM(HANDLE hBGM) {
    INT i;
    if (hBGM == 0) {
        for (i = 0; i < 2; i++) {
            CBGMAgent* pAgent = s_pBGMAgents[i];
            if (pAgent) {
                pAgent->stop(TRUE);
            }
        }
        _clearCurrentBGMInfo();
        TsSdStrStop(1, 0);
    } else {
        CBGMAgent* pAgent = _getBGMAgent(hBGM);
        if (pAgent) {
            pAgent->stop(FALSE);
        }
        if (hBGM == s_hCurrentBGMHandle) {
            _clearCurrentBGMInfo();
        }
    }
}

void CSound::fadeoutBGM(HANDLE hBGM, UINT nTime) {
    INT i;
    if (hBGM == 0) {
        for (i = 0; i < 2; i++) {
            CBGMAgent* pAgent = s_pBGMAgents[i];
            if (pAgent) {
                pAgent->fadeout(nTime, TRUE);
            }
        }
        _clearCurrentBGMInfo();
        TsSdStrFadeVol(1, 0, 0, nTime, 1);
    } else {
        CBGMAgent* pAgent = _getBGMAgent(hBGM);
        if (pAgent) {
            pAgent->fadeout(nTime, FALSE);
        }
        if (hBGM == s_hCurrentBGMHandle) {
            _clearCurrentBGMInfo();
        }
    }
}

void CSound::fadeoutAllBGM(UINT nTime) {
    fadeoutBGM(0, nTime);
}

HANDLE CSound::fadeinBGM(const CHAR* pszFile, UINT nTime, INT nVol, INT nFlag) {
    return _playBGM(pszFile, nVol, nTime, nFlag);
}

HANDLE CSound::crossFadeinBGM(const CHAR* pszName, UINT nTime, INT nVol, INT nFlag) {
    HANDLE hBGM;
    CBGMAgent* pCurrentAgent;
    if (!pszName || !*pszName) {
        return 0;
    }
    if (s_hCurrentBGMHandle) {
        pCurrentAgent = _getBGMAgent(s_hCurrentBGMHandle);
        if (!pCurrentAgent) {
            return 0;
        }
        if (pCurrentAgent->isMatchName(pszName)) {
            return 0;
        }
        if (nTime > 120) {
            TRACE("BGM crossfade \202\314\216\236\212\324\216w\222\350\202\252\202Q\225b\202\360\222\264\202\246\202\304\202\242\202\351\202\314\202\305\202Q\225b\202\311\202\265\202\334\202\267\n");
            nTime = 120;
        }
        pCurrentAgent->fadeout(nTime, TRUE);
        hBGM = _playBGM(pszName, nVol, nTime, 0x400);
    } else {
        hBGM = _playBGM(pszName, nVol, nTime, 0);
    }
    return hBGM;
}

static void _setBGMMasterVolume(INT nVol) {
    if ((UINT)nVol > 128) {
        nVol = 128;
    }
    TsSdSetMasterVol2_StrMusic(nVol);
}

void CSound::setBGMMasterVolume(INT nVol, INT nTime) {
    if (s_pBGMMasterVolAgent) {
        if (nTime < 0) {
            nTime = 5;
        }
        s_pBGMMasterVolAgent->setVolume(nVol, nTime);
    } else {
        _setBGMMasterVolume(nVol);
    }
}

void CSound::setBGMVolume(HANDLE hBGM, INT nVol, INT nTime) {
    INT i;
    if (hBGM == 0) {
        for (i = 0; i < 2; i++) {
            CBGMAgent* pAgent = s_pBGMAgents[i];
            if (pAgent) {
                pAgent->setVolume(nVol, nTime);
            }
        }
    } else if (s_hCurrentBGMHandle && hBGM == s_hCurrentBGMHandle) {
        CBGMAgent* pAgent = _getBGMAgent(hBGM);
        if (pAgent) {
            pAgent->setVolume(nVol, nTime);
        }
    }
}

void CSound::setPlayMode(INT nMode) {
    switch (nMode) {
    case 1:
        nMode = 1;
        break;
    case 2:
        nMode = 0;
        break;
    default:
        nMode = 2;
        break;
    }
    TsSdSetModeOutput(nMode);
}

void CSound::setReverbMode(INT nMode) {
}

void CSound::setReverbAttribute(INT nCore, INT nMode, INT nDepth, INT nDelay, INT nFeedBack) {
}

void CSound::setReverbDepth(INT nDepth) {
    TsSdSetRevDepth(nDepth);
}

void CSound::setReverbVolume(INT nCore, INT nVol, INT nTime) {
}

void CSound::endScene() {
}

static HANDLE _generateVsbHandle(INT nVsbType) {
    HANDLE h = s_hVsbHandleGenerator++;
    return (nVsbType << 24) | (h & 0xFFFFFF);
}

static INT _getVsbID(INT nType) {
    INT nID;
    switch (nType) {
    case 0:
        nID = 0x10;
        break;
    case 1:
        nID = 0x20;
        break;
    case 2:
        nID = 0x40;
        break;
    default:
        nID = -1;
        break;
    }
    return nID;
}

HANDLE CSound::playVsb(const CHAR* pszVsb, INT nType, INT nSeq, INT nVol, INT nPan, INT nFileID) {
    INT nVsbID;
    BYTE* pData;
    CRsrcData* pRsrc;
    char szFile[8];
    if (nPan < 0) {
        return 0;
    }
    nVsbID = _getVsbID(nType);
    if (nVsbID < 0) {
        return 0;
    }
    pRsrc = CRsrcDB::getRsrc(0x2F, pszVsb, NAME_CASE_LOWER);
    pData = pRsrc ? (BYTE*)CRsrcDB::getRsrcData(*pRsrc) : NULL;
    if (!pData) {
        return 0;
    }
    if (nVol < 0) {
        nVol = 127;
    }
    TsSdVsbPlay((u_int)pData, nVsbID, nSeq, nVol, nPan);
    if (nFileID < 0) {
        strncpy(szFile, pszVsb, 6);
        szFile[6] = 0;
        nFileID = atoi(szFile + 2);
    }
    return _generateVsbHandle(nVsbID);
}

HANDLE CSound::playVsb(INT nSeq, INT nVol, INT nPan) {
    char szFile[32];
    INT n = nSeq & 0x0FFFFFFF;
    INT nFileID = n / 1000;
    INT nNum = n % 1000;
    INT nType = nFileID / 1000;
    sprintf(szFile, "vo%04d", nFileID);
    return playVsb(szFile, nType, nNum, nVol, nPan, nFileID);
}

void CSound::stopVsb(HANDLE nHandle) {
    TsSdVsbStop((UINT)nHandle >> 24);
}

void CSound::setVsbPanAndVol(HANDLE nHandle, INT nSeq, INT nPan, INT nVol) {
    INT nSebID = (UINT)nHandle >> 24;
    if (nPan < 0) {
        nPan = 0;
        nVol = 0;
    } else if ((UINT)nVol > 127) {
        nVol = 127;
    }
    TsSdVsbSetVP(nSebID, nSeq, nVol, nPan);
}

BOOL CSound::isVsbPlaying(HANDLE nHandle, INT nSeq) {
    INT nStat = TsSdStrGetStatus(2, ((nHandle >> 24) << 8) + nSeq + 1);
    return nStat >= 1 && nStat <= 5;
}

static CVoiceAgent* _getFreeVoiceAgent() {
    CVoiceAgent* pAgent = s_pVoiceAgent;
    if (pAgent) {
        if (pAgent->isPlaying()) {
            pAgent->stop(FALSE);
        }
        return pAgent;
    }
    return NULL;
}

HANDLE CSound::playVoice(const CHAR* pszVoice, INT nVol, INT nPan) {
    CVoiceAgent* pAgent;
    if (!pszVoice || nPan < 0) {
        return 0;
    }
    if (s_hCurrentVoiceHandle) {
        if (strcmp(pszVoice, s_szCurrentVoice) == 0) {
            return 0;
        }
        stopVoice(0);
    }
    pAgent = _getFreeVoiceAgent();
    if (pAgent) {
        s_hCurrentVoiceHandle = pAgent->play(pszVoice, nVol, nPan);
        if (s_hCurrentVoiceHandle) {
            strncpy(s_szCurrentVoice, pszVoice, 23);
            s_szCurrentVoice[23] = 0;
        }
    }
    return s_hCurrentVoiceHandle;
}

HANDLE CSound::playVoiceEx(const CHAR* pszVoice, INT nVol, INT nPan) {
    CVoiceAgent* pAgent;
    if (!pszVoice || nPan < 0) {
        return 0;
    }
    if (s_hCurrentVoiceHandle) {
        if (strcmp(s_szCurrentVoice, pszVoice) == 0) {
            return 0;
        }
        stopVoice(0);
    }
    pAgent = _getFreeVoiceAgent();
    if (pAgent) {
        s_hCurrentVoiceHandle = pAgent->playEx(pszVoice, nVol, nPan);
        if (s_hCurrentVoiceHandle) {
            strncpy(s_szCurrentVoice, pszVoice, 23);
            s_szCurrentVoice[23] = 0;
        }
    }
    return s_hCurrentVoiceHandle;
}

static CVoiceAgent* _getVoiceAgent(HANDLE hHandle) {
    if (hHandle) {
        CVoiceAgent* pAgent = s_pVoiceAgent;
        if (pAgent && pAgent->getHandle() == hHandle) {
            return pAgent;
        }
    }
    return NULL;
}

void CSound::stopVoice(HANDLE hHandle) {
    CVoiceAgent* pAgent;
    if (!hHandle) {
        hHandle = s_hCurrentVoiceHandle;
    }
    pAgent = _getVoiceAgent(hHandle);
    if (pAgent) {
        pAgent->stop(FALSE);
    }
    _clearCurrentVoiceInfo();
}

BOOL CSound::isVoicePlaying() {
    if (s_hCurrentVoiceHandle) {
        CVoiceAgent* pAgent = _getVoiceAgent(s_hCurrentVoiceHandle);
        if (pAgent) {
            return pAgent->isPlaying();
        }
    }
    return FALSE;
}

void CSound::setVoiceVolume(HANDLE hHandle, INT nVol, INT nTime) {
    CVoiceAgent* pAgent;
    if (!s_hCurrentVoiceHandle) {
        return;
    }
    if (!hHandle) {
        hHandle = s_hCurrentVoiceHandle;
    } else if (hHandle != s_hCurrentVoiceHandle) {
        return;
    }
    pAgent = _getVoiceAgent(hHandle);
    if (pAgent) {
        pAgent->setVolume(nVol, nTime);
    }
}

void CSound::fadeoutVoice(HANDLE hHandle, INT nTime) {
    CVoiceAgent* pAgent;
    if (!s_hCurrentVoiceHandle) {
        return;
    }
    if (!hHandle) {
        hHandle = s_hCurrentVoiceHandle;
    } else if (hHandle != s_hCurrentVoiceHandle) {
        return;
    }
    pAgent = _getVoiceAgent(hHandle);
    if (pAgent) {
        pAgent->fadeout(nTime, FALSE);
    }
    _clearCurrentVoiceInfo();
}

void CSound::setVoicePan(HANDLE hHandle, INT nPan, INT nTime) {
    CVoiceAgent* pAgent;
    if (!s_hCurrentVoiceHandle) {
        return;
    }
    if (!hHandle) {
        hHandle = s_hCurrentVoiceHandle;
    } else if (hHandle != s_hCurrentVoiceHandle) {
        return;
    }
    pAgent = _getVoiceAgent(hHandle);
    if (pAgent) {
        pAgent->setPan(nPan, nTime);
    }
}

void CSound::setVoicePanAndVol(HANDLE hHandle, INT nPan, INT nVol, INT nTime) {
    CVoiceAgent* pAgent;
    if (!s_hCurrentVoiceHandle) {
        return;
    }
    if (!hHandle) {
        hHandle = s_hCurrentVoiceHandle;
    } else if (hHandle != s_hCurrentVoiceHandle) {
        return;
    }
    pAgent = _getVoiceAgent(hHandle);
    if (pAgent) {
        pAgent->setPanAndVol(nPan, nVol, nTime);
    }
}

INT CSound::getVoiceTime() {
    if (s_hCurrentVoiceHandle) {
        CVoiceAgent* pAgent = _getVoiceAgent(s_hCurrentVoiceHandle);
        if (pAgent) {
            return pAgent->getPlayTime();
        }
    }
    return 0;
}

void CSound::setWarningPause(BOOL bPause, BOOL bOnlyBGM) {
    if (bPause) {
        TsSdStrPause(1, 0);
        if (!bOnlyBGM) {
            _setFlag(s_nFlagSE, 4, ((INT)s_nFlagSE >> 1) & 1);
            pauseAllSE();
        }
    } else {
        TsSdStrStart(1, 0);
        if (!(s_nFlagSE & 4)) {
            startAllPausedSE();
        }
        s_nFlagSE &= ~4;
    }
}

CBGMAgent::CBGMAgent(USHORT nID) : CSoundAgent(nID) {
}

CBGMAgent::~CBGMAgent() {
}

void CBGMAgent::mainProc(TIMEQ nTQ) {
}

INT CBGMAgent::getState() const {
    if (getStep() == 0) {
        return 0;
    }
    return 3;
}

HANDLE CBGMAgent::play(const CHAR* pszName, INT nVol, SHORT nFlag) {
    return 0;
}

void CBGMAgent::stop(BOOL bDummy) {
}

void CBGMAgent::reset() {
    clearHandle();
    clearName();
    clearFlag();
    changeStep(0);
}

HANDLE CBGMAgent::fadein(const CHAR* pszName, UINT nTime, INT nVol, SHORT nFlag) {
    return 0;
}

void CBGMAgent::fadeout(UINT nTime, BOOL bDummy) {
}

void CBGMAgent::setVolume(INT nVol, INT nTime) {
}

CBGMAgentStream::CBGMAgentStream(USHORT nID) : CBGMAgent(nID) {
}

CBGMAgentStream::~CBGMAgentStream() {
}

void CBGMAgentStream::mainProc(TIMEQ nTQ) {
    CRsrcVSV* pRsrc;
    switch (getStep()) {
    case 1:
        if (getFadeinTimer() == 0) {
            changeStep(5);
            break;
        }
        if (s_bCrossPlaying) {
            cancel();
            return;
        }
        s_bCrossPlaying = TRUE;
        changeStep(2);
    case 2:
        pRsrc = (CRsrcVSV*)CRsrcDB::getRsrc(0x35, getName(), NAME_CASE_LOWER);
#line 2307
        ASSERT(pRsrc);
        if (CFileIO::loadFileNoWait(pRsrc->getStartSector(), 0x60, s_pCrossBuffer)) {
            changeStep(3);
        }
        break;
    case 3: {
        INT nResult = CFileIO::syncLoadFile();
        if (nResult < 0) {
            s_bCrossPlaying = FALSE;
            changeStep(1);
            TRACE_USER_COL(TGOTO, _TTY_RED, "BGM-CrossFade buffering error... retry...\n");
            return;
        }
        if (nResult) {
            pRsrc = (CRsrcVSV*)CRsrcDB::getRsrc(0x35, getName(), NAME_CASE_LOWER);
#line 2330
            ASSERT(pRsrc);
            TsSdStrPlayMusicCrossFade(pRsrc->getStartSector(), 0, "", getHandle(), (u_int)s_pCrossBuffer, 0x60, getFadeinTimer());
            setTimer(6000);
            changeStep(9);
        }
        break;
    }
    case 4:
        changeStep(5);
        break;
    case 5: {
        BOOL bFadein = getFadeinTimer() != 0;
        INT nVol;
        pRsrc = (CRsrcVSV*)CRsrcDB::getRsrc(0x35, getName(), NAME_CASE_LOWER);
#line 2354
        ASSERT(pRsrc);
        TsSdStrPlayMusic(pRsrc->getStartSector(), 0, "", getHandle());
        if (bFadein) {
            changeStep(7);
            setTimer(getFadeinTimer() * 50);
            nVol = 0;
        } else {
            changeStep(6);
            nVol = getVolumeVal();
        }
        if (nVol >= 0) {
            TsSdStrVolSet2(1, getHandle(), nVol);
        }
        if (bFadein) {
            TsSdStrFadeVol(1, getHandle(), getVolumeVal(), getFadeinTimer(), 0);
        }
        break;
    }
    case 7:
        if (decTimer(nTQ)) {
            changeStep(6);
        }
        break;
    case 9:
        if (decTimer(nTQ)) {
            changeStep(6);
            s_bCrossPlaying = FALSE;
        }
        break;
    case 8:
        if (decTimer(nTQ)) {
            reset();
        }
        break;
    }
}

void CBGMAgentStream::cancel() {
    changeStep(0);
    clearFlag();
    CSound::stopBGM(getHandle());
    TRACE("CBGMAgentStream not started ...\n");
}

INT CBGMAgentStream::getState() const {
    switch (getStep()) {
    case 0:
        return 0;
    case 5:
        return 2;
    }
    return 3;
}

HANDLE CBGMAgentStream::play(const CHAR* pszName, INT nVol, SHORT nFlag) {
    CRsrcVSV* pRsrc;
    HANDLE h;
    if (isPlaying()) {
        return 0;
    }
    pRsrc = (CRsrcVSV*)CRsrcDB::getRsrc(0x35, pszName, NAME_CASE_LOWER);
    if (!pRsrc) {
        return 0;
    }
    h = pRsrc->getBgmID();
    setHandle(h);
    setName(pszName);
    setVolumeVal(nVol < 0 ? 127 : nVol);
    changeStep((nFlag & 0x400) ? 1 : 4);
    setFlag(1);
    setFadeinTimer(0);
    return h;
}

BOOL CBGMAgentStream::start() {
    return 0;
}

void CBGMAgentStream::stop(BOOL bDummy) {
    if (!isPlaying()) {
        return;
    }
    if (getStep() >= 6 && !bDummy) {
        TsSdStrStop(1, getHandle());
    }
    if (getStep() == 3 || getStep() == 9) {
        s_bCrossPlaying = FALSE;
    }
    reset();
}

HANDLE CBGMAgentStream::fadein(const CHAR* pszName, UINT nTime, INT nVol, SHORT nFlag) {
    HANDLE h = play(pszName, nVol, nFlag);
    if (h) {
        UINT n = nTime;
        if (n) {
            setFadeinTimer(n);
        }
    }
    return h;
}

void CBGMAgentStream::fadeout(UINT nTime, BOOL bDummy) {
    if (!isPlaying()) {
        return;
    }
    if (getStep() < 6) {
        stop(bDummy);
        return;
    }
    if (getStep() == 8) {
        return;
    }
    if (!bDummy) {
        TsSdStrFadeVol(1, getHandle(), 0, nTime, 1);
    }
    changeStep(8);
    setTimer(nTime * 50);
}

void CBGMAgentStream::setVolume(INT nVol, INT nTime) {
    if (!isPlaying()) {
        return;
    }
    if (getStep() == 8) {
        return;
    }
    setVolumeVal(nVol);
    if (getStep() < 6) {
        return;
    }
    TsSdStrFadeVol(1, getHandle(), nVol, nTime, 0);
}

CVoiceAgent::CVoiceAgent(USHORT nID) : CSoundAgent(nID) {
    m_nPlayID = -1;
}

CVoiceAgent::~CVoiceAgent() {
}

void CVoiceAgent::mainProc(TIMEQ nTQ) {
    switch (getStep()) {
    case 1:
        changeStep(2);
        break;
    case 2:
        TsSdStrPlayVoice(m_nAddr);
        changeStep(3);
        if (getVolumeVal() != 127) {
            setVolume(getVolumeVal(), 1);
        }
        if (m_nPan != 64) {
            setPan(m_nPan, 1);
        }
        break;
    case 3:
        if (!TsSdStrPlayVoiceCheck(m_nAddr)) {
            reset();
        }
        break;
    case 4:
        if (decTimer(nTQ)) {
            stop(FALSE);
        }
        break;
    }
}

HANDLE CVoiceAgent::play(const CHAR* pszVoice, INT nVol, INT nPan) {
    CRsrcData* pRsrc;
    HANDLE hHandle;
    if (isPlaying()) {
        return 0;
    }
    pRsrc = CRsrcDB::getRsrc(0x3A, pszVoice, NAME_CASE_LOWER);
    if (!pRsrc) {
        return 0;
    }
    m_nAddr = (INT)pRsrc->getRsrcData();
    if (!m_nAddr) {
        return 0;
    }
    hHandle = generateHandle();
    setHandle(hHandle);
    setName(pszVoice);
    setVolumeVal(nVol);
    m_nPan = nPan;
    changeStep(1);
    setFlag(1);
    m_nTime = 0;
    return hHandle;
}

HANDLE CVoiceAgent::playEx(const CHAR* pszVoice, INT nVol, INT nPan) {
    return 0;
}

void CVoiceAgent::stop(BOOL bDummy) {
    if (!isPlaying()) {
        return;
    }
    if (getStep() >= 3) {
        TsSdStrStop(2, 0);
    }
    reset();
}

void CVoiceAgent::fadeout(UINT nTime, BOOL bDummy) {
    if (nTime == 0) {
        stop(FALSE);
        return;
    }
    if (!isPlaying()) {
        return;
    }
    if (getStep() >= 3) {
        reset();
        return;
    }
    TsSdStrFadeVol(2, 0, 0, nTime, 1);
    changeStep(4);
    setTimer(nTime * 50);
}

void CVoiceAgent::setVolume(INT nVol, INT nTime) {
    if (!isPlaying()) {
        return;
    }
    if (getStep() < 3) {
        return;
    }
    if (getStep() == 4) {
        return;
    }
    if (nVol < 0) {
        nVol = 127;
    }
    setVolumeVal(nVol);
    TsSdStrVolSet2(2, 0, nVol);
}

void CVoiceAgent::setPan(INT nPan, INT nTime) {
    if (!isPlaying()) {
        return;
    }
    if (getStep() < 3) {
        return;
    }
    if (nPan < 0) {
        nPan = 64;
    }
    m_nPan = nPan;
    TsSdStrVoiceSetPan(m_nAddr, nPan);
}

void CVoiceAgent::setPanAndVol(INT nPan, INT nVol, INT nTime) {
    if (!isPlaying()) {
        return;
    }
    if (getStep() < 3) {
        return;
    }
    if (nPan < 0) {
        nPan = 64;
    }
    m_nPan = nPan;
    TsSdStrVoiceSetPan(m_nAddr, nPan);
    if (nVol < 0) {
        nVol = 127;
    }
    setVolumeVal(nVol);
    TsSdStrVolSet2(2, nTime, nVol);
}

void CVoiceAgent::reset() {
    clearHandle();
    clearName();
    clearFlag();
    changeStep(0);
    m_nTime = 0;
    m_nAddr = 0;
}

HANDLE CVoiceAgent::generateHandle() {
    HANDLE h = ++s_hVoiceHandleGenerator & 0xFF;
    if (h == 0) {
        h = ++s_hVoiceHandleGenerator & 0xFF;
    }
    return h;
}

CMasterVolAgent::CMasterVolAgent() {
    m_nVolume = 128 << 16;
    m_nVolumeDelta = 0;
    m_nTargetVolume = 0;
    m_nTimer = 0;
}

CMasterVolAgent::~CMasterVolAgent() {
}

void CMasterVolAgent::mainProc(TIMEQ nTQ) {
    if (m_nTimer > 0) {
        m_nTimer -= nTQ;
        m_nVolume += m_nVolumeDelta * nTQ;
        if (m_nTimer < 3) {
            m_nTimer = 0;
            m_nVolume = m_nTargetVolume;
        }
        _setBGMMasterVolume((SHORT)(m_nVolume >> 16));
    }
}

void CMasterVolAgent::setVolume(INT nVol, INT nTime) {
    INT f;
    if ((m_nVolume >> 16) == nVol) {
        return;
    }
    if (nTime < 2) {
        m_nVolume = nVol << 16;
        m_nTimer = 0;
        _setBGMMasterVolume(nVol);
        return;
    }
    f = nTime * 50;
    if ((UINT)nVol > 128) {
        nVol = 128;
    }
    m_nTargetVolume = nVol << 16;
    m_nTimer = f;
    m_nVolumeDelta = (m_nTargetVolume - m_nVolume) / f;
}

static CHAR* s_BgmTbl[45] = {
    "03F_Hercul",
    "Boss2_World",
    "Event1_Kinpak",
    "Title",
    "03F_Hercules_B",
    "Event_Unrest",
    "Event2_",
    "01F_Town_B",
    "01F_Town_F",
    "Boss1",
    "Event4",
    "12_Event_Yuttari",
    "13_Event_Namine",
    "14_Event_Odayaka",
    "15_Event_XIII",
    "16_XIIIBoss1",
    "17_Halloween_F",
    "18_Helloween_B",
    "19_Alice_Field",
    "20_Event_13thFloor",
    "21_Alice_Battle",
    "22_PeterPan_Field",
    "23_PeterPan_Battle",
    "24_Pinocchio_Field",
    "25_Aladdin_F",
    "26_Aladdin_B",
    "27_Pinocchio_B",
    "28_Forget_F",
    "29_Forget_B",
    "30_Twilight_F",
    "31_Twilight_B",
    "32_Destiny_F",
    "33_Destiny_B",
    "34_Boss_RikuAnsem",
    "35_Boss_NiseRiku",
    "36_WinnieThePooh",
    "37_LastBoss1",
    "38_UnderTheSea",
    "39_LittleMermaid_B",
    "40_LastBoss2",
    "41_LastBoss3",
    "42_Hollow_F",
    "43_Hollow_B",
    "44_PooGame1",
    "45_PoohGame2",
};

UINT CSound::getBgmID_withName(const CHAR* pszName) {
    UINT i;
    const CHAR* ps;
    const CHAR* pd;
    if (!pszName || !*pszName) {
        return 0;
    }
    for (i = 0; i < 45; i++) {
        ps = s_BgmTbl[i];
        pd = pszName;
        while (*ps && *ps == *pd) {
            ps++;
            pd++;
        }
        if (!*ps && (!*pd || *pd == '.')) {
            return i + 1;
        }
    }
    return 0;
}

static CHAR* _getBgmName_withID(UINT nID) {
    if (nID - 1 >= 44) {
        return NULL;
    }
    return s_BgmTbl[nID - 1];
}
// SOURCE DATA: .data .rodata .bss
// SOURCE DATA SLUS_217.99_retail: .bss .data .rodata
