#ifndef SYSTEM_MOVIE_MOVIEFILES_H
#define SYSTEM_MOVIE_MOVIEFILES_H

#include "khtypes.h"
#include "System/RcfCD.h"

class CMovieFiles {
public:
    static void initialize();
    static void finalize();
    static BOOL hasFile(const CHAR* pszFileName);
    static UINT getFileCount();
    static void addFileInfo(UINT nInfoIdx);
    static BOOL getFileInfo(const CHAR* pszFileName, SectorInfo& outInfo);
    static CHAR* getFilePath(UINT nIdx);
};

#endif /* SYSTEM_MOVIE_MOVIEFILES_H */
