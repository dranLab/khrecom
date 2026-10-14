#ifndef SYSTEM_MCD_H
#define SYSTEM_MCD_H

#include "khtypes.h"

typedef struct {
    u_int lsn;
    u_int size;
    char name[16];
    u_char date[8];
    u_int flag;
} sceCdlFILE;

typedef struct {
    u_char stat;
    u_char second;
    u_char minute;
    u_char hour;
    u_char pad;
    u_char day;
    u_char month;
    u_char year;
} sceCdCLOCK;

extern "C" {
int mcdInit();
int mcdInitEeCB(int cb_prio, void* stack_addr, int stack_size);
int mcdCallback(void (*func)(int, void*), void* pContext);
int mcdSearchFile(sceCdlFILE* fp, char* name);
int mcdRead(u_int lsn, u_int sectors, void* buf);
int mcdRead2(u_int lsn, u_int sectors, void* buf);
int mcdAsyncRead(u_int lsn, u_int sectors, void* buf);
int mcdAsyncRead2(u_int lsn, u_int sectors, void* buf);
int mcdSync(int mode);
int mcdGetLastResult();
int mcdBeginStream();
int mcdEndStream();
int mcdReadClock(sceCdCLOCK* rtc);
int mcdChangeMedia(int media);
int mcdSetCheckFile(char* file);
int mcdCheckCDTray();
int mcdCheckStat();
}

#endif // SYSTEM_MCD_H
