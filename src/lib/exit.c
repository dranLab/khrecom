/* ExecPS2 / LoadExecPS2 / Exit / ExecOSD: pass the arguments to the kernel and shut the library down first. */

int strlen(const char *s);
int InitTLB(void);
int _ExecPS2(void *entry, void *gp, int argc, char **argv);
void _LoadExecPS2(const char *name, int argc, char **argv);
void _Exit(int status);
void _ExecOSD(int argc, char **argv);

/* the kernel's argument area: argv[16] (the file name first), then the strings */
extern char **_kExecArg;

struct syscall_entry {
    int number;
    void *function;
};

static int kCopy(unsigned char *dst, unsigned char *src, unsigned int size);

static struct syscall_entry SysEntry[] = {
    { 0x5A, kCopy },
};

/* System call stubs: the arguments are already in a0/a1/a2 (Copy returning int matters for SetArg's registers). */
static int Copy(void *dst, void *src, int size)
{
    asm volatile ("li\t$3, 0x5A\n\tsyscall");
}

static int kCopy(unsigned char *dst, unsigned char *src, unsigned int size)
{
    unsigned int i;

    for (i = 0; i < size; i++) {
        *dst++ = *src++;
    }
    return 0;
}

static void setup(int number, void *function)
{
    asm volatile ("li\t$3, 0x74\n\tsyscall");
}

/* Copy name and argc (at most 15) arguments into the kernel's argument area; returns the copied name. */
static char *SetArg(const char *name, int argc, char *argv[])
{
    char **arg = _kExecArg;
    char *ptr = (char *)(arg + 16);
    char *ret = ptr;
    int len, i;

    setup(SysEntry[0].number, SysEntry[0].function);
    if (argc >= 16) {
        argc = 15;
    }
    Copy(arg++, &ptr, sizeof(ptr));
    len = strlen(name) + 1;
    Copy(ptr, (void *)name, len);
    ptr += len;
    for (i = 0; i < argc; i++) {
        Copy(arg++, &ptr, sizeof(ptr));
        len = strlen(argv[i]) + 1;
        Copy(ptr, argv[i], len);
        ptr += len;
    }
    return ret;
}

static void TerminateLibrary(void)
{
    InitTLB();
}

int ExecPS2(void *entry, void *gp, int argc, char **argv)
{
    SetArg("", argc, argv);
    TerminateLibrary();
    return _ExecPS2(entry, gp, argc, _kExecArg + 1);
}

void LoadExecPS2(const char *name, int argc, char **argv)
{
    char *file;

    file = SetArg(name, argc, argv);
    TerminateLibrary();
    _LoadExecPS2(file, argc, _kExecArg + 1);
}

void Exit(int status)
{
    TerminateLibrary();
    _Exit(status);
}

void ExecOSD(int argc, char **argv)
{
    SetArg("", argc, argv);
    TerminateLibrary();
    _ExecOSD(argc, _kExecArg + 1);
}
// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .data .rodata
