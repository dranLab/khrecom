/* GCC 2.95 libgcc2.c, L_fixunsdfdi. */
#include "libgcc2.h"

UDItype __fixunsdfdi(DFtype a)
{
    DFtype b;
    UDItype v;

    if (a < 0)
        return 0;

    /* Compute high word of result, as a flonum.  */
    b = (a / HIGH_WORD_COEFF);
    /* Convert that to fixed (but not to DItype!),
       and shift it into the high word.  */
    v = (USItype)b;
    v <<= WORD_SIZE;
    /* Remove high part from the DFtype, leaving the low part as flonum.  */
    a -= (DFtype)v;
    /* Convert that to fixed (but not to DItype!) and add it in.
       Sometimes A comes out negative.  This is significant, since
       A has more bits than a long int does.  */
    if (a < 0)
        v -= (USItype)(-a);
    else
        v += (USItype)a;
    return v;
}
