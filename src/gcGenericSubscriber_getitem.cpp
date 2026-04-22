#include "gcGenericSubscriber.h"

int gcGenericSubscriber::GetItem(int idx) const {
    int *arr = *(int **)((const char *)this + 0x24);
    int count = 0;
    if (arr) count = arr[-1] & 0x3FFFFFFF;
    if (count <= 0) {
        return idx + 1;
    }
    int result = 0;
    if (idx < 0) return result;
    if (idx >= count) return result;
    result = arr[idx];
    return result;
}
