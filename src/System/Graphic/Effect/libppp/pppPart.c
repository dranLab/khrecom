#include "pppPart.h"
#include "pppAlloc.h"
#include "System/MainApp.h"
#include "System/Debug.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixXYZ.h"
#include "include_asm.h"

float sqrtf(float x);
float atan2f(float y, float x);
float asinf(float x);

void pppRyjMegaBirth();
void pppRyjMegaBirthModel();
void pppEiDrawShipoly();

/* calc functions pppEmptyRunPobject must not run */
static void *NoEmptyRunCalcFunc[] = {
    pppRyjMegaBirth,
    pppRyjMegaBirthModel,
    pppEiDrawShipoly,
    NULL,
};

/* The .bss globals (initialized so that gcc emits them in this order, not in the order of the header's
   declarations); ppvParLoc, ppvParAng and ppvParScl are common symbols in the original. */
pppMngSt *ppvMng __attribute__((section(".bss"))) = 0;
pppEnvSt *ppvEnv __attribute__((section(".bss"))) = 0;
pppPObject *ppvPObj __attribute__((section(".bss"))) = 0;
sceVu0FMATRIX ppvParMatrix __attribute__((section(".bss"))) = {0};
sceVu0FMATRIX ppvCameraMatrix __attribute__((section(".bss"))) = {0};
sceVu0FMATRIX ppvCameraMatrixOld __attribute__((section(".bss"))) = {0};
sceVu0FMATRIX ppvCameraMatrixInv __attribute__((section(".bss"))) = {0};
sceVu0FVECTOR ppvEye __attribute__((section(".bss"))) = {0};
sceVu0FVECTOR ppvAt __attribute__((section(".bss"))) = {0};
sceVu0FVECTOR ppvEyeOld __attribute__((section(".bss"))) = {0};
sceVu0FVECTOR ppvAtOld __attribute__((section(".bss"))) = {0};
float ppvFov __attribute__((section(".bss"))) = 0;
sceVu0FMATRIX ppvWorldMatrix __attribute__((section(".bss"))) = {0};
sceVu0FMATRIX ppvWorldMatrixWood __attribute__((section(".bss"))) = {0};
sceVu0FMATRIX ppvScreenMatrix __attribute__((section(".bss"))) = {0};
sceVu0FMATRIX ppvSWMatrix __attribute__((section(".bss"))) = {0};
sceVu0FMATRIX ppvSWMatrixWood __attribute__((section(".bss"))) = {0};
sceVu0FMATRIX ppvLightMatrix __attribute__((section(".bss"))) = {0};
sceVu0FMATRIX ppvColorMatrix __attribute__((section(".bss"))) = {0};
sceVu0FMATRIX ppvSCMatrix __attribute__((section(".bss"))) = {0};
sceVu0FMATRIX ppvUnitMatrix __attribute__((section(".bss"))) = {0};
sceVu0FVECTOR ppvZeroVector __attribute__((section(".bss"))) = {0};
sceVu0FMATRIX *ppvlpDMatrix __attribute__((section(".bss"))) = 0;
sceVu0FMATRIX *ppvlpCMatrix __attribute__((section(".bss"))) = 0;
BOOL ppvSysStopPartF __attribute__((section(".bss"))) = 0;
BOOL ppvUserStopPartF __attribute__((section(".bss"))) = 0;
PPPUSRFUNC *ppvHookFuncTbl __attribute__((section(".bss"))) = 0;
u_long128 *ppvCurPrimp __attribute__((section(".bss"))) = 0;
int ppvCurPrimpLen __attribute__((section(".bss"))) = 0;
u_long128 *ppvPrimp __attribute__((section(".bss"))) = 0;
int ppvPrimpLen __attribute__((section(".bss"))) = 0;
int ppvEmptyLoop __attribute__((section(".bss"))) = 0;
pppSVECTOR *ppvVertex __attribute__((section(".bss"))) = 0;
pppSVECTOR *ppvNormal __attribute__((section(".bss"))) = 0;
long128 ppvVNWork[2048] __attribute__((section(".bss"))) = {0};
int ppvchgmem __attribute__((section(".bss"))) = 0;

static void pppEmptyRunPobject(pppMngSt *mng, pppPObject *pobj);

/* Where the original keeps a store to ppvPObj ahead of the loads after it, as if the variable were volatile */
#define SET_PPVPOBJ(p) (*(pppPObject *volatile *)&ppvPObj = (p))

static void callInitProg(pppMngSt *mng) {
    word progn;
    pppProg *prog;

    for (progn = 0; progn < mng->progn; progn++) {
        prog = mng->m_prog[progn];
        if (prog->iniFunc != NULL) {
            ((PPPINI)prog->iniFunc)(mng);
        }
    }
}

static void callStartProg(pppMngSt *mng) {
    word progn;
    pppProg *prog;

    for (progn = 0; progn < mng->progn; progn++) {
        prog = mng->m_prog[progn];
        if (prog->staFunc != NULL) {
            ((PPPSTA)prog->staFunc)(mng);
        }
    }
}

static void callEndProg(pppMngSt *mng) {
    word progn;
    pppProg *prog;

    for (progn = 0; progn < mng->progn; progn++) {
        prog = mng->m_prog[progn];
        if (prog->endFunc != NULL) {
            ((PPPEND)prog->endFunc)(mng);
        }
    }
}

static void callCon2Prog(pppPObject *pobj) {
    pppPData *pdt;
    word ctrln;
    pppCtrlData **lpcdt;
    pppProg *prog;

    pdt = pobj->pdtval->pdt;
    lpcdt = (pppCtrlData **)((char *)pobj + pdt->valbytes);
    for (ctrln = 0; ctrln < pdt->ctrlTablen; ctrln++) {
        pppCtrlTable *ctbl = &pdt->ctrlTable[ctrln];
        prog = ctbl->prog;
        *lpcdt++ = ctbl->ctrldt;
        if (prog != NULL && prog->con2Func != NULL) {
            ((PPPCON2)prog->con2Func)(pobj, ctbl);
        }
    }
}

static void callConProg(pppPObject *pobj) {
    pppPData *pdt;
    word ctrln;
    pppCtrlData **lpcdt;
    pppProg *prog;

    pdt = pobj->pdtval->pdt;
    lpcdt = (pppCtrlData **)((char *)pobj + pdt->valbytes);
    for (ctrln = 0; ctrln < pdt->ctrlTablen; ctrln++) {
        pppCtrlTable *ctbl = &pdt->ctrlTable[ctrln];
        prog = ctbl->prog;
        *lpcdt++ = ctbl->ctrldt;
        if (prog != NULL && prog->conFunc != NULL) {
            ((PPPCON)prog->conFunc)(pobj, ctbl);
        }
    }
}

pppPObject *pppCreatePObject(pppMngSt *mng, pppPDataVal *pdv0) {
    pppPData *pdt;
    pppPObject *apobj;
    pppPObject *pobj;
    pppPObject *ppobj;
    short prio;

    pdt = pdv0->pdt;
    apobj = pppAlloc(&ppvEnv->heapMng, pdt->valbytes + pdt->ctrlTablen * 4);
#line 222
    if (!(((int)apobj & 0xf) == 0)) AssertApp(__FILE__, __LINE__, "((int)apobj & 0xf) == 0", 0);
    if (apobj == NULL) {
        TRACE("!!!!!!!!!!!!!!!!!!!!!!!!! Effect pppCreatePObject Malloc FALSE !!!!!!!!!!!!!!!!!!!!!!!\n");
        return NULL;
    }
    apobj->time = 0;
    apobj->appearf = 1;
    apobj->lpvert = NULL;
    apobj->lpnorm = NULL;
    apobj->pdtval = pdv0;
    apobj->loop_in = 0;
    apobj->loop_start0 = 0;
    pobj = mng->m_pobj;
    if (pobj == NULL) {
        pdv0->pobj = apobj;
        mng->m_pobj = apobj;
        apobj->next = NULL;
    } else if (pdv0->pobj != NULL) {
        apobj->next = pdv0->pobj->next;
        pdv0->pobj->next = apobj;
    } else {
        prio = pdt->prio;
        ppobj = (pppPObject *)&mng->m_pobj;
        for (; pobj != NULL; pobj = pobj->next) {
            if (pobj->pdtval->pdt->prio >= prio) {
                pdv0->pobj = apobj;
                ppobj->next = apobj;
                apobj->next = pobj;
                goto end;
            }
            ppobj = pobj;
        }
        pdv0->pobj = apobj;
        ppobj->next = apobj;
        apobj->next = NULL;
    }
end:
    pdv0->pobjn++;
    callConProg(apobj);
    return apobj;
}

void pppDeletePObject(pppPObject *pobj) {
    pppPDataVal *pdv;
    pppPData *pdt;
    word ctrln;
    pppProg *prog;

    pdv = pobj->pdtval;
    pdt = pdv->pdt;
    for (ctrln = 0; ctrln < pdt->ctrlTablen; ctrln++) {
        pppCtrlTable *ctbl = &pdt->ctrlTable[ctrln];
        prog = ctbl->prog;
        if (prog != NULL && prog->desFunc != NULL) {
            ((PPPDES)prog->desFunc)(pobj, ctbl);
        }
    }
    if (--pdv->pobjn == 0) {
        pdv->pobj = NULL;
    } else if (pdv->pobj == pobj) {
        pdv->pobj = pobj->next;
    }
    pppFree(&ppvEnv->heapMng, pobj);
}

void pppAllFreePObject(pppMngSt *mng) {
    pppPObject *pobj;
    pppPObject *next;

    pobj = mng->m_pobj;
    while (pobj != NULL) {
        next = pobj->next;
        pppDeletePObject(pobj);
        pobj = next;
    }
    mng->m_pobj = NULL;
    if (mng->m_pdtval != NULL) {
        pppFree(&ppvEnv->heapMng, mng->m_pdtval);
        mng->m_pdtval = NULL;
    }
}

void pppNormalize(float *v0, float *v1) {
    float len;

    len = sceVu0InnerProduct(v1, v1);
    if (len > 0.0f) {
        len = sqrtf(len);
        sceVu0Normalize(v0, v1);
        v0[3] = len;
    } else {
        v0[2] = 1.0f;
        v0[0] = 0.0f;
        v0[1] = 0.0f;
        v0[3] = 0.0f;
    }
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Effect/libppp/pppPart", pppSetMatrix);

/* Differs only in v0/v1 register choice for the scale factors of SCALE_VECTOR_XYZ (reload rotation): the original rotates v0/v1, ours always uses v0. */
#if 0
void pppSetMatrix(pppMngSt *mng) {
    sceVu0FMATRIX mat;
    sceVu0FVECTOR v;
    int i;

    sceVu0CopyMatrix(ppvParMatrix, mng->u.parMat);
    SCALE_VECTOR_XYZ(ppvParMatrix[0], mng->parScl[0]);
    SCALE_VECTOR_XYZ(ppvParMatrix[1], mng->parScl[1]);
    SCALE_VECTOR_XYZ(ppvParMatrix[2], mng->parScl[2]);
    if (mng->pEtc->bChrScale) {
        SCALE_VECTOR_XYZ(ppvParMatrix[0], mng->parChrScl[0]);
        SCALE_VECTOR_XYZ(ppvParMatrix[1], mng->parChrScl[1]);
        SCALE_VECTOR_XYZ(ppvParMatrix[2], mng->parChrScl[2]);
    }
    pppGetRotMatrixXYZ(mat, (pppIVECTOR *)mng->pEtc->ivecAngle);
    SCALE_VECTOR_XYZ(mat[0], mng->pEtc->vecScale[0]);
    SCALE_VECTOR_XYZ(mat[1], mng->pEtc->vecScale[1]);
    SCALE_VECTOR_XYZ(mat[2], mng->pEtc->vecScale[2]);
    mat[3][0] = mng->pEtc->vecOffset[0];
    mat[3][1] = mng->pEtc->vecOffset[1];
    mat[3][2] = mng->pEtc->vecOffset[2];
    sceVu0MulMatrix(ppvParMatrix, ppvParMatrix, mat);
    if (mng->bBoolFollowCam) {
        ppvParMatrix[3][0] += ppvEye[0];
        ppvParMatrix[3][1] += ppvEye[1];
        ppvParMatrix[3][2] += ppvEye[2];
    }
    for (i = 0; i < 3; i++) {
        pppNormalize(mat[i], ppvParMatrix[i]);
        ppvParScl[i] = mat[i][3];
        mat[i][3] = 0.0f;
    }
    v[0] = atan2f(-mat[1][2], mat[0][2]);
    v[1] = asinf(-mat[2][2]);
    v[2] = atan2f(mat[2][1], mat[2][0]);
    for (i = 0; i < 3; i++) {
        ppvParAng[i] = v[i];
    }
    sceVu0CopyVector(ppvParLoc, ppvParMatrix[3]);
}
#endif

BOOL pppStartPart(pppMngSt *mng, long *lpdt, BOOL callInitFuncf) {
    char *pdt0;
    u_char pdtno;
    pppPData *pdt;
    pppPDataVal *pdv;
    word *progdt;
    word *lppdt;
    word partflag;

    ppvMng = mng;
    pdt0 = (char *)lpdt;
    mng->totalFrame = *(word *)lpdt;
    lpdt = (long *)((word *)lpdt + 1);
    partflag = *(word *)lpdt;
    lpdt = (long *)((word *)lpdt + 1);
    progdt = (word *)(pdt0 + *(word *)lpdt);
    lpdt = (long *)((word *)lpdt + 1);
    lppdt = (word *)(pdt0 + *(word *)lpdt);
    lpdt = (long *)((word *)lpdt + 1);
    if (partflag & 1) {
        mng->partLoopf = 1;
    } else {
        mng->partLoopf = 0;
    }
    mng->progn = *progdt++;
    mng->m_prog = (pppProg **)progdt;
    mng->pdtn = *lppdt++;
    mng->m_lppdt = (pppPData **)lppdt;
    if (mng->pdtn != 0) {
        mng->m_pdtval = pppAlloc(&ppvEnv->heapMng, mng->pdtn * sizeof(pppPDataVal));
        if (mng->m_pdtval == NULL) {
            TRACE("!!!!!!!!!!!!!!!!!!!!!!!!! Effect pppStartPart Malloc FALSE !!!!!!!!!!!!!!!!!!!!!!!\n");
            return FALSE;
        }
    } else {
        mng->m_pdtval = NULL;
    }
    pdt = (pppPData *)lpdt;
    if (pdt->next != NULL) {
        mng->m_pdata = NULL;
    }
    mng->time = 0;
    mng->partLoopEndf = 0;
    if (mng->pdtn == 0) {
        mng->m_pdata = NULL;
    } else {
        mng->m_pdata = pdt;
    }
    mng->m_pobj = NULL;
    pdv = mng->m_pdtval;
    pdtno = 0;
    while (pdt != NULL) {
        pdv->pdt = pdt;
        pdv->startFrame = pdt->startFrameInit;
        pdv->pdtno = pdtno++;
        pdv->pobj = NULL;
        pdv->pobjn = 0;
        pdv++;
        pdt = pdt->next;
    }
    if (callInitFuncf) {
        callInitProg(mng);
    }
    mng->fFrameTimeAccum = 0;
    return TRUE;
}

void pppInitPdt(long *lpdt, pppProg *progtbl) {
    char *pdt0;
    pppProg **progdt;
    word *lppdt;
    word progn;
    word pct;
    word pdtn;
    word pdtct;
    pppPData *pdt;
    word ctbln;
    int idx;

    lpdt = (long *)((char *)lpdt + sizeof(pppPEtcData));
    pdt0 = (char *)lpdt;
    progdt = (pppProg **)(pdt0 + ((word *)pdt0)[2]);
    lppdt = (word *)(pdt0 + ((word *)pdt0)[3]);
    pdt = (pppPData *)(pdt0 + 16);
    progn = *(word *)progdt;
    pdtn = *lppdt;
    progdt++;
    lppdt++;
    if (pdt->next == NULL) return;
    for (;;) {
        pdt->next = (pppPData *)(pdt0 + (word)pdt->next);
        for (ctbln = 0; ctbln < pdt->ctrlTablen; ctbln++) {
            pppCtrlTable *ctbl = &pdt->ctrlTable[ctbln];
            idx = (int)ctbl->prog;
            ctbl->ctrldt = (pppCtrlData *)(pdt0 + (word)ctbl->ctrldt);
            ctbl->prog = &progtbl[idx];
            ctbl->useVal = (word *)(pdt0 + (word)ctbl->useVal);
        }
        if (pdt->next->next == NULL) break;
        pdt = pdt->next;
    }
    pdt->next = NULL;
    for (pct = 0; pct < progn; pct++) {
        progdt[pct] = &progtbl[(int)progdt[pct]];
    }
    for (pdtct = 0; pdtct < pdtn; pdtct++) {
        lppdt[pdtct] = lppdt[pdtct] + (word)pdt0;
    }
}

void pppMovePdt(long *lpdt, int offset) {
    char *pdt0;
    word *lppdt;
    word pdtn;
    word pdtct;
    pppPData *pdt;
    word ctbln;
    word *lp;

    lpdt = (long *)((char *)lpdt + sizeof(pppPEtcData));
    pdt0 = (char *)lpdt;
    lpdt = (long *)((char *)lpdt + 12);
    lppdt = (word *)(pdt0 + *(word *)lpdt);
    lpdt = (long *)((char *)lpdt + 4);
    pdt = (pppPData *)lpdt;
    pdtn = *lppdt++;
    for (;;) {
        for (ctbln = 0; ctbln < pdt->ctrlTablen; ctbln++) {
            pppCtrlTable *ctbl = &pdt->ctrlTable[ctbln];
            ctbl->ctrldt = (pppCtrlData *)(offset + (word)ctbl->ctrldt);
            ctbl->useVal = (word *)(offset + (word)ctbl->useVal);
        }
        if (pdt->next == NULL) break;
        pdt->next = (pppPData *)(offset + (word)pdt->next);
        pdt = pdt->next;
    }
    for (pdtct = 0; pdtct < pdtn; pdtct++) {
        lppdt[pdtct] += offset;
    }
}

static void pppRunPartCalc(pppMngSt *mng) {
    int pdvn;
    pppPDataVal *pdv;
    pppPData *pdt;
    int ctrln;
    pppCtrlTable *ctbl;
    pppProg *prog;
    int pobjct;
    pppPObject *pobj;
    PPPCALC calcFunc;
    PPPDRAW drawFunc;
    pppCtrlData **lpcdt;
    pppCtrlData *cdt;
    pppCtrlData *ncdt;
    pppPObject *next;

    for (pdvn = 0; pdvn < mng->pdtn; pdvn++) {
        pdv = &mng->m_pdtval[pdvn];
        pdt = pdv->pdt;
        if (pdv->pobjn == 0) continue;
        for (ctrln = 0; ctrln < pdt->ctrlTablen; ctrln++) {
            ctbl = &pdt->ctrlTable[ctrln];
            pobj = pdv->pobj;
            prog = ctbl->prog;
            calcFunc = (PPPCALC)prog->calcFunc;
            drawFunc = (PPPDRAW)prog->drawFunc;
            if (calcFunc == NULL || drawFunc != NULL) continue;
            pobjct = pdv->pobjn;
            do {
                lpcdt = (pppCtrlData **)((char *)pobj + pdt->valbytes) + ctrln;
                next = pobj->next;
                if (!pobj->appearf) {
                    cdt = *lpcdt;
                    ncdt = (pppCtrlData *)((char *)cdt + ctbl->dtbytes);
                    ppvPObj = pobj;
                    if (!ppvUserStopPartF && !ppvMng->bFlagStop && !ppvMng->bBoolSlow && pobj->loop_start0) {
                        pobj->time = 0;
                        pobj->loop_in = 1;
                        callCon2Prog(pobj);
                        pobj->loop_start0 = 0;
                    }
                    if (ncdt->time == pobj->time) {
                        cdt = ncdt;
                        *lpcdt = cdt;
                    }
                    calcFunc(pobj, cdt, ctbl);
                }
                pobj = next;
            } while (--pobjct);
        }
    }
}

static void pppRunPartDraw(pppMngSt *mng, BOOL drawf) {
    int pdvn;
    pppPDataVal *pdv;
    pppPData *pdt;
    int ctrln;
    pppCtrlTable *ctbl;
    pppProg *prog;
    int pobjct;
    pppPObject *pobj;
    PPPCALC calcFunc;
    PPPDRAW drawFunc;

    for (pdvn = 0; pdvn < mng->pdtn; pdvn++) {
        pdv = &mng->m_pdtval[pdvn];
        pdt = pdv->pdt;
        if (pdv->pobjn == 0) continue;
        for (ctrln = 0; ctrln < pdt->ctrlTablen; ctrln++) {
            ctbl = &pdt->ctrlTable[ctrln];
            pobj = pdv->pobj;
            prog = ctbl->prog;
            calcFunc = (PPPCALC)prog->calcFunc;
            drawFunc = (PPPDRAW)prog->drawFunc;
            if (drawFunc == NULL) continue;
            if (calcFunc != NULL) {
                pobjct = pdv->pobjn;
                do {
                    pppCtrlData **lpcdt;
                    pppCtrlData *cdt;
                    pppCtrlData *ncdt;
                    pppPObject *next;

                    lpcdt = (pppCtrlData **)((char *)pobj + pdt->valbytes) + ctrln;
                    next = pobj->next;
                    if (!pobj->appearf) {
                        cdt = *lpcdt;
                        ncdt = (pppCtrlData *)((char *)cdt + ctbl->dtbytes);
                        ppvPObj = pobj;
                        if (!ppvUserStopPartF && !ppvMng->bFlagStop && !ppvMng->bBoolSlow && pobj->loop_start0) {
                            pobj->time = 0;
                            pobj->loop_in = 1;
                            callCon2Prog(pobj);
                            pobj->loop_start0 = 0;
                        }
                        if (ncdt->time == pobj->time) {
                            cdt = ncdt;
                            *lpcdt = cdt;
                        }
                        calcFunc(pobj, cdt, ctbl);
                        if (drawf) {
                            drawFunc(pobj, cdt, ctbl);
                        }
                    }
                    pobj = next;
                } while (--pobjct);
            } else {
                pobjct = pdv->pobjn;
                do {
                    pppCtrlData **lpcdt;
                    pppCtrlData *cdt;
                    pppCtrlData *ncdt;

                    lpcdt = (pppCtrlData **)((char *)pobj + pdt->valbytes) + ctrln;
                    if (!pobj->appearf) {
                        cdt = *lpcdt;
                        ncdt = (pppCtrlData *)((char *)cdt + ctbl->dtbytes);
                        SET_PPVPOBJ(pobj);
                        if (ncdt->time == pobj->time) {
                            cdt = ncdt;
                            *lpcdt = cdt;
                        }
                        if (drawf) {
                            drawFunc(pobj, cdt, ctbl);
                        }
                    }
                    pobj = pobj->next;
                } while (--pobjct);
            }
        }
        if (!ppvUserStopPartF && !ppvMng->bFlagStop && !ppvMng->bBoolSlow) {
            int pobjct;
            pppPObject *pobj;

            pobj = pdv->pobj;
            pobjct = pdv->pobjn;
            do {
                if (!pobj->appearf) {
                    pobj->time += 0x1000;
                    if (pobj->time > 0x6FFFFFFF) {
                        pobj->time = 0x6FFFF000;
                    }
                } else {
                    pobj->appearf = 0;
                }
                pobj = pobj->next;
            } while (--pobjct);
        }
    }
    if (!ppvUserStopPartF && !ppvMng->bFlagStop && !ppvMng->bBoolSlow) {
        u_word or;
        pppPObject *pobj;
        pppPObject *ppobj;
        pppPObject *next;
        pppPDataVal *pdv;
        pppPData *pdt;

        or = 0;
        ppobj = (pppPObject *)&mng->m_pobj;
        for (pobj = mng->m_pobj; pobj != NULL; pobj = next) {
            pdv = pobj->pdtval;
            pdt = pdv->pdt;
            next = pobj->next;
            if (!mng->partLoopEndf && pobj->time >= pdt->loopFrame1 && pdt->loopFrame0 <= 0x6FFFFFFF &&
                pdt->loopFrame1 <= 0x6FFFFFFF) {
                pobj->time = 0;
                pobj->loop_in = 1;
                callCon2Prog(pobj);
                if (pdt->loopFrame0 == 0) {
                    ppvEmptyLoop = 1;
                    pppEmptyRunPobject(mng, pobj);
                    ppvEmptyLoop = 0;
                    pobj->loop_start0 = 1;
                } else {
                    ppvEmptyLoop = 1;
                    /* a plain while loop gets its test copied to the top; the original tests at the bottom only */
                    goto test;
                    do {
                        pppEmptyRunPobject(mng, pobj);
                    test:;
                    } while (pobj->time < pdt->loopFrame0);
                    ppvEmptyLoop = 0;
                }
                ppobj = pobj;
            } else {
                or |= pdt->totalFrame;
                if ((u_word)pobj->time >= (u_word)pdt->totalFrame) {
                    ppobj->next = next;
                    pppDeletePObject(pobj);
                } else {
                    ppobj = pobj;
                }
            }
        }
        if (mng->partLoopf && mng->partLoopEndf && or == 0x70000000) {
            pppPObject *p;
            pppPObject *pnxt;

            p = mng->m_pobj;
            while (p != NULL) {
                pnxt = p->next;
                pppDeletePObject(p);
                p = pnxt;
            }
            mng->m_pobj = NULL;
        }
    }
}

static void pppEmptyRunPobject(pppMngSt *mng, pppPObject *pobj) {
    pppPDataVal *pdv;
    pppPData *pdt;
    pppCtrlData **lpcdt;
    word ctrln;
    pppCtrlTable *ctbl;
    pppProg *prog;
    pppCtrlData *cdt;
    pppCtrlData *ncdt;
    void **ppv;
    int not_exec;

    pdv = pobj->pdtval;
    pdt = pdv->pdt;
    lpcdt = (pppCtrlData **)((char *)pobj + pdt->valbytes);
    SET_PPVPOBJ(pobj);
    for (ctrln = 0; ctrln < pdt->ctrlTablen; ctrln++) {
        ctbl = &pdt->ctrlTable[ctrln];
        prog = ctbl->prog;
        if (prog == NULL) continue;
        cdt = *lpcdt;
        ncdt = (pppCtrlData *)((char *)cdt + ctbl->dtbytes);
        if (ncdt->time == pobj->time) {
            cdt = ncdt;
            *lpcdt++ = cdt;
        } else {
            lpcdt++;
        }
        if (prog->calcFunc == NULL) continue;
        ppv = NoEmptyRunCalcFunc;
        not_exec = 0;
        for (; *ppv != NULL; ppv++) {
            if (prog->calcFunc == *ppv) {
                not_exec = 1;
                break;
            }
        }
        if (!not_exec) {
            ((PPPCALC)prog->calcFunc)(pobj, cdt, ctbl);
        }
    }
    if (!ppvUserStopPartF && !ppvMng->bFlagStop && !ppvMng->bBoolSlow) {
        pobj->time += 0x1000;
        if (pobj->time > 0x6FFFFFFF) {
            pobj->time = 0x6FFFF000;
        }
    }
}

static BOOL apeaPObject(pppMngSt *mng) {
    int pdtct;
    pppPDataVal *pdv;

    for (pdtct = 0; pdtct < mng->pdtn; pdtct++) {
        pdv = &mng->m_pdtval[pdtct];
#line 1011
        if (!(pdv != NULL)) AssertApp(__FILE__, __LINE__, "pdv != ((void *)0)", 0);
        if (mng->time >= pdv->startFrame) {
            pdv->startFrame = 0x7FFFFFFF;
            pdv->pobj = pppCreatePObject(mng, pdv);
            if (pdv->pobj == NULL) return FALSE;
            pdv->pobj->appearf = 0;
        }
    }
    return TRUE;
}

BOOL pppRunPartExec(pppMngSt *mng) {
#line 1033
    if (!(mng != NULL)) AssertApp(__FILE__, __LINE__, "mng != ((void *)0)", 0);
    callStartProg(mng);
    if (!apeaPObject(mng)) return FALSE;
    pppRunPartCalc(mng);
    return TRUE;
}

BOOL pppRunPart(pppMngSt *mng, BOOL drawf) {
#line 1053
    if (!(mng != NULL)) AssertApp(__FILE__, __LINE__, "mng != ((void *)0)", 0);
    pppRunPartDraw(mng, drawf);
    callEndProg(mng);
    return pppRunPartImpotent(mng);
}

BOOL pppRunPartImpotent(pppMngSt *mng) {
#line 1067
    if (!(mng != NULL)) AssertApp(__FILE__, __LINE__, "mng != ((void *)0)", 0);
    if (ppvUserStopPartF || ppvMng->bFlagStop || ppvMng->bBoolSlow) return FALSE;
    mng->otime = mng->time;
    mng->time += 0x1000;
    if (!mng->bEnd && mng->time >= mng->totalFrame) {
        mng->bEnd = 1;
    }
    if (mng->partLoopf) {
        if (mng->partLoopEndf) {
            return mng->m_pobj == NULL;
        }
        if (mng->time > 0x6FFFFFFF) {
            mng->time = 0x70000000;
            mng->otime = 0x6FFFF000;
        }
        return FALSE;
    }
    return mng->bEnd;
}
// SOURCE DATA: .data .rodata .bss
