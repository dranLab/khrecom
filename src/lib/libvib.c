/* libvib (Sony EE SDK): controller vibration through the DBC (dbcman) driver. */

typedef struct {
    unsigned long cmd : 14;
    unsigned long mode : 2;
    unsigned long type : 8;
    unsigned long size : 8;
    unsigned long pad : 32;
} sceDbcCmd;

void *memcpy(void *dst, const void *src, unsigned int n);
int sceDbcReceiveData(int port, sceDbcCmd cmd, void *rdata, void *data);
int sceDbcSendData2(int port, sceDbcCmd cmd, void *rdata, void *sdata);

static char __ps2_libinfo__[16] = "PsIIlibvib  3100";

int sceVibGetProfile(int port, unsigned char *profile)
{
    sceDbcCmd cmd;
    int rdata[4];
    int ret;

    cmd.cmd = 2;
    cmd.mode = 2;
    cmd.type = 3;
    cmd.size = 1;
    ret = sceDbcReceiveData(port, cmd, rdata, profile);
    if (ret < 0) {
        return ret;
    }
    return rdata[0];
}

int sceVibSetActParam(int port, int size2, unsigned char *data2, int size1, unsigned char *data1)
{
    sceDbcCmd cmd;
    struct {
        int size2;
        int size1;
        unsigned char data[0x28];
    } sdata;
    int rsize;

    rsize = 0x28;
    cmd.cmd = 0xb;
    cmd.mode = 1;
    cmd.type = 3;
    cmd.size = 1;
    sdata.size1 = size1;
    memcpy(sdata.data, data1, size1);
    sdata.size2 = size2;
    memcpy(sdata.data + size1, data2, size2);
    return sceDbcSendData2(port, cmd, &rsize, &sdata);
}
// SOURCE DATA: .data
