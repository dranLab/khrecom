/* GCC 2.95 libgcc2.c, L_floatdidf. */
#include "libgcc2.h"

DFtype __floatdidf(DItype u)
{
    DFtype d;

    d = (SItype)(u >> WORD_SIZE);
    d *= HIGH_HALFWORD_COEFF;
    d *= HIGH_HALFWORD_COEFF;
    d += (USItype)(u & (HIGH_WORD_COEFF - 1));

    return d;
}
