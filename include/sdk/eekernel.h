/* eekernel.h (Sony EE SDK): the declarations the library units use. */
#ifndef SDK_EEKERNEL_H
#define SDK_EEKERNEL_H

typedef struct {
    unsigned int spdifMode : 1;
    unsigned int screenType : 2;
    unsigned int videoOutput : 1;
    unsigned int japLanguage : 1;
    unsigned int ps1drvConfig : 8;
    unsigned int version : 3;
    unsigned int language : 5;
    unsigned int timezoneOffset : 11;
} sceOsdConfigParam;

void FlushCache(int operation);
void SetOsdConfigParam(sceOsdConfigParam *addr);
int GetOsdConfigParam(sceOsdConfigParam *addr);

#endif
