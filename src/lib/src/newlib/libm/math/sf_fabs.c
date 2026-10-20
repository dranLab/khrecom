/* newlib libm/math/sf_fabs.c */
#include "../fdlibm.h"

float fabsf(float x)
{
    __uint32_t ix;
    GET_FLOAT_WORD(ix, x);
    SET_FLOAT_WORD(x, ix & 0x7fffffff);
    return x;
}
