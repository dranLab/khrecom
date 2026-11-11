/* Timer alarms: one-shot handlers on top of the timer counters (timer.c), from a pool of 64 entries.
 * An alarm id is (entry address << 4) | (counter id & 0xFE) | 1. */
#include "sdk/eekernel.h"

typedef unsigned long (*TimerAlarmHandler)(int id, unsigned long schedule, unsigned long actual, void *arg);

int AllocTimerCounter(void);
int iAllocTimerCounter(void);
int FreeTimerCounter(int id);
int iFreeTimerCounter(int id);
int SetTimerHandler(int id, unsigned long schedule, void *handler, void *arg);
int iSetTimerHandler(int id, unsigned long schedule, void *handler, void *arg);
int StartTimerCounter(int id);
int iStartTimerCounter(int id);

#define KE_ILLEGAL_HANDLER 0x80000016
#define KE_NO_ALARM 0x80008005
#define KE_ILLEGAL_ALARMID 0x80008002

typedef struct Alarm {
    struct Alarm *next;
    volatile int id;  /* volatile: the original reads it again after each check */
    TimerAlarmHandler handler;
    void *arg;
} Alarm;

static Alarm g_AlarmBuf[64];
static Alarm *g_pFreeAlarm;

static inline Alarm *AllocAlarm(void)
{
    Alarm *p = g_pFreeAlarm;

    if (p) {
        g_pFreeAlarm = p->next;
    }
    return p;
}

static inline void FreeAlarm(Alarm *p)
{
    p->next = g_pFreeAlarm;
    g_pFreeAlarm = p;
    p->id = 0;
}

int _InitAlarm(void)
{
    int i;

    g_pFreeAlarm = g_AlarmBuf;
    for (i = 63; i >= 0; i--) {
        g_AlarmBuf[i].next = &g_AlarmBuf[i + 1];
    }
    g_AlarmBuf[63].next = 0;
    return 0;
}

static unsigned long AlarmHandler(int id, unsigned long schedule, unsigned long actual, Alarm *p)
{
    unsigned long ret;

    ret = p->handler(((int)p << 4) | (id & 0xFE) | 1, schedule, actual, p->arg);
    if (ret == 0) {
        FreeAlarm(p);
        return -1;
    }
    return ret;
}

int SetTimerAlarm(unsigned long clock, TimerAlarmHandler handler, void *arg)
{
    Alarm *p;
    int eie, id;

    if (!handler) {
        return KE_ILLEGAL_HANDLER;
    }
    eie = DIntr();
    p = AllocAlarm();
    if (!p) {
        if (eie) {
            EIntr();
        }
        return KE_NO_ALARM;
    }
    id = AllocTimerCounter();
    if (id < 0) {
        FreeAlarm(p);
        if (eie) {
            EIntr();
        }
        return id;
    }
    p->handler = handler;
    p->arg = arg;
    p->id = id;
    SetTimerHandler(id, clock, AlarmHandler, p);
    StartTimerCounter(id);
    if (eie) {
        EIntr();
    }
    return ((int)p << 4) | (id & 0xFE) | 1;
}

int iSetTimerAlarm(unsigned long clock, TimerAlarmHandler handler, void *arg)
{
    Alarm *p;
    int id;

    if (!handler) {
        return KE_ILLEGAL_HANDLER;
    }
    p = AllocAlarm();
    if (!p) {
        return KE_NO_ALARM;
    }
    id = iAllocTimerCounter();
    if (id < 0) {
        FreeAlarm(p);
        return id;
    }
    p->handler = handler;
    p->arg = arg;
    p->id = id;
    iSetTimerHandler(id, clock, AlarmHandler, p);
    iStartTimerCounter(id);
    return ((int)p << 4) | (id & 0xFE) | 1;
}

int ReleaseTimerAlarm(int id)
{
    Alarm *p = (Alarm *)(((unsigned int)id >> 8) << 4);
    int eie;

    eie = DIntr();
    if (id < 0 || ((id ^ p->id) & 0xFF)) {
        if (eie) {
            EIntr();
        }
        return KE_ILLEGAL_ALARMID;
    }
    FreeTimerCounter(p->id);
    FreeAlarm(p);
    if (eie) {
        EIntr();
    }
    return 0;
}

int iReleaseTimerAlarm(int id)
{
    Alarm *p = (Alarm *)(((unsigned int)id >> 8) << 4);
    int ret;

    if (id < 0 || ((id ^ p->id) & 0xFF)) {
        return KE_ILLEGAL_ALARMID;
    }
    ret = iFreeTimerCounter(p->id);
    if (ret == 0) {
        FreeAlarm(p);
    }
    return ret;
}
// SOURCE DATA: .bss
// SOURCE DATA SLUS_217.99_retail: .bss
