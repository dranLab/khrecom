/* newlib libc/stdio/findfp.c */
#include "stdio_local.h"

#define NEWS 4
#define ENOMEM 12

extern void *_malloc_r(struct _reent *, size_t);
extern void *memset(void *, int, size_t);
extern int __sread(void *, char *, int);
extern int __swrite(void *, const char *, int);
extern long __sseek(void *, long, int);
extern int __sclose(void *);
extern void _cleanup_r(struct _reent *);
void __sinit(struct _reent *);

static void std(FILE *ptr, int flags, int file, struct _reent *data)
{
    ptr->_p = 0;
    ptr->_r = 0;
    ptr->_w = 0;
    ptr->_flags = flags;
    ptr->_file = file;
    ptr->_bf._base = 0;
    ptr->_lbfsize = 0;
    ptr->_cookie = ptr;
    ptr->_read = __sread;
    ptr->_write = __swrite;
    ptr->_seek = __sseek;
    ptr->_close = __sclose;
    ptr->_data = data;
}

struct _glue *__sfmoreglue(struct _reent *d, register int n)
{
    struct _glue *g;
    FILE *p;

    g = (struct _glue *)_malloc_r(d, sizeof(*g) + n * sizeof(FILE));
    if (g == NULL)
        return NULL;
    p = (FILE *)(g + 1);
    g->_next = NULL;
    g->_niobs = n;
    g->_iobs = p;
    memset(p, 0, n * sizeof(FILE));
    return g;
}

/*
 * Find a free FILE for fopen et al.
 */
FILE *__sfp(struct _reent *d)
{
    FILE *fp;
    int n;
    struct _glue *g;

    if (!d->__sdidinit)
        __sinit(d);
    for (g = &d->__sglue;; g = g->_next) {
        for (fp = g->_iobs, n = g->_niobs; --n >= 0; fp++)
            if (fp->_flags == 0)
                goto found;
        if (g->_next == NULL && (g->_next = __sfmoreglue(d, NEWS)) == NULL)
            break;
    }
    d->_errno = ENOMEM;
    return NULL;

found:
    fp->_flags = 1; /* reserve this slot; caller sets real flags */
    fp->_p = NULL; /* no current pointer */
    fp->_w = 0; /* nothing to read or write */
    fp->_r = 0;
    fp->_bf._base = NULL; /* no buffer */
    fp->_bf._size = 0;
    fp->_lbfsize = 0; /* not line buffered */
    fp->_file = -1; /* no file */
    /* fp->_cookie = <any>; */ /* caller sets cookie, _read/_write etc */
    fp->_data = d;
    fp->_ub._base = NULL; /* no ungetc buffer */
    fp->_ub._size = 0;
    fp->_lb._base = NULL; /* no line buffer */
    fp->_lb._size = 0;

    return fp;
}

/*
 * __sinit() is called whenever stdio's internal variables must be set up.
 */
void __sinit(struct _reent *s)
{
    /* make sure we clean up on exit */
    s->__cleanup = _cleanup_r; /* conservative */
    s->__sdidinit = 1;

    std(s->__sf + 0, __SRD, 0, s);
    std(s->__sf + 1, __SWR | __SLBF, 1, s);
    std(s->__sf + 2, __SWR | __SNBF, 2, s);

    /* initialise glue */
    s->__sglue._next = NULL;
    s->__sglue._niobs = 3;
    s->__sglue._iobs = &s->__sf[0];
}
