#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "Game/EntityFactory.h"
#include "Game/Bullet/Bullet.h"
#include "System/TaskMan.h"
#include "System/HeapMan.h"

inline void* operator new(size_t, void* p) { return p; }

/* Debug allocation: com_malloc_d gets the message "<file>(<line>)" and whatever $ra holds here. */
#define NEW_D(p, T, line, args)                                                 \
    {                                                                           \
        char* msg = __FILE__ "(" #line ")";                                     \
        UINT nRA;                                                               \
        __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA));                      \
        p = (T*)com_malloc_d(sizeof(T), msg, nRA);                              \
        if (p != NULL) p = new (p) T args;                                      \
    }

#line 27
CEntity* CEntityFactory::createBullet(const BULLET_PARAM& inParam, CTask* inParent) {
    CTask* parent = CTaskMan::getSysTask(27);
    if (parent == NULL) parent = this;
    CBullet* bullet = NULL;
    INT typeID;

    switch (inParam.iBulletID) {
    case 1: NEW_D(bullet, CBullet, 34, (parent));
        break;
    case 2:
        NEW_D(bullet, CHomingBullet, 37, (parent));
        break;
    }

    if (bullet == NULL) return bullet;

    if (inParent != NULL) {
        typeID = inParent->getTypeID();
        if (typeID == 0x1000 || typeID == 0x1002 || typeID == 0x1004 || typeID == 0x1005 || typeID == 0x2001 ||
            typeID == 0x2000 || typeID == 0x1006 || typeID == 0x2003 || typeID == 0x2004 || typeID == 0x2005 ||
            typeID == 0x4000) {
            bullet->setOwner((CEntity*)inParent);
        }
    }

    if (!bullet->setup(inParam)) {
        bullet->out();
        return NULL;
    }
    return bullet;
}
// SOURCE DATA: .rodata
