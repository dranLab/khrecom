/* The IOP heap and system memory, through the IOP's heap RPC server (0x80000003). */
#include "sdk/sifrpc.h"

#define SIF_RPC_HEAP 0x80000003

static sceSifClientData cd __attribute__((aligned(64)));
static unsigned int rdata[16] __attribute__((aligned(64)));
static unsigned int sdata[16] __attribute__((aligned(64)));
static struct {
    void *addr;
    char path[252];
} _lih_data __attribute__((aligned(64)));
static int _bind = -1;

int sceSifInitIopHeap(void)
{
    int i;

    for (;;) {
        if (sceSifBindRpc(&cd, SIF_RPC_HEAP, 0) < 0) {
            return -1;
        }
        if (cd.serve != 0) {
            _bind = 0;
            break;
        }
        i = 0x100000;
        while (i--) {
        }
    }
    return 0;
}

void *sceSifAllocIopHeap(int size)
{
    if (_bind < 0) {
        return 0;
    }
    sdata[0] = size;
    if (sceSifCallRpc(&cd, 1, 0, sdata, 4, rdata, 4, 0, 0) >= 0) {
        return (void *)rdata[0];
    }
    return 0;
}

void *sceSifAllocSysMemory(int type, int size, void *addr)
{
    if (_bind < 0) {
        return 0;
    }
    sdata[0] = size;
    sdata[1] = type;
    sdata[2] = (unsigned int)addr;
    if (sceSifCallRpc(&cd, 4, 0, sdata, 12, rdata, 4, 0, 0) >= 0) {
        return (void *)rdata[0];
    }
    return 0;
}

int sceSifFreeSysMemory(void *addr)
{
    if (_bind < 0) {
        return 0;
    }
    sdata[0] = (unsigned int)addr;
    if (sceSifCallRpc(&cd, 2, 0, sdata, 4, rdata, 4, 0, 0) >= 0) {
        return rdata[0];
    }
    return -1;
}

int sceSifFreeIopHeap(void *addr)
{
    return sceSifFreeSysMemory(addr);
}

/* Load the file at path (an IOP device path) into IOP memory at addr. */
int sceSifLoadIopHeap(const char *path, void *addr)
{
    int i;

    if (_bind < 0) {
        return 0;
    }
    for (i = 0; i < 252; i++) {
        if ((_lih_data.path[i] = path[i]) == 0) {
            break;
        }
    }
    if (i == 252) {
        i = 251;
        _lih_data.path[i] = 0;
    }
    _lih_data.addr = addr;
    _lih_data.path[251] = 0;
    if (sceSifCallRpc(&cd, 3, 0, &_lih_data, i + 5, rdata, 4, 0, 0) >= 0) {
        return rdata[0];
    }
    return -1;
}

unsigned int sceSifQueryMemSize(void)
{
    if (_bind < 0) {
        return 0;
    }
    if (sceSifCallRpc(&cd, 5, 0, 0, 0, rdata, 4, 0, 0) < 0) {
        return -1;
    }
    return rdata[0];
}

unsigned int sceSifQueryMaxFreeMemSize(void)
{
    if (_bind < 0) {
        return 0;
    }
    if (sceSifCallRpc(&cd, 6, 0, 0, 0, rdata, 4, 0, 0) < 0) {
        return -1;
    }
    return rdata[0];
}

unsigned int sceSifQueryTotalFreeMemSize(void)
{
    if (_bind < 0) {
        return 0;
    }
    if (sceSifCallRpc(&cd, 7, 0, 0, 0, rdata, 4, 0, 0) < 0) {
        return -1;
    }
    return rdata[0];
}

unsigned int sceSifQueryBlockTopAddress(void *addr)
{
    if (_bind < 0) {
        return 0;
    }
    sdata[0] = (unsigned int)addr;
    if (sceSifCallRpc(&cd, 8, 0, sdata, 4, rdata, 4, 0, 0) < 0) {
        return -1;
    }
    return rdata[0];
}

unsigned int sceSifQueryBlockSize(void *addr)
{
    if (_bind < 0) {
        return 0;
    }
    sdata[0] = (unsigned int)addr;
    if (sceSifCallRpc(&cd, 9, 0, sdata, 4, rdata, 4, 0, 0) < 0) {
        return -1;
    }
    return rdata[0];
}
// SOURCE DATA: .data .bss
