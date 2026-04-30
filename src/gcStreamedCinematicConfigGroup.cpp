// gcStreamedCinematicConfigGroup: holds a cBaseArray of gcStreamedCinematicConfig.
// Inherits cNamed (uses cNamed::Write inline). 40-byte struct.

class cBase;
class cFile;
class cReadBlock;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};
class cType;

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

class cNamed {
public:
    cBase *m_parent;
    void *m_classdesc;
    int m_n0;
    int m_n1;
    int m_n2;
    int m_n3;
    int m_n4;
    int m_n5;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static cBase *New(cMemPool *, cBase *);
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
    void RemoveAll(void);
    cBaseArray &operator=(const cBaseArray &);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

template <class T> class cGUIDT {
public:
    int m0;
    int m4;
};

class gcStreamedCinematic;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

struct DeleteEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

extern char gcStreamedCinematicConfigGroupclassdesc[];

void gcStreamedCinematicConfigGroup_ctor(void *, cBase *);
void cFile_SetCurrentPos(void *, unsigned int);

class gcStreamedCinematicConfigGroup : public cNamed {
public:
    void *m_arrayData;            // 0x20 (cBaseArray data ptr)
    void *m_owner;                // 0x24

    gcStreamedCinematicConfigGroup(cBase *);
    ~gcStreamedCinematicConfigGroup();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void *FindStreamedCinematic(const cGUIDT<gcStreamedCinematic> &, int *) const;
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteEntry *entry = (DeleteEntry *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = entry->offset;
        void (*fn)(void *, void *) = entry->fn;
        fn(block + off, p);
    }
};

gcStreamedCinematicConfigGroup *dcast(const cBase *);

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_00099ADC;

// ── Constructor ──

gcStreamedCinematicConfigGroup::gcStreamedCinematicConfigGroup(cBase *parent) {
    m_parent = parent;
    *(short *)((char *)this + 0x1C) = 0;
    *(short *)((char *)this + 0x1E) = 0;
    *(unsigned char *)((char *)this + 0x08) = 0;
    ((void **)this)[1] = gcStreamedCinematicConfigGroupclassdesc;
    m_arrayData = 0;
    m_owner = this;
}

// ── Write ──

void gcStreamedCinematicConfigGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cNamed::Write(file);
    ((cBaseArray *)((char *)this + 0x20))->Write(wb);
    wb.End();
}

// ── Read ──

int gcStreamedCinematicConfigGroup::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if (rb._data[3] != 1 || cNamed::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    ((cBaseArray *)((char *)this + 0x20))->Read(rb);
    return result;
}

// ── AssignCopy ──

struct _CopyBlob {
    int w0, w1, w2, w3, w4, w5;
};

void gcStreamedCinematicConfigGroup::AssignCopy(const cBase *base) {
    gcStreamedCinematicConfigGroup *src = dcast(base);
    *(_CopyBlob *)((char *)this + 8) = *(_CopyBlob *)((char *)src + 8);
    ((cBaseArray *)((char *)this + 0x20))->operator=(
        *(cBaseArray *)((char *)src + 0x20));
}

// ── New ──

cBase *gcStreamedCinematicConfigGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcStreamedCinematicConfigGroup *result = 0;
    gcStreamedCinematicConfigGroup *obj =
        (gcStreamedCinematicConfigGroup *)entry->fn(base, 0x28, 4, 0, 0);
    if (obj != 0) {
        gcStreamedCinematicConfigGroup_ctor(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── GetType ──

const cType *gcStreamedCinematicConfigGroup::GetType(void) const {
    if (D_00099ADC == 0) {
        if (D_000385E0 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                               &cNamed::New, 0, 0, 0);
        }
        D_00099ADC = cType::InitializeType(0, 0, 0x114, D_000385E0,
                                           &gcStreamedCinematicConfigGroup::New,
                                           0, 0, 0);
    }
    return D_00099ADC;
}

// SNC emitted a branch-loop pad in this destructor's symbol range.
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

// ── Destructor ──

gcStreamedCinematicConfigGroup::~gcStreamedCinematicConfigGroup() {
    *(void **)((char *)this + 4) = gcStreamedCinematicConfigGroupclassdesc;
    void *array = (char *)this + 0x20;
    if (array != 0) {
        ((cBaseArray *)array)->RemoveAll();
    }
    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}

// ── FindStreamedCinematic ──

void *gcStreamedCinematicConfigGroup::FindStreamedCinematic(
    const cGUIDT<gcStreamedCinematic> &guid, int *outIdx) const {
    int i = 0;
    int offset = 0;
    int count = 0;
    char *arr = (char *)m_arrayData;
    for (;;) {
        count = 0;
        if (arr != 0) {
            count = ((int *)arr)[-1];
        }
        if (!(i < count)) {
            break;
        }
        int *entry = *(int **)(arr + offset);
        if (entry != 0) {
            bool matched = (entry[2] == guid.m0) && (entry[3] == guid.m4);
            if (matched) {
                if (outIdx != 0) {
                    *outIdx = i;
                }
                return entry;
            }
        }
        i += 1;
        offset += 4;
    }
    if (outIdx != 0) {
        *outIdx = -1;
    }
    return 0;
}
