
#ifndef SYSTEM_GRAPHIC_2D_2DSEQCTRL_H
#define SYSTEM_GRAPHIC_2D_2DSEQCTRL_H

#include "khtypes.h"
#include "System/Vector.h"


struct G_UV;

namespace D2 {

struct SeqControl {
    FMATRIX matrix;                 /* 0x000 */
    FMATRIX parentMatrix;           /* 0x040 */
    FMATRIX controlMatrix;          /* 0x080 */
    FVECTOR parentColor;            /* 0x0c0 */
    FVECTOR controlColor;           /* 0x0d0 */
    FVECTOR vPos;                   /* 0x0e0 */
    FVECTOR vRot;                   /* 0x0f0 */
    FVECTOR vScl;                   /* 0x100 */
    FVECTOR vCol;                   /* 0x110 */
    SeqControl* pParentSeqControl;  /* 0x120 */
    CHAR fgColor;                   /* 0x124 */
    CHAR fgBilinear;                /* 0x125 */
    CHAR fgFntScale;                /* 0x126 */
    CHAR fg3D;                      /* 0x127 */
};

class C2DSeqObj;
class C2DFnt;

/* All inline members are emitted (weak) in System/Graphic/2d/2DSeqCtrl.cpp, in declaration order: keep the order. */
class C2DSeqCtrl {
public: /* public as in the debug info - Form/Menu/MenuParts reads m_seqControl.vPos */
    SeqControl m_seqControl;  /* 0x000 */
private:
    static unsigned char m_bitTbl[8];
    CHAR m_bitEnable;         /* 0x130 */
protected:
    int m_hSeqHandle[8];      /* 0x134 */
    CHAR m_nType;             /* 0x154 */
    CHAR m_nSeq;              /* 0x155 */
    CHAR m_nLayer;            /* 0x156 */
    CHAR m_bEnable;           /* 0x157 */

public:
    C2DSeqCtrl();
    C2DSeqCtrl(INT nType);
    virtual ~C2DSeqCtrl();
    virtual C2DSeqObj* addSeq(const CHAR* pLadName, UINT idxUnit, UINT idxSeq);
    virtual C2DSeqObj* addSeq(const CHAR* pSedName, UINT idxSeq);
    virtual C2DSeqObj* addFnt(const CHAR* pLadName, UINT idxUnit, C2DFnt* p2DFnt);
    virtual C2DSeqObj* addFnt(const CHAR* pSqdName, C2DFnt* p2DFnt);
    virtual C2DSeqObj* replaceSeq(UINT nIdx, const CHAR* pLadName, UINT idxUnit, UINT idxSeq);
    virtual C2DSeqObj* replaceSeq(UINT nIdx, const CHAR* pSedName, UINT idxSeq);
    virtual C2DSeqObj* replaceFnt(UINT nIdx, const CHAR* pLadName, UINT idxUnit, C2DFnt* p2DFnt);
    virtual C2DSeqObj* replaceFnt(UINT nIdx, const CHAR* pSedName, C2DFnt* p2DFnt);
    virtual C2DSeqObj* addSeqShift(const CHAR* pLadName, UINT idxUnit, UINT idxSeq, FLOAT fShiftX, FLOAT fShiftY);
    virtual C2DSeqObj* addSeqShift(const CHAR* pSedName, UINT idxSeq, FLOAT fShiftX, FLOAT fShiftY);
    virtual C2DSeqObj* addFntShift(const CHAR* pLadName, UINT idxUnit, C2DFnt* p2DFnt, FLOAT fShiftX, FLOAT fShiftY);
    virtual C2DSeqObj* addFntShift(const CHAR* pSqdName, C2DFnt* p2DFnt, FLOAT fShiftX, FLOAT fShiftY);
    virtual C2DSeqObj* replaceSeqShift(UINT nIdx, const CHAR* pLadName, UINT idxUnit, UINT idxSeq, FLOAT fShiftX,
                                       FLOAT fShiftY);
    virtual C2DSeqObj* replaceSeqShift(UINT nIdx, const CHAR* pSedName, UINT idxSeq, FLOAT fShiftX, FLOAT fShiftY);
    virtual C2DSeqObj* replaceFntShift(UINT nIdx, const CHAR* pLadName, UINT idxUnit, C2DFnt* p2DFnt, FLOAT fShiftX,
                                       FLOAT fShiftY);
    virtual C2DSeqObj* replaceFntShift(UINT nIdx, const CHAR* pSedName, C2DFnt* p2DFnt, FLOAT fShiftX, FLOAT fShiftY);
    virtual C2DSeqObj* shiftSeqPos(UINT nIdx, FLOAT fShiftX, FLOAT fShiftY);
    virtual C2DSeqObj* setFntPos(UINT nIdx, FLOAT fShiftX, FLOAT fShiftY);
    void enableDisp(INT nIdx, BOOL bEnable);
    void enableDisp(BOOL bEnable);
    void enableDispForce(BOOL bEnable);
    void enableDispEach(BOOL bEnable);
    void enableDispOnlyFlag(BOOL bEnable) { m_bEnable = bEnable; }
    void setLayer(INT nLayer) { m_nLayer = nLayer; }
    void enableColorCtrl(BOOL bEnable) { m_seqControl.fgColor = bEnable; }
    void enableFntScaleCtrl(BOOL bEnable) { m_seqControl.fgFntScale = bEnable; }
    BOOL isEnableDisp() { return m_bEnable; }
    BOOL isEnableDisp(INT nIdx);
    void destroy(UINT nIdx);
    void destroy();
    void destroyLast();
    void destroyIndexToLast(UINT nIdx);
    void setPos(FVECTOR& vPos);
    void setLocalPos(FVECTOR& vPos);
    void set3DPos(FVECTOR& vPos);
    void setRot(FVECTOR& vRot);
    void setScl(FLOAT fScl);
    void setScl(FVECTOR& vScl);
    void setCol(FVECTOR& vCol);
    void addPosX(FLOAT fPosX);
    void addPosY(FLOAT fPosY);
    void addRotX(FLOAT fRotX);
    void addRotY(FLOAT fRotY);
    void addRotZ(FLOAT fRotZ);
    void changeTime(UINT inTime);
    void changeTime(UINT nIdx, UINT inTime);
    TIMEQ getTime(UINT nIdx);
    void changeLayer(INT nLayer);
    void changeLayer(UINT nIdx, INT nLayer);
    void restart(UINT nIdx, FLOAT fStart);
    BOOL isEnd(UINT nIdx);
    void changeTexture(UINT nIdx, INT inSeqIdx, INT inSprIdx, CHAR* pTexName, G_UV* pStart, G_UV* pEnd);
    void changeTexture(UINT nIdx, INT inSprIdx, CHAR* pTexName, G_UV* pStart, G_UV* pEnd);
    UINT getNumSeq() { return m_nSeq; }
    HANDLE* getHandle() { return m_hSeqHandle; }
    void setParent(C2DSeqCtrl* p2DSeqCtrl);

private:
    void initialize();
    void setMatrix();
};

}  // namespace D2

#endif /* SYSTEM_GRAPHIC_2D_2DSEQCTRL_H */
