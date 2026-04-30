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
    void Write(int);
    void End(void);
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
extern char gcDoCameraFollowEntityvirtualtable[];

extern const char gcDoCameraFollowEntity_base_name[];
extern const char gcDoCameraFollowEntity_base_desc[];

class gcDoCameraFollowEntity : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoCameraFollow;
static cType *type_gcDoCameraFollowEntity;

cBase *gcDoCameraFollowEntity::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoCameraFollowEntity *result = 0;
    gcDoCameraFollowEntity *obj =
        (gcDoCameraFollowEntity *)entry->fn(base, 0x7C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoCameraModevirtualtable;

        gcDesiredCamera_gcDesiredCamera((char *)obj + 0x0C, (cBase *)obj);

        *(int *)((char *)obj + 0x44) = (int)obj | 1;
        *(unsigned char *)((char *)obj + 0x48) = 0;
        ((void **)obj)[1] = gcDoCameraFollowEntityvirtualtable;

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
        *(int *)((char *)obj + 0x78) = 0;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoCameraFollowEntity::GetType(void) const {
    if (!type_gcDoCameraFollowEntity) {
        if (!type_gcDoCameraFollow) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoCameraFollowEntity_base_name,
                            gcDoCameraFollowEntity_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoCameraFollow = cType::InitializeType(
                0, 0, 0x24F, type_action, 0, 0, 0, 0);
        }
        type_gcDoCameraFollowEntity = cType::InitializeType(
            0, 0, 0x251, type_gcDoCameraFollow,
            gcDoCameraFollowEntity::New, 0, 0, 0);
    }
    return type_gcDoCameraFollowEntity;
}

void gcDoCameraFollowEntity::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
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

    wb.Write(*(const int *)((const char *)this + 0x78));
    wb.End();
}
