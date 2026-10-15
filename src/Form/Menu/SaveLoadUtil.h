#ifndef FORM_MENU_SAVELOADUTIL_H
#define FORM_MENU_SAVELOADUTIL_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Buf.h"
#include "System/Memoca.h"

struct GameDataAttr;
struct SysDataCOM;

struct GameDataHeader {
    BYTE _cFlag;
    BYTE _cLevel;
    CHAR _cFloor;
    BYTE _cWorld;
    INT _nMoogle;
    INT _nTime;
    BYTE _cGameLevel;
    unsigned char __nFillers[3];

    BOOL hasData() { return (_cFlag & 0x3) != 0; }
    BOOL isDataBroken() { return (_cFlag >> 4) & 1; }
    BOOL isDirBroken() { return (_cFlag >> 5) & 1; }
    BOOL isGameCleared() { return (_cFlag >> 2) & 1; }
};

class CMemocaDataEntries {
private:
    GameDataHeader* m_DataTbl;
    GameDataAttr* m_DataAttrTbl;
    USHORT m_nDataEntryCount;
    USHORT m_nFlag;
    USHORT m_nDataUnitCount[3];
    USHORT m_nSavableBrokenDataCount;
    USHORT m_nSysDataBrokenSize;
    USHORT m_nFreeEntSpace;
    SysDataCOM* m_pSysDataCOM;

    friend class CMemocaAgent;

public:
    CMemocaDataEntries(INT nEntryCount);
    ~CMemocaDataEntries();
    static void initialize();
    void clearAll();
    GameDataHeader* getEntry(UINT nIndex);
    sceMcStDateTime* getEntryTimeInfo(UINT nIndex) const;
    UINT getDataEntryCount() { return m_nDataEntryCount; }
    USHORT getDataUnitCountByType(UINT nType);
    USHORT getSavableBrokenDataCount() { return m_nSavableBrokenDataCount; }
    void useOneSavableUnit(UINT nIndex);
    UINT getLatestEntryIndex(UINT nSlot) const;
    void setLatestEntryIndex(UINT nSlot, UINT nIdx);
    static UINT getLatestSlot();
    BOOL isUnformated() { return (m_nFlag >> 1) & 1; }
    void formated() { m_nFlag &= ~2; }
    void setNoSpace(BOOL bOn) {
        if (bOn) {
            m_nFlag |= 1;
        } else {
            m_nFlag &= ~1;
        }
    }
};

enum _REQUEST {
    _REQ_NOP = 0,
    _REQ_CHECK_FREE_MEM = 1,
    _REQ_GET_ENTRIES = 2,
    _REQ_SAVE_GAME_DATA = 3,
    _REQ_LOAD_GAME_DATA = 4,
    _REQ_IDLE_CHECK_CARD = 5,
    _REQ_LOAD_SYS_DATA = 6,
    _REQ_SAVE_SYS_DATA = 7
};

class CMemocaAgent : public CTask {
private:
    ULONGLONG m_tmpTime;
    USHORT m_nCurrentSlot;
    USHORT m_nTmpSlot;
    CBuf m_bufTmp;
    char m_szTitle[80];
    _REQUEST m_nRequest;
    INT m_nLastResult;
    short m_nTmpResult[2];
    SHORT m_nPhase;
    SHORT m_nStep;
    USHORT m_nDataCount;
    USHORT m_nFlag;
    USHORT m_nTargetIndex;
    USHORT m_nDataID;
    sceMcTblGetDir* m_pDirEntBuf;
    CMemocaDataEntries* m_pEntries;

public:
    CMemocaAgent(CTask* pParent);
    virtual ~CMemocaAgent();
    virtual void main();
    void setCurrentSlot(INT nSlot) { m_nCurrentSlot = nSlot & 1; }
    BOOL reqCheckFreeMem();
    BOOL reqGetEntries(CMemocaDataEntries& entries, BOOL bForSave);
    BOOL reqSaveGameData(USHORT nUnitIdx, CMemocaDataEntries& entries, BOOL bGameClear);
    BOOL reqLoadGameData(USHORT nUnitIdx);
    BOOL reqIdleCheckCard();
    BOOL reqLoadSysData();
    BOOL reqSaveSysData();
    BOOL isCompleteReq(INT* pOutResult) const;
    INT getLastResult() const { return m_nLastResult; }
    UINT getLatestDataIndex() const { return m_pEntries ? m_pEntries->getLatestEntryIndex(m_nCurrentSlot) : 0; }

protected:
    void endPhase();
    void phaseCheckFreeMem();
    void phaseGetEntries();
    void phaseSaveGameData();
    void phaseLoadGameData();
    void phaseIdleCheckCard();
    void phaseLoadSysData();
    void phaseSaveSysData();
    void setupSaveData(CBuf& buf);
    void setupLoadData(const CBuf& buf);
};

#endif /* FORM_MENU_SAVELOADUTIL_H */
