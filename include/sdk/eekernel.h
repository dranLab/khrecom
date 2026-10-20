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

struct SemaParam {
    int currentCount;
    int maxCount;
    int initCount;
    int numWaitThreads;
    unsigned int attr;
    unsigned int option;
};

struct ThreadParam {
    int status;
    void (*entry)(void *);
    void *stack;
    int stackSize;
    void *gpReg;
    int initPriority;
    int currentPriority;
    unsigned int attr;
    unsigned int option;
    int waitType;
    int waitId;
    int wakeupCount;
};

#define THS_DORMANT 0x10

int CreateSema(struct SemaParam *sema);
int DeleteSema(int sema);
int WaitSema(int sema);
int iSignalSema(int sema);
int CreateThread(struct ThreadParam *param);
int ReferThreadStatus(int tid, struct ThreadParam *info);
int GetThreadId(void);
int ChangeThreadPriority(int tid, int priority);
int QueryIntrContext(void);

void FlushCache(int operation);
void SetOsdConfigParam(sceOsdConfigParam *addr);
int GetOsdConfigParam(sceOsdConfigParam *addr);
int DIntr(void);
int EIntr(void);

#endif
