#include "RyjMegaBirthUserCtrl.h"
#include "System/Graphic/Effect/libppp/draw/shape.h"
#include "System/Graphic/Effect/libppp/draw/md.h"
#include "System/Graphic/Effect/libppp/draw/sin_t.h"

typedef struct _USER_PARAM { /* 0x4 */
    u_char Alpha;
    u_char SeqCt;
    u_char Rot;
    u_char Scale;
} USER_PARAM;

extern sceVu0FMATRIX ppvUnitMatrix;
extern sceVu0FMATRIX ppvSCMatrix;

/* m1..m4 (vf1..vf4) = screen (vf13..vf16) * m */
#define MUL_SCREEN \
    "vmulax.xyzw $ACC, $vf13, $vf1x\n" \
    "vmadday.xyzw $ACC, $vf14, $vf1y\n" \
    "vmaddaz.xyzw $ACC, $vf15, $vf1z\n" \
    "vmaddw.xyzw $vf1, $vf16, $vf1w\n" \
    "vmulax.xyzw $ACC, $vf13, $vf2x\n" \
    "vmadday.xyzw $ACC, $vf14, $vf2y\n" \
    "vmaddaz.xyzw $ACC, $vf15, $vf2z\n" \
    "vmaddw.xyzw $vf2, $vf16, $vf2w\n" \
    "vmulax.xyzw $ACC, $vf13, $vf3x\n" \
    "vmadday.xyzw $ACC, $vf14, $vf3y\n" \
    "vmaddaz.xyzw $ACC, $vf15, $vf3z\n" \
    "vmaddw.xyzw $vf3, $vf16, $vf3w\n" \
    "vmulax.xyzw $ACC, $vf13, $vf4x\n" \
    "vmadday.xyzw $ACC, $vf14, $vf4y\n" \
    "vmaddaz.xyzw $ACC, $vf15, $vf4z\n" \
    "vmaddw.xyzw $vf4, $vf16, $vf4w\n"

void RyjMegaBirthUserCtrDrawShapeRefPos(int shape_id, pppCVECTOR *color, u_char fog, u_char near) {
    OSHAPE_IN_VU1 *shi0;
    OSHAPE_IN_VU1 *shi1;
    OSHAPE_VU1 *sh;
    OANM_HEADER *ah;
    OANM_SEQ *seq;
    pppShapeSt *shape;
    int i;
    sceVu0FMATRIX rot_matrix;
    USER_PARAM *user_param;
    int rot;
    float scale;
    int anm_seq;
    float s;
    float c;

    pppStartVu1ShapeProg(fog);
    shi0 = pppGetVu1DrawSpad0();
    shi1 = pppGetVu1DrawSpad1();
    if (color->a >= 0xFF) {
        shi0->flag = shi1->flag = 2;
    } else {
        shi0->flag = shi1->flag = 0;
    }
    shi0->addz = shi1->addz = 0.0f;
    shi0->r = shi1->r = color->r;
    shi0->g = shi1->g = color->g;
    shi0->b = shi1->b = color->b;
    shi0->a = shi1->a = color->a;
    if (shape_id == 0xFFFF) {
        return;
    }
    for (i = 0; i < ppvReferencePosNum; i++) {
        user_param = NULL;
        rot = 0;
        scale = 1.0f;
        anm_seq = 0;
        if (ppvReferencePosList[i][3] != 1.0f) {
            user_param = (USER_PARAM *)&ppvReferencePosList[i][3];
        }
        if (user_param != NULL && user_param->Alpha == 0) {
            continue;
        }
        shi0 = pppGetVu1DrawSpad();
        if (user_param != NULL) {
            shi0->a = user_param->Alpha;
            anm_seq = user_param->SeqCt;
            if (user_param->Rot) {
                rot = user_param->Rot;
                rot <<= 8;
            }
            scale = user_param->Scale;
            scale *= 0.1f;
        }
        if (rot == 0) {
            /* MATCH: the copy goes through t0-t3, all loads before the stores */
            register u_long128 m0 asm("$8");
            register u_long128 m1 asm("$9");
            register u_long128 m2 asm("$10");
            register u_long128 m3 asm("$11");
            u_long128 *src = (u_long128 *)ppvUnitMatrix;

            /* MATCH: makes src opaque, so loop.c does not hoist &ppvUnitMatrix out of the loop (its lui/addiu stay here) */
            asm volatile("" : "=r"(src) : "0"(src));
            m0 = src[0];
            m1 = src[1];
            m2 = src[2];
            m3 = src[3];
            ((u_long128 *)rot_matrix)[0] = m0;
            ((u_long128 *)rot_matrix)[1] = m1;
            ((u_long128 *)rot_matrix)[2] = m2;
            ((u_long128 *)rot_matrix)[3] = m3;
            /* MATCH: src stays live past the loads (no REG_DEAD in the last one: sched1 keeps the source order), and the asm
               stops reorg from moving the last sq into the b's delay slot */
            asm volatile("" : : "r"(src));
        } else {
            s = eff_sin_t[(u_int)rot >> 4];
            c = eff_sin_t[((rot + 0x4000) >> 4) & 0xFFF];
            rot_matrix[0][0] = c;
            rot_matrix[0][1] = s;
            rot_matrix[1][0] = -s;
            rot_matrix[1][1] = c;
            rot_matrix[2][0] = 0.0f;
            rot_matrix[3][0] = 0.0f;
            rot_matrix[2][1] = 0.0f;
            rot_matrix[3][1] = 0.0f;
            rot_matrix[0][2] = 0.0f;
            rot_matrix[1][2] = 0.0f;
            rot_matrix[2][2] = 1.0f;
            rot_matrix[3][2] = 0.0f;
            rot_matrix[0][3] = 0.0f;
            rot_matrix[1][3] = 0.0f;
            rot_matrix[2][3] = 0.0f;
            rot_matrix[3][3] = 1.0f;
        }
        shape = &ppvEnv->lpShape[shape_id];
        ah = (OANM_HEADER *)shape->shape;
        seq = &ah->seq[anm_seq];
        sh = (OSHAPE_VU1 *)((char *)ah + seq->sha);
        /* MATCH: one asm for the VU0/COP2-only run (no GPR instruction inside). One asm per instruction was tried
           (2026-11-08): 66 lines with the operands in $2/$3/$5/$6/$7 register locals, 52 with shi0 and anm_seq pinned
           too. Each asm statement adds one insn to the life of every pseudo across the block, and shi0 (24 refs, life
           114) beats rot (14 refs, life 50) by 0.842 to 0.840 in global-alloc priority: one extra empty asm already
           swaps their s0/s1 (39 lines) */
        __asm__ __volatile__(
            "lqc2 $vf1, 0x0(%0)\n"
            "lqc2 $vf2, 0x10(%0)\n"
            "lqc2 $vf3, 0x20(%0)\n"
            "lqc2 $vf4, 0x30(%0)\n"
            "qmtc2.ni %1, $vf7\n"
            "vmulx.xyz $vf1, $vf1, $vf7x\n"
            "vmulx.xyz $vf2, $vf2, $vf7x\n"
            "vmulx.xyz $vf3, $vf3, $vf7x\n"
            "lqc2 $vf4, 0x0(%2)\n"
            "vmove.w $vf4, $vf0\n"
            "lqc2 $vf9, 0x0(%3)\n"
            "lqc2 $vf10, 0x10(%3)\n"
            "lqc2 $vf11, 0x20(%3)\n"
            "lqc2 $vf12, 0x30(%3)\n"
            "lqc2 $vf13, 0x0(%4)\n"
            "lqc2 $vf14, 0x10(%4)\n"
            "lqc2 $vf15, 0x20(%4)\n"
            "lqc2 $vf16, 0x30(%4)\n"
            "vmulax.xyzw $ACC, $vf9, $vf4x\n"
            "vmadday.xyzw $ACC, $vf10, $vf4y\n"
            "vmaddaz.xyzw $ACC, $vf11, $vf4z\n"
            "vmaddw.xyzw $vf4, $vf12, $vf4w\n"
            MUL_SCREEN
            "sqc2 $vf1, 0x0(%5)\n"
            "sqc2 $vf2, 0x10(%5)\n"
            "sqc2 $vf3, 0x20(%5)\n"
            "sqc2 $vf4, 0x30(%5)"
            :
            : "r"(rot_matrix), "r"(scale), "r"(&ppvReferencePosList[i]), "r"(ppvCameraMatrix), "r"(ppvScreenMatrix),
              "r"(ppvPObj->dmat));
        ((u_long128 *)shi0->mat)[0] = ((u_long128 *)ppvPObj->dmat)[0];
        ((u_long128 *)shi0->mat)[1] = ((u_long128 *)ppvPObj->dmat)[1];
        ((u_long128 *)shi0->mat)[2] = ((u_long128 *)ppvPObj->dmat)[2];
        ((u_long128 *)shi0->mat)[3] = ((u_long128 *)ppvPObj->dmat)[3];
        pppVu1ShapeDraw(sh, shi0);
        shi0->flag = 10;
        if (user_param == NULL) {
            continue;
        }
        if (ppvUserStopPartF) {
            continue;
        }
        if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
            continue;
        }
        user_param->SeqCt++;
        if (user_param->SeqCt >= ah->seqc) {
            if (seq->flag & 0x80) {
                user_param->SeqCt = 0;
            } else {
                user_param->Alpha = 0;
                user_param->SeqCt = 0;
            }
        }
    }
    pppWaitVif1();
    op_ztest_init();
}

void RyjMegaBirthUserCtrDrawShape(int shape_id, pppCVECTOR *color, u_char fog, u_char near) {
    OSHAPE_IN_VU1 *shi0;
    OSHAPE_IN_VU1 *shi1;
    OSHAPE_VU1 *sh;
    OANM_HEADER *ah;
    OANM_SEQ *seq;
    pppShapeSt *shape;
    int i;
    USER_PARAM *user_param;
    float scale;
    int anm_seq;

    pppStartVu1ShapeProg(fog);
    shi0 = pppGetVu1DrawSpad0();
    shi1 = pppGetVu1DrawSpad1();
    if (color->a >= 0xFF) {
        shi0->flag = shi1->flag = 2;
    } else {
        shi0->flag = shi1->flag = 0;
    }
    shi0->addz = shi1->addz = 0.0f;
    shi0->r = shi1->r = color->r;
    shi0->g = shi1->g = color->g;
    shi0->b = shi1->b = color->b;
    shi0->a = shi1->a = color->a;
    if (shape_id == 0xFFFF) {
        return;
    }
    for (i = 0; i < ppvReferenceMatrixNum; i++) {
        user_param = NULL;
        scale = 1.0f;
        anm_seq = 0;
        if (ppvReferenceMatrixList[i][3][3] != 1.0f) {
            user_param = (USER_PARAM *)&ppvReferenceMatrixList[i][3][3];
        }
        if (user_param != NULL && user_param->Alpha == 0) {
            continue;
        }
        shi0 = pppGetVu1DrawSpad();
        if (user_param != NULL) {
            shi0->a = user_param->Alpha;
            anm_seq = user_param->SeqCt;
            scale = user_param->Scale;
            scale *= 0.1f;
        }
        shape = &ppvEnv->lpShape[shape_id];
        ah = (OANM_HEADER *)shape->shape;
        seq = &ah->seq[anm_seq];
        sh = (OSHAPE_VU1 *)((char *)ah + seq->sha);
        __asm__ __volatile__(
            "vsub.xyzw $vf4, $vf0, $vf0\n"
            "vadd.w $vf4, $vf4, $vf0\n"
            "vmr32.xyzw $vf3, $vf4\n"
            "vmr32.xyzw $vf2, $vf3\n"
            "vmr32.xyzw $vf1, $vf2\n"
            "qmtc2.ni %0, $vf7\n"
            "vmulx.xyz $vf1, $vf1, $vf7x\n"
            "vmulx.xyz $vf2, $vf2, $vf7x\n"
            "vmulx.xyz $vf3, $vf3, $vf7x\n"
            "lqc2 $vf8, 0x30(%1)\n"
            "vmove.w $vf8, $vf0\n"
            "vmove.xyzw $vf4, $vf8\n"
            "lqc2 $vf9, 0x0(%2)\n"
            "lqc2 $vf10, 0x10(%2)\n"
            "lqc2 $vf11, 0x20(%2)\n"
            "lqc2 $vf12, 0x30(%2)\n"
            "lqc2 $vf13, 0x0(%3)\n"
            "lqc2 $vf14, 0x10(%3)\n"
            "lqc2 $vf15, 0x20(%3)\n"
            "lqc2 $vf16, 0x30(%3)\n"
            "vmulax.xyzw $ACC, $vf9, $vf4x\n"
            "vmadday.xyzw $ACC, $vf10, $vf4y\n"
            "vmaddaz.xyzw $ACC, $vf11, $vf4z\n"
            "vmaddw.xyzw $vf4, $vf12, $vf4w\n"
            MUL_SCREEN
            "sqc2 $vf1, 0x0(%4)\n"
            "sqc2 $vf2, 0x10(%4)\n"
            "sqc2 $vf3, 0x20(%4)\n"
            "sqc2 $vf4, 0x30(%4)"
            :
            : "r"(scale), "r"(&ppvReferenceMatrixList[i]), "r"(ppvCameraMatrix), "r"(ppvScreenMatrix),
              "r"(ppvPObj->dmat));
        ((u_long128 *)shi0->mat)[0] = ((u_long128 *)ppvPObj->dmat)[0];
        ((u_long128 *)shi0->mat)[1] = ((u_long128 *)ppvPObj->dmat)[1];
        ((u_long128 *)shi0->mat)[2] = ((u_long128 *)ppvPObj->dmat)[2];
        ((u_long128 *)shi0->mat)[3] = ((u_long128 *)ppvPObj->dmat)[3];
        pppVu1ShapeDraw(sh, shi0);
        shi0->flag = 10;
        if (user_param == NULL) {
            continue;
        }
        if (ppvUserStopPartF) {
            continue;
        }
        if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
            continue;
        }
        user_param->SeqCt++;
        if (user_param->SeqCt >= ah->seqc) {
            if (seq->flag & 0x80) {
                user_param->SeqCt = 0;
            } else {
                user_param->Alpha = 0;
                user_param->SeqCt = 0;
            }
        }
    }
    pppWaitVif1();
    op_ztest_init();
}

void RyjMegaBirthUserCtrDrawShapeLine(int shape_id, pppCVECTOR *color, u_char fog, u_char near, float shape_scale,
                                      float length, u_int num) {
    OSHAPE_IN_VU1 *shi0;
    OSHAPE_IN_VU1 *shi1;
    OSHAPE_VU1 *sh;
    OANM_HEADER *ah;
    OANM_SEQ *seq;
    pppShapeSt *shape;
    float interval;
    int i;
    int j;
    USER_PARAM *user_param;
    float scale;
    float offset;
    int anm_seq;

    interval = length / num;
    pppStartVu1ShapeProg(fog);
    shi0 = pppGetVu1DrawSpad0();
    shi1 = pppGetVu1DrawSpad1();
    if (color->a >= 0xFF) {
        shi0->flag = shi1->flag = 2;
    } else {
        shi0->flag = shi1->flag = 0;
    }
    shi0->addz = shi1->addz = 0.0f;
    shi0->r = shi1->r = color->r;
    shi0->g = shi1->g = color->g;
    shi0->b = shi1->b = color->b;
    shi0->a = shi1->a = color->a;
    if (shape_id == 0xFFFF) {
        return;
    }
    for (i = 0; i < ppvReferenceMatrixNum; i++) {
        user_param = NULL;
        scale = 1.0f;
        offset = -(num * 0.5f * interval);
        anm_seq = 0;
        if (ppvReferenceMatrixList[i][3][3] != 1.0f) {
            user_param = (USER_PARAM *)&ppvReferenceMatrixList[i][3][3];
        }
        if (user_param != NULL && user_param->Alpha == 0) {
            continue;
        }
        for (j = 0; j < num; j++) {
            shi0 = pppGetVu1DrawSpad();
            if (user_param != NULL) {
                shi0->a = user_param->Alpha;
                anm_seq = user_param->SeqCt;
                scale = user_param->Scale;
                scale *= shape_scale * 0.1f;
            }
            shape = &ppvEnv->lpShape[shape_id];
            ah = (OANM_HEADER *)shape->shape;
            seq = &ah->seq[anm_seq];
            sh = (OSHAPE_VU1 *)((char *)ah + seq->sha);
            __asm__ __volatile__(
                "vsub.xyzw $vf4, $vf0, $vf0\n"
                "vadd.w $vf4, $vf4, $vf0\n"
                "vmr32.xyzw $vf3, $vf4\n"
                "vmr32.xyzw $vf2, $vf3\n"
                "vmr32.xyzw $vf1, $vf2\n"
                "qmtc2.ni %0, $vf7\n"
                "vmulx.xyz $vf1, $vf1, $vf7x\n"
                "vmulx.xyz $vf2, $vf2, $vf7x\n"
                "vmulx.xyz $vf3, $vf3, $vf7x\n"
                "lqc2 $vf8, 0x30(%1)\n"
                "vmove.w $vf8, $vf0\n"
                "qmtc2.ni %4, $vf5\n"
                "lqc2 $vf6, 0x20(%1)\n"
                "vmulx.xyz $vf6, $vf6, $vf5x\n"
                "vadd.xyzw $vf8, $vf8, $vf6\n"
                "vmove.xyzw $vf4, $vf8\n"
                "lqc2 $vf9, 0x0(%2)\n"
                "lqc2 $vf10, 0x10(%2)\n"
                "lqc2 $vf11, 0x20(%2)\n"
                "lqc2 $vf12, 0x30(%2)\n"
                "lqc2 $vf13, 0x0(%3)\n"
                "lqc2 $vf14, 0x10(%3)\n"
                "lqc2 $vf15, 0x20(%3)\n"
                "lqc2 $vf16, 0x30(%3)\n"
                "vmulax.xyzw $ACC, $vf9, $vf4x\n"
                "vmadday.xyzw $ACC, $vf10, $vf4y\n"
                "vmaddaz.xyzw $ACC, $vf11, $vf4z\n"
                "vmaddw.xyzw $vf4, $vf12, $vf4w\n"
                MUL_SCREEN
                "sqc2 $vf1, 0x0(%5)\n"
                "sqc2 $vf2, 0x10(%5)\n"
                "sqc2 $vf3, 0x20(%5)\n"
                "sqc2 $vf4, 0x30(%5)"
                :
                : "r"(scale), "r"(&ppvReferenceMatrixList[i]), "r"(ppvCameraMatrix), "r"(ppvScreenMatrix), "r"(offset), "r"(ppvPObj->dmat));
            offset += interval;
            ((u_long128 *)shi0->mat)[0] = ((u_long128 *)ppvPObj->dmat)[0];
            ((u_long128 *)shi0->mat)[1] = ((u_long128 *)ppvPObj->dmat)[1];
            ((u_long128 *)shi0->mat)[2] = ((u_long128 *)ppvPObj->dmat)[2];
            ((u_long128 *)shi0->mat)[3] = ((u_long128 *)ppvPObj->dmat)[3];
            pppVu1ShapeDraw(sh, shi0);
            shi0->flag = 10;
        }
        if (user_param == NULL) {
            continue;
        }
        if (ppvUserStopPartF) {
            continue;
        }
        if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
            continue;
        }
        user_param->SeqCt++;
        if (user_param->SeqCt >= ah->seqc) {
            if (seq->flag & 0x80) {
                user_param->SeqCt = 0;
            } else {
                user_param->Alpha = 0;
                user_param->SeqCt = 0;
            }
        }
    }
    pppWaitVif1();
    op_ztest_init();
}

void RyjMegaBirthUserCtrlDrawModel(int model_id, pppCVECTOR *color, u_char fog, u_char near) {
    OMD_IN mdi;
    pppModelSt *mdl;
    OMD_H *omd;
    int i;
    USER_PARAM *user_param;
    float scale;

    if (model_id == 0xFFFF) {
        return;
    }
    mdl = &ppvEnv->lpModel[model_id];
    omd = mdl->omd;
    mdi.pzmat = 0;
    mdi.lwmat = 0;
    mdi.lvmat = 0;
    mdi.lcmat = 0;
    mdi.pri = 0;
    *(int *)&mdi.ar = 0;
    mdi.va = ppvPObj->lpvert;
    mdi.na = ppvPObj->lpnorm;
    mdi.tmp_a = (char *)ppvVNWork;
    mdi.pmat = &ppvPObj->dmat;
    mdi.flag = 0x24;
    *(int *)&mdi.toff = 0;
    *(u_long *)&mdi.u = 0;
    *(int *)&mdi.ot = 0;
    if (!(omd->flag & 1)) {
        mdi.flag |= 8;
    }
    if (!(omd->flag & 2)) {
        mdi.flag |= 0x10;
    }
    if (fog) {
        mdi.flag |= 0x200;
    }
    if (near) {
        mdi.flag |= 0x100;
    }
    *(u_short *)&mdi.alpha = *(u_short *)&omd->alpha;
    mdi.r = color->r;
    mdi.g = color->g;
    mdi.b = color->b;
    mdi.a = color->a;
    for (i = 0; i < ppvReferenceMatrixNum; i++) {
        user_param = NULL;
        scale = 1.0f;
        if (ppvReferenceMatrixList[i][3][3] != 1.0f) {
            user_param = (USER_PARAM *)&ppvReferenceMatrixList[i][3][3];
        }
        if (user_param != NULL) {
            if (user_param->Alpha == 0) {
                continue;
            }
            mdi.a = user_param->Alpha;
            scale = user_param->Scale;
            scale *= 0.1f;
        }
        __asm__ __volatile__(
            "lqc2 $vf1, 0x0(%0)\n"
            "lqc2 $vf2, 0x10(%0)\n"
            "lqc2 $vf3, 0x20(%0)\n"
            "lqc2 $vf4, 0x30(%0)\n"
            "vmove.w $vf4, $vf0\n"
            "lqc2 $vf5, 0x0(%1)\n"
            "lqc2 $vf6, 0x10(%1)\n"
            "lqc2 $vf7, 0x20(%1)\n"
            "lqc2 $vf8, 0x30(%1)\n"
            "qmtc2.ni %2, $vf9\n"
            "vmulx.xyz $vf1, $vf1, $vf9x\n"
            "vmulx.xyz $vf2, $vf2, $vf9x\n"
            "vmulx.xyz $vf3, $vf3, $vf9x\n"
            "vmulax.xyzw $ACC, $vf5, $vf1x\n"
            "vmadday.xyzw $ACC, $vf6, $vf1y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf1z\n"
            "vmaddw.xyzw $vf1, $vf8, $vf1w\n"
            "vmulax.xyzw $ACC, $vf5, $vf2x\n"
            "vmadday.xyzw $ACC, $vf6, $vf2y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf2z\n"
            "vmaddw.xyzw $vf2, $vf8, $vf2w\n"
            "vmulax.xyzw $ACC, $vf5, $vf3x\n"
            "vmadday.xyzw $ACC, $vf6, $vf3y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf3z\n"
            "vmaddw.xyzw $vf3, $vf8, $vf3w\n"
            "vmulax.xyzw $ACC, $vf5, $vf4x\n"
            "vmadday.xyzw $ACC, $vf6, $vf4y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf4z\n"
            "vmaddw.xyzw $vf4, $vf8, $vf4w\n"
            "sqc2 $vf1, 0x0(%3)\n"
            "sqc2 $vf2, 0x10(%3)\n"
            "sqc2 $vf3, 0x20(%3)\n"
            "sqc2 $vf4, 0x30(%3)"
            :
            : "r"(&ppvReferenceMatrixList[i]), "r"(ppvSCMatrix), "r"(scale), "r"(ppvPObj->dmat));
        mdi.packet = ppvCurPrimp;
        ppvCurPrimp = op1_md_draw(omd, &mdi, mdl->nPacketSize);
    }
}
