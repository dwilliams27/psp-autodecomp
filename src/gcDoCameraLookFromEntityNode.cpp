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
    void Write(bool);
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

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
};

struct VTableSlot {
    short offset;
    short _pad;
    const cType *(*getType)(void *);
};

struct cTypeNode {
    char pad[0x1C];
    const cType *parent;
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

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredCamera_gcDesiredCamera(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");

extern char D_00000338[];
extern char gcDoCameraModevirtualtable[];
extern char gcDoCameraLookFromEntityNodevirtualtable[];

extern const char gcDoCameraLookFromEntityNode_base_name[];
extern const char gcDoCameraLookFromEntityNode_base_desc[];

class gcDoCameraLookFromEntityNode : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    gcDoCameraLookFromEntityNode &operator=(const gcDoCameraLookFromEntityNode &);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoCameraFollowEntity;
static cType *type_gcDoCameraLookFromEntityNode;

void gcDoCameraLookFromEntityNode::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoCameraLookFromEntityNode) {
            if (!type_gcDoCameraFollowEntity) {
                if (!type_action) {
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
                    type_action = cType::InitializeType(
                        0, 0, 0x6B, type_expression, 0, 0, 0, 0);
                }
                type_gcDoCameraFollowEntity = cType::InitializeType(
                    0, 0, 0x24F, type_action, 0, 0, 0, 0);
            }
            type_gcDoCameraLookFromEntityNode = cType::InitializeType(
                0, 0, 0x252, type_gcDoCameraFollowEntity,
                gcDoCameraLookFromEntityNode::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoCameraLookFromEntityNode;
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
    *this = *(const gcDoCameraLookFromEntityNode *)copy;
}

cBase *gcDoCameraLookFromEntityNode::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoCameraLookFromEntityNode *result = 0;
    gcDoCameraLookFromEntityNode *obj =
        (gcDoCameraLookFromEntityNode *)entry->fn(base, 0x98, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoCameraModevirtualtable;

        gcDesiredCamera_gcDesiredCamera((char *)obj + 0x0C, (cBase *)obj);

        int encodedObj = (int)obj | 1;
        *(int *)((char *)obj + 0x44) = encodedObj;
        *(unsigned char *)((char *)obj + 0x48) = 0;
        ((void **)obj)[1] = gcDoCameraLookFromEntityNodevirtualtable;

        char *desired = (char *)obj + 0x4C;
        gcDesiredObject_gcDesiredObject(desired, (cBase *)obj);
        *(void **)((char *)obj + 0x50) = D_00000338;

        char *helper = (char *)obj + 0x58;
        int one = 1;
        gcDesiredEntityHelper_ctor(helper, 1, 0, 0);
        *(void **)((char *)obj + 0x50) = (void *)0x388A48;
        *(void **)((char *)obj + 0x64) = (void *)0x37E6A8;
        *(void **)((char *)obj + 0x60) = desired;
        *(void **)((char *)obj + 0x64) = (void *)0x388568;
        *(unsigned char *)((char *)obj + 0x68) = one;
        *(unsigned char *)((char *)obj + 0x69) = 0;
        *(int *)((char *)obj + 0x6C) = 0;
        *(int *)((char *)obj + 0x70) = 0;
        *(int *)((char *)obj + 0x74) = (int)desired | 1;
        *(short *)((char *)obj + 0x8C) = 0;
        *(short *)((char *)obj + 0x8E) = 0;
        *(unsigned char *)((char *)obj + 0x78) = 0;
        *(int *)((char *)obj + 0x90) = encodedObj;
        *(unsigned char *)((char *)obj + 0x94) = 0;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoCameraLookFromEntityNode::GetType(void) const {
    if (!type_gcDoCameraLookFromEntityNode) {
        if (!type_gcDoCameraFollowEntity) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoCameraLookFromEntityNode_base_name,
                            gcDoCameraLookFromEntityNode_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoCameraFollowEntity = cType::InitializeType(
                0, 0, 0x24F, type_action, 0, 0, 0, 0);
        }
        type_gcDoCameraLookFromEntityNode = cType::InitializeType(
            0, 0, 0x252, type_gcDoCameraFollowEntity,
            gcDoCameraLookFromEntityNode::New, 0, 0, 0);
    }
    return type_gcDoCameraLookFromEntityNode;
}

void gcDoCameraLookFromEntityNode::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    cWriteBlock inner(file, 1);
    gcAction::Write(file);

    {
        char *typeInfo = *(char **)((const char *)this + 0x10);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        short off = rec->offset;
        void *base = (void *)((const char *)this + 0x0C);
        rec->fn((char *)base + off, inner._file);
    }

    ((const gcDesiredValue *)((const char *)this + 0x44))->Write(inner);
    inner.Write(*(const bool *)((const char *)this + 0x48));
    inner.End();

    {
        char *typeInfo = *(char **)((const char *)this + 0x50);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        short off = rec->offset;
        void *base = (void *)((const char *)this + 0x4C);
        rec->fn((char *)base + off, wb._file);
    }

    ((const cName *)((const char *)this + 0x78))->Write(wb);
    wb.Write(*(const bool *)((const char *)this + 0x94));
    ((const gcDesiredValue *)((const char *)this + 0x90))->Write(wb);
    wb.End();
}
