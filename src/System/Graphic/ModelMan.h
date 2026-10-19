#ifndef SYSTEM_GRAPHIC_MODELMAN_H
#define SYSTEM_GRAPHIC_MODELMAN_H

#include "khtypes.h"
#include "System/Vector.h"
#include "System/Graphic/Gprim.h"

class CTask;
class CLightUnit;
class CDrawable;
class CDispListBase;
struct MODELDATA;
struct TexInfoStub;

struct MODEL_SEND_CACHE {
    union {
        u_long128 qOBLM;
        struct {
            ULONGLONG lOBLM_L;
            ULONGLONG lOBLM_H;
        };
        struct {
            CTask* pObject;
            INT iBoneStart;
            CLightUnit* pLight;
            INT iMode;
        };
    };
    IVECTOR ivPP;
    FVECTOR fvColorScale;
};

struct SEMI_PACKET_LIST {
    void* pPkt;
    u_long128* pMProg;
    G_LAYER sLayerInfo;
    FIX4 nZ;
};

class CModelMan {
public:
    static void initialize();
    static void finalize();
    static void reset();
    static void setupDrawEnv();
    static void setupDrawMode(INT nInAlphaMode);
    static void clearDispList();
    static void add2DispList(CDrawable& inPrim, FIX4 inZ);
    static CDispListBase* getDispList(INT nDid);
    static void drawDispList();
    static void drawDispListSemi();
    static SEMI_PACKET_LIST* getSemiPktList();
    static INT getSemiPktListNum();
    static void drawDispListFG();
    static void clearTexCache();
    static BOOL updateTexCache(UINT nInIndex, const TexInfoStub& inTex);
    static USHORT getPacketSize(USHORT nInBoneCount, USHORT nInTexCount, USHORT nInTransTexCount,
                                const MODELDATA* pModelData);
    static INT getModelCount();
    static void incModelCount();
    static void decModelCount();
    static void changePrimMode(BOOL bOnlyPrim);
    static G_LAYER getLayerInfo();
    static void setLayerInfo(G_LAYER sLayerInfo);
    static INT getDrawCount();
    static void incDrawCount();
    static void clearDrawCount();
    static INT getTexSendCount(INT i);
    static void incTexSendCount(INT i);
    static void clearTexSendCount(INT i);
    static MODEL_SEND_CACHE& getModelCacheRef();
};

#endif /* SYSTEM_GRAPHIC_MODELMAN_H */
