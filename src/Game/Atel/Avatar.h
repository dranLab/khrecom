// PARTIAL: added for Game/Atel/CameraMan (only what that unit uses)
#ifndef GAME_ATEL_AVATAR_H
#define GAME_ATEL_AVATAR_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/Task.h"
#include "Game/Atel/CommandAccepter.h"

#ifndef AVATAR_H_PATH
#define AVATAR_H_PATH "c:/KHCOM/MainApp/Game/Atel/Avatar.h"
#endif

namespace ATEL {

/* CEntity is the second base in the original (CTask at 0x10). */
class CAvatar : public ICommandAccepter, public CTask {
public:
    CAvatar();
};

/* Placeholder for the original header's inline functions: their assert strings are in .rodata of every
 * includer, in this order. Line numbers are not known. */
#line 1 AVATAR_H_PATH
inline void _avatarCheckStructs(void* in_pFadeStruct, void* m_pScaleStruct, void* m_pRotateStruct, void* m_pMoveStruct) {
    if (!(in_pFadeStruct)) AssertApp(__FILE__, __LINE__, "in_pFadeStruct", 0);
    if (!(m_pScaleStruct)) AssertApp(__FILE__, __LINE__, "m_pScaleStruct", 0);
    if (!(m_pRotateStruct)) AssertApp(__FILE__, __LINE__, "m_pRotateStruct", 0);
    if (!(m_pMoveStruct)) AssertApp(__FILE__, __LINE__, "m_pMoveStruct", 0);
}

} // namespace ATEL

#endif /* GAME_ATEL_AVATAR_H */
