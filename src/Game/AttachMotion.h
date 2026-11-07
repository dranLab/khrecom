
#ifndef GAME_ATTACHMOTION_H
#define GAME_ATTACHMOTION_H

#include "khtypes.h"

class CEntity;

/* All virtual functions are inline or pure: no vtable of its own (weak copies of the vtable and the destructor
 * wherever a derived class is constructed or destroyed). */
class CAttachMotion {
protected:
    CEntity* m_pOwner; /* 0x0 */
public:
    CAttachMotion(CEntity* inOwner) : m_pOwner(inOwner) {}
    virtual ~CAttachMotion() {}
    virtual BOOL set(INT inIndex) = 0;
    virtual void stop() = 0;
    virtual void update() = 0;
};

#endif /* GAME_ATTACHMOTION_H */
