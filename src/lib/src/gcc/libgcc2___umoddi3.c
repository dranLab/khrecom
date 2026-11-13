/* GCC 2.95 libgcc2.c, L_umoddi3. */
#define L_umoddi3
#include "libgcc2_div.h"

UDItype __umoddi3(UDItype u, UDItype v)
{
    UDItype w;

    (void)__udivmoddi4(u, v, &w);

    return w;
}
// SOURCE DATA: .rodata
// SOURCE DATA SLUS_217.99_retail: .rodata
