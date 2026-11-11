/* GCC 2.9 libgcc2.c, L__main: global constructors/destructors (no init section, no EH frame registration). */
typedef void (*func_ptr)(void);

extern func_ptr __CTOR_LIST__[];
extern func_ptr __DTOR_LIST__[];

/* gbl-ctors.h */
#define DO_GLOBAL_CTORS_BODY                                      \
    do {                                                          \
        unsigned long nptrs = (unsigned long)__CTOR_LIST__[0];    \
        unsigned i;                                               \
        if (nptrs == (unsigned long)-1)                           \
            for (nptrs = 0; __CTOR_LIST__[nptrs + 1] != 0; nptrs++) \
                ;                                                 \
        for (i = nptrs; i >= 1; i--)                              \
            __CTOR_LIST__[i]();                                   \
    } while (0)

void __do_global_dtors(void)
{
    static func_ptr *p = __DTOR_LIST__ + 1;
    while (*p) {
        p++;
        (*(p - 1))();
    }
}

void __do_global_ctors(void)
{
    DO_GLOBAL_CTORS_BODY;
}

static int initialized;

void __main(void)
{
    /* Support recursive calls to `main': run initializers just once.  */
    if (!initialized) {
        initialized = 1;
        __do_global_ctors();
    }
}
// SOURCE DATA: .data .bss
// SOURCE DATA SLUS_217.99_retail: .bss .data
