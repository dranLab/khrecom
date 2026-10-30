#define TASK_H_NO_SLC_OVERFLOW_MSG
#include "System/Graphic/Gprim.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
inline void _taskSLCOverflow() {
    TRACE("\x83\x5e\x83\x58\x83\x4e\x97\x70\x83\x52\x83\x93\x83\x65\x83\x69\x82\xaa MAX \x82\xc9\x93\x9e\x92\x42\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
}

inline void* operator new(size_t, void* p) throw() { return p; }

#include "Game/Atel/CameraMan.h"
#include "Game/Atel/AtelFog.h"
#include "Game/Atel/AtelBlur.h"
#include "Game/Atel/AtelCrossFade.h"
#include "Game/GameSub.h"
#include "System/TaskMan.h"

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

#include "Game/Atel/Atel.h"













#include "Game/SceneInfo.h"

class CRsrcDB { /* System/RsrcDB.h */
public:
    static unsigned char** getRsrcABC(INT nID);
};

#include "System/Rcf.h"




CGameSub::CGameSub(CStateTemplate<CGameSub>* pStateTbl) : m_pStateTbl(pStateTbl) {
    m_nNewStateID = -1;
    m_nStateID = -1;
    m_nMapID = 0;
    m_pAtel = NULL;
}

CGameSub::~CGameSub() {}

void CGameSub::changeState(INT nState) {
    m_nNewStateID = nState;
    gotoState(m_pStateTbl[nState]);
}

void CGameSub::endStateProc() {
    m_nStateID = m_nNewStateID;
    m_nNewStateID = -1;
}

#line 74
void CGameSub::preSetupAtel() {
    m_nAtelTimeOut = 300;
    if (!m_pAtel) {
        unsigned char** abc = CRsrcDB::getRsrcABC(CSceneInfo::getNextAtelSrcID());
        if (abc) {
            m_pAtel = new (com_malloc_d(sizeof(CAtel), __FILE__ "(79)", RET_ADDR())) CAtel(CTaskMan::getSysTask(1));
            if (!(m_pAtel)) AssertApp(__FILE__, __LINE__, "m_pAtel", 0);
            m_pAtel->loadFromMem(*abc);
            m_pAtel->setupActor();
        }
    }
    CAtel::clearScreenFader(1);
    if (!m_pAtel) CAtel::clearScreenFader(0);
    ATEL::CCameraMan::initialize(CTaskMan::getSysTask(1));
    ATEL::CAtelFog::initialize(CTaskMan::getSysTask(1));
    ATEL::CAtelBlur::initialize(CTaskMan::getSysTask(1));
    ATEL::CAtelCrossFade::initialize(CTaskMan::getSysTask(1));
}

BOOL CGameSub::setupAtel() {
    if (m_pAtel && !m_pAtel->canStart() && m_nAtelTimeOut > 0) {
        m_nAtelTimeOut--;
        return FALSE;
    }
    BOOL bComplete = CRcf::isCompleteLoadRCF(0, 0);
    if (bComplete && !m_nAtelTimeOut) {
        TRACE("\n******************************************************\n"
              "*                                                    *\n"
              "*       <<< Time Out... Give up ATEL-SETUP >>>       *\n"
              "*                                                    *\n"
              "******************************************************\n"
              "\x81i\x92\x8d\x81jATEL\x83\\\x81[\x83X\x82\xcc\x81w\x8f\x89\x8a\xfa\x89\xbb\x83t\x83" "F\x81[\x83Y\x81x\x82\xf0\x8am\x94" "F\x82\xb5\x82\xc4\x82\xad\x82\xbe\x82\xb3\x82\xa2\n\n");
    }
    return bComplete;
}

void CGameSub::startAtel() {
    if (m_pAtel) m_pAtel->start();
}

BOOL CGameSub::isAtelStarted() const {
    return m_pAtel && m_pAtel->isStarted();
}

void CGameSub::exitAtel() {
    if (m_pAtel) delete m_pAtel;
    m_pAtel = NULL;
    ATEL::CCameraMan::finalize();
    ATEL::CAtelFog::finalize();
    ATEL::CAtelBlur::finalize();
    ATEL::CAtelCrossFade::finalize();
}

BOOL CGameSub::isFadeActive() const {
    return m_pAtel && CAtel::isFadeActive();
}

void CGameSub::suspendAtel() {
    if (m_pAtel) m_pAtel->setActive(FALSE);
}

void CGameSub::resumeAtel() {
    if (m_pAtel) m_pAtel->setActive(TRUE);
}

// SOURCE DATA: .rodata
