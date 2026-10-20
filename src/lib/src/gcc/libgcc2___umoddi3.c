/* GCC 2.95 libgcc2.c, L_umoddi3. */
#define L_umoddi3
/* The last object of .rodata: its table and the padding up to .sdata stay assembly (the padding is the
   original link's section alignment, not this unit's data). */
#define CLZ_TAB_EXTERN __clz_tab_004FF558
#include "libgcc2_div.h"

UDItype __umoddi3(UDItype u, UDItype v)
{
    UDItype w;

    (void)__udivmoddi4(u, v, &w);

    return w;
}
