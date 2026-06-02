#include "cBase.h"

class cMemPool;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *), const char *, const char *,
                                 unsigned int);
};

struct gcDesiredUIWidgetHelper {
    void *GetDialog(bool) const;
    void *GetWidget(const cType *, bool) const;
};

class gcDoUISetOrigin : public gcAction {
public:
    float Evaluate(void) const;
};

extern const char gcDoUISetOrigin_base_name[] asm("D_0036D894");
extern const char gcDoUISetOrigin_base_desc[] asm("D_0036D89C");

extern "C" cBase *cNamed__New_cMemPoolptr_cBaseptrstatic__001C3C58(cMemPool *, cBase *);

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385E0");
static cType *type_action asm("D_0009990C");

float gcDoUISetOrigin::Evaluate(void) const {
    const gcDesiredUIWidgetHelper *helper =
        (const gcDesiredUIWidgetHelper *)((const char *)this + 0xC);

    if (*(const unsigned char *)((const char *)this + 0x1C) != 0) {
        void *dialog = helper->GetDialog(1);
        if (dialog != 0) {
            *(int *)((char *)dialog + 0x60) = *(const int *)((const char *)this + 0x18);
        }
    } else {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcDoUISetOrigin_base_name,
                                                      gcDoUISetOrigin_base_desc,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 2, type_base,
                    cNamed__New_cMemPoolptr_cBaseptrstatic__001C3C58, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x84, type_expression, 0, 0, 0, 0);
        }
        void *widget = helper->GetWidget(type_action, 1);
        if (widget != 0) {
            *(int *)((char *)widget + 0x4C) = *(const int *)((const char *)this + 0x18);
        }
    }
    return 1.0f;
}
