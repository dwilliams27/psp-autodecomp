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

class gcDoEntityPartialBodyAttack : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoEntityPartialBodyAttack asm("D_0009F624");

const cType *gcDoEntityPartialBodyAttack::GetType(void) const {
    if (!type_gcDoEntityPartialBodyAttack) {
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
        type_gcDoEntityPartialBodyAttack = cType::InitializeType(
            0, 0, 0x149, type_action, gcDoEntityPartialBodyAttack::New, 0, 0, 0x80);
    }
    return type_gcDoEntityPartialBodyAttack;
}

void gcDoEntityPartialBodyAttack::Write(cFile &file) const {
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

    char *typeInfo2 = *(char **)((const char *)this + 0x74);
    WriteRec *rec2 = (WriteRec *)(typeInfo2 + 0x28);
    short off2 = rec2->offset;
    void *base2 = (char *)this + 0x70;
    rec2->fn((char *)base2 + off2, wb._file);

    wb.Write(*(int *)((const char *)this + 0x38));

    char *typeInfo3 = *(char **)((const char *)this + 0x58);
    WriteRec *rec3 = (WriteRec *)(typeInfo3 + 0x28);
    short off3 = rec3->offset;
    void *base3 = (char *)this + 0x54;
    rec3->fn((char *)base3 + off3, wb._file);

    wb.End();
}
