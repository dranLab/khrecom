#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define TITLEREPORTDLLSTARTER_H_PATH "c:/KHCOM/MainApp/Form/Menu/TitleReportDllStarter.h" /* MATCH: this unit names TitleReportDllStarter.h so (header default "Form/Menu/TitleReportDllStarter.h") */
#define SAVELOADCTRL_H_PATH "System/../Form/Menu/SaveLoadCtrl.h" /* MATCH: this unit names SaveLoadCtrl.h so (header default "Form/Menu/SaveLoadCtrl.h") */
#include <string.h>

#include "System/TaskMan.h"
#include "System/Task.h"
#include "Form/Menu/TitleReportDllStarter.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Form/Menu/SaveLoadCtrl.h"
#include "System/Fsm.h"

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

#include "System/RsrcDB.h"




#include "System/Rcf.h"
#include "System/HeapMan.h"





/* The battle report and title tasks live in a DLL: their constructors and static flags are not linked into the
 * main program (the original refers to address 0). Sizes from the allocations. */
namespace BattleReport {
class CBtlReport : public CTask {
    BYTE _pad[0x2E0 - sizeof(CTask)];
public:
    static BYTE m_EndFlag;
    CBtlReport(CTask* pParent, INT nParam);
};
}
class CTitle : public CTask {
    BYTE _pad[0x1A0 - sizeof(CTask)];
public:
    static BYTE m_EndFlag;
    CTitle(CTask* pParent);
};

/* Ps2EeAs makes undefined weak symbols global (the link would fail), so define them as weak absolute 0. */
__asm__(
    ".weak __Q212BattleReport10CBtlReportP5CTaski\n__Q212BattleReport10CBtlReportP5CTaski = 0\n"
    ".weak __6CTitleP5CTask\n__6CTitleP5CTask = 0\n"
    ".weak _Q212BattleReport10CBtlReport$m_EndFlag\n_Q212BattleReport10CBtlReport$m_EndFlag = 0\n"
    ".weak _6CTitle$m_EndFlag\n_6CTitle$m_EndFlag = 0\n");

#line 20 "c:/KHCOM/MainApp/Form/Menu/TitleReportDllStarter.cpp"
CTitleReportDllStarter* CTitleReportDllStarter::m_pInstance = NULL;

CTitleReportDllStarter::CTitleReportDllStarter(CTask* pParent, INT in_LaunchMode, INT in_nParam)
    : CTask(pParent, 0x8000) {
    m_State[0].set(this, &CTitleReportDllStarter::beginStateInit, &CTitleReportDllStarter::stateInit,
                   &CTitleReportDllStarter::endStateInit);
    m_State[1].set(this, &CTitleReportDllStarter::beginStateDllLoad, &CTitleReportDllStarter::stateDllLoad,
                   &CTitleReportDllStarter::endStateDllLoad);
    m_State[2].set(this, &CTitleReportDllStarter::beginStateMain, &CTitleReportDllStarter::stateMain,
                   &CTitleReportDllStarter::endStateMain);
    changeState(0);
    m_LaunchMode = in_LaunchMode;
    m_nParam = in_nParam;
    m_hChildTask = 0;
}

CTitleReportDllStarter::~CTitleReportDllStarter() {
    if (m_hChildTask) deleteChild(m_hChildTask);
    TRACE("CTitleReportDllStarter::~CTitleReportDllStarter()\n");
    CRsrcDB::removeGroup(0, 200);
    m_pInstance = NULL;
}

void CTitleReportDllStarter::main() {
    update();
}

void CTitleReportDllStarter::disp() {}

void CTitleReportDllStarter::beginStateInit() {}

void CTitleReportDllStarter::stateInit() {
    changeState(1);
}

void CTitleReportDllStarter::endStateInit() {}

void CTitleReportDllStarter::beginStateDllLoad() {
    CRcf::reqLoadRCF(0, 200, 0x100, NULL, 0);
}

void CTitleReportDllStarter::stateDllLoad() {
    if (CRcf::isCompleteLoadRCF(0, 200)) {
        switch (m_LaunchMode) {
        case 0:
#line 126
            m_hChildTask = (new (__FILE__ "(126)")
                                BattleReport::CBtlReport(this, m_nParam))->getHandle();
            break;
        case 1:
            break;
        case 2:
#line 128
            new (__FILE__ "(128)") CTitle(this);
            break;
        }
        changeState(2);
    }
}

void CTitleReportDllStarter::endStateDllLoad() {}

void CTitleReportDllStarter::beginStateMain() {}

void CTitleReportDllStarter::stateMain() {
    switch (m_LaunchMode) {
    case 0:
        if (BattleReport::CBtlReport::m_EndFlag) out();
        break;
    case 1:
        break;
    case 2:
        if (CTitle::m_EndFlag) {
            CMainApp::reqSoftReset(SOFTRESET_SKIP_LOGO);
            out();
        }
        break;
    }
}

void CTitleReportDllStarter::endStateMain() {}

void CTitleReportDllStarter::changeState(INT state) {
#line 186
    ASSERT((0 <= state) && (state < STATE_MAX));
    TRACE_USER(KYOSHIOK, "StateChange:--->%d\n", state);
    gotoState(m_State[state]);
}

// SOURCE DATA: .data .rodata
