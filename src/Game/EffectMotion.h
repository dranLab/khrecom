#ifndef GAME_EFFECTMOTION_H
#define GAME_EFFECTMOTION_H

#include "khtypes.h"
#include "Game/AttachMotion.h"

union FVECTOR;
class CEntity;
class CEffectMotion;

struct ATTACH_EFFECT_DATA {
    char szDpxName[32]; /* 0x00 */
    INT iEffectNo;      /* 0x20 */
    INT iBoneIndex;     /* 0x24 */
    FLOAT fOffsetX;     /* 0x28 */
    FLOAT fOffsetY;     /* 0x2c */
    FLOAT fOffsetZ;     /* 0x30 */
    TIMEQ tqStart;      /* 0x34 */
    TIMEQ tqEnd;        /* 0x38 */
    FLOAT fRotX;        /* 0x3c */
    FLOAT fRotY;        /* 0x40 */
    FLOAT fRotZ;        /* 0x44 */
    FLOAT fScaleX;      /* 0x48 */
    FLOAT fScaleY;      /* 0x4c */
    FLOAT fScaleZ;      /* 0x50 */
    UINT uiFlag;        /* 0x54 */
    FLOAT fFadeTime;    /* 0x58 */
    INT iNo;            /* 0x5c */
    FLOAT fSkipFrame;   /* 0x60 */
    INT iSetCount;      /* 0x64 */
    INT iZOffset;       /* 0x68 */
    HANDLE handle;      /* 0x6c */
    INT step;           /* 0x70 */
    INT count;          /* 0x74 */
    BOOL set;           /* 0x78 */
    int pad[9];         /* 0x7c */

    TIMEQ getStart() const { return tqStart * 50; }
    TIMEQ getEnd() const { return tqEnd * 50; }
    UINT isAttachOff() const { return uiFlag & 0x1; }
    UINT isParentRotOff() const { return uiFlag & 0x2; }
    UINT isAttachReverseTrs() const { return uiFlag & 0x4; }
    UINT isLoopFadeOut() const { return uiFlag & 0x8; }
    UINT isLoopEnd() const { return uiFlag & 0x10; }
    UINT isLoopContinue() const { return uiFlag & 0x20; }
    UINT isLightOn() const { return uiFlag & 0x40; }
    UINT isNoGetOwnerAlpha() const { return uiFlag & 0x80; }
    UINT isViewClipOff() const { return uiFlag & 0x100; }
    UINT isNoAttachScale() const { return uiFlag & 0x200; }
};

struct LOCUS_EFFECT_DATA {
    char szDpxName[32]; /* 0x00 */
    INT iEffectNo;      /* 0x20 */
    INT iBoneIndex1;    /* 0x24 */
    FLOAT fOffsetX1;    /* 0x28 */
    FLOAT fOffsetY1;    /* 0x2c */
    FLOAT fOffsetZ1;    /* 0x30 */
    INT iBoneIndex2;    /* 0x34 */
    FLOAT fOffsetX2;    /* 0x38 */
    FLOAT fOffsetY2;    /* 0x3c */
    FLOAT fOffsetZ2;    /* 0x40 */
    INT iBoneIndex3;    /* 0x44 */
    FLOAT fOffsetX3;    /* 0x48 */
    FLOAT fOffsetY3;    /* 0x4c */
    FLOAT fOffsetZ3;    /* 0x50 */
    TIMEQ tqStart;      /* 0x54 */
    TIMEQ tqEnd;        /* 0x58 */
    INT iNo;            /* 0x5c */
    FLOAT fFadeTime;    /* 0x60 */
    HANDLE handle;      /* 0x64 */
    INT step;           /* 0x68 */
    int pad[13];        /* 0x6c */

    TIMEQ getStart() const { return tqStart * 50; }
    TIMEQ getEnd() const { return tqEnd * 50; }
};

struct ATTACH_EFFECT_DATA_TBL {
    char szMotionName[32];          /* 0x00 */
    INT iMotionIndex;               /* 0x20 */
    SHORT sAttachDataNum;           /* 0x24 */
    SHORT sLocusDataNum;            /* 0x26 */
    ATTACH_EFFECT_DATA* pAttachData; /* 0x28 */
    LOCUS_EFFECT_DATA* pLocusData;  /* 0x2c */
};

/* addrMapping is inline; its strings are not in the .rodata of every includer of this header (Game/Player/
 * PlayerWeapon), so Game/EffectMotion.cpp defines it. */
struct EET_HEADER {
    char szCode[4];                   /* 0x0 */
    UINT uiVersion;                   /* 0x4 */
    INT iDataTblNum;                  /* 0x8 */
    ATTACH_EFFECT_DATA_TBL* pDataTbl; /* 0xc */

    BOOL addrMapping();
};

class CEffectMotionCtrl {
private:
    INT m_iStep;                            /* 0x00 */
    INT m_iCount;                           /* 0x04 */
    const ATTACH_EFFECT_DATA* m_pAttachEffectData; /* 0x08 */
    const LOCUS_EFFECT_DATA* m_pLocusEffectData;   /* 0x0c */
    HANDLE m_hHandle;                       /* 0x10 */
    CEntity* m_pOwner;                      /* 0x14 */
    CEffectMotion* m_pParent;               /* 0x18 */

public:
    CEffectMotionCtrl(CEffectMotion* inParent);
    ~CEffectMotionCtrl();
    BOOL initalize(CEntity* inOwner);
    void set(const ATTACH_EFFECT_DATA* inData);
    void set(const LOCUS_EFFECT_DATA* inData);
    void update(TIMEQ inCurrentTime);
    HANDLE getHandle() { return m_hHandle; }
    const ATTACH_EFFECT_DATA* getAttachEffectData() { return m_pAttachEffectData; }
    const LOCUS_EFFECT_DATA* getLocusEffectData() { return m_pLocusEffectData; }
    void stop();
    void detach();
    void kill();
    void kill(FLOAT inFadeTime);

private:
    HANDLE fadeoutEffect_(FLOAT inTime);
    BOOL updateAttachEffect_(TIMEQ inCurrentTime);
    BOOL updateLocusEffect_(TIMEQ inCurrentTime);
};

struct OBSERVE_EFFECT {
    HANDLE hHandle;
    const ATTACH_EFFECT_DATA* pAttachEffectData;
    const LOCUS_EFFECT_DATA* pLocusEffectData;
    BOOL bStop;
};

class CEffectMotion : public CAttachMotion {
private:
    INT m_iID;                          /* 0x008 */
    EET_HEADER* m_pHeader;              /* 0x00c */
    INT m_iEffMotCtrlCount;             /* 0x010 */
    INT m_iNowCtrlCount;                /* 0x014 */
    INT m_iObserveEffectCount;          /* 0x018 */
    OBSERVE_EFFECT m_ObserveEffect[64]; /* 0x01c */
    CEffectMotionCtrl* m_pEffMotCtrl;   /* 0x41c */

public:
    CEffectMotion(CEntity* inOwner);
    virtual ~CEffectMotion();
    BOOL initalize(INT inID);
    BOOL initalize(const CHAR* inRsrcName);
    virtual BOOL set(INT inIndex) { return set_(inIndex, 0, TRUE); }
    BOOL set(INT inIndex, INT inNo, BOOL inStop) { return set_(inIndex, inNo, inStop); }
    virtual void stop();
    void detach();
    void kill(FLOAT inFadeTime);
    virtual void update();
    void killLoopEffect(BOOL inLoopEnd, INT inNo);
    void setObserveEffect(HANDLE inHandle, const ATTACH_EFFECT_DATA* inAttach, const LOCUS_EFFECT_DATA* inLocus);
    void setColor(const FVECTOR& inColor);
    void setAlpha(FLOAT inAlpha);

private:
    BOOL initialize_(EET_HEADER* inHeader);
    BOOL set_(INT inIndex, INT inNo, BOOL inStop);
    void updateObserveEffect_();
    BOOL doLoopEffect_(const ATTACH_EFFECT_DATA* inData);
    OBSERVE_EFFECT* getObserveEffect_();
    void releaseObserveEffect_();
};

#endif /* GAME_EFFECTMOTION_H */
