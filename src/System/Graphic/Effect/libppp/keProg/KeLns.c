#include "KeLns.h"
#include "System/Graphic/Effect/libppp/keProg/Ke.h"

void sceVu0SubVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2);

#define LNS_SEQ(shp, idx) (&((OANM_HEADER *)(shp)->shape)->seq[idx])

void KeLnsFlr_Init(KeLnsFlr *_p, pppShapeSt *pSS) {
    _p->Cnt = LNS_SEQ(pSS, 0)->wait;
    _p->SeqIdx = 0;
}

void KeLnsShp_Update(KeWkLnsShp_Draw *_w) {
    KeLnsFlr *pFlr;
    OANM_SEQ *seq;

    pFlr = _w->Flr;
    seq = LNS_SEQ(_w->Shp, pFlr->SeqIdx);
    pFlr->Cnt += 0x200;
    if (pFlr->Cnt >= seq->wait) {
        pFlr->Cnt -= seq->wait;
        pFlr->SeqIdx++;
        if (pFlr->SeqIdx >= ((OANM_HEADER *)_w->Shp->shape)->seqc) {
            if (seq->flag & 0x80) {
                pFlr->SeqIdx = 0;
                pFlr->Cnt = 0;
            } else {
                pFlr->Cnt = 0;
                pFlr->SeqIdx--;
            }
        }
    }
}

void KeLnsShp_Draw(KeWkLnsShp_Draw *_w) {
    OANM_SEQ *seq;

    seq = LNS_SEQ(_w->Shp, _w->Flr->SeqIdx);
    *(u_int *)&_w->In.toff = 0;
    _w->In.pri = 0;
    _w->In.ot = 0x12;
    _w->In.packet = ppvCurPrimp;
    _w->In.flag |= 0x30;
    ppvCurPrimp = op_shape_draw((OSHAPE_VU1 *)((char *)_w->Shp->shape + seq->sha), &_w->In, 0);
}

void KeLnsLp_Update(KeWkLnsLp_Update *_w) {
    sceVu0FMATRIX fm0;
    sceVu0FMATRIX fm1;
    sceVu0FVECTOR fv0;
    sceVu0FVECTOR fv1;
    sceVu0FVECTOR fv2;
    KeLnsLp *_p;
    float f;

    _p = _w->This;
    sceVu0CopyMatrix(fm1, *_w->WvMat);
    if (_p->Md) {
        sceVu0CopyMatrix(fm0, fm1);
        fm0[3][0] = 0;
        fm0[3][1] = 0;
        fm0[3][2] = 0;
        sceVu0ApplyMatrix(fv0, fm0, *_w->RPos);
        sceVu0ScaleVector(fv1, fm1[3], -1.0f);
        sceVu0SubVector(fv2, fv1, fv0);
    } else {
        *(u_long128 *)fv0 = 0;
        *(u_long128 *)fv1 = 0;
        fv0[3] = 1.0f;
        fv1[3] = 1.0f;
        sceVu0UnitMatrix(fm0);
        fm0[3][0] = ppvWorldMatrix[3][0] + _p->LcPos[0];
        fm0[3][1] = ppvWorldMatrix[3][1] + _p->LcPos[1];
        fm0[3][2] = ppvWorldMatrix[3][2] + _p->LcPos[2];
        sceVu0MulMatrix(fm0, ppvScreenMatrix, fm0);
        sceVu0ApplyMatrix(fv0, fm0, fv0);
        sceVu0ScaleVector(fv0, fv0, 1.0f / fv0[3]);
        sceVu0UnitMatrix(fm0);
        sceVu0CopyVector(fm0[3], ppvWorldMatrix[3]);
        sceVu0MulMatrix(fm0, ppvScreenMatrix, fm0);
        sceVu0ApplyMatrix(fv1, fm0, fv1);
        sceVu0ScaleVector(fv1, fv1, 1.0f / fv1[3]);
        sceVu0SubVector(fv2, fv1, fv0);
        fv2[1] *= 1.7142857f;
        fv2[2] = 512.0f;
    }
    sceVu0UnitMatrix(_p->Mat);
    sceVu0ApplyMatrix(_p->Mat[3], fm1, *_w->RPos);
    if (fv2[0] == 0 && fv2[1] == 0) {
        sceVu0CopyVector(_p->ZMat[3], _p->Mat[3]);
    } else {
        __asm__ __volatile__(
            "lqc2 $vf18, 0x0(%0)\n"
            "vmul.xy $vf26, $vf18, $vf18\n"
            "vaddy.x $vf26, $vf26, $vf26y\n"
            "vsqrt $Q, $vf26x\n"
            "vsub.zw $vf22, $vf0, $vf0\n"
            "vsub.w $vf21, $vf0, $vf0\n"
            "vsub.xyw $vf20, $vf0, $vf0\n"
            "vaddw.z $vf20, $vf0, $vf0w\n"
            "sqc2 $vf20, 0x20(%1)\n"
            "vwaitq\n"
            "vaddq.x $vf26, $vf0, $Q\n"
            "vdiv $Q, $vf0w, $vf26x\n"
            "lqc2 $vf17, 0x0(%2)\n"
            "sqc2 $vf17, 0x30(%1)\n"
            "vwaitq\n"
            "vmulq.xy $vf22, $vf18, $Q\n"
            "sqc2 $vf22, 0x0(%1)\n"
            "vopmula.xyz $ACC, $vf22, $vf20\n"
            "vopmsub.xyz $vf21, $vf20, $vf22\n"
            "sqc2 $vf21, 0x10(%1)\n"
            :
            : "r"(fv2), "r"(_p->ZMat), "r"(_p->Mat[3])
            : "memory");
    }
    if (_p->Md) {
        _p->Pers = _p->Mat[3][2] * (1.0f / 512.0f);
    } else {
        _p->Pers = (ppvWorldMatrix[3][2] + _p->LcPos[2]) * (1.0f / 512.0f);
    }
    sceVu0Normalize(fv2, fv2);
    _p->Vec[0] = fv2[0] * _p->Pers;
    _p->Vec[1] = fv2[1] * _p->Pers;
    sceVu0SubVector(fv1, fv1, fv0);
    f = KeVu0VecLen(fv1);
    _p->Len2 = KeVu0VecLenXY(fv1);
    sceVu0Normalize(fv0, fv1);
    if (_p->Md) {
        fv1[0] = 0;
        fv1[1] = 0;
        fv1[2] = 1.0f;
        fv1[0] = 1.0f - (1.0f - sceVu0InnerProduct(fv1, fv0)) * 0.5f;
        fv1[0] /= 0.3f;
        _p->VPow = 1.0f < fv1[0] ? 0.0f : 1.0f - fv1[0];
        if (0.0f <= _p->LVec[3]) {
            sceVu0CopyMatrix(fm0, fm1);
            fm0[3][2] = fm0[3][1] = fm0[3][0] = 0.0f;
            sceVu0ApplyMatrix(fv1, fm0, fv1);
            fv1[0] = (1.0f - sceVu0InnerProduct(_p->LVec, fv0)) * 0.5f;
            fv1[0] = fv1[0] / _p->LVec[3];
            fv1[0] = 1.0f < fv1[0] ? 0.0f : 1.0f - fv1[0];
            _p->VPow *= fv1[0];
        }
        fv0[0] = _p->Pow - _p->R;
        if (0.0f < fv0[0]) {
            fv0[0] = f / fv0[0];
            f = 1.0f <= fv0[0] ? 0.0f : 1.0f - fv0[0];
            _p->VPow *= f;
        } else {
            _p->VPow = 0.0f;
        }
    } else {
        if (0.0f < _p->Pow) {
            _p->VPow = _p->Len2 / _p->Pow;
            _p->VPow = 1.0f < _p->VPow ? 0.0f : 1.0f - _p->VPow;
        } else {
            _p->VPow = 0.0f;
        }
    }
}

void KeLnsLp_Init(KeLnsLp *_p) {
    ppvMng->bDrawPriority = 2;
    sceVu0UnitMatrix(_p->ZMat);
    _p->PreHidePow = 1.0f;
}

void KeLnsClm_Draw(KeWkLnsClm_Draw *_w, int Flg) {
    sceVu0FMATRIX fm0;
    sceVu0FMATRIX fm1;
    KeWkLnsShp_Draw wShp;
    KeLnsClmFlr *pCF;
    KeLnsClm *_p;
    KeLnsLp *Lp;
    float a;
    float b;
    float c;
    float d;
    float e;
    float f;
    int i;

    _p = _w->This;
    Lp = _p->Lp;
    if (Lp == (KeLnsLp *)ppvDbgTemp || _p->Shp == NULL || Lp->VPow == 0) {
        return;
    }
    if (Lp->Md) {
        sceVu0CopyMatrix(fm0, Lp->Mat);
    } else {
        sceVu0UnitMatrix(fm0);
        fm0[3][2] = ppvWorldMatrix[3][2] + Lp->LcPos[2];
    }
    *(u_int *)&wShp.In.r = *(u_int *)_w->Col;
    wShp.Shp = _p->Shp;
    wShp.In.flag = Flg;
    wShp.In.pmat = &fm1;
    wShp.In.pzmat = NULL;
    *(u_int *)&wShp.In.toff = 0;
    wShp.In.ot = 0;
    wShp.In.z = 0;
    if (Lp->Md) {
        a = b = 0.0f;
    } else {
        a = ppvWorldMatrix[3][0];
        b = ppvWorldMatrix[3][1];
    }
    f = Lp->Vec[0];
    a += f * _p->StOfs;
    c = f * _p->Betw;
    f = Lp->Vec[1];
    b += f * _p->StOfs;
    d = f * _p->Betw;
    f = Lp->Len2;
    f -= Lp->R;
    if (f <= 0.0f) {
        f = 0.0f;
    } else {
        f /= _w->R * Lp->Pers;
        if (1.0f < f) {
            f = 1.0f;
        }
    }
    if (_p->FlgType) {
        f = 1.0f - f;
    }
    f *= wShp.In.a * Lp->VPow;
    pCF = _p->Flr;
    for (i = _p->Num; i > 0; i--) {
        e = pCF->RdPr;
        wShp.Flr = &pCF->Flr;
        KeLnsShp_Update(&wShp);
        pCF++;
        a += c * e;
        b += d * e;
        fm0[3][0] = a;
        fm0[3][1] = b;
        wShp.In.a = f * (e * 0.75 + 0.25);
        e *= _w->Scl;
        fm0[0][0] = Lp->Mat[0][0] * e * (*_w->SclVec)[0];
        fm0[1][1] = Lp->Mat[1][1] * e * (*_w->SclVec)[1];
        sceVu0MulMatrix(fm1, *_w->VsMat, fm0);
        KeLnsShp_Draw(&wShp);
    }
}

void KeLnsClm_SetShp(KeLnsClm *_p, pppShapeSt *pSS, int seed) {
    int i;
    int r;

    _p->Shp = pSS;
    for (i = 0; i < _p->Num; i++) {
        KeLnsFlr_Init(&_p->Flr[i].Flr, pSS);
    }
    if (!(_p->Flg & 1)) {
        r = seed;
        for (i = 0; i < _p->Num; i++) {
            r = (r * 0x80D + 7) & 0xFFFF;
            _p->Flr[i].RdPr = r / 65535.0f;
        }
        _p->Flg |= 1;
    }
}

void KeLnsClm_Init(KeLnsClm *_p) {
    _p->Shp = NULL;
    _p->Flg = 0;
    _p->Lp = (KeLnsLp *)ppvDbgTemp;
}

void KeLnsArnd_Draw(KeWkLnsArnd_Draw *_w, int Flg) {
    sceVu0FMATRIX fm0;
    KeWkLnsShp_Draw wShp;
    KeLnsArnd *_p;
    KeLnsLp *Lp;

    _p = _w->This;
    Lp = _p->Lp;
    if (Lp == (KeLnsLp *)ppvDbgTemp || _p->Shp == NULL || Lp->VPow == 0) {
        return;
    }
    if (Lp->Md) {
        sceVu0CopyMatrix(fm0, Lp->Mat);
    } else {
        sceVu0UnitMatrix(fm0);
        fm0[3][0] = ppvWorldMatrix[3][0] + Lp->LcPos[0];
        fm0[3][1] = ppvWorldMatrix[3][1] + Lp->LcPos[1];
        fm0[3][2] = ppvWorldMatrix[3][2] + Lp->LcPos[2];
    }
    fm0[0][0] *= _w->Scl;
    fm0[1][1] *= _w->Scl;
    sceVu0ScaleVector(fm0[0], fm0[0], (*_w->SclVec)[0]);
    sceVu0ScaleVector(fm0[1], fm0[1], (*_w->SclVec)[1]);
    sceVu0ScaleVector(fm0[2], fm0[2], (*_w->SclVec)[2]);
    sceVu0MulMatrix(fm0, *_w->VsMat, fm0);
    *(u_int *)&wShp.In.r = *(u_int *)_w->Col;
    wShp.In.a = wShp.In.a * Lp->VPow;
    wShp.Shp = _p->Shp;
    wShp.Flr = &_p->Flr;
    KeLnsShp_Update(&wShp);
    wShp.In.flag = Flg;
    wShp.In.pmat = &fm0;
    wShp.In.pzmat = NULL;
    *(u_int *)&wShp.In.toff = 0;
    *(u_int *)&wShp.In.ot = 0;
    KeLnsShp_Draw(&wShp);
}

void KeLnsArnd_Init(KeLnsArnd *_p) {
    _p->Shp = NULL;
    _p->Lp = (KeLnsLp *)ppvDbgTemp;
}

void KeLnsCrn_Draw(KeWkLnsCrn_Draw *_w, int Flg) {
    sceVu0FMATRIX fm0;
    KeWkLnsShp_Draw wShp;
    KeLnsCrn *_p;
    KeLnsLp *Lp;
    float f;

    _p = _w->This;
    Lp = _p->Lp;
    if (Lp == (KeLnsLp *)ppvDbgTemp || _p->Shp == NULL || Lp->VPow == 0) {
        return;
    }
    sceVu0CopyVector(fm0[0], Lp->ZMat[0]);
    sceVu0CopyVector(fm0[1], Lp->ZMat[1]);
    sceVu0CopyVector(fm0[2], Lp->ZMat[2]);
    sceVu0CopyVector(fm0[3], Lp->Mat[3]);
    f = _w->Scl * Lp->Len2 / (_w->PLen * 0.01f);
    sceVu0ScaleVector(fm0[0], fm0[0], -f);
    sceVu0ScaleVector(fm0[1], fm0[1], f);
    if (!Lp->Md) {
        fm0[3][0] = ppvWorldMatrix[3][0] + Lp->LcPos[0];
        fm0[3][1] = ppvWorldMatrix[3][1] + Lp->LcPos[1];
        fm0[3][2] = ppvWorldMatrix[3][2] + Lp->LcPos[2];
    }
    sceVu0ScaleVector(fm0[0], fm0[0], (*_w->SclVec)[0]);
    sceVu0ScaleVector(fm0[1], fm0[1], (*_w->SclVec)[1]);
    sceVu0ScaleVector(fm0[2], fm0[2], (*_w->SclVec)[2]);
    sceVu0MulMatrix(fm0, *_w->VsMat, fm0);
    *(u_int *)&wShp.In.r = *(u_int *)_w->Col;
    f = Lp->Len2 / (_w->DLen * 0.01f);
    f = 1.0f < f ? 1.0f : f;
    wShp.In.a = wShp.In.a * Lp->VPow * f;
    wShp.Shp = _p->Shp;
    wShp.Flr = &_p->Flr;
    KeLnsShp_Update(&wShp);
    wShp.In.flag = Flg;
    wShp.In.pmat = &fm0;
    wShp.In.pzmat = NULL;
    *(u_int *)&wShp.In.toff = 0;
    wShp.In.ot = 0;
    wShp.In.z = 0;
    KeLnsShp_Draw(&wShp);
}

void KeLnsCrn_SetShp(KeLnsCrn *_p, pppShapeSt *pSS) {
    _p->Shp = pSS;
    KeLnsFlr_Init(&_p->Flr, pSS);
}

void KeLnsCrn_Init(KeLnsCrn *_p) {
    _p->Shp = NULL;
    _p->Lp = (KeLnsLp *)ppvDbgTemp;
}

void KeLnsFls_Draw(KeWkLnsFls_Draw *_w, int Flg) {
    sceVu0FMATRIX fm0;
    KeWkLnsShp_Draw wShp;
    KeLnsFls *_p;
    KeLnsLp *Lp;
    float f;
    float e;

    _p = _w->This;
    Lp = _p->Lp;
    if (Lp == (KeLnsLp *)ppvDbgTemp || _p->Shp == NULL || Lp->VPow == 0) {
        return;
    }
    f = 0.0f;
    e = Lp->Len2 / (_w->PLen * 0.01f);
    if (!(1.0f < e)) {
        f = 1.0f - e;
    }
    if (Lp->Md) {
        sceVu0CopyMatrix(fm0, Lp->Mat);
    } else {
        sceVu0UnitMatrix(fm0);
        fm0[3][0] = ppvWorldMatrix[3][0] + Lp->LcPos[0];
        fm0[3][1] = ppvWorldMatrix[3][1] + Lp->LcPos[1];
        fm0[3][2] = ppvWorldMatrix[3][2] + Lp->LcPos[2];
    }
    fm0[0][0] = _w->Scl;
    switch (_w->Typ[0]) {
    case 1:
        fm0[0][0] = _w->Scl * f;
        break;
    case 2:
        fm0[0][0] = e * _w->Scl + _w->Scl;
        break;
    }
    fm0[1][1] = _w->Scl;
    switch (_w->Typ[1]) {
    case 1:
        fm0[1][1] = _w->Scl * f;
        break;
    case 2:
        fm0[1][1] = e * _w->Scl + _w->Scl;
        break;
    }
    sceVu0ScaleVector(fm0[0], fm0[0], (*_w->SclVec)[0]);
    sceVu0ScaleVector(fm0[1], fm0[1], (*_w->SclVec)[1]);
    sceVu0MulMatrix(fm0, *_w->VsMat, fm0);
    *(u_int *)&wShp.In.r = *(u_int *)_w->Col;
    wShp.In.a = wShp.In.a * Lp->VPow * f;
    wShp.Shp = _p->Shp;
    wShp.Flr = &_p->Flr;
    KeLnsShp_Update(&wShp);
    wShp.In.flag = Flg;
    wShp.In.pmat = &fm0;
    wShp.In.pzmat = NULL;
    *(u_int *)&wShp.In.toff = 0;
    wShp.In.ot = 0;
    wShp.In.z = 0;
    KeLnsShp_Draw(&wShp);
}

void KeLnsFls_Init(KeLnsFls *_p) {
    _p->Shp = NULL;
    _p->Lp = (KeLnsLp *)ppvDbgTemp;
}
