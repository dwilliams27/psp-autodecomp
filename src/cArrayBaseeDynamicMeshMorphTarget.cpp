class eDynamicMeshMorphTarget;
class eDynamicMeshVisSkinTriList;
class gcStateMachine;

class cMemPool {
public:
    static void *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern void eDynamicMeshMorphTarget___dtor_eDynamicMeshMorphTarget_void(void *, int);
extern "C" void free(void *);

template <class T>
class cArrayBase {
public:
    int mData;
    cArrayBase();
};

template <class T>
cArrayBase<T>::cArrayBase() : mData(0) {
}

template cArrayBase<eDynamicMeshMorphTarget>::cArrayBase();
template cArrayBase<eDynamicMeshVisSkinTriList>::cArrayBase();

template <class T>
class cHandleT {
public:
    int mIndex;
    cHandleT();
};

template <class T>
cHandleT<T>::cHandleT() : mIndex(0) {
}

template cHandleT<gcStateMachine>::cHandleT();

extern "C" {

void cArrayBase_eDynamicMeshMorphTarget____dtor_cArrayBase_void(void **self, int flags) {
    if (self != 0) {
        int count = 0;
        void *check = *self;
        if (check != 0) {
            count = *((int *)check - 1) & 0x3FFFFFFF;
        }
        int i = 0;
        int offset = 0;
        void *data = *self;
        while (i < count) {
            eDynamicMeshMorphTarget___dtor_eDynamicMeshMorphTarget_void((char *)data + offset, 2);
            i++;
            offset += 12;
            data = *self;
        }
        if (data != 0) {
            int *alloc = (int *)data - 1;
            if (alloc != 0) {
                void *pool = cMemPool::GetPoolFromPtr(alloc);
                void *block = *(void **)((char *)pool + 0x24);
                DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
                short off = rec->offset;
                rec->fn((char *)block + off, alloc);
            }
            *self = 0;
        }
        if ((flags & 1) && (self != 0)) {
            void *pool = cMemPool::GetPoolFromPtr(self);
            if (pool != 0) {
                void *block = *(void **)((char *)pool + 0x24);
                DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
                short off = rec->offset;
                rec->fn((char *)block + off, self);
            } else {
                free(self);
            }
        }
    }
}

}
