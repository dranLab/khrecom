#include "md.h"
#include "draw_w.h"
#include "System/Debug.h"

int ot_omd_size[] = {
    20, 28, 32, 40, 24, 32, 40, 48,
    0, 0, 0, 0, 0, 0, 0, 0,
    28
};

static void set_ztest_off() {
    static u_long auqPacket[6] __attribute__((aligned(16))) = {
        0x70000002, 0, 0x1000000000008001, 0xE, 0x3000D, 0x47
    };

    sceGsSyncPath(0, 0);
    sceDmaSend(oa_dmagif, auqPacket);
}

static void set_invisible() {
    static u_long auqPacket[6] __attribute__((aligned(16))) = {
        0x70000002, 0, 0x1000000000008001, 0xE, 0x72001, 0x47
    };

    sceGsSyncPath(0, 0);
    sceDmaSend(oa_dmagif, auqPacket);
}

static void unset_test() {
    static u_long auqPacket[6] __attribute__((aligned(16))) = {
        0x70000002, 0, 0x1000000000008001, 0xE, 0, 0
    };

    auqPacket[4] = os_gs2_register.test;
    auqPacket[5] = os_gs2_register.test_code;
    sceGsSyncPath(0, 0);
    sceDmaSend(oa_dmagif, auqPacket);
}

void *op1_md_draw(OMD_H *mdh, OMD_IN *mdi, int nPacketSize) {
    void *pvReturn;

    if (mdi->flag & 4) {
        if (mdi->ztest_off) {
            set_ztest_off();
        } else if (mdi->invisible) {
            set_invisible();
        }
        if (mdi->flag & 0x300) {
            pvReturn = op_md_draw_path3_fog(mdh, mdi);
        } else {
            pvReturn = op_md_draw_path3(mdh, mdi);
        }
        if (mdi->invisible || mdi->ztest_off) {
            unset_test();
        }
    } else if (ppvCurPrimpLen + nPacketSize >= ppvPrimpLen) {
        TRACE("Effect::md.c MODEL: PACKET OVER! size:(%08x+%08x)/%08x\n", nPacketSize, ppvCurPrimpLen, ppvPrimpLen);
        return ppvCurPrimp;
    } else {
        if (mdi->flag & 0x80) {
            if (mdi->flag & 0x300) {
                pvReturn = op_md_ot8_fog(mdh, mdi);
            } else {
                pvReturn = op_md_ot8(mdh, mdi);
            }
        } else {
            if (mdi->flag & 0x300) {
                pvReturn = op_md_one8_fog(mdh, mdi);
            } else {
                pvReturn = op_md_one8(mdh, mdi);
            }
        }
        ppvCurPrimpLen += (int)pvReturn - (int)ppvCurPrimp;
    }
    return pvReturn;
}
// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .data
