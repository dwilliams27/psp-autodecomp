// gcValEntityPathDistance - gcAll_psp.obj
//   0x0033A058 GetType(void) const
//   0x0033A170 Write(cFile &) const

#include "cBase.h"

class cFile;
class cMemPool;

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char _pad[0x28];
    cTypeMethod write_m;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDesiredObject {
public:
    int parent;
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

class gcValEntityPathDistance : public gcValue {
public:
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

extern const char gcValEntityPathDistance_base_name[] asm("D_0036D894");
extern const char gcValEntityPathDistance_base_desc[] asm("D_0036D89C");

void gcValEntityPathDistance::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);

    const cTypeMethod *entity_write =
        &((const gcDesiredObject *)((const char *)this + 8))->mType->write_m;
    char *entity_base = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)entity_write->fn)(entity_base + entity_write->offset, wb.file);

    wb.Write(*(const unsigned int *)((const char *)this + 0x34));
    wb.Write(*(const int *)((const char *)this + 0x38));
    wb.Write(*(const int *)((const char *)this + 0x3C));
    ((const gcDesiredValue *)((const char *)this + 0x40))->Write(wb);
    wb.End();
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValEntityPathDistance;

const cType *gcValEntityPathDistance::GetType(void) const {
    if (!type_gcValEntityPathDistance) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcValEntityPathDistance_base_name,
                        gcValEntityPathDistance_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_value = cType::InitializeType(
                0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
        }
        type_gcValEntityPathDistance = cType::InitializeType(
            0, 0, 0xE3, type_value, gcValEntityPathDistance::New, 0, 0, 0);
    }
    return type_gcValEntityPathDistance;
}
