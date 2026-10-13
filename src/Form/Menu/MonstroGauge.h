#ifndef FORM_MENU_MONSTROGAUGE_H
#define FORM_MENU_MONSTROGAUGE_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/PadHandler.h"

namespace D2 {
class C2DSeqCtrl;
}

class CMostroGauge : public CTask, public CPadHandler {
public:
    enum EXEC_CMD {
        GAUGE_IN = 0,
        TIME_START = 1,
        TIME_STOP = 2,
        GAUGE_OUT = 3,
        SET_LIFE = 4,
        IS_TIMEOVER = 5,
        HELP_IN = 6
    };

private:
    INT m_nStep;                  /* 0x50 */
    BOOL m_bEnd;                  /* 0x54 */
    BOOL m_bTimeOver;             /* 0x58 */
    BOOL m_bTimeStop;             /* 0x5c */
    TIMEQ m_qTime;                /* 0x60 */
    TIMEQ m_qTargetTime;          /* 0x64 */
    FLOAT m_fLife;                /* 0x68 */
    FLOAT m_fLife2;               /* 0x6c */
    FLOAT m_fLifeTarget;          /* 0x70 */
    HANDLE m_hHelpLay;            /* 0x74 */
    D2::C2DSeqCtrl* m_pGaugeCtrl; /* 0x78 */
    D2::C2DSeqCtrl* m_pTimeCtrl;  /* 0x7c */

public:
    CMostroGauge(CTask* p);
    virtual ~CMostroGauge();
    BOOL exec(EXEC_CMD nCmd, INT nParam);

private:
    virtual void main();
    void setLife();
    void setTime(BOOL bInit);
};

#endif /* FORM_MENU_MONSTROGAUGE_H */
