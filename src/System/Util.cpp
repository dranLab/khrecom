#include "System/Util.h"
#include "System/MainApp.h"
#include "System/Rnd.h"

UINT crc1(UINT nSize, const BYTE* pSrc) {
    UINT r;
    UINT i;
    UINT j;

    r = 0xFFFFFFFF;
    for (i = 0; i < nSize; i++) {
        r ^= pSrc[i] << 31;
        for (j = 0; j < 1; j++) {
            if (r & 0x80000000) {
                r = (r << 1) ^ 0x04C11DB7;
            } else {
                r <<= 1;
            }
        }
    }
    return ~r;
}

UINT crc2(INT nSize, const BYTE* pSrc) {
    UINT r;
    UINT i;
    UINT j;

    r = 0xFFFFFFFF;
    for (i = 0; i < nSize; i++) {
        r ^= pSrc[i];
        for (j = 0; j < 1; j++) {
            if (r & 1) {
                r = (r >> 1) ^ 0xEDB88320;
            } else {
                r >>= 1;
            }
        }
    }
    return ~r;
}

INT lotProbability(const INT* pRate, INT nNum) {
    INT i;
    INT nTotal;
    INT nRand;

    nTotal = -1;
    for (i = 0; i < nNum; i++) {
#line 80
        KH_DEBUG(if (!(pRate[i] >= 0)) AssertApp(__FILE__, __LINE__, "pRate[i] >= 0", 0);)
        nTotal += pRate[i];
    }
    if (nTotal < 0) {
        return -1;
    }
    nRand = RND::genrand_int31() % (nTotal + 1);
    for (i = 0; i < nNum; i++) {
        if (nRand < pRate[i]) {
            return i;
        }
        nRand -= pRate[i];
    }
#line 95
    AssertApp(__FILE__, __LINE__, "0", 0);
    return -1;
}
// SOURCE DATA: .rodata
