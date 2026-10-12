// PARTIAL: added for System/Movie/Movie
#ifndef SYSTEM_MOVIE_MSVIDEO_H
#define SYSTEM_MOVIE_MSVIDEO_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/PadHandler.h"

enum MOVIE_STATUS {
    MOVIE_STATUS_NOP = 0,
    MOVIE_STATUS_OPENED = 1,
    MOVIE_STATUS_PRE_PLAY = 2,
    MOVIE_STATUS_PLAYING = 3,
    MOVIE_STATUS_RESUME = 4,
    MOVIE_STATUS_SKIPOUT = 5,
    MOVIE_STATUS_PRE_END = 6,
    MOVIE_STATUS_END = 7,
    MOVIE_STATUS_END_STILL = 8,
    MOVIE_STATUS_END_SKIP = 9
};

class CMoviePlayerBase : public CTask, public CPadHandler {
public:
    CMoviePlayerBase(CTask* pParent);
    virtual ~CMoviePlayerBase();
    virtual BOOL drawMovie();
    virtual BOOL open(const CHAR* pszFile);
    virtual void start();
    virtual MOVIE_STATUS getStatus();
    virtual INT getNowFrame();
    virtual void setMode(UINT nFlags);
};

class CMoviePlayer : public CMoviePlayerBase {
private:
    BYTE _pad[0xF0 - 0x50];
public:
    CMoviePlayer(CTask* pParent, BOOL bForDebug);
};

class CMoviePlayerDummy : public CMoviePlayerBase {
private:
    BYTE _pad[0x160 - 0x50];
public:
    CMoviePlayerDummy(CTask* pParent);
};

#endif // SYSTEM_MOVIE_MSVIDEO_H
