/* libc glue for the EE: tty I/O through DECI2, the heap, VSync, and stubs for the rest. */
#include "sdk/eekernel.h"

#define INTC_STAT ((volatile int *)0x1000F000)
#define INTC_VBLANK_S 4

int *__errno(void);
#define errno (*__errno())
#define EIO 5
#define ENOMEM 12

int sceTtyInit(void);
int sceTtyWrite(const char *buf, int len);
int sceTtyRead(char *buf, int len);
void *EndOfHeap(void);
void SetVSyncFlag(volatile int *flag, volatile unsigned long *count);
void Exit(int status);

extern char end[];

/* newlib's struct stat (long is 64 bits) */
struct stat {
    short st_dev;
    unsigned short st_ino;
    unsigned int st_mode;
    unsigned short st_nlink;
    unsigned short st_uid;
    unsigned short st_gid;
    short st_rdev;
    long st_size;
    long st_atime;
    long st_spare1;
    long st_mtime;
    long st_spare2;
    long st_ctime;
    long st_spare3;
    long st_blksize;
    long st_blocks;
    long st_spare4[2];
};

static int ttyinit = 0;

void sceResetttyinit(void)
{
    ttyinit = 0;
}

/* Wait for the next vertical blank start. */
void VSync(void)
{
    int eie;

    eie = DIntr();
    *INTC_STAT = INTC_VBLANK_S;
    asm volatile ("sync");
    if (eie) {
        EIntr();
    }
    while (!(*INTC_STAT & INTC_VBLANK_S)) {
    }
    eie = DIntr();
    *INTC_STAT = INTC_VBLANK_S;
    asm volatile ("sync");
    if (eie) {
        EIntr();
    }
}

/* As VSync, also woken by the kernel's VSync flag; returns its counter. */
unsigned long VSync2(void)
{
    volatile int flag = 0;
    volatile unsigned long count;
    int eie;

    SetVSyncFlag(&flag, &count);
    eie = DIntr();
    *INTC_STAT = INTC_VBLANK_S;
    asm volatile ("sync");
    if (eie) {
        EIntr();
    }
    while (!(*INTC_STAT & INTC_VBLANK_S) && !flag) {
    }
    eie = DIntr();
    *INTC_STAT = INTC_VBLANK_S;
    asm volatile ("sync");
    if (eie) {
        EIntr();
    }
    return count;
}

int write(int fd, const char *buf, int len)
{
    if (fd == 1 || fd == 2) {
        if (!ttyinit) {
            if (!sceTtyInit()) {
                return -1;
            }
            ttyinit = 1;
        }
        return sceTtyWrite(buf, len);
    }
    return -1;
}

int read(int fd, char *buf, int len)
{
    if (fd == 0) {
        if (!ttyinit) {
            if (!sceTtyInit()) {
                return -1;
            }
            ttyinit = 1;
        }
        return sceTtyRead(buf, len);
    }
    return -1;
}

int open(const char *name, int flags, int mode)
{
    errno = EIO;
    return -1;
}

int close(int fd)
{
    return -1;
}

int ioctl(int fd, int request, void *arg)
{
    return -1;
}

int lseek(int fd, int offset, int whence)
{
    return -1;
}

void *sbrk(int incr)
{
    static char *heap_ptr = end;
    char *prev, *next;
    int eie, status;

    asm volatile ("mfc0\t%0, $12" : "=r" (eie));
    eie &= 0x10000;
    if (eie) {
        do {
            asm volatile ("di");
            asm volatile ("sync.p");
            asm volatile ("mfc0\t%0, $12" : "=r" (status));
        } while (status & 0x10000);
    }
    next = heap_ptr + incr;
    if ((char *)EndOfHeap() < next) {
        errno = ENOMEM;
        if (eie) {
            asm volatile ("ei");
        }
        return (void *)-1;
    }
    prev = heap_ptr;
    heap_ptr = next;
    if (eie) {
        asm volatile ("ei");
    }
    return prev;
}

int isatty(int fd)
{
    return 1;
}

int fstat(int fd, struct stat *st)
{
    st->st_mode = 0x2000;  /* S_IFCHR */
    st->st_blksize = 0;
    return 0;
}

int getpid(void)
{
    return 1;
}

int kill(int pid, int sig)
{
    if (pid == 1) {
        Exit(sig);
    }
    return 0;
}

int stat(const char *name, struct stat *st)
{
    errno = EIO;
    return -1;
}

int unlink(const char *name)
{
    errno = EIO;
    return -1;
}
// SOURCE DATA: .data
// SOURCE DATA SLUS_217.99_retail: .data
