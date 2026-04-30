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
    short pad;
    void (*fn)(void *, cFile *);
};

extern const char gcDoEntityAttach_base_name[];
extern const char gcDoEntityAttach_base_desc[];

class gcDoEntityAttach : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntityAttach;

const cType *gcDoEntityAttach::GetType(void) const {
    if (!type_gcDoEntityAttach) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoEntityAttach_base_name,
                        gcDoEntityAttach_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntityAttach = cType::InitializeType(
            0, 0, 0x156, type_action, gcDoEntityAttach::New, 0, 0, 0);
    }
    return type_gcDoEntityAttach;
}

void gcDoEntityAttach::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    gcAction::Write(file);
    wb.Write(*(const int *)((const char *)this + 0x0C));

    {
        char *typeInfo = *(char **)((const char *)this + 0x14);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x10;
        rec->fn(base + rec->offset, wb._file);
    }

    {
        char *typeInfo = *(char **)((const char *)this + 0x40);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x3C;
        rec->fn(base + rec->offset, wb._file);
    }

    ((const cName *)((const char *)this + 0x68))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x80))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x84))->Write(wb);
    wb.End();
}
