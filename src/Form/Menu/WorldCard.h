#ifndef FORM_MENU_WORLDCARD_H
#define FORM_MENU_WORLDCARD_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Fsm.h"
#include "System/PadHandler.h"

class C2DSeqStrings;
class C2DCard;

namespace WorldCard {

class CWorldCardSeq : public CTask, public CFSM, public CPadHandler {
private:
    static CWorldCardSeq* m_pInstance;
    CStateTemplate<CWorldCardSeq> m_State[4];  /* 0x7c */
    INT m_Type;                                /* 0xfc */
    INT m_Step;                                /* 0x100 */
    BOOL m_bFpsBackup;                         /* 0x104 */
    FLOAT m_fWait;                             /* 0x108 */
    C2DSeqStrings* m_pStrings[12];             /* 0x10c */
    C2DCard* m_pCard[5];                       /* 0x13c */

public:
    CWorldCardSeq(CTask* pParent, INT in_CreateType);
    virtual ~CWorldCardSeq();
    virtual void main();
    virtual void disp();
    static INT isNowDispCard();
    static void create(CTask* in_pCTask, INT in_CreateType);

protected:
    enum { STATE_INIT = 0, STATE_MAIN = 1, STATE_EXIT = 2, STATE_END = 3, STATE_MAX = 4 };
    void beginStateInit();
    void stateInit();
    void endStateInit();
    void beginStateMain();
    void stateMain();
    void endStateMain();
    void beginStateExit();
    void stateExit();
    void endStateExit();
    void beginStateEnd();
    void stateEnd();
    void endStateEnd();
    void changeState(INT state);
    void createGetWorldCardWnd();
    void WorldWndOne(INT in_CardKind, const CHAR* in_pStr);
    void WorldWndFour(INT* in_CardKindTable, const CHAR* in_pStr);
    void WorldWndFive(INT* in_CardKindTable, const CHAR* in_pStr);
    C2DSeqStrings* createSeq(INT in_SeqIndex, INT in_SeqType, INT in_LayoutDef, INT in_DispLayer, const CHAR* in_pStr);
};

}  // namespace WorldCard

#endif /* FORM_MENU_WORLDCARD_H */
