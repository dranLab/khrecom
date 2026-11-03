#ifndef FORM_DEBUGMENU_H
#define FORM_DEBUGMENU_H

#include "khtypes.h"
#include "System/Debug.h"
#include "System/Task.h"
#include "System/PadHandler.h"

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


class CDebugMenuLocal;
class CTile;
class CPolyG4;

class CDebugMenu : public CTask {
private:
    CDebugMenuLocal* m_pL;

public:
    CDebugMenu(CTask* pInParent);
    virtual ~CDebugMenu();
    virtual void main();
    virtual void disp();
    static void setDebugUser(USER inID);
    static USER getDebugUser();
    static DebugParam& getDebugParam();
    static void initParam(INT nLocale);
    static BOOL startLoadFromMemoca();
    static BOOL isCompleteLoadFromMemoca();
};

class CScreenCheck : public CTask, public CPadHandler {
private:
    CTile* m_pTileLines[30];
    INT m_nLineMax;

public:
    CScreenCheck(CTask* pInParent);
    virtual ~CScreenCheck();
    virtual void main();
    virtual void disp();

private:
    void _initCreateLine();
    CTile* _createLine();
};

class CColorCheck : public CTask, public CPadHandler {
private:
    CPolyG4* m_pPolyBar[7];

public:
    CColorCheck(CBfObj* pInParent);
    virtual ~CColorCheck();
    virtual void main();
    virtual void disp();
};


#endif // FORM_DEBUGMENU_H
