/* newlib libc/stdio/printf.c */
#include "sdk/stdarg.h"

typedef struct __sFILE {
    char _pad[0x54];
    struct _reent *_data;
} FILE;

struct _reent {
    int _errno;
    FILE *_stdin, *_stdout, *_stderr;
};

extern struct _reent *_impure_ptr;
#define _REENT _impure_ptr
#define _stdout_r(x) ((x)->_stdout)
extern int _vfprintf_r(struct _reent *, FILE *, const char *, va_list);
extern int vfprintf(FILE *, const char *, va_list);

int _printf_r(struct _reent *ptr, const char *fmt, ...)
{
    int ret;
    va_list ap;

    va_start(ap, fmt);
    ret = _vfprintf_r(ptr, _stdout_r(ptr), fmt, ap);
    va_end(ap);
    return ret;
}

int printf(const char *fmt, ...)
{
    int ret;
    va_list ap;

    va_start(ap, fmt);
    /* not a struct-field store: gcc then reloads _impure_ptr after it, like the original */
    *(struct _reent **)&_stdout_r(_REENT)->_data = _REENT;
    ret = vfprintf(_stdout_r(_REENT), fmt, ap);
    va_end(ap);
    return ret;
}
