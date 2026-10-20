/* newlib libm/common/sf_finite.c */
#include "../fdlibm.h"

int finitef(float x)
{
    __int32_t ix;
    GET_FLOAT_WORD(ix, x);
    ix &= 0x7fffffff;
    return (int)((__uint32_t)(ix - 0x7f800000) >> 31);
}
