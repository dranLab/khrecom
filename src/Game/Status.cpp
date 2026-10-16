#define TASK_H_NO_SLC_OVERFLOW_MSG
#include "include_asm.h"
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#define CARD_H_PATH "c:/KHCOM/MainApp/Game/Card.h"
#define TECHNIC_H_PATH "c:/KHCOM/MainApp/Game/Technic.h"
#define STATUS_H_PATH "c:/KHCOM/MainApp/Game/Status.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _statusEmptyString() {
    TRACE("");
}

#include "Game/Card.h"
#include "Game/Technic.h"
#include "Game/SceneInfo.h"
#include "Game/Status.h"
#include "Game/Game.h"
#include "Game/GameWork.h"

enum NAME_CASE {
    NAME_CASE_NONE = 0,
};

class CRsrcData;

class CRsrcDB {
public:
    static CRsrcData* getRsrc(INT nGroup, const char* pszName, NAME_CASE eCase);
    static void* getRsrcData(const CRsrcData& data);
};

class CPlayer {
public:
    static INT calcNextEXP(INT inNowLV);
};

/* the CardInit resource (original type name unknown) */
struct CARD_INIT_HEADER {
    UINT sig;      /* 0x0 '@CI' */
    UINT _04;
    UINT idxOfs;   /* 0x8 */
    UINT cardOfs;  /* 0xc */
};

static inline void _clampIdx(INT& ioIdx, INT inNo, INT inDefault) {
    if ((UINT)(inNo - 1) >= 13) ioIdx = inDefault;
}

CStatus* CStatus::m_pInstance = NULL;
STATUS_INFO* CStatus::m_pStatusInfo;

CStatus::CStatus() {
    m_pStatusInfo = (STATUS_INFO*)CGameWork::progMCW()._statusInfo;
}

CStatus::~CStatus() {}

/* initStatus: the source below matches except for one delay slot (bnez s0 in the world branch takes
 * "addiu v0,s0,-1" instead of the original's "addiu a2,s0,14"); it needs lines 43 and 46 for its asserts. */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Status", initStatus__7CStatusUs8WORLD_IDs);
#if 0
void CStatus::initStatus(USHORT inNo, WORLD_ID inDebugWorldID, SHORT inCurrentFloorNumber) {
    USHORT* pIdxTbl;
    unsigned char (*pInitCardTbl)[2];
    INT idx;
    INT idxCard;
    CRsrcData* pData = CRsrcDB::getRsrc(1, "CardInit", NAME_CASE_NONE);
    CARD_INIT_HEADER* pCardInitRsrc = pData ? (CARD_INIT_HEADER*)CRsrcDB::getRsrcData(*pData) : NULL;
    if (!pCardInitRsrc) AssertApp(__FILE__, __LINE__, "pCardInitRsrc", 0);
    CARD_INIT_HEADER* pCIHeader = pCardInitRsrc;

    if (!(*(UINT*)&pCIHeader->sig == 0x00494340)) AssertApp(__FILE__, __LINE__, "*(UINT*)&pCIHeader->sig == 0x00494340", 0);
    pIdxTbl = (USHORT*)((BYTE*)pCIHeader + pCIHeader->idxOfs);
    if (inNo == 0) {
        if (inCurrentFloorNumber == 0) {
            CTechnic::setAll();
        }
        if (CGame::isSoraGame()) {
            idx = inCurrentFloorNumber - 1;
            _clampIdx(idx, inCurrentFloorNumber, 13);
        } else if (inDebugWorldID == 0) {
            idx = 27;
        } else {
            idx = inDebugWorldID + 14;
            _clampIdx(idx, inDebugWorldID, 28);
        }
        idx += 1;
        idxCard = pIdxTbl[idx - 1];
        pInitCardTbl = (unsigned char (*)[2])((BYTE*)pCIHeader + pCIHeader->cardOfs) + idxCard;
        if (CGame::isSoraGame()) {
            setStatus(pInitCardTbl);
        }
        if (!CGame::isSoraGame()) {
            idx = inCurrentFloorNumber + 27;
            _clampIdx(idx, inCurrentFloorNumber + 13, 28);
            idx += 1;
            idxCard = pIdxTbl[idx - 1];
            setStatus((unsigned char (*)[2])((BYTE*)pCIHeader + pCIHeader->cardOfs) + idxCard);
        }
    } else {
        if (inNo == 1) {
            idx = 0;
        } else if (inNo == 2) {
            idx = 15;
        } else {
            return;
        }
        idx += 1;
        idxCard = pIdxTbl[idx - 1];
        pInitCardTbl = (unsigned char (*)[2])((BYTE*)pCIHeader + pCIHeader->cardOfs) + idxCard;
        if (inNo == 1) {
            setStatus(pInitCardTbl);
        }
        if (inNo == 2) {
            idxCard = pIdxTbl[15];
            setStatus((unsigned char (*)[2])((BYTE*)pCIHeader + pCIHeader->cardOfs) + idxCard);
        }
    }
}
#endif

/* Placeholder while initStatus is INCLUDE_ASM'd: its string literals, in their .rodata position. */
inline void _initStatusStrings(CARD_INIT_HEADER* pCardInitRsrc) {
    CRsrcDB::getRsrc(1, "CardInit", NAME_CASE_NONE);
    if (!pCardInitRsrc) AssertApp(__FILE__, __LINE__, "pCardInitRsrc", 0);
    if (!(*(UINT*)&pCardInitRsrc->sig == 0x00494340)) AssertApp(__FILE__, __LINE__, "*(UINT*)&pCIHeader->sig == 0x00494340", 0);
}

void CStatus::setStatus(unsigned char (*pInitCardTbl)[2]) {
    USHORT statusTbl[4];
    {
        INT i;
        for (i = 0; i < 4; i++) {
            statusTbl[i] = pInitCardTbl[i * 2][1] | (pInitCardTbl[i * 2 + 1][1] << 8);
        }
    }
    setLV(statusTbl[0]);
    setEXP(getLvExp(statusTbl[0] ? statusTbl[0] - 1 : 0));
    setHPMax(statusTbl[1]);
    setHP(statusTbl[1]);
    if (CGame::isSoraGame()) {
        setAP(10);
        setCPMax(statusTbl[2]);
        unsigned char techTbl[12] = { 4, 6, 8, 5, 7, 9, 39, 41, 10, 51, 11, 34 };
        INT i;
        if (statusTbl[3]) {
            for (i = 0; i < statusTbl[3] - 1; i++) {
                CTechnic::addTechnic(techTbl[i]);
            }
        }
    } else {
        setDPMax(statusTbl[2]);
        setAP(statusTbl[3]);
    }
}

BOOL CStatus::setMP(INT inMP) {
    if (inMP < 0) {
        _info()->MP = 0;
        return FALSE;
    }
    if (inMP > 99999) {
        _info()->MP = 99999;
        return FALSE;
    }
    _info()->MP = inMP;
    return TRUE;
}

BOOL CStatus::addMP(INT inMP) {
    return setMP(_info()->MP + inMP);
}

INT CStatus::getLvExp(UINT inLV) {
    if (inLV == 0) return 0;
    if (inLV >= 99) return -1;
    return CPlayer::calcNextEXP(inLV);
}

BOOL CStatus::isLevelUp() {
    INT exp;
    if (CGame::getDbgGameMode() == 7) return FALSE;
    if (CGame::getDbgGameMode() == 8) return FALSE;
    exp = getLvExp(_info()->LV);
    if (exp < 0) return FALSE;
    return _info()->EXP >= exp;
}

// SOURCE DATA: .data .rodata
