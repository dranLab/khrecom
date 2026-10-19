#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/gprim.h"
#define D2MAN_H_PATH "c:/KHCOM/MainApp/System/Graphic/2d/2DMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/2d/2DCard.h"
#include "System/Graphic/2d/2DSeqObj.h"
#include "System/Vector.h"
#include "include_asm.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m, const FVECTOR& v1);
FLOAT InnerProduct(const FVECTOR& v0, const FVECTOR& v1);

namespace D2 {
class C2DMan { /* System/Graphic/2d/2DMan.h */
public:
    static HANDLE create(const CHAR* pName, UINT nIdx, SeqControl* pSeqControl);
    static void destroy(HANDLE handle);
    static C2DSeqObj* getSeqObj(HANDLE handle);
    static void enableDisp(HANDLE handle, BOOL bEnable);
    static void changeSeqLayer(HANDLE handle, INT nLayer);
};
}

using namespace D2;

/* Game/CardKind's card table: a static in the original's header, emitted (with its strings) at the end of
 * .rodata. */
static const CARD_DATA cardDataTbl[224] = {
    { { 0x00 }, 0, 0, 0, "" }, // 0
    { { 0x11 }, 2, 0, 3, "Kingdom Key" }, // 1
    { { 0x11 }, 2, 1, 3, "Three Wishes" }, // 2
    { { 0x11 }, 2, 2, 3, "Crabclaw" }, // 3
    { { 0x11 }, 2, 3, 3, "Pumpkinhead" }, // 4
    { { 0x11 }, 2, 4, 3, "Fairy Harp" }, // 5
    { { 0x11 }, 2, 5, 3, "Wishing Star" }, // 6
    { { 0x11 }, 2, 6, 3, "Spellbinder" }, // 7
    { { 0x11 }, 2, 7, 3, "Metal Chocobo" }, // 8
    { { 0x11 }, 3, 0, 3, "Olympia" }, // 9
    { { 0x11 }, 3, 1, 3, "Lionheart" }, // 10
    { { 0x11 }, 3, 2, 3, "Lady Luck" }, // 11
    { { 0x11 }, 3, 3, 3, "Divine Rose" }, // 12
    { { 0x11 }, 3, 4, 3, "Oathkeeper" }, // 13
    { { 0x11 }, 3, 5, 3, "Oblivion" }, // 14
    { { 0x11 }, 3, 6, 3, "Ultima Weapon" }, // 15
    { { 0x11 }, 3, 7, 3, "Diamond Dust" }, // 16
    { { 0x11 }, 4, 0, 3, "One-Winged Angel" }, // 17
    { { 0x11 }, 4, 1, 4, "Soul Eater" }, // 18
    { { 0x11 }, 4, 2, 3, "Star Seeker" }, // 19
    { { 0x11 }, 4, 3, 3, "Monochrome" }, // 20
    { { 0x11 }, 4, 4, 3, "Follow the Wind" }, // 21
    { { 0x11 }, 4, 5, 3, "Hidden Dragon" }, // 22
    { { 0x11 }, 4, 6, 3, "Photon Debugger" }, // 23
    { { 0x11 }, 4, 7, 3, "Bond of Flame" }, // 24
    { { 0x11 }, 26, 0, 0, "-" }, // 25
    { { 0x22 }, 5, 0, 3, "Fire" }, // 26
    { { 0x22 }, 5, 1, 3, "Blizzard" }, // 27
    { { 0x22 }, 5, 2, 3, "Thunder" }, // 28
    { { 0x22 }, 5, 3, 3, "Cure" }, // 29
    { { 0x22 }, 5, 4, 3, "Gravity" }, // 30
    { { 0x22 }, 5, 5, 3, "Stop" }, // 31
    { { 0x22 }, 5, 6, 3, "Aero" }, // 32
    { { 0x22 }, 26, 2, 0, "-" }, // 33
    { { 0x22 }, 26, 1, 0, "-" }, // 34
    { { 0x33 }, 5, 7, 3, "Simba" }, // 35
    { { 0x33 }, 6, 0, 3, "Genie" }, // 36
    { { 0x33 }, 6, 1, 3, "Bambi" }, // 37
    { { 0x33 }, 6, 2, 3, "Dumbo" }, // 38
    { { 0x33 }, 6, 3, 3, "Tinker Bell" }, // 39
    { { 0x33 }, 6, 4, 3, "Mushu" }, // 40
    { { 0x33 }, 6, 5, 3, "Cloud" }, // 41
    { { 0x44 }, 6, 6, 6, "Potion" }, // 42
    { { 0x44 }, 6, 7, 6, "Hi-Potion" }, // 43
    { { 0x44 }, 7, 0, 2, "Mega-Potion" }, // 44
    { { 0x44 }, 7, 1, 2, "Ether" }, // 45
    { { 0x44 }, 7, 2, 2, "Mega-Ether" }, // 46
    { { 0x44 }, 7, 3, 2, "Elixir" }, // 47
    { { 0x44 }, 7, 4, 2, "Megalixir" }, // 48
    { { 0x55 }, 7, 5, 2, "Donald Duck" }, // 49
    { { 0x55 }, 7, 6, 2, "Goofy" }, // 50
    { { 0x55 }, 7, 7, 2, "Aladdin" }, // 51
    { { 0x55 }, 8, 0, 2, "Ariel" }, // 52
    { { 0x55 }, 8, 1, 2, "Jack" }, // 53
    { { 0x55 }, 8, 2, 2, "Peter Pan" }, // 54
    { { 0x55 }, 8, 3, 2, "The Beast" }, // 55
    { { 0x55 }, 8, 4, 4, "The King" }, // 56
    { { 0x55 }, 8, 5, 2, "Pluto" }, // 57
    { { 0x66 }, 8, 6, 6, "Guard Armor" }, // 58
    { { 0x66 }, 8, 6, 6, "Trickmaster" }, // 59
    { { 0x66 }, 8, 6, 6, "Jafar" }, // 60
    { { 0x66 }, 8, 6, 6, "Oogie Boogie" }, // 61
    { { 0x66 }, 8, 6, 6, "Parasite Cage" }, // 62
    { { 0x66 }, 8, 6, 6, "Ursula" }, // 63
    { { 0x66 }, 8, 6, 6, "Dragon Maleficent" }, // 64
    { { 0x66 }, 8, 6, 6, "Darkside" }, // 65
    { { 0xe6 }, 8, 7, 4, "-" }, // 66
    { { 0xe6 }, 9, 0, 0, "-" }, // 67
    { { 0x77 }, 9, 1, 6, "Shadow" }, // 68
    { { 0x77 }, 9, 2, 2, "Soldier" }, // 69
    { { 0x77 }, 9, 3, 6, "Large Body" }, // 70
    { { 0x77 }, 9, 4, 2, "Red Nocturne" }, // 71
    { { 0x77 }, 9, 5, 2, "Blue Rhapsody" }, // 72
    { { 0x77 }, 9, 6, 2, "Yellow Opera" }, // 73
    { { 0x77 }, 9, 7, 2, "Green Requiem" }, // 74
    { { 0x77 }, 10, 0, 6, "Powerwild" }, // 75
    { { 0x77 }, 10, 1, 2, "Bouncywild" }, // 76
    { { 0x77 }, 10, 2, 2, "Air Soldier" }, // 77
    { { 0x77 }, 10, 3, 2, "Bandit" }, // 78
    { { 0x77 }, 10, 4, 6, "Fat Bandit" }, // 79
    { { 0x77 }, 10, 5, 2, "Barrel Spider" }, // 80
    { { 0x77 }, 10, 6, 6, "Search Ghost" }, // 81
    { { 0x77 }, 10, 7, 6, "Sea Neon" }, // 82
    { { 0x77 }, 11, 0, 2, "Screwdiver" }, // 83
    { { 0x77 }, 11, 1, 2, "Aquatank" }, // 84
    { { 0x77 }, 11, 2, 6, "Wight Knight" }, // 85
    { { 0x77 }, 11, 3, 2, "Gargoyle" }, // 86
    { { 0x77 }, 11, 4, 6, "Pirate" }, // 87
    { { 0x77 }, 11, 5, 2, "Air Pirate" }, // 88
    { { 0x77 }, 11, 6, 4, "Darkball" }, // 89
    { { 0x77 }, 11, 7, 6, "Defender" }, // 90
    { { 0x77 }, 12, 0, 2, "Wyvern" }, // 91
    { { 0x77 }, 12, 1, 2, "Wizard" }, // 92
    { { 0x77 }, 12, 2, 2, "Neoshadow" }, // 93
    { { 0x77 }, 12, 3, 2, "White Mushroom" }, // 94
    { { 0x77 }, 12, 4, 2, "Black Fungus" }, // 95
    { { 0x77 }, 12, 5, 2, "Creeper Plant" }, // 96
    { { 0x77 }, 12, 6, 2, "Tornado Step" }, // 97
    { { 0x77 }, 12, 7, 2, "Crescendo" }, // 98
    { { 0x77 }, 13, 0, 6, "Guard Armor" }, // 99
    { { 0x77 }, 13, 1, 6, "Parasite Cage" }, // 100
    { { 0x77 }, 13, 2, 6, "Trickmaster" }, // 101
    { { 0x77 }, 13, 3, 6, "Darkside" }, // 102
    { { 0x77 }, 13, 4, 2, "Card Soldier" }, // 103
    { { 0x77 }, 13, 5, 6, "Hades" }, // 104
    { { 0x77 }, 13, 6, 6, "Jafar" }, // 105
    { { 0x77 }, 13, 7, 6, "Oogie Boogie" }, // 106
    { { 0x77 }, 14, 0, 6, "Ursula" }, // 107
    { { 0x77 }, 14, 1, 6, "Hook" }, // 108
    { { 0x77 }, 14, 2, 6, "Dragon Maleficent" }, // 109
    { { 0x77 }, 14, 3, 2, "Riku" }, // 110
    { { 0x77 }, 14, 4, 2, "Axel" }, // 111
    { { 0x77 }, 14, 5, 2, "Larxene" }, // 112
    { { 0x77 }, 14, 6, 2, "Vexen" }, // 113
    { { 0x77 }, 14, 7, 2, "Marluxia" }, // 114
    { { 0x77 }, 15, 0, 6, "Lexaeus" }, // 115
    { { 0x77 }, 15, 1, 2, "Ansem" }, // 116
    { { 0x77 }, 15, 3, 6, "Zexion" }, // 117
    { { 0x77 }, 15, 4, 2, "Xemnas" }, // 118
    { { 0x77 }, 15, 5, 2, "Xigbar" }, // 119
    { { 0x77 }, 15, 6, 2, "Xaldin" }, // 120
    { { 0x77 }, 15, 7, 2, "Sa\203\307x" }, // 121
    { { 0x77 }, 16, 0, 2, "Demyx" }, // 122
    { { 0x77 }, 16, 1, 2, "Luxord" }, // 123
    { { 0x77 }, 16, 2, 2, "Roxas" }, // 124
    { { 0x77 }, 21, 6, 2, "Gold Card" }, // 125
    { { 0x77 }, 21, 7, 2, "Platinum Card" }, // 126
    { { 0x77 }, 15, 2, 0, "Card Soldier Spade" }, // 127
    { { 0x77 }, 13, 4, 0, "Card Soldier Heart" }, // 128
    { { 0x17 }, 22, 0, 0, "\203N\203\211\203E\203hA" }, // 129
    { { 0x17 }, 22, 1, 0, "\203N\203\211\203E\203hB" }, // 130
    { { 0x17 }, 22, 2, 0, "\203n\203f\203XA" }, // 131
    { { 0x27 }, 22, 3, 0, "\203n\203f\203XB" }, // 132
    { { 0x17 }, 22, 4, 0, "\203t\203b\203NA" }, // 133
    { { 0x17 }, 22, 5, 0, "\203t\203b\203NB" }, // 134
    { { 0x17 }, 22, 6, 0, "\203t\203b\203NC" }, // 135
    { { 0x27 }, 22, 7, 0, "\203t\203b\203ND" }, // 136
    { { 0x17 }, 23, 0, 0, "\203A\203N\203Z\203\213A" }, // 137
    { { 0x17 }, 23, 1, 0, "\203A\203N\203Z\203\213B" }, // 138
    { { 0x17 }, 23, 2, 0, "\203\211\203N\203V\201[\203kA" }, // 139
    { { 0x17 }, 23, 3, 0, "\203\211\203N\203V\201[\203kB" }, // 140
    { { 0x27 }, 23, 4, 0, "\203\211\203N\203V\201[\203kC" }, // 141
    { { 0x17 }, 23, 5, 0, "\203\212\203NA" }, // 142
    { { 0x17 }, 23, 6, 0, "\203\212\203NB" }, // 143
    { { 0x17 }, 23, 7, 0, "\203\212\203NC" }, // 144
    { { 0x17 }, 24, 0, 0, "\203\212\203ND" }, // 145
    { { 0x17 }, 24, 1, 0, "\203\224\203B\203N\203Z\203\223A" }, // 146
    { { 0x27 }, 24, 2, 0, "\203\224\203B\203N\203Z\203\223B" }, // 147
    { { 0x17 }, 24, 3, 0, "\203}\201[\203\213\201[\203V\203\203A" }, // 148
    { { 0x17 }, 24, 4, 0, "\203}\201[\203\213\201[\203V\203\203B" }, // 149
    { { 0x27 }, 24, 5, 0, "\203}\201[\203\213\201[\203V\203\203C" }, // 150
    { { 0x77 }, 24, 6, 0, "\203}\201[\203\213\201[\203V\203\2032" }, // 151
    { { 0x77 }, 24, 7, 0, "\203}\201[\203\213\201[\203V\203\2033" }, // 152
    { { 0x17 }, 25, 0, 0, "\203\214\203N\203Z\203E\203XA" }, // 153
    { { 0x17 }, 25, 1, 0, "\203\214\203N\203Z\203E\203XB" }, // 154
    { { 0x27 }, 25, 2, 0, "\203\214\203N\203Z\203E\203XC" }, // 155
    { { 0x17 }, 25, 3, 0, "\203A\203\223\203Z\203\200A" }, // 156
    { { 0x27 }, 25, 4, 0, "\203A\203\223\203Z\203\200B" }, // 157
    { { 0x27 }, 25, 5, 0, "\203[\203N\203V\203I\203\223A" }, // 158
    { { 0x77 }, 26, 3, 0, "-" }, // 159
    { { 0x88 }, 0, 0, 6, "\203\212\203\215\201[\203h" }, // 160
    { { 0x99 }, 16, 3, 6, "Tranquil Darkness" }, // 161
    { { 0x99 }, 16, 4, 6, "Teeming Darkness" }, // 162
    { { 0x99 }, 16, 5, 6, "Feeble Darkness" }, // 163
    { { 0x99 }, 16, 6, 6, "Almighty Darkness" }, // 164
    { { 0x99 }, 16, 7, 6, "Sleeping Darkness" }, // 165
    { { 0x99 }, 17, 0, 6, "Looming Darkness" }, // 166
    { { 0x99 }, 17, 1, 2, "Premium Room" }, // 167
    { { 0x99 }, 17, 2, 2, "White Room" }, // 168
    { { 0x99 }, 17, 3, 6, "Black Room" }, // 169
    { { 0x99 }, 17, 4, 6, "Bottomless Darkness" }, // 170
    { { 0x99 }, 17, 5, 6, "Roulette Room" }, // 171
    { { 0xa9 }, 17, 6, 6, "Martial Waking" }, // 172
    { { 0xa9 }, 17, 7, 2, "Sorcerous Waking" }, // 173
    { { 0xa9 }, 18, 0, 2, "Alchemic Waking" }, // 174
    { { 0xa9 }, 18, 1, 6, "Meeting Ground" }, // 175
    { { 0xa9 }, 18, 2, 6, "Stagnant Space" }, // 176
    { { 0xa9 }, 18, 3, 6, "Strong Initiative" }, // 177
    { { 0xa9 }, 18, 4, 6, "Lasting Daze" }, // 178
    { { 0xb9 }, 18, 5, 2, "Calm Bounty" }, // 179
    { { 0xb9 }, 18, 6, 2, "Guarded Trove" }, // 180
    { { 0xb9 }, 18, 7, 2, "False Bounty" }, // 181
    { { 0xb9 }, 19, 0, 6, "Moment's Reprieve" }, // 182
    { { 0xb9 }, 19, 1, 6, "Mingling Worlds" }, // 183
    { { 0xb9 }, 19, 2, 2, "Moogle Room" }, // 184
    { { 0xb9 }, 19, 3, 6, "Random Joker" }, // 185
    { { 0xca }, 19, 4, 6, "Key of Beginnings" }, // 186
    { { 0xca }, 19, 5, 6, "Key of Guidance" }, // 187
    { { 0xca }, 19, 6, 6, "Key to Truth" }, // 188
    { { 0xca }, 19, 7, 2, "Key to Rewards" }, // 189
    { { 0xdb }, 20, 0, 6, "Traverse Town" }, // 190
    { { 0xdb }, 20, 3, 6, "Agrabah" }, // 191
    { { 0xdb }, 20, 2, 6, "Olympus Coliseum" }, // 192
    { { 0xdb }, 20, 1, 6, "Wonderland" }, // 193
    { { 0xdb }, 20, 5, 6, "Monstro" }, // 194
    { { 0xdb }, 20, 4, 6, "Halloween Town" }, // 195
    { { 0xdb }, 20, 6, 6, "Atlantica" }, // 196
    { { 0xdb }, 20, 7, 6, "Neverland" }, // 197
    { { 0xdb }, 21, 0, 6, "Hollow Bastion" }, // 198
    { { 0xdb }, 21, 1, 2, "100 Acre Woods" }, // 199
    { { 0xdb }, 21, 2, 6, "Twilight Town" }, // 200
    { { 0xdb }, 21, 3, 6, "Destiny Islands" }, // 201
    { { 0xdb }, 21, 4, 4, "Castle Oblivion" }, // 202
    { { 0x0d }, 0, 0, 0, "EXP\220\302\217\254" }, // 203
    { { 0x0d }, 0, 1, 0, "EXP\220\302\222\206" }, // 204
    { { 0x0d }, 0, 2, 0, "EXP\220\302\221\345" }, // 205
    { { 0x0d }, 0, 3, 0, "EXP\220\324\217\254" }, // 206
    { { 0x0d }, 0, 4, 0, "EXP\220\324\222\206" }, // 207
    { { 0x0d }, 0, 5, 0, "EXP\220\324\221\345" }, // 208
    { { 0x0d }, 0, 6, 0, "EXP\213\342" }, // 209
    { { 0x0d }, 1, 0, 0, "HP\217\254" }, // 210
    { { 0x0d }, 1, 1, 0, "HP\221\345" }, // 211
    { { 0x0d }, 2, 0, 0, "MP\217\254" }, // 212
    { { 0x0d }, 2, 1, 0, "MP\221\345" }, // 213
    { { 0x0d }, 3, 0, 0, "P" }, // 214
    { { 0x0d }, 4, 0, 0, "R" }, // 215
    { { 0x0c }, 0, 0, 0, "Unknown Room" }, // 216
    { { 0x0c }, 0, 0, 0, "Conqueror's Respite" }, // 217
    { { 0x0c }, 0, 0, 0, "Room of Rewards" }, // 218
    { { 0x0c }, 0, 0, 0, "" }, // 219
    { { 0x0c }, 0, 0, 0, "" }, // 220
    { { 0x00 }, 0, 0, 0, "" }, // 221
    { { 0x00 }, 0, 0, 0, "" }, // 222
    { { 0x00 }, 0, 0, 0, "" }, // 223
};

LPCTSTR C2DCard::m_pSeqName = "card:00";

C2DCard::C2DCard() {
    m_card.m_nId = 0;
    m_idxSelect = 0;
    m_fgBack = 0;
    m_hSeqHandleBack = 0;
    m_pParentCtrl = NULL;
}

C2DCard::C2DCard(INT nType) : C2DSeqCtrl(nType) {
    m_card.m_nId = 0;
    m_idxSelect = 0;
    m_fgBack = 0;
    m_pParentCtrl = NULL;
}

C2DCard::~C2DCard() {
    if (m_hSeqHandleBack) C2DMan::destroy(m_hSeqHandleBack);
}

void C2DCard::replace(CARD inCard) {
    CHAR nLayer = m_nLayer;
    destroy();
    m_nLayer = nLayer;
    create(inCard);
}

void C2DCard::create(CARD inCard) {
    static LPCTSTR seqName2 = "card:01";
    static LPCTSTR seqName3 = "card:02";
    static LPCTSTR texName[4] = { "card:%02d", "card_b:%02d", "card_p:%02d", "card_n:%02d" };
    static BYTE wakuTbl[15] = { 0x00, 0x07, 0x05, 0x05, 0x06, 0x06, 0x06, 0x04, 0x2F, 0x02, 0x01, 0x00, 0x03, 0x09, 0x07 };
    static BYTE numTbl[15] = { 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1 };
    if (inCard.m_nCategory20 == 0xC) return;
    if (inCard.m_nKind == 0xA0) {
        addSeq(m_pSeqName, 0x2E);
        if (inCard.m_nNumber < 0) {
            addSeq(m_pSeqName, 0x20);
            m_card = inCard;
            return;
        }
    } else {
        C2DSeqObj* pObj = addSeq(seqName3, 0);
        if (pObj) {
            const CARD_DATA* pData = &cardDataTbl[inCard.m_nKind];
            UINT nTexPos = pData->nTexPos;
            UINT nTexNo = pData->nTexNo;
            G_UV uvStart, uvEnd;
            CHAR szTex[16];
            USHORT u = ((nTexPos & 3) << 6) | 2;
            USHORT v = ((nTexPos & 0xC) << 4) | 1;
            uvEnd.u = u | 0x3C;
            uvEnd.v = v | 0x3E;
            uvStart.u = u;
            uvStart.v = v;
            if (nTexNo < 22) sprintf(szTex, texName[0], nTexNo);
            else if (nTexNo < 26) sprintf(szTex, texName[1], nTexNo - 22);
            else if (nTexNo < 27) sprintf(szTex, texName[2], nTexNo - 26);
            else sprintf(szTex, texName[3], nTexNo - 27);
            pObj->changeTexture(0, szTex, &uvStart, &uvEnd);
        }
    }
    addSeq((inCard.m_nCategory21 != 8) ? seqName2 : m_pSeqName,
           (inCard.m_nKind == 0xB9) ? 0x14 : wakuTbl[inCard.m_nCategory21]);
    if (numTbl[inCard.m_nCategory21]) {
        if (inCard.m_nNumber >= 0 && inCard.m_nKind != 0xB9) {
            addSeq(m_pSeqName, inCard.m_fgPremium ? inCard.m_nNumber + 0x3D : inCard.m_nNumber + 0x23);
        }
    }
    if (inCard.m_fgPremium || inCard.m_nCategory21 == 0xC || inCard.m_nKind == 0x7D || inCard.m_nKind == 0x7E) {
        addSeq(m_pSeqName, 0x2D);
    }
    m_card = inCard;
}

void C2DCard::replaceNumberNormal(BYTE inNumber) {
    if (m_nSeq >= 3) replaceSeq(2, m_pSeqName, m_card.m_fgPremium ? inNumber + 0x3D : inNumber + 0x23);
}

void C2DCard::replaceNumberBlue(BYTE inNumber) {
    if (m_nSeq >= 3) replaceSeq(2, m_pSeqName, inNumber + 0x33);
}

void C2DCard::replaceNumberRoulette() {
    if (m_nSeq >= 3) replaceSeq(2, m_pSeqName, 0x47);
}

void C2DCard::select(BOOL bUnusable) {
    if (m_nSeq && !m_idxSelect) {
        m_idxSelect = m_nSeq;
        addSeq(m_pSeqName, bUnusable ? 0x48 : 0);
    }
}

void C2DCard::unselect() {
    if (m_idxSelect) {
        destroy(m_idxSelect);
        m_idxSelect = 0;
    }
}

void C2DCard::changeTime(UINT inTime) {
    if (!m_idxSelect || m_card.m_nKind != 0xA0) {
        C2DSeqCtrl::changeTime(inTime);
    } else {
        for (INT i = 0; i < m_idxSelect; i++) C2DSeqCtrl::changeTime(i, inTime);
    }
}

void C2DCard::createEmpty() {
    addSeq(m_pSeqName, 1);
}

/* not matching: the original passes inNorma to createNorma as four halfword loads combined into a1 (ours loads
 * the copy with one ld) */
#if 0
void C2DCard::replaceNorma(RCNORMA::NormaSrc inNorma, INT inStatus) {
    destroy();
    createNorma(inNorma, inStatus);
}
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DCard", replaceNorma__7C2DCardGQ27RCNORMA8NormaSrci);

void C2DCard::createNorma(RCNORMA::NormaSrc inNorma, INT inStatus) {
    static BYTE wakuTbl[4] = { 0x20, 0x22, 0x21, 0x1F };
    static BYTE rgbUpperTbl[4] = { 0x00, 0x07, 0x05, 0x03 };
    static BYTE opTbl[4] = { 0x00, 0x13, 0x14, 0x12 };
    C2DSeqObj* p2DSeqObj;
    G_UV uvStart;
    G_UV uvEnd;
    addSeq(m_pSeqName, wakuTbl[inNorma._nColor]);
    if (inNorma._nColor) addSeq(m_pSeqName, rgbUpperTbl[inNorma._nColor]);
    if (inNorma._nOp) {
        if (inNorma._nOp != 4) {
            addSeq(m_pSeqName, inNorma._nNumber + 8);
            addSeq(m_pSeqName, opTbl[inNorma._nOp]);
        } else {
            INT nLow = inNorma._nNumber % 10;
            p2DSeqObj = addSeq(m_pSeqName, nLow + 0x15);
            if (inNorma._nNumber < 10) p2DSeqObj->shiftBase(-15.0f, 0.0f);
            else {
                INT nNum = inNorma._nNumber;
                INT nHigh = nNum / 10;
                addSeq(m_pSeqName, nHigh + 8);
            }
        }
    }
}

void C2DCard::createBack() {
    static LPCTSTR seqName2 = "card:01";
    static BYTE uraTbl[15] = { 0x00, 0x11, 0x0F, 0x0F, 0x10, 0x10, 0x10, 0x0E, 0x0E, 0x0C, 0x0B, 0x0A, 0x0D, 0x13, 0x11 };
    UINT idx = (m_card.m_nKind == 0xB9) ? 0x15 : uraTbl[m_card.m_nCategory21];
    if (idx) {
        HANDLE h = C2DMan::create(seqName2, idx, &m_seqControl);
        if (h) {
            C2DSeqObj* pObj = C2DMan::getSeqObj(h);
            if (pObj) {
                pObj->setParent();
                if (m_nLayer >= 0) pObj->changeLayer(m_nLayer);
                m_hSeqHandleBack = h;
            }
        }
    }
}

void C2DCard::setRot(FVECTOR& vRot) {
    C2DSeqCtrl::setRot(vRot);
    FVECTOR v;
    v.x = 0;
    v.y = 0;
    v.z = 1.0f;
    v.w = 1.0f;
    ApplyMatrix(v, *(FMATRIX*)&m_seqControl, v);
    if (v.z >= 0.0f) {
        if (m_fgBack) {
            m_fgBack = 0;
            if (m_bEnable) enableDisp(TRUE);
            C2DMan::enableDisp(m_hSeqHandleBack, FALSE);
        }
    } else if (!m_fgBack) {
        m_fgBack = 1;
        if (!m_hSeqHandleBack) createBack();
        for (INT i = 0; i < m_nSeq; i++) C2DMan::enableDisp(m_hSeqHandle[i], FALSE);
        if (m_bEnable) C2DMan::enableDisp(m_hSeqHandleBack, TRUE);
    }
}

void C2DCard::checkBack() {
    if (!m_nSeq) return;
    if (m_seqControl.pParentSeqControl) {
        int* pHandle = m_pParentCtrl->getHandle();
        if (!pHandle) return;
        C2DSeqObj* pObj = C2DMan::getSeqObj(*pHandle);
        if (!pObj) return;
        pObj->_main();
        pObj->m_fgMainSkip = 1;
    }
    FMATRIX* pMat = m_seqControl.pParentSeqControl ? (FMATRIX*)((CHAR*)m_seqControl.pParentSeqControl + 0x80)
                                                   : (FMATRIX*)&m_seqControl;
    FVECTOR v;
    v.x = 0;
    v.y = 0;
    v.z = 1.0f;
    v.w = 1.0f;
    v.w = InnerProduct(v, pMat->v[2]);
    if (v.w >= 0.0f) {
        if (m_fgBack) {
            m_fgBack = 0;
            enableDisp(TRUE);
            C2DMan::enableDisp(m_hSeqHandleBack, FALSE);
        }
    } else if (!m_fgBack) {
        m_fgBack = 1;
        enableDisp(FALSE);
        if (!m_hSeqHandleBack) createBack();
        C2DMan::enableDisp(m_hSeqHandleBack, TRUE);
    }
}

void C2DCard::setParent(C2DSeqCtrl* p2DSeqCtrl) {
    m_pParentCtrl = p2DSeqCtrl;
    C2DSeqCtrl::setParent(p2DSeqCtrl);
}

void C2DCard::enableDisp(BOOL bEnable) {
    if (m_hSeqHandleBack) {
        if (!m_fgBack) {
            C2DSeqCtrl::enableDisp(bEnable);
        } else {
            C2DMan::enableDisp(m_hSeqHandleBack, bEnable);
            if (bEnable) m_bEnable = bEnable;
            else C2DSeqCtrl::enableDisp(FALSE);
        }
    } else {
        C2DSeqCtrl::enableDisp(bEnable);
    }
}

void C2DCard::changeLayer(INT nLayer) {
    C2DSeqCtrl::changeLayer(nLayer);
    if (m_hSeqHandleBack) C2DMan::changeSeqLayer(m_hSeqHandleBack, nLayer);
}

// SOURCE DATA: .data .rodata
