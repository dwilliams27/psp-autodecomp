inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;

template <class T> T *dcast(const cBase *);

class cObject {
public:
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
    gcEvent &operator=(const gcEvent &);
};

class gcRoomInstance {
public:
    gcRoomInstance(cBase *);

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
