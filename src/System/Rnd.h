#ifndef SYSTEM_RND_H
#define SYSTEM_RND_H

#include "khtypes.h"

union FVECTOR;

namespace RND {
void initRnd(UINT nInSeed);
void getRndVecXYZ(FVECTOR& vOut, const FVECTOR& vInRndMax);
void getRndVec(FVECTOR& vOut, const FVECTOR& vInRndMax);
void addRndVecXYZ(FVECTOR& vOut, const FVECTOR& vInSrc, const FVECTOR& vInRndMax);
void addRndVec(FVECTOR& vOut, const FVECTOR& vInSrc, const FVECTOR& vInRndMax);
void addRndVecXYZ_Center(FVECTOR& vOut, const FVECTOR& vInSrc, const FVECTOR& vInRndMax);
void addRndVecXYZ_Sphere(FVECTOR& vOut, const FVECTOR& vInSrc, const FVECTOR& vInRndMax);
void init_genrand(unsigned int s);
void init_by_array(unsigned int* init_key, int key_length);
unsigned int genrand_int32();
int genrand_int31();
float genrand_real1();
float genrand_real2();
}

#endif /* SYSTEM_RND_H */
