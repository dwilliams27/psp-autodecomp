// gcDoCameraMode.cpp - gcAll_psp.obj

class cBase;
class cFile;
class cMemPool;
class cType;

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

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    char _pad[12];

    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    int mField0;
    void Write(cWriteBlock &) const;
};

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
};

struct DtorSlot {
    short offset;
    short _pad;
    void (*fn)(void *, int);
};

struct PoolDeleteSlot {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class gcDoCameraMode : public gcAction {
public:
    char _pad0C[0x38];        // 0x0C
    gcDesiredValue mDesired;  // 0x44
    bool mMode;               // 0x48

    const cType *GetType(void) const;
    void Write(cFile &) const;
    ~gcDoCameraMode(void);
    static void operator delete(void *);
};

class gcDoCameraOp : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

extern const char gcDoCameraMode_base_name[] asm("D_0036D894");
extern const char gcDoCameraMode_base_desc[] asm("D_0036D89C");

extern "C" void gcAction___dtor_gcAction_void(void *, int);
extern "C" void gcDesiredCamera___dtor_gcDesiredCamera_void(void *, int);
extern char gcDoCameraModevirtualtable[];
extern "C" void free(void *);
void *cMemPool_GetPoolFromPtr(const void *);

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoCameraMode asm("D_0009F5B8");
static cType *type_gcDoCameraOp asm("D_0009F5C8");

inline void gcDoCameraMode::operator delete(void *ptr) {
    void *pool = cMemPool_GetPoolFromPtr(ptr);
    if (pool != 0) {
        void *block = *(void **)((char *)pool + 0x24);
        char *entries = *(char **)((char *)block + 0x1C);
        PoolDeleteSlot *slot = (PoolDeleteSlot *)(entries + 0x30);
        slot->fn((char *)block + slot->offset, ptr);
    } else {
        free(ptr);
    }
}

// 0x002946d8 - gcDoCameraMode::GetType(void) const
const cType *gcDoCameraMode::GetType(void) const {
    if (!type_gcDoCameraMode) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoCameraMode_base_name, gcDoCameraMode_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoCameraMode = cType::InitializeType(
            0, 0, 0x24F, type_action, 0, 0, 0, 0);
    }
    return type_gcDoCameraMode;
}

// 0x002988b0 - gcDoCameraOp::GetType(void) const
const cType *gcDoCameraOp::GetType(void) const {
    if (!type_gcDoCameraOp) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoCameraMode_base_name, gcDoCameraMode_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoCameraOp = cType::InitializeType(
            0, 0, 0x24D, type_action, gcDoCameraOp::New, 0, 0, 0);
    }
    return type_gcDoCameraOp;
}

// 0x002947e8 - gcDoCameraMode::Write(cFile &) const
void gcDoCameraMode::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction::Write(file);

    char *typeInfo = *(char **)((const char *)this + 0x10);
    WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
    short off = rec->offset;
    void *base = (void *)((const char *)this + 0x0C);
    rec->fn((char *)base + off, wb._file);

    mDesired.Write(wb);
    wb.Write(mMode);
    wb.End();
}

// Original object keeps this dead branch tail inside the destructor symbol.
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

// 0x00294de0 - gcDoCameraMode::~gcDoCameraMode(void)
gcDoCameraMode::~gcDoCameraMode(void) {
    *(void **)((char *)this + 4) = gcDoCameraModevirtualtable;

    if ((void *)((char *)this + 0x44) != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x44);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorSlot *slot = (DtorSlot *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, 3);
            *(int *)((char *)this + 0x44) = 0;
        }
    }

    gcDesiredCamera___dtor_gcDesiredCamera_void((char *)this + 0x0C, 2);
    gcAction___dtor_gcAction_void(this, 0);
}
