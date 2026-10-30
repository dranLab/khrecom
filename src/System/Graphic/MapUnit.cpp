#define TASK_H_NO_SLC_OVERFLOW_MSG
#define MODELDATA_H_PATH "c:/KHCOM/MainApp/System/Graphic/modeldata.h"
#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h"
#define MODEL_H_PATH "c:/KHCOM/MainApp/System/Graphic/Model.h"
#define MAPUNIT_H_PATH "c:/KHCOM/MainApp/System/Graphic/MapUnit.h"
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
#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, _ASSERT_STR(e), 0)

#include "System/RsrcDB.h"




void MulMatrix(FMATRIX& res, const FMATRIX& m0, const FMATRIX& m1); /* System/Coord.h */

/* (original names unknown) */
static inline void* _getAZFData(void* pInAZF, INT nIndex) {
    if (!pInAZF) return NULL;
    void* pTbl = pInAZF;
    return ((void**)pTbl)[nIndex];
}

static inline void _unitMatrix(FMATRIX& m) {
    __asm__ volatile("vmr32.xyzw $vf1, $vf0\n"
                     "sqc2 $vf0, 0x30(%0)\n"
                     "vmr32.xyzw $vf2, $vf1\n"
                     "sqc2 $vf1, 0x20(%0)\n"
                     "vmr32.xyzw $vf3, $vf2\n"
                     "sqc2 $vf2, 0x10(%0)\n"
                     "sqc2 $vf3, 0x0(%0)"
                     :
                     : "r"(&m)
                     : "memory");
}

static inline FMATRIX operator*(const FMATRIX& m0, const FMATRIX& m1) {
    FMATRIX b;
    MulMatrix(b, m0, m1);
    return b;
}

static inline void _scale(FVECTOR& v, FLOAT inF) {
    v.x *= inF;
    v.y *= inF;
    v.z *= inF;
    v.w *= inF;
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
        _unitMatrix(crd.m_Local);
        if (fr.rot.x) {
            FMATRIX rx;
            _unitMatrix(rx);
            rx.m[1][1] = rx.m[2][2] = tcos(fr.rot.x);
            rx.m[1][2] = tsin(fr.rot.x);
            rx.m[2][1] = -rx.m[1][2];
            crd.m_Local = crd.m_Local * rx;
        }
        if (fr.rot.y) {
            FMATRIX ry;
            _unitMatrix(ry);
            ry.m[2][2] = ry.m[0][0] = tcos(fr.rot.y);
            ry.m[2][0] = tsin(fr.rot.y);
            ry.m[0][2] = -ry.m[2][0];
            crd.m_Local = crd.m_Local * ry;
        }
        if (fr.rot.z) {
            FMATRIX rz;
            _unitMatrix(rz);
            rz.m[0][0] = rz.m[1][1] = tcos(fr.rot.z);
            rz.m[0][1] = tsin(fr.rot.z);
            rz.m[1][0] = -rz.m[0][1];
            crd.m_Local = crd.m_Local * rz;
        }
        _scale(crd.m_Local.v[0], fr.scale[0]);
        _scale(crd.m_Local.v[1], fr.scale[1]);
        _scale(crd.m_Local.v[2], fr.scale[2]);
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
