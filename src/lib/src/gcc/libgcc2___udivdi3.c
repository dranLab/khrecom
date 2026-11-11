/* GCC 2.95 libgcc2.c, L_udivdi3. */
#define L_udivdi3
#include "libgcc2_div.h"

UDItype __udivdi3(UDItype n, UDItype d)
{
    return __udivmoddi4(n, d, (UDItype *)0);
}
// SOURCE DATA: .rodata
// SOURCE DATA SLUS_217.99_retail: .rodata
