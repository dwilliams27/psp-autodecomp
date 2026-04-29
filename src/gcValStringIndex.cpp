// gcValStringIndex — gcValue subclass holding a string-table reference.
//
// Functions matched here:
//   gcValStringIndex::Write(cFile &) const   @ 0x0035ea8c  (gcAll_psp.obj)
//   gcValStringIndex::GetText(char *) const  @ 0x0035ec18  (gcAll_psp.obj)
//
// The class layout (relevant subset):
//   0x00..0x07  gcValue base (mParent, mClassdesc)
//   0x08..0x0f  gcDesiredObject { _parent, mType }  ← reflective member
//
// Both methods do reflective dispatch through gcDesiredObject::mType: the
// cType holds entries at 0x28 (write) and 0x78 (text), each a {short offset,
// short pad, void *fn} record. The offset re-bases the call inside the
// gcDesiredObject payload.

class cFile;
class cBase;
class cMemPool;

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char _p0[0x28];
    cTypeMethod write_m;    // 0x28
    cTypeMethod read_m;     // 0x30
    char _p1[0x40];
    cTypeMethod text_m;     // 0x78
};

class gcDesiredObject {
public:
    int _parent;
    cType *mType;
};

class gcValue {
public:
    cBase *mParent;
    void *mVtable;
    void Write(cFile &) const;
};

class gcValStringIndex : public gcValue {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    void GetText(char *) const;
};

extern "C" void cStrCat(char *, const char *);
void gcDesiredObject_gcDesiredObject(void *, cBase *);

extern const char gcValStringIndex_text[];   // @ 0x36F79C
extern char cBaseclassdesc[];
extern char gcValuevirtualtable[];
extern char gcDesiredString_cBase_vtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// 0x0035e8b4 (192B) — New
cBase *gcValStringIndex::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValStringIndex *result = 0;
    gcValStringIndex *obj = (gcValStringIndex *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcValuevirtualtable;
        void *desired = (char *)obj + 8;
        gcDesiredObject_gcDesiredObject(desired, (cBase *)obj);
        ((void **)obj)[3] = gcDesiredString_cBase_vtable;
        ((unsigned char *)obj)[0x14] = 0;
        ((unsigned char *)obj)[0x15] = 0;
        ((int *)obj)[6] = 0;
        ((int *)obj)[7] = 0;
        ((int *)obj)[8] = (int)((unsigned int)desired | 1);
        result = obj;
    }
    return (cBase *)result;
}

// 0x0035ea8c (108B) — Write
void gcValStringIndex::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcValue::Write(file);
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 40);
    char *base = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)e->fn)(base + e->offset, wb.file);
    wb.End();
}

// 0x0035ec18 (80B) — GetText
void gcValStringIndex::GetText(char *buf) const {
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 120);
    char *base = (char *)this + 8;
    typedef void (*TextFn)(void *, char *);
    ((TextFn)e->fn)(base + e->offset, buf);
    cStrCat(buf, gcValStringIndex_text);
}
