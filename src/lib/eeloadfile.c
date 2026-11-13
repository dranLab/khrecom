/* Loading IOP modules and EE ELF files through the IOP's LOADFILE RPC server (0x80000006). */
#include "sdk/sifrpc.h"

int memcmp(const void *s1, const void *s2, unsigned int n);
void *memcpy(void *dst, const void *src, unsigned int n);
void *memset(void *s, int c, unsigned int n);
char *strncpy(char *dst, const char *src, unsigned int n);

#define SIF_RPC_LOADFILE 0x80000006
#define SCE_EBINDMISS 0xFFFF0000
#define SCE_ECALLMISS 0xFFFEFFFF
#define SCE_ELOADMISS 0xFFFEFFFD
#define SCE_EVERSIONMISS 0xFFFEFFFC
#define SCE_EILLEGALTYPE 0xFFFEFFFE

typedef struct {
    int epc;
    int gp;
    int sp;
    int dummy;
} sceExecData;

/* libkernl's version ("3100", after "PsIIlibkernl" in klib.s) */
extern char __ps2_klibinfo__[];
#define KERNL_VERSION (__ps2_klibinfo__ + 12)

static int _bind_check = -1;
static char *_lfwildcard = "....";

static struct {
    int id;
    int arg;
    char path[252];
    char args[252];
} _senddata __attribute__((aligned(64)));
static sceSifClientData cd;
static char _lfversion[4];

static int _lf_bind(void)
{
    int i;

    if (_bind_check < 0) {
    retry:
        if (sceSifBindRpc(&cd, SIF_RPC_LOADFILE, 0) < 0) {
            return -1;
        }
        if (cd.serve != 0) {
            _bind_check = 0;
            if (sceSifCallRpc(&cd, 0xFF, 0, 0, 0, &_senddata, 4, 0, 0) < 0) {
                return SCE_ECALLMISS;
            }
            memcpy(_lfversion, &_senddata, 4);
            return 0;
        }
        i = 0x100000;
        while (i--) {
        }
        goto retry;
    }
    return 0;
}

/* Whether the IOP's LOADFILE doesn't fit this libkernl ("...." matches any version). */
static int _lf_version(void)
{
    char *ver = KERNL_VERSION;
    int ret = 0;

    if (memcmp(_lfversion, ver, 4) != 0 && memcmp(_lfversion, _lfwildcard, 4) != 0) {
        ret = memcmp(ver, _lfwildcard, 4) != 0;
    }
    return ret;
}

int sceSifLoadFileReset(void)
{
    _bind_check = -1;
    memset(_lfversion, 0, sizeof(_lfversion));
    return 0;
}

static int _sceSifLoadModuleBuffer(void *ptr, int args, const char *argp, int *result)
{
    int ret;

    if (_lf_bind() < 0) {
        return SCE_EBINDMISS;
    }
    if (_lf_version()) {
        return SCE_EVERSIONMISS;
    }
    _senddata.id = (int)ptr;
    if (argp) {
        if (args > 252) {
            memcpy(_senddata.args, argp, 252);
            _senddata.arg = 252;
        } else {
            memcpy(_senddata.args, argp, args);
            _senddata.arg = args;
        }
    } else {
        _senddata.arg = 0;
    }
    if (sceSifCallRpc(&cd, 6, 0, &_senddata, sizeof(_senddata), &_senddata, 8, 0, 0) < 0) {
        return SCE_ECALLMISS;
    }
    ret = _senddata.id;
    *result = _senddata.arg;
    return ret;
}

int sceSifStopModule(int modid, int args, const char *argp, int *result)
{
    int ret;

    if (_lf_bind() < 0) {
        return SCE_EBINDMISS;
    }
    if (_lf_version()) {
        return SCE_EVERSIONMISS;
    }
    _senddata.id = modid;
    if (argp) {
        if (args > 252) {
            memcpy(_senddata.args, argp, 252);
            _senddata.arg = 252;
        } else {
            memcpy(_senddata.args, argp, args);
            _senddata.arg = args;
        }
    } else {
        _senddata.arg = 0;
    }
    if (sceSifCallRpc(&cd, 7, 0, &_senddata, sizeof(_senddata), &_senddata, 8, 0, 0) < 0) {
        return SCE_ECALLMISS;
    }
    ret = _senddata.id;
    *result = _senddata.arg;
    return ret;
}

int sceSifUnloadModule(int modid)
{
    if (_lf_bind() < 0) {
        return SCE_EBINDMISS;
    }
    if (_lf_version()) {
        return SCE_EVERSIONMISS;
    }
    _senddata.id = modid;
    if (sceSifCallRpc(&cd, 8, 0, &_senddata, 4, &_senddata, 4, 0, 0) < 0) {
        return SCE_ECALLMISS;
    }
    return _senddata.id;
}

int sceSifSearchModuleByName(const char *name)
{
    if (_lf_bind() < 0) {
        return SCE_EBINDMISS;
    }
    if (_lf_version()) {
        return SCE_EVERSIONMISS;
    }
    strncpy(_senddata.path, name, sizeof(_senddata.path));
    _senddata.path[sizeof(_senddata.path) - 1] = 0;
    if (sceSifCallRpc(&cd, 9, 0, &_senddata, sizeof(_senddata), &_senddata, 4, 0, 0) < 0) {
        return SCE_ECALLMISS;
    }
    return _senddata.id;
}

int sceSifSearchModuleByAddress(const void *addr)
{
    if (_lf_bind() < 0) {
        return SCE_EBINDMISS;
    }
    if (_lf_version()) {
        return SCE_EVERSIONMISS;
    }
    _senddata.id = (int)addr;
    if (sceSifCallRpc(&cd, 10, 0, &_senddata, 4, &_senddata, 4, 0, 0) < 0) {
        return SCE_ECALLMISS;
    }
    return _senddata.id;
}

int sceSifLoadModuleBuffer(void *ptr, int args, const char *argp)
{
    int result;

    return _sceSifLoadModuleBuffer(ptr, args, argp, &result);
}

int sceSifLoadStartModuleBuffer(void *ptr, int args, const char *argp, int *result)
{
    return _sceSifLoadModuleBuffer(ptr, args, argp, result);
}

static int _sceSifLoadModule(const char *path, int args, const char *argp, int *result, int fno)
{
    int ret;

    if (_lf_bind() < 0) {
        return SCE_EBINDMISS;
    }
    if (_lf_version()) {
        return SCE_EVERSIONMISS;
    }
    strncpy(_senddata.path, path, sizeof(_senddata.path));
    _senddata.path[sizeof(_senddata.path) - 1] = 0;
    if (argp) {
        if (args > 252) {
            memcpy(_senddata.args, argp, 252);
            _senddata.id = 252;
        } else {
            memcpy(_senddata.args, argp, args);
            _senddata.id = args;
        }
    } else {
        _senddata.args[0] = 0;
        _senddata.id = 0;
    }
    if (sceSifCallRpc(&cd, fno, 0, &_senddata, sizeof(_senddata), &_senddata, 8, 0, 0) < 0) {
        return SCE_ECALLMISS;
    }
    ret = _senddata.id;
    *result = _senddata.arg;
    return ret;
}

int sceSifLoadModule(const char *path, int args, const char *argp)
{
    int result;

    return _sceSifLoadModule(path, args, argp, &result, 0);
}

int sceSifLoadStartModule(const char *path, int args, const char *argp, int *result)
{
    return _sceSifLoadModule(path, args, argp, result, 0);
}

static int _sceSifLoadElfPart(const char *path, const char *secname, sceExecData *data, int fno)
{
    if (_lf_bind() < 0) {
        return SCE_EBINDMISS;
    }
    if (_lf_version()) {
        return SCE_EVERSIONMISS;
    }
    strncpy(_senddata.path, path, sizeof(_senddata.path));
    _senddata.path[sizeof(_senddata.path) - 1] = 0;
    strncpy(_senddata.args, secname, sizeof(_senddata.args));
    _senddata.args[sizeof(_senddata.args) - 1] = 0;
    if (sceSifCallRpc(&cd, fno, 0, &_senddata, sizeof(_senddata), &_senddata, 16, 0, 0) < 0) {
        return SCE_ECALLMISS;
    }
    if (_senddata.id == 0) {
        return SCE_ELOADMISS;
    }
    data->epc = _senddata.id;
    data->gp = _senddata.arg;
    return 0;
}

int sceSifLoadElfPart(const char *path, const char *secname, sceExecData *data)
{
    return _sceSifLoadElfPart(path, secname, data, 1);
}

int sceSifLoadElf(const char *path, sceExecData *data)
{
    return _sceSifLoadElfPart(path, "all", data, 1);
}

/* Read a byte (type 0), halfword (1) or word (2) of IOP memory. */
int sceSifGetIopAddr(unsigned int addr, void *buf, unsigned int type)
{
    if (_lf_bind() < 0) {
        return SCE_EBINDMISS;
    }
    if (type >= 3) {
        return SCE_EILLEGALTYPE;
    }
    _senddata.id = addr;
    _senddata.arg = type;
    if (sceSifCallRpc(&cd, 3, 0, &_senddata, 0x20, &_senddata, 0x20, 0, 0) < 0) {
        return SCE_ECALLMISS;
    }
    if (type == 0) {
        *(unsigned char *)buf = *(unsigned char *)&_senddata.id;
    } else if (type == 1) {
        *(unsigned short *)buf = *(unsigned short *)&_senddata.id;
    } else if (type == 2) {
        *(unsigned int *)buf = _senddata.id;
    } else {
        return SCE_EILLEGALTYPE;
    }
    return 0;
}

int sceSifSetIopAddr(unsigned int addr, void *buf, unsigned int type)
{
    if (_lf_bind() < 0) {
        return SCE_EBINDMISS;
    }
    _senddata.id = addr;
    _senddata.arg = type;
    if (type == 0) {
        *(unsigned char *)_senddata.path = *(unsigned char *)buf;
    } else if (type == 1) {
        *(unsigned short *)_senddata.path = *(unsigned short *)buf;
    } else if (type == 2) {
        *(unsigned int *)_senddata.path = *(unsigned int *)buf;
    } else {
        return SCE_EILLEGALTYPE;
    }
    if (sceSifCallRpc(&cd, 2, 0, &_senddata, 0x20, &_senddata, 0x10, 0, 0) < 0) {
        return SCE_ECALLMISS;
    }
    return 0;
}
// SOURCE DATA: .data .rodata .bss
// SOURCE DATA SLUS_217.99_retail: .bss .data .rodata
