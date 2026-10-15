// PARTIAL: added for Game/Atel/AtelTutorial
#ifndef SYSTEM_GRAPHIC_2D_2DSEQCTRL_H
#define SYSTEM_GRAPHIC_2D_2DSEQCTRL_H

#include "khtypes.h"
#include "System/Vector.h"

/* PARTIAL: added for Form/SystemMessage */
struct SeqControl {
    u_long128 _m_0x000[0xe0 / 16];  /* 0x000 matrix ... controlColor */
    FVECTOR vPos;                   /* 0x0e0 PARTIAL: added for Form/Menu/MenuParts */
    u_long128 _m_0x0f0[2];          /* 0x0f0 vRot, vScl */
    FVECTOR vCol;                   /* 0x110 PARTIAL: added for Form/Menu/Roulette */
    SeqControl* pParentSeqControl;  /* 0x120 */
    CHAR fgColor;                   /* 0x124 */
    CHAR fgBilinear;                /* 0x125 */
    CHAR fgFntScale;                /* 0x126 */
    CHAR fg3D;                      /* 0x127 */
};

struct G_UV; /* PARTIAL: added for Form/Menu/WorldWarp */

namespace D2 {

class C2DSeqObj;
class C2DFnt;

class C2DSeqCtrl {
public: /* PARTIAL: public as in the debug info - Form/Menu/MenuParts reads m_seqControl.vPos */
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
    C2DSeqCtrl(INT inSw); /* PARTIAL: added for Form/Menu/DecknameEdit */
    virtual ~C2DSeqCtrl();
    virtual C2DSeqObj* addSeq(LPCTSTR pSedName, UINT idxSeq, UINT nLayer);
    virtual C2DSeqObj* addSeq(LPCTSTR pSedName, UINT idxSeq);
    /* PARTIAL: added for Form/SystemMessage - the overloads with a layer are guesses */
    virtual C2DSeqObj* addFnt(LPCTSTR pSqdName, UINT nLayer, C2DFnt* p2DFnt); /* PARTIAL: order from the symbol, for Game/Player/NaviMap */
    virtual C2DSeqObj* addFnt(LPCTSTR pSqdName, C2DFnt* p2DFnt);
    virtual C2DSeqObj* replaceSeq(UINT nIdx, LPCTSTR pSedName, UINT idxSeq, UINT nLayer);
    virtual C2DSeqObj* replaceSeq(UINT nIdx, LPCTSTR pSedName, UINT idxSeq);
    virtual C2DSeqObj* replaceFnt(UINT nIdx, LPCTSTR pSedName, C2DFnt* p2DFnt, UINT nLayer);
    virtual C2DSeqObj* replaceFnt(UINT nIdx, LPCTSTR pSedName, C2DFnt* p2DFnt);
    virtual C2DSeqObj* addSeqShift(LPCTSTR pSedName, UINT idxSeq, UINT nLayer, FLOAT fShiftX, FLOAT fShiftY);
    virtual C2DSeqObj* addSeqShift(LPCTSTR pSedName, UINT idxSeq, FLOAT fShiftX, FLOAT fShiftY);
    virtual C2DSeqObj* addFntShift(LPCTSTR pSqdName, C2DFnt* p2DFnt, UINT nLayer, FLOAT fShiftX, FLOAT fShiftY);
    virtual C2DSeqObj* addFntShift(LPCTSTR pSqdName, C2DFnt* p2DFnt, FLOAT fShiftX, FLOAT fShiftY);
    /* PARTIAL: added for Form/Menu/MonstroGauge - [14]..[19]; signatures from the symbols */
    virtual C2DSeqObj* replaceSeqShift(UINT nIdx, LPCTSTR pSedName, UINT idxSeq, UINT nLayer, FLOAT fShiftX, FLOAT fShiftY);
    virtual C2DSeqObj* replaceSeqShift(UINT nIdx, LPCTSTR pSedName, UINT idxSeq, FLOAT fShiftX, FLOAT fShiftY);
    virtual C2DSeqObj* replaceFntShift(UINT nIdx, LPCTSTR pSqdName, UINT nLayer, C2DFnt* p2DFnt, FLOAT fShiftX, FLOAT fShiftY);
    virtual C2DSeqObj* replaceFntShift(UINT nIdx, LPCTSTR pSqdName, C2DFnt* p2DFnt, FLOAT fShiftX, FLOAT fShiftY);
    virtual C2DSeqObj* shiftSeqPos(UINT nIdx, FLOAT fShiftX, FLOAT fShiftY);
    virtual C2DSeqObj* setFntPos(UINT nIdx, FLOAT fX, FLOAT fY);
    void destroy();                                           /* PARTIAL: added for Game/Player/NaviMap */
    void setLocalPos(FVECTOR& vPos);
    void setPos(FVECTOR& vPos);                               /* PARTIAL: added for Form/Menu/GameOver */
    BOOL isEnd(UINT nIdx);                                    /* PARTIAL: added for Form/Menu/GameOver */
    void setLayer(INT nLayer) { m_nLayer = nLayer; }          /* PARTIAL: added for Form/Menu/GameOver */
    void enableDisp(INT nIdx, BOOL bEnable);                  /* PARTIAL: added for Form/SystemMessage */
    void changeLayer(INT nLayer);                             /* PARTIAL: added for Form/SystemMessage */
    void changeTime(UINT nIdx, UINT qTime);                  /* PARTIAL: added for Form/Menu/MonstroGauge */
    void enableDisp(BOOL bEnable);                            /* PARTIAL: added for Form/Menu/MenuParts */
    void setScl(FVECTOR& vScl);                               /* PARTIAL: added for Form/Menu/MenuParts */
    void setParent(C2DSeqCtrl* pParent);                      /* PARTIAL: added for Form/Menu/MenuParts */
    void destroy(UINT nIdx);                                  /* PARTIAL: added for Form/Menu/WorldSelect */
    void setScl(FLOAT fScl);                                  /* PARTIAL: added for Form/Menu/WorldSelect */
    void setRot(FVECTOR& vRot);                               /* PARTIAL: added for Form/Menu/WorldSelect */
    UINT getNumSeq() { return m_nSeq; }                       /* PARTIAL: added for Form/Menu/WorldSelect */
    void changeTexture(UINT nIdx, INT inSprIdx, CHAR* pTexName, G_UV* pStart, G_UV* pEnd); /* PARTIAL: added for Form/Menu/WorldWarp */
    void setCol(FVECTOR& vCol);                               /* PARTIAL: added for Form/Menu/RoomCreation */
    void enableDispEach(BOOL bEnable);                        /* PARTIAL: added for Form/Menu/RoomCreation */
    void enableColorCtrl(BOOL bEnable) { m_seqControl.fgColor = bEnable; } /* PARTIAL: added for Form/SystemMessage */
};

}  // namespace D2

#endif /* SYSTEM_GRAPHIC_2D_2DSEQCTRL_H */
