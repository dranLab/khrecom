#ifndef GAME_DEBUG_DEBUGPATHDATA_H
#define GAME_DEBUG_DEBUGPATHDATA_H
#ifndef KH_RETAIL /* retail has no path debugger */
#include "khtypes.h"
#include "System/Task.h"
#include "System/MainApp.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);

#include "System/Graphic/PathData.h"
#include "System/Graphic/Str.h"
#include "System/DebugProcInfo.h"
#ifndef DEBUGPATHDATA_H_PATH
#define DEBUGPATHDATA_H_PATH __FILE__
#endif
class CPathData;
class CNakCamera;
class CPDFrameTime {
    INT m_iFlags;      /* 0x00 */
    FLOAT m_fTimer;    /* 0x04 */
    char m_szStr[32];  /* 0x08 */
    CFnt m_cFnt;
    CTile m_cTile;

public:
    CPDFrameTime() {
        m_iFlags = 0;
        m_fTimer = 0.0f;
        m_szStr[0] = 0;
        m_cFnt.setText(m_szStr);
        m_cFnt.setStyle(1);
        m_cFnt.setSize(16);
        m_cFnt.setLayerFG(8);
        m_cFnt.setPosition(240, -168, 0xFFFF28);
        m_cTile.m_C0.set(0, 0, 0, 0x60);
        m_cTile.setSemiTrans((ALPHA)1);
        m_cTile.setLayerFG(8);
        m_cTile.m_P0.set(112 << 4, -184 << 4, 0xFFFF280);
        m_cTile.m_P1.set(244 << 4, -166 << 4, 0xFFFF280);
    }
    ~CPDFrameTime() {}
    void reset() { *(FLOAT*)&m_fTimer = 0.0f; } // MATCH: not a struct-field store, so callers reload m_pFrame after it (gcc 2.95 alias heuristic; SimulateSpeedLineCall)
    void start() { *(INT*)&m_iFlags |= 2; } // MATCH: as reset
    void stop() { m_iFlags &= ~2; }
    void on() { *(INT*)&m_iFlags |= 1; } // MATCH: as reset
    void off() { m_iFlags &= ~1; }
    void disp(FLOAT fRate) {
        if (m_iFlags & 1) {
            if (m_iFlags & 2) m_fTimer += fRate;
            sprintf(m_szStr, "%d", (INT)m_fTimer);
            m_cTile.disp();
            m_cFnt.disp();
        }
    }
};
class CDebugPathData : public CTask {
    CPathData* m_pPath;       /* 0x40 */
    CNakCamera* m_pDCam;      /* 0x44 */
    char m_szFileName[64];    /* 0x48 */
    HANDLE m_hChildWnd;       /* 0x88 */
    CCPrim m_gpPArrowPoly;    /* 0x90 */
    static CDebugPathData* m_pInstance;

public:
    static INT m_iActivePath;
    static INT m_iActivePathNum;
    static FVECTOR m_vActivePathPos;
    static FLOAT m_fValuePlus;
    static PATH_PARAM m_sParam;
    static FVECTOR m_vBackUpPos;
    static char m_szPathName[64];
    static char m_szPathActName[64];
    static char m_szNames[128][16];
    static CPDFrameTime* m_pFrame;
    static UINT m_iCtrlFlg;

    CDebugPathData(CTask* p, BOOL bEnd);
    virtual ~CDebugPathData();
    virtual void main();
    virtual void disp();
    static BOOL create(CTask* p, BOOL bEnd);
    static BOOL isDebugPathData() { return m_pInstance != NULL; }
    static void makeFileName(const CHAR* pszName, BOOL bExt);
    static INT getPathNum() { return getInstance()->m_pPath->getPathNum(); }
    static INT getPathVectorNum() { return getInstance()->m_pPath->getPathVectorNum(m_iActivePath); }
    static FVECTOR* getPathVector() { return getInstance()->m_pPath->getPathVector(m_iActivePath); }
    static CPathVector* getPathVectorData() { return getInstance()->m_pPath->getPathVectorData(m_iActivePath); }
    static CPathData* getPathPointer() { return getInstance()->m_pPath; }
    static void setActivePathNum(INT iNum, INT iVnum) {
        getInstance();
        m_iActivePath = iNum;
        if (iNum >= 0 && iNum < getInstance()->m_pPath->getPathNum()) {
            if (iVnum >= 0) setActivePathVecNum(iVnum);
            limitClampActivePathVecNum();
            getActiveParam();
        }
    }
    static INT getActivePathNum() { return m_iActivePath; }
    static BOOL addPath(const FVECTOR* pVec, INT iVNum) { return getInstance()->m_pPath->addPath(pVec, iVNum); }
    static BOOL delPath(INT iNum) { return getInstance()->m_pPath->delPath(iNum); }
    static void limitClampActivePathNum() {
        getInstance();
        if (m_iActivePath >= getInstance()->m_pPath->getPathNum()) setActivePathNum(getInstance()->m_pPath->getPathNum() - 1, -1);
        if (m_iActivePath < 0) setActivePathNum(0, -1);
        else setActivePathNum(m_iActivePath, -1);
    }
    static void limitRepeatActivePathNum() {
        getInstance();
        if (m_iActivePath >= getInstance()->m_pPath->getPathNum()) setActivePathNum(0, -1);
        if (m_iActivePath < 0) setActivePathNum(getInstance()->m_pPath->getPathNum() - 1, -1);
        else setActivePathNum(m_iActivePath, -1);
    }
    static void incActivePathNum(BOOL bRpt) {
        getInstance();
        setActivePathNum(m_iActivePath + 1, -1);
        if (bRpt) limitRepeatActivePathNum();
        else limitClampActivePathNum();
    }
    static void decActivePathNum(BOOL bRpt) {
        getInstance();
        setActivePathNum(m_iActivePath - 1, -1);
        if (bRpt) limitRepeatActivePathNum();
        else limitClampActivePathNum();
    }
    static void setActivePathVecNum(INT iNum) {
        CDebugPathData* p = getInstance();
        m_iActivePathNum = iNum;
        if (iNum >= 0 && iNum < getInstance()->m_pPath->getPathVectorNum(m_iActivePath)) {
            p->m_pPath->setActive(m_iActivePath, iNum);
            m_vActivePathPos = p->m_pPath->getPathPosition(m_iActivePath, m_iActivePathNum);
        }
    }
    static void setActivePathVecPosition(const FVECTOR& vPos) {
        CDebugPathData* p = getInstance();
        m_vActivePathPos = vPos;
        p->m_pPath->setPathPosition(m_iActivePath, m_iActivePathNum, vPos);
    }
    static void setActivePathVecPositionXYZ(const FVECTOR& vPos) {
        CDebugPathData* p = getInstance();
        m_vActivePathPos.x = vPos.x;
        m_vActivePathPos.y = vPos.y;
        m_vActivePathPos.z = vPos.z;
        p->m_pPath->setPathPosition(m_iActivePath, m_iActivePathNum, m_vActivePathPos);
    }
    static INT getActivePathVecNum() { return m_iActivePathNum; }
    static FVECTOR& getActivePathVecPosition() { return m_vActivePathPos; }
    static BOOL addPathVector(INT iNum) { return getInstance()->m_pPath->addPathVector(m_iActivePath, iNum); }
    static BOOL delPathVector(INT iNum) { return getInstance()->m_pPath->delPathVector(m_iActivePath, iNum); }
    static void limitClampActivePathVecNum() {
        getInstance();
        if (m_iActivePathNum >= getInstance()->m_pPath->getPathVectorNum(m_iActivePath))
            setActivePathVecNum(getInstance()->m_pPath->getPathVectorNum(m_iActivePath) - 1);
        if (m_iActivePathNum < 0) setActivePathVecNum(0);
        else setActivePathVecNum(m_iActivePathNum);
    }
    static void limitRepeatActivePathVecNum() {
        getInstance();
        if (m_iActivePathNum >= getInstance()->m_pPath->getPathVectorNum(m_iActivePath)) setActivePathVecNum(0);
        if (m_iActivePathNum < 0) setActivePathVecNum(getInstance()->m_pPath->getPathVectorNum(m_iActivePath) - 1);
        else setActivePathVecNum(m_iActivePathNum);
    }
    static void incActivePathVecNum(BOOL bRpt) {
        getInstance();
        setActivePathVecNum(m_iActivePathNum + 1);
        if (bRpt) limitRepeatActivePathVecNum();
        else limitClampActivePathVecNum();
    }
    static void decActivePathVecNum(BOOL bRpt) {
        getInstance();
        setActivePathVecNum(m_iActivePathNum - 1);
        if (bRpt) limitRepeatActivePathVecNum();
        else limitClampActivePathVecNum();
    }
    static void getActiveParam() { getInstance()->m_pPath->getPathParam(m_iActivePath, m_sParam); }
    static void setActiveParam() { getInstance()->m_pPath->setPathParam(m_iActivePath, m_sParam); }
    static void removePath() {
        if (getInstance()->m_pPath) delete getInstance()->m_pPath;
        getInstance()->m_pPath = NULL;
    }
    static BOOL entryPath(const CHAR* pName);
    static BOOL newPath(const CHAR* pName);
    static BOOL savePath(const CHAR* pName);
    static BOOL loadPath(const CHAR* pName);
    static void entryCam();
    static CNakCamera* getCam();
    static void removeCam();

private:
    BOOL isEnd();
    static CDebugPathData* getInstance() {
#line 259 DEBUGPATHDATA_H_PATH
        KH_DEBUG(if (!(m_pInstance)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);)
        return m_pInstance;
    }
};
#endif /* KH_RETAIL */
#endif /* GAME_DEBUG_DEBUGPATHDATA_H */
