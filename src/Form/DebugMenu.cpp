#include "System/Task.h"
#include "System/PadHandler.h"
#define FORM_DEBUGMENU_FULL
#include "Form/DebugMenu.h"
#include "System/Graphic/Gprim.h"
#include "System/DebugProcInfo.h"
#include "System/HeapMan.h"
#include "System/MainApp.h"
#include "System/SJoint.h"
#include "System/Memoca.h"
#include "System/TaskMan.h"
#include "System/Sound/SoundMan.h"
#include "System/Graphic/WorldEnv.h"
#include "Game/SceneInfo.h"
#include "Game/Card.h"
#include "Game/Status.h"
#include "Game/Game.h"
#include "Game/GameWork.h"
#include "Game/NowLoading.h"
#include "Game/Debug/DebugWork.h"
#include "include_asm.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char* strcat(char* dst, const char* src);
extern "C" int strcmp(const char* a, const char* b);

inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
#define NEW(line, T) new (com_malloc_d(sizeof(T), __FILE__ "(" #line ")", RET_ADDR())) T
#define NEW_SIZE(line, size, T) new (com_malloc_d(size, __FILE__ "(" #line ")", RET_ADDR())) T
#define ASSERT_LINE(line, e) if (!(e)) AssertApp(__FILE__, line, #e, 0)

extern CHAR* __BUILD;

using namespace FLOOR;

// System/Rcf
struct RcfKey {
    SHORT _nGroup;
    SHORT _nMember;
};

struct RcfSceneInfo : RcfKey, CSJointT<RcfSceneInfo> {
    char _szTitle[128];
    USHORT _nAtelCount;
    SHORT _nRcfMapIdx;
};

typedef CSJointListT<RcfSceneInfo> RcfSceneInfoList;

struct RcfMapInfo : RcfKey {
    char _szTitle[128];
    RcfSceneInfoList _SceneList;
};

/* CBuf after RcfSceneInfo and CSJointListT<RcfSceneInfo>: gcc emits the vtables in reverse order of definition */
#include "System/Buf.h"

class CRcf { /* PARTIAL: stand-in for System/Rcf CRcf: DebugMenu keeps its own RcfSceneInfo/RcfMapInfo copies (cc1plus fails with Rcf.h here) */
public:
    static void reqLoadRCF(INT nType, INT nGroup, INT nMember, unsigned char* pBuf, INT nFlag);
    static BOOL isCompleteLoadRCF(INT nType, INT nGroup);
    static RcfSceneInfo* getSceneInfo(SHORT nIndex);
    static RcfSceneInfo* getSceneInfo(SHORT nGroup, SHORT nMember);
    static SHORT getSceneInfoIndex(SHORT nGroup, SHORT nMember);
    static SHORT getSceneInfoCount();
    static RcfMapInfo* getMapInfo(SHORT nIndex);
    static SHORT getMapInfoIndex(SHORT nGroup, SHORT nMember);
    static SHORT getMapInfoCount();
};

#include "System/RsrcDB.h"




#include "Form/Menu/SaveLoad.h"
















#include "Game/Atel/Atel.h"






struct RoomStorageInfo {
    BYTE _pad[5];
    BYTE _bOpen;
};

class CDebugMenuLocal : public CPadHandler {
public:
    typedef CHAR* (CDebugMenuLocal::*SELFUNC)();

private:
    CDebugMenu& m_Parent;
    INT m_nPhase;
    INT m_nStep;
    INT m_nDebugStep;
    CTile* m_pTileWindow[2];
    CFnt* m_pStrTitle;
    CFnt* m_pStrVersion;
    CFnt* m_pStrMenu[10];
    CFnt* m_pStrBody[10];
    CFnt* m_pStrMemoca;
    CFnt* m_pStrLoadGame;
    SELFUNC m_MenuItemSelector[10];
    INT m_nMenuItemSel;

public:
    CDebugMenuLocal(CDebugMenu& parent);
    ~CDebugMenuLocal();
    void main();
    void disp();

private:
    void _preparePhaseInit();
    void _phaseInit();
    void _preparePhaseMain();
    void _phaseMain();
    void _setupParam();
    void _checkStartupParam();
    void _setupRcfSceneInfo();
    void _disposeRcfSceneInfo();
    RcfSceneInfo* _getSceneInfo(SHORT nIndex) const;
    RcfSceneInfo* _getSceneInfoFromMap(const RcfMapInfo& info, SHORT nIndex) const;
    RcfSceneInfo* _getSceneInfoFromMap(SHORT nMapIndex, SHORT nIndex) const;
    RcfMapInfo* _getMapInfo(SHORT nIndex) const;
    CHAR* _getSceneTitle(SHORT nArea, SHORT nScene) const;
    INT _getAreaSceneIndex(const RcfSceneInfoList* pList) const;
    INT _getMapIndex(INT& nOutMapToAreaIdx) const;
    FloorLayout* _getFloorDataSrc(BOOL bChkPoo) const;
    SHORT _getFloorNum() const;
    SHORT _getWorldNumOnFloor() const;
    SHORT _getSceneNumOnFloor() const;
    BOOL _isEntranceRoom(const FloorLayout& src) const;
    SHORT _getEntranceSceneOnFloor(BOOL bStart) const;
    void _updateSelMode();
    void _updateAreaName();
    void _mainProc();
    void _selectMenu();
    void _setMenuColor();
    void _selectMenuItem();
    CHAR* _selectUser();
    CHAR* _selectApp();
    CHAR* _selectMode();
    CHAR* _selectModeProc();
    CHAR* _selectWorld();
    CHAR* _selectScene();
    CHAR* _selectSceneTitle();
    CHAR* _selectMap();
    CHAR* _selectMapMember();
    CHAR* _selectSceneTitleFromMap();
    CHAR* _selectAtel();
    CHAR* _selectLocale();
    CHAR* _selectThisMenu();
    CHAR* _selectLoadMark();
    CHAR* _selectFloor();
    CHAR* _selectWorldOnFloor();
    CHAR* _selectRoom();
    CHAR* _selectSceneTitleOnFloor();
    INT _getRoomID(INT nRoomIdx);
    INT _seekTopSceneIdxInRoom();
    void _launchApp(INT nSelApp);
    void _updateSceneInfo(INT nSelApp);
    void _returnMainProc();
    void _prepareSaveDebugParam();
    void _saveDebugParam();
    void _prepareLoadGame();
    void _loadGame();
    void _dispProc();
    void _setMemocaMessage(INT nMesID);
};

static CHAR* _STR_RESERVED_ = "(reserved)";
static CHAR* _STR_DISABLE_SELECT = "\x83Z\x83\x93\x83^\x83N\x83t\x83J";
static CHAR* s_pszUser = "USER";
static CHAR* s_pszApp = "APP.";
static CHAR* s_pszMode = "MODE";
static CHAR* s_pszFloor = "FLOOR";
static CHAR* s_pszWorld = "WORLD";
static CHAR* s_pszScene = "SCENE";
static CHAR* s_pszTitle = "TITLE";
static CHAR* s_pszAtel = "ATEL";
static CHAR* s_pszMenu = "THIS-MENU";
static CHAR* s_pszMap = "MAP";
static CHAR* s_pszMember = "MEMBER";
static CHAR* s_pszRoom = "ROOM";
static CHAR* s_pszLMark = "LOAD-MARK";

namespace {
USER s_DebugUser = USR_SYSTEM;
}

static CHAR* s_pszWorldNameTbl[];
static CHAR* s_pszWorldPrefixTbl[];
static CHAR* s_pszSelectMode[];
static CHAR* s_pszAppTable[];
static CHAR* s_pszLocale[];
static CHAR* s_pszThisMenu[];
static CHAR* s_pszLoadMark[];

static CHAR* s_pszMenuItem[3][10] = {
    {s_pszUser, s_pszApp, s_pszMode, s_pszFloor, s_pszWorld, s_pszScene, s_pszTitle, s_pszAtel, s_pszMenu, s_pszLMark},
    {s_pszUser, s_pszApp, s_pszMode, s_pszFloor, s_pszMap, s_pszMember, s_pszScene, s_pszAtel, s_pszMenu, s_pszLMark},
    {s_pszUser, s_pszApp, s_pszMode, s_pszFloor, s_pszWorld, s_pszRoom, s_pszScene, s_pszAtel, s_pszMenu, s_pszLMark},
};

static CHAR* s_pszNameTable[61] = {
    "RELEASE(NO DEBUG)",
    "PROGRAMMER",
    "\x83L\x83V\x83\x8f\x83_",
    "\x83L\x83N\x83`",
    "\x83S\x83g\x83" "E",
    "\x83R\x83" "E",
    "\x83i\x83J\x83~\x83`",
    "\x83}\x83" "c\x83I\x83J",
    "\x83J\x83^\x83I\x83J",
    "\x83J\x83n\x83\x89",
    "\x83i\x83J\x83V\x83}",
    "PLANNER",
    "\x83\x88\x83V\x83\x82\x83g",
    "\x83\x84\x83X\x83G",
    "\x83~\x83\x84\x83U\x83L",
    "\x83\x88\x83V\x83I\x83J",
    "\x83J\x83\x8f\x83x",
    "\x83t\x83W\x83\x8f\x83\x89",
    "\x83^\x83i\x83_",
    "\x83}\x83" "c\x83" "E\x83\x89",
    "GRAPHIC",
    "\x83t\x83W\x83\x82\x83g",
    "\x83\x84\x83}\x83V\x83^",
    "\x83n\x83}",
    "\x83I\x83I\x83^\x83j",
    "\x83R\x83\x93\x83h\x83" "E",
    "Vigouroux",
    "\x83" "C\x83\x8f\x83T\x83L",
    "\x83\x84\x83}\x83U\x83L",
    "\x83I\x83j\x83V",
    "\x83X\x83M\x83m",
    "\x83R\x83o\x83\x84\x83V(\x83Q\x83\x93)",
    "\x83^\x83j\x83O\x83`",
    "\x83X\x83M\x83\x84\x83}",
    "\x83V\x83m\x83_",
    "\x83n\x83\x84\x83V",
    "\x83n\x83}\x83_",
    "\x83~\x83L",
    "\x83" "C\x83m\x83" "E\x83G",
    "\x83" "A\x83_\x83`",
    "\x83" "C\x83\x8f\x83^",
    "\x83X\x83K",
    "\x83\x84\x83}\x83O\x83`",
    "\x83R\x83o\x83\x84\x83V(\x83V\x83\x93)",
    "\x83i\x83J\x83K\x83\x8f",
    "\x83" "E\x83`\x83\x80\x83\x89",
    "\x83" "C\x83P\x83_",
    "\x83t\x83W\x83I\x83J",
    "\x83`\x83o",
    "\x83\x8f\x83L\x83\x80\x83\x89",
    "\x83" "C\x83^\x83" "C",
    "\x83z\x83\x93\x83_",
    _STR_RESERVED_,
    _STR_RESERVED_,
    "SOUND",
    _STR_RESERVED_,
    _STR_RESERVED_,
    "OTHER",
    _STR_RESERVED_,
    _STR_RESERVED_,
    "END",
};

namespace {
DebugParam s_Param;
char s_szFloor[32];
char s_szArea[128];
char s_szScene[16];
char s_szAreaTitle[128];
char s_szAtel[16];
RcfSceneInfo* s_pErrSceneInfo;
RcfMapInfo* s_pErrMapInfo;
}

template <class T> static inline T& _ref(T& v) {
    return v;
}

static BOOL _menuUpDown(INT& nOutV, INT nMax, BOOL bSetSE);
static BOOL _checkRangeMenu(INT& nOutV, INT nMax);
static BOOL _menuUpDown_SelMode(BOOL bChkRange);
static BOOL _menuUpDown_App();
static BOOL _menuUpDown_Localize();
static BOOL _menuUpDown_ThisMenu();
static BOOL _menuUpDown_LoadMark();

CDebugMenu::CDebugMenu(CTask* pInParent) : CTask(pInParent, PRI_SYSTEM) {
    CSceneInfo::doneChangeScene();
    m_pL = NEW(344, CDebugMenuLocal)(*this);
}

CDebugMenu::~CDebugMenu() {
    delete m_pL;
    m_pL = NULL;
    CSaveLoad::disposeRsrc();
}

void CDebugMenu::main() {
    m_pL->main();
}

void CDebugMenu::disp() {
    m_pL->disp();
}

// store 0x38 (_nRoomIndex) is scheduled early instead of into the jr delay slot
void CDebugMenu::initParam(INT nLocale) {
    DebugParam& p = s_Param;
    p._nUserIndex = 0;
    p._nAppIndex = 0;
    p._nSelectIndex = 0;
    p._nAreaIndex = 0;
    p._nArea = 0;
    p._nScene = 0;
    p._nGroup = 0;
    p._nMember = 0;
    p._nMapToAreaIndex = 0;
    p._nFloorIndex = 0;
    p._nAtelNo = 0;
    p._nLocale = nLocale;
    p._nThisMenu = 0;
    p._nLoadingMark = 0;
    p._iDispCounter = 0;
    p._iBGM = 0;
    p._iDagameNumber = 0;
    p._iEffect = 0;
    p._nRoomIndex = 0;
    p._nSceneIndexInRoom = 0;
}

BOOL CDebugMenu::startLoadFromMemoca() {
    return CMemoca::reqLoad(s_Param);
}

BOOL CDebugMenu::isCompleteLoadFromMemoca() {
    return CMemoca::loadProc() != -9999;
}

CDebugMenuLocal::CDebugMenuLocal(CDebugMenu& parent) : m_Parent(parent) {
    m_pStrTitle = NULL;
    m_pStrVersion = NULL;
    m_pStrMemoca = NULL;
    m_pStrLoadGame = NULL;
    m_nMenuItemSel = 0;
    for (INT i = 0; i < 2; i++) {
        m_pTileWindow[i] = NULL;
    }
    for (INT i = 0; i < 10; i++) {
        m_pStrMenu[i] = NULL;
        m_pStrBody[i] = NULL;
    }
    m_MenuItemSelector[0] = &CDebugMenuLocal::_selectUser;
    m_MenuItemSelector[1] = &CDebugMenuLocal::_selectApp;
    m_MenuItemSelector[2] = &CDebugMenuLocal::_selectMode;
    m_MenuItemSelector[3] = NULL;
    m_MenuItemSelector[4] = &CDebugMenuLocal::_selectWorld;
    m_MenuItemSelector[5] = &CDebugMenuLocal::_selectScene;
    m_MenuItemSelector[6] = &CDebugMenuLocal::_selectSceneTitle;
    m_MenuItemSelector[7] = &CDebugMenuLocal::_selectAtel;
    m_MenuItemSelector[8] = &CDebugMenuLocal::_selectThisMenu;
    m_MenuItemSelector[9] = &CDebugMenuLocal::_selectLoadMark;
    _preparePhaseInit();
}

CDebugMenuLocal::~CDebugMenuLocal() {
    delete m_pStrTitle;
    m_pStrTitle = NULL;
    for (INT i = 0; i < 2; i++) {
        delete m_pTileWindow[i];
        m_pTileWindow[i] = NULL;
    }
    delete m_pStrVersion;
    m_pStrVersion = NULL;
    delete m_pStrMemoca;
    m_pStrMemoca = NULL;
    delete m_pStrLoadGame;
    m_pStrLoadGame = NULL;
    for (INT i = 0; i < 10; i++) {
        delete m_pStrMenu[i];
        m_pStrMenu[i] = NULL;
        delete m_pStrBody[i];
        m_pStrBody[i] = NULL;
    }
    _disposeRcfSceneInfo();
    CRsrcDB::removeGroup(0, 0x2134);
}

void CDebugMenuLocal::main() {
    switch (m_nPhase) {
    case 0:
        _phaseInit();
        break;
    case 1:
        _phaseMain();
        break;
    }
}

void CDebugMenuLocal::disp() {
    if (m_nPhase == 1) {
        _dispProc();
    }
}

void CDebugMenuLocal::_dispProc() {
    for (INT i = 0; i < 2; i++) {
        if (m_pTileWindow[i]) {
            m_pTileWindow[i]->disp();
        }
    }
    if (m_pStrTitle) {
        m_pStrTitle->disp();
    }
    if (m_pStrVersion) {
        m_pStrVersion->disp();
    }
    if (m_pStrMemoca) {
        m_pStrMemoca->disp();
    }
    if (m_pStrLoadGame) {
        m_pStrLoadGame->disp();
    }
    for (int i = 0; i < 10; i++) {
        m_pStrMenu[i]->disp();
        m_pStrBody[i]->disp();
    }
}

void CDebugMenuLocal::_preparePhaseInit() {
    CWorldEnv::setFogColor(0, 0, 0);
    CRcf::reqLoadRCF(0, 0x2134, 0, NULL, 0);
    m_nPhase = 0;
}

void CDebugMenuLocal::_phaseInit() {
    if (CRcf::isCompleteLoadRCF(0, 0)) {
        _setupRcfSceneInfo();
        _checkStartupParam();
        _setupParam();
        _preparePhaseMain();
    }
}

FloorLayout* CDebugMenuLocal::_getFloorDataSrc(BOOL bChkPoo) const {
    BOOL bSora = s_Param._nAppIndex != 1;
    if (bChkPoo && _getWorldNumOnFloor() == WORLD_ID_100ACRE) {
        return CSceneInfo::getFloorDataSrc(CSceneInfo::getFloorIndex(100, TRUE), TRUE);
    }
    return CSceneInfo::getFloorDataSrc(s_Param._nFloorIndex, bSora);
}

static BOOL _isTgsFloor() {
    return s_Param._nAppIndex != 1 && s_Param._nFloorIndex == 14;
}

static BOOL _isJFestaFloor() {
    return s_Param._nAppIndex != 1 && s_Param._nFloorIndex == 13;
}

static SHORT _getEventRoomSceneNum(WORLD_ID w, const RoomSrcInfo& info) {
    BOOL bSora = s_Param._nAppIndex != 1;
    if (_isTgsFloor() || _isJFestaFloor()) {
        w = (WORLD_ID)(s_Param._nFloorIndex + 1);
    }
    return CSceneInfo::getEventRoomSceneNum(w, info, bSora);
}

void CDebugMenuLocal::_checkStartupParam() {
    if (s_Param._nSelectIndex == 2 && !CSceneInfo::getFloorDataCount(s_Param._nAppIndex != 1)) {
        s_Param._nSelectIndex = 0;
    }
    switch (s_Param._nSelectIndex) {
    case 1: {
        INT nMapToAreaIdx = s_Param._nMapToAreaIndex;
        INT nIdx = _getMapIndex(nMapToAreaIdx);
        if (nIdx < 0) {
            s_Param._nAreaIndex = 0;
            s_Param._nMapToAreaIndex = 0;
        } else {
            s_Param._nAreaIndex = nIdx;
            s_Param._nMapToAreaIndex = nMapToAreaIdx;
        }
        break;
    }
    case 2: {
        BOOL bValidRoom = TRUE;
        _getFloorNum();
        SHORT nWorld = _getWorldNumOnFloor();
        FloorLayout* pSrc = _getFloorDataSrc(TRUE);
        ASSERT_LINE(634, pSrc);
        if (nWorld != s_Param._nArea) {
            if (s_Param._nRoomIndex == 0 || pSrc->_nRoomCount + 1 == s_Param._nRoomIndex) {
                if (s_Param._nArea) {
                    bValidRoom = FALSE;
                } else {
                    s_Param._nArea = nWorld;
                }
            } else {
                bValidRoom = FALSE;
            }
        }
        if (bValidRoom) {
            if (s_Param._nRoomIndex < 0 || pSrc->_nRoomCount + 1 < s_Param._nRoomIndex) {
                bValidRoom = FALSE;
            } else {
                SHORT nScene;
                if (_isEntranceRoom(*pSrc)) {
                    nScene = _getEntranceSceneOnFloor(s_Param._nRoomIndex == 0);
                } else {
                    INT nIdx = s_Param._nRoomIndex - 1;
                    const RoomSrcInfo& r = pSrc->_rooms[nIdx];
                    nScene = _getEventRoomSceneNum((WORLD_ID)nWorld, r);
                    if (!nScene && r.isFixSceneRoom()) {
                        nScene = r._nFixScene;
                    }
                }
                if (nScene && nScene != s_Param._nScene) {
                    bValidRoom = FALSE;
                }
            }
            if (bValidRoom) {
                INT nIdx = _getAreaSceneIndex(NULL);
                if (nIdx >= 0) {
                    s_Param._nSceneIndexInRoom = nIdx;
                } else {
                    bValidRoom = FALSE;
                }
                if (bValidRoom) {
                    break;
                }
            }
        }
        s_Param._nRoomIndex = 0;
        s_Param._nSceneIndexInRoom = _seekTopSceneIdxInRoom();
        break;
    }
    default: {
        INT nIdx = _getAreaSceneIndex(NULL);
        s_Param._nAreaIndex = nIdx > -1 ? nIdx : 0;
        break;
    }
    }
}

void CDebugMenuLocal::_setupParam() {
    G_RGBA color;
    G_RGBA color2;
    INT i;
    if (!(s_Param._iDagameNumber & 0xFE)) {
        DebugWork::g_dw.sys.m_iDagameNumber = s_Param._iDagameNumber;
    }
    if (!(s_Param._iBGM & 0xFE)) {
        DebugWork::g_dw.sys.m_iBGM = s_Param._iBGM;
    }
    if (!(s_Param._iEffect & 0xFE)) {
        DebugWork::g_dw.sys.m_iEffect = s_Param._iEffect;
    }
    for (i = 0; i < 2; i++) {
        if (!m_pTileWindow[i]) {
            m_pTileWindow[i] = NEW(705, CTile);
        }
        INT y = i * 4;
        m_pTileWindow[i]->m_P0.set((-209 + y) << 4, (-170 + y) << 4);
        m_pTileWindow[i]->m_P1.set((210 + y) << 4, (169 + y) << 4);
        BYTE c = 0xA0 - i * 0x60;
        m_pTileWindow[i]->m_C0.set(c, c, c, 0x80);
        m_pTileWindow[i]->setLayerFG(1 - i);
    }
    color.set(0x20, 0x20, 0x20, 0x80);
    color2.set(0x28, 0x38, 0x80, 0x80);
    if (!m_pStrTitle) {
        m_pStrTitle = NEW(718, CFnt)(0, -124, &color2, 28, "reC-N proj. Debug Menu", 0x42, 0xFFFF28);
        m_pStrTitle->setLayerFG(3);
    }
    if (!m_pStrVersion) {
        char szTmp[256];
        sprintf(szTmp, " - Build : %s -", __BUILD);
        m_pStrVersion = NEW(728, CFnt)(0, -104, &color, 12, NULL, 2, 0xFFFF28);
        m_pStrVersion->allocText(szTmp);
        m_pStrVersion->setLayerFG(2);
    }
    for (INT i = 0; i < 10; i++) {
        INT nY = i * 22 - 74;
        if (!m_pStrMenu[i]) {
            m_pStrMenu[i] =
                NEW(740, CFnt)(-160, nY, &color, 16, s_pszMenuItem[s_Param._nSelectIndex][i], 0, 0xFFFF28);
            m_pStrMenu[i]->setLayerFG(2);
        }
        if (!m_pStrBody[i]) {
            m_pStrBody[i] = NEW(745, CFnt)(-80, nY, &color, 16, NULL, 0, 0xFFFF28);
            m_pStrBody[i]->setLayerFG(2);
        }
    }
    _setMenuColor();
    if (!m_pStrMemoca) {
        m_pStrMemoca = NEW(756, CFnt)(-110, 162, &color, 13, NULL, 0, 0xFFFF28);
        m_pStrMemoca->setLayerFG(2);
    }
    _setMemocaMessage(0);
    if (!m_pStrLoadGame) {
        m_pStrLoadGame = NEW(764, CFnt)(-110, 146, &color, 13,
                                        "Push \xf9W\x81\xa0\xf9" "A Button \x81\xa8 LOAD GAME", 0, 0xFFFF28);
        m_pStrLoadGame->setLayerFG(2);
    }
    CDebugMenu::setDebugUser((USER)s_Param._nUserIndex);
    m_pStrBody[0]->setText(s_pszNameTable[s_Param._nUserIndex]);
    m_pStrBody[1]->setText(s_pszAppTable[s_Param._nAppIndex]);
    m_pStrBody[2]->setText(s_pszSelectMode[s_Param._nSelectIndex]);
    if (s_Param._nLocale < 0) {
        s_Param._nLocale = 0;
    }
    m_pStrBody[8]->setText(s_pszThisMenu[s_Param._nThisMenu]);
    m_pStrBody[9]->setText(s_pszLoadMark[s_Param._nLoadingMark]);
    m_pStrBody[3]->setText(s_szFloor);
    m_pStrBody[4]->setText(s_szArea);
    m_pStrBody[5]->setText(s_szScene);
    m_pStrBody[6]->setText(s_szAreaTitle);
    m_pStrBody[7]->setText(s_szAtel);
    if (s_Param._nSelectIndex) {
        _updateSelMode();
    }
    _updateAreaName();
}

void CDebugMenuLocal::_setupRcfSceneInfo() {
    s_pErrSceneInfo = NEW(798, RcfSceneInfo);
    ASSERT_LINE(799, s_pErrSceneInfo);
    _ref(s_pErrSceneInfo)->_szTitle[0] = 0;
    _ref(s_pErrSceneInfo)->_nGroup = 0;
    _ref(s_pErrSceneInfo)->_nMember = 1;
    _ref(s_pErrSceneInfo)->_nAtelCount = 0;
    _ref(s_pErrSceneInfo)->_nRcfMapIdx = 0;
    s_pErrMapInfo = NEW(806, RcfMapInfo);
    ASSERT_LINE(807, s_pErrMapInfo);
    _ref(s_pErrMapInfo)->_szTitle[0] = 0;
    _ref(s_pErrMapInfo)->_nGroup = 0;
    _ref(s_pErrMapInfo)->_nMember = 1;
    _ref(s_pErrMapInfo)->_SceneList.pushBack(*_ref(s_pErrSceneInfo));
}

void CDebugMenuLocal::_disposeRcfSceneInfo() {
    delete s_pErrSceneInfo;
    s_pErrSceneInfo = NULL;
    delete s_pErrMapInfo;
    s_pErrMapInfo = NULL;
}

RcfSceneInfo* CDebugMenuLocal::_getSceneInfo(SHORT nIndex) const {
    RcfSceneInfo* p = CRcf::getSceneInfo(nIndex);
    return p ? p : s_pErrSceneInfo;
}

RcfSceneInfo* CDebugMenuLocal::_getSceneInfoFromMap(SHORT nMapIndex, SHORT nIndex) const {
    RcfMapInfo* pInfo = CRcf::getMapInfo(nMapIndex);
    return pInfo ? _getSceneInfoFromMap(*pInfo, nIndex) : s_pErrSceneInfo;
}

RcfSceneInfo* CDebugMenuLocal::_getSceneInfoFromMap(const RcfMapInfo& info, SHORT nIndex) const {
    UINT i = 0;
    RcfSceneInfo* p;
    RcfSceneInfo* __pn;
    for (p = ((RcfSceneInfoList&)info._SceneList).getHead(), __pn = p ? p->getNext() : NULL; p;
         p = __pn, __pn = p ? p->getNext() : NULL) {
        if (i == nIndex) {
            return p;
        }
        i++;
    }
    return s_pErrSceneInfo;
}

RcfMapInfo* CDebugMenuLocal::_getMapInfo(SHORT nIndex) const {
    RcfMapInfo* p = CRcf::getMapInfo(nIndex);
    return p ? p : s_pErrMapInfo;
}

template <class T> static inline T* _sjNext(T* p) {
    return p ? p->getNext() : NULL;
}

INT CDebugMenuLocal::_getAreaSceneIndex(const RcfSceneInfoList* pList) const {
    if (!pList) {
        return CRcf::getSceneInfoIndex(s_Param._nArea, s_Param._nScene);
    }
    INT i = 0;
    RcfSceneInfo* p;
    RcfSceneInfo* __pn;
    for (p = ((RcfSceneInfoList*)pList)->getHead(), __pn = p ? p->getNext() : NULL; p != NULL;
         __pn = ((p = __pn) != NULL ? p->getNext() : NULL)) {
        if (p->_nGroup == s_Param._nArea && p->_nMember == s_Param._nScene) {
            return i;
        }
        i++;
    }
    return -1;
}

INT CDebugMenuLocal::_getMapIndex(INT& nOutMapToAreaIdx) const {
    SHORT nIdx = CRcf::getMapInfoIndex(s_Param._nGroup, s_Param._nMember);
    if (nIdx >= 0) {
        RcfMapInfo* pMap = CRcf::getMapInfo(nIdx);
        ASSERT_LINE(886, pMap);
        if ((UINT)nOutMapToAreaIdx < (UINT)pMap->_SceneList.getListSize()) {
            nOutMapToAreaIdx = _getAreaSceneIndex(&pMap->_SceneList);
            if (nOutMapToAreaIdx < 0) {
                nOutMapToAreaIdx = 0;
            }
        } else {
            nOutMapToAreaIdx = 0;
        }
    }
    return nIdx;
}

CHAR* CDebugMenuLocal::_getSceneTitle(SHORT nArea, SHORT nScene) const {
    RcfSceneInfo* p = CRcf::getSceneInfo(nArea, nScene);
    return p ? p->_szTitle : NULL;
}

SHORT CDebugMenuLocal::_getFloorNum() const {
    BOOL bSora = s_Param._nAppIndex != 1;
    SHORT nNum = CSceneInfo::getFloorNumber(s_Param._nFloorIndex, bSora);
    if (!nNum) {
        s_Param._nFloorIndex = 0;
        nNum = CSceneInfo::getFloorNumber(0, bSora);
    }
    return nNum;
}

SHORT CDebugMenuLocal::_getWorldNumOnFloor() const {
    WorldSelTable* pTbl = CSceneInfo::getSelectableWorldTable(s_Param._nFloorIndex, s_Param._nAppIndex != 1);
    ASSERT_LINE(929, pTbl);
    if (pTbl->_nCount <= s_Param._nAreaIndex) {
        s_Param._nAreaIndex = 0;
    }
    return pTbl->_pTbl[s_Param._nAreaIndex];
}

SHORT CDebugMenuLocal::_getSceneNumOnFloor() const {
    if (s_Param._nSceneIndexInRoom < 0) {
        FloorLayout* pSrc = _getFloorDataSrc(TRUE);
        ASSERT_LINE(945, pSrc);
        SHORT nScene;
        if (_isEntranceRoom(*pSrc)) {
            nScene = _getEntranceSceneOnFloor(s_Param._nRoomIndex == 0);
        } else {
            WORLD_ID w = (WORLD_ID)_getWorldNumOnFloor();
            const RoomSrcInfo& r = pSrc->_rooms[s_Param._nRoomIndex - 1];
            nScene = _getEventRoomSceneNum(w, r);
            if (!nScene && r.isFixSceneRoom()) {
                nScene = r._nFixScene;
            }
            if (!nScene) {
                nScene = 1;
            }
        }
        return nScene;
    }
    return _getSceneInfo(s_Param._nSceneIndexInRoom)->_nMember;
}

BOOL CDebugMenuLocal::_isEntranceRoom(const FloorLayout& src) const {
    return s_Param._nRoomIndex == 0 || src._nRoomCount + 1 == s_Param._nRoomIndex;
}

SHORT CDebugMenuLocal::_getEntranceSceneOnFloor(BOOL bStart) const {
    FloorLayout* pSrc = _getFloorDataSrc(FALSE);
    if (!pSrc) {
        return 0;
    }
    return bStart ? pSrc->_nSceneStartEnt : pSrc->_nSceneGoalEnt;
}

void CDebugMenuLocal::_preparePhaseMain() {
    m_nStep = 0;
    m_nPhase = 1;
}

void CDebugMenuLocal::_phaseMain() {
    switch (m_nStep) {
    case 0:
        m_nStep = 1;
    case 1:
        _mainProc();
        break;
    case 2:
        _saveDebugParam();
        break;
    case 3:
        _loadGame();
        break;
    }
}

void CDebugMenuLocal::_returnMainProc() {
    _setMemocaMessage(0);
    m_nStep = 1;
}

void CDebugMenuLocal::_mainProc() {
    _selectMenu();
    _selectMenuItem();
    if (isButtonTrg(0x10)) {
        _prepareSaveDebugParam();
    } else if (isButtonTrg(0x80)) {
        _prepareLoadGame();
    } else if (isButtonTrg(0x8E0)) {
        CNowLoading::enable(s_Param._nLoadingMark == 0);
        CNowLoading::setMode(1);
        _launchApp(s_Param._nAppIndex);
        m_nStep = 4;
    }
}

void CDebugMenuLocal::_selectMenu() {
    INT nOldSel = m_nMenuItemSel;
    if (isButtonRpt(0x305000)) {
        SELFUNC pfnc = NULL;
        INT nMax = 9;
        while (pfnc == (SELFUNC)NULL) {
            if (isButtonRpt(0x204000)) {
                if (m_nMenuItemSel < nMax) {
                    m_nMenuItemSel++;
                } else {
                    m_nMenuItemSel = 0;
                }
            } else {
                if (m_nMenuItemSel > 0) {
                    m_nMenuItemSel--;
                } else {
                    m_nMenuItemSel = nMax;
                }
            }
            pfnc = m_MenuItemSelector[m_nMenuItemSel];
        }
        CSound::playSE(1, -1, 0x40);
    }
    if (nOldSel != m_nMenuItemSel) {
        _setMenuColor();
    }
}

void CDebugMenuLocal::_setMenuColor() {
    INT i;
    for (i = 0; i < 10; i++) {
        CFnt* pMenu = m_pStrMenu[i];
        if (i == m_nMenuItemSel) {
            pMenu->m_C0.set(0x50, 0x10, 0x10);
        } else {
            pMenu->m_C0.set(0x40, 0x40, 0x40);
        }
    }
}

void CDebugMenuLocal::_selectMenuItem() {
    if (m_nMenuItemSel >= 0 && m_nMenuItemSel < 10) {
        SELFUNC pfnc = m_MenuItemSelector[m_nMenuItemSel];
        CHAR* pszBody = pfnc ? (this->*pfnc)() : NULL;
        if (pszBody) {
            m_pStrBody[m_nMenuItemSel]->setText(pszBody);
        }
    }
}

CHAR* CDebugMenuLocal::_selectUser() {
    CHAR* pszBody = NULL;
    if (_menuUpDown(s_Param._nUserIndex, 60, TRUE)) {
        pszBody = s_pszNameTable[s_Param._nUserIndex];
        while (strcmp(pszBody, _STR_RESERVED_) == 0) {
            _menuUpDown(s_Param._nUserIndex, 60, FALSE);
            pszBody = s_pszNameTable[s_Param._nUserIndex];
        }
    }
    return pszBody;
}

CHAR* CDebugMenuLocal::_selectApp() {
    BOOL bSora = s_Param._nAppIndex != 1;
    if (_menuUpDown_App()) {
        if (bSora != (s_Param._nAppIndex != 1)) {
            if (_menuUpDown_SelMode(TRUE)) {
                CHAR* pszMode = _selectModeProc();
                if (pszMode) {
                    m_pStrBody[2]->setText(pszMode);
                }
            }
            if (s_Param._nSelectIndex == 2) {
                s_Param._nFloorIndex = 0;
                s_Param._nAreaIndex = 0;
                s_Param._nRoomIndex = 0;
                s_Param._nSceneIndexInRoom = _seekTopSceneIdxInRoom();
                _updateAreaName();
            }
        }
        return s_pszAppTable[s_Param._nAppIndex];
    }
    return NULL;
}

CHAR* CDebugMenuLocal::_selectMode() {
    return _menuUpDown_SelMode(FALSE) ? _selectModeProc() : NULL;
}

CHAR* CDebugMenuLocal::_selectModeProc() {
    s_Param._nAreaIndex = 0;
    s_Param._nMapToAreaIndex = 0;
    s_Param._nAtelNo = 0;
    _updateSelMode();
    _updateAreaName();
    return s_pszSelectMode[s_Param._nSelectIndex];
}

CHAR* CDebugMenuLocal::_selectWorld() {
    BOOL bUpdate = FALSE;
    INT nSize = CRcf::getSceneInfoCount();
    RcfSceneInfo* pInfo = nSize > 1 ? _getSceneInfo(s_Param._nAreaIndex) : NULL;
    if (pInfo) {
        SHORT nArea = pInfo->_nGroup;
        if (_menuUpDown(s_Param._nAreaIndex, nSize, TRUE)) {
            INT i;
            s_Param._nAtelNo = 0;
            for (i = 0; i < nSize; i++) {
                if (_getSceneInfo(s_Param._nAreaIndex)->_nGroup != nArea) {
                    bUpdate = TRUE;
                    break;
                }
                _menuUpDown(s_Param._nAreaIndex, nSize, FALSE);
            }
        }
    }
    if (bUpdate) {
        _updateAreaName();
    }
    return NULL;
}

CHAR* CDebugMenuLocal::_selectScene() {
    BOOL bUpdate = FALSE;
    INT nSize = CRcf::getSceneInfoCount();
    RcfSceneInfo* pInfo = nSize > 1 ? _getSceneInfo(s_Param._nAreaIndex) : NULL;
    if (pInfo) {
        SHORT nArea = pInfo->_nGroup;
        if (_menuUpDown(s_Param._nAreaIndex, nSize, TRUE)) {
            INT i;
            s_Param._nAtelNo = 0;
            for (i = 0; i < nSize; i++) {
                if (_getSceneInfo(s_Param._nAreaIndex)->_nGroup == nArea) {
                    bUpdate = TRUE;
                    break;
                }
                _menuUpDown(s_Param._nAreaIndex, nSize, FALSE);
            }
        }
    }
    if (bUpdate) {
        _updateAreaName();
    }
    return NULL;
}

CHAR* CDebugMenuLocal::_selectSceneTitle() {
    if (_menuUpDown(s_Param._nAreaIndex, CRcf::getSceneInfoCount(), TRUE)) {
        s_Param._nAtelNo = 0;
        if (_getSceneInfo(s_Param._nAreaIndex)) {
            _updateAreaName();
        }
    }
    return NULL;
}

CHAR* CDebugMenuLocal::_selectMap() {
    BOOL bUpdate = FALSE;
    INT nSize = CRcf::getMapInfoCount();
    RcfMapInfo* pMap = nSize > 1 ? CRcf::getMapInfo(s_Param._nAreaIndex) : NULL;
    if (pMap && _menuUpDown(s_Param._nAreaIndex, nSize, TRUE)) {
        INT i;
        SHORT nArea = pMap->_nGroup;
        for (i = 0; i < nSize; i++) {
            if (_getMapInfo(s_Param._nAreaIndex)->_nGroup != nArea) {
                bUpdate = TRUE;
                break;
            }
            _menuUpDown(s_Param._nAreaIndex, nSize, FALSE);
        }
    }
    if (bUpdate) {
        s_Param._nMapToAreaIndex = 0;
        _updateAreaName();
    }
    return NULL;
}

CHAR* CDebugMenuLocal::_selectMapMember() {
    BOOL bUpdate = FALSE;
    INT nSize = CRcf::getMapInfoCount();
    RcfMapInfo* pMap = nSize > 1 ? CRcf::getMapInfo(s_Param._nAreaIndex) : NULL;
    if (pMap && _menuUpDown(s_Param._nAreaIndex, nSize, TRUE)) {
        INT i;
        SHORT nArea = pMap->_nGroup;
        for (i = 0; i < nSize; i++) {
            if (CRcf::getMapInfo(s_Param._nAreaIndex)->_nGroup == nArea) {
                bUpdate = TRUE;
                break;
            }
            _menuUpDown(s_Param._nAreaIndex, nSize, FALSE);
        }
    }
    if (bUpdate) {
        s_Param._nMapToAreaIndex = 0;
        _updateAreaName();
    }
    return NULL;
}

CHAR* CDebugMenuLocal::_selectSceneTitleFromMap() {
    RcfMapInfo* pMap = _getMapInfo(s_Param._nAreaIndex);
    if (pMap && _menuUpDown(s_Param._nMapToAreaIndex, pMap->_SceneList.getListSize(), TRUE) &&
        _getSceneInfoFromMap(*pMap, s_Param._nMapToAreaIndex)) {
        _updateAreaName();
    }
    return NULL;
}

CHAR* CDebugMenuLocal::_selectAtel() {
    if (s_Param._nSelectIndex == 0) {
        RcfSceneInfo* pInfo = CRcf::getSceneInfo(s_Param._nAreaIndex);
        if (pInfo && pInfo->_nAtelCount && _menuUpDown(s_Param._nAtelNo, pInfo->_nAtelCount, TRUE)) {
            sprintf(s_szAtel, "%2d", s_Param._nAtelNo);
        }
    }
    return NULL;
}

CHAR* CDebugMenuLocal::_selectLocale() {
    return _menuUpDown_Localize() ? s_pszLocale[s_Param._nLocale] : NULL;
}

CHAR* CDebugMenuLocal::_selectThisMenu() {
    return _menuUpDown_ThisMenu() ? s_pszThisMenu[s_Param._nThisMenu] : NULL;
}

CHAR* CDebugMenuLocal::_selectLoadMark() {
    return _menuUpDown_LoadMark() ? s_pszLoadMark[s_Param._nLoadingMark] : NULL;
}

CHAR* CDebugMenuLocal::_selectFloor() {
    if (s_Param._nSelectIndex == 2) {
        INT nMax = CSceneInfo::getFloorDataCount(s_Param._nAppIndex != 1);
        if (_menuUpDown(s_Param._nFloorIndex, nMax, TRUE)) {
            if (CSceneInfo::getFloorNumber(s_Param._nFloorIndex, TRUE) == 100) {
                _menuUpDown(s_Param._nFloorIndex, nMax, FALSE);
            }
            s_Param._nAreaIndex = 0;
            s_Param._nRoomIndex = _getEntranceSceneOnFloor(TRUE) == 0;
            s_Param._nSceneIndexInRoom = _seekTopSceneIdxInRoom();
            _updateAreaName();
        }
    }
    return NULL;
}

CHAR* CDebugMenuLocal::_selectWorldOnFloor() {
    WorldSelTable* pTbl = CSceneInfo::getSelectableWorldTable(s_Param._nFloorIndex, s_Param._nAppIndex != 1);
    ASSERT_LINE(1419, pTbl);
    if (_menuUpDown(s_Param._nAreaIndex, pTbl->_nCount, TRUE)) {
        if (_getWorldNumOnFloor() == WORLD_ID_100ACRE && !CSceneInfo::getFloorIndex(100, TRUE)) {
            _menuUpDown(s_Param._nAreaIndex, pTbl->_nCount, FALSE);
        }
        s_Param._nRoomIndex = _getEntranceSceneOnFloor(TRUE) == 0;
        s_Param._nSceneIndexInRoom = _seekTopSceneIdxInRoom();
        _updateAreaName();
    }
    return NULL;
}

CHAR* CDebugMenuLocal::_selectRoom() {
    FloorLayout* pSrc = _getFloorDataSrc(TRUE);
    ASSERT_LINE(1442, pSrc);
    INT nMax = pSrc->_nRoomCount + (_getEntranceSceneOnFloor(FALSE) ? 2 : 1);
    if (_menuUpDown(s_Param._nRoomIndex, nMax, TRUE)) {
        if (s_Param._nRoomIndex == 0 && !_getEntranceSceneOnFloor(TRUE)) {
            _menuUpDown(s_Param._nRoomIndex, nMax, FALSE);
        }
        s_Param._nSceneIndexInRoom = _seekTopSceneIdxInRoom();
        _updateAreaName();
    }
    return NULL;
}

CHAR* CDebugMenuLocal::_selectSceneTitleOnFloor() {
    FloorLayout* pSrc = _getFloorDataSrc(TRUE);
    ASSERT_LINE(1465, pSrc);
    if (s_Param._nRoomIndex) {
        if (pSrc->_nRoomCount + 1 == s_Param._nRoomIndex) {
            return NULL;
        }
        RcfSceneInfo* pInfo = _getSceneInfo(s_Param._nSceneIndexInRoom);
        SHORT nArea = pInfo->_nGroup;
        const RoomSrcInfo& r = pSrc->_rooms[s_Param._nRoomIndex - 1];
        SHORT nScene = _getEventRoomSceneNum((WORLD_ID)nArea, r);
        if (!nScene && r.isFixSceneRoom()) {
            nScene = r._nFixScene;
        }
        if (nScene) {
            return NULL;
        }
        BOOL bUpdate = FALSE;
        INT nSize = CRcf::getSceneInfoCount();
        INT nIdx = s_Param._nSceneIndexInRoom;
        if (_menuUpDown(nIdx, nSize, TRUE)) {
            INT i;
            for (i = 0; i < nSize; i++) {
                pInfo = _getSceneInfo(nIdx);
                if (pInfo->_nGroup == nArea && pInfo->_nMember >= 3 && (USHORT)pInfo->_nMember < 1000) {
                    bUpdate = TRUE;
                    break;
                }
                _menuUpDown(nIdx, nSize, FALSE);
            }
        }
        if (bUpdate) {
            s_Param._nSceneIndexInRoom = nIdx;
            _updateAreaName();
        }
    }
    return NULL;
}

INT CDebugMenuLocal::_getRoomID(INT nRoomIdx) {
    FloorLayout* pSrc = _getFloorDataSrc(TRUE);
    ASSERT_LINE(1514, pSrc);
    if (nRoomIdx == 0) {
        return 0;
    }
    if (pSrc->_nRoomCount + 1 == nRoomIdx) {
        return 99;
    }
    return (nRoomIdx > 0 && nRoomIdx < pSrc->_nRoomCount + 1) ? nRoomIdx : -1;
}

INT CDebugMenuLocal::_seekTopSceneIdxInRoom() {
    FloorLayout* pSrc = _getFloorDataSrc(TRUE);
    ASSERT_LINE(1531, pSrc);
    SHORT nArea;
    SHORT nScene;
    BOOL bUpper;
    INT i;
    RcfSceneInfo* pInfo;
    if (_isEntranceRoom(*pSrc)) {
        nArea = 0;
        nScene = _getEntranceSceneOnFloor(s_Param._nRoomIndex == 0);
    } else {
        nArea = _getWorldNumOnFloor();
        const RoomSrcInfo& r = pSrc->_rooms[s_Param._nRoomIndex - 1];
        nScene = _getEventRoomSceneNum((WORLD_ID)nArea, r);
        if (!nScene && r.isFixSceneRoom()) {
            nScene = r._nFixScene;
        }
    }
    bUpper = FALSE;
    if (!nScene) {
        nScene = 2;
        bUpper = TRUE;
    }
    for (i = 0; (pInfo = CRcf::getSceneInfo(i)) != NULL; i++) {
        if (nArea == pInfo->_nGroup) {
            if (bUpper) {
                if ((USHORT)nScene < 1000 && nScene < pInfo->_nMember) {
                    return i;
                }
            } else if (nScene == pInfo->_nMember) {
                return i;
            }
        }
    }
    return -1;
}

void CDebugMenuLocal::_updateSelMode() {
    CHAR** ppszMenu = s_pszMenuItem[s_Param._nSelectIndex];
    m_pStrMenu[3]->setText(ppszMenu[3]);
    m_pStrMenu[4]->setText(ppszMenu[4]);
    m_pStrMenu[5]->setText(ppszMenu[5]);
    m_pStrMenu[6]->setText(ppszMenu[6]);
    SELFUNC fncFloor = NULL;
    SELFUNC fncArea;
    SELFUNC fncScene;
    SELFUNC fncAreaTitle;
    SELFUNC fncAtel = NULL;
    switch (s_Param._nSelectIndex) {
    case 1:
        fncArea = &CDebugMenuLocal::_selectMap;
        fncScene = &CDebugMenuLocal::_selectMapMember;
        fncAreaTitle = &CDebugMenuLocal::_selectSceneTitleFromMap;
        break;
    case 2:
        fncFloor = &CDebugMenuLocal::_selectFloor;
        fncArea = &CDebugMenuLocal::_selectWorldOnFloor;
        fncScene = &CDebugMenuLocal::_selectRoom;
        fncAreaTitle = &CDebugMenuLocal::_selectSceneTitleOnFloor;
        break;
    default:
        fncArea = &CDebugMenuLocal::_selectWorld;
        fncScene = &CDebugMenuLocal::_selectScene;
        fncAreaTitle = &CDebugMenuLocal::_selectSceneTitle;
        fncAtel = &CDebugMenuLocal::_selectAtel;
        break;
    }
    m_MenuItemSelector[3] = fncFloor;
    m_MenuItemSelector[4] = fncArea;
    m_MenuItemSelector[5] = fncScene;
    m_MenuItemSelector[6] = fncAreaTitle;
    m_MenuItemSelector[7] = fncAtel;
}

void CDebugMenuLocal::_updateAreaName() {
    RcfSceneInfo* pInfo;
    CHAR* pszAtel = _STR_DISABLE_SELECT;
    CHAR* pszFloor = pszAtel;
    BOOL bSetWorldName = TRUE;
    SHORT nArea;
    SHORT nScene;
    if (s_Param._nSelectIndex == 1) {
        bSetWorldName = FALSE;
        RcfMapInfo* pMap = _getMapInfo(s_Param._nAreaIndex);
        pInfo = _getSceneInfoFromMap(*pMap, s_Param._nMapToAreaIndex);
        INT n = pMap->_nGroup;
        if (n >= 1 && n <= 13) {
            strcpy(s_szArea, s_pszWorldPrefixTbl[n]);
        } else {
            sprintf(s_szArea, "%2d", n);
        }
        s_Param._nGroup = pMap->_nGroup;
        sprintf(s_szScene, "%2d", pMap->_nMember);
        s_Param._nMember = pMap->_nMember;
        nArea = pInfo->_nGroup;
        nScene = pInfo->_nMember;
        sprintf(s_szAreaTitle, "%d-%d %s", nArea, nScene, pInfo->_szTitle);
    } else if (s_Param._nSelectIndex == 2) {
        pszFloor = NULL;
        SHORT nFloor = _getFloorNum();
        if (nFloor < 0) {
            sprintf(s_szFloor, "B%dF", -nFloor);
        } else {
            sprintf(s_szFloor, "%2dF", nFloor);
            if (_isTgsFloor()) {
                strcat(s_szFloor, " (TGS)");
            } else if (_isJFestaFloor()) {
                strcat(s_szFloor, " (\x83W\x83\x83\x83\x93\x83t\x83" "F\x83X)");
            }
        }
        nArea = _getWorldNumOnFloor();
        sprintf(s_szArea, "%2d", nArea);
        INT nRoomID = _getRoomID(s_Param._nRoomIndex);
        sprintf(s_szScene, "%2d", nRoomID);
        BOOL bEntrance = nRoomID == 0 || nRoomID == 99;
        if (bEntrance) {
            nArea = 0;
        }
        nScene = _getSceneNumOnFloor();
        CHAR* pszTitle = _getSceneTitle(nArea, nScene);
        sprintf(s_szAreaTitle, "%d-%d %s", nArea, nScene, pszTitle ? pszTitle : "");
    } else {
        pInfo = _getSceneInfo(s_Param._nAreaIndex);
        if (pInfo->_nAtelCount) {
            if (!((UINT)s_Param._nAtelNo < pInfo->_nAtelCount)) {
                s_Param._nAtelNo = pInfo->_nAtelCount - 1;
            }
            sprintf(s_szAtel, "%2d", s_Param._nAtelNo);
            pszAtel = NULL;
        } else {
            pszAtel = "\x96\xa2\x93o\x98^";
        }
        nArea = pInfo->_nGroup;
        nScene = pInfo->_nMember;
        sprintf(s_szArea, "%2d", nArea);
        sprintf(s_szScene, "%2d", nScene);
        sprintf(s_szAreaTitle, "%d-%d %s", nArea, nScene, pInfo->_szTitle);
    }
    s_Param._nScene = nScene;
    s_Param._nArea = nArea;
    if (bSetWorldName && nArea < 14) {
        if (s_Param._nSelectIndex == 2) {
            nArea = _getWorldNumOnFloor();
        }
        strcat(s_szArea, "  ");
        strcat(s_szArea, s_pszWorldNameTbl[nArea]);
    }
    if (pszFloor) {
        strcpy(s_szFloor, pszFloor);
    }
    if (pszAtel) {
        strcpy(s_szAtel, pszAtel);
    }
}

void CDebugMenuLocal::_launchApp(INT nSelApp) {
    BOOL b60Fps = TRUE;
    INT nAppID = -1;
    switch (nSelApp) {
    case 0:
    case 1:
        nAppID = 6;
        break;
    case 2:
        nAppID = 10;
        break;
    case 3:
        nAppID = 11;
        break;
    case 5:
        nAppID = 12;
        break;
    case 4:
        nAppID = 13;
        break;
    case 6:
        nAppID = 14;
        break;
    case 7:
        nAppID = 15;
        break;
    case 8:
        nAppID = 16;
        break;
    case 9:
        nAppID = 17;
        break;
    case 10:
        nAppID = 18;
        break;
    case 16:
        nAppID = 19;
        break;
    case 17:
        nAppID = 27;
        break;
    case 19:
        nAppID = 20;
        b60Fps = FALSE;
        break;
    case 20:
        nAppID = 21;
        break;
    case 13:
        nAppID = 22;
        break;
    case 14:
        nAppID = 23;
        b60Fps = FALSE;
        break;
    case 15:
        nAppID = 24;
        b60Fps = FALSE;
        break;
    case 18:
        nAppID = 25;
        break;
    case 21:
        nAppID = 26;
        break;
    case 11:
        nAppID = 9;
        break;
    case 12:
        nAppID = 8;
        break;
    }
    if (nAppID == -1) {
        return;
    }
    if (!CTaskMan::reqChangeMainTask(nAppID, (MAINTASK_CALLMODE)1)) {
        return;
    }
    if (b60Fps) {
        CMainApp::setFpsFull();
    } else {
        CMainApp::setFpsHalf();
    }
    CSound::playSE(2, -1, 0x40);
    CMainApp::startCountPlayTime();
    CDebugMenu::setDebugUser((USER)s_Param._nUserIndex);
    if (nSelApp < 13) {
        if (nSelApp >= 11) {
            return;
        }
    }
    BOOL bRiku = nSelApp == 1;
    RcfSceneInfo* pInfo;
    switch (s_Param._nSelectIndex) {
    case 1:
        pInfo = _getSceneInfoFromMap(s_Param._nAreaIndex, s_Param._nMapToAreaIndex);
        break;
    case 2:
        pInfo = _getSceneInfo(s_Param._nSceneIndexInRoom);
        break;
    default:
        pInfo = _getSceneInfo(s_Param._nAreaIndex);
        break;
    }
    AREA_SCENE as(pInfo->_nGroup, pInfo->_nMember);
    CGameWork::resetAll();
    if (bRiku) {
        SysDataCOM s;
        INT i;
        CGameWork::getSysDataCOM(s);
        s.Data.SoraClear = 1;
        CGameWork::setFlag(0, 9, TRUE);
        CGameWork::setFlag(0, 0x11, TRUE);
        for (i = 0; i < 14; i++) {
            s.Data.WorldTblSora[i] = i + 1;
        }
        for (i = 0; i < 6; i++) {
            s.Data.PoohClearIntro[i] = 1;
            s.Data.PoohClear[i] = 3;
        }
        s.Data.PoohIntro = 0xFF;
        CGameWork::setSysDataCOM(&s, NULL);
    }
    CSceneInfo::initNewGame(!bRiku);
    if (s_Param._nSelectIndex == 2) {
        DOOR_ID nDoorID = (DOOR_ID)2;
        INT nRoomID = _getRoomID(s_Param._nRoomIndex);
        SHORT nFloorNum = _getFloorNum();
        BOOL bStart = FALSE;
        if (nRoomID == 0 || nRoomID == 99) {
            bStart = TRUE;
        }
        WORLD_ID w = (WORLD_ID)_getWorldNumOnFloor();
        WORLD_ID w2 = bStart ? WORLD_ID_COMMON : w;
        CCard::initCard(0, w2, nFloorNum);
        CStatus::initStatus(0, w2, nFloorNum);
        CSceneInfo::setEventDoorUnavailable(s_Param._nFloorIndex, nRoomID);
        CAtel::setFloorCompleteFlag(nFloorNum, nRoomID);
        WORLD_ID startWorld = w;
        if (bStart && nRoomID == 0 && (bRiku || w != WORLD_ID_CASTLE)) {
            startWorld = WORLD_ID_COMMON;
        }
        CSceneInfo::initFloor(s_Param._nFloorIndex, startWorld);
        CSceneInfo::setupCompleteFloorWorld(w);
        RoomInfo* p = bStart ? NULL : CSceneInfo::getRoomInfo(nRoomID);
        FloorLayout* pSrc = _getFloorDataSrc(TRUE);
        ASSERT_LINE(1848, pSrc);
        if (p && p->_pSrcInfo) {
            INT id = -1;
            switch (p->_pSrcInfo->_nRoomType) {
            case 1:
                id = 0;
                break;
            case 2:
                id = 1;
                break;
            case 3:
                id = 2;
                break;
            case 4:
                id = 3;
                break;
            case 5:
                id = 4;
                break;
            default:
                if (!bStart) {
                    id = 1;
                }
                break;
            }
            if (id >= 0) {
                CAtel::setWorldStepFlag(w, id);
            }
            DOOR_ID d = CSceneInfo::setTmpCurrentRoomID(p->_pSrcInfo->_nRoomType);
            if ((UINT)d <= 3) {
                nDoorID = d;
            }
        }
        if (bStart) {
            if (nRoomID == 99) {
                CSceneInfo::openGoalDoor();
            }
        } else {
            RoomInfo* pInfo = CSceneInfo::getEventRoomInfo(0);
            if (pInfo && pInfo->_pStorageInfo) {
                ((RoomStorageInfo*)pInfo->_pStorageInfo)->_bOpen = TRUE;
            }
        }
        if (p && p->_pSrcInfo && p->_pSrcInfo->_nRoomType == ROOM_TYPE_GOAL) {
            CSceneInfo::reqChangeGoalRoom();
        } else {
            CSceneInfo::reqChangeRoom(nRoomID, nDoorID, as._nScene);
        }
    } else {
        CSceneInfo::ignoreFloorInfo();
        CCard::initCard(0, WORLD_ID_COMMON, 0);
        CStatus::initStatus(0, WORLD_ID_COMMON, 0);
        CAtel::setSceneDeSelectInitFlags();
        CSceneInfo::reqChangeScene(as._nArea, as._nScene, -1, 1, s_Param._nAtelNo + 1);
    }
    _updateSceneInfo(nSelApp);
    CMainApp::enableDbgUsrPause(TRUE, FALSE);
}

static inline BOOL _isBattleScene(SHORT nScene) {
    return nScene >= 1000 && nScene < 1199;
}

void CDebugMenuLocal::_updateSceneInfo(INT nSelApp) {
    BOOL bFireGame = (UINT)nSelApp < 2;
    BOOL bBattle = FALSE;
    SHORT nScene = CSceneInfo::getNextSceneNum();
    if (_isBattleScene(nScene) && bFireGame) {
        bBattle = TRUE;
    }
    if (bBattle) {
        SHORT nField;
        switch (nScene) {
        case 1002:
            nField = 20;
            break;
        case 1003:
            nField = 12;
            break;
        case 1004:
            nField = 19;
            break;
        case 1005:
            nField = 26;
            break;
        default:
            nField = 3;
            break;
        }
        CSceneInfo::setNextAreaScene(CSceneInfo::getNextAreaNum(), nField, nScene);
    }
    CGame::dbgStartFromBattle(bBattle);
    CSceneInfo::update();
    if (!bFireGame) {
        CSceneInfo::doneChangeScene();
    }
}

void CDebugMenuLocal::_prepareSaveDebugParam() {
    m_nStep = 2;
    m_nDebugStep = 0;
    if (isButtonDown(8)) {
        m_nDebugStep = 2;
    } else if (isButtonDown(4)) {
        m_nDebugStep = 3;
    } else if (dbgButtonDown((USER)4, 2, 0)) {
        m_nDebugStep = 4;
    }
}

void CDebugMenuLocal::_saveDebugParam() {
    static CBuf* s_pTmpBuf;
    INT i;
    INT j;
    switch (m_nDebugStep) {
    case 0:
        s_Param._iDispCounter = DebugWork::g_dw.sys.m_iDispCounter;
        s_Param._iDagameNumber = DebugWork::g_dw.sys.m_iDagameNumber;
        s_Param._iBGM = DebugWork::g_dw.sys.m_iBGM;
        s_Param._iEffect = DebugWork::g_dw.sys.m_iEffect;
        if (CMemoca::reqSave(s_Param)) {
            _setMemocaMessage(1);
            m_nDebugStep = 1;
        }
        break;
    case 1:
        if (CMemoca::saveProc() != -9999) {
            _returnMainProc();
        }
        break;
    case 2:
    case 3:
        if (CMemoca::reqSaveSysTest(s_Param._nAppIndex == 0, s_Param._nAppIndex == 1, m_nDebugStep == 2)) {
            _setMemocaMessage(2);
            m_nDebugStep = 1;
        }
        break;
    case 4:
        s_pTmpBuf = NEW(1998, CBuf);
        if (CMemoca::reqLoadSysTest(*s_pTmpBuf)) {
            _setMemocaMessage(3);
            m_nDebugStep = 5;
        }
        break;
    case 5:
        if (CMemoca::loadProc() != -9999) {
            BYTE* p = s_pTmpBuf->getBufPtr();
            for (i = 0; i < 5; i++) {
                for (j = 0; j < 16; j++, p++) {
                    TRACE("%02X ", *p);
                }
                TRACE("\n");
            }
            delete s_pTmpBuf;
            _returnMainProc();
        }
        break;
    }
}

void CDebugMenuLocal::_prepareLoadGame() {
    m_nStep = 3;
    m_nDebugStep = 6;
}

void CDebugMenuLocal::_loadGame() {
    switch (m_nDebugStep) {
    case 6:
        NEW_SIZE(2046, 0x50, CSaveLoad)(&m_Parent, SLMODE_LOAD, GAMECLEAR_NONE);
        m_nDebugStep = 7;
        break;
    case 7:
        if (CSaveLoad::isExited()) {
            if (CSaveLoad::isSuccessLoad()) {
                CNowLoading::enable(s_Param._nLoadingMark == 0);
                m_Parent.setVisible(FALSE);
                CSaveLoad::disposeRsrc();
                CDebugMenu::setDebugUser((USER)s_Param._nUserIndex);
                CGame::startFromLoadGame();
                m_nStep = 4;
            } else {
                _returnMainProc();
            }
        }
        break;
    }
}

static BOOL _menuUpDown(INT& nOutV, INT nMax, BOOL bSetSE) {
    BOOL bUpdate = FALSE;
    if (CPadHandler::isButtonRpt(0x408000)) {
        if (nOutV > 0) {
            nOutV--;
        } else {
            nOutV = nMax - 1;
        }
        bUpdate = TRUE;
    } else if (CPadHandler::isButtonRpt(0x802000)) {
        if (nOutV < nMax - 1) {
            nOutV++;
        } else {
            nOutV = 0;
        }
        bUpdate = TRUE;
    }
    if (bUpdate && bSetSE) {
        CSound::playSE(1, -1, 0x40);
    }
    return bUpdate;
}

static BOOL _checkRangeMenu(INT& nOutV, INT nMax) {
    if (nOutV < 0 || nOutV >= nMax) {
        nOutV = 0;
        return TRUE;
    }
    return FALSE;
}

void CDebugMenuLocal::_setMemocaMessage(INT nMesID) {
    CHAR* pszMes;
    switch (nMesID) {
    case 1:
        pszMes = "Now Save Debug-Menu....";
        break;
    case 2:
        pszMes = "Now Save System(test)-Data....";
        break;
    case 3:
        pszMes = "Now Load System(test)-Data....";
        break;
    default:
        pszMes = "Push \xf9V\x81\xa2\xf9" "A Button \x81\xa8 Save This-Menu";
        break;
    }
    m_pStrMemoca->setText(pszMes);
}

void CDebugMenu::setDebugUser(USER inID) {
    s_DebugUser = inID;
}

USER CDebugMenu::getDebugUser() {
    return s_DebugUser;
}

DebugParam& CDebugMenu::getDebugParam() {
    return s_Param;
}

CScreenCheck::CScreenCheck(CTask* pInParent) : CTask(pInParent, PRI_DEFAULT) {
    INT x;
    INT y;
    CTile* pTile;
    _initCreateLine();
    for (x = 0; x < 320; x += 50) {
        pTile = _createLine();
        pTile->m_P0.set((x + 1) << 4, -0xE00);
        pTile->m_P1.set((x - 1) << 4, 0xE00);
        if (x == 0) {
            pTile->m_C0.set(0xFF, 0, 0, 0x80);
        } else {
            pTile->m_C0.set(0x80, 0x80, 0x80, 0x80);
        }
    }
    for (x = -50; x > -320; x -= 50) {
        pTile = _createLine();
        pTile->m_P0.set((x + 1) << 4, -0xE00);
        pTile->m_P1.set((x - 1) << 4, 0xE00);
        pTile->m_C0.set(0x80, 0x80, 0x80, 0x80);
    }
    for (y = 0; y < 224; y += 47) {
        pTile = _createLine();
        pTile->m_P0.set(-0x1400, (y + 1) << 4);
        pTile->m_P1.set(0x1400, (y - 1) << 4);
        if (y == 0) {
            pTile->m_C0.set(0xFF, 0, 0, 0x80);
        } else {
            pTile->m_C0.set(0x80, 0x80, 0x80, 0x80);
        }
    }
    for (y = 47; y < 224; y += 47) {
        pTile = _createLine();
        pTile->m_P0.set(-0x1400, (-y + 1) << 4);
        pTile->m_P1.set(0x1400, (-y - 1) << 4);
        pTile->m_C0.set(0x80, 0x80, 0x80, 0x80);
    }
}

CScreenCheck::~CScreenCheck() {
    INT i;
    for (i = 0; i < m_nLineMax; i++) {
        delete m_pTileLines[i];
        m_pTileLines[i] = NULL;
    }
}

void CScreenCheck::main() {
    if (dbgButtonTrg(USR_SYSTEM, 0xF0, 0)) {
        out();
    }
}

void CScreenCheck::disp() {
    INT i;
    for (i = 0; i < m_nLineMax; i++) {
        m_pTileLines[i]->disp();
    }
}

void CScreenCheck::_initCreateLine() {
    m_nLineMax = 0;
}

CTile* CScreenCheck::_createLine() {
    CTile* pTile = NEW(2233, CTile);
    m_pTileLines[m_nLineMax] = pTile;
    if (pTile) {
        m_nLineMax++;
    }
    return pTile;
}

CColorCheck::CColorCheck(CTask* pInParent) : CTask(pInParent, PRI_DEFAULT) {
    INT i;
    for (i = 0; i < 7; i++) {
        CPolyG4* p = NEW(2256, CPolyG4);
        m_pPolyBar[i] = p;
        p->m_P0.set(-0x12C0, (i * 50 - 200) << 4);
        p->m_P1.set(0x12C0, (i * 50 - 200) << 4);
        p->m_P2.set(-0x12C0, (i * 50 - 150) << 4);
        p->m_P3.set(0x12C0, (i * 50 - 150) << 4);
        switch (i) {
        case 0:
            p->m_C0.set(0xFF, 0xFF, 0xFF, 0x80);
            break;
        case 1:
            p->m_C0.set(0, 0xFF, 0xFF, 0x80);
            break;
        case 2:
            p->m_C0.set(0xFF, 0, 0xFF, 0x80);
            break;
        case 3:
            p->m_C0.set(0xFF, 0xFF, 0, 0x80);
            break;
        case 4:
            p->m_C0.set(0, 0, 0xFF, 0x80);
            break;
        case 5:
            p->m_C0.set(0xFF, 0, 0, 0x80);
            break;
        case 6:
            p->m_C0.set(0, 0xFF, 0, 0x80);
            break;
        case 7:
            p->m_C0.set(0, 0, 0, 0x80);
            break;
        }
        p->m_C2 = p->m_C0;
        p->m_C1.set(0, 0, 0, 0x80);
        p->m_C3 = p->m_C1;
    }
}

CColorCheck::~CColorCheck() {
    INT i;
    for (i = 0; i < 7; i++) {
        delete m_pPolyBar[i];
        m_pPolyBar[i] = NULL;
    }
}

void CColorCheck::main() {
    if (dbgButtonTrg(USR_SYSTEM, 0xF0, 0)) {
        out();
    }
}

void CColorCheck::disp() {
    INT i;
    for (i = 0; i < 7; i++) {
        m_pPolyBar[i]->disp();
    }
}

static BOOL _menuUpDown_SelMode(BOOL bChkRange) {
    INT nSize = 3;
    if (!CSceneInfo::getFloorDataCount(s_Param._nAppIndex != 1)) {
        nSize = 2;
    }
    if (bChkRange) {
        return _checkRangeMenu(s_Param._nSelectIndex, nSize);
    }
    return _menuUpDown(s_Param._nSelectIndex, nSize, TRUE);
}

static BOOL _menuUpDown_App() {
    return _menuUpDown(s_Param._nAppIndex, 22, TRUE);
}

static BOOL _menuUpDown_Localize() {
    return _menuUpDown(s_Param._nLocale, 1, TRUE);
}

static BOOL _menuUpDown_ThisMenu() {
    INT nValue = s_Param._nThisMenu;
    if (_menuUpDown(nValue, 2, TRUE)) {
        s_Param._nThisMenu = nValue;
        return TRUE;
    }
    return FALSE;
}

static BOOL _menuUpDown_LoadMark() {
    INT nValue = s_Param._nLoadingMark;
    if (_menuUpDown(nValue, 2, TRUE)) {
        s_Param._nLoadingMark = nValue;
        return TRUE;
    }
    return FALSE;
}

static CHAR* s_pszWorldNameTbl[14] = {
    "",
    "\x83g\x83\x89\x83\x94\x83@\x81[\x83X\x83^\x83" "E\x83\x93",
    "\x83" "A\x83O\x83\x89\x83o\x81[",
    "\x83I\x83\x8a\x83\x93\x83|\x83X\x83R\x83\x8d\x83V\x83" "A\x83\x80",
    "\x83\x8f\x83\x93\x83_\x81[\x83\x89\x83\x93\x83h",
    "\x83\x82\x83\x93\x83X\x83g\x83\x8d",
    "\x83n\x83\x8d\x83" "E\x83" "B\x83\x93\x83^\x83" "E\x83\x93",
    "\x83" "A\x83g\x83\x89\x83\x93\x83" "e\x83" "B\x83J",
    "\x83l\x83o\x81[\x83\x89\x83\x93\x83h",
    "\x83z\x83\x8d\x83" "E\x83o\x83X\x83" "e\x83" "B\x83I\x83\x93",
    "100\x83G\x81[\x83J\x81[\x82\xcc\x83\x82\x83\x8a",
    "\x83g\x83\x8f\x83" "C\x83\x89\x83" "C\x83g\x83^\x83" "E\x83\x93",
    "\x83" "f\x83X\x83" "e\x83" "B\x83j\x81[\x83" "A\x83" "C\x83\x89\x83\x93\x83h",
    "\x83{\x83" "E\x83L\x83\x83\x83N\x82\xcc\x83V\x83\x8d",
};

static CHAR* s_pszWorldPrefixTbl[14] = {
    "", "tr", "ag", "ol", "wo", "mo", "ha", "at", "ne", "ho", "po", "tw", "de", "ca",
};

static CHAR* s_pszSelectMode[3] = {
    "\x83V\x81[\x83\x93" "de\x83Z\x83\x8c\x83N\x83g",
    "\x83}\x83" "b\x83vde\x83Z\x83\x8c\x83N\x83g",
    "\x83\x8b\x81[\x83\x80" "de\x83Z\x83\x8c\x83N\x83g",
};

static CHAR* s_pszAppTable[22] = {
    "GAME  - Sora -",
    "GAME  - Riku -",
    "\x83L\x83V\x83\x8f\x83_ \x83" "e\x83X\x83g",
    "\x83L\x83N\x83` \x83" "e\x83X\x83g",
    "\x83S\x83g\x83" "E \x83" "e\x83X\x83g",
    "\x83R\x83" "E \x83" "e\x83X\x83g",
    "\x83i\x83J\x83~\x83` \x83" "e\x83X\x83g",
    "\x83}\x83" "c\x83I\x83J \x83" "e\x83X\x83g",
    "\x83J\x83^\x83I\x83J \x83" "e\x83X\x83g",
    "\x83J\x83n\x83\x89 \x83" "e\x83X\x83g",
    "\x83i\x83J\x83V\x83} \x83" "e\x83X\x83g",
    "SCREEN CHECK",
    "COLOR CHECK",
    "\x83}\x83" "b\x83v\x83r\x83\x85\x81[\x83" "A\x81[",
    "\x83\x82\x83" "f\x83\x8b\x83r\x83\x85\x81[\x83" "A\x81[",
    "EFFECT VIEWER",
    "\x83R\x83\x8a\x83W\x83\x87\x83\x93\x83G\x83" "f\x83" "B\x83^",
    "\x83n\x83" "C\x83`\x83G\x83" "f\x83" "B\x83^",
    "2D VIEWER",
    "ATTACH EDITOR",
    "FONT VIEWER",
    "MOVIE VIEWER",
};

static CHAR* s_pszLocale[7] = {
    "JP\x83j\x83z\x83\x93",
    "US\x83" "A\x83\x81\x83\x8a\x83J",
    "UK\x83" "C\x83M\x83\x8a\x83X",
    "FR\x83t\x83\x89\x83\x93\x83X",
    "SP\x83X\x83y\x83" "C\x83\x93",
    "GE\x83h\x83" "C\x83" "c",
    "IT\x83" "C\x83^\x83\x8a\x83" "A",
};

static CHAR* s_pszThisMenu[2] = {"KEEP", "RELOAD"};
static CHAR* s_pszLoadMark[2] = {"ON", "OFF"};

// SOURCE DATA: .data .rodata .bss
