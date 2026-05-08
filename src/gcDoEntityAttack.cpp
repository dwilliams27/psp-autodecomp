#include "cBase.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

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

class cReadBlock {
public:
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
    void Read(cReadBlock &);
    void Write(cWriteBlock &) const;
};

struct DesiredReadEntry {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
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

class gcDoEntityAttack : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    gcDoEntityAttack &operator=(const gcDoEntityAttack &);
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    void GetText(char *) const;
    void Write(cFile &) const;
};

extern const char gcDoEntityAttack_base_name[];
extern const char gcDoEntityAttack_base_desc[];

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

extern char D_00000338[];
extern char D_00000478[];
extern char D_00002268[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntityAttack;

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

cBase *gcDoEntityAttack::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntityAttack *result = 0;
    gcDoEntityAttack *obj =
        (gcDoEntityAttack *)entry->fn(base, 0x70, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);

        int *obj_i = (int *)obj;
        obj_i[1] = (int)D_00002268;

        void *desired0 = (char *)obj + 0x0C;
        gcDesiredObject_gcDesiredObject(desired0, (cBase *)obj);

        obj_i[4] = (int)D_00000338;

        gcDesiredEntityHelper_ctor((char *)obj + 0x18, 1, 0, 0);

        obj_i[4] = 0x388A48;
        obj_i[9] = 0x37E6A8;
        obj_i[8] = (int)desired0;
        obj_i[9] = 0x388568;
        ((char *)obj)[0x28] = 1;
        ((char *)obj)[0x29] = 0;
        obj_i[11] = 0;
        int desired0Encoded = (int)desired0 | 1;
        __asm__ volatile("" : "+r"(desired0Encoded));
        obj_i[12] = 0;
        obj_i[13] = desired0Encoded;
        obj_i[16] = 0x37E6A8;
        obj_i[14] = 0;
        obj_i[15] = (int)obj;
        obj_i[16] = 0x388568;
        ((char *)obj)[0x44] = 1;
        ((char *)obj)[0x45] = 0;
        obj_i[18] = 0;
        int encoded = (int)obj | 1;
        obj_i[19] = 0;
        obj_i[20] = encoded;

        gcDesiredObject_gcDesiredObject((char *)obj + 0x54, (cBase *)obj);

        obj_i[22] = (int)D_00000478;
        obj_i[24] = 7;
        obj_i[25] = 0;
        obj_i[22] = 0x389270;
        obj_i[26] = encoded;
        obj_i[27] = 0;

        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoEntityAttack::GetType(void) const {
    if (!type_gcDoEntityAttack) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcDoEntityAttack_base_name, gcDoEntityAttack_base_desc, 1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntityAttack = cType::InitializeType(0, 0, 0x148, type_action, gcDoEntityAttack::New, 0, 0, 0x80);
    }
    return type_gcDoEntityAttack;
}

void gcDoEntityAttack::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoEntityAttack) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoEntityAttack_base_name,
                            gcDoEntityAttack_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoEntityAttack = cType::InitializeType(
                0, 0, 0x148, type_action,
                gcDoEntityAttack::New, 0, 0, 0x80);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoEntityAttack;
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
    *this = *(const gcDoEntityAttack *)copy;
}

void gcDoEntityAttack::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    gcAction::Write(file);

    char *typeInfo0 = *(char **)((const char *)this + 0x10);
    WriteRec *rec0 = (WriteRec *)(typeInfo0 + 0x28);
    short off0 = rec0->offset;
    void *base0 = (char *)this + 0x0C;
    rec0->fn((char *)base0 + off0, wb._file);

    ((const gcDesiredValue *)((const char *)this + 0x68))->Write(wb);

    char *typeInfo1 = *(char **)((const char *)this + 0x40);
    WriteRec *rec1 = (WriteRec *)(typeInfo1 + 0x28);
    short off1 = rec1->offset;
    void *base1 = (char *)this + 0x3C;
    rec1->fn((char *)base1 + off1, wb._file);

    wb.Write(*(int *)((const char *)this + 0x6C));
    wb.Write(*(int *)((const char *)this + 0x38));

    char *typeInfo2 = *(char **)((const char *)this + 0x58);
    WriteRec *rec2 = (WriteRec *)(typeInfo2 + 0x28);
    short off2 = rec2->offset;
    void *base2 = (char *)this + 0x54;
    rec2->fn((char *)base2 + off2, wb._file);

    wb.End();
}

int gcDoEntityAttack::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 4, true);
    if ((unsigned int)rb[3] >= 5 || (unsigned int)rb[3] < 3 ||
        ((gcAction *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    char *typeInfo0 = *(char **)((char *)this + 0x10);
    char *base0 = (char *)this + 0x0C;
    DesiredReadEntry *rec0 = (DesiredReadEntry *)(typeInfo0 + 0x30);
    short off0 = rec0->offset;
    cFile *f0 = *(cFile **)&rb[0];
    rec0->fn(base0 + off0, f0, cMemPool::GetPoolFromPtr(base0));

    ((gcDesiredValue *)((char *)this + 0x68))->Read(*(cReadBlock *)rb);

    char *typeInfo1 = *(char **)((char *)this + 0x40);
    char *base1 = (char *)this + 0x3C;
    DesiredReadEntry *rec1 = (DesiredReadEntry *)(typeInfo1 + 0x30);
    short off1 = rec1->offset;
    cFile *f1 = *(cFile **)&rb[0];
    rec1->fn(base1 + off1, f1, cMemPool::GetPoolFromPtr(base1));

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x6C, 4);

    if ((unsigned int)rb[3] >= 4) {
        cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x38, 4);

        char *typeInfo2 = *(char **)((char *)this + 0x58);
        char *base2 = (char *)this + 0x54;
        DesiredReadEntry *rec2 = (DesiredReadEntry *)(typeInfo2 + 0x30);
        short off2 = rec2->offset;
        cFile *f2 = *(cFile **)&rb[0];
        rec2->fn(base2 + off2, f2, cMemPool::GetPoolFromPtr(base2));
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

void gcDoEntityAttack::GetText(char *buf) const {
    register const gcDoEntityAttack *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    char text[256];
    char targetText[256];

    char *typeInfo0 = *(char **)((const char *)self + 0x10);
    GetTextSlot *slot0 = (GetTextSlot *)(typeInfo0 + 0x78);
    char *sub0 = (char *)self + 0x0C;
    slot0->fn(sub0 + slot0->offset, out);

    text[0] = '\0';
    targetText[0] = '\0';

    if (*(int *)((const char *)self + 0x38) == 0) {
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
    } else {
        char *typeInfo1 = *(char **)((const char *)self + 0x58);
        GetTextSlot *slot1 = (GetTextSlot *)(typeInfo1 + 0x40);
        char *sub1 = (char *)self + 0x54;
        slot1->fn(sub1 + slot1->offset, text);
    }

    int target = *(int *)((const char *)self + 0x68);
    int owned3 = 0;
    if (target & 1) {
        owned3 = 1;
    }
    if (owned3 != 0) {
        target = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }

    int targetCheck = target;
    if (targetCheck != 0) {
        GetTextSlot *slot2 = (GetTextSlot *)(*(char **)(targetCheck + 4) + 0xD0);
        slot2->fn((char *)target + slot2->offset, targetText);
    } else {
        cStrCat(targetText, (const char *)0x36DB24);
    }

    int mode = *(int *)((const char *)self + 0x6C);
    if (mode < 2) {
        if (mode < 0) {
            goto done;
        }
        if (mode > 0) {
            goto mode_one;
        }
        cStrAppend(out, (const char *)0x36E528, text);
        goto done;
    } else {
        if (mode < 3) {
            goto mode_two;
        }
        if (mode >= 4) {
            goto done;
        }
        cStrAppend(out, (const char *)0x36E53C, text);
        goto done;
    }

mode_one:
    cStrAppend(out, (const char *)0x36E554, text, targetText);
    goto done;

mode_two:
    cStrAppend(out, (const char *)0x36E578, targetText);

done:
    ;
}
