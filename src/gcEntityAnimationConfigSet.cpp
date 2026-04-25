// gcEntityAnimationConfigSet — inherits gcNamedSet and holds a cBaseArray.
// Decompiled functions:
//   0x0025aa80 gcEntityAnimationConfigSet::SetSize(int)
//   0x0025a8c4 gcEntityAnimationConfigSet::AssignCopy(const cBase *)
//   0x0010e270 gcEntityAnimationConfigSet::Write(cFile &) const
//   0x0025ad30 gcEntityAnimationConfigSet::~gcEntityAnimationConfigSet(void)
//   0x0025a908 gcEntityAnimationConfigSet::New(cMemPool *, cBase *) static

class cBase;
class cFile {
public:
    void SetCurrentPos(unsigned int);
};
class cMemPool;
class gcEntityAnimationConfig;

class cType {
public:
    cType *mParent; // 0x1C in the real layout; accessed through cTypeNode below.
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cBaseArray {
public:
    void *mData;
    void *mOwner;
    void SetSize(int);
    void Write(cWriteBlock &) const;
    void Read(void *);
    void RemoveAll(void);
    cBaseArray &operator=(const cBaseArray &);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class gcNamedSetName {
public:
    int mIndex;
};

class gcNamedSet {
public:
    cBase *m_parent;     // 0x00
    void *m_vtable;      // 0x04
    gcNamedSetName mName;// 0x08
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class gcEntityAnimationConfigSet : public gcNamedSet {
public:
    cBaseArray mArray;   // 0x0C (data ptr) + 0x10 (self ptr) — 8 bytes

    int GetSize(void) const;
    void SetSize(int);
    void AssignCopy(const cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    static cBase *New(cMemPool *, cBase *);
    static int GetAnimationIndex(const cBaseArray &, const gcEntityAnimationConfig *, int *, const gcEntityAnimationConfig *);
};

gcEntityAnimationConfigSet *dcast(const cBase *);

extern char gcEntityAnimationConfigSet_cBase_vtable[];       // 0x37E6A8
extern char gcEntityAnimationConfigSet_gcNamedSet_vtable[];  // 0x386C58
extern char gcEntityAnimationConfigSetvirtualtable[];        // 0x3884E8
extern char cBaseclassdesc[];                                // 0x37E6A8

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
    short _pad;
    void (*fn)(void *, void *);
};

void *cMemPool_GetPoolFromPtr(void *);

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000998F4;
extern cType *D_0009A3EC;

struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
};

struct VTableSlot {
    short offset;
    short pad;
    const cType *(*fn)(void *);
};

struct VTableSlotVoid {
    short offset;
    short pad;
    void (*fn)(void *, cMemPool *, int);
};

struct VTableSlotVisit {
    short offset;
    short pad;
    void (*fn)(void *, unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

// ============================================================
// 0x0025aa9c — GetSize(void) const
// ============================================================
int gcEntityAnimationConfigSet::GetSize(void) const {
    int size = 0;
    int *data = (int *)mArray.mData;
    if (data != 0) {
        size = data[-1];
    }
    return size;
}

// ============================================================
// 0x0025aa80 — SetSize(int): forward to cBaseArray::SetSize on member at +12
// ============================================================
void gcEntityAnimationConfigSet::SetSize(int size) {
    ((cBaseArray *)((char *)this + 12))->SetSize(size);
}

// ============================================================
// 0x0010e3b0 — GetAnimationIndex
// ============================================================
int gcEntityAnimationConfigSet::GetAnimationIndex(
        const cBaseArray &sets,
        const gcEntityAnimationConfig *config,
        int *setIndex,
        const gcEntityAnimationConfig *animIndexOut) {
    int neg = -1;
    void **arg0 = (void **)&sets;
    *(int *)animIndexOut = neg;
    *setIndex = neg;
    int outerIndex = 0;
    void *outerData = *arg0;
    int outerOffset = 0;
loop_1:
    {
        int outerSize = 0;
        if (outerData != 0) {
            outerSize = ((int *)outerData)[-1];
        }
        if (outerIndex < outerSize) {
            gcEntityAnimationConfigSet *set =
                *(gcEntityAnimationConfigSet **)((char *)*arg0 + outerOffset);
            int innerIndex = 0;
            int innerOffset = 0;
            cBaseArray *inner = (cBaseArray *)((char *)set + 12);
loop_5:
            void *innerData = inner->mData;
            int innerSize = 0;
            if (innerData != 0) {
                innerSize = ((int *)innerData)[-1];
            }
            if (innerIndex >= innerSize) {
                outerIndex++;
                outerOffset += 4;
                goto loop_1;
            }
            if (*(gcEntityAnimationConfig **)((char *)set->mArray.mData + innerOffset) == config) {
                *setIndex = outerIndex;
                *(int *)animIndexOut = innerIndex;
                return 1;
            }
            innerIndex++;
            innerOffset += 4;
            goto loop_5;
        }
    }
    return 0;
}

// ============================================================
// 0x0025a8c4 — AssignCopy(const cBase *)
// ============================================================
void gcEntityAnimationConfigSet::AssignCopy(const cBase *base) {
    gcEntityAnimationConfigSet *src = dcast(base);
    ((gcNamedSet *)this)->mName = ((gcNamedSet *)src)->mName;
    __asm__ volatile("" ::: "memory");
    this->mArray = src->mArray;
}

// ============================================================
// 0x0010e270 — Write(cFile &) const
// ============================================================
void gcEntityAnimationConfigSet::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcNamedSet::Write(file);
    ((cBaseArray *)((char *)this + 12))->Write(wb);
    wb.End();
}

// ============================================================
// 0x0025a9a4 — GetType
// ============================================================
const cType *gcEntityAnimationConfigSet::GetType(void) const {
    if (D_0009A3EC == 0) {
        if (D_000998F4 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894, (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_000998F4 = cType::InitializeType(0, 0, 0xAD, D_000385DC, 0, 0, 0, 0);
        }
        D_0009A3EC = cType::InitializeType(0, 0, 0xB1, D_000998F4,
                                           gcEntityAnimationConfigSet::New, 0, 0, 0);
    }
    return D_0009A3EC;
}

// ============================================================
// 0x0010e2c8 — Read(cFile &, cMemPool *)
// ============================================================
int gcEntityAnimationConfigSet::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != 1 || gcNamedSet::Read(file, pool) == 0) {
        file.SetCurrentPos(rb._data[1]);
        return 0;
    }
    mArray.Read(&rb);
    void *vt = m_vtable;
    VTableSlotVoid *slot = (VTableSlotVoid *)((char *)vt + 0x38);
    slot->fn((char *)this + slot->offset, pool, 0);
    return result;
}

// ============================================================
// 0x0025aab4 — VisitReferences
// ============================================================
void gcEntityAnimationConfigSet::VisitReferences(
        unsigned int flags,
        cBase *base,
        void (*callback)(cBase *, unsigned int, void *),
        void *ctx,
        unsigned int mask) {
    gcNamedSet::VisitReferences(flags, base, callback, ctx, mask);
    int i = 0;
    int offset = 0;
    unsigned int maskedFlags = flags & 0xFE00;
    int size = 0;
    int *data = (int *)mArray.mData;
    if (data != 0) {
        size = data[-1];
    }
    while (i < size) {
        cBase *child = *(cBase **)((char *)mArray.mData + offset);
        if (child != 0) {
            cBase *typedChild = 0;
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                           (const char *)0x36D89C,
                                                           1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                       (cBase *(*)(cMemPool *, cBase *))0x1C3C58,
                                                       0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0, 0, 0, 0, 0);
            }

            void *vt = ((void **)child)[1];
            VTableSlot *typeSlot = (VTableSlot *)((char *)vt + 8);
            const cType *childType = typeSlot->fn((char *)child + typeSlot->offset);
            int ok = 0;
            if (D_000385E4 != 0 && childType != 0) {
                const cTypeNode *node = (const cTypeNode *)childType;
                do {
                    if (node == (const cTypeNode *)D_000385E4) {
                        ok = 1;
                        break;
                    }
                    node = node->parent;
                } while (node != 0);
            }
            if (ok != 0) {
                typedChild = child;
            }

            if (typedChild == 0 || ((maskedFlags & *(unsigned short *)((char *)typedChild + 0x28)) == 0)) {
                cBase *nextChild = *(cBase **)((char *)mArray.mData + offset);
                void *nextVt = ((void **)nextChild)[1];
                VTableSlotVisit *visitSlot = (VTableSlotVisit *)((char *)nextVt + 0x60);
                visitSlot->fn((char *)nextChild + visitSlot->offset, flags, base, callback, ctx, mask);
            } else if (callback != 0) {
                callback(base, (unsigned int)*(cBase **)((char *)mArray.mData + offset), ctx);
            }
        }
        offset += 4;
        i++;
    }
}

// ============================================================
// 0x0025a908 — New(cMemPool *, cBase *) static
// ============================================================
cBase *gcEntityAnimationConfigSet::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcEntityAnimationConfigSet *result = 0;
    gcEntityAnimationConfigSet *obj =
        (gcEntityAnimationConfigSet *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = gcEntityAnimationConfigSet_cBase_vtable;
        ((void **)obj)[0] = parent;
        ((void **)obj)[1] = gcEntityAnimationConfigSet_gcNamedSet_vtable;
        ((int *)obj)[2] = 0;
        ((void **)obj)[1] = gcEntityAnimationConfigSetvirtualtable;
        ((int *)obj)[3] = 0;
        ((int **)obj)[4] = (int *)obj;
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x0025ad30 — destructor
// ============================================================
extern "C" {

void gcEntityAnimationConfigSet___dtor_gcEntityAnimationConfigSet_void(
        gcEntityAnimationConfigSet *self, int flags) {
    if (self != 0) {
        ((void **)self)[1] = gcEntityAnimationConfigSetvirtualtable;
        void *arr = (char *)self + 12;
        if (arr != 0) {
            ((cBaseArray *)arr)->RemoveAll();
        }
        ((void **)self)[1] = cBaseclassdesc;
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}

}
