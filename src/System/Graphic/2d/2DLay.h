#ifndef SYSTEM_GRAPHIC_2D_2DLAY_H
#define SYSTEM_GRAPHIC_2D_2DLAY_H

#include "System/Graphic/2d/2DBase.h"

namespace D2 {

struct LAY_SCREEN {
    UINT image_unit;
    UINT sequence_unit;
    UINT sequence_id;
    INT time;
    INT x;
    INT y;
};

struct LAY_LAYOUT {
    USHORT pos;
    USHORT max;
    int ex[4];
};

struct LAY_SEQUENCE_UNIT {
    UINT ofs;
};

class C2DLay : public C2DBase {
private:
    LAY_SCREEN* m_pLayScreen;               /* 0x28 */
    LAY_LAYOUT* m_pLayLayout;               /* 0x2c */
    LAY_SEQUENCE_UNIT* m_pLaySequenceUnit;  /* 0x30 */
    INT m_nScreenMax;                       /* 0x34 */
    INT m_nLayoutMax;                       /* 0x38 */
    INT m_nSequenceUnitMax;                 /* 0x3c */
    HANDLE* m_pSeqHandle;                   /* 0x40 */

public:
    C2DLay();
    virtual ~C2DLay();
    void _main();
    void _disp();
    virtual TIMEQ getTime(HANDLE handle);
    virtual TIMEQ getTimeMax(HANDLE handle);
    virtual BOOL isEnd(HANDLE handle);
    virtual void enableDisp(HANDLE handle, BOOL bEnable);
    LAY_SCREEN* getLayScreen(UINT inNo);

private:
    virtual BOOL init(void* pLad, const CHAR* pBaseName);
    virtual HANDLE create(UINT inNo);
    virtual HANDLE createSeq(UINT inUnitNo, UINT inNo, SeqControl* pSeqControl);
    virtual HANDLE createSpr(UINT inUnitNo, UINT inNo);
    virtual void destroy(HANDLE handle);
    virtual INT getIdMax() { return m_nLayoutMax; }
    virtual INT getSeqIdMax(UINT inUnitNo);
    virtual INT getSprIdMax(UINT inUnitNo);
    virtual INT getUnitIdMax() { return m_nSequenceUnitMax; }
};

}  // namespace D2

#endif /* SYSTEM_GRAPHIC_2D_2DLAY_H */
