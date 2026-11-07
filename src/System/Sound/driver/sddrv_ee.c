#include "System/Sound/driver/sddrv_ee.h"
#include "include_asm.h"

typedef struct {
    void *paddr;
    unsigned int pid;
    int tid;
    unsigned int mode;
} sceSifRpcData;

typedef void (*sceSifEndFunc)(void *);

typedef struct {
    sceSifRpcData rpcd;
    unsigned int command;
    void *buff;
    void *gp;
    sceSifEndFunc func;
    void *para;
    void *serve;
} sceSifClientData;

int sceSifCheckStatRpc(sceSifRpcData *cd);
int sceSifCallRpc(sceSifClientData *cd, unsigned int rpc_number, unsigned int mode, void *send, int ssize,
                  void *receive, int rsize, sceSifEndFunc end_function, void *end_param);
int sceSifBindRpc(sceSifClientData *cd, unsigned int request, unsigned int mode);
void sceSifInitRpc(unsigned int mode);
void FlushCache(int operation);
char *strcpy(char *dst, const char *src);
void *memcpy(void *dst, const void *src, unsigned int n);

typedef struct {
    u_int id;
    char loop_flag;
    char vol;
    short int pan;
    short int wd_num;
    u_char tr_max;
    u_char wd_id;
    u_char priority;
    u_char attribute;
    char pause_flag;
    char sep_flag;
} SEQ_DATA_CHK;

typedef struct {
    u_short id;
    u_char read_flag2;
    char status;
    int time_count;
    char check_mark;
    char dummy[7];
} STR_DATA_CHK;

typedef struct {
    char mID[4];
    int mVers;
    int mReserved1;
    int mVagSize;
    int mSampleRate;
    int mLoopStart;
    int mLoopEnd;
    u_char mReserved2;
    u_char mChannels;
    u_char mUseVoice;
    u_char mVolume;
    char mFileName[32];
} SSL_VAG_STREAM_HEADER;

typedef struct {
    char Head[8];
    short int Sequence;
    short int MaxSe;
    int Size;
} SebTimeHEAD;

sceSifClientData cd;
u_int rpc_buf[8];
u_char ee_param[4096];
u_int *wd_param;
u_int wd_ee_param[16];
u_int *seb_param;
u_int seb_ee_param[16];
u_int *sep_param;
u_int sep_ee_param[4];
int sep_play_addr[16];
int sep_play_num[16];
u_char seb_id_cm;
SEQ_DATA_CHK *se_data_chk;
u_int *se_err_chk;
u_int se_data_ee_id[16];
char str_filename[4][256];
int str_fnpt;
STR_DATA_CHK *str_data_chk;
/* static STR_DATA_CHK str_data_ee[6]: the original .bss of this unit is 0x110 bytes (SoundMan's .bss follows at
   0x647300), more than this one static, so the .bss stays from the original assembly and this is its label. */
extern STR_DATA_CHK D_006471F0[6];
#define str_data_ee D_006471F0
u_int *str_err_chk;
u_int str_data_ee_id[6];
int fade_all_flag;
int output_mode;
int mvol_str_m;
int mvol_str_v;
int mvol_seq_se;
int mvol_seq_v;
int iop_str_addr;
int nowait_func_count;
u_int nowait_func_buff[1024];
u_int nowait_trans_buff[1024];

void TsPrintN(char *args, ...) {}

void TsSdDebugPrint() {
    nowait_func_buff[nowait_func_count++] = 3;
}

void TsSdSetWdMain(int flag, u_int addr, int wd_num, int wd_id, int size) {
    int num;

    for (num = 0; num < 16; num++) {
        if (wd_ee_param[num] == 9999) {
            wd_ee_param[num] = wd_num + 0x4000;
            TsPrintN("EE:  TsSdSetWdMain: addr=0x%08x, wd_num=%04d, wd_id=%d, size=%d\n", addr, wd_num, wd_id, size);
            TsPrintN("EE:  TsSdSetWdMain: num=%d\n", num);
            break;
        }
    }
    if (wd_num >= 200 && wd_num < 600) {
        wd_num += ((int *)addr)[4] << 24;
        TsPrintN("EE:  TsSdSetWdMain: area=%d\n", ((int *)addr)[4]);
    }
    if (flag == 0) {
        while (sceSifCheckStatRpc(&cd.rpcd)) {
        }
        rpc_buf[0] = addr;
        rpc_buf[1] = wd_num + (wd_id << 16);
        rpc_buf[2] = size;
        sceSifCallRpc(&cd, 2, 0, rpc_buf, 12, 0, 0, 0, 0);
    } else if (nowait_func_count < 0x3FC) {
        nowait_func_buff[nowait_func_count] = 0x33;
        nowait_func_buff[nowait_func_count + 1] = addr;
        nowait_func_buff[nowait_func_count + 2] = wd_num + (wd_id << 16);
        nowait_func_buff[nowait_func_count + 3] = size;
        nowait_func_count += 4;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSetWdMain\n");
    }
    FlushCache(0);
}

void TsSdSetWd(int flag, u_int addr) {
    int id;
    int num;
    int size;

    size = ((u_short *)addr)[2] + (((u_short *)addr)[3] << 16);
    num = ((u_short *)addr)[1];
    TsPrintN("EE:  TsSdSetWd: flag=%d\n", flag);
    if (num == 0) {
        id = 2;
    } else if (num == 1) {
        id = 3;
    } else if (num == 4) {
        id = 13;
    } else if (num >= 2 && num < 4) {
        id = 12;
    } else if (num >= 100 && num < 200) {
        id = 0;
    } else if (num >= 200 && num < 600) {
        id = 4;
    } else if (num >= 600 && num < 800) {
        id = 1;
    } else if (num >= 800 && num < 1000) {
        id = 4;
    } else if (num >= 1000 && num < 8000) {
        id = 4;
    } else {
        TsPrintN("EE:  TsSdSetWd: ERROR! wd_num=%04d\n", num);
        return;
    }
    TsSdSetWdMain(flag, addr, num, id, size);
}

void TsSdSetWd2(int flag, u_int addr, int wd_id) {
    TsSdSetWdMain(flag, addr, ((u_short *)addr)[1], wd_id, ((u_short *)addr)[2] | (((u_short *)addr)[3] << 16));
}

void TsSdSetWdClear(int wd_num) {
    TsPrintN("EE:  TsSdSetWdClear: wd_num=%04d\n", wd_num);
    if (nowait_func_count < 0x3FE) {
        nowait_func_buff[nowait_func_count] = 0x37;
        nowait_func_buff[nowait_func_count + 1] = wd_num;
        nowait_func_count += 2;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSetWdClear\n");
    }
}

int TsSdSetWdCheck(int wd_num) {
    int num;
    int check_iop;
    int check_ee;

    check_iop = -1;
    check_ee = -1;
    for (num = 0; num < 16; num++) {
        if ((wd_ee_param[num] & 0x3FFF) == wd_num) {
            check_ee = (wd_ee_param[num] >> 14) & 3;
            break;
        }
    }
    for (num = 0; num < 16; num++) {
        if ((wd_param[num] & 0x3FFF) == wd_num) {
            check_iop = (wd_param[num] >> 14) & 3;
            break;
        }
    }
    if (check_ee < 0 && check_iop < 0) {
        TsPrintN("EE:  TsSdSetWdCheck: No wd_num=%d\n", wd_num);
        return -1;
    }
    if (check_iop == 2 || check_iop == 0) {
        return 0;
    }
    return 1;
}

void TsSdSetWdClearAddr(int addr) {
    TsSdSetWdClear(((u_short *)addr)[1]);
}

int TsSdSetWdCheckAddr(int addr) {
    return TsSdSetWdCheck(((u_short *)addr)[1]);
}

int TsSdSetWdCheckArea(int wd_id) {
    int num;
    int check;

    check = 0;
    for (num = 0; num < 1; num++) {
        if ((wd_param[wd_id + num] >> 14) == 0) {
            check++;
        }
    }
    return check;
}

int TsSdGetWdOpenArea(int wd_id) {
    int num;
    int max;

    max = 1;
    for (num = 0; num < max; num++) {
        if ((wd_param[wd_id + num] >> 14) == 0) {
            break;
        }
    }
    if (num == max) {
        return -1;
    }
    return wd_id + num;
}

void TsSdSetSebMain(int flag, u_int addr, int seb_num, int seb_id, int size) {
    int num;

    TsPrintN("EE:  TsSdSetSebMain: addr=0x%08x, seb_num=%d, size=%d\n", addr, seb_num, size);
    for (num = 0; num < 16; num++) {
        if (seb_ee_param[num] == 9999) {
            seb_ee_param[num] = seb_num + 0x4000;
            break;
        }
    }
    if (flag == 0) {
        while (sceSifCheckStatRpc(&cd.rpcd)) {
        }
        rpc_buf[0] = addr;
        rpc_buf[1] = seb_num + (seb_id << 16);
        rpc_buf[2] = size;
        sceSifCallRpc(&cd, 3, 0, rpc_buf, 8, 0, 0, 0, 0);
    } else if (nowait_func_count < 0x3FC) {
        nowait_func_buff[nowait_func_count] = 0x34;
        nowait_func_buff[nowait_func_count + 1] = addr;
        nowait_func_buff[nowait_func_count + 2] = seb_num + (seb_id << 16);
        nowait_func_buff[nowait_func_count + 3] = size;
        nowait_func_count += 4;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSetSebMain\n");
    }
    FlushCache(0);
}

void TsSdSetSeb(int flag, u_int addr) {
    int id;
    int num;
    int size;

    size = ((u_short *)addr)[6] + (((u_short *)addr)[7] << 16);
    num = ((u_short *)addr)[4];
    TsPrintN("EE:  TsSdSetSeb: flag=%d\n", flag);
    if (num == 0) {
        id = 0;
    } else if (num == 1) {
        id = 1;
    } else if (num >= 2 && num < 4) {
        id = 3;
    } else if (num >= 100 && num < 200) {
        id = 2;
    } else if (num >= 200 && num < 600) {
        id = 4;
    } else if (num >= 800 && num < 1000) {
        id = 4;
    } else if (num >= 1000 && num < 8000) {
        id = 4;
    } else if (num >= 600 && num < 800) {
        switch (num % 5) {
        case 0:
        case 1:
            id = 12;
            break;
        case 2:
            id = 13;
            break;
        case 3:
            id = 14;
            break;
        case 4:
            id = 15;
            break;
        }
    } else {
        TsPrintN("EE:  TsSdSetSeb: ERROR! seb_num=%04d\n", num);
        return;
    }
    TsSdSetSebMain(flag, addr, num, id, size);
}

void TsSdSetSeb2(int flag, u_int addr, int seb_id) {
    TsSdSetSebMain(flag, addr, ((u_short *)addr)[4], seb_id, ((u_short *)addr)[6] | (((u_short *)addr)[7] << 16));
}

void TsSdSetSebClear(int seb_num) {
    TsPrintN("EE:  TsSdSetSebClear: seb_num=%d\n", seb_num);
    if (nowait_func_count < 0x3FE) {
        nowait_func_buff[nowait_func_count] = 0x38;
        nowait_func_buff[nowait_func_count + 1] = seb_num;
        nowait_func_count += 2;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSetSebClear\n");
    }
}

int TsSdSetSebCheck(int seb_num) {
    int num;
    int check_iop;
    int check_ee;

    check_iop = -1;
    check_ee = -1;
    for (num = 0; num < 16; num++) {
        if ((seb_ee_param[num] & 0x3FFF) == seb_num) {
            check_ee = (seb_ee_param[num] >> 14) & 3;
            break;
        }
    }
    for (num = 0; num < 16; num++) {
        if ((seb_param[num] & 0x3FFF) == seb_num) {
            check_iop = (seb_param[num] >> 14) & 3;
            break;
        }
    }
    if (check_ee < 0 && check_iop < 0) {
        TsPrintN("EE:  TsSdSetSebCheck: No seb_num=%d\n", seb_num);
        return -1;
    }
    if (check_iop == 2 || check_iop == 0) {
        TsPrintN("EE:  TsSdSetSebCheck: Completed! seb_num=%d\n", seb_num);
        return 0;
    }
    TsPrintN("EE:  TsSdSetSebCheck: Transposing... seb_num=%d\n", seb_num);
    return 1;
}

void TsSdSetSebClearAddr(int addr) {
    TsSdSetSebClear(((u_short *)addr)[4]);
}

int TsSdSetSebCheckAddr(int addr) {
    return TsSdSetSebCheck(((u_short *)addr)[4]);
}

int TsSdGetSebOpenArea(int seb_id) {
    int num;
    int max;

    max = 1;
    for (num = 0; num < max; num++) {
        if ((seb_param[seb_id + num] >> 14) == 0) {
            break;
        }
    }
    if (num == max) {
        return -1;
    }
    return seb_id + num;
}

int TsSdGetSebID(u_int seb_num, int check) {
    int num;

    if (check < 0) {
        for (num = 0; num < 16; num++) {
            if ((seb_param[num] & 0x3FFF) == seb_num) {
                break;
            }
        }
    } else {
        seb_num += check << 14;
        for (num = 0; num < 16; num++) {
            if (seb_param[num] == seb_num) {
                break;
            }
        }
    }
    if (num == 16) {
        return -1;
    }
    return num;
}

int TsSdGetSebNum(u_int addr) {
    return ((short *)addr)[4];
}

int TsSdGetSepNum(int addr) {
    return ((int *)addr)[2];
}

u_int TsSdGetSepNum2(int addr) {
    int num;

    for (num = 0; num < 16; num++) {
        if (addr == sep_play_addr[num]) {
            return TsSdGetSepNum(addr) + ((num + 200) << 24);
        }
    }
    return 0;
}

int TsSdSetSepParam(int sep_num, int check) {
    int num;

    for (num = 0; num < 4; num++) {
        if ((sep_ee_param[num] & 0x7FFFFF) == sep_num) {
            sep_ee_param[num] = sep_num + (check << 23);
            break;
        }
    }
    if (num == 4) {
        for (num = 0; num < 4; num++) {
            if (sep_ee_param[num] == 8000000) {
                sep_ee_param[num] = sep_num + (check << 23);
                break;
            }
        }
        if (num == 4) {
            TsPrintN("EE:  TsSdSetSepParam: ERROR! MEMORY FULL!!! sep_num=%d\n", sep_num);
            return 0;
        }
    }
    return 1;
}

void TsSdSetSep(int flag, u_int addr) {
    int sep_num;
    int sep_size;

    if (((u_char *)addr)[16] == 0) {
        TsPrintN("EE:  TsSdSetSep: ERROR! NO DATA!!!\n");
        return;
    }
    sep_num = TsSdGetSepNum(addr);
    sep_size = ((u_char *)addr)[12] + (((u_char *)addr)[13] << 8);
    if (TsSdSetSepParam(sep_num, 2) == 0) {
        return;
    }
    if (flag == 0) {
        while (sceSifCheckStatRpc(&cd.rpcd)) {
        }
        rpc_buf[0] = addr;
        rpc_buf[1] = sep_num;
        rpc_buf[2] = sep_size;
        sceSifCallRpc(&cd, 0x35, 0, rpc_buf, 12, 0, 0, 0, 0);
    } else if (nowait_func_count < 0x3FC) {
        nowait_func_buff[nowait_func_count] = 0x35;
        nowait_func_buff[nowait_func_count + 1] = addr;
        nowait_func_buff[nowait_func_count + 2] = sep_num;
        nowait_func_buff[nowait_func_count + 3] = sep_size;
        nowait_func_count += 4;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSetSep\n");
    }
    FlushCache(0);
}

void TsSdSetSepClear(int addr) {
    if (nowait_func_count < 0x3FE) {
        nowait_func_buff[nowait_func_count++] = 0x39;
        nowait_func_buff[nowait_func_count++] = TsSdGetSepNum(addr);
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSetSepClear\n");
    }
}

void TsSdSetSepClearAll() {
    int num;

    for (num = 0; num < 4; num++) {
        sep_ee_param[num] = 8000000;
        sep_play_num[num] = 0;
        sep_play_addr[num] = 0;
    }
    if (nowait_func_count < 0x3FE) {
        nowait_func_buff[nowait_func_count] = 0x39;
        nowait_func_buff[nowait_func_count + 1] = 0x1000000;
        nowait_func_count += 2;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSetSepClearAll\n");
    }
}

int TsSdSetSepCheck(int addr) {
    int num;
    int check_iop;
    int check_ee;
    int sep_num;

    check_iop = -1;
    check_ee = -1;
    sep_num = TsSdGetSepNum(addr);
    for (num = 0; num < 4; num++) {
        if ((sep_ee_param[num] & 0x7FFFFF) == sep_num) {
            check_ee = (sep_ee_param[num] >> 23) & 3;
            break;
        }
    }
    for (num = 0; num < 4; num++) {
        if ((sep_param[num] & 8000000) == sep_num) {
            check_iop = (sep_param[num] >> 23) & 3;
            break;
        }
    }
    if (check_ee < 0 && check_iop < 0) {
        TsPrintN("EE:  TsSdSetSepCheck: No sep_num=%d\n", sep_num);
        return -1;
    }
    if (check_iop == 2 || check_iop == 0) {
        TsPrintN("EE:  TsSdSetSepCheck: Completed! sep_num=%d\n", sep_num);
        return 0;
    }
    TsPrintN("EE:  TsSdSetSepCheck: Transposing... sep_num=%d\n", sep_num);
    return 1;
}

void TsSdSetRevDepth(int depth) {
    if (nowait_func_count < 0x3FE) {
        nowait_func_buff[nowait_func_count] = 6;
        nowait_func_buff[nowait_func_count + 1] = depth;
        nowait_func_count += 2;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSetRevDepth\n");
    }
}

void TsSdResetRevDepth() {
    if (nowait_func_count < 0x3FE) {
        nowait_func_buff[nowait_func_count] = 6;
        nowait_func_buff[nowait_func_count + 1] = 20;
        nowait_func_count += 2;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdResetRevDepth\n");
    }
}

void TsSdSebPlaySI2(int seb_id, int num, int vol, int pan, int se_id) {
    int data;
    int id;

    TsPrintN("EE:  TsSdSebPlaySI2: seb_id=%d, num=%d, se_id=%d\n", seb_id, num, se_id);
    data = seb_id * 1000 + num + (se_id << 24);
    if (nowait_func_count < 0x3FD) {
        nowait_func_buff[nowait_func_count] = 0x1F;
        nowait_func_buff[nowait_func_count + 1] = data;
        nowait_func_buff[nowait_func_count + 2] = vol + (pan << 16);
        nowait_func_count += 3;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSebPlaySI2\n");
    }
    for (id = 0; id < 16; id++) {
        if (se_data_ee_id[id] == 0) {
            se_data_ee_id[id] = data;
            break;
        }
    }
    FlushCache(0);
    TsPrintN("EE:  TsSdSebPlaySI2: se_data_ee_id[%d]=%d\n", id, se_data_ee_id[id]);
}

void TsSdSebStopSI2(int seb_id, int num, int se_id) {
    int data;
    int id;

    TsPrintN("EE:  TsSdSebStopSI2: seb_id=%d, num=%d, se_id=%d\n", seb_id, num, se_id);
    data = seb_id * 1000 + num + (se_id << 24);
    if (nowait_func_count < 0x3FE) {
        nowait_func_buff[nowait_func_count] = 0x20;
        nowait_func_buff[nowait_func_count + 1] = data;
        nowait_func_count += 2;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSebStopSI2\n");
    }
    for (id = 0; id < 16; id++) {
        if (se_data_ee_id[id] == data) {
            se_data_ee_id[id] = 0;
        }
    }
}

void TsSdSebStartSI2(int seb_id, int num, int se_id) {
    TsPrintN("EE:  TsSdSebStartSI2: seb_id=%d, num=%d\n", seb_id, num);
    if (nowait_func_count < 0x3FE) {
        nowait_func_buff[nowait_func_count] = 0x21;
        nowait_func_buff[nowait_func_count + 1] = seb_id * 1000 + num + (se_id << 24);
        nowait_func_count += 2;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSebStartSI2\n");
    }
}

void TsSdSebPauseSI2(int seb_id, int num, int se_id) {
    TsPrintN("EE:  TsSdSebPauseSI2: seb_id=%d, num=%d\n", seb_id, num);
    if (nowait_func_count < 0x3FE) {
        nowait_func_buff[nowait_func_count] = 0x22;
        nowait_func_buff[nowait_func_count + 1] = seb_id * 1000 + num + (se_id << 24);
        nowait_func_count += 2;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSebPauseSI2\n");
    }
}

void TsSdSebSetVPPSI2(int seb_id, int num, int vol, int pan, int pitch, int se_id) {
    if (nowait_func_count < 0x3FD) {
        nowait_func_buff[nowait_func_count] = 0x23;
        nowait_func_buff[nowait_func_count + 1] = seb_id * 1000 + num + (se_id << 24);
        nowait_func_buff[nowait_func_count + 2] = (vol & 0xFF) + (pan << 16) + ((pitch & 0xFF) << 8);
        nowait_func_count += 3;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSebSetVPSI2\n");
    }
}

void TsSdSebSetVPSI2(int seb_id, int num, int vol, int pan, int se_id) {
    if (nowait_func_count < 0x3FD) {
        nowait_func_buff[nowait_func_count] = 0x23;
        nowait_func_buff[nowait_func_count + 1] = seb_id * 1000 + num + (se_id << 24);
        nowait_func_buff[nowait_func_count + 2] = vol + (pan << 16) + 0xFF00;
        nowait_func_count += 3;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSebSetVPSI2\n");
    }
}

void TsSdSebSetVolSI2(int seb_id, int num, int vol, int se_id) {
    if (nowait_func_count < 0x3FC) {
        nowait_func_buff[nowait_func_count] = 0x23;
        nowait_func_buff[nowait_func_count + 1] = seb_id * 1000 + num + (se_id << 24);
        nowait_func_buff[nowait_func_count + 2] = vol + 0xFFFF0000 + 0xFF00;
        nowait_func_count += 3;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSebSetVolSI2\n");
    }
}

void TsSdSebSetPanSI2(int seb_id, int num, int pan, int se_id) {
    if (nowait_func_count < 0x3FD) {
        nowait_func_buff[nowait_func_count] = 0x23;
        nowait_func_buff[nowait_func_count + 1] = seb_id * 1000 + num + (se_id << 24);
        nowait_func_buff[nowait_func_count + 2] = (pan << 16) | 0xFFFF;
        nowait_func_count += 3;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSebSetPanSI2\n");
    }
}

void TsSdSebFadeVPSI2(int seb_id, int num, int vol, int pan, int step, int flag, int turn, int se_id) {
    if (nowait_func_count < 0x3FC) {
        nowait_func_buff[nowait_func_count] = 0x24;
        nowait_func_buff[nowait_func_count + 1] = seb_id * 1000 + num + (se_id << 24);
        nowait_func_buff[nowait_func_count + 2] = vol + (pan << 16);
        nowait_func_buff[nowait_func_count + 3] = step + (flag << 23) + (turn << 24);
        nowait_func_count += 4;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSebFadeVPSI2\n");
    }
}

void TsSdSebFadeVolSI2(int seb_id, int num, int vol, int step, int flag, int se_id) {
    if (nowait_func_count < 0x3FC) {
        nowait_func_buff[nowait_func_count] = 0x24;
        nowait_func_buff[nowait_func_count + 1] = seb_id * 1000 + num + (se_id << 24);
        nowait_func_buff[nowait_func_count + 2] = vol + 0xFFFF0000;
        nowait_func_buff[nowait_func_count + 3] = step + (flag << 23);
        nowait_func_count += 4;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSebFadeVolSI2\n");
    }
}

void TsSdSebFadePanSI2(int seb_id, int num, int pan, int step, int turn, int se_id) {
    if (nowait_func_count < 0x3FC) {
        nowait_func_buff[nowait_func_count] = 0x24;
        nowait_func_buff[nowait_func_count + 1] = seb_id * 1000 + num + (se_id << 24);
        nowait_func_buff[nowait_func_count + 2] = (pan << 16) | 0xFFFF;
        nowait_func_buff[nowait_func_count + 3] = step + (turn << 24);
        nowait_func_count += 4;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSebFadePanSI2\n");
    }
}

int TsSdSebPlayCheckSI2(int seb_id, int num, int se_id) {
    return TsSdSePlayCheck(seb_id * 1000 + num + (se_id << 24));
}

void TsSdSebPlaySI(int seb_id, int num, int vol, int pan) {
    TsSdSebPlaySI2(seb_id, num, vol, pan, 0);
}

void TsSdSebStopSI(int seb_id, int num) {
    TsSdSebStopSI2(seb_id, num, 0);
}

void TsSdSebStartSI(int seb_id, int num) {
    TsSdSebStartSI2(seb_id, num, 0);
}

void TsSdSebPauseSI(int seb_id, int num) {
    TsSdSebPauseSI2(seb_id, num, 0);
}

void TsSdSebSetVPSI(int seb_id, int num, int vol, int pan) {
    TsSdSebSetVPSI2(seb_id, num, vol, pan, 0);
}

void TsSdSebSetVolSI(int seb_id, int num, int vol) {
    TsSdSebSetVolSI2(seb_id, num, vol, 0);
}

void TsSdSebSetPanSI(int seb_id, int num, int pan) {
    TsSdSebSetPanSI2(seb_id, num, pan, 0);
}

void TsSdSebFadeVPSI(int seb_id, int num, int vol, int pan, int step, int flag, int turn) {
    TsSdSebFadeVPSI2(seb_id, num, vol, pan, step, flag, turn, 0);
}

void TsSdSebFadeVolSI(int seb_id, int num, int vol, int step, int flag) {
    TsSdSebFadeVolSI2(seb_id, num, vol, step, flag, 0);
}

void TsSdSebFadePanSI(int seb_id, int num, int pan, int step, int turn) {
    TsSdSebFadePanSI2(seb_id, num, pan, step, turn, 0);
}

int TsSdSebPlayCheckSI(int seb_id, int num) {
    return TsSdSePlayCheck(seb_id * 1000 + num);
}

void TsSdSebPlaySN(int seb_num, int num, int vol, int pan, int se_id) {
    TsSdSebPlaySI2(TsSdGetSebID(seb_num, -1), num, vol, pan, se_id);
}

void TsSdSebStopSN(int seb_num, int num, int se_id) {
    TsSdSebStopSI2(TsSdGetSebID(seb_num, -1), num, se_id);
}

void TsSdSebStartSN(int seb_num, int num, int se_id) {
    TsSdSebStartSI2(TsSdGetSebID(seb_num, -1), num, se_id);
}

void TsSdSebPauseSN(int seb_num, int num, int se_id) {
    TsSdSebPauseSI2(TsSdGetSebID(seb_num, -1), num, se_id);
}

void TsSdSebSetVPSN(int seb_num, int num, int vol, int pan, int se_id) {
    TsSdSebSetVPSI2(TsSdGetSebID(seb_num, -1), num, vol, pan, se_id);
}

void TsSdSebFadeVPSN(int seb_num, int num, int vol, int pan, int step, int flag, int turn, int se_id) {
    TsSdSebFadeVPSI2(TsSdGetSebID(seb_num, -1), num, vol, pan, step, flag, turn, se_id);
}

void TsSdSebFadeVolSN(int seb_num, int num, int vol, int step, int flag, int se_id) {
    TsSdSebFadeVolSI2(TsSdGetSebID(seb_num, -1), num, vol, step, flag, se_id);
}

int TsSdSebPlayCheckSN(int seb_num, int num, int se_id) {
    return TsSdSePlayCheck(num + TsSdGetSebID(seb_num, -1) * 1000 + (se_id << 24));
}

int TsSdSebTimeCheck(int *buf, int num) {
    if (((SebTimeHEAD *)buf)->MaxSe < num) {
        return 0;
    }
    return buf[num + 4];
}

void TsSdSeStopAll() {
    int data;

    TsPrintN("EE:  TsSdSeStopAll\n");
    if (nowait_func_count < 0x3FE) {
        nowait_func_buff[nowait_func_count] = 0x20;
        nowait_func_buff[nowait_func_count + 1] = 0xFFFFFFFF;
        nowait_func_count += 2;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSeStopAll\n");
    }
    for (data = 15; data >= 0; data--) {
        se_data_ee_id[data] = 0;
    }
}

int TsSdSePlayCheck2(int num) {
    int data;

    for (data = 0; data < 16; data++) {
        if (se_data_chk[data].id == num || se_data_ee_id[data] == num) {
            return 1;
        }
    }
    return 0;
}

int TsSdSePlayCheck(int num) {
    int data;

    if (num == 0) {
        for (data = 0; data < 16; data++) {
            if (se_data_chk[data].id != 0 || se_data_ee_id[data] != 0) {
                return 1;
            }
        }
    } else if (num % 1000 == 0) {
        num /= 1000;
        for (data = 0; data < 16; data++) {
            if ((se_data_chk[data].id & 0x7FFFFF) / 1000 == num || (se_data_ee_id[data] & 0x7FFFFF) / 1000 == num) {
                return 1;
            }
        }
    } else {
        return TsSdSePlayCheck2(num);
    }
    return 0;
}

void TsSdSepPlayDBG(int addr, int vol, int pan, int se_id, int mtrack, int dtime) {
    int sep_num;
    int sep_size;
    int num;
    int check;
    int id;
    int wd_num;

    check = 1;
    if (((u_char *)addr)[16] == 0) {
        return;
    }
    wd_num = ((u_char *)addr)[17] + (((u_char *)addr)[18] << 8);
    if (TsSdSetWdCheck(wd_num)) {
        TsPrintN("EE:  TsSdSepPlayDBG: ERROR! NO WD!!! wave%04d.wd\n", wd_num);
        return;
    }
    sep_num = TsSdGetSepNum(addr);
    {
        /* MATCH: the two bytes pinned to a0/v0: v0 keeps byte 13 after the move of sep_num in both schedulers, the
           original's temporaries for the bytes and the sep_param scan loop (issue-order pins) */
        register int lo asm("$4") = ((u_char *)addr)[12];
        register int hi asm("$2") = ((u_char *)addr)[13];
        sep_size = lo + (hi << 8);
    }
    for (num = 0; num < 4; num++) {
        if (((sep_param[num] >> 23) & 3) != 2) {
            check = 0;
        }
    }
    if (check) {
        TsPrintN("EE:  TsSdSepPlayDBG: @@@@@@@@@@ SEP PLAY OVER!!! @@@@@@@@@@ sep_num=%d\n", sep_num);
        return;
    }
    TsSdSepPlayCheckAddr();
    if (TsSdSetSepParam(sep_num, 1) == 0) {
        return;
    }
    se_id += 200;
    if (se_id >= 256) {
        se_id = 255;
    }
    sep_num += se_id << 24;
    for (id = 0; id < 16; id++) {
        if (se_data_ee_id[id] == 0) {
            se_data_ee_id[id] = sep_num;
            TsPrintN("EE:  TsSdSepPlayDBG: se_data_ee_id[%d]=%d\n", id, sep_num);
            break;
        }
    }
    if (nowait_func_count < 0x3F9) {
        nowait_func_buff[nowait_func_count] = 0x28;
        nowait_func_buff[nowait_func_count + 1] = addr;
        nowait_func_buff[nowait_func_count + 2] = sep_num;
        nowait_func_buff[nowait_func_count + 3] = sep_size;
        nowait_func_buff[nowait_func_count + 4] = vol + (pan << 16);
        nowait_func_buff[nowait_func_count + 5] = mtrack;
        nowait_func_buff[nowait_func_count + 6] = dtime;
        nowait_func_count += 7;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSepPlayDBG\n");
    }
}

void TsSdSepPlay(int addr, int vol, int pan) {
    TsSdSepPlayDBG(addr, vol, pan, 0, 0, 0);
}

void TsSdSepPlay2(int addr, int vol, int pan, int se_id) {
    TsPrintN("EE:  TsSdSepPlay2: sep_num=%d, vol=%d, pan=%d, se_id=%d\n", TsSdGetSepNum(addr), vol, pan, se_id);
    TsSdSepPlayDBG(addr, vol, pan, se_id, 0, 0);
}

void TsSdSepStop(int addr) {
    u_int sep_num;

    sep_num = TsSdGetSepNum2(addr);
    TsPrintN("EE:  TsSdSepStop: sep_num=%d, se_id=%d\n", sep_num & 0xFFFFF, sep_num >> 24);
    if (sep_num) {
        nowait_func_buff[nowait_func_count++] = 0x2A;
        nowait_func_buff[nowait_func_count++] = addr;
        TsSdSebStopSI2(0, sep_num, 0);
    }
}

void TsSdSepStop2(int addr, int se_id) {
    u_int sep_num;

    sep_num = TsSdGetSepNum(addr);
    TsPrintN("EE:  TsSdSepStop2: sep_num=%d, se_id=%d\n", sep_num, se_id);
    if (sep_num) {
        nowait_func_buff[nowait_func_count++] = 0x2A;
        nowait_func_buff[nowait_func_count++] = addr;
        se_id += 200;
        if (se_id >= 256) {
            se_id = 255;
        }
        TsSdSebStopSI2(0, sep_num, se_id);
    }
}

void TsSdSepStart(int addr) {
    u_int sep_num;

    sep_num = TsSdGetSepNum2(addr);
    if (sep_num) {
        TsSdSebStartSI(0, sep_num);
    }
}

void TsSdSepPause(int addr) {
    u_int sep_num;

    sep_num = TsSdGetSepNum2(addr);
    if (sep_num) {
        TsSdSebPauseSI(0, sep_num);
    }
}

void TsSdSepSetVPP(int addr, int vol, int pan, int pitch) {
    u_int sep_num;

    sep_num = TsSdGetSepNum2(addr);
    if (sep_num) {
        TsSdSebSetVPPSI2(0, sep_num, vol, pan, pitch, 0);
    }
}

void TsSdSepSetVPP2(int addr, int vol, int pan, int pitch, int se_id) {
    u_int sep_num;

    sep_num = TsSdGetSepNum(addr);
    if (sep_num) {
        se_id += 200;
        if (se_id >= 256) {
            se_id = 255;
        }
        TsSdSebSetVPPSI2(0, sep_num, vol, pan, pitch, se_id);
    }
}

void TsSdSepSetVP(int addr, int vol, int pan) {
    u_int sep_num;

    sep_num = TsSdGetSepNum2(addr);
    if (sep_num) {
        TsSdSebSetVPSI(0, sep_num, vol, pan);
    }
}

void TsSdSepSetVol(int addr, int vol) {
    u_int sep_num;

    sep_num = TsSdGetSepNum2(addr);
    if (sep_num) {
        TsSdSebSetVolSI(0, sep_num, vol);
    }
}

void TsSdSepSetPan(int addr, int pan) {
    u_int sep_num;

    sep_num = TsSdGetSepNum2(addr);
    if (sep_num) {
        TsSdSebSetPanSI(0, sep_num, pan);
    }
}

int TsSdSepGetTime(int addr) {
    u_short *sep_buf;
    int time;

    sep_buf = (u_short *)addr;
    if (sep_buf[3] == 0xFFFF) {
        time = -1;
    } else {
        time = (sep_buf[3] * 5) >> 3;
    }
    return time;
}

void TsSdSepFadeVP(int addr, int vol, int pan, int step, int flag, int turn) {
    u_int sep_num;

    sep_num = TsSdGetSepNum2(addr);
    TsPrintN("EE:  TsSdSepFadeVP: sep_num=%d, se_id=%d, vol=%d, pan=%d\n", sep_num & 0xFFFFF, sep_num >> 24, vol, pan);
    if (sep_num) {
        TsSdSebFadeVPSI(0, sep_num, vol, pan, step, flag, turn);
    }
}

void TsSdSepFadeVol(int addr, int vol, int step, int flag) {
    u_int sep_num;

    sep_num = TsSdGetSepNum2(addr);
    TsPrintN("EE:  TsSdSepFadeVol: sep_num=%d, se_id=%d, vol=%d, step=%d\n", sep_num & 0xFFFFF, sep_num >> 24, vol, step);
    if (sep_num) {
        if (nowait_func_count < 0x3FB) {
            nowait_func_buff[nowait_func_count] = 0x2B;
            nowait_func_buff[nowait_func_count + 1] = sep_num;
            nowait_func_buff[nowait_func_count + 2] = vol + 0xFFFF0000;
            nowait_func_buff[nowait_func_count + 3] = step + (flag << 23);
            nowait_func_buff[nowait_func_count + 4] = addr;
            nowait_func_count += 5;
        } else {
            TsPrintN("EE: nowait_func_buff OVER!!: TsSdSebFadeVolSI2\n");
        }
    }
}

void TsSdSepFadeVol2(int addr, int vol, int step, int flag, int se_id) {
    u_int sep_num;

    sep_num = TsSdGetSepNum(addr);
    TsPrintN("EE:  TsSdSepFadeVol2: sep_num=%d, se_id=%d, vol=%d, step=%d\n", sep_num, se_id, vol, step);
    if (sep_num) {
        se_id += 200;
        if (se_id >= 256) {
            se_id = 255;
        }
        if (nowait_func_count < 0x3FB) {
            nowait_func_buff[nowait_func_count] = 0x2B;
            nowait_func_buff[nowait_func_count + 1] = sep_num + (se_id << 24);
            nowait_func_buff[nowait_func_count + 2] = vol + 0xFFFF0000;
            nowait_func_buff[nowait_func_count + 3] = step + (flag << 23);
            nowait_func_buff[nowait_func_count + 4] = addr;
            nowait_func_count += 5;
        } else {
            TsPrintN("EE: nowait_func_buff OVER!!: TsSdSebFadeVolSI2\n");
        }
    }
}

void TsSdSepFadePan(int addr, int pan, int step, int turn) {
    u_int sep_num;

    sep_num = TsSdGetSepNum2(addr);
    TsPrintN("EE:  TsSdSepFadePan: sep_num=%d, se_id=%d, pan=%d\n", sep_num & 0xFFFFF, sep_num >> 24, pan);
    if (sep_num) {
        TsSdSebFadePanSI(0, sep_num, pan, step, turn);
    }
}

int TsSdSepPlayCheck(int addr) {
    int sep_num;

    sep_num = TsSdGetSepNum2(addr);
    if (sep_num == 0) {
        return 0;
    }
    return TsSdSePlayCheck2(sep_num);
}

int TsSdSepPlayCheck2(int addr, int se_id) {
    int sep_num;

    sep_num = TsSdGetSepNum(addr);
    if (sep_num == 0) {
        return 0;
    }
    se_id += 200;
    if (se_id >= 256) {
        se_id = 255;
    }
    return TsSdSePlayCheck2(sep_num + (se_id << 24));
}

int TsSdSepPlayCheckAddr() {
    int num;

    for (num = 0; num < 4; num++) {
        if (TsSdSePlayCheck2(sep_play_num[num]) == 0) {
            sep_play_num[num] = 0;
            sep_play_addr[num] = 0;
        }
    }
    return (int)sep_play_addr;
}

void TsSdSepClearAddr() {
    int num;

    TsPrintN("EE:  TsSdSepClearAddr\n");
    for (num = 0; num < 4; num++) {
        sep_play_num[num] = 0;
        sep_play_addr[num] = 0;
    }
}

void TsSdStrPlayMusicAddr(u_int addr, int play_sect) {
    int num;

    TsPrintN("EE:  TsSdStrPlayMusicAddr: addr=%08x\n", addr);
    if (nowait_func_count < 0x3FC) {
        nowait_func_buff[nowait_func_count] = 0x16;
        nowait_func_buff[nowait_func_count + 1] = addr;
        nowait_func_count += 2;
        nowait_func_buff[nowait_func_count] = ((u_short *)addr)[6] + (((u_short *)addr)[7] << 16);
        nowait_func_buff[nowait_func_count + 1] = play_sect;
        nowait_func_count += 2;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrPlayMusicAddr\n");
    }
    for (num = 0; num < 6; num++) {
        if (str_data_ee[num].id == 0) {
            str_data_ee[num].id = ((u_short *)addr)[7];
            break;
        }
    }
}

void TsSdStrPlayMusic(int sect, int play_sect, char *filename, int id) {
    int num;

    TsPrintN("EE:  TsSdStrPlayMusic: sect=%d, play_sect=%d, id=%d\n", sect, play_sect, id);
    TsPrintN("EE:  TsSdStrPlayMusic: %s\n", filename);
    strcpy(str_filename[str_fnpt], filename);
    if (nowait_func_count < 0x3FB) {
        nowait_func_buff[nowait_func_count] = 0xB;
        nowait_func_buff[nowait_func_count + 1] = sect;
        nowait_func_buff[nowait_func_count + 2] = (u_int)str_filename[str_fnpt];
        nowait_func_buff[nowait_func_count + 3] = id;
        nowait_func_buff[nowait_func_count + 4] = play_sect;
        nowait_func_count += 5;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrPlayMusic\n");
    }
    str_fnpt++;
    if (str_fnpt >= 4) {
        str_fnpt = 0;
    }
    for (num = 0; num < 6; num++) {
        if (str_data_ee[num].id == 0) {
            str_data_ee[num].id = id;
            break;
        }
    }
}

void TsSdStrPlayMusicCrossFade(int sect, int play_sect, char *filename, int id, u_int addr, int read_sect, int fade_step) {
    int num;

    TsPrintN("EE:  TsSdStrPlayMusicCrossFade: sect=%d, play_sect=%d, id=%d\n", sect, play_sect, id);
    TsPrintN("EE:  TsSdStrPlayMusicCrossFade: addr=0x%08x, read_sect=%d\n", addr, read_sect);
    TsPrintN("EE:  TsSdStrPlayMusicCrossFade: %s\n", filename);
    strcpy(str_filename[str_fnpt], filename);
    if (nowait_func_count < 0x3F9) {
        nowait_func_buff[nowait_func_count] = 0x17;
        nowait_func_buff[nowait_func_count + 1] = sect;
        nowait_func_buff[nowait_func_count + 2] = (u_int)str_filename[str_fnpt];
        nowait_func_buff[nowait_func_count + 3] = id;
        nowait_func_buff[nowait_func_count + 4] = play_sect;
        nowait_func_buff[nowait_func_count + 5] = addr;
        nowait_func_buff[nowait_func_count + 6] = (read_sect << 16) + fade_step;
        nowait_func_count += 7;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrPlayMusicCrossFade\n");
    }
    str_fnpt++;
    if (str_fnpt >= 4) {
        str_fnpt = 0;
    }
    for (num = 0; num < 6; num++) {
        if (str_data_ee[num].id == 0) {
            str_data_ee[num].id = id;
            break;
        }
    }
}

void TsSdStrPlayVoice(u_int addr) {
    int num;
    u_short *data;

    data = (u_short *)addr;
    if (nowait_func_count < 0x3FD) {
        nowait_func_buff[nowait_func_count] = 0x15;
        nowait_func_buff[nowait_func_count + 1] = addr;
        nowait_func_count += 2;
        nowait_func_buff[nowait_func_count++] = data[6] + (data[7] << 16);
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrPlayVoice\n");
    }
    TsPrintN("EE:  TsSdStrPlayVoice: id=%d\n", data[7]);
    for (num = 0; num < 6; num++) {
        if (str_data_ee[num].id == 0) {
            str_data_ee[num].id = data[7] + 0x8000;
            break;
        }
    }
}

void TsSdStrStop(int type, int id) {
    int num;

    TsPrintN("EE:  TsSdStrStop: type=%d, id=%d\n", type, id);
    if (type & 1) {
        if (nowait_func_count < 0x3FE) {
            nowait_func_buff[nowait_func_count] = 0xC;
            nowait_func_buff[nowait_func_count + 1] = id;
            nowait_func_count += 2;
        } else {
            TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrStop SD_STR_TYPE_MUSIC\n");
        }
        for (num = 0; num < 6; num++) {
            if (str_data_ee[num].id == id) {
                str_data_ee[num].id = 0;
            }
        }
    }
    if (type & 2) {
        id += 0x8000;
        if (nowait_func_count < 0x3FE) {
            nowait_func_buff[nowait_func_count] = 0xC;
            nowait_func_buff[nowait_func_count + 1] = id;
            nowait_func_count += 2;
        } else {
            TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrStop SD_STR_TYPE_VOICE\n");
        }
        for (num = 0; num < 6; num++) {
            if (str_data_ee[num].id == id) {
                str_data_ee[num].id = 0;
            }
        }
    }
}

void TsSdStrFadeVol(int type, int id, int vol, int step, int flag) {
    vol = vol * 100 / 127;
    TsPrintN("EE:  TsSdStrFadeVol: type=%d, id=%d, vol=%d%%, step=%d\n", type, id, vol, step);
    if (step <= 0) {
        step = 1;
    }
    if (type & 1) {
        if (nowait_func_count < 0x3FC) {
            nowait_func_buff[nowait_func_count] = 0x12;
            nowait_func_buff[nowait_func_count + 1] = id;
            nowait_func_buff[nowait_func_count + 2] = vol + 0xFFFF0000;
            nowait_func_buff[nowait_func_count + 3] = step + (flag << 23);
            nowait_func_count += 4;
        } else {
            TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrFadeVol SD_STR_TYPE_MUSIC\n");
        }
    }
    if (type & 2) {
        if (nowait_func_count < 0x3FC) {
            nowait_func_buff[nowait_func_count] = 0x12;
            nowait_func_buff[nowait_func_count + 1] = id + 0x8000;
            nowait_func_buff[nowait_func_count + 2] = vol + 0xFFFF0000;
            nowait_func_buff[nowait_func_count + 3] = step + (flag << 23);
            nowait_func_count += 4;
        } else {
            TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrFadeVol SD_STR_TYPE_VOICE\n");
        }
    }
}

void TsSdStrFadePan(int type, int id, int pan, int step, int turn) {
    TsPrintN("EE:  TsSdStrFadePan: type=%d, id=%d, pan=%d, step=%d\n", type, id, pan, step);
    if (step <= 0) {
        step = 1;
    }
    if (type & 1) {
        if (nowait_func_count < 0x3FC) {
            nowait_func_buff[nowait_func_count] = 0x13;
            nowait_func_buff[nowait_func_count + 1] = id;
            nowait_func_buff[nowait_func_count + 2] = (pan << 16) | 0xFFFF;
            nowait_func_buff[nowait_func_count + 3] = step + (turn << 24);
            nowait_func_count += 4;
        } else {
            TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrFadePan SD_STR_TYPE_MUSIC\n");
        }
    }
    if (type & 2) {
        if (nowait_func_count < 0x3FC) {
            nowait_func_buff[nowait_func_count] = 0x13;
            nowait_func_buff[nowait_func_count + 1] = id + 0x8000;
            nowait_func_buff[nowait_func_count + 2] = (pan << 16) | 0xFFFF;
            nowait_func_buff[nowait_func_count + 3] = step + (turn << 24);
            nowait_func_count += 4;
        } else {
            TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrFadePan SD_STR_TYPE_VOICE\n");
        }
    }
}

void TsSdStrVolSet(int type, int id, int vol) {
    if (type & 1) {
        if (nowait_func_count < 0x3FD) {
            nowait_func_buff[nowait_func_count] = 0x10;
            nowait_func_buff[nowait_func_count + 1] = id;
            nowait_func_buff[nowait_func_count + 2] = vol;
            nowait_func_count += 3;
        } else {
            TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrVolSet SD_STR_TYPE_MUSIC\n");
        }
    }
    if (type & 2) {
        if (nowait_func_count < 0x3FD) {
            nowait_func_buff[nowait_func_count] = 0x10;
            nowait_func_buff[nowait_func_count + 1] = id + 0x8000;
            nowait_func_buff[nowait_func_count + 2] = vol;
            nowait_func_count += 3;
        } else {
            TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrVolSet SD_STR_TYPE_VOICE\n");
        }
    }
}

void TsSdStrVolSet2(int type, int id, int vol) {
    if (type & 1) {
        if (nowait_func_count < 0x3FD) {
            nowait_func_buff[nowait_func_count] = 0x10;
            nowait_func_buff[nowait_func_count + 1] = id;
            nowait_func_buff[nowait_func_count + 2] = vol + 0xFF00;
            nowait_func_count += 3;
        } else {
            TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrVolSet2 SD_STR_TYPE_MUSIC\n");
        }
    }
    if (type & 2) {
        if (nowait_func_count < 0x3FD) {
            nowait_func_buff[nowait_func_count] = 0x10;
            nowait_func_buff[nowait_func_count + 1] = id + 0x8000;
            nowait_func_buff[nowait_func_count + 2] = vol + 0xFF00;
            nowait_func_count += 3;
        } else {
            TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrVolSet2 SD_STR_TYPE_VOICE\n");
        }
    }
}

void TsSdStrVolReset(int type, int id) {
    TsPrintN("EE:  TsSdStrVolReset: type=%d, id=%d\n", type, id);
    if (type & 1) {
        if (nowait_func_count < 0x3FE) {
            nowait_func_buff[nowait_func_count] = 0x11;
            nowait_func_buff[nowait_func_count + 1] = id;
            nowait_func_count += 2;
        } else {
            TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrVolReset SD_STR_TYPE_MUSIC\n");
        }
    }
    if (type & 2) {
        if (nowait_func_count < 0x3FE) {
            nowait_func_buff[nowait_func_count] = 0x11;
            nowait_func_buff[nowait_func_count + 1] = id + 0x8000;
            nowait_func_count += 2;
        } else {
            TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrVolReset SD_STR_TYPE_VOICE\n");
        }
    }
}

void TsSdStrVoiceSetPan(u_int addr, int pan) {
    if (nowait_func_count < 0x3FD) {
        nowait_func_buff[nowait_func_count++] = 0x14;
        nowait_func_buff[nowait_func_count] = ((u_short *)addr)[7] + 0x8000;
        nowait_func_buff[nowait_func_count + 1] = pan;
        nowait_func_count += 2;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrVoiceSetPan\n");
    }
}

void TsSdStrPause(int type, int id) {
    TsPrintN("EE:  TsSdStrPause: type=%d, id=%d\n", type, id);
    if (type & 1) {
        if (nowait_func_count < 0x3FE) {
            nowait_func_buff[nowait_func_count] = 0xE;
            nowait_func_buff[nowait_func_count + 1] = id;
            nowait_func_count += 2;
        } else {
            TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrPause SD_STR_TYPE_MUSIC\n");
        }
    }
    if (type & 2) {
        if (nowait_func_count < 0x3FE) {
            nowait_func_buff[nowait_func_count] = 0xE;
            nowait_func_buff[nowait_func_count + 1] = id + 0x8000;
            nowait_func_count += 2;
        } else {
            TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrPause SD_STR_TYPE_VOICE\n");
        }
    }
}

void TsSdStrStart(int type, int id) {
    TsPrintN("EE:  TsSdStrStart: type=%d, id=%d\n", type, id);
    if (type & 1) {
        if (nowait_func_count < 0x3FC) {
            nowait_func_buff[nowait_func_count] = 0xF;
            nowait_func_buff[nowait_func_count + 1] = id;
            nowait_func_count += 2;
        } else {
            TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrStart SD_STR_TYPE_MUSIC\n");
        }
    }
    if (type & 2) {
        if (nowait_func_count < 0x3FC) {
            nowait_func_buff[nowait_func_count] = 0xF;
            nowait_func_buff[nowait_func_count + 1] = id + 0x8000;
            nowait_func_count += 2;
        } else {
            TsPrintN("EE: nowait_func_buff OVER!!: TsSdStrStart SD_STR_TYPE_VOICE\n");
        }
    }
}

int TsSdStrPlayMusicCheck(int id) {
    int data;
    int check;

    check = 0;
    if (id == 0) {
        for (data = 0; data < 6; data++) {
            if (str_data_ee[data].id > 0 && str_data_ee[data].id < 0x8000) {
                check = 1;
                break;
            }
            if ((str_data_chk[data].id > 0 && str_data_chk[data].id < 0x8000) || str_data_chk[data].read_flag2) {
                check = 2;
                break;
            }
        }
    } else {
        for (data = 0; data < 6; data++) {
            if (str_data_ee[data].id == id) {
                check = 1;
                break;
            }
            if (str_data_chk[data].id == id || str_data_chk[data].read_flag2) {
                check = 2;
                break;
            }
        }
    }
    return check;
}

int TsSdStrPlayVoiceCheck(int addr) {
    int data;
    int check;
    int id;

    check = 0;
    if (addr == 0) {
        for (data = 0; data < 6; data++) {
            if (str_data_ee[data].id > 0x8000) {
                check = 1;
                break;
            }
            if (str_data_chk[data].id > 0x8000) {
                check = 2;
                break;
            }
        }
    } else {
        id = ((u_short *)addr)[7] + 0x8000;
        for (data = 0; data < 6; data++) {
            if (str_data_ee[data].id == id) {
                check = 1;
                break;
            }
            if (str_data_chk[data].id == id) {
                check = 2;
                break;
            }
        }
    }
    return check;
}

int TsSdStrPlayVoiceCheckAll() {
    int data;
    int count;

    count = 0;
    for (data = 0; data < 6; data++) {
        if ((short)str_data_chk[data].id < 0) {
            count++;
        }
    }
    return count;
}

int TsSdStrGetStatus(int type, int id) {
    int data;

    if (id != 0) {
        if (type == 2) {
            id += 0x8000;
        }
        for (data = 0; data < 6; data++) {
            if (str_data_chk[data].id == id || str_data_ee[data].id == id) {
                return str_data_chk[data].status;
            }
        }
    }
    return 0;
}

int TsSdStrGetStatusAddr(int type, int addr) {
    return TsSdStrGetStatus(type, ((u_short *)addr)[7]);
}

int TsSdStrGetMarker(int type, int id) {
    int data;

    if (id != 0) {
        if (type == 2) {
            id += 0x8000;
        }
        for (data = 0; data < 6; data++) {
            if (str_data_chk[data].id == id || str_data_ee[data].id == id) {
                return str_data_chk[data].check_mark;
            }
        }
    }
    return 0;
}

int TsSdStrGetCount(int type, int id) {
    int data;

    if (id != 0) {
        if (type == 2) {
            id += 0x8000;
        }
        for (data = 0; data < 6; data++) {
            if (str_data_chk[data].id == id || str_data_ee[data].id == id) {
                return str_data_chk[data].time_count;
            }
        }
    }
    return 0;
}

int TsSdStrGetCountAddr(int type, int addr) {
    return TsSdStrGetCount(type, ((u_short *)addr)[7]);
}

int TsSdStrGetTime(int addr) {
    int time;

    time = ((u_short *)addr)[6] * 0xD2000 / ((u_short *)addr)[4];
    if (((u_short *)addr)[5] & 0x100) {
        time /= 2;
    }
    return time;
}

int ConvLittleEndian(int a) {
    int b[4];

    b[0] = a & 0xFF;
    b[1] = (a >> 8) & 0xFF;
    b[2] = (a >> 16) & 0xFF;
    b[3] = (a >> 24) & 0xFF;
    return (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];
}

/* TsSdVsbPlay's string literals, for its INCLUDE_ASM; remove with it. */
asm(".section .rodata\n"
    ".align 3\n.asciz \"EE:  TsSdStrPlayVSB: ERROR!!!!!!!!!!!!!!!!!!!! MAX OVER max=%d, num=%d\\n\"\n"
    ".align 3\n.asciz \"EE:  TsSdStrPlayVSB: ERROR!!!!!!!!!!!!!!!!!!!! No Entry num=%d\\n\"\n"
    ".align 3\n.asciz \"EE:  TsSdStrPlayVSB: vsb_id=%d, num=%d, vol=%d, pan=%d, pos=0x%08x, size=0x%08x\\n\"\n"
    ".align 3\n.asciz \"EE:  TsSdStrPlayVSB: volm=%d, rev=%d, rate=%d, size=0x%08x\\n\"\n"
    ".align 3\n.asciz \"footer   addr=0x%08x\\n\"\n"
    ".align 3\n.asciz \"%02x \"\n"
    ".align 3\n.asciz \"\\n\"\n"
    ".align 3\n.asciz \"end_flag addr=0x%08x\\n\"\n"
    ".align 3\n.asciz \"header   addr=0x%08x\\n\"\n"
    ".previous");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Sound/driver/sddrv_ee", TsSdVsbPlay);

/* Register allocation: addr is spilled instead of kept in s8 (one more stack slot), so most registers differ. */
#if 0
void TsSdVsbPlay(u_int addr, int vsb_id, int num, int vol, int pan) {
    u_int pos;
    u_int max;
    int size;
    int size2;
    int volm;
    int rev;
    int rate;
    int addr1;
    int addr2;
    SSL_VAG_STREAM_HEADER *vag_head;
    u_char *data;

    max = ((u_int *)addr)[3];
    if (num >= max) {
        TsPrintN("EE:  TsSdStrPlayVSB: ERROR!!!!!!!!!!!!!!!!!!!! MAX OVER max=%d, num=%d\n", max, num);
        return;
    }
    pos = ((u_int *)addr)[num * 2 + 4] + max * 8 + 16;
    size = ((int *)addr)[num * 2 + 5];
    if (size == -1) {
        TsPrintN("EE:  TsSdStrPlayVSB: ERROR!!!!!!!!!!!!!!!!!!!! No Entry num=%d\n", num);
        return;
    }
    TsPrintN("EE:  TsSdStrPlayVSB: vsb_id=%d, num=%d, vol=%d, pan=%d, pos=0x%08x, size=0x%08x\n", vsb_id, num, vol, pan, pos, size);
    vag_head = (SSL_VAG_STREAM_HEADER *)(addr + pos);
    volm = vag_head->mVolume & 0x7F;
    rev = vag_head->mVolume >> 7;
    rate = ConvLittleEndian(vag_head->mSampleRate);
    size = ConvLittleEndian(vag_head->mVagSize);
    TsPrintN("EE:  TsSdStrPlayVSB: volm=%d, rev=%d, rate=%d, size=0x%08x\n", volm, rev, rate, size);
    addr1 = pos + size + 16;
    data = (u_char *)(addr + addr1);
    data[1] = 3;
    TsPrintN("footer   addr=0x%08x\n", addr1);
    for (max = 0; max < 16; max++) {
        TsPrintN("%02x ", data[max]);
    }
    TsPrintN("\n");
    for (max = 16; max < 32; max++) {
        TsPrintN("%02x ", data[max]);
    }
    TsPrintN("\n");
    size2 = size / 2048;
    if ((size & 0x7FF) == 0) {
        size2--;
    }
    addr2 = pos + size2 * 2048 + 0x30;
    data = (u_char *)(addr + addr2);
    if (data[1] == 3) {
        data[17] = 3;
        addr1 += 16;
    }
    data[1] = 0x10;
    TsPrintN("end_flag addr=0x%08x\n", addr2);
    size = addr1 - addr2;
    for (max = 0; max < 16; max++) {
        TsPrintN("%02x ", data[max]);
    }
    TsPrintN("\n");
    for (max = 16; max < 32; max++) {
        TsPrintN("%02x ", data[max]);
    }
    TsPrintN("\n");
    data = (u_char *)vag_head + 0x30;
    data[0] = volm;
    data[1] = pan;
    data[2] = 100;
    data[3] = 0;
    data[4] = size;
    data[5] = size >> 8;
    data[6] = 0;
    data[7] = 0;
    data[8] = rate;
    data[9] = rate >> 8;
    data[10] = 0;
    data[11] = (rev << 6) | 0x10;
    data[12] = size2 + 1;
    data[13] = 0;
    data[14] = num + 1;
    data[15] = vsb_id;
    TsPrintN("header   addr=0x%08x\n", pos + 0x30);
    for (max = 0; max < 16; max++) {
        TsPrintN("%02x ", data[max]);
    }
    TsPrintN("\n");
    for (max = 16; max < 32; max++) {
        TsPrintN("%02x ", data[max]);
    }
    TsPrintN("\n");
    nowait_func_buff[nowait_func_count++] = 0x1D;
    nowait_func_buff[nowait_func_count++] = vsb_id;
    TsSdStrPlayVoice((u_int)data);
    TsSdStrVolSet(2, num + (vsb_id << 8) + 1, vol);
}
#endif

void TsSdVsbStop(int vsb_id) {
    nowait_func_buff[nowait_func_count++] = 0x1D;
    nowait_func_buff[nowait_func_count++] = vsb_id;
}

void TsSdVsbSetVP(int vsb_id, int num, int vol, int pan) {
    int id;

    vsb_id = (vsb_id << 8) + 1;
    id = num + vsb_id + 0x8000;
    nowait_func_buff[nowait_func_count++] = 0x10;
    nowait_func_buff[nowait_func_count++] = id;
    nowait_func_buff[nowait_func_count++] = vol;
    nowait_func_buff[nowait_func_count++] = 0x14;
    nowait_func_buff[nowait_func_count++] = id;
    nowait_func_buff[nowait_func_count++] = pan;
}

void TsSdVsbPause(int vsb_id, int num) {
    int id;

    vsb_id = (vsb_id << 8) + 1;
    id = num + vsb_id + 0x8000;
    nowait_func_buff[nowait_func_count++] = 0xE;
    nowait_func_buff[nowait_func_count++] = id;
}

void TsSdVsbStart(int vsb_id, int num) {
    int id;

    vsb_id = (vsb_id << 8) + 1;
    id = num + vsb_id + 0x8000;
    nowait_func_buff[nowait_func_count++] = 0xF;
    nowait_func_buff[nowait_func_count++] = id;
}

void TsSdMpegTransVoice(u_int m_addr, u_int s_addr, u_int size) {
    while (sceSifCheckStatRpc(&cd.rpcd)) {
    }
    rpc_buf[0] = m_addr;
    rpc_buf[1] = s_addr;
    rpc_buf[2] = size;
    sceSifCallRpc(&cd, 0xB, 0, rpc_buf, 12, 0, 0, 0, 0);
}

void TsSdMpegTransBlock(u_int m_addr, u_int size, u_int start_addr) {
    while (sceSifCheckStatRpc(&cd.rpcd)) {
    }
    rpc_buf[0] = m_addr;
    rpc_buf[1] = size;
    rpc_buf[2] = start_addr;
    sceSifCallRpc(&cd, 0xC, 0, rpc_buf, 12, 0, 0, 0, 0);
}

u_int TsSdMpegTransBlockStop() {
    while (sceSifCheckStatRpc(&cd.rpcd)) {
    }
    sceSifCallRpc(&cd, 0xD, 0, 0, 0, rpc_buf, 4, 0, 0);
    return (u_int)rpc_buf;
}

u_int TsSdMpegTransBlockStatus(int channel, int flag) {
    while (sceSifCheckStatRpc(&cd.rpcd)) {
    }
    rpc_buf[0] = channel;
    rpc_buf[1] = flag;
    sceSifCallRpc(&cd, 0xE, 0, rpc_buf, 8, rpc_buf, 4, 0, 0);
    return rpc_buf[0];
}

void TsSdMpegSetVolume(int vol) {
    if (nowait_func_count < 0x3FE) {
        nowait_func_buff[nowait_func_count] = 0x3C;
        nowait_func_buff[nowait_func_count + 1] = vol;
        nowait_func_count += 2;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdMpegSetVolume\n");
    }
}

int TsSdMpegGetAddrIOP() {
    return iop_str_addr;
}

void TsSdStopAll(int flag) {
    if (flag == 0) {
        TsPrintN("TsSdStopAll: START!\n");
        while (sceSifCheckStatRpc(&cd.rpcd)) {
        }
        TsPrintN("TsSdStopAll: working...\n");
        sceSifCallRpc(&cd, 6, 0, 0, 0, 0, 0, 0, 0);
        TsPrintN("TsSdStopAll: END!\n");
    }
}

void TsSdFadeOutAll(int step) {
    TsPrintN("EE:  TsSdFadeOutAll: step=%d\n", step);
    if (nowait_func_count < 0x3FE) {
        nowait_func_buff[nowait_func_count] = 5;
        nowait_func_buff[nowait_func_count + 1] = step;
        nowait_func_count += 2;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdFadeOutAll\n");
    }
}

int TsSdFadeOutAllCheck() {
    return fade_all_flag;
}

void TsSdCheckDriverGetParam() {
    int data;
    int num;
    int wd_num;
    int seb_num;
    int sep_num;

    if (nowait_func_count < 0x3FD) {
        nowait_func_buff[nowait_func_count] = 0;
        nowait_func_buff[nowait_func_count + 1] = (u_int)ee_param;
        nowait_func_buff[nowait_func_count + 2] = 0x1000;
        nowait_func_count += 3;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdCheckDriverGetParam\n");
    }
    for (num = 0; num < 16; num++) {
        wd_num = wd_ee_param[num] & 0x3FFF;
        if (wd_num < 9999) {
            for (data = 0; data < 16; data++) {
                if (wd_num == (wd_param[data] & 0x3FFF)) {
                    wd_ee_param[num] = 9999;
                    TsPrintN("EE:  TsSdCheckDriverGetParam: wd_ee_param[%d]=%d, wd_param[%d]=%d\n", num, wd_ee_param[num], data, wd_param[data] & 0x3FFF);
                }
            }
        }
    }
    for (num = 0; num < 16; num++) {
        seb_num = seb_ee_param[num] & 0x3FFF;
        if (seb_num < 9999) {
            for (data = 0; data < 16; data++) {
                if (seb_num == (seb_param[data] & 0x3FFF)) {
                    seb_ee_param[num] = 9999;
                    TsPrintN("EE:  TsSdCheckDriverGetParam: seb_ee_param[%d]=%d, seb_param[%d]=%d\n", num, seb_ee_param[num], data, seb_param[data] & 0x3FFF);
                }
            }
        }
    }
    for (num = 0; num < 4; num++) {
        sep_num = sep_ee_param[num] & 0x7FFFFF;
        if (sep_num < 8000000) {
            for (data = 0; data < 4; data++) {
                if (sep_num == (sep_param[data] & 0x7FFFFF)) {
                    sep_ee_param[num] = 8000000;
                }
            }
        }
    }
    for (num = 0; num < 6; num++) {
        for (data = 0; data < 6; data++) {
            if (str_data_chk[num].id == str_data_ee[data].id || str_err_chk[num] == str_data_ee[data].id) {
                str_data_ee[data].id = 0;
            }
        }
    }
    for (num = 0; num < 16; num++) {
        if (se_data_ee_id[num]) {
            for (data = 0; data < 16; data++) {
                if (se_data_chk[data].id == se_data_ee_id[num] || se_err_chk[data] == se_data_ee_id[num]) {
                    se_data_ee_id[num] = 0;
                    break;
                }
            }
        }
    }
}

int TsSdCheckDriverSetParam() {
    int iop_work_flag;

    if (nowait_func_count == 0) {
        return 0;
    }
    iop_work_flag = sceSifCheckStatRpc(&cd.rpcd);
    if (iop_work_flag == 0) {
        if (nowait_func_count < 0x3FF) {
            nowait_func_buff[nowait_func_count++] = 0xFFFFFFFF;
        } else {
            TsPrintN("EE: nowait_func_buff OVER!!: TsSdCheckDriverSetParam\n");
        }
        memcpy(nowait_trans_buff, nowait_func_buff, nowait_func_count * 4);
        FlushCache(0);
        sceSifCallRpc(&cd, 0x63, 1, nowait_trans_buff, nowait_func_count * 4, 0, 0, 0, 0);
        nowait_func_count = 0;
    }
    return iop_work_flag;
}

int TsSdCheckDriverSetParam2() {
    int iop_work_flag;

    if (nowait_func_count == 0) {
        return 0;
    }
    do {
        iop_work_flag = sceSifCheckStatRpc(&cd.rpcd);
    } while (iop_work_flag);
    if (nowait_func_count < 0x3FF) {
        nowait_func_buff[nowait_func_count++] = 0xFFFFFFFF;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdCheckDriverSetParam2\n");
    }
    FlushCache(0);
    sceSifCallRpc(&cd, 0x63, 1, nowait_func_buff, nowait_func_count * 4, 0, 0, 0, 0);
    nowait_func_count = 0;
    return iop_work_flag;
}

void TsSdCheckDriver() {
    TsSdCheckDriverGetParam();
    TsSdCheckDriverSetParam();
}

void TsSdSetModeOutput(int type) {
    output_mode = type;
    if (nowait_func_count < 0x3FE) {
        nowait_func_buff[nowait_func_count] = 1;
        nowait_func_buff[nowait_func_count + 1] = type;
        nowait_func_count += 2;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSetModeOutput\n");
    }
}

void TsSdSetMasterVol2_StrMusic(int mvol) {
    mvol_str_m = mvol;
    if (nowait_func_count < 0x3FE) {
        nowait_func_buff[nowait_func_count] = 0x42;
        nowait_func_buff[nowait_func_count + 1] = mvol;
        nowait_func_count += 2;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSetMasterVol2_StrMusic\n");
    }
}

void TsSdSetMasterVol2_StrVoice(int mvol) {
    mvol_str_v = mvol;
    if (nowait_func_count < 0x3FE) {
        nowait_func_buff[nowait_func_count] = 0x43;
        nowait_func_buff[nowait_func_count + 1] = mvol;
        nowait_func_count += 2;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSetMasterVol2_StrVoice\n");
    }
}

void TsSdSetMasterVol2_SeqSE(int mvol) {
    mvol_seq_se = mvol;
    if (nowait_func_count < 0x3FC) {
        nowait_func_buff[nowait_func_count] = 0x44;
        nowait_func_buff[nowait_func_count + 1] = mvol;
        nowait_func_count += 2;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSetMasterVol2_SeqSE\n");
    }
}

void TsSdSetMasterVol2_SeqVoice(int mvol) {
    mvol_seq_v = mvol;
    if (nowait_func_count < 0x3FC) {
        nowait_func_buff[nowait_func_count] = 0x45;
        nowait_func_buff[nowait_func_count + 1] = mvol;
        nowait_func_count += 2;
    } else {
        TsPrintN("EE: nowait_func_buff OVER!!: TsSdSetMasterVol2_SeqVoice\n");
    }
}

int TsSdGetModeOutput() {
    return output_mode;
}

int TsSdGetMasterVol2_StrMusic() {
    return mvol_str_m;
}

int TsSdGetMasterVol2_StrVoice() {
    return mvol_str_v;
}

int TsSdGetMasterVol2_SeqSE() {
    return mvol_seq_se;
}

int TsSdGetMasterVol2_SeqVoice() {
    return mvol_seq_v;
}

void TsInitParam() {
    int addr;

    addr = (int)ee_param + 0x10;
    TsPrintN("EE:  TsInitParam:     start: addr=%08x\n", addr - 0x10);
    wd_param = (u_int *)addr;
    addr += 0x40;
    seb_param = (u_int *)addr;
    addr += 0x40;
    sep_param = (u_int *)addr;
    addr += 0x10;
    se_data_chk = (SEQ_DATA_CHK *)addr;
    addr += 0x100;
    se_err_chk = (u_int *)addr;
    addr += 0x40;
    str_data_chk = (STR_DATA_CHK *)addr;
    addr += 0x60;
    str_err_chk = (u_int *)addr;
    addr += 0x20;
    TsPrintN("                        end: addr=%08x\n", addr);
}

void TsInitData() {
    int num;

    nowait_func_count = 0;
    seb_id_cm = 0;
    str_fnpt = 0;
    fade_all_flag = 0;
    for (num = 0; num < 16; num++) {
        wd_param[num] = 9999;
        wd_ee_param[num] = 9999;
    }
    for (num = 0; num < 16; num++) {
        seb_param[num] = 9999;
        seb_ee_param[num] = 9999;
    }
    for (num = 0; num < 4; num++) {
        sep_ee_param[num] = sep_param[num] = 8000000;
    }
    for (num = 0; num < 16; num++) {
        se_data_ee_id[num] = 0;
        sep_play_addr[num] = 0;
        sep_play_num[num] = 0;
    }
    for (num = 5; num >= 0; num--) {
        str_data_ee_id[num] = 0;
    }
    output_mode = 0;
    mvol_str_m = 0x80;
    mvol_str_v = 0x80;
    mvol_seq_se = 0x80;
    mvol_seq_v = 0x80;
}

void TsInitSif() {
    int num;
    int check;

    while (1) {
        check = sceSifBindRpc(&cd, 0x1000, 0);
        num = 0x10001;
        while (num--) {
        }
        if (check < 0) {
            continue;
        }
        num = 0x10001;
        while (num--) {
        }
        if (cd.serve != 0) {
            break;
        }
    }
    FlushCache(0);
}

void TsSdInitDriver() {
    TsPrintN("-----TEAM_REC----- ");
    TsPrintN("SOUND INITIALIZING! ------------------\n");
    TsPrintN("EE:  sddrv_ee.c: 07/02/06\n");
    TsInitParam();
    TsInitData();
    TsInitSif();
    TsPrintN("IOP_INIT...\n");
    sceSifCallRpc(&cd, 0, 0, 0, 0, 0, 0, 0, 0);
    rpc_buf[0] = 0x3FFF;
    rpc_buf[1] = 0x80;
    rpc_buf[2] = 0x80;
    rpc_buf[3] = 0x80;
    rpc_buf[4] = output_mode;
    sceSifCallRpc(&cd, 8, 0, rpc_buf, 20, 0, 0, 0, 0);
    rpc_buf[0] = 20;
    sceSifCallRpc(&cd, 9, 0, rpc_buf, 4, 0, 0, 0, 0);
    sceSifCallRpc(&cd, 10, 0, 0, 0, rpc_buf, 4, 0, 0);
    iop_str_addr = rpc_buf[0];
    TsPrintN("\t iop_str_addr=0x%08x\n", iop_str_addr);
    TsPrintN("----------------------- COMPLETED -----------------------\n");
}

// SOURCE DATA: .rodata
