#ifndef SYSTEM_MOVIE_MOVIESCRIPT_H
#define SYSTEM_MOVIE_MOVIESCRIPT_H

#include "khtypes.h"

class CTask;
class CTextWnd;

struct MovieScriptData {
    USHORT _nStartFrame;
    USHORT _nEndFrame;
    CHAR* _pszText;
};

struct MovieScriptDataHeader {
    char _cSig[4];
    USHORT _nVersion;
    USHORT _nDataCount;
    UINT _nTextTop;
    UINT __nFiller;
    MovieScriptData _scriptTbl[1];
};

struct MovieScriptUnit {
    MovieScriptData* m_pSrc;
    CTextWnd* m_pWnd;

    void open(CTask& parent, const MovieScriptData& src);
    void close();
    void clear() {
        m_pSrc = 0;
        m_pWnd = 0;
    }
    BOOL isOpened() { return m_pSrc != 0; }
    BOOL isEnd(INT nNowFrame) const;
};

class CMovieScript {
private:
    UINT m_nFlag;
    USHORT m_nScriptIdx;
    USHORT m_nScriptMax;
    MovieScriptData* m_pScriptPtr;
    CTask& m_parent;
    MovieScriptUnit m_UnitTbl[4];

public:
    CMovieScript(CTask& parent);
    ~CMovieScript();
    void open(const CHAR* pszMoviePath);
    void start();
    void update(INT nFrame);
    void terminate();
    static void setupRsrc(const CHAR* pszMoviePath);
    static BOOL isCompleteSetupRsrc();
    static BOOL isValidSrcFileID(USHORT nID) { return nID < 700; }
    static INT getMovieScriptID(const CHAR* pszFile);
    static MovieScriptDataHeader* getMovieScriptRsrc(INT nID);
    static void disposeRsrc();
    static CHAR* getFileName(const CHAR* pszPath);

private:
    void clearUnits();
    void disposeUnits();
};

#endif // SYSTEM_MOVIE_MOVIESCRIPT_H
