#include "System/Rnd.h"

#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfU
#define UPPER_MASK 0x80000000U
#define LOWER_MASK 0x7fffffffU

namespace RND {
static int mti = N + 1;
static unsigned int mt[N];
}

// VU0 random generator: the seed lives in VU0 integer memory (vi1 = 3)
#define RND_START \
    asm volatile("viaddi $vi1, $vi0, 0x3\n" \
                 "vilwr.x $vi2, ($vi1)\n")

#define RND_GET(f) \
    "mult $2, $2, $3\n" \
    "addiu $2, $2, 0x3039\n" \
    "ctc2 $2, $vi20\n" \
    "vrget." #f " $vf2, $R\n"

#define RND_HEAD \
    "lui $3, 0x41c6\n" \
    "ori $3, $3, 0x4e6d\n" \
    "cfc2 $2, $vi2\n"

#define RND_XYZ asm volatile(RND_HEAD RND_GET(x) RND_GET(y) RND_GET(z) "ctc2 $2, $vi2\n" : : : "$2", "$3")

#define RND_XYZW \
    asm volatile(RND_HEAD RND_GET(x) RND_GET(y) RND_GET(z) RND_GET(w) "ctc2 $2, $vi2\n" : : : "$2", "$3")

void RND::initRnd(UINT nInSeed) {
    asm volatile("qmtc2 %0, $vf1\n"
                 "viaddi $vi1, $vi0, 0x3\n"
                 "vsqi.x $vf1, ($vi1++)\n"
                 :
                 : "r"(nInSeed));
}

void RND::getRndVecXYZ(FVECTOR& vOut, const FVECTOR& vInRndMax) {
    asm volatile("lqc2 $vf2, 0x0(%0)\n" : : "r"(&vOut));
    RND_START;
    RND_XYZ;
    asm volatile("lqc2 $vf3, 0x0(%1)\n"
                 "vmula.xyz $ACC, $vf2, $vf3\n"
                 "vmsubw.xyz $vf2, $vf3, $vf0w\n"
                 "viswr.x $vi2, ($vi1)\n"
                 "sqc2 $vf2, 0x0(%0)\n"
                 :
                 : "r"(&vOut), "r"(&vInRndMax)
                 : "memory");
}

void RND::getRndVec(FVECTOR& vOut, const FVECTOR& vInRndMax) {
    RND_START;
    RND_XYZW;
    asm volatile("lqc2 $vf3, 0x0(%1)\n"
                 "vmula.xyzw $ACC, $vf2, $vf3\n"
                 "vmsubw.xyzw $vf2, $vf3, $vf0w\n"
                 "viswr.x $vi2, ($vi1)\n"
                 "sqc2 $vf2, 0x0(%0)\n"
                 :
                 : "r"(&vOut), "r"(&vInRndMax)
                 : "memory");
}

void RND::addRndVecXYZ(FVECTOR& vOut, const FVECTOR& vInSrc, const FVECTOR& vInRndMax) {
    asm volatile("lqc2 $vf2, 0x0(%0)\n" : : "r"(&vInSrc));
    RND_START;
    RND_XYZ;
    asm volatile("lqc2 $vf4, 0x0(%2)\n"
                 "lqc2 $vf3, 0x0(%1)\n"
                 "vmula.xyz $ACC, $vf2, $vf4\n"
                 "vmaddaw.xyz $ACC, $vf3, $vf0w\n"
                 "vmsubw.xyz $vf2, $vf4, $vf0w\n"
                 "viswr.x $vi2, ($vi1)\n"
                 "sqc2 $vf2, 0x0(%0)\n"
                 :
                 : "r"(&vOut), "r"(&vInSrc), "r"(&vInRndMax)
                 : "memory");
}

void RND::addRndVec(FVECTOR& vOut, const FVECTOR& vInSrc, const FVECTOR& vInRndMax) {
    RND_START;
    RND_XYZ;
    asm volatile("lqc2 $vf4, 0x0(%2)\n"
                 "lqc2 $vf3, 0x0(%1)\n"
                 "vmula.xyzw $ACC, $vf2, $vf4\n"
                 "vmaddaw.xyzw $ACC, $vf3, $vf0w\n"
                 "vmsubw.xyzw $vf2, $vf4, $vf0w\n"
                 "viswr.x $vi2, ($vi1)\n"
                 "sqc2 $vf2, 0x0(%0)\n"
                 :
                 : "r"(&vOut), "r"(&vInSrc), "r"(&vInRndMax)
                 : "memory");
}

void RND::addRndVecXYZ_Center(FVECTOR& vOut, const FVECTOR& vInSrc, const FVECTOR& vInRndMax) {
    FLOAT f;
    asm volatile("lqc2 $vf2, 0x0(%0)\n" : : "r"(&vInSrc));
    RND_START;
    RND_XYZ;
    f = 0.5f;
    asm volatile("lqc2 $vf4, 0x0(%2)\n"
                 "lqc2 $vf3, 0x0(%1)\n"
                 "vmula.xyz $ACC, $vf2, $vf4\n"
                 "vmaddaw.xyz $ACC, $vf3, $vf0w\n"
                 "vmsubaw.xyz $ACC, $vf4, $vf0w\n"
                 "qmtc2 %3, $vf3\n"
                 "vmsubx.xyz $vf2, $vf4, $vf3x\n"
                 "viswr.x $vi2, ($vi1)\n"
                 "sqc2 $vf2, 0x0(%0)\n"
                 :
                 : "r"(&vOut), "r"(&vInSrc), "r"(&vInRndMax), "r"(f)
                 : "memory");
}

void RND::addRndVecXYZ_Sphere(FVECTOR& vOut, const FVECTOR& vInSrc, const FVECTOR& vInRndMax) {
    FLOAT f;
    RND_START;
    RND_XYZW;
    f = 0.5f;
    asm volatile("vsubw.xyzw $vf1, $vf2, $vf0w\n"
                 "qmtc2 %3, $vf3\n"
                 "vsubx.xyz $vf1, $vf1, $vf3x\n"
                 "vmulx.w $vf1, $vf1, $vf3x\n"
                 "vmul.xyz $vf2, $vf1, $vf1\n"
                 "vaddw.x $vf3, $vf0, $vf0w\n"
                 "vadday.x $ACC, $vf2, $vf2y\n"
                 "vmaddz.x $vf2, $vf3, $vf2z\n"
                 "vrsqrt $Q, $vf0w, $vf2x\n"
                 "vwaitq\n"
                 "vmulq.xyz $vf1, $vf1, $Q\n"
                 "lqc2 $vf4, 0x0(%2)\n"
                 "lqc2 $vf2, 0x0(%1)\n"
                 "vmulw.xyz $vf1, $vf1, $vf1w\n"
                 "vadda.xyz $ACC, $vf2, $vf0\n"
                 "vmadd.xyz $vf2, $vf1, $vf4\n"
                 "viswr.x $vi2, ($vi1)\n"
                 "sqc2 $vf2, 0x0(%0)\n"
                 :
                 : "r"(&vOut), "r"(&vInSrc), "r"(&vInRndMax), "r"(f)
                 : "memory");
}

void RND::init_genrand(unsigned int s) {
    mt[0] = s;
    for (mti = 1; mti < N; mti++) {
        mt[mti] = (1812433253UL * (mt[mti - 1] ^ (mt[mti - 1] >> 30)) + mti);
    }
}

void RND::init_by_array(unsigned int* init_key, int key_length) {
    int i;
    int j;
    int k;
    init_genrand(19650218U);
    i = 1;
    j = 0;
    k = (N > key_length ? N : key_length);
    for (; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1664525UL)) + init_key[j] + j;
        i++;
        j++;
        if (i >= N) {
            mt[0] = mt[N - 1];
            i = 1;
        }
        if (j >= key_length) j = 0;
    }
    for (k = N - 1; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1566083941UL)) - i;
        i++;
        if (i >= N) {
            mt[0] = mt[N - 1];
            i = 1;
        }
    }
    mt[0] = 0x80000000U;
}

unsigned int RND::genrand_int32() {
    unsigned int y;
    static unsigned int mag01[2] = {0x0U, MATRIX_A};

    if (mti >= N) {
        int kk;
        if (mti == N + 1) init_genrand(5489U);
        for (kk = 0; kk < N - M; kk++) {
            y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
            mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1U];
        }
        for (; kk < N - 1; kk++) {
            y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
            mt[kk] = mt[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1U];
        }
        y = (mt[N - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
        mt[N - 1] = mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1U];
        mti = 0;
    }
    y = mt[mti++];
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680U;
    y ^= (y << 15) & 0xefc60000U;
    y ^= (y >> 18);
    return y;
}

int RND::genrand_int31() {
    return (int)(genrand_int32() >> 1);
}

float RND::genrand_real1() {
    return genrand_int32() * (1.0f / 4294967295.0f);
}

float RND::genrand_real2() {
    return genrand_int32() * (1.0f / 4294967296.0f);
}

// SOURCE DATA: .data .bss
