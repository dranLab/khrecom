/* _InitSys: the libkernl start-up (semaphores for libc, the kernel's system call table, the sub-systems). */
#include "sdk/eekernel.h"

void InitAlarm(void);
void InitTimer(int mode);
void StartTimerSystemTime(void);
void InitThread(void);
void InitExecPS2(void);
void InitTLBFunctions(void);
void sceSifInitRebootNotify(void);

struct syscall_entry {
    int number;
    void *function;
};

static unsigned int *_SyscallEntry = 0;

static unsigned int *kFindAddress(unsigned int *p, unsigned int *end, unsigned int value);
static int kCopy(unsigned int *dst, unsigned int *src, unsigned int size);
static void setup(int number, void *function);

static struct syscall_entry SysEntry[] = {
    { 0x83, kFindAddress },
    { 0x5A, kCopy },
};

int __sce_sema_id = 0;
int __sce_eh_sema_id = 0;

static void supplement_crt0(void)
{
    struct SemaParam sema, eh;

    sema.option = (unsigned int)"SceKernelLibc";
    sema.maxCount = 1;
    sema.initCount = 1;
    eh.maxCount = 1;
    eh.initCount = 1;
    eh.option = (unsigned int)"SceKernelLibcEh";
    __sce_sema_id = CreateSema(&sema);
    __sce_eh_sema_id = CreateSema(&eh);
}

int __sceKernlGetEhSemaId(void)
{
    return __sce_eh_sema_id;
}

/* System call stubs: the arguments are already in a0..a2, the result comes back in v0. */
static int Copy(void *dst, void *src, int size)
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

/* The first word in [p, end) that equals value, or 0. */
static unsigned int *kFindAddress(unsigned int *p, unsigned int *end, unsigned int value)
{
    while (*p != value && p < end) {
        p++;
    }
    if (p >= end) {
        return 0;
    }
    return p;
}

static unsigned int *FindAddress(unsigned int *p, unsigned int *end, void *value)
{
    asm volatile ("li\t$3, 0x83\n\tsyscall");
}

unsigned int GetSystemCallEntry(int number)
{
    unsigned int entry;

    Copy(&entry, _SyscallEntry + number, sizeof(entry));
    return entry;
}

/* Find the kernel's system call table: the one place where kFindAddress (call 0x83) and kCopy (call 0x5A),
 * just installed, are both found at their numbers. */
static void InitSystemCallTableAddress(void)
{
    unsigned int *find, *copy, *table1, *table2;

    setup(SysEntry[0].number, SysEntry[0].function);
    setup(SysEntry[1].number, SysEntry[1].function);
    find = FindAddress((unsigned int *)0x80000000, (unsigned int *)0x80080000, kFindAddress);
    copy = FindAddress((unsigned int *)0x80000000, (unsigned int *)0x80080000, kCopy);
    table1 = find - 0x83;
    table2 = copy - 0x5A;
    while (table1 != table2) {
        if (table1 < table2) {
            find = FindAddress(find + 1, (unsigned int *)0x80080000, kFindAddress);
            table1 = find - 0x83;
        } else {
            copy = FindAddress(copy + 1, (unsigned int *)0x80080000, kCopy);
            table2 = copy - 0x5A;
        }
    }
    _SyscallEntry = table1;
}

static void setup(int number, void *function)
{
    asm volatile ("li\t$3, 0x74\n\tsyscall");
}

void _InitSys(void)
{
    supplement_crt0();
    InitSystemCallTableAddress();
    InitAlarm();
    InitTimer(2);
    StartTimerSystemTime();
    InitThread();
    InitExecPS2();
    InitTLBFunctions();
    sceSifInitRebootNotify();
}
// SOURCE DATA: .data .rodata
