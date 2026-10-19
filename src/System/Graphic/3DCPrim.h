// PARTIAL: added for Game/Collision/CollDisp (original header unknown; only what that unit uses)
#ifndef SYSTEM_GRAPHIC_3DCPRIM_H
#define SYSTEM_GRAPHIC_3DCPRIM_H

#include "System/Graphic/Gprim.h"

class CCoord;
class CCoords;

class C3DCPrim : public CGPrim {
private:
    static unsigned char m_PackReglist[8];
    UINT m_PRIM;          /* 0x1c */
    UINT m_TEST;          /* 0x20 */
    ULONGLONG m_CLAMP;    /* 0x28 */
    ULONGLONG m_SCISSOR;  /* 0x30 */
    UINT m_ALPHA;         /* 0x38 */
    UINT m_ZBUF;          /* 0x3c */
    CCoord* m_pCrd;       /* 0x40 */
    CCoords* m_pObj;      /* 0x44 */
    FLOAT m_fGetaZ;       /* 0x48 */
    INT m_iZ;             /* 0x4c */

public:
    FLOAT m_fGeta;        /* 0x50 */
    USHORT m_iRep;        /* 0x54 */
    USHORT m_iMax;        /* 0x56 */
    G_STRIP* m_V;         /* 0x58 */

    C3DCPrim() { _init(); }
    virtual ~C3DCPrim();
    void create(ULONGLONG iPrim, INT iVertexNum, CCoord* pCrd, CCoords* pObj, INT iRep) {
        _create(iPrim, iVertexNum, pCrd, pObj, iRep);
    }
    CCoord* getCoordPtr() { return m_pCrd; }
    virtual UINT getPacketSize() const;

private:
    void _init();
    void _create(ULONGLONG iPrim, INT iVertexNum, CCoord* pCrd, CCoords* pObj, INT iRep);
    virtual FIX4 calcZ();
    virtual void makePkt();
};

#endif /* SYSTEM_GRAPHIC_3DCPRIM_H */
