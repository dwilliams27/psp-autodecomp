// gcUIEditBox::gcUIEditBox(cBase *) @ 0x0013a5d8 -- gcAll_psp.obj
// Constructor: chains base gcUITextControl ctor, sets vtable, __vec_new sprite
// handle array, then a few field inits.

class cBase;

class gcUITextControl {
public:
    gcUITextControl(cBase *);
};

class gcUIEditBox : public gcUITextControl {
public:
    gcUIEditBox(cBase *);
};

extern "C" void *__vec_new(void *, int, int, void (*)(void *));
extern char gcUIEditBoxvirtualtable[];
extern int D_0036C7FC;

gcUIEditBox::gcUIEditBox(cBase *parent) : gcUITextControl(parent) {
    *(void **)((char *)this + 4) = gcUIEditBoxvirtualtable;
    *(int *)((char *)this + 0x110) = 0x80000001;
    __vec_new((char *)this + 0x114, 1, 4, (void (*)(void *))0x1E74A4);
    int v = D_0036C7FC;
    *(int *)((char *)this + 0x11C) = 0;
    *(int *)((char *)this + 0x118) = v;
    *(int *)((char *)this + 0x120) = 0;
}
