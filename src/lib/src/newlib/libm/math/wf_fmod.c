/* newlib libm/math/wf_fmod.c */
#include "../fdlibm.h"

float fmodf(float x, float y) /* wrapper fmodf */
{
    float z;
    struct exception exc;
    z = __ieee754_fmodf(x, y);
    if (_LIB_VERSION == _IEEE_ || isnanf(y) || isnanf(x))
        return z;
    if (y == (float)0.0) {
        /* fmodf(x,0) */
        exc.type = DOMAIN;
        exc.name = "fmodf";
        exc.err = 0;
        exc.arg1 = (double)x;
        exc.arg2 = (double)y;
        if (_LIB_VERSION == _SVID_)
            exc.retval = x;
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
