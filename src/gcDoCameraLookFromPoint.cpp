#include "cBase.h"

class cFile;
class cFileHandle;
class cReadBlock;

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
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    cFile *_file;
    int _pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void End(void);
};

class gcDesiredValue {
public:
    void Read(cReadBlock &);
    void Write(cWriteBlock &) const;
};

class gcDesiredCamera {
public:
    void GetText(char *) const;
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
};

struct ReadRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

struct TextRec {
    short offset;
    short _pad;
    void (*fn)(void *, char *);
};

struct VTableSlot {
    short offset;
    short _pad;
    const cType *(*getType)(void *);
};

struct cTypeNode {
    char pad[0x1C];
    const cType *parent;
};

class gcDoCameraLookFromPoint : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    gcDoCameraLookFromPoint &operator=(const gcDoCameraLookFromPoint &);
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    void GetText(char *) const;
};

void cStrAppend(char *, const char *, ...);

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredCamera_gcDesiredCamera(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

extern char D_00000338[];
extern char D_000005B8[];
extern char D_000018C8[];
extern char D_00001B68[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoCameraFollow;
static cType *type_gcDoCameraLookFromPoint;

cBase *gcDoCameraLookFromPoint::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoCameraLookFromPoint *result = 0;
    gcDoCameraLookFromPoint *obj =
        (gcDoCameraLookFromPoint *)entry->fn(base, 0x8C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = D_000018C8;

        gcDesiredCamera_gcDesiredCamera((char *)obj + 0x0C, (cBase *)obj);

        int encoded = (int)obj | 1;
        *(int *)((char *)obj + 0x44) = encoded;
        *(unsigned char *)((char *)obj + 0x48) = 0;
        ((void **)obj)[1] = D_00001B68;

        gcDesiredObject_gcDesiredObject((char *)obj + 0x4C, (cBase *)obj);

        *(void **)((char *)obj + 0x50) = D_000005B8;
        *(int *)((char *)obj + 0x58) = 7;
        *(int *)((char *)obj + 0x5C) = 0;
        *(void **)((char *)obj + 0x50) = (void *)0x389760;

        char *desired = (char *)obj + 0x60;
        gcDesiredObject_gcDesiredObject(desired, (cBase *)obj);
        *(void **)((char *)obj + 0x64) = D_00000338;

        gcDesiredEntityHelper_ctor((char *)obj + 0x6C, 0, 0, 0);
        *(void **)((char *)obj + 0x64) = (void *)0x388A48;
        *(void **)((char *)obj + 0x78) = (void *)0x37E6A8;
        *(void **)((char *)obj + 0x74) = desired;
        *(void **)((char *)obj + 0x78) = (void *)0x388568;
        *(unsigned char *)((char *)obj + 0x7C) = 1;
        *(unsigned char *)((char *)obj + 0x7D) = 0;
        *(int *)((char *)obj + 0x80) = 0;
        *(int *)((char *)obj + 0x84) = 0;
        *(int *)((char *)obj + 0x88) = (int)desired | 1;
        result = obj;
    }
    return (cBase *)result;
}

void gcDoCameraLookFromPoint::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoCameraLookFromPoint) {
            if (!type_gcDoCameraFollow) {
                if (!type_action) {
                    if (!type_expression) {
                        if (!type_base) {
                            type_base = cType::InitializeType(
                                (const char *)0x36D894,
                                (const char *)0x36D89C,
                                1, 0, 0, 0, 0, 0);
                        }
                        type_expression = cType::InitializeType(
                            0, 0, 0x6A, type_base, 0, 0, 0, 0);
                    }
                    type_action = cType::InitializeType(
                        0, 0, 0x6B, type_expression, 0, 0, 0, 0);
                }
                type_gcDoCameraFollow = cType::InitializeType(
                    0, 0, 0x24F, type_action, 0, 0, 0, 0);
            }
            type_gcDoCameraLookFromPoint = cType::InitializeType(
                0, 0, 0x250, type_gcDoCameraFollow,
                gcDoCameraLookFromPoint::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoCameraLookFromPoint;
        VTableSlot *slot = (VTableSlot *)((char *)vt + 8);
        short voff = slot->offset;
        const cType *(*getType)(void *) = slot->getType;
        const cType *type = getType((char *)other + voff);
        int ok;

        if (myType == 0) {
            ok = 0;
            goto done;
        }
        if (type != 0) {
        loop:
            if (type == myType) {
                ok = 1;
                goto done;
            }
            type = ((cTypeNode *)type)->parent;
            if (type != 0) {
                goto loop;
            }
        }
        ok = 0;
    done:
        if (ok != 0) {
            copy = other;
        }
    }
    *this = *(const gcDoCameraLookFromPoint *)copy;
}

const cType *gcDoCameraLookFromPoint::GetType(void) const {
    if (!type_gcDoCameraLookFromPoint) {
        if (!type_gcDoCameraFollow) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            (const char *)0x36D894,
                            (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoCameraFollow = cType::InitializeType(
                0, 0, 0x24F, type_action, 0, 0, 0, 0);
        }
        type_gcDoCameraLookFromPoint = cType::InitializeType(
            0, 0, 0x250, type_gcDoCameraFollow,
            gcDoCameraLookFromPoint::New, 0, 0, 0);
    }
    return type_gcDoCameraLookFromPoint;
}

void gcDoCameraLookFromPoint::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cWriteBlock inner(file, 1);
    gcAction::Write(file);

    {
        char *typeInfo = *(char **)((const char *)this + 0x10);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        short off = rec->offset;
        void *base = (void *)((const char *)this + 0x0C);
        rec->fn((char *)base + off, inner._file);
    }

    ((const gcDesiredValue *)((const char *)this + 0x44))->Write(inner);
    inner.Write(*(const bool *)((const char *)this + 0x48));
    inner.End();

    {
        char *typeInfo = *(char **)((const char *)this + 0x50);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        short off = rec->offset;
        void *base = (void *)((const char *)this + 0x4C);
        rec->fn((char *)base + off, wb._file);
    }

    {
        char *typeInfo = *(char **)((const char *)this + 0x64);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        short off = rec->offset;
        void *base = (void *)((const char *)this + 0x60);
        rec->fn((char *)base + off, wb._file);
    }

    wb.End();
}

int gcDoCameraLookFromPoint::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    int inner[5];
    char flag;
    int ok;

    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1) {
        goto fail_outer;
    }

    __0oKcReadBlockctR6FcFileUib(inner, file, 1, true);
    if (inner[3] != 1) {
        goto fail_inner;
    }
    if (((gcAction *)this)->Read(file, pool) != 0) {
        goto read_inner;
    }

fail_inner:
    cFile_SetCurrentPos(*(void **)&inner[0], inner[1]);
    __0oKcReadBlockdtv(inner, 2);
    ok = 0;
    goto after_inner;

read_inner:
    {
        char *typeInfo0 = *(char **)((char *)this + 0x10);
        char *base0 = (char *)this + 0x0C;
        ReadRec *rec0 = (ReadRec *)(typeInfo0 + 0x30);
        short off0 = rec0->offset;
        cFile *f0 = *(cFile **)&inner[0];
        rec0->fn(base0 + off0, f0, cMemPool::GetPoolFromPtr(base0));
    }

    ((gcDesiredValue *)((char *)this + 0x44))->Read(*(cReadBlock *)inner);

    cFileSystem::Read(*(cFileHandle **)inner[0], &flag, 1);
    *(unsigned char *)((char *)this + 0x48) = flag != 0;

    __0oKcReadBlockdtv(inner, 2);
    ok = 1;

after_inner:
    if (ok != 0) {
        goto read_outer;
    }

fail_outer:
    cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
    __0oKcReadBlockdtv(rb, 2);
    return 0;

read_outer:
    {
        char *typeInfo1 = *(char **)((char *)this + 0x50);
        char *base1 = (char *)this + 0x4C;
        ReadRec *rec1 = (ReadRec *)(typeInfo1 + 0x30);
        short off1 = rec1->offset;
        cFile *f1 = *(cFile **)&rb[0];
        rec1->fn(base1 + off1, f1, cMemPool::GetPoolFromPtr(base1));
    }

    {
        char *typeInfo2 = *(char **)((char *)this + 0x64);
        char *base2 = (char *)this + 0x60;
        ReadRec *rec2 = (ReadRec *)(typeInfo2 + 0x30);
        short off2 = rec2->offset;
        cFile *f2 = *(cFile **)&rb[0];
        rec2->fn(base2 + off2, f2, cMemPool::GetPoolFromPtr(base2));
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

void gcDoCameraLookFromPoint::GetText(char *buf) const {
    ((const gcDesiredCamera *)((const char *)this + 0x0C))->GetText(buf);

    const char *fmt = (const char *)0x36E3D0;
    int hasFrom = *(const signed char *)((const char *)this + 0x6C) != 0;
    const char *arg;
    if (hasFrom != 0) {
        arg = (const char *)0x36E3E4;
    } else {
        arg = (const char *)0x36D944;
    }
    cStrAppend(buf, fmt, arg);

    {
        char *typeInfo = *(char **)((const char *)this + 0x50);
        TextRec *rec = (TextRec *)(typeInfo + 0x78);
        short off = rec->offset;
        void *base = (void *)((const char *)this + 0x4C);
        rec->fn((char *)base + off, buf);
    }

    if (hasFrom != 0) {
        cStrAppend(buf, (const char *)0x36DAD8);
        char *typeInfo = *(char **)((const char *)this + 0x64);
        TextRec *rec = (TextRec *)(typeInfo + 0x78);
        short off = rec->offset;
        void *base = (void *)((const char *)this + 0x60);
        rec->fn((char *)base + off, buf);
    }

    cStrAppend(buf, (const char *)0x36DCEC);
}
