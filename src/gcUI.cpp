// gcUI — UI manager. Functions matched here:
//   gcUI::~gcUI(void)                          @ 0x000e04bc  (gcAll_psp.obj)
//   gcUI::New(cMemPool *, cBase *) static      @ 0x0023d8d8  (gcAll_psp.obj)
//   gcUI::GetType(void) const                  @ 0x0023d958  (gcAll_psp.obj)

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;
class cMemPool;
class cType;
class cBase;
class cOutStream;
class gcUIDialog;

extern "C" void *memset(void *, int, unsigned int);

extern "C" void *cMemPool_GetPoolFromPtr(const void *);

extern char cBaseclassdesc[];                  // 0x37E6A8
extern char gcUIclassdesc[];                   // 0x387168

extern const char gcUI_base_name[];            // 0x36D894
extern const char gcUI_base_desc[];            // 0x36D89C

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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcUI {
public:
    class gcFader {
    public:
        gcFader(void);
    };

    gcUI(cBase *, unsigned int);
    ~gcUI();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cOutStream &) const;
    void DeleteSpawned(void);
    void RemoveFromDestroyList(gcUIDialog *);
};

class cOutStream {
public:
    unsigned char *mData;
    int mCapacity;
    int mBitPos;
    int mCRC;
    unsigned char mDirty;
    char _pad11;
    unsigned char mOverflow;
    char _pad13;
    int mCRCBitPos;
};

class cHandle {
public:
    unsigned int mKey;
    void Write(cOutStream &) const;
};

class gcUIDialog {
public:
    static void FreeDynamicInstance(gcUIDialog *);
    void Write(cOutStream &) const;
};

struct gcUI_DestroySlotScan {
    char pad[0xDC];
    gcUIDialog *slot;
};

extern "C" void gcUI_gcFader_ctor(gcUI::gcFader *) asm("__0o5EgcUIHgcFaderctv");

// ── Constructor ──  @ 0x000e041c, 160B
gcUI::gcUI(cBase *parent, unsigned int flags) {
    *(cBase **)((char *)this + 0) = parent;
    *(char **)((char *)this + 4) = gcUIclassdesc;
    gcUI_gcFader_ctor((gcFader *)((char *)this + 8));
    gcUI_gcFader_ctor((gcFader *)((char *)this + 0x2C));
    *(unsigned int *)((char *)this + 0x50) = flags;
    *(int *)((char *)this + 0xD4) = -1;
    *(int *)((char *)this + 0xD8) = 0;
    *(int *)((char *)this + 0x15C) = 0;
    *(float *)((char *)this + 0x160) = 0.0f;
    *(float *)((char *)this + 0x164) = 0.0f;
    memset((char *)this + 0x54, 0, 0x80);
    memset((char *)this + 0xDC, 0, 0x80);
    memset((char *)this + 0x168, 0, 0x20);
}

// ── Write(cOutStream &) ──  @ 0x000e1080, 412B
void gcUI::Write(cOutStream &out) const {
    int temp_a2 = out.mBitPos;
    int var_s1 = 0;
    gcUIDialog **var_s2 = (gcUIDialog **)((char *)this + 0x54);
    int var_a0 = temp_a2 & 7;
    __asm__ volatile("" ::: "memory");
    unsigned char *var_a1 = out.mData + (temp_a2 >> 3);
    int var_a2 = temp_a2 + 1;
    __asm__ volatile("" ::: "memory");
    gcUIDialog *temp_s3 = 0;
    int one1 = 1;
    int one0 = 1;

    do {
        temp_s3 = *var_s2;
        if (temp_s3 != 0) {
            int var_a3 = (unsigned char)(((*(int *)((char *)temp_s3 + 0x50) & 0x10) == 0));
            if (var_a3 != 0) {
                __asm__ volatile("" ::: "memory");
                var_a3 = out.mOverflow;
                out.mBitPos = var_a2;
                if (var_a3 == 0) {
                    if (out.mCapacity < ((out.mBitPos + 7) >> 3)) {
                        out.mOverflow = one0;
                    }
                }
                int var_a2_2 = (unsigned char)(out.mOverflow == 0);
                if (var_a2_2 != 0) {
                    var_a2_2 = *var_a1;
                    var_a3 = ~(one1 << var_a0);
                    var_a2_2 = var_a2_2 & var_a3;
                    int var_a0_2 = var_a0;
                    *var_a1 = var_a2_2 | (one1 << var_a0_2);
                }

                cHandle handle;
                handle.mKey = *(unsigned int *)((char *)temp_s3 + 0x30);
                handle.Write(out);
                temp_s3->Write(out);

                temp_a2 = out.mBitPos;
                var_a0 = temp_a2 & 7;
                var_a1 = out.mData + (temp_a2 >> 3);
                var_a2 = temp_a2 + 1;
            }
        }
        var_s1 += 1;
        var_s2 += 1;
    } while (var_s1 < 0x20);

    out.mBitPos = var_a2;
    unsigned char temp_a2_3 = out.mOverflow;
    int var_a2_3;
    if (temp_a2_3 == 0) {
        var_a2_3 = (temp_a2_3 == 0);
        if (out.mCapacity < ((out.mBitPos + 7) >> 3)) {
            out.mOverflow = one0;
            goto block_13;
        }
    } else {
block_13:
        var_a2_3 = (out.mOverflow == 0);
    }
    if ((unsigned char)var_a2_3 != 0) {
        int zero = 0;
        int var_a2_4 = *var_a1;
        int var_a3 = ~(one1 << var_a0);
        var_a2_4 = var_a2_4 & var_a3;
        __asm__ volatile("" ::: "memory");
        *var_a1 = var_a2_4 | (zero << var_a0);
    }
}

// ── DeleteSpawned ──  @ 0x000e0a08, 100B
void gcUI::DeleteSpawned(void) {
    int i = 0;
    if (i < *(int *)((char *)this + 0x15C)) {
        gcUI_DestroySlotScan *scan = (gcUI_DestroySlotScan *)this;
        do {
            gcUIDialog::FreeDynamicInstance(scan->slot);
            i++;
            scan = (gcUI_DestroySlotScan *)((char *)scan + 4);
        } while (i < *(int *)((char *)this + 0x15C));
    }
    *(int *)((char *)this + 0x15C) = 0;
}

// ── RemoveFromDestroyList ──  @ 0x000e177c, 96B
#pragma control sched=1
void gcUI::RemoveFromDestroyList(gcUIDialog *dialog) {
    int i = 0;
    int count = *(int *)((char *)this + 0x15C);

    if (i < count) {
        gcUI_DestroySlotScan *scan = (gcUI_DestroySlotScan *)this;
        do {
            if (scan->slot == dialog) {
                count--;
                gcUI_DestroySlotScan *lastScan =
                    (gcUI_DestroySlotScan *)((char *)this + count * 4);
                *(int *)((char *)this + 0x15C) = count;
                scan->slot = lastScan->slot;
                *(int *)((char *)dialog + 0x54) &= ~8;
                return;
            }
            i++;
            scan = (gcUI_DestroySlotScan *)((char *)scan + 4);
        } while (i < count);
    }
}
#pragma control sched=2

// ── New ──  @ 0x0023d8d8, 128B
cBase *gcUI::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcUI *result = 0;
    gcUI *obj = (gcUI *)entry->fn(base, 0x188, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcUI(parent, 0);
        result = obj;
    }
    return (cBase *)result;
}

// ── GetType ──  @ 0x0023d958, 160B
static cType *type_base;
static cType *type_gcUI;

const cType *gcUI::GetType(void) const {
    if (!type_gcUI) {
        if (!type_base) {
            type_base = cType::InitializeType(gcUI_base_name, gcUI_base_desc, 1,
                                              0, 0, 0, 0, 0);
        }
        type_gcUI = cType::InitializeType(0, 0, 0xED, type_base, &gcUI::New,
                                          0, 0, 0);
    }
    return type_gcUI;
}

// ── Destructor ──  @ 0x000e04bc, 100B
extern "C" {

void gcUI___dtor_gcUI_void(gcUI *self, int flags) {
    if (self != 0) {
        *(char **)((char *)self + 4) = cBaseclassdesc;
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
