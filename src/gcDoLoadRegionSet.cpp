#include "cBase.h"

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
    void Write(bool);
    void Write(int);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

struct WriteRec {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

struct GetTextSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcDoLoadRegionSet : public gcAction {
public:
    static void operator delete(void *);
    ~gcDoLoadRegionSet(void);
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void GetText(char *) const;
    void Write(cFile &) const;
    gcDoLoadRegionSet &operator=(const gcDoLoadRegionSet &);
};

class gcMap {
public:
    void GetRegionSetName(unsigned int, char *) const;
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

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern char D_00003938[];
extern const char gcDoLoadRegionSet_base_name[];
extern const char gcDoLoadRegionSet_base_desc[];
extern gcMap *g_gcMap asm("D_0037D7FC");

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoLoadRegionSet;

void gcAction_gcAction(void *, cBase *);
void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);
extern "C" void gcAction___dtor_gcAction_void(void *, int);
extern "C" void gcEvent___dtor_gcEvent_void(void *, int);
extern "C" void gcEvent_ctor(void *, cBase *, const char *)
    __asm__("__0oHgcEventctP6FcBasePCc");

inline void gcDoLoadRegionSet::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oRgcDoLoadRegionSetdtv, 0x140\n");

gcDoLoadRegionSet::~gcDoLoadRegionSet(void) {
    *(void **)((char *)this + 4) = D_00003938;
    gcEvent___dtor_gcEvent_void((char *)this + 0x34, 2);
    gcEvent___dtor_gcEvent_void((char *)this + 0x18, 2);
    char *slot14 = (char *)this + 0x14;
    char *slot10 = (char *)this + 0x10;

    if ((void *)slot14 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x14);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot =
                (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 0x14) = 0;
        }
    }

    if ((void *)slot10 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x10);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot =
                (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 0x10) = 0;
        }
    }

    gcAction___dtor_gcAction_void(this, 0);
}

cBase *gcDoLoadRegionSet::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoLoadRegionSet *result = 0;
    gcDoLoadRegionSet *obj =
        (gcDoLoadRegionSet *)entry->fn(base, 0x54, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = D_00003938;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = (int)((unsigned int)obj | 1);
        ((int *)obj)[5] = ((int *)obj)[4];
        gcEvent_ctor((char *)obj + 0x18, (cBase *)obj, (const char *)0x36EBE8);
        gcEvent_ctor((char *)obj + 0x34, (cBase *)obj, (const char *)0x36EBF8);
        ((char *)obj)[0x50] = 0;
        ((char *)obj)[0x51] = 0;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoLoadRegionSet::GetType(void) const {
    if (!type_gcDoLoadRegionSet) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoLoadRegionSet_base_name,
                        gcDoLoadRegionSet_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoLoadRegionSet = cType::InitializeType(
            0, 0, 0xB4, type_action, gcDoLoadRegionSet::New, 0, 0, 0);
    }
    return type_gcDoLoadRegionSet;
}

void gcDoLoadRegionSet::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoLoadRegionSet) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoLoadRegionSet_base_name,
                            gcDoLoadRegionSet_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoLoadRegionSet = cType::InitializeType(
                0, 0, 0xB4, type_action, gcDoLoadRegionSet::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoLoadRegionSet;
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
    *this = *(const gcDoLoadRegionSet *)copy;
}

void gcDoLoadRegionSet::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    gcAction::Write(file);
    wb.Write(*(const int *)((const char *)this + 0x0C));
    ((const gcDesiredValue *)((const char *)this + 0x10))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x14))->Write(wb);

    {
        char *typeInfo = *(char **)((const char *)this + 0x1C);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        short off = rec->offset;
        void *base = (char *)this + 0x18;
        rec->fn((char *)base + off, wb._file);
    }

    {
        char *typeInfo = *(char **)((const char *)this + 0x38);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        short off = rec->offset;
        void *base = (char *)this + 0x34;
        rec->fn((char *)base + off, wb._file);
    }

    wb.Write(*(const bool *)((const char *)this + 0x50));
    wb.Write(*(const bool *)((const char *)this + 0x51));
    wb.End();
}

void gcDoLoadRegionSet::GetText(char *buf) const {
    register const gcDoLoadRegionSet *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    char text0[256];
    char text1[256];
    char regionName[256];

    gcMap *map = g_gcMap;
    if (map != 0) {
        if (*(unsigned char *)((const char *)self + 0x51) != 0) {
            text0[0] = '\0';
            text1[0] = '\0';
            int val0 = *(int *)((const char *)self + 0x10);
            register char *text1Out __asm__("$18") = text1;
            int flag0 = 0;
            if (val0 & 1) {
                flag0 = 1;
            }
            if (flag0 != 0) {
                val0 = 0;
            } else {
                __asm__ volatile("" ::: "memory");
            }
            int check0 = val0;
            int val1;
            if (check0 != 0) {
                char *typeInfo = *(char **)(check0 + 4);
                GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0xD0);
                slot->fn((char *)val0 + slot->offset, text0);
                val1 = *(int *)((const char *)self + 0x14);
            } else {
                cStrCat(text0, (const char *)0x36DB24);
                val1 = *(int *)((const char *)self + 0x14);
            }

            int flag1 = 0;
            if (val1 & 1) {
                flag1 = 1;
            }
            if (flag1 != 0) {
                val1 = 0;
            } else {
                __asm__ volatile("" ::: "memory");
            }
            int check1 = val1;
            if (check1 != 0) {
                char *typeInfo = *(char **)(check1 + 4);
                GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0xD0);
                slot->fn((char *)val1 + slot->offset, text1Out);
            } else {
                cStrCat(text1Out, (const char *)0x36DB24);
            }

            cStrAppend(out, (const char *)0x36EC0C, text0, text1Out);
        } else {
            unsigned int id = *(unsigned int *)((const char *)self + 0x0C);
            regionName[0] = '\0';
            map->GetRegionSetName(id, regionName);
            cStrAppend(out, (const char *)0x36EC24, regionName);
        }
    } else {
        cStrCat(out, (const char *)0x36EC3C);
    }
}
