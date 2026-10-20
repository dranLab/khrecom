/* GCC 2.95 libgcc2.c, L_exit (newlib flavour): run the destructors, flush stdio, exit. */
extern void __do_global_dtors(void);
extern void _cleanup(void);
extern void _exit(int status) __attribute__((noreturn));

void exit(int status)
{
    __do_global_dtors();
    _cleanup();
    _exit(status);
}
