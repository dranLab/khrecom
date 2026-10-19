#ifndef SYSTEM_GRAPHIC_2D_2DSEQOBJ_H
#define SYSTEM_GRAPHIC_2D_2DSEQOBJ_H

#include "khtypes.h"
#include "System/SJoint.h"
#include "System/Graphic/2d/2DObj.h"

struct G_UV;
struct TexInfo;
namespace D2 { struct SeqControl; } // PARTIAL: in D2 as in the original
namespace D2 { // PARTIAL: added for System/Graphic/2d/2DSeq
class C2DFnt;
class C2DObj;
struct SEQ_SEQUENCE;
struct SEQ_ANIMATION;
struct SPR_GROUP;
struct SPR_PARTS;
struct SPR_SPRITE;
struct LAY_SCREEN;
class C2DSprObj;

struct SPR_PARTS { // PARTIAL: added for System/Graphic/2d/2DSeqObj
    UINT palette;
    UINT u0;
    UINT v0;
    UINT u1;
    UINT v1;
    FLOAT s_scroll;
    FLOAT t_scroll;
    BYTE r0, g0, b0, a0;
    BYTE r1, g1, b1, a1;
    BYTE r2, g2, b2, a2;
    BYTE r3, g3, b3, a3;
};

struct SEQ_ANIMATION { // PARTIAL: added for System/Graphic/2d/2DSeqObj (members after sprite_id as padding)
    UINT status;
    INT sprite_id;
    UBYTE _m_0x08[0x88];
};
}

class CScissoring;
class C2DCard;

namespace D2 {
class C2DObjList : public CSJointListT<C2DObj> {
public:
    ~C2DObjList() {
        for (C2DObj *pEntry = getHead(), *__pn = pEntry ? pEntry->getNext() : NULL; pEntry;
             pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL) {
            delete pEntry;
        }
        resetParam();
    }
};
}

namespace D2 {

class C2DO {
public: // PARTIAL: inline (weak in the original) and public as in the debug info, for System/Graphic/2d/2DLayObj
    HANDLE m_hBaseHandle; /* 0x0 */
    HANDLE m_hHandle;     /* 0x4 */
    /* vtable 0x8 */

    C2DO() : m_hBaseHandle(0), m_hHandle(0) {}
    virtual ~C2DO() {}
    HANDLE getBaseHandle() { return m_hBaseHandle; }
    HANDLE _getHandle() { return m_hHandle; }
    void _setHandle(HANDLE h) { m_hHandle = h; } // PARTIAL: added for System/Graphic/2d/2DSpr
    virtual void _main() {}
    virtual void _disp() {}
    virtual void changeTimeExt(INT qTime) {}
    virtual void changeTexture(INT nIdx, CHAR* pName, G_UV* pStart, G_UV* pEnd) {}
    virtual void changeTexture(INT nIdx, INT nIdx2, CHAR* pName, G_UV* pStart, G_UV* pEnd) {}
    virtual void restart(INT nFlag) {}
    virtual void changeLayer(INT nLayer) {}
    virtual void enableDisp(INT bEnable) {}
};

class C2DSeqObj : public C2DO {
    friend class ::C2DCard; // PARTIAL: System/Graphic/2d/2DCard sets m_fgMainSkip
    friend class C2DSeqCtrl; // PARTIAL: System/Graphic/2d/2DSeqCtrl clears m_fgParent
    friend class C2DMan;     // PARTIAL: System/Graphic/2d/2DMan resets m_uiID
private:
    static UINT m_uiID;
    CScissoring* m_pScissoringEnd;  /* 0x0c */
    C2DObjList* m_p2DObjList;       /* 0x10 */
    SeqControl* m_pSeqControl;      /* 0x14 */
    TIMEQ m_qTime;                  /* 0x18 */
    TIMEQ m_qTimeMax;               /* 0x1c */
    TIMEQ m_qTimeExt;               /* 0x20 */
    INT m_nLoop;                    /* 0x24 */
    INT m_nLoopFrame;               /* 0x28 */
    CHAR m_bSeq;                    /* 0x2c */
    CHAR m_bEnable;                 /* 0x2d */
    CHAR m_bEnd;                    /* 0x2e */
    CHAR m_fgParent;                /* 0x2f */
    CHAR m_fgMainSkip;              /* 0x30 */

public:
    C2DSeqObj();
    C2DSeqObj(SeqControl* pSeqControl);
    virtual ~C2DSeqObj();
    virtual void _main();
    virtual void _disp();
    HANDLE create(HANDLE handle, UINT nSpr, const TexInfo* pTexInfo, SPR_GROUP* pSprGroupe, SPR_PARTS* pSprParts);
    HANDLE create(HANDLE handle, const TexInfo* pTexInfo, SEQ_SEQUENCE* pSeqSequence, SEQ_ANIMATION* pSeqAnimation,
                  SPR_GROUP* pSprGroupe, SPR_PARTS* pSprParts, SPR_SPRITE* pSprSprite, LAY_SCREEN* pLayScreen);
    HANDLE createFnt(HANDLE handle, C2DFnt* p2DFnt);
    TIMEQ getTime() { return m_qTime; }
    TIMEQ getTimeMax() { return m_qTimeMax; }
    virtual void changeTexture(INT inSprIdx, CHAR* pTexName, G_UV* pStart, G_UV* pEnd);
    virtual void changeTexture(INT inSeqIdx, INT inSprIdx, CHAR* pTexName, G_UV* pStart, G_UV* pEnd);
    virtual void enableDisp(BOOL bEnable) { m_bEnable = bEnable; }
    BOOL isDisp() { return m_bEnable; }
    virtual void changeTimeExt(TIMEQ qTimeExt);
    virtual void changeLayer(INT nLayer);
    BOOL isEnd() { return m_bEnd; }
    virtual void restart(TIMEQ qStart);
    void shiftBase(FLOAT fX, FLOAT fY);
    void shiftColor(FLOAT fR, FLOAT fG, FLOAT fB, FLOAT fA);
    void setParent() { m_fgParent = 0; }
    void setFukidasiSize(FLOAT fSizeX, FLOAT fSizeY);
    INT getLoopTimes() { return m_nLoop; }
    void setLoopTimes(INT nLoopTimes) { m_nLoop = nLoopTimes; }
    C2DObjList* get2DObjList() { return m_p2DObjList; }

private:
    void initialize();
    C2DSprObj* createSub(UINT nSpr, const TexInfo* pTexInfo, SPR_GROUP* pSprGroupe, SPR_PARTS* pSprParts);
    void restart();
};

}  // namespace D2

#endif /* SYSTEM_GRAPHIC_2D_2DSEQOBJ_H */
