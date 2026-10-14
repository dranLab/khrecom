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
    /* main() is emitted after the vtables, and this asm puts the bytes there: the original object holds an unnamed
     * copy of CLightUnit's (weak) vtable after its own, which tools/ee_asm_compat.py drops from the compiled
     * output because the linker kept the named copy elsewhere. Remove once the tool keeps such copies. */
    virtual void main() {
        __asm__(".rdata\n\t.align 3\n\t.word 0, 0, 0, _$_10CLightUnit, 0, setAnchor__10CLightUnitPC6CCoordRC9MODELDATAi, 0, "
                "reCalc__10CLightUnit, 0, getPointLightData__C10CLightUnit, 0, _calc__10CLightUnit, 0, 0\n\t.text");
    }
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
