/* newlib libm/common/fdlibm.h: the parts these units use (little-endian EE, 32-bit int, double in a 64-bit GPR). */
#ifndef FDLIBM_H
#define FDLIBM_H

typedef int __int32_t;
typedef unsigned int __uint32_t;

typedef union {
    double value;
    struct {
        __uint32_t lsw;
        __uint32_t msw;
    } parts;
} ieee_double_shape_type;

#define EXTRACT_WORDS(ix0, ix1, d)       \
    do {                                 \
        ieee_double_shape_type ew_u;     \
        ew_u.value = (d);                \
        (ix0) = ew_u.parts.msw;          \
        (ix1) = ew_u.parts.lsw;          \
    } while (0)

#define GET_HIGH_WORD(i, d)              \
    do {                                 \
        ieee_double_shape_type gh_u;     \
        gh_u.value = (d);                \
        (i) = gh_u.parts.msw;            \
    } while (0)

#define GET_LOW_WORD(i, d)               \
    do {                                 \
        ieee_double_shape_type gl_u;     \
        gl_u.value = (d);                \
        (i) = gl_u.parts.lsw;            \
    } while (0)

#define INSERT_WORDS(d, ix0, ix1)        \
    do {                                 \
        ieee_double_shape_type iw_u;     \
        iw_u.parts.msw = (ix0);          \
        iw_u.parts.lsw = (ix1);          \
        (d) = iw_u.value;                \
    } while (0)

#define SET_HIGH_WORD(d, v)              \
    do {                                 \
        ieee_double_shape_type sh_u;     \
        sh_u.value = (d);                \
        sh_u.parts.msw = (v);            \
        (d) = sh_u.value;                \
    } while (0)

#define SET_LOW_WORD(d, v)               \
    do {                                 \
        ieee_double_shape_type sl_u;     \
        sl_u.value = (d);                \
        sl_u.parts.lsw = (v);            \
        (d) = sl_u.value;                \
    } while (0)

typedef union {
    float value;
    __uint32_t word;
} ieee_float_shape_type;

#define GET_FLOAT_WORD(i, d)             \
    do {                                 \
        ieee_float_shape_type gf_u;      \
        gf_u.value = (d);                \
        (i) = gf_u.word;                 \
    } while (0)

#define SET_FLOAT_WORD(d, i)             \
    do {                                 \
        ieee_float_shape_type sf_u;      \
        sf_u.word = (i);                 \
        (d) = sf_u.value;                \
    } while (0)

#define FLT_UWORD_MAX 0x7f7fffffL
#define FLT_UWORD_IS_FINITE(x) ((x) <= FLT_UWORD_MAX)

float __kernel_sinf(float x, float y, int iy);
float __kernel_cosf(float x, float y);
float __kernel_tanf(float x, float y, int iy);
__int32_t __ieee754_rem_pio2f(float x, float *y);

/* math.h / fdlibm.h: the error-handling wrappers. */
struct exception {
    int type;
    char *name;
    double arg1;
    double arg2;
    double retval;
    int err;
};

#define DOMAIN 1
#define SING 2
#define OVERFLOW 3
#define UNDERFLOW 4

typedef enum { __fdlibm_ieee = -1, __fdlibm_svid, __fdlibm_xopen, __fdlibm_posix } __fdlibm_version;
extern __fdlibm_version __fdlib_version;
#define _LIB_VERSION __fdlib_version
#define _IEEE_ __fdlibm_ieee
#define _SVID_ __fdlibm_svid
#define _XOPEN_ __fdlibm_xopen
#define _POSIX_ __fdlibm_posix

extern int *__errno(void);
#define errno (*__errno())
#define EDOM 33
#define ERANGE 34

extern int matherr(struct exception *e);
extern int isnanf(float x) __attribute__((const));
extern int finitef(float x) __attribute__((const));
extern float __ieee754_sqrtf(float x);
extern float __ieee754_acosf(float x);
extern float __ieee754_asinf(float x);
extern float __ieee754_atan2f(float y, float x);
extern float __ieee754_fmodf(float x, float y);
extern float fabsf(float x) __attribute__((const));
extern float atanf(float x);

#endif
