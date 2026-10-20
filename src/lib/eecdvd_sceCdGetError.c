/* libcdvd (Sony EE SDK): error code of the last command (S-command 0x03 to the IOP's cdvdman). */
#include "sdk/sifrpc.h"

#define UNCACHED(p) ((unsigned int)(p) | 0x20000000)

extern sceSifClientData _sceCd_cd_scmd;
extern int _sceCd_scmdrdata[];
extern int _sceCd_scmd_semid;

int _sceCd_scmd_prechk(int cmd);
int SignalSema(int sema);

int sceCdGetError(void)
{
    int ret;

    if (!_sceCd_scmd_prechk(3)) {
        return -1;
    }
    if (sceSifCallRpc(&_sceCd_cd_scmd, 4, 0, 0, 0, _sceCd_scmdrdata, 4, 0, 0) < 0) {
        SignalSema(_sceCd_scmd_semid);
        return -1;
    }
    ret = *(int *)UNCACHED(_sceCd_scmdrdata);
    SignalSema(_sceCd_scmd_semid);
    return ret;
}
