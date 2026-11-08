/* libgraph.h / eestruct.h (Sony EE SDK): the GS register types the game and libppp use. The ref has no owner for
   them (lost+found.h), except sceGifTag and sceGsDrawEnv1, which it lists in System/Graphic/PackMan.h: they are
   here because the libppp units (C) use them too. PackMan.h has the environments built from them. */
#ifndef SDK_LIBGRAPH_H
#define SDK_LIBGRAPH_H

#include "khtypes.h"

typedef struct {
    unsigned int EN1 : 1;
    unsigned int EN2 : 1;
    unsigned int CRTMD : 3;
    unsigned int MMOD : 1;
    unsigned int AMOD : 1;
    unsigned int SLBG : 1;
    unsigned int ALP : 8;
    unsigned int p0 : 16;
    unsigned int p1;
} tGS_PMODE;

typedef struct {
    unsigned int INT : 1;
    unsigned int FFMD : 1;
    unsigned int DPMS : 2;
    unsigned int p0 : 28;
    unsigned int p1;
} tGS_SMODE2;

typedef struct {
    unsigned int FBP : 9;
    unsigned int FBW : 6;
    unsigned int PSM : 5;
    unsigned int p0 : 12;
    unsigned int DBX : 11;
    unsigned int DBY : 11;
    unsigned int p1 : 10;
} tGS_DISPFB2;

typedef struct {
    unsigned int DX : 12;
    unsigned int DY : 11;
    unsigned int MAGH : 4;
    unsigned int MAGV : 2;
    unsigned int p0 : 3;
    unsigned int DW : 12;
    unsigned int DH : 11;
    unsigned int p1 : 9;
} tGS_DISPLAY2;

typedef struct {
    unsigned int R : 8;
    unsigned int G : 8;
    unsigned int B : 8;
    unsigned int p0 : 8;
    unsigned int p1;
} tGS_BGCOLOR;

typedef struct {
    u_long NLOOP : 15;
    u_long EOP : 1;
    u_long pad16 : 16;
    u_long id : 14;
    u_long PRE : 1;
    u_long PRIM : 11;
    u_long FLG : 2;
    u_long NREG : 4;
    u_long REGS0 : 4;
    u_long REGS1 : 4;
    u_long REGS2 : 4;
    u_long REGS3 : 4;
    u_long REGS4 : 4;
    u_long REGS5 : 4;
    u_long REGS6 : 4;
    u_long REGS7 : 4;
    u_long REGS8 : 4;
    u_long REGS9 : 4;
    u_long REGS10 : 4;
    u_long REGS11 : 4;
    u_long REGS12 : 4;
    u_long REGS13 : 4;
    u_long REGS14 : 4;
    u_long REGS15 : 4;
} sceGifTag;

typedef struct {
    u_long CLAMP : 1;
    u_long pad01 : 63;
} sceGsColclamp;

typedef struct {
    u_long DTHE : 1;
    u_long pad01 : 63;
} sceGsDthe;

typedef struct {
    u_long FBP : 9;
    u_long pad09 : 7;
    u_long FBW : 6;
    u_long pad22 : 2;
    u_long PSM : 6;
    u_long pad30 : 2;
    u_long FBMSK : 32;
} sceGsFrame;

typedef struct {
    u_long PRIM : 3;
    u_long IIP : 1;
    u_long TME : 1;
    u_long FGE : 1;
    u_long ABE : 1;
    u_long AA1 : 1;
    u_long FST : 1;
    u_long CTXT : 1;
    u_long FIX : 1;
    u_long pad11 : 53;
} sceGsPrim;

typedef struct {
    u_long AC : 1;
    u_long pad01 : 63;
} sceGsPrmodecont;

typedef struct {
    u_int R : 8;
    u_int G : 8;
    u_int B : 8;
    u_int A : 8;
    float Q;
} sceGsRgbaq;

typedef struct {
    u_long SCAX0 : 11;
    u_long pad11 : 5;
    u_long SCAX1 : 11;
    u_long pad27 : 5;
    u_long SCAY0 : 11;
    u_long pad43 : 5;
    u_long SCAY1 : 11;
    u_long pad59 : 5;
} sceGsScissor;

typedef struct {
    u_long ATE : 1;
    u_long ATST : 3;
    u_long AREF : 8;
    u_long AFAIL : 2;
    u_long DATE : 1;
    u_long DATM : 1;
    u_long ZTE : 1;
    u_long ZTST : 2;
    u_long pad19 : 45;
} sceGsTest;

typedef struct {
    u_long OFX : 16;
    u_long pad16 : 16;
    u_long OFY : 16;
    u_long pad48 : 16;
} sceGsXyoffset;

typedef struct {
    u_long X : 16;
    u_long Y : 16;
    u_long Z : 32;
} sceGsXyz;

typedef struct {
    u_long ZBP : 9;
    u_long pad09 : 15;
    u_long PSM : 4;
    u_long pad28 : 4;
    u_long ZMSK : 1;
    u_long pad33 : 31;
} sceGsZbuf;

typedef struct {
    sceGsFrame frame1;
    u_long frame1addr;
    sceGsZbuf zbuf1;
    long zbuf1addr;
    sceGsXyoffset xyoffset1;
    long xyoffset1addr;
    sceGsScissor scissor1;
    long scissor1addr;
    sceGsPrmodecont prmodecont;
    long prmodecontaddr;
    sceGsColclamp colclamp;
    long colclampaddr;
    sceGsDthe dthe;
    long dtheaddr;
    sceGsTest test1;
    long test1addr;
} sceGsDrawEnv1;

#endif /* SDK_LIBGRAPH_H */
