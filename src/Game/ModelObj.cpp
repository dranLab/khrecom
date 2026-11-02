
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/LightUnit.h"
#include "Game/SceneEnv.h"
#include "Game/ModelObj.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new(size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}
inline void* operator new[](size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}
inline void operator delete[](void* mem) { com_free(mem); }

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

#include "System/RsrcDB.h"






void MulVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);

static inline INT isGreater(INT a, INT b) { return a > b; }

BOOL CModelObj::initModelData() {
    if (!initModelData0_setRsrc()) {
        return FALSE;
    }
    if (!initModelData1_setModel()) {
        return FALSE;
    }
    return initModelData2_setLight() != FALSE;
}

BOOL CModelObj::initModelData0_setRsrc() {
    m_iModelDataNum = CRsrcDB::getModelCount(m_iRsrcID);
    if (m_iModelDataNum <= 0) {
        addCoord(1);
        return FALSE;
    }
    m_pModelData = CRsrcDB::getRsrcMDL(m_iRsrcID);
#line 45
    ASSERT(m_pModelData);
    return TRUE;
}

BOOL CModelObj::initModelData1_setModel() {
    m_iModelNum = m_iModelDataNum;
#line 57
    m_pModels = new (__FILE__ "(57)") CModel[m_iModelNum];
    ASSERT(m_pModels);
    for (INT i = 0; i < m_iModelNum; i++) {
        m_pModels[i].link(this, m_pModelData[i]);
    }
    return TRUE;
}

BOOL CModelObj::initModelData2_setLight() {
    INT i;
    for (i = 0; i < m_iModelNum; i++) {
        if (m_pModelData[i]->enablePointLight()) {
            if (!m_pGLight) {
#line 75
                m_pGLight = new (__FILE__ "(75)") CGimmickLightUnit;
                ASSERT(m_pGLight);
            }
            m_pModels[i].setLightUnit(m_pGLight);
        } else if (m_pModelData[i]->enableLight()) {
            if (!m_pNLight) {
#line 82
                m_pNLight = new (__FILE__ "(82)") CLightUnit;
                ASSERT(m_pNLight);
            }
            m_pModels[i].setLightUnit(m_pNLight);
        }
    }
    INT nCenter = isGreater(getCoordNum(), 1);
    if (m_pNLight) {
        m_pNLight->setAnchor(&Crd(nCenter), *m_pModelData[0], 2);
    }
    if (m_pGLight) {
        m_pGLight->setAnchor(&Crd(nCenter), *m_pModelData[0], 2);
    }
    return TRUE;
}

CModelObj::CModelObj(CTask* inpParent, UINT inRsrcID, ObjType::eObjType inObjtype)
    : CTask(inpParent, 0x4000), m_iRsrcID(inRsrcID), m_eObjType(inObjtype), m_iModelDataNum(0), m_pModelData(NULL),
      m_iModelNum(0), m_pModels(NULL), m_pNLight(NULL), m_pGLight(NULL), m_pvColors(NULL) {}

CModelObj::~CModelObj() {
    if (m_pModels) {
        delete[] m_pModels;
    }
    m_pModels = NULL;
    if (m_pGLight) {
        delete m_pGLight;
    }
    m_pGLight = NULL;
    if (m_pNLight) {
        delete m_pNLight;
    }
    m_pNLight = NULL;
    if (m_pvColors) {
        delete[] m_pvColors;
    }
    m_pvColors = NULL;
}

void CModelObj::disp() {
    if (!m_pModels) {
        return;
    }
    if (m_pNLight) {
        m_pNLight->calc();
    }
    if (m_pGLight) {
        m_pGLight->calc();
    }
    FVECTOR* pSceneColor = CSceneEnv::getTypeColor(m_eObjType);
    if (pSceneColor) {
        if (!m_pvColors) {
#line 140
            m_pvColors = new (__FILE__ "(140)") FVECTOR[m_iModelNum];
            ASSERT(m_pvColors);
        }
        for (INT i = 0; i < m_iModelNum; i++) {
            FVECTOR vColor;
            CModel* pModel = &m_pModels[i];
            m_pvColors[i] = pModel->getColorScale();
            MulVector(vColor, m_pvColors[i], *pSceneColor);
            pModel->setColorScale(vColor);
        }
    }
    for (INT i = 0; i < m_iModelNum; i++) {
        m_pModels[i].disp();
    }
}

void CModelObj::after() {
    if (CSceneEnv::getTypeColor(m_eObjType) && m_pvColors) {
        for (INT i = 0; i < m_iModelNum; i++) {
            m_pModels[i].setColorScale(m_pvColors[i]);
        }
    }
}

CModelAnimObj::CModelAnimObj(CTask* inpParent, UINT inRsrcID, ObjType::eObjType inObjtype)
    : CModelObj(inpParent, inRsrcID, inObjtype), m_pANNData(NULL), m_pAnim(NULL) {}

CModelAnimObj::~CModelAnimObj() {
    if (m_pAnim) {
        delete m_pAnim;
    }
    m_pAnim = NULL;
}

void CModelAnimObj::main() {
    if (m_pAnim) {
        m_pAnim->play(getFrameRate(), isClippedLastTime());
    }
}

BOOL CModelAnimObj::initAnimData() {
    if (!m_pModelData) {
        return FALSE;
    }
    m_pANNData = CRsrcDB::getRsrcANN(m_iRsrcID);
    if (!m_pANNData) {
        return FALSE;
    }
#line 234
    m_pAnim = new (__FILE__ "(234)") CAnim;
    if (!m_pAnim) {
        return FALSE;
    }
    m_pAnim->link(this, m_pANNData, m_pModelData[0]);
    m_pAnim->init(0, 0);
    m_pAnim->setClipCheckFg(FALSE);
    return TRUE;
}
// SOURCE DATA: .rodata
