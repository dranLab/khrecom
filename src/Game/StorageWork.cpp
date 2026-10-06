#include "Game/StorageWork.h"

extern "C" void* memset(void*, int, unsigned int);

void STORAGE::MCWork::initialize() {
    memset(this, 0, sizeof(*this));
}

void STORAGE::MCWork::preSave() {}

void STORAGE::MCWork::postLoad() {}

void STORAGE::MCWork2::initialize() {
    memset(this, 0, sizeof(*this));
}

void STORAGE::MCWork2::preSave() {}

void STORAGE::MCWork2::postLoad() {}
