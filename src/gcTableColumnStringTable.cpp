// gcTableColumnStringTable — string-table-typed table column.
//
// Functions matched here:
//   gcTableColumnStringTable::Write(cFile &) const           @ 0x0012b05c (gcAll_psp.obj)
//   gcTableColumnStringTable::New(cMemPool *, cBase *) static @ 0x00272178 (gcAll_psp.obj)
//   gcTableColumnStringTable::GetType(void) const            @ 0x00272200 (gcAll_psp.obj)
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

    void Write(cFile &file) const;
    static cBase *New(cMemPool *pool, cBase *parent);
    const cType *GetType(void) const;
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
