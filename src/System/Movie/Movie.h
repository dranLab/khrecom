#ifndef SYSTEM_MOVIE_MOVIE_H
#define SYSTEM_MOVIE_MOVIE_H

#include "khtypes.h"
#include "System/Movie/MSVideo.h"

class CMovie {
public:
    static void initialize();
    static void finalize();
    static void setupScript(const CHAR* pszFile);
    static BOOL isCompleteSetupScript();
    static void disposeScriptRsrc();
    static BOOL open(const CHAR* pszFile);
    static BOOL openDbg(const CHAR* pszFile);
    static BOOL play(UINT nFlags);
    static void close();
    static void draw();
    static MOVIE_STATUS getStatus();
    static INT getNowFrame();
    static BOOL isTrayOpened();
};

extern INT NMSVIDEO_NUMBER_OF_HSYNC;
extern INT NMSVIDEO_FILLBUFFMAXTIME;
extern INT NMSVIDEO_MAX_WIDTH;
extern INT NMSVIDEO_MAX_HEIGHT;
extern INT NMSVIDEO_MAX_MBX;
extern INT NMSVIDEO_MAX_MBY;
extern INT NMSVIDEO_IOP_ADDR_FOR_AUDIO_ZEROBUF;
extern INT NMSVIDEO_IOP_ADDR_FOR_AUDIO_DECODE;

#endif // SYSTEM_MOVIE_MOVIE_H
