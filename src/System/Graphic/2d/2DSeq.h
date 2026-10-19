#ifndef SYSTEM_GRAPHIC_2D_2DSEQ_H
#define SYSTEM_GRAPHIC_2D_2DSEQ_H

#include "System/Graphic/2d/2DSpr.h"

namespace D2 {

class C2DSeqObj;
struct SEQ_ANIMATION;
struct LAY_SCREEN;

struct SEQ_SEQUENCE {
    USHORT pos;
    USHORT max;
    USHORT loop;
    USHORT bilinear_disable : 1;
    USHORT ablend_disable : 1;
    USHORT rotate_disable : 1;
    USHORT scale_disable : 1;
    USHORT color_disable : 1;
    USHORT scroll_disable : 1;
    USHORT pos_disable : 1;
    USHORT active_disable : 1;
    USHORT curve_disable : 1;
    USHORT sway_disable : 1;
    UINT frame;
    UINT end;
    INT x;
    INT y;
    INT size;
    INT cr;
    INT ex;
};

class C2DSeq : public C2DSpr {
    friend class C2DSeqCtrl; // PARTIAL: System/Graphic/2d/2DSeqCtrl reads m_pCurrent2DSeqObj
private:
    SEQ_ANIMATION* m_pSeqAnimation; /* 0xd0 */
    SEQ_SEQUENCE* m_pSeqSequence;   /* 0xd4 */
    INT m_nAnimationMax;            /* 0xd8 */
    INT m_nSequenceMax;             /* 0xdc */
    static C2DSeqObj* m_pCurrent2DSeqObj;

public:
    C2DSeq();
    virtual ~C2DSeq();
    virtual BOOL init(void* pSqd, const CHAR* pBaseName);
    virtual HANDLE create(UINT inNo);
    virtual HANDLE create(UINT inNo, SeqControl* pSeqControl);
    HANDLE create(LAY_SCREEN* pLayScreen);
    virtual HANDLE createSeq(UINT inUnitNo, UINT inNo, SeqControl* pSeqControl);
    HANDLE createFnt(C2DFnt* p2DFnt, SeqControl* pSeqControl);
    HANDLE replaceSeq(HANDLE hCurrent, UINT inNo, SeqControl* pSeqControl);
    HANDLE replaceFnt(HANDLE hCurrent, C2DFnt* p2DFnt, SeqControl* pSeqControl);
    virtual HANDLE createSpr(UINT inUnitNo, UINT inNo);
    virtual INT getIdMax() { return m_nSequenceMax; }
    virtual INT getSprIdMax(UINT inUnitNo) { return C2DSpr::getIdMax(); }
    virtual TIMEQ getTime(HANDLE handle);
    virtual TIMEQ getTimeMax(HANDLE handle);
    virtual BOOL isEnd(HANDLE handle);
    static C2DSeqObj* getCurrent2DSeqObj() { return m_pCurrent2DSeqObj; }
};

}  // namespace D2

#endif /* SYSTEM_GRAPHIC_2D_2DSEQ_H */
