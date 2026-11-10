#ifndef GAME_ATEL_ATELCROSSFADE_H
#define GAME_ATEL_ATELCROSSFADE_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/HeapMan.h"
#include "System/Vector.h"
#include "Game/Atel/CommandAccepter.h"

/* Includers define the placement operator new before including this header. The original's file is
 * Game/ATEL/AtelCrossFade.h; most includers name it with the upper-case directory. */
#ifndef ATELCROSSFADE_H_PATH
#define ATELCROSSFADE_H_PATH "c:/KHCOM/MainApp/Game/ATEL/AtelCrossFade.h"
#endif

namespace ATEL {

struct AtelCrossColorAnimStruct {
    FVECTOR _vStartColor;    /* 0x00 */
    FVECTOR _vTargetColor;   /* 0x10 */
    FVECTOR _vColorSpeed;    /* 0x20 */
    FVECTOR _vColorBuf;      /* 0x30 */
    TIMEQ _nColorTimeLimit;  /* 0x40 */
    TIMEQ _nColorTimeCount;  /* 0x44 */
    BOOL isOverTimeLimit() { return _nColorTimeCount >= _nColorTimeLimit; }
};

class CAtelCrossFade : public CTask, public ICommandAccepter {
private:
    INT m_nState;                         /* 0x50 */
    INT m_CrossFadeFlag;                  /* 0x54 */
    AtelCrossColorAnimStruct m_ColorAnim; /* 0x60 */
    IVECTOR m_Color;                      /* 0xb0 */

public:
    static CAtelCrossFade* m_pInstance;
    u_long128* m_pCrossFadeImage;         /* 0xc0 */
    CAtelCrossFade(CTask* pParent);
    virtual ~CAtelCrossFade();
    static void initialize(CTask* pParent) {
        if (m_pInstance) return;
        TRACE("CAtelCrossFade::initialize\n");
#line 110 ATELCROSSFADE_H_PATH
        if (!(__null == m_pInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
        m_pInstance = new KH_AT("(111)") CAtelCrossFade(pParent);
#line 112
        if (!(m_pInstance)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
    }
    static void finalize() {
        CAtelCrossFade*& p = m_pInstance;
        if (p) delete p;
        p = __null;
    }
    static CAtelCrossFade& instance() {
        CAtelCrossFade*& p = m_pInstance;
#line 122
        if (!(p)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
        return *p;
    }
    virtual void main();
    virtual INT reqCommand(INT nCmd, void* pParam, INT nID) {
        if (nCmd == 0x640) return onCreateTexMemory(nCmd, pParam, nID);
        if (nCmd == 0x641) return onStoreTexImage(nCmd, pParam, nID);
        if (nCmd == 0x642) return onDrawTexImage(nCmd, pParam, nID);
        if (nCmd == 0x643) return onSetCrossFadeRGBA(nCmd, pParam, nID);
        if (nCmd == 0x644) return onSetCrossFadeRGBA_Anim(nCmd, pParam, nID);
        return 0;
    }

private:
    INT AnimColor();

protected:
    INT onCreateTexMemory(INT nCmd, void* pParam, INT nID);
    INT onStoreTexImage(INT nCmd, void* pParam, INT nID);
    INT onDrawTexImage(INT nCmd, void* pParam, INT nID);
    INT onSetCrossFadeRGBA(INT nCmd, void* pParam, INT nID);
    INT onSetCrossFadeRGBA_Anim(INT nCmd, void* pParam, INT nID);

public:
    void setCrossFadeFlag(UINT inSet, UINT inMask) {
        INT& f = m_CrossFadeFlag;
        f = (f | inSet) & ~inMask;
    }
    BOOL isCrossFadeStoreImage();
    BOOL isCrossFadeDrawImage();
    void storeCrossFadeImage();
    void drawCrossFadeImage();
};

} // namespace ATEL

#endif /* GAME_ATEL_ATELCROSSFADE_H */
