#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKELNS_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKELNS_H

#include "System/Graphic/Effect/libppp/partMng.h"
#include "System/Graphic/Effect/libppp/keProg/KeLns.h"
#include "System/Graphic/Effect/libppp/keProg/Ke.h"
#include "System/Graphic/Effect/libppp/draw/draw_w.h"

/* libgraph */
typedef struct { /* 0x8 */
    u_long FBP : 9;
    u_long pad09 : 7;
    u_long FBW : 6;
    u_long pad22 : 2;
    u_long PSM : 6;
    u_long pad30 : 2;
    u_long FBMSK : 32;
} sceGsFrame;

typedef struct { /* 0x8 */
    u_long ZBP : 9;
    u_long pad09 : 15;
    u_long PSM : 4;
    u_long pad28 : 4;
    u_long ZMSK : 1;
    u_long pad33 : 31;
} sceGsZbuf;

typedef struct { /* 0x8 */
    u_long AC : 1;
    u_long pad01 : 63;
} sceGsPrmodecont;

typedef struct { /* 0x8 */
    u_long CLAMP : 1;
    u_long pad01 : 63;
} sceGsColclamp;

typedef struct { /* 0x8 */
    u_long DTHE : 1;
    u_long pad01 : 63;
} sceGsDthe;

typedef struct { /* 0x80 */
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


extern u_long ppvGsFrame;
extern u_long ppvGsZbuf;
extern u_long ppvGsXyoffset;
extern u_long ppvGsTest;
extern u_long ppvGsDthe;
extern u_long ppvGsColorClamp;

void sceVu0FTOI4Vector(sceVu0IVECTOR v0, sceVu0FVECTOR v1);
int sceGsSetDefStoreImage(sceGsStoreImage *sp, short sbp, short sbw, short spsm, short x, short y, short w,
                          short h);
int sceGsExecStoreImage(sceGsStoreImage *sp, u_long128 *dstaddr);
int sceGsSyncPath(int mode, u_short timeout);

typedef struct _KeLnsClmHd { /* 0x10 */
    KeLnsLp *Lp;
    pppShapeSt *Shp;
    u_char Num;
    u_char Flg;
    short Betw;
    short StOfs;
    u_char FlgType;
    char _P0[1];
} KeLnsClmHd;

typedef struct { /* 0x10 */
    pppCDT cdt;
    float LWid;
    float Pow;
    u_char R;
    u_char Md;
} PKeLnsLp;

enum {
    IKeLnsArnd_This = 0,
    IKeLnsArnd_KeLnsLp = 1,
    IKeLnsArnd_DMatrix = 2
};

typedef struct { /* 0x10 */
    pppCDT cdt;
    int Shp;
    pppCVECTOR Col;
    float Scl;
} PKeLnsArnd;

typedef struct {
    KeLnsArnd Arnd;
} VKeLnsArnd;

enum {
    IKeLnsFls_This = 0,
    IKeLnsFls_KeLnsLp = 1,
    IKeLnsFls_DMatrix = 2
};

typedef struct { /* 0x14 */
    pppCDT cdt;
    int Shp;
    pppCVECTOR Col;
    float Scl;
    u_short PLen;
    u_char Typ0;
    u_char Typ1;
} PKeLnsFls;

typedef struct {
    KeLnsFls Fls;
} VKeLnsFls;

enum {
    IKeLnsClm_This = 0,
    IKeLnsClm_KeLnsLp = 1,
    IKeLnsClm_DMatrix = 2
};

typedef struct { /* 0x18 */
    pppCDT cdt;
    int Shp;
    pppCVECTOR Col;
    float Scl;
    short StOfs;
    short Betw;
    u_short R;
    u_char Seed;
    u_char FlgType;
} PKeLnsClm;

typedef struct { /* 0x30 */
    KeLnsClmHd Hd;
    KeLnsClmFlr Flr[4];
} VKeLnsClm;

enum {
    IKeLnsCrn_This = 0,
    IKeLnsCrn_KeLnsLp = 1,
    IKeLnsCrn_DMatrix = 2
};

typedef struct { /* 0x14 */
    pppCDT cdt;
    int Shp;
    pppCVECTOR Col;
    float Scl;
    u_short PLen;
    u_short DLen;
} PKeLnsCrn;

typedef struct {
    KeLnsCrn Crn;
} VKeLnsCrn;

enum {
    IKeLnsLpT_This = 0,
    IKeLnsLpT_Matrix = 1
};

typedef struct { /* 0x14 */
    pppCDT cdt;
    float LWid;
    float Pow;
    u_char R;
    u_char Md;
    u_char SmplW;
    u_char SmplH;
    u_char Mbk;
} PKeLnsLpT;

typedef struct { /* 0xd0 */
    KeLnsLp Lp;
    sceVu0FVECTOR ScrnPos;
    u_char SmplW;
    u_char SmplH;
} VKeLnsLpT;

enum {
    IKeLnsArndT_This = 0,
    IKeLnsArndT_KeLnsLpT = 1,
    IKeLnsArndT_DMatrix = 2
};

typedef struct { /* 0x10 */
    pppCDT cdt;
    int Shp;
    pppCVECTOR Col;
    float Scl;
} PKeLnsArndT;

typedef struct { /* 0x30 */
    KeLnsArnd Arnd;
    char dummy0[4];
    sceVu0FVECTOR SclVec;
    float Scl;
    u_char Col[4];
} VKeLnsArndT;

enum {
    IKeLnsFlsT_This = 0,
    IKeLnsFlsT_KeLnsLp = 1,
    IKeLnsFlsT_DMatrix = 2
};

typedef struct { /* 0x14 */
    pppCDT cdt;
    int Shp;
    pppCVECTOR Col;
    float Scl;
    u_short PLen;
    u_char Typ0;
    u_char Typ1;
} PKeLnsFlsT;

typedef struct { /* 0x30 */
    KeLnsFls Fls;
    char dummy0[4];
    sceVu0FVECTOR SclVec;
    float Scl;
    u_char Col[4];
    float PLen;
    u_short Typ;
} VKeLnsFlsT;

enum {
    IKeLnsClmT_This = 0,
    IKeLnsClmT_KeLnsLpT = 1,
    IKeLnsClmT_DMatrix = 2
};

typedef struct { /* 0x18 */
    pppCDT cdt;
    int Shp;
    pppCVECTOR Col;
    float Scl;
    short StOfs;
    short Betw;
    u_short R;
    u_char Seed;
    u_char FlgType;
} PKeLnsClmT;

typedef struct { /* 0x50 */
    KeLnsClmHd Hd;
    KeLnsClmFlr Flr[4];
    sceVu0FVECTOR SclVec;
    float R;
    float Scl;
    u_char Col[4];
} VKeLnsClmT;

enum {
    IKeLnsCrnT_This = 0,
    IKeLnsCrnT_KeLnsLp = 1,
    IKeLnsCrnT_DMatrix = 2
};

typedef struct { /* 0x14 */
    pppCDT cdt;
    int Shp;
    pppCVECTOR Col;
    float Scl;
    u_short PLen;
    u_short DLen;
} PKeLnsCrnT;

typedef struct { /* 0x30 */
    KeLnsCrn Crn;
    sceVu0FVECTOR SclVec;
    float Scl;
    u_char Col[4];
    float PLen;
    float DLen;
} VKeLnsCrnT;

void pppKeLnsLp(pppPObject *pobj, PKeLnsLp *dat, pppCtrlTable *ctbl);
void pppKeLnsLpDraw(pppPObject *pobj, PKeLnsLp *dat, pppCtrlTable *ctbl);
void pppKeLnsLpCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeLnsLpCon2(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeLnsArndUpdate(pppPObject *pobj, PKeLnsArnd *dat, pppCtrlTable *ctbl);
void pppKeLnsArnd(pppPObject *pobj, PKeLnsArnd *dat, pppCtrlTable *ctbl);
void pppKeLnsArndDraw(pppPObject *pobj, PKeLnsArnd *dat, pppCtrlTable *ctbl);
void pppKeLnsArndCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeLnsClmUpdate(pppPObject *pobj, PKeLnsClm *dat, pppCtrlTable *ctbl);
void pppKeLnsClm(pppPObject *pobj, PKeLnsClm *dat, pppCtrlTable *ctbl);
void pppKeLnsClmDraw(pppPObject *pobj, PKeLnsClm *dat, pppCtrlTable *ctbl);
void pppKeLnsClmCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeLnsCrnUpdate(pppPObject *pobj, PKeLnsCrn *dat, pppCtrlTable *ctbl);
void pppKeLnsCrn(pppPObject *pobj, PKeLnsCrn *dat, pppCtrlTable *ctbl);
void pppKeLnsCrnDraw(pppPObject *pobj, PKeLnsCrn *dat, pppCtrlTable *ctbl);
void pppKeLnsCrnCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeLnsFlsUpdate(pppPObject *pobj, PKeLnsFls *dat, pppCtrlTable *ctbl);
void pppKeLnsFls(pppPObject *pobj, PKeLnsFls *dat, pppCtrlTable *ctbl);
void pppKeLnsFlsDraw(pppPObject *pobj, PKeLnsFls *dat, pppCtrlTable *ctbl);
void pppKeLnsFlsCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeLnsLpT(pppPObject *pobj, PKeLnsLpT *dat, pppCtrlTable *ctbl);
void pppKeLnsLpTDraw(pppPObject *pobj, PKeLnsLpT *dat, pppCtrlTable *ctbl);
void pppKeLnsLpTCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeLnsLpTCon2(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeLnsLpTDes(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeLnsArndT(pppPObject *pobj, PKeLnsArndT *dat, pppCtrlTable *ctbl);
void pppKeLnsArndTDraw(pppPObject *pobj, PKeLnsArndT *dat, pppCtrlTable *ctbl);
void pppKeLnsArndTCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeLnsArndTDes(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeLnsClmT(pppPObject *pobj, PKeLnsClmT *dat, pppCtrlTable *ctbl);
void pppKeLnsClmTDraw(pppPObject *pobj, PKeLnsClmT *dat, pppCtrlTable *ctbl);
void pppKeLnsClmTCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeLnsClmTDes(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeLnsCrnT(pppPObject *pobj, PKeLnsCrnT *dat, pppCtrlTable *ctbl);
void pppKeLnsCrnTDraw(pppPObject *pobj, PKeLnsCrnT *dat, pppCtrlTable *ctbl);
void pppKeLnsCrnTCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeLnsCrnTDes(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeLnsFlsT(pppPObject *pobj, PKeLnsFlsT *dat, pppCtrlTable *ctbl);
void pppKeLnsFlsTDraw(pppPObject *pobj, PKeLnsFlsT *dat, pppCtrlTable *ctbl);
void pppKeLnsFlsTCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeLnsFlsTDes(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKELNS_H */
