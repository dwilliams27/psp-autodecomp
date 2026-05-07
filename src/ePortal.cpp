#include "cObject.h"

// ODR-WARNING: local redeclaration used to add split-TU methods without
// changing the shared ePortal header, which can perturb matched siblings.
class eRoom;

template <class T>
class cHandleT {
public:
    int mIndex;
};

class cMemPool {
public:
    static void *GetPoolFromPtr(const void *);
};

class ePortal : public cObject {
public:
    ePortal(cBase *);
    ~ePortal(void);
    const cType *GetType(void) const;
    void Activate(bool);
    void AssignCopy(const cBase *);
    void ConnectRoom(cHandleT<eRoom>, bool);
    void DisconnectRoom(cHandleT<eRoom>, bool);
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

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
    void Write(int);
    void Write(float);
    void Write(int, const float *);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class eMaterialSet;
class eSurfaceSet;

class eSkin : public cObject {
public:
    void Write(cFile &) const;
};

template <class T>
T dcast(const cBase *);

typedef int v4sf_t __attribute__((mode(V4SF)));

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

void ePortal::AssignCopy(const cBase *base) {
    ePortal *other = dcast<ePortal *>(base);
    this->cObject::operator=(*(cObject *)other);

    register unsigned int i asm("$a2") = 0;
    register v4sf_t *dst asm("$a1") = (v4sf_t *)((char *)this + 0x50);
    register v4sf_t *src asm("$a0") = (v4sf_t *)((char *)other + 0x50);
    do {
        *dst = *src;
        i++;
        dst++;
        src++;
    } while (i < 4U);

    float value9C = *(float *)((char *)other + 0x9C);
    *(v4sf_t *)((char *)this + 0x90) = *(v4sf_t *)((char *)other + 0x90);
    *(float *)((char *)this + 0x9C) = value9C;
    *(v4sf_t *)((char *)this + 0xD0) = *(v4sf_t *)((char *)other + 0xD0);
    *(v4sf_t *)((char *)this + 0xA0) = *(v4sf_t *)((char *)other + 0xA0);
    *(v4sf_t *)((char *)this + 0xB0) = *(v4sf_t *)((char *)other + 0xB0);
    *(v4sf_t *)((char *)this + 0xC0) = *(v4sf_t *)((char *)other + 0xC0);

    register int j asm("$a2") = 0;
    *(float *)((char *)this + 0xE0) = *(float *)((char *)other + 0xE0);
    register int *dstHandles asm("$a1") = (int *)((char *)this + 0xE8);
    *(float *)((char *)this + 0xE4) = *(float *)((char *)other + 0xE4);
    register int *srcHandles asm("$a0") = (int *)((char *)other + 0xE8);
    do {
        *dstHandles = *srcHandles;
        j++;
        dstHandles++;
        srcHandles++;
    } while (j < 2);

    *(unsigned char *)((char *)this + 0xF0) =
        *(unsigned char *)((char *)other + 0xF0);
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

#pragma control sched=1
void eSkin::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cObject::Write(file);

    int *handles0 = *(int **)((const char *)this + 0x44);
    int count0 = 0;
    if (handles0 != 0) {
        count0 = handles0[-1] & 0x3FFFFFFF;
    }
    wb.Write(count0);

    int *handles = *(int **)((const char *)this + 0x44);
    int count = 0;
    if (handles != 0) {
        count = handles[-1] & 0x3FFFFFFF;
    }

    int i = 0;
    if (i < count) {
        int offset = 0;
        int *handle = handles + offset;
        do {
            ((const cHandle *)handle)->Write(wb);
            i++;
            handle++;
        } while (i < count);
    }

    int *handles1 = *(int **)((const char *)this + 0x48);
    int count1 = 0;
    if (handles1 != 0) {
        count1 = handles1[-1] & 0x3FFFFFFF;
    }
    __asm__ volatile("move $s1, %0" : : "r"(this));
    wb.Write(count1);

    int *handles2;
    __asm__ volatile("lw %0, 0x48($s1)" : "=r"(handles2));
    int count2 = 0;
    if (handles2 != 0) {
        count2 = handles2[-1] & 0x3FFFFFFF;
    }

    int j = 0;
    if (j < count2) {
        int offset = 0;
        int *handle = handles2 + offset;
        do {
            ((const cHandle *)handle)->Write(wb);
            j++;
            handle++;
        } while (j < count2);
    }

    wb.End();
}
#pragma control sched=2

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
