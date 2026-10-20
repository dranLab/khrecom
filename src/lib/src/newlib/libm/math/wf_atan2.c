/* newlib libm/math/wf_atan2.c */
#include "../fdlibm.h"

float atan2f(float y, float x) /* wrapper atan2f */
{
    float z;
    struct exception exc;
    z = __ieee754_atan2f(y, x);
    if (_LIB_VERSION == _IEEE_ || isnanf(x) || isnanf(y))
        return z;
    if (x == (float)0.0 && y == (float)0.0) {
        /* atan2f(+-0,+-0) */
        exc.arg1 = y;
        exc.arg2 = x;
        exc.err = 0;
        exc.type = DOMAIN;
        exc.name = "atan2f";
        exc.retval = 0.0;
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
