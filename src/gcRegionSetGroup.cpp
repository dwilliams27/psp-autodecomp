// gcRegionSetGroup — inherits cNamed, holds a cBaseArray of region sets. 40-byte struct.
// Decompiled functions:
//   0x000ef96c gcRegionSetGroup::Write(cFile &) const
//   0x00246090 gcRegionSetGroup::AssignCopy(const cBase *)
//   0x002460fc gcRegionSetGroup::New(cMemPool *, cBase *) static

class cBase;
class cFile;
class cMemPool;
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
    static cBase *New(cMemPool *, cBase *);
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
    cBaseArray &operator=(const cBaseArray &);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

class gcRegionSetGroup : public cNamed {
public:
    void *m_arrayData;            // 0x20 (cBaseArray data ptr)
    void *m_owner;                // 0x24

    void Write(cFile &) const;
    int FindRegionSet(unsigned int) const;
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

gcRegionSetGroup *dcast(const cBase *);

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_00099AEC;

// ============================================================
// 0x000ef96c — Write(cFile &) const
// ============================================================
void gcRegionSetGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cNamed::Write(file);
    ((cBaseArray *)((char *)this + 0x20))->Write(wb);
    wb.End();
}

// ============================================================
// 0x000efa8c — FindRegionSet(unsigned int) const
// ============================================================
int gcRegionSetGroup::FindRegionSet(unsigned int id) const {
    void **items = (void **)m_arrayData;
    int i = 0;
    int offset = 0;
    int count;

    do {
        count = 0;
        if (items != 0) {
            count = ((int *)items)[-1];
        }
        if (i >= count) {
            break;
        }
        void *regionSet = *(void **)((char *)items + offset);
        if (regionSet != 0) {
            unsigned int regionId = *(volatile unsigned int *)((char *)regionSet + 0x18);
            if (regionId == id) {
                return i;
            }
        }
        i++;
        offset += 4;
    } while (1);
    return -1;
}

// ============================================================
// 0x00246090 — AssignCopy(const cBase *)
// ============================================================
struct _CopyBlob6 {
    int w0, w1, w2, w3, w4, w5;
};

void gcRegionSetGroup::AssignCopy(const cBase *base) {
    gcRegionSetGroup *other = dcast(base);
    const int *src = (const int *)((const char *)other + 8);
    int w0 = src[0];
    int *dst = (int *)((char *)this + 8);
    int w1 = src[1];
    int w2 = src[2];
    dst[0] = w0;
    int w3 = src[3];
    dst[1] = w1;
    int w4 = src[4];
    dst[2] = w2;
    int w5 = src[5];
    dst[3] = w3;
    dst[4] = w4;
    dst[5] = w5;
    ((cBaseArray *)((char *)this + 0x20))->operator=(
        *(cBaseArray *)((char *)other + 0x20));
}

// ============================================================
// 0x002460fc — New(cMemPool *, cBase *) static
// ============================================================
cBase *gcRegionSetGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    int result = 0;
    gcRegionSetGroup *obj = (gcRegionSetGroup *)entry->fn(base, 0x28, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = (void *)0x37E6A8;
        ((void **)obj)[0] = parent;
        ((void **)obj)[1] = (void *)0x37E8A8;
        *(short *)((char *)obj + 0x1C) = 0;
        *(short *)((char *)obj + 0x1E) = 0;
        *(unsigned char *)((char *)obj + 0x08) = 0;
        ((void **)obj)[1] = (void *)0x387A68;
        *(int *)((char *)obj + 0x20) = 0;
        *(void **)((char *)obj + 0x24) = obj;
        result = (int)obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x002461a0 — GetType(void) const
// ============================================================
const cType *gcRegionSetGroup::GetType(void) const {
    if (D_00099AEC == 0) {
        if (D_000385E0 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                               &cNamed::New, 0, 0, 0);
        }
        D_00099AEC = cType::InitializeType(0, 0, 0x113, D_000385E0,
                                           &gcRegionSetGroup::New, 0, 0, 0);
    }
    return D_00099AEC;
}
