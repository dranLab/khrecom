#ifndef GAME_NOWLOADING_H
#define GAME_NOWLOADING_H

#include "khtypes.h"
#include "System/Task.h"

class CSprt;

class CNowLoading : public CTask {
private:
    INT m_nStep;
    CSprt* m_pSprite;
    INT m_nX;
    INT m_nY;
    TIMEQ m_nTimer;
    TIMEQ m_nSleepTimer;
    SHORT m_nSpritePtn;
    SHORT m_nFlag;

public:
    CNowLoading(CTask* pParent);
    virtual ~CNowLoading();
    static void enable(BOOL bEnable);
    static BOOL isEnable();
    static void start();
    static void end(BOOL bRightNow);
    static void loadRsrc();
    static BOOL hasRsrc();
    static void setMode(INT nMode);
    static INT getMode();
    virtual void main();
    virtual void disp();
    void sleep(BOOL bRightNow);
    void wakeup();

private:
    void setupSprite();
    void animSprite();
    void setPtn();
    void runProc();
    void halfSleepProc();
    void sleepProc();
};

#endif /* GAME_NOWLOADING_H */
