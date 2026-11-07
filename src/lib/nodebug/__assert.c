/* newlib libc/stdlib/assert.c */
struct _reent {
    int _errno;
    void *_stdin, *_stdout, *_stderr;
};
extern struct _reent *_impure_ptr;
extern int fiprintf(void *, const char *, ...);
extern void abort(void) __attribute__((noreturn));
/* .rodata stays asm: its split segment starts with 8 bytes of impure.o's .rodata ("C", _REENT_INIT's
   _current_locale, referenced by impure_data; impure_data has no .rodata segment of its own). From source,
   only those 8 bytes differ (checked 2026-11-07). */
extern const char D_004FE5A8[]; /* "assertion \"%s\" failed: file \"%s\", line %d\n" */

void __assert(const char *file, int line, const char *failedexpr)
{
    (void)fiprintf(_impure_ptr->_stderr, D_004FE5A8, failedexpr, file, line);
    abort();
    /* NOTREACHED */
}
