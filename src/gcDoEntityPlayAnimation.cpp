#include "cBase.h"

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
    void Write(cWriteBlock &) const;
};

struct WriteRec {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
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
