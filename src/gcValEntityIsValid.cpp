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
    char _p0[0x28];
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

class gcDesiredEntity : public gcDesiredObject {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcDesiredEntityTemplate : public gcDesiredObject {
public:
    gcDesiredEntityTemplate &operator=(const gcDesiredEntityTemplate &);
};

class gcValue {
public:
    void Write(cFile &) const;
};

class gcValEntityIsValid : public gcValue {
public:
    char _pad0[0x34];
    bool field_34;

    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

gcValEntityIsValid *dcast(const cBase *);

extern const char gcValEntityIsValid_base_name[];
extern const char gcValEntityIsValid_base_desc[];

void gcValEntityIsValid::AssignCopy(const cBase *base) {
    gcValEntityIsValid *other = dcast(base);
    const gcDesiredEntity *otherEntity =
        (const gcDesiredEntity *)((const char *)other + 8);
    __asm__ volatile("" :: "r"(otherEntity) : "memory");
    ((gcDesiredEntity *)((char *)this + 8))->operator=(*otherEntity);
    gcDesiredEntityTemplate *destTemplate =
        (gcDesiredEntityTemplate *)((char *)this + 0x38);
    field_34 = other->field_34;
    destTemplate->operator=(
        *(const gcDesiredEntityTemplate *)((const char *)other + 0x38));
}

void gcValEntityIsValid::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcValue::Write(file);

    const cTypeMethod *entityWrite =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 0x28);
    const char *entityBase = (const char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)entityWrite->fn)((void *)(entityBase + entityWrite->offset), wb.file);

    const cTypeMethod *templateWrite =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 0x38))->mType + 0x28);
    const char *templateBase = (const char *)this + 0x38;
    ((WriteFn)templateWrite->fn)((void *)(templateBase + templateWrite->offset), wb.file);

    wb.Write(field_34);
    wb.End();
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValEntityIsValid;

const cType *gcValEntityIsValid::GetType(void) const {
    if (!type_gcValEntityIsValid) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcValEntityIsValid_base_name,
                                                       gcValEntityIsValid_base_desc,
                                                       1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValEntityIsValid = cType::InitializeType(0, 0, 0xE4, type_value,
                                                        gcValEntityIsValid::New,
                                                        0, 0, 0);
    }
    return type_gcValEntityIsValid;
}
