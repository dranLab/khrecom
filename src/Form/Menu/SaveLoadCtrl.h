// PARTIAL: added for Form/SystemMessage
#ifndef FORM_MENU_SAVELOADCTRL_H
#define FORM_MENU_SAVELOADCTRL_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Task.h"
#include "System/Graphic/2d/2DSeqCtrl.h"

/* __FILE__ of this header in the original differs per includer ("Form/Menu/SaveLoadCtrl.h" in
 * Form/SystemMessage, "c:/KHCOM/MainApp/Form/Menu/SaveLoadCtrl.h" in Form/Menu/SaveLoad): define
 * SAVELOADCTRL_H_PATH before including it to pick another one. */
#ifndef SAVELOADCTRL_H_PATH
#define SAVELOADCTRL_H_PATH "Form/Menu/SaveLoadCtrl.h"
#endif

class C2DSeqAgent : public CTask {
public:
    enum STATE {
        STATE_IN = 0,
        STATE_LOOP = 1,
        STATE_OUT = 2,
        _STATE_MAX = 3,
        _STATE_NOP = 3,
        _STATE_NOT_START = 4
    };
    struct SeqTbl {
        short _tbl[3];
    };

private:
    CHAR* m_pszLad;                    /* 0x40 */
    CHAR* m_pszSqd;                    /* 0x44 */
    HANDLE m_hSeqSingle;               /* 0x48 */
    D2::C2DSeqCtrl* m_pSeqGroup;       /* 0x4c */
    SHORT m_nSeqUnit;                  /* 0x50 */
    SHORT m_nSeqGroupTarget;           /* 0x52 */
    void (*m_pcbEndIN)(C2DSeqAgent&);  /* 0x54 */
    void (*m_pcbEndOUT)(C2DSeqAgent&); /* 0x58 */
    STATE m_nState;                    /* 0x5c */
    SeqTbl m_SeqTbl;                   /* 0x60 */
    SHORT m_nCurrentSeq;               /* 0x66 */
    SHORT m_nLayer;                    /* 0x68 */

public:
    C2DSeqAgent(CTask* pParent);
    virtual ~C2DSeqAgent();
    void setupSeq(D2::C2DSeqCtrl& seqCtrl, const CHAR* pszSqd, SHORT nLayer);
    void setupState(SHORT nIn, SHORT nLoop, SHORT nOut, void (*pcbEndIN)(C2DSeqAgent&),
                    void (*pcbEndOUT)(C2DSeqAgent&));
    void start();
    void changeState(STATE nState, BOOL bSkipIfSame, SHORT nExtSeq);
    STATE getState() const { return m_nState; }
    BOOL isNotStarted() const; /* inline in the original, but not inlined/emitted by its users */
};

/* Placeholder for an inline function of the original header at line 310 (an allocation message). */
#line 310 SAVELOADCTRL_H_PATH
inline void* _saveLoadCtrlAlloc310() {
    return com_malloc_d(4, __FILE__ "(310)", (UINT)-1);
}

#endif /* FORM_MENU_SAVELOADCTRL_H */
