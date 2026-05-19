// gcDoEntitySetCollisionMask.cpp - decompiled from gcAll_psp.obj.
//
// Functions:
//   0x002c5914 gcDoEntitySetCollisionMask::GetType(void) const   280B
//   0x002c5a2c gcDoEntitySetCollisionMask::Write(cFile &) const  164B

class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cReadBlock;
class cType;

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

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

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcExpression {
};

class gcAction : public gcExpression {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    int Read(cFile &, cMemPool *);
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
typedef void (*gcDoEntitySetCollisionMaskReadFn)(void *, cFile *, cMemPool *);

struct gcDoEntitySetCollisionMaskWriteSlot {
    short mOffset;
    short mPad;
    gcDoEntitySetCollisionMaskWriteFn mWrite;
};

struct gcDoEntitySetCollisionMaskReadSlot {
    short mOffset;
    short mPad;
    gcDoEntitySetCollisionMaskReadFn mRead;
};

struct gcDoEntitySetCollisionMaskTypeInfo {
    char mPad[0x28];
    gcDoEntitySetCollisionMaskWriteSlot mSlot;
};

extern const char gcDoEntitySetCollisionMask_base_name[];
extern const char gcDoEntitySetCollisionMask_base_desc[];
extern char D_00002F98[];
extern char D_00000338[];
extern char cBase_vtbl_40_enum[] asm("__0dFcBaseG__vtbl");
extern char cBase_vtbl_40_obj[] asm("__0dFcBaseG__vtbl");
extern char cBase_vtbl_14_enum[] asm("__0dFcBaseG__vtbl");
extern char cBase_vtbl_14_obj[] asm("__0dFcBaseG__vtbl");
extern char gcDesiredEntity_vtbl_40[] asm("__0dPgcDesiredEntityG__vtbl");
extern char gcDesiredEntity_vtbl_14[] asm("__0dPgcDesiredEntityG__vtbl");
extern char gcDesiredEnumerationEntry_vtbl_40[]
    asm("__0dZgcDesiredEnumerationEntryG__vtbl");
extern char gcDesiredEnumerationEntry_vtbl_14[]
    asm("__0dZgcDesiredEnumerationEntryG__vtbl");
extern char gcDesiredObject_vtbl_40[] asm("__0dPgcDesiredObjectG__vtbl");
extern char gcDesiredObject_vtbl_14[] asm("__0dPgcDesiredObjectG__vtbl");

extern "C" {
void gcAction_ctor_cBase(void *, cBase *);
void gcDesiredObject_ctor_cBase(void *, cBase *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);
}

class gcDoEntitySetCollisionMask : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    static void operator delete(void *);
    ~gcDoEntitySetCollisionMask(void);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);
extern "C" void gcAction_dtor(void *, int) asm("__0oIgcActiondtv");

inline void gcDoEntitySetCollisionMask::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

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

int gcDoEntitySetCollisionMask::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 7, true);
    if ((unsigned int)rb[3] != 7 ||
        ((gcAction *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x0C, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x10, 4);

    char *desired0 = (char *)this + 0x14;
    char *type0 = *(char **)((char *)this + 0x18);
    gcDoEntitySetCollisionMaskReadSlot *slot0 =
        (gcDoEntitySetCollisionMaskReadSlot *)(type0 + 0x30);
    cFile *f = *(cFile **)&rb[0];
    slot0->mRead(desired0 + slot0->mOffset, f,
                 cMemPool::GetPoolFromPtr(desired0));

    char *desired1 = (char *)this + 0x40;
    char *type1 = *(char **)((char *)this + 0x44);
    gcDoEntitySetCollisionMaskReadSlot *slot1 =
        (gcDoEntitySetCollisionMaskReadSlot *)(type1 + 0x30);
    f = *(cFile **)&rb[0];
    slot1->mRead(desired1 + slot1->mOffset, f,
                 cMemPool::GetPoolFromPtr(desired1));

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oagcDoEntitySetCollisionMaskdtv, 0x280\n");

gcDoEntitySetCollisionMask::~gcDoEntitySetCollisionMask(void) {
    *(void **)((char *)this + 4) = D_00002F98;
    char *p40 = (char *)this + 0x40;
    char *p14 = (char *)this + 0x14;

    if ((void *)p40 != 0) {
        *(void **)((char *)this + 0x44) = gcDesiredEntity_vtbl_40;

        if ((void *)((char *)this + 0x54) != 0) {
            *(void **)((char *)this + 0x58) =
                gcDesiredEnumerationEntry_vtbl_40;

            if ((void *)((char *)this + 0x68) != 0) {
                int owned = 1;
                int val = *(int *)((char *)this + 0x68);
                if (val & 1) {
                    owned = 0;
                }
                if (owned != 0) {
                    if (val != 0) {
                        char *typeInfo = *(char **)(val + 4);
                        DtorDeleteRecord *slot =
                            (DtorDeleteRecord *)(typeInfo + 0x50);
                        slot->fn((char *)val + slot->offset, (void *)3);
                        *(int *)((char *)this + 0x68) = 0;
                    }
                }
            }
            *(void **)((char *)this + 0x58) = cBase_vtbl_40_enum;
        }

        *(void **)((char *)this + 0x44) = gcDesiredObject_vtbl_40;

        if ((void *)((char *)this + 0x48) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x48);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0) {
                if (val != 0) {
                    char *typeInfo = *(char **)(val + 4);
                    DtorDeleteRecord *slot =
                        (DtorDeleteRecord *)(typeInfo + 0x50);
                    slot->fn((char *)val + slot->offset, (void *)3);
                    *(int *)((char *)this + 0x48) = 0;
                }
            }
        }
        *(void **)((char *)this + 0x44) = cBase_vtbl_40_obj;
    }

    if ((void *)p14 != 0) {
        *(void **)((char *)this + 0x18) = gcDesiredEntity_vtbl_14;

        if ((void *)((char *)this + 0x28) != 0) {
            *(void **)((char *)this + 0x2C) =
                gcDesiredEnumerationEntry_vtbl_14;

            if ((void *)((char *)this + 0x3C) != 0) {
                int owned = 1;
                int val = *(int *)((char *)this + 0x3C);
                if (val & 1) {
                    owned = 0;
                }
                if (owned != 0) {
                    if (val != 0) {
                        char *typeInfo = *(char **)(val + 4);
                        DtorDeleteRecord *slot =
                            (DtorDeleteRecord *)(typeInfo + 0x50);
                        slot->fn((char *)val + slot->offset, (void *)3);
                        *(int *)((char *)this + 0x3C) = 0;
                    }
                }
            }
            *(void **)((char *)this + 0x2C) = cBase_vtbl_14_enum;
        }

        *(void **)((char *)this + 0x18) = gcDesiredObject_vtbl_14;

        if ((void *)((char *)this + 0x1C) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x1C);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0) {
                if (val != 0) {
                    char *typeInfo = *(char **)(val + 4);
                    DtorDeleteRecord *slot =
                        (DtorDeleteRecord *)(typeInfo + 0x50);
                    slot->fn((char *)val + slot->offset, (void *)3);
                    *(int *)((char *)this + 0x1C) = 0;
                }
            }
        }
        *(void **)((char *)this + 0x18) = cBase_vtbl_14_obj;
    }

    gcAction_dtor(this, 0);
}
