/* newlib libm/math/wf_sqrt.c */
#include "../fdlibm.h"

float sqrtf(float x) /* wrapper sqrtf */
{
    float z;
    struct exception exc;
    z = __ieee754_sqrtf(x);
    if (_LIB_VERSION == _IEEE_ || isnanf(x))
        return z;
    if (x < (float)0.0) {
        /* sqrtf(x<0) */
        exc.type = DOMAIN;
        exc.name = "sqrtf";
        exc.err = 0;
        exc.arg1 = exc.arg2 = (double)x;
        if (_LIB_VERSION == _SVID_)
            exc.retval = 0.0;
        else
            exc.retval = 0.0 / 0.0;
        if (_LIB_VERSION == _POSIX_)
            errno = EDOM;
        else if (!matherr(&exc)) {
            errno = EDOM;
        }
        if (exc.err != 0)
            errno = exc.err;
        return (float)exc.retval;
    } else
        return z;
}
// SOURCE DATA: .rodata
// SOURCE DATA SLUS_217.99_retail: .rodata
