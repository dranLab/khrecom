#ifndef SYSTEM_GRAPHIC_2D_2DFNTOBJ_H
#define SYSTEM_GRAPHIC_2D_2DFNTOBJ_H

#include "System/Graphic/2d/2DObj.h"
#include "System/Vector.h"

namespace D2 {

class C2DFnt;

class C2DFntObj : public C2DObj {
private:
    FVECTOR m_vPos;    /* 0x10 */
    FVECTOR m_vCol;    /* 0x20 */
    INT m_iStep;       /* 0x30 */
    C2DFnt* m_p2DFnt;  /* 0x34 */

public:
    C2DFntObj(SeqControl* pSeqControl);
    virtual ~C2DFntObj();
    virtual void _main(TIMEQ qTime);
    virtual INT _disp();
    void move();
    TIMEQ init(C2DFnt* p2DFnt);
    virtual void changeLayer(INT nLayer);
    virtual void shiftBase(FLOAT fX, FLOAT fY);
    virtual void shiftColor(FLOAT fR, FLOAT fG, FLOAT fB, FLOAT fA);
    void clear2DFnt() { m_p2DFnt = NULL; }
    FVECTOR& getPos() { return m_vPos; }
    void setPos(FVECTOR& vPos) { m_vPos = vPos; }
    FVECTOR& getCol() { return m_vCol; }
    void setCol(FVECTOR& vCol) { m_vCol = vCol; }
};

}  // namespace D2

#endif /* SYSTEM_GRAPHIC_2D_2DFNTOBJ_H */
