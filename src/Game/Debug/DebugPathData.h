// PARTIAL: added for Form/DebugWindowTBL
#ifndef GAME_DEBUG_DEBUGPATHDATA_H
#define GAME_DEBUG_DEBUGPATHDATA_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/MainApp.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);

class CDebugPathData : public CTask {
private:
    static CDebugPathData* m_pInstance;

public:
    CDebugPathData(CTask* p, BOOL bEnd);
    static BOOL create(CTask* p, BOOL bEnd);
    static BOOL isDebugPathData() { return m_pInstance != NULL; }

    /* Placeholders for the original header's inline functions: their strings ("%d", then the assert of
     * m_pInstance) are in .rodata of every includer. */
#line 1 "Game/Debug/DebugPathData.h"
    static void _formatNumber(char* pBuf, INT n) { sprintf(pBuf, "%d", n); }
    static CDebugPathData* _getInstance() {
        if (!m_pInstance) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
        return m_pInstance;
    }
};

#endif /* GAME_DEBUG_DEBUGPATHDATA_H */
