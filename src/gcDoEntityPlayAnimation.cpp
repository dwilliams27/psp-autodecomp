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
    void Write(int);
    void End(void);
};

class gcDesiredValue {
public:
    void Read(class cReadBlock &);
    void Write(cWriteBlock &) const;
};

class cReadBlock {
public:
};

struct WriteRec {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

struct ReadRec {
    short offset;
    short pad;
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
    const cType *parent;
};

struct VTableSlot {
    short offset;
    short _pad;
    const cType *(*getType)(void *);
};

struct GetTextSlot {
    short offset;
    short _pad;
    void (*fn)(void *, char *);
};

class gcDesiredEnumerationEntryHelper {
public:
    void GetText(char *, bool) const;
};

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

extern "C" {
void gcAction_ctor_cBase(void *, cBase *);
void gcDesiredObject_ctor_cBase(void *, cBase *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);
int gcAction_Read(gcAction *, cFile &, cMemPool *)
    asm("__0fIgcActionEReadR6FcFileP6IcMemPool");
void cFile_SetCurrentPos(void *, unsigned int);
void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
void __0oKcReadBlockdtv(void *, int);
}

extern char D_000002A58[];
extern char D_00000338[];
extern char D_000003D8[];
extern const char gcDoEntityPlayAnimation_base_name[];
extern const char gcDoEntityPlayAnimation_base_desc[];

class gcDoEntityPlayAnimation : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void GetText(char *) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    gcDoEntityPlayAnimation &operator=(const gcDoEntityPlayAnimation &);
};

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntityPlayAnimation;

cBase *gcDoEntityPlayAnimation::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntityPlayAnimation *result = 0;
    gcDoEntityPlayAnimation *obj =
        (gcDoEntityPlayAnimation *)entry->fn(base, 0x6C, 4, 0, 0);
    if (obj != 0) {
        gcAction_ctor_cBase(obj, parent);

        int *obj_i = (int *)obj;
        obj_i[1] = (int)D_000002A58;

        void *desobj = (char *)obj + 0x0C;
        gcDesiredObject_ctor_cBase(desobj, (cBase *)obj);

        obj_i[4] = (int)D_00000338;

        gcDesiredEntityHelper_ctor((char *)obj + 0x18, 1, 0, 0);

        obj_i[4] = 0x388A48;
        obj_i[9] = 0x37E6A8;
        obj_i[8] = (int)desobj;
        obj_i[9] = 0x388568;
        ((char *)obj)[0x28] = 1;
        ((char *)obj)[0x29] = 0;
        obj_i[11] = 0;
        __asm__ volatile("" ::: "memory");
        int encodedDesobj = (int)desobj | 1;
        obj_i[12] = 0;
        obj_i[13] = encodedDesobj;
        obj_i[16] = 0x37E6A8;
        obj_i[14] = 0;
        obj_i[15] = (int)obj;
        obj_i[16] = 0x388568;
        ((char *)obj)[0x44] = 1;
        ((char *)obj)[0x45] = 0;
        obj_i[18] = 0;
        int encodedObj = (int)obj | 1;
        obj_i[19] = 0;
        obj_i[20] = encodedObj;

        gcDesiredObject_ctor_cBase((char *)obj + 0x54, (cBase *)obj);

        obj_i[22] = (int)D_000003D8;
        obj_i[24] = 7;
        obj_i[25] = 0;
        obj_i[22] = 0x389130;
        obj_i[26] = encodedObj;

        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoEntityPlayAnimation::GetType(void) const {
    if (!type_gcDoEntityPlayAnimation) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoEntityPlayAnimation_base_name,
                        gcDoEntityPlayAnimation_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntityPlayAnimation = cType::InitializeType(
            0, 0, 0xC1, type_action, gcDoEntityPlayAnimation::New, 0, 0, 0x80);
    }
    return type_gcDoEntityPlayAnimation;
}

void gcDoEntityPlayAnimation::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoEntityPlayAnimation) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoEntityPlayAnimation_base_name,
                            gcDoEntityPlayAnimation_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoEntityPlayAnimation = cType::InitializeType(
                0, 0, 0xC1, type_action,
                gcDoEntityPlayAnimation::New, 0, 0, 0x80);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoEntityPlayAnimation;
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
    *this = *(const gcDoEntityPlayAnimation *)copy;
}

void gcDoEntityPlayAnimation::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    gcAction::Write(file);

    {
        char *typeInfo = *(char **)((char *)this + 0x10);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x0C;
        rec->fn(base + rec->offset, wb._file);
    }

    {
        char *typeInfo = *(char **)((char *)this + 0x40);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x3C;
        rec->fn(base + rec->offset, wb._file);
    }

    wb.Write(*(const int *)((const char *)this + 0x38));

    {
        char *typeInfo = *(char **)((char *)this + 0x58);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x54;
        rec->fn(base + rec->offset, wb._file);
    }

    ((const gcDesiredValue *)((const char *)this + 0x68))->Write(wb);
    wb.End();
}

int gcDoEntityPlayAnimation::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 5, true);
    if ((unsigned int)rb[3] != 5 ||
        gcAction_Read((gcAction *)this, file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    char *typeInfo0 = *(char **)((char *)this + 0x10);
    char *base0 = (char *)this + 0x0C;
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

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x38, 4);

    char *typeInfo2 = *(char **)((char *)this + 0x58);
    char *base2 = (char *)this + 0x54;
    ReadRec *rec2 = (ReadRec *)(typeInfo2 + 0x30);
    short off2 = rec2->offset;
    cFile *f2 = *(cFile **)&rb[0];
    rec2->fn(base2 + off2, f2, cMemPool::GetPoolFromPtr(base2));

    ((gcDesiredValue *)((char *)this + 0x68))->Read(*(cReadBlock *)rb);

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

void gcDoEntityPlayAnimation::GetText(char *buf) const {
    register const gcDoEntityPlayAnimation *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    char text[256];
    char objectText[256];

    char *typeInfo0 = *(char **)((const char *)self + 0x10);
    GetTextSlot *slot0 = (GetTextSlot *)(typeInfo0 + 0x78);
    char *sub0 = (char *)self + 0x0C;
    slot0->fn(sub0 + slot0->offset, out);

    int object;
    if (*(int *)((const char *)self + 0x38) == 0) {
        text[0] = '\0';
        char *textStart = text;
        *textStart = '\0';
        bool helperFlag = false;
        ((const gcDesiredEnumerationEntryHelper *)((const char *)self + 0x44))
            ->GetText(text, helperFlag);

        int branchOwned = 0;
        int branch = *(int *)((const char *)self + 0x50);
        int branchTagged = branch & 1;
        if (branchTagged != 0) {
            branchOwned = 1;
        }
        int branchValid;
        if (branchOwned != 0) {
            branchValid = 0;
        } else {
            branchValid = branch != 0;
            branchValid &= 0xFF;
            branchValid = branchValid != 0;
        }

        if (branchValid != 0) {
            cStrAppend(text, (const char *)0x36DCBC);

            branch = *(int *)((const char *)self + 0x50);
            int owned2 = 0;
            if (branch & 1) {
                owned2 = 1;
            }
            if (owned2 != 0) {
                branch = 0;
            } else {
                __asm__ volatile("" ::: "memory");
            }

            int check = branch;
            if (check != 0) {
                GetTextSlot *slot =
                    (GetTextSlot *)(*(char **)(check + 4) + 0xD0);
                slot->fn((char *)branch + slot->offset, text);
            } else {
                cStrCat(text, (const char *)0x36DB24);
            }
        }

        cStrAppend(out, (const char *)0x36E82C, text);
        object = *(int *)((const char *)self + 0x68);
    } else {
        char *typeInfo1 = *(char **)((const char *)self + 0x58);
        objectText[0] = '\0';
        register char *objectOut __asm__("$18") = objectText;
        GetTextSlot *slot1 = (GetTextSlot *)(typeInfo1 + 0x40);
        char *sub1 = (char *)self + 0x54;
        slot1->fn(sub1 + slot1->offset, objectOut);
        cStrAppend(out, (const char *)0x36E840, objectOut);
        object = *(int *)((const char *)self + 0x68);
    }

    int objectTagged = object & 1;
    int objectOwned = 0;
    if (objectTagged != 0) {
        objectOwned = 1;
    }
    if (objectOwned != 0) {
        object = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }

    int objectCheck = object;
    if (objectCheck != 0) {
        cStrCat(out, (const char *)0x36E85C);

        object = *(int *)((const char *)self + 0x68);
        int owned2 = 0;
        if (object & 1) {
            owned2 = 1;
        }
        if (owned2 != 0) {
            object = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int check = object;
        if (check != 0) {
            GetTextSlot *slot = (GetTextSlot *)(*(char **)(check + 4) + 0xD0);
            slot->fn((char *)object + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }

        cStrCat(out, (const char *)0x36E2E8);
    }

    cStrCat(out, (const char *)0x36DCEC);
}
