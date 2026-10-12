// PARTIAL: added for Game/Gimmick/GimmickHitMore
#ifndef SYSTEM_GRAPHIC_ANIM_H
#define SYSTEM_GRAPHIC_ANIM_H

#include "khtypes.h"
#include "System/MainApp.h"

/* Placeholder for the original header's inline function asserting an index into CAnim's two flag words
 * (probably CAnim::getFlag): its strings ("System/Graphic/Anim.h", "2 > idx") head .rodata of every
 * includer. The line number is not known (it is not part of the data). */
#line 1 "System/Graphic/Anim.h"
inline void _animCheckFlagIdx(UINT idx) {
    if (!(2 > idx)) AssertApp(__FILE__, __LINE__, "2 > idx", 0);
}

// PARTIAL: added for Game/Gimmick/GimmickLotusLeaf
typedef ULONG ANIMID;
class CAnim {
private:
    UBYTE _m_0x00[0x14]; /* 0x000 m_pCrds .. 0x010 m_pMotData */
    ANIMID m_noId;       /* 0x014 */
public:
    ULONG getFlag(UINT idx) const;
    ANIMID getAnimID() { return m_noId; }
};

#endif /* SYSTEM_GRAPHIC_ANIM_H */
