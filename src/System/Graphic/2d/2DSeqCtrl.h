// PARTIAL: added for Game/Atel/AtelTutorial
#ifndef SYSTEM_GRAPHIC_2D_2DSEQCTRL_H
#define SYSTEM_GRAPHIC_2D_2DSEQCTRL_H

#include "khtypes.h"

union FVECTOR;

namespace D2 {

class C2DSeqObj;
class C2DFnt;

class C2DSeqCtrl {
protected:
    u_long128 _m_0x000[0x130 / 16]; /* 0x000 m_seqControl */
private:
    CHAR m_bitEnable;      /* 0x130 */
protected:
    int m_hSeqHandle[8];   /* 0x134 */
    CHAR m_nType;          /* 0x154 */
    CHAR m_nSeq;           /* 0x155 */
    CHAR m_nLayer;         /* 0x156 */
    CHAR m_bEnable;        /* 0x157 */

public:
    C2DSeqCtrl();
    virtual ~C2DSeqCtrl();
    virtual C2DSeqObj* addSeq(LPCTSTR pSedName, UINT idxSeq, UINT nLayer);
    virtual C2DSeqObj* addSeq(LPCTSTR pSedName, UINT idxSeq);
    void setLocalPos(FVECTOR& vPos);
};

}  // namespace D2

#endif /* SYSTEM_GRAPHIC_2D_2DSEQCTRL_H */
