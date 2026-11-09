
#include "System/Task.h"
#include "System/Graphic/Anim.h"
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in LevelUp/WorldSelect); which header
 * it comes from is not known. phaseSaveGameData's "" shares it. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

/* System/TaskMan.h's container overflow message: here it follows the empty string. */
#include "System/TaskMan.h"



#include "System/Ctd/CtdMan.h"
#include "Form/Menu/SaveLoadUtil.h"
#include "System/HeapMan.h"
#include "System/MainApp.h"
#include "System/Debug.h"
#include "System/TaskMan.h"
#include "Game/Game.h"
#include "Game/GameWork.h"
#include "Game/SceneInfo.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/Player/Player.h"
#include <string.h>
#include "include_asm.h"


extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int atoi(const char* s);

inline void _safeFree(void* pv) {
    if (pv) com_free(pv);
}

struct GameDataAttr {
    sceMcStDateTime _LastTime;
    USHORT _nBrokenSize;
} __attribute__((aligned(8)));

enum _SIZE_CHECK_TYPE {
    _CHECK_TYPE_SYSDAT = 0,
    _CHECK_TYPE_GAMEDAT = 1
};

/* SysDataCOM: Game/GameWork.h */










/* STATUS_INFO (Game/Status.h would add its strings to .rodata) */
struct SaveStatusInfo {
    INT HP;
    INT HPMax;
    INT CP;
    INT CPMax;
    INT AP;
    INT DPMax;
    INT EXP;
    INT LV;
    INT MP;
    INT playTime;
};

CHAR* s_pszBrowserForm = "%s\x81^%3d\x81""F%02d\x81@%s%s";
CHAR* s_pszGameLevelMark = "\x81\x99\x81@\x81\x9a\x81\x9d";
CHAR* s_pszGameClearMark = "\x81@\x81\xf4";

static ULONGLONG s_nLatestDataTime;
static UINT s_nLatestDataIdx;
static UINT s_nLatestDataSlot;

CMemocaAgent::CMemocaAgent(CTask* pParent) : CTask(pParent, 0x8000), m_nCurrentSlot(0) {
    m_nRequest = _REQ_NOP;
    m_nLastResult = 0;
    ignorePause(TRUE);
    ignorePlaySpeed(TRUE);
    m_nPhase = 0;
    m_nStep = 0;
    m_nFlag = 0;
    m_pDirEntBuf = NULL;
    m_pEntries = NULL;
}

CMemocaAgent::~CMemocaAgent() {
    _safeFree(m_pDirEntBuf);
    m_pDirEntBuf = NULL;
}

void CMemocaAgent::main() {
    switch (m_nPhase) {
    case _REQ_CHECK_FREE_MEM:
        phaseCheckFreeMem();
        break;
    case _REQ_GET_ENTRIES:
        phaseGetEntries();
        break;
    case _REQ_SAVE_GAME_DATA:
        phaseSaveGameData();
        break;
    case _REQ_LOAD_GAME_DATA:
        phaseLoadGameData();
        break;
    case _REQ_IDLE_CHECK_CARD:
        phaseIdleCheckCard();
        break;
    case _REQ_LOAD_SYS_DATA:
        phaseLoadSysData();
        break;
    case _REQ_SAVE_SYS_DATA:
        phaseSaveSysData();
        break;
    }
}

BOOL CMemocaAgent::reqCheckFreeMem() {
    if (!isCompleteReq(NULL)) {
        return FALSE;
    }
    m_nPhase = m_nRequest = _REQ_CHECK_FREE_MEM;
    m_nStep = 0;
    m_nTmpSlot = 0;
    m_nTmpResult[1] = m_nTmpResult[0] = -1;
    _safeFree(m_pDirEntBuf);
#line 147
    m_pDirEntBuf = (sceMcTblGetDir*)com_memalign_d(64, sizeof(sceMcTblGetDir) * 1024, __FILE__ "(147)", (UINT)-1);
    if (!m_pDirEntBuf) AssertApp(__FILE__, __LINE__, "m_pDirEntBuf", 0);
    memset(m_pDirEntBuf, 0, sizeof(sceMcTblGetDir) * 1024);
    return TRUE;
}

static BOOL _checkDatFileExist(const sceMcTblGetDir* pDirInfo, UINT nInfoCnt) {
    CHAR* pszSysName = CMemoca::getSysFileNameCOM();
    CHAR* pszGameName = CMemoca::getGameDataFileName();
    INT nLen = strlen(pszGameName);
    for (INT i = 0; i < nInfoCnt; i++, pDirInfo++) {
        CHAR* pszDirName = (CHAR*)pDirInfo->EntryName;
        if (strcmp(pszDirName, pszSysName) == 0 || strncmp(pszDirName, pszGameName, nLen) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

static BOOL _checkFreeSizeByType(_SIZE_CHECK_TYPE type, UINT nFreeSize) {
    BOOL bResult = FALSE;
    switch (type) {
    case _CHECK_TYPE_SYSDAT:
        bResult = nFreeSize >= 0x5D;
        break;
    case _CHECK_TYPE_GAMEDAT:
        bResult = nFreeSize >= 0x77;
        break;
    }
    return bResult;
}

void CMemocaAgent::phaseCheckFreeMem() {
    INT r;
    switch (m_nStep) {
    case 0:
    case 4:
        if (CMemoca::reqGetFreeSize(m_nTmpSlot, m_pDirEntBuf, 1024)) {
            m_nStep++;
        }
        return;
    case 1:
    case 5: {
        r = CMemoca::getFreeSize();
        if (r == -9999) {
            return;
        }
        if (r == -2) {
            m_nTmpSlot++;
            m_nStep += 3;
            return;
        }
        INT nSize = r < 0 ? 0 : r;
        m_nTmpResult[m_nTmpSlot] = nSize;
        if (nSize >= 200) {
            r = nSize;
            break;
        }
        if (r == -3) {
            r = 0x3E7C;
            break;
        }
        if (!_checkDatFileExist(m_pDirEntBuf, 1024)) {
            m_nTmpSlot++;
            m_nStep += 3;
            return;
        }
        m_nDataCount = CMemoca::getRootDirCount();
        m_nFlag &= ~0x300;
        m_nStep++;
        m_nTargetIndex = 0;
        return;
    }
    case 2:
    case 6:
        if ((m_nFlag & 0x300) != 0x300) {
            while (m_nTargetIndex < m_nDataCount) {
                CHAR* pszTarget = (CHAR*)m_pDirEntBuf[m_nTargetIndex].EntryName;
                if (strcmp(pszTarget, CMemoca::getSysFileNameCOM()) == 0) {
                    if (!(m_nFlag & 0x100)) {
                        m_nFlag |= 0x100;
                    }
                } else if (strncmp(pszTarget, CMemoca::getGameDataFileName(), strlen(CMemoca::getGameDataFileName())) == 0) {
                    if (!(m_nFlag & 0x200)) {
                        m_nFlag |= 0x200;
                    }
                }
                m_nTargetIndex++;
                if ((m_nFlag & 0x300) == 0x300) {
                    break;
                }
            }
        }
        if ((m_nFlag & 0x300) == 0x300) {
            r = m_nTmpResult[m_nTmpSlot] + 0xBC;
            break;
        }
        if (m_nFlag & 0x300) {
            if (m_nFlag & 0x100) {
                if (_checkFreeSizeByType(_CHECK_TYPE_GAMEDAT, m_nTmpResult[m_nTmpSlot])) {
                    r = m_nTmpResult[m_nTmpSlot] + 0x51;
                    break;
                }
            } else {
                if (_checkFreeSizeByType(_CHECK_TYPE_SYSDAT, m_nTmpResult[m_nTmpSlot])) {
                    r = m_nTmpResult[m_nTmpSlot] + 0x6B;
                    break;
                }
            }
        }
        m_nTmpSlot++;
        m_nStep += 2;
        return;
    case 3:
    case 7: {
        r = CMemoca::checkDataFileAttr(0, (CHAR*)m_pDirEntBuf[m_nTargetIndex].EntryName, m_nTmpSlot);
        if (r == -9999) {
            return;
        }
        CHAR* pszTarget = (CHAR*)m_pDirEntBuf[m_nTargetIndex].EntryName;
        if (r == 0) {
            if (strcmp(pszTarget, CMemoca::getSysFileNameCOM()) == 0) {
                m_nFlag |= 0x100;
            } else if (strncmp(pszTarget, CMemoca::getGameDataFileName(), strlen(CMemoca::getGameDataFileName())) == 0) {
                m_nFlag |= 0x200;
            }
        } else if (r != -2) {
            if (strcmp(pszTarget, CMemoca::getSysFileNameCOM()) == 0) {
                INT nDirSize = CMemoca::getDirSizeAfterCheckDir() / 512;
                if (m_nTmpResult[m_nTmpSlot] >= 0x51 - nDirSize) {
                    m_nTmpResult[m_nTmpSlot] -= 0x51 - nDirSize;
                    m_nFlag |= 0x100;
                }
            }
        }
        m_nTargetIndex++;
        m_nStep--;
        return;
    }
    case 8:
        if (m_nTmpResult[0] < 0) {
            r = m_nTmpResult[1] < 0 ? -2 : -6;
        } else {
            r = m_nTmpResult[1] < 0 ? -5 : -4;
        }
        break;
    }
    m_nLastResult = r;
    endPhase();
    _safeFree(m_pDirEntBuf);
    m_pDirEntBuf = NULL;
}

BOOL CMemocaAgent::reqGetEntries(CMemocaDataEntries& entries, BOOL bForSave) {
    if (!isCompleteReq(NULL)) {
        return FALSE;
    }
    m_pEntries = &entries;
    entries.clearAll();
    m_nPhase = m_nRequest = _REQ_GET_ENTRIES;
    m_nStep = 0;
    m_nDataCount = 0;
    _safeFree(m_pDirEntBuf);
    m_pDirEntBuf = NULL;
    if (bForSave) {
        m_nFlag |= 1;
    } else {
        m_nFlag &= ~1;
    }
    return TRUE;
}

void CMemocaAgent::phaseGetEntries() {
    INT r;
    switch (m_nStep) {
    case 0:
        if (!m_pDirEntBuf) {
#line 438
            m_pDirEntBuf = (sceMcTblGetDir*)com_memalign_d(64, sizeof(sceMcTblGetDir) * 1024, __FILE__ "(438)", (UINT)-1);
            if (!m_pDirEntBuf) AssertApp(__FILE__, __LINE__, "m_pDirEntBuf", 0);
        }
        memset(m_pDirEntBuf, 0, sizeof(sceMcTblGetDir) * 1024);
        if (CMemoca::reqGetFreeSize(m_nCurrentSlot, m_pDirEntBuf, 1024)) {
            m_nStep = 1;
        }
        break;
    case 1:
        r = CMemoca::getFreeSize();
        if (r == -9999) {
            return;
        }
        m_nLastResult = r;
        if (r < 0) {
            m_nStep = 9;
            if (r == -3) {
                m_pEntries->m_nFlag |= 2;
            }
            return;
        }
        if (m_nFlag & 1) {
            m_nTmpResult[0] = r;
            m_nFlag &= ~0x300;
            if (r < 200 && !_checkDatFileExist(m_pDirEntBuf, 1024)) {
                m_nLastResult = -4;
                m_nStep = 9;
                break;
            }
        }
        m_pEntries->m_nDataUnitCount[0] = r < 0 ? 0 : r;
        TRACE_USER(TGOTO, "Memoca Savable Size = %d (FileEntry)\n", r);
        m_pEntries->m_nFreeEntSpace = CMemoca::getFreeEntSpace();
        TRACE_USER(TGOTO, "Memoca Free DirEnt Size = %d (FileEntry)\n", m_pEntries->m_nFreeEntSpace);
        m_nDataCount = CMemoca::getRootDirCount();
        if (m_nDataCount == 0) {
            m_nLastResult = 0;
            m_nStep = 9;
            break;
        }
        m_nTargetIndex = 0;
        m_nStep = 2;
        m_bufTmp.releaseBuf(0);
        m_bufTmp.allocBuf_A64(16);
        return;
    case 8:
        m_nTargetIndex++;
        if (m_nDataCount == m_nTargetIndex) {
            INT n = m_nTmpResult[0];
            if (n < 200) {
                BOOL bOK = (m_nFlag & 0x300) == 0x300;
                UINT nFreeSize = n;
                if (!bOK) {
                    if (m_nFlag & 0x300) {
                        if ((m_nFlag & 0x100) || m_pEntries->m_nSysDataBrokenSize) {
                            if (m_pEntries->m_nSysDataBrokenSize) {
                                UINT nBrokenSize = ((m_pEntries->m_nSysDataBrokenSize + 1) >> 1) << 1;
                                if ((UINT)n < 0x50 - nBrokenSize) {
                                    nFreeSize = 0;
                                } else {
                                    nFreeSize = n - (0x50 - nBrokenSize);
                                }
                            }
                            bOK = _checkFreeSizeByType(_CHECK_TYPE_GAMEDAT, nFreeSize);
                        } else {
                            bOK = _checkFreeSizeByType(_CHECK_TYPE_SYSDAT, nFreeSize);
                        }
                    }
                    if (!bOK) {
                        m_nLastResult = -4;
                        m_nStep = 9;
                        break;
                    }
                }
            }
            m_nLastResult = 0;
            m_nStep = 9;
            break;
        }
        m_nStep = 2;
    case 2: {
        sceMcTblGetDir* pDir = &m_pDirEntBuf[m_nTargetIndex];
        CHAR* pszDirName = (CHAR*)pDir->EntryName;
        CHAR* pszGameName = CMemoca::getGameDataFileName();
        INT nLen = strlen(pszGameName);
        m_nDataID = 0;
        if (strcmp(pszDirName, CMemoca::getSysFileNameCOM()) == 0) {
            m_nStep = 3;
        } else {
            if (strncmp(pszDirName, pszGameName, nLen) == 0) {
                m_nDataID = atoi(pszDirName + nLen);
            }
            if (m_nDataID >= 1 && m_nDataID <= 99) {
                m_nStep = 3;
            } else if (pszDirName[0] == '.') {
                m_nStep = 8;
            } else {
                m_nStep = 6;
            }
        }
        break;
    }
    case 3:
        r = CMemoca::checkDataFileAttr(0, (CHAR*)m_pDirEntBuf[m_nTargetIndex].EntryName, m_nCurrentSlot);
        if (r == -9999) {
            return;
        }
        if (r == 0) {
            if (strcmp((CHAR*)m_pDirEntBuf[m_nTargetIndex].EntryName, CMemoca::getSysFileNameCOM()) == 0) {
                m_nStep = 8;
                m_nFlag |= 0x100;
            } else {
                m_pEntries->m_DataAttrTbl[m_nDataID - 1]._LastTime = CMemoca::getLastCheckFileTime();
                m_nStep = 4;
                m_nFlag |= 0x200;
            }
            break;
        }
        if (r == -2) {
            m_nLastResult = -1;
            m_nStep = 9;
            break;
        }
        if (strcmp((CHAR*)m_pDirEntBuf[m_nTargetIndex].EntryName, CMemoca::getSysFileNameCOM()) == 0) {
            m_pEntries->m_nSysDataBrokenSize = CMemoca::getDirSizeAfterCheckDir() / 512;
        } else if (strncmp((CHAR*)m_pDirEntBuf[m_nTargetIndex].EntryName, CMemoca::getGameDataFileName(),
                           strlen(CMemoca::getGameDataFileName())) == 0) {
            if (r == -15) {
                m_pEntries->m_DataTbl[m_nDataID - 1]._cFlag |= 0x20;
            } else {
                m_pEntries->m_DataAttrTbl[m_nDataID - 1]._nBrokenSize = CMemoca::getDirSizeAfterCheckDir() / 512;
                m_pEntries->m_DataTbl[m_nDataID - 1]._cFlag |= 0x10;
                m_pEntries->m_nSavableBrokenDataCount++;
            }
        }
        m_nStep = 8;
        break;
    case 4:
        if (CMemoca::reqLoad(m_nCurrentSlot, (CHAR*)m_pDirEntBuf[m_nTargetIndex].EntryName, m_bufTmp,
                             MC_DATATYPE_GAME_HEAD)) {
            m_nStep = 5;
        }
        break;
    case 5:
        r = CMemoca::loadProc();
        if (r == -9999) {
            return;
        }
        if (r >= 0) {
            m_pEntries->m_DataTbl[m_nDataID - 1] = *(GameDataHeader*)m_bufTmp.getBufPtr();
            m_pEntries->m_nDataUnitCount[1]++;
        } else if (r == -2) {
            m_nLastResult = -1;
            m_nStep = 9;
            break;
        } else {
            m_pEntries->m_DataAttrTbl[m_nDataID - 1]._nBrokenSize = CMemoca::getDirSizeAfterCheckDir() / 512;
            m_pEntries->m_DataTbl[m_nDataID - 1]._cFlag |= 0x10;
            m_pEntries->m_nSavableBrokenDataCount++;
        }
        m_nStep = 8;
        break;
    case 6:
        if (CMemoca::reqGetDirSize(m_nCurrentSlot, (CHAR*)m_pDirEntBuf[m_nTargetIndex].EntryName)) {
            m_nStep = 7;
        }
        break;
    case 7:
        r = CMemoca::getDirSize();
        if (r == -9999) {
            return;
        }
        if (r < 0 && r != -15) {
            m_nLastResult = -1;
            m_nStep = 9;
        } else {
            m_pEntries->m_nDataUnitCount[2] += r / 512;
            m_nStep = 8;
        }
        break;
    case 9:
        if (m_pEntries->isUnformated()) {
            m_pEntries->m_nDataUnitCount[0] = 10700;
        } else if (m_nLastResult != -4) {
            INT n = m_pEntries->m_nDataUnitCount[2];
            m_pEntries->m_nDataUnitCount[2] = (n + 0x6A) / 0x6B;
            n = m_pEntries->m_nDataUnitCount[0];
            if (!(m_nFlag & 0x100)) {
                if (m_pEntries->m_nSysDataBrokenSize == 0) {
                    if (m_pEntries->m_nFreeEntSpace == 0) {
                        if (n > 0) {
                            n -= 2;
                            m_pEntries->m_nFreeEntSpace = 2;
                        }
                    }
                    m_pEntries->m_nFreeEntSpace--;
                }
                n -= 0x50;
                n += ((m_pEntries->m_nSysDataBrokenSize + 1) >> 1) << 1;
                if (n < 0) {
                    n = 0;
                }
            }
            m_pEntries->m_nDataUnitCount[0] = n;
            BOOL bOn = FALSE;
            if (m_pEntries->m_nFreeEntSpace) {
                if (n < 0x6A) {
                    goto on;
                }
            } else if (n < 0x6B) {
            on:
                bOn = TRUE;
            }
            m_pEntries->setNoSpace(bOn);
        }
        m_bufTmp.releaseBuf(0);
        _safeFree(m_pDirEntBuf);
        m_pDirEntBuf = NULL;
        endPhase();
        break;
    }
}

BOOL CMemocaAgent::reqSaveGameData(USHORT nUnitIdx, CMemocaDataEntries& entries, BOOL bGameClear) {
    if (!isCompleteReq(NULL) || nUnitIdx >= 99) {
        return FALSE;
    }
    if (bGameClear) {
        m_nFlag |= 2;
    } else {
        m_nFlag &= ~2;
    }
    m_nTargetIndex = nUnitIdx;
    m_pEntries = &entries;
    m_nPhase = m_nRequest = _REQ_SAVE_GAME_DATA;
    m_nStep = 0;
    return TRUE;
}

static UINT _getGameDataSizes(UINT* pOutSizeTbl) {
    UINT nSize = 0;
    pOutSizeTbl[0] = sizeof(GameDataHeader);
    pOutSizeTbl[1] = ((INT)CGameWork::getIntWorkUnit(0)->getWorkSize() + 15) & ~15;
    pOutSizeTbl[2] = ((INT)CGameWork::getByteWorkUnit(0)->getWorkSize() + 15) & ~15;
    pOutSizeTbl[3] = ((INT)CGameWork::getFlagWorkUnit(0)->getWorkSize() + 15) & ~15;
    pOutSizeTbl[4] = (sizeof(STORAGE::MCWork) + 15) & ~15;
    pOutSizeTbl[5] = (sizeof(STORAGE::MCWork2) + 15) & ~15;
    for (INT i = 0; i < 6; i++) {
        nSize += pOutSizeTbl[i];
    }
    return nSize;
}

void CMemocaAgent::setupSaveData(CBuf& buf) {
    GameDataHeader* pHeader = m_pEntries->getEntry(m_nTargetIndex);
#line 862
    if (!pHeader) AssertApp(__FILE__, __LINE__, "pHeader", 0);
    BOOL bGameClearNow = (m_nFlag >> 1) & 1;
    BOOL bGameCleared = CGameWork::getFlag(0, 0x83);
    if (bGameClearNow) {
        CGameWork::backup(CGameWork::BACKUP_SECONDARY);
        CGameWork::rollback(CGameWork::BACKUP_PRIMARY);
        if (CGame::isRikuGame() && !CSceneInfo::getCurrentAreaScene()) {
            CGameWork::rollback(CGameWork::BACKUP_SECONDARY);
            CGameWork::backup(CGameWork::BACKUP_SECONDARY);
        }
    } else {
        CSceneInfo::backupEventDoorState();
    }
    BOOL bRiku = CGameWork::getByte(0, 0x60);
    SaveStatusInfo& pl_info = *(SaveStatusInfo*)CGameWork::progMCW()._statusInfo;
    pHeader->_cLevel = pl_info.LV;
    pHeader->_cFloor = CSceneInfo::getCurrentFloorNumber();
    INT w = CSceneInfo::getCurrentWorldID(FALSE);
    INT c;
    if (CSceneInfo::getCurrentRoomID()) {
        c = w;
    } else {
        c = w | 0x10;
    }
    pHeader->_cWorld = c;
    if (bGameClearNow && bRiku) {
        pHeader->_cWorld = 0x1D;
    }
    pHeader->_nMoogle = pl_info.MP;
    BYTE cGameLevel = CGameWork::getByte(0, 0x61);
    if (cGameLevel == 0) {
        cGameLevel = 1;
    } else if (cGameLevel == 1) {
        cGameLevel = 0;
    }
    pHeader->_cGameLevel = cGameLevel;
    if (bGameClearNow) {
        static const float _vPos[2][4] = {
            {8.8f, 0.0035f, -1.5f, 3.14159274f},
            {-4.5f, 0.0035f, -9.5f, 3.14159274f},
        };
        static const unsigned char _nRoom[2] = {20, 0};
        CGameWork::setInt(0, 10, 1);
        INT i = bRiku != 0;
        const INT* pPos = (const INT*)_vPos[i];
        CGameWork::setInt(0, 12, pPos[0]);
        CGameWork::setInt(0, 13, pPos[1]);
        CGameWork::setInt(0, 14, pPos[2]);
        CGameWork::setInt(0, 15, pPos[3]);
        CGameWork::progMCW()._nRoomID = _nRoom[i];
        RoomInfo* pRoom = CSceneInfo::getRoomInfo(_nRoom[i]);
#line 920
        if (!pRoom) AssertApp(__FILE__, __LINE__, "pRoom", 0);
        CGameWork::progMCW()._nScene = pRoom->getSceneNum();
    } else if (g_pPlayer) {
        FVECTOR vPos;
        BOOL bOK = FALSE;
        CGameWork::setInt(0, 10, 1);
        CGimmickManager* pMan = (CGimmickManager*)CTaskMan::getSysTask(0x11);
        if (pMan) {
            bOK = pMan->getSavePointPos(vPos);
        }
        if (!bOK) {
            TRACE("\x83Z\x81[\x83u\x83|\x83""C\x83\x93\x83g\x82\xcc\x8e\xe6\x93\xbe\x82\xc9\x8e\xb8\x94s\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
            vPos._q = g_pPlayer->getPos()._q;
        }
        vPos.w = 3.14159274f;
        CGameWork::setInt(0, 12, vPos._ui[0]);
        CGameWork::setInt(0, 13, vPos._ui[1]);
        CGameWork::setInt(0, 14, vPos._ui[2]);
        CGameWork::setInt(0, 15, vPos._ui[3]);
    }
    pHeader->_cFlag = bRiku ? 2 : 1;
    if (bGameClearNow || bGameCleared) {
        pHeader->_cFlag |= 4;
    }
    INT nTimeSec = CMainApp::getPlayTime();
    if (nTimeSec > 3599940) {
        nTimeSec = 3599940;
    }
    pHeader->_nTime = nTimeSec;
    CGameWork::preSave(bGameClearNow || bGameCleared);
    UINT nSizeTbl[6];
    UINT nSize = _getGameDataSizes(nSizeTbl);
    TRACE_USER_COL(TGOTO, _TTY_RED, "Memoca Save Size = 0x%X bytes,  SystemData Size = %X\n", nSize, 0x400);
    buf.allocBuf_A64(nSize);
    UINT* pSize = nSizeTbl;
    BYTE* p = buf.getBufPtr();
    *(GameDataHeader*)p = *pHeader;
    p += *pSize++;
    memcpy(p, CGameWork::getIntWorkUnit(0)->getWorkTop(), CGameWork::getIntWorkUnit(0)->getWorkSize());
    p += *pSize++;
    memcpy(p, CGameWork::getByteWorkUnit(0)->getWorkTop(), CGameWork::getByteWorkUnit(0)->getWorkSize());
    p += *pSize++;
    memcpy(p, CGameWork::getFlagWorkUnit(0)->getWorkTop(), CGameWork::getFlagWorkUnit(0)->getWorkSize());
    p += *pSize++;
    memcpy(p, &CGameWork::progMCW(), sizeof(STORAGE::MCWork));
    p += *pSize++;
    memcpy(p, &CGameWork::progMCW2(), sizeof(STORAGE::MCWork2));
    if (bGameClearNow) {
        CGameWork::rollback(CGameWork::BACKUP_SECONDARY);
    }
}

void CMemocaAgent::phaseSaveGameData() {
    INT r;
    switch (m_nStep) {
    case 0:
        m_bufTmp.releaseBuf(0);
        setupSaveData(m_bufTmp);
        m_nStep++;
    case 1: {
        char szSaveFile[32];
        sprintf(szSaveFile, "%s%02d", CMemoca::getGameDataFileName(), m_nTargetIndex + 1);
        GameDataHeader* pHeader = m_pEntries->getEntry(m_nTargetIndex);
        UINT nHour = pHeader->_nTime / 3600;
        UINT nMinute;
        if (nHour >= 1000) {
            nHour = 999;
            nMinute = 59;
        } else {
            nMinute = (pHeader->_nTime - nHour * 3600) / 60;
        }
        INT nPlayerCtd = (pHeader->_cFlag & 1) ? 0x100001 : 0x100002;
        char szMark[2][4];
        strncpy(szMark[0], &s_pszGameLevelMark[pHeader->_cGameLevel < 4 ? pHeader->_cGameLevel * 2 : 0], 2);
        szMark[0][2] = 0;
        strncpy(szMark[1], pHeader->isGameCleared() ? s_pszGameClearMark + 2 : s_pszGameClearMark, 2);
        szMark[1][2] = 0;
        CHAR* pszTitle = m_szTitle;
        CHAR* pszName = CCtdMan::getText(nPlayerCtd);
        sprintf(pszTitle, s_pszBrowserForm, pszName ? pszName : "", nHour, nMinute, szMark[0], szMark[1]);
        if (CMemoca::reqSave(m_nCurrentSlot, szSaveFile, m_bufTmp, MC_DATATYPE_GAME, m_szTitle, 0)) {
            m_nStep++;
        }
        break;
    }
    case 2:
        r = CMemoca::saveProc();
        if (r == -9999) {
            break;
        }
        if (r < 0) {
            TRACE_COL(_TTY_RED, "\x83\x81\x83\x82\x83J\x83Z\x81[\x83u\x82\xc9\x8e\xb8\x94s\x82\xb5\x82\xdc\x82\xb5\x82\xbd\x81i #%d \x81j\n", r);
        }
        CGameWork::setInt(0, 10, 0);
        m_bufTmp.releaseBuf(0);
        m_nLastResult = r;
        endPhase();
        break;
    }
}

BOOL CMemocaAgent::reqLoadGameData(USHORT nUnitIdx) {
    if (isCompleteReq(NULL) && nUnitIdx < 99) {
        m_nTargetIndex = nUnitIdx;
        m_nPhase = m_nRequest = _REQ_LOAD_GAME_DATA;
        m_nStep = 0;
        return TRUE;
    }
    return FALSE;
}

void CMemocaAgent::setupLoadData(const CBuf& buf) {
    const GameDataHeader* pHeader = (const GameDataHeader*)buf.getBufPtr();
    CMainApp::resetPlayTime(pHeader->_nTime);
    BOOL bRiku;
    if (pHeader->_cFlag & 2) {
        bRiku = FALSE;
    } else {
        bRiku = TRUE;
    }
    WORLD_ID w = (WORLD_ID)(pHeader->_cWorld & 0xF);
    CSceneInfo::initFloor(CSceneInfo::getFloorIndex(pHeader->_cFloor, bRiku), w);
    UINT nSizeTbl[6];
    _getGameDataSizes(nSizeTbl);
    UINT* pSize = nSizeTbl;
    const BYTE* p = buf.getBufPtr() + *pSize++;
    memcpy(CGameWork::getIntWorkUnit(0)->getWorkTop(), p, CGameWork::getIntWorkUnit(0)->getWorkSize());
    p += *pSize++;
    memcpy(CGameWork::getByteWorkUnit(0)->getWorkTop(), p, CGameWork::getByteWorkUnit(0)->getWorkSize());
    p += *pSize++;
    memcpy(CGameWork::getFlagWorkUnit(0)->getWorkTop(), p, CGameWork::getFlagWorkUnit(0)->getWorkSize());
    p += *pSize++;
    memcpy(&CGameWork::progMCW(), p, sizeof(STORAGE::MCWork));
    p += *pSize++;
    memcpy(&CGameWork::progMCW2(), p, sizeof(STORAGE::MCWork2));
    CGameWork::postLoad();
}

void CMemocaAgent::phaseLoadGameData() {
    INT r;
    char szLoadFile[32];
    switch (m_nStep) {
    case 0: {
        UINT nSizeTbl[6];
        m_bufTmp.releaseBuf(0);
        m_bufTmp.allocBuf_A64(_getGameDataSizes(nSizeTbl));
        m_nStep++;
    }
    case 1:
        sprintf(szLoadFile, "%s%02d", CMemoca::getGameDataFileName(), m_nTargetIndex + 1);
        if (CMemoca::reqLoad(m_nCurrentSlot, szLoadFile, m_bufTmp, MC_DATATYPE_GAME)) {
            m_nStep++;
        }
        break;
    case 2:
        r = CMemoca::loadProc();
        if (r == -9999) {
            break;
        }
        if (r < 0) {
            TRACE_COL(_TTY_RED, "\x83\x81\x83\x82\x83J\x83\x8d\x81[\x83h\x82\xc9\x8e\xb8\x94s\x82\xb5\x82\xdc\x82\xb5\x82\xbd\x81i #%d \x81j\n", r);
        } else {
            setupLoadData(m_bufTmp);
        }
        m_bufTmp.releaseBuf(0);
        m_nLastResult = r;
        endPhase();
        break;
    }
}

BOOL CMemocaAgent::reqIdleCheckCard() {
    if (!isCompleteReq(NULL)) {
        return FALSE;
    }
    m_nStep = 0;
    m_nPhase = m_nRequest = _REQ_IDLE_CHECK_CARD;
    return TRUE;
}

void CMemocaAgent::phaseIdleCheckCard() {
    INT r = CMemoca::idleCheckCard(m_nCurrentSlot);
    if (r != -9999) {
        m_nLastResult = r;
        endPhase();
    }
}

BOOL CMemocaAgent::reqLoadSysData() {
    if (!isCompleteReq(NULL)) {
        return FALSE;
    }
    m_nPhase = m_nRequest = _REQ_LOAD_SYS_DATA;
    m_nStep = 0;
    m_nTmpSlot = 0;
    return TRUE;
}

void CMemocaAgent::phaseLoadSysData() {
    INT r;
    SysDataCOM* pData;
    switch (m_nStep) {
    case 0:
        m_bufTmp.releaseBuf(0);
        m_bufTmp.allocBuf_A64(sizeof(SysDataCOM));
        m_nStep++;
    case 1:
    case 4:
        r = CMemoca::checkDataFileAttr(0, CMemoca::getSysFileNameCOM(), m_nTmpSlot);
        if (r == -9999) {
            return;
        }
        if (r == 0) {
            sceMcStDateTime t = CMemoca::getLastCheckFileTime();
            m_tmpTime = *(ULONGLONG*)&t;
            m_nStep++;
        } else {
            m_nStep += 3;
            m_nTmpSlot++;
        }
        break;
    case 2:
    case 5:
        if (CMemoca::reqLoad(m_nTmpSlot, CMemoca::getSysFileNameCOM(), m_bufTmp, MC_DATATYPE_SYS)) {
            m_nStep++;
        }
        return;
    case 3:
    case 6:
        r = CMemoca::loadProc();
        if (r == -9999) {
            return;
        }
        pData = (SysDataCOM*)m_bufTmp.getBufPtr();
        if (r >= 0) {
            if (pData->Header.Id != 0x4D4F43 || *(u_int*)&pData->Header.Area != 0x1504A) {
                r = -1;
            }
            if (r >= 0) {
                CGameWork::setSysDataCOM(pData, &m_tmpTime);
            }
        }
        m_nTmpSlot++;
        m_nStep++;
        break;
    case 7:
        m_bufTmp.releaseBuf(0);
        m_nLastResult = 0;
        endPhase();
        break;
    }
}

BOOL CMemocaAgent::reqSaveSysData() {
    if (!isCompleteReq(NULL)) {
        return FALSE;
    }
    m_nStep = 0;
    m_nPhase = m_nRequest = _REQ_SAVE_SYS_DATA;
    return TRUE;
}

void CMemocaAgent::phaseSaveSysData() {
    INT r;
    switch (m_nStep) {
    case 0: {
        m_bufTmp.releaseBuf(0);
        SysDataCOM* p = (SysDataCOM*)m_bufTmp.allocBuf_A64(sizeof(SysDataCOM));
#line 1291
        if (!p) AssertApp(__FILE__, __LINE__, "p", 0);
        memset(p, 0, sizeof(SysDataCOM));
        p->Header.Version = 1;
        p->Header.Area = 0x504A;
        p->Header.Id = 0x4D4F43;
        CGameWork::getSysDataCOM(*p);
        m_nStep++;
    }
    case 1:
        if (CMemoca::reqSave(m_nCurrentSlot, CMemoca::getSysFileNameCOM(), m_bufTmp, MC_DATATYPE_SYS, NULL, 0)) {
            m_nStep++;
        }
        break;
    case 2:
        r = CMemoca::saveProc();
        if (r == -9999) {
            break;
        }
        if (r >= 0) {
            TRACE_COL(_TTY_RED, "\x83\x81\x83\x82\x83J(#%d)\x82\xc9\x81""A\x83V\x83X\x83""e\x83\x80\x83""f\x81[\x83^\x82\xf0\x83Z\x81[\x83u\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n", m_nCurrentSlot + 1);
        } else {
            TRACE_COL(_TTY_RED, "\x83\x81\x83\x82\x83J(#%d)\x82\xd6\x82\xcc\x83V\x83X\x83""e\x83\x80\x83""f\x81[\x83^\x82\xcc\x83Z\x81[\x83u\x82\xc9\x8e\xb8\x94s\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n", m_nCurrentSlot + 1);
        }
        m_bufTmp.releaseBuf(0);
        m_nLastResult = r;
        endPhase();
        break;
    }
}

void CMemocaAgent::endPhase() {
    m_nPhase = 0;
    m_nRequest = _REQ_NOP;
}

BOOL CMemocaAgent::isCompleteReq(INT* pOutResult) const {
    BOOL bComplete = m_nPhase == 0 && m_nRequest == _REQ_NOP;
    if (bComplete && pOutResult) {
        *pOutResult = m_nLastResult;
    }
    return bComplete;
}

CMemocaDataEntries::CMemocaDataEntries(INT nEntryCount) {
    m_nDataEntryCount = nEntryCount;
    m_pSysDataCOM = NULL;
#line 1461
    if (!(0 < nEntryCount)) AssertApp(__FILE__, __LINE__, "0 < nEntryCount", 0);
    m_DataTbl = new (__FILE__ "(1462)") GameDataHeader[nEntryCount];
    m_DataAttrTbl = new (__FILE__ "(1463)") GameDataAttr[nEntryCount];
    clearAll();
}

CMemocaDataEntries::~CMemocaDataEntries() {
    _safeFree(m_DataTbl);
    _safeFree(m_DataAttrTbl);
    _safeFree(m_pSysDataCOM);
}

void CMemocaDataEntries::initialize() {
    s_nLatestDataTime = 0;
    s_nLatestDataIdx = 0;
    s_nLatestDataSlot = 0;
}

void CMemocaDataEntries::clearAll() {
    m_nFlag = 0;
    if (m_nDataEntryCount) {
        if (m_DataTbl) {
            memset(m_DataTbl, 0, sizeof(GameDataHeader) * m_nDataEntryCount);
        }
        if (m_DataAttrTbl) {
            memset(m_DataAttrTbl, 0, sizeof(GameDataAttr) * m_nDataEntryCount);
        }
    }
    _safeFree(m_pSysDataCOM);
    m_pSysDataCOM = NULL;
    for (INT i = 2; i >= 0; i--) {
        m_nDataUnitCount[i] = 0;
    }
    m_nSavableBrokenDataCount = 0;
    m_nSysDataBrokenSize = 0;
    m_nFreeEntSpace = 0;
}

GameDataHeader* CMemocaDataEntries::getEntry(UINT nIndex) {
    GameDataHeader* pData = NULL;
    if (m_DataTbl && nIndex < m_nDataEntryCount) {
        pData = &m_DataTbl[nIndex];
    }
    if (pData && !pData->hasData() && !pData->isDataBroken() && !pData->isDirBroken()) {
        if (m_nFlag & 1) {
            pData = NULL;
        } else if (m_nFreeEntSpace ? m_nDataUnitCount[0] < 0x6A : m_nDataUnitCount[0] < 0x6B) {
            pData = NULL;
        }
    }
    return pData;
}

sceMcStDateTime* CMemocaDataEntries::getEntryTimeInfo(UINT nIndex) const {
    if (m_DataTbl && nIndex < m_nDataEntryCount) {
        return &m_DataAttrTbl[nIndex]._LastTime;
    }
    return NULL;
}

USHORT CMemocaDataEntries::getDataUnitCountByType(UINT nType) {
    if (nType >= 3) {
        return 0;
    }
    if (nType == 0) {
        INT n = 0;
        INT nFreeEnt = m_nFreeEntSpace;
        INT nFree = m_nDataUnitCount[0];
        for (; nFree >= 0x6A; n++) {
            if (!nFreeEnt) {
                nFreeEnt = 2;
                nFree -= 2;
            }
            nFree -= 0x6A;
            nFreeEnt--;
            if (nFree < 0) {
                break;
            }
        }
        return n;
    }
    return m_nDataUnitCount[nType];
}

UINT CMemocaDataEntries::getLatestEntryIndex(UINT nSlot) const {
    if (s_nLatestDataTime && nSlot == s_nLatestDataSlot) {
        sceMcStDateTime* pTime = getEntryTimeInfo(s_nLatestDataIdx);
        if (pTime && *(ULONGLONG*)pTime == s_nLatestDataTime) {
            return s_nLatestDataIdx;
        }
        s_nLatestDataTime = 0;
        s_nLatestDataIdx = 0;
        s_nLatestDataSlot = 0;
    }
    ULONGLONG nTime = 0;
    UINT nIdx = 0;
    GameDataHeader* pData = m_DataTbl;
    GameDataAttr* pAttr = m_DataAttrTbl;
    for (USHORT i = 0; i < m_nDataEntryCount; i++, pData++, pAttr++) {
        if (pData && pData->hasData() && nTime < *(ULONGLONG*)&pAttr->_LastTime) {
            nTime = *(ULONGLONG*)&pAttr->_LastTime;
            nIdx = i;
        }
    }
    return nIdx;
}

void CMemocaDataEntries::setLatestEntryIndex(UINT nSlot, UINT nIdx) {
    sceMcStDateTime* pTime = getEntryTimeInfo(nIdx);
    if (pTime) {
        s_nLatestDataTime = *(ULONGLONG*)pTime;
        s_nLatestDataIdx = nIdx;
        s_nLatestDataSlot = nSlot & 1;
    }
}

UINT CMemocaDataEntries::getLatestSlot() {
    return s_nLatestDataSlot & 1;
}

void CMemocaDataEntries::useOneSavableUnit(UINT nIndex) {
    BOOL bError = FALSE;
    if (m_DataAttrTbl[nIndex]._nBrokenSize) {
        USHORT nNeedSize = 0x6A - ((m_DataAttrTbl[nIndex]._nBrokenSize + 1) >> 1 << 1);
        TRACE_USER(TGOTO, "Broken Data Size = %d,  Need Size = %d (FileEntry)\n", m_DataAttrTbl[nIndex]._nBrokenSize,
                   nNeedSize);
        if (m_nDataUnitCount[0] >= nNeedSize) {
            m_nDataUnitCount[0] -= nNeedSize;
            m_nDataUnitCount[1]++;
            m_DataAttrTbl[nIndex]._nBrokenSize = 0;
        } else {
            bError = TRUE;
        }
    } else {
        if (m_nFreeEntSpace) {
            if (m_nDataUnitCount[0] < 0x6A) {
                goto error;
            }
        } else {
            if (m_nDataUnitCount[0] < 0x6B) {
            error:
                bError = TRUE;
                goto done;
            }
            m_nFreeEntSpace += 2;
            m_nDataUnitCount[0] -= 2;
        }
        m_nDataUnitCount[0] -= 0x6A;
        m_nDataUnitCount[1]++;
        m_nFreeEntSpace--;
    done:;
    }
    if (bError) {
        TRACE_COL(_TTY_RED, "\x81\x9f\x81\x9f \x83\x81\x83\x82\x83J\x8b\xf3\x82\xab\x97""e\x97\xca\x82\xaa\x96\xb3\x82\xa2\x82\xcc\x82\xc9\x83Q\x81[\x83\x80\x83""f\x81[\x83^\x82\xf0\x8f\x91\x82\xb1\x82\xa4\x82\xc6\x82\xb5\x82\xc4\x82\xa2\x82\xdc\x82\xb7\x81I\x81I \x81\x9f\x81\x9f\n");
    }
    TRACE_USER(TGOTO, "Memoca Savable Size = %d (FileEntry)\n", m_nDataUnitCount[0]);
    TRACE_USER(TGOTO, "Memoca Free DirEnt Size = %d (FileEntry)\n", m_nFreeEntSpace);
    if (m_nFreeEntSpace ? m_nDataUnitCount[0] < 0x6A : m_nDataUnitCount[0] < 0x6B) {
        m_nFlag |= 1;
    }
}

// SOURCE DATA: .data .rodata .bss
