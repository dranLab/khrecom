#ifndef SYSTEM_GRAPHIC_2D_2DLAYOBJ_H
#define SYSTEM_GRAPHIC_2D_2DLAYOBJ_H

#include "System/Graphic/2d/2DSeqObj.h"

namespace D2 {

struct LAY_SCREEN;

class C2DLayObj : public C2DO {
    friend class C2DMan; // PARTIAL: System/Graphic/2d/2DMan resets m_uiID
private:
    static UINT m_uiID;
    HANDLE* m_pSeqObjHandle; /* 0xc */
    INT m_nSeqObj;           /* 0x10 */
    TIMEQ m_qTimeMax;        /* 0x14 */

public:
    C2DLayObj();
    virtual ~C2DLayObj();
    TIMEQ getTime();
    TIMEQ getTimeMax() { return m_qTimeMax; }
    HANDLE create(HANDLE handle, UINT nScreen, LAY_SCREEN* pLayScreen, HANDLE* pSeqHandle);
    BOOL isEnd();
    void enableDisp(HANDLE handle, BOOL bEnable);
};

}  // namespace D2

#endif /* SYSTEM_GRAPHIC_2D_2DLAYOBJ_H */
