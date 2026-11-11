/* libcdvd (Sony EE SDK): start reading sectors (N-command 0x01); the IOP signals the end through a callback. */
#include "sdk/sifrpc.h"

typedef struct {
    unsigned char trycount;
    unsigned char spindlctrl;
    unsigned char datapattern;
    unsigned char pad;
} sceCdRMode;

typedef struct {
    unsigned int lbn;
    unsigned int sectors;
    void *buf;
    unsigned char trycount;
    unsigned char spindlctrl;
    unsigned char datapattern;
    unsigned char pad;
    void *intr_data;
    void *cur_pos;
} sceCdReadArg;

extern sceSifClientData _sceCd_cd_ncmd;
extern sceCdReadArg _sceCd_ncmdsdata;
extern volatile int _sceCd_ncmd_semid;
extern volatile int _sceCd_ee_read_mode;
extern int _sceCd_Read_cur_pos;
extern char _sceCd_rd_intr_data[];
extern int SCE_CD_debug;
extern volatile int sceCdCbfunc_num;
extern volatile int _sceCd_c_cb_sem;

void _sceCd_cd_read_intr(void *data);
int _sceCd_ncmd_prechk(int cmd);
int sceCdNcmdDiskReady(void);
int SignalSema(int sema);
void sceSifWriteBackDCache(void *ptr, int size);
void scePrintf(const char *fmt, ...);

int sceCdRead(unsigned int lbn, unsigned int sectors, void *buf, sceCdRMode *mode)
{
    sceCdReadArg *arg = &_sceCd_ncmdsdata;
    int size;

    if (!(_sceCd_ee_read_mode & 1) && sceCdNcmdDiskReady() == 6) {
        return 0;
    }
    if (!_sceCd_ncmd_prechk(4)) {
        return 0;
    }
    arg->lbn = lbn;
    arg->sectors = sectors;
    arg->buf = buf;
    arg->trycount = mode->trycount;
    arg->spindlctrl = mode->spindlctrl;
    arg->datapattern = mode->datapattern;
    arg->intr_data = _sceCd_rd_intr_data;
    arg->cur_pos = &_sceCd_Read_cur_pos;
    switch (mode->datapattern) {
    case 1:
        size = sectors * 2328;
        break;
    case 2:
        size = sectors * 2340;
        break;
    case 0:
    default:
        size = sectors * 2048;
        break;
    }
    _sceCd_Read_cur_pos = 0;
    if (!(_sceCd_ee_read_mode & 2)) {
        sceSifWriteBackDCache(buf, size);
    }
    sceSifWriteBackDCache(_sceCd_rd_intr_data, 0x90);
    sceSifWriteBackDCache(arg, sizeof(*arg));
    sceSifWriteBackDCache(&_sceCd_Read_cur_pos, 4);
    if (SCE_CD_debug > 0) {
        scePrintf("call cdread cmd\n");
    }
    sceCdCbfunc_num = 1;
    _sceCd_c_cb_sem = 1;
    if (sceSifCallRpc(&_sceCd_cd_ncmd, 1, 1, arg, sizeof(*arg), 0, 0, _sceCd_cd_read_intr, _sceCd_rd_intr_data) < 0) {
        sceCdCbfunc_num = 0;
        _sceCd_c_cb_sem = 0;
        SignalSema(_sceCd_ncmd_semid);
        return 0;
    }
    if (SCE_CD_debug > 0) {
        scePrintf("cdread end\n");
    }
    return 1;
}
// SOURCE DATA: .rodata
// SOURCE DATA SLUS_217.99_retail: .rodata
