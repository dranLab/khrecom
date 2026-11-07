
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in GameOver). */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}
inline void _emptyStringPlaceholder2() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}

#include "System/HeapMan.h"
#include "Game/Game.h"
#include "Game/GameWork.h"

using STORAGE::MCWork;
using STORAGE::MCWork2;

#include "Game/Player/Player.h"

#include "Form/Menu/JMemoFlag.h"




inline void* operator new(size_t, void* p) throw() { return p; }



MCWork* CGameWork::m_pMCWork = NULL;
MCWork2* CGameWork::m_pMCWork2 = NULL;
MCWork* CGameWork::m_pMCWorkBackup = NULL;
MCWork* CGameWork::m_pMCWorkBackup2nd = NULL;

static unsigned int s_SizeTbl[6] = { 1024, 128, 128, 1024, 384, 128 };
static unsigned int s_RollbackSizeTbl[6] = { 768, 96, 96, 896, 320, 112 };
static int s_PoohClearIntroTbl[6] = { 10, 11, 12, 13, 14, 15 };
static int s_PoohClearTbl[6] = { 3, 4, 5, 6, 7, 8 };

static SysDataCOM::DATA s_SysDataCOM;
static ULONGLONG s_timeSysDataCOM;
CWorkUnit* CGameWork::m_ppWorkTbl[2][3];
CWorkUnit* CGameWork::m_ppWorkBackupTbl[2][3];
CWorkUnit* CGameWork::m_ppWorkBackupTbl2nd[2][3];

void CGameWork::coldReset() {
    memset(&s_SysDataCOM, 0, sizeof(s_SysDataCOM));
    s_timeSysDataCOM = 0;
}

void CGameWork::initialize() {
    UINT* pSize = s_SizeTbl;
    INT i;
    for (i = 0; i < 2; i++) {
        m_ppWorkTbl[i][0] = new (__FILE__ "(64)") CIntWork(*pSize++);
        m_ppWorkTbl[i][1] = new (__FILE__ "(65)") CByteWork(*pSize++);
        m_ppWorkTbl[i][2] = new (__FILE__ "(66)") CBitWork(*pSize++);
    }
    pSize = s_SizeTbl;
    for (i = 0; i < 2; i++) {
        m_ppWorkBackupTbl[i][0] = new (__FILE__ "(71)") CIntWork(*pSize++);
        m_ppWorkBackupTbl[i][1] = new (__FILE__ "(72)") CByteWork(*pSize++);
        m_ppWorkBackupTbl[i][2] = new (__FILE__ "(73)") CBitWork(*pSize++);
        m_ppWorkBackupTbl2nd[i][0] = NULL;
        m_ppWorkBackupTbl2nd[i][1] = NULL;
        m_ppWorkBackupTbl2nd[i][2] = NULL;
    }
    m_pMCWork = new (__FILE__ "(78)") MCWork;
    m_pMCWork2 = new (__FILE__ "(79)") MCWork2;
    m_pMCWorkBackup = new (__FILE__ "(80)") MCWork;
    m_pMCWorkBackup2nd = NULL;
    resetAll();
}

void CGameWork::finalize() {
    INT i;
    INT j;
    CWorkUnit* p;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            p = m_ppWorkTbl[i][j];
            if (p) delete p;
            p = m_ppWorkBackupTbl[i][j];
            if (p) delete p;
            p = m_ppWorkBackupTbl2nd[i][j];
            if (p) delete p;
        }
    }
    if (m_pMCWork) delete m_pMCWork;
    if (m_pMCWork2) delete m_pMCWork2;
    if (m_pMCWorkBackup) delete m_pMCWorkBackup;
    if (m_pMCWorkBackup2nd) delete m_pMCWorkBackup2nd;
}

void CGameWork::resetAll() {
    for (INT i = 0; i < 2; i++) {
        for (INT j = 0; j < 3; j++) {
            CWorkUnit* p;
            p = m_ppWorkTbl[i][j];
            if (p) p->resetAll();
            p = m_ppWorkBackupTbl[i][j];
            if (p) p->resetAll();
        }
    }
    m_pMCWork->initialize();
    m_pMCWork2->initialize();
    m_pMCWorkBackup->initialize();
}

void CGameWork::resetAll_GameComplete() {
    resetAll();
}

void CGameWork::setupGameClearInfo() {
    if (s_SysDataCOM.SoraClear) {
        INT i;
        setFlag(0, 9, TRUE);
        setFlag(0, 17, TRUE);
        if (CGame::isRikuGame()) {
            for (i = 0; i < 16; i++) {
                m_pMCWork2->_nWorldOnFloor[0][i] = s_SysDataCOM.WorldTblSora[i];
            }
            for (i = 0; i < 6; i++) {
                setFlag(0, s_PoohClearIntroTbl[i], s_SysDataCOM.PoohClearIntro[i] != 0);
                setByte(0, s_PoohClearTbl[i], s_SysDataCOM.PoohClear[i]);
            }
            setInt(0, 35, s_SysDataCOM.PoohIntro);
            setByte(0, 12, s_SysDataCOM.LastPoohEvent);
            if (s_SysDataCOM.JiminyMemoComplete) setFlag(0, 1000, TRUE);
        }
    }
    if (s_SysDataCOM.RikuClear) {
        INT i;
        setFlag(0, 18, TRUE);
        if (CGame::isSoraGame()) {
            for (i = 0; i < 16; i++) {
                m_pMCWork2->_nWorldOnFloor[1][i] = s_SysDataCOM.WorldTblRiku[i];
            }
            if (s_SysDataCOM.DReportComplete) setFlag(0, 1001, TRUE);
        }
    }
}

void CGameWork::preSave(BOOL bGameClear) {
    BOOL bOutSysSora;
    BOOL bOutSysRiku;
    INT i;
    if (!bGameClear) {
        if (CGame::isSoraGame()) {
            s_SysDataCOM.LatestGameType = 1;
            bOutSysSora = FALSE;
            bOutSysRiku = FALSE;
        } else {
            s_SysDataCOM.LatestGameType = 2;
            bOutSysSora = TRUE;
            bOutSysRiku = FALSE;
        }
    } else {
        s_SysDataCOM.LatestGameType = CGame::isSoraGame() ? 2 : 1;
        setFlag(0, 9, TRUE);
        bOutSysSora = TRUE;
        setFlag(0, 17, TRUE);
        setFlag(0, 131, TRUE);
        if (CGame::isSoraGame()) {
            if (CJiminyMemoFlag::getContentOpenPercentage(0) == 100) setFlag(0, 1000, TRUE);
            bOutSysRiku = getFlag(0, 18);
        } else {
            setFlag(0, 18, TRUE);
            if (CJiminyMemoFlag::getContentOpenPercentage(1) == 100) setFlag(0, 1001, TRUE);
            bOutSysRiku = TRUE;
        }
    }
    if (bOutSysSora) {
        s_SysDataCOM.SoraClear = 1;
        for (i = 0; i < 6; i++) {
            if (!s_SysDataCOM.PoohClearIntro[i] && getFlag(0, s_PoohClearIntroTbl[i])) {
                s_SysDataCOM.PoohClearIntro[i] = 1;
            }
            BYTE n = getByte(0, s_PoohClearTbl[i]);
            if (s_SysDataCOM.PoohClear[i] < n) s_SysDataCOM.PoohClear[i] = n;
        }
        s_SysDataCOM.PoohIntro |= getInt(0, 35);
        s_SysDataCOM.LastPoohEvent = getByte(0, 12);
        if (getFlag(0, 1000)) s_SysDataCOM.JiminyMemoComplete = 1;
        for (i = 0; i < 16; i++) {
            s_SysDataCOM.WorldTblSora[i] = m_pMCWork2->_nWorldOnFloor[0][i];
        }
    }
    if (bOutSysRiku) {
        INT i;
        s_SysDataCOM.RikuClear = 1;
        if (getFlag(0, 1001)) s_SysDataCOM.DReportComplete = 1;
        for (i = 0; i < 16; i++) {
            s_SysDataCOM.WorldTblRiku[i] = m_pMCWork2->_nWorldOnFloor[1][i];
        }
    }
    m_pMCWork->preSave();
    m_pMCWork2->preSave();
}

void CGameWork::postLoad() {
    setupGameClearInfo();
    m_pMCWork->postLoad();
    m_pMCWork2->postLoad();
}

void CGameWork::backup(BACKUP b) {
    MCWork* pDst;
    if (b == BACKUP_SECONDARY) {
        UINT* pSize = s_SizeTbl;
        for (INT i = 0; i < 2; i++) {
            if (!m_ppWorkBackupTbl2nd[i][0]) m_ppWorkBackupTbl2nd[i][0] = new (__FILE__ "(336)") CIntWork(*pSize++);
            if (!m_ppWorkBackupTbl2nd[i][1]) m_ppWorkBackupTbl2nd[i][1] = new (__FILE__ "(338)") CByteWork(*pSize++);
            if (!m_ppWorkBackupTbl2nd[i][2]) m_ppWorkBackupTbl2nd[i][2] = new (__FILE__ "(340)") CBitWork(*pSize++);
        }
        if (!m_pMCWorkBackup2nd) m_pMCWorkBackup2nd = new (__FILE__ "(343)") MCWork;
    }
    CPlayer::exportStatus();
    for (INT i = 0; i < 2; i++) {
        for (INT j = 0; j < 3; j++) {
            CWorkUnit* pDst = (b == BACKUP_SECONDARY) ? m_ppWorkBackupTbl2nd[i][j] : m_ppWorkBackupTbl[i][j];
            pDst->copyFrom(*m_ppWorkTbl[i][j], 0);
        }
    }
    pDst = (b == BACKUP_SECONDARY) ? m_pMCWorkBackup2nd : m_pMCWorkBackup;
    *pDst = *m_pMCWork;
}

void CGameWork::rollback(BACKUP b) {
    UINT* pSize = s_RollbackSizeTbl;
    for (INT i = 0; i < 2; i++) {
        for (INT j = 0; j < 3; j++) {
            CWorkUnit* pDst = m_ppWorkTbl[i][j];
            CWorkUnit* pSrc = (b == BACKUP_SECONDARY) ? m_ppWorkBackupTbl2nd[i][j] : m_ppWorkBackupTbl[i][j];
            pDst->copyFrom(*pSrc, *pSize++);
        }
    }
    *m_pMCWork = (b == BACKUP_SECONDARY) ? *m_pMCWorkBackup2nd : *m_pMCWorkBackup;
    if (b == BACKUP_SECONDARY) {
        for (INT i = 0; i < 2; i++) {
            if (m_ppWorkBackupTbl2nd[i][0]) delete m_ppWorkBackupTbl2nd[i][0];
            m_ppWorkBackupTbl2nd[i][0] = NULL;
            if (m_ppWorkBackupTbl2nd[i][1]) delete m_ppWorkBackupTbl2nd[i][1];
            m_ppWorkBackupTbl2nd[i][1] = NULL;
            if (m_ppWorkBackupTbl2nd[i][2]) delete m_ppWorkBackupTbl2nd[i][2];
            m_ppWorkBackupTbl2nd[i][2] = NULL;
        }
        if (m_pMCWorkBackup2nd) delete m_pMCWorkBackup2nd;
        m_pMCWorkBackup2nd = NULL;
    }
}

INT CGameWork::getInt(UINT nLv, UINT nIdx) {
    if (nLv < 2) {
        CIntWork* p = (CIntWork*)getIntWorkUnit(nLv);
        if (p) return p->getWork(nIdx);
    }
    return 0;
}

BYTE CGameWork::getByte(UINT nLv, UINT nIdx) {
    if (nLv < 2) {
        CByteWork* p = (CByteWork*)getByteWorkUnit(nLv);
        if (p) return p->getByte(nIdx);
    }
    return 0;
}

BOOL CGameWork::getFlag(UINT nLv, UINT nIdx) {
    if (nLv < 2) {
        CBitWork* p = (CBitWork*)getFlagWorkUnit(nLv);
        if (p) return p->getFlag(nIdx);
    }
    return 0;
}

void CGameWork::setInt(UINT nLv, UINT nIdx, INT nValue) {
    if (nLv < 2) {
        CIntWork* p = (CIntWork*)getIntWorkUnit(nLv);
        if (p) p->setWork(nIdx, nValue);
    }
}

void CGameWork::setByte(UINT nLv, UINT nIdx, BYTE nValue) {
    if (nLv < 2) {
        CByteWork* p = (CByteWork*)getByteWorkUnit(nLv);
        if (p) p->setByte(nIdx, nValue);
    }
}

void CGameWork::setFlag(UINT nLv, UINT nIdx, BOOL bSet) {
    if (nLv < 2) {
        CBitWork* p = (CBitWork*)getFlagWorkUnit(nLv);
        if (p) p->setFlag(nIdx, bSet);
    }
}

void CGameWork::getSysDataCOM(SysDataCOM& dst) {
    dst.Data = s_SysDataCOM;
}

void CGameWork::setSysDataCOM(const SysDataCOM* pSrc, const ULONGLONG* pTime) {
    if (!pSrc) return;
    const SysDataCOM::DATA& src = pSrc->Data;
    BOOL bSoraClearTrg = FALSE;
    BOOL bRikuClearTrg;
    BOOL bUpdateGameType;
    INT i;
    if (src.SoraClear) {
        if (!s_SysDataCOM.SoraClear) bSoraClearTrg = TRUE;
        s_SysDataCOM.SoraClear = 1;
        for (i = 0; i < 6; i++) {
            if (src.PoohClearIntro[i] > 0) s_SysDataCOM.PoohClearIntro[i] = src.PoohClearIntro[i];
            if (s_SysDataCOM.PoohClear[i] < src.PoohClear[i]) s_SysDataCOM.PoohClear[i] = src.PoohClear[i];
        }
        s_SysDataCOM.PoohIntro |= src.PoohIntro;
    }
    bRikuClearTrg = FALSE;
    if (src.RikuClear) {
        if (!s_SysDataCOM.RikuClear) bRikuClearTrg = TRUE;
        s_SysDataCOM.RikuClear = 1;
    }
    if (src.JiminyMemoComplete) s_SysDataCOM.JiminyMemoComplete = 1;
    if (src.DReportComplete) s_SysDataCOM.DReportComplete = 1;
    bUpdateGameType = (pTime == NULL);
    if (pTime && s_timeSysDataCOM < *pTime) {
        s_timeSysDataCOM = *pTime;
        bUpdateGameType = TRUE;
        s_SysDataCOM.LatestGameType = pSrc->Data.LatestGameType;
    }
    if (src.SoraClear && (bUpdateGameType || bSoraClearTrg)) {
        char WorldTblSora[16];
        for (INT i = 0; i < 16; i++) {
            s_SysDataCOM.WorldTblSora[i] = src.WorldTblSora[i];
        }
        s_SysDataCOM.LastPoohEvent = src.LastPoohEvent;
    }
    if (src.RikuClear && (bUpdateGameType || bRikuClearTrg)) {
        char WorldTblRiku[16];
        for (INT i = 0; i < 16; i++) {
            s_SysDataCOM.WorldTblRiku[i] = src.WorldTblRiku[i];
        }
    }
}

void CGameWork::getPoohSysFlag(SysDataCOM& s) {
    for (INT i = 0; i < 6; i++) {
        s.Data.PoohClearIntro[i] = getFlag(0, s_PoohClearIntroTbl[i]);
        s.Data.PoohClear[i] = getByte(0, s_PoohClearTbl[i]);
    }
    s.Data.PoohIntro = getInt(0, 35);
    s.Data.LastPoohEvent = getByte(0, 12);
}

CWorkUnit::CWorkUnit() : m_pWork(NULL), m_nWorkSize(0), m_bWorkManaged(FALSE), m_nRange(0) {}

CWorkUnit::~CWorkUnit() {
    dispose();
}

void CWorkUnit::setup(UINT nSize) {
    dispose();
    UINT nIntSize = nSize / sizeof(INT);
    if (nSize != nIntSize * sizeof(INT)) nIntSize++;
    nSize = nIntSize << 2;
    manageWork((INT*)com_malloc_d(nSize, __FILE__ "(627)", (UINT)-1), nSize, TRUE);
}

void CWorkUnit::dispose() {
    if (getWorkTop() && isManaged()) {
        com_free(getWorkTop());
    }
    m_pWork = NULL;
    m_nWorkSize = 0;
    m_bWorkManaged = FALSE;
    setRange(0);
}

void CWorkUnit::manageWork(INT* pWork, UINT nSize, BOOL bManage) {
    m_bWorkManaged = bManage;
    m_pWork = pWork;
    m_nWorkSize = nSize;
    setRange(pWork ? size2range(nSize) : 0);
}

void CWorkUnit::resetAll() {
    INT nIntSize = (INT)range2size(getRange()) >> 2;
    INT* pW = getTopPtr();
    for (INT i = 0; i < nIntSize; i++) {
        *pW++ = 0;
    }
}

void CWorkUnit::attach(INT* pWork, UINT nSize) {
    dispose();
    manageWork(pWork, nSize, FALSE);
}

void CWorkUnit::copyFrom(const CWorkUnit& inSrc, INT nByteSize) {
    if (nByteSize <= 0) nByteSize = range2size(inSrc.getRange());
    INT nIntSize = nByteSize >> 2;
    INT* pW = getTopPtr();
    const INT* pSrc = inSrc.getTopPtr();
    for (INT i = 0; i < nIntSize; i++) {
        *pW++ = *pSrc++;
    }
}

UINT CIntWork::size2range(UINT nSize) const {
    UINT nRange = nSize / sizeof(INT);
    if (nSize != nRange * sizeof(INT)) nRange++;
    return nRange;
}

UINT CIntWork::range2size(UINT nRange) const {
    return nRange * sizeof(INT);
}

INT CIntWork::getWork(UINT nIndex) const {
    return (getTopPtr() && isValidIndex(nIndex)) ? getTopPtr()[nIndex] : 0;
}

void CIntWork::setWork(UINT nIndex, INT nData) {
    if (getTopPtr() && isValidIndex(nIndex)) getTopPtr()[nIndex] = nData;
}

UINT CBitWork::size2range(UINT nSize) const {
    UINT nRange = nSize / sizeof(INT);
    if (nSize != nRange * sizeof(INT)) nRange++;
    return nRange * 32;
}

UINT CBitWork::range2size(UINT nRange) const {
    UINT nSize = nRange / 8;
    if (nRange != nSize * 8) nSize++;
    return nSize;
}

BOOL CBitWork::getFlag(UINT nIndex) const {
    if (isValidIndex(nIndex)) {
        UINT nOutIndex;
        INT nOutBit;
        getWorkOffset(nIndex, nOutIndex, nOutBit);
        return getTopPtr() ? ((getTopPtr()[nOutIndex] & nOutBit) != 0) : FALSE;
    }
    return FALSE;
}

static inline void setBitFlag(int& flag, INT nBit, BOOL bOn) {
    if (bOn) flag |= nBit;
    else flag &= ~nBit;
}
void CBitWork::setFlag(UINT nIndex, BOOL bSet) {
    if (isValidIndex(nIndex)) {
        UINT nOutIndex;
        INT nOutBit;
        getWorkOffset(nIndex, nOutIndex, nOutBit);
        if (getTopPtr()) setBitFlag(getTopPtr()[nOutIndex], nOutBit, bSet);
    }
}

BYTE CByteWork::getByte(UINT nIndex) const {
    return (getTopPtr() && isValidIndex(nIndex)) ? ((const BYTE*)getTopPtr())[nIndex] : 0;
}

void CByteWork::setByte(UINT nIndex, BYTE nData) {
    if (getTopPtr() && isValidIndex(nIndex)) ((BYTE*)getTopPtr())[nIndex] = nData;
}

// SOURCE DATA: .data .rodata .bss
