// PARTIAL: added for Form/Menu/MonstroGauge
#ifndef SYSTEM_GRAPHIC_2D_2DSEQOBJ_H
#define SYSTEM_GRAPHIC_2D_2DSEQOBJ_H

#include "khtypes.h"

struct G_UV;
namespace D2 { struct SeqControl; } // PARTIAL: in D2 as in the original

namespace D2 {

class C2DO {
public: // PARTIAL: inline (weak in the original) and public as in the debug info, for System/Graphic/2d/2DLayObj
    HANDLE m_hBaseHandle; /* 0x0 */
    HANDLE m_hHandle;     /* 0x4 */
    /* vtable 0x8 */

    C2DO() : m_hBaseHandle(0), m_hHandle(0) {}
    virtual ~C2DO() {}
    HANDLE getBaseHandle() { return m_hBaseHandle; }
    HANDLE _getHandle() { return m_hHandle; }
    virtual void _main() {}
    virtual void _disp() {}
    virtual void changeTimeExt(INT qTime) {}
    virtual void changeTexture(INT nIdx, CHAR* pName, G_UV* pStart, G_UV* pEnd) {}
    virtual void changeTexture(INT nIdx, INT nIdx2, CHAR* pName, G_UV* pStart, G_UV* pEnd) {}
    virtual void restart(INT nFlag) {}
    virtual void changeLayer(INT nLayer) {}
    virtual void enableDisp(INT bEnable) {}
};

class C2DSeqObj : public C2DO {
private:
    void* m_pScissoringEnd;     /* 0x0c */
    void* m_p2DObjList;         /* 0x10 */
    SeqControl* m_pSeqControl;  /* 0x14 */
    TIMEQ m_qTime;              /* 0x18 */
    TIMEQ m_qTimeMax;           /* 0x1c */
    TIMEQ m_qTimeExt;           /* 0x20 */
    INT m_nLoop;                /* 0x24 */
    INT m_nLoopFrame;           /* 0x28 */
    CHAR m_bSeq;                /* 0x2c */
    CHAR m_bEnable;             /* 0x2d */
    CHAR m_bEnd;                /* 0x2e */
    CHAR m_fgParent;            /* 0x2f */
    CHAR m_fgMainSkip;          /* 0x30 */

public:
    C2DSeqObj();
    virtual ~C2DSeqObj();
    TIMEQ getTime() { return m_qTime; } // PARTIAL: added for System/Graphic/2d/2DLayObj
    BOOL isEnd() { return m_bEnd; }
};

}  // namespace D2

#endif /* SYSTEM_GRAPHIC_2D_2DSEQOBJ_H */
