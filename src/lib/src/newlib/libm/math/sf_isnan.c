/* newlib libm/math/sf_isnan.c */
#include "../fdlibm.h"

int isnanf(float x)
{
    __int32_t ix;
    GET_FLOAT_WORD(ix, x);
    ix &= 0x7fffffff;
    ix = 0x7f800000 - ix;
    return (int)(((__uint32_t)(ix)) >> 31);
}
