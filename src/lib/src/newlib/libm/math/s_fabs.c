/* newlib libm/math/s_fabs.c */
#include "../fdlibm.h"

double fabs(double x)
{
    __uint32_t high;
    GET_HIGH_WORD(high, x);
    SET_HIGH_WORD(x, high & 0x7fffffff);
    return x;
}
