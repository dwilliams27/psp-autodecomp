#include "cBase.h"

class cFile;
class cMemPool;

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void End(void);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char _pad0[0x28];
    cTypeMethod write_m;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDesiredObject {
public:
    int _parent;
    cType *mType;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcValue {
public:
    void Write(cFile &) const;
};

class gcLValue : public gcValue {
public:
    void Write(cFile &) const;
};

class gcValEntitySoundFrequency : public gcLValue {
public:
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

#pragma control sched=1
void gcValEntitySoundFrequency::Write(cFile &file) const {
    const gcValEntitySoundFrequency *self = this;
    __asm__ volatile("" : "+r"(self));
    cWriteBlock wb(file, 3);
    ((const gcLValue *)self)->Write(file);

    const cTypeMethod *entity_write =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)self + 8))->mType + 0x28);
    const char *entity_base = (const char *)self + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)entity_write->fn)((void *)(entity_base + entity_write->offset), wb.file);

    ((const gcDesiredValue *)((const char *)self + 0x34))->Write(wb);
    wb.Write(*(const bool *)((const char *)self + 0x38));
    wb.Write(*(const bool *)((const char *)self + 0x39));
    wb.Write(*(const bool *)((const char *)self + 0x3A));
    wb.End();
}
#pragma control sched=2

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_lvalue;
static cType *type_gcValEntitySoundFrequency;

const cType *gcValEntitySoundFrequency::GetType(void) const {
    if (!type_gcValEntitySoundFrequency) {
        if (!type_lvalue) {
            if (!type_value) {
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
                type_value = cType::InitializeType(
                    0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
            }
            type_lvalue = cType::InitializeType(
                0, 0, 0x6D, type_value, 0, 0, 0, 0);
        }
        type_gcValEntitySoundFrequency = cType::InitializeType(
            0, 0, 0x1E2, type_lvalue,
            gcValEntitySoundFrequency::New, 0, 0, 0);
    }
    return type_gcValEntitySoundFrequency;
}
