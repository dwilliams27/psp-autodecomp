// gcDoUIEditBoxOp — gcAll_psp.obj
//   0x00309070  gcDoUIEditBoxOp::New(cMemPool *, cBase *) static
//   0x00309118  gcDoUIEditBoxOp::GetType(void) const
//   0x00309230  gcDoUIEditBoxOp::Write(cFile &) const

class cBase;
class cFile;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
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
    unsigned int mValue;
    void Write(cWriteBlock &) const;
};

class gcDesiredUIWidgetHelper {
public:
    int mField0;
    int mField4;
    unsigned int mField8;
    void GetText(char *) const;
    void Write(cWriteBlock &) const;
};

class cStr {
public:
    char _data[256];
    cStr(const char *, ...);
};

class gcAction {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDoUIEditBoxOp : public gcAction {
public:
    gcDesiredUIWidgetHelper mWidget;
    int mField18;
    unsigned int mField1C;
    gcDesiredValue mField20;

    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void GetText(char *) const;
    void Write(cFile &) const;
    gcDoUIEditBoxOp &operator=(const gcDoUIEditBoxOp &);
    static void operator delete(void *);
    ~gcDoUIEditBoxOp(void);
};

void gcAction_gcAction(gcDoUIEditBoxOp *, cBase *);
extern "C" void gcAction___dtor_gcAction_void(void *, int);
void gcDesiredUIWidgetHelper_ctor(gcDesiredUIWidgetHelper *, int);

extern char gcDoUIEditBoxOpvirtualtable[];
extern const char gcDoUIEditBoxOp_base_name[];
extern const char gcDoUIEditBoxOp_base_desc[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
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
    short pad;
    void *fn;
};

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

inline void gcDoUIEditBoxOp::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

cBase *gcDoUIEditBoxOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUIEditBoxOp *result = 0;
    gcDoUIEditBoxOp *obj = (gcDoUIEditBoxOp *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoUIEditBoxOpvirtualtable;
        gcDesiredUIWidgetHelper_ctor((gcDesiredUIWidgetHelper *)((char *)obj + 0x0C), 1);
        ((int *)obj)[6] = 3;
        int tagged = (int)obj | 1;
        ((int *)obj)[7] = tagged;
        ((int *)obj)[8] = tagged;
        result = obj;
    }
    return (cBase *)result;
}

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoUIEditBoxOp;

void gcDoUIEditBoxOp::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoUIEditBoxOp) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoUIEditBoxOp_base_name,
                            gcDoUIEditBoxOp_base_desc, 1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoUIEditBoxOp = cType::InitializeType(
                0, 0, 0x174, type_action, gcDoUIEditBoxOp::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoUIEditBoxOp;
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
    *this = *(const gcDoUIEditBoxOp *)copy;
}

const cType *gcDoUIEditBoxOp::GetType(void) const {
    if (!type_gcDoUIEditBoxOp) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcDoUIEditBoxOp_base_name,
                                                      gcDoUIEditBoxOp_base_desc,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoUIEditBoxOp = cType::InitializeType(0, 0, 0x174, type_action,
                                                      gcDoUIEditBoxOp::New,
                                                      0, 0, 0);
    }
    return type_gcDoUIEditBoxOp;
}

void gcDoUIEditBoxOp::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction::Write(file);
    mWidget.Write(wb);
    wb.Write(mField18);

    int value = mField1C;
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
    mField20.Write(wb);
    wb.End();
}

void gcDoUIEditBoxOp::GetText(char *buf) const {
    register const gcDoUIEditBoxOp *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    char widgetText[256];

    widgetText[0] = *widgetText = '\0';
    ((const gcDesiredUIWidgetHelper *)((const char *)self + 0x0C))
        ->GetText(widgetText);
    cStrAppend(out, (const char *)0x36EFFC, widgetText,
               (const char *)0x36DAF0);

    int kind = *(int *)((const char *)self + 0x18);
    int useName = 0;
    if (kind >= 0) {
        useName = 0;
        int less2 = kind < 2;
        if (less2 == 0) {
            useName = 0;
        } else {
            goto set_use_name;
        }
    }
    goto use_name_done;
set_use_name:
    useName = 1;
use_name_done:

    if (useName != 0) {
        register const gcDoUIEditBoxOp *savedSelf __asm__("$18") = self;
        cStr text((const char *)0x36DACC);
        register char *catOut __asm__("$4") = out;

        int val = *(int *)((const char *)savedSelf + 0x1C);
        int flag = 0;
        int tag = val & 1;
        if (tag != 0) {
            flag = 1;
        }

        int valid;
        if (flag != 0) {
            valid = 0;
        } else {
            valid = val != 0;
            valid &= 0xFF;
            valid = valid != 0;
        }

        if (valid != 0) {
            int flag2 = 0;
            if (tag != 0) {
                flag2 = 1;
            }

            char *type;
            if (flag2 != 0) {
                val = 0;
                type = *(char **)(val + 4);
            } else {
                type = *(char **)(val + 4);
            }

            GetTextSlot *slot = (GetTextSlot *)(type + 0x40);
            ((void (*)(void *, char *))slot->fn)(
                (char *)val + slot->offset, (char *)&text);
        }

        cStrCat(catOut, (char *)&text);
    } else {
        int useValue;
        if (kind >= 4) {
            useValue = kind < 6;
            if (useValue != 0) {
                useValue = 1;
            } else {
                useValue = 0;
            }
        } else {
            useValue = 0;
        }

        if (useValue != 0) {
            int val = *(int *)((const char *)self + 0x20);
            int tagged = 0;
            if (val & 1) {
                tagged = 1;
            }
            if (tagged != 0) {
                val = 0;
            } else {
                __asm__ volatile("" ::: "memory");
            }

            int check = val;
            if (check != 0) {
                char *type = *(char **)(check + 4);
                GetTextSlot *slot = (GetTextSlot *)(type + 0xD0);
                ((void (*)(void *, char *))slot->fn)(
                    (char *)val + slot->offset, out);
            } else {
                cStrCat(out, (const char *)0x36DB24);
            }
        }
    }

    cStrCat(out, (const char *)0x36DCEC);
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oPgcDoUIEditBoxOpdtv, 0x128\n");

gcDoUIEditBoxOp::~gcDoUIEditBoxOp(void) {
    *(void **)((char *)this + 4) = gcDoUIEditBoxOpvirtualtable;
    char *second = (char *)this + 0x1C;

    if ((void *)((char *)this + 0x20) != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x20);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x20) = 0;
            }
        }
    }

    if ((void *)second != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x1C);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 0x1C) = 0;
        }
    }

    gcAction___dtor_gcAction_void(this, 0);
}
