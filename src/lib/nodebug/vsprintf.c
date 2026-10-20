/* newlib libc/stdio/vsprintf.c (+ SCE's vsnprintf) */
#include "stdio_local.h"

extern int vfprintf(FILE *, const char *, va_list);

int vsprintf(char *str, const char *fmt, va_list ap)
{
    int ret;
    FILE f;

    f._flags = __SWR | __SSTR;
    f._bf._base = f._p = (unsigned char *)str;
    f._bf._size = f._w = INT_MAX;
    f._data = _REENT;
    ret = vfprintf(&f, fmt, ap);
    *f._p = 0;
    return ret;
}

int vsnprintf(char *str, size_t size, const char *fmt, va_list ap)
{
    int ret;
    FILE f;

    f._flags = __SWR | __SSTR;
    f._bf._base = f._p = (unsigned char *)str;
    f._bf._size = f._w = size;
    f._data = _REENT;
    ret = vfprintf(&f, fmt, ap);
    if (size < ret)
        ret = -1;
    *f._p = 0;
    return ret;
}
