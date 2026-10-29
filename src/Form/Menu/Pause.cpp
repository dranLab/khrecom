#include "Form/Menu/Menu.h"
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Form/Menu/Pause.h"
#include "System/HeapMan.h"
#include "System/Vector.h"
#include "System/PadIO.h"
#include "System/TaskMan.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DFnt.h"
#include "Game/GameWork.h"

inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
#define NEW(line, T) new (com_malloc_d(sizeof(T), __FILE__ "(" #line ")", RET_ADDR())) T

namespace D2 {
class C2DMan {
public:
    enum D2MAN_LEVEL {
        D2MAN_LEVEL0 = 0,
        D2MAN_LEVEL1 = 1
    };
    static HANDLE create(const char* pName, UINT nIdx);
    static void destroy(HANDLE h);
    static BOOL isEnd(HANDLE h);
    static void setLayer(INT nLayer);
    static void changeLevel(D2MAN_LEVEL nLevel);
};
}

namespace ATEL {
class CVSM {
public:
    void AtelEventSkip();
};
}

#include "System/Graphic/PackMan.h"




#include "System/Sound/SoundMan.h"
#line 52

class CMinigameManager {
public:
    static BOOL reqDispHelp(CTask* pParent, INT nKind, INT nPage);
    static BOOL isEndDispHelp();
};

extern "C" void sceVu0InterVectorXYZ(float* v0, float* v1, float* v2, float t);
extern "C" float powf(float, float);

using namespace D2;

UINT CPause::m_nResult;

/* CDrawable::setLayer on the fonts, written through a USHORT pointer as in GameOver. */
static inline void _setFntLayer(C2DFnt* p, USHORT nID, USHORT nDepth) {
    ((USHORT*)p)[8] = nID;
    ((USHORT*)p)[9] = nDepth;
}

static inline void _initPos(FVECTOR& v) {
    v._q = 0;
    v.w = 1.0f;
}

CPause::CPause(CTask* p, INT nPauseKind) : CMenu(p, -1) {
    m_pLadName = "pause.lad";
    int selectMaxTbl[7] = {0, 2, 4, 2, 2, 2, 2};
    float cursorBaseY[7] = {0.0f, 185.0f, 177.0f, 233.0f, 185.0f, 185.0f, 185.0f};

    m_nSelectMax = selectMaxTbl[nPauseKind];
    m_fCursorBaseY = cursorBaseY[nPauseKind];
    m_nPauseKind = nPauseKind;
    m_bEventSkip = FALSE;
    m_pSelectCtrl = NULL;
    m_pCursorCtrl = NULL;
    m_pScreenCtrl = NULL;
    m_nSelect = 0;
    m_fgCursorMove = 0;
    m_nResult = 0;
    m_hHelpLay = 0;
    if (nPauseKind != 5) {
        m_saveObjBit = CSceneEnv::getStopAndRequestTypeBit();
        CSceneEnv::setStop(ObjType::OBJ_BIT_ALL);
    }
    CPadIO::suspendVibration();
}

CPause::~CPause() {
    dispose();
    C2DMan::changeLevel(C2DMan::D2MAN_LEVEL0);
    if (m_nPauseKind != 5) {
        if (!m_bEventSkip) {
            BOOL bResume = TRUE;
            if (m_nPauseKind == 2 || m_nPauseKind == 3) {
                if (m_nResult == 2 || m_nResult == 3) {
                    bResume = FALSE;
                }
            }
            if (bResume) {
                CSceneEnv::setStop(m_saveObjBit);
            }
        }
        CPackMan::changeDrawMode(0);
    }
    CPadIO::resumeVibration();
}

void CPause::dispose() {
    C2DMan::destroy(m_hHelpLay);
    m_hHelpLay = 0;
    delete m_pSelectCtrl;
    m_pSelectCtrl = NULL;
    delete m_pCursorCtrl;
    m_pCursorCtrl = NULL;
    delete m_pScreenCtrl;
    m_pScreenCtrl = NULL;
}

void CPause::main() {
    static LPCTSTR pSeqName = "pause:00";
    static LPCTSTR pMEBLayName = "gamehelp.lad";
    unsigned char bgSeqTbl[7] = {12, 13, 16, 24, 13, 13, 13};
    unsigned char selectSeqTbl[7][4] = {
        {0, 0, 0, 0}, {14, 15, 0, 0}, {17, 18, 19, 20}, {21, 22, 23, 0}, {14, 15, 0, 0}, {14, 15, 0, 0}, {14, 15, 0, 0},
    };
    static char* selectWordTbl[7][4] = {
        {NULL, NULL, NULL, NULL},
        {"Continue", "Skip Event", NULL, NULL},
        {"Continue", "Retry", "Help", "Quit"},
        {"Quit", "Retry", NULL, NULL},
        {"Continue", "Help", NULL, NULL},
        {"Continue", "Skip Event", NULL, NULL},
        {"Continue", "Return to Theater", NULL, NULL},
    };
    unsigned int resultTbl[7][4] = {
        {0, 0, 0, 0}, {0, 1, 0, 0}, {0, 2, 4, 3}, {3, 2, 0, 0}, {0, 5, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0},
    };
    FVECTOR pos;
    G_RGBA rgba;
    C2DFnt* p2DFnt;
    INT nPauseKind = CGameWork::getFlag(1, 0x33) ? 6 : m_nPauseKind;

    switch (m_nStep) {
    case 0:
        if (m_nPauseKind != 5) {
            C2DMan::changeLevel(C2DMan::D2MAN_LEVEL1);
            CPackMan::changeDrawMode(1);
            CSound::setBGMMasterVolume(0x40, -1);
        }
        m_nStep = 1;
        break;
    case 1:
        m_nStep = 2;
        break;
    case 2:
        _initPos(pos);
        m_pScreenCtrl = NEW(197, C2DSeqCtrl);
        m_pScreenCtrl->setLayer(12);
        m_pScreenCtrl->setPos(pos);
        if (bgSeqTbl[nPauseKind]) {
            m_pScreenCtrl->addSeq(pSeqName, bgSeqTbl[nPauseKind]);
        }
        if (selectSeqTbl[nPauseKind][0]) {
            m_pSelectCtrl = NEW(205, C2DSeqCtrl);
            m_pSelectCtrl->setLayer(12);
            m_pSelectCtrl->addSeq(pSeqName, selectSeqTbl[nPauseKind][0]);
            m_pSelectCtrl->setPos(pos);
        }
        rgba.col = 0x80707070;
        for (INT i = 0; i < m_nSelectMax; i++) {
            m_pSelectFnt[i] = p2DFnt = NEW(213, C2DFnt)(0, 0, &rgba, 17, selectWordTbl[nPauseKind][i], 0x42);
            _setFntLayer(p2DFnt, 12, 0);
            m_pSelectCtrl->addFntShift(pSeqName, p2DFnt, 0.0f, i * 33.0f + 9.0f);
            rgba.col = 0x80303030;
        }
        if (m_nSelectMax) {
            m_pCursorCtrl = NEW(221, C2DSeqCtrl);
            m_pCursorCtrl->setLayer(13);
            m_pCursorCtrl->addSeq(pSeqName, 0);
            m_vPosCursorCurrent.set(350.0f, m_fCursorBaseY, 0.0f);
            m_pCursorCtrl->setPos(m_vPosCursorCurrent);
        }
        CSound::playSE(3, -1, 64);
        m_nStep++;
        break;
    case 3:
        m_nStep = nPauseKind ? 5 : 4;
        break;
    case 4:
        if (isButtonTrg(0x800, 0)) {
            CSound::playSE(4, -1, 64);
            m_nStep = 6;
        }
        break;
    case 5:
        if (isButtonTrg(0x800, 0) && nPauseKind != 3) {
            CSound::playSE(4, -1, 64);
            m_nStep = 6;
        } else if (isTrgOK(0)) {
            m_nResult = resultTbl[nPauseKind][m_nSelect];
            if (m_nResult == 1) {
                m_bEventSkip = m_nResult;
                CSound::stopAllSE();
            }
            if (m_nResult == 4) {
                m_nStep = 9;
            } else if (m_nResult == 5) {
                m_nStep = 12;
            } else {
                CSound::playSE(4, -1, 64);
                m_nStep = 6;
            }
            CSound::playSE(2, -1, 64);
        } else if (isButtonRpt(0x305000, 0)) {
            INT nSelectOld = m_nSelect;
            m_nSelect = isButtonRpt(0x101000, 0) ? nSelectOld - 1 : nSelectOld + 1;
            if (m_nSelect < 0) {
                if (isButtonTrg(0x101000, 0)) {
                    m_nSelect = m_nSelectMax - 1;
                } else {
                    m_nSelect = nSelectOld;
                    break;
                }
            } else if (m_nSelect >= m_nSelectMax) {
                if (isButtonTrg(0x204000, 0)) {
                    m_nSelect = 0;
                } else {
                    m_nSelect = nSelectOld;
                    break;
                }
            }
            CSound::playSE(1, -1, 64);
            m_pSelectCtrl->replaceSeq(0, pSeqName, selectSeqTbl[nPauseKind][m_nSelect]);
            rgba.col = 0x80707070;
            m_pSelectFnt[m_nSelect]->m_C0 = rgba;
            rgba.col = 0x80303030;
            m_pSelectFnt[nSelectOld]->m_C0 = rgba;
            FLOAT _y = m_fCursorBaseY + m_nSelect * 33.0f;
            m_vPosCursorTarget.set(350.0f, _y, 0.0f);
            m_fgCursorMove = 1;
            m_fCursorTimer = 0.0f;
            m_vPosCursorOld = m_vPosCursorCurrent;
        }
        moveCursor();
        break;
    case 6:
        dispose();
        m_nStep = 8;
        break;
    case 7:
    case 8:
    default:
        if (m_bEventSkip && m_nPauseKind != 5) {
            CTask* pTask = CTaskMan::getSysTask(0x1A);
            if (pTask) {
                (*(ATEL::CVSM**)((char*)pTask + 0x70))->AtelEventSkip();
            }
        }
        if (m_nPauseKind != 5) {
            CSound::setBGMMasterVolume(-1, -1);
        }
        out();
        break;
    case 9:
        CMinigameManager::reqDispHelp(this, 13, 0);
        m_nStep++;
        break;
    case 10:
        if (CMinigameManager::isEndDispHelp()) {
            m_nStep++;
        }
        break;
    case 12:
        C2DMan::setLayer(13);
        m_hHelpLay = C2DMan::create(pMEBLayName, 0);
        m_nStep++;
        break;
    case 13:
        if (C2DMan::isEnd(m_hHelpLay)) {
            m_nStep++;
        }
        break;
    case 14:
        if (isTrgOK(0) || isTrgCANCEL(0)) {
            C2DMan::destroy(m_hHelpLay);
            C2DMan::setLayer(13);
            m_hHelpLay = C2DMan::create(pMEBLayName, 1);
            CSound::playSE(4, -1, 64);
            m_nStep++;
        }
        break;
    case 15:
        if (!C2DMan::isEnd(m_hHelpLay)) {
            break;
        }
        C2DMan::destroy(m_hHelpLay);
    case 11:
        m_nStep = 5;
        break;
    }
}

void CPause::disp() {}

void CPause::moveCursor() {
    if (m_fgCursorMove && m_pCursorCtrl) {
        m_fCursorTimer += getFrameRateF();
        if (m_fCursorTimer >= 40.0f) {
            m_fCursorTimer = 40.0f;
            m_fgCursorMove = 0;
        }
        FLOAT fRate = 1.0f - powf(1.0f - m_fCursorTimer / 40.0f, 3.0f);
        sceVu0InterVectorXYZ(m_vPosCursorCurrent.v, m_vPosCursorTarget.v, m_vPosCursorOld.v, fRate);
        m_pCursorCtrl->setPos(m_vPosCursorCurrent);
    }
}
// SOURCE DATA: .data .rodata
