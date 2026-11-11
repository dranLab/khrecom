/* ATRAC3 decoder (libat3dec): work area setup and the channel mode names. */

int strcmp(const char *s1, const char *s2);

extern int chswTable[];
extern int chwtTable[];

typedef struct {
    int *sw[4];
    int *wt;
    int n;
} AT3_HDR;

typedef struct {
    int a;
    int b;
    char rest[0x38];
} AT3_BAND;

typedef struct {
    char data[0x32D4];
    AT3_BAND band[4];
    char tail[0xC];
} AT3_CHANNEL;

typedef struct {
    AT3_HDR cur;
    AT3_HDR prev;
    AT3_CHANNEL ch[2];
    int pad;
    int nch;
    int x;
    int y;
} AT3_WORK;

void init_at3dec(int nch, int x, int y, AT3_WORK *work)
{
    int i, j;

    work->x = x;
    work->y = y;
    work->cur.wt = &chwtTable[15];
    work->cur.n = 4;
    work->nch = nch;
    for (i = 0; i < 4; i++) {
        work->cur.sw[i] = &chswTable[3];
        for (j = 0; j < nch; j++) {
            work->ch[j].band[i].b = 4;
            work->ch[j].band[i].a = 0xFF;
        }
    }
    /* on one line with the brace: a line label before the return keeps ee-as from filling its delay slot */
    work->prev = work->cur; }

int atrac2_numof_channels(int mode)
{
    return mode & 0xF;
}

int atrac2_mode_str2id(char *str)
{
    if (!strcmp(str, "MONO")) {
        return 1;
    }
    if (!strcmp(str, "STEREO")) {
        return 2;
    }
    if (!strcmp(str, "4CH")) {
        return 4;
    }
    if (!strcmp(str, "8CH")) {
        return 8;
    }
    if (!strcmp(str, "JOINT")) {
        return 0x12;
    }
    return 0;
}

char *atrac2_mode_id2str(int id)
{
    switch (id) {
    case 1:
        return "MONO";
    case 2:
        return "STEREO";
    case 4:
        return "4CH";
    case 8:
        return "8CH";
    case 0x12:
        return "JOINT";
    }
    return "UNKNOWN";
}
// SOURCE DATA: .rodata
// SOURCE DATA SLUS_217.99_retail: .rodata
