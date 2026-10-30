#define GPRIM_H_PATH "System/Graphic/gprim.h"
#include "System/Graphic/Gprim.h"
#include "System/Movie/MovieScript.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Debug.h"
#include <string.h>

extern "C" {
int atoi(const char* s);
int sprintf(char* buf, const char* fmt, ...);
int strncasecmp(const char* s1, const char* s2, size_t n);
}

inline void* operator new(size_t, void* p) { return p; }







class CRsrcData;

#include "System/RsrcDB.h"






#include "System/Rcf.h"





template <class T> struct CPointT {
    T m_x;
    T m_y;
    CPointT(T x, T y) { m_x = x; m_y = y; }
};
template <class T> struct CSizeT {
    T m_w;
    T m_h;
    CSizeT(T w, T h) { m_w = w; m_h = h; }
    void setSize(T w, T h) { m_w = w; m_h = h; }
};
typedef CPointT<int> CPointI;
typedef CSizeT<int> CSizeI;

class CTextWnd { /* PARTIAL: stand-in for System/Wnd CTextWnd: the assembler step crashes with Wnd.h here (cause unknown) */
public:
    CTextWnd(CTask& parent, INT nID);
    void createWindow(CPointI& pos, CSizeI& size, UINT nAttr, const char* pszText, UINT nCol, UINT nFontSize,
                      UINT nFontStyle, CPointI& cpos, UINT n0, UINT n1);
    void setFontSpace(CSizeI size);
    void open();
    void closeDestroy();
    void setWndStyle(UINT nStyle);
private:
    char _pad[0x140];
};

static UINT s_nMovieScriptRsrcID;

CMovieScript::CMovieScript(CTask& parent) : m_parent(parent) {
    m_nFlag = 0;
    m_nScriptIdx = 0;
    m_nScriptMax = 0;
    m_pScriptPtr = 0;
}

CMovieScript::~CMovieScript() {
    terminate();
}

void CMovieScript::setupRsrc(const CHAR* pszMoviePath) {
    s_nMovieScriptRsrcID = 0;
    if (pszMoviePath) {
        INT nID = getMovieScriptID(getFileName(pszMoviePath));
        if (!getMovieScriptRsrc(nID) && isValidSrcFileID(nID)) {
            s_nMovieScriptRsrcID = nID;
            CRcf::reqLoadRCF(2, nID + 5000, 0, 0, 0);
        }
    }
}

BOOL CMovieScript::isCompleteSetupRsrc() {
    BOOL bRes;
    MovieScriptDataHeader* pHeader;
    MovieScriptData* pData;
    INT i;
    if (!isValidSrcFileID(s_nMovieScriptRsrcID)) return TRUE;
    bRes = CRcf::isCompleteLoadRCF(2, s_nMovieScriptRsrcID + 5000);
    if (bRes) {
        pHeader = getMovieScriptRsrc(s_nMovieScriptRsrcID);
        if (pHeader) {
            pData = pHeader->_scriptTbl;
            for (i = 0; i < pHeader->_nDataCount; i++, pData++) {
                if ((UINT)pData->_pszText <= 0xFFFFF) {
                    pData->_pszText = (CHAR*)((UINT)pData->_pszText + (UINT)pHeader);
                }
            }
        }
    }
    return bRes;
}

void CMovieScript::disposeRsrc() {
    if (isValidSrcFileID(s_nMovieScriptRsrcID)) {
        CRsrcDB::removeGroup(2, s_nMovieScriptRsrcID + 5000);
    }
    s_nMovieScriptRsrcID = 0;
}

INT CMovieScript::getMovieScriptID(const CHAR* pszFile) {
    INT nID = -1;
    if (pszFile && *pszFile) {
        if (strncasecmp("COP", pszFile, 3) == 0) {
            nID = 0;
        } else {
            nID = atoi(pszFile);
            if (nID == 0) nID = -1;
        }
    }
    return nID;
}

MovieScriptDataHeader* CMovieScript::getMovieScriptRsrc(INT nID) {
    char szRsrc[32];
    CRsrcData* pData;
    if (!isValidSrcFileID(nID)) return 0;
    sprintf(szRsrc, "MVS%03d", nID);
    pData = CRsrcDB::getRsrc(1, szRsrc, NAME_CASE_SENSITIVE);
    return pData ? (MovieScriptDataHeader*)CRsrcDB::getRsrcData(*pData) : 0;
}

void CMovieScript::open(const CHAR* pszMoviePath) {
    MovieScriptDataHeader* pHeader;
    m_nFlag = 0;
    m_pScriptPtr = 0;
    clearUnits();
    if (pszMoviePath) {
        pHeader = getMovieScriptRsrc(getMovieScriptID(getFileName(pszMoviePath)));
        if (pHeader) {
            m_nScriptIdx = 0;
            m_pScriptPtr = pHeader->_scriptTbl;
            m_nScriptMax = pHeader->_nDataCount;
        }
    }
}

void CMovieScript::start() {
    m_nFlag = 1;
}

void CMovieScript::update(INT nFrame) {
    INT i;
    if (m_pScriptPtr && (m_nFlag & 1)) {
        for (i = 0; i < 4; i++) {
            if (m_UnitTbl[i].isEnd(nFrame)) m_UnitTbl[i].close();
        }
        while (m_nScriptMax > m_nScriptIdx && nFrame >= m_pScriptPtr->_nStartFrame) {
            MovieScriptUnit* pUnit = 0;
            for (INT i = 0; i < 4; i++) {
                if (!m_UnitTbl[i].isOpened()) {
                    pUnit = &m_UnitTbl[i];
                    break;
                }
            }
            if (!pUnit) {
                TRACE("\x83\x5a\x83\x8a\x83\x74\x97\x70\x83\x8f\x81\x5b\x83\x4e\x82\xaa\x82\xa2\x82\xc1\x82\xcf\x82\xa2"
                      "\x82\xc5\x8a\x4a\x8e\x6e\x82\xc5\x82\xab\x82\xdc\x82\xb9\x82\xf1\n");
                return;
            }
            pUnit->open(m_parent, *m_pScriptPtr);
            m_pScriptPtr++;
            m_nScriptIdx++;
        }
    }
}

void CMovieScript::terminate() {
    m_pScriptPtr = 0;
    m_nFlag = 0;
    disposeUnits();
}

void CMovieScript::clearUnits() {
    INT i;
    for (i = 0; i < 4; i++) {
        m_UnitTbl[i].clear();
    }
}

void CMovieScript::disposeUnits() {
    INT i;
    for (i = 0; i < 4; i++) {
        m_UnitTbl[i].close();
    }
}

CHAR* CMovieScript::getFileName(const CHAR* pszPath) {
    CHAR* pszFile;
    pszFile = 0;
    if (pszPath) {
        pszFile = strrchr(pszPath, '\\');
        if (pszFile) return pszFile + 1;
        pszFile = strrchr(pszPath, '/');
        if (pszFile) return pszFile + 1;
        pszFile = (CHAR*)pszPath;
    }
    return pszFile;
}

void MovieScriptUnit::open(CTask& parent, const MovieScriptData& src) {
    char* msg = __FILE__ "(268)";
    m_pSrc = (MovieScriptData*)&src;
    UINT nRA;
    __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA));
    CTextWnd* p = (CTextWnd*)com_malloc_d(sizeof(CTextWnd), msg, nRA);
    if (p != 0) p = new (p) CTextWnd(parent, 0);
    m_pWnd = p;
#line 269
    if (!m_pWnd) AssertApp(__FILE__, __LINE__, "m_pWnd", 0);
    m_pWnd->setWndStyle(5);
    CSizeI size(0, 2);
    m_pWnd->setFontSpace(size);
    CPointI pos(0x30, 0x128);
    CPointI cpos(0, 0);
    size.setSize(0x1A0, 0x2E);
    m_pWnd->createWindow(pos, size, 0, src._pszText, 0x80808080, 0x16, 2, cpos, 0, 0x300B);
    m_pWnd->open();
}

void MovieScriptUnit::close() {
    if (m_pWnd) {
        m_pWnd->closeDestroy();
        m_pWnd = 0;
    } else {
        m_pWnd = 0;
    }
    m_pSrc = 0;
}

BOOL MovieScriptUnit::isEnd(INT nNowFrame) const {
    BOOL bRes = FALSE;
    if (m_pSrc) bRes = nNowFrame >= m_pSrc->_nEndFrame;
    return bRes;
}
// SOURCE DATA: .rodata .bss
