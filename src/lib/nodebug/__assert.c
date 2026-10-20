/* newlib libc/stdlib/assert.c */
struct _reent {
    int _errno;
    void *_stdin, *_stdout, *_stderr;
};
extern struct _reent *_impure_ptr;
extern int fiprintf(void *, const char *, ...);
extern void abort(void) __attribute__((noreturn));
/* .rodata stays asm: it starts with a "C" string of another newlib object */
extern const char D_004FE5A8[]; /* "assertion \"%s\" failed: file \"%s\", line %d\n" */

void __assert(const char *file, int line, const char *failedexpr)
{
    (void)fiprintf(_impure_ptr->_stderr, D_004FE5A8, failedexpr, file, line);
    abort();
    /* NOTREACHED */
}
