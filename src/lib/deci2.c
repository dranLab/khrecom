/* DECI2 (debugger protocol) for the EE: each call packs its arguments for the kernel's Deci2Call. */

int Deci2Call(int func, int *param);

/* the DECI2 manager's work area, used uncached */
static char userarea[0x1038];

int sceDeci2Open(unsigned short protocol, void *opt, void (*handler)(int event, int param, void *opt))
{
    int param[4];

    param[1] = (int)opt;
    param[0] = protocol;
    param[2] = (int)handler;
    param[3] = (int)userarea | 0x20000000;
    return Deci2Call(1, param);
}

int sceDeci2Close(int s)
{
    int param[4];

    param[0] = s;
    return Deci2Call(2, param);
}

int sceDeci2ReqSend(int s, char dest)
{
    int param[4];

    param[0] = s;
    param[1] = dest;
    return Deci2Call(3, param);
}

int sceDeci2Poll(int s)
{
    int param[4];

    param[0] = s;
    return Deci2Call(4, param);
}

int sceDeci2GrpOpen(unsigned char group, void *opt, void (*handler)(int event, int param, void *opt))
{
    int param[5];

    param[1] = (int)opt;
    param[0] = group;
    param[2] = (int)handler;
    return Deci2Call(0x20, param);
}

int sceDeci2GrpClose(int s)
{
    int param[4];

    param[0] = s;
    return Deci2Call(0x21, param);
}

int sceDeci2ExRecv(int s, void *buf, unsigned short len)
{
    int param[4];

    param[0] = s;
    param[1] = (int)buf;
    param[2] = len;
    return Deci2Call(-5, param);
}

int sceDeci2ExSend(int s, void *buf, unsigned short len)
{
    int param[4];

    param[0] = s;
    param[1] = (int)buf;
    param[2] = len;
    return Deci2Call(-6, param);
}

int sceDeci2ExReqSend(int s, char dest)
{
    int param[4];

    param[0] = s;
    param[1] = dest;
    return Deci2Call(-7, param);
}

int sceDeci2ExLock(int s)
{
    int param[4];

    param[0] = s;
    return Deci2Call(-8, param);
}

int sceDeci2ExUnLock(int s)
{
    int param[4];

    param[0] = s;
    return Deci2Call(-9, param);
}

int kputs(char *s)
{
    int param[4];

    param[0] = (int)s;
    return Deci2Call(0x10, param);
}
// SOURCE DATA: .bss
// SOURCE DATA SLUS_217.99_retail: .bss
