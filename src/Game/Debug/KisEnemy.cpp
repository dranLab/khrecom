
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/BodyCollision.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Debug/KisEnemy.h"
#include "Game/Collision/CollUtil.h"
#include "Game/Collision/CollDisp.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"

/* Placeholder: .rodata has an empty string literal after Str.h's strings (from an unidentified header). */
inline void _kisEnemyEmptyString() {
    TRACE("");
}

#include "System/Vector.h"

#include "Game/Movement.h"

CKisEnemy::CKisEnemy(CTask* inParent) : CBattleEnemy(inParent, 0, 0) {
    setHp(1);
}

CKisEnemy::~CKisEnemy() {}

BOOL CKisEnemy::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBattleEnemy::initalize(inGroupID, inRsrcID)) return FALSE;
    return TRUE;
}

void CKisEnemy::beginStateMain() {
    m_iStep = 0;
}

void CKisEnemy::stateMain() {
    FVECTOR vec;

    if (m_pMove->getPos().z < 0.5f) {
        switch (m_iStep) {
        case 0:
            vec.x = 0.0f;
            vec.y = 0.01f;
            vec.z = 0.0f;
            vec.w = 0.0f;
            m_pMove->setVel(vec);
            if (m_pMove->getPos().y > 2.0f) m_iStep++;
            break;
        case 1:
            vec.x = 0.0f;
            vec.y = -0.01f;
            vec.z = 0.0f;
            vec.w = 0.0f;
            m_pMove->setVel(vec);
            if (m_pMove->getPos().y < 1.0f) m_iStep--;
            break;
        case 2:
            break;
        }
    } else if (m_pMove->getPos().z < 5.5f) {
        if (m_iStep == 0) {
            vec = m_pMove->getRot();
            vec.y += 0.01f;
            if (vec.y > 3.1415927f) vec.y -= 6.2831855f;
            m_pMove->getRot() = vec;
        }
    } else {
        switch (m_iStep) {
        case 0:
            vec.x = 0.05f;
            vec.y = 0.02f;
            vec.z = 0.0f;
            vec.w = 0.0f;
            m_pMove->setVel(vec);
            if (m_pMove->getPos().y > 3.0f) m_iStep++;
            break;
        case 1:
            vec.x = -0.05f;
            vec.y = -0.02f;
            vec.z = 0.0f;
            vec.w = 0.0f;
            m_pMove->setVel(vec);
            if (m_pMove->getPos().y < 1.0f) m_iStep--;
            break;
        case 2:
            break;
        }
        vec = m_pMove->getRot();
        vec.y += 0.02f;
        if (vec.y > 3.1415927f) vec.y -= 6.2831855f;
        m_pMove->getRot() = vec;
    }
}

void CKisEnemy::endStateMain() {}
// SOURCE DATA: .data .rodata
