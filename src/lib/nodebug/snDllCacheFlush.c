/* libsn: flush the data and instruction caches after loading a DLL */
extern void FlushCache(int);

void snDllCacheFlush(void)
{
    FlushCache(0);
    FlushCache(2);
}
