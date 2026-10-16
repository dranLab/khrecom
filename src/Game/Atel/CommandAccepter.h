// PARTIAL: split out of Game/Atel/ChatMan.h for Game/Atel/AtelBlur
#ifndef GAME_ATEL_COMMANDACCEPTER_H
#define GAME_ATEL_COMMANDACCEPTER_H

#include "khtypes.h"

namespace ATEL {

class ICommandAccepter {
public:
    virtual INT reqCommand(INT nCmd, void* pParam, INT nID) = 0;
};

} // namespace ATEL

#endif /* GAME_ATEL_COMMANDACCEPTER_H */
