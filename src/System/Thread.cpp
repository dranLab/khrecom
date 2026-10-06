#include "Thread.h"

extern INT SCAN_RATE_H;
extern INT HLINE_PER_FRAME;

extern "C" int DelayThread(int usec);

namespace Thread {

void delayThread_1Frame() {
    DelayThread(HLINE_PER_FRAME * 1000000 / SCAN_RATE_H);
}

}
