// PARTIAL: added for Form/DebugWindowTBL
#ifndef GAME_DEBUG_DEBUGPATHDATA_H
#define GAME_DEBUG_DEBUGPATHDATA_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/MainApp.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);

#ifdef GAME_DEBUG_DEBUGPATHDATA_CPP
/* PARTIAL: the full class, for Game/Debug/DebugPathData only (no SOURCE DATA yet: the inline's strings are labels) */
class CPathData;
class CNakCamera;
class CPDFrameTime;
extern "C" char D_00485FA8[]; /* "c:/KHCOM/MainApp/Game/Debug/DebugPathData.h" */
extern "C" char D_00485FD8[]; /* "m_pInstance" */
class CDebugPathData : public CTask {
    CPathData* m_pPath;       /* 0x40 */
    CNakCamera* m_pDCam;      /* 0x44 */
    char m_szFileName[64];    /* 0x48 */
    HANDLE m_hChildWnd;       /* 0x88 */
    UBYTE m_gpPArrowPoly[0x50] __attribute__((aligned(16))); /* 0x90 CCPrim */
    static CDebugPathData* m_pInstance;

public:
    static INT m_iActivePath;
    static INT m_iActivePathNum;
    static FLOAT m_fValuePlus;
    static char m_szNames[128][16];
    static CPDFrameTime* m_pFrame;
    static UINT m_iCtrlFlg;

    CDebugPathData(CTask* p, BOOL bEnd);
    virtual ~CDebugPathData();
    virtual void main();
    virtual void disp();
    static BOOL create(CTask* p, BOOL bEnd);
    static void makeFileName(const CHAR* pszName, BOOL bExt);
    static void entryCam();
    static CNakCamera* getCam();
    static void removeCam();
    static INT getActivePathNum() { return m_iActivePath; }

private:
    BOOL isEnd();
    static CDebugPathData* getInstance() {
#line 259 "Game/Debug/DebugPathData.h"
        if (!(m_pInstance)) AssertApp(D_00485FA8, __LINE__, D_00485FD8, 0);
        return m_pInstance;
    }
};
#else
class CDebugPathData : public CTask {
private:
    static CDebugPathData* m_pInstance;

public:
    static UINT m_iCtrlFlg; // PARTIAL: added for System/Graphic/PathDataCamera

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
#endif

#endif /* GAME_DEBUG_DEBUGPATHDATA_H */
