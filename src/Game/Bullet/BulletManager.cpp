#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "Game/Bullet/BulletManager.h"
#include "Game/Bullet/Bullet.h"

CBulletManager::CBulletManager(CTask* inParent) : CTask(inParent, 0x4000) {
    declareSysTask(27);
}

CBulletManager::~CBulletManager() {}

INT CBulletManager::kill(HANDLE inOwner) {
    INT count = 0;
    SLContainerT<CTask>* pContainer;

    for (pContainer = getChildren().getHead(); pContainer != NULL; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task != NULL && task->getTypeID() == 0x1007 && ((CBullet*)task)->getOwner() == inOwner) {
            count++;
            ((CBullet*)task)->kill();
        }
    }
    return count;
}

BOOL CBulletManager::isExist(HANDLE inOwner) {
    SLContainerT<CTask>* pContainer;

    for (pContainer = getChildren().getHead(); pContainer != NULL; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task != NULL && task->getTypeID() == 0x1007 && ((CBullet*)task)->getOwner() == inOwner) {
            return TRUE;
        }
    }
    return FALSE;
}
// SOURCE DATA: .rodata
