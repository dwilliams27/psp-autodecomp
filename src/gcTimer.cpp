class cBase;
class cFile;
class cMemPool;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cObject {
public:
    cObject(cBase *);
    ~cObject(void);
    cObject &operator=(const cObject &);
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

class gcEvent {
public:
    gcEvent &operator=(const gcEvent &);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" void gcEvent_ctor(void *, cBase *, const char *) __asm__("__0oHgcEventctP6FcBasePCc");
extern "C" void gcEvent_dtor(void *, int) __asm__("__0oHgcEventdtv");

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_0009F49C;

class gcTimer : public cObject {
public:
    gcTimer(cBase *);
    ~gcTimer(void);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        rec->fn(block + off, p);
    }
};

typedef void (*WriteFn)(cBase *, cFile *);

struct TypeMethod {
    short offset;
    short _pad;
    WriteFn fn;
};

cBase *gcTimer::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcTimer *result = 0;
    gcTimer *obj = (gcTimer *)rec->fn(base, 0xC0, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcTimer(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcTimer::GetType(void) const {
    if (D_0009F49C == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_0009F49C = cType::InitializeType(0, 0, 0xDD, D_000385E4,
                                           &gcTimer::New,
                                           (const char *)0x36D9E0,
                                           (const char *)0x36D9E8, 4);
    }
    return D_0009F49C;
}

gcTimer::gcTimer(cBase *parent) : cObject(parent) {
    *(char **)((char *)this + 4) = (char *)0x38A0A0;
    gcEvent_ctor((char *)this + 0x44, (cBase *)this, (const char *)0x36DB40);
    gcEvent_ctor((char *)this + 0x60, (cBase *)this, (const char *)0x36DB4C);
    gcEvent_ctor((char *)this + 0x7C, (cBase *)this, (const char *)0x36DB64);
    gcEvent_ctor((char *)this + 0x98, (cBase *)this, (const char *)0x36DB58);
    *(int *)((char *)this + 0xB4) = 0;
    *(int *)((char *)this + 0xB8) = 0;
    *(int *)((char *)this + 0xBC) = 0;
}

gcTimer::~gcTimer(void) {
    *(char **)((char *)this + 4) = (char *)0x38A0A0;
    gcEvent_dtor((char *)this + 0x98, 2);
    gcEvent_dtor((char *)this + 0x7C, 2);
    gcEvent_dtor((char *)this + 0x60, 2);
    gcEvent_dtor((char *)this + 0x44, 2);
}

extern "C" void *dcastdcast_gcTimerptr__constcBaseptr(const cBase *);

void gcTimer::AssignCopy(const cBase *src) {
    gcTimer *other = (gcTimer *)dcastdcast_gcTimerptr__constcBaseptr(src);
    ((cObject *)this)->operator=(*(const cObject *)other);
    ((gcEvent *)((char *)this + 0x44))->operator=(*(const gcEvent *)((char *)other + 0x44));
    ((gcEvent *)((char *)this + 0x60))->operator=(*(const gcEvent *)((char *)other + 0x60));
    ((gcEvent *)((char *)this + 0x7C))->operator=(*(const gcEvent *)((char *)other + 0x7C));
    ((gcEvent *)((char *)this + 0x98))->operator=(*(const gcEvent *)((char *)other + 0x98));
    int value0 = *(const int *)((char *)other + 0xB4);
    const int *src1 = (const int *)((char *)other + 0xB8);
    *(int *)((char *)this + 0xB4) = value0;
    int *dst1 = (int *)((char *)this + 0xB8);
    const int *src2 = (const int *)((char *)other + 0xBC);
    int *dst2 = (int *)((char *)this + 0xBC);
    *dst1 = *src1;
    *dst2 = *src2;
}

void gcTimer::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cObject::Write(file);
    wb.Write(*(const unsigned int *)((const char *)this + 0xB4));
    wb.Write(*(const int *)((const char *)this + 0xBC));

    TypeMethod *slot0 = (TypeMethod *)((char *)*(void **)((char *)this + 0x48) + 0x28);
    cBase *base0 = (cBase *)((char *)this + 0x44);
    slot0->fn((cBase *)((char *)base0 + slot0->offset), wb._file);

    TypeMethod *slot1 = (TypeMethod *)((char *)*(void **)((char *)this + 0x64) + 0x28);
    cBase *base1 = (cBase *)((char *)this + 0x60);
    slot1->fn((cBase *)((char *)base1 + slot1->offset), wb._file);

    TypeMethod *slot2 = (TypeMethod *)((char *)*(void **)((char *)this + 0x80) + 0x28);
    cBase *base2 = (cBase *)((char *)this + 0x7C);
    slot2->fn((cBase *)((char *)base2 + slot2->offset), wb._file);

    TypeMethod *slot3 = (TypeMethod *)((char *)*(void **)((char *)this + 0x9C) + 0x28);
    cBase *base3 = (cBase *)((char *)this + 0x98);
    slot3->fn((cBase *)((char *)base3 + slot3->offset), wb._file);

    wb.End();
}
