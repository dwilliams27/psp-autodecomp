#include "gcDoUIFade.h"
#include "cBase.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct cTypeNode {
    char pad[0x1C];
    const cType *parent;
};

struct VTableSlot {
    short offset;
    short pad;
    const cType *(*getType)(void *);
};

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoUISendMessage;

struct gcDoUISendMessageChildData {
    char _pad[0x30];
    gcExpression **mChildren;
};

gcExpression *gcDoUISendMessage::GetChild(int index) const {
    gcDoUISendMessageChildData *self = (gcDoUISendMessageChildData *)this;
    return self->mChildren[index];
}

void gcDoUISendMessage::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoUISendMessage) {
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
            type_gcDoUISendMessage = cType::InitializeType(
                0, 0, 0x202, type_action, gcDoUISendMessage::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoUISendMessage;
        VTableSlot *slot = (VTableSlot *)((char *)vt + 8);
        short voff = slot->offset;
        const cType *(*getType)(void *) = slot->getType;
        const cType *type = getType((char *)other + voff);
        int ok;

        if (myType == 0) {
            ok = 0;
            goto done;
        }
        if (type != 0) {
        loop:
            if (type == myType) {
                ok = 1;
                goto done;
            }
            type = ((cTypeNode *)type)->parent;
            if (type != 0) {
                goto loop;
            }
        }
        ok = 0;
    done:
        if (ok != 0) {
            copy = other;
        }
    }
    *this = *(const gcDoUISendMessage *)copy;
}
