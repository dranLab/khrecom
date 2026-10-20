/* libmpeg (Sony EE SDK): the declarations the library units use. */
#ifndef SDK_LIBMPEG_H
#define SDK_LIBMPEG_H

typedef struct {
    int width;
    int height;
    int frameCount;
    long pts;
    long dts;
    unsigned long flags;
    long pts2nd;
    long dts2nd;
    unsigned long flags2nd;
    void *sys;
} sceMpeg;

#endif
