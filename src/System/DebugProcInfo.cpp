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





static inline void _setFntPos(CFntM2* p, INT x, INT y) {
    p->m_P0.set((x / 2) << 4, y << 4, 0xFFFF28 << 4);
}

static inline void _setFntXY(CFntM2* p, INT x, INT y) {
    p->m_P0.set(x << 4, y << 4, 0xFFFF28 << 4);
}







void CDebugProcInfo::initialize() {
    KH_DEBUG(INT i;)
    KH_DEBUG(for (i = 0; i < 10; i++) {)
        KH_DEBUG(CFnt*& pStr = _pStr[i];)
        KH_DEBUG(pStr = new KH_AT("(64)") CFnt;)
        KH_DEBUG(if (pStr) {)
            KH_DEBUG(_szBuf[i][0] = 0;)
            KH_DEBUG(pStr->setText(_szBuf[i]);)
            KH_DEBUG(pStr->m_Size = 14 << 4;)
            KH_DEBUG(if (i >= 5) {)
                KH_DEBUG(_setFntPos(pStr, (i - 5) * 70 - 140, _ref(SCREEN_HEIGHT) / 2 - 10);)
            KH_DEBUG(} else {)
                KH_DEBUG(_setFntPos(pStr, i * 70 - 140, _ref(SCREEN_HEIGHT) / 2 - 24);)
            KH_DEBUG(})
            KH_DEBUG(pStr->setStyle(1);)
            KH_DEBUG(pStr->m_C0.set(0x60, 0x60, 0);)
            KH_DEBUG(pStr->setLayerFG(24);)
        KH_DEBUG(})
    KH_DEBUG(})
    KH_DEBUG(if (_pStr[4]) _pStr[4]->m_P0.x += 7 << 4;)

    KH_DEBUG(_ref(_ptlCountBack) = new KH_AT("(84)") CTile;)
    KH_DEBUG(_ref(_ptlCountBack)->m_P0.set(-104 << 4, (_ref(SCREEN_HEIGHT) / 2 - 38) << 4);)
    KH_DEBUG(_ref(_ptlCountBack)->m_P1.set(78 << 4, (_ref(SCREEN_HEIGHT) / 2 - 8) << 4);)
    KH_DEBUG(_ref(_ptlCountBack)->m_C0.set(0, 0, 0, 0x50);)
    KH_DEBUG(_ref(_ptlCountBack)->setSemiTrans(ALPHA_BLEND);)
    KH_DEBUG(_ref(_ptlCountBack)->setLayerFG(24);)

    KH_DEBUG(_ref(_pfntASM) = new KH_AT("(92)") CFnt;)
    KH_DEBUG(_ref(_pfntASM)->setText(_szASM);)
    KH_DEBUG(_setFntXY(_ref(_pfntASM), 240, 24 - _ref(SCREEN_HEIGHT) / 2);)
    KH_DEBUG(_ref(_pfntASM)->setStyle(1);)
    KH_DEBUG(_ref(_pfntASM)->m_C0.set(0x60, 0x60, 0);)
    KH_DEBUG(_ref(_pfntASM)->setLayerFG(24);)
    KH_DEBUG(_ref(_pfntASM)->m_Size = 12 << 4;)
    KH_DEBUG(_ref(_pfntASM)->m_Space.y = 32;)

    KH_DEBUG(_ref(_pgptASM) = new KH_AT("(101)") CTile;)
    KH_DEBUG(_ref(_pgptASM)->m_C0.set(0, 0, 0, 0x50);)
    KH_DEBUG(_ref(_pgptASM)->setSemiTrans(ALPHA_BLEND);)
    KH_DEBUG(_ref(_pgptASM)->setLayerFG(24);)
    KH_DEBUG(_ref(_pgptASM)->m_P0.set(60 << 4, (10 - _ref(SCREEN_HEIGHT) / 2) << 4);)
    KH_DEBUG(_ref(_pgptASM)->m_P1.set(244 << 4, (24 - _ref(SCREEN_HEIGHT) / 2) << 4);)

    KH_DEBUG(_ref(_pfntPAD) = new KH_AT("(109)") CFnt;)
    KH_DEBUG(_ref(_pfntPAD)->setText(_szPAD);)
    KH_DEBUG(_setFntXY(_ref(_pfntPAD), -46, 24 - _ref(SCREEN_HEIGHT) / 2);)
    KH_DEBUG(_ref(_pfntPAD)->setStyle(2);)
    KH_DEBUG(_ref(_pfntPAD)->m_C0.set(0x60, 0x60, 0);)
    KH_DEBUG(_ref(_pfntPAD)->setLayerFG(24);)
    KH_DEBUG(_ref(_pfntPAD)->m_Size = 12 << 4;)

    KH_DEBUG(_ref(_pgptPAD) = new KH_AT("(117)") CTile;)
    KH_DEBUG(_ref(_pgptPAD)->m_C0.set(0, 0, 0, 0x50);)
    KH_DEBUG(_ref(_pgptPAD)->setSemiTrans(ALPHA_BLEND);)
    KH_DEBUG(_ref(_pgptPAD)->setLayerFG(24);)
    KH_DEBUG(_ref(_pgptPAD)->m_P0.set(-145 << 4, (10 - _ref(SCREEN_HEIGHT) / 2) << 4);)
    KH_DEBUG(_ref(_pgptPAD)->m_P1.set(53 << 4, (26 - _ref(SCREEN_HEIGHT) / 2) << 4);)
}

void CDebugProcInfo::finalize() {
    KH_DEBUG(INT i;)
    KH_DEBUG(for (i = 0; i < 10; i++) {)
        KH_DEBUG(delete _pStr[i];)
        KH_DEBUG(_pStr[i] = NULL;)
    KH_DEBUG(})
    KH_DEBUG(delete _ptlCountBack;)
    KH_DEBUG(_ptlCountBack = NULL;)
    KH_DEBUG(delete _pfntASM;)
    KH_DEBUG(_pfntASM = NULL;)
    KH_DEBUG(delete _pgptASM;)
    KH_DEBUG(_pgptASM = NULL;)
    KH_DEBUG(delete _pfntPAD;)
    KH_DEBUG(_pfntPAD = NULL;)
    KH_DEBUG(delete _pgptPAD;)
    KH_DEBUG(_pgptPAD = NULL;)
}

void CDebugProcInfo::dispProcInfo() {
    KH_DEBUG(INT nFloor;)
    KH_DEBUG(INT nArea;)
    KH_DEBUG(INT nScene;)
    KH_DEBUG(INT nRoom;)
    KH_DEBUG(INT nMap;)
    KH_DEBUG(INT i;)

    KH_DEBUG(if (DebugWork::g_dw.sys.m_iDispCounter) return;)

    KH_DEBUG(sprintf(_szBuf[0], "%d", CMainApp::getProcCount());)
    KH_DEBUG(sprintf(_szBuf[1], "%d", CMainApp::getDrawCount());)
    KH_DEBUG(sprintf(_szBuf[2], "%d", CMainApp::getDraw2Count());)
    KH_DEBUG(sprintf(_szBuf[3], "%d", CMainApp::getProcCountMax());)
    KH_DEBUG(sprintf(_szBuf[4], "%4.1f%%", CHeapMan::getUsedHeapRatio());)
    KH_DEBUG(sprintf(_szBuf[5], "%d/", CModelMan::getDrawCount());)
    KH_DEBUG(sprintf(_szBuf[6], "%d", CModelMan::getModelCount());)
    KH_DEBUG(sprintf(_szBuf[7], "%d", CModelMan::getTexSendCount(0));)
    KH_DEBUG(sprintf(_szBuf[8], "%d", CModelMan::getTexSendCount(1));)
    KH_DEBUG(sprintf(_szBuf[9], "%d", CMainApp::getObjCount());)
    KH_DEBUG(CModelMan::clearDrawCount();)
    KH_DEBUG(CModelMan::clearTexSendCount(0);)
    KH_DEBUG(CModelMan::clearTexSendCount(1);)

    KH_DEBUG(_ptlCountBack->disp();)
    KH_DEBUG(for (i = 0; i < 10; i++) {)
        KH_DEBUG(if (_pStr[i]) _pStr[i]->disp();)
    KH_DEBUG(})

    KH_DEBUG(nFloor = CSceneInfo::getCurrentFloorNumber();)
    KH_DEBUG(nArea = CSceneInfo::getCurrentAreaNum();)
    KH_DEBUG(nScene = CSceneInfo::getCurrentSceneNum();)
    KH_DEBUG(nRoom = CSceneInfo::getCurrentRoomID();)
    KH_DEBUG(nMap = CGame::getMapID();)
    KH_DEBUG(if (dbgCheckUser(USR_PROG)) {)
        KH_DEBUG(INT nUse = CPackMan::getPkUseSize();)
        KH_DEBUG(INT nTotal = CPackMan::getPkTotalSize();)
        KH_DEBUG(sprintf(_szASM, "F:%02d W:%02d S:%04d R:%02d M:%04d\nPkt:%04Xqw / %04Xqw %10d", nFloor, nArea, nScene, nRoom, nMap, nUse, nTotal, CMainApp::getMainCount() / 50);)

        KH_DEBUG(_pgptASM->m_P1.set(244 << 4, (42 - SCREEN_HEIGHT / 2) << 4, 0xFFFF28 << 4);)
    KH_DEBUG(} else {)
        KH_DEBUG(sprintf(_szASM, "F:%02d W:%02d S:%04d R:%02d M:%04d\n", nFloor, nArea, nScene, nRoom, nMap);)
        KH_DEBUG(_pgptASM->m_P1.set(244 << 4, (26 - SCREEN_HEIGHT / 2) << 4, 0xFFFF28 << 4);)
    KH_DEBUG(})
    KH_DEBUG(_pgptASM->disp();)
    KH_DEBUG(if (_pfntASM) _pfntASM->disp();)
}

void CDebugProcInfo::changeProcCounterColor(BYTE r, BYTE g, BYTE b) {
    KH_DEBUG(INT i;)
    KH_DEBUG(for (i = 0; i < 10; i++) {)
        KH_DEBUG(CFnt* pStr = _pStr[i];)
        KH_DEBUG(if (pStr) pStr->m_C0.set(r, g, b);)
    KH_DEBUG(})
}

void CDebugProcInfo::dispPadInfo() {
    KH_DEBUG(BOOL bError;)
    KH_DEBUG(UINT nPad;)
    KH_DEBUG(UINT nBit;)
    KH_DEBUG(CHAR* pDst;)

    KH_DEBUG(if (DebugWork::g_dw.sys.m_iDispCounter) return;)

    KH_DEBUG(bError = CPadIO::isStandby(0);)
    KH_DEBUG(nPad = CPadIO::getButtonDown(0);)
    KH_DEBUG(pDst = _szPAD;)
    KH_DEBUG(nBit = 0x800000;)
    KH_DEBUG(do {)
        KH_DEBUG(*pDst++ = bError ? 'E' : ((nPad & nBit) ? 'O' : 'x');)
        KH_DEBUG(nBit >>= 1;)
        KH_DEBUG(if (nBit & 0x88888888) *pDst++ = ' ';)
    KH_DEBUG(} while (nBit);)
    KH_DEBUG(*pDst++ = ' ';)
    KH_DEBUG(*pDst++ = bError ? 'E' : (CPadIO::getVibMotorL(0) ? 'L' : 'x');)
    KH_DEBUG(*pDst++ = bError ? 'E' : (CPadIO::getVibMotorS(0) ? 'S' : 'x');)
    KH_DEBUG(*pDst = 0;)
    KH_DEBUG(if (_pgptPAD) _pgptPAD->disp();)
    KH_DEBUG(if (_pfntPAD) _pfntPAD->disp();)
}
// SOURCE DATA: .rodata .bss
