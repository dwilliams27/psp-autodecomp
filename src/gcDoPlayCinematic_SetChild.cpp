#include "gcDoPlayCinematic.h"
#include "cBase.h"

struct gcDoPlayCinematicData {
    char _pad[0x24];
    gcExpression **mChildren;
};

void gcDoPlayCinematic::SetChild(int index, gcExpression *child) {
    gcDoPlayCinematicData *self = (gcDoPlayCinematicData *)this;
    self->mChildren[index] = child;
}
