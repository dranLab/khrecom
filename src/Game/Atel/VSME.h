#ifndef GAME_ATEL_VSME_H
#define GAME_ATEL_VSME_H

#include "khtypes.h"
#include "System/Task.h"

class CAtel;
namespace ATEL { class CAvatar; class CEventTrap; }
class CAtelCardSeq;

union F_UL {
    FLOAT f;
    ULONG ul;
};

namespace ATEL {

struct ATP {
    USHORT tag_count;            /* 0x000 */
    char name[32];               /* 0x002 */
    int tname[64];               /* 0x024 */
    unsigned short loc[64];      /* 0x124 */
    SHORT id;                    /* 0x1a4 */
    BYTE* Dseg;                  /* 0x1a8 */
    INT Freg;                    /* 0x1ac */
    USHORT next_address;         /* 0x1b0 */
    USHORT wait_timer;           /* 0x1b2 */
    SHORT wait_key;              /* 0x1b4 */
    USHORT rq_lv;                /* 0x1b6 */
    SHORT rq_tag;                /* 0x1b8 */
    SHORT rq_m_actor;            /* 0x1ba */
    SHORT status;                /* 0x1bc */
    USHORT address;              /* 0x1be */
};

struct RTP {
    USHORT rq_lv;      /* 0x0 */
    USHORT rq_actor;   /* 0x2 */
    SHORT rq_tag;      /* 0x4 */
    SHORT rq_m_actor;  /* 0x6 */
    SHORT status;      /* 0x8 */
    SHORT wait_key;    /* 0xa */
    RTP() {}
};

struct Actor {
    INT _nID;              /* 0x000 */
    INT _nRsrcID;          /* 0x004 */
    INT _GroupID;          /* 0x008 */
    INT _nMembarID;        /* 0x00c */
    CAvatar* _pAvatar;     /* 0x010 */
    CEventTrap* _pEventTrap; /* 0x014 */
    ATP _ATP;              /* 0x018 */
    FLOAT _fTalkableRadius; /* 0x1d8 */
};

struct INSTR {
    USHORT _cOp;
    USHORT _cReg;
    INT _nAddr;
};

struct SE_INFO {
    INT _nHanle;
    INT _nSeq;
    INT _nVol;
};

union OpStack {
    INT I;
    FLOAT F;
};

void _TRACE(BOOL bDisp, const CHAR* fmt, ...);

class CVSM {
protected:
    BOOL m_PadWaitTime;          /* 0x0000 */
private:
    CAtel& m_Atel;               /* 0x0004 */
    INT m_GAptr;                 /* 0x0008 */
    INT m_FAptr;                 /* 0x000c */
    INT m_Pctr;                  /* 0x0010 */
    INT m_SP;                    /* 0x0014 */
    INT m_Freg;                  /* 0x0018 */
    INT m_InsCount;              /* 0x001c */
    INT m_MaxSD;                 /* 0x0020 */
    INT m_MinFP;                 /* 0x0024 */
    INT m_MaxPC;                 /* 0x0028 */
    INT m_CallC;                 /* 0x002c */
    INT m_MaxActorCount;         /* 0x0030 */
    INT m_ActorCount;            /* 0x0034 */
    INT m_ReadyActorCount;       /* 0x0038 */
    INT m_CurrentActor;          /* 0x003c */
    INT m_WinCloseTimer;         /* 0x0040 */
    TIMEQ m_nFrameRate;          /* 0x0044 */
    FLOAT m_fEventTrapStartWait; /* 0x0048 */
    FLOAT m_fTalkWait;           /* 0x004c */
    INT m_hEffect;               /* 0x0050 */
    CAtelCardSeq* m_pAtelCard;   /* 0x0054 */
    INT m_nFlag;                 /* 0x0058 */
    BOOL m_bTrace;               /* 0x005c */
    INSTR* m_pIseg;              /* 0x0060 */
    BYTE* m_pDseg;               /* 0x0064 */
    INT m_nCurrentActorIdx;      /* 0x0068 */
    Actor* m_pCurrentActor;      /* 0x006c */
    Actor m_ActorTbl[64];        /* 0x0070 */
    BYTE m_ActorStateFnc[31 * 8]; /* 0x7770 */
    RTP m_ReqBuf[64];            /* 0x7868 */
    INT m_nReqBufPtr;            /* 0x7b68 */
    OpStack m_S[100];            /* 0x7b6c */
    SE_INFO m_SeInf;             /* 0x7cfc */

public:
    CVSM(CAtel& atel, BOOL bTrace);
    virtual ~CVSM();
    INT getActorIndex(INT nID) const;
    INT getActorIndex(const CHAR* pszName) const;
    CAvatar* getAvatar(INT nIdx) { return m_ActorTbl[nIdx]._pAvatar; }
    CEventTrap* getEventTrap(INT nIdx) { return m_ActorTbl[nIdx]._pEventTrap; }
    CHAR* getAvatarName(INT nIdx) { return m_ActorTbl[nIdx]._ATP.name; }
    INT getActorNum() { return m_ActorCount; }
    void setPC(INT nPC) { m_Pctr = nPC; }
    void clearFlag(INT nFlag) { m_nFlag &= ~nFlag; }

protected:
    void deleteActorAll();
    void clearReq();
    void execInit();
    INT searchLockOnNpc();
    void initActor();
    void exec(TIMEQ nTQ);
    void execActor();
    void checkEvents();
    void tuneReq();
    void setReq(const RTP& rq);

private:
    BOOL reqN(Actor& act, INT nTag, INT nLv, BOOL bTop);
    BOOL reqNS(Actor& act, const CHAR* pszTag, INT nLv);
    BOOL reqSSD(INT nTagIdx, INT nActIdx, INT nLv);
    INT searchActor(const CHAR* pszActor) const;
    INT searchActorID(const CHAR* pszActor) const;
    void acWaitPad(Actor& act, INT nKey, INT nAddr);
    void acWaitMes(Actor& act, INT nKey, INT nAddr);
    void acWaitCam(Actor& act, INT nKey, INT nAddr);
    void acWaitBusy(Actor& act, INT nKey, INT nAddr);
    void acWaitBusyActor(Actor& act, INT nKey, INT nWin, INT nAddr);
    void acWaitActor(Actor& act, INT nKey, INT nWin, INT nAddr);
    void acWaitRsrc(Actor& act, INT nKey, INT nAddr);
    void acWaitSnatch(Actor& act, INT nKey, INT nAddr);
    void acWaitEquip(Actor& act, INT nKey, INT nAddr);
    void acWaitDelLeftSword(Actor& act, INT nKey, INT nAddr);
    void acWaitLoadEnemy(Actor& act, INT nAddr);
    void acWaitSelectPad(Actor& act, INT nKey, INT nWin, INT nAddr);
    void acWaitSelectPadEx(Actor& act, INT nKey, INT nWin, INT nAddr);
    void acWaitChangeWork(Actor& act, INT nKey, INT nVal, INT nAddr);
    void acWaitReq(Actor& act, INT nTime, INT nAddr);
    void acWaitLoadRsrc(Actor& act, INT nAddr);
    void acWaitCardWind(Actor& act, INT nAddr);
    void acWaitTechWind(Actor& act, INT nAddr);
    void acWaitWorldCardWind(Actor& act, INT nAddr);
    void acWaitCrossFade(Actor& act, INT nAddr);
    BOOL acStatNop();
    BOOL acStatExec();
    BOOL acStatSNWait();
    BOOL acStatShopWait();
    BOOL acStatDropWait();
    BOOL acStatEnemyLoadWait();
    BOOL acStatRCWait();
    BOOL acStatEQWait();
    BOOL acStatWKWait();
    BOOL acStatDLWait();
    BOOL acStatBGWait();
    BOOL acStatEternityWait();
    BOOL acStatCardWndWait();
    BOOL acStatWait();
    BOOL acStatXWait();
    BOOL acStatSWait();
    BOOL acStatBAWait();
    BOOL acStatActorWait();
    BOOL acStatBSWait();
    BOOL isAvatarBusy(INT nType, CAvatar* pAvatar);
    BOOL acStatIWait();
    BOOL acStatCWait();
    BOOL acStatRsrcLoadWait();
    BOOL acStatRWait();
    BOOL acStatCrossFadeWait();
    BOOL acStatTechWndWait();
    BOOL acStatWorldCardWndWait();
    INT player_ctrl(INT nCode, FLOAT fArg);
    void info_disp(INT nCmnd);
    BOOL actor_ref(INT nCmnd, INT nArg);
    void cam_ajt(INT nCmnd, F_UL& arg, INT nID);
    BOOL player_rsrc(INT nCmnd, INT nArg);
};
typedef char _check_vsm_size[sizeof(CVSM) == 0x7d0c ? 1 : -1];

} // namespace ATEL

class CDebugWindow;
class CActorDebug : public CTask {
    CDebugWindow* m_pWindow; /* 0x40 */

public:
    CActorDebug(CTask* pParent);
    virtual ~CActorDebug();
    virtual void main();
};

class CEventLineDisp : public CTask {
public:
    CEventLineDisp(CTask* pParent);
    virtual ~CEventLineDisp();
    virtual void main();
    virtual void disp();
};

void WinOpenSE(INT ctd);
void WinCloseSE();

#endif /* GAME_ATEL_VSME_H */
