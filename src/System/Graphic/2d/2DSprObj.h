#ifndef SYSTEM_GRAPHIC_2D_2DSPROBJ_H
#define SYSTEM_GRAPHIC_2D_2DSPROBJ_H

#include "System/Graphic/2d/2DObj.h"
#include "System/Vector.h"
#include "System/Graphic/Gprim.h"

union FVECTOR2 {
    float v[2];
    struct {
        FLOAT x;
        FLOAT y;
    };
    ULONGLONG _u64;
    LONGLONG _s64;
    void set(FLOAT fX, FLOAT fY) {
        x = fX;
        y = fY;
    }
    void init0() { _u64 = 0; }
};

namespace D2 {

struct SPR_GROUP;
struct SPR_PARTS;
struct SEQ_ANIMATION;
struct LAY_SCREEN;

struct SEQ_STATUS {
    UINT curve_disable : 1;
    UINT active : 1;
    UINT bilinear_disable : 1;
    UINT sway_lag : 1;
    UINT sway_disable : 1;
    UINT rotate_disable : 1;
    UINT scale_disable : 1;
    UINT color_stop : 1;
    UINT rotate_stop : 1;
    UINT scale_stop : 1;
    UINT color_disable : 1;
    UINT sway_stop : 1;
    UINT base_stop : 1;
    UINT ofs_stop : 1;
    UINT ofs_disable : 1;
    UINT last_cut : 1;
    UINT base_disable : 1;
    UINT base_ofs_disable : 1;
    UINT pos_disable : 1;
    UINT tag : 1;
};

struct SprPrim {
    SPR_GROUP* pSprGroupe;
    SPR_PARTS* pSprParts;
    CPolyGT4ST polyGT4;
};

class C2DSprObj : public C2DObj {
private:
    FVECTOR m_vRot;          /* 0x010 */
    FVECTOR m_vCol;          /* 0x020 */
    FVECTOR m_vRotate[2];    /* 0x030 */
    FVECTOR m_vColor[2];     /* 0x050 */
    FVECTOR m_vCurve;        /* 0x070 */
    FVECTOR m_vDiff;         /* 0x080 */
    FVECTOR2 m_vPos;         /* 0x090 */
    FVECTOR2 m_vOfs;         /* 0x098 */
    FVECTOR2 m_vScl;         /* 0x0a0 */
    FVECTOR2 m_vSwy;         /* 0x0a8 */
    FVECTOR2 m_vBase[2];     /* 0x0b0 */
    FVECTOR2 m_vOffset[2];   /* 0x0c0 */
    FVECTOR2 m_vScale[2];    /* 0x0d0 */
    FVECTOR2 m_vSway[2];     /* 0x0e0 */
    int m_qTime[2];          /* 0x0f0 */
    SEQ_STATUS m_status;     /* 0x0f8 */
    SprPrim* m_pSprPrim;     /* 0x0fc */
    CScissoring* m_pScissoring; /* 0x100 */
    const TexInfo* m_pTexInfo;  /* 0x104 */
    BYTE m_nPoly;            /* 0x108 */
    CHAR m_iStep;            /* 0x109 */
    UBYTE m_Flg;             /* 0x10a */
    CHAR m_fgRotTransPers;   /* 0x10b */
public:
    static INT m_fgParent;

    C2DSprObj(SeqControl* pSeqControl, const TexInfo* pTexInfo);
    virtual ~C2DSprObj();
    virtual void _main(TIMEQ qTime);
    virtual INT _disp();
    void init(UINT nSpr);
    void initSprite(INT inNum, SPR_GROUP* pSprGroupe, SPR_PARTS* pSprParts);
    TIMEQ initForSeq(SEQ_ANIMATION* pSeqAnim, LAY_SCREEN* pLayScreen);
    virtual void restart();
    virtual void shiftBase(FLOAT fX, FLOAT fY);
    virtual void setFukidasiSize(FLOAT fSizeX, FLOAT fSizeY);
    void setLast(BOOL bVal) {
        if (bVal) {
            m_Flg |= 4;
        } else {
            m_Flg &= ~4;
        }
    }
private:
    void move();
    virtual void changeTexture(INT inNum, const TexInfo* pTexInfo, G_UV* pStart, G_UV* pEnd);
    virtual void changeLayer(INT nLayer);
    void getRect(FVECTOR2* pvOut4, SprPrim& rSprPrim);
    void getFukidasiRect(FVECTOR2 (*vOut)[4]);
    void setBilinear(BOOL bVal) {
        if (bVal) {
            m_Flg |= 2;
        } else {
            m_Flg &= ~2;
        }
    }
    INT isBilinear() { return (m_Flg >> 1) & 1; }
};

}  // namespace D2

void ApplyMatrix2(FVECTOR& v0, const FMATRIX& m0, const FVECTOR2& v2);
void AddVector2(FVECTOR2& v0, const FVECTOR2& v1, const FVECTOR2& v2);
void AddVector2(FVECTOR& v0, const FVECTOR2& v1, const FVECTOR2& v2);
void InterVector2(FVECTOR2& v0, const FVECTOR2& v1, const FVECTOR2& v2, float t);
void InterVector2(FVECTOR& v0, const FVECTOR2& v1, const FVECTOR2& v2, float t);

#endif /* SYSTEM_GRAPHIC_2D_2DSPROBJ_H */
