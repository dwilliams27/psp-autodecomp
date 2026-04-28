class cBase;
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

class eBspTree {
public:
    eBspTree(cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
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

extern cType *D_000385DC;
extern cType *D_00040F78;

cBase *eBspTree::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eBspTree *result = 0;
    eBspTree *obj = (eBspTree *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        new (obj) eBspTree(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *eBspTree::GetType(void) const {
    if (D_00040F78 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                               (const char *)0x36CD7C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00040F78 = cType::InitializeType(0, 0, 0x25A, D_000385DC,
                                           &eBspTree::New, 0, 0, 0);
    }
    return D_00040F78;
}
