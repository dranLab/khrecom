/* newlib libc/stdlib/mlock.c, SCE version: a recursive lock on the libc semaphore */
struct _reent;
extern int GetThreadId(void);
extern int WaitSema(int);
extern int SignalSema(int);
extern int __sce_sema_id;
extern int __malloc_lock_owner;
extern int call_count;

void __malloc_lock(struct _reent *ptr)
{
    int id = GetThreadId();

    if (__malloc_lock_owner == id) {
        call_count++;
        return;
    }
    WaitSema(__sce_sema_id);
    __malloc_lock_owner = id;
    call_count++;
}

void __malloc_unlock(struct _reent *ptr)
{
    if (--call_count == 0) {
        __malloc_lock_owner = -1;
        SignalSema(__sce_sema_id);
    }
}
