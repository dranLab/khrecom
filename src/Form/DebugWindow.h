// PARTIAL: added for Game/Player/PlayerManager
#ifndef FORM_DEBUGWINDOW_H
#define FORM_DEBUGWINDOW_H

#include "khtypes.h"
#include "System/Task.h"

class CDebugWindow;
struct DEBUG_WINDOW_TBL;

struct DEBUG_WINDOW_LINE {
    LPCSTR szString;
    void* pValue;
    UBYTE iValueSize;
    unsigned char pad[1];
    USHORT iFlags;
    FLOAT iVMin;
    FLOAT iVMax;
    FLOAT iVPlus;
    LPCSTR* szValueStr;
    UINT iMask;
    BOOL (*pFunc)(CDebugWindow* pObj, INT inY, INT inParam);
    DEBUG_WINDOW_TBL* pChild;
};

struct DEBUG_WINDOW_TBL {
    LPCSTR szTitle;
    INT iWx;
    INT iWy;
    INT iWw;
    INT iWh;
    UBYTE iStrPitch;
    UBYTE iCurY;
    USHORT iFlags;
    DEBUG_WINDOW_LINE* pLineTbl;
    INT (*pDispFunc)(CDebugWindow* pObj);
    INT (*pAllFunc)(CDebugWindow* pObj);
};

class CDebugWindow : public CTask {
private:
    UBYTE _m_0x40[0x180]; /* 0x040 .. 0x1bc */

public:
    static BYTE none;

    CDebugWindow(CTask* pParent, const DEBUG_WINDOW_TBL& inTbl);
    virtual ~CDebugWindow();
    BOOL sendMessage(INT iMes, INT iParam);
    void dispOn(INT inParam);
    void dispOff(INT inParam);
};

// Form/DebugWindowTBL.h in the original
namespace DWT {
extern char* szOnOff[2];
extern char* szOffOn[2];
}

#endif /* FORM_DEBUGWINDOW_H */
