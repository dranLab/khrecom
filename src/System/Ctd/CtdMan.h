#ifndef SYSTEM_CTD_CTDMAN_H
#define SYSTEM_CTD_CTDMAN_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"

/* __FILE__ of this header in the original differs per includer ("System/CTD/CtdMan.h" in most, also
 * "System/Ctd/CtdMan.h" and "c:/KHCOM/MainApp/System/CTD/CtdMan.h"): define CTDMAN_H_PATH before
 * including it to pick another one. Line numbers below are the original ones (__LINE__ in messages). */
#ifndef CTDMAN_H_PATH
#define CTDMAN_H_PATH "System/CTD/CtdMan.h"
#endif

struct CtdHeader {
    char sig[4];
    USHORT nBlock;
    USHORT nText;
    UINT textAddrArea;
    UINT textDataArea;
};

struct CtdBlock {
    UINT id;
    UINT wndColor;
    UINT txtColor;
    UINT txtExColor;
    CHAR** ppText;
    USHORT nText;
    USHORT posX;
    USHORT posY;
    USHORT sizeX;
    USHORT sizeY;
    USHORT wndStyle;
    USHORT fntStyle;
    USHORT fntSize;
    USHORT fntSpaceX;
    USHORT fntSpaceY;
    USHORT txtMarginX;
    USHORT txtMarginY;
    USHORT cyobiType;
    SHORT cyobiPos;
};
class CCtdList;
class CCtdMan;


#line 20 CTDMAN_H_PATH
class CCtdMan {
public:
    static CCtdMan* m_pInstance;
private:
    static CCtdList* m_pCtdList;
public:
    CCtdMan();
    virtual ~CCtdMan();
    void setup();
    void dispose();

    static CCtdMan& initialize() {
#line 38
        if (m_pInstance != NULL) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
        m_pInstance = new KH_AT("(39)") CCtdMan;
        if (!m_pInstance) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
        m_pInstance->setup();
        return *m_pInstance;
    }
    static void finalize() {
        delete m_pInstance;
        m_pInstance = NULL;
    }
    static CCtdMan& instance() {
        /* read through a reference (Form/Menu/LevelUp): keeps the load of m_pInstance after preceding member stores */
        CCtdMan*& pInstance = m_pInstance;
#line 50
        if (!pInstance) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
        return *pInstance;
    }
    void initializeData(void* pDtHead);
    void disposeData(void* pDtHead);
    static CHAR* getText(UINT ctdID) { return instance()._getText(ctdID); }
    static CHAR* getStr(UINT ctdID) { return instance()._getText(ctdID); }
    static CtdBlock* getCtdBlock(UINT ctdID, CHAR** pOutText) { return instance()._getCtdBlock(ctdID, pOutText); }
    static INT sprintf(LPSTR inBuf, UINT inBufSize, UINT inSrcCtdID, UINT (&ctdID)[4]) {
        return instance()._sprintf(inBuf, inBufSize, inSrcCtdID, ctdID);
    }
    static INT sprintf(LPSTR inBuf, UINT inBufSize, const CHAR* inSrcStr, UINT (&ctdID)[4]) {
        return instance()._sprintf(inBuf, inBufSize, inSrcStr, ctdID);
    }
    static INT sprintf(LPSTR inBuf, UINT inBufSize, const CHAR* inSrcStr, const CHAR* (&pWord)[4], CHAR** outbuf) {
        return instance()._sprintf(inBuf, inBufSize, inSrcStr, pWord, outbuf);
    }

private:
    CHAR* _getText(UINT ctdID);
    CtdBlock* _getCtdBlock(UINT ctdID, CHAR** pOutText);
    INT _sprintf(LPSTR inBuf, UINT inBufSize, UINT inSrcCtdID, UINT (&ctdID)[4]);
    INT _sprintf(LPSTR inBuf, UINT inBufSize, const CHAR* inSrcStr, UINT (&ctdID)[4]);
    INT _sprintf(LPSTR inBuf, UINT inBufSize, const CHAR* inSrcStr, const CHAR* (&pWord)[4], CHAR** outBuf);
};

#endif /* SYSTEM_CTD_CTDMAN_H */
