#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/TaskMan.h"
#include "Game/EntityFactory.h"
#include "Game/Bullet/Bullet.h"
#include "System/HeapMan.h"













#line 27
CEntity* CEntityFactory::createBullet(const BULLET_PARAM& inParam, CTask* inParent) {
    CTask* parent = CTaskMan::getSysTask(27);
    if (parent == NULL) parent = this;
    CBullet* bullet = NULL;


    switch (inParam.iBulletID) {
    case 1: bullet = new KH_AT("(34)") CBullet(parent);
        break;
    case 2:
        bullet = new KH_AT("(37)") CHomingBullet(parent);
        break;
    }

    if (bullet != NULL) {
        if (inParent != NULL) {
            INT typeID = inParent->getTypeID();
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
    }
    return bullet;
}
// SOURCE DATA: .rodata
