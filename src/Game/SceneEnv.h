#ifndef GAME_SCENEENV_H
#define GAME_SCENEENV_H

#include "khtypes.h"
#include "System/Vector.h"
#include "System/Graphic/Effect/EffectMan.h"

class CTask;

namespace ObjType {
enum eObjBit {
    OBJ_BIT_CLEAR = 0,
    OBJ_BIT_NONE = 1,
    OBJ_BIT_PLAYER = 2,
    OBJ_BIT_FRIEND = 131072,
    OBJ_BIT_ENEMY = 4,
    OBJ_BIT_GIMMICK = 8,
    OBJ_BIT_AVATAR = 16,
    OBJ_BIT_BULLET = 32,
    OBJ_BIT_PRIZE = 64,
    OBJ_BIT_DOOR_RU = 128,
    OBJ_BIT_DOOR_RD = 256,
    OBJ_BIT_DOOR_LD = 512,
    OBJ_BIT_DOOR_LU = 1024,
    OBJ_BIT_MAP = 2048,
    OBJ_BIT_BG = 4096,
    OBJ_BIT_ATEL = 8192,
    OBJ_BIT_FUKIDASI = 16384,
    OBJ_BIT_CAMERA = 32768,
    OBJ_BIT_2D = 65536,
    OBJ_BIT_ENCOUNT = 1073741824,
    /* the enum is 8 bytes (0x80000000 and -1 together need a 64-bit type): 0x80000000 and 0xFFFEFFFF are
       positive (the debug info prints them truncated to 32 bits) */
    OBJ_BIT_STILL_SCR = 0x80000000L,
    OBJ_BIT_DOOR = 1920,
    OBJ_BIT_ALL = 0xFFFEFFFFL,
    OBJ_BIT_TRUE_ALL = -1
};
} // namespace ObjType

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
