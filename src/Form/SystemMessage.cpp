#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/TaskMan.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Form/Menu/SaveLoadCtrl.h"
#include "System/Ctd/CtdMan.h"
#include "System/Graphic/2d/2DFnt.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "Form/SystemMessage.h"
#include <string.h>

extern "C" int sprintf(char* buf, const char* fmt, ...);

#include "System/Rcf.h"





#include "System/RsrcDB.h"
#include "System/HeapMan.h"




inline void* operator new(size_t, void* p) throw() { return p; }

namespace {
C2DSeqAgent* s_pSysWindowAgent = NULL;
D2::C2DSeqCtrl* s_pSysWindowCtrl = NULL;
UINT s_nSysWindowFlag = 0;
void (*s_pExtEndHandler)() = NULL;
char s_szSysMessage[256];
}


static void _openWnd();
static void _callbackSysMessageLoop(C2DSeqAgent& agent);
static void _callbackSysMessageClosed(C2DSeqAgent& agent);
static void _createWindow();
static void _setVisibleContents(BOOL bOn);

void CSystemMessage::initialize() {
    s_nSysWindowFlag = 0;
    _createWindow();
}

void CSystemMessage::finalize() {
    delete s_pSysWindowAgent;
    s_pSysWindowAgent = NULL;
    delete s_pSysWindowCtrl;
    s_pSysWindowCtrl = NULL;
    s_nSysWindowFlag = 0;
    s_pExtEndHandler = NULL;
}

void CSystemMessage::setupRsrc() {
    CRcf::reqLoadRCF(2, 0x208, 0, NULL, 0);
}

BOOL CSystemMessage::isRsrcSetupComplete() {
    return CRcf::isCompleteLoadRCF(2, 0x208);
}

void CSystemMessage::disposeRsrc() {
    CRsrcDB::removeGroup(2, 0x208);
}

void CSystemMessage::setEndHandler(void (*pExtEndHandler)()) {
    s_pExtEndHandler = pExtEndHandler;
}

static void _openWnd() {
    C2DSeqAgent* pAgent = s_pSysWindowAgent;
    if (pAgent != NULL) {
        if (pAgent->isNotStarted()) {
            pAgent->start();
        } else {
            pAgent->changeState(C2DSeqAgent::STATE_IN, TRUE, -1);
        }
        s_nSysWindowFlag |= 1;
        _setVisibleContents(FALSE);
    }
}

void CSystemMessage::open(const CHAR* pszMsg) {
    strcpy(s_szSysMessage, pszMsg);
    _openWnd();
}

void CSystemMessage::open(SYSMSG_ID id) {
    CHAR* pszText;
    CHAR* pszFmt;

    s_szSysMessage[0] = '\0';
    pszText = CCtdMan::getText(0x100000);
    if (pszText == NULL) pszText = "";
    switch (id) {
    case SYSMSG_ID_NO_PAD:
        pszFmt = CCtdMan::getText(0x100100);
        if (pszFmt == NULL) pszFmt = "";
        strcpy(s_szSysMessage, pszFmt);
        break;
    case SYSMSG_ID_OPENTRAY:
        pszFmt = CCtdMan::getText(0x100101);
        if (pszFmt == NULL) pszFmt = "";
        sprintf(s_szSysMessage, pszFmt, pszText);
        break;
    }
    _openWnd();
}

void CSystemMessage::close() {
    if (s_pSysWindowAgent != NULL) {
        s_pSysWindowAgent->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
        _setVisibleContents(FALSE);
    }
}

BOOL CSystemMessage::isOpened() {
    return s_pSysWindowAgent != NULL && s_pSysWindowAgent->getState() < C2DSeqAgent::STATE_OUT;
}

BOOL CSystemMessage::isClosed() {
    return !(s_nSysWindowFlag & 1);
}

static void _callbackSysMessageLoop(C2DSeqAgent& agent) {
    _setVisibleContents(TRUE);
}

static void _callbackSysMessageClosed(C2DSeqAgent& agent) {
    s_nSysWindowFlag &= ~1;
    if (s_pExtEndHandler != NULL) s_pExtEndHandler();
}

#line 205
static void _createWindow() {
    static CHAR* s_pszSysWndSeq = "syswindow.sqd";
    C2DSeqAgent::SeqTbl tbl = {{2, 3, 4}};
    G_RGBA rgba;
    D2::C2DFnt* pFnt;

    /* through a reference: keeps the store before the virtual call (scheduling) */
    D2::C2DSeqCtrl*& rCtrl = s_pSysWindowCtrl;
    rCtrl = new (__FILE__ "(213)") D2::C2DSeqCtrl;
    rCtrl->addSeq(s_pszSysWndSeq, tbl._tbl[0]);
    s_pSysWindowCtrl->changeLayer(25);
    rgba.col = 0x80707070;
    pFnt = new (__FILE__ "(220)") D2::C2DFnt(0, 0, &rgba, 15, s_szSysMessage, 0x46);
    pFnt->m_Space.x = 0;
    pFnt->m_Space.y = 0x30;
    s_szSysMessage[0] = '\0';
    s_pSysWindowCtrl->addFntShift(s_pszSysWndSeq, pFnt, 0.0f, -8.0f);
    s_pSysWindowCtrl->addSeqShift(s_pszSysWndSeq, 0, -150.0f, -36.0f);
    rgba.col = 0x80000070;
    pFnt = new (__FILE__ "(229)") D2::C2DFnt(0, 0, &rgba, 15, "INFORMATION", 0x40);
    s_pSysWindowCtrl->addFntShift(s_pszSysWndSeq, pFnt, -120.0f, -40.0f);
    s_pSysWindowCtrl->enableColorCtrl(TRUE);
    s_pSysWindowAgent = new (__FILE__ "(233)") C2DSeqAgent(CTaskMan::getSysTask(0));
    s_pSysWindowAgent->setupSeq(*s_pSysWindowCtrl, s_pszSysWndSeq, 0);
    s_pSysWindowAgent->setupState(tbl._tbl[0], tbl._tbl[1], tbl._tbl[2], _callbackSysMessageLoop,
                                  _callbackSysMessageClosed);
    s_pSysWindowAgent->ignorePause(TRUE);
}

static void _setVisibleContents(BOOL bOn) {
    if (s_pSysWindowCtrl != NULL) {
        s_pSysWindowCtrl->enableDisp(1, bOn);
        s_pSysWindowCtrl->enableDisp(2, bOn);
        s_pSysWindowCtrl->enableDisp(3, bOn);
    }
}
// SOURCE DATA: .data .rodata .bss
