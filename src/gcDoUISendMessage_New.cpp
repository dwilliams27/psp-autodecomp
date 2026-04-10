#include "gcDoUIFade.h"
#include "cBase.h"

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

void gcAction_gcAction(gcDoUISendMessage *, cBase *);
extern char gcDoUISendMessagevirtualtable[];
void gcDesiredUIWidgetHelper_ctor(void *, int);
void cBaseArray_SetSize(void *, int);
extern char gcDoUISendMessage_typedispatch_base[];
extern char gcDoUISendMessage_typedispatch[];

cBase *gcDoUISendMessage::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUISendMessage *result = 0;
    gcDoUISendMessage *obj = (gcDoUISendMessage *)entry->fn(base, 0x40, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoUISendMessagevirtualtable;
        gcDesiredUIWidgetHelper_ctor((char *)obj + 0x0C, 1);
        ((void **)obj)[0x1C / 4] = gcDoUISendMessage_typedispatch_base;
        ((void **)obj)[0x18 / 4] = (void *)obj;
        ((void **)obj)[0x1C / 4] = gcDoUISendMessage_typedispatch;
        ((char *)obj)[0x20] = 1;
        ((char *)obj)[0x21] = 0;
        ((int *)obj)[0x24 / 4] = 0;
        void *arr = (char *)obj + 0x30;
        int encoded = (int)obj | 1;
        ((int *)obj)[0x28 / 4] = 0;
        ((int *)obj)[0x2C / 4] = encoded;
        ((int *)obj)[0x30 / 4] = 0;
        ((int *)obj)[0x34 / 4] = (int)obj;
        ((int *)obj)[0x38 / 4] = encoded;
        ((char *)obj)[0x3C] = 0;
        cBaseArray_SetSize(arr, 4);
        result = obj;
    }
    return (cBase *)result;
}
