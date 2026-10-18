#ifndef SYSTEM_GRAPHIC_TEXANM_H
#define SYSTEM_GRAPHIC_TEXANM_H

#include "khtypes.h"

struct VTransInfo;
struct TexInfoStub;
struct _TEXANM2;

struct _TEXANM_HEAD {
    int id;
    short ver;
    short nNum;
    int pad[2];
};

struct _TEXANM1 {
    char szName[16];
    short nJump;
    short nNum;
    union {
        int nOffset;
        _TEXANM2* pData2;
    };
};

struct _TEXANM2 {
    union {
        int nOffset;
        void* pDataTex;
        struct {
            unsigned char nFromPar;
            unsigned char nToPar;
            unsigned char nPad[2];
        };
    };
    short Frame0;
    short Frame1;
};

struct _TEXANM0 {
    char szName[16];
    char szDstTexName[24];
    union {
        struct {
            short DstX;
            short DstY;
            short DstW;
            short DstH;
        };
        struct {
            union {
                int nOffsetClt;
                void* pDataClt;
            };
            int DstWH;
        };
    };
    short nNum;
    short nPad;
    union {
        int nOffset;
        _TEXANM1* pData1;
    };

    BOOL isClutAnm() { return DstWH == 0; }
};

struct TEXANM_DATA {
    _TEXANM_HEAD sHead;
    _TEXANM0 sData[1];

    void mapping();
    void check() const;
    int getDataNum() const;
    int getSeqNum(int nDataNum) const;
};

class CTexAnm {
private:
    _TEXANM0* m_pData;      /* 0x00 */
    _TEXANM1* m_pPlayData;  /* 0x04 */
    u_long128* m_pClut;     /* 0x08 */
    u_long128* m_pClutSrc;  /* 0x0c */
    INT m_nClutQSize;       /* 0x10 */
    INT m_nPlayNum;         /* 0x14 */
    INT m_nSeqIdx;          /* 0x18 */
    FLOAT m_fTimer;         /* 0x1c */
    FLOAT m_fTimeMax;       /* 0x20 */

public:
    CTexAnm();
    ~CTexAnm();
    void setup(INT nGroup, INT nAnmGroupNum);
    void setup(const TEXANM_DATA& inData, INT nAnmGroupNum);
    INT setSeq(const CHAR* pszAnmName);
    INT setSeq(INT nAnmNum);
    void update(FLOAT fR);
    CHAR* getDstTexName();
    CHAR* getDefaultName();
    void sendTexture(USHORT nTBP, const VTransInfo& Tinfo, USHORT nCBP, const VTransInfo& Cinfo);
private:
    void _setupTimer();
    void _updateClut();
};

class CModelTexAnm {
private:
    INT m_nTexAnmNum;     /* 0x0 */
    CTexAnm** m_pTexAnm;  /* 0x4 */
    UBYTE* m_pID;         /* 0x8 */

public:
    CModelTexAnm();
    ~CModelTexAnm();
    void setup(CTexAnm* pATbl, INT nANum, TexInfoStub* pTTbl, INT nTNum);
    BOOL check(INT nID);
    void sendTexture(INT nID, USHORT nTBP, const VTransInfo& Tinfo, USHORT nCBP, const VTransInfo& Cinfo);
    static INT getTexAnmCount(CTexAnm* pATbl, INT nANum, TexInfoStub* pTTbl, INT nTNum);
private:
    void _dispose();
};

#endif /* SYSTEM_GRAPHIC_TEXANM_H */
