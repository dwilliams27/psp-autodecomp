#include "ePortal.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void Write(int, const float *);
    void End(void);
};

extern char ePortalvirtualtable[];
extern void *__vec_new(void *array, int count, int size, void (*ctor)(void *));
extern void cHandleT_eRoom_ctor(void *);
extern void cObject___dtor_cObject_void(void *, int);
extern void *D_00038890[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000469A0;

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

const cType *ePortal::GetType(void) const {
    if (D_000469A0 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_000469A0 = cType::InitializeType(0, 0, 0x21E, D_000385E4,
                                           &ePortal::New,
                                           (const char *)0x36CE20,
                                           (const char *)0x36CE28, 3);
    }
    return D_000469A0;
}

void ePortal::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cObject::Write(file);

    int i = 0;
    const float *value = (const float *)((const char *)this + 0x50);
    do {
        wb.Write(3, value);
        i++;
        value += 4;
    } while (i < 4);

    wb.Write(3, (const float *)((const char *)this + 0x90));
    wb.Write(*(const float *)((const char *)this + 0x9C));
    wb.Write(3, (const float *)((const char *)this + 0xD0));
    wb.Write(3, (const float *)((const char *)this + 0xA0));
    wb.Write(3, (const float *)((const char *)this + 0xB0));
    wb.Write(3, (const float *)((const char *)this + 0xC0));
    wb.Write(*(const float *)((const char *)this + 0xE4));
    wb.Write(*(const float *)((const char *)this + 0xE0));
    wb.End();
}

void ePortal::Activate(bool active) {
    *(unsigned char *)((char *)this + 0xF0) = (unsigned char)active;
}

void ePortal::ConnectRoom(cHandleT<eRoom> handle, bool second) {
    int *room = (int *)((char *)this + ((second != 0) * 4) + 0xE8);
    int current = *room;
    int canStore;
    if (current == 0) {
        canStore = 1;
    } else {
        unsigned short index = (unsigned short)current;
        void *lookup = D_00038890[index];
        void *result = 0;
        if (lookup != 0) {
            if (*(int *)((char *)lookup + 0x30) == current) {
                result = lookup;
            }
        }
        canStore = (result == 0) & 0xFF;
    }
    if (canStore) {
        *room = handle.mIndex;
    }
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
