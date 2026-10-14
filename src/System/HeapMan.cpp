#include "khtypes.h"
#include "System/HeapMan.h"
#include "System/dlmalloc.h"
#include "System/Debug.h"
#include "System/FileIO.h"

struct SemaParam {
    int currentCount;
    int maxCount;
    int initCount;
    int numWaitThreads;
    unsigned int attr;
    unsigned int option;
};

extern "C" {
int CreateSema(SemaParam* param);
int SignalSema(int sid);
int WaitSema(int sid);
int sprintf(char* buf, const char* fmt, ...);
void* malloc(size_t size);
void free(void* buffer);
}

extern char _end[];

// Kept here: main.cpp and DebugProcInfo.cpp still declare their own CHeapMan.
class CHeapMan {
public:
    static void createHeap(void* base, size_t capacity);
    static void resetHeap();
    static size_t getFreeHeapSize();
    static size_t getUsedHeapSize();
    static float getUsedHeapRatio();
    static size_t getAllHeapSize();
    static size_t getMaxFreeBlockSize();
    static void reportMemory();
    static CHAR* getDebugComment(void* pMem);
};

struct _HEAPINFO_F {
    UINT _nSize;
    char _szFile[28];
};

namespace {
mspace s_msp = NULL;
size_t s_heap_size_all = 0;
size_t s_heap_used = 0;
}

namespace EERAM {
static const INT _HEAP_START = (((INT)_end - 1) / 64 + 1) * 64;
static const INT _HEAP_SIZE = 0x2000000 - 0x40000 - _HEAP_START;
}

namespace {
void* s_base;
int s_nSemaID;
}

extern "C" {

void* com_malloc(size_t bytes) {
    void* p;

    WaitSema(s_nSemaID);
    p = mspace_malloc(s_msp, bytes);
    if (p) {
        s_heap_used += mspace_get_chunk_size_from_mem(p);
    }
    SignalSema(s_nSemaID);
    return p;
}

void* com_realloc(void* mem, size_t newsize) {
    void* p;
    size_t oldsize;

    WaitSema(s_nSemaID);
    oldsize = mem ? mspace_get_chunk_size_from_mem(mem) : 0;
    p = mspace_realloc(s_msp, mem, newsize);
    if (p) {
        s_heap_used = s_heap_used + mspace_get_chunk_size_from_mem(p) - oldsize;
    }
    SignalSema(s_nSemaID);
    return p;
}

void* com_calloc(size_t n_elements, size_t elem_size) {
    void* p;

    WaitSema(s_nSemaID);
    p = mspace_calloc(s_msp, n_elements, elem_size);
    if (p) {
        s_heap_used += mspace_get_chunk_size_from_mem(p);
    }
    SignalSema(s_nSemaID);
    return p;
}

void* com_memalign(size_t alignment, size_t bytes) {
    void* p;

    WaitSema(s_nSemaID);
    p = mspace_memalign(s_msp, alignment, bytes);
    if (p) {
        s_heap_used += mspace_get_chunk_size_from_mem(p);
    }
    SignalSema(s_nSemaID);
    return p;
}

void com_free(void* mem) {
    if (mem) {
        WaitSema(s_nSemaID);
        if (!mspace_check_inuse_mem(s_msp, mem)) {
            TRACE_COL(_TTY_REV_RED, "!!!!!!!!!!!!!!!!!!!!!!!!!!! \203\201\203\202\203\212\223\361\217d\212J\225\372\202\314\211\302\224\134\220\253\202\252\202\240\202\350\202\334\202\267 !!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        }
        size_t size = mspace_get_chunk_size_from_mem(mem);
        s_heap_used -= size;
        mspace_free(s_msp, mem);
        SignalSema(s_nSemaID);
    }
}

void* libc_malloc(size_t size) {
    return malloc(size);
}

void libc_free(void* buffer) {
    free(buffer);
}

void* com_malloc_d(size_t bytes, char* msg, unsigned int ra) {
    void* p;

    WaitSema(s_nSemaID);
    p = mspace_malloc_d(s_msp, bytes, msg, ra);
    if (p) {
        s_heap_used += mspace_get_chunk_size_from_mem(p);
    }
    SignalSema(s_nSemaID);
    if (!p) {
        TRACE_COL(_TTY_RED, "!!!!!!!!!!!!!!!!!!!!!!!!!!!malloc Faild Size:%08xbytes ra:%08x!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", bytes, ra);
    }
    return p;
}

void* com_realloc_d(void* mem, size_t newsize, char* msg, unsigned int ra) {
    void* p;
    size_t oldsize;

    WaitSema(s_nSemaID);
    oldsize = mem ? mspace_get_chunk_size_from_mem(mem) : 0;
    p = mspace_realloc_d(s_msp, mem, newsize, msg, ra);
    if (p) {
        s_heap_used = s_heap_used + mspace_get_chunk_size_from_mem(p) - oldsize;
    }
    SignalSema(s_nSemaID);
    if (!p) {
        TRACE_COL(_TTY_RED, "!!!!!!!!!!!!!!!!!!!!!!!!!!!realloc Faild Size:%08xbytes ra:%08x!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", newsize, ra);
    }
    return p;
}

void* com_calloc_d(size_t n_elements, size_t elem_size, char* msg, unsigned int ra) {
    void* p;

    WaitSema(s_nSemaID);
    p = mspace_calloc_d(s_msp, n_elements, elem_size, msg, ra);
    if (p) {
        s_heap_used += mspace_get_chunk_size_from_mem(p);
    }
    SignalSema(s_nSemaID);
    if (!p) {
        TRACE_COL(_TTY_RED, "!!!!!!!!!!!!!!!!!!!!!!!!!!!calloc Faild Size:%08xbytes ra:%08x!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", n_elements * elem_size, ra);
    }
    return p;
}

void* com_memalign_d(size_t alignment, size_t bytes, char* msg, unsigned int ra) {
    void* p;

    WaitSema(s_nSemaID);
    p = mspace_memalign_d(s_msp, alignment, bytes, msg, ra);
    if (p) {
        s_heap_used += mspace_get_chunk_size_from_mem(p);
    }
    SignalSema(s_nSemaID);
    if (!p) {
        TRACE_COL(_TTY_RED, "!!!!!!!!!!!!!!!!!!!!!!!!!!!memalign Faild Size:%08xbytes ra:%08x!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", bytes, ra);
    }
    return p;
}

static char* s_pszInuse[2] = { "used", "free" };
static char* s_pszSize[2] = { "small ", "normal" };

static void _report_memory_leak(BOOL bLeak) {
    char szComment[64];
    int nASize;
    int nFSize;
    int nCount;
    void* pChunk;
    size_t size;
    char* pszComment;
    UINT nRA;
    TtyColor nCol;

    nCount = 0;
    nASize = 0;
    szComment[63] = 0;
    nFSize = 0;
    pChunk = mspace_get_top_chunk(s_msp);
    while (pChunk) {
        size = mspace_get_chunk_size(pChunk) - 12;
        if (!bLeak || mspace_chunk_inuse(pChunk)) {
            pszComment = mspace_get_chunk_comment(pChunk);
            nRA = mspace_get_chunk_ra(pChunk);
            if ((UINT)pszComment <= 0xFFFFF || (UINT)pszComment >= (UINT)(EERAM::_HEAP_START + EERAM::_HEAP_SIZE)) {
                pszComment = "-";
            }
            if (nCount == 0) {
                if (bLeak) {
                    TRACE("---------------- memory leak report ----------------\n");
                } else {
                    TRACE("---------------- memory report ----------------\n");
                }
                TRACE(" ADDRESS   SIZE    TYPE\n");
            }
            if (mspace_chunk_inuse(pChunk)) {
                nASize += size + 12;
                nCol = _TTY_RED;
                strncpy(szComment, pszComment, 63);
                pszComment = szComment;
            } else {
                nFSize += size + 12;
                nCol = _TTY_BLUE;
                sprintf(szComment, "FREE ");
                strncpy(szComment + 5, pszComment, 58);
                pszComment = szComment;
            }
            nCount++;
            TRACE_COL(nCol, "%8x%7x  %-6s   ->  %s (RA:%8x)\n", mspace_chunk2mem(pChunk), size,
                      s_pszSize[mspace_chunk_is_small(size) ? 0 : 1], pszComment, nRA);
        }
        pChunk = mspace_get_next_chunk(s_msp, pChunk);
    }
    if (nCount) {
        if (!bLeak) {
            TRACE("All:%08x HoleFree:%08x AllFree:%08x Use:%08x(%f%%) Start:%08x End:%08x\n",
                  CHeapMan::getAllHeapSize(), nFSize, CHeapMan::getAllHeapSize() - nASize, nASize,
                  (FLOAT)nASize / CHeapMan::getAllHeapSize() * 100.0f, EERAM::_HEAP_START,
                  EERAM::_HEAP_START + EERAM::_HEAP_SIZE);
            TRACE("-------------- memory report end --------------\n");
        } else {
            TRACE("-------------- memory leak report end --------------\n");
        }
    }
}

void _heap_dump(char* pszFileName, bool bCreate) {
    int ihost;
    char szMes[512];
    void* pChunk;
    size_t size;
    int nFree;
    char* pszComment;

    ihost = CFileIO::openFile(pszFileName, 0x602, 0x1FF);
    if (ihost >= 0) {
        for (pChunk = mspace_get_top_chunk(s_msp); pChunk; pChunk = mspace_get_next_chunk(s_msp, pChunk)) {
            size = mspace_get_chunk_size(pChunk);
            nFree = !mspace_chunk_inuse(pChunk);
            pszComment = nFree ? NULL : mspace_get_chunk_comment(pChunk);
            if (!pszComment) {
                pszComment = "-";
            }
            sprintf(szMes, "0x%08x, %s, %s, 0x%08x, %s\n", mspace_chunk2mem(pChunk), s_pszInuse[nFree],
                    s_pszSize[mspace_chunk_is_small(size) ? 0 : 1], size, pszComment);
            CFileIO::writeFile(ihost, szMes, strlen(szMes));
        }
        sprintf(szMes, "0x%08x, %s, %s, 0x%08x\n", mspace_get_rest_heap(s_msp), "     ", "     ",
                mspace_get_rest_heap_size(s_msp));
        CFileIO::closeFile(ihost);
    }
}

void dump_heap(char* pszFileName, bool bCreate) {
    WaitSema(s_nSemaID);
    _heap_dump(pszFileName, bCreate);
    SignalSema(s_nSemaID);
}

}

void CHeapMan::createHeap(void* base, size_t capacity) {
    SemaParam sparam;

    if (!s_msp) {
        s_base = base;
        s_msp = create_mspace_with_base(base, capacity, 0);
        sparam.initCount = 1;
        sparam.maxCount = 1;
        s_nSemaID = CreateSema(&sparam);
        s_heap_size_all = capacity;
        s_heap_used = capacity - mspace_get_rest_heap_size(s_msp);
    }
}

void CHeapMan::reportMemory() {
    WaitSema(s_nSemaID);
    _report_memory_leak(0);
    SignalSema(s_nSemaID);
}

void CHeapMan::resetHeap() {
    WaitSema(s_nSemaID);
    _report_memory_leak(1);
    s_msp = create_mspace_with_base(s_base, s_heap_size_all, 0);
    s_heap_used = s_heap_size_all - mspace_get_rest_heap_size(s_msp);
    SignalSema(s_nSemaID);
}

size_t CHeapMan::getFreeHeapSize() {
    return s_heap_size_all - s_heap_used;
}

size_t CHeapMan::getUsedHeapSize() {
    return s_heap_used;
}

float CHeapMan::getUsedHeapRatio() {
    return (FLOAT)s_heap_used * 100.0f / s_heap_size_all;
}

size_t CHeapMan::getAllHeapSize() {
    return s_heap_size_all;
}

size_t CHeapMan::getMaxFreeBlockSize() {
    size_t nMax;
    void* pChunk;
    size_t rest;
    size_t size;

    nMax = 0;
    WaitSema(s_nSemaID);
    for (pChunk = mspace_get_top_chunk(s_msp); pChunk; pChunk = mspace_get_next_chunk(s_msp, pChunk)) {
        if (!mspace_chunk_inuse(pChunk)) {
            size = mspace_get_chunk_size(pChunk) - 12;
            if (nMax < size) {
                nMax = size;
            }
        }
    }
    rest = mspace_get_rest_heap_size(s_msp);
    if (nMax < rest) {
        nMax = rest;
    }
    SignalSema(s_nSemaID);
    return nMax;
}

CHAR* CHeapMan::getDebugComment(void* pMem) {
    return mspace_get_chunk_comment(mspace_mem2chunk(pMem));
}

static UINT _analyzeHeapPerFile(_HEAPINFO_F* pOutTbl) {
    char szFile[32];
    UINT nCount;
    void* pChunk;
    size_t size;
    CHAR* pszComment;
    _HEAPINFO_F* pInfo;
    CHAR* p;
    UINT i;

    nCount = 1;
    strcpy(pOutTbl->_szFile, "unknown");
    *(UINT*)pOutTbl = 0; // scalar store: schedules right after the copy, as in the original
    for (pChunk = mspace_get_top_chunk(s_msp); pChunk; pChunk = mspace_get_next_chunk(s_msp, pChunk)) {
        if (!mspace_chunk_inuse(pChunk)) {
            continue;
        }
        pInfo = NULL;
        size = mspace_get_chunk_size(pChunk) - 12;
        pszComment = mspace_get_chunk_comment(pChunk);
        if (!pszComment) {
            pInfo = pOutTbl;
        } else {
            p = strrchr(pszComment, '/');
            if (p) {
                pszComment = p;
            }
            strncpy(szFile, pszComment + 1, 27);
            szFile[27] = 0;
            p = strrchr(szFile, '(');
            if (p) {
                *p = 0;
            }
            for (i = 1; i < nCount; i++) {
                if (strcmp(pOutTbl[i]._szFile, szFile) == 0) {
                    pInfo = &pOutTbl[i];
                    break;
                }
            }
            if (!pInfo) {
                if (nCount >= 1024) {
                    TRACE("_analyzeHeapPerFile() : \202\261\202\352\210\310\217\343\217W\214v\202\305\202\253\202\334\202\271\202\361\n");
                    return nCount;
                }
                pInfo = &pOutTbl[nCount++];
                strcpy(pInfo->_szFile, szFile);
                pInfo->_nSize = 0;
            }
        }
        pInfo->_nSize += size;
    }
    return nCount;
}

static void _sortHeapInfoPerFile(const _HEAPINFO_F* pInfoTbl, USHORT* pIdxTbl, UINT nCount) {
    UINT i;
    UINT nSize;
    UINT j;
    UINT idx;
    UINT k;

    pIdxTbl[0] = 0;
    for (i = 1; i < nCount; i++) {
        nSize = pInfoTbl[i]._nSize;
        for (j = 0; j < i; j++) {
            if (pInfoTbl[pIdxTbl[j]]._nSize < nSize) {
                for (k = i; k >= j + 1; k--) {
                    pIdxTbl[k] = pIdxTbl[k - 1];
                }
                break;
            }
        }
        pIdxTbl[j] = i;
    }
}

void dbgReportHeapPerFile() {
    _HEAPINFO_F infoTbl[1024];
    UINT nFileCount;
    USHORT idxTbl[1024];
    FLOAT fAllSize;
    UINT i;
    UINT idx;
    UINT sum;

    nFileCount = _analyzeHeapPerFile(infoTbl);
    _sortHeapInfoPerFile(infoTbl, idxTbl, nFileCount);
    fAllSize = (FLOAT)s_heap_size_all / 100.0f;
    TRACE("----------------- heap report -----------------\n");
    for (i = 0; i < nFileCount; i++) {
        idx = idxTbl[i];
        sum = infoTbl[idx]._nSize;
        TRACE(" %-27s : %8d (%.2f%%)\n", infoTbl[idx]._szFile, sum, sum / fAllSize);
    }
    TRACE("--------------- heap report end ---------------\n");
}
// SOURCE DATA: .data .rodata .bss
