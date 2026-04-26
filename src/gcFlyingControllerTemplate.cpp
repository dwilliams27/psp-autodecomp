// gcFlyingControllerTemplate::gcFlyingControllerTemplate(cBase *) @ 0x00153d78
// Inherits gcCreatureControllerTemplate; sets vtable and ORs flag bit 1 into +0x18.

class cBase;

class gcCreatureControllerTemplate {
public:
    gcCreatureControllerTemplate(cBase *);
    char _ccPad[0x40];
};

class gcFlyingControllerTemplate : public gcCreatureControllerTemplate {
public:
    gcFlyingControllerTemplate(cBase *);
};

extern char gcFlyingControllerTemplatevirtualtable[];

gcFlyingControllerTemplate::gcFlyingControllerTemplate(cBase *parent)
    : gcCreatureControllerTemplate(parent) {
    *(void **)((char *)this + 4) = gcFlyingControllerTemplatevirtualtable;
    *(unsigned int *)((char *)this + 0x18) |= 1;
}
