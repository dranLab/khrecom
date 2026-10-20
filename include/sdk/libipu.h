/* libipu (Sony EE SDK): the declarations the library units use. */
#ifndef SDK_LIBIPU_H
#define SDK_LIBIPU_H

typedef struct {
    unsigned int d4madr;
    unsigned int d4tadr;
    unsigned int d4qwc;
    unsigned int d4chcr;
    unsigned int d3madr;
    unsigned int d3qwc;
    unsigned int d3chcr;
    unsigned int ipubp;
    unsigned int ipuctrl;
} sceIpuDmaEnv;

void sceIpuStopDMA(sceIpuDmaEnv *env);
void sceIpuRestartDMA(sceIpuDmaEnv *env);

#endif
