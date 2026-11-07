/* newlib libc/stdlib/assert.c */
struct _reent {
    int _errno;
    void *_stdin, *_stdout, *_stderr;
};
extern struct _reent *_impure_ptr;
extern int fiprintf(void *, const char *, ...);
extern void abort(void) __attribute__((noreturn));

void __assert(const char *file, int line, const char *failedexpr)
{
    (void)fiprintf(_impure_ptr->_stderr, "assertion \"%s\" failed: file \"%s\", line %d\n", failedexpr, file, line);
    abort();
    /* NOTREACHED */
}
// SOURCE DATA: .rodata
