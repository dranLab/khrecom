#ifndef SYSTEM_MOVIE_AUDIODEC_H
#define SYSTEM_MOVIE_AUDIODEC_H

#include "khtypes.h"

class CAudioDec {
public:
    static void create();
    static void dispose();
    static INT putData(const BYTE* pSrc, INT nSize);
    static BOOL isOK();
    static BOOL isStarted();
    static BOOL isPause();
    static void start();
    static INT sendToIOP();
    static void reset();
    static void pause();
    static void resume();
};

#endif // SYSTEM_MOVIE_AUDIODEC_H
