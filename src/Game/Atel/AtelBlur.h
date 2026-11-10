#ifndef GAME_ATEL_ATELBLUR_H
#define GAME_ATEL_ATELBLUR_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/HeapMan.h"
#include "System/Vector.h"
#include "Game/Atel/CommandAccepter.h"

/* Includers define the placement operator new before including this header. The original's file is
 * Game/ATEL/AtelBlur.h; most includers name it with the upper-case directory. */
#ifndef ATELBLUR_H_PATH
#define ATELBLUR_H_PATH "c:/KHCOM/MainApp/Game/ATEL/AtelBlur.h"
#endif

namespace ATEL {

struct AtelBlurAnimStruct {
    FVECTOR _fTarget;  /* 0x00 */
    FVECTOR _Calc;     /* 0x10 */
    FVECTOR _fSpeed;   /* 0x20 */
    TIMEQ _nTimeLimit; /* 0x30 */
    TIMEQ _nTimeCount; /* 0x34 */
    BOOL isOverTimeLimit() { return _nTimeCount >= _nTimeLimit; }
};

class CAtelBlur : public CTask, public ICommandAccepter {
private:
    INT m_nState;              /* 0x50 */
    INT m_Xofs;                /* 0x54 */
    INT m_Yofs;                /* 0x58 */
    INT m_Alpha;               /* 0x5c */
    AtelBlurAnimStruct m_Anim; /* 0x60 */

public:
    static CAtelBlur* m_pInstance;
    CAtelBlur(CTask* pParent);
    virtual ~CAtelBlur();
    static void initialize(CTask* pParent) {
        if (m_pInstance) return;
        TRACE("CAtelBlur::initialize\n");
#line 80 ATELBLUR_H_PATH
        if (!(__null == m_pInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
        m_pInstance = new KH_AT("(81)") CAtelBlur(pParent);
#line 82
        if (!(m_pInstance)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
    }
    static void finalize() {
        CAtelBlur*& p = m_pInstance;
        if (p) delete p;
        p = __null;
    }
    static CAtelBlur& instance() {
        CAtelBlur*& p = m_pInstance;
#line 92
        if (!(p)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
        return *p;
    }
    virtual void main();
    virtual INT reqCommand(INT nCmd, void* pParam, INT nID) {
        if (nCmd == 0x640) return onSetBlurAlpha(nCmd, pParam, nID);
        if (nCmd == 0x642) return onSetBlurXofs(nCmd, pParam, nID);
        if (nCmd == 0x644) return onSetBlurYofs(nCmd, pParam, nID);
        if (nCmd == 0x641) return onSetBlurAlphaAnim(nCmd, pParam, nID);
        if (nCmd == 0x643) return onSetBlurXofsAnim(nCmd, pParam, nID);
        if (nCmd == 0x645) return onSetBlurYofsAnim(nCmd, pParam, nID);
        if (nCmd == 0x646) return onSetBlurAnim(nCmd, pParam, nID);
        if (nCmd == 0x647) return onSetBlurEnd(nCmd, pParam, nID);
        return 0;
    }

private:
    INT Anim();

protected:
    INT onSetBlurAlpha(INT nCmd, void* pParam, INT nID);
    INT onSetBlurXofs(INT nCmd, void* pParam, INT nID);
    INT onSetBlurYofs(INT nCmd, void* pParam, INT nID);
    INT onSetBlurAlphaAnim(INT nCmd, void* pParam, INT nID);
    INT onSetBlurXofsAnim(INT nCmd, void* pParam, INT nID);
    INT onSetBlurYofsAnim(INT nCmd, void* pParam, INT nID);
    INT onSetBlurAnim(INT nCmd, void* pParam, INT nID);
    INT onSetBlurEnd(INT nCmd, void* pParam, INT nID);
};

} // namespace ATEL

#endif /* GAME_ATEL_ATELBLUR_H */
