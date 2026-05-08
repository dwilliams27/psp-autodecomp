// gcDoEntityAssignPath.cpp - gcAll_psp.obj

class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cReadBlock;
class cType;

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
    void Write(unsigned int);
    void Write(float);
    void End(void);
};

class gcDesiredValue {
public:
    void Read(cReadBlock &);
    void Write(cWriteBlock &) const;
};

class gcDesiredUIWidgetHelper {
public:
    void GetText(char *) const;
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    static cBase *New(cMemPool *, cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
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

struct GetTextRec {
    short offset;
    short _pad;
    void (*fn)(void *, char *);
};

struct GetTextSlot {
    short offset;
    short _pad;
    void (*fn)(void *, char *);
};

class gcDoEntityAssignPath : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void GetText(char *) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    gcDoEntityAssignPath &operator=(const gcDoEntityAssignPath &);
};

class gcDoUISetTextSprite : public gcAction {
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

struct cTypeNode {
    char pad[0x1C];
    const cType *parent;
};

struct VTableSlot {
    short offset;
    short _pad;
    const cType *(*getType)(void *);
};

extern "C" {
void gcAction_ctor_cBase(void *, cBase *);
void gcDesiredObject_ctor_cBase(void *, cBase *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);
void gcEvent_ctor(void *, cBase *, const char *)
    __asm__("__0oHgcEventctP6FcBasePCc");
void cFile_SetCurrentPos(void *, unsigned int);
void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
void __0oKcReadBlockdtv(void *, int);
}

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

extern char D_00000338[];
extern char D_00000518[];
extern char D_000025F0[];

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoEntityAssignPath asm("D_0009F614");

cBase *gcDoEntityAssignPath::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntityAssignPath *result = 0;
    gcDoEntityAssignPath *obj =
        (gcDoEntityAssignPath *)entry->fn(base, 0x78, 4, 0, 0);
    if (obj != 0) {
        gcAction_ctor_cBase(obj, parent);

        int *obj_i = (int *)obj;
        obj_i[1] = (int)D_000025F0;
        obj_i[3] = 6;

        void *desobj = (char *)obj + 0x10;
        gcDesiredObject_ctor_cBase(desobj, (cBase *)obj);

        obj_i[5] = (int)D_00000338;

        gcDesiredEntityHelper_ctor((char *)obj + 0x1C, 1, 0, 0);

        obj_i[5] = 0x388A48;
        obj_i[10] = 0x37E6A8;
        obj_i[9] = (int)desobj;
        obj_i[10] = 0x388568;
        ((char *)obj)[0x2C] = 1;
        ((char *)obj)[0x2D] = 0;
        obj_i[12] = 0;
        obj_i[13] = 0;
        obj_i[14] = (int)desobj | 1;

        gcDesiredObject_ctor_cBase((char *)obj + 0x3C, (cBase *)obj);

        obj_i[16] = (int)D_00000518;
        obj_i[18] = 7;
        obj_i[19] = 0;
        obj_i[16] = 0x3896C0;
        int encoded = (int)obj | 1;
        obj_i[20] = encoded;
        obj_i[21] = encoded;

        gcEvent_ctor((char *)obj + 0x58, (cBase *)obj, (const char *)0x36DB58);

        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoEntityAssignPath::GetType(void) const {
    if (!type_gcDoEntityAssignPath) {
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
        type_gcDoEntityAssignPath = cType::InitializeType(
            0, 0, 0x7D, type_action, gcDoEntityAssignPath::New, 0, 0, 0);
    }
    return type_gcDoEntityAssignPath;
}

void gcDoEntityAssignPath::GetText(char *buf) const {
    char text[256];

    char *typeInfo0 = *(char **)((const char *)this + 0x14);
    GetTextRec *rec0 = (GetTextRec *)(typeInfo0 + 0x78);
    char *base0 = (char *)this + 0x10;
    rec0->fn(base0 + rec0->offset, buf);

    int flag = (*(int *)((const char *)this + 0x0C) & 8) != 0;
    flag &= 0xFF;
    if (flag != 0) {
        cStrCat(buf, (const char *)0x36E694);
        return;
    }

    char *typeInfo1 = *(char **)((const char *)this + 0x40);
    text[0] = '\0';
    GetTextRec *rec1 = (GetTextRec *)(typeInfo1 + 0x78);
    char *base1 = (char *)this + 0x3C;
    rec1->fn(base1 + rec1->offset, text);

    cStrAppend(buf, (const char *)0x36E6A8, text);

    int empty = *(int *)((const char *)this + 0x60) == 0;
    empty &= 0xFF;
    if (empty == 0) {
        cStrAppend(buf, (const char *)0x36E6B8);
    }
    cStrAppend(buf, (const char *)0x36DCEC);
}

void gcDoEntityAssignPath::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoEntityAssignPath) {
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
            type_gcDoEntityAssignPath = cType::InitializeType(
                0, 0, 0x7D, type_action,
                gcDoEntityAssignPath::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoEntityAssignPath;
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
    *this = *(const gcDoEntityAssignPath *)copy;
}

void gcDoEntityAssignPath::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcAction::Write(file);
    wb.Write(*(const unsigned int *)((const char *)this + 0x0C));

    {
        char *typeInfo = *(char **)((const char *)this + 0x14);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        short off = rec->offset;
        void *base = (void *)((const char *)this + 0x10);
        rec->fn((char *)base + off, wb._file);
    }

    {
        char *typeInfo = *(char **)((const char *)this + 0x40);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        short off = rec->offset;
        void *base = (void *)((const char *)this + 0x3C);
        rec->fn((char *)base + off, wb._file);
    }

    ((const gcDesiredValue *)((const char *)this + 0x50))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x54))->Write(wb);

    {
        char *typeInfo = *(char **)((const char *)this + 0x5C);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        short off = rec->offset;
        void *base = (void *)((const char *)this + 0x58);
        rec->fn((char *)base + off, wb._file);
    }

    wb.Write(*(const float *)((const char *)this + 0x74));
    wb.End();
}

int gcDoEntityAssignPath::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 3, true);
    if ((unsigned int)rb[3] >= 4 || (unsigned int)rb[3] < 2 ||
        ((gcAction *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x0C, 4);

    char *typeInfo0 = *(char **)((char *)this + 0x14);
    char *base0 = (char *)this + 0x10;
    ReadRec *rec0 = (ReadRec *)(typeInfo0 + 0x30);
    short off0 = rec0->offset;
    cFile *f0 = *(cFile **)&rb[0];
    rec0->fn(base0 + off0, f0, cMemPool::GetPoolFromPtr(base0));

    char *typeInfo1 = *(char **)((char *)this + 0x40);
    char *base1 = (char *)this + 0x3C;
    ReadRec *rec1 = (ReadRec *)(typeInfo1 + 0x30);
    short off1 = rec1->offset;
    cFile *f1 = *(cFile **)&rb[0];
    rec1->fn(base1 + off1, f1, cMemPool::GetPoolFromPtr(base1));

    ((gcDesiredValue *)((char *)this + 0x50))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0x54))->Read(*(cReadBlock *)rb);

    char *typeInfo2 = *(char **)((char *)this + 0x5C);
    char *base2 = (char *)this + 0x58;
    ReadRec *rec2 = (ReadRec *)(typeInfo2 + 0x30);
    short off2 = rec2->offset;
    cFile *f2 = *(cFile **)&rb[0];
    rec2->fn(base2 + off2, f2, cMemPool::GetPoolFromPtr(base2));

    if ((unsigned int)rb[3] >= 3) {
        cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x74, 4);
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

void gcDoUISetTextSprite::GetText(char *buf) const {
    char text[256];
    text[0] = *text = '\0';

    ((const gcDesiredUIWidgetHelper *)((const char *)this + 0x0C))->GetText(text);
    cStrAppend(buf, (const char *)0x36F108, text);

    int val = *(int *)((const char *)this + 0x18);
    int owned = 0;
    if (val & 1) {
        owned = 1;
    }
    if (owned != 0) {
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

    cStrCat(buf, (const char *)0x36DAD8);

    char *typeInfo2 = *(char **)((const char *)this + 0x20);
    GetTextRec *rec2 = (GetTextRec *)(typeInfo2 + 0x78);
    char *base2 = (char *)this + 0x1C;
    rec2->fn(base2 + rec2->offset, buf);

    cStrCat(buf, (const char *)0x36DCEC);
}
