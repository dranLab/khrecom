#ifndef SYSTEM_GRAPHIC_2D_2DFNT_H
#define SYSTEM_GRAPHIC_2D_2DFNT_H

#include "System/Graphic/StrPrim.h"

namespace D2 {

class C2DFnt : public CStrPrim {
public:
    INT m_seqFntSize;

    C2DFnt(INT nInX, INT nInY, const G_RGBA* pInColor, INT nInSize, const CHAR* lpszInText, UINT nInStyle);
    C2DFnt();
    virtual ~C2DFnt();
    virtual void drawFont(INT nInCode);
};

}  // namespace D2

#endif /* SYSTEM_GRAPHIC_2D_2DFNT_H */
