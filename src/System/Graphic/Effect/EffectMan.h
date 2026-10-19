// PARTIAL: added for Game/Gimmick/GimmickHitReturn
#ifndef SYSTEM_GRAPHIC_EFFECT_EFFECTMAN_H
#define SYSTEM_GRAPHIC_EFFECT_EFFECTMAN_H

#include "khtypes.h"

union FMATRIX;
union FVECTOR; // PARTIAL: added for Game/Gimmick/GimmickTutorial
class CEffect; // PARTIAL: added for Game/Player/SummonTink
class CCoord;  // PARTIAL: added for Form/Menu/WorldSelect

#include "System/Graphic/Effect/ObjType.h"

// PARTIAL: moved here from Game/SceneEnv.h for Game/Player/Summon (CEffectMan::killAll)
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

class CTask;
class CCamera;
struct EFFECT_DATA;
struct EFFECT_SE;
struct EFFMAN_ZSORT_BUF;

class CEffectMan {
public:
    // PARTIAL: complete for System/Graphic/Effect/EffectMan
    static void _effInitialize(int work_memory_size, int packet_memory_size);
    static void _effFinalize();
    static HANDLE _setEffect(const EFFECT_DATA& effect_data, UINT no, ObjType::eObjType eType, INT nNum, const CCoord* pParent,
                             const FMATRIX* pMat, const FVECTOR* pvPos, const FVECTOR* pvRot, FLOAT fWait, const EFFECT_SE* pSeData);
    static void _setCoord(CEffect* pEff, INT nCNum, const CCoord* pParent, const FMATRIX* pMat, const FVECTOR* pvPos,
                          const FVECTOR* pvRot);
    static HANDLE setEffect(const CHAR* inName, UINT inFlag, ObjType::eObjType inType, const FVECTOR* inPos,
                            const FVECTOR* inRot, FLOAT inScale);
    static HANDLE setEffect(const EFFECT_DATA& data, UINT nNo, ObjType::eObjType eType, const FVECTOR* pvPos,
                            const FVECTOR* pvRot, FLOAT fWait, const EFFECT_SE* pSeData);
    static HANDLE setEffect(const CHAR* inName, UINT inFlag, ObjType::eObjType inType, const FMATRIX* inMat, FLOAT inScale);
    static HANDLE setEffect(const EFFECT_DATA& data, UINT nNo, ObjType::eObjType eType, const FMATRIX* pMat, FLOAT fWait,
                            const EFFECT_SE* pSeData);
    static HANDLE attachEffect(const CHAR* inName, UINT inNo, ObjType::eObjType inType, const CCoord* inParent,
                               const FVECTOR* inPos, const FVECTOR* inRot, FLOAT inWait);
    static HANDLE attachEffect(const EFFECT_DATA& data, UINT nNo, ObjType::eObjType eType, const CCoord* pParent,
                               const FVECTOR* pvPos, const FVECTOR* pvRot, FLOAT fWait, const EFFECT_SE* pSeData);
    static HANDLE attachEffect(const CHAR* inName, UINT nNo, ObjType::eObjType eType, const CCoord* pParent,
                               const FMATRIX* pMat, FLOAT fWait);
    static HANDLE attachEffect(const EFFECT_DATA& data, UINT nNo, ObjType::eObjType eType, const CCoord* pParent,
                               const FMATRIX* pMat, FLOAT fWait, const EFFECT_SE* pSeData);
    static HANDLE attachEffect2(const CHAR* inName, UINT nNo, ObjType::eObjType eType, const CCoord* pParent,
                                const CCoord* pParent1, const FVECTOR* pvPos, const FVECTOR* pvPos1, FLOAT fWait);
    static HANDLE attachEffect2(const EFFECT_DATA& data, UINT nNo, ObjType::eObjType eType, const CCoord* pParent,
                                const CCoord* pParent1, const FVECTOR* pvPos, const FVECTOR* pvPos1, FLOAT fWait,
                                const EFFECT_SE* pSeData);
    static HANDLE attachEffect2(const CHAR* inName, UINT nNo, ObjType::eObjType eType, const CCoord* pParent,
                                const CCoord* pParent1, const FMATRIX* pMat, const FMATRIX* pMat1, FLOAT fWait);
    static HANDLE attachEffect2(const EFFECT_DATA& data, UINT nNo, ObjType::eObjType eType, const CCoord* pParent,
                                const CCoord* pParent1, const FMATRIX* pMat, const FMATRIX* pMat1, FLOAT fWait,
                                const EFFECT_SE* pSeData);
    static HANDLE attachEffect3(const CHAR* inName, UINT nNo, ObjType::eObjType eType, const CCoord* pParent,
                                const CCoord* pParent1, const CCoord* pParent2, const FVECTOR* pvPos,
                                const FVECTOR* pvPos1, const FVECTOR* pvPos2, FLOAT fWait);
    static HANDLE attachEffect3(const EFFECT_DATA& data, UINT nNo, ObjType::eObjType eType, const CCoord* pParent,
                                const CCoord* pParent1, const CCoord* pParent2, const FVECTOR* pvPos,
                                const FVECTOR* pvPos1, const FVECTOR* pvPos2, FLOAT fWait, const EFFECT_SE* pSeData);
    static HANDLE attachEffect3(const CHAR* inName, UINT nNo, ObjType::eObjType eType, const CCoord* pParent,
                                const CCoord* pParent1, const CCoord* pParent2, const FMATRIX* pMat,
                                const FMATRIX* pMat1, const FMATRIX* pMat2, FLOAT fWait);
    static HANDLE attachEffect3(const EFFECT_DATA& data, UINT nNo, ObjType::eObjType eType, const CCoord* pParent,
                                const CCoord* pParent1, const CCoord* pParent2, const FMATRIX* pMat,
                                const FMATRIX* pMat1, const FMATRIX* pMat2, FLOAT fWait, const EFFECT_SE* pSeData);
    static void killAll(ObjType::eObjBit inBit, FLOAT inTime); // PARTIAL: added for Game/Player/Summon
    static void killAll(const CHAR* inName, FLOAT inTime); // PARTIAL: added for Game/Player/PlayerManager
    static INT getEffectNameNum(const CHAR* pszName);
    static INT getEffectNameHandles(HANDLE* pOutHandle, const CHAR* pszName, INT nSize);
    static void setFogRange(FLOAT fNear, FLOAT fFar);            // PARTIAL: added for System/Graphic/WorldEnv
    static void setFogClamp(unsigned char nMin, unsigned char nMax);
    static void setFogColor(unsigned char r, unsigned char g, unsigned char b);
    static INT _sortEffect(EFFMAN_ZSORT_BUF* elist, const FMATRIX& mWS, INT nEffNum, INT nSemiNum);
    static BOOL _updateCoord(CEffect* effect);
    static void _updateColor(CEffect* effect);
    static void _calc(FLOAT fFrameRate, const FMATRIX& mWV, const FMATRIX& mVS, CCamera* pCam, BOOL frustumClip);
    static FLOAT getFrameRate();
    static void play(FLOAT fFrameRate);
    static void update();
    static void _sendModel(void* pDmaChanVif1, EFFMAN_ZSORT_BUF* pEffList, INT nIdx, BOOL bReInit);
    static INT setup();
    static void draw();
    static void destroyer();
    static void initialize(CTask* p);
    static void finalize();
    static HANDLE add(CEffect& eff);
    static BOOL del(CEffect& eff);
    static BOOL del(HANDLE hEffect);
    static INT getNum();
    static CEffect* getEffect(HANDLE inHandle); // PARTIAL: added for Game/Player/SummonTink
    static void killEffect(HANDLE inHandle, FLOAT inTime);
    static void loopEndEffect(HANDLE hEffect);
    static CEffect& getExceptEffect();
    static void setWorldViewScreenMatrix(const FMATRIX* pMWV, const FMATRIX* pMVS, CCamera* pCam);
    static void setLastDraw(BOOL bLast);
    static BOOL isLastDraw();
    static void dispDebugBBOXLine(BOOL bDisp);
    static void _setDebugBBOXLine(const FVECTOR* pV);
};

#endif /* SYSTEM_GRAPHIC_EFFECT_EFFECTMAN_H */
