#include "gcGameGlobals.h"

void *gcGameGlobals::GetGroups(int *count) {
    *count = 1;
    return mGroups;
}
