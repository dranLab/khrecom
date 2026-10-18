// PARTIAL: added for System/Graphic/2d/2DSeqObj (layout from the debug info; members as needed)
#ifndef SYSTEM_GRAPHIC_2D_2DSPROBJ_H
#define SYSTEM_GRAPHIC_2D_2DSPROBJ_H

#include "System/Graphic/2d/2DObj.h"
#include "System/Vector.h"

namespace D2 {

struct SPR_GROUP;
struct SPR_PARTS;
struct SEQ_ANIMATION;
struct LAY_SCREEN;

class C2DSprObj : public C2DObj {
private:
    FVECTOR m_vRot;         /* 0x010 */
    UBYTE _m_0x20[0xea];    /* 0x020 m_vCol .. 0x109 m_iStep */
    UBYTE m_Flg;            /* 0x10a */
    CHAR m_fgRotTransPers;  /* 0x10b */
public:
    static INT m_fgParent;

    C2DSprObj(SeqControl* pSeqControl, const TexInfo* pTexInfo);
    virtual ~C2DSprObj();
    void init(UINT nSpr);
    void initSprite(INT nIdx, SPR_GROUP* pSprGroup, SPR_PARTS* pSprParts);
    TIMEQ initForSeq(SEQ_ANIMATION* pSeqAnimation, LAY_SCREEN* pLayScreen);
    void setLastFlag() { m_Flg |= 4; } /* name unknown */
    virtual void _main(TIMEQ qTime);
    virtual INT _disp();
    virtual void enableDisp(BOOL bEnable);
    virtual void restart();
    virtual void shiftBase(FLOAT fX, FLOAT fY);
    virtual void shiftColor(FLOAT fR, FLOAT fG, FLOAT fB, FLOAT fA);
    virtual void setFukidasiSize(FLOAT fSizeX, FLOAT fSizeY);
protected:
    virtual void changeTexture(INT inNum, const TexInfo* pTexInfo, G_UV* pStart, G_UV* pEnd);
    virtual void changeLayer(INT nLayer);
};

}  // namespace D2

#endif /* SYSTEM_GRAPHIC_2D_2DSPROBJ_H */
