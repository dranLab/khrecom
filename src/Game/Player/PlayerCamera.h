#ifndef GAME_PLAYER_PLAYERCAMERA_H
#define GAME_PLAYER_PLAYERCAMERA_H

#include "khtypes.h"
#include "System/Vector.h"

struct PCAM_DATA {
    FLOAT fViewAngle;
    FLOAT fSize;
    FLOAT fAdjust;
    FLOAT fDistance;
    FVECTOR vEye;
    FVECTOR vAim;
};

struct PCAM_PARAM {
    PCAM_DATA FieldNormal;
    PCAM_DATA FieldFar;
    PCAM_DATA BattleNormal;
    PCAM_DATA BattleLockon;
    PCAM_DATA BattleLoading;
};

namespace PlayerCamera {

struct BCAM_PARAM {
    PCAM_DATA Normal;
    PCAM_DATA Lockon;
};

} // namespace PlayerCamera

/* The full class (PlayerCamera.cpp defines GAME_PLAYER_PLAYERCAMERA_CPP) needs the camera, collision and state
 * machine headers; the other units get the reduced declaration below with the layout of the members they use. */
#ifdef GAME_PLAYER_PLAYERCAMERA_CPP /* MATCH: GAME_PLAYER_PLAYERCAMERA_CPP gate, only for its own unit (tested 2026-11-02) */
#include "System/Graphic/Camera.h"
#include "System/PadHandler.h"
#include "System/Fsm.h"
#include "Game/Entity.h"
#include "Game/Collision/CollMapCamera.h"

class CPlayer;
class CDebugWindow;
class C3DLines;

namespace PlayerCamera {

class CPlayerCamera : public CCamera, public CPadHandler, public CFSM {
private:
    enum { STATE_FIELD, STATE_BATTLE, STATE_MAX };
    static CPlayerCamera* m_pInstance;
    static INT m_nBossCamID;
    HANDLE m_hOwnerBCAM;                       /* 0x0ac */
    CStateTemplate<CPlayerCamera> m_State[2];  /* 0x0b0 */
    INT m_iState;                              /* 0x0f0 */
    CPlayer* m_pPlayer;                        /* 0x0f4 */
    PCAM_PARAM* m_pParam;                      /* 0x0f8 */
    PCAM_DATA* m_pData;                        /* 0x0fc */
    INT m_iMode;                               /* 0x100 */
    INT m_iPreMode;                            /* 0x104 */
    INT m_iModeFieldNear;                      /* 0x108 */
    INT m_iModeFieldFar;                       /* 0x10c */
    UINT m_uiFlag;                             /* 0x110 */
    C3DLines* m_pLines;                        /* 0x114 */
    char m_strCamera0[64];                     /* 0x118 */
    char m_strCamera1[64];                     /* 0x158 */
    char m_strCamera2[64];                     /* 0x198 */
    char m_strCamera3[64];                     /* 0x1d8 */
    char m_strCamera4[64];                     /* 0x218 */
    CDebugWindow* m_pWindow;                   /* 0x258 */
    CCollMapCamera m_collMap;                  /* 0x25c */
    FVECTOR m_vPos;                            /* 0x290 */
    FVECTOR m_vPreAim;                         /* 0x2a0 */
    FVECTOR m_vPreEye;                         /* 0x2b0 */
    FVECTOR m_vAim;                            /* 0x2c0 */
    FVECTOR m_vEye;                            /* 0x2d0 */
    FVECTOR m_vFinalAim;                       /* 0x2e0 */
    FVECTOR m_vFinalEye;                       /* 0x2f0 */
    FVECTOR m_vDir;                            /* 0x300 */
    FVECTOR m_vFinalDir;                       /* 0x310 */
    FVECTOR m_vOffset;                         /* 0x320 */
    FVECTOR m_vLocal;                          /* 0x330 */
    FLOAT m_fDist;                             /* 0x340 */
    HANDLE m_hLockon;                          /* 0x344 */
    TIMEQ m_tqTimer;                           /* 0x348 */
    TIMEQ m_tqEffect;                          /* 0x34c */
    TIMEQ m_tqNear;                            /* 0x350 */
    HANDLE m_hNowPlayer;                       /* 0x354 */
    CEntity* m_pNowPlayer;                     /* 0x358 */
    HANDLE m_hNowTarget;                       /* 0x35c */
    CEntity* m_pNowTarget;                     /* 0x360 */
    FVECTOR m_vPlayer;                         /* 0x370 */
    FVECTOR m_vTarget;                         /* 0x380 */
    FLOAT m_fReverseX;                         /* 0x390 */
    FLOAT m_fReverseY;                         /* 0x394 */
    FLOAT m_fXLimitHi;                         /* 0x398 */
    FLOAT m_fXLimitLow;                        /* 0x39c */
    FLOAT m_fYLimitHi;                         /* 0x3a0 */
    FLOAT m_fRikuShiftY;                       /* 0x3a4 */

public:
    CPlayerCamera(CTask* pParent);
    virtual ~CPlayerCamera();
    virtual void main();
    virtual void disp();
    static INT getPlayerCameraPos(FVECTOR& outPos);
    static FVECTOR getPlayerCameraDir();
    static void setBossCamFile(INT inID);
    static void setBossCamID(INT inID);
    static void setBattleCamData(HANDLE inHandle, const BCAM_PARAM* inParam);
    static void getBattleCamData(BCAM_PARAM& outParam);

private:
    void _initialize();
    void changeState(INT inState);
    INT getCurrentState() { return m_iState; }
    void preStateField();
    void stateField();
    void preStateBattle();
    void stateBattle();
    void calcDirection(FVECTOR& outDir, const FVECTOR& inAim, const FVECTOR& inEye);
    void adjustCamera(FVECTOR& outPos, const FVECTOR& inFinal, FLOAT inRate);
    INT calcMapHit(FVECTOR& outPos, const FVECTOR& inStart, FLOAT inSize, FLOAT inDist);
    INT calcMapHit2(FVECTOR& outPos, const FVECTOR& inStart, FLOAT inSize, FLOAT inDist);
    void collideMapTrueCamera(const FVECTOR& inEye, const FVECTOR& inAim);
    void recalcNearMove();
    void procFieldNormal();
    void procFieldParallel();
    void procBattleNormal();
    void procBattleLockon();
    void changeModeBattle(INT inMode);
    void setFrameEffect(INT inMode, TIMEQ inTime);
    void exeFrameEffect();
    void checkNowPlayer();
    CEntity* getNowPlayer();
    CEntity* getNowTarget();
    FVECTOR& getNowPlayerPos();
    FLOAT getNowPlayerDir();
    FVECTOR& getNowTargetPos();
    FVECTOR& getNowPlayerCamPos();

public:
    void initCamera(INT inMode, INT inReset, FVECTOR* inDir);
    void setFlag(UINT inSet, UINT inMask) { m_uiFlag = (m_uiFlag | inSet) & ~inMask; }
    INT isFlag(UINT inFlag) const { return m_uiFlag & inFlag; }
    PCAM_DATA* getNowCamData() { return m_pData; }
    FVECTOR getNowCamDir() { return m_vDir; }
    FVECTOR getNowCamEye() { return m_vEye; }
    void changeNowPlayer(HANDLE inPlayer, HANDLE inTarget);
    void changeCamMode(INT inMode, INT inReset);
    void enableRikuShiftY();
    static void updateParam(INT inMode) {
        if (m_pInstance && inMode == m_pInstance->m_iMode) m_pInstance->initCamera(inMode, 0, NULL);
    }
    static INT getNowCamMode() { return m_pInstance ? m_pInstance->m_iMode : 0; }
};

} // namespace PlayerCamera

#else /* !GAME_PLAYER_PLAYERCAMERA_CPP */

namespace PlayerCamera {

/* Reduced declaration for the other units (same layout of the members they use). */
class CPlayerCamera {
private:
    static CPlayerCamera* m_pInstance; // PARTIAL: added for Game/Player/PlayerManager
    UBYTE _m_0x000[0x100]; /* 0x000 .. 0x0fc m_pData */ // PARTIAL: added for Game/Player/SummonDumbo
    INT m_iMode;           /* 0x100 */ // PARTIAL: split out for Game/Player/PlayerManager
    UBYTE _m_0x104[0xc];   /* 0x104 m_iPreMode .. 0x10c m_iModeFieldFar */
    UINT m_uiFlag;         /* 0x110 */
    UBYTE _m_0x114[0x1bc]; /* 0x114 .. 0x2c0 m_vAim */ // PARTIAL: added for Game/Gimmick/GimmickCarry
    FVECTOR m_vEye;        /* 0x2d0 */ UBYTE _m_0x2e0[0xd0]; /* 0x2e0 .. 0x3ac */ // PARTIAL: padding to sizeof 0x3b0 added for Game/Player/Player
public:
    CPlayerCamera(CTask* pParent); // PARTIAL: added for Game/Player/Player
    FVECTOR getNowCamEye() { return m_vEye; } // PARTIAL: added for Game/Gimmick/GimmickCarry
    // PARTIAL: added for Game/Player/SummonDumbo (the original is setFlag(UINT inSet, UINT inMask); these callers
    // pass (flag, on/off) - kept as they were matched with it)
    void setFlag(UINT inFlag, BOOL inOn) {
        if (inOn) m_uiFlag |= inFlag;
        else m_uiFlag &= ~inFlag;
    }
    void changeNowPlayer(HANDLE inPlayer, HANDLE inTarget);
    void changeCamMode(INT inMode, INT inReset);
    void initCamera(INT inMode, INT inReset, FVECTOR* inDir); // PARTIAL: added for Game/Player/Player
    void enableRikuShiftY(); // PARTIAL: added for Game/Player/PlayerRiku
    // PARTIAL: added for Game/Player/PlayerRiku (through a reference: the original reloads m_pInstance after stores)
    static CPlayerCamera*& getInstanceRef() { return m_pInstance; }
    INT getCamMode() const { return m_iMode; }
    // PARTIAL: added for Game/Player/SummonMushu
    static void setBattleCamData(HANDLE inHandle, const BCAM_PARAM* inParam);
    static void getBattleCamData(BCAM_PARAM& outParam);
    static INT getNowCamMode() { return m_pInstance ? m_pInstance->m_iMode : 0; } // PARTIAL: added for Game/Player/PlayerManager
    static INT getPlayerCameraPos(FVECTOR& outPos); // PARTIAL: added for Game/Player/PlayerGauge
    static void setBossCamID(INT inID); // PARTIAL: added for Game/Enemy/BattleEnemy
};

} // namespace PlayerCamera

#endif /* GAME_PLAYER_PLAYERCAMERA_CPP */

namespace PlayerCamera {
// PARTIAL: added for Game/Player/PlayerManager
extern char* szTestCamMode[6];
extern PCAM_DATA* pTestCamData[6];
} // namespace PlayerCamera

using PlayerCamera::BCAM_PARAM;

#endif /* GAME_PLAYER_PLAYERCAMERA_H */
