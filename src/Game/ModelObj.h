#ifndef GAME_MODELOBJ_H
#define GAME_MODELOBJ_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Vector.h"
#include "System/Graphic/Effect/EffectMan.h"

class CModel;
class CLightUnit;
class CAnim;
class CRsrcANN;
struct MODELDATA;

class CModelObj : public CTask {
protected:
    UINT m_iRsrcID;
    ObjType::eObjType m_eObjType;
    INT m_iModelDataNum;
    MODELDATA** m_pModelData;
    INT m_iModelNum;
    CModel* m_pModels;
    CLightUnit* m_pNLight;
    CLightUnit* m_pGLight;
    FVECTOR* m_pvColors;

public:
    CModelObj(CTask* inpParent, UINT inRsrcID, ObjType::eObjType inObjtype);
    virtual ~CModelObj();
    virtual void main() {}
    virtual void disp();
    virtual void after();
    void setObjType(ObjType::eObjType inObjtype) { m_eObjType = inObjtype; }
    ObjType::eObjType getObjType() const { return m_eObjType; }

protected:
    virtual BOOL initModelData();
    virtual BOOL initModelData0_setRsrc();
    virtual BOOL initModelData1_setModel();
    virtual BOOL initModelData2_setLight();
};

class CModelAnimObj : public CModelObj {
protected:
    CRsrcANN* m_pANNData;
    CAnim* m_pAnim;

public:
    CModelAnimObj(CTask* inpParent, UINT inRsrcID, ObjType::eObjType inObjtype);
    virtual ~CModelAnimObj();
    virtual void main();

protected:
    virtual BOOL initAnimData();
};

#endif /* GAME_MODELOBJ_H */
