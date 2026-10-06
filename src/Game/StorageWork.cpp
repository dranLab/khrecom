#include <string.h>

#include "Game/StorageWork.h"

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
