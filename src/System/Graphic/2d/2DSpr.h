#ifndef SYSTEM_GRAPHIC_2D_2DSPR_H
#define SYSTEM_GRAPHIC_2D_2DSPR_H

#include "System/Graphic/2d/2DBase.h"
#include "System/Vector.h"

template <class T> class CSLCListT;

struct TexInfo;

namespace D2 {

class C2DO;
class C2DSeqObj;
struct SPR_PARTS;

struct SPR_HEADER {
    unsigned char id[4];
    USHORT version;
    unsigned char padding[2];
    UINT parts_max;   /* 0x08 */
    UINT parts_ofs;   /* 0x0c */
    UINT group_max;   /* 0x10 */
    UINT group_ofs;   /* 0x14 */
    UINT sprite_max;  /* 0x18 */
    UINT sprite_ofs;  /* 0x1c */
};

struct SPR_GROUP {
    INT x0;
    INT y0;
    INT x1;
    INT y1;
    UINT parts_id;
};

struct SPR_SPRITE {
    USHORT pos;
    USHORT max;
};

typedef CSLCListT<C2DO> C2DOSLCList;

class C2DSpr : public C2DBase {
protected:
    SPR_PARTS* m_pSprParts;    /* 0x28 */
    SPR_GROUP* m_pSprGroup;    /* 0x2c */
    SPR_SPRITE* m_pSprSprite;  /* 0x30 */
    TexInfo* m_pTexInfo;       /* 0x34 */
    char m_pTexName[24];       /* 0x38 */
    static C2DOSLCList* m_p2DOSLCList;
private:
    static INT m_n2DSeqObjListSelect;
    static INT m_n2DSeqObjListSelectSave;
    INT m_nPartsMax;           /* 0x50 */
    INT m_nGroupMax;           /* 0x54 */
    INT m_nSpriteMax;          /* 0x58 */
    FLOAT m_fSpeed;            /* 0x5c */
    FLOAT m_fCounter;          /* 0x60 */
    FLOAT m_fTimeCounter;      /* 0x64 */
    INT m_nNumber;             /* 0x68 */
    FLOAT m_fShade;            /* 0x6c */
    FLOAT m_fScale;            /* 0x70 */
    FVECTOR m_vRotate;         /* 0x80 */
    FVECTOR m_vScale;          /* 0x90 */
    IVECTOR m_vPosition;       /* 0xa0 */
    IVECTOR m_vOffset;         /* 0xb0 */
    UINT m_gRGBA;              /* 0xc0 (G_RGBA) */
    UINT m_uiDrawZ;            /* 0xc4 */
    static C2DOSLCList* m_p2DOSLCL[4];

public:
    C2DSpr();
    virtual ~C2DSpr();
    static void initializeList();
    static void _main(UINT nLevel);
    static void _disp(UINT nLevel);
    static void destroyAll();
    static void destroyAllForSceneChange();
    static void change2DSeqObjList(UINT nList);
    virtual void destroy(HANDLE handle);
    virtual INT getIdMax() { return m_nSpriteMax; }
    virtual HANDLE createSpr(UINT inUnitNo, UINT inNo);
    C2DSeqObj* getSeqObj(HANDLE handle);
    virtual void enableDisp(HANDLE handle, BOOL bEnable);
    void resetTexInfo();
    static HANDLE addList(C2DO& Obj);
    static BOOL deleteList(C2DO& Obj);
    static HANDLE replaceList(HANDLE hCurrent, C2DO& Obj);
protected:
    virtual HANDLE create(UINT inNo);
    virtual BOOL init(void* pSpd, const CHAR* pBaseName);
};

}  // namespace D2

#endif /* SYSTEM_GRAPHIC_2D_2DSPR_H */
