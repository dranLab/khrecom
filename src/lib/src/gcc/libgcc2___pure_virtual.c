/* GCC 2.95 libgcc2.c, L_pure: a pure virtual function was called. */
extern void __terminate(void) __attribute__((noreturn));

void __pure_virtual(void)
{
    __terminate();
}
