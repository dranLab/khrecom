#include "System/Task.h"
#include "System/Graphic/Anim.h"
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in FriendAladdin). */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() {
    TRACE("");
}
inline void _emptyStringPlaceholder2() {
    TRACE("");
}

#include "Form/Menu/GameOver.h"
#include "System/HeapMan.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DFnt.h"
#include "Game/Game.h"
#include "Game/GameWork.h"

inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
#define NEW(line, T) new (com_malloc_d(sizeof(T), __FILE__ "(" #line ")", RET_ADDR())) T


#include "Game/SceneEnv.h"
#line 44

#include "System/Sound/SoundMan.h"
#line 49

extern "C" void sceVu0InterVectorXYZ(float* v0, float* v1, float* v2, float t);
extern "C" float powf(float, float);

using namespace D2;

CGameOver::CGameOver(CTask* p, BOOL bHeartless) : CMenu(p, -1) {
    declareSysTask(30);
    m_pLadName = "pause.lad";
    CGame::enablePause(FALSE);
    m_bHeartless = bHeartless;
    m_pScreenCtrl = NULL;
    m_pSelectCtrl[1] = NULL;
    m_pSelectCtrl[0] = NULL;
    m_pLightCtrl = NULL;
    m_pCursorCtrl = NULL;
    m_nSelect = 0;
    m_fgLightMove = 0;
}

CGameOver::~CGameOver() {
    delete m_pScreenCtrl;
    m_pScreenCtrl = NULL;
    delete m_pSelectCtrl[0];
    m_pSelectCtrl[0] = NULL;
    delete m_pSelectCtrl[1];
    m_pSelectCtrl[1] = NULL;
    delete m_pLightCtrl;
    m_pLightCtrl = NULL;
    delete m_pCursorCtrl;
    m_pCursorCtrl = NULL;
}

void CGameOver::dispose() {
    FVECTOR vColor;

    vColor.set(1.0f, 1.0f, 1.0f, 1.0f);
    CSceneEnv::setColor((ObjType::eObjBit)0x187C, 0.0f, vColor, NULL, 0);
    CSceneEnv::setStop((ObjType::eObjBit)0);
    CGameWork::setInt(1, 0xE2, 0);
}

static inline void _setColor(ObjType::eObjBit iBit, FLOAT fFrame, const FVECTOR& vAim) {
    CSceneEnv::setColor(iBit, fFrame, vAim, NULL, 0);
}

/* CDrawable::setLayer on the fonts. The original's stores are not "in struct" memory references
   (the second addFntShift's scheduling depends on it), so they are written through a USHORT pointer
   to CDrawable::m_LayerInfo (+0x10). */
static inline void _setFntLayer(C2DFnt* p, USHORT nID, USHORT nDepth) {
    ((USHORT*)p)[8] = nID;
    ((USHORT*)p)[9] = nDepth;
}

void CGameOver::main() {
    static INT iObjType = 0x1FFC;
    static CHAR* pSeqName = "pause:00";
    FVECTOR vPos;
    FVECTOR vColor;
    G_RGBA rgba;
    C2DFnt* p2DFnt;

    switch (m_nStep) {
    case 0:
        vColor._q = 0;
        _setColor((ObjType::eObjBit)iObjType, 1.0f, vColor);
        CSceneEnv::setStop((ObjType::eObjBit)iObjType);
        vPos._q = 0;
        vPos.w = 1.0f;
        m_pScreenCtrl = NEW(115, C2DSeqCtrl);
        m_pScreenCtrl->setLayer(13);
        m_pScreenCtrl->addSeq(pSeqName, 3);
        m_pScreenCtrl->setPos(vPos);
        vColor._q = 0;
        vPos.y = m_bHeartless ? 0.0f : 56.0f;
        rgba.col = 0x80707070;
        m_pSelectCtrl[0] = NEW(124, C2DSeqCtrl);
        m_pSelectCtrl[0]->setLayer(13);
        m_pSelectCtrl[0]->addSeq(pSeqName, 9);
        m_pSelectCtrl[0]->enableColorCtrl(TRUE);
        m_pSelectCtrl[0]->setPos(vPos);
        p2DFnt = NEW(129, C2DFnt)(0, 0, &rgba, 17, "Continue", 0x42);
        m_pSelectFnt[0] = p2DFnt;
        _setFntLayer(p2DFnt, 13, 0);
        m_pSelectCtrl[0]->addFntShift(pSeqName, p2DFnt, 0.0f, 9.0f);
        m_pSelectCtrl[1] = NEW(134, C2DSeqCtrl);
        m_pSelectCtrl[1]->setLayer(13);
        m_pSelectCtrl[1]->addSeq(pSeqName, 9);
        m_pSelectCtrl[1]->enableColorCtrl(TRUE);
        vPos.y += 32.0f;
        m_pSelectCtrl[1]->setPos(vPos);
        p2DFnt = NEW(141, C2DFnt)(0, 0, &rgba, 17, "Return to Title", 0x42);
        m_pSelectFnt[1] = p2DFnt;
        _setFntLayer(p2DFnt, 13, 0);
        m_pSelectCtrl[1]->addFntShift(pSeqName, p2DFnt, 0.0f, 9.0f);
        m_nStep++;
        break;
    case 1:
        if (m_pScreenCtrl->isEnd(0)) {
            m_pScreenCtrl->replaceSeq(0, pSeqName, 4);
            m_pSelectCtrl[0]->replaceSeq(0, pSeqName, 7);
            m_pSelectCtrl[1]->replaceSeq(0, pSeqName, 10);
            m_pLightCtrl = NEW(157, C2DSeqCtrl);
            m_pLightCtrl->setLayer(14);
            m_pLightCtrl->addSeq(pSeqName, 0);
            m_vPosLightCurrent.set(350.0f, 175.0f, 0.0f);
            if (!m_bHeartless) {
                m_vPosLightCurrent.y += 56.0f;
            }
            m_pLightCtrl->setPos(m_vPosLightCurrent);
            m_pCursorCtrl = NEW(164, C2DSeqCtrl);
            m_pCursorCtrl->setLayer(14);
            m_pCursorCtrl->addSeq(pSeqName, 1);
            vPos = m_vPosLightCurrent;
            vPos.x += -200.0f;
            vPos.y += 4.0f;
            m_pCursorCtrl->setPos(vPos);
            m_nStep++;
        }
        break;
    case 2:
        m_nStep = 4;
        break;
    case 4:
        if (isTrgOK(0)) {
            CSound::playSE(2, -1, 64);
            m_pScreenCtrl->replaceSeq(0, pSeqName, 5);
            if (m_nSelect == 0) {
                m_pSelectCtrl[0]->replaceSeq(0, pSeqName, 8);
                m_pSelectCtrl[1]->replaceSeq(0, pSeqName, 11);
            } else {
                m_pSelectCtrl[0]->replaceSeq(0, pSeqName, 11);
                m_pSelectCtrl[1]->replaceSeq(0, pSeqName, 8);
            }
            delete m_pLightCtrl;
            m_pLightCtrl = NULL;
            delete m_pCursorCtrl;
            m_pCursorCtrl = NULL;
            m_nStep = 5;
        } else if (isButtonTrg(0x101000, 0) || isButtonTrg(0x204000, 0)) {
            CSound::playSE(1, -1, 64);
            m_nSelect ^= 1;
            if (m_nSelect == 0) {
                m_pSelectCtrl[0]->replaceSeq(0, pSeqName, 7);
                m_pSelectCtrl[1]->replaceSeq(0, pSeqName, 10);
                m_vPosLightTarget.set(350.0f, 175.0f, 0.0f);
            } else {
                m_pSelectCtrl[0]->replaceSeq(0, pSeqName, 10);
                m_pSelectCtrl[1]->replaceSeq(0, pSeqName, 7);
                m_vPosLightTarget.set(350.0f, 208.0f, 0.0f);
            }
            if (!m_bHeartless) {
                m_vPosLightTarget.y += 56.0f;
            }
            m_fLightTimer = 0.0f;
            m_vPosLightOld = m_vPosLightCurrent;
            m_fgLightMove = 1;
            vPos = m_vPosLightTarget;
            vPos.x += -200.0f;
            vPos.y += 4.0f;
            m_pCursorCtrl->setPos(vPos);
        }
        moveCursor();
        break;
    case 5:
        if (m_pScreenCtrl->isEnd(0)) {
            m_nStep = 16;
        }
        break;
    case 16:
    default:
        if (m_nSelect == 0) {
            CGame::reqContinue_GameOver();
        } else {
            CGame::reqExit_GameOver();
        }
        out();
        break;
    }
}

void CGameOver::moveCursor() {
    if (m_fgLightMove && m_pLightCtrl) {
        m_fLightTimer += getFrameRateF();
        if (m_fLightTimer >= 30.0f) {
            m_fLightTimer = 30.0f;
            m_fgLightMove = 0;
        }
        FLOAT fRate = 1.0f - powf(1.0f - m_fLightTimer / 30.0f, 2.5f);
        sceVu0InterVectorXYZ(m_vPosLightCurrent.v, m_vPosLightTarget.v, m_vPosLightOld.v, fRate);
        m_pLightCtrl->setPos(m_vPosLightCurrent);
    }
}

void CGameOver::disp() {}
// SOURCE DATA: .data .rodata
