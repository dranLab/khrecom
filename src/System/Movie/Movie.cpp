#include "System/Movie/Movie.h"
#include "System/TaskMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Movie/MovieFiles.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Debug.h"



#include "System/Movie/MovieScript.h"






extern "C" int TsSdMpegGetAddrIOP();
extern "C" int sceGsSyncPath(int mode, u_short timeout);

namespace {
HANDLE s_hVideoTask = 0;
}
INT NMSVIDEO_NUMBER_OF_HSYNC;
INT NMSVIDEO_FILLBUFFMAXTIME;
INT NMSVIDEO_MAX_WIDTH;
INT NMSVIDEO_MAX_HEIGHT;
INT NMSVIDEO_MAX_MBX;
INT NMSVIDEO_MAX_MBY;
INT NMSVIDEO_IOP_ADDR_FOR_AUDIO_ZEROBUF;
INT NMSVIDEO_IOP_ADDR_FOR_AUDIO_DECODE;
namespace {
BOOL s_bFpsBackup;
}

void CMovie::initialize() {
    if (CMainApp::isPAL()) {
        NMSVIDEO_NUMBER_OF_HSYNC = 625;
        NMSVIDEO_MAX_WIDTH = 512;
        NMSVIDEO_MAX_HEIGHT = 384;
    } else {
        NMSVIDEO_NUMBER_OF_HSYNC = 525;
        NMSVIDEO_MAX_WIDTH = 512;
        NMSVIDEO_MAX_HEIGHT = 384;
    }
    NMSVIDEO_FILLBUFFMAXTIME = NMSVIDEO_NUMBER_OF_HSYNC - 200;
    NMSVIDEO_MAX_MBX = NMSVIDEO_MAX_WIDTH / 16;
    NMSVIDEO_MAX_MBY = NMSVIDEO_MAX_HEIGHT / 16;
    NMSVIDEO_IOP_ADDR_FOR_AUDIO_ZEROBUF = TsSdMpegGetAddrIOP();
    NMSVIDEO_IOP_ADDR_FOR_AUDIO_DECODE = NMSVIDEO_IOP_ADDR_FOR_AUDIO_ZEROBUF + 0x800;
    s_hVideoTask = 0;
}

void CMovie::finalize() {
    s_hVideoTask = 0;
}

void CMovie::setupScript(const CHAR* pszFile) {
    CMovieScript::setupRsrc(pszFile);
}

BOOL CMovie::isCompleteSetupScript() {
    return CMovieScript::isCompleteSetupRsrc();
}

void CMovie::disposeScriptRsrc() {
    CMovieScript::disposeRsrc();
}










#line 136
static BOOL _openMovie(const CHAR* pszFile KH_DEBUG(, BOOL bForDebug)) {
    KH_DEBUG(if (s_hVideoTask) {)
        KH_DEBUG(if (CTaskMan::getTask(s_hVideoTask)) {)
            KH_DEBUG(TRACE("\x82\xb7\x82\xc5\x82\xc9\x8a\x4a\x82\xa2\x82\xc4\x82\xa2\x82\xe9\x83\x80\x81\x5b\x83\x72\x81\x5b\x83\x74\x83\x40\x83\x43\x83\x8b\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb7\n");)
            KH_DEBUG(return TRUE;)
        KH_DEBUG(})
        KH_DEBUG(s_hVideoTask = 0;)
    KH_DEBUG(})
    BOOL bFileOK = CMovieFiles::hasFile(pszFile);
    CTask* pParent = CTaskMan::getSysTask(14);
    CMoviePlayerBase* p;
    KH_DEBUG(if (bFileOK) {)
        p = new KH_AT("(148)") CMoviePlayer(pParent KH_DEBUG(, bForDebug));
    KH_DEBUG(} else {)
        KH_DEBUG(p = new KH_AT("(148)") CMoviePlayerDummy(pParent);)
    KH_DEBUG(})
    KH_DEBUG(if (!p) AssertApp(__FILE__, __LINE__, "p", 0);)
    /* volatile: keeps the store ahead of the vtable load (the original schedules it there) */
    *(volatile HANDLE*)&s_hVideoTask = p->getHandle();
    BOOL bResult = p->open(pszFile);
    if (!bResult) {
        TRACE("\x83\x80\x81\x5b\x83\x72\x81\x5b\x83\x74\x83\x40\x83\x43\x83\x8b\x81\x69%s\x81\x6a\x82\xcc\x83\x49\x81\x5b\x83\x76\x83\x93\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n", pszFile);
        p->out();
        s_hVideoTask = 0;
    }
    return bResult;
}

BOOL CMovie::open(const CHAR* pszFile) {
    return _openMovie(pszFile KH_DEBUG(, FALSE));
}

KH_DEBUG(BOOL CMovie::openDbg(const CHAR* pszFile) {)
    KH_DEBUG(return _openMovie(pszFile, TRUE);)
KH_DEBUG(})

BOOL CMovie::play(UINT nFlags) {
    if (s_hVideoTask) {
        s_bFpsBackup = CMainApp::isFpsHalf();
        CMainApp::setFpsHalf();
        CMoviePlayerBase* p = (CMoviePlayerBase*)CTaskMan::getTask(s_hVideoTask);
        if (p && p->getStatus() == MOVIE_STATUS_OPENED) {
            p->setMode(nFlags);
            p->start();
            return TRUE;
        }
    }
    return FALSE;
}

void CMovie::close() {
    if (s_hVideoTask) {
        CTask* p = CTaskMan::getTask(s_hVideoTask);
        if (p) {
            p->out();
        }
        s_hVideoTask = 0;
        if (!s_bFpsBackup) {
            CMainApp::setFpsFull();
        }
    }
}

MOVIE_STATUS CMovie::getStatus() {
    if (s_hVideoTask) {
        CMoviePlayerBase* p = (CMoviePlayerBase*)CTaskMan::getTask(s_hVideoTask);
        if (p) {
            MOVIE_STATUS s = p->getStatus();
            if (s == MOVIE_STATUS_END_STILL) s = MOVIE_STATUS_END;
            return s;
        }
        s_hVideoTask = 0;
    }
    return MOVIE_STATUS_NOP;
}

INT CMovie::getNowFrame() {
    if (s_hVideoTask) {
        CMoviePlayerBase* p = (CMoviePlayerBase*)CTaskMan::getTask(s_hVideoTask);
        if (p) {
            return p->getNowFrame();
        }
    }
    return -1;
}

void CMovie::draw() {
    CMoviePlayerBase* p = (CMoviePlayerBase*)CTaskMan::getTask(s_hVideoTask);
    if (p) {
        if (p->drawMovie()) {
            sceGsSyncPath(0, 0);
        }
    }
}
// SOURCE DATA: .data .rodata .bss
// SOURCE DATA SLUS_217.99_retail: .bss .data
