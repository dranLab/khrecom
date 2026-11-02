
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/Anim.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in GameOver). */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() {
    TRACE("");
}
inline void _emptyStringPlaceholder2() {
    TRACE("");
}

#include "Game/Status.h"
#include "Game/Technic.h"
#include "System/Graphic/Model.h"
#include "Form/Menu/MonstroGauge.h"
#include "System/HeapMan.h"
#include "System/Vector.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DSeqObj.h"
#include "Game/GameWork.h"

inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
#define NEW(line, T) new (com_malloc_d(sizeof(T), __FILE__ "(" #line ")", RET_ADDR())) T











#include "System/Sound/SoundMan.h"
#line 50

using namespace D2;

CMostroGauge::CMostroGauge(CTask* p) : CTask(p, 0x4000) {
    m_bTimeStop = TRUE;
    m_pGaugeCtrl = NULL;
    m_pTimeCtrl = NULL;
    m_nStep = 0;
    m_bEnd = FALSE;
    m_bTimeOver = FALSE;
    m_qTime = 0;
    m_fLife = 0;
    m_fLifeTarget = 0;
    m_fLife2 = 0;
    m_hHelpLay = 0;
}

CMostroGauge::~CMostroGauge() {
    C2DMan::destroy(m_hHelpLay);
    delete m_pGaugeCtrl;
    m_pGaugeCtrl = NULL;
    delete m_pTimeCtrl;
    m_pTimeCtrl = NULL;
}

void CMostroGauge::main() {
    LPCTSTR pSeqName = "monstro_gauge:00";
    LPCTSTR pMEBLayName = "gamehelp.lad";
    FVECTOR vec;
    FVECTOR vec2;
    C2DSeqObj* p2DSeqObj;

    switch (m_nStep) {
    case 0:
        break;
    case 1:
        vec._q = 0;
        vec.w = 1.0f;
#line 79
        m_pGaugeCtrl = NEW(79, C2DSeqCtrl);
        m_pGaugeCtrl->enableColorCtrl(TRUE);
        m_pGaugeCtrl->setPos(vec);
        p2DSeqObj = m_pGaugeCtrl->addSeq(pSeqName, 1);
        p2DSeqObj->changeLayer(4);
        p2DSeqObj = m_pGaugeCtrl->addSeq(pSeqName, 4);
        p2DSeqObj->changeLayer(3);
        p2DSeqObj = m_pGaugeCtrl->addSeq(pSeqName, 0);
        p2DSeqObj->changeLayer(3);
        p2DSeqObj->changeTimeExt(0);
        p2DSeqObj = m_pGaugeCtrl->addSeq(pSeqName, 0);
        p2DSeqObj->changeLayer(3);
        p2DSeqObj->changeTimeExt(0);
        p2DSeqObj = m_pGaugeCtrl->addSeq(pSeqName, 5);
        p2DSeqObj->changeLayer(2);
#line 96
        m_pTimeCtrl = NEW(96, C2DSeqCtrl);
        m_pTimeCtrl->setLayer(2);
        m_pTimeCtrl->enableColorCtrl(TRUE);
        m_pTimeCtrl->setPos(vec);
        m_pTimeCtrl->addSeq(pSeqName, 7);
        setTime(TRUE);
        m_nStep = 2;
        break;
    case 2:
        if (m_pGaugeCtrl->isEnd(0)) {
            m_pGaugeCtrl->replaceSeq(0, pSeqName, 2);
            m_pTimeCtrl->replaceSeq(0, pSeqName, 8);
            m_nStep = 3;
        }
        break;
    case 3:
        break;
    case 4:
        m_pGaugeCtrl->replaceSeq(0, pSeqName, 3);
        m_pTimeCtrl->replaceSeq(0, pSeqName, 9);
        m_nStep = 5;
        break;
    case 5:
        if (m_pGaugeCtrl->isEnd(0)) {
            m_nStep = 10;
        }
        break;
    case 6:
        C2DMan::setLayer(13);
        m_hHelpLay = C2DMan::create(pMEBLayName, 0);
        CSound::playSE(3, -1, 64);
        m_nStep = 7;
        break;
    case 7:
        if (C2DMan::isEnd(m_hHelpLay)) {
            m_nStep = 8;
        }
        break;
    case 8:
        if (isTrgOK(0) || isTrgCANCEL(0)) {
            C2DMan::destroy(m_hHelpLay);
            C2DMan::setLayer(13);
            m_hHelpLay = C2DMan::create(pMEBLayName, 1);
            CSound::playSE(4, -1, 64);
            m_nStep = 9;
        }
        break;
    case 9:
        if (C2DMan::isEnd(m_hHelpLay)) {
            C2DMan::destroy(m_hHelpLay);
            CGameWork::setInt(1, 0xE2, 0);
            m_nStep = 0;
        }
        break;
    case 10:
    default:
        out();
        break;
    }
    setLife();
    setTime(FALSE);
}

void CMostroGauge::setLife() {
    if (m_pGaugeCtrl) {
        FLOAT fRate = getFrameRateF();
        if (m_fLife != m_fLifeTarget) {
            if (m_fLifeTarget < m_fLife) {
                m_fLife -= fRate * 0.25f;
                if (m_fLife < m_fLifeTarget) {
                    m_fLife = m_fLifeTarget;
                }
            } else {
                m_fLife += fRate * 0.5f;
                if (m_fLifeTarget < m_fLife) {
                    m_fLife = m_fLifeTarget;
                }
            }
            m_pGaugeCtrl->changeTime(2, (UINT)m_fLife);
        }
        m_fLife2 += fRate;
        if (m_fLife < m_fLife2) {
            m_fLife2 = 0;
        }
        m_pGaugeCtrl->changeTime(3, (INT)m_fLife2);
    }
}

void CMostroGauge::setTime(BOOL bInit) {
    LPCTSTR pSeqName;
    int nNumSeq[6];

    if (bInit) {
        m_qTime = m_qTargetTime;
    } else {
        if (m_nStep != 3 || !m_pGaugeCtrl || m_bTimeStop) {
            return;
        }
        m_qTime -= getFrameRate();
        if (m_qTime < 0) {
            m_qTime = 0;
            m_bTimeOver = TRUE;
        }
    }
    INT nFrac = (m_qTime % 3000) / 30;
    INT nSec = (m_qTime % 180000) / 3000;
    INT nMin = m_qTime / 180000;
    nNumSeq[0] = nFrac % 10 + 10;
    nNumSeq[1] = nFrac / 10 + 10;
    nNumSeq[2] = nSec % 10 + 10;
    nNumSeq[3] = nSec / 10 + 10;
    nNumSeq[4] = nMin % 10 + 10;
    pSeqName = "monstro_gauge:00";
    if (bInit) {
        m_pTimeCtrl->addSeqShift(pSeqName, nNumSeq[0], 120.0f, 0.0f);
        m_pTimeCtrl->addSeqShift(pSeqName, nNumSeq[1], 104.0f, 0.0f);
        m_pTimeCtrl->addSeqShift(pSeqName, nNumSeq[2], 80.0f, 0.0f);
        m_pTimeCtrl->addSeqShift(pSeqName, nNumSeq[3], 64.0f, 0.0f);
        m_pTimeCtrl->addSeqShift(pSeqName, nNumSeq[4], 40.0f, 0.0f);
    } else {
        m_pTimeCtrl->replaceSeqShift(1, pSeqName, nNumSeq[0], 120.0f, 0.0f);
        m_pTimeCtrl->replaceSeqShift(2, pSeqName, nNumSeq[1], 104.0f, 0.0f);
        m_pTimeCtrl->replaceSeqShift(3, pSeqName, nNumSeq[2], 80.0f, 0.0f);
        m_pTimeCtrl->replaceSeqShift(4, pSeqName, nNumSeq[3], 64.0f, 0.0f);
        m_pTimeCtrl->replaceSeqShift(5, pSeqName, nNumSeq[4], 40.0f, 0.0f);
    }
}

BOOL CMostroGauge::exec(EXEC_CMD nCmd, INT nParam) {
    BOOL ret = TRUE;

    switch (nCmd) {
    case GAUGE_IN:
        if (m_nStep == 0) {
            m_qTargetTime = nParam;
            m_nStep = 1;
        } else {
            ret = FALSE;
        }
        break;
    case TIME_START:
        if (m_nStep == 3) {
            m_bTimeStop = FALSE;
        } else {
            ret = FALSE;
        }
        break;
    case TIME_STOP:
        if (m_nStep == 3) {
            m_bTimeStop = TRUE;
        } else {
            ret = FALSE;
        }
        break;
    case GAUGE_OUT:
        m_nStep = 4;
        break;
    case SET_LIFE:
        if (m_pGaugeCtrl) {
            if (nParam < 0) {
                nParam = 0;
            }
            if ((INT)m_fLifeTarget < nParam) {
                C2DSeqObj* p2DSeqObj = m_pGaugeCtrl->replaceSeq(4, "monstro_gauge:00", 6);
                p2DSeqObj->changeLayer(2);
            }
            m_fLifeTarget = nParam;
        } else {
            ret = FALSE;
        }
        break;
    case IS_TIMEOVER:
        ret = m_bTimeOver;
        break;
    case HELP_IN:
        if (m_nStep == 0) {
            m_nStep = 6;
        } else {
            ret = FALSE;
        }
        break;
    default:
        ret = FALSE;
        break;
    }
    return ret;
}
// SOURCE DATA: .rodata
