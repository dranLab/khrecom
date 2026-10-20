/* newlib libc/stdio/fwalk.c */
#define NULL 0

typedef struct __sFILE {
    char _pad[0xc];
    short _flags;
    char _pad2[0x58 - 0xe];
} FILE;

struct _glue {
    struct _glue *_next;
    int _niobs;
    FILE *_iobs;
};

struct _reent {
    char _pad[0x1d8];
    struct _glue __sglue;
};

int _fwalk(struct _reent *ptr, register int (*function)(FILE *))
{
    register FILE *fp;
    register int n, ret = 0;
    register struct _glue *g;

    for (g = &ptr->__sglue; g != NULL; g = g->_next)
        for (fp = g->_iobs, n = g->_niobs; --n >= 0; fp++)
            if (fp->_flags != 0)
                ret |= (*function)(fp);
    return ret;
}
