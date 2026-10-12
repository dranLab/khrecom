#include "khtypes.h"
#include "System/Debug.h"
#include "System/MainApp.h"

extern char _end[];

namespace EERAM {
static INT _HEAP_START = (((INT)_end - 1) / 64 + 1) * 64;
static INT _HEAP_SIZE = 0x2000000 - 0x40000 - _HEAP_START;
}

class CHeapMan {
public:
    static void createHeap(void* base, unsigned int size);
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

// SOURCE DATA: .rodata .bss
