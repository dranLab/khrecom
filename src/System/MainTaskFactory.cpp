
#define ANIM_H_PATH "System/Graphic/anim.h" /* MATCH: this unit names Anim.h so (header default "System/Graphic/Anim.h") */
#include "System/MainTaskFactory.h"
#include "System/HeapMan.h"

inline void* operator new(size_t, void* p) throw() { return p; }

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

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

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

/* Ps2EeAs makes undefined weak symbols global (the link would fail), so define them as weak absolute 0: the
 * same "jal 0" as the original (whose relocation names func_0, hence funcdiff's "differs"). */
__asm__(
    ".weak __14CKishiwadaTestP5CTask\n__14CKishiwadaTestP5CTask = 0\n"
    ".weak __12CKikuchiTestP5CTask\n__12CKikuchiTestP5CTask = 0\n"
    ".weak __8CKouTestP5CTask\n__8CKouTestP5CTask = 0\n"
    ".weak __9CGotoTestP5CTask\n__9CGotoTestP5CTask = 0\n"
    ".weak __14CNakamichiTestP5CTask\n__14CNakamichiTestP5CTask = 0\n"
    ".weak __13CMatsuokaTestP5CTask\n__13CMatsuokaTestP5CTask = 0\n"
    ".weak __12CKataokaTestP5CTask\n__12CKataokaTestP5CTask = 0\n"
    ".weak __11CKaharaTestP5CTask\n__11CKaharaTestP5CTask = 0\n"
    ".weak __14CNakashimaTestP5CTask\n__14CNakashimaTestP5CTask = 0\n"
    ".weak __Q215CollisionEditor16CCollisionEditorP5CTask\n__Q215CollisionEditor16CCollisionEditorP5CTask = 0\n"
    ".weak __Q29SetEditor10CSetEditorP5CTask\n__Q29SetEditor10CSetEditorP5CTask = 0\n"
    ".weak __Q212AttachEditor13CAttachEditorP5CTask\n__Q212AttachEditor13CAttachEditorP5CTask = 0\n"
    ".weak __11CFontViewerP5CTask\n__11CFontViewerP5CTask = 0\n"
    ".weak __10CMapViewerP5CTask\n__10CMapViewerP5CTask = 0\n"
    ".weak __12CModelViewerP5CTask\n__12CModelViewerP5CTask = 0\n"
    ".weak __13CEffectViewerP5CTask\n__13CEffectViewerP5CTask = 0\n"
    ".weak __9C2DViewerP5CTask\n__9C2DViewerP5CTask = 0\n"
    ".weak __10CMovieTestP5CTask\n__10CMovieTestP5CTask = 0\n"
);

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
        return new (com_malloc_d(0x40, __FILE__ "(65)", RET_ADDR())) CGame(pParent);
#line 68
    case MAINTASK_ID_GAME_JF_HALLOWEEN:
        return new (com_malloc_d(0x40, __FILE__ "(68)", RET_ADDR())) CGame(pParent, 7);
    case MAINTASK_ID_GAME_TGS_NORMAL:
        return new (com_malloc_d(0x40, __FILE__ "(69)", RET_ADDR())) CGame(pParent, 5);
    case MAINTASK_ID_GAME_JF_VEXEN:
        return new (com_malloc_d(0x40, __FILE__ "(70)", RET_ADDR())) CGame(pParent, 8);
    case MAINTASK_ID_DEBUG_GAME:
        return new (com_malloc_d(0x40, __FILE__ "(71)", RET_ADDR())) CGame(pParent, 1);
    case MAINTASK_ID_DEBUG_MENU:
        return (CTask*)new (com_malloc_d(0x50, __FILE__ "(73)", RET_ADDR())) CDebugMenu(pParent);
    case MAINTASK_ID_DEBUG_COLOR:
        return (CTask*)new (com_malloc_d(0x70, __FILE__ "(75)", RET_ADDR())) CColorCheck(pParent);
    case MAINTASK_ID_DEBUG_SCREEN:
        return (CTask*)new (com_malloc_d(0xD0, __FILE__ "(76)", RET_ADDR())) CScreenCheck(pParent);
    case MAINTASK_ID_DEBUG_KISIWADA:
        return new (com_malloc_d(sizeof(CKishiwadaTest), __FILE__ "(78)", RET_ADDR())) CKishiwadaTest(pParent);
    case MAINTASK_ID_DEBUG_KIKUCHI:
        return new (com_malloc_d(sizeof(CKikuchiTest), __FILE__ "(79)", RET_ADDR())) CKikuchiTest(pParent);
    case MAINTASK_ID_DEBUG_KOU:
        return new (com_malloc_d(sizeof(CKouTest), __FILE__ "(80)", RET_ADDR())) CKouTest(pParent);
    case MAINTASK_ID_DEBUG_GOTO:
        return new (com_malloc_d(sizeof(CGotoTest), __FILE__ "(81)", RET_ADDR())) CGotoTest(pParent);
    case MAINTASK_ID_DEBUG_NAKAMICHI:
        return new (com_malloc_d(sizeof(CNakamichiTest), __FILE__ "(82)", RET_ADDR())) CNakamichiTest(pParent);
    case MAINTASK_ID_DEBUG_MATSUOKA:
        return new (com_malloc_d(sizeof(CMatsuokaTest), __FILE__ "(83)", RET_ADDR())) CMatsuokaTest(pParent);
    case MAINTASK_ID_DEBUG_KATAOKA:
        return new (com_malloc_d(sizeof(CKataokaTest), __FILE__ "(84)", RET_ADDR())) CKataokaTest(pParent);
    case MAINTASK_ID_DEBUG_KAHARA:
        return new (com_malloc_d(sizeof(CKaharaTest), __FILE__ "(85)", RET_ADDR())) CKaharaTest(pParent);
    case MAINTASK_ID_DEBUG_NAKASHIMA:
        return new (com_malloc_d(sizeof(CNakashimaTest), __FILE__ "(86)", RET_ADDR())) CNakashimaTest(pParent);
    case MAINTASK_ID_DEBUG_COLLISIN_EDITOR:
        return new (com_malloc_d(sizeof(CollisionEditor::CCollisionEditor), __FILE__ "(88)", RET_ADDR())) CollisionEditor::CCollisionEditor(pParent);
    case MAINTASK_ID_DEBUG_SET_EDITOR:
        return new (com_malloc_d(sizeof(SetEditor::CSetEditor), __FILE__ "(89)", RET_ADDR())) SetEditor::CSetEditor(pParent);
    case MAINTASK_ID_DEBUG_ATTACH_EDITOR:
        return new (com_malloc_d(sizeof(AttachEditor::CAttachEditor), __FILE__ "(91)", RET_ADDR())) AttachEditor::CAttachEditor(pParent);
    case MAINTASK_ID_DEBUG_FONT_VIEWER:
        return new (com_malloc_d(sizeof(CFontViewer), __FILE__ "(92)", RET_ADDR())) CFontViewer(pParent);
    case MAINTASK_ID_MAPVIEWER:
        return new (com_malloc_d(sizeof(CMapViewer), __FILE__ "(94)", RET_ADDR())) CMapViewer(pParent);
    case MAINTASK_ID_MODELVIEWER:
        return new (com_malloc_d(sizeof(CModelViewer), __FILE__ "(95)", RET_ADDR())) CModelViewer(pParent);
    case MAINTASK_ID_EFFECTVIEWER:
        return new (com_malloc_d(sizeof(CEffectViewer), __FILE__ "(96)", RET_ADDR())) CEffectViewer(pParent);
    case MAINTASK_ID_2DVIEWER:
        return new (com_malloc_d(sizeof(C2DViewer), __FILE__ "(97)", RET_ADDR())) C2DViewer(pParent);
    case MAINTASK_ID_MOVIE_TEST:
        return new (com_malloc_d(sizeof(CMovieTest), __FILE__ "(98)", RET_ADDR())) CMovieTest(pParent);
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
