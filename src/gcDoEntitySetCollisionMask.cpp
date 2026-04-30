// gcDoEntitySetCollisionMask.cpp - decompiled from gcAll_psp.obj.
//
// Functions:
//   0x002c5914 gcDoEntitySetCollisionMask::GetType(void) const   280B
//   0x002c5a2c gcDoEntitySetCollisionMask::Write(cFile &) const  164B

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    cFile *mFile;
    int mPos;

    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
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

class gcExpression {
};

class gcAction : public gcExpression {
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

typedef void (*gcDoEntitySetCollisionMaskWriteFn)(cBase *, cFile *);

struct gcDoEntitySetCollisionMaskWriteSlot {
    short mOffset;
    short mPad;
    gcDoEntitySetCollisionMaskWriteFn mWrite;
};

struct gcDoEntitySetCollisionMaskTypeInfo {
    char mPad[0x28];
    gcDoEntitySetCollisionMaskWriteSlot mSlot;
};

extern const char gcDoEntitySetCollisionMask_base_name[];
extern const char gcDoEntitySetCollisionMask_base_desc[];
extern char D_00002F98[];
extern char D_00000338[];

extern "C" {
void gcAction_ctor_cBase(void *, cBase *);
void gcDesiredObject_ctor_cBase(void *, cBase *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);
}

class gcDoEntitySetCollisionMask : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntitySetCollisionMask;

cBase *gcDoEntitySetCollisionMask::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntitySetCollisionMask *result = 0;
    gcDoEntitySetCollisionMask *obj =
        (gcDoEntitySetCollisionMask *)entry->fn(base, 0x6C, 4, 0, 0);
    if (obj != 0) {
        gcAction_ctor_cBase(obj, parent);

        int *obj_i = (int *)obj;
        obj_i[1] = (int)D_00002F98;
        obj_i[3] = 0;
        obj_i[4] = 0;

        void *desired = (char *)obj_i + 0x14;
        gcDesiredObject_ctor_cBase(desired, (cBase *)obj);

        obj_i[6] = (int)D_00000338;

        gcDesiredEntityHelper_ctor((char *)obj + 0x20, 1, 0, 0);

        obj_i[6] = 0x388A48;
        obj_i[11] = 0x37E6A8;
        obj_i[10] = (int)desired;
        obj_i[11] = 0x388568;
        ((char *)obj)[0x30] = 1;
        ((char *)obj)[0x31] = 0;
        obj_i[13] = 0;
        obj_i[14] = 0;
        obj_i[15] = (int)desired | 1;

        desired = (char *)obj + 0x40;
        gcDesiredObject_ctor_cBase(desired, (cBase *)obj);

        obj_i[17] = (int)D_00000338;

        gcDesiredEntityHelper_ctor((char *)obj + 0x4C, 1, 0, 0);

        obj_i[22] = 0x37E6A8;
        obj_i[17] = 0x388A48;
        obj_i[21] = (int)desired;
        obj_i[22] = 0x388568;
        ((char *)obj)[0x5C] = 1;
        ((char *)obj)[0x5D] = 0;
        obj_i[24] = 0;
        obj_i[25] = 0;
        obj_i[26] = (int)desired | 1;

        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoEntitySetCollisionMask::GetType(void) const {
    if (!type_gcDoEntitySetCollisionMask) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoEntitySetCollisionMask_base_name,
                        gcDoEntitySetCollisionMask_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntitySetCollisionMask = cType::InitializeType(
            0, 0, 0x14C, type_action, gcDoEntitySetCollisionMask::New, 0, 0, 0);
    }
    return type_gcDoEntitySetCollisionMask;
}

void gcDoEntitySetCollisionMask::Write(cFile &file) const {
    cWriteBlock wb(file, 7);
    gcAction::Write(file);
    wb.Write(*(const unsigned int *)((const char *)this + 0xC));
    wb.Write(*(const unsigned int *)((const char *)this + 0x10));

    gcDoEntitySetCollisionMaskTypeInfo *type0 =
        *(gcDoEntitySetCollisionMaskTypeInfo **)((const char *)this + 0x18);
    gcDoEntitySetCollisionMaskWriteSlot *slot0 = &type0->mSlot;
    cBase *entity0 = (cBase *)((char *)this + 0x14);
    slot0->mWrite((cBase *)((char *)entity0 + slot0->mOffset), wb.mFile);

    gcDoEntitySetCollisionMaskTypeInfo *type1 =
        *(gcDoEntitySetCollisionMaskTypeInfo **)((const char *)this + 0x44);
    gcDoEntitySetCollisionMaskWriteSlot *slot1 = &type1->mSlot;
    cBase *entity1 = (cBase *)((char *)this + 0x40);
    slot1->mWrite((cBase *)((char *)entity1 + slot1->mOffset), wb.mFile);

    wb.End();
}
