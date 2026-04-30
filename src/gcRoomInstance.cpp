inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cType;

template <class T> T *dcast(const cBase *);

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
    char _pad[0x44];

    cObject(cBase *);
    cObject &operator=(const cObject &);
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

class gcEvent {
public:
    gcEvent(cBase *, const char *);
    gcEvent &operator=(const gcEvent &);
};

class gcRoomInstance : public cObject {
public:
    gcRoomInstance(cBase *);
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct TypeDispatchEntry {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

extern "C" void gcEvent_ctor(void *, cBase *, const char *)
    __asm__("__0oHgcEventctP6FcBasePCc");

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_0009F59C;

gcRoomInstance::gcRoomInstance(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = (void *)0x38B5D8;
    *(int *)((char *)this + 0x44) = 0;
    *(int *)((char *)this + 0x48) = 0;
    gcEvent_ctor((char *)this + 0x4C, (cBase *)this, 0);
    gcEvent_ctor((char *)this + 0x68, (cBase *)this, 0);
    *(int *)((char *)this + 0x84) = 0;
}

const cType *gcRoomInstance::GetType(void) const {
    if (D_0009F59C == 0) {
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
        D_0009F59C = cType::InitializeType(0, 0, 0x220, D_000385E4,
                                           &gcRoomInstance::New,
                                           (const char *)0x36DA70,
                                           (const char *)0x36DA80,
                                           2);
    }
    return D_0009F59C;
}

void gcRoomInstance::AssignCopy(const cBase *base) {
    gcRoomInstance *other = dcast<gcRoomInstance>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(cHandle *)((char *)this + 0x44) = *(const cHandle *)((const char *)other + 0x44);
    *(cHandle *)((char *)this + 0x48) = *(const cHandle *)((const char *)other + 0x48);
    ((gcEvent *)((char *)this + 0x4C))->operator=(*(const gcEvent *)((const char *)other + 0x4C));
    ((gcEvent *)((char *)this + 0x68))->operator=(*(const gcEvent *)((const char *)other + 0x68));
    *(int *)((char *)this + 0x84) = *(const int *)((const char *)other + 0x84);
}

void gcRoomInstance::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cObject *)this)->Write(file);
    ((const cHandle *)((const char *)this + 0x44))->Write(wb);
    ((const cHandle *)((const char *)this + 0x48))->Write(wb);

    char *type0 = *(char **)((const char *)this + 0x50);
    TypeDispatchEntry *entry0 = (TypeDispatchEntry *)(type0 + 0x28);
    const char *base0 = (const char *)this + 0x4C;
    entry0->fn((void *)(base0 + entry0->offset), wb._data[0]);

    char *type1 = *(char **)((const char *)this + 0x6C);
    TypeDispatchEntry *entry1 = (TypeDispatchEntry *)(type1 + 0x28);
    const char *base1 = (const char *)this + 0x68;
    entry1->fn((void *)(base1 + entry1->offset), wb._data[0]);

    wb.End();
}

cBase *gcRoomInstance::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcRoomInstance *result = 0;
    gcRoomInstance *obj = (gcRoomInstance *)entry->fn(base, 0x88, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcRoomInstance(parent);
        result = obj;
    }
    return (cBase *)result;
}
