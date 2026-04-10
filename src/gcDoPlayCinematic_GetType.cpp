#include "gcDoPlayCinematic.h"
#include "cBase.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *, cBase *(*)(cMemPool *, cBase *), const char *, const char *, unsigned int);
};

extern const char gcDoPlayCinematic_base_name[];
extern const char gcDoPlayCinematic_base_desc[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoPlayCinematic;

const cType *gcDoPlayCinematic::GetType(void) const {
    if (!type_gcDoPlayCinematic) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcDoPlayCinematic_base_name, gcDoPlayCinematic_base_desc, 1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoPlayCinematic = cType::InitializeType(0, 0, 0x16A, type_action, gcDoPlayCinematic::New, 0, 0, 0);
    }
    return type_gcDoPlayCinematic;
}
