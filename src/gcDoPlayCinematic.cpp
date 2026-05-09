#include "cBase.h"

class cFileHandle;
class cReadBlock;

class gcDoPlayCinematic {
public:
    int GetExprFlags(void) const;
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    gcDoPlayCinematic &operator=(const gcDoPlayCinematic &);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDoPlayMovie {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    gcDoPlayMovie &operator=(const gcDoPlayMovie &);
};

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
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
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class cHandle {
public:
    int mIndex;
    void Read(cReadBlock &, cMemPool *);
};

class cGUID {
public:
    void Read(cReadBlock &);
};

class cBaseArray {
public:
    void Read(cReadBlock &);
};

class gcDesiredValue {
public:
    void Read(cReadBlock &);
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

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoPlayCinematic asm("D_0009F6C0");
static cType *type_gcDoPlayMovie asm("D_0009F6D4");

struct gcDoPlayCinematicData {
    char _pad[0x24];
    gcExpression **mChildren;
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

gcExpression *gcDoPlayCinematic::GetChild(int index) const {
    gcDoPlayCinematicData *self = (gcDoPlayCinematicData *)this;
    return self->mChildren[index];
}

int gcDoPlayCinematic::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 5, true);
    if ((unsigned int)rb[3] >= 6 || (unsigned int)rb[3] < 2 ||
        ((gcAction *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x0C, 4);

    cHandle *handle0 = (cHandle *)((char *)this + 0x10);
    *(int *)((char *)this + 0x10) = 0;
    handle0->Read(*(cReadBlock *)rb, cMemPool::GetPoolFromPtr(handle0));

    if ((unsigned int)rb[3] >= 3) {
        cHandle *handle1 = (cHandle *)((char *)this + 0x14);
        *(int *)((char *)this + 0x14) = 0;
        handle1->Read(*(cReadBlock *)rb, cMemPool::GetPoolFromPtr(handle1));
    }

    ((cGUID *)((char *)this + 0x18))->Read(*(cReadBlock *)rb);

    int value = *(int *)((char *)this + 0x20);
    int tag = value & 1;
    int flag = 0;
    if (tag != 0) {
        flag = 1;
    }

    int outValue;
    if (flag != 0) {
        outValue = 0;
        goto out_done0;
    }
    outValue = value;
out_done0:
    int sp14 = outValue;

    int flag2 = 0;
    if (tag != 0) {
        flag2 = 1;
    }

    int base;
    if (flag2 != 0) {
        base = value & ~1;
    } else {
        base = *(int *)value;
    }

    ((cReadBlock *)rb)->ReadBase(cMemPool::GetPoolFromPtr((char *)this + 0x20),
                                 (cBase *)base, *(cBase **)&sp14);

    int newValue;
    if (sp14 == 0) {
        newValue = base | 1;
    } else {
        newValue = sp14;
    }
    *(int *)((char *)this + 0x20) = newValue;

    ((cBaseArray *)((char *)this + 0x24))->Read(*(cReadBlock *)rb);

    if ((unsigned int)rb[3] >= 5) {
        ((gcDesiredValue *)((char *)this + 0x2C))->Read(*(cReadBlock *)rb);
    }
    if ((unsigned int)rb[3] >= 4) {
        ((gcDesiredValue *)((char *)this + 0x30))->Read(*(cReadBlock *)rb);
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

void gcDoPlayCinematic::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoPlayCinematic) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C, 1,
                            0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoPlayCinematic = cType::InitializeType(
                0, 0, 0x16A, type_action,
                gcDoPlayCinematic::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoPlayCinematic;
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
    *this = *(const gcDoPlayCinematic *)copy;
}

void gcDoPlayMovie::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoPlayMovie) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C, 1,
                            0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoPlayMovie = cType::InitializeType(
                0, 0, 0x152, type_action,
                gcDoPlayMovie::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoPlayMovie;
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
    *this = *(const gcDoPlayMovie *)copy;
}
