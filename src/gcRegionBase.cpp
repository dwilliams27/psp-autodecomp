// gcRegionBase — base class for gcRegion and related region types.
//
// Matched functions:
//   0x000ef394  MemCardReplicate(gcReplicationVisitor &)
//   0x000ee0b8  Write(cFile &) const
//   0x000ee450  gcRegionBase(cBase *)
//   0x000ee4d8  ~gcRegionBase()
//   0x000ef2b4  OnMemPoolReset(const cMemPool *, unsigned int)

class cBase;
class cFile;
class cMemPool;
class gcReplicationVisitor;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class cBaseArray {
public:
    void RemoveAll(void);
    void Write(cWriteBlock &) const;
};

class cFactory {
public:
    void Write(cFile &) const;
    void WriteGroups(cWriteBlock &) const;
};

class cObject : public cFactory {
public:
    cObject(cBase *);
};

class gcRegionBase : public cObject {
public:
    gcRegionBase(cBase *);
    void Write(cFile &) const;
    void OnMemPoolReset(const cMemPool *, unsigned int);
    void MemCardReplicate(gcReplicationVisitor &);
};

struct TypeDispatchEntry {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern char gcRegionBaseclassdesc[];  // 0x387838
extern char cObjectclassdesc[];        // 0x37E9C0

extern void *g_regionSlots[2];   // 0x37D7D4
extern void *g_gcMap;             // 0x37D7FC

extern "C" {
    void *__vec_new(void *, int, int, void (*)(void *));
    void cObject___dtor_cObject_void(void *, int);
    void *cMemPool_GetPoolFromPtr(const void *);
    int cObject_WillBeDeleted(const void *, const cMemPool *, unsigned int);
    void gcStreamedCinematic_Delete(void *);
    void gcGameSettings_Get(void);
}

// ============================================================
// 0x000ef394 — MemCardReplicate(gcReplicationVisitor &)
// Compiler emits an explicit null guard on `this`, then dispatches via the
// classdesc[0xD0] TypeDispatchEntry (offset + fn).
// ============================================================
void gcRegionBase::MemCardReplicate(gcReplicationVisitor &v) {
    if (this != 0) {
        char *classdesc = *(char **)((char *)this + 4);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(classdesc + 0xD0);
        ((void (*)(void *, gcReplicationVisitor &))entry->fn)(
            (char *)this + entry->offset, v);
    }
}

// ============================================================
// 0x000ee0b8 — Write(cFile &) const
// ============================================================
void gcRegionBase::Write(cFile &file) const {
    cWriteBlock wb(file, 6);
    cFactory::Write(file);
    cFactory::WriteGroups(wb);
    ((cHandle *)((char *)this + 0x54))->Write(wb);
    ((cHandle *)((char *)this + 0x58))->Write(wb);
    ((cBaseArray *)((char *)this + 0x5C))->Write(wb);
    wb.End();
}

// ============================================================
// 0x000ee450 — gcRegionBase(cBase *)
// ============================================================
gcRegionBase::gcRegionBase(cBase *parent) : cObject(parent) {
    *(int *)((char *)this + 0x44) = 1;
    *(int *)((char *)this + 0x48) = 0x800;
    *(char **)((char *)this + 0x4) = gcRegionBaseclassdesc;
    *(int *)((char *)this + 0x4C) = 0;
    *(int *)((char *)this + 0x50) = 0;
    *(int *)((char *)this + 0x54) = 0;
    *(int *)((char *)this + 0x58) = 0;
    *(int *)((char *)this + 0x5C) = 0;
    *(void **)((char *)this + 0x60) = this;
    __vec_new((char *)this + 0x64, 3, 6, (void (*)(void *))0x24400C);
    *(void **)((char *)cMemPool_GetPoolFromPtr(this) + 0x28) = this;
}

// ============================================================
// 0x000ee4d8 — ~gcRegionBase
// ============================================================
extern "C" void gcRegionBase___dtor_gcRegionBase_void(gcRegionBase *self, int flags) {
    if (self != 0) {
        *(char **)((char *)self + 4) = gcRegionBaseclassdesc;
        void *arr = (char *)self + 0x5C;
        if (arr != 0) {
            ((cBaseArray *)arr)->RemoveAll();
        }
        *(char **)((char *)self + 4) = cObjectclassdesc;
        cObject___dtor_cObject_void(self, 0);
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}

// ============================================================
// 0x000ef2b4 — OnMemPoolReset
// Iterates a 2-slot pointer table at 0x37D7D4, and for each live entry
// whose cBase will be deleted, dispatches via g_gcMap's classdesc[0x58]
// then invokes gcStreamedCinematic::Delete on the entry's cBase.
// ============================================================
void gcRegionBase::OnMemPoolReset(const cMemPool *pool, unsigned int flags) {
    int i = 0;
    void **ptr = &g_regionSlots[0];
    do {
        void *p = *ptr;
        if (p != 0) {
            if (cObject_WillBeDeleted(*(void **)p, pool, flags)) {
                gcGameSettings_Get();
                void *map = g_gcMap;
                char *classdesc = *(char **)((char *)map + 4);
                TypeDispatchEntry *entry = (TypeDispatchEntry *)(classdesc + 0x58);
                void *base = (char *)map + entry->offset;
                cMemPool *poolResult = (cMemPool *)cMemPool_GetPoolFromPtr(p);
                ((void (*)(void *, const cMemPool *, int))entry->fn)(base, poolResult, 0);
                gcStreamedCinematic_Delete(*(void **)p);
            }
        }
        i++;
        ptr++;
    } while (i < 2);
}
