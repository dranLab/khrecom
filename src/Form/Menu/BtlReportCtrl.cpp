#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#define SAVELOADCTRL_H_PATH "c:/KHCOM/MainApp/Form/Menu/SaveLoadCtrl.h"
/* the task container message follows SaveLoadCtrl.h's strings here (placeholder below) */

#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Form/Menu/SaveLoadCtrl.h"
#include "System/TaskMan.h"


#include "System/Ctd/CtdMan.h"
#include "Game/CardBattle.h"
#include "Game/Technic.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "Form/Menu/BtlReportCtrl.h"
#include <string.h>

extern "C" int sprintf(char* buf, const char* fmt, ...);

inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

C2DSeqStrings::C2DSeqStrings(CTask* pParent) : C2DSeqAgent(pParent) {
    m_pCtrl = NULL;
    m_pFnt = NULL;
}

C2DSeqStrings::~C2DSeqStrings() {
    if (m_pCtrl) delete m_pCtrl;
    m_pCtrl = NULL;
}

#line 72
void C2DSeqStrings::setupSeq(SeqTbl stbl, const CHAR* pszLad, SHORT nUnit, const CHAR* pStr, SHORT nLayer,
                             INT in_FontSize) {
    D2::C2DSeqCtrl* pCtrl;

    pCtrl = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(78)", RET_ADDR())) D2::C2DSeqCtrl;
    pCtrl->addSeq(pszLad, nUnit, stbl._tbl[0]);
    pCtrl->changeLayer(nLayer);
    pCtrl->m_seqControl.fgColor = 1;
    if (pStr) {
        memset(m_Str, 0, sizeof(m_Str));
        sprintf(m_Str, pStr);
        m_pFnt = new (com_malloc_d(sizeof(D2::C2DFnt), __FILE__ "(88)", RET_ADDR())) D2::C2DFnt(0, 0, NULL, in_FontSize, m_Str, 2);
        pCtrl->addFnt(pszLad, 0, m_pFnt);
    }
    C2DSeqAgent::setupSeq(*pCtrl, pszLad, nUnit, 0);
    m_pCtrl = pCtrl;
}

void C2DSeqStrings::setPos(FVECTOR& vPos) {
    m_pCtrl->setPos(vPos);
}

void C2DSeqStrings::changeText(const CHAR* pStr) {
    if (m_pFnt) m_pFnt->allocText(pStr);
}

void C2DSeqStrings::dispose() {
    if (m_pCtrl) delete m_pCtrl;
    m_pCtrl = NULL;
    C2DSeqAgent::dispose();
}

void C2DSeqStrings::setStyle(INT in_Style) {
    if (m_pFnt) m_pFnt->m_Style = in_Style;
}

void C2DSeqStrings::setDisp(BOOL in_Flag) {
    if (in_Flag) {
        m_pCtrl->enableDisp(TRUE);
    } else {
        m_pCtrl->enableDisp(FALSE);
    }
}

// SOURCE DATA: .rodata
