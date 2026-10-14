// PARTIAL: added for System/Debug
#ifndef FORM_DEBUGMENU_H
#define FORM_DEBUGMENU_H

#include "khtypes.h"
#include "System/Debug.h"

class CTask; // PARTIAL: added for System/MainTaskFactory

// PARTIAL: added for System/RootTask
struct DebugParam {
    INT _nUserIndex;
    INT _nAppIndex;
    INT _nSelectIndex;
    INT _nAreaIndex;
    SHORT _nArea;
    SHORT _nScene;
    SHORT _nGroup;
    SHORT _nMember;
    INT _nMapToAreaIndex;
    INT _nAtelNo;
    INT _nFloorIndex;
    INT _nLocale;
    SHORT _nThisMenu;
    SHORT _nLoadingMark;
    SBYTE _iDispCounter;
    SBYTE _iBGM;
    INT _iDagameNumber;
    INT _iEffect;
    INT _nRoomIndex;
    INT _nSceneIndexInRoom;
};

class CDebugMenu {
public:
    CDebugMenu(CTask* pParent); // PARTIAL: added for System/MainTaskFactory
    static USER getDebugUser();
    static DebugParam& getDebugParam(); // PARTIAL: added for System/RootTask
    static void initParam(INT nUser);
    static BOOL startLoadFromMemoca();
    static BOOL isCompleteLoadFromMemoca();
};

// PARTIAL: added for System/MainTaskFactory (CTask-derived; base omitted so the header need not include Task.h)
class CColorCheck {
public:
    CColorCheck(CTask* pParent);
};

// PARTIAL: added for System/MainTaskFactory (CTask-derived; base omitted so the header need not include Task.h)
class CScreenCheck {
public:
    CScreenCheck(CTask* pParent);
};

#endif // FORM_DEBUGMENU_H
