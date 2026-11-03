
#ifndef SYSTEM_TASK_H
#define SYSTEM_TASK_H

#include "khtypes.h"
#include "System/Debug.h"
#include "System/SLContainer.h"

class CCoords;
class CCoord;
union FMATRIX;
class CTask;

enum TASK_PRIORITY {
    PRI_BIT_ALWAYS = 32768,
    PRI_BIT_DEFAULT = 16384,
    PRI_BIT_SOMETIMES = 8192,
    PRI_SOMETIMES_SHIFT = 0,
    PRI_SOMETIMES_LV1 = 8192,
    PRI_SOMETIMES_LV2 = 8193,
    PRI_SOMETIMES_LV3 = 8194,
    PRI_SOMETIMES_LV4 = 8196,
    PRI_SOMETIMES_LV5 = 8202,
    PRI_SOMETIMES_MASK = 3,
    PRI_DEFAULT = 16384,
    PRI_PLAYER = 32768,
    PRI_PSHOT = 32768,
    PRI_ENEMY = 8194,
    PRI_ESHOT = 32768,
    PRI_EFFECT = 8194,
    PRI_FURNITURE = 8202,
    PRI_EBOSS = 32768,
    PRI_NPC = 8196,
    PRI_SYSTEM = 32768,
    PRI_SLEEP = 8202,
    PRI_ENEMY_LV1 = 8192,
    PRI_ENEMY_LV2 = 8194,
    PRI_EFFECT_LV1 = 8192,
    PRI_EFFECT_LV2 = 8194,
    PRI_EFFECT_LV3 = 8196,
    PRI_ENEMY_LV3 = 8202,
    PRI_EFFECT_D = 16384
};

struct TaskMainParam;

/* m_nInfo bits */
#define TASK_INFO_SYSTASK 0x0001
#define TASK_INFO_SECONDARY 0x0002
#define TASK_INFO_COORDMANAGED 0x0004
#define TASK_INFO_INACTIVE 0x0008
#define TASK_INFO_INVISIBLE 0x0010
#define TASK_INFO_INACTIVETRG 0x0020
#define TASK_INFO_IGNOREPAUSE 0x0080
#define TASK_INFO_IGNOREPLAYSPEED 0x0100
#define TASK_INFO_IGNORECLIPOUT 0x0200
#define TASK_INFO_FORCEPROC 0x1000
#define TASK_INFO_NOCOORDCALC 0x2000
#define TASK_INFO_MAINTASK 0x4000
#define TASK_INFO_OUT 0x8000

/* All inline members are emitted (weak) in Task.cpp, in declaration order: keep the order. */
class CTask {
    friend class CBaseWnd; // MATCH: System/Wnd's ignorePause/ignorePlaySpeed set m_nInfo directly (friends are not in the debug info)
private:
    union {
        u_long128 _nZeroMember128;
        struct {
            CCoords* m_pCrds;
            TIMEQ m_nLagStock;
            TIMEQ m_nTimeLag;
            SHORT m_nInfo;
            BYTE m_cFrameSkip;
            SBYTE m_cLastClip;
        };
    };
    INT m_nProcPrio;
    HANDLE m_hThis;
    CTask* m_pParent;
    CTaskSLCList m_listChildren;
    CHAR* m_pszDebugInfo;

public:
    CTask(CTask* pParent, INT nProcPrio);
    virtual ~CTask();
    virtual void main() {}
    virtual void disp() {}
    virtual void after() {}
    virtual void onOut() {}
    virtual INT getTypeID() const { return 0; }
    CTask* getParent() { return m_pParent; }
    const CTask* getParent() const { return m_pParent; }
    HANDLE getHandle() { return m_hThis; }
    HANDLE addChild(CTask& inTask);
    BOOL removeChild(CTask& inTask);
    BOOL removeChild(HANDLE hTask);
    BOOL deleteChild(CTask& inTask);
    BOOL deleteChild(HANDLE hTask);
    void deleteAllChildren();
    TIMEQ getFrameRate() const { return m_nTimeLag; }
    FLOAT getFrameRateF() const { return (FLOAT)getFrameRate() * 0.020000001f; }
    TIMEQ getDispFrameRate() const;
    void addCoord(UINT nNum);
    void addCoord(UINT nNum, CCoord* pParent);
    void setCoord(CCoords& crds);
    INT getCoordNum() const;
    void delCoord();
    void createCoords();
    CCoord& Crd(UINT nIndex);
    const CCoord& Crd(UINT nIndex) const;
    CCoords* getCoords() { return m_pCrds; }
    const CCoords* getCoords() const { return m_pCrds; }
    void adjustCoord();
    void adjustSelfCoord();
    FMATRIX& getLocal(UINT nIndex);
    const FMATRIX& getWorld(UINT nIndex) const;
    void setPriority(TASK_PRIORITY pri) { m_nProcPrio = pri; }
    /* MATCH: callers pass plain ints (System/Graphic/PathCamera, TexCtrl, Camera); a member template is
     * not emitted here. */
    template <class T> void setPriority(T pri) { setPriority((TASK_PRIORITY)pri); }
    TASK_PRIORITY getPriority() const { return (TASK_PRIORITY)m_nProcPrio; }
    void out() {
        if (!(m_nInfo & TASK_INFO_OUT)) {
            m_nInfo |= TASK_INFO_OUT;
            onOut();
        }
    }
    BOOL isClippedLastTime() const { return m_cLastClip < 0; }
    void setLastClip(INT c);
    BOOL isOut() const { return (m_nInfo & TASK_INFO_OUT) != 0; }
    BOOL isSysTask() const { return (m_nInfo & TASK_INFO_SYSTASK) != 0; }
    BOOL isMainTask() const { return (m_nInfo & TASK_INFO_MAINTASK) != 0; }
    BOOL isSecondaryObj() const { return (m_nInfo & TASK_INFO_SECONDARY) != 0; }
    BOOL isCoordManaged() const { return (m_nInfo & TASK_INFO_COORDMANAGED) != 0; }
    BOOL isIgnorePause() const { return (m_nInfo & TASK_INFO_IGNOREPAUSE) != 0; }
    BOOL isIgnorePlaySpeed() const { return (m_nInfo & TASK_INFO_IGNOREPLAYSPEED) != 0; }
    BOOL isIgnoreClipOut() const { return (m_nInfo & TASK_INFO_IGNORECLIPOUT) != 0; }
    void dontSkipMainOnce() { m_nInfo |= TASK_INFO_FORCEPROC; }
    BOOL isForceProcNextMain() const { return (USHORT)m_nInfo & TASK_INFO_FORCEPROC; }
    void setActive(BOOL bFlag) {
        if (!bFlag) {
            m_nInfo |= TASK_INFO_INACTIVE | TASK_INFO_INACTIVETRG;
        } else {
            m_nInfo &= ~(TASK_INFO_INACTIVE | TASK_INFO_INACTIVETRG);
        }
    }
    BOOL isActive() const { return !(m_nInfo & (TASK_INFO_OUT | TASK_INFO_INACTIVE)); }
    BOOL isInactive() const { return (m_nInfo & (TASK_INFO_OUT | TASK_INFO_INACTIVE)) != 0; }
    BOOL isInactiveTrg() const { return (m_nInfo & TASK_INFO_INACTIVETRG) != 0; }
    void setVisible(BOOL bFlag) {
        if (!bFlag) {
            m_nInfo |= TASK_INFO_INVISIBLE;
        } else {
            m_nInfo &= ~TASK_INFO_INVISIBLE;
        }
    }
    BOOL isVisible() const { return !(m_nInfo & (TASK_INFO_OUT | TASK_INFO_INVISIBLE)); }
    BOOL isInvisible() const { return (m_nInfo & (TASK_INFO_OUT | TASK_INFO_INVISIBLE)) != 0; }
    void enableCoordCalc(BOOL bFlag);
    BOOL isEnableCoordCalc() const { return !(m_nInfo & TASK_INFO_NOCOORDCALC); }
    void ignorePause(BOOL bFlag) {
        if (bFlag) {
            m_nInfo |= TASK_INFO_IGNOREPAUSE;
        } else {
            m_nInfo &= ~TASK_INFO_IGNOREPAUSE;
        }
    }
    void ignorePlaySpeed(BOOL bFlag) {
        if (bFlag) {
            m_nInfo |= TASK_INFO_IGNOREPLAYSPEED;
        } else {
            m_nInfo &= ~TASK_INFO_IGNOREPLAYSPEED;
        }
    }

protected:
    CTask* getSysTask(UINT nID);
    BOOL addToParent(CTask& inParent);
    BOOL removeFromParent();
    BOOL declareSysTask(UINT nID);
    BOOL removeFromSystem();
    void removeMainTask();
    void manageCoord(BOOL bFlag) {
        if (bFlag) {
            m_nInfo |= TASK_INFO_COORDMANAGED;
        } else {
            m_nInfo &= ~TASK_INFO_COORDMANAGED;
        }
    }
    void ignoreClipOut(BOOL bFlag) {
        if (bFlag) {
            m_nInfo |= TASK_INFO_IGNORECLIPOUT;
        } else {
            m_nInfo &= ~TASK_INFO_IGNORECLIPOUT;
        }
    }
    CTaskSLCList& getChildren() { return m_listChildren; }
    const CTaskSLCList& getChildren() const { return m_listChildren; }
    friend class CTaskSecondary; // MATCH: System/Graphic/Camera reads m_nInfo (friends are not in the debug info)
    friend class CFadeIOMask;    // MATCH: System/Graphic/ScreenMask reads m_nInfo
    friend class CTaskMan;       // MATCH: System/TaskMan manages the private task state

private:
    void invokeMain(TaskMainParam& param);
    void invokeDispAll(INT nMode);
    void invokeSweepChildren();
    BOOL canProcMain(TaskMainParam& param);
    void calcCoord(TIMEQ nTQ);
    void calcCoordAll(TIMEQ nTQ);
    void clearLastClip() { m_cLastClip = 0; }
    void clearTimeLag() { m_nTimeLag = 0; }
    void clearInactiveTrg() { m_nInfo &= ~TASK_INFO_INACTIVETRG; }
    void setParent(CTask* pTask) { m_pParent = pTask; }
    void setHandle(HANDLE hTask) { m_hThis = hTask; }
    SHORT getInfo() { return m_nInfo; }
    void setInfo(SHORT nInfo) { m_nInfo = nInfo; }

public:
    CHAR* getDebugInfo() const { return m_pszDebugInfo; }
};


class CTaskSecondary : public CTask {
public:
    CTaskSecondary(CTask* pParent, INT nProcPrio = 0x4000) : CTask(pParent, nProcPrio) { m_nInfo |= 2; }
};

#endif /* SYSTEM_TASK_H */
