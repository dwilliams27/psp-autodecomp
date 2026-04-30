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
    short _pad;
    void (*fn)(void *, cFile *);
};

extern const char gcDoCameraLookFromEntityNode_base_name[];
extern const char gcDoCameraLookFromEntityNode_base_desc[];

class gcDoCameraLookFromEntityNode : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoCameraFollowEntity;
static cType *type_gcDoCameraLookFromEntityNode;

const cType *gcDoCameraLookFromEntityNode::GetType(void) const {
    if (!type_gcDoCameraLookFromEntityNode) {
        if (!type_gcDoCameraFollowEntity) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoCameraLookFromEntityNode_base_name,
                            gcDoCameraLookFromEntityNode_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoCameraFollowEntity = cType::InitializeType(
                0, 0, 0x24F, type_action, 0, 0, 0, 0);
        }
        type_gcDoCameraLookFromEntityNode = cType::InitializeType(
            0, 0, 0x252, type_gcDoCameraFollowEntity,
            gcDoCameraLookFromEntityNode::New, 0, 0, 0);
    }
    return type_gcDoCameraLookFromEntityNode;
}

void gcDoCameraLookFromEntityNode::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
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

    ((const cName *)((const char *)this + 0x78))->Write(wb);
    wb.Write(*(const bool *)((const char *)this + 0x94));
    ((const gcDesiredValue *)((const char *)this + 0x90))->Write(wb);
    wb.End();
}
