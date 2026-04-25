#include "cFileSystem.h"

class cDynamicMemAllocator {
public:
    void LogAllocations(void) const;
};

void cDynamicMemAllocator::LogAllocations(void) const {
}

int cFileSystem::Create(const char *) {
    return 0;
}
