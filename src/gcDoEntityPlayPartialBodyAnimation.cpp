// gcDoEntityPlayPartialBodyAnimation.cpp - gcAll_psp.obj

inline void *operator new(unsigned int, void *p) { return p; }

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

class cReadBlock {
public:
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
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

    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcDoEntityPlayPartialBodyAnimation : public gcAction {
public:
    gcDoEntityPlayPartialBodyAnimation(cBase *);

    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void GetText(char *) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    gcDoEntityPlayPartialBodyAnimation &
    operator=(const gcDoEntityPlayPartialBodyAnimation &);
};

struct AllocRec {
    short offset;
    short _pad;
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

struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
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

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

extern char D_00000338[];
extern char D_00002B38[];
extern const char gcDoEntityPlayPartialBodyAnimation_base_name[];
extern const char gcDoEntityPlayPartialBodyAnimation_base_desc[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntityPlayPartialBodyAnimation;

cBase *gcDoEntityPlayPartialBodyAnimation::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoEntityPlayPartialBodyAnimation *result = 0;
    gcDoEntityPlayPartialBodyAnimation *obj =
        (gcDoEntityPlayPartialBodyAnimation *)rec->fn(base, 0x80, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEntityPlayPartialBodyAnimation(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoEntityPlayPartialBodyAnimation::GetType(void) const {
    if (!type_gcDoEntityPlayPartialBodyAnimation) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoEntityPlayPartialBodyAnimation_base_name,
                        gcDoEntityPlayPartialBodyAnimation_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntityPlayPartialBodyAnimation = cType::InitializeType(
            0, 0, 0x10C, type_action,
            gcDoEntityPlayPartialBodyAnimation::New, 0, 0, 0x80);
    }
    return type_gcDoEntityPlayPartialBodyAnimation;
}

void gcDoEntityPlayPartialBodyAnimation::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoEntityPlayPartialBodyAnimation) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoEntityPlayPartialBodyAnimation_base_name,
                            gcDoEntityPlayPartialBodyAnimation_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoEntityPlayPartialBodyAnimation = cType::InitializeType(
                0, 0, 0x10C, type_action,
                gcDoEntityPlayPartialBodyAnimation::New, 0, 0, 0x80);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoEntityPlayPartialBodyAnimation;
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
    *this = *(const gcDoEntityPlayPartialBodyAnimation *)copy;
}

gcDoEntityPlayPartialBodyAnimation::gcDoEntityPlayPartialBodyAnimation(
    cBase *parent) {
    gcAction_gcAction(this, parent);

    int *obj_i = (int *)this;
    obj_i[1] = (int)D_00002B38;

    void *desired0 = (char *)this + 0x0C;
    gcDesiredObject_gcDesiredObject(desired0, (cBase *)this);

    obj_i[4] = (int)D_00000338;

    gcDesiredEntityHelper_ctor((char *)this + 0x18, 1, 0, 0);

    obj_i[4] = 0x388A48;
    obj_i[8] = (int)desired0;
    obj_i[9] = 0x388568;
    ((char *)this)[0x28] = 1;
    ((char *)this)[0x29] = 0;
    obj_i[11] = 0;
    int desired0Encoded = (int)desired0 | 1;
    obj_i[12] = 0;
    obj_i[13] = desired0Encoded;
    obj_i[14] = (int)this;
    obj_i[15] = 0x388568;
    ((char *)this)[0x40] = 1;
    ((char *)this)[0x41] = 0;
    obj_i[17] = 0;
    int encoded = (int)this | 1;
    obj_i[18] = 0;
    obj_i[19] = encoded;
    obj_i[20] = 0;
    obj_i[21] = (int)this;
    obj_i[22] = 0x388568;
    ((char *)this)[0x5C] = 1;
    ((char *)this)[0x5D] = 0;
    obj_i[24] = 0;
    obj_i[25] = 0;
    obj_i[26] = encoded;

    gcDesiredObject_gcDesiredObject((char *)this + 0x6C, (cBase *)this);

    obj_i[30] = 7;
    obj_i[31] = 0;
    obj_i[28] = 0x389130;
}

void gcDoEntityPlayPartialBodyAnimation::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    gcAction::Write(file);

    char *typeInfo0 = *(char **)((const char *)this + 0x10);
    WriteRec *rec0 = (WriteRec *)(typeInfo0 + 0x28);
    short off0 = rec0->offset;
    void *base0 = (char *)this + 0x0C;
    rec0->fn((char *)base0 + off0, wb._file);

    char *typeInfo1 = *(char **)((const char *)this + 0x3C);
    WriteRec *rec1 = (WriteRec *)(typeInfo1 + 0x28);
    short off1 = rec1->offset;
    void *base1 = (char *)this + 0x38;
    rec1->fn((char *)base1 + off1, wb._file);

    char *typeInfo2 = *(char **)((const char *)this + 0x58);
    WriteRec *rec2 = (WriteRec *)(typeInfo2 + 0x28);
    short off2 = rec2->offset;
    void *base2 = (char *)this + 0x54;
    rec2->fn((char *)base2 + off2, wb._file);

    wb.Write(*(int *)((const char *)this + 0x50));

    char *typeInfo3 = *(char **)((const char *)this + 0x70);
    WriteRec *rec3 = (WriteRec *)(typeInfo3 + 0x28);
    short off3 = rec3->offset;
    void *base3 = (char *)this + 0x6C;
    rec3->fn((char *)base3 + off3, wb._file);

    wb.End();
}

int gcDoEntityPlayPartialBodyAnimation::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 4, true);
    if ((unsigned int)rb[3] != 4 ||
        ((gcAction *)this)->Read(file, pool) == 0) {
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

    char *typeInfo1 = *(char **)((char *)this + 0x3C);
    char *base1 = (char *)this + 0x38;
    ReadRec *rec1 = (ReadRec *)(typeInfo1 + 0x30);
    short off1 = rec1->offset;
    cFile *f1 = *(cFile **)&rb[0];
    rec1->fn(base1 + off1, f1, cMemPool::GetPoolFromPtr(base1));

    char *typeInfo2 = *(char **)((char *)this + 0x58);
    char *base2 = (char *)this + 0x54;
    ReadRec *rec2 = (ReadRec *)(typeInfo2 + 0x30);
    short off2 = rec2->offset;
    cFile *f2 = *(cFile **)&rb[0];
    rec2->fn(base2 + off2, f2, cMemPool::GetPoolFromPtr(base2));

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x50, 4);

    char *typeInfo3 = *(char **)((char *)this + 0x70);
    char *base3 = (char *)this + 0x6C;
    ReadRec *rec3 = (ReadRec *)(typeInfo3 + 0x30);
    short off3 = rec3->offset;
    cFile *f3 = *(cFile **)&rb[0];
    rec3->fn(base3 + off3, f3, cMemPool::GetPoolFromPtr(base3));

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

void gcDoEntityPlayPartialBodyAnimation::GetText(char *buf) const {
    register const gcDoEntityPlayPartialBodyAnimation *self __asm__("$16") =
        this;
    register char *out __asm__("$17") = buf;
    char text[256];
    char branchText[256];
    char objectText[256];

    char *typeInfo0 = *(char **)((const char *)self + 0x10);
    GetTextSlot *slot0 = (GetTextSlot *)(typeInfo0 + 0x78);
    char *sub0 = (char *)self + 0x0C;
    slot0->fn(sub0 + slot0->offset, out);

    text[0] = '\0';
    char *textStart = text;
    *textStart = '\0';
    bool helperFlag = false;
    ((const gcDesiredEnumerationEntryHelper *)((const char *)self + 0x40))
        ->GetText(text, helperFlag);

    int branchOwned = 0;
    int branch = *(int *)((const char *)self + 0x4C);
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

        branch = *(int *)((const char *)self + 0x4C);
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
            GetTextSlot *slot = (GetTextSlot *)(*(char **)(check + 4) + 0xD0);
            slot->fn((char *)branch + slot->offset, text);
        } else {
            cStrCat(text, (const char *)0x36DB24);
        }
    }

    if (*(int *)((const char *)self + 0x50) == 0) {
        branchText[0] = '\0';
        char *branchStart = branchText;
        *branchStart = '\0';
        bool helperFlag2 = false;
        ((const gcDesiredEnumerationEntryHelper *)((const char *)self + 0x5C))
            ->GetText(branchText, helperFlag2);

        int branchOwned2 = 0;
        int branch2 = *(int *)((const char *)self + 0x68);
        int branchTagged2 = branch2 & 1;
        if (branchTagged2 != 0) {
            branchOwned2 = 1;
        }
        int branchValid2;
        if (branchOwned2 != 0) {
            branchValid2 = 0;
        } else {
            branchValid2 = branch2 != 0;
            branchValid2 &= 0xFF;
            branchValid2 = branchValid2 != 0;
        }

        if (branchValid2 != 0) {
            cStrAppend(branchText, (const char *)0x36DCBC);

            branch2 = *(int *)((const char *)self + 0x68);
            int owned3 = 0;
            if (branch2 & 1) {
                owned3 = 1;
            }
            if (owned3 != 0) {
                branch2 = 0;
            } else {
                __asm__ volatile("" ::: "memory");
            }

            int check2 = branch2;
            if (check2 != 0) {
                GetTextSlot *slot =
                    (GetTextSlot *)(*(char **)(check2 + 4) + 0xD0);
                slot->fn((char *)branch2 + slot->offset, branchText);
            } else {
                cStrCat(branchText, (const char *)0x36DB24);
            }
        }

        cStrAppend(out, (const char *)0x36E868, text, branchText);
    } else {
        char *typeInfo1 = *(char **)((const char *)self + 0x70);
        objectText[0] = '\0';
        register char *objectOut __asm__("$18") = objectText;
        GetTextSlot *slot1 = (GetTextSlot *)(typeInfo1 + 0x40);
        char *sub1 = (char *)self + 0x6C;
        slot1->fn(sub1 + slot1->offset, objectOut);
        cStrAppend(out, (const char *)0x36E88C, text, objectOut);
    }
}
