// PARTIAL: added for Game/Gimmick/GimmickManager
#ifndef SYSTEM_COM_H
#define SYSTEM_COM_H

#include "khtypes.h"

/* Angle conversions (the original's lines 83/84; their line notes decide the assembler's padding before a
 * following division). */
#line 83
inline FLOAT RAD2DEG(FLOAT r) { return r * 57.295776f; }
inline FLOAT DEG2RAD(FLOAT r) { return r * 0.017453292f; }

#endif /* SYSTEM_COM_H */
