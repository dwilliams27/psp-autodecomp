#include "gcDoEntitySendMessage.h"
#include "cBase.h"

void gcAction_gcAction(gcDoEntitySendMessage *, cBase *);
void gcDesiredObject_gcDesiredObject(void *, void *);
void gcDesiredEntityHelper_gcDesiredEntityHelper(void *, int, int, int);
void cBaseArray_SetSize(void *, int);

extern char gcDoEntitySendMessagevirtualtable[];
extern char gcDoEntitySendMessage_desobj_vtable[];
extern char gcDoEntitySendMessage_vtable1[];
extern char gcDoEntitySendMessage_handle_base_vtable[];
extern char gcDoEntitySendMessage_handle_vtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

cBase *gcDoEntitySendMessage::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntitySendMessage *result = 0;
    gcDoEntitySendMessage *obj = (gcDoEntitySendMessage *)entry->fn(base, 0x64, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoEntitySendMessagevirtualtable;
        void *desobj = (char *)obj + 0x0C;
        gcDesiredObject_gcDesiredObject(desobj, obj);
        ((void **)obj)[4] = gcDoEntitySendMessage_desobj_vtable;
        gcDesiredEntityHelper_gcDesiredEntityHelper((char *)obj + 0x18, 1, 0, 0);
        ((void **)obj)[4] = gcDoEntitySendMessage_vtable1;
        ((void **)obj)[9] = gcDoEntitySendMessage_handle_base_vtable;
        ((void **)obj)[8] = desobj;
        ((void **)obj)[9] = gcDoEntitySendMessage_handle_vtable;
        ((char *)obj)[0x28] = 1;
        ((char *)obj)[0x29] = 0;
        ((int *)obj)[0x0B] = 0;
        ((int *)obj)[0x0C] = 0;
        int encoded1 = (int)desobj | 1;
        ((void **)obj)[0x0F] = gcDoEntitySendMessage_handle_base_vtable;
        ((int *)obj)[0x0D] = encoded1;
        ((int *)obj)[0x0E] = (int)obj;
        ((void **)obj)[0x0F] = gcDoEntitySendMessage_handle_vtable;
        ((char *)obj)[0x40] = 1;
        ((char *)obj)[0x41] = 0;
        ((int *)obj)[0x11] = 0;
        void *arr = (char *)obj + 0x50;
        int encoded2 = (int)obj | 1;
        ((int *)obj)[0x12] = 0;
        ((int *)obj)[0x13] = encoded2;
        ((int *)obj)[0x14] = 0;
        ((int *)obj)[0x15] = (int)obj;
        ((int *)obj)[0x16] = encoded2;
        ((int *)obj)[0x17] = 0;
        ((int *)obj)[0x18] = 0;
        cBaseArray_SetSize(arr, 4);
        result = obj;
    }
    return (cBase *)result;
}
