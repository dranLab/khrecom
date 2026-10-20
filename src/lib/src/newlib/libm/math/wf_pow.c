/* newlib libm/math/wf_pow.c */
#include "../fdlibm.h"

union __dmath {
    __uint32_t i[2];
    double d;
};
extern const union __dmath __infinity;
#define HUGE_VAL (__infinity.d)
#define HUGE 3.40282347e+38F

extern double rint(double x);

float powf(float x, float y) /* wrapper powf */
{
	float z;
	struct exception exc;
	z=__ieee754_powf(x,y);
	if(_LIB_VERSION == _IEEE_|| isnanf(y)) return z;
	if(isnanf(x)) {
	    if(y==(float)0.0) {
		/* powf(NaN,0.0) */
		/* error only if _LIB_VERSION == _SVID_ & _XOPEN_ */
		exc.type = DOMAIN;
		exc.name = "powf";
		exc.err = 0;
		exc.arg1 = (double)x;
		exc.arg2 = (double)y;
		exc.retval = x;
		if (_LIB_VERSION == _IEEE_ ||
		    _LIB_VERSION == _POSIX_) exc.retval = 1.0;
		else if (!matherr(&exc)) {
			errno = EDOM;
		}
	        if (exc.err != 0)
	           errno = exc.err;
                return (float)exc.retval;
	    } else
		return z;
	}
	if(x==(float)0.0){
	    if(y==(float)0.0) {
		/* powf(0.0,0.0) */
		/* error only if _LIB_VERSION == _SVID_ */
		exc.type = DOMAIN;
		exc.name = "powf";
		exc.err = 0;
		exc.arg1 = (double)x;
		exc.arg2 = (double)y;
		exc.retval = 0.0;
		if (_LIB_VERSION != _SVID_) exc.retval = 1.0;
		else if (!matherr(&exc)) {
			errno = EDOM;
		}
	        if (exc.err != 0)
	           errno = exc.err;
                return (float)exc.retval;
	    }
            if(finitef(y)&&y<(float)0.0) {
		/* 0**neg */
		exc.type = DOMAIN;
		exc.name = "powf";
		exc.err = 0;
		exc.arg1 = (double)x;
		exc.arg2 = (double)y;
		if (_LIB_VERSION == _SVID_)
		  exc.retval = 0.0;
		else
		  exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
		  errno = EDOM;
		else if (!matherr(&exc)) {
		  errno = EDOM;
		}
	        if (exc.err != 0)
	           errno = exc.err;
                return (float)exc.retval;
            }
	    return z;
	}
	if(!finitef(z)) {
	    if(finitef(x)&&finitef(y)) {
	        if(isnanf(z)) {
		    /* neg**non-int */
		    exc.type = DOMAIN;
		    exc.name = "powf";
		    exc.err = 0;
		    exc.arg1 = (double)x;
		    exc.arg2 = (double)y;
		    if (_LIB_VERSION == _SVID_)
		        exc.retval = 0.0;
		    else
		        exc.retval = 0.0/0.0;	/* X/Open allow NaN */
		    if (_LIB_VERSION == _POSIX_)
		        errno = EDOM;
		    else if (!matherr(&exc)) {
		        errno = EDOM;
		    }
	            if (exc.err != 0)
	                errno = exc.err;
                    return (float)exc.retval;
	        } else {
		    /* powf(x,y) overflow */
		    exc.type = OVERFLOW;
		    exc.name = "powf";
		    exc.err = 0;
		    exc.arg1 = (double)x;
		    exc.arg2 = (double)y;
		    if (_LIB_VERSION == _SVID_) {
		       exc.retval = HUGE;
		       y *= 0.5;
		       if(x<0.0&&rint(y)!=y) exc.retval = -HUGE;
		    } else {
		       exc.retval = HUGE_VAL;
                       y *= 0.5;
		       if(x<0.0&&rint(y)!=y) exc.retval = -HUGE_VAL;
		    }
		    if (_LIB_VERSION == _POSIX_)
		        errno = ERANGE;
		    else if (!matherr(&exc)) {
			errno = ERANGE;
		    }
	            if (exc.err != 0)
	                errno = exc.err;
                    return (float)exc.retval;
	        }
	    }
	}
	if(z==(float)0.0&&finitef(x)&&finitef(y)) {
	    /* powf(x,y) underflow */
	    exc.type = UNDERFLOW;
	    exc.name = "powf";
	    exc.err = 0;
	    exc.arg1 = (double)x;
	    exc.arg2 = (double)y;
	    exc.retval =  0.0;
	    if (_LIB_VERSION == _POSIX_)
	        errno = ERANGE;
	    else if (!matherr(&exc)) {
	        errno = ERANGE;
	    }
	    if (exc.err != 0)
	        errno = exc.err;
            return (float)exc.retval;
	}
	return z;
}
// SOURCE DATA: .rodata
