#ifndef SYSTEM_GRAPHIC_EFFECT_EFFECT_H
#define SYSTEM_GRAPHIC_EFFECT_EFFECT_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/Vector.h" // PARTIAL: added for Game/Gimmick/GimmickTutorial
#include "System/Graphic/Effect/ObjType.h"

#ifndef EFFECT_H_PATH // MATCH: path override for Game/Enemy/EnemyBarrelspider ("System/Graphic/Effect/Effect.h")
#define EFFECT_H_PATH "System/Graphic/Effect/effect.h"
#endif

#ifdef EFFECT_CPP /* MATCH: EFFECT_CPP gate, only for its own unit (tested 2026-11-02) */
/* Placeholder: effect.cpp includes effect_inl.h (whose asserts own these strings) after other headers, but the
 * strings come first in its .rodata. */
#line 1 EFFECT_H_PATH
inline void _effectCheckValid(BOOL inValid) {
    if (!inValid) AssertApp(__FILE__, __LINE__, "isValid()", 0);
}
#endif

class CEntity; // PARTIAL: added for Game/Player/SummonGenie
class CCoords; // PARTIAL: added for Game/Bullet/Bullet
class CCoord;
class CLightUnit;
struct G_RGBA;
typedef INT FIX4;

struct EFFECT_DATA_DPD {
private:
    u_long128* Data;

public:
    void set(u_long128* data);
    void clear(/* parameters unknown */);
    BOOL is_exist(/* parameters unknown */);
    static void Setup(u_long128* data);
    static void Shift(u_long128* to, u_long128* from);
};

struct EFFECT_DATA {
private:
    u_long128* Data;

public:
    void set(u_long128* data);
    void clear(/* parameters unknown */);
    BOOL no_search(u_int effect_no, u_long128*& dpd, u_int& dpd_id) const;
    int idx_to_no(int idx) const;
    int get_num() const;
    BOOL is_exist(/* parameters unknown */);
    static void Setup(u_long128* data, BOOL bDownVer);
    static void Shift(u_long128* to, u_long128* from);
};

struct EFFECT_SE_DATA {
    USHORT nENo;
    USHORT nFile;
    USHORT nNo;
    UBYTE nFadeTime;
    unsigned char nPad8[1];

    void init(/* parameters unknown */);
    BOOL isEmpty(/* parameters unknown */);
};

struct EFFECT_SE_HEAD {
    INT nId;
    int nPad32[2];
    USHORT nVer;
    USHORT nNum;

    void init(/* parameters unknown */);
};

struct EFFECT_SE {
    EFFECT_SE_HEAD sHead;
    EFFECT_SE_DATA sData[1];

    void check() const;
    const EFFECT_SE_DATA* getData(INT nNo) const;
    INT getSeId(INT nNo) const;
    INT getSeFade(INT nNo) const;
    INT getNum() const;
    INT getVer() const;
    INT getDataSize() const;
};

/* Every inline member is emitted (weak) in effect.cpp, in declaration order: keep the order. */
/* The inline members are defined in effect_inl.h, in the order effect.cpp emits them (weak). */
class CEffect {
    friend class CEffectMan; // PARTIAL: added for System/Graphic/Effect/EffectMan
    FVECTOR m_vScale;       /* 0x00 */
    FVECTOR m_vColor;       /* 0x10 */
    HANDLE m_hHandle;       /* 0x20 */
    CCoords* m_pCrds;       /* 0x24 */
    FVECTOR* m_pVecLst;     /* 0x28 */
    ObjType::eObjType m_eType; /* 0x2c */
public:
    u_int m_iFlag;          /* 0x30 */
private:
    void* m_pData;          /* 0x34 */
    void* m_pPartMng;       /* 0x38 */
    float m_fFade;          /* 0x3c */
    float m_fFadeAdd;       /* 0x40 */
    HANDLE m_hSe;           /* 0x44 */
    INT m_nSeID;            /* 0x48 */
    UBYTE m_nSeFade;        /* 0x4c */
    UBYTE m_nMCode;         /* 0x4d */
    USHORT m_nEffNo;        /* 0x4e */
    CLightUnit* m_pNLight;  /* 0x50 */
    SHORT m_nZOffset;       /* 0x54 */
    SHORT m_nEntZOfs;       /* 0x56 */
    HANDLE m_hEnt;          /* 0x58 */
    char m_pEffName[20];    /* 0x5c */
    union {                 /* 0x70 */
        struct {
            FVECTOR* List;
            int Num;
        } ReferencePos;
        struct {
            FMATRIX* List;
            int Num;
        } ReferenceMatrix;
    };

public:
    CEffect();
    virtual ~CEffect();

private:
    void _dispose();
    inline HANDLE _getHandle() const;
    inline void _setHandle(HANDLE h);
    void _playSE(const EFFECT_SE* pSeData, UINT nNo);
    void _updateSE();
    void _fadeoutSE(FLOAT fFrame);
    void _setSpecialData();
    BOOL _fade(FLOAT fFR);
    BOOL _viewclip(const FMATRIX& mLC, BOOL frustumClip);
    int _setData(const EFFECT_DATA& effect_data, u_int no);
    FVECTOR& _pos();
    FVECTOR& _scale();
    FMATRIX& _matrix();
    FVECTOR& _color();
    INT _strcmpName(const char* pszEffName);
    void _setName(const char* pszEffName);
    BOOL _checkData();
    BOOL start(const EFFECT_DATA& Data, UINT nNo, ObjType::eObjType eType, INT nCrdNum, FLOAT fWait);
    inline void setType(ObjType::eObjType eType);
    inline ObjType::eObjType getType();

public:
    inline CCoord& Crd(INT nNum);
    inline CCoord& Crd(INT nNum) const;
    inline INT getCoordNum() const;
#ifndef EFFECT_CPP /* MATCH: EFFECT_CPP gate, only for its own unit (tested 2026-11-02) */
    CCoords* getCoords() { return m_pCrds; } const FVECTOR& getScale() const { return m_vScale; } // PARTIAL: added for Game/Bullet/Bullet (getScale: Game/Enemy/BattleEnemy, name unknown)
    // PARTIAL: added for Game/Player/FriendDonald
    void setScale(FLOAT _x, FLOAT _y, FLOAT _z) { m_vScale.set(_x, _y, _z); }
#endif
    void detachCoord();
    inline FVECTOR& scale();
    inline void setScale(const FVECTOR& vScale);
    inline void setScale(FLOAT fScale);
    inline FVECTOR& color();
    void setIColor(const G_RGBA& color);
    inline void setAttachNoRot(BOOL bVal);
    inline BOOL isAttachNoRot() const;
    inline void setAttachReverseTrs(BOOL bVal);
    inline BOOL isAttachReverseTrs() const;
    inline void setAttachNoScale(BOOL bVal);
    inline BOOL isAttachNoScale() const;
    void setLightCalc(BOOL bVal);
    BOOL isLightCalc() const;
    BOOL calcLight(FVECTOR& outColor, const FVECTOR& inColor);
    void setSkipFrame(FLOAT fFrame);
    INT getTotalFrame();
    FLOAT getNowFrame();
    BOOL isEndFrame();
    void kill();
    void fadeout(FLOAT fFrame);
    void loopEnd();
    void setDrawFirst(u_int priority);
    void setDrawLast(u_int priority);
    inline void setZOffset(FIX4 nZOffset);
    inline FIX4 getZOffset();
    void setZSortEntity(CEntity* pEnt, FIX4 nEntZOfs, BOOL bAlpha);
    CEntity* getZSortEntity();
    inline FIX4 getZSortEntityOffset();
    inline BOOL isEntityAlpha();
    void setStop(BOOL b);
    void flipStop();
    inline BOOL isStop();
    void setDrawing(BOOL b);
    inline BOOL isDrawing();
    void setViewclip(BOOL b);
    BOOL isLoop();
    inline BOOL isActive();

private:
    inline BOOL isAlive();
    BOOL isWaiting();
    BOOL isDelete();
    BOOL isSuicide();
    BOOL isValid();
    inline void setAttachFlag(BOOL bVal);
    inline BOOL isAttachFlag() const;
    void setReferenceCoordPos(INT nNum);

public:
    void setReferencePos(FVECTOR* list, int num);

private:
    void setReferenceMatrix(FMATRIX* list, int num);
    char* print_prof();
};

#ifndef EFFECT_CPP /* MATCH: EFFECT_CPP gate, only for its own unit (tested 2026-11-02) */
#include "System/Graphic/Effect/effect_inl.h"
#endif

#endif /* SYSTEM_GRAPHIC_EFFECT_EFFECT_H */
