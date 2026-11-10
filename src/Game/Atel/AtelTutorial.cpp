
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Graphic/Anim.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: two empty strings follow CollUtil.h's strings in .rodata; which headers they come from is
 * not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}

#include "System/Vector.h"

#include "Game/Atel/AtelTutorial.h"
#include "Game/Player/Player.h"
#include "Game/GameWork.h"
#include "System/HeapMan.h"



CAtelTutorial::CAtelTutorial(CTask* pParent) : CTask(pParent, 0x8000) {
    TRACE("CAtelTutorial Create\n");
}

CAtelTutorial::~CAtelTutorial() {
    TRACE("CAtelTutorial Delete\n");
}

void CAtelTutorial::main() {
    CPlayer* pPlayer = (CPlayer*)getSysTask(3);
    if (pPlayer) {
        switch (pPlayer->getPlayerState()) {
        case 7:
            CGameWork::setFlag(1, 13, 1);
            break;
        case 6:
            CGameWork::setFlag(1, 14, 1);
            break;
        }
        if (pPlayer->cmdRevolver_isEmpty()) {
            CGameWork::setFlag(1, 17, 1);
        }
    }
}

CAtelCardSeq::CAtelCardSeq(CTask* pParent, INT in_type, INT in_posX, INT in_posY) : CTask(pParent, 0x8000) {
    TRACE("CAtelCardSeq Create\n");
    char* mesfile = "message.sqd";
    m_pSeqArrow = NULL;
    m_pSeqArrow = new (__FILE__ "(80)") D2::C2DSeqCtrl;
    if (m_pSeqArrow != NULL) {
        switch (in_type) {
        case 0:
            m_pSeqArrow->addSeq(mesfile, 2);
            break;
        case 1:
            m_pSeqArrow->addSeq(mesfile, 3);
            break;
        case 2:
            m_pSeqArrow->addSeq(mesfile, 4);
            break;
        }
        FVECTOR v;
        v.set(in_posX, in_posY, 0.0f, 1.0f);



        m_pSeqArrow->setLocalPos(v);
    } else {
        TRACE("\x92\x76\x96\xbd\x93\x49\x82\xc8\x83\x47\x83\x89\x81\x5b\x81\x46\x83\x60\x83\x85\x81\x5b\x83\x67\x83\x8a\x83\x41\x83\x8b CAtelCardSeq\n");
    }
}

CAtelCardSeq::~CAtelCardSeq() {
    if (m_pSeqArrow) delete m_pSeqArrow;
    TRACE("CAtelCardSeq Delete\n");
}

void CAtelCardSeq::main() {}
// SOURCE DATA: .rodata
