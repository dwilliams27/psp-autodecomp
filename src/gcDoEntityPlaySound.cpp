#include "cBase.h"

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

class cBaseArray {
public:
    void SetSize(int);
    void Read(class cReadBlock &);
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Read(class cReadBlock &);
    void Write(cWriteBlock &) const;
};

class gcDesiredObject;

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
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

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
};

struct VTableSlot {
    short offset;
    short _pad;
    const cType *(*getType)(void *);
};

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");

extern char D_00000338[];
extern char D_00000658[] asm("D_00000658");
extern char gcDoEntityPlaySoundvirtualtable[];

class gcDoEntityPlaySound : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    gcDoEntityPlaySound &operator=(const gcDoEntityPlaySound &);
};

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoEntityPlaySound asm("D_0009F630");

void gcDoEntityPlaySound::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoEntityPlaySound) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoEntityPlaySound = cType::InitializeType(
                0, 0, 0x14F, type_action,
                gcDoEntityPlaySound::New, 0, 0, 0x80);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoEntityPlaySound;
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
            type = (const cType *)((cTypeNode *)type)->parent;
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
    *this = *(const gcDoEntityPlaySound *)copy;
}

cBase *gcDoEntityPlaySound::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntityPlaySound *result = 0;
    gcDoEntityPlaySound *obj =
        (gcDoEntityPlaySound *)entry->fn(base, 0x5C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoEntityPlaySoundvirtualtable;

        int encodedObj = (int)obj | 1;
        char *desired = (char *)obj + 0x10;
        *(int *)((char *)obj + 0x0C) = encodedObj;
        gcDesiredObject_gcDesiredObject(desired, (cBase *)obj);
        *(void **)((char *)obj + 0x14) = D_00000338;

        char *helper = (char *)obj + 0x1C;
        int one = 1;
        gcDesiredEntityHelper_ctor(helper, 1, 0, 0);
        *(void **)((char *)obj + 0x14) = (void *)0x388A48;
        *(void **)((char *)obj + 0x28) = (void *)0x37E6A8;
        *(void **)((char *)obj + 0x24) = desired;
        *(void **)((char *)obj + 0x28) = (void *)0x388568;
        *(unsigned char *)((char *)obj + 0x2C) = one;
        *(unsigned char *)((char *)obj + 0x2D) = 0;
        *(int *)((char *)obj + 0x30) = 0;
        *(int *)((char *)obj + 0x34) = 0;
        *(int *)((char *)obj + 0x38) = (int)desired | 1;
        *(int *)((char *)obj + 0x3C) = 0;
        *(void **)((char *)obj + 0x40) = obj;
        *(int *)((char *)obj + 0x44) = encodedObj;
        *(int *)((char *)obj + 0x48) = encodedObj;
        *(int *)((char *)obj + 0x4C) = encodedObj;
        *(int *)((char *)obj + 0x50) = encodedObj;
        *(unsigned char *)((char *)obj + 0x54) = 0;
        *(unsigned char *)((char *)obj + 0x55) = 0;
        *(int *)((char *)obj + 0x58) = encodedObj;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoEntityPlaySound::GetType(void) const {
    if (!type_gcDoEntityPlaySound) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A,
                                                        type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoEntityPlaySound = cType::InitializeType(
            0, 0, 0x14F, type_action, gcDoEntityPlaySound::New, 0, 0, 0x80);
    }
    return type_gcDoEntityPlaySound;
}

void gcDoEntityPlaySound::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    gcAction::Write(file);
    ((const gcDesiredValue *)((const char *)this + 0x0C))->Write(wb);

    {
        char *typeInfo = *(char **)((const char *)this + 0x14);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        short off = rec->offset;
        void *base = (void *)((const char *)this + 0x10);
        rec->fn((char *)base + off, wb._file);
    }

    ((const cBaseArray *)((const char *)this + 0x3C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x44))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x48))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x4C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x50))->Write(wb);
    wb.Write(*(const bool *)((const char *)this + 0x54));
    wb.Write(*(const bool *)((const char *)this + 0x55));
    ((const gcDesiredValue *)((const char *)this + 0x58))->Write(wb);
    wb.End();
}

int gcDoEntityPlaySound::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    char flag0;
    char flag1;

    __0oKcReadBlockctR6FcFileUib(rb, file, 5, true);
    if ((unsigned int)rb[3] >= 6 || (unsigned int)rb[3] < 3 ||
        ((gcAction *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    ((gcDesiredValue *)((char *)this + 0x0C))->Read(*(cReadBlock *)rb);
    {
        char *typeInfo = *(char **)((char *)this + 0x14);
        char *base = (char *)this + 0x10;
        ReadRec *rec = (ReadRec *)(typeInfo + 0x30);
        short off = rec->offset;
        cFile *f = *(cFile **)&rb[0];
        rec->fn(base + off, f, cMemPool::GetPoolFromPtr(base));
    }

    int version = rb[3];
    if ((unsigned int)version < 4) {
        ((cBaseArray *)((char *)this + 0x3C))->SetSize(1);

        void *block = ((void **)pool)[9];
        register char *allocTable __asm__("$6") = ((PoolBlock *)block)->allocTable;
        gcDesiredObject *obj = 0;
        AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
        register int off __asm__("$7") = entry->offset;
        void *base = (char *)block + off;
        gcDesiredObject *created =
            (gcDesiredObject *)entry->fn(base, 0x14, 4, 0, 0);
        if (created != 0) {
            gcDesiredObject_gcDesiredObject(created, (cBase *)this);
            *(void **)((char *)created + 0x04) = D_00000658;
            *(int *)((char *)created + 0x0C) = 7;
            *(int *)((char *)created + 0x10) = 0;
            *(void **)((char *)created + 0x04) = (void *)0x389800;
            obj = created;
        }

        **(gcDesiredObject ***)((char *)this + 0x3C) = obj;
        {
            char *desired = **(char ***)((char *)this + 0x3C);
            char *typeInfo = (char *)((void **)desired)[1];
            ReadRec *rec = (ReadRec *)(typeInfo + 0x30);
            short roff = rec->offset;
            cFile *f = *(cFile **)&rb[0];
            rec->fn(desired + roff, f, cMemPool::GetPoolFromPtr(desired));
        }
        version = rb[3];
    }

    if ((unsigned int)version >= 4) {
        ((cBaseArray *)((char *)this + 0x3C))->Read(*(cReadBlock *)rb);
        ((gcDesiredValue *)((char *)this + 0x44))->Read(*(cReadBlock *)rb);
        ((gcDesiredValue *)((char *)this + 0x48))->Read(*(cReadBlock *)rb);
        ((gcDesiredValue *)((char *)this + 0x4C))->Read(*(cReadBlock *)rb);
        ((gcDesiredValue *)((char *)this + 0x50))->Read(*(cReadBlock *)rb);
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], &flag0, 1);
    int version2 = rb[3];
    *(unsigned char *)((char *)this + 0x54) = flag0 != 0;
    if ((unsigned int)version2 >= 5) {
        cFileSystem::Read(*(cFileHandle **)rb[0], &flag1, 1);
        *(unsigned char *)((char *)this + 0x55) = flag1 != 0;
        ((gcDesiredValue *)((char *)this + 0x58))->Read(*(cReadBlock *)rb);
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
