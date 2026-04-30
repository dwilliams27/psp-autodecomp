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

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcEvent {
public:
    gcEvent &operator=(const gcEvent &);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct WriteRecord {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

extern char gcAnimationEventvirtualtable[];
extern char cBaseclassdesc[];

extern const char gcAnimationEvents_base_name[];
extern const char gcAnimationEvents_base_desc[];

extern cType *D_000385DC;
extern cType *D_00099AB8;

void *cMemPool_GetPoolFromPtr(const void *);
void gcEvent_gcEvent(void *, cBase *, const char *);

extern "C" {
    void gcAnimationEvents__gcAnimationEvents_cBaseptr(void *, cBase *);
    void gcEvent___dtor_gcEvent_void(void *, int);
}

template <class T> T *dcast(const cBase *);

class gcAnimationEvents {
public:
    gcAnimationEvents(cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    ~gcAnimationEvents();
};

inline void operator delete(void *p) {
    void *pool = cMemPool_GetPoolFromPtr(p);
    void *block = *(void **)((char *)pool + 0x24);
    DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
    short off = rec->offset;
    rec->fn((char *)block + off, p);
}

gcAnimationEvents::gcAnimationEvents(cBase *parent) {
    *(cBase **)((char *)this + 0) = parent;
    *(void **)((char *)this + 4) = gcAnimationEventvirtualtable;
    gcEvent_gcEvent((char *)this + 0x08, (cBase *)this, (const char *)0x36DB40);
    gcEvent_gcEvent((char *)this + 0x24, (cBase *)this, (const char *)0x36DB4C);
    gcEvent_gcEvent((char *)this + 0x40, (cBase *)this, (const char *)0x36DB58);
    gcEvent_gcEvent((char *)this + 0x5C, (cBase *)this, (const char *)0x36DB64);
}

cBase *gcAnimationEvents::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcAnimationEvents *result = 0;
    gcAnimationEvents *obj = (gcAnimationEvents *)entry->fn(base, 0x78, 4, 0, 0);
    if (obj != 0) {
        gcAnimationEvents__gcAnimationEvents_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

void gcAnimationEvents::Write(cFile &file) const {
    cWriteBlock wb(file, 1);

    char *typeInfo0 = *(char **)((char *)this + 0xC);
    WriteRecord *rec0 = (WriteRecord *)(typeInfo0 + 0x28);
    short off0 = rec0->offset;
    void *base0 = (char *)this + 8;
    rec0->fn((char *)base0 + off0, *(cFile **)&wb);

    char *typeInfo1 = *(char **)((char *)this + 0x28);
    WriteRecord *rec1 = (WriteRecord *)(typeInfo1 + 0x28);
    short off1 = rec1->offset;
    void *base1 = (char *)this + 0x24;
    rec1->fn((char *)base1 + off1, *(cFile **)&wb);

    char *typeInfo2 = *(char **)((char *)this + 0x44);
    WriteRecord *rec2 = (WriteRecord *)(typeInfo2 + 0x28);
    short off2 = rec2->offset;
    void *base2 = (char *)this + 0x40;
    rec2->fn((char *)base2 + off2, *(cFile **)&wb);

    char *typeInfo3 = *(char **)((char *)this + 0x60);
    WriteRecord *rec3 = (WriteRecord *)(typeInfo3 + 0x28);
    short off3 = rec3->offset;
    void *base3 = (char *)this + 0x5C;
    rec3->fn((char *)base3 + off3, *(cFile **)&wb);

    wb.End();
}

void gcAnimationEvents::AssignCopy(const cBase *base) {
    gcAnimationEvents *other = dcast<gcAnimationEvents>(base);
    gcEvent *src0 = (gcEvent *)((char *)other + 8);
    ((gcEvent *)((char *)this + 8))->operator=(*src0);
    *(gcEvent *)((char *)this + 0x24) = *(gcEvent *)((char *)other + 0x24);
    *(gcEvent *)((char *)this + 0x40) = *(gcEvent *)((char *)other + 0x40);
    *(gcEvent *)((char *)this + 0x5C) = *(gcEvent *)((char *)other + 0x5C);
}

const cType *gcAnimationEvents::GetType(void) const {
    if (D_00099AB8 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType(gcAnimationEvents_base_name,
                                               gcAnimationEvents_base_desc,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00099AB8 = cType::InitializeType(0, 0, 0x1A3, D_000385DC,
                                           &gcAnimationEvents::New, 0, 0, 0);
    }
    return D_00099AB8;
}

gcAnimationEvents::~gcAnimationEvents() {
    *(void **)((char *)this + 4) = gcAnimationEventvirtualtable;
    gcEvent___dtor_gcEvent_void((char *)this + 0x5C, 2);
    gcEvent___dtor_gcEvent_void((char *)this + 0x40, 2);
    gcEvent___dtor_gcEvent_void((char *)this + 0x24, 2);
    gcEvent___dtor_gcEvent_void((char *)this + 0x08, 2);
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
