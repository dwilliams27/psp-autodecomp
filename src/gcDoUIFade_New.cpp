#include "gcDoUIFade.h"
#include "cBase.h"

void gcAction_gcAction(gcDoUIFade *, cBase *);
extern char gcDoUIFadevirtualtable[];
void gcDesiredCamera_gcDesiredCamera(void *, cBase *);
void gcEvent_gcEvent(void *, cBase *, const char *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

void cBaseArray_SetSize(void *, int);

cBase *gcDoUIFade::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUIFade *result = 0;
    gcDoUIFade *obj = (gcDoUIFade *)entry->fn(base, 0x80, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoUIFadevirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = 3;
        gcDesiredCamera_gcDesiredCamera((char *)obj + 0x14, (cBase *)obj);
        int encoded = (int)obj | 1;
        ((int *)obj)[0x4C / 4] = 0;
        ((int *)obj)[0x50 / 4] = encoded;
        ((int *)obj)[0x54 / 4] = encoded;
        gcEvent_gcEvent((char *)obj + 0x58, (cBase *)obj, (const char *)0x36DB58);
        ((int *)obj)[0x74 / 4] = encoded;
        ((int *)obj)[0x78 / 4] = 0;
        ((int *)obj)[0x7C / 4] = (int)obj;
        cBaseArray_SetSize((char *)obj + 0x78, 1);
        result = obj;
    }
    return (cBase *)result;
}
