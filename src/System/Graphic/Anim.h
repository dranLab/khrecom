// PARTIAL: added for Game/Gimmick/GimmickHitMore
#ifndef SYSTEM_GRAPHIC_ANIM_H
#define SYSTEM_GRAPHIC_ANIM_H

#include "khtypes.h"
#include "System/MainApp.h"

/* Placeholder for the original header's inline function asserting an index into CAnim's two flag words
 * (probably CAnim::getFlag): its strings ("System/Graphic/Anim.h", "2 > idx") head .rodata of every
 * includer. The line number is not known (it is not part of the data). */
/* PARTIAL: added for System/MainTaskFactory - __FILE__ of this header differs per includer ("System/Graphic/anim.h"
 * in System/MainTaskFactory): define ANIM_H_PATH before including it to pick another one. */
#ifndef ANIM_H_PATH
#define ANIM_H_PATH "System/Graphic/Anim.h"
#endif
#ifdef ANIM_H_FULL_CLASS
#include "System/Vector.h"
/* The complete class, for System/Graphic/Anim itself (its key unit: every inline member is emitted there, in this
 * order). The other includers keep the partial class below (their inline helpers were reconstructed under other
 * names). */
#line 1 ANIM_H_PATH
typedef ULONG ANIMID;
class CTask;
class CCoords;
class CRsrcANN;
class CMotData;
class CLegIK;
class CArmIK;
struct MODELDATA;
struct QUATERNION_DT;

struct INT16_8 {
    UBYTE l;
    UBYTE h;
};

class CAnim {
private:
    CCoords* m_pCrds;            /* 0x000 */
    MODELDATA* m_pModelData;     /* 0x004 */
    CRsrcANN* m_pANN;            /* 0x008 */
    CRsrcANN* m_pANNEx;          /* 0x00c */
    CMotData* m_pMotData;        /* 0x010 */
    ANIMID m_noId;               /* 0x014 */
    TIMEQ m_Tq;                  /* 0x018 */
    SHORT m_nSkl;                /* 0x01c */
    SHORT m_nMaxFrame;           /* 0x01e */
    BYTE m_nFps;                 /* 0x020 */
    BYTE m_ZeroHokan;            /* 0x021 */
    SHORT m_nLookBoneNum;        /* 0x022 */
    TIMEQ m_nMaxTq;              /* 0x024 */
    FLOAT m_Speed;               /* 0x028 */
    union {
        USHORT m_Flags[2];       /* 0x02c */
        INT16_8 m_FlagsB[2];
    };
    TIMEQ m_HokanMaxTq;          /* 0x030 */
    TIMEQ m_HokanTq;             /* 0x034 */
    QUATERNION_DT* m_pQuatHokan; /* 0x038 */
    QUATERNION_DT* m_pQuatNow;   /* 0x03c */
    QUATERNION_DT* m_pFixQuat;   /* 0x040 */
    FVECTOR m_vRootMovePreTrs;   /* 0x050 */
    FVECTOR m_vRootMovePreRot;   /* 0x060 */
    FVECTOR m_vRootMoveTrs;      /* 0x070 */
    FVECTOR m_vRootMoveRot;      /* 0x080 */
    FMATRIX m_BufRootMtx;        /* 0x090 */
    union {
        FVECTOR m_vLookPos;      /* 0x0d0 */
        struct {
            float m_fLookPos[3];
            FLOAT m_fRate;
        };
    };
    FVECTOR m_vLookMaxMin;       /* 0x0e0 */
    union {
        FVECTOR m_vLookRot;      /* 0x0f0 */
        struct {
            float m_fLookRot[2];
            FLOAT m_fTimer;
            USHORT m_iLookStatus;
            USHORT m_iLookFlags;
        };
    };
    CLegIK* m_pLegIK;            /* 0x100 */
    CArmIK* m_pArmIK;            /* 0x104 */

public:
    CAnim();
    virtual ~CAnim();

private:
    void _setupRootMove(BOOL inRootMove, BOOL bContinue);
    void _exeRootMove();
    void _setRootMoveFg(BOOL inFg) {
        if (inFg) {
            m_Flags[1] |= 0x8;
        } else {
            m_Flags[1] &= ~0x38;
        }
    }
    void setNowFirstRootMove() { m_Flags[1] |= 0x10; }
    void setNowFirstRootMoveContinue() { m_Flags[1] |= 0x20; }
    void clearNowFirstRootMove() { m_Flags[1] &= ~0x30; }
    BOOL isNowFirstRootMove() {
        if (m_Flags[1] & 0x10) {
            return TRUE;
        }
        return (m_Flags[1] & 0x20) >> 4;
    }
    void setNowFirst() { m_Flags[1] |= 0x4; }
    void clearNowFirst() { m_Flags[1] &= ~0x4; }
    BOOL isNowFirst() { return (m_Flags[1] >> 2) & 1; }
    void setExFlag(BOOL inFg) {
        if (inFg) {
            m_Flags[1] |= 0x200;
        } else {
            m_Flags[1] &= ~0x200;
        }
    }
    BOOL linkEx(INT inRsrcId);
    BOOL linkEx(const CRsrcANN* inAnnEx);
    BOOL linkEx(const CHAR* name);
    void _init(ANIMID id, TIMEQ tq, BOOL inExFg);
    void _change(ANIMID id, TIMEQ tq, TIMEQ gr, BOOL inExFg);
    void _start(ANIMID id, TIMEQ tq, BOOL inExFg);
    void _setUpdate(int nSkl, TIMEQ tqFr, BOOL bClipped);
    void _updateLookPosition(FLOAT fR);
    void _create(INT nSklNum);
    void _dispose();
    void setup(ANIMID id, TIMEQ tq);

public:
    void link(CCoords& crds, INT nGrpID);
    void link(CTask* p, INT nGrpID);
    void link(CTask* p, const CHAR* name, MODELDATA* model);
    void link(CCoords& crds, const CHAR* name, MODELDATA* model);
    void link(CTask* p, const CRsrcANN* ann, MODELDATA* model);
    void link(CCoords& crds, const CRsrcANN* ann, MODELDATA* model);
    BOOL isLinked() const { return m_pCrds != __null; }
    void setRootMove(BOOL inRootMove, BOOL bContinue);
    void save_hokan_by_mtx();
    void init(ANIMID id, TIMEQ tq);
    void change(ANIMID id, TIMEQ tq, TIMEQ gr);
    void change_by_mtx(ANIMID id, TIMEQ tq, TIMEQ gr);
    void start(ANIMID id, TIMEQ tq);
    void start_by_mtx(ANIMID id, TIMEQ tq);
    BOOL initEx(INT inRsrcId, ANIMID id, TIMEQ tq);
    BOOL initEx(const CHAR* rsrc_name, ANIMID id, TIMEQ tq);
    BOOL initEx(const CRsrcANN* inAnnEx, ANIMID id, TIMEQ tq);
    BOOL startEx(INT inRsrcId, ANIMID id, TIMEQ tq);
    BOOL startEx(const CHAR* rsrc_name, ANIMID id, TIMEQ tq);
    BOOL startEx(const CRsrcANN* inAnnEx, ANIMID id, TIMEQ tq);
    BOOL changeEx(INT inRsrcId, ANIMID id, TIMEQ tq, TIMEQ gr);
    BOOL changeEx(const CHAR* rsrc_name, ANIMID id, TIMEQ tq, TIMEQ gr);
    BOOL changeEx(const CRsrcANN* inAnnEx, ANIMID id, TIMEQ tq, TIMEQ gr);
    void play(TIMEQ inAddTq, BOOL bClipped);
    BOOL isMotionEnd() { return getFlag(0) & 3; }
    BOOL isMotionLoop() { return getFlag(0) & 1; }
    BOOL isMotionLooped() { return getFlag(0) & 0x80; }
    void setSpeed(FLOAT s) { m_Speed = s; }
    void setMotion(TIMEQ inTq, TIMEQ inPlus, BOOL bClipped);
    void setZeroHokan(BOOL in_bZeroHokan) { m_ZeroHokan = in_bZeroHokan; }
    ULONG getFlag(UINT idx) const {
#line 296
        if (!(2 > idx)) AssertApp(__FILE__, __LINE__, "2 > idx", 0);
        return m_Flags[idx];
    }
    void clearFlag() { m_Flags[1] &= ~3; }
    TIMEQ getTotalTqCrd() { return m_nMaxTq; }
    TIMEQ getCurrentTqCrd() { return m_Tq; }
    TIMEQ getHokanTq() { return m_HokanTq; }
    void getRootMove(FVECTOR& outTrs, FLOAT& outRotY);
    BOOL isRootMove() { return (m_Flags[1] >> 3) & 1; }
    ANIMID getAnimID() { return m_noId; }
    void setClipCheckFg(BOOL inFg) {
        if (inFg) {
            m_Flags[1] |= 0x100;
        } else {
            m_Flags[1] &= ~0x100;
        }
    }
    BOOL isNowClipCheck() { return (m_Flags[1] >> 8) & 1; }
    void getRootMovePre(FVECTOR& outTrans, FVECTOR& outRot) {
        outTrans = m_vRootMovePreTrs;
        outRot = m_vRootMovePreRot;
    }
    BOOL isLoop(INT inAnimID);
    BOOL isLoopEx(const CHAR* rsrc_name, INT inAnimID);
    BOOL isLoopEx(const CRsrcANN* inAnnEx, INT inAnimID);
    CHAR* getMotFilename(INT inAnimID);
    BOOL isNowExAnim() { return (m_Flags[1] >> 9) & 1; }
    INT getMotCount();
    INT getSklNum() const { return m_nSkl; }
    FLOAT getSpeed() const { return m_Speed; }
    BOOL isNowEffective() const { return m_pMotData != __null; }
    INT checkID(ANIMID id, BOOL bEx);
    void setLookPosition(const FVECTOR& vLook, const FVECTOR& vMax, const FVECTOR& vMin, INT nBoneNum, FLOAT fRate,
                         UINT iFlags);
    void endLookPosition(FLOAT fFrame);
    void updateLookPosition(const FVECTOR& vLook, FLOAT fRate, UINT iFlags);
    BOOL isLookPosition() { return m_iLookStatus & 1; }
    BOOL createLegIK(FLOAT fFootLen, FLOAT fToesLen);
    BOOL createArmIK();
    void updateIK(BOOL bBeforeCoordFix, BOOL bAfterCoordFix, FLOAT fr);
    void setGroundIK(BOOL bOn) {
        if (bOn) {
            m_Flags[1] |= 0x2000;
        } else {
            m_Flags[1] &= ~0x2000;
        }
    }
    CLegIK* getLegIK() const { return m_pLegIK; }
};

#else /* !ANIM_H_FULL_CLASS */
#line 1 ANIM_H_PATH
inline void _animCheckFlagIdx(UINT idx) {
    if (!(2 > idx)) AssertApp(__FILE__, __LINE__, "2 > idx", 0);
}

// PARTIAL: added for Game/Gimmick/GimmickLotusLeaf
typedef ULONG ANIMID;
class CTask;
class CRsrcANN;  // PARTIAL: added for Game/ModelObj
struct MODELDATA; // PARTIAL: added for Game/ModelObj
class CAnim {
private:
    UBYTE _m_0x00[0x14]; /* 0x000 m_pCrds .. 0x010 m_pMotData */
    ANIMID m_noId;       /* 0x014 */
    // PARTIAL: m_Flags added for Game/Gimmick/GimmickDeceiveEnemy
    // PARTIAL: m_Tq split out for Game/Gimmick/GimmickOnceReaction
    TIMEQ m_Tq;          /* 0x018 */
    UBYTE _m_0x1c[0x8];  /* 0x01c .. 0x024 */
    TIMEQ m_nMaxTq;      /* 0x024 */ // PARTIAL: split out for Game/Gimmick/GimmickCarry
    FLOAT m_Speed;       /* 0x028 PARTIAL: split out for Form/Menu/MenuModel */
    USHORT m_Flags[2];   /* 0x02c */
    UBYTE _m_0x30[0xd8] __attribute__((aligned(16))); /* 0x030 .. 0x108 PARTIAL: added for Form/Menu/MenuModel (sizeof 0x110) */
public:
    /* 0x108 vtable */
    // PARTIAL: added for Form/Menu/MenuModel
    CAnim();
    virtual ~CAnim();
    void link(CTask* pTask, INT n);
    void link(CTask* pTask, const CRsrcANN* pANN, MODELDATA* pData); // PARTIAL: added for Game/ModelObj
    // PARTIAL: added for Game/ModelObj (name guessed: clears a bit of m_Flags[1] inline)
    void clearFlag1(USHORT nBit) { m_Flags[1] &= ~nBit; }
    void init(UINT nId, INT n);
    void play(TIMEQ tq, INT n);
    void changeEx(INT nRsrcID, ANIMID inId, TIMEQ inTq, BOOL inLoop);
    void setSpeed(FLOAT s) { m_Speed = s; }
    BOOL isMotionEnd() { return m_Flags[0] & 3; }

    ULONG getFlag(UINT idx) const;
    BOOL isLoop(INT n); // PARTIAL: added for Game/EffectMotion
    ANIMID getAnimID() { return m_noId; }
    TIMEQ getCurrentTqCrd() { return m_Tq; } // PARTIAL: added for Game/Gimmick/GimmickOnceReaction
    TIMEQ getTotalTqCrd() { return m_nMaxTq; } // PARTIAL: added for Game/Gimmick/GimmickCarry
    // PARTIAL: added for Game/Gimmick/GimmickDeceiveEnemy (name guessed: tests m_Flags[1] bit 1 inline)
    BOOL isMotionLooped() { return m_Flags[1] & 2; }
    void change(ANIMID inId, TIMEQ inTq, BOOL inLoop); // PARTIAL: added for Game/Gimmick/GimmickDeceiveEnemy
};

#endif /* ANIM_H_FULL_CLASS */

#endif /* SYSTEM_GRAPHIC_ANIM_H */
