// 0x0030c898, 412B
// gcDoUISetFocus::Evaluate(void) const  ->  __0fOgcDoUISetFocusIEvaluatevK

class cType;
class cBase;
class cMemPool;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

struct gcSubHandle {
    int _h;
    int _gen;
};

class gcUIDialog {
public:
    void Focus(void);
    void FocusControl(const gcSubHandle &);
};

struct gcDesiredUIWidgetHelper {
    gcUIDialog *GetWidget(const cType *, bool) const;
};

class gcAction {
public:
    float Evaluate(void) const;
};

class gcDoUISetFocus : public gcAction {
public:
    float Evaluate(void) const;
};

extern const char gcDoUISetFocus_base_name[];
extern const char gcDoUISetFocus_base_desc[];

static cType *type_base;
static cType *type_expression;
static cType *type_gcDoUISetFocus;

float gcDoUISetFocus::Evaluate(void) const {
    gcDesiredUIWidgetHelper *helper = (gcDesiredUIWidgetHelper *)((char *)this + 0xC);
    if (!type_gcDoUISetFocus) {
        if (!type_expression) {
            if (!type_base) {
                type_base = cType::InitializeType(gcDoUISetFocus_base_name,
                                                  gcDoUISetFocus_base_desc,
                                                  1, 0, 0, 0, 0, 0);
            }
            type_expression = cType::InitializeType(0, 0, 2, type_base,
                                                    cNamed::New, 0, 0, 0);
        }
        type_gcDoUISetFocus = cType::InitializeType(0, 0, 0x84, type_expression,
                                                    0, 0, 0, 0);
    }

    gcUIDialog *widget = helper->GetWidget(type_gcDoUISetFocus, true);
    if (widget == 0) {
        return 0.0f;
    }
    gcUIDialog *ctrl = *(gcUIDialog **)((char *)widget + 0x20);
    if (ctrl == 0) {
        return 0.0f;
    }
    if (*(unsigned char *)((char *)this + 0x18) != 0) {
        ctrl->Focus();
    }
    gcSubHandle *src = (gcSubHandle *)((char *)widget + 0x28);
    gcSubHandle h;
    h._h = src->_h;
    ctrl->FocusControl(h);
    return 1.0f;
}
