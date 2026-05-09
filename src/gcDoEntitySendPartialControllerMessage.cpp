// ODR-WARNING: local redeclaration TU. include/gcDoEntitySendMessage.h does
// not declare these Read methods; keep method additions source-local to avoid
// perturbing matched split-TU siblings.

class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class gcExpression;

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
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class cBaseArray {
public:
    void Read(cReadBlock &);
    void SetSize(int);
};

class gcAction {
public:
    int Read(cFile &, cMemPool *);
};

extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);
extern "C" void cFile_SetCurrentPos(void *, unsigned int);

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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

extern const char gcDoEntitySendPartialControllerMessage_base_name[];
extern const char gcDoEntitySendPartialControllerMessage_base_desc[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntitySendPartialControllerMessage;

struct gcDoEntitySendPartialControllerMessageData {
    char _pad[0x68];
    gcExpression **mChildren;
};

class gcDoEntitySendMessage {
public:
    void AssignCopy(const cBase *);
    int Read(cFile &, cMemPool *);
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    static cBase *New(cMemPool *, cBase *);
    gcDoEntitySendMessage &operator=(const gcDoEntitySendMessage &);
};

class gcDoEntitySendPartialControllerMessage {
public:
    gcDoEntitySendPartialControllerMessage(cBase *);
    void AssignCopy(const cBase *);
    int Read(cFile &, cMemPool *);
    gcExpression *GetChild(int) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    gcDoEntitySendPartialControllerMessage &operator=(
        const gcDoEntitySendPartialControllerMessage &);
};

struct ReadRec {
    short offset;
    short pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

gcExpression *gcDoEntitySendPartialControllerMessage::GetChild(int index) const {
    gcDoEntitySendPartialControllerMessageData *self = (gcDoEntitySendPartialControllerMessageData *)this;
    return self->mChildren[index];
}

const cType *gcDoEntitySendPartialControllerMessage::GetType(void) const {
    if (!type_gcDoEntitySendPartialControllerMessage) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoEntitySendPartialControllerMessage_base_name,
                        gcDoEntitySendPartialControllerMessage_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntitySendPartialControllerMessage = cType::InitializeType(
            0, 0, 0x10B, type_action,
            gcDoEntitySendPartialControllerMessage::New, 0, 0, 0x80);
    }
    return type_gcDoEntitySendPartialControllerMessage;
}

void gcDoEntitySendPartialControllerMessage::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoEntitySendPartialControllerMessage) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoEntitySendPartialControllerMessage_base_name,
                            gcDoEntitySendPartialControllerMessage_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoEntitySendPartialControllerMessage = cType::InitializeType(
                0, 0, 0x10B, type_action,
                gcDoEntitySendPartialControllerMessage::New, 0, 0, 0x80);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoEntitySendPartialControllerMessage;
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
    *this = *(const gcDoEntitySendPartialControllerMessage *)copy;
}

int gcDoEntitySendPartialControllerMessage::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 3, true);
    if (rb[3] != 3 || ((gcAction *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    {
        char *typeInfo0 = *(char **)((char *)this + 0x10);
        char *base0 = (char *)this + 0x0C;
        ReadRec *rec0 = (ReadRec *)(typeInfo0 + 0x30);
        short off0 = rec0->offset;
        cFile *f0 = *(cFile **)&rb[0];
        rec0->fn(base0 + off0, f0, cMemPool::GetPoolFromPtr(base0));
    }

    {
        char *typeInfo1 = *(char **)((char *)this + 0x3C);
        char *base1 = (char *)this + 0x38;
        ReadRec *rec1 = (ReadRec *)(typeInfo1 + 0x30);
        short off1 = rec1->offset;
        cFile *f1 = *(cFile **)&rb[0];
        rec1->fn(base1 + off1, f1, cMemPool::GetPoolFromPtr(base1));
    }

    {
        char *typeInfo2 = *(char **)((char *)this + 0x54);
        char *base2 = (char *)this + 0x50;
        ReadRec *rec2 = (ReadRec *)(typeInfo2 + 0x30);
        short off2 = rec2->offset;
        cFile *f2 = *(cFile **)&rb[0];
        rec2->fn(base2 + off2, f2, cMemPool::GetPoolFromPtr(base2));
    }

    cBaseArray *children = (cBaseArray *)((char *)this + 0x68);
    children->Read(*(cReadBlock *)rb);

    int sp14;
    int value = *(int *)((char *)this + 0x70);
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
    sp14 = outValue;

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

    ((cReadBlock *)rb)->ReadBase(cMemPool::GetPoolFromPtr((char *)this + 0x70),
                                 (cBase *)base, *(cBase **)&sp14);

    int newValue;
    if (sp14 == 0) {
        newValue = base | 1;
    } else {
        newValue = sp14;
    }
    *(int *)((char *)this + 0x70) = newValue;

    children->SetSize(4);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

int gcDoEntitySendMessage::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    struct Locals {
        int rb[5];
        int pad14;
        int sp18;
        char oldMode;
    } locals;

    __0oKcReadBlockctR6FcFileUib(locals.rb, file, 7, true);
    if ((unsigned int)locals.rb[3] >= 8 || (unsigned int)locals.rb[3] < 6 ||
        ((gcAction *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&locals.rb[0], locals.rb[1]);
        __0oKcReadBlockdtv(locals.rb, 2);
        return 0;
    }

    {
        char *typeInfo0 = *(char **)((char *)this + 0x10);
        char *base0 = (char *)this + 0x0C;
        ReadRec *rec0 = (ReadRec *)(typeInfo0 + 0x30);
        short off0 = rec0->offset;
        cFile *f0 = *(cFile **)&locals.rb[0];
        rec0->fn(base0 + off0, f0, cMemPool::GetPoolFromPtr(base0));
    }

    {
        char *typeInfo1 = *(char **)((char *)this + 0x3C);
        char *base1 = (char *)this + 0x38;
        ReadRec *rec1 = (ReadRec *)(typeInfo1 + 0x30);
        short off1 = rec1->offset;
        cFile *f1 = *(cFile **)&locals.rb[0];
        rec1->fn(base1 + off1, f1, cMemPool::GetPoolFromPtr(base1));
    }

    cBaseArray *children = (cBaseArray *)((char *)this + 0x50);
    children->Read(*(cReadBlock *)locals.rb);

    int value = *(int *)((char *)this + 0x58);
    int tag = value & 1;
    int flag = 0;
    if (tag != 0) {
        flag = 1;
    }

    int outValue;
    if (flag != 0) {
        outValue = 0;
        goto out_done1;
    }
    outValue = value;
out_done1:
    locals.sp18 = outValue;

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

    ((cReadBlock *)locals.rb)
        ->ReadBase(cMemPool::GetPoolFromPtr((char *)this + 0x58),
                   (cBase *)base, *(cBase **)&locals.sp18);

    {
        int newValue = locals.sp18;
        cFile *readFile;
        if (newValue != 0) {
            readFile = *(cFile **)&locals.rb[0];
        } else {
            readFile = *(cFile **)&locals.rb[0];
            newValue = base | 1;
        }
        *(int *)((char *)this + 0x58) = newValue;

        cFileSystem::Read(*(cFileHandle **)readFile, (char *)this + 0x5C, 4);
    }

    int version = locals.rb[3];
    cFile *versionFile = *(cFile **)&locals.rb[0];
    if ((unsigned int)version < 7) {
        cFileSystem::Read(*(cFileHandle **)versionFile, &locals.oldMode, 1);
        int mode = 0;
        int oldFlag = locals.oldMode != 0;
        if (oldFlag != 0) {
            mode = 2;
        }
        *(int *)((char *)this + 0x60) = mode;
    } else {
        cFileSystem::Read(*(cFileHandle **)versionFile, (char *)this + 0x60,
                          4);
    }

    children->SetSize(4);
    children->SetSize(4);
    __0oKcReadBlockdtv(locals.rb, 2);
    return result;
}
