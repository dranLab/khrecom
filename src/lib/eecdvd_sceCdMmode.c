/* libcdvd (Sony EE SDK): media mode (S-command 0x22). */
#include "sdk/sifrpc.h"

#define UNCACHED(p) ((unsigned int)(p) | 0x20000000)

extern sceSifClientData _sceCd_cd_scmd;
extern int _sceCd_scmdsdata[];
extern int _sceCd_scmdrdata[];
extern int _sceCd_scmd_semid;

int _sceCd_scmd_prechk(int cmd);
int SignalSema(int sema);
void sceSifWriteBackDCache(void *ptr, int size);
void scePrintf(const char *fmt, ...);

static int rpc_mmode = 0x22;
static int outcnt_mmode = 4;
static int incnt_mmode = 4;

int sceCdMmode(int media)
{
    int *sdata = _sceCd_scmdsdata;
    int ret;

    if (!_sceCd_scmd_prechk(0x22)) {
        return 0;
    }
    *sdata = media;
    sceSifWriteBackDCache(sdata, 4);
    if (sceSifCallRpc(&_sceCd_cd_scmd, rpc_mmode, 0, sdata, outcnt_mmode, _sceCd_scmdrdata, incnt_mmode,
                      0, 0) < 0) {
        SignalSema(_sceCd_scmd_semid);
        return 0;
    }
    ret = *(int *)UNCACHED(_sceCd_scmdrdata);
    SignalSema(_sceCd_scmd_semid);
    return ret;
}

int sceCdChgSys(int sys)
{
    scePrintf("sceCdChgSys: Illegal_function_call\n");
    return 2;
}
// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .data .rodata
