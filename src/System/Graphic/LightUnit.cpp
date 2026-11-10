#include "System/TaskMan.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/Debug.h"
#include "System/Graphic/Camera.h"
#include "System/Graphic/WorldEnv.h"
#include "Game/Map.h"
#include "System/Graphic/LightUnit.h"

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)
#define POINT_LIGHT_MAX 4

void Normalize(FVECTOR& v0, const FVECTOR& v1);
void AbsVector(FVECTOR& v0, const FVECTOR& v1);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void spinMatrixX(FMATRIX& m0, const FMATRIX& m1, INT deg);
void spinMatrixY(FMATRIX& m0, const FMATRIX& m1, INT deg);
namespace Math3D {
FLOAT GetDistance2OfLimitedLineAndPoint(const FVECTOR& inP1, const FVECTOR& inP2, const FVECTOR& inP);
}

/* The map viewer of the debug DLL (task 0x31): not in the main program, the original calls address 0 */
class CMapViewer {
    UBYTE _pad[0x3410];
    const CCoord* m_pPlCrd; /* 0x3410 */
    INT m_LightCount;       /* 0x3414 */

public:
    void setPlayerCrd(const CCoord* p) { m_pPlCrd = p; }
    void setLightCount(INT count) { m_LightCount = count; }
    void setActiveLight(INT idx, const void* l, INT kind);
};
asm(".weak setActiveLight__10CMapVieweriPCvi\nsetActiveLight__10CMapVieweriPCvi = 0\n");

union _LIGHT_POWER {
    struct {
        FLOAT pow;
        FLOAT dist2;
        INT idx;
        INT kind;
    };
    u_long128 _q;
};

struct _TMPTMP {
    POINTLIGHT* l;
    FLOAT pow;
};

static FLOAT getLightVolume(const FVECTOR& ctr, INFINITELIGHT* l);
static FLOAT getLightVolume(FLOAT& dv, const FVECTOR& ctr, POINTLIGHT* l);
static FLOAT getLightVolume(const FVECTOR& c1, const FVECTOR& c2, POINTLIGHT* l);

static INFINITELIGHT inflight[4] = {
    {{-822.740173f, 547.139526f, -646.249939f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}},
    {{822.740173f, 547.139526f, -646.249939f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}},
    {{-822.740173f, 547.139526f, 646.249939f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}},
    {{822.740173f, 547.139526f, 646.249939f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}},
};

static AMBIENTLIGHT ambientlight = {{1.0f, 1.0f, 1.0f, 0.0f}};

LIGHTDATA ALF_default = {4, 0, 0, 0, inflight, NULL, NULL, &ambientlight};

static const FVECTOR lightVolumeVec = {0.299f, 0.587f, 0.114f, 0.0f};

void CLightUnit::setAmbient(const FVECTOR& in_color) {
    m_Ambient = in_color;
    m_Ambient.w = sceVu0InnerProduct((float*)in_color.v, (float*)lightVolumeVec.v) + -1.0f;
}

void CLightUnit::setAnchor(const CCoord* p, INT kind) {
    m_pCrd = (CCoord*)p;
    m_Kind = kind;
    if (kind == 1) {
        CMapViewer* pViewer = (CMapViewer*)CTaskMan::getSysTask(0x31);
        if (pViewer) {
            pViewer->setPlayerCrd(p);
        }
    }
}

void CLightUnit::getCenter(FVECTOR& ctr) {
    ctr = m_pCrd->m_World.v[3];
}

LIGHTDATA* CLightUnit::getLightData() const {
    LIGHTDATA* ld = &ALF_default;
    CMap* pMap = (CMap*)CTaskMan::getSysTask(2);
    if (pMap) {
        if (!m_Map) {
            ld = pMap->getLightData();
        } else {
            ld = pMap->getMapLightData();
        }
    }
    return ld;
}

static void _getPowColor(FVECTOR& color, FLOAT dv, const POINTLIGHT* l) {
    sceVu0ClampVector(color.v, (float*)l->col.v, -2.0f, 2.0f);
    FLOAT start = l->start * l->start;
    FLOAT end = l->end * l->end;
    if (!(dv <= start)) {
        FLOAT pow = 1.0f - (dv - start) / (end - start);
        color.x *= pow;
        color.y *= pow;
        color.z *= pow;
        color.w *= pow;
    }
}

static void _setLightPower(_LIGHT_POWER* ll, INT lbsuu, FLOAT pow, FLOAT dist2, INT i, INT kind) {
    int j;
    for (j = 0; j < lbsuu; j++) {
        if (ll[j].pow < pow) {
            int k;
            for (k = lbsuu - 1; k >= j + 1; k--) {
                ll[k] = ll[k - 1];
            }
            ll[j].pow = pow;
            ll[j].dist2 = dist2;
            ll[j].idx = i;
            ll[j].kind = kind;
            return;
        }
    }
}

void CLightUnit::_calc(const FVECTOR& ctr) {
    LIGHTDATA* ld;
    int lbsuu;
    int count;
    int i;
    FLOAT fDist2;
    if (isCalc()) {
        return;
    }
    m_iFlags |= 1;
    lbsuu = 5;
    ld = &ALF_default;
    CMap* pMap = (CMap*)CTaskMan::getSysTask(2);
    if (pMap) {
        ld = pMap->getLightData();
    }
    if (m_Kind == 1) {
        if (CTaskMan::getSysTask(0x31)) {
            lbsuu = 16;
        }
    }
    _LIGHT_POWER ll[lbsuu];
    FVECTOR vlight[5];
    FVECTOR vcolor[5];
    count = 0;
    for (i = 0; i < lbsuu; i++) {
        ll[i].pow = -1.0f;
    }
    for (i = 0; i < ld->point_suu; i++) {
        FLOAT pow = getLightVolume(fDist2, ctr, &ld->point[i]);
        if (pow > 0.0f) {
            count++;
        }
        _setLightPower(ll, lbsuu, pow, fDist2, i, 2);
    }
    for (i = 0; i < ld->infinite_suu; i++) {
        _setLightPower(ll, lbsuu, getLightVolume(ctr, &ld->infinite[i]), 0.0f, i, 1);
    }
    CWorldEnv::initSeekActiveEffLight();
    CEffectLight* el;
    while ((el = CWorldEnv::seekNextActiveEffLight()) != NULL) {
        FLOAT pow = getLightVolume(fDist2, ctr, el);
        _setLightPower(ll, lbsuu, pow, fDist2, el->getID(), 5);
    }
    for (i = 0; i < lbsuu; i++) {
        FVECTOR light;
        FVECTOR color;
        if (ll[i].pow <= 0.0f) {
            light._q = 0;
            color._q = 0;
            if (m_Kind == 1) {
                CMapViewer* pViewer = (CMapViewer*)CTaskMan::getSysTask(0x31);
                if (pViewer) {
                    pViewer->setActiveLight(i, NULL, 0);
                }
            }
        } else {
            switch (ll[i].kind) {
            case 2: {
                POINTLIGHT* l = &ld->point[ll[i].idx];
                sceVu0SubVector(light.v, l->pos.v, (float*)ctr.v);
                FLOAT dv = ll[i].dist2;
                sceVu0DivVector(light.v, light.v, dv);
                _getPowColor(color, dv, l);
                if (m_Kind == 1) {
                    CMapViewer* pViewer = (CMapViewer*)CTaskMan::getSysTask(0x31);
                    if (pViewer) {
                        pViewer->setActiveLight(i, l, 2);
                    }
                }
                break;
            }
            case 1: {
                INFINITELIGHT* l = &ld->infinite[ll[i].idx];
                light = l->dir;
                color = l->col;
                if (m_Kind == 1) {
                    CMapViewer* pViewer = (CMapViewer*)CTaskMan::getSysTask(0x31);
                    if (pViewer) {
                        pViewer->setActiveLight(i, l, 1);
                    }
                }
                break;
            }
            case 5: {
                POINTLIGHT* l = CWorldEnv::getEffLight(ll[i].idx);
                sceVu0SubVector(light.v, l->pos.v, (float*)ctr.v);
                FLOAT dv = ll[i].dist2;
                sceVu0DivVector(light.v, light.v, dv);
                _getPowColor(color, dv, l);
                break;
            }
            }
        }
        if (i < 5) {
            vlight[i] = light;
            vcolor[i] = color;
        }
    }
    Normalize(vlight[3], vlight[3]);
    Normalize(vlight[4], vlight[4]);
    vlight[3].x += vlight[4].x;
    vlight[3].y += vlight[4].y;
    vlight[3].z += vlight[4].z;
    vlight[3].w += vlight[4].w;
    Normalize(vlight[3], vlight[3]);
    vcolor[3].x = (vcolor[3].x + vcolor[4].x) * 0.5f;
    vcolor[3].y = (vcolor[3].y + vcolor[4].y) * 0.5f;
    vcolor[3].z = (vcolor[3].z + vcolor[4].z) * 0.5f;
    vcolor[3].w = (vcolor[3].w + vcolor[4].w) * 0.5f;
    for (i = 0; i < 4; i++) {
        setLight(i, vlight[i], vcolor[i]);
    }
    if (!CTaskMan::getSysTask(2)) {
        static INT debug_deg = 0;
        static INT debug_deg2 = 0;
        INT rx = dbgAnalogRX(USR_SYSTEM, 0);
        INT ry = dbgAnalogRY(USR_SYSTEM, 0);
        debug_deg += rx * 18 / 4096;
        debug_deg2 += ry * 18 / 4096;
        spinMatrixY(m_Light, m_Light, debug_deg);
        spinMatrixX(m_Light, m_Light, debug_deg2);
    }
    if (m_Kind == 1) {
        CMapViewer* pViewer = (CMapViewer*)CTaskMan::getSysTask(0x31);
        if (pViewer) {
            pViewer->setLightCount(count);
        }
    }
    setAmbient(ld->ambient->col);
}

void CLightUnit::setLight(INT idx, FVECTOR in_light, FVECTOR in_color) {
    if ((UINT)idx < 4) {
        FVECTOR light;
        Normalize(light, in_light);
        m_Light.m[0][idx] = light.x;
        m_Light.m[1][idx] = light.y;
        m_Light.m[2][idx] = light.z;
        m_Light.m[3][idx] = 0.0f;
        m_Color.m[idx][0] = in_color.x;
        m_Color.m[idx][1] = in_color.y;
        m_Color.m[idx][2] = in_color.z;
        FVECTOR vLCol;
        AbsVector(vLCol, in_color);
        m_Color.m[idx][3] = sceVu0InnerProduct(vLCol.v, (float*)lightVolumeVec.v);
    }
}



void CLightUnit::betaShade(FVECTOR& out, const FVECTOR& in) {
    FVECTOR ret = m_Ambient;
    ret += m_Color.v[0];
    ret += m_Color.v[1];
    ret += m_Color.v[2];
    ret += m_Color.v[3];
    ret.w = 1.0f;
    out = ret * in;
    if (out.x > 255.0f) {
        out.x = 255.0f;
    }
    if (out.y > 255.0f) {
        out.y = 255.0f;
    }
    if (out.z > 255.0f) {
        out.z = 255.0f;
    }
}

static FLOAT getLightVolume(const FVECTOR& ctr, INFINITELIGHT* l) {
    FVECTOR vLCol;
    AbsVector(vLCol, l->col);
    return sceVu0InnerProduct(vLCol.v, (float*)lightVolumeVec.v);
}

static FLOAT getLightVolume(FLOAT& dv, const FVECTOR& ctr, POINTLIGHT* l) {
    if (l->nType == 0) {
        FVECTOR d;
        d.x = ctr.x - l->pos.x;
        if (d.x > l->end) {
            return 0.0f;
        }
        if (-d.x > l->end) {
            return 0.0f;
        }
        d.z = ctr.z - l->pos.z;
        if (d.z > l->end) {
            return 0.0f;
        }
        d.y = ctr.y - l->pos.y;
        dv = sceVu0InnerProduct(d.v, d.v);
    } else {
        dv = Math3D::GetDistance2OfLimitedLineAndPoint(l->pos, l->pos1, ctr);
    }
    FLOAT end = l->end * l->end;
    if (dv >= end) {
        return 0.0f;
    }
    FVECTOR vLCol;
    AbsVector(vLCol, l->col);
    FLOAT pow = sceVu0InnerProduct(vLCol.v, (float*)lightVolumeVec.v);
    FLOAT start = l->start * l->start;
    if (dv <= start) {
        return pow;
    }
    return pow * (1.0f - (dv - start) / (end - start));
}

static inline FLOAT ABS(FLOAT f) {
    FLOAT ret;
    __asm__ volatile("abs.s %0, %1" : "=f"(ret) : "f"(f));
    return ret;
}

static FLOAT getLightVolume(const FVECTOR& c1, const FVECTOR& c2, POINTLIGHT* l) {
    FVECTOR d;
    FVECTOR d1;
    FVECTOR d2;
    d1.x = c1.x - l->pos.x;
    d2.x = c2.x - l->pos.x;
    d.x = (ABS(d1.x) > ABS(d2.x)) ? ABS(d2.x) : ABS(d1.x);
    if (d.x > l->end) {
        if (d1.x * d2.x >= 0.0f) {
            return 0.0f;
        }
    }
    d1.z = c1.z - l->pos.z;
    d2.z = c2.z - l->pos.z;
    d.z = (ABS(d1.z) > ABS(d2.z)) ? ABS(d2.z) : ABS(d1.z);
    if (d.z > l->end) {
        if (d1.z * d2.z >= 0.0f) {
            return 0.0f;
        }
    }
    d1.y = c1.y - l->pos.y;
    d2.y = c2.y - l->pos.y;
    d.y = (ABS(d1.y) > ABS(d2.y)) ? ABS(d2.y) : ABS(d1.y);
    if (d.y > l->end) {
        if (d1.y * d2.y >= 0.0f) {
            return 0.0f;
        }
    }
    return l->col.w;
}

void CMapLightUnit::setAnchor(const CCoord* p, const BBOX& bbox, INT kind) {
    CLightUnit::setAnchor(p, kind);
    m_bbox = &bbox;
}

void CMapLightUnit::setAnchor(const CCoord* p, const MODELDATA& modeldata, INT kind) {
    setAnchor(p, *modeldata.m_pBBox, kind);
}

static inline FVECTOR operator*(const FMATRIX& m, const FVECTOR& v) {
    FVECTOR r;
    ApplyMatrix(r, m, v);
    return r;
}

void CMapLightUnit::calcCenter(const FMATRIX& m) {
    int i;
    for (i = 0; i < 2; i++) {
        ctr[i].x = m_bbox->x[i];
        ctr[i].y = m_bbox->y[i];
        ctr[i].z = m_bbox->z[i];
        ctr[i].w = 1.0f;
        ctr[i] = m * ctr[i];
    }
}

void CMapLightUnit::init() {
    LIGHTDATA* ld = getLightData();
    int i;
    calcCenter(*(FMATRIX*)m_pCrd);
    for (i = 0; i < 4; i++) {
        m_Fixed[i] = NULL;
        m_FixedVolume[i] = 0.0f;
    }
    for (i = 0; i < ld->point_suu; i++) {
        POINTLIGHT* l = &ld->point[i];
        FLOAT pow = getLightVolume(ctr[0], ctr[1], l);
        int j;
        for (j = 0; j < 4; j++) {
            if (m_FixedVolume[j] < pow) {
                int k;
                for (k = 3; k >= j + 1; k--) {
                    m_Fixed[k] = m_Fixed[k - 1];
                    m_FixedVolume[k] = m_FixedVolume[k - 1];
                }
                m_Fixed[j] = l;
                m_FixedVolume[j] = pow;
                break;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        if (i < ld->infinite_suu) {
            CLightUnit::setLight(i, ld->infinite[i].dir, ld->infinite[i].col);
        } else {
            FVECTOR v;
            v._q = 0;
            CLightUnit::setLight(i, v, v);
        }
    }
    setAmbient(ld->ambient->col);
}

void CMapLightUnit::_calc() {
    _TMPTMP ll[4];
    int i;
    if (isCalc()) {
        return;
    }
    m_iFlags |= 1;
    for (i = 0; i < 4; i++) {
        ll[i].l = m_Fixed[i];
        ll[i].pow = m_FixedVolume[i];
    }
    CWorldEnv::initSeekActiveEffLight();
    CEffectLight* l;
    while ((l = CWorldEnv::seekNextActiveEffLight()) != NULL) {
        FLOAT pow = getLightVolume(ctr[0], ctr[1], l);
        int j;
        for (j = 0; j < 4; j++) {
            if (ll[j].pow < pow) {
                int k;
                for (k = 3; k >= j + 1; k--) {
                    ll[k] = ll[k - 1];
                }
                ll[j].pow = pow;
                ll[j].l = l;
                break;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        setLight(i, ll[i].l);
    }
}

#line 699
void CMapLightUnit::setLight(INT idx, const POINTLIGHT* l) {
    ASSERT((idx>= 0) &&(idx< POINT_LIGHT_MAX));
    // (original line numbers: the asserts are at 700, 704, 746 and 747)

    INT addr = (INT)m_Point;
    ASSERT((addr&0xf)==0);
    if (l) {
        FLOAT end = l->end;
        FLOAT start = l->start;
        if (end <= start) {
            start = end - 1.0f;
        }
        if (start <= 0.0f) {
            if (end < 1.0f) {
                end = 1.0f;
            }
            start = 0.0f;
        }
#line 746
        ASSERT(end > start);
        ASSERT(start >= 0.0f);
        ((POINTLIGHTPACKED*)addr)->m_fX[idx] = l->pos.x;
        ((POINTLIGHTPACKED*)addr)->m_fY[idx] = l->pos.y;
        ((POINTLIGHTPACKED*)addr)->m_fZ[idx] = l->pos.z;
        ((POINTLIGHTPACKED*)addr)->m_fFar[idx] = end * end;
        ((POINTLIGHTPACKED*)addr)->m_vColor[idx].x = l->col.x;
        ((POINTLIGHTPACKED*)addr)->m_vColor[idx].y = l->col.y;
        ((POINTLIGHTPACKED*)addr)->m_vColor[idx].z = l->col.z;
        ((POINTLIGHTPACKED*)addr)->m_vColor[idx].w = 0.0f;
        ((POINTLIGHTPACKED*)addr)->m_fNear[idx] = start * start;
        ((POINTLIGHTPACKED*)addr)->m_fLength[idx] = 1.0f / (((POINTLIGHTPACKED*)addr)->m_fFar[idx] - ((POINTLIGHTPACKED*)addr)->m_fNear[idx]);
    } else {
        ((POINTLIGHTPACKED*)addr)->m_fX[idx] = 0.0f;
        ((POINTLIGHTPACKED*)addr)->m_fY[idx] = 0.0f;
        ((POINTLIGHTPACKED*)addr)->m_fZ[idx] = 0.0f;
        ((POINTLIGHTPACKED*)addr)->m_fFar[idx] = 0.0f;
        ((POINTLIGHTPACKED*)addr)->m_vColor[idx].x = 0.0f;
        ((POINTLIGHTPACKED*)addr)->m_vColor[idx].y = 0.0f;
        ((POINTLIGHTPACKED*)addr)->m_vColor[idx].z = 0.0f;
        ((POINTLIGHTPACKED*)addr)->m_vColor[idx].w = 0.0f;
        ((POINTLIGHTPACKED*)addr)->m_fNear[idx] = 0.0f;
        ((POINTLIGHTPACKED*)addr)->m_fLength[idx] = 0.0f;
    }
}

void CGimmickLightUnit::_calc() {
    _TMPTMP ll[4];
    int i;
    if (isCalc()) {
        return;
    }
    m_iFlags |= 1;
    calcCenter(m_pCrd->getWorldMatrix());
    for (i = 0; i < 4; i++) {
        ll[i].l = NULL;
        ll[i].pow = 0.0f;
    }
    LIGHTDATA* ld = getLightData();
    for (i = 0; i < ld->point_suu; i++) {
        POINTLIGHT* l = &ld->point[i];
        FLOAT pow = getLightVolume(ctr[0], ctr[1], l);
        int j;
        for (j = 0; j < 4; j++) {
            if (ll[j].pow < pow) {
                int k;
                for (k = 3; k >= j + 1; k--) {
                    ll[k] = ll[k - 1];
                }
                ll[j].pow = pow;
                ll[j].l = l;
                break;
            }
        }
    }
    CWorldEnv::initSeekActiveEffLight();
    CEffectLight* l;
    while ((l = CWorldEnv::seekNextActiveEffLight()) != NULL) {
        FLOAT pow = getLightVolume(ctr[0], ctr[1], l);
        int j;
        for (j = 0; j < 4; j++) {
            if (ll[j].pow < pow) {
                int k;
                for (k = 3; k >= j + 1; k--) {
                    ll[k] = ll[k - 1];
                }
                ll[j].pow = pow;
                ll[j].l = l;
                break;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        setLight(i, ll[i].l);
    }
    for (i = 0; i < 4; i++) {
        if (i < ld->infinite_suu) {
            CLightUnit::setLight(i, ld->infinite[i].dir, ld->infinite[i].col);
        } else {
            FVECTOR v;
            v._q = 0;
            CLightUnit::setLight(i, v, v);
        }
    }
    setAmbient(ld->ambient->col);
}

CEffectLightMan::CEffectLightMan() {
    m_nActiveLightInfo = 0;
    CEffectLight* p = m_LightTbl;
    do {
        p->linkManager(*this);
        p++;
    } while (p < &m_LightTbl[32]);
}

CEffectLight* CEffectLightMan::getBlankLight() {
    UINT n = 1;
    UINT nInfo = m_nActiveLightInfo;
    CEffectLight* pEL = m_LightTbl;
    INT i;
    for (i = 0; i < 32; i++, n <<= 1, pEL++) {
        if (!(nInfo & n)) {
            pEL->open(i);
            m_nActiveLightInfo |= n;
            return pEL;
        }
    }
    return NULL;
}

CEffectLight* CEffectLightMan::seekNextActiveLight() {
    CEffectLight* pLight = NULL;
    UINT nIndex = m_nLightIndex;
    UINT nFlag = m_nLightSeeker;
    UINT nInfo = m_nActiveLightInfo;
    UINT nMask = m_nLightSeekMask;
    while ((nInfo & nMask) && nIndex < 32 && pLight == NULL) {
        if (nInfo & nFlag) {
            pLight = &m_LightTbl[nIndex];
        }
        nMask <<= 1;
        nFlag <<= 1;
        nIndex++;
    }
    m_nLightIndex = nIndex;
    m_nLightSeeker = nFlag;
    m_nLightSeekMask = nMask;
    return pLight;
}

void CEffectLightMan::reset() {
    UINT n = 1;
    UINT nInfo = m_nActiveLightInfo;
    CEffectLight* pEL = m_LightTbl;
    INT i;
    for (i = 0; i < 32; n <<= 1, i++, pEL++) {
        if (nInfo & n) {
            if (!pEL->isClosed()) {
                pEL->close();
            }
        }
    }
    m_nActiveLightInfo = 0;
}
// SOURCE DATA: .data .rodata
