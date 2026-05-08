#include "cBase.h"

class cFile;
class cFileHandle;
class cMemPool;
class cReadBlock;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *), const char *, const char *,
                                 unsigned int);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(bool);
    void End(void);
};

struct gcDesiredUIWidgetHelper {
    int _a;
    int _b;
    int _c;
    void Read(cReadBlock &);
    void Write(cWriteBlock &) const;
    void GetText(char *) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class cHandle {
public:
    int mId;
    void Read(cReadBlock &, cMemPool *);
};

class gcDoUISetOrigin : public gcAction {
public:
    ~gcDoUISetOrigin();
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    void GetText(char *) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

class eCameraEffectLayer {
public:
    cBase *mOwner;
    void *mClassDesc;
    bool mField8;
    char _pad[3];
    int mFieldC;
    cHandle mField10;

    int Read(cFile &, cMemPool *);
};

void cStrAppend(char *, const char *, ...);
void gcAction_Write(const gcDoUISetOrigin *, cFile &);
gcDoUISetOrigin *dcast(const cBase *);
void gcAction__gcAction_cBaseptr__0012F4C8(void *, cBase *);
void gcDesiredUIWidgetHelper_ctor(void *, int);
extern "C" int __0fIgcActionEReadR6FcFileP6IcMemPool(void *, cFile &, cMemPool *);
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

extern char gcDoUISetOriginvirtualtable[];
extern const char gcDoUISetOrigin_fmt[];
extern const char gcDoUISetOrigin_str2[];
extern const char gcDoUISetOrigin_base_name[] asm("D_0036D894");
extern const char gcDoUISetOrigin_base_desc[] asm("D_0036D89C");

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoUISetOrigin asm("D_0009F73C");

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// 0x0030fd44, 280B
const cType *gcDoUISetOrigin::GetType(void) const {
    if (!type_gcDoUISetOrigin) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcDoUISetOrigin_base_name,
                                                       gcDoUISetOrigin_base_desc,
                                                       1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoUISetOrigin = cType::InitializeType(
            0, 0, 0xE6, type_action, gcDoUISetOrigin::New, 0, 0, 0);
    }
    return type_gcDoUISetOrigin;
}

// 0x0030fca4, 160B
cBase *gcDoUISetOrigin::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUISetOrigin *result = 0;
    gcDoUISetOrigin *obj = (gcDoUISetOrigin *)entry->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        gcAction__gcAction_cBaseptr__0012F4C8(obj, parent);
        ((void **)obj)[1] = gcDoUISetOriginvirtualtable;
        gcDesiredUIWidgetHelper_ctor((char *)obj + 0xC, 1);
        *(int *)((char *)obj + 0x18) = 5;
        *(unsigned char *)((char *)obj + 0x1C) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// 0x0030fc24, 128B
void gcDoUISetOrigin::AssignCopy(const cBase *other) {
    gcDoUISetOrigin *src = dcast(other);
    int *self_i = (int *)this;
    int *src_i = (int *)src;
    int v = self_i[2] & ~3;
    self_i[2] = v;
    self_i[2] = v | (src_i[2] & 3);
    self_i[3] = src_i[3];
    int *s1 = (int *)((char *)src + 0x10);
    int *d1 = (int *)((char *)this + 0x10);
    int *s2 = (int *)((char *)src + 0x14);
    int *d2 = (int *)((char *)this + 0x14);
    *d1 = *s1;
    *d2 = *s2;
    self_i[6] = src_i[6];
    *(unsigned char *)((char *)this + 0x1C) = *(const unsigned char *)((char *)src + 0x1C);
}

// 0x0030fe5c, 112B
void gcDoUISetOrigin::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->Write(wb);
    wb.Write(*(int *)((char *)this + 0x18));
    wb.Write(*(bool *)((char *)this + 0x1C));
    wb.End();
}

// 0x0030fecc, 252B
int gcDoUISetOrigin::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1 || __0fIgcActionEReadR6FcFileP6IcMemPool(this, file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }
    ((gcDesiredUIWidgetHelper *)((char *)this + 0x0C))->Read(*(cReadBlock *)rb);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x18, 4);
    {
        char flag;
        cFileSystem::Read(*(cFileHandle **)rb[0], &flag, 1);
        *(unsigned char *)((char *)this + 0x1C) = (flag != 0);
    }
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// 0x0002d044, 260B
int eCameraEffectLayer::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$17");
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 4, true);
    if ((unsigned int)rb._data[3] < 5 && (unsigned int)rb._data[3] >= 3) {
        cFileSystem::Read(*(cFileHandle **)rb._data[0], &mFieldC, 4);
        register cHandle *h __asm__("$18") = &mField10;
        mField10.mId = 0;
        h->Read(rb, cMemPool::GetPoolFromPtr(h));
        char flag;
        if ((unsigned int)rb._data[3] < 4 && ((void)rb._data[0], 1)) goto done;
        cFileSystem::Read(*(cFileHandle **)rb._data[0], &flag, 1);
        mField8 = (flag != 0);
done:
        return result;
    }
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
}

// 0x00310110, 80B
void gcDoUISetOrigin::GetText(char *buf) const {
    char local[256];
    local[0] = *local = '\0';
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->GetText(local);
    cStrAppend(buf, gcDoUISetOrigin_fmt, local, gcDoUISetOrigin_str2);
}

// 0x00310160, 128B
void gcDoUISetOrigin::VisitReferences(unsigned int flags, cBase *ctx, void (*cb)(cBase *, unsigned int, void *), void *user, unsigned int mask) {
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->VisitReferences(flags, (cBase *)this, cb, user, mask);
}

// 0x003101e0, 124B
gcDoUISetOrigin::~gcDoUISetOrigin() {
    *(void **)((char *)this + 4) = gcDoUISetOriginvirtualtable;
}
