#ifndef GAME_GAMEWORK_H
#define GAME_GAMEWORK_H

#include "khtypes.h"
#include "Game/StorageWork.h"

struct SysDataCOM { // 0x400
    struct HEADER { // 0x10
        /* 0x0 */ u_int Id;
        /* 0x4 */ u_short Area;
        /* 0x6 */ u_short Version;
        /* 0x8 */ char Filler[8];
    };

    struct DATA { // 0x40
        /* 0x00:0 */ u_int SoraClear : 1;
        /* 0x00:1 */ u_int RikuClear : 1;
        /* 0x00:2 */ u_int Pad : 30;
        /* 0x04 */ char PoohClearIntro[6];
        /* 0x0a */ char PoohClear[6];
        /* 0x10 */ char LatestGameType;
        /* 0x11 */ char LastPoohEvent;
        /* 0x12 */ char Pad2[2];
        /* 0x14:0 */ u_int JiminyMemoComplete : 1;
        /* 0x14:1 */ u_int DReportComplete : 1;
        /* 0x14:2 */ u_int Pad3 : 30;
        /* 0x18 */ u_int PoohIntro;
        /* 0x1c */ char Pad4[4];
        /* 0x20 */ char WorldTblSora[16];
        /* 0x30 */ char WorldTblRiku[16];
    };

private:
    static u_short ID_AREA_JP;
    static u_short ID_AREA_US;
    static u_short ID_AREA_EU;

public:
    static u_int DATA_ID;
    static u_short VERSION;
    /* 0x000 */ HEADER Header;
    /* 0x010 */ DATA Data;
    /* 0x050 */ char _Filler[944];
};

class CWorkUnit { // 0x14
private:
    /* 0x00 */ INT* m_pWork;
    /* 0x04 */ UINT m_nWorkSize;
    /* 0x08 */ BOOL m_bWorkManaged;
    /* 0x0c */ UINT m_nRange;

public:
    CWorkUnit();
    virtual ~CWorkUnit();
    virtual INT getWork(UINT nIndex) const = 0;
    virtual void setWork(UINT nIndex, INT nData) = 0;
    virtual UINT size2range(UINT nSize) const = 0;
    virtual UINT range2size(UINT nRange) const = 0;
    void setup(UINT nSize);
    void dispose();
    void resetAll();
    void copyFrom(const CWorkUnit& inSrc, INT nByteSize);
    void attach(INT* pWork, UINT nSize);
    UINT getRange() const { return m_nRange; }
    BOOL isValidIndex(UINT nIndex) const { return nIndex < m_nRange; }
    BOOL isManaged() const { return m_bWorkManaged; }
    INT* getWorkTop() { return m_pWork; }
    UINT getWorkSize() const { return m_nWorkSize; }

protected:
    INT* getTopPtr() { return m_pWork; }
    const INT* getTopPtr() const { return m_pWork; }
    void setRange(UINT nMax) { m_nRange = nMax; }
    void manageWork(INT* pWork, UINT nSize, BOOL bManage);
};

class CIntWork : public CWorkUnit { // 0x14
public:
    CIntWork(UINT nSize) {
        if (nSize) setup(nSize);
    }
    virtual UINT size2range(UINT nSize) const;
    virtual UINT range2size(UINT nRange) const;
    virtual INT getWork(UINT nIndex) const;
    virtual void setWork(UINT nIndex, INT nData);
};

class CBitWork : public CWorkUnit { // 0x14
public:
    CBitWork(UINT nSize) {
        if (nSize) setup(nSize);
    }
    virtual UINT size2range(UINT nSize) const;
    virtual UINT range2size(UINT nRange) const;
    virtual INT getWork(UINT nIndex) const { return getFlag(nIndex); }
    virtual void setWork(UINT nIndex, INT nData) { setFlag(nIndex, nData); }
    BOOL getFlag(UINT nIndex) const;
    void setFlag(UINT nIndex, BOOL bSet);

protected:
    void getWorkOffset(UINT nIndex, UINT& nOutIndex, INT& nOutBit) const {
        nOutIndex = nIndex >> 5;
        nOutBit = 1 << nIndex;
    }
};

class CByteWork : public CWorkUnit { // 0x14
public:
    CByteWork(UINT nSize) {
        if (nSize) setup(nSize);
    }
    virtual INT getWork(UINT nIndex) const { return getByte(nIndex); }
    virtual void setWork(UINT nIndex, INT nData) { setByte(nIndex, nData); }
    BYTE getByte(UINT nIndex) const;
    void setByte(UINT nIndex, BYTE nData);
    virtual UINT size2range(UINT nSize) const { return nSize; }
    virtual UINT range2size(UINT nRange) const { return nRange; }
};

class CGameWork {
    friend class CSceneInfo;       // MATCH: Game/SceneInfo uses the private members (friends are not in the debug info)
    friend struct _SceneInfoWork;  // MATCH: Game/SceneInfo's file-local functions (same)
    friend class CAtel; // MATCH: Game/Atel/Atel (_ref(m_pMCWork) keeps the original's reload after each store)
public:
    enum BACKUP {
        BACKUP_PRIMARY = 0,
        BACKUP_SECONDARY = 1
    };

private:
    static CWorkUnit* m_ppWorkTbl[2][3];
    static STORAGE::MCWork* m_pMCWork;
    static STORAGE::MCWork2* m_pMCWork2;
    static CWorkUnit* m_ppWorkBackupTbl[2][3];
    static STORAGE::MCWork* m_pMCWorkBackup;
    static CWorkUnit* m_ppWorkBackupTbl2nd[2][3];
    static STORAGE::MCWork* m_pMCWorkBackup2nd;

public:
    static void coldReset();
    static void initialize();
    static void finalize();
    static void resetAll();
    static void resetAll_GameComplete();
    static void preSave(BOOL bGameClear);
    static void postLoad();
    static void backup(BACKUP b);
    static void rollback(BACKUP b);
    static INT getInt(UINT nLv, UINT nIdx);
    static BYTE getByte(UINT nLv, UINT nIdx);
    static BOOL getFlag(UINT nLv, UINT nIdx);
    static void setInt(UINT nLv, UINT nIdx, INT nValue);
    static void setByte(UINT nLv, UINT nIdx, BYTE nValue);
    static void setFlag(UINT nLv, UINT nIdx, BOOL bSet);

private:
    static void setupGameClearInfo();

public:
    static STORAGE::MCWork& progMCW() { return *m_pMCWork; }
    static STORAGE::MCWork2& progMCW2() { return *m_pMCWork2; }
    static CWorkUnit* getIntWorkUnit(UINT nLv) { return m_ppWorkTbl[nLv][0]; }
    static CWorkUnit* getByteWorkUnit(UINT nLv) { return m_ppWorkTbl[nLv][1]; }
    static CWorkUnit* getFlagWorkUnit(UINT nLv) { return m_ppWorkTbl[nLv][2]; }
    static void getSysDataCOM(SysDataCOM& dst);
    static void setSysDataCOM(const SysDataCOM* pSrc, const ULONGLONG* pTime);
    static void getPoohSysFlag(SysDataCOM& s);
};

#endif /* GAME_GAMEWORK_H */
