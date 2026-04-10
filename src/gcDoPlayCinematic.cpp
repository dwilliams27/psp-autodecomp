#include "gcDoPlayCinematic.h"
#include "cBase.h"

struct gcDoPlayCinematicData {
    char _pad[0x24];
    gcExpression **mChildren;
};

gcExpression *gcDoPlayCinematic::GetChild(int index) const {
    gcDoPlayCinematicData *self = (gcDoPlayCinematicData *)this;
    return self->mChildren[index];
}
