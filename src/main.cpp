#include "khtypes.h"
#include "System/Debug.h"
#include "include_asm.h"

// The static initializer of EERAM::_HEAP_START/_HEAP_SIZE (below, in #if 0) can't be built yet: the link has
// no `_end` symbol, and ee_asm_compat does not rename `_GLOBAL_$I$src_main.cpp...` to the original
// `_GLOBAL_$I$c__KHCOM_MainApp_main.cpp00aaaa`. Until then the variables are the original .bss labels.
namespace EERAM {
extern INT _HEAP_START asm("D_0068C470");
extern INT _HEAP_SIZE asm("D_0068C474");
}

class CHeapMan {
public:
    static void createHeap(void* base, unsigned int size);
};

class CMainApp {
public:
    static void initialize(int mode);
    static INT run();
    static void finalize();
};

int main(int argc, char** argv) {
    int result;

    CHeapMan::createHeap((void*)EERAM::_HEAP_START, EERAM::_HEAP_SIZE);
    TRACE("---------------------- main start -----------------------\n");
    TRACE(" [Heap Size] = %d bytes\n\n", EERAM::_HEAP_SIZE);
    CMainApp::initialize(1);
    result = CMainApp::run();
    CMainApp::finalize();
    return result;
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/main", __static_initialization_and_destruction_0_0035CBF8);
#if 0
// Matches except for the relocation (`_end` vs the original's nearest symbol); needs `_end` in the link and
// the global ctor name mapped (see above).
extern char _end[];

namespace EERAM {
static INT _HEAP_START = (((INT)_end - 1) / 64 + 1) * 64;
static INT _HEAP_SIZE = 0x2000000 - 0x40000 - _HEAP_START;
}
#endif

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/main", _GLOBAL_$I$c__KHCOM_MainApp_main.cpp00aaaa);

// SOURCE DATA: .rodata
