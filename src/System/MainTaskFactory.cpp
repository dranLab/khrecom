
#define ANIM_H_PATH "System/Graphic/anim.h" /* MATCH: this unit names Anim.h so (header default "System/Graphic/Anim.h") */
#include "System/MainTaskFactory.h"
#include "System/HeapMan.h"



#include "Form/Menu/TitleReportDllStarter.h"
#include "Game/Game.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Form/DebugMenu.h"
#include "System/Vector.h"


extern INT SCREEN_HEIGHT;

/* Placeholders for the inline functions of the headers of the debug and viewer DLLs (not in the main program):
 * their strings are in .rodata in this order. Line numbers are only known where they are part of a string. */
#line 1 "System/../Form/Menu/JiminyMemo.h"
inline void _jiminyMemoCheckId(INT inId) {
    if (!(inId < 8)) AssertApp(__FILE__, __LINE__, "inId < PARAM_NUM", 0);
}

namespace CollisionEditor {
class CCollPrim;
struct ATK_COLL_DATA;

#line 1 "../Dlls/DebugDll/CollisionEditor.h"
inline void _checkIndex8(INT inIndex) {
    if (!(0 <= inIndex && inIndex < 8)) AssertApp(__FILE__, __LINE__, "0 <= inIndex && inIndex < 8", 0);
}
inline void* _alloc528() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    return com_malloc_d(4, __FILE__ "(528)", (UINT)-1);
}

class CCollisionEditorModel {
    CVector<CCollPrim*> m_BodyCollPrimContainer;
    CVector<ATK_COLL_DATA*> m_AtkCollDataContainer;

public:
    INT getBodyCollPrimCount() { return m_BodyCollPrimContainer.size(); }
    void checkBodyCollPrimIndex(INT inIndex) {
        if (!(0 <= inIndex && inIndex < getBodyCollPrimCount()))
            AssertApp(__FILE__, __LINE__, "0 <= inIndex && inIndex < getBodyCollPrimCount()", 0);
    }
    INT getAtkCollDataCount() { return m_AtkCollDataContainer.size(); }
    void checkAtkCollDataIndex(INT inIndex) {
        if (!(0 <= inIndex && inIndex < getAtkCollDataCount()))
            AssertApp(__FILE__, __LINE__, "0 <= inIndex && inIndex < getAtkCollDataCount()", 0);
    }
    void formatNumber(INT n) {
        TRACE("%d", n);
        TRACE(" ");
    }
    CCollPrim* getBodyCollPrim(INT inIndex) { return m_BodyCollPrimContainer[inIndex]; }
    ATK_COLL_DATA* getAtkCollData(INT inIndex) { return m_AtkCollDataContainer[inIndex]; }
    void addBodyCollPrim(CCollPrim* pPrim) { m_BodyCollPrimContainer.push_back(pPrim); }
    void addAtkCollPrim(ATK_COLL_DATA* pData) { m_AtkCollDataContainer.push_back(pData); }
};
}

#line 1 "Game/OsdDef.h"
inline void _osdCheckCategory(INT inCategory) {
    if (!(0 <= inCategory && inCategory < 4))
        AssertApp(__FILE__, __LINE__, "0 <= inCategory && inCategory < OSD_CATEGORY_MAX", 0);
}

#line 1 "../Dlls/Viewer/SetEditor.h"
inline void _checkOsdVersion(INT nVersion) {
    TRACE("@OSD");
    TRACE("****************** ESD\x82\xcc\x83\x6f\x81\x5b\x83\x57\x83\x87\x83\x93\x82\xaa\x8c\xc3\x82\xa2\x82\xc5\x82\xb7 ******************\n");
    TRACE("****************** Version = %d ******************\n", nVersion);
}
inline void _checkEvbVersion(INT nVersion) {
    TRACE("@EVB");
    TRACE("****************** EVB\x82\xcc\x83\x6f\x81\x5b\x83\x57\x83\x87\x83\x93\x82\xaa\x8c\xc3\x82\xa2\x82\xc5\x82\xb7 ******************\n");
    TRACE("****************** Version = %d ******************\n", nVersion);
}
inline void* _alloc183() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    return com_malloc_d(4, __FILE__ "(183)", (UINT)-1);
}
inline void _emptyString() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}

#line 1 "../Dlls/DebugDll/AttachEditor.h"
inline void _checkEetVersion(INT nVersion) {
    TRACE("@EET");
    TRACE("****************** EET\x82\xcc\x83\x6f\x81\x5b\x83\x57\x83\x87\x83\x93\x82\xaa\x8c\xc3\x82\xa2\x82\xc5\x82\xb7 ******************\n");
    TRACE("****************** Version = %d ******************\n", nVersion);
}
inline void _checkSesVersion(INT nVersion) {
    TRACE("@SES");
    TRACE("****************** SES\x82\xcc\x83\x6f\x81\x5b\x83\x57\x83\x87\x83\x93\x82\xaa\x8c\xc3\x82\xa2\x82\xc5\x82\xb7 ******************\n");
    TRACE("****************** Version = %d ******************\n", nVersion);
}
inline void* _alloc215() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    return com_malloc_d(4, __FILE__ "(215)", (UINT)-1);
}

#line 40 "src/System/MainTaskFactory.cpp"
/* The debug and viewer tasks live in DLLs: their constructors are not linked into the main program (the
 * original calls address 0). Sizes from the allocations; class names partly guessed (from the MAINTASK_ID). */
#define DLL_TASK(name, size)                          \
    class name : public CTask {                       \
        BYTE _pad[size - sizeof(CTask)];              \
    public:                                           \
        name(CTask* pParent);                         \
    }

DLL_TASK(CKishiwadaTest, 0x660);
DLL_TASK(CKikuchiTest, 0xC0);
DLL_TASK(CKouTest, 0x140);
DLL_TASK(CGotoTest, 0x40);
DLL_TASK(CNakamichiTest, 0x45A0);
DLL_TASK(CMatsuokaTest, 0x100);
DLL_TASK(CKataokaTest, 0x150);
DLL_TASK(CKaharaTest, 0x150);
DLL_TASK(CNakashimaTest, 0x80);
namespace CollisionEditor {
DLL_TASK(CCollisionEditor, 0x33D0);
}
namespace SetEditor {
DLL_TASK(CSetEditor, 0x4940);
}
namespace AttachEditor {
DLL_TASK(CAttachEditor, 0x4230);
}
DLL_TASK(CFontViewer, 0x40);
DLL_TASK(CMapViewer, 0x60);
DLL_TASK(CModelViewer, 0x19700);
DLL_TASK(CEffectViewer, 0x351A0);
DLL_TASK(C2DViewer, 0x3F650);
DLL_TASK(CMovieTest, 0x170);

/* DLL imports: undefined here, 0 for the link (config/SLUS_217.99/dll_imports.ld): the same "jal 0" as the
 * original, and each use keeps its relocation (tools/sndll.py lists them in the DLL table). */
/* DLL imports, 0 in config/SLUS_217.99/dll_imports.ld (the loader patches their uses):
 * __14CKishiwadaTestP5CTask
 * __12CKikuchiTestP5CTask
 * __8CKouTestP5CTask
 * __9CGotoTestP5CTask
 * __14CNakamichiTestP5CTask
 * __13CMatsuokaTestP5CTask
 * __12CKataokaTestP5CTask
 * __11CKaharaTestP5CTask
 * __14CNakashimaTestP5CTask
 * __Q215CollisionEditor16CCollisionEditorP5CTask
 * __Q29SetEditor10CSetEditorP5CTask
 * __Q212AttachEditor13CAttachEditorP5CTask
 * __11CFontViewerP5CTask
 * __10CMapViewerP5CTask
 * __12CModelViewerP5CTask
 * __13CEffectViewerP5CTask
 * __9C2DViewerP5CTask
 * __10CMovieTestP5CTask
 */

namespace CollisionEditor {
static INT MENU_BASE_POX_Y = -(SCREEN_HEIGHT / 2) + 24;
}

namespace SetEditor {
static INT MENU_BASE_POX_Y = -(SCREEN_HEIGHT / 2) + 22;
}

namespace AttachEditor {
static INT MENU_BASE_POX_Y = -(SCREEN_HEIGHT / 2) + 24;
}

#line 58
CTask* CMainTaskFactory::createMainTask(MAINTASK_ID nID, CTask* pParent) {
    switch (nID) {
    case MAINTASK_ID_TITLE:
        return CTitleReportDllStarter::create(pParent, 0, 0);
    case MAINTASK_ID_TITLE_NOLOGO:
        return CTitleReportDllStarter::create(pParent, 0, 1);
#line 64
    case MAINTASK_ID_GAME:
        return new (__FILE__ "(65)") CGame(pParent);
#line 68
    case MAINTASK_ID_GAME_JF_HALLOWEEN:
        return new (__FILE__ "(68)") CGame(pParent, 7);
    case MAINTASK_ID_GAME_TGS_NORMAL:
        return new (__FILE__ "(69)") CGame(pParent, 5);
    case MAINTASK_ID_GAME_JF_VEXEN:
        return new (__FILE__ "(70)") CGame(pParent, 8);
    case MAINTASK_ID_DEBUG_GAME:
        return new (__FILE__ "(71)") CGame(pParent, 1);
    case MAINTASK_ID_DEBUG_MENU:
        return (CTask*)new (__FILE__ "(73)") CDebugMenu(pParent);
    case MAINTASK_ID_DEBUG_COLOR:
        return (CTask*)new (__FILE__ "(75)") CColorCheck(pParent);
    case MAINTASK_ID_DEBUG_SCREEN:
        return (CTask*)new (__FILE__ "(76)") CScreenCheck(pParent);
    case MAINTASK_ID_DEBUG_KISIWADA:
        return new (__FILE__ "(78)") CKishiwadaTest(pParent);
    case MAINTASK_ID_DEBUG_KIKUCHI:
        return new (__FILE__ "(79)") CKikuchiTest(pParent);
    case MAINTASK_ID_DEBUG_KOU:
        return new (__FILE__ "(80)") CKouTest(pParent);
    case MAINTASK_ID_DEBUG_GOTO:
        return new (__FILE__ "(81)") CGotoTest(pParent);
    case MAINTASK_ID_DEBUG_NAKAMICHI:
        return new (__FILE__ "(82)") CNakamichiTest(pParent);
    case MAINTASK_ID_DEBUG_MATSUOKA:
        return new (__FILE__ "(83)") CMatsuokaTest(pParent);
    case MAINTASK_ID_DEBUG_KATAOKA:
        return new (__FILE__ "(84)") CKataokaTest(pParent);
    case MAINTASK_ID_DEBUG_KAHARA:
        return new (__FILE__ "(85)") CKaharaTest(pParent);
    case MAINTASK_ID_DEBUG_NAKASHIMA:
        return new (__FILE__ "(86)") CNakashimaTest(pParent);
    case MAINTASK_ID_DEBUG_COLLISIN_EDITOR:
        return new (__FILE__ "(88)") CollisionEditor::CCollisionEditor(pParent);
    case MAINTASK_ID_DEBUG_SET_EDITOR:
        return new (__FILE__ "(89)") SetEditor::CSetEditor(pParent);
    case MAINTASK_ID_DEBUG_ATTACH_EDITOR:
        return new (__FILE__ "(91)") AttachEditor::CAttachEditor(pParent);
    case MAINTASK_ID_DEBUG_FONT_VIEWER:
        return new (__FILE__ "(92)") CFontViewer(pParent);
    case MAINTASK_ID_MAPVIEWER:
        return new (__FILE__ "(94)") CMapViewer(pParent);
    case MAINTASK_ID_MODELVIEWER:
        return new (__FILE__ "(95)") CModelViewer(pParent);
    case MAINTASK_ID_EFFECTVIEWER:
        return new (__FILE__ "(96)") CEffectViewer(pParent);
    case MAINTASK_ID_2DVIEWER:
        return new (__FILE__ "(97)") C2DViewer(pParent);
    case MAINTASK_ID_MOVIE_TEST:
        return new (__FILE__ "(98)") CMovieTest(pParent);
    default:
        TRACE("\x8a\x59\x93\x96\x82\xb7\x82\xe9\x81\x75\x83\x81\x83\x43\x83\x93TASK\x81\x76\x82\xaa\x91\xb6\x8d\xdd\x82\xb5\x82\xdc\x82\xb9\x82\xf1\x81\x69 ID = %d \x81\x6a\n", nID);
        return NULL;
    }
}

INT CMainTaskFactory::getMainTaskDllRsrcID(MAINTASK_ID nID) {
    if (nID < __MAINTASK_IN_DEBUGDLL_TOP || nID >= __MAINTASK_IN_DEBUGDLL_END) {
        if (nID >= __MAINTASK_IN_VIEWERDLL_TOP && nID < __MAINTASK_IN_VIEWERDLL_END) {
            return 9001;
        }
        return 0;
    }
    return 9000;
}

// SOURCE DATA: .rodata .bss
