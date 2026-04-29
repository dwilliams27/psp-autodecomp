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

class gcDoCameraMode : public gcAction {
public:
    char _pad0C[0x38];        // 0x0C
    gcDesiredValue mDesired;  // 0x44
    bool mMode;               // 0x48

    const cType *GetType(void) const;
    void Write(cFile &) const;
};

class gcDoCameraOp : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

extern const char gcDoCameraMode_base_name[] asm("D_0036D894");
extern const char gcDoCameraMode_base_desc[] asm("D_0036D89C");

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoCameraMode asm("D_0009F5B8");
static cType *type_gcDoCameraOp asm("D_0009F5C8");

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
