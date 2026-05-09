// Decompiled functions from gcAll_psp.obj
//   0x000d75a4 gcMessageHandler::gcMessageHandler(cBase *)
//   0x0023bb7c gcMessageHandler::GetName(char *) const
//   0x0023ba60 gcMessageHandler::New(cMemPool *, cBase *) static
//   0x0023badc gcMessageHandler::GetType(void) const
//   0x000d75d4 gcMessageHandler::~gcMessageHandler(void)

#include "cBase.h"

class cFile;
class cFileHandle;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cHandlePairT_gE_cSe {
public:
    const char *GetName(char *, bool, char *) const;
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class cHandle {
public:
    void Read(cReadBlock &, cMemPool *);
};

struct cSubHandle {
    int mEncoded;
};

class gcMessageHandler {
public:
    cBase *mOwner;       // 0x00
    void *mVTable;       // 0x04
    int mPair[4];        // 0x08..0x14 (cHandlePairT-like subobject)
    cSubHandle mSub;     // 0x18 (encoded handle: (this|1) means self-owned)

    gcMessageHandler(cBase *);
    void GetName(char *) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
};

extern char gcMessageHandlervirtualtable[];
extern char gcMessageHandlerbasevtable[];
extern const char gcMessageHandler_base_name[];
extern const char gcMessageHandler_base_desc[];

static cType *type_base;
static cType *type_gcMessageHandler;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecordSub {
    short offset;
    short _pad;
    void (*fn)(void *, int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

void gcMessageHandler_ctor(gcMessageHandler *, cBase *);
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);
extern void *D_00038890[];

// ============================================================
// 0x000d75a4 — constructor
// ============================================================
gcMessageHandler::gcMessageHandler(cBase *owner) {
    mOwner = owner;
    mVTable = gcMessageHandlervirtualtable;
    mPair[0] = 0;
    mPair[1] = 0;
    mPair[2] = 0;
    mPair[3] = 0;
    mSub.mEncoded = (int)this | 1;
}

// ============================================================
// 0x0023bb7c — GetName forwards to cHandlePairT subobject at offset 8
// ============================================================
void gcMessageHandler::GetName(char *out) const {
    ((cHandlePairT_gE_cSe *)((char *)this + 8))->GetName(out, false, (char *)1);
}

// ============================================================
// 0x0023ba60 — New(cMemPool *, cBase *) static
// ============================================================
cBase *gcMessageHandler::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcMessageHandler *result = 0;
    gcMessageHandler *obj = (gcMessageHandler *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        gcMessageHandler_ctor(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x0023badc — GetType
// ============================================================
const cType *gcMessageHandler::GetType(void) const {
    if (!type_gcMessageHandler) {
        if (!type_base) {
            type_base = cType::InitializeType(gcMessageHandler_base_name,
                                              gcMessageHandler_base_desc,
                                              1, 0, 0, 0, 0, 0);
        }
        type_gcMessageHandler = cType::InitializeType(0, 0, 0x116, type_base,
                                                      gcMessageHandler::New, 0, 0, 0);
    }
    return type_gcMessageHandler;
}

// ============================================================
// 0x000d75d4 — destructor
// ============================================================
extern "C" void gcMessageHandler___dtor_gcMessageHandler_void(gcMessageHandler *self, int flags) {
    if (self != 0) {
        self->mVTable = gcMessageHandlervirtualtable;
        if ((char *)&self->mSub != 0) {
            int enc = self->mSub.mEncoded;
            int doDestroy = 1;
            if (enc & 1) {
                doDestroy = 0;
            }
            if (doDestroy && enc != 0) {
                void *vt = *(void **)((char *)enc + 4);
                DeleteRecordSub *rec = (DeleteRecordSub *)((char *)vt + 0x50);
                short so = rec->offset;
                rec->fn((char *)enc + so, 3);
                self->mSub.mEncoded = 0;
            }
        }
        self->mVTable = gcMessageHandlerbasevtable;
        if (flags & 1) {
            void *pool = cMemPool::GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off2 = rec->offset;
            rec->fn((char *)block + off2, self);
        }
    }
}

int gcMessageHandler::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$17") = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    if (rb[3] != 2) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cHandle *handle0 = (cHandle *)((char *)this + 0x08);
    *(int *)((char *)this + 0x08) = 0;
    handle0->Read(*(cReadBlock *)rb, cMemPool::GetPoolFromPtr(handle0));
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x0C, 4);

    int handleValue = *(int *)((char *)this + 0x08);
    int index = handleValue & 0xFFFF;
    void *lookup;
    if (handleValue != 0) {
        void *entry = D_00038890[index];
        lookup = 0;
        if (entry != 0) {
            if (*(int *)((char *)entry + 0x30) == handleValue) {
                lookup = entry;
            }
        }
    } else {
        lookup = 0;
    }

    if (lookup != 0) {
        void *entry2 = 0;
        if (handleValue != 0) {
            entry2 = D_00038890[(unsigned short)handleValue];
        }
        unsigned char flags = *(unsigned char *)((char *)entry2 + 0x4C);
        int bit = flags & 4;
        int hasBit = bit != 0;
        hasBit &= 0xFF;
        if (hasBit != 0) {
            int value = *(int *)((char *)this + 0x0C);
            value &= 0x8000FFFF;
            value |= 0x00010000;
            *(int *)((char *)this + 0x0C) = value;
        }
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x10, 4);

    cHandle *handle1 = (cHandle *)((char *)this + 0x14);
    *(int *)((char *)this + 0x14) = 0;
    handle1->Read(*(cReadBlock *)rb, cMemPool::GetPoolFromPtr(handle1));

    int sp14;
    int tagged = *(int *)((char *)this + 0x18);
    int tag = tagged & 1;
    int flag = 0;
    if (tag != 0) {
        flag = 1;
    }

    int outValue;
    if (flag != 0) {
        outValue = 0;
        goto out_done;
    }
    outValue = tagged;
out_done:
    sp14 = outValue;

    int flag2 = 0;
    if (tag != 0) {
        flag2 = 1;
    }

    int base;
    if (flag2 != 0) {
        base = tagged & ~1;
    } else {
        base = *(int *)tagged;
    }

    ((cReadBlock *)rb)->ReadBase(cMemPool::GetPoolFromPtr((char *)this + 0x18),
                                 (cBase *)base, *(cBase **)&sp14);

    int newValue;
    if (sp14 == 0) {
        newValue = base | 1;
    } else {
        newValue = sp14;
    }
    *(int *)((char *)this + 0x18) = newValue;

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
