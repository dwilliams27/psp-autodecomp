// gcValEntityDirection -- entity direction value helpers.

class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

inline void *operator new(unsigned int, void *p) { return p; }

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *);
};

struct ReadEntry {
    short offset;
    short pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

class cWriteBlock {
public:
    cFile *file;
    unsigned int pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void End(void);
};

class cName {
public:
    void Write(cWriteBlock &) const;
    void Read(class cReadBlock &);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char pad[0x1C];
    cType *mParent;
    char pad20[0x8];
    cTypeMethod write_m;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
    void Read(class cReadBlock &);
};

class gcValue {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class cReadBlock {
public:
    int _data[5];
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class gcDesiredObject {
public:
    gcDesiredObject(cBase *);
};

class gcDesiredEntityHelper {
public:
    enum gcPrimary { gcPrimary_One = 1 };
    enum gcRelationship { gcRelationship_None = 0 };

    gcDesiredEntityHelper(gcPrimary, gcRelationship, gcRelationship);
};

class gcDesiredCamera {
public:
    gcDesiredCamera(cBase *);
};

void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);
extern "C" void gcDesiredCamera_gcDesiredCamera(void *, cBase *);
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

__asm__(".globl gcValEntityDirectionvirtualtable_abs\n"
        "gcValEntityDirectionvirtualtable_abs = 0x6f48\n");
__asm__(".globl D_00000338_abs_entity_direction\n"
        "D_00000338_abs_entity_direction = 0x338\n");

extern char gcValEntityDirectionvirtualtable_abs[];
extern char D_00000338_abs_entity_direction[];

class gcValEntityDirection : public gcValue {
public:
    gcValEntityDirection(cBase *);

    gcValEntityDirection &operator=(const gcValEntityDirection &);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

// -- gcValEntityDirection::New(cMemPool *, cBase *) static @ 0x00328904 --
cBase *gcValEntityDirection::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcValEntityDirection *result = 0;
    gcValEntityDirection *obj =
        (gcValEntityDirection *)rec->fn(base, 0x108, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcValEntityDirection(parent);
        result = obj;
    }
    return (cBase *)result;
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValEntityDirection;

void gcValEntityDirection::AssignCopy(const cBase *base) {
    const gcValEntityDirection *other = 0;

    if (base != 0) {
        if (!type_gcValEntityDirection) {
            if (!type_value) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_value = cType::InitializeType(
                    0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
            }
            type_gcValEntityDirection = cType::InitializeType(
                0, 0, 0xF0, type_value, gcValEntityDirection::New,
                0, 0, 0);
        }

        DispatchEntry *entry =
            (DispatchEntry *)((char *)*(void **)((char *)base + 4) + 8);
        cType *wanted = type_gcValEntityDirection;
        cType *type = entry->fn((char *)base + entry->offset);
        int ok;

        if (wanted == 0) {
            ok = 0;
        } else if (type != 0) {
        loop:
            if (type == wanted) {
                ok = 1;
            } else {
                type = type->mParent;
                if (type != 0) {
                    goto loop;
                }
                goto fail;
            }
        } else {
fail:
            ok = 0;
        }
        if (ok != 0) {
            other = (const gcValEntityDirection *)base;
        }
    }

    operator=(*other);
}

gcValEntityDirection::gcValEntityDirection(cBase *parent) {
    *(cBase **)((char *)this + 0) = parent;
    *(void **)((char *)this + 4) = gcValEntityDirectionvirtualtable_abs;
    char *left = (char *)this + 0xC;
    *(int *)((char *)this + 8) = 0;
    gcDesiredObject_ctor(left, this);
    void *desiredType = D_00000338_abs_entity_direction;
    *(void **)((char *)this + 0x10) = desiredType;
    void *leftHelper = (char *)this + 0x18;
    __asm__ volatile("" : "+r"(leftHelper));
    register int one asm("s3") = 1;
    gcDesiredEntityHelper_ctor(leftHelper, 1, 0, 0);
    register void *helperDesc asm("s4") = (void *)0x388A48;
    *(void **)((char *)this + 0x10) = helperDesc;
    *(char **)((char *)this + 0x20) = left;
    register void *desiredDesc asm("s5") = (void *)0x388568;
    *(void **)((char *)this + 0x24) = desiredDesc;
    *(unsigned char *)((char *)this + 0x28) = one;
    *(unsigned char *)((char *)this + 0x29) = 0;
    *(int *)((char *)this + 0x2C) = 0;
    int leftFlagged = (int)left | 1;
    *(int *)((char *)this + 0x30) = 0;
    *(int *)((char *)this + 0x34) = leftFlagged;

    char *right = (char *)this + 0x38;
    gcDesiredObject_ctor(right, this);
    *(void **)((char *)this + 0x3C) = desiredType;
    gcDesiredEntityHelper_ctor((char *)this + 0x44, 1, 0, 0);
    *(void **)((char *)this + 0x3C) = helperDesc;
    *(char **)((char *)this + 0x4C) = right;
    *(void **)((char *)this + 0x50) = desiredDesc;
    *(unsigned char *)((char *)this + 0x54) = one;
    *(unsigned char *)((char *)this + 0x55) = 0;
    *(int *)((char *)this + 0x58) = 0;
    int rightFlagged = (int)right | 1;
    *(int *)((char *)this + 0x5C) = 0;
    *(int *)((char *)this + 0x60) = rightFlagged;

    gcDesiredObject_ctor((char *)this + 0x64, this);
    *(int *)((char *)this + 0x70) = 7;
    *(int *)((char *)this + 0x74) = 0;
    *(void **)((char *)this + 0x68) = (void *)0x389760;
    gcDesiredCamera_gcDesiredCamera((char *)this + 0x78, (cBase *)this);

    *(short *)((char *)this + 0xC4) = 0;
    *(short *)((char *)this + 0xC6) = 0;
    int self = (int)this | 1;
    *(unsigned char *)((char *)this + 0xB0) = 0;
    *(int *)((char *)this + 0xC8) = self;
    *(short *)((char *)this + 0xE0) = 0;
    *(short *)((char *)this + 0xE2) = 0;
    *(unsigned char *)((char *)this + 0xCC) = 0;
    *(int *)((char *)this + 0xE4) = self;
    *(int *)((char *)this + 0xE8) = 0;
    *(int *)((char *)this + 0xEC) = 0;
    *(int *)((char *)this + 0xF0) = 0;
    *(int *)((char *)this + 0xF4) = self;
    *(int *)((char *)this + 0xF8) = self;
    *(int *)((char *)this + 0xFC) = self;
    *(int *)((char *)this + 0x100) = self;
    *(unsigned char *)((char *)this + 0x104) = 0;
    *(unsigned char *)((char *)this + 0x105) = 0;
    *(unsigned char *)((char *)this + 0x106) = 0;
}

const cType *gcValEntityDirection::GetType(void) const {
    if (!type_gcValEntityDirection) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValEntityDirection =
            cType::InitializeType(0, 0, 0xF0, type_value,
                                  gcValEntityDirection::New, 0, 0, 0);
    }
    return type_gcValEntityDirection;
}

void gcValEntityDirection::Write(cFile &file) const {
    cWriteBlock wb(file, 0xB);
    gcValue::Write(file);
    wb.Write(*(const int *)((const char *)this + 8));

    typedef void (*WriteFn)(void *, cFile *);

    const cTypeMethod *leftWrite =
        &((const cType *)*(void *const *)((const char *)this + 0x10))->write_m;
    char *leftBase = (char *)this + 0xC;
    ((WriteFn)leftWrite->fn)(leftBase + leftWrite->offset, wb.file);

    const cTypeMethod *rightWrite =
        &((const cType *)*(void *const *)((const char *)this + 0x3C))->write_m;
    char *rightBase = (char *)this + 0x38;
    ((WriteFn)rightWrite->fn)(rightBase + rightWrite->offset, wb.file);

    ((const cName *)((const char *)this + 0xB0))->Write(wb);
    ((const cName *)((const char *)this + 0xCC))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xE4))->Write(wb);
    wb.Write(*(const int *)((const char *)this + 0xEC));
    ((const gcDesiredValue *)((const char *)this + 0xF4))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xF8))->Write(wb);
    wb.Write(*(const int *)((const char *)this + 0xE8));

    const cTypeMethod *cameraTargetWrite =
        &((const cType *)*(void *const *)((const char *)this + 0x68))->write_m;
    char *cameraTargetBase = (char *)this + 0x64;
    ((WriteFn)cameraTargetWrite->fn)(cameraTargetBase + cameraTargetWrite->offset,
                                     wb.file);

    wb.Write(*(const bool *)((const char *)this + 0x104));
    ((const gcDesiredValue *)((const char *)this + 0xFC))->Write(wb);
    wb.Write(*(const bool *)((const char *)this + 0x105));
    wb.Write(*(const bool *)((const char *)this + 0x106));
    wb.Write(*(const int *)((const char *)this + 0xF0));
    ((const gcDesiredValue *)((const char *)this + 0x100))->Write(wb);

    const cTypeMethod *cameraWrite =
        &((const cType *)*(void *const *)((const char *)this + 0x7C))->write_m;
    char *cameraBase = (char *)this + 0x78;
    ((WriteFn)cameraWrite->fn)(cameraBase + cameraWrite->offset, wb.file);

    ((const gcDesiredValue *)((const char *)this + 0xC8))->Write(wb);
    wb.End();
}

#pragma control sched=2
int gcValEntityDirection::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 0xB, true);
    if ((unsigned int)rb[3] >= 0xC || (unsigned int)rb[3] < 9 ||
        gcValue::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x08, 4);

    {
        char *typeInfo = *(char **)((char *)this + 0x10);
        ReadEntry *rec = (ReadEntry *)(typeInfo + 0x30);
        char *base = (char *)this + 0x0C;
        short off = rec->offset;
        char *target = base + off;
        register int f __asm__("$17") = rb[0];
        cMemPool *desiredPool = cMemPool::GetPoolFromPtr(base);
        rec->fn(target, (cFile *)f, desiredPool);
    }

    {
        char *typeInfo = *(char **)((char *)this + 0x3C);
        ReadEntry *rec = (ReadEntry *)(typeInfo + 0x30);
        char *base = (char *)this + 0x38;
        short off = rec->offset;
        char *target = base + off;
        register int f __asm__("$17") = rb[0];
        cMemPool *desiredPool = cMemPool::GetPoolFromPtr(base);
        rec->fn(target, (cFile *)f, desiredPool);
    }

    ((cName *)((char *)this + 0xB0))->Read(*(cReadBlock *)rb);
    ((cName *)((char *)this + 0xCC))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0xE4))->Read(*(cReadBlock *)rb);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0xEC, 4);
    ((gcDesiredValue *)((char *)this + 0xF4))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0xF8))->Read(*(cReadBlock *)rb);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0xE8, 4);

    {
        char *typeInfo = *(char **)((char *)this + 0x68);
        ReadEntry *rec = (ReadEntry *)(typeInfo + 0x30);
        char *base = (char *)this + 0x64;
        short off = rec->offset;
        char *target = base + off;
        register int f __asm__("$17") = rb[0];
        cMemPool *desiredPool = cMemPool::GetPoolFromPtr(base);
        rec->fn(target, (cFile *)f, desiredPool);
    }

    char sp14;
    cFileSystem::Read(*(cFileHandle **)rb[0], &sp14, 1);
    *(unsigned char *)((char *)this + 0x104) = sp14 != 0;

    ((gcDesiredValue *)((char *)this + 0xFC))->Read(*(cReadBlock *)rb);

    char sp15;
    cFileSystem::Read(*(cFileHandle **)rb[0], &sp15, 1);
    int readFile1 = rb[0];
    *(unsigned char *)((char *)this + 0x105) = sp15 != 0;

    char sp16;
    cFileSystem::Read(*(cFileHandle **)readFile1, &sp16, 1);
    int readFile2 = rb[0];
    *(unsigned char *)((char *)this + 0x106) = sp16 != 0;

    cFileSystem::Read(*(cFileHandle **)readFile2, (char *)this + 0xF0, 4);
    ((gcDesiredValue *)((char *)this + 0x100))->Read(*(cReadBlock *)rb);

    int version = rb[3];
    int under10 = (unsigned int)version < 0xA;
    if (under10 == 0) {
        char *typeInfo = *(char **)((char *)this + 0x7C);
        ReadEntry *rec = (ReadEntry *)(typeInfo + 0x30);
        char *base = (char *)this + 0x78;
        short off = rec->offset;
        char *target = base + off;
        register int f __asm__("$17") = rb[0];
        cMemPool *desiredPool = cMemPool::GetPoolFromPtr(base);
        rec->fn(target, (cFile *)f, desiredPool);
        version = rb[3];
    }
    if ((unsigned int)version >= 0xB) {
        ((gcDesiredValue *)((char *)this + 0xC8))->Read(*(cReadBlock *)rb);
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
