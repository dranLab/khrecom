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
    UBYTE _m_0x1c[0xc];  /* 0x01c .. 0x028 */
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
    // PARTIAL: added for Game/Gimmick/GimmickDeceiveEnemy (name guessed: tests m_Flags[1] bit 1 inline)
    BOOL isMotionLooped() { return m_Flags[1] & 2; }
    void change(ANIMID inId, TIMEQ inTq, BOOL inLoop); // PARTIAL: added for Game/Gimmick/GimmickDeceiveEnemy
};

#endif /* SYSTEM_GRAPHIC_ANIM_H */
