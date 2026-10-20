/* newlib libc/stdio/fopen.c and _cleanup_r/_cleanup from findfp.c */
#include "stdio_local.h"

#define SEEK_END 2

extern int __sflags(struct _reent *, const char *, int *);
extern FILE *__sfp(struct _reent *);
extern int _open_r(struct _reent *, const char *, int, int);
extern int fseek(FILE *, long, int);
extern int fflush(FILE *);
extern int _fwalk(struct _reent *, int (*)(FILE *));
extern int __sread(void *, char *, int);
extern int __swrite(void *, const char *, int);
extern long __sseek(void *, long, int);
extern int __sclose(void *);

FILE *_fopen_r(struct _reent *ptr, const char *file, const char *mode)
{
    register FILE *fp;
    register int f;
    int flags, oflags;

    if ((flags = __sflags(ptr, mode, &oflags)) == 0)
        return NULL;
    if ((fp = __sfp(ptr)) == NULL)
        return NULL;

    if ((f = _open_r(fp->_data, file, oflags, 0666)) < 0) {
        fp->_flags = 0; /* release */
        return NULL;
    }

    fp->_file = f;
    fp->_flags = flags;
    fp->_cookie = (void *)fp;
    fp->_read = __sread;
    fp->_write = __swrite;
    fp->_seek = __sseek;
    fp->_close = __sclose;

    if (fp->_flags & __SAPP)
        fseek(fp, 0, SEEK_END);

    return fp;
}

FILE *fopen(const char *file, const char *mode)
{
    return _fopen_r(_REENT, file, mode);
}

void _cleanup_r(struct _reent *ptr)
{
    (void)_fwalk(ptr, fflush);
}

void _cleanup(void)
{
    _cleanup_r(_REENT);
}
