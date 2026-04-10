#include "gcDoEntitySendMessage.h"
#include "cBase.h"

extern char gcDoFunctionvirtualtable[];
void gcAction___dtor_gcAction_void(void *, int);
void *cMemPool_GetPoolFromPtr(void *);
void cBaseArray_RemoveAll(void *);

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" {

void gcDoFunction___dtor_gcDoFunction_void(gcDoFunction *self, int flags) {
    if (self != 0) {
        ((void **)self)[1] = gcDoFunctionvirtualtable;
        void *arr = (char *)self + 0x10;
        if ((char *)self + 0x18 != 0) {
            int val = ((int *)self)[6];
            int owned = 1;
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0) {
                if (val != 0) {
                    void *vt = *(void **)((char *)val + 4);
                    DeleteRecord *rec = (DeleteRecord *)((char *)vt + 0x50);
                    short off = rec->offset;
                    rec->fn((char *)val + off, (void *)3);
                    ((int *)self)[6] = 0;
                }
            }
        }
        if (arr != 0) {
            cBaseArray_RemoveAll(arr);
        }
        gcAction___dtor_gcAction_void(self, 0);
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}

}
