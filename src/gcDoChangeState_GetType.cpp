#include "gcDoChangeState.h"
#include "cBase.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *, cBase *(*)(cMemPool *, cBase *), const char *, const char *, unsigned int);
};

extern const char gcDoChangeState_base_name[];
extern const char gcDoChangeState_base_desc[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoChangeState;

const cType *gcDoChangeState::GetType(void) const {
    if (!type_gcDoChangeState) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcDoChangeState_base_name, gcDoChangeState_base_desc, 1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoChangeState = cType::InitializeType(0, 0, 0xC2, type_action, gcDoChangeState::New, 0, 0, 0);
    }
    return type_gcDoChangeState;
}
