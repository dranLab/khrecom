#include "Form/Menu/Menu.h"
#include "System/TaskMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/Anim.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in GimmickTutorial); which header
 * it comes from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

#include "Game/Card.h"

#include "Game/Atel/ChatMan.h"
#include "System/Graphic/Effect/effect.h"

#include "Form/Menu/WorldSelect.h"
#include "System/HeapMan.h"
#include "System/Math3D.h"
#include "System/Graphic/PathCamera.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DFnt.h"
#include "System/Graphic/2d/2DCard.h"
#include "Game/Entity.h"
#include "Game/Gimmick/GimmickDoor.h"
#include "Game/Collision/CollMapCamera.h"
#include "Game/Player/Player.h"
#include "Game/SceneInfo.h"
#include "Game/SceneEnv.h"
#include "Game/GameWork.h"
#include "Game/Game.h"



#line 268





















#include "System/RsrcDB.h"




#include "System/Rcf.h"





#include "System/Graphic/PackMan.h"




extern "C" {
float sinf(float);
float cosf(float);
float powf(float, float);
int sprintf(char*, const char*, ...);
}
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);

#include "System/Sound/SoundMan.h"
#line 322

#include "System/Graphic/Glare.h"



















CWorldSelect::CWorldSelect(CTask* p, HANDLE hDoor) : CMenu(p, -1) {
    INT i;
    INT j;

    declareSysTask(0x1D);
    CGame::enablePause(FALSE);
    m_hDoor = hDoor;
    m_pPathCamera = NULL;
    m_pLadName = "wselect.lad";
    m_pWorldCard = NULL;
    m_nMenuStep = 0;
    m_fgEnd = 0;
    m_nSelectMax = 0;
    m_nSelect = 0;
    for (INT i = 0; i < 8; i++) { /* gcc runs this one backwards (from m_nWorld[7]) */
        m_nWorld[i] = 0;
    }
    for (INT i = 0; i < 5; i++) {
        m_pCardCtrl[i] = NULL;
        m_pCard[i] = NULL;
    }
    m_pExplainCtrl = NULL;
    m_pIconCtrl = NULL;
    m_pNameWndCtrl = NULL;
    m_fgRotate = 0;
    m_nRotCurrent = 0;
    m_fRotCurrent = 0.0f;
    CGameWork::setInt(1, 0xE2, -1);

    USHORT idxMax = CSceneInfo::getCurrentFloorIndex();
    INT fgSora = CGame::isSoraGame();
    INT nMax;
    if (idxMax == 0) {
        m_nWorld[0] = fgSora ? 1 : 9;
        m_nSelectMax = 1;
    } else if (idxMax < fgSora + 5) {
        static const USHORT worldLowTbl[2][5] = { { 2, 5, 8, 1, 9999 }, { 2, 3, 4, 5, 6 } };
        for (i = 0; i < fgSora + 4; i++) {
            USHORT worldID = worldLowTbl[fgSora][i];
            for (j = 1; j < fgSora + 5; j++) {
                if (worldID == CSceneInfo::getWorldID(j)) break;
            }
            if (j >= fgSora + 5) {
                m_nWorld[m_nSelectMax++] = worldID;
            }
        }
    } else if (idxMax < (nMax = fgSora + 9)) {
        static const USHORT worldMiddleTbl[2][4] = { { 7, 3, 4, 6 }, { 7, 8, 9, 10 } };
        for (i = 0; i < 4; i++) {
            USHORT worldID = worldMiddleTbl[fgSora][i];
            for (j = fgSora + 5; j < fgSora + 9; j++) {
                if (worldID == CSceneInfo::getWorldID(j)) break;
            }
            if (j >= fgSora + 9) {
                m_nWorld[m_nSelectMax++] = worldID;
            }
        }
    } else {
        static const USHORT worldHighTbl[2][3] = { { 12, 11, 13 }, { 11, 12, 13 } };
        m_nSelectMax = 1;
        m_nWorld[0] = worldHighTbl[fgSora][idxMax - nMax];
    }

    m_pGimmickDoor = (CGimmickDoor*)CTaskMan::getTask(m_hDoor);
    m_pRoomInfo = CSceneInfo::getRoomInfo(-1);
    m_pDoorInfo = &m_pRoomInfo->_doors[m_pGimmickDoor->getDoorID()];
    int iDoorType[4] = { 0x80, 0x100, 0x200, 0x400 };
    CSceneEnv::setStop((ObjType::eObjBit)(m_iObjType = iDoorType[m_pDoorInfo->_id] ^ 0x1FFC));
    CGameWork::setByte(1, 0x140, 5);
    CSound::playSE(0x4D, -1, 0x40);
}

CWorldSelect::~CWorldSelect() {
    INT i;
    for (i = 0; i < 5; i++) {
        delete m_pCardCtrl[i];
        m_pCardCtrl[i] = NULL;
        delete m_pCard[i];
        m_pCard[i] = NULL;
    }
    delete m_pNameWndCtrl;
    m_pNameWndCtrl = NULL;
    delete m_pIconCtrl;
    m_pIconCtrl = NULL;
    delete m_pExplainCtrl;
    m_pExplainCtrl = NULL;
    delete m_pPathCamera;
    m_pPathCamera = NULL;
    dispose();
    CGameWork::setByte(1, 0x140, 0);
    CGameWork::setFlag(1, 0x1E, FALSE);
    CSceneEnv::setStop((ObjType::eObjBit)0);
    CGame::enablePause(TRUE);
}

/* CSceneEnv::setColor through an inline function: a constant frame argument then goes straight into $f12 (the
 * original's register use around these calls). */
static inline void _setColor(ObjType::eObjBit iBit, FLOAT fFrame, const FVECTOR& v, const FVECTOR* pv, INT n) {
    CSceneEnv::setColor(iBit, fFrame, v, pv, n);
}

void CWorldSelect::dispose() {
    FVECTOR vColor;
    vColor.set(1.0f, 1.0f, 1.0f, 1.0f);
    _setColor((ObjType::eObjBit)0x187C, 0.0f, vColor, NULL, 0);
    CSceneEnv::setColor((ObjType::eObjBit)4, 0.0f, vColor, NULL, 1);
    CSceneEnv::setStop((ObjType::eObjBit)0);
    CGlare::on(0.0f);
    CGameWork::setInt(1, 0xE2, 0);
}

using namespace D2;

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

/* CDrawable::setLayer on the fonts, written through a USHORT pointer as in GameOver. */
static inline void _setFntLayer(C2DFnt* p, USHORT nID, USHORT nDepth) {
    ((USHORT*)p)[8] = nID;
    ((USHORT*)p)[9] = nDepth;
}


void CWorldSelect::main() {
    FVECTOR vColor;
    FVECTOR vColor2;
    FVECTOR vPos;
    CCoord camCrd;
    G_RGBA rgba;
    LPCTSTR pSeqName = "wselect:00";
    FLOAT fFrame = getFrameRateF();

    switch (m_nStep) {
    case 0:
        if (!CRsrcDB::getRsrcTex(pSeqName, NAME_CASE_SENSITIVE)) {
            CRcf::reqLoadRCFFront(2, 6, 0, NULL, 0);
            m_fgLoadRCF = TRUE;
        }
        if (!m_pGimmickDoor) {
            m_nStep = 999;
            break;
        }
#line 239
        ASSERT(g_pPlayer);
        m_pWorldCard = new (__FILE__ "(240)") CEntity(this, (ObjType::eObjType)1);
        m_pWorldCard->setEntityFlag(0x400000, 0);
        m_pWorldCard->initalize(10, 0x20);
        m_pWorldCard->Crd(0).setParent(&g_pPlayer->Crd(g_pPlayer->getBoneIndex("root", 0)));
        m_pWorldCard->changeMotion(1, 0, FALSE, 0, 0, 0);
        camCrd = g_pPlayer->Crd(0);
        addCoord(1);
        Crd(0).setParent(NULL);
        Crd(0).m_Local = camCrd.m_Local;
        Crd(0).recalcWorldMatrix();
        vPos.set(0.0f, 0.01f, 0.0f, 1.0f);
        m_hSpotEffect = CEffectMan::attachEffect("rec_mon_he", 0, (ObjType::eObjType)7, &m_pGimmickDoor->Crd(0), &vPos,
                                                 NULL, 0.0f);
        vColor.set(0.0f, 0.0f, 0.0f, 1.0f);
        _setColor((ObjType::eObjBit)m_iObjType, 120.0f, vColor, NULL, 0);
        vColor.set(0.75f, 0.75f, 0.75f, 1.0f);
        _setColor((ObjType::eObjBit)(m_pDoorInfo->_id + 0x80), 120.0f, vColor, NULL, 1);
        CGlare::off(30.0f);
        m_nStep++;
        break;
    case 1:
        if (!m_pWorldCard->isMotionEnd()) {
            return;
        }
        if (m_fgLoadRCF && !CRcf::isCompleteLoadRCF(2, 6)) {
            return;
        }
        vColor.zero();
        CSceneEnv::setColor((ObjType::eObjBit)m_iObjType, 10.0f, vColor, NULL, 0);
        m_nStep++;
        break;
    case 2:
        CSound::pauseAllSE();
        C2DMan::changeLevel(C2DMan::D2MAN_LEVEL1);
        CPackMan::changeDrawMode(1);
        m_hLay = C2DMan::create(m_pLadName, 0);
        vPos.init();
#line 290
        m_pExplainCtrl = new (__FILE__ "(290)") C2DSeqCtrl;
        m_pExplainCtrl->setLayer(12);
        m_pExplainCtrl->addSeq(pSeqName, 0x19);
        m_pExplainCtrl->setPos(vPos);
        m_pExplainCtrl->enableColorCtrl(TRUE);
        rgba.col = 0x807C7C7C;
#line 296
        m_pExplainFnt = new (__FILE__ "(296)") C2DFnt(0, 0, &rgba, 15, ": Select card         : Confirm", 0x240);
        _setFntLayer(m_pExplainFnt, 12, 0);
        m_pExplainCtrl->addFnt(pSeqName, m_pExplainFnt);
        CSound::playSE(0x4E, -1, 0x40);
        m_nStep++;
        break;
    case 3:
        if (C2DMan::isEnd(m_hLay)) {
            CSound::setBGMMasterVolume(0x40, -1);
            C2DMan::destroy(m_hLay);
            m_hLay = C2DMan::create(m_pLadName, 1);
            m_pExplainCtrl->replaceSeq(0, pSeqName, 0x1A);
            m_nStep++;
        }
        break;
    case 4:
        if (!menuMain()) {
            return;
        }
        if (m_fgEnd) {
            m_nStep = 5;
        } else {
            m_nStep = 7;
        }
        break;
    case 5:
    case 7:
        C2DMan::destroy(m_hLay);
        m_hLay = C2DMan::create(m_pLadName, 2);
        m_pExplainCtrl->replaceSeq(0, pSeqName, 0x1B);
        CSound::setBGMMasterVolume(-1, -1);
        m_nStep++;
        break;
    case 6:
    case 8:
        if (!C2DMan::isEnd(m_hLay)) {
            return;
        }
        C2DMan::destroy(m_hLay);
        m_hLay = 0;
        delete m_pExplainCtrl;
        m_pExplainCtrl = NULL;
        ATEL::CChatMan::instance().reqCommand(0x40F, NULL, 0);
        C2DMan::changeLevel(C2DMan::D2MAN_LEVEL0);
        CSound::startAllPausedSE();
        CPackMan::changeDrawMode(0);
#line 351
        ASSERT(g_pPlayer);
        if (m_nStep == 6) {
            CGameWork::setFlag(1, 0x26, TRUE);
            g_pPlayer->setTrgFlag(0x10000, 0);
            m_pWorldCard->changeMotion(CGame::isSoraGame() ? 2 : 3, 0, FALSE, 0, 0, 0);
            m_nStep = 9;
        } else {
            m_pWorldCard->setupFade(1.0f, 0.0f, 1200, -1);
            vColor.set(1.0f, 1.0f, 1.0f, 1.0f);
            vColor2.set(0.25f, 0.25f, 0.25f, 1.0f);
            _setColor((ObjType::eObjBit)m_iObjType, 30.0f, vColor, &vColor2, 0);
            CSceneEnv::setColor((ObjType::eObjBit)4, 30.0f, vColor, &vColor2, 1);
            CGlare::on(30.0f);
            CEffectMan::killEffect(m_hSpotEffect, 30.0f);
            m_nStep = 16;
        }
        break;
    case 9:
#line 373
        m_pPathCamera = new (__FILE__ "(373)") CPathCamera(this);
        m_pPathCamera->setLocal(TRUE);
        m_pPathCamera->init("world_cr_01", TRUE, FALSE, 0x80);
        m_pPathCamera->run();
        CCollMapCamera::setFgForceStop(TRUE);
        CSound::playSE(0x4F, -1, 0x40);
        m_fTimer = 240.0f;
        m_nStep++;
        break;
    case 10:
        m_fTimer -= fFrame;
        if (m_fTimer < 0.0f) {
            CGlare::fade(30.0f, 1.0f, -1.0f);
            m_pWorldCard->setupFade(1.0f, 0.0f, 10, -1);
            m_nStep++;
        }
    case 11:
        if (m_pPathCamera->isEnd()) {
            m_pPathCamera->init("world_cr_02", TRUE, FALSE, 0x80);
            m_pPathCamera->run();
            m_fTimer = 30.0f;
            m_nStep++;
        }
        break;
    case 12:
        m_fTimer -= fFrame;
        if (m_fTimer < 0.0f) {
            m_pGimmickDoor->changeMotion(2, 0, FALSE, -1, 0, 0);
            m_pPathCamera->init("world_cr_03", TRUE, FALSE, 0x80);
            m_pPathCamera->run();
            m_fTimer = 180.0f;
            m_nStep++;
        }
        break;
    case 13:
        m_fTimer -= fFrame;
        if (m_fTimer < 0.0f) {
            m_fTimer = 0.0f;
            m_nStep++;
        }
        break;
    case 14:
        m_fTimer -= fFrame;
        if (m_fTimer < 0.0f) {
            CSceneInfo::reqEnterWorld((WORLD_ID)m_nWorld[m_nSelect]);
            m_nStep = 15;
        }
        break;
    case 15:
        break;
    case 16:
        if (CSceneEnv::isColorEnd(0)) {
#line 439
            ASSERT(g_pPlayer);
            g_pPlayer->setTrgFlag(0x8000, 0);
            CGameWork::setInt(1, 0xE2, 0);
            CSceneEnv::setStop((ObjType::eObjBit)0);
            m_nStep = 17;
        }
        break;
    case 17:
    default:
        m_pWorldCard->out();
        out();
        break;
    }
}
#line 460














INT CWorldSelect::menuMain() {
    static const unsigned char cardKindTbl[14] = { 0, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202 };
    static const unsigned char iconSeqNameNoTbl[14] = { 0, 11, 1, 9, 13, 7, 5, 2, 8, 6, 10, 12, 4, 3 };
    INT ret = 0;
    CARD card;
    FVECTOR vec;
    FVECTOR vec2;
    FVECTOR vScale;
    G_RGBA rgba;
    LPCTSTR pSeqName = "wselect:00";
    INT nRotMove = 0;
    char pIconSeqNameTmp[16];

    sprintf(pIconSeqNameTmp, "wselect:%02d", iconSeqNameNoTbl[m_nWorld[m_nSelect]]);
    switch (m_nMenuStep) {
    case 0:
        m_rotCircle.zero();
        m_rotCircle.x = 1.0471976f;
        vec2.y = 0.0f;
        m_nRotOne = 360 / m_nSelectMax;
        for (INT i = 0; i < m_nSelectMax; i++) {
            m_posLocal[i].init();
            m_posLocal[i].w = 1.0f; /* again: the second use of 1.0f gets it hoisted out of the loop */
            m_posLocal[i].x = sinf(vec2.y) * 150.0f;
            m_posLocal[i].y = cosf(vec2.y) * 150.0f;
            vec2.y += m_nRotOne * 0.017453292f;
#line 509
            C2DSeqCtrl* pCardCtrl = m_pCardCtrl[i] = new (__FILE__ "(509)") C2DSeqCtrl;
            pCardCtrl->addSeq(pSeqName, 0xD);
#line 512
            C2DCard* pCard = m_pCard[i] = new (__FILE__ "(512)") C2DCard;
            card.init(cardKindTbl[m_nWorld[i]], 0);
            pCard->create(card);
            pCard->setParent(pCardCtrl);
        }
        m_fgEnd = 0;
        m_nMenuStep = 1;
        break;
    case 1:
        m_nMenuStep = 2;
        break;
    case 2:
        if (!m_pCardCtrl[0]->isEnd(0)) {
            break;
        }
        for (INT i = 0; i < m_nSelectMax; i++) {
            m_pCardCtrl[i]->replaceSeq(0, pSeqName, 0xE);
        }
        m_pCardCtrl[m_nSelect]->replaceSeq(0, pSeqName, 0x11);
        m_pCardCtrl[m_nSelect]->addSeq(pSeqName, 0x12);
#line 534
        m_pNameWndCtrl = new (__FILE__ "(534)") C2DSeqCtrl;
        m_pNameWndCtrl->setLayer(13);
        m_nSeqNameWnd = 10;
        m_pNameWndCtrl->addSeq(pSeqName, 10);
        vec.init();
        m_pNameWndCtrl->setPos(vec);
        rgba.col = 0x807C7C7C;
#line 541
        m_pNameWndFnt = new (__FILE__ "(541)") C2DFnt(0, 0, &rgba, 0x11, cardDataTbl[cardKindTbl[m_nWorld[m_nSelect]]].pName, 0x242);
        _setFntLayer(m_pNameWndFnt, 12, 0);
        m_pNameWndCtrl->addFnt(pSeqName, m_pNameWndFnt);
        m_pNameWndCtrl->enableColorCtrl(TRUE);
#line 548
        m_pIconCtrl = new (__FILE__ "(548)") C2DSeqCtrl;
        m_pIconCtrl->setLayer(12);
        m_pIconCtrl->setPos(vec);
        m_pIconCtrl->addSeq(pIconSeqNameTmp, 0);
        m_nMenuStep = 3;
        break;
    case 3:
        if (m_nSeqNameWnd != 11) {
            if (!m_pNameWndCtrl->isEnd(0)) {
                break;
            }
            m_nSeqNameWnd = 11;
            m_pNameWndCtrl->replaceSeq(0, pSeqName, 11);
        }
        if (_isTrgOK(0)) {
            m_fgEnd = 1;
            m_pIconCtrl->addSeq(pSeqName, 0x14);
            for (INT i = 0; i < m_nSelectMax; i++) {
                if (i != m_nSelect) {
                    m_pCardCtrl[i]->replaceSeq(0, pSeqName, 0xF);
                }
            }
            CSound::playSE(0x1A, -1, 0x40);
            m_nMenuStep = 6;
        } else if (_isTrgCANCEL(0)) {
            if (CGameWork::getFlag(1, 0x1E)) {
                break;
            }
            CSound::playSE(4, -1, 0x40);
            m_nMenuStep = 7;
        } else if (_isButtonRpt(0x408000, 0)) {
            nRotMove = -m_nRotOne;
        } else if (_isButtonRpt(0x802000, 0)) {
            nRotMove = m_nRotOne;
        }
        if (m_nSelectMax >= 2 && nRotMove != 0) {
            INT nSelectOld = m_nSelect;
            if (nRotMove > 0) {
                if (++m_nSelect >= m_nSelectMax) {
                    m_nSelect = 0;
                }
            } else {
                if (--m_nSelect < 0) {
                    m_nSelect = m_nSelectMax - 1;
                }
            }
            m_fgRotate = 1;
            m_fRotOld = m_nRotCurrent;
            m_nRotCurrent += nRotMove;
            m_fRotTarget = m_nRotCurrent;
            m_fTimer = 0.0f;
            if (m_pCardCtrl[nSelectOld]->getNumSeq() >= 2) {
                m_pCardCtrl[nSelectOld]->replaceSeq(0, pSeqName, 0xE);
                m_pCardCtrl[nSelectOld]->destroy(1);
                m_nSeqNameWnd = 12;
                m_pNameWndCtrl->replaceSeq(0, pSeqName, 12);
                m_pIconCtrl->replaceSeq(0, pIconSeqNameTmp, 1);
            }
            CSound::playSE(0x19, -1, 0x40);
            m_nMenuStep = 4;
        }
        break;
    case 4:
        if (m_fgRotate) {
            break;
        }
        m_pCardCtrl[m_nSelect]->replaceSeq(0, pSeqName, 0x11);
        m_pCardCtrl[m_nSelect]->addSeq(pSeqName, 0x12);
        m_nSeqNameWnd = 10;
        m_pNameWndCtrl->replaceSeq(0, pSeqName, 10);
        m_pNameWndFnt->setText(cardDataTbl[cardKindTbl[m_nWorld[m_nSelect]]].pName);
        m_pIconCtrl->replaceSeq(0, pIconSeqNameTmp, 0);
        m_nMenuStep = 3;
        break;
    case 6:
        if (m_pIconCtrl->isEnd(1)) {
            m_nMenuStep = 7;
        }
        break;
    case 7:
        for (INT i = 0; i < m_nSelectMax; i++) {
            if (!m_fgEnd || i == m_nSelect) {
                m_pCardCtrl[i]->replaceSeq(0, pSeqName, 0xF);
            }
        }
        m_pCardCtrl[m_nSelect]->replaceSeq(0, pSeqName, 0xF);
        m_pIconCtrl->replaceSeq(0, pIconSeqNameTmp, 1);
        m_pNameWndCtrl->replaceSeq(0, pSeqName, 0xC);
        m_nMenuStep = 8;
        break;
    case 8:
        if (m_pNameWndCtrl->isEnd(0)) {
            m_nMenuStep = 9;
        }
        break;
    case 9:
        for (INT i = 0; i < 5; i++) {
            delete m_pCardCtrl[i];
            m_pCardCtrl[i] = NULL;
            delete m_pCard[i];
            m_pCard[i] = NULL;
        }
        delete m_pNameWndCtrl;
        m_pNameWndCtrl = NULL;
        delete m_pIconCtrl;
        m_pIconCtrl = NULL;
        ret = 1;
        break;
    }
    if (m_nMenuStep == 9) {
        return ret;
    }
    if (m_fgRotate) {
        m_fTimer += getFrameRateF();
        if (m_fTimer >= 30.0f) {
            m_fTimer = 30.0f;
            m_fgRotate = 0;
        }
        FLOAT fRate = 1.0f - powf(1.0f - m_fTimer / 30.0f, 1.75f);
        m_fRotCurrent = m_fRotTarget * fRate + m_fRotOld * (1.0f - fRate);
        m_rotCircle.z = m_fRotCurrent = Math3D::LimitPI(m_fRotCurrent * 0.017453292f);
    }
    FMATRIX mat;
    mat.init();
    vec2.zero();
    vec2.y = m_rotCircle.z;
    RotMatrix(mat, mat, m_rotCircle);
    for (INT i = 0; i < m_nSelectMax; i++) {
        ApplyMatrix(vec, mat, m_posLocal[i]);
        m_pCardCtrl[i]->setLocalPos(vec);
        m_pCardCtrl[i]->setScl((vec.y + 75.0f) * 0.35 / 150.0 + 0.65f);
        vec2.y = Math3D::LimitPI(vec2.y);
        m_pCardCtrl[i]->setRot(vec2);
        m_pCard[i]->checkBack();
        vec2.y -= m_nRotOne * 0.017453292f;
    }
    return ret;
}
// SOURCE DATA: .rodata
