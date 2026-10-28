#include "khtypes.h"
#define FORM_MENU_SAVELOAD_CPP
inline void* operator new(unsigned int, void* p) throw() { return p; }
#define SAVELOADCTRL_H_PATH "c:/KHCOM/MainApp/Form/Menu/SaveLoadCtrl.h"
/* .rodata starts with the strings of the included headers' inline functions, in this order. */
#include "Form/Menu/SaveLoad.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Fsm.h"
#include "Form/Menu/SaveLoadCtrl.h"
#include "System/Ctd/CtdMan.h"
#include "System/Graphic/Anim.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"
#include "Form/Menu/SaveLoadUtil.h"
#include "System/TaskMan.h"
#include "System/Debug.h"
#include "System/PadHandler.h"
#include "System/Sound/SoundMan.h"
#include "Game/GameWork.h"
#include "Game/CardKind.h"
#include <string.h>
extern "C" int sprintf(char* buf, const char* fmt, ...);
#include "System/Graphic/ScreenMask.h"
#include "System/Graphic/2d/2DFnt.h"
#include "include_asm.h"
inline void C2DSeqSelectable::setCaptionSize(INT nSize) {
    if (m_pCaption) m_pCaption->setSize(nSize);
}
inline void C2DSeqSelectable::setCaptionText(const CHAR* pszText) {
    if (m_pCaption) m_pCaption->setText(pszText);
}
inline void C2DSeqSelectable::setCaptionSpace(INT nX, INT nY) {
    if (m_pCaption) m_pCaption->setSpace(nX, nY);
}

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

namespace {
BOOL s_bSaveLoadDone = 0;
CFadeIOMask* s_pFadeMask = NULL;
SLMSG_RESULT s_nMsgResult = SLMSG_RESULT_CANCEL;
UINT s_nWorldIconRsrc = 0;
SLMSG s_nMessage;
}

/* file-scope constant tables (deferred: emitted after the vtables, as in the original) */
static const CARD_DATA cardDataTbl[224] = {
    {{0}, 0, 0, 0, ""},
    {{17}, 2, 0, 3, "Kingdom Key"},
    {{17}, 2, 1, 3, "Three Wishes"},
    {{17}, 2, 2, 3, "Crabclaw"},
    {{17}, 2, 3, 3, "Pumpkinhead"},
    {{17}, 2, 4, 3, "Fairy Harp"},
    {{17}, 2, 5, 3, "Wishing Star"},
    {{17}, 2, 6, 3, "Spellbinder"},
    {{17}, 2, 7, 3, "Metal Chocobo"},
    {{17}, 3, 0, 3, "Olympia"},
    {{17}, 3, 1, 3, "Lionheart"},
    {{17}, 3, 2, 3, "Lady Luck"},
    {{17}, 3, 3, 3, "Divine Rose"},
    {{17}, 3, 4, 3, "Oathkeeper"},
    {{17}, 3, 5, 3, "Oblivion"},
    {{17}, 3, 6, 3, "Ultima Weapon"},
    {{17}, 3, 7, 3, "Diamond Dust"},
    {{17}, 4, 0, 3, "One-Winged Angel"},
    {{17}, 4, 1, 4, "Soul Eater"},
    {{17}, 4, 2, 3, "Star Seeker"},
    {{17}, 4, 3, 3, "Monochrome"},
    {{17}, 4, 4, 3, "Follow the Wind"},
    {{17}, 4, 5, 3, "Hidden Dragon"},
    {{17}, 4, 6, 3, "Photon Debugger"},
    {{17}, 4, 7, 3, "Bond of Flame"},
    {{17}, 26, 0, 0, "-"},
    {{34}, 5, 0, 3, "Fire"},
    {{34}, 5, 1, 3, "Blizzard"},
    {{34}, 5, 2, 3, "Thunder"},
    {{34}, 5, 3, 3, "Cure"},
    {{34}, 5, 4, 3, "Gravity"},
    {{34}, 5, 5, 3, "Stop"},
    {{34}, 5, 6, 3, "Aero"},
    {{34}, 26, 2, 0, "-"},
    {{34}, 26, 1, 0, "-"},
    {{51}, 5, 7, 3, "Simba"},
    {{51}, 6, 0, 3, "Genie"},
    {{51}, 6, 1, 3, "Bambi"},
    {{51}, 6, 2, 3, "Dumbo"},
    {{51}, 6, 3, 3, "Tinker Bell"},
    {{51}, 6, 4, 3, "Mushu"},
    {{51}, 6, 5, 3, "Cloud"},
    {{68}, 6, 6, 6, "Potion"},
    {{68}, 6, 7, 6, "Hi-Potion"},
    {{68}, 7, 0, 2, "Mega-Potion"},
    {{68}, 7, 1, 2, "Ether"},
    {{68}, 7, 2, 2, "Mega-Ether"},
    {{68}, 7, 3, 2, "Elixir"},
    {{68}, 7, 4, 2, "Megalixir"},
    {{85}, 7, 5, 2, "Donald Duck"},
    {{85}, 7, 6, 2, "Goofy"},
    {{85}, 7, 7, 2, "Aladdin"},
    {{85}, 8, 0, 2, "Ariel"},
    {{85}, 8, 1, 2, "Jack"},
    {{85}, 8, 2, 2, "Peter Pan"},
    {{85}, 8, 3, 2, "The Beast"},
    {{85}, 8, 4, 4, "The King"},
    {{85}, 8, 5, 2, "Pluto"},
    {{102}, 8, 6, 6, "Guard Armor"},
    {{102}, 8, 6, 6, "Trickmaster"},
    {{102}, 8, 6, 6, "Jafar"},
    {{102}, 8, 6, 6, "Oogie Boogie"},
    {{102}, 8, 6, 6, "Parasite Cage"},
    {{102}, 8, 6, 6, "Ursula"},
    {{102}, 8, 6, 6, "Dragon Maleficent"},
    {{102}, 8, 6, 6, "Darkside"},
    {{230}, 8, 7, 4, "-"},
    {{230}, 9, 0, 0, "-"},
    {{119}, 9, 1, 6, "Shadow"},
    {{119}, 9, 2, 2, "Soldier"},
    {{119}, 9, 3, 6, "Large Body"},
    {{119}, 9, 4, 2, "Red Nocturne"},
    {{119}, 9, 5, 2, "Blue Rhapsody"},
    {{119}, 9, 6, 2, "Yellow Opera"},
    {{119}, 9, 7, 2, "Green Requiem"},
    {{119}, 10, 0, 6, "Powerwild"},
    {{119}, 10, 1, 2, "Bouncywild"},
    {{119}, 10, 2, 2, "Air Soldier"},
    {{119}, 10, 3, 2, "Bandit"},
    {{119}, 10, 4, 6, "Fat Bandit"},
    {{119}, 10, 5, 2, "Barrel Spider"},
    {{119}, 10, 6, 6, "Search Ghost"},
    {{119}, 10, 7, 6, "Sea Neon"},
    {{119}, 11, 0, 2, "Screwdiver"},
    {{119}, 11, 1, 2, "Aquatank"},
    {{119}, 11, 2, 6, "Wight Knight"},
    {{119}, 11, 3, 2, "Gargoyle"},
    {{119}, 11, 4, 6, "Pirate"},
    {{119}, 11, 5, 2, "Air Pirate"},
    {{119}, 11, 6, 4, "Darkball"},
    {{119}, 11, 7, 6, "Defender"},
    {{119}, 12, 0, 2, "Wyvern"},
    {{119}, 12, 1, 2, "Wizard"},
    {{119}, 12, 2, 2, "Neoshadow"},
    {{119}, 12, 3, 2, "White Mushroom"},
    {{119}, 12, 4, 2, "Black Fungus"},
    {{119}, 12, 5, 2, "Creeper Plant"},
    {{119}, 12, 6, 2, "Tornado Step"},
    {{119}, 12, 7, 2, "Crescendo"},
    {{119}, 13, 0, 6, "Guard Armor"},
    {{119}, 13, 1, 6, "Parasite Cage"},
    {{119}, 13, 2, 6, "Trickmaster"},
    {{119}, 13, 3, 6, "Darkside"},
    {{119}, 13, 4, 2, "Card Soldier"},
    {{119}, 13, 5, 6, "Hades"},
    {{119}, 13, 6, 6, "Jafar"},
    {{119}, 13, 7, 6, "Oogie Boogie"},
    {{119}, 14, 0, 6, "Ursula"},
    {{119}, 14, 1, 6, "Hook"},
    {{119}, 14, 2, 6, "Dragon Maleficent"},
    {{119}, 14, 3, 2, "Riku"},
    {{119}, 14, 4, 2, "Axel"},
    {{119}, 14, 5, 2, "Larxene"},
    {{119}, 14, 6, 2, "Vexen"},
    {{119}, 14, 7, 2, "Marluxia"},
    {{119}, 15, 0, 6, "Lexaeus"},
    {{119}, 15, 1, 2, "Ansem"},
    {{119}, 15, 3, 6, "Zexion"},
    {{119}, 15, 4, 2, "Xemnas"},
    {{119}, 15, 5, 2, "Xigbar"},
    {{119}, 15, 6, 2, "Xaldin"},
    {{119}, 15, 7, 2, "Sa\x83\xc7x"},
    {{119}, 16, 0, 2, "Demyx"},
    {{119}, 16, 1, 2, "Luxord"},
    {{119}, 16, 2, 2, "Roxas"},
    {{119}, 21, 6, 2, "Gold Card"},
    {{119}, 21, 7, 2, "Platinum Card"},
    {{119}, 15, 2, 0, "Card Soldier Spade"},
    {{119}, 13, 4, 0, "Card Soldier Heart"},
    {{23}, 22, 0, 0, "\x83N\x83\x89\x83" "E\x83hA"},
    {{23}, 22, 1, 0, "\x83N\x83\x89\x83" "E\x83hB"},
    {{23}, 22, 2, 0, "\x83n\x83" "f\x83XA"},
    {{39}, 22, 3, 0, "\x83n\x83" "f\x83XB"},
    {{23}, 22, 4, 0, "\x83t\x83" "b\x83NA"},
    {{23}, 22, 5, 0, "\x83t\x83" "b\x83NB"},
    {{23}, 22, 6, 0, "\x83t\x83" "b\x83NC"},
    {{39}, 22, 7, 0, "\x83t\x83" "b\x83ND"},
    {{23}, 23, 0, 0, "\x83" "A\x83N\x83Z\x83\x8b" "A"},
    {{23}, 23, 1, 0, "\x83" "A\x83N\x83Z\x83\x8b" "B"},
    {{23}, 23, 2, 0, "\x83\x89\x83N\x83V\x81[\x83kA"},
    {{23}, 23, 3, 0, "\x83\x89\x83N\x83V\x81[\x83kB"},
    {{39}, 23, 4, 0, "\x83\x89\x83N\x83V\x81[\x83kC"},
    {{23}, 23, 5, 0, "\x83\x8a\x83NA"},
    {{23}, 23, 6, 0, "\x83\x8a\x83NB"},
    {{23}, 23, 7, 0, "\x83\x8a\x83NC"},
    {{23}, 24, 0, 0, "\x83\x8a\x83ND"},
    {{23}, 24, 1, 0, "\x83\x94\x83" "B\x83N\x83Z\x83\x93" "A"},
    {{39}, 24, 2, 0, "\x83\x94\x83" "B\x83N\x83Z\x83\x93" "B"},
    {{23}, 24, 3, 0, "\x83}\x81[\x83\x8b\x81[\x83V\x83\x83" "A"},
    {{23}, 24, 4, 0, "\x83}\x81[\x83\x8b\x81[\x83V\x83\x83" "B"},
    {{39}, 24, 5, 0, "\x83}\x81[\x83\x8b\x81[\x83V\x83\x83" "C"},
    {{119}, 24, 6, 0, "\x83}\x81[\x83\x8b\x81[\x83V\x83\x83" "2"},
    {{119}, 24, 7, 0, "\x83}\x81[\x83\x8b\x81[\x83V\x83\x83" "3"},
    {{23}, 25, 0, 0, "\x83\x8c\x83N\x83Z\x83" "E\x83XA"},
    {{23}, 25, 1, 0, "\x83\x8c\x83N\x83Z\x83" "E\x83XB"},
    {{39}, 25, 2, 0, "\x83\x8c\x83N\x83Z\x83" "E\x83XC"},
    {{23}, 25, 3, 0, "\x83" "A\x83\x93\x83Z\x83\x80" "A"},
    {{39}, 25, 4, 0, "\x83" "A\x83\x93\x83Z\x83\x80" "B"},
    {{39}, 25, 5, 0, "\x83[\x83N\x83V\x83I\x83\x93" "A"},
    {{119}, 26, 3, 0, "-"},
    {{136}, 0, 0, 6, "\x83\x8a\x83\x8d\x81[\x83h"},
    {{153}, 16, 3, 6, "Tranquil Darkness"},
    {{153}, 16, 4, 6, "Teeming Darkness"},
    {{153}, 16, 5, 6, "Feeble Darkness"},
    {{153}, 16, 6, 6, "Almighty Darkness"},
    {{153}, 16, 7, 6, "Sleeping Darkness"},
    {{153}, 17, 0, 6, "Looming Darkness"},
    {{153}, 17, 1, 2, "Premium Room"},
    {{153}, 17, 2, 2, "White Room"},
    {{153}, 17, 3, 6, "Black Room"},
    {{153}, 17, 4, 6, "Bottomless Darkness"},
    {{153}, 17, 5, 6, "Roulette Room"},
    {{169}, 17, 6, 6, "Martial Waking"},
    {{169}, 17, 7, 2, "Sorcerous Waking"},
    {{169}, 18, 0, 2, "Alchemic Waking"},
    {{169}, 18, 1, 6, "Meeting Ground"},
    {{169}, 18, 2, 6, "Stagnant Space"},
    {{169}, 18, 3, 6, "Strong Initiative"},
    {{169}, 18, 4, 6, "Lasting Daze"},
    {{185}, 18, 5, 2, "Calm Bounty"},
    {{185}, 18, 6, 2, "Guarded Trove"},
    {{185}, 18, 7, 2, "False Bounty"},
    {{185}, 19, 0, 6, "Moment's Reprieve"},
    {{185}, 19, 1, 6, "Mingling Worlds"},
    {{185}, 19, 2, 2, "Moogle Room"},
    {{185}, 19, 3, 6, "Random Joker"},
    {{202}, 19, 4, 6, "Key of Beginnings"},
    {{202}, 19, 5, 6, "Key of Guidance"},
    {{202}, 19, 6, 6, "Key to Truth"},
    {{202}, 19, 7, 2, "Key to Rewards"},
    {{219}, 20, 0, 6, "Traverse Town"},
    {{219}, 20, 3, 6, "Agrabah"},
    {{219}, 20, 2, 6, "Olympus Coliseum"},
    {{219}, 20, 1, 6, "Wonderland"},
    {{219}, 20, 5, 6, "Monstro"},
    {{219}, 20, 4, 6, "Halloween Town"},
    {{219}, 20, 6, 6, "Atlantica"},
    {{219}, 20, 7, 6, "Neverland"},
    {{219}, 21, 0, 6, "Hollow Bastion"},
    {{219}, 21, 1, 2, "100 Acre Woods"},
    {{219}, 21, 2, 6, "Twilight Town"},
    {{219}, 21, 3, 6, "Destiny Islands"},
    {{219}, 21, 4, 4, "Castle Oblivion"},
    {{13}, 0, 0, 0, "EXP\x90\xc2\x8f\xac"},
    {{13}, 0, 1, 0, "EXP\x90\xc2\x92\x86"},
    {{13}, 0, 2, 0, "EXP\x90\xc2\x91\xe5"},
    {{13}, 0, 3, 0, "EXP\x90\xd4\x8f\xac"},
    {{13}, 0, 4, 0, "EXP\x90\xd4\x92\x86"},
    {{13}, 0, 5, 0, "EXP\x90\xd4\x91\xe5"},
    {{13}, 0, 6, 0, "EXP\x8b\xe2"},
    {{13}, 1, 0, 0, "HP\x8f\xac"},
    {{13}, 1, 1, 0, "HP\x91\xe5"},
    {{13}, 2, 0, 0, "MP\x8f\xac"},
    {{13}, 2, 1, 0, "MP\x91\xe5"},
    {{13}, 3, 0, 0, "P"},
    {{13}, 4, 0, 0, "R"},
    {{12}, 0, 0, 0, "Unknown Room"},
    {{12}, 0, 0, 0, "Conqueror's Respite"},
    {{12}, 0, 0, 0, "Room of Rewards"},
    {{12}, 0, 0, 0, ""},
    {{12}, 0, 0, 0, ""},
    {{0}, 0, 0, 0, ""},
    {{0}, 0, 0, 0, ""},
    {{0}, 0, 0, 0, ""},
};
static const C2DSeqAgent::SeqTbl s_seqDataPlateTbl[3][2] = {
    {{{100, 101, 102}}, {{97, 98, 99}}},
    {{{111, 112, 113}}, {{108, 109, 110}}},
    {{{115, 116, 117}}, {{112, 113, 114}}},
};
static const short s_seqDataActPlateTbl[3][2] = {{99, 96}, {110, 107}, {114, 111}};

/* The file's inline helpers, whose strings follow the headers' in .rodata (then CSaveLoadUI::main's): the CTD text with
 * its "" fallback and the memory card slot name (createCheckingMsg, exeCheckSlot). */
static inline const CHAR* _ctdText(UINT nID) {
    const CHAR* p = CCtdMan::getText(nID);
    return p ? p : "";
}
static inline void _getSlotName(INT nSlot, CHAR* pszOutStr) {
    char szNum[16];
    if (nSlot == 12) {
        strcpy(szNum, "s 1 and 2");
    } else {
        strcpy(szNum, nSlot == 2 ? " 2" : " 1");
    }
    const CHAR* pszText = _ctdText(0x600001);
    sprintf(pszOutStr, "%s%s", pszText, szNum);
}

class CSaveLoadUI : public CTask, public CFSM {
public:
    enum LIGHTMARK_TYPE {
        LIGHTMARK_TYPE_RESULT = 0,
        LIGHTMARK_TYPE_CURSOR = 1
    };

private:
    CStateTemplate<CSaveLoadUI> m_State[19];     /* 0x06c */
    INT m_nNextState;                            /* 0x2cc */
    HANDLE m_hParent;                            /* 0x2d0 */
    SAVELOAD m_mode;                             /* 0x2d4 */
    CMemocaAgent* m_pMemoca;                     /* 0x2d8 */
    CMemocaDataEntries* m_pGameDataEntries;      /* 0x2dc */
    HANDLE m_hLay;                               /* 0x2e0 */
    HANDLE m_hCategory;                          /* 0x2e4 */
    CHAR* m_pszLad;                              /* 0x2e8 */
    INT m_nLastResult;                           /* 0x2ec */
    SHORT m_nMainSeqUnit;                        /* 0x2f0 */
    SHORT m_nIconSeqUnit;                        /* 0x2f2 */
    SHORT m_nFlag;                               /* 0x2f4 */
    SHORT m_nSlot;                               /* 0x2f6 */
    SHORT m_nDlgType;                            /* 0x2f8 */
    SHORT m_nDlgResult;                          /* 0x2fa */
    SHORT m_nDlgSel;                             /* 0x2fc */
    C2DSeqAgent* m_pMainFrameAgent;              /* 0x300 */
    C2DSeqAgent* m_pRoomNameAgent;               /* 0x304 */
    D2::C2DSeqCtrl* m_pRoomNameCtrl;             /* 0x308 */
    C2DSeqAgent* m_pWorldIconAgent;              /* 0x30c */
    D2::C2DSeqCtrl* m_pWorldIconCtrl;            /* 0x310 */
    C2DSeqAgent* m_pMainFrameHelpAgent;          /* 0x314 */
    D2::C2DSeqCtrl* m_pMainFrameHelpCtrl;        /* 0x318 */
    D2::C2DFnt* m_pMainFrameHelpFnt;             /* 0x31c */
    char m_szMainFrameHelpStr[128];              /* 0x320 */
    C2DSeqCursorCtrl* m_pMainCursor;             /* 0x3a0 */
    C2DSeqLightMarkAgent* m_pMainLightMark;      /* 0x3a4 */
    C2DSeqAgent* m_pTimeLabelAgent;              /* 0x3a8 */
    D2::C2DSeqCtrl* m_pTimeLabelCtrl;            /* 0x3ac */
    C2DSeqTimeCtrl* m_pTimeCtrl;                 /* 0x3b0 */
    C2DSeqAgent* m_pMPLabelAgent;                /* 0x3b4 */
    D2::C2DSeqCtrl* m_pMPLabelCtrl;              /* 0x3b8 */
    C2DSeqNumericCtrl* m_pMPCtrl;                /* 0x3bc */
    D2::C2DSeqCtrl* m_pResultPlateCtrl[2];       /* 0x3c0 */
    C2DSeqAgent* m_pResultPlateAgent[2];         /* 0x3c8 */
    D2::C2DFnt* m_pResultPlateFnt[2];            /* 0x3d0 */
    C2DSeqLightMarkAgent* m_pResultLightAgent[2]; /* 0x3d8 */
    C2DSeqSelectable* m_pSelSlotPlate[2];        /* 0x3e0 */
    C2DSeqAgent* m_pMemocaInfoAgent;             /* 0x3e8 */
    D2::C2DSeqCtrl* m_pMemocaInfoCtrl;           /* 0x3ec */
    D2::C2DSeqCtrl* m_pMemocaInfoBar[3];         /* 0x3f0 */
    char m_szMemocaInfoStr[3][32];               /* 0x3fc */
    C2DSeqAgent* m_pDataListContainerAgent;      /* 0x45c */
    D2::C2DSeqCtrl* m_pDataListContainerCtrl;    /* 0x460 */
    C2DSeqSelectable* m_pDataItemPlate[4];       /* 0x464 */
    D2::C2DSeqCtrl* m_pDataItemNumCircle[4];     /* 0x474 */
    D2::C2DFnt* m_pDataItemNumFnt[4];            /* 0x484 */
    D2::C2DSeqCtrl* m_pDataItemIcon[4];          /* 0x494 */
    D2::C2DSeqCtrl* m_pDataItemClearIcon[4];     /* 0x4a4 */
    char m_szDataItemContent[4][128];            /* 0x4b4 */
    char m_szDataItemID[4][4];                   /* 0x6b4 */
    char m_szDataItemTime[4][8];                 /* 0x6c4 */
    C2DSeqScrollbarCtrl* m_pDataListScrollbar;   /* 0x6e4 */
    SHORT m_nDataListTop;                        /* 0x6e8 */
    SHORT m_nDataListSel;                        /* 0x6ea */
    SHORT m_nStep;                               /* 0x6ec */
    SHORT m_nTimer;                              /* 0x6ee */
    SHORT m_nExecError;                          /* 0x6f0 */
    SHORT m_nTargetIndex;                        /* 0x6f2 */
    char m_szTargetFile[128];                    /* 0x6f4 */
    C2DSeqAgent* m_pMessageDlgAgent;             /* 0x774 */
    D2::C2DSeqCtrl* m_pMessageDlgCtrl;           /* 0x778 */
    D2::C2DFnt* m_pMessageDlgFntInf;             /* 0x77c */
    char m_szMessageDlgStr[256];                 /* 0x780 */
    C2DSeqSelectable* m_pMessageDlgBtnAgent[2];  /* 0x880 */
    C2DSeqCursorCtrl* m_pMessageDlgCursor;       /* 0x888 */
    static HANDLE m_hThis;

public:
    CSaveLoadUI(CSaveLoad& parent, SAVELOAD mode, GAMECLEAR clear);
    virtual ~CSaveLoadUI();
    virtual void main() {
        if ((m_nFlag & 4) && exeMessageDlg()) return;
        INT nCount = 10;
        BOOL bContinue;
        do {
            update();
            bContinue = m_nFlag & 1;
            if (bContinue) {
                m_nFlag &= ~1;
                if (--nCount == 0) {
                    TRACE("CSaveLoadUI \x82\xcc update() \x82\xaa\x89\xf1\x82\xe8\x82\xb7\x82\xac\x81" "c\n");
                    bContinue = FALSE;
                }
            }
        } while (bContinue);
        if (m_pTimeCtrl) m_pTimeCtrl->update();
    }
    void changeState(INT nNextState) { gotoState(m_State[nNextState]); }
    void changeNextState() {
        if (m_nNextState >= 0) changeState(m_nNextState);
    }

private:
    INT getRsrcType() const { return m_mode != SLMODE_SAVE; }
    void continueProc() { m_nFlag |= 1; }
    void preInit();
    void exeInit();
    void endInit();
    void preCheckBoot();
    void exeCheckBoot();
    void preLayIn();
    void exeLayIn();
    void preLayOut();
    void exeLayOut();
    void preExit();
    void exeExit();
    void preNop();
    void exeNop();
    void preOpenSlot();
    void exeOpenSlot();
    void preSelSlot();
    void exeSelSlot();
    void endSelSlot();
    void preCheckSlot();
    void exeCheckSlot();
    void endCheckSlot();
    void preOpenDataList();
    void exeOpenDataList();
    void preSelDataList();
    void exeSelDataList();
    void endSelDataList();
    void backtoSelSlot();
    void preSaveData();
    void exeSaveData();
    void preLoadData();
    void exeLoadData();
    void preCheckOverwrite();
    void exeCheckOverwrite();
    void preCheckNoMemory();
    void exeCheckNoMemory();
    void preCheckNoCard();
    void exeCheckNoCard();
    void preCheckUnformat();
    void exeCheckUnformat();
    void preConfirmExecErr();
    void exeConfirmExecErr();
    void preMessageOnly();
    void exeMessageOnly();
    C2DSeqAgent* createMainFrame();
    C2DSeqAgent* createWorldIcon(D2::C2DSeqCtrl*& pOutCtrl);
    C2DSeqAgent* createRoomName(D2::C2DSeqCtrl*& pOutCtrl);
    C2DSeqCursorCtrl* createCursor();
    C2DSeqLightMarkAgent* createLightMark(LIGHTMARK_TYPE nType, INT nLayer, BOOL bFollow);
    C2DSeqAgent* createTimeLabel(D2::C2DSeqCtrl*& pOutCtrl);
    C2DSeqTimeCtrl* createTimeCtrl();
    C2DSeqAgent* createMPLabel(D2::C2DSeqCtrl*& pOutCtrl);
    C2DSeqNumericCtrl* createMPCtrl();
    C2DSeqAgent* createMainHelp(D2::C2DSeqCtrl*& pOutCtrl, D2::C2DFnt*& pOutFnt);
    C2DSeqAgent* createResultPlate(D2::C2DSeqCtrl*& pOutCtrl, D2::C2DFnt*& pOutFnt, INT nIdx);
    C2DSeqSelectable* createSelSlotPlate(INT nIdx);
    C2DSeqAgent* createMemocaInfo(D2::C2DSeqCtrl*& pOutCtrl, D2::C2DSeqCtrl** ppOutInfoBar);
    C2DSeqAgent* createDataListContainer(D2::C2DSeqCtrl*& pOutCtrl, C2DSeqScrollbarCtrl*& pOutScrollbar);
    C2DSeqSelectable* createDataItemPlate(D2::C2DSeqCtrl& parent, D2::C2DSeqCtrl*& pOutNumCircle, D2::C2DFnt*& pOutNumFnt,
                                          D2::C2DSeqCtrl*& pOutIcon, D2::C2DSeqCtrl*& pOutClearIcon, INT nIdx);
    C2DSeqAgent* createMessageDlg(D2::C2DSeqCtrl*& pOutCtrl, D2::C2DFnt*& pOutFntInf);
    C2DSeqSelectable* createMessageDlgBtn(D2::C2DSeqCtrl* pParent, INT nIdx);
    void setActiveSlotPlate(INT nSlot);
    void setupDataPlate(SHORT nDataIdx);
    void updateMemocaInfoBar();
    void updateMemocaInfoStr();
    void setHelpMessage(const CHAR* pszMes);
    void outHelpMessage();
    void openMessageDlg(const CHAR* pszTxt, INT nIcon, INT nType);
    void closeMessageDlg();
    BOOL exeMessageDlg();
    INT getDlgResult() const { return m_nDlgResult; }
    void outAllAgent();
    void outDataList(INT nNextState);
    static void callbackMainFrame(C2DSeqAgent& agent);
    static void callbackSlotPlateIn(C2DSeqAgent& agent);
    static void callbackResultPlate(C2DSeqAgent& agent);
    static void callbackDataList(C2DSeqAgent& agent);
    static void callbackMemocaInfo(C2DSeqAgent& agent);
    static void callbackMessageDlg(C2DSeqAgent& agent);
    void lockPadControl(BOOL bLock) {
        if (bLock) m_nFlag |= 2;
        else m_nFlag &= ~2;
    }
    BOOL isPadEnable() const { return !(m_nFlag & 2); }
    INT isButtonRpt(UINT b) const {
        if (isPadEnable()) return CPadHandler::isButtonRpt(b);
        return 0;
    }
    INT isButtonTrg(UINT b) const {
        if (isPadEnable()) return CPadHandler::isButtonTrg(b);
        return 0;
    }
    BOOL isTrgOK() const { return isPadEnable() ? CPadHandler::isTrgOK() : FALSE; }
    BOOL isTrgCANCEL() const { return isPadEnable() ? CPadHandler::isTrgCANCEL() : FALSE; }
    BOOL checkSelector(SHORT& nOutIdx, SHORT nMax, INT nValDownKey, INT nValUpKey, SHORT nUpDownValue, BOOL bSetSE,
                       BOOL bLoopChk);
    BOOL checkSelectorV(SHORT& nOutIdx, SHORT nMax, BOOL bSetSE, BOOL bLoopChk) {
        return checkSelector(nOutIdx, nMax, 0x101000, 0x204000, 1, bSetSE, bLoopChk);
    }
    BOOL checkSelectorH(SHORT& nOutIdx, SHORT nMax, BOOL bSetSE, BOOL bLoopChk) {
        return checkSelector(nOutIdx, nMax, 0x408000, 0x802000, 1, bSetSE, bLoopChk);
    }
    void createBootInfoMsg(INT nRes, CHAR* pszOutMsg);
    void createCheckingMsg(INT nSlot, CHAR* pszOutMsg);
};
typedef char _check_saveloadui_size[sizeof(CSaveLoadUI) == 0x890 ? 1 : -1];


/* constant tables of the original (initializers in .rodata) */
struct _LayoutInfo {
    SHORT _nLayIdx;
    SHORT _nSeqIdx;
};
struct SEQTBL2 {
    C2DSeqAgent::SeqTbl t[2];
};

/* Access through a reference: keeps the original's order of global stores and later loads. */
template <class T> static inline T& _ref(T& x) { return x; }
static inline BOOL _isFadeDone() { return s_pFadeMask ? s_pFadeMask->isFadeDone() : TRUE; }
static inline BOOL _isAgentState(C2DSeqAgent* p, INT nState) { return p->getState() == nState; }

class CPlayer { /* PARTIAL: Game/Player/Player.h needs the game headers */
public:
    static void exportStatus();
};
class CRsrcDB { /* PARTIAL: System/RsrcDB.h needs RsrcData.h internals */
public:
    static void removeGroup(INT nGroupID, INT nMemberID);
    static class CRsrcData* getFirstRsrc(INT nRsrcType, INT nGroupID, BOOL bFollowLink);
};
class CRcf { /* PARTIAL: System/Rcf.h pulls in System/SJoint.h */
public:
    static void reqLoadRCFFront(INT nGroup, INT nMember, INT nType, BYTE* pBuff, INT nLinkOwner);
    static BOOL isCompleteLoadRCF(INT nGroup, INT nMember);
};
struct RoomInfo { /* PARTIAL: Game/SceneInfo.h needs the game headers */
    BYTE _m_0x00[0x60];
    void* _pSrcInfo;      /* 0x60 */
    void* _pStorageInfo;  /* 0x64 */
    void* _pStorageInfo2; /* 0x68 */
    BYTE getRoomNameCardID() const;
};
class CSceneInfo { /* PARTIAL: Game/SceneInfo.h needs the game headers */
public:
    static void initNewGame(BOOL bSora);
    static RoomInfo* getRoomInfo(INT nRoomID);
    static INT getCurrentWorldID(BOOL bCheck);
};
class CGame { /* PARTIAL: Game/Game.h needs the game headers */
public:
    static BOOL isRikuGame();
    static BOOL isSoraGame();
};
namespace ObjType { /* PARTIAL: System/Graphic/Effect/EffectMan.h is too heavy here (an 8-byte enum there too) */
enum eObjBit { OBJ_BIT_CLEAR = 0, OBJ_BIT_STILL_SCR = 0x80000000L, OBJ_BIT_ALL = 0xFFFEFFFFL, OBJ_BIT_TRUE_ALL = -1 };
}
class CSceneEnv { /* PARTIAL: Game/SceneEnv.h needs EffectMan.h */
public:
    static void setStop(ObjType::eObjBit iBit);
    static void setStopGauge(BOOL bStop);
};
class CPackMan { /* PARTIAL: System/Graphic/PackMan.h is not written yet */
public:
    static void changeDrawMode(INT nMode);
};

HANDLE CSaveLoadUI::m_hThis = 0;


CSaveLoadUI::CSaveLoadUI(CSaveLoad& parent, SAVELOAD mode, GAMECLEAR clear)
    : CTask(&parent, 0x4000), m_mode(mode), m_pMemoca(NULL), m_pGameDataEntries(NULL), m_hLay(0), m_nFlag(0),
      m_pMainFrameAgent(NULL), m_pRoomNameAgent(NULL), m_pRoomNameCtrl(NULL), m_pWorldIconAgent(NULL),
      m_pWorldIconCtrl(NULL), m_pMainFrameHelpAgent(NULL), m_pMainFrameHelpCtrl(NULL), m_pMainFrameHelpFnt(NULL),
      m_pMainCursor(NULL), m_pMainLightMark(NULL), m_pTimeLabelAgent(NULL), m_pTimeLabelCtrl(NULL), m_pTimeCtrl(NULL),
      m_pMPLabelAgent(NULL), m_pMPLabelCtrl(NULL), m_pMPCtrl(NULL), m_pMemocaInfoAgent(NULL), m_pMemocaInfoCtrl(NULL),
      m_pDataListContainerAgent(NULL), m_pDataListContainerCtrl(NULL), m_pDataListScrollbar(NULL), m_nDataListTop(0),
      m_nDataListSel(0), m_pMessageDlgAgent(NULL), m_pMessageDlgCtrl(NULL), m_pMessageDlgFntInf(NULL),
      m_pMessageDlgCursor(NULL) {
    ignorePause(TRUE);
    m_hParent = parent.getHandle();
    m_hThis = getHandle();
    s_pFadeMask = NULL;
    s_nMsgResult = SLMSG_RESULT_CANCEL;
    m_nSlot = CMemocaDataEntries::getLatestSlot();
    if (clear == GAMECLEAR_CLEAR) m_nFlag |= 0x40;
    if (CMainApp::isWarningPauseDisable()) m_nFlag |= 0x8000;
    CMainApp::enableWarningPause(FALSE);
    for (INT i = 0; i < 2; i++) {
        m_pResultPlateCtrl[i] = NULL;
        m_pResultPlateAgent[i] = NULL;
        m_pResultLightAgent[i] = NULL;
    }
    for (INT j = 0; j < 2; j++) {
        m_pSelSlotPlate[j] = NULL;
    }
    for (INT k = 0; k < 4; k++) {
        m_pDataItemPlate[k] = NULL;
        m_pDataItemNumCircle[k] = NULL;
        m_pDataItemNumFnt[k] = NULL;
        m_pDataItemIcon[k] = NULL;
        m_pDataItemClearIcon[k] = NULL;
    }
    for (INT l = 0; l < 3; l++) {
        m_pMemocaInfoBar[l] = NULL;
    }
    for (INT m = 0; m < 2; m++) {
        m_pMessageDlgBtnAgent[m] = NULL;
    }
    m_State[0].set(this, &CSaveLoadUI::preInit, &CSaveLoadUI::exeInit, &CSaveLoadUI::endInit);
    m_State[1].set(this, &CSaveLoadUI::preCheckBoot, &CSaveLoadUI::exeCheckBoot, NULL);
    m_State[2].set(this, &CSaveLoadUI::preLayIn, &CSaveLoadUI::exeLayIn, NULL);
    m_State[3].set(this, &CSaveLoadUI::preLayOut, &CSaveLoadUI::exeLayOut, NULL);
    m_State[4].set(this, &CSaveLoadUI::preExit, &CSaveLoadUI::exeExit, NULL);
    m_State[5].set(this, &CSaveLoadUI::preNop, &CSaveLoadUI::exeNop, NULL);
    m_State[6].set(this, &CSaveLoadUI::preOpenSlot, &CSaveLoadUI::exeOpenSlot, NULL);
    m_State[7].set(this, &CSaveLoadUI::preSelSlot, &CSaveLoadUI::exeSelSlot, &CSaveLoadUI::endSelSlot);
    m_State[8].set(this, &CSaveLoadUI::preCheckSlot, &CSaveLoadUI::exeCheckSlot, &CSaveLoadUI::endCheckSlot);
    m_State[9].set(this, &CSaveLoadUI::preOpenDataList, &CSaveLoadUI::exeOpenDataList, NULL);
    m_State[10].set(this, &CSaveLoadUI::preSelDataList, &CSaveLoadUI::exeSelDataList, &CSaveLoadUI::endSelDataList);
    m_State[11].set(this, &CSaveLoadUI::preSaveData, &CSaveLoadUI::exeSaveData, NULL);
    m_State[12].set(this, &CSaveLoadUI::preLoadData, &CSaveLoadUI::exeLoadData, NULL);
    m_State[13].set(this, &CSaveLoadUI::preCheckOverwrite, &CSaveLoadUI::exeCheckOverwrite, NULL);
    m_State[14].set(this, &CSaveLoadUI::preCheckNoMemory, &CSaveLoadUI::exeCheckNoMemory, NULL);
    m_State[15].set(this, &CSaveLoadUI::preCheckNoCard, &CSaveLoadUI::exeCheckNoCard, NULL);
    m_State[16].set(this, &CSaveLoadUI::preCheckUnformat, &CSaveLoadUI::exeCheckUnformat, NULL);
    m_State[17].set(this, &CSaveLoadUI::preConfirmExecErr, &CSaveLoadUI::exeConfirmExecErr, NULL);
    m_State[18].set(this, &CSaveLoadUI::preMessageOnly, &CSaveLoadUI::exeMessageOnly, NULL);
    if (m_mode == SLMODE_SAVE) {
        m_pszLad = "save.lad";
        m_nMainSeqUnit = 0;
        m_nIconSeqUnit = 1;
        s_nWorldIconRsrc = CSceneInfo::getCurrentWorldID(1);
        if (!s_nWorldIconRsrc) s_nWorldIconRsrc = 13;
        s_nWorldIconRsrc += 0x95;
    } else {
        m_pszLad = "load.lad";
        m_nMainSeqUnit = 0;
        m_nIconSeqUnit = 1;
        s_nWorldIconRsrc = 0;
    }
    if (mode != SLMODE_BOOT && mode != SLMODE_MESSAGE) {
        CSceneEnv::setStop(ObjType::OBJ_BIT_ALL);
        CSceneEnv::setStopGauge(TRUE);
        CSound::pauseAllSE();
    }
    changeState(0);
}
CSaveLoadUI::~CSaveLoadUI() {
    CTask* pParent = CTaskMan::getTask(m_hParent);
    if (pParent) pParent->out();
    delete m_pMemoca;
    delete m_pGameDataEntries;
    delete m_pRoomNameCtrl;
    delete m_pWorldIconCtrl;
    delete m_pMainFrameHelpCtrl;
    delete m_pMainCursor;
    delete m_pTimeLabelCtrl;
    delete m_pMPLabelCtrl;
    delete m_pTimeCtrl;
    delete m_pMPCtrl;
    for (INT i = 0; i < 2; i++) {
        delete m_pResultPlateCtrl[i];
    }
    delete m_pMemocaInfoCtrl;
    delete m_pDataListContainerCtrl;
    delete m_pDataListScrollbar;
    for (INT j = 0; j < 4; j++) {
        delete m_pDataItemNumCircle[j];
        delete m_pDataItemIcon[j];
        delete m_pDataItemClearIcon[j];
    }
    for (INT k = 0; k < 3; k++) {
        delete m_pMemocaInfoBar[k];
    }
    delete m_pMessageDlgCtrl;
    delete m_pMessageDlgCursor;
    _ref(m_hThis) = 0;
    if (m_mode != SLMODE_BOOT && m_mode != SLMODE_MESSAGE) {
        CSceneEnv::setStop(ObjType::OBJ_BIT_CLEAR);
        CSceneEnv::setStopGauge(FALSE);
        CPackMan::changeDrawMode(0);
        CSound::startAllPausedSE();
    }
    CMainApp::enableWarningPause(!(m_nFlag & 0x8000));
    D2::C2DMan::changeLevel(D2::C2DMan::D2MAN_LEVEL0);
}
void CSaveLoadUI::preInit() {
    if (!(m_mode == SLMODE_BOOT || m_mode == SLMODE_MESSAGE)) CPackMan::changeDrawMode(1);
    INT nID = !getRsrcType() ? 0x6A : 0x6B;
    if (!CRsrcDB::getFirstRsrc(0x11, nID + 20000, FALSE)) CRcf::reqLoadRCFFront(2, nID, 0, NULL, 0);
    if (s_nWorldIconRsrc) {
        INT nMember = s_nWorldIconRsrc;
        if (!CRsrcDB::getFirstRsrc(0x11, nMember + 20000, FALSE)) CRcf::reqLoadRCFFront(2, s_nWorldIconRsrc, 0, NULL, 0);
    }
    if (!CSystemDataChecker::isExited()) CSystemDataChecker::abort();
    m_nTimer = 2;
}
void CSaveLoadUI::exeInit() {
    if (m_nTimer > 0) m_nTimer--;
    INT nID = !getRsrcType() ? 0x6A : 0x6B;
    if (!CRcf::isCompleteLoadRCF(2, nID)) return;
    if (!CSystemDataChecker::isExited()) return;
    if (m_nTimer > 0) return;
    D2::C2DMan::changeLevel(D2::C2DMan::D2MAN_LEVEL1);
    switch (m_mode) {
    case SLMODE_SAVE:
        CGameWork::setByte(1, 0x140, 7);
    case SLMODE_LOAD:
        changeState(2);
        break;
    case SLMODE_BOOT:
        changeState(1);
        break;
    case SLMODE_MESSAGE:
        changeState(18);
        break;
    }
    m_pMemoca = new (com_malloc_d(sizeof(CMemocaAgent), __FILE__ "(708)", RET_ADDR())) CMemocaAgent(this);
    m_pGameDataEntries = new (com_malloc_d(sizeof(CMemocaDataEntries), __FILE__ "(709)", RET_ADDR())) CMemocaDataEntries(99);
}
void CSaveLoadUI::endInit() {}
void CSaveLoadUI::preCheckBoot() {
    TRACE_COL((TtyColor)0x1F, "\x83\x81\x83\x82\x83J\x97" "e\x97\xca\x83`\x83" "F\x83" "b\x83N\x8aJ\x8en\x81" "E\x81" "E\x81" "E\n");
    m_pMessageDlgAgent = createMessageDlg(m_pMessageDlgCtrl, m_pMessageDlgFntInf);
    for (INT i = 0; i < 2; i++) {
        m_pMessageDlgBtnAgent[i] = createMessageDlgBtn(m_pMessageDlgCtrl, i);
    }
    m_pMessageDlgCursor = createCursor();
    m_pMessageDlgCursor->changeLayer(14);
    m_pMainLightMark = createLightMark(LIGHTMARK_TYPE_CURSOR, 16, TRUE);
    for (INT j = 0; j < 2; j++) {
        m_pMessageDlgBtnAgent[j]->m_pCursor = m_pMessageDlgCursor;
        m_pMessageDlgBtnAgent[j]->m_pLightMark = m_pMainLightMark;
    }
    m_nStep = 0;
    m_nTimer = 0;
}
void CSaveLoadUI::exeCheckBoot() {
    char szMsg[256];
    switch (m_nStep) {
    case 0:
        m_nTimer--;
        if (m_pMemoca->reqCheckFreeMem()) m_nStep++;
        break;
    case 1:
        m_nTimer--;
        if (!m_pMemoca->isCompleteReq(NULL)) break;
        m_nLastResult = m_pMemoca->getLastResult();
        if (m_nLastResult >= 0) TRACE_COL((TtyColor)0x1F, "\x83\x81\x83\x82\x83J\x97" "e\x97\xca\x83`\x83" "F\x83" "b\x83N\x82n\x82j\n");
        m_nStep++;
    case 2:
        if (m_nTimer > 0) {
            m_nTimer--;
            break;
        }
        if (m_nLastResult >= 0) {
            closeMessageDlg();
            m_pMainLightMark->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
            m_nStep = 20;
        } else {
            createBootInfoMsg(m_nLastResult, szMsg);
            openMessageDlg(szMsg, 0, 2);
            m_nStep = 10;
        }
        break;
    case 10:
        if (!m_nDlgResult) break;
        if (m_nDlgResult == 1) {
            m_nLastResult = 0;
            m_nStep = 2;
        } else if (m_nDlgResult == 2) {
            createCheckingMsg(12, szMsg);
            openMessageDlg(szMsg, 0, 0);
            m_nStep = 0;
            m_nTimer = 60;
        }
        break;
    case 20:
        if (isPadEnable()) changeState(4);
        break;
    }
}
void CSaveLoadUI::preLayIn() {
    m_pMainFrameAgent = createMainFrame();
    m_pMainFrameAgent->start();
    CSound::playSE(3, -1, 64);
    m_pMainCursor = createCursor();
    m_pMainCursor->changeLayer(13);
    m_pMainLightMark = createLightMark(LIGHTMARK_TYPE_CURSOR, 16, TRUE);
    if (m_mode == SLMODE_SAVE) {
        m_pWorldIconAgent = createWorldIcon(m_pWorldIconCtrl);
        m_pWorldIconAgent->start();
        m_pRoomNameAgent = createRoomName(m_pRoomNameCtrl);
        m_pRoomNameAgent->start();
        m_pTimeLabelAgent = createTimeLabel(m_pTimeLabelCtrl);
        m_pTimeLabelAgent->start();
        m_pTimeCtrl = createTimeCtrl();
        if (CGame::isSoraGame()) {
            m_pMPLabelAgent = createMPLabel(m_pMPLabelCtrl);
            m_pMPLabelAgent->start();
            m_pMPCtrl = createMPCtrl();
        }
    }
    m_pMainFrameHelpAgent = createMainHelp(m_pMainFrameHelpCtrl, m_pMainFrameHelpFnt);
    for (INT i = 0; i < 2; i++) {
        m_pResultPlateAgent[i] = createResultPlate(m_pResultPlateCtrl[i], m_pResultPlateFnt[i], i);
        m_pResultLightAgent[i] = createLightMark(LIGHTMARK_TYPE_RESULT, 13, FALSE);
    }
    m_pResultPlateAgent[0]->start();
    for (INT i = 0; i < 2; i++) {
        m_pSelSlotPlate[i] = createSelSlotPlate(i);
        m_pSelSlotPlate[i]->attachCursor(m_pMainCursor);
        m_pSelSlotPlate[i]->attachLightMark(m_pMainLightMark);
    }
    m_pMemocaInfoAgent = createMemocaInfo(m_pMemocaInfoCtrl, m_pMemocaInfoBar);
    m_pDataListContainerAgent = createDataListContainer(m_pDataListContainerCtrl, m_pDataListScrollbar);
    for (INT i = 0; i < 4; i++) {
        *m_szDataItemContent[i] = '\0';
        *m_szDataItemID[i] = '\0';
        *m_szDataItemTime[i] = '\0';
        m_pDataItemPlate[i] = createDataItemPlate(*m_pDataListContainerCtrl, m_pDataItemNumCircle[i], m_pDataItemNumFnt[i],
                                                  m_pDataItemIcon[i], m_pDataItemClearIcon[i], i);
        m_pDataItemPlate[i]->attachCursor(m_pMainCursor);
        m_pDataItemPlate[i]->attachLightMark(m_pMainLightMark);
    }
    m_pMessageDlgAgent = createMessageDlg(m_pMessageDlgCtrl, m_pMessageDlgFntInf);
    for (INT i = 0; i < 2; i++) {
        m_pMessageDlgBtnAgent[i] = createMessageDlgBtn(m_pMessageDlgCtrl, i);
    }
    m_pMessageDlgCursor = createCursor();
    m_pMessageDlgCursor->changeLayer(14);
    for (INT i = 0; i < 2; i++) {
        m_pMessageDlgBtnAgent[i]->attachCursor(m_pMessageDlgCursor);
        m_pMessageDlgBtnAgent[i]->attachLightMark(m_pMainLightMark);
    }
}
void CSaveLoadUI::exeLayIn() {}
void CSaveLoadUI::preLayOut() {
    if (m_mode == SLMODE_LOAD && s_bSaveLoadDone) {
        _ref(s_pFadeMask) = new (com_malloc_d(sizeof(CFadeIOMask), __FILE__ "(915)", RET_ADDR()))
            CFadeIOMask(*CTaskMan::getTask(m_hParent)->getParent(), 0x18);
        s_pFadeMask->reqBlackOut(30, 1.0f);
        CSound::fadeoutAllBGM(30);
        m_nStep = 0;
    } else {
        outAllAgent();
        CSound::playSE(4, -1, 64);
    }
}
void CSaveLoadUI::exeLayOut() {
    if (m_mode != SLMODE_LOAD) return;
    if (m_nStep != 0) return;
    if (_isFadeDone()) {
        outAllAgent();
        m_nStep++;
    }
}
void CSaveLoadUI::preExit() {
    CGameWork::setByte(1, 0x140, 0);
    out();
}
void CSaveLoadUI::exeExit() {}
void CSaveLoadUI::preNop() {}
void CSaveLoadUI::exeNop() {}
void CSaveLoadUI::preOpenSlot() {
    for (INT i = 0; i < 2; i++) {
        m_pSelSlotPlate[i]->startOrRestart();
    }
}
void CSaveLoadUI::exeOpenSlot() {}
void CSaveLoadUI::preSelSlot() {
    setActiveSlotPlate(m_nSlot);
    m_pMainLightMark->m_nFlag &= ~2;
}
void CSaveLoadUI::exeSelSlot() {
    if (isTrgCANCEL()) {
        changeState(3);
        return;
    }
    if (isTrgOK()) {
        changeState(8);
        for (INT i = 0; i < 2; i++) {
            m_pSelSlotPlate[i]->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
        }
        CSound::playSE(2, -1, 64);
        return;
    }
    if (checkSelectorV(m_nSlot, 2, TRUE, TRUE)) setActiveSlotPlate(m_nSlot);
}
void CSaveLoadUI::endSelSlot() {
    if (m_pMainCursor) m_pMainCursor->enableDisp(FALSE);
    if (m_pMainLightMark && m_pMainLightMark->isSeqCtrlVisible()) {
        m_pMainLightMark->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
        m_pMainLightMark->m_nFlag |= 2;
    }
    outHelpMessage();
}
void CSaveLoadUI::preCheckSlot() {
    CHAR szMsg[256];
    m_pResultPlateAgent[1]->startOrRestart();
    m_pMemoca->setCurrentSlot(m_nSlot);
    createCheckingMsg(m_nSlot + 1, szMsg);
    openMessageDlg(szMsg, 0, 0);
    m_nDataListTop = 0;
    m_nDataListSel = 0;
    m_pGameDataEntries->clearAll();
    m_nStep = 0;
    m_nTargetIndex = 0;
}
void CSaveLoadUI::exeCheckSlot() {
    INT nRes;
    UINT nIdx;
    switch (m_nStep) {
    case 0:
        if (m_pMemoca->reqGetEntries(*m_pGameDataEntries, m_mode == SLMODE_SAVE)) m_nStep++;
        return;
    case 1:
        if (!m_pMemoca->isCompleteReq(NULL)) return;
        m_nStep++;
        break;
    case 2:
        if (isPadEnable()) changeState(9);
        return;
    case 10: {
        char szMsg[256];
        char szSlot[32];
        _getSlotName(m_nSlot + 1, szSlot);
        const CHAR* pszFmt = _ctdText(0x600302);
        sprintf(szMsg, pszFmt, _ctdText(0x600000), szSlot);
        openMessageDlg(szMsg, 0, 1);
        m_nStep++;
        return;
    }
    case 11:
        if (!m_nDlgResult) return;
        closeMessageDlg();
        m_nStep++;
        return;
    case 12:
        if (isPadEnable()) backtoSelSlot();
        return;
    }
    nRes = m_pMemoca->getLastResult();
    if (nRes < 0) {
        BOOL bExit = TRUE;
        switch (nRes) {
        case -2:
            changeState(15);
            break;
        case -4:
            if (m_mode == SLMODE_SAVE) {
                changeState(14);
                m_nFlag &= ~0x80;
            } else {
                bExit = FALSE;
            }
            break;
        case -3:
            bExit = FALSE;
            break;
        default:
            m_nStep = 10;
            return;
        }
        if (bExit) {
            m_nStep = -1;
            return;
        }
    }
    nIdx = m_pMemoca->getLatestDataIndex();
    m_nDataListSel = nIdx;
    if (nIdx >= 96) {
        m_nDataListTop = 95;
    } else {
        m_nDataListTop = nIdx < 4 ? 0 : nIdx - 3;
    }
    for (INT i = 0; i < 4; i++) {
        setupDataPlate(m_nDataListTop + i);
    }
    if (m_nFlag & 4) closeMessageDlg();
}
void CSaveLoadUI::endCheckSlot() {}
void CSaveLoadUI::preOpenDataList() {
    if (m_pMemocaInfoAgent) m_pMemocaInfoAgent->startOrRestart();
    m_pDataListContainerAgent->startOrRestart();
    m_pDataListScrollbar->setScrollPos(m_nDataListTop, TRUE);
    m_pDataListScrollbar->startOrRestart();
    for (INT i = 0; i < 4; i++) {
        m_pDataItemPlate[i]->startOrRestart();
        m_pDataItemPlate[i]->enableDispCaption(FALSE);
        m_pDataItemNumCircle[i]->enableDisp(FALSE);
        m_pDataItemIcon[i]->enableDisp(FALSE);
        m_pDataItemClearIcon[i]->enableDisp(FALSE);
    }
}
void CSaveLoadUI::exeOpenDataList() {}
void CSaveLoadUI::preSelDataList() {
    SHORT n = m_nDataListSel - m_nDataListTop;
    CHAR* pszMes;
    if (n < 0) {
        m_nDataListTop = m_nDataListSel;
        if (!(0 <= m_nDataListTop)) AssertApp(__FILE__, 1185, "0 <= m_nDataListTop", 0);
    } else if (n >= 4) {
        m_nDataListTop = m_nDataListSel - 3;
        if (!(0 <= m_nDataListTop)) AssertApp(__FILE__, 1190, "0 <= m_nDataListTop", 0);
    }
    m_pMainLightMark->setSleep(FALSE);
    m_pDataItemPlate[(SHORT)(m_nDataListSel - m_nDataListTop)]->select(TRUE);
    if (getRsrcType()) {
        pszMes = "Select file to load";
    } else {
        pszMes = "Select file to save";
    }
    setHelpMessage(pszMes);
    m_nStep = 0;
}
void CSaveLoadUI::exeSelDataList() {
    if (m_pDataListContainerAgent->getState() != C2DSeqAgent::STATE_LOOP) return;
    INT r = -9999;
    switch (m_nStep) {
    case 0:
        if (m_pMemoca->reqIdleCheckCard()) m_nStep++;
        break;
    case 1:
        if (m_pMemoca->isCompleteReq(NULL)) {
            r = m_pMemoca->getLastResult();
            m_nStep = 0;
        }
        break;
    default:
        return;
    }
    if (r == -2) {
        changeState(15);
        return;
    }
    if (isTrgCANCEL()) {
        m_nStep = 99;
        backtoSelSlot();
        CSound::playSE(4, -1, 64);
        return;
    }
    if (isTrgOK()) {
        INT nNext = -1;
        GameDataHeader* p = m_pGameDataEntries->getEntry(m_nDataListSel);
        if (m_mode == SLMODE_SAVE) {
            if (!p) {
                m_nFlag |= 0x80;
                nNext = 14;
            } else {
                m_nFlag &= ~0x100;
                if (m_pGameDataEntries->isUnformated()) {
                    nNext = 16;
                } else if (p->isDirBroken()) {
                    CSound::playSE(5, -1, 64);
                } else if (p->isDataBroken()) {
                    if (!m_pGameDataEntries->getDataUnitCountByType(0)) {
                        m_nFlag |= 0x80;
                        nNext = 14;
                    } else {
                        nNext = 11;
                    }
                } else {
                    nNext = p->hasData() ? 13 : 11;
                }
            }
        } else {
            if (p && p->hasData()) {
                nNext = 12;
            } else {
                CSound::playSE(5, -1, 64);
            }
        }
        if (nNext >= 0) {
            changeState(nNext);
            continueProc();
            CSound::playSE(2, -1, 64);
            m_nStep = 99;
            return;
        }
    }
    SHORT nOldOfs = m_nDataListSel - m_nDataListTop;
    SHORT nNewTop = m_nDataListTop;
    BOOL bUpdate = checkSelectorV(m_nDataListSel, 99, TRUE, TRUE);
    if (bUpdate) {
        SHORT n = m_nDataListSel - m_nDataListTop;
        if (n < 0) {
            nNewTop = m_nDataListSel;
        } else if (n >= 4) {
            nNewTop = m_nDataListSel - 3;
        }
    } else {
        bUpdate = checkSelector(m_nDataListSel, 99, 4, 8, 4, TRUE, TRUE);
        if (!bUpdate) return;
        nNewTop = m_nDataListSel - nOldOfs;
        if (nNewTop < 0) {
            nNewTop = 0;
        } else if (nNewTop >= 96) {
            nNewTop = 95;
        }
    }
    if (!bUpdate) return;
    if (nNewTop != m_nDataListTop) {
        m_nDataListTop = nNewTop;
        for (INT i = 0; i < 4; i++) {
            setupDataPlate(m_nDataListTop + i);
        }
        m_pDataListScrollbar->setScrollPos(m_nDataListTop, TRUE);
    }
    SHORT nOfs = m_nDataListSel - m_nDataListTop;
    if (nOfs != nOldOfs) m_pDataItemPlate[nOldOfs]->select(FALSE);
    m_pDataItemPlate[nOfs]->select(TRUE);
}
void CSaveLoadUI::endSelDataList() {
    outHelpMessage();
}
void CSaveLoadUI::backtoSelSlot() {
    outDataList(6);
}
void CSaveLoadUI::preSaveData() {
    char szSlot[32];
    _getSlotName(m_nSlot + 1, szSlot);
    char szMsg[256];
    const CHAR* pszFmt = _ctdText(0x600203);
    const CHAR* pszText = _ctdText(0x600000);
    sprintf(szMsg, pszFmt, pszText, szSlot, pszText);
    openMessageDlg(szMsg, 0, 0);
    m_nStep = 0;
}
void CSaveLoadUI::exeSaveData() {
    switch (m_nStep) {
    case 0:
        if (m_pMemoca->reqLoadSysData()) m_nStep++;
        break;
    case 1:
        if (!m_pMemoca->isCompleteReq(NULL)) break;
        m_nStep++;
    case 2:
        if (m_pMemoca->reqSaveGameData(m_nDataListSel, *m_pGameDataEntries, (m_nFlag & 0x40) != 0)) m_nStep++;
        break;
    case 3:
        if (!m_pMemoca->isCompleteReq(NULL)) break;
        m_nLastResult = m_pMemoca->getLastResult();
        if (m_nLastResult < 0) {
            m_nExecError = 2;
            m_nStep = 10;
            break;
        }
        m_nStep++;
    case 4:
        if (m_pMemoca->reqSaveSysData()) m_nStep++;
        break;
    case 5:
        if (!m_pMemoca->isCompleteReq(NULL)) break;
        m_nLastResult = m_pMemoca->getLastResult();
        if (m_nLastResult < 0) m_nExecError = 3;
        m_nStep = 10;
        break;
    case 10:
        closeMessageDlg();
        m_nStep++;
        break;
    case 11:
        if (!isPadEnable()) break;
        if (m_nLastResult >= 0) {
            _ref(s_bSaveLoadDone) = TRUE;
            if (!(m_nFlag & 0x100)) m_pGameDataEntries->useOneSavableUnit(m_nDataListSel);
            for (INT i = 0; i < 4; i++) {
                setupDataPlate(m_nDataListTop + i);
            }
            updateMemocaInfoStr();
            updateMemocaInfoBar();
            m_pGameDataEntries->setLatestEntryIndex(m_nSlot, m_nDataListSel);
            CSound::playSE(11, -1, 64);
            changeState(10);
        } else {
            changeState(17);
        }
        break;
    }
}
void CSaveLoadUI::preLoadData() {
    char szSlot[32];
    _getSlotName(m_nSlot + 1, szSlot);
    char szMsg[256];
    const CHAR* pszFmt = _ctdText(0x600300);
    const CHAR* pszText = _ctdText(0x600000);
    sprintf(szMsg, pszFmt, pszText, szSlot, pszText);
    openMessageDlg(szMsg, 0, 0);
    GameDataHeader* p = m_pGameDataEntries->getEntry(m_nDataListSel);
    if (!p) AssertApp(__FILE__, 1474, "p", 0);
    CGameWork::resetAll();
    CSceneInfo::initNewGame(p->_cFlag & 1);
    m_nStep = 0;
}
void CSaveLoadUI::exeLoadData() {
    switch (m_nStep) {
    case 0:
        if (m_pMemoca->reqLoadSysData()) m_nStep++;
        break;
    case 1:
        if (!m_pMemoca->isCompleteReq(NULL)) break;
        m_nStep++;
    case 2:
        if (m_pMemoca->reqLoadGameData(m_nDataListSel)) m_nStep++;
        break;
    case 3:
        if (!m_pMemoca->isCompleteReq(NULL)) break;
        closeMessageDlg();
        m_nStep++;
    case 4:
        if (!isPadEnable()) break;
        if (m_pMemoca->getLastResult() == 0) {
            s_bSaveLoadDone = TRUE;
            changeState(3);
            CSound::playSE(11, -1, 64);
            m_pGameDataEntries->setLatestEntryIndex(m_nSlot, m_nDataListSel);
        } else {
            m_nExecError = 1;
            changeState(17);
        }
        break;
    }
}
void CSaveLoadUI::preCheckOverwrite() {
    openMessageDlg(_ctdText(0x600200), 1, 3);
    m_nStep = 0;
}
void CSaveLoadUI::exeCheckOverwrite() {
    INT r = -9999;
    switch (m_nStep) {
    case 0:
        if (m_pMemoca->reqIdleCheckCard()) m_nStep++;
        break;
    case 1:
        if (m_pMemoca->isCompleteReq(NULL)) {
            r = m_pMemoca->getLastResult();
            m_nStep = 0;
        }
        break;
    }
    if (r == -2) {
        changeState(15);
        return;
    }
    switch (m_nStep) {
    case 0:
    case 1:
        if (m_nDlgResult) {
            if (m_nDlgResult == 1) {
                m_nFlag |= 0x100;
                changeState(11);
                continueProc();
            } else {
                closeMessageDlg();
                m_nStep = 2;
            }
        }
        break;
    case 2:
        if (isPadEnable()) changeState(10);
        break;
    }
}
void CSaveLoadUI::preCheckNoMemory() {
    char szSlot[32];
    _getSlotName(m_nSlot + 1, szSlot);
    char szLine1[128];
    CCtdMan::getText(0x600201);
    CHAR* pszLine1 = szLine1;
    const CHAR* pszFmt = _ctdText(0x600201);
    sprintf(pszLine1, pszFmt, _ctdText(0x600000), szSlot);
    char szLine2[128];
    CHAR* pszLine2 = szLine2;
    const CHAR* pszFmt2 = _ctdText(0x60020A);
    sprintf(pszLine2, pszFmt2, 100, _ctdText(0x100000));
    char szMsg[256];
    sprintf(szMsg, "%s\n%s", pszLine1, pszLine2);
    openMessageDlg(szMsg, 0, 1);
    m_nStep = 0;
}
void CSaveLoadUI::exeCheckNoMemory() {
    INT r = -9999;
    switch (m_nStep) {
    case 0:
        if (m_pMemoca->reqIdleCheckCard()) m_nStep++;
        break;
    case 1:
        if (m_pMemoca->isCompleteReq(NULL)) {
            r = m_pMemoca->getLastResult();
            m_nStep = 0;
        }
        break;
    }
    if (r == -2) {
        changeState(15);
        return;
    }
    switch (m_nStep) {
    case 0:
    case 1:
        if (m_nDlgResult) {
            closeMessageDlg();
            m_nStep = 2;
        }
        break;
    case 2:
        if (isPadEnable()) {
            if (m_nFlag & 0x80) {
                changeState(10);
            } else {
                backtoSelSlot();
            }
            m_nStep = 99;
        }
        break;
    }
}
void CSaveLoadUI::preCheckNoCard() {
    char szSlot[32];
    _getSlotName(m_nSlot + 1, szSlot);
    char szMsg[256];
    const CHAR* pszFmt = _ctdText(0x600202);
    sprintf(szMsg, pszFmt, _ctdText(0x600000), szSlot);
    openMessageDlg(szMsg, 0, 1);
    m_nStep = 0;
}
void CSaveLoadUI::exeCheckNoCard() {
    INT nResult = -9999;
    switch (m_nStep) {
    case 0:
        if (m_pMemoca->reqIdleCheckCard()) m_nStep++;
        break;
    case 1:
        if (m_pMemoca->isCompleteReq(NULL)) {
            nResult = m_pMemoca->getLastResult();
            m_nStep = 0;
        }
        break;
    default:
        return;
    }
    if (nResult == 0) {
        closeMessageDlg();
        outDataList(8);
        m_nStep = 99;
        return;
    }
    if (m_nDlgResult) {
        m_nStep = 99;
        closeMessageDlg();
        backtoSelSlot();
    }
}
void CSaveLoadUI::preCheckUnformat() {
    char szSlot[32];
    _getSlotName(m_nSlot + 1, szSlot);
    char szLine1[128];
    const CHAR* pszFmt = _ctdText(0x600205);
    sprintf(szLine1, pszFmt, _ctdText(0x600000), szSlot);
    char szMsg[256];
    sprintf(szMsg, "%s\n%s", szLine1, _ctdText(0x600206));
    openMessageDlg(szMsg, 1, 3);
    m_nStep = 0;
}
void CSaveLoadUI::exeCheckUnformat() {
    INT r = -9999;
    switch (m_nStep) {
    case 0:
        if (m_pMemoca->reqIdleCheckCard()) m_nStep++;
        break;
    case 1:
        if (m_pMemoca->isCompleteReq(NULL)) {
            r = m_pMemoca->getLastResult();
            m_nStep = 0;
        }
        break;
    }
    if (r == -2) {
        changeState(15);
        return;
    }
    switch (m_nStep) {
    case 0:
    case 1:
        if (m_nDlgResult) {
            closeMessageDlg();
            m_nStep = 2;
        }
        break;
    case 2:
        if (isPadEnable()) {
            if (m_nDlgResult == 1) {
                m_nStep = 10;
            } else {
                changeState(10);
            }
        }
        break;
    case 10: {
        char szSlot[32];
        _getSlotName(m_nSlot + 1, szSlot);
        char szMsg[256];
        const CHAR* pszFmt = _ctdText(0x600207);
        const CHAR* pszText = _ctdText(0x600000);
        sprintf(szMsg, pszFmt, pszText, szSlot, pszText);
        openMessageDlg(szMsg, 0, 0);
        m_nStep++;
        break;
    }
    case 11:
        if (CMemoca::reqFormat(m_nSlot)) m_nStep++;
        break;
    case 12:
        r = CMemoca::formatProc();
        if (r == -9999) break;
        m_nLastResult = r;
        if (r == 0) {
            m_pGameDataEntries->formated();
            m_nStep = 20;
        } else {
            m_nExecError = 4;
            changeState(17);
        }
        break;
    case 20:
        closeMessageDlg();
        m_nStep++;
        break;
    case 21:
        if (isPadEnable()) {
            changeState(11);
            continueProc();
        }
        break;
    }
}
void CSaveLoadUI::preConfirmExecErr() {
    INT nCtd;
    switch (m_nExecError) {
    case 2:
        nCtd = 0x600204;
        break;
    case 3:
        nCtd = 0x60020B;
        break;
    case 4:
        nCtd = 0x600208;
        break;
    case 1:
    default:
        nCtd = 0x600301;
        break;
    }
    char szMsg[256];
    CHAR* pszMsg = szMsg;
    char szSlot[32];
    _getSlotName(m_nSlot + 1, szSlot);
    const CHAR* pszFmt = _ctdText(nCtd);
    sprintf(szMsg, pszFmt, _ctdText(0x600000), szSlot);
    openMessageDlg(pszMsg, 0, 1);
    m_nStep = 0;
}
void CSaveLoadUI::exeConfirmExecErr() {
    switch (m_nStep) {
    case 0:
        if (m_pMemoca->reqIdleCheckCard()) m_nStep++;
        break;
    case 1:
        if (m_pMemoca->isCompleteReq(NULL)) m_nStep = 0;
        break;
    }
    switch (m_nStep) {
    case 0:
    case 1:
        if (m_nDlgResult) {
            closeMessageDlg();
            m_nStep = 2;
        }
        break;
    case 2:
        if (isPadEnable()) backtoSelSlot();
        break;
    }
}
void CSaveLoadUI::preMessageOnly() {
    m_pMessageDlgAgent = createMessageDlg(m_pMessageDlgCtrl, m_pMessageDlgFntInf);
    for (INT i = 0; i < 2; i++) {
        m_pMessageDlgBtnAgent[i] = createMessageDlgBtn(m_pMessageDlgCtrl, i);
    }
    m_pMessageDlgCursor = createCursor();
    m_pMessageDlgCursor->changeLayer(14);
    m_pMainLightMark = createLightMark(LIGHTMARK_TYPE_CURSOR, 16, TRUE);
    for (INT i = 0; i < 2; i++) {
        m_pMessageDlgBtnAgent[i]->attachCursor(m_pMessageDlgCursor);
        m_pMessageDlgBtnAgent[i]->attachLightMark(m_pMainLightMark);
    }
    m_nStep = 0;
    m_nTimer = 0;
}
void CSaveLoadUI::exeMessageOnly() {
    switch (m_nStep) {
    case 0: {
        BOOL bSoraClear = CGameWork::getFlag(0, 9);
        INT nCtd = 0;
        INT nIcon;
        INT nType;
        switch (s_nMessage) {
        case 0:
            nCtd = bSoraClear ? 0x600501 : 0x600500;
            nIcon = 0;
            nType = 1;
            break;
        case 1:
            nCtd = bSoraClear ? 0x600503 : 0x600502;
            nIcon = 1;
            nType = 3;
            break;
        }
        if (!nCtd) {
            changeState(4);
            break;
        }
        openMessageDlg(_ctdText(nCtd), nIcon, nType);
        m_nStep++;
        break;
    }
    case 1:
        if (!m_nDlgResult) break;
        {
            SLMSG_RESULT r;
            switch (m_nDlgResult) {
            case 1:
                r = SLMSG_RESULT_OK;
                break;
            case 2:
                r = SLMSG_RESULT_NO;
                break;
            default:
                r = SLMSG_RESULT_CANCEL;
                break;
            }
            s_nMsgResult = r;
        }
        closeMessageDlg();
        m_nStep++;
        break;
    case 2:
        if (isPadEnable()) changeState(4);
        break;
    }
}
C2DSeqAgent* CSaveLoadUI::createMainFrame() {
    C2DSeqAgent::SeqTbl tbl[2] = {{{2, 4, 5}}, {{3, 5, 6}}};
    C2DSeqAgent* pAgent = new (com_malloc_d(sizeof(C2DSeqAgent), __FILE__ "(1990)", RET_ADDR())) C2DSeqAgent(this);
    pAgent->setupSeq(m_pszLad, 11);
    pAgent->setupState(tbl[getRsrcType()], callbackMainFrame, callbackMainFrame);
    return pAgent;
}
C2DSeqAgent* CSaveLoadUI::createWorldIcon(D2::C2DSeqCtrl*& pOutCtrl) {
    static CHAR* s_pszWorldIcon = "world_title.lad";
    if (m_nFlag & 0x40) s_pszWorldIcon = "world_dummy.lad";
    C2DSeqAgent::SeqTbl tbl = {{0, 1, 2}};
    FVECTOR v;
    C2DSeqAgent* pAgent;
    pOutCtrl = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(2006)", RET_ADDR())) D2::C2DSeqCtrl;
    pOutCtrl->addSeq(s_pszWorldIcon, 0, tbl._tbl[0]);
    pOutCtrl->changeLayer(12);
    v.init();
    pOutCtrl->setPos(v);
    pOutCtrl->enableColorCtrl(TRUE);
    pAgent = new (com_malloc_d(sizeof(C2DSeqAgent), __FILE__ "(2014)", RET_ADDR())) C2DSeqAgent(this);
    pAgent->setupSeq(*pOutCtrl, s_pszWorldIcon, 0, 0);
    pAgent->setupState(tbl);
    return pAgent;
}
/* CARD's inline constructor (Game/Card.cpp's _setCard) */
static inline void _setCard(CARD& c, USHORT nKind, CHAR nNumber) {
    c.m_nId = 0;
    c.m_nNumber2 = nNumber;
    c.m_nKind2 = nKind;
    c.m_nCategory20 = cardDataTbl[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl[c.m_nKind].nCategory1;
}
C2DSeqAgent* CSaveLoadUI::createRoomName(D2::C2DSeqCtrl*& pOutCtrl) {
    C2DSeqAgent::SeqTbl tbl = {{42, 43, 44}};
    FVECTOR v;
    BYTE nCardID;
    pOutCtrl = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(2028)", RET_ADDR())) D2::C2DSeqCtrl;
    nCardID = 0;
    pOutCtrl->addSeq(m_pszLad, m_nMainSeqUnit, tbl._tbl[0]);
    pOutCtrl->changeLayer(12);
    v.init();
    pOutCtrl->setPos(v);
    if (!(m_nFlag & 0x40)) {
        RoomInfo* pInfo = CSceneInfo::getRoomInfo(-1);
        if (pInfo && ((UINT)pInfo->_pSrcInfo | (UINT)pInfo->_pStorageInfo | (UINT)pInfo->_pStorageInfo2)) {
            nCardID = pInfo->getRoomNameCardID();
        }
    }
    CARD card;
    _setCard(card, nCardID, 0);
    G_RGBA rgba;
    rgba.col = 0x80707070;
    D2::C2DFnt* pFnt =
        new (com_malloc_d(sizeof(D2::C2DFnt), __FILE__ "(2047)", RET_ADDR())) D2::C2DFnt(0, 0, &rgba, 16, card.getName(), 0x241);
    pOutCtrl->addFnt(m_pszLad, m_nMainSeqUnit, pFnt);
    pOutCtrl->enableColorCtrl(TRUE);
    C2DSeqAgent* pAgent = new (com_malloc_d(sizeof(C2DSeqAgent), __FILE__ "(2051)", RET_ADDR())) C2DSeqAgent(this);
    pAgent->setupSeq(*pOutCtrl, m_pszLad, m_nMainSeqUnit, 0);
    pAgent->setupState(tbl);
    return pAgent;
}
C2DSeqCursorCtrl* CSaveLoadUI::createCursor() {
    short tbl[2][2] = {{8, 7}, {8, 7}};
    SHORT* pTbl = tbl[getRsrcType()];
    C2DSeqCursorCtrl* pCursor =
        new (com_malloc_d(sizeof(C2DSeqCursorCtrl), __FILE__ "(2068)", RET_ADDR())) C2DSeqCursorCtrl;
    pCursor->setupSeq(m_pszLad, m_nMainSeqUnit, pTbl[0], pTbl[1]);
    return pCursor;
}
C2DSeqLightMarkAgent* CSaveLoadUI::createLightMark(LIGHTMARK_TYPE nType, INT nLayer, BOOL bFollow) {
    C2DSeqAgent::SeqTbl tbl[2][2] = {{{{33, 34, 35}}, {{33, 34, 35}}}, {{{9, 10, 11}}, {{9, 10, 11}}}};
    C2DSeqAgent::SeqTbl& t = tbl[nType][getRsrcType()];
    C2DSeqLightMarkAgent* pLight =
        new (com_malloc_d(sizeof(C2DSeqLightMarkAgent), __FILE__ "(2086)", RET_ADDR())) C2DSeqLightMarkAgent(this);
    pLight->setupSeq(t, m_pszLad, m_nMainSeqUnit, nLayer, bFollow);
    pLight->setupState(t);
    return pLight;
}
C2DSeqAgent* CSaveLoadUI::createTimeLabel(D2::C2DSeqCtrl*& pOutCtrl) {
    C2DSeqAgent::SeqTbl tbl = {{45, 46, 47}};
    FVECTOR v;
    C2DSeqAgent* pAgent;
    pOutCtrl = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(2101)", RET_ADDR())) D2::C2DSeqCtrl;
    pOutCtrl->addSeq(m_pszLad, m_nMainSeqUnit, tbl._tbl[0]);
    pOutCtrl->changeLayer(12);
    v.init();
    if (CGame::isRikuGame()) v.y = -35.0f;
    pOutCtrl->setPos(v);
    pOutCtrl->enableColorCtrl(TRUE);
    pAgent = new (com_malloc_d(sizeof(C2DSeqAgent), __FILE__ "(2110)", RET_ADDR())) C2DSeqAgent(this);
    pAgent->setupSeq(*pOutCtrl, m_pszLad, m_nMainSeqUnit, 0);
    pAgent->setupState(tbl);
    return pAgent;
}
C2DSeqTimeCtrl* CSaveLoadUI::createTimeCtrl() {
    C2DSeqTimeCtrl* pCtrl = new (com_malloc_d(sizeof(C2DSeqTimeCtrl), __FILE__ "(2122)", RET_ADDR())) C2DSeqTimeCtrl;
    pCtrl->setupSeq(m_pszLad, m_nMainSeqUnit, 22, 32, 65);
    pCtrl->setLayer(12);
    FVECTOR v;
    v.set(28.0f, 357.0f, 0.0f);
    if (CGame::isRikuGame()) v.y += -35.0f;
    pCtrl->setPos(v);
    return pCtrl;
}
C2DSeqAgent* CSaveLoadUI::createMPLabel(D2::C2DSeqCtrl*& pOutCtrl) {
    C2DSeqAgent::SeqTbl tbl = {{39, 40, 41}};
    FVECTOR v;
    C2DSeqAgent* pAgent;
    pOutCtrl = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(2142)", RET_ADDR())) D2::C2DSeqCtrl;
    pOutCtrl->addSeq(m_pszLad, m_nMainSeqUnit, tbl._tbl[0]);
    pOutCtrl->changeLayer(12);
    v.init();
    pOutCtrl->setPos(v);
    pOutCtrl->enableColorCtrl(TRUE);
    pAgent = new (com_malloc_d(sizeof(C2DSeqAgent), __FILE__ "(2150)", RET_ADDR())) C2DSeqAgent(this);
    pAgent->setupSeq(*pOutCtrl, m_pszLad, m_nMainSeqUnit, 0);
    pAgent->setupState(tbl);
    return pAgent;
}
C2DSeqNumericCtrl* CSaveLoadUI::createMPCtrl() {
    C2DSeqNumericCtrl* pCtrl =
        new (com_malloc_d(sizeof(C2DSeqNumericCtrl), __FILE__ "(2162)", RET_ADDR())) C2DSeqNumericCtrl;
    pCtrl->setupSeq(m_pszLad, m_nMainSeqUnit, 12, 65, 99999);
    pCtrl->setLayer(12);
    FVECTOR v;
    v.set(36.0f, 322.0f, 0.0f);
    pCtrl->setPos(v);
    pCtrl->updateValue(CGameWork::progMCW()._statusInfo[8]);
    return pCtrl;
}
C2DSeqAgent* CSaveLoadUI::createMainHelp(D2::C2DSeqCtrl*& pOutCtrl, D2::C2DFnt*& pOutFnt) {
    C2DSeqAgent::SeqTbl tbl[2] = {{{89, 90, 91}}, {{90, 91, 92}}};
    _LayoutInfo ltbl[2] = {{3, 9}, {4, 9}};
    pOutCtrl = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(2189)", RET_ADDR())) D2::C2DSeqCtrl;
    C2DSeqAgent::SeqTbl& t = tbl[getRsrcType()];
    pOutCtrl->addSeq(m_pszLad, m_nMainSeqUnit, t._tbl[0]);
    pOutCtrl->changeLayer(12);
    G_RGBA rgba;
    rgba.col = 0x80707070;
    _LayoutInfo& info = ltbl[getRsrcType()];
    m_szMainFrameHelpStr[0] = '\0';
    pOutFnt = new (com_malloc_d(sizeof(D2::C2DFnt), __FILE__ "(2198)", RET_ADDR()))
        D2::C2DFnt(0, 0, &rgba, 16, m_szMainFrameHelpStr, 64);
    pOutCtrl->addFnt(m_pszLad, m_nMainSeqUnit, pOutFnt);
    pOutCtrl->enableColorCtrl(TRUE);
    FVECTOR vPos;
    D2::C2DMan::getLayPosition(m_pszLad, info._nLayIdx, info._nSeqIdx, vPos, FALSE);
    pOutCtrl->setPos(vPos);
    C2DSeqAgent* pAgent = new (com_malloc_d(sizeof(C2DSeqAgent), __FILE__ "(2206)", RET_ADDR())) C2DSeqAgent(this);
    pAgent->setupSeq(*pOutCtrl, m_pszLad, m_nMainSeqUnit, 0);
    pAgent->setupState(t);
    return pAgent;
}
static CHAR* s_pStrSaveLoad[2] = {"Save", "Load"};
static CHAR* s_pStrSlot[2] = {"Slot 1", "Slot 2"};
C2DSeqAgent* CSaveLoadUI::createResultPlate(D2::C2DSeqCtrl*& pOutCtrl, D2::C2DFnt*& pOutFnt, INT nIdx) {
    C2DSeqAgent::SeqTbl tbl[2][2] = {{{{36, 37, 38}}, {{36, 37, 38}}}, {{{4, 5, 6}}, {{4, 5, 6}}}};
    _LayoutInfo ltbl[2][2] = {{{3, 4}, {4, 4}}, {{3, 5}, {4, 5}}};
    float ftbl[2][2] = {{4.0f, 16.0f}, {-44.0f, 7.0f}};
    C2DSeqAgent::SeqTbl& t = tbl[nIdx][getRsrcType()];
    pOutCtrl = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(2234)", RET_ADDR())) D2::C2DSeqCtrl;
    pOutCtrl->addSeq(m_pszLad, m_nMainSeqUnit, t._tbl[0]);
    pOutCtrl->changeLayer(12);
    _LayoutInfo& info = ltbl[nIdx][getRsrcType()];
    G_RGBA rgba;
    rgba.col = 0x80707070;
    switch (nIdx) {
    case 0:
        pOutFnt = new (com_malloc_d(sizeof(D2::C2DFnt), __FILE__ "(2244)", RET_ADDR()))
            D2::C2DFnt(0, 0, &rgba, 15, s_pStrSaveLoad[getRsrcType()], 0x40);
        break;
    case 1:
        pOutFnt = new (com_malloc_d(sizeof(D2::C2DFnt), __FILE__ "(2247)", RET_ADDR()))
            D2::C2DFnt(0, 0, &rgba, 15, s_pStrSlot[m_nSlot], 0x40);
        break;
    }
    FLOAT* ofsTbl = ftbl[nIdx];
    pOutCtrl->addFntShift(m_pszLad, m_nMainSeqUnit, pOutFnt, ofsTbl[0], ofsTbl[1]);
    pOutCtrl->enableColorCtrl(TRUE);
    FVECTOR vPos;
    D2::C2DMan::getLayPosition(m_pszLad, info._nLayIdx, info._nSeqIdx, vPos, FALSE);
    pOutCtrl->setPos(vPos);
    C2DSeqAgent* pAgent = new (com_malloc_d(sizeof(C2DSeqAgent), __FILE__ "(2258)", RET_ADDR())) C2DSeqAgent(this);
    pAgent->setupSeq(*pOutCtrl, m_pszLad, m_nMainSeqUnit, 0);
    pAgent->setupState(t, callbackResultPlate, NULL);
    return pAgent;
}
C2DSeqSelectable* CSaveLoadUI::createSelSlotPlate(INT nIdx) {
    C2DSeqAgent::SeqTbl tbl[2] = {{{1, 2, 3}}, {{1, 2, 3}}};
    short actTbl[2] = {0, 0};
    _LayoutInfo ltbl[2] = {{3, 6}, {4, 6}};
    C2DSeqAgent::SeqTbl& t = tbl[getRsrcType()];
    C2DSeqSelectable* pPlate =
        new (com_malloc_d(sizeof(C2DSeqSelectable), __FILE__ "(2279)", RET_ADDR())) C2DSeqSelectable(this);
    pPlate->setupSeq(t, m_pszLad, m_nMainSeqUnit, actTbl[getRsrcType()], 12);
    G_RGBA rgba;
    rgba.col = 0x80707070;
    pPlate->setCaptionParam(s_pStrSlot[nIdx], 15, &rgba, 0x40);
    pPlate->setCaptionShift(-44.0f, 7.0f);
    FVECTOR vPos;
    _LayoutInfo& info = ltbl[getRsrcType()];
    D2::C2DMan::getLayPosition(m_pszLad, info._nLayIdx, info._nSeqIdx + nIdx, vPos, FALSE);
    pPlate->setCtrlPos(vPos);
    pPlate->setupState(t, nIdx == 0 ? callbackSlotPlateIn : NULL, NULL);
    pPlate->setCursorOfs(-32.0f, -8.0f);
    pPlate->setLightMarkOfs(64.0f, -4.0f);
    return pPlate;
}
C2DSeqAgent* CSaveLoadUI::createMemocaInfo(D2::C2DSeqCtrl*& pOutCtrl, D2::C2DSeqCtrl** ppOutInfoBar) {
    if (m_mode == SLMODE_LOAD) {
        pOutCtrl = NULL;
        return NULL;
    }
    C2DSeqAgent::SeqTbl t = {{107, 108, 109}};
    _LayoutInfo info = {3, 8};
    FVECTOR vPos;
    pOutCtrl = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(2311)", RET_ADDR())) D2::C2DSeqCtrl;
    pOutCtrl->addSeq(m_pszLad, m_nMainSeqUnit, t._tbl[0]);
    pOutCtrl->changeLayer(12);
    D2::C2DMan::getLayPosition(m_pszLad, info._nLayIdx, info._nSeqIdx, vPos, FALSE);
    pOutCtrl->setPos(vPos);
    C2DSeqAgent* pAgent = new (com_malloc_d(sizeof(C2DSeqAgent), __FILE__ "(2319)", RET_ADDR())) C2DSeqAgent(this);
    pAgent->setupSeq(*pOutCtrl, m_pszLad, m_nMainSeqUnit, 0);
    pAgent->setupState(t, callbackMemocaInfo, callbackMemocaInfo);
    unsigned short barTbl[3] = {55, 57, 56};
    _LayoutInfo barInfo = {6, 1};
    unsigned int fntCol[3] = {0x80208000, 0x80204080, 0x80804020};
    for (INT i = 0; i < 3; i++) {
        D2::C2DSeqCtrl* pCtrl = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(2331)", RET_ADDR())) D2::C2DSeqCtrl;
        *ppOutInfoBar++ = pCtrl;
        pCtrl->addSeq(m_pszLad, m_nMainSeqUnit, barTbl[i]);
        pCtrl->setParent(pOutCtrl);
        pCtrl->changeLayer(12);
        pCtrl->enableColorCtrl(TRUE);
        D2::C2DMan::getLayPosition(m_pszLad, barInfo._nLayIdx, barInfo._nSeqIdx, vPos, FALSE);
        pCtrl->setLocalPos(vPos);
        G_RGBA col;
        col.col = fntCol[i];
        m_szMemocaInfoStr[i][0] = '\0';
        D2::C2DFnt* pFont = new (com_malloc_d(sizeof(D2::C2DFnt), __FILE__ "(2344)", RET_ADDR()))
            D2::C2DFnt(0, 0, &col, 12, m_szMemocaInfoStr[i], 0x40);
        pOutCtrl->addFntShift(m_pszLad, m_nMainSeqUnit, pFont, -36.0f, i * 58.0f - 52.0f);
    }
    return pAgent;
}
C2DSeqAgent* CSaveLoadUI::createDataListContainer(D2::C2DSeqCtrl*& pOutCtrl, C2DSeqScrollbarCtrl*& pOutScrollbar) {
    C2DSeqAgent::SeqTbl tbl[2] = {{{96, 97, 98}}, {{93, 94, 95}}};
    _LayoutInfo ltbl[2] = {{3, 3}, {4, 3}};
    C2DSeqAgent::SeqTbl& t = tbl[getRsrcType()];
    pOutCtrl = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(2364)", RET_ADDR())) D2::C2DSeqCtrl;
    pOutCtrl->addSeq(m_pszLad, m_nMainSeqUnit, t._tbl[0]);
    pOutCtrl->changeLayer(12);
    FVECTOR vPos;
    _LayoutInfo& info = ltbl[getRsrcType()];
    D2::C2DMan::getLayPosition(m_pszLad, info._nLayIdx, info._nSeqIdx, vPos, FALSE);
    pOutCtrl->setPos(vPos);
    C2DSeqAgent* pAgent = new (com_malloc_d(sizeof(C2DSeqAgent), __FILE__ "(2373)", RET_ADDR())) C2DSeqAgent(this);
    pAgent->setupSeq(*pOutCtrl, m_pszLad, m_nMainSeqUnit, 0);
    pAgent->setupState(t, callbackDataList, callbackDataList);
    C2DSeqAgent::SeqTbl scr_tbl[2][3] = {{{{83, 84, 85}}, {{80, 81, 82}}, {{86, 87, 88}}},
                                          {{{84, 85, 86}}, {{81, 82, 83}}, {{87, 88, 89}}}};
    pOutScrollbar = new (com_malloc_d(sizeof(C2DSeqScrollbarCtrl), __FILE__ "(2386)", RET_ADDR())) C2DSeqScrollbarCtrl(this);
    pOutScrollbar->setupSeq(pOutCtrl, scr_tbl[getRsrcType()], m_pszLad, m_nMainSeqUnit, 12, 100);
    pOutScrollbar->setScrollRange(166.0f, 99, 4, FALSE);
    vPos.x = 176.0f;
    vPos.y = -82.0f;
    pOutScrollbar->setBasePos(vPos);
    return pAgent;
}
C2DSeqSelectable* CSaveLoadUI::createDataItemPlate(D2::C2DSeqCtrl& parent, D2::C2DSeqCtrl*& pOutNumCircle,
                                                   D2::C2DFnt*& pOutNumFnt, D2::C2DSeqCtrl*& pOutIcon,
                                                   D2::C2DSeqCtrl*& pOutClearIcon, INT nIdx) {
    _LayoutInfo ltbl[2] = {{7, 1}, {8, 1}};
    const C2DSeqAgent::SeqTbl& t = s_seqDataPlateTbl[0][getRsrcType()];
    C2DSeqSelectable* pPlate =
        new (com_malloc_d(sizeof(C2DSeqSelectable), __FILE__ "(2428)", RET_ADDR())) C2DSeqSelectable(this);
    pPlate->setupSeq(t, m_pszLad, m_nMainSeqUnit, s_seqDataActPlateTbl[0][getRsrcType()], 12);
    pPlate->setCtrlParent(&parent);
    G_RGBA rgba;
    rgba.col = 0x802864FA;
    pPlate->setCaptionParam(s_pStrSlot[nIdx], 11, &rgba, 0x40);
    pPlate->setCaptionText(m_szDataItemContent[nIdx]);
    pPlate->setCaptionSpace(0, 48);
    FVECTOR vPos;
    _LayoutInfo& info = ltbl[getRsrcType()];
    D2::C2DMan::getLayPosition(m_pszLad, info._nLayIdx, info._nSeqIdx + nIdx, vPos, FALSE);
    pPlate->setCtrlLocalPos(vPos);
    pPlate->setupState(t);
    pPlate->setCursorOfs(-104.0f, -16.0f);
    pPlate->setLightMarkOfs(210.0f, -20.0f);

    short cirTbl[2] = {75, 79};
    _LayoutInfo cirltbl[2] = {{1, 1}, {1, 1}};
    pOutNumCircle = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(2451)", RET_ADDR())) D2::C2DSeqCtrl;
    pOutNumCircle->setParent(pPlate->getSeqCtrl());
    pOutNumCircle->addSeq(m_pszLad, m_nMainSeqUnit, cirTbl[getRsrcType()]);
    _LayoutInfo& info2 = cirltbl[getRsrcType()];
    D2::C2DMan::getLayPosition(m_pszLad, info2._nLayIdx, info2._nSeqIdx, vPos, FALSE);
    pOutNumCircle->setLocalPos(vPos);
    pOutNumCircle->changeLayer(12);
    pOutNumCircle->enableColorCtrl(TRUE);
    rgba.col = 0x80707070;
    pOutNumFnt = new (com_malloc_d(sizeof(D2::C2DFnt), __FILE__ "(2462)", RET_ADDR()))
        D2::C2DFnt(0, 0, &rgba, 12, m_szDataItemID[nIdx], 0x40);
    pOutNumCircle->addFntShift(m_pszLad, m_nMainSeqUnit, pOutNumFnt, -6.5f, 5.5f);

    _LayoutInfo icontbl[2] = {{1, 2}, {1, 2}};
    pOutIcon = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(2469)", RET_ADDR())) D2::C2DSeqCtrl;
    pOutIcon->setParent(pPlate->getSeqCtrl());
    pOutIcon->addSeq(m_pszLad, m_nIconSeqUnit, 0);
    _LayoutInfo& info3 = icontbl[getRsrcType()];
    D2::C2DMan::getLayPosition(m_pszLad, info3._nLayIdx, info3._nSeqIdx, vPos, FALSE);
    pOutIcon->setLocalPos(vPos);
    pOutIcon->changeLayer(12);
    pOutIcon->enableColorCtrl(TRUE);
    rgba.col = 0x802864FA;
    D2::C2DFnt* pTimeFnt = new (com_malloc_d(sizeof(D2::C2DFnt), __FILE__ "(2481)", RET_ADDR()))
        D2::C2DFnt(0, 0, &rgba, 12, m_szDataItemTime[nIdx], 0x40);
    pPlate->getSeqCtrl()->addFntShift(m_pszLad, m_nMainSeqUnit, pTimeFnt, 112.0f, 20.0f);

    short clrIcon[2] = {118, 115};
    _LayoutInfo clr_icontbl[2] = {{1, 3}, {1, 3}};
    pOutClearIcon = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(2492)", RET_ADDR())) D2::C2DSeqCtrl;
    pOutClearIcon->setParent(pPlate->getSeqCtrl());
    pOutClearIcon->addSeq(m_pszLad, m_nMainSeqUnit, clrIcon[getRsrcType()]);
    _LayoutInfo& info4 = clr_icontbl[getRsrcType()];
    D2::C2DMan::getLayPosition(m_pszLad, info4._nLayIdx, info4._nSeqIdx, vPos, FALSE);
    pOutClearIcon->setLocalPos(vPos);
    pOutClearIcon->changeLayer(12);
    pOutClearIcon->enableColorCtrl(TRUE);
    return pPlate;
}
C2DSeqAgent* CSaveLoadUI::createMessageDlg(D2::C2DSeqCtrl*& pOutCtrl, D2::C2DFnt*& pOutFntInf) {
    C2DSeqAgent::SeqTbl tbl[4] = {{{52, 53, 54}}, {{52, 53, 54}}, {{55, 56, 57}}, {{55, 56, 57}}};
    _LayoutInfo ltbl[4] = {{0, 0}, {0, 0}, {2, 0}, {2, 0}};
    C2DSeqAgent::SeqTbl& t = tbl[m_mode];
    pOutCtrl = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(2522)", RET_ADDR())) D2::C2DSeqCtrl;
    pOutCtrl->addSeq(m_pszLad, m_nMainSeqUnit, t._tbl[0]);
    pOutCtrl->changeLayer(14);
    G_RGBA rgba;
    rgba.col = 0x80707070;
    D2::C2DFnt* pBody = new (com_malloc_d(sizeof(D2::C2DFnt), __FILE__ "(2528)", RET_ADDR()))
        D2::C2DFnt(0, 0, &rgba, 14, m_szMessageDlgStr, 0x46);
    pBody->setSpace(0, 80);
    pOutCtrl->addFnt(m_pszLad, m_nMainSeqUnit, pBody);
    rgba.col = 0x80000070;
    pOutFntInf = new (com_malloc_d(sizeof(D2::C2DFnt), __FILE__ "(2533)", RET_ADDR()))
        D2::C2DFnt(0, 0, &rgba, 15, NULL, 0x40);
    pOutCtrl->addFntShift(m_pszLad, m_nMainSeqUnit, pOutFntInf, -120.0f, -56.0f);
    pOutCtrl->enableColorCtrl(TRUE);
    FVECTOR vPos;
    _LayoutInfo& info = ltbl[m_mode];
    D2::C2DMan::getLayPosition(m_pszLad, info._nLayIdx, info._nSeqIdx, vPos, FALSE);
    pOutCtrl->setPos(vPos);
    C2DSeqAgent* pAgent = new (com_malloc_d(sizeof(C2DSeqAgent), __FILE__ "(2542)", RET_ADDR())) C2DSeqAgent(this);
    pAgent->setupSeq(*pOutCtrl, m_pszLad, m_nMainSeqUnit, 0);
    pAgent->setupState(t, callbackMessageDlg, callbackMessageDlg);
    return pAgent;
}
C2DSeqSelectable* CSaveLoadUI::createMessageDlgBtn(D2::C2DSeqCtrl* pParent, INT nIdx) {
    C2DSeqAgent::SeqTbl tbl[2] = {{{49, 50, 51}}, {{49, 50, 51}}};
    short actTbl[2] = {48, 48};
    _LayoutInfo ltbl[4][2] = {{{0, 1}, {0, 2}}, {{0, 1}, {0, 2}}, {{2, 2}, {2, 1}}, {{2, 2}, {2, 1}}};
    C2DSeqAgent::SeqTbl& t = tbl[getRsrcType()];
    C2DSeqSelectable* pBtn =
        new (com_malloc_d(sizeof(C2DSeqSelectable), __FILE__ "(2567)", RET_ADDR())) C2DSeqSelectable(this);
    pBtn->setupSeq(t, m_pszLad, m_nMainSeqUnit, actTbl[getRsrcType()], 14);
    pBtn->getSeqCtrl()->setParent(pParent);
    G_RGBA rgba;
    rgba.col = 0x80707070;
    pBtn->setCaptionParam(NULL, 15, &rgba, 0x42);
    pBtn->setCaptionShift(48.0f, 16.0f);
    FVECTOR vPos;
    _LayoutInfo& info = ltbl[m_mode][nIdx];
    D2::C2DMan::getLayPosition(m_pszLad, info._nLayIdx, info._nSeqIdx, vPos, FALSE);
    pBtn->setCtrlPos(vPos);
    pBtn->setupState(t);
    pBtn->setLightMarkOfs(100.0f, 2.0f);
    return pBtn;
}
void CSaveLoadUI::setActiveSlotPlate(INT nSlot) {
    unsigned short seqTbl[2][2] = {{0, 2}, {0, 2}};
    char szMes[64];
    CHAR* pszFrm;
    for (INT i = 0; i < 2; i++) {
        m_pSelSlotPlate[i]->select(i == nSlot);
    }
    if (m_mode == SLMODE_SAVE) {
        pszFrm = "Save data to %s";
    } else {
        pszFrm = "Load data from %s";
    }
    sprintf(szMes, pszFrm, s_pStrSlot[nSlot]);
    setHelpMessage(szMes);
    m_pResultPlateFnt[1]->setText(s_pStrSlot[nSlot]);
}

void CSaveLoadUI::setupDataPlate(SHORT nDataIdx) {
    if (nDataIdx < m_nDataListTop || nDataIdx >= m_nDataListTop + 4) {
        TRACE("CSaveLoadUI::setupDataPlate(%d) ... \x94\xcd\x88\xcd\x8aO\x82\xc5\x82\xb7\n", nDataIdx);
        return;
    }
    SHORT n = nDataIdx - m_nDataListTop;
    short cirTbl[2][2] = {{0x4B, 0x4C}, {0x4F, 0x50}};
    C2DSeqSelectable* pPlate = m_pDataItemPlate[n];
    D2::C2DSeqCtrl* pCircle = m_pDataItemNumCircle[n];
    D2::C2DSeqCtrl* pWorldIcon = m_pDataItemIcon[n];
    D2::C2DSeqCtrl* pClearIcon = m_pDataItemClearIcon[n];
    INT nCircle = 0;
    INT nGameMode;
    sprintf(m_szDataItemID[n], "%02d", nDataIdx + 1);
    BOOL bUpdateIcon = FALSE;
    BOOL bClearIcon = FALSE;
    GameDataHeader* pHead = m_pGameDataEntries->getEntry(nDataIdx);
    char szLine[3][64];
    *m_szDataItemTime[n] = 0;
    if (pHead && pHead->hasData()) {
        pPlate->setCaptionShift(-50.0f, -8.0f);
        pPlate->setCaptionSize(12);
        if (pHead->_cFlag & 2) {
            sprintf(szLine[0], "%s  LV\xf9" "E%2d", _ctdText(0x100002), pHead->_cLevel);
        } else {
            sprintf(szLine[0], "%s  LV\xf9" "E%2d      %5d MP", _ctdText(0x100001), pHead->_cLevel, pHead->_nMoogle);
        }
        szLine[1][0] = 0;
        if (pHead->_cGameLevel) {
            INT id = pHead->_cGameLevel < 4 ? pHead->_cGameLevel : 0;
            UINT ctdID = 0x100200 + id;
            strcpy(szLine[1], _ctdText(ctdID));
        }
        INT nFloor = pHead->_cFloor;
        CHAR* pszFloor = szLine[2];
        if (nFloor < 0) {
            *pszFloor++ = 'B';
            nFloor = -nFloor;
        } else {
            *pszFloor++ = ' ';
        }
        const CHAR* pszWorld;
        BYTE w = pHead->_cWorld & 0xF;
        if ((pHead->_cWorld & 0x10) || !w) {
            pszWorld = "Entrance Hall";
        } else {
            pszWorld = cardDataTbl[w + 0xBD].pName;
        }
        bUpdateIcon = w < 14;
        char szFloor[8];
        sprintf(szFloor, "%dF", nFloor);
        if (nFloor < 10) strcat(szFloor, " ");
        sprintf(pszFloor, "%s %s", szFloor, pszWorld);
        if (pPlate->getState() == C2DSeqAgent::STATE_LOOP) {
            INT nH = pHead->_nTime / 3600;
            INT nM = pHead->_nTime % 3600 / 60;
            sprintf(m_szDataItemTime[n], "%3d:%02d", nH, nM);
        }
        nCircle = (pHead->_cFlag & 2) ? 2 : 1;
        nGameMode = 0;
        bClearIcon = pHead->isGameCleared();
    } else {
        szLine[0][0] = 0;
        pPlate->setCaptionShift(-110.0f, -8.0f);
        pPlate->setCaptionSize(12);
        UINT nCtd;
        INT nArgc = 0;
        if (m_mode == SLMODE_SAVE) {
            if (pHead) {
                if (pHead->isDirBroken()) {
                    nCtd = 0x600404;
                } else {
                    nCtd = pHead->isDataBroken() ? 0x600403 : 0x600401;
                }
            } else {
                nArgc = 1;
                nCtd = 0x600400;
            }
        } else {
            nCtd = 0x600402;
            if (pHead && (pHead->isDataBroken() || pHead->isDirBroken())) nCtd = 0x600403;
        }
        if (nArgc) {
            const CHAR* pszArg = _ctdText(0x600000);
            sprintf(szLine[1], _ctdText(nCtd), pszArg);
        } else {
            strcpy(szLine[1], _ctdText(nCtd));
        }
        nGameMode = 1;
        szLine[2][0] = 0;
        FLOAT fY = strchr(szLine[1], '\n') ? -14.0f : -8.0f;
        pPlate->setCaptionShift(-110.0f, fY);
    }
    sprintf(m_szDataItemContent[n], "%s\n%s\n%s", szLine[0], szLine[1], szLine[2]);
    BOOL bSelected = pPlate->getCurerntSeq() == pPlate->m_nActiveLoopSeq;
    pPlate->setupState(s_seqDataPlateTbl[nCircle][getRsrcType()]);
    pPlate->setupActiveSeq(s_seqDataActPlateTbl[nCircle][getRsrcType()]);
    BOOL bNew = pPlate->getState() == C2DSeqAgent::_STATE_NOT_START;
    if (bNew) {
        pPlate->changeSeq(s_seqDataPlateTbl[nCircle][getRsrcType()]._tbl[0]);
        if (pPlate->m_pCtrl) pPlate->m_pCtrl->enableDisp(FALSE);
    } else if (pPlate->getState() == C2DSeqAgent::STATE_LOOP) {
        pPlate->changeSeq(bSelected ? s_seqDataActPlateTbl[nCircle][getRsrcType()]
                                    : s_seqDataPlateTbl[nCircle][getRsrcType()]._tbl[1]);
    }
    if (bUpdateIcon) {
        INT id = pHead->_cWorld & 0xF;
        if (id) {
            id--;
        } else {
            id = 13;
        }
        pWorldIcon->replaceSeq(0, m_pszLad, m_nIconSeqUnit, id);
        BOOL bShow = pPlate->getState() != C2DSeqAgent::_STATE_NOT_START;
        if (bShow) pWorldIcon->enableDisp(TRUE);
    } else {
        pWorldIcon->enableDisp(FALSE);
    }
    pCircle->replaceSeq(0, m_pszLad, m_nMainSeqUnit, cirTbl[getRsrcType()][nGameMode]);
    pClearIcon->enableDisp(bClearIcon);
}
void CSaveLoadUI::updateMemocaInfoBar() {
    INT nY = 0;
    INT nH;
    FVECTOR v;
    FVECTOR vScale;
    vScale.set(1.0f, 1.0f, 1.0f, 0.0f);
    UINT nAll = 0;
    unsigned int nTbl[3];
    nTbl[0] = m_pGameDataEntries->getDataUnitCountByType(0);
    nTbl[1] = m_pGameDataEntries->getDataUnitCountByType(1);
    nTbl[2] = m_pGameDataEntries->getDataUnitCountByType(2);
    if (nTbl[0] + nTbl[1] >= 100) nTbl[0] = 99 - nTbl[1];
    INT i;
    for (i = 0; i < 3; i++) {
        nAll += nTbl[i];
    }
    if (!nAll) AssertApp(__FILE__, 2807, "nAll", 0);
    for (i = 0; i < 3; i++) {
        UINT nCount = nTbl[i];
        if (!nCount) m_pMemocaInfoBar[i]->enableDisp(FALSE);
        vScale.y = (FLOAT)nCount / (FLOAT)nAll;
        if (i == 2) {
            nH = 128 - nY;
        } else {
            INT n = (vScale.y * 128.0f + 0.5f) * 10.0f;
            nH = n / 10;
        }
        v = m_pMemocaInfoBar[i]->m_seqControl.vPos;
        v.y = nY + (nH - 128.0f) * 0.5f;
        m_pMemocaInfoBar[i]->setScl(vScale);
        m_pMemocaInfoBar[i]->setLocalPos(v);
        nY += nH;
    }
}
void CSaveLoadUI::updateMemocaInfoStr() {
    CHAR* pszStrTbl[3] = {"Available\n\x81@\x81@\x81@\x81@%3d", "Saved\n\x81@\x81@\x81@\x81@%3d", "Other data"};
    USHORT nSavable = m_pGameDataEntries->getDataUnitCountByType(0);
    USHORT nSaved = m_pGameDataEntries->getDataUnitCountByType(1);
    if (nSavable + nSaved >= 100) nSavable = 99 - nSaved;
    for (INT i = 0; i < 3; i++) {
        m_pMemocaInfoBar[i]->enableDisp(TRUE);
        switch (i) {
        case 0:
        case 1:
            sprintf(m_szMemocaInfoStr[i], pszStrTbl[i], !i ? nSavable : nSaved);
            break;
        case 2:
            strcpy(m_szMemocaInfoStr[2], pszStrTbl[2]);
            break;
        }
    }
}
void CSaveLoadUI::setHelpMessage(const CHAR* pszMes) {
    strcpy(m_szMainFrameHelpStr, pszMes);
    if (_isAgentState(m_pMainFrameHelpAgent, C2DSeqAgent::_STATE_NOT_START)) m_pMainFrameHelpAgent->start();
    else if (m_pMainFrameHelpAgent->getState() != C2DSeqAgent::STATE_LOOP) m_pMainFrameHelpAgent->changeState(C2DSeqAgent::STATE_IN, TRUE, -1);
}
void CSaveLoadUI::outHelpMessage() {
    m_pMainFrameHelpAgent->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
}
void CSaveLoadUI::openMessageDlg(const CHAR* pszTxt, INT nIcon, INT nType) {
    int tbl[2][2] = {{0x42, 0x43}, {0x46, 0x47}};
    CHAR* pszInfTbl[2] = {"INFORMATION", "QUESTION"};
    float infOfsTbl[2] = {-150.0f, -56.0f};
    float bdyYofsTbl[4] = {-15.0f, -30.0f, -30.0f, -30.0f};
    C2DSeqAgent* pAgent = m_pMessageDlgAgent;
    D2::C2DSeqCtrl* pCtrl = m_pMessageDlgCtrl;
    m_nDlgType = nType;
    m_nDlgResult = 0;
    m_nDlgSel = 1;
    CHAR* pszInfo = pszInfTbl[nIcon];
    INT nIconSeq = tbl[getRsrcType()][nIcon];
    BOOL bOpened = pAgent->getState() < C2DSeqAgent::STATE_OUT;
    BOOL bLooped = pAgent->getState() == C2DSeqAgent::STATE_LOOP;
    if (!bOpened) {
        if (m_pMainLightMark->getState() < C2DSeqAgent::STATE_OUT) {
            m_nFlag |= 0x20;
        } else {
            m_nFlag &= ~0x20;
        }
    }
    m_pMessageDlgFntInf->setText(pszInfo);
    strcpy(m_szMessageDlgStr, pszTxt);
    pCtrl->setFntPos(1, 0.0f, bdyYofsTbl[nType]);
    BOOL bNew = pAgent->getState() == C2DSeqAgent::_STATE_NOT_START;
    if (bNew) {
        pCtrl->addSeqShift(m_pszLad, m_nMainSeqUnit, nIconSeq, infOfsTbl[0], infOfsTbl[1]);
        if (!bOpened) pAgent->start();
    } else {
        pCtrl->replaceSeqShift(3, m_pszLad, m_nMainSeqUnit, nIconSeq, infOfsTbl[0], infOfsTbl[1]);
        if (!bOpened) pAgent->changeState(C2DSeqAgent::STATE_IN, TRUE, -1);
    }
    CHAR* pszCapTbl[2];
    int bOpenTbl[2];
    BOOL bCursorActive = TRUE;
    switch (nType) {
    case 1:
        bOpenTbl[0] = FALSE;
        bOpenTbl[1] = TRUE;
        pszCapTbl[1] = "OK";
        break;
    case 2:
    case 3:
        pszCapTbl[0] = "Yes";
        pszCapTbl[1] = "No";
        bOpenTbl[0] = TRUE;
        bOpenTbl[1] = TRUE;
        break;
    default:
        bOpenTbl[0] = FALSE;
        bOpenTbl[1] = FALSE;
        bCursorActive = FALSE;
        break;
    }
    for (INT i = 0; i < 2; i++) {
        C2DSeqSelectable* pBtn = m_pMessageDlgBtnAgent[i];
        if (bOpenTbl[i]) {
            CHAR* pszText = pszCapTbl[i];
            if (pBtn->m_pCaption) pBtn->m_pCaption->setText(pszText);
            if (bLooped) {
                BOOL bSel = i == m_nDlgSel;
                pBtn->select(bSel);
                if (bSel) {
                    FVECTOR vPos;
                    pBtn->getCursorPos(vPos);
                    m_pMessageDlgCursor->setPos(vPos);
                    m_pMessageDlgCursor->enableDisp(TRUE);
                    pBtn->getLightMarkPos(vPos);
                    m_pMainLightMark->setPos(vPos);
                    if (!(m_nFlag & 0x20)) m_pMainLightMark->startOrRestart();
                }
            } else {
                pBtn->startOrRestart();
            }
        } else {
            pBtn->dispOff();
        }
    }
    if (bOpened && !bCursorActive) m_pMessageDlgCursor->enableDisp(FALSE);
    if (!bCursorActive && m_pMainLightMark->getState() < C2DSeqAgent::STATE_OUT) {
        m_pMainLightMark->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
        m_pMainLightMark->dispOff();
    }
    m_nFlag |= 6;
    if (m_pMainCursor) m_pMainCursor->setHistorySeq();
}
void CSaveLoadUI::closeMessageDlg() {
    m_pMessageDlgAgent->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    for (INT i = 0; i < 2; i++) {
        m_pMessageDlgBtnAgent[i]->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    }
    m_nFlag &= ~4;
    m_pMessageDlgCursor->enableDisp(FALSE);
    if (m_nDlgType && !(m_nFlag & 0x20) && m_pMainLightMark->getState() < C2DSeqAgent::STATE_OUT) {
        m_pMainLightMark->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    }
}
/* Parked: the case-1 OK tail (m_nDlgResult = 1; CSound::playSE(2, -1, 64)) is cross-jumped into the identical tail of
 * case 2/3 in ours, the original keeps both copies; also the lockPadControl(TRUE) tail loads into v1 (original v0) and
 * one nop in the select loop; tried: switch vs if/else chain, combined early return, store after the call,
 * SLMSG_RESULT_OK / m_nDlgType as the value, permuter 4 min (6886 candidates, no gain) */
#if 0
BOOL CSaveLoadUI::exeMessageDlg() {
    if (!m_pMessageDlgAgent || m_pMessageDlgAgent->getState() != C2DSeqAgent::STATE_LOOP || m_nDlgResult) return FALSE;
    switch (m_nDlgType) {
    case 1:
        lockPadControl(FALSE);
        if (isTrgOK()) {
            m_nDlgResult = 1;
            CSound::playSE(2, -1, 64);
        } else if (isTrgCANCEL()) {
            m_nDlgResult = 3;
            CSound::playSE(4, -1, 64);
        }
        break;
    case 2:
    case 3:
        lockPadControl(FALSE);
        if (checkSelectorH(m_nDlgSel, 2, TRUE, TRUE)) {
            for (INT i = 0; i < 2; i++) {
                m_pMessageDlgBtnAgent[i]->select(i == m_nDlgSel);
            }
        } else if (isTrgOK()) {
            m_nDlgResult = m_nDlgSel + 1;
            CSound::playSE(2, -1, 64);
        } else if (m_nDlgType == 3 && isTrgCANCEL()) {
            m_nDlgResult = 3;
            CSound::playSE(4, -1, 64);
        }
        break;
    default:
        return FALSE;
    }
    lockPadControl(TRUE);
    return FALSE;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", exeMessageDlg__11CSaveLoadUI);
#endif
void CSaveLoadUI::outAllAgent() {
    m_pMainFrameAgent->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    if (m_pRoomNameAgent) m_pRoomNameAgent->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    if (m_pWorldIconAgent) m_pWorldIconAgent->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    m_pMainFrameHelpCtrl->enableDisp(FALSE);
    if (m_pMainCursor) m_pMainCursor->enableDisp(FALSE);
    if (m_pMainLightMark && m_pMainLightMark->isSeqCtrlVisible()) {
        m_pMainLightMark->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    }
    if (m_pRoomNameCtrl) m_pRoomNameCtrl->enableDisp(FALSE);
    if (m_pWorldIconCtrl) m_pWorldIconCtrl->enableDisp(FALSE);
    if (m_pTimeLabelAgent) m_pTimeLabelAgent->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    if (m_pMPLabelAgent) m_pMPLabelAgent->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    if (m_pTimeCtrl) m_pTimeCtrl->hide();
    if (m_pMPCtrl) m_pMPCtrl->hide();
    for (INT i = 0; i < 2; i++) {
        if (m_pResultPlateAgent[i]->getState() < C2DSeqAgent::STATE_OUT) {
            m_pResultPlateAgent[i]->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
            m_pResultLightAgent[i]->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
        }
    }
    for (INT i = 0; i < 2; i++) {
        m_pSelSlotPlate[i]->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    }
    if (m_nFlag & 4) closeMessageDlg();
    outDataList(-1);
    CSound::setBGMMasterVolume(-1, -1);
}
void CSaveLoadUI::outDataList(INT nNextState) {
    if (m_pResultPlateAgent[1]->getState() < C2DSeqAgent::STATE_OUT) {
        m_pResultPlateAgent[1]->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
        m_pResultLightAgent[1]->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
    }
    if (m_pDataListContainerAgent && m_pDataListContainerAgent->getState() < C2DSeqAgent::STATE_OUT) {
        if (m_pMemocaInfoAgent) {
            m_pMemocaInfoAgent->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
            for (INT i = 2; i >= 0; i--) {
                m_szMemocaInfoStr[i][0] = 0;
            }
        }
        m_pDataListContainerAgent->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
        C2DSeqScrollbarCtrl* pScrollbar = m_pDataListScrollbar;
        for (INT i = 0; i < 3; i++) {
            pScrollbar->m_pParts[i]->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
        }
        for (INT i = 0; i < 4; i++) {
            m_pDataItemPlate[i]->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
        }
        m_nNextState = nNextState;
        if (m_pMainCursor) m_pMainCursor->enableDisp(FALSE);
        if (m_pMainLightMark && m_pMainLightMark->isSeqCtrlVisible()) {
            m_pMainLightMark->changeState(C2DSeqAgent::STATE_OUT, TRUE, -1);
            m_pMainLightMark->m_nFlag |= 2;
        }
    } else {
        if (nNextState >= 0) changeState(nNextState);
    }
}
void CSaveLoadUI::callbackMainFrame(C2DSeqAgent& agent) {
    CSaveLoadUI* p = (CSaveLoadUI*)CTaskMan::getTask(m_hThis);
    if (!p) return;
    switch (agent.getState()) {
    case C2DSeqAgent::STATE_IN:
        CSound::setBGMMasterVolume(64, -1);
        p->changeState(6);
        if (p->m_pRoomNameCtrl) p->m_pRoomNameCtrl->enableDisp(TRUE);
        if (p->m_pWorldIconCtrl) p->m_pWorldIconCtrl->enableDisp(TRUE);
        if (p->m_pTimeCtrl) p->m_pTimeCtrl->show();
        if (p->m_pMPCtrl) p->m_pMPCtrl->show();
        break;
    case C2DSeqAgent::STATE_OUT:
        p->changeState(4);
        break;
    }
}
void CSaveLoadUI::callbackSlotPlateIn(C2DSeqAgent& agent) {
    CSaveLoadUI* p = (CSaveLoadUI*)CTaskMan::getTask(m_hThis);
    if (p) p->changeState(7);
}
void CSaveLoadUI::callbackResultPlate(C2DSeqAgent& agent) {
    CSaveLoadUI* pThis = (CSaveLoadUI*)CTaskMan::getTask(m_hThis);
    if (!pThis) return;
    INT idx = -1;
    for (INT i = 0; i < 2; i++) {
        if (&agent == pThis->m_pResultPlateAgent[i]) {
            idx = i;
            break;
        }
    }
    if (idx < 0) return;
    if (!agent.getSeqCtrl()) return;
    float ofsTbl[2][2] = {{136.0f, 48.0f}, {56.0f, -4.0f}};
    FLOAT* pTbl = ofsTbl[idx];
    FVECTOR vPos = agent.getSeqCtrl()->m_seqControl.vPos;
    vPos.x += pTbl[0];
    vPos.y += pTbl[1];
    C2DSeqAgent* pLight = pThis->m_pResultLightAgent[idx];
    if (!pLight) return;
    if (_isAgentState(pLight, C2DSeqAgent::_STATE_NOT_START)) {
        pLight->getSeqCtrl()->setLocalPos(vPos);
        pLight->start();
    } else {
        pLight->changeState(C2DSeqAgent::STATE_IN, TRUE, -1);
    }
}
void CSaveLoadUI::callbackDataList(C2DSeqAgent& agent) {
    CSaveLoadUI* pThis = (CSaveLoadUI*)CTaskMan::getTask(m_hThis);
    if (!pThis) return;
    switch (agent.getState()) {
    case C2DSeqAgent::STATE_IN:
        pThis->changeState(10);
        for (INT i = 0; i < 4; i++) {
            pThis->m_pDataItemPlate[i]->enableDispCaption(TRUE);
            pThis->m_pDataItemNumCircle[i]->enableDisp(TRUE);
            GameDataHeader* pInfo = pThis->m_pGameDataEntries->getEntry(pThis->m_nDataListTop + i);
            BOOL bDispIcon;
            if (pInfo && pInfo->hasData()) {
                sprintf(pThis->m_szDataItemTime[i], "%3d:%02d", pInfo->_nTime / 3600, pInfo->_nTime % 3600 / 60);
                bDispIcon = (pInfo->_cWorld & 0xF) < 14;
            } else {
                *pThis->m_szDataItemTime[i] = '\0';
                bDispIcon = FALSE;
            }
            pThis->m_pDataItemIcon[i]->enableDisp(bDispIcon);
            pThis->m_pDataItemClearIcon[i]->enableDisp(pInfo && pInfo->isGameCleared());
        }
        break;
    case C2DSeqAgent::STATE_OUT:
        for (INT i = 0; i < 4; i++) {
            pThis->m_pDataItemPlate[i]->enableDisp(FALSE);
            pThis->m_pDataItemNumCircle[i]->enableDisp(FALSE);
            pThis->m_pDataItemIcon[i]->enableDisp(FALSE);
            pThis->m_pDataItemClearIcon[i]->enableDisp(FALSE);
        }
        pThis->changeNextState();
        break;
    }
}
void CSaveLoadUI::callbackMemocaInfo(C2DSeqAgent& agent) {
    CSaveLoadUI* p = (CSaveLoadUI*)CTaskMan::getTask(m_hThis);
    if (!p) return;
    switch (agent.getState()) {
    case C2DSeqAgent::STATE_IN:
        p->updateMemocaInfoStr();
        p->updateMemocaInfoBar();
        break;
    case C2DSeqAgent::STATE_OUT:
        for (INT i = 0; i < 3; i++) {
            p->m_pMemocaInfoBar[i]->enableDisp(FALSE);
        }
        break;
    }
}
void CSaveLoadUI::callbackMessageDlg(C2DSeqAgent& agent) {
    CSaveLoadUI* pThis = (CSaveLoadUI*)CTaskMan::getTask(m_hThis);
    if (!pThis) return;
    switch (agent.getState()) {
    case C2DSeqAgent::STATE_IN:
        if (pThis->m_nDlgType) {
            C2DSeqSelectable* pBtn = pThis->m_pMessageDlgBtnAgent[pThis->m_nDlgSel];
            if (pBtn) pBtn->select(TRUE);
        }
        break;
    case C2DSeqAgent::STATE_OUT:
        pThis->lockPadControl(FALSE);
        if (pThis->m_pMainCursor) pThis->m_pMainCursor->setNormalSeq();
        break;
    }
}
/* Parked: registers only (22 lines): the original keeps this in s1, nOutIdx in s2 and nValDownKey in s3, ours
 * nValDownKey/this/nOutIdx in s1/s2/s3 (global-alloc priority); code and layout match once isButtonRpt/Trg are the
 * if-return-call inlines; tried: ternary/if forms of the pad inlines, permuter 4 min (5617 candidates, no gain) */
#if 0
BOOL CSaveLoadUI::checkSelector(SHORT& nOutIdx, SHORT nMax, INT nValDownKey, INT nValUpKey, SHORT nUpDownValue,
                                BOOL bSetSE, BOOL bLoopChk) {
    SHORT nNewIdx = nOutIdx;
    if (isButtonRpt(nValDownKey)) {
        nNewIdx -= nUpDownValue;
        if (nNewIdx < 0) {
            if (nOutIdx != 0) {
                nNewIdx = 0;
            } else if (!bLoopChk || isButtonTrg(nValDownKey)) {
                nNewIdx = nMax - 1;
            } else {
                nNewIdx = 0;
            }
        }
    } else if (isButtonRpt(nValUpKey)) {
        nNewIdx += nUpDownValue;
        if (nNewIdx > nMax - 1) {
            if (nOutIdx != nMax - 1) {
                nNewIdx = nMax - 1;
            } else if (!bLoopChk || isButtonTrg(nValUpKey)) {
                nNewIdx = 0;
            } else {
                nNewIdx = nMax - 1;
            }
        }
    }
    BOOL bUpdate = nNewIdx != nOutIdx;
    if (bUpdate) {
        nOutIdx = nNewIdx;
        if (bSetSE) CSound::playSE(1, -1, 64);
    }
    return bUpdate;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", checkSelector__11CSaveLoadUIRssiisii);
#endif
/* Parked: 2 lines - the original fills the beqz (nSlot) delay slot with the else branch's first `move a1, s0`
 * (pszForm), ours keeps it in the else block; everything else matches; tried: if/else polarity, szSlot at function
 * scope, a nested slot-sprintf inline, case orders, locals for each CTD text, permuter 4 min (5641 candidates) */
#if 0
void CSaveLoadUI::createBootInfoMsg(INT nRes, CHAR* pszOutMsg) {
    INT nSlot = 0;
    INT nCtd = 0x600100;
    pszOutMsg[0] = '\0';
    const CHAR* pszMemoca = _ctdText(0x600000);
    switch (nRes) {
    case -4:
        nSlot = 12;
        break;
    case -5:
        nSlot = 1;
        break;
    case -6:
        nSlot = 2;
        break;
    default:
        nCtd = 0x600102;
        break;
    }
    char szLine1[128];
    CHAR* pszForm = (CHAR*)_ctdText(nCtd);
    if (nSlot) {
        char szSlot[32];
        _getSlotName(nSlot, szSlot);
        sprintf(szLine1, pszForm, pszMemoca, szSlot);
    } else {
        sprintf(szLine1, pszForm, pszMemoca);
    }
    char szLine2[256];
    const CHAR* pszNum = _ctdText(0x100000);
    const CHAR* pszFmt = _ctdText(0x600103);
    sprintf(szLine2, pszFmt, 100, pszNum);
    const CHAR* pszInfo = _ctdText(0x600104);
    sprintf(pszOutMsg, "%s\n%s\n%s", szLine1, szLine2, pszInfo);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/SaveLoad", createBootInfoMsg__11CSaveLoadUIiPc);
#endif
void CSaveLoadUI::createCheckingMsg(INT nSlot, CHAR* pszOutMsg) {
    char szSlot[32];
    pszOutMsg[0] = '\0';
    _getSlotName(nSlot, szSlot);
    const CHAR* pszText = _ctdText(0x600000);
    sprintf(pszOutMsg, _ctdText(0x600105), pszText, szSlot, pszText);
}
HANDLE CSaveLoad::m_hThis = 0;

CSaveLoad::CSaveLoad(CTask* pParent, SAVELOAD mode, GAMECLEAR clear) : CTask(pParent, 0x8000) {
    m_nFlag = 0;
    construct(mode);
    new (com_malloc_d(sizeof(CSaveLoadUI), __FILE__ "(3485)", RET_ADDR())) CSaveLoadUI(*this, mode, clear);
}
CSaveLoad::CSaveLoad(CTask* pParent, SLMSG msg) : CTask(pParent, 0x8000) {
    m_nFlag = 0;
    construct(SLMODE_MESSAGE);
    s_nMessage = msg;
    new (com_malloc_d(sizeof(CSaveLoadUI), __FILE__ "(3493)", RET_ADDR())) CSaveLoadUI(*this, SLMODE_MESSAGE, GAMECLEAR_NONE);
}
CSaveLoad::~CSaveLoad() {
    _ref(m_hThis) = 0;
    CMainApp::enableSoftReset(!(m_nFlag & 4));
    if (m_nFlag & 2) CMainApp::setFpsHalf();
}
void CSaveLoad::construct(SAVELOAD mode) {
    if (m_hThis) {
        TRACE("\x82\xb7\x82\xc5\x82\xc9 SAVE-LOAD \x83\x81\x83j\x83\x85\x81[\x82\xcd\x8d\xec\x90\xac\x8d\xcf\x82\xdd\x82\xc5\x82\xb7\n");
        out();
        return;
    }
    if (!CMainApp::isEnableSoftReset()) m_nFlag |= 4;
    else m_nFlag &= ~4;
    CMainApp::enableSoftReset(FALSE);
    if (CMainApp::isFpsHalf()) m_nFlag |= 2;
    else m_nFlag &= ~2;
    CMainApp::setFpsFull();
    ignorePause(TRUE);
    ignorePlaySpeed(TRUE);
    m_hThis = getHandle();
    s_bSaveLoadDone = FALSE;
    if (mode == SLMODE_SAVE) CPlayer::exportStatus();
}
BOOL CSaveLoad::isExited() {
    return m_hThis == 0;
}
void CSaveLoad::disposeRsrc() {
    CRsrcDB::removeGroup(2, 0x6B);
    CRsrcDB::removeGroup(2, 0x6A);
    delete s_pFadeMask;
    s_pFadeMask = NULL;
}
BOOL CSaveLoad::isSuccessLoad() {
    return s_bSaveLoadDone;
}
BOOL CSaveLoad::isSuccessSave() {
    return s_bSaveLoadDone;
}
SLMSG_RESULT CSaveLoad::getMsgResult() {
    return s_nMsgResult;
}
static HANDLE s_hSystemDataChecker = 0;

class CSystemDataCheckerLocal : public CTask {
private:
    CMemocaAgent* m_pMemoca; /* 0x40 */
    SHORT m_nStep;           /* 0x44 */
    USHORT m_nFlag;          /* 0x46 */

public:
    CSystemDataCheckerLocal(CTask* pParent) : CTask(pParent, 0x4000) {
        m_nStep = 0;
        m_nFlag = 0;
        s_hSystemDataChecker = getHandle();
        m_pMemoca = new (com_malloc_d(sizeof(CMemocaAgent), __FILE__ "(3599)", RET_ADDR())) CMemocaAgent(this);
    }
    virtual ~CSystemDataCheckerLocal() { s_hSystemDataChecker = 0; }
    virtual void main();
    void abort() { m_nFlag |= 1; }
};

void CSystemDataCheckerLocal::main() {
    switch (m_nStep) {
    case 0:
        m_nStep = 10;
        break;
    case 10:
        if (m_pMemoca->reqLoadSysData()) m_nStep++;
        break;
    case 11:
        if (m_pMemoca->isCompleteReq(NULL)) {
            if (m_nFlag & 1) {
                out();
            } else {
                m_nStep = 0;
            }
        }
        break;
    }
}

void CSystemDataChecker::start() {
    if (s_hSystemDataChecker) {
        TRACE("\x82\xb7\x82\xc5\x82\xc9 CSystemDataChecker \x82\xcd\x8bN\x93\xae\x8d\xcf\x82\xdd\x82\xc5\x82\xb7\n");
        return;
    }
    new (com_malloc_d(sizeof(CSystemDataCheckerLocal), __FILE__ "(3687)", RET_ADDR())) CSystemDataCheckerLocal(CTaskMan::getSysTask(0));
}
void CSystemDataChecker::abort() {
    if (!s_hSystemDataChecker) return;
    CSystemDataCheckerLocal* p = (CSystemDataCheckerLocal*)CTaskMan::getTask(s_hSystemDataChecker);
    if (p) p->abort();
    else s_hSystemDataChecker = 0;
}
BOOL CSystemDataChecker::isExited() {
    return s_hSystemDataChecker == 0;
}
// SOURCE DATA: .data .rodata .bss
