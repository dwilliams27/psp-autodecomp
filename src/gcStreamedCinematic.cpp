// gcStreamedCinematic — streaming cinematic playback state

#define NULL 0

class cFile;
class cMemPool;
class cBase;
class cReadBlock;

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
