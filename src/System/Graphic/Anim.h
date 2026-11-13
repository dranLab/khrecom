// CAnim (System/Graphic/Anim)
#ifndef SYSTEM_GRAPHIC_ANIM_H
#define SYSTEM_GRAPHIC_ANIM_H

#include "khtypes.h"
#include "System/MainApp.h"

/* CAnim::getFlag's assert strings ("System/Graphic/Anim.h", "2 > idx") head .rodata of every includer. Units whose
 * original calls getFlag out of line define ANIM_H_GETFLAG_DECL_ONLY: a placeholder keeps the strings there. */
/* __FILE__ of this header differs per includer ("System/Graphic/anim.h" in System/MainTaskFactory): define ANIM_H_PATH
 * before including it to pick another one. */
#ifndef ANIM_H_PATH // MATCH: __FILE__ as in the original .rodata
#define ANIM_H_PATH "System/Graphic/Anim.h"
#endif
#include "System/Vector.h"
/* System/Graphic/Anim is the key unit: every inline member is emitted there, in this order. */


#line 1 ANIM_H_PATH
#ifdef ANIM_H_GETFLAG_DECL_ONLY // MATCH: these units' originals call CAnim::getFlag out of line (Entity, EffectMotion, SEMotion, GimmickLotusLeaf)
inline void _animCheckFlagIdx(UINT idx) {
    KH_DEBUG(if (!(2 > idx)) AssertApp(__FILE__, __LINE__, "2 > idx", 0);)
}
#endif
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
#ifdef ANIM_H_GETFLAG_DECL_ONLY // MATCH: called out of line in these units
    ULONG getFlag(UINT idx) const;
#else
    ULONG getFlag(UINT idx) const {
#line 296
        KH_DEBUG(if (!(2 > idx)) AssertApp(__FILE__, __LINE__, "2 > idx", 0);)
        return m_Flags[idx];
    }
#endif
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


#endif /* SYSTEM_GRAPHIC_ANIM_H */
