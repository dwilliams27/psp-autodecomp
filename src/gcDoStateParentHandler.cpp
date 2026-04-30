#include "gcDoReturn.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoStateParentHandler asm("D_0009F700");

int gcDoStateParentHandler::GetExprFlags(void) const {
    return 0xA;
}

const cType *gcDoStateParentHandler::GetType(void) const {
    if (!type_gcDoStateParentHandler) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoStateParentHandler = cType::InitializeType(
            0, 0, 0x2B6, type_action, gcDoStateParentHandler::New, 0, 0, 0x80);
    }
    return type_gcDoStateParentHandler;
}
