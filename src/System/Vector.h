// PARTIAL: FVECTOR/FMATRIX layouts shared by all units; the inline operators are added as units need them.
// The original header's inline functions leave "System/Vector.h(252)" in only a few objects' .rodata, so it
// must not get string literals (asserts) until a unit that has them reproduces them.
#ifndef SYSTEM_VECTOR_H
#define SYSTEM_VECTOR_H

#include "khtypes.h"

typedef float _FVECTOR[4];
union FVECTOR {
    _FVECTOR v;
    struct {
        FLOAT x;
        FLOAT y;
        FLOAT z;
        FLOAT w;
    };
    u_long128 _q;
    unsigned long _ul[2];
    unsigned int _ui[4];
};

typedef float _FMATRIX[4][4];
union FMATRIX {
    _FMATRIX m;
    FVECTOR v[4];
    void setTrans(const FVECTOR& vecIn) { v[3] = vecIn; }
};

#endif /* SYSTEM_VECTOR_H */
