#include "cBase.h"

class cFile;
class cFileHandle;
class cMemPool;
class cReadBlock;

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
};

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void End(void);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char _pad0[0x28];
    cTypeMethod write_m;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDesiredObject {
public:
    int _parent;
    cType *mType;
};

class gcDesiredValue {
public:
    void Read(cReadBlock &);
    void Write(cWriteBlock &) const;
};

class gcValue {
public:
    void Write(cFile &) const;
};

class gcLValue : public gcValue {
public:
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcValEntitySoundFrequency : public gcLValue {
public:
    void GetText(char *) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
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

struct GetTextSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

struct ReadRec {
    short offset;
    short pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

class cStr {
public:
    char _data[256];
};

void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);
extern "C" char *cStr_ctor(void *, const char *, ...) __asm__("__0oEcStrctPCce");
void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

cBase *gcValEntitySoundFrequency::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValEntitySoundFrequency *result = 0;
    gcValEntitySoundFrequency *obj =
        (gcValEntitySoundFrequency *)entry->fn(base, 0x3C, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = (void *)0x37E6A8;
        {
            register void *type0 asm("a0");
            __asm__ volatile("lui %0,0x0" : "=r"(type0));
            ((cBase **)obj)[0] = parent;
            __asm__ volatile("addiu %0,%0,0x7e38" : "+r"(type0));
            ((void **)obj)[1] = type0;
        }
        char *sub = (char *)obj + 8;
        gcDesiredObject_ctor(sub, obj);
        {
            register void *type1 asm("a0");
            __asm__ volatile("lui %0,0x0\n\taddiu %0,%0,0x338"
                             : "=r"(type1));
            ((void **)obj)[3] = type1;
        }
        gcDesiredEntityHelper_ctor((char *)obj + 0x14, 1, 0, 0);
        {
            register void *helper_desc asm("a0");
            __asm__ volatile("lui %0,0x39\n\taddiu %0,%0,-0x75b8"
                             : "=r"(helper_desc));
            ((void **)obj)[8] = (void *)0x37E6A8;
            ((void **)obj)[3] = helper_desc;
        }
        ((char **)obj)[7] = sub;
        ((void **)obj)[8] = (void *)0x388568;
        ((char *)obj)[0x24] = 1;
        ((char *)obj)[0x25] = 0;
        ((int *)obj)[10] = 0;
        int sub_or_1 = (int)sub | 1;
        ((int *)obj)[11] = 0;
        int obj_or_1 = (int)obj | 1;
        ((int *)obj)[12] = sub_or_1;
        ((int *)obj)[13] = obj_or_1;
        {
            register int one asm("a0");
            __asm__ volatile("ori %0,$zero,0x1" : "=r"(one));
            ((char *)obj)[0x38] = 0;
            ((char *)obj)[0x39] = one;
            ((char *)obj)[0x3A] = one;
        }
        result = obj;
    }
    return (cBase *)result;
}

void gcValEntitySoundFrequency::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcLValue::Write(file);

    const cTypeMethod *entity_write =
        &((const gcDesiredObject *)((const char *)this + 8))->mType->write_m;
    char *entity_base = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)entity_write->fn)(entity_base + entity_write->offset, wb.file);

    ((const gcDesiredValue *)((const char *)this + 0x34))->Write(wb);
    wb.Write(*(const bool *)((const char *)this + 0x38));
    wb.Write(*(const bool *)((const char *)this + 0x39));
    wb.Write(*(const bool *)((const char *)this + 0x3A));
    wb.End();
}

int gcValEntitySoundFrequency::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 3, true);
    if ((unsigned int)rb[3] >= 4 || (unsigned int)rb[3] < 1 ||
        gcLValue::Read(file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    char *typeInfo = *(char **)((char *)this + 0x0C);
    char *base = (char *)this + 0x08;
    ReadRec *rec = (ReadRec *)(typeInfo + 0x30);
    short off = rec->offset;
    cFile *f = *(cFile **)&rb[0];
    rec->fn(base + off, f, cMemPool::GetPoolFromPtr(base));

    ((gcDesiredValue *)((char *)this + 0x34))->Read(*(cReadBlock *)rb);

    if ((unsigned int)rb[3] >= 2) {
        char value0;
        cFileSystem::Read(*(cFileHandle **)rb[0], &value0, 1);
        int flag0 = value0 != 0;
        *(char *)((char *)this + 0x38) = flag0;
    }

    if ((unsigned int)rb[3] >= 3) {
        char value1;
        char value2;
        cFileSystem::Read(*(cFileHandle **)rb[0], &value1, 1);
        cFile *f2 = *(cFile **)&rb[0];
        *(char *)((char *)this + 0x39) = value1 != 0;

        cFileSystem::Read(*(cFileHandle **)f2, &value2, 1);
        *(char *)((char *)this + 0x3A) = value2 != 0;
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_lvalue;
static cType *type_gcValEntitySoundFrequency;

const cType *gcValEntitySoundFrequency::GetType(void) const {
    if (!type_gcValEntitySoundFrequency) {
        if (!type_lvalue) {
            if (!type_value) {
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
                type_value = cType::InitializeType(
                    0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
            }
            type_lvalue = cType::InitializeType(
                0, 0, 0x6D, type_value, 0, 0, 0, 0);
        }
        type_gcValEntitySoundFrequency = cType::InitializeType(
            0, 0, 0x1E2, type_lvalue,
            gcValEntitySoundFrequency::New, 0, 0, 0);
    }
    return type_gcValEntitySoundFrequency;
}

void gcValEntitySoundFrequency::GetText(char *buf) const {
    cTypeMethod *entityText =
        (cTypeMethod *)((char *)((gcDesiredObject *)((char *)this + 8))->mType +
                        0x78);
    char *entityBase = (char *)this + 8;
    ((void (*)(void *, char *))entityText->fn)(entityBase + entityText->offset,
                                               buf);

    const char *fmt = (const char *)0x36F3B0;
    const char *state;
    if (*(unsigned char *)((char *)this + 0x38) != 0) {
        state = (const char *)0x36F3C0;
    } else if (*(unsigned char *)((char *)this + 0x39) != 0) {
        state = (const char *)0x36F3C8;
    } else {
        state = (const char *)0x36F3D0;
    }

    cStr text;
    char *built = cStr_ctor(&text, fmt, state);
    __asm__ volatile("" : "+r"(built));
    cStrAppend(buf, built);

    int val = *(int *)((char *)this + 0x34);
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    if (flag != 0) {
        val = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }

    int check = val;
    if (check != 0) {
        char *typeInfo = *(char **)(check + 4);
        GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0xD0);
        slot->fn((char *)val + slot->offset, buf);
    } else {
        cStrCat(buf, (const char *)0x36DB24);
    }

    cStrAppend(buf, (const char *)0x36E2E8);
}
