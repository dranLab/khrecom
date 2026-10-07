// PARTIAL: added for System/Graphic/2d/2DFnt
#ifndef SYSTEM_GRAPHIC_GPRIM_H
#define SYSTEM_GRAPHIC_GPRIM_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"

/* __FILE__ of this header in the original differs per includer ("System/Graphic/Gprim.h" in most, also
 * "c:/KHCOM/MainApp/System/Graphic/Gprim.h" and "System/../System/Graphic/Gprim.h"): define
 * GPRIM_H_PATH before including it to pick another one. Line numbers in messages are the original ones. */
#ifndef GPRIM_H_PATH
#define GPRIM_H_PATH "System/Graphic/Gprim.h"
#endif

typedef USHORT ALPHA;

struct G_XY {
    FIX4 x;
    FIX4 y;
};

struct G_XYZ {
    FIX4 x;
    FIX4 y;
    FIX4 z;
};

struct G_RGBA {
    union {
        struct {
            UBYTE r;
            UBYTE g;
            UBYTE b;
            UBYTE a;
        };
        UINT col;
        UINT c;
        unsigned char v[4];
    };
};

union G_LAYER {
    INT _nInfo;
    struct {
        USHORT _nID;
        USHORT _nDepth;
    };
};

union FVERTEX {
    struct {
        int iPad[3];
        G_RGBA C;
    };
    float f[4];
};

struct FTVERTEX {
    float fT[4];
    union {
        struct {
            int iPad[3];
            G_RGBA C;
        };
        float fP[4];
    };
};

class CDrawable {
private:
    CDrawable* m_pNext;
protected:
    FIX4 m_nZ;
    USHORT m_iFlag;
    ALPHA m_nAlphaMode;
    UINT m_nTestFlag;
    G_LAYER m_LayerInfo;
public:
    virtual void disp();
    virtual UINT getPacketSize() const;
private:
    virtual void makePkt();
};

class CGPrim : public CDrawable {
protected:
    UINT m_nAttribute;
public:
    virtual ~CGPrim();
    virtual void disp();
    virtual FIX4 calcZ();
};

class CGTexPrim : public CGPrim {
protected:
    struct TexInfo* m_pTexInfo;
public:
    virtual ~CGTexPrim();
    virtual UINT getPacketSize() const;
};

class CCPrim : public CGPrim {
private:
    UINT m_PRIM;
    UINT m_TEST;
    ULONGLONG m_CLAMP;
    ULONGLONG m_SCISSOR;
    UINT m_ALPHA;
public:
    INT m_iMax;
    FVERTEX* m_V;

    void create(ULONGLONG iPrim, INT iVertexNum) {
        remove();
        m_iMax = iVertexNum;
        m_PRIM = iPrim;
#line 613 GPRIM_H_PATH
        m_V = (FVERTEX*)com_malloc_d(iVertexNum * sizeof(FVERTEX), __FILE__ "(613)", (UINT)-1);
        if (!m_V) AssertApp(__FILE__, __LINE__, "m_V", 0);
    }
    void remove();
};

class CTPrim : public CGTexPrim {
private:
    UINT m_PRIM;
    UINT m_TEST;
    ULONGLONG m_CLAMP;
    ULONGLONG m_SCISSOR;
    UINT m_ALPHA;
public:
    INT m_iMax;
    FTVERTEX* m_V;

    void create(ULONGLONG iPrim, INT iVertexNum) {
        remove();
        m_iMax = iVertexNum;
        m_PRIM = iPrim;
#line 684
        m_V = (FTVERTEX*)com_malloc_d(iVertexNum * sizeof(FTVERTEX), __FILE__ "(684)", (UINT)-1);
        if (!m_V) AssertApp(__FILE__, __LINE__, "m_V", 0);
    }
    void remove();
};

#endif /* SYSTEM_GRAPHIC_GPRIM_H */
