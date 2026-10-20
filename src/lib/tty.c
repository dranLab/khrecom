/* The EE's tty over DECI2 (protocol 0x210): writes are sent as packets to the host, received bytes go to a queue. */
#include "sdk/eekernel.h"

int kprintf(const char *format, ...);
int sceDeci2Open(unsigned short protocol, void *opt, void (*handler)(int event, int param, void *opt));
int sceDeci2ReqSend(int s, char dest);
void sceDeci2Poll(int s);  /* int in deci2.c; declared void here as the original register use shows */
int sceDeci2ExRecv(int s, void *buf, unsigned short len);
int sceDeci2ExSend(int s, void *buf, unsigned short len);

#define DECI2_READ 1
#define DECI2_READDONE 2
#define DECI2_WRITE 3
#define DECI2_WRITEDONE 4

#define UNCACHED(p) ((char *)((unsigned int)(p) | 0x20000000))

typedef struct {
    unsigned short len;
    unsigned short rsvd;
    unsigned short proto;
    char src;
    char dest;
} DECI2_HDR;

typedef struct {
    DECI2_HDR deci2;
    int tty;
    char data[0x134];
} TTY_PACKET;

typedef struct {
    int size;
    volatile int count;
    unsigned char *rp;
    unsigned char *wp;
    unsigned char buf[0x100];
} Queue;

typedef struct {
    volatile int s;
    volatile int wlen;
    volatile int rlen;
    volatile int wbusy;
    char *wptr;
    char *rptr;
    Queue *q;
} TTY_INFO;

static TTY_INFO tinfo;
static TTY_PACKET wbuf __attribute__((aligned(64)));
static TTY_PACKET rbuf __attribute__((aligned(64)));

static Queue *QueueInit(int size)
{
    static Queue q;

    q.size = size;
    q.count = 0;
    q.wp = q.buf;
    q.rp = q.buf;
    return &q;
}

static void QueuePeekWriteDone(Queue *q)
{
    q->count++;
    if (++q->wp == q->buf + q->size) {
        q->wp = q->buf;
    }
}

static void QueuePeekReadDone(Queue *q)
{
    q->count--;
    if (++q->rp == q->buf + q->size) {
        q->rp = q->buf;
    }
}

static void sceTtyHandler(int event, int param, void *opt)
{
    TTY_INFO *ti = opt;
    char *r;
    int n;

    switch (event) {
    case DECI2_READ:
    case DECI2_READDONE:
        if (param) {
            if (ti->rlen + param > sizeof(rbuf)) {
                kprintf("TTY: packet size larger than expect\n");
            }
            param = sceDeci2ExRecv(ti->s, ti->rptr + ti->rlen, param);
            if (param < 0) {
                kprintf("TTY: receive error");
            }
            ti->rlen += param;
        } else {
            r = ti->rptr;
            for (param = sizeof(DECI2_HDR) + sizeof(int); param < ((DECI2_HDR *)r)->len; param++) {
                *ti->q->wp = ti->rptr[param];
                QueuePeekWriteDone(ti->q);
            }
            ti->rlen = 0;
        }
        break;
    case DECI2_WRITE:
        n = sceDeci2ExSend(ti->s, ti->wptr, ti->wlen);
        if (n < 0) {
            kprintf("TTY: send err %d\n", n);
            ti->wbusy = 0;
        } else {
            ti->wptr += n;
            ti->wlen -= n;
        }
        break;
    case DECI2_WRITEDONE:
        if (ti->wlen) {
            kprintf("TTY: err ti->wlen=%08x\n", ti->wlen);
        }
        ti->wbusy = 0;
        break;
    }
}

int sceTtyWrite(const char *buf, int len)
{
    char *pkt, *p;
    int i = 0, n = 0;
    int eie;

    if (tinfo.wbusy) {
        return -1;
    }
    eie = DIntr();
    tinfo.wbusy = 1;
    pkt = UNCACHED(&wbuf);
    tinfo.wptr = pkt;
    p = pkt + sizeof(DECI2_HDR) + sizeof(int);
    while (len--) {
        if (*buf == '\n') {
            *p++ = '\r';
            if (++i >= 0x100) {
                break;
            }
        }
        *p++ = *buf++;
        i++;
        n++;
        if (i >= 0x100) {
            break;
        }
    }
    tinfo.wlen = i + sizeof(DECI2_HDR) + sizeof(int);
    ((DECI2_HDR *)pkt)->len = tinfo.wlen;
    if (sceDeci2ReqSend(tinfo.s, ((DECI2_HDR *)pkt)->dest) < 0) {
        tinfo.wbusy = 0;
        if (eie) {
            EIntr();
        }
        return -1;
    }
    while (tinfo.wbusy) {
        sceDeci2Poll(tinfo.s);
    }
    if (eie) {
        EIntr();
    }
    return n;
}

int sceTtyRead(char *buf, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        while (tinfo.q->count == 0) {
        }
        buf[i] = *tinfo.q->rp;
        QueuePeekReadDone(tinfo.q);
        if (buf[i] == '\n' || buf[i] == '\r') {
            return i + 1;
        }
    }
    return i;
}

int sceTtyInit(void)
{
    TTY_PACKET *pkt;

    FlushCache(0);
    tinfo.s = sceDeci2Open(0x210, &tinfo, sceTtyHandler);
    if (tinfo.s < 0) {
        return 0;
    }
    tinfo.wbusy = 0;
    tinfo.wlen = 0;
    tinfo.rlen = 0;
    tinfo.rptr = UNCACHED(&rbuf);
    pkt = (TTY_PACKET *)UNCACHED(&wbuf);
    tinfo.wptr = (char *)pkt;
    pkt->deci2.proto = 0x210;
    pkt->deci2.src = 'E';
    pkt->deci2.dest = 'H';
    pkt->deci2.rsvd = 0;
    pkt->tty = 0;
    tinfo.q = QueueInit(0x100);
    return 1;
}
// SOURCE DATA: .rodata .bss
