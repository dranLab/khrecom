// PARTIAL: added for Form/Menu/MoogleShop
#ifndef GAME_GAMEWORK_H
#define GAME_GAMEWORK_H

#include "khtypes.h"
#include "Game/StorageWork.h"

// PARTIAL: added for Form/Menu/SaveLoadUtil
struct SysDataCOM;
class CWorkUnit {
private:
    INT* m_pWork;
    UINT m_nWorkSize;
    BOOL m_bWorkManaged;
    UINT m_nRange;

public:
    virtual ~CWorkUnit();
    INT* getWorkTop() { return m_pWork; }
    UINT getWorkSize() { return m_nWorkSize; }
};

class CGameWork {
private:
    static CWorkUnit* m_ppWorkTbl[2][3]; // PARTIAL: added for Form/Menu/SaveLoadUtil
    static STORAGE::MCWork* m_pMCWork;
    static STORAGE::MCWork2* m_pMCWork2; // PARTIAL: added for Form/Menu/SaveLoadUtil

public:
    // PARTIAL: added for Form/Menu/SaveLoadUtil
    enum BACKUP {
        BACKUP_PRIMARY = 0,
        BACKUP_SECONDARY = 1
    };
    static void preSave(BOOL bGameClear);
    static void postLoad();
    static void backup(BACKUP nBackup);
    static void rollback(BACKUP nBackup);
    static STORAGE::MCWork2& progMCW2() { return *m_pMCWork2; }
    static CWorkUnit* getIntWorkUnit(UINT inKind) { return m_ppWorkTbl[inKind][0]; }
    static CWorkUnit* getByteWorkUnit(UINT inKind) { return m_ppWorkTbl[inKind][1]; }
    static CWorkUnit* getFlagWorkUnit(UINT inKind) { return m_ppWorkTbl[inKind][2]; }
    static void getSysDataCOM(SysDataCOM& outData);
    static void setSysDataCOM(const SysDataCOM* pData, const ULONGLONG* pTime);
    static STORAGE::MCWork& progMCW() { return *m_pMCWork; }
    static void setFlag(UINT inKind, UINT inFlag, BOOL inOn); // PARTIAL: added for Game/Gimmick/GimmickSavePoint
    static INT getInt(UINT inKind, UINT inIndex); // PARTIAL: added for Game/Gimmick/GimmickWarpPoint
    static void setInt(UINT inKind, UINT inIndex, INT inValue); // PARTIAL: added for Form/Menu/GameOver
    static BOOL getFlag(UINT inKind, UINT inFlag); // PARTIAL: added for Form/Menu/Menu
    static BYTE getByte(UINT inKind, UINT inIndex); // PARTIAL: added for Game/Player/PlayerControl
    static void setByte(UINT inKind, UINT inIndex, BYTE inValue); // PARTIAL: added for Form/Menu/WorldSelect
};

#endif /* GAME_GAMEWORK_H */
