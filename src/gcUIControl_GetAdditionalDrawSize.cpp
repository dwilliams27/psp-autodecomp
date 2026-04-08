#include "gcUIControl.h"
#include "mVec2.h"

void gcUIControl::GetAdditionalDrawSize(mVec2 *size, unsigned int) const {
    size->x = 0.0f;
    size->y = 0.0f;
}
