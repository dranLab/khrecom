#include "System/PadIO.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Debug.h"
#include "System/Rnd.h"
#include "System/Util.h"

typedef struct {
    unsigned int option;
    int port;
    int slot;
    int number;
    unsigned char name[16];
} scePad2SocketParam;

extern "C" {
int sceDbcInit(void);
int scePad2Init(int mode);
int scePad2End(void);
int scePad2CreateSocket(scePad2SocketParam* param, void* addr);
int scePad2DeleteSocket(int socket);
int scePad2GetState(int socket);
int scePad2GetButtonProfile(int socket, unsigned char* profile);
int scePad2Read(int socket, unsigned char* data);
int sceVibSetActParam(int socket, int profilesize, unsigned char* profile, int datasize, unsigned char* data);
}

extern INT VSYNC_TIMEQ;

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)


#define PAD_MAX 2
#define ROBOT_PAD_MAX 24
#define VIB_SLOT_MAX 8

union _VIB_DATA {
    USHORT _sVibData;
    unsigned char _cVibDatas[2];
};

struct CRobotPad {
protected:
    BOOL m_bEnableFlag;
    int m_nPadMode[ROBOT_PAD_MAX];
    int m_nPadRate[ROBOT_PAD_MAX][3];
    int m_nPadCount[ROBOT_PAD_MAX];
    int m_nPadCountMax[ROBOT_PAD_MAX];

public:
    CRobotPad();
    virtual ~CRobotPad();
    BOOL isEnable() const;
    void setEnableFlag(BOOL bEnableFlag);
    void setPadRate(INT nRobotPad, INT nTrgRate, INT nPressRate);
    void setPadData(BYTE* pData);

protected:
    void setPadMode(INT nRobotPad);
};

struct VIB_SLOT {
    USHORT _MotorL;
    USHORT _MotorS;
    TIMEQ _Timeq;

    void init() { _Timeq = 0; }
    void main(TIMEQ tq) {
        if (_Timeq < tq) {
            _Timeq = 0;
        } else {
            _Timeq -= tq;
        }
    }
};

struct CJoyPad {
public:
    INT m_nPort;
    UINT m_nBtnDown;
    UINT m_nBtnDownOld;
    UINT m_nBtnTrg;
    UINT m_nBtnRTrg;
    UINT m_nBtnRepeat;
    TIMEQ m_nBtnRepeatTimer;
    VIB_SLOT m_VibSlot[VIB_SLOT_MAX];
    BOOL m_bVibEnable;
    UINT m_nScan;
    int m_nAnalog[4];
    BOOL m_bBoot;
    CRobotPad m_Robot;

    CJoyPad(USHORT nPort);
    virtual ~CJoyPad() {}
    virtual void init() {}
    virtual void readProc() {}
    virtual BOOL isConnected() { return FALSE; }
    virtual BOOL isStandby() { return TRUE; }
    INT getPortID() { return m_nPort; }
    virtual INT getAnalogInfo(UINT nKind) { return 0; }
    INT isButtonDown(UINT b) { return (m_nBtnDown & b) == b; }
    INT isButtonTrg(UINT b) { return m_nBtnTrg & b; }
    INT isButtonRTrg(UINT b) { return m_nBtnRTrg & b; }
    INT isButtonRpt(UINT b) { return m_nBtnRepeat & b; }
    INT getPressedButton() { return m_nBtnDown; }
    INT getAnalogRX() { return m_nAnalog[0]; }
    INT getAnalogRY() { return m_nAnalog[1]; }
    INT getAnalogLX() { return m_nAnalog[2]; }
    INT getAnalogLY() { return m_nAnalog[3]; }
    virtual BYTE getVibMotorL() { return 0; }
    virtual BYTE getVibMotorS() { return 0; }
    void update(TIMEQ nTickCount);
    void initVib();
    void mainVib(TIMEQ tq);
    INT startVib(INT motorL, INT motorS, TIMEQ tq);
    void stopVib(INT handle);
    void enableVib(BOOL f) { m_bVibEnable = f; }
    BOOL isEnableVib() { return m_bVibEnable; }
    VIB_SLOT& getVibSlot(INT n) { return m_VibSlot[n]; }
    void reset();
};

struct CJoyPad2 : CJoyPad {
    scePad2SocketParam m_socket;
    INT m_nSocket;
    INT m_nPhase;
    INT m_nState;
    INT m_nProfileSize;
    INT m_nRdataLen;
    BYTE m_cMotorS;
    BYTE m_cMotorL;
    BYTE m_cNoLinkCount;
    BYTE _dum;
    unsigned char m_rdata[256];
    unsigned char m_profile[20];

    CJoyPad2(USHORT nPort);
    virtual ~CJoyPad2();
    virtual void init();
    virtual void readProc();
    virtual BOOL isConnected();
    virtual BOOL isStandby();
    virtual BYTE getVibMotorL() { return isEnableVib() ? m_cMotorL : 0; }
    virtual BYTE getVibMotorS() { return isEnableVib() ? m_cMotorS : 0; }
    virtual INT getAnalogInfo(UINT nKind);
    void setVibParam(BYTE cL, BYTE cS);
    INT readAnalogStick(UINT nOfs);
    INT readAnalogButton(UINT nOfs);
};

namespace {
u_long128 s_PadBuf[PAD_MAX][16] __attribute__((aligned(64)));
CJoyPad* s_pJoyPad[PAD_MAX];
BOOL s_bVibPause;
}

#line 60
void CPadIO::initialize() {
    INT i;
    CJoyPad* pPad;

    sceDbcInit();
    scePad2Init(0);
    s_bVibPause = FALSE;
    for (i = 0; i < PAD_MAX; i++) {
        pPad = new KH_AT("(68)") CJoyPad2(i);
        ASSERT(pPad);
        s_pJoyPad[i] = pPad;
        pPad->init();
    }
}

void CPadIO::finalize() {
    INT i;

    for (i = PAD_MAX - 1; i >= 0; i--) {
        delete s_pJoyPad[i];
        s_pJoyPad[i] = NULL;
    }
    scePad2End();
}

void CPadIO::mainProc(TIMEQ nTickCount, TIMEQ nRate) {
    INT i;
    CJoyPad* pPad;

    for (i = 0; i < PAD_MAX; i++) {
        pPad = s_pJoyPad[i];
        pPad->readProc();
        pPad->update(nTickCount);
        if (!isVibrationSuspended()) {
            pPad->mainVib(nRate);
        }
    }
}

INT CPadIO::isButtonDown(UINT nBtn, UINT nPad) {
    return nPad >= PAD_MAX ? 0 : s_pJoyPad[nPad]->isButtonDown(nBtn);
}

INT CPadIO::isButtonTrg(UINT nBtn, UINT nPad) {
    return nPad >= PAD_MAX ? 0 : s_pJoyPad[nPad]->isButtonTrg(nBtn);
}

INT CPadIO::isButtonRTrg(UINT nBtn, UINT nPad) {
    return nPad >= PAD_MAX ? 0 : s_pJoyPad[nPad]->isButtonRTrg(nBtn);
}

INT CPadIO::isButtonRpt(UINT nBtn, UINT nPad) {
    return nPad >= PAD_MAX ? 0 : s_pJoyPad[nPad]->isButtonRpt(nBtn);
}

UINT CPadIO::getButtonDown(UINT nPad) {
    return nPad < PAD_MAX ? s_pJoyPad[nPad]->m_nBtnDown : 0;
}

UINT CPadIO::getButtonDownOld(UINT nPad) {
    return nPad < PAD_MAX ? s_pJoyPad[nPad]->m_nBtnDownOld : 0;
}

INT CPadIO::getAnalogRX(UINT nPad) {
    return nPad >= PAD_MAX ? 0 : s_pJoyPad[nPad]->getAnalogRX();
}

INT CPadIO::getAnalogRY(UINT nPad) {
    return nPad >= PAD_MAX ? 0 : s_pJoyPad[nPad]->getAnalogRY();
}

INT CPadIO::getAnalogLX(UINT nPad) {
    return nPad >= PAD_MAX ? 0 : s_pJoyPad[nPad]->getAnalogLX();
}

INT CPadIO::getAnalogLY(UINT nPad) {
    return nPad >= PAD_MAX ? 0 : s_pJoyPad[nPad]->getAnalogLY();
}

BYTE CPadIO::getVibMotorL(UINT nPad) {
    return nPad < PAD_MAX ? s_pJoyPad[nPad]->getVibMotorL() : 0;
}

BYTE CPadIO::getVibMotorS(UINT nPad) {
    return nPad < PAD_MAX ? s_pJoyPad[nPad]->getVibMotorS() : 0;
}

INT CPadIO::startVibration(INT motorL, INT motorS, TIMEQ tq, UINT nPad) {
    return nPad >= PAD_MAX ? -1 : s_pJoyPad[nPad]->startVib(motorL, motorS, tq);
}

void CPadIO::stopVibration(INT handle, UINT nPad) {
    if (nPad < PAD_MAX) {
        s_pJoyPad[nPad]->stopVib(handle);
    }
}

void CPadIO::resetVibration() {
    int i;

    for (i = 0; i < PAD_MAX; i++) {
        s_pJoyPad[i]->initVib();
    }
}

void CPadIO::enableVibration(BOOL f, UINT nPad) {
    int i;

    for (i = 0; i < PAD_MAX; i++) {
        s_pJoyPad[i]->enableVib(f);
    }
}

void CPadIO::suspendVibration() {
    s_bVibPause = TRUE;
}

void CPadIO::resumeVibration() {
    s_bVibPause = FALSE;
}

BOOL CPadIO::isVibrationSuspended() {
    return s_bVibPause;
}

BOOL CPadIO::isConnected(UINT nPad) {
    return nPad >= PAD_MAX ? FALSE : s_pJoyPad[nPad]->isConnected();
}

BOOL CPadIO::isStandby(UINT nPad) {
    return nPad >= PAD_MAX ? FALSE : s_pJoyPad[nPad]->isStandby();
}

BOOL CPadIO::isRobotEnable(UINT nPad) {
    return s_pJoyPad[nPad]->m_Robot.isEnable();
}

void CPadIO::setRobotEnableFlag(BOOL bEnableFlag, UINT nPad) {
    s_pJoyPad[nPad]->m_Robot.setEnableFlag(bEnableFlag);
}

void CPadIO::setRobotPadRate(INT nRobotPad, INT nTrgRate, INT nPressRate, UINT nPad) {
    s_pJoyPad[nPad]->m_Robot.setPadRate(nRobotPad, nTrgRate, nPressRate);
}

CJoyPad::CJoyPad(USHORT nPort)
    : m_nPort(nPort), m_nBtnDown(0), m_nBtnDownOld(0), m_nBtnTrg(0), m_nBtnRTrg(0), m_nBtnRepeat(0),
      m_nBtnRepeatTimer(0), m_bVibEnable(TRUE), m_nScan(0), m_bBoot(FALSE) {
    INT i;

    for (i = 0; i < 4; i++) {
        m_nAnalog[i] = 0;
    }
}

void CJoyPad::reset() {
    INT i;

    m_nScan = 0;
    m_nBtnDown = 0;
    m_nBtnDownOld = 0;
    m_nBtnTrg = 0;
    m_nBtnRTrg = 0;
    m_nBtnRepeat = 0;
    for (i = 0; i < 4; i++) {
        m_nAnalog[i] = 0;
    }
    initVib();
}

void CJoyPad::update(TIMEQ nTickCount) {
    UINT nBtn;
    INT a;

    nBtn = m_nScan;
    {
        INT nVal = m_nAnalog[0] = getAnalogInfo(0x10000000);
        if (nVal >= 0xE00) {
            nBtn |= 0x80000;
        } else if (nVal <= -0xE00) {
            nBtn |= 0x40000;
        }
    }
    {
        INT nVal = m_nAnalog[1] = getAnalogInfo(0x20000000);
        if (nVal >= 0xE00) {
            nBtn |= 0x20000;
        } else if (nVal <= -0xE00) {
            nBtn |= 0x10000;
        }
    }
    {
        INT nVal = m_nAnalog[2] = getAnalogInfo(0x40000000);
        if (nVal >= 0xE00) {
            nBtn |= 0x800000;
        } else if (nVal <= -0xE00) {
            nBtn |= 0x400000;
        }
    }
    {
        INT nVal = m_nAnalog[3] = getAnalogInfo(0x80000000);
        if (nVal >= 0xE00) {
            nBtn |= 0x200000;
        } else if (nVal <= -0xE00) {
            nBtn |= 0x100000;
        }
    }
    if ((nBtn & 0x5000) == 0x5000) {
        nBtn &= ~0x5000;
    }
    if ((nBtn & 0xA000) == 0xA000) {
        nBtn &= ~0xA000;
    }
    m_nBtnDownOld = m_nBtnDown;
    m_nBtnDown = nBtn;
    if (m_bBoot) {
        m_nBtnDownOld = nBtn;
        m_bBoot = FALSE;
    }
    m_nBtnTrg = m_nBtnDown & ~m_nBtnDownOld;
    m_nBtnRTrg = ~m_nBtnDown & m_nBtnDownOld;
    if (m_nBtnTrg) {
        m_nBtnRepeatTimer = nTickCount;
        m_nBtnRepeat = m_nBtnTrg;
    } else if (nTickCount - m_nBtnRepeatTimer > VSYNC_TIMEQ * 20) {
        m_nBtnRepeat = m_nBtnDown;
        m_nBtnRepeatTimer += VSYNC_TIMEQ * 4;
    } else {
        m_nBtnRepeat = 0;
    }
}

void CJoyPad::initVib() {
    int i;

    for (i = 0; i < VIB_SLOT_MAX; i++) {
        m_VibSlot[i].init();
    }
}

void CJoyPad::mainVib(TIMEQ tq) {
    int i;

    for (i = 0; i < VIB_SLOT_MAX; i++) {
        m_VibSlot[i].main(tq);
    }
}

INT CJoyPad::startVib(INT motorL, INT motorS, TIMEQ tq) {
    int i;

    for (i = 0; i < VIB_SLOT_MAX; i++) {
        if (getVibSlot(i)._Timeq == 0) {
            if (motorL >= 0xFF) motorL = 0xFF;
            if (motorL < 0) motorL = 0;
            if (motorS >= 1) motorS = 1;
            if (motorS < 0) motorS = 0;
            getVibSlot(i)._MotorS = motorS;
            getVibSlot(i)._MotorL = motorL;
            getVibSlot(i)._Timeq = tq;
            return (motorL << 24) | (motorS << 16) | i;
        }
    }
    return -1;
}

void CJoyPad::stopVib(INT handle) {
    int i;
    int L;
    int S;

    if (handle == -1) return;
    i = handle & 0xFF;
    S = (handle >> 16) & 0xFF;
    L = (UINT)handle >> 24;
    if ((UINT)i < VIB_SLOT_MAX) {
        if (getVibSlot(i)._MotorL == L && getVibSlot(i)._MotorS == S) {
            getVibSlot(i).init();
        }
    }
}

CJoyPad2::CJoyPad2(USHORT nPort) : CJoyPad(nPort), m_nPhase(0), m_cNoLinkCount(0) {}

CJoyPad2::~CJoyPad2() {
    if (m_nSocket >= 0) {
        scePad2DeleteSocket(m_nSocket);
    }
}

void CJoyPad2::init() {
    INT nPort;
    void* addr;

    nPort = getPortID();
    m_socket.option = 2;
    m_socket.port = nPort != 0;
    m_socket.slot = 0;
    addr = s_PadBuf[nPort];
    m_nSocket = scePad2CreateSocket(&m_socket, addr);
    if (m_nSocket < 0) {
        TRACE("\x83p\x83" "b\x83h%d\x8f\x89\x8a\xfa\x89\xbb\x8e\xb8\x94s\n", nPort);
    } else {
        TRACE("\x83|\x81[\x83g%d \x83p\x83" "b\x83h\x8f\x89\x8a\xfa\x89\xbb\x90\xac\x8c\xf7(%d) %x\n", nPort,
              m_nSocket, addr);
        initVib();
    }
}

void CJoyPad2::readProc() {
    int i;

    m_nState = scePad2GetState(m_nSocket);
    if (m_nState == 0) {
        if (m_cNoLinkCount < 0x7F) {
            m_cNoLinkCount++;
        }
    } else {
        m_cNoLinkCount = 0;
    }
    if (m_nState != 1) {
        if (m_nPhase == 2) {
            reset();
            setVibParam(0, 0);
        }
        m_nPhase = 0;
        return;
    }
    switch (m_nPhase) {
    case 0:
        m_nProfileSize = scePad2GetButtonProfile(m_nSocket, m_profile);
        m_nPhase = 1;
        break;
    case 1:
        m_bBoot = TRUE;
        m_nPhase = 2;
    case 2:
        m_nRdataLen = scePad2Read(m_nSocket, m_rdata);
        m_Robot.setPadData(m_rdata);
        m_cMotorS = 0;
        m_cMotorL = 0;
        m_nScan = ((m_rdata[0] << 8) | m_rdata[1]) ^ 0xFFFF;
        if (isEnableVib() && !CPadIO::isVibrationSuspended()) {
            for (i = 0; i < VIB_SLOT_MAX; i++) {
                if (getVibSlot(i)._Timeq > 0) {
                    m_cMotorS += getVibSlot(i)._MotorS;
                    m_cMotorL += getVibSlot(i)._MotorL;
                }
            }
            if (m_cMotorL >= 0xFF) {
                m_cMotorL = 0xFF;
            }
            if (m_cMotorS) {
                m_cMotorS = 1;
            }
        }
        setVibParam(m_cMotorL, m_cMotorS);
        break;
    }
}

void CJoyPad2::setVibParam(BYTE cL, BYTE cS) {
    _VIB_DATA vd;
    BYTE profile;

    vd._sVibData = (cS & 1) | (cL << 1);
    profile = 3;
    sceVibSetActParam(m_nSocket, 1, &profile, 2, vd._cVibDatas);
}

BOOL CJoyPad2::isConnected() {
    return m_nState == 1;
}

BOOL CJoyPad2::isStandby() {
    return !(m_nState == 1 && m_nPhase == 2);
}

INT CJoyPad2::getAnalogInfo(UINT nKind) {
    if (m_nPhase != 2) return 0;
    switch (nKind) {
    case 0x10000000: return readAnalogStick(2);
    case 0x20000000: return readAnalogStick(3);
    case 0x40000000: return readAnalogStick(4);
    case 0x80000000: return readAnalogStick(5);
    case 0x2000: return readAnalogButton(6);
    case 0x8000: return readAnalogButton(7);
    case 0x1000: return readAnalogButton(8);
    case 0x4000: return readAnalogButton(9);
    case 0x10: return readAnalogButton(10);
    case 0x20: return readAnalogButton(11);
    case 0x80: return readAnalogButton(12);
    case 0x40: return readAnalogButton(13);
    case 0x4: return readAnalogButton(14);
    case 0x8: return readAnalogButton(15);
    case 0x1: return readAnalogButton(16);
    case 0x2: return readAnalogButton(17);
    }
    return 0;
}

INT CJoyPad2::readAnalogStick(UINT nOfs) {
    INT nVal;
    BOOL bMinus;
    INT nRet;

    nVal = m_rdata[nOfs];
    nVal -= 0x80;
    bMinus = FALSE;
    if (nVal < 0) {
        nVal = -nVal - 1;
        bMinus = TRUE;
    }
    if (nVal < 0x20) {
        nRet = 0;
    } else if (nVal == 0x7F) {
        nRet = 0x1000;
    } else {
        nRet = ((nVal - 0x20) << 12) / 0x5E;
    }
    return bMinus ? -nRet : nRet;
}

INT CJoyPad2::readAnalogButton(UINT nOfs) {
    INT nRet;

    nRet = m_rdata[nOfs];
    if (nRet < 0x20) {
        nRet = 0;
    } else if ((UINT)nRet >= 0xFE) {
        nRet = 0x1000;
    } else {
        nRet = ((nRet - 0x20) << 12) / 0xDE;
    }
    return nRet;
}

CRobotPad::CRobotPad() {
    INT i;
    INT j;

    m_bEnableFlag = FALSE;
    for (i = 0; i < ROBOT_PAD_MAX; i++) {
        m_nPadMode[i] = -1;
        for (j = 0; j < 3; j++) {
            m_nPadRate[i][j] = 0;
        }
        m_nPadCount[i] = 0;
        m_nPadCountMax[i] = 0;
    }
}

CRobotPad::~CRobotPad() {}

BOOL CRobotPad::isEnable() const {
    return m_bEnableFlag;
}

void CRobotPad::setEnableFlag(BOOL bEnableFlag) {
    m_bEnableFlag = bEnableFlag;
}

#line 944
void CRobotPad::setPadRate(INT nRobotPad, INT nTrgRate, INT nPressRate) {
    ASSERT((nRobotPad >= 0) && (nRobotPad < ROBOT_PAD_MAX));
    if (nTrgRate + nPressRate >= 1000) {
        nTrgRate = 0;
        nPressRate = 1000;
    }
    m_nPadRate[nRobotPad][1] = nTrgRate;
    m_nPadRate[nRobotPad][2] = nPressRate;
    m_nPadMode[nRobotPad] = -1;
}

#define PAD_MIN(n) { INT min = nMin; FLOAT len = RND::genrand_real1() * (0xFF - min); pData[n] = min + len; }
#define PAD_MAX_(n) { INT max = nMax; FLOAT len = RND::genrand_real1() * max; pData[n] = 0 + len; }

void CRobotPad::setPadData(BYTE* pData) {
    INT i;
    INT nMin;
    INT nMax;

    if (!isEnable()) return;
    for (i = 0; i < 18; i++) {
        pData[i] = 0;
    }
    pData[2] = 0x80;
    pData[3] = 0x80;
    pData[4] = 0x80;
    pData[5] = 0x80;
    for (i = 0; i < ROBOT_PAD_MAX; i++) {
        if (m_nPadMode[i] == -1) {
            setPadMode(i);
        } else if (m_nPadMode[i] != 0) {
            if (i < 8) {
                pData[1] |= 1 << i;
            } else if (i < 16) {
                pData[0] |= 1 << (i - 8);
            }
            nMax = 0x30;
            nMin = 0xD0;
            switch (i) {
            case 0: PAD_MIN(0x10); break;
            case 1: PAD_MIN(0x11); break;
            case 2: PAD_MIN(0xE); break;
            case 3: PAD_MIN(0xF); break;
            case 12: PAD_MIN(0x8); break;
            case 13: PAD_MIN(0x6); break;
            case 14: PAD_MIN(0x9); break;
            case 15: PAD_MIN(0x7); break;
            case 4: PAD_MIN(0xA); break;
            case 5: PAD_MIN(0xB); break;
            case 6: PAD_MIN(0xC); break;
            case 7: PAD_MIN(0xD); break;
            case 16: PAD_MAX_(0x3); break;
            case 17: PAD_MIN(0x2); break;
            case 18: PAD_MIN(0x3); break;
            case 19: PAD_MAX_(0x2); break;
            case 20: PAD_MAX_(0x5); break;
            case 21: PAD_MIN(0x4); break;
            case 22: PAD_MIN(0x5); break;
            case 23: PAD_MAX_(0x4); break;
            }
        }
        m_nPadCount[i]++;
        if (m_nPadCount[i] > m_nPadCountMax[i]) {
            m_nPadMode[i] = -1;
        }
    }
    pData[0] = ~pData[0];
    pData[1] = ~pData[1];
}

#line 1076
void CRobotPad::setPadMode(INT nRobotPad) {
    ASSERT((nRobotPad >= 0) && (nRobotPad < ROBOT_PAD_MAX));
    m_nPadRate[nRobotPad][0] = 1000 - (m_nPadRate[nRobotPad][1] + m_nPadRate[nRobotPad][2]);
    m_nPadMode[nRobotPad] = lotProbability(m_nPadRate[nRobotPad], 3);
    if (m_nPadMode[nRobotPad] == 1) {
        FLOAT len = RND::genrand_real1() * 3.0f;
        m_nPadCountMax[nRobotPad] = (INT)(len + 1.0f);
    } else if (m_nPadMode[nRobotPad] == 2) {
        FLOAT len = RND::genrand_real1() * 180.0f;
        m_nPadCountMax[nRobotPad] = (INT)(len + 120.0f);
    } else {
        FLOAT len = RND::genrand_real1() * 119.0f;
        m_nPadCountMax[nRobotPad] = (INT)(len + 1.0f);
    }
    // empty barrier: keeps gcse from reusing nRobotPad * 4 from above (the original recomputes it here)
    __asm__ volatile("" : "+r"(nRobotPad));
    m_nPadCount[nRobotPad] = 0;
}

// SOURCE DATA: .rodata .bss
