
#ifndef GAME_ATEL_CHATMAN_H
#define GAME_ATEL_CHATMAN_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Task.h"
#include "Game/Atel/CommandAccepter.h"

/* Includers define the placement operator new before including this header. */

namespace ATEL {

class CChatWndDt;
class CChatWndDtList;

/* MATCH: path override: __FILE__ of this header differs per includer ("c:/KHCOM/MainApp/Game/Atel/ChatMan.h" in Game/Atel/ChatMan) */
#ifndef CHATMAN_H_PATH
#define CHATMAN_H_PATH "Game/Atel/ChatMan.h"
#endif

/* The inline members are emitted (weak) in Game/Atel/ChatMan.cpp, in this order: keep it.
 */
class CChatMan : public CTask, public ICommandAccepter {
private:
    static CChatMan* m_pInstance;
    static CChatWndDtList* m_pCChatWndDtList;
    static INT m_uniqueID;

    CChatMan();
    virtual ~CChatMan();
    void setup();
    void dispose();

public:
#line 45 CHATMAN_H_PATH
    static CChatMan& initialize() {
        KH_DEBUG(if (!(__null == m_pInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);)
        m_pInstance = new KH_AT("(47)") CChatMan;
        KH_DEBUG(if (!(m_pInstance)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);)
        m_pInstance->setup();
        return *m_pInstance;
    }
    static void finalize() {
        delete m_pInstance;
        m_pInstance = NULL;
    }
    /* MATCH: m_pInstance read through a reference (Form/Menu/WorldSelect: the load stays after stores)
     */
#line 57 CHATMAN_H_PATH
    static CChatMan& instance() { CChatMan*& instance = m_pInstance;
        KH_DEBUG(if (!(instance)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);)
        return *instance;
    }
    static BOOL isExist() { return m_pInstance != NULL; }
    static void destroyAllWindow() { instance()._destroyAllWindow(); }
    virtual INT reqCommand(INT nCmd, void* pParam, INT nID) {
        if (nCmd == 0x3E8) return onCreateChatWindow(nCmd, pParam, nID);
        if (nCmd == 0x3E9) return onCreateChatWindowC(nCmd, pParam, nID);
        if (nCmd == 0x3EA) return onOpen(nCmd, pParam, nID);
        if (nCmd == 0x3EB) return onClose(nCmd, pParam, nID);
        if (nCmd == 0x3EC) return onDestroyChatWindow(nCmd, pParam, nID);
        if (nCmd == 0x3ED) return onCheckStatus(nCmd, pParam, nID);
        if (nCmd == 0x3EE) return onClearText(nCmd, pParam, nID);
        if (nCmd == 0x3EF) return onSetText(nCmd, pParam, nID);
        if (nCmd == 0x3F0) return onSetTextDelay(nCmd, pParam, nID);
        if (nCmd == 0x3F1) return onSetWndStyle(nCmd, pParam, nID);
        if (nCmd == 0x3F2) return onChangeWndStyle(nCmd, pParam, nID);
        if (nCmd == 0x3F3) return onSetTextMargin(nCmd, pParam, nID);
        if (nCmd == 0x3F4) return onSetFontSize(nCmd, pParam, nID);
        if (nCmd == 0x3F5) return onSetFontSpace(nCmd, pParam, nID);
        if (nCmd == 0x3F6) return onSetTextStyle(nCmd, pParam, nID);
        if (nCmd == 0x3F7) return onSetTextColor(nCmd, pParam, nID);
        if (nCmd == 0x3F8) return onSetTextEXColor(nCmd, pParam, nID);
        if (nCmd == 0x3F9) return onChangeTextSelect(nCmd, pParam, nID);
        if (nCmd == 0x3FA) return onSetSystemPause(nCmd, pParam, nID);
        if (nCmd == 0x3FB) return onCreateImageWindow(nCmd, pParam, nID);
        if (nCmd == 0x3FC) return onDestroyChatWindow(nCmd, pParam, nID);
        if (nCmd == 0x3FF) return onSetWndRate(nCmd, pParam, nID);
        if (nCmd == 0x400) return onChangeText(nCmd, pParam, nID);
        if (nCmd == 0x401) return onInitPhaseImageWindow(nCmd, pParam, nID);
        if (nCmd == 0x402) return onSetSelectOffset(nCmd, pParam, nID);
        if (nCmd == 0x403) return onChangeCursorColor(nCmd, pParam, nID);
        if (nCmd == 0x404) return onSetDispInterval(nCmd, pParam, nID);
        if (nCmd == 0x405) return onCreateChatWindowCTD(nCmd, pParam, nID);
        if (nCmd == 0x406) return onChangeTextNext(nCmd, pParam, nID);
        if (nCmd == 0x407) return onEnableDisp(nCmd, pParam, nID);
        if (nCmd == 0x408) return onSetWords(nCmd, pParam, nID);
        if (nCmd == 0x409) return onSetTextCTD(nCmd, pParam, nID);
        if (nCmd == 0x40A) return onChangeTextCTD(nCmd, pParam, nID);
        if (nCmd == 0x40B) return onEnableDisp0(nCmd, pParam, nID);
        if (nCmd == 0x40C) return onOpenDelay(nCmd, pParam, nID);
        if (nCmd == 0x40E) return onCheckPhaseCnt(nCmd, pParam, nID);
        if (nCmd == 0x40F) return onDestroyChatWindowAll(nCmd, pParam, nID);
        if (nCmd == 0x410) return onSetLayerTutorial(nCmd, pParam, nID);
        if (nCmd == 0x411) return onSetCharFadeInDelay(nCmd, pParam, nID);
        if (nCmd == 0x412) return onSetTextFadeOutTime(nCmd, pParam, nID);
        if (nCmd == 0x413) return onEnableLFDisp(nCmd, pParam, nID);
        return 0;
    }

private:
    CChatWndDt* searchChatWndDt(INT nID);
    void _destroyAllWindow();
    INT createChatWindow(INT nCmd, void* pParam, INT nID, INT fg);
    INT onCreateChatWindow(INT nCmd, void* pParam, INT nID);
    INT onCreateChatWindowC(INT nCmd, void* pParam, INT nID);
    INT onDestroyChatWindow(INT nCmd, void* pParam, INT nID);
    INT onOpen(INT nCmd, void* pParam, INT nID);
    INT onClose(INT nCmd, void* pParam, INT nID);
    INT onCheckStatus(INT nCmd, void* pParam, INT nID);
    INT onClearText(INT nCmd, void* pParam, INT nID);
    INT onSetText(INT nCmd, void* pParam, INT nID);
    INT onSetTextDelay(INT nCmd, void* pParam, INT nID);
    INT onSetWndStyle(INT nCmd, void* pParam, INT nID);
    INT onChangeWndStyle(INT nCmd, void* pParam, INT nID);
    INT onSetTextMargin(INT nCmd, void* pParam, INT nID);
    INT onSetFontSize(INT nCmd, void* pParam, INT nID);
    INT onSetFontSpace(INT nCmd, void* pParam, INT nID);
    INT onSetTextStyle(INT nCmd, void* pParam, INT nID);
    INT onSetTextColor(INT nCmd, void* pParam, INT nID);
    INT onSetTextEXColor(INT nCmd, void* pParam, INT nID);
    INT onChangeTextSelect(INT nCmd, void* pParam, INT nID);
    INT onSetSystemPause(INT nCmd, void* pParam, INT nID);
    INT onCreateImageWindow(INT nCmd, void* pParam, INT nID);
    INT onSetWndRate(INT nCmd, void* pParam, INT nID);
    INT onChangeText(INT nCmd, void* pParam, INT nID);
    INT onInitPhaseImageWindow(INT nCmd, void* pParam, INT nID);
    INT onSetSelectOffset(INT nCmd, void* pParam, INT nID);
    INT onChangeCursorColor(INT nCmd, void* pParam, INT nID);
    INT onSetDispInterval(INT nCmd, void* pParam, INT nID);
    INT onCreateChatWindowCTD(INT nCmd, void* pParam, INT nID);
    INT onChangeTextNext(INT nCmd, void* pParam, INT nID);
    INT onEnableDisp(INT nCmd, void* pParam, INT nID);
    INT onSetWords(INT nCmd, void* pParam, INT nID);
    INT onSetTextCTD(INT nCmd, void* pParam, INT nID);
    INT onChangeTextCTD(INT nCmd, void* pParam, INT nID);
    INT onEnableDisp0(INT nCmd, void* pParam, INT nID);
    INT onOpenDelay(INT nCmd, void* pParam, INT nID);
    INT onCheckPhaseCnt(INT nCmd, void* pParam, INT nID);
    INT onDestroyChatWindowAll(INT nCmd, void* pParam, INT nID);
    INT onSetLayerTutorial(INT nCmd, void* pParam, INT nID);
    INT onSetCharFadeInDelay(INT nCmd, void* pParam, INT nID);
    INT onSetTextFadeOutTime(INT nCmd, void* pParam, INT nID);
    INT onEnableLFDisp(INT nCmd, void* pParam, INT nID);
};

} // namespace ATEL

#endif /* GAME_ATEL_CHATMAN_H */
