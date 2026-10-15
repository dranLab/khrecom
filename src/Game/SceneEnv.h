#ifndef GAME_SCENEENV_H
#define GAME_SCENEENV_H

#include "khtypes.h"
#include "System/Vector.h"
#include "System/Graphic/Effect/EffectMan.h"

class CTask;

/* PARTIAL: ObjType::eObjBit moved to System/Graphic/Effect/EffectMan.h for Game/Player/Summon */

struct CSceneEnvColor;
struct CSceneEnvTask;

class CSceneEnv {
private:
    static CSceneEnvColor* m_pCol;
    static CSceneEnvTask* m_pTask;
    static ObjType::eObjBit m_iStopBit;
    static ObjType::eObjBit m_iStopBitReq;
    static BOOL m_iStopGauge;
    static BOOL m_iStopGaugeReq;
    static UINT m_iStatus;
    static FVECTOR m_vOutColor;

    static void _update(FLOAT fr);

    friend struct CSceneEnvTask;

public:
    static void initialize(CTask* p);
    static void finalize();
    static void updateSys();
    static void setColor(ObjType::eObjBit iBit, FLOAT fFrame, const FVECTOR& vAim, const FVECTOR* pvOrg, INT nCh);
    static BOOL isColorEnd(INT nCh);
    static BOOL isColorAllEnd();
    static FVECTOR* getTypeColor(ObjType::eObjType eType);
    static FVECTOR& getColor(INT nCh);
    static ObjType::eObjBit getColorTypeBit(INT nCh);
    static void setStop(ObjType::eObjBit iBit);
    static ObjType::eObjBit getStopTypeBit();
    static ObjType::eObjBit getStopAndRequestTypeBit();
    static BOOL getStopRequestTypeBit(ObjType::eObjBit* pOutStopBit);
    static void setStopGauge(BOOL bStop);
    static BOOL isStopGauge();
    static BOOL isStopAndRequestGauge();
};

#endif /* GAME_SCENEENV_H */
