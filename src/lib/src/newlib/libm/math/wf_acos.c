/* newlib libm/math/wf_acos.c */
#include "../fdlibm.h"

float acosf(float x) /* wrapper acosf */
{
    float z;
    struct exception exc;
    z = __ieee754_acosf(x);
    if (_LIB_VERSION == _IEEE_ || isnanf(x))
        return z;
    if (fabsf(x) > (float)1.0) {
        /* acosf(|x|>1) */
        exc.type = DOMAIN;
        exc.name = "acosf";
        exc.err = 0;
        exc.arg1 = exc.arg2 = (double)x;
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
