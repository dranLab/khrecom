
#ifndef SYSTEM_GRAPHIC_2D_2DOBJ_H
#define SYSTEM_GRAPHIC_2D_2DOBJ_H

#include "khtypes.h"
#include "System/SJoint.h"

namespace D2 { struct SeqControl; } // PARTIAL: in D2 as in the original
struct TexInfo;
struct G_UV;

namespace D2 {

class C2DSeqObj;

class C2DObj : public CSJointT<C2DObj> {
    friend class C2DSeqObj; // MATCH: System/Graphic/2d/2DSeqObj calls the protected changeTexture/changeLayer (friends are not in the debug info)
protected:
    SeqControl* m_pSeqControl; /* 0x8 */
    CHAR m_fgDisp;             /* 0xc */

public:
    C2DObj() {}
    C2DObj(SeqControl* pSeqControl) : m_pSeqControl(pSeqControl) {}
    virtual ~C2DObj() {}
    virtual void _main(TIMEQ qTime) = 0;
    virtual INT _disp() = 0;
    virtual void enableDisp(BOOL bEnable) { m_fgDisp = bEnable; }
    virtual void restart() {}
    virtual void shiftBase(FLOAT fX, FLOAT fY) = 0;
    virtual void shiftColor(FLOAT fR, FLOAT fG, FLOAT fB, FLOAT fA) {}
    virtual void setFukidasiSize(FLOAT fSizeX, FLOAT fSizeY) {}
protected:
    virtual void changeTexture(INT inNum, const TexInfo* pTexInfo, G_UV* pStart, G_UV* pEnd) {}
    virtual void changeLayer(INT nLayer) = 0;
};

}  // namespace D2

#endif /* SYSTEM_GRAPHIC_2D_2DOBJ_H */
