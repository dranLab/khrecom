#include "sdk/libmpeg.h"

/* libmpeg's work area allocator. */
typedef struct {
    unsigned char *start;
    int size;
    unsigned char *cur;
    unsigned char *dynamic;
} _sceMpegAlalc;

void _sceMpegError(sceMpeg *mp, char *message);

void _sceMpegAlalcInit(_sceMpegAlalc *a, unsigned char *start, int size)
{
    a->size = size;
    a->start = start;
    a->cur = start;
    a->dynamic = start;
}

void _sceMpegAlalcSetDynamic(_sceMpegAlalc *a)
{
    a->dynamic = a->cur;
}

void _sceMpegAlalcFree(_sceMpegAlalc *a)
{
    a->cur = a->dynamic;
}

unsigned char *_sceMpegAlalcAlloc(sceMpeg *mp, _sceMpegAlalc *a, int size, unsigned int align)
{
    unsigned char *p = (unsigned char *)(((unsigned int)a->cur + align - 1) / align * align);

    if (a->start + a->size < p + size) {
        _sceMpegError(mp, "work area size is too small");
        return 0;
    }
    a->cur = p + size;
    return p;
}

int _sceMpegAlalcReset(_sceMpegAlalc *a)
{
    return a->start + a->size - a->cur;
}
// SOURCE DATA: .rodata
