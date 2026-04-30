#include "cBase.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cFile;

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
    short _pad;
    void (*fn)(void *, cFile *);
};

class gcDoEntityAttack : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

extern const char gcDoEntityAttack_base_name[];
extern const char gcDoEntityAttack_base_desc[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntityAttack;

const cType *gcDoEntityAttack::GetType(void) const {
    if (!type_gcDoEntityAttack) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcDoEntityAttack_base_name, gcDoEntityAttack_base_desc, 1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntityAttack = cType::InitializeType(0, 0, 0x148, type_action, gcDoEntityAttack::New, 0, 0, 0x80);
    }
    return type_gcDoEntityAttack;
}

void gcDoEntityAttack::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    gcAction::Write(file);

    char *typeInfo0 = *(char **)((const char *)this + 0x10);
    WriteRec *rec0 = (WriteRec *)(typeInfo0 + 0x28);
    short off0 = rec0->offset;
    void *base0 = (char *)this + 0x0C;
    rec0->fn((char *)base0 + off0, wb._file);

    ((const gcDesiredValue *)((const char *)this + 0x68))->Write(wb);

    char *typeInfo1 = *(char **)((const char *)this + 0x40);
    WriteRec *rec1 = (WriteRec *)(typeInfo1 + 0x28);
    short off1 = rec1->offset;
    void *base1 = (char *)this + 0x3C;
    rec1->fn((char *)base1 + off1, wb._file);

    wb.Write(*(int *)((const char *)this + 0x6C));
    wb.Write(*(int *)((const char *)this + 0x38));

    char *typeInfo2 = *(char **)((const char *)this + 0x58);
    WriteRec *rec2 = (WriteRec *)(typeInfo2 + 0x28);
    short off2 = rec2->offset;
    void *base2 = (char *)this + 0x54;
    rec2->fn((char *)base2 + off2, wb._file);

    wb.End();
}
