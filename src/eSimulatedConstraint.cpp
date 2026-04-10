class cBase;

extern char eSimulatedConstraintclassdesc[];

class eSimulatedConstraint {
public:
    eSimulatedConstraint(cBase *parent);
    cBase *base;
    void *vtable;
};

eSimulatedConstraint::eSimulatedConstraint(cBase *parent) {
    base = parent;
    vtable = eSimulatedConstraintclassdesc;
}
