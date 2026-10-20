/* newlib libc/stdlib/abort.c */
extern void _exit(int) __attribute__((noreturn));

void abort(void)
{
    while (1) {
        _exit(1);
    }
}
