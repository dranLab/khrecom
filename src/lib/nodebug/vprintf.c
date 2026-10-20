/* newlib libc/stdio/vprintf.c */
#include "sdk/stdarg.h"
struct _reent {
    int _errno;
    void *_stdin, *_stdout, *_stderr;
};
extern struct _reent *_impure_ptr;
extern int vfprintf(void *, const char *, va_list);

int vprintf(const char *fmt, va_list ap)
{
    return vfprintf(_impure_ptr->_stdout, fmt, ap);
}
