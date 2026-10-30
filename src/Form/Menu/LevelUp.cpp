#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/Anim.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in WorldSelect); which header it comes
 * from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

#include "Game/Status.h"
#include "Game/Technic.h"

/* System/Task.h's container pool overflow message: here it follows Technic.h's strings. */
inline void _taskSLCOverflowLevelUp() {
    TRACE("\x83\x5e\x83\x58\x83\x4e\x97\x70\x83\x52\x83\x93\x83\x65\x83\x69\x82\xaa MAX \x82\xc9\x93\x9e\x92\x42\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
}

#include "System/Ctd/CtdMan.h"
#include "Form/Menu/LevelUp.h"
#include "Form/Menu/MenuModel.h"
#include "Form/Menu/TechHelp.h"
#include "System/HeapMan.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DFnt.h"
#include "Game/Game.h"
#include "Game/GameWork.h"
#include "Game/SceneInfo.h"

inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
#define NEW(line, T) new (com_malloc_d(sizeof(T), __FILE__ "(" #line ")", RET_ADDR())) T

enum NAME_CASE {
    NAME_CASE_SENSITIVE = 0,
    NAME_CASE_UPPER = 1,
    NAME_CASE_LOWER = 2
};

namespace D2 {
class C2DMan {
public:
    static HANDLE create(const char* pName, UINT nIdx);
    static void destroy(HANDLE h);
    static BOOL isEnd(HANDLE h);
};
}

class CRsrcDB {
public:
    static void removeGroup(INT nGroup, INT nID);
    static struct TexInfo* getRsrcTex(const char* pName, NAME_CASE eCase);
};

#include "System/Rcf.h"





#include "System/Sound/SoundMan.h"
#line 72

#include "Game/Atel/Atel.h"




extern "C" {
int sprintf(char*, const char*, ...);
float powf(float, float);
void sceVu0InterVectorXYZ(float* v0, float* v1, float* v2, float t);
}

using namespace D2;

BOOL CLevelUp::m_bEnd = FALSE;

CLevelUp::CLevelUp(CTask* p, USHORT nTechKind) : CMenu(p, -1) {
    m_bEnd = FALSE;
    m_nTechHelpKind = nTechKind;
    CGame::enablePause(FALSE);
    m_pLadName = "levelup.lad";
    m_pSeqName = "levelup:00";
    m_pWinInfoCtrl = NULL;
    for (INT i = 0; i < 5; i++) {
        m_pInfoCtrl[i] = NULL;
        m_pInfoFntCtrl[i][0] = NULL;
        m_pInfoFntCtrl[i][1] = NULL;
    }
    m_vPosLightCurrent.init();
    m_pWinAbCtrl[2] = NULL;
    m_pWinAbCtrl[1] = NULL;
    m_pWinAbCtrl[0] = NULL;
    m_pLightCtrl = NULL;
    m_pCursorCtrl = NULL;
    m_pMenuModel[1] = NULL;
    m_pMenuModel[0] = NULL;
    m_pInfoSelCtrl = NULL;
    m_pLevelCtrl = NULL;
    m_pUpMsgCtrl = NULL;
    m_pEffectCtrl = NULL;
    m_pUpEffectCtrl = NULL;
    m_pExplaneCtrl = NULL;
    m_bPlayerKind = CGame::isRikuGame();
    m_bPlayerMode = CSceneInfo::getCurrentWorldID(TRUE) == 6;
}

CLevelUp::~CLevelUp() {
    m_bEnd = TRUE;
    dispose();
    CGame::enablePause(TRUE);
    CRsrcDB::removeGroup(2, m_bPlayerKind + 0x67);
}

void CLevelUp::dispose() {
    delete m_pWinInfoCtrl;
    m_pWinInfoCtrl = NULL;
    for (INT i = 0; i < 5; i++) {
        delete m_pInfoCtrl[i];
        m_pInfoCtrl[i] = NULL;
        delete m_pInfoFntCtrl[i][0];
        m_pInfoFntCtrl[i][0] = NULL;
        delete m_pInfoFntCtrl[i][1];
        m_pInfoFntCtrl[i][1] = NULL;
    }
    for (INT i = 0; i < 3; i++) {
        delete m_pWinAbCtrl[i];
        m_pWinAbCtrl[i] = NULL;
    }
    delete m_pCursorCtrl;
    m_pCursorCtrl = NULL;
    delete m_pLightCtrl;
    m_pLightCtrl = NULL;
    delete m_pMenuModel[0];
    m_pMenuModel[0] = NULL;
    delete m_pMenuModel[1];
    m_pMenuModel[1] = NULL;
    delete m_pInfoSelCtrl;
    m_pInfoSelCtrl = NULL;
    delete m_pLevelCtrl;
    m_pLevelCtrl = NULL;
    delete m_pUpMsgCtrl;
    m_pUpMsgCtrl = NULL;
    delete m_pEffectCtrl;
    m_pEffectCtrl = NULL;
    delete m_pExplaneCtrl;
    m_pExplaneCtrl = NULL;
    delete m_pUpEffectCtrl;
    m_pUpEffectCtrl = NULL;
}

void CLevelUp::main() {
    static LPCTSTR pBoneName = "WeaponR";
    static LPCTSTR pUpText = "Choose your bonus!";
    FVECTOR vec;
    FVECTOR vec2;
    G_RGBA rgba;
    INT idx;
    INT fgEnd;
    INT ret;
    unsigned char playerRsrcIDTbl[4][2] = { { 1, 2 }, { 11, 12 }, { 101, 0 }, { 111, 0 } };
    unsigned char availableTechTbl[12][2] = {
        { 2, 4 }, { 7, 6 }, { 12, 8 }, { 17, 5 }, { 22, 7 }, { 27, 9 },
        { 32, 39 }, { 37, 41 }, { 42, 10 }, { 47, 51 }, { 52, 11 }, { 57, 34 },
    };

    switch (m_nStep) {
    case 0:
        m_nStep++;
        if (CRsrcDB::getRsrcTex(m_pSeqName, NAME_CASE_SENSITIVE) != NULL) goto loaded;
        CRcf::reqLoadRCFFront(2, m_bPlayerKind + 0x67, 0, NULL, 0);
        break;
    case 1:
        if (!CRcf::isCompleteLoadRCF(2, m_bPlayerKind + 0x67)) break;
    loaded:
        m_pTechName = NULL;
        m_nAvailable[2] = -1;
        m_nAvailable[1] = -1;
        m_nAvailable[0] = -1;
        m_nSelectOld = -1;
        m_fTimer = 180.0f;
        m_nSelect = 0;
        m_nStatusUpMsg = 0;
        if (m_nTechHelpKind == 0) {
            idx = m_bPlayerKind * 2 + m_bPlayerMode;
#line 175
            m_pMenuModel[0] = NEW(175, CMenuModel)(this, playerRsrcIDTbl[idx][0] + 10000, NULL, 120, 110);
            m_pMenuModel[0]->changeAnim(31, FALSE);
            if (m_bPlayerKind == 0) {
#line 180
                m_pMenuModel[1] = NEW(180, CMenuModel)(this, playerRsrcIDTbl[idx][1] + 10000,
                                                       m_pMenuModel[0]->getCoord(pBoneName), 120, 110);
                m_pMenuModel[1]->changeAnim(31, FALSE);
            } else {
                m_pMenuModel[0]->setScale(0.09f);
            }
            m_fMenuModelAlpha = 0.0f;
            {
                FLOAT fAlpha = m_fMenuModelAlpha;
                m_pMenuModel[0]->setAlphaScale(fAlpha);
                if (m_pMenuModel[1]) m_pMenuModel[1]->setAlphaScale(fAlpha);
            }
            CStatus::setLV(CStatus::getLV() + 1);
            if (m_bPlayerKind == 0) {
                if (CStatus::getHPMax() < 560) m_nAvailable[0] = 0;
                if (CStatus::getCPMax() < 1625) m_nAvailable[1] = 0;
                for (INT i = 0; i < 12; i++) {
                    if (CStatus::getLV() < availableTechTbl[i][0]) break;
                    if (!CTechnic::canUse(availableTechTbl[i][1])) {
                        m_nAvailable[2] = i;
                        m_pTechName = CCtdMan::getText(availableTechTbl[i][1] + 0x80100);
                        break;
                    }
                }
            } else {
                if (CStatus::getHPMax() < 560) m_nAvailable[0] = 0;
                if ((CStatus::getLV() + 1) / 3 > CStatus::getAP() - 10 && CStatus::getAP() < 30) m_nAvailable[1] = 0;
                if (CTechnic::canUse(0x6D) && CStatus::getDPMax() < 99) m_nAvailable[2] = 0;
                if (m_nAvailable[0] < 0 && m_nAvailable[1] < 0 && m_nAvailable[2] < 0) m_nAvailable[2] = 0;
            }
            for (INT i = 2; i >= 0; i--) {
                if (m_nAvailable[i] >= 0) m_nSelect = i;
            }
            m_nStep++;
        } else {
            m_bPlayerKind = 0;
            m_nSelect = 2;
            m_pTechName = CTechnic::getName(m_nTechHelpKind);
            m_nStep = 14;
        }
        break;
    case 2:
        m_hLay = C2DMan::create(m_pLadName, 3);
#line 248
        m_pExplaneCtrl = NEW(248, C2DSeqCtrl);
        m_pExplaneCtrl->setLayer(12);
        m_pExplaneCtrl->enableColorCtrl(TRUE);
        vec.x = 26.0f;
        vec.y = 368.0f;
        vec.z = 0.0f;
        vec.w = 1.0f;
        m_pExplaneCtrl->setPos(vec);
        m_pExplaneCtrl->addSeq(m_pSeqName, 0x41);
        rgba.col = 0x807C7C7C;
#line 256
        m_pExplaneFnt = NEW(256, C2DFnt)(0, 0, &rgba, 0x11, pUpText, 0x40);
        m_pExplaneFnt->setLayer(12, 0);
        m_pExplaneCtrl->addFnt(m_pSeqName, m_pExplaneFnt);
        m_nStatusLad = 0;
        setWinInfo(0);
        setWinAb(0);
        setWinLvl(0);
        setEffect(0);
        m_nStep++;
        break;
    case 3:
        if (m_nStatusLad == 0) {
            if (C2DMan::isEnd(m_hLay)) {
                C2DMan::destroy(m_hLay);
                m_hLay = C2DMan::create(m_pLadName, 4);
                m_pExplaneCtrl->replaceSeq(0, m_pSeqName, 0x42);
                m_nStatusLad = 1;
            }
        }
        fgEnd = 0;
        if (m_nStatusLad) fgEnd++;
        if (setWinInfo(1)) fgEnd++;
        if (setWinAb(1)) fgEnd++;
        if (setWinLvl(1)) fgEnd++;
        if (setEffect(1)) fgEnd++;
        if (fgEnd >= 5 && m_fMenuModelAlpha >= 1.0f) {
            setWinInfo(7);
            m_nStep++;
        }
        m_fMenuModelAlpha += getFrameRateF() * 0.05f;
        if (m_fMenuModelAlpha > 1.0f) m_fMenuModelAlpha = 1.0f;
        m_pMenuModel[0]->setAlphaScale(m_fMenuModelAlpha);
        if (m_pMenuModel[1]) m_pMenuModel[1]->setAlphaScale(m_fMenuModelAlpha);
        break;
    case 4:
        m_nStep = 6;
        break;
    case 5:
        break;
    case 6:
        ret = setWinAb(7);
        if (ret < 0) {
            CSound::playSE(0x54, -1, 0x40);
            if (m_nSelect == 0) {
                CStatus::setHPMax(CStatus::getHPMax() + 15);
                CStatus::setHP(CStatus::getHPMax());
            } else if (m_bPlayerKind == 0) {
                if (m_nSelect == 1) {
                    CStatus::setCPMax(CStatus::getCPMax() + 25);
                } else {
                    CTechnic::addTechnic(availableTechTbl[m_nAvailable[2]][1]);
                }
            } else {
                if (m_nSelect == 1) {
                    CStatus::setAP(CStatus::getAP() + 1);
                } else {
                    INT inDPMax = CStatus::getDPMax() + 2;
                    if (inDPMax > 99) inDPMax = 99;
                    CStatus::setDPMax(inDPMax);
                }
            }
            setWinLvl(7);
            setUpMsg(0);
            setEffect(8);
            {
                INT nAnim = m_bPlayerKind == 0 ? 0xFC8 : 0xFCA;
                m_pMenuModel[0]->changeAnimEx(nAnim + 10000, 0, TRUE);
            }
            if (m_pMenuModel[1]) m_pMenuModel[1]->changeAnimEx(0x36D9, 0, TRUE);
            m_nStep = 7;
        }
        if (ret != 0 && m_nStep == 6) setWinInfo(7);
        break;
    case 7:
        fgEnd = setUpMsg(1);
        if ((m_pMenuModel[0] && m_pMenuModel[0]->isAnimEnd()) ||
            (fgEnd && (CPadHandler::isTrgCANCEL() || CPadHandler::isTrgOK()))) {
            m_nStep = 8;
        }
        if (m_nStep != 7) setUpMsg(3);
        break;
    case 8:
        if (setUpMsg(4)) m_nStep = 10;
        if (m_nStep != 8 && m_bPlayerKind == 0 && m_nSelect == 2) {
#line 387
            NEW(387, CTechHelp)(this, availableTechTbl[m_nAvailable[2]][1]);
            m_nStep = 9;
        }
        break;
    case 9:
        if (CTechHelp::m_bEnd) m_nStep = 10;
        break;
    case 10:
        C2DMan::destroy(m_hLay);
        m_hLay = C2DMan::create(m_pLadName, 5);
        m_pExplaneCtrl->replaceSeq(0, m_pSeqName, 0x43);
        setWinInfo(4);
        setWinAb(4);
        setWinLvl(4);
        setEffect(4);
        if (!CStatus::isLevelUp()) {
            CSound::fadeoutAllSE(30, 1);
            if (CGameWork::getFlag(1, 0x2E)) {
                CAtel::reqWhiteFadeOut(20);
                CGameWork::setFlag(1, 0x2E, FALSE);
            }
        }
        m_nStep++;
        break;
    case 11:
        if (C2DMan::isEnd(m_hLay)) {
            C2DMan::destroy(m_hLay);
            dispose();
            if (CStatus::isLevelUp()) {
                m_nStep = 1;
            } else {
                m_nStep = 13;
            }
            break;
        }
        m_fMenuModelAlpha -= getFrameRateF() * 0.075f;
        if (m_fMenuModelAlpha < 0.0f) m_fMenuModelAlpha = 0.0f;
        m_pMenuModel[0]->setAlphaScale(m_fMenuModelAlpha);
        if (m_pMenuModel[1]) m_pMenuModel[1]->setAlphaScale(m_fMenuModelAlpha);
        break;
    case 14:
        setUpMsg(0);
        CSound::playSE(0x54, -1, 0x40);
        m_nStep = 15;
        break;
    case 15:
        ret = setUpMsg(1);
        m_fTimer -= getFrameRateF();
        if (ret) {
            if (CPadHandler::isTrgCANCEL() || CPadHandler::isTrgOK() || m_fTimer < 0.0f) {
#line 457
                NEW(457, CTechHelp)(this, m_nTechHelpKind);
                m_nStep = 16;
            }
        }
        break;
    case 16:
        if (CTechHelp::m_bEnd) {
            setUpMsg(3);
            m_nStep = 17;
        }
        break;
    case 17:
        if (setUpMsg(4)) m_nStep = 13;
        break;
    default:
        out();
        break;
    }
}

INT CLevelUp::setWinInfo(INT nSetCmd) {
    static char* infoTextTbl[2][5][3] = {
        {
            { "MAX HP", "%4d", "%4d \xf9Y\x81\xa8\xf9R%4d" },
            { "MAX CP", "%4d", "%4d \xf9Y\x81\xa8\xf9R%4d" },
            { "Sleights", "", "\xf9R%s" },
            { "EXP", "%d", "%d" },
            { "Next Level", "%d", "%d" },
        },
        {
            { "MAX HP", "%4d", "%4d \xf9Y\x81\xa8\xf9R%4d" },
            { "AP", "%4d", "%4d \xf9Y\x81\xa8\xf9R%4d" },
            { "MAX DP", "%4d", "%4d \xf9Y\x81\xa8\xf9R%4d" },
            { "EXP", "%d", "%d" },
            { "Next Level", "%d", "%d" },
        },
    };
    char infoBaseSeqTbl[5] = { 53, 51, 55, 52, 54 };
    char infoFntLRSeqTbl[5][2] = { { 43, 48 }, { 41, 46 }, { 45, 50 }, { 42, 47 }, { 44, 49 } };
    FVECTOR vec;
    FVECTOR vec2;
    C2DSeqCtrl* p2DSeqCtrl2;
    C2DFnt* p2DFnt;
    G_RGBA rgba;
    LPCTSTR pText;
    char pBuf[32];
    INT lvl;
    INT ret = 0;

    switch (nSetCmd) {
    case 0:
#line 552
        m_pWinInfoCtrl = NEW(552, C2DSeqCtrl);
        m_pWinInfoCtrl->setLayer(12);
        m_pWinInfoCtrl->enableColorCtrl(TRUE);
        vec.set(390.0f, 286.0f, 0.0f);
        m_pWinInfoCtrl->setPos(vec);
        m_pWinInfoCtrl->addSeq(m_pSeqName, 0x23);
        rgba.col = 0x807C7C7C;
        lvl = CStatus::getLV();
        for (INT i = 0; i < 5; i++) {
            C2DSeqCtrl* p2DSeqCtrl;
#line 563
            p2DSeqCtrl = m_pInfoCtrl[i] = NEW(563, C2DSeqCtrl);
            p2DSeqCtrl->setLayer(13);
            p2DSeqCtrl->enableColorCtrl(TRUE);
            p2DSeqCtrl->addSeq(m_pSeqName, infoBaseSeqTbl[i]);
            p2DSeqCtrl->setParent(m_pWinInfoCtrl);
#line 571
            p2DSeqCtrl2 = m_pInfoFntCtrl[i][0] = NEW(571, C2DSeqCtrl);
            p2DSeqCtrl2->setLayer(13);
            p2DSeqCtrl2->enableColorCtrl(TRUE);
            p2DSeqCtrl2->addSeq(m_pSeqName, infoFntLRSeqTbl[i][0]);
            p2DSeqCtrl2->setParent(p2DSeqCtrl);
#line 577
            p2DFnt = m_pInfoFnt[i][0] = NEW(577, C2DFnt)(0, 0, &rgba, 15, infoTextTbl[m_bPlayerKind][i][0], 0x240);
            p2DFnt->setLayer(13, 0);
            p2DSeqCtrl2->addFnt(m_pSeqName, p2DFnt);
#line 583
            p2DSeqCtrl2 = m_pInfoFntCtrl[i][1] = NEW(583, C2DSeqCtrl);
            p2DSeqCtrl2->setLayer(13);
            p2DSeqCtrl2->enableColorCtrl(TRUE);
            p2DSeqCtrl2->addSeq(m_pSeqName, infoFntLRSeqTbl[i][1]);
            p2DSeqCtrl2->setParent(p2DSeqCtrl);
#line 589
            p2DFnt = m_pInfoFnt[i][1] = NEW(589, C2DFnt)(0, 0, &rgba, i < 3 ? 15 : 14, NULL, 0x241);
            p2DFnt->setLayer(13, 0);
            p2DSeqCtrl2->addFnt(m_pSeqName, p2DFnt);
            pText = infoTextTbl[m_bPlayerKind][i][1];
            if (m_bPlayerKind == 0) {
                if (i < 2) {
                    sprintf(pBuf, pText, i == 0 ? CStatus::getHPMax() : CStatus::getCPMax());
                    pText = pBuf;
                }
            } else {
                if (i < 3) {
                    sprintf(pBuf, pText,
                            i == 0 ? CStatus::getHPMax() : i == 1 ? CStatus::getAP() : CStatus::getDPMax());
                    pText = pBuf;
                }
            }
            m_pInfoFnt[i][1]->allocText(pText);
            if (i >= 3) {
                INT nEXP = CStatus::getEXP();
                INT expNext = CStatus::getLvExp(lvl) - nEXP;
                if (expNext < 0) expNext = 0;
                sprintf(pBuf, infoTextTbl[m_bPlayerKind][i][1], i == 3 ? nEXP : expNext);
                m_pInfoFnt[i][1]->allocText(pBuf);
            }
        }
#line 620
        m_pInfoSelCtrl = NEW(620, C2DSeqCtrl);
        m_pInfoSelCtrl->setLayer(12);
        m_pInfoSelCtrl->enableColorCtrl(TRUE);
        m_pInfoSelCtrl->addSeq(m_pSeqName, 0x21);
        m_pInfoSelCtrl->setParent(m_pWinInfoCtrl);
        m_pInfoSelCtrl->enableDisp(FALSE);
        m_nStatusInfo = 0;
        break;
    case 1:
    case 2:
        if (m_nStatusInfo == 0) {
            if (m_pWinInfoCtrl->isEnd(0)) {
                m_pWinInfoCtrl->replaceSeq(0, m_pSeqName, 0x24);
                m_nStatusInfo = 1;
            }
        }
        ret = m_nStatusInfo;
        break;
    case 3:
    case 4:
        m_pWinInfoCtrl->replaceSeq(0, m_pSeqName, 0x25);
        break;
    case 7:
        m_pInfoSelCtrl->setParent(m_pInfoCtrl[m_nSelect]);
        m_pInfoSelCtrl->enableDisp(TRUE);
        for (INT i = 0; i < 3; i++) {
            if (m_bPlayerKind == 0) {
                if (i < 2) {
                    if (i == m_nSelect) {
                        sprintf(pBuf, infoTextTbl[0][i][2], i == 0 ? CStatus::getHPMax() : CStatus::getCPMax(),
                                i == 0 ? CStatus::getHPMax() + 15 : CStatus::getCPMax() + 25);
                    } else {
                        sprintf(pBuf, infoTextTbl[0][i][1], i == 0 ? CStatus::getHPMax() : CStatus::getCPMax());
                    }
                } else {
                    pBuf[0] = ' ';
                    pBuf[1] = '\0';
                    if (i == m_nSelect && m_nAvailable[2] >= 0) {
                        sprintf(pBuf, infoTextTbl[0][i][2], m_pTechName);
                    }
                }
            } else {
                if (i == m_nSelect) {
                    INT dpMaxPlus = CStatus::getDPMax() + 2;
                    if (dpMaxPlus > 99) dpMaxPlus = 99;
                    sprintf(pBuf, infoTextTbl[m_bPlayerKind][i][2],
                            i == 0 ? CStatus::getHPMax() : i == 1 ? CStatus::getAP() : CStatus::getDPMax(),
                            i == 0 ? CStatus::getHPMax() + 15 : i == 1 ? CStatus::getAP() + 1 : dpMaxPlus);
                } else {
                    sprintf(pBuf, infoTextTbl[m_bPlayerKind][i][1],
                            i == 0 ? CStatus::getHPMax() : i == 1 ? CStatus::getAP() : CStatus::getDPMax());
                }
            }
            m_pInfoFnt[i][1]->allocText(pBuf);
        }
        break;
    }
    return ret;
}

INT CLevelUp::setWinAb(INT nSetCmd) {
    static char* abilityTextTbl[2][3][2] = {
        {
            { "HP Boost", "Raise Sora's HP." },
            { "CP Boost", "Raise Sora's CP." },
            { "Sleights", "Learn a new sleight." },
        },
        {
            { "HP Boost", "Raise Riku's HP." },
            { "Attack Boost", "Raise Riku's AP." },
            { "Darkness Boost", "Raise Riku's DP." },
        },
    };
    FVECTOR vec;
    FVECTOR vec2;
    C2DSeqCtrl* p2DSeqCtrl;
    C2DFnt* p2DFnt;
    G_RGBA rgba;
    INT ret = 0;
    FLOAT fFrameRate = getFrameRateF();

    switch (nSetCmd) {
    case 0:
        rgba.col = 0x807C7C7C;
        vec.x = 0.0f;
        vec.y = 96.0f;
        vec.z = 0.0f;
        vec.w = 1.0f;
        for (INT i = 0; i < 3; i++) {
#line 747
            p2DSeqCtrl = m_pWinAbCtrl[i] = NEW(747, C2DSeqCtrl);
            p2DSeqCtrl->enableColorCtrl(TRUE);
            p2DSeqCtrl->setLayer(12);
            p2DSeqCtrl->setPos(vec);
            p2DSeqCtrl->addSeq(m_pSeqName, m_nAvailable[i] < 0 ? 7 : i + 4);
            p2DSeqCtrl->addSeqShift(m_pSeqName, i + 0x3B, 80.0f, -34.0f);
            {
                INT n = i;
                if (n < 2) n ^= 1;
                p2DSeqCtrl->addSeqShift(m_pSeqName, n + 0x3E, 36.0f, 0.0f);
            }
            vec.y += 94.0f;
#line 759
            p2DFnt = m_pWinAbFnt[i][0] = NEW(759, C2DFnt)(0, 0, &rgba, 17, abilityTextTbl[m_bPlayerKind][i][0], 0x240);
            p2DFnt->setLayer(12, 0);
            p2DSeqCtrl->addFntShift(m_pSeqName, p2DFnt, 72.0f, 2.0f);
#line 764
            p2DFnt = m_pWinAbFnt[i][1] = NEW(764, C2DFnt)(0, 0, &rgba, 15, abilityTextTbl[m_bPlayerKind][i][1], 0x240);
            p2DFnt->setLayer(12, 0);
            p2DSeqCtrl->addFntShift(m_pSeqName, p2DFnt, 50.0f, 37.0f);
        }
        m_nStatusAb = 0;
        m_nStepStatusAb = 0;
        break;
    case 1:
    case 2:
        if (m_nStatusAb == 0) {
            INT fgLastEnd = -1;
            for (INT i = 0; i < 3; i++) {
                p2DSeqCtrl = m_pWinAbCtrl[i];
                if (p2DSeqCtrl->isEnd(0)) {
                    p2DSeqCtrl->replaceSeq(0, m_pSeqName, m_nAvailable[i] < 0 ? 11 : 8);
                    fgLastEnd = i;
                }
            }
            if (fgLastEnd >= 2) {
#line 794
                m_pCursorCtrl = NEW(794, C2DSeqCtrl);
                m_pCursorCtrl->setLayer(13);
                m_pCursorCtrl->addSeq(m_pSeqName, 12);
#line 797
                m_pLightCtrl = NEW(797, C2DSeqCtrl);
                m_pLightCtrl->setLayer(13);
                m_pLightCtrl->addSeq(m_pSeqName, 16);
                m_fgCursorMove = -1;
                moveCursor(100.0f);
                m_nStatusAb = 1;
            }
        }
        ret = m_nStatusAb;
        break;
    case 3:
    case 4:
        for (INT i = 0; i < 3; i++) {
            p2DSeqCtrl = m_pWinAbCtrl[i];
            p2DSeqCtrl->replaceSeq(0, m_pSeqName, m_nAvailable[i] < 0 ? 10 : 9);
        }
        m_pLightCtrl->replaceSeq(0, m_pSeqName, 0x11);
        delete m_pCursorCtrl;
        m_pCursorCtrl = NULL;
        break;
    case 7:
        if (m_nStepStatusAb == 0) {
            p2DSeqCtrl = m_pWinAbCtrl[m_nSelect];
            p2DSeqCtrl->replaceSeq(0, m_pSeqName, 3);
            if (m_nSelectOld >= 0) {
                p2DSeqCtrl = m_pWinAbCtrl[m_nSelectOld];
                p2DSeqCtrl->replaceSeq(0, m_pSeqName, 8);
            }
            m_nStepStatusAb++;
        } else {
            if (CPadHandler::isTrgOK()) {
                CSound::playSE(2, -1, 0x40);
                ret = -1;
            } else if (CPadHandler::isButtonRpt(0x305000)) {
                INT nSelect = m_nSelect;
                INT nAdd = CPadHandler::isButtonRpt(0x101000) ? -1 : 1;
                for (INT i = 0; i < 3; i++) {
                    nSelect += nAdd;
                    if (nSelect < 0) {
                        if (!CPadHandler::isButtonTrg(0x101000)) { nSelect = -1; break; }
                        nSelect = 2;
                    } else if (nSelect >= 3) {
                        if (!CPadHandler::isButtonTrg(0x204000)) { nSelect = -1; break; }
                        nSelect = 0;
                    }
                    if (m_nAvailable[nSelect] >= 0) break;
                }
                if (nSelect < 0) break;
                m_nSelectOld = m_nSelect;
                m_nSelect = nSelect;
                if (m_nSelectOld != m_nSelect) {
                    m_nStepStatusAb = 0;
                    m_fgCursorMove = -1;
                    ret = 1;
                    CSound::playSE(1, -1, 0x40);
                }
            }
            moveCursor(fFrameRate);
        }
        break;
    }
    return ret;
}

INT CLevelUp::setWinLvl(INT nSetCmd) {
    static LPCTSTR pLevelStr = "LEVEL %2d";
    FVECTOR vec;
    G_RGBA rgba;
    INT ret = 0;
    static char pStaticBuf[16];

    switch (nSetCmd) {
    case 0:
        vec.x = 365.0f;
        vec.y = 46.0f;
        vec.z = 0.0f;
        vec.w = 1.0f;
        rgba.col = 0x807C7C7C;
#line 912
        m_pLevelCtrl = NEW(912, C2DSeqCtrl);
        m_pLevelCtrl->setLayer(12);
        m_pLevelCtrl->enableColorCtrl(TRUE);
        m_pLevelCtrl->setPos(vec);
        m_pLevelCtrl->addSeq(m_pSeqName, 0x44);
        sprintf(pStaticBuf, pLevelStr, CStatus::getLV());
#line 918
        m_pLevelFnt = NEW(918, C2DFnt)(0, 0, &rgba, 15, pStaticBuf, 0x242);
        m_pLevelFnt->setLayer(12, 0);
        m_pLevelCtrl->addFntShift(m_pSeqName, m_pLevelFnt, 0.0f, 8.0f);
        m_nStatusLvl = 0;
        break;
    case 1:
    case 2:
        if (m_nStatusLvl == 0) {
            if (m_pLevelCtrl->isEnd(0)) {
                m_pLevelCtrl->replaceSeq(0, m_pSeqName, 0x45);
                m_nStatusLvl = 1;
            }
        }
        ret = m_nStatusLvl;
        break;
    case 3:
    case 4:
        m_pLevelCtrl->replaceSeq(0, m_pSeqName, 0x46);
        break;
    case 7:
        sprintf(pStaticBuf, pLevelStr, CStatus::getLV());
        m_pLevelFnt->setText(pStaticBuf);
        break;
    }
    return ret;
}

INT CLevelUp::setEffect(INT nSetCmd) {
    static LPCTSTR pCockpitSeqName = "Cockpit:00";
    FVECTOR vec;
    INT ret = 0;

    switch (nSetCmd) {
    case 0:
        vec.x = 380.0f;
        vec.y = 150.0f;
        vec.z = 0.0f;
        vec.w = 1.0f;
#line 975
        m_pEffectCtrl = NEW(975, C2DSeqCtrl);
        m_pEffectCtrl->setLayer(12);
        m_pEffectCtrl->setPos(vec);
        m_pEffectCtrl->addSeq(m_pSeqName, 0x38);
        m_nStatusEffect = 0;
        break;
    case 1:
    case 2:
        if (m_nStatusEffect == 0) {
            if (m_pEffectCtrl->isEnd(0)) {
                m_pEffectCtrl->replaceSeq(0, m_pSeqName, 0x39);
                m_nStatusEffect = 1;
            }
        }
        ret = m_nStatusEffect;
        break;
    case 3:
    case 4:
        m_pEffectCtrl->replaceSeq(0, m_pSeqName, 0x3A);
        break;
    case 8:
#line 1003
        m_pUpEffectCtrl = NEW(1003, C2DSeqCtrl);
        m_pUpEffectCtrl->setLayer(15);
        vec.x = 120.0f;
        vec.y = 0.0f;
        vec.z = 0.0f;
        vec.w = 1.0f;
        m_pUpEffectCtrl->setLocalPos(vec);
        m_pUpEffectCtrl->addSeq(pCockpitSeqName, 0x3D);
        break;
    }
    return ret;
}

INT CLevelUp::setUpMsg(INT nSetCmd) {
    static char* infoTextTbl[2][3] = {
        { "HP increased by 15.", "CP increased by 25.", "Sora learns\n%s." },
        { "HP increased by 15.", "AP increased by 1.", "DP increased by 2." },
    };
    LPCTSTR pInfoText2 = "Sora learns\n%s.";
    LPCTSTR pInfoText3 = "DP increased by 1.";
    LPCTSTR pInfoText4 = "Riku learns\n%s.";
    FVECTOR vec;
    G_RGBA rgba;
    INT ret = 0;
    static char pStaticBuf[64];

    switch (nSetCmd) {
    case 0:
#line 1041
        m_pUpMsgCtrl = NEW(1041, C2DSeqCtrl);
        m_pUpMsgCtrl->setLayer(13);
        m_pUpMsgCtrl->enableColorCtrl(TRUE);
        if (m_nTechHelpKind == 0) {
            vec.set(152.0f, 282.0f, 0.0f);
        } else {
            vec.set(256.0f, 192.0f, 0.0f);
        }
        m_pUpMsgCtrl->setPos(vec);
        m_pUpMsgCtrl->addSeq(m_pSeqName, 0x19);
        if (m_bPlayerKind == 0 && m_nSelect == 2) {
            sprintf(pStaticBuf,
                    m_nTechHelpKind < 90 ? infoTextTbl[0][2] : !CGame::isSoraGame() ? pInfoText4 : pInfoText2,
                    m_pTechName);
        } else if (m_bPlayerKind != 0 && m_nSelect == 2 && CStatus::getDPMax() >= 99) {
            sprintf(pStaticBuf, pInfoText3);
        } else {
            sprintf(pStaticBuf, infoTextTbl[m_bPlayerKind][m_nSelect]);
        }
        rgba.col = 0x807C7C7C;
#line 1063
        m_pUpMsgFnt = NEW(1063, C2DFnt)(0, 0, &rgba, 16, pStaticBuf, 0x242);
        m_pUpMsgFnt->setLayer(13, 0);
        m_pUpMsgFnt->setSpace(0, 0x40);
        m_pUpMsgCtrl->addFntShift(m_pSeqName, m_pUpMsgFnt, 0.0f, -12.0f);
        break;
    case 1:
    case 2:
        if (m_nStatusUpMsg == 0) {
            if (m_pUpMsgCtrl->isEnd(0)) {
                m_pUpMsgCtrl->replaceSeq(0, m_pSeqName, 0x1A);
                m_nStatusUpMsg = 1;
            }
        }
        ret = m_nStatusUpMsg;
        break;
    case 3:
        m_pUpMsgCtrl->replaceSeq(0, m_pSeqName, 0x1B);
        break;
    case 4:
        ret = m_pUpMsgCtrl->isEnd(0) != 0;
        break;
    case 5:
    case 6:
        break;
    }
    return ret;
}

void CLevelUp::moveCursor(FLOAT fFrameRate) {
    FVECTOR vec;

    if (m_fgCursorMove == 0) return;
    if (m_pCursorCtrl == NULL) return;
    if (m_fgCursorMove < 0) {
        vec.x = 236.0f;
        vec.y = m_nSelect * 94.0f + 90.0f;
        vec.z = 0.0f;
        vec.w = 1.0f;
        m_pCursorCtrl->setPos(vec);
        vec.x += -4.0f;
        vec.y += -24.0f;
        m_fgCursorMove = 1;
        m_vPosLightOld = m_vPosLightCurrent;
        m_fLightTimer = 0.0f;
        m_vPosLightTarget = vec;
    }
    m_fLightTimer += fFrameRate;
    if (m_fLightTimer >= 40.0f) {
        m_fLightTimer = 40.0f;
        m_fgCursorMove = 0;
    }
    {
        FLOAT t = 1.0f - powf(1.0f - m_fLightTimer / 40.0f, 3.0f);
        sceVu0InterVectorXYZ(m_vPosLightCurrent.v, m_vPosLightTarget.v, m_vPosLightOld.v, t);
    }
    m_pLightCtrl->setPos(m_vPosLightCurrent);
}
// SOURCE DATA: .data .rodata .bss
