class cBase;
class cFile;
class cMemPool;

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
    void Write(unsigned int);
    void End(void);
};

class cName {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
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

    void Write(cFile &) const;
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

class cStr {
public:
    char _data[0x100];
};

class gcDesiredCamera {
public:
    void GetText(char *) const;
};

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);
extern "C" cStr *__0oEcStrctPCce(void *, const char *, ...);

class gcDoEntityMoveToPoint : public gcAction {
public:
    gcDoEntityMoveToPoint(cBase *);

    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void GetText(char *) const;
    void Write(cFile &) const;
    gcDoEntityMoveToPoint &operator=(const gcDoEntityMoveToPoint &);
};

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredCamera_gcDesiredCamera(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");

extern char D_00000338[];
extern char gcDoEntityMoveToPointvirtualtable[];

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoEntityMoveToPoint asm("D_0009F620");

void gcDoEntityMoveToPoint::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoEntityMoveToPoint) {
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
            type_gcDoEntityMoveToPoint = cType::InitializeType(
                0, 0, 0xDC, type_action,
                gcDoEntityMoveToPoint::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoEntityMoveToPoint;
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
    *this = *(const gcDoEntityMoveToPoint *)copy;
}

gcDoEntityMoveToPoint::gcDoEntityMoveToPoint(cBase *parent) {
    gcAction_gcAction(this, parent);
    mVTable = gcDoEntityMoveToPointvirtualtable;

    char *desired0 = (char *)this + 0x0C;
    gcDesiredObject_gcDesiredObject(desired0, (cBase *)this);

    void *entityType = D_00000338;
    *(void **)((char *)this + 0x10) = entityType;

    void *helper0 = (char *)this + 0x18;
    int one = 1;
    gcDesiredEntityHelper_ctor(helper0, 1, 0, 0);

    void *helperDesc = (void *)0x388A48;
    *(void **)((char *)this + 0x10) = helperDesc;
    *(void **)((char *)this + 0x20) = desired0;
    void *desiredVTable = (void *)0x388568;
    *(void **)((char *)this + 0x24) = desiredVTable;
    *(unsigned char *)((char *)this + 0x28) = one;
    *(unsigned char *)((char *)this + 0x29) = 0;
    *(int *)((char *)this + 0x2C) = 0;
    *(int *)((char *)this + 0x30) = 0;
    *(int *)((char *)this + 0x34) = (int)desired0 | 1;

    gcDesiredObject_gcDesiredObject((char *)this + 0x38, (cBase *)this);
    *(int *)((char *)this + 0x44) = 7;
    *(int *)((char *)this + 0x48) = 0;
    *(void **)((char *)this + 0x3C) = (void *)0x389760;

    char *desired1 = (char *)this + 0x4C;
    gcDesiredObject_gcDesiredObject(desired1, (cBase *)this);
    *(void **)((char *)this + 0x50) = entityType;

    gcDesiredEntityHelper_ctor((char *)this + 0x58, 1, 0, 0);

    *(void **)((char *)this + 0x50) = helperDesc;
    *(void **)((char *)this + 0x60) = desired1;
    *(void **)((char *)this + 0x64) = desiredVTable;
    *(unsigned char *)((char *)this + 0x68) = one;
    *(unsigned char *)((char *)this + 0x69) = 0;
    *(int *)((char *)this + 0x6C) = 0;
    *(int *)((char *)this + 0x70) = 0;
    *(int *)((char *)this + 0x74) = (int)desired1 | 1;

    gcDesiredCamera_gcDesiredCamera((char *)this + 0x78, (cBase *)this);

    int encoded = (int)this | 1;
    *(int *)((char *)this + 0xB0) = encoded;
    *(int *)((char *)this + 0xB4) = encoded;
    *(int *)((char *)this + 0xB8) = encoded;
    *(int *)((char *)this + 0xBC) = encoded;
    *(int *)((char *)this + 0xC0) = encoded;
    *(int *)((char *)this + 0xC4) = encoded;
    *(int *)((char *)this + 0xC8) = encoded;
    *(int *)((char *)this + 0xCC) = encoded;
    *(int *)((char *)this + 0xD0) = encoded;
    *(int *)((char *)this + 0xD4) = encoded;
    *(int *)((char *)this + 0xD8) = 0;
    *(int *)((char *)this + 0xDC) = one;
    *(int *)((char *)this + 0xE0) = 2;
    *(int *)((char *)this + 0xE4) = 0;
    *(short *)((char *)this + 0xFC) = 0;
    *(short *)((char *)this + 0xFE) = 0;
    *(unsigned char *)((char *)this + 0xE8) = 0;
    *(int *)((char *)this + 0x100) = encoded;
    *(unsigned char *)((char *)this + 0x104) = 0;
}

const cType *gcDoEntityMoveToPoint::GetType(void) const {
    if (!type_gcDoEntityMoveToPoint) {
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
        type_gcDoEntityMoveToPoint = cType::InitializeType(
            0, 0, 0xDC, type_action, gcDoEntityMoveToPoint::New, 0, 0, 0);
    }
    return type_gcDoEntityMoveToPoint;
}

void gcDoEntityMoveToPoint::Write(cFile &file) const {
    cWriteBlock wb(file, 7);
    gcAction::Write(file);

    {
        char *typeInfo = *(char **)((const char *)this + 0x10);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x0C;
        rec->fn(base + rec->offset, wb._file);
    }

    {
        char *typeInfo = *(char **)((const char *)this + 0x3C);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x38;
        rec->fn(base + rec->offset, wb._file);
    }

    {
        char *typeInfo = *(char **)((const char *)this + 0x50);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x4C;
        rec->fn(base + rec->offset, wb._file);
    }

    ((const gcDesiredValue *)((const char *)this + 0xB0))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xB4))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xB8))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xC0))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xC4))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xC8))->Write(wb);
    wb.Write(*(const unsigned int *)((const char *)this + 0xD8));
    wb.Write(*(const unsigned int *)((const char *)this + 0xE4));
    ((const cName *)((const char *)this + 0xE8))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x100))->Write(wb);
    wb.Write(*(const bool *)((const char *)this + 0x104));
    wb.Write(*(const unsigned int *)((const char *)this + 0xDC));
    wb.Write(*(const unsigned int *)((const char *)this + 0xE0));
    ((const gcDesiredValue *)((const char *)this + 0xBC))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xCC))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xD0))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xD4))->Write(wb);

    {
        char *typeInfo = *(char **)((const char *)this + 0x7C);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x78;
        rec->fn(base + rec->offset, wb._file);
    }

    wb.End();
}

void gcDoEntityMoveToPoint::GetText(char *buf) const {
    register const gcDoEntityMoveToPoint *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    char text[256];
    char temp[256];

    char *typeInfo0 = *(char **)((const char *)self + 0x10);
    int active = *(int *)((const char *)self + 0xE4) & 1;
    GetTextSlot *slot0 = (GetTextSlot *)(typeInfo0 + 0x78);
    char *sub0 = (char *)self + 0x0C;
    short off0 = slot0->offset;
    void (*fn0)(void *, char *) = slot0->fn;
    fn0(sub0 + off0, out);

    const char *fmtIntro = (const char *)0x36E738;
    const char *flagText;
    if (*(unsigned char *)((const char *)self + 0x104) != 0) {
        flagText = (const char *)0x36E740;
    } else {
        flagText = (const char *)0x36D944;
    }

    const char *activeText;
    if (active != 0) {
        activeText = (const char *)0x36E744;
    } else {
        activeText = (const char *)0x36E74C;
    }

    cStrAppend(out, fmtIntro, flagText, activeText);

    int mode = *(int *)((const char *)self + 0xD8);
    if (mode < 2) {
        if (mode < 0) {
            goto switch_done;
        }
        if (mode > 0) {
            goto mode_one;
        }

        cStrCat(out, (const char *)0x36E754);
        {
            char *typeInfo1 = *(char **)((const char *)self + 0x3C);
            GetTextSlot *slot1 = (GetTextSlot *)(typeInfo1 + 0x78);
            char *sub1 = (char *)self + 0x38;
            slot1->fn(sub1 + slot1->offset, out);
        }
        cStrCat(out, (const char *)0x36DCEC);
        goto switch_done;
    } else {
        if (mode < 3) {
            goto mode_two;
        }
        if (mode < 4) {
            goto mode_three;
        }
        goto switch_done;
    }

mode_one:
    cStrCat(out, (const char *)0x36E760);
    {
        char *typeInfo2 = *(char **)((const char *)self + 0x50);
        GetTextSlot *slot2 = (GetTextSlot *)(typeInfo2 + 0x78);
        char *sub2 = (char *)self + 0x4C;
        slot2->fn(sub2 + slot2->offset, out);
    }

    int value0 = *(int *)((const char *)self + 0x100);
    text[0] = '\0';
    register char *tempPtr __asm__("$20");
    register int fmtBase __asm__("$21");

    int valueAfter;
    int taggedAfter;
    {
        int value = value0;
        int owned = 0;
        int tagged = value & 1;
        tempPtr = temp;
        __asm__ volatile("lui %0, 0x37" : "=r"(fmtBase));
        if (tagged != 0) {
            owned = 1;
        }
        if (owned != 0) {
            value = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int check = value;
        if (check != 0) {
            GetTextSlot *slot =
                (GetTextSlot *)(*(char **)(check + 4) + 0xD0);
            slot->fn((char *)value + slot->offset, text);
            valueAfter = *(int *)((const char *)self + 0x100);
            taggedAfter = valueAfter & 1;
        } else {
            cStrCat(text, (const char *)0x36DB24);
            valueAfter = *(int *)((const char *)self + 0x100);
            taggedAfter = valueAfter & 1;
        }
    }

    {
        int value = valueAfter;
        int tagged = taggedAfter;
        const char *nameText = (const char *)self + 0xE8;
        int owned2 = 0;
        if (tagged != 0) {
            owned2 = 1;
        }
        if (owned2 == 0) {
            value = value != 0;
        } else {
            value = 0;
            __asm__ volatile("" ::: "memory");
        }

        const char *targetText;
        if (value != 0) {
            targetText = text;
        } else {
            targetText = (const char *)0x36D944;
        }

        register const char *fmtArg __asm__("$5");
        __asm__ volatile("addiu %0, %1, -6920"
                         : "=r"(fmtArg)
                         : "r"(fmtBase));
        __0oEcStrctPCce(tempPtr, fmtArg, nameText, targetText);
        __asm__ volatile("addiu %0, $sp, 256" : "=r"(tempPtr));
        int empty = *tempPtr == '\0';
        empty &= 0xFF;
        const char *prefix;
        if (empty != 0) {
            prefix = (const char *)0x36D944;
        } else {
            prefix = (const char *)0x36DADC;
        }
        __asm__ volatile("addiu %0, %1, -6920"
                         : "=r"(fmtArg)
                         : "r"(fmtBase));
        cStrAppend(out, fmtArg, prefix, tempPtr);
    }
    cStrCat(out, (const char *)0x36DCEC);
    goto switch_done;

mode_two:
    cStrCat(out, (const char *)0x36E770);
    goto switch_done;

mode_three:
    cStrCat(out, (const char *)0x36E780);
    ((const gcDesiredCamera *)((const char *)self + 0x78))->GetText(out);
    cStrCat(out, (const char *)0x36DCEC);

switch_done:
    if (active != 0) {
        cStrCat(out, (const char *)0x36E484);

        int value = *(int *)((const char *)self + 0xB0);
        int owned = 0;
        if (value & 1) {
            owned = 1;
        }
        if (owned != 0) {
            value = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int check = value;
        if (check != 0) {
            GetTextSlot *slot =
                (GetTextSlot *)(*(char **)(check + 4) + 0xD0);
            slot->fn((char *)value + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }
        cStrCat(out, (const char *)0x36E48C);
    }
}
