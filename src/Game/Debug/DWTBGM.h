// PARTIAL: added for Form/DebugWindowTBL. The BGM test window; included inside namespace DWT by
// Form/DebugWindowTBL (its allocation messages carry this header's name and line numbers).
#ifndef GAME_DEBUG_DWTBGM_H
#define GAME_DEBUG_DWTBGM_H
} /* MATCH: namespace DWT closed around RsrcData.h: its classes stay global, its string comes here as in the original */
#include "System/RsrcData.h"
namespace DWT {
static DEBUG_WINDOW_LINE s_debugWinBGM_nothing_lines[4] = {
    {"\xf9S--------------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {" \xf9TBGM \x82\xcd\x93o\x98^\x82\xb3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb9\x82\xf1", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\xf9S--------------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL s_debugWinBGM_nothing = {"BGM \x90\xdd\x92\xe8", -80, -160, 13, 3, 12, 0, 0, s_debugWinBGM_nothing_lines, NULL, NULL};

static DEBUG_WINDOW_LINE* s_debugWinBGM_lines = NULL;

static BOOL _CallBGM(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x20) {
        HANDLE hBGM = CSound::playBGM(s_debugWinBGM_lines[inY].szString, -1, 0);
        TRACE("Call BGM %s - <%d>\n", s_debugWinBGM_lines[inY].szString, hBGM);
    }
    return TRUE;
}

static BOOL _StopBGM(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x20) {
        CSound::stopBGM(0);
        TRACE("Stop BGM\n");
    }
    return TRUE;
}

class CDebugBGMChanger : public CTask {
private:
    static CDebugBGMChanger* _pInstance;
    CDebugBGMChanger(CTask* p) : CTask(p, 0x4000) {}

public:
    virtual ~CDebugBGMChanger() {
        _pInstance = NULL;
        DEBUG_WINDOW_LINE*& rLines = s_debugWinBGM_lines;
        if (rLines) {
            com_free(rLines);
        }
        rLines = NULL;
    }
    static void create(CTask* p) {
        if (!_pInstance) {
#line 79 "Game/Debug/DWTBGM.h"
            _pInstance = new (__FILE__ "(79)") CDebugBGMChanger(p);
        }
    }
    static void destroy() {
        if (_pInstance) {
            _pInstance->out();
            _pInstance = NULL;
        }
    }
};

CDebugBGMChanger* CDebugBGMChanger::_pInstance = NULL;

static DEBUG_WINDOW_TBL s_debugWinBGM = s_debugWinBGM_nothing;

static BOOL DebugBGMchangeCB(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x20) {
        if (!CRsrcDB::getFirstRsrc(0x35)) {
            pObj->sendMessage(1, &s_debugWinBGM_nothing);
        } else {
        CDebugBGMChanger::create(pObj);
        if (s_debugWinBGM_lines) {
            com_free(s_debugWinBGM_lines);
        }
        s_debugWinBGM_lines = NULL;
        INT nCount = 0;
        CRsrcDB::initSeekRsrc(0x35, -1);
        while (CRsrcDB::seekNextRsrc()) {
            nCount++;
        }
#line 85 "Game/Debug/DWTBGM.h"
        s_debugWinBGM_lines = new (__FILE__ "(85)") DEBUG_WINDOW_LINE[nCount + 2];
        s_debugWinBGM_lines[0] = (DEBUG_WINDOW_LINE){"BGM \x83X\x83g\x83" "b\x83v", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, _StopBGM, NULL};
        if (nCount > 0) {
            CRsrcData* pRsrc;
            CRsrcDB::initSeekRsrc(0x35, -1);
            for (INT i = 1; (pRsrc = CRsrcDB::seekNextRsrc()) && i <= nCount; i++) {
                s_debugWinBGM_lines[i] = (DEBUG_WINDOW_LINE){pRsrc->getName(), &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, _CallBGM, NULL};
            }
        }
        s_debugWinBGM_lines[nCount + 1] = (DEBUG_WINDOW_LINE){NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL};
        DEBUG_WINDOW_LINE*& rLines = s_debugWinBGM_lines;
        s_debugWinBGM.iWh = nCount + 1;
        s_debugWinBGM.pLineTbl = rLines;
        pObj->sendMessage(1, &s_debugWinBGM);
        }
    } else if (inParam & 0x800) {
        CDebugBGMChanger::destroy();
    }
    return TRUE;
}

static BOOL DebugBGMToggleCB(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x10) {
        CSound::setBGMVolume(-1, DebugWork::g_dw.sys.m_iBGM ? 0 : -1, 1);
    }
    return TRUE;
}

#endif /* GAME_DEBUG_DWTBGM_H */
