#include "gcDoEntitySendMessage.h"
#include "cBase.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *, cBase *(*)(cMemPool *, cBase *), const char *, const char *, unsigned int);
};

extern const char gcDoEntitySendMessage_base_name[];
extern const char gcDoEntitySendMessage_base_desc[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntitySendMessage;

const cType *gcDoEntitySendMessage::GetType(void) const {
    if (!type_gcDoEntitySendMessage) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcDoEntitySendMessage_base_name, gcDoEntitySendMessage_base_desc, 1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntitySendMessage = cType::InitializeType(0, 0, 0xC0, type_action, gcDoEntitySendMessage::New, 0, 0, 0x80);
    }
    return type_gcDoEntitySendMessage;
}
