// eKeyframedControllerConfig — 32-byte config object that extends
// ePhysicsControllerConfig (which itself adds only vtable wiring on top of
// cBase). The config's own 24 bytes of data live at offset 0x08..0x20 and are
// what AssignCopy splat-copies from a sibling instance.
//
// Functions matched here:
//   eKeyframedControllerConfig::Write(cFile &) const                @ 0x00075558  (eAll_psp.obj, sched=1)
//   eKeyframedControllerConfig::AssignCopy(const cBase *)           @ 0x0020dfb0  (eAll_psp.obj, sched=2)
//   eKeyframedControllerConfig::~eKeyframedControllerConfig(void)   @ 0x00075694  (eAll_psp.obj, sched=1)
//
// The destructor follows the canonical deleting-destructor shape used
// throughout the engine (see src/eKeyframedController_dtor.cpp,
// src/eSphereShape_dtor.cpp): SNC's ABI auto-generates the (this != 0) guard,
// the chain call to ~ePhysicsControllerConfig(this, 0), and the deleting tail
// dispatch via operator delete (inlined here).

class cBase;
class cFile;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class ePhysicsControllerConfig {
public:
    char _pad[0x8];
    ePhysicsControllerConfig(cBase *);
    ~ePhysicsControllerConfig();
    void Write(cFile &) const;
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct eKCCData { int _w[6]; };

class eKeyframedControllerConfig : public ePhysicsControllerConfig {
public:
    eKCCData mData;  // offset 0x08..0x20

    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    ~eKeyframedControllerConfig();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

eKeyframedControllerConfig *dcast(const cBase *);

// ── Write ──  @ 0x00075558, 76B
#pragma control sched=1
void eKeyframedControllerConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const ePhysicsControllerConfig *)this)->Write(file);
    wb.End();
}
#pragma control sched=2

// ── AssignCopy ──  @ 0x0020dfb0, 96B
//
// dcast()s the source cBase* to the concrete type, then splat-copies the
// 24-byte config payload at offset +8. SNC schedules the copy as two
// 3-load + 3-store batches (a2/a3/t0 → a2/a3/a0).
#pragma control sched=1
void eKeyframedControllerConfig::AssignCopy(const cBase *base) {
    eKeyframedControllerConfig *other = dcast(base);
    *(eKCCData *)((char *)this + 8) = *(eKCCData *)((char *)other + 8);
}
#pragma control sched=2

// ── Destructor ──  @ 0x00075694, 124B
#pragma control sched=1
eKeyframedControllerConfig::~eKeyframedControllerConfig() {
    *(void **)((char *)this + 4) = (void *)0x383DA8;
}
#pragma control sched=2
