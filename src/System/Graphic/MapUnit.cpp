
#define MODELDATA_H_PATH "c:/KHCOM/MainApp/System/Graphic/modeldata.h" /* MATCH: this unit names modeldata.h so (header default "System/Graphic/modeldata.h") */
#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define MODEL_H_PATH "c:/KHCOM/MainApp/System/Graphic/Model.h" /* MATCH: this unit names Model.h so (header default "System/Graphic/Model.h") */
#define MAPUNIT_H_PATH "c:/KHCOM/MainApp/System/Graphic/MapUnit.h" /* MATCH: this unit names MapUnit.h so (header default "System/Graphic/MapUnit.h") */
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/Graphic/MapUnit.h"
#include "System/Graphic/Camera.h"
#include "System/Tsin.h"
#include "Game/Map.h"

/* the expression is stringified after expansion (NULL -> __null) */
#define _ASSERT_STR(e) #e
#define ASSERT(e) KH_DEBUG(if (!(e)) AssertApp(__FILE__, __LINE__, _ASSERT_STR(e), 0))

#include "System/RsrcDB.h"





/* (original names unknown) */
static inline void* _getAZFData(void* pInAZF, INT nIndex) {
    if (!pInAZF) return NULL;
    void* pTbl = pInAZF;
    return ((void**)pTbl)[nIndex];
}





















#line 21
MODELDATA* searchDefaultModel(const ZONEPARTS& parts) {
    void* azf = CRsrcDB::getFirstData(9);
    ASSERT(azf!= NULL);
    MODELDATA* d = (MODELDATA*)_getAZFData(azf, parts.modelIndex + 1);
    ASSERT(d != NULL);
    return d;
}

class CSampleMapUnit : public CTask {
private:
    CModel model;
    TDEG deg;
    FLOAT y0;

public:
    CSampleMapUnit(CTask* map, const ZONEPARTS& parts) : CTask(map, 0x4000), model(NULL) {
        model.link(this, searchDefaultModel(parts));
        CCoord& crd = Crd(0);
        const ZONEPARTS& fr = parts;
        FVECTOR vec;
        vec._q = 0;
        vu0UnitMatrix(crd.m_Local);
        if (fr.rot.x) {
            FMATRIX rx;
            vu0UnitMatrix(rx);
            rx.m[1][1] = rx.m[2][2] = tcos(fr.rot.x);
            rx.m[1][2] = tsin(fr.rot.x);
            rx.m[2][1] = -rx.m[1][2];
            crd.m_Local = MulMatrix(crd.m_Local, rx);
        }
        if (fr.rot.y) {
            FMATRIX ry;
            vu0UnitMatrix(ry);
            ry.m[2][2] = ry.m[0][0] = tcos(fr.rot.y);
            ry.m[2][0] = tsin(fr.rot.y);
            ry.m[0][2] = -ry.m[2][0];
            crd.m_Local = MulMatrix(crd.m_Local, ry);
        }
        if (fr.rot.z) {
            FMATRIX rz;
            vu0UnitMatrix(rz);
            rz.m[0][0] = rz.m[1][1] = tcos(fr.rot.z);
            rz.m[0][1] = tsin(fr.rot.z);
            rz.m[1][0] = -rz.m[0][1];
            crd.m_Local = MulMatrix(crd.m_Local, rz);
        }
        crd.m_Local.v[0] *= fr.scale[0];
        crd.m_Local.v[1] *= fr.scale[1];
        crd.m_Local.v[2] *= fr.scale[2];
        crd.m_Local.m[3][0] = fr.trans[0];
        crd.m_Local.m[3][1] = fr.trans[1];
        crd.m_Local.m[3][2] = fr.trans[2];
        crd.m_Local.m[3][3] = 1.0f;
        crd.touch();
        crd.m_Local.m[1][3] = 0.0f;
        crd.m_Local.m[2][3] = 0.0f;
        crd.m_Local.m[0][3] = 0.0f;
        y0 = crd.m_Local.m[3][1];
        deg = 0;
    }
    virtual void main() {
        deg += getFrameRate() * 900 / 3000;
        while (deg > 3600) deg -= 3600;
        Crd(0).m_Local.m[3][1] = y0 + (tsin(deg) + 1.0f) * 10.0f;
        Crd(0).touch();
    }
    virtual void disp() { model.disp(); }
};

MAPUNITTABLE MUT_ita_ = {"ita_", NewClass<CSampleMapUnit>, 2};

// SOURCE DATA: .rodata .bss
// SOURCE DATA SLUS_217.99_retail: .bss .rodata
