#ifndef GAME_ATEL_ATELFOG_H
#define GAME_ATEL_ATELFOG_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/HeapMan.h"
#include "System/Vector.h"
#include "Game/Atel/CommandAccepter.h"

/* Includers define the placement operator new before including this header. The original's file is
 * Game/ATEL/AtelFog.h; most includers name it with the upper-case directory. */
#ifndef ATELFOG_H_PATH
#define ATELFOG_H_PATH "c:/KHCOM/MainApp/Game/ATEL/AtelFog.h"
#endif

namespace ATEL {

struct AtelFogColorAnimStruct {
    FVECTOR _vStartColor;    /* 0x00 */
    FVECTOR _vTargetColor;   /* 0x10 */
    FVECTOR _vColorSpeed;    /* 0x20 */
    FVECTOR _vColorBuf;      /* 0x30 */
    TIMEQ _nColorTimeLimit;  /* 0x40 */
    TIMEQ _nColorTimeCount;  /* 0x44 */
    BOOL isOverTimeLimit() { return _nColorTimeCount >= _nColorTimeLimit; }
};

struct AtelFogNormalAnimStruct {
    FLOAT _fTarget;     /* 0x0 */
    FLOAT _fSpeed;      /* 0x4 */
    TIMEQ _nTimeLimit;  /* 0x8 */
    TIMEQ _nTimeCount;  /* 0xc */
    BOOL isOverTimeLimit() { return _nTimeCount >= _nTimeLimit; }
};

struct AtelFogClampAnimStruct {
    FLOAT _fTarget;     /* 0x00 */
    FLOAT _fcalBuf;     /* 0x04 */
    FLOAT _fSpeed;      /* 0x08 */
    TIMEQ _nTimeLimit;  /* 0x0c */
    TIMEQ _nTimeCount;  /* 0x10 */
    BOOL isOverTimeLimit() { return _nTimeCount >= _nTimeLimit; }
};

class CAtelFog : public CTask, public ICommandAccepter {
private:
    INT m_nState;                       /* 0x50 */
    AtelFogColorAnimStruct m_ColorAnim; /* 0x60 */
    AtelFogNormalAnimStruct m_NearAnim; /* 0xb0 */
    AtelFogNormalAnimStruct m_FarAnim;  /* 0xc0 */
    AtelFogClampAnimStruct m_MaxAnim;   /* 0xd0 */
    AtelFogClampAnimStruct m_MinAnim;   /* 0xe4 */

public:
    static CAtelFog* m_pInstance;
    CAtelFog(CTask* pParent);
    virtual ~CAtelFog();
#line 107 ATELFOG_H_PATH
    static void initialize(CTask* pParent) {
        if (m_pInstance) return;
        TRACE("CAtelFog::initialize\n");
        if (!(__null == m_pInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
        m_pInstance = new (__FILE__ "(111)") CAtelFog(pParent);
        if (!(m_pInstance)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
    }
    static void finalize() {
        CAtelFog*& p = m_pInstance;
        if (p) delete p;
        p = __null;
    }
    static CAtelFog& instance() {
        CAtelFog*& p = m_pInstance;
#line 122
        if (!(p)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
        return *p;
    }
    virtual void main();
    virtual INT reqCommand(INT nCmd, void* pParam, INT nID) {
        if (nCmd == 0x5DC) return onSetFogRGBA(nCmd, pParam, nID);
        if (nCmd == 0x5DD) return onSetFogRGBA_Anim(nCmd, pParam, nID);
        if (nCmd == 0x5DE) return onSetFogNear(nCmd, pParam, nID);
        if (nCmd == 0x5DF) return onSetFogNear_Anim(nCmd, pParam, nID);
        if (nCmd == 0x5E0) return onSetFogFar(nCmd, pParam, nID);
        if (nCmd == 0x5E1) return onSetFogFar_Anim(nCmd, pParam, nID);
        if (nCmd == 0x5E2) return onSetFogMax(nCmd, pParam, nID);
        if (nCmd == 0x5E3) return onSetFogMax_Anim(nCmd, pParam, nID);
        if (nCmd == 0x5E4) return onSetFogMin(nCmd, pParam, nID);
        if (nCmd == 0x5E5) return onSetFogMin_Anim(nCmd, pParam, nID);
        return 0;
    }

private:
    INT AnimColor();
    INT AnimNear();
    INT AnimFar();
    INT AnimMax();
    INT AnimMin();

protected:
    INT onSetFogRGBA(INT nCmd, void* pParam, INT nID);
    INT onSetFogRGBA_Anim(INT nCmd, void* pParam, INT nID);
    INT onSetFogNear(INT nCmd, void* pParam, INT nID);
    INT onSetFogNear_Anim(INT nCmd, void* pParam, INT nID);
    INT onSetFogFar(INT nCmd, void* pParam, INT nID);
    INT onSetFogFar_Anim(INT nCmd, void* pParam, INT nID);
    INT onSetFogMax(INT nCmd, void* pParam, INT nID);
    INT onSetFogMax_Anim(INT nCmd, void* pParam, INT nID);
    INT onSetFogMin(INT nCmd, void* pParam, INT nID);
    INT onSetFogMin_Anim(INT nCmd, void* pParam, INT nID);
};

} // namespace ATEL

#endif /* GAME_ATEL_ATELFOG_H */
