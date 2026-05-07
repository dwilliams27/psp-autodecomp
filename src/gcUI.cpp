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
class mCollideInfo;

typedef unsigned int SceULong128 __attribute__((mode(TI)));

extern int eSphereShape_collision_handler_guard asm("D_0000B0A8");
extern struct eSphereShape_CollisionHandlerStorage
    eSphereShape_collision_handler_storage asm("D_0000B0B8");

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
    void AssignCopy(const cBase *);
    void Write(cOutStream &) const;
    int Read(cFile &, cMemPool *);
    void DeleteSpawned(void);
    void GetMouseAvailable(void);
    gcUIDialog *GetFocusedDialog(void) const;
    static int GetActiveSpawnedDialogCount(const gcUIDialog *);
    void RemoveFromDestroyList(gcUIDialog *);
};

class eSphereShape {
public:
    char _pad[0x74];
    float mRadius;

    int GetCollisionHandler(const mCollideInfo &) const;
};

struct eSphereShape_CollisionHandlerStorage {
    char pad00[0x30];
    int field30;
    char pad34[0x8];
    void *vtable;
    char pad40[0xF0];
    char array130[0x4820];
    union {
        SceULong128 sphere;
        struct {
            char pad[0xC];
            float radius;
        };
    } world;
    union {
        SceULong128 sphere;
        struct {
            char pad[0xC];
            float radius;
        };
    } local;
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

struct gcUI_ActiveSpawnScan {
    char pad[0x54];
    void **active;
};

struct gcUI_TypeVtableEntry {
    short offset;
    short pad;
    const cType *(*fn)(const void *);
};

struct gcUI_CopyBlock {
    int field08;
    int field00;
    int field18;
    int field04;
    int field10;
    float field14;
    int field1C;
    float field0C;
    float field20;
};

extern cType *D_000385DC;
extern cType *D_00099924;

extern "C" void *__vec_new(void *, int, int, void *);

extern "C" void gcUI_gcFader_ctor(gcUI::gcFader *) asm("__0o5EgcUIHgcFaderctv");

extern "C" void __0oKcReadBlockctR6FcFileUib(void *rb, cFile &file,
                                             unsigned int id, bool validate);
extern "C" void __0oKcReadBlockdtv(void *rb, int flags);
extern "C" void cFile_SetCurrentPos(void *file, unsigned int pos);

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

// ── Read(cFile &, cMemPool *) ──  @ 0x000e03a4, 120B
int gcUI::Read(cFile &file, cMemPool *) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// ── AssignCopy ──  @ 0x0023d64c, 652B
void gcUI::AssignCopy(const cBase *base) {
    gcUI *other = 0;
    if (base != 0) {
        if (D_00099924 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType(
                    (const char *)0x36D894, (const char *)0x36D89C,
                    1, 0, 0, 0, 0, 0);
            }
            const cType *parentType = D_000385DC;
            D_00099924 = cType::InitializeType(0, 0, 0xED, parentType,
                                               (cBase *(*)(cMemPool *, cBase *))0x23D8D8,
                                               0, 0, 0);
                                               __asm__ volatile("" ::: "memory");
        }
        const gcUI_TypeVtableEntry *entry =
            (const gcUI_TypeVtableEntry *)(*(const char **)((const char *)base + 4) + 8);
        const cType *target = D_00099924;
        const cType *type = entry->fn((const char *)base + entry->offset);
        int isUI;
        if (target != 0) {
            goto have_target;
        }
        goto type_failed;
have_target:
        if (type != 0) {
            do {
                if (type == target) {
                    isUI = 1;
                    goto checked_type;
                }
                type = *(const cType *const *)((const char *)type + 0x1C);
            } while (type != 0);
        }
type_failed:
        isUI = 0;
checked_type:
        if (isUI != 0) {
            other = (gcUI *)base;
        }
    }

    const char *src = (const char *)other;
    char *dst = (char *)this;

    *(gcUI_CopyBlock *)(dst + 0x08) = *(const gcUI_CopyBlock *)(src + 0x08);
    *(gcUI_CopyBlock *)(dst + 0x2C) = *(const gcUI_CopyBlock *)(src + 0x2C);

    const int *srcDialogs = (const int *)src;
    int *dstDialogs = (int *)dst;
    __asm__ volatile("" ::: "memory");
    int i = 0;
    ((int *)dst)[0x50 / 4] = ((const int *)src)[0x50 / 4];
    do {
        i += 1;
        dstDialogs[0x54 / 4] = srcDialogs[0x54 / 4];
        srcDialogs += 1;
        dstDialogs += 1;
    } while (i < 0x20);

    ((int *)dst)[0xD4 / 4] = ((const int *)src)[0xD4 / 4];
    __asm__ volatile("" ::: "memory");
    int _tmp_100 = ((const int *)src)[0xD8 / 4];
    int j = 0;
    ((int *)dst)[0xD8 / 4] = _tmp_100;
    int *dstDestroy = (int *)dst;
    const int *srcDestroy = (const int *)other;
    do {
        j += 1;
        dstDestroy[0xDC / 4] = srcDestroy[0xDC / 4];
        srcDestroy += 1;
        dstDestroy += 1;
    } while (j < 0x20);

    ((float *)dst)[0x164 / 4] = ((const float *)src)[0x164 / 4];
    ((int *)dst)[0x15C / 4] = ((const int *)src)[0x15C / 4];
    __asm__ volatile("" ::: "memory");
    ((float *)dst)[0x160 / 4] = ((const float *)src)[0x160 / 4];

    const char *srcFaders = (const char *)other + 0x168;
    dst += 0x168;
    int k = 0;
    do {
        srcFaders += 8;
        k += 1;
        *(float *)(dst + 4) = *(const float *)(srcFaders + 4);
        *(float *)dst = *(const float *)srcFaders;
        dst += 8;
    } while ((unsigned int)k < 4U);
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

// ── GetMouseAvailable ──  @ 0x000e1cec, 164B
void gcUI::GetMouseAvailable(void) {
    *(unsigned int *)((char *)this + 0x50) &= 0x7FFFFFFF;
    __asm__ volatile("" ::: "memory");

    register unsigned char *p __asm__("a1") = (unsigned char *)0x45338;
    if (*(unsigned char *)(p + 0x7C) != 0) {
        if (*(unsigned char *)(p + 0x7F) == 0) {
            *(unsigned int *)((char *)this + 0x50) |= 0x80000000;
            return;
        }
    }

    register int available __asm__("a2");
    register int masked __asm__("a2");
    register int i __asm__("a3") = 0;
    p = (unsigned char *)0x41118;
    do {
        available = 0;
        if (*(unsigned char *)(p + 0x814) != 0) {
            masked = available & 0xFF;
            if (*(unsigned char *)(p + 0x821) != 0) {
                available = 1;
                goto mouse_available_done;
            }
        } else {
mouse_available_done:
            masked = available & 0xFF;
        }

        i++;
        if (masked != 0) {
            *(unsigned int *)((char *)this + 0x50) |= 0x80000000;
            return;
        }
        p += 0x844;
    } while (i < 4);
}

// ── GetFocusedDialog ──  @ 0x000e0a6c, 136B
gcUIDialog *gcUI::GetFocusedDialog(void) const {
    int top = *(int *)((char *)this + 0xD4);
    if (top >= 0) {
        int i = top;
        if (i < 0) {
            return 0;
        }
        gcUIDialog **base = (gcUIDialog **)((char *)this + 0x54);
        gcUIDialog **slot = base + top;
        unsigned int focusMask = 0x10000;
        do {
            gcUIDialog *dialog = *slot;
            if (dialog != 0) {
                int active = 0;
                int hidden = (unsigned char)((*(unsigned int *)((char *)dialog + 0x50) & focusMask) != 0);
                if (hidden == 0) {
                    int blocked = (unsigned char)((*(unsigned int *)((char *)dialog + 0x54) & 0x100) != 0);
                    if (blocked == 0) {
                        active = 1;
                    }
                }
                if ((unsigned char)active != 0) {
                    return dialog;
                }
            }
            i--;
            slot--;
        } while (i >= 0);
    }
    return 0;
}

// ── GetActiveSpawnedDialogCount ──  @ 0x000e0b80, 108B
int gcUI::GetActiveSpawnedDialogCount(const gcUIDialog *dialog) {
    gcUI *ui = *(gcUI **)0x37D7BC;
    if (ui == 0) {
        return 0;
    }
    if (dialog == 0) {
        return 0;
    }
    int i = *(int *)((char *)ui + 0xD4);
    int count = 0;
    if (i >= 0) {
        gcUI_ActiveSpawnScan *slot = (gcUI_ActiveSpawnScan *)((char *)ui + i * 4);
        do {
            void **active = slot->active;
            if ((active != 0) && (*active == dialog)) {
                count++;
            }
            i--;
            slot = (gcUI_ActiveSpawnScan *)((char *)slot - 4);
        } while (i >= 0);
    }
    return count;
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

// ── eSphereShape::GetCollisionHandler ──  @ 0x00067cdc, 172B
#pragma control sched=1
int eSphereShape::GetCollisionHandler(const mCollideInfo &info) const {
    eSphereShape_CollisionHandlerStorage *handler =
        &eSphereShape_collision_handler_storage;

    if (eSphereShape_collision_handler_guard == 0) {
        eSphereShape_collision_handler_guard = 1;
        handler->field30 = 0;
        handler->vtable = (void *)0x382CE0;
        char *array = handler->array130;
        __asm__ volatile("" : "+r"(array));
        void *ctor = (void *)0x201BC8;
        __asm__ volatile("" : "+r"(ctor));
        __vec_new(array, 0x80, 0x90, ctor);
        handler->vtable = (void *)0x382D08;
    }

    float radius = mRadius;
    volatile SceULong128 sphere;
    __asm__ volatile(
        "lv.q C120, 0(%0)\n"
        "sv.q C120, 0($sp)\n"
        :
        : "r"(&info)
        : "memory"
    );
    *(volatile float *)((char *)&sphere + 0xC) = radius;
    __asm__ volatile(
        "lv.q C120, 0($sp)\n"
        "sv.q C120, 0x4950(%0)\n"
        :
        : "r"(handler)
        : "memory"
    );
    handler->world.radius = radius;
    __asm__ volatile(
        "sv.q C120, 0x4960(%0)\n"
        :
        : "r"(handler)
        : "memory"
    );
    handler->local.radius = radius;
    return (int)handler;
}
#pragma control sched=2

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
