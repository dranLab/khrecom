#include "pppRyjMegaBirthPrize.h"
#include "System/Graphic/Effect/libppp/draw/shape.h"

typedef struct { /* 0x10 */
    float X;
    float Y;
    float Z;
    u_char Type;
    u_char Alpha;
    u_char Scale;
    u_char SeqCt;
} PRIZE_DATA;

typedef struct _PRIZE_SHAPE { /* 0xc */
    int nShape;
    pppCVECTOR Col;
    float fScale;
} PRIZE_SHAPE;

void pppRyjDrawMegaBirthPrize(pppPObject *pobj, PRyjMegaBirthPrize *param, pppCtrlTable *ctbl) {
    PRIZE_SHAPE *prize_shape_list;
    sceVu0FMATRIX prize_type_mat_list[10];
    OSHAPE_IN_VU1 *shi0;
    OSHAPE_IN_VU1 *shi1;
    OSHAPE_VU1 *sh;
    OANM_HEADER *ah;
    OANM_SEQ *seq;
    pppShapeSt *shape;
    int i;
    int prize_type;
    int anm_seq;
    float scale;
    sceVu0FMATRIX *prize_type_mat;
    sceVu0FVECTOR pos;
    PRIZE_DATA *prize_data_list;
    PRIZE_DATA *prize_data;
    int n;

    prize_shape_list = (PRIZE_SHAPE *)&param->nShape0;
    prize_data_list = (PRIZE_DATA *)ppvReferencePosList;
    if (prize_data_list == NULL || ppvReferencePosNum <= 0) {
        return;
    }
    pppStartVu1ShapeProg(0);
    shi0 = pppGetVu1DrawSpad0();
    shi1 = pppGetVu1DrawSpad1();
    shi0->flag = shi1->flag = 0;
    for (i = 0; i < 10; i++) {
        if (prize_shape_list[i].Col.a >= 0xFF) {
            shi0->flag = shi1->flag = 2;
            break;
        }
    }
    shi0->addz = shi1->addz = 0.0f;
    for (i = 0; i < 10; i++) {
        PRIZE_SHAPE *ps = &prize_shape_list[i];

        sceVu0UnitMatrix(prize_type_mat_list[i]);
        sceVu0ScaleVector(prize_type_mat_list[i][0], prize_type_mat_list[i][0], ps->fScale * ppvParScl[0]);
        sceVu0ScaleVector(prize_type_mat_list[i][1], prize_type_mat_list[i][1], ps->fScale * ppvParScl[1]);
        sceVu0ScaleVector(prize_type_mat_list[i][2], prize_type_mat_list[i][2], ps->fScale * ppvParScl[2]);
    }
    for (i = 0; i < ppvReferencePosNum; i++) {
        prize_data = &((PRIZE_DATA *)ppvReferencePosList)[i];
        if (prize_data->Alpha == 0) {
            continue;
        }
        prize_type = prize_data->Type;
        n = prize_data->Alpha;
        if (prize_shape_list[prize_type].nShape == 0xFFFF) {
            continue;
        }
        shi0 = pppGetVu1DrawSpad();
        shape = &ppvEnv->lpShape[prize_shape_list[prize_type].nShape];
        prize_type_mat = &prize_type_mat_list[prize_type];
        shi0->r = prize_shape_list[prize_type].Col.r;
        shi0->g = prize_shape_list[prize_type].Col.g;
        shi0->b = prize_shape_list[prize_type].Col.b;
        shi0->a = n;
        ah = (OANM_HEADER *)shape->shape;
        if (prize_data->SeqCt == 0xFF) {
            OANM_SEQ *s = &ah->seq[ah->seqc - 1];

            if (s->flag & 0x80) {
                float r;

                __asm__ __volatile__("" : : : "memory");
                PPP_RAND_F(r);
                anm_seq = r * (ah->seqc - 1);
                if (anm_seq < 0 || anm_seq >= ah->seqc) {
                    anm_seq = 0;
                }
                prize_data->SeqCt = anm_seq;
            }
        }
        /* seq before scale: SeqCt loads before the division and Scale gets v1/f1. The pos loads as scalar
         * accesses: a struct member load through a pointer may pass the store of a scalar local (scale), a
         * scalar one may not, and the original loads X, Y, Z after the scale store. */
        seq = &ah->seq[prize_data->SeqCt];
        sh = (OSHAPE_VU1 *)((char *)ah + seq->sha);
        scale = prize_data->Scale / 255.0f;
        pos[0] = *(float *)((char *)prize_data + 0);
        pos[1] = *(float *)((char *)prize_data + 4);
        pos[2] = *(float *)((char *)prize_data + 8);
        pos[3] = 1.0f;
        __asm__ __volatile__(
            "lqc2 $vf1, 0x0(%0)\n"
            "lqc2 $vf2, 0x10(%0)\n"
            "lqc2 $vf3, 0x20(%0)\n"
            "lqc2 $vf4, 0x30(%0)\n"
            "lqc2 $vf5, 0x0(%2)\n"
            "lqc2 $vf6, 0x10(%2)\n"
            "lqc2 $vf7, 0x20(%2)\n"
            "lqc2 $vf8, 0x30(%2)\n"
            "lqc2 $vf9, 0x0(%3)\n"
            "lqc2 $vf10, 0x10(%3)\n"
            "lqc2 $vf11, 0x20(%3)\n"
            "lqc2 $vf12, 0x30(%3)\n"
            "lqc2 $vf13, 0x0(%1)\n"
            "lw $8, 0x0(%4)\n"
            "qmtc2.ni $8, $vf14\n"
            "vmulax.xyzw $ACC, $vf5, $vf13x\n"
            "vmadday.xyzw $ACC, $vf6, $vf13y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf13z\n"
            "vmaddw.xyzw $vf4, $vf8, $vf13w\n"
            "vmulx.xyzw $vf1, $vf1, $vf14x\n"
            "vmulx.xyzw $vf2, $vf2, $vf14x\n"
            "vmulx.xyzw $vf3, $vf3, $vf14x\n"
            "vmulax.xyzw $ACC, $vf9, $vf1x\n"
            "vmadday.xyzw $ACC, $vf10, $vf1y\n"
            "vmaddaz.xyzw $ACC, $vf11, $vf1z\n"
            "vmaddw.xyzw $vf1, $vf12, $vf1w\n"
            "vmulax.xyzw $ACC, $vf9, $vf2x\n"
            "vmadday.xyzw $ACC, $vf10, $vf2y\n"
            "vmaddaz.xyzw $ACC, $vf11, $vf2z\n"
            "vmaddw.xyzw $vf2, $vf12, $vf2w\n"
            "vmulax.xyzw $ACC, $vf9, $vf3x\n"
            "vmadday.xyzw $ACC, $vf10, $vf3y\n"
            "vmaddaz.xyzw $ACC, $vf11, $vf3z\n"
            "vmaddw.xyzw $vf3, $vf12, $vf3w\n"
            "vmulax.xyzw $ACC, $vf9, $vf4x\n"
            "vmadday.xyzw $ACC, $vf10, $vf4y\n"
            "vmaddaz.xyzw $ACC, $vf11, $vf4z\n"
            "vmaddw.xyzw $vf4, $vf12, $vf4w\n"
            "sqc2 $vf1, 0x0(%5)\n"
            "sqc2 $vf2, 0x10(%5)\n"
            "sqc2 $vf3, 0x20(%5)\n"
            "sqc2 $vf4, 0x30(%5)"
            :
            : "r"(prize_type_mat), "r"(pos), "r"(ppvCameraMatrix), "r"(ppvScreenMatrix), "r"(&scale), "r"(ppvPObj->dmat)
            : "$8");
        ((u_long128 *)shi0->mat)[0] = ((u_long128 *)ppvPObj->dmat)[0];
        ((u_long128 *)shi0->mat)[1] = ((u_long128 *)ppvPObj->dmat)[1];
        ((u_long128 *)shi0->mat)[2] = ((u_long128 *)ppvPObj->dmat)[2];
        ((u_long128 *)shi0->mat)[3] = ((u_long128 *)ppvPObj->dmat)[3];
        pppVu1ShapeDraw(sh, shi0);
        *(volatile int *)&shi0->flag = 10;
        if (ppvUserStopPartF) {
            continue;
        }
        if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
            continue;
        }
        prize_data->SeqCt++;
        if (prize_data->SeqCt >= ah->seqc) {
            if (seq->flag & 0x80) {
                prize_data->SeqCt = 0;
            } else {
                prize_data->SeqCt--;
            }
        }
    }
    pppWaitVif1();
    op_ztest_init();
}

void pppRyjMegaBirthPrizeCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    ppvMng->bBoolNoViewClip = 1;
}
