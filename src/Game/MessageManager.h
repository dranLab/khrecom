#ifndef GAME_MESSAGEMANAGER_H
#define GAME_MESSAGEMANAGER_H

#include "System/Task.h"
#include "Game/PoolList.h"

namespace Message {

struct MSG_LIST {
    HANDLE hSender;     /* 0x00 */
    HANDLE hReceiver;   /* 0x04 */
    INT iMsg;           /* 0x08 */
    void* pExtraInfo;   /* 0x0c */
    TIMEQ tqDelay;      /* 0x10 */
    TIMEQ tqCurrent;    /* 0x14 */
    MSG_LIST* pPrev;    /* 0x18 */
    MSG_LIST* pNext;    /* 0x1c */
};

class CMessageManager : public CTask, public CPoolList<MSG_LIST> {
private:
    static CMessageManager* m_pInstance;

public:
    virtual ~CMessageManager();
    static CMessageManager& getInstance();
    virtual void main();
    BOOL sendMessage(TIMEQ inDelay, HANDLE inSender, HANDLE inReceiver, INT inMsg, void* inExtraInfo);

private:
    CMessageManager();
};

}  // namespace Message

#endif /* GAME_MESSAGEMANAGER_H */
