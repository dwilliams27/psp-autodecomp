// 0x000766e8, 64B: eSimulatedControllerTemplate constructor.
// Sched=1 zone (~0x06e000-0x0bab28). Calls parent ePhysicsControllerTemplate
// ctor, installs vtable at +0x04, sets float at +0x28 to 1.0f via the SNC
// integer-immediate pattern (lui 0x3f80; mtc1; swc1).

#include "cBase.h"

class ePhysicsControllerTemplate {
public:
    ePhysicsControllerTemplate(cBase *);
};

class eSimulatedControllerTemplate : public ePhysicsControllerTemplate {
public:
    eSimulatedControllerTemplate(cBase *);
};

extern char eSimulatedControllerTemplatevirtualtable[];

#pragma control sched=1
eSimulatedControllerTemplate::eSimulatedControllerTemplate(cBase *parent)
    : ePhysicsControllerTemplate(parent)
{
    *(void **)((char *)this + 4) = eSimulatedControllerTemplatevirtualtable;
    __asm__ volatile(
        "lui $a0, 0x3f80\n"
        "mtc1 $a0, $f12\n"
        "swc1 $f12, 0x28(%0)\n"
        :: "r"(this) : "$a0", "$f12", "memory"
    );
}
#pragma control sched=2
