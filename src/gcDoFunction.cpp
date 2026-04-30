#include "gcDoEntitySendMessage.h"
#include "cBase.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoFunction;

struct gcDoFunctionData {
    char _pad[0x10];
    gcExpression **mChildren;
};

const cType *gcDoFunction::GetType(void) const {
    if (!type_gcDoFunction) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoFunction = cType::InitializeType(0, 0, 0x73, type_action,
                                                   gcDoFunction::New, 0, 0,
                                                   0x80);
    }
    return type_gcDoFunction;
}

gcExpression *gcDoFunction::GetChild(int index) const {
    gcDoFunctionData *self = (gcDoFunctionData *)this;
    return self->mChildren[index];
}
