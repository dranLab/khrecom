/* newlib's FILE and the parts of struct _reent the nodebug stdio units use */
#ifndef NODEBUG_STDIO_LOCAL_H
#define NODEBUG_STDIO_LOCAL_H

#include "sdk/stdarg.h"

#define NULL 0
#define EOF (-1)
#define INT_MAX 0x7fffffff
typedef unsigned int size_t;

#define __SLBF 0x0001
#define __SNBF 0x0002
#define __SRD 0x0004
#define __SWR 0x0008
#define __SRW 0x0010
#define __SEOF 0x0020
#define __SERR 0x0040
#define __SMBF 0x0080
#define __SAPP 0x0100
#define __SSTR 0x0200
#define __SOPT 0x0400
#define __SNPT 0x0800

struct __sbuf {
    unsigned char *_base;
    int _size;
};

typedef struct __sFILE {
    unsigned char *_p;
    int _r;
    int _w;
    short _flags;
    short _file;
    struct __sbuf _bf;
    int _lbfsize;
    void *_cookie;
    int (*_read)(void *, char *, int);
    int (*_write)(void *, const char *, int);
    long (*_seek)(void *, long, int);
    int (*_close)(void *);
    struct __sbuf _ub;
    unsigned char *_up;
    int _ur;
    unsigned char _ubuf[3];
    unsigned char _nbuf[1];
    struct __sbuf _lb;
    int _blksize;
    int _offset;
    struct _reent *_data;
} FILE;

struct _glue {
    struct _glue *_next;
    int _niobs;
    FILE *_iobs;
};

struct _reent {
    int _errno;
    FILE *_stdin, *_stdout, *_stderr;
    char _pad[0x38 - 0x10];
    int __sdidinit;
    void (*__cleanup)(struct _reent *);
    char _pad2[0x1d8 - 0x40];
    struct _glue __sglue;
    FILE __sf[3];
};

extern struct _reent *_impure_ptr;
#define _REENT _impure_ptr

#endif
