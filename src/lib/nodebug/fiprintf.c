/* newlib libc/stdio/fiprintf.c */
#include "sdk/stdarg.h"
extern int vfiprintf(void *, const char *, va_list);

int fiprintf(void *fp, const char *fmt, ...)
{
    int ret;
    va_list ap;

    va_start(ap, fmt);
    ret = vfiprintf(fp, fmt, ap);
    va_end(ap);
    return ret;
}
