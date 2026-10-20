/* newlib libc/stdio/stdio.c */
#include "stdio_local.h"

#define __SOFF 0x1000
#define SEEK_END 2
typedef long off_t;

extern long _read_r(struct _reent *, int, void *, size_t);
extern long _write_r(struct _reent *, int, const void *, size_t);
extern off_t _lseek_r(struct _reent *, int, off_t, int);
extern int _close_r(struct _reent *, int);

int __sread(void *cookie, char *buf, int n)
{
    register FILE *fp = (FILE *)cookie;
    register int ret;

    ret = _read_r(fp->_data, fp->_file, buf, n);

    /* If the read succeeded, update the current offset.  */
    if (ret >= 0)
        fp->_offset += ret;
    else
        fp->_flags &= ~__SOFF; /* paranoia */
    return ret;
}

int __swrite(void *cookie, char const *buf, int n)
{
    register FILE *fp = (FILE *)cookie;

    if (fp->_flags & __SAPP)
        (void)_lseek_r(fp->_data, fp->_file, (off_t)0, SEEK_END);
    fp->_flags &= ~__SOFF; /* in case O_APPEND mode is set */
    return _write_r(fp->_data, fp->_file, buf, n);
}

long __sseek(void *cookie, long offset, int whence)
{
    register FILE *fp = (FILE *)cookie;
    register off_t ret;

    ret = _lseek_r(fp->_data, fp->_file, (off_t)offset, whence);
    if (ret == -1L)
        fp->_flags &= ~__SOFF;
    else {
        fp->_flags |= __SOFF;
        fp->_offset = ret;
    }
    return ret;
}

int __sclose(void *cookie)
{
    FILE *fp = (FILE *)cookie;

    return _close_r(fp->_data, fp->_file);
}
