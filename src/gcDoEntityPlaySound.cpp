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

class cBaseArray {
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

class gcDoEntityPlaySound : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoEntityPlaySound asm("D_0009F630");

const cType *gcDoEntityPlaySound::GetType(void) const {
    if (!type_gcDoEntityPlaySound) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A,
                                                        type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoEntityPlaySound = cType::InitializeType(
            0, 0, 0x14F, type_action, gcDoEntityPlaySound::New, 0, 0, 0x80);
    }
    return type_gcDoEntityPlaySound;
}

void gcDoEntityPlaySound::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    gcAction::Write(file);
    ((const gcDesiredValue *)((const char *)this + 0x0C))->Write(wb);

    {
        char *typeInfo = *(char **)((const char *)this + 0x14);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        short off = rec->offset;
        void *base = (void *)((const char *)this + 0x10);
        rec->fn((char *)base + off, wb._file);
    }

    ((const cBaseArray *)((const char *)this + 0x3C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x44))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x48))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x4C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x50))->Write(wb);
    wb.Write(*(const bool *)((const char *)this + 0x54));
    wb.Write(*(const bool *)((const char *)this + 0x55));
    ((const gcDesiredValue *)((const char *)this + 0x58))->Write(wb);
    wb.End();
}
