#ifndef GAME_SEMOTION_H
#define GAME_SEMOTION_H

#include "khtypes.h"
#include "Game/AttachMotion.h"

class CEntity;
class CSEMotion;

struct SE_DATA {
    INT iFileID;         /* 0x00 */
    INT iSeID;           /* 0x04 */
    INT iBoneIndex;      /* 0x08 */
    FLOAT fOffsetX;      /* 0x0c */
    FLOAT fOffsetY;      /* 0x10 */
    FLOAT fOffsetZ;      /* 0x14 */
    TIMEQ tqStart;       /* 0x18 */
    TIMEQ tqEnd;         /* 0x1c */
    INT iNo;             /* 0x20 */
    INT iFadeTime;       /* 0x24 */
    UINT uiFlag;         /* 0x28 */
    SHORT iCallCount;    /* 0x2c */
    SHORT sPriority;     /* 0x2e */
    HANDLE handle;       /* 0x30 */
    SHORT step;          /* 0x34 */
    SHORT selID;         /* 0x36 */
    INT count;           /* 0x38 */
    BOOL set;            /* 0x3c */
    short sRandom[4][2]; /* 0x40 */

    TIMEQ getStart() const { return tqStart * 50; }
    TIMEQ getEnd() const { return tqEnd * 50; }
    UINT isFoot() const { return uiFlag & 0x1; }
    UINT isVoice() const { return uiFlag & 0x2; }
    UINT isRandom() const { return uiFlag & 0x4; }
};

struct SE_DATA_TBL {
    char szMotionName[32]; /* 0x00 */
    INT iMotionIndex;      /* 0x20 */
    INT iSeDataNum;        /* 0x24 */
    SE_DATA* pSeData;      /* 0x28 */
    INT pad;               /* 0x2c */
};

struct SES_HEADER {
    char szCode[4];        /* 0x0 */
    UINT uiVersion;        /* 0x4 */
    INT iDataTblNum;       /* 0x8 */
    SE_DATA_TBL* pDataTbl; /* 0xc */

    BOOL addrMapping();
};

struct RANDOM_DATA {
    const SE_DATA* pSrc; /* 0x0 */
    INT iBefore;         /* 0x4 */
};

class CSEMotionCtrl {
private:
    INT m_iStep;             /* 0x00 */
    INT m_iCounter;          /* 0x04 */
    INT m_iFileID;           /* 0x08 */
    INT m_iSeID;             /* 0x0c */
    INT m_iFootChange;       /* 0x10 */
    BOOL m_bVolumeOff;       /* 0x14 */
    const SE_DATA* m_pSeData; /* 0x18 */
    HANDLE m_hHandle;        /* 0x1c */
    CEntity* m_pOwner;       /* 0x20 */
    CSEMotion* m_pParent;    /* 0x24 */

public:
    CSEMotionCtrl(CSEMotion* inParent);
    ~CSEMotionCtrl();
    BOOL initalize(CEntity* inOwner);
    void set(const SE_DATA* inData);
    void set(const SE_DATA* inData, INT inSeID);
    void update(TIMEQ inCurrentTime);
    HANDLE getHandle() const { return m_hHandle; }
    const SE_DATA* getSeData() const { return m_pSeData; }
    INT getFileID() const { return m_iFileID; }
    INT getSeID() const { return m_iSeID; }
    void stop();

private:
    BOOL setupFootSE_();
    void playPanAndVol_();
    void setPanAndVol_();
    void fadeoutSE_(INT inTime);
};

class CSEMotion : public CAttachMotion {
private:
    INT m_iID;                  /* 0x08 */
    SES_HEADER* m_pHeader;      /* 0x0c */
    INT m_iSEMotCtrlCount;      /* 0x10 */
    INT m_iNowCtrlCount;        /* 0x14 */
    CSEMotionCtrl* m_pSEMotCtrl; /* 0x18 */
    INT m_iRandomDataNum;       /* 0x1c */
    RANDOM_DATA* m_pRandomData; /* 0x20 */
    HANDLE m_hVoice;            /* 0x24 */
    INT m_iFileID;              /* 0x28 */
    INT m_iVoiceID;             /* 0x2c */
    INT m_iPriority;            /* 0x30 */
    HANDLE m_hBeforeVoice;      /* 0x34 */
    INT m_iBeforeFileID;        /* 0x38 */
    INT m_iBeforeVoiceID;       /* 0x3c */
    INT m_iBeforePriority;      /* 0x40 */
    const SE_DATA* m_pBeforeSeData; /* 0x44 */
    TIMEQ m_tqVoiceTimer;       /* 0x48 */

public:
    CSEMotion(CEntity* inOwner);
    virtual ~CSEMotion();
    BOOL initalize(INT inID);
    virtual BOOL set(INT inIndex) { return set_(inIndex, 0, TRUE); }
    BOOL set(INT inIndex, INT inNo, BOOL inStop) { return set_(inIndex, inNo, inStop); }
    virtual void stop();
    virtual void update();
    void playFootStopSE();
    void playVoice(INT inFileID, INT inID, INT inPriority);

private:
    BOOL initialize_(SES_HEADER* inHeader);
    BOOL set_(INT inIndex, INT inNo, BOOL inStop);
    INT randomSelect_(const SE_DATA* inData);
    BOOL isDamagetMotion_(INT inIndex) const;
    BOOL isPlayVoice_();
};

#endif /* GAME_SEMOTION_H */
