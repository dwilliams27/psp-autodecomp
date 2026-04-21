#include "gcCinematic.h"

#define NULL 0

class cBase;
class cFile;

class cObject {
public:
    cObject &operator=(const cObject &);
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(unsigned int);
    void End(void);
};

class cBaseArray {
public:
    cBaseArray &operator=(const cBaseArray &);
    void Write(cWriteBlock &) const;
};

class gcEvent {
public:
    gcEvent &operator=(const gcEvent &);
};

// C-style wrappers for external ctors (avoid placement-new null checks).
void cObject_ctor(void *, cBase *);
void gcEvent_ctor(void *, cBase *, const char *);
void gcCinematic_ctor(gcCinematic *, cBase *);
void cHandle_Write(const cHandle *, cWriteBlock &);

extern char gcCinematicvirtualtable[];
extern const char gcEventName_Cinematic[];

gcCinematic *dcast(const cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

// Previously matched stubs kept.
void gcCinematic::Reset(cMemPool *, bool) {
}

void gcCinematicInstance::Reset(cMemPool *, bool) {
}

void gcCinematicInstance::UpdateProfile(void) {
}

void gcReplicationVisitor::SetNetConnection(int connection) {
    mNetConnection = connection;
}

void gcReplicationVisitor::SetMemCardStream(cInStream *stream) {
    mInStream = stream;
}

// ─────────────────────────────────────────────────────
// 0x000eaac4 (64B) — gcCinematic::FreeDynamicInstance
// ─────────────────────────────────────────────────────
void gcCinematic::FreeDynamicInstance(gcCinematicInstance *inst) {
    if (inst != NULL) {
        int *vt = (int *)(((char **)inst)[1] + 0x50);
        short thunk = *(short *)vt;
        ((void (*)(char *, int))vt[1])((char *)inst + thunk, 3);
    }
}

// ─────────────────────────────────────────────────────
// 0x000ea888 (112B) — gcCinematic::gcCinematic(cBase *)
// ─────────────────────────────────────────────────────
gcCinematic::gcCinematic(cBase *parent) {
    cObject_ctor(this, parent);
    ((void **)this)[1] = gcCinematicvirtualtable;
    ((int *)this)[0x44 / 4] = 0;
    ((int *)this)[0x48 / 4] = (int)this;
    ((unsigned char *)this)[0x4C] = 0;
    ((unsigned char *)this)[0x4D] = 0;
    ((unsigned char *)this)[0x4E] = 0;
    ((int *)this)[0x50 / 4] = 0;
    ((int *)this)[0x54 / 4] = 0;
    ((int *)this)[0x58 / 4] = 0;
    ((int *)this)[0x5C / 4] = 0;
    gcEvent_ctor((char *)this + 0x60, (cBase *)this, gcEventName_Cinematic);
    ((int *)this)[0x7C / 4] = 0;
}

// ─────────────────────────────────────────────────────
// 0x00242ddc (124B) — gcCinematic::New
// ─────────────────────────────────────────────────────
cBase *gcCinematic::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcCinematic *result = 0;
    gcCinematic *obj = (gcCinematic *)entry->fn(base, 0x80, 4, 0, 0);
    if (obj != 0) {
        gcCinematic_ctor(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ─────────────────────────────────────────────────────
// 0x00242d24 (184B) — gcCinematic::AssignCopy
// ─────────────────────────────────────────────────────
void gcCinematic::AssignCopy(const cBase *base) {
    gcCinematic *other = dcast(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    ((cBaseArray *)((char *)this + 0x44))->operator=(*(const cBaseArray *)((char *)other + 0x44));
    ((unsigned char *)this)[0x4C] = ((const unsigned char *)other)[0x4C];
    ((unsigned char *)this)[0x4D] = ((const unsigned char *)other)[0x4D];
    ((unsigned char *)this)[0x4E] = ((const unsigned char *)other)[0x4E];
    ((cHandle *)((char *)this + 0x50))->operator=(*(const cHandle *)((const char *)other + 0x50));
    ((cHandle *)((char *)this + 0x54))->operator=(*(const cHandle *)((const char *)other + 0x54));
    ((cHandle *)((char *)this + 0x58))->operator=(*(const cHandle *)((const char *)other + 0x58));
    ((cHandle *)((char *)this + 0x5C))->operator=(*(const cHandle *)((const char *)other + 0x5C));
    ((gcEvent *)((char *)this + 0x60))->operator=(*(const gcEvent *)((char *)other + 0x60));
    ((int *)this)[0x7C / 4] = ((const int *)other)[0x7C / 4];
}

// ─────────────────────────────────────────────────────
// 0x000ea520 (204B) — gcCinematic::Write
// ─────────────────────────────────────────────────────
struct gcEventVTableEntry {
    short this_adjust;
    short pad;
    void (*fn)(void *, cFile *);
};

void gcCinematic::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    ((const cObject *)this)->Write(file);
    ((const cBaseArray *)((char *)this + 0x44))->Write(wb);
    wb.Write(((const bool *)this)[0x4C]);
    wb.Write(((const bool *)this)[0x4D]);
    wb.Write(((const bool *)this)[0x4E]);
    cHandle_Write((const cHandle *)((char *)this + 0x50), wb);
    wb.Write((unsigned int)((const int *)this)[0x54 / 4]);
    cHandle_Write((const cHandle *)((char *)this + 0x58), wb);
    wb.Write((unsigned int)((const int *)this)[0x5C / 4]);
    gcEventVTableEntry *entry =
        (gcEventVTableEntry *)(*(char **)((char *)this + 0x64) + 0x28);
    ((void (*)(void *, cFile *))entry->fn)(
        (char *)this + 0x60 + entry->this_adjust, *(cFile **)&wb);
    wb.End();
}
