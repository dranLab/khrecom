/* ATRAC3 decoder front end (middleware): init and per-frame decode into 4 blocks of PCM. */

typedef struct {
    void *in;
    short *out;
    unsigned int mode;
    char *work;
    int size;
} hncAt3decParam;

void *memset(void *dst, int c, unsigned int n);
int init_at3dec(int a, int b, int c, char *work);
int at3dec_proc(char *work);
void final_dout(short *src, short *dst);

int hncAt3dec_Init(hncAt3decParam *param)
{
    int c;
    int b;
    int ch;
    char *work;

    switch (param->mode) {
    case 1:
        ch = 2;
        b = 2;
        c = 0xc0;
        break;
    case 2:
        ch = 2;
        b = 0x12;
        c = 0x60;
        break;
    case 3:
        ch = 1;
        b = 1;
        c = 0xc0;
        break;
    case 4:
        ch = 1;
        b = 1;
        c = 0x60;
        break;
    default:
        ch = 2;
        b = 2;
        c = 0xc0;
        break;
    }
    work = param->work;
    memset(work, 0, 0x68ca);
    return init_at3dec(ch, b, c, work);
}

int hncAt3dec_Idle(hncAt3decParam *param)
{
    char *work = param->work;
    int stereo;
    int i;
    int j;
    short *out;
    short *out2;

    *(int *)(work + 0x68c4) = param->size;
    *(void **)(work + 0x6800) = param->in;
    stereo = (unsigned int)(param->mode - 1) < 2;
    if (at3dec_proc(work) == -1) {
        return -1;
    }
    i = 0;
    out = param->out;
    out2 = out + 0x100;
    if ((unsigned int)(param->mode - 1) < 2) {
        for (j = 4; j != 0; j--) {
            final_dout((short *)(work + 0x2a90) + i, out);
            out += 0x200;
            final_dout((short *)(work + stereo * 0x33e0 + 0x2a90) + i, out2);
            out2 += 0x200;
            i += 0x100;
        }
    } else {
        for (j = 4; j != 0; j--) {
            final_dout((short *)(work + 0x2a90) + i, out);
            out += 0x100;
            i += 0x100;
        }
    }
    return 0;
}
