#define OSDDEF_H_PATH "c:/KHCOM/MainApp/Game/OsdDef.h" /* MATCH: this unit names OsdDef.h so (header default "Game/OsdDef.h") */
#include "System/TaskMan.h"
#include "System/Task.h"
#include "Game/OsdDef.h"
#include "Game/EntityFactory.h"
#include "Game/Entity.h"
#include "System/Math3D.h"
#include "System/Com.h"
#include "System/Debug.h"
#include "System/HeapMan.h"

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

inline void* operator new(size_t, void* p) throw() { return p; }

#include "Game/Prize.h"







#include "Game/Atel/Atel.h"




#include "System/Rcf.h"




CEntityFactory* CEntityFactory::m_pInstance = NULL;

CEntityFactory::CEntityFactory() : CTask(getSysTask(1), 0x4000) {
    declareSysTask(4);
}

CEntityFactory::~CEntityFactory() {
    m_pInstance = NULL;
}

#line 46
CEntityFactory* CEntityFactory::getInstance() {
    if (!m_pInstance) {
        m_pInstance = new (__FILE__ "(48)") CEntityFactory;
        ASSERT(m_pInstance);
    }
    return m_pInstance;
}

void CEntityFactory::main() {}

CEntity* CEntityFactory::createEntity(const OBJ_INFO& inObjInfo, CTask* inParent, void* inExtraInfo) {
    FVECTOR rot;
    if (inObjInfo.uiFlag & 2) {
        rot.set(DEG2RAD(inObjInfo.fRotX), DEG2RAD(inObjInfo.dir), DEG2RAD(inObjInfo.fRotZ));
    } else {
        rot.set(0.0f, DEG2RAD(inObjInfo.dir), 0.0f);
    }
    Math3D::LimitPIVector(rot);
    FVECTOR pos;
    pos.set(inObjInfo.x, inObjInfo.y + inObjInfo.height, inObjInfo.z);
    CEntity* entity = createEntity(inObjInfo.sGroupID, inObjInfo.sID, pos, rot, inParent, inExtraInfo);
    if (entity && (USHORT)inObjInfo.sGroupID == 12 && inObjInfo.height == 0.0f) {
        entity->land(5.0f, 1.0f, FALSE);
        entity->setStartPos(entity->getPos());
        entity->updateCoord();
        entity->coordFix();
    }
    return entity;
}

CEntity* CEntityFactory::createEntity(INT inGroupID, INT inRsrcID, const FVECTOR& inPos, FLOAT inDir, CTask* inParent,
                                      void* inExtraInfo) {
    CEntity* entity = createEntity(inGroupID, inRsrcID, inParent, inExtraInfo);
    if (entity) {
        entity->positionFix(inPos, inDir);
        entity->setStartPos(inPos);
    }
    return entity;
}

CEntity* CEntityFactory::createEntity(INT inGroupID, INT inRsrcID, const FVECTOR& inPos, const FVECTOR& inRot,
                                      CTask* inParent, void* inExtraInfo) {
    CEntity* entity = createEntity(inGroupID, inRsrcID, inParent, inExtraInfo);
    if (entity) {
        entity->positionFix(inPos, inRot);
        entity->setStartPos(inPos);
    }
    return entity;
}

CEntity* CEntityFactory::createEntity(INT inGroupID, INT inRsrcID, CTask* inParent, void* inExtraInfo) {
    CEntity* entity = NULL;
    /* (original form unknown: a plain switch over 10/11/12 tests "< 10" instead of "< 11") */
    if (inGroupID != 10) {
        if (inGroupID > 10) {
            switch (inGroupID) {
            case 12:
                entity = createEnemy(inRsrcID, inParent, inExtraInfo);
                break;
            case 11:
                entity = createGimmick(inRsrcID, inParent, inExtraInfo);
                break;
            }
        }
    } else {
        entity = CAtel::createAvatar(inRsrcID, inParent);
    }
    if (entity) {
        if (!entity->initalize(inGroupID, inRsrcID)) {
            entity->out();
            TRACE("Create entity failed!! ... GROUP_ID = 0x%08x, RSRC_ID = 0x%08x\n", inGroupID, inRsrcID);
            return NULL;
        }
        TRACE("Create entity ... GROUP_ID = 0x%08x, RSRC_ID = 0x%08x\n", inGroupID, inRsrcID);
    }
    return entity;
}

CEntity* CEntityFactory::createPrize(const PRIZE_PARAM& inParam, CTask* inParent) {
    CTask* parent;
    if (!inParent) {
        parent = CTaskMan::getSysTask(0x19);
        if (!parent) parent = this;
    } else {
        parent = inParent;
    }
    CPrize* prize = new (__FILE__ "(198)") CPrize(parent);
    /* two failure paths with their own TRACE (cross-jumped): with one shared tail the first null test's
     * delay slot is filled from the target instead of the fallthrough */
    if (prize == NULL) {
        TRACE("Create prize failed!!\n");
        return NULL;
    }
    if (!prize->setup(inParam)) {
        prize->out();
        TRACE("Create prize failed!!\n");
        return NULL;
    }
    return prize;
}

static BOOL LoadEnemy() {
    return TRUE;
}

void LoadMapEnemy() {
    LoadEnemy();
    CRcf::reqLoadEnemy(1);
    CRcf::reqLoadEnemy(302);
    CRcf::reqLoadEnemy(900);
}

void LoadBattleEnemy() {
    LoadEnemy();
    if (dbgCheckUser((USER)5)) {
    }
    CRcf::reqLoadEnemy(900);
}

// SOURCE DATA: .data .rodata
