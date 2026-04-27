// gcTableColumnStringTable — string-table-typed table column.
//
// Functions matched here:
//   gcTableColumnStringTable::Write(cFile &) const            @ 0x0012b05c (gcAll_psp.obj)
//   gcTableColumnStringTable::New(cMemPool *, cBase *) static  @ 0x00272178 (gcAll_psp.obj)
//   gcTableColumnStringTable::GetType(void) const             @ 0x00272200 (gcAll_psp.obj)
//   gcTableColumnStringTable::IsValid(void) const             @ 0x002727dc (gcAll_psp.obj)
//   gcTableColumnStringTable::GetContainer(void) const        @ 0x00272828 (gcAll_psp.obj)
//   gcTableColumnStringTable::~gcTableColumnStringTable(void) @ 0x00272b8c (gcAll_psp.obj)
//
// (AssignCopy and Set(int,float) for this class live in other TUs and are
// already matched.)

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;

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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                  cBase *(*)(cMemPool *, cBase *),
                                  const char *, const char *, unsigned int);
};

extern char gcTableColumnclassdesc[];            // 0x37e6a8
extern char gcTableColumnStringTableclassdesc[]; // 0x389cf0

extern const char gcTableColumnStringTable_base_name[];
extern const char gcTableColumnStringTable_base_desc[];

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

class gcTableColumnStringTablePoolNS {
public:
    static gcTableColumnStringTablePoolNS *GetPoolFromPtr(const void *);
};

struct gcTableColumn {
    void *mOwner;
    void *mClassDesc;

    void Write(cFile &) const;
};

struct gcTableColumnStringTable : public gcTableColumn {
    cHandle mHandle;

    gcTableColumnStringTable(cBase *parent) {
        mClassDesc = gcTableColumnclassdesc;
        mOwner = parent;
        mClassDesc = gcTableColumnStringTableclassdesc;
        mHandle.mIndex = 0;
    }

    ~gcTableColumnStringTable();

    void Write(cFile &file) const;
    static cBase *New(cMemPool *pool, cBase *parent);
    const cType *GetType(void) const;
    int IsValid(void) const;
    void *GetContainer(void) const;

    // Inline so SNC inlines it into the deleting-destructor variant.
    static void operator delete(void *p) {
        gcTableColumnStringTablePoolNS *pool =
            gcTableColumnStringTablePoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec =
            (DeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

static cType *type_base;
static cType *type_intermediate;
static cType *type_gcTableColumnStringTable;

// 0x0012b05c — Write to cFile via cWriteBlock; writes column header + handle.
void gcTableColumnStringTable::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcTableColumn::Write(file);
    mHandle.Write(wb);
    wb.End();
}

// 0x00272178 — pool-allocate a new gcTableColumnStringTable and inline-construct.
cBase *gcTableColumnStringTable::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcTableColumnStringTable *result = 0;
    gcTableColumnStringTable *obj = (gcTableColumnStringTable *)entry->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcTableColumnStringTable(parent);
        result = obj;
    }
    return (cBase *)result;
}

// 0x002727dc — handle table validity check for the bound string-table handle.
// NOT YET MATCHING — shape is right but SNC differs in register allocation
// (uses a1 for mIndex instead of a0) and basic-block ordering (early-return
// at top vs at end). Diff ~53/76 bytes structural.
int gcTableColumnStringTable::IsValid(void) const {
    int idx = mHandle.mIndex;
    if (idx) {
        void **table = (void **)0x38890;
        void *cand = table[idx & 0xFFFF];
        void *r = 0;
        if (cand != 0 && *(int *)((char *)cand + 0x30) == idx) {
            r = cand;
        }
        return r != 0;
    }
    return 0;
}

// 0x00272828 — handle table lookup; returns the resolved entry or 0.
// NOT YET MATCHING — same structural issue as IsValid above.
void *gcTableColumnStringTable::GetContainer(void) const {
    int idx = mHandle.mIndex;
    if (idx) {
        void **table = (void **)0x38890;
        void *cand = table[idx & 0xFFFF];
        void *r = 0;
        if (cand != 0 && *(int *)((char *)cand + 0x30) == idx) {
            r = cand;
        }
        return r;
    }
    return 0;
}

// 0x00272b8c — destructor; resets classdesc to gcTableColumn (parent class).
// SNC's ABI auto-emits the (this != 0) guard and the deleting-tail dispatch
// through operator delete on (flag & 1).
gcTableColumnStringTable::~gcTableColumnStringTable() {
    mClassDesc = gcTableColumnclassdesc;
}

// 0x00272200 — lazily-initialized cType chain (base -> intermediate -> derived).
const cType *gcTableColumnStringTable::GetType(void) const {
    if (!type_gcTableColumnStringTable) {
        if (!type_intermediate) {
            if (!type_base) {
                type_base = cType::InitializeType(gcTableColumnStringTable_base_name,
                                                   gcTableColumnStringTable_base_desc,
                                                   1, 0, 0, 0, 0, 0);
            }
            type_intermediate = cType::InitializeType(0, 0, 0x241, type_base, 0, 0, 0, 0);
        }
        type_gcTableColumnStringTable = cType::InitializeType(0, 0, 0x245, type_intermediate,
                                                               gcTableColumnStringTable::New,
                                                               0, 0, 0);
    }
    return type_gcTableColumnStringTable;
}
