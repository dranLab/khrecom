#ifndef SYSTEM_UTIL_H
#define SYSTEM_UTIL_H

#include "khtypes.h"

UINT crc1(UINT nSize, const BYTE* pSrc);
UINT crc2(INT nSize, const BYTE* pSrc);
INT lotProbability(const INT* pRate, INT nNum);

#endif /* SYSTEM_UTIL_H */
