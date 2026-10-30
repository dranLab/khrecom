#include "System/Graphic/Gprim.h"

inline void* operator new(size_t, void* p) throw() { return p; }

#include "System/TaskMan.h"
#include "Game/NowLoading.h"

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

enum NAME_CASE {
    NAME_CASE_SENSITIVE = 0,
    NAME_CASE_UPPER = 1,
    NAME_CASE_LOWER = 2
};

class CRsrcData;

class CRsrcDB {
public:
    static void removeRsrc(INT nID);
    static CRsrcData* getFirstRsrc(INT nType, INT nID, INT nFlag);
    static struct TexInfo* getRsrcTex(const char* pName, NAME_CASE eCase);
};

#include "System/Rcf.h"





extern INT SCREEN_HEIGHT;

namespace {
CNowLoading* s_pInstance = NULL;
BOOL s_bEnable = 0;
INT s_nNL_Mode = 0;
}

CNowLoading::CNowLoading(CTask* pParent) : CTask(pParent, 0x8000) {
    ignorePause(TRUE);
    ignorePlaySpeed(TRUE);
    m_nSleepTimer = -1;
    m_nStep = 4;
    m_pSprite = NULL;
    m_nFlag = 0;
    /* read through a pointer: the original keeps this load after the member stores (no fixed-scalar alias exemption) */
    if (*(CNowLoading**)&s_pInstance == NULL) {
        s_pInstance = this;
    }
}

CNowLoading::~CNowLoading() {
    delete m_pSprite;
    if (this == s_pInstance) {
        CRsrcDB::removeRsrc(8000);
        s_pInstance = NULL;
    }
}

void CNowLoading::enable(BOOL bEnable) {
    s_bEnable = bEnable;
    if (!bEnable) {
        end(TRUE);
    }
}

BOOL CNowLoading::isEnable() {
    return s_bEnable;
}

void CNowLoading::setMode(INT nMode) {
    s_nNL_Mode = nMode;
}

INT CNowLoading::getMode() {
    return s_nNL_Mode;
}

void CNowLoading::start() {
    if (!s_pInstance) {
#line 121
        s_pInstance = new (com_malloc_d(sizeof(CNowLoading), __FILE__ "(121)", RET_ADDR()))
            CNowLoading(CTaskMan::getSysTask(0));
    }
    if (s_pInstance) {
        s_pInstance->wakeup();
    }
}

void CNowLoading::end(BOOL bRightNow) {
    if (s_pInstance) {
        s_pInstance->sleep(bRightNow);
    }
}

BOOL CNowLoading::hasRsrc() {
    return CRsrcDB::getFirstRsrc(2, 8000, 1) != NULL;
}

void CNowLoading::loadRsrc() {
    if (!hasRsrc() && CRcf::isCompleteLoadRCF(0, 8000) == TRUE) {
        CRcf::reqLoadRCFFront(0, 8000, 0x100, NULL, 0);
    }
}

void CNowLoading::main() {
    switch (m_nStep) {
    case 0:
        if (!CRcf::isCompleteLoadRCF(0, 8000)) {
            break;
        }
        if (!hasRsrc()) {
            break;
        }
        m_nStep = 1;
    case 1:
        setupSprite();
        m_nStep = 2;
        break;
    case 2:
        animSprite();
        runProc();
        break;
    case 3:
        animSprite();
        halfSleepProc();
        break;
    case 4:
        sleepProc();
        break;
    }
}

void CNowLoading::disp() {
    if (m_pSprite && m_nStep < 4 && m_nStep >= 2) {
        m_pSprite->disp();
    }
}

void CNowLoading::setupSprite() {
    if (!m_pSprite) {
#line 236
        m_pSprite = new (com_malloc_d(sizeof(CSprt), __FILE__ "(236)", RET_ADDR())) CSprt;
        if (!m_pSprite) AssertApp(__FILE__, __LINE__, "m_pSprite", 0);
    }
    m_nX = 208;
    m_nY = (*(INT*)&SCREEN_HEIGHT >> 1) - 48; /* through a pointer: load stays after the m_nX store */
    m_pSprite->setXY(m_nX << 4, m_nY << 4);
    m_pSprite->setSemiTrans(1);
    m_pSprite->setLayerFG(24);
    m_pSprite->m_C0.a = 0;
    m_pSprite->setTPage(CRsrcDB::getRsrcTex("load_icon_a", NAME_CASE_SENSITIVE));
    m_nTimer = 0;
    m_nSpritePtn = 0;
    setPtn();
}

void CNowLoading::setPtn() {
    if (m_pSprite) {
        m_pSprite->setUVWH((m_nSpritePtn & 3) * 32, (m_nSpritePtn >> 2) * 32, 31, 31);
    }
}

void CNowLoading::animSprite() {
    TIMEQ nWait = 150; /* a register operand at expand time: slt with 150 in a register */
    m_nTimer += getFrameRate();
    if (m_nTimer > nWait) {
        m_nTimer -= nWait;
        setPtn();
        if (++m_nSpritePtn >= 16) {
            m_nSpritePtn = 0;
        }
    }
}

void CNowLoading::sleep(BOOL bRightNow) {
    if (m_nStep != 4) {
        if (bRightNow) {
            m_nStep = 4;
            m_nSleepTimer = 1500;
        } else {
            m_nSleepTimer = 1000;
            m_nStep = 3;
        }
    }
}

void CNowLoading::wakeup() {
    if (s_bEnable) {
        switch (m_nStep) {
        case 3:
            m_nStep = 2;
            break;
        case 4:
            if (hasRsrc()) {
                m_nStep = 1;
            } else {
                loadRsrc();
                m_nStep = 0;
            }
            break;
        }
    }
}

void CNowLoading::runProc() {
    if (m_pSprite && m_pSprite->m_C0.a < 0x80) {
        m_pSprite->m_C0.a += 0x20;
        if (m_pSprite->m_C0.a > 0x80) {
            m_pSprite->m_C0.a = 0x80;
        }
    }
}

void CNowLoading::halfSleepProc() {
    if (m_nSleepTimer > 0) {
        m_nSleepTimer -= getFrameRate();
        if (m_nSleepTimer <= 0) {
            sleep(TRUE);
        } else if (m_pSprite) {
            m_pSprite->m_C0.a = (m_nSleepTimer * 128) / 1000;
        }
    }
}

void CNowLoading::sleepProc() {
    if (m_nSleepTimer > 0) {
        m_nSleepTimer -= getFrameRate();
        if (m_nSleepTimer <= 0) {
            out();
        }
    }
}
// SOURCE DATA: .data .rodata
