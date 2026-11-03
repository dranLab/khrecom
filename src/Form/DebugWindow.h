#ifndef FORM_DEBUGWINDOW_H
#define FORM_DEBUGWINDOW_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/DebugProcInfo.h"
#include "System/Graphic/Camera.h"

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
    INT (*pDispFunc)(LPSTR pStr, INT iLineY, void* pValue);
    INT (*pAllFunc)(CDebugWindow* pObj, INT inParam);
};

struct DEBUG_WINDOW_LINE_BUF {
    CFnt fntString;
    char szString[128];
};

struct DEBUG_WINDOW_LINE_CURX {
    SBYTE iCurX;
    UBYTE iPosX;
    UBYTE iCurWidth;
    SBYTE iNextWidth;
    UBYTE iMax;
};


class CDebugWindow : public CTask {
private:
    UINT m_iStatus;
    INT m_iStep;
    INT m_iAnswer;
    CFnt m_fntTitle;
    DEBUG_WINDOW_LINE_BUF* m_pWinLine;
    DEBUG_WINDOW_LINE_CURX* m_pWinCurX;
    const DEBUG_WINDOW_TBL* m_pWinData;
    DEBUG_WINDOW_LINE* m_pWinLineData;
    const char* m_pStr;
    INT m_iWx;
    INT m_iWy;
    INT m_iWw;
    INT m_iWh;
    INT m_iPitch;
    INT (*m_pDispFunc)(LPSTR pStr, INT iLineY, void* pValue);
    INT (*m_pAllFunc)(CDebugWindow* pObj, INT inParam);
    INT m_iCurY;
    INT m_iOldCurY;
    INT m_iCurMax;
    INT m_iEffCurMax;
    INT m_iLineTop;
    HANDLE m_hChild;
    CDebugWindow* m_pParent;
    CTile m_gpFrame;
    CTile m_gpTitle;
    CTile m_gpCur;
    static CDebugWindow* m_pInstance;
    static BOOL m_bTopOpen;

public:
    static BYTE none;

    CDebugWindow(CTask* pParent, const DEBUG_WINDOW_TBL& inTbl);
    CDebugWindow(CDebugWindow* pParent, const DEBUG_WINDOW_TBL& inTbl);
    CDebugWindow(CTask* pParent, const DEBUG_WINDOW_TBL& inTbl, INT iTop);
    CDebugWindow(CTask* pParent, const char* pStr);
    CDebugWindow(CDebugWindow* pParent, const char* pStr);
    virtual ~CDebugWindow();
    virtual void out();
    virtual void main();
    virtual void disp();
    BOOL sendMessage(INT iMes, const DEBUG_WINDOW_TBL* pWin);
    BOOL sendMessage(INT iMes, const char* pStr);
    BOOL sendMessage(INT iMes, INT iParam);
    BOOL isChildAnswer();
    INT getChildAnswer();
    INT getCurX();
    BOOL initCurX(UBYTE iPosX, UBYTE iCurWidth, SBYTE iNextWidth, UBYTE iMax);
    BOOL addValue(INT iPM, const FLOAT* pVMin, const FLOAT* pVMax, const FLOAT* pVPlus, void* pValue);
    void sleepCur();
    void wakeupCur();
    void dispOn(INT iLevel);
    void dispOff(INT iLevel);
    static void openTopDebugWindow(CTask* p);
    static void closeTopDebugWindow();
    static BOOL isTopDebugWindow();
    static void enableTopDebugWindow(BOOL bFlg);

private:
    void mainProc(INT* iParam);
    INT getY(INT iY);
    INT getY();
    BOOL isActive();
    INT isAction(INT iY);
    void subCurYAfter();
    void addCurYAfter();
    void moveCurX(INT iPM);
    BOOL openChild(const DEBUG_WINDOW_TBL* pWTBL, const char* pStr);
    void init(const DEBUG_WINDOW_TBL& data);
};


class CNakCamera : public CCamera {
private:
    INT m_iType;
    INT m_iPad;
    UINT m_iStatus;
    void** m_pVoid;
    FVECTOR m_vCamMove;
    FVECTOR m_vCamLook;
    FVECTOR m_vCamLookAim;
    FVECTOR m_vCamPos;
    FVECTOR m_vCamPosAim;
    FVECTOR m_vCamRot;
    FVECTOR m_vCamRotAim;
    FLOAT m_fDistance;
    FLOAT m_fDistanceAim;
    INT m_iUpdate;
    CDebugWindow* m_pWin;
    C3DLines m_3DLines;

public:
    CNakCamera(CTask* p, INT iPad, UINT iFlg, FVECTOR* pvPos, FLOAT fDirY, void** pVoid);
    virtual ~CNakCamera();
    virtual void main();
    virtual void disp();
    virtual INT getTypeID() const;
    void ChangeType(INT iType);
    INT GetType() { return m_iType; }
    void Action(UINT iBit);
    void GetCameraLook(FVECTOR& vLook) { vLook = m_vCamLook; }
    void GetCameraPos(FVECTOR& vPos) { vPos = m_vCamPos; }
    void GetCameraRot(FVECTOR& vRot) { vRot = m_vCamRot; }
    FLOAT GetCameraDistance() { return m_fDistance; }
    void SetCameraLook(const FVECTOR& vLook) {
        m_vCamLookAim = m_vCamLook = vLook;
        m_vCamLook.w = 1.0f;
        m_vCamLookAim.w = 1.0f;
    }
    void SetCameraPos(const FVECTOR& vPos) {
        m_vCamPosAim = m_vCamPos = vPos;
        m_vCamPos.w = 1.0f;
        m_vCamPosAim.w = 1.0f;
    }
    void SetCameraRot(const FVECTOR& vRot) {
        m_vCamRotAim = m_vCamRot = vRot;
        m_vCamRot.w = 1.0f;
        m_vCamRotAim.w = 1.0f;
    }
    void SetCameraDistance(FLOAT fDis) { m_fDistanceAim = m_fDistance = fDis; }
    void SetCameraLookAim(const FVECTOR& vLook) {
        m_vCamLookAim = vLook;
        m_vCamLookAim.w = 1.0f;
    }
    void SetCameraPosAim(const FVECTOR& vPos) {
        m_vCamPosAim = vPos;
        m_vCamPosAim.w = 1.0f;
    }
    void SetCameraRotAim(const FVECTOR& vRot) {
        m_vCamRotAim = vRot;
        m_vCamRotAim.w = 1.0f;
    }
    void SetCameraDistanceAim(FLOAT fDis) { m_fDistanceAim = fDis; }
    void SetDebugViewAngle(FLOAT fDist);

private:
    void init(INT iPad, UINT iFlg, FVECTOR* pvPos, FLOAT fDirY, void** pVoid);
    void LookAngleDistanceToPos();
    void PosAngleDistanceToLook();
    void LookPosToAngleDistance();
    void Update();
    void SubjectivityMove();
    void SubjectivityUpdate();
    void ObjectivityMove();
    void ObjectivityUpdate();
    void _setResetPos(const FVECTOR* pvLookPos, const FVECTOR* pvCamPos, FLOAT fDirY);
};

class CGlidWorld : public CTask {
private:
    INT m_iStatus;
    C3DLineStrip m_3DLines;

public:
    CGlidWorld(CTask* p);
    virtual ~CGlidWorld();
    virtual void main();
    virtual void disp();
    void setDisp(BOOL bOn) {
        if (bOn) {
            m_iStatus |= 1;
        } else {
            m_iStatus &= ~1;
        }
    }
};


// Form/DebugWindowTBL.h in the original
namespace DWT {
extern char* szOnOff[2];
extern char* szOffOn[2];
}

#endif /* FORM_DEBUGWINDOW_H */
