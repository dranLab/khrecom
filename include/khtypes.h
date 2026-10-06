#ifndef KHTYPES_H
#define KHTYPES_H

/* The game's basic types, as recorded in the .mdebug debug info. EE-GCC: int and LONG are 32-bit,
 * long is 64-bit, and TImode gives the EE's 128-bit integers. */

typedef int long128 __attribute__((mode(TI)));
typedef unsigned int u_long128 __attribute__((mode(TI)));
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;

typedef unsigned char BYTE;
typedef char SBYTE;
typedef unsigned char UBYTE;
typedef short SHORT;
typedef unsigned short USHORT;
typedef int LONG;
typedef unsigned int ULONG;
typedef int INT;
typedef unsigned int UINT;
typedef long LONGLONG;
typedef unsigned long ULONGLONG;
typedef float FLOAT;
typedef double DOUBLE;
typedef int BOOL;
typedef char CHAR;
typedef char *LPCSTR;
typedef char *LPCTSTR;
typedef char *LPSTR;
typedef char *LPTSTR;
typedef LONG TIMEQ;
typedef INT FIX4;
typedef INT HANDLE;

typedef char int8;
typedef short int16;
typedef int int32;
typedef long int64;
typedef long128 int128;
typedef u_char u_int8;
typedef u_short u_int16;
typedef u_int u_int32;
typedef u_long u_int64;
typedef u_long128 u_int128;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#endif /* KHTYPES_H */
