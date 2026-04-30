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

extern const char gcDoCameraFollowEntity_base_name[];
extern const char gcDoCameraFollowEntity_base_desc[];

class gcDoCameraFollowEntity : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoCameraFollow;
static cType *type_gcDoCameraFollowEntity;

const cType *gcDoCameraFollowEntity::GetType(void) const {
    if (!type_gcDoCameraFollowEntity) {
        if (!type_gcDoCameraFollow) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoCameraFollowEntity_base_name,
                            gcDoCameraFollowEntity_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoCameraFollow = cType::InitializeType(
                0, 0, 0x24F, type_action, 0, 0, 0, 0);
        }
        type_gcDoCameraFollowEntity = cType::InitializeType(
            0, 0, 0x251, type_gcDoCameraFollow,
            gcDoCameraFollowEntity::New, 0, 0, 0);
    }
    return type_gcDoCameraFollowEntity;
}

void gcDoCameraFollowEntity::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    cWriteBlock inner(file, 1);
    gcAction::Write(file);

    {
        char *typeInfo = *(char **)((const char *)this + 0x10);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        short off = rec->offset;
        void *base = (void *)((const char *)this + 0x0C);
        rec->fn((char *)base + off, inner._file);
    }

    ((const gcDesiredValue *)((const char *)this + 0x44))->Write(inner);
    inner.Write(*(const bool *)((const char *)this + 0x48));
    inner.End();

    {
        char *typeInfo = *(char **)((const char *)this + 0x50);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        short off = rec->offset;
        void *base = (void *)((const char *)this + 0x4C);
        rec->fn((char *)base + off, wb._file);
    }

    wb.Write(*(const int *)((const char *)this + 0x78));
    wb.End();
}
