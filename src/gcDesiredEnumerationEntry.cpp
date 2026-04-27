// gcDesiredEnumerationEntry — entry holding a Helper at +8 and a tagged value at +0x14.
//
// Functions matched here:
//   gcDesiredEnumerationEntry::Write(cFile &) const                 @ 0x0010eacc
//   gcDesiredEnumerationEntry::New(cMemPool *, cBase *) static      @ 0x0025b334
//   gcDesiredEnumerationEntry::GetType(void) const                  @ 0x0025b3d4

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;
class cMemPool;
class cType;
class cBase;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcDesiredEnumerationEntryHelper {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern char gcDesiredEnumerationEntry_cBase_vtable[];   // 0x37E6A8
extern char gcDesiredEnumerationEntryvirtualtable[];    // 0x388568

extern const char gcDesiredEnumerationEntry_base_name[]; // 0x36D894
extern const char gcDesiredEnumerationEntry_base_desc[]; // 0x36D89C

extern cType *D_000385DC;
extern cType *D_0009A3F0;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcDesiredEnumerationEntry {
public:
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

// ============================================================
// 0x0010eacc — Write(cFile &) const, 76B
// ============================================================
void gcDesiredEnumerationEntry::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    ((const gcDesiredEnumerationEntryHelper *)((const char *)this + 8))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x14))->Write(wb);
    wb.End();
}

// ============================================================
// 0x0025b334 — New(cMemPool *, cBase *) static, 160B
// ============================================================
cBase *gcDesiredEnumerationEntry::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDesiredEnumerationEntry *result = 0;
    gcDesiredEnumerationEntry *obj = (gcDesiredEnumerationEntry *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = gcDesiredEnumerationEntry_cBase_vtable;
        ((void **)obj)[0] = parent;
        ((void **)obj)[1] = gcDesiredEnumerationEntryvirtualtable;
        *((char *)obj + 8) = 1;
        *((char *)obj + 9) = 0;
        *(int *)((char *)obj + 12) = 0;
        *(int *)((char *)obj + 16) = 0;
        *(int *)((char *)obj + 20) = (int)((unsigned int)parent | 1);
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x0025b3d4 — GetType(void) const, 160B
// ============================================================
const cType *gcDesiredEnumerationEntry::GetType(void) const {
    if (D_0009A3F0 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType(gcDesiredEnumerationEntry_base_name,
                                               gcDesiredEnumerationEntry_base_desc,
                                               1, 0, 0, 0, 0, 0);
        }
        D_0009A3F0 = cType::InitializeType(0, 0, 0x1A5, D_000385DC,
                                           &gcDesiredEnumerationEntry::New, 0, 0, 0);
    }
    return D_0009A3F0;
}
