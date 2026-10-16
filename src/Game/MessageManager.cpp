#include "include_asm.h"
#include "System/Graphic/modeldata.h"
#include "System/Task.h"
#include "Game/MessageManager.h"
#include "System/TaskMan.h"
#include "Game/Entity.h"

inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

namespace Message {

CMessageManager* CMessageManager::m_pInstance = NULL;

CMessageManager::CMessageManager() : CTask(getSysTask(1), 0x4000), CPoolList<MSG_LIST>(128) {
    declareSysTask(23);
}

CMessageManager::~CMessageManager() {
    CMessageManager*& rInstance = m_pInstance;
    rInstance = NULL;
}

#line 46
CMessageManager& CMessageManager::getInstance() {
    if (m_pInstance == NULL) {
        m_pInstance = new (com_malloc_d(sizeof(CMessageManager), __FILE__ "(48)", RET_ADDR())) CMessageManager;
        if (!m_pInstance) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
    }
    return *m_pInstance;
}

void CMessageManager::main() {
    MSG_LIST* p;
    for (p = m_Top.pNext; p->pNext != NULL; p = p->pNext) {
        p->tqCurrent += getFrameRate();
        if (p->tqCurrent >= p->tqDelay) {
            CTask* task = CTaskMan::getTask(p->hReceiver);
            if (task) {
                INT typeID = task->getTypeID();
                if (typeID == 0x1000 || typeID == 0x1002 || typeID == 0x1004) {
                    TRACE("Send message = %d, Delay = %d, Current = %d, Owner = 0x%08x\n", p->iMsg, p->tqDelay, p->tqCurrent, task);
                    ((CEntity*)task)->onMsg(p->iMsg, p->pExtraInfo);
                }
            }
            releaseList(p);
        }
    }
}

/* sendMessage: with the getList() in Game/PoolList.h the source below differs only in the branch layout of
 * getList's empty-slot test (the original branches likely to the body and jumps to the end). */
}  // namespace Message
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/MessageManager", sendMessage__Q27Message15CMessageManageriiiiPv);
namespace Message {
#if 0
BOOL CMessageManager::sendMessage(TIMEQ inDelay, HANDLE inSender, HANDLE inReceiver, INT inMsg, void* inExtraInfo) {
    MSG_LIST* p = getList();
    if (p == NULL) return FALSE;
    p->hSender = inSender;
    p->hReceiver = inReceiver;
    p->iMsg = inMsg;
    p->pExtraInfo = inExtraInfo;
    p->tqDelay = inDelay;
    p->tqCurrent = 0;
    return TRUE;
}
#endif

}  // namespace Message

// SOURCE DATA: .data .rodata
