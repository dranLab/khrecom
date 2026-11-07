#include "System/Graphic/Gprim.h"
#include "System/Graphic/Str.h"
#include "System/DebugProcInfo.h"
#include "System/HeapMan.h"
#include "System/MainApp.h"
#include "System/PadIO.h"
#include "System/Debug.h"
#include "Game/Debug/DebugWork.h"
#include "Game/SceneInfo.h"

#include "Game/Game.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);










#include "System/Graphic/ModelMan.h"

#include "System/Graphic/PackMan.h"





extern INT SCREEN_HEIGHT;

/* In the original, the struct member stores below do not let gcc keep these globals' values in registers (the
 * loads are repeated after every statement): accessed through a reference, they aren't scalar MEMs. */
template <class T> static inline T& _ref(T& v) { return v; }

namespace {
CFnt* _pStr[10];
char _szBuf[10][32];
CTile* _ptlCountBack;
char _szASM[64];
CFnt* _pfntASM;
CTile* _pgptASM;
char _szPAD[34];
CFnt* _pfntPAD;
CTile* _pgptPAD;
}

static inline void _setXYZ(G_XYZ& p, FIX4 x, FIX4 y, FIX4 z) {
    p.set(x, y, z);
}

static inline void _setFntPos(CFntM2* p, INT x, INT y) {
    _setXYZ(p->m_P0, (x / 2) << 4, y << 4, 0xFFFF28 << 4);
}

static inline void _setFntXY(CFntM2* p, INT x, INT y) {
    _setXYZ(p->m_P0, x << 4, y << 4, 0xFFFF28 << 4);
}

static inline void _setColor(G_RGBA& c, UBYTE r, UBYTE g, UBYTE b) {
    c.r = r;
    c.g = g;
    c.b = b;
}

void CDebugProcInfo::initialize() {
    INT i;
    for (i = 0; i < 10; i++) {
        CFnt*& pStr = _pStr[i];
        pStr = new (__FILE__ "(64)") CFnt;
        if (pStr) {
            _szBuf[i][0] = 0;
            pStr->setText(_szBuf[i]);
            pStr->m_Size = 14 << 4;
            if (i >= 5) {
                _setFntPos(pStr, (i - 5) * 70 - 140, _ref(SCREEN_HEIGHT) / 2 - 10);
            } else {
                _setFntPos(pStr, i * 70 - 140, _ref(SCREEN_HEIGHT) / 2 - 24);
            }
            pStr->setStyle(1);
            _setColor(pStr->m_C0, 0x60, 0x60, 0);
            pStr->setLayerFG(24);
        }
    }
    if (_pStr[4]) _pStr[4]->m_P0.x += 7 << 4;

    _ref(_ptlCountBack) = new (__FILE__ "(84)") CTile;
    _ref(_ptlCountBack)->m_P0.set(-104 << 4, (_ref(SCREEN_HEIGHT) / 2 - 38) << 4);
    _ref(_ptlCountBack)->m_P1.set(78 << 4, (_ref(SCREEN_HEIGHT) / 2 - 8) << 4);
    _ref(_ptlCountBack)->m_C0.set(0, 0, 0, 0x50);
    _ref(_ptlCountBack)->setSemiTrans(ALPHA_BLEND);
    _ref(_ptlCountBack)->setLayerFG(24);

    _ref(_pfntASM) = new (__FILE__ "(92)") CFnt;
    _ref(_pfntASM)->setText(_szASM);
    _setFntXY(_ref(_pfntASM), 240, 24 - _ref(SCREEN_HEIGHT) / 2);
    _ref(_pfntASM)->setStyle(1);
    _setColor(_ref(_pfntASM)->m_C0, 0x60, 0x60, 0);
    _ref(_pfntASM)->setLayerFG(24);
    _ref(_pfntASM)->m_Size = 12 << 4;
    _ref(_pfntASM)->m_Space.y = 32;

    _ref(_pgptASM) = new (__FILE__ "(101)") CTile;
    _ref(_pgptASM)->m_C0.set(0, 0, 0, 0x50);
    _ref(_pgptASM)->setSemiTrans(ALPHA_BLEND);
    _ref(_pgptASM)->setLayerFG(24);
    _ref(_pgptASM)->m_P0.set(60 << 4, (10 - _ref(SCREEN_HEIGHT) / 2) << 4);
    _ref(_pgptASM)->m_P1.set(244 << 4, (24 - _ref(SCREEN_HEIGHT) / 2) << 4);

    _ref(_pfntPAD) = new (__FILE__ "(109)") CFnt;
    _ref(_pfntPAD)->setText(_szPAD);
    _setFntXY(_ref(_pfntPAD), -46, 24 - _ref(SCREEN_HEIGHT) / 2);
    _ref(_pfntPAD)->setStyle(2);
    _setColor(_ref(_pfntPAD)->m_C0, 0x60, 0x60, 0);
    _ref(_pfntPAD)->setLayerFG(24);
    _ref(_pfntPAD)->m_Size = 12 << 4;

    _ref(_pgptPAD) = new (__FILE__ "(117)") CTile;
    _ref(_pgptPAD)->m_C0.set(0, 0, 0, 0x50);
    _ref(_pgptPAD)->setSemiTrans(ALPHA_BLEND);
    _ref(_pgptPAD)->setLayerFG(24);
    _ref(_pgptPAD)->m_P0.set(-145 << 4, (10 - _ref(SCREEN_HEIGHT) / 2) << 4);
    _ref(_pgptPAD)->m_P1.set(53 << 4, (26 - _ref(SCREEN_HEIGHT) / 2) << 4);
}

void CDebugProcInfo::finalize() {
    INT i;
    for (i = 0; i < 10; i++) {
        delete _pStr[i];
        _pStr[i] = NULL;
    }
    delete _ptlCountBack;
    _ptlCountBack = NULL;
    delete _pfntASM;
    _pfntASM = NULL;
    delete _pgptASM;
    _pgptASM = NULL;
    delete _pfntPAD;
    _pfntPAD = NULL;
    delete _pgptPAD;
    _pgptPAD = NULL;
}

void CDebugProcInfo::dispProcInfo() {
    INT nFloor;
    INT nArea;
    INT nScene;
    INT nRoom;
    INT nMap;
    INT i;

    if (DebugWork::g_dw.sys.m_iDispCounter) return;

    sprintf(_szBuf[0], "%d", CMainApp::getProcCount());
    sprintf(_szBuf[1], "%d", CMainApp::getDrawCount());
    sprintf(_szBuf[2], "%d", CMainApp::getDraw2Count());
    sprintf(_szBuf[3], "%d", CMainApp::getProcCountMax());
    sprintf(_szBuf[4], "%4.1f%%", CHeapMan::getUsedHeapRatio());
    sprintf(_szBuf[5], "%d/", CModelMan::getDrawCount());
    sprintf(_szBuf[6], "%d", CModelMan::getModelCount());
    sprintf(_szBuf[7], "%d", CModelMan::getTexSendCount(0));
    sprintf(_szBuf[8], "%d", CModelMan::getTexSendCount(1));
    sprintf(_szBuf[9], "%d", CMainApp::getObjCount());
    CModelMan::clearDrawCount();
    CModelMan::clearTexSendCount(0);
    CModelMan::clearTexSendCount(1);

    _ptlCountBack->disp();
    for (i = 0; i < 10; i++) {
        if (_pStr[i]) _pStr[i]->disp();
    }

    nFloor = CSceneInfo::getCurrentFloorNumber();
    nArea = CSceneInfo::getCurrentAreaNum();
    nScene = CSceneInfo::getCurrentSceneNum();
    nRoom = CSceneInfo::getCurrentRoomID();
    nMap = CGame::getMapID();
    if (dbgCheckUser(USR_PROG)) {
        INT nUse = CPackMan::getPkUseSize();
        INT nTotal = CPackMan::getPkTotalSize();
        sprintf(_szASM, "F:%02d W:%02d S:%04d R:%02d M:%04d\nPkt:%04Xqw / %04Xqw %10d", nFloor, nArea, nScene,
                nRoom, nMap, nUse, nTotal, CMainApp::getMainCount() / 50);
        _setXYZ(_pgptASM->m_P1, 244 << 4, (42 - SCREEN_HEIGHT / 2) << 4, 0xFFFF28 << 4);
    } else {
        sprintf(_szASM, "F:%02d W:%02d S:%04d R:%02d M:%04d\n", nFloor, nArea, nScene, nRoom, nMap);
        _setXYZ(_pgptASM->m_P1, 244 << 4, (26 - SCREEN_HEIGHT / 2) << 4, 0xFFFF28 << 4);
    }
    _pgptASM->disp();
    if (_pfntASM) _pfntASM->disp();
}

void CDebugProcInfo::changeProcCounterColor(BYTE r, BYTE g, BYTE b) {
    INT i;
    for (i = 0; i < 10; i++) {
        CFnt* pStr = _pStr[i];
        if (pStr) _setColor(pStr->m_C0, r, g, b);
    }
}

void CDebugProcInfo::dispPadInfo() {
    BOOL bError;
    UINT nPad;
    UINT nBit;
    CHAR* pDst;

    if (DebugWork::g_dw.sys.m_iDispCounter) return;

    bError = CPadIO::isStandby(0);
    nPad = CPadIO::getButtonDown(0);
    pDst = _szPAD;
    nBit = 0x800000;
    do {
        *pDst++ = bError ? 'E' : ((nPad & nBit) ? 'O' : 'x');
        nBit >>= 1;
        if (nBit & 0x88888888) *pDst++ = ' ';
    } while (nBit);
    *pDst++ = ' ';
    *pDst++ = bError ? 'E' : (CPadIO::getVibMotorL(0) ? 'L' : 'x');
    *pDst++ = bError ? 'E' : (CPadIO::getVibMotorS(0) ? 'S' : 'x');
    *pDst = 0;
    if (_pgptPAD) _pgptPAD->disp();
    if (_pfntPAD) _pfntPAD->disp();
}
// SOURCE DATA: .rodata .bss
