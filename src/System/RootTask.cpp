#include "System/RootTask.h"
#include "System/TaskMan.h"
#include "System/Graphic/Gprim.h"



#include "Game/Atel/ChatMan.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/Effect/effect.h"
#include "System/PadIO.h"
#include "Form/DebugMenu.h"
#include "Form/SystemMessage.h"
#include "Game/SceneInfo.h"


#include "System/Rcf.h"






#include "System/Memoca.h"




#include "System/Graphic/ModelMan.h"

#include "System/Graphic/FrecFont.h"

#include "Game/SceneEnv.h"
#line 48

#include "System/Graphic/TexCtrl.h"






#include "Form/Menu/SaveLoad.h"




















#include "Form/Menu/SaveLoadUtil.h"
#include "System/HeapMan.h"

























#line 100 "c:/KHCOM/MainApp/System/RootTask.cpp"
namespace {
UINT s_nRebootCount = 0;
KH_DEBUG(BOOL s_bDbgSettingLoaded = 0;)
}

CRootTask::CRootTask() : CTask(NULL, 0x8000) {
    m_nPhase = 0;
    m_nStep = 0;
    declareSysTask(0);
    ignorePause(TRUE);
    ignorePlaySpeed(TRUE);
    _preparePhaseInit();
}

CRootTask::~CRootTask() {
    KH_DEBUG(if (CDebugMenu::getDebugParam()._nThisMenu == 1) {)
        KH_DEBUG(s_bDbgSettingLoaded = 0;)
    KH_DEBUG(})
    CSceneInfo::dispose();
    CSaveLoad::disposeRsrc();
}

void CRootTask::main() {
    switch (m_nPhase) {
    case 0:
        _phaseInit();
        break;
    case 1:
        _phaseChkPad();
        break;
    case 2:
        _phaseChkMemoca();
        break;
    case 3:
        _phaseMain();
        break;
    }
}

void CRootTask::_preparePhaseInit() {
    KH_DEBUG(if (!s_bDbgSettingLoaded) {)
        KH_DEBUG(CDebugMenu::initParam(-1);)
        KH_DEBUG(m_nStep = 0;)
    KH_DEBUG(} else {)
        m_nStep = 2;
    KH_DEBUG(})
    m_nPhase = 0;
}

void CRootTask::_phaseInit() {
    switch (m_nStep) {
    KH_DEBUG(case 0:)
        KH_DEBUG(if (CDebugMenu::startLoadFromMemoca()) {)
            KH_DEBUG(m_nStep = 1;)
        KH_DEBUG(})
        KH_DEBUG(break;)
    KH_DEBUG(case 1:)
        KH_DEBUG(if (CDebugMenu::isCompleteLoadFromMemoca()) {)
            KH_DEBUG(INT nLocale = CDebugMenu::getDebugParam()._nLocale;)
            KH_DEBUG(if (nLocale >= 0) {)
                KH_DEBUG(CRcf::setLocale(nLocale);)
            KH_DEBUG(})
            KH_DEBUG(s_bDbgSettingLoaded = 1;)
            KH_DEBUG(m_nStep = 2;)
        KH_DEBUG(})
        KH_DEBUG(break;)
    case 2:
        CMemoca::setupMcName(1);
        CRcf::reqLoadRCF(13, 1, 0, 0, 0);
        CRcf::reqLoadRCF(0, 1, 0, 0, 0);
        m_nStep = 3;
        break;
    case 3:
        if (CRcf::isCompleteLoadRCF(0, 0)) {
            CTask* pRootTop;
            CModelMan::reset();
            pRootTop = getSysTask(14);
#line 195
            new KH_AT("(195)") CTexCtrl(this);
            CFrecFont::setupFonts();
            CSceneInfo::clear();
            ATEL::CChatMan::initialize();
            CEffectMan::initialize(pRootTop);
            D2::C2DMan::initialize(pRootTop);
            CSceneEnv::initialize(this);
            CRcf::reqLoadRCF(0, 4, 0, 0, 0);
            m_nStep = 4;
        }
        break;
    case 4:
        if (CRcf::isCompleteLoadRCF(0, 0)) {
            CMainApp::enableWarningPause(1);
            KH_DEBUG(if (CPadIO::isButtonDown(0x40, 0)) {)
                KH_DEBUG(m_nStep = 5;)
            KH_DEBUG(} else {)
                _preparePhaseChkPad();
            KH_DEBUG(})
        }
        break;
    KH_DEBUG(case 5:)
        KH_DEBUG(_launchTitle(1);)
        break;
    }
}

void CRootTask::_preparePhaseChkPad() {
    m_nPhase = 1;
    m_nStep = 0;
}

void CRootTask::_phaseChkPad() {
    switch (m_nStep) {
    case 0:
        if (CPadIO::isConnected(0)) {
            _preparePhaseChkMemoca();
        } else {
            CSystemMessage::setupRsrc();
            m_nStep = 1;
        }
        break;
    case 1:
        if (CSystemMessage::isRsrcSetupComplete()) {
            CSystemMessage::initialize();
            CSystemMessage::open(SYSMSG_ID_NO_PAD);
            m_nStep = 2;
        }
        break;
    case 2:
        if (CPadIO::isConnected(0)) {
            CSystemMessage::close();
            m_nStep = 3;
        }
        break;
    case 3:
        if (CSystemMessage::isClosed()) {
            CSystemMessage::finalize();
            CSystemMessage::disposeRsrc();
            _preparePhaseChkMemoca();
        }
        break;
    }
}

void CRootTask::_preparePhaseChkMemoca() {
    UINT* pCount = &s_nRebootCount; /* read through a pointer: keeps the load after the member stores */
    m_nPhase = 2;
    m_nStep = 0;
    if (*pCount == 0) {
#line 317
        new KH_AT("(317)") CSaveLoad(this, SLMODE_BOOT, GAMECLEAR_NONE);
    } else {
        m_nStep = 1;
    }
    s_nRebootCount++;
}

void CRootTask::_phaseChkMemoca() {
    switch (m_nStep) {
    case 0:
        if (CSaveLoad::isExited()) {
            m_nStep = 1;
        }
        break;
    case 1:
#line 340
        m_pMemoca = new KH_AT("(340)") CMemocaAgent(this);
        KH_DEBUG(if (!(m_pMemoca)) AssertApp(__FILE__, __LINE__, "m_pMemoca", 0);)
        m_pMemoca->reqLoadSysData();
        m_nStep = 2;
        break;
    case 2:
        if (!m_pMemoca->isCompleteReq(NULL)) {
            break;
        }
        delete m_pMemoca;
        m_pMemoca = NULL;
        m_nStep = 5;
    case 5:
        _launchTitle(0);
        break;
    }
}

void CRootTask::_launchTitle(BOOL bDebug) {
    BOOL bLaunch;
    KH_DEBUG(if (bDebug) {)
        KH_DEBUG(bLaunch = CTaskMan::reqChangeMainTask(7, MAINTASK_REPLACE);)
    KH_DEBUG(} else {)
        bLaunch = CTaskMan::reqChangeMainTask(CMainApp::isSkipLogo() != 0, MAINTASK_REPLACE);
    KH_DEBUG(})
    if (bLaunch) {
        CMainApp::enableSoftReset(1);
        _preparePhaseMain();
    }
}

void CRootTask::_preparePhaseMain() {
    m_nPhase = 3;
    m_nStep = 0;
}

void CRootTask::_phaseMain() {
}

// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .data
