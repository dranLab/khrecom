/* TLB set-up for the EE kernel (kernel/default/extended mappings, 32 MB consoles) and the TLB/debug exception
 * handler tables used by tlbtrap.s. */
#include "sdk/eekernel.h"

int kprintf(const char *format, ...);
void Exit(int status);
int GetMemorySize(void);
int _InitTLB(void);
void SetVTLBRefillHandler(int cause, void *handler);
void SetVCommonHandler(int cause, void *handler);
extern char _kTLBException[];
extern char _kDebugException[];
extern char _kExitTLBHandler[];

struct syscall_entry {
    int number;
    void *function;
};

typedef struct {
    unsigned int PageMask;
    unsigned int EntryHi;
    unsigned int EntryLo0;
    unsigned int EntryLo1;
} TLBEntry;

/* The TLB system calls' code for kernel memory at 0x80075000 (prebuilt, copied as is). */
static unsigned int tlbsrc[] = {
    0x3C028007, 0x0000282D, 0x24435300, 0x00000000, 0x8C620000, 0x14820003,
    0x24A50001, 0x03E00008, 0x8C620004, 0x2CA20006, 0x1440FFF9, 0x24630008,
    0x03E00008, 0x0000102D, 0x00051602, 0x304300F0, 0x24020030, 0x10620014,
    0x2C620031, 0x10400009, 0x24020010, 0x1062000E, 0x2C620011, 0x1040000A,
    0x24020020, 0x1060000C, 0x2402FFFF, 0x10000014, 0x00000000, 0x24020050,
    0x10620005, 0x2C620051, 0x10400003, 0x24020040, 0x10620003, 0x00000000,
    0x03E00008, 0x2402FFFF, 0x40842800, 0x40855000, 0x40861000, 0x40871800,
    0x0000040F, 0x42000006, 0x0000040F, 0x42000008, 0x0000040F, 0x40020000,
    0x03E00008, 0x00000000, 0x2C820030, 0x1040000B, 0x2402FFFF, 0x40840000,
    0x40852800, 0x40865000, 0x40871000, 0x40881800, 0x0000040F, 0x42000002,
    0x0000040F, 0x03E00008, 0x0080102D, 0x03E00008, 0x00000000, 0x00000000,
    0x2C820030, 0x14400003, 0x00000000, 0x03E00008, 0x2402FFFF, 0x40840000,
    0x0000040F, 0x42000001, 0x0000040F, 0x40022800, 0xACA20000, 0x40035000,
    0xACC30000, 0x40021000, 0xACE20000, 0x40031800, 0xAD030000, 0x03E00008,
    0x0080102D, 0x00000000, 0x40845000, 0x0000040F, 0x42000008, 0x0000040F,
    0x40040000, 0x04810003, 0x00000000, 0x10000009, 0x2404FFFF, 0x42000001,
    0x0000040F, 0x40022800, 0xACA20000, 0x40031000, 0xACC30000, 0x40021800,
    0xACE20000, 0x03E00008, 0x0080102D, 0x00000000, 0x27BDFFD0, 0xFFB00010,
    0x0080802D, 0x32020FFF, 0x14400007, 0xFFBF0020, 0x3C02000F, 0x2603FFFF,
    0x3442FFFE, 0x0043102B, 0x14400003, 0x3C047000, 0x1000003B, 0x2402FFFF,
    0x03A0282D, 0x34844000, 0x37A60004, 0x0C01D456, 0x37A70008, 0x0040282D,
    0x04A10009, 0x00000000, 0x12000031, 0x0000102D, 0x40053000, 0x24A20001,
    0x40823000, 0x0000040F, 0x10000013, 0x00000000, 0x16000011, 0x24A2FFFF,
    0x3C03E001, 0x00021340, 0x00433021, 0x40023000, 0x2442FFFF, 0x40823000,
    0x40850000, 0x40802800, 0x40865000, 0x40801000, 0x40801800, 0x0000040F,
    0x42000002, 0x0000040F, 0x10000019, 0x0000102D, 0x3C02FFFF, 0x26041000,
    0x3442F000, 0x3C067000, 0x00822024, 0xAFA00000, 0x02021024, 0x00042182,
    0x00021182, 0x3484001F, 0x3442001F, 0x34C64000, 0xAFA20004, 0xAFA40008,
    0x40850000, 0x0000182D, 0x40832800, 0x40865000, 0x40821000, 0x40841800,
    0x0000040F, 0x42000002, 0x0000040F, 0x00A0102D, 0xDFBF0020, 0xDFB00010,
    0x03E00008, 0x27BD0030, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000055, 0x80075038, 0x00000056, 0x800750C8, 0x00000057, 0x80075108,
    0x00000058, 0x80075158, 0x00000059, 0x800751A8, 0x00000003, 0x80075330,
};

void *_kTLBRefillHandler = 0;
void *_kDebugHandler[16] = { 0 };
char **_kExecArg = 0;

static int kCopy(unsigned int *dst, unsigned int *src, unsigned int size);

static struct syscall_entry SysEntry[] = {
    { 0x5A, kCopy },
    { 0x5B, (void *)0x80075000 },
    { 0x54, _kExitTLBHandler },
    { 0x55, 0 },
    { 0x56, 0 },
    { 0x57, 0 },
    { 0x58, 0 },
    { 0x59, 0 },
};

static TLBEntry kernelTLB[] = {
    { 0x00000000, 0x70000000, 0x80000007, 0x00000007 },
    { 0x00006000, 0xFFFF8000, 0x00001E1F, 0x00001F1F },
    { 0x00000000, 0x10000000, 0x00400017, 0x00400053 },
    { 0x00000000, 0x10002000, 0x00400097, 0x004000D7 },
    { 0x00000000, 0x10004000, 0x00400117, 0x00400157 },
    { 0x00000000, 0x10006000, 0x00400197, 0x004001D7 },
    { 0x00000000, 0x10008000, 0x00400217, 0x00400257 },
    { 0x00000000, 0x1000A000, 0x00400297, 0x004002D7 },
    { 0x00000000, 0x1000C000, 0x00400313, 0x00400357 },
    { 0x00000000, 0x1000E000, 0x00400397, 0x004003D7 },
    { 0x0001E000, 0x11000000, 0x00440017, 0x00440415 },
    { 0x0001E000, 0x12000000, 0x00480017, 0x00480415 },
    { 0x01FFE000, 0x1E000000, 0x00780017, 0x007C0017 },
};

static TLBEntry defaultTLB[] = {
    { 0x0007E000, 0x00080000, 0x0000201F, 0x0000301F },
    { 0x0007E000, 0x00100000, 0x0000401F, 0x0000501F },
    { 0x0007E000, 0x00180000, 0x0000601F, 0x0000701F },
    { 0x001FE000, 0x00200000, 0x0000801F, 0x0000C01F },
    { 0x001FE000, 0x00400000, 0x0001001F, 0x0001401F },
    { 0x001FE000, 0x00600000, 0x0001801F, 0x0001C01F },
    { 0x007FE000, 0x00800000, 0x0002001F, 0x0003001F },
    { 0x007FE000, 0x01000000, 0x0004001F, 0x0005001F },
    { 0x007FE000, 0x01800000, 0x0006001F, 0x0007001F },
    { 0x0007E000, 0x20080000, 0x00002017, 0x00003017 },
    { 0x0007E000, 0x20100000, 0x00004017, 0x00005017 },
    { 0x0007E000, 0x20180000, 0x00006017, 0x00007017 },
    { 0x001FE000, 0x20200000, 0x00008017, 0x0000C017 },
    { 0x001FE000, 0x20400000, 0x00010017, 0x00014017 },
    { 0x001FE000, 0x20600000, 0x00018017, 0x0001C017 },
    { 0x007FE000, 0x20800000, 0x00020017, 0x00030017 },
    { 0x007FE000, 0x21000000, 0x00040017, 0x00050017 },
    { 0x007FE000, 0x21800000, 0x00060017, 0x00070017 },
};

static TLBEntry extendTLB[] = {
    { 0x0007E000, 0x30100000, 0x0000403F, 0x0000503F },
    { 0x0007E000, 0x30180000, 0x0000603F, 0x0000703F },
    { 0x001FE000, 0x30200000, 0x0000803F, 0x0000C03F },
    { 0x001FE000, 0x30400000, 0x0001003F, 0x0001403F },
    { 0x001FE000, 0x30600000, 0x0001803F, 0x0001C03F },
    { 0x007FE000, 0x30800000, 0x0002003F, 0x0003003F },
    { 0x007FE000, 0x31000000, 0x0004003F, 0x0005003F },
    { 0x007FE000, 0x31800000, 0x0006003F, 0x0007003F },
};

struct {
    int nKernel;
    int nDefault;
    int nExtend;
    int nWired;
    TLBEntry *kernel;
    TLBEntry *def;
    TLBEntry *extend;
} TLBInfo = {
    sizeof(kernelTLB) / sizeof(TLBEntry), sizeof(defaultTLB) / sizeof(TLBEntry), sizeof(extendTLB) / sizeof(TLBEntry), 0,
    kernelTLB, defaultTLB, extendTLB,
};

void *SetTLBHandler(void *handler)
{
    _kTLBRefillHandler = handler;
    SetVTLBRefillHandler(1, _kTLBException);
    SetVTLBRefillHandler(2, _kTLBException);
    SetVTLBRefillHandler(3, _kTLBException);
    return handler;
}

void *SetDebugHandler(int cause, void *handler)
{
    void *old;

    if ((unsigned int)(cause - 1) >= 13) {
        return (void *)-1;
    }
    old = _kDebugHandler[cause];
    _kDebugHandler[cause] = handler;
    if ((unsigned int)(cause - 1) < 3) {
        SetVTLBRefillHandler(cause, _kDebugException);
    } else {
        SetVCommonHandler(cause, _kDebugException);
    }
    return old;
}

/* System call stubs: the arguments are already in a0..t0, the result comes back in v0. */
static void Copy(void *dst, void *src, int size)
{
    asm volatile ("li\t$3, 0x5A\n\tsyscall");
}

static int kCopy(unsigned int *dst, unsigned int *src, unsigned int size)
{
    unsigned int i;

    size >>= 2;
    for (i = 0; i < size; i++) {
        *dst++ = *src++;
    }
    return 0;
}

static void *GetEntryAddress(int number)
{
    asm volatile ("li\t$3, 0x5B\n\tsyscall");
}

static void setup(int number, void *function)
{
    asm volatile ("li\t$3, 0x74\n\tsyscall");
}

void InitTLBFunctions(void)
{
    unsigned int i;

    setup(SysEntry[0].number, SysEntry[0].function);
    Copy((void *)0x80075000, tlbsrc, sizeof(tlbsrc));
    FlushCache(0);
    FlushCache(2);
    setup(SysEntry[1].number, SysEntry[1].function);
    setup(SysEntry[2].number, SysEntry[2].function);
    for (i = 3; i < 8; i++) {
        setup(SysEntry[i].number, GetEntryAddress(SysEntry[i].number));
    }
    _kExecArg = GetEntryAddress(3);
}

#define SYSCALL(name, number) \
    int name() \
    { \
        asm volatile ("li\t$3, " #number "\n\tsyscall"); \
    }

SYSCALL(PutTLBEntry, 0x55)
SYSCALL(iPutTLBEntry, -0x55)

static int _SetTLBEntry(unsigned int index, unsigned int pageMask, unsigned int entryHi, unsigned int entryLo0,
                        unsigned int entryLo1)
{
    asm volatile ("li\t$3, 0x56\n\tsyscall");
}

/* Indices below 13 are the kernel's, the TLB has 48. */
int SetTLBEntry(unsigned int index, unsigned int pageMask, unsigned int entryHi, unsigned int entryLo0,
                unsigned int entryLo1)
{
    if (index - 13 >= 35) {
        return -1;
    }
    return _SetTLBEntry(index, pageMask, entryHi, entryLo0, entryLo1);
}

SYSCALL(iSetTLBEntry, -0x56)
SYSCALL(GetTLBEntry, 0x57)
SYSCALL(iGetTLBEntry, -0x57)
SYSCALL(ProbeTLBEntry, 0x58)
SYSCALL(iProbeTLBEntry, -0x58)
SYSCALL(ExpandScratchPad, 0x59)

static int InitTLB32MB(void);

int InitTLB(void)
{
    if (GetMemorySize() == 0x2000000) {
        return InitTLB32MB();
    }
    return _InitTLB();
}

/* Map 32 MB: the kernel's, the default (wired) and the extended entries, the rest invalid. */
static int InitTLB32MB(void)
{
    register int i asm("$25");
    TLBEntry *e;
    int n, j;

    kprintf("# TLB spad=0 kernel=1:%d default=%d:%d extended=%d:%d\n", TLBInfo.nKernel - 1, TLBInfo.nKernel,
            TLBInfo.nKernel + TLBInfo.nDefault - 1, TLBInfo.nKernel + TLBInfo.nDefault,
            TLBInfo.nKernel + TLBInfo.nDefault + TLBInfo.nExtend - 1);
    asm volatile ("mtc0\t$0, $6\n\tsync.p");
    n = TLBInfo.nKernel;
    i = 0;
    if (n > 48) {
        kprintf("# TLB over flow (1)");
        Exit(1);
    }
    for (e = TLBInfo.kernel; i < n; i++, e++) {
        _SetTLBEntry(i, e->PageMask, e->EntryHi, e->EntryLo0, e->EntryLo1);
    }
    n = i + TLBInfo.nDefault;
    if (n > 48) {
        kprintf("# TLB over flow (2)");
        Exit(1);
    }
    for (e = TLBInfo.def; i < n; i++, e++) {
        _SetTLBEntry(i, e->PageMask, e->EntryHi, e->EntryLo0, e->EntryLo1);
    }
    TLBInfo.nWired = i;
    asm volatile ("mtc0\t%0, $6\n\tsync.p" : : "r" (i));
    if (TLBInfo.nExtend > 0) {
        n = i + TLBInfo.nExtend;
        if (n > 48) {
            kprintf("# TLB over flow (3)");
            Exit(1);
        }
        for (e = TLBInfo.extend; i < n; i++, e++) {
            _SetTLBEntry(i, e->PageMask, e->EntryHi, e->EntryLo0, e->EntryLo1);
        }
    }
    for (j = i; j < 48; j++) {
        _SetTLBEntry(j, 0, 0xE0000000 + (j << 13), 0, 0);
    }
    return i;
}
// SOURCE DATA: .data .rodata
