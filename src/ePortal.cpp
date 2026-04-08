#include "ePortal.h"

extern char ePortalvirtualtable[];
extern void *__vec_new(void *array, int count, int size, void (*ctor)(void *));
extern void cHandleT_eRoom_ctor(void *);
extern void cObject___dtor_cObject_void(void *, int);

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

void ePortal::Activate(bool active) {
    *(unsigned char *)((char *)this + 0xF0) = (unsigned char)active;
}

void ePortal::VisitReferences(unsigned int a, cBase *b, void (*c)(cBase *, unsigned int, void *), void *d, unsigned int e) {
    cObject::VisitReferences(a, b, c, d, e);
}

void ePortal::DisconnectRoom(cHandleT<eRoom> handle, bool second) {
    int *room = (int *)((char *)this + ((second != 0) * 4) + 0xE8);
    if (((handle.mIndex == *room) & 0xFF) != 0) {
        volatile cHandleT<eRoom> empty;
        empty.mIndex = 0;
        *room = empty.mIndex;
    }
}

ePortal::ePortal(cBase *base) : cObject(base) {
    *(void **)((char *)this + 4) = ePortalvirtualtable;
    *(float *)((char *)this + 0xE0) = 0.0f;
    *(float *)((char *)this + 0xE4) = 0.0f;
    __vec_new((char *)this + 0xE8, 2, 4, cHandleT_eRoom_ctor);
    *(unsigned char *)((char *)this + 0xF0) = 1;
}

extern "C" {

void ePortal___dtor_ePortal_void(ePortal *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = ePortalvirtualtable;
        cObject___dtor_cObject_void(self, 0);
        if (flags & 1) {
            void *pool = cMemPool::GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}

}
