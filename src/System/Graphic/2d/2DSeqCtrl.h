// PARTIAL: added for Game/Atel/AtelTutorial
#ifndef SYSTEM_GRAPHIC_2D_2DSEQCTRL_H
#define SYSTEM_GRAPHIC_2D_2DSEQCTRL_H

#include "khtypes.h"

union FVECTOR;

/* PARTIAL: added for Form/SystemMessage */
struct SeqControl {
    u_long128 _m_0x000[0x120 / 16]; /* 0x000 matrix ... vCol */
    SeqControl* pParentSeqControl;  /* 0x120 */
    CHAR fgColor;                   /* 0x124 */
    CHAR fgBilinear;                /* 0x125 */
    CHAR fgFntScale;                /* 0x126 */
    CHAR fg3D;                      /* 0x127 */
};

namespace D2 {

class C2DSeqObj;
class C2DFnt;

class C2DSeqCtrl {
protected:
    SeqControl m_seqControl;  /* 0x000 */
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
    /* PARTIAL: added for Form/SystemMessage - the overloads with a layer are guesses */
    virtual C2DSeqObj* addFnt(LPCTSTR pSqdName, C2DFnt* p2DFnt, UINT nLayer);
    virtual C2DSeqObj* addFnt(LPCTSTR pSqdName, C2DFnt* p2DFnt);
    virtual C2DSeqObj* replaceSeq(UINT nIdx, LPCTSTR pSedName, UINT idxSeq, UINT nLayer);
    virtual C2DSeqObj* replaceSeq(UINT nIdx, LPCTSTR pSedName, UINT idxSeq);
    virtual C2DSeqObj* replaceFnt(UINT nIdx, LPCTSTR pSedName, C2DFnt* p2DFnt, UINT nLayer);
    virtual C2DSeqObj* replaceFnt(UINT nIdx, LPCTSTR pSedName, C2DFnt* p2DFnt);
    virtual C2DSeqObj* addSeqShift(LPCTSTR pSedName, UINT idxSeq, UINT nLayer, FLOAT fShiftX, FLOAT fShiftY);
    virtual C2DSeqObj* addSeqShift(LPCTSTR pSedName, UINT idxSeq, FLOAT fShiftX, FLOAT fShiftY);
    virtual C2DSeqObj* addFntShift(LPCTSTR pSqdName, C2DFnt* p2DFnt, UINT nLayer, FLOAT fShiftX, FLOAT fShiftY);
    virtual C2DSeqObj* addFntShift(LPCTSTR pSqdName, C2DFnt* p2DFnt, FLOAT fShiftX, FLOAT fShiftY);
    void setLocalPos(FVECTOR& vPos);
    void setPos(FVECTOR& vPos);                               /* PARTIAL: added for Form/Menu/GameOver */
    BOOL isEnd(UINT nIdx);                                    /* PARTIAL: added for Form/Menu/GameOver */
    void setLayer(INT nLayer) { m_nLayer = nLayer; }          /* PARTIAL: added for Form/Menu/GameOver */
    void enableDisp(INT nIdx, BOOL bEnable);                  /* PARTIAL: added for Form/SystemMessage */
    void changeLayer(INT nLayer);                             /* PARTIAL: added for Form/SystemMessage */
    void enableColorCtrl(BOOL bEnable) { m_seqControl.fgColor = bEnable; } /* PARTIAL: added for Form/SystemMessage */
};

}  // namespace D2

#endif /* SYSTEM_GRAPHIC_2D_2DSEQCTRL_H */
