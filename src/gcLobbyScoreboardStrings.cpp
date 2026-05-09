// gcLobbyScoreboardStrings — gcAll_psp.obj
//   Class hierarchy: cBase -> gcStringValue -> gcLobbyScoreboardStrings

class cBase {
public:
    int _b0;
    int _b4;
};

class cMemPool;
class cType;
class cFileHandle;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFile {
public:
    void SetCurrentPos(unsigned int);
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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

class gcStringValue : public cBase {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

extern char gcLobbyScoreboardStringsvirtualtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcLobbyScoreboardStrings : public gcStringValue {
public:
    int mValue0;  // 0x08 - tagged self-pointer
    int mValue1;  // 0x0C - tagged self-pointer
    int mValue2;  // 0x10
    int mValue3;  // 0x14
    int mValue4;  // 0x18 - tagged self-pointer

    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    gcLobbyScoreboardStrings &operator=(const gcLobbyScoreboardStrings &);
};

extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F4FC;

struct gcLobbyScoreboardStringsTypeNode {
    char pad[0x1C];
    gcLobbyScoreboardStringsTypeNode *parent;
};

struct gcLobbyScoreboardStringsVTableSlot {
    short offset;
    short pad;
    const cType *(*getType)(void *);
};

void gcLobbyScoreboardStrings::AssignCopy(const cBase *base) {
    const cBase *other = 0;
    if (base != 0) {
        if (D_0009F4FC == 0) {
            if (D_0009F454 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009F4FC = cType::InitializeType(0, 0, 0x125, D_0009F454,
                                               &gcLobbyScoreboardStrings::New,
                                               0, 0, 0);
        }

        void *vt = ((void **)base)[1];
        const cType *myType = D_0009F4FC;
        gcLobbyScoreboardStringsVTableSlot *slot =
            (gcLobbyScoreboardStringsVTableSlot *)((char *)vt + 8);
        short typeOff = slot->offset;
        const cType *(*getType)(void *) = slot->getType;
        void *type = (void *)getType((char *)base + typeOff);
        int ok;
        if (myType == 0) {
            goto fail;
        }
        if (type != 0) {
        loop:
            if (type != myType) {
                type = *(void **)((char *)type + 0x1C);
                if (type == 0) {
                    goto fail;
                }
                goto loop;
            }
            ok = 1;
        } else {
fail:
            ok = 0;
        }
        if (ok != 0) {
            other = base;
        }
    }
    *this = *(const gcLobbyScoreboardStrings *)other;
}

const cType *gcLobbyScoreboardStrings::GetType(void) const {
    if (D_0009F4FC == 0) {
        if (D_0009F454 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F4FC = cType::InitializeType(0, 0, 0x125, D_0009F454,
                                           &gcLobbyScoreboardStrings::New,
                                           0, 0, 0);
    }
    return D_0009F4FC;
}

void gcLobbyScoreboardStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcStringValue::Write(file);
    ((const gcDesiredValue *)((const char *)this + 8))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 12))->Write(wb);
    wb.Write(mValue2);
    int value = mValue4;
    int flag = 0;
    if (value & 1) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)value;
    }
    wb.WriteBase(ptr);
    wb.Write(mValue3);
    wb.End();
}

int gcLobbyScoreboardStrings::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);

    if (rb[3] != 1 || gcStringValue::Read(file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    ((gcDesiredValue *)((char *)this + 0x08))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0x0C))->Read(*(cReadBlock *)rb);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x10, 4);

    int sp14;
    int value = *(int *)((char *)this + 0x18);
    int tag = value & 1;
    int flag = 0;
    if (tag != 0) {
        flag = 1;
    }

    int outValue;
    if (flag != 0) {
        outValue = 0;
        goto out_done;
    }
    outValue = value;
out_done:
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

    cMemPool *childPool = cMemPool::GetPoolFromPtr((char *)this + 0x18);
    ((cReadBlock *)rb)->ReadBase(childPool, (cBase *)base, *(cBase **)&sp14);

    int newValue;
    if (sp14 == 0) {
        newValue = base | 1;
    } else {
        newValue = sp14;
    }
    cFile *f = *(cFile **)&rb[0];
    *(int *)((char *)this + 0x18) = newValue;
    cFileSystem::Read(*(cFileHandle **)f, (char *)this + 0x14, 4);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

cBase *gcLobbyScoreboardStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    void *base = (char *)block + entry->offset;
    gcLobbyScoreboardStrings *result = 0;
    gcLobbyScoreboardStrings *obj =
        (gcLobbyScoreboardStrings *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        ((int *)obj)[1] = 0x37E6A8;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcLobbyScoreboardStringsvirtualtable;
        int tagged = (int)obj | 1;
        obj->mValue0 = tagged;
        obj->mValue1 = tagged;
        obj->mValue2 = 0;
        obj->mValue3 = 0;
        obj->mValue4 = tagged;
        result = obj;
    }
    return (cBase *)result;
}
