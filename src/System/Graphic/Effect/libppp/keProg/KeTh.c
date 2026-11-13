#include "KeTh.h"
#include "System/Graphic/Effect/libppp/keProg/Ke.h"

typedef struct _sceDmaChan sceDmaChan;
sceDmaChan *sceDmaGetChan(int id);
void sceDmaSend(sceDmaChan *d, void *tag);
int sceGsSyncPath(int mode, u_short timeout);
void sceVu0FTOI0Vector(sceVu0IVECTOR v0, sceVu0FVECTOR v1);
void sceVu0ClampVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, float min, float max);
void sceVu0SubVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2);
void sceVu0InterVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2, float t);
void sceVu0RotMatrixX(sceVu0FMATRIX m0, sceVu0FMATRIX m1, float rx);
void sceVu0RotMatrixY(sceVu0FMATRIX m0, sceVu0FMATRIX m1, float ry);
void sceVu0RotMatrixZ(sceVu0FMATRIX m0, sceVu0FMATRIX m1, float rz);

static void (*ERotMatrix[3])(sceVu0FMATRIX m0, sceVu0FMATRIX m1, float r) = {
    sceVu0RotMatrixX,
    sceVu0RotMatrixY,
    sceVu0RotMatrixZ,
};

void KeThRP_Quit(KeThRP *_p, int flg) {
    KeThBP *pBP;

    flg |= KeThBP_FlgParentDel;
    for (pBP = (KeThBP *)_p->GrpList.FirstItem; pBP != NULL; pBP = (KeThBP *)pBP->GrpTag.NextItem) {
        pBP->GrpTag.PrevTag = NULL;
        pBP->Flg |= flg;
    }
}

#define KE_RP_NEXT() (vi._02_Rp = (KeThRP *)((char *)vi._02_Rp + vi._06_RpSz))
int KeThBP_MkGifPk(KeaThBp_MkGifPk *_a) {
    struct {
        sceVu0FMATRIX _01_04_Mat;
        sceVu0FVECTOR _05_ColCe;
        sceVu0FVECTOR _06_ColLt;
        float _07xy_Scl[2];
        float _07z_MaxCol;
        float _07w_Pstg;
        sceVu0FVECTOR _08_AddColCe;
        sceVu0FVECTOR _09_AddColLt;
        float _10xy_Clip[2];
        float _10z_WidSclSz;
        float _10w_NoKick;
        sceVu0FVECTOR _11_PrePos;
        sceVu0FVECTOR _12_CurPos;
        sceVu0FVECTOR _13_NxtPos;
        sceVu0FVECTOR _14_CurVec;
        sceVu0FVECTOR _15_PreEv;
        float _16x;
        float _16y_Nega;
        float _16z;
        float _16w_AddWid;
        KeData16 _17_Tag0;
        KeData16 _18_Tag1;
        sceVu0FVECTOR _19_PreWrk;
        sceVu0FVECTOR _20_CurWrk;
        float _21x_Wid;
        float _21y_CurPers;
        float _21z_NxtPers;
        float _21w_WidScl;
        KeData16 _22_Tag2;
        sceVu0FVECTOR _23;
        sceVu0FVECTOR _24;
        sceVu0FVECTOR _25;
        sceVu0FVECTOR _26;
        sceVu0FVECTOR _27;
        sceVu0FVECTOR _28;
        sceVu0FVECTOR _29;
        sceVu0FVECTOR _30;
        sceVu0FVECTOR _31;
    } vf;
    struct {
        KeData16 *_01_Pk;
        KeThRP *_02_Rp;
        int _03;
        int _04;
        int _05;
        int _06_RpSz;
        KeData16 *_07_PkTop;
        int _08_Cnt;
        sceGifTag *_09_lt;
        int _10_Clip;
        int _11_12_StpNum[2];
        KeThRP *_13_14_TopRp[2];
        int ClipFlg;
    } vi;
    /* The original's headers define 40 inline functions before this point (gcc numbers nested functions
       after them), so give the nested functions their original names. */
    auto void root(void) __asm__("root.126");
    auto void betw(void) __asm__("betw.130");
    auto void last(void) __asm__("last.134");
    auto void set1p(void) __asm__("set1p.138");
    auto void set2p(void) __asm__("set2p.142");

    void root() {
        vi._09_lt = &vi._01_Pk->gt;
        *vi._01_Pk = vf._22_Tag2;
        vi._01_Pk++;
        vf._21w_WidScl = 1.0f - vi._02_Rp->Scl * vf._10z_WidSclSz;
        sceVu0FTOI0Vector((int *)vf._31, vf._05_ColCe);
        KeVu0CopyVecXYZ(vf._26, vf._31);
        sceVu0CopyVector(vi._01_Pk->fv, vf._26);
        vi._01_Pk++;
        KeVu0PacVec(vi._01_Pk->iv, vf._11_PrePos);
        vi._01_Pk++;
        sceVu0FTOI0Vector((int *)vf._25, vf._06_ColLt);
        sceVu0AddVector(vf._31, vf._06_ColLt, vf._09_AddColLt);
        sceVu0ClampVector(vf._06_ColLt, vf._31, 0.0f, vf._07z_MaxCol);
        sceVu0FTOI0Vector((int *)vf._24, vf._06_ColLt);
        sceVu0CopyVector(vf._31, vi._02_Rp->RPos);
        KeVu0RTPersF(vf._13_NxtPos, (float **)vf._01_04_Mat, vf._31);
        vf._21z_NxtPers = 1.0f / vf._13_NxtPos[3];
        KE_RP_NEXT();
        KeVu0SubVecXY(vf._31, vf._13_NxtPos, vf._12_CurPos);
        KeVu0NormXY(vf._29, vf._31);
        KeVu0AddVecXY(vf._31, vf._14_CurVec, vf._29);
        KeVu0NormXY(vf._31, vf._31);
        vf._21x_Wid += vf._16w_AddWid;
        vf._21x_Wid = KeVu0LowClamp(vf._21x_Wid, 0.0f);
        vf._30[0] = vf._31[1] * vf._21x_Wid * vf._21y_CurPers * (vf._21w_WidScl * vf._07xy_Scl[0]);
        vf._30[1] = -vf._31[0] * vf._21x_Wid * vf._21y_CurPers * (vf._21w_WidScl * vf._07xy_Scl[1]);
        KeVu0CopyVecXY(vf._14_CurVec, vf._29);
        vf._21y_CurPers = vf._21z_NxtPers;
        KeVu0CopyVecXYZ(vf._26, vf._24);
        sceVu0CopyVector(vi._01_Pk->fv, vf._26);
        vi._01_Pk++;
        KeVu0PacAddVecXY(vi._01_Pk->iv, vf._12_CurPos, vf._30);
        vi._01_Pk++;
        sceVu0CopyVector(vf._15_PreEv, vf._30);
        sceVu0ScaleVector(vf._27, vf._30, vf._07w_Pstg);
        sceVu0CopyVector(vi._01_Pk->fv, vf._24);
        vi._01_Pk++;
        KeVu0PacAddVecXY(vi._01_Pk->iv, vf._12_CurPos, vf._27);
        vi._01_Pk++;
        sceVu0AddVector(vf._31, vf._05_ColCe, vf._08_AddColCe);
        sceVu0ClampVector(vf._05_ColCe, vf._31, 0.0f, 255.0f);
        sceVu0FTOI0Vector((int *)vf._29, vf._05_ColCe);
        sceVu0CopyVector(vi._01_Pk->fv, vf._29);
        vi._01_Pk++;
        KeVu0PacVec(vi._01_Pk->iv, vf._12_CurPos);
        vi._01_Pk++;
        sceVu0ScaleVector(vf._27, vf._30, vf._16y_Nega);
        sceVu0ScaleVector(vf._29, vf._27, vf._07w_Pstg);
        sceVu0CopyVector(vi._01_Pk->fv, vf._24);
        vi._01_Pk++;
        KeVu0PacAddVecXY(vi._01_Pk->iv, vf._12_CurPos, vf._29);
        vi._01_Pk++;
        KeVu0CopyVecXYZ(vf._26, vf._24);
        sceVu0CopyVector(vi._01_Pk->fv, vf._26);
        vi._01_Pk++;
        KeVu0PacAddVecXY(vi._01_Pk->iv, vf._12_CurPos, vf._27);
        vi._01_Pk++;
        sceVu0CopyVector(vf._11_PrePos, vf._12_CurPos);
        sceVu0CopyVector(vf._12_CurPos, vf._13_NxtPos);
        vi._08_Cnt--;
    }

    void betw() {
        for (; vi._08_Cnt > 0; vi._08_Cnt--) {
            vf._21w_WidScl = 1.0f - vi._02_Rp->Scl * vf._10z_WidSclSz;
            vi._09_lt = &vi._01_Pk->gt;
            *vi._01_Pk = vf._18_Tag1;
            vi._01_Pk++;
            sceVu0FTOI0Vector((int *)vf._25, vf._06_ColLt);
            sceVu0AddVector(vf._31, vf._06_ColLt, vf._09_AddColLt);
            sceVu0ClampVector(vf._06_ColLt, vf._31, 0.0f, vf._07z_MaxCol);
            sceVu0FTOI0Vector((int *)vf._24, vf._06_ColLt);
            sceVu0CopyVector(vf._31, vi._02_Rp->RPos);
            KeVu0RTPersF(vf._13_NxtPos, (float **)vf._01_04_Mat, vf._31);
            vf._21z_NxtPers = 1.0f / vf._13_NxtPos[3];
            KE_RP_NEXT();
            KeVu0SubVecXY(vf._31, vf._13_NxtPos, vf._12_CurPos);
            KeVu0NormXY(vf._29, vf._31);
            KeVu0AddVecXY(vf._31, vf._14_CurVec, vf._29);
            KeVu0NormXY(vf._31, vf._31);
            vf._21x_Wid += vf._16w_AddWid;
            vf._21x_Wid = KeVu0LowClamp(vf._21x_Wid, 0.0f);
            vf._30[0] = vf._31[1] * vf._21x_Wid * vf._21y_CurPers * (vf._21w_WidScl * vf._07xy_Scl[0]);
            vf._30[1] = -vf._31[0] * vf._21x_Wid * vf._21y_CurPers * (vf._21w_WidScl * vf._07xy_Scl[1]);
            KeVu0CopyVecXY(vf._14_CurVec, vf._29);
            vf._21y_CurPers = vf._21z_NxtPers;
            KeVu0CopyVecXYZ(vf._26, vf._25);
            sceVu0CopyVector(vi._01_Pk->fv, vf._26);
            vi._01_Pk++;
            KeVu0PacAddVecXY(vi._01_Pk->iv, vf._11_PrePos, vf._15_PreEv);
            vi._01_Pk++;
            KeVu0CopyVecXYZ(vf._26, vf._24);
            sceVu0CopyVector(vi._01_Pk->fv, vf._26);
            vi._01_Pk++;
            KeVu0PacAddVecXY(vi._01_Pk->iv, vf._12_CurPos, vf._30);
            vi._01_Pk++;
            sceVu0ScaleVector(vf._29, vf._15_PreEv, vf._07w_Pstg);
            sceVu0ScaleVector(vf._28, vf._15_PreEv, vf._16y_Nega);
            sceVu0CopyVector(vf._15_PreEv, vf._30);
            sceVu0ScaleVector(vf._27, vf._30, vf._07w_Pstg);
            sceVu0CopyVector(vi._01_Pk->fv, vf._25);
            vi._01_Pk++;
            KeVu0PacAddVecXY(vi._01_Pk->iv, vf._11_PrePos, vf._29);
            vi._01_Pk++;
            sceVu0CopyVector(vi._01_Pk->fv, vf._24);
            vi._01_Pk++;
            KeVu0PacAddVecXY(vi._01_Pk->iv, vf._12_CurPos, vf._27);
            vi._01_Pk++;
            sceVu0FTOI0Vector((int *)vf._27, vf._05_ColCe);
            sceVu0AddVector(vf._31, vf._05_ColCe, vf._08_AddColCe);
            sceVu0ClampVector(vf._05_ColCe, vf._31, 0.0f, 255.0f);
            sceVu0FTOI0Vector((int *)vf._29, vf._05_ColCe);
            sceVu0CopyVector(vi._01_Pk->fv, vf._27);
            vi._01_Pk++;
            KeVu0PacVec(vi._01_Pk->iv, vf._11_PrePos);
            vi._01_Pk++;
            sceVu0CopyVector(vi._01_Pk->fv, vf._29);
            vi._01_Pk++;
            KeVu0PacVec(vi._01_Pk->iv, vf._12_CurPos);
            vi._01_Pk++;
            sceVu0ScaleVector(vf._27, vf._30, vf._16y_Nega);
            sceVu0ScaleVector(vf._30, vf._28, vf._07w_Pstg);
            sceVu0ScaleVector(vf._29, vf._27, vf._07w_Pstg);
            sceVu0CopyVector(vi._01_Pk->fv, vf._25);
            vi._01_Pk++;
            KeVu0PacAddVecXY(vi._01_Pk->iv, vf._11_PrePos, vf._30);
            vi._01_Pk++;
            sceVu0CopyVector(vi._01_Pk->fv, vf._24);
            vi._01_Pk++;
            KeVu0PacAddVecXY(vi._01_Pk->iv, vf._12_CurPos, vf._29);
            vi._01_Pk++;
            KeVu0CopyVecXYZ(vf._26, vf._25);
            sceVu0CopyVector(vi._01_Pk->fv, vf._26);
            vi._01_Pk++;
            KeVu0PacAddVecXY(vi._01_Pk->iv, vf._11_PrePos, vf._28);
            vi._01_Pk++;
            KeVu0CopyVecXYZ(vf._26, vf._24);
            sceVu0CopyVector(vi._01_Pk->fv, vf._26);
            vi._01_Pk++;
            KeVu0PacAddVecXY(vi._01_Pk->iv, vf._12_CurPos, vf._27);
            vi._01_Pk++;
            sceVu0CopyVector(vf._11_PrePos, vf._12_CurPos);
            sceVu0CopyVector(vf._12_CurPos, vf._13_NxtPos);
        }
    }

    void last() {
        vi._09_lt = &vi._01_Pk->gt;
        *vi._01_Pk = vf._22_Tag2;
        vi._01_Pk++;
        sceVu0FTOI0Vector((int *)vf._27, vf._05_ColCe);
        sceVu0AddVector(vf._31, vf._05_ColCe, vf._08_AddColCe);
        sceVu0ClampVector(vf._05_ColCe, vf._31, 0.0f, 255.0f);
        sceVu0FTOI0Vector((int *)vf._29, vf._05_ColCe);
        KeVu0CopyVecXYZ(vf._31, vf._29);
        sceVu0CopyVector(vi._01_Pk->fv, vf._31);
        vi._01_Pk++;
        KeVu0PacVec(vi._01_Pk->iv, vf._12_CurPos);
        vi._01_Pk++;
        sceVu0FTOI0Vector((int *)vf._25, vf._06_ColLt);
        KeVu0CopyVecXYZ(vf._26, vf._25);
        sceVu0CopyVector(vi._01_Pk->fv, vf._26);
        vi._01_Pk++;
        KeVu0PacAddVecXY(vi._01_Pk->iv, vf._11_PrePos, vf._15_PreEv);
        vi._01_Pk++;
        sceVu0ScaleVector(vf._29, vf._15_PreEv, vf._07w_Pstg);
        sceVu0ScaleVector(vf._28, vf._15_PreEv, vf._16y_Nega);
        sceVu0CopyVector(vi._01_Pk->fv, vf._25);
        vi._01_Pk++;
        KeVu0PacAddVecXY(vi._01_Pk->iv, vf._11_PrePos, vf._29);
        vi._01_Pk++;
        sceVu0CopyVector(vi._01_Pk->fv, vf._27);
        vi._01_Pk++;
        KeVu0PacVec(vi._01_Pk->iv, vf._11_PrePos);
        vi._01_Pk++;
        sceVu0ScaleVector(vf._30, vf._28, vf._07w_Pstg);
        sceVu0FTOI0Vector((int *)vf._25, vf._06_ColLt);
        sceVu0CopyVector(vi._01_Pk->fv, vf._25);
        vi._01_Pk++;
        KeVu0PacAddVecXY(vi._01_Pk->iv, vf._11_PrePos, vf._30);
        vi._01_Pk++;
        KeVu0CopyVecXYZ(vf._26, vf._25);
        sceVu0CopyVector(vi._01_Pk->fv, vf._26);
        vi._01_Pk++;
        KeVu0PacAddVecXY(vi._01_Pk->iv, vf._11_PrePos, vf._28);
        vi._01_Pk++;
        _a->LTag = vi._09_lt;
        _a->GifPkBuf = &vi._01_Pk->u16;
    }

    void set1p() {
        sceVu0CopyVector(vf._31, vi._02_Rp->RPos);
        KeVu0RTPersF(vf._11_PrePos, (float **)vf._01_04_Mat, vf._31);
        KE_RP_NEXT();
        vi._08_Cnt--;
    }

    void set2p() {
        sceVu0CopyVector(vf._31, vi._02_Rp->RPos);
        KeVu0RTPersF(vf._12_CurPos, (float **)vf._01_04_Mat, vf._31);
        vf._21y_CurPers = 1.0f / vf._12_CurPos[3];
        sceVu0SubVector(vf._31, vf._12_CurPos, vf._11_PrePos);
        KeVu0NormXY(vf._14_CurVec, vf._31);
        KE_RP_NEXT();
        vi._08_Cnt--;
    }

    if (_a->This->FromRoot + _a->This->FromZero < 3) {
        return 0;
    }
    vi._01_Pk = vi._07_PkTop = (KeData16 *)_a->GifPkBuf;
    vi._06_RpSz = sizeof(KeThRP);
    sceVu0CopyMatrix(vf._01_04_Mat, *_a->Mat);
    vf._05_ColCe[0] = _a->Pr->ColCe[0] * ppvMng->parCol[0];
    vf._05_ColCe[1] = _a->Pr->ColCe[1] * ppvMng->parCol[1];
    vf._05_ColCe[2] = _a->Pr->ColCe[2] * ppvMng->parCol[2];
    vf._05_ColCe[3] = _a->Pr->ColCe[3] * ppvMng->parCol[3];
    vf._06_ColLt[0] = _a->Pr->ColLt[0] * ppvMng->parCol[0];
    vf._06_ColLt[1] = _a->Pr->ColLt[1] * ppvMng->parCol[1];
    vf._06_ColLt[2] = _a->Pr->ColLt[2] * ppvMng->parCol[2];
    vf._06_ColLt[3] = _a->Pr->ColLt[3] * ppvMng->parCol[3];
    vf._08_AddColCe[0] = _a->Pr->AddColCe[0] * ppvMng->parCol[0];
    vf._08_AddColCe[1] = _a->Pr->AddColCe[1] * ppvMng->parCol[1];
    vf._08_AddColCe[2] = _a->Pr->AddColCe[2] * ppvMng->parCol[2];
    vf._08_AddColCe[3] = _a->Pr->AddColCe[3] * ppvMng->parCol[3];
    vf._09_AddColLt[0] = _a->Pr->AddColLt[0] * ppvMng->parCol[0];
    vf._09_AddColLt[1] = _a->Pr->AddColLt[1] * ppvMng->parCol[1];
    vf._09_AddColLt[2] = _a->Pr->AddColLt[2] * ppvMng->parCol[2];
    vf._09_AddColLt[3] = _a->Pr->AddColLt[3] * ppvMng->parCol[3];
    vf._16y_Nega = -1.0f;
    vf._21x_Wid = _a->Pr->Wid;
    vf._10z_WidSclSz = _a->Pr->MvWidSz;
    vf._07w_Pstg = _a->Pr->CePstg;
    vf._16w_AddWid = _a->Pr->AddWid;
    vf._07z_MaxCol = 255.0f;
    *(int *)&vf._26[3] = 0;
    vf._10xy_Clip[0] = 4096.0f;
    vf._10xy_Clip[1] = 4096.0f;
    *(int *)&vf._10w_NoKick = 0x8000;
    vf._07xy_Scl[0] = _a->Scl[0];
    vf._07xy_Scl[1] = _a->Scl[1];
    vf._17_Tag0.u8[0] = 0x2000000000000001;
    vf._17_Tag0.u8[1] = 0xEE;
    vf._18_Tag1.u8[0] = 0x202640000000000A;
    vf._22_Tag2.u8[0] = 0x2026C00000000006;
    vf._18_Tag1.u8[1] = 0x51;
    vf._22_Tag2.u8[1] = 0x51;
    if (!(_a->This->Flg & KeThBP_SttRvrs)) {
        vf._30[0] = _a->This->OrgStep;
        sceVu0ScaleVector(vf._31, vf._08_AddColCe, vf._30[0]);
        sceVu0AddVector(vf._31, vf._05_ColCe, vf._31);
        sceVu0ClampVector(vf._05_ColCe, vf._31, 0.0f, vf._07z_MaxCol);
        sceVu0ScaleVector(vf._31, vf._09_AddColLt, vf._30[0]);
        sceVu0AddVector(vf._31, vf._06_ColLt, vf._31);
        sceVu0ClampVector(vf._06_ColLt, vf._31, 0.0f, vf._07z_MaxCol);
        vf._31[0] = vf._21x_Wid + vf._16w_AddWid * vf._30[0];
        if (vf._31[0] < 0.0f) {
            vf._21x_Wid = 0.0f;
        } else {
            vf._21x_Wid = vf._31[0];
        }
        vi._11_12_StpNum[0] = _a->This->FromRoot;
        vi._11_12_StpNum[1] = _a->This->FromZero;
        vi._13_14_TopRp[0] = &_a->This->RpTbl[_a->This->RootIdx];
        vi._13_14_TopRp[1] = _a->This->RpTbl;
    } else {
        vi._11_12_StpNum[0] = _a->This->FromZero;
        vi._11_12_StpNum[1] = _a->This->FromRoot;
        vi._13_14_TopRp[0] = &_a->This->RpTbl[_a->This->FromZero] - 1;
        vi._13_14_TopRp[1] = &_a->This->RpTbl[_a->This->RootIdx] + _a->This->FromRoot - 1;
        vi._06_RpSz = -vi._06_RpSz;
    }
    vi._09_lt = &vi._01_Pk->gt;
    *(u_long128 *)vi._01_Pk = vf._17_Tag0.u16;
    ((u_long *)++vi._01_Pk)[0] = _a->Test;
    ((u_long *)vi._01_Pk)[1] = 0x47;
    vi._01_Pk++;
    ((u_long *)vi._01_Pk)[0] = _a->Alpha;
    ((u_long *)vi._01_Pk)[1] = 0x42;
    vi._01_Pk++;

    if (vi._11_12_StpNum[0] == 0) {
        goto seg1;
    }
    vi._08_Cnt = vi._11_12_StpNum[0];
    vi._02_Rp = vi._13_14_TopRp[0];
    set1p();
    if (vi._08_Cnt == 0) {
        vi._02_Rp = vi._13_14_TopRp[1];
        vi._08_Cnt = vi._11_12_StpNum[1];
        goto p2;
    }
    set2p();
    if (vi._08_Cnt == 0) {
        vi._02_Rp = vi._13_14_TopRp[1];
        vi._08_Cnt = vi._11_12_StpNum[1];
        goto rt;
    }
    root();
    vi._11_12_StpNum[0]--;
    betw();
    if (vi._11_12_StpNum[1] == 0) {
        goto lst;
    }
    vi._08_Cnt = vi._11_12_StpNum[1];
    vi._02_Rp = vi._13_14_TopRp[1];
    goto btw;
seg1:
    vi._02_Rp = vi._13_14_TopRp[1];
    vi._08_Cnt = vi._11_12_StpNum[1];
    set1p();
p2:
    set2p();
rt:
    root();
btw:
    betw();
lst:
    last();
    return 1;
}

/* Shape from the original's line markers and .mdebug block tree: the pTp == NULL and rootStep >= preStep tests
   are two ifs (jump2 cross-jumps their bodies), pTp->Nxt is tested before it is assigned, the UdtTag insertion
   has its own block with an `it` the optimizer removes (an empty block in the .mdebug). */
void KeThBP_Update(KeaThBp_Update *_w) {
    struct {
        sceVu0FMATRIX _01_04_Mat;
        sceVu0FVECTOR _05;
        sceVu0FVECTOR _06;
        sceVu0FVECTOR _07;
        sceVu0FVECTOR _08;
        float _09x_Spd1st;
        float _09y_Dist;
        float _09z_Pow;
        float _09w_Corner;
        float _10x_ValidR;
        float _10y_HitR;
        float _10z_TPow;
        float _10w_Type;
        sceVu0FVECTOR _11;
        sceVu0FVECTOR _12;
        sceVu0FVECTOR _31;
    } vf;
    KeThBP *_p;
    int flg;
    KeThBP *pBp;
    KeThRP *rpTbl;
    KeThRP *pRP;
    KeThRP *pPreRP;
    KeThRP *pRpW;
    KeThTP *pTp;
    float step;
    int rootStep;
    int preStep;
    int nNewStep;
    int nRootStep;
    int i;
    int j;
    int r;
    int a;
    int rn;
    u_int lwPstg;
    int lwf;
    int cmdSpdFix;
    float f;

    _p = _w->This;
    flg = _p->Flg;
    if (flg & KeThBP_SttHit) {
        flg |= KeThBP_SttHitted;
        flg &= ~KeThBP_SttHit;
    }
    if (flg & KeThBP_FlgParentDel) {
        if (!(flg & KeThBP_FlgRootStep)) {
            flg |= KeThBP_FlgReqDel;
            _p->Flg = flg;
            return;
        }
        flg &= ~KeThBP_FlgParentDel;
        flg |= KeThBP_FlgNoParent;
    }
    step = _p->Step;
    rootStep = _p->RootStep;
    preStep = step;
    rn = _p->RPNum;
    lwPstg = _w->Pr->Pstg;
    rpTbl = _p->RpTbl;
    lwf = 0;
    if (lwPstg != 0xFF) {
        lwf = _p->LowNum < _w->Pr->LowMax;
    }
    cmdSpdFix = _p->Flg & KeThBP_SttSpdFix;
    i = (int)(step + _w->Pr->StepSpd) - preStep;
    nRootStep = i;
    if (!(flg & KeThBP_FlgStepStop)) {
        step += _w->Pr->StepSpd;
        if (!(flg & KeThBP_FlgStepLimit)) {
            if ((int)step >= _w->Pr->MaxStep) {
                if (flg & KeThBP_SttNonStop) {
                    flg |= KeThBP_FlgRootStep;
                }
                if (!(flg & KeThBP_FlgRootStep)) {
                    nRootStep = (int)step - _w->Pr->MaxStep;
                    step = _w->Pr->MaxStep;
                    i -= nRootStep;
                } else {
                    flg |= KeThBP_FlgStepLimit;
                    flg &= ~KeThBP_SttCpSync;
                    nRootStep = (int)step - _w->Pr->MaxStep;
                }
            }
        }
        nNewStep = i;
    } else {
        nNewStep = 0;
    }
    r = flg & KeThBP_FlgRootStep;
    if (!r) {
        nRootStep = 0;
        if (flg & KeThBP_FlgNoParent) {
            pTp = _w->TP;
            _p->TpAdr = pTp;
        } else {
            pTp = _p->TpAdr;
        }
    } else {
        rootStep += nRootStep;
        pTp = _p->TpAdr;
        if (pTp == NULL) {
            _p->Flg |= KeThBP_FlgReqDel;
            return;
        }
        if (rootStep >= preStep) {
            _p->Flg |= KeThBP_FlgReqDel;
            return;
        }
    }
    j = _p->RootIdx;
    pRP = &rpTbl[j];
    for (i = nRootStep; i > 0; i--) {
        KeThRP_Quit(pRP, r);
        if (++j == rn) {
            j = 0;
            pRP = rpTbl;
        } else {
            pRP++;
        }
    }
    _p->RootIdx = j;
    _p->OrgStep += nRootStep;
    vf._09x_Spd1st = _w->Pr->Spd1st;
    vf._09w_Corner = _w->Pr->Corner;
    vf._10x_ValidR = pTp->ValidR;
    vf._10y_HitR = pTp->HitR;
    vf._10z_TPow = pTp->Pow;
    vf._10w_Type = pTp->RPos[3];
    sceVu0CopyVector(vf._08, pTp->RPos);
    KeVu0CopyVecXYZ(vf._06, pRP->InVec);
    vf._06[3] = 0.0f;
    sceVu0CopyVector(vf._07, pRP->RPos);
    a = preStep - rootStep + nNewStep;
    i = 0;
    if ((flg & (KeThBP_FlgRootStep | KeThBP_FlgNoParent)) == KeThBP_FlgNoParent) {
        sceVu0SubVector(vf._05, vf._08, vf._07);
        vf._09y_Dist = KeVu0VecLen(vf._05);
        vf._09z_Pow = vf._09w_Corner;
        if (vf._09y_Dist < vf._10x_ValidR) {
            vf._09z_Pow = vf._09w_Corner + (vf._10z_TPow - vf._10z_TPow * (vf._09y_Dist - vf._10y_HitR) / (vf._10x_ValidR - vf._10y_HitR));
        }
        sceVu0Normalize(vf._05, vf._05);
        sceVu0InterVector(vf._06, vf._05, vf._06, vf._09z_Pow);
        if (cmdSpdFix) {
            sceVu0Normalize(vf._06, vf._06);
        }
        r = KeVu0RndI();
        ERotMatrix[r % 3](vf._01_04_Mat, _w->UnitMat, _w->Pr->Curv * (float)(r % 1001 - 500) / 500.0f);
        sceVu0ApplyMatrix(vf._06, vf._01_04_Mat, vf._06);
        sceVu0InterVector(vf._06, vf._06, pRP->InVec, _w->Pr->Wild);
        if (cmdSpdFix) {
            sceVu0Normalize(vf._06, vf._06);
        }
    }
    while (1) {
        sceVu0SubVector(vf._05, vf._08, vf._07);
        vf._09y_Dist = KeVu0VecLen(vf._05);
        if ((vf._10w_Type != 0.0f && pTp->HitR < vf._09y_Dist) || (vf._10w_Type == 0.0f && vf._09y_Dist < pTp->HitR)) {
            if (pTp->Nxt == NULL) {
                flg |= KeThBP_SttHit;
                step = rootStep + i;
                _p->LastIdx = j;
                for (i++; i <= preStep - rootStep; i++) {
                    if (++j == rn) {
                        j = 0;
                        pRP = rpTbl;
                    } else {
                        pRP++;
                    }
                    KeThRP_Quit(pRP, flg & KeThBP_FlgRootStep);
                }
                if (flg & KeThBP_SttNonStop) {
                    flg |= KeThBP_FlgRootStep;
                }
                goto end;
            }
            pTp = pTp->Nxt;
            sceVu0CopyVector(vf._08, pTp->RPos);
            if (i == 0) {
                _p->TpAdr = pTp;
            }
            vf._10x_ValidR = pTp->ValidR;
            vf._10y_HitR = pTp->HitR;
            vf._10z_TPow = pTp->Pow;
            vf._10w_Type = pTp->RPos[3];
        }
        if (++i > a) {
            _p->LastIdx = j;
            goto end;
        }
        pPreRP = pRP;
        if (++j == rn) {
            j = 0;
            pRP = rpTbl;
        } else {
            pRP++;
        }
        sceVu0CopyVector(vf._12, vf._07);
        KeVu0CopyVecXYZ(vf._11, vf._06);
        sceVu0ScaleVector(vf._31, vf._06, vf._09x_Spd1st);
        sceVu0AddVector(vf._07, vf._07, vf._31);
        KeVu0CopyVecXYZ(pRP->RPos, vf._07);
        vf._09z_Pow = vf._09w_Corner;
        if (vf._09y_Dist < vf._10x_ValidR) {
            vf._09z_Pow = vf._09w_Corner + (vf._10z_TPow - vf._10z_TPow * (vf._09y_Dist - vf._10y_HitR) / (vf._10x_ValidR - vf._10y_HitR));
        }
        sceVu0Normalize(vf._05, vf._05);
        sceVu0InterVector(vf._06, vf._05, vf._06, vf._09z_Pow);
        if (cmdSpdFix) {
            sceVu0Normalize(vf._06, vf._06);
        }
        r = KeVu0RndI();
        ERotMatrix[r % 3](vf._01_04_Mat, _w->UnitMat, _w->Pr->Curv * (float)(r % 1001 - 500) / 500.0f);
        sceVu0ApplyMatrix(vf._06, vf._01_04_Mat, vf._06);
        if (i <= preStep - rootStep) {
            sceVu0InterVector(vf._06, vf._06, pRP->InVec, _w->Pr->Wild);
            if (cmdSpdFix) {
                sceVu0Normalize(vf._06, vf._06);
            }
            f = ((float)r * (1.0f / 2147483648.0f) * 2.0f + -1.0f) * _w->Pr->MvWidMv;
            f += pPreRP->Scl;
            if (1.0f < f) {
                f = 1.0f;
            } else if (f < 0.0f) {
                f = 0.0f;
            }
            pRP->Scl += (f - pRP->Scl) * _w->Pr->Wild;
            for (pBp = (KeThBP *)pRP->GrpList.FirstItem; pBp != NULL; pBp = (KeThBP *)pBp->GrpTag.NextItem) {
                pRpW = &pBp->RpTbl[pBp->RootIdx];
                KeVu0CopyVecXYZ(pRpW->RPos, vf._12);
                KeVu0CopyVecXYZ(pRpW->InVec, vf._11);
                pRpW->Scl = pRP->Scl;
                pBp->TpAdr = pTp;
            }
        } else {
            pRP->GrpList.FirstItem = NULL;
            pRP->RPos[3] = 1.0f;
            f = ((float)r * (1.0f / 2147483648.0f) * 2.0f + -1.0f) * _w->Pr->MvWidMv;
            f += pPreRP->Scl;
            if (1.0f < f) {
                f = 1.0f;
            } else if (f < 0.0f) {
                f = 0.0f;
            }
            pRP->Scl = f;
            if (lwf && !(r & lwPstg)) {
                KeThBP *pBP;

                pBP = KeThResHd_BPAlloc(_w->PkHd);
                if (pBP != NULL) {
                    KeWrkThBP_Init wrk;

                    _p->LowNum++;
                    wrk.This = pBP;
                    wrk.RPNum = _w->PkHd->RPNum;
                    KeThBP_Init(&wrk);
                    pBP->Flg |= _p->Flg & (KeThBP_SttSpdFix | KeThBP_SttRvrs);
                    pBP->TpAdr = pTp;
                    pBP->LowNum = _p->LowNum;
                    pBP->OrgStep = _p->OrgStep + preStep + i - (preStep - rootStep);
                    KeVu0CopyVecXYZ(pBP->RpTbl[0].RPos, vf._12);
                    KeVu0CopyVecXYZ(pBP->RpTbl[0].InVec, vf._11);
                    pBP->RpTbl[0].Scl = pPreRP->Scl;
                    {
                        KeListTag *it = &pBP->UdtTag;
                    
                        if (_p->UdtTag.NextItem != NULL) {
                            ((KeThBP *)_p->UdtTag.NextItem)->UdtTag.PrevTag = it;
                        }
                        it->NextItem = _p->UdtTag.NextItem;
                        it->PrevTag = &_p->UdtTag;
                        _p->UdtTag.NextItem = pBP;
                    }
                    {
                        KeListTag *it = &pBP->GrpTag;
                    
                        if (pRP->GrpList.FirstItem != NULL) {
                            ((KeThBP *)pRP->GrpList.FirstItem)->GrpTag.PrevTag = it;
                        }
                        it->NextItem = pRP->GrpList.FirstItem;
                        it->PrevTag = (KeListTag *)&pRP->GrpList;
                        pRP->GrpList.FirstItem = pBP;
                    }
                } else {
                    lwf = 0;
                }
                if (_p->LowNum >= _w->Pr->LowMax) {
                    lwf = 0;
                }
            }
        }
        KeVu0CopyVecXYZ(pRP->InVec, vf._06);
    }
end:
    i = _p->RootIdx;
    j = _p->LastIdx;
    if (i < j) {
        _p->FromZero = 0;
        _p->FromRoot = j - i + 1;
    } else if (j < i) {
        _p->FromZero = j + 1;
        _p->FromRoot = _p->RPNum - i;
    } else {
        _p->FromRoot = 0;
        _p->FromZero = 0;
    }
    _p->Step = step;
    _p->RootStep = rootStep;
    _p->Flg = flg;
}

void KeThBP_Init(KeWrkThBP_Init *_w) {
    KeThBP *_p = _w->This;
    KeData16 *pV;

    _p->Flg = 0;
    _p->This = _p;
    _p->RPNum = _w->RPNum;
    _p->RpTbl[0].RPos[3] = 1.0f;
    _p->Send.u1[3] = 0x70;
    _p->Send.u4[2] = 0x40000B0;
    _p->Send.u2[0] = (_p->RPNum * sizeof(KeThRP) >> 4) + 2;
    _p->RootStep = 0;
    _p->Step = 0.0f;
    _p->RootIdx = 0;
    _p->LastIdx = 0;
    _p->FromZero = 0;
    _p->FromRoot = 0;
    _p->LowNum = 0;
    _p->TpAdr = NULL;
    _p->UdtTag.PrevTag = NULL;
    _p->UdtTag.NextItem = NULL;
    _p->GrpTag.PrevTag = NULL;
    _p->GrpTag.NextItem = NULL;
    _p->RpTbl[0].GrpList.FirstItem = NULL;
    pV = (KeData16 *)&_p->RpTbl[_p->RPNum];
    pV->u4[0] = 0;
    pV->u4[1] = 0x11000000;
    pV->u4[2] = 0x14000000;
    pV->u4[3] = 0x11000000;
}

void KeThBP_Quit(KeThBP *_p) {
    KeThRP *pRp;
    int i;
    int flg;

    pRp = _p->RpTbl;
    flg = _p->Flg & KeThBP_FlgRootStep;
    for (i = _p->FromZero; i > 0; i--, pRp++) {
        KeThRP_Quit(pRp, flg);
    }
    pRp = &_p->RpTbl[_p->RootIdx];
    for (i = _p->FromRoot; i > 0; i--, pRp++) {
        KeThRP_Quit(pRp, flg);
    }
    if (_p->GrpTag.PrevTag != NULL) {
        if (_p->GrpTag.NextItem != NULL) {
            ((KeThBP *)_p->GrpTag.NextItem)->GrpTag.PrevTag = _p->GrpTag.PrevTag;
        }
        _p->GrpTag.PrevTag->NextItem = _p->GrpTag.NextItem;
    }
}

KeThBP *KeThResHd_BPAlloc(KeThResHd *_t) {
    KeThBP *pBP = (KeThBP *)_t->FreeAlow.FirstItem;

    if (pBP == NULL) {
        return NULL;
    }
    _t->FreeAlow.FirstItem = pBP->UdtTag.NextItem;
    return pBP;
}

void KeThResHd_Init(KeThResHd *_t, u_char bpNum, u_char rpNum) {
    KeThBP *pBP;
    KeThBP *pPreBP;
    u_short sz;
    int i;

    _t->BPNum = bpNum;
    _t->RPNum = rpNum;
    sz = (u_char)(rpNum - 1) * sizeof(KeThRP) + sizeof(KeThBP) + 0x10;
    pBP = _t->BP;
    _t->FreeAlow.FirstItem = pBP;
    _t->Sz = sz;
    for (i = bpNum - 1; i > 0; i--) {
        pPreBP = pBP;
        pBP = (KeThBP *)((char *)pBP + sz);
        pPreBP->UdtTag.NextItem = pBP;
    }
    pBP->UdtTag.NextItem = NULL;
}

void KeThCp_Reset(KeThCp *_t, KeThResHd *res) {
    KeThBP *bp;
    KeThBP *bpw;

    bp = (KeThBP *)_t->UdtList.FirstItem;
    while (bp != NULL) {
        bpw = bp;
        bp = (KeThBP *)bp->UdtTag.NextItem;
        KeThBP_Quit(bpw);
        bpw->UdtTag.NextItem = res->FreeAlow.FirstItem;
        res->FreeAlow.FirstItem = bpw;
    }
    _t->UdtList.FirstItem = NULL;
}

void KeThCp_AddBp(KeThCp *_t, KeThResHd *pResHd, sceVu0FVECTOR *pRPos, sceVu0FVECTOR *pVec) {
    KeThBP *pBp;
    KeWrkThBP_Init aBp;
    int f;

    pBp = KeThResHd_BPAlloc(pResHd);
    if (pBp != NULL) {
        aBp.This = pBp;
        aBp.RPNum = pResHd->RPNum;
        f = KeThBP_FlgNoParent;
        KeThBP_Init(&aBp);
        pBp->RpTbl[0].Scl = 0.0f;
        pBp->OrgStep = 0.0f;
        if (_t->Stt & KeThCp_SttNonStop) {
            f |= KeThBP_SttNonStop;
        }
        if (_t->Stt & KeThCp_SttCpSync) {
            f |= KeThBP_SttCpSync;
        }
        if (_t->Stt & KeThCp_SttSpdFix) {
            f |= KeThBP_SttSpdFix;
        }
        if (_t->Stt & KeThCp_SttRvrs) {
            f |= KeThBP_SttRvrs;
        }
        pBp->Flg |= f;
        KeVu0CopyVecXYZ(pBp->RpTbl[0].RPos, *pRPos);
        KeVu0CopyVecXYZ(pBp->RpTbl[0].InVec, *pVec);
        if (_t->UdtList.FirstItem != NULL) {
            ((KeThBP *)_t->UdtList.FirstItem)->UdtTag.PrevTag = &pBp->UdtTag;
        }
        pBp->UdtTag.NextItem = _t->UdtList.FirstItem;
        pBp->UdtTag.PrevTag = (KeListTag *)&_t->UdtList;
        _t->UdtList.FirstItem = pBp;
    }
}

void KeThCp_Update(KeaThCp_Update *_a) {
    KeaThBp_Update wrk;
    KeThBP *pBP;
    KeThCp *_t;
    int cmd;
    KeThBP *bp;

    _t = _a->This;
    cmd = 0;
    if (_t->Stt & KeThCp_FlgRootStep) {
        _t->Stt &= ~KeThCp_FlgRootStep;
        cmd = KeThBP_FlgRootStep;
    }
    sceVu0UnitMatrix(wrk.UnitMat);
    wrk.TP = _a->TP;
    wrk.PkHd = _a->Res;
    wrk.Pr = _a->Pr;
    pBP = (KeThBP *)_t->UdtList.FirstItem;
    while (pBP != NULL) {
        if (pBP->Flg & KeThBP_SttCpSync) {
            KeVu0CopyVecXYZ(pBP->RpTbl[pBP->RootIdx].RPos, *_a->RPos);
            KeVu0CopyVecXYZ(pBP->RpTbl[pBP->RootIdx].InVec, *_a->Vec);
        }
        if (pBP->Flg & KeThBP_FlgNoParent) {
            pBP->Flg |= cmd;
        }
        wrk.This = pBP;
        KeThBP_Update(&wrk);
        if (pBP->Flg & KeThBP_FlgReqDel) {
            bp = pBP;
            pBP = (KeThBP *)pBP->UdtTag.NextItem;
            if (bp->UdtTag.NextItem != NULL) {
                ((KeThBP *)bp->UdtTag.NextItem)->UdtTag.PrevTag = bp->UdtTag.PrevTag;
            }
            bp->UdtTag.PrevTag->NextItem = bp->UdtTag.NextItem;
            KeThBP_Quit(bp);
            bp->UdtTag.NextItem = _a->Res->FreeAlow.FirstItem;
            _a->Res->FreeAlow.FirstItem = bp;
        } else {
            pBP = (KeThBP *)pBP->UdtTag.NextItem;
        }
    }
}

void KeThCp_Draw(KeThCp *_t, sceVu0FMATRIX *pMat, float *scl, KetThBp_Pr *pPr, u_long128 **pPkBuf, u_long Alpha, u_long Test) {
    sceDmaChan *dcGif;
    KeaThBp_MkGifPk aMk;
    KeThBP *pBP;
    sceDmaTag *tag;

    dcGif = sceDmaGetChan(2);
    pBP = (KeThBP *)_t->UdtList.FirstItem;
    aMk.Pr = pPr;
    aMk.Mat = pMat;
    aMk.Scl = scl;
    aMk.Alpha = Alpha;
    aMk.Test = Test;
    sceGsSyncPath(0, 0);
    for (; pBP != NULL; pBP = (KeThBP *)pBP->UdtTag.NextItem) {
        aMk.This = pBP;
        aMk.GifPkBuf = *pPkBuf + 1;
        if (KeThBP_MkGifPk(&aMk)) {
            tag = (sceDmaTag *)*pPkBuf;
            *(u_long128 *)tag = 0;
            tag->id = 0x70;
            aMk.LTag->EOP = 1;
            tag->qwc = ((u_int)aMk.GifPkBuf - (u_int)*pPkBuf >> 4) - 1;
            FlushCache(0);
            sceDmaSend(dcGif, tag);
            sceGsSyncPath(0, 0);
        }
    }
}

void KeThCp_Init(KeThCp *_t) {
    _t->UdtList.FirstItem = NULL;
    _t->Stt = 0;
}

// SOURCE DATA: .data
// SOURCE DATA SLUS_217.99_retail: .data
