// gcStreamedCinematic — streaming cinematic playback state

#define NULL 0

class cFile;
class cMemPool;
class cBase;
class cReadBlock;
class cType;

template <class T> T *dcast(const cBase *);

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cObject {
public:
    cObject &operator=(const cObject &);
};

class cBaseArray {
public:
    cBaseArray &operator=(const cBaseArray &);
};

class gcEvent {
public:
    gcEvent &operator=(const gcEvent &);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFactory {
public:
    void Write(cFile &) const;
    void WriteGroups(cWriteBlock &) const;
    int Read(cFile &, cMemPool *);
    void ReadGroups(cReadBlock &, cMemPool *);
    void DeleteGroups(void);
};

class gcEntity {
public:
    static void DestroyAll(const cMemPool *);
};

namespace cMemPoolNS {
    const cMemPool *GetPoolFromPtr(const void *);
}

void *cMemPool_GetPoolFromPtr(void *);
void cFile_SetCurrentPos(void *, unsigned int);
void cObject___dtor_cObject_void(void *, int);
void gcCinematic___dtor_gcCinematic_void(void *, int);
int gcCinematic_Read(void *, cFile &, cMemPool *);

class gcStreamedCinematic {
public:
    gcStreamedCinematic(cBase *, bool);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void RemoveFromWorld(void);
    static unsigned char IsFullscreenInProgress(void);
    static unsigned char HasFullscreenStarted(void);
    static unsigned char IsFullscreenPlaying(void);
    static cBase *New(cMemPool *, cBase *);
    static void Delete(cBase *);
    static int HandleDeletion(void);
    int *GetGroups(int *);
};

extern char gcStreamedCinematicvirtualtable[];               // 0x387688
extern char cObjectvirtualtable_for_gcStreamedCinematic[];   // 0x37E9C0
extern int gcStreamedCinematic_currentIndex;                 // 0x37D7D0
extern gcStreamedCinematic *gcStreamedCinematic_table[];     // 0x37D7D4
extern void *gcStreamedCinematic_root;                       // 0x37D7FC
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040C90;
extern cType *D_00099AD4;

inline void *operator new(unsigned int, void *p) { return p; }

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

struct VCall3 {
    short offset;
    short pad;
    void (*fn)(void *, void *, int);
};

// ──────────────────────────────────────────────────────
// 0x000ed660 (36B) — RemoveFromWorld
// ──────────────────────────────────────────────────────
void gcStreamedCinematic::RemoveFromWorld(void) {
    gcEntity::DestroyAll(cMemPoolNS::GetPoolFromPtr(this));
}

// ──────────────────────────────────────────────────────
// 0x000ed254 (72B) — IsFullscreenInProgress (static)
// ──────────────────────────────────────────────────────
unsigned char gcStreamedCinematic::IsFullscreenInProgress(void) {
    int *g = *(int **)0x37D7FC;
    int result = 0;
    if (g != NULL) {
        int inner = 0;
        int f = g[0x140 / 4];
        if (f & 0x4000) {
            if (f & 0x2000) {
                inner = 1;
            }
        }
        if ((unsigned char)inner) {
            result = 1;
        }
    }
    return (unsigned char)result;
}

// ──────────────────────────────────────────────────────
// 0x000ed29c (72B) — HasFullscreenStarted (static)
// ──────────────────────────────────────────────────────
unsigned char gcStreamedCinematic::HasFullscreenStarted(void) {
    int *g = *(int **)0x37D7FC;
    int result = 0;
    if (g != NULL) {
        int inner = 0;
        int f = g[0x140 / 4];
        if (f & 0x4000) {
            if (f & 0x8000) {
                inner = 1;
            }
        }
        if ((unsigned char)inner) {
            result = 1;
        }
    }
    return (unsigned char)result;
}

// ──────────────────────────────────────────────────────
// 0x000ed1fc (88B) — IsFullscreenPlaying (static)
// ──────────────────────────────────────────────────────
unsigned char gcStreamedCinematic::IsFullscreenPlaying(void) {
    int *g = *(int **)0x37D7FC;
    int result = 0;
    if (g != NULL) {
        int inner = 0;
        int f = g[0x140 / 4];
        if (((f & 0x4000) && (f & 0x1000)) || (f & 0x200000)) {
            inner = 1;
        }
        if ((unsigned char)inner) {
            result = 1;
        }
    }
    return (unsigned char)result;
}

// ──────────────────────────────────────────────────────
// 0x000ecfd4 (120B) — Write(cFile &) const
// ──────────────────────────────────────────────────────
struct gcStreamedCinematicVTEntry {
    short this_adjust;
    short pad;
    void (*fn)(void *, cFile *);
};

void gcStreamedCinematic::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const cFactory *)this)->Write(file);
    ((const cFactory *)this)->WriteGroups(wb);
    gcStreamedCinematicVTEntry *entry =
        (gcStreamedCinematicVTEntry *)(*(char **)((char *)this + 0x50) + 0x28);
    char *base = (char *)this + 0x4C;
    ((void (*)(void *, cFile *))entry->fn)(base + entry->this_adjust, &file);
    wb.End();
}

// ──────────────────────────────────────────────────────
// 0x0024436c (16B) — GetGroups(int *)
// ──────────────────────────────────────────────────────
int *gcStreamedCinematic::GetGroups(int *count) {
    *count = 0x1B;
    return (int *)((char *)this + 0xD4);
}

// ──────────────────────────────────────────────────────
// 0x00244064 (292B) — AssignCopy(const cBase *)
// ──────────────────────────────────────────────────────
void gcStreamedCinematic::AssignCopy(const cBase *base) {
    gcStreamedCinematic *other = dcast<gcStreamedCinematic>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(int *)((char *)this + 0x44) = *(int *)((char *)other + 0x44);
    *(int *)((char *)this + 0x48) = *(int *)((char *)other + 0x48);
    ((cObject *)((char *)this + 0x4C))->operator=(
        *(const cObject *)((char *)other + 0x4C));
    ((cBaseArray *)((char *)this + 0x90))->operator=(
        *(const cBaseArray *)((char *)other + 0x90));
    *(unsigned char *)((char *)this + 0x98) =
        *(unsigned char *)((char *)other + 0x98);
    *(unsigned char *)((char *)this + 0x99) =
        *(unsigned char *)((char *)other + 0x99);
    *(unsigned char *)((char *)this + 0x9A) =
        *(unsigned char *)((char *)other + 0x9A);

    char *var_a1 = (char *)other + 0x9C;
    int temp_a0 = *(int *)var_a1;
    char *var_a2 = (char *)this + 0x9C;
    var_a1 = (char *)other + 0xA0;
    char *var_a0 = (char *)this + 0xA0;
    *(int *)var_a2 = temp_a0;
    int temp_a1 = *(int *)var_a1;
    var_a2 = (char *)other + 0xA4;
    *(int *)var_a0 = temp_a1;
    temp_a1 = *(int *)var_a2;
    var_a0 = (char *)this + 0xA4;
    var_a2 = (char *)other + 0xA8;
    var_a1 = (char *)this + 0xA8;
    *(int *)var_a0 = temp_a1;
    int temp_a2 = *(int *)var_a2;
    *(int *)var_a1 = temp_a2;

    ((gcEvent *)((char *)this + 0xAC))->operator=(
        *(const gcEvent *)((char *)other + 0xAC));
    *(int *)((char *)this + 0xC8) = *(int *)((char *)other + 0xC8);
    *(int *)((char *)this + 0xCC) = *(int *)((char *)other + 0xCC);
    *(unsigned char *)((char *)this + 0xD0) =
        *(unsigned char *)((char *)other + 0xD0);

    int var_i = 0;
    void *src_loop = other;
    void *dst_loop = this;
    do {
        int temp_a3 = *(int *)((char *)src_loop + 0xD4);
        var_i += 1;
        *(int *)((char *)dst_loop + 0xD4) = temp_a3;
        src_loop = (char *)src_loop + 4;
    } while ((dst_loop = (char *)dst_loop + 4, var_i < 0x1B));

    *(unsigned char *)((char *)this + 0x140) =
        *(unsigned char *)((char *)other + 0x140);
    *(unsigned char *)((char *)this + 0x141) =
        *(unsigned char *)((char *)other + 0x141);
    *(int *)((char *)this + 0x144) = *(int *)((char *)other + 0x144);
}

// ──────────────────────────────────────────────────────
// 0x00244208 (356B) — GetType(void) const
// ──────────────────────────────────────────────────────
const cType *gcStreamedCinematic::GetType(void) const {
    if (D_00099AD4 == 0) {
        if (D_00040C90 == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                           (const char *)0x36D89C,
                                                           1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                       &cNamed::New, 0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                                   0, 0, 0, 0);
            }
            D_00040C90 = cType::InitializeType(0, 0, 5, D_000385E4,
                                               0, 0, 0, 0);
        }
        D_00099AD4 = cType::InitializeType(0, 0, 0x16B, D_00040C90,
                                           &gcStreamedCinematic::New,
                                           (const char *)0x36D8F0,
                                           (const char *)0x36D8FC, 1);
    }
    return D_00099AD4;
}

// ──────────────────────────────────────────────────────
// 0x000ed04c (252B) — Read(cFile &, cMemPool *)
// ──────────────────────────────────────────────────────
int gcStreamedCinematic::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    cReadBlock rb(file, 2, true);
    if ((unsigned int)rb._data[3] >= 3 || (unsigned int)rb._data[3] < 1)
        goto fail;
    if (!((cFactory *)this)->Read(file, pool))
        goto fail;
    if ((unsigned int)rb._data[3] >= 2)
        goto do_groups;
    goto after_groups;
fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
do_groups:
    ((cFactory *)this)->ReadGroups(rb, pool);
after_groups:
    gcCinematic_Read((char *)this + 0x4C, file, pool);
    return result;
}

// ──────────────────────────────────────────────────────
// 0x00244188 (128B) — New(cMemPool *, cBase *) static
// ──────────────────────────────────────────────────────
cBase *gcStreamedCinematic::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcStreamedCinematic *result = 0;
    gcStreamedCinematic *obj =
        (gcStreamedCinematic *)entry->fn(base, 0x148, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcStreamedCinematic(parent, false);
        result = obj;
    }
    return (cBase *)result;
}

// ──────────────────────────────────────────────────────
// 0x000ed41c (184B) — HandleDeletion(void) static
// ──────────────────────────────────────────────────────
int gcStreamedCinematic::HandleDeletion(void) {
    register void *self __asm__("a0") =
        gcStreamedCinematic_table[gcStreamedCinematic_currentIndex];
    if (self == 0) goto ret0;
    {
        unsigned int flag = *(unsigned char *)((char *)self + 0x140);
        if (flag != 0) goto remove_path;
    }
    {
        unsigned int flag = *(unsigned char *)((char *)self + 0x141);
        if (flag == 0) goto ret0;
    }
    {
        void *root = gcStreamedCinematic_root;
        register VCall3 *entry __asm__("s0") =
            (VCall3 *)(*(char **)((char *)root + 4) + 0x58);
        short off = entry->offset;
        void *base = (char *)root + off;
        void *pool = cMemPool_GetPoolFromPtr(self);
        entry->fn(base, pool, 0);
        gcStreamedCinematic::Delete(0);
        return 1;
    }
remove_path:
    *(unsigned char *)((char *)self + 0x140) = 0;
    ((gcStreamedCinematic *)self)->RemoveFromWorld();
ret0:
    return 0;
}

// ──────────────────────────────────────────────────────
// 0x000ed148 (180B) — ~gcStreamedCinematic(void)
// ──────────────────────────────────────────────────────
extern "C" {

void gcStreamedCinematic___dtor_gcStreamedCinematic_void(
        gcStreamedCinematic *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = gcStreamedCinematicvirtualtable;
        ((cFactory *)self)->DeleteGroups();
        gcStreamedCinematic_table[*(int *)((char *)self + 0x144)] = 0;
        gcCinematic___dtor_gcCinematic_void((char *)self + 0x4C, 2);
        *(void **)((char *)self + 4) = cObjectvirtualtable_for_gcStreamedCinematic;
        cObject___dtor_cObject_void(self, 0);
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn((char *)block + off, self);
        }
    }
}

}
