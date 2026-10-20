/* newlib libm/math/sf_tan.c */
#include "../fdlibm.h"

float tanf(float x)
{
    float y[2], z = 0.0;
    __int32_t n, ix;

    GET_FLOAT_WORD(ix, x);

    /* |x| ~< pi/4 */
    ix &= 0x7fffffff;
    if (ix <= 0x3f490fda)
        return __kernel_tanf(x, z, 1);

    /* tan(Inf or NaN) is NaN */
    else if (!FLT_UWORD_IS_FINITE(ix))
        return x - x; /* NaN */

    /* argument reduction needed */
    else {
        n = __ieee754_rem_pio2f(x, y);
        return __kernel_tanf(y[0], y[1], 1 - ((n & 1) << 1)); /*   1 -- n even
                                                                  -1 -- n odd */
    }
}
