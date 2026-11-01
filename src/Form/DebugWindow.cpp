/* .rodata starts with the strings of the included headers' inline functions, in this order. */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Str.h"
#include "System/Task.h"
#include "System/TaskMan.h"
#include "System/Debug.h"
#include "System/DebugProcInfo.h"
#include "System/Graphic/Camera.h"
#include "System/Graphic/DeFocus.h"
#include "System/Graphic/WorldEnv.h"
#include "Game/Debug/DebugWork.h"
#define FORM_DEBUGWINDOW_FULL /* MATCH: the owner sees the full classes (Gprim.h included first) */
#include "Form/DebugWindow.h"
#include "include_asm.h"

extern "C" size_t strlen(const char* s);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" void* memset(void* p, int c, size_t n);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" float atan2f(float y, float x);
extern "C" void sceVu0RotMatrixX(FMATRIX& m0, FMATRIX& m1, float rx);
extern "C" void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
extern "C" void sceVu0RotMatrixZ(FMATRIX& m0, FMATRIX& m1, float rz);

// System/Coord.h
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
FLOAT InnerProduct(const FVECTOR& v0);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
namespace Math3D {
FLOAT PointToPointDistance(const FVECTOR& inP0, const FVECTOR& inP1);
FLOAT LimitPI(FLOAT pai);
}

// Form/DebugWindowTBL.h
namespace DWT {
extern DEBUG_WINDOW_TBL debugWinTop;
}

static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}

static inline void _unitMatrix(FMATRIX& m) {
    asm volatile("vmr32.xyzw $vf1, $vf0\n"
                 "sqc2 $vf0, 0x30(%0)\n"
                 "vmr32.xyzw $vf2, $vf1\n"
                 "sqc2 $vf1, 0x20(%0)\n"
                 "vmr32.xyzw $vf3, $vf2\n"
                 "sqc2 $vf2, 0x10(%0)\n"
                 "sqc2 $vf3, 0x0(%0)\n"
                 :
                 : "r"(&m)
                 : "memory");
}

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new(size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}
inline void* operator new[](size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}
inline void operator delete[](void* mem) { com_free(mem); }

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

BYTE CDebugWindow::none = 0;
CDebugWindow* CDebugWindow::m_pInstance = NULL;
BOOL CDebugWindow::m_bTopOpen = TRUE;

static BOOL debugMessageBoxLineCall(CDebugWindow* pObj, INT inY, INT inParam);

static DEBUG_WINDOW_LINE debugMessageBoxLine[2] = {
    {"", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, debugMessageBoxLineCall, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugMessageBox = {"Check", 0, -32, 1, 3, 12, 0, 6, debugMessageBoxLine, NULL, NULL};

static BOOL debugMessageBoxLineCall(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x60) {
        pObj->sendMessage(2, 0);
    }
    return TRUE;
}

CDebugWindow::CDebugWindow(CTask* p, const DEBUG_WINDOW_TBL& data) : CTask(p, 0x4000) {
    m_pParent = NULL;
    m_iStatus = 0;
    m_pStr = NULL;
    init(data);
}

CDebugWindow::CDebugWindow(CDebugWindow* p, const DEBUG_WINDOW_TBL& data) : CTask(p, 0x4000) {
    m_pParent = p;
    m_iStatus = 0;
    m_pStr = NULL;
    init(data);
}

CDebugWindow::CDebugWindow(CTask* p, const DEBUG_WINDOW_TBL& data, INT iTop) : CTask(p, 0x4000) {
    m_pParent = NULL;
    m_iStatus = 0;
    if (iTop) {
        m_iStatus = 0x10000;
    }
    m_pStr = NULL;
    init(data);
}

CDebugWindow::CDebugWindow(CTask* p, const char* pStr) : CTask(p, 0x4000) {
    m_pStr = pStr;
    m_pParent = NULL;
    m_iStatus = 0;
    init(debugMessageBox);
}

CDebugWindow::CDebugWindow(CDebugWindow* p, const char* pStr) : CTask(p, 0x4000) {
    m_pParent = p;
    m_pStr = pStr;
    m_iStatus = 0;
    init(debugMessageBox);
}

void CDebugWindow::out() {
    INT iCurYBak = m_iCurY;
    INT i;
    m_iCurY = 0;
    for (i = 0; i < m_iCurMax && m_pWinLineData[getY(i)].szString; i++) {
        if (isAction(i) & 4) {
            m_pWinLineData[getY(i)].pFunc(this, i, 2);
        }
        m_iCurY++;
    }
    m_iCurY = iCurYBak;
    if (m_pAllFunc) {
        m_pAllFunc(this, 2);
    }
    CTask::out();
}

CDebugWindow::~CDebugWindow() {
    if (m_pWinLine) {
        delete[] m_pWinLine;
    }
    m_pWinLine = NULL;
    if (m_pWinCurX) {
        com_free(m_pWinCurX);
    }
    m_pWinCurX = NULL;
    if (m_iStatus & 0x10000) {
        if (this == m_pInstance) {
            m_pInstance = NULL;
        }
    }
}

void CDebugWindow::init(const DEBUG_WINDOW_TBL& data) {
    INT i;
    INT iMaxStrSize = 1;
    BOOL bCurX = FALSE;
    INT iCurY = 0;
    INT iCurYBak;

    if (data.iFlags & 0x40) {
        iCurY = data.iCurY;
    }
    m_pWinData = &data;
    ignorePause(TRUE);
    ignorePlaySpeed(TRUE);
    m_pWinLineData = data.pLineTbl;
    m_pWinLine = NULL;
    m_pWinCurX = NULL;
    m_pDispFunc = data.pDispFunc;
    m_pAllFunc = data.pAllFunc;
    m_hChild = 0;
    m_iWx = data.iWx;
    m_iWy = data.iWy;
    m_iPitch = data.iStrPitch;
    m_iCurY = iCurY;
    m_iOldCurY = iCurY;
    m_iStatus |= data.iFlags;
    m_iStep = 0;
    if (m_pStr) {
        m_iStatus |= 0x30;
    }
    m_gpFrame.m_C0.set(0, 0, 0, 0x60);
    m_gpFrame.setSemiTrans(ALPHA_BLEND);
    m_gpFrame.setLayerFG(0x18);
    m_gpTitle.m_C0.set(0, 0, 0x80, 0x60);
    m_gpTitle.setSemiTrans(ALPHA_BLEND);
    m_gpTitle.setLayerFG(0x18);
    m_gpCur.m_C0.set(0x80, 0x80, 0, 0x60);
    m_gpCur.setSemiTrans(ALPHA_BLEND);
    m_gpCur.setLayerFG(0x18);
    m_fntTitle.setText(m_pWinData->szTitle);
    m_fntTitle.setBrightness(0x80, 0x80, 0, 0x80);
    m_fntTitle.setStyle(2);
    m_fntTitle.setSize(m_iPitch);
    m_fntTitle.setLayerFG(0x18);
    m_iLineTop = 0;
    m_iCurMax = 0;
    m_iEffCurMax = 0;
    if (m_iStatus & 8) {
        if (m_pAllFunc) {
            m_iCurMax = m_pAllFunc(this, 1);
        }
        for (i = 0; i < m_iCurMax; i++) {
            if (!m_pWinLineData[getY(i)].szString) {
                m_iCurMax = 0;
                break;
            }
            if (isAction(i) & 2) {
                m_iEffCurMax++;
                if (isAction(i) & 0x20) {
                    bCurX = TRUE;
                }
            }
        }
    } else {
        if (m_pAllFunc) {
            m_pAllFunc(this, 1);
        }
        for (i = 0; m_pWinLineData[getY(i)].szString; i++, m_iCurMax++) {
            if (isAction(i) & 2) {
                m_iEffCurMax++;
                if (isAction(i) & 0x20) {
                    bCurX = TRUE;
                }
            }
        }
    }
    if (m_iCurMax < m_pWinData->iWh) {
        m_iWh = m_iCurMax;
    } else {
        m_iWh = m_pWinData->iWh;
    }
    m_iWw = m_pWinData->iWw;
    if (m_iCurMax <= 0) {
        m_iStatus |= 0x40000;
        return;
    }
    if (m_iCurY >= m_iCurMax) {
        m_iCurY = 0;
    }
    if (bCurX) {
#line 269
        m_pWinCurX = (DEBUG_WINDOW_LINE_CURX*)com_malloc_d(m_iCurMax * sizeof(DEBUG_WINDOW_LINE_CURX), __FILE__ "(269)", RET_ADDR());
        ASSERT(m_pWinCurX);
        for (i = 0; i < m_iCurMax; i++) {
            m_pWinCurX[i].iCurX = -1;
        }
    }
#line 277
    m_pWinLine = new (__FILE__ "(277)") DEBUG_WINDOW_LINE_BUF[m_iWh];
    ASSERT(m_pWinLine);
    for (i = 0; i < m_iWh; i++) {
        INT iSize = (strlen(m_pWinLineData[getY(i)].szString) + 1) / 2;
        if (iMaxStrSize < iSize) {
            iMaxStrSize = iSize;
        }
        strcpy(m_pWinLine[i].szString, m_pWinLineData[getY(i)].szString);
        m_pWinLine[i].fntString.setText(m_pWinLine[i].szString);
        m_pWinLine[i].fntString.setBrightness(0x80, 0x80, 0x80, 0x80);
        if (m_iStatus & 0x10) {
            m_pWinLine[i].fntString.setStyle(2);
        } else {
            m_pWinLine[i].fntString.setStyle(0);
        }
        m_pWinLine[i].fntString.setSize(m_iPitch);
        m_pWinLine[i].fntString.setLayerFG(0x18);
    }
    iCurYBak = m_iCurY;
    m_iCurY = 0;
    for (i = 0; i < m_iCurMax; i++) {
        if (isAction(i) & 4) {
            m_pWinLineData[getY(i)].pFunc(this, i, 1);
        }
        m_iCurY++;
    }
    m_iCurY = iCurYBak;
    if (m_pStr) {
        iMaxStrSize = (strlen(m_pStr) + 1) / 2;
    }
    if (m_iStatus & 0x10) {
        if (m_iWw < iMaxStrSize) {
            m_iWw = iMaxStrSize;
        }
    }
    if (m_iStatus & 0x20) {
        m_iWx = -(m_iWw * m_iPitch >> 1);
    }
    if (m_iEffCurMax > 0) {
        addCurYAfter();
    }
}

static INT __getLowBit(UINT iBit) {
    INT i;
    for (i = 0; i < 32; i++, iBit >>= 1) {
        if (iBit & 1) {
            break;
        }
    }
    return i < 32 ? i : -1;
}

INT CDebugWindow::getY() {
    if (m_iStatus & 8) {
        return 0;
    }
    return m_iCurY;
}

INT CDebugWindow::getY(INT iY) {
    return (m_iStatus & 8) ? 0 : iY;
}

template <class T, class U>
static BOOL __addValue(T& outParam, U add, U max, U min, INT iFlag, UINT iMask) {
    BOOL bRet = FALSE;
    T param = outParam;
    T paramOld;
    INT bit = __getLowBit(iMask);
    unsigned int iParam[2];
    if (bit >= 0) {
        iParam[0] = param;
        param = (iParam[0] & iMask) >> bit;
    }
    paramOld = param;
    if (iFlag & 0x10) {
        if (add >= 0) {
            param *= add;
        } else {
            param /= -add;
        }
    } else {
        param += add;
    }
    if (param >= paramOld) {
        if (param > max) {
            bRet = TRUE;
            if (iFlag & 8) {
                param = max;
            } else {
                param = min;
            }
        }
    } else if (param < min) {
        bRet = TRUE;
        if (iFlag & 8) {
            param = min;
        } else {
            param = max;
        }
    }
    if (bit >= 0) {
        iParam[0] = param;
        iParam[1] = outParam;
        outParam = (iParam[1] & ~iMask) | ((iParam[0] << bit) & iMask);
    } else {
        outParam = param;
    }
    return bRet;
}

BOOL CDebugWindow::addValue(INT iPM, const FLOAT* pVMin, const FLOAT* pVMax, const FLOAT* pVPlus, void* pValue) {
    INT iCurY = getY();
    if (!pVMin) {
        pVMin = &m_pWinLineData[iCurY].iVMin;
    }
    if (!pVMax) {
        pVMax = &m_pWinLineData[iCurY].iVMax;
    }
    if (!pVPlus) {
        pVPlus = &m_pWinLineData[iCurY].iVPlus;
    }
    if (!pValue) {
        pValue = m_pWinLineData[iCurY].pValue;
    }
    switch (m_pWinLineData[iCurY].iValueSize) {
    case 1:
    default:
        __addValue(*(SBYTE*)pValue, iPM >= 0 ? (INT)*pVPlus : -(INT)*pVPlus, (INT)*pVMax, (INT)*pVMin,
                   m_pWinLineData[iCurY].iFlags, m_pWinLineData[iCurY].iMask);
        break;
    case 2:
        __addValue(*(SHORT*)pValue, iPM >= 0 ? (INT)*pVPlus : -(INT)*pVPlus, (INT)*pVMax, (INT)*pVMin,
                   m_pWinLineData[iCurY].iFlags, m_pWinLineData[iCurY].iMask);
        break;
    case 4:
        if (m_pWinLineData[iCurY].iFlags & 1) {
            __addValue(*(FLOAT*)pValue, iPM >= 0 ? *pVPlus : -*pVPlus, *pVMax, *pVMin, m_pWinLineData[iCurY].iFlags,
                       0);
        } else {
            __addValue(*(INT*)pValue, iPM >= 0 ? (INT)*pVPlus : -(INT)*pVPlus, (INT)*pVMax, (INT)*pVMin,
                       m_pWinLineData[iCurY].iFlags, m_pWinLineData[iCurY].iMask);
        }
        break;
    }
    return TRUE;
}

BOOL CDebugWindow::isActive() {
    if (m_hChild) {
        CTask* p = CTaskMan::getTask(m_hChild);
        if (p == NULL || p->isOut()) {
            m_hChild = 0;
        } else {
            return FALSE;
        }
    }
    return TRUE;
}

INT CDebugWindow::isAction(INT iY) {
    INT iRet;
    INT y = getY(iY);
    iRet = m_pWinLineData[y].pValue != &none;
    if (!(m_pWinLineData[y].iFlags & 2)) {
        iRet |= 2;
    }
    if (m_pWinLineData[y].pChild) {
        iRet |= 8;
    }
    if (m_pWinLineData[y].pFunc) {
        iRet |= 4;
    }
    if (!(m_pWinLineData[y].iFlags & 4) && m_pWinLineData[y].iVMax > m_pWinLineData[y].iVMin &&
        m_pWinLineData[y].iVPlus != 0.0f) {
        iRet |= 0x10;
    }
    if (m_pWinLineData[y].iFlags & 0x40) {
        iRet |= 0x20;
    }
    return iRet;
}

void CDebugWindow::subCurYAfter() {
    while (1) {
        if (m_iCurY < 0) {
            if (m_iStatus & 1) {
                m_iLineTop = 0;
                m_iCurY = m_iOldCurY;
            } else {
                m_iCurY = m_iCurMax - 1;
                if (m_iCurMax >= m_iWh) {
                    m_iLineTop = m_iCurMax - m_iWh;
                }
            }
        } else if (m_iLineTop > 0 && m_iCurY < m_iLineTop + 1) {
            m_iLineTop = m_iLineTop - 1;
        }
        if (isAction(m_iCurY) & 2) {
            break;
        }
        m_iCurY--;
    }
}

void CDebugWindow::addCurYAfter() {
    while (1) {
        if (m_iCurY >= m_iCurMax) {
            if (m_iStatus & 1) {
                m_iCurY = m_iOldCurY;
            } else {
                m_iCurY = 0;
                m_iLineTop = 0;
            }
        } else if (m_iLineTop + m_iWh < m_iCurMax && m_iCurY >= m_iLineTop + m_iWh - 1) {
            m_iLineTop = m_iLineTop + 1;
        }
        if (isAction(m_iCurY) & 2) {
            break;
        }
        m_iCurY++;
    }
}

void CDebugWindow::moveCurX(INT iPM) {
    if (isAction(m_iCurY) & 0x20) {
        if (m_pWinCurX) {
            if (m_pWinCurX[m_iCurY].iCurX >= 0) {
                if (m_pWinCurX[m_iCurY].iNextWidth < 0) {
                    iPM = -iPM;
                }
                m_pWinCurX[m_iCurY].iCurX += iPM;
                if (m_pWinCurX[m_iCurY].iCurX >= m_pWinCurX[m_iCurY].iMax) {
                    m_pWinCurX[m_iCurY].iCurX = 0;
                }
                if (m_pWinCurX[m_iCurY].iCurX < 0) {
                    m_pWinCurX[m_iCurY].iCurX = m_pWinCurX[m_iCurY].iMax - 1;
                }
            }
        }
    }
}

BOOL CDebugWindow::openChild(const DEBUG_WINDOW_TBL* pWTBL, const char* pStr) {
    BOOL bRet = FALSE;
    if (!CTaskMan::getTask(m_hChild)) {
        CTask* p;
        if (pWTBL) {
#line 615
            p = new (__FILE__ "(615)") CDebugWindow(this, *pWTBL);
        } else if (pStr) {
#line 617
            p = new (__FILE__ "(617)") CDebugWindow(this, pStr);
        } else {
            return FALSE;
        }
        if (p) {
            m_hChild = p->getHandle();
        }
        m_iStatus &= ~0x200000;
        bRet = TRUE;
    }
    return bRet;
}

BOOL CDebugWindow::sendMessage(INT iMes, const DEBUG_WINDOW_TBL* pWin) {
    BOOL bRet = FALSE;
    if (iMes == 1) {
        bRet = openChild(pWin, NULL);
    }
    return bRet;
}

BOOL CDebugWindow::sendMessage(INT iMes, const char* pStr) {
    BOOL bRet = FALSE;
    if (iMes == 1) {
        bRet = openChild(NULL, pStr);
    }
    return bRet;
}

BOOL CDebugWindow::sendMessage(INT iMes, INT iParam) {
    BOOL bRet = FALSE;
    switch (iMes) {
    case 2:
        if (iParam) {
            m_iStatus |= 0x80000;
        } else {
            m_iStatus |= 0x40000;
        }
        bRet = TRUE;
        break;
    case 3:
        if (m_pParent) {
            m_pParent->m_iAnswer = iParam;
            bRet = TRUE;
            m_pParent->m_iStatus |= 0x200000;
        }
        break;
    case 4:
        if (iParam) {
            m_iStatus |= 0x1000000;
        } else {
            m_iStatus |= 0x800000;
        }
        bRet = TRUE;
        break;
    }
    return bRet;
}

void CDebugWindow::sleepCur() { m_iStatus |= 0x400000; }

void CDebugWindow::wakeupCur() { m_iStatus &= ~0x400000; }

void CDebugWindow::dispOn(INT iLevel) {
    m_iStatus &= ~0x20000;
    if (--iLevel >= 0 && m_pParent) {
        m_pParent->dispOn(iLevel);
    }
}

void CDebugWindow::dispOff(INT iLevel) {
    m_iStatus |= 0x20000;
    if (--iLevel >= 0 && m_pParent) {
        m_pParent->dispOff(iLevel);
    }
}

BOOL CDebugWindow::isChildAnswer() { return m_iStatus & 0x200000; }

INT CDebugWindow::getChildAnswer() { return m_iAnswer; }

INT CDebugWindow::getCurX() {
    INT iRet = -1;
    if (m_pWinCurX && (isAction(m_iCurY) & 0x20)) {
        iRet = m_pWinCurX[m_iCurY].iCurX;
    }
    return iRet;
}

BOOL CDebugWindow::initCurX(UBYTE iPosX, UBYTE iCurWidth, SBYTE iNextWidth, UBYTE iMax) {
    BOOL bRet = FALSE;
    if (m_pWinCurX && (isAction(m_iCurY) & 0x20)) {
        bRet = TRUE;
        m_pWinCurX[m_iCurY].iCurX = 0;
        m_pWinCurX[m_iCurY].iPosX = iPosX;
        m_pWinCurX[m_iCurY].iCurWidth = iCurWidth;
        m_pWinCurX[m_iCurY].iNextWidth = iNextWidth;
        m_pWinCurX[m_iCurY].iMax = iMax;
    }
    return bRet;
}

void CDebugWindow::main() {
    int iParam[2];
    if (!m_iStep) {
        m_iStep = 1;
        return;
    }
    memset(iParam, 0, sizeof(iParam));
    m_iOldCurY = m_iCurY;
    if (!isActive()) {
        m_iStatus |= 0x100000;
        return;
    }
    if (m_iStatus & 0x100000) {
        m_iStatus &= ~0x100000;
        iParam[0] |= 0x800;
    }
    if (!(m_iStatus & 0xC0000)) {
        mainProc(iParam);
    }
    if (m_iOldCurY != m_iCurY && (iParam[1] & 8) && (isAction(m_iOldCurY) & 4)) {
        m_pWinLineData[getY(m_iOldCurY)].pFunc(this, m_iOldCurY, iParam[1]);
    }
    if (isAction(m_iCurY) & 4) {
        m_pWinLineData[getY()].pFunc(this, m_iCurY, iParam[0]);
    }
    if (m_pAllFunc) {
        m_pAllFunc(this, 0);
    }
    if (m_iStatus & 0xC0000) {
        out();
        if ((m_iStatus & 0x80000) && m_pParent) {
            m_pParent->m_iStatus |= 0x80000;
        }
    } else if (!(m_iStatus & 0x400000)) {
        if ((iParam[0] & 0x20) && (isAction(m_iCurY) & 8)) {
            if (openChild(m_pWinLineData[getY()].pChild, NULL)) {
                return;
            }
        }
        if ((iParam[0] & 0x40) && !(m_iStatus & 4)) {
            out();
        }
    }
}

void CDebugWindow::mainProc(INT* iParam) {
    if (!(m_iStatus & 2)) {
        m_iWx += dbgAnalogLX(USR_SYSTEM, 1) >> 10;
        m_iWy += dbgAnalogLY(USR_SYSTEM, 1) >> 10;
    }
    if (m_iEffCurMax > 0) {
        if (dbgButtonRpt(USR_SYSTEM, 0x1000, 1) || (m_iStatus & 0x800000)) {
            iParam[0] |= 0x200;
            if (!(m_iStatus & 0x400000)) {
                m_iCurY--;
                subCurYAfter();
                if (m_iOldCurY != m_iCurY) {
                    iParam[0] |= 4;
                    iParam[1] |= 8;
                }
            }
        } else if (dbgButtonRpt(USR_SYSTEM, 0x4000, 1) || (m_iStatus & 0x1000000)) {
            iParam[0] |= 0x400;
            if (!(m_iStatus & 0x400000)) {
                m_iCurY++;
                addCurYAfter();
                if (m_iOldCurY != m_iCurY) {
                    iParam[0] |= 4;
                    iParam[1] |= 8;
                }
            }
        }
    }
    m_iStatus &= ~0x1800000;
    if (dbgButtonRpt(USR_SYSTEM, 0x8000, 1)) {
        iParam[0] |= 0x80;
        if (!(m_iStatus & 0x400000)) {
            if ((isAction(m_iCurY) & 0x11) == 0x11) {
                addValue(-1, NULL, NULL, NULL, NULL);
                iParam[0] |= 0x10;
            }
            moveCurX(-1);
        }
    } else if (dbgButtonRpt(USR_SYSTEM, 0x2000, 1)) {
        iParam[0] |= 0x100;
        if (!(m_iStatus & 0x400000)) {
            if ((isAction(m_iCurY) & 0x11) == 0x11) {
                addValue(1, NULL, NULL, NULL, NULL);
                iParam[0] |= 0x10;
            }
            moveCurX(1);
        }
    }
    if (dbgButtonTrg(USR_SYSTEM, 0x20, 1)) {
        iParam[0] |= 0x20;
    } else if (dbgButtonTrg(USR_SYSTEM, 0x40, 1)) {
        iParam[0] |= 0x40;
    }
    if (dbgButtonTrg(USR_SYSTEM, 4, 1)) {
        iParam[0] |= 0x1000;
    } else if (dbgButtonTrg(USR_SYSTEM, 8, 1)) {
        iParam[0] |= 0x2000;
    }
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/DebugWindow", disp__12CDebugWindow);
/* Close: the reload registers of the spilled WW/sign (t0 instead of a3) and the loop's line-font position stores
 * (G_XYZ pointer formed before the x store) differ. */
#if 0
void CDebugWindow::disp() {
    INT iIdx;
    INT WW = m_iWw * m_iPitch;
    INT iBit;
    SBYTE iByte;
    SHORT iHWord;
    INT iWord;
    FLOAT fFloat;
    if (m_iStatus & 0x60000) {
        return;
    }
    if (isActive()) {
        m_gpFrame.m_C0.set(0, 0, 0, 0x60);
        m_gpTitle.m_C0.set(0, 0, 0x80, 0x60);
        m_gpCur.m_C0.set(0x80, 0x80, 0, 0x60);
        m_fntTitle.setBrightness(0x80, 0x80, 0, 0x80);
    } else {
        m_gpFrame.m_C0.set(0, 0, 0, 0x60);
        m_gpTitle.m_C0.set(0, 0, 0x40, 0x60);
        m_gpCur.m_C0.set(0x40, 0x40, 0, 0x60);
        m_fntTitle.setBrightness(0x40, 0x40, 0, 0x80);
    }
    m_gpFrame.m_P0.set((m_iWx - 2) << 4, m_iWy << 4, 0xFFFF280);
    INT WW2 = WW + 2;
    m_gpFrame.m_P1.set((m_iWx + WW2) << 4, (m_iWy + (m_iPitch + 2) * m_iWh) << 4);
    m_gpFrame.disp();
    {
        INT x = m_iWx - 2;
        INT y = m_iWy - 4;
        m_gpTitle.m_P0.set(x << 4, (y - m_iPitch) << 4, 0xFFFF280);
    }
    {
        INT x = m_iWx + WW2;
        INT y = m_iWy;
        m_gpTitle.m_P1.set(x << 4, y << 4);
    }
    m_gpTitle.disp();
    if (m_iEffCurMax > 0) {
        INT iCurPos = (m_iPitch + 2) * (m_iCurY - m_iLineTop) + 1;
        m_gpCur.m_P0.set(m_iWx << 4, (m_iWy + iCurPos) << 4, 0xFFFF280);
        m_gpCur.m_P1.set((m_iWx + WW) << 4, m_gpCur.m_P0.y + (m_iPitch << 4));
        if ((isAction(m_iCurY) & 0x20) && m_pWinCurX && m_pWinCurX[m_iCurY].iCurX >= 0) {
            m_gpCur.m_P0.x += (m_pWinCurX[m_iCurY].iPosX + m_pWinCurX[m_iCurY].iNextWidth * m_pWinCurX[m_iCurY].iCurX) << 4;
            m_gpCur.m_P1.x = m_gpCur.m_P0.x + (m_pWinCurX[m_iCurY].iCurWidth << 4);
        }
        m_gpCur.disp();
    }
    m_fntTitle.m_P0.set((m_iWx + WW / 2) << 4, (m_iWy - 2) << 4, 0xFFFF280);
    m_fntTitle.disp();
    iIdx = m_iLineTop;
    for (INT i = 0; i < m_iWh; i++, iIdx++) {
        const char* pStr;
        if (m_pStr) {
            pStr = m_pStr;
        } else {
            pStr = m_pWinLineData[getY(iIdx)].szString;
        }
        if (m_pDispFunc && pStr[0] == 0) {
            m_pDispFunc(m_pWinLine[i].szString, iIdx, m_pWinLineData[getY(iIdx)].pValue);
        } else {
            iBit = __getLowBit(m_pWinLineData[getY(iIdx)].iMask);
            switch (m_pWinLineData[getY(iIdx)].iValueSize) {
            case 1:
            default:
                iByte = *(SBYTE*)m_pWinLineData[getY(iIdx)].pValue;
                if (iBit >= 0) {
                    iByte = (iByte & m_pWinLineData[getY(iIdx)].iMask) >> iBit;
                }
                if ((m_pWinLineData[getY(iIdx)].iFlags & 0x20) && iByte != 0) {
                    iByte = 1;
                }
                if (m_pWinLineData[getY(iIdx)].szValueStr) {
                    sprintf(m_pWinLine[i].szString, pStr, m_pWinLineData[getY(iIdx)].szValueStr[iByte]);
                } else {
                    sprintf(m_pWinLine[i].szString, pStr, iByte);
                }
                break;
            case 2:
                iHWord = *(SHORT*)m_pWinLineData[getY(iIdx)].pValue;
                if (iBit >= 0) {
                    iHWord = (iHWord & m_pWinLineData[getY(iIdx)].iMask) >> iBit;
                }
                if ((m_pWinLineData[getY(iIdx)].iFlags & 0x20) && iHWord != 0) {
                    iHWord = 1;
                }
                if (m_pWinLineData[getY(iIdx)].szValueStr) {
                    sprintf(m_pWinLine[i].szString, pStr, m_pWinLineData[getY(iIdx)].szValueStr[iHWord]);
                } else {
                    sprintf(m_pWinLine[i].szString, pStr, iHWord);
                }
                break;
            case 4:
                if (m_pWinLineData[getY(iIdx)].iFlags & 1) {
                    fFloat = *(FLOAT*)m_pWinLineData[getY(iIdx)].pValue;
                    if ((m_pWinLineData[getY(iIdx)].iFlags & 0x20) && fFloat != 0.0f) {
                        fFloat = 1.0f;
                    }
                    if (m_pWinLineData[getY(iIdx)].szValueStr) {
                        sprintf(m_pWinLine[i].szString, pStr, m_pWinLineData[getY(iIdx)].szValueStr[(INT)fFloat]);
                    } else {
                        sprintf(m_pWinLine[i].szString, pStr, fFloat);
                    }
                } else {
                    iWord = *(INT*)m_pWinLineData[getY(iIdx)].pValue;
                    if (iBit >= 0) {
                        iWord = (iWord & m_pWinLineData[getY(iIdx)].iMask) >> iBit;
                    }
                    if ((m_pWinLineData[getY(iIdx)].iFlags & 0x20) && iWord != 0) {
                        iWord = 1;
                    }
                    if (m_pWinLineData[getY(iIdx)].szValueStr) {
                        sprintf(m_pWinLine[i].szString, pStr, m_pWinLineData[getY(iIdx)].szValueStr[iWord]);
                    } else {
                        sprintf(m_pWinLine[i].szString, pStr, iWord);
                    }
                }
                break;
            }
        }
        if (m_iStatus & 0x10) {
            m_pWinLine[i].fntString.m_P0.set((m_iWx + WW / 2) << 4, (i * 2 + (m_iPitch * (i + 1) + m_iWy) + 1) << 4,
                                             0xFFFF280);
        } else {
            m_pWinLine[i].fntString.m_P0.set(m_iWx << 4, (i * 2 + (m_iPitch * (i + 1) + m_iWy) + 1) << 4, 0xFFFF280);
        }
        if (isActive()) {
            m_pWinLine[i].fntString.setBrightness(0x80, 0x80, 0x80, 0x80);
        } else {
            m_pWinLine[i].fntString.setBrightness(0x40, 0x40, 0x40, 0x80);
        }
        m_pWinLine[i].fntString.disp();
    }
}
#endif

static DEBUG_WINDOW_LINE debugCamLine[8] = {
    {"POS-X :% 11.6f", &DebugWork::g_dw.nakamich.vCamPos.x, 4, {0}, 3, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"POS-Y :% 11.6f", &DebugWork::g_dw.nakamich.vCamPos.y, 4, {0}, 3, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"POS-Z :% 11.6f", &DebugWork::g_dw.nakamich.vCamPos.z, 4, {0}, 3, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"LOOK-X:% 11.6f", &DebugWork::g_dw.nakamich.vCamLook.x, 4, {0}, 3, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"LOOK-Y:% 11.6f", &DebugWork::g_dw.nakamich.vCamLook.y, 4, {0}, 3, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"LOOK-Z:% 11.6f", &DebugWork::g_dw.nakamich.vCamLook.z, 4, {0}, 3, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"DIST  :%11.6f", &DebugWork::g_dw.nakamich.fCamDist, 4, {0}, 3, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static char dbgCamName[32] = {0};

static DEBUG_WINDOW_TBL debugCamWin = {dbgCamName, 126, 72, 9, 8, 14, 0, 6, debugCamLine, NULL, NULL};

static float fCamSpeed[4] = {0.025f, 0.1f, 0.4f, 0.8f};

#define PI 3.1415927f

CNakCamera::CNakCamera(CTask* p, INT iPad, UINT iFlg, FVECTOR* pvPos, FLOAT fDirY, void** pVoid) : CCamera(p, -1) {
    init(iPad, iFlg, pvPos, fDirY, pVoid);
    { CDeFocusPrim* p = CDeFocus::getDeFocus(); if (p) p->stop(); }
}

void CNakCamera::init(INT iPad, UINT iFlg, FVECTOR* pvPos, FLOAT fDirY, void** pVoid) {
    G_LINE Line;
    m_pWin = NULL;
    if (iFlg & 0x100) {
#line 1210
        m_pWin = new (__FILE__ "(1210)") CDebugWindow(this, debugCamWin);
    }
    m_pVoid = pVoid;
    if (iPad < 0) {
        iPad = 0;
    }
    if (iPad >= 2) {
        iPad = 1;
    }
    m_iPad = iPad;
    DebugWork::g_dw.nakamich.fCamRotZ = 0.0f;
    if (iFlg & 0x200) {
        CCamera* pCam = CWorldEnv::getOldCamera();
        if (pCam) {
            const FVECTOR* pvLook = pCam->getLook();
            _setResetPos(pvLook, &pCam->Crd(0).getWorldMatrix().v[3], fDirY);
        } else {
            _setResetPos(pvPos, NULL, fDirY);
        }
    } else {
        _setResetPos(pvPos, NULL, fDirY);
    }
    m_iType = 1;
    m_iStatus = 0xF;
    ignorePause(TRUE);
    setPriority(PRI_BIT_ALWAYS);
    {
        CCamera* pCam = CWorldEnv::getActiveCamera();
        if (pCam && pCam != this) {
            setCameraPriority(pCam->getCameraPriority());
            setEnable(FALSE);
            setEnable(TRUE);
        }
    }
    {
        CTask* pM = CTaskMan::getSysTask(2);
        if (pM) {
            m_3DLines.m_pCrd = &pM->Crd(0);
        } else {
            m_3DLines.m_pCrd = NULL;
        }
    }
    m_3DLines.setSemiTrans(ALPHA_OFF);
    Line.P0.set(-1.0f, 0.0f, m_fDistance);
    Line.P1.set(1.0f, 0.0f, m_fDistance);
    Line.C0.set(0xFF, 0, 0, 0xFF);
    Line.C1.set(0xFF, 0, 0, 0xFF);
    m_3DLines.m_vLINE.push_back(Line);
    Line.P0.set(0.0f, -1.0f, m_fDistance);
    Line.P1.set(0.0f, 1.0f, m_fDistance);
    Line.C0.set(0, 0xFF, 0, 0xFF);
    Line.C1.set(0, 0xFF, 0, 0xFF);
    m_3DLines.m_vLINE.push_back(Line);
    Line.P0.set(0.0f, 0.0f, m_fDistance - 1.0f);
    Line.P1.set(0.0f, 0.0f, m_fDistance + 1.0f);
    Line.C0.set(0, 0, 0xFF, 0xFF);
    Line.C1.set(0, 0, 0xFF, 0xFF);
    m_3DLines.m_vLINE.push_back(Line);
    m_iUpdate = 7;
    Update();
}

CNakCamera::~CNakCamera() {
    if (m_pVoid) {
        *m_pVoid = NULL;
    }
    { CDeFocusPrim* p = CDeFocus::getDeFocus(); if (p) p->start(); }
}

INT CNakCamera::getTypeID() const { return 0x8000; }

void CNakCamera::SetDebugViewAngle(FLOAT fDist) {
    if (fDist > -1e+07f) {
        setProjectionDist(fDist);
    }
    DebugWork::g_dw.nakamich.fCamVAngle = ProjectionToViewAngle(getProjectionDist());
}

void CNakCamera::_setResetPos(const FVECTOR* pvLookPos, const FVECTOR* pvCamPos, FLOAT fDirY) {
    m_vCamRot.set(0.0f, fDirY, 0.0f);
    if (!pvLookPos) {
        m_fDistance = 5.0f;
        m_vCamLook.set(0.0f, 1.0f, 0.0f);
        m_vCamPos.set(0.0f, 1.0f, 5.0f);
    } else if (!pvCamPos) {
        m_fDistance = 5.0f;
        m_vCamLook = *pvLookPos;
        LookAngleDistanceToPos();
    } else {
        m_vCamLook = *pvLookPos;
        m_vCamPos = *pvCamPos;
        LookPosToAngleDistance();
    }
    m_vCamMove.set(0.0f, 0.0f, 0.0f);
    m_vCamLookAim = m_vCamLook;
    m_vCamPosAim = m_vCamPos;
    m_vCamRotAim = m_vCamRot;
    m_fDistanceAim = m_fDistance;
}

void CNakCamera::LookAngleDistanceToPos() {
    FMATRIX mat;
    FVECTOR vec;
    vec.set(0.0f, 0.0f, -m_fDistance);
    _unitMatrix(mat);
    sceVu0RotMatrixX(mat, mat, m_vCamRot.x);
    sceVu0RotMatrixY(mat, mat, m_vCamRot.y);
    ApplyMatrix(vec, mat, vec);
    AddVectorXYZ(m_vCamPos, m_vCamLook, vec);
}

void CNakCamera::PosAngleDistanceToLook() {
    FMATRIX mat;
    FVECTOR vec;
    vec.set(0.0f, 0.0f, m_fDistance);
    _unitMatrix(mat);
    sceVu0RotMatrixX(mat, mat, m_vCamRot.x);
    sceVu0RotMatrixY(mat, mat, m_vCamRot.y);
    ApplyMatrix(vec, mat, vec);
    AddVectorXYZ(m_vCamLook, m_vCamPos, vec);
}

void CNakCamera::LookPosToAngleDistance() {
    FVECTOR vec;
    FLOAT ret;
    SubVector(vec, m_vCamLook, m_vCamPos);
    m_fDistance = _sqrtf(InnerProduct(vec));
    ret = _sqrtf(vec.x * vec.x + vec.z * vec.z);
    m_vCamRot.x = -atan2f(vec.y, ret);
    m_vCamRot.y = atan2f(vec.x, vec.z);
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/DebugWindow", main__10CNakCamera);
/* STOPGAP while main is INCLUDE_ASM: its sprintf formats under their original labels. */
asm(".section .rodata\n"
    ".align 3\n"
    "D_004D9B90:\n"
    ".asciz \"\\203J\\203\\201\\203\\211\\217\\356\\225\\361(L3:\\216\\345)\"\n"
    ".align 3\n"
    "D_004D9BA8:\n"
    ".asciz \"\\203J\\203\\201\\203\\211\\217\\356\\225\\361(L3:\\213q)\"\n"
    ".previous\n");
/* Only the scheduling differs: the m_fProjectionDist store after ViewAngleToProjection is placed after the
 * CDeFocus instance load (the original stores it first, with %hi in v0). */
#if 0
void CNakCamera::main() {
    if (this != CWorldEnv::getActiveCamera()) {
        if (m_pWin) {
            m_pWin->dispOff(0);
        }
        { CDeFocusPrim* p = CDeFocus::getDeFocus(); if (p) p->start(); }
        return;
    }
    {
        FLOAT f = ViewAngleToProjection(DebugWork::g_dw.nakamich.fCamVAngle);
        CDeFocusPrim* p = CDeFocus::getDeFocus();
        setProjectionDist(f);
        if (p) {
            p->stop();
        }
    }
    if ((m_iStatus & 0x30) && dbgButtonTrg(USR_SYSTEM, 0x200, m_iPad)) {
        ChangeType(-1);
    }
    m_vCamRot.z = DebugWork::g_dw.nakamich.fCamRotZ;
    if (m_iType) {
        SubjectivityMove();
        sprintf(dbgCamName, "\x83J\x83\x81\x83\x89\x8f\xee\x95\xf1(L3:\x8e\xe5)");
    } else {
        ObjectivityMove();
        sprintf(dbgCamName, "\x83J\x83\x81\x83\x89\x8f\xee\x95\xf1(L3:\x8bq)");
    }
    if (m_pWin) {
        if (DebugWork::g_dw.nakamich.iCamDisp) {
            m_pWin->dispOff(0);
        } else {
            m_pWin->dispOn(0);
        }
        DebugWork::g_dw.nakamich.vCamPos = m_vCamPos;
        DebugWork::g_dw.nakamich.vCamLook = m_vCamLook;
        DebugWork::g_dw.nakamich.fCamDist = Math3D::PointToPointDistance(m_vCamPos, m_vCamLook);
    }
    if ((m_iStatus & 0x40) && dbgButtonTrg(USR_SYSTEM, 0xF, m_iPad) && dbgButtonDown(USR_SYSTEM, 0xF, m_iPad)) {
        _setResetPos(NULL, NULL, PI);
    }
}
#endif

void CNakCamera::disp() {
    G_LINE Line;
    if (this != CWorldEnv::getActiveCamera()) {
        return;
    }
    if (DebugWork::g_dw.nakamich.iCamDisp) {
        return;
    }
    if (m_iType) {
        return;
    }
    Line.P0 = m_vCamLook;
    Line.P1 = m_vCamLook;
    Line.P1.x += 0.5f;
    Line.P0.w = 1.0f;
    Line.P1.w = 1.0f;
    Line.C0.set(0xFF, 0, 0, 0xFF);
    Line.C1.set(0xFF, 0, 0, 0xFF);
    m_3DLines.m_vLINE.getBuffer()[0] = Line;
    Line.P0 = m_vCamLook;
    Line.P1 = m_vCamLook;
    Line.P1.y += 0.5f;
    Line.P0.w = 1.0f;
    Line.P1.w = 1.0f;
    Line.C0.set(0, 0xFF, 0, 0xFF);
    Line.C1.set(0, 0xFF, 0, 0xFF);
    m_3DLines.m_vLINE.getBuffer()[1] = Line;
    Line.P0 = m_vCamLook;
    Line.P1 = m_vCamLook;
    Line.P1.z += 0.5f;
    Line.P0.w = 1.0f;
    Line.P1.w = 1.0f;
    Line.C0.set(0, 0, 0xFF, 0xFF);
    Line.C1.set(0, 0, 0xFF, 0xFF);
    m_3DLines.m_vLINE.getBuffer()[2] = Line;
    m_3DLines.disp();
}

void CNakCamera::Update() {
    if (m_iType) {
        SubjectivityUpdate();
    } else {
        ObjectivityUpdate();
    }
    Crd(0).fix(0);
}

void CNakCamera::Action(UINT iBit) { m_iStatus = iBit; }

void CNakCamera::ChangeType(INT iType) {
    if (iType < 0) {
        if (m_iType == 0) {
            iType = 1;
        } else if (m_iType == 1) {
            iType = 0;
        }
    }
    if (m_iType != iType) {
        m_iType = iType;
        if (iType == 0 && !(m_iStatus & 0x20)) {
            PosAngleDistanceToLook();
            m_vCamLookAim = m_vCamLook;
        }
    }
}

#define ANALOG_ON(a) (((a) < 0 ? -(a) : (a)) >> 8 << 8)

void CNakCamera::SubjectivityMove() {
    TIMEQ fr = getFrameRate();
    INT iAna;
    m_vCamMove.set(0.0f, 0.0f, 0.0f);
    if (m_iStatus & 2) {
        iAna = dbgAnalogRX(USR_SYSTEM, m_iPad);
        if (ANALOG_ON(iAna)) {
            m_vCamRot.y -= iAna / 4096.0f * PI / 180.0f * 0.05f * fr;
            m_vCamRot.y = Math3D::LimitPI(m_vCamRot.y);
            m_iUpdate |= 1;
        }
        iAna = dbgAnalogRY(USR_SYSTEM, m_iPad);
        if (ANALOG_ON(iAna)) {
            m_vCamRot.x -= iAna / 4096.0f * PI / 180.0f * 0.05f * fr;
            m_vCamRot.x = Math3D::LimitPI(m_vCamRot.x);
            m_iUpdate |= 1;
        }
    }
    if (m_iStatus & 8) {
        iAna = dbgAnalogLX(USR_SYSTEM, m_iPad);
        if (ANALOG_ON(iAna)) {
            m_vCamMove.x = -(FLOAT)iAna / 4096.0f * 0.04f * fr;
            m_iUpdate |= 2;
        }
        iAna = dbgAnalogLY(USR_SYSTEM, m_iPad);
        if (ANALOG_ON(iAna)) {
            m_vCamMove.y = -(FLOAT)iAna / 4096.0f * 0.04f * fr;
            m_iUpdate |= 2;
        }
        if (dbgButtonDown(USR_SYSTEM, 8, m_iPad) && !dbgButtonDown(USR_SYSTEM, 4, m_iPad)) {
            m_vCamMove.z = fr * 0.04f;
            m_iUpdate |= 2;
        } else if (dbgButtonDown(USR_SYSTEM, 4, m_iPad) && !dbgButtonDown(USR_SYSTEM, 8, m_iPad)) {
            m_vCamMove.z = fr * -0.04f;
            m_iUpdate |= 2;
        }
        ScaleVectorXYZ(m_vCamMove, m_vCamMove, fCamSpeed[DebugWork::g_dw.nakamich.iCamSpeed]);
    }
    Update();
}

void CNakCamera::SubjectivityUpdate() {
    FLOAT fr = getFrameRateF();
    FVECTOR vVec;
    FMATRIX& mat = Crd(0).getLocalMatrix();
    _unitMatrix(mat);
    if (m_iUpdate & 1) {
        m_vCamRotAim = m_vCamRot;
    }
    sceVu0RotMatrixZ(mat, mat, m_vCamRot.z);
    sceVu0RotMatrixX(mat, mat, m_vCamRot.x);
    sceVu0RotMatrixY(mat, mat, m_vCamRot.y);
    if (m_iUpdate & 2) {
        ApplyMatrix(m_vCamMove, mat, m_vCamMove);
        AddVectorXYZ(m_vCamPos, m_vCamPos, m_vCamMove);
        m_vCamPosAim = m_vCamPos;
    } else {
        SubVectorXYZ(vVec, m_vCamPosAim, m_vCamPos);
        ScaleVectorXYZ(vVec, vVec, fr * 0.0625f);
        AddVectorXYZ(m_vCamPos, m_vCamPos, vVec);
    }
    m_vCamLookAim = m_vCamLook;
    m_fDistanceAim = m_fDistance;
    mat.v[3] = m_vCamPos;
    m_vCamLook.set(0.0f, 0.0f, m_fDistance);
    ApplyMatrix(m_vCamLook, mat, m_vCamLook);
    setLook(m_vCamLook);
    Crd(0).touch();
    m_iUpdate = 0;
}

void CNakCamera::ObjectivityMove() {
    TIMEQ fr = getFrameRate();
    INT iAna;
    m_vCamMove.set(0.0f, 0.0f, 0.0f);
    if (m_iStatus & 1) {
        iAna = dbgAnalogRX(USR_SYSTEM, m_iPad);
        if (ANALOG_ON(iAna)) {
            m_vCamRot.y += iAna / 4096.0f * PI / 180.0f * 0.05f * fr;
            m_vCamRot.y = Math3D::LimitPI(m_vCamRot.y);
            m_iUpdate |= 1;
        }
        iAna = dbgAnalogRY(USR_SYSTEM, m_iPad);
        if (ANALOG_ON(iAna)) {
            m_vCamRot.x -= iAna / 4096.0f * PI / 180.0f * 0.05f * fr;
            m_vCamRot.x = Math3D::LimitPI(m_vCamRot.x);
            if (m_vCamRot.x > PI / 2) {
                m_vCamRot.x = PI / 2;
            } else if (m_vCamRot.x < -PI / 2) {
                m_vCamRot.x = -PI / 2;
            }
            m_iUpdate |= 1;
        }
    }
    if (m_iStatus & 4) {
        iAna = dbgAnalogLX(USR_SYSTEM, m_iPad);
        if (ANALOG_ON(iAna)) {
            m_vCamMove.x = -(FLOAT)iAna / 4096.0f * 0.04f * fr;
            m_iUpdate |= 2;
        }
        iAna = dbgAnalogLY(USR_SYSTEM, m_iPad);
        if (ANALOG_ON(iAna)) {
            m_vCamMove.z = -(FLOAT)iAna / 4096.0f * 0.04f * fr;
            m_iUpdate |= 2;
        }
        ScaleVectorXYZ(m_vCamMove, m_vCamMove, fCamSpeed[DebugWork::g_dw.nakamich.iCamSpeed]);
        if (dbgButtonDown(USR_SYSTEM, 2, m_iPad) && !dbgButtonDown(USR_SYSTEM, 1, m_iPad)) {
            m_vCamLook.y += fr * 0.02f * fCamSpeed[DebugWork::g_dw.nakamich.iCamSpeed];
            m_iUpdate |= 2;
        } else if (dbgButtonDown(USR_SYSTEM, 1, m_iPad) && !dbgButtonDown(USR_SYSTEM, 2, m_iPad)) {
            m_vCamLook.y -= fr * 0.02f * fCamSpeed[DebugWork::g_dw.nakamich.iCamSpeed];
            m_iUpdate |= 2;
        }
    }
    if (m_iStatus & 1) {
        if (dbgButtonDown(USR_SYSTEM, 8, m_iPad) && !dbgButtonDown(USR_SYSTEM, 4, m_iPad)) {
            m_fDistance -= fr * 0.04f * fCamSpeed[DebugWork::g_dw.nakamich.iCamSpeed];
            if (m_fDistance < 0.001f) {
                m_fDistance = 0.001f;
            }
            m_iUpdate |= 4;
        } else if (dbgButtonDown(USR_SYSTEM, 4, m_iPad) && !dbgButtonDown(USR_SYSTEM, 8, m_iPad)) {
            m_fDistance += fr * 0.04f * fCamSpeed[DebugWork::g_dw.nakamich.iCamSpeed];
            m_iUpdate |= 4;
        }
    }
    Update();
}

void CNakCamera::ObjectivityUpdate() {
    FLOAT fr = getFrameRateF();
    FMATRIX& crd = Crd(0).getLocalMatrix();
    _unitMatrix(crd);
    if (m_iUpdate & 1) {
        m_vCamRotAim = m_vCamRot;
    }
    sceVu0RotMatrixZ(crd, crd, m_vCamRot.z);
    sceVu0RotMatrixX(crd, crd, m_vCamRot.x);
    sceVu0RotMatrixY(crd, crd, m_vCamRot.y);
    if (m_iUpdate & 2) {
        FMATRIX mat;
        _unitMatrix(mat);
        sceVu0RotMatrixY(mat, mat, m_vCamRot.y);
        ApplyMatrix(m_vCamMove, mat, m_vCamMove);
        AddVectorXYZ(m_vCamLook, m_vCamLook, m_vCamMove);
        m_vCamLookAim = m_vCamLook;
    } else {
        FVECTOR vVec;
        SubVectorXYZ(vVec, m_vCamLookAim, m_vCamLook);
        ScaleVectorXYZ(vVec, vVec, fr * 0.0625f);
        AddVectorXYZ(m_vCamLook, m_vCamLook, vVec);
    }
    if (m_iUpdate & 4) {
        m_fDistanceAim = m_fDistance;
    }
    LookAngleDistanceToPos();
    m_vCamPosAim = m_vCamPos;
    crd.v[3] = m_vCamPos;
    setLook(m_vCamLook);
    Crd(0).touch();
    m_iUpdate = 0;
}

CGlidWorld::CGlidWorld(CTask* p) : CTask(p, 0x4000) {
    G_STRIP Line;
    m_iStatus = 1;
    addCoord(1);
    _unitMatrix(Crd(0).getLocalMatrix());
    Crd(0).touch();
    m_3DLines.setCoord(&Crd(0));
    m_3DLines.setSemiTrans(ALPHA_OFF);
    Line.Term = 0;
    for (INT i = 0; i <= 20; i++) {
        for (INT j = 0; j <= 20; j++) {
            if ((j & 1) == 0) {
                Line.C.set(0x40, 0, 0, 0xFF);
            } else {
                Line.C.set(0xFF, 0x20, 0x20, 0xFF);
            }
            Line.P.set(j - 10.0f, 0.0f, i - 10.0f);
            m_3DLines.m_vVERT.push_back(Line);
        }
        m_3DLines.m_vVERT.back().Term = 1;
    }
    for (INT i = 0; i <= 20; i++) {
        for (INT j = 0; j <= 20; j++) {
            if ((j & 1) == 0) {
                Line.C.set(0, 0, 0x40, 0xFF);
            } else {
                Line.C.set(0, 0x80, 0xFF, 0xFF);
            }
            Line.P.set(i - 10.0f, 0.0f, j - 10.0f);
            m_3DLines.m_vVERT.push_back(Line);
        }
        m_3DLines.m_vVERT.back().Term = 1;
    }
    Line.C.set(0, 0x40, 0, 0xFF);
    Line.P.set(0.0f, -1.0f, 0.0f);
    m_3DLines.m_vVERT.push_back(Line);
    Line.C.set(0, 0xFF, 0, 0xFF);
    Line.P.set(0.0f, 1.0f, 0.0f);
    Line.Term = 1;
    m_3DLines.m_vVERT.push_back(Line);
}

CGlidWorld::~CGlidWorld() {}

void CGlidWorld::main() {}

void CGlidWorld::disp() {
    if (m_iStatus & 1) {
        m_3DLines.disp();
    }
}

void CDebugWindow::openTopDebugWindow(CTask* p) {
    if (m_bTopOpen && !m_pInstance) {
#line 2085
        m_pInstance = new (__FILE__ "(2085)") CDebugWindow(p, DWT::debugWinTop, TRUE);
    }
}

void CDebugWindow::closeTopDebugWindow() {
    if (m_pInstance) {
        m_pInstance->out();
        m_pInstance = NULL;
    }
}

BOOL CDebugWindow::isTopDebugWindow() { return m_pInstance != NULL; }

void CDebugWindow::enableTopDebugWindow(BOOL bFlg) {
    m_bTopOpen = bFlg;
    if (!bFlg) {
        closeTopDebugWindow();
    }
}

// SOURCE DATA: .data .rodata
