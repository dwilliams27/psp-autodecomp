// gcFunction.cpp — decompiled from gcAll_psp.obj

class cBase;
class cFile;
class cMemPool;

template <class T> T *dcast(const cBase *);

class cWriteBlock {
public:
    cFile *file;
    unsigned int _id;
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cObject {
public:
    cObject &operator=(const cObject &);
    void Write(cFile &) const;
};

class gcEvent {
public:
    char _pad[0x1C];
    gcEvent &operator=(const gcEvent &);
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcFunction {
public:
    char _pad[0x44];
    gcEvent mEvent;

    gcFunction(cBase *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern "C" void gcFunction__gcFunction_cBaseptr(void *self, cBase *parent);

// ── gcFunction::AssignCopy(const cBase *) @ 0x0027D108 ──
void gcFunction::AssignCopy(const cBase *base) {
    gcFunction *other = dcast<gcFunction>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    mEvent = other->mEvent;
}

// ── gcFunction::Write(cFile &) const @ 0x0012F7A8 ──
struct gcEventVEntry {
    short adj;
    short pad;
    void (*fn)(const void *, cFile *);
};

void gcFunction::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cObject *)this)->Write(file);
    const gcEventVEntry *vt = *(const gcEventVEntry **)((const char *)this + 0x48);
    const gcEventVEntry *e = vt + 5;
    short adj = e->adj;
    const char *base44 = (const char *)this + 0x44;
    void (*fn)(const void *, cFile *) = e->fn;
    fn(base44 + adj, wb.file);
    wb.End();
}

// ── gcFunction::New(cMemPool *, cBase *) static @ 0x0027D154 ──
cBase *gcFunction::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcFunction *result = 0;
    gcFunction *obj = (gcFunction *)entry->fn(base, 0x60, 4, 0, 0);
    if (obj != 0) {
        gcFunction__gcFunction_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
