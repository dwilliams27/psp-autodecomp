// eRoomEnvironment.cpp - decompiled from eAll_psp.obj.

class cBase;
class cFile;
class cMemPool;

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cObject {
public:
    char _pad[0x44];
    cObject(cBase *);
    ~cObject();
    cObject &operator=(const cObject &);
    void Write(cFile &) const;
};

class eRoomEnvironment : public cObject {
public:
    eRoomEnvironment(cBase *);
    ~eRoomEnvironment();
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        void *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

extern char eRoomEnvironmentvirtualtable[];

extern "C" void eRoomEnvironment__eRoomEnvironment_cBaseptr(void *self, cBase *parent);

template <class T> T *dcast(const cBase *);

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void Write(unsigned int);
    void Write(float);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

struct CopyWord {
    int value;
    CopyWord &operator=(const CopyWord &rhs) { value = rhs.value; return *this; }
};

#pragma control sched=1

// eRoomEnvironment::Write(cFile &) const @ 0x0005c648
void eRoomEnvironment::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    cObject::Write(file);
    wb.Write(*(const bool *)((const char *)this + 0x48));
    wb.Write(*(const float *)((const char *)this + 0x4C));
    wb.Write(*(const float *)((const char *)this + 0x50));
    wb.Write(*(const unsigned int *)((const char *)this + 0x54));
    ((const cHandle *)((const char *)this + 0x58))->Write(wb);
    wb.Write(*(const bool *)((const char *)this + 0x5C));
    wb.Write(*(const int *)((const char *)this + 0x44));
    wb.End();
}

// eRoomEnvironment::~eRoomEnvironment(void) @ 0x0005c8c0
eRoomEnvironment::~eRoomEnvironment() {
    *(void **)((char *)this + 4) = eRoomEnvironmentvirtualtable;
}

// eRoomEnvironment::AssignCopy(const cBase *) @ 0x00203d24
void eRoomEnvironment::AssignCopy(const cBase *src) {
    eRoomEnvironment *other = dcast<eRoomEnvironment>(src);
    ((cObject *)this)->operator=(*(const cObject *)other);
    __asm__ volatile("" ::: "memory");
    *(CopyWord *)((char *)this + 0x44) = *(CopyWord *)((char *)other + 0x44);
    *(unsigned char *)((char *)this + 0x48) = *(const unsigned char *)((char *)other + 0x48);
    *(float *)((char *)this + 0x4C) = *(const float *)((char *)other + 0x4C);
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x50) = *(const float *)((char *)other + 0x50);
    __asm__ volatile("" ::: "memory");
    *(CopyWord *)((char *)this + 0x54) = *(CopyWord *)((char *)other + 0x54);
    *(CopyWord *)((char *)this + 0x58) = *(CopyWord *)((char *)other + 0x58);
    *(unsigned char *)((char *)this + 0x5C) = *(const unsigned char *)((char *)other + 0x5C);
    *(int *)((char *)this + 0x60) = *(const int *)((char *)other + 0x60);
}

// eRoomEnvironment::New(cMemPool *, cBase *) static @ 0x00203dbc
cBase *eRoomEnvironment::New(cMemPool *pool, cBase *parent) {
    eRoomEnvironment *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eRoomEnvironment *obj = (eRoomEnvironment *)rec->fn(base, 0x64, 4, 0, 0);
    if (obj != 0) {
        eRoomEnvironment__eRoomEnvironment_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
