#include "gcUIWidgetGroup.h"
#include "gcUIWidget.h"

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" {
    void gcUIWidgetGroup__gcUIWidgetGroup_cBaseptr(void *self, cBase *parent);
}

cBase *gcUIWidgetGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcUIWidgetGroup *result = 0;
    gcUIWidgetGroup *obj = (gcUIWidgetGroup *)rec->fn(base, 0xD8, 4, 0, 0);
    if (obj != 0) {
        gcUIWidgetGroup__gcUIWidgetGroup_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

int gcUIWidgetGroup::NeedsEffectUpdate(void) const {
    int result = 0;
    int groupEffect = ((*(unsigned int *)((char *)this + 0xB0) & 0x20000000) != 0) & 0xFF;
    if (groupEffect ||
        (((*(unsigned int *)((char *)this + 0x24) & 0x08000000) != 0) & 0xFF)) {
        result = 1;
    }
    return result & 0xFF;
}

void *gcUIWidgetGroup::GetResizableSprites(void) {
    int localSprites = ((*(unsigned int *)((char *)this + 0xB0) & 8) != 0) & 0xFF;
    if (localSprites) {
        return (char *)this + 0xC8;
    }
    return ((gcUIWidget *)this)->GetResizableSprites();
}
